#ifndef __PRINTF_H
#define __PRINTF_H



#define printf_cnt_time  100

#define TransmitNum 512
#define ReceiveNum 512

#define frame_header 0x55
#define frame_tail   0xAA



extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart3;
extern DMA_HandleTypeDef hdma_usart1_rx;
extern DMA_HandleTypeDef hdma_usart1_tx;
extern DMA_HandleTypeDef hdma_usart2_tx;
extern DMA_HandleTypeDef hdma_usart2_rx;
extern DMA_HandleTypeDef hdma_usart3_rx;
extern DMA_HandleTypeDef hdma_usart3_tx;


extern char *device_number;
typedef struct                         // 声明一个结构体，方便管理变量
{
	char   TransmitTemp[TransmitNum];
    uint8_t   ReceiveData[ReceiveNum];        // 接收到的数据
    uint8_t   ReceiveTemp[ReceiveNum];           // 接收缓存; 注意：这个数组，只是一个缓存，用于DMA逐个字节接收，当接收完一帧后，数据在回调函数中，转存到 ReceiveData[ ] 存放。即：双缓冲，有效减少单缓冲的接收过程新数据覆盖旧数据
    uint16_t  ReceiveN;              // 接收字节数
} xUSATR_TypeDef;

extern uint8_t printf_mode;
extern uint16_t printf_cnt;
extern uint32_t printf_time;

extern xUSATR_TypeDef xUSART1 ;        // 定义结构体，方便管理变量。也可以不用结构体，用单独的变量
extern xUSATR_TypeDef xUSART2 ;        // 定义结构体，方便管理变量。也可以不用结构体，用单独的变量
extern xUSATR_TypeDef xUSART3 ;        // 定义结构体，方便管理变量。也可以不用结构体，用单独的变量
void printf_init(void);
void printf1(const char *format,...);
void printf2(const char *format,...);
void printf3(const char *format,...);
void printf_log(const char * sFormat, ...);
void segger_rtt_init(char * str);

#endif
