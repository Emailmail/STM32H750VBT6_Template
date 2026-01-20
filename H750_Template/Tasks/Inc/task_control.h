#ifndef __TAKS_CONTROL_H
#define __TAKS_CONTROL_H
#include "pid_controller.h"
#include "task_attitude.h"
#include "task_motor.h"
#include "task_remote.h"
#include "task_light.h"
extern PID_Controller *speed_pid;
extern PID_Controller *angle_pid;
extern PID_Controller *angvel_pid;
extern PID_Controller *turn_pid;
extern float target_speed;
#endif
