#ifndef __SERVO_PID_H_
#define __SERVO_PID_H_
#endif

#define SERVO_CENTER 1.5/20*PWM_DUTY_MAX //�������ռ�ձ�
#define SERVO_PIN PWMB_CH1_P74 //�������

void servoPID_init(void);
void servoPID(int16);