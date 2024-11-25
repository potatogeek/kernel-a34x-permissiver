<<<<<<< HEAD
/* Large capacity key type
 *
 * Copyright (C) 2017 Jason A. Donenfeld <Jason@zx2c4.com>. All Rights Reserved.
 * Copyright (C) 2013 Red Hat, Inc. All Rights Reserved.
 * Written by David Howells (dhowells@redhat.com)
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public Licence
 * as published by the Free Software Foundation; either version
 * 2 of the Licence, or (at your option) any later version.
=======
// SPDX-License-Identifier: GPL-2.0-or-later
/* Large capacity key type
 *
 * Copyright (C) 2017-2020 Jason A. Donenfeld <Jason@zx2c4.com>. All Rights Reserved.
 * Copyright (C) 2013 Red Hat, Inc. All Rights Reserved.
 * Written by David Howells (dhowells@redhat.com)
>>>>>>> upstream/android-13
 */

#define pr_fmt(fmt) "big_key: "fmt
#include <linux/init.h>
#include <linux/seq_file.h>
#include <linux/file.h>
#include <linux/shmem_fs.h>
#include <linux/err.h>
<<<<<<< HEAD
#include <linux/scatterlist.h>
#include <linux/random.h>
#include <linux/vmalloc.h>
#include <keys/user-type.h>
#include <keys/big_key-type.h>
#include <crypto/aead.h>
#include <crypto/gcm.h>

struct big_key_buf {
	unsigned int		nr_pages;
	void			*virt;
	struct scatterlist	*sg;
	struct page		*pages[];
};
=======
#include <linux/random.h>
#include <keys/user-type.h>
#include <keys/big_key-type.h>
#include <crypto/chacha20poly1305.h>
>>>>>>> upstream/android-13

/*
 * Layout of key payload words.
 */
enum {
	big_key_data,
	big_key_path,
	big_key_path_2nd_part,
	big_key_len,
};

/*
<<<<<<< HEAD
 * Crypto operation with big_key data
 */
enum big_key_op {
	BIG_KEY_ENC,
	BIG_KEY_DEC,
};

/*
=======
>>>>>>> upstream/android-13
 * If the data is under this limit, there's no point creating a shm file to
 * hold it as the permanently resident metadata for the shmem fs will be at
 * least as large as the data.
 */
#define BIG_KEY_FILE_THRESHOLD (sizeof(struct inode) + sizeof(struct dentry))

/*
<<<<<<< HEAD
 * Key size for big_key data encryption
 */
#define ENC_KEY_SIZE 32

/*
 * Authentication tag length
 */
#define ENC_AUTHTAG_SIZE 16

/*
=======
>>>>>>> upstream/android-13
 * big_key defined keys take an arbitrary string as the description and an
 * arbitrary blob of data as the payload
 */
struct key_type key_type_big_key = {
	.name			= "big_key",
	.preparse		= big_key_preparse,
	.free_preparse		= big_key_free_preparse,
	.instantiate		= generic_key_instantiate,
	.revoke			= big_key_revoke,
	.destroy		= big_key_destroy,
	.describe		= big_key_describe,
	.read			= big_key_read,
<<<<<<< HEAD
	/* no ->update(); don't add it without changing big_key_crypt() nonce */
};

/*
 * Crypto names for big_key data authenticated encryption
 */
static const char big_key_alg_name[] = "gcm(aes)";
#define BIG_KEY_IV_SIZE		GCM_AES_IV_SIZE

/*
 * Crypto algorithms for big_key data authenticated encryption
 */
static struct crypto_aead *big_key_aead;

/*
 * Since changing the key affects the entire object, we need a mutex.
 */
static DEFINE_MUTEX(big_key_aead_lock);

/*
 * Encrypt/decrypt big_key data
 */
