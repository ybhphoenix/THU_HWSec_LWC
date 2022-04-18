/*
Implementation by Shuohang Peng 

Thanks for Ronny Van Keer's pioneering implementation of the Xoodyak C code,
our Xoodyak software protection implementation was adapted from his/her reference code. 

The reference link for the original C code is as follows:
https://csrc.nist.gov/Projects/lightweight-cryptography/finalists
*/

#ifndef _Cyclist_h_
#define _Cyclist_h_

#include <stdint.h>
#include "align.h"

#define Cyclist_ModeHash    1
#define Cyclist_ModeKeyed   2

#define Cyclist_PhaseDown   1
#define Cyclist_PhaseUp     2

#ifdef OUTPUT

#include    <stdio.h>

#define KCP_DeclareCyclistStructure(prefix, size, alignment)                    \
            ALIGN(alignment) typedef struct prefix##_CyclistInstanceStruct {    \
            uint8_t         state[size];                                        \
            uint8_t         stateShadow[size];                                  \
            FILE            *file;                                              \
            unsigned int    phase;                                              \
            unsigned int    mode;                                               \
            unsigned int    Rabsorb;                                            \
            unsigned int    Rsqueeze;                                           \
        } prefix##_Instance;

#else

#define KCP_DeclareCyclistStructure(prefix, size, alignment)                    \
            ALIGN(alignment) typedef struct prefix##_CyclistInstanceStruct {    \
            uint8_t         state[size];                                        \
            unsigned int    phase;                                              \
            unsigned int    mode;                                               \
            unsigned int    Rabsorb;                                            \
            unsigned int    Rsqueeze;                                           \
        } prefix##_Instance;

#endif

#define KCP_DeclareCyclistFunctions(prefix) \
    void prefix##_Initialize(prefix##_Instance *instance1, prefix##_Instance *instance0, uint8_t *K1, uint8_t *K0, size_t KLen, uint8_t *ID1, uint8_t *ID0, size_t IDLen, uint8_t *counter, size_t counterLen); \
    void prefix##_Absorb(prefix##_Instance *instance1, prefix##_Instance *instance0, uint8_t *X1, uint8_t *X0, size_t XLen); \
    void prefix##_Encrypt(prefix##_Instance *instance1, prefix##_Instance *instance0, uint8_t *P1, uint8_t *P0, uint8_t *C1, uint8_t *C0, size_t PLen); \
    void prefix##_Decrypt(prefix##_Instance *instance1, prefix##_Instance *instance0, uint8_t *C1, uint8_t *C0, uint8_t *P1, uint8_t *P0, size_t CLen); \
    void prefix##_Squeeze(prefix##_Instance *instance1, prefix##_Instance *instance0, uint8_t *Y1, uint8_t *Y0, size_t YLen); \
    void prefix##_SqueezeKey(prefix##_Instance *instance1, prefix##_Instance *instance0, uint8_t *K1, uint8_t *K0, size_t KLen); \
    void prefix##_Ratchet(prefix##_Instance *instance1, prefix##_Instance *instance0);

#endif
