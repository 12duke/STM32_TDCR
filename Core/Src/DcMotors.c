/*
 * DcMotors.c
 *
 *  Created on: Jun 3, 2024
 *      Author: Bunny
 */

#include "main.h"
#include "DcMotors.h"
#include "printf.h"
#include "ad.h"
#include "joy.h"
#include <stdlib.h>

xMotors M1;
xMotors M2;
xMotors M3;
xMotors M4;

/*uint32_t whole_dis = 0;
uint32_t Inspire_buf = 0;
uint8_t Inspire_flag = 0;
电推杠相关*/

//uint8_t position_mode = 0;
uint16_t motor_homeing_n = 0;
uint16_t distal_homeing_n;
uint16_t proximal_homeing_n;
uint8_t button_home_flag = 0;
uint16_t button_home_time = 0;

float proximal_rotating_angle = 0;
float proximal_bending_circle = 0;
uint8_t proximal_status = 0;

float distal_rotating_angle = 0;
float distal_bending_circle = 0;
uint8_t distal_status = 0;

float proximal_bending_circle_vangle = 0;

float distal_bending_circle_vangle = 0;




uint8_t proximal_rotating_en = 0;
uint8_t proximal_bending_en = 0;
uint8_t distal_rotating_en = 0;
uint8_t distal_bending_en = 0;


