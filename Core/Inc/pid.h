
#ifndef INC_PID_H_
#define INC_PID_H_
#include "main.h"

typedef struct
{
	float target_val;   //Ŀ��ֵ
	float Error;          /*�� k ��ƫ�� */
	float LastError;     /* Error[-1],�� k-1 ��ƫ�� */
	float PrevError;    /* Error[-2],�� k-2 ��ƫ�� */
	float Kp,Ki,Kd;     //���������֡�΢��ϵ��
	float integral;     //����
	float integral_max;     //����ֵ
	float output_val;   //���ֵ
	float output_max;   //���ֵ
}PID;



extern PID M1_pid;
extern PID M2_pid;
extern PID M3_pid;
extern PID M4_pid;


void PID_param_init(PID *pid,float p,float i,float d);
void PID_param_init2(PID *pid);

float PosionPID_realize(PID *pid, float actual_val);

float addPID_realize(PID *pid, float actual_val);

#endif
