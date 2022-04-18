#include <randomseed.h>
#include "StrOp.h"
#include "stm32f3_hal.h"

#define Max_Crypto_Byte 32

void Init_Seed()
{
	char temp;
   int i, flag = 1;
    
	while(flag)
    {
        temp = getch();
        if(temp == 's')
        {
            for(i = 0; i < Max_Crypto_Byte;)
            {
                seed[i] = getch();
                i++;
            }
            flag = 0;
        }
    }
	
}

void kernelrandombytes(unsigned char *x,unsigned long long xlen)
{
	int i = xlen - 1;
	
	Init_Seed();
	
    while (i >= 0) {
		x[i] = (unsigned char)seed[i];
		i--;
  }
}
