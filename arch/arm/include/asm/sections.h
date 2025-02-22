/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _ASM_ARM_SECTIONS_H
#define _ASM_ARM_SECTIONS_H

#include <asm-generic/sections.h>

extern char _exiprom[];

extern char __idmap_text_start[];
extern char __idmap_text_end[];
extern char __entry_text_start[];
extern char __entry_text_end[];
<<<<<<< HEAD
extern char __hyp_idmap_text_start[];
extern char __hyp_idmap_text_end[];
=======
>>>>>>> upstream/android-13

static inline bool in_entry_text(unsigned long addr)
{
	return memory_contains(__entry_text_start, __entry_text_end,
			       (void *)addr, 1);
}

static inline bool in_idmap_text(unsigned long addr)
{
	void *a = (void *)addr;
<<<<<<< HEAD
	return memory_contains(__idmap_text_start, __idmap_text_end, a, 1) ||
	       memory_contains(__hyp_idmap_text_start, __hyp_idmap_text_end,
			       a, 1);
=======
	return memory_contains(__idmap_text_start, __idmap_text_end, a, 1);
>>>>>>> upstream/android-13
}

#endif	/* _ASM_ARM_SECTIONS_H */
