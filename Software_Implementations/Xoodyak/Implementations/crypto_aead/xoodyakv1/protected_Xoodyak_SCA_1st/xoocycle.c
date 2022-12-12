/*
Implementation by Shuohang Peng 

Thanks for Sean B. Palmer's pioneering implementation of the Xoodyak C code,
our Xoodyak software protection implementation was adapted from his/her reference code. 

The reference link for the original C code is as follows:
https://github.com/sbp/xoocycle
*/

#include "xoocycle.h"
#include "storerand.h"
#include <string.h>

#define XOOCYCLE_HASH 16
#define XOOCYCLE_KEYIN 44
#define XOOCYCLE_KEYOUT 24
#define XOOCYCLE_RATCHET 16

#define DOMAIN_DEFAULT 0x00
#define DOMAIN_KEY 0x02
#define DOMAIN_ABSORB 0x03
#define DOMAIN_RATCHET 0x10
#define DOMAIN_SQUEEZE 0x40
#define DOMAIN_CRYPT 0x80

#define PERMUTATION xoodoo8
#define ROUNDS 12
#define SPONGE_END ((XOOCYCLE_SPONGE) - 1)

#define ROTR(v, n) (((v) >> (n)) | ((v) << (32 - (n))))
#define SWAP(s, u, v) t = (s)[u], (s)[u] = (s)[v], (s)[v] = t
#define MIN(a, b) (size)((a) < (b) ? (a) : (b))

#define ZERO(i, data, len)                                 \
  FOR((i), (len)) {                                        \
    (data)[i] = 0;                                         \
  }

#define SPLIT(n, len, start, step)                         \
  for ((start) = 0, (step) = MIN((n), (len) - (start));    \
       ((start) == 0) || ((start) < (len));                \
       (start) += (n), (step) = MIN((n), (len) - (start)))

unsigned int Cnt = 0;
unsigned int CntEnc = 0;
int flag = 1;

typedef enum { false, true } boolean;

u8 xoocycle_empty0[1] = {0};
u8 xoocycle_empty1[1] = {0};


