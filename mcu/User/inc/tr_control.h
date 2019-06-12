#ifndef TR_CONTROL_H
#define TR_CONTROL_H

#include "stm32f4xx.h"
#include <stdbool.h>

enum TrControlState
{
	WAITSTART,
	STARTED,
	STOPED
};

struct  Tr 
{
	float x,y,thetta;
};

struct SpeedTr 
{
	float x,y,thetta;
};

struct GenCoordinate
{
	float d1, d2, d3, d4;
};

struct SecondGenCoordinate
{
	float a1, a2, a3, a4;
};

struct SpeedGenCoordinate
{
	float d1, d2, d3, d4;
};

struct SpeedSecondGenCoordinate
{
	float a1, a2, a3, a4;
};

struct MechState 
{
	struct SpeedGenCoordinate speed;
	struct GenCoordinate tr;
	enum TrControlState state;
};

struct TrState
{
	struct Tr* tr; //size n
	struct SpeedTr* speedTr; //size n - 1
	uint16_t* t; //size n
};

struct MechStateEngine
{
	float speed;
	float deltaTr;
	enum TrControlState state;
};

struct QueueTrEngine {
	struct MechStateEngine state;
	struct QueueTrEngine *next;
};




void pushQueueTr(struct Tr tr, struct SpeedTr speedTr, enum TrControlState state);
struct MechStateEngine popQueueTrEngine(uint8_t engineNumber);


void initTr(struct TrState* tS, uint16_t size);
void deinitTr();

struct GenCoordinate getGenCoordinateByTr(struct  Tr tr);

struct SecondGenCoordinate getSecondGenCoordinateByTr(struct  Tr tr);

struct SpeedGenCoordinate getSpeedGenCoordinateByTr(struct  Tr tr,
struct SecondGenCoordinate sgc, struct  SpeedTr speedtr);

//struct SecondGenCoordinate getSecondGenCoordinateByTr(struct  Tr );

#endif
