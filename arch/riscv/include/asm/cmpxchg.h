<<<<<<< HEAD
/*
 * Copyright (C) 2014 Regents of the University of California
 *
 *   This program is free software; you can redistribute it and/or
 *   modify it under the terms of the GNU General Public License
 *   as published by the Free Software Foundation, version 2.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
=======
/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Copyright (C) 2014 Regents of the University of California
>>>>>>> upstream/android-13
 */

#ifndef _ASM_RISCV_CMPXCHG_H
#define _ASM_RISCV_CMPXCHG_H

#include <linux/bug.h>

#include <asm/barrier.h>
#include <asm/fence.h>

#define __xchg_relaxed(ptr, new, size)					\
({									\
	__typeof__(ptr) __ptr = (ptr);					\
	__typeof__(new) __new = (new);					\
	__typeof__(*(ptr)) __ret;					\
	switch (size) {							\
	case 4:								\
		__asm__ __volatile__ (					\
			"	amoswap.w %0, %2, %1\n"			\
			: "=r" (__ret), "+A" (*__ptr)			\
			: "r" (__new)					\
			: "memory");					\
		break;							\
	case 8:								\
		__asm__ __volatile__ (					\
			"	amoswap.d %0, %2, %1\n"			\
			: "=r" (__ret), "+A" (*__ptr)			\
			: "r" (__new)					\
			: "memory");					\
		break;							\
	default:							\
		BUILD_BUG();						\
	}								\
	__ret;								\
})

<<<<<<< HEAD
#define xchg_relaxed(ptr, x)						\
=======
#define arch_xchg_relaxed(ptr, x)					\
>>>>>>> upstream/android-13
({									\
	__typeof__(*(ptr)) _x_ = (x);					\
	(__typeof__(*(ptr))) __xchg_relaxed((ptr),			\
					    _x_, sizeof(*(ptr)));	\
})

#define __xchg_acquire(ptr, new, size)					\
({									\
	__typeof__(ptr) __ptr = (ptr);					\
	__typeof__(new) __new = (new);					\
	__typeof__(*(ptr)) __ret;					\
	switch (size) {							\
	case 4:								\
		__asm__ __volatile__ (					\
			"	amoswap.w %0, %2, %1\n"			\
			RISCV_ACQUIRE_BARRIER				\
			: "=r" (__ret), "+A" (*__ptr)			\
			: "r" (__new)					\
			: "memory");					\
		break;							\
	case 8:								\
		__asm__ __volatile__ (					\
			"	amoswap.d %0, %2, %1\n"			\
			RISCV_ACQUIRE_BARRIER				\
			: "=r" (__ret), "+A" (*__ptr)			\
			: "r" (__new)					\
			: "memory");					\
		break;							\
	default:							\
		BUILD_BUG();						\
	}								\
	__ret;								\
})

<<<<<<< HEAD
#define xchg_acquire(ptr, x)						\
=======
#define arch_xchg_acquire(ptr, x)					\
>>>>>>> upstream/android-13
({									\
	__typeof__(*(ptr)) _x_ = (x);					\
	(__typeof__(*(ptr))) __xchg_acquire((ptr),			\
					    _x_, sizeof(*(ptr)));	\
})

#define __xchg_release(ptr, new, size)					\
({									\
	__typeof__(ptr) __ptr = (ptr);					\
	__typeof__(new) __new = (new);					\
	__typeof__(*(ptr)) __ret;					\
	switch (size) {							\
	case 4:								\
		__asm__ __volatile__ (					\
			RISCV_RELEASE_BARRIER				\
			"	amoswap.w %0, %2, %1\n"			\
			: "=r" (__ret), "+A" (*__ptr)			\
			: "r" (__new)					\
			: "memory");					\
		break;							\
	case 8:								\
		__asm__ __volatile__ (					\
			RISCV_RELEASE_BARRIER				\
			"	amoswap.d %0, %2, %1\n"			\
			: "=r" (__ret), "+A" (*__ptr)			\
			: "r" (__new)					\
			: "memory");					\
		break;							\
	default:							\
		BUILD_BUG();						\
	}								\
	__ret;								\
})

