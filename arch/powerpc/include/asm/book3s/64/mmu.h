/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _ASM_POWERPC_BOOK3S_64_MMU_H_
#define _ASM_POWERPC_BOOK3S_64_MMU_H_

<<<<<<< HEAD
=======
#include <asm/page.h>

>>>>>>> upstream/android-13
#ifndef __ASSEMBLY__
/*
 * Page size definition
 *
 *    shift : is the "PAGE_SHIFT" value for that page size
 *    sllp  : is a bit mask with the value of SLB L || LP to be or'ed
 *            directly to a slbmte "vsid" value
 *    penc  : is the HPTE encoding mask for the "LP" field:
 *
 */
struct mmu_psize_def {
	unsigned int	shift;	/* number of bits */
	int		penc[MMU_PAGE_COUNT];	/* HPTE encoding */
	unsigned int	tlbiel;	/* tlbiel supported for that page size */
	unsigned long	avpnm;	/* bits to mask out in AVPN in the HPTE */
<<<<<<< HEAD
=======
	unsigned long   h_rpt_pgsize; /* H_RPT_INVALIDATE page size encoding */
>>>>>>> upstream/android-13
	union {
		unsigned long	sllp;	/* SLB L||LP (exact mask to use in slbmte) */
		unsigned long ap;	/* Ap encoding used by PowerISA 3.0 */
	};
};
extern struct mmu_psize_def mmu_psize_defs[MMU_PAGE_COUNT];
<<<<<<< HEAD

=======
>>>>>>> upstream/android-13
#endif /* __ASSEMBLY__ */

/* 64-bit classic hash table MMU */
#include <asm/book3s/64/mmu-hash.h>

#ifndef __ASSEMBLY__
/*
 * ISA 3.0 partition and process table entry format
 */
struct prtb_entry {
	__be64 prtb0;
	__be64 prtb1;
};
extern struct prtb_entry *process_tb;

struct patb_entry {
	__be64 patb0;
	__be64 patb1;
};
extern struct patb_entry *partition_tb;

/* Bits in patb0 field */
#define PATB_HR		(1UL << 63)
#define RPDB_MASK	0x0fffffffffffff00UL
#define RPDB_SHIFT	(1UL << 8)
#define RTS1_SHIFT	61		/* top 2 bits of radix tree size */
#define RTS1_MASK	(3UL << RTS1_SHIFT)
#define RTS2_SHIFT	5		/* bottom 3 bits of radix tree size */
#define RTS2_MASK	(7UL << RTS2_SHIFT)
#define RPDS_MASK	0x1f		/* root page dir. size field */

/* Bits in patb1 field */
#define PATB_GR		(1UL << 63)	/* guest uses radix; must match HR */
#define PRTS_MASK	0x1f		/* process table size field */
#define PRTB_MASK	0x0ffffffffffff000UL

/* Number of supported PID bits */
extern unsigned int mmu_pid_bits;

/* Base PID to allocate from */
extern unsigned int mmu_base_pid;

<<<<<<< HEAD
=======
/*
 * memory block size used with radix translation.
 */
extern unsigned long __ro_after_init radix_mem_block_size;

>>>>>>> upstream/android-13
#define PRTB_SIZE_SHIFT	(mmu_pid_bits + 4)
#define PRTB_ENTRIES	(1ul << mmu_pid_bits)

/*
 * Power9 currently only support 64K partition table size.
 */
#define PATB_SIZE_SHIFT	16

typedef unsigned long mm_context_id_t;
struct spinlock;

/* Maximum possible number of NPUs in a system. */
#define NV_MAX_NPUS 8

<<<<<<< HEAD
/*
 * One bit per slice. We have lower slices which cover 256MB segments
 * upto 4G range. That gets us 16 low slices. For the rest we track slices
 * in 1TB size.
 */
struct slice_mask {
	u64 low_slices;
	DECLARE_BITMAP(high_slices, SLICE_NUM_HIGH);
};

