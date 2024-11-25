/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _ASM_X86_UACCESS_H
#define _ASM_X86_UACCESS_H
/*
 * User space memory access functions
 */
#include <linux/compiler.h>
#include <linux/kasan-checks.h>
#include <linux/string.h>
#include <asm/asm.h>
#include <asm/page.h>
#include <asm/smap.h>
#include <asm/extable.h>

/*
<<<<<<< HEAD
 * The fs value determines whether argument validity checking should be
 * performed or not.  If get_fs() == USER_DS, checking is performed, with
 * get_fs() == KERNEL_DS, checking is bypassed.
 *
 * For historical reasons, these macros are grossly misnamed.
 */

#define MAKE_MM_SEG(s)	((mm_segment_t) { (s) })

#define KERNEL_DS	MAKE_MM_SEG(-1UL)
#define USER_DS 	MAKE_MM_SEG(TASK_SIZE_MAX)

#define get_ds()	(KERNEL_DS)
#define get_fs()	(current->thread.addr_limit)
static inline void set_fs(mm_segment_t fs)
{
	current->thread.addr_limit = fs;
	/* On user-mode return, check fs is correct */
	set_thread_flag(TIF_FSCHECK);
}

#define segment_eq(a, b)	((a).seg == (b).seg)

#define user_addr_max() (current->thread.addr_limit.seg)
#define __addr_ok(addr) 	\
	((unsigned long __force)(addr) < user_addr_max())

/*
=======
>>>>>>> upstream/android-13
 * Test whether a block of memory is a valid user space address.
 * Returns 0 if the range is valid, nonzero otherwise.
 */
static inline bool __chk_range_not_ok(unsigned long addr, unsigned long size, unsigned long limit)
{
	/*
	 * If we have used "sizeof()" for the size,
	 * we know it won't overflow the limit (but
	 * it might overflow the 'addr', so it's
	 * important to subtract the size from the
	 * limit, not add it to the address).
	 */
	if (__builtin_constant_p(size))
		return unlikely(addr > limit - size);

	/* Arbitrary sizes? Be careful about overflow */
	addr += size;
	if (unlikely(addr < size))
		return true;
	return unlikely(addr > limit);
}

#define __range_not_ok(addr, size, limit)				\
({									\
	__chk_user_ptr(addr);						\
	__chk_range_not_ok((unsigned long __force)(addr), size, limit); \
})

#ifdef CONFIG_DEBUG_ATOMIC_SLEEP
<<<<<<< HEAD
# define WARN_ON_IN_IRQ()	WARN_ON_ONCE(!in_task())
=======
static inline bool pagefault_disabled(void);
# define WARN_ON_IN_IRQ()	\
	WARN_ON_ONCE(!in_task() && !pagefault_disabled())
>>>>>>> upstream/android-13
#else
# define WARN_ON_IN_IRQ()
#endif

/**
<<<<<<< HEAD
 * access_ok: - Checks if a user space pointer is valid
 * @type: Type of access: %VERIFY_READ or %VERIFY_WRITE.  Note that
 *        %VERIFY_WRITE is a superset of %VERIFY_READ - if it is safe
 *        to write to a block, it is always safe to read from it.
=======
 * access_ok - Checks if a user space pointer is valid
>>>>>>> upstream/android-13
 * @addr: User space pointer to start of block to check
 * @size: Size of block to check
 *
 * Context: User context only. This function may sleep if pagefaults are
 *          enabled.
 *
 * Checks if a pointer to a block of memory in user space is valid.
 *
<<<<<<< HEAD
 * Returns true (nonzero) if the memory block may be valid, false (zero)
 * if it is definitely invalid.
 *
 * Note that, depending on architecture, this function probably just
 * checks that the pointer is in the user space range - after calling
 * this function, memory access functions may still return -EFAULT.
 */
#define access_ok(type, addr, size)					\
({									\
	WARN_ON_IN_IRQ();						\
	likely(!__range_not_ok(addr, size, user_addr_max()));		\
})

/*
 * These are the main single-value transfer routines.  They automatically
 * use the right size if we just have the right pointer type.
 *
 * This gets kind of ugly. We want to return _two_ values in "get_user()"
 * and yet we don't want to do any pointers, because that is too much
 * of a performance impact. Thus we have a few rather ugly macros here,
 * and hide all the ugliness from the user.
 *
 * The "__xxx" versions of the user access functions are versions that
 * do not verify the address space, that must have been done previously
 * with a separate "access_ok()" call (this is used when we do multiple
 * accesses to the same area of user memory).
 */

=======
 * Note that, depending on architecture, this function probably just
 * checks that the pointer is in the user space range - after calling
 * this function, memory access functions may still return -EFAULT.
 *
 * Return: true (nonzero) if the memory block may be valid, false (zero)
 * if it is definitely invalid.
 */
#define access_ok(addr, size)					\
({									\
	WARN_ON_IN_IRQ();						\
	likely(!__range_not_ok(addr, size, TASK_SIZE_MAX));		\
})

>>>>>>> upstream/android-13
extern int __get_user_1(void);
extern int __get_user_2(void);
extern int __get_user_4(void);
extern int __get_user_8(void);
<<<<<<< HEAD
=======
extern int __get_user_nocheck_1(void);
extern int __get_user_nocheck_2(void);
extern int __get_user_nocheck_4(void);
extern int __get_user_nocheck_8(void);
>>>>>>> upstream/android-13
extern int __get_user_bad(void);

