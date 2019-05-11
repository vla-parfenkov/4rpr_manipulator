/****************************************/
/*			Очередь			*/
/****************************************/

#ifndef QUEUE_H
#define QUEUE_H


#include "main.h"
#include <stdbool.h>

struct QueueCommand {
	char *cmd;
	struct QueueCommand *next;
};


void pushQueueCmd(char *cmd);
char *popQueueCmd(void);
bool isEmptyQueueCmd (void);

#endif
