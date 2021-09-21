#include "pid.h"

Pid pid;  

		
		void PidParamInit(void)
{
    //有可能改变
    pid.servo.kp1 = 6;
    pid.servo.kp2 = 37;
    pid.servo.kd  = 0;
    pid.leftMotor.kp  = 10;
    pid.leftMotor.ki  = 7;
    pid.leftMotor.kd  = 0;
    pid.rightMotor.kp = 10;
    pid.rightMotor.ki = 7;
    pid.rightMotor.kd = 0;
    //基本不变
    pid.servo.kp1Sum = 0;
    pid.servo.kp2Sum = 0;
    pid.servo.kpSum  = 0;
    pid.servo.kdSum  = 0;
    pid.leftMotor.error  = 0;
    pid.leftMotor.errorPre  = 0;
    pid.leftMotor.errorPrePre  = 0;
    pid.rightMotor.error = 0;
    pid.rightMotor.errorPre = 0;
    pid.rightMotor.errorPrePre = 0; 
		
}



void fuzzy (void)
{		
//		int E[7] = {-450,-300,-150, 0, 150, 300, 450,};
//		int Ec[7]= { -60, -40, -20, 0,  20,  40,  60,};
//		int En[7]= {  -9,  -6,  -3, 0,   3,   6,   9,};
//		int fuzzykp[7][7]={
//			//   				 Kp模糊规则表
//			//							 Ec
//			// 		 -9,-6,-3, 0, 3, 6, 9
//						{ 9, 9, 9, 9, 6, 0, 0,},//-9
//						{ 9, 9, 9, 6, 6, 0, 0,},//-6
//						{ 9, 6, 6, 3, 0,-3,-6,},//-3
//						{ 6, 6, 3, 0,-3,-6,-6,},// 0		E
//						{ 3, 3, 0,-6,-6,-6,-9,},// 3
//						{ 0, 0, 0,-6,-9,-9,-9,},// 6
//						{ 0,-3,-9,-9,-9,-9,-9,} // 9
//				};
//	if(camServoError<=E[6] && camServoError>=E[0]){
//		if(camServoError<=E[1]){
//			pid.x.Ea = (E[1] - camServoError)/(E[1] - E[0]);
//			li = 1;
//		}else if(camServoError<=E[2]){
//			Ea = (E[2] - camServoError)/(E[2] - E[1]);
//			li = 2;
//		}else if(camServoError<=E[3]){
//			Ea = (E[3] - camServoError)/(E[3] - E[2]);
//			li = 3;
//		}else if(camServoError<=E[4]){
//			Ea = (E[4] - camServoError)/(E[4] - E[3]);
//			li = 4;
//		}else if(camServoError<=E[5]){
//			Ea = (E[5] - camServoError)/(E[5] - E[4]);
//			li = 5;
//		}else if(camServoError<=E[6]){
//			Ea = (E[6] - camServoError)/(E[6] - E[5]);
//			li = 6;
//		};    
//	};
//	if(camServoErrorAdd<=Ec[6] && camServoErrorAdd>=Ec[0]){
//		if(camServoErrorAdd<=Ec[1]){
//			Eb = (Ec[1] - camServoErrorAdd)/(Ec[1] - Ec[0]);
//			j = 1;
//		}else if(camServoErrorAdd<=Ec[2]){
//			Eb = (Ec[2] - camServoErrorAdd)/(Ec[2] - Ec[1]);
//			j = 2;
//		}else if(camServoErrorAdd<=Ec[3]){
//			Eb = (Ec[3] - camServoErrorAdd)/(Ec[3] - Ec[2]);
//			j = 3;
//		}else if(camServoErrorAdd<=Ec[4]){
//			Eb = (Ec[4] - camServoErrorAdd)/(Ec[4] - Ec[3]);
//			j = 4;
//		}else if(camServoErrorAdd<=Ec[5]){
//			Eb = (Ec[5] - camServoErrorAdd)/(Ec[5] - Ec[4]);
//			j = 5;
//		}else if(camServoErrorAdd<=Ec[6]){
//			Eb = (Ec[6] - camServoErrorAdd)/(Ec[6] - Ec[5]);
//			j = 6;
//		};    
//	};
//	pid.servo.kp1 = Ea*Eb*fuzzykp[li][j] +
//									(1-Ea)*Eb*fuzzykp[li-1][j] +
//									Ea*(1-Eb)*fuzzykp[li][j-1] +
//									(1-Ea)*(1-Eb)*fuzzykp[li-1][j-1];
}

