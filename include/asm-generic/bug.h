/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _ASM_GENERIC_BUG_H
#define _ASM_GENERIC_BUG_H

#include <linux/compiler.h>
<<<<<<< HEAD
=======
#include <linux/instrumentation.h>
#include <linux/once_lite.h>
>>>>>>> upstream/android-13

#define CUT_HERE		"------------[ cut here ]------------\n"

#ifdef CONFIG_GENERIC_BUG
#define BUGFLAG_WARNING		(1 << 0)
#define BUGFLAG_ONCE		(1 << 1)
#define BUGFLAG_DONE		(1 << 2)
<<<<<<< HEAD
=======
#define BUGFLAG_NO_CUT_HERE	(1 << 3)	/* CUT_HERE already sent */
>>>>>>> upstream/android-13
#define BUGFLAG_TAINT(taint)	((taint) << 8)
#define BUG_GET_TAINT(bug)	((bug)->flags >> 8)
#endif

#ifndef __ASSEMBLY__
<<<<<<< HEAD
#include <linux/kernel.h>
=======
#include <linux/panic.h>
#include <linux/printk.h>
>>>>>>> upstream/android-13

#ifdef CONFIG_BUG

#ifdef CONFIG_GENERIC_BUG
struct bug_entry {
#ifndef CONFIG_GENERIC_BUG_RELATIVE_POINTERS
	unsigned long	bug_addr;
#else
	signed int	bug_addr_disp;
#endif
#ifdef CONFIG_DEBUG_BUGVERBOSE
#ifndef CONFIG_GENERIC_BUG_RELATIVE_POINTERS
	const char	*file;
#else
	signed int	file_disp;
#endif
	unsigned short	line;
#endif
	unsigned short	flags;
};
#endif	/* CONFIG_GENERIC_BUG */

/*
 * Don't use BUG() or BUG_ON() unless there's really no way out; one
 * example might be detecting data structure corruption in the middle
 * of an operation that can't be backed out of.  If the (sub)system
 * can somehow continue operating, perhaps with reduced functionality,
 * it's probably not BUG-worthy.
 *
 * If you're tempted to BUG(), think again:  is completely giving up
 * really the *only* solution?  There are usually better options, where
 * users don't need to reboot ASAP and can mostly shut down cleanly.
 */
#ifndef HAVE_ARCH_BUG
#define BUG() do { \
	printk("BUG: failure at %s:%d/%s()!\n", __FILE__, __LINE__, __func__); \
	barrier_before_unreachable(); \
	panic("BUG!"); \
} while (0)
#endif

#ifndef HAVE_ARCH_BUG_ON
#define BUG_ON(condition) do { if (unlikely(condition)) BUG(); } while (0)
#endif

<<<<<<< HEAD
#ifdef __WARN_FLAGS
#define __WARN_TAINT(taint)		__WARN_FLAGS(BUGFLAG_TAINT(taint))
#define __WARN_ONCE_TAINT(taint)	__WARN_FLAGS(BUGFLAG_ONCE|BUGFLAG_TAINT(taint))

#define WARN_ON_ONCE(condition) ({				\
	int __ret_warn_on = !!(condition);			\
	if (unlikely(__ret_warn_on))				\
		__WARN_ONCE_TAINT(TAINT_WARN);			\
	unlikely(__ret_warn_on);				\
})
#endif

=======
>>>>>>> upstream/android-13
/*
 * WARN(), WARN_ON(), WARN_ON_ONCE, and so on can be used to report
 * significant kernel issues that need prompt attention if they should ever
 * appear at runtime.
 *
 * Do not use these macros when checking for invalid external inputs
 * (e.g. invalid system call arguments, or invalid data coming from
 * network/devices), and on transient conditions like ENOMEM or EAGAIN.
 * These macros should be used for recoverable kernel issues only.
 * For invalid external inputs, transient conditions, etc use
 * pr_err[_once/_ratelimited]() followed by dump_stack(), if necessary.
 * Do not include "BUG"/"WARNING" in format strings manually to make these
 * conditions distinguishable from kernel issues.
 *
 * Use the versions with printk format strings to provide better diagnostics.
 */
<<<<<<< HEAD
#ifndef __WARN_TAINT
extern __printf(3, 4)
void warn_slowpath_fmt(const char *file, const int line,
		       const char *fmt, ...);