=======
>>>>>>> upstream/android-13
typedef struct {
	union {
		/*
		 * We use id as the PIDR content for radix. On hash we can use
		 * more than one id. The extended ids are used when we start
		 * having address above 512TB. We allocate one extended id
		 * for each 512TB. The new id is then used with the 49 bit
		 * EA to build a new VA. We always use ESID_BITS_1T_MASK bits
		 * from EA and new context ids to build the new VAs.
		 */
		mm_context_id_t id;
		mm_context_id_t extended_id[TASK_SIZE_USER64/TASK_CONTEXT_SIZE];
	};
<<<<<<< HEAD
	u16 user_psize;		/* page size index */
=======
>>>>>>> upstream/android-13

	/* Number of bits in the mm_cpumask */
	atomic_t active_cpus;

	/* Number of users of the external (Nest) MMU */
	atomic_t copros;

<<<<<<< HEAD
	/* NPU NMMU context */
	struct npu_context *npu_context;

#ifdef CONFIG_PPC_MM_SLICES
	 /* SLB page size encodings*/
	unsigned char low_slices_psize[BITS_PER_LONG / BITS_PER_BYTE];
	unsigned char high_slices_psize[SLICE_ARRAY_SIZE];
	unsigned long slb_addr_limit;
# ifdef CONFIG_PPC_64K_PAGES
	struct slice_mask mask_64k;
# endif
	struct slice_mask mask_4k;
# ifdef CONFIG_HUGETLB_PAGE
	struct slice_mask mask_16m;
	struct slice_mask mask_16g;
# endif
#else
	u16 sllp;		/* SLB page size encoding */
#endif
	unsigned long vdso_base;
#ifdef CONFIG_PPC_SUBPAGE_PROT
	struct subpage_prot_table spt;
#endif /* CONFIG_PPC_SUBPAGE_PROT */
=======
	/* Number of user space windows opened in process mm_context */
	atomic_t vas_windows;

	struct hash_mm_context *hash_context;

	void __user *vdso;
>>>>>>> upstream/android-13
	/*
	 * pagetable fragment support
	 */
	void *pte_frag;
	void *pmd_frag;
#ifdef CONFIG_SPAPR_TCE_IOMMU
	struct list_head iommu_group_mem_list;
#endif

#ifdef CONFIG_PPC_MEM_KEYS
	/*
	 * Each bit represents one protection key.
	 * bit set   -> key allocated
	 * bit unset -> key available for allocation
	 */
	u32 pkey_allocation_map;
	s16 execute_only_pkey; /* key holding execute-only protection */
#endif
} mm_context_t;

<<<<<<< HEAD
=======
static inline u16 mm_ctx_user_psize(mm_context_t *ctx)
{
	return ctx->hash_context->user_psize;
}

static inline void mm_ctx_set_user_psize(mm_context_t *ctx, u16 user_psize)
{
	ctx->hash_context->user_psize = user_psize;
}

static inline unsigned char *mm_ctx_low_slices(mm_context_t *ctx)
{
	return ctx->hash_context->low_slices_psize;
}

static inline unsigned char *mm_ctx_high_slices(mm_context_t *ctx)
{
	return ctx->hash_context->high_slices_psize;
}

static inline unsigned long mm_ctx_slb_addr_limit(mm_context_t *ctx)
{
	return ctx->hash_context->slb_addr_limit;
}

static inline void mm_ctx_set_slb_addr_limit(mm_context_t *ctx, unsigned long limit)
{
	ctx->hash_context->slb_addr_limit = limit;
}

static inline struct slice_mask *slice_mask_for_size(mm_context_t *ctx, int psize)
{
#ifdef CONFIG_PPC_64K_PAGES
	if (psize == MMU_PAGE_64K)
		return &ctx->hash_context->mask_64k;
#endif
#ifdef CONFIG_HUGETLB_PAGE
	if (psize == MMU_PAGE_16M)
		return &ctx->hash_context->mask_16m;
	if (psize == MMU_PAGE_16G)
		return &ctx->hash_context->mask_16g;
#endif
	BUG_ON(psize != MMU_PAGE_4K);

	return &ctx->hash_context->mask_4k;
}

#ifdef CONFIG_PPC_SUBPAGE_PROT
static inline struct subpage_prot_table *mm_ctx_subpage_prot(mm_context_t *ctx)
{
	return ctx->hash_context->spt;
}
#endif

