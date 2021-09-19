#ifndef CONTRAL_H
#define CONTRAL_H

#include "headfile.h"



#define NORMALSPEED 300

#define DIR_1 D0
#define DIR_2 D1
#define PWM_1 PWM2_MODULE3_CHA_D2
#define PWM_2 PWM2_MODULE3_CHB_D3


extern void Cam_Motor_Control(void);//电磁 前馈-微分先行增量式PID 控制电机



extern void Control(void);




#endif
