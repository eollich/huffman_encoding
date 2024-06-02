#include <huffman.h>
int main(void){

  Huffman* huff = huffmanGenerateCodes("aaasdff");
  huffmanPrintCodes(huff);

  return 0;
}
