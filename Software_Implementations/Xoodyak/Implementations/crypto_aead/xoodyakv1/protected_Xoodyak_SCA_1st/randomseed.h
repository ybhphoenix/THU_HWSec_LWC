/*
Implementation by Shuohang Peng 

Thanks for Ronny Van Keer's pioneering implementation of the Xoodyak C code,
our Xoodyak software protection implementation was adapted from his/her reference code. 

The reference link for the original C code is as follows:
https://csrc.nist.gov/Projects/lightweight-cryptography/finalists
*/

#ifndef randomseed_h
#define randomseed_h
#include <stdint.h> 

#ifdef __cplusplus
extern "C" {
#endif

extern uint8_t seed[48];
  #ifdef __cplusplus
}
#endif

#endif
