/*
Implementation by Shuohang Peng 

Thanks for Ronny Van Keer's pioneering implementation of the Xoodyak C code,
our Xoodyak software protection implementation was adapted from his/her reference code. 

The reference link for the original C code is as follows:
https://csrc.nist.gov/Projects/lightweight-cryptography/finalists
*/

/* #define VERBOSE_LEVEL    0 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "Xoodoo.h"
#include "storerand.h"

unsigned int Cnt = 0;
/* ---------------------------------------------------------------- */

void Xoodoo_StaticInitialize( void )
{
}

/* ---------------------------------------------------------------- */

void Xoodoo_Initialize(void *state1, void *state0)
{
    memset(state0, 0, NLANES*sizeof(tXoodooLane));  
    memset(state1, 0, NLANES*sizeof(tXoodooLane)); 
}

/* ---------------------------------------------------------------- */
void Xoodoo_AddByte(void *state, unsigned char byte, unsigned int offset)  
{
    assert(offset < NLANES*sizeof(tXoodooLane));  
    ((unsigned char *)state)[offset] ^= byte;
}

/* ---------------------------------------------------------------- */

void Xoodoo_AddBytes(void *state, unsigned char *data, unsigned int offset, unsigned int length)
{
    unsigned int i;

    assert(offset < NLANES*sizeof(tXoodooLane));
    assert(offset+length <= NLANES*sizeof(tXoodooLane));
    for(i=0; i<length; i++)
        ((unsigned char *)state)[offset+i] ^= data[i]; 
}

/* ---------------------------------------------------------------- */

void Xoodoo_OverwriteBytes(void *state, unsigned char *data, unsigned int offset, unsigned int length)
{
    assert(offset < NLANES*sizeof(tXoodooLane));
    assert(offset+length <= NLANES*sizeof(tXoodooLane));
    memcpy((unsigned char*)state+offset, data, length); 
}

/* ---------------------------------------------------------------- */

void Xoodoo_OverwriteWithZeroes(void *state, unsigned int byteCount)
{
    assert(byteCount <= NLANES*sizeof(tXoodooLane));
    memset(state, 0, byteCount);
}

/* ---------------------------------------------------------------- */

void Xoodoo_ExtractBytes(void *state, unsigned char *data, unsigned int offset, unsigned int length)  
{
    assert(offset < NLANES*sizeof(tXoodooLane));
    assert(offset+length <= NLANES*sizeof(tXoodooLane));
    memcpy(data, (unsigned char*)state+offset, length);
}

/* ---------------------------------------------------------------- */

void Xoodoo_ExtractAndAddBytes(void *state, unsigned char *input, unsigned char *output, unsigned int offset, unsigned int length)
{
    unsigned int i;

    assert(offset < NLANES*sizeof(tXoodooLane));
    assert(offset+length <= NLANES*sizeof(tXoodooLane));
    for(i=0; i<length; i++)
        output[i] = input[i] ^ ((unsigned char *)state)[offset+i];
}

/* ---------------------------------------------------------------- */

#if defined(VERBOSE_LEVEL)

static void Dump(char * text, tXoodooLane * a, unsigned int level)
{
    if (level == VERBOSE_LEVEL) {
    #if 0
        printf("%-8.8s ", text);
        printf("%u %u %u %u - ", a[0+0], a[0+1], a[0+2], a[0+3] );
        printf("%u %u %u %u - ", a[4+0], a[4+1], a[4+2], a[4+3] );
        printf("%u %u %u %u\n", a[8+0], a[8+1], a[8+2], a[8+3] );
        if ((level == 2) && !strcmp(text, "Rho-east"))
            printf("\n");
    #elif 0
        printf("%-8.8s ", text);
        printf("%08x %08x %08x %08x - ", a[0+0], a[0+1], a[0+2], a[0+3] );
        printf("%08x %08x %08x %08x - ", a[4+0], a[4+1], a[4+2], a[4+3] );
        printf("%08x %08x %08x %08x\n", a[8+0], a[8+1], a[8+2], a[8+3] );
    #else
        printf("%s\n", text);
        printf("a00 %08x, a01 %08x, a02 %08x, a03 %08x\n", a[0+0], a[0+1], a[0+2], a[0+3] );
        printf("a10 %08x, a11 %08x, a12 %08x, a13 %08x\n", a[4+0], a[4+1], a[4+2], a[4+3] );
        printf("a20 %08x, a21 %08x, a22 %08x, a23 %08x\n\n", a[8+0], a[8+1], a[8+2], a[8+3] );
    #endif
    }
}

