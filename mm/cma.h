/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __MM_CMA_H__
#define __MM_CMA_H__

<<<<<<< HEAD
=======
#include <linux/debugfs.h>
#include <linux/kobject.h>

struct cma_kobject {
	struct kobject kobj;
	struct cma *cma;
};

>>>>>>> upstream/android-13
struct cma {
	unsigned long   base_pfn;
	unsigned long   count;
	unsigned long   *bitmap;
	unsigned int order_per_bit; /* Order of pages represented by one bit */
<<<<<<< HEAD
	struct mutex    lock;
#ifdef CONFIG_CMA_DEBUGFS
	struct hlist_head mem_head;
	spinlock_t mem_head_lock;
#endif
	const char *name;
=======
	spinlock_t	lock;
#ifdef CONFIG_CMA_DEBUGFS
	struct hlist_head mem_head;
	spinlock_t mem_head_lock;
	struct debugfs_u32_array dfs_bitmap;
#endif
	char name[CMA_MAX_NAME];
#ifdef CONFIG_CMA_SYSFS
	/* the number of CMA page successful allocations */
	atomic64_t nr_pages_succeeded;
	/* the number of CMA page allocation failures */
	atomic64_t nr_pages_failed;
	/* kobject requires dynamic object */
	struct cma_kobject *cma_kobj;
#endif
>>>>>>> upstream/android-13
};

extern struct cma cma_areas[MAX_CMA_AREAS];
extern unsigned cma_area_count;

static inline unsigned long cma_bitmap_maxno(struct cma *cma)
{
	return cma->count >> cma->order_per_bit;
}

<<<<<<< HEAD
=======
#ifdef CONFIG_CMA_SYSFS
void cma_sysfs_account_success_pages(struct cma *cma, unsigned long nr_pages);
void cma_sysfs_account_fail_pages(struct cma *cma, unsigned long nr_pages);
#else
static inline void cma_sysfs_account_success_pages(struct cma *cma,
						   unsigned long nr_pages) {};
static inline void cma_sysfs_account_fail_pages(struct cma *cma,
						unsigned long nr_pages) {};
#endif
>>>>>>> upstream/android-13
#endif
