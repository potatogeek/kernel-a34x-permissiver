/* SPDX-License-Identifier: GPL-2.0 */
#include <linux/syscalls.h>
#include <linux/export.h>
#include <linux/uaccess.h>
#include <linux/fs_struct.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/prefetch.h>
#include "mount.h"

<<<<<<< HEAD
static int prepend(char **buffer, int *buflen, const char *str, int namelen)
{
	*buflen -= namelen;
	if (*buflen < 0)
		return -ENAMETOOLONG;
	*buffer -= namelen;
	memcpy(*buffer, str, namelen);
	return 0;
=======
struct prepend_buffer {
	char *buf;
	int len;
};
#define DECLARE_BUFFER(__name, __buf, __len) \
	struct prepend_buffer __name = {.buf = __buf + __len, .len = __len}

static char *extract_string(struct prepend_buffer *p)
{
	if (likely(p->len >= 0))
		return p->buf;
	return ERR_PTR(-ENAMETOOLONG);
}

static bool prepend_char(struct prepend_buffer *p, unsigned char c)
{
	if (likely(p->len > 0)) {
		p->len--;
		*--p->buf = c;
		return true;
	}
	p->len = -1;
	return false;
}

/*
 * The source of the prepend data can be an optimistoc load
 * of a dentry name and length. And because we don't hold any
 * locks, the length and the pointer to the name may not be
 * in sync if a concurrent rename happens, and the kernel
 * copy might fault as a result.
 *
 * The end result will correct itself when we check the
 * rename sequence count, but we need to be able to handle
 * the fault gracefully.
 */
static bool prepend_copy(void *dst, const void *src, int len)
{
	if (unlikely(copy_from_kernel_nofault(dst, src, len))) {
		memset(dst, 'x', len);
		return false;
	}
	return true;
}

static bool prepend(struct prepend_buffer *p, const char *str, int namelen)
{
	// Already overflowed?
	if (p->len < 0)
		return false;

	// Will overflow?
	if (p->len < namelen) {
		// Fill as much as possible from the end of the name
		str += namelen - p->len;
		p->buf -= p->len;
		prepend_copy(p->buf, str, p->len);
		p->len = -1;
		return false;
	}

	// Fits fully
	p->len -= namelen;
	p->buf -= namelen;
	return prepend_copy(p->buf, str, namelen);
>>>>>>> upstream/android-13
}

/**
 * prepend_name - prepend a pathname in front of current buffer pointer
 * @buffer: buffer pointer
 * @buflen: allocated length of the buffer
 * @name:   name string and length qstr structure
 *
 * With RCU path tracing, it may race with d_move(). Use READ_ONCE() to
 * make sure that either the old or the new name pointer and length are
 * fetched. However, there may be mismatch between length and pointer.
<<<<<<< HEAD
 * The length cannot be trusted, we need to copy it byte-by-byte until
 * the length is reached or a null byte is found. It also prepends "/" at
=======
 * But since the length cannot be trusted, we need to copy the name very
 * carefully when doing the prepend_copy(). It also prepends "/" at
>>>>>>> upstream/android-13
 * the beginning of the name. The sequence number check at the caller will
 * retry it again when a d_move() does happen. So any garbage in the buffer
 * due to mismatched pointer and length will be discarded.
 *
<<<<<<< HEAD
 * Load acquire is needed to make sure that we see that terminating NUL.
 */
static int prepend_name(char **buffer, int *buflen, const struct qstr *name)
{
	const char *dname = smp_load_acquire(&name->name); /* ^^^ */
	u32 dlen = READ_ONCE(name->len);
	char *p;

	*buflen -= dlen + 1;
	if (*buflen < 0)
		return -ENAMETOOLONG;
	p = *buffer -= dlen + 1;
	*p++ = '/';
	while (dlen--) {
		char c = *dname++;
		if (!c)
			break;
		*p++ = c;
=======
 * Load acquire is needed to make sure that we see the new name data even
 * if we might get the length wrong.
 */
static bool prepend_name(struct prepend_buffer *p, const struct qstr *name)
{
	const char *dname = smp_load_acquire(&name->name); /* ^^^ */
	u32 dlen = READ_ONCE(name->len);

	return prepend(p, dname, dlen) && prepend_char(p, '/');
}

static int __prepend_path(const struct dentry *dentry, const struct mount *mnt,
			  const struct path *root, struct prepend_buffer *p)
{
	while (dentry != root->dentry || &mnt->mnt != root->mnt) {
		const struct dentry *parent = READ_ONCE(dentry->d_parent);

		if (dentry == mnt->mnt.mnt_root) {
			struct mount *m = READ_ONCE(mnt->mnt_parent);
			struct mnt_namespace *mnt_ns;

			if (likely(mnt != m)) {
				dentry = READ_ONCE(mnt->mnt_mountpoint);
				mnt = m;
				continue;
			}
			/* Global root */
			mnt_ns = READ_ONCE(mnt->mnt_ns);
			/* open-coded is_mounted() to use local mnt_ns */
			if (!IS_ERR_OR_NULL(mnt_ns) && !is_anon_ns(mnt_ns))
				return 1;	// absolute root
			else
				return 2;	// detached or not attached yet
		}

		if (unlikely(dentry == parent))
			/* Escaped? */
			return 3;

		prefetch(parent);
		if (!prepend_name(p, &dentry->d_name))
			break;
		dentry = parent;
>>>>>>> upstream/android-13
	}
	return 0;
}

/**
 * prepend_path - Prepend path string to a buffer
 * @path: the dentry/vfsmount to report
 * @root: root vfsmnt/dentry
 * @buffer: pointer to the end of the buffer
 * @buflen: pointer to buffer length
 *
 * The function will first try to write out the pathname without taking any
 * lock other than the RCU read lock to make sure that dentries won't go away.
 * It only checks the sequence number of the global rename_lock as any change
 * in the dentry's d_seq will be preceded by changes in the rename_lock
 * sequence number. If the sequence number had been changed, it will restart
 * the whole pathname back-tracing sequence again by taking the rename_lock.
 * In this case, there is no need to take the RCU read lock as the recursive
 * parent pointer references will keep the dentry chain alive as long as no
 * rename operation is performed.
 */
static int prepend_path(const struct path *path,
			const struct path *root,
<<<<<<< HEAD
			char **buffer, int *buflen)
{
	struct dentry *dentry;
	struct vfsmount *vfsmnt;
	struct mount *mnt;
	int error = 0;
	unsigned seq, m_seq = 0;
	char *bptr;
	int blen;
=======
			struct prepend_buffer *p)
{
	unsigned seq, m_seq = 0;
	struct prepend_buffer b;
	int error;
>>>>>>> upstream/android-13

	rcu_read_lock();
restart_mnt:
	read_seqbegin_or_lock(&mount_lock, &m_seq);
	seq = 0;
	rcu_read_lock();
restart:
<<<<<<< HEAD
	bptr = *buffer;
	blen = *buflen;
	error = 0;
	dentry = path->dentry;
	vfsmnt = path->mnt;
	mnt = real_mount(vfsmnt);
	read_seqbegin_or_lock(&rename_lock, &seq);
	while (dentry != root->dentry || vfsmnt != root->mnt) {
		struct dentry * parent;

		if (dentry == vfsmnt->mnt_root || IS_ROOT(dentry)) {
			struct mount *parent = READ_ONCE(mnt->mnt_parent);
			/* Escaped? */
			if (dentry != vfsmnt->mnt_root) {
				bptr = *buffer;
				blen = *buflen;
				error = 3;
				break;
			}
			/* Global root? */
			if (mnt != parent) {
				dentry = READ_ONCE(mnt->mnt_mountpoint);
				mnt = parent;
#ifdef CONFIG_KDP_NS
				vfsmnt = mnt->mnt;
#else
				vfsmnt = &mnt->mnt;
#endif
				continue;
			}
			if (!error)
				error = is_mounted(vfsmnt) ? 1 : 2;
			break;
		}
		parent = dentry->d_parent;
		prefetch(parent);
		error = prepend_name(&bptr, &blen, &dentry->d_name);
		if (error)
			break;

		dentry = parent;
	}
=======
	b = *p;
	read_seqbegin_or_lock(&rename_lock, &seq);
	error = __prepend_path(path->dentry, real_mount(path->mnt), root, &b);
>>>>>>> upstream/android-13
	if (!(seq & 1))
		rcu_read_unlock();
	if (need_seqretry(&rename_lock, seq)) {
		seq = 1;
		goto restart;
	}
	done_seqretry(&rename_lock, seq);

	if (!(m_seq & 1))
		rcu_read_unlock();
	if (need_seqretry(&mount_lock, m_seq)) {
		m_seq = 1;
		goto restart_mnt;
	}
	done_seqretry(&mount_lock, m_seq);

<<<<<<< HEAD
	if (error >= 0 && bptr == *buffer) {
		if (--blen < 0)
			error = -ENAMETOOLONG;
		else
			*--bptr = '/';
	}
	*buffer = bptr;
	*buflen = blen;
=======
	if (unlikely(error == 3))
		b = *p;

	if (b.len == p->len)
		prepend_char(&b, '/');

	*p = b;
>>>>>>> upstream/android-13
	return error;
}

/**
 * __d_path - return the path of a dentry
 * @path: the dentry/vfsmount to report
 * @root: root vfsmnt/dentry
 * @buf: buffer to return value in
 * @buflen: buffer length
 *
 * Convert a dentry into an ASCII path name.
 *
 * Returns a pointer into the buffer or an error code if the
 * path was too long.
 *
 * "buflen" should be positive.
 *
 * If the path is not reachable from the supplied root, return %NULL.
 */
char *__d_path(const struct path *path,
	       const struct path *root,
	       char *buf, int buflen)
{
<<<<<<< HEAD
	char *res = buf + buflen;
	int error;

	prepend(&res, &buflen, "\0", 1);
	error = prepend_path(path, root, &res, &buflen);

	if (error < 0)
		return ERR_PTR(error);
	if (error > 0)
		return NULL;
	return res;
=======
	DECLARE_BUFFER(b, buf, buflen);

	prepend_char(&b, 0);
	if (unlikely(prepend_path(path, root, &b) > 0))
		return NULL;
	return extract_string(&b);
>>>>>>> upstream/android-13
}

char *d_absolute_path(const struct path *path,
	       char *buf, int buflen)
{
	struct path root = {};
<<<<<<< HEAD
	char *res = buf + buflen;
	int error;

	prepend(&res, &buflen, "\0", 1);
	error = prepend_path(path, &root, &res, &buflen);

	if (error > 1)
		error = -EINVAL;
	if (error < 0)
		return ERR_PTR(error);
	return res;
}

/*
 * same as __d_path but appends "(deleted)" for unlinked files.
 */
static int path_with_deleted(const struct path *path,
			     const struct path *root,
			     char **buf, int *buflen)
{
	prepend(buf, buflen, "\0", 1);
	if (d_unlinked(path->dentry)) {
		int error = prepend(buf, buflen, " (deleted)", 10);
		if (error)
			return error;
	}

	return prepend_path(path, root, buf, buflen);
}

static int prepend_unreachable(char **buffer, int *buflen)
{
	return prepend(buffer, buflen, "(unreachable)", 13);
=======
	DECLARE_BUFFER(b, buf, buflen);

	prepend_char(&b, 0);
	if (unlikely(prepend_path(path, &root, &b) > 1))
		return ERR_PTR(-EINVAL);
	return extract_string(&b);
>>>>>>> upstream/android-13
}

static void get_fs_root_rcu(struct fs_struct *fs, struct path *root)
{
	unsigned seq;

	do {
		seq = read_seqcount_begin(&fs->seq);
		*root = fs->root;
	} while (read_seqcount_retry(&fs->seq, seq));
}

/**
 * d_path - return the path of a dentry
 * @path: path to report
 * @buf: buffer to return value in
 * @buflen: buffer length
 *
 * Convert a dentry into an ASCII path name. If the entry has been deleted
 * the string " (deleted)" is appended. Note that this is ambiguous.
 *
 * Returns a pointer into the buffer or an error code if the path was
 * too long. Note: Callers should use the returned pointer, not the passed
 * in buffer, to use the name! The implementation often starts at an offset
 * into the buffer, and may leave 0 bytes at the start.
 *
 * "buflen" should be positive.
 */
char *d_path(const struct path *path, char *buf, int buflen)
{
<<<<<<< HEAD
	char *res = buf + buflen;
	struct path root;
	int error;
=======
	DECLARE_BUFFER(b, buf, buflen);
	struct path root;
>>>>>>> upstream/android-13

	/*
	 * We have various synthetic filesystems that never get mounted.  On
	 * these filesystems dentries are never used for lookup purposes, and
	 * thus don't need to be hashed.  They also don't need a name until a
	 * user wants to identify the object in /proc/pid/fd/.  The little hack
	 * below allows us to generate a name for these objects on demand:
	 *
	 * Some pseudo inodes are mountable.  When they are mounted
	 * path->dentry == path->mnt->mnt_root.  In that case don't call d_dname
	 * and instead have d_path return the mounted path.
	 */
	if (path->dentry->d_op && path->dentry->d_op->d_dname &&
	    (!IS_ROOT(path->dentry) || path->dentry != path->mnt->mnt_root))
		return path->dentry->d_op->d_dname(path->dentry, buf, buflen);

	rcu_read_lock();
	get_fs_root_rcu(current->fs, &root);
<<<<<<< HEAD
	error = path_with_deleted(path, &root, &res, &buflen);
	rcu_read_unlock();

	if (error < 0)
		res = ERR_PTR(error);
	return res;
=======
	if (unlikely(d_unlinked(path->dentry)))
		prepend(&b, " (deleted)", 11);
	else
		prepend_char(&b, 0);
	prepend_path(path, &root, &b);
	rcu_read_unlock();

	return extract_string(&b);
>>>>>>> upstream/android-13
}
EXPORT_SYMBOL(d_path);

/*
 * Helper function for dentry_operations.d_dname() members
 */
char *dynamic_dname(struct dentry *dentry, char *buffer, int buflen,
			const char *fmt, ...)
{
	va_list args;
	char temp[64];
	int sz;

	va_start(args, fmt);
	sz = vsnprintf(temp, sizeof(temp), fmt, args) + 1;
	va_end(args);

	if (sz > sizeof(temp) || sz > buflen)
		return ERR_PTR(-ENAMETOOLONG);

	buffer += buflen - sz;
	return memcpy(buffer, temp, sz);
}

char *simple_dname(struct dentry *dentry, char *buffer, int buflen)
{
<<<<<<< HEAD
	char *end = buffer + buflen;
	/* these dentries are never renamed, so d_lock is not needed */
	if (prepend(&end, &buflen, " (deleted)", 11) ||
	    prepend(&end, &buflen, dentry->d_name.name, dentry->d_name.len) ||
	    prepend(&end, &buflen, "/", 1))  
		end = ERR_PTR(-ENAMETOOLONG);
	return end;
}
EXPORT_SYMBOL(simple_dname);
=======
	DECLARE_BUFFER(b, buffer, buflen);
	/* these dentries are never renamed, so d_lock is not needed */
	prepend(&b, " (deleted)", 11);
	prepend(&b, dentry->d_name.name, dentry->d_name.len);
	prepend_char(&b, '/');
	return extract_string(&b);
}
>>>>>>> upstream/android-13

/*
 * Write full pathname from the root of the filesystem into the buffer.
 */
<<<<<<< HEAD
static char *__dentry_path(struct dentry *d, char *buf, int buflen)
{
	struct dentry *dentry;
	char *end, *retval;
	int len, seq = 0;
	int error = 0;

	if (buflen < 2)
		goto Elong;
=======
static char *__dentry_path(const struct dentry *d, struct prepend_buffer *p)
{
	const struct dentry *dentry;
	struct prepend_buffer b;
	int seq = 0;
>>>>>>> upstream/android-13

	rcu_read_lock();
restart:
	dentry = d;
<<<<<<< HEAD
	end = buf + buflen;
	len = buflen;
	prepend(&end, &len, "\0", 1);
	/* Get '/' right */
	retval = end-1;
	*retval = '/';
	read_seqbegin_or_lock(&rename_lock, &seq);
	while (!IS_ROOT(dentry)) {
		struct dentry *parent = dentry->d_parent;

		prefetch(parent);
		error = prepend_name(&end, &len, &dentry->d_name);
		if (error)
			break;

		retval = end;
=======
	b = *p;
	read_seqbegin_or_lock(&rename_lock, &seq);
	while (!IS_ROOT(dentry)) {
		const struct dentry *parent = dentry->d_parent;

		prefetch(parent);
		if (!prepend_name(&b, &dentry->d_name))
			break;
>>>>>>> upstream/android-13
		dentry = parent;
	}
	if (!(seq & 1))
		rcu_read_unlock();
	if (need_seqretry(&rename_lock, seq)) {
		seq = 1;
		goto restart;
	}
	done_seqretry(&rename_lock, seq);
<<<<<<< HEAD
	if (error)
		goto Elong;
	return retval;
Elong:
	return ERR_PTR(-ENAMETOOLONG);
}

char *dentry_path_raw(struct dentry *dentry, char *buf, int buflen)
{
	return __dentry_path(dentry, buf, buflen);
}
EXPORT_SYMBOL(dentry_path_raw);

char *dentry_path(struct dentry *dentry, char *buf, int buflen)
{
	char *p = NULL;
	char *retval;

	if (d_unlinked(dentry)) {
		p = buf + buflen;
		if (prepend(&p, &buflen, "//deleted", 10) != 0)
			goto Elong;
		buflen++;
	}
	retval = __dentry_path(dentry, buf, buflen);
	if (!IS_ERR(retval) && p)
		*p = '/';	/* restore '/' overriden with '\0' */
	return retval;
Elong:
	return ERR_PTR(-ENAMETOOLONG);
=======
	if (b.len == p->len)
		prepend_char(&b, '/');
	return extract_string(&b);
}

char *dentry_path_raw(const struct dentry *dentry, char *buf, int buflen)
{
	DECLARE_BUFFER(b, buf, buflen);

	prepend_char(&b, 0);
	return __dentry_path(dentry, &b);
}
EXPORT_SYMBOL(dentry_path_raw);

char *dentry_path(const struct dentry *dentry, char *buf, int buflen)
{
	DECLARE_BUFFER(b, buf, buflen);

	if (unlikely(d_unlinked(dentry)))
		prepend(&b, "//deleted", 10);
	else
		prepend_char(&b, 0);
	return __dentry_path(dentry, &b);
>>>>>>> upstream/android-13
}

static void get_fs_root_and_pwd_rcu(struct fs_struct *fs, struct path *root,
				    struct path *pwd)
{
	unsigned seq;

	do {
		seq = read_seqcount_begin(&fs->seq);
		*root = fs->root;
		*pwd = fs->pwd;
	} while (read_seqcount_retry(&fs->seq, seq));
}

/*
 * NOTE! The user-level library version returns a
 * character pointer. The kernel system call just
 * returns the length of the buffer filled (which
 * includes the ending '\0' character), or a negative
 * error value. So libc would do something like
 *
 *	char *getcwd(char * buf, size_t size)
 *	{
 *		int retval;
 *
 *		retval = sys_getcwd(buf, size);
 *		if (retval >= 0)
 *			return buf;
 *		errno = -retval;
 *		return NULL;
 *	}
 */
SYSCALL_DEFINE2(getcwd, char __user *, buf, unsigned long, size)
{
	int error;
	struct path pwd, root;
	char *page = __getname();

	if (!page)
		return -ENOMEM;

	rcu_read_lock();
	get_fs_root_and_pwd_rcu(current->fs, &root, &pwd);

<<<<<<< HEAD
	error = -ENOENT;
	if (!d_unlinked(pwd.dentry)) {
		unsigned long len;
		char *cwd = page + PATH_MAX;
		int buflen = PATH_MAX;

		prepend(&cwd, &buflen, "\0", 1);
		error = prepend_path(&pwd, &root, &cwd, &buflen);
		rcu_read_unlock();

		if (error < 0)
			goto out;

		/* Unreachable from current root */
		if (error > 0) {
			error = prepend_unreachable(&cwd, &buflen);
			if (error)
				goto out;
		}

		error = -ERANGE;
		len = PATH_MAX + page - cwd;
		if (len <= size) {
			error = len;
			if (copy_to_user(buf, cwd, len))
				error = -EFAULT;
		}
	} else {
		rcu_read_unlock();
	}

out:
=======
	if (unlikely(d_unlinked(pwd.dentry))) {
		rcu_read_unlock();
		error = -ENOENT;
	} else {
		unsigned len;
		DECLARE_BUFFER(b, page, PATH_MAX);

		prepend_char(&b, 0);
		if (unlikely(prepend_path(&pwd, &root, &b) > 0))
			prepend(&b, "(unreachable)", 13);
		rcu_read_unlock();

		len = PATH_MAX - b.len;
		if (unlikely(len > PATH_MAX))
			error = -ENAMETOOLONG;
		else if (unlikely(len > size))
			error = -ERANGE;
		else if (copy_to_user(buf, b.buf, len))
			error = -EFAULT;
		else
			error = len;
	}
>>>>>>> upstream/android-13
	__putname(page);
	return error;
}
