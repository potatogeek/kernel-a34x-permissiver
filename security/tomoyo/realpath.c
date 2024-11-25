// SPDX-License-Identifier: GPL-2.0
/*
 * security/tomoyo/realpath.c
 *
 * Copyright (C) 2005-2011  NTT DATA CORPORATION
 */

#include "common.h"
#include <linux/magic.h>
<<<<<<< HEAD
=======
#include <linux/proc_fs.h>
>>>>>>> upstream/android-13

/**
 * tomoyo_encode2 - Encode binary string to ascii string.
 *
 * @str:     String in binary format.
 * @str_len: Size of @str in byte.
 *
 * Returns pointer to @str in ascii format on success, NULL otherwise.
 *
 * This function uses kzalloc(), so caller must kfree() if this function
 * didn't return NULL.
 */
char *tomoyo_encode2(const char *str, int str_len)
{
	int i;
	int len = 0;
	const char *p = str;
	char *cp;
	char *cp0;

	if (!p)
		return NULL;
	for (i = 0; i < str_len; i++) {
		const unsigned char c = p[i];

		if (c == '\\')
			len += 2;
		else if (c > ' ' && c < 127)
			len++;
		else
			len += 4;
	}
	len++;
	/* Reserve space for appending "/". */
	cp = kzalloc(len + 10, GFP_NOFS);
	if (!cp)
		return NULL;
	cp0 = cp;
	p = str;
	for (i = 0; i < str_len; i++) {
		const unsigned char c = p[i];

		if (c == '\\') {
			*cp++ = '\\';
			*cp++ = '\\';
		} else if (c > ' ' && c < 127) {
			*cp++ = c;
		} else {
			*cp++ = '\\';
			*cp++ = (c >> 6) + '0';
			*cp++ = ((c >> 3) & 7) + '0';
			*cp++ = (c & 7) + '0';
		}
	}
	return cp0;
}

/**
 * tomoyo_encode - Encode binary string to ascii string.
 *
 * @str: String in binary format.
 *
 * Returns pointer to @str in ascii format on success, NULL otherwise.
 *
 * This function uses kzalloc(), so caller must kfree() if this function
 * didn't return NULL.
 */
char *tomoyo_encode(const char *str)
{
	return str ? tomoyo_encode2(str, strlen(str)) : NULL;
}

/**
 * tomoyo_get_absolute_path - Get the path of a dentry but ignores chroot'ed root.
 *
 * @path:   Pointer to "struct path".
 * @buffer: Pointer to buffer to return value in.
 * @buflen: Sizeof @buffer.
 *
 * Returns the buffer on success, an error code otherwise.
 *
 * If dentry is a directory, trailing '/' is appended.
 */
static char *tomoyo_get_absolute_path(const struct path *path, char * const buffer,
				      const int buflen)
{
	char *pos = ERR_PTR(-ENOMEM);
<<<<<<< HEAD
=======

>>>>>>> upstream/android-13
	if (buflen >= 256) {
		/* go to whatever namespace root we are under */
		pos = d_absolute_path(path, buffer, buflen - 1);
		if (!IS_ERR(pos) && *pos == '/' && pos[1]) {
			struct inode *inode = d_backing_inode(path->dentry);
<<<<<<< HEAD
=======

>>>>>>> upstream/android-13
			if (inode && S_ISDIR(inode->i_mode)) {
				buffer[buflen - 2] = '/';
				buffer[buflen - 1] = '\0';
			}
		}
	}
	return pos;
}

/**
 * tomoyo_get_dentry_path - Get the path of a dentry.
 *
 * @dentry: Pointer to "struct dentry".
 * @buffer: Pointer to buffer to return value in.
 * @buflen: Sizeof @buffer.
 *
 * Returns the buffer on success, an error code otherwise.
 *
 * If dentry is a directory, trailing '/' is appended.
 */
