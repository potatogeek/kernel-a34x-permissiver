<<<<<<< HEAD
// SPDX-License-Identifier: GPL-2.0
=======
// SPDX-License-Identifier: LGPL-2.1
>>>>>>> upstream/android-13

#include "trace/beauty/beauty.h"
#include <linux/kernel.h>
#include <uapi/linux/fcntl.h>

#ifndef LOCK_MAND
#define LOCK_MAND	 32
#endif

#ifndef LOCK_READ
#define LOCK_READ	 64
#endif

#ifndef LOCK_WRITE
#define LOCK_WRITE	128
#endif

#ifndef LOCK_RW
#define LOCK_RW		192
#endif

size_t syscall_arg__scnprintf_flock(char *bf, size_t size, struct syscall_arg *arg)
{
<<<<<<< HEAD
=======
	bool show_prefix = arg->show_string_prefix;
	const char *prefix = "LOCK_";
>>>>>>> upstream/android-13
	int printed = 0, op = arg->val;

	if (op == 0)
		return scnprintf(bf, size, "NONE");
#define	P_CMD(cmd) \
	if ((op & LOCK_##cmd) == LOCK_##cmd) { \
<<<<<<< HEAD
		printed += scnprintf(bf + printed, size - printed, "%s%s", printed ? "|" : "", #cmd); \
=======
		printed += scnprintf(bf + printed, size - printed, "%s%s%s", printed ? "|" : "", show_prefix ? prefix : "", #cmd); \
>>>>>>> upstream/android-13
		op &= ~LOCK_##cmd; \
	}

	P_CMD(SH);
	P_CMD(EX);
	P_CMD(NB);
	P_CMD(UN);
	P_CMD(MAND);
	P_CMD(RW);
	P_CMD(READ);
	P_CMD(WRITE);
#undef P_OP

	if (op)
		printed += scnprintf(bf + printed, size - printed, "%s%#x", printed ? "|" : "", op);

	return printed;
}
