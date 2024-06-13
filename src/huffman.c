#include <huffman.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void toBinary(unsigned char* code, int num_bits) {
  for (int i = 0; i < num_bits; i++) {
    if(i!=0 && i%8 == 0) printf(" ");
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
    //check
    packed_code->leftover_bits = 8 - depth % 8;
    packed_code->leftover_bits = depth % 8;
    packed_code->leftover_bits = depth % 8 == 0 ? 8 : depth % 8;
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
    temp_code[depth / 8] |= 1 << (7 - (depth % 8));
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

  unsigned char* code = malloc(1);
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
    if (huff->code_table[i]!=NULL) {
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


void addPackedCode(PackedCode* pc1, PackedCode* pc2) {
  int total_bits = (pc1->num_bytes - 1) * 8 + pc1->leftover_bits + (pc2->num_bytes - 1) * 8 + pc2->leftover_bits;
  int num_bytes = (total_bits + 7) / 8;

  unsigned char* new_code = calloc(num_bytes, sizeof(unsigned char));
  if (!new_code) {
    fprintf(stderr, "Calloc error in addPackedCode\n");
    exit(EXIT_FAILURE);
  }

  if(pc1->code)
    for (int i = 0; i < pc1->num_bytes; i++) {
      new_code[i] = pc1->code[i];
    }

  int bit_pos = pc1->leftover_bits;
  int byte_pos = pc1->num_bytes - 1;

  for (int i = 0; i < pc2->num_bytes; i++) {
    new_code[byte_pos] |= pc2->code[i] >> bit_pos;

    if (bit_pos > 0 && byte_pos + 1 < num_bytes) {
      new_code[byte_pos + 1] |= pc2->code[i] << (8 - bit_pos);
    }

    byte_pos++;
  }

  free(pc1->code);
  pc1->code = new_code;
  pc1->num_bytes = num_bytes;
  pc1->leftover_bits = total_bits % 8;
  if (pc1->leftover_bits == 0) {
    pc1->leftover_bits = 8;
  }
}


void printPackedCode(PackedCode* pc){
  toBinary(pc->code, pc->num_bytes * 8 - (8 - pc->leftover_bits));
  printf("\n");
}


int calculateEncodedTreeLength(HTNode* node) {
  if (node->left == NULL && node->right == NULL) {
    return 1 + 8;
  } else {
    return 1 + calculateEncodedTreeLength(node->left) + calculateEncodedTreeLength(node->right);
  }
}

void encodeNodeToBuffer(HTNode* node, char* buffer, int* bit_pos) {
  if (node->left == NULL && node->right == NULL) {
    buffer[*bit_pos / 8] |= (1 << (7 - (*bit_pos % 8)));
    (*bit_pos)++;
    for (int i = 0; i < 8; i++) {
      if (node->val & (1 << (7 - i))) {
        buffer[*bit_pos / 8] |= (1 << (7 - (*bit_pos % 8)));
      }
      (*bit_pos)++;
    }
  } else {
    buffer[*bit_pos / 8] &= ~(1 << (7 - (*bit_pos % 8)));
    (*bit_pos)++;
    encodeNodeToBuffer(node->left, buffer, bit_pos);
    encodeNodeToBuffer(node->right, buffer, bit_pos);
  }
}

char* huffmanEncodeTree(HTNode* root) {
  if (!root) return NULL;

  int length = calculateEncodedTreeLength(root);
  int numBytes = (length + 7) / 8;

  char* buffer = (char*)calloc(numBytes, sizeof(char));
  if (!buffer) {
    fprintf(stderr, "Calloc error in huffmanEncodeTree\n");
    exit(EXIT_FAILURE);
  }

  int bit_pos = 0;
  encodeNodeToBuffer(root, buffer, &bit_pos);

  return buffer;
}

void printEncodedTree(char* encoded_tree, int length) {
  for (int i = 0; i < (length + 7) / 8; i++) {
    for (int j = 7; j >= 0; j--) {
      printf("%d", (encoded_tree[i] >> j) & 1);
    }
  }
  printf("\n");
}