>>>>>>> upstream/android-13
/*
 * The current system page and segment sizes
 */
extern int mmu_linear_psize;
extern int mmu_virtual_psize;
extern int mmu_vmalloc_psize;
extern int mmu_vmemmap_psize;
extern int mmu_io_psize;

/* MMU initialization */
void mmu_early_init_devtree(void);
void hash__early_init_devtree(void);
void radix__early_init_devtree(void);
<<<<<<< HEAD
extern void radix_init_native(void);
extern void hash__early_init_mmu(void);
extern void radix__early_init_mmu(void);
static inline void early_init_mmu(void)
=======
#ifdef CONFIG_PPC_PKEY
void pkey_early_init_devtree(void);
#else
static inline void pkey_early_init_devtree(void) {}
#endif

extern void hash__early_init_mmu(void);
extern void radix__early_init_mmu(void);
static inline void __init early_init_mmu(void)
>>>>>>> upstream/android-13
{
	if (radix_enabled())
		return radix__early_init_mmu();
	return hash__early_init_mmu();
}
extern void hash__early_init_mmu_secondary(void);
extern void radix__early_init_mmu_secondary(void);
static inline void early_init_mmu_secondary(void)
{
	if (radix_enabled())
		return radix__early_init_mmu_secondary();
	return hash__early_init_mmu_secondary();
}

extern void hash__setup_initial_memory_limit(phys_addr_t first_memblock_base,
					 phys_addr_t first_memblock_size);
<<<<<<< HEAD
extern void radix__setup_initial_memory_limit(phys_addr_t first_memblock_base,
					 phys_addr_t first_memblock_size);
static inline void setup_initial_memory_limit(phys_addr_t first_memblock_base,
					      phys_addr_t first_memblock_size)
{
	if (early_radix_enabled())
		return radix__setup_initial_memory_limit(first_memblock_base,
						   first_memblock_size);
=======
static inline void setup_initial_memory_limit(phys_addr_t first_memblock_base,
					      phys_addr_t first_memblock_size)
{
	/*
	 * Hash has more strict restrictions. At this point we don't
	 * know which translations we will pick. Hence go with hash
	 * restrictions.
	 */
>>>>>>> upstream/android-13
	return hash__setup_initial_memory_limit(first_memblock_base,
					   first_memblock_size);
}

<<<<<<< HEAD
extern int (*register_process_table)(unsigned long base, unsigned long page_size,
				     unsigned long tbl_size);

#ifdef CONFIG_PPC_PSERIES
extern void radix_init_pseries(void);
#else
static inline void radix_init_pseries(void) { };
#endif

static inline int get_ea_context(mm_context_t *ctx, unsigned long ea)
=======
#ifdef CONFIG_PPC_PSERIES
extern void radix_init_pseries(void);
#else
static inline void radix_init_pseries(void) { }
#endif

#ifdef CONFIG_HOTPLUG_CPU
#define arch_clear_mm_cpumask_cpu(cpu, mm)				\
	do {								\
		if (cpumask_test_cpu(cpu, mm_cpumask(mm))) {		\
			atomic_dec(&(mm)->context.active_cpus);		\
			cpumask_clear_cpu(cpu, mm_cpumask(mm));		\
		}							\
	} while (0)

void cleanup_cpu_mmu_context(void);
#endif

static inline int get_user_context(mm_context_t *ctx, unsigned long ea)
>>>>>>> upstream/android-13
{
	int index = ea >> MAX_EA_BITS_PER_CONTEXT;

	if (likely(index < ARRAY_SIZE(ctx->extended_id)))
		return ctx->extended_id[index];

	/* should never happen */
	WARN_ON(1);
	return 0;
}

static inline unsigned long get_user_vsid(mm_context_t *ctx,
					  unsigned long ea, int ssize)
{
<<<<<<< HEAD
	unsigned long context = get_ea_context(ctx, ea);
=======
	unsigned long context = get_user_context(ctx, ea);
>>>>>>> upstream/android-13

	return get_vsid(context, ea, ssize);
}

#endif /* __ASSEMBLY__ */
#endif /* _ASM_POWERPC_BOOK3S_64_MMU_H_ */
