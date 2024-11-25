/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _ASM_POWERPC_BUG_H
#define _ASM_POWERPC_BUG_H
#ifdef __KERNEL__

#include <asm/asm-compat.h>
<<<<<<< HEAD

/*
 * Define an illegal instr to trap on the bug.
 * We don't use 0 because that marks the end of a function
 * in the ELF ABI.  That's "Boo Boo" in case you wonder...
 */
#define BUG_OPCODE .long 0x00b00b00  /* For asm */
#define BUG_ILLEGAL_INSTR "0x00b00b00" /* For BUG macro */
=======
#include <asm/extable.h>
>>>>>>> upstream/android-13

#ifdef CONFIG_BUG

#ifdef __ASSEMBLY__
#include <asm/asm-offsets.h>
#ifdef CONFIG_DEBUG_BUGVERBOSE
.macro EMIT_BUG_ENTRY addr,file,line,flags
	 .section __bug_table,"aw"
<<<<<<< HEAD
5001:	 PPC_LONG \addr, 5002f
=======
5001:	 .4byte \addr - 5001b, 5002f - 5001b
>>>>>>> upstream/android-13
	 .short \line, \flags
	 .org 5001b+BUG_ENTRY_SIZE
	 .previous
	 .section .rodata,"a"
5002:	 .asciz "\file"
	 .previous
.endm
#else
.macro EMIT_BUG_ENTRY addr,file,line,flags
	 .section __bug_table,"aw"
<<<<<<< HEAD
5001:	 PPC_LONG \addr
=======
5001:	 .4byte \addr - 5001b
>>>>>>> upstream/android-13
	 .short \flags
	 .org 5001b+BUG_ENTRY_SIZE
	 .previous
.endm
#endif /* verbose */

<<<<<<< HEAD
=======
.macro EMIT_WARN_ENTRY addr,file,line,flags
	EX_TABLE(\addr,\addr+4)
	EMIT_BUG_ENTRY \addr,\file,\line,\flags
.endm

>>>>>>> upstream/android-13
#else /* !__ASSEMBLY__ */
/* _EMIT_BUG_ENTRY expects args %0,%1,%2,%3 to be FILE, LINE, flags and
   sizeof(struct bug_entry), respectively */
#ifdef CONFIG_DEBUG_BUGVERBOSE
#define _EMIT_BUG_ENTRY				\
	".section __bug_table,\"aw\"\n"		\
<<<<<<< HEAD
	"2:\t" PPC_LONG "1b, %0\n"		\
=======
	"2:\t.4byte 1b - 2b, %0 - 2b\n"		\
>>>>>>> upstream/android-13
	"\t.short %1, %2\n"			\
	".org 2b+%3\n"				\
	".previous\n"
#else
#define _EMIT_BUG_ENTRY				\
	".section __bug_table,\"aw\"\n"		\
<<<<<<< HEAD
	"2:\t" PPC_LONG "1b\n"			\
=======
	"2:\t.4byte 1b - 2b\n"			\
>>>>>>> upstream/android-13
	"\t.short %2\n"				\
	".org 2b+%3\n"				\
	".previous\n"
#endif

<<<<<<< HEAD
=======
#define BUG_ENTRY(insn, flags, ...)			\
	__asm__ __volatile__(				\
		"1:	" insn "\n"			\
		_EMIT_BUG_ENTRY				\
		: : "i" (__FILE__), "i" (__LINE__),	\
		  "i" (flags),				\
		  "i" (sizeof(struct bug_entry)),	\
		  ##__VA_ARGS__)

#define WARN_ENTRY(insn, flags, label, ...)		\
	asm_volatile_goto(				\
		"1:	" insn "\n"			\
		EX_TABLE(1b, %l[label])			\
		_EMIT_BUG_ENTRY				\
		: : "i" (__FILE__), "i" (__LINE__),	\
		  "i" (flags),				\
		  "i" (sizeof(struct bug_entry)),	\
		  ##__VA_ARGS__ : : label)

>>>>>>> upstream/android-13
/*
 * BUG_ON() and WARN_ON() do their best to cooperate with compile-time
 * optimisations. However depending on the complexity of the condition
 * some compiler versions may not produce optimal results.
 */

#define BUG() do {						\
<<<<<<< HEAD
	__asm__ __volatile__(					\
		"1:	twi 31,0,0\n"				\
		_EMIT_BUG_ENTRY					\
		: : "i" (__FILE__), "i" (__LINE__),		\
		    "i" (0), "i"  (sizeof(struct bug_entry)));	\
	unreachable();						\
} while (0)

=======
	BUG_ENTRY("twi 31, 0, 0", 0);				\
	unreachable();						\
} while (0)
#define HAVE_ARCH_BUG

#define __WARN_FLAGS(flags) do {				\
	__label__ __label_warn_on;				\
								\
	WARN_ENTRY("twi 31, 0, 0", BUGFLAG_WARNING | (flags), __label_warn_on); \
	unreachable();						\
								\
