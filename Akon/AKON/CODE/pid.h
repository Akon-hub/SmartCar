#ifndef PID_H
#define PID_H
 

#include "headfile.h"


typedef struct      //位置式pid
{
    float kp;
    float ki;
    float kd;
    float errorPrePre;
    float errorPre;
    float error;
}MotorPid;

typedef struct      //位置式pid
{
    float kp1;
    float kp2;
    float kd;
    float kp1Sum;
    float kp2Sum;
    float kpSum;
    float kdSum;
}ServoPid;

typedef struct      //位置式pid
{
    float Ea = 0;   //隶属度a
		float Eb = 0;		//隶属度b
		int li = 0;
		int j = 0;
}X;

typedef struct      //位置式pid
{
    MotorPid leftMotor;
    MotorPid rightMotor;
    ServoPid servo;
		X x; 
}Pid;
extern Pid pid;
extern void PidParamInit(void);


#endif
