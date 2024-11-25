// SPDX-License-Identifier: GPL-2.0
<<<<<<< HEAD
/* Copyright (C) 2012-2018 ARM Limited or its affiliates. */
=======
/* Copyright (C) 2012-2019 ARM Limited (or its affiliates). */
>>>>>>> upstream/android-13

#include <linux/kernel.h>
#include <linux/module.h>
#include <crypto/algapi.h>
#include <crypto/internal/skcipher.h>
<<<<<<< HEAD
#include <crypto/des.h>
#include <crypto/xts.h>
=======
#include <crypto/internal/des.h>
#include <crypto/xts.h>
#include <crypto/sm4.h>
>>>>>>> upstream/android-13
#include <crypto/scatterwalk.h>

#include "cc_driver.h"
#include "cc_lli_defs.h"
#include "cc_buffer_mgr.h"
#include "cc_cipher.h"
#include "cc_request_mgr.h"

<<<<<<< HEAD
#define MAX_ABLKCIPHER_SEQ_LEN 6

#define template_skcipher	template_u.skcipher

struct cc_cipher_handle {
	struct list_head alg_list;
};

=======
#define MAX_SKCIPHER_SEQ_LEN 6

#define template_skcipher	template_u.skcipher

>>>>>>> upstream/android-13
struct cc_user_key_info {
	u8 *key;
	dma_addr_t key_dma_addr;
};

struct cc_hw_key_info {
	enum cc_hw_crypto_key key1_slot;
	enum cc_hw_crypto_key key2_slot;
};

<<<<<<< HEAD
struct cc_cipher_ctx {
	struct cc_drvdata *drvdata;
	int keylen;
	int key_round_number;
	int cipher_mode;
	int flow_mode;
	unsigned int flags;
	bool hw_key;
	struct cc_user_key_info user;
	struct cc_hw_key_info hw;
	struct crypto_shash *shash_tfm;
=======
struct cc_cpp_key_info {
	u8 slot;
	enum cc_cpp_alg alg;
};

enum cc_key_type {
	CC_UNPROTECTED_KEY,		/* User key */
	CC_HW_PROTECTED_KEY,		/* HW (FDE) key */
	CC_POLICY_PROTECTED_KEY,	/* CPP key */
	CC_INVALID_PROTECTED_KEY	/* Invalid key */
};

struct cc_cipher_ctx {
	struct cc_drvdata *drvdata;
	int keylen;
	int cipher_mode;
	int flow_mode;
	unsigned int flags;
	enum cc_key_type key_type;
	struct cc_user_key_info user;
	union {
		struct cc_hw_key_info hw;
		struct cc_cpp_key_info cpp;
	};
	struct crypto_shash *shash_tfm;
	struct crypto_skcipher *fallback_tfm;
	bool fallback_on;
>>>>>>> upstream/android-13
};

static void cc_cipher_complete(struct device *dev, void *cc_req, int err);

<<<<<<< HEAD
static inline bool cc_is_hw_key(struct crypto_tfm *tfm)
{
	struct cc_cipher_ctx *ctx_p = crypto_tfm_ctx(tfm);

	return ctx_p->hw_key;
=======
static inline enum cc_key_type cc_key_type(struct crypto_tfm *tfm)
{
	struct cc_cipher_ctx *ctx_p = crypto_tfm_ctx(tfm);

	return ctx_p->key_type;
>>>>>>> upstream/android-13
}

static int validate_keys_sizes(struct cc_cipher_ctx *ctx_p, u32 size)
{
	switch (ctx_p->flow_mode) {
	case S_DIN_to_AES:
		switch (size) {
		case CC_AES_128_BIT_KEY_SIZE:
		case CC_AES_192_BIT_KEY_SIZE:
<<<<<<< HEAD
			if (ctx_p->cipher_mode != DRV_CIPHER_XTS &&
			    ctx_p->cipher_mode != DRV_CIPHER_ESSIV &&
			    ctx_p->cipher_mode != DRV_CIPHER_BITLOCKER)
=======
			if (ctx_p->cipher_mode != DRV_CIPHER_XTS)
>>>>>>> upstream/android-13
				return 0;
			break;
		case CC_AES_256_BIT_KEY_SIZE:
			return 0;
		case (CC_AES_192_BIT_KEY_SIZE * 2):
		case (CC_AES_256_BIT_KEY_SIZE * 2):
			if (ctx_p->cipher_mode == DRV_CIPHER_XTS ||
<<<<<<< HEAD
			    ctx_p->cipher_mode == DRV_CIPHER_ESSIV ||
			    ctx_p->cipher_mode == DRV_CIPHER_BITLOCKER)
=======
			    ctx_p->cipher_mode == DRV_CIPHER_ESSIV)
>>>>>>> upstream/android-13
				return 0;
			break;
		default:
			break;
		}
		break;
	case S_DIN_to_DES:
		if (size == DES3_EDE_KEY_SIZE || size == DES_KEY_SIZE)
			return 0;
		break;
<<<<<<< HEAD
=======
	case S_DIN_to_SM4:
		if (size == SM4_KEY_SIZE)
			return 0;
		break;
>>>>>>> upstream/android-13
	default:
		break;
	}
	return -EINVAL;
}

static int validate_data_size(struct cc_cipher_ctx *ctx_p,
			      unsigned int size)
{
	switch (ctx_p->flow_mode) {
	case S_DIN_to_AES:
		switch (ctx_p->cipher_mode) {
		case DRV_CIPHER_XTS:
<<<<<<< HEAD
			if (size >= AES_BLOCK_SIZE &&
			    IS_ALIGNED(size, AES_BLOCK_SIZE))
				return 0;
			break;
=======
>>>>>>> upstream/android-13
		case DRV_CIPHER_CBC_CTS:
			if (size >= AES_BLOCK_SIZE)
				return 0;
			break;
		case DRV_CIPHER_OFB:
		case DRV_CIPHER_CTR:
				return 0;
		case DRV_CIPHER_ECB:
		case DRV_CIPHER_CBC:
		case DRV_CIPHER_ESSIV:
<<<<<<< HEAD
		case DRV_CIPHER_BITLOCKER:
=======
>>>>>>> upstream/android-13
			if (IS_ALIGNED(size, AES_BLOCK_SIZE))
				return 0;
			break;
		default:
			break;
		}
		break;
	case S_DIN_to_DES:
		if (IS_ALIGNED(size, DES_BLOCK_SIZE))
			return 0;
		break;
<<<<<<< HEAD
=======
	case S_DIN_to_SM4:
		switch (ctx_p->cipher_mode) {
		case DRV_CIPHER_CTR:
			return 0;
		case DRV_CIPHER_ECB:
		case DRV_CIPHER_CBC:
			if (IS_ALIGNED(size, SM4_BLOCK_SIZE))
				return 0;
			break;
		default:
			break;
		}
		break;
>>>>>>> upstream/android-13
	default:
		break;
	}
	return -EINVAL;
}

static int cc_cipher_init(struct crypto_tfm *tfm)
{
	struct cc_cipher_ctx *ctx_p = crypto_tfm_ctx(tfm);
	struct cc_crypto_alg *cc_alg =
			container_of(tfm->__crt_alg, struct cc_crypto_alg,
				     skcipher_alg.base);
	struct device *dev = drvdata_to_dev(cc_alg->drvdata);
	unsigned int max_key_buf_size = cc_alg->skcipher_alg.max_keysize;
<<<<<<< HEAD
=======
	unsigned int fallback_req_size = 0;
>>>>>>> upstream/android-13

	dev_dbg(dev, "Initializing context @%p for %s\n", ctx_p,
		crypto_tfm_alg_name(tfm));

<<<<<<< HEAD
	crypto_skcipher_set_reqsize(__crypto_skcipher_cast(tfm),
				    sizeof(struct cipher_req_ctx));

=======
>>>>>>> upstream/android-13
	ctx_p->cipher_mode = cc_alg->cipher_mode;
	ctx_p->flow_mode = cc_alg->flow_mode;
	ctx_p->drvdata = cc_alg->drvdata;

	if (ctx_p->cipher_mode == DRV_CIPHER_ESSIV) {
<<<<<<< HEAD
		/* Alloc hash tfm for essiv */
		ctx_p->shash_tfm = crypto_alloc_shash("sha256-generic", 0, 0);
=======
		const char *name = crypto_tfm_alg_name(tfm);

		/* Alloc hash tfm for essiv */
		ctx_p->shash_tfm = crypto_alloc_shash("sha256", 0, 0);
>>>>>>> upstream/android-13
		if (IS_ERR(ctx_p->shash_tfm)) {
			dev_err(dev, "Error allocating hash tfm for ESSIV.\n");
			return PTR_ERR(ctx_p->shash_tfm);
		}
<<<<<<< HEAD
	}

	/* Allocate key buffer, cache line aligned */
	ctx_p->user.key = kmalloc(max_key_buf_size, GFP_KERNEL);
	if (!ctx_p->user.key)
		goto free_shash;
=======
		max_key_buf_size <<= 1;

		/* Alloc fallabck tfm or essiv when key size != 256 bit */
		ctx_p->fallback_tfm =
			crypto_alloc_skcipher(name, 0, CRYPTO_ALG_NEED_FALLBACK | CRYPTO_ALG_ASYNC);

		if (IS_ERR(ctx_p->fallback_tfm)) {
			/* Note we're still allowing registration with no fallback since it's
			 * better to have most modes supported than none at all.
			 */
			dev_warn(dev, "Error allocating fallback algo %s. Some modes may be available.\n",
			       name);
			ctx_p->fallback_tfm = NULL;
		} else {
			fallback_req_size = crypto_skcipher_reqsize(ctx_p->fallback_tfm);
		}
	}

	crypto_skcipher_set_reqsize(__crypto_skcipher_cast(tfm),
				    sizeof(struct cipher_req_ctx) + fallback_req_size);

	/* Allocate key buffer, cache line aligned */
	ctx_p->user.key = kzalloc(max_key_buf_size, GFP_KERNEL);
	if (!ctx_p->user.key)
		goto free_fallback;
>>>>>>> upstream/android-13

	dev_dbg(dev, "Allocated key buffer in context. key=@%p\n",
		ctx_p->user.key);

