/*
 * hugetlbpage-backed filesystem.  Based on ramfs.
 *
 * Nadia Yvette Chambers, 2002
 *
 * Copyright (C) 2002 Linus Torvalds.
 * License: GPL
 */

#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linux/thread_info.h>
#include <asm/current.h>
#include <linux/sched/signal.h>		/* remove ASAP */
#include <linux/falloc.h>
#include <linux/fs.h>
#include <linux/mount.h>
#include <linux/file.h>
#include <linux/kernel.h>
#include <linux/writeback.h>
#include <linux/pagemap.h>
#include <linux/highmem.h>
#include <linux/init.h>
#include <linux/string.h>
#include <linux/capability.h>
#include <linux/ctype.h>
#include <linux/backing-dev.h>
#include <linux/hugetlb.h>
#include <linux/pagevec.h>
<<<<<<< HEAD
#include <linux/parser.h>
=======
#include <linux/fs_parser.h>
>>>>>>> upstream/android-13
#include <linux/mman.h>
#include <linux/slab.h>
#include <linux/dnotify.h>
#include <linux/statfs.h>
#include <linux/security.h>
#include <linux/magic.h>
#include <linux/migrate.h>
#include <linux/uio.h>

#include <linux/uaccess.h>
<<<<<<< HEAD
=======
#include <linux/sched/mm.h>
>>>>>>> upstream/android-13

static const struct super_operations hugetlbfs_ops;
static const struct address_space_operations hugetlbfs_aops;
const struct file_operations hugetlbfs_file_operations;
static const struct inode_operations hugetlbfs_dir_inode_operations;
static const struct inode_operations hugetlbfs_inode_operations;

