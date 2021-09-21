#include "lcd.h"
/*
ips��ʾ����  Һ����ʾ�ַ�   ips200_showchar(uint16 x,uint16 y,const int8 dat)   ips200_showchar(0,0,'x');//����0,0дһ���ַ�x
               Һ����ʾ�ַ��� ips200_showstr(uint16 x,uint16 y,const int8 dat[])  ips200_showstr(0,0,"seekfree");   
               Һ����ʾ8λ�з��� ips200_showint8(uint16 x,uint16 y,int8 dat)       ips200_showint8(0,0,x);//xΪint8����
               Һ����ʾ������(ȥ������������Ч��0) ips200_showfloat(uint16 x,uint16 y,double dat,int8 num,int8 pointnum) ips_showfloat(0,0,x,2,3);//��ʾ������ 
NOTION:��ɫ�Ǵ�д�ģ�eg����ɫ RED
*/



Lcd lcd;

void LcdInit()
{
    ips200_init();			//��ʼ��2.0��IPS��Ļ
    
    lcd.page = 0;
    lcd.cursor = 0;
}

uint8 GetKeyState(void)  //��ȡ����Ŀǰ��״̬
{
    if( gpio_get(DIALSWITCH2) == 0 )
    {
        return 1;
    }
    else if( gpio_get(DIALSWITCH2) == 1 )
    {
        return 2;
    }
    return 0;
}
//----------------------------------------------------------------
//  @brief        ��ȡĿǰ���뿪��״̬
//  @param        void
//  @return       void
//  Sample usage: KeyState(); 
//----------------------------------------------------------------
//Statement
LcdStateEnum keyState;  //Ŀǰ��Ļ���뿪��״̬
//Function
void KeyState()  
{
    if(GetKeyState() == 1 )  keyState = Select;
    else if(GetKeyState() == 2 )  keyState = Change;
}


void CursorMove(uint8 direction, int8 moveDistance) //����ƶ�
{
    if( direction == Up)
    {
        lcd.cursor += moveDistance;
    }
    else if( direction == Down)
    {
        lcd.cursor -= moveDistance;
    }
}
//----------------------------------------------------------------
//  @brief        ����
//  @param        void
//  @return       void
//  Sample usage: UpAndDown(); 
//----------------------------------------------------------------
void UpAndDown()  
{
    if(key.up == onePress || key.up == alwaysPress)
    {
        key.up = noPress;
        CursorMove(Up, 1);
        ips200_clear(IPS200_BGCOLOR);
    }
    else if(key.down == onePress || key.down == alwaysPress)
    {
        key.down = noPress;
        CursorMove(Down, 1);
        ips200_clear(IPS200_BGCOLOR);
    }
    lcd.cursor = range_protect(lcd.cursor, 0, 15);
}