#define __uaccess_begin() stac()
#define __uaccess_end()   clac()
#define __uaccess_begin_nospec()	\
({					\
	stac();				\
	barrier_nospec();		\
})

/*
<<<<<<< HEAD
 * This is a type: either unsigned long, if the argument fits into
 * that type, or otherwise unsigned long long.
 */
#define __inttype(x) \
__typeof__(__builtin_choose_expr(sizeof(x) > sizeof(0UL), 0ULL, 0UL))

/**
 * get_user: - Get a simple variable from user space.
 * @x:   Variable to store result.
 * @ptr: Source address, in user space.
 *
 * Context: User context only. This function may sleep if pagefaults are
 *          enabled.
 *
 * This macro copies a single simple variable from user space to kernel
 * space.  It supports simple types like char and int, but not larger
 * data types like structures or arrays.
 *
 * @ptr must have pointer-to-simple-variable type, and the result of
 * dereferencing @ptr must be assignable to @x without a cast.
 *
 * Returns zero on success, or -EFAULT on error.
 * On error, the variable @x is set to zero.
 */
/*
=======
 * This is the smallest unsigned integer type that can fit a value
 * (up to 'long long')
 */
#define __inttype(x) __typeof__(		\
	__typefits(x,char,			\
	  __typefits(x,short,			\
	    __typefits(x,int,			\
	      __typefits(x,long,0ULL)))))

#define __typefits(x,type,not) \
	__builtin_choose_expr(sizeof(x)<=sizeof(type),(unsigned type)0,not)

/*
 * This is used for both get_user() and __get_user() to expand to
 * the proper special function call that has odd calling conventions
 * due to returning both a value and an error, and that depends on
 * the size of the pointer passed in.
 *
>>>>>>> upstream/android-13
 * Careful: we have to cast the result to the type of the pointer
 * for sign reasons.
 *
 * The use of _ASM_DX as the register specifier is a bit of a
 * simplification, as gcc only cares about it as the starting point
 * and not size: for a 64-bit value it will use %ecx:%edx on 32 bits
 * (%ecx being the next register in gcc's x86 register sequence), and
 * %rdx on 64 bits.
 *
 * Clang/LLVM cares about the size of the register, but still wants
 * the base register for something that ends up being a pair.
 */
