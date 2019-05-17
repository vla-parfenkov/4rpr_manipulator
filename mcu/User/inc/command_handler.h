#ifndef COMMAND_HANDLER_H
#define COMMAND_HANDLER_H

#include "main.h"
#include "motor_controller.h"

//Передвинуть в ноль
//void SetZero (void);

void startTimer(TIM_TypeDef* TIMx, uint16_t Period, uint16_t Prescaler);
void stopTimer(TIM_TypeDef* TIMx);
uint16_t getTimer(TIM_TypeDef* TIMx);
void usartCmdHandler(char *cmd);

#endif
