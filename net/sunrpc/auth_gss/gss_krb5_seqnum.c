/*
 *  linux/net/sunrpc/gss_krb5_seqnum.c
 *
 *  Adapted from MIT Kerberos 5-1.2.1 lib/gssapi/krb5/util_seqnum.c
 *
 *  Copyright (c) 2000 The Regents of the University of Michigan.
 *  All rights reserved.
 *
 *  Andy Adamson   <andros@umich.edu>
 */

/*
 * Copyright 1993 by OpenVision Technologies, Inc.
 *
 * Permission to use, copy, modify, distribute, and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appears in all copies and
 * that both that copyright notice and this permission notice appear in
 * supporting documentation, and that the name of OpenVision not be used
 * in advertising or publicity pertaining to distribution of the software
 * without specific, written prior permission. OpenVision makes no
 * representations about the suitability of this software for any
 * purpose.  It is provided "as is" without express or implied warranty.
 *
 * OPENVISION DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE,
 * INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO
 * EVENT SHALL OPENVISION BE LIABLE FOR ANY SPECIAL, INDIRECT OR
 * CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF
 * USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR
 * OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 */

#include <crypto/skcipher.h>
#include <linux/types.h>
#include <linux/sunrpc/gss_krb5.h>

#if IS_ENABLED(CONFIG_SUNRPC_DEBUG)
# define RPCDBG_FACILITY        RPCDBG_AUTH
#endif

<<<<<<< HEAD
static s32
krb5_make_rc4_seq_num(struct krb5_ctx *kctx, int direction, s32 seqnum,
		      unsigned char *cksum, unsigned char *buf)
{
	struct crypto_skcipher *cipher;
	unsigned char *plain;
	s32 code;

	dprintk("RPC:       %s:\n", __func__);
	cipher = crypto_alloc_skcipher(kctx->gk5e->encrypt_name, 0,
				       CRYPTO_ALG_ASYNC);
	if (IS_ERR(cipher))
		return PTR_ERR(cipher);

	plain = kmalloc(8, GFP_NOFS);
	if (!plain)
		return -ENOMEM;

	plain[0] = (unsigned char) ((seqnum >> 24) & 0xff);
	plain[1] = (unsigned char) ((seqnum >> 16) & 0xff);
	plain[2] = (unsigned char) ((seqnum >> 8) & 0xff);
	plain[3] = (unsigned char) ((seqnum >> 0) & 0xff);
	plain[4] = direction;
	plain[5] = direction;
	plain[6] = direction;
	plain[7] = direction;

	code = krb5_rc4_setup_seq_key(kctx, cipher, cksum);
	if (code)
		goto out;

	code = krb5_encrypt(cipher, cksum, plain, buf, 8);
out:
	crypto_free_skcipher(cipher);
	kfree(plain);
	return code;
}
s32
krb5_make_seq_num(struct krb5_ctx *kctx,
		struct crypto_skcipher *key,
=======
s32
krb5_make_seq_num(struct krb5_ctx *kctx,
		struct crypto_sync_skcipher *key,
>>>>>>> upstream/android-13
		int direction,
		u32 seqnum,
		unsigned char *cksum, unsigned char *buf)
{
	unsigned char *plain;
	s32 code;

<<<<<<< HEAD
	if (kctx->enctype == ENCTYPE_ARCFOUR_HMAC)
		return krb5_make_rc4_seq_num(kctx, direction, seqnum,
					     cksum, buf);

=======
>>>>>>> upstream/android-13
	plain = kmalloc(8, GFP_NOFS);
	if (!plain)
		return -ENOMEM;

	plain[0] = (unsigned char) (seqnum & 0xff);
	plain[1] = (unsigned char) ((seqnum >> 8) & 0xff);
	plain[2] = (unsigned char) ((seqnum >> 16) & 0xff);
	plain[3] = (unsigned char) ((seqnum >> 24) & 0xff);

	plain[4] = direction;
	plain[5] = direction;
	plain[6] = direction;
	plain[7] = direction;

	code = krb5_encrypt(key, cksum, plain, buf, 8);
	kfree(plain);
	return code;
}

<<<<<<< HEAD
static s32
krb5_get_rc4_seq_num(struct krb5_ctx *kctx, unsigned char *cksum,
		     unsigned char *buf, int *direction, s32 *seqnum)
{
	struct crypto_skcipher *cipher;
	unsigned char *plain;
	s32 code;

	dprintk("RPC:       %s:\n", __func__);
	cipher = crypto_alloc_skcipher(kctx->gk5e->encrypt_name, 0,
				       CRYPTO_ALG_ASYNC);
	if (IS_ERR(cipher))
		return PTR_ERR(cipher);

	code = krb5_rc4_setup_seq_key(kctx, cipher, cksum);
	if (code)
		goto out;

	plain = kmalloc(8, GFP_NOFS);
	if (!plain) {
		code = -ENOMEM;
		goto out;
	}

	code = krb5_decrypt(cipher, cksum, buf, plain, 8);
	if (code)
		goto out_plain;

	if ((plain[4] != plain[5]) || (plain[4] != plain[6])
				   || (plain[4] != plain[7])) {
		code = (s32)KG_BAD_SEQ;
		goto out_plain;
	}

	*direction = plain[4];

	*seqnum = ((plain[0] << 24) | (plain[1] << 16) |
					(plain[2] << 8) | (plain[3]));
out_plain:
	kfree(plain);
out:
	crypto_free_skcipher(cipher);
	return code;
}

=======
>>>>>>> upstream/android-13
s32
krb5_get_seq_num(struct krb5_ctx *kctx,
	       unsigned char *cksum,
	       unsigned char *buf,
	       int *direction, u32 *seqnum)
{
	s32 code;
<<<<<<< HEAD
	struct crypto_skcipher *key = kctx->seq;
	unsigned char *plain;

	dprintk("RPC:       krb5_get_seq_num:\n");

	if (kctx->enctype == ENCTYPE_ARCFOUR_HMAC)
		return krb5_get_rc4_seq_num(kctx, cksum, buf,
					    direction, seqnum);
=======
	unsigned char *plain;
	struct crypto_sync_skcipher *key = kctx->seq;

	dprintk("RPC:       krb5_get_seq_num:\n");

>>>>>>> upstream/android-13
	plain = kmalloc(8, GFP_NOFS);
	if (!plain)
		return -ENOMEM;

	if ((code = krb5_decrypt(key, cksum, buf, plain, 8)))
		goto out;

	if ((plain[4] != plain[5]) || (plain[4] != plain[6]) ||
	    (plain[4] != plain[7])) {
		code = (s32)KG_BAD_SEQ;
		goto out;
	}

	*direction = plain[4];

	*seqnum = ((plain[0]) |
		   (plain[1] << 8) | (plain[2] << 16) | (plain[3] << 24));

out:
	kfree(plain);
	return code;
}
