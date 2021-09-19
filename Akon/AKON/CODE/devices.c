#include "devices.h"

/***************************��������(ctrl + G��ת)*********************
	(1)�豸��ʼ��		    ------------------------------------	 23
	(2)������				------------------------------------	 57
	(3)����  				------------------------------------	 88
	(4)��� 				------------------------------------	193
	(5)������ 				------------------------------------	236
	(6)���ܿ��� 			------------------------------------    322
	(7)������				------------------------------------    330
*********************************************************************/


int8 ramp_flag=0;
//----------------------------------------------------------------
//  @brief        ���������ʼ������
//  @param        void
//  @return       void
//  Sample usage: DevicesInit(); 
//----------------------------------------------------------------
void DevicesInit()
{
    //beep
	gpio_init(BEEP_PIN,GPO,0,GPIO_PIN_CONFIG);
    BeepParamInit();
    //��������ʼ��
    qtimer_quad_init(QTIMER_1,QTIMER1_TIMER0_C0,QTIMER1_TIMER1_C1);
    qtimer_quad_init(QTIMER_1,QTIMER1_TIMER2_C2,QTIMER1_TIMER3_C24);
    EncoderParamInit();
     //�����ʼ��
	gpio_init(DIR_1, GPO, 0, GPIO_PIN_CONFIG); 		//��Ƭ���˿�D0 ��ʼ��DIR_1			GPIO
	gpio_init(DIR_2, GPO, 0, GPIO_PIN_CONFIG); 		//��Ƭ���˿�D1 ��ʼ��DIR_2			GPIO
	pwm_init(PWM_1, 17000, 0);      				//��Ƭ���˿�D2 ��ʼ��PWM_1����10K ռ�ձ�0
	pwm_init(PWM_2, 17000, 0);     					//��Ƭ���˿�D3 ��ʼ��PWM_2����10K ռ�ձ�0
    MotorParamInit();
    //�����ʼ��
    pwm_init(S_MOTOR1_PIN,50,servo.duty);
    ServoParamInit();
    //key
    gpio_init(KEY_UP_PIN, GPI, 0, GPIO_PIN_CONFIG);
	gpio_init(KEY_DOWN_PIN, GPI, 0, GPIO_PIN_CONFIG);
	gpio_init(KEY_LEFT_PIN, GPI, 1, GPIO_PIN_CONFIG);
	gpio_init(KEY_RIGHT_PIN, GPI, 0, GPIO_PIN_CONFIG); 
    KeyParamInit();
    //DialSwitch
    gpio_init(DIALSWITCH1, GPI, 0, GPIO_PIN_CONFIG);
	gpio_init(DIALSWITCH2, GPI, 0, GPIO_PIN_CONFIG);
     /**�����ǳ�ʼ��**/
    icm20602_init_spi();
    ICM20602_Offset();
    //LED��ʼ��       ��Ϊ���ź���Ļ�����ˣ��Ȳ�ʹ��
//    pwm_init(GREENPIN, 17000, 0);       //Green	
//	  pwm_init(REDPIN,   17000, 0);       //RED	
//    pwm_init(BLUEPIN,  17000, 0);       //Blue	
}




//============================================BEEP==========================================================================================================================//
//    Note       BeepInt()Ҫ����1ms���ж��У���Ҫ�������Ļ�ֱ�ӵ���
//    ��������   BEEP_ON
//    ��������   BEEP_OFF



//Statement
Beep beep; 
//Function
void BeepParamInit()
{
    //���ܻ�ı�
    beep.function = BeepInt;
    //��������
    beep.state = OFF;
    beep.num = 0;
    beep.time = 0;
    beep.cnt = 0;
}
//----------------------------------------------------------------
//  @brief        �������ڶ೤ʱ�����켸��
//  @param        n     �켸��
//  @param        time  �ڶ೤ʱ����
//  @return       void
//  Sample usage: Beep_DiDi(2, 150); 150ms����2��
//----------------------------------------------------------------
void Beep_DiDi(uint16 n,uint16 time)
{
	if(beep.state == OFF)
	{
		beep.state = ON;
		beep.time  = time;
		beep.num   = n;
		if(beep.num == 0) beep.num = 1;
		beep.cnt = 1;
		BEEP_OFF;
	}
}

