#include "tr_control.h"
#include "stdio.h"
#include "math.h"
#include <stdlib.h>
#include "string.h"

struct TrState* trStates;
enum TrControlState state = WAITSTART;
uint32_t trQueuSize[4] = {0,0,0,0};
struct QueueTrEngine *head[4];
struct QueueTrEngine *tail[4];
uint16_t trStatesSize = 0;
uint16_t trStatesCount = 0;
struct Tr currentTr = {0.0, 343.443, 0.0};
struct GenCoordinate currentGC = {538.0, 538.0, 538.0, 538.0};
//мм
float a1 = 150.0; 
float a2 = 150.0; 
float a3 =  -150.0; 
float a4 = -150.0;
float cy = 156.0;
float bx1 = 250.0;
float bx2 = 500.0;
float bx3 = -500.0;
float bx4 = -250.0;
float by1 = 0.0;
float by2 = 120.0;
float by3 = 120.0;
float by4 = 0.0;


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

struct SpeedGenCoordinate getSpeedGenCoordinateByTr(struct  Tr tr,
	struct SecondGenCoordinate sgc, struct  SpeedTr speedtr)
	{
		struct SpeedGenCoordinate spgc;
		float sth = sin(tr.thetta);
		float cth = cos(tr.thetta);
		spgc.d1 = cos(sgc.a1)* speedtr.x + sin(sgc.a1) * speedtr.y + a1 * (cth * sin(sgc.a1) 
				- sth * cos(sgc.a1)) * speedtr.thetta;
		spgc.d2 = cos(sgc.a2)* speedtr.x + sin(sgc.a2) * speedtr.y + a2 * (cth * sin(sgc.a2) 
				- sth * cos(sgc.a2)) * speedtr.thetta;
		spgc.d3 = cos(sgc.a3)* speedtr.x + sin(sgc.a3) * speedtr.y + a3 * (cth * sin(sgc.a3) 
				- sth * cos(sgc.a3)) * speedtr.thetta;
		spgc.d4 = cos(sgc.a4)* speedtr.x + sin(sgc.a4) * speedtr.y + a4 * (cth * sin(sgc.a4) 
				- sth * cos(sgc.a4)) * speedtr.thetta;
		
		return spgc;
	}
	
	
	struct SecondGenCoordinate getSecondGenCoordinateByTr(struct  Tr tr)
	{
		struct SecondGenCoordinate sgc;
		float sth = sin(tr.thetta);
		float cth = cos(tr.thetta);
		sgc.a1 = atan (((tr.y + cy * sth) - by1 + a1 * sth) / (tr.x - bx1 + a1 * cth));
		sgc.a2 = atan (((tr.y + cy * sth) - by2 + a2 * sth) / (tr.x - bx2 + a2 * cth));
		sgc.a3 = atan (((tr.y + cy * sth) - by3 + a3 * sth) / (tr.x - bx3 + a3 * cth));
		sgc.a4 = atan (((tr.y + cy * sth) - by4 + a4 * sth) / (tr.x - bx4 + a4 * cth));
		
		
		return sgc;
	}

void pushQueueTrEngine(uint8_t number, struct MechStateEngine state)
{
	struct QueueTrEngine *newEngineCmd;

	newEngineCmd = tail[number];
	
	if(number > 3)
		return;
	
  tail[number] = (struct QueueTrEngine *)malloc(sizeof(struct QueueTrEngine ));
	if(!tail[number])
		return;
	tail[number]->state = state;
	tail[number]->next = NULL;
  if (trQueuSize[number] == 0)
    head[number] = tail[number];
  else 
    newEngineCmd->next = tail[number];
  trQueuSize[number]++;
}
	
void pushQueueTr(struct Tr tr, struct SpeedTr speedTr, enum TrControlState state)
{
	struct SecondGenCoordinate sgc;
	struct SpeedGenCoordinate speed;
	struct GenCoordinate gc;
	struct MechStateEngine mstate;
		
	sgc = getSecondGenCoordinateByTr(tr);
	
	gc = getGenCoordinateByTr(tr);
	
	speed = getSpeedGenCoordinateByTr(tr, sgc, speedTr);
	
	mstate.speed  = speed.d1;
	mstate.deltaTr  = gc.d1 - currentGC.d1;
	mstate.state = state;
	pushQueueTrEngine(0, mstate);
	mstate.speed  = speed.d2;
	mstate.deltaTr  = gc.d2 - currentGC.d2;
	mstate.state = state;
	pushQueueTrEngine(1, mstate);
	mstate.speed  = speed.d3;
	mstate.deltaTr  = gc.d3 - currentGC.d3;
	mstate.state = state;
	pushQueueTrEngine(2, mstate);
	mstate.speed  = speed.d4;
	mstate.deltaTr  = gc.d4 - currentGC.d4;
	mstate.state = state;
	pushQueueTrEngine(3, mstate);
	currentGC = gc;

}
struct MechStateEngine popQueueTrEngine(uint8_t engineNumber)
{
  struct QueueTrEngine *tr;
  struct MechStateEngine res;
  tr = head[engineNumber];
	if(!tr)
	{
		res.state = STOPED;
		return res;
	}
	trQueuSize[engineNumber]--;
  head[engineNumber] = head[engineNumber]->next;
	res = tr->state;
	free(tr);
  return res;
}

//need free
struct GenCoordinate getGenCoordinateByTr(struct  Tr tr)
{
		struct GenCoordinate gc;
		float sth = sin(tr.thetta);
		float cth = cos(tr.thetta);
		gc.d1 = sqrt(pow(((tr.y + cy * sth) - by1 + a1 * sth),2) + pow((tr.x - bx1 + a1 * cth),2));
		gc.d2 = sqrt(pow(((tr.y + cy * sth) - by2 + a2 * sth),2) + pow((tr.x - bx2 + a2 * cth),2));
		gc.d3 = sqrt(pow(((tr.y + cy * sth) - by3 + a3 * sth),2) + pow((tr.x - bx3 + a3 * cth),2));
		gc.d4 = sqrt(pow(((tr.y + cy * sth) - by4 + a4 * sth),2) + pow((tr.x - bx4 + a4 * cth),2));
		
		return gc;
	
}
	