extern __printf(4, 5)
void warn_slowpath_fmt_taint(const char *file, const int line, unsigned taint,
			     const char *fmt, ...);
extern void warn_slowpath_null(const char *file, const int line);
#define WANT_WARN_ON_SLOWPATH
#define __WARN()		warn_slowpath_null(__FILE__, __LINE__)
#define __WARN_printf(arg...)	warn_slowpath_fmt(__FILE__, __LINE__, arg)
#define __WARN_printf_taint(taint, arg...)				\
	warn_slowpath_fmt_taint(__FILE__, __LINE__, taint, arg)
#else
extern __printf(1, 2) void __warn_printk(const char *fmt, ...);
#define __WARN() do { \
	printk(KERN_WARNING CUT_HERE); __WARN_TAINT(TAINT_WARN); \
} while (0)
#define __WARN_printf(arg...)	__WARN_printf_taint(TAINT_WARN, arg)
#define __WARN_printf_taint(taint, arg...)				\
	do { __warn_printk(arg); __WARN_TAINT(taint); } while (0)
=======
#ifndef __WARN_FLAGS
extern __printf(4, 5)
void warn_slowpath_fmt(const char *file, const int line, unsigned taint,
		       const char *fmt, ...);
#define __WARN()		__WARN_printf(TAINT_WARN, NULL)
#define __WARN_printf(taint, arg...) do {				\
		instrumentation_begin();				\
		warn_slowpath_fmt(__FILE__, __LINE__, taint, arg);	\
		instrumentation_end();					\
	} while (0)
#else
extern __printf(1, 2) void __warn_printk(const char *fmt, ...);
#define __WARN()		__WARN_FLAGS(BUGFLAG_TAINT(TAINT_WARN))
#define __WARN_printf(taint, arg...) do {				\
		instrumentation_begin();				\
		__warn_printk(arg);					\
		__WARN_FLAGS(BUGFLAG_NO_CUT_HERE | BUGFLAG_TAINT(taint));\
		instrumentation_end();					\
	} while (0)
#define WARN_ON_ONCE(condition) ({				\
	int __ret_warn_on = !!(condition);			\
	if (unlikely(__ret_warn_on))				\
		__WARN_FLAGS(BUGFLAG_ONCE |			\
			     BUGFLAG_TAINT(TAINT_WARN));	\
	unlikely(__ret_warn_on);				\
})
>>>>>>> upstream/android-13
#endif

/* used internally by panic.c */
struct warn_args;
struct pt_regs;

void __warn(const char *file, int line, void *caller, unsigned taint,
	    struct pt_regs *regs, struct warn_args *args);

#ifndef WARN_ON
#define WARN_ON(condition) ({						\
	int __ret_warn_on = !!(condition);				\
	if (unlikely(__ret_warn_on))					\
		__WARN();						\
	unlikely(__ret_warn_on);					\
})
#endif

#ifndef WARN
#define WARN(condition, format...) ({					\
	int __ret_warn_on = !!(condition);				\
	if (unlikely(__ret_warn_on))					\
<<<<<<< HEAD
		__WARN_printf(format);					\
=======
		__WARN_printf(TAINT_WARN, format);			\
>>>>>>> upstream/android-13
	unlikely(__ret_warn_on);					\
})
#endif

#define WARN_TAINT(condition, taint, format...) ({			\
	int __ret_warn_on = !!(condition);				\
	if (unlikely(__ret_warn_on))					\
<<<<<<< HEAD
		__WARN_printf_taint(taint, format);			\
=======
		__WARN_printf(taint, format);				\
>>>>>>> upstream/android-13
	unlikely(__ret_warn_on);					\
})

#ifndef WARN_ON_ONCE
<<<<<<< HEAD
#define WARN_ON_ONCE(condition)	({				\
	static bool __section(.data.once) __warned;		\
	int __ret_warn_once = !!(condition);			\
								\
	if (unlikely(__ret_warn_once && !__warned)) {		\
		__warned = true;				\
		WARN_ON(1);					\
	}							\
	unlikely(__ret_warn_once);				\
})
#endif

#define WARN_ONCE(condition, format...)	({			\
	static bool __section(.data.once) __warned;		\
	int __ret_warn_once = !!(condition);			\
								\
	if (unlikely(__ret_warn_once && !__warned)) {		\
		__warned = true;				\
		WARN(1, format);				\
	}							\
	unlikely(__ret_warn_once);				\
})

