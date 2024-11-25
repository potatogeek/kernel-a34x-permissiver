<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> upstream/android-13
#ifndef _LINUX_EXPORT_H
#define _LINUX_EXPORT_H

/*
 * Export symbols from the kernel to modules.  Forked from module.h
 * to reduce the amount of pointless cruft we feed to gcc when only
 * exporting a simple symbol or two.
 *
 * Try not to add #includes here.  It slows compilation and makes kernel
 * hackers place grumpy comments in header files.
 */

#ifndef __ASSEMBLY__
#ifdef MODULE
extern struct module __this_module;
#define THIS_MODULE (&__this_module)
#else
#define THIS_MODULE ((struct module *)0)
#endif

<<<<<<< HEAD
#ifdef CONFIG_MODULES

#if defined(__KERNEL__) && !defined(__GENKSYMS__)
=======
>>>>>>> upstream/android-13
#ifdef CONFIG_MODVERSIONS
/* Mark the CRC weak since genksyms apparently decides not to
 * generate a checksums for some symbols */
#if defined(CONFIG_MODULE_REL_CRCS)
#define __CRC_SYMBOL(sym, sec)						\
	asm("	.section \"___kcrctab" sec "+" #sym "\", \"a\"	\n"	\
	    "	.weak	__crc_" #sym "				\n"	\
	    "	.long	__crc_" #sym " - .			\n"	\
<<<<<<< HEAD
	    "	.previous					\n");
=======
	    "	.previous					\n")
>>>>>>> upstream/android-13
#else
#define __CRC_SYMBOL(sym, sec)						\
	asm("	.section \"___kcrctab" sec "+" #sym "\", \"a\"	\n"	\
	    "	.weak	__crc_" #sym "				\n"	\
	    "	.long	__crc_" #sym "				\n"	\
<<<<<<< HEAD
	    "	.previous					\n");
=======
	    "	.previous					\n")
>>>>>>> upstream/android-13
#endif
#else
#define __CRC_SYMBOL(sym, sec)
#endif

#ifdef CONFIG_HAVE_ARCH_PREL32_RELOCATIONS
#include <linux/compiler.h>
/*
 * Emit the ksymtab entry as a pair of relative references: this reduces
 * the size by half on 64-bit architectures, and eliminates the need for
 * absolute relocations that require runtime processing on relocatable
 * kernels.
 */
#define __KSYMTAB_ENTRY(sym, sec)					\
	__ADDRESSABLE(sym)						\
	asm("	.section \"___ksymtab" sec "+" #sym "\", \"a\"	\n"	\
<<<<<<< HEAD
	    "	.balign	8					\n"	\
	    "__ksymtab_" #sym ":				\n"	\
	    "	.long	" #sym "- .				\n"	\
	    "	.long	__kstrtab_" #sym "- .			\n"	\
=======
	    "	.balign	4					\n"	\
	    "__ksymtab_" #sym ":				\n"	\
	    "	.long	" #sym "- .				\n"	\
	    "	.long	__kstrtab_" #sym "- .			\n"	\
	    "	.long	__kstrtabns_" #sym "- .			\n"	\
>>>>>>> upstream/android-13
	    "	.previous					\n")