static char *tomoyo_get_dentry_path(struct dentry *dentry, char * const buffer,
				    const int buflen)
{
	char *pos = ERR_PTR(-ENOMEM);
<<<<<<< HEAD
=======

>>>>>>> upstream/android-13
	if (buflen >= 256) {
		pos = dentry_path_raw(dentry, buffer, buflen - 1);
		if (!IS_ERR(pos) && *pos == '/' && pos[1]) {
			struct inode *inode = d_backing_inode(dentry);
<<<<<<< HEAD
=======

>>>>>>> upstream/android-13
			if (inode && S_ISDIR(inode->i_mode)) {
				buffer[buflen - 2] = '/';
				buffer[buflen - 1] = '\0';
			}
		}
	}
	return pos;
}

/**
 * tomoyo_get_local_path - Get the path of a dentry.
 *
 * @dentry: Pointer to "struct dentry".
 * @buffer: Pointer to buffer to return value in.
 * @buflen: Sizeof @buffer.
 *
 * Returns the buffer on success, an error code otherwise.
 */
static char *tomoyo_get_local_path(struct dentry *dentry, char * const buffer,
				   const int buflen)
{
	struct super_block *sb = dentry->d_sb;
	char *pos = tomoyo_get_dentry_path(dentry, buffer, buflen);
<<<<<<< HEAD
=======

>>>>>>> upstream/android-13
	if (IS_ERR(pos))
		return pos;
	/* Convert from $PID to self if $PID is current thread. */
	if (sb->s_magic == PROC_SUPER_MAGIC && *pos == '/') {
		char *ep;
		const pid_t pid = (pid_t) simple_strtoul(pos + 1, &ep, 10);
<<<<<<< HEAD
		if (*ep == '/' && pid && pid ==
		    task_tgid_nr_ns(current, sb->s_fs_info)) {
=======
		struct pid_namespace *proc_pidns = proc_pid_ns(sb);

		if (*ep == '/' && pid && pid ==
		    task_tgid_nr_ns(current, proc_pidns)) {
>>>>>>> upstream/android-13
			pos = ep - 5;
			if (pos < buffer)
				goto out;
			memmove(pos, "/self", 5);
		}
		goto prepend_filesystem_name;
	}
	/* Use filesystem name for unnamed devices. */
	if (!MAJOR(sb->s_dev))
		goto prepend_filesystem_name;
	{
		struct inode *inode = d_backing_inode(sb->s_root);
<<<<<<< HEAD
=======

>>>>>>> upstream/android-13
		/*
		 * Use filesystem name if filesystem does not support rename()
		 * operation.
		 */
		if (!inode->i_op->rename)
			goto prepend_filesystem_name;
	}
	/* Prepend device name. */
	{
		char name[64];
		int name_len;
		const dev_t dev = sb->s_dev;
<<<<<<< HEAD
=======

>>>>>>> upstream/android-13
		name[sizeof(name) - 1] = '\0';
		snprintf(name, sizeof(name) - 1, "dev(%u,%u):", MAJOR(dev),
			 MINOR(dev));
		name_len = strlen(name);
		pos -= name_len;
		if (pos < buffer)
			goto out;
		memmove(pos, name, name_len);
		return pos;
	}
	/* Prepend filesystem name. */
prepend_filesystem_name:
	{
		const char *name = sb->s_type->name;
		const int name_len = strlen(name);
<<<<<<< HEAD
=======

>>>>>>> upstream/android-13
		pos -= name_len + 1;
		if (pos < buffer)
			goto out;
		memmove(pos, name, name_len);
		pos[name_len] = ':';
	}
	return pos;
out:
	return ERR_PTR(-ENOMEM);
}

/**
<<<<<<< HEAD
 * tomoyo_get_socket_name - Get the name of a socket.
 *
 * @path:   Pointer to "struct path".
 * @buffer: Pointer to buffer to return value in.
 * @buflen: Sizeof @buffer.
 *
 * Returns the buffer.
 */
static char *tomoyo_get_socket_name(const struct path *path, char * const buffer,
				    const int buflen)
{
	struct inode *inode = d_backing_inode(path->dentry);
	struct socket *sock = inode ? SOCKET_I(inode) : NULL;
	struct sock *sk = sock ? sock->sk : NULL;
	if (sk) {
		snprintf(buffer, buflen, "socket:[family=%u:type=%u:"
			 "protocol=%u]", sk->sk_family, sk->sk_type,
			 sk->sk_protocol);
	} else {
		snprintf(buffer, buflen, "socket:[unknown]");
	}
	return buffer;
}

/**
=======
>>>>>>> upstream/android-13
 * tomoyo_realpath_from_path - Returns realpath(3) of the given pathname but ignores chroot'ed root.
 *
 * @path: Pointer to "struct path".
 *
 * Returns the realpath of the given @path on success, NULL otherwise.
 *
 * If dentry is a directory, trailing '/' is appended.
 * Characters out of 0x20 < c < 0x7F range are converted to
 * \ooo style octal string.
 * Character \ is converted to \\ string.
 *
 * These functions use kzalloc(), so the caller must call kfree()
 * if these functions didn't return NULL.
 */
char *tomoyo_realpath_from_path(const struct path *path)
{
	char *buf = NULL;
	char *name = NULL;
	unsigned int buf_len = PAGE_SIZE / 2;
	struct dentry *dentry = path->dentry;
	struct super_block *sb;
<<<<<<< HEAD
=======

>>>>>>> upstream/android-13
	if (!dentry)
		return NULL;
	sb = dentry->d_sb;
	while (1) {
		char *pos;
		struct inode *inode;
<<<<<<< HEAD
=======

>>>>>>> upstream/android-13
		buf_len <<= 1;
		kfree(buf);
		buf = kmalloc(buf_len, GFP_NOFS);
		if (!buf)
			break;
		/* To make sure that pos is '\0' terminated. */
		buf[buf_len - 1] = '\0';
<<<<<<< HEAD
		/* Get better name for socket. */
		if (sb->s_magic == SOCKFS_MAGIC) {
			pos = tomoyo_get_socket_name(path, buf, buf_len - 1);
			goto encode;
		}
		/* For "pipe:[\$]". */
=======
		/* For "pipe:[\$]" and "socket:[\$]". */
>>>>>>> upstream/android-13
		if (dentry->d_op && dentry->d_op->d_dname) {
			pos = dentry->d_op->d_dname(dentry, buf, buf_len - 1);
			goto encode;
		}
		inode = d_backing_inode(sb->s_root);
		/*
		 * Get local name for filesystems without rename() operation
		 * or dentry without vfsmount.
		 */
		if (!path->mnt ||
<<<<<<< HEAD
		    (!inode->i_op->rename))
=======
		    (!inode->i_op->rename &&
		     !(sb->s_type->fs_flags & FS_REQUIRES_DEV)))
>>>>>>> upstream/android-13
			pos = tomoyo_get_local_path(path->dentry, buf,
						    buf_len - 1);
		/* Get absolute name for the rest. */
		else {
			pos = tomoyo_get_absolute_path(path, buf, buf_len - 1);
			/*
			 * Fall back to local name if absolute name is not
			 * available.
			 */
			if (pos == ERR_PTR(-EINVAL))
				pos = tomoyo_get_local_path(path->dentry, buf,
							    buf_len - 1);
		}
encode:
		if (IS_ERR(pos))
			continue;
		name = tomoyo_encode(pos);
		break;
	}
	kfree(buf);
	if (!name)
		tomoyo_warn_oom(__func__);
	return name;
}

/**
 * tomoyo_realpath_nofollow - Get realpath of a pathname.
 *
 * @pathname: The pathname to solve.
 *
 * Returns the realpath of @pathname on success, NULL otherwise.
 */
char *tomoyo_realpath_nofollow(const char *pathname)
{
	struct path path;

	if (pathname && kern_path(pathname, 0, &path) == 0) {
		char *buf = tomoyo_realpath_from_path(&path);
<<<<<<< HEAD
=======

>>>>>>> upstream/android-13
		path_put(&path);
		return buf;
	}
	return NULL;
}
