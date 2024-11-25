/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_MEMREMAP_H_
#define _LINUX_MEMREMAP_H_
<<<<<<< HEAD
#include <linux/ioport.h>
#include <linux/percpu-refcount.h>

#include <asm/pgtable.h>

=======
#include <linux/range.h>
#include <linux/ioport.h>
#include <linux/percpu-refcount.h>

>>>>>>> upstream/android-13
struct resource;
struct device;

/**
 * struct vmem_altmap - pre-allocated storage for vmemmap_populate
 * @base_pfn: base of the entire dev_pagemap mapping
 * @reserve: pages mapped, but reserved for driver use (relative to @base)
 * @free: free pages set aside in the mapping for memmap storage
 * @align: pages reserved to meet allocation alignments
 * @alloc: track pages consumed, private to vmemmap_populate()
 */
struct vmem_altmap {
<<<<<<< HEAD
	const unsigned long base_pfn;
=======
	unsigned long base_pfn;
	const unsigned long end_pfn;
>>>>>>> upstream/android-13
	const unsigned long reserve;
	unsigned long free;
	unsigned long align;
	unsigned long alloc;
};

/*
<<<<<<< HEAD
 * Specialize ZONE_DEVICE memory into multiple types each having differents
=======
 * Specialize ZONE_DEVICE memory into multiple types each has a different
>>>>>>> upstream/android-13
 * usage.
 *
 * MEMORY_DEVICE_PRIVATE:
 * Device memory that is not directly addressable by the CPU: CPU can neither
 * read nor write private memory. In this case, we do still have struct pages
 * backing the device memory. Doing so simplifies the implementation, but it is
 * important to remember that there are certain points at which the struct page
 * must be treated as an opaque object, rather than a "normal" struct page.
 *
 * A more complete discussion of unaddressable memory may be found in
 * include/linux/hmm.h and Documentation/vm/hmm.rst.
 *
<<<<<<< HEAD
 * MEMORY_DEVICE_PUBLIC:
 * Device memory that is cache coherent from device and CPU point of view. This
 * is use on platform that have an advance system bus (like CAPI or CCIX). A
 * driver can hotplug the device memory using ZONE_DEVICE and with that memory
 * type. Any page of a process can be migrated to such memory. However no one
 * should be allow to pin such memory so that it can always be evicted.
 *
=======
>>>>>>> upstream/android-13
 * MEMORY_DEVICE_FS_DAX:
 * Host memory that has similar access semantics as System RAM i.e. DMA
 * coherent and supports page pinning. In support of coordinating page
 * pinning vs other operations MEMORY_DEVICE_FS_DAX arranges for a
 * wakeup event whenever a page is unpinned and becomes idle. This
 * wakeup is used to coordinate physical address space management (ex:
 * fs truncate/hole punch) vs pinned pages (ex: device dma).
<<<<<<< HEAD
 */
enum memory_type {
	MEMORY_DEVICE_PRIVATE = 1,
	MEMORY_DEVICE_PUBLIC,
	MEMORY_DEVICE_FS_DAX,
};

/*
 * For MEMORY_DEVICE_PRIVATE we use ZONE_DEVICE and extend it with two
 * callbacks:
 *   page_fault()
 *   page_free()
 *
 * Additional notes about MEMORY_DEVICE_PRIVATE may be found in
 * include/linux/hmm.h and Documentation/vm/hmm.rst. There is also a brief
 * explanation in include/linux/memory_hotplug.h.
 *
 * The page_fault() callback must migrate page back, from device memory to
 * system memory, so that the CPU can access it. This might fail for various
 * reasons (device issues,  device have been unplugged, ...). When such error
 * conditions happen, the page_fault() callback must return VM_FAULT_SIGBUS and
 * set the CPU page table entry to "poisoned".
 *
 * Note that because memory cgroup charges are transferred to the device memory,
 * this should never fail due to memory restrictions. However, allocation
 * of a regular system page might still fail because we are out of memory. If
 * that happens, the page_fault() callback must return VM_FAULT_OOM.
 *
 * The page_fault() callback can also try to migrate back multiple pages in one
 * chunk, as an optimization. It must, however, prioritize the faulting address
 * over all the others.
 *
 *
 * The page_free() callback is called once the page refcount reaches 1
 * (ZONE_DEVICE pages never reach 0 refcount unless there is a refcount bug.
 * This allows the device driver to implement its own memory management.)
 *
 * For MEMORY_DEVICE_PUBLIC only the page_free() callback matter.
 */
