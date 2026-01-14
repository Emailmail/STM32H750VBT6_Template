#ifndef __TASK_MOTOR_H__
#define __TASK_MOTOR_H__
#include "FreeRTOS.h"
#include "task.h"
#include "tb6612.h"
#include "tim.h"
extern TB6612_Instance *tb6612_motor1;
extern TB6612_Instance *tb6612_motor2;
extern float duty;
#endif