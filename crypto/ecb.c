<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> upstream/android-13
/*
 * ECB: Electronic CodeBook mode
 *
 * Copyright (c) 2006 Herbert Xu <herbert@gondor.apana.org.au>
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version.
 *
 */

#include <crypto/algapi.h>
=======
 */

#include <crypto/algapi.h>
#include <crypto/internal/cipher.h>
#include <crypto/internal/skcipher.h>
>>>>>>> upstream/android-13
#include <linux/err.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
<<<<<<< HEAD
#include <linux/scatterlist.h>
#include <linux/slab.h>

struct crypto_ecb_ctx {
	struct crypto_cipher *child;
};

static int crypto_ecb_setkey(struct crypto_tfm *parent, const u8 *key,
			     unsigned int keylen)
{
	struct crypto_ecb_ctx *ctx = crypto_tfm_ctx(parent);
	struct crypto_cipher *child = ctx->child;
	int err;

	crypto_cipher_clear_flags(child, CRYPTO_TFM_REQ_MASK);
	crypto_cipher_set_flags(child, crypto_tfm_get_flags(parent) &
				       CRYPTO_TFM_REQ_MASK);
	err = crypto_cipher_setkey(child, key, keylen);
	crypto_tfm_set_flags(parent, crypto_cipher_get_flags(child) &
				     CRYPTO_TFM_RES_MASK);
	return err;
}

