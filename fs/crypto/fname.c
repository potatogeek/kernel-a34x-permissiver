// SPDX-License-Identifier: GPL-2.0
/*
 * This contains functions for filename crypto management
 *
 * Copyright (C) 2015, Google, Inc.
 * Copyright (C) 2015, Motorola Mobility
 *
 * Written by Uday Savagaonkar, 2014.
 * Modified by Jaegeuk Kim, 2015.
 *
 * This has not yet undergone a rigorous security audit.
 */

#include <linux/namei.h>
#include <linux/scatterlist.h>
#include <crypto/hash.h>
<<<<<<< HEAD
#include <crypto/sha.h>
=======
#include <crypto/sha2.h>
>>>>>>> upstream/android-13
#include <crypto/skcipher.h>
#include "fscrypt_private.h"

/*
 * struct fscrypt_nokey_name - identifier for directory entry when key is absent
 *
 * When userspace lists an encrypted directory without access to the key, the
 * filesystem must present a unique "no-key name" for each filename that allows
 * it to find the directory entry again if requested.  Naively, that would just
 * mean using the ciphertext filenames.  However, since the ciphertext filenames
 * can contain illegal characters ('\0' and '/'), they must be encoded in some
<<<<<<< HEAD
 * way.  We use base64.  But that can cause names to exceed NAME_MAX (255
=======
 * way.  We use base64url.  But that can cause names to exceed NAME_MAX (255
>>>>>>> upstream/android-13
 * bytes), so we also need to use a strong hash to abbreviate long names.
 *
 * The filesystem may also need another kind of hash, the "dirhash", to quickly
 * find the directory entry.  Since filesystems normally compute the dirhash
 * over the on-disk filename (i.e. the ciphertext), it's not computable from
 * no-key names that abbreviate the ciphertext using the strong hash to fit in
 * NAME_MAX.  It's also not computable if it's a keyed hash taken over the
 * plaintext (but it may still be available in the on-disk directory entry);
 * casefolded directories use this type of dirhash.  At least in these cases,
 * each no-key name must include the name's dirhash too.
 *
<<<<<<< HEAD
 * To meet all these requirements, we base64-encode the following
=======
 * To meet all these requirements, we base64url-encode the following
>>>>>>> upstream/android-13
 * variable-length structure.  It contains the dirhash, or 0's if the filesystem
 * didn't provide one; up to 149 bytes of the ciphertext name; and for
 * ciphertexts longer than 149 bytes, also the SHA-256 of the remaining bytes.
 *
 * This ensures that each no-key name contains everything needed to find the
 * directory entry again, contains only legal characters, doesn't exceed
 * NAME_MAX, is unambiguous unless there's a SHA-256 collision, and that we only
 * take the performance hit of SHA-256 on very long filenames (which are rare).
 */
struct fscrypt_nokey_name {
	u32 dirhash[2];
	u8 bytes[149];
	u8 sha256[SHA256_DIGEST_SIZE];
<<<<<<< HEAD
}; /* 189 bytes => 252 bytes base64-encoded, which is <= NAME_MAX (255) */

/*
 * Decoded size of max-size nokey name, i.e. a name that was abbreviated using
=======
}; /* 189 bytes => 252 bytes base64url-encoded, which is <= NAME_MAX (255) */

/*
 * Decoded size of max-size no-key name, i.e. a name that was abbreviated using
>>>>>>> upstream/android-13
 * the strong hash and thus includes the 'sha256' field.  This isn't simply
 * sizeof(struct fscrypt_nokey_name), as the padding at the end isn't included.
 */
#define FSCRYPT_NOKEY_NAME_MAX	offsetofend(struct fscrypt_nokey_name, sha256)

<<<<<<< HEAD
static struct crypto_shash *sha256_hash_tfm;

