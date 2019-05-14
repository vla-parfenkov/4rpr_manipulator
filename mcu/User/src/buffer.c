
#include "buffer.h"
#include <stdlib.h>
#include <string.h> 

void initBuffer(struct Buffer *buf)
{
	if(buf == NULL)
		return;
	buf->head = 0;
	buf->tail = 0;
	buf->sizebuff = 1024; 
	buf->data[0] = 0;
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

void pushBuffer(char bait, struct Buffer *buf)
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

char popBuffer(struct Buffer *buf, bool errorFlag)
{
	char res = 0;
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

char showBuffer(struct Buffer *buf, bool empty)
{
	char res = 0;
	if (isEmptyBuffer(buf))
	{		
		empty = true;
    return res;
	}
  res = buf->data[buf->head];
	empty = false;
  return res;
}

void readDataFromBuffer(struct Buffer *buf, char **data)
{
	char *p = buf->data;
	if(!buf)
		return;
	
	if(isEmptyBuffer(buf))
		return;
	
	if (buf->head > buf->tail)
	{
		*data = (char *)malloc(sizeof(char) * (buf->sizebuff - buf->head + buf->tail));
    if(*data == NULL)
			return;
		memcpy(*data, (p + buf->head), (buf->sizebuff - buf->head));
		memcpy((*data + buf->sizebuff - buf->head), p, buf->tail);
		buf->head = 0;
		buf->tail = 0;
	}
  else
	{
		*data = (char*)malloc(sizeof(char) * (buf->tail - buf->head));
		if(*data == NULL)
			return;
		memcpy(*data, (p + buf->head), (buf->tail - buf->head));
		buf->head = 0;
		buf->tail = 0;
	}
}