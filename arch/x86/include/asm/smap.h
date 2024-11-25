<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> upstream/android-13
/*
 * Supervisor Mode Access Prevention support
 *
 * Copyright (C) 2012 Intel Corporation
 * Author: H. Peter Anvin <hpa@linux.intel.com>
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; version 2
 * of the License.
=======
>>>>>>> upstream/android-13
 */

#ifndef _ASM_X86_SMAP_H
#define _ASM_X86_SMAP_H

<<<<<<< HEAD
#include <linux/stringify.h>
#include <asm/nops.h>
#include <asm/cpufeatures.h>

/* "Raw" instruction opcodes */
#define __ASM_CLAC	.byte 0x0f,0x01,0xca
#define __ASM_STAC	.byte 0x0f,0x01,0xcb

#ifdef __ASSEMBLY__

#include <asm/alternative-asm.h>

#ifdef CONFIG_X86_SMAP

#define ASM_CLAC \
	ALTERNATIVE "", __stringify(__ASM_CLAC), X86_FEATURE_SMAP

#define ASM_STAC \
	ALTERNATIVE "", __stringify(__ASM_STAC), X86_FEATURE_SMAP
=======
#include <asm/nops.h>
#include <asm/cpufeatures.h>
#include <asm/alternative.h>

/* "Raw" instruction opcodes */
#define __ASM_CLAC	".byte 0x0f,0x01,0xca"
#define __ASM_STAC	".byte 0x0f,0x01,0xcb"

#ifdef __ASSEMBLY__

#ifdef CONFIG_X86_SMAP

#define ASM_CLAC \
	ALTERNATIVE "", __ASM_CLAC, X86_FEATURE_SMAP

#define ASM_STAC \
	ALTERNATIVE "", __ASM_STAC, X86_FEATURE_SMAP
>>>>>>> upstream/android-13

#else /* CONFIG_X86_SMAP */

#define ASM_CLAC
#define ASM_STAC

#endif /* CONFIG_X86_SMAP */

#else /* __ASSEMBLY__ */

<<<<<<< HEAD
#include <asm/alternative.h>

=======
>>>>>>> upstream/android-13
#ifdef CONFIG_X86_SMAP

static __always_inline void clac(void)
{
	/* Note: a barrier is implicit in alternative() */
<<<<<<< HEAD
	alternative("", __stringify(__ASM_CLAC), X86_FEATURE_SMAP);
=======
	alternative("", __ASM_CLAC, X86_FEATURE_SMAP);
>>>>>>> upstream/android-13
}

static __always_inline void stac(void)
{
	/* Note: a barrier is implicit in alternative() */
<<<<<<< HEAD
	alternative("", __stringify(__ASM_STAC), X86_FEATURE_SMAP);
=======
	alternative("", __ASM_STAC, X86_FEATURE_SMAP);
>>>>>>> upstream/android-13
}

static __always_inline unsigned long smap_save(void)
{
	unsigned long flags;

<<<<<<< HEAD
	asm volatile (ALTERNATIVE("", "pushf; pop %0; " __stringify(__ASM_CLAC),
=======
	asm volatile ("# smap_save\n\t"
		      ALTERNATIVE("", "pushf; pop %0; " __ASM_CLAC "\n\t",
>>>>>>> upstream/android-13
				  X86_FEATURE_SMAP)
		      : "=rm" (flags) : : "memory", "cc");

	return flags;
}

static __always_inline void smap_restore(unsigned long flags)
{
<<<<<<< HEAD
	asm volatile (ALTERNATIVE("", "push %0; popf", X86_FEATURE_SMAP)
=======
	asm volatile ("# smap_restore\n\t"
		      ALTERNATIVE("", "push %0; popf\n\t",
				  X86_FEATURE_SMAP)
>>>>>>> upstream/android-13
		      : : "g" (flags) : "memory", "cc");
}

/* These macros can be used in asm() statements */
#define ASM_CLAC \
<<<<<<< HEAD
	ALTERNATIVE("", __stringify(__ASM_CLAC), X86_FEATURE_SMAP)
#define ASM_STAC \
	ALTERNATIVE("", __stringify(__ASM_STAC), X86_FEATURE_SMAP)
=======
	ALTERNATIVE("", __ASM_CLAC, X86_FEATURE_SMAP)
#define ASM_STAC \
	ALTERNATIVE("", __ASM_STAC, X86_FEATURE_SMAP)
>>>>>>> upstream/android-13

#else /* CONFIG_X86_SMAP */

static inline void clac(void) { }
static inline void stac(void) { }

static inline unsigned long smap_save(void) { return 0; }
static inline void smap_restore(unsigned long flags) { }

#define ASM_CLAC
#define ASM_STAC

#endif /* CONFIG_X86_SMAP */

#endif /* __ASSEMBLY__ */

#endif /* _ASM_X86_SMAP_H */
