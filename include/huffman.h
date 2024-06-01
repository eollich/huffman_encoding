#ifndef __HUFFMAN_H__
#define __HUFFMAN_H__

#include <stdlib.h>

typedef struct HTNode{
  char val;
  struct HTNode* left;
  struct HTNode* right;
} HTNode;

typedef struct PQNode{
  int priority;
  HTNode* data;
  struct PQNode* next;
} PQNode;

typedef struct {
  int num_nodes;
  PQNode* root;
} PQ;


char* huffmanGenerateCodes(char* str);
void huffmanPrintCodes(char* codes);

size_t* huffmanGenFT(char* str);
PQ* huffmanGenPQ(size_t* ft);
HTNode* huffmanGenHT(PQ* pq);
char* huffmanGenCodes(HTNode* ht_root);


void printFrequencyTable(size_t* frequency_table);

#endif
