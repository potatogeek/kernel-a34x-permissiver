/* SPDX-License-Identifier: GPL-2.0 WITH Linux-syscall-note */
#ifndef _UAPI_LINUX_SWAB_H
#define _UAPI_LINUX_SWAB_H

#include <linux/types.h>
#include <linux/compiler.h>
#include <asm/bitsperlong.h>
#include <asm/swab.h>


#define ___constant_swab16(x) ((__u16)(				\
	(((__u16)(x) & (__u16)0x00ffU) << 8) |			\
	(((__u16)(x) & (__u16)0xff00U) >> 8)))

#define ___constant_swab32(x) ((__u32)(				\
	(((__u32)(x) & (__u32)0x000000ffUL) << 24) |		\
	(((__u32)(x) & (__u32)0x0000ff00UL) <<  8) |		\
	(((__u32)(x) & (__u32)0x00ff0000UL) >>  8) |		\
	(((__u32)(x) & (__u32)0xff000000UL) >> 24)))

#define ___constant_swab64(x) ((__u64)(				\
	(((__u64)(x) & (__u64)0x00000000000000ffULL) << 56) |	\
	(((__u64)(x) & (__u64)0x000000000000ff00ULL) << 40) |	\
	(((__u64)(x) & (__u64)0x0000000000ff0000ULL) << 24) |	\
	(((__u64)(x) & (__u64)0x00000000ff000000ULL) <<  8) |	\
	(((__u64)(x) & (__u64)0x000000ff00000000ULL) >>  8) |	\
	(((__u64)(x) & (__u64)0x0000ff0000000000ULL) >> 24) |	\
	(((__u64)(x) & (__u64)0x00ff000000000000ULL) >> 40) |	\
	(((__u64)(x) & (__u64)0xff00000000000000ULL) >> 56)))

#define ___constant_swahw32(x) ((__u32)(			\
	(((__u32)(x) & (__u32)0x0000ffffUL) << 16) |		\
	(((__u32)(x) & (__u32)0xffff0000UL) >> 16)))

#define ___constant_swahb32(x) ((__u32)(			\
	(((__u32)(x) & (__u32)0x00ff00ffUL) << 8) |		\
	(((__u32)(x) & (__u32)0xff00ff00UL) >> 8)))



static inline __attribute_const__ __u16 __fswab16(__u16 val)
{
#if defined (__arch_swab16)
	return __arch_swab16(val);
#else
	return ___constant_swab16(val);
#endif
}

static inline __attribute_const__ __u32 __fswab32(__u32 val)
{
#if defined(__arch_swab32)
	return __arch_swab32(val);
#else
	return ___constant_swab32(val);
#endif
}

static inline __attribute_const__ __u64 __fswab64(__u64 val)
{
#if defined (__arch_swab64)
	return __arch_swab64(val);
#elif defined(__SWAB_64_THRU_32__)
	__u32 h = val >> 32;
	__u32 l = val & ((1ULL << 32) - 1);
	return (((__u64)__fswab32(l)) << 32) | ((__u64)(__fswab32(h)));
#else
	return ___constant_swab64(val);
#endif
}

static inline __attribute_const__ __u32 __fswahw32(__u32 val)
{
#ifdef __arch_swahw32
	return __arch_swahw32(val);
#else
	return ___constant_swahw32(val);
#endif
}

static inline __attribute_const__ __u32 __fswahb32(__u32 val)
{
#ifdef __arch_swahb32
	return __arch_swahb32(val);
#else
	return ___constant_swahb32(val);
#endif
}


#ifdef __HAVE_BUILTIN_BSWAP16__
#define __swab16(x) (__u16)__builtin_bswap16((__u16)(x))
#else
#define __swab16(x)				\
	(__builtin_constant_p((__u16)(x)) ?	\
	___constant_swab16(x) :			\
	__fswab16(x))
#endif


#ifdef __HAVE_BUILTIN_BSWAP32__
#define __swab32(x) (__u32)__builtin_bswap32((__u32)(x))
#else
#define __swab32(x)				\
	(__builtin_constant_p((__u32)(x)) ?	\
	___constant_swab32(x) :			\
	__fswab32(x))
#endif


#ifdef __HAVE_BUILTIN_BSWAP64__
#define __swab64(x) (__u64)__builtin_bswap64((__u64)(x))
#else
#define __swab64(x)				\
	(__builtin_constant_p((__u64)(x)) ?	\
	___constant_swab64(x) :			\
	__fswab64(x))
#endif

static __always_inline unsigned long __swab(const unsigned long y)
{
#if __BITS_PER_LONG == 64
	return __swab64(y);
#else 
	return __swab32(y);
#endif
}


#define __swahw32(x)				\
	(__builtin_constant_p((__u32)(x)) ?	\
	___constant_swahw32(x) :		\
	__fswahw32(x))


#define __swahb32(x)				\
	(__builtin_constant_p((__u32)(x)) ?	\
	___constant_swahb32(x) :		\
	__fswahb32(x))


static __always_inline __u16 __swab16p(const __u16 *p)
{
#ifdef __arch_swab16p
	return __arch_swab16p(p);
#else
	return __swab16(*p);
#endif
}


static __always_inline __u32 __swab32p(const __u32 *p)
{
#ifdef __arch_swab32p
	return __arch_swab32p(p);
#else
	return __swab32(*p);
#endif
}


static __always_inline __u64 __swab64p(const __u64 *p)
{
#ifdef __arch_swab64p
	return __arch_swab64p(p);
#else
	return __swab64(*p);
#endif
}


static inline __u32 __swahw32p(const __u32 *p)
{
#ifdef __arch_swahw32p
	return __arch_swahw32p(p);
#else
	return __swahw32(*p);
#endif
}


static inline __u32 __swahb32p(const __u32 *p)
{
#ifdef __arch_swahb32p
	return __arch_swahb32p(p);
#else
	return __swahb32(*p);
#endif
}


static inline void __swab16s(__u16 *p)
{
#ifdef __arch_swab16s
	__arch_swab16s(p);
#else
	*p = __swab16p(p);
#endif
}

static __always_inline void __swab32s(__u32 *p)
{
#ifdef __arch_swab32s
	__arch_swab32s(p);
#else
	*p = __swab32p(p);
#endif
}


static __always_inline void __swab64s(__u64 *p)
{
#ifdef __arch_swab64s
	__arch_swab64s(p);
#else
	*p = __swab64p(p);
#endif
}


static inline void __swahw32s(__u32 *p)
{
#ifdef __arch_swahw32s
	__arch_swahw32s(p);
#else
	*p = __swahw32p(p);
#endif
}


static inline void __swahb32s(__u32 *p)
{
#ifdef __arch_swahb32s
	__arch_swahb32s(p);
#else
	*p = __swahb32p(p);
#endif
}


#endif 
