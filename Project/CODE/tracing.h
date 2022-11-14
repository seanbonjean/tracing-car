#ifndef __TRACING_H_
#define __TRACING_H_
#endif

//电感对应运放引脚
#define L1_PIN ADC_P00
#define L2_PIN ADC_P01
#define L3_PIN ADC_P05
#define L4_PIN ADC_P06

void trace_init(void);
int16 trace_once(void);
uint16 adc_mean_filter(ADCN_enum adcn,ADCRES_enum resolution, uint8 count);