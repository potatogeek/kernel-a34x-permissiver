// SPDX-License-Identifier: GPL-2.0
#include <linux/file.h>
#include <linux/mount.h>
#include <linux/namei.h>
#include <linux/utime.h>
#include <linux/syscalls.h>
#include <linux/uaccess.h>
#include <linux/compat.h>
#include <asm/unistd.h>

<<<<<<< HEAD
#ifdef __ARCH_WANT_SYS_UTIME

/*
 * sys_utime() can be implemented in user-level using sys_utimes().
 * Is this for backwards compatibility?  If so, why not move it
 * into the appropriate arch directory (for those architectures that
 * need it).
 */

/* If times==NULL, set access and modification to current time,
 * must be owner or have write permission.
 * Else, update from *times, must be owner or super user.
 */
SYSCALL_DEFINE2(utime, char __user *, filename, struct utimbuf __user *, times)
{
	struct timespec64 tv[2];

	if (times) {
		if (get_user(tv[0].tv_sec, &times->actime) ||
		    get_user(tv[1].tv_sec, &times->modtime))
			return -EFAULT;
		tv[0].tv_nsec = 0;
		tv[1].tv_nsec = 0;
	}
	return do_utimes(AT_FDCWD, filename, times ? tv : NULL, 0);
}

#endif

=======
>>>>>>> upstream/android-13
static bool nsec_valid(long nsec)
{
	if (nsec == UTIME_OMIT || nsec == UTIME_NOW)
		return true;

	return nsec >= 0 && nsec <= 999999999;
}

<<<<<<< HEAD
static int utimes_common(const struct path *path, struct timespec64 *times)
=======
int vfs_utimes(const struct path *path, struct timespec64 *times)
>>>>>>> upstream/android-13
{
	int error;
	struct iattr newattrs;
	struct inode *inode = path->dentry->d_inode;
	struct inode *delegated_inode = NULL;

<<<<<<< HEAD
=======
	if (times) {
		if (!nsec_valid(times[0].tv_nsec) ||
		    !nsec_valid(times[1].tv_nsec))
			return -EINVAL;
		if (times[0].tv_nsec == UTIME_NOW &&
		    times[1].tv_nsec == UTIME_NOW)
			times = NULL;
	}

>>>>>>> upstream/android-13
	error = mnt_want_write(path->mnt);
	if (error)
		goto out;

<<<<<<< HEAD
	if (times && times[0].tv_nsec == UTIME_NOW &&
		     times[1].tv_nsec == UTIME_NOW)
		times = NULL;

=======
>>>>>>> upstream/android-13
	newattrs.ia_valid = ATTR_CTIME | ATTR_MTIME | ATTR_ATIME;
	if (times) {
		if (times[0].tv_nsec == UTIME_OMIT)
			newattrs.ia_valid &= ~ATTR_ATIME;
		else if (times[0].tv_nsec != UTIME_NOW) {
<<<<<<< HEAD
			newattrs.ia_atime.tv_sec = times[0].tv_sec;
			newattrs.ia_atime.tv_nsec = times[0].tv_nsec;
=======
			newattrs.ia_atime = times[0];
>>>>>>> upstream/android-13
			newattrs.ia_valid |= ATTR_ATIME_SET;
		}

		if (times[1].tv_nsec == UTIME_OMIT)
			newattrs.ia_valid &= ~ATTR_MTIME;
		else if (times[1].tv_nsec != UTIME_NOW) {
<<<<<<< HEAD
			newattrs.ia_mtime.tv_sec = times[1].tv_sec;
			newattrs.ia_mtime.tv_nsec = times[1].tv_nsec;
=======
			newattrs.ia_mtime = times[1];
>>>>>>> upstream/android-13
			newattrs.ia_valid |= ATTR_MTIME_SET;
		}
		/*
		 * Tell setattr_prepare(), that this is an explicit time
		 * update, even if neither ATTR_ATIME_SET nor ATTR_MTIME_SET
		 * were used.
		 */
		newattrs.ia_valid |= ATTR_TIMES_SET;
	} else {
		newattrs.ia_valid |= ATTR_TOUCH;
	}
retry_deleg:
	inode_lock(inode);
<<<<<<< HEAD
	error = notify_change2(path->mnt, path->dentry, &newattrs, &delegated_inode);
=======
	error = notify_change(mnt_user_ns(path->mnt), path->dentry, &newattrs,
			      &delegated_inode);
>>>>>>> upstream/android-13
	inode_unlock(inode);
	if (delegated_inode) {
		error = break_deleg_wait(&delegated_inode);
		if (!error)
			goto retry_deleg;
	}

	mnt_drop_write(path->mnt);
out:
	return error;
}

