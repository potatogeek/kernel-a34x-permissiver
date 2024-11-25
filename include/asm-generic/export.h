<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> upstream/android-13
#ifndef __ASM_GENERIC_EXPORT_H
#define __ASM_GENERIC_EXPORT_H

#ifndef KSYM_FUNC
#define KSYM_FUNC(x) x
#endif
<<<<<<< HEAD
#ifdef CONFIG_64BIT
#ifndef KSYM_ALIGN
#define KSYM_ALIGN 8
#endif
#else
#ifndef KSYM_ALIGN
#define KSYM_ALIGN 4
#endif
=======
#ifdef CONFIG_HAVE_ARCH_PREL32_RELOCATIONS
#define KSYM_ALIGN 4
#elif defined(CONFIG_64BIT)
#define KSYM_ALIGN 8
#else
#define KSYM_ALIGN 4
>>>>>>> upstream/android-13
#endif
#ifndef KCRC_ALIGN
#define KCRC_ALIGN 4
#endif

.macro __put, val, name
#ifdef CONFIG_HAVE_ARCH_PREL32_RELOCATIONS
<<<<<<< HEAD
	.long	\val - ., \name - .
#elif defined(CONFIG_64BIT)
	.quad	\val, \name
#else
	.long	\val, \name
=======
	.long	\val - ., \name - ., 0
#elif defined(CONFIG_64BIT)
	.quad	\val, \name, 0
#else
	.long	\val, \name, 0
>>>>>>> upstream/android-13
#endif
.endm

/*
<<<<<<< HEAD
 * note on .section use: @progbits vs %progbits nastiness doesn't matter,
 * since we immediately emit into those sections anyway.
 */
.macro ___EXPORT_SYMBOL name,val,sec
#ifdef CONFIG_MODULES
	.globl __ksymtab_\name
=======
 * note on .section use: we specify progbits since usage of the "M" (SHF_MERGE)
 * section flag requires it. Use '%progbits' instead of '@progbits' since the
 * former apparently works on all arches according to the binutils source.
 */

.macro ___EXPORT_SYMBOL name,val,sec
#if defined(CONFIG_MODULES) && !defined(__DISABLE_EXPORTS)
>>>>>>> upstream/android-13
	.section ___ksymtab\sec+\name,"a"
	.balign KSYM_ALIGN
__ksymtab_\name:
	__put \val, __kstrtab_\name
	.previous
<<<<<<< HEAD
	.section __ksymtab_strings,"a"
=======
	.section __ksymtab_strings,"aMS",%progbits,1
>>>>>>> upstream/android-13
__kstrtab_\name:
	.asciz "\name"
	.previous
#ifdef CONFIG_MODVERSIONS
	.section ___kcrctab\sec+\name,"a"
	.balign KCRC_ALIGN
<<<<<<< HEAD
__kcrctab_\name:
=======
>>>>>>> upstream/android-13
#if defined(CONFIG_MODULE_REL_CRCS)
	.long __crc_\name - .
#else
	.long __crc_\name
#endif
	.weak __crc_\name
	.previous
#endif
#endif
.endm
<<<<<<< HEAD
#undef __put

#if defined(__KSYM_DEPS__)

#define __EXPORT_SYMBOL(sym, val, sec)	=== __KSYM_##sym ===

#elif defined(CONFIG_TRIM_UNUSED_KSYMS)
=======

#if defined(CONFIG_TRIM_UNUSED_KSYMS)
>>>>>>> upstream/android-13

#include <linux/kconfig.h>
#include <generated/autoksyms.h>

<<<<<<< HEAD
#define __EXPORT_SYMBOL(sym, val, sec)				\
=======
.macro __ksym_marker sym
	.section ".discard.ksym","a"
__ksym_marker_\sym:
	 .previous
.endm

#define __EXPORT_SYMBOL(sym, val, sec)				\
	__ksym_marker sym;					\
>>>>>>> upstream/android-13
	__cond_export_sym(sym, val, sec, __is_defined(__KSYM_##sym))
#define __cond_export_sym(sym, val, sec, conf)			\
	___cond_export_sym(sym, val, sec, conf)
#define ___cond_export_sym(sym, val, sec, enabled)		\
	__cond_export_sym_##enabled(sym, val, sec)
#define __cond_export_sym_1(sym, val, sec) ___EXPORT_SYMBOL sym, val, sec
#define __cond_export_sym_0(sym, val, sec) /* nothing */

#else
#define __EXPORT_SYMBOL(sym, val, sec) ___EXPORT_SYMBOL sym, val, sec
#endif

#define EXPORT_SYMBOL(name)					\
	__EXPORT_SYMBOL(name, KSYM_FUNC(name),)
#define EXPORT_SYMBOL_GPL(name) 				\
	__EXPORT_SYMBOL(name, KSYM_FUNC(name), _gpl)
#define EXPORT_DATA_SYMBOL(name)				\
	__EXPORT_SYMBOL(name, name,)
#define EXPORT_DATA_SYMBOL_GPL(name)				\
	__EXPORT_SYMBOL(name, name,_gpl)

#endif
