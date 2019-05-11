#include "motor_controller.h"

double CalcSpeed (uint16_t TAKT, uint16_t MotorPin)
{
	double Mech = 60;
	uint32_t TaktConf = TAKT_CONF; //количество тактов на оборот исходя из настроек драйвера
	double Speed = 0;
	
	if(MotorPin == AXIS_Q1 || MotorPin == AXIS_Q2){
		Mech = 56.5; // мм на оборот
	}
	if(MotorPin == AXIS_Q3 || MotorPin == AXIS_Q4){
		Mech = 56.5; // мм на оборот
	}

	Speed = ( (double) TAKT * Mech) / ((double) TaktConf)  ; 
	return Speed;
}


//----------------------------------------------------------------------------------
//Функция запускает указанный двигатель в указанном направлении с требуемой скоростью
// Запуск занимает некоторое время требуемое на разгон
//----------------------------------------------------------------------------------
void StartMotor (uint16_t MotorPin, uint8_t Direction, double Speed)
{
	TIM_TypeDef *TypeTim = 0;
	
	/*if ( MotorPin == AXIS_Z1 || MotorPin == AXIS_Z2){
		TypeTim = TIM2;
	} else {*/
		TypeTim = TIM4;
	//}
	
	if ( Direction == FORWARD){
		//Push (SETBITS_COMMAND, 0, 0, MotorPin, 0, 0, 0, GPIOD);
		GPIO_SetBits(GPIOD,MotorPin);
	} else {
		//Push (RESETBITS_COMMAND, 0, 0, MotorPin, 0, 0, 0, GPIOD);
		GPIO_ResetBits(GPIOD,MotorPin);
	}
	
	//Push (PWM_COMMAND, 0, TypeTim , MotorPin, TIM_PERIOD, TIM_PULSE, TIM_TAKT/(TIM_PERIOD * CalcTAKT (Speed, MotorPin)), 0);
	//Push (RESETBITS_COMMAND, 0, 0, MotorPin, 0, 0, 0, GPIOE);
	PWM (TypeTim, MotorPin, TIM_PERIOD, TIM_PULSE, TIM_TAKT/(TIM_PERIOD * CalcTAKT (Speed, MotorPin)));
	GPIO_ResetBits(GPIOE,MotorPin);

}

//----------------------------------------------------------------------------------
//Функция останавливает указанный двигатель
// Торможение занимает некоторое время
//----------------------------------------------------------------------------------
void StopMotor (uint16_t MotorPin, double Speed)
{

	//Push (SETBITS_COMMAND, 0, 0, MotorPin, 0, 0, 0, GPIOE);
	GPIO_SetBits(GPIOE,MotorPin);
}




//----------------------------------------------------------------------------------
//Функция линейно набирает требуемую скорость 
//----------------------------------------------------------------------------------

/*void Racing(uint16_t ReqFrequency, double Acceler, uint16_t MotorPin, uint16_t StartFrequency, double* deltaPosition)
{
	TIM_TypeDef *TypeTim = 0;

	uint32_t CountPulse = 1; // текущая дискрета ускорения
	uint16_t CountFrequency = StartFrequency; // текущая частота
	float gSnum = StartFrequency - Acceler/(2 * StartFrequency); // считаем параметр старта
	uint32_t Time = 10000 / StartFrequency; //время импульса в мс
	float CountCoef = 0;
	uint16_t CurrentPin = 0;
	uint8_t i = 0;
	uint16_t TestBit = 0x1000;
  gSnum = gSnum * gSnum;
	


	
	while ( CountFrequency < ReqFrequency){
		TestBit = 0x1000;
		for ( i = 1; i < 7; ++i) {
			CurrentPin = (TestBit >> i) & MotorPin;
			if (CurrentPin != 0){
				if ( CurrentPin == AXIS_Z1 || CurrentPin == AXIS_Z2){
					TypeTim = TIM2;
				} else {
					TypeTim = TIM4;
				}
				//Push (PWM_COMMAND, 0, TypeTim , CurrentPin, TIM_PERIOD, TIM_PULSE, TIM_TAKT/(TIM_PERIOD * CountFrequency), 0);
				//Push (DELAY_COMMAND, Time, 0, 0, 0, 0, 0, 0);
				//deltaPosition [6 - i] = deltaPosition [6 - i] + CalcSpeed( CountFrequency, CurrentPin) / (double)Time;
				PWM (TypeTim, CurrentPin, TIM_PERIOD, TIM_PULSE, TIM_TAKT/(TIM_PERIOD * CountFrequency));
				delay_mcsXhun(Time);
		}
		}	
		CountCoef = (sqrt( gSnum + 2 * CountPulse * Acceler ) + sqrt( gSnum + 2 * (CountPulse - 1)* Acceler ));
		Time = 20000 / CountCoef + 1;
		CountFrequency = CountCoef / 2;
		CountPulse++;
	}	
	
		TestBit = 0x1000;
		for ( i = 1; i < 7; ++i) {
			CurrentPin = (TestBit >> i) & MotorPin;
			if (CurrentPin != 0){
				if ( CurrentPin == AXIS_Z1 || CurrentPin == AXIS_Z2){
					TypeTim = TIM2;
				} else {
					TypeTim = TIM4;
				}
				//Push (PWM_COMMAND, 0, TypeTim , CurrentPin, TIM_PERIOD, TIM_PULSE, TIM_TAKT/(TIM_PERIOD * ReqFrequency), 0);
				PWM (TypeTim, CurrentPin, TIM_PERIOD, TIM_PULSE,  TIM_TAKT/(TIM_PERIOD * ReqFrequency));
		}
		}

}*/