static int big_key_crypt(enum big_key_op op, struct big_key_buf *buf, size_t datalen, u8 *key)
{
	int ret;
	struct aead_request *aead_req;
	/* We always use a zero nonce. The reason we can get away with this is
	 * because we're using a different randomly generated key for every
	 * different encryption. Notably, too, key_type_big_key doesn't define
	 * an .update function, so there's no chance we'll wind up reusing the
	 * key to encrypt updated data. Simply put: one key, one encryption.
	 */
	u8 zero_nonce[BIG_KEY_IV_SIZE];

	aead_req = aead_request_alloc(big_key_aead, GFP_KERNEL);
	if (!aead_req)
		return -ENOMEM;

	memset(zero_nonce, 0, sizeof(zero_nonce));
	aead_request_set_crypt(aead_req, buf->sg, buf->sg, datalen, zero_nonce);
	aead_request_set_callback(aead_req, CRYPTO_TFM_REQ_MAY_SLEEP, NULL, NULL);
	aead_request_set_ad(aead_req, 0);

	mutex_lock(&big_key_aead_lock);
	if (crypto_aead_setkey(big_key_aead, key, ENC_KEY_SIZE)) {
		ret = -EAGAIN;
		goto error;
	}
	if (op == BIG_KEY_ENC)
		ret = crypto_aead_encrypt(aead_req);
	else
		ret = crypto_aead_decrypt(aead_req);
error:
	mutex_unlock(&big_key_aead_lock);
	aead_request_free(aead_req);
	return ret;
}

/*
 * Free up the buffer.
 */
static void big_key_free_buffer(struct big_key_buf *buf)
{
	unsigned int i;

	if (buf->virt) {
		memset(buf->virt, 0, buf->nr_pages * PAGE_SIZE);
		vunmap(buf->virt);
	}

	for (i = 0; i < buf->nr_pages; i++)
		if (buf->pages[i])
			__free_page(buf->pages[i]);

	kfree(buf);
}

/*
 * Allocate a buffer consisting of a set of pages with a virtual mapping
 * applied over them.
 */
static void *big_key_alloc_buffer(size_t len)
{
	struct big_key_buf *buf;
	unsigned int npg = (len + PAGE_SIZE - 1) >> PAGE_SHIFT;
	unsigned int i, l;

	buf = kzalloc(sizeof(struct big_key_buf) +
		      sizeof(struct page) * npg +
		      sizeof(struct scatterlist) * npg,
		      GFP_KERNEL);
	if (!buf)
		return NULL;

	buf->nr_pages = npg;
	buf->sg = (void *)(buf->pages + npg);
	sg_init_table(buf->sg, npg);

	for (i = 0; i < buf->nr_pages; i++) {
		buf->pages[i] = alloc_page(GFP_KERNEL);
		if (!buf->pages[i])
			goto nomem;

		l = min_t(size_t, len, PAGE_SIZE);
		sg_set_page(&buf->sg[i], buf->pages[i], l, 0);
		len -= l;
	}

	buf->virt = vmap(buf->pages, buf->nr_pages, VM_MAP, PAGE_KERNEL);
	if (!buf->virt)
		goto nomem;

	return buf;

nomem:
	big_key_free_buffer(buf);
	return NULL;
}

/*
=======
	.update			= big_key_update,
};

/*
>>>>>>> upstream/android-13
 * Preparse a big key
 */