	/* Map key buffer */
<<<<<<< HEAD
	ctx_p->user.key_dma_addr = dma_map_single(dev, (void *)ctx_p->user.key,
=======
	ctx_p->user.key_dma_addr = dma_map_single(dev, ctx_p->user.key,
>>>>>>> upstream/android-13
						  max_key_buf_size,
						  DMA_TO_DEVICE);
	if (dma_mapping_error(dev, ctx_p->user.key_dma_addr)) {
		dev_err(dev, "Mapping Key %u B at va=%pK for DMA failed\n",
			max_key_buf_size, ctx_p->user.key);
		goto free_key;
	}
	dev_dbg(dev, "Mapped key %u B at va=%pK to dma=%pad\n",
		max_key_buf_size, ctx_p->user.key, &ctx_p->user.key_dma_addr);

	return 0;

free_key:
	kfree(ctx_p->user.key);
<<<<<<< HEAD
free_shash:
=======
free_fallback:
	crypto_free_skcipher(ctx_p->fallback_tfm);
>>>>>>> upstream/android-13
	crypto_free_shash(ctx_p->shash_tfm);

	return -ENOMEM;
}

static void cc_cipher_exit(struct crypto_tfm *tfm)
{
	struct crypto_alg *alg = tfm->__crt_alg;
	struct cc_crypto_alg *cc_alg =
			container_of(alg, struct cc_crypto_alg,
				     skcipher_alg.base);
	unsigned int max_key_buf_size = cc_alg->skcipher_alg.max_keysize;
	struct cc_cipher_ctx *ctx_p = crypto_tfm_ctx(tfm);
	struct device *dev = drvdata_to_dev(ctx_p->drvdata);

	dev_dbg(dev, "Clearing context @%p for %s\n",
		crypto_tfm_ctx(tfm), crypto_tfm_alg_name(tfm));

	if (ctx_p->cipher_mode == DRV_CIPHER_ESSIV) {
		/* Free hash tfm for essiv */
		crypto_free_shash(ctx_p->shash_tfm);
		ctx_p->shash_tfm = NULL;
<<<<<<< HEAD
=======
		crypto_free_skcipher(ctx_p->fallback_tfm);
		ctx_p->fallback_tfm = NULL;
>>>>>>> upstream/android-13
	}

	/* Unmap key buffer */
	dma_unmap_single(dev, ctx_p->user.key_dma_addr, max_key_buf_size,
			 DMA_TO_DEVICE);
	dev_dbg(dev, "Unmapped key buffer key_dma_addr=%pad\n",
		&ctx_p->user.key_dma_addr);

	/* Free key buffer in context */
<<<<<<< HEAD
	kzfree(ctx_p->user.key);
	dev_dbg(dev, "Free key buffer in context. key=@%p\n", ctx_p->user.key);
=======
	dev_dbg(dev, "Free key buffer in context. key=@%p\n", ctx_p->user.key);
	kfree_sensitive(ctx_p->user.key);
>>>>>>> upstream/android-13
}

struct tdes_keys {
	u8	key1[DES_KEY_SIZE];
	u8	key2[DES_KEY_SIZE];
	u8	key3[DES_KEY_SIZE];
};

<<<<<<< HEAD
static enum cc_hw_crypto_key cc_slot_to_hw_key(int slot_num)
=======
static enum cc_hw_crypto_key cc_slot_to_hw_key(u8 slot_num)
>>>>>>> upstream/android-13
{
	switch (slot_num) {
	case 0:
		return KFDE0_KEY;
	case 1:
		return KFDE1_KEY;
	case 2:
		return KFDE2_KEY;
	case 3:
		return KFDE3_KEY;
	}
	return END_OF_KEYS;
}

<<<<<<< HEAD
=======
static u8 cc_slot_to_cpp_key(u8 slot_num)
{
	return (slot_num - CC_FIRST_CPP_KEY_SLOT);
}

static inline enum cc_key_type cc_slot_to_key_type(u8 slot_num)
{
	if (slot_num >= CC_FIRST_HW_KEY_SLOT && slot_num <= CC_LAST_HW_KEY_SLOT)
		return CC_HW_PROTECTED_KEY;
	else if (slot_num >=  CC_FIRST_CPP_KEY_SLOT &&
		 slot_num <=  CC_LAST_CPP_KEY_SLOT)
		return CC_POLICY_PROTECTED_KEY;
	else
		return CC_INVALID_PROTECTED_KEY;
}

>>>>>>> upstream/android-13
static int cc_cipher_sethkey(struct crypto_skcipher *sktfm, const u8 *key,
			     unsigned int keylen)
{
	struct crypto_tfm *tfm = crypto_skcipher_tfm(sktfm);
	struct cc_cipher_ctx *ctx_p = crypto_tfm_ctx(tfm);
	struct device *dev = drvdata_to_dev(ctx_p->drvdata);
	struct cc_hkey_info hki;

	dev_dbg(dev, "Setting HW key in context @%p for %s. keylen=%u\n",
		ctx_p, crypto_tfm_alg_name(tfm), keylen);
<<<<<<< HEAD
	dump_byte_array("key", (u8 *)key, keylen);

	/* STAT_PHASE_0: Init and sanity checks */

	/* This check the size of the hardware key token */
	if (keylen != sizeof(hki)) {
		dev_err(dev, "Unsupported HW key size %d.\n", keylen);
		crypto_tfm_set_flags(tfm, CRYPTO_TFM_RES_BAD_KEY_LEN);
		return -EINVAL;
	}

	if (ctx_p->flow_mode != S_DIN_to_AES) {
		dev_err(dev, "HW key not supported for non-AES flows\n");
=======
	dump_byte_array("key", key, keylen);

	/* STAT_PHASE_0: Init and sanity checks */

	/* This check the size of the protected key token */
	if (keylen != sizeof(hki)) {
		dev_err(dev, "Unsupported protected key size %d.\n", keylen);
>>>>>>> upstream/android-13
		return -EINVAL;
	}

	memcpy(&hki, key, keylen);

	/* The real key len for crypto op is the size of the HW key
	 * referenced by the HW key slot, not the hardware key token
	 */
	keylen = hki.keylen;

	if (validate_keys_sizes(ctx_p, keylen)) {
<<<<<<< HEAD
		dev_err(dev, "Unsupported key size %d.\n", keylen);
		crypto_tfm_set_flags(tfm, CRYPTO_TFM_RES_BAD_KEY_LEN);
		return -EINVAL;
	}

	ctx_p->hw.key1_slot = cc_slot_to_hw_key(hki.hw_key1);
	if (ctx_p->hw.key1_slot == END_OF_KEYS) {
		dev_err(dev, "Unsupported hw key1 number (%d)\n", hki.hw_key1);
		return -EINVAL;
	}

	if (ctx_p->cipher_mode == DRV_CIPHER_XTS ||
	    ctx_p->cipher_mode == DRV_CIPHER_ESSIV ||
	    ctx_p->cipher_mode == DRV_CIPHER_BITLOCKER) {
		if (hki.hw_key1 == hki.hw_key2) {
			dev_err(dev, "Illegal hw key numbers (%d,%d)\n",
				hki.hw_key1, hki.hw_key2);
			return -EINVAL;
		}
		ctx_p->hw.key2_slot = cc_slot_to_hw_key(hki.hw_key2);
		if (ctx_p->hw.key2_slot == END_OF_KEYS) {
			dev_err(dev, "Unsupported hw key2 number (%d)\n",
				hki.hw_key2);
			return -EINVAL;
		}
	}

	ctx_p->keylen = keylen;
	ctx_p->hw_key = true;
	dev_dbg(dev, "cc_is_hw_key ret 0");
=======
		dev_dbg(dev, "Unsupported key size %d.\n", keylen);
		return -EINVAL;
	}

	ctx_p->keylen = keylen;
	ctx_p->fallback_on = false;

	switch (cc_slot_to_key_type(hki.hw_key1)) {
	case CC_HW_PROTECTED_KEY:
		if (ctx_p->flow_mode == S_DIN_to_SM4) {
			dev_err(dev, "Only AES HW protected keys are supported\n");
			return -EINVAL;
		}

		ctx_p->hw.key1_slot = cc_slot_to_hw_key(hki.hw_key1);
		if (ctx_p->hw.key1_slot == END_OF_KEYS) {
			dev_err(dev, "Unsupported hw key1 number (%d)\n",
				hki.hw_key1);
			return -EINVAL;
		}

		if (ctx_p->cipher_mode == DRV_CIPHER_XTS ||
		    ctx_p->cipher_mode == DRV_CIPHER_ESSIV) {
			if (hki.hw_key1 == hki.hw_key2) {
				dev_err(dev, "Illegal hw key numbers (%d,%d)\n",
					hki.hw_key1, hki.hw_key2);
				return -EINVAL;
			}

			ctx_p->hw.key2_slot = cc_slot_to_hw_key(hki.hw_key2);
			if (ctx_p->hw.key2_slot == END_OF_KEYS) {
				dev_err(dev, "Unsupported hw key2 number (%d)\n",
					hki.hw_key2);
				return -EINVAL;
			}
		}

		ctx_p->key_type = CC_HW_PROTECTED_KEY;
		dev_dbg(dev, "HW protected key  %d/%d set\n.",
			ctx_p->hw.key1_slot, ctx_p->hw.key2_slot);
		break;

	case CC_POLICY_PROTECTED_KEY:
		if (ctx_p->drvdata->hw_rev < CC_HW_REV_713) {
			dev_err(dev, "CPP keys not supported in this hardware revision.\n");
			return -EINVAL;
		}

		if (ctx_p->cipher_mode != DRV_CIPHER_CBC &&
		    ctx_p->cipher_mode != DRV_CIPHER_CTR) {
			dev_err(dev, "CPP keys only supported in CBC or CTR modes.\n");
			return -EINVAL;
		}

		ctx_p->cpp.slot = cc_slot_to_cpp_key(hki.hw_key1);
		if (ctx_p->flow_mode == S_DIN_to_AES)
			ctx_p->cpp.alg = CC_CPP_AES;
		else /* Must be SM4 since due to sethkey registration */
			ctx_p->cpp.alg = CC_CPP_SM4;
		ctx_p->key_type = CC_POLICY_PROTECTED_KEY;
		dev_dbg(dev, "policy protected key alg: %d slot: %d.\n",
			ctx_p->cpp.alg, ctx_p->cpp.slot);
		break;

	default:
		dev_err(dev, "Unsupported protected key (%d)\n", hki.hw_key1);
		return -EINVAL;
	}
>>>>>>> upstream/android-13

	return 0;
}

static int cc_cipher_setkey(struct crypto_skcipher *sktfm, const u8 *key,
			    unsigned int keylen)
{
	struct crypto_tfm *tfm = crypto_skcipher_tfm(sktfm);
	struct cc_cipher_ctx *ctx_p = crypto_tfm_ctx(tfm);
	struct device *dev = drvdata_to_dev(ctx_p->drvdata);
	struct cc_crypto_alg *cc_alg =
			container_of(tfm->__crt_alg, struct cc_crypto_alg,
				     skcipher_alg.base);
	unsigned int max_key_buf_size = cc_alg->skcipher_alg.max_keysize;

	dev_dbg(dev, "Setting key in context @%p for %s. keylen=%u\n",
		ctx_p, crypto_tfm_alg_name(tfm), keylen);
<<<<<<< HEAD
	dump_byte_array("key", (u8 *)key, keylen);
=======
	dump_byte_array("key", key, keylen);
>>>>>>> upstream/android-13

	/* STAT_PHASE_0: Init and sanity checks */

	if (validate_keys_sizes(ctx_p, keylen)) {
<<<<<<< HEAD
		dev_err(dev, "Unsupported key size %d.\n", keylen);
		crypto_tfm_set_flags(tfm, CRYPTO_TFM_RES_BAD_KEY_LEN);
		return -EINVAL;
	}

	ctx_p->hw_key = false;
=======
		dev_dbg(dev, "Invalid key size %d.\n", keylen);
		return -EINVAL;
	}

	if (ctx_p->cipher_mode == DRV_CIPHER_ESSIV) {

		/* We only support 256 bit ESSIV-CBC-AES keys */
		if (keylen != AES_KEYSIZE_256)  {
			unsigned int flags = crypto_tfm_get_flags(tfm) & CRYPTO_TFM_REQ_MASK;

			if (likely(ctx_p->fallback_tfm)) {
				ctx_p->fallback_on = true;
				crypto_skcipher_clear_flags(ctx_p->fallback_tfm,
							    CRYPTO_TFM_REQ_MASK);
				crypto_skcipher_clear_flags(ctx_p->fallback_tfm, flags);
				return crypto_skcipher_setkey(ctx_p->fallback_tfm, key, keylen);
			}

			dev_dbg(dev, "Unsupported key size %d and no fallback.\n", keylen);
			return -EINVAL;
		}

		/* Internal ESSIV key buffer is double sized */
		max_key_buf_size <<= 1;
	}

	ctx_p->fallback_on = false;
	ctx_p->key_type = CC_UNPROTECTED_KEY;
>>>>>>> upstream/android-13

