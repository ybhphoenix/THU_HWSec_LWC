/*
Implementation by Shuohang Peng 

Thanks for Ronny Van Keer's pioneering implementation of the Xoodyak C code,
our Xoodyak software protection implementation was adapted from his/her reference code. 

The reference link for the original C code is as follows:
https://csrc.nist.gov/Projects/lightweight-cryptography/finalists
*/

#include <storerand.h>
#include "randombytes.h"

unsigned char randombytes48_1[48];
unsigned char randombytes48_2[48];
unsigned char randombytes48_3[48];
unsigned char randombytes48_4[48];

unsigned char randombytes16[576 * 6];

void Get_Encrypt_Randnom()
{
	randombytes(randombytes48_1, 48);
	randombytes(randombytes48_2, 48);
	randombytes(randombytes48_3, 48);
	randombytes(randombytes48_4, 48);
	
	randombytes(randombytes16, 576 * 6);
}
