<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-or-later */
>>>>>>> upstream/android-13
/* System keyring containing trusted public keys.
 *
 * Copyright (C) 2013 Red Hat, Inc. All Rights Reserved.
 * Written by David Howells (dhowells@redhat.com)
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public Licence
 * as published by the Free Software Foundation; either version
 * 2 of the Licence, or (at your option) any later version.
=======
>>>>>>> upstream/android-13
 */

#ifndef _KEYS_SYSTEM_KEYRING_H
#define _KEYS_SYSTEM_KEYRING_H

#include <linux/key.h>

#ifdef CONFIG_SYSTEM_TRUSTED_KEYRING

extern int restrict_link_by_builtin_trusted(struct key *keyring,
					    const struct key_type *type,
					    const union key_payload *payload,
					    struct key *restriction_key);
<<<<<<< HEAD

#else
#define restrict_link_by_builtin_trusted restrict_link_reject
=======
extern __init int load_module_cert(struct key *keyring);

#else
#define restrict_link_by_builtin_trusted restrict_link_reject

static inline __init int load_module_cert(struct key *keyring)
{
	return 0;
}

>>>>>>> upstream/android-13
#endif

#ifdef CONFIG_SECONDARY_TRUSTED_KEYRING
extern int restrict_link_by_builtin_and_secondary_trusted(
	struct key *keyring,
	const struct key_type *type,
	const union key_payload *payload,
	struct key *restriction_key);
#else
#define restrict_link_by_builtin_and_secondary_trusted restrict_link_by_builtin_trusted
#endif

<<<<<<< HEAD
=======
extern struct pkcs7_message *pkcs7;
>>>>>>> upstream/android-13
#ifdef CONFIG_SYSTEM_BLACKLIST_KEYRING
extern int mark_hash_blacklisted(const char *hash);
extern int is_hash_blacklisted(const u8 *hash, size_t hash_len,
			       const char *type);
<<<<<<< HEAD
=======
extern int is_binary_blacklisted(const u8 *hash, size_t hash_len);
>>>>>>> upstream/android-13
#else
static inline int is_hash_blacklisted(const u8 *hash, size_t hash_len,
				      const char *type)
{
	return 0;
}
<<<<<<< HEAD
=======

static inline int is_binary_blacklisted(const u8 *hash, size_t hash_len)
{
	return 0;
}
#endif

#ifdef CONFIG_SYSTEM_REVOCATION_LIST
extern int add_key_to_revocation_list(const char *data, size_t size);
extern int is_key_on_revocation_list(struct pkcs7_message *pkcs7);
#else
static inline int add_key_to_revocation_list(const char *data, size_t size)
{
	return 0;
}
static inline int is_key_on_revocation_list(struct pkcs7_message *pkcs7)
{
	return -ENOKEY;
}
>>>>>>> upstream/android-13
#endif

#ifdef CONFIG_IMA_BLACKLIST_KEYRING
extern struct key *ima_blacklist_keyring;

static inline struct key *get_ima_blacklist_keyring(void)
{
	return ima_blacklist_keyring;
}
#else
static inline struct key *get_ima_blacklist_keyring(void)
{
	return NULL;
}
#endif /* CONFIG_IMA_BLACKLIST_KEYRING */

<<<<<<< HEAD
=======
#if defined(CONFIG_INTEGRITY_PLATFORM_KEYRING) && \
	defined(CONFIG_SYSTEM_TRUSTED_KEYRING)
extern void __init set_platform_trusted_keys(struct key *keyring);
#else
static inline void set_platform_trusted_keys(struct key *keyring)
{
}
#endif
>>>>>>> upstream/android-13

#endif /* _KEYS_SYSTEM_KEYRING_H */
