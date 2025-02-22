/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _CRYPTO_XTS_H
#define _CRYPTO_XTS_H

#include <crypto/b128ops.h>
#include <crypto/internal/skcipher.h>
#include <linux/fips.h>

#define XTS_BLOCK_SIZE 16

#define XTS_TWEAK_CAST(x) ((void (*)(void *, u8*, const u8*))(x))

static inline int xts_check_key(struct crypto_tfm *tfm,
				const u8 *key, unsigned int keylen)
{
	u32 *flags = &tfm->crt_flags;

	
	if (keylen % 2) {
		*flags |= CRYPTO_TFM_RES_BAD_KEY_LEN;
		return -EINVAL;
	}

	
	if (fips_enabled &&
	    !crypto_memneq(key, key + (keylen / 2), keylen / 2)) {
		*flags |= CRYPTO_TFM_RES_WEAK_KEY;
		return -EINVAL;
	}

	return 0;
}

static inline int xts_verify_key(struct crypto_skcipher *tfm,
				 const u8 *key, unsigned int keylen)
{
	
	if (keylen % 2) {
		crypto_skcipher_set_flags(tfm, CRYPTO_TFM_RES_BAD_KEY_LEN);
		return -EINVAL;
	}

	
	if ((fips_enabled || crypto_skcipher_get_flags(tfm) &
			     CRYPTO_TFM_REQ_WEAK_KEY) &&
	    !crypto_memneq(key, key + (keylen / 2), keylen / 2)) {
		crypto_skcipher_set_flags(tfm, CRYPTO_TFM_RES_WEAK_KEY);
		return -EINVAL;
	}

	return 0;
}

#endif  
