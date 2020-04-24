#ifndef __GPRS_USART_H
#define	__GPRS_USART_H

#include "stm32f10x.h"
#include <stdio.h>

extern void BSP_GPRS_USART_IRQHandler(void);

#define UART_BUFF_SIZE                255

// GPRS_GPRS使用的串口
#define  GPRS_USARTx                   USART2
#define  GPRS_USART_CLK                RCC_APB1Periph_USART2
#define  GPRS_USART_APBxClkCmd         RCC_APB1PeriphClockCmd
#define  GPRS_USART_BAUDRATE           115200

// USART GPIO 引脚宏定义
#define  GPRS_USART_GPIO_CLK           (RCC_APB2Periph_GPIOA)
    
#define  GPRS_TX_GPIO_PORT             GPIOA
#define  GPRS_TX_GPIO_PIN              GPIO_Pin_2
#define  GPRS_RX_GPIO_PORT             GPIOA
#define  GPRS_RX_GPIO_PIN              GPIO_Pin_3

#define  GPRS_USART_IRQ                USART2_IRQn

void GPRS_USART_Config(void);
void BSP_GPRS_USART_IRQHandler(void);
char *get_rebuff(uint8_t *len);
void clean_rebuff(void);

void GPRS_USART_printf(char *Data,...);
u8 Find_usart4(char *a);
void UartWriteBuffer_Config(const uint8_t *pBuffer,uint32_t NumByteToWrite);
void CLR_Buf4(void);
#endif /* __GPRS_USART_H */