void LcdPageZero()  //Ŀ¼
{
    ips200_showstr(80, 0, "CATALOG");
    
    ips200_showstr(10, 1, "Page One   - LeftMotorPID");
    ips200_showstr(10, 2, "Page Two   - SwitchFunction");
    ips200_showstr(10, 3, "Page Three - ServoPID");
    ips200_showstr(10, 4, "Page Four  - CamPicture");
    ips200_showstr(10, 5, "Page Five  - ELC");
    ips200_showstr(10, 6, "Page Six   - Gyroscope");
    ips200_showstr(10, 7, "Page Seven - Circle");
		ips200_display_chinese(10,8*LINEWIDTH,16,chinese[0],5,IPS200_PENCOLOR);
    
}
void LcdPageOne()   //��һҳ�����PID����Ϣ
{
    ips200_showstr(80, 0, "Page One");
    
    ips200_showstr(10, 1, "LeftMotorKP");
    ips200_showfloat(130, 1, pid.leftMotor.kp, 3, 2);
    ips200_showstr(10, 2, "LeftMotorKi");
    ips200_showfloat(130, 2, pid.leftMotor.ki, 3, 2);
    ips200_showstr(10, 3, "LeftMotorKd");
    ips200_showfloat(130, 3, pid.leftMotor.kd, 3, 2);
    
    ips200_showstr(10, 4, "RightMotorKp");
    ips200_showfloat(130, 4, pid.rightMotor.kp, 3, 2);
    ips200_showstr(10, 5, "RightMotorKi");
    ips200_showfloat(130, 5, pid.rightMotor.ki, 3, 2);
    ips200_showstr(10, 6, "RightMotorKd");
    ips200_showfloat(130, 6, pid.rightMotor.kd, 3, 2);
    
    
    ips200_showstr(10, 7, "ExpectSpeed");
    ips200_showuint16(130, 7, motor.expectSpeed);
    ips200_showstr(10, 8, "RightWheelNow");
    ips200_showuint16(130, 8, wheel.right.now);
    ips200_showstr(10, 9, "LeftWheelNow");
    ips200_showuint16(130, 9, wheel.left.now);
    
    ips200_showstr(10, 10, "ExpectSpeedLeft");
    ips200_showint16(130, 10, motor.left.expectSpeed);
    ips200_showstr(10, 11, "ExpectSpeedRight");
    ips200_showint16(130, 11, motor.right.expectSpeed);
    
    ips200_showstr(10, 12, "motor.left.differential");
    ips200_showuint16(130, 12, motor.left.differential);
    ips200_showstr(10, 13, "RightDifferential");
    ips200_showuint16(130, 13, motor.right.differential);
}
void LcdPageTwo()  //�ڶ�ҳ��Function����
{
    ips200_showstr(80, 0, "Page Two");
    ips200_showstr(10, 1, "ServoOpen");
    if(debug.servo)  ips200_showstr(130, 1, "True");
    else ips200_showstr(130, 1, "False");
    
    ips200_showstr(10, 2, "StopProtectOpen");
    if(debug.stopProtect)  ips200_showstr(130, 2, "True");
    else ips200_showstr(130, 2, "Flase");
    
}
void LcdPageThree() //����ҳ�����PID
{
    ips200_showstr(80, 0, "Page Three");
    
    ips200_showstr(10, 1, "ServoKp1");
    ips200_showint32(130, 1, pid.servo.kp1, 2);
    ips200_showstr(10, 2, "ServoKp2");
    ips200_showint32(130, 2, pid.servo.kp2, 2);
    ips200_showstr(10, 3, "ServoKd");
    ips200_showint32(130, 3, pid.servo.kd,  2);
}
void LcdPageFour()  //����ҳ��ʾ����
{
    Display_Pitcure();
    ips200_showstr(10, 5, "Threshold");
    ips200_showuint16(130, 5, Threshold);
    ips200_showstr(10, 6, "const camServoError");
    ips200_showfloat(130, 6, camServoError, 3, 2);
    ips200_showstr(10, 7, "ServoPwmNow");
    ips200_showuint16(130, 7, servo.duty - servo.mid);
}
void LcdPageFive()  //����ҳ��ʾ���� ���ֵ
{
    ips200_showstr(80, 0, "Page Five");
    
    ips200_showstr(10, 1, "L2");
    ips200_showuint8(130, 1, adc.l2.value);
    ips200_showstr(10, 2, "L1");
    ips200_showuint8(130, 2, adc.l1.value);
    ips200_showstr(10, 3, "R1");
    ips200_showuint8(130, 3, adc.r1.value);
    ips200_showstr(10, 4, "R2");
    ips200_showuint8(130, 4, adc.r2.value);
    ips200_showstr(10, 5, "Sum");
    ips200_showuint16(130, 5, adc.sum);
    
    
}
void LcdPageSix()   //����ҳ������
{
    ips200_showstr(80, 0, "Page Six");
    
    ips200_showstr(10, 1, "Gyro X");
    ips200_showfloat(130, 1, IMC_Treated.gyro.x, 3, 2);
    ips200_showstr(10, 2, "Gyro Y");
    ips200_showfloat(130, 2, IMC_Treated.gyro.y, 3, 2);
    ips200_showstr(10, 3, "Gyro Z");
    ips200_showfloat(130, 3, IMC_Treated.gyro.z, 3, 2);
    ips200_showstr(10, 4, "Circle_angle");
    ips200_showint16(130, 4, Circle_angle);
    ips200_showstr(10, 5, "Circle_angle0");
    ips200_showint32(130, 5, Circle_angle0, 5);

}



void LcdPageSeven() //����ҳ������
{
    ips200_showstr(80, 0, "Page Seven");
    
    ips200_showstr(10, 1, "CirclePositionLeft");
    ips200_showuint8(130, 1, hd_zuo);
    ips200_showstr(10, 2, "CirclePositionRight");
    ips200_showuint8(130, 2, hd_you);
    ips200_showstr(10, 3, "ELC_Circle_flag");
    ips200_showuint8(130, 3, ELC_Circle_flag);
    ips200_showstr(10, 4, "ELC_circle_count");
    ips200_showuint8(130, 4, ELC_circle_count);
    ips200_showstr(10,   5, "ELC_Circle_turn");
    ips200_showuint8(130, 5, ELC_Circle_turn);
}