#else

#define Dump(text, a, level)

#endif


static void fromBytesToWords(tXoodooLane *stateAsWords, unsigned char *state)
{
    unsigned int i, j;

    for(i=0; i<NLANES; i++) {
        stateAsWords[i] = 0;
        for(j=0; j<sizeof(tXoodooLane); j++)  
            stateAsWords[i] |= (tXoodooLane)(state[i*sizeof(tXoodooLane)+j]) << (8*j);
    }
}

static void fromWordsToBytes(unsigned char *state, tXoodooLane *stateAsWords)
{
    unsigned int i, j;

    for(i=0; i<NLANES; i++)
        for(j=0; j<sizeof(tXoodooLane); j++)
            state[i*sizeof(tXoodooLane)+j] = (stateAsWords[i] >> (8*j)) & 0xFF;
}

static void fromBytesToWords_sel1(uint32_t *state, unsigned char *data, uint32_t wlen)
{
	uint32_t i, j;

	for(i = 0; i < wlen; i++) {
        state[i] = 0;
        for(j=0; j<sizeof(tXoodooLane); j++)
            state[i] |= (tXoodooLane)(data[i*sizeof(tXoodooLane)+j]) << (8*j);
    }
}


static void ISW_and(uint32_t *output1, uint32_t *output0, uint32_t *stateb1, uint32_t *stateb0, uint32_t *statea1, uint32_t *statea0)
{
	unsigned char r01[16] = {0};
	uint32_t x;
	uint32_t tempwords[4];
	uint32_t r10[4];
	
	//Get Random Number from randnombytes16
	memcpy(r01, randombytes16 + Cnt * 16, 16);
	
	if(Cnt == 216)
		Cnt = 0;
	else
	    Cnt++;
	
	fromBytesToWords_sel1(tempwords, r01, 4);

	for (x = 0; x < NCOLUMS; x++)
	{
		r10[index1(x,0)] = tempwords[index1(x,0)] ^ (statea0[index1(x,0)] & stateb1[(index1(x,0))]);
	}
	for (x = 0; x < NCOLUMS; x++)
	{
		r10[index1(x,0)] = r10[index1(x,0)] ^ (statea1[index1(x,0)] & stateb0[(index1(x,0))]);
	}

	for (x = 0; x < NCOLUMS; x++)
	{
		output0[index1(x,0)] = statea0[index1(x,0)] & stateb0[(index1(x,0))];
	}

	for (x = 0; x < NCOLUMS; x++)
	{
		output0[index1(x,0)] = output0[index1(x,0)] ^ tempwords[(index1(x,0))];
	}

	for (x = 0; x < NCOLUMS; x++)
	{
		output1[index1(x,0)] = statea1[index1(x,0)] & stateb1[(index1(x,0))];
	}
	for (x = 0; x < NCOLUMS; x++)
	{
		output1[index1(x,0)] = r10[index1(x,0)] ^ output1[index1(x,0)];
	}
}

static void PlaneRev(tXoodooLane * output, tXoodooLane * input, tXoodooLane y, tXoodooLane x)
{
	uint32_t i = 0;
	for(i = 0; i < NCOLUMS; i++)
	{
		output[i] = ~input[index1(x + i,y)];
	}
}

