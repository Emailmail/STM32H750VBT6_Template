#include "lsm6dsv.h"

static uint8_t device_index = 0;                           // Allocate 1 ID per device when regisering
static LSM6DSV_Instance *device_instance[LSM6DSV_NUM_MAX]; // The instance addr to the ID
static uint8_t device_num = 0;                             // The number of devices

/* -------------------------------- Driver Function Begin --------------------------------*/
static int32_t stm32_ms6dsv_i2c_write_reg(void *handle, uint8_t reg, const uint8_t *bufp, uint16_t len)
{
    return (int32_t)HAL_I2C_Mem_Write(device_instance[device_index]->hi2c,
                                      LSM6DSV16X_I2C_ADD_L >> 1,
                                      reg,
                                      I2C_MEMADD_SIZE_8BIT,
                                      (uint8_t *)bufp,
                                      len,
                                      HAL_MAX_DELAY);
}

static int32_t stm32_ms6dsv_i2c_read_reg(void *handle, uint8_t reg, uint8_t *bufp, uint16_t len)
{
    return (int32_t)HAL_I2C_Mem_Read(device_instance[device_index]->hi2c,
                                     LSM6DSV16X_I2C_ADD_L >> 1,
                                     reg,
                                     I2C_MEMADD_SIZE_8BIT,
                                     (uint8_t *)bufp,
                                     len,
                                     HAL_MAX_DELAY);
}

stmdev_ctx_t stm32_ctx =
    {
        .write_reg = stm32_ms6dsv_i2c_write_reg,
        .read_reg = stm32_ms6dsv_i2c_read_reg,
        .mdelay = HAL_Delay,
        .handle = (void *)0,
};
/* -------------------------------- Driver Function  End  --------------------------------*/
/* --------------------------------- User Function Begin ---------------------------------*/
LSM6DSV_Instance *LSM6DSV_Register(LSM6DSV_InitTypedef *init)
{
    if (init == NULL)
        return NULL;

    LSM6DSV_Instance *instance = (LSM6DSV_Instance *)malloc(sizeof(LSM6DSV_Instance));
    if (instance == NULL)
        return NULL;
    memset((void *)instance, 0, sizeof(LSM6DSV_Instance));

    instance->device_id = device_num;
    device_instance[device_num] = instance;
    device_num++;
    device_index = device_num - 1;

    instance->hi2c = init->hi2c;
    instance->int_pin = init->int_pin;
    instance->int_port = init->int_port;
    instance->sa0_pin = init->sa0_pin;
    instance->sa0_port = init->sa0_port;

    return instance;
}

int16_t LSM6DSV_Init(LSM6DSV_Instance *instance)
{
    int32_t ret;
    uint8_t lsm6dsv16x_id;
    lsm6dsv16x_reset_t rst;

    device_index = instance->device_id;

    /* 初始化硬件接口 */
    HAL_GPIO_WritePin(instance->sa0_port, instance->sa0_pin, GPIO_PIN_RESET);

    /* 校验ATK-MS6DSV模块ID */
    lsm6dsv16x_device_id_get(&stm32_ctx, &lsm6dsv16x_id);
    if (lsm6dsv16x_id != LSM6DSV16X_ID)
    {
        return 1;
    }

    /* 复位ATK-MS6DSV模块 */
    lsm6dsv16x_reset_set(&stm32_ctx, LSM6DSV16X_RESTORE_CTRL_REGS);
    do
    {
        lsm6dsv16x_reset_get(&stm32_ctx, &rst);
    } while (rst != LSM6DSV16X_READY);

    /* 使能块数据更新功能 */
    lsm6dsv16x_block_data_update_set(&stm32_ctx, PROPERTY_ENABLE);

    /* 配置加速度计和陀螺仪的ODR */
    lsm6dsv16x_xl_data_rate_set(&stm32_ctx, LSM6DSV16X_ODR_AT_60Hz);
    lsm6dsv16x_gy_data_rate_set(&stm32_ctx, LSM6DSV16X_ODR_AT_60Hz);

    /* 配置加速度计和陀螺仪的量程 */
    lsm6dsv16x_xl_full_scale_set(&stm32_ctx, LSM6DSV16X_2g);
    lsm6dsv16x_gy_full_scale_set(&stm32_ctx, LSM6DSV16X_2000dps);

    /* 配置滤波器 */
    lsm6dsv16x_filt_settling_mask_t filt_settling_mask;
    filt_settling_mask.drdy = PROPERTY_ENABLE;
    filt_settling_mask.irq_xl = PROPERTY_ENABLE;
    filt_settling_mask.irq_g = PROPERTY_ENABLE;
    lsm6dsv16x_filt_settling_mask_set(&stm32_ctx, filt_settling_mask);
    lsm6dsv16x_filt_gy_lp1_set(&stm32_ctx, PROPERTY_ENABLE);
    lsm6dsv16x_filt_gy_lp1_bandwidth_set(&stm32_ctx, LSM6DSV16X_GY_ULTRA_LIGHT);
    lsm6dsv16x_filt_xl_lp2_set(&stm32_ctx, PROPERTY_ENABLE);
    lsm6dsv16x_filt_xl_lp2_bandwidth_set(&stm32_ctx, LSM6DSV16X_XL_STRONG);

    return 0;
}

int32_t LSM6DSV_IsBusy(LSM6DSV_Instance *instance)
{
    return lsm6dsv16x_flag_data_ready_get(&stm32_ctx, &instance->drdy);
}

void LSM6DSV_ReadAccel(LSM6DSV_Instance *instance)
{
    int16_t data_raw_acceleration[3];
    lsm6dsv16x_acceleration_raw_get(&stm32_ctx, data_raw_acceleration);
    instance->accel[0] = lsm6dsv16x_from_fs2_to_mg(data_raw_acceleration[0]);
    instance->accel[1] = lsm6dsv16x_from_fs2_to_mg(data_raw_acceleration[1]);
    instance->accel[2] = lsm6dsv16x_from_fs2_to_mg(data_raw_acceleration[2]);
}

void LSM6DSV_ReadGyro(LSM6DSV_Instance *instance)
{
    int16_t data_raw_angular_rate[3];
    lsm6dsv16x_angular_rate_raw_get(&stm32_ctx, data_raw_angular_rate);
    instance->gyro[0] = lsm6dsv16x_from_fs2000_to_mdps(data_raw_angular_rate[0]);
    instance->gyro[1] = lsm6dsv16x_from_fs2000_to_mdps(data_raw_angular_rate[1]);
    instance->gyro[2] = lsm6dsv16x_from_fs2000_to_mdps(data_raw_angular_rate[2]);
}

void LSM6DSV_ReadTemperature(LSM6DSV_Instance *instance)
{
    int16_t data_raw_temperature;
    lsm6dsv16x_temperature_raw_get(&stm32_ctx, &data_raw_temperature);
    instance->temperature = lsm6dsv16x_from_lsb_to_celsius(data_raw_temperature);
}
/* --------------------------------- User Function  End  ---------------------------------*/
