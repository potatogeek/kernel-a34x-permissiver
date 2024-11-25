<<<<<<< HEAD
/* XTS: as defined in IEEE1619/D16
 *	http://grouper.ieee.org/groups/1619/email/pdf00086.pdf
 *	(sector sizes which are not a multiple of 16 bytes are,
 *	however currently unsupported)
=======
// SPDX-License-Identifier: GPL-2.0-or-later
/* XTS: as defined in IEEE1619/D16
 *	http://grouper.ieee.org/groups/1619/email/pdf00086.pdf
>>>>>>> upstream/android-13
 *
 * Copyright (c) 2007 Rik Snel <rsnel@cube.dyndns.org>
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
=======
 */
#include <crypto/internal/cipher.h>
>>>>>>> upstream/android-13
#include <crypto/internal/skcipher.h>
#include <crypto/scatterwalk.h>
#include <linux/err.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/scatterlist.h>
#include <linux/slab.h>

#include <crypto/xts.h>
#include <crypto/b128ops.h>
#include <crypto/gf128mul.h>

<<<<<<< HEAD
#define XTS_BUFFER_SIZE 128u

struct priv {
=======
struct xts_tfm_ctx {
>>>>>>> upstream/android-13
	struct crypto_skcipher *child;
	struct crypto_cipher *tweak;
};

struct xts_instance_ctx {
	struct crypto_skcipher_spawn spawn;
	char name[CRYPTO_MAX_ALG_NAME];
};

<<<<<<< HEAD
struct rctx {
	le128 buf[XTS_BUFFER_SIZE / sizeof(le128)];

	le128 t;

	le128 *ext;

	struct scatterlist srcbuf[2];
	struct scatterlist dstbuf[2];
	struct scatterlist *src;
	struct scatterlist *dst;

	unsigned int left;