void xM_init(xMotors *xM)
{
	xM->overflow = 0;
	xM->enc_now_total = 0;
	xM->enc_now_total_last = 0;
	xM->enc_Target = 0;
	xM->enc_Difference = 0;

	xM->busy = 0;
	xM->rpm_Target = 0;
	xM->PWM = 0;
	xM->state = 0;
}
void motor_init(void)
{

	__HAL_TIM_SET_COMPARE(&htim4,TIM_CHANNEL_1,PWM_val_stop);
	__HAL_TIM_SET_COMPARE(&htim4,TIM_CHANNEL_2,PWM_val_stop);
	//__HAL_TIM_SET_COMPARE(&htim4,TIM_CHANNEL_3,PWM_val_stop);
	//__HAL_TIM_SET_COMPARE(&htim4,TIM_CHANNEL_4,PWM_val_stop);

  HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_2);
  //HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_3);
  //HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_4);



	__HAL_TIM_CLEAR_IT(&htim1,TIM_IT_UPDATE);
	__HAL_TIM_CLEAR_IT(&htim2,TIM_IT_UPDATE);
	//__HAL_TIM_CLEAR_IT(&htim3,TIM_IT_UPDATE);
	//__HAL_TIM_CLEAR_IT(&htim8,TIM_IT_UPDATE);


	HAL_TIM_Encoder_Start(&htim1, TIM_CHANNEL_1|TIM_CHANNEL_2);
	HAL_TIM_Encoder_Start(&htim2, TIM_CHANNEL_1|TIM_CHANNEL_2);
	//HAL_TIM_Encoder_Start(&htim3, TIM_CHANNEL_1|TIM_CHANNEL_2);
	//HAL_TIM_Encoder_Start(&htim8, TIM_CHANNEL_1|TIM_CHANNEL_2);


	HAL_TIM_Base_Start_IT(&htim1);
	HAL_TIM_Base_Start_IT(&htim2);
	//HAL_TIM_Base_Start_IT(&htim3);
	//HAL_TIM_Base_Start_IT(&htim8);


	M1.GPIOx = M1_DIR_OUT_GPIO_Port;
	M1.GPIO_Pin =  M1_DIR_OUT_Pin;
	M1.PWM_CHANNEL = TIM_CHANNEL_1;
	M1_enc=0;

	M2.GPIOx = M2_DIR_OUT_GPIO_Port;
	M2.GPIO_Pin =  M2_DIR_OUT_Pin;
	M2.PWM_CHANNEL = TIM_CHANNEL_2;
	M2_enc=0;


	/*M3.GPIOx = M3_DIR_OUT_GPIO_Port;
	M3.GPIO_Pin =  M3_DIR_OUT_Pin;
	M3.PWM_CHANNEL = TIM_CHANNEL_3;
	M3_enc=0;


	M4.GPIOx = M4_DIR_OUT_GPIO_Port;
	M4.GPIO_Pin =  M4_DIR_OUT_Pin;
	M4.PWM_CHANNEL = TIM_CHANNEL_4;
	M4_enc=0;
*/
	xM_init(&M1);
	xM_init(&M2);
	//xM_init(&M3);
	//xM_init(&M4);

	__HAL_TIM_SET_COUNTER(&htim1,enc_default);
	__HAL_TIM_SET_COUNTER(&htim2,enc_default);
	//__HAL_TIM_SET_COUNTER(&htim3,enc_default);
	//__HAL_TIM_SET_COUNTER(&htim8,enc_default);


	proximal_rotating_angle = 0;
	proximal_bending_circle = 0;

	distal_rotating_angle = 0;
	distal_bending_circle = 0;

	proximal_bending_circle_vangle = 0;

	distal_bending_circle_vangle = 0;



	proximal_rotating_en = 0;
	proximal_bending_en = 0;
	distal_rotating_en = 0;
	distal_bending_en = 0;

	M1.rpm_max_Target = Mx_rpm_max;
	M2.rpm_max_Target = Mx_rpm_max;
	//M3.rpm_max_Target = Mx_rpm_max;
	//M4.rpm_max_Target = Mx_rpm_max;

	M1.mode = 3;
	M2.mode = 3;
	//M3.mode = 3;
	//M4.mode = 3;

	M1.error = 0;
	M2.error = 0;
	//M3.error = 0;
	//M4.error = 0;


	PID_param_init(&M1.pid_speed,0.1,2.5,0.1);                              //速度环PID初始�???????????????
	PID_param_init(&M2.pid_speed,0.1,2.5,0.1);
	//PID_param_init(&M3.pid_speed,0.1,2.5,0.1);
	//PID_param_init(&M4.pid_speed,0.1,2.5,0.1);

	PID_param_init(&M1.pid_position,0.2,0.015,5);
	PID_param_init(&M2.pid_position,0.2,0.015,5);
	//PID_param_init(&M3.pid_position,0.2,0.015,5);
	//PID_param_init(&M4.pid_position,0.2,0.015,5);


}
void motor_run(void)
{
    const uint16_t JOY_DEADZONE = 80;          // ← 新增死区（可调 50~120）

    if (joy_r < JOY_DEADZONE)
    {
        joy_en = 0;                            // 强制视为松手
    }

    if (!(M1.error || M2.error))
    {
        if (joy_en)
        {
            M1.mode = 3;
            M2.mode = 3;

            distal_rotating(joy_a);
            distal_bending(joy_a, joy_r, joy_rMax);
            distal_status = 1;
        }
        else
        {
            if (distal_status)
            {
                distal_status = 0;
                M1.mode = 2;
                M2.mode = 2;

                distal_rotating_angle = -distal_rotating_angle_now;
                distal_bending_circle = distal_bending_circle_now;
            }
        }
    }
}
void motor_run_speed_50ms(xMotors *Mx)
{
	uint16_t PWM;
	Mx->pid_speed.target_val = Mx->rpm_Target;
	addPID_realize(&(Mx->pid_speed),Mx->rpm);

	Mx->PWM =Mx->pid_speed.output_val;

	if(Mx->PWM>PWM_val_max)Mx->PWM = PWM_val_max;
	else if(Mx->PWM<-PWM_val_max)Mx->PWM = -PWM_val_max;


	if(Mx->rpm_Target == 0)
	{
		Mx->PWM = 0;
		__HAL_TIM_SetCompare(&M1234_tim,Mx->PWM_CHANNEL,PWM_val_stop);
	}
	else
	{

		if(Mx->PWM>0)HAL_GPIO_WritePin(Mx->GPIOx, Mx->GPIO_Pin, GPIO_PIN_RESET);
		else HAL_GPIO_WritePin(Mx->GPIOx, Mx->GPIO_Pin, GPIO_PIN_SET);

		PWM = abs((int16_t)Mx->PWM);
		__HAL_TIM_SetCompare(&M1234_tim,Mx->PWM_CHANNEL,PWM);

	}
}