	/*
	 * Verify DES weak keys
	 * Note that we're dropping the expanded key since the
	 * HW does the expansion on its own.
	 */
	if (ctx_p->flow_mode == S_DIN_to_DES) {
<<<<<<< HEAD
		u32 tmp[DES3_EDE_EXPKEY_WORDS];
		if (keylen == DES3_EDE_KEY_SIZE &&
		    __des3_ede_setkey(tmp, &tfm->crt_flags, key,
				      DES3_EDE_KEY_SIZE)) {
			dev_dbg(dev, "weak 3DES key");
			return -EINVAL;
		} else if (!des_ekey(tmp, key) &&
		    (crypto_tfm_get_flags(tfm) & CRYPTO_TFM_REQ_WEAK_KEY)) {
			tfm->crt_flags |= CRYPTO_TFM_RES_WEAK_KEY;
=======
		if ((keylen == DES3_EDE_KEY_SIZE &&
		     verify_skcipher_des3_key(sktfm, key)) ||
		    verify_skcipher_des_key(sktfm, key)) {
>>>>>>> upstream/android-13
			dev_dbg(dev, "weak DES key");
			return -EINVAL;
		}
	}

	if (ctx_p->cipher_mode == DRV_CIPHER_XTS &&
	    xts_check_key(tfm, key, keylen)) {
		dev_dbg(dev, "weak XTS key");
		return -EINVAL;
	}

	/* STAT_PHASE_1: Copy key to ctx */
	dma_sync_single_for_cpu(dev, ctx_p->user.key_dma_addr,
				max_key_buf_size, DMA_TO_DEVICE);

	memcpy(ctx_p->user.key, key, keylen);
<<<<<<< HEAD
	if (keylen == 24)
		memset(ctx_p->user.key + 24, 0, CC_AES_KEY_SIZE_MAX - 24);

	if (ctx_p->cipher_mode == DRV_CIPHER_ESSIV) {
		/* sha256 for key2 - use sw implementation */
		int key_len = keylen >> 1;
		int err;

		SHASH_DESC_ON_STACK(desc, ctx_p->shash_tfm);

		desc->tfm = ctx_p->shash_tfm;

		err = crypto_shash_digest(desc, ctx_p->user.key, key_len,
					  ctx_p->user.key + key_len);
=======

	if (ctx_p->cipher_mode == DRV_CIPHER_ESSIV) {
		/* sha256 for key2 - use sw implementation */
		int err;

		err = crypto_shash_tfm_digest(ctx_p->shash_tfm,
					      ctx_p->user.key, keylen,
					      ctx_p->user.key + keylen);
>>>>>>> upstream/android-13
		if (err) {
			dev_err(dev, "Failed to hash ESSIV key.\n");
			return err;
		}
<<<<<<< HEAD
=======

		keylen <<= 1;
>>>>>>> upstream/android-13
	}
	dma_sync_single_for_device(dev, ctx_p->user.key_dma_addr,
				   max_key_buf_size, DMA_TO_DEVICE);
	ctx_p->keylen = keylen;

	dev_dbg(dev, "return safely");
	return 0;
}

<<<<<<< HEAD
static void cc_setup_cipher_desc(struct crypto_tfm *tfm,
=======
static int cc_out_setup_mode(struct cc_cipher_ctx *ctx_p)
{
	switch (ctx_p->flow_mode) {
	case S_DIN_to_AES:
		return S_AES_to_DOUT;
	case S_DIN_to_DES:
		return S_DES_to_DOUT;
	case S_DIN_to_SM4:
		return S_SM4_to_DOUT;
	default:
		return ctx_p->flow_mode;
	}
}

static void cc_setup_readiv_desc(struct crypto_tfm *tfm,
				 struct cipher_req_ctx *req_ctx,
				 unsigned int ivsize, struct cc_hw_desc desc[],
				 unsigned int *seq_size)
{
	struct cc_cipher_ctx *ctx_p = crypto_tfm_ctx(tfm);
	struct device *dev = drvdata_to_dev(ctx_p->drvdata);
	int cipher_mode = ctx_p->cipher_mode;
	int flow_mode = cc_out_setup_mode(ctx_p);
	int direction = req_ctx->gen_ctx.op_type;
	dma_addr_t iv_dma_addr = req_ctx->gen_ctx.iv_dma_addr;

	if (ctx_p->key_type == CC_POLICY_PROTECTED_KEY)
		return;

	switch (cipher_mode) {
	case DRV_CIPHER_ECB:
		break;
	case DRV_CIPHER_CBC:
	case DRV_CIPHER_CBC_CTS:
	case DRV_CIPHER_CTR:
	case DRV_CIPHER_OFB:
		/* Read next IV */
		hw_desc_init(&desc[*seq_size]);
		set_dout_dlli(&desc[*seq_size], iv_dma_addr, ivsize, NS_BIT, 1);
		set_cipher_config0(&desc[*seq_size], direction);
		set_flow_mode(&desc[*seq_size], flow_mode);
		set_cipher_mode(&desc[*seq_size], cipher_mode);
		if (cipher_mode == DRV_CIPHER_CTR ||
		    cipher_mode == DRV_CIPHER_OFB) {
			set_setup_mode(&desc[*seq_size], SETUP_WRITE_STATE1);
		} else {
			set_setup_mode(&desc[*seq_size], SETUP_WRITE_STATE0);
		}
		set_queue_last_ind(ctx_p->drvdata, &desc[*seq_size]);
		(*seq_size)++;
		break;
	case DRV_CIPHER_XTS:
	case DRV_CIPHER_ESSIV:
		/*  IV */
		hw_desc_init(&desc[*seq_size]);
		set_setup_mode(&desc[*seq_size], SETUP_WRITE_STATE1);
		set_cipher_mode(&desc[*seq_size], cipher_mode);
		set_cipher_config0(&desc[*seq_size], direction);
		set_flow_mode(&desc[*seq_size], flow_mode);
		set_dout_dlli(&desc[*seq_size], iv_dma_addr, CC_AES_BLOCK_SIZE,
			     NS_BIT, 1);
		set_queue_last_ind(ctx_p->drvdata, &desc[*seq_size]);
		(*seq_size)++;
		break;
	default:
		dev_err(dev, "Unsupported cipher mode (%d)\n", cipher_mode);
	}
}


static void cc_setup_state_desc(struct crypto_tfm *tfm,
>>>>>>> upstream/android-13
				 struct cipher_req_ctx *req_ctx,
				 unsigned int ivsize, unsigned int nbytes,
				 struct cc_hw_desc desc[],
				 unsigned int *seq_size)
{
	struct cc_cipher_ctx *ctx_p = crypto_tfm_ctx(tfm);
	struct device *dev = drvdata_to_dev(ctx_p->drvdata);
	int cipher_mode = ctx_p->cipher_mode;
	int flow_mode = ctx_p->flow_mode;
	int direction = req_ctx->gen_ctx.op_type;
<<<<<<< HEAD
	dma_addr_t key_dma_addr = ctx_p->user.key_dma_addr;
	unsigned int key_len = ctx_p->keylen;
	dma_addr_t iv_dma_addr = req_ctx->gen_ctx.iv_dma_addr;
	unsigned int du_size = nbytes;

	struct cc_crypto_alg *cc_alg =
		container_of(tfm->__crt_alg, struct cc_crypto_alg,
			     skcipher_alg.base);

	if (cc_alg->data_unit)
		du_size = cc_alg->data_unit;

	switch (cipher_mode) {
=======
	dma_addr_t iv_dma_addr = req_ctx->gen_ctx.iv_dma_addr;

	switch (cipher_mode) {
	case DRV_CIPHER_ECB:
		break;
>>>>>>> upstream/android-13
	case DRV_CIPHER_CBC:
	case DRV_CIPHER_CBC_CTS:
	case DRV_CIPHER_CTR:
	case DRV_CIPHER_OFB:
<<<<<<< HEAD
		/* Load cipher state */
=======
		/* Load IV */
>>>>>>> upstream/android-13
		hw_desc_init(&desc[*seq_size]);
		set_din_type(&desc[*seq_size], DMA_DLLI, iv_dma_addr, ivsize,
			     NS_BIT);
		set_cipher_config0(&desc[*seq_size], direction);
		set_flow_mode(&desc[*seq_size], flow_mode);
		set_cipher_mode(&desc[*seq_size], cipher_mode);
		if (cipher_mode == DRV_CIPHER_CTR ||
		    cipher_mode == DRV_CIPHER_OFB) {
			set_setup_mode(&desc[*seq_size], SETUP_LOAD_STATE1);
		} else {
			set_setup_mode(&desc[*seq_size], SETUP_LOAD_STATE0);
		}
		(*seq_size)++;
<<<<<<< HEAD
		/*FALLTHROUGH*/
	case DRV_CIPHER_ECB:
		/* Load key */
		hw_desc_init(&desc[*seq_size]);
		set_cipher_mode(&desc[*seq_size], cipher_mode);
		set_cipher_config0(&desc[*seq_size], direction);
		if (flow_mode == S_DIN_to_AES) {
			if (cc_is_hw_key(tfm)) {
				set_hw_crypto_key(&desc[*seq_size],
						  ctx_p->hw.key1_slot);
			} else {
				set_din_type(&desc[*seq_size], DMA_DLLI,
					     key_dma_addr, ((key_len == 24) ?
							    AES_MAX_KEY_SIZE :
							    key_len), NS_BIT);
			}
			set_key_size_aes(&desc[*seq_size], key_len);
		} else {
			/*des*/
			set_din_type(&desc[*seq_size], DMA_DLLI, key_dma_addr,
				     key_len, NS_BIT);
			set_key_size_des(&desc[*seq_size], key_len);
		}
		set_flow_mode(&desc[*seq_size], flow_mode);
		set_setup_mode(&desc[*seq_size], SETUP_LOAD_KEY0);
		(*seq_size)++;
		break;
	case DRV_CIPHER_XTS:
	case DRV_CIPHER_ESSIV:
	case DRV_CIPHER_BITLOCKER:
		/* Load AES key */
		hw_desc_init(&desc[*seq_size]);
		set_cipher_mode(&desc[*seq_size], cipher_mode);
		set_cipher_config0(&desc[*seq_size], direction);
		if (cc_is_hw_key(tfm)) {
			set_hw_crypto_key(&desc[*seq_size],
					  ctx_p->hw.key1_slot);
		} else {
			set_din_type(&desc[*seq_size], DMA_DLLI, key_dma_addr,
				     (key_len / 2), NS_BIT);
		}
		set_key_size_aes(&desc[*seq_size], (key_len / 2));
		set_flow_mode(&desc[*seq_size], flow_mode);
		set_setup_mode(&desc[*seq_size], SETUP_LOAD_KEY0);
		(*seq_size)++;
=======
		break;
	case DRV_CIPHER_XTS:
	case DRV_CIPHER_ESSIV:
		break;
	default:
		dev_err(dev, "Unsupported cipher mode (%d)\n", cipher_mode);
	}
}


static void cc_setup_xex_state_desc(struct crypto_tfm *tfm,
				 struct cipher_req_ctx *req_ctx,
				 unsigned int ivsize, unsigned int nbytes,
				 struct cc_hw_desc desc[],
				 unsigned int *seq_size)
{
	struct cc_cipher_ctx *ctx_p = crypto_tfm_ctx(tfm);
	struct device *dev = drvdata_to_dev(ctx_p->drvdata);
	int cipher_mode = ctx_p->cipher_mode;
	int flow_mode = ctx_p->flow_mode;
	int direction = req_ctx->gen_ctx.op_type;
	dma_addr_t key_dma_addr = ctx_p->user.key_dma_addr;
	unsigned int key_len = (ctx_p->keylen / 2);
	dma_addr_t iv_dma_addr = req_ctx->gen_ctx.iv_dma_addr;
	unsigned int key_offset = key_len;

	switch (cipher_mode) {
	case DRV_CIPHER_ECB:
		break;
	case DRV_CIPHER_CBC:
	case DRV_CIPHER_CBC_CTS:
	case DRV_CIPHER_CTR:
	case DRV_CIPHER_OFB:
		break;
	case DRV_CIPHER_XTS:
	case DRV_CIPHER_ESSIV:

		if (cipher_mode == DRV_CIPHER_ESSIV)
			key_len = SHA256_DIGEST_SIZE;
>>>>>>> upstream/android-13

		/* load XEX key */
		hw_desc_init(&desc[*seq_size]);
		set_cipher_mode(&desc[*seq_size], cipher_mode);
		set_cipher_config0(&desc[*seq_size], direction);
<<<<<<< HEAD
		if (cc_is_hw_key(tfm)) {
=======
		if (cc_key_type(tfm) == CC_HW_PROTECTED_KEY) {
>>>>>>> upstream/android-13
			set_hw_crypto_key(&desc[*seq_size],
					  ctx_p->hw.key2_slot);
		} else {
			set_din_type(&desc[*seq_size], DMA_DLLI,
<<<<<<< HEAD
				     (key_dma_addr + (key_len / 2)),
				     (key_len / 2), NS_BIT);
		}
		set_xex_data_unit_size(&desc[*seq_size], du_size);
		set_flow_mode(&desc[*seq_size], S_DIN_to_AES2);
		set_key_size_aes(&desc[*seq_size], (key_len / 2));
		set_setup_mode(&desc[*seq_size], SETUP_LOAD_XEX_KEY);
		(*seq_size)++;

		/* Set state */
=======
				     (key_dma_addr + key_offset),
				     key_len, NS_BIT);
		}
		set_xex_data_unit_size(&desc[*seq_size], nbytes);
		set_flow_mode(&desc[*seq_size], S_DIN_to_AES2);
		set_key_size_aes(&desc[*seq_size], key_len);
		set_setup_mode(&desc[*seq_size], SETUP_LOAD_XEX_KEY);
		(*seq_size)++;

		/* Load IV */
>>>>>>> upstream/android-13
		hw_desc_init(&desc[*seq_size]);
		set_setup_mode(&desc[*seq_size], SETUP_LOAD_STATE1);
		set_cipher_mode(&desc[*seq_size], cipher_mode);
		set_cipher_config0(&desc[*seq_size], direction);
<<<<<<< HEAD
		set_key_size_aes(&desc[*seq_size], (key_len / 2));
=======
		set_key_size_aes(&desc[*seq_size], key_len);
>>>>>>> upstream/android-13
		set_flow_mode(&desc[*seq_size], flow_mode);
		set_din_type(&desc[*seq_size], DMA_DLLI, iv_dma_addr,
			     CC_AES_BLOCK_SIZE, NS_BIT);
		(*seq_size)++;
		break;
	default:
		dev_err(dev, "Unsupported cipher mode (%d)\n", cipher_mode);
	}
}

<<<<<<< HEAD
static void cc_setup_cipher_data(struct crypto_tfm *tfm,
				 struct cipher_req_ctx *req_ctx,
				 struct scatterlist *dst,
				 struct scatterlist *src, unsigned int nbytes,
				 void *areq, struct cc_hw_desc desc[],
				 unsigned int *seq_size)
{
	struct cc_cipher_ctx *ctx_p = crypto_tfm_ctx(tfm);
	struct device *dev = drvdata_to_dev(ctx_p->drvdata);
	unsigned int flow_mode = ctx_p->flow_mode;

	switch (ctx_p->flow_mode) {
	case S_DIN_to_AES:
		flow_mode = DIN_AES_DOUT;
		break;
	case S_DIN_to_DES:
		flow_mode = DIN_DES_DOUT;
		break;
	default:
		dev_err(dev, "invalid flow mode, flow_mode = %d\n", flow_mode);
		return;
	}
=======
static int cc_out_flow_mode(struct cc_cipher_ctx *ctx_p)
{
	switch (ctx_p->flow_mode) {
	case S_DIN_to_AES:
		return DIN_AES_DOUT;
	case S_DIN_to_DES:
		return DIN_DES_DOUT;
	case S_DIN_to_SM4:
		return DIN_SM4_DOUT;
	default:
		return ctx_p->flow_mode;
	}
}

static void cc_setup_key_desc(struct crypto_tfm *tfm,
			      struct cipher_req_ctx *req_ctx,
			      unsigned int nbytes, struct cc_hw_desc desc[],
			      unsigned int *seq_size)
{
	struct cc_cipher_ctx *ctx_p = crypto_tfm_ctx(tfm);
	struct device *dev = drvdata_to_dev(ctx_p->drvdata);
	int cipher_mode = ctx_p->cipher_mode;
	int flow_mode = ctx_p->flow_mode;
	int direction = req_ctx->gen_ctx.op_type;
	dma_addr_t key_dma_addr = ctx_p->user.key_dma_addr;
	unsigned int key_len = ctx_p->keylen;
	unsigned int din_size;

	switch (cipher_mode) {
	case DRV_CIPHER_CBC:
	case DRV_CIPHER_CBC_CTS:
	case DRV_CIPHER_CTR:
	case DRV_CIPHER_OFB:
	case DRV_CIPHER_ECB:
		/* Load key */
		hw_desc_init(&desc[*seq_size]);
		set_cipher_mode(&desc[*seq_size], cipher_mode);
		set_cipher_config0(&desc[*seq_size], direction);

		if (cc_key_type(tfm) == CC_POLICY_PROTECTED_KEY) {
			/* We use the AES key size coding for all CPP algs */
			set_key_size_aes(&desc[*seq_size], key_len);
			set_cpp_crypto_key(&desc[*seq_size], ctx_p->cpp.slot);
			flow_mode = cc_out_flow_mode(ctx_p);
		} else {
			if (flow_mode == S_DIN_to_AES) {
				if (cc_key_type(tfm) == CC_HW_PROTECTED_KEY) {
					set_hw_crypto_key(&desc[*seq_size],
							  ctx_p->hw.key1_slot);
				} else {
					/* CC_POLICY_UNPROTECTED_KEY
					 * Invalid keys are filtered out in
					 * sethkey()
					 */
					din_size = (key_len == 24) ?
						AES_MAX_KEY_SIZE : key_len;

					set_din_type(&desc[*seq_size], DMA_DLLI,
						     key_dma_addr, din_size,
						     NS_BIT);
				}
				set_key_size_aes(&desc[*seq_size], key_len);
			} else {
				/*des*/
				set_din_type(&desc[*seq_size], DMA_DLLI,
					     key_dma_addr, key_len, NS_BIT);
				set_key_size_des(&desc[*seq_size], key_len);
			}
			set_setup_mode(&desc[*seq_size], SETUP_LOAD_KEY0);
		}
		set_flow_mode(&desc[*seq_size], flow_mode);
		(*seq_size)++;
		break;
	case DRV_CIPHER_XTS:
	case DRV_CIPHER_ESSIV:
		/* Load AES key */
		hw_desc_init(&desc[*seq_size]);
		set_cipher_mode(&desc[*seq_size], cipher_mode);
		set_cipher_config0(&desc[*seq_size], direction);
		if (cc_key_type(tfm) == CC_HW_PROTECTED_KEY) {
			set_hw_crypto_key(&desc[*seq_size],
					  ctx_p->hw.key1_slot);
		} else {
			set_din_type(&desc[*seq_size], DMA_DLLI, key_dma_addr,
				     (key_len / 2), NS_BIT);
		}
		set_key_size_aes(&desc[*seq_size], (key_len / 2));
		set_flow_mode(&desc[*seq_size], flow_mode);
		set_setup_mode(&desc[*seq_size], SETUP_LOAD_KEY0);
		(*seq_size)++;
		break;
	default:
		dev_err(dev, "Unsupported cipher mode (%d)\n", cipher_mode);
	}
}

static void cc_setup_mlli_desc(struct crypto_tfm *tfm,
			       struct cipher_req_ctx *req_ctx,
			       struct scatterlist *dst, struct scatterlist *src,
			       unsigned int nbytes, void *areq,
			       struct cc_hw_desc desc[], unsigned int *seq_size)
{
	struct cc_cipher_ctx *ctx_p = crypto_tfm_ctx(tfm);
	struct device *dev = drvdata_to_dev(ctx_p->drvdata);

	if (req_ctx->dma_buf_type == CC_DMA_BUF_MLLI) {
		/* bypass */
		dev_dbg(dev, " bypass params addr %pad length 0x%X addr 0x%08X\n",
			&req_ctx->mlli_params.mlli_dma_addr,
			req_ctx->mlli_params.mlli_len,
			ctx_p->drvdata->mlli_sram_addr);
		hw_desc_init(&desc[*seq_size]);
		set_din_type(&desc[*seq_size], DMA_DLLI,
			     req_ctx->mlli_params.mlli_dma_addr,
			     req_ctx->mlli_params.mlli_len, NS_BIT);
		set_dout_sram(&desc[*seq_size],
			      ctx_p->drvdata->mlli_sram_addr,
			      req_ctx->mlli_params.mlli_len);
		set_flow_mode(&desc[*seq_size], BYPASS);
		(*seq_size)++;
	}
}

static void cc_setup_flow_desc(struct crypto_tfm *tfm,
			       struct cipher_req_ctx *req_ctx,
			       struct scatterlist *dst, struct scatterlist *src,
			       unsigned int nbytes, struct cc_hw_desc desc[],
			       unsigned int *seq_size)
{
	struct cc_cipher_ctx *ctx_p = crypto_tfm_ctx(tfm);
	struct device *dev = drvdata_to_dev(ctx_p->drvdata);
	unsigned int flow_mode = cc_out_flow_mode(ctx_p);
	bool last_desc = (ctx_p->key_type == CC_POLICY_PROTECTED_KEY ||
			  ctx_p->cipher_mode == DRV_CIPHER_ECB);

>>>>>>> upstream/android-13
	/* Process */
	if (req_ctx->dma_buf_type == CC_DMA_BUF_DLLI) {
		dev_dbg(dev, " data params addr %pad length 0x%X\n",
			&sg_dma_address(src), nbytes);
		dev_dbg(dev, " data params addr %pad length 0x%X\n",
			&sg_dma_address(dst), nbytes);
		hw_desc_init(&desc[*seq_size]);
		set_din_type(&desc[*seq_size], DMA_DLLI, sg_dma_address(src),
			     nbytes, NS_BIT);
		set_dout_dlli(&desc[*seq_size], sg_dma_address(dst),
<<<<<<< HEAD
			      nbytes, NS_BIT, (!areq ? 0 : 1));
		if (areq)
=======
			      nbytes, NS_BIT, (!last_desc ? 0 : 1));
		if (last_desc)
>>>>>>> upstream/android-13
			set_queue_last_ind(ctx_p->drvdata, &desc[*seq_size]);

		set_flow_mode(&desc[*seq_size], flow_mode);
		(*seq_size)++;
	} else {
<<<<<<< HEAD
		/* bypass */
		dev_dbg(dev, " bypass params addr %pad length 0x%X addr 0x%08X\n",
			&req_ctx->mlli_params.mlli_dma_addr,
			req_ctx->mlli_params.mlli_len,
			(unsigned int)ctx_p->drvdata->mlli_sram_addr);
		hw_desc_init(&desc[*seq_size]);
		set_din_type(&desc[*seq_size], DMA_DLLI,
			     req_ctx->mlli_params.mlli_dma_addr,
			     req_ctx->mlli_params.mlli_len, NS_BIT);
		set_dout_sram(&desc[*seq_size],
			      ctx_p->drvdata->mlli_sram_addr,
			      req_ctx->mlli_params.mlli_len);
		set_flow_mode(&desc[*seq_size], BYPASS);
		(*seq_size)++;

=======
>>>>>>> upstream/android-13
		hw_desc_init(&desc[*seq_size]);
		set_din_type(&desc[*seq_size], DMA_MLLI,
			     ctx_p->drvdata->mlli_sram_addr,
			     req_ctx->in_mlli_nents, NS_BIT);
		if (req_ctx->out_nents == 0) {
			dev_dbg(dev, " din/dout params addr 0x%08X addr 0x%08X\n",
<<<<<<< HEAD
				(unsigned int)ctx_p->drvdata->mlli_sram_addr,
				(unsigned int)ctx_p->drvdata->mlli_sram_addr);
			set_dout_mlli(&desc[*seq_size],
				      ctx_p->drvdata->mlli_sram_addr,
				      req_ctx->in_mlli_nents, NS_BIT,
				      (!areq ? 0 : 1));
		} else {
			dev_dbg(dev, " din/dout params addr 0x%08X addr 0x%08X\n",
				(unsigned int)ctx_p->drvdata->mlli_sram_addr,
				(unsigned int)ctx_p->drvdata->mlli_sram_addr +
=======
				ctx_p->drvdata->mlli_sram_addr,
				ctx_p->drvdata->mlli_sram_addr);
			set_dout_mlli(&desc[*seq_size],
				      ctx_p->drvdata->mlli_sram_addr,
				      req_ctx->in_mlli_nents, NS_BIT,
				      (!last_desc ? 0 : 1));
		} else {
			dev_dbg(dev, " din/dout params addr 0x%08X addr 0x%08X\n",
				ctx_p->drvdata->mlli_sram_addr,
				ctx_p->drvdata->mlli_sram_addr +
>>>>>>> upstream/android-13
				(u32)LLI_ENTRY_BYTE_SIZE * req_ctx->in_nents);
			set_dout_mlli(&desc[*seq_size],
				      (ctx_p->drvdata->mlli_sram_addr +
				       (LLI_ENTRY_BYTE_SIZE *
					req_ctx->in_mlli_nents)),
				      req_ctx->out_mlli_nents, NS_BIT,
<<<<<<< HEAD
				      (!areq ? 0 : 1));
		}
		if (areq)
=======
				      (!last_desc ? 0 : 1));
		}
		if (last_desc)
>>>>>>> upstream/android-13
			set_queue_last_ind(ctx_p->drvdata, &desc[*seq_size]);

		set_flow_mode(&desc[*seq_size], flow_mode);
		(*seq_size)++;
	}
}

<<<<<<< HEAD
/*
 * Update a CTR-AES 128 bit counter
 */
static void cc_update_ctr(u8 *ctr, unsigned int increment)
{
	if (IS_ENABLED(CONFIG_HAVE_EFFICIENT_UNALIGNED_ACCESS) ||
	    IS_ALIGNED((unsigned long)ctr, 8)) {

		__be64 *high_be = (__be64 *)ctr;
		__be64 *low_be = high_be + 1;
		u64 orig_low = __be64_to_cpu(*low_be);
		u64 new_low = orig_low + (u64)increment;

		*low_be = __cpu_to_be64(new_low);

		if (new_low < orig_low)
			*high_be = __cpu_to_be64(__be64_to_cpu(*high_be) + 1);
	} else {
		u8 *pos = (ctr + AES_BLOCK_SIZE);
		u8 val;
		unsigned int size;

		for (; increment; increment--)
			for (size = AES_BLOCK_SIZE; size; size--) {
				val = *--pos + 1;
				*pos = val;
				if (val)
					break;
			}
	}
}

=======
>>>>>>> upstream/android-13
static void cc_cipher_complete(struct device *dev, void *cc_req, int err)
{
	struct skcipher_request *req = (struct skcipher_request *)cc_req;
	struct scatterlist *dst = req->dst;
	struct scatterlist *src = req->src;
	struct cipher_req_ctx *req_ctx = skcipher_request_ctx(req);
	struct crypto_skcipher *sk_tfm = crypto_skcipher_reqtfm(req);
<<<<<<< HEAD
	struct crypto_tfm *tfm = crypto_skcipher_tfm(sk_tfm);
	struct cc_cipher_ctx *ctx_p = crypto_tfm_ctx(tfm);
	unsigned int ivsize = crypto_skcipher_ivsize(sk_tfm);
	unsigned int len;

	cc_unmap_cipher_request(dev, req_ctx, ivsize, src, dst);

	switch (ctx_p->cipher_mode) {
	case DRV_CIPHER_CBC:
		/*
		 * The crypto API expects us to set the req->iv to the last
		 * ciphertext block. For encrypt, simply copy from the result.
		 * For decrypt, we must copy from a saved buffer since this
		 * could be an in-place decryption operation and the src is
		 * lost by this point.
		 */
		if (req_ctx->gen_ctx.op_type == DRV_CRYPTO_DIRECTION_DECRYPT)  {
			memcpy(req->iv, req_ctx->backup_info, ivsize);
			kzfree(req_ctx->backup_info);
		} else if (!err) {
			len = req->cryptlen - ivsize;
			scatterwalk_map_and_copy(req->iv, req->dst, len,
						 ivsize, 0);
		}
		break;

	case DRV_CIPHER_CTR:
		/* Compute the counter of the last block */
		len = ALIGN(req->cryptlen, AES_BLOCK_SIZE) / AES_BLOCK_SIZE;
		cc_update_ctr((u8 *)req->iv, len);
		break;

	default:
		break;
	}

	kzfree(req_ctx->iv);

=======
	unsigned int ivsize = crypto_skcipher_ivsize(sk_tfm);

	if (err != -EINPROGRESS) {
		/* Not a BACKLOG notification */
		cc_unmap_cipher_request(dev, req_ctx, ivsize, src, dst);
		memcpy(req->iv, req_ctx->iv, ivsize);
		kfree_sensitive(req_ctx->iv);
	}

>>>>>>> upstream/android-13
	skcipher_request_complete(req, err);
}

static int cc_cipher_process(struct skcipher_request *req,
			     enum drv_crypto_direction direction)
{
	struct crypto_skcipher *sk_tfm = crypto_skcipher_reqtfm(req);
	struct crypto_tfm *tfm = crypto_skcipher_tfm(sk_tfm);
	struct cipher_req_ctx *req_ctx = skcipher_request_ctx(req);
	unsigned int ivsize = crypto_skcipher_ivsize(sk_tfm);
	struct scatterlist *dst = req->dst;
	struct scatterlist *src = req->src;
	unsigned int nbytes = req->cryptlen;
	void *iv = req->iv;
	struct cc_cipher_ctx *ctx_p = crypto_tfm_ctx(tfm);
	struct device *dev = drvdata_to_dev(ctx_p->drvdata);
<<<<<<< HEAD
	struct cc_hw_desc desc[MAX_ABLKCIPHER_SEQ_LEN];
=======
	struct cc_hw_desc desc[MAX_SKCIPHER_SEQ_LEN];
>>>>>>> upstream/android-13
	struct cc_crypto_req cc_req = {};
	int rc;
	unsigned int seq_len = 0;
	gfp_t flags = cc_gfp_flags(&req->base);

