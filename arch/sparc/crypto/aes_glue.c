<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> upstream/android-13
/* Glue code for AES encryption optimized for sparc64 crypto opcodes.
 *
 * This is based largely upon arch/x86/crypto/aesni-intel_glue.c
 *
 * Copyright (C) 2008, Intel Corp.
 *    Author: Huang Ying <ying.huang@intel.com>
 *
 * Added RFC4106 AES-GCM support for 128-bit keys under the AEAD
 * interface for 64-bit kernels.
 *    Authors: Adrian Hoban <adrian.hoban@intel.com>
 *             Gabriele Paoloni <gabriele.paoloni@intel.com>
 *             Tadeusz Struk (tadeusz.struk@intel.com)
 *             Aidan O'Mahony (aidan.o.mahony@intel.com)
 *    Copyright (c) 2010, Intel Corporation.
 */

#define pr_fmt(fmt)	KBUILD_MODNAME ": " fmt

#include <linux/crypto.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/mm.h>
#include <linux/types.h>
#include <crypto/algapi.h>
#include <crypto/aes.h>
<<<<<<< HEAD
=======
#include <crypto/internal/skcipher.h>
>>>>>>> upstream/android-13

#include <asm/fpumacro.h>
#include <asm/pstate.h>
#include <asm/elf.h>

#include "opcodes.h"

struct aes_ops {
	void (*encrypt)(const u64 *key, const u32 *input, u32 *output);
	void (*decrypt)(const u64 *key, const u32 *input, u32 *output);
	void (*load_encrypt_keys)(const u64 *key);
	void (*load_decrypt_keys)(const u64 *key);
	void (*ecb_encrypt)(const u64 *key, const u64 *input, u64 *output,
			    unsigned int len);
	void (*ecb_decrypt)(const u64 *key, const u64 *input, u64 *output,
			    unsigned int len);
	void (*cbc_encrypt)(const u64 *key, const u64 *input, u64 *output,
			    unsigned int len, u64 *iv);
	void (*cbc_decrypt)(const u64 *key, const u64 *input, u64 *output,
			    unsigned int len, u64 *iv);
	void (*ctr_crypt)(const u64 *key, const u64 *input, u64 *output,
			  unsigned int len, u64 *iv);
};

struct crypto_sparc64_aes_ctx {
	struct aes_ops *ops;
	u64 key[AES_MAX_KEYLENGTH / sizeof(u64)];
	u32 key_length;
	u32 expanded_key_length;
};

extern void aes_sparc64_encrypt_128(const u64 *key, const u32 *input,
				    u32 *output);
extern void aes_sparc64_encrypt_192(const u64 *key, const u32 *input,
				    u32 *output);
extern void aes_sparc64_encrypt_256(const u64 *key, const u32 *input,
				    u32 *output);

extern void aes_sparc64_decrypt_128(const u64 *key, const u32 *input,
				    u32 *output);
extern void aes_sparc64_decrypt_192(const u64 *key, const u32 *input,
				    u32 *output);
extern void aes_sparc64_decrypt_256(const u64 *key, const u32 *input,
				    u32 *output);

extern void aes_sparc64_load_encrypt_keys_128(const u64 *key);
extern void aes_sparc64_load_encrypt_keys_192(const u64 *key);
extern void aes_sparc64_load_encrypt_keys_256(const u64 *key);

extern void aes_sparc64_load_decrypt_keys_128(const u64 *key);
extern void aes_sparc64_load_decrypt_keys_192(const u64 *key);
extern void aes_sparc64_load_decrypt_keys_256(const u64 *key);

extern void aes_sparc64_ecb_encrypt_128(const u64 *key, const u64 *input,
					u64 *output, unsigned int len);
extern void aes_sparc64_ecb_encrypt_192(const u64 *key, const u64 *input,
					u64 *output, unsigned int len);
extern void aes_sparc64_ecb_encrypt_256(const u64 *key, const u64 *input,
					u64 *output, unsigned int len);

