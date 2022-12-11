/*
Implementation by Shuohang Peng 

Thanks for Ronny Van Keer's pioneering implementation of the Xoodyak C code,
our Xoodyak software protection implementation was adapted from his/her reference code. 

The reference link for the original C code is as follows:
https://csrc.nist.gov/Projects/lightweight-cryptography/finalists
*/

#include "storerand.h"
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

static unsigned char init = 0;


//unsigned char randombytes32[32] = {0};
//unsigned char randombytes32[80] = {0};
//unsigned char randombytes32[24] = {0};

unsigned char randombytes48_1[48] = {0};
unsigned char randombytes48_2[48] = {0};
unsigned char randombytes48_3[48] = {0};
unsigned char randombytes48_4[48] = {0};

unsigned char randombytes16[2024] = {0};

// const unsigned char mf[32] = {0x91,0x70,0x57,0x2e,0xbb,0x77,0x56,0x8a,0x7c,0x42,0x36,0x1f,0xe0,0x2c,0x37,0x14,0x8d,0xeb,0x7a,0x1,0x95,0x97,0x50,0x72,0x77,0xfe,0xbb,0xc,0xaf,0xdd,0x1f,0xdd};

// const unsigned char adf[32] = {0x23,0x9a,0x23,0xac,0x9e,0x69,0xd0,0x75,0x4f,0xc0,0x4,0x69,0x18,0xe4,0x7c,0xe8,0xa0,0x32,0x79,0xae,0xd,0x1a,0xcd,0x97,0x2c,0x87,0xe,0xd4,0x8b,0x84,0xb3,0x6d};

// const unsigned char npubf[16] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f};

//unsigned char mf[32] = {0};
//unsigned char adf[32] = {0};
// unsigned char npubf[16] = {0};

//kat 137
//kat 588


void Get_Encrypt_Random()
{
    char temp0;
    int i = 0;
    int flag = 1;
    uint16_t temp = 0;
    
   if (!init) 
   {  
        while(flag)
        {
                flag = 0;
                srand(time(NULL));
        }  
       init = 1;
   } 
   
    
   for(i = 0; i < 24; i++)
   {
      temp = rand(); 
      randombytes48_1[2 * i] = temp & 0xff;
      randombytes48_1[2 * i + 1] = (temp >> 8) & 0xff;
   }
   for(i = 0; i < 24; i++)
   {
      temp = rand(); 
      randombytes48_2[2 * i] = temp & 0xff;
      randombytes48_2[2 * i + 1] = (temp >> 8) & 0xff;
   }
   for(i = 0; i < 24; i++)
   {
      temp = rand(); 
      randombytes48_3[2 * i] = temp & 0xff;
      randombytes48_3[2 * i + 1] = (temp >> 8) & 0xff;
   }
   for(i = 0; i < 24; i++)
   {
      temp = rand(); 
      randombytes48_4[2 * i] = temp & 0xff;
      randombytes48_4[2 * i + 1] = (temp >> 8) & 0xff;
   }
    
   for(i = 0; i < 1012; i++)
   {
      temp = rand(); 
      randombytes16[2 * i] = temp & 0xff;
      randombytes16[2 * i + 1] = (temp >> 8) & 0xff;
   }   
}

void Close_Random()
{
    char temp0;
    int i;
    int flag = 1;
    uint16_t temp;
    
   if (!init) 
   {  
        while(flag)
        {
                flag = 0;
                srand(time(NULL));
        }  
       init = 1;
   } 
    
    for(i = 0; i < 48; i++)
    {
        randombytes48_1[i] = 0;
        randombytes48_2[i] = 0;
        randombytes48_3[i] = 0;
        randombytes48_4[i] = 0;
    }
    for(i = 0; i < 2024; i++)
    {
        randombytes16[i] = 0;

    }
}