	dev_dbg(dev, "%s req=%p iv=%p nbytes=%d\n",
		((direction == DRV_CRYPTO_DIRECTION_ENCRYPT) ?
		"Encrypt" : "Decrypt"), req, iv, nbytes);

	/* STAT_PHASE_0: Init and sanity checks */

<<<<<<< HEAD
	/* TODO: check data length according to mode */
	if (validate_data_size(ctx_p, nbytes)) {
		dev_err(dev, "Unsupported data size %d.\n", nbytes);
		crypto_tfm_set_flags(tfm, CRYPTO_TFM_RES_BAD_BLOCK_LEN);
=======
	if (validate_data_size(ctx_p, nbytes)) {
		dev_dbg(dev, "Unsupported data size %d.\n", nbytes);
>>>>>>> upstream/android-13
		rc = -EINVAL;
		goto exit_process;
	}
	if (nbytes == 0) {
		/* No data to process is valid */
		rc = 0;
		goto exit_process;
	}

<<<<<<< HEAD
	/* The IV we are handed may be allocted from the stack so
=======
	if (ctx_p->fallback_on) {
		struct skcipher_request *subreq = skcipher_request_ctx(req);

		*subreq = *req;
		skcipher_request_set_tfm(subreq, ctx_p->fallback_tfm);
		if (direction == DRV_CRYPTO_DIRECTION_ENCRYPT)
			return crypto_skcipher_encrypt(subreq);
		else
			return crypto_skcipher_decrypt(subreq);
	}

	/* The IV we are handed may be allocated from the stack so
>>>>>>> upstream/android-13
	 * we must copy it to a DMAable buffer before use.
	 */
	req_ctx->iv = kmemdup(iv, ivsize, flags);
	if (!req_ctx->iv) {
		rc = -ENOMEM;
		goto exit_process;
	}

	/* Setup request structure */
<<<<<<< HEAD
	cc_req.user_cb = (void *)cc_cipher_complete;
	cc_req.user_arg = (void *)req;
=======
	cc_req.user_cb = cc_cipher_complete;
	cc_req.user_arg = req;

	/* Setup CPP operation details */
	if (ctx_p->key_type == CC_POLICY_PROTECTED_KEY) {
		cc_req.cpp.is_cpp = true;
		cc_req.cpp.alg = ctx_p->cpp.alg;
		cc_req.cpp.slot = ctx_p->cpp.slot;
	}
>>>>>>> upstream/android-13

	/* Setup request context */
	req_ctx->gen_ctx.op_type = direction;

	/* STAT_PHASE_1: Map buffers */

	rc = cc_map_cipher_request(ctx_p->drvdata, req_ctx, ivsize, nbytes,
				      req_ctx->iv, src, dst, flags);
	if (rc) {
		dev_err(dev, "map_request() failed\n");
		goto exit_process;
	}

	/* STAT_PHASE_2: Create sequence */

<<<<<<< HEAD
	/* Setup processing */
	cc_setup_cipher_desc(tfm, req_ctx, ivsize, nbytes, desc, &seq_len);
	/* Data processing */
	cc_setup_cipher_data(tfm, req_ctx, dst, src, nbytes, req, desc,
			     &seq_len);
=======
	/* Setup state (IV)  */
	cc_setup_state_desc(tfm, req_ctx, ivsize, nbytes, desc, &seq_len);
	/* Setup MLLI line, if needed */
	cc_setup_mlli_desc(tfm, req_ctx, dst, src, nbytes, req, desc, &seq_len);
	/* Setup key */
	cc_setup_key_desc(tfm, req_ctx, nbytes, desc, &seq_len);
	/* Setup state (IV and XEX key)  */
	cc_setup_xex_state_desc(tfm, req_ctx, ivsize, nbytes, desc, &seq_len);
	/* Data processing */
	cc_setup_flow_desc(tfm, req_ctx, dst, src, nbytes, desc, &seq_len);
	/* Read next IV */
	cc_setup_readiv_desc(tfm, req_ctx, ivsize, desc, &seq_len);
>>>>>>> upstream/android-13

	/* STAT_PHASE_3: Lock HW and push sequence */

	rc = cc_send_request(ctx_p->drvdata, &cc_req, desc, seq_len,
			     &req->base);
	if (rc != -EINPROGRESS && rc != -EBUSY) {
		/* Failed to send the request or request completed
		 * synchronously
		 */
		cc_unmap_cipher_request(dev, req_ctx, ivsize, src, dst);
	}

exit_process:
	if (rc != -EINPROGRESS && rc != -EBUSY) {
<<<<<<< HEAD
		kzfree(req_ctx->backup_info);
		kzfree(req_ctx->iv);
=======
		kfree_sensitive(req_ctx->iv);
>>>>>>> upstream/android-13
	}

	return rc;
}

static int cc_cipher_encrypt(struct skcipher_request *req)
{
	struct cipher_req_ctx *req_ctx = skcipher_request_ctx(req);

	memset(req_ctx, 0, sizeof(*req_ctx));

	return cc_cipher_process(req, DRV_CRYPTO_DIRECTION_ENCRYPT);
}

static int cc_cipher_decrypt(struct skcipher_request *req)
{
<<<<<<< HEAD
	struct crypto_skcipher *sk_tfm = crypto_skcipher_reqtfm(req);
	struct crypto_tfm *tfm = crypto_skcipher_tfm(sk_tfm);
	struct cc_cipher_ctx *ctx_p = crypto_tfm_ctx(tfm);
	struct cipher_req_ctx *req_ctx = skcipher_request_ctx(req);
	unsigned int ivsize = crypto_skcipher_ivsize(sk_tfm);
	gfp_t flags = cc_gfp_flags(&req->base);
	unsigned int len;

	memset(req_ctx, 0, sizeof(*req_ctx));

	if ((ctx_p->cipher_mode == DRV_CIPHER_CBC) &&
	    (req->cryptlen >= ivsize)) {

		/* Allocate and save the last IV sized bytes of the source,
		 * which will be lost in case of in-place decryption.
		 */
		req_ctx->backup_info = kzalloc(ivsize, flags);
		if (!req_ctx->backup_info)
			return -ENOMEM;

		len = req->cryptlen - ivsize;
		scatterwalk_map_and_copy(req_ctx->backup_info, req->src, len,
					 ivsize, 0);
	}

=======
	struct cipher_req_ctx *req_ctx = skcipher_request_ctx(req);

	memset(req_ctx, 0, sizeof(*req_ctx));

>>>>>>> upstream/android-13
	return cc_cipher_process(req, DRV_CRYPTO_DIRECTION_DECRYPT);
}

/* Block cipher alg */
static const struct cc_alg_template skcipher_algs[] = {
	{
		.name = "xts(paes)",
		.driver_name = "xts-paes-ccree",
<<<<<<< HEAD
		.blocksize = AES_BLOCK_SIZE,
=======
		.blocksize = 1,
>>>>>>> upstream/android-13
		.template_skcipher = {
			.setkey = cc_cipher_sethkey,
			.encrypt = cc_cipher_encrypt,
			.decrypt = cc_cipher_decrypt,
			.min_keysize = CC_HW_KEY_SIZE,
			.max_keysize = CC_HW_KEY_SIZE,
			.ivsize = AES_BLOCK_SIZE,
			},
		.cipher_mode = DRV_CIPHER_XTS,
		.flow_mode = S_DIN_to_AES,
		.min_hw_rev = CC_HW_REV_630,
<<<<<<< HEAD
	},
	{
		.name = "xts512(paes)",
		.driver_name = "xts-paes-du512-ccree",
		.blocksize = AES_BLOCK_SIZE,
		.template_skcipher = {
			.setkey = cc_cipher_sethkey,
			.encrypt = cc_cipher_encrypt,
			.decrypt = cc_cipher_decrypt,
			.min_keysize = CC_HW_KEY_SIZE,
			.max_keysize = CC_HW_KEY_SIZE,
			.ivsize = AES_BLOCK_SIZE,
			},
		.cipher_mode = DRV_CIPHER_XTS,
		.flow_mode = S_DIN_to_AES,
		.data_unit = 512,
		.min_hw_rev = CC_HW_REV_712,
	},
	{
		.name = "xts4096(paes)",
		.driver_name = "xts-paes-du4096-ccree",
		.blocksize = AES_BLOCK_SIZE,
		.template_skcipher = {
			.setkey = cc_cipher_sethkey,
			.encrypt = cc_cipher_encrypt,
			.decrypt = cc_cipher_decrypt,
			.min_keysize = CC_HW_KEY_SIZE,
			.max_keysize = CC_HW_KEY_SIZE,
			.ivsize = AES_BLOCK_SIZE,
			},
		.cipher_mode = DRV_CIPHER_XTS,
		.flow_mode = S_DIN_to_AES,
		.data_unit = 4096,
		.min_hw_rev = CC_HW_REV_712,
	},
	{
		.name = "essiv(paes)",
=======
		.std_body = CC_STD_NIST,
		.sec_func = true,
	},
	{
		.name = "essiv(cbc(paes),sha256)",
>>>>>>> upstream/android-13
		.driver_name = "essiv-paes-ccree",
		.blocksize = AES_BLOCK_SIZE,
		.template_skcipher = {
			.setkey = cc_cipher_sethkey,
			.encrypt = cc_cipher_encrypt,
			.decrypt = cc_cipher_decrypt,
			.min_keysize = CC_HW_KEY_SIZE,
			.max_keysize = CC_HW_KEY_SIZE,
			.ivsize = AES_BLOCK_SIZE,
			},
		.cipher_mode = DRV_CIPHER_ESSIV,
		.flow_mode = S_DIN_to_AES,
		.min_hw_rev = CC_HW_REV_712,
<<<<<<< HEAD
	},
	{
		.name = "essiv512(paes)",
		.driver_name = "essiv-paes-du512-ccree",
		.blocksize = AES_BLOCK_SIZE,
		.template_skcipher = {
			.setkey = cc_cipher_sethkey,
			.encrypt = cc_cipher_encrypt,
			.decrypt = cc_cipher_decrypt,
			.min_keysize = CC_HW_KEY_SIZE,
			.max_keysize = CC_HW_KEY_SIZE,
			.ivsize = AES_BLOCK_SIZE,
			},
		.cipher_mode = DRV_CIPHER_ESSIV,
		.flow_mode = S_DIN_to_AES,
		.data_unit = 512,
		.min_hw_rev = CC_HW_REV_712,
	},
	{
		.name = "essiv4096(paes)",
		.driver_name = "essiv-paes-du4096-ccree",
		.blocksize = AES_BLOCK_SIZE,
		.template_skcipher = {
			.setkey = cc_cipher_sethkey,
			.encrypt = cc_cipher_encrypt,
			.decrypt = cc_cipher_decrypt,
			.min_keysize = CC_HW_KEY_SIZE,
			.max_keysize = CC_HW_KEY_SIZE,
			.ivsize = AES_BLOCK_SIZE,
			},
		.cipher_mode = DRV_CIPHER_ESSIV,
		.flow_mode = S_DIN_to_AES,
		.data_unit = 4096,
		.min_hw_rev = CC_HW_REV_712,
	},
	{
		.name = "bitlocker(paes)",
		.driver_name = "bitlocker-paes-ccree",
		.blocksize = AES_BLOCK_SIZE,
		.template_skcipher = {
			.setkey = cc_cipher_sethkey,
			.encrypt = cc_cipher_encrypt,
			.decrypt = cc_cipher_decrypt,
			.min_keysize = CC_HW_KEY_SIZE,
			.max_keysize = CC_HW_KEY_SIZE,
			.ivsize = AES_BLOCK_SIZE,
			},
		.cipher_mode = DRV_CIPHER_BITLOCKER,
		.flow_mode = S_DIN_to_AES,
		.min_hw_rev = CC_HW_REV_712,
	},
	{
		.name = "bitlocker512(paes)",
		.driver_name = "bitlocker-paes-du512-ccree",
		.blocksize = AES_BLOCK_SIZE,
		.template_skcipher = {
			.setkey = cc_cipher_sethkey,
			.encrypt = cc_cipher_encrypt,
			.decrypt = cc_cipher_decrypt,
			.min_keysize = CC_HW_KEY_SIZE,
			.max_keysize = CC_HW_KEY_SIZE,
			.ivsize = AES_BLOCK_SIZE,
			},
		.cipher_mode = DRV_CIPHER_BITLOCKER,
		.flow_mode = S_DIN_to_AES,
		.data_unit = 512,
		.min_hw_rev = CC_HW_REV_712,
	},
	{
		.name = "bitlocker4096(paes)",
		.driver_name = "bitlocker-paes-du4096-ccree",
		.blocksize = AES_BLOCK_SIZE,
		.template_skcipher = {
			.setkey = cc_cipher_sethkey,
			.encrypt = cc_cipher_encrypt,
			.decrypt = cc_cipher_decrypt,
			.min_keysize = CC_HW_KEY_SIZE,
			.max_keysize =  CC_HW_KEY_SIZE,
			.ivsize = AES_BLOCK_SIZE,
			},
		.cipher_mode = DRV_CIPHER_BITLOCKER,
		.flow_mode = S_DIN_to_AES,
		.data_unit = 4096,
		.min_hw_rev = CC_HW_REV_712,
=======
		.std_body = CC_STD_NIST,
		.sec_func = true,
>>>>>>> upstream/android-13
	},
	{
		.name = "ecb(paes)",
		.driver_name = "ecb-paes-ccree",
		.blocksize = AES_BLOCK_SIZE,
		.template_skcipher = {
			.setkey = cc_cipher_sethkey,
			.encrypt = cc_cipher_encrypt,
			.decrypt = cc_cipher_decrypt,
			.min_keysize = CC_HW_KEY_SIZE,
			.max_keysize = CC_HW_KEY_SIZE,
			.ivsize = 0,
			},
		.cipher_mode = DRV_CIPHER_ECB,
		.flow_mode = S_DIN_to_AES,
		.min_hw_rev = CC_HW_REV_712,
<<<<<<< HEAD
=======
		.std_body = CC_STD_NIST,
		.sec_func = true,
>>>>>>> upstream/android-13
	},
	{
		.name = "cbc(paes)",
		.driver_name = "cbc-paes-ccree",
		.blocksize = AES_BLOCK_SIZE,
		.template_skcipher = {
			.setkey = cc_cipher_sethkey,
			.encrypt = cc_cipher_encrypt,
			.decrypt = cc_cipher_decrypt,
			.min_keysize = CC_HW_KEY_SIZE,
			.max_keysize = CC_HW_KEY_SIZE,
			.ivsize = AES_BLOCK_SIZE,
		},
		.cipher_mode = DRV_CIPHER_CBC,
		.flow_mode = S_DIN_to_AES,
		.min_hw_rev = CC_HW_REV_712,
<<<<<<< HEAD
=======
		.std_body = CC_STD_NIST,
		.sec_func = true,
>>>>>>> upstream/android-13
	},
	{
		.name = "ofb(paes)",
		.driver_name = "ofb-paes-ccree",
		.blocksize = AES_BLOCK_SIZE,
		.template_skcipher = {
			.setkey = cc_cipher_sethkey,
			.encrypt = cc_cipher_encrypt,
			.decrypt = cc_cipher_decrypt,
			.min_keysize = CC_HW_KEY_SIZE,
			.max_keysize = CC_HW_KEY_SIZE,
			.ivsize = AES_BLOCK_SIZE,
			},
		.cipher_mode = DRV_CIPHER_OFB,
		.flow_mode = S_DIN_to_AES,
		.min_hw_rev = CC_HW_REV_712,
<<<<<<< HEAD
=======
		.std_body = CC_STD_NIST,
		.sec_func = true,
>>>>>>> upstream/android-13
	},
	{
		.name = "cts(cbc(paes))",
		.driver_name = "cts-cbc-paes-ccree",
		.blocksize = AES_BLOCK_SIZE,
		.template_skcipher = {
			.setkey = cc_cipher_sethkey,
			.encrypt = cc_cipher_encrypt,
			.decrypt = cc_cipher_decrypt,
			.min_keysize = CC_HW_KEY_SIZE,
			.max_keysize = CC_HW_KEY_SIZE,
			.ivsize = AES_BLOCK_SIZE,
			},
		.cipher_mode = DRV_CIPHER_CBC_CTS,
		.flow_mode = S_DIN_to_AES,
		.min_hw_rev = CC_HW_REV_712,
<<<<<<< HEAD
=======
		.std_body = CC_STD_NIST,
		.sec_func = true,
>>>>>>> upstream/android-13
	},
	{
		.name = "ctr(paes)",
		.driver_name = "ctr-paes-ccree",
		.blocksize = 1,
		.template_skcipher = {
			.setkey = cc_cipher_sethkey,
			.encrypt = cc_cipher_encrypt,
			.decrypt = cc_cipher_decrypt,
			.min_keysize = CC_HW_KEY_SIZE,
			.max_keysize = CC_HW_KEY_SIZE,
			.ivsize = AES_BLOCK_SIZE,
			},
		.cipher_mode = DRV_CIPHER_CTR,
		.flow_mode = S_DIN_to_AES,
		.min_hw_rev = CC_HW_REV_712,
<<<<<<< HEAD
	},
	{
		.name = "xts(aes)",
		.driver_name = "xts-aes-ccree",
		.blocksize = AES_BLOCK_SIZE,
=======
		.std_body = CC_STD_NIST,
		.sec_func = true,
	},
	{
		/* See https://www.mail-archive.com/linux-crypto@vger.kernel.org/msg40576.html
		 * for the reason why this differs from the generic
		 * implementation.
		 */
		.name = "xts(aes)",
		.driver_name = "xts-aes-ccree",
		.blocksize = 1,
>>>>>>> upstream/android-13
		.template_skcipher = {
			.setkey = cc_cipher_setkey,
			.encrypt = cc_cipher_encrypt,
			.decrypt = cc_cipher_decrypt,
			.min_keysize = AES_MIN_KEY_SIZE * 2,
			.max_keysize = AES_MAX_KEY_SIZE * 2,
			.ivsize = AES_BLOCK_SIZE,
			},
		.cipher_mode = DRV_CIPHER_XTS,
		.flow_mode = S_DIN_to_AES,
		.min_hw_rev = CC_HW_REV_630,
<<<<<<< HEAD
	},
	{
		.name = "xts512(aes)",
		.driver_name = "xts-aes-du512-ccree",
		.blocksize = AES_BLOCK_SIZE,
		.template_skcipher = {
			.setkey = cc_cipher_setkey,
			.encrypt = cc_cipher_encrypt,
			.decrypt = cc_cipher_decrypt,
			.min_keysize = AES_MIN_KEY_SIZE * 2,
			.max_keysize = AES_MAX_KEY_SIZE * 2,
			.ivsize = AES_BLOCK_SIZE,
			},
		.cipher_mode = DRV_CIPHER_XTS,
		.flow_mode = S_DIN_to_AES,
		.data_unit = 512,
		.min_hw_rev = CC_HW_REV_712,
	},
	{
		.name = "xts4096(aes)",
		.driver_name = "xts-aes-du4096-ccree",
		.blocksize = AES_BLOCK_SIZE,
		.template_skcipher = {
			.setkey = cc_cipher_setkey,
			.encrypt = cc_cipher_encrypt,
			.decrypt = cc_cipher_decrypt,
			.min_keysize = AES_MIN_KEY_SIZE * 2,
			.max_keysize = AES_MAX_KEY_SIZE * 2,
			.ivsize = AES_BLOCK_SIZE,
			},
		.cipher_mode = DRV_CIPHER_XTS,
		.flow_mode = S_DIN_to_AES,
		.data_unit = 4096,
		.min_hw_rev = CC_HW_REV_712,
	},
	{
		.name = "essiv(aes)",
=======
		.std_body = CC_STD_NIST,
	},
	{
		.name = "essiv(cbc(aes),sha256)",
>>>>>>> upstream/android-13
		.driver_name = "essiv-aes-ccree",
		.blocksize = AES_BLOCK_SIZE,
		.template_skcipher = {
			.setkey = cc_cipher_setkey,
			.encrypt = cc_cipher_encrypt,
			.decrypt = cc_cipher_decrypt,
<<<<<<< HEAD
			.min_keysize = AES_MIN_KEY_SIZE * 2,
			.max_keysize = AES_MAX_KEY_SIZE * 2,
=======
			.min_keysize = AES_MIN_KEY_SIZE,
			.max_keysize = AES_MAX_KEY_SIZE,
>>>>>>> upstream/android-13
			.ivsize = AES_BLOCK_SIZE,
			},
		.cipher_mode = DRV_CIPHER_ESSIV,
		.flow_mode = S_DIN_to_AES,
		.min_hw_rev = CC_HW_REV_712,
<<<<<<< HEAD
	},
	{
		.name = "essiv512(aes)",
		.driver_name = "essiv-aes-du512-ccree",
		.blocksize = AES_BLOCK_SIZE,
		.template_skcipher = {
			.setkey = cc_cipher_setkey,
			.encrypt = cc_cipher_encrypt,
			.decrypt = cc_cipher_decrypt,
			.min_keysize = AES_MIN_KEY_SIZE * 2,
			.max_keysize = AES_MAX_KEY_SIZE * 2,
			.ivsize = AES_BLOCK_SIZE,
			},
		.cipher_mode = DRV_CIPHER_ESSIV,
		.flow_mode = S_DIN_to_AES,
		.data_unit = 512,
		.min_hw_rev = CC_HW_REV_712,
	},
	{
		.name = "essiv4096(aes)",
		.driver_name = "essiv-aes-du4096-ccree",
		.blocksize = AES_BLOCK_SIZE,
		.template_skcipher = {
			.setkey = cc_cipher_setkey,
			.encrypt = cc_cipher_encrypt,
			.decrypt = cc_cipher_decrypt,
			.min_keysize = AES_MIN_KEY_SIZE * 2,
			.max_keysize = AES_MAX_KEY_SIZE * 2,
			.ivsize = AES_BLOCK_SIZE,
			},
		.cipher_mode = DRV_CIPHER_ESSIV,
		.flow_mode = S_DIN_to_AES,
		.data_unit = 4096,
		.min_hw_rev = CC_HW_REV_712,
	},
	{
		.name = "bitlocker(aes)",
		.driver_name = "bitlocker-aes-ccree",
		.blocksize = AES_BLOCK_SIZE,
		.template_skcipher = {
			.setkey = cc_cipher_setkey,
			.encrypt = cc_cipher_encrypt,
			.decrypt = cc_cipher_decrypt,
			.min_keysize = AES_MIN_KEY_SIZE * 2,
			.max_keysize = AES_MAX_KEY_SIZE * 2,
			.ivsize = AES_BLOCK_SIZE,
			},
		.cipher_mode = DRV_CIPHER_BITLOCKER,
		.flow_mode = S_DIN_to_AES,
		.min_hw_rev = CC_HW_REV_712,
	},
	{
		.name = "bitlocker512(aes)",
		.driver_name = "bitlocker-aes-du512-ccree",
		.blocksize = AES_BLOCK_SIZE,
		.template_skcipher = {
			.setkey = cc_cipher_setkey,
			.encrypt = cc_cipher_encrypt,
			.decrypt = cc_cipher_decrypt,
			.min_keysize = AES_MIN_KEY_SIZE * 2,
			.max_keysize = AES_MAX_KEY_SIZE * 2,
			.ivsize = AES_BLOCK_SIZE,
			},
		.cipher_mode = DRV_CIPHER_BITLOCKER,
		.flow_mode = S_DIN_to_AES,
		.data_unit = 512,
		.min_hw_rev = CC_HW_REV_712,
	},
	{
		.name = "bitlocker4096(aes)",
		.driver_name = "bitlocker-aes-du4096-ccree",
		.blocksize = AES_BLOCK_SIZE,
		.template_skcipher = {
			.setkey = cc_cipher_setkey,
			.encrypt = cc_cipher_encrypt,
			.decrypt = cc_cipher_decrypt,
			.min_keysize = AES_MIN_KEY_SIZE * 2,
			.max_keysize = AES_MAX_KEY_SIZE * 2,
			.ivsize = AES_BLOCK_SIZE,
			},
		.cipher_mode = DRV_CIPHER_BITLOCKER,
		.flow_mode = S_DIN_to_AES,
		.data_unit = 4096,
		.min_hw_rev = CC_HW_REV_712,
=======
		.std_body = CC_STD_NIST,
>>>>>>> upstream/android-13
	},
	{
		.name = "ecb(aes)",
		.driver_name = "ecb-aes-ccree",
		.blocksize = AES_BLOCK_SIZE,
		.template_skcipher = {
			.setkey = cc_cipher_setkey,
			.encrypt = cc_cipher_encrypt,
			.decrypt = cc_cipher_decrypt,
			.min_keysize = AES_MIN_KEY_SIZE,
			.max_keysize = AES_MAX_KEY_SIZE,
			.ivsize = 0,
			},
		.cipher_mode = DRV_CIPHER_ECB,
		.flow_mode = S_DIN_to_AES,
		.min_hw_rev = CC_HW_REV_630,
<<<<<<< HEAD
=======
		.std_body = CC_STD_NIST,
>>>>>>> upstream/android-13
	},
	{
		.name = "cbc(aes)",
		.driver_name = "cbc-aes-ccree",
		.blocksize = AES_BLOCK_SIZE,
		.template_skcipher = {
			.setkey = cc_cipher_setkey,
			.encrypt = cc_cipher_encrypt,
			.decrypt = cc_cipher_decrypt,
			.min_keysize = AES_MIN_KEY_SIZE,
			.max_keysize = AES_MAX_KEY_SIZE,
			.ivsize = AES_BLOCK_SIZE,
		},
		.cipher_mode = DRV_CIPHER_CBC,
		.flow_mode = S_DIN_to_AES,
		.min_hw_rev = CC_HW_REV_630,
<<<<<<< HEAD
=======
		.std_body = CC_STD_NIST,
>>>>>>> upstream/android-13
	},
	{
		.name = "ofb(aes)",
		.driver_name = "ofb-aes-ccree",
<<<<<<< HEAD
		.blocksize = AES_BLOCK_SIZE,
=======
		.blocksize = 1,
>>>>>>> upstream/android-13
		.template_skcipher = {
			.setkey = cc_cipher_setkey,
			.encrypt = cc_cipher_encrypt,
			.decrypt = cc_cipher_decrypt,
			.min_keysize = AES_MIN_KEY_SIZE,
			.max_keysize = AES_MAX_KEY_SIZE,
			.ivsize = AES_BLOCK_SIZE,
			},
		.cipher_mode = DRV_CIPHER_OFB,
		.flow_mode = S_DIN_to_AES,
		.min_hw_rev = CC_HW_REV_630,
<<<<<<< HEAD
=======
		.std_body = CC_STD_NIST,
>>>>>>> upstream/android-13
	},
	{
		.name = "cts(cbc(aes))",
		.driver_name = "cts-cbc-aes-ccree",
		.blocksize = AES_BLOCK_SIZE,
		.template_skcipher = {
			.setkey = cc_cipher_setkey,
			.encrypt = cc_cipher_encrypt,
			.decrypt = cc_cipher_decrypt,
			.min_keysize = AES_MIN_KEY_SIZE,
			.max_keysize = AES_MAX_KEY_SIZE,
			.ivsize = AES_BLOCK_SIZE,
			},
		.cipher_mode = DRV_CIPHER_CBC_CTS,
		.flow_mode = S_DIN_to_AES,
		.min_hw_rev = CC_HW_REV_630,
<<<<<<< HEAD
=======
		.std_body = CC_STD_NIST,
>>>>>>> upstream/android-13
	},
	{
		.name = "ctr(aes)",
		.driver_name = "ctr-aes-ccree",
		.blocksize = 1,
		.template_skcipher = {
			.setkey = cc_cipher_setkey,
			.encrypt = cc_cipher_encrypt,
			.decrypt = cc_cipher_decrypt,
			.min_keysize = AES_MIN_KEY_SIZE,
			.max_keysize = AES_MAX_KEY_SIZE,
			.ivsize = AES_BLOCK_SIZE,
			},
		.cipher_mode = DRV_CIPHER_CTR,
		.flow_mode = S_DIN_to_AES,
		.min_hw_rev = CC_HW_REV_630,
<<<<<<< HEAD
=======
		.std_body = CC_STD_NIST,
>>>>>>> upstream/android-13
	},
	{
		.name = "cbc(des3_ede)",
		.driver_name = "cbc-3des-ccree",
		.blocksize = DES3_EDE_BLOCK_SIZE,
		.template_skcipher = {
			.setkey = cc_cipher_setkey,
			.encrypt = cc_cipher_encrypt,
			.decrypt = cc_cipher_decrypt,
			.min_keysize = DES3_EDE_KEY_SIZE,
			.max_keysize = DES3_EDE_KEY_SIZE,
			.ivsize = DES3_EDE_BLOCK_SIZE,
			},
		.cipher_mode = DRV_CIPHER_CBC,
		.flow_mode = S_DIN_to_DES,
		.min_hw_rev = CC_HW_REV_630,
<<<<<<< HEAD
=======
		.std_body = CC_STD_NIST,
>>>>>>> upstream/android-13
	},
	{
		.name = "ecb(des3_ede)",
		.driver_name = "ecb-3des-ccree",
		.blocksize = DES3_EDE_BLOCK_SIZE,
		.template_skcipher = {
			.setkey = cc_cipher_setkey,
			.encrypt = cc_cipher_encrypt,
			.decrypt = cc_cipher_decrypt,
			.min_keysize = DES3_EDE_KEY_SIZE,
			.max_keysize = DES3_EDE_KEY_SIZE,
			.ivsize = 0,
			},
		.cipher_mode = DRV_CIPHER_ECB,
		.flow_mode = S_DIN_to_DES,
		.min_hw_rev = CC_HW_REV_630,
<<<<<<< HEAD
=======
		.std_body = CC_STD_NIST,
>>>>>>> upstream/android-13
	},
	{
		.name = "cbc(des)",
		.driver_name = "cbc-des-ccree",
		.blocksize = DES_BLOCK_SIZE,
		.template_skcipher = {
			.setkey = cc_cipher_setkey,
			.encrypt = cc_cipher_encrypt,
			.decrypt = cc_cipher_decrypt,
			.min_keysize = DES_KEY_SIZE,
			.max_keysize = DES_KEY_SIZE,
			.ivsize = DES_BLOCK_SIZE,
			},
		.cipher_mode = DRV_CIPHER_CBC,
		.flow_mode = S_DIN_to_DES,
		.min_hw_rev = CC_HW_REV_630,
<<<<<<< HEAD
=======
		.std_body = CC_STD_NIST,
>>>>>>> upstream/android-13
	},
	{
		.name = "ecb(des)",
		.driver_name = "ecb-des-ccree",
		.blocksize = DES_BLOCK_SIZE,
		.template_skcipher = {
			.setkey = cc_cipher_setkey,
			.encrypt = cc_cipher_encrypt,
			.decrypt = cc_cipher_decrypt,
			.min_keysize = DES_KEY_SIZE,
			.max_keysize = DES_KEY_SIZE,
			.ivsize = 0,
			},
		.cipher_mode = DRV_CIPHER_ECB,
		.flow_mode = S_DIN_to_DES,
		.min_hw_rev = CC_HW_REV_630,
<<<<<<< HEAD
=======
		.std_body = CC_STD_NIST,
	},
	{
		.name = "cbc(sm4)",
		.driver_name = "cbc-sm4-ccree",
		.blocksize = SM4_BLOCK_SIZE,
		.template_skcipher = {
			.setkey = cc_cipher_setkey,
			.encrypt = cc_cipher_encrypt,
			.decrypt = cc_cipher_decrypt,
			.min_keysize = SM4_KEY_SIZE,
			.max_keysize = SM4_KEY_SIZE,
			.ivsize = SM4_BLOCK_SIZE,
			},
		.cipher_mode = DRV_CIPHER_CBC,
		.flow_mode = S_DIN_to_SM4,
		.min_hw_rev = CC_HW_REV_713,
		.std_body = CC_STD_OSCCA,
	},
	{
		.name = "ecb(sm4)",
		.driver_name = "ecb-sm4-ccree",
		.blocksize = SM4_BLOCK_SIZE,
		.template_skcipher = {
			.setkey = cc_cipher_setkey,
			.encrypt = cc_cipher_encrypt,
			.decrypt = cc_cipher_decrypt,
			.min_keysize = SM4_KEY_SIZE,
			.max_keysize = SM4_KEY_SIZE,
			.ivsize = 0,
			},
		.cipher_mode = DRV_CIPHER_ECB,
		.flow_mode = S_DIN_to_SM4,
		.min_hw_rev = CC_HW_REV_713,
		.std_body = CC_STD_OSCCA,
	},
	{
		.name = "ctr(sm4)",
		.driver_name = "ctr-sm4-ccree",
		.blocksize = 1,
		.template_skcipher = {
			.setkey = cc_cipher_setkey,
			.encrypt = cc_cipher_encrypt,
			.decrypt = cc_cipher_decrypt,
			.min_keysize = SM4_KEY_SIZE,
			.max_keysize = SM4_KEY_SIZE,
			.ivsize = SM4_BLOCK_SIZE,
			},
		.cipher_mode = DRV_CIPHER_CTR,
		.flow_mode = S_DIN_to_SM4,
		.min_hw_rev = CC_HW_REV_713,
		.std_body = CC_STD_OSCCA,
	},
	{
		.name = "cbc(psm4)",
		.driver_name = "cbc-psm4-ccree",
		.blocksize = SM4_BLOCK_SIZE,
		.template_skcipher = {
			.setkey = cc_cipher_sethkey,
			.encrypt = cc_cipher_encrypt,
			.decrypt = cc_cipher_decrypt,
			.min_keysize = CC_HW_KEY_SIZE,
			.max_keysize = CC_HW_KEY_SIZE,
			.ivsize = SM4_BLOCK_SIZE,
			},
		.cipher_mode = DRV_CIPHER_CBC,
		.flow_mode = S_DIN_to_SM4,
		.min_hw_rev = CC_HW_REV_713,
		.std_body = CC_STD_OSCCA,
		.sec_func = true,
	},
	{
		.name = "ctr(psm4)",
		.driver_name = "ctr-psm4-ccree",
		.blocksize = SM4_BLOCK_SIZE,
		.template_skcipher = {
			.setkey = cc_cipher_sethkey,
			.encrypt = cc_cipher_encrypt,
			.decrypt = cc_cipher_decrypt,
			.min_keysize = CC_HW_KEY_SIZE,
			.max_keysize = CC_HW_KEY_SIZE,
			.ivsize = SM4_BLOCK_SIZE,
			},
		.cipher_mode = DRV_CIPHER_CTR,
		.flow_mode = S_DIN_to_SM4,
		.min_hw_rev = CC_HW_REV_713,
		.std_body = CC_STD_OSCCA,
		.sec_func = true,
>>>>>>> upstream/android-13
	},
};

static struct cc_crypto_alg *cc_create_alg(const struct cc_alg_template *tmpl,
					   struct device *dev)
{
	struct cc_crypto_alg *t_alg;
	struct skcipher_alg *alg;

<<<<<<< HEAD
	t_alg = kzalloc(sizeof(*t_alg), GFP_KERNEL);
=======
	t_alg = devm_kzalloc(dev, sizeof(*t_alg), GFP_KERNEL);
>>>>>>> upstream/android-13
	if (!t_alg)
		return ERR_PTR(-ENOMEM);

