#include "stm32f10x.h"
#include "dev_usart.h"
#include "dev_lcd_i2c.h"
#include "dev_keypad.h"
#include "dev_utilities.h"
#include <string.h>

GPIO_InitTypeDef        GPIO_InitStructure;
uint32_t                systick_timer_counter = 0;          //Counter variable for systick
unsigned char           usart_received_string[BUF_SIZE];    //Receiver arry for usart
uint8_t                 usart_received_string_i = 0;        //Index for receiver arry
uint8_t                 stop = 0;                           //stop flag when receive "OK" from esp
unsigned char           key;
uint8_t                 is_backlight_on = 1;
char           elec[BUF_SIZE];
char           water[BUF_SIZE];
char           temp[BUF_SIZE];
uint8_t                 i_temp = 0;	
volatile uint8_t        mode = 0;

void dev_systick_delay(uint16_t nTime);  

int main(void)
{
    SysTick_Config(SystemCoreClock/1000);           //Set clock for sysTick
    
    dev_uti_blink();                                // blink to indicate that program has been sucessfully downloaded
		dev_usart_usart2_init();
    dev_keypad_init();                              //Inititation Keypad
    dev_lcd_init();                                 //Inititation LCD  

    dev_uti_show_electric_water();                  //Display Electric and Water on LCD
    while (1)
    {
        key = dev_keypad_get_key();								
        switch (key)
        {
            case 'A':														
                if(mode==0)
                {                                       //Mode 0									
										strcpy(elec,temp);									//First time temp is elec, so we copy temp into elec
										dev_uti_string_clear(temp);					//Clear temp
                    dev_lcd_goto_xy(2,10);              //Jump cursor to Water
                    i_temp = 0;                         //Restart index of temp
                    mode = 1;                           //Move to mode 1
                }
                else if(mode==1)
                {                                       //Mode 1
										strcpy(water,temp); 								//Second time temp is water, so we copy temp into water	
										dev_uti_string_clear(temp);					//Clear temp
                    i_temp = 0;                         //Restart index of temp
                    dev_uti_make_confirmation();        //Show confirm dialog

                    do{
                        key = dev_keypad_get_key();
                        if(key=='A'){                   //Press 'A' to sending elec and water
														char ESP_str[100];
														dev_uti_format_send_data(elec,water,ESP_str);
														dev_usart_send_string(ESP_str,USART2);
                            dev_lcd_clear();
                            dev_lcd_send_string("Sending...");
                            dev_systick_delay(500);
                        }
                        else if(key=='B'){              //Press 'B' to cancling
                            dev_lcd_clear();
                            dev_lcd_send_string("Canceling...");
                            dev_systick_delay(500);
                        }
                    }while(key != 'A' && key != 'B');   //If not 'A' or 'B', loop until user press 'A' or 'B'

                    dev_uti_show_electric_water();      //Turn back show elec and water again
                    mode = 0;
                }
                break;
            case 'B':		
								dev_uti_string_clear(temp);
                i_temp = 0; 
                dev_lcd_clear();	
                mode = 0;
                dev_uti_show_electric_water();
                break;
            case 'C':
                break;
            case 'D':												
                if(!is_backlight_on)
                {                                       //If LCD is off, we can turn on
                    dev_lcd_on_backlight();
                    is_backlight_on=1;
                    dev_uti_show_electric_water();
                }
                else
                {                                       //If LCD is on, we can turn off
                    dev_lcd_off_backlight();
                    is_backlight_on=0;
                }
								mode = 0;
								dev_uti_string_clear(temp);
								i_temp=0;
                break;
            case 'n':                                   //User don't press key
                break;
            case '*':                                   //The other key is disable
                break;
            case '#':                                   //The other key is disable
                break;
            default:
                if(is_backlight_on)
                {
                    dev_lcd_send_char(key);						
                    temp[i_temp++]=key;                 //Get data from keypad into temp
                }
                break;
         }
    }
	
}

void dev_systick_delay(uint16_t nTime)              // Delay using sysTick
{
	systick_timer_counter = nTime;
	while(systick_timer_counter !=0)
	{
	}
}