//----------------------------------------------------------------
//  @brief        ����������������1ms��ʱ����
//  @param        void
//  @return       void
//  Sample usage: BeepInt(void); 
//----------------------------------------------------------------
void BeepInt(void)
{
	if(beep.state == ON)
	{
		beep.cnt ++;
		if(beep.cnt %beep.time == 0) BEEP_TOGGLE;
		if((beep.cnt %beep.time == 0) && ((beep.cnt / beep.time) == (beep.num*2)))		
			beep.state = OFF;
	}
}

//=============================================KEY==========================================================================================================================//
//    Note       �����ϴ��ϵ��£���������


//Statement
Key key;
//KeyStateEnum KeyPageUp,KeyPageDown;
//Function
void KeyParamInit()
{   
    //���ܻ��޸�
    key.function = KeyScanInt;
    //��������
    key.up    = noPress;
    key.down  = noPress;
    key.left  = noPress;
    key.right = noPress;
}
//----------------------------------------------------------------
//  @brief        ������������ʱ����
//  @param        void
//  @return       void
//  Sample usage: KeyScanInt(void); 
//----------------------------------------------------------------
void KeyScanInt(void)
{
	static uint8 keyPressNum = 0;
	static uint16 keyPressTime = 0;
	static uint8 someKeyPressFlag = 0;//0 �ɿ� 1 �а���  2:������ 3:����
	#define AlwaysPressTime 700//һֱ�����ʱ��
	#define debouncing  5
	if(someKeyPressFlag == 0 && (KEY_UP == 0 || KEY_DOWN == 0 || \
		KEY_LEFT == 0 || KEY_RIGHT == 0 ))
	{
		 someKeyPressFlag = 1;
	}	
	if(someKeyPressFlag > 0)
	{
		keyPressTime ++;
		//��ʱ5ms����
		if(someKeyPressFlag == 1 && keyPressTime >= debouncing) 
		{
			someKeyPressFlag = 2;
			if(KEY_UP == 0) 
				keyPressNum = 1;//��ⰴ������
			else if(KEY_DOWN == 0) 
				keyPressNum = 2; 
			else if(KEY_LEFT == 0) 
				keyPressNum = 3;
			else if(KEY_RIGHT == 0) 
				keyPressNum = 4;
//			else if(KEY_CENTER == 0) 
//				keyPressNum = 5;
//			else if(KEY_PAGE_UP == 0) 
//				keyPressNum = 6;
//			else if(KEY_PAGE_DOWN == 0) 
//				keyPressNum = 7;
		}
		//��һ�¾��ɿ���״̬
		if((KEY_UP == 1 && KEY_DOWN == 1 && KEY_LEFT == 1 && KEY_RIGHT == 1 ) && keyPressTime < AlwaysPressTime && someKeyPressFlag == 2)
			{
				someKeyPressFlag = 0;//�����ɿ���
				if(keyPressNum == 1) 
                {
                    key.up = onePress;
                }
				else if(keyPressNum == 2) 
                {
					key.down = onePress;
                }
				else if(keyPressNum == 3) 
                {
					key.left = onePress;
                }
				else if(keyPressNum == 4) 
                {
					key.right = onePress;
                }
	//			else if(keyPressNum == 5) 
	//				KeyCenter = onePress;
	//			else if(keyPressNum == 6) 
	//				KeyPageUp = onePress;
	//			else if(keyPressNum == 7) 
	//				KeyPageDown = onePress;
				keyPressTime = 0;
				someKeyPressFlag = 0;
				Beep_DiDi(1,30);
		}
	
		//�������ɿ���״̬
		if(  keyPressTime >= AlwaysPressTime && someKeyPressFlag == 2) 
		{
			if(keyPressNum == 1) 	  key.up = alwaysPress;
			else if(keyPressNum == 2) key.down = alwaysPress;
			else if(keyPressNum == 3) key.left = alwaysPress;
			else if(keyPressNum == 4) key.right = alwaysPress;
			
//			else if(keyPressNum == 5) KeyCenter = alwaysPress;
//			else if(keyPressNum == 6) KeyPageUp = alwaysPress;
//			else if(keyPressNum == 7) KeyPageDown = alwaysPress;
			Beep_DiDi(2,50);
			if(KEY_UP == 1 && KEY_DOWN == 1 && KEY_LEFT == 1 && KEY_RIGHT == 1 )
//				KEY_CENTER == 1 && KEY_PAGE_DOWN == 1 && KEY_PAGE_UP == 1)
			{
				someKeyPressFlag = 0;
				keyPressTime = 0;
				key.up = noPress;
				key.down = noPress;
				key.left = noPress;
				key.right = noPress;
//				KeyPageUp = noPress;
//				KeyPageDown = noPress;
//				KeyCenter = noPress;
			}
		}
	 }
 }
