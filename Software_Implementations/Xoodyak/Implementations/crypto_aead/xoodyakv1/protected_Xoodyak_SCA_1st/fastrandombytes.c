#include <string.h>
#include "kernelrandombytes.h"
#include "crypto_rng.h"

unsigned long long randombytes_calls = 0;
unsigned long long randombytes_bytes = 0;


static unsigned int init = 0;

static unsigned char g[crypto_rng_KEYBYTES];
static unsigned char r[crypto_rng_OUTPUTBYTES];
static unsigned long long pos = crypto_rng_OUTPUTBYTES;

void randombytes(unsigned char *x, unsigned long long xlen)
{
  randombytes_calls += 1;
  randombytes_bytes += xlen;

  if (!init) {  
    kernelrandombytes(g,sizeof(g));		  
    init = 1;
  }
  

#ifdef SIMPLE

  while (xlen > 0) {
    if (pos == crypto_rng_OUTPUTBYTES) {
      crypto_rng(r,g,g);
      pos = 0;
    }
    *x++ = r[pos]; xlen -= 1;
    r[pos++] = 0;
  }

#else /* same output but optimizing copies */

  while (xlen > 0) {
    unsigned long long ready;

	  
    if (pos == crypto_rng_OUTPUTBYTES) {
      while (xlen > crypto_rng_OUTPUTBYTES) {
        crypto_rng(x,g,g);
        x += crypto_rng_OUTPUTBYTES;
        xlen -= crypto_rng_OUTPUTBYTES;
      }
      if (xlen == 0) return;
	  
      crypto_rng(r,g,g);
	  
      pos = 0;
    }
	
		ready = crypto_rng_OUTPUTBYTES - pos;
		if (xlen <= ready) ready = xlen;
		memcpy(x,r + pos,ready);
		memset(r + pos,0,ready);
		x += ready;
		xlen -= ready;
		pos += ready;
	
//	USART_SendData(USART1, (pos >> 8)&0xff);  
//		while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET);
//	USART_SendData(USART1, pos&0xff);  
//		while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET);
  }

#endif

}
/*
int main()
{
  unsigned long long i;
  unsigned long long j;

  for (j = 0;j < 3;++j) {
    randombytes(x,sizeof x);
    for (i = 0;i < 256;++i) freq[i] = 0;
    for (i = 0;i < sizeof x;++i) ++freq[255 & (int) x[i]];
    for (i = 0;i < 256;++i) if (!freq[i]) return 111;
  }

for(i = 0; i < 65536; i++)
{
  printf("%x", x[i]);
}

  return 0;
}
*/
