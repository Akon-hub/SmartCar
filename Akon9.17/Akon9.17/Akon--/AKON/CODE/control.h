#ifndef CONTRAL_H
#define CONTRAL_H

#include "headfile.h"



#define NORMALSPEED 300

#define DIR_1 D0
#define DIR_2 D1
#define PWM_1 PWM2_MODULE3_CHA_D2
#define PWM_2 PWM2_MODULE3_CHB_D3


extern void Cam_Motor_Control(void);//��� ǰ��-΢����������ʽPID ���Ƶ��



extern void Control(void);




#endif