struct kernel_symbol {
	int value_offset;
	int name_offset;
<<<<<<< HEAD
=======
	int namespace_offset;
>>>>>>> upstream/android-13
};
#else
#define __KSYMTAB_ENTRY(sym, sec)					\
	static const struct kernel_symbol __ksymtab_##sym		\
	__attribute__((section("___ksymtab" sec "+" #sym), used))	\
<<<<<<< HEAD
	= { (unsigned long)&sym, __kstrtab_##sym }
=======
	__aligned(sizeof(void *))					\
	= { (unsigned long)&sym, __kstrtab_##sym, __kstrtabns_##sym }
>>>>>>> upstream/android-13

struct kernel_symbol {
	unsigned long value;
	const char *name;
<<<<<<< HEAD
};
#endif

/* For every exported symbol, place a struct in the __ksymtab section */
#define ___EXPORT_SYMBOL(sym, sec)					\
	extern typeof(sym) sym;						\
	__CRC_SYMBOL(sym, sec)						\
	static const char __kstrtab_##sym[]				\
	__attribute__((section("__ksymtab_strings"), used, aligned(1)))	\
	= #sym;								\
	__KSYMTAB_ENTRY(sym, sec)

#if defined(__DISABLE_EXPORTS)
=======
	const char *namespace;
};
#endif

#ifdef __GENKSYMS__

#define ___EXPORT_SYMBOL(sym, sec, ns)	__GENKSYMS_EXPORT_SYMBOL(sym)

#else

/*
 * For every exported symbol, do the following:
 *
 * - If applicable, place a CRC entry in the __kcrctab section.
 * - Put the name of the symbol and namespace (empty string "" for none) in
 *   __ksymtab_strings.
 * - Place a struct kernel_symbol entry in the __ksymtab section.
 *
 * note on .section use: we specify progbits since usage of the "M" (SHF_MERGE)
 * section flag requires it. Use '%progbits' instead of '@progbits' since the
 * former apparently works on all arches according to the binutils source.
 */
#define ___EXPORT_SYMBOL(sym, sec, ns)						\
	extern typeof(sym) sym;							\
	extern const char __kstrtab_##sym[];					\
	extern const char __kstrtabns_##sym[];					\
	__CRC_SYMBOL(sym, sec);							\
	asm("	.section \"__ksymtab_strings\",\"aMS\",%progbits,1	\n"	\
	    "__kstrtab_" #sym ":					\n"	\
	    "	.asciz 	\"" #sym "\"					\n"	\
	    "__kstrtabns_" #sym ":					\n"	\
	    "	.asciz 	\"" ns "\"					\n"	\
	    "	.previous						\n");	\
	__KSYMTAB_ENTRY(sym, sec)

#endif

#if !defined(CONFIG_MODULES) || defined(__DISABLE_EXPORTS)
>>>>>>> upstream/android-13

/*
 * Allow symbol exports to be disabled completely so that C code may
 * be reused in other execution contexts such as the UEFI stub or the
 * decompressor.
 */
<<<<<<< HEAD
#define __EXPORT_SYMBOL(sym, sec)

#elif defined(__KSYM_DEPS__)

/*
 * For fine grained build dependencies, we want to tell the build system
 * about each possible exported symbol even if they're not actually exported.
 * We use a string pattern that is unlikely to be valid code that the build
 * system filters out from the preprocessor output (see ksym_dep_filter
 * in scripts/Kbuild.include).
 */
#define __EXPORT_SYMBOL(sym, sec)	=== __KSYM_##sym ===
=======
#define __EXPORT_SYMBOL(sym, sec, ns)
>>>>>>> upstream/android-13

#elif defined(CONFIG_TRIM_UNUSED_KSYMS) && !defined(MODULE)

#include <generated/autoksyms.h>

<<<<<<< HEAD
#define __EXPORT_SYMBOL(sym, sec)				\
	__cond_export_sym(sym, sec, __is_defined(__KSYM_##sym))
#define __cond_export_sym(sym, sec, conf)			\
	___cond_export_sym(sym, sec, conf)
#define ___cond_export_sym(sym, sec, enabled)			\
	__cond_export_sym_##enabled(sym, sec)
#define __cond_export_sym_1(sym, sec) ___EXPORT_SYMBOL(sym, sec)
#define __cond_export_sym_0(sym, sec) /* nothing */

#else
#define __EXPORT_SYMBOL ___EXPORT_SYMBOL
#endif

#define EXPORT_SYMBOL(sym)					\
	__EXPORT_SYMBOL(sym, "")
#if IS_ENABLED(CONFIG_SEC_KUNIT) && IS_ENABLED(CONFIG_KUNIT)
#define EXPORT_SYMBOL_KUNIT(sym)	__EXPORT_SYMBOL(sym, "")
#else
#define EXPORT_SYMBOL_KUNIT(sym)	/* nothing */
#endif

#define EXPORT_SYMBOL_GPL(sym)					\
	__EXPORT_SYMBOL(sym, "_gpl")

#define EXPORT_SYMBOL_GPL_FUTURE(sym)				\
	__EXPORT_SYMBOL(sym, "_gpl_future")

#ifdef CONFIG_UNUSED_SYMBOLS
#define EXPORT_UNUSED_SYMBOL(sym) __EXPORT_SYMBOL(sym, "_unused")
#define EXPORT_UNUSED_SYMBOL_GPL(sym) __EXPORT_SYMBOL(sym, "_unused_gpl")
#else
#define EXPORT_UNUSED_SYMBOL(sym)
#define EXPORT_UNUSED_SYMBOL_GPL(sym)
#endif

#endif	/* __GENKSYMS__ */

#else /* !CONFIG_MODULES... */

#define EXPORT_SYMBOL(sym)
#define EXPORT_SYMBOL_GPL(sym)
#define EXPORT_SYMBOL_GPL_FUTURE(sym)
#define EXPORT_UNUSED_SYMBOL(sym)
#define EXPORT_UNUSED_SYMBOL_GPL(sym)

#endif /* CONFIG_MODULES */
=======
/*
 * For fine grained build dependencies, we want to tell the build system
 * about each possible exported symbol even if they're not actually exported.
 * We use a symbol pattern __ksym_marker_<symbol> that the build system filters
 * from the $(NM) output (see scripts/gen_ksymdeps.sh). These symbols are
 * discarded in the final link stage.
 */
#define __ksym_marker(sym)	\
	static int __ksym_marker_##sym[0] __section(".discard.ksym") __used

#define __EXPORT_SYMBOL(sym, sec, ns)					\
	__ksym_marker(sym);						\
	__cond_export_sym(sym, sec, ns, __is_defined(__KSYM_##sym))
#define __cond_export_sym(sym, sec, ns, conf)				\
	___cond_export_sym(sym, sec, ns, conf)
#define ___cond_export_sym(sym, sec, ns, enabled)			\
	__cond_export_sym_##enabled(sym, sec, ns)
#define __cond_export_sym_1(sym, sec, ns) ___EXPORT_SYMBOL(sym, sec, ns)

#ifdef __GENKSYMS__
#define __cond_export_sym_0(sym, sec, ns) __GENKSYMS_EXPORT_SYMBOL(sym)
#else
#define __cond_export_sym_0(sym, sec, ns) /* nothing */
#endif

#else

#define __EXPORT_SYMBOL(sym, sec, ns)	___EXPORT_SYMBOL(sym, sec, ns)

#endif /* CONFIG_MODULES */

#ifdef DEFAULT_SYMBOL_NAMESPACE
#include <linux/stringify.h>
#define _EXPORT_SYMBOL(sym, sec)	__EXPORT_SYMBOL(sym, sec, __stringify(DEFAULT_SYMBOL_NAMESPACE))
#else
#define _EXPORT_SYMBOL(sym, sec)	__EXPORT_SYMBOL(sym, sec, "")
#endif

#define EXPORT_SYMBOL(sym)		_EXPORT_SYMBOL(sym, "")
#define EXPORT_SYMBOL_GPL(sym)		_EXPORT_SYMBOL(sym, "_gpl")
#define _EXPORT_SYMBOL_NS(sym, ns)	__EXPORT_SYMBOL(sym, "", #ns)
#define _EXPORT_SYMBOL_NS_GPL(sym, ns)	__EXPORT_SYMBOL(sym, "_gpl", #ns)
#define EXPORT_SYMBOL_NS(sym, ns)	_EXPORT_SYMBOL_NS(sym, ns)
#define EXPORT_SYMBOL_NS_GPL(sym, ns)	_EXPORT_SYMBOL_NS_GPL(sym, ns)

>>>>>>> upstream/android-13
#endif /* !__ASSEMBLY__ */

#endif /* _LINUX_EXPORT_H */
