#ifndef core_h
#define core_h

#ifdef __cplusplus
extern "C" {
#endif

extern int crypto_core(
        unsigned char *out,
  const unsigned char *in,
  const unsigned char *k,
  const unsigned char *c
);
  #ifdef __cplusplus
}
#endif

#endif