<<<<<<< HEAD
=======
static int do_utimes_path(int dfd, const char __user *filename,
		struct timespec64 *times, int flags)
{
	struct path path;
	int lookup_flags = 0, error;

	if (flags & ~(AT_SYMLINK_NOFOLLOW | AT_EMPTY_PATH))
		return -EINVAL;

	if (!(flags & AT_SYMLINK_NOFOLLOW))
		lookup_flags |= LOOKUP_FOLLOW;
	if (flags & AT_EMPTY_PATH)
		lookup_flags |= LOOKUP_EMPTY;

retry:
	error = user_path_at(dfd, filename, lookup_flags, &path);
	if (error)
		return error;

	error = vfs_utimes(&path, times);
	path_put(&path);
	if (retry_estale(error, lookup_flags)) {
		lookup_flags |= LOOKUP_REVAL;
		goto retry;
	}

	return error;
}

static int do_utimes_fd(int fd, struct timespec64 *times, int flags)
{
	struct fd f;
	int error;

	if (flags)
		return -EINVAL;

	f = fdget(fd);
	if (!f.file)
		return -EBADF;
	error = vfs_utimes(&f.file->f_path, times);
	fdput(f);
	return error;
}

>>>>>>> upstream/android-13
/*
 * do_utimes - change times on filename or file descriptor
 * @dfd: open file descriptor, -1 or AT_FDCWD
 * @filename: path name or NULL
 * @times: new times or NULL
 * @flags: zero or more flags (only AT_SYMLINK_NOFOLLOW for the moment)
 *
 * If filename is NULL and dfd refers to an open file, then operate on
 * the file.  Otherwise look up filename, possibly using dfd as a
 * starting point.
 *
 * If times==NULL, set access and modification to current time,
 * must be owner or have write permission.
 * Else, update from *times, must be owner or super user.
 */
long do_utimes(int dfd, const char __user *filename, struct timespec64 *times,
	       int flags)
{
<<<<<<< HEAD
	int error = -EINVAL;

	if (times && (!nsec_valid(times[0].tv_nsec) ||
		      !nsec_valid(times[1].tv_nsec))) {
		goto out;
	}

	if (flags & ~AT_SYMLINK_NOFOLLOW)
		goto out;

	if (filename == NULL && dfd != AT_FDCWD) {
		struct fd f;

		if (flags & AT_SYMLINK_NOFOLLOW)
			goto out;

		f = fdget(dfd);
		error = -EBADF;
		if (!f.file)
			goto out;

		error = utimes_common(&f.file->f_path, times);
		fdput(f);
	} else {
		struct path path;
		int lookup_flags = 0;

		if (!(flags & AT_SYMLINK_NOFOLLOW))
			lookup_flags |= LOOKUP_FOLLOW;
retry:
		error = user_path_at(dfd, filename, lookup_flags, &path);
		if (error)
			goto out;

		error = utimes_common(&path, times);
		path_put(&path);
		if (retry_estale(error, lookup_flags)) {
			lookup_flags |= LOOKUP_REVAL;
			goto retry;
		}
	}

out:
	return error;
}

SYSCALL_DEFINE4(utimensat, int, dfd, const char __user *, filename,
		struct timespec __user *, utimes, int, flags)
