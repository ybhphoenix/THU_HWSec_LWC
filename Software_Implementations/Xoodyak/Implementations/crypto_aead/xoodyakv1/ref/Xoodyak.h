/*
Implementation by Shuohang Peng 

Thanks for Ronny Van Keer's pioneering implementation of the Xoodyak C code,
our Xoodyak software protection implementation was adapted from his/her reference code. 

The reference link for the original C code is as follows:
https://csrc.nist.gov/Projects/lightweight-cryptography/finalists
*/

#ifndef _Xoodyak_h_
#define _Xoodyak_h_

#include <stddef.h>
#include "Xoodoo-SnP.h"
#include "Cyclist.h"

#define Xoodyak_f_bPrime    48
#define Xoodyak_Rhash       16
#define Xoodyak_Rkin        44
#define Xoodyak_Rkout       24
#define Xoodyak_lRatchet    16


KCP_DeclareCyclistStructure(Xoodyak, Xoodoo_stateSizeInBytes, Xoodoo_stateAlignment)  //Xoodoo_stateAlignment 4    Xoodoo_stateSizeInBytes 48
KCP_DeclareCyclistFunctions(Xoodyak)

#endif
