#include "queue.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h> 

uint32_t size = 0;
struct QueueCommand *head = NULL;
struct QueueCommand *tail = NULL;
struct QueueCommand *initCmd(char *cmd, struct QueueCommand *next)
{
	struct QueueCommand *newCmd = (struct QueueCommand*)malloc(sizeof(struct QueueCommand));
	if(!newCmd)
		return NULL;
	
	newCmd->cmd = cmd;
	newCmd->next = next;
}

void pushQueueCmd(char *cmd)
{
	struct QueueCommand *newCmd = tail;
  tail = initCmd(cmd, NULL);
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
	size--;
  cmd = head;
	if(!cmd)
		return NULL;
  head = head->next;
	res = cmd->cmd;
	free(cmd);
  return res;
}

bool isEmptyQueueCmd (void)
{
	return (size == 0);
}

