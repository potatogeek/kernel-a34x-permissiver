<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> upstream/android-13
/* LRW: as defined by Cyril Guyot in
 *	http://grouper.ieee.org/groups/1619/email/pdf00017.pdf
 *
 * Copyright (c) 2006 Rik Snel <rsnel@cube.dyndns.org>
 *
 * Based on ecb.c
 * Copyright (c) 2006 Herbert Xu <herbert@gondor.apana.org.au>
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version.
 */
/* This implementation is checked against the test vectors in the above
 * document and by a test vector provided by Ken Buchanan at
 * http://www.mail-archive.com/stds-p1619@listserv.ieee.org/msg00173.html
=======
 */
/* This implementation is checked against the test vectors in the above
 * document and by a test vector provided by Ken Buchanan at
 * https://www.mail-archive.com/stds-p1619@listserv.ieee.org/msg00173.html
>>>>>>> upstream/android-13
 *
 * The test vectors are included in the testing module tcrypt.[ch] */

#include <crypto/internal/skcipher.h>
#include <crypto/scatterwalk.h>
#include <linux/err.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/scatterlist.h>
#include <linux/slab.h>

#include <crypto/b128ops.h>
#include <crypto/gf128mul.h>

<<<<<<< HEAD
#define LRW_BUFFER_SIZE 128u

#define LRW_BLOCK_SIZE 16

struct priv {
=======
#define LRW_BLOCK_SIZE 16

struct lrw_tfm_ctx {
>>>>>>> upstream/android-13
	struct crypto_skcipher *child;

	/*
	 * optimizes multiplying a random (non incrementing, as at the
	 * start of a new sector) value with key2, we could also have
	 * used 4k optimization tables or no optimization at all. In the
	 * latter case we would have to store key2 here
	 */
	struct gf128mul_64k *table;

	/*
	 * stores:
	 *  key2*{ 0,0,...0,0,0,0,1 }, key2*{ 0,0,...0,0,0,1,1 },
	 *  key2*{ 0,0,...0,0,1,1,1 }, key2*{ 0,0,...0,1,1,1,1 }
	 *  key2*{ 0,0,...1,1,1,1,1 }, etc
	 * needed for optimized multiplication of incrementing values
	 * with key2
	 */
	be128 mulinc[128];
};

<<<<<<< HEAD
struct rctx {
	be128 buf[LRW_BUFFER_SIZE / sizeof(be128)];

	be128 t;

	be128 *ext;

	struct scatterlist srcbuf[2];
	struct scatterlist dstbuf[2];
	struct scatterlist *src;
	struct scatterlist *dst;

	unsigned int left;

