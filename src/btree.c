#include <btree.h>

void bubbleUp(BTreeNode* node) {
  while (node->parent && node->weight < node->parent->weight) {
    size_t tempWeight = node->weight;
    void* tempData = node->data;
    node->weight = node->parent->weight;
    node->data = node->parent->data;
    node->parent->weight = tempWeight;
    node->parent->data = tempData;

    node = node->parent;
  }
}


void bubbleDown(BTreeNode* node) {
  while (node) {
    BTreeNode* smallest = node;
    if (node->left && node->left->weight < smallest->weight) {
      smallest = node->left;
    }
    if (node->right && node->right->weight < smallest->weight) {
      smallest = node->right;
    }
    if (smallest == node) break;

    size_t tempWeight = node->weight;
    void* tempData = node->data;
    node->weight = smallest->weight;
    node->data = smallest->data;
    smallest->weight = tempWeight;
    smallest->data = tempData;

    node = smallest;
  }
}


BTreeNode* findLastNode(BTreeNode* root) {
  if (!root) return NULL;

  BTreeNode* lastNode = NULL;
  size_t queueCapacity = 16;
  BTreeNode** queue = malloc(queueCapacity * sizeof(BTreeNode*));
  if (!queue) {
    fprintf(stderr, "findLastNode: malloc error\n");
    exit(EXIT_FAILURE);
  }

  size_t front = 0, rear = 0;

  queue[rear++] = root;

  while (front < rear) {
    if (rear >= queueCapacity) {
      queueCapacity *= 2;
      queue = realloc(queue, queueCapacity * sizeof(BTreeNode*));
      if (!queue) {
        fprintf(stderr, "findLastNode: realloc error\n");
        exit(EXIT_FAILURE);
      }
    }

    BTreeNode* current = queue[front++];

    lastNode = current;

    if (current->left) queue[rear++] = current->left;
    if (current->right) queue[rear++] = current->right;
  }

  free(queue);
  return lastNode;
}


//interface
BTreeNode* bTreeNodeInit(size_t weight, void* data, BTreeNode* parent, BTreeNode* left, BTreeNode* right){
  BTreeNode* node = malloc(sizeof(BTreeNode));
  if(!node){
    fprintf(stderr, "bTreeNodeInit malloc error\n");
    exit(EXIT_FAILURE);
  }

  node->weight = weight;
  node->data = data;
  node->parent = parent;
  node->left = left;
  node->right = right;
  return node;
}

void bTreeNodeDelete(BTreeNode* btree_node, void (*freeFunction)(BTreeNode*)){
  if(btree_node && freeFunction) freeFunction(btree_node);
}


BTree* bTreeInit(void (*freeFunction)(BTreeNode*), int (*compareFunction)(const void*, const void*)){
  BTree* tree = malloc(sizeof(BTree));
  if(!tree){
    fprintf(stderr, "bTreeInit malloc error\n");
    exit(EXIT_FAILURE);
  }

  tree->root = NULL;
  tree->freeFunction = freeFunction ? freeFunction : bTreeDefaultFreeFunction;
  tree->compareFunction = compareFunction ? compareFunction : bTreeDefaultCompareFunction;

  return tree;
}

BTreeNode* bTreeFindLastNode(BTree* btree){
  return findLastNode(btree->root);
}

void bTreeInsert(BTree* btree, size_t weight, void* data) {
  if (!btree) return;
  BTreeNode* node = bTreeNodeInit(weight, data, NULL, NULL, NULL);

  if (!btree->root) {
    btree->root = node;
    return;
  }

  BTreeNode* cur = btree->root;
  BTreeNode* parent = NULL;
  while (cur) {
    parent = cur;
    if (weight < cur->weight) {
      if (!cur->left) {
        cur->left = node;
        node->parent = cur;
        break;
      }
      cur = cur->left;
    } else {
      if (!cur->right) {
        cur->right = node;
        node->parent = cur;
        break;
      }
      cur = cur->right;
    }
  }

  bubbleUp(node);
}


// Remove a node from the binary tree

BTreeNode* bTreeSearch(const BTree* btree, const void* data){
  if(!btree || !btree->root) return NULL;
  BTreeNode* cur = btree->root;
  while(cur){
    int cmp = btree->compareFunction(data, cur->data);
    if(cmp == 0){
      return cur;
    }else if(cmp < 0){
      cur = cur -> left;
    }else{
      cur = cur->right;
    }
  }

  return NULL;
}

void bTreeRemove(BTree* btree, const void* data) {
  if (!btree || !btree->root) return;

  BTreeNode* node = bTreeSearch(btree, data);
  if (!node) return;

  BTreeNode* lastNode = findLastNode(btree->root);
  if (!lastNode) return;

  if (node != lastNode) {
    node->weight = lastNode->weight;
    node->data = lastNode->data;
  }

  BTreeNode* parent = lastNode->parent;
  if (parent) {
    if (parent->left == lastNode) {
      parent->left = NULL;
    } else if (parent->right == lastNode) {
      parent->right = NULL;
    }
  } else {
    btree->root = NULL; // The heap is now empty
  }

  btree->freeFunction(lastNode);

  bubbleDown(node);
}

//void deleteNodeRecursively(BTreeNode* node, void (*freeFunction)(BTreeNode*)) {
//  if (!node) return;
//  deleteNodeRecursively(node->left, freeFunction);
//  deleteNodeRecursively(node->right, freeFunction);
//  bTreeNodeDelete(node, freeFunction);
//}
//
//void bTreeDelete(BTree* btree) {
//  if (!btree) return;
//
//
//  deleteNodeRecursively(btree->root, btree->freeFunction);
//  free(btree);
//}

void bTreeDelete(BTree* btree) {
  if (!btree) return;

  BTreeNode* cur = btree->root;
  BTreeNode* parent = NULL;
  while (cur) {
    if (cur->left) {
      cur = cur->left;
    } else if (cur->right) {
      cur = cur->right;
    } else {
      if (cur == btree->root) {
        bTreeNodeDelete(cur, btree->freeFunction);
        btree->root = NULL;
        break;
      }
      parent = cur->parent;
      if (parent->left == cur) {
        parent->left = NULL;
      } else {
        parent->right = NULL;
      }
      bTreeNodeDelete(cur, btree->freeFunction);
      cur = parent;
    }
  }

  free(btree);
}

int bTreeDefaultCompareFunction(const void* a, const void* b) {
  return (*(int*)a - *(int*)b);
}

void bTreeDefaultFreeFunction(BTreeNode* btree_node) {
  if (btree_node) {
    free(btree_node);
  }
}
