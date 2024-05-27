#ifndef __BTREE_H__
#define __BTREE_H__

#include <stdlib.h>
#include <stdio.h>

typedef struct BTreeNode{
  size_t weight;
  void* data; //basic types only right now (cmp with ==)
  struct BTreeNode* parent;
  struct BTreeNode* left;
  struct BTreeNode* right;
} BTreeNode;

typedef struct{
  BTreeNode* root;
  void (*freeFunction)(BTreeNode*);
  int (*compareFunction)(const void*, const void*);
} BTree;

// Function prototypes
BTreeNode* bTreeNodeInit(size_t weight, void* data, BTreeNode* parent, BTreeNode* left, BTreeNode* right);
void bTreeNodeDelete(BTreeNode* btree_node, void (*freeFunction)(BTreeNode*));

BTree* bTreeInit(void (*freeFunction)(BTreeNode*), int (*compareFunction)(const void*, const void*));
BTreeNode* bTreeFindLastNode(BTree* btree);
void bTreeInsert(BTree* btree, size_t weight, void* data);
BTreeNode* bTreeSearch(const BTree* btree, const void* data);
void bTreeRemove(BTree* btree, const void* data);
void bTreeDelete(BTree* btree);
int bTreeDefaultCompareFunction(const void*, const void*);
void bTreeDefaultFreeFunction(BTreeNode* btree_node);

#endif
