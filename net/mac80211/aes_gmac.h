<<<<<<< HEAD
/*
 * Copyright 2015, Qualcomm Atheros, Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
=======
/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Copyright 2015, Qualcomm Atheros, Inc.
>>>>>>> upstream/android-13
 */

#ifndef AES_GMAC_H
#define AES_GMAC_H

#include <linux/crypto.h>

#define GMAC_AAD_LEN	20
#define GMAC_MIC_LEN	16
#define GMAC_NONCE_LEN	12

struct crypto_aead *ieee80211_aes_gmac_key_setup(const u8 key[],
						 size_t key_len);
int ieee80211_aes_gmac(struct crypto_aead *tfm, const u8 *aad, u8 *nonce,
		       const u8 *data, size_t data_len, u8 *mic);
void ieee80211_aes_gmac_key_free(struct crypto_aead *tfm);

#endif /* AES_GMAC_H */
