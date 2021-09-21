#include "headfile.h"


/* 遗留问题:
		摄像头初始化过不去
    还未对flase 和true按键处理。
*/

/**
 *                             _ooOoo_
 *                            o8888888o
 *                            88" . "88
 *                            (| -_- |)
 *                            O\  =  /O
 *                         ____/`---'\____
 *                       .'  \\|     |//  `.
 *                      /  \\|||  :  |||//  \
 *                     /  _||||| -:- |||||-  \
 *                     |   | \\\  -  /// |   |
 *                     | \_|  ''\---/''  |   |
 *                     \  .-\__  `-`  ___/-. /
 *                   ___`. .'  /--.--\  `. . __
 *                ."" '<  `.___\_<|>_/___.'  >'"".
 *               | | :  `- \`.;`\ _ /`;.`/ - ` : | |
 *               \  \ `-.   \_ __\ /__ _/   .-` /  /
 *          ======`-.____`-.___\_____/___.-`____.-'======
 *                             `=---='
 *          ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
 *                     佛祖保佑        永无BUG
 *            佛曰:
 *                   写字楼里写字间，写字间里程序员；
 *                   程序人员写程序，又拿程序换酒钱。
 *                   酒醒只在网上坐，酒醉还来网下眠；
 *                   酒醉酒醒日复日，网上网下年复年。
 *                   但愿老死电脑间，不愿鞠躬老板前；
 *                   奔驰宝马贵者趣，公交自行程序员。
 *                   别人笑我忒疯癫，我笑自己命太贱；
 *                   不见满街漂亮妹，哪个归得程序员？
*/

int main(void)
{
    DisableGlobalIRQ();
    board_init();   	//务必保留，本函数用于初始化MPU 时钟 调试串口
	systick_delay_ms(300);	//延时300ms，等待主板其他外设上电成功
    
	while(!GlobalInit())
    {
    };
    
    CreatTask(beepTask,  BeepInt,           1,   true);
    CreatTask(motorTask, Cam_Motor_Control, 2,   true);
    CreatTask(servoTask, ServoMotorControl, 2,   true);
    CreatTask(lcdTask,   LcdDisplay,        150, true);

    CreatTask(gyroscopeTask, GetGyroscope,  2,   false);
    CreatTask(debugTask, DebugOpen,         10,  false);
    CreatTask(ANODTTask, ANODTSend,         6,   false);
    unsigned char i;
    systick_delay_ms(500);
    EnableGlobalIRQ(0);
    while(1)
    {
        for(i = 0; i < taskSum; i++)
        {
            if(task[i].status == run)
            {
                task[i].status = sleep;
                (*(task[i].function))();
            }
        }
    }
}




