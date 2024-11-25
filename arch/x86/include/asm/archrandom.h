<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> upstream/android-13
/*
 * This file is part of the Linux kernel.
 *
 * Copyright (c) 2011-2014, Intel Corporation
 * Authors: Fenghua Yu <fenghua.yu@intel.com>,
 *          H. Peter Anvin <hpa@linux.intel.com>
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin St - Fifth Floor, Boston, MA 02110-1301 USA.
 *
=======
>>>>>>> upstream/android-13
 */

#ifndef ASM_X86_ARCHRANDOM_H
#define ASM_X86_ARCHRANDOM_H

#include <asm/processor.h>
#include <asm/cpufeature.h>

#define RDRAND_RETRY_LOOPS	10

<<<<<<< HEAD
#define RDRAND_INT	".byte 0x0f,0xc7,0xf0"
#define RDSEED_INT	".byte 0x0f,0xc7,0xf8"
#ifdef CONFIG_X86_64
# define RDRAND_LONG	".byte 0x48,0x0f,0xc7,0xf0"
# define RDSEED_LONG	".byte 0x48,0x0f,0xc7,0xf8"
#else
# define RDRAND_LONG	RDRAND_INT
# define RDSEED_LONG	RDSEED_INT
#endif

/* Unconditional execution of RDRAND and RDSEED */

static inline bool rdrand_long(unsigned long *v)
=======
/* Unconditional execution of RDRAND and RDSEED */

static inline bool __must_check rdrand_long(unsigned long *v)
>>>>>>> upstream/android-13
{
	bool ok;
	unsigned int retry = RDRAND_RETRY_LOOPS;
	do {
<<<<<<< HEAD
		asm volatile(RDRAND_LONG
			     CC_SET(c)
			     : CC_OUT(c) (ok), "=a" (*v));
=======
		asm volatile("rdrand %[out]"
			     CC_SET(c)
			     : CC_OUT(c) (ok), [out] "=r" (*v));
>>>>>>> upstream/android-13
		if (ok)
			return true;
	} while (--retry);
	return false;
}

<<<<<<< HEAD
static inline bool rdrand_int(unsigned int *v)
=======
static inline bool __must_check rdrand_int(unsigned int *v)
>>>>>>> upstream/android-13
{
	bool ok;
	unsigned int retry = RDRAND_RETRY_LOOPS;
	do {
<<<<<<< HEAD
		asm volatile(RDRAND_INT
			     CC_SET(c)
			     : CC_OUT(c) (ok), "=a" (*v));
=======
		asm volatile("rdrand %[out]"
			     CC_SET(c)
			     : CC_OUT(c) (ok), [out] "=r" (*v));
>>>>>>> upstream/android-13
		if (ok)
			return true;
	} while (--retry);
	return false;
}

<<<<<<< HEAD
static inline bool rdseed_long(unsigned long *v)
{
	bool ok;
	asm volatile(RDSEED_LONG
		     CC_SET(c)
		     : CC_OUT(c) (ok), "=a" (*v));
	return ok;
}

static inline bool rdseed_int(unsigned int *v)
{
	bool ok;
	asm volatile(RDSEED_INT
		     CC_SET(c)
		     : CC_OUT(c) (ok), "=a" (*v));
	return ok;
}

/* Conditional execution based on CPU type */
#define arch_has_random()	static_cpu_has(X86_FEATURE_RDRAND)
#define arch_has_random_seed()	static_cpu_has(X86_FEATURE_RDSEED)

=======
static inline bool __must_check rdseed_long(unsigned long *v)
{
	bool ok;
	asm volatile("rdseed %[out]"
		     CC_SET(c)
		     : CC_OUT(c) (ok), [out] "=r" (*v));
	return ok;
}

static inline bool __must_check rdseed_int(unsigned int *v)
{
	bool ok;
	asm volatile("rdseed %[out]"
		     CC_SET(c)
		     : CC_OUT(c) (ok), [out] "=r" (*v));
	return ok;
}

>>>>>>> upstream/android-13
/*
 * These are the generic interfaces; they must not be declared if the
 * stubs in <linux/random.h> are to be invoked,
 * i.e. CONFIG_ARCH_RANDOM is not defined.
 */
#ifdef CONFIG_ARCH_RANDOM

<<<<<<< HEAD
static inline bool arch_get_random_long(unsigned long *v)
{
	return arch_has_random() ? rdrand_long(v) : false;
}

static inline bool arch_get_random_int(unsigned int *v)
{
	return arch_has_random() ? rdrand_int(v) : false;
}

static inline bool arch_get_random_seed_long(unsigned long *v)
{
	return arch_has_random_seed() ? rdseed_long(v) : false;
}

static inline bool arch_get_random_seed_int(unsigned int *v)
{
	return arch_has_random_seed() ? rdseed_int(v) : false;
=======
static inline bool __must_check arch_get_random_long(unsigned long *v)
{
	return static_cpu_has(X86_FEATURE_RDRAND) ? rdrand_long(v) : false;
}

static inline bool __must_check arch_get_random_int(unsigned int *v)
{
	return static_cpu_has(X86_FEATURE_RDRAND) ? rdrand_int(v) : false;
}

static inline bool __must_check arch_get_random_seed_long(unsigned long *v)
{
	return static_cpu_has(X86_FEATURE_RDSEED) ? rdseed_long(v) : false;
}

static inline bool __must_check arch_get_random_seed_int(unsigned int *v)
{
	return static_cpu_has(X86_FEATURE_RDSEED) ? rdseed_int(v) : false;
>>>>>>> upstream/android-13
}

extern void x86_init_rdrand(struct cpuinfo_x86 *c);

#else  /* !CONFIG_ARCH_RANDOM */

static inline void x86_init_rdrand(struct cpuinfo_x86 *c) { }

#endif  /* !CONFIG_ARCH_RANDOM */

#endif /* ASM_X86_ARCHRANDOM_H */
