<<<<<<< HEAD
// SPDX-License-Identifier: GPL-2.0
=======
// SPDX-License-Identifier: LGPL-2.1
>>>>>>> upstream/android-13
#include <linux/futex.h>

#ifndef FUTEX_BITSET_MATCH_ANY
#define FUTEX_BITSET_MATCH_ANY 0xffffffff
#endif

static size_t syscall_arg__scnprintf_futex_val3(char *bf, size_t size, struct syscall_arg *arg)
{
<<<<<<< HEAD
	unsigned int bitset = arg->val;

	if (bitset == FUTEX_BITSET_MATCH_ANY)
		return scnprintf(bf, size, "MATCH_ANY");
=======
	const char *prefix = "FUTEX_BITSET_";
	unsigned int bitset = arg->val;

	if (bitset == FUTEX_BITSET_MATCH_ANY)
		return scnprintf(bf, size, "%s%s", arg->show_string_prefix ? prefix : "", "MATCH_ANY");
>>>>>>> upstream/android-13

	return scnprintf(bf, size, "%#xd", bitset);
}

#define SCA_FUTEX_VAL3  syscall_arg__scnprintf_futex_val3
