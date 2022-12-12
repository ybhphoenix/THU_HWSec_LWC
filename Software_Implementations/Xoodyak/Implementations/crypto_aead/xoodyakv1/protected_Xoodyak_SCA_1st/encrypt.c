/*
Implementation by Shuohang Peng 

Thanks for Sean B. Palmer's pioneering implementation of the Xoodyak C code,
our Xoodyak software protection implementation was adapted from his/her reference code. 

The reference link for the original C code is as follows:
https://github.com/sbp/xoocycle
*/

#include "api.h"
#include "xoocycle.h"
#include "ShareOp.h" 
#include "storerand.h"

#if !defined(CRYPTO_KEYBYTES)
    #define CRYPTO_KEYBYTES     16
#endif
#if !defined(CRYPTO_NPUBBYTES)
    #define CRYPTO_NPUBBYTES    16
#endif

#define    TAGLEN        16

#define CRYPTO_KEYWORDS 4  //4
#define CRYPTO_NPUBWORDS 4  //4


int crypto_aead_encrypt_shared(
    mask_c_uint32_t * cs , unsigned long long * clen ,
    mask_m_uint32_t * ms , unsigned long long mlen ,
    mask_ad_uint32_t * ads , unsigned long long adlen ,
    mask_npub_uint32_t * npubs ,
    mask_key_uint32_t * ks
)
{
    xoocycle   instance0;
    xoocycle    instance1;

    unsigned char empty[1] = {0};

    unsigned char m1[NUM_BYTES_M], m0[NUM_BYTES_M];
    unsigned char c1[NUM_BYTES_C], c0[NUM_BYTES_C];
    unsigned char ad1[NUM_BYTES_AD], ad0[NUM_BYTES_AD];
    unsigned char k1[CRYPTO_KEYBYTES], k0[CRYPTO_KEYBYTES];
    unsigned char npub1[CRYPTO_NPUBBYTES], npub0[CRYPTO_NPUBBYTES];
    unsigned char tmp[TAGLEN] = {0};
    
    uint32_t i;
    uint32_t tmpw[4] = {0};
    uint32_t m_wlen = (mlen * 8) / 32;
    uint32_t ad_wlen = (adlen * 8) / 32;
    uint32_t c_wlen, c_blen;

    if(((mlen * 8) % 32) > 0)
	{
		m_wlen++;
	}

	if(((adlen * 8) % 32) > 0)
	{
		ad_wlen++;
	}

    mask_wordstobytes_m(m0, mlen, ms, 0, m_wlen);
    mask_wordstobytes_m(m1, mlen, ms, 1, m_wlen);

    mask_wordstobytes_ad(ad0, adlen, ads, 0, ad_wlen);
    mask_wordstobytes_ad(ad1, adlen, ads, 1, ad_wlen);

    mask_wordstobytes_key(k0, CRYPTO_KEYBYTES, ks, 0, CRYPTO_KEYWORDS);
    mask_wordstobytes_key(k1, CRYPTO_KEYBYTES, ks, 1, CRYPTO_KEYWORDS);

    mask_wordstobytes_npub(npub0, CRYPTO_NPUBBYTES, npubs, 0, CRYPTO_NPUBWORDS);
    mask_wordstobytes_npub(npub1, CRYPTO_NPUBBYTES, npubs, 1, CRYPTO_NPUBWORDS);


    xoocycle_cyclist(&instance1, &instance0, k1, k0, CRYPTO_KEYBYTES, npub1, npub0, CRYPTO_NPUBBYTES, empty, 0);
    xoocycle_absorb(&instance1, &instance0, ad1, ad0, (size_t)adlen);
    xoocycle_encrypt(&instance1, &instance0, m1, m0, (size_t)mlen);
    xoocycle_squeeze(&instance1, &instance0, c1, c0, TAGLEN);
    
    
    
//     xoocycle_erase(&instance1, &instance0);

// if you want to check the output 
    *clen = TAGLEN;

	 c_blen = *clen;
	c_wlen = (c_blen * 8) / 32;
	if (((c_blen * 8) % 32) > 0)
	{
		 c_wlen++;
	}
    
    mask_bytestowords_c(cs, 0, c_wlen, c0);
    mask_bytestowords_c(cs, 1, c_wlen, c1);

    
    return 0;
}