extern void aes_sparc64_ecb_decrypt_128(const u64 *key, const u64 *input,
					u64 *output, unsigned int len);
extern void aes_sparc64_ecb_decrypt_192(const u64 *key, const u64 *input,
					u64 *output, unsigned int len);
extern void aes_sparc64_ecb_decrypt_256(const u64 *key, const u64 *input,
					u64 *output, unsigned int len);

extern void aes_sparc64_cbc_encrypt_128(const u64 *key, const u64 *input,
					u64 *output, unsigned int len,
					u64 *iv);

extern void aes_sparc64_cbc_encrypt_192(const u64 *key, const u64 *input,
					u64 *output, unsigned int len,
					u64 *iv);

extern void aes_sparc64_cbc_encrypt_256(const u64 *key, const u64 *input,
					u64 *output, unsigned int len,
					u64 *iv);

extern void aes_sparc64_cbc_decrypt_128(const u64 *key, const u64 *input,
					u64 *output, unsigned int len,
					u64 *iv);

extern void aes_sparc64_cbc_decrypt_192(const u64 *key, const u64 *input,
					u64 *output, unsigned int len,
					u64 *iv);

extern void aes_sparc64_cbc_decrypt_256(const u64 *key, const u64 *input,
					u64 *output, unsigned int len,
					u64 *iv);

extern void aes_sparc64_ctr_crypt_128(const u64 *key, const u64 *input,
				      u64 *output, unsigned int len,
				      u64 *iv);
extern void aes_sparc64_ctr_crypt_192(const u64 *key, const u64 *input,
				      u64 *output, unsigned int len,
				      u64 *iv);
extern void aes_sparc64_ctr_crypt_256(const u64 *key, const u64 *input,
				      u64 *output, unsigned int len,
				      u64 *iv);

static struct aes_ops aes128_ops = {
	.encrypt		= aes_sparc64_encrypt_128,
	.decrypt		= aes_sparc64_decrypt_128,
	.load_encrypt_keys	= aes_sparc64_load_encrypt_keys_128,
	.load_decrypt_keys	= aes_sparc64_load_decrypt_keys_128,
	.ecb_encrypt		= aes_sparc64_ecb_encrypt_128,
	.ecb_decrypt		= aes_sparc64_ecb_decrypt_128,
	.cbc_encrypt		= aes_sparc64_cbc_encrypt_128,
	.cbc_decrypt		= aes_sparc64_cbc_decrypt_128,
	.ctr_crypt		= aes_sparc64_ctr_crypt_128,
};

static struct aes_ops aes192_ops = {
	.encrypt		= aes_sparc64_encrypt_192,
	.decrypt		= aes_sparc64_decrypt_192,
	.load_encrypt_keys	= aes_sparc64_load_encrypt_keys_192,
	.load_decrypt_keys	= aes_sparc64_load_decrypt_keys_192,
	.ecb_encrypt		= aes_sparc64_ecb_encrypt_192,
	.ecb_decrypt		= aes_sparc64_ecb_decrypt_192,
	.cbc_encrypt		= aes_sparc64_cbc_encrypt_192,
	.cbc_decrypt		= aes_sparc64_cbc_decrypt_192,
	.ctr_crypt		= aes_sparc64_ctr_crypt_192,
};

static struct aes_ops aes256_ops = {
	.encrypt		= aes_sparc64_encrypt_256,
	.decrypt		= aes_sparc64_decrypt_256,
	.load_encrypt_keys	= aes_sparc64_load_encrypt_keys_256,
	.load_decrypt_keys	= aes_sparc64_load_decrypt_keys_256,
	.ecb_encrypt		= aes_sparc64_ecb_encrypt_256,
	.ecb_decrypt		= aes_sparc64_ecb_decrypt_256,
	.cbc_encrypt		= aes_sparc64_cbc_encrypt_256,
	.cbc_decrypt		= aes_sparc64_cbc_decrypt_256,
	.ctr_crypt		= aes_sparc64_ctr_crypt_256,
};

