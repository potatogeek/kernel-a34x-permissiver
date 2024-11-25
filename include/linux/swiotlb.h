/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __LINUX_SWIOTLB_H
#define __LINUX_SWIOTLB_H

<<<<<<< HEAD
#include <linux/dma-direction.h>
#include <linux/init.h>
#include <linux/types.h>
=======
#include <linux/device.h>
#include <linux/dma-direction.h>
#include <linux/init.h>
#include <linux/types.h>
#include <linux/limits.h>
#include <linux/spinlock.h>
>>>>>>> upstream/android-13

struct device;
struct page;
struct scatterlist;

enum swiotlb_force {
	SWIOTLB_NORMAL,		/* Default - depending on HW DMA mask etc. */
	SWIOTLB_FORCE,		/* swiotlb=force */
	SWIOTLB_NO_FORCE,	/* swiotlb=noforce */
};

<<<<<<< HEAD
extern enum swiotlb_force swiotlb_force;

=======
>>>>>>> upstream/android-13
/*
 * Maximum allowable number of contiguous slabs to map,
 * must be a power of 2.  What is the appropriate value ?
 * The complexity of {map,unmap}_single is linearly dependent on this value.
 */
#define IO_TLB_SEGSIZE	128

/*
 * log of the size of each IO TLB slab.  The number of slabs is command line
 * controllable.
 */
#define IO_TLB_SHIFT 11
<<<<<<< HEAD

extern void swiotlb_init(int verbose);
int swiotlb_init_with_tbl(char *tlb, unsigned long nslabs, int verbose);
extern unsigned long swiotlb_nr_tbl(void);
unsigned long swiotlb_size_or_default(void);
extern int swiotlb_late_init_with_tbl(char *tlb, unsigned long nslabs);
extern void __init swiotlb_update_mem_attributes(void);

/*
 * Enumeration for sync targets
 */
enum dma_sync_target {
	SYNC_FOR_CPU = 0,
	SYNC_FOR_DEVICE = 1,
};

/* define the last possible byte of physical address space as a mapping error */
#define SWIOTLB_MAP_ERROR (~(phys_addr_t)0x0)

extern phys_addr_t swiotlb_tbl_map_single(struct device *hwdev,
					  dma_addr_t tbl_dma_addr,
					  phys_addr_t phys, size_t size,
					  enum dma_data_direction dir,
					  unsigned long attrs);

extern void swiotlb_tbl_unmap_single(struct device *hwdev,
				     phys_addr_t tlb_addr,
				     size_t size, enum dma_data_direction dir,
				     unsigned long attrs);

extern void swiotlb_tbl_sync_single(struct device *hwdev,
				    phys_addr_t tlb_addr,
				    size_t size, enum dma_data_direction dir,
				    enum dma_sync_target target);

/* Accessory functions. */

void *swiotlb_alloc(struct device *hwdev, size_t size, dma_addr_t *dma_handle,
		gfp_t flags, unsigned long attrs);
void swiotlb_free(struct device *dev, size_t size, void *vaddr,
		dma_addr_t dma_addr, unsigned long attrs);

extern dma_addr_t swiotlb_map_page(struct device *dev, struct page *page,
				   unsigned long offset, size_t size,
				   enum dma_data_direction dir,
				   unsigned long attrs);
extern void swiotlb_unmap_page(struct device *hwdev, dma_addr_t dev_addr,
			       size_t size, enum dma_data_direction dir,
			       unsigned long attrs);

extern int
swiotlb_map_sg_attrs(struct device *hwdev, struct scatterlist *sgl, int nelems,
		     enum dma_data_direction dir,
		     unsigned long attrs);

extern void
swiotlb_unmap_sg_attrs(struct device *hwdev, struct scatterlist *sgl,
		       int nelems, enum dma_data_direction dir,
		       unsigned long attrs);

extern void
swiotlb_sync_single_for_cpu(struct device *hwdev, dma_addr_t dev_addr,
			    size_t size, enum dma_data_direction dir);

extern void
swiotlb_sync_sg_for_cpu(struct device *hwdev, struct scatterlist *sg,
			int nelems, enum dma_data_direction dir);

extern void
swiotlb_sync_single_for_device(struct device *hwdev, dma_addr_t dev_addr,
			       size_t size, enum dma_data_direction dir);

extern void
swiotlb_sync_sg_for_device(struct device *hwdev, struct scatterlist *sg,
			   int nelems, enum dma_data_direction dir);

extern int
swiotlb_dma_mapping_error(struct device *hwdev, dma_addr_t dma_addr);

extern int
swiotlb_dma_supported(struct device *hwdev, u64 mask);

#ifdef CONFIG_SWIOTLB
extern void __init swiotlb_exit(void);
unsigned int swiotlb_max_segment(void);
#else
static inline void swiotlb_exit(void) { }
static inline unsigned int swiotlb_max_segment(void) { return 0; }
#endif

extern void swiotlb_print_info(void);
extern int is_swiotlb_buffer(phys_addr_t paddr);
extern void swiotlb_set_max_segment(unsigned int);

extern const struct dma_map_ops swiotlb_dma_ops;
=======
#define IO_TLB_SIZE (1 << IO_TLB_SHIFT)

/* default to 64MB */
#define IO_TLB_DEFAULT_SIZE (64UL<<20)

