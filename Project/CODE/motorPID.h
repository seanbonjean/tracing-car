#ifndef __MOTOR_PID_H_
#define __MOTOR_PID_H_
#endif

//编码器1
#define ENC1_DIR P35 //方向引脚
#define ENC1_LSB CTIM0_P34 //脉冲计数引脚

//编码器2
#define ENC2_DIR P77 //方向引脚
#define ENC2_LSB CTIM3_P04 //脉冲计数引脚

//电机1
#define MOT1_DIR P63 //方向引脚
#define MOT1_PWM PWMA_CH1P_P60 //PWM引脚

//电机2
#define MOT2_DIR P44 //方向引脚
#define MOT2_PWM PWMA_CH2P_P62 //PWM引脚

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