#ifndef __KEYPAD_H
#define __KEYPAD_H

#include "stm32f10x.h"

#define KEYPAD_ROWS_PORT GPIOA
#define KEYPAD_ROW_1_PIN GPIO_Pin_12
#define KEYPAD_ROW_2_PIN GPIO_Pin_11
#define KEYPAD_ROW_3_PIN GPIO_Pin_1
#define KEYPAD_ROW_4_PIN GPIO_Pin_0

#define KEYPAD_COLS_PORT GPIOB
#define KEYPAD_COL_1_PIN GPIO_Pin_15
#define KEYPAD_COL_2_PIN GPIO_Pin_14
#define KEYPAD_COL_3_PIN GPIO_Pin_13
#define KEYPAD_COL_4_PIN GPIO_Pin_12

void dev_keypad_init(void);
unsigned char dev_keypad_respond(void);
unsigned char dev_keypad_get_key(void);

#endif
