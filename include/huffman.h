#ifndef __HUFFMAN_H__
#define __HUFFMAN_H__

#include <hashtable.h>

Hashtable* huffmanGenerateFrequencyTable(const char* str);
char* huffmanGetFrequencyTableStr(Hashtable* hashtable);

#endif