void motor_position(xMotors *Mx,int16_t speed)
{

	Mx->enc_Difference = Mx->enc_now_total - Mx->enc_Target;

	Mx->pid_position.target_val = 0;
	PosionPID_realize(&(Mx->pid_position),Mx->enc_Difference);

	Mx->rpm_Target = Mx->pid_position.output_val;


	if(Mx->rpm_Target>speed)Mx->rpm_Target=speed;
	else if(Mx->rpm_Target<-speed)Mx->rpm_Target=-speed;

	if(labs(Mx->enc_Difference)>150)Mx->busy=200;
	else if(Mx->busy)Mx->busy--;

}


void motor_run_position(void)
{
    // ==================== 1. 协调运动（A→B 同时旋转+弯曲） ====================
    static float target_rotating_angle = 0;
    static float target_bending_circle = 0;
    static uint8_t move_to_target_en = 0;          // 外部可通过 start_move_to_target() 置 1

    if (move_to_target_en)
    {
        float rot_step  = 0.6f;     // 每1ms 旋转步长（可调）
        float bend_step = 0.006f;   // 每1ms 弯曲步长（可调）

        if (fabs(distal_rotating_angle - target_rotating_angle) > rot_step)
            distal_rotating_angle += (target_rotating_angle > distal_rotating_angle ? rot_step : -rot_step);
        else
            distal_rotating_angle = target_rotating_angle;

        if (fabs(distal_bending_circle - target_bending_circle) > bend_step)
            distal_bending_circle += (target_bending_circle > distal_bending_circle ? bend_step : -bend_step);
        else
            distal_bending_circle = target_bending_circle;

        if (fabs(distal_rotating_angle - target_rotating_angle) < 0.5f &&
            fabs(distal_bending_circle - target_bending_circle) < 0.005f)
        {
            move_to_target_en = 0;
            M1.mode = 2;
            M2.mode = 2;
        }
        else
        {
            M1.mode = 3;
            M2.mode = 3;
        }
    }

    // ==================== 2. 原有逻辑 + 135° 安全限位 ====================
    if (M1.mode >= 3)
        M1.rpm_max_Target = 0.1 + (0.5 + 0.4) * (1.0 - 0.7 * JOY_S / AD_max) * M1.speed * Mx_rpm_max;

    if (M1.mode >= 2)
    {
        float rotation_contrib = distal_rotating_angle * ROTATION_PER_DEG;
        float bending_contrib  = distal_bending_circle * BENDING_SCALE;

        // ★ 135° 安全边界（只限制弯曲额外行程）
        float max_bending_add = MAX_BENDING_EXTRA_TURNS * BENDING_SCALE;
        if (bending_contrib > max_bending_add) bending_contrib = max_bending_add;
        if (bending_contrib < 0)               bending_contrib = 0;

        M1.enc_Target = (int64_t)(rotation_contrib + bending_contrib);
    }

    if (M1.mode >= 1)
        motor_position(&M1, M1.rpm_max_Target);

    // M2 部分保持原有逻辑...
    if (M2.mode >= 2)
        M2.enc_Target = distal_rotating_angle * ROTATION_PER_DEG;

    if (M2.mode >= 1)
        motor_position(&M2, M2.rpm_max_Target);
}
void current_errror(void)
{

	if(M1.current>=110)M1.error = 1;
	if(M2.current>=110)M2.error = 1;
	if(M3.current>=110)M3.error = 1;
	if(M4.current>=110)M4.error = 1;

	if( M1.error == 1|| M2.error == 1|| M3.error == 1||M4.error== 1)
	{
		M1.rpm_Target = 0;
		M2.rpm_Target = 0;
		M1.mode = 0;
		M2.mode = 0;

		M3.rpm_Target = 0;
		M4.rpm_Target = 0;
		M3.mode = 0;
		M4.mode = 0;
	}
}

