/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_DAX_H
#define _LINUX_DAX_H

#include <linux/fs.h>
#include <linux/mm.h>
#include <linux/radix-tree.h>
<<<<<<< HEAD
#include <asm/pgtable.h>

struct iomap_ops;
=======

/* Flag for synchronous flush */
#define DAXDEV_F_SYNC (1UL << 0)

typedef unsigned long dax_entry_t;

struct iomap_ops;
struct iomap;
>>>>>>> upstream/android-13
struct dax_device;
struct dax_operations {
	/*
	 * direct_access: translate a device-relative
	 * logical-page-offset into an absolute physical pfn. Return the
	 * number of pages available for DAX at that pfn.
	 */
	long (*direct_access)(struct dax_device *, pgoff_t, long,
			void **, pfn_t *);
<<<<<<< HEAD
=======
	/*
	 * Validate whether this device is usable as an fsdax backing
	 * device.
	 */
	bool (*dax_supported)(struct dax_device *, struct block_device *, int,
			sector_t, sector_t);
>>>>>>> upstream/android-13
	/* copy_from_iter: required operation for fs-dax direct-i/o */
	size_t (*copy_from_iter)(struct dax_device *, pgoff_t, void *, size_t,
			struct iov_iter *);
	/* copy_to_iter: required operation for fs-dax direct-i/o */
	size_t (*copy_to_iter)(struct dax_device *, pgoff_t, void *, size_t,
			struct iov_iter *);
<<<<<<< HEAD
=======
	/* zero_page_range: required operation. Zero page range   */
	int (*zero_page_range)(struct dax_device *, pgoff_t, size_t);
>>>>>>> upstream/android-13
};

extern struct attribute_group dax_attribute_group;

#if IS_ENABLED(CONFIG_DAX)
<<<<<<< HEAD
struct dax_device *dax_get_by_host(const char *host);
struct dax_device *alloc_dax(void *private, const char *host,
		const struct dax_operations *ops);
=======
struct dax_device *alloc_dax(void *private, const char *host,
		const struct dax_operations *ops, unsigned long flags);
>>>>>>> upstream/android-13
void put_dax(struct dax_device *dax_dev);
void kill_dax(struct dax_device *dax_dev);
void dax_write_cache(struct dax_device *dax_dev, bool wc);
bool dax_write_cache_enabled(struct dax_device *dax_dev);
<<<<<<< HEAD
#else
static inline struct dax_device *dax_get_by_host(const char *host)
{
	return NULL;
}
static inline struct dax_device *alloc_dax(void *private, const char *host,
		const struct dax_operations *ops)
=======
bool __dax_synchronous(struct dax_device *dax_dev);
static inline bool dax_synchronous(struct dax_device *dax_dev)
{
	return  __dax_synchronous(dax_dev);
}
void __set_dax_synchronous(struct dax_device *dax_dev);
static inline void set_dax_synchronous(struct dax_device *dax_dev)
{
	__set_dax_synchronous(dax_dev);
}
/*
 * Check if given mapping is supported by the file / underlying device.
 */
static inline bool daxdev_mapping_supported(struct vm_area_struct *vma,
					     struct dax_device *dax_dev)
{
	if (!(vma->vm_flags & VM_SYNC))
		return true;
	if (!IS_DAX(file_inode(vma->vm_file)))
		return false;
	return dax_synchronous(dax_dev);
}
#else
static inline struct dax_device *alloc_dax(void *private, const char *host,
		const struct dax_operations *ops, unsigned long flags)
>>>>>>> upstream/android-13
{
	/*
	 * Callers should check IS_ENABLED(CONFIG_DAX) to know if this
	 * NULL is an error or expected.
	 */
	return NULL;
}
static inline void put_dax(struct dax_device *dax_dev)
{
}
static inline void kill_dax(struct dax_device *dax_dev)
{
}
static inline void dax_write_cache(struct dax_device *dax_dev, bool wc)
{
}
static inline bool dax_write_cache_enabled(struct dax_device *dax_dev)
{
	return false;
}
<<<<<<< HEAD
=======
static inline bool dax_synchronous(struct dax_device *dax_dev)
{
	return true;
}
static inline void set_dax_synchronous(struct dax_device *dax_dev)
{
}
static inline bool daxdev_mapping_supported(struct vm_area_struct *vma,
				struct dax_device *dax_dev)
{
	return !(vma->vm_flags & VM_SYNC);
}
>>>>>>> upstream/android-13
#endif

struct writeback_control;
int bdev_dax_pgoff(struct block_device *, sector_t, size_t, pgoff_t *pgoff);
#if IS_ENABLED(CONFIG_FS_DAX)
<<<<<<< HEAD
bool __bdev_dax_supported(struct block_device *bdev, int blocksize);
static inline bool bdev_dax_supported(struct block_device *bdev, int blocksize)
{
	return __bdev_dax_supported(bdev, blocksize);
}

static inline struct dax_device *fs_dax_get_by_host(const char *host)
{
	return dax_get_by_host(host);
}
=======
bool generic_fsdax_supported(struct dax_device *dax_dev,
		struct block_device *bdev, int blocksize, sector_t start,
		sector_t sectors);

bool dax_supported(struct dax_device *dax_dev, struct block_device *bdev,
		int blocksize, sector_t start, sector_t len);
>>>>>>> upstream/android-13

static inline void fs_put_dax(struct dax_device *dax_dev)
{
	put_dax(dax_dev);
}

