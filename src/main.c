#include <huffman.h>
int main(void){
  char* codes = huffmanGenerateCodes("aaasdff");
  huffmanPrintCodes(codes);
  return 0;
}
