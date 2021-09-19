#include "servomotor.h"


void ServoMotorControl(void)
{
    Cal_weight_average();
      ///////////参数求和,方便显示///////////		
    pid.servo.kp1Sum = camServoError *pid.servo.kp1;
    
    pid.servo.kp2Sum = ((camServoError*(80 - length)*(80 - length)*pid.servo.kp2)/1000);
    pid.servo.kpSum = pid.servo.kp1Sum + pid.servo.kp2Sum;
    pid.servo.kdSum = pid.servo.kd*(camServoErrorLast - camServoError);
	
    servo.dutyAdd = pid.servo.kpSum +pid.servo.kdSum;
    //消除静差
    STATICDIFFERENCE(servo.dutyAdd,8);
    servo.duty = servo.mid + servo.dutyAdd;
    servo.duty = range_protect(servo.duty, servo.mid - 400 , servo.mid + 400);
    
    if(!debug.servo) servo.duty = servo.mid;
    
	pwm_duty(S_MOTOR1_PIN, servo.duty);
}


int8 servo_sum_flag=0;

/**
  * *摄像头中线偏差处理
  **后期如果可行的话，可以考虑动态中线，比如川大的那个最远有效点列
  **/

float standardMid;    //标准中线列数

float camServoError; //中线加权偏差
float errorAverge;    //中线偏差

//偏差权重值
int16 weight[80]={  0,0,0,0,0,0,0,0,0,0, //40行到49行
                        0,0,0,0,0,5,5,5,5,5, //40行到49行
                        5,5,5,10,10,10,10,10,10,10,  //60行到59行
                        10,10,10,10,10,10,10,10,10,10,   //20行到29行
                        10,10,10,10,10,10,10,10,10,10,   //20行到29行
                        10,10,10,10,10,10,10,10,10,10,  //40行到49行
                        10,10,10,10,10,10,10,10,5,5,  //40行到49行
                        0,0,0,0,0,0,0,0,0,0, //70行到79行
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
float sum,weight_sum;  //权重中线和，权重和
float sum_ave,add_ave; //不带权重中线和，加的中线点数      

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



