<<<<<<< HEAD
// SPDX-License-Identifier: GPL-2.0
=======
// SPDX-License-Identifier: LGPL-2.1
>>>>>>> upstream/android-13
#ifndef EFD_SEMAPHORE
#define EFD_SEMAPHORE		1
#endif

#ifndef EFD_NONBLOCK
#define EFD_NONBLOCK		00004000
#endif

#ifndef EFD_CLOEXEC
#define EFD_CLOEXEC		02000000
#endif

static size_t syscall_arg__scnprintf_eventfd_flags(char *bf, size_t size, struct syscall_arg *arg)
{
<<<<<<< HEAD
=======
	bool show_prefix = arg->show_string_prefix;
	const char *prefix = "EFD_";
>>>>>>> upstream/android-13
	int printed = 0, flags = arg->val;

	if (flags == 0)
		return scnprintf(bf, size, "NONE");
#define	P_FLAG(n) \
	if (flags & EFD_##n) { \
<<<<<<< HEAD
		printed += scnprintf(bf + printed, size - printed, "%s%s", printed ? "|" : "", #n); \
=======
		printed += scnprintf(bf + printed, size - printed, "%s%s%s", printed ? "|" : "", show_prefix ? prefix : "", #n); \
>>>>>>> upstream/android-13
		flags &= ~EFD_##n; \
	}

	P_FLAG(SEMAPHORE);
	P_FLAG(CLOEXEC);
	P_FLAG(NONBLOCK);
#undef P_FLAG

	if (flags)
		printed += scnprintf(bf + printed, size - printed, "%s%#x", printed ? "|" : "", flags);

	return printed;
}

#define SCA_EFD_FLAGS syscall_arg__scnprintf_eventfd_flags