static int fscrypt_do_sha256(const u8 *data, unsigned int data_len, u8 *result)
{
	struct crypto_shash *tfm = READ_ONCE(sha256_hash_tfm);

	if (unlikely(!tfm)) {
		struct crypto_shash *prev_tfm;

		tfm = crypto_alloc_shash("sha256", 0, 0);
		if (IS_ERR(tfm)) {
			fscrypt_err(NULL,
				    "Error allocating SHA-256 transform: %ld",
				    PTR_ERR(tfm));
			return PTR_ERR(tfm);
		}
		prev_tfm = cmpxchg(&sha256_hash_tfm, NULL, tfm);
		if (prev_tfm) {
			crypto_free_shash(tfm);
			tfm = prev_tfm;
		}
	}
	{
		SHASH_DESC_ON_STACK(desc, tfm);

		desc->tfm = tfm;
		desc->flags = 0;

		return crypto_shash_digest(desc, data, data_len, result);
	}
}
=======
/* Encoded size of max-size no-key name */
#define FSCRYPT_NOKEY_NAME_MAX_ENCODED \
		FSCRYPT_BASE64URL_CHARS(FSCRYPT_NOKEY_NAME_MAX)
>>>>>>> upstream/android-13

static inline bool fscrypt_is_dot_dotdot(const struct qstr *str)
{
	if (str->len == 1 && str->name[0] == '.')
		return true;

	if (str->len == 2 && str->name[0] == '.' && str->name[1] == '.')
		return true;

	return false;
}

/**
 * fscrypt_fname_encrypt() - encrypt a filename
 * @inode: inode of the parent directory (for regular filenames)
 *	   or of the symlink (for symlink targets)
 * @iname: the filename to encrypt
 * @out: (output) the encrypted filename
 * @olen: size of the encrypted filename.  It must be at least @iname->len.
 *	  Any extra space is filled with NUL padding before encryption.
 *
 * Return: 0 on success, -errno on failure
 */
int fscrypt_fname_encrypt(const struct inode *inode, const struct qstr *iname,
			  u8 *out, unsigned int olen)
{
	struct skcipher_request *req = NULL;
	DECLARE_CRYPTO_WAIT(wait);
	const struct fscrypt_info *ci = inode->i_crypt_info;
<<<<<<< HEAD
	struct crypto_skcipher *tfm = ci->ci_key.tfm;
=======
	struct crypto_skcipher *tfm = ci->ci_enc_key.tfm;
>>>>>>> upstream/android-13
	union fscrypt_iv iv;
	struct scatterlist sg;
	int res;

	/*
	 * Copy the filename to the output buffer for encrypting in-place and
	 * pad it with the needed number of NUL bytes.
	 */
	if (WARN_ON(olen < iname->len))
		return -ENOBUFS;
	memcpy(out, iname->name, iname->len);
	memset(out + iname->len, 0, olen - iname->len);

	/* Initialize the IV */
	fscrypt_generate_iv(&iv, 0, ci);

	/* Set up the encryption request */
	req = skcipher_request_alloc(tfm, GFP_NOFS);
	if (!req)
		return -ENOMEM;
	skcipher_request_set_callback(req,
			CRYPTO_TFM_REQ_MAY_BACKLOG | CRYPTO_TFM_REQ_MAY_SLEEP,
			crypto_req_done, &wait);
	sg_init_one(&sg, out, olen);
	skcipher_request_set_crypt(req, &sg, &sg, olen, &iv);

	/* Do the encryption */
	res = crypto_wait_req(crypto_skcipher_encrypt(req), &wait);
	skcipher_request_free(req);
	if (res < 0) {
		fscrypt_err(inode, "Filename encryption failed: %d", res);
		return res;
	}

	return 0;
}

/**
 * fname_decrypt() - decrypt a filename
 * @inode: inode of the parent directory (for regular filenames)
 *	   or of the symlink (for symlink targets)
 * @iname: the encrypted filename to decrypt
 * @oname: (output) the decrypted filename.  The caller must have allocated
 *	   enough space for this, e.g. using fscrypt_fname_alloc_buffer().
 *
 * Return: 0 on success, -errno on failure
 */
