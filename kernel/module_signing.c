<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> upstream/android-13
/* Module signature checker
 *
 * Copyright (C) 2012 Red Hat, Inc. All Rights Reserved.
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

#include <linux/kernel.h>
#include <linux/errno.h>
<<<<<<< HEAD
=======
#include <linux/module.h>
#include <linux/module_signature.h>
>>>>>>> upstream/android-13
#include <linux/string.h>
#include <linux/verification.h>
#include <crypto/public_key.h>
#include "module-internal.h"

<<<<<<< HEAD
enum pkey_id_type {
	PKEY_ID_PGP,		/* OpenPGP generated key ID */
	PKEY_ID_X509,		/* X.509 arbitrary subjectKeyIdentifier */
	PKEY_ID_PKCS7,		/* Signature in PKCS#7 message */
};

/*
 * Module signature information block.
 *
 * The constituents of the signature section are, in order:
 *
 *	- Signer's name
 *	- Key identifier
 *	- Signature data
 *	- Information block
 */
struct module_signature {
	u8	algo;		/* Public-key crypto algorithm [0] */
	u8	hash;		/* Digest algorithm [0] */
	u8	id_type;	/* Key identifier type [PKEY_ID_PKCS7] */
	u8	signer_len;	/* Length of signer's name [0] */
	u8	key_id_len;	/* Length of key identifier [0] */
	u8	__pad[3];
	__be32	sig_len;	/* Length of signature data */
};

=======
>>>>>>> upstream/android-13
/*
 * Verify the signature on a module.
 */
int mod_verify_sig(const void *mod, struct load_info *info)
{
	struct module_signature ms;
	size_t sig_len, modlen = info->len;
<<<<<<< HEAD
=======
	int ret;
>>>>>>> upstream/android-13

	pr_devel("==>%s(,%zu)\n", __func__, modlen);

	if (modlen <= sizeof(ms))
		return -EBADMSG;

	memcpy(&ms, mod + (modlen - sizeof(ms)), sizeof(ms));
<<<<<<< HEAD
	modlen -= sizeof(ms);

	sig_len = be32_to_cpu(ms.sig_len);
	if (sig_len >= modlen)
		return -EBADMSG;
	modlen -= sig_len;
	info->len = modlen;

	if (ms.id_type != PKEY_ID_PKCS7) {
		pr_err("%s: Module is not signed with expected PKCS#7 message\n",
		       info->name);
		return -ENOPKG;
	}

	if (ms.algo != 0 ||
	    ms.hash != 0 ||
	    ms.signer_len != 0 ||
	    ms.key_id_len != 0 ||
	    ms.__pad[0] != 0 ||
	    ms.__pad[1] != 0 ||
	    ms.__pad[2] != 0) {
		pr_err("%s: PKCS#7 signature info has unexpected non-zero params\n",
		       info->name);
		return -EBADMSG;
	}

	return verify_pkcs7_signature(mod, modlen, mod + modlen, sig_len,
				      NULL, VERIFYING_MODULE_SIGNATURE,
=======

	ret = mod_check_sig(&ms, modlen, "module");
	if (ret)
		return ret;

	sig_len = be32_to_cpu(ms.sig_len);
	modlen -= sig_len + sizeof(ms);
	info->len = modlen;

	return verify_pkcs7_signature(mod, modlen, mod + modlen, sig_len,
				      VERIFY_USE_SECONDARY_KEYRING,
				      VERIFYING_MODULE_SIGNATURE,
>>>>>>> upstream/android-13
				      NULL, NULL);
}
