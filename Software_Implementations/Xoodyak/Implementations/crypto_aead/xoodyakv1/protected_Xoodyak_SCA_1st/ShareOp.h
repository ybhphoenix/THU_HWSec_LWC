#ifndef _ShareOp_h_
#define _ShareOp_h_

#include "share.h"
#include <string.h>

#define tXoodooLane uint32_t

void fromWordsToBytes_sel(unsigned char *state, tXoodooLane *stateAsWords, uint32_t wlen)
{
    unsigned int i, j;

    for(i=0; i<wlen; i++)
        for(j=0; j<sizeof(tXoodooLane); j++)
            state[i*sizeof(tXoodooLane)+j] = (stateAsWords[i] >> (8*j)) & 0xFF;
}

void fromBytesToWords_sel(uint32_t *state, unsigned char *data, uint32_t wlen)
{
	uint32_t i, j;

	for(i = 0; i < wlen; i++) {
        state[i] = 0;
        for(j=0; j<sizeof(tXoodooLane); j++)
            state[i] |= (tXoodooLane)(data[i*sizeof(tXoodooLane)+j]) << (8*j);
    }
}

void mask_wordstobytes_m(unsigned char *des, uint32_t des_len, const mask_m_uint32_t * mask_var, uint32_t share_src, uint32_t var_len)
{
	unsigned char tempbytes[48];
	unsigned int i, j;

	for(i = 0; i < var_len; i++)
        for(j = 0; j < sizeof(uint32_t); j++)
            tempbytes[i * sizeof(uint32_t)+j] = (mask_var[i].shares[share_src] >> (8*j)) & 0xFF;

	memcpy(des, tempbytes, des_len);
}

void mask_wordstobytes_c(unsigned char *des, uint32_t des_len, const mask_c_uint32_t * mask_var, uint32_t share_src, uint32_t var_len)
{
	unsigned char tempbytes[48];
	unsigned int i, j;

	for(i = 0; i < var_len; i++)
        for(j = 0; j < sizeof(uint32_t); j++)
            tempbytes[i * sizeof(uint32_t)+j] = (mask_var[i].shares[share_src] >> (8*j)) & 0xFF;

	memcpy(des, tempbytes, des_len);
}

void mask_wordstobytes_ad(unsigned char *des, uint32_t des_len, const mask_ad_uint32_t * mask_var, uint32_t share_src, uint32_t var_len)
{
	unsigned char tempbytes[48];
	unsigned int i, j;

	for(i = 0; i < var_len; i++)
        for(j = 0; j < sizeof(uint32_t); j++)
            tempbytes[i * sizeof(uint32_t)+j] = (mask_var[i].shares[share_src] >> (8*j)) & 0xFF;

	memcpy(des, tempbytes, des_len);
}

void mask_wordstobytes_npub(unsigned char *des, uint32_t des_len, const mask_npub_uint32_t * mask_var, uint32_t share_src, uint32_t var_len)
{
	unsigned char tempbytes[48];
	unsigned int i, j;

	for(i = 0; i < var_len; i++)
        for(j = 0; j < sizeof(uint32_t); j++)
            tempbytes[i * sizeof(uint32_t)+j] = (mask_var[i].shares[share_src] >> (8*j)) & 0xFF;

	memcpy(des, tempbytes, des_len);
}

void mask_wordstobytes_key(unsigned char *des, uint32_t des_len, const mask_key_uint32_t * mask_var, uint32_t share_src, uint32_t var_len)
{
	unsigned char tempbytes[48];
	unsigned int i, j;

	for(i = 0; i < var_len; i++)
        for(j = 0; j < sizeof(uint32_t); j++)
            tempbytes[i * sizeof(uint32_t)+j] = (mask_var[i].shares[share_src] >> (8*j)) & 0xFF;

	memcpy(des, tempbytes, des_len);
}



void mask_bytestowords_m(mask_m_uint32_t * mask_var, uint32_t share_des, uint32_t var_len, unsigned char *data)
{
	unsigned int i, j;
	/*    from bytes to words    */
	for(i = 0; i < var_len; i++) {
        mask_var[i].shares[share_des] = 0;
        for(j = 0; j < sizeof(uint32_t); j++)  
            mask_var[i].shares[share_des] |= (uint32_t)(data[i*sizeof(uint32_t)+j]) << (8*j);
    }

}
void mask_bytestowords_c(mask_c_uint32_t * mask_var, uint32_t share_des, uint32_t var_len, unsigned char *data)
{
	unsigned int i, j;
	/*    from bytes to words    */
	for(i = 0; i < var_len; i++) {
        mask_var[i].shares[share_des] = 0;
        for(j = 0; j < sizeof(uint32_t); j++)  
            mask_var[i].shares[share_des] |= (uint32_t)(data[i*sizeof(uint32_t)+j]) << (8*j);
    }

}

void mask_bytestowords_ad(mask_ad_uint32_t * mask_var, uint32_t share_des, uint32_t var_len, unsigned char *data)
{
	unsigned int i, j;
	/*    from bytes to words    */
	for(i = 0; i < var_len; i++) {
        mask_var[i].shares[share_des] = 0;
        for(j = 0; j < sizeof(uint32_t); j++)  
            mask_var[i].shares[share_des] |= (uint32_t)(data[i*sizeof(uint32_t)+j]) << (8*j);
    }

}

