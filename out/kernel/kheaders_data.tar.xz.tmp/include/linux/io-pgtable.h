/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __IO_PGTABLE_H
#define __IO_PGTABLE_H
#include <linux/bitops.h>

#include <linux/scatterlist.h>


enum io_pgtable_fmt {
	ARM_32_LPAE_S1,
	ARM_32_LPAE_S2,
	ARM_64_LPAE_S1,
	ARM_64_LPAE_S2,
	ARM_V7S,
	ARM_V8L_FAST,
	IO_PGTABLE_NUM_FMTS,
};


struct iommu_gather_ops {
	void (*tlb_flush_all)(void *cookie);
	void (*tlb_add_flush)(unsigned long iova, size_t size, size_t granule,
			      bool leaf, void *cookie);
	void (*tlb_sync)(void *cookie);
	void *(*alloc_pages_exact)(void *cookie, size_t size, gfp_t gfp_mask);
	void (*free_pages_exact)(void *cookie, void *virt, size_t size);
};


struct io_pgtable_cfg {
	
	#define IO_PGTABLE_QUIRK_ARM_NS		BIT(0)
	#define IO_PGTABLE_QUIRK_NO_PERMS	BIT(1)
	#define IO_PGTABLE_QUIRK_TLBI_ON_MAP	BIT(2)
	#define IO_PGTABLE_QUIRK_ARM_MTK_4GB	BIT(3)
	#define IO_PGTABLE_QUIRK_NO_DMA		BIT(4)
	#define IO_PGTABLE_QUIRK_QSMMUV500_NON_SHAREABLE BIT(5)
	#define IO_PGTABLE_QUIRK_QCOM_USE_UPSTREAM_HINT	BIT(6)
	#define IO_PGTABLE_QUIRK_QCOM_USE_LLC_NWA	BIT(7)
	unsigned long			quirks;
	unsigned long			pgsize_bitmap;
	unsigned int			ias;
	unsigned int			oas;
	const struct iommu_gather_ops	*tlb;
	struct device			*iommu_dev;
	dma_addr_t			iova_base;
	dma_addr_t			iova_end;

	
	union {
		struct {
			u64	ttbr[2];
			u64	tcr;
			u64	mair[2];
		} arm_lpae_s1_cfg;

		struct {
			u64	vttbr;
			u64	vtcr;
		} arm_lpae_s2_cfg;

		struct {
			u32	ttbr[2];
			u32	tcr;
			u32	nmrr;
			u32	prrr;
		} arm_v7s_cfg;

		struct {
			u64	ttbr[2];
			u64	tcr;
			u64	mair[2];
			void	*pmds;
		} av8l_fast_cfg;
	};
};


struct io_pgtable_ops {
	int (*map)(struct io_pgtable_ops *ops, unsigned long iova,
		   phys_addr_t paddr, size_t size, int prot);
	size_t (*unmap)(struct io_pgtable_ops *ops, unsigned long iova,
			size_t size);
	int (*map_sg)(struct io_pgtable_ops *ops, unsigned long iova,
		      struct scatterlist *sg, unsigned int nents,
		      int prot, size_t *size);
	phys_addr_t (*iova_to_phys)(struct io_pgtable_ops *ops,
				    unsigned long iova);
	bool (*is_iova_coherent)(struct io_pgtable_ops *ops,
				unsigned long iova);
	uint64_t (*iova_to_pte)(struct io_pgtable_ops *ops,
		    unsigned long iova);

};


struct io_pgtable_ops *alloc_io_pgtable_ops(enum io_pgtable_fmt fmt,
					    struct io_pgtable_cfg *cfg,
					    void *cookie);


void free_io_pgtable_ops(struct io_pgtable_ops *ops);





struct io_pgtable {
	enum io_pgtable_fmt	fmt;
	void			*cookie;
	struct io_pgtable_cfg	cfg;
	struct io_pgtable_ops	ops;
};

#define io_pgtable_ops_to_pgtable(x) container_of((x), struct io_pgtable, ops)

static inline void io_pgtable_tlb_flush_all(struct io_pgtable *iop)
{
	if (!iop->cfg.tlb)
		return;
	iop->cfg.tlb->tlb_flush_all(iop->cookie);
}

static inline void io_pgtable_tlb_add_flush(struct io_pgtable *iop,
		unsigned long iova, size_t size, size_t granule, bool leaf)
{
	if (!iop->cfg.tlb)
		return;
	iop->cfg.tlb->tlb_add_flush(iova, size, granule, leaf, iop->cookie);
}

static inline void io_pgtable_tlb_sync(struct io_pgtable *iop)
{
	if (!iop->cfg.tlb)
		return;
	iop->cfg.tlb->tlb_sync(iop->cookie);
}


struct io_pgtable_init_fns {
	struct io_pgtable *(*alloc)(struct io_pgtable_cfg *cfg, void *cookie);
	void (*free)(struct io_pgtable *iop);
};

extern struct io_pgtable_init_fns io_pgtable_arm_32_lpae_s1_init_fns;
extern struct io_pgtable_init_fns io_pgtable_arm_32_lpae_s2_init_fns;
extern struct io_pgtable_init_fns io_pgtable_arm_64_lpae_s1_init_fns;
extern struct io_pgtable_init_fns io_pgtable_arm_64_lpae_s2_init_fns;
extern struct io_pgtable_init_fns io_pgtable_arm_v7s_init_fns;
extern struct io_pgtable_init_fns io_pgtable_av8l_fast_init_fns;
extern struct io_pgtable_init_fns io_pgtable_arm_msm_secure_init_fns;


void *io_pgtable_alloc_pages_exact(struct io_pgtable_cfg *cfg, void *cookie,
				   size_t size, gfp_t gfp_mask);


void io_pgtable_free_pages_exact(struct io_pgtable_cfg *cfg, void *cookie,
				 void *virt, size_t size);

#endif 
