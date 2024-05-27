//Frequency Table   Hashmap
//Priority Queue    Priority Queue
//Tree              Tree
//Tree Node         Node
//Code Table        Hashmap
//Encoded Data      bit stream / array of bits

#include <huffman.h>

int main(void){

  char* str = "Hello World\nHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH";
  Hashtable* ht = huffmanGenerateFrequencyTable(str);
  char* s = huffmanGetFrequencyTableStr(ht);
  printf("%s\n", s);

  return 0;
}
