#include "headfile.h"
#include "user.h"

struct motor
{
	float Kp, Ki, Kd; //增量式PID参数
	int16 delta_u; //增量式PID输出增量
	int16 u; //输出量
	int16 real_speed; //实际速度
	int16 set_speed; //期望速度
	int16 ek, ek1, ek2; //前后三次误差
	int32 duty; //电机占空比
}mot1, mot2;

//电机相关部件初始化
void motorPID_init(void)
{
	pit_timer_ms(TIM_1, 5); //定时器初始化，周期5ms
	
	//编码器初始化
	ctimer_count_init(ENC1_LSB);
	ctimer_count_init(ENC2_LSB);
	
	//电机PWM初始化
	//输出PWM频率10000HZ 初始占空比为零 占空比为百分之 pwm_duty / PWM_DUTY_MAX * 100
	pwm_init(MOT1_PWM, 10000, 0);
	pwm_init(MOT2_PWM, 10000, 0);
	
	//设置PID系数
  mot1.Kp = MOT1_KP; //proportional
  mot1.Ki = MOT1_KI; //integral
  mot1.Kd = MOT1_KD; //derivative
	
	mot2.Kp = MOT2_KP; //proportional
  mot2.Ki = MOT2_KI; //integral
  mot2.Kd = MOT2_KD; //derivative
	
	//设置期望速度
	mot1.set_speed = MOT_NORM_SPEED;
	mot2.set_speed = MOT_NORM_SPEED;
	
	//ek赋初值
	mot1.ek = 0;
	mot2.ek = 0;
}

void motorPID(void)
{
	//编码器获取电机速度
	mot1.real_speed = input_speed(MOTOR1);
	mot2.real_speed = input_speed(MOTOR2);
	
	mot1.ek2 = mot1.ek1; //更新历史误差
  mot1.ek1 = mot1.ek;
  mot1.ek = mot1.set_speed - mot1.real_speed; //计算error term
	
	mot2.ek2 = mot2.ek1; //更新历史误差
  mot2.ek1 = mot2.ek;
  mot2.ek = mot2.set_speed - mot2.real_speed; //计算error term
	
	//增量式PID
  mot1.delta_u = (int16) (mot1.Kp * (mot1.ek - mot1.ek1) + mot1.Ki * mot1.ek + mot1.Kd * (mot1.ek - 2 * mot1.ek1 + mot1.ek2)); //计算增量
  mot1.u += mot1.delta_u; //累加入增量
  mot1.u = limit(mot1.u, PWM_DUTY_MAX); //输出限幅，防止输出超过100%占空比
  mot1.duty = (int32) mot1.u; //输出给电机占空比
	
	mot2.delta_u = (int16) (mot2.Kp * (mot2.ek - mot2.ek1) + mot2.Ki * mot2.ek + mot2.Kd * (mot2.ek - 2 * mot2.ek1 + mot2.ek2)); //计算增量
  mot2.u += mot2.delta_u; //累加入增量
  mot2.u = limit(mot2.u, PWM_DUTY_MAX); //输出限幅，防止输出超过100%占空比
  mot2.duty = (int32) mot2.u; //输出给电机占空比
	
	//输出电机速度给驱动板
	output_speed(MOTOR1);
	output_speed(MOTOR2);
}

//编码器获取电机速度
int16 input_speed(enum select_motor mot)
{
	int16 speed = 0;
	
	switch(mot) //电机选择
	{
		case MOTOR1:
			if (1 == ENC1_DIR) //方向引脚
			{
				speed = (int16)ctimer_count_read(ENC1_LSB); //编码器脉冲计数
			}
			else
			{
				speed = -(int16)ctimer_count_read(ENC1_LSB); //反向则取负
			}
			ctimer_count_clean(ENC1_LSB); //计数器清零
		break;
			
		case MOTOR2:
			if (1 == ENC2_DIR) //方向引脚
			{
				speed = (int16)ctimer_count_read(ENC2_LSB); //编码器脉冲计数
			}
			else
			{
				speed = -(int16)ctimer_count_read(ENC2_LSB); //反向则取负
			}
			ctimer_count_clean(ENC2_LSB); //计数器清零
		break;
	}
	
	return speed;
}

//将电机占空比转换为DRV8701E芯片逻辑
void output_speed(enum select_motor mot)
{
	switch(mot) //电机选择
	{
		case MOTOR1:
			if (mot1.duty >= 0) //若电机占空比>=0
			{
				MOT1_DIR = 0; //低电平正转
				pwm_duty(MOT1_PWM, (uint32)mot1.duty); //设置电机占空比
			}
			else
			{
				MOT1_DIR = 1; //高电平反转
				pwm_duty(MOT1_PWM, (uint32)(-mot1.duty)); //设置电机占空比
			}
		break;
			
		case MOTOR2:
			if (mot2.duty >= 0) //若电机占空比>=0
			{
				MOT2_DIR = 0; //低电平正转
				pwm_duty(MOT2_PWM, (uint32)mot2.duty); //设置电机占空比
			}
			else
			{
				MOT2_DIR = 1; //高电平反转
				pwm_duty(MOT2_PWM, (uint32)(-mot2.duty)); //设置电机占空比
			}
		break;
	}
}

//限幅
int16 limit(int16 value, int16 limit_value)
{
	if (value > limit_value)
		return limit_value;
	else if (value < -limit_value)
		return -limit_value;
	else
		return value;
}