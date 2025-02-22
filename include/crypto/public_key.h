<<<<<<< HEAD
/* Asymmetric public-key algorithm definitions
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
/* Asymmetric public-key algorithm definitions
 *
 * See Documentation/crypto/asymmetric-keys.rst
 *
 * Copyright (C) 2012 Red Hat, Inc. All Rights Reserved.
 * Written by David Howells (dhowells@redhat.com)
>>>>>>> upstream/android-13
 */

#ifndef _LINUX_PUBLIC_KEY_H
#define _LINUX_PUBLIC_KEY_H

<<<<<<< HEAD
=======
#include <linux/keyctl.h>
#include <linux/oid_registry.h>

>>>>>>> upstream/android-13
/*
 * Cryptographic data for the public-key subtype of the asymmetric key type.
 *
 * Note that this may include private part of the key as well as the public
 * part.
 */
struct public_key {
	void *key;
	u32 keylen;
<<<<<<< HEAD
=======
	enum OID algo;
	void *params;
	u32 paramlen;
	bool key_is_private;
>>>>>>> upstream/android-13
	const char *id_type;
	const char *pkey_algo;
};

extern void public_key_free(struct public_key *key);

/*
 * Public key cryptography signature data
 */
struct public_key_signature {
	struct asymmetric_key_id *auth_ids[2];
	u8 *s;			/* Signature */
<<<<<<< HEAD
	u32 s_size;		/* Number of bytes in signature */
	u8 *digest;
	u8 digest_size;		/* Number of bytes in digest */
	const char *pkey_algo;
	const char *hash_algo;
=======
	u8 *digest;
	u32 s_size;		/* Number of bytes in signature */
	u32 digest_size;	/* Number of bytes in digest */
	const char *pkey_algo;
	const char *hash_algo;
	const char *encoding;
	const void *data;
	unsigned int data_size;
>>>>>>> upstream/android-13
};

extern void public_key_signature_free(struct public_key_signature *sig);

extern struct asymmetric_key_subtype public_key_subtype;

struct key;
struct key_type;
union key_payload;

extern int restrict_link_by_signature(struct key *dest_keyring,
				      const struct key_type *type,
				      const union key_payload *payload,
				      struct key *trust_keyring);

extern int restrict_link_by_key_or_keyring(struct key *dest_keyring,
					   const struct key_type *type,
					   const union key_payload *payload,
					   struct key *trusted);

extern int restrict_link_by_key_or_keyring_chain(struct key *trust_keyring,
						 const struct key_type *type,
						 const union key_payload *payload,
						 struct key *trusted);

<<<<<<< HEAD
extern int verify_signature(const struct key *key,
			    const struct public_key_signature *sig);
=======
extern int query_asymmetric_key(const struct kernel_pkey_params *,
				struct kernel_pkey_query *);

extern int encrypt_blob(struct kernel_pkey_params *, const void *, void *);
extern int decrypt_blob(struct kernel_pkey_params *, const void *, void *);
extern int create_signature(struct kernel_pkey_params *, const void *, void *);
extern int verify_signature(const struct key *,
			    const struct public_key_signature *);
>>>>>>> upstream/android-13

int public_key_verify_signature(const struct public_key *pkey,
				const struct public_key_signature *sig);

#endif /* _LINUX_PUBLIC_KEY_H */
