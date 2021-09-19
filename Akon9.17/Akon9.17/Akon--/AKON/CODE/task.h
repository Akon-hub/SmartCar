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
    
    taskSum,  //��������
}TaskID;

typedef enum
{
    sleep,  //����
    run,    //����
}TaskStatusEnum;

typedef struct
{
    void (*function)(void);         //������ָ��
    TaskStatusEnum status;  //����״̬
    unsigned short period;  //�ܵ��ж�ʱ��Ϊ1ms�����趨���پ��ж�ʱ��Ϊ����
    unsigned short runTime;
    bool isOpen;
}Task;
extern volatile Task task[taskSum];
extern void ClockInterrupt(void);
extern void TaskInit();
extern void CreatTask(unsigned char ID, void (*function)(void), unsigned short Period, bool isOpen);
#endif