//===========================================MOTOR==============================================================================================================================//
//    Note       PWMһ��Ϊ��ֵ�����Ʒ������IO���ı䷽������pwm��Ϊ��ֵ
//    ����     ��IO��1 <--> 0
//    ���ַ���   DIR_1
//    ���ַ���   DIR_2
 

//Statement
Motor motor;
//Function
void MotorParamInit(void)  //�����ڶ����ʱ���ʼ����������ֱ̫�ۣ��޸�Ҳ�����㡣
{
    //���ܻ��޸�
    motor.expectSpeed = 600;    //�����ٶ�
    motor.left.differential  = 180;    //����ֵ
    motor.right.differential = 180;
    motor.function = Cam_Motor_Control;
    //��������
    motor.left.expectSpeed  = 0;
    motor.left.pwm  = 0;
    motor.right.expectSpeed = 0;
    motor.right.pwm = 0;
}
//----------------------------------------------------------------
//  @brief        ���pwmֵд�룬��ʱ����
//  @param        void
//  @return       void
//  Sample usage: PWM_Write(void); 
//----------------------------------------------------------------
void PWM_Write(void)
{
	//�޷�
	pwm_limit(motor.left.pwm);
	pwm_limit(motor.right.pwm);
	if(motor.left.pwm >= 0)
	{
        gpio_set(DIR_1,1);		
	    pwm_duty(PWM_1,motor.left.pwm);						
	}
	else
	{
		gpio_set(DIR_1,0);		
	    pwm_duty(PWM_1,-motor.left.pwm);			
	}	
	//�ұߵ��
	if(motor.right.pwm >= 0)
	{
		gpio_set(DIR_2,1);
            pwm_duty(PWM_2,motor.right.pwm); 	
	}
	else
	{
		gpio_set(DIR_2,0);
            pwm_duty(PWM_2,-motor.right.pwm); 	
	}
	//��ߵ��
}

//===========================================SERVO==============================================================================================================================//

//Statement
Servo servo;
//Function
void ServoParamInit(void)
{
    //���ܻ��޸�
    servo.mid = 3750;
    servo.function = ServoMotorControl;
    //��������
    servo.expectPosition = 0;
    servo.duty = 0;
    servo.dutyAdd = 0;
}

//===============================================ENCODER=================================================================================================================================//
//    Note       ������ʹ�ã�EncoderRead()����
//    ����     ���ĳ�ʼ����positive <--> negative
//    ���ַ���   encoderLeftDir
//    ���ַ���   encoderRightDir

