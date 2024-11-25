<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> upstream/android-13
/*
 * Atomic futex routines
 *
 * Based on the PowerPC implementataion
 *
<<<<<<< HEAD
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
=======
>>>>>>> upstream/android-13
 * Copyright (C) 2013 TangoTec Ltd.
 *
 * Baruch Siach <baruch@tkos.co.il>
 */

#ifndef _ASM_XTENSA_FUTEX_H
#define _ASM_XTENSA_FUTEX_H

<<<<<<< HEAD
#ifdef __KERNEL__

=======
>>>>>>> upstream/android-13
#include <linux/futex.h>
#include <linux/uaccess.h>
#include <linux/errno.h>

<<<<<<< HEAD
#define __futex_atomic_op(insn, ret, oldval, uaddr, oparg) \
	__asm__ __volatile(				\
	"1:	l32i	%0, %2, 0\n"			\
		insn "\n"				\
	"	wsr	%0, scompare1\n"		\
	"2:	s32c1i	%1, %2, 0\n"			\
	"	bne	%1, %0, 1b\n"			\
	"	movi	%1, 0\n"			\
	"3:\n"						\
	"	.section .fixup,\"ax\"\n"		\
	"	.align 4\n"				\
	"4:	.long	3b\n"				\
	"5:	l32r	%0, 4b\n"			\
	"	movi	%1, %3\n"			\
	"	jx	%0\n"				\
	"	.previous\n"				\
	"	.section __ex_table,\"a\"\n"		\
	"	.long 1b,5b,2b,5b\n"			\
	"	.previous\n"				\
	: "=&r" (oldval), "=&r" (ret)			\
	: "r" (uaddr), "I" (-EFAULT), "r" (oparg)	\
	: "memory")
=======
#if XCHAL_HAVE_EXCLUSIVE
#define __futex_atomic_op(insn, ret, old, uaddr, arg)	\
	__asm__ __volatile(				\
	"1:	l32ex	%[oldval], %[addr]\n"		\
		insn "\n"				\
	"2:	s32ex	%[newval], %[addr]\n"		\
	"	getex	%[newval]\n"			\
	"	beqz	%[newval], 1b\n"		\
	"	movi	%[newval], 0\n"			\
	"3:\n"						\
	"	.section .fixup,\"ax\"\n"		\
	"	.align 4\n"				\
	"	.literal_position\n"			\
	"5:	movi	%[oldval], 3b\n"		\
	"	movi	%[newval], %[fault]\n"		\
	"	jx	%[oldval]\n"			\
	"	.previous\n"				\
	"	.section __ex_table,\"a\"\n"		\
	"	.long 1b, 5b, 2b, 5b\n"			\
	"	.previous\n"				\
	: [oldval] "=&r" (old), [newval] "=&r" (ret)	\
	: [addr] "r" (uaddr), [oparg] "r" (arg),	\
	  [fault] "I" (-EFAULT)				\
	: "memory")
#elif XCHAL_HAVE_S32C1I
#define __futex_atomic_op(insn, ret, old, uaddr, arg)	\
	__asm__ __volatile(				\
	"1:	l32i	%[oldval], %[mem]\n"		\
		insn "\n"				\
	"	wsr	%[oldval], scompare1\n"		\
	"2:	s32c1i	%[newval], %[mem]\n"		\
	"	bne	%[newval], %[oldval], 1b\n"	\
	"	movi	%[newval], 0\n"			\
	"3:\n"						\
	"	.section .fixup,\"ax\"\n"		\
	"	.align 4\n"				\
	"	.literal_position\n"			\
	"5:	movi	%[oldval], 3b\n"		\
	"	movi	%[newval], %[fault]\n"		\
	"	jx	%[oldval]\n"			\
	"	.previous\n"				\
	"	.section __ex_table,\"a\"\n"		\
	"	.long 1b, 5b, 2b, 5b\n"			\
	"	.previous\n"				\
	: [oldval] "=&r" (old), [newval] "=&r" (ret),	\
	  [mem] "+m" (*(uaddr))				\
	: [oparg] "r" (arg), [fault] "I" (-EFAULT)	\
	: "memory")
#endif
>>>>>>> upstream/android-13

