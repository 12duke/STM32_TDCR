#include "main.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include "printf.h"

uint8_t printf_mode = 0;
uint16_t printf_cnt = 0;
uint32_t printf_time =0;

ALIGN_32BYTES(__attribute__((section (".RAM1"))) xUSATR_TypeDef xUSART1);
ALIGN_32BYTES(__attribute__((section (".RAM1"))) xUSATR_TypeDef xUSART2);
ALIGN_32BYTES(__attribute__((section (".RAM1"))) xUSATR_TypeDef xUSART3);

void printf_init(void)
{
	__HAL_DMA_DISABLE_IT(&hdma_usart1_rx,DMA_IT_HT);
	while(HAL_UARTEx_ReceiveToIdle_DMA(&huart1, xUSART1.ReceiveTemp, ReceiveNum) != HAL_OK)\
	HAL_UART_AbortReceive(&huart1);

	__HAL_DMA_DISABLE_IT(&hdma_usart2_rx,DMA_IT_HT);
	while(HAL_UARTEx_ReceiveToIdle_DMA(&huart2, xUSART2.ReceiveTemp, ReceiveNum) != HAL_OK)\
	HAL_UART_AbortReceive(&huart2);

	__HAL_DMA_DISABLE_IT(&hdma_usart3_rx,DMA_IT_HT);
	while(HAL_UARTEx_ReceiveToIdle_DMA(&huart3, xUSART3.ReceiveTemp, ReceiveNum) != HAL_OK)\
	HAL_UART_AbortReceive(&huart3);

	xUSART1.ReceiveN = 0;
	xUSART2.ReceiveN = 0;
	xUSART3.ReceiveN = 0;
}


void printf1(const char *format,...)
{
	//  static char  strTem[100] = "Hello World! d\n";
	uint32_t length;
	va_list args;
	va_start(args, format);
	while(huart1.gState != HAL_UART_STATE_READY);
	length = vsnprintf (xUSART1.TransmitTemp,TransmitNum,format, args);
	va_end(args);
//	SCB_CleanDCache_by_Addr((uint32_t *)xUSART1.TransmitTemp, length);
	HAL_UART_Transmit_DMA(&huart1,(uint8_t*)xUSART1.TransmitTemp,length);
}

void printf2(const char *format,...)
{
	//  static char  strTem[100] = "Hello World! d\n";
	uint32_t length;
	va_list args;
	va_start(args, format);
	while(huart2.gState != HAL_UART_STATE_READY);
	length = vsnprintf (xUSART2.TransmitTemp,TransmitNum,format, args);
	va_end(args);
//	SCB_CleanDCache_by_Addr((uint32_t *)xUSART2.TransmitTemp, length);
	HAL_UART_Transmit_DMA(&huart2,(uint8_t*)xUSART2.TransmitTemp,length);
}

void printf3(const char *format,...)
{
	//  static char  strTem[100] = "Hello World! d\n";
	uint32_t length;
	va_list args;

	va_start(args, format);
	while(huart3.gState != HAL_UART_STATE_READY);
	length = vsnprintf (xUSART3.TransmitTemp,TransmitNum,format, args);
	va_end(args);
//	SCB_CleanDCache_by_Addr((uint32_t *)xUSART3.TransmitTemp, length);
	HAL_UART_Transmit_DMA(&huart3,(uint8_t*)xUSART3.TransmitTemp,length);
}


void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
	HAL_UART_DMAStop(huart);
    if (huart == &huart1)
    {
		xUSART1.ReceiveN  = Size;


//        SCB_InvalidateDCache_by_Addr((uint32_t *)xUSART1.ReceiveTemp, ReceiveNum);

        memset(xUSART1.ReceiveData, 0, Size);
        memcpy(xUSART1.ReceiveData, xUSART1.ReceiveTemp, Size);


//        SCB_CleanDCache_by_Addr((uint32_t *)xUSART1.ReceiveData, Size); // 回传
//		HAL_UART_Transmit_DMA(&huart1,(uint8_t*)xUSART1.ReceiveData,Size);

        HAL_UART_AbortReceive(&huart1);
        HAL_UARTEx_ReceiveToIdle_DMA(&huart1, xUSART1.ReceiveTemp, ReceiveNum);


    }
    else if (huart == &huart2)
    {
		xUSART2.ReceiveN  = Size;


 //       SCB_InvalidateDCache_by_Addr((uint32_t *)xUSART2.ReceiveTemp, ReceiveNum);

        memset(xUSART2.ReceiveData, 0, Size);
        memcpy(xUSART2.ReceiveData, xUSART2.ReceiveTemp, Size);


//        SCB_CleanDCache_by_Addr((uint32_t *)xUSART2.ReceiveData, Size); // 回传
//		HAL_UART_Transmit_DMA(&huart1,(uint8_t*)xUSART2.ReceiveData,Size);

        HAL_UART_AbortReceive(&huart2);
        HAL_UARTEx_ReceiveToIdle_DMA(&huart2, xUSART2.ReceiveTemp, ReceiveNum);
    }
    else if (huart == &huart3)
    {
		xUSART3.ReceiveN  = Size;


 //       SCB_InvalidateDCache_by_Addr((uint32_t *)xUSART3.ReceiveTemp, ReceiveNum);

        memset(xUSART3.ReceiveData, 0, Size);
        memcpy(xUSART3.ReceiveData, xUSART3.ReceiveTemp, Size);


//        SCB_CleanDCache_by_Addr((uint32_t *)xUSART3.ReceiveData, Size); // 回传
//		HAL_UART_Transmit_DMA(&huart1,(uint8_t*)xUSART3.ReceiveData,Size);

        HAL_UART_AbortReceive(&huart3);
        HAL_UARTEx_ReceiveToIdle_DMA(&huart3, xUSART3.ReceiveTemp, ReceiveNum);
    }
}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
    if (huart == &huart1)
    {
    	HAL_UART_AbortReceive(&huart1);
        HAL_UARTEx_ReceiveToIdle_DMA(&huart1, xUSART1.ReceiveTemp, ReceiveNum);
    }
    else if (huart == &huart2)
    {
    	HAL_UART_AbortReceive(&huart2);
        HAL_UARTEx_ReceiveToIdle_DMA(&huart2, xUSART2.ReceiveTemp, ReceiveNum);
    }
    else if (huart == &huart3)
    {
    	HAL_UART_AbortReceive(&huart3);
        HAL_UARTEx_ReceiveToIdle_DMA(&huart3, xUSART3.ReceiveTemp, ReceiveNum);
    }
}


