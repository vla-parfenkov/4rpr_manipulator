#ifndef BUFFER_H
#define BUFFER_H

#include "stm32f4xx.h"
#include <stdbool.h>

struct Buffer
{
	char data[1024];
	uint16_t sizebuff;
	uint16_t head;
	uint16_t tail;
};

void initBuffer(struct Buffer *buf);
int sizeBuffer(struct Buffer *buf);
bool isEmptyBuffer(struct Buffer *buf);
void pushBuffer(char bait, struct Buffer *buf);
char popBuffer(struct Buffer *buf, bool errorFlag);
char showBuffer(struct Buffer *buf, bool empty);
void readDataFromBuffer(struct Buffer *buf, char **data);



#endif
