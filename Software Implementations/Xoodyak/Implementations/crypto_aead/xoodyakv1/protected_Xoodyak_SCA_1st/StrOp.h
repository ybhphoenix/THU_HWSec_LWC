/*
Implementation by Shuohang Peng 

Thanks for Ronny Van Keer's pioneering implementation of the Xoodyak C code,
our Xoodyak software protection implementation was adapted from his/her reference code. 

The reference link for the original C code is as follows:
https://csrc.nist.gov/Projects/lightweight-cryptography/finalists
*/

#ifndef _StrOp_h_
#define _StrOp_h_

#include <stdint.h>

void StrToBytes(uint8_t *data, char *str, uint8_t strlen);
void BytesToStr(char *str, uint8_t *data, uint8_t dlen);

#endif
