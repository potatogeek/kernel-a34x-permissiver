<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> upstream/android-13
/*
 * Persistent Memory Driver
 *
 * Copyright (c) 2014-2015, Intel Corporation.
 * Copyright (c) 2015, Christoph Hellwig <hch@lst.de>.
 * Copyright (c) 2015, Boaz Harrosh <boaz@plexistor.com>.
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 */

#include <asm/cacheflush.h>
#include <linux/blkdev.h>
=======
 */

#include <linux/blkdev.h>
#include <linux/pagemap.h>
>>>>>>> upstream/android-13
#include <linux/hdreg.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/set_memory.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/badblocks.h>
#include <linux/memremap.h>
#include <linux/vmalloc.h>
#include <linux/blk-mq.h>
#include <linux/pfn_t.h>
#include <linux/slab.h>
#include <linux/uio.h>
#include <linux/dax.h>
#include <linux/nd.h>
<<<<<<< HEAD
#include <linux/backing-dev.h>
#include "pmem.h"
#include "pfn.h"
#include "nd.h"
#include "nd-core.h"
=======
#include <linux/mm.h>
#include <asm/cacheflush.h>
#include "pmem.h"
#include "btt.h"
#include "pfn.h"
#include "nd.h"
>>>>>>> upstream/android-13

static struct device *to_dev(struct pmem_device *pmem)
{
	/*
	 * nvdimm bus services need a 'dev' parameter, and we record the device
	 * at init in bb.dev.
	 */
	return pmem->bb.dev;
}

static struct nd_region *to_region(struct pmem_device *pmem)
{
	return to_nd_region(to_dev(pmem)->parent);
}

static void hwpoison_clear(struct pmem_device *pmem,
		phys_addr_t phys, unsigned int len)
{
	unsigned long pfn_start, pfn_end, pfn;

	/* only pmem in the linear map supports HWPoison */
	if (is_vmalloc_addr(pmem->virt_addr))
		return;

	pfn_start = PHYS_PFN(phys);
	pfn_end = pfn_start + PHYS_PFN(len);
	for (pfn = pfn_start; pfn < pfn_end; pfn++) {
		struct page *page = pfn_to_page(pfn);

		/*
		 * Note, no need to hold a get_dev_pagemap() reference
		 * here since we're in the driver I/O path and
		 * outstanding I/O requests pin the dev_pagemap.
		 */
		if (test_and_clear_pmem_poison(page))
			clear_mce_nospec(pfn);
	}
}

static blk_status_t pmem_clear_poison(struct pmem_device *pmem,
		phys_addr_t offset, unsigned int len)
{
	struct device *dev = to_dev(pmem);
	sector_t sector;
	long cleared;
	blk_status_t rc = BLK_STS_OK;

	sector = (offset - pmem->data_offset) / 512;

	cleared = nvdimm_clear_poison(dev, pmem->phys_addr + offset, len);
	if (cleared < len)
		rc = BLK_STS_IOERR;
	if (cleared > 0 && cleared / 512) {
		hwpoison_clear(pmem, pmem->phys_addr + offset, cleared);
		cleared /= 512;
		dev_dbg(dev, "%#llx clear %ld sector%s\n",
				(unsigned long long) sector, cleared,
				cleared > 1 ? "s" : "");
		badblocks_clear(&pmem->bb, sector, cleared);
		if (pmem->bb_state)
			sysfs_notify_dirent(pmem->bb_state);
	}

	arch_invalidate_pmem(pmem->virt_addr + offset, len);

	return rc;
}

static void write_pmem(void *pmem_addr, struct page *page,
		unsigned int off, unsigned int len)
{
	unsigned int chunk;
	void *mem;

	while (len) {
		mem = kmap_atomic(page);
		chunk = min_t(unsigned int, len, PAGE_SIZE - off);
		memcpy_flushcache(pmem_addr, mem + off, chunk);
		kunmap_atomic(mem);
		len -= chunk;
		off = 0;
		page++;
		pmem_addr += chunk;
	}
}

