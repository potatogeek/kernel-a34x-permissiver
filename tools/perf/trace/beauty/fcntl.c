<<<<<<< HEAD
=======
// SPDX-License-Identifier: LGPL-2.1
>>>>>>> upstream/android-13
/*
 * trace/beauty/fcntl.c
 *
 *  Copyright (C) 2017, Red Hat Inc, Arnaldo Carvalho de Melo <acme@redhat.com>
<<<<<<< HEAD
 *
 * Released under the GPL v2. (and only v2, not any later version)
=======
>>>>>>> upstream/android-13
 */

#include "trace/beauty/beauty.h"
#include <linux/kernel.h>
#include <uapi/linux/fcntl.h>

<<<<<<< HEAD
static size_t fcntl__scnprintf_getfd(unsigned long val, char *bf, size_t size)
{
	return scnprintf(bf, size, "%s", val ? "CLOEXEC" : "0");
=======
static size_t fcntl__scnprintf_getfd(unsigned long val, char *bf, size_t size, bool show_prefix)
{
	return val ? scnprintf(bf, size, "%s", "0") :
		     scnprintf(bf, size, "%s%s", show_prefix ? "FD_" : "", "CLOEXEC");
>>>>>>> upstream/android-13
}

static size_t syscall_arg__scnprintf_fcntl_getfd(char *bf, size_t size, struct syscall_arg *arg)
{
<<<<<<< HEAD
	return fcntl__scnprintf_getfd(arg->val, bf, size);
}

static size_t fcntl__scnprintf_getlease(unsigned long val, char *bf, size_t size)
{
	static const char *fcntl_setlease[] = { "RDLCK", "WRLCK", "UNLCK", };
	static DEFINE_STRARRAY(fcntl_setlease);

	return strarray__scnprintf(&strarray__fcntl_setlease, bf, size, "%x", val);
=======
	return fcntl__scnprintf_getfd(arg->val, bf, size, arg->show_string_prefix);
}

static size_t fcntl__scnprintf_getlease(unsigned long val, char *bf, size_t size, bool show_prefix)
{
	static const char *fcntl_setlease[] = { "RDLCK", "WRLCK", "UNLCK", };
	static DEFINE_STRARRAY(fcntl_setlease, "F_");

	return strarray__scnprintf(&strarray__fcntl_setlease, bf, size, "%x", show_prefix, val);
>>>>>>> upstream/android-13
}

static size_t syscall_arg__scnprintf_fcntl_getlease(char *bf, size_t size, struct syscall_arg *arg)
{
<<<<<<< HEAD
	return fcntl__scnprintf_getlease(arg->val, bf, size);
=======
	return fcntl__scnprintf_getlease(arg->val, bf, size, arg->show_string_prefix);
>>>>>>> upstream/android-13
}

size_t syscall_arg__scnprintf_fcntl_cmd(char *bf, size_t size, struct syscall_arg *arg)
{
	if (arg->val == F_GETFL) {
		syscall_arg__set_ret_scnprintf(arg, syscall_arg__scnprintf_open_flags);
		goto mask_arg;
	}
	if (arg->val == F_GETFD) {
		syscall_arg__set_ret_scnprintf(arg, syscall_arg__scnprintf_fcntl_getfd);
		goto mask_arg;
	}
	if (arg->val == F_DUPFD_CLOEXEC || arg->val == F_DUPFD) {
		syscall_arg__set_ret_scnprintf(arg, syscall_arg__scnprintf_fd);
		goto out;
	}
	if (arg->val == F_GETOWN) {
		syscall_arg__set_ret_scnprintf(arg, syscall_arg__scnprintf_pid);
		goto mask_arg;
	}
	if (arg->val == F_GETLEASE) {
		syscall_arg__set_ret_scnprintf(arg, syscall_arg__scnprintf_fcntl_getlease);
		goto mask_arg;
	}
	/*
	 * Some commands ignore the third fcntl argument, "arg", so mask it
	 */
	if (arg->val == F_GET_SEALS ||
	    arg->val == F_GETSIG) {
mask_arg:
		arg->mask |= (1 << 2);
	}
out:
	return syscall_arg__scnprintf_strarrays(bf, size, arg);
}

size_t syscall_arg__scnprintf_fcntl_arg(char *bf, size_t size, struct syscall_arg *arg)
{
<<<<<<< HEAD
=======
	bool show_prefix = arg->show_string_prefix;
>>>>>>> upstream/android-13
	int cmd = syscall_arg__val(arg, 1);

	if (cmd == F_DUPFD)
		return syscall_arg__scnprintf_fd(bf, size, arg);

	if (cmd == F_SETFD)
<<<<<<< HEAD
		return fcntl__scnprintf_getfd(arg->val, bf, size);

	if (cmd == F_SETFL)
		return open__scnprintf_flags(arg->val, bf, size);
=======
		return fcntl__scnprintf_getfd(arg->val, bf, size, show_prefix);

	if (cmd == F_SETFL)
		return open__scnprintf_flags(arg->val, bf, size, show_prefix);
>>>>>>> upstream/android-13

	if (cmd == F_SETOWN)
		return syscall_arg__scnprintf_pid(bf, size, arg);

	if (cmd == F_SETLEASE)
<<<<<<< HEAD
		return fcntl__scnprintf_getlease(arg->val, bf, size);
=======
		return fcntl__scnprintf_getlease(arg->val, bf, size, show_prefix);
>>>>>>> upstream/android-13
	/*
	 * We still don't grab the contents of pointers on entry or exit,
	 * so just print them as hex numbers
	 */
	if (cmd == F_SETLK || cmd == F_SETLKW || cmd == F_GETLK ||
	    cmd == F_OFD_SETLK || cmd == F_OFD_SETLKW || cmd == F_OFD_GETLK ||
	    cmd == F_GETOWN_EX || cmd == F_SETOWN_EX ||
	    cmd == F_GET_RW_HINT || cmd == F_SET_RW_HINT ||
	    cmd == F_GET_FILE_RW_HINT || cmd == F_SET_FILE_RW_HINT)
		return syscall_arg__scnprintf_hex(bf, size, arg);

	return syscall_arg__scnprintf_long(bf, size, arg);
}
