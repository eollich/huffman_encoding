#ifndef __HUFFMAN_FREQUENCY_TABLE_H__
#define __HUFFMAN_FREQUENCY_TABLE_H__

#include <stdlib.h>

#define HASHTALBE_INTIAL_SIZE (1 << 4)
#define LOAD_FACTOR_THRESHOLD 0.75

typedef struct HashtableNode{
  void* key;
  void* value;
  struct HashtableNode* next;
} HashtableNode;

typedef struct{
  size_t num_buckets;
  size_t size;
  size_t (*hashfunction)(void*);
  void (*freefunction)(void*);

} Hashtable;


Hashtable* hashtableInit(const size_t (*hash_function)(void*), const void (*free_function(void*)));

void hashtableResize(Hashtable* hashtable);

void hashtableInsert(Hashtable* hashtable, const void* key, const void* value);

void* hashtableLookup(const Hashtable* hashtable, const void* key);

void hashtableDeleteEntry(Hashtable* hashtable, const char* key);

void hashtableDelete(Hashtable* hashtable);

void hashtableDefaultFreeFunction(HashtableNode* node);

size_t hashtableDefaultHashFunction(const char* key);

#endif
