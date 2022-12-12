#ifndef storerand_h
#define storerand_h

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
    
extern unsigned char randombytes48_1[48];
extern unsigned char randombytes48_2[48];
extern unsigned char randombytes48_3[48];
extern unsigned char randombytes48_4[48];
//extern unsigned char randombytes32[32];
    
// extern uint32_t z12[73];
// extern uint32_t z13[73];
// extern uint32_t z23[73];
// extern uint32_t rs0[73];
// extern uint32_t rs1[73];
// extern uint32_t rs2[73];
// extern uint32_t rs3[73];
extern unsigned char randombytes16[2024];

void Get_Encrypt_Random();
void Close_Random();
  #ifdef __cplusplus
}
#endif

#endif
