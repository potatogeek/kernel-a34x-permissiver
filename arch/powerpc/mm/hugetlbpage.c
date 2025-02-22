/*
 * PPC Huge TLB Page Support for Kernel.
 *
 * Copyright (C) 2003 David Gibson, IBM Corporation.
 * Copyright (C) 2011 Becky Bruce, Freescale Semiconductor
 *
 * Based on the IA-32 version:
 * Copyright (C) 2002, Rohit Seth <rohit.seth@intel.com>
 */

#include <linux/mm.h>
#include <linux/io.h>
#include <linux/slab.h>
#include <linux/hugetlb.h>
#include <linux/export.h>
#include <linux/of_fdt.h>
#include <linux/memblock.h>
<<<<<<< HEAD
#include <linux/bootmem.h>
=======
>>>>>>> upstream/android-13
#include <linux/moduleparam.h>
#include <linux/swap.h>
#include <linux/swapops.h>
#include <linux/kmemleak.h>
<<<<<<< HEAD
#include <asm/pgtable.h>
=======
>>>>>>> upstream/android-13
#include <asm/pgalloc.h>
#include <asm/tlb.h>
#include <asm/setup.h>
#include <asm/hugetlb.h>
#include <asm/pte-walk.h>

<<<<<<< HEAD

#ifdef CONFIG_HUGETLB_PAGE

#define PAGE_SHIFT_64K	16
#define PAGE_SHIFT_512K	19
#define PAGE_SHIFT_8M	23
#define PAGE_SHIFT_16M	24
#define PAGE_SHIFT_16G	34

bool hugetlb_disabled = false;

unsigned int HPAGE_SHIFT;
EXPORT_SYMBOL(HPAGE_SHIFT);

#define hugepd_none(hpd)	(hpd_val(hpd) == 0)

=======
bool hugetlb_disabled = false;

#define hugepd_none(hpd)	(hpd_val(hpd) == 0)

#define PTE_T_ORDER	(__builtin_ffs(sizeof(pte_basic_t)) - \
			 __builtin_ffs(sizeof(void *)))

>>>>>>> upstream/android-13
pte_t *huge_pte_offset(struct mm_struct *mm, unsigned long addr, unsigned long sz)
{
	/*
	 * Only called for hugetlbfs pages, hence can ignore THP and the
	 * irq disabled walk.
	 */
	return __find_linux_pte(mm->pgd, addr, NULL, NULL);
}

static int __hugepte_alloc(struct mm_struct *mm, hugepd_t *hpdp,
			   unsigned long address, unsigned int pdshift,
			   unsigned int pshift, spinlock_t *ptl)
{
	struct kmem_cache *cachep;
	pte_t *new;
	int i;
	int num_hugepd;

	if (pshift >= pdshift) {
<<<<<<< HEAD
		cachep = hugepte_cache;
=======
		cachep = PGT_CACHE(PTE_T_ORDER);
>>>>>>> upstream/android-13
		num_hugepd = 1 << (pshift - pdshift);
	} else {
		cachep = PGT_CACHE(pdshift - pshift);
		num_hugepd = 1;
	}

<<<<<<< HEAD
	new = kmem_cache_zalloc(cachep, pgtable_gfp_flags(mm, GFP_KERNEL));
=======
	if (!cachep) {
		WARN_ONCE(1, "No page table cache created for hugetlb tables");
		return -ENOMEM;
	}

	new = kmem_cache_alloc(cachep, pgtable_gfp_flags(mm, GFP_KERNEL));
>>>>>>> upstream/android-13

	BUG_ON(pshift > HUGEPD_SHIFT_MASK);
	BUG_ON((unsigned long)new & HUGEPD_SHIFT_MASK);

<<<<<<< HEAD
	if (! new)
=======
	if (!new)
>>>>>>> upstream/android-13
		return -ENOMEM;

	/*
	 * Make sure other cpus find the hugepd set only after a
	 * properly initialized page table is visible to them.
	 * For more details look for comment in __pte_alloc().
	 */
	smp_wmb();

	spin_lock(ptl);
	/*
	 * We have multiple higher-level entries that point to the same
	 * actual pte location.  Fill in each as we go and backtrack on error.
	 * We need all of these so the DTLB pgtable walk code can find the
	 * right higher-level entry without knowing if it's a hugepage or not.
	 */
	for (i = 0; i < num_hugepd; i++, hpdp++) {
		if (unlikely(!hugepd_none(*hpdp)))
			break;
<<<<<<< HEAD
		else {
#ifdef CONFIG_PPC_BOOK3S_64
			*hpdp = __hugepd(__pa(new) |
					 (shift_to_mmu_psize(pshift) << 2));
#elif defined(CONFIG_PPC_8xx)
			*hpdp = __hugepd(__pa(new) | _PMD_USER |
					 (pshift == PAGE_SHIFT_8M ? _PMD_PAGE_8M :
					  _PMD_PAGE_512K) | _PMD_PRESENT);
#else
			/* We use the old format for PPC_FSL_BOOK3E */
			*hpdp = __hugepd(((unsigned long)new & ~PD_HUGE) | pshift);
#endif
		}
=======
		hugepd_populate(hpdp, new, pshift);
>>>>>>> upstream/android-13
	}
	/* If we bailed from the for loop early, an error occurred, clean up */
	if (i < num_hugepd) {
		for (i = i - 1 ; i >= 0; i--, hpdp--)
			*hpdp = __hugepd(0);
		kmem_cache_free(cachep, new);
	} else {
		kmemleak_ignore(new);
	}
	spin_unlock(ptl);
	return 0;
}

