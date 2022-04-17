/*
Implementation by Shuohang Peng 

Thanks for Ronny Van Keer's pioneering implementation of the Xoodyak C code,
our Xoodyak software protection implementation was adapted from his/her reference code. 

The reference link for the original C code is as follows:
https://csrc.nist.gov/Projects/lightweight-cryptography/finalists
*/

#include <stdint.h> 

uint8_t seed[48] = {0};
