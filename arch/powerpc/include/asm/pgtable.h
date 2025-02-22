/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _ASM_POWERPC_PGTABLE_H
#define _ASM_POWERPC_PGTABLE_H

#ifndef __ASSEMBLY__
#include <linux/mmdebug.h>
#include <linux/mmzone.h>
#include <asm/processor.h>		/* For TASK_SIZE */
#include <asm/mmu.h>
#include <asm/page.h>
#include <asm/tlbflush.h>

struct mm_struct;

#endif /* !__ASSEMBLY__ */

#ifdef CONFIG_PPC_BOOK3S
#include <asm/book3s/pgtable.h>
#else
#include <asm/nohash/pgtable.h>
#endif /* !CONFIG_PPC_BOOK3S */

<<<<<<< HEAD
#ifndef __ASSEMBLY__

#include <asm/tlbflush.h>
=======
/* Note due to the way vm flags are laid out, the bits are XWR */
#define __P000	PAGE_NONE
#define __P001	PAGE_READONLY
#define __P010	PAGE_COPY
#define __P011	PAGE_COPY
#define __P100	PAGE_READONLY_X
#define __P101	PAGE_READONLY_X
#define __P110	PAGE_COPY_X
#define __P111	PAGE_COPY_X

#define __S000	PAGE_NONE
#define __S001	PAGE_READONLY
#define __S010	PAGE_SHARED
#define __S011	PAGE_SHARED
#define __S100	PAGE_READONLY_X
#define __S101	PAGE_READONLY_X
#define __S110	PAGE_SHARED_X
#define __S111	PAGE_SHARED_X

#ifndef __ASSEMBLY__

#ifndef MAX_PTRS_PER_PGD
#define MAX_PTRS_PER_PGD PTRS_PER_PGD
#endif
>>>>>>> upstream/android-13

/* Keep these as a macros to avoid include dependency mess */
#define pte_page(x)		pfn_to_page(pte_pfn(x))
#define mk_pte(page, pgprot)	pfn_pte(page_to_pfn(page), (pgprot))
<<<<<<< HEAD

=======
/*
 * Select all bits except the pfn
 */
static inline pgprot_t pte_pgprot(pte_t pte)
{
	unsigned long pte_flags;

	pte_flags = pte_val(pte) & ~PTE_RPN_MASK;
	return __pgprot(pte_flags);
}

#ifndef pmd_page_vaddr
static inline unsigned long pmd_page_vaddr(pmd_t pmd)
{
	return ((unsigned long)__va(pmd_val(pmd) & ~PMD_MASKED_BITS));
}
#define pmd_page_vaddr pmd_page_vaddr
#endif
>>>>>>> upstream/android-13
/*
 * ZERO_PAGE is a global shared page that is always zero: used
 * for zero-mapped memory areas etc..
 */
extern unsigned long empty_zero_page[];
#define ZERO_PAGE(vaddr) (virt_to_page(empty_zero_page))

extern pgd_t swapper_pg_dir[];

<<<<<<< HEAD
void limit_zone_pfn(enum zone_type zone, unsigned long max_pfn);
int dma_pfn_limit_to_zone(u64 pfn_limit);
extern void paging_init(void);
=======
extern void paging_init(void);
void poking_init(void);

extern unsigned long ioremap_bot;
>>>>>>> upstream/android-13

/*
 * kern_addr_valid is intended to indicate whether an address is a valid
 * kernel address.  Most 32-bit archs define it as always true (like this)
 * but most 64-bit archs actually perform a test.  What should we do here?
 */
#define kern_addr_valid(addr)	(1)

<<<<<<< HEAD
#include <asm-generic/pgtable.h>


/*
 * This gets called at the end of handling a page fault, when
 * the kernel has put a new PTE into the page table for the process.
 * We use it to ensure coherency between the i-cache and d-cache
 * for the page which has just been mapped in.
 * On machines which use an MMU hash table, we use this to put a
 * corresponding HPTE into the hash table ahead of time, instead of
 * waiting for the inevitable extra hash-table miss exception.
 */
extern void update_mmu_cache(struct vm_area_struct *, unsigned long, pte_t *);

extern int gup_hugepte(pte_t *ptep, unsigned long sz, unsigned long addr,
		       unsigned long end, int write,
		       struct page **pages, int *nr);
=======
>>>>>>> upstream/android-13
#ifndef CONFIG_TRANSPARENT_HUGEPAGE
#define pmd_large(pmd)		0
#endif

/* can we use this in kvm */
unsigned long vmalloc_to_phys(void *vmalloc_addr);

<<<<<<< HEAD
void pgtable_cache_add(unsigned shift, void (*ctor)(void *));
void pgtable_cache_init(void);
=======
void pgtable_cache_add(unsigned int shift);

pte_t *early_pte_alloc_kernel(pmd_t *pmdp, unsigned long va);
>>>>>>> upstream/android-13

#if defined(CONFIG_STRICT_KERNEL_RWX) || defined(CONFIG_PPC32)
void mark_initmem_nx(void);
#else
static inline void mark_initmem_nx(void) { }
#endif

<<<<<<< HEAD
=======
/*
 * When used, PTE_FRAG_NR is defined in subarch pgtable.h
 * so we are sure it is included when arriving here.
 */
#ifdef PTE_FRAG_NR
static inline void *pte_frag_get(mm_context_t *ctx)
{
	return ctx->pte_frag;
}

static inline void pte_frag_set(mm_context_t *ctx, void *p)
{
	ctx->pte_frag = p;
}
#else
#define PTE_FRAG_NR		1
#define PTE_FRAG_SIZE_SHIFT	PAGE_SHIFT
#define PTE_FRAG_SIZE		(1UL << PTE_FRAG_SIZE_SHIFT)

static inline void *pte_frag_get(mm_context_t *ctx)
{
	return NULL;
}

static inline void pte_frag_set(mm_context_t *ctx, void *p)
{
}
#endif

#ifndef pmd_is_leaf
#define pmd_is_leaf pmd_is_leaf
static inline bool pmd_is_leaf(pmd_t pmd)
{
	return false;
}
#endif

#ifndef pud_is_leaf
#define pud_is_leaf pud_is_leaf
static inline bool pud_is_leaf(pud_t pud)
{
	return false;
}
#endif

#ifndef p4d_is_leaf
#define p4d_is_leaf p4d_is_leaf
static inline bool p4d_is_leaf(p4d_t p4d)
{
	return false;
}
#endif

#define pmd_pgtable pmd_pgtable
static inline pgtable_t pmd_pgtable(pmd_t pmd)
{
	return (pgtable_t)pmd_page_vaddr(pmd);
}

#ifdef CONFIG_PPC64
#define is_ioremap_addr is_ioremap_addr
static inline bool is_ioremap_addr(const void *x)
{
	unsigned long addr = (unsigned long)x;

	return addr >= IOREMAP_BASE && addr < IOREMAP_END;
}

struct seq_file;
void arch_report_meminfo(struct seq_file *m);
#endif /* CONFIG_PPC64 */

>>>>>>> upstream/android-13
#endif /* __ASSEMBLY__ */

#endif /* _ASM_POWERPC_PGTABLE_H */
