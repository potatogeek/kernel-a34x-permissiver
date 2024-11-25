<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> upstream/android-13
/*
 * Synchronous Cryptographic Hash operations.
 *
 * Copyright (c) 2008 Herbert Xu <herbert@gondor.apana.org.au>
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version.
 *
=======
>>>>>>> upstream/android-13
 */

#include <crypto/scatterwalk.h>
#include <crypto/internal/hash.h>
#include <linux/err.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/seq_file.h>
#include <linux/cryptouser.h>
#include <net/netlink.h>
#include <linux/compiler.h>

#include "internal.h"

static const struct crypto_type crypto_shash_type;

<<<<<<< HEAD
int shash_no_setkey(struct crypto_shash *tfm, const u8 *key,
		    unsigned int keylen)
{
	return -ENOSYS;
}
EXPORT_SYMBOL_GPL(shash_no_setkey);
=======
static int shash_no_setkey(struct crypto_shash *tfm, const u8 *key,
			   unsigned int keylen)
{
	return -ENOSYS;
}

/*
 * Check whether an shash algorithm has a setkey function.
 *
 * For CFI compatibility, this must not be an inline function.  This is because
 * when CFI is enabled, modules won't get the same address for shash_no_setkey
 * (if it were exported, which inlining would require) as the core kernel will.
 */
bool crypto_shash_alg_has_setkey(struct shash_alg *alg)
{
	return alg->setkey != shash_no_setkey;
}
EXPORT_SYMBOL_GPL(crypto_shash_alg_has_setkey);
>>>>>>> upstream/android-13

static int shash_setkey_unaligned(struct crypto_shash *tfm, const u8 *key,
				  unsigned int keylen)
{
	struct shash_alg *shash = crypto_shash_alg(tfm);
	unsigned long alignmask = crypto_shash_alignmask(tfm);
	unsigned long absize;
	u8 *buffer, *alignbuffer;
	int err;

	absize = keylen + (alignmask & ~(crypto_tfm_ctx_alignment() - 1));
	buffer = kmalloc(absize, GFP_ATOMIC);
	if (!buffer)
		return -ENOMEM;

	alignbuffer = (u8 *)ALIGN((unsigned long)buffer, alignmask + 1);
	memcpy(alignbuffer, key, keylen);
	err = shash->setkey(tfm, alignbuffer, keylen);
<<<<<<< HEAD
	kzfree(buffer);
=======
	kfree_sensitive(buffer);
>>>>>>> upstream/android-13
	return err;
}

static void shash_set_needkey(struct crypto_shash *tfm, struct shash_alg *alg)
{
<<<<<<< HEAD
	if (crypto_shash_alg_has_setkey(alg) &&
	    !(alg->base.cra_flags & CRYPTO_ALG_OPTIONAL_KEY))
=======
	if (crypto_shash_alg_needs_key(alg))
>>>>>>> upstream/android-13
		crypto_shash_set_flags(tfm, CRYPTO_TFM_NEED_KEY);
}

int crypto_shash_setkey(struct crypto_shash *tfm, const u8 *key,
			unsigned int keylen)
{
	struct shash_alg *shash = crypto_shash_alg(tfm);
	unsigned long alignmask = crypto_shash_alignmask(tfm);
	int err;

	if ((unsigned long)key & alignmask)
		err = shash_setkey_unaligned(tfm, key, keylen);
	else
		err = shash->setkey(tfm, key, keylen);

	if (unlikely(err)) {
		shash_set_needkey(tfm, shash);
		return err;
	}

	crypto_shash_clear_flags(tfm, CRYPTO_TFM_NEED_KEY);
	return 0;
}
EXPORT_SYMBOL_GPL(crypto_shash_setkey);

<<<<<<< HEAD
static inline unsigned int shash_align_buffer_size(unsigned len,
						   unsigned long mask)
{
	typedef u8 __aligned_largest u8_aligned;
	return len + (mask & ~(__alignof__(u8_aligned) - 1));
}

