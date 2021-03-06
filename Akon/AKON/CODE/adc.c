#include "adc.h"


uint8 ELC_circle_count=0;//环岛计数
uint8  ELC_Circle_turn=0;//进环出环转换，0代表未进
uint8  ELC_Circle_flag=0;//暂定用电磁判断环岛标志，用摄像头判断左右环岛
uint8  circle_ready_flag=0;//进环前是否有预标志位
uint8  ELC_Circle_at=0;//处于电磁环岛环境中的标志位

Adc adc;
void AdcParamInit()
{
    //可能会修改
    adc.l2.max = 0;
    adc.l1.max = 0;
    adc.r1.max = 0;
    adc.r2.max = 0; 
    adc.l2.min = 0;
    adc.l1.min = 0;
    adc.r1.min = 0;
    adc.r2.min = 0;
    //基本不变
    uint8 i;
    for(i = 0; i<FILTERNUM; i++)
    {
        adc.l2.raw[i] = 0;
        adc.l1.raw[i] = 0;
        adc.r1.raw[i] = 0;
        adc.r2.raw[i] = 0;
    }
    adc.l2.value = 0;
    adc.l2.sum = 0;
    adc.l1.value = 0;
    adc.l1.sum = 0;
    adc.r1.value = 0;
    adc.r1.sum = 0;
    adc.r2.value = 0;
    adc.r2.sum = 0;
    adc.sum = 0;
}
void ADC_init()
{
	adc_init(ADC_1,ADC1_CH3_B14,ADC_8BIT);
    adc_init(ADC_1,ADC1_CH4_B15,ADC_8BIT);
    adc_init(ADC_1,ADC1_CH10_B21,ADC_8BIT);
    adc_init(ADC_1,ADC1_CH12_B23,ADC_8BIT);
}

void ADC_calc()
{
	uint8 temp = 0;
	for(int i=0;i<FILTERNUM;i++)
	{
        adc.l2.value = 0;
		adc.l2.raw[i] = adc_convert(ADC_1,ADC1_CH12_B23);
		adc.l1.raw[i] = adc_convert(ADC_1,ADC1_CH10_B21);
		adc.r1.raw[i] = adc_convert(ADC_1,ADC1_CH4_B15);
		adc.r2.raw[i] = adc_convert(ADC_1,ADC1_CH3_B14);
        
	}
	 for( int a=0; a<FILTERNUM - 1; a++)//每进一次中断更新一次数组，所有值前移一位，第一位被抹掉，最后一位添加最新值进来
    {
        adc.l2.raw[a]=adc.l2.raw[a+1];
        adc.l1.raw[a]=adc.l1.raw[a+1];
        adc.r1.raw[a]=adc.r1.raw[a+1];
        adc.r2.raw[a]=adc.r2.raw[a+1];
    }
	for(int j=0; j<FILTERNUM - 1; j++)//算出最大值最小值，10*4个数共45*4次比较
    {
        for(int k=0; k<FILTERNUM - 1 - j; k++)
        {
            if(adc.l2.raw[k]>adc.l2.raw[k+1])
            {
                temp=adc.l2.raw[k+1];
                adc.l2.raw[k+1]=adc.l2.raw[k];
                adc.l2.raw[k]=temp;
            }
            if(adc.l1.raw[k]>adc.l1.raw[k+1])
            {
                temp=adc.l1.raw[k+1];
                adc.l1.raw[k+1]=adc.l1.raw[k];
                adc.l1.raw[k]=temp;
            }
            if(adc.r1.raw[k]>adc.r1.raw[k+1])
            {
                temp=adc.r1.raw[k+1];
                adc.r1.raw[k+1]=adc.r1.raw[k];
                adc.r1.raw[k]=temp;
            }
            if(adc.r2.raw[k]>adc.r2.raw[k+1])
            {
                temp=adc.r2.raw[k+1];
                adc.r2.raw[k+1]=adc.r2.raw[k];
                adc.r2.raw[k]=temp;
            }
        }
    }
    //////////去掉两个最大最小值后取平均//////////
    for(int i=2; i<FILTERNUM - 2; i++)
    {
        adc.l2.sum += adc.l2.raw[i];
        adc.l1.sum += adc.l1.raw[i];
        adc.r1.sum += adc.r1.raw[i];
        adc.r2.sum += adc.r2.raw[i];//累加第3-8个数
    }
    adc.l2.value=adc.l2.sum/(FILTERNUM - 4);//最左
    adc.l1.value=adc.l1.sum/(FILTERNUM - 4);//中左
    adc.r1.value=adc.r1.sum/(FILTERNUM - 4);//中右
    adc.r2.value=adc.r2.sum/(FILTERNUM - 4);//最右 写入最新值到历史数组最右端
    
     adc.l2.sum = 0;
     adc.l1.sum = 0;
     adc.r1.sum = 0;
     adc.r2.sum = 0;//累加第3-8个数
		
	adc.sum = (adc.l2.value + adc.l1.value + adc.r1.value + adc.r2.value);
	
	if(adc.sum < 4 && debug.stopProtect)
	{
		motor.expectSpeed = 0;
	}
}



