#ifndef _task_h_
#define _task_h_

#include "headfile.h"

typedef enum
{
    motorTask,
    servoTask,
    lcdTask,
    beepTask,
    gyroscopeTask,
    adcTask,
    camTask,
    debugTask,
    keyTask,
    ANODTTask,
		x,
    
    taskSum,  //任务总数
}TaskID;

typedef enum
{
    sleep,  //休眠
    run,    //运行
}TaskStatusEnum;

typedef struct
{
    void (*function)(void);         //任务函数指针
    TaskStatusEnum status;  //运行状态
    unsigned short period;  //总的中断时间为1ms，故设定多少就中断时间为多少
    unsigned short runTime;
    bool isOpen;
}Task;
extern volatile Task task[taskSum];
extern void ClockInterrupt(void);
extern void TaskInit();
extern void CreatTask(unsigned char ID, void (*function)(void), unsigned short Period, bool isOpen);
#endif
