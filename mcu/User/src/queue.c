#include "queue.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h> 

uint32_t size = 0;
struct QueueCommand *headCmd = NULL;
struct QueueCommand *tailCmd = NULL;


void pushQueueCmd(char *cmd)
{
	struct QueueCommand *newCmd = tailCmd;
  tailCmd = (struct QueueCommand*)malloc(sizeof(struct QueueCommand));
	tailCmd->cmd = cmd;
	tailCmd->next = NULL;
  if (size == 0)
    headCmd = tailCmd;
  else 
    newCmd->next = tailCmd;
  size++;
}

char *popQueueCmd(void)
{
	struct QueueCommand *cmd;
	char *res;
  cmd = headCmd;
	if(!cmd)
		return NULL;
	size--;
  headCmd = headCmd->next;
	res = cmd->cmd;
	free(cmd);
  return res;
}

bool isEmptyQueueCmd (void)
{
	return (size == 0);
}