<<<<<<< HEAD
#define get_user(x, ptr)						\
=======
#define do_get_user_call(fn,x,ptr)					\
>>>>>>> upstream/android-13
({									\
	int __ret_gu;							\
	register __inttype(*(ptr)) __val_gu asm("%"_ASM_DX);		\
	__chk_user_ptr(ptr);						\
<<<<<<< HEAD
	might_fault();							\
	asm volatile("call __get_user_%P4"				\
=======
	asm volatile("call __" #fn "_%P4"				\
>>>>>>> upstream/android-13
		     : "=a" (__ret_gu), "=r" (__val_gu),		\
			ASM_CALL_CONSTRAINT				\
		     : "0" (ptr), "i" (sizeof(*(ptr))));		\
	(x) = (__force __typeof__(*(ptr))) __val_gu;			\
	__builtin_expect(__ret_gu, 0);					\
})

<<<<<<< HEAD
#define __put_user_x(size, x, ptr, __ret_pu)			\
	asm volatile("call __put_user_" #size : "=a" (__ret_pu)	\
		     : "0" ((typeof(*(ptr)))(x)), "c" (ptr) : "ebx")



#ifdef CONFIG_X86_32
#define __put_user_asm_u64(x, addr, err, errret)			\
	asm volatile("\n"						\
		     "1:	movl %%eax,0(%2)\n"			\
		     "2:	movl %%edx,4(%2)\n"			\
		     "3:"						\
		     ".section .fixup,\"ax\"\n"				\
		     "4:	movl %3,%0\n"				\
		     "	jmp 3b\n"					\
		     ".previous\n"					\
		     _ASM_EXTABLE(1b, 4b)				\
		     _ASM_EXTABLE(2b, 4b)				\
		     : "=r" (err)					\
		     : "A" (x), "r" (addr), "i" (errret), "0" (err))

#define __put_user_asm_ex_u64(x, addr)					\
	asm volatile("\n"						\
		     "1:	movl %%eax,0(%1)\n"			\
		     "2:	movl %%edx,4(%1)\n"			\
		     "3:"						\
		     _ASM_EXTABLE_EX(1b, 2b)				\
		     _ASM_EXTABLE_EX(2b, 3b)				\
		     : : "A" (x), "r" (addr))

#define __put_user_x8(x, ptr, __ret_pu)				\
	asm volatile("call __put_user_8" : "=a" (__ret_pu)	\
		     : "A" ((typeof(*(ptr)))(x)), "c" (ptr) : "ebx")
#else
#define __put_user_asm_u64(x, ptr, retval, errret) \
	__put_user_asm(x, ptr, retval, "q", "", "er", errret)
#define __put_user_asm_ex_u64(x, addr)	\
	__put_user_asm_ex(x, addr, "q", "", "er")
#define __put_user_x8(x, ptr, __ret_pu) __put_user_x(8, x, ptr, __ret_pu)
#endif

extern void __put_user_bad(void);

/*
 * Strange magic calling convention: pointer in %ecx,
 * value in %eax(:%edx), return value in %eax. clobbers %rbx
 */
extern void __put_user_1(void);
extern void __put_user_2(void);
extern void __put_user_4(void);
extern void __put_user_8(void);

/**
 * put_user: - Write a simple value into user space.
 * @x:   Value to copy to user space.
 * @ptr: Destination address, in user space.
=======
/**
 * get_user - Get a simple variable from user space.
 * @x:   Variable to store result.
 * @ptr: Source address, in user space.
>>>>>>> upstream/android-13
 *
 * Context: User context only. This function may sleep if pagefaults are
 *          enabled.
 *
<<<<<<< HEAD
 * This macro copies a single simple value from kernel space to user
 * space.  It supports simple types like char and int, but not larger
 * data types like structures or arrays.
 *
 * @ptr must have pointer-to-simple-variable type, and @x must be assignable
 * to the result of dereferencing @ptr.
 *
 * Returns zero on success, or -EFAULT on error.
 */
#define put_user(x, ptr)					\
({								\
	int __ret_pu;						\
	__typeof__(*(ptr)) __pu_val;				\
	__chk_user_ptr(ptr);					\
	might_fault();						\
	__pu_val = x;						\
	switch (sizeof(*(ptr))) {				\
	case 1:							\
		__put_user_x(1, __pu_val, ptr, __ret_pu);	\
		break;						\
	case 2:							\
		__put_user_x(2, __pu_val, ptr, __ret_pu);	\
		break;						\
	case 4:							\
		__put_user_x(4, __pu_val, ptr, __ret_pu);	\
		break;						\
	case 8:							\
		__put_user_x8(__pu_val, ptr, __ret_pu);		\
		break;						\
	default:						\
		__put_user_x(X, __pu_val, ptr, __ret_pu);	\
		break;						\
	}							\
	__builtin_expect(__ret_pu, 0);				\
})

#define __put_user_size(x, ptr, size, retval, errret)			\
do {									\
	retval = 0;							\
	__chk_user_ptr(ptr);						\
	switch (size) {							\
	case 1:								\
		__put_user_asm(x, ptr, retval, "b", "b", "iq", errret);	\
		break;							\
	case 2:								\
		__put_user_asm(x, ptr, retval, "w", "w", "ir", errret);	\
		break;							\
	case 4:								\
		__put_user_asm(x, ptr, retval, "l", "k", "ir", errret);	\
		break;							\
	case 8:								\
		__put_user_asm_u64(x, ptr, retval, errret);		\
		break;							\
	default:							\
		__put_user_bad();					\
	}								\
} while (0)

/*
 * This doesn't do __uaccess_begin/end - the exception handling
 * around it must do that.
 */
#define __put_user_size_ex(x, ptr, size)				\
do {									\
	__chk_user_ptr(ptr);						\
	switch (size) {							\
	case 1:								\
		__put_user_asm_ex(x, ptr, "b", "b", "iq");		\
		break;							\
	case 2:								\
		__put_user_asm_ex(x, ptr, "w", "w", "ir");		\
		break;							\
	case 4:								\
		__put_user_asm_ex(x, ptr, "l", "k", "ir");		\
		break;							\
	case 8:								\
		__put_user_asm_ex_u64((__typeof__(*ptr))(x), ptr);	\
		break;							\
	default:							\
		__put_user_bad();					\
	}								\
} while (0)

#ifdef CONFIG_X86_32
#define __get_user_asm_u64(x, ptr, retval, errret)			\
({									\
	__typeof__(ptr) __ptr = (ptr);					\
	asm volatile("\n"					\
		     "1:	movl %2,%%eax\n"			\
		     "2:	movl %3,%%edx\n"			\
		     "3:\n"				\
		     ".section .fixup,\"ax\"\n"				\
		     "4:	mov %4,%0\n"				\
		     "	xorl %%eax,%%eax\n"				\
		     "	xorl %%edx,%%edx\n"				\
		     "	jmp 3b\n"					\
		     ".previous\n"					\
		     _ASM_EXTABLE(1b, 4b)				\
		     _ASM_EXTABLE(2b, 4b)				\
		     : "=r" (retval), "=&A"(x)				\
		     : "m" (__m(__ptr)), "m" __m(((u32 __user *)(__ptr)) + 1),	\
		       "i" (errret), "0" (retval));			\
})

#define __get_user_asm_ex_u64(x, ptr)			(x) = __get_user_bad()
#else
#define __get_user_asm_u64(x, ptr, retval, errret) \
	 __get_user_asm(x, ptr, retval, "q", "", "=r", errret)
#define __get_user_asm_ex_u64(x, ptr) \
	 __get_user_asm_ex(x, ptr, "q", "", "=r")
#endif

#define __get_user_size(x, ptr, size, retval, errret)			\
do {									\
	retval = 0;							\
	__chk_user_ptr(ptr);						\
	switch (size) {							\
	case 1:								\
		__get_user_asm(x, ptr, retval, "b", "b", "=q", errret);	\
		break;							\
	case 2:								\
		__get_user_asm(x, ptr, retval, "w", "w", "=r", errret);	\
		break;							\
	case 4:								\
		__get_user_asm(x, ptr, retval, "l", "k", "=r", errret);	\
		break;							\
	case 8:								\
		__get_user_asm_u64(x, ptr, retval, errret);		\
		break;							\
	default:							\
		(x) = __get_user_bad();					\
	}								\
} while (0)

#define __get_user_asm(x, addr, err, itype, rtype, ltype, errret)	\
	asm volatile("\n"						\
		     "1:	mov"itype" %2,%"rtype"1\n"		\
		     "2:\n"						\
		     ".section .fixup,\"ax\"\n"				\
		     "3:	mov %3,%0\n"				\
		     "	xor"itype" %"rtype"1,%"rtype"1\n"		\
		     "	jmp 2b\n"					\
		     ".previous\n"					\
		     _ASM_EXTABLE(1b, 3b)				\
		     : "=r" (err), ltype(x)				\
		     : "m" (__m(addr)), "i" (errret), "0" (err))

#define __get_user_asm_nozero(x, addr, err, itype, rtype, ltype, errret)	\
	asm volatile("\n"						\
		     "1:	mov"itype" %2,%"rtype"1\n"		\
		     "2:\n"						\
		     ".section .fixup,\"ax\"\n"				\
		     "3:	mov %3,%0\n"				\
		     "	jmp 2b\n"					\
		     ".previous\n"					\
		     _ASM_EXTABLE(1b, 3b)				\
		     : "=r" (err), ltype(x)				\
		     : "m" (__m(addr)), "i" (errret), "0" (err))

/*
 * This doesn't do __uaccess_begin/end - the exception handling
 * around it must do that.
 */
#define __get_user_size_ex(x, ptr, size)				\
do {									\
	__chk_user_ptr(ptr);						\
	switch (size) {							\
	case 1:								\
		__get_user_asm_ex(x, ptr, "b", "b", "=q");		\
		break;							\
	case 2:								\
		__get_user_asm_ex(x, ptr, "w", "w", "=r");		\
		break;							\
	case 4:								\
		__get_user_asm_ex(x, ptr, "l", "k", "=r");		\
		break;							\
	case 8:								\
		__get_user_asm_ex_u64(x, ptr);				\
		break;							\
	default:							\
		(x) = __get_user_bad();					\
	}								\
} while (0)

#define __get_user_asm_ex(x, addr, itype, rtype, ltype)			\
	asm volatile("1:	mov"itype" %1,%"rtype"0\n"		\
		     "2:\n"						\
		     ".section .fixup,\"ax\"\n"				\
                     "3:xor"itype" %"rtype"0,%"rtype"0\n"		\
		     "  jmp 2b\n"					\
		     ".previous\n"					\
		     _ASM_EXTABLE_EX(1b, 3b)				\
		     : ltype(x) : "m" (__m(addr)))

#define __put_user_nocheck(x, ptr, size)			\
({								\
	int __pu_err;						\
	__typeof__(*(ptr)) __pu_val;				\
	__pu_val = x;						\
	__uaccess_begin();					\
	__put_user_size(__pu_val, (ptr), (size), __pu_err, -EFAULT);\
	__uaccess_end();					\
	__builtin_expect(__pu_err, 0);				\
})

#define __get_user_nocheck(x, ptr, size)				\
({									\
	int __gu_err;							\
	__inttype(*(ptr)) __gu_val;					\
	__typeof__(ptr) __gu_ptr = (ptr);				\
	__typeof__(size) __gu_size = (size);				\
	__uaccess_begin_nospec();					\
	__get_user_size(__gu_val, __gu_ptr, __gu_size, __gu_err, -EFAULT);	\
	__uaccess_end();						\
	(x) = (__force __typeof__(*(ptr)))__gu_val;			\
	__builtin_expect(__gu_err, 0);					\
})

/* FIXME: this hack is definitely wrong -AK */
struct __large_struct { unsigned long buf[100]; };
#define __m(x) (*(struct __large_struct __user *)(x))

/*
 * Tell gcc we read from memory instead of writing: this is because
 * we do not write to any memory gcc knows about, so there are no
 * aliasing issues.
 */
#define __put_user_asm(x, addr, err, itype, rtype, ltype, errret)	\
	asm volatile("\n"						\
		     "1:	mov"itype" %"rtype"1,%2\n"		\
		     "2:\n"						\
		     ".section .fixup,\"ax\"\n"				\
		     "3:	mov %3,%0\n"				\
		     "	jmp 2b\n"					\
		     ".previous\n"					\
		     _ASM_EXTABLE(1b, 3b)				\
		     : "=r"(err)					\
		     : ltype(x), "m" (__m(addr)), "i" (errret), "0" (err))

#define __put_user_asm_ex(x, addr, itype, rtype, ltype)			\
	asm volatile("1:	mov"itype" %"rtype"0,%1\n"		\
		     "2:\n"						\
		     _ASM_EXTABLE_EX(1b, 2b)				\
		     : : ltype(x), "m" (__m(addr)))

/*
 * uaccess_try and catch
 */
#define uaccess_try	do {						\
	current->thread.uaccess_err = 0;				\
	__uaccess_begin();						\
	barrier();

#define uaccess_try_nospec do {						\
	current->thread.uaccess_err = 0;				\
	__uaccess_begin_nospec();					\

#define uaccess_catch(err)						\
	__uaccess_end();						\
	(err) |= (current->thread.uaccess_err ? -EFAULT : 0);		\
} while (0)