=======
	if (filename == NULL && dfd != AT_FDCWD)
		return do_utimes_fd(dfd, times, flags);
	return do_utimes_path(dfd, filename, times, flags);
}

SYSCALL_DEFINE4(utimensat, int, dfd, const char __user *, filename,
		struct __kernel_timespec __user *, utimes, int, flags)
>>>>>>> upstream/android-13
{
	struct timespec64 tstimes[2];

	if (utimes) {
		if ((get_timespec64(&tstimes[0], &utimes[0]) ||
			get_timespec64(&tstimes[1], &utimes[1])))
			return -EFAULT;

		/* Nothing to do, we must not even check the path.  */
		if (tstimes[0].tv_nsec == UTIME_OMIT &&
		    tstimes[1].tv_nsec == UTIME_OMIT)
			return 0;
	}

	return do_utimes(dfd, filename, utimes ? tstimes : NULL, flags);
}

<<<<<<< HEAD
static long do_futimesat(int dfd, const char __user *filename,
			 struct timeval __user *utimes)
{
	struct timeval times[2];
=======
#ifdef __ARCH_WANT_SYS_UTIME
/*
 * futimesat(), utimes() and utime() are older versions of utimensat()
 * that are provided for compatibility with traditional C libraries.
 * On modern architectures, we always use libc wrappers around
 * utimensat() instead.
 */
static long do_futimesat(int dfd, const char __user *filename,
			 struct __kernel_old_timeval __user *utimes)
{
	struct __kernel_old_timeval times[2];
>>>>>>> upstream/android-13
	struct timespec64 tstimes[2];

	if (utimes) {
		if (copy_from_user(&times, utimes, sizeof(times)))
			return -EFAULT;

		/* This test is needed to catch all invalid values.  If we
		   would test only in do_utimes we would miss those invalid
		   values truncated by the multiplication with 1000.  Note
		   that we also catch UTIME_{NOW,OMIT} here which are only
		   valid for utimensat.  */
		if (times[0].tv_usec >= 1000000 || times[0].tv_usec < 0 ||
		    times[1].tv_usec >= 1000000 || times[1].tv_usec < 0)
			return -EINVAL;

		tstimes[0].tv_sec = times[0].tv_sec;
		tstimes[0].tv_nsec = 1000 * times[0].tv_usec;
		tstimes[1].tv_sec = times[1].tv_sec;
		tstimes[1].tv_nsec = 1000 * times[1].tv_usec;
	}

	return do_utimes(dfd, filename, utimes ? tstimes : NULL, 0);
}


SYSCALL_DEFINE3(futimesat, int, dfd, const char __user *, filename,
<<<<<<< HEAD
		struct timeval __user *, utimes)
=======
		struct __kernel_old_timeval __user *, utimes)
>>>>>>> upstream/android-13
{
	return do_futimesat(dfd, filename, utimes);
}

SYSCALL_DEFINE2(utimes, char __user *, filename,
<<<<<<< HEAD
		struct timeval __user *, utimes)
=======
		struct __kernel_old_timeval __user *, utimes)
>>>>>>> upstream/android-13
{
	return do_futimesat(AT_FDCWD, filename, utimes);
}

<<<<<<< HEAD
#ifdef CONFIG_COMPAT
=======
SYSCALL_DEFINE2(utime, char __user *, filename, struct utimbuf __user *, times)
{
	struct timespec64 tv[2];

	if (times) {
		if (get_user(tv[0].tv_sec, &times->actime) ||
		    get_user(tv[1].tv_sec, &times->modtime))
			return -EFAULT;
		tv[0].tv_nsec = 0;
		tv[1].tv_nsec = 0;
	}
	return do_utimes(AT_FDCWD, filename, times ? tv : NULL, 0);
}
#endif

#ifdef CONFIG_COMPAT_32BIT_TIME
>>>>>>> upstream/android-13
/*
 * Not all architectures have sys_utime, so implement this in terms
 * of sys_utimes.
 */