static int crypto_ecb_crypt(struct blkcipher_desc *desc,
			    struct blkcipher_walk *walk,
			    struct crypto_cipher *tfm,
			    void (*fn)(struct crypto_tfm *, u8 *, const u8 *))
{
	int bsize = crypto_cipher_blocksize(tfm);
	unsigned int nbytes;
	int err;

	err = blkcipher_walk_virt(desc, walk);

	while ((nbytes = walk->nbytes)) {
		u8 *wsrc = walk->src.virt.addr;
		u8 *wdst = walk->dst.virt.addr;

		do {
			fn(crypto_cipher_tfm(tfm), wdst, wsrc);

			wsrc += bsize;
			wdst += bsize;
		} while ((nbytes -= bsize) >= bsize);

		err = blkcipher_walk_done(desc, walk, nbytes);
=======

static int crypto_ecb_crypt(struct skcipher_request *req,
			    struct crypto_cipher *cipher,
			    void (*fn)(struct crypto_tfm *, u8 *, const u8 *))
{
	const unsigned int bsize = crypto_cipher_blocksize(cipher);
	struct skcipher_walk walk;
	unsigned int nbytes;
	int err;

	err = skcipher_walk_virt(&walk, req, false);

	while ((nbytes = walk.nbytes) != 0) {
		const u8 *src = walk.src.virt.addr;
		u8 *dst = walk.dst.virt.addr;

		do {
			fn(crypto_cipher_tfm(cipher), dst, src);

			src += bsize;
			dst += bsize;
		} while ((nbytes -= bsize) >= bsize);

		err = skcipher_walk_done(&walk, nbytes);
>>>>>>> upstream/android-13
	}

	return err;
}

<<<<<<< HEAD
static int crypto_ecb_encrypt(struct blkcipher_desc *desc,
			      struct scatterlist *dst, struct scatterlist *src,
			      unsigned int nbytes)
{
	struct blkcipher_walk walk;
	struct crypto_blkcipher *tfm = desc->tfm;
	struct crypto_ecb_ctx *ctx = crypto_blkcipher_ctx(tfm);
	struct crypto_cipher *child = ctx->child;

	blkcipher_walk_init(&walk, dst, src, nbytes);
	return crypto_ecb_crypt(desc, &walk, child,
				crypto_cipher_alg(child)->cia_encrypt);
}

static int crypto_ecb_decrypt(struct blkcipher_desc *desc,
			      struct scatterlist *dst, struct scatterlist *src,
			      unsigned int nbytes)
{
	struct blkcipher_walk walk;
	struct crypto_blkcipher *tfm = desc->tfm;
	struct crypto_ecb_ctx *ctx = crypto_blkcipher_ctx(tfm);
	struct crypto_cipher *child = ctx->child;

	blkcipher_walk_init(&walk, dst, src, nbytes);
	return crypto_ecb_crypt(desc, &walk, child,
				crypto_cipher_alg(child)->cia_decrypt);
}

static int crypto_ecb_init_tfm(struct crypto_tfm *tfm)
{
	struct crypto_instance *inst = (void *)tfm->__crt_alg;
	struct crypto_spawn *spawn = crypto_instance_ctx(inst);
	struct crypto_ecb_ctx *ctx = crypto_tfm_ctx(tfm);
	struct crypto_cipher *cipher;

	cipher = crypto_spawn_cipher(spawn);
	if (IS_ERR(cipher))
		return PTR_ERR(cipher);

	ctx->child = cipher;
	return 0;
}

static void crypto_ecb_exit_tfm(struct crypto_tfm *tfm)
{
	struct crypto_ecb_ctx *ctx = crypto_tfm_ctx(tfm);
	crypto_free_cipher(ctx->child);
}

static struct crypto_instance *crypto_ecb_alloc(struct rtattr **tb)
{
	struct crypto_instance *inst;
	struct crypto_alg *alg;
	int err;

	err = crypto_check_attr_type(tb, CRYPTO_ALG_TYPE_BLKCIPHER);
	if (err)
		return ERR_PTR(err);

	alg = crypto_get_attr_alg(tb, CRYPTO_ALG_TYPE_CIPHER,
				  CRYPTO_ALG_TYPE_MASK);
	if (IS_ERR(alg))
		return ERR_CAST(alg);

	inst = crypto_alloc_instance("ecb", alg);
	if (IS_ERR(inst))
		goto out_put_alg;

	inst->alg.cra_flags = CRYPTO_ALG_TYPE_BLKCIPHER;
	inst->alg.cra_priority = alg->cra_priority;
	inst->alg.cra_blocksize = alg->cra_blocksize;
	inst->alg.cra_alignmask = alg->cra_alignmask;
	inst->alg.cra_type = &crypto_blkcipher_type;

	inst->alg.cra_blkcipher.min_keysize = alg->cra_cipher.cia_min_keysize;
	inst->alg.cra_blkcipher.max_keysize = alg->cra_cipher.cia_max_keysize;

	inst->alg.cra_ctxsize = sizeof(struct crypto_ecb_ctx);

	inst->alg.cra_init = crypto_ecb_init_tfm;
	inst->alg.cra_exit = crypto_ecb_exit_tfm;

	inst->alg.cra_blkcipher.setkey = crypto_ecb_setkey;
	inst->alg.cra_blkcipher.encrypt = crypto_ecb_encrypt;
	inst->alg.cra_blkcipher.decrypt = crypto_ecb_decrypt;

out_put_alg:
	crypto_mod_put(alg);
	return inst;
}

static void crypto_ecb_free(struct crypto_instance *inst)
{
	crypto_drop_spawn(crypto_instance_ctx(inst));
	kfree(inst);
=======
static int crypto_ecb_encrypt(struct skcipher_request *req)
{
	struct crypto_skcipher *tfm = crypto_skcipher_reqtfm(req);
	struct crypto_cipher *cipher = skcipher_cipher_simple(tfm);

	return crypto_ecb_crypt(req, cipher,
				crypto_cipher_alg(cipher)->cia_encrypt);
}

static int crypto_ecb_decrypt(struct skcipher_request *req)
{
	struct crypto_skcipher *tfm = crypto_skcipher_reqtfm(req);
	struct crypto_cipher *cipher = skcipher_cipher_simple(tfm);

	return crypto_ecb_crypt(req, cipher,
				crypto_cipher_alg(cipher)->cia_decrypt);
}

static int crypto_ecb_create(struct crypto_template *tmpl, struct rtattr **tb)
{
	struct skcipher_instance *inst;
	int err;

	inst = skcipher_alloc_instance_simple(tmpl, tb);
	if (IS_ERR(inst))
		return PTR_ERR(inst);

	inst->alg.ivsize = 0; /* ECB mode doesn't take an IV */

	inst->alg.encrypt = crypto_ecb_encrypt;
	inst->alg.decrypt = crypto_ecb_decrypt;

	err = skcipher_register_instance(tmpl, inst);
	if (err)
		inst->free(inst);

	return err;
>>>>>>> upstream/android-13
}

static struct crypto_template crypto_ecb_tmpl = {
	.name = "ecb",
<<<<<<< HEAD
	.alloc = crypto_ecb_alloc,
	.free = crypto_ecb_free,
=======
	.create = crypto_ecb_create,
>>>>>>> upstream/android-13
	.module = THIS_MODULE,
};

static int __init crypto_ecb_module_init(void)
{
	return crypto_register_template(&crypto_ecb_tmpl);
}

static void __exit crypto_ecb_module_exit(void)
{
	crypto_unregister_template(&crypto_ecb_tmpl);
}

<<<<<<< HEAD
module_init(crypto_ecb_module_init);
module_exit(crypto_ecb_module_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("ECB block cipher algorithm");
=======
subsys_initcall(crypto_ecb_module_init);
module_exit(crypto_ecb_module_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("ECB block cipher mode of operation");
>>>>>>> upstream/android-13
MODULE_ALIAS_CRYPTO("ecb");
