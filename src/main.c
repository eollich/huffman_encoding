#include <huffman.h>


int main(void) {
    const char* str = "Hello World!";
    // frequency table
    Hashtable* ht = huffmanGenerateFrequencyTable(str);

    // priority queue
    PQueue* pq = huffmanGeneratePriorityQueue(ht);

    // huffman tree
    BTreeNode* huffmanTreeRoot = huffmanBuildTree(pq);

    // huffman codes
    Hashtable* codesTable = huffmanGenerateCodesTable(huffmanTreeRoot);
    if (!codesTable) {
        printf("Failed to generate Huffman codes.\n");
        hashtableDelete(ht);
        pQueueDelete(pq, 0);
        bTreeDelete(huffmanTreeRoot);
        return EXIT_FAILURE;
    }
    printf("Huffman Codes:\n");
    huffmanPrintCodes(codesTable);

     hashtableDelete(ht);
     pQueueDelete(pq, 0);
     //bTreeDelete(huffmanTreeRoot);
     hashtableDelete(codesTable);

    return 0;
}
