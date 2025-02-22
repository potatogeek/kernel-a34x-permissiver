<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> upstream/android-13
/*
 * CBC: Cipher Block Chaining mode
 *
 * Copyright (c) 2006-2016 Herbert Xu <herbert@gondor.apana.org.au>
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version.
 *
 */

#include <crypto/algapi.h>
#include <crypto/cbc.h>
=======
 */

#include <crypto/algapi.h>
#include <crypto/internal/cipher.h>
>>>>>>> upstream/android-13
#include <crypto/internal/skcipher.h>
#include <linux/err.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/log2.h>
#include <linux/module.h>
<<<<<<< HEAD
#include <linux/slab.h>

struct crypto_cbc_ctx {
	struct crypto_cipher *child;
};

static int crypto_cbc_setkey(struct crypto_skcipher *parent, const u8 *key,
			     unsigned int keylen)
{
	struct crypto_cbc_ctx *ctx = crypto_skcipher_ctx(parent);
	struct crypto_cipher *child = ctx->child;
	int err;

	crypto_cipher_clear_flags(child, CRYPTO_TFM_REQ_MASK);
	crypto_cipher_set_flags(child, crypto_skcipher_get_flags(parent) &
				       CRYPTO_TFM_REQ_MASK);
	err = crypto_cipher_setkey(child, key, keylen);
	crypto_skcipher_set_flags(parent, crypto_cipher_get_flags(child) &
					  CRYPTO_TFM_RES_MASK);
	return err;
}

