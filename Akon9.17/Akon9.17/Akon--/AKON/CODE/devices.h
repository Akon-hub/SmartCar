#ifndef _devices_h_
#define _devices_h_

#include "headfile.h"


extern void DevicesInit();


//===============================================BEEP======================================//


#define BEEP_PIN       B11
#define BEEP_ON        gpio_set(BEEP_PIN,1)
#define BEEP_OFF       gpio_set(BEEP_PIN,0)
#define BEEP_TOGGLE    gpio_toggle(BEEP_PIN)
#define ON    1
#define OFF   0



//statement
typedef struct
{
	uint16 state;
	uint16 num;
	uint16 time;
	uint32 cnt;
    void (*function)(void);
}Beep;

extern Beep beep; 

//function
extern void BeepParamInit();
extern void Beep_DiDi(uint16 n,uint16 time);
extern void BeepInt(void);

//===========================================KEY==========================================//

//define    板子上从上到下：上下左右
#define    KEY_UP_PIN 	    C31
#define    KEY_DOWN_PIN 	C27	
#define    KEY_LEFT_PIN 	C26
#define    KEY_RIGHT_PIN    C4 

//#define    DIP_1_PIN 	    A9
//#define    DIP_2_PIN 	    A10
//#define    DIP_3_PIN        A11
//#define    DIP_4_PIN  	    A12

//#define    KEY_CENTER   gpio_get(KEY_CENTER_PIN)
#define    KEY_UP 		gpio_get(KEY_UP_PIN)
#define    KEY_DOWN 	gpio_get(KEY_DOWN_PIN)
#define    KEY_LEFT 	gpio_get(KEY_LEFT_PIN)
#define    KEY_RIGHT	gpio_get(KEY_RIGHT_PIN)
//#define    KEY_PAGE_UP	gpio_get(PAGE_UP_PIN)
//#define    KEY_PAGE_DOWN gpio_get(PAGE_DOWN_PIN)
//#define    DIP_1 	    gpio_get(DIP_1_PIN)
//#define    DIP_2 	    gpio_get(DIP_2_PIN)
//#define    DIP_3       gpio_get(DIP_3_PIN)
//#define    DIP_4	     gpio_get(DIP_4_PIN)

typedef enum
{
	noPress = 0,
	onePress,//短按一下
	alwaysPress,//一直按着
} KeyStateEnum;
typedef struct
{
    KeyStateEnum up;
    KeyStateEnum down;
    KeyStateEnum left;
    KeyStateEnum right; 
    void (*function)(void);
}Key;
//extern KeyStateEnum KeyPageUp,KeyPageDown;
extern Key key;
//fucntion
extern void KeyParamInit();
extern void KeyScanInt(void);

//===========================================MOTOR==========================================//


//define 
#define PWM_TIM				TIM_5
#define PWM_L0				TIM_5_CH1_A00
#define PWM_L1				TIM_5_CH2_A01
#define PWM_R0				TIM_5_CH3_A02
#define PWM_R1				TIM_5_CH4_A03
//输出限幅 限制在1000以内
#define    pwm_limit(x)  		( (x)>0? ((x)>45000? ((x)=45000):(x)) : ((x)<-45000? ((x)=-45000):(x)) )

//statement
typedef struct
{
    uint16 differential;
    int16 expectSpeed;
    int32 pwm;
}Speed;
typedef struct
{
    uint16 expectSpeed;
    Speed  left;
    Speed  right;
    void (*function)(void);
}Motor;
extern Motor motor;
//fucntion
void MotorParamInit(void);
void PWM_Write(void);

//===========================================SERVO==============================================================================================================================//

typedef struct
{
    uint16 duty;
    int32  dutyAdd;
    int    expectPosition;
    int32  mid;
    void (*function)(void);
}Servo;
extern Servo servo;

//Function
extern void ServoParamInit(void);
//===============================================ENCODER======================================//

//检测出来的编码器方向
typedef enum
{
    positive,
    negative,
}EncoderDIREnum;

//statement
typedef struct
{
	int32 encoder;
	int16 now;
	int16 last;
	int16 acc;	
	int16 out;
    EncoderDIREnum dir;
}Encoder;


typedef struct
{
	int16 leftSpeed;
	int16 rightSpeed;
	int16 totalSpeed;
	int16 lastTotalSpeed;
	int16 DistanceAll;
    Encoder left;
    Encoder right;
    void (*function)(void);
}Wheel;

extern Wheel wheel;

//function
extern void EncoderParamInit();
extern void EncoderRead(void);

//===============================================Debug======================================// 
//extern bool debugOpen;

//extern bool servoOpen;
//extern bool stopProtectOpen;

#define DIALSWITCH1 D27
#define DIALSWITCH2 D4
typedef struct
{
    bool open;
    bool servo;
    bool stopProtect;
    void (*function)(void);
}Debug;
extern Debug debug;
extern void DebugParamInit();
extern void DebugOpen();
//===============================================Gyroscope======================================// 

#define AcceRatio   4096.0f
#define GyroRatio   16.4f
#define ACC_FILTER_NUM  2        // 加速度计滤波深度
#define GYRO_FILTER_NUM 2       // 陀螺仪滤波深度

extern int64 Circle_angle0;
extern int16 Circle_angle;

extern void ICM20602_Offset(void);  //置零偏
extern void GetGyroscope();
typedef struct
{
  int16 x;
  int16 y;
  int16 z;
}ICMDATA;

typedef struct
{
  ICMDATA  accdata;
  ICMDATA  gyro;
}ICM20602;

typedef struct
{
  float x;
  float y;
  float z;
}ICMDATA_Treated;


typedef struct
{
  ICMDATA_Treated  accdata;
  ICMDATA_Treated  gyro;
}ICM20602_Treated;

extern ICM20602_Treated IMC_Treated; //经过滑动处理的数据+

//===============================================LED======================================// 

#define REDPIN   PWM1_MODULE1_CHB_D15
#define BLUEPIN  PWM1_MODULE1_CHA_D14
#define GREENPIN PWM1_MODULE0_CHB_D13

typedef enum
{
	red = 0,
	green,
	blue,
    white,
}ColorEnum;

extern void LedInit(void);
extern void LedColor(ColorEnum Color);
#endif
