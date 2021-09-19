#include "task.h"

volatile Task task[taskSum];
//----------------------------------------------------------------
//  @brief        ��ÿ��������г�ʼ��
//  @param        void
//  @return       void
//  Sample usage: TaskInit(void); 
//----------------------------------------------------------------
void TaskInit()
{
    unsigned char i;
    for(i = 0; i < taskSum; i++)
    {
        task[i].function = 0;
        task[i].status = sleep;
        task[i].period = 0;
        task[i].runTime = 0;
    }
}
//----------------------------------------------------------------
//  @brief        ��������
//  @param        ID        �������ƣ���TaskIDö�����棬���������Ҫ��ö�����油�䣬��������taskSum֮ǰ
//  @param        function  �������ƣ���������Ӧ�ĺ���
//  @param        Period    ִ�����ڣ���ʱ����1s���������ֵ�Ƕ��پ����Զ���Ϊ����ִ��
//  @param        isOpen    ����ȷ���Ƿ�ִ������������簴������Ļ�ȡ�
//  @return       void
//  Sample usage: CreatTask(motorTask, Cam_Motor_Control, 2, true); 
//----------------------------------------------------------------
void CreatTask(unsigned char ID, void (*function)(void), unsigned short Period, bool isOpen)
{
    if(!task[ID].function)  //�ж������Ƿ񱻴���
    {
        task[ID].function = function;
        task[ID].status = sleep;
        task[ID].period = Period;
        task[ID].runTime = 0;
        task[ID].isOpen = isOpen;
    }
}
//----------------------------------------------------------------
//  @brief        ��ʱ����,ȷ����run����sleep
//  @param        void
//  @return       void
//  Sample usage: ClockInterrupt(void); 
//----------------------------------------------------------------
void ClockInterrupt(void)
{
    unsigned char i;
    for(i = 0; i < taskSum ; i++)
    {
        if(task[i].function && task[i].isOpen)
        {
            task[i].runTime++;
            if(task[i].runTime >= task[i].period)
            {
                task[i].runTime = 0;
                task[i].status = run;
            }
        }
    }
}
