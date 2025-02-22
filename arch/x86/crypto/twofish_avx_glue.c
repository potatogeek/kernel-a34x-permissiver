<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> upstream/android-13
/*
 * Glue Code for AVX assembler version of Twofish Cipher
 *
 * Copyright (C) 2012 Johannes Goetzfried
 *     <Johannes.Goetzfried@informatik.stud.uni-erlangen.de>
 *
 * Copyright © 2013 Jussi Kivilinna <jussi.kivilinna@iki.fi>
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307
 * USA
 *
=======
>>>>>>> upstream/android-13
 */

#include <linux/module.h>
#include <linux/types.h>
#include <linux/crypto.h>
#include <linux/err.h>
#include <crypto/algapi.h>
#include <crypto/internal/simd.h>
#include <crypto/twofish.h>
<<<<<<< HEAD
#include <crypto/xts.h>
#include <asm/crypto/glue_helper.h>
#include <asm/crypto/twofish.h>
=======

#include "twofish.h"
#include "ecb_cbc_helpers.h"
>>>>>>> upstream/android-13

#define TWOFISH_PARALLEL_BLOCKS 8

/* 8-way parallel cipher functions */
<<<<<<< HEAD
asmlinkage void twofish_ecb_enc_8way(struct twofish_ctx *ctx, u8 *dst,
				     const u8 *src);
asmlinkage void twofish_ecb_dec_8way(struct twofish_ctx *ctx, u8 *dst,
				     const u8 *src);

asmlinkage void twofish_cbc_dec_8way(struct twofish_ctx *ctx, u8 *dst,
				     const u8 *src);
asmlinkage void twofish_ctr_8way(struct twofish_ctx *ctx, u8 *dst,
				 const u8 *src, le128 *iv);

asmlinkage void twofish_xts_enc_8way(struct twofish_ctx *ctx, u8 *dst,
				     const u8 *src, le128 *iv);
asmlinkage void twofish_xts_dec_8way(struct twofish_ctx *ctx, u8 *dst,
				     const u8 *src, le128 *iv);
=======
asmlinkage void twofish_ecb_enc_8way(const void *ctx, u8 *dst, const u8 *src);
asmlinkage void twofish_ecb_dec_8way(const void *ctx, u8 *dst, const u8 *src);

asmlinkage void twofish_cbc_dec_8way(const void *ctx, u8 *dst, const u8 *src);
>>>>>>> upstream/android-13

static int twofish_setkey_skcipher(struct crypto_skcipher *tfm,
				   const u8 *key, unsigned int keylen)
{
	return twofish_setkey(&tfm->base, key, keylen);
}

<<<<<<< HEAD
static inline void twofish_enc_blk_3way(struct twofish_ctx *ctx, u8 *dst,
					const u8 *src)
=======
static inline void twofish_enc_blk_3way(const void *ctx, u8 *dst, const u8 *src)
>>>>>>> upstream/android-13
{
	__twofish_enc_blk_3way(ctx, dst, src, false);
}

<<<<<<< HEAD
static void twofish_xts_enc(void *ctx, u128 *dst, const u128 *src, le128 *iv)
{
	glue_xts_crypt_128bit_one(ctx, dst, src, iv,
				  GLUE_FUNC_CAST(twofish_enc_blk));
}

static void twofish_xts_dec(void *ctx, u128 *dst, const u128 *src, le128 *iv)
{
	glue_xts_crypt_128bit_one(ctx, dst, src, iv,
				  GLUE_FUNC_CAST(twofish_dec_blk));
}

struct twofish_xts_ctx {
	struct twofish_ctx tweak_ctx;
	struct twofish_ctx crypt_ctx;
};