static void ISW_AND_3(uint32_t *output, uint32_t tempa2, uint32_t tempa1, uint32_t tempb2, uint32_t tempb1)
{
	uint32_t z12  = 0, z13 = 0, z23 = 0;
	uint32_t z21  = 0, z31 = 0, z32 = 0;
	uint32_t rs0  = 0, rs1 = 0;
	uint32_t rs2  = 0, rs3 = 0;
	uint32_t a1   = 0, b1  = 0;
	uint32_t a2   = 0, b2  = 0;
	uint32_t a3   = 0, b3  = 0;
   uint32_t temp = 0;
	
    
   temp = Cnt * 28;
   
//    memcpy(&temp1, randombytesRefresh, 4);
//    a1 = tempa1 ^ temp1;//(randombytesRefresh[0] | (randombytesRefresh[1] << 8) | (randombytesRefresh[2] << 16) | (randombytesRefresh[3] << 24));
//    a2 = tempa2 ^ temp1;//(randombytesRefresh[0] | (randombytesRefresh[1] << 8) | (randombytesRefresh[2] << 16) | (randombytesRefresh[3] << 24));
//    memcpy(&temp1, randombytesRefresh + 4, 4);
//    b1 = tempb1 ^ temp1;//(randombytesRefresh[4] | (randombytesRefresh[5] << 8) | (randombytesRefresh[6] << 16) | (randombytesRefresh[7] << 24));
//    b2 = tempb2 ^ temp1;//(randombytesRefresh[4] | (randombytesRefresh[5] << 8) | (randombytesRefresh[6] << 16) | (randombytesRefresh[7] << 24));

   memcpy(&z12, randombytes16 + temp    , 4);
   memcpy(&z13, randombytes16 + temp + 4 , 4);
   memcpy(&z23, randombytes16 + temp + 8 , 4);
   memcpy(&rs0, randombytes16 + temp + 12, 4);
   memcpy(&rs1, randombytes16 + temp + 16, 4);
   memcpy(&rs2, randombytes16 + temp + 20, 4);
   memcpy(&rs3, randombytes16 + temp + 24, 4);
    
  if(Cnt >= 72)
    {
        Cnt = 0;
    }
   else
   {
        Cnt++;
   }
   
//     a1 = tempa1 ^ rs0; 
//     a2 = tempa2 ^ rs1; 
//     a3 = rs0 ^ rs1;
    
//     b1 = tempb1 ^ rs2; 
//     b2 = tempb2 ^ rs3; 
//     b3 = rs2 ^ rs3;
//    if((CntEnc == 0) && (flag == 1))
//    {
//        SerialSend(&z12);
//        SerialSend(&z13);
//        SerialSend(&z23);
//        SerialSend(&rs0);
//        SerialSend(&rs1);
//        SerialSend(&rs2);
//        SerialSend(&rs3);
//        //flag = 0;
//    }
   
    
	a1 = tempa1 ^ rs0; a2 = tempa2 ^ rs1; a3 = rs0 ^ rs1;
	b1 = tempb1 ^ rs2; b2 = tempb2 ^ rs3; b3 = rs2 ^ rs3;
	
//	ISW AND Gate
	z21 = (z12 ^ (a1 & b2)) ^ (a2 & b1);
	z31 = (z13 ^ (a1 & b3)) ^ (a3 & b1);
	z32 = (z23 ^ (a2 & b3)) ^ (a3 & b2);
	
	output[0] = (a1 & b1) ^ z12 ^ z13;
	output[1] = (a2 & b2) ^ z21 ^ z23;
	output[2] = (a3 & b3) ^ z31 ^ z32;
	output[0] = ((a3 & b3) ^ (a3 & b2) ^ (a2 & b3));
	output[1] = ((a1 & b1) ^ (a1 & b3) ^ (a3 & b1));
	output[2] = ((a2 & b2) ^ (a2 & b1) ^ (a1 & b2));
	
// 	trigger_low();
    
    //   __asm__ volatile(
    //   "mov %0, #0x00000000; EOR %0, %3, %4; mov %1, #0x00000000; EOR %1, %5, %6; mov %2, #0x00000000; EOR %2, %4, %6":"+r"(a1), "+r"(a2), "+r"(a3):"r"(tempa1), "r"(rs0), "r"(tempa2), "r"(rs1):
    //       );
    //  __asm__ volatile(
    //   "mov %0, #0x00000000; EOR %0, %3, %4; mov %1, #0x00000000; EOR %1, %5, %6; mov %2, #0x00000000; EOR %2, %4, %6":"+r"(b1), "+r"(b2), "+r"(b3):"r"(tempb1), "r"(rs2), "r"(tempb2), "r"(rs3):
    //       );
    
    // __asm__ volatile(
    //  "mov %0, #0x00000000; mov r6, #0x00000000; mov r10, #0x00000000; mov r11, #0x00000000; AND r6, %2, %3; EOR r10, r6, %1; AND r11, %4, %5; EOR %0, r10, r11":"+r"(z21):"r"(z12), "r"(a1), "r"(b2), "r"(a2), "r"(b1):
    //      );
    // __asm__ volatile(
    //  " mov %0, #0x00000000; mov r8, #0x00000000; mov r9, #0x00000000; AND r9, %2, %3; EOR r9, r9, %1; AND r8, %4, %5; EOR %0, r9, r8":"+r"(z31):"r"(z13), "r"(b3), "r"(a1), "r"(b1), "r"(a3):
    //      );
    // __asm__ volatile(
    //  " mov %0, #0x00000000; mov r8, #0x00000000; mov r9, #0x00000000; AND r9, %2, %3; EOR r9, r9, %1; AND r8, %4, %5; EOR %0, r9, r8":"+r"(z32):"r"(z23), "r"(a2), "r"(b3), "r"(a3), "r"(b2):
    //      );
    
    
    // __asm__ volatile(
    //  " mov %0, #0x00000000; mov r11, #0x00000000; mov r12, #0x00000000; AND r11, %1, %2; EOR r12, r11, %3; EOR %0, r12, %4; ":"+r"(output[0]):"r"(a1), "r"(b1), "r"(z12), "r"(z13):
    //      );
    // __asm__ volatile(
    //  " mov %0, #0x00000000; mov r6, #0x00000000; AND r6, %1, %2; EOR r6, r6, %3; EOR %0, r6, %4; ":"+r"(output[1]):"r"(b2), "r"(a2), "r"(z21), "r"(z23):
    //      );
    // __asm__ volatile(
    //  " mov %0, #0x00000000; mov r6, #0x00000000; AND r6, %1, %2; EOR r6, r6, %3; EOR %0, r6, %4; ":"+r"(output[2]):"r"(a3), "r"(b3), "r"(z31), "r"(z32):
    //      );
    
//  TI AND Gate
//     trigger_low();
//     __asm__ volatile(
//      "MOV %0, 0; AND r8, %1, %2; MOV r9, r8; AND r8, %4, %1; EOR r9, r9, r8; AND r8, %3, %2; EOR r9, r9, r8; EOR %0, r9, %5":"+r"(output[0]):"r"(a3), "r"(b3), "r"(a2), "r"(b2),"r"(z12):
//          );
//    __asm__ volatile(
//      "MOV %0, 0; AND r8, %1, %2; MOV r9, r8; AND r8, %4, %1; EOR r9, r9, r8; AND r8, %3, %2; EOR r9, r9, r8; EOR %0, r9, %5":"+r"(output[1]):"r"(a1), "r"(b1), "r"(a3), "r"(b3),"r"(z13):
//          );
//    __asm__ volatile(
//      "MOV %0, 0; MOV r9, 0; AND r8, %1, %2; MOV r9, r8; AND r8, %4, %1; EOR r9, r9, r8; AND r8, %3, %2; EOR r10, r9, r8; EOR r9, r10, %5; EOR %0, r9, %6":"+r"(output[2]):"r"(a2), "r"(b2), "r"(a1), "r"(b1),"r"(z12), "r"(z13):
//          );
// 	trigger_high();
    
    
}


