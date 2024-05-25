//refactor to include hashnode functions



#ifndef __HASHTABLE_H__
#define __HASHTABLE_H__

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define HASHTABLE_INTIAL_SIZE (1 << 4)
#define LOAD_FACTOR_THRESHOLD 0.75

typedef struct HashtableNode{
  void* key;
  void* value;
  struct HashtableNode* next;
} HashtableNode;

typedef struct{
  HashtableNode** buckets;
  size_t num_buckets;
  size_t size;
  size_t (*hashFunction)(const char*);
  void (*freeFunction)(void*);

} Hashtable;


Hashtable* hashtableInit(size_t (*hashFunction)(const char*), void (*freeFunction)(void*));

void hashtableResize(Hashtable* hashtable);

void hashtableInsert(Hashtable* hashtable, const void* key, const void* value);

void* hashtableLookup(const Hashtable* hashtable, const void* key);

void hashtableDeleteEntry(Hashtable* hashtable, const char* key);

void hashtableDelete(Hashtable* hashtable);

void hashtableDefaultFreeFunction(void* value);

size_t hashtableDefaultHashFunction(const char* key); //fnv1a

#endif
