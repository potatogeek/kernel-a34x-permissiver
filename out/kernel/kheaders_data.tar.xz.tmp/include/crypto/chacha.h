/* SPDX-License-Identifier: GPL-2.0 */


#ifndef _CRYPTO_CHACHA_H
#define _CRYPTO_CHACHA_H

#include <asm/unaligned.h>
#include <linux/types.h>


#define CHACHA_IV_SIZE		16

#define CHACHA_KEY_SIZE		32
#define CHACHA_BLOCK_SIZE	64

#define CHACHA_STATE_WORDS	(CHACHA_BLOCK_SIZE / sizeof(u32))


#define XCHACHA_IV_SIZE		32

void chacha_block_generic(u32 *state, u8 *stream, int nrounds);
static inline void chacha20_block(u32 *state, u8 *stream)
{
	chacha_block_generic(state, stream, 20);
}

void hchacha_block_arch(const u32 *state, u32 *out, int nrounds);
void hchacha_block_generic(const u32 *state, u32 *out, int nrounds);

static inline void hchacha_block(const u32 *state, u32 *out, int nrounds)
{
	if (IS_ENABLED(CONFIG_CRYPTO_ARCH_HAVE_LIB_CHACHA))
		hchacha_block_arch(state, out, nrounds);
	else
		hchacha_block_generic(state, out, nrounds);
}

void chacha_init_arch(u32 *state, const u32 *key, const u8 *iv);
static inline void chacha_init_generic(u32 *state, const u32 *key, const u8 *iv)
{
	state[0]  = 0x61707865; 
	state[1]  = 0x3320646e; 
	state[2]  = 0x79622d32; 
	state[3]  = 0x6b206574; 
	state[4]  = key[0];
	state[5]  = key[1];
	state[6]  = key[2];
	state[7]  = key[3];
	state[8]  = key[4];
	state[9]  = key[5];
	state[10] = key[6];
	state[11] = key[7];
	state[12] = get_unaligned_le32(iv +  0);
	state[13] = get_unaligned_le32(iv +  4);
	state[14] = get_unaligned_le32(iv +  8);
	state[15] = get_unaligned_le32(iv + 12);
}

static inline void chacha_init(u32 *state, const u32 *key, const u8 *iv)
{
	if (IS_ENABLED(CONFIG_CRYPTO_ARCH_HAVE_LIB_CHACHA))
		chacha_init_arch(state, key, iv);
	else
		chacha_init_generic(state, key, iv);
}

void chacha_crypt_arch(u32 *state, u8 *dst, const u8 *src,
		       unsigned int bytes, int nrounds);
void chacha_crypt_generic(u32 *state, u8 *dst, const u8 *src,
			  unsigned int bytes, int nrounds);

static inline void chacha_crypt(u32 *state, u8 *dst, const u8 *src,
				unsigned int bytes, int nrounds)
{
	if (IS_ENABLED(CONFIG_CRYPTO_ARCH_HAVE_LIB_CHACHA))
		chacha_crypt_arch(state, dst, src, bytes, nrounds);
	else
		chacha_crypt_generic(state, dst, src, bytes, nrounds);
}

static inline void chacha20_crypt(u32 *state, u8 *dst, const u8 *src,
				  unsigned int bytes)
{
	chacha_crypt(state, dst, src, bytes, 20);
}

#endif 