<<<<<<< HEAD
#define xchg_release(ptr, x)						\
=======
#define arch_xchg_release(ptr, x)					\
>>>>>>> upstream/android-13
({									\
	__typeof__(*(ptr)) _x_ = (x);					\
	(__typeof__(*(ptr))) __xchg_release((ptr),			\
					    _x_, sizeof(*(ptr)));	\
})

#define __xchg(ptr, new, size)						\
({									\
	__typeof__(ptr) __ptr = (ptr);					\
	__typeof__(new) __new = (new);					\
	__typeof__(*(ptr)) __ret;					\
	switch (size) {							\
	case 4:								\
		__asm__ __volatile__ (					\
			"	amoswap.w.aqrl %0, %2, %1\n"		\
			: "=r" (__ret), "+A" (*__ptr)			\
			: "r" (__new)					\
			: "memory");					\
		break;							\
	case 8:								\
		__asm__ __volatile__ (					\
			"	amoswap.d.aqrl %0, %2, %1\n"		\
			: "=r" (__ret), "+A" (*__ptr)			\
			: "r" (__new)					\
			: "memory");					\
		break;							\
	default:							\
		BUILD_BUG();						\
	}								\
	__ret;								\
})

<<<<<<< HEAD
#define xchg(ptr, x)							\
=======
#define arch_xchg(ptr, x)						\
>>>>>>> upstream/android-13
({									\
	__typeof__(*(ptr)) _x_ = (x);					\
	(__typeof__(*(ptr))) __xchg((ptr), _x_, sizeof(*(ptr)));	\
})

#define xchg32(ptr, x)							\
({									\
	BUILD_BUG_ON(sizeof(*(ptr)) != 4);				\
<<<<<<< HEAD
	xchg((ptr), (x));						\
=======
	arch_xchg((ptr), (x));						\
>>>>>>> upstream/android-13
})

#define xchg64(ptr, x)							\
({									\
	BUILD_BUG_ON(sizeof(*(ptr)) != 8);				\
<<<<<<< HEAD
	xchg((ptr), (x));						\
=======
	arch_xchg((ptr), (x));						\
>>>>>>> upstream/android-13
})

/*
 * Atomic compare and exchange.  Compare OLD with MEM, if identical,
 * store NEW in MEM.  Return the initial value in MEM.  Success is
 * indicated by comparing RETURN with OLD.
 */
#define __cmpxchg_relaxed(ptr, old, new, size)				\
({									\
	__typeof__(ptr) __ptr = (ptr);					\
	__typeof__(*(ptr)) __old = (old);				\
	__typeof__(*(ptr)) __new = (new);				\
	__typeof__(*(ptr)) __ret;					\
	register unsigned int __rc;					\
	switch (size) {							\
	case 4:								\
		__asm__ __volatile__ (					\
			"0:	lr.w %0, %2\n"				\
			"	bne  %0, %z3, 1f\n"			\
			"	sc.w %1, %z4, %2\n"			\
			"	bnez %1, 0b\n"				\
			"1:\n"						\
			: "=&r" (__ret), "=&r" (__rc), "+A" (*__ptr)	\
			: "rJ" ((long)__old), "rJ" (__new)		\
			: "memory");					\
		break;							\
	case 8:								\
		__asm__ __volatile__ (					\
			"0:	lr.d %0, %2\n"				\
			"	bne %0, %z3, 1f\n"			\
			"	sc.d %1, %z4, %2\n"			\
			"	bnez %1, 0b\n"				\
			"1:\n"						\
			: "=&r" (__ret), "=&r" (__rc), "+A" (*__ptr)	\
			: "rJ" (__old), "rJ" (__new)			\
			: "memory");					\
		break;							\
	default:							\
		BUILD_BUG();						\
	}								\
	__ret;								\
})

