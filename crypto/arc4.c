<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> upstream/android-13
/*
 * Cryptographic API
 *
 * ARC4 Cipher Algorithm
 *
 * Jon Oberheide <jon@oberheide.org>
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */

#include <linux/module.h>
#include <linux/init.h>
#include <linux/crypto.h>
#include <crypto/algapi.h>

#define ARC4_MIN_KEY_SIZE	1
#define ARC4_MAX_KEY_SIZE	256
#define ARC4_BLOCK_SIZE		1

struct arc4_ctx {
	u32 S[256];
	u32 x, y;
};

static int arc4_set_key(struct crypto_tfm *tfm, const u8 *in_key,
			unsigned int key_len)
{
	struct arc4_ctx *ctx = crypto_tfm_ctx(tfm);
	int i, j = 0, k = 0;

	ctx->x = 1;
	ctx->y = 0;

	for (i = 0; i < 256; i++)
		ctx->S[i] = i;

	for (i = 0; i < 256; i++) {
		u32 a = ctx->S[i];
		j = (j + in_key[k] + a) & 0xff;
		ctx->S[i] = ctx->S[j];
		ctx->S[j] = a;
		if (++k >= key_len)
			k = 0;
	}

	return 0;
}

static void arc4_crypt(struct arc4_ctx *ctx, u8 *out, const u8 *in,
		       unsigned int len)
{
	u32 *const S = ctx->S;
	u32 x, y, a, b;
	u32 ty, ta, tb;

	if (len == 0)
		return;

	x = ctx->x;
	y = ctx->y;

	a = S[x];
	y = (y + a) & 0xff;
	b = S[y];

	do {
		S[y] = a;
		a = (a + b) & 0xff;
		S[x] = b;
		x = (x + 1) & 0xff;
		ta = S[x];
		ty = (y + ta) & 0xff;
		tb = S[ty];
		*out++ = *in++ ^ S[a];
		if (--len == 0)
			break;
		y = ty;
		a = ta;
		b = tb;
	} while (true);

	ctx->x = x;
	ctx->y = y;
}

static void arc4_crypt_one(struct crypto_tfm *tfm, u8 *out, const u8 *in)
{
	arc4_crypt(crypto_tfm_ctx(tfm), out, in, 1);
}

static int ecb_arc4_crypt(struct blkcipher_desc *desc, struct scatterlist *dst,
			  struct scatterlist *src, unsigned int nbytes)
{
	struct arc4_ctx *ctx = crypto_blkcipher_ctx(desc->tfm);
	struct blkcipher_walk walk;
	int err;

	blkcipher_walk_init(&walk, dst, src, nbytes);

	err = blkcipher_walk_virt(desc, &walk);

