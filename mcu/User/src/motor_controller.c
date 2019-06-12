#include "motor_controller.h"

uint16_t minPeriodSpeed = 34;
uint16_t taktMask[4];
uint16_t countTakt[4];
uint16_t sizeTakt[4];
uint8_t motorFlag[4];


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

uint16_t calcPeriod(double Speed)
{
	uint16_t res = (MIN_PERIOD_SPEED * 10) / Speed;
	if(res < MIN_PERIOD_SPEED / 2)
		res =  MIN_PERIOD_SPEED / 2;
	return res;
	
}

uint16_t calcImpl(uint16_t path)
{
	return path * 160;
}



//----------------------------------------------------------------------------------
//Функция запускает указанный двигатель в указанном направлении с требуемой скоростью
// Запуск занимает некоторое время требуемое на разгон
//----------------------------------------------------------------------------------
void StartMotor (uint16_t MotorPin, uint8_t Direction, double Speed, uint16_t path)
{
	if ( Direction == FORWARD){
		GPIO_SetBits(GPIOD,MotorPin);
	} else {
		GPIO_ResetBits(GPIOD,MotorPin);
	}
	
	GPIO_SetBits(GPIOE,MotorPin);
	if(MotorPin == AXIS_Q1)
	{
		taktMask[0] = calcPeriod(Speed);
		countTakt[0] = calcImpl(path);
	}
}

//----------------------------------------------------------------------------------
//Функция останавливает указанный двигатель
// Торможение занимает некоторое время
//----------------------------------------------------------------------------------
void StopMotor (uint16_t MotorPin, double Speed)
{

	//Push (SETBITS_COMMAND, 0, 0, MotorPin, 0, 0, 0, GPIOE);
	//GPIO_SetBits(GPIOE,MotorPin);
	StartMotor (MotorPin, FORWARD, 0, 0);
}


//----------------------------------------------------------------------------------
//Функция считает необходимую тактовую частоту  подачи импульсов, исходя из скорости 
//--------------------------------------------------------------------------------

uint16_t CalcTAKT (double Speed, uint16_t MotorPin)
{
	double Mech = 60;
	uint32_t TaktConf = TAKT_CONF; //количество тактов на оборот исходя из настроек драйвера
	double Freq = 0;
	
	if(MotorPin == AXIS_Q1 || MotorPin == AXIS_Q2){
		Mech = 5; // мм на оборот
	}
	if(MotorPin == AXIS_Q3 || MotorPin == AXIS_Q4){
		Mech = 5; // мм на оборот
	}
	

	Freq = (((double)TaktConf) * Speed) / Mech; 
	return (uint16_t)Freq;
	
}	

void motorControlImpl(int16_t speed, int16_t lastSpeed, uint16_t MotorPin)
{
	if(speed == lastSpeed)
		return;
		
	if(speed == 0) 
	{
		StopMotor(MotorPin, speed);
	} 
	else if (speed > 0)
	{
		StartMotor (MotorPin, FORWARD, speed, 0);
	} else 
	{
		StartMotor (MotorPin, REVERSE, speed, 0);
	}
}

void motorControl(struct SpeedGenCoordinate speed, struct SpeedGenCoordinate lastSpeed)
{
	if(speed.d1 > 100 && speed.d2 > 100 && speed.d3 > 100 && speed.d4 > 100)
	{
		StopMotor(AXIS_Q1, 0);
		StopMotor(AXIS_Q2, 0);
		StopMotor(AXIS_Q3, 0);
		StopMotor(AXIS_Q4, 0);
		return;
	}
	
	motorControlImpl(speed.d1, lastSpeed.d1, AXIS_Q1);
	motorControlImpl(speed.d2, lastSpeed.d2, AXIS_Q2);
	motorControlImpl(speed.d3, lastSpeed.d3, AXIS_Q3);
	motorControlImpl(speed.d4, lastSpeed.d4, AXIS_Q4);
	
	
}

uint16_t getMotorPin(uint8_t number)
{
	switch(number)
	{
		case 0: 
			return AXIS_Q1;
		case 1: 
			return AXIS_Q2;
		case 2: 
			return AXIS_Q3;
		case 3: 
			return AXIS_Q4;
		default: return 0;
	}
}

void readStateFromQueueEngine(uint8_t motorNumber)
{
	struct MechStateEngine state = popQueueTrEngine(motorNumber);
	if(state.state == STOPED)
	{
		sizeTakt[motorNumber] = 1;
		taktMask[motorNumber] = 0;
	} else 
	{
		if(state.speed >= 0 || state.deltaTr >= 0)
			GPIO_SetBits(GPIOD,getMotorPin(motorNumber));
		else
		{
			GPIO_ResetBits(GPIOD,getMotorPin(motorNumber));
			if(state.speed < 0) 
				state.speed = state.speed * (-1);
			if(state.deltaTr < 0)
				state.deltaTr = state.deltaTr * (-1);
		}
		sizeTakt[motorNumber] = calcPeriod(state.speed);
		taktMask[motorNumber] = calcImpl(state.deltaTr);
	}
}

void motorQueueControlImpl(uint8_t motorNumber)
{
	if(motorNumber > 3)
		return;
	
	if(countTakt[motorNumber] == sizeTakt[motorNumber])
	{	
		if(taktMask[motorNumber] > 0)
		{
			if(motorFlag[motorNumber] == 0)
			{
				GPIO_SetBits(GPIOB,getMotorPin(motorNumber));
				motorFlag[motorNumber] = 1;
			} else
			{
				GPIO_ResetBits(GPIOB,getMotorPin(motorNumber));
				motorFlag[motorNumber] = 0;
				taktMask[motorNumber] --;
			}
		} else
		{
			readStateFromQueueEngine(motorNumber);
		}
		countTakt[motorNumber] = 0;
	} 
	else 
	{
			countTakt[motorNumber]++;
	}
	
}

void motorQueueControl()
{
	int i = 0;
	for (i = 0; i < 4; i++)
		motorQueueControlImpl(i);
}

