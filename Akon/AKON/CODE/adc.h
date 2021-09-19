#ifndef ADC_H
#define ADC_H
#include "headfile.h"

#define FILTERNUM 10
extern void ADC_init();
extern void ADC_calc();


extern uint8 ELC_circle_count;//环岛计数
extern uint8  ELC_Circle_turn;//进环出环转换，0代表未进
extern uint8  ELC_Circle_flag;//暂定用电磁判断环岛标志，用摄像头判断左右环岛
extern uint8  circle_ready_flag;//进环前是否有预标志位
extern uint8  ELC_Circle_at;//处于电磁环岛环境中的标志位

typedef struct
{
    uint8  raw[FILTERNUM];
    uint8  value;
    uint8  max;
    uint8  min;
    uint16 sum;
}SpecificAdc;

typedef struct
{
    SpecificAdc l2;
    SpecificAdc l1;
    SpecificAdc r1;
    SpecificAdc r2;
    uint16 sum;
}Adc;
extern Adc adc;
#endif
