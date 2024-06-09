#include <huffman.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void toBinary(unsigned char* code, int num_bits) {
  for (int i = 0; i < num_bits; i++) {
    printf("%d", (code[i / 8] >> (7 - (i % 8))) & 1);
  }
}

void pQInsert(PQ* pqueue, HTNode* data, size_t priority) {
  PQNode* new_node = (PQNode*)malloc(sizeof(PQNode));
  if (!new_node) {
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

size_t* huffmanGenFT(char* str) {
  size_t* frequency = calloc(256, sizeof(size_t));
  if (!frequency) {
    fprintf(stderr, "Calloc error when creating huffman frequency table in huffmanGenFT\n");
    exit(EXIT_FAILURE);
  }
  while (*str != '\0') {
    frequency[(size_t)*str]++;
    str++;
  }
  return frequency;
}

PQ* huffmanGenPQ(size_t* ft) {
  PQ* pq = malloc(sizeof(PQ));
  if (!pq) {
    fprintf(stderr, "Malloc error when trying to malloc PQ struct in huffmanGenPQ\n");
    exit(EXIT_FAILURE);
  }
  pq->root = NULL;
  pq->num_nodes = 0;

  for (int i = 0; i < 256; i++) {
    if (ft[i] != 0) {
      HTNode* htn = malloc(sizeof(HTNode));
      if (!htn) {
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

HTNode* huffmanGenHT(PQ* pq) {
  PQNode* left;
  PQNode* right;
  HTNode* parent;

  HTNode* root = NULL;

  while (pq->num_nodes > 1) {
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

void huffmanGenCodesHelper(Huffman* huff, HTNode* node, unsigned char* code, int depth) {
  if (!node->left && !node->right) {
    PackedCode* packed_code = malloc(sizeof(PackedCode));
    if (!packed_code) {
      fprintf(stderr, "Malloc error when creating packed code in huffmanGenCodesHelper\n");
      exit(EXIT_FAILURE);
    }
    int num_bytes = (depth + 7) / 8;
    packed_code->code = malloc(num_bytes);
    if (!packed_code->code) {
      fprintf(stderr, "Malloc error when creating code array in huffmanGenCodesHelper\n");
      exit(EXIT_FAILURE);
    }
    memcpy(packed_code->code, code, num_bytes);
    packed_code->num_bytes = num_bytes;
    packed_code->leftover_bits = depth % 8;
    huff->code_table[(int)node->val] = packed_code;
    huff->code_lens[(int)node->val] = depth;
    return;
  }

  if (depth % 8 == 0 && depth != 0) {
    code = realloc(code, (depth / 8) + 1);
    if (!code) {
      fprintf(stderr, "Realloc error in huffmanGenCodesHelper\n");
      exit(EXIT_FAILURE);
    }
  }

  if (node->left) {
    unsigned char* temp_code = malloc((depth / 8) + 1);
    memcpy(temp_code, code, (depth / 8) + 1);
    temp_code[depth / 8] &= ~(1 << (7 - (depth % 8))); 
    huffmanGenCodesHelper(huff, node->left, temp_code, depth + 1);
    free(temp_code);
  }

  if (node->right) {
    unsigned char* temp_code = malloc((depth / 8) + 1);
    memcpy(temp_code, code, (depth / 8) + 1);
    temp_code[depth / 8] |= 1 << (7 - (depth % 8)); // Set bit to 1
    huffmanGenCodesHelper(huff, node->right, temp_code, depth + 1);
    free(temp_code);
  }
}

Huffman* huffmanGenCodes(HTNode* ht_root) {
  Huffman* huffman = malloc(sizeof(Huffman));
  if (!huffman) {
    fprintf(stderr, "Malloc error creating huffman object in huffmanGenCodes\n");
    exit(EXIT_FAILURE);
  }
  PackedCode** code_table = calloc(256, sizeof(PackedCode*));
  int* code_lens = malloc(256 * sizeof(int));
  if (!code_table || !code_lens) {
    fprintf(stderr, "Malloc error creating code table in huffmanGenCodes\n");
    exit(EXIT_FAILURE);
  }
  memset(code_lens, -1, 256 * sizeof(int));
  huffman->encoded_tree = NULL;
  huffman->code_table = code_table;
  huffman->code_lens = code_lens;

  unsigned char* code = malloc(1); // Start with one byte
  if (!code) {
    fprintf(stderr, "Malloc error creating initial code in huffmanGenCodes\n");
    exit(EXIT_FAILURE);
  }
  huffmanGenCodesHelper(huffman, ht_root, code, 0);

  free(code);
  return huffman;
}

void printFrequencyTable(size_t* frequency_table) {
  for (int i = 0; i < 256; i++) {
    if (frequency_table[i] != 0) {
      printf("%c: %zu\n", i, frequency_table[i]);
    }
  }
}

void printPriorityQueue(PQ* pq) {
  PQNode* temp = pq->root;
  while (temp) {
    printf("%c %d\n", temp->data->val, temp->priority);
    temp = temp->next;
  }
}

void huffmanPrintCodes(Huffman* huff) {
  for (int i = 0; i < 256; i++) {
    if (huff->code_lens[i] > 0) {
      printf("%c: bin(", i);
      toBinary(huff->code_table[i]->code, huff->code_lens[i]);
      printf(")\n");
    }
  }
}

Huffman* huffmanGenerateCodes(char* str) {
  size_t* ft = huffmanGenFT(str);
  PQ* pq = huffmanGenPQ(ft);
  HTNode* ht = huffmanGenHT(pq);
  Huffman* huffman = huffmanGenCodes(ht);

  free(ft);
  free(ht);
  free(pq);

  return huffman;
}
