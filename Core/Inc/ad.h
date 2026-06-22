/*
 * ad.h
 *
 *  Created on: Jun 18, 2024
 *      Author: Benny
 */

#ifndef INC_AD_H_
#define INC_AD_H_

#define ADC1_BUFFER_SIZE 7
#define ADC3_BUFFER_SIZE 3



extern uint32_t ADC1_flag;
extern uint32_t ADC3_flag;

extern uint32_t ADC1_buf[ADC1_BUFFER_SIZE];
extern uint16_t ADC3_buf[ADC3_BUFFER_SIZE];
extern uint16_t *ADC1_data[ADC1_BUFFER_SIZE*2];
extern float ADC3_Values[ADC3_BUFFER_SIZE];


#define AD_max 65535

#define Vref ADC3_Values[2]

#define Ins *ADC1_data[6]
#define JOY_S *ADC1_data[10]
#define proximal_rotating_AD *ADC1_data[9]
#define diatal_rotating_AD *ADC1_data[12]
#define JOY_S2 *ADC1_data[11]

#define JOY_X (*ADC1_data[0])
#define JOY_Y (*ADC1_data[1])

#define multiplying 100
//#define sampling_resistor 50
//#define A4 200

#define M1_current multiplying*3.3**ADC1_data[2]/0xFFFF
#define M2_current multiplying*3.3**ADC1_data[3]/0xFFFF
#define M3_current multiplying*3.3**ADC1_data[4]/0xFFFF
#define M4_current multiplying*3.3**ADC1_data[5]/0xFFFF


void adc_init(void);
void adc3_transform(void);
float get_CPU_Temperature(void);

#endif /* INC_AD_H_ */
