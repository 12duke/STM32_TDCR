/*
 * vofa.c
 *
 *  Created on: Apr 9, 2024
 *      Author: Benny
 */

#include "main.h"
#include "vofa.h"


float vofa;

float vofa_Byte_to_Float(volatile uint8_t *p)
{
	unsigned long vofa_longdata = 0;
	float float_data = 0;

	vofa_longdata = (*p<< 0) + (*(p+1) << 8) + (*(p + 2) << 16) + (*(p + 3) << 24);

	float_data = *(float*)&vofa_longdata;

	return float_data;

}

