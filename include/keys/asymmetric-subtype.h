<<<<<<< HEAD
/* Asymmetric public-key cryptography key subtype
 *
 * See Documentation/crypto/asymmetric-keys.txt
 *
 * Copyright (C) 2012 Red Hat, Inc. All Rights Reserved.
 * Written by David Howells (dhowells@redhat.com)
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public Licence
 * as published by the Free Software Foundation; either version
 * 2 of the Licence, or (at your option) any later version.
=======
/* SPDX-License-Identifier: GPL-2.0-or-later */
/* Asymmetric public-key cryptography key subtype
 *
 * See Documentation/crypto/asymmetric-keys.rst
 *
 * Copyright (C) 2012 Red Hat, Inc. All Rights Reserved.
 * Written by David Howells (dhowells@redhat.com)
>>>>>>> upstream/android-13
 */

#ifndef _KEYS_ASYMMETRIC_SUBTYPE_H
#define _KEYS_ASYMMETRIC_SUBTYPE_H

#include <linux/seq_file.h>
#include <keys/asymmetric-type.h>

<<<<<<< HEAD
=======
struct kernel_pkey_query;
struct kernel_pkey_params;
>>>>>>> upstream/android-13
struct public_key_signature;

/*
 * Keys of this type declare a subtype that indicates the handlers and
 * capabilities.
 */
struct asymmetric_key_subtype {
	struct module		*owner;
	const char		*name;
	unsigned short		name_len;	/* length of name */

	/* Describe a key of this subtype for /proc/keys */
	void (*describe)(const struct key *key, struct seq_file *m);

	/* Destroy a key of this subtype */
	void (*destroy)(void *payload_crypto, void *payload_auth);

<<<<<<< HEAD
=======
	int (*query)(const struct kernel_pkey_params *params,
		     struct kernel_pkey_query *info);

	/* Encrypt/decrypt/sign data */
	int (*eds_op)(struct kernel_pkey_params *params,
		      const void *in, void *out);

>>>>>>> upstream/android-13
	/* Verify the signature on a key of this subtype (optional) */
	int (*verify_signature)(const struct key *key,
				const struct public_key_signature *sig);
};

/**
 * asymmetric_key_subtype - Get the subtype from an asymmetric key
 * @key: The key of interest.
 *
 * Retrieves and returns the subtype pointer of the asymmetric key from the
 * type-specific data attached to the key.
 */
static inline
struct asymmetric_key_subtype *asymmetric_key_subtype(const struct key *key)
{
	return key->payload.data[asym_subtype];
}

#endif /* _KEYS_ASYMMETRIC_SUBTYPE_H */