void enc_errror(void)
{
	if(M1.PWM>=1000 && M1.rpm <= 5 )M1.error = 2;
	if(M2.PWM>=1000 && M2.rpm <= 5 )M2.error = 2;
	if(M3.PWM>=1000 && M3.rpm <= 5 )M3.error = 2;
	if(M4.PWM>=1000 && M4.rpm <= 5 )M4.error = 2;

	if( M1.error == 2|| M2.error == 2|| M3.error == 2||M4.error== 2)
	{
		M1.rpm_Target = 0;
		M2.rpm_Target = 0;
		M1.mode = 0;
		M2.mode = 0;

		M3.rpm_Target = 0;
		M4.rpm_Target = 0;
		M3.mode = 0;
		M4.mode = 0;
	}
}


void proximal_rotating(float angle)
{
	float rotating_angle_buf;
	float  proximal_rotating_angle_vangle;
	float proximal_rotating_angle_target;

	proximal_rotating_angle_target = fmod(proximal_rotating_angle,360);
	if(proximal_rotating_angle_target<0)proximal_rotating_angle_target = 360+proximal_rotating_angle_target;


	proximal_rotating_angle_vangle = 360-angle- proximal_rotating_angle_target;

	if(proximal_rotating_angle_vangle < -180)proximal_rotating_angle_vangle =  proximal_rotating_angle_vangle + 360;
	else if(proximal_rotating_angle_vangle > 180)proximal_rotating_angle_vangle = proximal_rotating_angle_vangle - 360;


	if(fabs(proximal_rotating_angle_vangle) <= sectors  || proximal_bending_angle_now >= -1 )
	{

		rotating_angle_buf = proximal_rotating_angle + proximal_rotating_angle_vangle;
		if(rotating_angle_buf>=-rotating_cir_limit*A2PI && rotating_angle_buf<=rotating_cir_limit*A2PI)
		{
			proximal_rotating_angle = rotating_angle_buf;
			proximal_rotating_en = 1;
		}
	}

}


void distal_rotating(float angle)
{

	float rotating_angle_buf;
	float  distal_rotating_angle_vangle;
	float distal_rotating_angle_target;

	distal_rotating_angle_target = fmod(distal_rotating_angle,360);
	if(distal_rotating_angle_target<0)distal_rotating_angle_target = 360+distal_rotating_angle_target;


	distal_rotating_angle_vangle = 360-angle - distal_rotating_angle_target;

	if(distal_rotating_angle_vangle < -180)distal_rotating_angle_vangle =  distal_rotating_angle_vangle + 360;
	else if(distal_rotating_angle_vangle > 180)distal_rotating_angle_vangle = distal_rotating_angle_vangle - 360;


	//if(fabs(distal_rotating_angle_vangle)<=sectors || distal_bending_angle_now >= -1)
	{
		rotating_angle_buf = distal_rotating_angle + distal_rotating_angle_vangle;

		if(rotating_angle_buf>=-rotating_cir_limit*A2PI && rotating_angle_buf<=rotating_cir_limit*A2PI)
		{
			distal_rotating_angle =  rotating_angle_buf;
			distal_rotating_en = 1;
		}
	}
}
void proximal_bending(float angle,float R,uint16_t Rmax)
{

	angle += (int16_t)(-proximal_rotating_angle/360+(proximal_rotating_angle>=0?-1:0))*360;

	if((angle>=(-proximal_rotating_angle-sectors)) && (angle<=(-proximal_rotating_angle+sectors)))
	{
		proximal_bending_circle = proximal_bending_circle_n;
	}
	else proximal_bending_circle = 0;

	if(fabs(proximal_bending_circle-proximal_bending_circle_now)>0.01)M4.speed =R/Rmax;
	else M4.speed =0;
}