static void xoodoo32(uint32_t * ss32, uint32_t *s32, uint32_t rounds)
{
  uint32_t e[4] = {0}, a = 0, b = 0, c = 0, t = 0, r = 0;//, i;
  uint32_t aa = 0, bb = 0, cc = 0;
  uint32_t output[3] = {0};
  //Round Constant 
  uint32_t k[24] = {120, 52, 576, 160, 384, 22, 112, 60, 832, 144, 320, 24,
               88, 56, 960, 208, 288, 20, 96, 44, 896, 240, 416, 18};

  FOR(r, 12) 
  {
    //FOR(i, 4) 
//       trigger_low();
	{
      e[0] = ROTR(s32[0] ^ s32[4] ^ s32[8], 18),
      e[0] ^= ROTR(e[0], 9);
      
      e[1] = ROTR(s32[1] ^ s32[5] ^ s32[9], 18),
      e[1] ^= ROTR(e[1], 9);
      
      e[2] = ROTR(s32[2] ^ s32[6] ^ s32[10], 18),
      e[2] ^= ROTR(e[2], 9);
      
      e[3] = ROTR(s32[3] ^ s32[7] ^ s32[11], 18),
      e[3] ^= ROTR(e[3], 9);
    }
    
    //FOR(i, 12) 
	{
      //s32[i] ^= e[(i - 1) & 3];
      s32[0]  ^= e[3];
      s32[1]  ^= e[0];
      s32[2]  ^= e[1];
      s32[3]  ^= e[2];
      s32[4]  ^= e[3];
      s32[5]  ^= e[0];
      s32[6]  ^= e[1];
      s32[7]  ^= e[2];
      s32[8]  ^= e[3];
      s32[9]  ^= e[0];
      s32[10] ^= e[1];
      s32[11] ^= e[2];
    }
    SWAP(s32, 7, 4);
    SWAP(s32, 7, 5);
    SWAP(s32, 7, 6);
    
    //Iota
    s32[0] ^= k[(24 - rounds) + r];
    
    /**************************************************/
     e[0] = 0;
     e[1] = 0;
     e[2] = 0;
     e[3] = 0;
//      trigger_high();
    //FOR(i, 4) 
	{
//      e[i] = ROTR(ss32[i] ^ ss32[i + 4] ^ ss32[i + 8], 18),
//      e[i] ^= ROTR(e[i], 9);
		e[0] = ROTR(ss32[0] ^ ss32[4] ^ ss32[8], 18),
        e[0] ^= ROTR(e[0], 9);
        
        e[1] = ROTR(ss32[1] ^ ss32[5] ^ ss32[9], 18),
        e[1] ^= ROTR(e[1], 9);
        
        e[2] = ROTR(ss32[2] ^ ss32[6] ^ ss32[10], 18),
        e[2] ^= ROTR(e[2], 9);
        
        e[3] = ROTR(ss32[3] ^ ss32[7] ^ ss32[11], 18),
        e[3] ^= ROTR(e[3], 9);
    }
//    FOR(i, 12) 
	{
//      ss32[i] ^= e[(i - 1) & 3];
	    ss32[0]  ^= e[3];
        ss32[1]  ^= e[0];
        ss32[2]  ^= e[1];
        ss32[3]  ^= e[2];
        ss32[4]  ^= e[3];
        ss32[5]  ^= e[0];
        ss32[6]  ^= e[1];
        ss32[7]  ^= e[2];
        ss32[8]  ^= e[3];
        ss32[9]  ^= e[0];
        ss32[10] ^= e[1];
        ss32[11] ^= e[2];
    }
      
    
      
    SWAP(ss32, 7, 4);
    SWAP(ss32, 7, 5);
    SWAP(ss32, 7, 6);
      
     e[0] = 0;
     e[1] = 0;
     e[2] = 0;
     e[3] = 0;
      
//       trigger_low();
    /**************************************************/
    
    //Chi mixed with rho-west A2 and rho-east A1 A2 z
      
//     trigger_high();
    //FOR(i, 4) 
//       trigger_low();
	{
      a = s32[0]; b = s32[4]; c = ROTR(s32[8], 21);
      aa = ss32[0]; bb = ss32[4]; cc = ROTR(ss32[8], 21);
      
      ISW_AND_3(output, aa, ~a, bb, b);
      s32 [8] = ROTR(output[0] ^ c, 24);
      ss32[8] = ROTR((output[1] ^ output[2]) ^ cc, 24);
      
       output[0] = 0;
	    output[1] = 0;
	    output[2] = 0;
        
      ISW_AND_3(output, aa, a, cc, ~c);
      s32 [4] = ROTR(output[0] ^  b, 31);
      ss32[4] = ROTR((output[1] ^ output[2]) ^ bb, 31);
      
       output[0] = 0;
	    output[1] = 0;
	    output[2] = 0;
        
      ISW_AND_3(output, cc, c, bb, ~b);
      s32 [0] ^= output[0];
      ss32[0] ^= (output[1] ^ output[2]);
      
      a = 0; b = 0; c = 0;
	   aa = 0; bb = 0; cc = 0; 
	   output[0] = 0;
	   output[1] = 0;
	   output[2] = 0;
        
      a = s32[1], b = s32[5], c = ROTR(s32[9], 21);
      aa = ss32[1], bb = ss32[5], cc = ROTR(ss32[9], 21);
      
      ISW_AND_3(output, aa, ~a, bb, b);
      s32 [9] = ROTR(output[0] ^  c, 24);
      ss32[9] = ROTR((output[1] ^ output[2]) ^ cc, 24);
      
       output[0] = 0;
	    output[1] = 0;
	    output[2] = 0;
        
      ISW_AND_3(output, aa, a, cc, ~c);
      s32 [5] = ROTR(output[0] ^  b, 31);
      ss32[5] = ROTR((output[1] ^ output[2]) ^ bb, 31);
      
       output[0] = 0;
	    output[1] = 0;
	    output[2] = 0;
        
      ISW_AND_3(output, cc, c, bb, ~b);
      s32 [1] ^= output[0];
      ss32[1] ^= (output[1] ^ output[2]);
        
      a = 0; b = 0; c = 0;
	   aa = 0; bb = 0; cc = 0; 
	   output[0] = 0;
	   output[1] = 0;
	   output[2] = 0;
      
      a = s32[2], b = s32[6], c = ROTR(s32[10], 21);
      aa = ss32[2], bb = ss32[6], cc = ROTR(ss32[10], 21);
      
      ISW_AND_3(output, aa, ~a, bb, b);
      s32 [10] = ROTR(output[0] ^  c, 24);
      ss32[10] = ROTR((output[1] ^ output[2]) ^ cc, 24);
      
       output[0] = 0;
	    output[1] = 0;
	    output[2] = 0;
        
      ISW_AND_3(output, aa, a, cc, ~c);
      s32 [6] = ROTR(output[0] ^  b, 31);
      ss32[6] = ROTR((output[1] ^ output[2]) ^ bb, 31);
      
       output[0] = 0;
	    output[1] = 0;
	    output[2] = 0;
        
      ISW_AND_3(output, cc, c, bb, ~b);
      s32 [2] ^= output[0];
      ss32[2] ^= (output[1] ^ output[2]);
      
      a = 0; b = 0; c = 0;
      aa = 0; bb = 0; cc = 0; 
      output[0] = 0;
      output[1] = 0;
      output[2] = 0;
        
      a = s32[3], b = s32[7], c = ROTR(s32[11], 21);
      aa = ss32[3], bb = ss32[7], cc = ROTR(ss32[11], 21);
      
      ISW_AND_3(output, aa, ~a, bb, b);
      s32 [11] = ROTR(output[0] ^  c, 24);
      ss32[11] = ROTR((output[1] ^ output[2]) ^ cc, 24);
      
       output[0] = 0;
	    output[1] = 0;
	    output[2] = 0;
        
      ISW_AND_3(output, aa, a, cc, ~c);
      s32 [7] = ROTR(output[0] ^  b, 31);
      ss32[7] = ROTR((output[1] ^ output[2]) ^ bb, 31);
      
       output[0] = 0;
	    output[1] = 0;
	    output[2] = 0;
        
      ISW_AND_3(output, cc, c, bb, ~b);
      s32 [3] ^= output[0];
      ss32[3] ^= (output[1] ^ output[2]);
      
      a = 0; b = 0; c = 0;
	   aa = 0; bb = 0; cc = 0; 
	   output[0] = 0;
	   output[1] = 0;
	   output[2] = 0;
//     trigger_high();
    }
//      trigger_high();
    //rho-east A2
    SWAP(s32, 8, 10);
    SWAP(s32, 9, 11);
    
    SWAP(ss32, 8, 10);
    SWAP(ss32, 9, 11);
//     trigger_high();
  }
}