=======
>>>>>>> upstream/android-13
static int shash_update_unaligned(struct shash_desc *desc, const u8 *data,
				  unsigned int len)
{
	struct crypto_shash *tfm = desc->tfm;
	struct shash_alg *shash = crypto_shash_alg(tfm);
	unsigned long alignmask = crypto_shash_alignmask(tfm);
	unsigned int unaligned_len = alignmask + 1 -
				     ((unsigned long)data & alignmask);
<<<<<<< HEAD
	u8 ubuf[shash_align_buffer_size(unaligned_len, alignmask)]
		__aligned_largest;
	u8 *buf = PTR_ALIGN(&ubuf[0], alignmask + 1);
	int err;

=======
	/*
	 * We cannot count on __aligned() working for large values:
	 * https://patchwork.kernel.org/patch/9507697/
	 */
	u8 ubuf[MAX_ALGAPI_ALIGNMASK * 2];
	u8 *buf = PTR_ALIGN(&ubuf[0], alignmask + 1);
	int err;

	if (WARN_ON(buf + unaligned_len > ubuf + sizeof(ubuf)))
		return -EINVAL;

>>>>>>> upstream/android-13
	if (unaligned_len > len)
		unaligned_len = len;

	memcpy(buf, data, unaligned_len);
	err = shash->update(desc, buf, unaligned_len);
	memset(buf, 0, unaligned_len);

	return err ?:
	       shash->update(desc, data + unaligned_len, len - unaligned_len);
}

int crypto_shash_update(struct shash_desc *desc, const u8 *data,
			unsigned int len)
{
	struct crypto_shash *tfm = desc->tfm;
	struct shash_alg *shash = crypto_shash_alg(tfm);
	unsigned long alignmask = crypto_shash_alignmask(tfm);

	if ((unsigned long)data & alignmask)
		return shash_update_unaligned(desc, data, len);

	return shash->update(desc, data, len);
}
EXPORT_SYMBOL_GPL(crypto_shash_update);

static int shash_final_unaligned(struct shash_desc *desc, u8 *out)
{
	struct crypto_shash *tfm = desc->tfm;
	unsigned long alignmask = crypto_shash_alignmask(tfm);
	struct shash_alg *shash = crypto_shash_alg(tfm);
	unsigned int ds = crypto_shash_digestsize(tfm);
<<<<<<< HEAD
	u8 ubuf[shash_align_buffer_size(ds, alignmask)]
		__aligned_largest;
	u8 *buf = PTR_ALIGN(&ubuf[0], alignmask + 1);
	int err;

=======
	/*
	 * We cannot count on __aligned() working for large values:
	 * https://patchwork.kernel.org/patch/9507697/
	 */
	u8 ubuf[MAX_ALGAPI_ALIGNMASK + HASH_MAX_DIGESTSIZE];
	u8 *buf = PTR_ALIGN(&ubuf[0], alignmask + 1);
	int err;

	if (WARN_ON(buf + ds > ubuf + sizeof(ubuf)))
		return -EINVAL;

>>>>>>> upstream/android-13
	err = shash->final(desc, buf);
	if (err)
		goto out;

	memcpy(out, buf, ds);

out:
	memset(buf, 0, ds);
	return err;
}

int crypto_shash_final(struct shash_desc *desc, u8 *out)
{
	struct crypto_shash *tfm = desc->tfm;
	struct shash_alg *shash = crypto_shash_alg(tfm);
	unsigned long alignmask = crypto_shash_alignmask(tfm);

	if ((unsigned long)out & alignmask)
		return shash_final_unaligned(desc, out);

	return shash->final(desc, out);
}
EXPORT_SYMBOL_GPL(crypto_shash_final);

static int shash_finup_unaligned(struct shash_desc *desc, const u8 *data,
				 unsigned int len, u8 *out)
{
	return crypto_shash_update(desc, data, len) ?:
	       crypto_shash_final(desc, out);
}

int crypto_shash_finup(struct shash_desc *desc, const u8 *data,
		       unsigned int len, u8 *out)
{
	struct crypto_shash *tfm = desc->tfm;
	struct shash_alg *shash = crypto_shash_alg(tfm);
	unsigned long alignmask = crypto_shash_alignmask(tfm);

	if (((unsigned long)data | (unsigned long)out) & alignmask)
		return shash_finup_unaligned(desc, data, len, out);

	return shash->finup(desc, data, len, out);
}
EXPORT_SYMBOL_GPL(crypto_shash_finup);

static int shash_digest_unaligned(struct shash_desc *desc, const u8 *data,
				  unsigned int len, u8 *out)
{
	return crypto_shash_init(desc) ?:
	       crypto_shash_finup(desc, data, len, out);
}

