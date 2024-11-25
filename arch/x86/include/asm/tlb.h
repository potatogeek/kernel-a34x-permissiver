/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _ASM_X86_TLB_H
#define _ASM_X86_TLB_H

#define tlb_start_vma(tlb, vma) do { } while (0)
#define tlb_end_vma(tlb, vma) do { } while (0)
<<<<<<< HEAD
#define __tlb_remove_tlb_entry(tlb, ptep, address) do { } while (0)

#define tlb_flush(tlb)							\
{									\
	if (!tlb->fullmm && !tlb->need_flush_all) 			\
		flush_tlb_mm_range(tlb->mm, tlb->start, tlb->end, 0UL);	\
	else								\
		flush_tlb_mm_range(tlb->mm, 0UL, TLB_FLUSH_ALL, 0UL);	\
}

#include <asm-generic/tlb.h>

=======

#define tlb_flush tlb_flush
static inline void tlb_flush(struct mmu_gather *tlb);

#include <asm-generic/tlb.h>

static inline void tlb_flush(struct mmu_gather *tlb)
{
	unsigned long start = 0UL, end = TLB_FLUSH_ALL;
	unsigned int stride_shift = tlb_get_unmap_shift(tlb);

	if (!tlb->fullmm && !tlb->need_flush_all) {
		start = tlb->start;
		end = tlb->end;
	}

	flush_tlb_mm_range(tlb->mm, start, end, stride_shift, tlb->freed_tables);
}

>>>>>>> upstream/android-13
/*
 * While x86 architecture in general requires an IPI to perform TLB
 * shootdown, enablement code for several hypervisors overrides
 * .flush_tlb_others hook in pv_mmu_ops and implements it by issuing
 * a hypercall. To keep software pagetable walkers safe in this case we
<<<<<<< HEAD
 * switch to RCU based table free (HAVE_RCU_TABLE_FREE). See the comment
 * below 'ifdef CONFIG_HAVE_RCU_TABLE_FREE' in include/asm-generic/tlb.h
=======
 * switch to RCU based table free (MMU_GATHER_RCU_TABLE_FREE). See the comment
 * below 'ifdef CONFIG_MMU_GATHER_RCU_TABLE_FREE' in include/asm-generic/tlb.h
>>>>>>> upstream/android-13
 * for more details.
 */
static inline void __tlb_remove_table(void *table)
{
	free_page_and_swap_cache(table);
}

#endif /* _ASM_X86_TLB_H */