//----------------------------------------------------------------------------------
//Функция считает необходимую тактовую частоту  подачи импульсов, исходя из скорости 
//--------------------------------------------------------------------------------

uint16_t CalcTAKT (double Speed, uint16_t MotorPin)
{
	double Mech = 60;
	uint32_t TaktConf = TAKT_CONF; //количество тактов на оборот исходя из настроек драйвера
	double Freq = 0;
	
	if(MotorPin == AXIS_Q1 || MotorPin == AXIS_Q2){
		Mech = 56.5; // мм на оборот
	}
	if(MotorPin == AXIS_Q3 || MotorPin == AXIS_Q4){
		Mech = 56.5; // мм на оборот
	}
	

	Freq = (((double)TaktConf) * Speed) / Mech; 
	return (uint16_t)Freq;
	
}	

//----------------------------------------------------------------------------------
//Функция линейно замедляет двигатель
//----------------------------------------------------------------------------------

/*void Braking(uint16_t ReqFrequency, double Deceler, uint16_t MotorPin, uint16_t StartFrequency, double* deltaPosition)
{
	TIM_TypeDef *TypeTim = 0;

	uint8_t CountPulse = 1; // текущая дискрета торможения
	uint16_t CountFrequency = StartFrequency; // текущая частота
	uint32_t Time = 10000 / StartFrequency; //время импульса в мс
	float CountCoef = 0; 
	uint16_t CurrentPin = 0;
	uint8_t i = 0;
	uint16_t TestBit = 0x1000;
  

	
	while ( CountFrequency > ReqFrequency){
		TestBit = 0x1000;
		for ( i = 1; i < 7; ++i) {
			CurrentPin = (TestBit >> i) & MotorPin;
			if (CurrentPin != 0){
				if ( CurrentPin == AXIS_Z1 || CurrentPin == AXIS_Z2){
					TypeTim = TIM2;
				} else {
					TypeTim = TIM4;
				}
				//Push (PWM_COMMAND, 0, TypeTim , CurrentPin, TIM_PERIOD, TIM_PULSE, TIM_TAKT/(TIM_PERIOD * CountFrequency), 0);
				//Push (DELAY_COMMAND, Time, 0, 0, 0, 0, 0, 0);
				//deltaPosition [6 - i] = deltaPosition [6 - i] + CalcSpeed( CountFrequency, CurrentPin) / (double)Time;
				PWM (TypeTim, CurrentPin, TIM_PERIOD, TIM_PULSE, TIM_TAKT/(TIM_PERIOD * CountFrequency));
				delay_mcsXhun(Time);
		}
		}
		CountCoef = (sqrt( StartFrequency * StartFrequency - 2 * CountPulse * Deceler ) + sqrt( StartFrequency * StartFrequency - 2 * (CountPulse - 1)* Deceler ));
		Time = 20000 / CountCoef + 1;
		CountFrequency = (CountCoef) /  2;
		CountPulse++;
	}		
	
		TestBit = 0x1000;
		for ( i = 1; i < 7; ++i) {
			CurrentPin = (TestBit >> i) & MotorPin;
			if (CurrentPin != 0){
				if ( MotorPin == AXIS_Z1 || MotorPin == AXIS_Z2){
					TypeTim = TIM2;
				} else {
					TypeTim = TIM4;
				}
				//Push (PWM_COMMAND, 0, TypeTim , CurrentPin, TIM_PERIOD, TIM_PULSE, TIM_TAKT/(TIM_PERIOD * ReqFrequency), 0);
				PWM (TypeTim, CurrentPin, TIM_PERIOD, TIM_PULSE, TIM_TAKT/(TIM_PERIOD * ReqFrequency));
		}
		}
	
}*/

void motorControl(struct SpeedGenCoordinate speed)
{
	if(speed.d1 == 0) 
	{
		StopMotor(AXIS_Q1, speed.d1);
	} else if (speed.d1 > 0)
	{
		StartMotor (AXIS_Q1, FORWARD, speed.d1);
	} else 
	{
		StartMotor (AXIS_Q1, REVERSE, speed.d1);
	}
	
	if(speed.d2 == 0) 
	{
		StopMotor(AXIS_Q2, speed.d2);
	} else if (speed.d2 > 0)
	{
		StartMotor (AXIS_Q2, FORWARD, speed.d2);
	} else 
	{
		StartMotor (AXIS_Q2, REVERSE, speed.d2);
	}
	
	if(speed.d3 == 0) 
	{
		StopMotor(AXIS_Q3, speed.d1);
	} else if (speed.d3 > 0)
	{
		StartMotor (AXIS_Q3, FORWARD, speed.d3);
	} else 
	{
		StartMotor (AXIS_Q3, REVERSE, speed.d3);
	}
	
	if(speed.d4 == 0) 
	{
		StopMotor(AXIS_Q4, speed.d4);
	} else if (speed.d4 > 0)
	{
		StartMotor (AXIS_Q4, FORWARD, speed.d4);
	} else 
	{
		StartMotor (AXIS_Q4, REVERSE, speed.d4);
	}
	
}


