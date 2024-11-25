<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> upstream/android-13
/*
 * Copyright (C) 2011 Intel Corporation
 *
 * Author:
 * Dmitry Kasatkin <dmitry.kasatkin@intel.com>
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 2 of the License.
 *
 */

#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

=======
 */

>>>>>>> upstream/android-13
#include <linux/err.h>
#include <linux/sched.h>
#include <linux/slab.h>
#include <linux/cred.h>
<<<<<<< HEAD
=======
#include <linux/kernel_read_file.h>
>>>>>>> upstream/android-13
#include <linux/key-type.h>
#include <linux/digsig.h>
#include <linux/vmalloc.h>
#include <crypto/public_key.h>
#include <keys/system_keyring.h>

#include "integrity.h"

static struct key *keyring[INTEGRITY_KEYRING_MAX];

<<<<<<< HEAD
static const char *keyring_name[INTEGRITY_KEYRING_MAX] = {
=======
static const char * const keyring_name[INTEGRITY_KEYRING_MAX] = {
>>>>>>> upstream/android-13
#ifndef CONFIG_INTEGRITY_TRUSTED_KEYRING
	"_evm",
	"_ima",
#else
	".evm",
	".ima",
#endif
<<<<<<< HEAD
	"_module",
};

#ifdef CONFIG_INTEGRITY_TRUSTED_KEYRING
static bool init_keyring __initdata = true;
#else
static bool init_keyring __initdata;
#endif

=======
	".platform",
};

>>>>>>> upstream/android-13
#ifdef CONFIG_IMA_KEYRINGS_PERMIT_SIGNED_BY_BUILTIN_OR_SECONDARY
#define restrict_link_to_ima restrict_link_by_builtin_and_secondary_trusted
#else
#define restrict_link_to_ima restrict_link_by_builtin_trusted
#endif

<<<<<<< HEAD
int integrity_digsig_verify(const unsigned int id, const char *sig, int siglen,
			    const char *digest, int digestlen)
{
	if (id >= INTEGRITY_KEYRING_MAX || siglen < 2)
		return -EINVAL;
=======
static struct key *integrity_keyring_from_id(const unsigned int id)
{
	if (id >= INTEGRITY_KEYRING_MAX)
		return ERR_PTR(-EINVAL);
>>>>>>> upstream/android-13

	if (!keyring[id]) {
		keyring[id] =
			request_key(&key_type_keyring, keyring_name[id], NULL);
		if (IS_ERR(keyring[id])) {
			int err = PTR_ERR(keyring[id]);
			pr_err("no %s keyring: %d\n", keyring_name[id], err);
			keyring[id] = NULL;
<<<<<<< HEAD
			return err;
		}
	}

	switch (sig[1]) {
	case 1:
		/* v1 API expect signature without xattr type */
		return digsig_verify(keyring[id], sig + 1, siglen - 1,
				     digest, digestlen);
	case 2:
		return asymmetric_verify(keyring[id], sig, siglen,
					 digest, digestlen);
=======
			return ERR_PTR(err);
		}
	}

	return keyring[id];
}

int integrity_digsig_verify(const unsigned int id, const char *sig, int siglen,
			    const char *digest, int digestlen)
{
	struct key *keyring;

	if (siglen < 2)
		return -EINVAL;

	keyring = integrity_keyring_from_id(id);
	if (IS_ERR(keyring))
		return PTR_ERR(keyring);

	switch (sig[1]) {
	case 1:
		/* v1 API expect signature without xattr type */
		return digsig_verify(keyring, sig + 1, siglen - 1, digest,
				     digestlen);
	case 2:
		return asymmetric_verify(keyring, sig, siglen, digest,
					 digestlen);
>>>>>>> upstream/android-13
	}

	return -EOPNOTSUPP;
}