/**
 * __get_user: - Get a simple variable from user space, with less checking.
=======
 * This macro copies a single simple variable from user space to kernel
 * space.  It supports simple types like char and int, but not larger
 * data types like structures or arrays.
 *
 * @ptr must have pointer-to-simple-variable type, and the result of
 * dereferencing @ptr must be assignable to @x without a cast.
 *
 * Return: zero on success, or -EFAULT on error.
 * On error, the variable @x is set to zero.
 */
#define get_user(x,ptr) ({ might_fault(); do_get_user_call(get_user,x,ptr); })

/**
 * __get_user - Get a simple variable from user space, with less checking.
>>>>>>> upstream/android-13
 * @x:   Variable to store result.
 * @ptr: Source address, in user space.
 *
 * Context: User context only. This function may sleep if pagefaults are
 *          enabled.
 *
 * This macro copies a single simple variable from user space to kernel
 * space.  It supports simple types like char and int, but not larger
 * data types like structures or arrays.
 *
 * @ptr must have pointer-to-simple-variable type, and the result of
 * dereferencing @ptr must be assignable to @x without a cast.
 *
 * Caller must check the pointer with access_ok() before calling this
 * function.
 *
<<<<<<< HEAD
 * Returns zero on success, or -EFAULT on error.
 * On error, the variable @x is set to zero.
 */