//statement
Wheel wheel;    
//Function
void EncoderParamInit()
{
    //���ܻ��޸�
    wheel.left.dir = positive;
    wheel.right.dir = positive;
    wheel.function = EncoderRead;
    //��������
    wheel.leftSpeed  = 0;
    wheel.rightSpeed = 0;
    wheel.totalSpeed = 0;
    wheel.lastTotalSpeed = 0;
    wheel.DistanceAll = 0;
    wheel.left.encoder = 0;
    wheel.left.now = 0;
    wheel.left.last = 0;
    wheel.left.acc = 0;
    wheel.left.out = 0;
    wheel.right.encoder = 0;
    wheel.right.now = 0;
    wheel.right.last = 0;
    wheel.right.acc = 0;
    wheel.right.out = 0;
}
//----------------------------------------------------------------
//  @brief        ��������ֵ ��ֵ�ж�Ϊ2ms���ڵ���Ŀ��ƺ�����
//  @param        void
//  @return       void
//  Sample usage: EncoderRead();
//----------------------------------------------------------------
void EncoderRead(void)
{

    //�ɼ�
	if(wheel.left.dir == positive)
	{
        wheel.left.encoder = qtimer_quad_get(QTIMER_1,QTIMER1_TIMER0_C0 ); 
        qtimer_quad_clear(QTIMER_1,QTIMER1_TIMER0_C0 );
	}
    else
    {
        wheel.left.encoder = -qtimer_quad_get(QTIMER_1,QTIMER1_TIMER0_C0 );
        qtimer_quad_clear(QTIMER_1,QTIMER1_TIMER0_C0 );    
	}
	if(wheel.right.dir == positive)
	{
        wheel.right.encoder = qtimer_quad_get(QTIMER_1,QTIMER1_TIMER2_C2 );
        qtimer_quad_clear(QTIMER_1,QTIMER1_TIMER2_C2 );
	}
	else
    {
        wheel.right.encoder = -qtimer_quad_get(QTIMER_1,QTIMER1_TIMER2_C2 );
        qtimer_quad_clear(QTIMER_1,QTIMER1_TIMER2_C2 );
	}
    /**********�������ֲ���**********/
	//��ʵ�ٶ�
	wheel.left.now = wheel.left.encoder;
	wheel.left.acc = wheel.left.now - wheel.left.last;
    wheel.left.last=0; // �Լ�д�ģ�Ϊ�˱�����Ϊ�˲��������last 2021.8.31
	//�˲�
	if(ABS(wheel.left.acc) > 800)
		wheel.left.now = wheel.left.now *5 + wheel.left.last *5;
	else
		wheel.left.now = wheel.left.now *9 + wheel.left.last *1;
	//�޷�
	wheel.left.out = range_protect(wheel.left.now,-5000,5000);
	//��¼
	wheel.left.last = wheel.left.now;
	/**********�������ֲ���**********/
	//��ʵ�ٶ�
	wheel.right.now = wheel.right.encoder;
	wheel.right.acc = wheel.right.now - wheel.right.last;
    wheel.right.last=0; // �Լ�д�ģ�Ϊ�˱�����Ϊ�˲��������last 2021.8.31
	//�˲�
	if(ABS(wheel.right.acc) > 800)
		wheel.right.now = wheel.right.now *5 + wheel.right.last *5;
	else
		wheel.right.now = wheel.right.now *9 + wheel.right.last;
	//�޷����
	wheel.right.out = range_protect(wheel.right.now,-5000,5000);
	//��¼
	wheel.right.last = wheel.right.now;
    /**********��ʵ�ٶ��Լ�·��**********/
	//�������
	wheel.DistanceAll += ((wheel.right.encoder + wheel.left.encoder)/2);
	//��ʵ�ٶ� PAI * BodyR /180 = 0.17
	wheel.leftSpeed =  wheel.left.out; //�Լ�ȥ���������ǣ�-ICM_Treated.gyro.z * 0.17  2021.9.1������wheel.now�Ļ������ʵ�ٶ�ҲӦ���ø��˲���
	wheel.rightSpeed =  wheel.right.out; //�Լ�ȥ���������ǣ�+ICM_Treated.gyro.z * 0.17  2021.9.1������wheel.now�Ļ������ʵ�ٶ�ҲӦ���ø��˲���
    //ѡ���ٶ�
//	if(ICM_Treated.gyro.z < -50)//��ת����
//	{
//		wheel.now = leftWheel.out - ICM_Treated.gyro.z * 0.17;
//	}
//	else if(ICM_Treated.gyro.z > 50)//��תȡ��
//	{
//		wheel.now = rightWheel.out + ICM_Treated.gyro.z * 0.17;
//	}
//	else
//	{
		wheel.totalSpeed = (wheel.left.out + wheel.right.out) / 2;
//	}
	//�˲�
	wheel.totalSpeed = wheel.totalSpeed * 9 + wheel.lastTotalSpeed *1;
	wheel.lastTotalSpeed = 0; //2021.8.31���ϴε�ֵ��0��ԭ����wheel.now  ��Ϊ ���˲��¼�
}

    
    
    
//===============================================Debug======================================================================================================================// 