static int xts_twofish_setkey(struct crypto_skcipher *tfm, const u8 *key,
			      unsigned int keylen)
{
	struct twofish_xts_ctx *ctx = crypto_skcipher_ctx(tfm);
	u32 *flags = &tfm->base.crt_flags;
	int err;

	err = xts_verify_key(tfm, key, keylen);
	if (err)
		return err;

	/* first half of xts-key is for crypt */
	err = __twofish_setkey(&ctx->crypt_ctx, key, keylen / 2, flags);
	if (err)
		return err;

	/* second half of xts-key is for tweak */
	return __twofish_setkey(&ctx->tweak_ctx, key + keylen / 2, keylen / 2,
				flags);
}

static const struct common_glue_ctx twofish_enc = {
	.num_funcs = 3,
	.fpu_blocks_limit = TWOFISH_PARALLEL_BLOCKS,

	.funcs = { {
		.num_blocks = TWOFISH_PARALLEL_BLOCKS,
		.fn_u = { .ecb = GLUE_FUNC_CAST(twofish_ecb_enc_8way) }
	}, {
		.num_blocks = 3,
		.fn_u = { .ecb = GLUE_FUNC_CAST(twofish_enc_blk_3way) }
	}, {
		.num_blocks = 1,
		.fn_u = { .ecb = GLUE_FUNC_CAST(twofish_enc_blk) }
	} }
};

static const struct common_glue_ctx twofish_ctr = {
	.num_funcs = 3,
	.fpu_blocks_limit = TWOFISH_PARALLEL_BLOCKS,

	.funcs = { {
		.num_blocks = TWOFISH_PARALLEL_BLOCKS,
		.fn_u = { .ctr = GLUE_CTR_FUNC_CAST(twofish_ctr_8way) }
	}, {
		.num_blocks = 3,
		.fn_u = { .ctr = GLUE_CTR_FUNC_CAST(twofish_enc_blk_ctr_3way) }
	}, {
		.num_blocks = 1,
		.fn_u = { .ctr = GLUE_CTR_FUNC_CAST(twofish_enc_blk_ctr) }
	} }
};

static const struct common_glue_ctx twofish_enc_xts = {
	.num_funcs = 2,
	.fpu_blocks_limit = TWOFISH_PARALLEL_BLOCKS,

	.funcs = { {
		.num_blocks = TWOFISH_PARALLEL_BLOCKS,
		.fn_u = { .xts = GLUE_XTS_FUNC_CAST(twofish_xts_enc_8way) }
	}, {
		.num_blocks = 1,
		.fn_u = { .xts = GLUE_XTS_FUNC_CAST(twofish_xts_enc) }
	} }
};

static const struct common_glue_ctx twofish_dec = {
	.num_funcs = 3,
	.fpu_blocks_limit = TWOFISH_PARALLEL_BLOCKS,

	.funcs = { {
		.num_blocks = TWOFISH_PARALLEL_BLOCKS,
		.fn_u = { .ecb = GLUE_FUNC_CAST(twofish_ecb_dec_8way) }
	}, {
		.num_blocks = 3,
		.fn_u = { .ecb = GLUE_FUNC_CAST(twofish_dec_blk_3way) }
	}, {
		.num_blocks = 1,
		.fn_u = { .ecb = GLUE_FUNC_CAST(twofish_dec_blk) }
	} }
};

static const struct common_glue_ctx twofish_dec_cbc = {
	.num_funcs = 3,
	.fpu_blocks_limit = TWOFISH_PARALLEL_BLOCKS,

	.funcs = { {
		.num_blocks = TWOFISH_PARALLEL_BLOCKS,
		.fn_u = { .cbc = GLUE_CBC_FUNC_CAST(twofish_cbc_dec_8way) }
	}, {
		.num_blocks = 3,
		.fn_u = { .cbc = GLUE_CBC_FUNC_CAST(twofish_dec_blk_cbc_3way) }
	}, {
		.num_blocks = 1,
		.fn_u = { .cbc = GLUE_CBC_FUNC_CAST(twofish_dec_blk) }
	} }
};