void generate_shares_encrypt(
	unsigned char *m , mask_m_uint32_t * ms , unsigned long long mlen ,
	unsigned char * ad , mask_ad_uint32_t * ads , unsigned long long adlen ,
	unsigned char * npub , mask_npub_uint32_t * npubs ,
	unsigned char *k , mask_key_uint32_t * ks
)
{
	unsigned char tempbytes[NUM_BYTES_C] = {0};//= {0xab, 0xf9, 0x73, 0x9a, 0x41, 0x63, 0x5d, 0x91, 0xf0, 0x71, 0x1e, 0x18, 0x3f, 0x67, 0xb7, 0x60, 0xae, 0x51, 0x3a, 0xca, 0x64, 0x1f, 0x2f, 0xe6, 0xb3, 0x30, 0xd7, 0x8b, 0x10, 0xe5, 0x3b, 0x3f, 0xc3, 0x3e, 0x87, 0x0c, 0x99, 0x34, 0x4b, 0xaf, 0xeb, 0x98, 0x3f, 0x70, 0x98, 0xce, 0xd8, 0x60};
	uint32_t m_words_num = (mlen * 8) / 32;
	uint32_t ad_words_num = (adlen * 8) / 32;

	if(((mlen * 8) % 32) > 0)
	{
		m_words_num++;
	}

	if(((adlen * 8) % 32) > 0)
	{
		ad_words_num++;
	}

	
	mask_init_m(ms);
	mask_init_ad(ads);
	mask_init_npub(npubs);
	mask_init_key(ks);

	//Creat a random share of m
	//randombytes(tempbytes, sizeof(tempbytes));
	memcpy(tempbytes, randombytes48_1, 48);
	mask_bytestowords_m(ms, 0, m_words_num, tempbytes);
    mask_xor_m(ms, m, mlen, 1, 0, m_words_num);

	//Creat a random share of ad
	//randombytes(tempbytes, sizeof(tempbytes));
	memcpy(tempbytes, randombytes48_2, 48);
	mask_bytestowords_ad(ads, 0, ad_words_num, tempbytes);
    mask_xor_ad(ads, ad, adlen, 1, 0, ad_words_num);

	//Creat a random share of npub
	//randombytes(tempbytes, sizeof(tempbytes));
	memcpy(tempbytes, randombytes48_3, 48);
	mask_bytestowords_npub(npubs, 0, NUM_WORDS_NPUB, tempbytes);
    mask_xor_npub(npubs, npub, NUM_BYTES_NPUB, 1, 0, NUM_WORDS_NPUB);

	//Creat a random share of k
	//randombytes(tempbytes, sizeof(tempbytes));
	memcpy(tempbytes, randombytes48_4, 48);
	mask_bytestowords_key(ks, 0, NUM_WORDS_KEY, tempbytes);
    mask_xor_key(ks, k, NUM_BYTES_KEY, 1, 0, NUM_WORDS_KEY);
}

void generate_shares_decrypt (
	unsigned char *c , mask_c_uint32_t * cs , unsigned long long clen ,
	unsigned char * ad , mask_ad_uint32_t * ads , unsigned long long adlen ,
	unsigned char * npub , mask_npub_uint32_t * npubs ,
	unsigned char *k , mask_key_uint32_t * ks )
{
	unsigned char tempbytes[NUM_BYTES_C];
	uint32_t c_words_num = (clen * 8) / 32;
	uint32_t ad_words_num = (adlen * 8) / 32;

	if(((clen * 8) % 32) > 0)
	{
		c_words_num++;
	}
	if(((adlen * 8) % 32) > 0)
	{
		ad_words_num++;
	}

	mask_init_c(cs);
	mask_init_ad(ads);
	mask_init_npub(npubs);
	mask_init_key(ks);

	//Creat a random share of c
	memcpy(tempbytes, randombytes48_1, 48);
	mask_bytestowords_c(cs, 0, c_words_num, tempbytes);
    mask_xor_c(cs, c, clen, 1, 0, c_words_num);

	//Creat a random share of ad
	memcpy(tempbytes, randombytes48_2, 48);
	mask_bytestowords_ad(ads, 0, ad_words_num, tempbytes);
    mask_xor_ad(ads, ad, adlen, 1, 0, ad_words_num);

	//Creat a random share of npub
	memcpy(tempbytes, randombytes48_3, 48);
	mask_bytestowords_npub(npubs, 0, NUM_WORDS_NPUB, tempbytes);
    mask_xor_npub(npubs, npub, NUM_BYTES_NPUB, 1, 0, NUM_WORDS_NPUB);

	//Creat a random share of k
	memcpy(tempbytes, randombytes48_4, 48);
	mask_bytestowords_key(ks, 0, NUM_WORDS_KEY, tempbytes);
    mask_xor_key(ks, k, NUM_BYTES_KEY, 1, 0, NUM_WORDS_KEY);
}

void combine_shares_encrypt(
	mask_c_uint32_t * cs , unsigned char *c , unsigned long long clen )
{
	uint32_t tempwords[NUM_WORDS_C];
	uint32_t i;
	uint32_t word_num = (clen * 8) / 32;

	if(((clen * 8) % 32) > 0)
	{
		word_num++;
	}

	for ( i = 0; i < word_num; i++)
	{
		tempwords[i] = cs[i].shares[0] ^ cs[i].shares[1];
	}
	fromWordsToBytes_sel(c, tempwords, NUM_WORDS_C);
}

void combine_shares_decrypt(
mask_m_uint32_t * ms , unsigned char *m , unsigned long long mlen )
{
	uint32_t tempwords[NUM_WORDS_M];
	uint32_t i, word_num = (mlen * 8) / 32;

	if(((mlen * 8) % 32) > 0)
	{
		word_num++;
	}
	

	for ( i = 0; i < word_num; i++)
	{
		tempwords[i] = ms[i].shares[0] ^ ms[i].shares[1];
	}
	fromWordsToBytes_sel(m, tempwords, NUM_WORDS_M);
}
