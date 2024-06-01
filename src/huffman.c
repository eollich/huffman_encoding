#include <huffman.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//frequency table DONE
//pq
//huffman tree
//code table
//code len table?

void pQInsert(PQ* pqueue, HTNode* data, size_t priority) {
  PQNode* new_node = (PQNode*)malloc(sizeof(PQNode));
  if(!new_node){
    fprintf(stderr, "Malloc error when trying to malloc PQNode struct in huffmanGenPQ\n");
    exit(EXIT_FAILURE);
  }
  new_node->priority = priority;
  new_node->data = data;
  new_node->next = NULL;

  if (!pqueue->root || priority < pqueue->root->priority) {
    new_node->next = pqueue->root;
    pqueue->root = new_node;
  } else {
    PQNode* current = pqueue->root;
    while (current->next && current->next->priority <= priority) {
      current = current->next;
    }
    new_node->next = current->next;
    current->next = new_node;
  }
  pqueue->num_nodes++;
}


PQNode* pqPop(PQ* pq) {
    if (!pq || !pq->root) return NULL;
    PQNode* temp = pq->root;
    pq->root = pq->root->next;
    temp->next = NULL;
  pq->num_nodes--;
    return temp;
}



size_t* huffmanGenFT(char* str){
  size_t* frequency = calloc(256, sizeof(size_t));
  if(!frequency){
    fprintf(stderr, "Calloc error when creating huffman frequency table in huffmanGenFT\n");
    exit(EXIT_FAILURE);
  }
  while(*str!='\0'){
    frequency[(size_t)*str]++;
    str++;
  }
  return frequency;
}

PQ* huffmanGenPQ(size_t* ft){
  PQ* pq = malloc(sizeof(PQ));
  if(!pq){
    fprintf(stderr, "Malloc error when trying to malloc PQ struct in huffmanGenPQ\n");
    exit(EXIT_FAILURE);
  }

  for(int i=0; i<256; i++){
    if(ft[i]!=0){
      PQNode* pqn = malloc(sizeof(PQNode));
      if(!pqn){
        fprintf(stderr, "Malloc error when trying to malloc PQNode struct in huffmanGenPQ\n");
        exit(EXIT_FAILURE);
      }
      HTNode* htn = malloc(sizeof(HTNode));
      if(!htn){
        fprintf(stderr, "Malloc error when trying to malloc HTNode struct in huffmanGenPQ\n");
        exit(EXIT_FAILURE);
      }
      htn->val = i;
      htn->left = NULL;
      htn->right = NULL;
      pQInsert(pq, htn, ft[i]);
    }
  }
  return pq;
}

HTNode* huffmanGenHT(PQ* pq);
char* huffmanGenCodes(HTNode* ht_root);


HTNode* huffmanGenHT(PQ* pq){
  PQNode* left;
  PQNode* right;
  HTNode* parent;

  HTNode* root = NULL;

  while(pq->num_nodes >1){
    left = pqPop(pq);
    right = pqPop(pq);
    parent = malloc(sizeof(HTNode));
    parent->left = left->data;
    parent->right = right->data;

    pQInsert(pq, parent, left->priority + right->priority);
    free(left);
    free(right);
  }
  left = pqPop(pq);
  root = left->data;
  free(left);
  return root;
}


void huffmanGenCodesHelper(char* codes, HTNode* node, char code, int depth){
  if(!node->left && !node->right){
    codes[node->val] = code;
    return;
  }

  if(node->left){
    huffmanGenCodesHelper(codes, node->left, code<<1 | 0, depth+1);
  }

  if(node->right){
    huffmanGenCodesHelper(codes, node->right, code<<1 | 1, depth+1);
  }
}

char* huffmanGenCodes(HTNode* ht_root){
  char* codes = malloc(256 * sizeof(char));
  memset(codes, -1, 256*sizeof(char));

  huffmanGenCodesHelper(codes, ht_root, 0, 0);


  return codes;
}


void printFrequencyTable(size_t* frequency_table){
  for(int i=0; i<256; i++){
    if(frequency_table[i]!=0){
      printf("%c: %zu\n", i, frequency_table[i]);
    }
  }
}

void printPriorityQueue(PQ* pq){
  PQNode* temp = pq->root;
  for(int i=0; i<pq->num_nodes; i++){
    printf("%c %d\n", temp->data->val, temp->priority);
    temp = temp->next;
  }
}


void toBinary(char c){
  for(int i=7; i>=0; i--){
    printf("%d", (c >> i) & 1);
  }
}

void huffmanPrintCodes(char* codes){
  for(int i=0; i<256; i++){
    if(codes[i] >= 0){
      printf("%c: char(%c)\tint(%d)\tbin(", i, codes[i], codes[i]);
      toBinary(codes[i]);
      printf(")\n");
    }
  }
}

char* huffmanGenerateCodes(char* str){

  //frequency table

  size_t* ft = huffmanGenFT(str);
  //printFrequencyTable(frequency);
  PQ* pq = huffmanGenPQ(ft);
  //printPriorityQueue(pq);
  HTNode* ht = huffmanGenHT(pq);
  //HTNode* ht_t = ht;
  //printf("root\n");
  //printf("%c : L%p R%p\n", ht_t->val, ht_t->left, ht_t->right);

  //printf("root->left\n");
  //printf("%c : L%p R%p\n", ht_t->left->val, ht_t->left->left, ht_t->left->right);

  //printf("root->right\n");
  //printf("%c : L%p R%p\n", ht_t->right->val, ht_t->right->left, ht_t->right->right);

  //printf("root->right->left\n");
  //printf("%c : L%p R%p\n", ht_t->right->left->val, ht_t->right->left->left, ht_t->right->left->right);

  //printf("root->right->right\n");
  //printf("%c : L%p R%p\n", ht_t->right->right->val, ht_t->right->right->left, ht_t->right->right->right);

  //printf("root->right->right->left\n");
  //printf("%c : L%p R%p\n", ht_t->right->right->left->val, ht_t->right->right->left->left, ht_t->right->right->left->right);

  //printf("root->right->right->right\n");
  //printf("%c : L%p R%p\n", ht_t->right->right->right->val, ht_t->right->right->right->left, ht_t->right->right->right->right);
  char* codes = huffmanGenCodes(ht);
  free(ft);
  free(ht);
  free(pq);

  return codes;
  
}