//----------------------------------------------------------------
//  @brief        ��ȡdebug����״̬�����뿪��D27
//  @param        void
//  @return       void
//  Sample usage: DebugForMain(void); 
//----------------------------------------------------------------
void DebugOpenAndClose()
{
    debug.open = gpio_get(DIALSWITCH1);
}


//Statement
Debug debug;
//Function
void DebugParamInit()
{
    debug.function = DebugOpen;
    debug.open = false;
    debug.servo = true;
    debug.stopProtect = false;
}
//----------------------------------------------------------------
//  @brief        ��ʱ�������й�debug�������أ�Ĭ��Ϊ1ms���ó����Զ����ж�ʱ��
//  @param        void
//  @return       void
//  Sample usage: DebugForTim(void); 
//----------------------------------------------------------------
void DebugOpen()
{
    DebugOpenAndClose();  //���Ŀǰ�ǿ����ǹ�
    if(debug.open == true) 
    {
        task[lcdTask].isOpen   = true;
        task[keyTask].isOpen   = true;
        task[ANODTTask].isOpen = true;
    }
}

static const unsigned char crc_table[] =
{
    0x00,0x31,0x62,0x53,0xc4,0xf5,0xa6,0x97,0xb9,0x88,0xdb,0xea,0x7d,0x4c,0x1f,0x2e,
    0x43,0x72,0x21,0x10,0x87,0xb6,0xe5,0xd4,0xfa,0xcb,0x98,0xa9,0x3e,0x0f,0x5c,0x6d,
    0x86,0xb7,0xe4,0xd5,0x42,0x73,0x20,0x11,0x3f,0x0e,0x5d,0x6c,0xfb,0xca,0x99,0xa8,
    0xc5,0xf4,0xa7,0x96,0x01,0x30,0x63,0x52,0x7c,0x4d,0x1e,0x2f,0xb8,0x89,0xda,0xeb,
    0x3d,0x0c,0x5f,0x6e,0xf9,0xc8,0x9b,0xaa,0x84,0xb5,0xe6,0xd7,0x40,0x71,0x22,0x13,
    0x7e,0x4f,0x1c,0x2d,0xba,0x8b,0xd8,0xe9,0xc7,0xf6,0xa5,0x94,0x03,0x32,0x61,0x50,
    0xbb,0x8a,0xd9,0xe8,0x7f,0x4e,0x1d,0x2c,0x02,0x33,0x60,0x51,0xc6,0xf7,0xa4,0x95,
    0xf8,0xc9,0x9a,0xab,0x3c,0x0d,0x5e,0x6f,0x41,0x70,0x23,0x12,0x85,0xb4,0xe7,0xd6,
    0x7a,0x4b,0x18,0x29,0xbe,0x8f,0xdc,0xed,0xc3,0xf2,0xa1,0x90,0x07,0x36,0x65,0x54,
    0x39,0x08,0x5b,0x6a,0xfd,0xcc,0x9f,0xae,0x80,0xb1,0xe2,0xd3,0x44,0x75,0x26,0x17,
    0xfc,0xcd,0x9e,0xaf,0x38,0x09,0x5a,0x6b,0x45,0x74,0x27,0x16,0x81,0xb0,0xe3,0xd2,
    0xbf,0x8e,0xdd,0xec,0x7b,0x4a,0x19,0x28,0x06,0x37,0x64,0x55,0xc2,0xf3,0xa0,0x91,
    0x47,0x76,0x25,0x14,0x83,0xb2,0xe1,0xd0,0xfe,0xcf,0x9c,0xad,0x3a,0x0b,0x58,0x69,
    0x04,0x35,0x66,0x57,0xc0,0xf1,0xa2,0x93,0xbd,0x8c,0xdf,0xee,0x79,0x48,0x1b,0x2a,
    0xc1,0xf0,0xa3,0x92,0x05,0x34,0x67,0x56,0x78,0x49,0x1a,0x2b,0xbc,0x8d,0xde,0xef,
    0x82,0xb3,0xe0,0xd1,0x46,0x77,0x24,0x15,0x3b,0x0a,0x59,0x68,0xff,0xce,0x9d,0xac
};