int big_key_preparse(struct key_preparsed_payload *prep)
{
<<<<<<< HEAD
	struct big_key_buf *buf;
	struct path *path = (struct path *)&prep->payload.data[big_key_path];
	struct file *file;
	u8 *enckey;
	ssize_t written;
	size_t datalen = prep->datalen, enclen = datalen + ENC_AUTHTAG_SIZE;
=======
	struct path *path = (struct path *)&prep->payload.data[big_key_path];
	struct file *file;
	u8 *buf, *enckey;
	ssize_t written;
	size_t datalen = prep->datalen;
	size_t enclen = datalen + CHACHA20POLY1305_AUTHTAG_SIZE;
>>>>>>> upstream/android-13
	int ret;

	if (datalen <= 0 || datalen > 1024 * 1024 || !prep->data)
		return -EINVAL;

	/* Set an arbitrary quota */
	prep->quotalen = 16;

	prep->payload.data[big_key_len] = (void *)(unsigned long)datalen;

	if (datalen > BIG_KEY_FILE_THRESHOLD) {
		/* Create a shmem file to store the data in.  This will permit the data
		 * to be swapped out if needed.
		 *
		 * File content is stored encrypted with randomly generated key.
<<<<<<< HEAD
		 */
		loff_t pos = 0;

		buf = big_key_alloc_buffer(enclen);
		if (!buf)
			return -ENOMEM;
		memcpy(buf->virt, prep->data, datalen);

		/* generate random key */
		enckey = kmalloc(ENC_KEY_SIZE, GFP_KERNEL);
=======
		 * Since the key is random for each file, we can set the nonce
		 * to zero, provided we never define a ->update() call.
		 */
		loff_t pos = 0;

		buf = kvmalloc(enclen, GFP_KERNEL);
		if (!buf)
			return -ENOMEM;

		/* generate random key */
		enckey = kmalloc(CHACHA20POLY1305_KEY_SIZE, GFP_KERNEL);
>>>>>>> upstream/android-13
		if (!enckey) {
			ret = -ENOMEM;
			goto error;
		}
<<<<<<< HEAD
		ret = get_random_bytes_wait(enckey, ENC_KEY_SIZE);
		if (unlikely(ret))
			goto err_enckey;

		/* encrypt aligned data */
		ret = big_key_crypt(BIG_KEY_ENC, buf, datalen, enckey);
		if (ret)
			goto err_enckey;
=======
		ret = get_random_bytes_wait(enckey, CHACHA20POLY1305_KEY_SIZE);
		if (unlikely(ret))
			goto err_enckey;

		/* encrypt data */
		chacha20poly1305_encrypt(buf, prep->data, datalen, NULL, 0,
					 0, enckey);
>>>>>>> upstream/android-13

		/* save aligned data to file */
		file = shmem_kernel_file_setup("", enclen, 0);
		if (IS_ERR(file)) {
			ret = PTR_ERR(file);
			goto err_enckey;
		}

<<<<<<< HEAD
		written = kernel_write(file, buf->virt, enclen, &pos);
		if (written != enclen) {
			ret = written;
			if (written >= 0)
				ret = -ENOMEM;
=======
		written = kernel_write(file, buf, enclen, &pos);
		if (written != enclen) {
			ret = written;
			if (written >= 0)
				ret = -EIO;
>>>>>>> upstream/android-13
			goto err_fput;
		}

		/* Pin the mount and dentry to the key so that we can open it again
		 * later
		 */
		prep->payload.data[big_key_data] = enckey;
		*path = file->f_path;
		path_get(path);
		fput(file);
<<<<<<< HEAD
		big_key_free_buffer(buf);
=======
		kvfree_sensitive(buf, enclen);
>>>>>>> upstream/android-13
	} else {
		/* Just store the data in a buffer */
		void *data = kmalloc(datalen, GFP_KERNEL);

		if (!data)
			return -ENOMEM;

		prep->payload.data[big_key_data] = data;
		memcpy(data, prep->data, prep->datalen);
	}
	return 0;

err_fput:
	fput(file);
err_enckey:
<<<<<<< HEAD
	kzfree(enckey);
error:
	big_key_free_buffer(buf);
=======
	kfree_sensitive(enckey);
error:
	kvfree_sensitive(buf, enclen);
>>>>>>> upstream/android-13
	return ret;
}

/*
 * Clear preparsement.
 */
void big_key_free_preparse(struct key_preparsed_payload *prep)
{
	if (prep->datalen > BIG_KEY_FILE_THRESHOLD) {
		struct path *path = (struct path *)&prep->payload.data[big_key_path];

		path_put(path);
	}
<<<<<<< HEAD
	kzfree(prep->payload.data[big_key_data]);
=======
	kfree_sensitive(prep->payload.data[big_key_data]);
>>>>>>> upstream/android-13
}

/*
 * dispose of the links from a revoked keyring
 * - called with the key sem write-locked
 */
void big_key_revoke(struct key *key)
{
	struct path *path = (struct path *)&key->payload.data[big_key_path];

	/* clear the quota */
	key_payload_reserve(key, 0);
	if (key_is_positive(key) &&
	    (size_t)key->payload.data[big_key_len] > BIG_KEY_FILE_THRESHOLD)
		vfs_truncate(path, 0);
}

/*
 * dispose of the data dangling from the corpse of a big_key key
 */
void big_key_destroy(struct key *key)
{
	size_t datalen = (size_t)key->payload.data[big_key_len];

	if (datalen > BIG_KEY_FILE_THRESHOLD) {
		struct path *path = (struct path *)&key->payload.data[big_key_path];

		path_put(path);
		path->mnt = NULL;
		path->dentry = NULL;
	}
<<<<<<< HEAD
	kzfree(key->payload.data[big_key_data]);
=======
	kfree_sensitive(key->payload.data[big_key_data]);
>>>>>>> upstream/android-13
	key->payload.data[big_key_data] = NULL;
}

/*
<<<<<<< HEAD
=======
 * Update a big key
 */
int big_key_update(struct key *key, struct key_preparsed_payload *prep)
{
	int ret;

	ret = key_payload_reserve(key, prep->datalen);
	if (ret < 0)
		return ret;

	if (key_is_positive(key))
		big_key_destroy(key);

	return generic_key_instantiate(key, prep);
}

/*
>>>>>>> upstream/android-13
 * describe the big_key key
 */
void big_key_describe(const struct key *key, struct seq_file *m)
{
	size_t datalen = (size_t)key->payload.data[big_key_len];

	seq_puts(m, key->description);

	if (key_is_positive(key))
		seq_printf(m, ": %zu [%s]",
			   datalen,
			   datalen > BIG_KEY_FILE_THRESHOLD ? "file" : "buff");
}

/*
 * read the key data
 * - the key's semaphore is read-locked
 */
long big_key_read(const struct key *key, char *buffer, size_t buflen)
{
	size_t datalen = (size_t)key->payload.data[big_key_len];
	long ret;

	if (!buffer || buflen < datalen)
		return datalen;

	if (datalen > BIG_KEY_FILE_THRESHOLD) {
<<<<<<< HEAD
		struct big_key_buf *buf;
		struct path *path = (struct path *)&key->payload.data[big_key_path];
		struct file *file;
		u8 *enckey = (u8 *)key->payload.data[big_key_data];
		size_t enclen = datalen + ENC_AUTHTAG_SIZE;
		loff_t pos = 0;

		buf = big_key_alloc_buffer(enclen);
=======
		struct path *path = (struct path *)&key->payload.data[big_key_path];
		struct file *file;
		u8 *buf, *enckey = (u8 *)key->payload.data[big_key_data];
		size_t enclen = datalen + CHACHA20POLY1305_AUTHTAG_SIZE;
		loff_t pos = 0;

		buf = kvmalloc(enclen, GFP_KERNEL);
>>>>>>> upstream/android-13
		if (!buf)
			return -ENOMEM;

		file = dentry_open(path, O_RDONLY, current_cred());
		if (IS_ERR(file)) {
			ret = PTR_ERR(file);
			goto error;
		}

		/* read file to kernel and decrypt */
<<<<<<< HEAD
		ret = kernel_read(file, buf->virt, enclen, &pos);
		if (ret >= 0 && ret != enclen) {
			ret = -EIO;
			goto err_fput;
		}

		ret = big_key_crypt(BIG_KEY_DEC, buf, enclen, enckey);
		if (ret)
=======
		ret = kernel_read(file, buf, enclen, &pos);
		if (ret != enclen) {
			if (ret >= 0)
				ret = -EIO;
			goto err_fput;
		}

		ret = chacha20poly1305_decrypt(buf, buf, enclen, NULL, 0, 0,
					       enckey) ? 0 : -EBADMSG;
		if (unlikely(ret))
>>>>>>> upstream/android-13
			goto err_fput;

		ret = datalen;

		/* copy out decrypted data */
<<<<<<< HEAD
		memcpy(buffer, buf->virt, datalen);
=======
		memcpy(buffer, buf, datalen);
>>>>>>> upstream/android-13

err_fput:
		fput(file);
error:
<<<<<<< HEAD
		big_key_free_buffer(buf);
=======
		kvfree_sensitive(buf, enclen);
>>>>>>> upstream/android-13
	} else {
		ret = datalen;
		memcpy(buffer, key->payload.data[big_key_data], datalen);
	}

	return ret;
}

/*
 * Register key type
 */
static int __init big_key_init(void)
{
<<<<<<< HEAD
	int ret;

	/* init block cipher */
	big_key_aead = crypto_alloc_aead(big_key_alg_name, 0, CRYPTO_ALG_ASYNC);
	if (IS_ERR(big_key_aead)) {
		ret = PTR_ERR(big_key_aead);
		pr_err("Can't alloc crypto: %d\n", ret);
		return ret;
	}

	if (unlikely(crypto_aead_ivsize(big_key_aead) != BIG_KEY_IV_SIZE)) {
		WARN(1, "big key algorithm changed?");
		ret = -EINVAL;
		goto free_aead;
	}

	ret = crypto_aead_setauthsize(big_key_aead, ENC_AUTHTAG_SIZE);
	if (ret < 0) {
		pr_err("Can't set crypto auth tag len: %d\n", ret);
		goto free_aead;
	}

	ret = register_key_type(&key_type_big_key);
	if (ret < 0) {
		pr_err("Can't register type: %d\n", ret);
		goto free_aead;
	}

	return 0;

free_aead:
	crypto_free_aead(big_key_aead);
	return ret;
=======
	return register_key_type(&key_type_big_key);
>>>>>>> upstream/android-13
}

late_initcall(big_key_init);