extern void aes_sparc64_key_expand(const u32 *in_key, u64 *output_key,
				   unsigned int key_len);

static int aes_set_key(struct crypto_tfm *tfm, const u8 *in_key,
		       unsigned int key_len)
{
	struct crypto_sparc64_aes_ctx *ctx = crypto_tfm_ctx(tfm);
<<<<<<< HEAD
	u32 *flags = &tfm->crt_flags;
=======
>>>>>>> upstream/android-13

	switch (key_len) {
	case AES_KEYSIZE_128:
		ctx->expanded_key_length = 0xb0;
		ctx->ops = &aes128_ops;
		break;

	case AES_KEYSIZE_192:
		ctx->expanded_key_length = 0xd0;
		ctx->ops = &aes192_ops;
		break;

	case AES_KEYSIZE_256:
		ctx->expanded_key_length = 0xf0;
		ctx->ops = &aes256_ops;
		break;

	default:
<<<<<<< HEAD
		*flags |= CRYPTO_TFM_RES_BAD_KEY_LEN;
=======
>>>>>>> upstream/android-13
		return -EINVAL;
	}

	aes_sparc64_key_expand((const u32 *)in_key, &ctx->key[0], key_len);
	ctx->key_length = key_len;

	return 0;
}

<<<<<<< HEAD
static void aes_encrypt(struct crypto_tfm *tfm, u8 *dst, const u8 *src)
=======
static int aes_set_key_skcipher(struct crypto_skcipher *tfm, const u8 *in_key,
				unsigned int key_len)
{
	return aes_set_key(crypto_skcipher_tfm(tfm), in_key, key_len);
}

static void crypto_aes_encrypt(struct crypto_tfm *tfm, u8 *dst, const u8 *src)
>>>>>>> upstream/android-13
{
	struct crypto_sparc64_aes_ctx *ctx = crypto_tfm_ctx(tfm);

	ctx->ops->encrypt(&ctx->key[0], (const u32 *) src, (u32 *) dst);
}

<<<<<<< HEAD
static void aes_decrypt(struct crypto_tfm *tfm, u8 *dst, const u8 *src)
=======
static void crypto_aes_decrypt(struct crypto_tfm *tfm, u8 *dst, const u8 *src)
>>>>>>> upstream/android-13
{
	struct crypto_sparc64_aes_ctx *ctx = crypto_tfm_ctx(tfm);

	ctx->ops->decrypt(&ctx->key[0], (const u32 *) src, (u32 *) dst);
}

<<<<<<< HEAD
#define AES_BLOCK_MASK	(~(AES_BLOCK_SIZE-1))

