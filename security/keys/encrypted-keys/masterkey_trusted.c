<<<<<<< HEAD
/*
 * Copyright (C) 2010 IBM Corporation
 * Copyright (C) 2010 Politecnico di Torino, Italy
 *                    TORSEC group -- http://security.polito.it
=======
// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright (C) 2010 IBM Corporation
 * Copyright (C) 2010 Politecnico di Torino, Italy
 *                    TORSEC group -- https://security.polito.it
>>>>>>> upstream/android-13
 *
 * Authors:
 * Mimi Zohar <zohar@us.ibm.com>
 * Roberto Sassu <roberto.sassu@polito.it>
 *
<<<<<<< HEAD
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 2 of the License.
 *
=======
>>>>>>> upstream/android-13
 * See Documentation/security/keys/trusted-encrypted.rst
 */

#include <linux/uaccess.h>
<<<<<<< HEAD
#include <linux/module.h>
=======
>>>>>>> upstream/android-13
#include <linux/err.h>
#include <keys/trusted-type.h>
#include <keys/encrypted-type.h>
#include "encrypted.h"

/*
 * request_trusted_key - request the trusted key
 *
 * Trusted keys are sealed to PCRs and other metadata. Although userspace
 * manages both trusted/encrypted key-types, like the encrypted key type
 * data, trusted key type data is not visible decrypted from userspace.
 */
struct key *request_trusted_key(const char *trusted_desc,
				const u8 **master_key, size_t *master_keylen)
{
	struct trusted_key_payload *tpayload;
	struct key *tkey;

	tkey = request_key(&key_type_trusted, trusted_desc, NULL);
	if (IS_ERR(tkey))
		goto error;

	down_read(&tkey->sem);
	tpayload = tkey->payload.data[0];
	*master_key = tpayload->key;
	*master_keylen = tpayload->key_len;
error:
	return tkey;
}