extern void swiotlb_init(int verbose);
int swiotlb_init_with_tbl(char *tlb, unsigned long nslabs, int verbose);
unsigned long swiotlb_size_or_default(void);
extern int swiotlb_late_init_with_tbl(char *tlb, unsigned long nslabs);
extern int swiotlb_late_init_with_default_size(size_t default_size);
extern void __init swiotlb_update_mem_attributes(void);

phys_addr_t swiotlb_tbl_map_single(struct device *hwdev, phys_addr_t phys,
		size_t mapping_size, size_t alloc_size,
		unsigned int alloc_aligned_mask, enum dma_data_direction dir,
		unsigned long attrs);

extern void swiotlb_tbl_unmap_single(struct device *hwdev,
				     phys_addr_t tlb_addr,
				     size_t mapping_size,
				     enum dma_data_direction dir,
				     unsigned long attrs);

void swiotlb_sync_single_for_device(struct device *dev, phys_addr_t tlb_addr,
		size_t size, enum dma_data_direction dir);
void swiotlb_sync_single_for_cpu(struct device *dev, phys_addr_t tlb_addr,
		size_t size, enum dma_data_direction dir);
dma_addr_t swiotlb_map(struct device *dev, phys_addr_t phys,
		size_t size, enum dma_data_direction dir, unsigned long attrs);

#ifdef CONFIG_SWIOTLB
extern enum swiotlb_force swiotlb_force;

/**
 * struct io_tlb_mem - IO TLB Memory Pool Descriptor
 *
 * @start:	The start address of the swiotlb memory pool. Used to do a quick
 *		range check to see if the memory was in fact allocated by this
 *		API.
 * @end:	The end address of the swiotlb memory pool. Used to do a quick
 *		range check to see if the memory was in fact allocated by this
 *		API.
 * @nslabs:	The number of IO TLB blocks (in groups of 64) between @start and
 *		@end. For default swiotlb, this is command line adjustable via
 *		setup_io_tlb_npages.
 * @used:	The number of used IO TLB block.
 * @list:	The free list describing the number of free entries available
 *		from each index.
 * @index:	The index to start searching in the next round.
 * @orig_addr:	The original address corresponding to a mapped entry.
 * @alloc_size:	Size of the allocated buffer.
 * @lock:	The lock to protect the above data structures in the map and
 *		unmap calls.
 * @debugfs:	The dentry to debugfs.
 * @late_alloc:	%true if allocated using the page allocator
 * @force_bounce: %true if swiotlb bouncing is forced
 * @for_alloc:  %true if the pool is used for memory allocation
 */
struct io_tlb_mem {
	phys_addr_t start;
	phys_addr_t end;
	unsigned long nslabs;
	unsigned long used;
	unsigned int index;
	spinlock_t lock;
	struct dentry *debugfs;
	bool late_alloc;
	bool force_bounce;
	bool for_alloc;
	struct io_tlb_slot {
		phys_addr_t orig_addr;
		size_t alloc_size;
		unsigned int list;
	} *slots;
};
extern struct io_tlb_mem io_tlb_default_mem;

static inline bool is_swiotlb_buffer(struct device *dev, phys_addr_t paddr)
{
	struct io_tlb_mem *mem = dev->dma_io_tlb_mem;

	return mem && paddr >= mem->start && paddr < mem->end;
}

static inline bool is_swiotlb_force_bounce(struct device *dev)
{
	struct io_tlb_mem *mem = dev->dma_io_tlb_mem;

	return mem && mem->force_bounce;
}

void __init swiotlb_exit(void);
unsigned int swiotlb_max_segment(void);
size_t swiotlb_max_mapping_size(struct device *dev);
bool is_swiotlb_active(struct device *dev);
void __init swiotlb_adjust_size(unsigned long size);
#else
#define swiotlb_force SWIOTLB_NO_FORCE
static inline bool is_swiotlb_buffer(struct device *dev, phys_addr_t paddr)
{
	return false;
}
static inline bool is_swiotlb_force_bounce(struct device *dev)
{
	return false;
}
static inline void swiotlb_exit(void)
{
}
static inline unsigned int swiotlb_max_segment(void)
{
	return 0;
}
static inline size_t swiotlb_max_mapping_size(struct device *dev)
{
	return SIZE_MAX;
}

static inline bool is_swiotlb_active(struct device *dev)
{
	return false;
}

static inline void swiotlb_adjust_size(unsigned long size)
{
}
#endif /* CONFIG_SWIOTLB */

extern void swiotlb_print_info(void);
extern void swiotlb_set_max_segment(unsigned int);

#ifdef CONFIG_DMA_RESTRICTED_POOL
struct page *swiotlb_alloc(struct device *dev, size_t size);
bool swiotlb_free(struct device *dev, struct page *page, size_t size);

static inline bool is_swiotlb_for_alloc(struct device *dev)
{
	return dev->dma_io_tlb_mem->for_alloc;
}
#else
static inline struct page *swiotlb_alloc(struct device *dev, size_t size)
{
	return NULL;
}
static inline bool swiotlb_free(struct device *dev, struct page *page,
				size_t size)
{
	return false;
}
static inline bool is_swiotlb_for_alloc(struct device *dev)
{
	return false;
}
#endif /* CONFIG_DMA_RESTRICTED_POOL */
>>>>>>> upstream/android-13

#endif /* __LINUX_SWIOTLB_H */
