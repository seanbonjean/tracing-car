#include "headfile.h"
#include "user.h"

struct servo{
	float Kp, Kd; //λ��ʽPD����
	int16 u; //�����
	int16 ek, ek1; //�������&��ʷ���
	int16 real_pos; //ʵ��λ��ƫ��
	int16 set_pos; //����λ��ƫ��
	int32 duty; //���ռ�ձ�
}ser;

void servoPID_init(void)
{
	pit_timer_ms(TIM_4, 5); //��ʱ����ʼ��������5ms
	
	pwm_init(SERVO_PIN, 50, SERVO_CENTER); //���PWM��ʼ��������
	
	ser.set_pos = 0; //����λ��ƫ��Ϊ0�����ϸ�������������ʻ
	
	//����PDϵ��
  ser.Kp = SER_KP; //proportional
  ser.Kd = SER_KD; //derivative
	
	ser.ek = 0; //ek����ֵ
}

void servoPID(int16 pos)
{
  ser.real_pos = pos; //��ȡ��д��������ص�ת�ǲο�ֵ

  ser.ek1 = ser.ek; //������ʷ���
  ser.ek = ser.set_pos - ser.real_pos; //����error term

  ser.u = (int16) (ser.Kp * ser.ek + ser.Kd * (ser.ek - ser.ek1)); //�������ռ�ձ�
  ser.u = limit(ser.u, SERVO_LIMIT); //����޷�

  ser.duty = (int32) ser.u; //������ռ�ձ�
  pwm_duty(SERVO_PIN, SERVO_CENTER + ser.duty); //ִ��
}