#include "headfile.h"
#include "user.h"

struct motor
{
	float Kp, Ki, Kd; //����ʽPID����
	int16 delta_u; //����ʽPID�������
	int16 u; //�����
	int16 real_speed; //ʵ���ٶ�
	int16 set_speed; //�����ٶ�
	int16 ek, ek1, ek2; //ǰ���������
	int32 duty; //���ռ�ձ�
}mot1, mot2;

//�����ز�����ʼ��
void motorPID_init(void)
{
	pit_timer_ms(TIM_1, 5); //��ʱ����ʼ��������5ms
	
	//��������ʼ��
	ctimer_count_init(ENC1_LSB);
	ctimer_count_init(ENC2_LSB);
	
	//���PWM��ʼ��
	//���PWMƵ��10000HZ ��ʼռ�ձ�Ϊ�� ռ�ձ�Ϊ�ٷ�֮ pwm_duty / PWM_DUTY_MAX * 100
	pwm_init(MOT1_PWM, 10000, 0);
	pwm_init(MOT2_PWM, 10000, 0);
	
	//����PIDϵ��
  mot1.Kp = MOT1_KP; //proportional
  mot1.Ki = MOT1_KI; //integral
  mot1.Kd = MOT1_KD; //derivative
	
	mot2.Kp = MOT2_KP; //proportional
  mot2.Ki = MOT2_KI; //integral
  mot2.Kd = MOT2_KD; //derivative
	
	//���������ٶ�
	mot1.set_speed = MOT_NORM_SPEED;
	mot2.set_speed = MOT_NORM_SPEED;
	
	//ek����ֵ
	mot1.ek = 0;
	mot2.ek = 0;
}

void motorPID(void)
{
	//��������ȡ����ٶ�
	mot1.real_speed = input_speed(MOTOR1);
	mot2.real_speed = input_speed(MOTOR2);
	
	mot1.ek2 = mot1.ek1; //������ʷ���
  mot1.ek1 = mot1.ek;
  mot1.ek = mot1.set_speed - mot1.real_speed; //����error term
	
	mot2.ek2 = mot2.ek1; //������ʷ���
  mot2.ek1 = mot2.ek;
  mot2.ek = mot2.set_speed - mot2.real_speed; //����error term
	
	//����ʽPID
  mot1.delta_u = (int16) (mot1.Kp * (mot1.ek - mot1.ek1) + mot1.Ki * mot1.ek + mot1.Kd * (mot1.ek - 2 * mot1.ek1 + mot1.ek2)); //��������
  mot1.u += mot1.delta_u; //�ۼ�������
  mot1.u = limit(mot1.u, PWM_DUTY_MAX); //����޷�����ֹ�������100%ռ�ձ�
  mot1.duty = (int32) mot1.u; //��������ռ�ձ�
	
	mot2.delta_u = (int16) (mot2.Kp * (mot2.ek - mot2.ek1) + mot2.Ki * mot2.ek + mot2.Kd * (mot2.ek - 2 * mot2.ek1 + mot2.ek2)); //��������
  mot2.u += mot2.delta_u; //�ۼ�������
  mot2.u = limit(mot2.u, PWM_DUTY_MAX); //����޷�����ֹ�������100%ռ�ձ�
  mot2.duty = (int32) mot2.u; //��������ռ�ձ�
	
	//�������ٶȸ�������
	output_speed(MOTOR1);
	output_speed(MOTOR2);
}

//��������ȡ����ٶ�
int16 input_speed(enum select_motor mot)
{
	int16 speed = 0;
	
	switch(mot) //���ѡ��
	{
		case MOTOR1:
			if (1 == ENC1_DIR) //��������
			{
				speed = (int16)ctimer_count_read(ENC1_LSB); //�������������
			}
			else
			{
				speed = -(int16)ctimer_count_read(ENC1_LSB); //������ȡ��
			}
			ctimer_count_clean(ENC1_LSB); //����������
		break;
			
		case MOTOR2:
			if (1 == ENC2_DIR) //��������
			{
				speed = (int16)ctimer_count_read(ENC2_LSB); //�������������
			}
			else
			{
				speed = -(int16)ctimer_count_read(ENC2_LSB); //������ȡ��
			}
			ctimer_count_clean(ENC2_LSB); //����������
		break;
	}
	
	return speed;
}

//�����ռ�ձ�ת��ΪDRV8701EоƬ�߼�
void output_speed(enum select_motor mot)
{
	switch(mot) //���ѡ��
	{
		case MOTOR1:
			if (mot1.duty >= 0) //�����ռ�ձ�>=0
			{
				MOT1_DIR = 0; //�͵�ƽ��ת
				pwm_duty(MOT1_PWM, (uint32)mot1.duty); //���õ��ռ�ձ�
			}
			else
			{
				MOT1_DIR = 1; //�ߵ�ƽ��ת
				pwm_duty(MOT1_PWM, (uint32)(-mot1.duty)); //���õ��ռ�ձ�
			}
		break;
			
		case MOTOR2:
			if (mot2.duty >= 0) //�����ռ�ձ�>=0
			{
				MOT2_DIR = 0; //�͵�ƽ��ת
				pwm_duty(MOT2_PWM, (uint32)mot2.duty); //���õ��ռ�ձ�
			}
			else
			{
				MOT2_DIR = 1; //�ߵ�ƽ��ת
				pwm_duty(MOT2_PWM, (uint32)(-mot2.duty)); //���õ��ռ�ձ�
			}
		break;
	}
}

//�޷�
int16 limit(int16 value, int16 limit_value)
{
	if (value > limit_value)
		return limit_value;
	else if (value < -limit_value)
		return -limit_value;
	else
		return value;
}