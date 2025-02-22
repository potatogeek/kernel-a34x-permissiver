<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0+
>>>>>>> upstream/android-13
/*
 * caam - Freescale FSL CAAM support for crypto API
 *
 * Copyright 2008-2011 Freescale Semiconductor, Inc.
<<<<<<< HEAD
 * Copyright 2016 NXP
=======
 * Copyright 2016-2019 NXP
>>>>>>> upstream/android-13
 *
 * Based on talitos crypto API driver.
 *
 * relationship of job descriptors to shared descriptors (SteveC Dec 10 2008):
 *
 * ---------------                     ---------------
 * | JobDesc #1  |-------------------->|  ShareDesc  |
 * | *(packet 1) |                     |   (PDB)     |
 * ---------------      |------------->|  (hashKey)  |
 *       .              |              | (cipherKey) |
 *       .              |    |-------->| (operation) |
 * ---------------      |    |         ---------------
 * | JobDesc #2  |------|    |
 * | *(packet 2) |           |
 * ---------------           |
 *       .                   |
 *       .                   |
 * ---------------           |
 * | JobDesc #3  |------------
 * | *(packet 3) |
 * ---------------
 *
 * The SharedDesc never changes for a connection unless rekeyed, but
 * each packet will likely be in a different place. So all we need
 * to know to process the packet is where the input is, where the
 * output goes, and what context we want to process with. Context is
 * in the SharedDesc, packet references in the JobDesc.
 *
 * So, a job desc looks like:
 *
 * ---------------------
 * | Header            |
 * | ShareDesc Pointer |
 * | SEQ_OUT_PTR       |
 * | (output buffer)   |
 * | (output length)   |
 * | SEQ_IN_PTR        |
 * | (input buffer)    |
 * | (input length)    |
 * ---------------------
 */

#include "compat.h"

#include "regs.h"
#include "intern.h"
#include "desc_constr.h"
#include "jr.h"
#include "error.h"
#include "sg_sw_sec4.h"
#include "key_gen.h"
#include "caamalg_desc.h"
<<<<<<< HEAD
=======
#include <crypto/engine.h>
#include <crypto/xts.h>
#include <asm/unaligned.h>
>>>>>>> upstream/android-13

/*
 * crypto alg
 */
#define CAAM_CRA_PRIORITY		3000
/* max key is sum of AES_MAX_KEY_SIZE, max split key size */
#define CAAM_MAX_KEY_SIZE		(AES_MAX_KEY_SIZE + \
					 CTR_RFC3686_NONCE_SIZE + \
					 SHA512_DIGEST_SIZE * 2)

#define AEAD_DESC_JOB_IO_LEN		(DESC_JOB_IO_LEN + CAAM_CMD_SZ * 2)
#define GCM_DESC_JOB_IO_LEN		(AEAD_DESC_JOB_IO_LEN + \
					 CAAM_CMD_SZ * 4)
#define AUTHENC_DESC_JOB_IO_LEN		(AEAD_DESC_JOB_IO_LEN + \
					 CAAM_CMD_SZ * 5)

<<<<<<< HEAD
#define DESC_MAX_USED_BYTES		(CAAM_DESC_BYTES_MAX - DESC_JOB_IO_LEN)
#define DESC_MAX_USED_LEN		(DESC_MAX_USED_BYTES / CAAM_CMD_SZ)

#ifdef DEBUG
/* for print_hex_dumps with line references */
#define debug(format, arg...) printk(format, arg)
#else
#define debug(format, arg...)
#endif

static struct list_head alg_list;

=======
#define CHACHAPOLY_DESC_JOB_IO_LEN	(AEAD_DESC_JOB_IO_LEN + CAAM_CMD_SZ * 6)

#define DESC_MAX_USED_BYTES		(CAAM_DESC_BYTES_MAX - DESC_JOB_IO_LEN_MIN)
#define DESC_MAX_USED_LEN		(DESC_MAX_USED_BYTES / CAAM_CMD_SZ)

>>>>>>> upstream/android-13
struct caam_alg_entry {
	int class1_alg_type;
	int class2_alg_type;
	bool rfc3686;
	bool geniv;
<<<<<<< HEAD
=======
	bool nodkp;
>>>>>>> upstream/android-13
};

struct caam_aead_alg {
	struct aead_alg aead;
	struct caam_alg_entry caam;
	bool registered;
};

<<<<<<< HEAD
=======
struct caam_skcipher_alg {
	struct skcipher_alg skcipher;
	struct caam_alg_entry caam;
	bool registered;
};

>>>>>>> upstream/android-13
/*
 * per-session context
 */
struct caam_ctx {
<<<<<<< HEAD
	u32 sh_desc_enc[DESC_MAX_USED_LEN];
	u32 sh_desc_dec[DESC_MAX_USED_LEN];
	u32 sh_desc_givenc[DESC_MAX_USED_LEN];
	u8 key[CAAM_MAX_KEY_SIZE];
	dma_addr_t sh_desc_enc_dma;
	dma_addr_t sh_desc_dec_dma;
	dma_addr_t sh_desc_givenc_dma;
=======
	struct crypto_engine_ctx enginectx;
	u32 sh_desc_enc[DESC_MAX_USED_LEN];
	u32 sh_desc_dec[DESC_MAX_USED_LEN];
	u8 key[CAAM_MAX_KEY_SIZE];
	dma_addr_t sh_desc_enc_dma;
	dma_addr_t sh_desc_dec_dma;
>>>>>>> upstream/android-13
	dma_addr_t key_dma;
	enum dma_data_direction dir;
	struct device *jrdev;
	struct alginfo adata;
	struct alginfo cdata;
	unsigned int authsize;
<<<<<<< HEAD
=======
	bool xts_key_fallback;
	struct crypto_skcipher *fallback;
};

struct caam_skcipher_req_ctx {
	struct skcipher_edesc *edesc;
	struct skcipher_request fallback_req;
};

struct caam_aead_req_ctx {
	struct aead_edesc *edesc;
>>>>>>> upstream/android-13
};

static int aead_null_set_sh_desc(struct crypto_aead *aead)
{
	struct caam_ctx *ctx = crypto_aead_ctx(aead);
	struct device *jrdev = ctx->jrdev;
	struct caam_drv_private *ctrlpriv = dev_get_drvdata(jrdev->parent);
	u32 *desc;
	int rem_bytes = CAAM_DESC_BYTES_MAX - AEAD_DESC_JOB_IO_LEN -
			ctx->adata.keylen_pad;

	/*
	 * Job Descriptor and Shared Descriptors
	 * must all fit into the 64-word Descriptor h/w Buffer
	 */
	if (rem_bytes >= DESC_AEAD_NULL_ENC_LEN) {
		ctx->adata.key_inline = true;
		ctx->adata.key_virt = ctx->key;
	} else {
		ctx->adata.key_inline = false;
		ctx->adata.key_dma = ctx->key_dma;
	}

	/* aead_encrypt shared descriptor */
	desc = ctx->sh_desc_enc;
	cnstr_shdsc_aead_null_encap(desc, &ctx->adata, ctx->authsize,
				    ctrlpriv->era);
	dma_sync_single_for_device(jrdev, ctx->sh_desc_enc_dma,
				   desc_bytes(desc), ctx->dir);

	/*
	 * Job Descriptor and Shared Descriptors
	 * must all fit into the 64-word Descriptor h/w Buffer
	 */
	if (rem_bytes >= DESC_AEAD_NULL_DEC_LEN) {
		ctx->adata.key_inline = true;
		ctx->adata.key_virt = ctx->key;
	} else {
		ctx->adata.key_inline = false;
		ctx->adata.key_dma = ctx->key_dma;
	}

	/* aead_decrypt shared descriptor */
	desc = ctx->sh_desc_dec;
	cnstr_shdsc_aead_null_decap(desc, &ctx->adata, ctx->authsize,
				    ctrlpriv->era);
	dma_sync_single_for_device(jrdev, ctx->sh_desc_dec_dma,
				   desc_bytes(desc), ctx->dir);

	return 0;
}

static int aead_set_sh_desc(struct crypto_aead *aead)
{
	struct caam_aead_alg *alg = container_of(crypto_aead_alg(aead),
						 struct caam_aead_alg, aead);
	unsigned int ivsize = crypto_aead_ivsize(aead);
	struct caam_ctx *ctx = crypto_aead_ctx(aead);
	struct device *jrdev = ctx->jrdev;
	struct caam_drv_private *ctrlpriv = dev_get_drvdata(jrdev->parent);
	u32 ctx1_iv_off = 0;
	u32 *desc, *nonce = NULL;
	u32 inl_mask;
	unsigned int data_len[2];
	const bool ctr_mode = ((ctx->cdata.algtype & OP_ALG_AAI_MASK) ==
			       OP_ALG_AAI_CTR_MOD128);
	const bool is_rfc3686 = alg->caam.rfc3686;

	if (!ctx->authsize)
		return 0;

	/* NULL encryption / decryption */
	if (!ctx->cdata.keylen)
		return aead_null_set_sh_desc(aead);

	/*
	 * AES-CTR needs to load IV in CONTEXT1 reg
	 * at an offset of 128bits (16bytes)
	 * CONTEXT1[255:128] = IV
	 */
	if (ctr_mode)
		ctx1_iv_off = 16;

	/*
	 * RFC3686 specific:
	 *	CONTEXT1[255:128] = {NONCE, IV, COUNTER}
	 */
	if (is_rfc3686) {
		ctx1_iv_off = 16 + CTR_RFC3686_NONCE_SIZE;
		nonce = (u32 *)((void *)ctx->key + ctx->adata.keylen_pad +
				ctx->cdata.keylen - CTR_RFC3686_NONCE_SIZE);
	}

<<<<<<< HEAD
=======
	/*
	 * In case |user key| > |derived key|, using DKP<imm,imm>
	 * would result in invalid opcodes (last bytes of user key) in
	 * the resulting descriptor. Use DKP<ptr,imm> instead => both
	 * virtual and dma key addresses are needed.
	 */
	ctx->adata.key_virt = ctx->key;
	ctx->adata.key_dma = ctx->key_dma;

	ctx->cdata.key_virt = ctx->key + ctx->adata.keylen_pad;
	ctx->cdata.key_dma = ctx->key_dma + ctx->adata.keylen_pad;

>>>>>>> upstream/android-13
	data_len[0] = ctx->adata.keylen_pad;
	data_len[1] = ctx->cdata.keylen;

	if (alg->caam.geniv)
		goto skip_enc;

	/*
	 * Job Descriptor and Shared Descriptors
	 * must all fit into the 64-word Descriptor h/w Buffer
	 */
	if (desc_inline_query(DESC_AEAD_ENC_LEN +
			      (is_rfc3686 ? DESC_AEAD_CTR_RFC3686_LEN : 0),
			      AUTHENC_DESC_JOB_IO_LEN, data_len, &inl_mask,
			      ARRAY_SIZE(data_len)) < 0)
		return -EINVAL;

<<<<<<< HEAD
	if (inl_mask & 1)
		ctx->adata.key_virt = ctx->key;
	else
		ctx->adata.key_dma = ctx->key_dma;

	if (inl_mask & 2)
		ctx->cdata.key_virt = ctx->key + ctx->adata.keylen_pad;
	else
		ctx->cdata.key_dma = ctx->key_dma + ctx->adata.keylen_pad;

=======
>>>>>>> upstream/android-13
	ctx->adata.key_inline = !!(inl_mask & 1);
	ctx->cdata.key_inline = !!(inl_mask & 2);

	/* aead_encrypt shared descriptor */
	desc = ctx->sh_desc_enc;
	cnstr_shdsc_aead_encap(desc, &ctx->cdata, &ctx->adata, ivsize,
			       ctx->authsize, is_rfc3686, nonce, ctx1_iv_off,
			       false, ctrlpriv->era);
	dma_sync_single_for_device(jrdev, ctx->sh_desc_enc_dma,
				   desc_bytes(desc), ctx->dir);

skip_enc:
	/*
	 * Job Descriptor and Shared Descriptors
	 * must all fit into the 64-word Descriptor h/w Buffer
	 */
	if (desc_inline_query(DESC_AEAD_DEC_LEN +
			      (is_rfc3686 ? DESC_AEAD_CTR_RFC3686_LEN : 0),
			      AUTHENC_DESC_JOB_IO_LEN, data_len, &inl_mask,
			      ARRAY_SIZE(data_len)) < 0)
		return -EINVAL;

<<<<<<< HEAD
	if (inl_mask & 1)
		ctx->adata.key_virt = ctx->key;
	else
		ctx->adata.key_dma = ctx->key_dma;

	if (inl_mask & 2)
		ctx->cdata.key_virt = ctx->key + ctx->adata.keylen_pad;
	else
		ctx->cdata.key_dma = ctx->key_dma + ctx->adata.keylen_pad;

=======
>>>>>>> upstream/android-13
	ctx->adata.key_inline = !!(inl_mask & 1);
	ctx->cdata.key_inline = !!(inl_mask & 2);

	/* aead_decrypt shared descriptor */
	desc = ctx->sh_desc_dec;
	cnstr_shdsc_aead_decap(desc, &ctx->cdata, &ctx->adata, ivsize,
			       ctx->authsize, alg->caam.geniv, is_rfc3686,
			       nonce, ctx1_iv_off, false, ctrlpriv->era);
	dma_sync_single_for_device(jrdev, ctx->sh_desc_dec_dma,
				   desc_bytes(desc), ctx->dir);

	if (!alg->caam.geniv)
		goto skip_givenc;

	/*
	 * Job Descriptor and Shared Descriptors
	 * must all fit into the 64-word Descriptor h/w Buffer
	 */
	if (desc_inline_query(DESC_AEAD_GIVENC_LEN +
			      (is_rfc3686 ? DESC_AEAD_CTR_RFC3686_LEN : 0),
			      AUTHENC_DESC_JOB_IO_LEN, data_len, &inl_mask,
			      ARRAY_SIZE(data_len)) < 0)
		return -EINVAL;

<<<<<<< HEAD
	if (inl_mask & 1)
		ctx->adata.key_virt = ctx->key;
	else
		ctx->adata.key_dma = ctx->key_dma;

	if (inl_mask & 2)
		ctx->cdata.key_virt = ctx->key + ctx->adata.keylen_pad;
	else
		ctx->cdata.key_dma = ctx->key_dma + ctx->adata.keylen_pad;

=======
>>>>>>> upstream/android-13
	ctx->adata.key_inline = !!(inl_mask & 1);
	ctx->cdata.key_inline = !!(inl_mask & 2);

	/* aead_givencrypt shared descriptor */
	desc = ctx->sh_desc_enc;
	cnstr_shdsc_aead_givencap(desc, &ctx->cdata, &ctx->adata, ivsize,
				  ctx->authsize, is_rfc3686, nonce,
				  ctx1_iv_off, false, ctrlpriv->era);
	dma_sync_single_for_device(jrdev, ctx->sh_desc_enc_dma,
				   desc_bytes(desc), ctx->dir);

skip_givenc:
	return 0;
}

static int aead_setauthsize(struct crypto_aead *authenc,
				    unsigned int authsize)
{
	struct caam_ctx *ctx = crypto_aead_ctx(authenc);

	ctx->authsize = authsize;
	aead_set_sh_desc(authenc);

	return 0;
}

static int gcm_set_sh_desc(struct crypto_aead *aead)
{
	struct caam_ctx *ctx = crypto_aead_ctx(aead);
	struct device *jrdev = ctx->jrdev;
	unsigned int ivsize = crypto_aead_ivsize(aead);
	u32 *desc;
	int rem_bytes = CAAM_DESC_BYTES_MAX - GCM_DESC_JOB_IO_LEN -
			ctx->cdata.keylen;

	if (!ctx->cdata.keylen || !ctx->authsize)
		return 0;

	/*
	 * AES GCM encrypt shared descriptor
	 * Job Descriptor and Shared Descriptor
	 * must fit into the 64-word Descriptor h/w Buffer
	 */
	if (rem_bytes >= DESC_GCM_ENC_LEN) {
		ctx->cdata.key_inline = true;
		ctx->cdata.key_virt = ctx->key;
	} else {
		ctx->cdata.key_inline = false;
		ctx->cdata.key_dma = ctx->key_dma;
	}

	desc = ctx->sh_desc_enc;
	cnstr_shdsc_gcm_encap(desc, &ctx->cdata, ivsize, ctx->authsize, false);
	dma_sync_single_for_device(jrdev, ctx->sh_desc_enc_dma,
				   desc_bytes(desc), ctx->dir);

	/*
	 * Job Descriptor and Shared Descriptors
	 * must all fit into the 64-word Descriptor h/w Buffer
	 */
	if (rem_bytes >= DESC_GCM_DEC_LEN) {
		ctx->cdata.key_inline = true;
		ctx->cdata.key_virt = ctx->key;
	} else {
		ctx->cdata.key_inline = false;
		ctx->cdata.key_dma = ctx->key_dma;
	}

	desc = ctx->sh_desc_dec;
	cnstr_shdsc_gcm_decap(desc, &ctx->cdata, ivsize, ctx->authsize, false);
	dma_sync_single_for_device(jrdev, ctx->sh_desc_dec_dma,
				   desc_bytes(desc), ctx->dir);

	return 0;
}

static int gcm_setauthsize(struct crypto_aead *authenc, unsigned int authsize)
{
	struct caam_ctx *ctx = crypto_aead_ctx(authenc);
<<<<<<< HEAD
=======
	int err;

	err = crypto_gcm_check_authsize(authsize);
	if (err)
		return err;
>>>>>>> upstream/android-13

	ctx->authsize = authsize;
	gcm_set_sh_desc(authenc);

	return 0;
}

static int rfc4106_set_sh_desc(struct crypto_aead *aead)
{
	struct caam_ctx *ctx = crypto_aead_ctx(aead);
	struct device *jrdev = ctx->jrdev;
	unsigned int ivsize = crypto_aead_ivsize(aead);
	u32 *desc;
	int rem_bytes = CAAM_DESC_BYTES_MAX - GCM_DESC_JOB_IO_LEN -
			ctx->cdata.keylen;

	if (!ctx->cdata.keylen || !ctx->authsize)
		return 0;

	/*
	 * RFC4106 encrypt shared descriptor
	 * Job Descriptor and Shared Descriptor
	 * must fit into the 64-word Descriptor h/w Buffer
	 */
	if (rem_bytes >= DESC_RFC4106_ENC_LEN) {
		ctx->cdata.key_inline = true;
		ctx->cdata.key_virt = ctx->key;
	} else {
		ctx->cdata.key_inline = false;
		ctx->cdata.key_dma = ctx->key_dma;
	}

	desc = ctx->sh_desc_enc;
	cnstr_shdsc_rfc4106_encap(desc, &ctx->cdata, ivsize, ctx->authsize,
				  false);
	dma_sync_single_for_device(jrdev, ctx->sh_desc_enc_dma,
				   desc_bytes(desc), ctx->dir);

	/*
	 * Job Descriptor and Shared Descriptors
	 * must all fit into the 64-word Descriptor h/w Buffer
	 */
	if (rem_bytes >= DESC_RFC4106_DEC_LEN) {
		ctx->cdata.key_inline = true;
		ctx->cdata.key_virt = ctx->key;
	} else {
		ctx->cdata.key_inline = false;
		ctx->cdata.key_dma = ctx->key_dma;
	}

	desc = ctx->sh_desc_dec;
	cnstr_shdsc_rfc4106_decap(desc, &ctx->cdata, ivsize, ctx->authsize,
				  false);
	dma_sync_single_for_device(jrdev, ctx->sh_desc_dec_dma,
				   desc_bytes(desc), ctx->dir);

	return 0;
}

static int rfc4106_setauthsize(struct crypto_aead *authenc,
			       unsigned int authsize)
{
	struct caam_ctx *ctx = crypto_aead_ctx(authenc);
<<<<<<< HEAD
=======
	int err;

	err = crypto_rfc4106_check_authsize(authsize);
	if (err)
		return err;
>>>>>>> upstream/android-13

	ctx->authsize = authsize;
	rfc4106_set_sh_desc(authenc);

	return 0;
}

static int rfc4543_set_sh_desc(struct crypto_aead *aead)
{
	struct caam_ctx *ctx = crypto_aead_ctx(aead);
	struct device *jrdev = ctx->jrdev;
	unsigned int ivsize = crypto_aead_ivsize(aead);
	u32 *desc;
	int rem_bytes = CAAM_DESC_BYTES_MAX - GCM_DESC_JOB_IO_LEN -
			ctx->cdata.keylen;

	if (!ctx->cdata.keylen || !ctx->authsize)
		return 0;

	/*
	 * RFC4543 encrypt shared descriptor
	 * Job Descriptor and Shared Descriptor
	 * must fit into the 64-word Descriptor h/w Buffer
	 */
	if (rem_bytes >= DESC_RFC4543_ENC_LEN) {
		ctx->cdata.key_inline = true;
		ctx->cdata.key_virt = ctx->key;
	} else {
		ctx->cdata.key_inline = false;
		ctx->cdata.key_dma = ctx->key_dma;
	}

	desc = ctx->sh_desc_enc;
	cnstr_shdsc_rfc4543_encap(desc, &ctx->cdata, ivsize, ctx->authsize,
				  false);
	dma_sync_single_for_device(jrdev, ctx->sh_desc_enc_dma,
				   desc_bytes(desc), ctx->dir);

	/*
	 * Job Descriptor and Shared Descriptors
	 * must all fit into the 64-word Descriptor h/w Buffer
	 */
	if (rem_bytes >= DESC_RFC4543_DEC_LEN) {
		ctx->cdata.key_inline = true;
		ctx->cdata.key_virt = ctx->key;
	} else {
		ctx->cdata.key_inline = false;
		ctx->cdata.key_dma = ctx->key_dma;
	}

	desc = ctx->sh_desc_dec;
	cnstr_shdsc_rfc4543_decap(desc, &ctx->cdata, ivsize, ctx->authsize,
				  false);
	dma_sync_single_for_device(jrdev, ctx->sh_desc_dec_dma,
				   desc_bytes(desc), ctx->dir);

	return 0;
}

static int rfc4543_setauthsize(struct crypto_aead *authenc,
			       unsigned int authsize)
{
	struct caam_ctx *ctx = crypto_aead_ctx(authenc);

<<<<<<< HEAD
=======
	if (authsize != 16)
		return -EINVAL;

>>>>>>> upstream/android-13
	ctx->authsize = authsize;
	rfc4543_set_sh_desc(authenc);

	return 0;
}

<<<<<<< HEAD
=======
static int chachapoly_set_sh_desc(struct crypto_aead *aead)
{
	struct caam_ctx *ctx = crypto_aead_ctx(aead);
	struct device *jrdev = ctx->jrdev;
	unsigned int ivsize = crypto_aead_ivsize(aead);
	u32 *desc;

	if (!ctx->cdata.keylen || !ctx->authsize)
		return 0;

	desc = ctx->sh_desc_enc;
	cnstr_shdsc_chachapoly(desc, &ctx->cdata, &ctx->adata, ivsize,
			       ctx->authsize, true, false);
	dma_sync_single_for_device(jrdev, ctx->sh_desc_enc_dma,
				   desc_bytes(desc), ctx->dir);

	desc = ctx->sh_desc_dec;
	cnstr_shdsc_chachapoly(desc, &ctx->cdata, &ctx->adata, ivsize,
			       ctx->authsize, false, false);
	dma_sync_single_for_device(jrdev, ctx->sh_desc_dec_dma,
				   desc_bytes(desc), ctx->dir);

	return 0;
}

static int chachapoly_setauthsize(struct crypto_aead *aead,
				  unsigned int authsize)
{
	struct caam_ctx *ctx = crypto_aead_ctx(aead);

	if (authsize != POLY1305_DIGEST_SIZE)
		return -EINVAL;

	ctx->authsize = authsize;
	return chachapoly_set_sh_desc(aead);
}

static int chachapoly_setkey(struct crypto_aead *aead, const u8 *key,
			     unsigned int keylen)
{
	struct caam_ctx *ctx = crypto_aead_ctx(aead);
	unsigned int ivsize = crypto_aead_ivsize(aead);
	unsigned int saltlen = CHACHAPOLY_IV_SIZE - ivsize;

	if (keylen != CHACHA_KEY_SIZE + saltlen)
		return -EINVAL;

	ctx->cdata.key_virt = key;
	ctx->cdata.keylen = keylen - saltlen;

	return chachapoly_set_sh_desc(aead);
}

