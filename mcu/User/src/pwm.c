#include "pwm.h"


//------------------------
//генерация ШИМ сигнала
//-----------------------
void PWM (TIM_TypeDef* TIMx, uint16_t Pin, uint16_t Period, uint16_t Pul,uint16_t Prescaler)
{
	TIM_OCInitTypeDef PWM_ini; //структура для настройки ШИМ на таймере
	//------------------------------------
	//настройка таймера
	//------------------------------------
	timer_ini (TIMx, Period,Prescaler);
  
	PWM_ini.TIM_OCMode = TIM_OCMode_PWM1 ; //режим работы таймера
	PWM_ini.TIM_OutputState = TIM_OutputState_Enable; // включить режим работы
	PWM_ini.TIM_Pulse = Pul; // Значения сброса сигнала
	PWM_ini.TIM_OCPolarity = TIM_OCPolarity_High ; //Прямой шим

	if ((Pin == GPIO_Pin_9)||(Pin == GPIO_Pin_11))
	{
		TIM_OC4Init(TIMx,&PWM_ini);
	} 
	if ((Pin == GPIO_Pin_8)||(Pin == GPIO_Pin_10))
	{
		TIM_OC3Init(TIMx,&PWM_ini);
	}
	if (Pin == GPIO_Pin_7)
	{
		TIM_OC2Init(TIMx,&PWM_ini);
	}
	if (Pin == GPIO_Pin_6)
	{
		TIM_OC1Init(TIMx,&PWM_ini);
	}
		
	
}






