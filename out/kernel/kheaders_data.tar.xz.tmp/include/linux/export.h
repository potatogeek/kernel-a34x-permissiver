#ifndef _LINUX_EXPORT_H
#define _LINUX_EXPORT_H



#ifndef __ASSEMBLY__
#ifdef MODULE
extern struct module __this_module;
#define THIS_MODULE (&__this_module)
#else
#define THIS_MODULE ((struct module *)0)
#endif

#ifdef CONFIG_MODULES

#if defined(__KERNEL__) && !defined(__GENKSYMS__)
#ifdef CONFIG_MODVERSIONS

#if defined(CONFIG_MODULE_REL_CRCS)
#define __CRC_SYMBOL(sym, sec)						\
	asm("	.section \"___kcrctab" sec "+" #sym "\", \"a\"	\n"	\
	    "	.weak	__crc_" #sym "				\n"	\
	    "	.long	__crc_" #sym " - .			\n"	\
	    "	.previous					\n");
#else
#define __CRC_SYMBOL(sym, sec)						\
	asm("	.section \"___kcrctab" sec "+" #sym "\", \"a\"	\n"	\
	    "	.weak	__crc_" #sym "				\n"	\
	    "	.long	__crc_" #sym "				\n"	\
	    "	.previous					\n");
#endif
#else
#define __CRC_SYMBOL(sym, sec)
#endif

#ifdef CONFIG_HAVE_ARCH_PREL32_RELOCATIONS
#include <linux/compiler.h>

#define __KSYMTAB_ENTRY(sym, sec)					\
	__ADDRESSABLE(sym)						\
	asm("	.section \"___ksymtab" sec "+" #sym "\", \"a\"	\n"	\
	    "	.balign	8					\n"	\
	    "__ksymtab_" #sym ":				\n"	\
	    "	.long	" #sym "- .				\n"	\
	    "	.long	__kstrtab_" #sym "- .			\n"	\
	    "	.previous					\n")

struct kernel_symbol {
	int value_offset;
	int name_offset;
};
#else
#define __KSYMTAB_ENTRY(sym, sec)					\
	static const struct kernel_symbol __ksymtab_##sym		\
	__attribute__((section("___ksymtab" sec "+" #sym), used))	\
	= { (unsigned long)&sym, __kstrtab_##sym }

struct kernel_symbol {
	unsigned long value;
	const char *name;
};
#endif


#define ___EXPORT_SYMBOL(sym, sec)					\
	extern typeof(sym) sym;						\
	__CRC_SYMBOL(sym, sec)						\
	static const char __kstrtab_##sym[]				\
	__attribute__((section("__ksymtab_strings"), used, aligned(1)))	\
	= #sym;								\
	__KSYMTAB_ENTRY(sym, sec)

#if defined(__DISABLE_EXPORTS)


#define __EXPORT_SYMBOL(sym, sec)

#elif defined(__KSYM_DEPS__)


#define __EXPORT_SYMBOL(sym, sec)	=== __KSYM_##sym ===

#elif defined(CONFIG_TRIM_UNUSED_KSYMS) && !defined(MODULE)

#include <generated/autoksyms.h>

#define __EXPORT_SYMBOL(sym, sec)				\
	__cond_export_sym(sym, sec, __is_defined(__KSYM_##sym))
#define __cond_export_sym(sym, sec, conf)			\
	___cond_export_sym(sym, sec, conf)
#define ___cond_export_sym(sym, sec, enabled)			\
	__cond_export_sym_##enabled(sym, sec)
#define __cond_export_sym_1(sym, sec) ___EXPORT_SYMBOL(sym, sec)
#define __cond_export_sym_0(sym, sec) 

#else
#define __EXPORT_SYMBOL ___EXPORT_SYMBOL
#endif

#define EXPORT_SYMBOL(sym)					\
	__EXPORT_SYMBOL(sym, "")
#if IS_ENABLED(CONFIG_SEC_KUNIT) && IS_ENABLED(CONFIG_KUNIT)
#define EXPORT_SYMBOL_KUNIT(sym)	__EXPORT_SYMBOL(sym, "")
#else
#define EXPORT_SYMBOL_KUNIT(sym)	
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

#endif	

#else 

#define EXPORT_SYMBOL(sym)
#define EXPORT_SYMBOL_GPL(sym)
#define EXPORT_SYMBOL_GPL_FUTURE(sym)
#define EXPORT_UNUSED_SYMBOL(sym)
#define EXPORT_UNUSED_SYMBOL_GPL(sym)

#endif 
#endif 

#endif 