static blk_status_t read_pmem(struct page *page, unsigned int off,
		void *pmem_addr, unsigned int len)
{
	unsigned int chunk;
	unsigned long rem;
	void *mem;

	while (len) {
		mem = kmap_atomic(page);
		chunk = min_t(unsigned int, len, PAGE_SIZE - off);
<<<<<<< HEAD
		rem = memcpy_mcsafe(mem + off, pmem_addr, chunk);
=======
		rem = copy_mc_to_kernel(mem + off, pmem_addr, chunk);
>>>>>>> upstream/android-13
		kunmap_atomic(mem);
		if (rem)
			return BLK_STS_IOERR;
		len -= chunk;
		off = 0;
		page++;
		pmem_addr += chunk;
	}
	return BLK_STS_OK;
}

<<<<<<< HEAD
static blk_status_t pmem_do_bvec(struct pmem_device *pmem, struct page *page,
			unsigned int len, unsigned int off, unsigned int op,
			sector_t sector)
=======
static blk_status_t pmem_do_read(struct pmem_device *pmem,
			struct page *page, unsigned int page_off,
			sector_t sector, unsigned int len)
{
	blk_status_t rc;
	phys_addr_t pmem_off = sector * 512 + pmem->data_offset;
	void *pmem_addr = pmem->virt_addr + pmem_off;

	if (unlikely(is_bad_pmem(&pmem->bb, sector, len)))
		return BLK_STS_IOERR;

	rc = read_pmem(page, page_off, pmem_addr, len);
	flush_dcache_page(page);
	return rc;
}

static blk_status_t pmem_do_write(struct pmem_device *pmem,
			struct page *page, unsigned int page_off,
			sector_t sector, unsigned int len)