void xoodoo8(u8 *s81, u8 *s82, u32 rounds)
{
  u32 *s32 = (u32 *)s81;
  u32 *ss32 = (u32 *)s82;

  xoodoo32(s32, ss32, rounds);
}

static void up(xoocycle *cyc1, xoocycle *cyc0, u8 *output1, u8 *output0, size len, u8 domain)
{
  size i = 0;

  cyc0->phase = xoocycle_up;
  cyc1->phase = xoocycle_up;
  if (cyc0->mode != xoocycle_hash) {
    cyc0->sponge[SPONGE_END] ^= domain;
    cyc1->sponge[SPONGE_END] ^= 0;
  }
  
  PERMUTATION(cyc1->sponge, cyc0->sponge, ROUNDS);

//  print8(cyc0->sponge, cyc1->sponge, 48);
  FOR(i, len) {
    output0[i] = cyc0->sponge[i];
    output1[i] = cyc1->sponge[i];
  }
}

static void down(xoocycle *cyc1, xoocycle *cyc0, u8 *input1, u8 *input0, size len, u8 domain)
{
  size i = 0;

  cyc0->phase = xoocycle_down;
  cyc1->phase = xoocycle_down;
  FOR(i, len) {
    cyc0->sponge[i] ^= input0[i];
    cyc1->sponge[i] ^= input1[i];
  }
  cyc0->sponge[len] ^= 0x01;
  cyc1->sponge[len] ^= 0x00;
  if (cyc0->mode == xoocycle_hash) {
    cyc0->sponge[SPONGE_END] ^= (domain & 0x01);
    cyc1->sponge[SPONGE_END] ^= 0;
  } else {
    cyc0->sponge[SPONGE_END] ^= domain;
    cyc1->sponge[SPONGE_END] ^= 0;
  }
}