int crypto_shash_digest(struct shash_desc *desc, const u8 *data,
			unsigned int len, u8 *out)
{
	struct crypto_shash *tfm = desc->tfm;
	struct shash_alg *shash = crypto_shash_alg(tfm);
	unsigned long alignmask = crypto_shash_alignmask(tfm);

	if (crypto_shash_get_flags(tfm) & CRYPTO_TFM_NEED_KEY)
		return -ENOKEY;

	if (((unsigned long)data | (unsigned long)out) & alignmask)
		return shash_digest_unaligned(desc, data, len, out);

	return shash->digest(desc, data, len, out);
}
EXPORT_SYMBOL_GPL(crypto_shash_digest);

<<<<<<< HEAD
=======
int crypto_shash_tfm_digest(struct crypto_shash *tfm, const u8 *data,
			    unsigned int len, u8 *out)
{
	SHASH_DESC_ON_STACK(desc, tfm);
	int err;

	desc->tfm = tfm;

	err = crypto_shash_digest(desc, data, len, out);

	shash_desc_zero(desc);

	return err;
}
EXPORT_SYMBOL_GPL(crypto_shash_tfm_digest);

>>>>>>> upstream/android-13
static int shash_default_export(struct shash_desc *desc, void *out)
{
	memcpy(out, shash_desc_ctx(desc), crypto_shash_descsize(desc->tfm));
	return 0;
}

static int shash_default_import(struct shash_desc *desc, const void *in)
{
	memcpy(shash_desc_ctx(desc), in, crypto_shash_descsize(desc->tfm));
	return 0;
}

static int shash_async_setkey(struct crypto_ahash *tfm, const u8 *key,
			      unsigned int keylen)
{
	struct crypto_shash **ctx = crypto_ahash_ctx(tfm);

	return crypto_shash_setkey(*ctx, key, keylen);
}

static int shash_async_init(struct ahash_request *req)
{
	struct crypto_shash **ctx = crypto_ahash_ctx(crypto_ahash_reqtfm(req));
	struct shash_desc *desc = ahash_request_ctx(req);

	desc->tfm = *ctx;
<<<<<<< HEAD
	desc->flags = req->base.flags;
=======
>>>>>>> upstream/android-13

	return crypto_shash_init(desc);
}

int shash_ahash_update(struct ahash_request *req, struct shash_desc *desc)
{
	struct crypto_hash_walk walk;
	int nbytes;

	for (nbytes = crypto_hash_walk_first(req, &walk); nbytes > 0;
	     nbytes = crypto_hash_walk_done(&walk, nbytes))
		nbytes = crypto_shash_update(desc, walk.data, nbytes);

	return nbytes;
}
EXPORT_SYMBOL_GPL(shash_ahash_update);

static int shash_async_update(struct ahash_request *req)
{
	return shash_ahash_update(req, ahash_request_ctx(req));
}

static int shash_async_final(struct ahash_request *req)
{
	return crypto_shash_final(ahash_request_ctx(req), req->result);
}

int shash_ahash_finup(struct ahash_request *req, struct shash_desc *desc)
{
	struct crypto_hash_walk walk;
	int nbytes;

	nbytes = crypto_hash_walk_first(req, &walk);
	if (!nbytes)
		return crypto_shash_final(desc, req->result);

	do {
		nbytes = crypto_hash_walk_last(&walk) ?
			 crypto_shash_finup(desc, walk.data, nbytes,
					    req->result) :
			 crypto_shash_update(desc, walk.data, nbytes);
		nbytes = crypto_hash_walk_done(&walk, nbytes);
	} while (nbytes > 0);

	return nbytes;
}
EXPORT_SYMBOL_GPL(shash_ahash_finup);

static int shash_async_finup(struct ahash_request *req)
{
	struct crypto_shash **ctx = crypto_ahash_ctx(crypto_ahash_reqtfm(req));
	struct shash_desc *desc = ahash_request_ctx(req);

	desc->tfm = *ctx;
<<<<<<< HEAD
	desc->flags = req->base.flags;
=======
>>>>>>> upstream/android-13

	return shash_ahash_finup(req, desc);
}

