#ifndef __HUFFMAN_H__
#define __HUFFMAN_H__

#include <hashtable.h>
#include <pqueue.h>
#include <btree.h>

Hashtable* huffmanGenerateFrequencyTable(const char* str);
PQueue* huffmanGeneratePriorityQueue(Hashtable* hashtable);
BTreeNode* huffmanBuildTree(PQueue* pqueue);
Hashtable* huffmanGenerateCodesTable(BTreeNode* root);

char* huffmanGetFrequencyTableStr(Hashtable* hashtable);

void huffmanPrintPQ(PQueue* pqueue);
void huffmanPrintCodes(Hashtable* codesTable);
void huffmanPrintTree(BTreeNode* node, int depth);
#endif
