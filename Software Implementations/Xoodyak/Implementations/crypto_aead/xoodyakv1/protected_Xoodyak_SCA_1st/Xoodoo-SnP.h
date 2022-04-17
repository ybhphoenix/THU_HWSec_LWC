/*
Implementation by Shuohang Peng 

Thanks for Ronny Van Keer's pioneering implementation of the Xoodyak C code,
our Xoodyak software protection implementation was adapted from his/her reference code. 

The reference link for the original C code is as follows:
https://csrc.nist.gov/Projects/lightweight-cryptography/finalists
*/

#ifndef _Xoodoo_SnP_h_
#define _Xoodoo_SnP_h_

/** For the documentation, see SnP-documentation.h.
 */

#define Xoodoo_implementation      "32-bit reference implementation"
#define Xoodoo_stateSizeInBytes    (3*4*4)
#define Xoodoo_stateAlignment      4
#define Xoodoo_HasNround

void Xoodoo_StaticInitialize( void );
void Xoodoo_Initialize(void *state1, void *state0);
void Xoodoo_AddByte(void *state, unsigned char data, unsigned int offset);
void Xoodoo_AddBytes(void *state, unsigned char *data, unsigned int offset, unsigned int length);
void Xoodoo_OverwriteBytes(void *state, unsigned char *data, unsigned int offset, unsigned int length);
void Xoodoo_OverwriteWithZeroes(void *state, unsigned int byteCount);
void Xoodoo_Permute_Nrounds(void *state1, void *state0, unsigned int nrounds);
void Xoodoo_Permute_6rounds(void *state1, void *state0);
void Xoodoo_Permute_12rounds(void *state1, void *state0);
void Xoodoo_ExtractBytes( void *state, unsigned char *data, unsigned int offset, unsigned int length);
void Xoodoo_ExtractAndAddBytes( void *state, unsigned char *input, unsigned char *output, unsigned int offset, unsigned int length);

#endif
