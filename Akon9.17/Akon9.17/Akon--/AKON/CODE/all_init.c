
#include "all_init.h"

uint8 GlobalInit()
{
    
    LcdInit();
    ips200_showstr(0,0,"ips200_init");
    ips200_showstr(200,0,"OK");
    ips200_showstr(0,1,"mt9v03x_init :");
//	mt9v03x_csi_init();	//初始化摄像头 使用CSI接口
	ips200_showstr(200,1,"OK");
    pit_init();                     //初始化pit外设
    pit_interrupt_ms(PIT_CH0,1);  //初始化pit通道0 周期
    NVIC_SetPriority(PIT_IRQn,10);  //设置中断优先级 范围0-15 越小优先级越高 四路PIT共用一个PIT中断函数
    ADC_init();
    seekfree_wireless_init();
//	FlashInit();
    DevicesInit();
    DebugParamInit();
    PidParamInit();
    BEEP_ON;
    delay_ms(300);
    BEEP_OFF;
    ips200_clear(IPS200_BGCOLOR);

    TaskInit();

    
    return 1;
    
}    
