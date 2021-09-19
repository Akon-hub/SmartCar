#ifndef ADC_H
#define ADC_H
#include "headfile.h"

#define FILTERNUM 10
extern void ADC_init();
extern void ADC_calc();


extern uint8 ELC_circle_count;//��������
extern uint8  ELC_Circle_turn;//��������ת����0����δ��
extern uint8  ELC_Circle_flag;//�ݶ��õ���жϻ�����־��������ͷ�ж����һ���
extern uint8  circle_ready_flag;//����ǰ�Ƿ���Ԥ��־λ
extern uint8  ELC_Circle_at;//���ڵ�Ż��������еı�־λ

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