#define WARN_TAINT_ONCE(condition, taint, format...)	({	\
	static bool __section(.data.once) __warned;		\
	int __ret_warn_once = !!(condition);			\
								\
	if (unlikely(__ret_warn_once && !__warned)) {		\
		__warned = true;				\
		WARN_TAINT(1, taint, format);			\
	}							\
	unlikely(__ret_warn_once);				\
})
=======
#define WARN_ON_ONCE(condition)					\
	DO_ONCE_LITE_IF(condition, WARN_ON, 1)
#endif

#define WARN_ONCE(condition, format...)				\
	DO_ONCE_LITE_IF(condition, WARN, 1, format)

#define WARN_TAINT_ONCE(condition, taint, format...)		\
	DO_ONCE_LITE_IF(condition, WARN_TAINT, 1, taint, format)
>>>>>>> upstream/android-13

#else /* !CONFIG_BUG */
#ifndef HAVE_ARCH_BUG
#define BUG() do {} while (1)
#endif

#ifndef HAVE_ARCH_BUG_ON
<<<<<<< HEAD
#define BUG_ON(condition) do { if (condition) BUG(); } while (0)
=======
#define BUG_ON(condition) do { if (unlikely(condition)) BUG(); } while (0)
>>>>>>> upstream/android-13
#endif

#ifndef HAVE_ARCH_WARN_ON
#define WARN_ON(condition) ({						\
	int __ret_warn_on = !!(condition);				\
	unlikely(__ret_warn_on);					\
})
#endif

#ifndef WARN
#define WARN(condition, format...) ({					\
	int __ret_warn_on = !!(condition);				\
	no_printk(format);						\
	unlikely(__ret_warn_on);					\
})
#endif

#define WARN_ON_ONCE(condition) WARN_ON(condition)
#define WARN_ONCE(condition, format...) WARN(condition, format)
#define WARN_TAINT(condition, taint, format...) WARN(condition, format)
#define WARN_TAINT_ONCE(condition, taint, format...) WARN(condition, format)

#endif

/*
 * WARN_ON_SMP() is for cases that the warning is either
 * meaningless for !SMP or may even cause failures.
<<<<<<< HEAD
 * This is usually used for cases that we have
 * WARN_ON(!spin_is_locked(&lock)) checks, as spin_is_locked()
 * returns 0 for uniprocessor settings.
=======
>>>>>>> upstream/android-13
 * It can also be used with values that are only defined
 * on SMP:
 *
 * struct foo {
 *  [...]
 * #ifdef CONFIG_SMP
 *	int bar;
 * #endif
 * };
 *
 * void func(struct foo *zoot)
 * {
 *	WARN_ON_SMP(!zoot->bar);
 *
 * For CONFIG_SMP, WARN_ON_SMP() should act the same as WARN_ON(),
 * and should be a nop and return false for uniprocessor.
 *
 * if (WARN_ON_SMP(x)) returns true only when CONFIG_SMP is set
 * and x is true.
 */
#ifdef CONFIG_SMP
# define WARN_ON_SMP(x)			WARN_ON(x)
#else
/*
 * Use of ({0;}) because WARN_ON_SMP(x) may be used either as
 * a stand alone line statement or as a condition in an if ()
 * statement.
 * A simple "0" would cause gcc to give a "statement has no effect"
 * warning.
 */
# define WARN_ON_SMP(x)			({0;})
#endif

<<<<<<< HEAD
=======
/*
 * WARN_ON_FUNCTION_MISMATCH() warns if a value doesn't match a
 * function address, and can be useful for catching issues with
 * callback functions, for example.
 *
 * With CONFIG_CFI_CLANG, the warning is disabled because the
 * compiler replaces function addresses taken in C code with
 * local jump table addresses, which breaks cross-module function
 * address equality.
 */
#if defined(CONFIG_CFI_CLANG) && defined(CONFIG_MODULES)
# define WARN_ON_FUNCTION_MISMATCH(x, fn) ({ 0; })
#else
# define WARN_ON_FUNCTION_MISMATCH(x, fn) WARN_ON_ONCE((x) != (fn))
#endif

>>>>>>> upstream/android-13
#endif /* __ASSEMBLY__ */

#endif
