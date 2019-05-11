
#include "buffer.h"
#include <stdlib.h>
#include <string.h> 

void initBuffer(struct Buffer *buf)
{
	buf = (struct Buffer*)malloc(sizeof(struct Buffer));
	if(buf == NULL)
		return;
	buf->head = 0;
	buf->tail = 0;
	buf->sizebuff = 1024; 
	buf->data[0] = 0;
}

void deinitBuffer(struct Buffer *buf)
{
	if(buf)
		free(buf);
}

int sizeBuffer(struct Buffer *buf)
{
	if(!buf)
		return -1;
	if (buf->head > buf->tail)
    return (buf->sizebuff - buf->head + buf->tail);
  else
    return (buf->tail - buf->head);
}

void pushBuffer(uint8_t bait, struct Buffer *buf)
{
	if(!buf)
		return;
	if(sizeBuffer(buf) != buf->sizebuff)
	{
		buf->data[buf->tail] = bait;
		buf->tail = (buf->tail + 1) % buf->sizebuff;
	}
}

bool isEmptyBuffer(struct Buffer *buf)
{
	if(!buf)
		return true;
	
	return (buf->head == buf->tail);
}

uint8_t popBuffer(struct Buffer *buf, bool errorFlag)
{
	uint8_t res = 0;
	if (isEmptyBuffer(buf))
	{		
		errorFlag = true;
    return res;
	}
  res = buf->data[buf->head];
  buf->head = (buf->head + 1) % buf->sizebuff;
	errorFlag = false;
  return res;
}

uint8_t showBuffer(struct Buffer *buf, bool empty)
{
	uint8_t res = 0;
	if (isEmptyBuffer(buf))
	{		
		empty = true;
    return res;
	}
  res = buf->data[buf->head];
	empty = false;
  return res;
}

void readDataFromBuffer(struct Buffer *buf, uint8_t *data)
{
	uint8_t *p = buf->data;
	data = NULL;
	if(!buf)
		return;
	
	if(isEmptyBuffer(buf))
		return;
	
	if (buf->head > buf->tail)
	{
		data = (uint8_t*)malloc(sizeof(uint8_t) * (buf->sizebuff - buf->head + buf->tail));
    if(data == NULL)
			return;
		memcpy(data, (p + buf->head), (buf->sizebuff - buf->head));
		memcpy((data + buf->sizebuff - buf->head), p, buf->tail);
		buf->head = 0;
		buf->tail = 0;
	}
  else
	{
		data = (uint8_t*)malloc(sizeof(uint8_t) * (buf->tail - buf->head));
		if(data == NULL)
			return;
		memcpy(data, (p + buf->head), (buf->tail - buf->head));
	}
}