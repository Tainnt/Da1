#ifndef __uart_H
#define __uart_H

#include "stm32f10x.h"
				
#define USART_A_CHANNEL             USART2				//using USART2 of STM32
#define USART_A_IRQn                USART2_IRQn

#define USART_B_CHANNEL             USART3				//using USART3 of STM32
#define USART_B_IRQn                USART3_IRQn

#define BUF_SIZE                    100

extern void dev_systick_delay(uint16_t nTime);
void dev_usart_clear_buf(char *s);
void dev_usart_usart2_init(void);
void dev_usart_usart3_init(void);
void dev_usart_send_char(char s, USART_TypeDef* USARTx);
void dev_usart_send_string(char* s,USART_TypeDef* USARTx);
#endif