unsigned char cal_table_high_first(unsigned char value)
{
    unsigned char i, crc;

    crc = value;
    /* ������������8λ����Ҫ����8�� */
    for (i=8; i>0; --i)
    { 
        if (crc & 0x80)  /* �ж����λ�Ƿ�Ϊ1 */
        {
        /* ���λΪ1������Ҫ���������һλ��Ȼ����0x31��� */
        /* 0x31(����ʽ��x8+x5+x4+1��100110001)�����λ����Ҫ���ֱ��ȥ�� */
            crc = (crc << 1) ^ 0x31;        }
        else
        {
            /* ���λΪ0ʱ������Ҫ�����������������һλ */
            crc = (crc << 1);
        }
    }

    return crc;
}


unsigned char crc_high_first(unsigned char *ptr, unsigned char len)
{
    unsigned char i; 
    unsigned char crc=0x00; /* ����ĳ�ʼcrcֵ */ 

    while(len--)
    {
        crc ^= *ptr++;  /* ÿ��������Ҫ������������,������ָ����һ���� */  
        for (i=8; i>0; --i)   /* ������μ�����������һ���ֽ�crcһ�� */  
        { 
            if (crc & 0x80)
                crc = (crc << 1) ^ 0x31;
            else
                crc = (crc << 1);
        }
    }

    return (crc); 
}


void  create_crc_table(void)
{
    unsigned short i;
    unsigned char j = 123;

    for (i=0; i<=0xFF; i++)
    {
        if (0 == (i%16))
            PRINTF("\n");

        j = i&0xFF;
        PRINTF("0x%.2x, ", cal_table_high_first (j));  /*���μ���ÿ���ֽڵ�crcУ��ֵ*/
    }
}


unsigned char cal_crc_table(unsigned char *ptr, unsigned char len) 
{
    unsigned char  crc = 0x00;

    while (len--)
    {
        crc = crc_table[crc ^ *ptr++];
    }
    return (crc);
}



//===============================================Gyroscope====================================================================================================================// 
//    Note      ����������ֻ��Ҫ��ʼ����Ȼ�����GetGyroscope()�����Ϳ��Զ�ȡ��ֵ��
//    ����ֵ   
//    ԭʼֵ    IMC_State;     
//    ��Ưֵ    IMC_Offset;
//    �˲���ֵ  IMC_Treated;
//   �ǶȻ���  ��getAngle������




//----------------------------------------------------------------
//  @brief        ������������Ư
//  @param        void
//  @return       void
//  Sample usage: ICM20602_Offset(); //����������������Ư����
//----------------------------------------------------------------
//Statement
ICM20602 IMC_State; //��ǰ����ֵ
ICM20602 IMC_Offset;//��Ưֵ
//Function
void ICM20602_Offset(void)
{
    uint8 i, Count = 100;
    int64 temp[6] = {0};
    
    IMC_State.gyro.x    = icm_gyro_x;
    IMC_State.gyro.y    = icm_gyro_y;
    IMC_State.gyro.z    = icm_gyro_z;
    IMC_State.accdata.x = icm_acc_x;
    IMC_State.accdata.y = icm_acc_y;
    IMC_State.accdata.z = icm_acc_z;
    
    for (i = 0; i < Count; i++)
    {
        get_icm20602_accdata_spi();//���ٶȼ�
        get_icm20602_gyro_spi();//������
        systick_delay_ms(2);  //�ϵ���ʱ

        temp[0] += IMC_State.accdata.x;
        temp[1] += IMC_State.accdata.y;
        temp[2] += IMC_State.accdata.z;

        temp[3] += IMC_State.gyro.x;
        temp[4] += IMC_State.gyro.y;
        temp[5] += IMC_State.gyro.z;
    }
    IMC_Offset.accdata.x = temp[0] / Count;
    IMC_Offset.accdata.y = temp[1] / Count;
    IMC_Offset.accdata.z = temp[2] / Count;

    IMC_Offset.gyro.x = temp[3] / Count;
    IMC_Offset.gyro.y = temp[4] / Count;
    IMC_Offset.gyro.z = temp[5] / Count;
//    ICM20602_Offset_Finished=1;
}







//----------------------------------------------------------------
//  @brief        ���������ݻ����˲�
//  @param        void
//  @return       void
//  Sample usage: Data_Filter(); //���������������˲�
//----------------------------------------------------------------
//Statement
ICM20602_Treated IMC_Treated; //�����������������+
int32 ACC_X_BUF[ACC_FILTER_NUM], ACC_Y_BUF[ACC_FILTER_NUM], ACC_Z_BUF[ACC_FILTER_NUM];  // �˲���������
int32 GYRO_X_BUF[GYRO_FILTER_NUM], GYRO_Y_BUF[GYRO_FILTER_NUM], GYRO_Z_BUF[GYRO_FILTER_NUM];
//Function
void Data_Filter(void)  // �����˲�
{
    int i=0;
    int64 temp1 = 0, temp2 = 0, temp3 = 0, temp4 = 0, temp5 = 0, temp6 = 0;
//    ACC_X_BUF[0] = IMC_State.accdata.x; // ���»�����������
//    ACC_Y_BUF[0] = IMC_State.accdata.y;
//    ACC_Z_BUF[0] = IMC_State.accdata.z;
    GYRO_X_BUF[0] = IMC_State.gyro.x;
    GYRO_Y_BUF[0] = IMC_State.gyro.y;
    GYRO_Z_BUF[0] = IMC_State.gyro.z;

//    for(i=0;i<ACC_FILTER_NUM;i++)
//    {
//        temp1 += ACC_X_BUF[i];
//        temp2 += ACC_Y_BUF[i];
//        temp3 += ACC_Z_BUF[i];
//    }
//    IMC_Treated.accdata.x = temp1 / ACC_FILTER_NUM / AcceRatio;
//    IMC_Treated.accdata.y = temp2 / ACC_FILTER_NUM / AcceRatio;
//    IMC_Treated.accdata.z = temp3 / ACC_FILTER_NUM / AcceRatio;
    for(i=0;i<GYRO_FILTER_NUM;i++)
    {
        temp4 += GYRO_X_BUF[i];
        temp5 += GYRO_Y_BUF[i];
        temp6 += GYRO_Z_BUF[i];
    }
    IMC_Treated.gyro.x = temp4 / GYRO_FILTER_NUM / GyroRatio;
    IMC_Treated.gyro.y = temp5 / GYRO_FILTER_NUM / GyroRatio;
    IMC_Treated.gyro.z = temp6 / GYRO_FILTER_NUM / GyroRatio;

//    for(i = 0; i < ACC_FILTER_NUM - 1; i++)
//    {
//        ACC_X_BUF[ACC_FILTER_NUM-1-i] = ACC_X_BUF[ACC_FILTER_NUM-2-i];
//        ACC_Y_BUF[ACC_FILTER_NUM-1-i] = ACC_Y_BUF[ACC_FILTER_NUM-2-i];
//        ACC_Z_BUF[ACC_FILTER_NUM-1-i] = ACC_Z_BUF[ACC_FILTER_NUM-2-i];
//    }
    for(i = 0; i < GYRO_FILTER_NUM - 1; i++)
    {
        GYRO_X_BUF[GYRO_FILTER_NUM-1-i] = GYRO_X_BUF[GYRO_FILTER_NUM-2-i];
        GYRO_Y_BUF[GYRO_FILTER_NUM-1-i] = GYRO_Y_BUF[GYRO_FILTER_NUM-2-i];
        GYRO_Z_BUF[GYRO_FILTER_NUM-1-i] = GYRO_Z_BUF[GYRO_FILTER_NUM-2-i];
    }
}


