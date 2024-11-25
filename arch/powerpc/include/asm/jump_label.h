<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-or-later */
>>>>>>> upstream/android-13
#ifndef _ASM_POWERPC_JUMP_LABEL_H
#define _ASM_POWERPC_JUMP_LABEL_H

/*
 * Copyright 2010 Michael Ellerman, IBM Corp.
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version
 * 2 of the License, or (at your option) any later version.
=======
>>>>>>> upstream/android-13
 */

#ifndef __ASSEMBLY__
#include <linux/types.h>

#include <asm/feature-fixups.h>
#include <asm/asm-const.h>

#define JUMP_ENTRY_TYPE		stringify_in_c(FTR_ENTRY_LONG)
#define JUMP_LABEL_NOP_SIZE	4

static __always_inline bool arch_static_branch(struct static_key *key, bool branch)
{
	asm_volatile_goto("1:\n\t"
		 "nop # arch_static_branch\n\t"
		 ".pushsection __jump_table,  \"aw\"\n\t"
<<<<<<< HEAD
		 JUMP_ENTRY_TYPE "1b, %l[l_yes], %c0\n\t"
=======
		 ".long 1b - ., %l[l_yes] - .\n\t"
		 JUMP_ENTRY_TYPE "%c0 - .\n\t"
>>>>>>> upstream/android-13
		 ".popsection \n\t"
		 : :  "i" (&((char *)key)[branch]) : : l_yes);

	return false;
l_yes:
	return true;
}

static __always_inline bool arch_static_branch_jump(struct static_key *key, bool branch)
{
	asm_volatile_goto("1:\n\t"
		 "b %l[l_yes] # arch_static_branch_jump\n\t"
		 ".pushsection __jump_table,  \"aw\"\n\t"
<<<<<<< HEAD
		 JUMP_ENTRY_TYPE "1b, %l[l_yes], %c0\n\t"
=======
		 ".long 1b - ., %l[l_yes] - .\n\t"
		 JUMP_ENTRY_TYPE "%c0 - .\n\t"
>>>>>>> upstream/android-13
		 ".popsection \n\t"
		 : :  "i" (&((char *)key)[branch]) : : l_yes);

	return false;
l_yes:
	return true;
}

<<<<<<< HEAD
#ifdef CONFIG_PPC64
typedef u64 jump_label_t;
#else
typedef u32 jump_label_t;
#endif

struct jump_entry {
	jump_label_t code;
	jump_label_t target;
	jump_label_t key;
};

=======
>>>>>>> upstream/android-13
#else
#define ARCH_STATIC_BRANCH(LABEL, KEY)		\
1098:	nop;					\
	.pushsection __jump_table, "aw";	\
<<<<<<< HEAD
	FTR_ENTRY_LONG 1098b, LABEL, KEY;	\
=======
	.long 1098b - ., LABEL - .;		\
	FTR_ENTRY_LONG KEY - .;			\
>>>>>>> upstream/android-13
	.popsection
#endif

#endif /* _ASM_POWERPC_JUMP_LABEL_H */
