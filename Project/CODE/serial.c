#include "headfile.h"
#include "user.h"

//���ڳ�ʼ��
void serial_init(void)
{
	#if(0 == ENABLE_WIRELESS)
	//uart_init(DEBUG_UART, DEBUG_UART_RX_PIN, DEBUG_UART_TX_PIN, DEBUG_UART_BAUD, DEBUG_UART_TIM); //��ʼ������������
	//���ﲻ��ʼ������Ϊboard_init�����ں���DEBUG_UART�ĳ�ʼ���������ظ���ʼ��
	#elif(1 == ENABLE_WIRELESS)
	//wireless_uart_init(); //��ʼ�����ߴ���
	//��ɿ�Դ�������������������������ʼ�������������£�
	//һ���������������ͣ���ἤ������Լҵ����߶����ݺ��������ݶ����Ǹ��������������Լ��ĺ����Ͷ�������
	//���ǳ�ʼ�����������ţ������������û��Ҫ��
	uart_init(WIRELESS_UART, WIRELESS_UART_RX_PIN, WIRELESS_UART_TX_PIN, WIRELESS_UART_BAUD, WIRELESS_TIMER_N); //��ʼ�����ߴ���
	#endif
}