#include "lcd.h"
/*
ips显示函数  液晶显示字符   ips200_showchar(uint16 x,uint16 y,const int8 dat)   ips200_showchar(0,0,'x');//坐标0,0写一个字符x
               液晶显示字符串 ips200_showstr(uint16 x,uint16 y,const int8 dat[])  ips200_showstr(0,0,"seekfree");   
               液晶显示8位有符号 ips200_showint8(uint16 x,uint16 y,int8 dat)       ips200_showint8(0,0,x);//x为int8类型
               液晶显示浮点数(去除整数部分无效的0) ips200_showfloat(uint16 x,uint16 y,double dat,int8 num,int8 pointnum) ips_showfloat(0,0,x,2,3);//显示浮点数 
NOTION:颜色是大写的，eg：红色 RED
*/



Lcd lcd;

void LcdInit()
{
    ips200_init();			//初始化2.0寸IPS屏幕
    
    lcd.page = 0;
    lcd.cursor = 0;
}

uint8 GetKeyState(void)  //获取按键目前的状态
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
//  @brief        获取目前拨码开关状态
//  @param        void
//  @return       void
//  Sample usage: KeyState(); 
//----------------------------------------------------------------
//Statement
LcdStateEnum keyState;  //目前屏幕拨码开关状态
//Function
void KeyState()  
{
    if(GetKeyState() == 1 )  keyState = Select;
    else if(GetKeyState() == 2 )  keyState = Change;
}


void CursorMove(uint8 direction, int8 moveDistance) //光标移动
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
//  @brief        上下
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

void LcdPageZero()  //目录
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
void LcdPageOne()   //第一页：电机PID和信息
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
void LcdPageTwo()  //第二页：Function开关
{
    ips200_showstr(80, 0, "Page Two");
    ips200_showstr(10, 1, "ServoOpen");
    if(debug.servo)  ips200_showstr(130, 1, "True");
    else ips200_showstr(130, 1, "False");
    
    ips200_showstr(10, 2, "StopProtectOpen");
    if(debug.stopProtect)  ips200_showstr(130, 2, "True");
    else ips200_showstr(130, 2, "Flase");
    
}
void LcdPageThree() //第三页：舵机PID
{
    ips200_showstr(80, 0, "Page Three");
    
    ips200_showstr(10, 1, "ServoKp1");
    ips200_showint32(130, 1, pid.servo.kp1, 2);
    ips200_showstr(10, 2, "ServoKp2");
    ips200_showint32(130, 2, pid.servo.kp2, 2);
    ips200_showstr(10, 3, "ServoKd");
    ips200_showint32(130, 3, pid.servo.kd,  2);
}
void LcdPageFour()  //第四页显示内容
{
    Display_Pitcure();
    ips200_showstr(10, 5, "Threshold");
    ips200_showuint16(130, 5, Threshold);
    ips200_showstr(10, 6, "const camServoError");
    ips200_showfloat(130, 6, camServoError, 3, 2);
    ips200_showstr(10, 7, "ServoPwmNow");
    ips200_showuint16(130, 7, servo.duty - servo.mid);
}
void LcdPageFive()  //第五页显示内容 电感值
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
void LcdPageSix()   //第六页陀螺仪
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



void LcdPageSeven() //第七页：环岛
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

void LcdPageEight() //第八页
{
    ips200_showstr(80, 0, "Page Eight");
}
void KEYSTATESelect()   //每页内容显示
{
    switch(lcd.page)
    {
        case 0:
            LcdPageZero();  //目录
            break;
        case 1:
            LcdPageOne();   //电机PID
            break;
        case 2:
            LcdPageTwo();   //开关保护
            break;
        case 3:
            LcdPageThree(); //舵机PID
            break;
        case 4:
            LcdPageFour();  //摄像头图像
            break;
        case 5:
            LcdPageFive();  //电感值
            break;
        case 6:
            LcdPageSix();   //陀螺仪
            break;
        case 7:
            LcdPageSeven(); //环岛
            break;
        case 8:
            LcdPageEight();
            break;
				default:
						break;
    }
}

void ConfirmAndBack()   //确认键和返回键
{
    if(key.left == onePress || key.left == alwaysPress)
    {
        key.left = noPress;
        switch(lcd.page)
        {
            //在第一页的话是跳转到其他页面
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



//画一个箭头，x，y和其他函数一样，起始位置
void ShowArrow(uint16 x, uint16 y, uint16 color, uint8 direction) 
{
    int i;
    y = y*16;
    for(i = 0; i < ARROWLENGTH; i++)  //画箭头的横线
    {
        ips200_drawpoint(x+i, y+ARROWWIDTH/2, color);
    }
    switch(direction)
    {
        case Right:
            for(i = 0; i < ARROWWIDTH/2; i++)//画箭头的两条斜线
            { 
                ips200_drawpoint(x+i+ARROWWIDTH/2, y+i, color);
                ips200_drawpoint(x+i+ARROWWIDTH/2, y-i+ARROWWIDTH, color);
            }
            break;
        case Left:
            for(i = 0; i < ARROWWIDTH/2; i++)//画箭头的两条斜线
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
void ChangeParamInt16(int16 *Param) //用于修改模式中的修改int16型
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

void ChangeParamUint16(uint16 *Param) //用于修改模式中的修改uint16型
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
void ChangeParamInt(int *Param) //用于修改模式中的修改int型
{
	if(Param)	//指针不为空
	{
		//正常执行代码
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
		//错误执行代码
		BEEP_ON;
		while(1)
		{
			PRINTF("Param in ChangeParamInt(int *Param) Function Error");
		}
	}
}
void ChangeParamFloat(float *Param)//用于修改模式中的修改float型
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
void ChangeParamBool(bool *Param)//用于修改模式中的修改bool型
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
//  @brief        修改参数。需要和屏幕显示的参数保持一致
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
//  @brief        屏幕总体函数，包含修改flash
//  @param        void
//  @return       void
//  Sample usage: LcdDisplay(); 
//  Notion        ChangeParam()摆放一定要和KEYSTATESelect()里面一样
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