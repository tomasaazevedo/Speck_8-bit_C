/*
 * Speck implementation in C-language for 8-bit platforms.
 * 
 * This main file gives example of use of the speck functions with a simple test.
 */

#include "Speck.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define ENCRYPT
// #define DECRYPT

static void Display(const char *tag, unsigned char block[], int block_size) {
    int i;
    printf("%s\t=\t", tag);
    for (i = 0; i < block_size; i++) {
        printf("%02X", block[i]);
    }
    printf("\n");
}

static BOOL equalBlocks(byte first_block[], byte second_block[], const int block_sizes) {
  int i;
  BOOL result = TRUE;

  for (i = 0; i < block_sizes; i++) {
    if(first_block[i] != second_block[i]) {
      result = FALSE;
      break;
    }
  }

  return result;
}

BOOL speckTest(int ntests) {
  byte key[SPECK_BBYTES], pt[SPECK_BBYTES], ct[SPECK_BBYTES], ct_aux[SPECK_BBYTES];

  //test values for 128-bit block and key sizes
  memcpy(key, "\x2b\x7e\x15\x16\x78\xcf\xa7\xca\xc4\xd7\x57\xe9\x7b\xbd\x2a\x48", SPECK_BBYTES);
  memcpy(pt, "\x02\x05\x05\x09\x54\x6f\x6d\xe1\x73\x20\x41\x2e\x20\x41\x7a\x65", SPECK_BBYTES);
  memcpy(ct_aux, "\x2D\x14\xF7\xE2\x96\x62\xFA\xC8\xD3\x8F\x1B\xBE\x8A\xBF\x5A\x19", SPECK_BBYTES);

  for(int j = 0; j < ntests; j++) {
    #ifdef ENCRYPT
      speckEncrypt(key, pt, ct);
    #elif defined(DECRYPT)
      speckDecrypt(key, ct_aux, ct);
    #endif

    Display("key", key, SPECK_BBYTES);
    Display("pt", pt, SPECK_BBYTES);
    Display("ct", ct, SPECK_BBYTES);
    Display("ct_aux", ct_aux, SPECK_BBYTES);
    #ifdef ENCRYPT
      if(equalBlocks(ct, ct_aux, SPECK_BBYTES))
        printf("ENCRYPT SUCESS!!\n");
      else
        printf("ENCRYPT FAILURE!!\n");
    #elif defined(DECRYPT)
      if(equalBlocks(ct, pt, SPECK_BBYTES))
        printf("DECRYPT SUCESS!!\n");
      else
        printf("DECRYPT FAILURE!!\n");
    #endif
  }

  return TRUE;

}

int main(void) {
  int ntests;
  BOOL test_results;

  ntests = 1;

  test_results = speckTest(ntests);

  if(test_results)
      printf("SUCESS!!\n");
  else
      printf("FAILURE!!\n");

  return 0;
}