/*
 * At this point we do the placement change only for BOOK3S 64. This would
 * possibly work on other subarchs.
 */
<<<<<<< HEAD
pte_t *huge_pte_alloc(struct mm_struct *mm, unsigned long addr, unsigned long sz)
{
	pgd_t *pg;
=======
pte_t *huge_pte_alloc(struct mm_struct *mm, struct vm_area_struct *vma,
		      unsigned long addr, unsigned long sz)
{
	pgd_t *pg;
	p4d_t *p4;
>>>>>>> upstream/android-13
	pud_t *pu;
	pmd_t *pm;
	hugepd_t *hpdp = NULL;
	unsigned pshift = __ffs(sz);
	unsigned pdshift = PGDIR_SHIFT;
	spinlock_t *ptl;

	addr &= ~(sz-1);
	pg = pgd_offset(mm, addr);
<<<<<<< HEAD
=======
	p4 = p4d_offset(pg, addr);
>>>>>>> upstream/android-13

#ifdef CONFIG_PPC_BOOK3S_64
	if (pshift == PGDIR_SHIFT)
		/* 16GB huge page */
<<<<<<< HEAD
		return (pte_t *) pg;
=======
		return (pte_t *) p4;
>>>>>>> upstream/android-13
	else if (pshift > PUD_SHIFT) {
		/*
		 * We need to use hugepd table
		 */
		ptl = &mm->page_table_lock;
<<<<<<< HEAD
		hpdp = (hugepd_t *)pg;
	} else {
		pdshift = PUD_SHIFT;
		pu = pud_alloc(mm, pg, addr);
=======
		hpdp = (hugepd_t *)p4;
	} else {
		pdshift = PUD_SHIFT;
		pu = pud_alloc(mm, p4, addr);
>>>>>>> upstream/android-13
		if (!pu)
			return NULL;
		if (pshift == PUD_SHIFT)
			return (pte_t *)pu;
		else if (pshift > PMD_SHIFT) {
			ptl = pud_lockptr(mm, pu);
			hpdp = (hugepd_t *)pu;
		} else {
			pdshift = PMD_SHIFT;
			pm = pmd_alloc(mm, pu, addr);
			if (!pm)
				return NULL;
			if (pshift == PMD_SHIFT)
				/* 16MB hugepage */
				return (pte_t *)pm;
			else {
				ptl = pmd_lockptr(mm, pm);
				hpdp = (hugepd_t *)pm;
			}
		}
	}
#else
	if (pshift >= PGDIR_SHIFT) {
		ptl = &mm->page_table_lock;
<<<<<<< HEAD
		hpdp = (hugepd_t *)pg;
	} else {
		pdshift = PUD_SHIFT;
		pu = pud_alloc(mm, pg, addr);
=======
		hpdp = (hugepd_t *)p4;
	} else {
		pdshift = PUD_SHIFT;
		pu = pud_alloc(mm, p4, addr);
>>>>>>> upstream/android-13
		if (!pu)
			return NULL;
		if (pshift >= PUD_SHIFT) {
			ptl = pud_lockptr(mm, pu);
			hpdp = (hugepd_t *)pu;
		} else {
			pdshift = PMD_SHIFT;
			pm = pmd_alloc(mm, pu, addr);
			if (!pm)
				return NULL;
			ptl = pmd_lockptr(mm, pm);
			hpdp = (hugepd_t *)pm;
		}
	}
#endif
	if (!hpdp)
		return NULL;

<<<<<<< HEAD
=======
	if (IS_ENABLED(CONFIG_PPC_8xx) && pshift < PMD_SHIFT)
		return pte_alloc_map(mm, (pmd_t *)hpdp, addr);

>>>>>>> upstream/android-13
	BUG_ON(!hugepd_none(*hpdp) && !hugepd_ok(*hpdp));

	if (hugepd_none(*hpdp) && __hugepte_alloc(mm, hpdp, addr,
						  pdshift, pshift, ptl))
		return NULL;

	return hugepte_offset(*hpdp, addr, pdshift);
}

#ifdef CONFIG_PPC_BOOK3S_64
/*
 * Tracks gpages after the device tree is scanned and before the
 * huge_boot_pages list is ready on pseries.
 */
#define MAX_NUMBER_GPAGES	1024
__initdata static u64 gpage_freearray[MAX_NUMBER_GPAGES];
__initdata static unsigned nr_gpages;

/*
 * Build list of addresses of gigantic pages.  This function is used in early
 * boot before the buddy allocator is setup.
 */
void __init pseries_add_gpage(u64 addr, u64 page_size, unsigned long number_of_pages)
{
	if (!addr)
		return;
	while (number_of_pages > 0) {
		gpage_freearray[nr_gpages] = addr;
		nr_gpages++;
		number_of_pages--;
		addr += page_size;
	}
}

<<<<<<< HEAD
int __init pseries_alloc_bootmem_huge_page(struct hstate *hstate)
=======
static int __init pseries_alloc_bootmem_huge_page(struct hstate *hstate)
>>>>>>> upstream/android-13
{
	struct huge_bootmem_page *m;
	if (nr_gpages == 0)
		return 0;
	m = phys_to_virt(gpage_freearray[--nr_gpages]);
	gpage_freearray[nr_gpages] = 0;
	list_add(&m->list, &huge_boot_pages);
	m->hstate = hstate;
	return 1;
}
#endif


int __init alloc_bootmem_huge_page(struct hstate *h)
{

#ifdef CONFIG_PPC_BOOK3S_64
	if (firmware_has_feature(FW_FEATURE_LPAR) && !radix_enabled())
		return pseries_alloc_bootmem_huge_page(h);
#endif
	return __alloc_bootmem_huge_page(h);
}

<<<<<<< HEAD
#if defined(CONFIG_PPC_FSL_BOOK3E) || defined(CONFIG_PPC_8xx)
=======
#ifndef CONFIG_PPC_BOOK3S_64
>>>>>>> upstream/android-13
#define HUGEPD_FREELIST_SIZE \
	((PAGE_SIZE - sizeof(struct hugepd_freelist)) / sizeof(pte_t))

struct hugepd_freelist {
	struct rcu_head	rcu;
	unsigned int index;
<<<<<<< HEAD
	void *ptes[0];
=======
	void *ptes[];
>>>>>>> upstream/android-13
};

static DEFINE_PER_CPU(struct hugepd_freelist *, hugepd_freelist_cur);

static void hugepd_free_rcu_callback(struct rcu_head *head)
{
	struct hugepd_freelist *batch =
		container_of(head, struct hugepd_freelist, rcu);
	unsigned int i;

	for (i = 0; i < batch->index; i++)
<<<<<<< HEAD
		kmem_cache_free(hugepte_cache, batch->ptes[i]);
=======
		kmem_cache_free(PGT_CACHE(PTE_T_ORDER), batch->ptes[i]);
>>>>>>> upstream/android-13

	free_page((unsigned long)batch);
}

static void hugepd_free(struct mmu_gather *tlb, void *hugepte)
{
	struct hugepd_freelist **batchp;

	batchp = &get_cpu_var(hugepd_freelist_cur);

	if (atomic_read(&tlb->mm->mm_users) < 2 ||
	    mm_is_thread_local(tlb->mm)) {
<<<<<<< HEAD
		kmem_cache_free(hugepte_cache, hugepte);
=======
		kmem_cache_free(PGT_CACHE(PTE_T_ORDER), hugepte);
>>>>>>> upstream/android-13
		put_cpu_var(hugepd_freelist_cur);
		return;
	}

	if (*batchp == NULL) {
		*batchp = (struct hugepd_freelist *)__get_free_page(GFP_ATOMIC);
		(*batchp)->index = 0;
	}

	(*batchp)->ptes[(*batchp)->index++] = hugepte;
	if ((*batchp)->index == HUGEPD_FREELIST_SIZE) {
<<<<<<< HEAD
		call_rcu_sched(&(*batchp)->rcu, hugepd_free_rcu_callback);
=======
		call_rcu(&(*batchp)->rcu, hugepd_free_rcu_callback);
>>>>>>> upstream/android-13
		*batchp = NULL;
	}
	put_cpu_var(hugepd_freelist_cur);
}
#else
static inline void hugepd_free(struct mmu_gather *tlb, void *hugepte) {}
#endif

<<<<<<< HEAD
=======
/* Return true when the entry to be freed maps more than the area being freed */
static bool range_is_outside_limits(unsigned long start, unsigned long end,
				    unsigned long floor, unsigned long ceiling,
				    unsigned long mask)
{
	if ((start & mask) < floor)
		return true;
	if (ceiling) {
		ceiling &= mask;
		if (!ceiling)
			return true;
	}
	return end - 1 > ceiling - 1;
}

>>>>>>> upstream/android-13
static void free_hugepd_range(struct mmu_gather *tlb, hugepd_t *hpdp, int pdshift,
			      unsigned long start, unsigned long end,
			      unsigned long floor, unsigned long ceiling)
{
	pte_t *hugepte = hugepd_page(*hpdp);
	int i;

	unsigned long pdmask = ~((1UL << pdshift) - 1);
	unsigned int num_hugepd = 1;
	unsigned int shift = hugepd_shift(*hpdp);

	/* Note: On fsl the hpdp may be the first of several */
	if (shift > pdshift)
		num_hugepd = 1 << (shift - pdshift);

<<<<<<< HEAD
	start &= pdmask;
	if (start < floor)
		return;
	if (ceiling) {
		ceiling &= pdmask;
		if (! ceiling)
			return;
	}
	if (end - 1 > ceiling - 1)
=======
	if (range_is_outside_limits(start, end, floor, ceiling, pdmask))
>>>>>>> upstream/android-13
		return;

	for (i = 0; i < num_hugepd; i++, hpdp++)
		*hpdp = __hugepd(0);

	if (shift >= pdshift)
		hugepd_free(tlb, hugepte);
	else
		pgtable_free_tlb(tlb, hugepte,
				 get_hugepd_cache_index(pdshift - shift));
}

<<<<<<< HEAD
=======
static void hugetlb_free_pte_range(struct mmu_gather *tlb, pmd_t *pmd,
				   unsigned long addr, unsigned long end,
				   unsigned long floor, unsigned long ceiling)
{
	pgtable_t token = pmd_pgtable(*pmd);

	if (range_is_outside_limits(addr, end, floor, ceiling, PMD_MASK))
		return;

	pmd_clear(pmd);
	pte_free_tlb(tlb, token, addr);
	mm_dec_nr_ptes(tlb->mm);
}

>>>>>>> upstream/android-13
static void hugetlb_free_pmd_range(struct mmu_gather *tlb, pud_t *pud,
				   unsigned long addr, unsigned long end,
				   unsigned long floor, unsigned long ceiling)
{
	pmd_t *pmd;
	unsigned long next;
	unsigned long start;

	start = addr;
	do {
		unsigned long more;

		pmd = pmd_offset(pud, addr);
		next = pmd_addr_end(addr, end);
		if (!is_hugepd(__hugepd(pmd_val(*pmd)))) {
<<<<<<< HEAD
=======
			if (pmd_none_or_clear_bad(pmd))
				continue;

>>>>>>> upstream/android-13
			/*
			 * if it is not hugepd pointer, we should already find
			 * it cleared.
			 */
<<<<<<< HEAD
			WARN_ON(!pmd_none_or_clear_bad(pmd));
=======
			WARN_ON(!IS_ENABLED(CONFIG_PPC_8xx));

			hugetlb_free_pte_range(tlb, pmd, addr, end, floor, ceiling);

>>>>>>> upstream/android-13
			continue;
		}
		/*
		 * Increment next by the size of the huge mapping since
		 * there may be more than one entry at this level for a
		 * single hugepage, but all of them point to
		 * the same kmem cache that holds the hugepte.
		 */
		more = addr + (1 << hugepd_shift(*(hugepd_t *)pmd));
		if (more > next)
			next = more;

		free_hugepd_range(tlb, (hugepd_t *)pmd, PMD_SHIFT,
				  addr, next, floor, ceiling);
	} while (addr = next, addr != end);

<<<<<<< HEAD
	start &= PUD_MASK;
	if (start < floor)
		return;
	if (ceiling) {
		ceiling &= PUD_MASK;
		if (!ceiling)
			return;
	}
	if (end - 1 > ceiling - 1)
		return;

	pmd = pmd_offset(pud, start);
	pud_clear(pud);
	pmd_free_tlb(tlb, pmd, start);
	mm_dec_nr_pmds(tlb->mm);
}

static void hugetlb_free_pud_range(struct mmu_gather *tlb, pgd_t *pgd,
=======
	if (range_is_outside_limits(start, end, floor, ceiling, PUD_MASK))
		return;

	pmd = pmd_offset(pud, start & PUD_MASK);
	pud_clear(pud);
	pmd_free_tlb(tlb, pmd, start & PUD_MASK);
	mm_dec_nr_pmds(tlb->mm);
}

static void hugetlb_free_pud_range(struct mmu_gather *tlb, p4d_t *p4d,
>>>>>>> upstream/android-13
				   unsigned long addr, unsigned long end,
				   unsigned long floor, unsigned long ceiling)
{
	pud_t *pud;
	unsigned long next;
	unsigned long start;

	start = addr;
	do {
<<<<<<< HEAD
		pud = pud_offset(pgd, addr);
=======
		pud = pud_offset(p4d, addr);
>>>>>>> upstream/android-13
		next = pud_addr_end(addr, end);
		if (!is_hugepd(__hugepd(pud_val(*pud)))) {
			if (pud_none_or_clear_bad(pud))
				continue;
			hugetlb_free_pmd_range(tlb, pud, addr, next, floor,
					       ceiling);
		} else {
			unsigned long more;
			/*
			 * Increment next by the size of the huge mapping since
			 * there may be more than one entry at this level for a
			 * single hugepage, but all of them point to
			 * the same kmem cache that holds the hugepte.
			 */
			more = addr + (1 << hugepd_shift(*(hugepd_t *)pud));
			if (more > next)
				next = more;

			free_hugepd_range(tlb, (hugepd_t *)pud, PUD_SHIFT,
					  addr, next, floor, ceiling);
		}
	} while (addr = next, addr != end);

<<<<<<< HEAD
	start &= PGDIR_MASK;
	if (start < floor)
		return;
	if (ceiling) {
		ceiling &= PGDIR_MASK;
		if (!ceiling)
			return;
	}
	if (end - 1 > ceiling - 1)
		return;

	pud = pud_offset(pgd, start);
	pgd_clear(pgd);
	pud_free_tlb(tlb, pud, start);
=======
	if (range_is_outside_limits(start, end, floor, ceiling, PGDIR_MASK))
		return;

	pud = pud_offset(p4d, start & PGDIR_MASK);
	p4d_clear(p4d);
	pud_free_tlb(tlb, pud, start & PGDIR_MASK);
>>>>>>> upstream/android-13
	mm_dec_nr_puds(tlb->mm);
}

/*
 * This function frees user-level page tables of a process.
 */
void hugetlb_free_pgd_range(struct mmu_gather *tlb,
			    unsigned long addr, unsigned long end,
			    unsigned long floor, unsigned long ceiling)
{
	pgd_t *pgd;
<<<<<<< HEAD
=======
	p4d_t *p4d;
>>>>>>> upstream/android-13
	unsigned long next;

	/*
	 * Because there are a number of different possible pagetable
	 * layouts for hugepage ranges, we limit knowledge of how
	 * things should be laid out to the allocation path
	 * (huge_pte_alloc(), above).  Everything else works out the
	 * structure as it goes from information in the hugepd
	 * pointers.  That means that we can't here use the
	 * optimization used in the normal page free_pgd_range(), of
	 * checking whether we're actually covering a large enough
	 * range to have to do anything at the top level of the walk
	 * instead of at the bottom.
	 *
	 * To make sense of this, you should probably go read the big
	 * block comment at the top of the normal free_pgd_range(),
	 * too.
	 */

	do {
		next = pgd_addr_end(addr, end);
		pgd = pgd_offset(tlb->mm, addr);
<<<<<<< HEAD
		if (!is_hugepd(__hugepd(pgd_val(*pgd)))) {
			if (pgd_none_or_clear_bad(pgd))
				continue;
			hugetlb_free_pud_range(tlb, pgd, addr, next, floor, ceiling);
=======
		p4d = p4d_offset(pgd, addr);
		if (!is_hugepd(__hugepd(pgd_val(*pgd)))) {
			if (p4d_none_or_clear_bad(p4d))
				continue;
			hugetlb_free_pud_range(tlb, p4d, addr, next, floor, ceiling);
>>>>>>> upstream/android-13
		} else {
			unsigned long more;
			/*
			 * Increment next by the size of the huge mapping since
			 * there may be more than one entry at the pgd level
			 * for a single hugepage, but all of them point to the
			 * same kmem cache that holds the hugepte.
			 */
			more = addr + (1 << hugepd_shift(*(hugepd_t *)pgd));
			if (more > next)
				next = more;

<<<<<<< HEAD
			free_hugepd_range(tlb, (hugepd_t *)pgd, PGDIR_SHIFT,
=======
			free_hugepd_range(tlb, (hugepd_t *)p4d, PGDIR_SHIFT,
>>>>>>> upstream/android-13
					  addr, next, floor, ceiling);
		}
	} while (addr = next, addr != end);
}

struct page *follow_huge_pd(struct vm_area_struct *vma,
			    unsigned long address, hugepd_t hpd,
			    int flags, int pdshift)
{
	pte_t *ptep;
	spinlock_t *ptl;
	struct page *page = NULL;
	unsigned long mask;
	int shift = hugepd_shift(hpd);
	struct mm_struct *mm = vma->vm_mm;

retry:
	/*
	 * hugepage directory entries are protected by mm->page_table_lock
	 * Use this instead of huge_pte_lockptr
	 */
	ptl = &mm->page_table_lock;
	spin_lock(ptl);

	ptep = hugepte_offset(hpd, address, pdshift);
	if (pte_present(*ptep)) {
		mask = (1UL << shift) - 1;
		page = pte_page(*ptep);
		page += ((address & mask) >> PAGE_SHIFT);
		if (flags & FOLL_GET)
			get_page(page);
	} else {
		if (is_hugetlb_entry_migration(*ptep)) {
			spin_unlock(ptl);
			__migration_entry_wait(mm, ptep, ptl);
			goto retry;
		}
	}
	spin_unlock(ptl);
	return page;
}

<<<<<<< HEAD
static unsigned long hugepte_addr_end(unsigned long addr, unsigned long end,
				      unsigned long sz)
{
	unsigned long __boundary = (addr + sz) & ~(sz-1);
	return (__boundary - 1 < end - 1) ? __boundary : end;
}

int gup_huge_pd(hugepd_t hugepd, unsigned long addr, unsigned pdshift,
		unsigned long end, int write, struct page **pages, int *nr)
{
	pte_t *ptep;
	unsigned long sz = 1UL << hugepd_shift(hugepd);
	unsigned long next;

	ptep = hugepte_offset(hugepd, addr, pdshift);
	do {
		next = hugepte_addr_end(addr, end, sz);
		if (!gup_hugepte(ptep, sz, addr, end, write, pages, nr))
			return 0;
	} while (ptep++, addr = next, addr != end);

	return 1;
}

=======
>>>>>>> upstream/android-13
#ifdef CONFIG_PPC_MM_SLICES
unsigned long hugetlb_get_unmapped_area(struct file *file, unsigned long addr,
					unsigned long len, unsigned long pgoff,
					unsigned long flags)
{
	struct hstate *hstate = hstate_file(file);
	int mmu_psize = shift_to_mmu_psize(huge_page_shift(hstate));

#ifdef CONFIG_PPC_RADIX_MMU
	if (radix_enabled())
		return radix__hugetlb_get_unmapped_area(file, addr, len,
						       pgoff, flags);
#endif
	return slice_get_unmapped_area(addr, len, flags, mmu_psize, 1);
}
#endif

unsigned long vma_mmu_pagesize(struct vm_area_struct *vma)
{
<<<<<<< HEAD
#ifdef CONFIG_PPC_MM_SLICES
	/* With radix we don't use slice, so derive it from vma*/
	if (!radix_enabled()) {
=======
	/* With radix we don't use slice, so derive it from vma*/
	if (IS_ENABLED(CONFIG_PPC_MM_SLICES) && !radix_enabled()) {
>>>>>>> upstream/android-13
		unsigned int psize = get_slice_psize(vma->vm_mm, vma->vm_start);

		return 1UL << mmu_psize_to_shift(psize);
	}
<<<<<<< HEAD
#endif
	return vma_kernel_pagesize(vma);
}

static inline bool is_power_of_4(unsigned long x)
{
	if (is_power_of_2(x))
		return (__ilog2(x) % 2) ? false : true;
	return false;
}

static int __init add_huge_page_size(unsigned long long size)
=======
	return vma_kernel_pagesize(vma);
}

bool __init arch_hugetlb_valid_size(unsigned long size)
>>>>>>> upstream/android-13
{
	int shift = __ffs(size);
	int mmu_psize;

	/* Check that it is a page size supported by the hardware and
	 * that it fits within pagetable and slice limits. */
<<<<<<< HEAD
	if (size <= PAGE_SIZE)
		return -EINVAL;
#if defined(CONFIG_PPC_FSL_BOOK3E)
	if (!is_power_of_4(size))
		return -EINVAL;
#elif !defined(CONFIG_PPC_8xx)
	if (!is_power_of_2(size) || (shift > SLICE_HIGH_SHIFT))
		return -EINVAL;
#endif

	if ((mmu_psize = shift_to_mmu_psize(shift)) < 0)
		return -EINVAL;

#ifdef CONFIG_PPC_BOOK3S_64
	/*
	 * We need to make sure that for different page sizes reported by
	 * firmware we only add hugetlb support for page sizes that can be
	 * supported by linux page table layout.
	 * For now we have
	 * Radix: 2M and 1G
	 * Hash: 16M and 16G
	 */
	if (radix_enabled()) {
		if (mmu_psize != MMU_PAGE_2M && mmu_psize != MMU_PAGE_1G)
			return -EINVAL;
	} else {
		if (mmu_psize != MMU_PAGE_16M && mmu_psize != MMU_PAGE_16G)
			return -EINVAL;
	}
#endif

	BUG_ON(mmu_psize_defs[mmu_psize].shift != shift);

	/* Return if huge page size has already been setup */
	if (size_to_hstate(size))
		return 0;

	hugetlb_add_hstate(shift - PAGE_SHIFT);

	return 0;
}

static int __init hugepage_setup_sz(char *str)
{
	unsigned long long size;

	size = memparse(str, &str);

	if (add_huge_page_size(size) != 0) {
		hugetlb_bad_size();
		pr_err("Invalid huge page size specified(%llu)\n", size);
	}

	return 1;
}
__setup("hugepagesz=", hugepage_setup_sz);

struct kmem_cache *hugepte_cache;
static int __init hugetlbpage_init(void)
{
=======
	if (size <= PAGE_SIZE || !is_power_of_2(size))
		return false;

	mmu_psize = check_and_get_huge_psize(shift);
	if (mmu_psize < 0)
		return false;

	BUG_ON(mmu_psize_defs[mmu_psize].shift != shift);

	return true;
}

static int __init add_huge_page_size(unsigned long long size)
{
	int shift = __ffs(size);

	if (!arch_hugetlb_valid_size((unsigned long)size))
		return -EINVAL;

	hugetlb_add_hstate(shift - PAGE_SHIFT);
	return 0;
}

static int __init hugetlbpage_init(void)
{
	bool configured = false;
>>>>>>> upstream/android-13
	int psize;

	if (hugetlb_disabled) {
		pr_info("HugeTLB support is disabled!\n");
		return 0;
	}

<<<<<<< HEAD
#if !defined(CONFIG_PPC_FSL_BOOK3E) && !defined(CONFIG_PPC_8xx)
	if (!radix_enabled() && !mmu_has_feature(MMU_FTR_16M_PAGE))
		return -ENODEV;
#endif
=======
	if (IS_ENABLED(CONFIG_PPC_BOOK3S_64) && !radix_enabled() &&
	    !mmu_has_feature(MMU_FTR_16M_PAGE))
		return -ENODEV;

>>>>>>> upstream/android-13
	for (psize = 0; psize < MMU_PAGE_COUNT; ++psize) {
		unsigned shift;
		unsigned pdshift;

		if (!mmu_psize_defs[psize].shift)
			continue;

		shift = mmu_psize_to_shift(psize);

#ifdef CONFIG_PPC_BOOK3S_64
		if (shift > PGDIR_SHIFT)
			continue;
		else if (shift > PUD_SHIFT)
			pdshift = PGDIR_SHIFT;
		else if (shift > PMD_SHIFT)
			pdshift = PUD_SHIFT;
		else
			pdshift = PMD_SHIFT;
#else
		if (shift < PUD_SHIFT)
			pdshift = PMD_SHIFT;
		else if (shift < PGDIR_SHIFT)
			pdshift = PUD_SHIFT;
		else
			pdshift = PGDIR_SHIFT;
#endif

		if (add_huge_page_size(1ULL << shift) < 0)
			continue;
		/*
		 * if we have pdshift and shift value same, we don't
		 * use pgt cache for hugepd.
		 */
<<<<<<< HEAD
		if (pdshift > shift)
			pgtable_cache_add(pdshift - shift, NULL);
#if defined(CONFIG_PPC_FSL_BOOK3E) || defined(CONFIG_PPC_8xx)
		else if (!hugepte_cache) {
			/*
			 * Create a kmem cache for hugeptes.  The bottom bits in
			 * the pte have size information encoded in them, so
			 * align them to allow this
			 */
			hugepte_cache = kmem_cache_create("hugepte-cache",
							  sizeof(pte_t),
							  HUGEPD_SHIFT_MASK + 1,
							  0, NULL);
			if (hugepte_cache == NULL)
				panic("%s: Unable to create kmem cache "
				      "for hugeptes\n", __func__);

		}
#endif
	}

#if defined(CONFIG_PPC_FSL_BOOK3E) || defined(CONFIG_PPC_8xx)
	/* Default hpage size = 4M on FSL_BOOK3E and 512k on 8xx */
	if (mmu_psize_defs[MMU_PAGE_4M].shift)
		HPAGE_SHIFT = mmu_psize_defs[MMU_PAGE_4M].shift;
	else if (mmu_psize_defs[MMU_PAGE_512K].shift)
		HPAGE_SHIFT = mmu_psize_defs[MMU_PAGE_512K].shift;
#else
	/* Set default large page size. Currently, we pick 16M or 1M
	 * depending on what is available
	 */
	if (mmu_psize_defs[MMU_PAGE_16M].shift)
		HPAGE_SHIFT = mmu_psize_defs[MMU_PAGE_16M].shift;
	else if (mmu_psize_defs[MMU_PAGE_1M].shift)
		HPAGE_SHIFT = mmu_psize_defs[MMU_PAGE_1M].shift;
	else if (mmu_psize_defs[MMU_PAGE_2M].shift)
		HPAGE_SHIFT = mmu_psize_defs[MMU_PAGE_2M].shift;
#endif
=======
		if (pdshift > shift) {
			if (!IS_ENABLED(CONFIG_PPC_8xx))
				pgtable_cache_add(pdshift - shift);
		} else if (IS_ENABLED(CONFIG_PPC_FSL_BOOK3E) ||
			   IS_ENABLED(CONFIG_PPC_8xx)) {
			pgtable_cache_add(PTE_T_ORDER);
		}

		configured = true;
	}

	if (configured) {
		if (IS_ENABLED(CONFIG_HUGETLB_PAGE_SIZE_VARIABLE))
			hugetlbpage_init_default();
	} else
		pr_info("Failed to initialize. Disabling HugeTLB");

>>>>>>> upstream/android-13
	return 0;
}

arch_initcall(hugetlbpage_init);

<<<<<<< HEAD
void flush_dcache_icache_hugepage(struct page *page)
{
	int i;
	void *start;

	BUG_ON(!PageCompound(page));

	for (i = 0; i < (1UL << compound_order(page)); i++) {
		if (!PageHighMem(page)) {
			__flush_dcache_icache(page_address(page+i));
		} else {
			start = kmap_atomic(page+i);
			__flush_dcache_icache(start);
			kunmap_atomic(start);
		}
	}
}

#endif /* CONFIG_HUGETLB_PAGE */

/*
 * We have 4 cases for pgds and pmds:
 * (1) invalid (all zeroes)
 * (2) pointer to next table, as normal; bottom 6 bits == 0
 * (3) leaf pte for huge page _PAGE_PTE set
 * (4) hugepd pointer, _PAGE_PTE = 0 and bits [2..6] indicate size of table
 *
 * So long as we atomically load page table pointers we are safe against teardown,
 * we can follow the address down to the the page and take a ref on it.
 * This function need to be called with interrupts disabled. We use this variant
 * when we have MSR[EE] = 0 but the paca->irq_soft_mask = IRQS_ENABLED
 */
pte_t *__find_linux_pte(pgd_t *pgdir, unsigned long ea,
			bool *is_thp, unsigned *hpage_shift)
{
	pgd_t pgd, *pgdp;
	pud_t pud, *pudp;
	pmd_t pmd, *pmdp;
	pte_t *ret_pte;
	hugepd_t *hpdp = NULL;
	unsigned pdshift = PGDIR_SHIFT;

	if (hpage_shift)
		*hpage_shift = 0;

	if (is_thp)
		*is_thp = false;

	pgdp = pgdir + pgd_index(ea);
	pgd  = READ_ONCE(*pgdp);
	/*
	 * Always operate on the local stack value. This make sure the
	 * value don't get updated by a parallel THP split/collapse,
	 * page fault or a page unmap. The return pte_t * is still not
	 * stable. So should be checked there for above conditions.
	 */
	if (pgd_none(pgd))
		return NULL;
	else if (pgd_huge(pgd)) {
		ret_pte = (pte_t *) pgdp;
		goto out;
	} else if (is_hugepd(__hugepd(pgd_val(pgd))))
		hpdp = (hugepd_t *)&pgd;
	else {
		/*
		 * Even if we end up with an unmap, the pgtable will not
		 * be freed, because we do an rcu free and here we are
		 * irq disabled
		 */
		pdshift = PUD_SHIFT;
		pudp = pud_offset(&pgd, ea);
		pud  = READ_ONCE(*pudp);

		if (pud_none(pud))
			return NULL;
		else if (pud_huge(pud)) {
			ret_pte = (pte_t *) pudp;
			goto out;
		} else if (is_hugepd(__hugepd(pud_val(pud))))
			hpdp = (hugepd_t *)&pud;
		else {
			pdshift = PMD_SHIFT;
			pmdp = pmd_offset(&pud, ea);
			pmd  = READ_ONCE(*pmdp);
			/*
			 * A hugepage collapse is captured by pmd_none, because
			 * it mark the pmd none and do a hpte invalidate.
			 */
			if (pmd_none(pmd))
				return NULL;

			if (pmd_trans_huge(pmd) || pmd_devmap(pmd)) {
				if (is_thp)
					*is_thp = true;
				ret_pte = (pte_t *) pmdp;
				goto out;
			}

			if (pmd_huge(pmd)) {
				ret_pte = (pte_t *) pmdp;
				goto out;
			} else if (is_hugepd(__hugepd(pmd_val(pmd))))
				hpdp = (hugepd_t *)&pmd;
			else
				return pte_offset_kernel(&pmd, ea);
		}
	}
	if (!hpdp)
		return NULL;

	ret_pte = hugepte_offset(*hpdp, ea, pdshift);
	pdshift = hugepd_shift(*hpdp);
out:
	if (hpage_shift)
		*hpage_shift = pdshift;
	return ret_pte;
}
EXPORT_SYMBOL_GPL(__find_linux_pte);

int gup_hugepte(pte_t *ptep, unsigned long sz, unsigned long addr,
		unsigned long end, int write, struct page **pages, int *nr)
{
	unsigned long pte_end;
	struct page *head, *page;
	pte_t pte;
	int refs;

	pte_end = (addr + sz) & ~(sz-1);
	if (pte_end < end)
		end = pte_end;

	pte = READ_ONCE(*ptep);

	if (!pte_access_permitted(pte, write))
		return 0;

	/* hugepages are never "special" */
	VM_BUG_ON(!pfn_valid(pte_pfn(pte)));

	refs = 0;
	head = pte_page(pte);

	page = head + ((addr & (sz-1)) >> PAGE_SHIFT);
	do {
		VM_BUG_ON(compound_head(page) != head);
		pages[*nr] = page;
		(*nr)++;
		page++;
		refs++;
	} while (addr += PAGE_SIZE, addr != end);

	if (!page_cache_add_speculative(head, refs)) {
		*nr -= refs;
		return 0;
	}

	if (unlikely(pte_val(pte) != pte_val(*ptep))) {
		/* Could be optimized better */
		*nr -= refs;
		while (refs--)
			put_page(head);
		return 0;
	}

	return 1;
=======
void __init gigantic_hugetlb_cma_reserve(void)
{
	unsigned long order = 0;

	if (radix_enabled())
		order = PUD_SHIFT - PAGE_SHIFT;
	else if (!firmware_has_feature(FW_FEATURE_LPAR) && mmu_psize_defs[MMU_PAGE_16G].shift)
		/*
		 * For pseries we do use ibm,expected#pages for reserving 16G pages.
		 */
		order = mmu_psize_to_shift(MMU_PAGE_16G) - PAGE_SHIFT;

	if (order) {
		VM_WARN_ON(order < MAX_ORDER);
		hugetlb_cma_reserve(order);
	}
>>>>>>> upstream/android-13
}
