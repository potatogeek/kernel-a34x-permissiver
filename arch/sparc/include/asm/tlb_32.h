/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _SPARC_TLB_H
#define _SPARC_TLB_H

<<<<<<< HEAD
#define tlb_start_vma(tlb, vma) \
do {								\
	flush_cache_range(vma, vma->vm_start, vma->vm_end);	\
} while (0)

#define tlb_end_vma(tlb, vma) \
do {								\
	flush_tlb_range(vma, vma->vm_start, vma->vm_end);	\
} while (0)

#define __tlb_remove_tlb_entry(tlb, pte, address) \
	do { } while (0)

#define tlb_flush(tlb) \
do {								\
	flush_tlb_mm((tlb)->mm);				\
} while (0)

=======
>>>>>>> upstream/android-13
#include <asm-generic/tlb.h>

#endif /* _SPARC_TLB_H */