static int fname_decrypt(const struct inode *inode,
			 const struct fscrypt_str *iname,
			 struct fscrypt_str *oname)
{
	struct skcipher_request *req = NULL;
	DECLARE_CRYPTO_WAIT(wait);
	struct scatterlist src_sg, dst_sg;
	const struct fscrypt_info *ci = inode->i_crypt_info;
<<<<<<< HEAD
	struct crypto_skcipher *tfm = ci->ci_key.tfm;
=======
	struct crypto_skcipher *tfm = ci->ci_enc_key.tfm;
>>>>>>> upstream/android-13
	union fscrypt_iv iv;
	int res;

	/* Allocate request */
	req = skcipher_request_alloc(tfm, GFP_NOFS);
	if (!req)
		return -ENOMEM;
	skcipher_request_set_callback(req,
		CRYPTO_TFM_REQ_MAY_BACKLOG | CRYPTO_TFM_REQ_MAY_SLEEP,
		crypto_req_done, &wait);

	/* Initialize IV */
	fscrypt_generate_iv(&iv, 0, ci);

	/* Create decryption request */
	sg_init_one(&src_sg, iname->name, iname->len);
	sg_init_one(&dst_sg, oname->name, oname->len);
	skcipher_request_set_crypt(req, &src_sg, &dst_sg, iname->len, &iv);
	res = crypto_wait_req(crypto_skcipher_decrypt(req), &wait);
	skcipher_request_free(req);
	if (res < 0) {
		fscrypt_err(inode, "Filename decryption failed: %d", res);
		return res;
	}

	oname->len = strnlen(oname->name, iname->len);
	return 0;
}

<<<<<<< HEAD
static const char lookup_table[65] =
	"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+,";

#define BASE64_CHARS(nbytes)	DIV_ROUND_UP((nbytes) * 4, 3)

/**
 * base64_encode() - base64-encode some bytes
 * @src: the bytes to encode
 * @len: number of bytes to encode
 * @dst: (output) the base64-encoded string.  Not NUL-terminated.
 *
 * Encodes the input string using characters from the set [A-Za-z0-9+,].
 * The encoded string is roughly 4/3 times the size of the input string.
 *
 * Return: length of the encoded string
 */
static int base64_encode(const u8 *src, int len, char *dst)
{
	int i, bits = 0, ac = 0;
	char *cp = dst;

	for (i = 0; i < len; i++) {
		ac += src[i] << bits;
		bits += 8;
		do {
			*cp++ = lookup_table[ac & 0x3f];
			ac >>= 6;
			bits -= 6;
		} while (bits >= 6);
	}
	if (bits)
		*cp++ = lookup_table[ac & 0x3f];
	return cp - dst;
}

static int base64_decode(const char *src, int len, u8 *dst)
{
	int i, bits = 0, ac = 0;
	const char *p;
	u8 *cp = dst;

	for (i = 0; i < len; i++) {
		p = strchr(lookup_table, src[i]);
		if (p == NULL || src[i] == 0)
			return -2;
		ac += (p - lookup_table) << bits;
		bits += 6;
		if (bits >= 8) {
			*cp++ = ac & 0xff;
			ac >>= 8;
			bits -= 8;
		}
	}
	if (ac)
		return -1;
	return cp - dst;
}

