#include "control.h"


////////////////////////////////////////////////////////////
//float Cam_Motor_kvff=10;//速度前馈系数15(没啥比用)
//int Cam_Motor_kvff_k=0;//速度前馈系数放大倍数10(没啥比用)
//float Cam_Motor_kaff=0;//加速度前馈系数35(没啥比用)
//int Ackerman_kl=245;//双轮差速242 242 单轮差速251(有点甩尾)  240
//int Ackerman_kr=245;//右转(新差速方案差速系数)跳轮,是差速太小，甩尾是图像有跳变

//int v_want_l=0;
//int v_want_r=0;//本次期望速度
//int v_want_l_last[10];
//int v_want_r_last[10];//上次期望速度
//uint8 Cam_Motor_max=70;
//uint8 Cam_Motor_min=40;//最大最小速度（用于二次函数极值，不是实际值,可动态改变，相当于占空比百分比，不可超过100）
//int32 Cam_Motor_out_L=0;
//int32 Cam_Motor_out_R=0;
//int16 Cam_V_target=0;//当前目标速度
//int16 Cam_V_target_last[5];//上一次目标速度
//uint16 Cam_stop_count=0;//用于出界保护或终点停车计数

/* void Control(void)
{

	   EncoderRead();
//    if(camServoError > 5 )
//    {
//        expectSpeedLeft  = (int)(expectSpeed*(1-((float)ak_l/1000)*tan((float)camServoError*3.14/4600)/0.396));
//        expectSpeedRight = expectSpeed ;
//    }
//    else if(camServoError < -5)
//    {
//        expectSpeedLeft  = expectSpeed;
//        expectSpeedRight = (int)(expectSpeed*(1-((float)motor.right.differential/1000)*tan((float)camServoError*3.14/4600)/0.396)) ;
//    }
//    else 
    {
        expectSpeedLeft  = expectSpeed;
        expectSpeedRight  = expectSpeed;
    }
        
       //左轮
       speedOut  =	SimpleIncremental_PID(&leftMotorSpeedPIDParam[0][0], &speedPIDLeft, expectSpeedLeft, wheel.leftSpeed);
       pwmLeft = speedOut;
       //右轮
       speedOut  =	SimpleIncremental_PID(&rightMotorSpeedPIDParam[0][0], &speedPIDRight, expectSpeedRight, wheel.rightSpeed);
       pwmRight = speedOut;

       pwmLeft = range_protect(pwmLeft,-30000,30000);
       pwmRight = range_protect(pwmRight,-30000,30000);

       PWM_Write();  //给电机赋PWM
}*/

void Cam_Motor_Control(void)//电磁 前馈-微分先行增量式PID 控制电机
{
    //////////////处理偏差///////////////
    pid.leftMotor.errorPrePre = pid.leftMotor.errorPre;
    pid.rightMotor.errorPrePre = pid.rightMotor.errorPre;
//    Cam_Motor_error_pre_pre_L=Cam_Motor_error_pre_L;
//    Cam_Motor_error_pre_pre_R=Cam_Motor_error_pre_R;
    
//    Cam_Motor_error_pre_L=Cam_Motor_error_L;
//    Cam_Motor_error_pre_R=Cam_Motor_error_R;
    pid.leftMotor.errorPre = pid.leftMotor.error;
    pid.rightMotor.errorPre = pid.rightMotor.error;
    
    
    //////////差速控制//////////
     EncoderRead();
   
//    int ak_r,ak_l;
    
    /*if(ELC_Circle_flag==1)//253 253
    {
        ak_r=250;
        ak_l=250;
    }
    else//摄像头差速
    {*/
//        ak_r=Ackerman_kr;
//        ak_l=Ackerman_kl;
    //}
    if(servo.dutyAdd<=-15)//右转
    {
        motor.left.expectSpeed = motor.expectSpeed;
//        expectSpeedLeft=expectSpeed;
        //v_want_l=(int)(Cam_V_target*(1-((float)ak_r/1000)*tan((float)Cam_Servo_add*3.14/4600)/0.396));
        motor.right.expectSpeed=(int)(motor.expectSpeed*(1+((float)motor.right.differential/1000)*tan((float)servo.dutyAdd*3.14/4600)/0.396));
    }
    else if(servo.dutyAdd>=15)//左转
    {
        motor.left.expectSpeed=(int)(motor.expectSpeed*(1-((float)motor.left.differential/1000)*tan((float)servo.dutyAdd*3.14/4600)/0.396));
        motor.right.expectSpeed=motor.expectSpeed;
        //v_want_r=(int)(Cam_V_target*(1+((float)ak_r/1000)*tan((float)Cam_Servo_add*3.14/4600)/0.396));
    }
    else
    {
        motor.left.expectSpeed=motor.expectSpeed;
        motor.right.expectSpeed=motor.expectSpeed;//内轮减速外轮加速确保中轴线速度不变
    }
    pid.leftMotor.error=motor.left.expectSpeed-wheel.left.now;
    pid.rightMotor.error=motor.right.expectSpeed-wheel.right.now;//得到本次各轮速度偏差

    ////////前馈-微分先行增量式PID////////
    
    motor.left.pwm+=(int32)(/*(Cam_Motor_kvff*Cam_Motor_kvff_k*(v_want_l-v_want_l_last[1]))//乘三次之前的速度确保加减速迅速(这前馈没用)
                           +*/pid.leftMotor.ki*pid.leftMotor.error+pid.leftMotor.kp*(pid.leftMotor.error-pid.leftMotor.errorPre)
                           +pid.leftMotor.kd*(pid.leftMotor.error-2*pid.leftMotor.errorPre+pid.leftMotor.errorPrePre)
                           /*-Cam_Motor_kaff*(V_L_Now-V_L_Now_last)*/);
    motor.right.pwm+=(int32)(/*(Cam_Motor_kvff*Cam_Motor_kvff_k*(v_want_r-v_want_r_last[1]))//速度前馈
                           +*/pid.rightMotor.ki*pid.rightMotor.error+pid.rightMotor.kp*(pid.rightMotor.error-pid.rightMotor.errorPre)
                           +pid.rightMotor.kd*(pid.rightMotor.error-2*pid.rightMotor.errorPre+pid.rightMotor.errorPrePre)//增量式PID
                           /*-Cam_Motor_kaff*(V_R_Now-V_R_Now_last)*/);//加速度前馈
    
   
    ///////////对应输出的电机控制///////////
    
    motor.left.pwm = range_protect(motor.left.pwm,-45000,45000);
    motor.right.pwm = range_protect(motor.right.pwm,-45000,45000);

    PWM_Write();  //给电机赋PWM
}