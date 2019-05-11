#ifndef PWM_H
#define PWM_H

#include "stm32f4xx.h"
#include "main.h"

void PWM (TIM_TypeDef* TIMx, uint16_t Pin, uint16_t Period, uint16_t Pul,uint16_t Prescaler);


#endif 

