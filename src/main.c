//Frequency Table   Hashmap
//Priority Queue    Priority Queue
//Tree              Tree
//Tree Node         Node
//Code Table        Hashmap
//Encoded Data      bit stream / array of bits

#include <hashtable.h>

#include <pqueue.h>


int main(void){


  //int* x = malloc(sizeof(int));
  //*x = 3;

  //Hashtable* ht = hashtableInit(NULL, NULL);
  //hashtableInsert(ht, "test", x);
  //void* val = hashtableLookup(ht, "test");
  //if(!val) return 0;
  //printf("val: %d\n", *(int*)val);
  //hashtableDelete(ht);


  int *a = malloc(sizeof(int));
  int *b = malloc(sizeof(int));
  int *c = malloc(sizeof(int));
  int *d = malloc(sizeof(int));
  *a = 11;
  *b = 12;
  *c = 13;
  *d = 14;
  PQueue* pq = pQueueInit(NULL);
  pQueueInsert(pq, a, 4);
  pQueueInsert(pq, b, 24);
  pQueueInsert(pq, c, 1);
  pQueueInsert(pq, d, 3);

  PQueueNode* pqn = pQueuePop(pq);
  pqn = pQueuePop(pq);
  printf("%d\n", *(int*)pqn->data);
  pQueueNodeDelete(pqn);

  pQueueDelete(pq, 1);



  return 0;
}
