#include "mcu_ini.h"


//-----------------------------
//Настройка портов ввода/вывода
//------------------------------



void GPIO_ini (GPIO_TypeDef* GPIOPORT, uint16_t Pin,uint8_t GMode)
{
	GPIO_InitTypeDef GPIO_INITPORT; //структура для настройки порта
	
	if(GMode == OUT)
	{
	GPIO_INITPORT.GPIO_Pin = Pin; //включение выходов
	GPIO_INITPORT.GPIO_Mode = GPIO_Mode_OUT; //настройка на AF режим ножки 
	GPIO_INITPORT.GPIO_Speed = GPIO_Speed_2MHz; //скорость 
	GPIO_INITPORT.GPIO_OType = GPIO_OType_PP; //подтягивание земли к питанию
	GPIO_INITPORT.GPIO_PuPd = GPIO_PuPd_NOPULL; 
  }
	if(GMode == IN)
	{
		GPIO_INITPORT.GPIO_Pin = Pin; //включение входов
	GPIO_INITPORT.GPIO_Mode = GPIO_Mode_IN; //настройка  режим ножки 
	GPIO_INITPORT.GPIO_Speed = GPIO_Speed_2MHz; //скорость 
	GPIO_INITPORT.GPIO_OType = GPIO_OType_PP; //подтягивание земли к питанию
	GPIO_INITPORT.GPIO_PuPd = GPIO_PuPd_DOWN;
	}
	if(GMode == AF){
		
		GPIO_INITPORT.GPIO_Pin = Pin; //включение выходов
	GPIO_INITPORT.GPIO_Mode = GPIO_Mode_AF; //настройка на AF режим ножки 
	GPIO_INITPORT.GPIO_Speed = GPIO_Speed_100MHz; //скорость 
	GPIO_INITPORT.GPIO_OType = GPIO_OType_PP; //подтягивание земли к питанию
	GPIO_INITPORT.GPIO_PuPd = GPIO_PuPd_NOPULL; 
	}
	if(GMode == AN ){	
	GPIO_INITPORT.GPIO_Pin = Pin; //включение выходов
	GPIO_INITPORT.GPIO_Mode = GPIO_Mode_AN; //настройка на аналоговый режим ножки 
	GPIO_INITPORT.GPIO_Speed = GPIO_Speed_2MHz; //скорость 
	GPIO_INITPORT.GPIO_OType = GPIO_OType_PP; //подтягивание земли к питанию
	GPIO_INITPORT.GPIO_PuPd = GPIO_PuPd_NOPULL; 
	}
	
	GPIO_Init(GPIOPORT, &GPIO_INITPORT);
	
}





	//-----------------
	//настройка таймера
	//------------------
	
void timer_ini (TIM_TypeDef* TIMx, uint16_t Period, uint16_t Prescaler)
{
	TIM_TimeBaseInitTypeDef TIM_timer1;//структура отвечающая за настройку таймера
	
	
	TIM_timer1.TIM_Prescaler = Prescaler; //Предделитель 
	TIM_timer1.TIM_CounterMode = TIM_CounterMode_Up; //счет вверх
	TIM_timer1.TIM_Period = Period ; //период таймера 
	TIM_timer1.TIM_ClockDivision = TIM_CKD_DIV1;
	
	TIM_TimeBaseInit(TIMx, &TIM_timer1); //функция настройки таймера
	

}

void usartIni()
{
	GPIO_InitTypeDef gpioUSART;
	USART_InitTypeDef usart;
	
	gpioUSART.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3;
	gpioUSART.GPIO_Mode = GPIO_Mode_AF;
	gpioUSART.GPIO_Speed = GPIO_Speed_50MHz; 
	gpioUSART.GPIO_OType = GPIO_OType_PP;
	gpioUSART.GPIO_PuPd = GPIO_PuPd_UP; 
	
	GPIO_Init(GPIOA, &gpioUSART);
	
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2);
	
	usart.USART_BaudRate = 9600;
	usart.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	usart.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	usart.USART_Parity = USART_Parity_No;
	usart.USART_StopBits = USART_StopBits_1;
	usart.USART_WordLength = USART_WordLength_8b;
	
	USART_Init(USART2, &usart);
	
	NVIC_EnableIRQ(USART2_IRQn);
	USART_Cmd(USART2, ENABLE);
}


