<<<<<<< HEAD
=======
// SPDX-License-Identifier: LGPL-2.1
>>>>>>> upstream/android-13
/*
 * trace/beauty/statx.c
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
#include <sys/types.h>
#include <uapi/linux/fcntl.h>
#include <uapi/linux/stat.h>

size_t syscall_arg__scnprintf_statx_flags(char *bf, size_t size, struct syscall_arg *arg)
{
<<<<<<< HEAD
	int printed = 0, flags = arg->val;

	if (flags == 0)
		return scnprintf(bf, size, "SYNC_AS_STAT");
#define	P_FLAG(n) \
	if (flags & AT_##n) { \
		printed += scnprintf(bf + printed, size - printed, "%s%s", printed ? "|" : "", #n); \
=======
	bool show_prefix = arg->show_string_prefix;
	const char *prefix = "AT_";
	int printed = 0, flags = arg->val;

	if (flags == 0)
		return scnprintf(bf, size, "%s%s", show_prefix ? "AT_STATX_" : "", "SYNC_AS_STAT");
#define	P_FLAG(n) \
	if (flags & AT_##n) { \
		printed += scnprintf(bf + printed, size - printed, "%s%s", printed ? "|" : "", show_prefix ? prefix : "", #n); \
>>>>>>> upstream/android-13
		flags &= ~AT_##n; \
	}

	P_FLAG(SYMLINK_NOFOLLOW);
	P_FLAG(REMOVEDIR);
	P_FLAG(SYMLINK_FOLLOW);
	P_FLAG(NO_AUTOMOUNT);
	P_FLAG(EMPTY_PATH);
	P_FLAG(STATX_FORCE_SYNC);
	P_FLAG(STATX_DONT_SYNC);

#undef P_FLAG

	if (flags)
		printed += scnprintf(bf + printed, size - printed, "%s%#x", printed ? "|" : "", flags);

	return printed;
}

size_t syscall_arg__scnprintf_statx_mask(char *bf, size_t size, struct syscall_arg *arg)
{
<<<<<<< HEAD
=======
	bool show_prefix = arg->show_string_prefix;
	const char *prefix = "STATX_";
>>>>>>> upstream/android-13
	int printed = 0, flags = arg->val;

#define	P_FLAG(n) \
	if (flags & STATX_##n) { \
<<<<<<< HEAD
		printed += scnprintf(bf + printed, size - printed, "%s%s", printed ? "|" : "", #n); \
=======
		printed += scnprintf(bf + printed, size - printed, "%s%s", printed ? "|" : "", show_prefix ? prefix : "", #n); \
>>>>>>> upstream/android-13
		flags &= ~STATX_##n; \
	}

	P_FLAG(TYPE);
	P_FLAG(MODE);
	P_FLAG(NLINK);
	P_FLAG(UID);
	P_FLAG(GID);
	P_FLAG(ATIME);
	P_FLAG(MTIME);
	P_FLAG(CTIME);
	P_FLAG(INO);
	P_FLAG(SIZE);
	P_FLAG(BLOCKS);
	P_FLAG(BTIME);
<<<<<<< HEAD
=======
	P_FLAG(MNT_ID);
>>>>>>> upstream/android-13

#undef P_FLAG

	if (flags)
		printed += scnprintf(bf + printed, size - printed, "%s%#x", printed ? "|" : "", flags);

	return printed;
}
