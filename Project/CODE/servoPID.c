#include "headfile.h"
#include "user.h"

struct servo{
	float Kp, Kd; //位置式PD参数
	int16 u; //输出量
	int16 ek, ek1; //当次误差&历史误差
	int16 real_pos; //实际位置偏差
	int16 set_pos; //期望位置偏差
	int32 duty; //舵机占空比
}ser;

void servoPID_init(void)
{
	pit_timer_ms(TIM_4, 5); //定时器初始化，周期5ms
	
	pwm_init(SERVO_PIN, 50, SERVO_CENTER); //舵机PWM初始化并居中
	
	ser.set_pos = 0; //期望位置偏差为0，即严格按照赛道中线行驶
	
	//设置PD系数
  ser.Kp = SER_KP; //proportional
  ser.Kd = SER_KD; //derivative
	
	ser.ek = 0; //ek赋初值
}

void servoPID(int16 pos)
{
  ser.real_pos = pos; //读取电感传感器返回的转角参考值

  ser.ek1 = ser.ek; //更新历史误差
  ser.ek = ser.set_pos - ser.real_pos; //计算error term

  ser.u = (int16) (ser.Kp * ser.ek + ser.Kd * (ser.ek - ser.ek1)); //计算输出占空比
  ser.u = limit(ser.u, SERVO_LIMIT); //输出限幅

  ser.duty = (int32) ser.u; //输出舵机占空比
  pwm_duty(SERVO_PIN, SERVO_CENTER + ser.duty); //执行
}