<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> upstream/android-13
/*
 * Software WEP encryption implementation
 * Copyright 2002, Jouni Malinen <jkmaline@cc.hut.fi>
 * Copyright 2003, Instant802 Networks, Inc.
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
=======
>>>>>>> upstream/android-13
 */

#ifndef WEP_H
#define WEP_H

#include <linux/skbuff.h>
#include <linux/types.h>
#include "ieee80211_i.h"
#include "key.h"

<<<<<<< HEAD
int ieee80211_wep_init(struct ieee80211_local *local);
void ieee80211_wep_free(struct ieee80211_local *local);
int ieee80211_wep_encrypt_data(struct crypto_cipher *tfm, u8 *rc4key,
=======
void ieee80211_wep_init(struct ieee80211_local *local);
int ieee80211_wep_encrypt_data(struct arc4_ctx *ctx, u8 *rc4key,
>>>>>>> upstream/android-13
				size_t klen, u8 *data, size_t data_len);
int ieee80211_wep_encrypt(struct ieee80211_local *local,
			  struct sk_buff *skb,
			  const u8 *key, int keylen, int keyidx);
<<<<<<< HEAD
int ieee80211_wep_decrypt_data(struct crypto_cipher *tfm, u8 *rc4key,
=======
int ieee80211_wep_decrypt_data(struct arc4_ctx *ctx, u8 *rc4key,
>>>>>>> upstream/android-13
			       size_t klen, u8 *data, size_t data_len);

ieee80211_rx_result
ieee80211_crypto_wep_decrypt(struct ieee80211_rx_data *rx);
ieee80211_tx_result
ieee80211_crypto_wep_encrypt(struct ieee80211_tx_data *tx);

#endif /* WEP_H */
