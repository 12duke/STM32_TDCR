/*
 * joy.c
 *
 *  Created on: Apr 16, 2024
 *      Author: Benny
 */

#ifndef INC_JOY_C_
#define INC_JOY_C_



#include "main.h"
#include <stdlib.h>
#include "ad.h"
#include "joy.h"

int32_t joy_x;
int32_t joy_y;
uint32_t joy_r;
float joy_a =0;
float joy_speed = 0;

ALIGN_32BYTES(__attribute__((section (".RAM1"))) int32_t joy_x_buf[100]);
ALIGN_32BYTES(__attribute__((section (".RAM1"))) int32_t joy_y_buf[100]);
ALIGN_32BYTES(__attribute__((section (".RAM1"))) uint32_t joy_r_buf[100]);


uint8_t joy_en = 1;
uint8_t joy_key3_button = 0;
uint8_t joy_button = 0;



void joy_xy(int32_t X_AD,int32_t Y_AD )
{

	static uint16_t key3_flag=0;
	static uint16_t joy_key_flag=0;
	static int8_t i=0;
	static int8_t i_last=0;
	static uint16_t joy_start=0;

	uint8_t key3=0;
	uint8_t joy_key=0;

	float data;


	key3 = HAL_GPIO_ReadPin(KEY3_GPIO_Port, KEY3_Pin);
	joy_key = HAL_GPIO_ReadPin(JOY_KEY_GPIO_Port, JOY_KEY_Pin);


	if(++i>=100)i=0;

	joy_x_buf[i] = joy_xzero-X_AD;
	joy_y_buf[i] = Y_AD-joy_yzero;

	joy_r_buf[i] = (uint32_t)sqrt(joy_x_buf[i]*joy_x_buf[i]+joy_y_buf[i]*joy_y_buf[i]);




	if(joy_r_buf[i]>joy_rinvalid){if(joy_start<110)joy_start++;}//遥控使能时间
	else joy_start = 0;


	i_last = (i+1)>=100 ? 0:(i+1);

	if(joy_start >= 110)
	{

		joy_x = joy_x_buf[i_last];
		joy_y = joy_y_buf[i_last];
		joy_r = joy_r_buf[i_last];


		if(joy_r>joy_rzero)
		{
			data = atan((double)joy_y_buf[i_last]/joy_x_buf[i_last])*to_angle;
			if(joy_x_buf[i_last]>0)joy_a = 90-data;
			else joy_a = 270-data;


			joy_r -=joy_rzero;
		//	if(joy_r > joy_rMax)joy_r = joy_rMax;

			joy_en  = 1;
		}
		else joy_r = 0;

			if(joy_r>joy_rMax)joy_r=joy_rMax;
	}
	else joy_en  = 0;


//	if(key3 == 0)                                           // 切换进远端控�????
//	{
//		if(key3_flag<10)key3_flag++;
//		else joy_key3_button = 1;
//	}
//
//	else
//	{
//		if(key3_flag>0)key3_flag--;
//		else joy_key3_button = 0;
//	}
	joy_key3_button = 0;

//	if(JOY_S<100)
//	{
//
//		if(joy_key_flag<10)joy_key_flag++;
//		else joy_button = 1;
//	}
//	else
//	{
//
//		if(joy_key_flag>0)joy_key_flag--;
//		else joy_button = 0;
//	}

	if(joy_key == 0)                                           // 切换进远端控�????
	{
		if(joy_key_flag<10)joy_key_flag++;
		else joy_button = 1;
	}

	else
	{
		if(joy_key_flag>0)joy_key_flag--;
		else joy_button = 0;
	}


}




#endif /* INC_JOY_C_ */
