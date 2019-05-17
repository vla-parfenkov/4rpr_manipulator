#include "tr_control.h"
#include "stdio.h"
#include "math.h"
#include <stdlib.h>
#include "string.h"

struct TrState* trStates;
enum TrControlState state = WAITSTART;
uint16_t trStatesSize = 0;
uint16_t trStatesCount = 0;
float a1 = 0.36 / 2; 
float a2 = -0.36 / 2; 
float a3 =  -0.36 / 2; 
float a4 = 0.36 / 2; 
float bx1 = 0.45;
float bx2 = -0.45;
float bx3 = -0.225;
float bx4 = 0.225;
float by1 = 0.095;
float by2 = 0.095;
float by3 = 0;
float by4 = 0;


void initTr(struct TrState *tS, uint16_t size)
{
	trStates = tS;
	trStatesSize = size;
	state = STARTED;
}

void deinitTr()
{
	if(!trStates)
		return;
	if(trStates->tr)
		free(trStates->tr);
	
	if(trStates->speedTr)
		free(trStates->speedTr);
	
	if(trStates->t)
		free(trStates->t);
	
	state = WAITSTART;
	trStatesSize = 0;
	trStatesCount = 0;
	
	free(trStates);
}

//need free
struct MechState* getMechStateByTime(uint16_t time)
{
	uint16_t i;
	uint16_t temp;
	struct Tr nTr;
	struct SpeedTr nspTr;
	struct MechState* res;
	struct SecondGenCoordinate* sgc;
	struct Tr sTr;
	struct SpeedTr sspTr;
	struct SpeedGenCoordinate* speed;
	
	
	if(state != STARTED)
		return NULL;
	
	if(!trStates)
		return NULL;
	
	if(!trStates->t)
		return NULL;
	
	if (!trStates->speedTr)
		return NULL;
	
	if (!trStates->tr)
		return NULL;
	
	
	for (i = trStatesCount; i < trStatesSize; i++)
  {
		trStatesCount = i;
		temp = *(trStates->t + i);
		if (time >= temp && time < *(trStates->t + i + 1))
		{
      break;
		}
	}

	if ( trStatesCount >= trStatesSize - 1)
		state = STOPED;
	
	
	sTr =  *(trStates->tr + trStatesCount);
	sspTr =  *(trStates->speedTr + trStatesCount);

	nTr.thetta = sTr.thetta + sspTr.thetta * (time - temp) * 0.001; 
	nTr.x = sTr.x + sspTr.x * (time - temp) * 0.001;
	nTr.y = sTr.y + sspTr.y * (time - temp) * 0.001;
	
	sgc = getSecondGenCoordinateByTr(nTr);
	
	if(!sgc)
		return NULL;
	
	res = (struct MechState*)malloc(sizeof(struct MechState));
	
	if(!res)
	{
		if (sgc)
			free(sgc);
		return NULL;
	}
	
	speed = getSpeedGenCoordinateByTr(nTr, *sgc, sspTr);
	
	memcpy(&res->speed, speed , sizeof(struct SpeedGenCoordinate));
	res->state = state;

	if (speed)
		free(speed);
	
	return res;
};

//need free
struct SpeedGenCoordinate* getSpeedGenCoordinateByTr(struct  Tr tr,
	struct SecondGenCoordinate sgc, struct  SpeedTr speedtr)
	{
		struct SpeedGenCoordinate* spgc = (struct SpeedGenCoordinate*)malloc(sizeof(struct SpeedGenCoordinate));
		
		if (!spgc)
			return NULL;
		
		spgc->d1 = cos(sgc.a1)* speedtr.x + sin(sgc.a1) * speedtr.y + a1 * (cos(tr.thetta) * sin(sgc.a1) 
				- sin(tr.thetta) * cos(sgc.a1)) * speedtr.thetta;
		spgc->d2 = cos(sgc.a2)* speedtr.x + sin(sgc.a2) * speedtr.y + a2 * (cos(tr.thetta) * sin(sgc.a2) 
				- sin(tr.thetta) * cos(sgc.a2)) * speedtr.thetta;
		spgc->d3 = cos(sgc.a3)* speedtr.x + sin(sgc.a3) * speedtr.y + a3 * (cos(tr.thetta) * sin(sgc.a3) 
				- sin(tr.thetta) * cos(sgc.a3)) * speedtr.thetta;
		spgc->d4 = cos(sgc.a4)* speedtr.x + sin(sgc.a4) * speedtr.y + a4 * (cos(tr.thetta) * sin(sgc.a4) 
				- sin(tr.thetta) * cos(sgc.a4)) * speedtr.thetta;
		
		return spgc;
	}
	
	
	//need free
	struct SecondGenCoordinate* getSecondGenCoordinateByTr(struct  Tr tr)
	{
		struct SecondGenCoordinate* sgc = (struct SecondGenCoordinate*)malloc(sizeof(struct SecondGenCoordinate));
		
		if (!sgc)
			return NULL;
		
		sgc->a1 = atan ((tr.y - by1 + a1*sin(tr.thetta)) / (tr.x - bx1 + a1*cos(tr.thetta)));
		sgc->a2 = atan ((tr.y - by2 + a2*sin(tr.thetta)) / (tr.x - bx2 + a2*cos(tr.thetta)));
		sgc->a3 = atan ((tr.y - by3 + a3*sin(tr.thetta)) / (tr.x - bx3 + a3*cos(tr.thetta)));
		sgc->a4 = atan ((tr.y - by4 + a4*sin(tr.thetta)) / (tr.x - bx4 + a4*cos(tr.thetta)));
		
		
		return sgc;
	}
	