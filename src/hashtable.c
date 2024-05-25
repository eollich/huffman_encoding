#include <hashtable.h>
#include <stdlib.h>

Hashtable* hashtableInit(size_t (*hashFunction)(const char*), void (*freeFunction)(void*)){
  Hashtable* hashtable = malloc(sizeof(Hashtable));
  if(!hashtable){
    fprintf(stderr, "hashtableInit malloc error\n");
    exit(EXIT_FAILURE);
  }

  hashtable->buckets = calloc(HASHTABLE_INTIAL_SIZE, sizeof(HashtableNode*));
  if(!hashtable->buckets){
    fprintf(stderr, "hashtableInit calloc buckets error\n");
    exit(EXIT_FAILURE);
  }

  hashtable->size = 0;
  hashtable->num_buckets = HASHTABLE_INTIAL_SIZE;
  hashtable->hashFunction = hashFunction ? hashFunction: hashtableDefaultHashFunction;
  hashtable->freeFunction = freeFunction ? freeFunction : hashtableDefaultFreeFunction;

  return hashtable;
}

void hashtableResize(Hashtable* hashtable){
  if(!hashtable) return;

  size_t old_size = hashtable->num_buckets;
  size_t new_size = old_size << 1;
  HashtableNode** new_buckets = calloc(new_size, sizeof(HashtableNode*));
  if(!new_buckets){
    fprintf(stderr, "hashtableResize calloc error\n");
    exit(EXIT_FAILURE);
  }

  for(size_t i = 0; i<old_size; i++){
    HashtableNode* cur = hashtable->buckets[i];
    while(cur){ 
      size_t new_index = hashtable->hashFunction(cur->key) & (new_size - 1); 
      HashtableNode* temp = cur;  
      cur = cur->next; 
      temp->next = new_buckets[new_index]; 
      new_buckets[new_index] = temp; 
    }
  }

  free(hashtable->buckets);
  hashtable->buckets = new_buckets;
  hashtable->num_buckets = new_size;

}

void hashtableInsert(Hashtable* hashtable, const void* key, const void* value){
  if(!hashtable || !key) return;

  float load_factor = (float)hashtable->size / hashtable->num_buckets;
  if(load_factor > LOAD_FACTOR_THRESHOLD){
    hashtableResize(hashtable);
  }

  size_t index = hashtable->hashFunction(key) & (hashtable->num_buckets - 1);
  HashtableNode* node = hashtable->buckets[index];
  while(node){
    if(strcmp(node->key, key) == 0){
      hashtable->freeFunction(node);
      node->value = (void*)value;
      return;
    }
    node = node->next;
  }

  HashtableNode* new_node = malloc(sizeof(HashtableNode));
  if(!new_node){
    fprintf(stderr, "hashtableInsert malloc error\n");
    exit(EXIT_FAILURE);
  }

  new_node->key = strdup(key);
  if(!new_node->key){
    free(new_node);
    hashtableDelete(hashtable);
    fprintf(stderr, "hashtableInsert strdup key error\n");
    exit(EXIT_FAILURE);
  }

  new_node -> value = (void*)value;
  new_node -> next = hashtable->buckets[index];
  hashtable->buckets[index] = new_node;
  hashtable->size++;
}


void* hashtableLookup(const Hashtable* hashtable, const void* key){
  if(!hashtable || !key) return NULL;

  size_t index = hashtable->hashFunction(key) & (hashtable->num_buckets-1);
  HashtableNode* node = hashtable->buckets[index];

  while(node!=NULL){
    if(strcmp(node->key, key) == 0){
      return node->value;
    }
    node = node->next;
  }
  return NULL;
}

void hashtableDeleteEntry(Hashtable* hashtable, const char* key){
  if(!hashtable || !key) return;

  size_t index = hashtable->hashFunction(key) & (hashtable->num_buckets- 1);
  HashtableNode** cur = &(hashtable->buckets[index]);

  while(*cur){
    HashtableNode* entry = *cur;
    if(strcmp(entry->key, key) == 0){
      *cur = entry->next;
      free(entry->key);
      hashtable->freeFunction(entry->value);
      free(entry);
      hashtable->size--;
      return;
    }
    cur = &((*cur)->next);
  }
}

void hashtableDelete(Hashtable* hashtable){
  if(!hashtable) return;

  for(size_t i=0; i<hashtable->num_buckets; i++){
    HashtableNode* cur = hashtable->buckets[i];
    while(cur){
      HashtableNode* temp = cur;
      cur = cur->next;
      free(temp->key);
      hashtable->freeFunction(temp->value);
      free(temp);
      hashtable->size--;
    }
    hashtable->buckets[i] = NULL;
  }
  free(hashtable->buckets);
  free(hashtable);
}

void hashtableDefaultFreeFunction(void* value){
  free(value);
}

size_t hashtableDefaultHashFunction(const char* key){
  size_t total = 0;
  for(size_t i = 0; key[i] != 0; i++){
    total+= (int)key[i];
    total*=101;
  }
  return total;
}
