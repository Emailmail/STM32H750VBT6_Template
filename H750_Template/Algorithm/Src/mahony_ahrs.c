#include "mahony_ahrs.h"

/**
 * @brief 求开方的倒数值
 * @param x 输入数据
 * @retval 结果
 */
static float Mahony_invSqrt(float x)
{
    float halfx = 0.5f * x;
    float y = x;
    long i = *(long *)&y;
    i = 0x5f3759df - (i >> 1);
    y = *(float *)&i;
    y = y * (1.5f - (halfx * y * y));
    y = y * (1.5f - (halfx * y * y));
    return y;
}

/**
 * @brief MahonyAHRS实例注册
 * @param init_config 初始化配置结构体
 * @retval 实例
 */
MahonyAHRS_Instance *MahonyAHRS_Register(MahonyAHRS_Init_Config_s *init_config)
{
    if(init_config == NULL)
        return NULL;
    else if(init_config->Kp < 0.0f || init_config->Ki < 0.0f || init_config->SamplePeriod <= 0.0f)
        return NULL;

    MahonyAHRS_Instance *instance = (MahonyAHRS_Instance *)pvPortMalloc(sizeof(MahonyAHRS_Instance));
    if(instance == NULL)
        return NULL;
    memset(instance, 0, sizeof(MahonyAHRS_Instance));

    instance->doubleKp = 2 * init_config->Kp;
    instance->doubleKi = 2 * init_config->Ki;
    instance->SamplePeriod = init_config->SamplePeriod;

    return instance;
}

/**
 * @brief MahonyAHRS上电后快速初始化
 * @param instance 实例
 * @param ax x轴加速度
 * @param ay y轴加速度
 * @param az z轴加速度
 */
void MahonyAHRS_Init(MahonyAHRS_Instance *instance, float ax, float ay, float az)
{
    float recipNorm;
    float init_yaw, init_pitch, init_roll;
    float cr2, cp2, cy2, sr2, sp2, sy2;
    float sin_roll, cos_roll, sin_pitch, cos_pitch;
    float magX, magY;

    recipNorm = Mahony_invSqrt(ax * ax + ay * ay + az * az);
    ax *= recipNorm;
    ay *= recipNorm;
    az *= recipNorm;

    init_pitch = atan2f(-ax, az);
    init_roll = atan2f(ay, az);
    init_yaw = 0.0f;

    sin_roll = sinf(init_roll);
    cos_roll = cosf(init_roll);
    cos_pitch = cosf(init_pitch);
    sin_pitch = sinf(init_pitch);

    cr2 = cosf(init_roll * 0.5f);
    cp2 = cosf(init_pitch * 0.5f);
    cy2 = cosf(init_yaw * 0.5f);
    sr2 = sinf(init_roll * 0.5f);
    sp2 = sinf(init_pitch * 0.5f);
    sy2 = sinf(init_yaw * 0.5f);

    instance->q0 = cr2 * cp2 * cy2 + sr2 * sp2 * sy2;
    instance->q1 = sr2 * cp2 * cy2 - cr2 * sp2 * sy2;
    instance->q2 = cr2 * sp2 * cy2 + sr2 * cp2 * sy2;
    instance->q3 = cr2 * cp2 * sy2 - sr2 * sp2 * cy2;

    recipNorm = Mahony_invSqrt(instance->q0 * instance->q0 + instance->q1 * instance->q1 + instance->q2 * instance->q2 + instance->q3 * instance->q3);

    instance->q0 *= recipNorm;
    instance->q1 *= recipNorm;
    instance->q2 *= recipNorm;
    instance->q3 *= recipNorm;
}

/**
 * @brief MahonyAHRS更新四元数
 * @param instance 实例
 * @param gx 角速度x轴
 * @param gy 角速度y轴
 * @param gz 角速度z轴
 * @param ax 加速度x轴
 * @param ay 加速度y轴
 * @param az 加速度z轴
 */
void MahonyAHRS_Update(MahonyAHRS_Instance *instance, float gx, float gy, float gz, float ax, float ay, float az)
{
    float recipNorm;
    float halfvx, halfvy, halfvz;
    float halfex, halfey, halfez;
    float qa, qb, qc;

    /* Normalise accelerometer measurement */
    recipNorm = Mahony_invSqrt(ax * ax + ay * ay + az * az);
    ax *= recipNorm;
    ay *= recipNorm;
    az *= recipNorm;

    /* Estimated direction of gravity and vector perpendicular to magnetic flux */
    halfvx = instance->q1 * instance->q3 - instance->q0 * instance->q2;
    halfvy = instance->q0 * instance->q1 + instance->q2 * instance->q3;
    halfvz = instance->q0 * instance->q0 - 0.5f + instance->q3 * instance->q3;

    /* Error is sum of cross product between estimated and measured direction of gravity */
    halfex = (ay * halfvz - az * halfvy);
    halfey = (az * halfvx - ax * halfvz);
    halfez = (ax * halfvy - ay * halfvx);

    /* Compute and apply integral feedback if enabled */
    instance->integralFBx += instance->doubleKi * halfex * instance->SamplePeriod; // integral error scaled by Ki
    instance->integralFBy += instance->doubleKi * halfey * instance->SamplePeriod;
    instance->integralFBz += instance->doubleKi * halfez * instance->SamplePeriod;
    gx += instance->integralFBx; // apply integral feedback
    gy += instance->integralFBy;
    gz += instance->integralFBz;

    /* Apply proportional feedback */
    gx += instance->doubleKp * halfex;
    gy += instance->doubleKp * halfey;
    gz += instance->doubleKp * halfez;

    /* Integrate rate of change of quaternion */
    gx *= (0.5f * instance->SamplePeriod); // pre-multiply common factors
    gy *= (0.5f * instance->SamplePeriod);
    gz *= (0.5f * instance->SamplePeriod);
    qa = instance->q0;
    qb = instance->q1;
    qc = instance->q2;
    instance->q0 += (-qb * gx - qc * gy - instance->q3 * gz);
    instance->q1 += (qa * gx + qc * gz - instance->q3 * gy);
    instance->q2 += (qa * gy - qb * gz + instance->q3 * gx);
    instance->q3 += (qa * gz + qb * gy - qc * gx);

    /* Normalise quaternion */
    recipNorm = Mahony_invSqrt(instance->q0 * instance->q0 + instance->q1 * instance->q1 + instance->q2 * instance->q2 + instance->q3 * instance->q3);
    instance->q0 *= recipNorm;
    instance->q1 *= recipNorm;
    instance->q2 *= recipNorm;
    instance->q3 *= recipNorm;
}

/**
 * @brief MahonyAHRS获取角度
 * @param instance 实例
 * @param roll 横滚角
 * @param pitch 俯仰角
 * @param yaw 偏航角
 */
void MahonyAHRS_GetAngles(MahonyAHRS_Instance *instance, float *pitch, float *roll, float *yaw)
{
    *pitch = atan2f(instance->q0 * instance->q1 + instance->q2 * instance->q3, 0.5f - instance->q1 * instance->q1 - instance->q2 * instance->q2);
    instance->pitch = *pitch *= RAD_TO_DEG;
    instance->roll = *roll = RAD_TO_DEG * asinf(-2.0f * (instance->q1 * instance->q3 - instance->q0 * instance->q2));
    *yaw = atan2f(instance->q1 * instance->q2 + instance->q0 * instance->q3, 0.5f - instance->q2 * instance->q2 - instance->q3 * instance->q3);
    instance->yaw = *yaw *= RAD_TO_DEG;
}