	while (walk.nbytes > 0) {
		u8 *wsrc = walk.src.virt.addr;
		u8 *wdst = walk.dst.virt.addr;

		arc4_crypt(ctx, wdst, wsrc, walk.nbytes);

		err = blkcipher_walk_done(desc, &walk, 0);
=======
 */

#include <crypto/algapi.h>
#include <crypto/arc4.h>
#include <crypto/internal/skcipher.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/sched.h>

static int crypto_arc4_setkey(struct crypto_skcipher *tfm, const u8 *in_key,
			      unsigned int key_len)
{
	struct arc4_ctx *ctx = crypto_skcipher_ctx(tfm);

	return arc4_setkey(ctx, in_key, key_len);
}

static int crypto_arc4_crypt(struct skcipher_request *req)
{
	struct crypto_skcipher *tfm = crypto_skcipher_reqtfm(req);
	struct arc4_ctx *ctx = crypto_skcipher_ctx(tfm);
	struct skcipher_walk walk;
	int err;

	err = skcipher_walk_virt(&walk, req, false);

	while (walk.nbytes > 0) {
		arc4_crypt(ctx, walk.dst.virt.addr, walk.src.virt.addr,
			   walk.nbytes);
		err = skcipher_walk_done(&walk, 0);
>>>>>>> upstream/android-13
	}

	return err;
}

<<<<<<< HEAD
static struct crypto_alg arc4_algs[2] = { {
	.cra_name		=	"arc4",
	.cra_flags		=	CRYPTO_ALG_TYPE_CIPHER,
	.cra_blocksize		=	ARC4_BLOCK_SIZE,
	.cra_ctxsize		=	sizeof(struct arc4_ctx),
	.cra_module		=	THIS_MODULE,
	.cra_u			=	{
		.cipher = {
			.cia_min_keysize	=	ARC4_MIN_KEY_SIZE,
			.cia_max_keysize	=	ARC4_MAX_KEY_SIZE,
			.cia_setkey		=	arc4_set_key,
			.cia_encrypt		=	arc4_crypt_one,
			.cia_decrypt		=	arc4_crypt_one,
		},
	},
}, {
	.cra_name		=	"ecb(arc4)",
	.cra_priority		=	100,
	.cra_flags		=	CRYPTO_ALG_TYPE_BLKCIPHER,
	.cra_blocksize		=	ARC4_BLOCK_SIZE,
	.cra_ctxsize		=	sizeof(struct arc4_ctx),
	.cra_alignmask		=	0,
	.cra_type		=	&crypto_blkcipher_type,
	.cra_module		=	THIS_MODULE,
	.cra_u			=	{
		.blkcipher = {
			.min_keysize	=	ARC4_MIN_KEY_SIZE,
			.max_keysize	=	ARC4_MAX_KEY_SIZE,
			.setkey		=	arc4_set_key,
			.encrypt	=	ecb_arc4_crypt,
			.decrypt	=	ecb_arc4_crypt,
		},
	},
} };

static int __init arc4_init(void)
{
	return crypto_register_algs(arc4_algs, ARRAY_SIZE(arc4_algs));
=======
static int crypto_arc4_init(struct crypto_skcipher *tfm)
{
	pr_warn_ratelimited("\"%s\" (%ld) uses obsolete ecb(arc4) skcipher\n",
			    current->comm, (unsigned long)current->pid);

	return 0;
}

static struct skcipher_alg arc4_alg = {
	/*
	 * For legacy reasons, this is named "ecb(arc4)", not "arc4".
	 * Nevertheless it's actually a stream cipher, not a block cipher.
	 */
	.base.cra_name		=	"ecb(arc4)",
	.base.cra_driver_name	=	"ecb(arc4)-generic",
	.base.cra_priority	=	100,
	.base.cra_blocksize	=	ARC4_BLOCK_SIZE,
	.base.cra_ctxsize	=	sizeof(struct arc4_ctx),
	.base.cra_module	=	THIS_MODULE,
	.min_keysize		=	ARC4_MIN_KEY_SIZE,
	.max_keysize		=	ARC4_MAX_KEY_SIZE,
	.setkey			=	crypto_arc4_setkey,
	.encrypt		=	crypto_arc4_crypt,
	.decrypt		=	crypto_arc4_crypt,
	.init			=	crypto_arc4_init,
};

static int __init arc4_init(void)
{
	return crypto_register_skcipher(&arc4_alg);
>>>>>>> upstream/android-13
}

static void __exit arc4_exit(void)
{
<<<<<<< HEAD
	crypto_unregister_algs(arc4_algs, ARRAY_SIZE(arc4_algs));
}

module_init(arc4_init);
=======
	crypto_unregister_skcipher(&arc4_alg);
}

subsys_initcall(arc4_init);
>>>>>>> upstream/android-13
module_exit(arc4_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("ARC4 Cipher Algorithm");
MODULE_AUTHOR("Jon Oberheide <jon@oberheide.org>");
<<<<<<< HEAD
MODULE_ALIAS_CRYPTO("arc4");
=======
MODULE_ALIAS_CRYPTO("ecb(arc4)");
>>>>>>> upstream/android-13
