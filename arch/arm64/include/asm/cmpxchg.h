<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> upstream/android-13
/*
 * Based on arch/arm/include/asm/cmpxchg.h
 *
 * Copyright (C) 2012 ARM Ltd.
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
=======
>>>>>>> upstream/android-13
 */
#ifndef __ASM_CMPXCHG_H
#define __ASM_CMPXCHG_H

#include <linux/build_bug.h>
#include <linux/compiler.h>

<<<<<<< HEAD
#include <asm/atomic.h>
=======
>>>>>>> upstream/android-13
#include <asm/barrier.h>
#include <asm/lse.h>

/*
 * We need separate acquire parameters for ll/sc and lse, since the full
 * barrier case is generated as release+dmb for the former and
 * acquire+release for the latter.
 */
<<<<<<< HEAD
#define __XCHG_CASE(w, sz, name, mb, nop_lse, acq, acq_lse, rel, cl)	\
static inline unsigned long __xchg_case_##name(unsigned long x,		\
					       volatile void *ptr)	\
{									\
	unsigned long ret, tmp;						\
									\
	asm volatile(ARM64_LSE_ATOMIC_INSN(				\
	/* LL/SC */							\
	"	prfm	pstl1strm, %2\n"				\
	"1:	ld" #acq "xr" #sz "\t%" #w "0, %2\n"			\
	"	st" #rel "xr" #sz "\t%w1, %" #w "3, %2\n"		\
	"	cbnz	%w1, 1b\n"					\
	"	" #mb,							\
	/* LSE atomics */						\
	"	swp" #acq_lse #rel #sz "\t%" #w "3, %" #w "0, %2\n"	\
		__nops(3)						\
	"	" #nop_lse)						\
	: "=&r" (ret), "=&r" (tmp), "+Q" (*(unsigned long *)ptr)	\
	: "r" (x)							\
	: cl);								\
									\
	return ret;							\
}

__XCHG_CASE(w, b,     1,        ,    ,  ,  ,  ,         )
__XCHG_CASE(w, h,     2,        ,    ,  ,  ,  ,         )
__XCHG_CASE(w,  ,     4,        ,    ,  ,  ,  ,         )
__XCHG_CASE( ,  ,     8,        ,    ,  ,  ,  ,         )
__XCHG_CASE(w, b, acq_1,        ,    , a, a,  , "memory")
__XCHG_CASE(w, h, acq_2,        ,    , a, a,  , "memory")
__XCHG_CASE(w,  , acq_4,        ,    , a, a,  , "memory")
__XCHG_CASE( ,  , acq_8,        ,    , a, a,  , "memory")
__XCHG_CASE(w, b, rel_1,        ,    ,  ,  , l, "memory")
__XCHG_CASE(w, h, rel_2,        ,    ,  ,  , l, "memory")
__XCHG_CASE(w,  , rel_4,        ,    ,  ,  , l, "memory")
__XCHG_CASE( ,  , rel_8,        ,    ,  ,  , l, "memory")
__XCHG_CASE(w, b,  mb_1, dmb ish, nop,  , a, l, "memory")
__XCHG_CASE(w, h,  mb_2, dmb ish, nop,  , a, l, "memory")
__XCHG_CASE(w,  ,  mb_4, dmb ish, nop,  , a, l, "memory")
__XCHG_CASE( ,  ,  mb_8, dmb ish, nop,  , a, l, "memory")
=======
#define __XCHG_CASE(w, sfx, name, sz, mb, nop_lse, acq, acq_lse, rel, cl)	\
static inline u##sz __xchg_case_##name##sz(u##sz x, volatile void *ptr)		\
{										\
	u##sz ret;								\
	unsigned long tmp;							\
										\
	asm volatile(ARM64_LSE_ATOMIC_INSN(					\
	/* LL/SC */								\
	"	prfm	pstl1strm, %2\n"					\
	"1:	ld" #acq "xr" #sfx "\t%" #w "0, %2\n"				\
	"	st" #rel "xr" #sfx "\t%w1, %" #w "3, %2\n"			\
	"	cbnz	%w1, 1b\n"						\
	"	" #mb,								\
	/* LSE atomics */							\
	"	swp" #acq_lse #rel #sfx "\t%" #w "3, %" #w "0, %2\n"		\
		__nops(3)							\
	"	" #nop_lse)							\
	: "=&r" (ret), "=&r" (tmp), "+Q" (*(u##sz *)ptr)			\
	: "r" (x)								\
	: cl);									\
										\
	return ret;								\
}

__XCHG_CASE(w, b,     ,  8,        ,    ,  ,  ,  ,         )
__XCHG_CASE(w, h,     , 16,        ,    ,  ,  ,  ,         )
__XCHG_CASE(w,  ,     , 32,        ,    ,  ,  ,  ,         )
__XCHG_CASE( ,  ,     , 64,        ,    ,  ,  ,  ,         )
__XCHG_CASE(w, b, acq_,  8,        ,    , a, a,  , "memory")
__XCHG_CASE(w, h, acq_, 16,        ,    , a, a,  , "memory")
__XCHG_CASE(w,  , acq_, 32,        ,    , a, a,  , "memory")
__XCHG_CASE( ,  , acq_, 64,        ,    , a, a,  , "memory")
__XCHG_CASE(w, b, rel_,  8,        ,    ,  ,  , l, "memory")
__XCHG_CASE(w, h, rel_, 16,        ,    ,  ,  , l, "memory")
__XCHG_CASE(w,  , rel_, 32,        ,    ,  ,  , l, "memory")
__XCHG_CASE( ,  , rel_, 64,        ,    ,  ,  , l, "memory")
__XCHG_CASE(w, b,  mb_,  8, dmb ish, nop,  , a, l, "memory")
__XCHG_CASE(w, h,  mb_, 16, dmb ish, nop,  , a, l, "memory")
__XCHG_CASE(w,  ,  mb_, 32, dmb ish, nop,  , a, l, "memory")
__XCHG_CASE( ,  ,  mb_, 64, dmb ish, nop,  , a, l, "memory")
>>>>>>> upstream/android-13

#undef __XCHG_CASE

#define __XCHG_GEN(sfx)							\
static __always_inline  unsigned long __xchg##sfx(unsigned long x,	\
					volatile void *ptr,		\
					int size)			\
{									\
	switch (size) {							\
	case 1:								\
<<<<<<< HEAD
		return __xchg_case##sfx##_1(x, ptr);			\
	case 2:								\
		return __xchg_case##sfx##_2(x, ptr);			\
	case 4:								\
		return __xchg_case##sfx##_4(x, ptr);			\
	case 8:								\
		return __xchg_case##sfx##_8(x, ptr);			\
=======
		return __xchg_case##sfx##_8(x, ptr);			\
	case 2:								\
		return __xchg_case##sfx##_16(x, ptr);			\
	case 4:								\
		return __xchg_case##sfx##_32(x, ptr);			\
	case 8:								\
		return __xchg_case##sfx##_64(x, ptr);			\
>>>>>>> upstream/android-13
	default:							\
		BUILD_BUG();						\
	}								\
									\
	unreachable();							\
}

__XCHG_GEN()
__XCHG_GEN(_acq)
__XCHG_GEN(_rel)
__XCHG_GEN(_mb)

#undef __XCHG_GEN

#define __xchg_wrapper(sfx, ptr, x)					\
({									\
	__typeof__(*(ptr)) __ret;					\
	__ret = (__typeof__(*(ptr)))					\
		__xchg##sfx((unsigned long)(x), (ptr), sizeof(*(ptr))); \
	__ret;								\
})

/* xchg */
<<<<<<< HEAD
#define xchg_relaxed(...)	__xchg_wrapper(    , __VA_ARGS__)
#define xchg_acquire(...)	__xchg_wrapper(_acq, __VA_ARGS__)
#define xchg_release(...)	__xchg_wrapper(_rel, __VA_ARGS__)
#define xchg(...)		__xchg_wrapper( _mb, __VA_ARGS__)
=======
#define arch_xchg_relaxed(...)	__xchg_wrapper(    , __VA_ARGS__)
#define arch_xchg_acquire(...)	__xchg_wrapper(_acq, __VA_ARGS__)
#define arch_xchg_release(...)	__xchg_wrapper(_rel, __VA_ARGS__)
#define arch_xchg(...)		__xchg_wrapper( _mb, __VA_ARGS__)

#define __CMPXCHG_CASE(name, sz)			\
static inline u##sz __cmpxchg_case_##name##sz(volatile void *ptr,	\
					      u##sz old,		\
					      u##sz new)		\
{									\
	return __lse_ll_sc_body(_cmpxchg_case_##name##sz,		\
				ptr, old, new);				\
}

__CMPXCHG_CASE(    ,  8)
__CMPXCHG_CASE(    , 16)
__CMPXCHG_CASE(    , 32)
__CMPXCHG_CASE(    , 64)
__CMPXCHG_CASE(acq_,  8)
__CMPXCHG_CASE(acq_, 16)
__CMPXCHG_CASE(acq_, 32)
__CMPXCHG_CASE(acq_, 64)
__CMPXCHG_CASE(rel_,  8)
__CMPXCHG_CASE(rel_, 16)
__CMPXCHG_CASE(rel_, 32)
__CMPXCHG_CASE(rel_, 64)
__CMPXCHG_CASE(mb_,  8)
__CMPXCHG_CASE(mb_, 16)
__CMPXCHG_CASE(mb_, 32)
__CMPXCHG_CASE(mb_, 64)

#undef __CMPXCHG_CASE

#define __CMPXCHG_DBL(name)						\
static inline long __cmpxchg_double##name(unsigned long old1,		\
					 unsigned long old2,		\
					 unsigned long new1,		\
					 unsigned long new2,		\
					 volatile void *ptr)		\
{									\
	return __lse_ll_sc_body(_cmpxchg_double##name, 			\
				old1, old2, new1, new2, ptr);		\
}

__CMPXCHG_DBL(   )
__CMPXCHG_DBL(_mb)

#undef __CMPXCHG_DBL
>>>>>>> upstream/android-13

#define __CMPXCHG_GEN(sfx)						\
static __always_inline unsigned long __cmpxchg##sfx(volatile void *ptr,	\
					   unsigned long old,		\
					   unsigned long new,		\
					   int size)			\
{									\
	switch (size) {							\
	case 1:								\
<<<<<<< HEAD
		return __cmpxchg_case##sfx##_1(ptr, (u8)old, new);	\
	case 2:								\
		return __cmpxchg_case##sfx##_2(ptr, (u16)old, new);	\
	case 4:								\
		return __cmpxchg_case##sfx##_4(ptr, old, new);		\
	case 8:								\
		return __cmpxchg_case##sfx##_8(ptr, old, new);		\
=======
		return __cmpxchg_case##sfx##_8(ptr, old, new);		\
	case 2:								\
		return __cmpxchg_case##sfx##_16(ptr, old, new);		\
	case 4:								\
		return __cmpxchg_case##sfx##_32(ptr, old, new);		\
	case 8:								\
		return __cmpxchg_case##sfx##_64(ptr, old, new);		\
>>>>>>> upstream/android-13
	default:							\
		BUILD_BUG();						\
	}								\
									\
	unreachable();							\
}

__CMPXCHG_GEN()
__CMPXCHG_GEN(_acq)
__CMPXCHG_GEN(_rel)
__CMPXCHG_GEN(_mb)

#undef __CMPXCHG_GEN

#define __cmpxchg_wrapper(sfx, ptr, o, n)				\
({									\
	__typeof__(*(ptr)) __ret;					\
	__ret = (__typeof__(*(ptr)))					\
		__cmpxchg##sfx((ptr), (unsigned long)(o),		\
				(unsigned long)(n), sizeof(*(ptr)));	\
	__ret;								\
})

