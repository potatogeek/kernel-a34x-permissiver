/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Generic UP xchg and cmpxchg using interrupt disablement.  Does not
 * support SMP.
 */

#ifndef __ASM_GENERIC_CMPXCHG_H
#define __ASM_GENERIC_CMPXCHG_H

#ifdef CONFIG_SMP
#error "Cannot use generic cmpxchg on SMP"
#endif

#include <linux/types.h>
#include <linux/irqflags.h>

<<<<<<< HEAD
#ifndef xchg

=======
>>>>>>> upstream/android-13
/*
 * This function doesn't exist, so you'll get a linker error if
 * something tries to do an invalidly-sized xchg().
 */
<<<<<<< HEAD
extern void __xchg_called_with_bad_pointer(void);

static inline
unsigned long __xchg(unsigned long x, volatile void *ptr, int size)
=======
extern void __generic_xchg_called_with_bad_pointer(void);

static inline
unsigned long __generic_xchg(unsigned long x, volatile void *ptr, int size)
>>>>>>> upstream/android-13
{
	unsigned long ret, flags;

	switch (size) {
	case 1:
#ifdef __xchg_u8
		return __xchg_u8(x, ptr);
#else
		local_irq_save(flags);
		ret = *(volatile u8 *)ptr;
		*(volatile u8 *)ptr = x;
		local_irq_restore(flags);
		return ret;
#endif /* __xchg_u8 */

	case 2:
#ifdef __xchg_u16
		return __xchg_u16(x, ptr);
#else
		local_irq_save(flags);
		ret = *(volatile u16 *)ptr;
		*(volatile u16 *)ptr = x;
		local_irq_restore(flags);
		return ret;
#endif /* __xchg_u16 */

	case 4:
#ifdef __xchg_u32
		return __xchg_u32(x, ptr);
#else
		local_irq_save(flags);
		ret = *(volatile u32 *)ptr;
		*(volatile u32 *)ptr = x;
		local_irq_restore(flags);
		return ret;
#endif /* __xchg_u32 */

#ifdef CONFIG_64BIT
	case 8:
#ifdef __xchg_u64
		return __xchg_u64(x, ptr);
#else
		local_irq_save(flags);
		ret = *(volatile u64 *)ptr;
		*(volatile u64 *)ptr = x;
		local_irq_restore(flags);
		return ret;
#endif /* __xchg_u64 */
#endif /* CONFIG_64BIT */

	default:
<<<<<<< HEAD
		__xchg_called_with_bad_pointer();
=======
		__generic_xchg_called_with_bad_pointer();
>>>>>>> upstream/android-13
		return x;
	}
}

<<<<<<< HEAD
#define xchg(ptr, x) ({							\
	((__typeof__(*(ptr)))						\
		__xchg((unsigned long)(x), (ptr), sizeof(*(ptr))));	\
})

#endif /* xchg */

=======
#define generic_xchg(ptr, x) ({							\
	((__typeof__(*(ptr)))							\
		__generic_xchg((unsigned long)(x), (ptr), sizeof(*(ptr))));	\
})

>>>>>>> upstream/android-13
/*
 * Atomic compare and exchange.
 */
#include <asm-generic/cmpxchg-local.h>

<<<<<<< HEAD
#ifndef cmpxchg_local
#define cmpxchg_local(ptr, o, n) ({					       \
	((__typeof__(*(ptr)))__cmpxchg_local_generic((ptr), (unsigned long)(o),\
			(unsigned long)(n), sizeof(*(ptr))));		       \
})
#endif

#ifndef cmpxchg64_local
#define cmpxchg64_local(ptr, o, n) __cmpxchg64_local_generic((ptr), (o), (n))
#endif

#define cmpxchg(ptr, o, n)	cmpxchg_local((ptr), (o), (n))
#define cmpxchg64(ptr, o, n)	cmpxchg64_local((ptr), (o), (n))
=======
#define generic_cmpxchg_local(ptr, o, n) ({					\
	((__typeof__(*(ptr)))__generic_cmpxchg_local((ptr), (unsigned long)(o),	\
			(unsigned long)(n), sizeof(*(ptr))));			\
})

#define generic_cmpxchg64_local(ptr, o, n) \
	__generic_cmpxchg64_local((ptr), (o), (n))


#ifndef arch_xchg
#define arch_xchg		generic_xchg
#endif

#ifndef arch_cmpxchg_local
#define arch_cmpxchg_local	generic_cmpxchg_local
#endif

#ifndef arch_cmpxchg64_local
#define arch_cmpxchg64_local	generic_cmpxchg64_local
#endif

#define arch_cmpxchg		arch_cmpxchg_local
#define arch_cmpxchg64		arch_cmpxchg64_local
>>>>>>> upstream/android-13

#endif /* __ASM_GENERIC_CMPXCHG_H */
