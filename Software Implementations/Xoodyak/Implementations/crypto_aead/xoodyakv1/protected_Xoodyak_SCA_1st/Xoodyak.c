/*
Implementation by Shuohang Peng 

Thanks for Ronny Van Keer's pioneering implementation of the Xoodyak C code,
our Xoodyak software protection implementation was adapted from his/her reference code. 

The reference link for the original C code is as follows:
https://csrc.nist.gov/Projects/lightweight-cryptography/finalists
*/

#ifdef XoodooReference
    #include "displayIntermediateValues.h"
#endif

#include <assert.h>
#include <string.h>
#include "Xoodyak.h"

#ifdef OUTPUT
#include <stdlib.h>
#include <string.h>

static void displayByteString(FILE *f, const char* synopsis, const uint8_t *data, unsigned int length);
static void displayByteString(FILE *f, const char* synopsis, const uint8_t *data, unsigned int length)
{
    unsigned int i;

    fprintf(f, "%s:", synopsis);
    for(i=0; i<length; i++)
        fprintf(f, " %02x", (unsigned int)data[i]);
    fprintf(f, "\n");
}
#endif

#define MyMin(a,b)  (((a) < (b)) ? (a) : (b))

#ifndef Xoodoo_excluded
    #include "Xoodoo-SnP.h"

    #define SnP                         Xoodoo
    #define SnP_Permute                 Xoodoo_Permute_12rounds
    #define prefix                      Xoodyak
        #include "Cyclist.inc"
    #undef  prefix
    #undef  SnP
    #undef  SnP_Permute
#endif