typedef int (*dev_page_fault_t)(struct vm_area_struct *vma,
				unsigned long addr,
				const struct page *page,
				unsigned int flags,
				pmd_t *pmdp);
typedef void (*dev_page_free_t)(struct page *page, void *data);

/**
 * struct dev_pagemap - metadata for ZONE_DEVICE mappings
 * @page_fault: callback when CPU fault on an unaddressable device page
 * @page_free: free page callback when page refcount reaches 1
 * @altmap: pre-allocated/reserved memory for vmemmap allocations
 * @res: physical address range covered by @ref
 * @ref: reference count that pins the devm_memremap_pages() mapping
 * @kill: callback to transition @ref to the dead state
 * @dev: host device of the mapping for debug
 * @data: private data pointer for page_free()
 * @type: memory type: see MEMORY_* in memory_hotplug.h
 */
struct dev_pagemap {
	dev_page_fault_t page_fault;
	dev_page_free_t page_free;
	struct vmem_altmap altmap;
	bool altmap_valid;
	struct resource res;
	struct percpu_ref *ref;
	void (*kill)(struct percpu_ref *ref);
	struct device *dev;
	void *data;
	enum memory_type type;
};

#ifdef CONFIG_ZONE_DEVICE
void *devm_memremap_pages(struct device *dev, struct dev_pagemap *pgmap);
struct dev_pagemap *get_dev_pagemap(unsigned long pfn,
		struct dev_pagemap *pgmap);

unsigned long vmem_altmap_offset(struct vmem_altmap *altmap);
void vmem_altmap_free(struct vmem_altmap *altmap, unsigned long nr_pfns);
=======
 *
 * MEMORY_DEVICE_GENERIC:
 * Host memory that has similar access semantics as System RAM i.e. DMA
 * coherent and supports page pinning. This is for example used by DAX devices
 * that expose memory using a character device.
 *
 * MEMORY_DEVICE_PCI_P2PDMA:
 * Device memory residing in a PCI BAR intended for use with Peer-to-Peer
 * transactions.
 */
enum memory_type {
	/* 0 is reserved to catch uninitialized type fields */
	MEMORY_DEVICE_PRIVATE = 1,
	MEMORY_DEVICE_FS_DAX,
	MEMORY_DEVICE_GENERIC,
	MEMORY_DEVICE_PCI_P2PDMA,
};

struct dev_pagemap_ops {
	/*
	 * Called once the page refcount reaches 1.  (ZONE_DEVICE pages never
	 * reach 0 refcount unless there is a refcount bug. This allows the
	 * device driver to implement its own memory management.)
	 */
	void (*page_free)(struct page *page);

	/*
	 * Transition the refcount in struct dev_pagemap to the dead state.
	 */
	void (*kill)(struct dev_pagemap *pgmap);

	/*
	 * Wait for refcount in struct dev_pagemap to be idle and reap it.
	 */
	void (*cleanup)(struct dev_pagemap *pgmap);

	/*
	 * Used for private (un-addressable) device memory only.  Must migrate
	 * the page back to a CPU accessible page.
	 */
	vm_fault_t (*migrate_to_ram)(struct vm_fault *vmf);
};

#define PGMAP_ALTMAP_VALID	(1 << 0)