void LcdPageEight() //�ڰ�ҳ
{
    ips200_showstr(80, 0, "Page Eight");
}
void KEYSTATESelect()   //ÿҳ������ʾ
{
    switch(lcd.page)
    {
        case 0:
            LcdPageZero();  //Ŀ¼
            break;
        case 1:
            LcdPageOne();   //���PID
            break;
        case 2:
            LcdPageTwo();   //���ر���
            break;
        case 3:
            LcdPageThree(); //���PID
            break;
        case 4:
            LcdPageFour();  //����ͷͼ��
            break;
        case 5:
            LcdPageFive();  //���ֵ
            break;
        case 6:
            LcdPageSix();   //������
            break;
        case 7:
            LcdPageSeven(); //����
            break;
        case 8:
            LcdPageEight();
            break;
				default:
						break;
    }
}

void ConfirmAndBack()   //ȷ�ϼ��ͷ��ؼ�
{
    if(key.left == onePress || key.left == alwaysPress)
    {
        key.left = noPress;
        switch(lcd.page)
        {
            //�ڵ�һҳ�Ļ�����ת������ҳ��
            case 0:
                lcd.page = lcd.cursor;
                break;
            case 1:
            case 2:
            case 3:
                FlashWrite();
                FlashRead();
                break;
						default:
                break;
                
        }
        ips200_clear(IPS200_BGCOLOR); 
        KEYSTATESelect();
    }
    else if(key.right == onePress || key.right == alwaysPress)
    {
        key.right = noPress;
        lcd.page = 0;
        ips200_clear(IPS200_BGCOLOR);
        KEYSTATESelect();
    }
}



//��һ����ͷ��x��y����������һ������ʼλ��
void ShowArrow(uint16 x, uint16 y, uint16 color, uint8 direction) 
{
    int i;
    y = y*16;
    for(i = 0; i < ARROWLENGTH; i++)  //����ͷ�ĺ���
    {
        ips200_drawpoint(x+i, y+ARROWWIDTH/2, color);
    }
    switch(direction)
    {
        case Right:
            for(i = 0; i < ARROWWIDTH/2; i++)//����ͷ������б��
            { 
                ips200_drawpoint(x+i+ARROWWIDTH/2, y+i, color);
                ips200_drawpoint(x+i+ARROWWIDTH/2, y-i+ARROWWIDTH, color);
            }
            break;
        case Left:
            for(i = 0; i < ARROWWIDTH/2; i++)//����ͷ������б��
            { 
                ips200_drawpoint(x+i, y-i+ARROWWIDTH/2, color);
                ips200_drawpoint(x+i, y+i+ARROWWIDTH/2, color);
            }
            break;
        default:
            BEEP_ON;
						while(1)
						{
							PRINTF("switch(direction) in ShowArrow(uint16 x, uint16 y, uint16 color, uint8 direction) Function Error");
						}
            break;
    }
}
void ChangeParamInt16(int16 *Param) //�����޸�ģʽ�е��޸�int16��
{
	if(Param)
	{
    if(key.up == onePress || key.up == alwaysPress)
    {
        key.up = noPress;
        *Param = *Param + 1;
        ips200_clear(IPS200_BGCOLOR);
        KEYSTATESelect();
    }
    else if(key.down == onePress || key.down == alwaysPress)
    {
        key.down = noPress;
        *Param = *Param - 1;
        ips200_clear(IPS200_BGCOLOR);
        KEYSTATESelect();
    }
	}
	else
	{
		BEEP_ON;
		while(1)
		{
			PRINTF("Param in ChangeParamInt16(int16 *Param) Function Error");
		}
	}
}