struct dax_device *fs_dax_get_by_bdev(struct block_device *bdev);
int dax_writeback_mapping_range(struct address_space *mapping,
<<<<<<< HEAD
		struct block_device *bdev, struct writeback_control *wbc);

struct page *dax_layout_busy_page(struct address_space *mapping);
bool dax_lock_mapping_entry(struct page *page);
void dax_unlock_mapping_entry(struct page *page);
#else
static inline bool bdev_dax_supported(struct block_device *bdev,
		int blocksize)
=======
		struct dax_device *dax_dev, struct writeback_control *wbc);

struct page *dax_layout_busy_page(struct address_space *mapping);
struct page *dax_layout_busy_page_range(struct address_space *mapping, loff_t start, loff_t end);
dax_entry_t dax_lock_page(struct page *page);
void dax_unlock_page(struct page *page, dax_entry_t cookie);
#else
#define generic_fsdax_supported		NULL

static inline bool dax_supported(struct dax_device *dax_dev,
		struct block_device *bdev, int blocksize, sector_t start,
		sector_t len)
>>>>>>> upstream/android-13
{
	return false;
}

<<<<<<< HEAD
static inline struct dax_device *fs_dax_get_by_host(const char *host)
{
	return NULL;
}

=======
>>>>>>> upstream/android-13
static inline void fs_put_dax(struct dax_device *dax_dev)
{
}

static inline struct dax_device *fs_dax_get_by_bdev(struct block_device *bdev)
{
	return NULL;
}

static inline struct page *dax_layout_busy_page(struct address_space *mapping)
{
	return NULL;
}

<<<<<<< HEAD
static inline int dax_writeback_mapping_range(struct address_space *mapping,
		struct block_device *bdev, struct writeback_control *wbc)
=======
static inline struct page *dax_layout_busy_page_range(struct address_space *mapping, pgoff_t start, pgoff_t nr_pages)
{
	return NULL;
}

static inline int dax_writeback_mapping_range(struct address_space *mapping,
		struct dax_device *dax_dev, struct writeback_control *wbc)
>>>>>>> upstream/android-13
{
	return -EOPNOTSUPP;
}

<<<<<<< HEAD
static inline bool dax_lock_mapping_entry(struct page *page)
{
	if (IS_DAX(page->mapping->host))
		return true;
	return false;
}

static inline void dax_unlock_mapping_entry(struct page *page)
=======
static inline dax_entry_t dax_lock_page(struct page *page)
{
	if (IS_DAX(page->mapping->host))
		return ~0UL;
	return 0;
}

static inline void dax_unlock_page(struct page *page, dax_entry_t cookie)
>>>>>>> upstream/android-13
{
}
#endif

<<<<<<< HEAD
int dax_read_lock(void);
void dax_read_unlock(int id);
=======
#if IS_ENABLED(CONFIG_DAX)
int dax_read_lock(void);
void dax_read_unlock(int id);
#else
static inline int dax_read_lock(void)
{
	return 0;
}

static inline void dax_read_unlock(int id)
{
}
#endif /* CONFIG_DAX */
>>>>>>> upstream/android-13
bool dax_alive(struct dax_device *dax_dev);
void *dax_get_private(struct dax_device *dax_dev);
long dax_direct_access(struct dax_device *dax_dev, pgoff_t pgoff, long nr_pages,
		void **kaddr, pfn_t *pfn);
size_t dax_copy_from_iter(struct dax_device *dax_dev, pgoff_t pgoff, void *addr,
		size_t bytes, struct iov_iter *i);
size_t dax_copy_to_iter(struct dax_device *dax_dev, pgoff_t pgoff, void *addr,
		size_t bytes, struct iov_iter *i);
<<<<<<< HEAD
=======
int dax_zero_page_range(struct dax_device *dax_dev, pgoff_t pgoff,
			size_t nr_pages);
>>>>>>> upstream/android-13
void dax_flush(struct dax_device *dax_dev, void *addr, size_t size);

ssize_t dax_iomap_rw(struct kiocb *iocb, struct iov_iter *iter,
		const struct iomap_ops *ops);
vm_fault_t dax_iomap_fault(struct vm_fault *vmf, enum page_entry_size pe_size,
		    pfn_t *pfnp, int *errp, const struct iomap_ops *ops);
vm_fault_t dax_finish_sync_fault(struct vm_fault *vmf,
		enum page_entry_size pe_size, pfn_t pfn);
int dax_delete_mapping_entry(struct address_space *mapping, pgoff_t index);
int dax_invalidate_mapping_entry_sync(struct address_space *mapping,
				      pgoff_t index);
<<<<<<< HEAD

#ifdef CONFIG_FS_DAX
int __dax_zero_page_range(struct block_device *bdev,
		struct dax_device *dax_dev, sector_t sector,
		unsigned int offset, unsigned int length);
#else
static inline int __dax_zero_page_range(struct block_device *bdev,
		struct dax_device *dax_dev, sector_t sector,
		unsigned int offset, unsigned int length)
{
	return -ENXIO;
}
#endif

=======
s64 dax_iomap_zero(loff_t pos, u64 length, struct iomap *iomap);
>>>>>>> upstream/android-13
static inline bool dax_mapping(struct address_space *mapping)
{
	return mapping->host && IS_DAX(mapping->host);
}

<<<<<<< HEAD
=======
#ifdef CONFIG_DEV_DAX_HMEM_DEVICES
void hmem_register_device(int target_nid, struct resource *r);
#else
static inline void hmem_register_device(int target_nid, struct resource *r)
{
}
#endif

>>>>>>> upstream/android-13
#endif
