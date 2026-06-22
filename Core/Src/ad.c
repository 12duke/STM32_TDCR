/*
 * ad.c
 *
 *  Created on: Jun 18, 2024
 *      Author: Benny
 */
#include "main.h"
#include "adc.h"
#include "ad.h"
#include "printf.h"

ALIGN_32BYTES(__attribute__((section (".RAM1"))) uint32_t ADC1_buf[ADC1_BUFFER_SIZE]);
ALIGN_32BYTES(__attribute__((section (".RAM3"))) uint16_t ADC3_buf[ADC3_BUFFER_SIZE]);

uint16_t *ADC1_data[ADC1_BUFFER_SIZE*2];
float ADC3_Values[ADC3_BUFFER_SIZE];

uint32_t ADC1_flag = 0;
uint32_t ADC3_flag = 0;


void adc_init(void)
{

	uint8_t i = 0;
    HAL_Delay(100);

    if (HAL_ADCEx_Calibration_Start(&hadc1, ADC_CALIB_OFFSET, ADC_SINGLE_ENDED) != HAL_OK)
    {
        printf1("hadc1 error with HAL_ADCEx_Calibration_Start\r\n");
        Error_Handler();
    }
    if (HAL_ADCEx_Calibration_Start(&hadc2, ADC_CALIB_OFFSET, ADC_SINGLE_ENDED) != HAL_OK)
    {
        printf1("hadc2 error with HAL_ADCEx_Calibration_Start\r\n");
        Error_Handler();
    }

    if (HAL_ADCEx_Calibration_Start(&hadc3, ADC_CALIB_OFFSET, ADC_SINGLE_ENDED) != HAL_OK)
    {
        printf1("hadc3 error with HAL_ADCEx_Calibration_Start\r\n");
        Error_Handler();
    }



   if (HAL_ADCEx_MultiModeStart_DMA(&hadc1, (uint32_t *)ADC1_buf, ADC1_BUFFER_SIZE) != HAL_OK)
    {
	   printf1("hadc1 error with HAL_ADC_Start_DMA\r\n");
       Error_Handler();
    }

    if (HAL_ADC_Start_DMA(&hadc3, (uint32_t *)ADC3_buf, ADC3_BUFFER_SIZE) != HAL_OK)
    {
        printf1("hadc3 error with HAL_ADC_Start_DMA\r\n");
        Error_Handler();
    }

     for(i=0;i<ADC1_BUFFER_SIZE;i++)
	 {
    	 ADC1_data[i] = ((uint16_t*)&ADC1_buf[i]);
    	 ADC1_data[ADC1_BUFFER_SIZE+i] = ((uint16_t*)&ADC1_buf[i])+1;
	 }



}
void adc3_transform(void)
{

	uint16_t TS_CAL1;
	uint16_t TS_CAL2;


    TS_CAL1 = *(__IO uint16_t *)(0x1FF1E820);
    TS_CAL2 = *(__IO uint16_t *)(0x1FF1E840);

    ADC3_Values[1] = ((110.0f - 30.0f) / (TS_CAL2 - TS_CAL1)) * (ADC3_buf[1] - TS_CAL1) + 30.0f;
    ADC3_Values[2] = 3.3 **(VREFINT_CAL_ADDR)/ ADC3_buf[2];
    ADC3_Values[0] = Vref*ADC3_buf[0]/0xFFFF;

}
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
   if(hadc->Instance == ADC1)
   {
       ADC1_flag++;               //  采样次数  591 每秒
    //   SCB_InvalidateDCache_by_Addr((uint32_t *) &ADC1_buf, ADC1_BUFFER_SIZE);
   } else if(hadc->Instance == ADC3)
   {
       ADC3_flag++;               //  采样次数  1377 每秒
     //  SCB_InvalidateDCache_by_Addr((uint32_t *) &ADC3_buf, ADC3_BUFFER_SIZE);


   }

}

