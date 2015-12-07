#ifndef __SPECK_H
#define __SPECK_H

#ifndef USUAL_TYPES
#define USUAL_TYPES
typedef unsigned char byte;
typedef unsigned int  uint;
typedef unsigned long long uint64_t;
#endif /* USUAL_TYPES */

#ifndef BOOL
#define BOOL    int
#define FALSE   0
#define TRUE    1
#endif

// Define size of block and key
#define SPECK_BITS 128

#if (SPECK_BITS==128)
#define SPECK_WBYTES 8
#define SPECK_BBYTES (2*SPECK_WBYTES)
#define SPECK_ROUNDS 32
#elif (SPECK_BITS==112)
#define SPECK_WBYTES 7
#define SPECK_BBYTES (2*SPECK_WBYTES)
#define SPECK_ROUNDS 30
#elif (SPECK_BITS==96)
#define SPECK_WBYTES 6
#define SPECK_BBYTES (2*SPECK_WBYTES)
#define SPECK_ROUNDS 28
#elif (SPECK_BITS==80)
#define SPECK_WBYTES 5
#define SPECK_BBYTES (2*SPECK_WBYTES)
#define SPECK_ROUNDS 26
#elif (SPECK_BITS==64)
#define SPECK_WBYTES 4
#define SPECK_BBYTES (2*SPECK_WBYTES)
#define SPECK_ROUNDS 24
#else
#error "SPECK_BITS undefined"
#endif // SPECK_BITS

void speckEncrypt(byte *key, byte *pt, byte *ct);
void speckDecrypt(byte *key, byte *ct, byte *pt);

#endif /* __SPECK_H */