#define __get_user(x, ptr)						\
	__get_user_nocheck((x), (ptr), sizeof(*(ptr)))

/**
 * __put_user: - Write a simple value into user space, with less checking.
=======
 * Return: zero on success, or -EFAULT on error.
 * On error, the variable @x is set to zero.
 */
#define __get_user(x,ptr) do_get_user_call(get_user_nocheck,x,ptr)


#ifdef CONFIG_X86_32
#define __put_user_goto_u64(x, addr, label)			\
	asm_volatile_goto("\n"					\
		     "1:	movl %%eax,0(%1)\n"		\
		     "2:	movl %%edx,4(%1)\n"		\
		     _ASM_EXTABLE_UA(1b, %l2)			\
		     _ASM_EXTABLE_UA(2b, %l2)			\
		     : : "A" (x), "r" (addr)			\
		     : : label)

#else
#define __put_user_goto_u64(x, ptr, label) \
	__put_user_goto(x, ptr, "q", "er", label)
#endif

extern void __put_user_bad(void);

/*
 * Strange magic calling convention: pointer in %ecx,
 * value in %eax(:%edx), return value in %ecx. clobbers %rbx
 */
extern void __put_user_1(void);
extern void __put_user_2(void);
extern void __put_user_4(void);
extern void __put_user_8(void);
extern void __put_user_nocheck_1(void);
extern void __put_user_nocheck_2(void);
extern void __put_user_nocheck_4(void);
extern void __put_user_nocheck_8(void);

/*
 * ptr must be evaluated and assigned to the temporary __ptr_pu before
 * the assignment of x to __val_pu, to avoid any function calls
 * involved in the ptr expression (possibly implicitly generated due
 * to KASAN) from clobbering %ax.
 */
#define do_put_user_call(fn,x,ptr)					\
({									\
	int __ret_pu;							\
	void __user *__ptr_pu;						\
	register __typeof__(*(ptr)) __val_pu asm("%"_ASM_AX);		\
	__chk_user_ptr(ptr);						\
	__ptr_pu = (ptr);						\
	__val_pu = (x);							\
	asm volatile("call __" #fn "_%P[size]"				\
		     : "=c" (__ret_pu),					\
			ASM_CALL_CONSTRAINT				\
		     : "0" (__ptr_pu),					\
		       "r" (__val_pu),					\
		       [size] "i" (sizeof(*(ptr)))			\
		     :"ebx");						\
	__builtin_expect(__ret_pu, 0);					\
})

/**
 * put_user - Write a simple value into user space.
 * @x:   Value to copy to user space.
 * @ptr: Destination address, in user space.
 *
 * Context: User context only. This function may sleep if pagefaults are
 *          enabled.
 *
 * This macro copies a single simple value from kernel space to user
 * space.  It supports simple types like char and int, but not larger
 * data types like structures or arrays.
 *
 * @ptr must have pointer-to-simple-variable type, and @x must be assignable
 * to the result of dereferencing @ptr.
 *
 * Return: zero on success, or -EFAULT on error.
 */
#define put_user(x, ptr) ({ might_fault(); do_put_user_call(put_user,x,ptr); })

/**
 * __put_user - Write a simple value into user space, with less checking.
>>>>>>> upstream/android-13
 * @x:   Value to copy to user space.
 * @ptr: Destination address, in user space.
 *
 * Context: User context only. This function may sleep if pagefaults are
 *          enabled.
 *
 * This macro copies a single simple value from kernel space to user
 * space.  It supports simple types like char and int, but not larger
 * data types like structures or arrays.
 *
 * @ptr must have pointer-to-simple-variable type, and @x must be assignable
 * to the result of dereferencing @ptr.
 *
 * Caller must check the pointer with access_ok() before calling this
 * function.
 *
<<<<<<< HEAD
 * Returns zero on success, or -EFAULT on error.
 */

#define __put_user(x, ptr)						\
	__put_user_nocheck((__typeof__(*(ptr)))(x), (ptr), sizeof(*(ptr)))

/*
 * {get|put}_user_try and catch
 *
 * get_user_try {
 *	get_user_ex(...);
 * } get_user_catch(err)
 */
#define get_user_try		uaccess_try_nospec
#define get_user_catch(err)	uaccess_catch(err)

#define get_user_ex(x, ptr)	do {					\
	unsigned long __gue_val;					\
	__get_user_size_ex((__gue_val), (ptr), (sizeof(*(ptr))));	\
	(x) = (__force __typeof__(*(ptr)))__gue_val;			\
} while (0)

#define put_user_try		uaccess_try
#define put_user_catch(err)	uaccess_catch(err)

#define put_user_ex(x, ptr)						\
	__put_user_size_ex((__typeof__(*(ptr)))(x), (ptr), sizeof(*(ptr)))
=======
 * Return: zero on success, or -EFAULT on error.
 */
