//Frequency Table   Hashmap
//Priority Queue    Priority Queue
//Tree              Tree
//Tree Node         Node
//Code Table        Hashmap
//Encoded Data      bit stream / array of bits

#include <hashtable.h>

int main(void){


  int* x = malloc(sizeof(int));
  *x = 3;

  Hashtable* ht = hashtableInit(NULL, NULL);
  hashtableInsert(ht, "test", x);
  void* val = hashtableLookup(ht, "test");
  if(!val) return 0;
  printf("val: %d\n", *(int*)val);
  hashtableDelete(ht);
  return 0;
}
