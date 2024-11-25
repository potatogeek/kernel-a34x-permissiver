<<<<<<< HEAD
/* SPDX-License-Identifier: GPL-2.0 */
=======
/* SPDX-License-Identifier: GPL-2.0-or-later */
>>>>>>> upstream/android-13
/*
 * AEGIS common definitions
 *
 * Copyright (c) 2018 Ondrej Mosnacek <omosnacek@gmail.com>
 * Copyright (c) 2018 Red Hat, Inc. All rights reserved.
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version.
=======
>>>>>>> upstream/android-13
 */

#ifndef _CRYPTO_AEGIS_H
#define _CRYPTO_AEGIS_H

#include <crypto/aes.h>
<<<<<<< HEAD
=======
#include <linux/bitops.h>
>>>>>>> upstream/android-13
#include <linux/types.h>

#define AEGIS_BLOCK_SIZE 16

union aegis_block {
	__le64 words64[AEGIS_BLOCK_SIZE / sizeof(__le64)];
	__le32 words32[AEGIS_BLOCK_SIZE / sizeof(__le32)];
	u8 bytes[AEGIS_BLOCK_SIZE];
};

<<<<<<< HEAD
#define AEGIS_BLOCK_ALIGN (__alignof__(union aegis_block))
#define AEGIS_ALIGNED(p) IS_ALIGNED((uintptr_t)p, AEGIS_BLOCK_ALIGN)

static const union aegis_block crypto_aegis_const[2] = {
	{ .words64 = {
		cpu_to_le64(U64_C(0x0d08050302010100)),
		cpu_to_le64(U64_C(0x6279e99059372215)),
	} },
	{ .words64 = {
		cpu_to_le64(U64_C(0xf12fc26d55183ddb)),
		cpu_to_le64(U64_C(0xdd28b57342311120)),
	} },
};

static void crypto_aegis_block_xor(union aegis_block *dst,
				   const union aegis_block *src)
=======
struct aegis_state;

extern int aegis128_have_aes_insn;

#define AEGIS_BLOCK_ALIGN (__alignof__(union aegis_block))
#define AEGIS_ALIGNED(p) IS_ALIGNED((uintptr_t)p, AEGIS_BLOCK_ALIGN)

bool crypto_aegis128_have_simd(void);
void crypto_aegis128_update_simd(struct aegis_state *state, const void *msg);
void crypto_aegis128_init_simd(struct aegis_state *state,
			       const union aegis_block *key,
			       const u8 *iv);
void crypto_aegis128_encrypt_chunk_simd(struct aegis_state *state, u8 *dst,
					const u8 *src, unsigned int size);
void crypto_aegis128_decrypt_chunk_simd(struct aegis_state *state, u8 *dst,
					const u8 *src, unsigned int size);
int crypto_aegis128_final_simd(struct aegis_state *state,
			       union aegis_block *tag_xor,
			       unsigned int assoclen,
			       unsigned int cryptlen,
			       unsigned int authsize);

static __always_inline void crypto_aegis_block_xor(union aegis_block *dst,
						   const union aegis_block *src)
>>>>>>> upstream/android-13
{
	dst->words64[0] ^= src->words64[0];
	dst->words64[1] ^= src->words64[1];
}

<<<<<<< HEAD
static void crypto_aegis_block_and(union aegis_block *dst,
				   const union aegis_block *src)
=======
static __always_inline void crypto_aegis_block_and(union aegis_block *dst,
						   const union aegis_block *src)
>>>>>>> upstream/android-13
{
	dst->words64[0] &= src->words64[0];
	dst->words64[1] &= src->words64[1];
}

<<<<<<< HEAD
static void crypto_aegis_aesenc(union aegis_block *dst,
				const union aegis_block *src,
				const union aegis_block *key)
{
	const u8  *s  = src->bytes;
	const u32 *t0 = crypto_ft_tab[0];
	const u32 *t1 = crypto_ft_tab[1];
	const u32 *t2 = crypto_ft_tab[2];
	const u32 *t3 = crypto_ft_tab[3];
	u32 d0, d1, d2, d3;

	d0 = t0[s[ 0]] ^ t1[s[ 5]] ^ t2[s[10]] ^ t3[s[15]];
	d1 = t0[s[ 4]] ^ t1[s[ 9]] ^ t2[s[14]] ^ t3[s[ 3]];
	d2 = t0[s[ 8]] ^ t1[s[13]] ^ t2[s[ 2]] ^ t3[s[ 7]];
	d3 = t0[s[12]] ^ t1[s[ 1]] ^ t2[s[ 6]] ^ t3[s[11]];
=======
static __always_inline void crypto_aegis_aesenc(union aegis_block *dst,
						const union aegis_block *src,
						const union aegis_block *key)
{
	const u8  *s  = src->bytes;
	const u32 *t = crypto_ft_tab[0];
	u32 d0, d1, d2, d3;

	d0 = t[s[ 0]] ^ rol32(t[s[ 5]], 8) ^ rol32(t[s[10]], 16) ^ rol32(t[s[15]], 24);
	d1 = t[s[ 4]] ^ rol32(t[s[ 9]], 8) ^ rol32(t[s[14]], 16) ^ rol32(t[s[ 3]], 24);
	d2 = t[s[ 8]] ^ rol32(t[s[13]], 8) ^ rol32(t[s[ 2]], 16) ^ rol32(t[s[ 7]], 24);
	d3 = t[s[12]] ^ rol32(t[s[ 1]], 8) ^ rol32(t[s[ 6]], 16) ^ rol32(t[s[11]], 24);
>>>>>>> upstream/android-13

	dst->words32[0] = cpu_to_le32(d0) ^ key->words32[0];
	dst->words32[1] = cpu_to_le32(d1) ^ key->words32[1];
	dst->words32[2] = cpu_to_le32(d2) ^ key->words32[2];
	dst->words32[3] = cpu_to_le32(d3) ^ key->words32[3];
}

#endif /* _CRYPTO_AEGIS_H */
