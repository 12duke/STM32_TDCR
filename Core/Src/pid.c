
#include "pid.h"

 /**
   * @brief  位置PID算法实现
   * @param  actual_val:实际测量值
   *	@note 	无
   * @retval 通过PID计算后的输出
   */
 float PosionPID_realize(PID *pid, float actual_val)
 {
 	/*计算目标值与实际值的误差*/
 	pid->Error = pid->target_val - actual_val;
 	/*积分项*/
 	pid->integral += pid->Error;

 	if(pid->integral>pid->integral_max)pid->integral=pid->integral_max;
 	else if(pid->integral<-pid->integral_max)pid->integral=-pid->integral_max;
 	/*PID算法实现*/
 	pid->output_val = pid->Kp * pid->Error +
 	                  pid->Ki * pid->integral +
 	                  pid->Kd *(pid->Error -pid->LastError);
 	/*误差传递*/
 	pid-> LastError = pid->Error;
 	/*返回当前实际值*/
 	return pid->output_val;
 }

 /**
   * @brief  速度PID算法实现
   * @param  actual_val:实际值
   *	@note 	无
   * @retval 通过PID计算后的输出
   */
 float addPID_realize(PID *pid, float actual_val)
 {
 	/*计算目标值与实际值的误差*/
 	pid->Error = pid->target_val - actual_val;
 	/*PID算法实现，照搬公式*/
 	pid->output_val += pid->Kp * (pid->Error - pid-> LastError) +
 	                  pid->Ki * pid->Error +
 	                  pid->Kd *(pid->Error -2*pid->LastError+pid->PrevError);
 	/*误差传递*/
 	pid-> PrevError = pid->LastError;
 	pid-> LastError = pid->Error;
 	/*返回当前实际值*/

 	if(pid->output_val>pid->output_max)pid->output_val=pid->output_max;
 	else if(pid->output_val<-pid->output_max)pid->output_val=-pid->output_max;

 	return pid->output_val;
 }



 void PID_param_init(PID *pid,float p,float i,float d)
 {
 	pid->target_val=0.0;
 	pid->output_max=1000.0;
 	pid->output_val=0.0;
 	pid->Error=0.0;
 	pid->LastError=0.0;
 	pid->integral=0.0;
 	pid->integral_max=100;

 	pid->Kp = p;
 	pid->Ki = i;
 	pid->Kd = d;

 }
 void PID_param_init2(PID *pid)
 {
 	pid->target_val=0.0;
 	pid->output_val=0.0;
 	pid->Error=0.0;
 	pid->LastError=0.0;
 	pid->integral=0.0;
 	pid->integral_max=100;

 	pid->Kp = 0.0;
 	pid->Ki = 0.1;
 	pid->Kd = 0.0;

 }



