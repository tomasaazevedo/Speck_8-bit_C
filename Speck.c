/*
 * Speck implementation in C-language for 8-bit platforms.
 * 
 * This implementation of Speck only considers block and key of the same sizes because minimum code size was needed.
 */

#include <string.h>

#include "Speck.h"

void speckDecrypt(byte *key, byte *ct, byte *pt) {
    byte p0, k0;
    byte kt[SPECK_BBYTES];
    int i, j;
    int ak;
    int ap;
    byte *p = &pt[SPECK_WBYTES];
    byte *k = &kt[SPECK_WBYTES];

    for (i = 0; i < SPECK_BBYTES; i++) {
        kt[i] = key[i];
        pt[i] = ct[i];
    }

    //calculates modified key of encrypt to start calculation
    for (i = 0; i < SPECK_ROUNDS; i++) {
        k0 = k[0];
        ak = 0;
        for (j = 0; j < SPECK_WBYTES-1; j++) {
            ak += (uint)kt[j] + (uint)k[j + 1]; k[j] = (byte)ak; ak >>= 8;
        }
        ak += (uint)kt[SPECK_WBYTES-1] + (uint)k0; k[SPECK_WBYTES-1] = (byte)ak;
        kt[SPECK_WBYTES] ^= (byte)i;
        k0 = kt[SPECK_WBYTES-1];
        for (j = SPECK_WBYTES-1; j > 0; j--) {
            kt[j] = ((kt[j] << 3) | (kt[j - 1] >> 5)) ^ k[j];
        }
        kt[0] = ((kt[0] << 3) | (k0 >> 5)) ^ k[0];
    }

    //starts decryption
    for(i = SPECK_ROUNDS-1; i >= 0; i--) {
        //decrypts first part of message
        for(j = 0; j < SPECK_WBYTES; j++) {
            kt[j] ^= k[j];
            pt[j] ^= p[j];
        }
        k0 = kt[0];
        p0 = pt[0];
        for(j = 0; j < SPECK_WBYTES-1; j++) {
            kt[j] = ((kt[j] >> 3) | (kt[j+1] << 5));
            pt[j] = ((pt[j] >> 3) | (pt[j+1] << 5));
        }
        kt[SPECK_WBYTES-1] = ((kt[SPECK_WBYTES-1] >> 3) | (k0 << 5));
        pt[SPECK_WBYTES-1] = ((pt[SPECK_WBYTES-1] >> 3) | (p0 << 5));

        //decrypts second part of message
        k[0] ^= (byte)i;
        for(j = 0; j < SPECK_WBYTES; j++) {
            p[j] ^= kt[j];
        }
        ak = 0;
        ap = 0;
        for(j = 0; j < SPECK_WBYTES; j++) {
            ak += (uint)k[j] - (uint)kt[j];
            k[j] = (byte)ak;
            ak >>= 8;
            ap += (uint)p[j] - (uint)pt[j];
            p[j] = (byte)ap;
            ap >>= 8;
        }

        k0 = k[SPECK_WBYTES-1];
        p0 = p[SPECK_WBYTES-1];
        for(j = SPECK_WBYTES-1; j > 0; j--) {
            k[j] = k[j-1];
            p[j] = p[j-1];
        }
        k[0] = k0;
        p[0] = p0;
    }
}

void speckEncrypt(byte *key, byte *pt, byte *ct) {
    byte c0, k0;
    byte kt[SPECK_BBYTES];
    int i, j;
    uint ac, ak;
    byte *c = &ct[SPECK_WBYTES];
    byte *k = &kt[SPECK_WBYTES];
    for (i = 0; i < SPECK_BBYTES; i++) {
        kt[i] = key[i];
        ct[i] = pt[i];
    }
    for (i = 0; i < SPECK_ROUNDS; i++) {
        c0 = c[0];
        k0 = k[0];
        ac = 0;
        ak = 0;
        for (j = 0; j < SPECK_WBYTES-1; j++) {
            ac += (uint)ct[j] + (uint)c[j + 1]; c[j] = (byte)ac ^ kt[j]; ac >>= 8;
            ak += (uint)kt[j] + (uint)k[j + 1]; k[j] = (byte)ak;         ak >>= 8;
        }
        ac += (uint)ct[SPECK_WBYTES-1] + (uint)c0; c[SPECK_WBYTES-1] = (byte)ac ^ kt[SPECK_WBYTES-1];
        ak += (uint)kt[SPECK_WBYTES-1] + (uint)k0; k[SPECK_WBYTES-1] = (byte)ak;
        kt[SPECK_WBYTES] ^= (byte)i;

        c0 = ct[SPECK_WBYTES-1];
        k0 = kt[SPECK_WBYTES-1];
        for (j = SPECK_WBYTES-1; j > 0; j--) {
            ct[j] = ((ct[j] << 3) | (ct[j - 1] >> 5)) ^ c[j];
            kt[j] = ((kt[j] << 3) | (kt[j - 1] >> 5)) ^ k[j];
        }
        ct[0] = ((ct[0] << 3) | (c0 >> 5)) ^ c[0];
        kt[0] = ((kt[0] << 3) | (k0 >> 5)) ^ k[0];
    }
}