static void squeeze_any(xoocycle *cyc1, xoocycle *cyc0, u8 *output1, u8 *output0, size len, u8 domain)
{
  size first_len = MIN(len, cyc0->squeeze);
  size subsequent_len = 0;
  size aggregate_len = 0;

  up(cyc1, cyc0, &output1[aggregate_len], &output0[aggregate_len], first_len, domain);
  
  aggregate_len += first_len;
  while (aggregate_len < len) {
    down(cyc1, cyc0, NULL, NULL, 0, DOMAIN_DEFAULT);

    // printf("squeeze_down: ");
    // for(int ii = 0; ii < 16; ii++)
    // {
    //     printf("%lx", cyc1->sponge[ii] ^ cyc0->sponge[ii]);
    // }
    // printf("\n");

    subsequent_len = MIN(len - aggregate_len, cyc0->squeeze);

    up(cyc1, cyc0, &output1[aggregate_len], &output0[aggregate_len], subsequent_len, DOMAIN_DEFAULT);
    // printf("squeeze_up: ");
    // for(int ii = 0; ii < 16; ii++)
    // {
    //     printf("%lx", cyc1->sponge[ii] ^ cyc0->sponge[ii]);
    // }
    // printf("\n");

    aggregate_len += subsequent_len;
  }
}

