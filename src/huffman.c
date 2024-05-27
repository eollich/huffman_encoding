#include "hashtable.h"
#include <huffman.h>


//maybe serialize the hufftree instead
char* huffmanGetFrequencyTableStr(Hashtable* ht) {
  // Initial buffer allocation size
  size_t buffer_size = 256;
  char *temp_buffer = (char *)malloc(buffer_size);
  if (temp_buffer == NULL) {
    perror("Failed to allocate memory");
    exit(EXIT_FAILURE);
  }
  temp_buffer[0] = '\0'; // Initialize with an empty string

  HashtableNode* cur;
  for (size_t i = 0; i < ht->num_buckets; i++) {
    cur = ht->buckets[i];
    while (cur) {
      size_t entry_size = snprintf(NULL, 0, "%s:%zu:", (char*)cur->key, *(size_t*)cur->value) + 1;
      char *entry = (char *)malloc(entry_size);
      if (entry == NULL) {
        perror("Failed to allocate memory");
        exit(EXIT_FAILURE);
      }
      snprintf(entry, entry_size, "%s:%zu:", (char*)cur->key, *(size_t*)cur->value);

      // Check if we need to expand the buffer
      size_t new_size = strlen(temp_buffer) + entry_size + 1;
      if (new_size > buffer_size) {
        buffer_size = new_size * 2;
        temp_buffer = (char *)realloc(temp_buffer, buffer_size);
        if (temp_buffer == NULL) {
          perror("Failed to allocate memory");
          exit(EXIT_FAILURE);
        }
      }

      // Concatenate the new entry to the temp_buffer
      strcat(temp_buffer, entry);

      // Free the entry memory
      free(entry);

      cur = cur->next;
    }
  }

  return temp_buffer;
}



void printFreq(Hashtable* ht){
  HashtableNode* cur;
  for(size_t i = 0; i<ht->num_buckets; i++){
    cur = ht->buckets[i];
    while(cur){
      printf("Bucket: %zu -- %s: %d\n", i, (char*)cur->key, *(int*)(cur->value));
      cur = cur->next;
    }
  }
}


//switch key for either priority queue node or tree node
Hashtable* huffmanGenerateFrequencyTable(const char* str){
  if(!str) return NULL;

  Hashtable* hashtable = hashtableInit(NULL, NULL);

  int index = 0;
  while(str[index]!='\0'){
    char key[] = {str[index], '\0'};
    void* val = hashtableLookup(hashtable, &key);
    if(!val){
      int* value = malloc(sizeof(int));
      *value = 1;
      hashtableInsert(hashtable, &key, value);
    }else{
      (*(int*)val)++;
    }

    index++;
  }

  return hashtable;

}
