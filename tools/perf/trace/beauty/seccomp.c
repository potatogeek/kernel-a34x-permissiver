<<<<<<< HEAD
// SPDX-License-Identifier: GPL-2.0
=======
// SPDX-License-Identifier: LGPL-2.1
>>>>>>> upstream/android-13
#ifndef SECCOMP_SET_MODE_STRICT
#define SECCOMP_SET_MODE_STRICT 0
#endif
#ifndef SECCOMP_SET_MODE_FILTER
#define SECCOMP_SET_MODE_FILTER 1
#endif

static size_t syscall_arg__scnprintf_seccomp_op(char *bf, size_t size, struct syscall_arg *arg)
{
<<<<<<< HEAD
=======
	bool show_prefix = arg->show_string_prefix;
	const char *prefix = "SECCOMP_SET_MODE_";
>>>>>>> upstream/android-13
	int op = arg->val;
	size_t printed = 0;

	switch (op) {
<<<<<<< HEAD
#define	P_SECCOMP_SET_MODE_OP(n) case SECCOMP_SET_MODE_##n: printed = scnprintf(bf, size, #n); break
=======
#define	P_SECCOMP_SET_MODE_OP(n) case SECCOMP_SET_MODE_##n: printed = scnprintf(bf, size, "%s%s", show_prefix ? prefix : "", #n); break
>>>>>>> upstream/android-13
	P_SECCOMP_SET_MODE_OP(STRICT);
	P_SECCOMP_SET_MODE_OP(FILTER);
#undef P_SECCOMP_SET_MODE_OP
	default: printed = scnprintf(bf, size, "%#x", op);			  break;
	}

	return printed;
}

#define SCA_SECCOMP_OP  syscall_arg__scnprintf_seccomp_op

#ifndef SECCOMP_FILTER_FLAG_TSYNC
#define SECCOMP_FILTER_FLAG_TSYNC 1
#endif

static size_t syscall_arg__scnprintf_seccomp_flags(char *bf, size_t size,
						   struct syscall_arg *arg)
{
<<<<<<< HEAD
=======
	bool show_prefix = arg->show_string_prefix;
	const char *prefix = "SECCOMP_FILTER_FLAG_";
>>>>>>> upstream/android-13
	int printed = 0, flags = arg->val;

#define	P_FLAG(n) \
	if (flags & SECCOMP_FILTER_FLAG_##n) { \
<<<<<<< HEAD
		printed += scnprintf(bf + printed, size - printed, "%s%s", printed ? "|" : "", #n); \
=======
		printed += scnprintf(bf + printed, size - printed, "%s%s%s", printed ? "|" : "", show_prefix ? prefix : "", #n); \
>>>>>>> upstream/android-13
		flags &= ~SECCOMP_FILTER_FLAG_##n; \
	}

	P_FLAG(TSYNC);
#undef P_FLAG

	if (flags)
		printed += scnprintf(bf + printed, size - printed, "%s%#x", printed ? "|" : "", flags);

	return printed;
}

#define SCA_SECCOMP_FLAGS syscall_arg__scnprintf_seccomp_flags