>>>>>>> upstream/android-13
{
	blk_status_t rc = BLK_STS_OK;
	bool bad_pmem = false;
	phys_addr_t pmem_off = sector * 512 + pmem->data_offset;
	void *pmem_addr = pmem->virt_addr + pmem_off;

	if (unlikely(is_bad_pmem(&pmem->bb, sector, len)))
		bad_pmem = true;

<<<<<<< HEAD
	if (!op_is_write(op)) {
		if (unlikely(bad_pmem))
			rc = BLK_STS_IOERR;
		else {
			rc = read_pmem(page, off, pmem_addr, len);
			flush_dcache_page(page);
		}
	} else {
		/*
		 * Note that we write the data both before and after
		 * clearing poison.  The write before clear poison
		 * handles situations where the latest written data is
		 * preserved and the clear poison operation simply marks
		 * the address range as valid without changing the data.
		 * In this case application software can assume that an
		 * interrupted write will either return the new good
		 * data or an error.
		 *
		 * However, if pmem_clear_poison() leaves the data in an
		 * indeterminate state we need to perform the write
		 * after clear poison.
		 */
		flush_dcache_page(page);
		write_pmem(pmem_addr, page, off, len);
		if (unlikely(bad_pmem)) {
			rc = pmem_clear_poison(pmem, pmem_off, len);
			write_pmem(pmem_addr, page, off, len);
		}
=======
	/*
	 * Note that we write the data both before and after
	 * clearing poison.  The write before clear poison
	 * handles situations where the latest written data is
	 * preserved and the clear poison operation simply marks
	 * the address range as valid without changing the data.
	 * In this case application software can assume that an
	 * interrupted write will either return the new good
	 * data or an error.
	 *
	 * However, if pmem_clear_poison() leaves the data in an
	 * indeterminate state we need to perform the write
	 * after clear poison.
	 */
	flush_dcache_page(page);
	write_pmem(pmem_addr, page, page_off, len);
	if (unlikely(bad_pmem)) {
		rc = pmem_clear_poison(pmem, pmem_off, len);
		write_pmem(pmem_addr, page, page_off, len);
>>>>>>> upstream/android-13
	}

	return rc;
}

<<<<<<< HEAD
static blk_qc_t pmem_make_request(struct request_queue *q, struct bio *bio)
=======
static blk_qc_t pmem_submit_bio(struct bio *bio)
>>>>>>> upstream/android-13
{
	int ret = 0;
	blk_status_t rc = 0;
	bool do_acct;
	unsigned long start;
	struct bio_vec bvec;
	struct bvec_iter iter;
<<<<<<< HEAD
	struct pmem_device *pmem = q->queuedata;
=======
	struct pmem_device *pmem = bio->bi_bdev->bd_disk->private_data;
>>>>>>> upstream/android-13
	struct nd_region *nd_region = to_region(pmem);

	if (bio->bi_opf & REQ_PREFLUSH)
		ret = nvdimm_flush(nd_region, bio);

<<<<<<< HEAD
	do_acct = nd_iostat_start(bio, &start);
	bio_for_each_segment(bvec, bio, iter) {
		rc = pmem_do_bvec(pmem, bvec.bv_page, bvec.bv_len,
				bvec.bv_offset, bio_op(bio), iter.bi_sector);
=======
	do_acct = blk_queue_io_stat(bio->bi_bdev->bd_disk->queue);
	if (do_acct)
		start = bio_start_io_acct(bio);
	bio_for_each_segment(bvec, bio, iter) {
		if (op_is_write(bio_op(bio)))
			rc = pmem_do_write(pmem, bvec.bv_page, bvec.bv_offset,
				iter.bi_sector, bvec.bv_len);
		else
			rc = pmem_do_read(pmem, bvec.bv_page, bvec.bv_offset,
				iter.bi_sector, bvec.bv_len);
>>>>>>> upstream/android-13
		if (rc) {
			bio->bi_status = rc;
			break;
		}
	}
	if (do_acct)
<<<<<<< HEAD
		nd_iostat_end(bio, start);
=======
		bio_end_io_acct(bio, start);
>>>>>>> upstream/android-13

	if (bio->bi_opf & REQ_FUA)
		ret = nvdimm_flush(nd_region, bio);

	if (ret)
		bio->bi_status = errno_to_blk_status(ret);

	bio_endio(bio);
	return BLK_QC_T_NONE;
}

static int pmem_rw_page(struct block_device *bdev, sector_t sector,
		       struct page *page, unsigned int op)
{
<<<<<<< HEAD
	struct pmem_device *pmem = bdev->bd_queue->queuedata;
	blk_status_t rc;

	rc = pmem_do_bvec(pmem, page, hpage_nr_pages(page) * PAGE_SIZE,
			  0, op, sector);

=======
	struct pmem_device *pmem = bdev->bd_disk->private_data;
	blk_status_t rc;

	if (op_is_write(op))
		rc = pmem_do_write(pmem, page, 0, sector, thp_size(page));
	else
		rc = pmem_do_read(pmem, page, 0, sector, thp_size(page));
>>>>>>> upstream/android-13
	/*
	 * The ->rw_page interface is subtle and tricky.  The core
	 * retries on any error, so we can only invoke page_endio() in
	 * the successful completion case.  Otherwise, we'll see crashes
	 * caused by double completion.
	 */
	if (rc == 0)
		page_endio(page, op_is_write(op), 0);

	return blk_status_to_errno(rc);
}

/* see "strong" declaration in tools/testing/nvdimm/pmem-dax.c */
__weak long __pmem_direct_access(struct pmem_device *pmem, pgoff_t pgoff,
		long nr_pages, void **kaddr, pfn_t *pfn)
{
	resource_size_t offset = PFN_PHYS(pgoff) + pmem->data_offset;

	if (unlikely(is_bad_pmem(&pmem->bb, PFN_PHYS(pgoff) / 512,
					PFN_PHYS(nr_pages))))
		return -EIO;

	if (kaddr)
		*kaddr = pmem->virt_addr + offset;
	if (pfn)
		*pfn = phys_to_pfn_t(pmem->phys_addr + offset, pmem->pfn_flags);

	/*
	 * If badblocks are present, limit known good range to the
	 * requested range.
	 */
	if (unlikely(pmem->bb.count))
		return nr_pages;
	return PHYS_PFN(pmem->size - pmem->pfn_pad - offset);
}

static const struct block_device_operations pmem_fops = {
	.owner =		THIS_MODULE,
<<<<<<< HEAD
	.rw_page =		pmem_rw_page,
	.revalidate_disk =	nvdimm_revalidate_disk,
};

=======
	.submit_bio =		pmem_submit_bio,
	.rw_page =		pmem_rw_page,
};

static int pmem_dax_zero_page_range(struct dax_device *dax_dev, pgoff_t pgoff,
				    size_t nr_pages)
{
	struct pmem_device *pmem = dax_get_private(dax_dev);

	return blk_status_to_errno(pmem_do_write(pmem, ZERO_PAGE(0), 0,
				   PFN_PHYS(pgoff) >> SECTOR_SHIFT,
				   PAGE_SIZE));
}

>>>>>>> upstream/android-13
static long pmem_dax_direct_access(struct dax_device *dax_dev,
		pgoff_t pgoff, long nr_pages, void **kaddr, pfn_t *pfn)
{
	struct pmem_device *pmem = dax_get_private(dax_dev);

	return __pmem_direct_access(pmem, pgoff, nr_pages, kaddr, pfn);
}

/*
 * Use the 'no check' versions of copy_from_iter_flushcache() and
<<<<<<< HEAD
 * copy_to_iter_mcsafe() to bypass HARDENED_USERCOPY overhead. Bounds
=======
 * copy_mc_to_iter() to bypass HARDENED_USERCOPY overhead. Bounds
>>>>>>> upstream/android-13
 * checking, both file offset and device offset, is handled by
 * dax_iomap_actor()
 */
static size_t pmem_copy_from_iter(struct dax_device *dax_dev, pgoff_t pgoff,
		void *addr, size_t bytes, struct iov_iter *i)
{
	return _copy_from_iter_flushcache(addr, bytes, i);
}

static size_t pmem_copy_to_iter(struct dax_device *dax_dev, pgoff_t pgoff,
		void *addr, size_t bytes, struct iov_iter *i)
{
<<<<<<< HEAD
	return _copy_to_iter_mcsafe(addr, bytes, i);
=======
	return _copy_mc_to_iter(addr, bytes, i);
>>>>>>> upstream/android-13
}

static const struct dax_operations pmem_dax_ops = {
	.direct_access = pmem_dax_direct_access,
<<<<<<< HEAD
	.copy_from_iter = pmem_copy_from_iter,
	.copy_to_iter = pmem_copy_to_iter,
=======
	.dax_supported = generic_fsdax_supported,
	.copy_from_iter = pmem_copy_from_iter,
	.copy_to_iter = pmem_copy_to_iter,
	.zero_page_range = pmem_dax_zero_page_range,
>>>>>>> upstream/android-13
};

static const struct attribute_group *pmem_attribute_groups[] = {
	&dax_attribute_group,
	NULL,
};

<<<<<<< HEAD
static void pmem_release_queue(void *q)
{
	blk_cleanup_queue(q);
}

static void pmem_freeze_queue(struct percpu_ref *ref)
{
	struct request_queue *q;

	q = container_of(ref, typeof(*q), q_usage_counter);
	blk_freeze_queue_start(q);
}

=======
>>>>>>> upstream/android-13
static void pmem_release_disk(void *__pmem)
{
	struct pmem_device *pmem = __pmem;

	kill_dax(pmem->dax_dev);
	put_dax(pmem->dax_dev);
	del_gendisk(pmem->disk);
<<<<<<< HEAD
	put_disk(pmem->disk);
}

static void pmem_release_pgmap_ops(void *__pgmap)
{
	dev_pagemap_put_ops();
}

static void fsdax_pagefree(struct page *page, void *data)
{
	wake_up_var(&page->_refcount);
}

static int setup_pagemap_fsdax(struct device *dev, struct dev_pagemap *pgmap)
{
	dev_pagemap_get_ops();
	if (devm_add_action_or_reset(dev, pmem_release_pgmap_ops, pgmap))
		return -ENOMEM;
	pgmap->type = MEMORY_DEVICE_FS_DAX;
	pgmap->page_free = fsdax_pagefree;

	return 0;
=======

	blk_cleanup_disk(pmem->disk);
>>>>>>> upstream/android-13
}

static int pmem_attach_disk(struct device *dev,
		struct nd_namespace_common *ndns)
{
	struct nd_namespace_io *nsio = to_nd_namespace_io(&ndns->dev);
	struct nd_region *nd_region = to_nd_region(dev->parent);
	int nid = dev_to_node(dev), fua;
	struct resource *res = &nsio->res;
<<<<<<< HEAD
	struct resource bb_res;
=======
	struct range bb_range;
>>>>>>> upstream/android-13
	struct nd_pfn *nd_pfn = NULL;
	struct dax_device *dax_dev;
	struct nd_pfn_sb *pfn_sb;
	struct pmem_device *pmem;
	struct request_queue *q;
<<<<<<< HEAD
	struct device *gendev;
	struct gendisk *disk;
	void *addr;
	int rc;
=======
	struct gendisk *disk;
	void *addr;
	int rc;
	unsigned long flags = 0UL;
>>>>>>> upstream/android-13

	pmem = devm_kzalloc(dev, sizeof(*pmem), GFP_KERNEL);
	if (!pmem)
		return -ENOMEM;

<<<<<<< HEAD
=======
	rc = devm_namespace_enable(dev, ndns, nd_info_block_reserve());
	if (rc)
		return rc;

>>>>>>> upstream/android-13
	/* while nsio_rw_bytes is active, parse a pfn info block if present */
	if (is_nd_pfn(dev)) {
		nd_pfn = to_nd_pfn(dev);
		rc = nvdimm_setup_pfn(nd_pfn, &pmem->pgmap);
		if (rc)
			return rc;
	}

	/* we're attaching a block device, disable raw namespace access */
<<<<<<< HEAD
	devm_nsio_disable(dev, nsio);
=======
	devm_namespace_disable(dev, ndns);
>>>>>>> upstream/android-13

	dev_set_drvdata(dev, pmem);
	pmem->phys_addr = res->start;
	pmem->size = resource_size(res);
	fua = nvdimm_has_flush(nd_region);
	if (!IS_ENABLED(CONFIG_ARCH_HAS_UACCESS_FLUSHCACHE) || fua < 0) {
		dev_warn(dev, "unable to guarantee persistence of writes\n");
		fua = 0;
	}

	if (!devm_request_mem_region(dev, res->start, resource_size(res),
				dev_name(&ndns->dev))) {
		dev_warn(dev, "could not reserve region %pR\n", res);
		return -EBUSY;
	}

<<<<<<< HEAD
	q = blk_alloc_queue_node(GFP_KERNEL, dev_to_node(dev), NULL);
	if (!q)
		return -ENOMEM;

	if (devm_add_action_or_reset(dev, pmem_release_queue, q))
		return -ENOMEM;

	pmem->pfn_flags = PFN_DEV;
	pmem->pgmap.ref = &q->q_usage_counter;
	pmem->pgmap.kill = pmem_freeze_queue;
	if (is_nd_pfn(dev)) {
		if (setup_pagemap_fsdax(dev, &pmem->pgmap))
			return -ENOMEM;
=======
	disk = blk_alloc_disk(nid);
	if (!disk)
		return -ENOMEM;
	q = disk->queue;

	pmem->disk = disk;
	pmem->pgmap.owner = pmem;
	pmem->pfn_flags = PFN_DEV;
	if (is_nd_pfn(dev)) {
		pmem->pgmap.type = MEMORY_DEVICE_FS_DAX;
>>>>>>> upstream/android-13
		addr = devm_memremap_pages(dev, &pmem->pgmap);
		pfn_sb = nd_pfn->pfn_sb;
		pmem->data_offset = le64_to_cpu(pfn_sb->dataoff);
		pmem->pfn_pad = resource_size(res) -
<<<<<<< HEAD
			resource_size(&pmem->pgmap.res);
		pmem->pfn_flags |= PFN_MAP;
		memcpy(&bb_res, &pmem->pgmap.res, sizeof(bb_res));
		bb_res.start += pmem->data_offset;
	} else if (pmem_should_map_pages(dev)) {
		memcpy(&pmem->pgmap.res, &nsio->res, sizeof(pmem->pgmap.res));
		pmem->pgmap.altmap_valid = false;
		if (setup_pagemap_fsdax(dev, &pmem->pgmap))
			return -ENOMEM;
		addr = devm_memremap_pages(dev, &pmem->pgmap);
		pmem->pfn_flags |= PFN_MAP;
		memcpy(&bb_res, &pmem->pgmap.res, sizeof(bb_res));
	} else {
		addr = devm_memremap(dev, pmem->phys_addr,
				pmem->size, ARCH_MEMREMAP_PMEM);
		memcpy(&bb_res, &nsio->res, sizeof(bb_res));
	}

	if (IS_ERR(addr))
		return PTR_ERR(addr);
	pmem->virt_addr = addr;

	blk_queue_write_cache(q, true, fua);
	blk_queue_make_request(q, pmem_make_request);
=======
			range_len(&pmem->pgmap.range);
		pmem->pfn_flags |= PFN_MAP;
		bb_range = pmem->pgmap.range;
		bb_range.start += pmem->data_offset;
	} else if (pmem_should_map_pages(dev)) {
		pmem->pgmap.range.start = res->start;
		pmem->pgmap.range.end = res->end;
		pmem->pgmap.nr_range = 1;
		pmem->pgmap.type = MEMORY_DEVICE_FS_DAX;
		addr = devm_memremap_pages(dev, &pmem->pgmap);
		pmem->pfn_flags |= PFN_MAP;
		bb_range = pmem->pgmap.range;
	} else {
		addr = devm_memremap(dev, pmem->phys_addr,
				pmem->size, ARCH_MEMREMAP_PMEM);
		bb_range.start =  res->start;
		bb_range.end = res->end;
	}

	if (IS_ERR(addr)) {
		rc = PTR_ERR(addr);
		goto out;
	}
	pmem->virt_addr = addr;

	blk_queue_write_cache(q, true, fua);
>>>>>>> upstream/android-13
	blk_queue_physical_block_size(q, PAGE_SIZE);
	blk_queue_logical_block_size(q, pmem_sector_size(ndns));
	blk_queue_max_hw_sectors(q, UINT_MAX);
	blk_queue_flag_set(QUEUE_FLAG_NONROT, q);
	if (pmem->pfn_flags & PFN_MAP)
		blk_queue_flag_set(QUEUE_FLAG_DAX, q);
<<<<<<< HEAD
	q->queuedata = pmem;

	disk = alloc_disk_node(0, nid);
	if (!disk)
		return -ENOMEM;
	pmem->disk = disk;

	disk->fops		= &pmem_fops;
	disk->queue		= q;
	disk->flags		= GENHD_FL_EXT_DEVT;
	disk->queue->backing_dev_info->capabilities |= BDI_CAP_SYNCHRONOUS_IO;
=======

	disk->fops		= &pmem_fops;
	disk->private_data	= pmem;
>>>>>>> upstream/android-13
	nvdimm_namespace_disk_name(ndns, disk->disk_name);
	set_capacity(disk, (pmem->size - pmem->pfn_pad - pmem->data_offset)
			/ 512);
	if (devm_init_badblocks(dev, &pmem->bb))
		return -ENOMEM;
<<<<<<< HEAD
	nvdimm_badblocks_populate(nd_region, &pmem->bb, &bb_res);
	disk->bb = &pmem->bb;

	dax_dev = alloc_dax(pmem, disk->disk_name, &pmem_dax_ops);
	if (!dax_dev) {
		put_disk(disk);
		return -ENOMEM;
	}
	dax_write_cache(dax_dev, nvdimm_has_cache(nd_region));
	pmem->dax_dev = dax_dev;
	gendev = disk_to_dev(disk);
	gendev->groups = pmem_attribute_groups;

	device_add_disk(dev, disk);
	if (devm_add_action_or_reset(dev, pmem_release_disk, pmem))
		return -ENOMEM;

	revalidate_disk(disk);
=======
	nvdimm_badblocks_populate(nd_region, &pmem->bb, &bb_range);
	disk->bb = &pmem->bb;

	if (is_nvdimm_sync(nd_region))
		flags = DAXDEV_F_SYNC;
	dax_dev = alloc_dax(pmem, disk->disk_name, &pmem_dax_ops, flags);
	if (IS_ERR(dax_dev)) {
		rc = PTR_ERR(dax_dev);
		goto out;
	}
	dax_write_cache(dax_dev, nvdimm_has_cache(nd_region));
	pmem->dax_dev = dax_dev;

	device_add_disk(dev, disk, pmem_attribute_groups);
	if (devm_add_action_or_reset(dev, pmem_release_disk, pmem))
		return -ENOMEM;

	nvdimm_check_and_set_ro(disk);
>>>>>>> upstream/android-13

	pmem->bb_state = sysfs_get_dirent(disk_to_dev(disk)->kobj.sd,
					  "badblocks");
	if (!pmem->bb_state)
		dev_warn(dev, "'badblocks' notification disabled\n");
<<<<<<< HEAD

	return 0;
=======
	return 0;
out:
	blk_cleanup_disk(pmem->disk);
	return rc;
>>>>>>> upstream/android-13
}

static int nd_pmem_probe(struct device *dev)
{
<<<<<<< HEAD
=======
	int ret;
>>>>>>> upstream/android-13
	struct nd_namespace_common *ndns;

	ndns = nvdimm_namespace_common_probe(dev);
	if (IS_ERR(ndns))
		return PTR_ERR(ndns);

<<<<<<< HEAD
	if (devm_nsio_enable(dev, to_nd_namespace_io(&ndns->dev)))
		return -ENXIO;

=======
>>>>>>> upstream/android-13
	if (is_nd_btt(dev))
		return nvdimm_namespace_attach_btt(ndns);

	if (is_nd_pfn(dev))
		return pmem_attach_disk(dev, ndns);

<<<<<<< HEAD
	/* if we find a valid info-block we'll come back as that personality */
	if (nd_btt_probe(dev, ndns) == 0 || nd_pfn_probe(dev, ndns) == 0
			|| nd_dax_probe(dev, ndns) == 0)
		return -ENXIO;

	/* ...otherwise we're just a raw pmem device */
	return pmem_attach_disk(dev, ndns);
}

static int nd_pmem_remove(struct device *dev)
=======
	ret = devm_namespace_enable(dev, ndns, nd_info_block_reserve());
	if (ret)
		return ret;

	ret = nd_btt_probe(dev, ndns);
	if (ret == 0)
		return -ENXIO;

	/*
	 * We have two failure conditions here, there is no
	 * info reserver block or we found a valid info reserve block
	 * but failed to initialize the pfn superblock.
	 *
	 * For the first case consider namespace as a raw pmem namespace
	 * and attach a disk.
	 *
	 * For the latter, consider this a success and advance the namespace
	 * seed.
	 */
	ret = nd_pfn_probe(dev, ndns);
	if (ret == 0)
		return -ENXIO;
	else if (ret == -EOPNOTSUPP)
		return ret;

	ret = nd_dax_probe(dev, ndns);
	if (ret == 0)
		return -ENXIO;
	else if (ret == -EOPNOTSUPP)
		return ret;

	/* probe complete, attach handles namespace enabling */
	devm_namespace_disable(dev, ndns);

	return pmem_attach_disk(dev, ndns);
}