#define __put_user(x, ptr) do_put_user_call(put_user_nocheck,x,ptr)

#define __put_user_size(x, ptr, size, label)				\
do {									\
	__chk_user_ptr(ptr);						\
	switch (size) {							\
	case 1:								\
		__put_user_goto(x, ptr, "b", "iq", label);		\
		break;							\
	case 2:								\
		__put_user_goto(x, ptr, "w", "ir", label);		\
		break;							\
	case 4:								\
		__put_user_goto(x, ptr, "l", "ir", label);		\
		break;							\
	case 8:								\
		__put_user_goto_u64(x, ptr, label);			\
		break;							\
	default:							\
		__put_user_bad();					\
	}								\
} while (0)

#ifdef CONFIG_CC_HAS_ASM_GOTO_OUTPUT

#ifdef CONFIG_X86_32
#define __get_user_asm_u64(x, ptr, label) do {				\
	unsigned int __gu_low, __gu_high;				\
	const unsigned int __user *__gu_ptr;				\
	__gu_ptr = (const void __user *)(ptr);				\
	__get_user_asm(__gu_low, __gu_ptr, "l", "=r", label);		\
	__get_user_asm(__gu_high, __gu_ptr+1, "l", "=r", label);	\
	(x) = ((unsigned long long)__gu_high << 32) | __gu_low;		\
} while (0)
#else
#define __get_user_asm_u64(x, ptr, label)				\
	__get_user_asm(x, ptr, "q", "=r", label)
#endif

#define __get_user_size(x, ptr, size, label)				\
do {									\
	__chk_user_ptr(ptr);						\
	switch (size) {							\
	case 1:	{							\
		unsigned char x_u8__;					\
		__get_user_asm(x_u8__, ptr, "b", "=q", label);		\
		(x) = x_u8__;						\
		break;							\
	}								\
	case 2:								\
		__get_user_asm(x, ptr, "w", "=r", label);		\
		break;							\
	case 4:								\
		__get_user_asm(x, ptr, "l", "=r", label);		\
		break;							\
	case 8:								\
		__get_user_asm_u64(x, ptr, label);			\
		break;							\
	default:							\
		(x) = __get_user_bad();					\
	}								\
} while (0)

#define __get_user_asm(x, addr, itype, ltype, label)			\
	asm_volatile_goto("\n"						\
		     "1:	mov"itype" %[umem],%[output]\n"		\
		     _ASM_EXTABLE_UA(1b, %l2)				\
		     : [output] ltype(x)				\
		     : [umem] "m" (__m(addr))				\
		     : : label)

#else // !CONFIG_CC_HAS_ASM_GOTO_OUTPUT

#ifdef CONFIG_X86_32
#define __get_user_asm_u64(x, ptr, retval)				\
({									\
	__typeof__(ptr) __ptr = (ptr);					\
	asm volatile("\n"						\
		     "1:	movl %[lowbits],%%eax\n"		\
		     "2:	movl %[highbits],%%edx\n"		\
		     "3:\n"						\
		     ".section .fixup,\"ax\"\n"				\
		     "4:	mov %[efault],%[errout]\n"		\
		     "	xorl %%eax,%%eax\n"				\
		     "	xorl %%edx,%%edx\n"				\
		     "	jmp 3b\n"					\
		     ".previous\n"					\
		     _ASM_EXTABLE_UA(1b, 4b)				\
		     _ASM_EXTABLE_UA(2b, 4b)				\
		     : [errout] "=r" (retval),				\
		       [output] "=&A"(x)				\
		     : [lowbits] "m" (__m(__ptr)),			\
		       [highbits] "m" __m(((u32 __user *)(__ptr)) + 1),	\
		       [efault] "i" (-EFAULT), "0" (retval));		\
})

#else
#define __get_user_asm_u64(x, ptr, retval) \
	 __get_user_asm(x, ptr, retval, "q", "=r")
#endif

#define __get_user_size(x, ptr, size, retval)				\
do {									\
	unsigned char x_u8__;						\
									\
	retval = 0;							\
	__chk_user_ptr(ptr);						\
	switch (size) {							\
	case 1:								\
		__get_user_asm(x_u8__, ptr, retval, "b", "=q");		\
		(x) = x_u8__;						\
		break;							\
	case 2:								\
		__get_user_asm(x, ptr, retval, "w", "=r");		\
		break;							\
	case 4:								\
		__get_user_asm(x, ptr, retval, "l", "=r");		\
		break;							\
	case 8:								\
		__get_user_asm_u64(x, ptr, retval);			\
		break;							\
	default:							\
		(x) = __get_user_bad();					\
	}								\
} while (0)

#define __get_user_asm(x, addr, err, itype, ltype)			\
	asm volatile("\n"						\
		     "1:	mov"itype" %[umem],%[output]\n"		\
		     "2:\n"						\
		     ".section .fixup,\"ax\"\n"				\
		     "3:	mov %[efault],%[errout]\n"		\
		     "	xorl %k[output],%k[output]\n"			\
		     "	jmp 2b\n"					\
		     ".previous\n"					\
		     _ASM_EXTABLE_UA(1b, 3b)				\
		     : [errout] "=r" (err),				\
		       [output] ltype(x)				\
		     : [umem] "m" (__m(addr)),				\
		       [efault] "i" (-EFAULT), "0" (err))

#endif // CONFIG_CC_ASM_GOTO_OUTPUT

/* FIXME: this hack is definitely wrong -AK */
struct __large_struct { unsigned long buf[100]; };
#define __m(x) (*(struct __large_struct __user *)(x))

