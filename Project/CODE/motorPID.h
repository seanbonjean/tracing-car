#ifndef __MOTOR_PID_H_
#define __MOTOR_PID_H_
#endif

//������1
#define ENC1_DIR P35 //��������
#define ENC1_LSB CTIM0_P34 //�����������

//������2
#define ENC2_DIR P77 //��������
#define ENC2_LSB CTIM3_P04 //�����������

//���1
#define MOT1_DIR P63 //��������
#define MOT1_PWM PWMA_CH1P_P60 //PWM����

//���2
#define MOT2_DIR P44 //��������
#define MOT2_PWM PWMA_CH2P_P62 //PWM����

enum select_motor
{
	MOTOR1,
	MOTOR2
};

void motorPID_init(void);
void motorPID(void);
int16 input_speed(enum select_motor);
void output_speed(enum select_motor);
int16 limit(int16, int16);