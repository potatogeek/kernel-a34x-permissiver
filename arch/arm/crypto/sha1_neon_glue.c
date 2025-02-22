<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> upstream/android-13
/*
 * Glue code for the SHA1 Secure Hash Algorithm assembler implementation using
 * ARM NEON instructions.
 *
 * Copyright © 2014 Jussi Kivilinna <jussi.kivilinna@iki.fi>
 *
 * This file is based on sha1_generic.c and sha1_ssse3_glue.c:
 *  Copyright (c) Alan Smithee.
 *  Copyright (c) Andrew McDonald <andrew@mcdonald.org.uk>
 *  Copyright (c) Jean-Francois Dive <jef@linuxbe.org>
 *  Copyright (c) Mathias Krause <minipli@googlemail.com>
 *  Copyright (c) Chandramouli Narayanan <mouli@linux.intel.com>
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version.
 *
 */

#include <crypto/internal/hash.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/mm.h>
#include <linux/cryptohash.h>
#include <linux/types.h>
#include <crypto/sha.h>
=======
 */

#include <crypto/internal/hash.h>
#include <crypto/internal/simd.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/mm.h>
#include <linux/types.h>
#include <crypto/sha1.h>
>>>>>>> upstream/android-13
#include <crypto/sha1_base.h>
#include <asm/neon.h>
#include <asm/simd.h>

#include "sha1.h"

asmlinkage void sha1_transform_neon(void *state_h, const char *data,
				    unsigned int rounds);

static int sha1_neon_update(struct shash_desc *desc, const u8 *data,
			  unsigned int len)
{
	struct sha1_state *sctx = shash_desc_ctx(desc);

<<<<<<< HEAD
	if (!may_use_simd() ||
=======
	if (!crypto_simd_usable() ||
>>>>>>> upstream/android-13
	    (sctx->count % SHA1_BLOCK_SIZE) + len < SHA1_BLOCK_SIZE)
		return sha1_update_arm(desc, data, len);

	kernel_neon_begin();
	sha1_base_do_update(desc, data, len,
			    (sha1_block_fn *)sha1_transform_neon);
	kernel_neon_end();

	return 0;
}

static int sha1_neon_finup(struct shash_desc *desc, const u8 *data,
			   unsigned int len, u8 *out)
{
<<<<<<< HEAD
	if (!may_use_simd())
=======
	if (!crypto_simd_usable())
>>>>>>> upstream/android-13
		return sha1_finup_arm(desc, data, len, out);

	kernel_neon_begin();
	if (len)
		sha1_base_do_update(desc, data, len,
				    (sha1_block_fn *)sha1_transform_neon);
	sha1_base_do_finalize(desc, (sha1_block_fn *)sha1_transform_neon);
	kernel_neon_end();

	return sha1_base_finish(desc, out);
}

static int sha1_neon_final(struct shash_desc *desc, u8 *out)
{
	return sha1_neon_finup(desc, NULL, 0, out);
}

static struct shash_alg alg = {
	.digestsize	=	SHA1_DIGEST_SIZE,
	.init		=	sha1_base_init,
	.update		=	sha1_neon_update,
	.final		=	sha1_neon_final,
	.finup		=	sha1_neon_finup,
	.descsize	=	sizeof(struct sha1_state),
	.base		=	{
		.cra_name		= "sha1",
		.cra_driver_name	= "sha1-neon",
		.cra_priority		= 250,
		.cra_blocksize		= SHA1_BLOCK_SIZE,
		.cra_module		= THIS_MODULE,
	}
};

static int __init sha1_neon_mod_init(void)
{
	if (!cpu_has_neon())
		return -ENODEV;

	return crypto_register_shash(&alg);
}

static void __exit sha1_neon_mod_fini(void)
{
	crypto_unregister_shash(&alg);
}

module_init(sha1_neon_mod_init);
module_exit(sha1_neon_mod_fini);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("SHA1 Secure Hash Algorithm, NEON accelerated");
MODULE_ALIAS_CRYPTO("sha1");
