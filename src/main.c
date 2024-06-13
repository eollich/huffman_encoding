#include <huffman.h>
#include <stdio.h>
#include <string.h>

int main(void){
  //bug when only one code, length is 0 so doesnt show ( only in print ) Kind of fixed, now just empty
  char* str = "asdf";

  Huffman* huff = huffmanGenerateCodes(str);
  huffmanPrintCodes(huff);

  printf("%d\n", *huff->code_table['a']->code);


  //PackedCode* pc = huff->code_table[(int)('f')];
  //printf("%d\n", pc->leftover_bits);
  //printPackedCode(pc);


  //unsigned char ca[] = {0b11000000, 0b10101010};
  //PackedCode a = {ca, 2, 8};

  //addPackedCode(pc, &a);
  //printPackedCode(pc);

  PackedCode* final = malloc(sizeof(PackedCode));
  final->leftover_bits= 0;
  final->code=NULL;
  final->num_bytes=1;

  //addPackedCode(final, huff->code_table[(int)('a')]);
  //addPackedCode(final, huff->code_table[(int)('b')]);
  //printPackedCode(final);
  //printPackedCode(huff->code_table[(int)('a')]);

  char* ex = str;
  while(*ex!='\0'){
    addPackedCode(final, huff->code_table[(int)(*ex)]);
    //printf("added :%c\n", *ex);
    //printPackedCode(final);
    //printf("\n\n");
    ex++;
  }

  printf("HUFF ENCODED STRING BYTE COUNT: %d\n", final->num_bytes);


  for(int i=0; i<final->num_bytes; i++){
    printf("%c", final->code[i]);
  }
  printf("\n");

  printf("ORIGINAL STRING BYTE COUNT: %zu\n", strlen(str));
  printf("%s\n",str);


  return 0;
}