static void nd_pmem_remove(struct device *dev)
>>>>>>> upstream/android-13
{
	struct pmem_device *pmem = dev_get_drvdata(dev);

	if (is_nd_btt(dev))
		nvdimm_namespace_detach_btt(to_nd_btt(dev));
	else {
		/*
<<<<<<< HEAD
		 * Note, this assumes device_lock() context to not race
		 * nd_pmem_notify()
=======
		 * Note, this assumes nd_device_lock() context to not
		 * race nd_pmem_notify()
>>>>>>> upstream/android-13
		 */
		sysfs_put(pmem->bb_state);
		pmem->bb_state = NULL;
	}
	nvdimm_flush(to_nd_region(dev->parent), NULL);
<<<<<<< HEAD

	return 0;
=======
>>>>>>> upstream/android-13
}

static void nd_pmem_shutdown(struct device *dev)
{
	nvdimm_flush(to_nd_region(dev->parent), NULL);
}

<<<<<<< HEAD
static void nd_pmem_notify(struct device *dev, enum nvdimm_event event)
=======
static void pmem_revalidate_poison(struct device *dev)
>>>>>>> upstream/android-13
{
	struct nd_region *nd_region;
	resource_size_t offset = 0, end_trunc = 0;
	struct nd_namespace_common *ndns;
	struct nd_namespace_io *nsio;
<<<<<<< HEAD
	struct resource res;
	struct badblocks *bb;
	struct kernfs_node *bb_state;

	if (event != NVDIMM_REVALIDATE_POISON)
		return;

=======
	struct badblocks *bb;
	struct range range;
	struct kernfs_node *bb_state;

>>>>>>> upstream/android-13
	if (is_nd_btt(dev)) {
		struct nd_btt *nd_btt = to_nd_btt(dev);

		ndns = nd_btt->ndns;
		nd_region = to_nd_region(ndns->dev.parent);
		nsio = to_nd_namespace_io(&ndns->dev);
		bb = &nsio->bb;
		bb_state = NULL;
	} else {
		struct pmem_device *pmem = dev_get_drvdata(dev);

		nd_region = to_region(pmem);
		bb = &pmem->bb;
		bb_state = pmem->bb_state;

		if (is_nd_pfn(dev)) {
			struct nd_pfn *nd_pfn = to_nd_pfn(dev);
			struct nd_pfn_sb *pfn_sb = nd_pfn->pfn_sb;

			ndns = nd_pfn->ndns;
			offset = pmem->data_offset +
					__le32_to_cpu(pfn_sb->start_pad);
			end_trunc = __le32_to_cpu(pfn_sb->end_trunc);
		} else {
			ndns = to_ndns(dev);
		}

		nsio = to_nd_namespace_io(&ndns->dev);
	}

<<<<<<< HEAD
	res.start = nsio->res.start + offset;
	res.end = nsio->res.end - end_trunc;
	nvdimm_badblocks_populate(nd_region, bb, &res);
=======
	range.start = nsio->res.start + offset;
	range.end = nsio->res.end - end_trunc;
	nvdimm_badblocks_populate(nd_region, bb, &range);
>>>>>>> upstream/android-13
	if (bb_state)
		sysfs_notify_dirent(bb_state);
}