bool fscrypt_fname_encrypted_size(const struct inode *inode, u32 orig_len,
				  u32 max_len, u32 *encrypted_len_ret)
{
	const struct fscrypt_info *ci = inode->i_crypt_info;
	int padding = 4 << (fscrypt_policy_flags(&ci->ci_policy) &
=======
static const char base64url_table[65] =
	"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_";

#define FSCRYPT_BASE64URL_CHARS(nbytes)	DIV_ROUND_UP((nbytes) * 4, 3)

/**
 * fscrypt_base64url_encode() - base64url-encode some binary data
 * @src: the binary data to encode
 * @srclen: the length of @src in bytes
 * @dst: (output) the base64url-encoded string.  Not NUL-terminated.
 *
 * Encodes data using base64url encoding, i.e. the "Base 64 Encoding with URL
 * and Filename Safe Alphabet" specified by RFC 4648.  '='-padding isn't used,
 * as it's unneeded and not required by the RFC.  base64url is used instead of
 * base64 to avoid the '/' character, which isn't allowed in filenames.
 *
 * Return: the length of the resulting base64url-encoded string in bytes.
 *	   This will be equal to FSCRYPT_BASE64URL_CHARS(srclen).
 */
static int fscrypt_base64url_encode(const u8 *src, int srclen, char *dst)
{
	u32 ac = 0;
	int bits = 0;
	int i;
	char *cp = dst;

	for (i = 0; i < srclen; i++) {
		ac = (ac << 8) | src[i];
		bits += 8;
		do {
			bits -= 6;
			*cp++ = base64url_table[(ac >> bits) & 0x3f];
		} while (bits >= 6);
	}
	if (bits)
		*cp++ = base64url_table[(ac << (6 - bits)) & 0x3f];
	return cp - dst;
}

/**
 * fscrypt_base64url_decode() - base64url-decode a string
 * @src: the string to decode.  Doesn't need to be NUL-terminated.
 * @srclen: the length of @src in bytes
 * @dst: (output) the decoded binary data
 *
 * Decodes a string using base64url encoding, i.e. the "Base 64 Encoding with
 * URL and Filename Safe Alphabet" specified by RFC 4648.  '='-padding isn't
 * accepted, nor are non-encoding characters such as whitespace.
 *
 * This implementation hasn't been optimized for performance.
 *
 * Return: the length of the resulting decoded binary data in bytes,
 *	   or -1 if the string isn't a valid base64url string.
 */
static int fscrypt_base64url_decode(const char *src, int srclen, u8 *dst)
{
	u32 ac = 0;
	int bits = 0;
	int i;
	u8 *bp = dst;

	for (i = 0; i < srclen; i++) {
		const char *p = strchr(base64url_table, src[i]);

		if (p == NULL || src[i] == 0)
			return -1;
		ac = (ac << 6) | (p - base64url_table);
		bits += 6;
		if (bits >= 8) {
			bits -= 8;
			*bp++ = (u8)(ac >> bits);
		}
	}
	if (ac & ((1 << bits) - 1))
		return -1;
	return bp - dst;
}

bool fscrypt_fname_encrypted_size(const union fscrypt_policy *policy,
				  u32 orig_len, u32 max_len,
				  u32 *encrypted_len_ret)
{
	int padding = 4 << (fscrypt_policy_flags(policy) &
>>>>>>> upstream/android-13
			    FSCRYPT_POLICY_FLAGS_PAD_MASK);
	u32 encrypted_len;

	if (orig_len > max_len)
		return false;
	encrypted_len = max(orig_len, (u32)FS_CRYPTO_BLOCK_SIZE);
	encrypted_len = round_up(encrypted_len, padding);
	*encrypted_len_ret = min(encrypted_len, max_len);
	return true;
}

/**
 * fscrypt_fname_alloc_buffer() - allocate a buffer for presented filenames
<<<<<<< HEAD
 * @inode: inode of the parent directory (for regular filenames)
 *	   or of the symlink (for symlink targets)
=======
>>>>>>> upstream/android-13
 * @max_encrypted_len: maximum length of encrypted filenames the buffer will be
 *		       used to present
 * @crypto_str: (output) buffer to allocate
 *
 * Allocate a buffer that is large enough to hold any decrypted or encoded
 * filename (null-terminated), for the given maximum encrypted filename length.
 *
 * Return: 0 on success, -errno on failure
 */
<<<<<<< HEAD
int fscrypt_fname_alloc_buffer(const struct inode *inode,
			       u32 max_encrypted_len,
			       struct fscrypt_str *crypto_str)
{
	const u32 max_encoded_len = BASE64_CHARS(FSCRYPT_NOKEY_NAME_MAX);
	u32 max_presented_len;

	max_presented_len = max(max_encoded_len, max_encrypted_len);
=======
int fscrypt_fname_alloc_buffer(u32 max_encrypted_len,
			       struct fscrypt_str *crypto_str)
{
	u32 max_presented_len = max_t(u32, FSCRYPT_NOKEY_NAME_MAX_ENCODED,
				      max_encrypted_len);
>>>>>>> upstream/android-13

	crypto_str->name = kmalloc(max_presented_len + 1, GFP_NOFS);
	if (!crypto_str->name)
		return -ENOMEM;
	crypto_str->len = max_presented_len;
	return 0;
}
EXPORT_SYMBOL(fscrypt_fname_alloc_buffer);

/**
 * fscrypt_fname_free_buffer() - free a buffer for presented filenames
 * @crypto_str: the buffer to free
 *
 * Free a buffer that was allocated by fscrypt_fname_alloc_buffer().
 */
void fscrypt_fname_free_buffer(struct fscrypt_str *crypto_str)
{
	if (!crypto_str)
		return;
	kfree(crypto_str->name);
	crypto_str->name = NULL;
}
EXPORT_SYMBOL(fscrypt_fname_free_buffer);

/**
 * fscrypt_fname_disk_to_usr() - convert an encrypted filename to
 *				 user-presentable form
 * @inode: inode of the parent directory (for regular filenames)
 *	   or of the symlink (for symlink targets)
 * @hash: first part of the name's dirhash, if applicable.  This only needs to
 *	  be provided if the filename is located in an indexed directory whose
 *	  encryption key may be unavailable.  Not needed for symlink targets.
 * @minor_hash: second part of the name's dirhash, if applicable
 * @iname: encrypted filename to convert.  May also be "." or "..", which
 *	   aren't actually encrypted.
 * @oname: output buffer for the user-presentable filename.  The caller must
 *	   have allocated enough space for this, e.g. using
 *	   fscrypt_fname_alloc_buffer().
 *
 * If the key is available, we'll decrypt the disk name.  Otherwise, we'll
 * encode it for presentation in fscrypt_nokey_name format.
 * See struct fscrypt_nokey_name for details.
 *
 * Return: 0 on success, -errno on failure
 */
int fscrypt_fname_disk_to_usr(const struct inode *inode,
			      u32 hash, u32 minor_hash,
			      const struct fscrypt_str *iname,
			      struct fscrypt_str *oname)
{
	const struct qstr qname = FSTR_TO_QSTR(iname);
	struct fscrypt_nokey_name nokey_name;
	u32 size; /* size of the unencoded no-key name */
<<<<<<< HEAD
	int err;
=======
>>>>>>> upstream/android-13

	if (fscrypt_is_dot_dotdot(&qname)) {
		oname->name[0] = '.';
		oname->name[iname->len - 1] = '.';
		oname->len = iname->len;
		return 0;
	}

	if (iname->len < FS_CRYPTO_BLOCK_SIZE)
		return -EUCLEAN;

	if (fscrypt_has_encryption_key(inode))
		return fname_decrypt(inode, iname, oname);

	/*
	 * Sanity check that struct fscrypt_nokey_name doesn't have padding
	 * between fields and that its encoded size never exceeds NAME_MAX.
	 */
	BUILD_BUG_ON(offsetofend(struct fscrypt_nokey_name, dirhash) !=
		     offsetof(struct fscrypt_nokey_name, bytes));
	BUILD_BUG_ON(offsetofend(struct fscrypt_nokey_name, bytes) !=
		     offsetof(struct fscrypt_nokey_name, sha256));
<<<<<<< HEAD
	BUILD_BUG_ON(BASE64_CHARS(FSCRYPT_NOKEY_NAME_MAX) > NAME_MAX);

	if (hash) {
		nokey_name.dirhash[0] = hash;
		nokey_name.dirhash[1] = minor_hash;
	} else {
		nokey_name.dirhash[0] = 0;
		nokey_name.dirhash[1] = 0;
	}
=======
	BUILD_BUG_ON(FSCRYPT_NOKEY_NAME_MAX_ENCODED > NAME_MAX);

	nokey_name.dirhash[0] = hash;
	nokey_name.dirhash[1] = minor_hash;

>>>>>>> upstream/android-13
	if (iname->len <= sizeof(nokey_name.bytes)) {
		memcpy(nokey_name.bytes, iname->name, iname->len);
		size = offsetof(struct fscrypt_nokey_name, bytes[iname->len]);
	} else {
		memcpy(nokey_name.bytes, iname->name, sizeof(nokey_name.bytes));
		/* Compute strong hash of remaining part of name. */
<<<<<<< HEAD
		err = fscrypt_do_sha256(&iname->name[sizeof(nokey_name.bytes)],
					iname->len - sizeof(nokey_name.bytes),
					nokey_name.sha256);
		if (err)
			return err;
		size = FSCRYPT_NOKEY_NAME_MAX;
	}
	oname->len = base64_encode((const u8 *)&nokey_name, size, oname->name);
=======
		sha256(&iname->name[sizeof(nokey_name.bytes)],
		       iname->len - sizeof(nokey_name.bytes),
		       nokey_name.sha256);
		size = FSCRYPT_NOKEY_NAME_MAX;
	}
	oname->len = fscrypt_base64url_encode((const u8 *)&nokey_name, size,
					      oname->name);
>>>>>>> upstream/android-13
	return 0;
}
EXPORT_SYMBOL(fscrypt_fname_disk_to_usr);

/**
 * fscrypt_setup_filename() - prepare to search a possibly encrypted directory
 * @dir: the directory that will be searched
 * @iname: the user-provided filename being searched for
 * @lookup: 1 if we're allowed to proceed without the key because it's
 *	->lookup() or we're finding the dir_entry for deletion; 0 if we cannot
 *	proceed without the key because we're going to create the dir_entry.
 * @fname: the filename information to be filled in
 *
 * Given a user-provided filename @iname, this function sets @fname->disk_name
 * to the name that would be stored in the on-disk directory entry, if possible.
 * If the directory is unencrypted this is simply @iname.  Else, if we have the
 * directory's encryption key, then @iname is the plaintext, so we encrypt it to
 * get the disk_name.
 *
<<<<<<< HEAD
 * Else, for keyless @lookup operations, @iname is the presented ciphertext, so
 * we decode it to get the fscrypt_nokey_name.  Non-@lookup operations will be
 * impossible in this case, so we fail them with ENOKEY.
=======
 * Else, for keyless @lookup operations, @iname should be a no-key name, so we
 * decode it to get the struct fscrypt_nokey_name.  Non-@lookup operations will
 * be impossible in this case, so we fail them with ENOKEY.
>>>>>>> upstream/android-13
 *
 * If successful, fscrypt_free_filename() must be called later to clean up.
 *
 * Return: 0 on success, -errno on failure
 */
int fscrypt_setup_filename(struct inode *dir, const struct qstr *iname,
			      int lookup, struct fscrypt_name *fname)
{
	struct fscrypt_nokey_name *nokey_name;
	int ret;

	memset(fname, 0, sizeof(struct fscrypt_name));
	fname->usr_fname = iname;

	if (!IS_ENCRYPTED(dir) || fscrypt_is_dot_dotdot(iname)) {
		fname->disk_name.name = (unsigned char *)iname->name;
		fname->disk_name.len = iname->len;
		return 0;
	}
<<<<<<< HEAD
	ret = fscrypt_get_encryption_info(dir);
=======
	ret = fscrypt_get_encryption_info(dir, lookup);
>>>>>>> upstream/android-13
	if (ret)
		return ret;

	if (fscrypt_has_encryption_key(dir)) {
<<<<<<< HEAD
		if (!fscrypt_fname_encrypted_size(dir, iname->len,
						  dir->i_sb->s_cop->max_namelen,
=======
		if (!fscrypt_fname_encrypted_size(&dir->i_crypt_info->ci_policy,
						  iname->len, NAME_MAX,
>>>>>>> upstream/android-13
						  &fname->crypto_buf.len))
			return -ENAMETOOLONG;
		fname->crypto_buf.name = kmalloc(fname->crypto_buf.len,
						 GFP_NOFS);
		if (!fname->crypto_buf.name)
			return -ENOMEM;

		ret = fscrypt_fname_encrypt(dir, iname, fname->crypto_buf.name,
					    fname->crypto_buf.len);
		if (ret)
			goto errout;
		fname->disk_name.name = fname->crypto_buf.name;
		fname->disk_name.len = fname->crypto_buf.len;
		return 0;
	}
	if (!lookup)
		return -ENOKEY;
<<<<<<< HEAD
	fname->is_ciphertext_name = true;
=======
	fname->is_nokey_name = true;
>>>>>>> upstream/android-13

	/*
	 * We don't have the key and we are doing a lookup; decode the
	 * user-supplied name
	 */

<<<<<<< HEAD
	if (iname->len > BASE64_CHARS(FSCRYPT_NOKEY_NAME_MAX))
=======
	if (iname->len > FSCRYPT_NOKEY_NAME_MAX_ENCODED)
>>>>>>> upstream/android-13
		return -ENOENT;

	fname->crypto_buf.name = kmalloc(FSCRYPT_NOKEY_NAME_MAX, GFP_KERNEL);
	if (fname->crypto_buf.name == NULL)
		return -ENOMEM;

<<<<<<< HEAD
	ret = base64_decode(iname->name, iname->len, fname->crypto_buf.name);
=======
	ret = fscrypt_base64url_decode(iname->name, iname->len,
				       fname->crypto_buf.name);
>>>>>>> upstream/android-13
	if (ret < (int)offsetof(struct fscrypt_nokey_name, bytes[1]) ||
	    (ret > offsetof(struct fscrypt_nokey_name, sha256) &&
	     ret != FSCRYPT_NOKEY_NAME_MAX)) {
		ret = -ENOENT;
		goto errout;
	}
	fname->crypto_buf.len = ret;

	nokey_name = (void *)fname->crypto_buf.name;
	fname->hash = nokey_name->dirhash[0];
	fname->minor_hash = nokey_name->dirhash[1];
	if (ret != FSCRYPT_NOKEY_NAME_MAX) {
		/* The full ciphertext filename is available. */
		fname->disk_name.name = nokey_name->bytes;
		fname->disk_name.len =
			ret - offsetof(struct fscrypt_nokey_name, bytes);
	}
	return 0;

errout:
	kfree(fname->crypto_buf.name);
	return ret;
}
EXPORT_SYMBOL(fscrypt_setup_filename);

/**
 * fscrypt_match_name() - test whether the given name matches a directory entry
 * @fname: the name being searched for
 * @de_name: the name from the directory entry
 * @de_name_len: the length of @de_name in bytes
 *
 * Normally @fname->disk_name will be set, and in that case we simply compare
 * that to the name stored in the directory entry.  The only exception is that
 * if we don't have the key for an encrypted directory and the name we're
 * looking for is very long, then we won't have the full disk_name and instead
 * we'll need to match against a fscrypt_nokey_name that includes a strong hash.
 *
 * Return: %true if the name matches, otherwise %false.
 */
bool fscrypt_match_name(const struct fscrypt_name *fname,
			const u8 *de_name, u32 de_name_len)
{
	const struct fscrypt_nokey_name *nokey_name =
		(const void *)fname->crypto_buf.name;
<<<<<<< HEAD
	u8 sha256[SHA256_DIGEST_SIZE];
=======
	u8 digest[SHA256_DIGEST_SIZE];
>>>>>>> upstream/android-13

	if (likely(fname->disk_name.name)) {
		if (de_name_len != fname->disk_name.len)
			return false;
		return !memcmp(de_name, fname->disk_name.name, de_name_len);
	}
	if (de_name_len <= sizeof(nokey_name->bytes))
		return false;
	if (memcmp(de_name, nokey_name->bytes, sizeof(nokey_name->bytes)))
		return false;
<<<<<<< HEAD
	if (fscrypt_do_sha256(&de_name[sizeof(nokey_name->bytes)],
			      de_name_len - sizeof(nokey_name->bytes), sha256))
		return false;
	return !memcmp(sha256, nokey_name->sha256, sizeof(sha256));
=======
	sha256(&de_name[sizeof(nokey_name->bytes)],
	       de_name_len - sizeof(nokey_name->bytes), digest);
	return !memcmp(digest, nokey_name->sha256, sizeof(digest));
>>>>>>> upstream/android-13
}
EXPORT_SYMBOL_GPL(fscrypt_match_name);

/**
 * fscrypt_fname_siphash() - calculate the SipHash of a filename
 * @dir: the parent directory
 * @name: the filename to calculate the SipHash of
 *
 * Given a plaintext filename @name and a directory @dir which uses SipHash as
 * its dirhash method and has had its fscrypt key set up, this function
 * calculates the SipHash of that name using the directory's secret dirhash key.
 *
 * Return: the SipHash of @name using the hash key of @dir
 */
u64 fscrypt_fname_siphash(const struct inode *dir, const struct qstr *name)
{
	const struct fscrypt_info *ci = dir->i_crypt_info;

	WARN_ON(!ci->ci_dirhash_key_initialized);

	return siphash(name->name, name->len, &ci->ci_dirhash_key);
}
EXPORT_SYMBOL_GPL(fscrypt_fname_siphash);

/*
 * Validate dentries in encrypted directories to make sure we aren't potentially
 * caching stale dentries after a key has been added.
 */
int fscrypt_d_revalidate(struct dentry *dentry, unsigned int flags)
{
	struct dentry *dir;
	int err;
	int valid;

	/*
	 * Plaintext names are always valid, since fscrypt doesn't support
<<<<<<< HEAD
	 * reverting to ciphertext names without evicting the directory's inode
	 * -- which implies eviction of the dentries in the directory.
	 */
	if (!(dentry->d_flags & DCACHE_ENCRYPTED_NAME))
		return 1;

	/*
	 * Ciphertext name; valid if the directory's key is still unavailable.
	 *
	 * Although fscrypt forbids rename() on ciphertext names, we still must
	 * use dget_parent() here rather than use ->d_parent directly.  That's
=======
	 * reverting to no-key names without evicting the directory's inode
	 * -- which implies eviction of the dentries in the directory.
	 */
	if (!(dentry->d_flags & DCACHE_NOKEY_NAME))
		return 1;

	/*
	 * No-key name; valid if the directory's key is still unavailable.
	 *
	 * Although fscrypt forbids rename() on no-key names, we still must use
	 * dget_parent() here rather than use ->d_parent directly.  That's
>>>>>>> upstream/android-13
	 * because a corrupted fs image may contain directory hard links, which
	 * the VFS handles by moving the directory's dentry tree in the dcache
	 * each time ->lookup() finds the directory and it already has a dentry
	 * elsewhere.  Thus ->d_parent can be changing, and we must safely grab
	 * a reference to some ->d_parent to prevent it from being freed.
	 */

	if (flags & LOOKUP_RCU)
		return -ECHILD;

	dir = dget_parent(dentry);
<<<<<<< HEAD
	err = fscrypt_get_encryption_info(d_inode(dir));
=======
	/*
	 * Pass allow_unsupported=true, so that files with an unsupported
	 * encryption policy can be deleted.
	 */
	err = fscrypt_get_encryption_info(d_inode(dir), true);
>>>>>>> upstream/android-13
	valid = !fscrypt_has_encryption_key(d_inode(dir));
	dput(dir);

	if (err < 0)
		return err;

	return valid;
}
EXPORT_SYMBOL_GPL(fscrypt_d_revalidate);