static int ecb_encrypt(struct blkcipher_desc *desc,
		       struct scatterlist *dst, struct scatterlist *src,
		       unsigned int nbytes)
{
	struct crypto_sparc64_aes_ctx *ctx = crypto_blkcipher_ctx(desc->tfm);
	struct blkcipher_walk walk;
	int err;

	blkcipher_walk_init(&walk, dst, src, nbytes);
	err = blkcipher_walk_virt(desc, &walk);
	desc->flags &= ~CRYPTO_TFM_REQ_MAY_SLEEP;

	ctx->ops->load_encrypt_keys(&ctx->key[0]);
	while ((nbytes = walk.nbytes)) {
		unsigned int block_len = nbytes & AES_BLOCK_MASK;

		if (likely(block_len)) {
			ctx->ops->ecb_encrypt(&ctx->key[0],
					      (const u64 *)walk.src.virt.addr,
					      (u64 *) walk.dst.virt.addr,
					      block_len);
		}
		nbytes &= AES_BLOCK_SIZE - 1;
		err = blkcipher_walk_done(desc, &walk, nbytes);
=======
static int ecb_encrypt(struct skcipher_request *req)
{
	struct crypto_skcipher *tfm = crypto_skcipher_reqtfm(req);
	const struct crypto_sparc64_aes_ctx *ctx = crypto_skcipher_ctx(tfm);
	struct skcipher_walk walk;
	unsigned int nbytes;
	int err;

	err = skcipher_walk_virt(&walk, req, true);
	if (err)
		return err;

	ctx->ops->load_encrypt_keys(&ctx->key[0]);
	while ((nbytes = walk.nbytes) != 0) {
		ctx->ops->ecb_encrypt(&ctx->key[0], walk.src.virt.addr,
				      walk.dst.virt.addr,
				      round_down(nbytes, AES_BLOCK_SIZE));
		err = skcipher_walk_done(&walk, nbytes % AES_BLOCK_SIZE);
>>>>>>> upstream/android-13
	}
	fprs_write(0);
	return err;
}

<<<<<<< HEAD
static int ecb_decrypt(struct blkcipher_desc *desc,
		       struct scatterlist *dst, struct scatterlist *src,
		       unsigned int nbytes)
{
	struct crypto_sparc64_aes_ctx *ctx = crypto_blkcipher_ctx(desc->tfm);
	struct blkcipher_walk walk;
	u64 *key_end;
	int err;

	blkcipher_walk_init(&walk, dst, src, nbytes);
	err = blkcipher_walk_virt(desc, &walk);
	desc->flags &= ~CRYPTO_TFM_REQ_MAY_SLEEP;

	ctx->ops->load_decrypt_keys(&ctx->key[0]);
	key_end = &ctx->key[ctx->expanded_key_length / sizeof(u64)];
	while ((nbytes = walk.nbytes)) {
		unsigned int block_len = nbytes & AES_BLOCK_MASK;

		if (likely(block_len)) {
			ctx->ops->ecb_decrypt(key_end,
					      (const u64 *) walk.src.virt.addr,
					      (u64 *) walk.dst.virt.addr, block_len);
		}
		nbytes &= AES_BLOCK_SIZE - 1;
		err = blkcipher_walk_done(desc, &walk, nbytes);
=======
static int ecb_decrypt(struct skcipher_request *req)
{
	struct crypto_skcipher *tfm = crypto_skcipher_reqtfm(req);
	const struct crypto_sparc64_aes_ctx *ctx = crypto_skcipher_ctx(tfm);
	const u64 *key_end;
	struct skcipher_walk walk;
	unsigned int nbytes;
	int err;

	err = skcipher_walk_virt(&walk, req, true);
	if (err)
		return err;

	ctx->ops->load_decrypt_keys(&ctx->key[0]);
	key_end = &ctx->key[ctx->expanded_key_length / sizeof(u64)];
	while ((nbytes = walk.nbytes) != 0) {
		ctx->ops->ecb_decrypt(key_end, walk.src.virt.addr,
				      walk.dst.virt.addr,
				      round_down(nbytes, AES_BLOCK_SIZE));
		err = skcipher_walk_done(&walk, nbytes % AES_BLOCK_SIZE);
>>>>>>> upstream/android-13
	}
	fprs_write(0);

	return err;
}

<<<<<<< HEAD
static int cbc_encrypt(struct blkcipher_desc *desc,
		       struct scatterlist *dst, struct scatterlist *src,
		       unsigned int nbytes)
{
	struct crypto_sparc64_aes_ctx *ctx = crypto_blkcipher_ctx(desc->tfm);
	struct blkcipher_walk walk;
	int err;

	blkcipher_walk_init(&walk, dst, src, nbytes);
	err = blkcipher_walk_virt(desc, &walk);
	desc->flags &= ~CRYPTO_TFM_REQ_MAY_SLEEP;

	ctx->ops->load_encrypt_keys(&ctx->key[0]);
	while ((nbytes = walk.nbytes)) {
		unsigned int block_len = nbytes & AES_BLOCK_MASK;

		if (likely(block_len)) {
			ctx->ops->cbc_encrypt(&ctx->key[0],
					      (const u64 *)walk.src.virt.addr,
					      (u64 *) walk.dst.virt.addr,
					      block_len, (u64 *) walk.iv);
		}
		nbytes &= AES_BLOCK_SIZE - 1;
		err = blkcipher_walk_done(desc, &walk, nbytes);
=======
static int cbc_encrypt(struct skcipher_request *req)
{
	struct crypto_skcipher *tfm = crypto_skcipher_reqtfm(req);
	const struct crypto_sparc64_aes_ctx *ctx = crypto_skcipher_ctx(tfm);
	struct skcipher_walk walk;
	unsigned int nbytes;
	int err;

	err = skcipher_walk_virt(&walk, req, true);
	if (err)
		return err;

	ctx->ops->load_encrypt_keys(&ctx->key[0]);
	while ((nbytes = walk.nbytes) != 0) {
		ctx->ops->cbc_encrypt(&ctx->key[0], walk.src.virt.addr,
				      walk.dst.virt.addr,
				      round_down(nbytes, AES_BLOCK_SIZE),
				      walk.iv);
		err = skcipher_walk_done(&walk, nbytes % AES_BLOCK_SIZE);
>>>>>>> upstream/android-13
	}
	fprs_write(0);
	return err;
}

<<<<<<< HEAD
static int cbc_decrypt(struct blkcipher_desc *desc,
		       struct scatterlist *dst, struct scatterlist *src,
		       unsigned int nbytes)
{
	struct crypto_sparc64_aes_ctx *ctx = crypto_blkcipher_ctx(desc->tfm);
	struct blkcipher_walk walk;
	u64 *key_end;
	int err;

	blkcipher_walk_init(&walk, dst, src, nbytes);
	err = blkcipher_walk_virt(desc, &walk);
	desc->flags &= ~CRYPTO_TFM_REQ_MAY_SLEEP;

	ctx->ops->load_decrypt_keys(&ctx->key[0]);
	key_end = &ctx->key[ctx->expanded_key_length / sizeof(u64)];
	while ((nbytes = walk.nbytes)) {
		unsigned int block_len = nbytes & AES_BLOCK_MASK;

		if (likely(block_len)) {
			ctx->ops->cbc_decrypt(key_end,
					      (const u64 *) walk.src.virt.addr,
					      (u64 *) walk.dst.virt.addr,
					      block_len, (u64 *) walk.iv);
		}
		nbytes &= AES_BLOCK_SIZE - 1;
		err = blkcipher_walk_done(desc, &walk, nbytes);
=======
static int cbc_decrypt(struct skcipher_request *req)
{
	struct crypto_skcipher *tfm = crypto_skcipher_reqtfm(req);
	const struct crypto_sparc64_aes_ctx *ctx = crypto_skcipher_ctx(tfm);
	const u64 *key_end;
	struct skcipher_walk walk;
	unsigned int nbytes;
	int err;

	err = skcipher_walk_virt(&walk, req, true);
	if (err)
		return err;

	ctx->ops->load_decrypt_keys(&ctx->key[0]);
	key_end = &ctx->key[ctx->expanded_key_length / sizeof(u64)];
	while ((nbytes = walk.nbytes) != 0) {
		ctx->ops->cbc_decrypt(key_end, walk.src.virt.addr,
				      walk.dst.virt.addr,
				      round_down(nbytes, AES_BLOCK_SIZE),
				      walk.iv);
		err = skcipher_walk_done(&walk, nbytes % AES_BLOCK_SIZE);
>>>>>>> upstream/android-13
	}
	fprs_write(0);

	return err;
}

<<<<<<< HEAD
static void ctr_crypt_final(struct crypto_sparc64_aes_ctx *ctx,
			    struct blkcipher_walk *walk)
=======
static void ctr_crypt_final(const struct crypto_sparc64_aes_ctx *ctx,
			    struct skcipher_walk *walk)
>>>>>>> upstream/android-13
{
	u8 *ctrblk = walk->iv;
	u64 keystream[AES_BLOCK_SIZE / sizeof(u64)];
	u8 *src = walk->src.virt.addr;
	u8 *dst = walk->dst.virt.addr;
	unsigned int nbytes = walk->nbytes;

	ctx->ops->ecb_encrypt(&ctx->key[0], (const u64 *)ctrblk,
			      keystream, AES_BLOCK_SIZE);
	crypto_xor_cpy(dst, (u8 *) keystream, src, nbytes);
	crypto_inc(ctrblk, AES_BLOCK_SIZE);
}

<<<<<<< HEAD
static int ctr_crypt(struct blkcipher_desc *desc,
		     struct scatterlist *dst, struct scatterlist *src,
		     unsigned int nbytes)
{
	struct crypto_sparc64_aes_ctx *ctx = crypto_blkcipher_ctx(desc->tfm);
	struct blkcipher_walk walk;
	int err;

	blkcipher_walk_init(&walk, dst, src, nbytes);
	err = blkcipher_walk_virt_block(desc, &walk, AES_BLOCK_SIZE);
	desc->flags &= ~CRYPTO_TFM_REQ_MAY_SLEEP;

	ctx->ops->load_encrypt_keys(&ctx->key[0]);
	while ((nbytes = walk.nbytes) >= AES_BLOCK_SIZE) {
		unsigned int block_len = nbytes & AES_BLOCK_MASK;

		if (likely(block_len)) {
			ctx->ops->ctr_crypt(&ctx->key[0],
					    (const u64 *)walk.src.virt.addr,
					    (u64 *) walk.dst.virt.addr,
					    block_len, (u64 *) walk.iv);
		}
		nbytes &= AES_BLOCK_SIZE - 1;
		err = blkcipher_walk_done(desc, &walk, nbytes);
	}
	if (walk.nbytes) {
		ctr_crypt_final(ctx, &walk);
		err = blkcipher_walk_done(desc, &walk, 0);
=======
static int ctr_crypt(struct skcipher_request *req)
{
	struct crypto_skcipher *tfm = crypto_skcipher_reqtfm(req);
	const struct crypto_sparc64_aes_ctx *ctx = crypto_skcipher_ctx(tfm);
	struct skcipher_walk walk;
	unsigned int nbytes;
	int err;

	err = skcipher_walk_virt(&walk, req, true);
	if (err)
		return err;

	ctx->ops->load_encrypt_keys(&ctx->key[0]);
	while ((nbytes = walk.nbytes) >= AES_BLOCK_SIZE) {
		ctx->ops->ctr_crypt(&ctx->key[0], walk.src.virt.addr,
				    walk.dst.virt.addr,
				    round_down(nbytes, AES_BLOCK_SIZE),
				    walk.iv);
		err = skcipher_walk_done(&walk, nbytes % AES_BLOCK_SIZE);
	}
	if (walk.nbytes) {
		ctr_crypt_final(ctx, &walk);
		err = skcipher_walk_done(&walk, 0);
>>>>>>> upstream/android-13
	}
	fprs_write(0);
	return err;
}

<<<<<<< HEAD
static struct crypto_alg algs[] = { {
=======
static struct crypto_alg cipher_alg = {
>>>>>>> upstream/android-13
	.cra_name		= "aes",
	.cra_driver_name	= "aes-sparc64",
	.cra_priority		= SPARC_CR_OPCODE_PRIORITY,
	.cra_flags		= CRYPTO_ALG_TYPE_CIPHER,
	.cra_blocksize		= AES_BLOCK_SIZE,
	.cra_ctxsize		= sizeof(struct crypto_sparc64_aes_ctx),
	.cra_alignmask		= 3,
	.cra_module		= THIS_MODULE,
	.cra_u	= {
		.cipher	= {
			.cia_min_keysize	= AES_MIN_KEY_SIZE,
			.cia_max_keysize	= AES_MAX_KEY_SIZE,
			.cia_setkey		= aes_set_key,
<<<<<<< HEAD
			.cia_encrypt		= aes_encrypt,
			.cia_decrypt		= aes_decrypt
		}
	}
}, {
	.cra_name		= "ecb(aes)",
	.cra_driver_name	= "ecb-aes-sparc64",
	.cra_priority		= SPARC_CR_OPCODE_PRIORITY,
	.cra_flags		= CRYPTO_ALG_TYPE_BLKCIPHER,
	.cra_blocksize		= AES_BLOCK_SIZE,
	.cra_ctxsize		= sizeof(struct crypto_sparc64_aes_ctx),
	.cra_alignmask		= 7,
	.cra_type		= &crypto_blkcipher_type,
	.cra_module		= THIS_MODULE,
	.cra_u = {
		.blkcipher = {
			.min_keysize	= AES_MIN_KEY_SIZE,
			.max_keysize	= AES_MAX_KEY_SIZE,
			.setkey		= aes_set_key,
			.encrypt	= ecb_encrypt,
			.decrypt	= ecb_decrypt,
		},
	},
}, {
	.cra_name		= "cbc(aes)",
	.cra_driver_name	= "cbc-aes-sparc64",
	.cra_priority		= SPARC_CR_OPCODE_PRIORITY,
	.cra_flags		= CRYPTO_ALG_TYPE_BLKCIPHER,
	.cra_blocksize		= AES_BLOCK_SIZE,
	.cra_ctxsize		= sizeof(struct crypto_sparc64_aes_ctx),
	.cra_alignmask		= 7,
	.cra_type		= &crypto_blkcipher_type,
	.cra_module		= THIS_MODULE,
	.cra_u = {
		.blkcipher = {
			.min_keysize	= AES_MIN_KEY_SIZE,
			.max_keysize	= AES_MAX_KEY_SIZE,
			.ivsize		= AES_BLOCK_SIZE,
			.setkey		= aes_set_key,
			.encrypt	= cbc_encrypt,
			.decrypt	= cbc_decrypt,
		},
	},
}, {
	.cra_name		= "ctr(aes)",
	.cra_driver_name	= "ctr-aes-sparc64",
	.cra_priority		= SPARC_CR_OPCODE_PRIORITY,
	.cra_flags		= CRYPTO_ALG_TYPE_BLKCIPHER,
	.cra_blocksize		= 1,
	.cra_ctxsize		= sizeof(struct crypto_sparc64_aes_ctx),
	.cra_alignmask		= 7,
	.cra_type		= &crypto_blkcipher_type,
	.cra_module		= THIS_MODULE,
	.cra_u = {
		.blkcipher = {
			.min_keysize	= AES_MIN_KEY_SIZE,
			.max_keysize	= AES_MAX_KEY_SIZE,
			.ivsize		= AES_BLOCK_SIZE,
			.setkey		= aes_set_key,
			.encrypt	= ctr_crypt,
			.decrypt	= ctr_crypt,
		},
	},
} };
=======
			.cia_encrypt		= crypto_aes_encrypt,
			.cia_decrypt		= crypto_aes_decrypt
		}
	}
};

static struct skcipher_alg skcipher_algs[] = {
	{
		.base.cra_name		= "ecb(aes)",
		.base.cra_driver_name	= "ecb-aes-sparc64",
		.base.cra_priority	= SPARC_CR_OPCODE_PRIORITY,
		.base.cra_blocksize	= AES_BLOCK_SIZE,
		.base.cra_ctxsize	= sizeof(struct crypto_sparc64_aes_ctx),
		.base.cra_alignmask	= 7,
		.base.cra_module	= THIS_MODULE,
		.min_keysize		= AES_MIN_KEY_SIZE,
		.max_keysize		= AES_MAX_KEY_SIZE,
		.setkey			= aes_set_key_skcipher,
		.encrypt		= ecb_encrypt,
		.decrypt		= ecb_decrypt,
	}, {
		.base.cra_name		= "cbc(aes)",
		.base.cra_driver_name	= "cbc-aes-sparc64",
		.base.cra_priority	= SPARC_CR_OPCODE_PRIORITY,
		.base.cra_blocksize	= AES_BLOCK_SIZE,
		.base.cra_ctxsize	= sizeof(struct crypto_sparc64_aes_ctx),
		.base.cra_alignmask	= 7,
		.base.cra_module	= THIS_MODULE,
		.min_keysize		= AES_MIN_KEY_SIZE,
		.max_keysize		= AES_MAX_KEY_SIZE,
		.ivsize			= AES_BLOCK_SIZE,
		.setkey			= aes_set_key_skcipher,
		.encrypt		= cbc_encrypt,
		.decrypt		= cbc_decrypt,
	}, {
		.base.cra_name		= "ctr(aes)",
		.base.cra_driver_name	= "ctr-aes-sparc64",
		.base.cra_priority	= SPARC_CR_OPCODE_PRIORITY,
		.base.cra_blocksize	= 1,
		.base.cra_ctxsize	= sizeof(struct crypto_sparc64_aes_ctx),
		.base.cra_alignmask	= 7,
		.base.cra_module	= THIS_MODULE,
		.min_keysize		= AES_MIN_KEY_SIZE,
		.max_keysize		= AES_MAX_KEY_SIZE,
		.ivsize			= AES_BLOCK_SIZE,
		.setkey			= aes_set_key_skcipher,
		.encrypt		= ctr_crypt,
		.decrypt		= ctr_crypt,
		.chunksize		= AES_BLOCK_SIZE,
	}
};
>>>>>>> upstream/android-13

static bool __init sparc64_has_aes_opcode(void)
{
	unsigned long cfr;

	if (!(sparc64_elf_hwcap & HWCAP_SPARC_CRYPTO))
		return false;

	__asm__ __volatile__("rd %%asr26, %0" : "=r" (cfr));
	if (!(cfr & CFR_AES))
		return false;

	return true;
}

static int __init aes_sparc64_mod_init(void)
{
<<<<<<< HEAD
	int i;

	for (i = 0; i < ARRAY_SIZE(algs); i++)
		INIT_LIST_HEAD(&algs[i].cra_list);

	if (sparc64_has_aes_opcode()) {
		pr_info("Using sparc64 aes opcodes optimized AES implementation\n");
		return crypto_register_algs(algs, ARRAY_SIZE(algs));
	}
	pr_info("sparc64 aes opcodes not available.\n");
	return -ENODEV;
=======
	int err;

	if (!sparc64_has_aes_opcode()) {
		pr_info("sparc64 aes opcodes not available.\n");
		return -ENODEV;
	}
	pr_info("Using sparc64 aes opcodes optimized AES implementation\n");
	err = crypto_register_alg(&cipher_alg);
	if (err)
		return err;
	err = crypto_register_skciphers(skcipher_algs,
					ARRAY_SIZE(skcipher_algs));
	if (err)
		crypto_unregister_alg(&cipher_alg);
	return err;
>>>>>>> upstream/android-13
}

static void __exit aes_sparc64_mod_fini(void)
{
<<<<<<< HEAD
	crypto_unregister_algs(algs, ARRAY_SIZE(algs));
=======
	crypto_unregister_alg(&cipher_alg);
	crypto_unregister_skciphers(skcipher_algs, ARRAY_SIZE(skcipher_algs));
>>>>>>> upstream/android-13
}

module_init(aes_sparc64_mod_init);
module_exit(aes_sparc64_mod_fini);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Rijndael (AES) Cipher Algorithm, sparc64 aes opcode accelerated");

MODULE_ALIAS_CRYPTO("aes");

#include "crop_devid.c"
