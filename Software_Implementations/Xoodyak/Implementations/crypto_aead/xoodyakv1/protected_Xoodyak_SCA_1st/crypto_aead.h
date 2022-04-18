#ifndef _crypto_aead_h_
#define _crypto_aead_h_

#include "share.h"

int crypto_aead_encrypt_shared(
    mask_c_uint32_t * cs , unsigned long long * clen ,
    mask_m_uint32_t * ms , unsigned long long mlen ,
    mask_ad_uint32_t * ads , unsigned long long adlen ,
    mask_npub_uint32_t * npubs ,
    mask_key_uint32_t * ks
);

int crypto_aead_decrypt_shared (
    mask_m_uint32_t * ms , unsigned long long * mlen ,
    mask_c_uint32_t * cs , unsigned long long clen ,
    mask_ad_uint32_t * ads , unsigned long long adlen ,
    mask_npub_uint32_t * npubs ,
    mask_key_uint32_t * ks
);
void generate_shares_encrypt(
	unsigned char *m , mask_m_uint32_t * ms , const unsigned long long mlen ,
	unsigned char * ad , mask_ad_uint32_t * ads , const unsigned long long adlen ,
	unsigned char * npub , mask_npub_uint32_t * npubs ,
	unsigned char *k , mask_key_uint32_t * ks
);
void generate_shares_decrypt (
	unsigned char *c , mask_c_uint32_t * cs , const unsigned long long clen ,
	unsigned char * ad , mask_ad_uint32_t * ads , const unsigned long long adlen ,
	unsigned char * npub , mask_npub_uint32_t * npubs ,
	unsigned char *k , mask_key_uint32_t * ks
);
void combine_shares_encrypt(
	mask_c_uint32_t * cs , unsigned char *c , unsigned long long clen
);

void combine_shares_decrypt(
	mask_m_uint32_t * ms , unsigned char *m , unsigned long long mlen
);
	
#endif

