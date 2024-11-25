/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _ASM_X86_SECTIONS_H
#define _ASM_X86_SECTIONS_H

<<<<<<< HEAD
=======
#define arch_is_kernel_initmem_freed arch_is_kernel_initmem_freed

>>>>>>> upstream/android-13
#include <asm-generic/sections.h>
#include <asm/extable.h>

extern char __brk_base[], __brk_limit[];
<<<<<<< HEAD
extern char __cfi_jt_start[], __cfi_jt_end[];
=======
>>>>>>> upstream/android-13
extern char __end_rodata_aligned[];

#if defined(CONFIG_X86_64)
extern char __end_rodata_hpage_align[];
<<<<<<< HEAD
extern char __entry_trampoline_start[], __entry_trampoline_end[];
#endif

=======
#endif

extern char __end_of_kernel_reserve[];

extern unsigned long _brk_start, _brk_end;

static inline bool arch_is_kernel_initmem_freed(unsigned long addr)
{
	/*
	 * If _brk_start has not been cleared, brk allocation is incomplete,
	 * and we can not make assumptions about its use.
	 */
	if (_brk_start)
		return 0;

	/*
	 * After brk allocation is complete, space between _brk_end and _end
	 * is available for allocation.
	 */
	return addr >= _brk_end && addr < (unsigned long)&_end;
}

>>>>>>> upstream/android-13
#endif	/* _ASM_X86_SECTIONS_H */