/**
 * struct dev_pagemap - metadata for ZONE_DEVICE mappings
 * @altmap: pre-allocated/reserved memory for vmemmap allocations
 * @ref: reference count that pins the devm_memremap_pages() mapping
 * @internal_ref: internal reference if @ref is not provided by the caller
 * @done: completion for @internal_ref
 * @type: memory type: see MEMORY_* in memory_hotplug.h
 * @flags: PGMAP_* flags to specify defailed behavior
 * @ops: method table
 * @owner: an opaque pointer identifying the entity that manages this
 *	instance.  Used by various helpers to make sure that no
 *	foreign ZONE_DEVICE memory is accessed.
 * @nr_range: number of ranges to be mapped
 * @range: range to be mapped when nr_range == 1
 * @ranges: array of ranges to be mapped when nr_range > 1
 */
struct dev_pagemap {
	struct vmem_altmap altmap;
	struct percpu_ref *ref;
	struct percpu_ref internal_ref;
	struct completion done;
	enum memory_type type;
	unsigned int flags;
	const struct dev_pagemap_ops *ops;
	void *owner;
	int nr_range;
	union {
		struct range range;
		struct range ranges[0];
	};
};

static inline struct vmem_altmap *pgmap_altmap(struct dev_pagemap *pgmap)
{
	if (pgmap->flags & PGMAP_ALTMAP_VALID)
		return &pgmap->altmap;
	return NULL;
}

#ifdef CONFIG_ZONE_DEVICE
void *memremap_pages(struct dev_pagemap *pgmap, int nid);
void memunmap_pages(struct dev_pagemap *pgmap);
void *devm_memremap_pages(struct device *dev, struct dev_pagemap *pgmap);
void devm_memunmap_pages(struct device *dev, struct dev_pagemap *pgmap);
struct dev_pagemap *get_dev_pagemap(unsigned long pfn,
		struct dev_pagemap *pgmap);
bool pgmap_pfn_valid(struct dev_pagemap *pgmap, unsigned long pfn);

unsigned long vmem_altmap_offset(struct vmem_altmap *altmap);
void vmem_altmap_free(struct vmem_altmap *altmap, unsigned long nr_pfns);
unsigned long memremap_compat_align(void);
>>>>>>> upstream/android-13
#else
static inline void *devm_memremap_pages(struct device *dev,
		struct dev_pagemap *pgmap)
{
	/*
	 * Fail attempts to call devm_memremap_pages() without
	 * ZONE_DEVICE support enabled, this requires callers to fall
	 * back to plain devm_memremap() based on config
	 */
	WARN_ON_ONCE(1);
	return ERR_PTR(-ENXIO);
}

<<<<<<< HEAD
=======
static inline void devm_memunmap_pages(struct device *dev,
		struct dev_pagemap *pgmap)
{
}

>>>>>>> upstream/android-13
static inline struct dev_pagemap *get_dev_pagemap(unsigned long pfn,
		struct dev_pagemap *pgmap)
{
	return NULL;
}

<<<<<<< HEAD
=======
static inline bool pgmap_pfn_valid(struct dev_pagemap *pgmap, unsigned long pfn)
{
	return false;
}

>>>>>>> upstream/android-13
static inline unsigned long vmem_altmap_offset(struct vmem_altmap *altmap)
{
	return 0;
}

static inline void vmem_altmap_free(struct vmem_altmap *altmap,
		unsigned long nr_pfns)
{
}
<<<<<<< HEAD
=======

/* when memremap_pages() is disabled all archs can remap a single page */
static inline unsigned long memremap_compat_align(void)
{
	return PAGE_SIZE;
}
>>>>>>> upstream/android-13
#endif /* CONFIG_ZONE_DEVICE */

static inline void put_dev_pagemap(struct dev_pagemap *pgmap)
{
	if (pgmap)
		percpu_ref_put(pgmap->ref);
}
<<<<<<< HEAD
=======

>>>>>>> upstream/android-13
#endif /* _LINUX_MEMREMAP_H_ */
