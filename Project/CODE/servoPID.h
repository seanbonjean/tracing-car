#ifndef __SERVO_PID_H_
#define __SERVO_PID_H_
#endif

#define SERVO_CENTER 1.5/20*PWM_DUTY_MAX //舵机居中占空比
#define SERVO_PIN PWMB_CH1_P74 //舵机引脚

void servoPID_init(void);
void servoPID(int16);