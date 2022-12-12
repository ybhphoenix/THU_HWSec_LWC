#include <stdint.h>
#include "api.h"

#define NUM_WORDS_M 8
#define NUM_WORDS_C 12
#define NUM_WORDS_AD 8
#define NUM_WORDS_NPUB 4
#define NUM_WORDS_KEY 4

#define NUM_BYTES_M 32
#define NUM_BYTES_C 48
#define NUM_BYTES_AD 32
#define NUM_BYTES_NPUB 16
#define NUM_BYTES_KEY 16


typedef struct 
{
    uint32_t shares[NUM_SHARES_M];
}mask_m_uint32_t;

typedef struct 
{
    uint32_t shares[NUM_SHARES_C];
}mask_c_uint32_t;

typedef struct 
{
    uint32_t shares[NUM_SHARES_AD];
}mask_ad_uint32_t;

typedef struct 
{
    uint32_t shares[NUM_SHARES_NPUB];
}mask_npub_uint32_t;

typedef struct 
{
    uint32_t shares[NUM_SHARES_KEY];
}mask_key_uint32_t;

