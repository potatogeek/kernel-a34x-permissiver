/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _ALPHA_CMPXCHG_H
#define _ALPHA_CMPXCHG_H

/*
 * Atomic exchange routines.
 */

#define ____xchg(type, args...)		__xchg ## type ## _local(args)
#define ____cmpxchg(type, args...)	__cmpxchg ## type ## _local(args)
#include <asm/xchg.h>

#define xchg_local(ptr, x)						\
({									\
	__typeof__(*(ptr)) _x_ = (x);					\
	(__typeof__(*(ptr))) __xchg_local((ptr), (unsigned long)_x_,	\
				       sizeof(*(ptr)));			\
})

<<<<<<< HEAD
#define cmpxchg_local(ptr, o, n)					\
=======
#define arch_cmpxchg_local(ptr, o, n)					\
>>>>>>> upstream/android-13
({									\
	__typeof__(*(ptr)) _o_ = (o);					\
	__typeof__(*(ptr)) _n_ = (n);					\
	(__typeof__(*(ptr))) __cmpxchg_local((ptr), (unsigned long)_o_,	\
					  (unsigned long)_n_,		\
					  sizeof(*(ptr)));		\
})

<<<<<<< HEAD
#define cmpxchg64_local(ptr, o, n)					\
=======
#define arch_cmpxchg64_local(ptr, o, n)					\
>>>>>>> upstream/android-13
({									\
	BUILD_BUG_ON(sizeof(*(ptr)) != 8);				\
	cmpxchg_local((ptr), (o), (n));					\
})

#undef ____xchg
#undef ____cmpxchg
#define ____xchg(type, args...)		__xchg ##type(args)
#define ____cmpxchg(type, args...)	__cmpxchg ##type(args)
#include <asm/xchg.h>

/*
 * The leading and the trailing memory barriers guarantee that these
 * operations are fully ordered.
 */
<<<<<<< HEAD
#define xchg(ptr, x)							\
=======
#define arch_xchg(ptr, x)						\
>>>>>>> upstream/android-13
({									\
	__typeof__(*(ptr)) __ret;					\
	__typeof__(*(ptr)) _x_ = (x);					\
	smp_mb();							\
	__ret = (__typeof__(*(ptr)))					\
		__xchg((ptr), (unsigned long)_x_, sizeof(*(ptr)));	\
	smp_mb();							\
	__ret;								\
})

<<<<<<< HEAD
#define cmpxchg(ptr, o, n)						\
=======
#define arch_cmpxchg(ptr, o, n)						\
>>>>>>> upstream/android-13
({									\
	__typeof__(*(ptr)) __ret;					\
	__typeof__(*(ptr)) _o_ = (o);					\
	__typeof__(*(ptr)) _n_ = (n);					\
	smp_mb();							\
	__ret = (__typeof__(*(ptr))) __cmpxchg((ptr),			\
		(unsigned long)_o_, (unsigned long)_n_, sizeof(*(ptr)));\
	smp_mb();							\
	__ret;								\
})

<<<<<<< HEAD
#define cmpxchg64(ptr, o, n)						\
({									\
	BUILD_BUG_ON(sizeof(*(ptr)) != 8);				\
	cmpxchg((ptr), (o), (n));					\
=======
#define arch_cmpxchg64(ptr, o, n)					\
({									\
	BUILD_BUG_ON(sizeof(*(ptr)) != 8);				\
	arch_cmpxchg((ptr), (o), (n));					\
>>>>>>> upstream/android-13
})

#undef ____cmpxchg

#endif /* _ALPHA_CMPXCHG_H */
