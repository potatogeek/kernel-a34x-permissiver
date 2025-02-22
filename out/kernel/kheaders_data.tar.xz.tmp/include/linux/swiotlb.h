/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __LINUX_SWIOTLB_H
#define __LINUX_SWIOTLB_H

#include <linux/dma-direction.h>
#include <linux/init.h>
#include <linux/types.h>

struct device;
struct page;
struct scatterlist;

enum swiotlb_force {
	SWIOTLB_NORMAL,		
	SWIOTLB_FORCE,		
	SWIOTLB_NO_FORCE,	
};

extern enum swiotlb_force swiotlb_force;


#define IO_TLB_SEGSIZE	128


#define IO_TLB_SHIFT 11

extern void swiotlb_init(int verbose);
int swiotlb_init_with_tbl(char *tlb, unsigned long nslabs, int verbose);
extern unsigned long swiotlb_nr_tbl(void);
unsigned long swiotlb_size_or_default(void);
extern int swiotlb_late_init_with_tbl(char *tlb, unsigned long nslabs);
extern void __init swiotlb_update_mem_attributes(void);


enum dma_sync_target {
	SYNC_FOR_CPU = 0,
	SYNC_FOR_DEVICE = 1,
};


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

#endif 
