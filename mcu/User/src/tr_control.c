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
struct Tr currentTr = {0.0,  372.625, 0.0};
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

bool checkSpace(struct GenCoordinate gc)
{
	if(gc.d1 < 537.0 || gc.d2 < 537.0 || gc.d3 < 538.0 || gc.d4 < 538.0)
	{
		return false;
	}
	
	if(gc.d1 > 838.0 || gc.d2 > 838.0 || gc.d3 > 838.0 || gc.d4 > 838.0)
	{
		return false;
	}
	
	return true;
}

struct SpeedGenCoordinate getSpeedGenCoordinateByTr(struct  Tr tr,
    struct GenCoordinate gc, struct  SpeedTr speedtr)
{
        struct SpeedGenCoordinate spgc;
        float sth = sin(tr.thetta);
        float cth = cos(tr.thetta);
        spgc.d1 =  (((tr.x + cy * sth)  - bx1 + a1 * cth)
                * (speedtr.x + cy * cth * speedtr.thetta - a1 * sth * speedtr.thetta)
                + ((tr.y + cy * cth) - by1 + a1 * sth)
                * (speedtr.y - cy * sth * speedtr.thetta + a1 * cth * speedtr.thetta)) / gc.d1;
        spgc.d2 =  (((tr.x + cy * sth)  - bx2 + a2 * cth)
                * (speedtr.x + cy * cth * speedtr.thetta - a2 * sth * speedtr.thetta)
                + ((tr.y + cy * cth) - by2 + a1 * sth)
                * (speedtr.y - cy * sth * speedtr.thetta + a2 * cth * speedtr.thetta)) / gc.d2;
        spgc.d3 =  (((tr.x + cy * sth)  - bx3 + a3 * cth)
                * (speedtr.x + cy * cth * speedtr.thetta - a3 * sth * speedtr.thetta)
                + ((tr.y + cy * cth) - by3 + a3 * sth)
                * (speedtr.y - cy * sth * speedtr.thetta + a3 * cth * speedtr.thetta)) / gc.d3;
        spgc.d4 =  (((tr.x + cy * sth)  - bx4 + a4 * cth)
                * (speedtr.x + cy * cth * speedtr.thetta - a4 * sth * speedtr.thetta)
                + ((tr.y + cy * cth) - by4 + a4 * sth)
                * (speedtr.y - cy * sth * speedtr.thetta + a4 * cth * speedtr.thetta)) / gc.d4;
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
	struct SpeedGenCoordinate speed;
	struct GenCoordinate gc;
	struct MechStateEngine mstate;
	
	gc = getGenCoordinateByTr(tr);
	
	speed = getSpeedGenCoordinateByTr(tr, gc, speedTr);

	if(!checkSpace(gc))
		return;
	
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
    gc.d1 = sqrt(pow(((tr.y + cy * cth) - by1 + a1 * sth),2) + pow(((tr.x + cy * sth)  - bx1 + a1 * cth),2));
    gc.d2 = sqrt(pow(((tr.y + cy * cth) - by2 + a2 * sth),2) + pow(((tr.x + cy * sth) - bx2 + a2 * cth),2));
    gc.d3 = sqrt(pow(((tr.y + cy * cth) - by3 + a3 * sth),2) + pow(((tr.x + cy * sth) - bx3 + a3 * cth),2));
    gc.d4 = sqrt(pow(((tr.y + cy * cth) - by4 + a4 * sth),2) + pow(((tr.x + cy * sth) - bx4 + a4 * cth),2));
		
		return gc;
	
}
	