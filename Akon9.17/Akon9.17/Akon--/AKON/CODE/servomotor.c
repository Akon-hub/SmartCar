#include "servomotor.h"


void ServoMotorControl(void)
{
    Cal_weight_average();
      ///////////�������,������ʾ///////////		
    pid.servo.kp1Sum = camServoError *pid.servo.kp1;
    
    pid.servo.kp2Sum = ((camServoError*(80 - length)*(80 - length)*pid.servo.kp2)/1000);
    pid.servo.kpSum = pid.servo.kp1Sum + pid.servo.kp2Sum;
    pid.servo.kdSum = pid.servo.kd*(camServoErrorLast - camServoError);
	
    servo.dutyAdd = pid.servo.kpSum +pid.servo.kdSum;
    //��������
    STATICDIFFERENCE(servo.dutyAdd,8);
    servo.duty = servo.mid + servo.dutyAdd;
    servo.duty = range_protect(servo.duty, servo.mid - 400 , servo.mid + 400);
    
    if(!debug.servo) servo.duty = servo.mid;
    
	pwm_duty(S_MOTOR1_PIN, servo.duty);
}


int8 servo_sum_flag=0;

/**
  * *����ͷ����ƫ���
  **����������еĻ������Կ��Ƕ�̬���ߣ����紨����Ǹ���Զ��Ч����
  **/

float standardMid;    //��׼��������

float camServoError; //���߼�Ȩƫ��
float errorAverge;    //����ƫ��

//ƫ��Ȩ��ֵ
int16 weight[80]={  0,0,0,0,0,0,0,0,0,0, //40�е�49��
                        0,0,0,0,0,5,5,5,5,5, //40�е�49��
                        5,5,5,10,10,10,10,10,10,10,  //60�е�59��
                        10,10,10,10,10,10,10,10,10,10,   //20�е�29��
                        10,10,10,10,10,10,10,10,10,10,   //20�е�29��
                        10,10,10,10,10,10,10,10,10,10,  //40�е�49��
                        10,10,10,10,10,10,10,10,5,5,  //40�е�49��
                        0,0,0,0,0,0,0,0,0,0, //70�е�79��
                     };
int16 weight_circle[80]={ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                        0, 0, 0, 0, 0,10,10,10,10,10,
                       10,10,10,10,10,10,10,10,10,10,
                       10,10,10,10,10,10,10,10,10,10,
                       10,10,10,10,10,10,10,10,10,10,
                       3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
                       0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                       0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                 };
										 
float camServoErrorLast,camServoErrorAdd;
void  Cal_weight_average(void)
{
   uint8 i;
float sum,weight_sum;  //Ȩ�����ߺͣ�Ȩ�غ�
float sum_ave,add_ave; //����Ȩ�����ߺͣ��ӵ����ߵ���      

   standardMid=80;

   for(i=15;i<length;i++)
   {
       if((ELC_Circle_flag==1&&ELC_Circle_turn==1)||(ELC_Circle_flag==1&&ELC_Circle_turn==0))
       {sum+= mid_point[i].x*weight_circle[i] ;      
       weight_sum+=weight_circle[i];}                
       
       
//       else if((forkroad_flag_right ==1
//               ||forkroad_flag_left ==1 ||forkroad_flag_right_out==1
//               ||forkroad_flag_left_out==1) )
//       {
//           sum+= mid_point[i].x*weight[i] ;
//           weight_sum+=weight[i];
//       }
//       else
       {
           sum+= mid_point[i].x*weight[i] ;
           weight_sum+=weight[i];
       }

       if(i>(length*(2/3)))
       {
           sum_ave+= mid_point[i].x;
           add_ave++;
       }
   }
	 camServoErrorLast = camServoError;
   camServoError=standardMid-sum/weight_sum;
	 camServoErrorAdd = camServoError - camServoErrorLast;
   errorAverge   =standardMid-sum_ave/add_ave;
   sum = 0;
   sum_ave = 0;
   add_ave = 0;
}