>>>>>>> upstream/android-13
static int aead_setkey(struct crypto_aead *aead,
			       const u8 *key, unsigned int keylen)
{
	struct caam_ctx *ctx = crypto_aead_ctx(aead);
	struct device *jrdev = ctx->jrdev;
	struct caam_drv_private *ctrlpriv = dev_get_drvdata(jrdev->parent);
	struct crypto_authenc_keys keys;
	int ret = 0;

	if (crypto_authenc_extractkeys(&keys, key, keylen) != 0)
		goto badkey;

<<<<<<< HEAD
#ifdef DEBUG
	printk(KERN_ERR "keylen %d enckeylen %d authkeylen %d\n",
	       keys.authkeylen + keys.enckeylen, keys.enckeylen,
	       keys.authkeylen);
	print_hex_dump(KERN_ERR, "key in @"__stringify(__LINE__)": ",
		       DUMP_PREFIX_ADDRESS, 16, 4, key, keylen, 1);
#endif
=======
	dev_dbg(jrdev, "keylen %d enckeylen %d authkeylen %d\n",
	       keys.authkeylen + keys.enckeylen, keys.enckeylen,
	       keys.authkeylen);
	print_hex_dump_debug("key in @"__stringify(__LINE__)": ",
			     DUMP_PREFIX_ADDRESS, 16, 4, key, keylen, 1);
>>>>>>> upstream/android-13

	/*
	 * If DKP is supported, use it in the shared descriptor to generate
	 * the split key.
	 */
	if (ctrlpriv->era >= 6) {
		ctx->adata.keylen = keys.authkeylen;
		ctx->adata.keylen_pad = split_key_len(ctx->adata.algtype &
						      OP_ALG_ALGSEL_MASK);

		if (ctx->adata.keylen_pad + keys.enckeylen > CAAM_MAX_KEY_SIZE)
			goto badkey;

		memcpy(ctx->key, keys.authkey, keys.authkeylen);
		memcpy(ctx->key + ctx->adata.keylen_pad, keys.enckey,
		       keys.enckeylen);
		dma_sync_single_for_device(jrdev, ctx->key_dma,
					   ctx->adata.keylen_pad +
					   keys.enckeylen, ctx->dir);
		goto skip_split_key;
	}

	ret = gen_split_key(ctx->jrdev, ctx->key, &ctx->adata, keys.authkey,
			    keys.authkeylen, CAAM_MAX_KEY_SIZE -
			    keys.enckeylen);
	if (ret) {
		goto badkey;
	}

	/* postpend encryption key to auth split key */
	memcpy(ctx->key + ctx->adata.keylen_pad, keys.enckey, keys.enckeylen);
	dma_sync_single_for_device(jrdev, ctx->key_dma, ctx->adata.keylen_pad +
				   keys.enckeylen, ctx->dir);
<<<<<<< HEAD
#ifdef DEBUG
	print_hex_dump(KERN_ERR, "ctx.key@"__stringify(__LINE__)": ",
		       DUMP_PREFIX_ADDRESS, 16, 4, ctx->key,
		       ctx->adata.keylen_pad + keys.enckeylen, 1);
#endif
=======

	print_hex_dump_debug("ctx.key@"__stringify(__LINE__)": ",
			     DUMP_PREFIX_ADDRESS, 16, 4, ctx->key,
			     ctx->adata.keylen_pad + keys.enckeylen, 1);
>>>>>>> upstream/android-13

skip_split_key:
	ctx->cdata.keylen = keys.enckeylen;
	memzero_explicit(&keys, sizeof(keys));
	return aead_set_sh_desc(aead);
badkey:
<<<<<<< HEAD
	crypto_aead_set_flags(aead, CRYPTO_TFM_RES_BAD_KEY_LEN);
=======
>>>>>>> upstream/android-13
	memzero_explicit(&keys, sizeof(keys));
	return -EINVAL;
}

<<<<<<< HEAD
=======
static int des3_aead_setkey(struct crypto_aead *aead, const u8 *key,
			    unsigned int keylen)
{
	struct crypto_authenc_keys keys;
	int err;

	err = crypto_authenc_extractkeys(&keys, key, keylen);
	if (unlikely(err))
		return err;

	err = verify_aead_des3_key(aead, keys.enckey, keys.enckeylen) ?:
	      aead_setkey(aead, key, keylen);

	memzero_explicit(&keys, sizeof(keys));
	return err;
}

>>>>>>> upstream/android-13
static int gcm_setkey(struct crypto_aead *aead,
		      const u8 *key, unsigned int keylen)
{
	struct caam_ctx *ctx = crypto_aead_ctx(aead);
	struct device *jrdev = ctx->jrdev;
<<<<<<< HEAD

#ifdef DEBUG
	print_hex_dump(KERN_ERR, "key in @"__stringify(__LINE__)": ",
		       DUMP_PREFIX_ADDRESS, 16, 4, key, keylen, 1);
#endif
=======
	int err;

	err = aes_check_keylen(keylen);
	if (err)
		return err;

	print_hex_dump_debug("key in @"__stringify(__LINE__)": ",
			     DUMP_PREFIX_ADDRESS, 16, 4, key, keylen, 1);
>>>>>>> upstream/android-13

	memcpy(ctx->key, key, keylen);
	dma_sync_single_for_device(jrdev, ctx->key_dma, keylen, ctx->dir);
	ctx->cdata.keylen = keylen;

	return gcm_set_sh_desc(aead);
}

static int rfc4106_setkey(struct crypto_aead *aead,
			  const u8 *key, unsigned int keylen)
{
	struct caam_ctx *ctx = crypto_aead_ctx(aead);
	struct device *jrdev = ctx->jrdev;
<<<<<<< HEAD

	if (keylen < 4)
		return -EINVAL;

#ifdef DEBUG
	print_hex_dump(KERN_ERR, "key in @"__stringify(__LINE__)": ",
		       DUMP_PREFIX_ADDRESS, 16, 4, key, keylen, 1);
#endif
=======
	int err;

	err = aes_check_keylen(keylen - 4);
	if (err)
		return err;

	print_hex_dump_debug("key in @"__stringify(__LINE__)": ",
			     DUMP_PREFIX_ADDRESS, 16, 4, key, keylen, 1);
>>>>>>> upstream/android-13

	memcpy(ctx->key, key, keylen);

	/*
	 * The last four bytes of the key material are used as the salt value
	 * in the nonce. Update the AES key length.
	 */
	ctx->cdata.keylen = keylen - 4;
	dma_sync_single_for_device(jrdev, ctx->key_dma, ctx->cdata.keylen,
				   ctx->dir);
	return rfc4106_set_sh_desc(aead);
}

static int rfc4543_setkey(struct crypto_aead *aead,
			  const u8 *key, unsigned int keylen)
{
	struct caam_ctx *ctx = crypto_aead_ctx(aead);
	struct device *jrdev = ctx->jrdev;
<<<<<<< HEAD

	if (keylen < 4)
		return -EINVAL;

#ifdef DEBUG
	print_hex_dump(KERN_ERR, "key in @"__stringify(__LINE__)": ",
		       DUMP_PREFIX_ADDRESS, 16, 4, key, keylen, 1);
#endif
=======
	int err;

	err = aes_check_keylen(keylen - 4);
	if (err)
		return err;

	print_hex_dump_debug("key in @"__stringify(__LINE__)": ",
			     DUMP_PREFIX_ADDRESS, 16, 4, key, keylen, 1);
>>>>>>> upstream/android-13

	memcpy(ctx->key, key, keylen);

	/*
	 * The last four bytes of the key material are used as the salt value
	 * in the nonce. Update the AES key length.
	 */
	ctx->cdata.keylen = keylen - 4;
	dma_sync_single_for_device(jrdev, ctx->key_dma, ctx->cdata.keylen,
				   ctx->dir);
	return rfc4543_set_sh_desc(aead);
}