void mask_bytestowords_npub(mask_npub_uint32_t * mask_var, uint32_t share_des, uint32_t var_len, unsigned char *data)
{
	unsigned int i, j;
	/*    from bytes to words    */
	for(i = 0; i < var_len; i++) {
        mask_var[i].shares[share_des] = 0;
        for(j = 0; j < sizeof(uint32_t); j++)  
            mask_var[i].shares[share_des] |= (uint32_t)(data[i*sizeof(uint32_t)+j]) << (8*j);
    }

}

void mask_bytestowords_key(mask_key_uint32_t * mask_var, uint32_t share_des, uint32_t var_len, unsigned char *data)
{
	unsigned int i, j;
	/*    from bytes to words    */
	for(i = 0; i < var_len; i++) {
        mask_var[i].shares[share_des] = 0;
        for(j = 0; j < sizeof(uint32_t); j++)  
            mask_var[i].shares[share_des] |= (uint32_t)(data[i*sizeof(uint32_t)+j]) << (8*j);
    }

}

/*--------------------------------------*/
/*    mask operation (just for test)    */
/*--------------------------------------*/

void mask_init_m(mask_m_uint32_t * mask_var)
{
	unsigned int i;
	for(i = 0; i < NUM_WORDS_M; i++){
		mask_var[i].shares[0] = 0;
		mask_var[i].shares[1] = 0;
	}
}

void mask_init_ad(mask_ad_uint32_t * mask_var)
{
	unsigned int i;
	for(i = 0; i < NUM_WORDS_AD; i++){
		mask_var[i].shares[0] = 0;
		mask_var[i].shares[1] = 0;
	}
}

void mask_init_npub(mask_npub_uint32_t * mask_var)
{
	unsigned int i;
	for(i = 0; i < NUM_WORDS_NPUB; i++){
		mask_var[i].shares[0] = 0;
		mask_var[i].shares[1] = 0;
	}
}

void mask_init_key(mask_key_uint32_t * mask_var)
{
	unsigned int i;
	for(i = 0; i < NUM_WORDS_KEY; i++){
		mask_var[i].shares[0] = 0;
		mask_var[i].shares[1] = 0;
	}
}

void mask_init_c(mask_c_uint32_t * mask_var)
{
	unsigned int i;
	for(i = 0; i < NUM_WORDS_C; i++){
		mask_var[i].shares[0] = 0;
		mask_var[i].shares[1] = 0;
	}
}

void mask_xor_m(mask_m_uint32_t * mask_var, const unsigned char *data, uint32_t data_len, uint32_t share_des, uint32_t share_src, uint32_t var_len)
{
	unsigned int i;
	unsigned char tempbytes[48];
	uint32_t tempwords[12];

	memcpy(tempbytes, data, data_len);
	fromBytesToWords_sel(tempwords, tempbytes, var_len);
	for(i = 0; i < var_len; i++){
		mask_var[i].shares[share_des] = mask_var[i].shares[share_src] ^ tempwords[i];
	}
}

void mask_xor_c(mask_c_uint32_t * mask_var, const unsigned char *data, uint32_t data_len, uint32_t share_des, uint32_t share_src, uint32_t var_len)
{
	unsigned int i;
	unsigned char tempbytes[48];
	uint32_t tempwords[12];

	memcpy(tempbytes, data, data_len);
	fromBytesToWords_sel(tempwords, tempbytes, var_len);
	for(i = 0; i < var_len; i++){
		mask_var[i].shares[share_des] = mask_var[i].shares[share_src] ^ tempwords[i];
	}
}
void mask_xor_ad(mask_ad_uint32_t * mask_var, const unsigned char *data, uint32_t data_len, uint32_t share_des, uint32_t share_src, uint32_t var_len)
{
	unsigned int i;
	unsigned char tempbytes[48];
	uint32_t tempwords[12];

	memcpy(tempbytes, data, data_len);
	fromBytesToWords_sel(tempwords, tempbytes, var_len);
	for(i = 0; i < var_len; i++){
		mask_var[i].shares[share_des] = mask_var[i].shares[share_src] ^ tempwords[i];
	}
}
void mask_xor_npub(mask_npub_uint32_t * mask_var, const unsigned char *data, uint32_t data_len, uint32_t share_des, uint32_t share_src, uint32_t var_len)
{
	unsigned int i;
	unsigned char tempbytes[48];
	uint32_t tempwords[12];

	memcpy(tempbytes, data, data_len);
	fromBytesToWords_sel(tempwords, tempbytes, var_len);
	for(i = 0; i < var_len; i++){
		mask_var[i].shares[share_des] = mask_var[i].shares[share_src] ^ tempwords[i];
	}
}
void mask_xor_key(mask_key_uint32_t * mask_var, const unsigned char *data, uint32_t data_len, uint32_t share_des, uint32_t share_src, uint32_t var_len)
{
	unsigned int i;
	unsigned char tempbytes[48];
	uint32_t tempwords[12];

	memcpy(tempbytes, data, data_len);
	fromBytesToWords_sel(tempwords, tempbytes, var_len);
	for(i = 0; i < var_len; i++){
		mask_var[i].shares[share_des] = mask_var[i].shares[share_src] ^ tempwords[i];
	}
}

#endif