/* cmpxchg */
<<<<<<< HEAD
#define cmpxchg_relaxed(...)	__cmpxchg_wrapper(    , __VA_ARGS__)
#define cmpxchg_acquire(...)	__cmpxchg_wrapper(_acq, __VA_ARGS__)
#define cmpxchg_release(...)	__cmpxchg_wrapper(_rel, __VA_ARGS__)
#define cmpxchg(...)		__cmpxchg_wrapper( _mb, __VA_ARGS__)
#define cmpxchg_local		cmpxchg_relaxed

/* cmpxchg64 */
#define cmpxchg64_relaxed	cmpxchg_relaxed
#define cmpxchg64_acquire	cmpxchg_acquire
#define cmpxchg64_release	cmpxchg_release
#define cmpxchg64		cmpxchg
#define cmpxchg64_local		cmpxchg_local
=======
#define arch_cmpxchg_relaxed(...)	__cmpxchg_wrapper(    , __VA_ARGS__)
#define arch_cmpxchg_acquire(...)	__cmpxchg_wrapper(_acq, __VA_ARGS__)
#define arch_cmpxchg_release(...)	__cmpxchg_wrapper(_rel, __VA_ARGS__)
#define arch_cmpxchg(...)		__cmpxchg_wrapper( _mb, __VA_ARGS__)
#define arch_cmpxchg_local		arch_cmpxchg_relaxed