void distal_bending(float angle, float R, uint16_t Rmax)
{
    if ((angle <= sectors) || (angle >= 360 - sectors))
    {
        // 核心修改：比例 + 135°上限
        distal_bending_circle = (R / Rmax) * MAX_BENDING_EXTRA_TURNS;

        // 如果增大弯曲时 M1 需要反转，请在这里加负号：
        // distal_bending_circle = -(R / Rmax) * MAX_BENDING_EXTRA_TURNS;
    }
    else
    {
        distal_bending_circle = 0;
    }

    M1.speed = R / Rmax;
}


void motor_homeing(void)
{
	static uint16_t AD_min[2];
	static int64_t enc_buf[5];

	if(motor_homeing_n == 1)
	{
		motor_homeing_n =  2;
		distal_homeing_n = 1;
		proximal_homeing_n = 0;
	}
	else if(distal_homeing_n == 0&&proximal_homeing_n == 0)
	{
		motor_homeing_n =0;
	}


	switch(proximal_homeing_n)
	{
		case 1:
			M4_enc=0;
			xM_init(&M4);
			__HAL_TIM_SET_COUNTER(&htim8,enc_default);
			proximal_homeing_n++;
			break;
		case 2:
			//position_mode = 10;
			M4.mode = 0;
			M4.rpm_Target = -Mx_rpm_mid;
			M4.state = 3;
			proximal_homeing_n++;
			break;
		case 3:
			 if(M4.state == 1)M4.rpm_Target = 0;
			 if(M4.state == 1&&M1.state == 1)proximal_homeing_n++;
			break;
		case 4:
			 M4.enc_Target = M4.enc_now_total;
			 M4.enc_Target += proximal_b_init_compensation;
			 M4.busy = 500;

			 //position_mode = 1;


			 M4.rpm_max_Target = Mx_rpm_max;
			 M4.mode = 1;
			 proximal_homeing_n++;
			break;
		case 5:
			 if(M4.busy == 0&&M1.busy == 0)proximal_homeing_n++;
			break;
		case 6:
			//position_mode = 0;
			 M4.mode = 3;
			M4_enc=0;
			xM_init(&M4);
			__HAL_TIM_SET_COUNTER(&htim8,enc_default);
			proximal_homeing_n++;
			break;
			//------------------------------------------------
		case 7:

			AD_min[0] = 0xFFFF;
			M3.rpm_max_Target = Mx_homeing_speed;
			M4.rpm_max_Target = Mx_homeing_speed;
			proximal_rotating_angle +=360+90;
			proximal_rotating_en = 1;
//			position_mode = 0;
			M3.mode = 2;
			M4.mode = 2;
			proximal_homeing_n ++;

			break;
		case 8:
			if(AD_min[0]>proximal_rotating_AD)
			{
				enc_buf[0] = M3.enc_now_total+proximal_p_init_compensation*proximal_rotating_b;
				enc_buf[1] = M4.enc_now_total+proximal_p_init_compensation*proximal_rotating_b;
				AD_min[0]=proximal_rotating_AD;

			}
			if(proximal_rotating_en == 0&&distal_rotating_en == 0)proximal_homeing_n++;
			break;
		case 9:
//			position_mode = 1;
			M3.mode = 1;
			M4.mode = 1;

			M3.enc_Target = enc_buf[0];
			M4.enc_Target = enc_buf[1];
			proximal_homeing_n++;
			break;
		case 10:
			 if(M3.busy == 0&&M4.busy == 0 && M1.busy == 0&&M2.busy == 0)proximal_homeing_n++;
			break;
		case 11:
			M3.mode = 2;
			M4.mode = 2;
			proximal_rotating_angle = 0;
			M3.rpm_max_Target = Mx_rpm_mid;
			M4.rpm_max_Target = Mx_rpm_mid;
			M3_enc=0;
			M4_enc=0;
			xM_init(&M3);
			__HAL_TIM_SET_COUNTER(&htim3,enc_default);
			xM_init(&M4);
			__HAL_TIM_SET_COUNTER(&htim8,enc_default);
			proximal_homeing_n++;
			break;
		case 12:
			proximal_bending_circle = 1;
			proximal_homeing_n++;
			break;
		case 13:
			 if(M1.busy == 0&&M2.busy == 0 && M3.busy == 0&&M4.busy == 0 )proximal_homeing_n++;
			break;
		case 14:
			proximal_bending_circle = 0;
			proximal_homeing_n++;
			break;
		case 15:
			 if(M1.busy == 0&&M2.busy == 0 && M3.busy == 0&&M4.busy == 0 )proximal_homeing_n++;
			break;
		case 16:

			M3.mode = 3;
			M4.mode = 3;
			proximal_homeing_n = 0;
			break;
	}
	switch(distal_homeing_n)
	{
		case 1:
			M1_enc=0;
			xM_init(&M1);
			__HAL_TIM_SET_COUNTER(&htim1,enc_default);
			distal_homeing_n++;
			break;
		case 2:
		//	position_mode = 10;
			M1.mode = 0;
			M1.rpm_Target = Mx_rpm_mid;
			M1.state = 3;
			distal_homeing_n++;
			break;
		case 3:
			 if(M1.state == 1)M1.rpm_Target = 0;
			 if(M1.state == 1/*&&M4.state == 1*/)distal_homeing_n++;
			break;
		case 4:
			 M1.enc_Target = M1.enc_now_total;
			 M1.enc_Target -= distal_b_init_compensation;
			 M1.busy = 500;
		//	 position_mode = 1;

			 M1.rpm_max_Target = Mx_rpm_max;
			 M1.mode = 1;
			 distal_homeing_n++;
			break;
		case 5:
			 if(M1.busy == 0/*&&M4.busy == 0*/)distal_homeing_n++;
			break;
		case 6:
		//	position_mode = 0;
			M1.mode = 3;
			M1_enc=0;
			xM_init(&M1);
			__HAL_TIM_SET_COUNTER(&htim1,enc_default);
			distal_homeing_n = 15;
			break;
			//------------------------------------------------
		case 7:

			AD_min[1] = 0xFFFF;
			M1.rpm_max_Target = Mx_homeing_speed;
			M2.rpm_max_Target = Mx_homeing_speed;
			distal_rotating_angle +=180;
			distal_rotating_en = 1;
//			position_mode = 0;
			M1.mode = 2;
			M2.mode = 2;
			distal_homeing_n ++;


			break;
		case 8:
			if(AD_min[1]>diatal_rotating_AD)
			{
				enc_buf[2] = M2.enc_now_total+distal_p_init_compensation*distal_rotating_b;
				enc_buf[3] = M1.enc_now_total+distal_p_init_compensation*distal_rotating_b;
				AD_min[1]=diatal_rotating_AD;

			}
			if(distal_rotating_en == 0 /*&& proximal_rotating_en == 0*/)distal_homeing_n ++;
			break;
		case 9:

			distal_rotating_angle -=360;
			distal_rotating_en = 1;
			M1.mode = 2;
			M2.mode = 2;
			distal_homeing_n ++;

			break;
		case 10:
			if(AD_min[1]>diatal_rotating_AD)
			{
				enc_buf[2] = M2.enc_now_total+distal_p_init_compensation*distal_rotating_b;
				enc_buf[3] = M1.enc_now_total+distal_p_init_compensation*distal_rotating_b;
				AD_min[1]=diatal_rotating_AD;

			}
			if(distal_rotating_en == 0 /*&& proximal_rotating_en == 0*/)distal_homeing_n ++;
			break;
		case 11:
		//	position_mode = 1;
			M1.mode = 1;
			M2.mode = 1;

			M2.enc_Target = enc_buf[2];
			M1.enc_Target = enc_buf[3];
			distal_homeing_n++;
			break;
		case 12:
			 if(M1.busy == 0&&M2.busy == 0/* && M3.busy == 0&&M4.busy == 0 */)distal_homeing_n++;
			break;
		case 13:

			M1.mode = 2;
			M2.mode = 2;
			distal_rotating_angle = 0;
			M1.rpm_max_Target = Mx_rpm_mid;
			M2.rpm_max_Target = Mx_rpm_mid;
			M1_enc=0;
			M2_enc=0;
			xM_init(&M1);
			__HAL_TIM_SET_COUNTER(&htim1,enc_default);
			xM_init(&M2);
			__HAL_TIM_SET_COUNTER(&htim2,enc_default);
			distal_homeing_n++;
			break;
		case 14:
			distal_bending_circle = -1;
			distal_homeing_n++;
			break;
		case 15:
			 if(M1.busy == 0&&M2.busy == 0 /*&& M3.busy == 0&&M4.busy == 0 */)distal_homeing_n++;
			break;
		case 16:
			distal_bending_circle = 0;
			distal_homeing_n++;
			break;
		case 17:
			 if(M1.busy == 0&&M2.busy == 0 /*&& M3.busy == 0&&M4.busy == 0 */)distal_homeing_n++;
			break;
		case 18:

			M1.mode = 3;
			M2.mode = 3;
			distal_homeing_n = 0;
			break;
	}




}