__label_warn_on:						\
	break;							\
} while (0)

#ifdef CONFIG_PPC64
>>>>>>> upstream/android-13
#define BUG_ON(x) do {						\
	if (__builtin_constant_p(x)) {				\
		if (x)						\
			BUG();					\
	} else {						\
<<<<<<< HEAD
		__asm__ __volatile__(				\
		"1:	"PPC_TLNEI"	%4,0\n"			\
		_EMIT_BUG_ENTRY					\
		: : "i" (__FILE__), "i" (__LINE__), "i" (0),	\
		  "i" (sizeof(struct bug_entry)),		\
		  "r" ((__force long)(x)));			\
	}							\
} while (0)

#define __WARN_FLAGS(flags) do {				\
	__asm__ __volatile__(					\
		"1:	twi 31,0,0\n"				\
		_EMIT_BUG_ENTRY					\
		: : "i" (__FILE__), "i" (__LINE__),		\
		  "i" (BUGFLAG_WARNING|(flags)),		\
		  "i" (sizeof(struct bug_entry)));		\
} while (0)

#define WARN_ON(x) ({						\
	int __ret_warn_on = !!(x);				\
	if (__builtin_constant_p(__ret_warn_on)) {		\
		if (__ret_warn_on)				\
			__WARN();				\
	} else {						\
		__asm__ __volatile__(				\
		"1:	"PPC_TLNEI"	%4,0\n"			\
		_EMIT_BUG_ENTRY					\
		: : "i" (__FILE__), "i" (__LINE__),		\
		  "i" (BUGFLAG_WARNING|BUGFLAG_TAINT(TAINT_WARN)),\
		  "i" (sizeof(struct bug_entry)),		\
		  "r" (__ret_warn_on));				\
	}							\
	unlikely(__ret_warn_on);				\
})

#define HAVE_ARCH_BUG
#define HAVE_ARCH_BUG_ON
#define HAVE_ARCH_WARN_ON
=======
		BUG_ENTRY(PPC_TLNEI " %4, 0", 0, "r" ((__force long)(x)));	\
	}							\
} while (0)

#define WARN_ON(x) ({						\
	bool __ret_warn_on = false;				\
	do {							\
		if (__builtin_constant_p((x))) {		\
			if (!(x)) 				\
				break; 				\
			__WARN();				\
			__ret_warn_on = true;			\
		} else {					\
			__label__ __label_warn_on;		\
								\
			WARN_ENTRY(PPC_TLNEI " %4, 0",		\
				   BUGFLAG_WARNING | BUGFLAG_TAINT(TAINT_WARN),	\
				   __label_warn_on,		\
				   "r" ((__force long)(x)));	\
			break;					\
__label_warn_on:						\
			__ret_warn_on = true;			\
		}						\
	} while (0);						\
	unlikely(__ret_warn_on);				\
})

#define HAVE_ARCH_BUG_ON
#define HAVE_ARCH_WARN_ON
#endif

>>>>>>> upstream/android-13
#endif /* __ASSEMBLY __ */
#else
#ifdef __ASSEMBLY__
.macro EMIT_BUG_ENTRY addr,file,line,flags
.endm
<<<<<<< HEAD
#else /* !__ASSEMBLY__ */
#define _EMIT_BUG_ENTRY
=======
.macro EMIT_WARN_ENTRY addr,file,line,flags
.endm
#else /* !__ASSEMBLY__ */
#define _EMIT_BUG_ENTRY
#define _EMIT_WARN_ENTRY
>>>>>>> upstream/android-13
#endif
#endif /* CONFIG_BUG */

#include <asm-generic/bug.h>

#ifndef __ASSEMBLY__

struct pt_regs;
<<<<<<< HEAD
extern int do_page_fault(struct pt_regs *, unsigned long, unsigned long);
extern void bad_page_fault(struct pt_regs *, unsigned long, int);
extern void _exception(int, struct pt_regs *, int, unsigned long);
extern void _exception_pkey(int, struct pt_regs *, int, unsigned long, int);
extern void die(const char *, struct pt_regs *, long);
=======
void hash__do_page_fault(struct pt_regs *);
void bad_page_fault(struct pt_regs *, int);
extern void _exception(int, struct pt_regs *, int, unsigned long);
extern void _exception_pkey(struct pt_regs *, unsigned long, int);
extern void die(const char *, struct pt_regs *, long);
void die_mce(const char *str, struct pt_regs *regs, long err);
>>>>>>> upstream/android-13
extern bool die_will_crash(void);
extern void panic_flush_kmsg_start(void);
extern void panic_flush_kmsg_end(void);
#endif /* !__ASSEMBLY__ */

#endif /* __KERNEL__ */
#endif /* _ASM_POWERPC_BUG_H */