<<<<<<< HEAD
int __init integrity_init_keyring(const unsigned int id)
{
	const struct cred *cred = current_cred();
	struct key_restriction *restriction;
	int err = 0;

	if (!init_keyring)
=======
int integrity_modsig_verify(const unsigned int id, const struct modsig *modsig)
{
	struct key *keyring;

	keyring = integrity_keyring_from_id(id);
	if (IS_ERR(keyring))
		return PTR_ERR(keyring);

	return ima_modsig_verify(keyring, modsig);
}

static int __init __integrity_init_keyring(const unsigned int id,
					   key_perm_t perm,
					   struct key_restriction *restriction)
{
	const struct cred *cred = current_cred();
	int err = 0;

	keyring[id] = keyring_alloc(keyring_name[id], KUIDT_INIT(0),
				    KGIDT_INIT(0), cred, perm,
				    KEY_ALLOC_NOT_IN_QUOTA, restriction, NULL);
	if (IS_ERR(keyring[id])) {
		err = PTR_ERR(keyring[id]);
		pr_info("Can't allocate %s keyring (%d)\n",
			keyring_name[id], err);
		keyring[id] = NULL;
	} else {
		if (id == INTEGRITY_KEYRING_PLATFORM)
			set_platform_trusted_keys(keyring[id]);
		if (id == INTEGRITY_KEYRING_IMA)
			load_module_cert(keyring[id]);
	}

	return err;
}

int __init integrity_init_keyring(const unsigned int id)
{
	struct key_restriction *restriction;
	key_perm_t perm;

	perm = (KEY_POS_ALL & ~KEY_POS_SETATTR) | KEY_USR_VIEW
		| KEY_USR_READ | KEY_USR_SEARCH;

	if (id == INTEGRITY_KEYRING_PLATFORM) {
		restriction = NULL;
		goto out;
	}

	if (!IS_ENABLED(CONFIG_INTEGRITY_TRUSTED_KEYRING))
>>>>>>> upstream/android-13
		return 0;

	restriction = kzalloc(sizeof(struct key_restriction), GFP_KERNEL);
	if (!restriction)
		return -ENOMEM;

	restriction->check = restrict_link_to_ima;
<<<<<<< HEAD

	keyring[id] = keyring_alloc(keyring_name[id], KUIDT_INIT(0),
				    KGIDT_INIT(0), cred,
				    ((KEY_POS_ALL & ~KEY_POS_SETATTR) |
				     KEY_USR_VIEW | KEY_USR_READ |
				     KEY_USR_WRITE | KEY_USR_SEARCH),
				    KEY_ALLOC_NOT_IN_QUOTA,
				    restriction, NULL);
	if (IS_ERR(keyring[id])) {
		err = PTR_ERR(keyring[id]);
		pr_info("Can't allocate %s keyring (%d)\n",
			keyring_name[id], err);
		keyring[id] = NULL;
	}
	return err;
}

int __init integrity_load_x509(const unsigned int id, const char *path)
{
	key_ref_t key;
	void *data;
	loff_t size;
	int rc;
=======
	perm |= KEY_USR_WRITE;

out:
	return __integrity_init_keyring(id, perm, restriction);
}

static int __init integrity_add_key(const unsigned int id, const void *data,
				    off_t size, key_perm_t perm)
{
	key_ref_t key;
	int rc = 0;
>>>>>>> upstream/android-13

	if (!keyring[id])
		return -EINVAL;

<<<<<<< HEAD
	rc = kernel_read_file_from_path(path, &data, &size, 0,
=======
	key = key_create_or_update(make_key_ref(keyring[id], 1), "asymmetric",
				   NULL, data, size, perm,
				   KEY_ALLOC_NOT_IN_QUOTA);
	if (IS_ERR(key)) {
		rc = PTR_ERR(key);
		pr_err("Problem loading X.509 certificate %d\n", rc);
	} else {
		pr_notice("Loaded X.509 cert '%s'\n",
			  key_ref_to_ptr(key)->description);
		key_ref_put(key);
	}

	return rc;

}

int __init integrity_load_x509(const unsigned int id, const char *path)
{
	void *data = NULL;
	size_t size;
	int rc;
	key_perm_t perm;

	rc = kernel_read_file_from_path(path, 0, &data, INT_MAX, NULL,
>>>>>>> upstream/android-13
					READING_X509_CERTIFICATE);
	if (rc < 0) {
		pr_err("Unable to open file: %s (%d)", path, rc);
		return rc;
	}
<<<<<<< HEAD

	key = key_create_or_update(make_key_ref(keyring[id], 1),
				   "asymmetric",
				   NULL,
				   data,
				   size,
				   ((KEY_POS_ALL & ~KEY_POS_SETATTR) |
				    KEY_USR_VIEW | KEY_USR_READ),
				   KEY_ALLOC_NOT_IN_QUOTA);
	if (IS_ERR(key)) {
		rc = PTR_ERR(key);
		pr_err("Problem loading X.509 certificate (%d): %s\n",
		       rc, path);
	} else {
		pr_notice("Loaded X.509 cert '%s': %s\n",
			  key_ref_to_ptr(key)->description, path);
		key_ref_put(key);
	}
	vfree(data);
	return 0;
=======
	size = rc;

	perm = (KEY_POS_ALL & ~KEY_POS_SETATTR) | KEY_USR_VIEW | KEY_USR_READ;

	pr_info("Loading X.509 certificate: %s\n", path);
	rc = integrity_add_key(id, (const void *)data, size, perm);

	vfree(data);
	return rc;
}

int __init integrity_load_cert(const unsigned int id, const char *source,
			       const void *data, size_t len, key_perm_t perm)
{
	if (!data)
		return -EINVAL;

	pr_info("Loading X.509 certificate: %s\n", source);
	return integrity_add_key(id, data, len, perm);
>>>>>>> upstream/android-13
}