<<<<<<< HEAD
struct hugetlbfs_config {
	struct hstate		*hstate;
	long			max_hpages;
	long			nr_inodes;
	long			min_hpages;
=======
enum hugetlbfs_size_type { NO_SIZE, SIZE_STD, SIZE_PERCENT };

struct hugetlbfs_fs_context {
	struct hstate		*hstate;
	unsigned long long	max_size_opt;
	unsigned long long	min_size_opt;
	long			max_hpages;
	long			nr_inodes;
	long			min_hpages;
	enum hugetlbfs_size_type max_val_type;
	enum hugetlbfs_size_type min_val_type;
>>>>>>> upstream/android-13
	kuid_t			uid;
	kgid_t			gid;
	umode_t			mode;
};

int sysctl_hugetlb_shm_group;

<<<<<<< HEAD
enum {
	Opt_size, Opt_nr_inodes,
	Opt_mode, Opt_uid, Opt_gid,
	Opt_pagesize, Opt_min_size,
	Opt_err,
};

static const match_table_t tokens = {
	{Opt_size,	"size=%s"},
	{Opt_nr_inodes,	"nr_inodes=%s"},
	{Opt_mode,	"mode=%o"},
	{Opt_uid,	"uid=%u"},
	{Opt_gid,	"gid=%u"},
	{Opt_pagesize,	"pagesize=%s"},
	{Opt_min_size,	"min_size=%s"},
	{Opt_err,	NULL},
=======
enum hugetlb_param {
	Opt_gid,
	Opt_min_size,
	Opt_mode,
	Opt_nr_inodes,
	Opt_pagesize,
	Opt_size,
	Opt_uid,
};

static const struct fs_parameter_spec hugetlb_fs_parameters[] = {
	fsparam_u32   ("gid",		Opt_gid),
	fsparam_string("min_size",	Opt_min_size),
	fsparam_u32oct("mode",		Opt_mode),
	fsparam_string("nr_inodes",	Opt_nr_inodes),
	fsparam_string("pagesize",	Opt_pagesize),
	fsparam_string("size",		Opt_size),
	fsparam_u32   ("uid",		Opt_uid),
	{}
>>>>>>> upstream/android-13
};

#ifdef CONFIG_NUMA
static inline void hugetlb_set_vma_policy(struct vm_area_struct *vma,
					struct inode *inode, pgoff_t index)
{
	vma->vm_policy = mpol_shared_policy_lookup(&HUGETLBFS_I(inode)->policy,
							index);
}

static inline void hugetlb_drop_vma_policy(struct vm_area_struct *vma)
{
	mpol_cond_put(vma->vm_policy);
}
#else
static inline void hugetlb_set_vma_policy(struct vm_area_struct *vma,
					struct inode *inode, pgoff_t index)
{
}

static inline void hugetlb_drop_vma_policy(struct vm_area_struct *vma)
{
}
#endif

static void huge_pagevec_release(struct pagevec *pvec)
{
	int i;

	for (i = 0; i < pagevec_count(pvec); ++i)
		put_page(pvec->pages[i]);

	pagevec_reinit(pvec);
}

/*
 * Mask used when checking the page offset value passed in via system
 * calls.  This value will be converted to a loff_t which is signed.
 * Therefore, we want to check the upper PAGE_SHIFT + 1 bits of the
 * value.  The extra bit (- 1 in the shift value) is to take the sign
 * bit into account.
 */
#define PGOFF_LOFFT_MAX \
	(((1UL << (PAGE_SHIFT + 1)) - 1) <<  (BITS_PER_LONG - (PAGE_SHIFT + 1)))

static int hugetlbfs_file_mmap(struct file *file, struct vm_area_struct *vma)
{
	struct inode *inode = file_inode(file);
<<<<<<< HEAD
=======
	struct hugetlbfs_inode_info *info = HUGETLBFS_I(inode);
>>>>>>> upstream/android-13
	loff_t len, vma_len;
	int ret;
	struct hstate *h = hstate_file(file);

	/*
	 * vma address alignment (but not the pgoff alignment) has
	 * already been checked by prepare_hugepage_range.  If you add
	 * any error returns here, do so after setting VM_HUGETLB, so
	 * is_vm_hugetlb_page tests below unmap_region go the right
<<<<<<< HEAD
	 * way when do_mmap_pgoff unwinds (may be important on powerpc
=======
	 * way when do_mmap unwinds (may be important on powerpc
>>>>>>> upstream/android-13
	 * and ia64).
	 */
	vma->vm_flags |= VM_HUGETLB | VM_DONTEXPAND;
	vma->vm_ops = &hugetlb_vm_ops;

<<<<<<< HEAD
=======
	ret = seal_check_future_write(info->seals, vma);
	if (ret)
		return ret;

>>>>>>> upstream/android-13
	/*
	 * page based offset in vm_pgoff could be sufficiently large to
	 * overflow a loff_t when converted to byte offset.  This can
	 * only happen on architectures where sizeof(loff_t) ==
	 * sizeof(unsigned long).  So, only check in those instances.
	 */
	if (sizeof(unsigned long) == sizeof(loff_t)) {
		if (vma->vm_pgoff & PGOFF_LOFFT_MAX)
			return -EINVAL;
	}

	/* must be huge page aligned */
	if (vma->vm_pgoff & (~huge_page_mask(h) >> PAGE_SHIFT))
		return -EINVAL;

	vma_len = (loff_t)(vma->vm_end - vma->vm_start);
	len = vma_len + ((loff_t)vma->vm_pgoff << PAGE_SHIFT);
	/* check for overflow */
	if (len < vma_len)
		return -EINVAL;

	inode_lock(inode);
	file_accessed(file);

	ret = -ENOMEM;
<<<<<<< HEAD
	if (hugetlb_reserve_pages(inode,
=======
	if (!hugetlb_reserve_pages(inode,
>>>>>>> upstream/android-13
				vma->vm_pgoff >> huge_page_order(h),
				len >> huge_page_shift(h), vma,
				vma->vm_flags))
		goto out;

	ret = 0;
	if (vma->vm_flags & VM_WRITE && inode->i_size < len)
		i_size_write(inode, len);
out:
	inode_unlock(inode);

	return ret;
}

/*
<<<<<<< HEAD
 * Called under down_write(mmap_sem).
=======
 * Called under mmap_write_lock(mm).
>>>>>>> upstream/android-13
 */

#ifndef HAVE_ARCH_HUGETLB_UNMAPPED_AREA
static unsigned long
<<<<<<< HEAD
=======
hugetlb_get_unmapped_area_bottomup(struct file *file, unsigned long addr,
		unsigned long len, unsigned long pgoff, unsigned long flags)
{
	struct hstate *h = hstate_file(file);
	struct vm_unmapped_area_info info;

	info.flags = 0;
	info.length = len;
	info.low_limit = current->mm->mmap_base;
	info.high_limit = arch_get_mmap_end(addr);
	info.align_mask = PAGE_MASK & ~huge_page_mask(h);
	info.align_offset = 0;
	return vm_unmapped_area(&info);
}

static unsigned long
hugetlb_get_unmapped_area_topdown(struct file *file, unsigned long addr,
		unsigned long len, unsigned long pgoff, unsigned long flags)
{
	struct hstate *h = hstate_file(file);
	struct vm_unmapped_area_info info;

	info.flags = VM_UNMAPPED_AREA_TOPDOWN;
	info.length = len;
	info.low_limit = max(PAGE_SIZE, mmap_min_addr);
	info.high_limit = arch_get_mmap_base(addr, current->mm->mmap_base);
	info.align_mask = PAGE_MASK & ~huge_page_mask(h);
	info.align_offset = 0;
	addr = vm_unmapped_area(&info);

	/*
	 * A failed mmap() very likely causes application failure,
	 * so fall back to the bottom-up function here. This scenario
	 * can happen with large stack limits and large mmap()
	 * allocations.
	 */
	if (unlikely(offset_in_page(addr))) {
		VM_BUG_ON(addr != -ENOMEM);
		info.flags = 0;
		info.low_limit = current->mm->mmap_base;
		info.high_limit = arch_get_mmap_end(addr);
		addr = vm_unmapped_area(&info);
	}

	return addr;
}

static unsigned long
>>>>>>> upstream/android-13
hugetlb_get_unmapped_area(struct file *file, unsigned long addr,
		unsigned long len, unsigned long pgoff, unsigned long flags)
{
	struct mm_struct *mm = current->mm;
	struct vm_area_struct *vma;
	struct hstate *h = hstate_file(file);
<<<<<<< HEAD
	struct vm_unmapped_area_info info;
=======
	const unsigned long mmap_end = arch_get_mmap_end(addr);
>>>>>>> upstream/android-13

	if (len & ~huge_page_mask(h))
		return -EINVAL;
	if (len > TASK_SIZE)
		return -ENOMEM;

	if (flags & MAP_FIXED) {
		if (prepare_hugepage_range(file, addr, len))
			return -EINVAL;
		return addr;
	}

	if (addr) {
		addr = ALIGN(addr, huge_page_size(h));
		vma = find_vma(mm, addr);
<<<<<<< HEAD
		if (TASK_SIZE - len >= addr &&
=======
		if (mmap_end - len >= addr &&
>>>>>>> upstream/android-13
		    (!vma || addr + len <= vm_start_gap(vma)))
			return addr;
	}

<<<<<<< HEAD
	info.flags = 0;
	info.length = len;
	info.low_limit = TASK_UNMAPPED_BASE;
	info.high_limit = TASK_SIZE;
	info.align_mask = PAGE_MASK & ~huge_page_mask(h);
	info.align_offset = 0;
	return vm_unmapped_area(&info);
=======
	/*
	 * Use mm->get_unmapped_area value as a hint to use topdown routine.
	 * If architectures have special needs, they should define their own
	 * version of hugetlb_get_unmapped_area.
	 */
	if (mm->get_unmapped_area == arch_get_unmapped_area_topdown)
		return hugetlb_get_unmapped_area_topdown(file, addr, len,
				pgoff, flags);
	return hugetlb_get_unmapped_area_bottomup(file, addr, len,
			pgoff, flags);
>>>>>>> upstream/android-13
}
#endif

static size_t
hugetlbfs_read_actor(struct page *page, unsigned long offset,
			struct iov_iter *to, unsigned long size)
{
	size_t copied = 0;
	int i, chunksize;

	/* Find which 4k chunk and offset with in that chunk */
	i = offset >> PAGE_SHIFT;
	offset = offset & ~PAGE_MASK;

	while (size) {
		size_t n;
		chunksize = PAGE_SIZE;
		if (offset)
			chunksize -= offset;
		if (chunksize > size)
			chunksize = size;
		n = copy_page_to_iter(&page[i], offset, chunksize, to);
		copied += n;
		if (n != chunksize)
			return copied;
		offset = 0;
		size -= chunksize;
		i++;
	}
	return copied;
}

/*
 * Support for read() - Find the page attached to f_mapping and copy out the
<<<<<<< HEAD
 * data. Its *very* similar to do_generic_mapping_read(), we can't use that
=======
 * data. Its *very* similar to generic_file_buffered_read(), we can't use that
>>>>>>> upstream/android-13
 * since it has PAGE_SIZE assumptions.
 */
static ssize_t hugetlbfs_read_iter(struct kiocb *iocb, struct iov_iter *to)
{
	struct file *file = iocb->ki_filp;
	struct hstate *h = hstate_file(file);
	struct address_space *mapping = file->f_mapping;
	struct inode *inode = mapping->host;
	unsigned long index = iocb->ki_pos >> huge_page_shift(h);
	unsigned long offset = iocb->ki_pos & ~huge_page_mask(h);
	unsigned long end_index;
	loff_t isize;
	ssize_t retval = 0;

	while (iov_iter_count(to)) {
		struct page *page;
		size_t nr, copied;

		/* nr is the maximum number of bytes to copy from this page */
		nr = huge_page_size(h);
		isize = i_size_read(inode);
		if (!isize)
			break;
		end_index = (isize - 1) >> huge_page_shift(h);
		if (index > end_index)
			break;
		if (index == end_index) {
			nr = ((isize - 1) & ~huge_page_mask(h)) + 1;
			if (nr <= offset)
				break;
		}
		nr = nr - offset;

		/* Find the page */
		page = find_lock_page(mapping, index);
		if (unlikely(page == NULL)) {
			/*
			 * We have a HOLE, zero out the user-buffer for the
			 * length of the hole or request.
			 */
			copied = iov_iter_zero(nr, to);
		} else {
			unlock_page(page);

			/*
			 * We have the page, copy it to user space buffer.
			 */
			copied = hugetlbfs_read_actor(page, offset, to, nr);
			put_page(page);
		}
		offset += copied;
		retval += copied;
		if (copied != nr && iov_iter_count(to)) {
			if (!retval)
				retval = -EFAULT;
			break;
		}
		index += offset >> huge_page_shift(h);
		offset &= ~huge_page_mask(h);
	}
	iocb->ki_pos = ((loff_t)index << huge_page_shift(h)) + offset;
	return retval;
}

static int hugetlbfs_write_begin(struct file *file,
			struct address_space *mapping,
			loff_t pos, unsigned len, unsigned flags,
			struct page **pagep, void **fsdata)
{
	return -EINVAL;
}

static int hugetlbfs_write_end(struct file *file, struct address_space *mapping,
			loff_t pos, unsigned len, unsigned copied,
			struct page *page, void *fsdata)
{
	BUG();
	return -EINVAL;
}

static void remove_huge_page(struct page *page)
{
	ClearPageDirty(page);
	ClearPageUptodate(page);
	delete_from_page_cache(page);
}

static void
hugetlb_vmdelete_list(struct rb_root_cached *root, pgoff_t start, pgoff_t end)
{
	struct vm_area_struct *vma;

	/*
<<<<<<< HEAD
	 * end == 0 indicates that the entire range after
	 * start should be unmapped.
	 */
	vma_interval_tree_foreach(vma, root, start, end ? end : ULONG_MAX) {
=======
	 * end == 0 indicates that the entire range after start should be
	 * unmapped.  Note, end is exclusive, whereas the interval tree takes
	 * an inclusive "last".
	 */
	vma_interval_tree_foreach(vma, root, start, end ? end - 1 : ULONG_MAX) {
>>>>>>> upstream/android-13
		unsigned long v_offset;
		unsigned long v_end;

		/*
		 * Can the expression below overflow on 32-bit arches?
		 * No, because the interval tree returns us only those vmas
		 * which overlap the truncated area starting at pgoff,
		 * and no vma on a 32-bit arch can span beyond the 4GB.
		 */
		if (vma->vm_pgoff < start)
			v_offset = (start - vma->vm_pgoff) << PAGE_SHIFT;
		else
			v_offset = 0;

		if (!end)
			v_end = vma->vm_end;
		else {
			v_end = ((end - vma->vm_pgoff) << PAGE_SHIFT)
							+ vma->vm_start;
			if (v_end > vma->vm_end)
				v_end = vma->vm_end;
		}

		unmap_hugepage_range(vma, vma->vm_start + v_offset, v_end,
									NULL);
	}
}

/*
 * remove_inode_hugepages handles two distinct cases: truncation and hole
 * punch.  There are subtle differences in operation for each case.
 *
 * truncation is indicated by end of range being LLONG_MAX
 *	In this case, we first scan the range and release found pages.
<<<<<<< HEAD
 *	After releasing pages, hugetlb_unreserve_pages cleans up region/reserv
 *	maps and global counts.  Page faults can not race with truncation
 *	in this routine.  hugetlb_no_page() prevents page faults in the
 *	truncated range.  It checks i_size before allocation, and again after
 *	with the page table lock for the page held.  The same lock must be
 *	acquired to unmap a page.
 * hole punch is indicated if end is not LLONG_MAX
 *	In the hole punch case we scan the range and release found pages.
 *	Only when releasing a page is the associated region/reserv map
 *	deleted.  The region/reserv map for ranges without associated
=======
 *	After releasing pages, hugetlb_unreserve_pages cleans up region/reserve
 *	maps and global counts.  Page faults can not race with truncation
 *	in this routine.  hugetlb_no_page() holds i_mmap_rwsem and prevents
 *	page faults in the truncated range by checking i_size.  i_size is
 *	modified while holding i_mmap_rwsem.
 * hole punch is indicated if end is not LLONG_MAX
 *	In the hole punch case we scan the range and release found pages.
 *	Only when releasing a page is the associated region/reserve map
 *	deleted.  The region/reserve map for ranges without associated
>>>>>>> upstream/android-13
 *	pages are not modified.  Page faults can race with hole punch.
 *	This is indicated if we find a mapped page.
 * Note: If the passed end of range value is beyond the end of file, but
 * not LLONG_MAX this routine still performs a hole punch operation.
 */
static void remove_inode_hugepages(struct inode *inode, loff_t lstart,
				   loff_t lend)
{
	struct hstate *h = hstate_inode(inode);
	struct address_space *mapping = &inode->i_data;
	const pgoff_t start = lstart >> huge_page_shift(h);
	const pgoff_t end = lend >> huge_page_shift(h);
<<<<<<< HEAD
	struct vm_area_struct pseudo_vma;
=======
>>>>>>> upstream/android-13
	struct pagevec pvec;
	pgoff_t next, index;
	int i, freed = 0;
	bool truncate_op = (lend == LLONG_MAX);

<<<<<<< HEAD
	vma_init(&pseudo_vma, current->mm);
	pseudo_vma.vm_flags = (VM_HUGETLB | VM_MAYSHARE | VM_SHARED);
=======
>>>>>>> upstream/android-13
	pagevec_init(&pvec);
	next = start;
	while (next < end) {
		/*
		 * When no more pages are found, we are done.
		 */
		if (!pagevec_lookup_range(&pvec, mapping, &next, end - 1))
			break;

		for (i = 0; i < pagevec_count(&pvec); ++i) {
			struct page *page = pvec.pages[i];
<<<<<<< HEAD
			u32 hash;

			index = page->index;
			hash = hugetlb_fault_mutex_hash(h, mapping, index, 0);
			mutex_lock(&hugetlb_fault_mutex_table[hash]);
=======
			u32 hash = 0;

			index = page->index;
			if (!truncate_op) {
				/*
				 * Only need to hold the fault mutex in the
				 * hole punch case.  This prevents races with
				 * page faults.  Races are not possible in the
				 * case of truncation.
				 */
				hash = hugetlb_fault_mutex_hash(mapping, index);
				mutex_lock(&hugetlb_fault_mutex_table[hash]);
			}
>>>>>>> upstream/android-13

			/*
			 * If page is mapped, it was faulted in after being
			 * unmapped in caller.  Unmap (again) now after taking
			 * the fault mutex.  The mutex will prevent faults
			 * until we finish removing the page.
			 *
			 * This race can only happen in the hole punch case.
			 * Getting here in a truncate operation is a bug.
			 */
			if (unlikely(page_mapped(page))) {
				BUG_ON(truncate_op);

<<<<<<< HEAD
				i_mmap_lock_write(mapping);
=======
				mutex_unlock(&hugetlb_fault_mutex_table[hash]);
				i_mmap_lock_write(mapping);
				mutex_lock(&hugetlb_fault_mutex_table[hash]);
>>>>>>> upstream/android-13
				hugetlb_vmdelete_list(&mapping->i_mmap,
					index * pages_per_huge_page(h),
					(index + 1) * pages_per_huge_page(h));
				i_mmap_unlock_write(mapping);
			}

			lock_page(page);
			/*
			 * We must free the huge page and remove from page
			 * cache (remove_huge_page) BEFORE removing the
			 * region/reserve map (hugetlb_unreserve_pages).  In
			 * rare out of memory conditions, removal of the
			 * region/reserve map could fail. Correspondingly,
			 * the subpool and global reserve usage count can need
			 * to be adjusted.
			 */
<<<<<<< HEAD
			VM_BUG_ON(PagePrivate(page));
=======
			VM_BUG_ON(HPageRestoreReserve(page));
>>>>>>> upstream/android-13
			remove_huge_page(page);
			freed++;
			if (!truncate_op) {
				if (unlikely(hugetlb_unreserve_pages(inode,
							index, index + 1, 1)))
					hugetlb_fix_reserve_counts(inode);
			}

			unlock_page(page);
<<<<<<< HEAD
			mutex_unlock(&hugetlb_fault_mutex_table[hash]);
=======
			if (!truncate_op)
				mutex_unlock(&hugetlb_fault_mutex_table[hash]);
>>>>>>> upstream/android-13
		}
		huge_pagevec_release(&pvec);
		cond_resched();
	}

	if (truncate_op)
		(void)hugetlb_unreserve_pages(inode, start, LONG_MAX, freed);
}

static void hugetlbfs_evict_inode(struct inode *inode)
{
	struct resv_map *resv_map;

	remove_inode_hugepages(inode, 0, LLONG_MAX);
<<<<<<< HEAD
	resv_map = (struct resv_map *)inode->i_mapping->private_data;
	/* root inode doesn't have the resv_map, so we should check it */
=======

	/*
	 * Get the resv_map from the address space embedded in the inode.
	 * This is the address space which points to any resv_map allocated
	 * at inode creation time.  If this is a device special inode,
	 * i_mapping may not point to the original address space.
	 */
	resv_map = (struct resv_map *)(&inode->i_data)->private_data;
	/* Only regular and link inodes have associated reserve maps */
>>>>>>> upstream/android-13
	if (resv_map)
		resv_map_release(&resv_map->refs);
	clear_inode(inode);
}

<<<<<<< HEAD
static int hugetlb_vmtruncate(struct inode *inode, loff_t offset)
=======
static void hugetlb_vmtruncate(struct inode *inode, loff_t offset)
>>>>>>> upstream/android-13
{
	pgoff_t pgoff;
	struct address_space *mapping = inode->i_mapping;
	struct hstate *h = hstate_inode(inode);

	BUG_ON(offset & ~huge_page_mask(h));
	pgoff = offset >> PAGE_SHIFT;

<<<<<<< HEAD
	i_size_write(inode, offset);
	i_mmap_lock_write(mapping);
=======
	i_mmap_lock_write(mapping);
	i_size_write(inode, offset);
>>>>>>> upstream/android-13
	if (!RB_EMPTY_ROOT(&mapping->i_mmap.rb_root))
		hugetlb_vmdelete_list(&mapping->i_mmap, pgoff, 0);
	i_mmap_unlock_write(mapping);
	remove_inode_hugepages(inode, offset, LLONG_MAX);
<<<<<<< HEAD
	return 0;
=======
>>>>>>> upstream/android-13
}

static long hugetlbfs_punch_hole(struct inode *inode, loff_t offset, loff_t len)
{
	struct hstate *h = hstate_inode(inode);
	loff_t hpage_size = huge_page_size(h);
	loff_t hole_start, hole_end;

	/*
	 * For hole punch round up the beginning offset of the hole and
	 * round down the end.
	 */
	hole_start = round_up(offset, hpage_size);
	hole_end = round_down(offset + len, hpage_size);

	if (hole_end > hole_start) {
		struct address_space *mapping = inode->i_mapping;
		struct hugetlbfs_inode_info *info = HUGETLBFS_I(inode);

		inode_lock(inode);

<<<<<<< HEAD
		/* protected by i_mutex */
=======
		/* protected by i_rwsem */
>>>>>>> upstream/android-13
		if (info->seals & (F_SEAL_WRITE | F_SEAL_FUTURE_WRITE)) {
			inode_unlock(inode);
			return -EPERM;
		}

		i_mmap_lock_write(mapping);
		if (!RB_EMPTY_ROOT(&mapping->i_mmap.rb_root))
			hugetlb_vmdelete_list(&mapping->i_mmap,
						hole_start >> PAGE_SHIFT,
						hole_end  >> PAGE_SHIFT);
		i_mmap_unlock_write(mapping);
		remove_inode_hugepages(inode, hole_start, hole_end);
		inode_unlock(inode);
	}

	return 0;
}

static long hugetlbfs_fallocate(struct file *file, int mode, loff_t offset,
				loff_t len)
{
	struct inode *inode = file_inode(file);
	struct hugetlbfs_inode_info *info = HUGETLBFS_I(inode);
	struct address_space *mapping = inode->i_mapping;
	struct hstate *h = hstate_inode(inode);
	struct vm_area_struct pseudo_vma;
	struct mm_struct *mm = current->mm;
	loff_t hpage_size = huge_page_size(h);
	unsigned long hpage_shift = huge_page_shift(h);
	pgoff_t start, index, end;
	int error;
	u32 hash;

	if (mode & ~(FALLOC_FL_KEEP_SIZE | FALLOC_FL_PUNCH_HOLE))
		return -EOPNOTSUPP;

	if (mode & FALLOC_FL_PUNCH_HOLE)
		return hugetlbfs_punch_hole(inode, offset, len);

	/*
	 * Default preallocate case.
	 * For this range, start is rounded down and end is rounded up
	 * as well as being converted to page offsets.
	 */
	start = offset >> hpage_shift;
	end = (offset + len + hpage_size - 1) >> hpage_shift;

	inode_lock(inode);

	/* We need to check rlimit even when FALLOC_FL_KEEP_SIZE */
	error = inode_newsize_ok(inode, offset + len);
	if (error)
		goto out;

	if ((info->seals & F_SEAL_GROW) && offset + len > inode->i_size) {
		error = -EPERM;
		goto out;
	}

	/*
	 * Initialize a pseudo vma as this is required by the huge page
	 * allocation routines.  If NUMA is configured, use page index
	 * as input to create an allocation policy.
	 */
	vma_init(&pseudo_vma, mm);
	pseudo_vma.vm_flags = (VM_HUGETLB | VM_MAYSHARE | VM_SHARED);
	pseudo_vma.vm_file = file;

	for (index = start; index < end; index++) {
		/*
		 * This is supposed to be the vaddr where the page is being
		 * faulted in, but we have no vaddr here.
		 */
		struct page *page;
		unsigned long addr;
<<<<<<< HEAD
		int avoid_reserve = 0;
=======
>>>>>>> upstream/android-13

		cond_resched();

		/*
		 * fallocate(2) manpage permits EINTR; we may have been
		 * interrupted because we are using up too much memory.
		 */
		if (signal_pending(current)) {
			error = -EINTR;
			break;
		}

		/* Set numa allocation policy based on index */
		hugetlb_set_vma_policy(&pseudo_vma, inode, index);

		/* addr is the offset within the file (zero based) */
		addr = index * hpage_size;

<<<<<<< HEAD
		/* mutex taken here, fault path and hole punch */
		hash = hugetlb_fault_mutex_hash(h, mapping, index, addr);
=======
		/*
		 * fault mutex taken here, protects against fault path
		 * and hole punch.  inode_lock previously taken protects
		 * against truncation.
		 */
		hash = hugetlb_fault_mutex_hash(mapping, index);
>>>>>>> upstream/android-13
		mutex_lock(&hugetlb_fault_mutex_table[hash]);

		/* See if already present in mapping to avoid alloc/free */
		page = find_get_page(mapping, index);
		if (page) {
			put_page(page);
			mutex_unlock(&hugetlb_fault_mutex_table[hash]);
			hugetlb_drop_vma_policy(&pseudo_vma);
			continue;
		}

<<<<<<< HEAD
		/* Allocate page and add to page cache */
		page = alloc_huge_page(&pseudo_vma, addr, avoid_reserve);
=======
		/*
		 * Allocate page without setting the avoid_reserve argument.
		 * There certainly are no reserves associated with the
		 * pseudo_vma.  However, there could be shared mappings with
		 * reserves for the file at the inode level.  If we fallocate
		 * pages in these areas, we need to consume the reserves
		 * to keep reservation accounting consistent.
		 */
		page = alloc_huge_page(&pseudo_vma, addr, 0);
>>>>>>> upstream/android-13
		hugetlb_drop_vma_policy(&pseudo_vma);
		if (IS_ERR(page)) {
			mutex_unlock(&hugetlb_fault_mutex_table[hash]);
			error = PTR_ERR(page);
			goto out;
		}
		clear_huge_page(page, addr, pages_per_huge_page(h));
		__SetPageUptodate(page);
		error = huge_add_to_page_cache(page, mapping, index);
		if (unlikely(error)) {
<<<<<<< HEAD
=======
			restore_reserve_on_error(h, &pseudo_vma, addr, page);
>>>>>>> upstream/android-13
			put_page(page);
			mutex_unlock(&hugetlb_fault_mutex_table[hash]);
			goto out;
		}

		mutex_unlock(&hugetlb_fault_mutex_table[hash]);

<<<<<<< HEAD
		set_page_huge_active(page);
=======
		SetHPageMigratable(page);
>>>>>>> upstream/android-13
		/*
		 * unlock_page because locked by add_to_page_cache()
		 * put_page() due to reference from alloc_huge_page()
		 */
		unlock_page(page);
		put_page(page);
	}

	if (!(mode & FALLOC_FL_KEEP_SIZE) && offset + len > inode->i_size)
		i_size_write(inode, offset + len);
	inode->i_ctime = current_time(inode);
out:
	inode_unlock(inode);
	return error;
}

<<<<<<< HEAD
static int hugetlbfs_setattr(struct dentry *dentry, struct iattr *attr)
=======
static int hugetlbfs_setattr(struct user_namespace *mnt_userns,
			     struct dentry *dentry, struct iattr *attr)
>>>>>>> upstream/android-13
{
	struct inode *inode = d_inode(dentry);
	struct hstate *h = hstate_inode(inode);
	int error;
	unsigned int ia_valid = attr->ia_valid;
	struct hugetlbfs_inode_info *info = HUGETLBFS_I(inode);

<<<<<<< HEAD
	BUG_ON(!inode);

	error = setattr_prepare(dentry, attr);
=======
	error = setattr_prepare(&init_user_ns, dentry, attr);
>>>>>>> upstream/android-13
	if (error)
		return error;

	if (ia_valid & ATTR_SIZE) {
		loff_t oldsize = inode->i_size;
		loff_t newsize = attr->ia_size;

		if (newsize & ~huge_page_mask(h))
			return -EINVAL;
<<<<<<< HEAD
		/* protected by i_mutex */
		if ((newsize < oldsize && (info->seals & F_SEAL_SHRINK)) ||
		    (newsize > oldsize && (info->seals & F_SEAL_GROW)))
			return -EPERM;
		error = hugetlb_vmtruncate(inode, newsize);
		if (error)
			return error;
	}

	setattr_copy(inode, attr);
=======
		/* protected by i_rwsem */
		if ((newsize < oldsize && (info->seals & F_SEAL_SHRINK)) ||
		    (newsize > oldsize && (info->seals & F_SEAL_GROW)))
			return -EPERM;
		hugetlb_vmtruncate(inode, newsize);
	}

	setattr_copy(&init_user_ns, inode, attr);
>>>>>>> upstream/android-13
	mark_inode_dirty(inode);
	return 0;
}

static struct inode *hugetlbfs_get_root(struct super_block *sb,
<<<<<<< HEAD
					struct hugetlbfs_config *config)
=======
					struct hugetlbfs_fs_context *ctx)
>>>>>>> upstream/android-13
{
	struct inode *inode;

	inode = new_inode(sb);
	if (inode) {
		inode->i_ino = get_next_ino();
<<<<<<< HEAD
		inode->i_mode = S_IFDIR | config->mode;
		inode->i_uid = config->uid;
		inode->i_gid = config->gid;
=======
		inode->i_mode = S_IFDIR | ctx->mode;
		inode->i_uid = ctx->uid;
		inode->i_gid = ctx->gid;
>>>>>>> upstream/android-13
		inode->i_atime = inode->i_mtime = inode->i_ctime = current_time(inode);
		inode->i_op = &hugetlbfs_dir_inode_operations;
		inode->i_fop = &simple_dir_operations;
		/* directory inodes start off with i_nlink == 2 (for "." entry) */
		inc_nlink(inode);
		lockdep_annotate_inode_mutex_key(inode);
	}
	return inode;
}

/*
 * Hugetlbfs is not reclaimable; therefore its i_mmap_rwsem will never
 * be taken from reclaim -- unlike regular filesystems. This needs an
 * annotation because huge_pmd_share() does an allocation under hugetlb's
 * i_mmap_rwsem.
 */
static struct lock_class_key hugetlbfs_i_mmap_rwsem_key;

static struct inode *hugetlbfs_get_inode(struct super_block *sb,
					struct inode *dir,
					umode_t mode, dev_t dev)
{
	struct inode *inode;
	struct resv_map *resv_map = NULL;

	/*
	 * Reserve maps are only needed for inodes that can have associated
	 * page allocations.
	 */
	if (S_ISREG(mode) || S_ISLNK(mode)) {
		resv_map = resv_map_alloc();
		if (!resv_map)
			return NULL;
	}

	inode = new_inode(sb);
	if (inode) {
		struct hugetlbfs_inode_info *info = HUGETLBFS_I(inode);

		inode->i_ino = get_next_ino();
<<<<<<< HEAD
		inode_init_owner(inode, dir, mode);
=======
		inode_init_owner(&init_user_ns, inode, dir, mode);
>>>>>>> upstream/android-13
		lockdep_set_class(&inode->i_mapping->i_mmap_rwsem,
				&hugetlbfs_i_mmap_rwsem_key);
		inode->i_mapping->a_ops = &hugetlbfs_aops;
		inode->i_atime = inode->i_mtime = inode->i_ctime = current_time(inode);
		inode->i_mapping->private_data = resv_map;
		info->seals = F_SEAL_SEAL;
		switch (mode & S_IFMT) {
		default:
			init_special_inode(inode, mode, dev);
			break;
		case S_IFREG:
			inode->i_op = &hugetlbfs_inode_operations;
			inode->i_fop = &hugetlbfs_file_operations;
			break;
		case S_IFDIR:
			inode->i_op = &hugetlbfs_dir_inode_operations;
			inode->i_fop = &simple_dir_operations;

			/* directory inodes start off with i_nlink == 2 (for "." entry) */
			inc_nlink(inode);
			break;
		case S_IFLNK:
			inode->i_op = &page_symlink_inode_operations;
			inode_nohighmem(inode);
			break;
		}
		lockdep_annotate_inode_mutex_key(inode);
	} else {
		if (resv_map)
			kref_put(&resv_map->refs, resv_map_release);
	}

	return inode;
}

/*
 * File creation. Allocate an inode, and we're done..
 */
<<<<<<< HEAD
static int hugetlbfs_mknod(struct inode *dir,
			struct dentry *dentry, umode_t mode, dev_t dev)
=======
static int do_hugetlbfs_mknod(struct inode *dir,
			struct dentry *dentry,
			umode_t mode,
			dev_t dev,
			bool tmpfile)
>>>>>>> upstream/android-13
{
	struct inode *inode;
	int error = -ENOSPC;

	inode = hugetlbfs_get_inode(dir->i_sb, dir, mode, dev);
	if (inode) {
		dir->i_ctime = dir->i_mtime = current_time(dir);
<<<<<<< HEAD
		d_instantiate(dentry, inode);
		dget(dentry);	/* Extra count - pin the dentry in core */
=======
		if (tmpfile) {
			d_tmpfile(dentry, inode);
		} else {
			d_instantiate(dentry, inode);
			dget(dentry);/* Extra count - pin the dentry in core */
		}
>>>>>>> upstream/android-13
		error = 0;
	}
	return error;
}

<<<<<<< HEAD
static int hugetlbfs_mkdir(struct inode *dir, struct dentry *dentry, umode_t mode)
{
	int retval = hugetlbfs_mknod(dir, dentry, mode | S_IFDIR, 0);
=======
static int hugetlbfs_mknod(struct user_namespace *mnt_userns, struct inode *dir,
			   struct dentry *dentry, umode_t mode, dev_t dev)
{
	return do_hugetlbfs_mknod(dir, dentry, mode, dev, false);
}

static int hugetlbfs_mkdir(struct user_namespace *mnt_userns, struct inode *dir,
			   struct dentry *dentry, umode_t mode)
{
	int retval = hugetlbfs_mknod(&init_user_ns, dir, dentry,
				     mode | S_IFDIR, 0);
>>>>>>> upstream/android-13
	if (!retval)
		inc_nlink(dir);
	return retval;
}

<<<<<<< HEAD
static int hugetlbfs_create(struct inode *dir, struct dentry *dentry, umode_t mode, bool excl)
{
	return hugetlbfs_mknod(dir, dentry, mode | S_IFREG, 0);
}

static int hugetlbfs_symlink(struct inode *dir,
			struct dentry *dentry, const char *symname)
=======
static int hugetlbfs_create(struct user_namespace *mnt_userns,
			    struct inode *dir, struct dentry *dentry,
			    umode_t mode, bool excl)
{
	return hugetlbfs_mknod(&init_user_ns, dir, dentry, mode | S_IFREG, 0);
}

static int hugetlbfs_tmpfile(struct user_namespace *mnt_userns,
			     struct inode *dir, struct dentry *dentry,
			     umode_t mode)
{
	return do_hugetlbfs_mknod(dir, dentry, mode | S_IFREG, 0, true);
}

static int hugetlbfs_symlink(struct user_namespace *mnt_userns,
			     struct inode *dir, struct dentry *dentry,
			     const char *symname)
>>>>>>> upstream/android-13
{
	struct inode *inode;
	int error = -ENOSPC;

	inode = hugetlbfs_get_inode(dir->i_sb, dir, S_IFLNK|S_IRWXUGO, 0);
	if (inode) {
		int l = strlen(symname)+1;
		error = page_symlink(inode, symname, l);
		if (!error) {
			d_instantiate(dentry, inode);
			dget(dentry);
		} else
			iput(inode);
	}
	dir->i_ctime = dir->i_mtime = current_time(dir);

	return error;
}

<<<<<<< HEAD
/*
 * mark the head page dirty
 */
static int hugetlbfs_set_page_dirty(struct page *page)
{
	struct page *head = compound_head(page);

	SetPageDirty(head);
	return 0;
}

=======
>>>>>>> upstream/android-13
static int hugetlbfs_migrate_page(struct address_space *mapping,
				struct page *newpage, struct page *page,
				enum migrate_mode mode)
{
	int rc;

	rc = migrate_huge_page_move_mapping(mapping, newpage, page);
	if (rc != MIGRATEPAGE_SUCCESS)
		return rc;

<<<<<<< HEAD
	/*
	 * page_private is subpool pointer in hugetlb pages.  Transfer to
	 * new page.  PagePrivate is not associated with page_private for
	 * hugetlb pages and can not be set here as only page_huge_active
	 * pages can be migrated.
	 */
	if (page_private(page)) {
		set_page_private(newpage, page_private(page));
		set_page_private(page, 0);
=======
	if (hugetlb_page_subpool(page)) {
		hugetlb_set_page_subpool(newpage, hugetlb_page_subpool(page));
		hugetlb_set_page_subpool(page, NULL);
>>>>>>> upstream/android-13
	}

	if (mode != MIGRATE_SYNC_NO_COPY)
		migrate_page_copy(newpage, page);
	else
		migrate_page_states(newpage, page);

	return MIGRATEPAGE_SUCCESS;
}

static int hugetlbfs_error_remove_page(struct address_space *mapping,
				struct page *page)
{
	struct inode *inode = mapping->host;
	pgoff_t index = page->index;

	remove_huge_page(page);
	if (unlikely(hugetlb_unreserve_pages(inode, index, index + 1, 1)))
		hugetlb_fix_reserve_counts(inode);

	return 0;
}

/*
 * Display the mount options in /proc/mounts.
 */
static int hugetlbfs_show_options(struct seq_file *m, struct dentry *root)
{
	struct hugetlbfs_sb_info *sbinfo = HUGETLBFS_SB(root->d_sb);
	struct hugepage_subpool *spool = sbinfo->spool;
	unsigned long hpage_size = huge_page_size(sbinfo->hstate);
	unsigned hpage_shift = huge_page_shift(sbinfo->hstate);
	char mod;

	if (!uid_eq(sbinfo->uid, GLOBAL_ROOT_UID))
		seq_printf(m, ",uid=%u",
			   from_kuid_munged(&init_user_ns, sbinfo->uid));
	if (!gid_eq(sbinfo->gid, GLOBAL_ROOT_GID))
		seq_printf(m, ",gid=%u",
			   from_kgid_munged(&init_user_ns, sbinfo->gid));
	if (sbinfo->mode != 0755)
		seq_printf(m, ",mode=%o", sbinfo->mode);
	if (sbinfo->max_inodes != -1)
		seq_printf(m, ",nr_inodes=%lu", sbinfo->max_inodes);

	hpage_size /= 1024;
	mod = 'K';
	if (hpage_size >= 1024) {
		hpage_size /= 1024;
		mod = 'M';
	}
	seq_printf(m, ",pagesize=%lu%c", hpage_size, mod);
	if (spool) {
		if (spool->max_hpages != -1)
			seq_printf(m, ",size=%llu",
				   (unsigned long long)spool->max_hpages << hpage_shift);
		if (spool->min_hpages != -1)
			seq_printf(m, ",min_size=%llu",
				   (unsigned long long)spool->min_hpages << hpage_shift);
	}
	return 0;
}

static int hugetlbfs_statfs(struct dentry *dentry, struct kstatfs *buf)
{
	struct hugetlbfs_sb_info *sbinfo = HUGETLBFS_SB(dentry->d_sb);
	struct hstate *h = hstate_inode(d_inode(dentry));

	buf->f_type = HUGETLBFS_MAGIC;
	buf->f_bsize = huge_page_size(h);
	if (sbinfo) {
		spin_lock(&sbinfo->stat_lock);
		/* If no limits set, just report 0 for max/free/used
		 * blocks, like simple_statfs() */
		if (sbinfo->spool) {
			long free_pages;

			spin_lock(&sbinfo->spool->lock);
			buf->f_blocks = sbinfo->spool->max_hpages;
			free_pages = sbinfo->spool->max_hpages
				- sbinfo->spool->used_hpages;
			buf->f_bavail = buf->f_bfree = free_pages;
			spin_unlock(&sbinfo->spool->lock);
			buf->f_files = sbinfo->max_inodes;
			buf->f_ffree = sbinfo->free_inodes;
		}
		spin_unlock(&sbinfo->stat_lock);
	}
	buf->f_namelen = NAME_MAX;
	return 0;
}

static void hugetlbfs_put_super(struct super_block *sb)
{
	struct hugetlbfs_sb_info *sbi = HUGETLBFS_SB(sb);

	if (sbi) {
		sb->s_fs_info = NULL;

		if (sbi->spool)
			hugepage_put_subpool(sbi->spool);

		kfree(sbi);
	}
}

static inline int hugetlbfs_dec_free_inodes(struct hugetlbfs_sb_info *sbinfo)
{
	if (sbinfo->free_inodes >= 0) {
		spin_lock(&sbinfo->stat_lock);
		if (unlikely(!sbinfo->free_inodes)) {
			spin_unlock(&sbinfo->stat_lock);
			return 0;
		}
		sbinfo->free_inodes--;
		spin_unlock(&sbinfo->stat_lock);
	}

	return 1;
}

static void hugetlbfs_inc_free_inodes(struct hugetlbfs_sb_info *sbinfo)
{
	if (sbinfo->free_inodes >= 0) {
		spin_lock(&sbinfo->stat_lock);
		sbinfo->free_inodes++;
		spin_unlock(&sbinfo->stat_lock);
	}
}


static struct kmem_cache *hugetlbfs_inode_cachep;

static struct inode *hugetlbfs_alloc_inode(struct super_block *sb)
{
	struct hugetlbfs_sb_info *sbinfo = HUGETLBFS_SB(sb);
	struct hugetlbfs_inode_info *p;

	if (unlikely(!hugetlbfs_dec_free_inodes(sbinfo)))
		return NULL;
	p = kmem_cache_alloc(hugetlbfs_inode_cachep, GFP_KERNEL);
	if (unlikely(!p)) {
		hugetlbfs_inc_free_inodes(sbinfo);
		return NULL;
	}

	/*
	 * Any time after allocation, hugetlbfs_destroy_inode can be called
	 * for the inode.  mpol_free_shared_policy is unconditionally called
	 * as part of hugetlbfs_destroy_inode.  So, initialize policy here
	 * in case of a quick call to destroy.
	 *
	 * Note that the policy is initialized even if we are creating a
	 * private inode.  This simplifies hugetlbfs_destroy_inode.
	 */
	mpol_shared_policy_init(&p->policy, NULL);

	return &p->vfs_inode;
}

<<<<<<< HEAD
static void hugetlbfs_i_callback(struct rcu_head *head)
{
	struct inode *inode = container_of(head, struct inode, i_rcu);
=======
static void hugetlbfs_free_inode(struct inode *inode)
{
>>>>>>> upstream/android-13
	kmem_cache_free(hugetlbfs_inode_cachep, HUGETLBFS_I(inode));
}

static void hugetlbfs_destroy_inode(struct inode *inode)
{
	hugetlbfs_inc_free_inodes(HUGETLBFS_SB(inode->i_sb));
	mpol_free_shared_policy(&HUGETLBFS_I(inode)->policy);
<<<<<<< HEAD
	call_rcu(&inode->i_rcu, hugetlbfs_i_callback);
=======
>>>>>>> upstream/android-13
}

static const struct address_space_operations hugetlbfs_aops = {
	.write_begin	= hugetlbfs_write_begin,
	.write_end	= hugetlbfs_write_end,
<<<<<<< HEAD
	.set_page_dirty	= hugetlbfs_set_page_dirty,
=======
	.set_page_dirty	=  __set_page_dirty_no_writeback,
>>>>>>> upstream/android-13
	.migratepage    = hugetlbfs_migrate_page,
	.error_remove_page	= hugetlbfs_error_remove_page,
};


static void init_once(void *foo)
{
	struct hugetlbfs_inode_info *ei = (struct hugetlbfs_inode_info *)foo;

	inode_init_once(&ei->vfs_inode);
}

const struct file_operations hugetlbfs_file_operations = {
	.read_iter		= hugetlbfs_read_iter,
	.mmap			= hugetlbfs_file_mmap,
	.fsync			= noop_fsync,
	.get_unmapped_area	= hugetlb_get_unmapped_area,
	.llseek			= default_llseek,
	.fallocate		= hugetlbfs_fallocate,
};

static const struct inode_operations hugetlbfs_dir_inode_operations = {
	.create		= hugetlbfs_create,
	.lookup		= simple_lookup,
	.link		= simple_link,
	.unlink		= simple_unlink,
	.symlink	= hugetlbfs_symlink,
	.mkdir		= hugetlbfs_mkdir,
	.rmdir		= simple_rmdir,
	.mknod		= hugetlbfs_mknod,
	.rename		= simple_rename,
	.setattr	= hugetlbfs_setattr,
<<<<<<< HEAD
=======
	.tmpfile	= hugetlbfs_tmpfile,
>>>>>>> upstream/android-13
};

static const struct inode_operations hugetlbfs_inode_operations = {
	.setattr	= hugetlbfs_setattr,
};

static const struct super_operations hugetlbfs_ops = {
	.alloc_inode    = hugetlbfs_alloc_inode,
<<<<<<< HEAD
=======
	.free_inode     = hugetlbfs_free_inode,
>>>>>>> upstream/android-13
	.destroy_inode  = hugetlbfs_destroy_inode,
	.evict_inode	= hugetlbfs_evict_inode,
	.statfs		= hugetlbfs_statfs,
	.put_super	= hugetlbfs_put_super,
	.show_options	= hugetlbfs_show_options,
};

<<<<<<< HEAD
enum hugetlbfs_size_type { NO_SIZE, SIZE_STD, SIZE_PERCENT };

=======
>>>>>>> upstream/android-13
/*
 * Convert size option passed from command line to number of huge pages
 * in the pool specified by hstate.  Size option could be in bytes
 * (val_type == SIZE_STD) or percentage of the pool (val_type == SIZE_PERCENT).
 */
static long
hugetlbfs_size_to_hpages(struct hstate *h, unsigned long long size_opt,
			 enum hugetlbfs_size_type val_type)
{
	if (val_type == NO_SIZE)
		return -1;

	if (val_type == SIZE_PERCENT) {
		size_opt <<= huge_page_shift(h);
		size_opt *= h->max_huge_pages;
		do_div(size_opt, 100);
	}

	size_opt >>= huge_page_shift(h);
	return size_opt;
}

<<<<<<< HEAD
static int
hugetlbfs_parse_options(char *options, struct hugetlbfs_config *pconfig)
{
	char *p, *rest;
	substring_t args[MAX_OPT_ARGS];
	int option;
	unsigned long long max_size_opt = 0, min_size_opt = 0;
	enum hugetlbfs_size_type max_val_type = NO_SIZE, min_val_type = NO_SIZE;

	if (!options)
		return 0;

	while ((p = strsep(&options, ",")) != NULL) {
		int token;
		if (!*p)
			continue;

		token = match_token(p, tokens, args);
		switch (token) {
		case Opt_uid:
			if (match_int(&args[0], &option))
 				goto bad_val;
			pconfig->uid = make_kuid(current_user_ns(), option);
			if (!uid_valid(pconfig->uid))
				goto bad_val;
			break;

		case Opt_gid:
			if (match_int(&args[0], &option))
 				goto bad_val;
			pconfig->gid = make_kgid(current_user_ns(), option);
			if (!gid_valid(pconfig->gid))
				goto bad_val;
			break;

		case Opt_mode:
			if (match_octal(&args[0], &option))
 				goto bad_val;
			pconfig->mode = option & 01777U;
			break;

		case Opt_size: {
			/* memparse() will accept a K/M/G without a digit */
			if (!isdigit(*args[0].from))
				goto bad_val;
			max_size_opt = memparse(args[0].from, &rest);
			max_val_type = SIZE_STD;
			if (*rest == '%')
				max_val_type = SIZE_PERCENT;
			break;
		}

		case Opt_nr_inodes:
			/* memparse() will accept a K/M/G without a digit */
			if (!isdigit(*args[0].from))
				goto bad_val;
			pconfig->nr_inodes = memparse(args[0].from, &rest);
			break;

		case Opt_pagesize: {
			unsigned long ps;
			ps = memparse(args[0].from, &rest);
			pconfig->hstate = size_to_hstate(ps);
			if (!pconfig->hstate) {
				pr_err("Unsupported page size %lu MB\n",
					ps >> 20);
				return -EINVAL;
			}
			break;
		}

		case Opt_min_size: {
			/* memparse() will accept a K/M/G without a digit */
			if (!isdigit(*args[0].from))
				goto bad_val;
			min_size_opt = memparse(args[0].from, &rest);
			min_val_type = SIZE_STD;
			if (*rest == '%')
				min_val_type = SIZE_PERCENT;
			break;
		}

		default:
			pr_err("Bad mount option: \"%s\"\n", p);
			return -EINVAL;
			break;
		}
	}

=======
/*
 * Parse one mount parameter.
 */
static int hugetlbfs_parse_param(struct fs_context *fc, struct fs_parameter *param)
{
	struct hugetlbfs_fs_context *ctx = fc->fs_private;
	struct fs_parse_result result;
	char *rest;
	unsigned long ps;
	int opt;

	opt = fs_parse(fc, hugetlb_fs_parameters, param, &result);
	if (opt < 0)
		return opt;

	switch (opt) {
	case Opt_uid:
		ctx->uid = make_kuid(current_user_ns(), result.uint_32);
		if (!uid_valid(ctx->uid))
			goto bad_val;
		return 0;

	case Opt_gid:
		ctx->gid = make_kgid(current_user_ns(), result.uint_32);
		if (!gid_valid(ctx->gid))
			goto bad_val;
		return 0;

	case Opt_mode:
		ctx->mode = result.uint_32 & 01777U;
		return 0;

	case Opt_size:
		/* memparse() will accept a K/M/G without a digit */
		if (!isdigit(param->string[0]))
			goto bad_val;
		ctx->max_size_opt = memparse(param->string, &rest);
		ctx->max_val_type = SIZE_STD;
		if (*rest == '%')
			ctx->max_val_type = SIZE_PERCENT;
		return 0;

	case Opt_nr_inodes:
		/* memparse() will accept a K/M/G without a digit */
		if (!isdigit(param->string[0]))
			goto bad_val;
		ctx->nr_inodes = memparse(param->string, &rest);
		return 0;

	case Opt_pagesize:
		ps = memparse(param->string, &rest);
		ctx->hstate = size_to_hstate(ps);
		if (!ctx->hstate) {
			pr_err("Unsupported page size %lu MB\n", ps >> 20);
			return -EINVAL;
		}
		return 0;

	case Opt_min_size:
		/* memparse() will accept a K/M/G without a digit */
		if (!isdigit(param->string[0]))
			goto bad_val;
		ctx->min_size_opt = memparse(param->string, &rest);
		ctx->min_val_type = SIZE_STD;
		if (*rest == '%')
			ctx->min_val_type = SIZE_PERCENT;
		return 0;

	default:
		return -EINVAL;
	}

bad_val:
	return invalfc(fc, "Bad value '%s' for mount option '%s'\n",
		      param->string, param->key);
}

/*
 * Validate the parsed options.
 */
static int hugetlbfs_validate(struct fs_context *fc)
{
	struct hugetlbfs_fs_context *ctx = fc->fs_private;

>>>>>>> upstream/android-13
	/*
	 * Use huge page pool size (in hstate) to convert the size
	 * options to number of huge pages.  If NO_SIZE, -1 is returned.
	 */
<<<<<<< HEAD
	pconfig->max_hpages = hugetlbfs_size_to_hpages(pconfig->hstate,
						max_size_opt, max_val_type);
	pconfig->min_hpages = hugetlbfs_size_to_hpages(pconfig->hstate,
						min_size_opt, min_val_type);
=======
	ctx->max_hpages = hugetlbfs_size_to_hpages(ctx->hstate,
						   ctx->max_size_opt,
						   ctx->max_val_type);
	ctx->min_hpages = hugetlbfs_size_to_hpages(ctx->hstate,
						   ctx->min_size_opt,
						   ctx->min_val_type);
>>>>>>> upstream/android-13

	/*
	 * If max_size was specified, then min_size must be smaller
	 */
<<<<<<< HEAD
	if (max_val_type > NO_SIZE &&
	    pconfig->min_hpages > pconfig->max_hpages) {
		pr_err("minimum size can not be greater than maximum size\n");
=======
	if (ctx->max_val_type > NO_SIZE &&
	    ctx->min_hpages > ctx->max_hpages) {
		pr_err("Minimum size can not be greater than maximum size\n");
>>>>>>> upstream/android-13
		return -EINVAL;
	}

	return 0;
<<<<<<< HEAD

bad_val:
	pr_err("Bad value '%s' for mount option '%s'\n", args[0].from, p);
 	return -EINVAL;
}

static int
hugetlbfs_fill_super(struct super_block *sb, void *data, int silent)
{
	int ret;
	struct hugetlbfs_config config;
	struct hugetlbfs_sb_info *sbinfo;

	config.max_hpages = -1; /* No limit on size by default */
	config.nr_inodes = -1; /* No limit on number of inodes by default */
	config.uid = current_fsuid();
	config.gid = current_fsgid();
	config.mode = 0755;
	config.hstate = &default_hstate;
	config.min_hpages = -1; /* No default minimum size */
	ret = hugetlbfs_parse_options(data, &config);
	if (ret)
		return ret;

=======
}

static int
hugetlbfs_fill_super(struct super_block *sb, struct fs_context *fc)
{
	struct hugetlbfs_fs_context *ctx = fc->fs_private;
	struct hugetlbfs_sb_info *sbinfo;

>>>>>>> upstream/android-13
	sbinfo = kmalloc(sizeof(struct hugetlbfs_sb_info), GFP_KERNEL);
	if (!sbinfo)
		return -ENOMEM;
	sb->s_fs_info = sbinfo;
<<<<<<< HEAD
	sbinfo->hstate = config.hstate;
	spin_lock_init(&sbinfo->stat_lock);
	sbinfo->max_inodes = config.nr_inodes;
	sbinfo->free_inodes = config.nr_inodes;
	sbinfo->spool = NULL;
	sbinfo->uid = config.uid;
	sbinfo->gid = config.gid;
	sbinfo->mode = config.mode;

	/*
	 * Allocate and initialize subpool if maximum or minimum size is
	 * specified.  Any needed reservations (for minimim size) are taken
	 * taken when the subpool is created.
	 */
	if (config.max_hpages != -1 || config.min_hpages != -1) {
		sbinfo->spool = hugepage_new_subpool(config.hstate,
							config.max_hpages,
							config.min_hpages);
=======
	spin_lock_init(&sbinfo->stat_lock);
	sbinfo->hstate		= ctx->hstate;
	sbinfo->max_inodes	= ctx->nr_inodes;
	sbinfo->free_inodes	= ctx->nr_inodes;
	sbinfo->spool		= NULL;
	sbinfo->uid		= ctx->uid;
	sbinfo->gid		= ctx->gid;
	sbinfo->mode		= ctx->mode;

	/*
	 * Allocate and initialize subpool if maximum or minimum size is
	 * specified.  Any needed reservations (for minimum size) are taken
	 * taken when the subpool is created.
	 */
	if (ctx->max_hpages != -1 || ctx->min_hpages != -1) {
		sbinfo->spool = hugepage_new_subpool(ctx->hstate,
						     ctx->max_hpages,
						     ctx->min_hpages);
>>>>>>> upstream/android-13
		if (!sbinfo->spool)
			goto out_free;
	}
	sb->s_maxbytes = MAX_LFS_FILESIZE;
<<<<<<< HEAD
	sb->s_blocksize = huge_page_size(config.hstate);
	sb->s_blocksize_bits = huge_page_shift(config.hstate);
	sb->s_magic = HUGETLBFS_MAGIC;
	sb->s_op = &hugetlbfs_ops;
	sb->s_time_gran = 1;
	sb->s_root = d_make_root(hugetlbfs_get_root(sb, &config));
=======
	sb->s_blocksize = huge_page_size(ctx->hstate);
	sb->s_blocksize_bits = huge_page_shift(ctx->hstate);
	sb->s_magic = HUGETLBFS_MAGIC;
	sb->s_op = &hugetlbfs_ops;
	sb->s_time_gran = 1;

	/*
	 * Due to the special and limited functionality of hugetlbfs, it does
	 * not work well as a stacking filesystem.
	 */
	sb->s_stack_depth = FILESYSTEM_MAX_STACK_DEPTH;
	sb->s_root = d_make_root(hugetlbfs_get_root(sb, ctx));
>>>>>>> upstream/android-13
	if (!sb->s_root)
		goto out_free;
	return 0;
out_free:
	kfree(sbinfo->spool);
	kfree(sbinfo);
	return -ENOMEM;
}

<<<<<<< HEAD
static struct dentry *hugetlbfs_mount(struct file_system_type *fs_type,
	int flags, const char *dev_name, void *data)
{
	return mount_nodev(fs_type, flags, data, hugetlbfs_fill_super);
}

static struct file_system_type hugetlbfs_fs_type = {
	.name		= "hugetlbfs",
	.mount		= hugetlbfs_mount,
	.kill_sb	= kill_litter_super,
=======
static int hugetlbfs_get_tree(struct fs_context *fc)
{
	int err = hugetlbfs_validate(fc);
	if (err)
		return err;
	return get_tree_nodev(fc, hugetlbfs_fill_super);
}

static void hugetlbfs_fs_context_free(struct fs_context *fc)
{
	kfree(fc->fs_private);
}

static const struct fs_context_operations hugetlbfs_fs_context_ops = {
	.free		= hugetlbfs_fs_context_free,
	.parse_param	= hugetlbfs_parse_param,
	.get_tree	= hugetlbfs_get_tree,
};

static int hugetlbfs_init_fs_context(struct fs_context *fc)
{
	struct hugetlbfs_fs_context *ctx;

	ctx = kzalloc(sizeof(struct hugetlbfs_fs_context), GFP_KERNEL);
	if (!ctx)
		return -ENOMEM;

	ctx->max_hpages	= -1; /* No limit on size by default */
	ctx->nr_inodes	= -1; /* No limit on number of inodes by default */
	ctx->uid	= current_fsuid();
	ctx->gid	= current_fsgid();
	ctx->mode	= 0755;
	ctx->hstate	= &default_hstate;
	ctx->min_hpages	= -1; /* No default minimum size */
	ctx->max_val_type = NO_SIZE;
	ctx->min_val_type = NO_SIZE;
	fc->fs_private = ctx;
	fc->ops	= &hugetlbfs_fs_context_ops;
	return 0;
}

static struct file_system_type hugetlbfs_fs_type = {
	.name			= "hugetlbfs",
	.init_fs_context	= hugetlbfs_init_fs_context,
	.parameters		= hugetlb_fs_parameters,
	.kill_sb		= kill_litter_super,
>>>>>>> upstream/android-13
};

static struct vfsmount *hugetlbfs_vfsmount[HUGE_MAX_HSTATE];

static int can_do_hugetlb_shm(void)
{
	kgid_t shm_group;
	shm_group = make_kgid(&init_user_ns, sysctl_hugetlb_shm_group);
	return capable(CAP_IPC_LOCK) || in_group_p(shm_group);
}

static int get_hstate_idx(int page_size_log)
{
	struct hstate *h = hstate_sizelog(page_size_log);

	if (!h)
		return -1;
<<<<<<< HEAD
	return h - hstates;
=======
	return hstate_index(h);
>>>>>>> upstream/android-13
}

/*
 * Note that size should be aligned to proper hugepage size in caller side,
 * otherwise hugetlb_reserve_pages reserves one less hugepages than intended.
 */
struct file *hugetlb_file_setup(const char *name, size_t size,
<<<<<<< HEAD
				vm_flags_t acctflag, struct user_struct **user,
=======
				vm_flags_t acctflag, struct ucounts **ucounts,
>>>>>>> upstream/android-13
				int creat_flags, int page_size_log)
{
	struct inode *inode;
	struct vfsmount *mnt;
	int hstate_idx;
	struct file *file;

	hstate_idx = get_hstate_idx(page_size_log);
	if (hstate_idx < 0)
		return ERR_PTR(-ENODEV);

<<<<<<< HEAD
	*user = NULL;
=======
	*ucounts = NULL;
>>>>>>> upstream/android-13
	mnt = hugetlbfs_vfsmount[hstate_idx];
	if (!mnt)
		return ERR_PTR(-ENOENT);

	if (creat_flags == HUGETLB_SHMFS_INODE && !can_do_hugetlb_shm()) {
<<<<<<< HEAD
		*user = current_user();
		if (user_shm_lock(size, *user)) {
=======
		*ucounts = current_ucounts();
		if (user_shm_lock(size, *ucounts)) {
>>>>>>> upstream/android-13
			task_lock(current);
			pr_warn_once("%s (%d): Using mlock ulimits for SHM_HUGETLB is deprecated\n",
				current->comm, current->pid);
			task_unlock(current);
		} else {
<<<<<<< HEAD
			*user = NULL;
=======
			*ucounts = NULL;
>>>>>>> upstream/android-13
			return ERR_PTR(-EPERM);
		}
	}

	file = ERR_PTR(-ENOSPC);
	inode = hugetlbfs_get_inode(mnt->mnt_sb, NULL, S_IFREG | S_IRWXUGO, 0);
	if (!inode)
		goto out;
	if (creat_flags == HUGETLB_SHMFS_INODE)
		inode->i_flags |= S_PRIVATE;

	inode->i_size = size;
	clear_nlink(inode);

<<<<<<< HEAD
	if (hugetlb_reserve_pages(inode, 0,
=======
	if (!hugetlb_reserve_pages(inode, 0,
>>>>>>> upstream/android-13
			size >> huge_page_shift(hstate_inode(inode)), NULL,
			acctflag))
		file = ERR_PTR(-ENOMEM);
	else
		file = alloc_file_pseudo(inode, mnt, name, O_RDWR,
					&hugetlbfs_file_operations);
	if (!IS_ERR(file))
		return file;

	iput(inode);
out:
<<<<<<< HEAD
	if (*user) {
		user_shm_unlock(size, *user);
		*user = NULL;
=======
	if (*ucounts) {
		user_shm_unlock(size, *ucounts);
		*ucounts = NULL;
>>>>>>> upstream/android-13
	}
	return file;
}

<<<<<<< HEAD
static int __init init_hugetlbfs_fs(void)
{
=======
static struct vfsmount *__init mount_one_hugetlbfs(struct hstate *h)
{
	struct fs_context *fc;
	struct vfsmount *mnt;

	fc = fs_context_for_mount(&hugetlbfs_fs_type, SB_KERNMOUNT);
	if (IS_ERR(fc)) {
		mnt = ERR_CAST(fc);
	} else {
		struct hugetlbfs_fs_context *ctx = fc->fs_private;
		ctx->hstate = h;
		mnt = fc_mount(fc);
		put_fs_context(fc);
	}
	if (IS_ERR(mnt))
		pr_err("Cannot mount internal hugetlbfs for page size %luK",
		       huge_page_size(h) >> 10);
	return mnt;
}

static int __init init_hugetlbfs_fs(void)
{
	struct vfsmount *mnt;
>>>>>>> upstream/android-13
	struct hstate *h;
	int error;
	int i;

	if (!hugepages_supported()) {
		pr_info("disabling because there are no supported hugepage sizes\n");
		return -ENOTSUPP;
	}

	error = -ENOMEM;
	hugetlbfs_inode_cachep = kmem_cache_create("hugetlbfs_inode_cache",
					sizeof(struct hugetlbfs_inode_info),
					0, SLAB_ACCOUNT, init_once);
	if (hugetlbfs_inode_cachep == NULL)
<<<<<<< HEAD
		goto out2;

	error = register_filesystem(&hugetlbfs_fs_type);
	if (error)
		goto out;

	i = 0;
	for_each_hstate(h) {
		char buf[50];
		unsigned ps_kb = 1U << (h->order + PAGE_SHIFT - 10);

		snprintf(buf, sizeof(buf), "pagesize=%uK", ps_kb);
		hugetlbfs_vfsmount[i] = kern_mount_data(&hugetlbfs_fs_type,
							buf);

		if (IS_ERR(hugetlbfs_vfsmount[i])) {
			pr_err("Cannot mount internal hugetlbfs for "
				"page size %uK", ps_kb);
			error = PTR_ERR(hugetlbfs_vfsmount[i]);
			hugetlbfs_vfsmount[i] = NULL;
		}
		i++;
	}
	/* Non default hstates are optional */
	if (!IS_ERR_OR_NULL(hugetlbfs_vfsmount[default_hstate_idx]))
		return 0;

 out:
	kmem_cache_destroy(hugetlbfs_inode_cachep);
 out2:
=======
		goto out;

	error = register_filesystem(&hugetlbfs_fs_type);
	if (error)
		goto out_free;

	/* default hstate mount is required */
	mnt = mount_one_hugetlbfs(&default_hstate);
	if (IS_ERR(mnt)) {
		error = PTR_ERR(mnt);
		goto out_unreg;
	}
	hugetlbfs_vfsmount[default_hstate_idx] = mnt;

	/* other hstates are optional */
	i = 0;
	for_each_hstate(h) {
		if (i == default_hstate_idx) {
			i++;
			continue;
		}

		mnt = mount_one_hugetlbfs(h);
		if (IS_ERR(mnt))
			hugetlbfs_vfsmount[i] = NULL;
		else
			hugetlbfs_vfsmount[i] = mnt;
		i++;
	}

	return 0;

 out_unreg:
	(void)unregister_filesystem(&hugetlbfs_fs_type);
 out_free:
	kmem_cache_destroy(hugetlbfs_inode_cachep);
 out:
>>>>>>> upstream/android-13
	return error;
}
fs_initcall(init_hugetlbfs_fs)
