#include "headfile.h"


/* ��������:
		����ͷ��ʼ������ȥ
    ��δ��flase ��true��������
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
 *                     ���汣��        ����BUG
 *            ��Ի:
 *                   д��¥��д�ּ䣬д�ּ������Ա��
 *                   ������Աд�������ó��򻻾�Ǯ��
 *                   ����ֻ���������������������ߣ�
 *                   ��������ո��գ����������긴�ꡣ
 *                   ��Ը�������Լ䣬��Ը�Ϲ��ϰ�ǰ��
 *                   ���۱������Ȥ���������г���Ա��
 *                   ����Ц��߯��񲣬��Ц�Լ���̫����
 *                   ��������Ư���ã��ĸ���ó���Ա��
*/

int main(void)
{
    DisableGlobalIRQ();
    board_init();   	//��ر��������������ڳ�ʼ��MPU ʱ�� ���Դ���
	systick_delay_ms(300);	//��ʱ300ms���ȴ��������������ϵ�ɹ�
    
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




