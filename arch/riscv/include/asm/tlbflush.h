<<<<<<< HEAD
/*
 * Copyright (C) 2009 Chen Liqin <liqin.chen@sunplusct.com>
 * Copyright (C) 2012 Regents of the University of California
 *
 *   This program is free software; you can redistribute it and/or
 *   modify it under the terms of the GNU General Public License
 *   as published by the Free Software Foundation, version 2.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
=======
/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Copyright (C) 2009 Chen Liqin <liqin.chen@sunplusct.com>
 * Copyright (C) 2012 Regents of the University of California
>>>>>>> upstream/android-13
 */

#ifndef _ASM_RISCV_TLBFLUSH_H
#define _ASM_RISCV_TLBFLUSH_H

#include <linux/mm_types.h>
<<<<<<< HEAD

/*
 * Flush entire local TLB.  'sfence.vma' implicitly fences with the instruction
 * cache as well, so a 'fence.i' is not necessary.
 */
=======
#include <asm/smp.h>
#include <asm/errata_list.h>

#ifdef CONFIG_MMU
>>>>>>> upstream/android-13
static inline void local_flush_tlb_all(void)
{
	__asm__ __volatile__ ("sfence.vma" : : : "memory");
}

/* Flush one page from local TLB */
static inline void local_flush_tlb_page(unsigned long addr)
{
<<<<<<< HEAD
	__asm__ __volatile__ ("sfence.vma %0" : : "r" (addr) : "memory");
}

#ifndef CONFIG_SMP
=======
	ALT_FLUSH_TLB_PAGE(__asm__ __volatile__ ("sfence.vma %0" : : "r" (addr) : "memory"));
}
#else /* CONFIG_MMU */
#define local_flush_tlb_all()			do { } while (0)
#define local_flush_tlb_page(addr)		do { } while (0)
#endif /* CONFIG_MMU */

#if defined(CONFIG_SMP) && defined(CONFIG_MMU)
void flush_tlb_all(void);
void flush_tlb_mm(struct mm_struct *mm);
void flush_tlb_page(struct vm_area_struct *vma, unsigned long addr);
void flush_tlb_range(struct vm_area_struct *vma, unsigned long start,
		     unsigned long end);
#ifdef CONFIG_TRANSPARENT_HUGEPAGE
#define __HAVE_ARCH_FLUSH_PMD_TLB_RANGE
void flush_pmd_tlb_range(struct vm_area_struct *vma, unsigned long start,
			unsigned long end);
#endif
#else /* CONFIG_SMP && CONFIG_MMU */
>>>>>>> upstream/android-13

#define flush_tlb_all() local_flush_tlb_all()
#define flush_tlb_page(vma, addr) local_flush_tlb_page(addr)

static inline void flush_tlb_range(struct vm_area_struct *vma,
		unsigned long start, unsigned long end)
{
	local_flush_tlb_all();
}

#define flush_tlb_mm(mm) flush_tlb_all()
<<<<<<< HEAD

#else /* CONFIG_SMP */

#include <asm/sbi.h>

#define flush_tlb_all() sbi_remote_sfence_vma(NULL, 0, -1)
#define flush_tlb_page(vma, addr) flush_tlb_range(vma, addr, 0)
#define flush_tlb_range(vma, start, end) \
	sbi_remote_sfence_vma(mm_cpumask((vma)->vm_mm)->bits, \
			      start, (end) - (start))
#define flush_tlb_mm(mm) \
	sbi_remote_sfence_vma(mm_cpumask(mm)->bits, 0, -1)

#endif /* CONFIG_SMP */
=======
#endif /* !CONFIG_SMP || !CONFIG_MMU */
>>>>>>> upstream/android-13

/* Flush a range of kernel pages */
static inline void flush_tlb_kernel_range(unsigned long start,
	unsigned long end)
{
	flush_tlb_all();
}

#endif /* _ASM_RISCV_TLBFLUSH_H */
