#ifndef BUFFER_H
#define BUFFER_H

#include "stm32f4xx.h"
#include <stdbool.h>

struct Buffer
{
	uint8_t data[1024];
	uint16_t sizebuff;
	uint16_t head;
	uint16_t tail;
};

void initBuffer(struct Buffer *buf);
void deinitBuffer(struct Buffer *buf);
int sizeBuffer(struct Buffer *buf);
bool isEmptyBuffer(struct Buffer *buf);
void pushBuffer(uint8_t bait, struct Buffer *buf);
uint8_t popBuffer(struct Buffer *buf, bool errorFlag);
uint8_t showBuffer(struct Buffer *buf, bool empty);
void readDataFromBuffer(struct Buffer *buf, uint8_t *data);



#endif
