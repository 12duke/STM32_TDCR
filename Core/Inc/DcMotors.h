/*
 * DcMotors.h
 *
 *  Created on: Jun 3, 2024
 *      Author: Bunny
 */


#ifndef INC_DCMOTORS_H_
#define INC_DCMOTORS_H_

#include "pid.h"

#define DRV_SEND_QUEUE_LENGTH 256

//below is DRV COM Used----------------------------------------------------
#define CMD_RD 						0x01//查询控制表内的数据
#define CMD_WR 						0x02//向控制表内写入数据

#define CMD_WR_NR 				0x03//定位模式（无反馈）
#define CMD_WR_R 					0x21//定位模式（反馈状态信息）
#define CMD_MC 						0x04//实现对电缸的功能控制
#define CMD_WR_SERVO_NR 	0x19//随动模式（无反馈）
#define CMD_WR_SERVO_R		0x20//随动模式（反馈状态信息）
#define TAG_POSITION_2B		0X37//0X37,为写入控制表的数据段的首地址（此时为目标位置）


//3.5.4.单控指令：控制单元向电缸发送控制命令-P15
#define CMD_MOTOR_BIT      				0x22	//0x22,查询电缸状态信息（BIT），包括目标位置、当前位置、温度、电机驱动电流以及异常信息
#define CMD_MOTOR_RUN							0x04	//0x04,工作，即使能电机功率驱动输出
#define CMD_MOTOR_POS_HOLD				0x23	//0x23,急停，即禁止电机功率驱动输出（需要运动时先发送工作启动指令、再发送位置指令才能运动）
#define CMD_MOTOR_POS_PAUSE				0x14	//0x14,暂停，即禁止电机功率驱动输出（需要运动时直接发送位置指令即可运动）
#define CMD_MOTOR_WRITE_FLASH			0x20	//0x20,参数装订，即将当前运行参数装订烧写到内部 Flash
#define CMD_MOTOR_DELETE_FAUSEH		0x1E	//0x1E,故障清除，当电缸发生过流、堵转、电机异常故障时可通过该指令清除故障码，恢复电缸的正常工作

#define CMD_MOTOR_SPEED0_STOP		1

#define CMD_SEEKPOS_SERVO 		    5
#define CMD_SEEKPOS 				1
#define CMD_READ_CURPOS 			2
#define CMD_POSHOLD			 		3
#define CMD_MOTORRUN				4

/* USER CODE BEGIN Private defines */
#define FRAME_HEAD1_DRV  			0x55
#define FRAME_HEAD2_DRV  			0xAA
//-----------------------------------------------------------------------



#define Min_Inspire_Motor 550 //450  // 因时电机最低位置
#define Max_Inspire_Motor 1650 // 因时电机最高位置
#define interval_Inspire_Motor (Max_Inspire_Motor-Min_Inspire_Motor)  //因时电机行程



extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim4;
extern TIM_HandleTypeDef htim6;
extern TIM_HandleTypeDef htim7;
extern TIM_HandleTypeDef htim8;
extern TIM_HandleTypeDef htim15;





#define M1234_tim htim4    //直流电机PWM定时器


#define Mx_rpm_max 250      //测得最快260
#define Mx_rpm_mid 150
#define Mx_homeing_speed 50



#define enc_default 0x7FFF
#define overflow_enc 0x8000
//#define overflow_enc 0x10000   //电机编码overflow系数




#define M1_enc __HAL_TIM_GET_COUNTER(&htim1)
#define M2_enc __HAL_TIM_GET_COUNTER(&htim2)
#define M3_enc __HAL_TIM_GET_COUNTER(&htim3)
#define M4_enc __HAL_TIM_GET_COUNTER(&htim8)


#define sectors 60

#define rotating_cir_limit 0.25


#define time6_minute 20*60   //rpm
#define Motors_encoder 4     //
#define Motors_wire 12
#define Reduction_ratio_256 256
#define Reduction_ratio_64 64
#define Gear_A 20
#define Gear_B 17
#define screw 0.5



#define PI 3.14159265
#define API 180
#define A2PI 360


////hongkong 2

#define coefficientd (2.4+2.2)
#define coefficientp (2.8+2.6)

#define proximal_b_init_compensation 11000
#define proximal_p_init_compensation 45

#define distal_b_init_compensation 5000
// #define distal_p_init_compensation 15    // 1
#define distal_p_init_compensation -120       //2



#define proximal_b_a 90

#define distal_b_a 130

#define proximal_bending_circle_n 5.5
#define distal_bending_circle_n -6.0

////SZ
//
//#define coefficientd (0.65+0.55)
//#define coefficientp (0.95+0.85)
//
//#define proximal_b_init_compensation 6000
//#define proximal_p_init_compensation (-67+100)
//#define distal_b_init_compensation 7500
//#define distal_p_init_compensation (-155-75)
//
////#define proximal_b_a 38
////
////#define distal_b_a 55
//
//#define proximal_bending_circle_n 7.5
//
//#define distal_bending_circle_n 7.5


////形状感知 1
//
//#define coefficientd (2.4+2.2)
//#define coefficientp (2.8+2.6)
//
//#define proximal_b_init_compensation 100
//#define proximal_p_init_compensation -40
//#define distal_b_init_compensation 3000
//#define distal_p_init_compensation 240
//
//#define proximal_b_a 38
//
//#define distal_b_a 55
//
//#define proximal_bending_circle_n 7.0
//#define distal_bending_circle_n 8.9

//hongkong 1

