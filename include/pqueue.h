#ifndef __PQUEUE_H__
#define __PQUEUE_H__

#include <stdlib.h>
#include <stdio.h>

typedef struct PQueueNode{
  size_t priority;
  void* data;
  struct PQueueNode* next;
} PQueueNode;

typedef struct{
  size_t num_nodes;
  int (*pQueueCompareFunction)(void*, void*);
  PQueueNode* root;
} PQueue;

PQueue* pQueueInit(int(*pQueueCompareFunction)(void*, void*) );

void pQueueInsert(PQueue* pqueue, void* data, size_t priority);

void* pQueueSearch(PQueue* pqueue, void* data);

PQueueNode* pQueuePop(PQueue* pqueue);

PQueueNode* pQueueRemove(PQueue* pqueue, void* data);

void pQueueDelete(PQueue* pqueue, int data_delete);



PQueueNode* pQueueNodeInit(void* data, size_t prioirty);

void pQueueNodeDelete(PQueueNode* pqueue_node);


int pQueueDefaultCompareFunction(void*, void*);

#endif
