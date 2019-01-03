#ifndef __UTILITIES_H
#define __UTILITIES_H

#include <string.h>
#define ELEC "Electric: "
#define WATER "Water: "

#include "dev_lcd_i2c.h"

void dev_uti_show_electric_water(void);                         //Display Electric and Water on LCD
void dev_uti_make_confirmation(void);                           //Display confirm dailog on LCD
void dev_uti_string_clear(char *a);															//Clear string a
void dev_uti_format_send_data(char *a, char *b, char *result);								//Format string to send ESP
void dev_uti_blink(void);

#endif
