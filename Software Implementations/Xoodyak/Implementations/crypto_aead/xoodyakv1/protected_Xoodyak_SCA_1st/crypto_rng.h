#define crypto_rng_OUTPUTBYTES 736
#define crypto_rng_KEYBYTES 32
#define crypto_rng_NONCEBYTES 8

#ifndef crypto_rng_h
#define crypto_rng_h

#ifdef __cplusplus
extern "C" {
#endif

extern int crypto_rng(unsigned char *r, unsigned char *n, const unsigned char *g );
#ifdef __cplusplus
}
#endif

#endif