	struct skcipher_request subreq;
};

static inline void setbit128_bbe(void *b, int bit)
=======
struct lrw_request_ctx {
	be128 t;
	struct skcipher_request subreq;
};

static inline void lrw_setbit128_bbe(void *b, int bit)
>>>>>>> upstream/android-13
{
	__set_bit(bit ^ (0x80 -
#ifdef __BIG_ENDIAN
			 BITS_PER_LONG
#else
			 BITS_PER_BYTE
#endif
			), b);
}

<<<<<<< HEAD
static int setkey(struct crypto_skcipher *parent, const u8 *key,
		  unsigned int keylen)
{
	struct priv *ctx = crypto_skcipher_ctx(parent);
=======
static int lrw_setkey(struct crypto_skcipher *parent, const u8 *key,
		      unsigned int keylen)
{
	struct lrw_tfm_ctx *ctx = crypto_skcipher_ctx(parent);
>>>>>>> upstream/android-13
	struct crypto_skcipher *child = ctx->child;
	int err, bsize = LRW_BLOCK_SIZE;
	const u8 *tweak = key + keylen - bsize;
	be128 tmp = { 0 };
	int i;

	crypto_skcipher_clear_flags(child, CRYPTO_TFM_REQ_MASK);
	crypto_skcipher_set_flags(child, crypto_skcipher_get_flags(parent) &
					 CRYPTO_TFM_REQ_MASK);
	err = crypto_skcipher_setkey(child, key, keylen - bsize);
<<<<<<< HEAD
	crypto_skcipher_set_flags(parent, crypto_skcipher_get_flags(child) &
					  CRYPTO_TFM_RES_MASK);
=======
>>>>>>> upstream/android-13
	if (err)
		return err;

	if (ctx->table)
		gf128mul_free_64k(ctx->table);

	/* initialize multiplication table for Key2 */
	ctx->table = gf128mul_init_64k_bbe((be128 *)tweak);
	if (!ctx->table)
		return -ENOMEM;

	/* initialize optimization table */
	for (i = 0; i < 128; i++) {
<<<<<<< HEAD
		setbit128_bbe(&tmp, i);
=======
		lrw_setbit128_bbe(&tmp, i);
>>>>>>> upstream/android-13
		ctx->mulinc[i] = tmp;
		gf128mul_64k_bbe(&ctx->mulinc[i], ctx->table);
	}

	return 0;
}

<<<<<<< HEAD
static inline void inc(be128 *iv)
{
	be64_add_cpu(&iv->b, 1);
	if (!iv->b)
		be64_add_cpu(&iv->a, 1);
}

/* this returns the number of consequative 1 bits starting
 * from the right, get_index128(00 00 00 00 00 00 ... 00 00 10 FB) = 2 */
static inline int get_index128(be128 *block)
{
	int x;
	__be32 *p = (__be32 *) block;

	for (p += 3, x = 0; x < 128; p--, x += 32) {
		u32 val = be32_to_cpup(p);

		if (!~val)
			continue;

		return x + ffz(val);
=======
/*
 * Returns the number of trailing '1' bits in the words of the counter, which is
 * represented by 4 32-bit words, arranged from least to most significant.
 * At the same time, increments the counter by one.
 *
 * For example:
 *
 * u32 counter[4] = { 0xFFFFFFFF, 0x1, 0x0, 0x0 };
 * int i = lrw_next_index(&counter);
 * // i == 33, counter == { 0x0, 0x2, 0x0, 0x0 }
 */
static int lrw_next_index(u32 *counter)
{
	int i, res = 0;

	for (i = 0; i < 4; i++) {
		if (counter[i] + 1 != 0)
			return res + ffz(counter[i]++);

		counter[i] = 0;
		res += 32;
>>>>>>> upstream/android-13
	}

	/*
	 * If we get here, then x == 128 and we are incrementing the counter
	 * from all ones to all zeros. This means we must return index 127, i.e.
	 * the one corresponding to key2*{ 1,...,1 }.
	 */
	return 127;
}

<<<<<<< HEAD
static int post_crypt(struct skcipher_request *req)
{
	struct rctx *rctx = skcipher_request_ctx(req);
	be128 *buf = rctx->ext ?: rctx->buf;
	struct skcipher_request *subreq;
	const int bs = LRW_BLOCK_SIZE;
	struct skcipher_walk w;
	struct scatterlist *sg;
	unsigned offset;
	int err;

	subreq = &rctx->subreq;
	err = skcipher_walk_virt(&w, subreq, false);

	while (w.nbytes) {
		unsigned int avail = w.nbytes;
		be128 *wdst;

		wdst = w.dst.virt.addr;

		do {
			be128_xor(wdst, buf++, wdst);
			wdst++;
		} while ((avail -= bs) >= bs);

		err = skcipher_walk_done(&w, avail);
	}

	rctx->left -= subreq->cryptlen;

	if (err || !rctx->left)
		goto out;

	rctx->dst = rctx->dstbuf;

	scatterwalk_done(&w.out, 0, 1);
	sg = w.out.sg;
	offset = w.out.offset;

	if (rctx->dst != sg) {
		rctx->dst[0] = *sg;
		sg_unmark_end(rctx->dst);
		scatterwalk_crypto_chain(rctx->dst, sg_next(sg), 2);
	}
	rctx->dst[0].length -= offset - sg->offset;
	rctx->dst[0].offset = offset;

out:
	return err;
}

static int pre_crypt(struct skcipher_request *req)
{
	struct crypto_skcipher *tfm = crypto_skcipher_reqtfm(req);
	struct rctx *rctx = skcipher_request_ctx(req);
	struct priv *ctx = crypto_skcipher_ctx(tfm);
	be128 *buf = rctx->ext ?: rctx->buf;
	struct skcipher_request *subreq;
	const int bs = LRW_BLOCK_SIZE;
	struct skcipher_walk w;
	struct scatterlist *sg;
	unsigned cryptlen;
	unsigned offset;
	be128 *iv;
	bool more;
	int err;

	subreq = &rctx->subreq;
	skcipher_request_set_tfm(subreq, tfm);

	cryptlen = subreq->cryptlen;
	more = rctx->left > cryptlen;
	if (!more)
		cryptlen = rctx->left;

	skcipher_request_set_crypt(subreq, rctx->src, rctx->dst,
				   cryptlen, req->iv);

	err = skcipher_walk_virt(&w, subreq, false);
	iv = w.iv;
=======
/*
 * We compute the tweak masks twice (both before and after the ECB encryption or
 * decryption) to avoid having to allocate a temporary buffer and/or make
 * mutliple calls to the 'ecb(..)' instance, which usually would be slower than
 * just doing the lrw_next_index() calls again.
 */
static int lrw_xor_tweak(struct skcipher_request *req, bool second_pass)
{
	const int bs = LRW_BLOCK_SIZE;
	struct crypto_skcipher *tfm = crypto_skcipher_reqtfm(req);
	const struct lrw_tfm_ctx *ctx = crypto_skcipher_ctx(tfm);
	struct lrw_request_ctx *rctx = skcipher_request_ctx(req);
	be128 t = rctx->t;
	struct skcipher_walk w;
	__be32 *iv;
	u32 counter[4];
	int err;

	if (second_pass) {
		req = &rctx->subreq;
		/* set to our TFM to enforce correct alignment: */
		skcipher_request_set_tfm(req, tfm);
	}

	err = skcipher_walk_virt(&w, req, false);
	if (err)
		return err;

	iv = (__be32 *)w.iv;
	counter[0] = be32_to_cpu(iv[3]);
	counter[1] = be32_to_cpu(iv[2]);
	counter[2] = be32_to_cpu(iv[1]);
	counter[3] = be32_to_cpu(iv[0]);
>>>>>>> upstream/android-13

	while (w.nbytes) {
		unsigned int avail = w.nbytes;
		be128 *wsrc;
		be128 *wdst;

		wsrc = w.src.virt.addr;
		wdst = w.dst.virt.addr;

		do {
<<<<<<< HEAD
			*buf++ = rctx->t;
			be128_xor(wdst++, &rctx->t, wsrc++);

			/* T <- I*Key2, using the optimization
			 * discussed in the specification */
			be128_xor(&rctx->t, &rctx->t,
				  &ctx->mulinc[get_index128(iv)]);
			inc(iv);
		} while ((avail -= bs) >= bs);

		err = skcipher_walk_done(&w, avail);
	}

	skcipher_request_set_tfm(subreq, ctx->child);
	skcipher_request_set_crypt(subreq, rctx->dst, rctx->dst,
				   cryptlen, NULL);

	if (err || !more)
		goto out;

	rctx->src = rctx->srcbuf;

	scatterwalk_done(&w.in, 0, 1);
	sg = w.in.sg;
	offset = w.in.offset;

	if (rctx->src != sg) {
		rctx->src[0] = *sg;
		sg_unmark_end(rctx->src);
		scatterwalk_crypto_chain(rctx->src, sg_next(sg), 2);
	}
	rctx->src[0].length -= offset - sg->offset;
	rctx->src[0].offset = offset;

out:
	return err;
}

static int init_crypt(struct skcipher_request *req, crypto_completion_t done)
{
	struct priv *ctx = crypto_skcipher_ctx(crypto_skcipher_reqtfm(req));
	struct rctx *rctx = skcipher_request_ctx(req);
	struct skcipher_request *subreq;
	gfp_t gfp;

	subreq = &rctx->subreq;
	skcipher_request_set_callback(subreq, req->base.flags, done, req);

	gfp = req->base.flags & CRYPTO_TFM_REQ_MAY_SLEEP ? GFP_KERNEL :
							   GFP_ATOMIC;
	rctx->ext = NULL;

	subreq->cryptlen = LRW_BUFFER_SIZE;
	if (req->cryptlen > LRW_BUFFER_SIZE) {
		unsigned int n = min(req->cryptlen, (unsigned int)PAGE_SIZE);

		rctx->ext = kmalloc(n, gfp);
		if (rctx->ext)
			subreq->cryptlen = n;
	}

	rctx->src = req->src;
	rctx->dst = req->dst;
	rctx->left = req->cryptlen;
=======
			be128_xor(wdst++, &t, wsrc++);

			/* T <- I*Key2, using the optimization
			 * discussed in the specification */
			be128_xor(&t, &t,
				  &ctx->mulinc[lrw_next_index(counter)]);
		} while ((avail -= bs) >= bs);

		if (second_pass && w.nbytes == w.total) {
			iv[0] = cpu_to_be32(counter[3]);
			iv[1] = cpu_to_be32(counter[2]);
			iv[2] = cpu_to_be32(counter[1]);
			iv[3] = cpu_to_be32(counter[0]);
		}

		err = skcipher_walk_done(&w, avail);
	}

	return err;
}

static int lrw_xor_tweak_pre(struct skcipher_request *req)
{
	return lrw_xor_tweak(req, false);
}

static int lrw_xor_tweak_post(struct skcipher_request *req)
{
	return lrw_xor_tweak(req, true);
}

static void lrw_crypt_done(struct crypto_async_request *areq, int err)
{
	struct skcipher_request *req = areq->data;

	if (!err) {
		struct lrw_request_ctx *rctx = skcipher_request_ctx(req);

		rctx->subreq.base.flags &= ~CRYPTO_TFM_REQ_MAY_SLEEP;
		err = lrw_xor_tweak_post(req);
	}

	skcipher_request_complete(req, err);
}

static void lrw_init_crypt(struct skcipher_request *req)
{
	const struct lrw_tfm_ctx *ctx =
		crypto_skcipher_ctx(crypto_skcipher_reqtfm(req));
	struct lrw_request_ctx *rctx = skcipher_request_ctx(req);
	struct skcipher_request *subreq = &rctx->subreq;

	skcipher_request_set_tfm(subreq, ctx->child);
	skcipher_request_set_callback(subreq, req->base.flags, lrw_crypt_done,
				      req);
	/* pass req->iv as IV (will be used by xor_tweak, ECB will ignore it) */
	skcipher_request_set_crypt(subreq, req->dst, req->dst,
				   req->cryptlen, req->iv);
>>>>>>> upstream/android-13

	/* calculate first value of T */
	memcpy(&rctx->t, req->iv, sizeof(rctx->t));

	/* T <- I*Key2 */
	gf128mul_64k_bbe(&rctx->t, ctx->table);
<<<<<<< HEAD

	return 0;
}

static void exit_crypt(struct skcipher_request *req)
{
	struct rctx *rctx = skcipher_request_ctx(req);

	rctx->left = 0;

	if (rctx->ext)
		kzfree(rctx->ext);
}

static int do_encrypt(struct skcipher_request *req, int err)
{
	struct rctx *rctx = skcipher_request_ctx(req);
	struct skcipher_request *subreq;

	subreq = &rctx->subreq;

	while (!err && rctx->left) {
		err = pre_crypt(req) ?:
		      crypto_skcipher_encrypt(subreq) ?:
		      post_crypt(req);

		if (err == -EINPROGRESS || err == -EBUSY)
			return err;
	}

	exit_crypt(req);
	return err;
}

static void encrypt_done(struct crypto_async_request *areq, int err)
{
	struct skcipher_request *req = areq->data;
	struct skcipher_request *subreq;
	struct rctx *rctx;

	rctx = skcipher_request_ctx(req);

	if (err == -EINPROGRESS) {
		if (rctx->left != req->cryptlen)
			return;
		goto out;
	}

	subreq = &rctx->subreq;
	subreq->base.flags &= CRYPTO_TFM_REQ_MAY_BACKLOG;

	err = do_encrypt(req, err ?: post_crypt(req));
	if (rctx->left)
		return;

out:
	skcipher_request_complete(req, err);
}

static int encrypt(struct skcipher_request *req)
{
	return do_encrypt(req, init_crypt(req, encrypt_done));
}

static int do_decrypt(struct skcipher_request *req, int err)
{
	struct rctx *rctx = skcipher_request_ctx(req);
	struct skcipher_request *subreq;

	subreq = &rctx->subreq;

	while (!err && rctx->left) {
		err = pre_crypt(req) ?:
		      crypto_skcipher_decrypt(subreq) ?:
		      post_crypt(req);

		if (err == -EINPROGRESS || err == -EBUSY)
			return err;
	}

	exit_crypt(req);
	return err;
}

static void decrypt_done(struct crypto_async_request *areq, int err)
{
	struct skcipher_request *req = areq->data;
	struct skcipher_request *subreq;
	struct rctx *rctx;

	rctx = skcipher_request_ctx(req);

	if (err == -EINPROGRESS) {
		if (rctx->left != req->cryptlen)
			return;
		goto out;
	}

	subreq = &rctx->subreq;
	subreq->base.flags &= CRYPTO_TFM_REQ_MAY_BACKLOG;

	err = do_decrypt(req, err ?: post_crypt(req));
	if (rctx->left)
		return;

out:
	skcipher_request_complete(req, err);
}

static int decrypt(struct skcipher_request *req)
{
	return do_decrypt(req, init_crypt(req, decrypt_done));
}

static int init_tfm(struct crypto_skcipher *tfm)
{
	struct skcipher_instance *inst = skcipher_alg_instance(tfm);
	struct crypto_skcipher_spawn *spawn = skcipher_instance_ctx(inst);
	struct priv *ctx = crypto_skcipher_ctx(tfm);
=======
}

static int lrw_encrypt(struct skcipher_request *req)
{
	struct lrw_request_ctx *rctx = skcipher_request_ctx(req);
	struct skcipher_request *subreq = &rctx->subreq;

	lrw_init_crypt(req);
	return lrw_xor_tweak_pre(req) ?:
		crypto_skcipher_encrypt(subreq) ?:
		lrw_xor_tweak_post(req);
}

static int lrw_decrypt(struct skcipher_request *req)
{
	struct lrw_request_ctx *rctx = skcipher_request_ctx(req);
	struct skcipher_request *subreq = &rctx->subreq;

	lrw_init_crypt(req);
	return lrw_xor_tweak_pre(req) ?:
		crypto_skcipher_decrypt(subreq) ?:
		lrw_xor_tweak_post(req);
}

static int lrw_init_tfm(struct crypto_skcipher *tfm)
{
	struct skcipher_instance *inst = skcipher_alg_instance(tfm);
	struct crypto_skcipher_spawn *spawn = skcipher_instance_ctx(inst);
	struct lrw_tfm_ctx *ctx = crypto_skcipher_ctx(tfm);
>>>>>>> upstream/android-13
	struct crypto_skcipher *cipher;

	cipher = crypto_spawn_skcipher(spawn);
	if (IS_ERR(cipher))
		return PTR_ERR(cipher);

	ctx->child = cipher;

	crypto_skcipher_set_reqsize(tfm, crypto_skcipher_reqsize(cipher) +
<<<<<<< HEAD
					 sizeof(struct rctx));
=======
					 sizeof(struct lrw_request_ctx));
>>>>>>> upstream/android-13

	return 0;
}

<<<<<<< HEAD
static void exit_tfm(struct crypto_skcipher *tfm)
{
	struct priv *ctx = crypto_skcipher_ctx(tfm);
=======
static void lrw_exit_tfm(struct crypto_skcipher *tfm)
{
	struct lrw_tfm_ctx *ctx = crypto_skcipher_ctx(tfm);
>>>>>>> upstream/android-13

	if (ctx->table)
		gf128mul_free_64k(ctx->table);
	crypto_free_skcipher(ctx->child);
}

<<<<<<< HEAD
static void free_inst(struct skcipher_instance *inst)
=======
static void lrw_free_instance(struct skcipher_instance *inst)
>>>>>>> upstream/android-13
{
	crypto_drop_skcipher(skcipher_instance_ctx(inst));
	kfree(inst);
}

<<<<<<< HEAD
static int create(struct crypto_template *tmpl, struct rtattr **tb)
{
	struct crypto_skcipher_spawn *spawn;
	struct skcipher_instance *inst;
	struct crypto_attr_type *algt;
	struct skcipher_alg *alg;
	const char *cipher_name;
	char ecb_name[CRYPTO_MAX_ALG_NAME];
	int err;

	algt = crypto_get_attr_type(tb);
	if (IS_ERR(algt))
		return PTR_ERR(algt);

	if ((algt->type ^ CRYPTO_ALG_TYPE_SKCIPHER) & algt->mask)
		return -EINVAL;
=======
static int lrw_create(struct crypto_template *tmpl, struct rtattr **tb)
{
	struct crypto_skcipher_spawn *spawn;
	struct skcipher_instance *inst;
	struct skcipher_alg *alg;
	const char *cipher_name;
	char ecb_name[CRYPTO_MAX_ALG_NAME];
	u32 mask;
	int err;

	err = crypto_check_attr_type(tb, CRYPTO_ALG_TYPE_SKCIPHER, &mask);
	if (err)
		return err;
>>>>>>> upstream/android-13

	cipher_name = crypto_attr_alg_name(tb[1]);
	if (IS_ERR(cipher_name))
		return PTR_ERR(cipher_name);

	inst = kzalloc(sizeof(*inst) + sizeof(*spawn), GFP_KERNEL);
	if (!inst)
		return -ENOMEM;

	spawn = skcipher_instance_ctx(inst);

<<<<<<< HEAD
	crypto_set_skcipher_spawn(spawn, skcipher_crypto_instance(inst));
	err = crypto_grab_skcipher(spawn, cipher_name, 0,
				   crypto_requires_sync(algt->type,
							algt->mask));
=======
	err = crypto_grab_skcipher(spawn, skcipher_crypto_instance(inst),
				   cipher_name, 0, mask);
>>>>>>> upstream/android-13
	if (err == -ENOENT) {
		err = -ENAMETOOLONG;
		if (snprintf(ecb_name, CRYPTO_MAX_ALG_NAME, "ecb(%s)",
			     cipher_name) >= CRYPTO_MAX_ALG_NAME)
			goto err_free_inst;

<<<<<<< HEAD
		err = crypto_grab_skcipher(spawn, ecb_name, 0,
					   crypto_requires_sync(algt->type,
								algt->mask));
=======
		err = crypto_grab_skcipher(spawn,
					   skcipher_crypto_instance(inst),
					   ecb_name, 0, mask);
>>>>>>> upstream/android-13
	}

	if (err)
		goto err_free_inst;

	alg = crypto_skcipher_spawn_alg(spawn);

	err = -EINVAL;
	if (alg->base.cra_blocksize != LRW_BLOCK_SIZE)
<<<<<<< HEAD
		goto err_drop_spawn;

	if (crypto_skcipher_alg_ivsize(alg))
		goto err_drop_spawn;
=======
		goto err_free_inst;

	if (crypto_skcipher_alg_ivsize(alg))
		goto err_free_inst;
>>>>>>> upstream/android-13

	err = crypto_inst_setname(skcipher_crypto_instance(inst), "lrw",
				  &alg->base);
	if (err)
<<<<<<< HEAD
		goto err_drop_spawn;
=======
		goto err_free_inst;
>>>>>>> upstream/android-13

	err = -EINVAL;
	cipher_name = alg->base.cra_name;

	/* Alas we screwed up the naming so we have to mangle the
	 * cipher name.
	 */
	if (!strncmp(cipher_name, "ecb(", 4)) {
		unsigned len;

		len = strlcpy(ecb_name, cipher_name + 4, sizeof(ecb_name));
		if (len < 2 || len >= sizeof(ecb_name))
<<<<<<< HEAD
			goto err_drop_spawn;

		if (ecb_name[len - 1] != ')')
			goto err_drop_spawn;
=======
			goto err_free_inst;

		if (ecb_name[len - 1] != ')')
			goto err_free_inst;
>>>>>>> upstream/android-13

		ecb_name[len - 1] = 0;

		if (snprintf(inst->alg.base.cra_name, CRYPTO_MAX_ALG_NAME,
			     "lrw(%s)", ecb_name) >= CRYPTO_MAX_ALG_NAME) {
			err = -ENAMETOOLONG;
<<<<<<< HEAD
			goto err_drop_spawn;
		}
	} else
		goto err_drop_spawn;

	inst->alg.base.cra_flags = alg->base.cra_flags & CRYPTO_ALG_ASYNC;
	inst->alg.base.cra_priority = alg->base.cra_priority;
	inst->alg.base.cra_blocksize = LRW_BLOCK_SIZE;
	inst->alg.base.cra_alignmask = alg->base.cra_alignmask |
				       (__alignof__(u64) - 1);
=======
			goto err_free_inst;
		}
	} else
		goto err_free_inst;

	inst->alg.base.cra_priority = alg->base.cra_priority;
	inst->alg.base.cra_blocksize = LRW_BLOCK_SIZE;
	inst->alg.base.cra_alignmask = alg->base.cra_alignmask |
				       (__alignof__(be128) - 1);
>>>>>>> upstream/android-13

	inst->alg.ivsize = LRW_BLOCK_SIZE;
	inst->alg.min_keysize = crypto_skcipher_alg_min_keysize(alg) +
				LRW_BLOCK_SIZE;
	inst->alg.max_keysize = crypto_skcipher_alg_max_keysize(alg) +
				LRW_BLOCK_SIZE;

<<<<<<< HEAD
	inst->alg.base.cra_ctxsize = sizeof(struct priv);

	inst->alg.init = init_tfm;
	inst->alg.exit = exit_tfm;

	inst->alg.setkey = setkey;
	inst->alg.encrypt = encrypt;
	inst->alg.decrypt = decrypt;

	inst->free = free_inst;

	err = skcipher_register_instance(tmpl, inst);
	if (err)
		goto err_drop_spawn;

out:
	return err;

err_drop_spawn:
	crypto_drop_skcipher(spawn);
err_free_inst:
	kfree(inst);
	goto out;
}

static struct crypto_template crypto_tmpl = {
	.name = "lrw",
	.create = create,
	.module = THIS_MODULE,
};

static int __init crypto_module_init(void)
{
	return crypto_register_template(&crypto_tmpl);
}

static void __exit crypto_module_exit(void)
{
	crypto_unregister_template(&crypto_tmpl);
}

module_init(crypto_module_init);
module_exit(crypto_module_exit);
=======
	inst->alg.base.cra_ctxsize = sizeof(struct lrw_tfm_ctx);

	inst->alg.init = lrw_init_tfm;
	inst->alg.exit = lrw_exit_tfm;

	inst->alg.setkey = lrw_setkey;
	inst->alg.encrypt = lrw_encrypt;
	inst->alg.decrypt = lrw_decrypt;

	inst->free = lrw_free_instance;

	err = skcipher_register_instance(tmpl, inst);
	if (err) {
err_free_inst:
		lrw_free_instance(inst);
	}
	return err;
}

static struct crypto_template lrw_tmpl = {
	.name = "lrw",
	.create = lrw_create,
	.module = THIS_MODULE,
};

static int __init lrw_module_init(void)
{
	return crypto_register_template(&lrw_tmpl);
}

static void __exit lrw_module_exit(void)
{
	crypto_unregister_template(&lrw_tmpl);
}

subsys_initcall(lrw_module_init);
module_exit(lrw_module_exit);
>>>>>>> upstream/android-13

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("LRW block cipher mode");
MODULE_ALIAS_CRYPTO("lrw");