static void crypto(xoocycle *cyc1, xoocycle *cyc0, u8 *io1, u8 *io0, size len, boolean decrypt)
{
  size start = 0;
  size step = 0;
  size i = 0;
  u8 up_output0[XOOCYCLE_KEYOUT];
  u8 up_output1[XOOCYCLE_KEYOUT];
  u8 down_input0[XOOCYCLE_KEYOUT];
  u8 down_input1[XOOCYCLE_KEYOUT];

  ZERO(i, up_output0, XOOCYCLE_KEYOUT);
  ZERO(i, up_output1, XOOCYCLE_KEYOUT);
  ZERO(i, down_input0, XOOCYCLE_KEYOUT);
  ZERO(i, down_input1, XOOCYCLE_KEYOUT);


  SPLIT(XOOCYCLE_KEYOUT, len, start, step) {
    if (start == 0) {
      up(cyc1, cyc0, &up_output1[0], &up_output0[0], step, DOMAIN_CRYPT);
    } else {
      up(cyc1, cyc0, &up_output1[0], &up_output0[0], step, DOMAIN_DEFAULT);
    }
    
    if (decrypt) {
      FOR(i, step) {
        io0[start + i] ^= up_output0[i];
        io1[start + i] ^= up_output1[i];
        down_input0[i] = io0[start + i];
        down_input1[i] = io1[start + i];
      }
    } else {
      FOR(i, step) {
        down_input0[i] = io0[start + i];
        down_input1[i] = io1[start + i];
        io0[start + i] ^= up_output0[i];
        io1[start + i] ^= up_output1[i];
      }
    }
    // printf("%d: ", cnt++);
    // for(int ii = 0; ii < 44; ii++)
    // {
    //     printf("%lx", io1[ii] ^ io0[ii]);
    // }
    // printf("\n");
    down(cyc1, cyc0, down_input1, down_input0, step, DOMAIN_DEFAULT);

    // printf("%d: ", cnt++);
    // for(int ii = 0; ii < 44; ii++)
    // {
    //     printf("%lx", cyc1->sponge[ii] ^ cyc0->sponge[ii]);
    // }
    // printf("\n");
  }
}

static void absorb_any(xoocycle *cyc1, xoocycle *cyc0, u8 *input1, u8 *input0, size len,
                       size r, u8 domain)
{
  size start = 0;
  size step = 0;

  SPLIT(r, len, start, step) {
    if (cyc0->phase != xoocycle_up) {
      up(cyc1, cyc0, xoocycle_empty1, xoocycle_empty0, 0, DOMAIN_DEFAULT);
    }
    if (start == 0) {
      down(cyc1, cyc0, &input1[start], &input0[start], step, domain);
    } else {
      down(cyc1, cyc0, &input1[start], &input0[start], step, DOMAIN_DEFAULT);
    }
  }
}

static void absorb_key(xoocycle *cyc1, xoocycle *cyc0, u8 *k1, u8 *k0, size k_len,
                       u8 *id1, u8 *id0, size id_len,
                       u8 *counter, size counter_len)
{
  size i = 0;
  u8 kie0[XOOCYCLE_KEYIN] = {0};
  u8 kie1[XOOCYCLE_KEYIN] = {0};
  size kie_len = k_len + id_len + 1;

  cyc0->mode = xoocycle_keyed;
  cyc0->absorb = XOOCYCLE_KEYIN;
  cyc0->squeeze = XOOCYCLE_KEYOUT;
  
  cyc1->mode = xoocycle_keyed;
  cyc1->absorb = XOOCYCLE_KEYIN;
  cyc1->squeeze = XOOCYCLE_KEYOUT;
  FOR(i, k_len) {
    kie0[i] = k0[i];
    kie1[i] = k1[i];
  }
//  print8(kie0, kie1, 48);
  FOR(i, id_len) {
    kie0[i + k_len] = id0[i];
    kie1[i + k_len] = id1[i];
  }
//  print8(kie0, kie1, 48);
  kie0[k_len + id_len] = (u8)(id_len & 0xff);
  kie1[k_len + id_len] = (u8)0;
  
  absorb_any(cyc1, cyc0, kie1, kie0, kie_len, cyc0->absorb, DOMAIN_KEY);
//  print8(cyc0->sponge, cyc1->sponge, 48);
  if (counter_len > 0) {
    absorb_any(cyc1, cyc0, counter, NULL, counter_len, 1, DOMAIN_DEFAULT);
  }
}

