#include "dev_usart.h"

extern GPIO_InitTypeDef     GPIO_InitStructure;
USART_InitTypeDef           USART_InitStructure;
NVIC_InitTypeDef            NVIC_InitStructure;
extern uint8_t              usart_received_string_i;


void dev_usart_usart2_init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

	// Set Tx mode AF_PP, Rx mode FLOATING  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;			//TX2 pinout
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;			//RX2 pinout
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);	

	// Configure usart component
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART_A_CHANNEL, &USART_InitStructure);
		
	// Set interrupt for usart
	NVIC_InitStructure.NVIC_IRQChannel = USART_A_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	// Clear interrupt flag for first time
	USART_ClearFlag(USART_A_CHANNEL, USART_IT_RXNE);

	// Enable interrupt reciever
	USART_ITConfig(USART_A_CHANNEL, USART_IT_RXNE, ENABLE);

	// Enable usart
	USART_Cmd(USART_A_CHANNEL, ENABLE);
}	

void dev_usart_usart3_init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);

	// Set Tx mode AF_PP, Rx mode FLOATING  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;			//TX1 pinout
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;			//RX1 pinout
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB, &GPIO_InitStructure);	

	// Configure usart component
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART_B_CHANNEL, &USART_InitStructure);
		
	// Set interrupt for usart
	NVIC_InitStructure.NVIC_IRQChannel = USART_B_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	// Clear interrupt flag for first time
	USART_ClearFlag(USART_B_CHANNEL, USART_IT_RXNE);

	// Enable interrupt receiver
	USART_ITConfig(USART_B_CHANNEL, USART_IT_RXNE, ENABLE);

	// Enable usart
	USART_Cmd(USART_B_CHANNEL, ENABLE);
}	

void dev_usart_send_char(char s, USART_TypeDef* USARTx)
{
	// Wait until transmit data register is empty
	while (!USART_GetFlagStatus(USARTx, USART_FLAG_TXE));
	// Send a char using USARTx
	USART_SendData(USARTx, s);
}

void dev_usart_send_string(char* s,USART_TypeDef* USARTx)
{
  while (*s)
	{
		dev_usart_send_char(*s++,USARTx);
	}                                 
}

void dev_usart_clear_buf(char *s)
{
	while (*s)
	{
		*s++='\0';
	}
	usart_received_string_i=0;
}
