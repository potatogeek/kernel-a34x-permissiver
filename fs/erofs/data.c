// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright (C) 2017-2018 HUAWEI, Inc.
<<<<<<< HEAD
 *             http://www.huawei.com/
 * Created by Gao Xiang <gaoxiang25@huawei.com>
 */
#include "internal.h"
#include <linux/prefetch.h>

#include <trace/events/erofs.h>

static void erofs_readendio(struct bio *bio)
{
	struct bio_vec *bvec;
	blk_status_t err = bio->bi_status;
	int i;

	bio_for_each_segment_all(bvec, bio, i) {
		struct page *page = bvec->bv_page;

		/* page is already locked */
		DBG_BUGON(PageUptodate(page));

		if (err)
			SetPageError(page);
		else
			SetPageUptodate(page);

		unlock_page(page);
		/* page could be reclaimed now */
	}
	bio_put(bio);
}

=======
 *             https://www.huawei.com/
 * Copyright (C) 2021, Alibaba Cloud
 */
#include "internal.h"
#include <linux/prefetch.h>
#include <linux/dax.h>
#include <trace/events/erofs.h>

>>>>>>> upstream/android-13
struct page *erofs_get_meta_page(struct super_block *sb, erofs_blk_t blkaddr)
{
	struct address_space *const mapping = sb->s_bdev->bd_inode->i_mapping;
	struct page *page;

	page = read_cache_page_gfp(mapping, blkaddr,
				   mapping_gfp_constraint(mapping, ~__GFP_FS));
	/* should already be PageUptodate */
	if (!IS_ERR(page))
		lock_page(page);
	return page;
}

static int erofs_map_blocks_flatmode(struct inode *inode,
				     struct erofs_map_blocks *map,
				     int flags)
{
	int err = 0;
	erofs_blk_t nblocks, lastblk;
	u64 offset = map->m_la;
	struct erofs_inode *vi = EROFS_I(inode);
	bool tailendpacking = (vi->datalayout == EROFS_INODE_FLAT_INLINE);

	trace_erofs_map_blocks_flatmode_enter(inode, map, flags);

	nblocks = DIV_ROUND_UP(inode->i_size, PAGE_SIZE);
	lastblk = nblocks - tailendpacking;

<<<<<<< HEAD
	if (offset >= inode->i_size) {
		/* leave out-of-bound access unmapped */
		map->m_flags = 0;
		map->m_plen = 0;
		goto out;
	}

=======
>>>>>>> upstream/android-13
	/* there is no hole in flatmode */
	map->m_flags = EROFS_MAP_MAPPED;

	if (offset < blknr_to_addr(lastblk)) {
		map->m_pa = blknr_to_addr(vi->raw_blkaddr) + map->m_la;
		map->m_plen = blknr_to_addr(lastblk) - offset;
	} else if (tailendpacking) {
		/* 2 - inode inline B: inode, [xattrs], inline last blk... */
		struct erofs_sb_info *sbi = EROFS_SB(inode->i_sb);

		map->m_pa = iloc(sbi, vi->nid) + vi->inode_isize +
			vi->xattr_isize + erofs_blkoff(map->m_la);
		map->m_plen = inode->i_size - offset;

		/* inline data should be located in one meta block */
		if (erofs_blkoff(map->m_pa) + map->m_plen > PAGE_SIZE) {
			erofs_err(inode->i_sb,
				  "inline data cross block boundary @ nid %llu",
				  vi->nid);
			DBG_BUGON(1);
			err = -EFSCORRUPTED;
			goto err_out;
		}

		map->m_flags |= EROFS_MAP_META;
	} else {
		erofs_err(inode->i_sb,
			  "internal error @ nid: %llu (size %llu), m_la 0x%llx",
			  vi->nid, inode->i_size, map->m_la);
		DBG_BUGON(1);
		err = -EIO;
		goto err_out;
	}

<<<<<<< HEAD
out:
	map->m_llen = map->m_plen;

=======
	map->m_llen = map->m_plen;
>>>>>>> upstream/android-13
err_out:
	trace_erofs_map_blocks_flatmode_exit(inode, map, flags, 0);
	return err;
}

<<<<<<< HEAD
int erofs_map_blocks(struct inode *inode,
		     struct erofs_map_blocks *map, int flags)
{
	if (erofs_inode_is_data_compressed(EROFS_I(inode)->datalayout)) {
		int err = z_erofs_map_blocks_iter(inode, map, flags);

		if (map->mpage) {
			put_page(map->mpage);
			map->mpage = NULL;
		}
		return err;
	}
	return erofs_map_blocks_flatmode(inode, map, flags);
}

