<<<<<<< HEAD

/*
 * Copyright (C) 2016 Cavium, Inc.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of version 2 of the GNU General Public License
 * as published by the Free Software Foundation.
=======
// SPDX-License-Identifier: GPL-2.0-only

/*
 * Copyright (C) 2016 Cavium, Inc.
>>>>>>> upstream/android-13
 */

#include <crypto/aes.h>
#include <crypto/algapi.h>
#include <crypto/authenc.h>
<<<<<<< HEAD
#include <crypto/cryptd.h>
#include <crypto/crypto_wq.h>
#include <crypto/des.h>
=======
#include <crypto/internal/des.h>
>>>>>>> upstream/android-13
#include <crypto/xts.h>
#include <linux/crypto.h>
#include <linux/err.h>
#include <linux/list.h>
#include <linux/scatterlist.h>

#include "cptvf.h"
#include "cptvf_algs.h"

struct cpt_device_handle {
	void *cdev[MAX_DEVICES];
	u32 dev_count;
};

static struct cpt_device_handle dev_handle;

static void cvm_callback(u32 status, void *arg)
{
	struct crypto_async_request *req = (struct crypto_async_request *)arg;

	req->complete(req, !status);
}

static inline void update_input_iv(struct cpt_request_info *req_info,
				   u8 *iv, u32 enc_iv_len,
				   u32 *argcnt)
{
	/* Setting the iv information */
	req_info->in[*argcnt].vptr = (void *)iv;
	req_info->in[*argcnt].size = enc_iv_len;
	req_info->req.dlen += enc_iv_len;

	++(*argcnt);
}

static inline void update_output_iv(struct cpt_request_info *req_info,
				    u8 *iv, u32 enc_iv_len,
				    u32 *argcnt)
{
	/* Setting the iv information */
	req_info->out[*argcnt].vptr = (void *)iv;
	req_info->out[*argcnt].size = enc_iv_len;
	req_info->rlen += enc_iv_len;

	++(*argcnt);
}

static inline void update_input_data(struct cpt_request_info *req_info,
				     struct scatterlist *inp_sg,
				     u32 nbytes, u32 *argcnt)
{
	req_info->req.dlen += nbytes;

	while (nbytes) {
		u32 len = min(nbytes, inp_sg->length);
		u8 *ptr = sg_virt(inp_sg);

		req_info->in[*argcnt].vptr = (void *)ptr;
		req_info->in[*argcnt].size = len;
		nbytes -= len;

		++(*argcnt);
		++inp_sg;
	}
}

static inline void update_output_data(struct cpt_request_info *req_info,
				      struct scatterlist *outp_sg,
				      u32 nbytes, u32 *argcnt)
{
	req_info->rlen += nbytes;

	while (nbytes) {
		u32 len = min(nbytes, outp_sg->length);
		u8 *ptr = sg_virt(outp_sg);

		req_info->out[*argcnt].vptr = (void *)ptr;
		req_info->out[*argcnt].size = len;
		nbytes -= len;
		++(*argcnt);
		++outp_sg;
	}
}