	alg = &t_alg->skcipher_alg;

	memcpy(alg, &tmpl->template_skcipher, sizeof(*alg));

	snprintf(alg->base.cra_name, CRYPTO_MAX_ALG_NAME, "%s", tmpl->name);
	snprintf(alg->base.cra_driver_name, CRYPTO_MAX_ALG_NAME, "%s",
		 tmpl->driver_name);
	alg->base.cra_module = THIS_MODULE;
	alg->base.cra_priority = CC_CRA_PRIO;
	alg->base.cra_blocksize = tmpl->blocksize;
	alg->base.cra_alignmask = 0;
	alg->base.cra_ctxsize = sizeof(struct cc_cipher_ctx);

	alg->base.cra_init = cc_cipher_init;
	alg->base.cra_exit = cc_cipher_exit;
	alg->base.cra_flags = CRYPTO_ALG_ASYNC | CRYPTO_ALG_KERN_DRIVER_ONLY;

	t_alg->cipher_mode = tmpl->cipher_mode;
	t_alg->flow_mode = tmpl->flow_mode;
<<<<<<< HEAD
	t_alg->data_unit = tmpl->data_unit;
=======
>>>>>>> upstream/android-13

	return t_alg;
}

int cc_cipher_free(struct cc_drvdata *drvdata)
{
	struct cc_crypto_alg *t_alg, *n;
<<<<<<< HEAD
	struct cc_cipher_handle *cipher_handle = drvdata->cipher_handle;

	if (cipher_handle) {
		/* Remove registered algs */
		list_for_each_entry_safe(t_alg, n, &cipher_handle->alg_list,
					 entry) {
			crypto_unregister_skcipher(&t_alg->skcipher_alg);
			list_del(&t_alg->entry);
			kfree(t_alg);
		}
		kfree(cipher_handle);
		drvdata->cipher_handle = NULL;
=======

	/* Remove registered algs */
	list_for_each_entry_safe(t_alg, n, &drvdata->alg_list, entry) {
		crypto_unregister_skcipher(&t_alg->skcipher_alg);
		list_del(&t_alg->entry);
>>>>>>> upstream/android-13
	}
	return 0;
}

int cc_cipher_alloc(struct cc_drvdata *drvdata)
{
<<<<<<< HEAD
	struct cc_cipher_handle *cipher_handle;
=======
>>>>>>> upstream/android-13
	struct cc_crypto_alg *t_alg;
	struct device *dev = drvdata_to_dev(drvdata);
	int rc = -ENOMEM;
	int alg;

<<<<<<< HEAD
	cipher_handle = kmalloc(sizeof(*cipher_handle), GFP_KERNEL);
	if (!cipher_handle)
		return -ENOMEM;

	INIT_LIST_HEAD(&cipher_handle->alg_list);
	drvdata->cipher_handle = cipher_handle;
=======
	INIT_LIST_HEAD(&drvdata->alg_list);
>>>>>>> upstream/android-13

	/* Linux crypto */
	dev_dbg(dev, "Number of algorithms = %zu\n",
		ARRAY_SIZE(skcipher_algs));
	for (alg = 0; alg < ARRAY_SIZE(skcipher_algs); alg++) {
<<<<<<< HEAD
		if (skcipher_algs[alg].min_hw_rev > drvdata->hw_rev)
=======
		if ((skcipher_algs[alg].min_hw_rev > drvdata->hw_rev) ||
		    !(drvdata->std_bodies & skcipher_algs[alg].std_body) ||
		    (drvdata->sec_disabled && skcipher_algs[alg].sec_func))
>>>>>>> upstream/android-13
			continue;

		dev_dbg(dev, "creating %s\n", skcipher_algs[alg].driver_name);
		t_alg = cc_create_alg(&skcipher_algs[alg], dev);
		if (IS_ERR(t_alg)) {
			rc = PTR_ERR(t_alg);
			dev_err(dev, "%s alg allocation failed\n",
				skcipher_algs[alg].driver_name);
			goto fail0;
		}
		t_alg->drvdata = drvdata;

		dev_dbg(dev, "registering %s\n",
			skcipher_algs[alg].driver_name);
		rc = crypto_register_skcipher(&t_alg->skcipher_alg);
		dev_dbg(dev, "%s alg registration rc = %x\n",
			t_alg->skcipher_alg.base.cra_driver_name, rc);
		if (rc) {
			dev_err(dev, "%s alg registration failed\n",
				t_alg->skcipher_alg.base.cra_driver_name);
<<<<<<< HEAD
			kfree(t_alg);
			goto fail0;
		} else {
			list_add_tail(&t_alg->entry,
				      &cipher_handle->alg_list);
			dev_dbg(dev, "Registered %s\n",
				t_alg->skcipher_alg.base.cra_driver_name);
		}
=======
			goto fail0;
		}

		list_add_tail(&t_alg->entry, &drvdata->alg_list);
		dev_dbg(dev, "Registered %s\n",
			t_alg->skcipher_alg.base.cra_driver_name);
>>>>>>> upstream/android-13
	}
	return 0;

fail0:
	cc_cipher_free(drvdata);
	return rc;
}