extern void xoocycle_cyclist(xoocycle *cyc1, xoocycle *cyc0, u8 *k1, u8 *k0, size k_len,
                             u8 *id1, u8 *id0, size id_len,
                             u8 *counter, size counter_len)
{
  size i = 0;

  cyc0->phase = xoocycle_up;
  cyc1->phase = xoocycle_up;
  
  FOR(i, XOOCYCLE_SPONGE) {
    cyc0->sponge[i] = 0;
    cyc1->sponge[i] = 0;
  }
  cyc0->mode = xoocycle_hash;
  cyc0->absorb = XOOCYCLE_HASH;
  cyc0->squeeze = XOOCYCLE_HASH;
  
  cyc1->mode = xoocycle_hash;
  cyc1->absorb = XOOCYCLE_HASH;
  cyc1->squeeze = XOOCYCLE_HASH;
  if (k_len > 0) {
    absorb_key(cyc1, cyc0, k1, k0, k_len, id1, id0, id_len, counter, counter_len);
  }
}

extern void xoocycle_absorb(xoocycle *cyc1, xoocycle *cyc0, u8 *input1, u8 *input0, size len)
{
  absorb_any(cyc1, cyc0, input1, input0, len, cyc0->absorb, DOMAIN_ABSORB);
}

extern void xoocycle_encrypt(xoocycle *cyc1, xoocycle *cyc0, u8 *plain1, u8 *plain0, size len)
{
  if (cyc0->mode != xoocycle_keyed) {
    return;
  }
  crypto(cyc1, cyc0, plain1, plain0, len, false);
}

extern void xoocycle_decrypt(xoocycle *cyc1, xoocycle *cyc0, u8 *cipher1, u8 *cipher0, size len)
{
  if (cyc0->mode != xoocycle_keyed) {
    return;
  }
  crypto(cyc1, cyc0, cipher1, cipher0, len, true);
}

extern void xoocycle_squeeze(xoocycle *cyc1, xoocycle *cyc0, u8 *output1, u8 *output0, size len)
{
  squeeze_any(cyc1, cyc0, output1, output0, len, DOMAIN_SQUEEZE);
}

extern void xoocycle_squeeze_key(xoocycle *cyc1, xoocycle *cyc0, u8 *output1, u8 *output0, size len)
{
  if (cyc0->mode != xoocycle_keyed) {
    return;
  }
  squeeze_any(cyc1, cyc0, output1, output0, len, DOMAIN_KEY);
}

extern void xoocycle_ratchet(xoocycle *cyc1, xoocycle *cyc0)
{
  size i;
  u8 io0[XOOCYCLE_RATCHET];
  u8 io1[XOOCYCLE_RATCHET];

  ZERO(i, io0, XOOCYCLE_RATCHET);
  ZERO(i, io1, XOOCYCLE_RATCHET);
  if (cyc0->mode != xoocycle_keyed) {
    return;
  }
  squeeze_any(cyc1, cyc0, io1, io0, XOOCYCLE_RATCHET, DOMAIN_RATCHET);
  absorb_any(cyc1, cyc0, io1, io0, XOOCYCLE_RATCHET, cyc0->absorb, DOMAIN_DEFAULT);
}

extern void xoocycle_erase_u8(void *delenda, size len)
{
  volatile u8 *volatile ephemeral = (volatile u8 *volatile)delenda;
  size i = (size)0U;

  while (i < len) {
    ephemeral[i++] = 0U;
  }
}

extern void xoocycle_erase(xoocycle *cyc1, xoocycle *cyc0)
{
  xoocycle_erase_u8(cyc0->sponge, XOOCYCLE_SPONGE);
  xoocycle_erase_u8(cyc1->sponge, XOOCYCLE_SPONGE);
}
