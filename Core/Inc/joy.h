/*
 * joy.h
 *
 *  Created on: Apr 16, 2024
 *      Author: Benny
 */

#ifndef INC_JOY_H_
#define INC_JOY_H_


#define joy_xzero 32616
#define joy_yzero 30982


#define joy_rinvalid 7000
#define joy_rzero    10000
#define joy_rMax 20000



//#define joy_rinvalid 7000
//#define joy_rMax (10000)



#define PI 3.14159265
#define to_angle  180.0/PI

extern int32_t joy_x;
extern int32_t joy_y;
extern uint32_t joy_r;

extern float joy_a ;

extern uint8_t joy_en;
extern uint8_t joy_key3_button;
extern uint8_t joy_button;


void joy_xy(int32_t X_AD,int32_t Y_AD);

#endif /* INC_JOY_H_ */
