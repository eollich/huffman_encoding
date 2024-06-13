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

typedef struct{
  unsigned char* code;
  int num_bytes;
  int leftover_bits;
} PackedCode;

typedef struct{
  char* encoded_tree;
  PackedCode** code_table;
  int* code_lens;
} Huffman;



Huffman* huffmanGenerateCodes(char* str);
void huffmanPrintCodes(Huffman* huff);

size_t* huffmanGenFT(char* str);
PQ* huffmanGenPQ(size_t* ft);
HTNode* huffmanGenHT(PQ* pq);
Huffman* huffmanGenCodes(HTNode* ht_root);


void printFrequencyTable(size_t* frequency_table);

void addPackedCode(PackedCode* pc1, PackedCode* pc2);
void printPackedCode(PackedCode* pc);

char* huffmanEncodeTree(HTNode* root);
int calculateEncodedTreeLength(HTNode* node);
void printEncodedTree(char* encoded_tree, int length);

#endif
