#include "headfile.h"
#include "user.h"

//串口初始化
void serial_init(void)
{
	#if(0 == ENABLE_WIRELESS)
	//uart_init(DEBUG_UART, DEBUG_UART_RX_PIN, DEBUG_UART_TX_PIN, DEBUG_UART_BAUD, DEBUG_UART_TIM); //初始化下载器串口
	//这里不初始化是因为board_init函数内含有DEBUG_UART的初始化，不用重复初始化
	#elif(1 == ENABLE_WIRELESS)
	//wireless_uart_init(); //初始化无线串口
	//逐飞开源库的这个函数比下面这个单独初始化多做了两件事：
	//一是设置了无线类型，这会激活逐飞自家的无线读数据函数，数据都被那个函数读了我们自己的函数就读不到了
	//二是初始化了流控引脚，如果数据量少没必要用
	uart_init(WIRELESS_UART, WIRELESS_UART_RX_PIN, WIRELESS_UART_TX_PIN, WIRELESS_UART_BAUD, WIRELESS_TIMER_N); //初始化无线串口
	#endif
}