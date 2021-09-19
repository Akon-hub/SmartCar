#include "control.h"


////////////////////////////////////////////////////////////
//float Cam_Motor_kvff=10;//�ٶ�ǰ��ϵ��15(ûɶ����)
//int Cam_Motor_kvff_k=0;//�ٶ�ǰ��ϵ���Ŵ���10(ûɶ����)
//float Cam_Motor_kaff=0;//���ٶ�ǰ��ϵ��35(ûɶ����)
//int Ackerman_kl=245;//˫�ֲ���242 242 ���ֲ���251(�е�˦β)  240
//int Ackerman_kr=245;//��ת(�²��ٷ�������ϵ��)����,�ǲ���̫С��˦β��ͼ��������

//int v_want_l=0;
//int v_want_r=0;//���������ٶ�
//int v_want_l_last[10];
//int v_want_r_last[10];//�ϴ������ٶ�
//uint8 Cam_Motor_max=70;
//uint8 Cam_Motor_min=40;//�����С�ٶȣ����ڶ��κ�����ֵ������ʵ��ֵ,�ɶ�̬�ı䣬�൱��ռ�ձȰٷֱȣ����ɳ���100��
//int32 Cam_Motor_out_L=0;
//int32 Cam_Motor_out_R=0;
//int16 Cam_V_target=0;//��ǰĿ���ٶ�
//int16 Cam_V_target_last[5];//��һ��Ŀ���ٶ�
//uint16 Cam_stop_count=0;//���ڳ��籣�����յ�ͣ������

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
        
       //����
       speedOut  =	SimpleIncremental_PID(&leftMotorSpeedPIDParam[0][0], &speedPIDLeft, expectSpeedLeft, wheel.leftSpeed);
       pwmLeft = speedOut;
       //����
       speedOut  =	SimpleIncremental_PID(&rightMotorSpeedPIDParam[0][0], &speedPIDRight, expectSpeedRight, wheel.rightSpeed);
       pwmRight = speedOut;

       pwmLeft = range_protect(pwmLeft,-30000,30000);
       pwmRight = range_protect(pwmRight,-30000,30000);

       PWM_Write();  //�������PWM
}*/

void Cam_Motor_Control(void)//��� ǰ��-΢����������ʽPID ���Ƶ��
{
    //////////////����ƫ��///////////////
    pid.leftMotor.errorPrePre = pid.leftMotor.errorPre;
    pid.rightMotor.errorPrePre = pid.rightMotor.errorPre;
//    Cam_Motor_error_pre_pre_L=Cam_Motor_error_pre_L;
//    Cam_Motor_error_pre_pre_R=Cam_Motor_error_pre_R;
    
//    Cam_Motor_error_pre_L=Cam_Motor_error_L;
//    Cam_Motor_error_pre_R=Cam_Motor_error_R;
    pid.leftMotor.errorPre = pid.leftMotor.error;
    pid.rightMotor.errorPre = pid.rightMotor.error;
    
    
    //////////���ٿ���//////////
     EncoderRead();
   
//    int ak_r,ak_l;
    
    /*if(ELC_Circle_flag==1)//253 253
    {
        ak_r=250;
        ak_l=250;
    }
    else//����ͷ����
    {*/
//        ak_r=Ackerman_kr;
//        ak_l=Ackerman_kl;
    //}
    if(servo.dutyAdd<=-15)//��ת
    {
        motor.left.expectSpeed = motor.expectSpeed;
//        expectSpeedLeft=expectSpeed;
        //v_want_l=(int)(Cam_V_target*(1-((float)ak_r/1000)*tan((float)Cam_Servo_add*3.14/4600)/0.396));
        motor.right.expectSpeed=(int)(motor.expectSpeed*(1+((float)motor.right.differential/1000)*tan((float)servo.dutyAdd*3.14/4600)/0.396));
    }
    else if(servo.dutyAdd>=15)//��ת
    {
        motor.left.expectSpeed=(int)(motor.expectSpeed*(1-((float)motor.left.differential/1000)*tan((float)servo.dutyAdd*3.14/4600)/0.396));
        motor.right.expectSpeed=motor.expectSpeed;
        //v_want_r=(int)(Cam_V_target*(1+((float)ak_r/1000)*tan((float)Cam_Servo_add*3.14/4600)/0.396));
    }
    else
    {
        motor.left.expectSpeed=motor.expectSpeed;
        motor.right.expectSpeed=motor.expectSpeed;//���ּ������ּ���ȷ���������ٶȲ���
    }
    pid.leftMotor.error=motor.left.expectSpeed-wheel.left.now;
    pid.rightMotor.error=motor.right.expectSpeed-wheel.right.now;//�õ����θ����ٶ�ƫ��

    ////////ǰ��-΢����������ʽPID////////
    
    motor.left.pwm+=(int32)(/*(Cam_Motor_kvff*Cam_Motor_kvff_k*(v_want_l-v_want_l_last[1]))//������֮ǰ���ٶ�ȷ���Ӽ���Ѹ��(��ǰ��û��)
                           +*/pid.leftMotor.ki*pid.leftMotor.error+pid.leftMotor.kp*(pid.leftMotor.error-pid.leftMotor.errorPre)
                           +pid.leftMotor.kd*(pid.leftMotor.error-2*pid.leftMotor.errorPre+pid.leftMotor.errorPrePre)
                           /*-Cam_Motor_kaff*(V_L_Now-V_L_Now_last)*/);
    motor.right.pwm+=(int32)(/*(Cam_Motor_kvff*Cam_Motor_kvff_k*(v_want_r-v_want_r_last[1]))//�ٶ�ǰ��
                           +*/pid.rightMotor.ki*pid.rightMotor.error+pid.rightMotor.kp*(pid.rightMotor.error-pid.rightMotor.errorPre)
                           +pid.rightMotor.kd*(pid.rightMotor.error-2*pid.rightMotor.errorPre+pid.rightMotor.errorPrePre)//����ʽPID
                           /*-Cam_Motor_kaff*(V_R_Now-V_R_Now_last)*/);//���ٶ�ǰ��
    
   
    ///////////��Ӧ����ĵ������///////////
    
    motor.left.pwm = range_protect(motor.left.pwm,-45000,45000);
    motor.right.pwm = range_protect(motor.right.pwm,-45000,45000);

    PWM_Write();  //�������PWM
}