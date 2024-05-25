#include <btree.h>

BTreeNode* bTreeNodeInit(size_t weight, void* data, BTreeNode* parent, BTreeNode* left, BTreeNode* right){
}

void bTreeNodeDelete(BTreeNode* btree_node, void (*freeFunction)(void*)){
}


BTree* bTreeInit(void (*freeFunction)(void*), int (*compareFunction)(const void*, const void*)){
}

void bTreeInsert(BTree* btree, void* data){
}

BTreeNode* bTreeSearch(const BTree* btree, const void* data){
}

void bTreeRemove(BTree* btree, const void* data){
}

void bTreeDelete(BTree* btree){
}