/* cmpxchg64 */
#define arch_cmpxchg64_relaxed		arch_cmpxchg_relaxed
#define arch_cmpxchg64_acquire		arch_cmpxchg_acquire
#define arch_cmpxchg64_release		arch_cmpxchg_release
#define arch_cmpxchg64			arch_cmpxchg
#define arch_cmpxchg64_local		arch_cmpxchg_local
>>>>>>> upstream/android-13

/* cmpxchg_double */
#define system_has_cmpxchg_double()     1

#define __cmpxchg_double_check(ptr1, ptr2)					\
({										\
	if (sizeof(*(ptr1)) != 8)						\
		BUILD_BUG();							\
	VM_BUG_ON((unsigned long *)(ptr2) - (unsigned long *)(ptr1) != 1);	\
})

<<<<<<< HEAD
#define cmpxchg_double(ptr1, ptr2, o1, o2, n1, n2) \
({\
	int __ret;\
	__cmpxchg_double_check(ptr1, ptr2); \
	__ret = !__cmpxchg_double_mb((unsigned long)(o1), (unsigned long)(o2), \
				     (unsigned long)(n1), (unsigned long)(n2), \
				     ptr1); \
	__ret; \
})

#define cmpxchg_double_local(ptr1, ptr2, o1, o2, n1, n2) \
({\
	int __ret;\
	__cmpxchg_double_check(ptr1, ptr2); \
	__ret = !__cmpxchg_double((unsigned long)(o1), (unsigned long)(o2), \
				  (unsigned long)(n1), (unsigned long)(n2), \
				  ptr1); \
	__ret; \
})

