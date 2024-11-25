<<<<<<< HEAD
=======
// SPDX-License-Identifier: LGPL-2.1
>>>>>>> upstream/android-13
/*
 * trace/beauty/pkey_alloc.c
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
#include <linux/log2.h>

<<<<<<< HEAD
static size_t pkey_alloc__scnprintf_access_rights(int access_rights, char *bf, size_t size)
{
	int i, printed = 0;

#include "trace/beauty/generated/pkey_alloc_access_rights_array.c"
	static DEFINE_STRARRAY(pkey_alloc_access_rights);

	if (access_rights == 0) {
		const char *s = strarray__pkey_alloc_access_rights.entries[0];
		if (s)
			return scnprintf(bf, size, "%s", s);
		return scnprintf(bf, size, "%d", 0);
	}

	for (i = 1; i < strarray__pkey_alloc_access_rights.nr_entries; ++i) {
		int bit = 1 << (i - 1);

		if (!(access_rights & bit))
=======
size_t strarray__scnprintf_flags(struct strarray *sa, char *bf, size_t size, bool show_prefix, unsigned long flags)
{
	int i, printed = 0;

	if (flags == 0) {
		const char *s = sa->entries[0];
		if (s)
			return scnprintf(bf, size, "%s%s", show_prefix ? sa->prefix : "", s);
		return scnprintf(bf, size, "%d", 0);
	}

	for (i = 1; i < sa->nr_entries; ++i) {
		unsigned long bit = 1UL << (i - 1);

		if (!(flags & bit))
>>>>>>> upstream/android-13
			continue;

		if (printed != 0)
			printed += scnprintf(bf + printed, size - printed, "|");

<<<<<<< HEAD
		if (strarray__pkey_alloc_access_rights.entries[i] != NULL)
			printed += scnprintf(bf + printed, size - printed, "%s", strarray__pkey_alloc_access_rights.entries[i]);
=======
		if (sa->entries[i] != NULL)
			printed += scnprintf(bf + printed, size - printed, "%s%s", show_prefix ? sa->prefix : "", sa->entries[i]);
>>>>>>> upstream/android-13
		else
			printed += scnprintf(bf + printed, size - printed, "0x%#", bit);
	}

	return printed;
}

<<<<<<< HEAD
=======
static size_t pkey_alloc__scnprintf_access_rights(int access_rights, char *bf, size_t size, bool show_prefix)
{
#include "trace/beauty/generated/pkey_alloc_access_rights_array.c"
	static DEFINE_STRARRAY(pkey_alloc_access_rights, "PKEY_");

	return strarray__scnprintf_flags(&strarray__pkey_alloc_access_rights, bf, size, show_prefix, access_rights);
}

>>>>>>> upstream/android-13
size_t syscall_arg__scnprintf_pkey_alloc_access_rights(char *bf, size_t size, struct syscall_arg *arg)
{
	unsigned long cmd = arg->val;

<<<<<<< HEAD
	return pkey_alloc__scnprintf_access_rights(cmd, bf, size);
=======
	return pkey_alloc__scnprintf_access_rights(cmd, bf, size, arg->show_string_prefix);
>>>>>>> upstream/android-13
}
