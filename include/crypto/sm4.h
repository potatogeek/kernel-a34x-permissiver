/* SPDX-License-Identifier: GPL-2.0 */

/*
 * Common values for the SM4 algorithm
 * Copyright (C) 2018 ARM Limited or its affiliates.
<<<<<<< HEAD
=======
 * Copyright (c) 2021 Tianjia Zhang <tianjia.zhang@linux.alibaba.com>
>>>>>>> upstream/android-13
 */

#ifndef _CRYPTO_SM4_H
#define _CRYPTO_SM4_H

#include <linux/types.h>
#include <linux/crypto.h>

#define SM4_KEY_SIZE	16
#define SM4_BLOCK_SIZE	16
#define SM4_RKEY_WORDS	32

<<<<<<< HEAD
struct crypto_sm4_ctx {
=======
struct sm4_ctx {
>>>>>>> upstream/android-13
	u32 rkey_enc[SM4_RKEY_WORDS];
	u32 rkey_dec[SM4_RKEY_WORDS];
};

<<<<<<< HEAD
int crypto_sm4_set_key(struct crypto_tfm *tfm, const u8 *in_key,
		       unsigned int key_len);
int crypto_sm4_expand_key(struct crypto_sm4_ctx *ctx, const u8 *in_key,
			  unsigned int key_len);

void crypto_sm4_encrypt(struct crypto_tfm *tfm, u8 *out, const u8 *in);
void crypto_sm4_decrypt(struct crypto_tfm *tfm, u8 *out, const u8 *in);
=======
/**
 * sm4_expandkey - Expands the SM4 key as described in GB/T 32907-2016
 * @ctx:	The location where the computed key will be stored.
 * @in_key:	The supplied key.
 * @key_len:	The length of the supplied key.
 *
 * Returns 0 on success. The function fails only if an invalid key size (or
 * pointer) is supplied.
 */
int sm4_expandkey(struct sm4_ctx *ctx, const u8 *in_key,
			  unsigned int key_len);

/**
 * sm4_crypt_block - Encrypt or decrypt a single SM4 block
 * @rk:		The rkey_enc for encrypt or rkey_dec for decrypt
 * @out:	Buffer to store output data
 * @in: 	Buffer containing the input data
 */
void sm4_crypt_block(const u32 *rk, u8 *out, const u8 *in);
>>>>>>> upstream/android-13

#endif
