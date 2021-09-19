#include "flash.h"

FlashInformation flashInformation;

void FlashInit()
{
    flash_init();            //初始化flash
    FlashWrite();
    FlashRead();
    
}

void FlashWrite()
{
        //pid
    flashInformation.temp_write[0][0] = float_conversion_uint32(pid.leftMotor.kp); 
    flashInformation.temp_write[0][1] = float_conversion_uint32(pid.leftMotor.ki);        
    flashInformation.temp_write[0][2] = float_conversion_uint32(pid.leftMotor.kd);
    
    flashInformation.temp_write[1][0] = float_conversion_uint32(pid.rightMotor.kp);
    flashInformation.temp_write[1][1] = float_conversion_uint32(pid.rightMotor.ki);  
    flashInformation.temp_write[1][2] = float_conversion_uint32(pid.rightMotor.kd);
    
//    temp_write[2][0] = float_conversion_uint32(turnPIDParam[0][0]);        
//    temp_write[2][1] = float_conversion_uint32(turnPIDParam[0][3]);   
//    temp_write[2][2] = float_conversion_uint32(turnPIDParam[0][4]);
    flashInformation.temp_write[2][0] = pid.servo.kp1;  
    flashInformation.temp_write[2][1] = pid.servo.kp2;
    flashInformation.temp_write[2][2] = pid.servo.kd;
    
    flashInformation.temp_write[3][0] = motor.expectSpeed;
    flashInformation.temp_write[3][1] = motor.left.differential;
    flashInformation.temp_write[3][2] = motor.right.differential;
    
    flashInformation.temp_write[4][0] = debug.servo;
    flashInformation.temp_write[4][1] = debug.stopProtect;
    
    if(flash_check(EXAMPLE_FLASH_SECTOR,EXAMPLE_FLASH_SECTOR_PAGE))//校验当前 扇区所在页是否有数据，如果有数据则擦除整个扇区
    {
        flashInformation.statusFlash = flash_erase_sector(EXAMPLE_FLASH_SECTOR);//擦除扇区，如果扇区已经有数据则必须擦除扇区之后才能再次写入新的数据
        if(flashInformation.statusFlash)  while(1);//擦除失败
    }
    
    flashInformation.write_buf[0]  = flashInformation.temp_write[0][0];
    flashInformation.write_buf[1]  = flashInformation.temp_write[0][1];
    flashInformation.write_buf[2]  = flashInformation.temp_write[0][2];
    flashInformation.write_buf[3]  = flashInformation.temp_write[1][0];
    flashInformation.write_buf[4]  = flashInformation.temp_write[1][1];
    flashInformation.write_buf[5]  = flashInformation.temp_write[1][2];
    flashInformation.write_buf[6]  = flashInformation.temp_write[2][0];
    flashInformation.write_buf[7]  = flashInformation.temp_write[2][1];
    flashInformation.write_buf[8]  = flashInformation.temp_write[2][2];
    flashInformation.write_buf[9]  = flashInformation.temp_write[3][0];
    flashInformation.write_buf[10] = flashInformation.temp_write[3][1];
    flashInformation.write_buf[11] = flashInformation.temp_write[3][2];
    flashInformation.write_buf[12] = flashInformation.temp_write[4][0];
    flashInformation.write_buf[13] = flashInformation.temp_write[4][1];
    
     
    flashInformation.statusFlash = flash_page_program(EXAMPLE_FLASH_SECTOR, EXAMPLE_FLASH_SECTOR_PAGE, flashInformation.write_buf, FLASH_SAVE_NUM);
    if(flashInformation.statusFlash)  while(1);//写入失败
    
}



void FlashRead()
{
    flash_read_page(EXAMPLE_FLASH_SECTOR, EXAMPLE_FLASH_SECTOR_PAGE, flashInformation.read_buf, FLASH_SAVE_NUM);
    
   
    pid.servo.kp1 = flashInformation.read_buf[6];
    pid.servo.kp2 = flashInformation.read_buf[7];
    pid.servo.kd  = flashInformation.read_buf[8];
    motor.expectSpeed = flashInformation.read_buf[9];
    motor.left.differential = flashInformation.read_buf[10];
    motor.right.differential = flashInformation.read_buf[11];
    debug.servo = flashInformation.read_buf[12];
    debug.stopProtect = flashInformation.read_buf[13];
    
    
}