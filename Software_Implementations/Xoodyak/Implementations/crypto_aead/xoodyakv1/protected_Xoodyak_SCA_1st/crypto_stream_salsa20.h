#ifndef crypto_stream_salsa20_h
#define crypto_stream_salsa20_h

#ifdef __cplusplus
extern "C" {
#endif

extern int crypto_stream(unsigned char *c,unsigned long long clen,
  const unsigned char *n,
  const unsigned char *k);

#ifdef __cplusplus
}
#endif

#endif