<<<<<<< HEAD
#define cmpxchg_relaxed(ptr, o, n)					\
=======
#define arch_cmpxchg_relaxed(ptr, o, n)					\
>>>>>>> upstream/android-13
({									\
	__typeof__(*(ptr)) _o_ = (o);					\
	__typeof__(*(ptr)) _n_ = (n);					\
	(__typeof__(*(ptr))) __cmpxchg_relaxed((ptr),			\
					_o_, _n_, sizeof(*(ptr)));	\
})

#define __cmpxchg_acquire(ptr, old, new, size)				\
({									\
	__typeof__(ptr) __ptr = (ptr);					\
	__typeof__(*(ptr)) __old = (old);				\
	__typeof__(*(ptr)) __new = (new);				\
	__typeof__(*(ptr)) __ret;					\
	register unsigned int __rc;					\
	switch (size) {							\
	case 4:								\
		__asm__ __volatile__ (					\
			"0:	lr.w %0, %2\n"				\
			"	bne  %0, %z3, 1f\n"			\
			"	sc.w %1, %z4, %2\n"			\
			"	bnez %1, 0b\n"				\
			RISCV_ACQUIRE_BARRIER				\
			"1:\n"						\
			: "=&r" (__ret), "=&r" (__rc), "+A" (*__ptr)	\
			: "rJ" ((long)__old), "rJ" (__new)		\
			: "memory");					\
		break;							\
	case 8:								\
		__asm__ __volatile__ (					\
			"0:	lr.d %0, %2\n"				\
			"	bne %0, %z3, 1f\n"			\
			"	sc.d %1, %z4, %2\n"			\
			"	bnez %1, 0b\n"				\
			RISCV_ACQUIRE_BARRIER				\
			"1:\n"						\
			: "=&r" (__ret), "=&r" (__rc), "+A" (*__ptr)	\
			: "rJ" (__old), "rJ" (__new)			\
			: "memory");					\
		break;							\
	default:							\
		BUILD_BUG();						\
	}								\
	__ret;								\
})

<<<<<<< HEAD
#define cmpxchg_acquire(ptr, o, n)					\
=======
#define arch_cmpxchg_acquire(ptr, o, n)					\
>>>>>>> upstream/android-13
({									\
	__typeof__(*(ptr)) _o_ = (o);					\
	__typeof__(*(ptr)) _n_ = (n);					\
	(__typeof__(*(ptr))) __cmpxchg_acquire((ptr),			\
					_o_, _n_, sizeof(*(ptr)));	\
})

#define __cmpxchg_release(ptr, old, new, size)				\
({									\
	__typeof__(ptr) __ptr = (ptr);					\
	__typeof__(*(ptr)) __old = (old);				\
	__typeof__(*(ptr)) __new = (new);				\
	__typeof__(*(ptr)) __ret;					\
	register unsigned int __rc;					\
	switch (size) {							\
	case 4:								\
		__asm__ __volatile__ (					\
			RISCV_RELEASE_BARRIER				\
			"0:	lr.w %0, %2\n"				\
			"	bne  %0, %z3, 1f\n"			\
			"	sc.w %1, %z4, %2\n"			\
			"	bnez %1, 0b\n"				\
			"1:\n"						\
			: "=&r" (__ret), "=&r" (__rc), "+A" (*__ptr)	\
			: "rJ" ((long)__old), "rJ" (__new)		\
			: "memory");					\
		break;							\
	case 8:								\
		__asm__ __volatile__ (					\
			RISCV_RELEASE_BARRIER				\
			"0:	lr.d %0, %2\n"				\
			"	bne %0, %z3, 1f\n"			\
			"	sc.d %1, %z4, %2\n"			\
			"	bnez %1, 0b\n"				\
			"1:\n"						\
			: "=&r" (__ret), "=&r" (__rc), "+A" (*__ptr)	\
			: "rJ" (__old), "rJ" (__new)			\
			: "memory");					\
		break;							\
	default:							\
		BUILD_BUG();						\
	}								\
	__ret;								\
})

<<<<<<< HEAD
#define cmpxchg_release(ptr, o, n)					\
=======
#define arch_cmpxchg_release(ptr, o, n)					\
>>>>>>> upstream/android-13
({									\
	__typeof__(*(ptr)) _o_ = (o);					\
	__typeof__(*(ptr)) _n_ = (n);					\
	(__typeof__(*(ptr))) __cmpxchg_release((ptr),			\
					_o_, _n_, sizeof(*(ptr)));	\
})