//#define coefficientd (2.4+2.2)
//#define coefficientp (2.8+2.6)
//#define proximal_b_init_compensation 6000
//#define proximal_p_init_compensation -40
//#define distal_b_init_compensation 4000
//#define distal_p_init_compensation 240
//
//#define proximal_b_a 38
//
//#define distal_b_a 55
//
//#define proximal_bending_circle_n 6.5
//#define distal_bending_circle_n .0








#define proximal_maxscope 360*5
#define distal_maxscope 360*5

#define M1_rpm ( (float)M1.enc_now_speed*time6_minute*Gear_B/Motors_wire/Reduction_ratio_64/Gear_A/Motors_encoder)
#define M2_rpm ( (float)M2.enc_now_speed*time6_minute*Gear_B/Motors_wire/Reduction_ratio_64/Gear_A/Motors_encoder)
#define M3_rpm ( (float)M3.enc_now_speed*time6_minute*Gear_B/Motors_wire/Reduction_ratio_64/Gear_A/Motors_encoder)
#define M4_rpm ( (float)M4.enc_now_speed*time6_minute*Gear_B/Motors_wire/Reduction_ratio_64/Gear_A/Motors_encoder)





#define rotating_circle_64   ((float)Motors_wire*Reduction_ratio_64*Motors_encoder*Gear_A/Gear_B)
#define rotating_circle_256  ((float)Motors_wire*Reduction_ratio_256*Motors_encoder*Gear_A/Gear_B)


#define distal_rotating_a    ((float)Motors_wire*Reduction_ratio_256*Motors_encoder*Gear_A/Gear_B/360)
#define distal_rotating_b    ((float)Motors_wire*Reduction_ratio_64*Motors_encoder*Gear_A/Gear_B/360)


#define proximal_rotating_a  ((float)Motors_wire*Reduction_ratio_256*Motors_encoder*Gear_A/Gear_B/360)
#define proximal_rotating_b  ((float)Motors_wire*Reduction_ratio_64*Motors_encoder*Gear_A/Gear_B/360)



#define PWM_val_stop 0   //直流电机PWM 0占空比
#define PWM_val_max 1000   //直流电机PWM 0占空比

typedef struct
{
	volatile int32_t overflow;       //编码器超出次数
	volatile int64_t enc_now_total;  // 当前编码
	volatile int64_t enc_now_total_last; //上次编码
	volatile int64_t enc_Target;     //目标编码
	int32_t enc_Difference; //目标编码差值
	int32_t enc_now_speed;  //编码速度
	uint16_t busy;          //忙状态
    PID pid_speed;          //速度环
    float speed;


    PID pid_position;       //位置环


    uint8_t mode;           //电机运行模式
    uint8_t error;          //过流

	float current;          //电流
	float rpm;              //当前速度
	float rpm_Target;       //目标速度
	float rpm_max_Target;       //目标最高速度
	float PWM;              //当前PWM值
	uint8_t state;          //电机状态


	uint32_t PWM_CHANNEL;
	GPIO_TypeDef *GPIOx;
	uint16_t GPIO_Pin;

} xMotors;

#define  distal_bending_angle_now (double)Gear_B*API*(M1.enc_now_total - M2.enc_now_total)*screw/Motors_wire/Reduction_ratio_64/Motors_encoder/Gear_A/coefficientd/PI
#define  distal_bending_circle_now (double)Gear_B*(M1.enc_now_total - M2.enc_now_total)/Motors_wire/Reduction_ratio_64/Motors_encoder/Gear_A
#define  proximal_bending_angle_now (double)Gear_B*API*(M4.enc_now_total - M3.enc_now_total)*screw/Motors_wire/Reduction_ratio_64/Motors_encoder/Gear_A/coefficientp/PI
#define  proximal_bending_circle_now (double)Gear_B*(M4.enc_now_total - M3.enc_now_total)/Motors_wire/Reduction_ratio_64/Motors_encoder/Gear_A

#define  distal_rotating_angle_now   (double)Gear_B*A2PI*(-M2.enc_now_total)/Motors_wire/Reduction_ratio_64/Motors_encoder/Gear_A
#define  proximal_rotating_angle_now (double)Gear_B*A2PI*(-M3.enc_now_total)/Motors_wire/Reduction_ratio_64/Motors_encoder/Gear_A

extern xMotors M1;
extern xMotors M2;
extern xMotors M3;
extern xMotors M4;

extern uint32_t whole_dis;
extern uint32_t Inspire_buf;
extern uint8_t Inspire_flag;



extern uint16_t motor_homeing_n;
extern uint16_t distal_homeing_n;
extern uint16_t proximal_homeing_n;
extern uint8_t button_home_flag;
extern uint16_t button_home_time;

extern float proximal_rotating_angle;
extern float proximal_bending_circle;
extern uint8_t proximal_status;

extern float distal_rotating_angle;
extern float distal_bending_circle;
extern uint8_t distal_status;


extern float proximal_bending_circle_vangle;

extern float distal_bending_circle_vangle;

extern uint8_t proximal_rotating_en;
extern uint8_t proximal_bending_en;
extern uint8_t distal_rotating_en;
extern uint8_t distal_bending_en;

void xM_init(xMotors *xM);
void motor_init(void);
void motor_run_speed_50ms(xMotors *Mx);
void motor_position(xMotors *Mx,int16_t speed);
void motor_run_position(void);
void motor_homeing(void);
void current_errror(void);
void enc_errror(void);
void proximal_rotating(float angle);
void distal_rotating(float angle);
void proximal_bending(float angle,float R,uint16_t Rmax);
void distal_bending(float angle,float R,uint16_t Rmax);
void motor_run(void);
void UART_DRV_DMA_SendCmd(uint8_t board_id,int16_t cmd,int16_t val);

#endif /* INC_DCMOTORS_H_ */