	struct skcipher_request subreq;
};

static int setkey(struct crypto_skcipher *parent, const u8 *key,
		  unsigned int keylen)
{
	struct priv *ctx = crypto_skcipher_ctx(parent);
=======
struct xts_request_ctx {
	le128 t;
	struct scatterlist *tail;
	struct scatterlist sg[2];
	struct skcipher_request subreq;
};

static int xts_setkey(struct crypto_skcipher *parent, const u8 *key,
		      unsigned int keylen)
{
	struct xts_tfm_ctx *ctx = crypto_skcipher_ctx(parent);
>>>>>>> upstream/android-13
	struct crypto_skcipher *child;
	struct crypto_cipher *tweak;
	int err;

	err = xts_verify_key(parent, key, keylen);
	if (err)
		return err;

	keylen /= 2;

	/* we need two cipher instances: one to compute the initial 'tweak'
	 * by encrypting the IV (usually the 'plain' iv) and the other
	 * one to encrypt and decrypt the data */

	/* tweak cipher, uses Key2 i.e. the second half of *key */
	tweak = ctx->tweak;
	crypto_cipher_clear_flags(tweak, CRYPTO_TFM_REQ_MASK);
	crypto_cipher_set_flags(tweak, crypto_skcipher_get_flags(parent) &
				       CRYPTO_TFM_REQ_MASK);
	err = crypto_cipher_setkey(tweak, key + keylen, keylen);
<<<<<<< HEAD
	crypto_skcipher_set_flags(parent, crypto_cipher_get_flags(tweak) &
					  CRYPTO_TFM_RES_MASK);
=======
>>>>>>> upstream/android-13
	if (err)
		return err;

	/* data cipher, uses Key1 i.e. the first half of *key */
	child = ctx->child;
	crypto_skcipher_clear_flags(child, CRYPTO_TFM_REQ_MASK);
	crypto_skcipher_set_flags(child, crypto_skcipher_get_flags(parent) &
					 CRYPTO_TFM_REQ_MASK);
<<<<<<< HEAD
	err = crypto_skcipher_setkey(child, key, keylen);
	crypto_skcipher_set_flags(parent, crypto_skcipher_get_flags(child) &
					  CRYPTO_TFM_RES_MASK);

	return err;
}

static int post_crypt(struct skcipher_request *req)
{
	struct rctx *rctx = skcipher_request_ctx(req);
	le128 *buf = rctx->ext ?: rctx->buf;
	struct skcipher_request *subreq;
	const int bs = XTS_BLOCK_SIZE;
	struct skcipher_walk w;
	struct scatterlist *sg;
	unsigned offset;
	int err;

	subreq = &rctx->subreq;
	err = skcipher_walk_virt(&w, subreq, false);

	while (w.nbytes) {
		unsigned int avail = w.nbytes;
		le128 *wdst;

		wdst = w.dst.virt.addr;

		do {
			le128_xor(wdst, buf++, wdst);
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
	struct rctx *rctx = skcipher_request_ctx(req);
	le128 *buf = rctx->ext ?: rctx->buf;
	struct skcipher_request *subreq;
	const int bs = XTS_BLOCK_SIZE;
	struct skcipher_walk w;
	struct scatterlist *sg;
	unsigned cryptlen;
	unsigned offset;
	bool more;
	int err;

	subreq = &rctx->subreq;
	cryptlen = subreq->cryptlen;

	more = rctx->left > cryptlen;
	if (!more)
		cryptlen = rctx->left;

	skcipher_request_set_crypt(subreq, rctx->src, rctx->dst,
				   cryptlen, NULL);

	err = skcipher_walk_virt(&w, subreq, false);
=======
	return crypto_skcipher_setkey(child, key, keylen);
}

/*
 * We compute the tweak masks twice (both before and after the ECB encryption or
 * decryption) to avoid having to allocate a temporary buffer and/or make
 * mutliple calls to the 'ecb(..)' instance, which usually would be slower than
 * just doing the gf128mul_x_ble() calls again.
 */
static int xts_xor_tweak(struct skcipher_request *req, bool second_pass,
			 bool enc)
{
	struct xts_request_ctx *rctx = skcipher_request_ctx(req);
	struct crypto_skcipher *tfm = crypto_skcipher_reqtfm(req);
	const bool cts = (req->cryptlen % XTS_BLOCK_SIZE);
	const int bs = XTS_BLOCK_SIZE;
	struct skcipher_walk w;
	le128 t = rctx->t;
	int err;

	if (second_pass) {
		req = &rctx->subreq;
		/* set to our TFM to enforce correct alignment: */
		skcipher_request_set_tfm(req, tfm);
	}
	err = skcipher_walk_virt(&w, req, false);
>>>>>>> upstream/android-13

	while (w.nbytes) {
		unsigned int avail = w.nbytes;
		le128 *wsrc;
		le128 *wdst;

		wsrc = w.src.virt.addr;
		wdst = w.dst.virt.addr;

		do {
<<<<<<< HEAD
			*buf++ = rctx->t;
			le128_xor(wdst++, &rctx->t, wsrc++);
			gf128mul_x_ble(&rctx->t, &rctx->t);
=======
			if (unlikely(cts) &&
			    w.total - w.nbytes + avail < 2 * XTS_BLOCK_SIZE) {
				if (!enc) {
					if (second_pass)
						rctx->t = t;
					gf128mul_x_ble(&t, &t);
				}
				le128_xor(wdst, &t, wsrc);
				if (enc && second_pass)
					gf128mul_x_ble(&rctx->t, &t);
				skcipher_walk_done(&w, avail - bs);
				return 0;
			}

			le128_xor(wdst++, &t, wsrc++);
			gf128mul_x_ble(&t, &t);
>>>>>>> upstream/android-13
		} while ((avail -= bs) >= bs);

		err = skcipher_walk_done(&w, avail);
	}

<<<<<<< HEAD
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
	skcipher_request_set_tfm(subreq, ctx->child);
	skcipher_request_set_callback(subreq, req->base.flags, done, req);

	gfp = req->base.flags & CRYPTO_TFM_REQ_MAY_SLEEP ? GFP_KERNEL :
							   GFP_ATOMIC;
	rctx->ext = NULL;

	subreq->cryptlen = XTS_BUFFER_SIZE;
	if (req->cryptlen > XTS_BUFFER_SIZE) {
		unsigned int n = min(req->cryptlen, (unsigned int)PAGE_SIZE);

		rctx->ext = kmalloc(n, gfp);
		if (rctx->ext)
			subreq->cryptlen = n;
	}

	rctx->src = req->src;
	rctx->dst = req->dst;
	rctx->left = req->cryptlen;
=======
	return err;
}

static int xts_xor_tweak_pre(struct skcipher_request *req, bool enc)
{
	return xts_xor_tweak(req, false, enc);
}

static int xts_xor_tweak_post(struct skcipher_request *req, bool enc)
{
	return xts_xor_tweak(req, true, enc);
}

static void xts_cts_done(struct crypto_async_request *areq, int err)
{
	struct skcipher_request *req = areq->data;
	le128 b;

	if (!err) {
		struct xts_request_ctx *rctx = skcipher_request_ctx(req);

		scatterwalk_map_and_copy(&b, rctx->tail, 0, XTS_BLOCK_SIZE, 0);
		le128_xor(&b, &rctx->t, &b);
		scatterwalk_map_and_copy(&b, rctx->tail, 0, XTS_BLOCK_SIZE, 1);
	}

	skcipher_request_complete(req, err);
}

static int xts_cts_final(struct skcipher_request *req,
			 int (*crypt)(struct skcipher_request *req))
{
	const struct xts_tfm_ctx *ctx =
		crypto_skcipher_ctx(crypto_skcipher_reqtfm(req));
	int offset = req->cryptlen & ~(XTS_BLOCK_SIZE - 1);
	struct xts_request_ctx *rctx = skcipher_request_ctx(req);
	struct skcipher_request *subreq = &rctx->subreq;
	int tail = req->cryptlen % XTS_BLOCK_SIZE;
	le128 b[2];
	int err;

	rctx->tail = scatterwalk_ffwd(rctx->sg, req->dst,
				      offset - XTS_BLOCK_SIZE);

	scatterwalk_map_and_copy(b, rctx->tail, 0, XTS_BLOCK_SIZE, 0);
	b[1] = b[0];
	scatterwalk_map_and_copy(b, req->src, offset, tail, 0);

	le128_xor(b, &rctx->t, b);

	scatterwalk_map_and_copy(b, rctx->tail, 0, XTS_BLOCK_SIZE + tail, 1);

	skcipher_request_set_tfm(subreq, ctx->child);
	skcipher_request_set_callback(subreq, req->base.flags, xts_cts_done,
				      req);
	skcipher_request_set_crypt(subreq, rctx->tail, rctx->tail,
				   XTS_BLOCK_SIZE, NULL);

	err = crypt(subreq);
	if (err)
		return err;

	scatterwalk_map_and_copy(b, rctx->tail, 0, XTS_BLOCK_SIZE, 0);
	le128_xor(b, &rctx->t, b);
	scatterwalk_map_and_copy(b, rctx->tail, 0, XTS_BLOCK_SIZE, 1);

	return 0;
}

static void xts_encrypt_done(struct crypto_async_request *areq, int err)
{
	struct skcipher_request *req = areq->data;

	if (!err) {
		struct xts_request_ctx *rctx = skcipher_request_ctx(req);

		rctx->subreq.base.flags &= ~CRYPTO_TFM_REQ_MAY_SLEEP;
		err = xts_xor_tweak_post(req, true);

		if (!err && unlikely(req->cryptlen % XTS_BLOCK_SIZE)) {
			err = xts_cts_final(req, crypto_skcipher_encrypt);
			if (err == -EINPROGRESS)
				return;
		}
	}

	skcipher_request_complete(req, err);
}

static void xts_decrypt_done(struct crypto_async_request *areq, int err)
{
	struct skcipher_request *req = areq->data;

	if (!err) {
		struct xts_request_ctx *rctx = skcipher_request_ctx(req);

		rctx->subreq.base.flags &= ~CRYPTO_TFM_REQ_MAY_SLEEP;
		err = xts_xor_tweak_post(req, false);

		if (!err && unlikely(req->cryptlen % XTS_BLOCK_SIZE)) {
			err = xts_cts_final(req, crypto_skcipher_decrypt);
			if (err == -EINPROGRESS)
				return;
		}
	}

	skcipher_request_complete(req, err);
}

static int xts_init_crypt(struct skcipher_request *req,
			  crypto_completion_t compl)
{
	const struct xts_tfm_ctx *ctx =
		crypto_skcipher_ctx(crypto_skcipher_reqtfm(req));
	struct xts_request_ctx *rctx = skcipher_request_ctx(req);
	struct skcipher_request *subreq = &rctx->subreq;

	if (req->cryptlen < XTS_BLOCK_SIZE)
		return -EINVAL;

	skcipher_request_set_tfm(subreq, ctx->child);
	skcipher_request_set_callback(subreq, req->base.flags, compl, req);
	skcipher_request_set_crypt(subreq, req->dst, req->dst,
				   req->cryptlen & ~(XTS_BLOCK_SIZE - 1), NULL);
>>>>>>> upstream/android-13

	/* calculate first value of T */
	crypto_cipher_encrypt_one(ctx->tweak, (u8 *)&rctx->t, req->iv);

	return 0;
}

<<<<<<< HEAD
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
	struct xts_instance_ctx *ictx = skcipher_instance_ctx(inst);
	struct priv *ctx = crypto_skcipher_ctx(tfm);
=======
static int xts_encrypt(struct skcipher_request *req)
{
	struct xts_request_ctx *rctx = skcipher_request_ctx(req);
	struct skcipher_request *subreq = &rctx->subreq;
	int err;

	err = xts_init_crypt(req, xts_encrypt_done) ?:
	      xts_xor_tweak_pre(req, true) ?:
	      crypto_skcipher_encrypt(subreq) ?:
	      xts_xor_tweak_post(req, true);

	if (err || likely((req->cryptlen % XTS_BLOCK_SIZE) == 0))
		return err;

	return xts_cts_final(req, crypto_skcipher_encrypt);
}

static int xts_decrypt(struct skcipher_request *req)
{
	struct xts_request_ctx *rctx = skcipher_request_ctx(req);
	struct skcipher_request *subreq = &rctx->subreq;
	int err;

	err = xts_init_crypt(req, xts_decrypt_done) ?:
	      xts_xor_tweak_pre(req, false) ?:
	      crypto_skcipher_decrypt(subreq) ?:
	      xts_xor_tweak_post(req, false);

	if (err || likely((req->cryptlen % XTS_BLOCK_SIZE) == 0))
		return err;

	return xts_cts_final(req, crypto_skcipher_decrypt);
}

static int xts_init_tfm(struct crypto_skcipher *tfm)
{
	struct skcipher_instance *inst = skcipher_alg_instance(tfm);
	struct xts_instance_ctx *ictx = skcipher_instance_ctx(inst);
	struct xts_tfm_ctx *ctx = crypto_skcipher_ctx(tfm);
>>>>>>> upstream/android-13
	struct crypto_skcipher *child;
	struct crypto_cipher *tweak;

	child = crypto_spawn_skcipher(&ictx->spawn);
	if (IS_ERR(child))
		return PTR_ERR(child);

	ctx->child = child;

	tweak = crypto_alloc_cipher(ictx->name, 0, 0);
	if (IS_ERR(tweak)) {
		crypto_free_skcipher(ctx->child);
		return PTR_ERR(tweak);
	}

	ctx->tweak = tweak;

	crypto_skcipher_set_reqsize(tfm, crypto_skcipher_reqsize(child) +
<<<<<<< HEAD
					 sizeof(struct rctx));
=======
					 sizeof(struct xts_request_ctx));
>>>>>>> upstream/android-13

	return 0;
}

<<<<<<< HEAD
static void exit_tfm(struct crypto_skcipher *tfm)
{
	struct priv *ctx = crypto_skcipher_ctx(tfm);
=======
static void xts_exit_tfm(struct crypto_skcipher *tfm)
{
	struct xts_tfm_ctx *ctx = crypto_skcipher_ctx(tfm);
>>>>>>> upstream/android-13

	crypto_free_skcipher(ctx->child);
	crypto_free_cipher(ctx->tweak);
}

<<<<<<< HEAD
static void free_inst(struct skcipher_instance *inst)
{
	crypto_drop_skcipher(skcipher_instance_ctx(inst));
	kfree(inst);
}

static int create(struct crypto_template *tmpl, struct rtattr **tb)
{
	struct skcipher_instance *inst;
	struct crypto_attr_type *algt;
=======
static void xts_free_instance(struct skcipher_instance *inst)
{
	struct xts_instance_ctx *ictx = skcipher_instance_ctx(inst);

	crypto_drop_skcipher(&ictx->spawn);
	kfree(inst);
}

static int xts_create(struct crypto_template *tmpl, struct rtattr **tb)
{
	struct skcipher_instance *inst;
>>>>>>> upstream/android-13
	struct xts_instance_ctx *ctx;
	struct skcipher_alg *alg;
	const char *cipher_name;
	u32 mask;
	int err;

<<<<<<< HEAD
	algt = crypto_get_attr_type(tb);
	if (IS_ERR(algt))
		return PTR_ERR(algt);

	if ((algt->type ^ CRYPTO_ALG_TYPE_SKCIPHER) & algt->mask)
		return -EINVAL;
=======
	err = crypto_check_attr_type(tb, CRYPTO_ALG_TYPE_SKCIPHER, &mask);
	if (err)
		return err;
>>>>>>> upstream/android-13

	cipher_name = crypto_attr_alg_name(tb[1]);
	if (IS_ERR(cipher_name))
		return PTR_ERR(cipher_name);

	inst = kzalloc(sizeof(*inst) + sizeof(*ctx), GFP_KERNEL);
	if (!inst)
		return -ENOMEM;

	ctx = skcipher_instance_ctx(inst);

<<<<<<< HEAD
	crypto_set_skcipher_spawn(&ctx->spawn, skcipher_crypto_instance(inst));

	mask = crypto_requires_off(algt->type, algt->mask,
				   CRYPTO_ALG_NEED_FALLBACK |
				   CRYPTO_ALG_ASYNC);

	err = crypto_grab_skcipher(&ctx->spawn, cipher_name, 0, mask);
=======
	err = crypto_grab_skcipher(&ctx->spawn, skcipher_crypto_instance(inst),
				   cipher_name, 0, mask);
>>>>>>> upstream/android-13
	if (err == -ENOENT) {
		err = -ENAMETOOLONG;
		if (snprintf(ctx->name, CRYPTO_MAX_ALG_NAME, "ecb(%s)",
			     cipher_name) >= CRYPTO_MAX_ALG_NAME)
			goto err_free_inst;

<<<<<<< HEAD
		err = crypto_grab_skcipher(&ctx->spawn, ctx->name, 0, mask);
=======
		err = crypto_grab_skcipher(&ctx->spawn,
					   skcipher_crypto_instance(inst),
					   ctx->name, 0, mask);
>>>>>>> upstream/android-13
	}

	if (err)
		goto err_free_inst;

	alg = crypto_skcipher_spawn_alg(&ctx->spawn);

	err = -EINVAL;
	if (alg->base.cra_blocksize != XTS_BLOCK_SIZE)
<<<<<<< HEAD
		goto err_drop_spawn;

	if (crypto_skcipher_alg_ivsize(alg))
		goto err_drop_spawn;
=======
		goto err_free_inst;

	if (crypto_skcipher_alg_ivsize(alg))
		goto err_free_inst;
>>>>>>> upstream/android-13

	err = crypto_inst_setname(skcipher_crypto_instance(inst), "xts",
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

		len = strlcpy(ctx->name, cipher_name + 4, sizeof(ctx->name));
		if (len < 2 || len >= sizeof(ctx->name))
<<<<<<< HEAD
			goto err_drop_spawn;

		if (ctx->name[len - 1] != ')')
			goto err_drop_spawn;
=======
			goto err_free_inst;

		if (ctx->name[len - 1] != ')')
			goto err_free_inst;
>>>>>>> upstream/android-13

		ctx->name[len - 1] = 0;

		if (snprintf(inst->alg.base.cra_name, CRYPTO_MAX_ALG_NAME,
			     "xts(%s)", ctx->name) >= CRYPTO_MAX_ALG_NAME) {
			err = -ENAMETOOLONG;
<<<<<<< HEAD
			goto err_drop_spawn;
		}
	} else
		goto err_drop_spawn;

	inst->alg.base.cra_flags = alg->base.cra_flags & CRYPTO_ALG_ASYNC;
=======
			goto err_free_inst;
		}
	} else
		goto err_free_inst;

>>>>>>> upstream/android-13
	inst->alg.base.cra_priority = alg->base.cra_priority;
	inst->alg.base.cra_blocksize = XTS_BLOCK_SIZE;
	inst->alg.base.cra_alignmask = alg->base.cra_alignmask |
				       (__alignof__(u64) - 1);

	inst->alg.ivsize = XTS_BLOCK_SIZE;
	inst->alg.min_keysize = crypto_skcipher_alg_min_keysize(alg) * 2;
	inst->alg.max_keysize = crypto_skcipher_alg_max_keysize(alg) * 2;

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
	crypto_drop_skcipher(&ctx->spawn);
err_free_inst:
	kfree(inst);
	goto out;
}

static struct crypto_template crypto_tmpl = {
	.name = "xts",
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
	inst->alg.base.cra_ctxsize = sizeof(struct xts_tfm_ctx);

	inst->alg.init = xts_init_tfm;
	inst->alg.exit = xts_exit_tfm;

	inst->alg.setkey = xts_setkey;
	inst->alg.encrypt = xts_encrypt;
	inst->alg.decrypt = xts_decrypt;

	inst->free = xts_free_instance;

	err = skcipher_register_instance(tmpl, inst);
	if (err) {
err_free_inst:
		xts_free_instance(inst);
	}
	return err;
}

static struct crypto_template xts_tmpl = {
	.name = "xts",
	.create = xts_create,
	.module = THIS_MODULE,
};

static int __init xts_module_init(void)
{
	return crypto_register_template(&xts_tmpl);
}

static void __exit xts_module_exit(void)
{
	crypto_unregister_template(&xts_tmpl);
}

subsys_initcall(xts_module_init);
module_exit(xts_module_exit);
>>>>>>> upstream/android-13

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("XTS block cipher mode");
MODULE_ALIAS_CRYPTO("xts");
<<<<<<< HEAD
=======
MODULE_IMPORT_NS(CRYPTO_INTERNAL);
MODULE_SOFTDEP("pre: ecb");
>>>>>>> upstream/android-13