static inline struct bio *erofs_read_raw_page(struct bio *bio,
					      struct address_space *mapping,
					      struct page *page,
					      erofs_off_t *last_block,
					      unsigned int nblocks,
					      bool ra)
{
	struct inode *const inode = mapping->host;
	struct super_block *const sb = inode->i_sb;
	erofs_off_t current_block = (erofs_off_t)page->index;
	int err;

	DBG_BUGON(!nblocks);

	if (PageUptodate(page)) {
		err = 0;
		goto has_updated;
	}
	
	if (cleancache_get_page(page) == 0) {
		err = 0;
		SetPageMappedToDisk(page);
		SetPageUptodate(page);
		goto has_updated;
	}

	/* note that for readpage case, bio also equals to NULL */
	if (bio &&
	    /* not continuous */
	    *last_block + 1 != current_block) {
submit_bio_retry:
		submit_bio(bio);
		bio = NULL;
	}

	if (!bio) {
		struct erofs_map_blocks map = {
			.m_la = blknr_to_addr(current_block),
		};
		erofs_blk_t blknr;
		unsigned int blkoff;

		err = erofs_map_blocks(inode, &map, EROFS_GET_BLOCKS_RAW);
		if (err)
			goto err_out;

		if ((map.m_flags & EROFS_MAP_MAPPED)) 
			SetPageMappedToDisk(page);

		/* zero out the holed page */
		if (!(map.m_flags & EROFS_MAP_MAPPED)) {
			zero_user_segment(page, 0, PAGE_SIZE);
			SetPageUptodate(page);

			/* imply err = 0, see erofs_map_blocks */
			goto has_updated;
		}

		/* for RAW access mode, m_plen must be equal to m_llen */
		DBG_BUGON(map.m_plen != map.m_llen);

		blknr = erofs_blknr(map.m_pa);
		blkoff = erofs_blkoff(map.m_pa);

		/* deal with inline page */
		if (map.m_flags & EROFS_MAP_META) {
			void *vsrc, *vto;
			struct page *ipage;

			DBG_BUGON(map.m_plen > PAGE_SIZE);

			ipage = erofs_get_meta_page(inode->i_sb, blknr);

			if (IS_ERR(ipage)) {
				err = PTR_ERR(ipage);
				goto err_out;
			}

			vsrc = kmap_atomic(ipage);
			vto = kmap_atomic(page);
			memcpy(vto, vsrc + blkoff, map.m_plen);
			memset(vto + map.m_plen, 0, PAGE_SIZE - map.m_plen);
			kunmap_atomic(vto);
			kunmap_atomic(vsrc);
			flush_dcache_page(page);

			SetPageUptodate(page);
			/* TODO: could we unlock the page earlier? */
			unlock_page(ipage);
			put_page(ipage);

			/* imply err = 0, see erofs_map_blocks */
			goto has_updated;
		}

		/* pa must be block-aligned for raw reading */
		DBG_BUGON(erofs_blkoff(map.m_pa));

		/* max # of continuous pages */
		if (nblocks > DIV_ROUND_UP(map.m_plen, PAGE_SIZE))
			nblocks = DIV_ROUND_UP(map.m_plen, PAGE_SIZE);
		if (nblocks > BIO_MAX_PAGES)
			nblocks = BIO_MAX_PAGES;

		bio = bio_alloc(GFP_NOIO, nblocks);

		bio->bi_end_io = erofs_readendio;
		bio_set_dev(bio, sb->s_bdev);
		bio->bi_iter.bi_sector = (sector_t)blknr <<
			LOG_SECTORS_PER_BLOCK;
		bio->bi_opf = REQ_OP_READ;
	}

	SetPageMappedToDisk(page);
	err = bio_add_page(bio, page, PAGE_SIZE, 0);
	/* out of the extent or bio is full */
	if (err < PAGE_SIZE)
		goto submit_bio_retry;

	*last_block = current_block;

	/* shift in advance in case of it followed by too many gaps */
	if (bio->bi_iter.bi_size >= bio->bi_max_vecs * PAGE_SIZE) {
		/* err should reassign to 0 after submitting */
		err = 0;
		goto submit_bio_out;
	}

	return bio;

err_out:
	/* for sync reading, set page error immediately */
	if (!ra) {
		SetPageError(page);
		ClearPageUptodate(page);
	}
has_updated:
	unlock_page(page);

	/* if updated manually, continuous pages has a gap */
	if (bio)
submit_bio_out:
		submit_bio(bio);
	return err ? ERR_PTR(err) : NULL;
=======
static int erofs_map_blocks(struct inode *inode,
			    struct erofs_map_blocks *map, int flags)
{
	struct super_block *sb = inode->i_sb;
	struct erofs_inode *vi = EROFS_I(inode);
	struct erofs_inode_chunk_index *idx;
	struct page *page;
	u64 chunknr;
	unsigned int unit;
	erofs_off_t pos;
	int err = 0;

	map->m_deviceid = 0;
	if (map->m_la >= inode->i_size) {
		/* leave out-of-bound access unmapped */
		map->m_flags = 0;
		map->m_plen = 0;
		goto out;
	}

	if (vi->datalayout != EROFS_INODE_CHUNK_BASED)
		return erofs_map_blocks_flatmode(inode, map, flags);

	if (vi->chunkformat & EROFS_CHUNK_FORMAT_INDEXES)
		unit = sizeof(*idx);			/* chunk index */
	else
		unit = EROFS_BLOCK_MAP_ENTRY_SIZE;	/* block map */

	chunknr = map->m_la >> vi->chunkbits;
	pos = ALIGN(iloc(EROFS_SB(sb), vi->nid) + vi->inode_isize +
		    vi->xattr_isize, unit) + unit * chunknr;

	page = erofs_get_meta_page(inode->i_sb, erofs_blknr(pos));
	if (IS_ERR(page))
		return PTR_ERR(page);

	map->m_la = chunknr << vi->chunkbits;
	map->m_plen = min_t(erofs_off_t, 1UL << vi->chunkbits,
			    roundup(inode->i_size - map->m_la, EROFS_BLKSIZ));

	/* handle block map */
	if (!(vi->chunkformat & EROFS_CHUNK_FORMAT_INDEXES)) {
		__le32 *blkaddr = page_address(page) + erofs_blkoff(pos);

		if (le32_to_cpu(*blkaddr) == EROFS_NULL_ADDR) {
			map->m_flags = 0;
		} else {
			map->m_pa = blknr_to_addr(le32_to_cpu(*blkaddr));
			map->m_flags = EROFS_MAP_MAPPED;
		}
		goto out_unlock;
	}
	/* parse chunk indexes */
	idx = page_address(page) + erofs_blkoff(pos);
	switch (le32_to_cpu(idx->blkaddr)) {
	case EROFS_NULL_ADDR:
		map->m_flags = 0;
		break;
	default:
		map->m_deviceid = le16_to_cpu(idx->device_id) &
			EROFS_SB(sb)->device_id_mask;
		map->m_pa = blknr_to_addr(le32_to_cpu(idx->blkaddr));
		map->m_flags = EROFS_MAP_MAPPED;
		break;
	}
out_unlock:
	unlock_page(page);
	put_page(page);
out:
	map->m_llen = map->m_plen;
	return err;
}

int erofs_map_dev(struct super_block *sb, struct erofs_map_dev *map)
{
	struct erofs_dev_context *devs = EROFS_SB(sb)->devs;
	struct erofs_device_info *dif;
	int id;

	/* primary device by default */
	map->m_bdev = sb->s_bdev;
	map->m_daxdev = EROFS_SB(sb)->dax_dev;

	if (map->m_deviceid) {
		down_read(&devs->rwsem);
		dif = idr_find(&devs->tree, map->m_deviceid - 1);
		if (!dif) {
			up_read(&devs->rwsem);
			return -ENODEV;
		}
		map->m_bdev = dif->bdev;
		map->m_daxdev = dif->dax_dev;
		up_read(&devs->rwsem);
	} else if (devs->extra_devices) {
		down_read(&devs->rwsem);
		idr_for_each_entry(&devs->tree, dif, id) {
			erofs_off_t startoff, length;

			if (!dif->mapped_blkaddr)
				continue;
			startoff = blknr_to_addr(dif->mapped_blkaddr);
			length = blknr_to_addr(dif->blocks);

			if (map->m_pa >= startoff &&
			    map->m_pa < startoff + length) {
				map->m_pa -= startoff;
				map->m_bdev = dif->bdev;
				map->m_daxdev = dif->dax_dev;
				break;
			}
		}
		up_read(&devs->rwsem);
	}
	return 0;
}

static int erofs_iomap_begin(struct inode *inode, loff_t offset, loff_t length,
		unsigned int flags, struct iomap *iomap, struct iomap *srcmap)
{
	int ret;
	struct erofs_map_blocks map;
	struct erofs_map_dev mdev;

	map.m_la = offset;
	map.m_llen = length;

	ret = erofs_map_blocks(inode, &map, EROFS_GET_BLOCKS_RAW);
	if (ret < 0)
		return ret;

	mdev = (struct erofs_map_dev) {
		.m_deviceid = map.m_deviceid,
		.m_pa = map.m_pa,
	};
	ret = erofs_map_dev(inode->i_sb, &mdev);
	if (ret)
		return ret;

	iomap->bdev = mdev.m_bdev;
	iomap->dax_dev = mdev.m_daxdev;
	iomap->offset = map.m_la;
	iomap->length = map.m_llen;
	iomap->flags = 0;
	iomap->private = NULL;

	if (!(map.m_flags & EROFS_MAP_MAPPED)) {
		iomap->type = IOMAP_HOLE;
		iomap->addr = IOMAP_NULL_ADDR;
		if (!iomap->length)
			iomap->length = length;
		return 0;
	}

	if (map.m_flags & EROFS_MAP_META) {
		struct page *ipage;

		iomap->type = IOMAP_INLINE;
		ipage = erofs_get_meta_page(inode->i_sb,
					    erofs_blknr(mdev.m_pa));
		if (IS_ERR(ipage))
			return PTR_ERR(ipage);
		iomap->inline_data = page_address(ipage) +
					erofs_blkoff(mdev.m_pa);
		iomap->private = ipage;
	} else {
		iomap->type = IOMAP_MAPPED;
		iomap->addr = mdev.m_pa;
	}
	return 0;
}

static int erofs_iomap_end(struct inode *inode, loff_t pos, loff_t length,
		ssize_t written, unsigned int flags, struct iomap *iomap)
{
	struct page *ipage = iomap->private;

	if (ipage) {
		DBG_BUGON(iomap->type != IOMAP_INLINE);
		unlock_page(ipage);
		put_page(ipage);
	} else {
		DBG_BUGON(iomap->type == IOMAP_INLINE);
	}
	return written;
}

static const struct iomap_ops erofs_iomap_ops = {
	.iomap_begin = erofs_iomap_begin,
	.iomap_end = erofs_iomap_end,
};

int erofs_fiemap(struct inode *inode, struct fiemap_extent_info *fieinfo,
		 u64 start, u64 len)
{
	if (erofs_inode_is_data_compressed(EROFS_I(inode)->datalayout)) {
#ifdef CONFIG_EROFS_FS_ZIP
		return iomap_fiemap(inode, fieinfo, start, len,
				    &z_erofs_iomap_report_ops);
#else
		return -EOPNOTSUPP;
#endif
	}
	return iomap_fiemap(inode, fieinfo, start, len, &erofs_iomap_ops);
>>>>>>> upstream/android-13
}

/*
 * since we dont have write or truncate flows, so no inode
 * locking needs to be held at the moment.
 */
<<<<<<< HEAD
static int erofs_raw_access_readpage(struct file *file, struct page *page)
{
	erofs_off_t last_block;
	struct bio *bio;

	trace_erofs_readpage(page, true);

	bio = erofs_read_raw_page(NULL, page->mapping,
				  page, &last_block, 1, false);

	if (IS_ERR(bio))
		return PTR_ERR(bio);

	DBG_BUGON(bio);	/* since we have only one bio -- must be NULL */
	return 0;
}

static int erofs_raw_access_readpages(struct file *filp,
				      struct address_space *mapping,
				      struct list_head *pages,
				      unsigned int nr_pages)
{
	erofs_off_t last_block;
	struct bio *bio = NULL;
	gfp_t gfp = readahead_gfp_mask(mapping);
	struct page *page = list_last_entry(pages, struct page, lru);

	trace_erofs_readpages(mapping->host, page, nr_pages, true);

	for (; nr_pages; --nr_pages) {
		page = list_entry(pages->prev, struct page, lru);

		prefetchw(&page->flags);
		list_del(&page->lru);

		if (!add_to_page_cache_lru(page, mapping, page->index, gfp)) {
			bio = erofs_read_raw_page(bio, mapping, page,
						  &last_block, nr_pages, true);

			/* all the page errors are ignored when readahead */
			if (IS_ERR(bio)) {
				pr_err("%s, readahead error at page %lu of nid %llu\n",
				       __func__, page->index,
				       EROFS_I(mapping->host)->nid);

				bio = NULL;
			}
		}

		/* pages could still be locked */
		put_page(page);
	}
	DBG_BUGON(!list_empty(pages));

	/* the rare case (end in gaps) */
	if (bio)
		submit_bio(bio);
	return 0;
}

static int erofs_get_block(struct inode *inode, sector_t iblock,
			   struct buffer_head *bh, int create)
{
	struct erofs_map_blocks map = {
		.m_la = iblock << 9,
	};
	int err;

	err = erofs_map_blocks(inode, &map, EROFS_GET_BLOCKS_RAW);
	if (err)
		return err;

	if (map.m_flags & EROFS_MAP_MAPPED)
		bh->b_blocknr = erofs_blknr(map.m_pa);

	return err;
=======
static int erofs_readpage(struct file *file, struct page *page)
{
	return iomap_readpage(page, &erofs_iomap_ops);
}

static void erofs_readahead(struct readahead_control *rac)
{
	return iomap_readahead(rac, &erofs_iomap_ops);
>>>>>>> upstream/android-13
}

static sector_t erofs_bmap(struct address_space *mapping, sector_t block)
{
<<<<<<< HEAD
	struct inode *inode = mapping->host;

	if (EROFS_I(inode)->datalayout == EROFS_INODE_FLAT_INLINE) {
		erofs_blk_t blks = i_size_read(inode) >> LOG_BLOCK_SIZE;

		if (block >> LOG_SECTORS_PER_BLOCK >= blks)
			return 0;
	}

	return generic_block_bmap(mapping, block, erofs_get_block);
=======
	return iomap_bmap(mapping, block, &erofs_iomap_ops);
}

static int erofs_prepare_dio(struct kiocb *iocb, struct iov_iter *to)
{
	struct inode *inode = file_inode(iocb->ki_filp);
	loff_t align = iocb->ki_pos | iov_iter_count(to) |
		iov_iter_alignment(to);
	struct block_device *bdev = inode->i_sb->s_bdev;
	unsigned int blksize_mask;

	if (bdev)
		blksize_mask = (1 << ilog2(bdev_logical_block_size(bdev))) - 1;
	else
		blksize_mask = (1 << inode->i_blkbits) - 1;

	if (align & blksize_mask)
		return -EINVAL;
	return 0;
}

static ssize_t erofs_file_read_iter(struct kiocb *iocb, struct iov_iter *to)
{
	/* no need taking (shared) inode lock since it's a ro filesystem */
	if (!iov_iter_count(to))
		return 0;

#ifdef CONFIG_FS_DAX
	if (IS_DAX(iocb->ki_filp->f_mapping->host))
		return dax_iomap_rw(iocb, to, &erofs_iomap_ops);
#endif
	if (iocb->ki_flags & IOCB_DIRECT) {
		int err = erofs_prepare_dio(iocb, to);

		if (!err)
			return iomap_dio_rw(iocb, to, &erofs_iomap_ops,
					    NULL, 0, 0);
		if (err < 0)
			return err;
	}
	return filemap_read(iocb, to, 0);
>>>>>>> upstream/android-13
}

/* for uncompressed (aligned) files and raw access for other files */
const struct address_space_operations erofs_raw_access_aops = {
<<<<<<< HEAD
	.readpage = erofs_raw_access_readpage,
	.readpages = erofs_raw_access_readpages,
	.bmap = erofs_bmap,
};

=======
	.readpage = erofs_readpage,
	.readahead = erofs_readahead,
	.bmap = erofs_bmap,
	.direct_IO = noop_direct_IO,
};

#ifdef CONFIG_FS_DAX
static vm_fault_t erofs_dax_huge_fault(struct vm_fault *vmf,
		enum page_entry_size pe_size)
{
	return dax_iomap_fault(vmf, pe_size, NULL, NULL, &erofs_iomap_ops);
}

static vm_fault_t erofs_dax_fault(struct vm_fault *vmf)
{
	return erofs_dax_huge_fault(vmf, PE_SIZE_PTE);
}

static const struct vm_operations_struct erofs_dax_vm_ops = {
	.fault		= erofs_dax_fault,
	.huge_fault	= erofs_dax_huge_fault,
};

static int erofs_file_mmap(struct file *file, struct vm_area_struct *vma)
{
	if (!IS_DAX(file_inode(file)))
		return generic_file_readonly_mmap(file, vma);

	if ((vma->vm_flags & VM_SHARED) && (vma->vm_flags & VM_MAYWRITE))
		return -EINVAL;

	vma->vm_ops = &erofs_dax_vm_ops;
	vma->vm_flags |= VM_HUGEPAGE;
	return 0;
}
#else
#define erofs_file_mmap	generic_file_readonly_mmap
#endif

const struct file_operations erofs_file_fops = {
	.llseek		= generic_file_llseek,
	.read_iter	= erofs_file_read_iter,
	.mmap		= erofs_file_mmap,
	.splice_read	= generic_file_splice_read,
};
>>>>>>> upstream/android-13
