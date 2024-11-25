

#ifndef LINUX_HMM_H
#define LINUX_HMM_H

#include <linux/kconfig.h>

#if IS_ENABLED(CONFIG_HMM)

#include <linux/device.h>
#include <linux/migrate.h>
#include <linux/memremap.h>
#include <linux/completion.h>

struct hmm;


enum hmm_pfn_flag_e {
	HMM_PFN_VALID = 0,
	HMM_PFN_WRITE,
	HMM_PFN_DEVICE_PRIVATE,
	HMM_PFN_FLAG_MAX
};


enum hmm_pfn_value_e {
	HMM_PFN_ERROR,
	HMM_PFN_NONE,
	HMM_PFN_SPECIAL,
	HMM_PFN_VALUE_MAX
};


struct hmm_range {
	struct vm_area_struct	*vma;
	struct list_head	list;
	unsigned long		start;
	unsigned long		end;
	uint64_t		*pfns;
	const uint64_t		*flags;
	const uint64_t		*values;
	uint8_t			pfn_shift;
	bool			valid;
};


static inline struct page *hmm_pfn_to_page(const struct hmm_range *range,
					   uint64_t pfn)
{
	if (pfn == range->values[HMM_PFN_NONE])
		return NULL;
	if (pfn == range->values[HMM_PFN_ERROR])
		return NULL;
	if (pfn == range->values[HMM_PFN_SPECIAL])
		return NULL;
	if (!(pfn & range->flags[HMM_PFN_VALID]))
		return NULL;
	return pfn_to_page(pfn >> range->pfn_shift);
}


static inline unsigned long hmm_pfn_to_pfn(const struct hmm_range *range,
					   uint64_t pfn)
{
	if (pfn == range->values[HMM_PFN_NONE])
		return -1UL;
	if (pfn == range->values[HMM_PFN_ERROR])
		return -1UL;
	if (pfn == range->values[HMM_PFN_SPECIAL])
		return -1UL;
	if (!(pfn & range->flags[HMM_PFN_VALID]))
		return -1UL;
	return (pfn >> range->pfn_shift);
}


static inline uint64_t hmm_pfn_from_page(const struct hmm_range *range,
					 struct page *page)
{
	return (page_to_pfn(page) << range->pfn_shift) |
		range->flags[HMM_PFN_VALID];
}


static inline uint64_t hmm_pfn_from_pfn(const struct hmm_range *range,
					unsigned long pfn)
{
	return (pfn << range->pfn_shift) |
		range->flags[HMM_PFN_VALID];
}


#if IS_ENABLED(CONFIG_HMM_MIRROR)


struct hmm_mirror;


enum hmm_update_type {
	HMM_UPDATE_INVALIDATE,
};


struct hmm_mirror_ops {
	
	void (*release)(struct hmm_mirror *mirror);

	
	void (*sync_cpu_device_pagetables)(struct hmm_mirror *mirror,
					   enum hmm_update_type update_type,
					   unsigned long start,
					   unsigned long end);
};


struct hmm_mirror {
	struct hmm			*hmm;
	const struct hmm_mirror_ops	*ops;
	struct list_head		list;
};

int hmm_mirror_register(struct hmm_mirror *mirror, struct mm_struct *mm);
void hmm_mirror_unregister(struct hmm_mirror *mirror);



int hmm_vma_get_pfns(struct hmm_range *range);
bool hmm_vma_range_done(struct hmm_range *range);



int hmm_vma_fault(struct hmm_range *range, bool block);


void hmm_mm_destroy(struct mm_struct *mm);

static inline void hmm_mm_init(struct mm_struct *mm)
{
	mm->hmm = NULL;
}
#else 
static inline void hmm_mm_destroy(struct mm_struct *mm) {}
static inline void hmm_mm_init(struct mm_struct *mm) {}
#endif 

#if IS_ENABLED(CONFIG_DEVICE_PRIVATE) ||  IS_ENABLED(CONFIG_DEVICE_PUBLIC)
struct hmm_devmem;

struct page *hmm_vma_alloc_locked_page(struct vm_area_struct *vma,
				       unsigned long addr);


struct hmm_devmem_ops {
	
	void (*free)(struct hmm_devmem *devmem, struct page *page);
	
	int (*fault)(struct hmm_devmem *devmem,
		     struct vm_area_struct *vma,
		     unsigned long addr,
		     const struct page *page,
		     unsigned int flags,
		     pmd_t *pmdp);
};


struct hmm_devmem {
	struct completion		completion;
	unsigned long			pfn_first;
	unsigned long			pfn_last;
	struct resource			*resource;
	struct device			*device;
	struct dev_pagemap		pagemap;
	const struct hmm_devmem_ops	*ops;
	struct percpu_ref		ref;
};


struct hmm_devmem *hmm_devmem_add(const struct hmm_devmem_ops *ops,
				  struct device *device,
				  unsigned long size);
struct hmm_devmem *hmm_devmem_add_resource(const struct hmm_devmem_ops *ops,
					   struct device *device,
					   struct resource *res);


static inline void hmm_devmem_page_set_drvdata(struct page *page,
					       unsigned long data)
{
	page->hmm_data = data;
}


static inline unsigned long hmm_devmem_page_get_drvdata(const struct page *page)
{
	return page->hmm_data;
}



struct hmm_device {
	struct device		device;
	unsigned int		minor;
};


struct hmm_device *hmm_device_new(void *drvdata);
void hmm_device_put(struct hmm_device *hmm_device);
#endif 
#else 
static inline void hmm_mm_destroy(struct mm_struct *mm) {}
static inline void hmm_mm_init(struct mm_struct *mm) {}
#endif 

#endif 
