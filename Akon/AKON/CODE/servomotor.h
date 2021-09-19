#ifndef SERVOMOTOR_H
#define SERVOMOTOR_H

#include "headfile.h"

#define S_MOTOR1_PIN   PWM4_MODULE2_CHA_C30



void ServoMotorControl(void);
void  Cal_weight_average(void);

extern float standardMid;    //��׼��������
extern float camServoError; //���߼�Ȩƫ��
extern float errorAverge;    //����ƫ��
extern float camServoErrorAdd;
extern float camServoErrorLast;

extern int8 servo_sum_flag;


#endif
