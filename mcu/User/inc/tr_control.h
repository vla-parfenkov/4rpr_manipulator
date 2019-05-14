#ifndef TR_CONTROL_H
#define TR_CONTROL_H

#include "stm32f4xx.h"

enum TrControlState
{
	WAITSTART,
	STARTED,
	STOPED
};

struct  Tr 
{
	int16_t x,y,thetta;
};

struct SpeedTr 
{
	int16_t x,y,thetta;
};

struct GenCoordinate
{
	int16_t d1, d2, d3, d4;
};

struct SecondGenCoordinate
{
	int16_t a1, a2, a3, a4;
};

struct SpeedGenCoordinate
{
	int16_t d1, d2, d3, d4;
};

struct SpeedSecondGenCoordinate
{
	int16_t a1, a2, a3, a4;
};

struct MechState 
{
	struct SpeedGenCoordinate speed;
	enum TrControlState state;
};

struct TrState
{
	struct Tr* tr; //size n
	struct SpeedTr* speedTr; //size n - 1
	uint16_t* t; //size n
};


void initTr(struct TrState* tS, uint16_t size);

struct MechState* getMechStateByTime(uint16_t time);

struct GenCoordinate* getGenCoordinateByTr(struct  Tr tr);

struct SecondGenCoordinate* getSecondGenCoordinateByTr(struct  Tr tr);

struct SpeedGenCoordinate* getSpeedGenCoordinateByTr(struct  Tr tr,
struct SecondGenCoordinate sgc, struct  SpeedTr speedtr);

//struct SecondGenCoordinate getSecondGenCoordinateByTr(struct  Tr );

#endif