static void Xoodoo_Round( tXoodooLane * a, tXoodooLane * as, tXoodooLane rc )
{
    unsigned int x, y;
    tXoodooLane    b[NLANES];   //NLANES = 12
    tXoodooLane    p[NCOLUMS];  //NCOLUMS = 4
    tXoodooLane    e[NCOLUMS];

    //share of B
    tXoodooLane b00[NCOLUMS], b01[NCOLUMS];
    tXoodooLane b10[NCOLUMS], b11[NCOLUMS];
    tXoodooLane b20[NCOLUMS], b21[NCOLUMS];
	
    //share a
    for (x=0; x<NCOLUMS; ++x)
        p[x] = a[index1(x,0)] ^ a[index1(x,1)] ^ a[index1(x,2)];
    for (x=0; x<NCOLUMS; ++x)
        e[x] = ROTL32(p[(x-1)%4], 5) ^ ROTL32(p[(x-1)%4], 14);  
    for (x=0; x<NCOLUMS; ++x)
        for (y=0; y<NROWS; ++y)
            a[index1(x,y)] ^= e[x];
//    Dump("Theta", a, 2);
	
	
    // share as
	for (x=0; x<NCOLUMS; ++x)
        p[x] = as[index1(x,0)] ^ as[index1(x,1)] ^ as[index1(x,2)];
    for (x=0; x<NCOLUMS; ++x)
        e[x] = ROTL32(p[(x-1)%4], 5) ^ ROTL32(p[(x-1)%4], 14);  
    for (x=0; x<NCOLUMS; ++x)
        for (y=0; y<NROWS; ++y)
            as[index1(x,y)] ^= e[x];
//    Dump("Theta", as, 2);

    /* Rho-west: plane shift */
    //share a
    for (x=0; x<NCOLUMS; ++x) {
        b[index1(x,0)] = a[index1(x,0)];
        b[index1(x,1)] = a[index1(x-1,1)];
        b[index1(x,2)] = ROTL32(a[index1(x,2)], 11);
    }
    memcpy( a, b, sizeof(b) );
	
	
    //Dump("Rho-west", a, 2);
    //share as
    for (x=0; x<NCOLUMS; ++x) {
        b[index1(x,0)] = as[index1(x,0)];
        b[index1(x,1)] = as[index1(x-1,1)];
        b[index1(x,2)] = ROTL32(as[index1(x,2)], 11);
    }
    memcpy( as, b, sizeof(b) );
    
        
    /* Iota: round constant */
    a[0] ^= rc;
	
    /* Chi: non linear layer */
    PlaneRev(p, a, 1, 0);
    ISW_and(b01, b00, as + 8, a + 8, as + 4, p);

    PlaneRev(p, a, 2, 0);
    ISW_and(b11, b10, as, a, as + 8, p);

    PlaneRev(p, a, 0, 0);
    ISW_and(b21, b20, as + 4, a + 4, as, p);

	
    for (x=0; x<NCOLUMS; ++x) {
        a[index1(x,0)] = a[index1(x,0)] ^ b00[index1(x,0)];
        as[index1(x,0)] = as[index1(x,0)] ^ b01[index1(x,0)];
        a[index1(x,1)] = a[index1(x,1)] ^ b10[index1(x,0)];
        as[index1(x,1)] = as[index1(x,1)] ^ b11[index1(x,0)];
        a[index1(x,2)] = a[index1(x,2)] ^ b20[index1(x,0)];
        as[index1(x,2)] = as[index1(x,2)] ^ b21[index1(x,0)];
    }

    /* Rho-east: plane shift */
    // share a
    for (x=0; x<NCOLUMS; ++x) {
        b[index1(x,0)] = a[index1(x,0)];
        b[index1(x,1)] = ROTL32(a[index1(x,1)], 1);
        b[index1(x,2)] = ROTL32(a[index1(x+2,2)], 8);
    }
    memcpy( a, b, sizeof(b) );
//    Dump("Rho-east", a, 2);
    // share as
	for (x=0; x<NCOLUMS; ++x) {
        b[index1(x,0)] = as[index1(x,0)];
        b[index1(x,1)] = ROTL32(as[index1(x,1)], 1);
        b[index1(x,2)] = ROTL32(as[index1(x+2,2)], 8);
    }
    memcpy( as, b, sizeof(b) );
//    Dump("Rho-east", as, 2);
}

static const uint32_t    RC[MAXROUNDS] = {
    _rc12,
    _rc11,
    _rc10,
    _rc9,
    _rc8,
    _rc7,
    _rc6,
    _rc5,
    _rc4,
    _rc3,
    _rc2,
    _rc1
};

void Xoodoo_Permute_Nrounds( void * state1, void * state0, uint32_t nr )
{
    tXoodooLane        a[NLANES];
    tXoodooLane        as[NLANES];
    unsigned int    i;

    fromBytesToWords(a, (unsigned char *)state0);
    fromBytesToWords(as, (unsigned char *)state1);

    for (i = MAXROUNDS - nr + 10; i < MAXROUNDS; ++i ) {
        Xoodoo_Round(a, as, RC[i]);		
//        Dump("Round", a, 1);
//        Dump("Round", as, 1);
    }
//    Dump("Permutation", a, 0);

    fromWordsToBytes((unsigned char *)state0, a);
    fromWordsToBytes((unsigned char *)state1, as);

}

void Xoodoo_Permute_6rounds( void * state1, void * state0)
{
    Xoodoo_Permute_Nrounds( state1, state0, 6 );
}

void Xoodoo_Permute_12rounds( void * state1, void * state0)
{
    Xoodoo_Permute_Nrounds( state1, state0, 12 );
}
