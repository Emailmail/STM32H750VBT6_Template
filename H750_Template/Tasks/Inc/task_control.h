#ifndef __TAKS_CONTROL_H
#define __TAKS_CONTROL_H
#include "pid_controller.h"
extern PID_Controller *speed_pid;
extern PID_Controller *angle_pid;
extern PID_Controller *angvel_pid;
extern float speed;
#endif