<<<<<<< HEAD
=======
static void pmem_revalidate_region(struct device *dev)
{
	struct pmem_device *pmem;

	if (is_nd_btt(dev)) {
		struct nd_btt *nd_btt = to_nd_btt(dev);
		struct btt *btt = nd_btt->btt;

		nvdimm_check_and_set_ro(btt->btt_disk);
		return;
	}

	pmem = dev_get_drvdata(dev);
	nvdimm_check_and_set_ro(pmem->disk);
}

static void nd_pmem_notify(struct device *dev, enum nvdimm_event event)
{
	switch (event) {
	case NVDIMM_REVALIDATE_POISON:
		pmem_revalidate_poison(dev);
		break;
	case NVDIMM_REVALIDATE_REGION:
		pmem_revalidate_region(dev);
		break;
	default:
		dev_WARN_ONCE(dev, 1, "notify: unknown event: %d\n", event);
		break;
	}
}

>>>>>>> upstream/android-13
MODULE_ALIAS("pmem");
MODULE_ALIAS_ND_DEVICE(ND_DEVICE_NAMESPACE_IO);
MODULE_ALIAS_ND_DEVICE(ND_DEVICE_NAMESPACE_PMEM);
static struct nd_device_driver nd_pmem_driver = {
	.probe = nd_pmem_probe,
	.remove = nd_pmem_remove,
	.notify = nd_pmem_notify,
	.shutdown = nd_pmem_shutdown,
	.drv = {
		.name = "nd_pmem",
	},
	.type = ND_DRIVER_NAMESPACE_IO | ND_DRIVER_NAMESPACE_PMEM,
};

module_nd_driver(nd_pmem_driver);

MODULE_AUTHOR("Ross Zwisler <ross.zwisler@linux.intel.com>");
MODULE_LICENSE("GPL v2");
