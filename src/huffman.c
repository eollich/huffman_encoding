#include <huffman.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


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


void huffmanGenCodesHelper(Huffman* huff, HTNode* node, unsigned char code, int depth){
  if(!node->left && !node->right){
    code <<= (8-depth);
    huff->code_table[(int)node->val] = code;
    huff->code_lens[(int)node->val] = depth;
    return;
  }

  if(node->left){
    huffmanGenCodesHelper(huff, node->left, code<<1 | 0, depth+1);
  }

  if(node->right){
    huffmanGenCodesHelper(huff, node->right, code<<1 | 1, depth+1);
  }
}

Huffman* huffmanGenCodes(HTNode* ht_root){
  Huffman* huffman = malloc(sizeof(Huffman));
  if(!huffman){
    fprintf(stderr, "Malloc error creating huffman object in huffmanGenCodes\n");
    exit(EXIT_FAILURE);
  }
  unsigned char* code_table = malloc(256 * sizeof(char));
  int* code_lens = malloc(256 * sizeof(char));
  if(!code_table || !code_lens){
    fprintf(stderr, "Malloc error creating code table in huffmanGenCodes\n");
    exit(EXIT_FAILURE);
  }
  memset(code_table, 0, 256*sizeof(char));
  memset(code_lens, -1, 256*sizeof(char));
  huffman->encoded_tree=NULL;
  huffman->code_table = code_table;
  huffman->code_lens = code_lens;

  huffmanGenCodesHelper(huffman, ht_root, 0, 0);


  return huffman;
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


void toBinary(char c, int num_bits){
  for(int i=7; i>=8-num_bits; i--){
    printf("%d", (c >> i) & 1);
  }
}

void huffmanPrintCodes(Huffman* huff){
  for(int i=0; i<256; i++){
    unsigned char* codes = huff->code_table;
    if(huff->code_lens[i] > 0){
      printf("%c: char(%-3c)\tint(%-3d)\tbin(", i, codes[i], codes[i]);
      toBinary(codes[i], huff->code_lens[i]);
      printf(")\n");
    }
  }
}

Huffman* huffmanGenerateCodes(char* str){

  size_t* ft = huffmanGenFT(str);
  PQ* pq = huffmanGenPQ(ft);
  HTNode* ht = huffmanGenHT(pq);
  Huffman* huffman = huffmanGenCodes(ht);
  free(ft);
  free(ht);
  free(pq);

  return huffman;
  
}