void ChangeParamUint16(uint16 *Param) //�����޸�ģʽ�е��޸�uint16��
{
	if(Param)
	{
    if(key.up == onePress || key.up == alwaysPress)
    {
        key.up = noPress;
        *Param = *Param + 1;
        ips200_clear(IPS200_BGCOLOR);
        KEYSTATESelect();
    }
    else if(key.down == onePress || key.down == alwaysPress)
    {
        key.down = noPress;
        *Param = *Param - 1;
        ips200_clear(IPS200_BGCOLOR);
        KEYSTATESelect();
    }
	}
	else
	{
		BEEP_ON;
		while(1)
		{
			PRINTF("Param in ChangeParamUint16(uint16 *Param) Function Error");
		}
	}
}
void ChangeParamInt(int *Param) //�����޸�ģʽ�е��޸�int��
{
	if(Param)	//ָ�벻Ϊ��
	{
		//����ִ�д���
    if(key.up == onePress || key.up == alwaysPress)
    {
        key.up = noPress;
        *Param = *Param + 1;
        ips200_clear(IPS200_BGCOLOR);
        KEYSTATESelect();
    }
    else if(key.down == onePress || key.down == alwaysPress)
    {
        key.down = noPress;
        *Param = *Param - 1;
        ips200_clear(IPS200_BGCOLOR);
        KEYSTATESelect();
    }
	}
	else
	{
		//����ִ�д���
		BEEP_ON;
		while(1)
		{
			PRINTF("Param in ChangeParamInt(int *Param) Function Error");
		}
	}
}
void ChangeParamFloat(float *Param)//�����޸�ģʽ�е��޸�float��
{
	if(Param)
	{
    if(key.up == onePress || key.up == alwaysPress)
    {
        key.up = noPress;
        *Param = *Param + 0.1;
        ips200_clear(IPS200_BGCOLOR);
        KEYSTATESelect();
    }
    else if(key.down == onePress || key.down == alwaysPress)
    {
        key.down = noPress;
        *Param = *Param - 0.1;
        ips200_clear(IPS200_BGCOLOR);
        KEYSTATESelect();
    }
	}
	else
	{
		BEEP_ON;
		while(1)
		{
			PRINTF("Param in ChangeParamFloat(float *Param) Function Error");
		}
	}
}
void ChangeParamBool(bool *Param)//�����޸�ģʽ�е��޸�bool��
{
	if(Param)
	{
    if(key.up == onePress || key.up == alwaysPress || key.down == onePress || key.down == alwaysPress)
    {
        key.up = noPress;
        key.down = noPress;
        if(*Param) *Param = false;
        else *Param = true;
        ips200_clear(IPS200_BGCOLOR);
        KEYSTATESelect();
    }
	}
	else
	{
		BEEP_ON;
		while(1)
		{
			PRINTF("Param in ChangeParamBool(bool *Param) Function Error");
		}
	}
}
//----------------------------------------------------------------
//  @brief        �޸Ĳ�������Ҫ����Ļ��ʾ�Ĳ�������һ��
//  @param        void
//  @return       void
//  Sample usage: ChangeParam(); 
//----------------------------------------------------------------
void ChangeParam()  
{
    switch(lcd.page)
    {
        case 1:
            switch(lcd.cursor)
            {
                case 1:
                    ChangeParamFloat(&pid.leftMotor.kp);
                    break;     
                case 2:        
                    ChangeParamFloat(&pid.leftMotor.ki);
                    break;     
                case 3:        
                    ChangeParamFloat(&pid.leftMotor.kd);
                    break;     
                case 4:        
                    ChangeParamFloat(&pid.rightMotor.kp);
                    break;     
                case 5:        
                    ChangeParamFloat(&pid.rightMotor.ki);
                    break;
                case 6:
                    ChangeParamFloat(&pid.rightMotor.kd);
                    break;
                case 7:
                    ChangeParamUint16(&motor.expectSpeed);
                    break;
//								case 8:
//									ChangeParamUint16(&rightWheel.now);
//										break;
//								case 9:
//										ChangeParamUint16(&leftWheel.now);
//										break;
								case 10:
									ChangeParamInt16(&motor.left.expectSpeed);
									break;
								case 11:
                    ChangeParamInt16(&motor.right.expectSpeed);
                    break;
                case 12:
                    ChangeParamUint16(&motor.left.differential);
                    break;
                case 13:
                    ChangeParamUint16(&motor.right.differential);
                    break;
								default:
										break;
                
            }
            break;
         case 2:
             switch(lcd.cursor)
             {
                case 1:
                    ChangeParamBool(&debug.servo);
                    break;
                case 2:
                    ChangeParamBool(&debug.stopProtect);
                    break;
                case 3:
                    break;
                case 4:
                    break;
								default:
										break;
             }
             break;
          case 3:
             switch(lcd.cursor)
             {
                 case 1:

                     break;
                 case 2:

                     break;
                 case 3:

                     break;
								 default:
										 break;
             }
             break;
					default:
						 break;	 
    }
}


//----------------------------------------------------------------
//  @brief        ��Ļ���庯���������޸�flash
//  @param        void
//  @return       void
//  Sample usage: LcdDisplay(); 
//  Notion        ChangeParam()�ڷ�һ��Ҫ��KEYSTATESelect()����һ��
//----------------------------------------------------------------
void LcdDisplay()
{
    KeyState();
    ConfirmAndBack();
    switch(keyState)
    {
        case Select:
            UpAndDown();
            KEYSTATESelect();
            break;
        case Change:
            
            ChangeParam();
            break;
				default:
					 BEEP_ON;
					 while(1)
					 		{
					 			PRINTF("switch(keyState) in LcdDisplay() Function  Error");
					 		}
					 break;
    }

}