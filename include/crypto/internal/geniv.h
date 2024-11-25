<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-or-later */
>>>>>>> upstream/android-13
/*
 * geniv: IV generation
 *
 * Copyright (c) 2015 Herbert Xu <herbert@gondor.apana.org.au>
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

#ifndef _CRYPTO_INTERNAL_GENIV_H
#define _CRYPTO_INTERNAL_GENIV_H

#include <crypto/internal/aead.h>
#include <linux/spinlock.h>
#include <linux/types.h>

struct aead_geniv_ctx {
	spinlock_t lock;
	struct crypto_aead *child;
<<<<<<< HEAD
	struct crypto_skcipher *sknull;
=======
	struct crypto_sync_skcipher *sknull;
>>>>>>> upstream/android-13
	u8 salt[] __attribute__ ((aligned(__alignof__(u32))));
};

struct aead_instance *aead_geniv_alloc(struct crypto_template *tmpl,
<<<<<<< HEAD
				       struct rtattr **tb, u32 type, u32 mask);
void aead_geniv_free(struct aead_instance *inst);
=======
				       struct rtattr **tb);
>>>>>>> upstream/android-13
int aead_init_geniv(struct crypto_aead *tfm);
void aead_exit_geniv(struct crypto_aead *tfm);

#endif	/* _CRYPTO_INTERNAL_GENIV_H */