//----------------------------------------------------------------
//  @brief        �ǶȻ��֣�ת�ĽǶ�
//  @param        void
//  @return       void
//  Sample usage: GetAngle(); //ת�ĽǶ�
//----------------------------------------------------------------
//Statement
int64 Circle_angle0=0;
int16 Circle_angle=0;
int64 ramp_angle0=0;
int64 ramp_angle=0;
//Function
void GetAngle(void)
{
    Circle_angle0 =Circle_angle0+ IMC_Treated.gyro.z;
    Circle_angle = Circle_angle0/490;

    if(ramp_flag!=0)
    {ramp_angle0 = ramp_angle0 + IMC_Treated.gyro.y;
    ramp_angle =   ramp_angle0/49;}
    else if(ramp_flag==0)
    {
     ramp_angle=0;
    }
}

//----------------------------------------------------------------
//  @brief        �����Ǻ����ܺͣ���ȡ�������˲����ֵ
//  @return       void
//  Sample usage: GetGyroscope(); //�����ǵ�ֵ   
//----------------------------------------------------------------
void GetGyroscope()
{
    get_icm20602_gyro_spi();
    Data_Filter();
    GetAngle();
}


//===============================================LED==========================================================================================================================//  
//    Note     LEDʹ�ã�LedColor(color)����
//    ������ɫ   (һ�����ʣ�Сд)
//               ��ɫ    red;   
//               ��ɫ    green;
//               ��ɫ    blue;
//               ��ɫ    white;


//----------------------------------------------------------------
//  @brief        ��LED��ʾRGB��ɫ
//  @param        Red       ��ɫ��ռ�ȣ�0-255��
//  @param        Green     ��ɫ��ռ�ȣ�0-255��
//  @param        Blue      ��ɫ��ռ�ȣ�0-255��
//  @return       void
//  Sample usage: LedSetColorRGB(255,   0,   0);    //LED��ʾ��ɫ   
//----------------------------------------------------------------
void LedSetColorRGB(uint8 Red, uint8 Green, uint8 Blue)
{
    //��Ϊ�ǹ����ĵ�
    uint16 redPwm   = (((255 - Red  )/255)*PWM_DUTY_MAX);
    uint16 greenPwm = (((255 - Green)/255)*PWM_DUTY_MAX);
    uint16 bluePwm  = (((255 - Blue )/255)*PWM_DUTY_MAX);
    pwm_duty(REDPIN,   redPwm);
    pwm_duty(GREENPIN, greenPwm);
    pwm_duty(BLUEPIN,  bluePwm);
}

//----------------------------------------------------------------
//  @brief        ��LED��ʾ�ض���ɫ
//  @param        Color       ��ʾ����ɫ
//  @return       void
//  Sample usage: LedColor(red);    //LED��ʾ��ɫ   
//----------------------------------------------------------------
void LedColor(ColorEnum Color)
{
    switch(Color)
    {
        case red:
            LedSetColorRGB(255,   0,   0);  //1
            break;
        case green:
            LedSetColorRGB(  0, 255,   0);  //2
            break;
        case blue:
            LedSetColorRGB(  0,   0, 255);  //3
            break;
        case white:
            LedSetColorRGB(255, 255, 255);  //5
            break;
        default:
            break;
    }
}