static const struct common_glue_ctx twofish_dec_xts = {
	.num_funcs = 2,
	.fpu_blocks_limit = TWOFISH_PARALLEL_BLOCKS,

	.funcs = { {
		.num_blocks = TWOFISH_PARALLEL_BLOCKS,
		.fn_u = { .xts = GLUE_XTS_FUNC_CAST(twofish_xts_dec_8way) }
	}, {
		.num_blocks = 1,
		.fn_u = { .xts = GLUE_XTS_FUNC_CAST(twofish_xts_dec) }
	} }
};

static int ecb_encrypt(struct skcipher_request *req)
{
	return glue_ecb_req_128bit(&twofish_enc, req);
=======
static int ecb_encrypt(struct skcipher_request *req)
{
	ECB_WALK_START(req, TF_BLOCK_SIZE, TWOFISH_PARALLEL_BLOCKS);
	ECB_BLOCK(TWOFISH_PARALLEL_BLOCKS, twofish_ecb_enc_8way);
	ECB_BLOCK(3, twofish_enc_blk_3way);
	ECB_BLOCK(1, twofish_enc_blk);
	ECB_WALK_END();
>>>>>>> upstream/android-13
}

static int ecb_decrypt(struct skcipher_request *req)
{
<<<<<<< HEAD
	return glue_ecb_req_128bit(&twofish_dec, req);
=======
	ECB_WALK_START(req, TF_BLOCK_SIZE, TWOFISH_PARALLEL_BLOCKS);
	ECB_BLOCK(TWOFISH_PARALLEL_BLOCKS, twofish_ecb_dec_8way);
	ECB_BLOCK(3, twofish_dec_blk_3way);
	ECB_BLOCK(1, twofish_dec_blk);
	ECB_WALK_END();
>>>>>>> upstream/android-13
}

static int cbc_encrypt(struct skcipher_request *req)
{
<<<<<<< HEAD
	return glue_cbc_encrypt_req_128bit(GLUE_FUNC_CAST(twofish_enc_blk),
					   req);
=======
	CBC_WALK_START(req, TF_BLOCK_SIZE, -1);
	CBC_ENC_BLOCK(twofish_enc_blk);
	CBC_WALK_END();
>>>>>>> upstream/android-13
}

static int cbc_decrypt(struct skcipher_request *req)
{
<<<<<<< HEAD
	return glue_cbc_decrypt_req_128bit(&twofish_dec_cbc, req);
}

static int ctr_crypt(struct skcipher_request *req)
{
	return glue_ctr_req_128bit(&twofish_ctr, req);
}

static int xts_encrypt(struct skcipher_request *req)
{
	struct crypto_skcipher *tfm = crypto_skcipher_reqtfm(req);
	struct twofish_xts_ctx *ctx = crypto_skcipher_ctx(tfm);

	return glue_xts_req_128bit(&twofish_enc_xts, req,
				   XTS_TWEAK_CAST(twofish_enc_blk),
				   &ctx->tweak_ctx, &ctx->crypt_ctx);
}

static int xts_decrypt(struct skcipher_request *req)
{
	struct crypto_skcipher *tfm = crypto_skcipher_reqtfm(req);
	struct twofish_xts_ctx *ctx = crypto_skcipher_ctx(tfm);

	return glue_xts_req_128bit(&twofish_dec_xts, req,
				   XTS_TWEAK_CAST(twofish_enc_blk),
				   &ctx->tweak_ctx, &ctx->crypt_ctx);
=======
	CBC_WALK_START(req, TF_BLOCK_SIZE, TWOFISH_PARALLEL_BLOCKS);
	CBC_DEC_BLOCK(TWOFISH_PARALLEL_BLOCKS, twofish_cbc_dec_8way);
	CBC_DEC_BLOCK(3, twofish_dec_blk_cbc_3way);
	CBC_DEC_BLOCK(1, twofish_dec_blk);
	CBC_WALK_END();
>>>>>>> upstream/android-13
}

static struct skcipher_alg twofish_algs[] = {
	{
		.base.cra_name		= "__ecb(twofish)",
		.base.cra_driver_name	= "__ecb-twofish-avx",
		.base.cra_priority	= 400,
		.base.cra_flags		= CRYPTO_ALG_INTERNAL,
		.base.cra_blocksize	= TF_BLOCK_SIZE,
		.base.cra_ctxsize	= sizeof(struct twofish_ctx),
		.base.cra_module	= THIS_MODULE,
		.min_keysize		= TF_MIN_KEY_SIZE,
		.max_keysize		= TF_MAX_KEY_SIZE,
		.setkey			= twofish_setkey_skcipher,
		.encrypt		= ecb_encrypt,
		.decrypt		= ecb_decrypt,
	}, {
		.base.cra_name		= "__cbc(twofish)",
		.base.cra_driver_name	= "__cbc-twofish-avx",
		.base.cra_priority	= 400,
		.base.cra_flags		= CRYPTO_ALG_INTERNAL,
		.base.cra_blocksize	= TF_BLOCK_SIZE,
		.base.cra_ctxsize	= sizeof(struct twofish_ctx),
		.base.cra_module	= THIS_MODULE,
		.min_keysize		= TF_MIN_KEY_SIZE,
		.max_keysize		= TF_MAX_KEY_SIZE,
		.ivsize			= TF_BLOCK_SIZE,
		.setkey			= twofish_setkey_skcipher,
		.encrypt		= cbc_encrypt,
		.decrypt		= cbc_decrypt,
<<<<<<< HEAD
	}, {
		.base.cra_name		= "__ctr(twofish)",
		.base.cra_driver_name	= "__ctr-twofish-avx",
		.base.cra_priority	= 400,
		.base.cra_flags		= CRYPTO_ALG_INTERNAL,
		.base.cra_blocksize	= 1,
		.base.cra_ctxsize	= sizeof(struct twofish_ctx),
		.base.cra_module	= THIS_MODULE,
		.min_keysize		= TF_MIN_KEY_SIZE,
		.max_keysize		= TF_MAX_KEY_SIZE,
		.ivsize			= TF_BLOCK_SIZE,
		.chunksize		= TF_BLOCK_SIZE,
		.setkey			= twofish_setkey_skcipher,
		.encrypt		= ctr_crypt,
		.decrypt		= ctr_crypt,
	}, {
		.base.cra_name		= "__xts(twofish)",
		.base.cra_driver_name	= "__xts-twofish-avx",
		.base.cra_priority	= 400,
		.base.cra_flags		= CRYPTO_ALG_INTERNAL,
		.base.cra_blocksize	= TF_BLOCK_SIZE,
		.base.cra_ctxsize	= sizeof(struct twofish_xts_ctx),
		.base.cra_module	= THIS_MODULE,
		.min_keysize		= 2 * TF_MIN_KEY_SIZE,
		.max_keysize		= 2 * TF_MAX_KEY_SIZE,
		.ivsize			= TF_BLOCK_SIZE,
		.setkey			= xts_twofish_setkey,
		.encrypt		= xts_encrypt,
		.decrypt		= xts_decrypt,
=======
>>>>>>> upstream/android-13
	},
};

static struct simd_skcipher_alg *twofish_simd_algs[ARRAY_SIZE(twofish_algs)];

static int __init twofish_init(void)
{
	const char *feature_name;

	if (!cpu_has_xfeatures(XFEATURE_MASK_SSE | XFEATURE_MASK_YMM, &feature_name)) {
		pr_info("CPU feature '%s' is not supported.\n", feature_name);
		return -ENODEV;
	}

	return simd_register_skciphers_compat(twofish_algs,
					      ARRAY_SIZE(twofish_algs),
					      twofish_simd_algs);
}

static void __exit twofish_exit(void)
{
	simd_unregister_skciphers(twofish_algs, ARRAY_SIZE(twofish_algs),
				  twofish_simd_algs);
}

module_init(twofish_init);
module_exit(twofish_exit);

MODULE_DESCRIPTION("Twofish Cipher Algorithm, AVX optimized");
MODULE_LICENSE("GPL");
MODULE_ALIAS_CRYPTO("twofish");
