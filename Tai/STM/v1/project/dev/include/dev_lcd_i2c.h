#ifndef __I2C_LCD_H
#define __I2C_LCD_H

#include "stm32f10x.h"

#define PCF8574A_Address    0x27<<1						
#define I2C_Channel         I2C1                        //using I2C1 of STM32

extern void dev_systick_delay(uint16_t nTime);

void dev_lcd_i2c_config(void);                          //Configure pin for I2C_LCD
void dev_lcd_init(void);                                //Inititation LCD 
void dev_lcd_send_byte(char data);                      //Send byte to LCD using 4-bit mode
void dev_lcd_send_data(char data);                      //Send data to I2C
void dev_lcd_send_cmd(char data);                       //Send command to I2C
void dev_lcd_send_string(char *str);                    //Send string to I2C
void dev_lcd_send_char(char chr);                       //Send char to I2C
void dev_lcd_clear(void);                               //Clear LCD
void dev_lcd_goto_xy (int row, int col);                //Set cusor at row and colum on LCD
void dev_lcd_off_backlight(void);                       //Turn backlight Off
void dev_lcd_on_backlight(void);                        //Turn backlight On
#endif