void UART_DRV_DMA_SendCmd(uint8_t board_id,int16_t cmd,int16_t val)
{

	uint8_t i = 0;
	uint8_t checksum = 0;
	uint16_t send_num = 0;

	xUSART2.TransmitTemp[0] = FRAME_HEAD1_DRV;
	xUSART2.TransmitTemp[1] = FRAME_HEAD2_DRV;
	xUSART2.TransmitTemp[3] = board_id;
	switch(cmd)
	{
		case CMD_SEEKPOS:
				{
					send_num = 9;
					xUSART2.TransmitTemp[2] = send_num-5;//len
					xUSART2.TransmitTemp[4] = CMD_WR_NR;//CMD_WR_NR; 随动模式无反馈
					xUSART2.TransmitTemp[5] = TAG_POSITION_2B;
					xUSART2.TransmitTemp[6] = (val & 0xFF);
					xUSART2.TransmitTemp[7] = ((val>>8) & 0xFF);
					break;
				}
		case CMD_SEEKPOS_SERVO:
				{
					send_num = 9;
					xUSART2.TransmitTemp[2] = send_num-5;//len
					xUSART2.TransmitTemp[4] = CMD_WR_SERVO_NR;
					xUSART2.TransmitTemp[5] = TAG_POSITION_2B;
					xUSART2.TransmitTemp[6] = (val & 0xFF);
					xUSART2.TransmitTemp[7] = ((val>>8) & 0xFF);
					break;
				}
		case CMD_READ_CURPOS:
				{
					send_num = 8;
					xUSART2.TransmitTemp[2] = send_num-5;//len
					xUSART2.TransmitTemp[4] = CMD_MC;
					xUSART2.TransmitTemp[5] = 0;  //
					xUSART2.TransmitTemp[6] = CMD_MOTOR_BIT;
					break;
				}
		case CMD_POSHOLD:
				{
					send_num = 8;
					xUSART2.TransmitTemp[2] = send_num-5;//len
					xUSART2.TransmitTemp[4] = CMD_MC;
					xUSART2.TransmitTemp[5] = 0;  //
					xUSART2.TransmitTemp[6] = CMD_MOTOR_POS_HOLD;
					break;
				}
		case CMD_MOTORRUN:
				{
					send_num = 8;
					xUSART2.TransmitTemp[2] = send_num-5;//len
					xUSART2.TransmitTemp[4] = CMD_MC;
					xUSART2.TransmitTemp[5] = 0;  //
					xUSART2.TransmitTemp[6] = CMD_MOTOR_RUN;
					break;
				}
		default:
				{
					return ;
				}
	}
	for(i=2;i<(send_num-1);i++)
	{
		checksum += xUSART2.TransmitTemp[i];
	}
	xUSART2.TransmitTemp[send_num-1] = checksum & 0xFF;

//	while(huart2.gState != HAL_UART_STATE_READY);
//	SCB_CleanDCache_by_Addr((uint32_t *)xUSART2.TransmitTemp, send_num);

	HAL_UART_Transmit_DMA(&huart2,(uint8_t*)xUSART2.TransmitTemp,send_num);

}