<<<<<<< HEAD
static inline u32 create_ctx_hdr(struct ablkcipher_request *req, u32 enc,
				 u32 *argcnt)
{
	struct crypto_ablkcipher *tfm = crypto_ablkcipher_reqtfm(req);
	struct cvm_enc_ctx *ctx = crypto_ablkcipher_ctx(tfm);
	struct cvm_req_ctx *rctx = ablkcipher_request_ctx(req);
	struct fc_context *fctx = &rctx->fctx;
	u64 *offset_control = &rctx->control_word;
	u32 enc_iv_len = crypto_ablkcipher_ivsize(tfm);
	struct cpt_request_info *req_info = &rctx->cpt_req;
	u64 *ctrl_flags = NULL;
=======
static inline u32 create_ctx_hdr(struct skcipher_request *req, u32 enc,
				 u32 *argcnt)
{
	struct crypto_skcipher *tfm = crypto_skcipher_reqtfm(req);
	struct cvm_enc_ctx *ctx = crypto_skcipher_ctx(tfm);
	struct cvm_req_ctx *rctx = skcipher_request_ctx(req);
	struct fc_context *fctx = &rctx->fctx;
	u32 enc_iv_len = crypto_skcipher_ivsize(tfm);
	struct cpt_request_info *req_info = &rctx->cpt_req;
	__be64 *ctrl_flags = NULL;
	__be64 *offset_control;
>>>>>>> upstream/android-13

	req_info->ctrl.s.grp = 0;
	req_info->ctrl.s.dma_mode = DMA_GATHER_SCATTER;
	req_info->ctrl.s.se_req = SE_CORE_REQ;

	req_info->req.opcode.s.major = MAJOR_OP_FC |
					DMA_MODE_FLAG(DMA_GATHER_SCATTER);
	if (enc)
		req_info->req.opcode.s.minor = 2;
	else
		req_info->req.opcode.s.minor = 3;

<<<<<<< HEAD
	req_info->req.param1 = req->nbytes; /* Encryption Data length */
=======
	req_info->req.param1 = req->cryptlen; /* Encryption Data length */
>>>>>>> upstream/android-13
	req_info->req.param2 = 0; /*Auth data length */

	fctx->enc.enc_ctrl.e.enc_cipher = ctx->cipher_type;
	fctx->enc.enc_ctrl.e.aes_key = ctx->key_type;
	fctx->enc.enc_ctrl.e.iv_source = FROM_DPTR;

	if (ctx->cipher_type == AES_XTS)
		memcpy(fctx->enc.encr_key, ctx->enc_key, ctx->key_len * 2);
	else
		memcpy(fctx->enc.encr_key, ctx->enc_key, ctx->key_len);
<<<<<<< HEAD
	ctrl_flags = (u64 *)&fctx->enc.enc_ctrl.flags;
	*ctrl_flags = cpu_to_be64(*ctrl_flags);

=======
	ctrl_flags = (__be64 *)&fctx->enc.enc_ctrl.flags;
	*ctrl_flags = cpu_to_be64(fctx->enc.enc_ctrl.flags);

	offset_control = (__be64 *)&rctx->control_word;
>>>>>>> upstream/android-13
	*offset_control = cpu_to_be64(((u64)(enc_iv_len) << 16));
	/* Storing  Packet Data Information in offset
	 * Control Word First 8 bytes
	 */
	req_info->in[*argcnt].vptr = (u8 *)offset_control;
	req_info->in[*argcnt].size = CONTROL_WORD_LEN;
	req_info->req.dlen += CONTROL_WORD_LEN;
	++(*argcnt);

	req_info->in[*argcnt].vptr = (u8 *)fctx;
	req_info->in[*argcnt].size = sizeof(struct fc_context);
	req_info->req.dlen += sizeof(struct fc_context);

	++(*argcnt);

	return 0;
}

<<<<<<< HEAD
static inline u32 create_input_list(struct ablkcipher_request  *req, u32 enc,
				    u32 enc_iv_len)
{
	struct cvm_req_ctx *rctx = ablkcipher_request_ctx(req);
=======
static inline u32 create_input_list(struct skcipher_request  *req, u32 enc,
				    u32 enc_iv_len)
{
	struct cvm_req_ctx *rctx = skcipher_request_ctx(req);
>>>>>>> upstream/android-13
	struct cpt_request_info *req_info = &rctx->cpt_req;
	u32 argcnt =  0;

	create_ctx_hdr(req, enc, &argcnt);
<<<<<<< HEAD
	update_input_iv(req_info, req->info, enc_iv_len, &argcnt);
	update_input_data(req_info, req->src, req->nbytes, &argcnt);
=======
	update_input_iv(req_info, req->iv, enc_iv_len, &argcnt);
	update_input_data(req_info, req->src, req->cryptlen, &argcnt);
>>>>>>> upstream/android-13
	req_info->incnt = argcnt;

	return 0;
}

<<<<<<< HEAD
static inline void store_cb_info(struct ablkcipher_request *req,
=======
static inline void store_cb_info(struct skcipher_request *req,
>>>>>>> upstream/android-13
				 struct cpt_request_info *req_info)
{
	req_info->callback = (void *)cvm_callback;
	req_info->callback_arg = (void *)&req->base;
}

<<<<<<< HEAD
static inline void create_output_list(struct ablkcipher_request *req,
				      u32 enc_iv_len)
{
	struct cvm_req_ctx *rctx = ablkcipher_request_ctx(req);
=======
static inline void create_output_list(struct skcipher_request *req,
				      u32 enc_iv_len)
{
	struct cvm_req_ctx *rctx = skcipher_request_ctx(req);
>>>>>>> upstream/android-13
	struct cpt_request_info *req_info = &rctx->cpt_req;
	u32 argcnt = 0;

	/* OUTPUT Buffer Processing
	 * AES encryption/decryption output would be
	 * received in the following format
	 *
	 * ------IV--------|------ENCRYPTED/DECRYPTED DATA-----|
	 * [ 16 Bytes/     [   Request Enc/Dec/ DATA Len AES CBC ]
	 */
	/* Reading IV information */
<<<<<<< HEAD
	update_output_iv(req_info, req->info, enc_iv_len, &argcnt);
	update_output_data(req_info, req->dst, req->nbytes, &argcnt);
	req_info->outcnt = argcnt;
}

static inline int cvm_enc_dec(struct ablkcipher_request *req, u32 enc)
{
	struct crypto_ablkcipher *tfm = crypto_ablkcipher_reqtfm(req);
	struct cvm_req_ctx *rctx = ablkcipher_request_ctx(req);
	u32 enc_iv_len = crypto_ablkcipher_ivsize(tfm);
=======
	update_output_iv(req_info, req->iv, enc_iv_len, &argcnt);
	update_output_data(req_info, req->dst, req->cryptlen, &argcnt);
	req_info->outcnt = argcnt;
}

static inline int cvm_enc_dec(struct skcipher_request *req, u32 enc)
{
	struct crypto_skcipher *tfm = crypto_skcipher_reqtfm(req);
	struct cvm_req_ctx *rctx = skcipher_request_ctx(req);
	u32 enc_iv_len = crypto_skcipher_ivsize(tfm);
>>>>>>> upstream/android-13
	struct fc_context *fctx = &rctx->fctx;
	struct cpt_request_info *req_info = &rctx->cpt_req;
	void *cdev = NULL;
	int status;

	memset(req_info, 0, sizeof(struct cpt_request_info));
	req_info->may_sleep = (req->base.flags & CRYPTO_TFM_REQ_MAY_SLEEP) != 0;
	memset(fctx, 0, sizeof(struct fc_context));
	create_input_list(req, enc, enc_iv_len);
	create_output_list(req, enc_iv_len);
	store_cb_info(req, req_info);
	cdev = dev_handle.cdev[smp_processor_id()];
	status = cptvf_do_request(cdev, req_info);
	/* We perform an asynchronous send and once
	 * the request is completed the driver would
	 * intimate through  registered call back functions
	 */

	if (status)
		return status;
	else
		return -EINPROGRESS;
}

<<<<<<< HEAD
static int cvm_encrypt(struct ablkcipher_request *req)
=======
static int cvm_encrypt(struct skcipher_request *req)
>>>>>>> upstream/android-13
{
	return cvm_enc_dec(req, true);
}

<<<<<<< HEAD
static int cvm_decrypt(struct ablkcipher_request *req)
=======
static int cvm_decrypt(struct skcipher_request *req)
>>>>>>> upstream/android-13
{
	return cvm_enc_dec(req, false);
}

<<<<<<< HEAD
static int cvm_xts_setkey(struct crypto_ablkcipher *cipher, const u8 *key,
		   u32 keylen)
{
	struct crypto_tfm *tfm = crypto_ablkcipher_tfm(cipher);
=======
static int cvm_xts_setkey(struct crypto_skcipher *cipher, const u8 *key,
		   u32 keylen)
{
	struct crypto_tfm *tfm = crypto_skcipher_tfm(cipher);
>>>>>>> upstream/android-13
	struct cvm_enc_ctx *ctx = crypto_tfm_ctx(tfm);
	int err;
	const u8 *key1 = key;
	const u8 *key2 = key + (keylen / 2);

	err = xts_check_key(tfm, key, keylen);
	if (err)
		return err;
	ctx->key_len = keylen;
	memcpy(ctx->enc_key, key1, keylen / 2);
	memcpy(ctx->enc_key + KEY2_OFFSET, key2, keylen / 2);
	ctx->cipher_type = AES_XTS;
	switch (ctx->key_len) {
	case 32:
		ctx->key_type = AES_128_BIT;
		break;
	case 64:
		ctx->key_type = AES_256_BIT;
		break;
	default:
		return -EINVAL;
	}

	return 0;
}

static int cvm_validate_keylen(struct cvm_enc_ctx *ctx, u32 keylen)
{
	if ((keylen == 16) || (keylen == 24) || (keylen == 32)) {
		ctx->key_len = keylen;
		switch (ctx->key_len) {
		case 16:
			ctx->key_type = AES_128_BIT;
			break;
		case 24:
			ctx->key_type = AES_192_BIT;
			break;
		case 32:
			ctx->key_type = AES_256_BIT;
			break;
		default:
			return -EINVAL;
		}

		if (ctx->cipher_type == DES3_CBC)
			ctx->key_type = 0;

		return 0;
	}

	return -EINVAL;
}

<<<<<<< HEAD
static int cvm_setkey(struct crypto_ablkcipher *cipher, const u8 *key,
		      u32 keylen, u8 cipher_type)
{
	struct crypto_tfm *tfm = crypto_ablkcipher_tfm(cipher);
=======
static int cvm_setkey(struct crypto_skcipher *cipher, const u8 *key,
		      u32 keylen, u8 cipher_type)
{
	struct crypto_tfm *tfm = crypto_skcipher_tfm(cipher);
>>>>>>> upstream/android-13
	struct cvm_enc_ctx *ctx = crypto_tfm_ctx(tfm);

	ctx->cipher_type = cipher_type;
	if (!cvm_validate_keylen(ctx, keylen)) {
		memcpy(ctx->enc_key, key, keylen);
		return 0;
	} else {
<<<<<<< HEAD
		crypto_ablkcipher_set_flags(cipher,
					    CRYPTO_TFM_RES_BAD_KEY_LEN);
=======
>>>>>>> upstream/android-13
		return -EINVAL;
	}
}

<<<<<<< HEAD
static int cvm_cbc_aes_setkey(struct crypto_ablkcipher *cipher, const u8 *key,
=======
static int cvm_cbc_aes_setkey(struct crypto_skcipher *cipher, const u8 *key,
>>>>>>> upstream/android-13
			      u32 keylen)
{
	return cvm_setkey(cipher, key, keylen, AES_CBC);
}

<<<<<<< HEAD
static int cvm_ecb_aes_setkey(struct crypto_ablkcipher *cipher, const u8 *key,
=======
static int cvm_ecb_aes_setkey(struct crypto_skcipher *cipher, const u8 *key,
>>>>>>> upstream/android-13
			      u32 keylen)
{
	return cvm_setkey(cipher, key, keylen, AES_ECB);
}

<<<<<<< HEAD
static int cvm_cfb_aes_setkey(struct crypto_ablkcipher *cipher, const u8 *key,
=======
static int cvm_cfb_aes_setkey(struct crypto_skcipher *cipher, const u8 *key,
>>>>>>> upstream/android-13
			      u32 keylen)
{
	return cvm_setkey(cipher, key, keylen, AES_CFB);
}

<<<<<<< HEAD
static int cvm_cbc_des3_setkey(struct crypto_ablkcipher *cipher, const u8 *key,
			       u32 keylen)
{
	return cvm_setkey(cipher, key, keylen, DES3_CBC);
}

static int cvm_ecb_des3_setkey(struct crypto_ablkcipher *cipher, const u8 *key,
			       u32 keylen)
{
	return cvm_setkey(cipher, key, keylen, DES3_ECB);
}

static int cvm_enc_dec_init(struct crypto_tfm *tfm)
{
	struct cvm_enc_ctx *ctx = crypto_tfm_ctx(tfm);

	memset(ctx, 0, sizeof(*ctx));
	tfm->crt_ablkcipher.reqsize = sizeof(struct cvm_req_ctx) +
					sizeof(struct ablkcipher_request);
	/* Additional memory for ablkcipher_request is
	 * allocated since the cryptd daemon uses
	 * this memory for request_ctx information
	 */
=======
static int cvm_cbc_des3_setkey(struct crypto_skcipher *cipher, const u8 *key,
			       u32 keylen)
{
	return verify_skcipher_des3_key(cipher, key) ?:
	       cvm_setkey(cipher, key, keylen, DES3_CBC);
}

static int cvm_ecb_des3_setkey(struct crypto_skcipher *cipher, const u8 *key,
			       u32 keylen)
{
	return verify_skcipher_des3_key(cipher, key) ?:
	       cvm_setkey(cipher, key, keylen, DES3_ECB);
}

static int cvm_enc_dec_init(struct crypto_skcipher *tfm)
{
	crypto_skcipher_set_reqsize(tfm, sizeof(struct cvm_req_ctx));
>>>>>>> upstream/android-13

	return 0;
}

<<<<<<< HEAD
static struct crypto_alg algs[] = { {
	.cra_flags = CRYPTO_ALG_TYPE_ABLKCIPHER | CRYPTO_ALG_ASYNC,
	.cra_blocksize = AES_BLOCK_SIZE,
	.cra_ctxsize = sizeof(struct cvm_enc_ctx),
	.cra_alignmask = 7,
	.cra_priority = 4001,
	.cra_name = "xts(aes)",
	.cra_driver_name = "cavium-xts-aes",
	.cra_type = &crypto_ablkcipher_type,
	.cra_u = {
		.ablkcipher = {
			.ivsize = AES_BLOCK_SIZE,
			.min_keysize = 2 * AES_MIN_KEY_SIZE,
			.max_keysize = 2 * AES_MAX_KEY_SIZE,
			.setkey = cvm_xts_setkey,
			.encrypt = cvm_encrypt,
			.decrypt = cvm_decrypt,
		},
	},
	.cra_init = cvm_enc_dec_init,
	.cra_module = THIS_MODULE,
}, {
	.cra_flags = CRYPTO_ALG_TYPE_ABLKCIPHER | CRYPTO_ALG_ASYNC,
	.cra_blocksize = AES_BLOCK_SIZE,
	.cra_ctxsize = sizeof(struct cvm_enc_ctx),
	.cra_alignmask = 7,
	.cra_priority = 4001,
	.cra_name = "cbc(aes)",
	.cra_driver_name = "cavium-cbc-aes",
	.cra_type = &crypto_ablkcipher_type,
	.cra_u = {
		.ablkcipher = {
			.ivsize = AES_BLOCK_SIZE,
			.min_keysize = AES_MIN_KEY_SIZE,
			.max_keysize = AES_MAX_KEY_SIZE,
			.setkey = cvm_cbc_aes_setkey,
			.encrypt = cvm_encrypt,
			.decrypt = cvm_decrypt,
		},
	},
	.cra_init = cvm_enc_dec_init,
	.cra_module = THIS_MODULE,
}, {
	.cra_flags = CRYPTO_ALG_TYPE_ABLKCIPHER | CRYPTO_ALG_ASYNC,
	.cra_blocksize = AES_BLOCK_SIZE,
	.cra_ctxsize = sizeof(struct cvm_enc_ctx),
	.cra_alignmask = 7,
	.cra_priority = 4001,
	.cra_name = "ecb(aes)",
	.cra_driver_name = "cavium-ecb-aes",
	.cra_type = &crypto_ablkcipher_type,
	.cra_u = {
		.ablkcipher = {
			.ivsize = AES_BLOCK_SIZE,
			.min_keysize = AES_MIN_KEY_SIZE,
			.max_keysize = AES_MAX_KEY_SIZE,
			.setkey = cvm_ecb_aes_setkey,
			.encrypt = cvm_encrypt,
			.decrypt = cvm_decrypt,
		},
	},
	.cra_init = cvm_enc_dec_init,
	.cra_module = THIS_MODULE,
}, {
	.cra_flags = CRYPTO_ALG_TYPE_ABLKCIPHER | CRYPTO_ALG_ASYNC,
	.cra_blocksize = AES_BLOCK_SIZE,
	.cra_ctxsize = sizeof(struct cvm_enc_ctx),
	.cra_alignmask = 7,
	.cra_priority = 4001,
	.cra_name = "cfb(aes)",
	.cra_driver_name = "cavium-cfb-aes",
	.cra_type = &crypto_ablkcipher_type,
	.cra_u = {
		.ablkcipher = {
			.ivsize = AES_BLOCK_SIZE,
			.min_keysize = AES_MIN_KEY_SIZE,
			.max_keysize = AES_MAX_KEY_SIZE,
			.setkey = cvm_cfb_aes_setkey,
			.encrypt = cvm_encrypt,
			.decrypt = cvm_decrypt,
		},
	},
	.cra_init = cvm_enc_dec_init,
	.cra_module = THIS_MODULE,
}, {
	.cra_flags = CRYPTO_ALG_TYPE_ABLKCIPHER | CRYPTO_ALG_ASYNC,
	.cra_blocksize = DES3_EDE_BLOCK_SIZE,
	.cra_ctxsize = sizeof(struct cvm_des3_ctx),
	.cra_alignmask = 7,
	.cra_priority = 4001,
	.cra_name = "cbc(des3_ede)",
	.cra_driver_name = "cavium-cbc-des3_ede",
	.cra_type = &crypto_ablkcipher_type,
	.cra_u = {
		.ablkcipher = {
			.min_keysize = DES3_EDE_KEY_SIZE,
			.max_keysize = DES3_EDE_KEY_SIZE,
			.ivsize = DES_BLOCK_SIZE,
			.setkey = cvm_cbc_des3_setkey,
			.encrypt = cvm_encrypt,
			.decrypt = cvm_decrypt,
		},
	},
	.cra_init = cvm_enc_dec_init,
	.cra_module = THIS_MODULE,
}, {
	.cra_flags = CRYPTO_ALG_TYPE_ABLKCIPHER | CRYPTO_ALG_ASYNC,
	.cra_blocksize = DES3_EDE_BLOCK_SIZE,
	.cra_ctxsize = sizeof(struct cvm_des3_ctx),
	.cra_alignmask = 7,
	.cra_priority = 4001,
	.cra_name = "ecb(des3_ede)",
	.cra_driver_name = "cavium-ecb-des3_ede",
	.cra_type = &crypto_ablkcipher_type,
	.cra_u = {
		.ablkcipher = {
			.min_keysize = DES3_EDE_KEY_SIZE,
			.max_keysize = DES3_EDE_KEY_SIZE,
			.ivsize = DES_BLOCK_SIZE,
			.setkey = cvm_ecb_des3_setkey,
			.encrypt = cvm_encrypt,
			.decrypt = cvm_decrypt,
		},
	},
	.cra_init = cvm_enc_dec_init,
	.cra_module = THIS_MODULE,
=======
static struct skcipher_alg algs[] = { {
	.base.cra_flags		= CRYPTO_ALG_ASYNC |
				  CRYPTO_ALG_ALLOCATES_MEMORY,
	.base.cra_blocksize	= AES_BLOCK_SIZE,
	.base.cra_ctxsize	= sizeof(struct cvm_enc_ctx),
	.base.cra_alignmask	= 7,
	.base.cra_priority	= 4001,
	.base.cra_name		= "xts(aes)",
	.base.cra_driver_name	= "cavium-xts-aes",
	.base.cra_module	= THIS_MODULE,

	.ivsize			= AES_BLOCK_SIZE,
	.min_keysize		= 2 * AES_MIN_KEY_SIZE,
	.max_keysize		= 2 * AES_MAX_KEY_SIZE,
	.setkey			= cvm_xts_setkey,
	.encrypt		= cvm_encrypt,
	.decrypt		= cvm_decrypt,
	.init			= cvm_enc_dec_init,
}, {
	.base.cra_flags		= CRYPTO_ALG_ASYNC |
				  CRYPTO_ALG_ALLOCATES_MEMORY,
	.base.cra_blocksize	= AES_BLOCK_SIZE,
	.base.cra_ctxsize	= sizeof(struct cvm_enc_ctx),
	.base.cra_alignmask	= 7,
	.base.cra_priority	= 4001,
	.base.cra_name		= "cbc(aes)",
	.base.cra_driver_name	= "cavium-cbc-aes",
	.base.cra_module	= THIS_MODULE,

	.ivsize			= AES_BLOCK_SIZE,
	.min_keysize		= AES_MIN_KEY_SIZE,
	.max_keysize		= AES_MAX_KEY_SIZE,
	.setkey			= cvm_cbc_aes_setkey,
	.encrypt		= cvm_encrypt,
	.decrypt		= cvm_decrypt,
	.init			= cvm_enc_dec_init,
}, {
	.base.cra_flags		= CRYPTO_ALG_ASYNC |
				  CRYPTO_ALG_ALLOCATES_MEMORY,
	.base.cra_blocksize	= AES_BLOCK_SIZE,
	.base.cra_ctxsize	= sizeof(struct cvm_enc_ctx),
	.base.cra_alignmask	= 7,
	.base.cra_priority	= 4001,
	.base.cra_name		= "ecb(aes)",
	.base.cra_driver_name	= "cavium-ecb-aes",
	.base.cra_module	= THIS_MODULE,

	.min_keysize		= AES_MIN_KEY_SIZE,
	.max_keysize		= AES_MAX_KEY_SIZE,
	.setkey			= cvm_ecb_aes_setkey,
	.encrypt		= cvm_encrypt,
	.decrypt		= cvm_decrypt,
	.init			= cvm_enc_dec_init,
}, {
	.base.cra_flags		= CRYPTO_ALG_ASYNC |
				  CRYPTO_ALG_ALLOCATES_MEMORY,
	.base.cra_blocksize	= AES_BLOCK_SIZE,
	.base.cra_ctxsize	= sizeof(struct cvm_enc_ctx),
	.base.cra_alignmask	= 7,
	.base.cra_priority	= 4001,
	.base.cra_name		= "cfb(aes)",
	.base.cra_driver_name	= "cavium-cfb-aes",
	.base.cra_module	= THIS_MODULE,

	.ivsize			= AES_BLOCK_SIZE,
	.min_keysize		= AES_MIN_KEY_SIZE,
	.max_keysize		= AES_MAX_KEY_SIZE,
	.setkey			= cvm_cfb_aes_setkey,
	.encrypt		= cvm_encrypt,
	.decrypt		= cvm_decrypt,
	.init			= cvm_enc_dec_init,
}, {
	.base.cra_flags		= CRYPTO_ALG_ASYNC |
				  CRYPTO_ALG_ALLOCATES_MEMORY,
	.base.cra_blocksize	= DES3_EDE_BLOCK_SIZE,
	.base.cra_ctxsize	= sizeof(struct cvm_des3_ctx),
	.base.cra_alignmask	= 7,
	.base.cra_priority	= 4001,
	.base.cra_name		= "cbc(des3_ede)",
	.base.cra_driver_name	= "cavium-cbc-des3_ede",
	.base.cra_module	= THIS_MODULE,

	.min_keysize		= DES3_EDE_KEY_SIZE,
	.max_keysize		= DES3_EDE_KEY_SIZE,
	.ivsize			= DES_BLOCK_SIZE,
	.setkey			= cvm_cbc_des3_setkey,
	.encrypt		= cvm_encrypt,
	.decrypt		= cvm_decrypt,
	.init			= cvm_enc_dec_init,
}, {
	.base.cra_flags		= CRYPTO_ALG_ASYNC |
				  CRYPTO_ALG_ALLOCATES_MEMORY,
	.base.cra_blocksize	= DES3_EDE_BLOCK_SIZE,
	.base.cra_ctxsize	= sizeof(struct cvm_des3_ctx),
	.base.cra_alignmask	= 7,
	.base.cra_priority	= 4001,
	.base.cra_name		= "ecb(des3_ede)",
	.base.cra_driver_name	= "cavium-ecb-des3_ede",
	.base.cra_module	= THIS_MODULE,

	.min_keysize		= DES3_EDE_KEY_SIZE,
	.max_keysize		= DES3_EDE_KEY_SIZE,
	.ivsize			= DES_BLOCK_SIZE,
	.setkey			= cvm_ecb_des3_setkey,
	.encrypt		= cvm_encrypt,
	.decrypt		= cvm_decrypt,
	.init			= cvm_enc_dec_init,
>>>>>>> upstream/android-13
} };

static inline int cav_register_algs(void)
{
<<<<<<< HEAD
	int err = 0;

	err = crypto_register_algs(algs, ARRAY_SIZE(algs));
	if (err)
		return err;

	return 0;
=======
	return crypto_register_skciphers(algs, ARRAY_SIZE(algs));
>>>>>>> upstream/android-13
}

static inline void cav_unregister_algs(void)
{
<<<<<<< HEAD
	crypto_unregister_algs(algs, ARRAY_SIZE(algs));
=======
	crypto_unregister_skciphers(algs, ARRAY_SIZE(algs));
>>>>>>> upstream/android-13
}

int cvm_crypto_init(struct cpt_vf *cptvf)
{
	struct pci_dev *pdev = cptvf->pdev;
	u32 dev_count;

	dev_count = dev_handle.dev_count;
	dev_handle.cdev[dev_count] = cptvf;
	dev_handle.dev_count++;

	if (dev_count == 3) {
		if (cav_register_algs()) {
			dev_err(&pdev->dev, "Error in registering crypto algorithms\n");
			return -EINVAL;
		}
	}

	return 0;
}

void cvm_crypto_exit(void)
{
	u32 dev_count;

	dev_count = --dev_handle.dev_count;
	if (!dev_count)
		cav_unregister_algs();
}
