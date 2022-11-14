#include "headfile.h"
#include "user.h"

//电感电压采样值
struct Lvalue
{
	uint16 L1;
	uint16 L2;
	uint16 L3;
	uint16 L4;
}Lv;

void trace_init(void)
{
	//初始化ADC，时钟频率 = SYSclk/2
	adc_init(L1_PIN, ADC_SYSclk_DIV_2);
	adc_init(L2_PIN, ADC_SYSclk_DIV_2);
	adc_init(L3_PIN, ADC_SYSclk_DIV_2);
	adc_init(L4_PIN, ADC_SYSclk_DIV_2);
}

//计算转角参考值
int16 trace_once(void)
{
	int16 angle; //转角参考值
	
	//均值滤波，采样5次求平均，ADC分辨率8位
	Lv.L1 = adc_mean_filter(L1_PIN, ADC_8BIT, 5);
	Lv.L2 = adc_mean_filter(L2_PIN, ADC_8BIT, 5);
	Lv.L3 = adc_mean_filter(L3_PIN, ADC_8BIT, 5);
	Lv.L4 = adc_mean_filter(L4_PIN, ADC_8BIT, 5);
	
	//差比和
	angle = ((int16)Lv.L1 - (int16)Lv.L4) / (int16)(Lv.L1 + Lv.L4);
	
	return angle;
}

//ADC均值滤波
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