/*
 * Tell gcc we read from memory instead of writing: this is because
 * we do not write to any memory gcc knows about, so there are no
 * aliasing issues.
 */
#define __put_user_goto(x, addr, itype, ltype, label)			\
	asm_volatile_goto("\n"						\
		"1:	mov"itype" %0,%1\n"				\
		_ASM_EXTABLE_UA(1b, %l2)				\
		: : ltype(x), "m" (__m(addr))				\
		: : label)
>>>>>>> upstream/android-13

extern unsigned long
copy_from_user_nmi(void *to, const void __user *from, unsigned long n);
extern __must_check long
strncpy_from_user(char *dst, const char __user *src, long count);

extern __must_check long strnlen_user(const char __user *str, long n);

unsigned long __must_check clear_user(void __user *mem, unsigned long len);
unsigned long __must_check __clear_user(void __user *mem, unsigned long len);

<<<<<<< HEAD
extern void __cmpxchg_wrong_size(void)
	__compiletime_error("Bad argument size for cmpxchg");

#define __user_atomic_cmpxchg_inatomic(uval, ptr, old, new, size)	\
({									\
	int __ret = 0;							\
	__typeof__(ptr) __uval = (uval);				\
	__typeof__(*(ptr)) __old = (old);				\
	__typeof__(*(ptr)) __new = (new);				\
	__uaccess_begin_nospec();					\
	switch (size) {							\
	case 1:								\
	{								\
		asm volatile("\n"					\
			"1:\t" LOCK_PREFIX "cmpxchgb %4, %2\n"		\
			"2:\n"						\
			"\t.section .fixup, \"ax\"\n"			\
			"3:\tmov     %3, %0\n"				\
			"\tjmp     2b\n"				\
			"\t.previous\n"					\
			_ASM_EXTABLE(1b, 3b)				\
			: "+r" (__ret), "=a" (__old), "+m" (*(ptr))	\
			: "i" (-EFAULT), "q" (__new), "1" (__old)	\
			: "memory"					\
		);							\
		break;							\
	}								\
	case 2:								\
	{								\
		asm volatile("\n"					\
			"1:\t" LOCK_PREFIX "cmpxchgw %4, %2\n"		\
			"2:\n"						\
			"\t.section .fixup, \"ax\"\n"			\
			"3:\tmov     %3, %0\n"				\
			"\tjmp     2b\n"				\
			"\t.previous\n"					\
			_ASM_EXTABLE(1b, 3b)				\
			: "+r" (__ret), "=a" (__old), "+m" (*(ptr))	\
			: "i" (-EFAULT), "r" (__new), "1" (__old)	\
			: "memory"					\
		);							\
		break;							\
	}								\
	case 4:								\
	{								\
		asm volatile("\n"					\
			"1:\t" LOCK_PREFIX "cmpxchgl %4, %2\n"		\
			"2:\n"						\
			"\t.section .fixup, \"ax\"\n"			\
			"3:\tmov     %3, %0\n"				\
			"\tjmp     2b\n"				\
			"\t.previous\n"					\
			_ASM_EXTABLE(1b, 3b)				\
			: "+r" (__ret), "=a" (__old), "+m" (*(ptr))	\
			: "i" (-EFAULT), "r" (__new), "1" (__old)	\
			: "memory"					\
		);							\
		break;							\
	}								\
	case 8:								\
	{								\
		if (!IS_ENABLED(CONFIG_X86_64))				\
			__cmpxchg_wrong_size();				\
									\
		asm volatile("\n"					\
			"1:\t" LOCK_PREFIX "cmpxchgq %4, %2\n"		\
			"2:\n"						\
			"\t.section .fixup, \"ax\"\n"			\
			"3:\tmov     %3, %0\n"				\
			"\tjmp     2b\n"				\
			"\t.previous\n"					\
			_ASM_EXTABLE(1b, 3b)				\
			: "+r" (__ret), "=a" (__old), "+m" (*(ptr))	\
			: "i" (-EFAULT), "r" (__new), "1" (__old)	\
			: "memory"					\
		);							\
		break;							\
	}								\
	default:							\
		__cmpxchg_wrong_size();					\
	}								\
	__uaccess_end();						\
	*__uval = __old;						\
	__ret;								\
})

#define user_atomic_cmpxchg_inatomic(uval, ptr, old, new)		\
({									\
	access_ok(VERIFY_WRITE, (ptr), sizeof(*(ptr))) ?		\
		__user_atomic_cmpxchg_inatomic((uval), (ptr),		\
				(old), (new), sizeof(*(ptr))) :		\
		-EFAULT;						\
})
=======
#ifdef CONFIG_ARCH_HAS_COPY_MC
unsigned long __must_check
copy_mc_to_kernel(void *to, const void *from, unsigned len);
#define copy_mc_to_kernel copy_mc_to_kernel

unsigned long __must_check
copy_mc_to_user(void *to, const void *from, unsigned len);
#endif
>>>>>>> upstream/android-13

/*
 * movsl can be slow when source and dest are not both 8-byte aligned
 */
#ifdef CONFIG_X86_INTEL_USERCOPY
extern struct movsl_mask {
	int mask;
} ____cacheline_aligned_in_smp movsl_mask;
#endif

#define ARCH_HAS_NOCACHE_UACCESS 1

#ifdef CONFIG_X86_32
# include <asm/uaccess_32.h>
#else
# include <asm/uaccess_64.h>
#endif

