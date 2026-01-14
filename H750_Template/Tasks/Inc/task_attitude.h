#ifndef __TASK_ATTITUDE_H
#define __TASK_ATTITUDE_H 
#include "FreeRTOS.h"
#include "task.h"
#include "lsm6dsv.h"
#include "i2c.h"
#include "mahony_ahrs.h"

#define GYROX_OFFSET 0.0065f
#define GYROY_OFFSET -0.0028f
#define GYROZ_OFFSET 0.0070f

extern LSM6DSV_Instance *lsm6dsv;
extern MahonyAHRS_Instance *mahony_ahrs;
extern float pitch, roll, yaw;

#endif