<<<<<<< HEAD
COMPAT_SYSCALL_DEFINE2(utime, const char __user *, filename,
		       struct compat_utimbuf __user *, t)
=======
#ifdef __ARCH_WANT_SYS_UTIME32
SYSCALL_DEFINE2(utime32, const char __user *, filename,
		struct old_utimbuf32 __user *, t)
>>>>>>> upstream/android-13
{
	struct timespec64 tv[2];

	if (t) {
		if (get_user(tv[0].tv_sec, &t->actime) ||
		    get_user(tv[1].tv_sec, &t->modtime))
			return -EFAULT;
		tv[0].tv_nsec = 0;
		tv[1].tv_nsec = 0;
	}
	return do_utimes(AT_FDCWD, filename, t ? tv : NULL, 0);
}
<<<<<<< HEAD

COMPAT_SYSCALL_DEFINE4(utimensat, unsigned int, dfd, const char __user *, filename, struct compat_timespec __user *, t, int, flags)
=======
#endif

SYSCALL_DEFINE4(utimensat_time32, unsigned int, dfd, const char __user *, filename, struct old_timespec32 __user *, t, int, flags)
>>>>>>> upstream/android-13
{
	struct timespec64 tv[2];

	if  (t) {
<<<<<<< HEAD
		if (compat_get_timespec64(&tv[0], &t[0]) ||
		    compat_get_timespec64(&tv[1], &t[1]))
=======
		if (get_old_timespec32(&tv[0], &t[0]) ||
		    get_old_timespec32(&tv[1], &t[1]))
>>>>>>> upstream/android-13
			return -EFAULT;

		if (tv[0].tv_nsec == UTIME_OMIT && tv[1].tv_nsec == UTIME_OMIT)
			return 0;
	}
	return do_utimes(dfd, filename, t ? tv : NULL, flags);
}

<<<<<<< HEAD
static long do_compat_futimesat(unsigned int dfd, const char __user *filename,
				struct compat_timeval __user *t)
=======
#ifdef __ARCH_WANT_SYS_UTIME32
static long do_compat_futimesat(unsigned int dfd, const char __user *filename,
				struct old_timeval32 __user *t)
>>>>>>> upstream/android-13
{
	struct timespec64 tv[2];

	if (t) {
		if (get_user(tv[0].tv_sec, &t[0].tv_sec) ||
		    get_user(tv[0].tv_nsec, &t[0].tv_usec) ||
		    get_user(tv[1].tv_sec, &t[1].tv_sec) ||
		    get_user(tv[1].tv_nsec, &t[1].tv_usec))
			return -EFAULT;
		if (tv[0].tv_nsec >= 1000000 || tv[0].tv_nsec < 0 ||
		    tv[1].tv_nsec >= 1000000 || tv[1].tv_nsec < 0)
			return -EINVAL;
		tv[0].tv_nsec *= 1000;
		tv[1].tv_nsec *= 1000;
	}
	return do_utimes(dfd, filename, t ? tv : NULL, 0);
}

<<<<<<< HEAD
COMPAT_SYSCALL_DEFINE3(futimesat, unsigned int, dfd,
		       const char __user *, filename,
		       struct compat_timeval __user *, t)
=======
SYSCALL_DEFINE3(futimesat_time32, unsigned int, dfd,
		       const char __user *, filename,
		       struct old_timeval32 __user *, t)
>>>>>>> upstream/android-13
{
	return do_compat_futimesat(dfd, filename, t);
}

<<<<<<< HEAD
COMPAT_SYSCALL_DEFINE2(utimes, const char __user *, filename, struct compat_timeval __user *, t)
=======
SYSCALL_DEFINE2(utimes_time32, const char __user *, filename, struct old_timeval32 __user *, t)
>>>>>>> upstream/android-13
{
	return do_compat_futimesat(AT_FDCWD, filename, t);
}
#endif
<<<<<<< HEAD
=======
#endif
>>>>>>> upstream/android-13
