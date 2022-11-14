#include "headfile.h"
#include "user.h"

//��е�ѹ����ֵ
struct Lvalue
{
	uint16 L1;
	uint16 L2;
	uint16 L3;
	uint16 L4;
}Lv;

void trace_init(void)
{
	//��ʼ��ADC��ʱ��Ƶ�� = SYSclk/2
	adc_init(L1_PIN, ADC_SYSclk_DIV_2);
	adc_init(L2_PIN, ADC_SYSclk_DIV_2);
	adc_init(L3_PIN, ADC_SYSclk_DIV_2);
	adc_init(L4_PIN, ADC_SYSclk_DIV_2);
}

//����ת�ǲο�ֵ
int16 trace_once(void)
{
	int16 angle; //ת�ǲο�ֵ
	
	//��ֵ�˲�������5����ƽ����ADC�ֱ���8λ
	Lv.L1 = adc_mean_filter(L1_PIN, ADC_8BIT, 5);
	Lv.L2 = adc_mean_filter(L2_PIN, ADC_8BIT, 5);
	Lv.L3 = adc_mean_filter(L3_PIN, ADC_8BIT, 5);
	Lv.L4 = adc_mean_filter(L4_PIN, ADC_8BIT, 5);
	
	//��Ⱥ�
	angle = ((int16)Lv.L1 - (int16)Lv.L4) / (int16)(Lv.L1 + Lv.L4);
	
	return angle;
}

//ADC��ֵ�˲�
uint16 adc_mean_filter(ADCN_enum adcn,ADCRES_enum resolution, uint8 count)
{
	uint32 sum = 0;
  uint8 i;
	
	if (0 == count)
		return 0;
	
  for (i = 0; i < count; i++)
    sum += adc_once(adcn, resolution);

  sum = sum / count;
	
  return (uint16)sum;
}