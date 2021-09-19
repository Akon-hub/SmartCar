#include "adc.h"


uint8 ELC_circle_count=0;//��������
uint8  ELC_Circle_turn=0;//��������ת����0����δ��
uint8  ELC_Circle_flag=0;//�ݶ��õ���жϻ�����־��������ͷ�ж����һ���
uint8  circle_ready_flag=0;//����ǰ�Ƿ���Ԥ��־λ
uint8  ELC_Circle_at=0;//���ڵ�Ż��������еı�־λ

Adc adc;
void AdcParamInit()
{
    //���ܻ��޸�
    adc.l2.max = 0;
    adc.l1.max = 0;
    adc.r1.max = 0;
    adc.r2.max = 0; 
    adc.l2.min = 0;
    adc.l1.min = 0;
    adc.r1.min = 0;
    adc.r2.min = 0;
    //��������
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
	 for( int a=0; a<FILTERNUM - 1; a++)//ÿ��һ���жϸ���һ�����飬����ֵǰ��һλ����һλ��Ĩ�������һλ��������ֵ����
    {
        adc.l2.raw[a]=adc.l2.raw[a+1];
        adc.l1.raw[a]=adc.l1.raw[a+1];
        adc.r1.raw[a]=adc.r1.raw[a+1];
        adc.r2.raw[a]=adc.r2.raw[a+1];
    }
	for(int j=0; j<FILTERNUM - 1; j++)//������ֵ��Сֵ��10*4������45*4�αȽ�
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
    //////////ȥ�����������Сֵ��ȡƽ��//////////
    for(int i=2; i<FILTERNUM - 2; i++)
    {
        adc.l2.sum += adc.l2.raw[i];
        adc.l1.sum += adc.l1.raw[i];
        adc.r1.sum += adc.r1.raw[i];
        adc.r2.sum += adc.r2.raw[i];//�ۼӵ�3-8����
    }
    adc.l2.value=adc.l2.sum/(FILTERNUM - 4);//����
    adc.l1.value=adc.l1.sum/(FILTERNUM - 4);//����
    adc.r1.value=adc.r1.sum/(FILTERNUM - 4);//����
    adc.r2.value=adc.r2.sum/(FILTERNUM - 4);//���� д������ֵ����ʷ�������Ҷ�
    
     adc.l2.sum = 0;
     adc.l1.sum = 0;
     adc.r1.sum = 0;
     adc.r2.sum = 0;//�ۼӵ�3-8����
		
	adc.sum = (adc.l2.value + adc.l1.value + adc.r1.value + adc.r2.value);
	
	if(adc.sum < 4 && debug.stopProtect)
	{
		motor.expectSpeed = 0;
	}
}



void ELC_Flag_Judge(void)
{
      if(ELC_Circle_turn==1)//������������
     {
         if(ELC_Circle_flag==0)//����
         {
             if(ELC_circle_count>100&&((hd_zuo==1&&left_lose_count<=10)||(hd_you==1&&right_lose_count<=10))
                &&((adc.l2.value+adc.l1.value+adc.r1.value+adc.r2.value)<550)     )//�������Ҳ����
             {
                 BEEP_OFF;
//                 run_flag=0;
                 ELC_Circle_turn=0;
                 ELC_circle_count=0;

//                 BEEP_OFF;
             }
         }
         else if(ELC_Circle_flag==1)//����
         {
             if(ELC_circle_count>1500&&ELC_Circle_at==0)
             {
//                 BEEP_NO;
                 ELC_Circle_turn=0;
                 ELC_circle_count=0;

             }
         }
     }

     if( //��һ����� ���뻷��
             (
             (((adc.l2.value+adc.l1.value+adc.r1.value+adc.r2.value)>630)
                     ||((adc.l2.value+adc.l1.value)>=300)||(adc.r1.value+adc.r2.value)>=300)
             &&((adc.l2.value>=100||adc.l1.value>=100||adc.r1.value>=70||adc.r2.value>=100)
              ||(adc.l2.value+adc.l1.value>=300)||(adc.r1.value+adc.r2.value>=200)
             )
             &&ELC_Circle_flag==0&&ELC_Circle_turn==0
             )//����������
//            �ڶ������ ������ ƫ�����ƫ��
             ||(
              (ELC_Circle_flag==1&&ELC_Circle_turn==0)
              /*((length>=60&&(adcL2[9]+adcL1[9]+adc.r1.raw[9]+adc.r2.value)>500    //��һ���ͷ��ɾȥ��&&������ָ��������ע�ͼǵò���
     
     
     
                &&(left_lose_count<=16||right_lose_count<=16))||*/        //angel_count>=325)   �������� ��angel_count>=325ע�͵� 2021.9.6  
                 )
             //��һ������
     )

     {
//         BEEP_NO;
         ELC_Circle_at=1;
         ELC_circle_count=0;
         if(ELC_Circle_flag==0&&ELC_Circle_turn==0)
         {
//             BEEP_NO;
             ELC_Circle_flag=1;
             ELC_Circle_turn=1;//������ʼ���뻷���� �����𱦱�����   ���� :��
//             crossroad_ready = 0;    δʶ��ʮ�� ע�͵� 2021.9.6
//             crossroad = 0;          δʶ��ʮ�� ע�͵� 2021.9.6
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
  