int shash_ahash_digest(struct ahash_request *req, struct shash_desc *desc)
{
	unsigned int nbytes = req->nbytes;
	struct scatterlist *sg;
	unsigned int offset;
	int err;

	if (nbytes &&
	    (sg = req->src, offset = sg->offset,
<<<<<<< HEAD
	     nbytes < min(sg->length, ((unsigned int)(PAGE_SIZE)) - offset))) {
=======
	     nbytes <= min(sg->length, ((unsigned int)(PAGE_SIZE)) - offset))) {
>>>>>>> upstream/android-13
		void *data;

		data = kmap_atomic(sg_page(sg));
		err = crypto_shash_digest(desc, data + offset, nbytes,
					  req->result);
		kunmap_atomic(data);
<<<<<<< HEAD
		crypto_yield(desc->flags);
=======
>>>>>>> upstream/android-13
	} else
		err = crypto_shash_init(desc) ?:
		      shash_ahash_finup(req, desc);

	return err;
}
EXPORT_SYMBOL_GPL(shash_ahash_digest);

static int shash_async_digest(struct ahash_request *req)
{
	struct crypto_shash **ctx = crypto_ahash_ctx(crypto_ahash_reqtfm(req));
	struct shash_desc *desc = ahash_request_ctx(req);

	desc->tfm = *ctx;
<<<<<<< HEAD
	desc->flags = req->base.flags;
=======
>>>>>>> upstream/android-13

	return shash_ahash_digest(req, desc);
}

static int shash_async_export(struct ahash_request *req, void *out)
{
	return crypto_shash_export(ahash_request_ctx(req), out);
}

static int shash_async_import(struct ahash_request *req, const void *in)
{
	struct crypto_shash **ctx = crypto_ahash_ctx(crypto_ahash_reqtfm(req));
	struct shash_desc *desc = ahash_request_ctx(req);

	desc->tfm = *ctx;
<<<<<<< HEAD
	desc->flags = req->base.flags;
=======
>>>>>>> upstream/android-13

	return crypto_shash_import(desc, in);
}

static void crypto_exit_shash_ops_async(struct crypto_tfm *tfm)
{
	struct crypto_shash **ctx = crypto_tfm_ctx(tfm);

	crypto_free_shash(*ctx);
}

int crypto_init_shash_ops_async(struct crypto_tfm *tfm)
{
	struct crypto_alg *calg = tfm->__crt_alg;
	struct shash_alg *alg = __crypto_shash_alg(calg);
	struct crypto_ahash *crt = __crypto_ahash_cast(tfm);
	struct crypto_shash **ctx = crypto_tfm_ctx(tfm);
	struct crypto_shash *shash;

	if (!crypto_mod_get(calg))
		return -EAGAIN;

	shash = crypto_create_tfm(calg, &crypto_shash_type);
	if (IS_ERR(shash)) {
		crypto_mod_put(calg);
		return PTR_ERR(shash);
	}

	*ctx = shash;
	tfm->exit = crypto_exit_shash_ops_async;

	crt->init = shash_async_init;
	crt->update = shash_async_update;
	crt->final = shash_async_final;
	crt->finup = shash_async_finup;
	crt->digest = shash_async_digest;
	if (crypto_shash_alg_has_setkey(alg))
		crt->setkey = shash_async_setkey;

	crypto_ahash_set_flags(crt, crypto_shash_get_flags(shash) &
				    CRYPTO_TFM_NEED_KEY);

<<<<<<< HEAD
	if (alg->export)
		crt->export = shash_async_export;
	if (alg->import)
		crt->import = shash_async_import;
=======
	crt->export = shash_async_export;
	crt->import = shash_async_import;
>>>>>>> upstream/android-13

	crt->reqsize = sizeof(struct shash_desc) + crypto_shash_descsize(shash);

	return 0;
}

<<<<<<< HEAD
=======
static void crypto_shash_exit_tfm(struct crypto_tfm *tfm)
{
	struct crypto_shash *hash = __crypto_shash_cast(tfm);
	struct shash_alg *alg = crypto_shash_alg(hash);

	alg->exit_tfm(hash);
}

>>>>>>> upstream/android-13
static int crypto_shash_init_tfm(struct crypto_tfm *tfm)
{
	struct crypto_shash *hash = __crypto_shash_cast(tfm);
	struct shash_alg *alg = crypto_shash_alg(hash);
<<<<<<< HEAD
=======
	int err;
>>>>>>> upstream/android-13

	hash->descsize = alg->descsize;

	shash_set_needkey(hash, alg);

<<<<<<< HEAD
	return 0;
}