#define __CMPWAIT_CASE(w, sz, name)					\
static inline void __cmpwait_case_##name(volatile void *ptr,		\
					 unsigned long val)		\
=======
#define arch_cmpxchg_double(ptr1, ptr2, o1, o2, n1, n2)				\
({										\
	int __ret;								\
	__cmpxchg_double_check(ptr1, ptr2);					\
	__ret = !__cmpxchg_double_mb((unsigned long)(o1), (unsigned long)(o2),	\
				     (unsigned long)(n1), (unsigned long)(n2),	\
				     ptr1);					\
	__ret;									\
})

#define arch_cmpxchg_double_local(ptr1, ptr2, o1, o2, n1, n2)			\
({										\
	int __ret;								\
	__cmpxchg_double_check(ptr1, ptr2);					\
	__ret = !__cmpxchg_double((unsigned long)(o1), (unsigned long)(o2),	\
				  (unsigned long)(n1), (unsigned long)(n2),	\
				  ptr1);					\
	__ret;									\
})

#define __CMPWAIT_CASE(w, sfx, sz)					\
static inline void __cmpwait_case_##sz(volatile void *ptr,		\
				       unsigned long val)		\
>>>>>>> upstream/android-13
{									\
	unsigned long tmp;						\
									\
	asm volatile(							\
	"	sevl\n"							\
	"	wfe\n"							\
<<<<<<< HEAD
	"	ldxr" #sz "\t%" #w "[tmp], %[v]\n"			\
=======
	"	ldxr" #sfx "\t%" #w "[tmp], %[v]\n"			\
>>>>>>> upstream/android-13
	"	eor	%" #w "[tmp], %" #w "[tmp], %" #w "[val]\n"	\
	"	cbnz	%" #w "[tmp], 1f\n"				\
	"	wfe\n"							\
	"1:"								\
	: [tmp] "=&r" (tmp), [v] "+Q" (*(unsigned long *)ptr)		\
	: [val] "r" (val));						\
}

<<<<<<< HEAD
__CMPWAIT_CASE(w, b, 1);
__CMPWAIT_CASE(w, h, 2);
__CMPWAIT_CASE(w,  , 4);
__CMPWAIT_CASE( ,  , 8);
=======
__CMPWAIT_CASE(w, b, 8);
__CMPWAIT_CASE(w, h, 16);
__CMPWAIT_CASE(w,  , 32);
__CMPWAIT_CASE( ,  , 64);
>>>>>>> upstream/android-13

#undef __CMPWAIT_CASE

#define __CMPWAIT_GEN(sfx)						\
static __always_inline void __cmpwait##sfx(volatile void *ptr,		\
				  unsigned long val,			\
				  int size)				\
{									\
	switch (size) {							\
	case 1:								\
<<<<<<< HEAD
		return __cmpwait_case##sfx##_1(ptr, (u8)val);		\
	case 2:								\
		return __cmpwait_case##sfx##_2(ptr, (u16)val);		\
	case 4:								\
		return __cmpwait_case##sfx##_4(ptr, val);		\
	case 8:								\
		return __cmpwait_case##sfx##_8(ptr, val);		\
=======
		return __cmpwait_case##sfx##_8(ptr, (u8)val);		\
	case 2:								\
		return __cmpwait_case##sfx##_16(ptr, (u16)val);		\
	case 4:								\
		return __cmpwait_case##sfx##_32(ptr, val);		\
	case 8:								\
		return __cmpwait_case##sfx##_64(ptr, val);		\
>>>>>>> upstream/android-13
	default:							\
		BUILD_BUG();						\
	}								\
									\
	unreachable();							\
}

__CMPWAIT_GEN()

#undef __CMPWAIT_GEN

#define __cmpwait_relaxed(ptr, val) \
	__cmpwait((ptr), (unsigned long)(val), sizeof(*(ptr)))

#endif	/* __ASM_CMPXCHG_H */