#define __cmpxchg(ptr, old, new, size)					\
({									\
	__typeof__(ptr) __ptr = (ptr);					\
	__typeof__(*(ptr)) __old = (old);				\
	__typeof__(*(ptr)) __new = (new);				\
	__typeof__(*(ptr)) __ret;					\
	register unsigned int __rc;					\
	switch (size) {							\
	case 4:								\
		__asm__ __volatile__ (					\
			"0:	lr.w %0, %2\n"				\
			"	bne  %0, %z3, 1f\n"			\
			"	sc.w.rl %1, %z4, %2\n"			\
			"	bnez %1, 0b\n"				\
			"	fence rw, rw\n"				\
			"1:\n"						\
			: "=&r" (__ret), "=&r" (__rc), "+A" (*__ptr)	\
			: "rJ" ((long)__old), "rJ" (__new)		\
			: "memory");					\
		break;							\
	case 8:								\
		__asm__ __volatile__ (					\
			"0:	lr.d %0, %2\n"				\
			"	bne %0, %z3, 1f\n"			\
			"	sc.d.rl %1, %z4, %2\n"			\
			"	bnez %1, 0b\n"				\
			"	fence rw, rw\n"				\
			"1:\n"						\
			: "=&r" (__ret), "=&r" (__rc), "+A" (*__ptr)	\
			: "rJ" (__old), "rJ" (__new)			\
			: "memory");					\
		break;							\
	default:							\
		BUILD_BUG();						\
	}								\
	__ret;								\
})

<<<<<<< HEAD
#define cmpxchg(ptr, o, n)						\
=======
#define arch_cmpxchg(ptr, o, n)						\
>>>>>>> upstream/android-13
({									\
	__typeof__(*(ptr)) _o_ = (o);					\
	__typeof__(*(ptr)) _n_ = (n);					\
	(__typeof__(*(ptr))) __cmpxchg((ptr),				\
				       _o_, _n_, sizeof(*(ptr)));	\
})

<<<<<<< HEAD
#define cmpxchg_local(ptr, o, n)					\
=======
#define arch_cmpxchg_local(ptr, o, n)					\
>>>>>>> upstream/android-13
	(__cmpxchg_relaxed((ptr), (o), (n), sizeof(*(ptr))))

#define cmpxchg32(ptr, o, n)						\
({									\
	BUILD_BUG_ON(sizeof(*(ptr)) != 4);				\
<<<<<<< HEAD
	cmpxchg((ptr), (o), (n));					\
=======
	arch_cmpxchg((ptr), (o), (n));					\
>>>>>>> upstream/android-13
})

#define cmpxchg32_local(ptr, o, n)					\
({									\
	BUILD_BUG_ON(sizeof(*(ptr)) != 4);				\
<<<<<<< HEAD
	cmpxchg_relaxed((ptr), (o), (n))				\
})

#define cmpxchg64(ptr, o, n)						\
({									\
	BUILD_BUG_ON(sizeof(*(ptr)) != 8);				\
	cmpxchg((ptr), (o), (n));					\
})

#define cmpxchg64_local(ptr, o, n)					\
({									\
	BUILD_BUG_ON(sizeof(*(ptr)) != 8);				\
	cmpxchg_relaxed((ptr), (o), (n));				\
=======
	arch_cmpxchg_relaxed((ptr), (o), (n))				\
})

#define arch_cmpxchg64(ptr, o, n)					\
({									\
	BUILD_BUG_ON(sizeof(*(ptr)) != 8);				\
	arch_cmpxchg((ptr), (o), (n));					\
})

#define arch_cmpxchg64_local(ptr, o, n)					\
({									\
	BUILD_BUG_ON(sizeof(*(ptr)) != 8);				\
	arch_cmpxchg_relaxed((ptr), (o), (n));				\
>>>>>>> upstream/android-13
})

#endif /* _ASM_RISCV_CMPXCHG_H */
