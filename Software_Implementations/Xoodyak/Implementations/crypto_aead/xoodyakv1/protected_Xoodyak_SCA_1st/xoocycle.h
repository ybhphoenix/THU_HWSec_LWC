#include <stddef.h>
#include <stdint.h>

#define XOOCYCLE_SPONGE 48

#define FOR(i, len) for ((i) = 0; (i) < (len); (i)++)

typedef uint8_t u8;
typedef uint32_t u32;
typedef size_t size;
typedef enum { xoocycle_up, xoocycle_down } xoocycle_phase;
typedef enum { xoocycle_hash, xoocycle_keyed } xoocycle_mode;
typedef struct
{
  u8 sponge[XOOCYCLE_SPONGE];
  xoocycle_phase phase;
  xoocycle_mode mode;
  size absorb;
  size squeeze;
} xoocycle;


extern void xoocycle_cyclist(xoocycle *cyc1, xoocycle *cyc0, u8 *k1, u8 *k0, size k_len,
                             u8 *id1, u8 *id0, size id_len,
                             u8 *counter, size counter_len);

extern void xoocycle_absorb(xoocycle *cyc1, xoocycle *cyc0, u8 *input1, u8 *input0, size len);

extern void xoocycle_encrypt(xoocycle *cyc1, xoocycle *cyc0, u8 *plain1, u8 *plain0, size len);

extern void xoocycle_decrypt(xoocycle *cyc1, xoocycle *cyc0, u8 *cipher1, u8 *cipher0, size len);

extern void xoocycle_squeeze(xoocycle *cyc1, xoocycle *cyc0, u8 *output1, u8 *output0, size len);

extern void xoocycle_squeeze_key(xoocycle *cyc1, xoocycle *cyc0, u8 *output1, u8 *output0, size len);

extern void xoocycle_ratchet(xoocycle *cyc1, xoocycle *cyc0);

extern void xoocycle_erase_u8(void *delenda, size len);

extern void xoocycle_erase(xoocycle *cyc1, xoocycle *cyc0);
