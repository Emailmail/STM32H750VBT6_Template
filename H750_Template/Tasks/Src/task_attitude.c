#include "task_attitude.h"

LSM6DSV_Instance *lsm6dsv;
MahonyAHRS_Instance *mahony_ahrs;
float pitch, roll, yaw;

#define GYROX_OFFSET 0.0065f
#define GYROY_OFFSET -0.0028f
#define GYROZ_OFFSET 0.0070f

void StartAttitudeTask(void *argument)
{
    static TickType_t xLastWakeTime = 0;
    const TickType_t xPeriod = pdMS_TO_TICKS(5);
    xLastWakeTime = xTaskGetTickCount();

    /* Register and Init LSM6DSV */
    LSM6DSV_InitTypedef init = 
    {
        .hi2c = &hi2c1,
        .int_pin = GPIO_PIN_7,
        .int_port = GPIOB,
        .sa0_pin = GPIO_PIN_4,
        .sa0_port = GPIOB,
    };
    lsm6dsv = LSM6DSV_Register(&init);
    LSM6DSV_Init(lsm6dsv);
    vTaskDelay(pdMS_TO_TICKS(50));

    /* Register and Init MahonyAHRS */
    MahonyAHRS_Init_Config_s mahony_init = 
    {
        .Kp = 0.5f,
        .Ki = 0.005f,
        .SamplePeriod = 0.005f
    };
    mahony_ahrs = MahonyAHRS_Register(&mahony_init);
    LSM6DSV_ReadData(lsm6dsv);
    MahonyAHRS_Init(mahony_ahrs, lsm6dsv->accel[0], lsm6dsv->accel[1], lsm6dsv->accel[2]);

    for (;;)
    {
        /* Read IMU Data and Calculate Attitude (Pitch, Roll, Yaw) */
        LSM6DSV_ReadData(lsm6dsv);
        MahonyAHRS_Update(  mahony_ahrs, 
                            lsm6dsv->gyro[0] - GYROX_OFFSET, 
                            lsm6dsv->gyro[1] - GYROY_OFFSET, 
                            lsm6dsv->gyro[2] - GYROZ_OFFSET, 
                            lsm6dsv->accel[0], 
                            lsm6dsv->accel[1], 
                            lsm6dsv->accel[2]);
        MahonyAHRS_GetAngles(mahony_ahrs, &pitch, &roll, &yaw);

        vTaskDelayUntil(&xLastWakeTime, xPeriod);
    }
}
