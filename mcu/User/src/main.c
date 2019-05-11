#include "main.h"
#include <stdlib.h>

uint32_t delay_count = 0;//счетчик задержки
uint8_t button_count = 0;//счетчик кнопки
uint8_t button_flag = 0;//флаг кнопки
uint8_t flag_zero = 0; //флаг операции установки нуля
uint8_t flag = 0;
uint32_t time_count = 0;
uint8_t SystikCount = 0;


enum MCUState mcuState = WaitCMD;
uint32_t doCmdTim = 0;
struct Buffer *g_buf = 0;


//---------------------
//Функция прерывания
//---------------------
void SysTick_Handler(void)
{
	if(delay_count > 0){
		delay_count--;
	}
	
	time_count ++;
	
	if( BUTTON_READ() == 1){
		if(button_count<5)//5 для защиты от дребезга
			{
			button_count++;
		} else {
				button_flag=1;
			}	
	} else {
			if(button_count>0) {
			button_count--;
		} else {
				button_flag=0;
			}	
	}
}

void USART2_IRQHandler()
{
	char data;
	char *cmd;
	if(USART_GetITStatus(USART2, USART_IT_RXNE) == SET)
	{
		USART_ClearITPendingBit(USART2, USART_IT_RXNE);
		data = (char)USART_ReceiveData(USART2);
		pushBuffer((uint8_t)data, g_buf);
		if( data == '\n')
		{
			readDataFromBuffer(g_buf, (uint8_t *)cmd);
			pushQueueCmd(cmd);
		}
	}
}

//---------------------
//Функция задержки
//---------------------
void delay_mcsXhun(uint32_t delay_temp)
{
	delay_count = delay_temp;
	
	while(delay_count){};
	
}
	

int main(void)
{
	struct MechState* currentState = 0;
	char *cmd = NULL;
	
	uint8_t flagTest = 0;
	initBuffer(g_buf);

	SystemCoreClockUpdate(); //посчитать SystemCoreClock
	SysTick_Config(SystemCoreClock/10000);//генерация каждую десятую миллисекунду
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //Включение тактирования 4 таймера
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //Включение тактирования 2 таймера
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);   //Тактирование порта D
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE); //Тактирование порта B
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//Тактирование порта A
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);   //Тактирование порта E
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);   //Тактирование порта C
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);   //Тактирование порта C

	
	//------------------------------------
	//настройка портов на различные режимы
	//------------------------------------
	GPIO_ini ( GPIOD, AXIS_Q1| AXIS_Q2| AXIS_Q3 |AXIS_Q4|GPIO_Pin_12|GPIO_Pin_13, OUT); //порт D на выход 
	GPIO_ini ( GPIOC, GPIO_Pin_6|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5, IN); //порт C на вход
	GPIO_ini ( GPIOB, AXIS_Q1| AXIS_Q2| AXIS_Q3 |AXIS_Q4,AF);//порт B на AF режим
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource6 , GPIO_AF_TIM4); //настройка AF режима
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource7 , GPIO_AF_TIM4); //настройка AF режима
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource8 , GPIO_AF_TIM4); //настройка AF режима
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource9 , GPIO_AF_TIM4); //настройка AF режима
	//GPIO_PinAFConfig(GPIOB, GPIO_PinSource10 , GPIO_AF_TIM2); //настройка AF режима
	//GPIO_PinAFConfig(GPIOB, GPIO_PinSource11 , GPIO_AF_TIM2); //настройка AF режима
	GPIO_ini ( GPIOE, AXIS_Q1| AXIS_Q2| AXIS_Q3 |AXIS_Q4,OUT); //порт E на выход
	usartIni();
	
	TIM_Cmd(TIM4, ENABLE);
	//TIM_Cmd(TIM2, ENABLE);
	
	//timer_mcsXhun(START_T);
	//startTimer(TIM3);
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	while( 1 )
	{
		switch(mcuState)
		{
			case WaitCMD:
			{
				cmd = popQueueCmd();
				if(cmd)
				{
					usartCmdHandler(cmd);
					free(cmd);
				}
				break;
			}
			case DoCMD:
			{
				doCmdTim = timer_mcsXhun(SEE_T);
				if(doCmdTim >= 300000)
				{
					mcuState = WaitCMD;
					initBuffer(g_buf);
					USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
				}
				break;
			}
			default:
				break;
		}
	}
	
	deinitBuffer(g_buf);
	
}

uint32_t timer_mcsXhun(uint8_t mode)
{
	if (mode == START_T){
		time_count = 0;
		return 0;
	} else {
		return  time_count;
	}
}

enum MCUState getMcuState()
{
	return mcuState;
}
void setMcuState(enum MCUState state)
{
	mcuState = state;
}

uint32_t getDoCmdTim()
{
	return doCmdTim;
}

struct Buffer *getUsartBuffer()
{
	return g_buf;
}

