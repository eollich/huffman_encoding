#include "btree.h"
#include "pqueue.h"
#include <huffman.h>
#include <stdlib.h>


char* huffmanGetFrequencyTableStr(Hashtable* ht) {
  // Initial buffer allocation size
  size_t buffer_size = 256;
  char *temp_buffer = (char *)malloc(buffer_size);
  if (temp_buffer == NULL) {
    perror("Failed to allocate memory");
    exit(EXIT_FAILURE);
  }
  temp_buffer[0] = '\0'; 

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

      size_t new_size = strlen(temp_buffer) + entry_size + 1;
      if (new_size > buffer_size) {
        buffer_size = new_size * 2;
        temp_buffer = (char *)realloc(temp_buffer, buffer_size);
        if (temp_buffer == NULL) {
          perror("Failed to allocate memory");
          exit(EXIT_FAILURE);
        }
      }

      strcat(temp_buffer, entry);

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

PQueue* huffmanGeneratePriorityQueue(Hashtable* hashtable){
  if(!hashtable || hashtable->size < 1) return NULL;

  HashtableNode* cur;
  PQueue* pq = pQueueInit(NULL);
  for(size_t i=0; i<hashtable->num_buckets; i++){
    cur = hashtable->buckets[i];
    while(cur){
      BTreeNode* node = bTreeNodeInit(*(size_t*)cur->value, cur->key, NULL, NULL, NULL);
      pQueueInsert(pq, node, *(size_t*)cur->value);
      cur = cur->next;
    }
  }
  return pq;
}

void huffmanPrintPQ(PQueue* pqueue){
  if(!pqueue || !pqueue->root) return;
  PQueueNode* cur = pqueue->root;
  while(cur){
    BTreeNode* btn_temp = cur->data;
    printf("%zu: %s\n", cur->priority, (char*)(btn_temp->data));
    cur = cur->next;
  }
}

//BTreeNode* huffmanBuildTree(PQueue* pqueue) {
//  huffmanPrintPQ(pqueue);
//  //while (pqueue->num_nodes > 1) {
//  //  PQueueNode* leftNode = pQueuePop(pqueue);
//  //  PQueueNode* rightNode = pQueuePop(pqueue);
//
//  //  size_t combinedWeight = leftNode->priority + rightNode->priority;
//  //  BTreeNode* internalNode = bTreeNodeInit(combinedWeight, NULL, NULL, leftNode->data, rightNode->data);
//
//  //  pQueueInsert(pqueue, internalNode, combinedWeight);
//  //  //pQueueNodeDelete(leftNode);
//  //  //pQueueNodeDelete(rightNode);
//  //}
//
//  //PQueueNode* rootNode = pQueuePop(pqueue);
//  //BTreeNode* root = (BTreeNode*)rootNode->data;
//  ////pQueueNodeDelete(rootNode);
//
//  //return root;
//
//  while(pqueue->num_nodes > 1){
//    PQueueNode* left_node = pQueuePop(pqueue);
//    PQueueNode* right_node = pQueuePop(pqueue);
//    BTreeNode* left_b_node = left_node->data;
//    BTreeNode* right_b_node = right_node->data;
//
//    size_t combined_weight = left_node->priority + right_node->priority;
//
//    BTreeNode* parent = bTreeNodeInit(combined_weight, NULL, NULL, left_b_node, right_b_node);
//    left_b_node->parent = parent;
//    right_b_node->parent = parent;
//
//    pQueueInsert(pqueue, parent, combined_weight);
//
//    free(left_node);
//    free(right_node);
//
//
//  }
//
//
//  BTreeNode* root = (BTreeNode*)pQueuePop(pqueue)->data;
//
//
//
//  return root;
//
//}

BTreeNode* huffmanBuildTree(PQueue* pqueue) {
  while (pqueue->num_nodes > 1) {
    PQueueNode* left_node = pQueuePop(pqueue);
    PQueueNode* right_node = pQueuePop(pqueue);

    BTreeNode* left_b_node = (BTreeNode*)left_node->data;
    BTreeNode* right_b_node = (BTreeNode*)right_node->data;

    size_t combined_weight = left_node->priority + right_node->priority;

    BTreeNode* parent = bTreeNodeInit(combined_weight, NULL, NULL, left_b_node, right_b_node);
    if (!parent) {
      printf("Failed to allocate parent node\n");
      return NULL;
    }

    left_b_node->parent = parent;
    right_b_node->parent = parent;

    pQueueInsert(pqueue, parent, combined_weight);

    free(left_node);
    free(right_node);
  }

  PQueueNode* rootNode = pQueuePop(pqueue);
  BTreeNode* root = (BTreeNode*)rootNode->data;
  free(rootNode);

  return root;
}


void huffmanGenerateCodes(BTreeNode* root, Hashtable* codesTable, char* code, int depth) {
  //printf("DEPTH: %d", depth);
  if (!root->left && !root->right) {
    char* finalCode = (char*)malloc(depth + 1);
    strncpy(finalCode, code, depth);
    finalCode[depth] = '\0';
    hashtableInsert(codesTable, root->data, finalCode);
    return;
  }

  if (root->left) {
    code[depth] = '0';
    huffmanGenerateCodes(root->left, codesTable, code, depth + 1);
  }

  if (root->right) {
    code[depth] = '1';
    huffmanGenerateCodes(root->right, codesTable, code, depth + 1);
  }
}

Hashtable* huffmanGenerateCodesTable(BTreeNode* root) {
  Hashtable* codesTable = hashtableInit(NULL, NULL);
  char code[256]; //max depth of 256 
  huffmanGenerateCodes(root, codesTable, code, 0);
  return codesTable;
}

void huffmanPrintCodes(Hashtable* codesTable) {
  if (!codesTable) {
    printf("Codes table is NULL\n");
    return;
  }

  for (size_t i = 0; i < codesTable->num_buckets; ++i) {
    HashtableNode* node = codesTable->buckets[i];
    while (node) {
      char* character = (char*)node->key;
      char* code = (char*)node->value;
      printf("%c: %s\n", character[0], code);
      node = node->next;
    }
  }
}

void huffmanPrintTree(BTreeNode* node, int depth) {
  if (node == NULL) return;

  for (int i = 0; i < depth; ++i) printf("  ");
  if (node->data) {
    printf("%c: %zu\n", ((char*)node->data)[0], node->weight);
  } else {
    printf("Node: %zu\n", node->weight);
  }

  huffmanPrintTree(node->left, depth + 1);
  huffmanPrintTree(node->right, depth + 1);
}
