#ifndef MCU_INI_H
#define MCU_INI_H

#include "stm32f4xx.h"
#include "main.h"


void GPIO_ini (GPIO_TypeDef* GPIOPORT, uint16_t Pin,uint8_t GMode);
void timer_ini (TIM_TypeDef* TIMx, uint16_t Period, uint16_t Prescaler);
void usartIni();

#endif 

