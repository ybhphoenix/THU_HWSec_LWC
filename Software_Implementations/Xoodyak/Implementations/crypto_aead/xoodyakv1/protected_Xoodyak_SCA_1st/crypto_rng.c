#include <string.h>
#include <stdio.h>
#include "stream.h"
#include "crypto_rng.h"

//#define crypto_stream crypto_stream_salsa20
#define KEYBYTES crypto_rng_KEYBYTES
#define NONCEBYTES crypto_rng_NONCEBYTES
#define OUTPUTBYTES crypto_rng_OUTPUTBYTES

#if KEYBYTES != crypto_rng_KEYBYTES
  KEYBYTES mismatch!
#endif

static const unsigned char nonce[NONCEBYTES];

int crypto_rng(
        unsigned char *r, /* random output */
        unsigned char *n, /* new key */
  const unsigned char *g  /* old key */
)
{
  unsigned int i;
  unsigned char x[KEYBYTES + OUTPUTBYTES];
	
	
//	USART_SendData(USART1, '2');  
//	while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET);
	
  i = crypto_stream(x,sizeof x,nonce,g);
	
	
	
  memcpy(n,x,KEYBYTES);
  memcpy(r,x + KEYBYTES,OUTPUTBYTES);
  return 0;
}

/*
int main()
{
  unsigned int i;
  unsigned char c[crypto_rng_OUTPUTBYTES] = {};
  unsigned char k[KEYBYTES] = {};
	const unsigned char m[32] = {0x01, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f};

  crypto_rng(c, k, m);

  for(i = 0; i < crypto_rng_OUTPUTBYTES; i++)
	{
		printf("%02x",c[i]);
	}
	printf("\n");

  return 0;
}*/