<<<<<<< HEAD
static int ablkcipher_setkey(struct crypto_ablkcipher *ablkcipher,
			     const u8 *key, unsigned int keylen)
{
	struct caam_ctx *ctx = crypto_ablkcipher_ctx(ablkcipher);
	struct crypto_tfm *tfm = crypto_ablkcipher_tfm(ablkcipher);
	const char *alg_name = crypto_tfm_alg_name(tfm);
	struct device *jrdev = ctx->jrdev;
	unsigned int ivsize = crypto_ablkcipher_ivsize(ablkcipher);
	u32 *desc;
	u32 ctx1_iv_off = 0;
	const bool ctr_mode = ((ctx->cdata.algtype & OP_ALG_AAI_MASK) ==
			       OP_ALG_AAI_CTR_MOD128);
	const bool is_rfc3686 = (ctr_mode &&
				 (strstr(alg_name, "rfc3686") != NULL));

#ifdef DEBUG
	print_hex_dump(KERN_ERR, "key in @"__stringify(__LINE__)": ",
		       DUMP_PREFIX_ADDRESS, 16, 4, key, keylen, 1);
#endif
	/*
	 * AES-CTR needs to load IV in CONTEXT1 reg
	 * at an offset of 128bits (16bytes)
	 * CONTEXT1[255:128] = IV
	 */
	if (ctr_mode)
		ctx1_iv_off = 16;
=======
static int skcipher_setkey(struct crypto_skcipher *skcipher, const u8 *key,
			   unsigned int keylen, const u32 ctx1_iv_off)
{
	struct caam_ctx *ctx = crypto_skcipher_ctx(skcipher);
	struct caam_skcipher_alg *alg =
		container_of(crypto_skcipher_alg(skcipher), typeof(*alg),
			     skcipher);
	struct device *jrdev = ctx->jrdev;
	unsigned int ivsize = crypto_skcipher_ivsize(skcipher);
	u32 *desc;
	const bool is_rfc3686 = alg->caam.rfc3686;

	print_hex_dump_debug("key in @"__stringify(__LINE__)": ",
			     DUMP_PREFIX_ADDRESS, 16, 4, key, keylen, 1);

	ctx->cdata.keylen = keylen;
	ctx->cdata.key_virt = key;
	ctx->cdata.key_inline = true;

	/* skcipher_encrypt shared descriptor */
	desc = ctx->sh_desc_enc;
	cnstr_shdsc_skcipher_encap(desc, &ctx->cdata, ivsize, is_rfc3686,
				   ctx1_iv_off);
	dma_sync_single_for_device(jrdev, ctx->sh_desc_enc_dma,
				   desc_bytes(desc), ctx->dir);

	/* skcipher_decrypt shared descriptor */
	desc = ctx->sh_desc_dec;
	cnstr_shdsc_skcipher_decap(desc, &ctx->cdata, ivsize, is_rfc3686,
				   ctx1_iv_off);
	dma_sync_single_for_device(jrdev, ctx->sh_desc_dec_dma,
				   desc_bytes(desc), ctx->dir);

	return 0;
}

static int aes_skcipher_setkey(struct crypto_skcipher *skcipher,
			       const u8 *key, unsigned int keylen)
{
	int err;

	err = aes_check_keylen(keylen);
	if (err)
		return err;

	return skcipher_setkey(skcipher, key, keylen, 0);
}

static int rfc3686_skcipher_setkey(struct crypto_skcipher *skcipher,
				   const u8 *key, unsigned int keylen)
{
	u32 ctx1_iv_off;
	int err;
>>>>>>> upstream/android-13

	/*
	 * RFC3686 specific:
	 *	| CONTEXT1[255:128] = {NONCE, IV, COUNTER}
	 *	| *key = {KEY, NONCE}
	 */
<<<<<<< HEAD
	if (is_rfc3686) {
		ctx1_iv_off = 16 + CTR_RFC3686_NONCE_SIZE;
		keylen -= CTR_RFC3686_NONCE_SIZE;
	}

	ctx->cdata.keylen = keylen;
	ctx->cdata.key_virt = key;
	ctx->cdata.key_inline = true;

	/* ablkcipher_encrypt shared descriptor */
	desc = ctx->sh_desc_enc;
	cnstr_shdsc_ablkcipher_encap(desc, &ctx->cdata, ivsize, is_rfc3686,
				     ctx1_iv_off);
	dma_sync_single_for_device(jrdev, ctx->sh_desc_enc_dma,
				   desc_bytes(desc), ctx->dir);

	/* ablkcipher_decrypt shared descriptor */
	desc = ctx->sh_desc_dec;
	cnstr_shdsc_ablkcipher_decap(desc, &ctx->cdata, ivsize, is_rfc3686,
				     ctx1_iv_off);
	dma_sync_single_for_device(jrdev, ctx->sh_desc_dec_dma,
				   desc_bytes(desc), ctx->dir);

	/* ablkcipher_givencrypt shared descriptor */
	desc = ctx->sh_desc_givenc;
	cnstr_shdsc_ablkcipher_givencap(desc, &ctx->cdata, ivsize, is_rfc3686,
					ctx1_iv_off);
	dma_sync_single_for_device(jrdev, ctx->sh_desc_givenc_dma,
				   desc_bytes(desc), ctx->dir);

	return 0;
}

static int xts_ablkcipher_setkey(struct crypto_ablkcipher *ablkcipher,
				 const u8 *key, unsigned int keylen)
{
	struct caam_ctx *ctx = crypto_ablkcipher_ctx(ablkcipher);
	struct device *jrdev = ctx->jrdev;
	u32 *desc;

	if (keylen != 2 * AES_MIN_KEY_SIZE  && keylen != 2 * AES_MAX_KEY_SIZE) {
		crypto_ablkcipher_set_flags(ablkcipher,
					    CRYPTO_TFM_RES_BAD_KEY_LEN);
		dev_err(jrdev, "key size mismatch\n");
		return -EINVAL;
=======
	ctx1_iv_off = 16 + CTR_RFC3686_NONCE_SIZE;
	keylen -= CTR_RFC3686_NONCE_SIZE;

	err = aes_check_keylen(keylen);
	if (err)
		return err;

	return skcipher_setkey(skcipher, key, keylen, ctx1_iv_off);
}

static int ctr_skcipher_setkey(struct crypto_skcipher *skcipher,
			       const u8 *key, unsigned int keylen)
{
	u32 ctx1_iv_off;
	int err;

	/*
	 * AES-CTR needs to load IV in CONTEXT1 reg
	 * at an offset of 128bits (16bytes)
	 * CONTEXT1[255:128] = IV
	 */
	ctx1_iv_off = 16;

	err = aes_check_keylen(keylen);
	if (err)
		return err;

	return skcipher_setkey(skcipher, key, keylen, ctx1_iv_off);
}

static int des_skcipher_setkey(struct crypto_skcipher *skcipher,
			       const u8 *key, unsigned int keylen)
{
	return verify_skcipher_des_key(skcipher, key) ?:
	       skcipher_setkey(skcipher, key, keylen, 0);
}

static int des3_skcipher_setkey(struct crypto_skcipher *skcipher,
				const u8 *key, unsigned int keylen)
{
	return verify_skcipher_des3_key(skcipher, key) ?:
	       skcipher_setkey(skcipher, key, keylen, 0);
}

static int xts_skcipher_setkey(struct crypto_skcipher *skcipher, const u8 *key,
			       unsigned int keylen)
{
	struct caam_ctx *ctx = crypto_skcipher_ctx(skcipher);
	struct device *jrdev = ctx->jrdev;
	struct caam_drv_private *ctrlpriv = dev_get_drvdata(jrdev->parent);
	u32 *desc;
	int err;

	err = xts_verify_key(skcipher, key, keylen);
	if (err) {
		dev_dbg(jrdev, "key size mismatch\n");
		return err;
	}

	if (keylen != 2 * AES_KEYSIZE_128 && keylen != 2 * AES_KEYSIZE_256)
		ctx->xts_key_fallback = true;

	if (ctrlpriv->era <= 8 || ctx->xts_key_fallback) {
		err = crypto_skcipher_setkey(ctx->fallback, key, keylen);
		if (err)
			return err;
>>>>>>> upstream/android-13
	}

	ctx->cdata.keylen = keylen;
	ctx->cdata.key_virt = key;
	ctx->cdata.key_inline = true;

<<<<<<< HEAD
	/* xts_ablkcipher_encrypt shared descriptor */
	desc = ctx->sh_desc_enc;
	cnstr_shdsc_xts_ablkcipher_encap(desc, &ctx->cdata);
	dma_sync_single_for_device(jrdev, ctx->sh_desc_enc_dma,
				   desc_bytes(desc), ctx->dir);

	/* xts_ablkcipher_decrypt shared descriptor */
	desc = ctx->sh_desc_dec;
	cnstr_shdsc_xts_ablkcipher_decap(desc, &ctx->cdata);
=======
	/* xts_skcipher_encrypt shared descriptor */
	desc = ctx->sh_desc_enc;
	cnstr_shdsc_xts_skcipher_encap(desc, &ctx->cdata);
	dma_sync_single_for_device(jrdev, ctx->sh_desc_enc_dma,
				   desc_bytes(desc), ctx->dir);

	/* xts_skcipher_decrypt shared descriptor */
	desc = ctx->sh_desc_dec;
	cnstr_shdsc_xts_skcipher_decap(desc, &ctx->cdata);
>>>>>>> upstream/android-13
	dma_sync_single_for_device(jrdev, ctx->sh_desc_dec_dma,
				   desc_bytes(desc), ctx->dir);

	return 0;
}

/*
 * aead_edesc - s/w-extended aead descriptor
 * @src_nents: number of segments in input s/w scatterlist
 * @dst_nents: number of segments in output s/w scatterlist
<<<<<<< HEAD
 * @sec4_sg_bytes: length of dma mapped sec4_sg space
=======
 * @mapped_src_nents: number of segments in input h/w link table
 * @mapped_dst_nents: number of segments in output h/w link table
 * @sec4_sg_bytes: length of dma mapped sec4_sg space
 * @bklog: stored to determine if the request needs backlog
>>>>>>> upstream/android-13
 * @sec4_sg_dma: bus physical mapped address of h/w link table
 * @sec4_sg: pointer to h/w link table
 * @hw_desc: the h/w job descriptor followed by any referenced link tables
 */
struct aead_edesc {
	int src_nents;
	int dst_nents;
<<<<<<< HEAD
	int sec4_sg_bytes;
=======
	int mapped_src_nents;
	int mapped_dst_nents;
	int sec4_sg_bytes;
	bool bklog;
>>>>>>> upstream/android-13
	dma_addr_t sec4_sg_dma;
	struct sec4_sg_entry *sec4_sg;
	u32 hw_desc[];
};

/*
<<<<<<< HEAD
 * ablkcipher_edesc - s/w-extended ablkcipher descriptor
 * @src_nents: number of segments in input s/w scatterlist
 * @dst_nents: number of segments in output s/w scatterlist
 * @iv_dma: dma address of iv for checking continuity and link table
 * @iv_dir: DMA mapping direction for IV
 * @sec4_sg_bytes: length of dma mapped sec4_sg space
=======
 * skcipher_edesc - s/w-extended skcipher descriptor
 * @src_nents: number of segments in input s/w scatterlist
 * @dst_nents: number of segments in output s/w scatterlist
 * @mapped_src_nents: number of segments in input h/w link table
 * @mapped_dst_nents: number of segments in output h/w link table
 * @iv_dma: dma address of iv for checking continuity and link table
 * @sec4_sg_bytes: length of dma mapped sec4_sg space
 * @bklog: stored to determine if the request needs backlog
>>>>>>> upstream/android-13
 * @sec4_sg_dma: bus physical mapped address of h/w link table
 * @sec4_sg: pointer to h/w link table
 * @hw_desc: the h/w job descriptor followed by any referenced link tables
 *	     and IV
 */
<<<<<<< HEAD
struct ablkcipher_edesc {
	int src_nents;
	int dst_nents;
	dma_addr_t iv_dma;
	enum dma_data_direction iv_dir;
	int sec4_sg_bytes;
	dma_addr_t sec4_sg_dma;
	struct sec4_sg_entry *sec4_sg;
	u32 hw_desc[0];
=======
struct skcipher_edesc {
	int src_nents;
	int dst_nents;
	int mapped_src_nents;
	int mapped_dst_nents;
	dma_addr_t iv_dma;
	int sec4_sg_bytes;
	bool bklog;
	dma_addr_t sec4_sg_dma;
	struct sec4_sg_entry *sec4_sg;
	u32 hw_desc[];
>>>>>>> upstream/android-13
};

static void caam_unmap(struct device *dev, struct scatterlist *src,
		       struct scatterlist *dst, int src_nents,
		       int dst_nents,
<<<<<<< HEAD
		       dma_addr_t iv_dma, int ivsize,
		       enum dma_data_direction iv_dir, dma_addr_t sec4_sg_dma,
=======
		       dma_addr_t iv_dma, int ivsize, dma_addr_t sec4_sg_dma,
>>>>>>> upstream/android-13
		       int sec4_sg_bytes)
{
	if (dst != src) {
		if (src_nents)
			dma_unmap_sg(dev, src, src_nents, DMA_TO_DEVICE);
<<<<<<< HEAD
		dma_unmap_sg(dev, dst, dst_nents, DMA_FROM_DEVICE);
=======
		if (dst_nents)
			dma_unmap_sg(dev, dst, dst_nents, DMA_FROM_DEVICE);
>>>>>>> upstream/android-13
	} else {
		dma_unmap_sg(dev, src, src_nents, DMA_BIDIRECTIONAL);
	}

	if (iv_dma)
<<<<<<< HEAD
		dma_unmap_single(dev, iv_dma, ivsize, iv_dir);
=======
		dma_unmap_single(dev, iv_dma, ivsize, DMA_BIDIRECTIONAL);
>>>>>>> upstream/android-13
	if (sec4_sg_bytes)
		dma_unmap_single(dev, sec4_sg_dma, sec4_sg_bytes,
				 DMA_TO_DEVICE);
}

static void aead_unmap(struct device *dev,
		       struct aead_edesc *edesc,
		       struct aead_request *req)
{
	caam_unmap(dev, req->src, req->dst,
<<<<<<< HEAD
		   edesc->src_nents, edesc->dst_nents, 0, 0, DMA_NONE,
		   edesc->sec4_sg_dma, edesc->sec4_sg_bytes);
}

static void ablkcipher_unmap(struct device *dev,
			     struct ablkcipher_edesc *edesc,
			     struct ablkcipher_request *req)
{
	struct crypto_ablkcipher *ablkcipher = crypto_ablkcipher_reqtfm(req);
	int ivsize = crypto_ablkcipher_ivsize(ablkcipher);

	caam_unmap(dev, req->src, req->dst,
		   edesc->src_nents, edesc->dst_nents,
		   edesc->iv_dma, ivsize, edesc->iv_dir,
		   edesc->sec4_sg_dma, edesc->sec4_sg_bytes);
}

static void aead_encrypt_done(struct device *jrdev, u32 *desc, u32 err,
				   void *context)
{
	struct aead_request *req = context;
	struct aead_edesc *edesc;

#ifdef DEBUG
	dev_err(jrdev, "%s %d: err 0x%x\n", __func__, __LINE__, err);
#endif

	edesc = container_of(desc, struct aead_edesc, hw_desc[0]);

	if (err)
		caam_jr_strstatus(jrdev, err);
=======
		   edesc->src_nents, edesc->dst_nents, 0, 0,
		   edesc->sec4_sg_dma, edesc->sec4_sg_bytes);
}

static void skcipher_unmap(struct device *dev, struct skcipher_edesc *edesc,
			   struct skcipher_request *req)
{
	struct crypto_skcipher *skcipher = crypto_skcipher_reqtfm(req);
	int ivsize = crypto_skcipher_ivsize(skcipher);

	caam_unmap(dev, req->src, req->dst,
		   edesc->src_nents, edesc->dst_nents,
		   edesc->iv_dma, ivsize,
		   edesc->sec4_sg_dma, edesc->sec4_sg_bytes);
}

static void aead_crypt_done(struct device *jrdev, u32 *desc, u32 err,
			    void *context)
{
	struct aead_request *req = context;
	struct caam_aead_req_ctx *rctx = aead_request_ctx(req);
	struct caam_drv_private_jr *jrp = dev_get_drvdata(jrdev);
	struct aead_edesc *edesc;
	int ecode = 0;
	bool has_bklog;

	dev_dbg(jrdev, "%s %d: err 0x%x\n", __func__, __LINE__, err);

	edesc = rctx->edesc;
	has_bklog = edesc->bklog;

	if (err)
		ecode = caam_jr_strstatus(jrdev, err);
>>>>>>> upstream/android-13

	aead_unmap(jrdev, edesc, req);

	kfree(edesc);

<<<<<<< HEAD
	aead_request_complete(req, err);
}

static void aead_decrypt_done(struct device *jrdev, u32 *desc, u32 err,
				   void *context)
{
	struct aead_request *req = context;
	struct aead_edesc *edesc;

#ifdef DEBUG
	dev_err(jrdev, "%s %d: err 0x%x\n", __func__, __LINE__, err);
#endif

	edesc = container_of(desc, struct aead_edesc, hw_desc[0]);

	if (err)
		caam_jr_strstatus(jrdev, err);

	aead_unmap(jrdev, edesc, req);

	/*
	 * verify hw auth check passed else return -EBADMSG
	 */
	if ((err & JRSTA_CCBERR_ERRID_MASK) == JRSTA_CCBERR_ERRID_ICVCHK)
		err = -EBADMSG;

	kfree(edesc);

	aead_request_complete(req, err);
}

static void ablkcipher_encrypt_done(struct device *jrdev, u32 *desc, u32 err,
				   void *context)
{
	struct ablkcipher_request *req = context;
	struct ablkcipher_edesc *edesc;
	struct crypto_ablkcipher *ablkcipher = crypto_ablkcipher_reqtfm(req);
	struct caam_ctx *ctx = crypto_ablkcipher_ctx(ablkcipher);
	int ivsize = crypto_ablkcipher_ivsize(ablkcipher);

#ifdef DEBUG
	dev_err(jrdev, "%s %d: err 0x%x\n", __func__, __LINE__, err);
#endif

	edesc = container_of(desc, struct ablkcipher_edesc, hw_desc[0]);

	if (err)
		caam_jr_strstatus(jrdev, err);

#ifdef DEBUG
	print_hex_dump(KERN_ERR, "dstiv  @"__stringify(__LINE__)": ",
		       DUMP_PREFIX_ADDRESS, 16, 4, req->info,
		       edesc->src_nents > 1 ? 100 : ivsize, 1);
#endif
	caam_dump_sg(KERN_ERR, "dst    @" __stringify(__LINE__)": ",
		     DUMP_PREFIX_ADDRESS, 16, 4, req->dst,
		     edesc->dst_nents > 1 ? 100 : req->nbytes, 1);

	ablkcipher_unmap(jrdev, edesc, req);

	/*
	 * The crypto API expects us to set the IV (req->info) to the last
	 * ciphertext block when running in CBC mode.
	 */
	if ((ctx->cdata.algtype & OP_ALG_AAI_MASK) == OP_ALG_AAI_CBC)
		scatterwalk_map_and_copy(req->info, req->dst, req->nbytes -
					 ivsize, ivsize, 0);

	/* In case initial IV was generated, copy it in GIVCIPHER request */
	if (edesc->iv_dir == DMA_FROM_DEVICE) {
		u8 *iv;
		struct skcipher_givcrypt_request *greq;

		greq = container_of(req, struct skcipher_givcrypt_request,
				    creq);
		iv = (u8 *)edesc->hw_desc + desc_bytes(edesc->hw_desc) +
		     edesc->sec4_sg_bytes;
		memcpy(greq->giv, iv, ivsize);
	}

	kfree(edesc);

	ablkcipher_request_complete(req, err);
}

static void ablkcipher_decrypt_done(struct device *jrdev, u32 *desc, u32 err,
				    void *context)
{
	struct ablkcipher_request *req = context;
	struct ablkcipher_edesc *edesc;
#ifdef DEBUG
	struct crypto_ablkcipher *ablkcipher = crypto_ablkcipher_reqtfm(req);
	int ivsize = crypto_ablkcipher_ivsize(ablkcipher);

	dev_err(jrdev, "%s %d: err 0x%x\n", __func__, __LINE__, err);
#endif

	edesc = container_of(desc, struct ablkcipher_edesc, hw_desc[0]);
	if (err)
		caam_jr_strstatus(jrdev, err);

#ifdef DEBUG
	print_hex_dump(KERN_ERR, "dstiv  @"__stringify(__LINE__)": ",
		       DUMP_PREFIX_ADDRESS, 16, 4, req->info,
		       ivsize, 1);
#endif
	caam_dump_sg(KERN_ERR, "dst    @" __stringify(__LINE__)": ",
		     DUMP_PREFIX_ADDRESS, 16, 4, req->dst,
		     edesc->dst_nents > 1 ? 100 : req->nbytes, 1);

	ablkcipher_unmap(jrdev, edesc, req);
	kfree(edesc);

	ablkcipher_request_complete(req, err);
=======
	/*
	 * If no backlog flag, the completion of the request is done
	 * by CAAM, not crypto engine.
	 */
	if (!has_bklog)
		aead_request_complete(req, ecode);
	else
		crypto_finalize_aead_request(jrp->engine, req, ecode);
}

static void skcipher_crypt_done(struct device *jrdev, u32 *desc, u32 err,
				void *context)
{
	struct skcipher_request *req = context;
	struct skcipher_edesc *edesc;
	struct caam_skcipher_req_ctx *rctx = skcipher_request_ctx(req);
	struct crypto_skcipher *skcipher = crypto_skcipher_reqtfm(req);
	struct caam_drv_private_jr *jrp = dev_get_drvdata(jrdev);
	int ivsize = crypto_skcipher_ivsize(skcipher);
	int ecode = 0;
	bool has_bklog;

	dev_dbg(jrdev, "%s %d: err 0x%x\n", __func__, __LINE__, err);

	edesc = rctx->edesc;
	has_bklog = edesc->bklog;
	if (err)
		ecode = caam_jr_strstatus(jrdev, err);

	skcipher_unmap(jrdev, edesc, req);

	/*
	 * The crypto API expects us to set the IV (req->iv) to the last
	 * ciphertext block (CBC mode) or last counter (CTR mode).
	 * This is used e.g. by the CTS mode.
	 */
	if (ivsize && !ecode) {
		memcpy(req->iv, (u8 *)edesc->sec4_sg + edesc->sec4_sg_bytes,
		       ivsize);

		print_hex_dump_debug("dstiv  @" __stringify(__LINE__)": ",
				     DUMP_PREFIX_ADDRESS, 16, 4, req->iv,
				     ivsize, 1);
	}

	caam_dump_sg("dst    @" __stringify(__LINE__)": ",
		     DUMP_PREFIX_ADDRESS, 16, 4, req->dst,
		     edesc->dst_nents > 1 ? 100 : req->cryptlen, 1);

	kfree(edesc);

	/*
	 * If no backlog flag, the completion of the request is done
	 * by CAAM, not crypto engine.
	 */
	if (!has_bklog)
		skcipher_request_complete(req, ecode);
	else
		crypto_finalize_skcipher_request(jrp->engine, req, ecode);
>>>>>>> upstream/android-13
}

/*
 * Fill in aead job descriptor
 */
static void init_aead_job(struct aead_request *req,
			  struct aead_edesc *edesc,
			  bool all_contig, bool encrypt)
{
	struct crypto_aead *aead = crypto_aead_reqtfm(req);
	struct caam_ctx *ctx = crypto_aead_ctx(aead);
	int authsize = ctx->authsize;
	u32 *desc = edesc->hw_desc;
	u32 out_options, in_options;
	dma_addr_t dst_dma, src_dma;
	int len, sec4_sg_index = 0;
	dma_addr_t ptr;
	u32 *sh_desc;

	sh_desc = encrypt ? ctx->sh_desc_enc : ctx->sh_desc_dec;
	ptr = encrypt ? ctx->sh_desc_enc_dma : ctx->sh_desc_dec_dma;

	len = desc_len(sh_desc);
	init_job_desc_shared(desc, ptr, len, HDR_SHARE_DEFER | HDR_REVERSE);

	if (all_contig) {
<<<<<<< HEAD
		src_dma = edesc->src_nents ? sg_dma_address(req->src) : 0;
		in_options = 0;
	} else {
		src_dma = edesc->sec4_sg_dma;
		sec4_sg_index += edesc->src_nents;
=======
		src_dma = edesc->mapped_src_nents ? sg_dma_address(req->src) :
						    0;
		in_options = 0;
	} else {
		src_dma = edesc->sec4_sg_dma;
		sec4_sg_index += edesc->mapped_src_nents;
>>>>>>> upstream/android-13
		in_options = LDST_SGF;
	}

	append_seq_in_ptr(desc, src_dma, req->assoclen + req->cryptlen,
			  in_options);

	dst_dma = src_dma;
	out_options = in_options;

	if (unlikely(req->src != req->dst)) {
<<<<<<< HEAD
		if (edesc->dst_nents == 1) {
=======
		if (!edesc->mapped_dst_nents) {
			dst_dma = 0;
			out_options = 0;
		} else if (edesc->mapped_dst_nents == 1) {
>>>>>>> upstream/android-13
			dst_dma = sg_dma_address(req->dst);
			out_options = 0;
		} else {
			dst_dma = edesc->sec4_sg_dma +
				  sec4_sg_index *
				  sizeof(struct sec4_sg_entry);
			out_options = LDST_SGF;
		}
	}

	if (encrypt)
		append_seq_out_ptr(desc, dst_dma,
				   req->assoclen + req->cryptlen + authsize,
				   out_options);
	else
		append_seq_out_ptr(desc, dst_dma,
				   req->assoclen + req->cryptlen - authsize,
				   out_options);
}

static void init_gcm_job(struct aead_request *req,
			 struct aead_edesc *edesc,
			 bool all_contig, bool encrypt)
{
	struct crypto_aead *aead = crypto_aead_reqtfm(req);
	struct caam_ctx *ctx = crypto_aead_ctx(aead);
	unsigned int ivsize = crypto_aead_ivsize(aead);
	u32 *desc = edesc->hw_desc;
	bool generic_gcm = (ivsize == GCM_AES_IV_SIZE);
	unsigned int last;

	init_aead_job(req, edesc, all_contig, encrypt);
	append_math_add_imm_u32(desc, REG3, ZERO, IMM, req->assoclen);

	/* BUG This should not be specific to generic GCM. */
	last = 0;
	if (encrypt && generic_gcm && !(req->assoclen + req->cryptlen))
		last = FIFOLD_TYPE_LAST1;

	/* Read GCM IV */
	append_cmd(desc, CMD_FIFO_LOAD | FIFOLD_CLASS_CLASS1 | IMMEDIATE |
			 FIFOLD_TYPE_IV | FIFOLD_TYPE_FLUSH1 | GCM_AES_IV_SIZE | last);
	/* Append Salt */
	if (!generic_gcm)
		append_data(desc, ctx->key + ctx->cdata.keylen, 4);
	/* Append IV */
	append_data(desc, req->iv, ivsize);
	/* End of blank commands */
}

<<<<<<< HEAD
=======
static void init_chachapoly_job(struct aead_request *req,
				struct aead_edesc *edesc, bool all_contig,
				bool encrypt)
{
	struct crypto_aead *aead = crypto_aead_reqtfm(req);
	unsigned int ivsize = crypto_aead_ivsize(aead);
	unsigned int assoclen = req->assoclen;
	u32 *desc = edesc->hw_desc;
	u32 ctx_iv_off = 4;

	init_aead_job(req, edesc, all_contig, encrypt);

	if (ivsize != CHACHAPOLY_IV_SIZE) {
		/* IPsec specific: CONTEXT1[223:128] = {NONCE, IV} */
		ctx_iv_off += 4;

		/*
		 * The associated data comes already with the IV but we need
		 * to skip it when we authenticate or encrypt...
		 */
		assoclen -= ivsize;
	}

	append_math_add_imm_u32(desc, REG3, ZERO, IMM, assoclen);

	/*
	 * For IPsec load the IV further in the same register.
	 * For RFC7539 simply load the 12 bytes nonce in a single operation
	 */
	append_load_as_imm(desc, req->iv, ivsize, LDST_CLASS_1_CCB |
			   LDST_SRCDST_BYTE_CONTEXT |
			   ctx_iv_off << LDST_OFFSET_SHIFT);
}

>>>>>>> upstream/android-13
static void init_authenc_job(struct aead_request *req,
			     struct aead_edesc *edesc,
			     bool all_contig, bool encrypt)
{
	struct crypto_aead *aead = crypto_aead_reqtfm(req);
	struct caam_aead_alg *alg = container_of(crypto_aead_alg(aead),
						 struct caam_aead_alg, aead);
	unsigned int ivsize = crypto_aead_ivsize(aead);
	struct caam_ctx *ctx = crypto_aead_ctx(aead);
	struct caam_drv_private *ctrlpriv = dev_get_drvdata(ctx->jrdev->parent);
	const bool ctr_mode = ((ctx->cdata.algtype & OP_ALG_AAI_MASK) ==
			       OP_ALG_AAI_CTR_MOD128);
	const bool is_rfc3686 = alg->caam.rfc3686;
	u32 *desc = edesc->hw_desc;
	u32 ivoffset = 0;

	/*
	 * AES-CTR needs to load IV in CONTEXT1 reg
	 * at an offset of 128bits (16bytes)
	 * CONTEXT1[255:128] = IV
	 */
	if (ctr_mode)
		ivoffset = 16;

	/*
	 * RFC3686 specific:
	 *	CONTEXT1[255:128] = {NONCE, IV, COUNTER}
	 */
	if (is_rfc3686)
		ivoffset = 16 + CTR_RFC3686_NONCE_SIZE;

	init_aead_job(req, edesc, all_contig, encrypt);

	/*
	 * {REG3, DPOVRD} = assoclen, depending on whether MATH command supports
	 * having DPOVRD as destination.
	 */
	if (ctrlpriv->era < 3)
		append_math_add_imm_u32(desc, REG3, ZERO, IMM, req->assoclen);
	else
		append_math_add_imm_u32(desc, DPOVRD, ZERO, IMM, req->assoclen);

	if (ivsize && ((is_rfc3686 && encrypt) || !alg->caam.geniv))
		append_load_as_imm(desc, req->iv, ivsize,
				   LDST_CLASS_1_CCB |
				   LDST_SRCDST_BYTE_CONTEXT |
				   (ivoffset << LDST_OFFSET_SHIFT));
}

/*
<<<<<<< HEAD
 * Fill in ablkcipher job descriptor
 */
static void init_ablkcipher_job(u32 *sh_desc, dma_addr_t ptr,
				struct ablkcipher_edesc *edesc,
				struct ablkcipher_request *req)
{
	struct crypto_ablkcipher *ablkcipher = crypto_ablkcipher_reqtfm(req);
	int ivsize = crypto_ablkcipher_ivsize(ablkcipher);
	u32 *desc = edesc->hw_desc;
	u32 out_options = 0;
	dma_addr_t dst_dma;
	int len;

#ifdef DEBUG
	print_hex_dump(KERN_ERR, "presciv@"__stringify(__LINE__)": ",
		       DUMP_PREFIX_ADDRESS, 16, 4, req->info,
		       ivsize, 1);
	pr_err("asked=%d, nbytes%d\n",
	       (int)edesc->src_nents > 1 ? 100 : req->nbytes, req->nbytes);
#endif
	caam_dump_sg(KERN_ERR, "src    @" __stringify(__LINE__)": ",
		     DUMP_PREFIX_ADDRESS, 16, 4, req->src,
		     edesc->src_nents > 1 ? 100 : req->nbytes, 1);

	len = desc_len(sh_desc);
	init_job_desc_shared(desc, ptr, len, HDR_SHARE_DEFER | HDR_REVERSE);

	append_seq_in_ptr(desc, edesc->sec4_sg_dma, req->nbytes + ivsize,
			  LDST_SGF);

	if (likely(req->src == req->dst)) {
		dst_dma = edesc->sec4_sg_dma + sizeof(struct sec4_sg_entry);
		out_options = LDST_SGF;
	} else {
		if (edesc->dst_nents == 1) {
			dst_dma = sg_dma_address(req->dst);
		} else {
			dst_dma = edesc->sec4_sg_dma + (edesc->src_nents + 1) *
				  sizeof(struct sec4_sg_entry);
			out_options = LDST_SGF;
		}
	}
	append_seq_out_ptr(desc, dst_dma, req->nbytes, out_options);
}

/*
 * Fill in ablkcipher givencrypt job descriptor
 */
static void init_ablkcipher_giv_job(u32 *sh_desc, dma_addr_t ptr,
				    struct ablkcipher_edesc *edesc,
				    struct ablkcipher_request *req)
{
	struct crypto_ablkcipher *ablkcipher = crypto_ablkcipher_reqtfm(req);
	int ivsize = crypto_ablkcipher_ivsize(ablkcipher);
	u32 *desc = edesc->hw_desc;
	u32 in_options;
	dma_addr_t dst_dma, src_dma;
	int len, sec4_sg_index = 0;

#ifdef DEBUG
	print_hex_dump(KERN_ERR, "presciv@" __stringify(__LINE__) ": ",
		       DUMP_PREFIX_ADDRESS, 16, 4, req->info,
		       ivsize, 1);
#endif
	caam_dump_sg(KERN_ERR, "src    @" __stringify(__LINE__) ": ",
		     DUMP_PREFIX_ADDRESS, 16, 4, req->src,
		     edesc->src_nents > 1 ? 100 : req->nbytes, 1);
=======
 * Fill in skcipher job descriptor
 */
static void init_skcipher_job(struct skcipher_request *req,
			      struct skcipher_edesc *edesc,
			      const bool encrypt)
{
	struct crypto_skcipher *skcipher = crypto_skcipher_reqtfm(req);
	struct caam_ctx *ctx = crypto_skcipher_ctx(skcipher);
	struct device *jrdev = ctx->jrdev;
	int ivsize = crypto_skcipher_ivsize(skcipher);
	u32 *desc = edesc->hw_desc;
	u32 *sh_desc;
	u32 in_options = 0, out_options = 0;
	dma_addr_t src_dma, dst_dma, ptr;
	int len, sec4_sg_index = 0;

	print_hex_dump_debug("presciv@"__stringify(__LINE__)": ",
			     DUMP_PREFIX_ADDRESS, 16, 4, req->iv, ivsize, 1);
	dev_dbg(jrdev, "asked=%d, cryptlen%d\n",
	       (int)edesc->src_nents > 1 ? 100 : req->cryptlen, req->cryptlen);

	caam_dump_sg("src    @" __stringify(__LINE__)": ",
		     DUMP_PREFIX_ADDRESS, 16, 4, req->src,
		     edesc->src_nents > 1 ? 100 : req->cryptlen, 1);

	sh_desc = encrypt ? ctx->sh_desc_enc : ctx->sh_desc_dec;
	ptr = encrypt ? ctx->sh_desc_enc_dma : ctx->sh_desc_dec_dma;
>>>>>>> upstream/android-13

	len = desc_len(sh_desc);
	init_job_desc_shared(desc, ptr, len, HDR_SHARE_DEFER | HDR_REVERSE);

<<<<<<< HEAD
	if (edesc->src_nents == 1) {
		src_dma = sg_dma_address(req->src);
		in_options = 0;
	} else {
		src_dma = edesc->sec4_sg_dma;
		sec4_sg_index += edesc->src_nents;
		in_options = LDST_SGF;
	}
	append_seq_in_ptr(desc, src_dma, req->nbytes, in_options);

	dst_dma = edesc->sec4_sg_dma + sec4_sg_index *
		  sizeof(struct sec4_sg_entry);
	append_seq_out_ptr(desc, dst_dma, req->nbytes + ivsize, LDST_SGF);
=======
	if (ivsize || edesc->mapped_src_nents > 1) {
		src_dma = edesc->sec4_sg_dma;
		sec4_sg_index = edesc->mapped_src_nents + !!ivsize;
		in_options = LDST_SGF;
	} else {
		src_dma = sg_dma_address(req->src);
	}

	append_seq_in_ptr(desc, src_dma, req->cryptlen + ivsize, in_options);

	if (likely(req->src == req->dst)) {
		dst_dma = src_dma + !!ivsize * sizeof(struct sec4_sg_entry);
		out_options = in_options;
	} else if (!ivsize && edesc->mapped_dst_nents == 1) {
		dst_dma = sg_dma_address(req->dst);
	} else {
		dst_dma = edesc->sec4_sg_dma + sec4_sg_index *
			  sizeof(struct sec4_sg_entry);
		out_options = LDST_SGF;
	}

	append_seq_out_ptr(desc, dst_dma, req->cryptlen + ivsize, out_options);
>>>>>>> upstream/android-13
}

/*
 * allocate and map the aead extended descriptor
 */
static struct aead_edesc *aead_edesc_alloc(struct aead_request *req,
					   int desc_bytes, bool *all_contig_ptr,
					   bool encrypt)
{
	struct crypto_aead *aead = crypto_aead_reqtfm(req);
	struct caam_ctx *ctx = crypto_aead_ctx(aead);
	struct device *jrdev = ctx->jrdev;
<<<<<<< HEAD
	gfp_t flags = (req->base.flags & CRYPTO_TFM_REQ_MAY_SLEEP) ?
		       GFP_KERNEL : GFP_ATOMIC;
	int src_nents, mapped_src_nents, dst_nents = 0, mapped_dst_nents = 0;
=======
	struct caam_aead_req_ctx *rctx = aead_request_ctx(req);
	gfp_t flags = (req->base.flags & CRYPTO_TFM_REQ_MAY_SLEEP) ?
		       GFP_KERNEL : GFP_ATOMIC;
	int src_nents, mapped_src_nents, dst_nents = 0, mapped_dst_nents = 0;
	int src_len, dst_len = 0;
>>>>>>> upstream/android-13
	struct aead_edesc *edesc;
	int sec4_sg_index, sec4_sg_len, sec4_sg_bytes;
	unsigned int authsize = ctx->authsize;

	if (unlikely(req->dst != req->src)) {
<<<<<<< HEAD
		src_nents = sg_nents_for_len(req->src, req->assoclen +
					     req->cryptlen);
		if (unlikely(src_nents < 0)) {
			dev_err(jrdev, "Insufficient bytes (%d) in src S/G\n",
				req->assoclen + req->cryptlen);
			return ERR_PTR(src_nents);
		}

		dst_nents = sg_nents_for_len(req->dst, req->assoclen +
					     req->cryptlen +
						(encrypt ? authsize :
							   (-authsize)));
		if (unlikely(dst_nents < 0)) {
			dev_err(jrdev, "Insufficient bytes (%d) in dst S/G\n",
				req->assoclen + req->cryptlen +
				(encrypt ? authsize : (-authsize)));
			return ERR_PTR(dst_nents);
		}
	} else {
		src_nents = sg_nents_for_len(req->src, req->assoclen +
					     req->cryptlen +
					     (encrypt ? authsize : 0));
		if (unlikely(src_nents < 0)) {
			dev_err(jrdev, "Insufficient bytes (%d) in src S/G\n",
				req->assoclen + req->cryptlen +
				(encrypt ? authsize : 0));
=======
		src_len = req->assoclen + req->cryptlen;
		dst_len = src_len + (encrypt ? authsize : (-authsize));

		src_nents = sg_nents_for_len(req->src, src_len);
		if (unlikely(src_nents < 0)) {
			dev_err(jrdev, "Insufficient bytes (%d) in src S/G\n",
				src_len);
			return ERR_PTR(src_nents);
		}

		dst_nents = sg_nents_for_len(req->dst, dst_len);
		if (unlikely(dst_nents < 0)) {
			dev_err(jrdev, "Insufficient bytes (%d) in dst S/G\n",
				dst_len);
			return ERR_PTR(dst_nents);
		}
	} else {
		src_len = req->assoclen + req->cryptlen +
			  (encrypt ? authsize : 0);

		src_nents = sg_nents_for_len(req->src, src_len);
		if (unlikely(src_nents < 0)) {
			dev_err(jrdev, "Insufficient bytes (%d) in src S/G\n",
				src_len);
>>>>>>> upstream/android-13
			return ERR_PTR(src_nents);
		}
	}

	if (likely(req->src == req->dst)) {
		mapped_src_nents = dma_map_sg(jrdev, req->src, src_nents,
					      DMA_BIDIRECTIONAL);
		if (unlikely(!mapped_src_nents)) {
			dev_err(jrdev, "unable to map source\n");
			return ERR_PTR(-ENOMEM);
		}
	} else {
		/* Cover also the case of null (zero length) input data */
		if (src_nents) {
			mapped_src_nents = dma_map_sg(jrdev, req->src,
						      src_nents, DMA_TO_DEVICE);
			if (unlikely(!mapped_src_nents)) {
				dev_err(jrdev, "unable to map source\n");
				return ERR_PTR(-ENOMEM);
			}
		} else {
			mapped_src_nents = 0;
		}

<<<<<<< HEAD
		mapped_dst_nents = dma_map_sg(jrdev, req->dst, dst_nents,
					      DMA_FROM_DEVICE);
		if (unlikely(!mapped_dst_nents)) {
			dev_err(jrdev, "unable to map destination\n");
			dma_unmap_sg(jrdev, req->src, src_nents, DMA_TO_DEVICE);
			return ERR_PTR(-ENOMEM);
		}
	}

	sec4_sg_len = mapped_src_nents > 1 ? mapped_src_nents : 0;
	sec4_sg_len += mapped_dst_nents > 1 ? mapped_dst_nents : 0;
=======
		/* Cover also the case of null (zero length) output data */
		if (dst_nents) {
			mapped_dst_nents = dma_map_sg(jrdev, req->dst,
						      dst_nents,
						      DMA_FROM_DEVICE);
			if (unlikely(!mapped_dst_nents)) {
				dev_err(jrdev, "unable to map destination\n");
				dma_unmap_sg(jrdev, req->src, src_nents,
					     DMA_TO_DEVICE);
				return ERR_PTR(-ENOMEM);
			}
		} else {
			mapped_dst_nents = 0;
		}
	}

	/*
	 * HW reads 4 S/G entries at a time; make sure the reads don't go beyond
	 * the end of the table by allocating more S/G entries.
	 */
	sec4_sg_len = mapped_src_nents > 1 ? mapped_src_nents : 0;
	if (mapped_dst_nents > 1)
		sec4_sg_len += pad_sg_nents(mapped_dst_nents);
	else
		sec4_sg_len = pad_sg_nents(sec4_sg_len);

>>>>>>> upstream/android-13
	sec4_sg_bytes = sec4_sg_len * sizeof(struct sec4_sg_entry);

	/* allocate space for base edesc and hw desc commands, link tables */
	edesc = kzalloc(sizeof(*edesc) + desc_bytes + sec4_sg_bytes,
			GFP_DMA | flags);
	if (!edesc) {
		caam_unmap(jrdev, req->src, req->dst, src_nents, dst_nents, 0,
<<<<<<< HEAD
			   0, DMA_NONE, 0, 0);
=======
			   0, 0, 0);
>>>>>>> upstream/android-13
		return ERR_PTR(-ENOMEM);
	}

	edesc->src_nents = src_nents;
	edesc->dst_nents = dst_nents;
<<<<<<< HEAD
	edesc->sec4_sg = (void *)edesc + sizeof(struct aead_edesc) +
			 desc_bytes;
=======
	edesc->mapped_src_nents = mapped_src_nents;
	edesc->mapped_dst_nents = mapped_dst_nents;
	edesc->sec4_sg = (void *)edesc + sizeof(struct aead_edesc) +
			 desc_bytes;

	rctx->edesc = edesc;

>>>>>>> upstream/android-13
	*all_contig_ptr = !(mapped_src_nents > 1);

	sec4_sg_index = 0;
	if (mapped_src_nents > 1) {
<<<<<<< HEAD
		sg_to_sec4_sg_last(req->src, mapped_src_nents,
=======
		sg_to_sec4_sg_last(req->src, src_len,
>>>>>>> upstream/android-13
				   edesc->sec4_sg + sec4_sg_index, 0);
		sec4_sg_index += mapped_src_nents;
	}
	if (mapped_dst_nents > 1) {
<<<<<<< HEAD
		sg_to_sec4_sg_last(req->dst, mapped_dst_nents,
=======
		sg_to_sec4_sg_last(req->dst, dst_len,
>>>>>>> upstream/android-13
				   edesc->sec4_sg + sec4_sg_index, 0);
	}

	if (!sec4_sg_bytes)
		return edesc;

	edesc->sec4_sg_dma = dma_map_single(jrdev, edesc->sec4_sg,
					    sec4_sg_bytes, DMA_TO_DEVICE);
	if (dma_mapping_error(jrdev, edesc->sec4_sg_dma)) {
		dev_err(jrdev, "unable to map S/G table\n");
		aead_unmap(jrdev, edesc, req);
		kfree(edesc);
		return ERR_PTR(-ENOMEM);
	}

	edesc->sec4_sg_bytes = sec4_sg_bytes;

	return edesc;
}

<<<<<<< HEAD
static int gcm_encrypt(struct aead_request *req)
=======
static int aead_enqueue_req(struct device *jrdev, struct aead_request *req)
{
	struct caam_drv_private_jr *jrpriv = dev_get_drvdata(jrdev);
	struct caam_aead_req_ctx *rctx = aead_request_ctx(req);
	struct aead_edesc *edesc = rctx->edesc;
	u32 *desc = edesc->hw_desc;
	int ret;

	/*
	 * Only the backlog request are sent to crypto-engine since the others
	 * can be handled by CAAM, if free, especially since JR has up to 1024
	 * entries (more than the 10 entries from crypto-engine).
	 */
	if (req->base.flags & CRYPTO_TFM_REQ_MAY_BACKLOG)
		ret = crypto_transfer_aead_request_to_engine(jrpriv->engine,
							     req);
	else
		ret = caam_jr_enqueue(jrdev, desc, aead_crypt_done, req);

	if ((ret != -EINPROGRESS) && (ret != -EBUSY)) {
		aead_unmap(jrdev, edesc, req);
		kfree(rctx->edesc);
	}

	return ret;
}

static inline int chachapoly_crypt(struct aead_request *req, bool encrypt)
>>>>>>> upstream/android-13
{
	struct aead_edesc *edesc;
	struct crypto_aead *aead = crypto_aead_reqtfm(req);
	struct caam_ctx *ctx = crypto_aead_ctx(aead);
	struct device *jrdev = ctx->jrdev;
	bool all_contig;
	u32 *desc;
<<<<<<< HEAD
	int ret = 0;

	/* allocate extended descriptor */
	edesc = aead_edesc_alloc(req, GCM_DESC_JOB_IO_LEN, &all_contig, true);
=======

	edesc = aead_edesc_alloc(req, CHACHAPOLY_DESC_JOB_IO_LEN, &all_contig,
				 encrypt);
	if (IS_ERR(edesc))
		return PTR_ERR(edesc);

	desc = edesc->hw_desc;

	init_chachapoly_job(req, edesc, all_contig, encrypt);
	print_hex_dump_debug("chachapoly jobdesc@" __stringify(__LINE__)": ",
			     DUMP_PREFIX_ADDRESS, 16, 4, desc, desc_bytes(desc),
			     1);

	return aead_enqueue_req(jrdev, req);
}

static int chachapoly_encrypt(struct aead_request *req)
{
	return chachapoly_crypt(req, true);
}

static int chachapoly_decrypt(struct aead_request *req)
{
	return chachapoly_crypt(req, false);
}

static inline int aead_crypt(struct aead_request *req, bool encrypt)
{
	struct aead_edesc *edesc;
	struct crypto_aead *aead = crypto_aead_reqtfm(req);
	struct caam_ctx *ctx = crypto_aead_ctx(aead);
	struct device *jrdev = ctx->jrdev;
	bool all_contig;

	/* allocate extended descriptor */
	edesc = aead_edesc_alloc(req, AUTHENC_DESC_JOB_IO_LEN,
				 &all_contig, encrypt);
>>>>>>> upstream/android-13
	if (IS_ERR(edesc))
		return PTR_ERR(edesc);

	/* Create and submit job descriptor */
<<<<<<< HEAD
	init_gcm_job(req, edesc, all_contig, true);
#ifdef DEBUG
	print_hex_dump(KERN_ERR, "aead jobdesc@"__stringify(__LINE__)": ",
		       DUMP_PREFIX_ADDRESS, 16, 4, edesc->hw_desc,
		       desc_bytes(edesc->hw_desc), 1);
#endif

	desc = edesc->hw_desc;
	ret = caam_jr_enqueue(jrdev, desc, aead_encrypt_done, req);
	if (!ret) {
		ret = -EINPROGRESS;
	} else {
		aead_unmap(jrdev, edesc, req);
		kfree(edesc);
	}

	return ret;
}

static int ipsec_gcm_encrypt(struct aead_request *req)
{
	if (req->assoclen < 8)
		return -EINVAL;

	return gcm_encrypt(req);
=======
	init_authenc_job(req, edesc, all_contig, encrypt);

	print_hex_dump_debug("aead jobdesc@"__stringify(__LINE__)": ",
			     DUMP_PREFIX_ADDRESS, 16, 4, edesc->hw_desc,
			     desc_bytes(edesc->hw_desc), 1);

	return aead_enqueue_req(jrdev, req);
>>>>>>> upstream/android-13
}

static int aead_encrypt(struct aead_request *req)
{
<<<<<<< HEAD
	struct aead_edesc *edesc;
	struct crypto_aead *aead = crypto_aead_reqtfm(req);
	struct caam_ctx *ctx = crypto_aead_ctx(aead);
	struct device *jrdev = ctx->jrdev;
	bool all_contig;
	u32 *desc;
	int ret = 0;

	/* allocate extended descriptor */
	edesc = aead_edesc_alloc(req, AUTHENC_DESC_JOB_IO_LEN,
				 &all_contig, true);
	if (IS_ERR(edesc))
		return PTR_ERR(edesc);

	/* Create and submit job descriptor */
	init_authenc_job(req, edesc, all_contig, true);
#ifdef DEBUG
	print_hex_dump(KERN_ERR, "aead jobdesc@"__stringify(__LINE__)": ",
		       DUMP_PREFIX_ADDRESS, 16, 4, edesc->hw_desc,
		       desc_bytes(edesc->hw_desc), 1);
#endif

	desc = edesc->hw_desc;
	ret = caam_jr_enqueue(jrdev, desc, aead_encrypt_done, req);
	if (!ret) {
		ret = -EINPROGRESS;
	} else {
		aead_unmap(jrdev, edesc, req);
		kfree(edesc);
	}

	return ret;
}

static int gcm_decrypt(struct aead_request *req)
{
	struct aead_edesc *edesc;
	struct crypto_aead *aead = crypto_aead_reqtfm(req);
	struct caam_ctx *ctx = crypto_aead_ctx(aead);
	struct device *jrdev = ctx->jrdev;
	bool all_contig;
	u32 *desc;
	int ret = 0;

	/* allocate extended descriptor */
	edesc = aead_edesc_alloc(req, GCM_DESC_JOB_IO_LEN, &all_contig, false);
	if (IS_ERR(edesc))
		return PTR_ERR(edesc);

	/* Create and submit job descriptor*/
	init_gcm_job(req, edesc, all_contig, false);
#ifdef DEBUG
	print_hex_dump(KERN_ERR, "aead jobdesc@"__stringify(__LINE__)": ",
		       DUMP_PREFIX_ADDRESS, 16, 4, edesc->hw_desc,
		       desc_bytes(edesc->hw_desc), 1);
#endif

	desc = edesc->hw_desc;
	ret = caam_jr_enqueue(jrdev, desc, aead_decrypt_done, req);
	if (!ret) {
		ret = -EINPROGRESS;
	} else {
		aead_unmap(jrdev, edesc, req);
		kfree(edesc);
	}

	return ret;
}

static int ipsec_gcm_decrypt(struct aead_request *req)
{
	if (req->assoclen < 8)
		return -EINVAL;

	return gcm_decrypt(req);
=======
	return aead_crypt(req, true);
>>>>>>> upstream/android-13
}

static int aead_decrypt(struct aead_request *req)
{
<<<<<<< HEAD
=======
	return aead_crypt(req, false);
}

static int aead_do_one_req(struct crypto_engine *engine, void *areq)
{
	struct aead_request *req = aead_request_cast(areq);
	struct caam_ctx *ctx = crypto_aead_ctx(crypto_aead_reqtfm(req));
	struct caam_aead_req_ctx *rctx = aead_request_ctx(req);
	u32 *desc = rctx->edesc->hw_desc;
	int ret;

	rctx->edesc->bklog = true;

	ret = caam_jr_enqueue(ctx->jrdev, desc, aead_crypt_done, req);

	if (ret == -ENOSPC && engine->retry_support)
		return ret;

	if (ret != -EINPROGRESS) {
		aead_unmap(ctx->jrdev, rctx->edesc, req);
		kfree(rctx->edesc);
	} else {
		ret = 0;
	}

	return ret;
}

static inline int gcm_crypt(struct aead_request *req, bool encrypt)
{
>>>>>>> upstream/android-13
	struct aead_edesc *edesc;
	struct crypto_aead *aead = crypto_aead_reqtfm(req);
	struct caam_ctx *ctx = crypto_aead_ctx(aead);
	struct device *jrdev = ctx->jrdev;
	bool all_contig;
<<<<<<< HEAD
	u32 *desc;
	int ret = 0;

	caam_dump_sg(KERN_ERR, "dec src@" __stringify(__LINE__)": ",
		     DUMP_PREFIX_ADDRESS, 16, 4, req->src,
		     req->assoclen + req->cryptlen, 1);

	/* allocate extended descriptor */
	edesc = aead_edesc_alloc(req, AUTHENC_DESC_JOB_IO_LEN,
				 &all_contig, false);
	if (IS_ERR(edesc))
		return PTR_ERR(edesc);

	/* Create and submit job descriptor*/
	init_authenc_job(req, edesc, all_contig, false);
#ifdef DEBUG
	print_hex_dump(KERN_ERR, "aead jobdesc@"__stringify(__LINE__)": ",
		       DUMP_PREFIX_ADDRESS, 16, 4, edesc->hw_desc,
		       desc_bytes(edesc->hw_desc), 1);
#endif

	desc = edesc->hw_desc;
	ret = caam_jr_enqueue(jrdev, desc, aead_decrypt_done, req);
	if (!ret) {
		ret = -EINPROGRESS;
	} else {
		aead_unmap(jrdev, edesc, req);
		kfree(edesc);
	}

	return ret;
}

/*
 * allocate and map the ablkcipher extended descriptor for ablkcipher
 */
static struct ablkcipher_edesc *ablkcipher_edesc_alloc(struct ablkcipher_request
						       *req, int desc_bytes)
{
	struct crypto_ablkcipher *ablkcipher = crypto_ablkcipher_reqtfm(req);
	struct caam_ctx *ctx = crypto_ablkcipher_ctx(ablkcipher);
=======

	/* allocate extended descriptor */
	edesc = aead_edesc_alloc(req, GCM_DESC_JOB_IO_LEN, &all_contig,
				 encrypt);
	if (IS_ERR(edesc))
		return PTR_ERR(edesc);

	/* Create and submit job descriptor */
	init_gcm_job(req, edesc, all_contig, encrypt);

	print_hex_dump_debug("aead jobdesc@"__stringify(__LINE__)": ",
			     DUMP_PREFIX_ADDRESS, 16, 4, edesc->hw_desc,
			     desc_bytes(edesc->hw_desc), 1);

	return aead_enqueue_req(jrdev, req);
}

static int gcm_encrypt(struct aead_request *req)
{
	return gcm_crypt(req, true);
}

static int gcm_decrypt(struct aead_request *req)
{
	return gcm_crypt(req, false);
}

static int ipsec_gcm_encrypt(struct aead_request *req)
{
	return crypto_ipsec_check_assoclen(req->assoclen) ? : gcm_encrypt(req);
}

static int ipsec_gcm_decrypt(struct aead_request *req)
{
	return crypto_ipsec_check_assoclen(req->assoclen) ? : gcm_decrypt(req);
}

/*
 * allocate and map the skcipher extended descriptor for skcipher
 */
static struct skcipher_edesc *skcipher_edesc_alloc(struct skcipher_request *req,
						   int desc_bytes)
{
	struct crypto_skcipher *skcipher = crypto_skcipher_reqtfm(req);
	struct caam_ctx *ctx = crypto_skcipher_ctx(skcipher);
	struct caam_skcipher_req_ctx *rctx = skcipher_request_ctx(req);
>>>>>>> upstream/android-13
	struct device *jrdev = ctx->jrdev;
	gfp_t flags = (req->base.flags & CRYPTO_TFM_REQ_MAY_SLEEP) ?
		       GFP_KERNEL : GFP_ATOMIC;
	int src_nents, mapped_src_nents, dst_nents = 0, mapped_dst_nents = 0;
<<<<<<< HEAD
	struct ablkcipher_edesc *edesc;
	dma_addr_t iv_dma;
	u8 *iv;
	int ivsize = crypto_ablkcipher_ivsize(ablkcipher);
	int dst_sg_idx, sec4_sg_ents, sec4_sg_bytes;

	src_nents = sg_nents_for_len(req->src, req->nbytes);
	if (unlikely(src_nents < 0)) {
		dev_err(jrdev, "Insufficient bytes (%d) in src S/G\n",
			req->nbytes);
=======
	struct skcipher_edesc *edesc;
	dma_addr_t iv_dma = 0;
	u8 *iv;
	int ivsize = crypto_skcipher_ivsize(skcipher);
	int dst_sg_idx, sec4_sg_ents, sec4_sg_bytes;

	src_nents = sg_nents_for_len(req->src, req->cryptlen);
	if (unlikely(src_nents < 0)) {
		dev_err(jrdev, "Insufficient bytes (%d) in src S/G\n",
			req->cryptlen);
>>>>>>> upstream/android-13
		return ERR_PTR(src_nents);
	}

	if (req->dst != req->src) {
<<<<<<< HEAD
		dst_nents = sg_nents_for_len(req->dst, req->nbytes);
		if (unlikely(dst_nents < 0)) {
			dev_err(jrdev, "Insufficient bytes (%d) in dst S/G\n",
				req->nbytes);
=======
		dst_nents = sg_nents_for_len(req->dst, req->cryptlen);
		if (unlikely(dst_nents < 0)) {
			dev_err(jrdev, "Insufficient bytes (%d) in dst S/G\n",
				req->cryptlen);
>>>>>>> upstream/android-13
			return ERR_PTR(dst_nents);
		}
	}

	if (likely(req->src == req->dst)) {
		mapped_src_nents = dma_map_sg(jrdev, req->src, src_nents,
					      DMA_BIDIRECTIONAL);
		if (unlikely(!mapped_src_nents)) {
			dev_err(jrdev, "unable to map source\n");
			return ERR_PTR(-ENOMEM);
		}
	} else {
		mapped_src_nents = dma_map_sg(jrdev, req->src, src_nents,
					      DMA_TO_DEVICE);
		if (unlikely(!mapped_src_nents)) {
			dev_err(jrdev, "unable to map source\n");
			return ERR_PTR(-ENOMEM);
		}
<<<<<<< HEAD

=======
>>>>>>> upstream/android-13
		mapped_dst_nents = dma_map_sg(jrdev, req->dst, dst_nents,
					      DMA_FROM_DEVICE);
		if (unlikely(!mapped_dst_nents)) {
			dev_err(jrdev, "unable to map destination\n");
			dma_unmap_sg(jrdev, req->src, src_nents, DMA_TO_DEVICE);
			return ERR_PTR(-ENOMEM);
		}
	}

<<<<<<< HEAD
	sec4_sg_ents = 1 + mapped_src_nents;
	dst_sg_idx = sec4_sg_ents;
	sec4_sg_ents += mapped_dst_nents > 1 ? mapped_dst_nents : 0;
=======
	if (!ivsize && mapped_src_nents == 1)
		sec4_sg_ents = 0; // no need for an input hw s/g table
	else
		sec4_sg_ents = mapped_src_nents + !!ivsize;
	dst_sg_idx = sec4_sg_ents;

	/*
	 * Input, output HW S/G tables: [IV, src][dst, IV]
	 * IV entries point to the same buffer
	 * If src == dst, S/G entries are reused (S/G tables overlap)
	 *
	 * HW reads 4 S/G entries at a time; make sure the reads don't go beyond
	 * the end of the table by allocating more S/G entries. Logic:
	 * if (output S/G)
	 *      pad output S/G, if needed
	 * else if (input S/G) ...
	 *      pad input S/G, if needed
	 */
	if (ivsize || mapped_dst_nents > 1) {
		if (req->src == req->dst)
			sec4_sg_ents = !!ivsize + pad_sg_nents(sec4_sg_ents);
		else
			sec4_sg_ents += pad_sg_nents(mapped_dst_nents +
						     !!ivsize);
	} else {
		sec4_sg_ents = pad_sg_nents(sec4_sg_ents);
	}

>>>>>>> upstream/android-13
	sec4_sg_bytes = sec4_sg_ents * sizeof(struct sec4_sg_entry);

	/*
	 * allocate space for base edesc and hw desc commands, link tables, IV
	 */
	edesc = kzalloc(sizeof(*edesc) + desc_bytes + sec4_sg_bytes + ivsize,
			GFP_DMA | flags);
	if (!edesc) {
		dev_err(jrdev, "could not allocate extended descriptor\n");
		caam_unmap(jrdev, req->src, req->dst, src_nents, dst_nents, 0,
<<<<<<< HEAD
			   0, DMA_NONE, 0, 0);
=======
			   0, 0, 0);
>>>>>>> upstream/android-13
		return ERR_PTR(-ENOMEM);
	}

	edesc->src_nents = src_nents;
	edesc->dst_nents = dst_nents;
<<<<<<< HEAD
	edesc->sec4_sg_bytes = sec4_sg_bytes;
	edesc->sec4_sg = (struct sec4_sg_entry *)((u8 *)edesc->hw_desc +
						  desc_bytes);
	edesc->iv_dir = DMA_TO_DEVICE;

	/* Make sure IV is located in a DMAable area */
	iv = (u8 *)edesc->hw_desc + desc_bytes + sec4_sg_bytes;
	memcpy(iv, req->info, ivsize);

	iv_dma = dma_map_single(jrdev, iv, ivsize, DMA_TO_DEVICE);
	if (dma_mapping_error(jrdev, iv_dma)) {
		dev_err(jrdev, "unable to map IV\n");
		caam_unmap(jrdev, req->src, req->dst, src_nents, dst_nents, 0,
			   0, DMA_NONE, 0, 0);
		kfree(edesc);
		return ERR_PTR(-ENOMEM);
	}

	dma_to_sec4_sg_one(edesc->sec4_sg, iv_dma, ivsize, 0);
	sg_to_sec4_sg_last(req->src, mapped_src_nents, edesc->sec4_sg + 1, 0);

	if (mapped_dst_nents > 1) {
		sg_to_sec4_sg_last(req->dst, mapped_dst_nents,
				   edesc->sec4_sg + dst_sg_idx, 0);
	}

	edesc->sec4_sg_dma = dma_map_single(jrdev, edesc->sec4_sg,
					    sec4_sg_bytes, DMA_TO_DEVICE);
	if (dma_mapping_error(jrdev, edesc->sec4_sg_dma)) {
		dev_err(jrdev, "unable to map S/G table\n");
		caam_unmap(jrdev, req->src, req->dst, src_nents, dst_nents,
			   iv_dma, ivsize, DMA_TO_DEVICE, 0, 0);
		kfree(edesc);
		return ERR_PTR(-ENOMEM);
=======
	edesc->mapped_src_nents = mapped_src_nents;
	edesc->mapped_dst_nents = mapped_dst_nents;
	edesc->sec4_sg_bytes = sec4_sg_bytes;
	edesc->sec4_sg = (struct sec4_sg_entry *)((u8 *)edesc->hw_desc +
						  desc_bytes);
	rctx->edesc = edesc;

	/* Make sure IV is located in a DMAable area */
	if (ivsize) {
		iv = (u8 *)edesc->sec4_sg + sec4_sg_bytes;
		memcpy(iv, req->iv, ivsize);

		iv_dma = dma_map_single(jrdev, iv, ivsize, DMA_BIDIRECTIONAL);
		if (dma_mapping_error(jrdev, iv_dma)) {
			dev_err(jrdev, "unable to map IV\n");
			caam_unmap(jrdev, req->src, req->dst, src_nents,
				   dst_nents, 0, 0, 0, 0);
			kfree(edesc);
			return ERR_PTR(-ENOMEM);
		}

		dma_to_sec4_sg_one(edesc->sec4_sg, iv_dma, ivsize, 0);
	}
	if (dst_sg_idx)
		sg_to_sec4_sg(req->src, req->cryptlen, edesc->sec4_sg +
			      !!ivsize, 0);

	if (req->src != req->dst && (ivsize || mapped_dst_nents > 1))
		sg_to_sec4_sg(req->dst, req->cryptlen, edesc->sec4_sg +
			      dst_sg_idx, 0);

	if (ivsize)
		dma_to_sec4_sg_one(edesc->sec4_sg + dst_sg_idx +
				   mapped_dst_nents, iv_dma, ivsize, 0);

	if (ivsize || mapped_dst_nents > 1)
		sg_to_sec4_set_last(edesc->sec4_sg + dst_sg_idx +
				    mapped_dst_nents - 1 + !!ivsize);

	if (sec4_sg_bytes) {
		edesc->sec4_sg_dma = dma_map_single(jrdev, edesc->sec4_sg,
						    sec4_sg_bytes,
						    DMA_TO_DEVICE);
		if (dma_mapping_error(jrdev, edesc->sec4_sg_dma)) {
			dev_err(jrdev, "unable to map S/G table\n");
			caam_unmap(jrdev, req->src, req->dst, src_nents,
				   dst_nents, iv_dma, ivsize, 0, 0);
			kfree(edesc);
			return ERR_PTR(-ENOMEM);
		}
>>>>>>> upstream/android-13
	}

	edesc->iv_dma = iv_dma;

<<<<<<< HEAD
#ifdef DEBUG
	print_hex_dump(KERN_ERR, "ablkcipher sec4_sg@"__stringify(__LINE__)": ",
		       DUMP_PREFIX_ADDRESS, 16, 4, edesc->sec4_sg,
		       sec4_sg_bytes, 1);
#endif
=======
	print_hex_dump_debug("skcipher sec4_sg@" __stringify(__LINE__)": ",
			     DUMP_PREFIX_ADDRESS, 16, 4, edesc->sec4_sg,
			     sec4_sg_bytes, 1);
>>>>>>> upstream/android-13

	return edesc;
}

<<<<<<< HEAD
static int ablkcipher_encrypt(struct ablkcipher_request *req)
{
	struct ablkcipher_edesc *edesc;
	struct crypto_ablkcipher *ablkcipher = crypto_ablkcipher_reqtfm(req);
	struct caam_ctx *ctx = crypto_ablkcipher_ctx(ablkcipher);
	struct device *jrdev = ctx->jrdev;
	u32 *desc;
	int ret = 0;

	/* allocate extended descriptor */
	edesc = ablkcipher_edesc_alloc(req, DESC_JOB_IO_LEN * CAAM_CMD_SZ);
	if (IS_ERR(edesc))
		return PTR_ERR(edesc);

	/* Create and submit job descriptor*/
	init_ablkcipher_job(ctx->sh_desc_enc, ctx->sh_desc_enc_dma, edesc, req);
#ifdef DEBUG
	print_hex_dump(KERN_ERR, "ablkcipher jobdesc@"__stringify(__LINE__)": ",
		       DUMP_PREFIX_ADDRESS, 16, 4, edesc->hw_desc,
		       desc_bytes(edesc->hw_desc), 1);
#endif
	desc = edesc->hw_desc;
	ret = caam_jr_enqueue(jrdev, desc, ablkcipher_encrypt_done, req);

	if (!ret) {
		ret = -EINPROGRESS;
	} else {
		ablkcipher_unmap(jrdev, edesc, req);
		kfree(edesc);
=======
static int skcipher_do_one_req(struct crypto_engine *engine, void *areq)
{
	struct skcipher_request *req = skcipher_request_cast(areq);
	struct caam_ctx *ctx = crypto_skcipher_ctx(crypto_skcipher_reqtfm(req));
	struct caam_skcipher_req_ctx *rctx = skcipher_request_ctx(req);
	u32 *desc = rctx->edesc->hw_desc;
	int ret;

	rctx->edesc->bklog = true;

	ret = caam_jr_enqueue(ctx->jrdev, desc, skcipher_crypt_done, req);

	if (ret == -ENOSPC && engine->retry_support)
		return ret;

	if (ret != -EINPROGRESS) {
		skcipher_unmap(ctx->jrdev, rctx->edesc, req);
		kfree(rctx->edesc);
	} else {
		ret = 0;
>>>>>>> upstream/android-13
	}

	return ret;
}

<<<<<<< HEAD
static int ablkcipher_decrypt(struct ablkcipher_request *req)
{
	struct ablkcipher_edesc *edesc;
	struct crypto_ablkcipher *ablkcipher = crypto_ablkcipher_reqtfm(req);
	struct caam_ctx *ctx = crypto_ablkcipher_ctx(ablkcipher);
	int ivsize = crypto_ablkcipher_ivsize(ablkcipher);
	struct device *jrdev = ctx->jrdev;
	u32 *desc;
	int ret = 0;

	/* allocate extended descriptor */
	edesc = ablkcipher_edesc_alloc(req, DESC_JOB_IO_LEN * CAAM_CMD_SZ);
	if (IS_ERR(edesc))
		return PTR_ERR(edesc);

	/*
	 * The crypto API expects us to set the IV (req->info) to the last
	 * ciphertext block when running in CBC mode.
	 */
	if ((ctx->cdata.algtype & OP_ALG_AAI_MASK) == OP_ALG_AAI_CBC)
		scatterwalk_map_and_copy(req->info, req->src, req->nbytes -
					 ivsize, ivsize, 0);

	/* Create and submit job descriptor*/
	init_ablkcipher_job(ctx->sh_desc_dec, ctx->sh_desc_dec_dma, edesc, req);
	desc = edesc->hw_desc;
#ifdef DEBUG
	print_hex_dump(KERN_ERR, "ablkcipher jobdesc@"__stringify(__LINE__)": ",
		       DUMP_PREFIX_ADDRESS, 16, 4, edesc->hw_desc,
		       desc_bytes(edesc->hw_desc), 1);
#endif

	ret = caam_jr_enqueue(jrdev, desc, ablkcipher_decrypt_done, req);
	if (!ret) {
		ret = -EINPROGRESS;
	} else {
		ablkcipher_unmap(jrdev, edesc, req);
		kfree(edesc);
	}

	return ret;
}

/*
 * allocate and map the ablkcipher extended descriptor
 * for ablkcipher givencrypt
 */
static struct ablkcipher_edesc *ablkcipher_giv_edesc_alloc(
				struct skcipher_givcrypt_request *greq,
				int desc_bytes)
{
	struct ablkcipher_request *req = &greq->creq;
	struct crypto_ablkcipher *ablkcipher = crypto_ablkcipher_reqtfm(req);
	struct caam_ctx *ctx = crypto_ablkcipher_ctx(ablkcipher);
	struct device *jrdev = ctx->jrdev;
	gfp_t flags = (req->base.flags &  CRYPTO_TFM_REQ_MAY_SLEEP) ?
		       GFP_KERNEL : GFP_ATOMIC;
	int src_nents, mapped_src_nents, dst_nents, mapped_dst_nents;
	struct ablkcipher_edesc *edesc;
	dma_addr_t iv_dma;
	u8 *iv;
	int ivsize = crypto_ablkcipher_ivsize(ablkcipher);
	int dst_sg_idx, sec4_sg_ents, sec4_sg_bytes;

	src_nents = sg_nents_for_len(req->src, req->nbytes);
	if (unlikely(src_nents < 0)) {
		dev_err(jrdev, "Insufficient bytes (%d) in src S/G\n",
			req->nbytes);
		return ERR_PTR(src_nents);
	}

	if (likely(req->src == req->dst)) {
		mapped_src_nents = dma_map_sg(jrdev, req->src, src_nents,
					      DMA_BIDIRECTIONAL);
		if (unlikely(!mapped_src_nents)) {
			dev_err(jrdev, "unable to map source\n");
			return ERR_PTR(-ENOMEM);
		}

		dst_nents = src_nents;
		mapped_dst_nents = src_nents;
	} else {
		mapped_src_nents = dma_map_sg(jrdev, req->src, src_nents,
					      DMA_TO_DEVICE);
		if (unlikely(!mapped_src_nents)) {
			dev_err(jrdev, "unable to map source\n");
			return ERR_PTR(-ENOMEM);
		}

		dst_nents = sg_nents_for_len(req->dst, req->nbytes);
		if (unlikely(dst_nents < 0)) {
			dev_err(jrdev, "Insufficient bytes (%d) in dst S/G\n",
				req->nbytes);
			return ERR_PTR(dst_nents);
		}

		mapped_dst_nents = dma_map_sg(jrdev, req->dst, dst_nents,
					      DMA_FROM_DEVICE);
		if (unlikely(!mapped_dst_nents)) {
			dev_err(jrdev, "unable to map destination\n");
			dma_unmap_sg(jrdev, req->src, src_nents, DMA_TO_DEVICE);
			return ERR_PTR(-ENOMEM);
		}
	}

	sec4_sg_ents = mapped_src_nents > 1 ? mapped_src_nents : 0;
	dst_sg_idx = sec4_sg_ents;
	sec4_sg_ents += 1 + mapped_dst_nents;

	/*
	 * allocate space for base edesc and hw desc commands, link tables, IV
	 */
	sec4_sg_bytes = sec4_sg_ents * sizeof(struct sec4_sg_entry);
	edesc = kzalloc(sizeof(*edesc) + desc_bytes + sec4_sg_bytes + ivsize,
			GFP_DMA | flags);
	if (!edesc) {
		dev_err(jrdev, "could not allocate extended descriptor\n");
		caam_unmap(jrdev, req->src, req->dst, src_nents, dst_nents, 0,
			   0, DMA_NONE, 0, 0);
		return ERR_PTR(-ENOMEM);
	}

	edesc->src_nents = src_nents;
	edesc->dst_nents = dst_nents;
	edesc->sec4_sg_bytes = sec4_sg_bytes;
	edesc->sec4_sg = (struct sec4_sg_entry *)((u8 *)edesc->hw_desc +
						  desc_bytes);
	edesc->iv_dir = DMA_FROM_DEVICE;

	/* Make sure IV is located in a DMAable area */
	iv = (u8 *)edesc->hw_desc + desc_bytes + sec4_sg_bytes;
	iv_dma = dma_map_single(jrdev, iv, ivsize, DMA_FROM_DEVICE);
	if (dma_mapping_error(jrdev, iv_dma)) {
		dev_err(jrdev, "unable to map IV\n");
		caam_unmap(jrdev, req->src, req->dst, src_nents, dst_nents, 0,
			   0, DMA_NONE, 0, 0);
		kfree(edesc);
		return ERR_PTR(-ENOMEM);
	}

	if (mapped_src_nents > 1)
		sg_to_sec4_sg_last(req->src, mapped_src_nents, edesc->sec4_sg,
				   0);

	dma_to_sec4_sg_one(edesc->sec4_sg + dst_sg_idx, iv_dma, ivsize, 0);
	sg_to_sec4_sg_last(req->dst, mapped_dst_nents, edesc->sec4_sg +
			   dst_sg_idx + 1, 0);

	edesc->sec4_sg_dma = dma_map_single(jrdev, edesc->sec4_sg,
					    sec4_sg_bytes, DMA_TO_DEVICE);
	if (dma_mapping_error(jrdev, edesc->sec4_sg_dma)) {
		dev_err(jrdev, "unable to map S/G table\n");
		caam_unmap(jrdev, req->src, req->dst, src_nents, dst_nents,
			   iv_dma, ivsize, DMA_FROM_DEVICE, 0, 0);
		kfree(edesc);
		return ERR_PTR(-ENOMEM);
	}
	edesc->iv_dma = iv_dma;

#ifdef DEBUG
	print_hex_dump(KERN_ERR,
		       "ablkcipher sec4_sg@" __stringify(__LINE__) ": ",
		       DUMP_PREFIX_ADDRESS, 16, 4, edesc->sec4_sg,
		       sec4_sg_bytes, 1);
#endif

	return edesc;
}

static int ablkcipher_givencrypt(struct skcipher_givcrypt_request *creq)
{
	struct ablkcipher_request *req = &creq->creq;
	struct ablkcipher_edesc *edesc;
	struct crypto_ablkcipher *ablkcipher = crypto_ablkcipher_reqtfm(req);
	struct caam_ctx *ctx = crypto_ablkcipher_ctx(ablkcipher);
	struct device *jrdev = ctx->jrdev;
	u32 *desc;
	int ret = 0;

	/* allocate extended descriptor */
	edesc = ablkcipher_giv_edesc_alloc(creq, DESC_JOB_IO_LEN * CAAM_CMD_SZ);
=======
static inline bool xts_skcipher_ivsize(struct skcipher_request *req)
{
	struct crypto_skcipher *skcipher = crypto_skcipher_reqtfm(req);
	unsigned int ivsize = crypto_skcipher_ivsize(skcipher);

	return !!get_unaligned((u64 *)(req->iv + (ivsize / 2)));
}

static inline int skcipher_crypt(struct skcipher_request *req, bool encrypt)
{
	struct skcipher_edesc *edesc;
	struct crypto_skcipher *skcipher = crypto_skcipher_reqtfm(req);
	struct caam_ctx *ctx = crypto_skcipher_ctx(skcipher);
	struct device *jrdev = ctx->jrdev;
	struct caam_drv_private_jr *jrpriv = dev_get_drvdata(jrdev);
	struct caam_drv_private *ctrlpriv = dev_get_drvdata(jrdev->parent);
	u32 *desc;
	int ret = 0;

	/*
	 * XTS is expected to return an error even for input length = 0
	 * Note that the case input length < block size will be caught during
	 * HW offloading and return an error.
	 */
	if (!req->cryptlen && !ctx->fallback)
		return 0;

	if (ctx->fallback && ((ctrlpriv->era <= 8 && xts_skcipher_ivsize(req)) ||
			      ctx->xts_key_fallback)) {
		struct caam_skcipher_req_ctx *rctx = skcipher_request_ctx(req);

		skcipher_request_set_tfm(&rctx->fallback_req, ctx->fallback);
		skcipher_request_set_callback(&rctx->fallback_req,
					      req->base.flags,
					      req->base.complete,
					      req->base.data);
		skcipher_request_set_crypt(&rctx->fallback_req, req->src,
					   req->dst, req->cryptlen, req->iv);

		return encrypt ? crypto_skcipher_encrypt(&rctx->fallback_req) :
				 crypto_skcipher_decrypt(&rctx->fallback_req);
	}

	/* allocate extended descriptor */
	edesc = skcipher_edesc_alloc(req, DESC_JOB_IO_LEN * CAAM_CMD_SZ);
>>>>>>> upstream/android-13
	if (IS_ERR(edesc))
		return PTR_ERR(edesc);

	/* Create and submit job descriptor*/
<<<<<<< HEAD
	init_ablkcipher_giv_job(ctx->sh_desc_givenc, ctx->sh_desc_givenc_dma,
				edesc, req);
#ifdef DEBUG
	print_hex_dump(KERN_ERR,
		       "ablkcipher jobdesc@" __stringify(__LINE__) ": ",
		       DUMP_PREFIX_ADDRESS, 16, 4, edesc->hw_desc,
		       desc_bytes(edesc->hw_desc), 1);
#endif
	desc = edesc->hw_desc;
	ret = caam_jr_enqueue(jrdev, desc, ablkcipher_encrypt_done, req);

	if (!ret) {
		ret = -EINPROGRESS;
	} else {
		ablkcipher_unmap(jrdev, edesc, req);
=======
	init_skcipher_job(req, edesc, encrypt);

	print_hex_dump_debug("skcipher jobdesc@" __stringify(__LINE__)": ",
			     DUMP_PREFIX_ADDRESS, 16, 4, edesc->hw_desc,
			     desc_bytes(edesc->hw_desc), 1);

	desc = edesc->hw_desc;
	/*
	 * Only the backlog request are sent to crypto-engine since the others
	 * can be handled by CAAM, if free, especially since JR has up to 1024
	 * entries (more than the 10 entries from crypto-engine).
	 */
	if (req->base.flags & CRYPTO_TFM_REQ_MAY_BACKLOG)
		ret = crypto_transfer_skcipher_request_to_engine(jrpriv->engine,
								 req);
	else
		ret = caam_jr_enqueue(jrdev, desc, skcipher_crypt_done, req);

	if ((ret != -EINPROGRESS) && (ret != -EBUSY)) {
		skcipher_unmap(jrdev, edesc, req);
>>>>>>> upstream/android-13
		kfree(edesc);
	}

	return ret;
}

<<<<<<< HEAD
#define template_aead		template_u.aead
#define template_ablkcipher	template_u.ablkcipher
struct caam_alg_template {
	char name[CRYPTO_MAX_ALG_NAME];
	char driver_name[CRYPTO_MAX_ALG_NAME];
	unsigned int blocksize;
	u32 type;
	union {
		struct ablkcipher_alg ablkcipher;
	} template_u;
	u32 class1_alg_type;
	u32 class2_alg_type;
};

static struct caam_alg_template driver_algs[] = {
	/* ablkcipher descriptor */
	{
		.name = "cbc(aes)",
		.driver_name = "cbc-aes-caam",
		.blocksize = AES_BLOCK_SIZE,
		.type = CRYPTO_ALG_TYPE_GIVCIPHER,
		.template_ablkcipher = {
			.setkey = ablkcipher_setkey,
			.encrypt = ablkcipher_encrypt,
			.decrypt = ablkcipher_decrypt,
			.givencrypt = ablkcipher_givencrypt,
			.geniv = "<built-in>",
			.min_keysize = AES_MIN_KEY_SIZE,
			.max_keysize = AES_MAX_KEY_SIZE,
			.ivsize = AES_BLOCK_SIZE,
			},
		.class1_alg_type = OP_ALG_ALGSEL_AES | OP_ALG_AAI_CBC,
	},
	{
		.name = "cbc(des3_ede)",
		.driver_name = "cbc-3des-caam",
		.blocksize = DES3_EDE_BLOCK_SIZE,
		.type = CRYPTO_ALG_TYPE_GIVCIPHER,
		.template_ablkcipher = {
			.setkey = ablkcipher_setkey,
			.encrypt = ablkcipher_encrypt,
			.decrypt = ablkcipher_decrypt,
			.givencrypt = ablkcipher_givencrypt,
			.geniv = "<built-in>",
			.min_keysize = DES3_EDE_KEY_SIZE,
			.max_keysize = DES3_EDE_KEY_SIZE,
			.ivsize = DES3_EDE_BLOCK_SIZE,
			},
		.class1_alg_type = OP_ALG_ALGSEL_3DES | OP_ALG_AAI_CBC,
	},
	{
		.name = "cbc(des)",
		.driver_name = "cbc-des-caam",
		.blocksize = DES_BLOCK_SIZE,
		.type = CRYPTO_ALG_TYPE_GIVCIPHER,
		.template_ablkcipher = {
			.setkey = ablkcipher_setkey,
			.encrypt = ablkcipher_encrypt,
			.decrypt = ablkcipher_decrypt,
			.givencrypt = ablkcipher_givencrypt,
			.geniv = "<built-in>",
			.min_keysize = DES_KEY_SIZE,
			.max_keysize = DES_KEY_SIZE,
			.ivsize = DES_BLOCK_SIZE,
			},
		.class1_alg_type = OP_ALG_ALGSEL_DES | OP_ALG_AAI_CBC,
	},
	{
		.name = "ctr(aes)",
		.driver_name = "ctr-aes-caam",
		.blocksize = 1,
		.type = CRYPTO_ALG_TYPE_ABLKCIPHER,
		.template_ablkcipher = {
			.setkey = ablkcipher_setkey,
			.encrypt = ablkcipher_encrypt,
			.decrypt = ablkcipher_decrypt,
			.geniv = "chainiv",
			.min_keysize = AES_MIN_KEY_SIZE,
			.max_keysize = AES_MAX_KEY_SIZE,
			.ivsize = AES_BLOCK_SIZE,
			},
		.class1_alg_type = OP_ALG_ALGSEL_AES | OP_ALG_AAI_CTR_MOD128,
	},
	{
		.name = "rfc3686(ctr(aes))",
		.driver_name = "rfc3686-ctr-aes-caam",
		.blocksize = 1,
		.type = CRYPTO_ALG_TYPE_GIVCIPHER,
		.template_ablkcipher = {
			.setkey = ablkcipher_setkey,
			.encrypt = ablkcipher_encrypt,
			.decrypt = ablkcipher_decrypt,
			.givencrypt = ablkcipher_givencrypt,
			.geniv = "<built-in>",
=======
static int skcipher_encrypt(struct skcipher_request *req)
{
	return skcipher_crypt(req, true);
}

static int skcipher_decrypt(struct skcipher_request *req)
{
	return skcipher_crypt(req, false);
}

static struct caam_skcipher_alg driver_algs[] = {
	{
		.skcipher = {
			.base = {
				.cra_name = "cbc(aes)",
				.cra_driver_name = "cbc-aes-caam",
				.cra_blocksize = AES_BLOCK_SIZE,
			},
			.setkey = aes_skcipher_setkey,
			.encrypt = skcipher_encrypt,
			.decrypt = skcipher_decrypt,
			.min_keysize = AES_MIN_KEY_SIZE,
			.max_keysize = AES_MAX_KEY_SIZE,
			.ivsize = AES_BLOCK_SIZE,
		},
		.caam.class1_alg_type = OP_ALG_ALGSEL_AES | OP_ALG_AAI_CBC,
	},
	{
		.skcipher = {
			.base = {
				.cra_name = "cbc(des3_ede)",
				.cra_driver_name = "cbc-3des-caam",
				.cra_blocksize = DES3_EDE_BLOCK_SIZE,
			},
			.setkey = des3_skcipher_setkey,
			.encrypt = skcipher_encrypt,
			.decrypt = skcipher_decrypt,
			.min_keysize = DES3_EDE_KEY_SIZE,
			.max_keysize = DES3_EDE_KEY_SIZE,
			.ivsize = DES3_EDE_BLOCK_SIZE,
		},
		.caam.class1_alg_type = OP_ALG_ALGSEL_3DES | OP_ALG_AAI_CBC,
	},
	{
		.skcipher = {
			.base = {
				.cra_name = "cbc(des)",
				.cra_driver_name = "cbc-des-caam",
				.cra_blocksize = DES_BLOCK_SIZE,
			},
			.setkey = des_skcipher_setkey,
			.encrypt = skcipher_encrypt,
			.decrypt = skcipher_decrypt,
			.min_keysize = DES_KEY_SIZE,
			.max_keysize = DES_KEY_SIZE,
			.ivsize = DES_BLOCK_SIZE,
		},
		.caam.class1_alg_type = OP_ALG_ALGSEL_DES | OP_ALG_AAI_CBC,
	},
	{
		.skcipher = {
			.base = {
				.cra_name = "ctr(aes)",
				.cra_driver_name = "ctr-aes-caam",
				.cra_blocksize = 1,
			},
			.setkey = ctr_skcipher_setkey,
			.encrypt = skcipher_encrypt,
			.decrypt = skcipher_decrypt,
			.min_keysize = AES_MIN_KEY_SIZE,
			.max_keysize = AES_MAX_KEY_SIZE,
			.ivsize = AES_BLOCK_SIZE,
			.chunksize = AES_BLOCK_SIZE,
		},
		.caam.class1_alg_type = OP_ALG_ALGSEL_AES |
					OP_ALG_AAI_CTR_MOD128,
	},
	{
		.skcipher = {
			.base = {
				.cra_name = "rfc3686(ctr(aes))",
				.cra_driver_name = "rfc3686-ctr-aes-caam",
				.cra_blocksize = 1,
			},
			.setkey = rfc3686_skcipher_setkey,
			.encrypt = skcipher_encrypt,
			.decrypt = skcipher_decrypt,
>>>>>>> upstream/android-13
			.min_keysize = AES_MIN_KEY_SIZE +
				       CTR_RFC3686_NONCE_SIZE,
			.max_keysize = AES_MAX_KEY_SIZE +
				       CTR_RFC3686_NONCE_SIZE,
			.ivsize = CTR_RFC3686_IV_SIZE,
<<<<<<< HEAD
			},
		.class1_alg_type = OP_ALG_ALGSEL_AES | OP_ALG_AAI_CTR_MOD128,
	},
	{
		.name = "xts(aes)",
		.driver_name = "xts-aes-caam",
		.blocksize = AES_BLOCK_SIZE,
		.type = CRYPTO_ALG_TYPE_ABLKCIPHER,
		.template_ablkcipher = {
			.setkey = xts_ablkcipher_setkey,
			.encrypt = ablkcipher_encrypt,
			.decrypt = ablkcipher_decrypt,
			.geniv = "eseqiv",
			.min_keysize = 2 * AES_MIN_KEY_SIZE,
			.max_keysize = 2 * AES_MAX_KEY_SIZE,
			.ivsize = AES_BLOCK_SIZE,
			},
		.class1_alg_type = OP_ALG_ALGSEL_AES | OP_ALG_AAI_XTS,
=======
			.chunksize = AES_BLOCK_SIZE,
		},
		.caam = {
			.class1_alg_type = OP_ALG_ALGSEL_AES |
					   OP_ALG_AAI_CTR_MOD128,
			.rfc3686 = true,
		},
	},
	{
		.skcipher = {
			.base = {
				.cra_name = "xts(aes)",
				.cra_driver_name = "xts-aes-caam",
				.cra_flags = CRYPTO_ALG_NEED_FALLBACK,
				.cra_blocksize = AES_BLOCK_SIZE,
			},
			.setkey = xts_skcipher_setkey,
			.encrypt = skcipher_encrypt,
			.decrypt = skcipher_decrypt,
			.min_keysize = 2 * AES_MIN_KEY_SIZE,
			.max_keysize = 2 * AES_MAX_KEY_SIZE,
			.ivsize = AES_BLOCK_SIZE,
		},
		.caam.class1_alg_type = OP_ALG_ALGSEL_AES | OP_ALG_AAI_XTS,
	},
	{
		.skcipher = {
			.base = {
				.cra_name = "ecb(des)",
				.cra_driver_name = "ecb-des-caam",
				.cra_blocksize = DES_BLOCK_SIZE,
			},
			.setkey = des_skcipher_setkey,
			.encrypt = skcipher_encrypt,
			.decrypt = skcipher_decrypt,
			.min_keysize = DES_KEY_SIZE,
			.max_keysize = DES_KEY_SIZE,
		},
		.caam.class1_alg_type = OP_ALG_ALGSEL_DES | OP_ALG_AAI_ECB,
	},
	{
		.skcipher = {
			.base = {
				.cra_name = "ecb(aes)",
				.cra_driver_name = "ecb-aes-caam",
				.cra_blocksize = AES_BLOCK_SIZE,
			},
			.setkey = aes_skcipher_setkey,
			.encrypt = skcipher_encrypt,
			.decrypt = skcipher_decrypt,
			.min_keysize = AES_MIN_KEY_SIZE,
			.max_keysize = AES_MAX_KEY_SIZE,
		},
		.caam.class1_alg_type = OP_ALG_ALGSEL_AES | OP_ALG_AAI_ECB,
	},
	{
		.skcipher = {
			.base = {
				.cra_name = "ecb(des3_ede)",
				.cra_driver_name = "ecb-des3-caam",
				.cra_blocksize = DES3_EDE_BLOCK_SIZE,
			},
			.setkey = des3_skcipher_setkey,
			.encrypt = skcipher_encrypt,
			.decrypt = skcipher_decrypt,
			.min_keysize = DES3_EDE_KEY_SIZE,
			.max_keysize = DES3_EDE_KEY_SIZE,
		},
		.caam.class1_alg_type = OP_ALG_ALGSEL_3DES | OP_ALG_AAI_ECB,
>>>>>>> upstream/android-13
	},
};

static struct caam_aead_alg driver_aeads[] = {
	{
		.aead = {
			.base = {
				.cra_name = "rfc4106(gcm(aes))",
				.cra_driver_name = "rfc4106-gcm-aes-caam",
				.cra_blocksize = 1,
			},
			.setkey = rfc4106_setkey,
			.setauthsize = rfc4106_setauthsize,
			.encrypt = ipsec_gcm_encrypt,
			.decrypt = ipsec_gcm_decrypt,
			.ivsize = GCM_RFC4106_IV_SIZE,
			.maxauthsize = AES_BLOCK_SIZE,
		},
		.caam = {
			.class1_alg_type = OP_ALG_ALGSEL_AES | OP_ALG_AAI_GCM,
<<<<<<< HEAD
=======
			.nodkp = true,
>>>>>>> upstream/android-13
		},
	},
	{
		.aead = {
			.base = {
				.cra_name = "rfc4543(gcm(aes))",
				.cra_driver_name = "rfc4543-gcm-aes-caam",
				.cra_blocksize = 1,
			},
			.setkey = rfc4543_setkey,
			.setauthsize = rfc4543_setauthsize,
			.encrypt = ipsec_gcm_encrypt,
			.decrypt = ipsec_gcm_decrypt,
			.ivsize = GCM_RFC4543_IV_SIZE,
			.maxauthsize = AES_BLOCK_SIZE,
		},
		.caam = {
			.class1_alg_type = OP_ALG_ALGSEL_AES | OP_ALG_AAI_GCM,
<<<<<<< HEAD
=======
			.nodkp = true,
>>>>>>> upstream/android-13
		},
	},
	/* Galois Counter Mode */
	{
		.aead = {
			.base = {
				.cra_name = "gcm(aes)",
				.cra_driver_name = "gcm-aes-caam",
				.cra_blocksize = 1,
			},
			.setkey = gcm_setkey,
			.setauthsize = gcm_setauthsize,
			.encrypt = gcm_encrypt,
			.decrypt = gcm_decrypt,
			.ivsize = GCM_AES_IV_SIZE,
			.maxauthsize = AES_BLOCK_SIZE,
		},
		.caam = {
			.class1_alg_type = OP_ALG_ALGSEL_AES | OP_ALG_AAI_GCM,
<<<<<<< HEAD
=======
			.nodkp = true,
>>>>>>> upstream/android-13
		},
	},
	/* single-pass ipsec_esp descriptor */
	{
		.aead = {
			.base = {
				.cra_name = "authenc(hmac(md5),"
					    "ecb(cipher_null))",
				.cra_driver_name = "authenc-hmac-md5-"
						   "ecb-cipher_null-caam",
				.cra_blocksize = NULL_BLOCK_SIZE,
			},
			.setkey = aead_setkey,
			.setauthsize = aead_setauthsize,
			.encrypt = aead_encrypt,
			.decrypt = aead_decrypt,
			.ivsize = NULL_IV_SIZE,
			.maxauthsize = MD5_DIGEST_SIZE,
		},
		.caam = {
			.class2_alg_type = OP_ALG_ALGSEL_MD5 |
					   OP_ALG_AAI_HMAC_PRECOMP,
		},
	},
	{
		.aead = {
			.base = {
				.cra_name = "authenc(hmac(sha1),"
					    "ecb(cipher_null))",
				.cra_driver_name = "authenc-hmac-sha1-"
						   "ecb-cipher_null-caam",
				.cra_blocksize = NULL_BLOCK_SIZE,
			},
			.setkey = aead_setkey,
			.setauthsize = aead_setauthsize,
			.encrypt = aead_encrypt,
			.decrypt = aead_decrypt,
			.ivsize = NULL_IV_SIZE,
			.maxauthsize = SHA1_DIGEST_SIZE,
		},
		.caam = {
			.class2_alg_type = OP_ALG_ALGSEL_SHA1 |
					   OP_ALG_AAI_HMAC_PRECOMP,
		},
	},
	{
		.aead = {
			.base = {
				.cra_name = "authenc(hmac(sha224),"
					    "ecb(cipher_null))",
				.cra_driver_name = "authenc-hmac-sha224-"
						   "ecb-cipher_null-caam",
				.cra_blocksize = NULL_BLOCK_SIZE,
			},
			.setkey = aead_setkey,
			.setauthsize = aead_setauthsize,
			.encrypt = aead_encrypt,
			.decrypt = aead_decrypt,
			.ivsize = NULL_IV_SIZE,
			.maxauthsize = SHA224_DIGEST_SIZE,
		},
		.caam = {
			.class2_alg_type = OP_ALG_ALGSEL_SHA224 |
					   OP_ALG_AAI_HMAC_PRECOMP,
		},
	},
	{
		.aead = {
			.base = {
				.cra_name = "authenc(hmac(sha256),"
					    "ecb(cipher_null))",
				.cra_driver_name = "authenc-hmac-sha256-"
						   "ecb-cipher_null-caam",
				.cra_blocksize = NULL_BLOCK_SIZE,
			},
			.setkey = aead_setkey,
			.setauthsize = aead_setauthsize,
			.encrypt = aead_encrypt,
			.decrypt = aead_decrypt,
			.ivsize = NULL_IV_SIZE,
			.maxauthsize = SHA256_DIGEST_SIZE,
		},
		.caam = {
			.class2_alg_type = OP_ALG_ALGSEL_SHA256 |
					   OP_ALG_AAI_HMAC_PRECOMP,
		},
	},
	{
		.aead = {
			.base = {
				.cra_name = "authenc(hmac(sha384),"
					    "ecb(cipher_null))",
				.cra_driver_name = "authenc-hmac-sha384-"
						   "ecb-cipher_null-caam",
				.cra_blocksize = NULL_BLOCK_SIZE,
			},
			.setkey = aead_setkey,
			.setauthsize = aead_setauthsize,
			.encrypt = aead_encrypt,
			.decrypt = aead_decrypt,
			.ivsize = NULL_IV_SIZE,
			.maxauthsize = SHA384_DIGEST_SIZE,
		},
		.caam = {
			.class2_alg_type = OP_ALG_ALGSEL_SHA384 |
					   OP_ALG_AAI_HMAC_PRECOMP,
		},
	},
	{
		.aead = {
			.base = {
				.cra_name = "authenc(hmac(sha512),"
					    "ecb(cipher_null))",
				.cra_driver_name = "authenc-hmac-sha512-"
						   "ecb-cipher_null-caam",
				.cra_blocksize = NULL_BLOCK_SIZE,
			},
			.setkey = aead_setkey,
			.setauthsize = aead_setauthsize,
			.encrypt = aead_encrypt,
			.decrypt = aead_decrypt,
			.ivsize = NULL_IV_SIZE,
			.maxauthsize = SHA512_DIGEST_SIZE,
		},
		.caam = {
			.class2_alg_type = OP_ALG_ALGSEL_SHA512 |
					   OP_ALG_AAI_HMAC_PRECOMP,
		},
	},
	{
		.aead = {
			.base = {
				.cra_name = "authenc(hmac(md5),cbc(aes))",
				.cra_driver_name = "authenc-hmac-md5-"
						   "cbc-aes-caam",
				.cra_blocksize = AES_BLOCK_SIZE,
			},
			.setkey = aead_setkey,
			.setauthsize = aead_setauthsize,
			.encrypt = aead_encrypt,
			.decrypt = aead_decrypt,
			.ivsize = AES_BLOCK_SIZE,
			.maxauthsize = MD5_DIGEST_SIZE,
		},
		.caam = {
			.class1_alg_type = OP_ALG_ALGSEL_AES | OP_ALG_AAI_CBC,
			.class2_alg_type = OP_ALG_ALGSEL_MD5 |
					   OP_ALG_AAI_HMAC_PRECOMP,
		},
	},
	{
		.aead = {
			.base = {
				.cra_name = "echainiv(authenc(hmac(md5),"
					    "cbc(aes)))",
				.cra_driver_name = "echainiv-authenc-hmac-md5-"
						   "cbc-aes-caam",
				.cra_blocksize = AES_BLOCK_SIZE,
			},
			.setkey = aead_setkey,
			.setauthsize = aead_setauthsize,
			.encrypt = aead_encrypt,
			.decrypt = aead_decrypt,
			.ivsize = AES_BLOCK_SIZE,
			.maxauthsize = MD5_DIGEST_SIZE,
		},
		.caam = {
			.class1_alg_type = OP_ALG_ALGSEL_AES | OP_ALG_AAI_CBC,
			.class2_alg_type = OP_ALG_ALGSEL_MD5 |
					   OP_ALG_AAI_HMAC_PRECOMP,
			.geniv = true,
		},
	},
	{
		.aead = {
			.base = {
				.cra_name = "authenc(hmac(sha1),cbc(aes))",
				.cra_driver_name = "authenc-hmac-sha1-"
						   "cbc-aes-caam",
				.cra_blocksize = AES_BLOCK_SIZE,
			},
			.setkey = aead_setkey,
			.setauthsize = aead_setauthsize,
			.encrypt = aead_encrypt,
			.decrypt = aead_decrypt,
			.ivsize = AES_BLOCK_SIZE,
			.maxauthsize = SHA1_DIGEST_SIZE,
		},
		.caam = {
			.class1_alg_type = OP_ALG_ALGSEL_AES | OP_ALG_AAI_CBC,
			.class2_alg_type = OP_ALG_ALGSEL_SHA1 |
					   OP_ALG_AAI_HMAC_PRECOMP,
		},
	},
	{
		.aead = {
			.base = {
				.cra_name = "echainiv(authenc(hmac(sha1),"
					    "cbc(aes)))",
				.cra_driver_name = "echainiv-authenc-"
						   "hmac-sha1-cbc-aes-caam",
				.cra_blocksize = AES_BLOCK_SIZE,
			},
			.setkey = aead_setkey,
			.setauthsize = aead_setauthsize,
			.encrypt = aead_encrypt,
			.decrypt = aead_decrypt,
			.ivsize = AES_BLOCK_SIZE,
			.maxauthsize = SHA1_DIGEST_SIZE,
		},
		.caam = {
			.class1_alg_type = OP_ALG_ALGSEL_AES | OP_ALG_AAI_CBC,
			.class2_alg_type = OP_ALG_ALGSEL_SHA1 |
					   OP_ALG_AAI_HMAC_PRECOMP,
			.geniv = true,
		},
	},
	{
		.aead = {
			.base = {
				.cra_name = "authenc(hmac(sha224),cbc(aes))",
				.cra_driver_name = "authenc-hmac-sha224-"
						   "cbc-aes-caam",
				.cra_blocksize = AES_BLOCK_SIZE,
			},
			.setkey = aead_setkey,
			.setauthsize = aead_setauthsize,
			.encrypt = aead_encrypt,
			.decrypt = aead_decrypt,
			.ivsize = AES_BLOCK_SIZE,
			.maxauthsize = SHA224_DIGEST_SIZE,
		},
		.caam = {
			.class1_alg_type = OP_ALG_ALGSEL_AES | OP_ALG_AAI_CBC,
			.class2_alg_type = OP_ALG_ALGSEL_SHA224 |
					   OP_ALG_AAI_HMAC_PRECOMP,
		},
	},
	{
		.aead = {
			.base = {
				.cra_name = "echainiv(authenc(hmac(sha224),"
					    "cbc(aes)))",
				.cra_driver_name = "echainiv-authenc-"
						   "hmac-sha224-cbc-aes-caam",
				.cra_blocksize = AES_BLOCK_SIZE,
			},
			.setkey = aead_setkey,
			.setauthsize = aead_setauthsize,
			.encrypt = aead_encrypt,
			.decrypt = aead_decrypt,
			.ivsize = AES_BLOCK_SIZE,
			.maxauthsize = SHA224_DIGEST_SIZE,
		},
		.caam = {
			.class1_alg_type = OP_ALG_ALGSEL_AES | OP_ALG_AAI_CBC,
			.class2_alg_type = OP_ALG_ALGSEL_SHA224 |
					   OP_ALG_AAI_HMAC_PRECOMP,
			.geniv = true,
		},
	},
	{
		.aead = {
			.base = {
				.cra_name = "authenc(hmac(sha256),cbc(aes))",
				.cra_driver_name = "authenc-hmac-sha256-"
						   "cbc-aes-caam",
				.cra_blocksize = AES_BLOCK_SIZE,
			},
			.setkey = aead_setkey,
			.setauthsize = aead_setauthsize,
			.encrypt = aead_encrypt,
			.decrypt = aead_decrypt,
			.ivsize = AES_BLOCK_SIZE,
			.maxauthsize = SHA256_DIGEST_SIZE,
		},
		.caam = {
			.class1_alg_type = OP_ALG_ALGSEL_AES | OP_ALG_AAI_CBC,
			.class2_alg_type = OP_ALG_ALGSEL_SHA256 |
					   OP_ALG_AAI_HMAC_PRECOMP,
		},
	},
	{
		.aead = {
			.base = {
				.cra_name = "echainiv(authenc(hmac(sha256),"
					    "cbc(aes)))",
				.cra_driver_name = "echainiv-authenc-"
						   "hmac-sha256-cbc-aes-caam",
				.cra_blocksize = AES_BLOCK_SIZE,
			},
			.setkey = aead_setkey,
			.setauthsize = aead_setauthsize,
			.encrypt = aead_encrypt,
			.decrypt = aead_decrypt,
			.ivsize = AES_BLOCK_SIZE,
			.maxauthsize = SHA256_DIGEST_SIZE,
		},
		.caam = {
			.class1_alg_type = OP_ALG_ALGSEL_AES | OP_ALG_AAI_CBC,
			.class2_alg_type = OP_ALG_ALGSEL_SHA256 |
					   OP_ALG_AAI_HMAC_PRECOMP,
			.geniv = true,
		},
	},
	{
		.aead = {
			.base = {
				.cra_name = "authenc(hmac(sha384),cbc(aes))",
				.cra_driver_name = "authenc-hmac-sha384-"
						   "cbc-aes-caam",
				.cra_blocksize = AES_BLOCK_SIZE,
			},
			.setkey = aead_setkey,
			.setauthsize = aead_setauthsize,
			.encrypt = aead_encrypt,
			.decrypt = aead_decrypt,
			.ivsize = AES_BLOCK_SIZE,
			.maxauthsize = SHA384_DIGEST_SIZE,
		},
		.caam = {
			.class1_alg_type = OP_ALG_ALGSEL_AES | OP_ALG_AAI_CBC,
			.class2_alg_type = OP_ALG_ALGSEL_SHA384 |
					   OP_ALG_AAI_HMAC_PRECOMP,
		},
	},
	{
		.aead = {
			.base = {
				.cra_name = "echainiv(authenc(hmac(sha384),"
					    "cbc(aes)))",
				.cra_driver_name = "echainiv-authenc-"
						   "hmac-sha384-cbc-aes-caam",
				.cra_blocksize = AES_BLOCK_SIZE,
			},
			.setkey = aead_setkey,
			.setauthsize = aead_setauthsize,
			.encrypt = aead_encrypt,
			.decrypt = aead_decrypt,
			.ivsize = AES_BLOCK_SIZE,
			.maxauthsize = SHA384_DIGEST_SIZE,
		},
		.caam = {
			.class1_alg_type = OP_ALG_ALGSEL_AES | OP_ALG_AAI_CBC,
			.class2_alg_type = OP_ALG_ALGSEL_SHA384 |
					   OP_ALG_AAI_HMAC_PRECOMP,
			.geniv = true,
		},
	},
	{
		.aead = {
			.base = {
				.cra_name = "authenc(hmac(sha512),cbc(aes))",
				.cra_driver_name = "authenc-hmac-sha512-"
						   "cbc-aes-caam",
				.cra_blocksize = AES_BLOCK_SIZE,
			},
			.setkey = aead_setkey,
			.setauthsize = aead_setauthsize,
			.encrypt = aead_encrypt,
			.decrypt = aead_decrypt,
			.ivsize = AES_BLOCK_SIZE,
			.maxauthsize = SHA512_DIGEST_SIZE,
		},
		.caam = {
			.class1_alg_type = OP_ALG_ALGSEL_AES | OP_ALG_AAI_CBC,
			.class2_alg_type = OP_ALG_ALGSEL_SHA512 |
					   OP_ALG_AAI_HMAC_PRECOMP,
		},
	},
	{
		.aead = {
			.base = {
				.cra_name = "echainiv(authenc(hmac(sha512),"
					    "cbc(aes)))",
				.cra_driver_name = "echainiv-authenc-"
						   "hmac-sha512-cbc-aes-caam",
				.cra_blocksize = AES_BLOCK_SIZE,
			},
			.setkey = aead_setkey,
			.setauthsize = aead_setauthsize,
			.encrypt = aead_encrypt,
			.decrypt = aead_decrypt,
			.ivsize = AES_BLOCK_SIZE,
			.maxauthsize = SHA512_DIGEST_SIZE,
		},
		.caam = {
			.class1_alg_type = OP_ALG_ALGSEL_AES | OP_ALG_AAI_CBC,
			.class2_alg_type = OP_ALG_ALGSEL_SHA512 |
					   OP_ALG_AAI_HMAC_PRECOMP,
			.geniv = true,
		},
	},
	{
		.aead = {
			.base = {
				.cra_name = "authenc(hmac(md5),cbc(des3_ede))",
				.cra_driver_name = "authenc-hmac-md5-"
						   "cbc-des3_ede-caam",
				.cra_blocksize = DES3_EDE_BLOCK_SIZE,
			},
<<<<<<< HEAD
			.setkey = aead_setkey,
=======
			.setkey = des3_aead_setkey,
>>>>>>> upstream/android-13
			.setauthsize = aead_setauthsize,
			.encrypt = aead_encrypt,
			.decrypt = aead_decrypt,
			.ivsize = DES3_EDE_BLOCK_SIZE,
			.maxauthsize = MD5_DIGEST_SIZE,
		},
		.caam = {
			.class1_alg_type = OP_ALG_ALGSEL_3DES | OP_ALG_AAI_CBC,
			.class2_alg_type = OP_ALG_ALGSEL_MD5 |
					   OP_ALG_AAI_HMAC_PRECOMP,
		}
	},
	{
		.aead = {
			.base = {
				.cra_name = "echainiv(authenc(hmac(md5),"
					    "cbc(des3_ede)))",
				.cra_driver_name = "echainiv-authenc-hmac-md5-"
						   "cbc-des3_ede-caam",
				.cra_blocksize = DES3_EDE_BLOCK_SIZE,
			},
<<<<<<< HEAD
			.setkey = aead_setkey,
=======
			.setkey = des3_aead_setkey,
>>>>>>> upstream/android-13
			.setauthsize = aead_setauthsize,
			.encrypt = aead_encrypt,
			.decrypt = aead_decrypt,
			.ivsize = DES3_EDE_BLOCK_SIZE,
			.maxauthsize = MD5_DIGEST_SIZE,
		},
		.caam = {
			.class1_alg_type = OP_ALG_ALGSEL_3DES | OP_ALG_AAI_CBC,
			.class2_alg_type = OP_ALG_ALGSEL_MD5 |
					   OP_ALG_AAI_HMAC_PRECOMP,
			.geniv = true,
		}
	},
	{
		.aead = {
			.base = {
				.cra_name = "authenc(hmac(sha1),"
					    "cbc(des3_ede))",
				.cra_driver_name = "authenc-hmac-sha1-"
						   "cbc-des3_ede-caam",
				.cra_blocksize = DES3_EDE_BLOCK_SIZE,
			},
<<<<<<< HEAD
			.setkey = aead_setkey,
=======
			.setkey = des3_aead_setkey,
>>>>>>> upstream/android-13
			.setauthsize = aead_setauthsize,
			.encrypt = aead_encrypt,
			.decrypt = aead_decrypt,
			.ivsize = DES3_EDE_BLOCK_SIZE,
			.maxauthsize = SHA1_DIGEST_SIZE,
		},
		.caam = {
			.class1_alg_type = OP_ALG_ALGSEL_3DES | OP_ALG_AAI_CBC,
			.class2_alg_type = OP_ALG_ALGSEL_SHA1 |
					   OP_ALG_AAI_HMAC_PRECOMP,
		},
	},
	{
		.aead = {
			.base = {
				.cra_name = "echainiv(authenc(hmac(sha1),"
					    "cbc(des3_ede)))",
				.cra_driver_name = "echainiv-authenc-"
						   "hmac-sha1-"
						   "cbc-des3_ede-caam",
				.cra_blocksize = DES3_EDE_BLOCK_SIZE,
			},
<<<<<<< HEAD
			.setkey = aead_setkey,
=======
			.setkey = des3_aead_setkey,
>>>>>>> upstream/android-13
			.setauthsize = aead_setauthsize,
			.encrypt = aead_encrypt,
			.decrypt = aead_decrypt,
			.ivsize = DES3_EDE_BLOCK_SIZE,
			.maxauthsize = SHA1_DIGEST_SIZE,
		},
		.caam = {
			.class1_alg_type = OP_ALG_ALGSEL_3DES | OP_ALG_AAI_CBC,
			.class2_alg_type = OP_ALG_ALGSEL_SHA1 |
					   OP_ALG_AAI_HMAC_PRECOMP,
			.geniv = true,
		},
	},
	{
		.aead = {
			.base = {
				.cra_name = "authenc(hmac(sha224),"
					    "cbc(des3_ede))",
				.cra_driver_name = "authenc-hmac-sha224-"
						   "cbc-des3_ede-caam",
				.cra_blocksize = DES3_EDE_BLOCK_SIZE,
			},
<<<<<<< HEAD
			.setkey = aead_setkey,
=======
			.setkey = des3_aead_setkey,
>>>>>>> upstream/android-13
			.setauthsize = aead_setauthsize,
			.encrypt = aead_encrypt,
			.decrypt = aead_decrypt,
			.ivsize = DES3_EDE_BLOCK_SIZE,
			.maxauthsize = SHA224_DIGEST_SIZE,
		},
		.caam = {
			.class1_alg_type = OP_ALG_ALGSEL_3DES | OP_ALG_AAI_CBC,
			.class2_alg_type = OP_ALG_ALGSEL_SHA224 |
					   OP_ALG_AAI_HMAC_PRECOMP,
		},
	},
	{
		.aead = {
			.base = {
				.cra_name = "echainiv(authenc(hmac(sha224),"
					    "cbc(des3_ede)))",
				.cra_driver_name = "echainiv-authenc-"
						   "hmac-sha224-"
						   "cbc-des3_ede-caam",
				.cra_blocksize = DES3_EDE_BLOCK_SIZE,
			},
<<<<<<< HEAD
			.setkey = aead_setkey,
=======
			.setkey = des3_aead_setkey,
>>>>>>> upstream/android-13
			.setauthsize = aead_setauthsize,
			.encrypt = aead_encrypt,
			.decrypt = aead_decrypt,
			.ivsize = DES3_EDE_BLOCK_SIZE,
			.maxauthsize = SHA224_DIGEST_SIZE,
		},
		.caam = {
			.class1_alg_type = OP_ALG_ALGSEL_3DES | OP_ALG_AAI_CBC,
			.class2_alg_type = OP_ALG_ALGSEL_SHA224 |
					   OP_ALG_AAI_HMAC_PRECOMP,
			.geniv = true,
		},
	},
	{
		.aead = {
			.base = {
				.cra_name = "authenc(hmac(sha256),"
					    "cbc(des3_ede))",
				.cra_driver_name = "authenc-hmac-sha256-"
						   "cbc-des3_ede-caam",
				.cra_blocksize = DES3_EDE_BLOCK_SIZE,
			},
<<<<<<< HEAD
			.setkey = aead_setkey,
=======
			.setkey = des3_aead_setkey,
>>>>>>> upstream/android-13
			.setauthsize = aead_setauthsize,
			.encrypt = aead_encrypt,
			.decrypt = aead_decrypt,
			.ivsize = DES3_EDE_BLOCK_SIZE,
			.maxauthsize = SHA256_DIGEST_SIZE,
		},
		.caam = {
			.class1_alg_type = OP_ALG_ALGSEL_3DES | OP_ALG_AAI_CBC,
			.class2_alg_type = OP_ALG_ALGSEL_SHA256 |
					   OP_ALG_AAI_HMAC_PRECOMP,
		},
	},
	{
		.aead = {
			.base = {
				.cra_name = "echainiv(authenc(hmac(sha256),"
					    "cbc(des3_ede)))",
				.cra_driver_name = "echainiv-authenc-"
						   "hmac-sha256-"
						   "cbc-des3_ede-caam",
				.cra_blocksize = DES3_EDE_BLOCK_SIZE,
			},
<<<<<<< HEAD
			.setkey = aead_setkey,
=======
			.setkey = des3_aead_setkey,
>>>>>>> upstream/android-13
			.setauthsize = aead_setauthsize,
			.encrypt = aead_encrypt,
			.decrypt = aead_decrypt,
			.ivsize = DES3_EDE_BLOCK_SIZE,
			.maxauthsize = SHA256_DIGEST_SIZE,
		},
		.caam = {
			.class1_alg_type = OP_ALG_ALGSEL_3DES | OP_ALG_AAI_CBC,
			.class2_alg_type = OP_ALG_ALGSEL_SHA256 |
					   OP_ALG_AAI_HMAC_PRECOMP,
			.geniv = true,
		},
	},
	{
		.aead = {
			.base = {
				.cra_name = "authenc(hmac(sha384),"
					    "cbc(des3_ede))",
				.cra_driver_name = "authenc-hmac-sha384-"
						   "cbc-des3_ede-caam",
				.cra_blocksize = DES3_EDE_BLOCK_SIZE,
			},
<<<<<<< HEAD
			.setkey = aead_setkey,
=======
			.setkey = des3_aead_setkey,
>>>>>>> upstream/android-13
			.setauthsize = aead_setauthsize,
			.encrypt = aead_encrypt,
			.decrypt = aead_decrypt,
			.ivsize = DES3_EDE_BLOCK_SIZE,
			.maxauthsize = SHA384_DIGEST_SIZE,
		},
		.caam = {
			.class1_alg_type = OP_ALG_ALGSEL_3DES | OP_ALG_AAI_CBC,
			.class2_alg_type = OP_ALG_ALGSEL_SHA384 |
					   OP_ALG_AAI_HMAC_PRECOMP,
		},
	},
	{
		.aead = {
			.base = {
				.cra_name = "echainiv(authenc(hmac(sha384),"
					    "cbc(des3_ede)))",
				.cra_driver_name = "echainiv-authenc-"
						   "hmac-sha384-"
						   "cbc-des3_ede-caam",
				.cra_blocksize = DES3_EDE_BLOCK_SIZE,
			},
<<<<<<< HEAD
			.setkey = aead_setkey,
=======
			.setkey = des3_aead_setkey,
>>>>>>> upstream/android-13
			.setauthsize = aead_setauthsize,
			.encrypt = aead_encrypt,
			.decrypt = aead_decrypt,
			.ivsize = DES3_EDE_BLOCK_SIZE,
			.maxauthsize = SHA384_DIGEST_SIZE,
		},
		.caam = {
			.class1_alg_type = OP_ALG_ALGSEL_3DES | OP_ALG_AAI_CBC,
			.class2_alg_type = OP_ALG_ALGSEL_SHA384 |
					   OP_ALG_AAI_HMAC_PRECOMP,
			.geniv = true,
		},
	},
	{
		.aead = {
			.base = {
				.cra_name = "authenc(hmac(sha512),"
					    "cbc(des3_ede))",
				.cra_driver_name = "authenc-hmac-sha512-"
						   "cbc-des3_ede-caam",
				.cra_blocksize = DES3_EDE_BLOCK_SIZE,
			},
<<<<<<< HEAD
			.setkey = aead_setkey,
=======
			.setkey = des3_aead_setkey,
>>>>>>> upstream/android-13
			.setauthsize = aead_setauthsize,
			.encrypt = aead_encrypt,
			.decrypt = aead_decrypt,
			.ivsize = DES3_EDE_BLOCK_SIZE,
			.maxauthsize = SHA512_DIGEST_SIZE,
		},
		.caam = {
			.class1_alg_type = OP_ALG_ALGSEL_3DES | OP_ALG_AAI_CBC,
			.class2_alg_type = OP_ALG_ALGSEL_SHA512 |
					   OP_ALG_AAI_HMAC_PRECOMP,
		},
	},
	{
		.aead = {
			.base = {
				.cra_name = "echainiv(authenc(hmac(sha512),"
					    "cbc(des3_ede)))",
				.cra_driver_name = "echainiv-authenc-"
						   "hmac-sha512-"
						   "cbc-des3_ede-caam",
				.cra_blocksize = DES3_EDE_BLOCK_SIZE,
			},
<<<<<<< HEAD
			.setkey = aead_setkey,
=======
			.setkey = des3_aead_setkey,
>>>>>>> upstream/android-13
			.setauthsize = aead_setauthsize,
			.encrypt = aead_encrypt,
			.decrypt = aead_decrypt,
			.ivsize = DES3_EDE_BLOCK_SIZE,
			.maxauthsize = SHA512_DIGEST_SIZE,
		},
		.caam = {
			.class1_alg_type = OP_ALG_ALGSEL_3DES | OP_ALG_AAI_CBC,
			.class2_alg_type = OP_ALG_ALGSEL_SHA512 |
					   OP_ALG_AAI_HMAC_PRECOMP,
			.geniv = true,
		},
	},
	{
		.aead = {
			.base = {
				.cra_name = "authenc(hmac(md5),cbc(des))",
				.cra_driver_name = "authenc-hmac-md5-"
						   "cbc-des-caam",
				.cra_blocksize = DES_BLOCK_SIZE,
			},
			.setkey = aead_setkey,
			.setauthsize = aead_setauthsize,
			.encrypt = aead_encrypt,
			.decrypt = aead_decrypt,
			.ivsize = DES_BLOCK_SIZE,
			.maxauthsize = MD5_DIGEST_SIZE,
		},
		.caam = {
			.class1_alg_type = OP_ALG_ALGSEL_DES | OP_ALG_AAI_CBC,
			.class2_alg_type = OP_ALG_ALGSEL_MD5 |
					   OP_ALG_AAI_HMAC_PRECOMP,
		},
	},
	{
		.aead = {
			.base = {
				.cra_name = "echainiv(authenc(hmac(md5),"
					    "cbc(des)))",
				.cra_driver_name = "echainiv-authenc-hmac-md5-"
						   "cbc-des-caam",
				.cra_blocksize = DES_BLOCK_SIZE,
			},
			.setkey = aead_setkey,
			.setauthsize = aead_setauthsize,
			.encrypt = aead_encrypt,
			.decrypt = aead_decrypt,
			.ivsize = DES_BLOCK_SIZE,
			.maxauthsize = MD5_DIGEST_SIZE,
		},
		.caam = {
			.class1_alg_type = OP_ALG_ALGSEL_DES | OP_ALG_AAI_CBC,
			.class2_alg_type = OP_ALG_ALGSEL_MD5 |
					   OP_ALG_AAI_HMAC_PRECOMP,
			.geniv = true,
		},
	},
	{
		.aead = {
			.base = {
				.cra_name = "authenc(hmac(sha1),cbc(des))",
				.cra_driver_name = "authenc-hmac-sha1-"
						   "cbc-des-caam",
				.cra_blocksize = DES_BLOCK_SIZE,
			},
			.setkey = aead_setkey,
			.setauthsize = aead_setauthsize,
			.encrypt = aead_encrypt,
			.decrypt = aead_decrypt,
			.ivsize = DES_BLOCK_SIZE,
			.maxauthsize = SHA1_DIGEST_SIZE,
		},
		.caam = {
			.class1_alg_type = OP_ALG_ALGSEL_DES | OP_ALG_AAI_CBC,
			.class2_alg_type = OP_ALG_ALGSEL_SHA1 |
					   OP_ALG_AAI_HMAC_PRECOMP,
		},
	},
	{
		.aead = {
			.base = {
				.cra_name = "echainiv(authenc(hmac(sha1),"
					    "cbc(des)))",
				.cra_driver_name = "echainiv-authenc-"
						   "hmac-sha1-cbc-des-caam",
				.cra_blocksize = DES_BLOCK_SIZE,
			},
			.setkey = aead_setkey,
			.setauthsize = aead_setauthsize,
			.encrypt = aead_encrypt,
			.decrypt = aead_decrypt,
			.ivsize = DES_BLOCK_SIZE,
			.maxauthsize = SHA1_DIGEST_SIZE,
		},
		.caam = {
			.class1_alg_type = OP_ALG_ALGSEL_DES | OP_ALG_AAI_CBC,
			.class2_alg_type = OP_ALG_ALGSEL_SHA1 |
					   OP_ALG_AAI_HMAC_PRECOMP,
			.geniv = true,
		},
	},
	{
		.aead = {
			.base = {
				.cra_name = "authenc(hmac(sha224),cbc(des))",
				.cra_driver_name = "authenc-hmac-sha224-"
						   "cbc-des-caam",
				.cra_blocksize = DES_BLOCK_SIZE,
			},
			.setkey = aead_setkey,
			.setauthsize = aead_setauthsize,
			.encrypt = aead_encrypt,
			.decrypt = aead_decrypt,
			.ivsize = DES_BLOCK_SIZE,
			.maxauthsize = SHA224_DIGEST_SIZE,
		},
		.caam = {
			.class1_alg_type = OP_ALG_ALGSEL_DES | OP_ALG_AAI_CBC,
			.class2_alg_type = OP_ALG_ALGSEL_SHA224 |
					   OP_ALG_AAI_HMAC_PRECOMP,
		},
	},
	{
		.aead = {
			.base = {
				.cra_name = "echainiv(authenc(hmac(sha224),"
					    "cbc(des)))",
				.cra_driver_name = "echainiv-authenc-"
						   "hmac-sha224-cbc-des-caam",
				.cra_blocksize = DES_BLOCK_SIZE,
			},
			.setkey = aead_setkey,
			.setauthsize = aead_setauthsize,
			.encrypt = aead_encrypt,
			.decrypt = aead_decrypt,
			.ivsize = DES_BLOCK_SIZE,
			.maxauthsize = SHA224_DIGEST_SIZE,
		},
		.caam = {
			.class1_alg_type = OP_ALG_ALGSEL_DES | OP_ALG_AAI_CBC,
			.class2_alg_type = OP_ALG_ALGSEL_SHA224 |
					   OP_ALG_AAI_HMAC_PRECOMP,
			.geniv = true,
		},
	},
	{
		.aead = {
			.base = {
				.cra_name = "authenc(hmac(sha256),cbc(des))",
				.cra_driver_name = "authenc-hmac-sha256-"
						   "cbc-des-caam",
				.cra_blocksize = DES_BLOCK_SIZE,
			},
			.setkey = aead_setkey,
			.setauthsize = aead_setauthsize,
			.encrypt = aead_encrypt,
			.decrypt = aead_decrypt,
			.ivsize = DES_BLOCK_SIZE,
			.maxauthsize = SHA256_DIGEST_SIZE,
		},
		.caam = {
			.class1_alg_type = OP_ALG_ALGSEL_DES | OP_ALG_AAI_CBC,
			.class2_alg_type = OP_ALG_ALGSEL_SHA256 |
					   OP_ALG_AAI_HMAC_PRECOMP,
		},
	},
	{
		.aead = {
			.base = {
				.cra_name = "echainiv(authenc(hmac(sha256),"
					    "cbc(des)))",
				.cra_driver_name = "echainiv-authenc-"
						   "hmac-sha256-cbc-des-caam",
				.cra_blocksize = DES_BLOCK_SIZE,
			},
			.setkey = aead_setkey,
			.setauthsize = aead_setauthsize,
			.encrypt = aead_encrypt,
			.decrypt = aead_decrypt,
			.ivsize = DES_BLOCK_SIZE,
			.maxauthsize = SHA256_DIGEST_SIZE,
		},
		.caam = {
			.class1_alg_type = OP_ALG_ALGSEL_DES | OP_ALG_AAI_CBC,
			.class2_alg_type = OP_ALG_ALGSEL_SHA256 |
					   OP_ALG_AAI_HMAC_PRECOMP,
			.geniv = true,
		},
	},
	{
		.aead = {
			.base = {
				.cra_name = "authenc(hmac(sha384),cbc(des))",
				.cra_driver_name = "authenc-hmac-sha384-"
						   "cbc-des-caam",
				.cra_blocksize = DES_BLOCK_SIZE,
			},
			.setkey = aead_setkey,
			.setauthsize = aead_setauthsize,
			.encrypt = aead_encrypt,
			.decrypt = aead_decrypt,
			.ivsize = DES_BLOCK_SIZE,
			.maxauthsize = SHA384_DIGEST_SIZE,
		},
		.caam = {
			.class1_alg_type = OP_ALG_ALGSEL_DES | OP_ALG_AAI_CBC,
			.class2_alg_type = OP_ALG_ALGSEL_SHA384 |
					   OP_ALG_AAI_HMAC_PRECOMP,
		},
	},
	{
		.aead = {
			.base = {
				.cra_name = "echainiv(authenc(hmac(sha384),"
					    "cbc(des)))",
				.cra_driver_name = "echainiv-authenc-"
						   "hmac-sha384-cbc-des-caam",
				.cra_blocksize = DES_BLOCK_SIZE,
			},
			.setkey = aead_setkey,
			.setauthsize = aead_setauthsize,
			.encrypt = aead_encrypt,
			.decrypt = aead_decrypt,
			.ivsize = DES_BLOCK_SIZE,
			.maxauthsize = SHA384_DIGEST_SIZE,
		},
		.caam = {
			.class1_alg_type = OP_ALG_ALGSEL_DES | OP_ALG_AAI_CBC,
			.class2_alg_type = OP_ALG_ALGSEL_SHA384 |
					   OP_ALG_AAI_HMAC_PRECOMP,
			.geniv = true,
		},
	},
	{
		.aead = {
			.base = {
				.cra_name = "authenc(hmac(sha512),cbc(des))",
				.cra_driver_name = "authenc-hmac-sha512-"
						   "cbc-des-caam",
				.cra_blocksize = DES_BLOCK_SIZE,
			},
			.setkey = aead_setkey,
			.setauthsize = aead_setauthsize,
			.encrypt = aead_encrypt,
			.decrypt = aead_decrypt,
			.ivsize = DES_BLOCK_SIZE,
			.maxauthsize = SHA512_DIGEST_SIZE,
		},
		.caam = {
			.class1_alg_type = OP_ALG_ALGSEL_DES | OP_ALG_AAI_CBC,
			.class2_alg_type = OP_ALG_ALGSEL_SHA512 |
					   OP_ALG_AAI_HMAC_PRECOMP,
		},
	},
	{
		.aead = {
			.base = {
				.cra_name = "echainiv(authenc(hmac(sha512),"
					    "cbc(des)))",
				.cra_driver_name = "echainiv-authenc-"
						   "hmac-sha512-cbc-des-caam",
				.cra_blocksize = DES_BLOCK_SIZE,
			},
			.setkey = aead_setkey,
			.setauthsize = aead_setauthsize,
			.encrypt = aead_encrypt,
			.decrypt = aead_decrypt,
			.ivsize = DES_BLOCK_SIZE,
			.maxauthsize = SHA512_DIGEST_SIZE,
		},
		.caam = {
			.class1_alg_type = OP_ALG_ALGSEL_DES | OP_ALG_AAI_CBC,
			.class2_alg_type = OP_ALG_ALGSEL_SHA512 |
					   OP_ALG_AAI_HMAC_PRECOMP,
			.geniv = true,
		},
	},
	{
		.aead = {
			.base = {
				.cra_name = "authenc(hmac(md5),"
					    "rfc3686(ctr(aes)))",
				.cra_driver_name = "authenc-hmac-md5-"
						   "rfc3686-ctr-aes-caam",
				.cra_blocksize = 1,
			},
			.setkey = aead_setkey,
			.setauthsize = aead_setauthsize,
			.encrypt = aead_encrypt,
			.decrypt = aead_decrypt,
			.ivsize = CTR_RFC3686_IV_SIZE,
			.maxauthsize = MD5_DIGEST_SIZE,
		},
		.caam = {
			.class1_alg_type = OP_ALG_ALGSEL_AES |
					   OP_ALG_AAI_CTR_MOD128,
			.class2_alg_type = OP_ALG_ALGSEL_MD5 |
					   OP_ALG_AAI_HMAC_PRECOMP,
			.rfc3686 = true,
		},
	},
	{
		.aead = {
			.base = {
				.cra_name = "seqiv(authenc("
					    "hmac(md5),rfc3686(ctr(aes))))",
				.cra_driver_name = "seqiv-authenc-hmac-md5-"
						   "rfc3686-ctr-aes-caam",
				.cra_blocksize = 1,
			},
			.setkey = aead_setkey,
			.setauthsize = aead_setauthsize,
			.encrypt = aead_encrypt,
			.decrypt = aead_decrypt,
			.ivsize = CTR_RFC3686_IV_SIZE,
			.maxauthsize = MD5_DIGEST_SIZE,
		},
		.caam = {
			.class1_alg_type = OP_ALG_ALGSEL_AES |
					   OP_ALG_AAI_CTR_MOD128,
			.class2_alg_type = OP_ALG_ALGSEL_MD5 |
					   OP_ALG_AAI_HMAC_PRECOMP,
			.rfc3686 = true,
			.geniv = true,
		},
	},
	{
		.aead = {
			.base = {
				.cra_name = "authenc(hmac(sha1),"
					    "rfc3686(ctr(aes)))",
				.cra_driver_name = "authenc-hmac-sha1-"
						   "rfc3686-ctr-aes-caam",
				.cra_blocksize = 1,
			},
			.setkey = aead_setkey,
			.setauthsize = aead_setauthsize,
			.encrypt = aead_encrypt,
			.decrypt = aead_decrypt,
			.ivsize = CTR_RFC3686_IV_SIZE,
			.maxauthsize = SHA1_DIGEST_SIZE,
		},
		.caam = {
			.class1_alg_type = OP_ALG_ALGSEL_AES |
					   OP_ALG_AAI_CTR_MOD128,
			.class2_alg_type = OP_ALG_ALGSEL_SHA1 |
					   OP_ALG_AAI_HMAC_PRECOMP,
			.rfc3686 = true,
		},
	},
	{
		.aead = {
			.base = {
				.cra_name = "seqiv(authenc("
					    "hmac(sha1),rfc3686(ctr(aes))))",
				.cra_driver_name = "seqiv-authenc-hmac-sha1-"
						   "rfc3686-ctr-aes-caam",
				.cra_blocksize = 1,
			},
			.setkey = aead_setkey,
			.setauthsize = aead_setauthsize,
			.encrypt = aead_encrypt,
			.decrypt = aead_decrypt,
			.ivsize = CTR_RFC3686_IV_SIZE,
			.maxauthsize = SHA1_DIGEST_SIZE,
		},
		.caam = {
			.class1_alg_type = OP_ALG_ALGSEL_AES |
					   OP_ALG_AAI_CTR_MOD128,
			.class2_alg_type = OP_ALG_ALGSEL_SHA1 |
					   OP_ALG_AAI_HMAC_PRECOMP,
			.rfc3686 = true,
			.geniv = true,
		},
	},
	{
		.aead = {
			.base = {
				.cra_name = "authenc(hmac(sha224),"
					    "rfc3686(ctr(aes)))",
				.cra_driver_name = "authenc-hmac-sha224-"
						   "rfc3686-ctr-aes-caam",
				.cra_blocksize = 1,
			},
			.setkey = aead_setkey,
			.setauthsize = aead_setauthsize,
			.encrypt = aead_encrypt,
			.decrypt = aead_decrypt,
			.ivsize = CTR_RFC3686_IV_SIZE,
			.maxauthsize = SHA224_DIGEST_SIZE,
		},
		.caam = {
			.class1_alg_type = OP_ALG_ALGSEL_AES |
					   OP_ALG_AAI_CTR_MOD128,
			.class2_alg_type = OP_ALG_ALGSEL_SHA224 |
					   OP_ALG_AAI_HMAC_PRECOMP,
			.rfc3686 = true,
		},
	},
	{
		.aead = {
			.base = {
				.cra_name = "seqiv(authenc("
					    "hmac(sha224),rfc3686(ctr(aes))))",
				.cra_driver_name = "seqiv-authenc-hmac-sha224-"
						   "rfc3686-ctr-aes-caam",
				.cra_blocksize = 1,
			},
			.setkey = aead_setkey,
			.setauthsize = aead_setauthsize,
			.encrypt = aead_encrypt,
			.decrypt = aead_decrypt,
			.ivsize = CTR_RFC3686_IV_SIZE,
			.maxauthsize = SHA224_DIGEST_SIZE,
		},
		.caam = {
			.class1_alg_type = OP_ALG_ALGSEL_AES |
					   OP_ALG_AAI_CTR_MOD128,
			.class2_alg_type = OP_ALG_ALGSEL_SHA224 |
					   OP_ALG_AAI_HMAC_PRECOMP,
			.rfc3686 = true,
			.geniv = true,
		},
	},
	{
		.aead = {
			.base = {
				.cra_name = "authenc(hmac(sha256),"
					    "rfc3686(ctr(aes)))",
				.cra_driver_name = "authenc-hmac-sha256-"
						   "rfc3686-ctr-aes-caam",
				.cra_blocksize = 1,
			},
			.setkey = aead_setkey,
			.setauthsize = aead_setauthsize,
			.encrypt = aead_encrypt,
			.decrypt = aead_decrypt,
			.ivsize = CTR_RFC3686_IV_SIZE,
			.maxauthsize = SHA256_DIGEST_SIZE,
		},
		.caam = {
			.class1_alg_type = OP_ALG_ALGSEL_AES |
					   OP_ALG_AAI_CTR_MOD128,
			.class2_alg_type = OP_ALG_ALGSEL_SHA256 |
					   OP_ALG_AAI_HMAC_PRECOMP,
			.rfc3686 = true,
		},
	},
	{
		.aead = {
			.base = {
				.cra_name = "seqiv(authenc(hmac(sha256),"
					    "rfc3686(ctr(aes))))",
				.cra_driver_name = "seqiv-authenc-hmac-sha256-"
						   "rfc3686-ctr-aes-caam",
				.cra_blocksize = 1,
			},
			.setkey = aead_setkey,
			.setauthsize = aead_setauthsize,
			.encrypt = aead_encrypt,
			.decrypt = aead_decrypt,
			.ivsize = CTR_RFC3686_IV_SIZE,
			.maxauthsize = SHA256_DIGEST_SIZE,
		},
		.caam = {
			.class1_alg_type = OP_ALG_ALGSEL_AES |
					   OP_ALG_AAI_CTR_MOD128,
			.class2_alg_type = OP_ALG_ALGSEL_SHA256 |
					   OP_ALG_AAI_HMAC_PRECOMP,
			.rfc3686 = true,
			.geniv = true,
		},
	},
	{
		.aead = {
			.base = {
				.cra_name = "authenc(hmac(sha384),"
					    "rfc3686(ctr(aes)))",
				.cra_driver_name = "authenc-hmac-sha384-"
						   "rfc3686-ctr-aes-caam",
				.cra_blocksize = 1,
			},
			.setkey = aead_setkey,
			.setauthsize = aead_setauthsize,
			.encrypt = aead_encrypt,
			.decrypt = aead_decrypt,
			.ivsize = CTR_RFC3686_IV_SIZE,
			.maxauthsize = SHA384_DIGEST_SIZE,
		},
		.caam = {
			.class1_alg_type = OP_ALG_ALGSEL_AES |
					   OP_ALG_AAI_CTR_MOD128,
			.class2_alg_type = OP_ALG_ALGSEL_SHA384 |
					   OP_ALG_AAI_HMAC_PRECOMP,
			.rfc3686 = true,
		},
	},
	{
		.aead = {
			.base = {
				.cra_name = "seqiv(authenc(hmac(sha384),"
					    "rfc3686(ctr(aes))))",
				.cra_driver_name = "seqiv-authenc-hmac-sha384-"
						   "rfc3686-ctr-aes-caam",
				.cra_blocksize = 1,
			},
			.setkey = aead_setkey,
			.setauthsize = aead_setauthsize,
			.encrypt = aead_encrypt,
			.decrypt = aead_decrypt,
			.ivsize = CTR_RFC3686_IV_SIZE,
			.maxauthsize = SHA384_DIGEST_SIZE,
		},
		.caam = {
			.class1_alg_type = OP_ALG_ALGSEL_AES |
					   OP_ALG_AAI_CTR_MOD128,
			.class2_alg_type = OP_ALG_ALGSEL_SHA384 |
					   OP_ALG_AAI_HMAC_PRECOMP,
			.rfc3686 = true,
			.geniv = true,
		},
	},
	{
		.aead = {
			.base = {
				.cra_name = "authenc(hmac(sha512),"
					    "rfc3686(ctr(aes)))",
				.cra_driver_name = "authenc-hmac-sha512-"
						   "rfc3686-ctr-aes-caam",
				.cra_blocksize = 1,
			},
			.setkey = aead_setkey,
			.setauthsize = aead_setauthsize,
			.encrypt = aead_encrypt,
			.decrypt = aead_decrypt,
			.ivsize = CTR_RFC3686_IV_SIZE,
			.maxauthsize = SHA512_DIGEST_SIZE,
		},
		.caam = {
			.class1_alg_type = OP_ALG_ALGSEL_AES |
					   OP_ALG_AAI_CTR_MOD128,
			.class2_alg_type = OP_ALG_ALGSEL_SHA512 |
					   OP_ALG_AAI_HMAC_PRECOMP,
			.rfc3686 = true,
		},
	},
	{
		.aead = {
			.base = {
				.cra_name = "seqiv(authenc(hmac(sha512),"
					    "rfc3686(ctr(aes))))",
				.cra_driver_name = "seqiv-authenc-hmac-sha512-"
						   "rfc3686-ctr-aes-caam",
				.cra_blocksize = 1,
			},
			.setkey = aead_setkey,
			.setauthsize = aead_setauthsize,
			.encrypt = aead_encrypt,
			.decrypt = aead_decrypt,
			.ivsize = CTR_RFC3686_IV_SIZE,
			.maxauthsize = SHA512_DIGEST_SIZE,
		},
		.caam = {
			.class1_alg_type = OP_ALG_ALGSEL_AES |
					   OP_ALG_AAI_CTR_MOD128,
			.class2_alg_type = OP_ALG_ALGSEL_SHA512 |
					   OP_ALG_AAI_HMAC_PRECOMP,
			.rfc3686 = true,
			.geniv = true,
		},
	},
<<<<<<< HEAD
};

struct caam_crypto_alg {
	struct crypto_alg crypto_alg;
	struct list_head entry;
	struct caam_alg_entry caam;
=======
	{
		.aead = {
			.base = {
				.cra_name = "rfc7539(chacha20,poly1305)",
				.cra_driver_name = "rfc7539-chacha20-poly1305-"
						   "caam",
				.cra_blocksize = 1,
			},
			.setkey = chachapoly_setkey,
			.setauthsize = chachapoly_setauthsize,
			.encrypt = chachapoly_encrypt,
			.decrypt = chachapoly_decrypt,
			.ivsize = CHACHAPOLY_IV_SIZE,
			.maxauthsize = POLY1305_DIGEST_SIZE,
		},
		.caam = {
			.class1_alg_type = OP_ALG_ALGSEL_CHACHA20 |
					   OP_ALG_AAI_AEAD,
			.class2_alg_type = OP_ALG_ALGSEL_POLY1305 |
					   OP_ALG_AAI_AEAD,
			.nodkp = true,
		},
	},
	{
		.aead = {
			.base = {
				.cra_name = "rfc7539esp(chacha20,poly1305)",
				.cra_driver_name = "rfc7539esp-chacha20-"
						   "poly1305-caam",
				.cra_blocksize = 1,
			},
			.setkey = chachapoly_setkey,
			.setauthsize = chachapoly_setauthsize,
			.encrypt = chachapoly_encrypt,
			.decrypt = chachapoly_decrypt,
			.ivsize = 8,
			.maxauthsize = POLY1305_DIGEST_SIZE,
		},
		.caam = {
			.class1_alg_type = OP_ALG_ALGSEL_CHACHA20 |
					   OP_ALG_AAI_AEAD,
			.class2_alg_type = OP_ALG_ALGSEL_POLY1305 |
					   OP_ALG_AAI_AEAD,
			.nodkp = true,
		},
	},
>>>>>>> upstream/android-13
};

static int caam_init_common(struct caam_ctx *ctx, struct caam_alg_entry *caam,
			    bool uses_dkp)
{
	dma_addr_t dma_addr;
	struct caam_drv_private *priv;
<<<<<<< HEAD
=======
	const size_t sh_desc_enc_offset = offsetof(struct caam_ctx,
						   sh_desc_enc);
>>>>>>> upstream/android-13

	ctx->jrdev = caam_jr_alloc();
	if (IS_ERR(ctx->jrdev)) {
		pr_err("Job Ring Device allocation for transform failed\n");
		return PTR_ERR(ctx->jrdev);
	}

	priv = dev_get_drvdata(ctx->jrdev->parent);
	if (priv->era >= 6 && uses_dkp)
		ctx->dir = DMA_BIDIRECTIONAL;
	else
		ctx->dir = DMA_TO_DEVICE;

	dma_addr = dma_map_single_attrs(ctx->jrdev, ctx->sh_desc_enc,
					offsetof(struct caam_ctx,
<<<<<<< HEAD
						 sh_desc_enc_dma),
=======
						 sh_desc_enc_dma) -
					sh_desc_enc_offset,
>>>>>>> upstream/android-13
					ctx->dir, DMA_ATTR_SKIP_CPU_SYNC);
	if (dma_mapping_error(ctx->jrdev, dma_addr)) {
		dev_err(ctx->jrdev, "unable to map key, shared descriptors\n");
		caam_jr_free(ctx->jrdev);
		return -ENOMEM;
	}

	ctx->sh_desc_enc_dma = dma_addr;
	ctx->sh_desc_dec_dma = dma_addr + offsetof(struct caam_ctx,
<<<<<<< HEAD
						   sh_desc_dec);
	ctx->sh_desc_givenc_dma = dma_addr + offsetof(struct caam_ctx,
						      sh_desc_givenc);
	ctx->key_dma = dma_addr + offsetof(struct caam_ctx, key);
=======
						   sh_desc_dec) -
					sh_desc_enc_offset;
	ctx->key_dma = dma_addr + offsetof(struct caam_ctx, key) -
					sh_desc_enc_offset;
>>>>>>> upstream/android-13

	/* copy descriptor header template value */
	ctx->cdata.algtype = OP_TYPE_CLASS1_ALG | caam->class1_alg_type;
	ctx->adata.algtype = OP_TYPE_CLASS2_ALG | caam->class2_alg_type;

	return 0;
}

<<<<<<< HEAD
static int caam_cra_init(struct crypto_tfm *tfm)
{
	struct crypto_alg *alg = tfm->__crt_alg;
	struct caam_crypto_alg *caam_alg =
		 container_of(alg, struct caam_crypto_alg, crypto_alg);
	struct caam_ctx *ctx = crypto_tfm_ctx(tfm);

	return caam_init_common(ctx, &caam_alg->caam, false);
=======
static int caam_cra_init(struct crypto_skcipher *tfm)
{
	struct skcipher_alg *alg = crypto_skcipher_alg(tfm);
	struct caam_skcipher_alg *caam_alg =
		container_of(alg, typeof(*caam_alg), skcipher);
	struct caam_ctx *ctx = crypto_skcipher_ctx(tfm);
	u32 alg_aai = caam_alg->caam.class1_alg_type & OP_ALG_AAI_MASK;
	int ret = 0;

	ctx->enginectx.op.do_one_request = skcipher_do_one_req;

	if (alg_aai == OP_ALG_AAI_XTS) {
		const char *tfm_name = crypto_tfm_alg_name(&tfm->base);
		struct crypto_skcipher *fallback;

		fallback = crypto_alloc_skcipher(tfm_name, 0,
						 CRYPTO_ALG_NEED_FALLBACK);
		if (IS_ERR(fallback)) {
			pr_err("Failed to allocate %s fallback: %ld\n",
			       tfm_name, PTR_ERR(fallback));
			return PTR_ERR(fallback);
		}

		ctx->fallback = fallback;
		crypto_skcipher_set_reqsize(tfm, sizeof(struct caam_skcipher_req_ctx) +
					    crypto_skcipher_reqsize(fallback));
	} else {
		crypto_skcipher_set_reqsize(tfm, sizeof(struct caam_skcipher_req_ctx));
	}

	ret = caam_init_common(ctx, &caam_alg->caam, false);
	if (ret && ctx->fallback)
		crypto_free_skcipher(ctx->fallback);

	return ret;
>>>>>>> upstream/android-13
}

static int caam_aead_init(struct crypto_aead *tfm)
{
	struct aead_alg *alg = crypto_aead_alg(tfm);
	struct caam_aead_alg *caam_alg =
		 container_of(alg, struct caam_aead_alg, aead);
	struct caam_ctx *ctx = crypto_aead_ctx(tfm);

<<<<<<< HEAD
	return caam_init_common(ctx, &caam_alg->caam,
				alg->setkey == aead_setkey);
=======
	crypto_aead_set_reqsize(tfm, sizeof(struct caam_aead_req_ctx));

	ctx->enginectx.op.do_one_request = aead_do_one_req;

	return caam_init_common(ctx, &caam_alg->caam, !caam_alg->caam.nodkp);
>>>>>>> upstream/android-13
}

static void caam_exit_common(struct caam_ctx *ctx)
{
	dma_unmap_single_attrs(ctx->jrdev, ctx->sh_desc_enc_dma,
<<<<<<< HEAD
			       offsetof(struct caam_ctx, sh_desc_enc_dma),
=======
			       offsetof(struct caam_ctx, sh_desc_enc_dma) -
			       offsetof(struct caam_ctx, sh_desc_enc),
>>>>>>> upstream/android-13
			       ctx->dir, DMA_ATTR_SKIP_CPU_SYNC);
	caam_jr_free(ctx->jrdev);
}

<<<<<<< HEAD
static void caam_cra_exit(struct crypto_tfm *tfm)
{
	caam_exit_common(crypto_tfm_ctx(tfm));
=======
static void caam_cra_exit(struct crypto_skcipher *tfm)
{
	struct caam_ctx *ctx = crypto_skcipher_ctx(tfm);

	if (ctx->fallback)
		crypto_free_skcipher(ctx->fallback);
	caam_exit_common(ctx);
>>>>>>> upstream/android-13
}

static void caam_aead_exit(struct crypto_aead *tfm)
{
	caam_exit_common(crypto_aead_ctx(tfm));
}

<<<<<<< HEAD
static void __exit caam_algapi_exit(void)
{

	struct caam_crypto_alg *t_alg, *n;
=======
void caam_algapi_exit(void)
{
>>>>>>> upstream/android-13
	int i;

	for (i = 0; i < ARRAY_SIZE(driver_aeads); i++) {
		struct caam_aead_alg *t_alg = driver_aeads + i;

		if (t_alg->registered)
			crypto_unregister_aead(&t_alg->aead);
	}

<<<<<<< HEAD
	if (!alg_list.next)
		return;

	list_for_each_entry_safe(t_alg, n, &alg_list, entry) {
		crypto_unregister_alg(&t_alg->crypto_alg);
		list_del(&t_alg->entry);
		kfree(t_alg);
	}
}

static struct caam_crypto_alg *caam_alg_alloc(struct caam_alg_template
					      *template)
{
	struct caam_crypto_alg *t_alg;
	struct crypto_alg *alg;

	t_alg = kzalloc(sizeof(*t_alg), GFP_KERNEL);
	if (!t_alg) {
		pr_err("failed to allocate t_alg\n");
		return ERR_PTR(-ENOMEM);
	}

	alg = &t_alg->crypto_alg;

	snprintf(alg->cra_name, CRYPTO_MAX_ALG_NAME, "%s", template->name);
	snprintf(alg->cra_driver_name, CRYPTO_MAX_ALG_NAME, "%s",
		 template->driver_name);
	alg->cra_module = THIS_MODULE;
	alg->cra_init = caam_cra_init;
	alg->cra_exit = caam_cra_exit;
	alg->cra_priority = CAAM_CRA_PRIORITY;
	alg->cra_blocksize = template->blocksize;
	alg->cra_alignmask = 0;
	alg->cra_ctxsize = sizeof(struct caam_ctx);
	alg->cra_flags = CRYPTO_ALG_ASYNC | CRYPTO_ALG_KERN_DRIVER_ONLY |
			 template->type;
	switch (template->type) {
	case CRYPTO_ALG_TYPE_GIVCIPHER:
		alg->cra_type = &crypto_givcipher_type;
		alg->cra_ablkcipher = template->template_ablkcipher;
		break;
	case CRYPTO_ALG_TYPE_ABLKCIPHER:
		alg->cra_type = &crypto_ablkcipher_type;
		alg->cra_ablkcipher = template->template_ablkcipher;
		break;
	}

	t_alg->caam.class1_alg_type = template->class1_alg_type;
	t_alg->caam.class2_alg_type = template->class2_alg_type;

	return t_alg;
=======
	for (i = 0; i < ARRAY_SIZE(driver_algs); i++) {
		struct caam_skcipher_alg *t_alg = driver_algs + i;

		if (t_alg->registered)
			crypto_unregister_skcipher(&t_alg->skcipher);
	}
}

static void caam_skcipher_alg_init(struct caam_skcipher_alg *t_alg)
{
	struct skcipher_alg *alg = &t_alg->skcipher;

	alg->base.cra_module = THIS_MODULE;
	alg->base.cra_priority = CAAM_CRA_PRIORITY;
	alg->base.cra_ctxsize = sizeof(struct caam_ctx);
	alg->base.cra_flags |= (CRYPTO_ALG_ASYNC | CRYPTO_ALG_ALLOCATES_MEMORY |
			      CRYPTO_ALG_KERN_DRIVER_ONLY);

	alg->init = caam_cra_init;
	alg->exit = caam_cra_exit;
>>>>>>> upstream/android-13
}

static void caam_aead_alg_init(struct caam_aead_alg *t_alg)
{
	struct aead_alg *alg = &t_alg->aead;

	alg->base.cra_module = THIS_MODULE;
	alg->base.cra_priority = CAAM_CRA_PRIORITY;
	alg->base.cra_ctxsize = sizeof(struct caam_ctx);
<<<<<<< HEAD
	alg->base.cra_flags = CRYPTO_ALG_ASYNC | CRYPTO_ALG_KERN_DRIVER_ONLY;
=======
	alg->base.cra_flags = CRYPTO_ALG_ASYNC | CRYPTO_ALG_ALLOCATES_MEMORY |
			      CRYPTO_ALG_KERN_DRIVER_ONLY;
>>>>>>> upstream/android-13

	alg->init = caam_aead_init;
	alg->exit = caam_aead_exit;
}

<<<<<<< HEAD
static int __init caam_algapi_init(void)
{
	struct device_node *dev_node;
	struct platform_device *pdev;
	struct device *ctrldev;
	struct caam_drv_private *priv;
	int i = 0, err = 0;
	u32 cha_vid, cha_inst, des_inst, aes_inst, md_inst;
	unsigned int md_limit = SHA512_DIGEST_SIZE;
	bool registered = false;

	dev_node = of_find_compatible_node(NULL, NULL, "fsl,sec-v4.0");
	if (!dev_node) {
		dev_node = of_find_compatible_node(NULL, NULL, "fsl,sec4.0");
		if (!dev_node)
			return -ENODEV;
	}

	pdev = of_find_device_by_node(dev_node);
	if (!pdev) {
		of_node_put(dev_node);
		return -ENODEV;
	}

	ctrldev = &pdev->dev;
	priv = dev_get_drvdata(ctrldev);
	of_node_put(dev_node);

	/*
	 * If priv is NULL, it's probably because the caam driver wasn't
	 * properly initialized (e.g. RNG4 init failed). Thus, bail out here.
	 */
	if (!priv)
		return -ENODEV;


	INIT_LIST_HEAD(&alg_list);
=======
int caam_algapi_init(struct device *ctrldev)
{
	struct caam_drv_private *priv = dev_get_drvdata(ctrldev);
	int i = 0, err = 0;
	u32 aes_vid, aes_inst, des_inst, md_vid, md_inst, ccha_inst, ptha_inst;
	unsigned int md_limit = SHA512_DIGEST_SIZE;
	bool registered = false, gcm_support;
>>>>>>> upstream/android-13

	/*
	 * Register crypto algorithms the device supports.
	 * First, detect presence and attributes of DES, AES, and MD blocks.
	 */
<<<<<<< HEAD
	cha_vid = rd_reg32(&priv->ctrl->perfmon.cha_id_ls);
	cha_inst = rd_reg32(&priv->ctrl->perfmon.cha_num_ls);
	des_inst = (cha_inst & CHA_ID_LS_DES_MASK) >> CHA_ID_LS_DES_SHIFT;
	aes_inst = (cha_inst & CHA_ID_LS_AES_MASK) >> CHA_ID_LS_AES_SHIFT;
	md_inst = (cha_inst & CHA_ID_LS_MD_MASK) >> CHA_ID_LS_MD_SHIFT;

	/* If MD is present, limit digest size based on LP256 */
	if (md_inst && ((cha_vid & CHA_ID_LS_MD_MASK) == CHA_ID_LS_MD_LP256))
		md_limit = SHA256_DIGEST_SIZE;

	for (i = 0; i < ARRAY_SIZE(driver_algs); i++) {
		struct caam_crypto_alg *t_alg;
		struct caam_alg_template *alg = driver_algs + i;
		u32 alg_sel = alg->class1_alg_type & OP_ALG_ALGSEL_MASK;
=======
	if (priv->era < 10) {
		u32 cha_vid, cha_inst, aes_rn;

		cha_vid = rd_reg32(&priv->ctrl->perfmon.cha_id_ls);
		aes_vid = cha_vid & CHA_ID_LS_AES_MASK;
		md_vid = (cha_vid & CHA_ID_LS_MD_MASK) >> CHA_ID_LS_MD_SHIFT;

		cha_inst = rd_reg32(&priv->ctrl->perfmon.cha_num_ls);
		des_inst = (cha_inst & CHA_ID_LS_DES_MASK) >>
			   CHA_ID_LS_DES_SHIFT;
		aes_inst = cha_inst & CHA_ID_LS_AES_MASK;
		md_inst = (cha_inst & CHA_ID_LS_MD_MASK) >> CHA_ID_LS_MD_SHIFT;
		ccha_inst = 0;
		ptha_inst = 0;

		aes_rn = rd_reg32(&priv->ctrl->perfmon.cha_rev_ls) &
			 CHA_ID_LS_AES_MASK;
		gcm_support = !(aes_vid == CHA_VER_VID_AES_LP && aes_rn < 8);
	} else {
		u32 aesa, mdha;

		aesa = rd_reg32(&priv->ctrl->vreg.aesa);
		mdha = rd_reg32(&priv->ctrl->vreg.mdha);

		aes_vid = (aesa & CHA_VER_VID_MASK) >> CHA_VER_VID_SHIFT;
		md_vid = (mdha & CHA_VER_VID_MASK) >> CHA_VER_VID_SHIFT;

		des_inst = rd_reg32(&priv->ctrl->vreg.desa) & CHA_VER_NUM_MASK;
		aes_inst = aesa & CHA_VER_NUM_MASK;
		md_inst = mdha & CHA_VER_NUM_MASK;
		ccha_inst = rd_reg32(&priv->ctrl->vreg.ccha) & CHA_VER_NUM_MASK;
		ptha_inst = rd_reg32(&priv->ctrl->vreg.ptha) & CHA_VER_NUM_MASK;

		gcm_support = aesa & CHA_VER_MISC_AES_GCM;
	}

	/* If MD is present, limit digest size based on LP256 */
	if (md_inst && md_vid  == CHA_VER_VID_MD_LP256)
		md_limit = SHA256_DIGEST_SIZE;

	for (i = 0; i < ARRAY_SIZE(driver_algs); i++) {
		struct caam_skcipher_alg *t_alg = driver_algs + i;
		u32 alg_sel = t_alg->caam.class1_alg_type & OP_ALG_ALGSEL_MASK;
>>>>>>> upstream/android-13

		/* Skip DES algorithms if not supported by device */
		if (!des_inst &&
		    ((alg_sel == OP_ALG_ALGSEL_3DES) ||
		     (alg_sel == OP_ALG_ALGSEL_DES)))
				continue;

		/* Skip AES algorithms if not supported by device */
		if (!aes_inst && (alg_sel == OP_ALG_ALGSEL_AES))
				continue;

		/*
		 * Check support for AES modes not available
		 * on LP devices.
		 */
<<<<<<< HEAD
		if ((cha_vid & CHA_ID_LS_AES_MASK) == CHA_ID_LS_AES_LP)
			if ((alg->class1_alg_type & OP_ALG_AAI_MASK) ==
			     OP_ALG_AAI_XTS)
				continue;

		t_alg = caam_alg_alloc(alg);
		if (IS_ERR(t_alg)) {
			err = PTR_ERR(t_alg);
			pr_warn("%s alg allocation failed\n", alg->driver_name);
			continue;
		}

		err = crypto_register_alg(&t_alg->crypto_alg);
		if (err) {
			pr_warn("%s alg registration failed\n",
				t_alg->crypto_alg.cra_driver_name);
			kfree(t_alg);
			continue;
		}

		list_add_tail(&t_alg->entry, &alg_list);
=======
		if (aes_vid == CHA_VER_VID_AES_LP &&
		    (t_alg->caam.class1_alg_type & OP_ALG_AAI_MASK) ==
		    OP_ALG_AAI_XTS)
			continue;

		caam_skcipher_alg_init(t_alg);

		err = crypto_register_skcipher(&t_alg->skcipher);
		if (err) {
			pr_warn("%s alg registration failed\n",
				t_alg->skcipher.base.cra_driver_name);
			continue;
		}

		t_alg->registered = true;
>>>>>>> upstream/android-13
		registered = true;
	}

	for (i = 0; i < ARRAY_SIZE(driver_aeads); i++) {
		struct caam_aead_alg *t_alg = driver_aeads + i;
		u32 c1_alg_sel = t_alg->caam.class1_alg_type &
				 OP_ALG_ALGSEL_MASK;
		u32 c2_alg_sel = t_alg->caam.class2_alg_type &
				 OP_ALG_ALGSEL_MASK;
		u32 alg_aai = t_alg->caam.class1_alg_type & OP_ALG_AAI_MASK;

		/* Skip DES algorithms if not supported by device */
		if (!des_inst &&
		    ((c1_alg_sel == OP_ALG_ALGSEL_3DES) ||
		     (c1_alg_sel == OP_ALG_ALGSEL_DES)))
				continue;

		/* Skip AES algorithms if not supported by device */
		if (!aes_inst && (c1_alg_sel == OP_ALG_ALGSEL_AES))
				continue;

<<<<<<< HEAD
		/*
		 * Check support for AES algorithms not available
		 * on LP devices.
		 */
		if ((cha_vid & CHA_ID_LS_AES_MASK) == CHA_ID_LS_AES_LP)
			if (alg_aai == OP_ALG_AAI_GCM)
				continue;
=======
		/* Skip CHACHA20 algorithms if not supported by device */
		if (c1_alg_sel == OP_ALG_ALGSEL_CHACHA20 && !ccha_inst)
			continue;

		/* Skip POLY1305 algorithms if not supported by device */
		if (c2_alg_sel == OP_ALG_ALGSEL_POLY1305 && !ptha_inst)
			continue;

		/* Skip GCM algorithms if not supported by device */
		if (c1_alg_sel == OP_ALG_ALGSEL_AES &&
		    alg_aai == OP_ALG_AAI_GCM && !gcm_support)
			continue;
>>>>>>> upstream/android-13

		/*
		 * Skip algorithms requiring message digests
		 * if MD or MD size is not supported by device.
		 */
<<<<<<< HEAD
		if (c2_alg_sel &&
		    (!md_inst || (t_alg->aead.maxauthsize > md_limit)))
				continue;
=======
		if (is_mdha(c2_alg_sel) &&
		    (!md_inst || t_alg->aead.maxauthsize > md_limit))
			continue;
>>>>>>> upstream/android-13

		caam_aead_alg_init(t_alg);

		err = crypto_register_aead(&t_alg->aead);
		if (err) {
			pr_warn("%s alg registration failed\n",
				t_alg->aead.base.cra_driver_name);
			continue;
		}

		t_alg->registered = true;
		registered = true;
	}

	if (registered)
		pr_info("caam algorithms registered in /proc/crypto\n");

	return err;
}
<<<<<<< HEAD

module_init(caam_algapi_init);
module_exit(caam_algapi_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("FSL CAAM support for crypto API");
MODULE_AUTHOR("Freescale Semiconductor - NMG/STC");
=======
>>>>>>> upstream/android-13
