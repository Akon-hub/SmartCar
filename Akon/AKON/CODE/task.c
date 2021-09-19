#include "task.h"

volatile Task task[taskSum];
//----------------------------------------------------------------
//  @brief        对每个任务进行初始化
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
//  @brief        创建任务
//  @param        ID        任务名称，在TaskID枚举里面，添加任务需要在枚举里面补充，放在最后的taskSum之前
//  @param        function  函数名称，这个任务对应的函数
//  @param        Period    执行周期，定时器是1s，所以这个值是多少就是以多少为周期执行
//  @param        isOpen    用来确定是否执行这个任务，例如按键和屏幕等。
//  @return       void
//  Sample usage: CreatTask(motorTask, Cam_Motor_Control, 2, true); 
//----------------------------------------------------------------
void CreatTask(unsigned char ID, void (*function)(void), unsigned short Period, bool isOpen)
{
    if(!task[ID].function)  //判断任务是否被创建
    {
        task[ID].function = function;
        task[ID].status = sleep;
        task[ID].period = Period;
        task[ID].runTime = 0;
        task[ID].isOpen = isOpen;
    }
}
//----------------------------------------------------------------
//  @brief        定时器中,确定是run还是sleep
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
