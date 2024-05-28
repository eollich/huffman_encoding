#include <pqueue.h>
#include <stdlib.h>

PQueue* pQueueInit(int(*pQueueCompareFunction)(void*, void*) ){
  PQueue* pqueue = malloc(sizeof(PQueue));
  if(!pqueue){
    fprintf(stderr, "pQueueInit malloc error\n");
    exit(EXIT_FAILURE);
  }

  pqueue->pQueueCompareFunction = pQueueCompareFunction ? pQueueCompareFunction : pQueueDefaultCompareFunction;
  pqueue->num_nodes = 0;
  pqueue->root = NULL;

  return pqueue;
}

void pQueueInsert(PQueue* pqueue, void* data, size_t priority){
  if(!pqueue) return;

  PQueueNode* node = pQueueNodeInit(data, priority);

  if(!pqueue->root || priority < pqueue->root->priority){
    node->next = pqueue->root;
    pqueue->root = node;
  }else{

    PQueueNode* cur = pqueue->root;
    PQueueNode* prev = NULL;
    while(cur && priority > cur->priority){
      prev = cur;
      cur = cur->next;
    }

    if(prev){
      prev->next = node;
    }else{
      pqueue->root = node;
    }

    node->next = cur;
  }
  pqueue->num_nodes++;

}

void* pQueueSearch(PQueue* pqueue, void* data){
  if(!pqueue) return NULL;

  PQueueNode* cur = pqueue->root;
  while(cur){
    if(pqueue->pQueueCompareFunction(data, cur->data) == 0) return cur->data;
    cur = cur->next;
  }

  return NULL;
}

PQueueNode* pQueuePop(PQueue* pqueue){
  if(!pqueue || pqueue->num_nodes==0) return NULL;
  PQueueNode* node = pqueue->root;
  pqueue->root = node->next;
  pqueue->num_nodes--;
  return node; 
}

PQueueNode* pQueueRemove(PQueue* pqueue, void* data){
  if(!pqueue) return NULL;

  PQueueNode* prev = NULL;
  PQueueNode* cur = pqueue->root;
  while(cur){
    if(pqueue->pQueueCompareFunction(data, cur->data) == 0){
      if(prev) {
        prev->next = cur->next;
      } else {
        pqueue->root = cur->next;
      }
      free(cur);
      return cur;
    }
    prev = cur;
    cur = cur->next;
  }

  return NULL;
}

void pQueueDelete(PQueue* pqueue, int data_delete){
  if(! pqueue) return;
  PQueueNode* temp = NULL;
  PQueueNode* cur = pqueue->root;
  while(cur){
    temp = cur ->next;
    if(data_delete){
      pQueueNodeDelete(cur);
    }else{
      free(cur);
    }
    cur = temp;
  }
}



PQueueNode* pQueueNodeInit(void* data, size_t prioirty){
  PQueueNode* pqueue_node = malloc(sizeof(PQueueNode));
  if(!pqueue_node){
    fprintf(stderr, "pQueueNodeInit malloc error\n");
    exit(EXIT_FAILURE);
  }

  pqueue_node->data = data;
  pqueue_node->priority = prioirty;
  pqueue_node->next = NULL;

  return pqueue_node;
}

void pQueueNodeDelete(PQueueNode* pqueue_node){
  free(pqueue_node->data);
  free(pqueue_node);
}



int pQueueDefaultCompareFunction(void* a, void* b){
  int sa = *(int*)a;
  int sb = *(int*)b;
  if (sa < sb) {
    return -1; 
  } else if (sa > sb) {
    return 1;
  } else {
    return 0;
  }
}
