#ifndef __BTREE_H__
#define __BTREE_H__

#include <stdlib.h>

typedef struct BTreeNode{
  size_t weight;
  void* data; //basic types only right now (cmp with ==)
  struct BTreeNode* parent;
  struct BTreeNode* left;
  struct BTreeNode* right;
} BTreeNode;

typedef struct{
  BTreeNode* root;
  void (*freeFunction)(void*);
  int (*compareFunction)(const void*, const void*);
} BTree;

BTreeNode* bTreeNodeInit(size_t weight, void* data, BTreeNode* parent, BTreeNode* left, BTreeNode* right);

void bTreeNodeDelete(BTreeNode* btree_node, void (*freeFunction)(void*));


BTree* bTreeInit(void (*freeFunction)(void*), int (*compareFunction)(const void*, const void*));

void bTreeInsert(BTree* btree, void* data);

BTreeNode* bTreeSearch(const BTree* btree, const void* data);

void bTreeRemove(BTree* btree, const void* data);

void bTreeDelete(BTree* btree);


#endif
