<<<<<<< HEAD
/*
 * Copyright (c) 2016, Intel Corporation
 * Authors: Salvatore Benedetto <salvatore.benedetto@intel.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version
 * 2 of the License, or (at your option) any later version.
=======
// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * Copyright (c) 2016, Intel Corporation
 * Authors: Salvatore Benedetto <salvatore.benedetto@intel.com>
>>>>>>> upstream/android-13
 */
#include <linux/kernel.h>
#include <linux/export.h>
#include <linux/err.h>
#include <linux/string.h>
#include <crypto/ecdh.h>
#include <crypto/kpp.h>

<<<<<<< HEAD
#define ECDH_KPP_SECRET_MIN_SIZE (sizeof(struct kpp_secret) + 2 * sizeof(short))
=======
#define ECDH_KPP_SECRET_MIN_SIZE (sizeof(struct kpp_secret) + sizeof(short))
>>>>>>> upstream/android-13

static inline u8 *ecdh_pack_data(void *dst, const void *src, size_t sz)
{
	memcpy(dst, src, sz);
	return dst + sz;
}

static inline const u8 *ecdh_unpack_data(void *dst, const void *src, size_t sz)
{
	memcpy(dst, src, sz);
	return src + sz;
}

unsigned int crypto_ecdh_key_len(const struct ecdh *params)
{
	return ECDH_KPP_SECRET_MIN_SIZE + params->key_size;
}
EXPORT_SYMBOL_GPL(crypto_ecdh_key_len);

int crypto_ecdh_encode_key(char *buf, unsigned int len,
			   const struct ecdh *params)
{
	u8 *ptr = buf;
	struct kpp_secret secret = {
		.type = CRYPTO_KPP_SECRET_TYPE_ECDH,
		.len = len
	};

	if (unlikely(!buf))
		return -EINVAL;

	if (len != crypto_ecdh_key_len(params))
		return -EINVAL;

	ptr = ecdh_pack_data(ptr, &secret, sizeof(secret));
<<<<<<< HEAD
	ptr = ecdh_pack_data(ptr, &params->curve_id, sizeof(params->curve_id));
=======
>>>>>>> upstream/android-13
	ptr = ecdh_pack_data(ptr, &params->key_size, sizeof(params->key_size));
	ecdh_pack_data(ptr, params->key, params->key_size);

	return 0;
}
EXPORT_SYMBOL_GPL(crypto_ecdh_encode_key);

int crypto_ecdh_decode_key(const char *buf, unsigned int len,
			   struct ecdh *params)
{
	const u8 *ptr = buf;
	struct kpp_secret secret;

	if (unlikely(!buf || len < ECDH_KPP_SECRET_MIN_SIZE))
		return -EINVAL;

	ptr = ecdh_unpack_data(&secret, ptr, sizeof(secret));
	if (secret.type != CRYPTO_KPP_SECRET_TYPE_ECDH)
		return -EINVAL;

	if (unlikely(len < secret.len))
		return -EINVAL;

<<<<<<< HEAD
	ptr = ecdh_unpack_data(&params->curve_id, ptr, sizeof(params->curve_id));
=======
>>>>>>> upstream/android-13
	ptr = ecdh_unpack_data(&params->key_size, ptr, sizeof(params->key_size));
	if (secret.len != crypto_ecdh_key_len(params))
		return -EINVAL;

	/* Don't allocate memory. Set pointer to data
	 * within the given buffer
	 */
	params->key = (void *)ptr;

	return 0;
}
EXPORT_SYMBOL_GPL(crypto_ecdh_decode_key);