static inline void crypto_cbc_encrypt_one(struct crypto_skcipher *tfm,
					  const u8 *src, u8 *dst)
{
	struct crypto_cbc_ctx *ctx = crypto_skcipher_ctx(tfm);

	crypto_cipher_encrypt_one(ctx->child, dst, src);
=======

static int crypto_cbc_encrypt_segment(struct skcipher_walk *walk,
				      struct crypto_skcipher *skcipher)
{
	unsigned int bsize = crypto_skcipher_blocksize(skcipher);
	void (*fn)(struct crypto_tfm *, u8 *, const u8 *);
	unsigned int nbytes = walk->nbytes;
	u8 *src = walk->src.virt.addr;
	u8 *dst = walk->dst.virt.addr;
	struct crypto_cipher *cipher;
	struct crypto_tfm *tfm;
	u8 *iv = walk->iv;

	cipher = skcipher_cipher_simple(skcipher);
	tfm = crypto_cipher_tfm(cipher);
	fn = crypto_cipher_alg(cipher)->cia_encrypt;

	do {
		crypto_xor(iv, src, bsize);
		fn(tfm, dst, iv);
		memcpy(iv, dst, bsize);

		src += bsize;
		dst += bsize;
	} while ((nbytes -= bsize) >= bsize);

	return nbytes;
}

static int crypto_cbc_encrypt_inplace(struct skcipher_walk *walk,
				      struct crypto_skcipher *skcipher)
{
	unsigned int bsize = crypto_skcipher_blocksize(skcipher);
	void (*fn)(struct crypto_tfm *, u8 *, const u8 *);
	unsigned int nbytes = walk->nbytes;
	u8 *src = walk->src.virt.addr;
	struct crypto_cipher *cipher;
	struct crypto_tfm *tfm;
	u8 *iv = walk->iv;

	cipher = skcipher_cipher_simple(skcipher);
	tfm = crypto_cipher_tfm(cipher);
	fn = crypto_cipher_alg(cipher)->cia_encrypt;

	do {
		crypto_xor(src, iv, bsize);
		fn(tfm, src, src);
		iv = src;

		src += bsize;
	} while ((nbytes -= bsize) >= bsize);

	memcpy(walk->iv, iv, bsize);

	return nbytes;
>>>>>>> upstream/android-13
}

static int crypto_cbc_encrypt(struct skcipher_request *req)
{
<<<<<<< HEAD
	return crypto_cbc_encrypt_walk(req, crypto_cbc_encrypt_one);
}

static inline void crypto_cbc_decrypt_one(struct crypto_skcipher *tfm,
					  const u8 *src, u8 *dst)
{
	struct crypto_cbc_ctx *ctx = crypto_skcipher_ctx(tfm);

	crypto_cipher_decrypt_one(ctx->child, dst, src);
}

static int crypto_cbc_decrypt(struct skcipher_request *req)
{
	struct crypto_skcipher *tfm = crypto_skcipher_reqtfm(req);
=======
	struct crypto_skcipher *skcipher = crypto_skcipher_reqtfm(req);
>>>>>>> upstream/android-13
	struct skcipher_walk walk;
	int err;

	err = skcipher_walk_virt(&walk, req, false);

	while (walk.nbytes) {
<<<<<<< HEAD
		err = crypto_cbc_decrypt_blocks(&walk, tfm,
						crypto_cbc_decrypt_one);
=======
		if (walk.src.virt.addr == walk.dst.virt.addr)
			err = crypto_cbc_encrypt_inplace(&walk, skcipher);
		else
			err = crypto_cbc_encrypt_segment(&walk, skcipher);
>>>>>>> upstream/android-13
		err = skcipher_walk_done(&walk, err);
	}

	return err;
}

<<<<<<< HEAD
static int crypto_cbc_init_tfm(struct crypto_skcipher *tfm)
{
	struct skcipher_instance *inst = skcipher_alg_instance(tfm);
	struct crypto_spawn *spawn = skcipher_instance_ctx(inst);
	struct crypto_cbc_ctx *ctx = crypto_skcipher_ctx(tfm);
	struct crypto_cipher *cipher;

	cipher = crypto_spawn_cipher(spawn);
	if (IS_ERR(cipher))
		return PTR_ERR(cipher);

	ctx->child = cipher;
	return 0;
}

static void crypto_cbc_exit_tfm(struct crypto_skcipher *tfm)
{
	struct crypto_cbc_ctx *ctx = crypto_skcipher_ctx(tfm);

	crypto_free_cipher(ctx->child);
}

static void crypto_cbc_free(struct skcipher_instance *inst)
{
	crypto_drop_skcipher(skcipher_instance_ctx(inst));
	kfree(inst);
=======
static int crypto_cbc_decrypt_segment(struct skcipher_walk *walk,
				      struct crypto_skcipher *skcipher)
{
	unsigned int bsize = crypto_skcipher_blocksize(skcipher);
	void (*fn)(struct crypto_tfm *, u8 *, const u8 *);
	unsigned int nbytes = walk->nbytes;
	u8 *src = walk->src.virt.addr;
	u8 *dst = walk->dst.virt.addr;
	struct crypto_cipher *cipher;
	struct crypto_tfm *tfm;
	u8 *iv = walk->iv;

	cipher = skcipher_cipher_simple(skcipher);
	tfm = crypto_cipher_tfm(cipher);
	fn = crypto_cipher_alg(cipher)->cia_decrypt;

	do {
		fn(tfm, dst, src);
		crypto_xor(dst, iv, bsize);
		iv = src;

		src += bsize;
		dst += bsize;
	} while ((nbytes -= bsize) >= bsize);

	memcpy(walk->iv, iv, bsize);

	return nbytes;
}

static int crypto_cbc_decrypt_inplace(struct skcipher_walk *walk,
				      struct crypto_skcipher *skcipher)
{
	unsigned int bsize = crypto_skcipher_blocksize(skcipher);
	void (*fn)(struct crypto_tfm *, u8 *, const u8 *);
	unsigned int nbytes = walk->nbytes;
	u8 *src = walk->src.virt.addr;
	u8 last_iv[MAX_CIPHER_BLOCKSIZE];
	struct crypto_cipher *cipher;
	struct crypto_tfm *tfm;

	cipher = skcipher_cipher_simple(skcipher);
	tfm = crypto_cipher_tfm(cipher);
	fn = crypto_cipher_alg(cipher)->cia_decrypt;

	/* Start of the last block. */
	src += nbytes - (nbytes & (bsize - 1)) - bsize;
	memcpy(last_iv, src, bsize);

	for (;;) {
		fn(tfm, src, src);
		if ((nbytes -= bsize) < bsize)
			break;
		crypto_xor(src, src - bsize, bsize);
		src -= bsize;
	}

	crypto_xor(src, walk->iv, bsize);
	memcpy(walk->iv, last_iv, bsize);

	return nbytes;
}

static int crypto_cbc_decrypt(struct skcipher_request *req)
{
	struct crypto_skcipher *skcipher = crypto_skcipher_reqtfm(req);
	struct skcipher_walk walk;
	int err;

	err = skcipher_walk_virt(&walk, req, false);

	while (walk.nbytes) {
		if (walk.src.virt.addr == walk.dst.virt.addr)
			err = crypto_cbc_decrypt_inplace(&walk, skcipher);
		else
			err = crypto_cbc_decrypt_segment(&walk, skcipher);
		err = skcipher_walk_done(&walk, err);
	}

	return err;
>>>>>>> upstream/android-13
}

static int crypto_cbc_create(struct crypto_template *tmpl, struct rtattr **tb)
{
	struct skcipher_instance *inst;
<<<<<<< HEAD
	struct crypto_attr_type *algt;
	struct crypto_spawn *spawn;
	struct crypto_alg *alg;
	u32 mask;
	int err;

	err = crypto_check_attr_type(tb, CRYPTO_ALG_TYPE_SKCIPHER);
	if (err)
		return err;

	inst = kzalloc(sizeof(*inst) + sizeof(*spawn), GFP_KERNEL);
	if (!inst)
		return -ENOMEM;

	algt = crypto_get_attr_type(tb);
	err = PTR_ERR(algt);
	if (IS_ERR(algt))
		goto err_free_inst;

	mask = CRYPTO_ALG_TYPE_MASK |
		crypto_requires_off(algt->type, algt->mask,
				    CRYPTO_ALG_NEED_FALLBACK);

	alg = crypto_get_attr_alg(tb, CRYPTO_ALG_TYPE_CIPHER, mask);
	err = PTR_ERR(alg);
	if (IS_ERR(alg))
		goto err_free_inst;

	spawn = skcipher_instance_ctx(inst);
	err = crypto_init_spawn(spawn, alg, skcipher_crypto_instance(inst),
				CRYPTO_ALG_TYPE_MASK);
	if (err)
		goto err_put_alg;

	err = crypto_inst_setname(skcipher_crypto_instance(inst), "cbc", alg);
	if (err)
		goto err_drop_spawn;

	err = -EINVAL;
	if (!is_power_of_2(alg->cra_blocksize))
		goto err_drop_spawn;

	inst->alg.base.cra_priority = alg->cra_priority;
	inst->alg.base.cra_blocksize = alg->cra_blocksize;
	inst->alg.base.cra_alignmask = alg->cra_alignmask;

	inst->alg.ivsize = alg->cra_blocksize;
	inst->alg.min_keysize = alg->cra_cipher.cia_min_keysize;
	inst->alg.max_keysize = alg->cra_cipher.cia_max_keysize;

	inst->alg.base.cra_ctxsize = sizeof(struct crypto_cbc_ctx);

	inst->alg.init = crypto_cbc_init_tfm;
	inst->alg.exit = crypto_cbc_exit_tfm;

	inst->alg.setkey = crypto_cbc_setkey;
	inst->alg.encrypt = crypto_cbc_encrypt;
	inst->alg.decrypt = crypto_cbc_decrypt;

	inst->free = crypto_cbc_free;

	err = skcipher_register_instance(tmpl, inst);
	if (err)
		goto err_drop_spawn;
	crypto_mod_put(alg);

out:
	return err;

err_drop_spawn:
	crypto_drop_spawn(spawn);
err_put_alg:
	crypto_mod_put(alg);
err_free_inst:
	kfree(inst);
	goto out;
=======
	struct crypto_alg *alg;
	int err;

	inst = skcipher_alloc_instance_simple(tmpl, tb);
	if (IS_ERR(inst))
		return PTR_ERR(inst);

	alg = skcipher_ialg_simple(inst);

	err = -EINVAL;
	if (!is_power_of_2(alg->cra_blocksize))
		goto out_free_inst;

	inst->alg.encrypt = crypto_cbc_encrypt;
	inst->alg.decrypt = crypto_cbc_decrypt;

	err = skcipher_register_instance(tmpl, inst);
	if (err) {
out_free_inst:
		inst->free(inst);
	}

	return err;
>>>>>>> upstream/android-13
}

static struct crypto_template crypto_cbc_tmpl = {
	.name = "cbc",
	.create = crypto_cbc_create,
	.module = THIS_MODULE,
};

static int __init crypto_cbc_module_init(void)
{
	return crypto_register_template(&crypto_cbc_tmpl);
}

static void __exit crypto_cbc_module_exit(void)
{
	crypto_unregister_template(&crypto_cbc_tmpl);
}

<<<<<<< HEAD
module_init(crypto_cbc_module_init);
module_exit(crypto_cbc_module_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("CBC block cipher algorithm");
=======
subsys_initcall(crypto_cbc_module_init);
module_exit(crypto_cbc_module_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("CBC block cipher mode of operation");
>>>>>>> upstream/android-13
MODULE_ALIAS_CRYPTO("cbc");
