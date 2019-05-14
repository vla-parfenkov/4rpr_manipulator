#include "queue.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h> 

uint32_t size = 0;
struct QueueCommand *head = NULL;
struct QueueCommand *tail = NULL;


void pushQueueCmd(char *cmd)
{
	struct QueueCommand *newCmd = tail;
  tail = (struct QueueCommand*)malloc(sizeof(struct QueueCommand));
	tail->cmd = cmd;
	tail->next = NULL;
  if (size == 0)
    head = tail;
  else 
    newCmd->next = tail;
  size++;
}

char *popQueueCmd(void)
{
	struct QueueCommand *cmd;
	char *res;
  cmd = head;
	if(!cmd)
		return NULL;
	size--;
  head = head->next;
	res = cmd->cmd;
	free(cmd);
  return res;
}

bool isEmptyQueueCmd (void)
{
	return (size == 0);
}

