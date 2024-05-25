//Frequency Table   Hashmap
//Priority Queue    Priority Queue
//Tree              Tree
//Tree Node         Node
//Code Table        Hashmap
//Encoded Data      bit stream / array of bits

#include <hashtable.h>
#include <btree.h>

int main(void){


  int *a = malloc(sizeof(int));
  int *b = malloc(sizeof(int));
  int *c = malloc(sizeof(int));
  int *d = malloc(sizeof(int));
  int *e = malloc(sizeof(int));

  if (!a || !b || !c || !d || !e) {
    fprintf(stderr, "Memory allocation error\n");
    return 1;
  }

  *a = 10;
  *b = 11;
  *c = 12;
  *d = 13;
  *e = 14;


  BTree* btree = bTreeInit(NULL, NULL);
  bTreeInsert(btree, 6, e);
  bTreeInsert(btree, 3, b);
  bTreeInsert(btree, 4, c);
  bTreeInsert(btree, 5, d);
  bTreeInsert(btree, 2, a);
  //BTreeNode* y = bTreeSearch(btree, x);
  BTreeNode* x = bTreeFindLastNode(btree);
  printf("%zu\n", x->weight);
  bTreeDelete(btree);

  return 0;
}
