/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _ASM_S390_JUMP_LABEL_H
#define _ASM_S390_JUMP_LABEL_H

#ifndef __ASSEMBLY__

#include <linux/types.h>
#include <linux/stringify.h>

#define JUMP_LABEL_NOP_SIZE 6
#define JUMP_LABEL_NOP_OFFSET 2

<<<<<<< HEAD
#if __GNUC__ < 9
=======
#ifdef CONFIG_CC_IS_CLANG
#define JUMP_LABEL_STATIC_KEY_CONSTRAINT "i"
#elif __GNUC__ < 9
>>>>>>> upstream/android-13
#define JUMP_LABEL_STATIC_KEY_CONSTRAINT "X"
#else
#define JUMP_LABEL_STATIC_KEY_CONSTRAINT "jdd"
#endif

/*
 * We use a brcl 0,2 instruction for jump labels at compile time so it
 * can be easily distinguished from a hotpatch generated instruction.
 */
static __always_inline bool arch_static_branch(struct static_key *key, bool branch)
{
<<<<<<< HEAD
	asm_volatile_goto("0:	brcl 0,"__stringify(JUMP_LABEL_NOP_OFFSET)"\n"
		".pushsection __jump_table, \"aw\"\n"
		".balign 8\n"
		".quad 0b, %l[label], %0+%1\n"
		".popsection\n"
		: : JUMP_LABEL_STATIC_KEY_CONSTRAINT (key), "i" (branch) : : label);

=======
	asm_volatile_goto("0:	brcl	0,"__stringify(JUMP_LABEL_NOP_OFFSET)"\n"
			  ".pushsection __jump_table,\"aw\"\n"
			  ".balign	8\n"
			  ".long	0b-.,%l[label]-.\n"
			  ".quad	%0+%1-.\n"
			  ".popsection\n"
			  : : JUMP_LABEL_STATIC_KEY_CONSTRAINT (key), "i" (branch) : : label);
>>>>>>> upstream/android-13
	return false;
label:
	return true;
}

static __always_inline bool arch_static_branch_jump(struct static_key *key, bool branch)
{
<<<<<<< HEAD
	asm_volatile_goto("0:	brcl 15, %l[label]\n"
		".pushsection __jump_table, \"aw\"\n"
		".balign 8\n"
		".quad 0b, %l[label], %0+%1\n"
		".popsection\n"
		: : JUMP_LABEL_STATIC_KEY_CONSTRAINT (key), "i" (branch) : : label);

=======
	asm_volatile_goto("0:	brcl 15,%l[label]\n"
			  ".pushsection __jump_table,\"aw\"\n"
			  ".balign	8\n"
			  ".long	0b-.,%l[label]-.\n"
			  ".quad	%0+%1-.\n"
			  ".popsection\n"
			  : : JUMP_LABEL_STATIC_KEY_CONSTRAINT (key), "i" (branch) : : label);
>>>>>>> upstream/android-13
	return false;
label:
	return true;
}

<<<<<<< HEAD
typedef unsigned long jump_label_t;

struct jump_entry {
	jump_label_t code;
	jump_label_t target;
	jump_label_t key;
};

=======
>>>>>>> upstream/android-13
#endif  /* __ASSEMBLY__ */
#endif
