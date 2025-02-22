<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-or-later */
>>>>>>> upstream/android-13
/* Signature verification
 *
 * Copyright (C) 2014 Red Hat, Inc. All Rights Reserved.
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

#ifndef _LINUX_VERIFICATION_H
#define _LINUX_VERIFICATION_H

<<<<<<< HEAD
=======
#include <linux/types.h>

>>>>>>> upstream/android-13
/*
 * Indicate that both builtin trusted keys and secondary trusted keys
 * should be used.
 */
#define VERIFY_USE_SECONDARY_KEYRING ((struct key *)1UL)
<<<<<<< HEAD
=======
#define VERIFY_USE_PLATFORM_KEYRING  ((struct key *)2UL)
>>>>>>> upstream/android-13

/*
 * The use to which an asymmetric key is being put.
 */
enum key_being_used_for {
	VERIFYING_MODULE_SIGNATURE,
	VERIFYING_FIRMWARE_SIGNATURE,
	VERIFYING_KEXEC_PE_SIGNATURE,
	VERIFYING_KEY_SIGNATURE,
	VERIFYING_KEY_SELF_SIGNATURE,
	VERIFYING_UNSPECIFIED_SIGNATURE,
	NR__KEY_BEING_USED_FOR
};
extern const char *const key_being_used_for[NR__KEY_BEING_USED_FOR];

#ifdef CONFIG_SYSTEM_DATA_VERIFICATION

struct key;
<<<<<<< HEAD
=======
struct pkcs7_message;
>>>>>>> upstream/android-13

extern int verify_pkcs7_signature(const void *data, size_t len,
				  const void *raw_pkcs7, size_t pkcs7_len,
				  struct key *trusted_keys,
				  enum key_being_used_for usage,
				  int (*view_content)(void *ctx,
						      const void *data, size_t len,
						      size_t asn1hdrlen),
				  void *ctx);
<<<<<<< HEAD
=======
extern int verify_pkcs7_message_sig(const void *data, size_t len,
				    struct pkcs7_message *pkcs7,
				    struct key *trusted_keys,
				    enum key_being_used_for usage,
				    int (*view_content)(void *ctx,
							const void *data,
							size_t len,
							size_t asn1hdrlen),
				    void *ctx);
>>>>>>> upstream/android-13

#ifdef CONFIG_SIGNED_PE_FILE_VERIFICATION
extern int verify_pefile_signature(const void *pebuf, unsigned pelen,
				   struct key *trusted_keys,
				   enum key_being_used_for usage);
#endif

#endif /* CONFIG_SYSTEM_DATA_VERIFICATION */
#endif /* _LINUX_VERIFY_PEFILE_H */