/*
<<<<<<< HEAD
 * We rely on the nested NMI work to allow atomic faults from the NMI path; the
 * nested NMI paths are careful to preserve CR2.
 *
 * Caller must use pagefault_enable/disable, or run in interrupt context,
 * and also do a uaccess_ok() check
 */
#define __copy_from_user_nmi __copy_from_user_inatomic

/*
=======
>>>>>>> upstream/android-13
 * The "unsafe" user accesses aren't really "unsafe", but the naming
 * is a big fat warning: you have to not only do the access_ok()
 * checking before using them, but you have to surround them with the
 * user_access_begin/end() pair.
 */
<<<<<<< HEAD
static __must_check inline bool user_access_begin(int type,
						  const void __user *ptr,
						  size_t len)
{
	if (unlikely(!access_ok(type, ptr, len)))
=======
static __must_check __always_inline bool user_access_begin(const void __user *ptr, size_t len)
{
	if (unlikely(!access_ok(ptr,len)))
>>>>>>> upstream/android-13
		return 0;
	__uaccess_begin_nospec();
	return 1;
}
<<<<<<< HEAD

#define user_access_begin(a, b, c)	user_access_begin(a, b, c)
=======
#define user_access_begin(a,b)	user_access_begin(a,b)
>>>>>>> upstream/android-13
#define user_access_end()	__uaccess_end()

#define user_access_save()	smap_save()
#define user_access_restore(x)	smap_restore(x)

<<<<<<< HEAD
#define unsafe_put_user(x, ptr, err_label)					\
do {										\
	int __pu_err;								\
	__typeof__(*(ptr)) __pu_val = (x);					\
	__put_user_size(__pu_val, (ptr), sizeof(*(ptr)), __pu_err, -EFAULT);	\
	if (unlikely(__pu_err)) goto err_label;					\
} while (0)

=======
#define unsafe_put_user(x, ptr, label)	\
	__put_user_size((__typeof__(*(ptr)))(x), (ptr), sizeof(*(ptr)), label)

#ifdef CONFIG_CC_HAS_ASM_GOTO_OUTPUT
#define unsafe_get_user(x, ptr, err_label)					\
do {										\
	__inttype(*(ptr)) __gu_val;						\
	__get_user_size(__gu_val, (ptr), sizeof(*(ptr)), err_label);		\
	(x) = (__force __typeof__(*(ptr)))__gu_val;				\
} while (0)
#else // !CONFIG_CC_HAS_ASM_GOTO_OUTPUT
>>>>>>> upstream/android-13
#define unsafe_get_user(x, ptr, err_label)					\
do {										\
	int __gu_err;								\
	__inttype(*(ptr)) __gu_val;						\
<<<<<<< HEAD
	__get_user_size(__gu_val, (ptr), sizeof(*(ptr)), __gu_err, -EFAULT);	\
	(x) = (__force __typeof__(*(ptr)))__gu_val;				\
	if (unlikely(__gu_err)) goto err_label;					\
} while (0)
=======
	__get_user_size(__gu_val, (ptr), sizeof(*(ptr)), __gu_err);		\
	(x) = (__force __typeof__(*(ptr)))__gu_val;				\
	if (unlikely(__gu_err)) goto err_label;					\
} while (0)
#endif // CONFIG_CC_HAS_ASM_GOTO_OUTPUT

/*
 * We want the unsafe accessors to always be inlined and use
 * the error labels - thus the macro games.
 */
#define unsafe_copy_loop(dst, src, len, type, label)				\
	while (len >= sizeof(type)) {						\
		unsafe_put_user(*(type *)(src),(type __user *)(dst),label);	\
		dst += sizeof(type);						\
		src += sizeof(type);						\
		len -= sizeof(type);						\
	}

#define unsafe_copy_to_user(_dst,_src,_len,label)			\
do {									\
	char __user *__ucu_dst = (_dst);				\
	const char *__ucu_src = (_src);					\
	size_t __ucu_len = (_len);					\
	unsafe_copy_loop(__ucu_dst, __ucu_src, __ucu_len, u64, label);	\
	unsafe_copy_loop(__ucu_dst, __ucu_src, __ucu_len, u32, label);	\
	unsafe_copy_loop(__ucu_dst, __ucu_src, __ucu_len, u16, label);	\
	unsafe_copy_loop(__ucu_dst, __ucu_src, __ucu_len, u8, label);	\
} while (0)

#define HAVE_GET_KERNEL_NOFAULT

#ifdef CONFIG_CC_HAS_ASM_GOTO_OUTPUT
#define __get_kernel_nofault(dst, src, type, err_label)			\
	__get_user_size(*((type *)(dst)), (__force type __user *)(src),	\
			sizeof(type), err_label)
#else // !CONFIG_CC_HAS_ASM_GOTO_OUTPUT
#define __get_kernel_nofault(dst, src, type, err_label)			\
do {									\
	int __kr_err;							\
									\
	__get_user_size(*((type *)(dst)), (__force type __user *)(src),	\
			sizeof(type), __kr_err);			\
	if (unlikely(__kr_err))						\
		goto err_label;						\
} while (0)
#endif // CONFIG_CC_HAS_ASM_GOTO_OUTPUT

#define __put_kernel_nofault(dst, src, type, err_label)			\
	__put_user_size(*((type *)(src)), (__force type __user *)(dst),	\
			sizeof(type), err_label)
>>>>>>> upstream/android-13

#endif /* _ASM_X86_UACCESS_H */