=======
	if (alg->exit_tfm)
		tfm->exit = crypto_shash_exit_tfm;

	if (!alg->init_tfm)
		return 0;

	err = alg->init_tfm(hash);
	if (err)
		return err;

	/* ->init_tfm() may have increased the descsize. */
	if (WARN_ON_ONCE(hash->descsize > HASH_MAX_DESCSIZE)) {
		if (alg->exit_tfm)
			alg->exit_tfm(hash);
		return -EINVAL;
	}

	return 0;
}

static void crypto_shash_free_instance(struct crypto_instance *inst)
{
	struct shash_instance *shash = shash_instance(inst);

	shash->free(shash);
}

>>>>>>> upstream/android-13
#ifdef CONFIG_NET
static int crypto_shash_report(struct sk_buff *skb, struct crypto_alg *alg)
{
	struct crypto_report_hash rhash;
	struct shash_alg *salg = __crypto_shash_alg(alg);

<<<<<<< HEAD
	strncpy(rhash.type, "shash", sizeof(rhash.type));
=======
	memset(&rhash, 0, sizeof(rhash));

	strscpy(rhash.type, "shash", sizeof(rhash.type));
>>>>>>> upstream/android-13

	rhash.blocksize = alg->cra_blocksize;
	rhash.digestsize = salg->digestsize;

<<<<<<< HEAD
	if (nla_put(skb, CRYPTOCFGA_REPORT_HASH,
		    sizeof(struct crypto_report_hash), &rhash))
		goto nla_put_failure;
	return 0;

nla_put_failure:
	return -EMSGSIZE;
=======
	return nla_put(skb, CRYPTOCFGA_REPORT_HASH, sizeof(rhash), &rhash);
>>>>>>> upstream/android-13
}
#else
static int crypto_shash_report(struct sk_buff *skb, struct crypto_alg *alg)
{
	return -ENOSYS;
}
#endif

static void crypto_shash_show(struct seq_file *m, struct crypto_alg *alg)
	__maybe_unused;
static void crypto_shash_show(struct seq_file *m, struct crypto_alg *alg)
{
	struct shash_alg *salg = __crypto_shash_alg(alg);

	seq_printf(m, "type         : shash\n");
	seq_printf(m, "blocksize    : %u\n", alg->cra_blocksize);
	seq_printf(m, "digestsize   : %u\n", salg->digestsize);
}

static const struct crypto_type crypto_shash_type = {
	.extsize = crypto_alg_extsize,
	.init_tfm = crypto_shash_init_tfm,
<<<<<<< HEAD
=======
	.free = crypto_shash_free_instance,
>>>>>>> upstream/android-13
#ifdef CONFIG_PROC_FS
	.show = crypto_shash_show,
#endif
	.report = crypto_shash_report,
	.maskclear = ~CRYPTO_ALG_TYPE_MASK,
	.maskset = CRYPTO_ALG_TYPE_MASK,
	.type = CRYPTO_ALG_TYPE_SHASH,
	.tfmsize = offsetof(struct crypto_shash, base),
};

<<<<<<< HEAD
=======
int crypto_grab_shash(struct crypto_shash_spawn *spawn,
		      struct crypto_instance *inst,
		      const char *name, u32 type, u32 mask)
{
	spawn->base.frontend = &crypto_shash_type;
	return crypto_grab_spawn(&spawn->base, inst, name, type, mask);
}
EXPORT_SYMBOL_GPL(crypto_grab_shash);

>>>>>>> upstream/android-13
struct crypto_shash *crypto_alloc_shash(const char *alg_name, u32 type,
					u32 mask)
{
	return crypto_alloc_tfm(alg_name, &crypto_shash_type, type, mask);
}
EXPORT_SYMBOL_GPL(crypto_alloc_shash);

