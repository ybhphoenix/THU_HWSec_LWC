/*
Implementation by Shuohang Peng 
*/

#include <stdio.h>
#include <string.h>
#include "crypto_aead.h" 
#include "storerand.h"
#include "xoocycle.h"

#define PRNGON

int main(void)
{

  unsigned int i = 1;
  unsigned char flag_m = 1;
	unsigned int flag = 1;

  uint8_t tmp[16] = {0};

	uint16_t temp0 = 0;
        
	unsigned char c[NUM_BYTES_C] = {0};
	unsigned long long clen;
  unsigned char mr[NUM_BYTES_M] = {0x00, 0x01, 0x02, 0x03};
    
	unsigned long long slen = 4;  //Seed length
	unsigned long long mlen = 4;  //Plaintext length
  unsigned char adr[NUM_BYTES_AD] = {0x00, 0x01, 0x02, 0x03};
	
	unsigned long long adlen = 4;  //AD length
	
	unsigned char npubr[NUM_BYTES_NPUB] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f };
	
	unsigned char k[NUM_BYTES_KEY] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f};
    
	mask_ad_uint32_t ads[NUM_WORDS_AD];
	mask_m_uint32_t ms[NUM_WORDS_M];

	mask_c_uint32_t cs[NUM_WORDS_C];
	mask_key_uint32_t ks[NUM_WORDS_KEY];
	mask_npub_uint32_t npubs[NUM_WORDS_NPUB];
    
	
  #ifdef PRNGON
      Get_Encrypt_Random();
  #endif
  
  #ifdef PRNGOFF
      Close_Random();
  #endif

//generate shares for plaintext, associated data, npub and key
generate_shares_encrypt(mr, ms, mlen, adr, ads, adlen, npubr, npubs, k, ks);

//encrypt
crypto_aead_encrypt_shared(cs, &clen, ms, mlen, ads, adlen, npubs, ks);
  
//combine shares                                
combine_shares_encrypt(cs, c, clen); 

//show results
mask_wordstobytes_c(tmp, 16, cs, 0, 4);
printf("First share : ");
for(i = 0; i < clen; i++)
{
  printf("%lx", tmp[i]);
}
printf("\n");

mask_wordstobytes_c(tmp, 16, cs, 1, 4);
printf("Second share : ");
for(i = 0; i < clen; i++)
{
  printf("%lx", tmp[i]);
}
printf("\n");

printf("Result(%d bytes) : ", clen);
for(i = 0; i < clen; i++)
{
  printf("%lx", c[i]);
}
            
           
}
