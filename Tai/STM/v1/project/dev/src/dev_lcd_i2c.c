#include "dev_lcd_i2c.h"

extern GPIO_InitTypeDef     GPIO_InitStructure;
I2C_InitTypeDef             I2C_InitStructure; 

void dev_lcd_send_byte(char data)
{
	// Send START condition
	I2C_GenerateSTART(I2C_Channel, ENABLE);
	// Test on EV5 and clear it 
	while (!I2C_CheckEvent(I2C_Channel, I2C_EVENT_MASTER_MODE_SELECT));
	// Send PCF8574A address for write 
	I2C_Send7bitAddress(I2C_Channel, PCF8574A_Address, I2C_Direction_Transmitter);
	// Test on EV6 and clear it 
	while (!I2C_CheckEvent(I2C_Channel, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
	// Send the byte to be writte
	I2C_SendData(I2C_Channel, data);
	// Test on EV8 and clear it
	while (!I2C_CheckEvent(I2C_Channel, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	// Send STOP condition
	I2C_GenerateSTOP(I2C_Channel, ENABLE);		
}
void dev_lcd_init(void)
{
	// Set I2C pinout
	dev_lcd_i2c_config();		
	dev_systick_delay(20);      //delay until power stable 
	//send 0x30 three time for set up LCD and send 0x20 for using 4-bit mode
	dev_lcd_send_cmd(0x33);     //send 0x30 first time, send 0x30 second time 
	dev_systick_delay(10);	
	dev_lcd_send_cmd(0x32);     //send 0x30 third time, send 0x20 for using 4-bit mode
	dev_systick_delay(10);
	
	// Start to set LCD function
	dev_lcd_send_cmd(0x28);
	dev_systick_delay(10);	
	// Set entry mode
	dev_lcd_send_cmd(0x06);	
	dev_systick_delay(10);
	// Set display on, cusor off 
	dev_lcd_send_cmd(0x0E);	
	dev_systick_delay(10);
	// Move cursor to home and set data address to 0
	dev_lcd_send_cmd(0x02);
	dev_systick_delay(10);	
	// Clear LCD
	dev_lcd_clear();
}

void dev_lcd_off_backlight(void)
{
	dev_lcd_clear();
	char data_h, data_l;
	uint8_t dataArr[4],i=0;
	char data =0x0A;
	data_h = data&0xf0;         //4 high bit of data
	data_l = (data<<4)&0xf0;    //4 low bit of data	
	dataArr[0] = data_h|0x04;   //backlight =0, en=1, rw=0, rs=0
	dataArr[1] = data_h|0x00;   //backlight =0, en=0, rw=0, rs=0
	dataArr[2] = data_l|0x04;   //backlight =0, en=1, rw=0, rs=0
	dataArr[3] = data_l|0x00;   //backlight =0, en=0, rw=0, rs=0
	for(i = 0;i<4;i++)
	{
		dev_lcd_send_byte(dataArr[i]);
	}
}

void dev_lcd_on_backlight(void)
{
	dev_lcd_clear();
	char data_h, data_l;
	uint8_t dataArr[4],i=0;
	char data =0x0E;
	data_h = data&0xf0;         //4 high bit of data
	data_l = (data<<4)&0xf0;    //4 low bit of data	
	dataArr[0] = data_h|0x0c;   //backlight =1, en=1, rw=0, rs=0
	dataArr[1] = data_h|0x08;   //backlight =1, en=0, rw=0, rs=0
	dataArr[2] = data_l|0x0c;   //backlight =1, en=1, rw=0, rs=0
	dataArr[3] = data_l|0x08;   //backlight =1, en=0, rw=0, rs=0
	for(i = 0;i<4;i++)
	{
		dev_lcd_send_byte(dataArr[i]);
	}
}

void dev_lcd_send_data(char data)
{
	char data_h, data_l;
	uint8_t dataArr[4],i=0;
	data_h = data&0xf0;         //4 high bit of data
	data_l = (data<<4)&0xf0;    //4 low bit of data	
	dataArr[0] = data_h|0x0d;   //backlight =1, en=1, rw=0, rs=1
	dataArr[1] = data_h|0x09;   //backlight =1, en=0, rw=0, rs=1
	dataArr[2] = data_l|0x0d;   //backlight =1, en=1, rw=0, rs=1
	dataArr[3] = data_l|0x09;   //backlight =1, en=0, rw=0, rs=1
	for(i = 0;i<4;i++)
	{
		dev_lcd_send_byte(dataArr[i]);
	}
}
void dev_lcd_send_cmd(char cmd)
{
	char data_h, data_l;
	uint8_t dataArr[4],i=0;
	data_h = cmd&0xf0;          //4 high bit of data
	data_l = (cmd<<4)&0xf0;     //4 low bit of data
	dataArr[0] = data_h|0x0c;   //backlight =1, en=1, rw=0, rs=0
	dataArr[1] = data_h|0x08;   //backlight =1, en=0, rw=0, rs=0
	dataArr[2] = data_l|0x0c;   //backlight =1, en=1, rw=0, rs=0
	dataArr[3] = data_l|0x08;   //backlight =1, en=0, rw=0, rs=0
	for(i = 0;i<4;i++)
	{
		dev_lcd_send_byte(dataArr[i]);
	}
}
void dev_lcd_send_string(char *str)
{
	while (*str) 
	{
		dev_lcd_send_data(*str++);
	}
}

void dev_lcd_send_char(char chr)
{
	dev_lcd_send_data(chr);
}
void dev_lcd_clear(void)
{
	dev_lcd_send_cmd(0x01);
	dev_systick_delay(10);
}

void dev_lcd_goto_xy(int row, int col)
{
	uint8_t pos_Addr;
	if(row == 1) 
	{
		pos_Addr = 0x80 | (0x00 + col); // 0x80 is intruction of Set DDram Address, 0x00 is start address of 1st row
	}
	else
	{
		pos_Addr = 0x80 | (0x40 + col);	// 0x80 is intruction of Set DDram Address. 0x40 is start address of 2nd row
	}
	dev_lcd_send_cmd(pos_Addr);
}

void dev_lcd_i2c_config(void)
{
	// Enable clock for GPIOB and I2C1
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);							
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

	// Set SDA and SCL pinout
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;      //PB6 - SCL, PB7 - SDA
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	// Configure I2C
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_InitStructure.I2C_OwnAddress1 = 0; 
	I2C_InitStructure.I2C_Ack = I2C_Ack_Disable;
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;       
	I2C_InitStructure.I2C_ClockSpeed = 100000;										
	I2C_Init(I2C1, &I2C_InitStructure);
	// Enable I2C
	I2C_Cmd(I2C1, ENABLE);
}