static int shash_prepare_alg(struct shash_alg *alg)
{
	struct crypto_alg *base = &alg->base;

<<<<<<< HEAD
	if (alg->digestsize > PAGE_SIZE / 8 ||
	    alg->descsize > PAGE_SIZE / 8 ||
	    alg->statesize > PAGE_SIZE / 8)
=======
	if (alg->digestsize > HASH_MAX_DIGESTSIZE ||
	    alg->descsize > HASH_MAX_DESCSIZE ||
	    alg->statesize > HASH_MAX_STATESIZE)
		return -EINVAL;

	if ((alg->export && !alg->import) || (alg->import && !alg->export))
>>>>>>> upstream/android-13
		return -EINVAL;

	base->cra_type = &crypto_shash_type;
	base->cra_flags &= ~CRYPTO_ALG_TYPE_MASK;
	base->cra_flags |= CRYPTO_ALG_TYPE_SHASH;

	if (!alg->finup)
		alg->finup = shash_finup_unaligned;
	if (!alg->digest)
		alg->digest = shash_digest_unaligned;
	if (!alg->export) {
		alg->export = shash_default_export;
		alg->import = shash_default_import;
		alg->statesize = alg->descsize;
	}
	if (!alg->setkey)
		alg->setkey = shash_no_setkey;

	return 0;
}

int crypto_register_shash(struct shash_alg *alg)
{
	struct crypto_alg *base = &alg->base;
	int err;

	err = shash_prepare_alg(alg);
	if (err)
		return err;

	return crypto_register_alg(base);
}
EXPORT_SYMBOL_GPL(crypto_register_shash);

<<<<<<< HEAD
int crypto_unregister_shash(struct shash_alg *alg)
{
	return crypto_unregister_alg(&alg->base);
=======
void crypto_unregister_shash(struct shash_alg *alg)
{
	crypto_unregister_alg(&alg->base);
>>>>>>> upstream/android-13
}
EXPORT_SYMBOL_GPL(crypto_unregister_shash);

int crypto_register_shashes(struct shash_alg *algs, int count)
{
	int i, ret;

	for (i = 0; i < count; i++) {
		ret = crypto_register_shash(&algs[i]);
		if (ret)
			goto err;
	}

	return 0;

err:
	for (--i; i >= 0; --i)
		crypto_unregister_shash(&algs[i]);

	return ret;
}
EXPORT_SYMBOL_GPL(crypto_register_shashes);

<<<<<<< HEAD
int crypto_unregister_shashes(struct shash_alg *algs, int count)
{
	int i, ret;

	for (i = count - 1; i >= 0; --i) {
		ret = crypto_unregister_shash(&algs[i]);
		if (ret)
			pr_err("Failed to unregister %s %s: %d\n",
			       algs[i].base.cra_driver_name,
			       algs[i].base.cra_name, ret);
	}

	return 0;
=======
void crypto_unregister_shashes(struct shash_alg *algs, int count)
{
	int i;

	for (i = count - 1; i >= 0; --i)
		crypto_unregister_shash(&algs[i]);
>>>>>>> upstream/android-13
}
EXPORT_SYMBOL_GPL(crypto_unregister_shashes);

int shash_register_instance(struct crypto_template *tmpl,
			    struct shash_instance *inst)
{
	int err;

<<<<<<< HEAD
=======
	if (WARN_ON(!inst->free))
		return -EINVAL;

>>>>>>> upstream/android-13
	err = shash_prepare_alg(&inst->alg);
	if (err)
		return err;

	return crypto_register_instance(tmpl, shash_crypto_instance(inst));
}
EXPORT_SYMBOL_GPL(shash_register_instance);

<<<<<<< HEAD
void shash_free_instance(struct crypto_instance *inst)
{
	crypto_drop_spawn(crypto_instance_ctx(inst));
	kfree(shash_instance(inst));
}
EXPORT_SYMBOL_GPL(shash_free_instance);

int crypto_init_shash_spawn(struct crypto_shash_spawn *spawn,
			    struct shash_alg *alg,
			    struct crypto_instance *inst)
{
	return crypto_init_spawn2(&spawn->base, &alg->base, inst,
				  &crypto_shash_type);
}
EXPORT_SYMBOL_GPL(crypto_init_shash_spawn);

struct shash_alg *shash_attr_alg(struct rtattr *rta, u32 type, u32 mask)
{
	struct crypto_alg *alg;

	alg = crypto_attr_alg2(rta, &crypto_shash_type, type, mask);
	return IS_ERR(alg) ? ERR_CAST(alg) :
	       container_of(alg, struct shash_alg, base);
}
EXPORT_SYMBOL_GPL(shash_attr_alg);
=======
void shash_free_singlespawn_instance(struct shash_instance *inst)
{
	crypto_drop_spawn(shash_instance_ctx(inst));
	kfree(inst);
}
EXPORT_SYMBOL_GPL(shash_free_singlespawn_instance);
>>>>>>> upstream/android-13

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Synchronous cryptographic hash type");