static inline int arch_futex_atomic_op_inuser(int op, int oparg, int *oval,
		u32 __user *uaddr)
{
<<<<<<< HEAD
	int oldval = 0, ret;

#if !XCHAL_HAVE_S32C1I
	return -ENOSYS;
#endif

	pagefault_disable();

	switch (op) {
	case FUTEX_OP_SET:
		__futex_atomic_op("mov %1, %4", ret, oldval, uaddr, oparg);
		break;
	case FUTEX_OP_ADD:
		__futex_atomic_op("add %1, %0, %4", ret, oldval, uaddr,
				oparg);
		break;
	case FUTEX_OP_OR:
		__futex_atomic_op("or %1, %0, %4", ret, oldval, uaddr,
				oparg);
		break;
	case FUTEX_OP_ANDN:
		__futex_atomic_op("and %1, %0, %4", ret, oldval, uaddr,
				~oparg);
		break;
	case FUTEX_OP_XOR:
		__futex_atomic_op("xor %1, %0, %4", ret, oldval, uaddr,
				oparg);
=======
#if XCHAL_HAVE_S32C1I || XCHAL_HAVE_EXCLUSIVE
	int oldval = 0, ret;

	if (!access_ok(uaddr, sizeof(u32)))
		return -EFAULT;

	switch (op) {
	case FUTEX_OP_SET:
		__futex_atomic_op("mov %[newval], %[oparg]",
				  ret, oldval, uaddr, oparg);
		break;
	case FUTEX_OP_ADD:
		__futex_atomic_op("add %[newval], %[oldval], %[oparg]",
				  ret, oldval, uaddr, oparg);
		break;
	case FUTEX_OP_OR:
		__futex_atomic_op("or %[newval], %[oldval], %[oparg]",
				  ret, oldval, uaddr, oparg);
		break;
	case FUTEX_OP_ANDN:
		__futex_atomic_op("and %[newval], %[oldval], %[oparg]",
				  ret, oldval, uaddr, ~oparg);
		break;
	case FUTEX_OP_XOR:
		__futex_atomic_op("xor %[newval], %[oldval], %[oparg]",
				  ret, oldval, uaddr, oparg);
>>>>>>> upstream/android-13
		break;
	default:
		ret = -ENOSYS;
	}

<<<<<<< HEAD
	pagefault_enable();

=======
>>>>>>> upstream/android-13
	if (!ret)
		*oval = oldval;

	return ret;
<<<<<<< HEAD
=======
#else
	return -ENOSYS;
#endif
>>>>>>> upstream/android-13
}

static inline int
futex_atomic_cmpxchg_inatomic(u32 *uval, u32 __user *uaddr,
			      u32 oldval, u32 newval)
{
<<<<<<< HEAD
	int ret = 0;

	if (!access_ok(VERIFY_WRITE, uaddr, sizeof(u32)))
		return -EFAULT;

#if !XCHAL_HAVE_S32C1I
	return -ENOSYS;
#endif

	__asm__ __volatile__ (
	"	# futex_atomic_cmpxchg_inatomic\n"
	"	wsr	%5, scompare1\n"
	"1:	s32c1i	%1, %4, 0\n"
	"	s32i	%1, %6, 0\n"
	"2:\n"
	"	.section .fixup,\"ax\"\n"
	"	.align 4\n"
	"3:	.long	2b\n"
	"4:	l32r	%1, 3b\n"
	"	movi	%0, %7\n"
	"	jx	%1\n"
	"	.previous\n"
	"	.section __ex_table,\"a\"\n"
	"	.long 1b,4b\n"
	"	.previous\n"
	: "+r" (ret), "+r" (newval), "+m" (*uaddr), "+m" (*uval)
	: "r" (uaddr), "r" (oldval), "r" (uval), "I" (-EFAULT)
	: "memory");

	return ret;
}

#endif /* __KERNEL__ */
=======
#if XCHAL_HAVE_S32C1I || XCHAL_HAVE_EXCLUSIVE
	unsigned long tmp;
	int ret = 0;

	if (!access_ok(uaddr, sizeof(u32)))
		return -EFAULT;

	__asm__ __volatile__ (
	"	# futex_atomic_cmpxchg_inatomic\n"
#if XCHAL_HAVE_EXCLUSIVE
	"1:	l32ex	%[tmp], %[addr]\n"
	"	s32i	%[tmp], %[uval], 0\n"
	"	bne	%[tmp], %[oldval], 2f\n"
	"	mov	%[tmp], %[newval]\n"
	"3:	s32ex	%[tmp], %[addr]\n"
	"	getex	%[tmp]\n"
	"	beqz	%[tmp], 1b\n"
#elif XCHAL_HAVE_S32C1I
	"	wsr	%[oldval], scompare1\n"
	"1:	s32c1i	%[newval], %[addr], 0\n"
	"	s32i	%[newval], %[uval], 0\n"
#endif
	"2:\n"
	"	.section .fixup,\"ax\"\n"
	"	.align 4\n"
	"	.literal_position\n"
	"4:	movi	%[tmp], 2b\n"
	"	movi	%[ret], %[fault]\n"
	"	jx	%[tmp]\n"
	"	.previous\n"
	"	.section __ex_table,\"a\"\n"
	"	.long 1b, 4b\n"
#if XCHAL_HAVE_EXCLUSIVE
	"	.long 3b, 4b\n"
#endif
	"	.previous\n"
	: [ret] "+r" (ret), [newval] "+r" (newval), [tmp] "=&r" (tmp)
	: [addr] "r" (uaddr), [oldval] "r" (oldval), [uval] "r" (uval),
	  [fault] "I" (-EFAULT)
	: "memory");

	return ret;
#else
	return -ENOSYS;
#endif
}

>>>>>>> upstream/android-13
#endif /* _ASM_XTENSA_FUTEX_H */
