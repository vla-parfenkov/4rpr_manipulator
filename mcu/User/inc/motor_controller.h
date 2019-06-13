#ifndef MOTOR_CONTROLLER_H
#define MOTOR_CONTROLLER_H

#include "main.h"
#include "mcu_ini.h"
#include "pwm.h"
#include "command_handler.h"
#include "queue.h"



#define FORWARD 1 // DIR+ Anti-Clock
#define REVERSE 0 // DIR- Clockwise
#define ACCELERATION 4 // об/сек*сек 
#define DECELERATION 0.8  // об/сек*сек
#define MIN_PERIOD_SPEED 50 
#define START_FREQ  400 // такт/сек
#define TAKT_CONF 800 // настройки на драйвере
#define TIM_PERIOD 100 // период срабатывания таймера
#define TIM_PULSE 50 // ШИМ сброс
#define TIM_TAKT 84000000 //тактирование таймера
typedef uint8_t					Bool; 

//старт двигатель
void StartMotor (uint16_t MotorPin, uint8_t Direction, double Speed, float path);

//остановить двигатель
void StopMotor (uint16_t MotorPin, double Speed);

void motorControl(struct SpeedGenCoordinate speed, struct SpeedGenCoordinate lastSpeed);

void motorQueueControl();
	
//Определить настройки таймера:
uint16_t CalcTAKT (double Speed, uint16_t MotorPin);

//разгон
void Racing(uint16_t ReqFrequency, double Acceler, uint16_t MotorPin, uint16_t StartFrequency, double* deltaPosition);

//торможение
void Braking(uint16_t ReqFrequency, double Deceler, uint16_t MotorPin, uint16_t StartFrequency, double* deltaPosition);



#endif 
