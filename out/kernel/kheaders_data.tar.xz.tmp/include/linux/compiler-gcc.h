/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __LINUX_COMPILER_TYPES_H
#error "Please don't include <linux/compiler-gcc.h> directly, include <linux/compiler.h> instead."
#endif


#define GCC_VERSION (__GNUC__ * 10000		\
		     + __GNUC_MINOR__ * 100	\
		     + __GNUC_PATCHLEVEL__)

#if GCC_VERSION < 40600
# error Sorry, your compiler is too old - please upgrade it.
#elif defined(CONFIG_ARM64) && GCC_VERSION < 50100

# error Sorry, your version of GCC is too old - please use 5.1 or newer.
#endif


#define RELOC_HIDE(ptr, off)						\
({									\
	unsigned long __ptr;						\
	__asm__ ("" : "=r"(__ptr) : "0"(ptr));				\
	(typeof(ptr)) (__ptr + (off));					\
})


#define uninitialized_var(x) x = x

#ifdef __CHECKER__
#define __must_be_array(a)	0
#else

#define __must_be_array(a)	BUILD_BUG_ON_ZERO(__same_type((a), &(a)[0]))
#endif

#ifdef CONFIG_RETPOLINE
#define __noretpoline __attribute__((indirect_branch("keep")))
#endif

#define __UNIQUE_ID(prefix) __PASTE(__PASTE(__UNIQUE_ID_, prefix), __COUNTER__)

#define __optimize(level)	__attribute__((__optimize__(level)))

#define __compiletime_object_size(obj) __builtin_object_size(obj, 0)

#ifndef __CHECKER__
#define __compiletime_warning(message) __attribute__((warning(message)))
#define __compiletime_error(message) __attribute__((error(message)))

#ifdef LATENT_ENTROPY_PLUGIN
#define __latent_entropy __attribute__((latent_entropy))
#endif
#endif 


#define barrier_before_unreachable() asm volatile("")


#define unreachable() \
	do {					\
		annotate_unreachable();		\
		barrier_before_unreachable();	\
		__builtin_unreachable();	\
	} while (0)


#define __noclone	__attribute__((__noclone__, __optimize__("no-tracer")))

#if defined(RANDSTRUCT_PLUGIN) && !defined(__CHECKER__)
#define __randomize_layout __attribute__((randomize_layout))
#define __no_randomize_layout __attribute__((no_randomize_layout))

#define randomized_struct_fields_start	struct {
#define randomized_struct_fields_end	} __randomize_layout;
#endif


#define __visible	__attribute__((externally_visible))



#if GCC_VERSION >= 40900 && !defined(__CHECKER__)

#define __assume_aligned(a, ...) __attribute__((__assume_aligned__(a, ## __VA_ARGS__)))
#endif


#define asm_volatile_goto(x...)	do { asm goto(x); asm (""); } while (0)


#if defined(CONFIG_ARCH_USE_BUILTIN_BSWAP) && !defined(__CHECKER__)
#define __HAVE_BUILTIN_BSWAP32__
#define __HAVE_BUILTIN_BSWAP64__
#if GCC_VERSION >= 40800
#define __HAVE_BUILTIN_BSWAP16__
#endif
#endif 

#if GCC_VERSION >= 70000
#define KASAN_ABI_VERSION 5
#elif GCC_VERSION >= 50000
#define KASAN_ABI_VERSION 4
#elif GCC_VERSION >= 40902
#define KASAN_ABI_VERSION 3
#endif


#ifndef __has_attribute
# define __has_attribute(x) __GCC4_has_attribute_##x
# define __GCC4_has_attribute___no_sanitize_address__ (__GNUC_MINOR__ >= 8)
#endif

#if __has_attribute(__no_sanitize_address__)
#define __no_sanitize_address __attribute__((no_sanitize_address))
#else
#define __no_sanitize_address
#endif

#if GCC_VERSION >= 50100

#define __designated_init __attribute__((designated_init))
#define COMPILER_HAS_GENERIC_BUILTIN_OVERFLOW 1
#endif

#if GCC_VERSION >= 90100
#define __copy(symbol)                 __attribute__((__copy__(symbol)))
#endif

#if !defined(__noclone)
#define __noclone	
#endif

#if !defined(__no_sanitize_address)
#define __no_sanitize_address
#endif


#define __diag_GCC(version, severity, s) \
	__diag_GCC_ ## version(__diag_GCC_ ## severity s)


#define __diag_GCC_ignore	ignored
#define __diag_GCC_warn		warning
#define __diag_GCC_error	error

#define __diag_str1(s)		#s
#define __diag_str(s)		__diag_str1(s)
#define __diag(s)		_Pragma(__diag_str(GCC diagnostic s))

#if GCC_VERSION >= 80000
#define __diag_GCC_8(s)		__diag(s)
#else
#define __diag_GCC_8(s)
#endif
