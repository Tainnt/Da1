#include "stm32f10x.h"
#include "dev_keypad.h"
#include "dev_usart.h"

extern GPIO_InitTypeDef     GPIO_InitStructure;

void dev_keypad_init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = KEYPAD_ROW_1_PIN + KEYPAD_ROW_2_PIN + KEYPAD_ROW_3_PIN + KEYPAD_ROW_4_PIN + GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(KEYPAD_ROWS_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = KEYPAD_COL_1_PIN + KEYPAD_COL_2_PIN + KEYPAD_COL_3_PIN + KEYPAD_COL_4_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(KEYPAD_COLS_PORT, &GPIO_InitStructure);
}

unsigned char dev_keypad_respond(void)
{
	/* Quet ROW_1 */
	GPIO_ResetBits(KEYPAD_ROWS_PORT, KEYPAD_ROW_1_PIN);
	GPIO_SetBits(KEYPAD_ROWS_PORT, KEYPAD_ROW_2_PIN + KEYPAD_ROW_3_PIN + KEYPAD_ROW_4_PIN);
	if(GPIO_ReadInputDataBit(KEYPAD_COLS_PORT, KEYPAD_COL_1_PIN) == 0)
		{dev_systick_delay(250); while(GPIO_ReadInputDataBit(KEYPAD_COLS_PORT, KEYPAD_COL_1_PIN) == 0){} return '1';}
	if(GPIO_ReadInputDataBit(KEYPAD_COLS_PORT, KEYPAD_COL_2_PIN) == 0)
		{dev_systick_delay(250); while(GPIO_ReadInputDataBit(KEYPAD_COLS_PORT, KEYPAD_COL_2_PIN) == 0){} return '2';}
	if(GPIO_ReadInputDataBit(KEYPAD_COLS_PORT, KEYPAD_COL_3_PIN) == 0)
		{dev_systick_delay(250); while(GPIO_ReadInputDataBit(KEYPAD_COLS_PORT, KEYPAD_COL_3_PIN) == 0){} return '3';}
	if(GPIO_ReadInputDataBit(KEYPAD_COLS_PORT, KEYPAD_COL_4_PIN) == 0)
		{dev_systick_delay(250); while(GPIO_ReadInputDataBit(KEYPAD_COLS_PORT, KEYPAD_COL_4_PIN) == 0){} return 'A';}
		
	/* Quet ROW_2 */
	GPIO_ResetBits(KEYPAD_ROWS_PORT, KEYPAD_ROW_2_PIN);
	GPIO_SetBits(KEYPAD_ROWS_PORT, KEYPAD_ROW_1_PIN + KEYPAD_ROW_3_PIN + KEYPAD_ROW_4_PIN);
	if(GPIO_ReadInputDataBit(KEYPAD_COLS_PORT, KEYPAD_COL_1_PIN) == 0)
		{dev_systick_delay(250); while(GPIO_ReadInputDataBit(KEYPAD_COLS_PORT, KEYPAD_COL_1_PIN) == 0){} return '4';}
	if(GPIO_ReadInputDataBit(KEYPAD_COLS_PORT, KEYPAD_COL_2_PIN) == 0)
		{dev_systick_delay(250); while(GPIO_ReadInputDataBit(KEYPAD_COLS_PORT, KEYPAD_COL_2_PIN) == 0){} return '5';}
	if(GPIO_ReadInputDataBit(KEYPAD_COLS_PORT, KEYPAD_COL_3_PIN) == 0)
		{dev_systick_delay(250); while(GPIO_ReadInputDataBit(KEYPAD_COLS_PORT, KEYPAD_COL_3_PIN) == 0){} return '6';}
	if(GPIO_ReadInputDataBit(KEYPAD_COLS_PORT, KEYPAD_COL_4_PIN) == 0)
		{dev_systick_delay(250); while(GPIO_ReadInputDataBit(KEYPAD_COLS_PORT, KEYPAD_COL_4_PIN) == 0){} return 'B';}
		
	/* Quet ROW_3 */
	GPIO_ResetBits(KEYPAD_ROWS_PORT, KEYPAD_ROW_3_PIN);
	GPIO_SetBits(KEYPAD_ROWS_PORT, KEYPAD_ROW_1_PIN + KEYPAD_ROW_2_PIN + KEYPAD_ROW_4_PIN);
	if(GPIO_ReadInputDataBit(KEYPAD_COLS_PORT, KEYPAD_COL_1_PIN) == 0)
		{dev_systick_delay(250); while(GPIO_ReadInputDataBit(KEYPAD_COLS_PORT, KEYPAD_COL_1_PIN) == 0){} return '7';}
	if(GPIO_ReadInputDataBit(KEYPAD_COLS_PORT, KEYPAD_COL_2_PIN) == 0)
		{dev_systick_delay(250); while(GPIO_ReadInputDataBit(KEYPAD_COLS_PORT, KEYPAD_COL_2_PIN) == 0){} return '8';}
	if(GPIO_ReadInputDataBit(KEYPAD_COLS_PORT, KEYPAD_COL_3_PIN) == 0)
		{dev_systick_delay(250); while(GPIO_ReadInputDataBit(KEYPAD_COLS_PORT, KEYPAD_COL_3_PIN) == 0){} return '9';}
	if(GPIO_ReadInputDataBit(KEYPAD_COLS_PORT, KEYPAD_COL_4_PIN) == 0)
		{dev_systick_delay(250); while(GPIO_ReadInputDataBit(KEYPAD_COLS_PORT, KEYPAD_COL_4_PIN) == 0){} return 'C';}
		
	/* Quet ROW_4 */
	GPIO_ResetBits(KEYPAD_ROWS_PORT, KEYPAD_ROW_4_PIN);
	GPIO_SetBits(KEYPAD_ROWS_PORT, KEYPAD_ROW_1_PIN + KEYPAD_ROW_2_PIN + KEYPAD_ROW_3_PIN);
	if(GPIO_ReadInputDataBit(KEYPAD_COLS_PORT, KEYPAD_COL_1_PIN) == 0)
		{dev_systick_delay(250); while(GPIO_ReadInputDataBit(KEYPAD_COLS_PORT, KEYPAD_COL_1_PIN) == 0){} return '*';}
	if(GPIO_ReadInputDataBit(KEYPAD_COLS_PORT, KEYPAD_COL_2_PIN) == 0)
		{dev_systick_delay(250); while(GPIO_ReadInputDataBit(KEYPAD_COLS_PORT, KEYPAD_COL_2_PIN) == 0){} return '0';}
	if(GPIO_ReadInputDataBit(KEYPAD_COLS_PORT, KEYPAD_COL_3_PIN) == 0)
		{dev_systick_delay(250); while(GPIO_ReadInputDataBit(KEYPAD_COLS_PORT, KEYPAD_COL_3_PIN) == 0){} return '#';}
	if(GPIO_ReadInputDataBit(KEYPAD_COLS_PORT, KEYPAD_COL_4_PIN) == 0)
		{dev_systick_delay(250); while(GPIO_ReadInputDataBit(KEYPAD_COLS_PORT, KEYPAD_COL_4_PIN) == 0){} return 'D';}
		
	return 'n';
}

unsigned char dev_keypad_get_key(void)
{
	unsigned char key = 'n';
	while(key == 'n')
	{
		key = dev_keypad_respond();
	}
	return key;
}

