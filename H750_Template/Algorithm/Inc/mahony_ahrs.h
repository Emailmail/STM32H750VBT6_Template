#ifndef __MahonyAHRS_H
#define __MahonyAHRS_H

#include "FreeRTOS.h"
#include "string.h"
#include "math.h"

#define RAD_TO_DEG (57.29578f)  // 角度值到弧度制

typedef struct // MahonyAHRS实例
{
    float doubleKp;                              // 2倍的Kp增益
    float doubleKi;                              // 2倍的Ki增益
    float q0, q1, q2, q3;                        // 四元数
    float integralFBx, integralFBy, integralFBz; // 累计误差
    float SamplePeriod;                         // 采样周期
    float pitch, roll, yaw;                      // 欧拉角
} MahonyAHRS_Instance;

typedef struct  // MahonyAHRS初始化配置结构体
{
    float Kp;                                    // Kp增益
    float Ki;                                    // Ki增益
    float SamplePeriod;                          // 采样周期(单位s)
} MahonyAHRS_Init_Config_s;

MahonyAHRS_Instance *MahonyAHRS_Register(MahonyAHRS_Init_Config_s *init_config);
void MahonyAHRS_Init(MahonyAHRS_Instance *instance, float ax, float ay, float az);
void MahonyAHRS_Update(MahonyAHRS_Instance *instance, float gx, float gy, float gz, float ax, float ay, float az);
void MahonyAHRS_GetAngles(MahonyAHRS_Instance *instance, float *pitch, float *roll, float *yaw);

#endif
