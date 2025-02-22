<<<<<<< HEAD
=======
// SPDX-License-Identifier: LGPL-2.1
>>>>>>> upstream/android-13
/*
 * trace/beauty/cone.c
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
#include <uapi/linux/sched.h>

<<<<<<< HEAD
static size_t clone__scnprintf_flags(unsigned long flags, char *bf, size_t size)
{
=======
static size_t clone__scnprintf_flags(unsigned long flags, char *bf, size_t size, bool show_prefix)
{
	const char *prefix = "CLONE_";
>>>>>>> upstream/android-13
	int printed = 0;

#define	P_FLAG(n) \
	if (flags & CLONE_##n) { \
<<<<<<< HEAD
		printed += scnprintf(bf + printed, size - printed, "%s%s", printed ? "|" : "", #n); \
=======
		printed += scnprintf(bf + printed, size - printed, "%s%s%s", printed ? "|" : "", show_prefix ? prefix : "", #n); \
>>>>>>> upstream/android-13
		flags &= ~CLONE_##n; \
	}

	P_FLAG(VM);
	P_FLAG(FS);
	P_FLAG(FILES);
	P_FLAG(SIGHAND);
<<<<<<< HEAD
=======
	P_FLAG(PIDFD);
>>>>>>> upstream/android-13
	P_FLAG(PTRACE);
	P_FLAG(VFORK);
	P_FLAG(PARENT);
	P_FLAG(THREAD);
	P_FLAG(NEWNS);
	P_FLAG(SYSVSEM);
	P_FLAG(SETTLS);
	P_FLAG(PARENT_SETTID);
	P_FLAG(CHILD_CLEARTID);
	P_FLAG(DETACHED);
	P_FLAG(UNTRACED);
	P_FLAG(CHILD_SETTID);
	P_FLAG(NEWCGROUP);
	P_FLAG(NEWUTS);
	P_FLAG(NEWIPC);
	P_FLAG(NEWUSER);
	P_FLAG(NEWPID);
	P_FLAG(NEWNET);
	P_FLAG(IO);
<<<<<<< HEAD
=======
	P_FLAG(CLEAR_SIGHAND);
	P_FLAG(INTO_CGROUP);
>>>>>>> upstream/android-13
#undef P_FLAG

	if (flags)
		printed += scnprintf(bf + printed, size - printed, "%s%#x", printed ? "|" : "", flags);

	return printed;
}

size_t syscall_arg__scnprintf_clone_flags(char *bf, size_t size, struct syscall_arg *arg)
{
	unsigned long flags = arg->val;
	enum syscall_clone_args {
		SCC_FLAGS	  = (1 << 0),
		SCC_CHILD_STACK	  = (1 << 1),
		SCC_PARENT_TIDPTR = (1 << 2),
		SCC_CHILD_TIDPTR  = (1 << 3),
		SCC_TLS		  = (1 << 4),
	};
	if (!(flags & CLONE_PARENT_SETTID))
		arg->mask |= SCC_PARENT_TIDPTR;

	if (!(flags & (CLONE_CHILD_SETTID | CLONE_CHILD_CLEARTID)))
		arg->mask |= SCC_CHILD_TIDPTR;

	if (!(flags & CLONE_SETTLS))
		arg->mask |= SCC_TLS;

<<<<<<< HEAD
	return clone__scnprintf_flags(flags, bf, size);
=======
	return clone__scnprintf_flags(flags, bf, size, arg->show_string_prefix);
>>>>>>> upstream/android-13
}