void ELC_Flag_Judge(void)
{
      if(ELC_Circle_turn==1)//环岛计数清零
     {
         if(ELC_Circle_flag==0)//出环
         {
             if(ELC_circle_count>100&&((hd_zuo==1&&left_lose_count<=10)||(hd_you==1&&right_lose_count<=10))
                &&((adc.l2.value+adc.l1.value+adc.r1.value+adc.r2.value)<550)     )//遇到弯道也清零
             {
                 BEEP_OFF;
//                 run_flag=0;
                 ELC_Circle_turn=0;
                 ELC_circle_count=0;

//                 BEEP_OFF;
             }
         }
         else if(ELC_Circle_flag==1)//进环
         {
             if(ELC_circle_count>1500&&ELC_Circle_at==0)
             {
//                 BEEP_NO;
                 ELC_Circle_turn=0;
                 ELC_circle_count=0;

             }
         }
     }

     if( //第一种情况 进入环岛
             (
             (((adc.l2.value+adc.l1.value+adc.r1.value+adc.r2.value)>630)
                     ||((adc.l2.value+adc.l1.value)>=300)||(adc.r1.value+adc.r2.value)>=300)
             &&((adc.l2.value>=100||adc.l1.value>=100||adc.r1.value>=70||adc.r2.value>=100)
              ||(adc.l2.value+adc.l1.value>=300)||(adc.r1.value+adc.r2.value>=200)
             )
             &&ELC_Circle_flag==0&&ELC_Circle_turn==0
             )//第三个条件
//            第二种情况 出环岛 偏左或者偏右
             ||(
              (ELC_Circle_flag==1&&ELC_Circle_turn==0)
              /*((length>=60&&(adcL2[9]+adcL1[9]+adc.r1.raw[9]+adc.r2.value)>500    //这一行最开头处删去了&&，如果恢复了下面的注释记得补上
     
     
     
                &&(left_lose_count<=16||right_lose_count<=16))||*/        //angel_count>=325)   无陀螺仪 把angel_count>=325注释掉 2021.9.6  
                 )
             //第一个条件
     )

     {
//         BEEP_NO;
         ELC_Circle_at=1;
         ELC_circle_count=0;
         if(ELC_Circle_flag==0&&ELC_Circle_turn==0)
         {
//             BEEP_NO;
             ELC_Circle_flag=1;
             ELC_Circle_turn=1;//代表开始进入环岛咯 开心吗宝贝儿？   开心 :）
//             crossroad_ready = 0;    未识别十字 注释掉 2021.9.6
//             crossroad = 0;          未识别十字 注释掉 2021.9.6
//             hd_zuo = 1;
//             run_flag=0;
         }
         else if(ELC_Circle_flag==1&&ELC_Circle_turn==0)
         {
             ELC_Circle_flag=0;
             ELC_Circle_turn=1;

//             circle_flag = 1;
//             BEEP_OFF;
//             BEEP_NO;
         }
     }
     else
         ELC_Circle_at=0;
}
  