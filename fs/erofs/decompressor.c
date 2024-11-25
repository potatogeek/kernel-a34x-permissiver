// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright (C) 2019 HUAWEI, Inc.
<<<<<<< HEAD
 *             http://www.huawei.com/
 * Created by Gao Xiang <gaoxiang25@huawei.com>
=======
 *             https://www.huawei.com/
>>>>>>> upstream/android-13
 */
#include "compress.h"
#include <linux/module.h>
#include <linux/lz4.h>

#ifndef LZ4_DISTANCE_MAX	/* history window size */
#define LZ4_DISTANCE_MAX 65535	/* set to maximum value by default */
#endif

#define LZ4_MAX_DISTANCE_PAGES	(DIV_ROUND_UP(LZ4_DISTANCE_MAX, PAGE_SIZE) + 1)
#ifndef LZ4_DECOMPRESS_INPLACE_MARGIN
#define LZ4_DECOMPRESS_INPLACE_MARGIN(srcsize)  (((srcsize) >> 8) + 32)
#endif

<<<<<<< HEAD
struct z_erofs_decompressor {
	/*
	 * if destpages have sparsed pages, fill them with bounce pages.
	 * it also check whether destpages indicate continuous physical memory.
	 */
	int (*prepare_destpages)(struct z_erofs_decompress_req *rq,
				 struct list_head *pagepool);
	int (*decompress)(struct z_erofs_decompress_req *rq, u8 *out);
	char *name;
};

static int z_erofs_lz4_prepare_destpages(struct z_erofs_decompress_req *rq,
					 struct list_head *pagepool)
=======
int z_erofs_load_lz4_config(struct super_block *sb,
			    struct erofs_super_block *dsb,
			    struct z_erofs_lz4_cfgs *lz4, int size)
{
	struct erofs_sb_info *sbi = EROFS_SB(sb);
	u16 distance;

	if (lz4) {
		if (size < sizeof(struct z_erofs_lz4_cfgs)) {
			erofs_err(sb, "invalid lz4 cfgs, size=%u", size);
			return -EINVAL;
		}
		distance = le16_to_cpu(lz4->max_distance);

		sbi->lz4.max_pclusterblks = le16_to_cpu(lz4->max_pclusterblks);
		if (!sbi->lz4.max_pclusterblks) {
			sbi->lz4.max_pclusterblks = 1;	/* reserved case */
		} else if (sbi->lz4.max_pclusterblks >
			   Z_EROFS_PCLUSTER_MAX_SIZE / EROFS_BLKSIZ) {
			erofs_err(sb, "too large lz4 pclusterblks %u",
				  sbi->lz4.max_pclusterblks);
			return -EINVAL;
		} else if (sbi->lz4.max_pclusterblks >= 2) {
			erofs_info(sb, "EXPERIMENTAL big pcluster feature in use. Use at your own risk!");
		}
	} else {
		distance = le16_to_cpu(dsb->u1.lz4_max_distance);
		sbi->lz4.max_pclusterblks = 1;
	}

	sbi->lz4.max_distance_pages = distance ?
					DIV_ROUND_UP(distance, PAGE_SIZE) + 1 :
					LZ4_MAX_DISTANCE_PAGES;
	return erofs_pcpubuf_growsize(sbi->lz4.max_pclusterblks);
}

/*
 * Fill all gaps with bounce pages if it's a sparse page list. Also check if
 * all physical pages are consecutive, which can be seen for moderate CR.
 */
static int z_erofs_lz4_prepare_dstpages(struct z_erofs_decompress_req *rq,
					struct page **pagepool)
>>>>>>> upstream/android-13
{
	const unsigned int nr =
		PAGE_ALIGN(rq->pageofs_out + rq->outputsize) >> PAGE_SHIFT;
	struct page *availables[LZ4_MAX_DISTANCE_PAGES] = { NULL };
	unsigned long bounced[DIV_ROUND_UP(LZ4_MAX_DISTANCE_PAGES,
					   BITS_PER_LONG)] = { 0 };
<<<<<<< HEAD
=======
	unsigned int lz4_max_distance_pages =
				EROFS_SB(rq->sb)->lz4.max_distance_pages;
>>>>>>> upstream/android-13
	void *kaddr = NULL;
	unsigned int i, j, top;

	top = 0;
	for (i = j = 0; i < nr; ++i, ++j) {
		struct page *const page = rq->out[i];
		struct page *victim;

<<<<<<< HEAD
		if (j >= LZ4_MAX_DISTANCE_PAGES)
=======
		if (j >= lz4_max_distance_pages)
>>>>>>> upstream/android-13
			j = 0;

		/* 'valid' bounced can only be tested after a complete round */
		if (test_bit(j, bounced)) {
<<<<<<< HEAD
			DBG_BUGON(i < LZ4_MAX_DISTANCE_PAGES);
			DBG_BUGON(top >= LZ4_MAX_DISTANCE_PAGES);
			availables[top++] = rq->out[i - LZ4_MAX_DISTANCE_PAGES];
=======
			DBG_BUGON(i < lz4_max_distance_pages);
			DBG_BUGON(top >= lz4_max_distance_pages);
			availables[top++] = rq->out[i - lz4_max_distance_pages];
>>>>>>> upstream/android-13
		}

		if (page) {
			__clear_bit(j, bounced);
			if (kaddr) {
				if (kaddr + PAGE_SIZE == page_address(page))
					kaddr += PAGE_SIZE;
				else
					kaddr = NULL;
			} else if (!i) {
				kaddr = page_address(page);
			}
			continue;
		}
		kaddr = NULL;
		__set_bit(j, bounced);

		if (top) {
			victim = availables[--top];
			get_page(victim);
		} else {
<<<<<<< HEAD
			victim = erofs_allocpage(pagepool, GFP_KERNEL, false);
			if (!victim)
				return -ENOMEM;
			victim->mapping = Z_EROFS_MAPPING_STAGING;
=======
			victim = erofs_allocpage(pagepool,
						 GFP_KERNEL | __GFP_NOFAIL);
			set_page_private(victim, Z_EROFS_SHORTLIVED_PAGE);
>>>>>>> upstream/android-13
		}
		rq->out[i] = victim;
	}
	return kaddr ? 1 : 0;
}

<<<<<<< HEAD
static void *generic_copy_inplace_data(struct z_erofs_decompress_req *rq,
				       u8 *src, unsigned int pageofs_in)
{
	/*
	 * if in-place decompression is ongoing, those decompressed
	 * pages should be copied in order to avoid being overlapped.
	 */
	struct page **in = rq->in;
	u8 *const tmp = erofs_get_pcpubuf(0);
	u8 *tmpp = tmp;
	unsigned int inlen = rq->inputsize - pageofs_in;
	unsigned int count = min_t(uint, inlen, PAGE_SIZE - pageofs_in);

	while (tmpp < tmp + inlen) {
		if (!src)
			src = kmap_atomic(*in);
		memcpy(tmpp, src + pageofs_in, count);
		kunmap_atomic(src);
		src = NULL;
		tmpp += count;
		pageofs_in = 0;
		count = PAGE_SIZE;
		++in;
	}
	return tmp;
}

static int z_erofs_lz4_decompress(struct z_erofs_decompress_req *rq, u8 *out)
{
	unsigned int inputmargin, inlen;
	u8 *src;
	bool copied, support_0padding;
	int ret;

	if (rq->inputsize > PAGE_SIZE)
		return -EOPNOTSUPP;

	src = kmap_atomic(*rq->in);
=======
static void *z_erofs_lz4_handle_inplace_io(struct z_erofs_decompress_req *rq,
			void *inpage, unsigned int *inputmargin, int *maptype,
			bool support_0padding)
{
	unsigned int nrpages_in, nrpages_out;
	unsigned int ofull, oend, inputsize, total, i, j;
	struct page **in;
	void *src, *tmp;

	inputsize = rq->inputsize;
	nrpages_in = PAGE_ALIGN(inputsize) >> PAGE_SHIFT;
	oend = rq->pageofs_out + rq->outputsize;
	ofull = PAGE_ALIGN(oend);
	nrpages_out = ofull >> PAGE_SHIFT;

	if (rq->inplace_io) {
		if (rq->partial_decoding || !support_0padding ||
		    ofull - oend < LZ4_DECOMPRESS_INPLACE_MARGIN(inputsize))
			goto docopy;

		for (i = 0; i < nrpages_in; ++i) {
			DBG_BUGON(rq->in[i] == NULL);
			for (j = 0; j < nrpages_out - nrpages_in + i; ++j)
				if (rq->out[j] == rq->in[i])
					goto docopy;
		}
	}

	if (nrpages_in <= 1) {
		*maptype = 0;
		return inpage;
	}
	kunmap_atomic(inpage);
	might_sleep();
	src = erofs_vm_map_ram(rq->in, nrpages_in);
	if (!src)
		return ERR_PTR(-ENOMEM);
	*maptype = 1;
	return src;

docopy:
	/* Or copy compressed data which can be overlapped to per-CPU buffer */
	in = rq->in;
	src = erofs_get_pcpubuf(nrpages_in);
	if (!src) {
		DBG_BUGON(1);
		kunmap_atomic(inpage);
		return ERR_PTR(-EFAULT);
	}

	tmp = src;
	total = rq->inputsize;
	while (total) {
		unsigned int page_copycnt =
			min_t(unsigned int, total, PAGE_SIZE - *inputmargin);

		if (!inpage)
			inpage = kmap_atomic(*in);
		memcpy(tmp, inpage + *inputmargin, page_copycnt);
		kunmap_atomic(inpage);
		inpage = NULL;
		tmp += page_copycnt;
		total -= page_copycnt;
		++in;
		*inputmargin = 0;
	}
	*maptype = 2;
	return src;
}

static int z_erofs_lz4_decompress_mem(struct z_erofs_decompress_req *rq,
				      u8 *out)
{
	unsigned int inputmargin;
	u8 *headpage, *src;
	bool support_0padding;
	int ret, maptype;

	DBG_BUGON(*rq->in == NULL);
	headpage = kmap_atomic(*rq->in);
>>>>>>> upstream/android-13
	inputmargin = 0;
	support_0padding = false;

	/* decompression inplace is only safe when 0padding is enabled */
<<<<<<< HEAD
	if (EROFS_SB(rq->sb)->feature_incompat &
	    EROFS_FEATURE_INCOMPAT_LZ4_0PADDING) {
		support_0padding = true;

		while (!src[inputmargin & ~PAGE_MASK])
=======
	if (erofs_sb_has_lz4_0padding(EROFS_SB(rq->sb))) {
		support_0padding = true;

		while (!headpage[inputmargin & ~PAGE_MASK])
>>>>>>> upstream/android-13
			if (!(++inputmargin & ~PAGE_MASK))
				break;

		if (inputmargin >= rq->inputsize) {
<<<<<<< HEAD
			kunmap_atomic(src);
=======
			kunmap_atomic(headpage);
>>>>>>> upstream/android-13
			return -EIO;
		}
	}

<<<<<<< HEAD
	copied = false;
	inlen = rq->inputsize - inputmargin;
	if (rq->inplace_io) {
		const uint oend = (rq->pageofs_out +
				   rq->outputsize) & ~PAGE_MASK;
		const uint nr = PAGE_ALIGN(rq->pageofs_out +
					   rq->outputsize) >> PAGE_SHIFT;

		if (rq->partial_decoding || !support_0padding ||
		    rq->out[nr - 1] != rq->in[0] ||
		    rq->inputsize - oend <
		      LZ4_DECOMPRESS_INPLACE_MARGIN(inlen)) {
			src = generic_copy_inplace_data(rq, src, inputmargin);
			inputmargin = 0;
			copied = true;
		}
	}

	ret = LZ4_decompress_safe_partial(src + inputmargin, out,
					  inlen, rq->outputsize,
					  rq->outputsize);
	if (ret != rq->outputsize) {
		erofs_err(rq->sb, "failed to decompress %d in[%u, %u] out[%u]",
			  ret, inlen, inputmargin, rq->outputsize);
		WARN_ON(1);
		print_hex_dump(KERN_DEBUG, "[ in]: ", DUMP_PREFIX_OFFSET,
			       16, 1, src + inputmargin, inlen, true);
		print_hex_dump(KERN_DEBUG, "[out]: ", DUMP_PREFIX_OFFSET,
			       16, 1, out, rq->outputsize, true);
		if (ret >= 0)
                     memset(out + ret, 0, rq->outputsize - ret);

		ret = -EIO;
	}

	if (copied)
		erofs_put_pcpubuf(src);
	else
		kunmap_atomic(src);
	return ret;
}

static struct z_erofs_decompressor decompressors[] = {
	[Z_EROFS_COMPRESSION_SHIFTED] = {
		.name = "shifted"
	},
	[Z_EROFS_COMPRESSION_LZ4] = {
		.prepare_destpages = z_erofs_lz4_prepare_destpages,
		.decompress = z_erofs_lz4_decompress,
		.name = "lz4"
	},
};

static void copy_from_pcpubuf(struct page **out, const char *dst,
			      unsigned short pageofs_out,
			      unsigned int outputsize)
{
	const char *end = dst + outputsize;
	const unsigned int righthalf = PAGE_SIZE - pageofs_out;
	const char *cur = dst - pageofs_out;

	while (cur < end) {
		struct page *const page = *out++;

		if (page) {
			char *buf = kmap_atomic(page);

			if (cur >= dst) {
				memcpy(buf, cur, min_t(uint, PAGE_SIZE,
						       end - cur));
			} else {
				memcpy(buf + pageofs_out, cur + pageofs_out,
				       min_t(uint, righthalf, end - cur));
			}
			kunmap_atomic(buf);
		}
		cur += PAGE_SIZE;
	}
}

static int z_erofs_decompress_generic(struct z_erofs_decompress_req *rq,
				      struct list_head *pagepool)
{
	const unsigned int nrpages_out =
		PAGE_ALIGN(rq->pageofs_out + rq->outputsize) >> PAGE_SHIFT;
	const struct z_erofs_decompressor *alg = decompressors + rq->alg;
	unsigned int dst_maptype;
	void *dst;
	int ret, i;

	if (nrpages_out == 1 && !rq->inplace_io) {
=======
	rq->inputsize -= inputmargin;
	src = z_erofs_lz4_handle_inplace_io(rq, headpage, &inputmargin,
					    &maptype, support_0padding);
	if (IS_ERR(src))
		return PTR_ERR(src);

	/* legacy format could compress extra data in a pcluster. */
	if (rq->partial_decoding || !support_0padding)
		ret = LZ4_decompress_safe_partial(src + inputmargin, out,
				rq->inputsize, rq->outputsize, rq->outputsize);
	else
		ret = LZ4_decompress_safe(src + inputmargin, out,
					  rq->inputsize, rq->outputsize);

	if (ret != rq->outputsize) {
		erofs_err(rq->sb, "failed to decompress %d in[%u, %u] out[%u]",
			  ret, rq->inputsize, inputmargin, rq->outputsize);

		print_hex_dump(KERN_DEBUG, "[ in]: ", DUMP_PREFIX_OFFSET,
			       16, 1, src + inputmargin, rq->inputsize, true);
		print_hex_dump(KERN_DEBUG, "[out]: ", DUMP_PREFIX_OFFSET,
			       16, 1, out, rq->outputsize, true);

		if (ret >= 0)
			memset(out + ret, 0, rq->outputsize - ret);
		ret = -EIO;
	} else {
		ret = 0;
	}

	if (maptype == 0) {
		kunmap_atomic(src);
	} else if (maptype == 1) {
		vm_unmap_ram(src, PAGE_ALIGN(rq->inputsize) >> PAGE_SHIFT);
	} else if (maptype == 2) {
		erofs_put_pcpubuf(src);
	} else {
		DBG_BUGON(1);
		return -EFAULT;
	}
	return ret;
}

static int z_erofs_lz4_decompress(struct z_erofs_decompress_req *rq,
				  struct page **pagepool)
{
	const unsigned int nrpages_out =
		PAGE_ALIGN(rq->pageofs_out + rq->outputsize) >> PAGE_SHIFT;
	unsigned int dst_maptype;
	void *dst;
	int ret;

	/* one optimized fast path only for non bigpcluster cases yet */
	if (rq->inputsize <= PAGE_SIZE && nrpages_out == 1 && !rq->inplace_io) {
>>>>>>> upstream/android-13
		DBG_BUGON(!*rq->out);
		dst = kmap_atomic(*rq->out);
		dst_maptype = 0;
		goto dstmap_out;
	}

<<<<<<< HEAD
	/*
	 * For the case of small output size (especially much less
	 * than PAGE_SIZE), memcpy the decompressed data rather than
	 * compressed data is preferred.
	 */
	if (rq->outputsize <= PAGE_SIZE * 7 / 8) {
		dst = erofs_get_pcpubuf(0);
		if (IS_ERR(dst))
			return PTR_ERR(dst);

		rq->inplace_io = false;
		ret = alg->decompress(rq, dst);
		if (!ret)
			copy_from_pcpubuf(rq->out, dst, rq->pageofs_out,
					  rq->outputsize);

		erofs_put_pcpubuf(dst);
		return ret;
	}

	ret = alg->prepare_destpages(rq, pagepool);
	if (ret < 0) {
		return ret;
	} else if (ret) {
=======
	/* general decoding path which can be used for all cases */
	ret = z_erofs_lz4_prepare_dstpages(rq, pagepool);
	if (ret < 0)
		return ret;
	if (ret) {
>>>>>>> upstream/android-13
		dst = page_address(*rq->out);
		dst_maptype = 1;
		goto dstmap_out;
	}

<<<<<<< HEAD
	i = 0;
	while (1) {
		dst = vm_map_ram(rq->out, nrpages_out, -1, PAGE_KERNEL);

		/* retry two more times (totally 3 times) */
		if (dst || ++i >= 3)
			break;
		vm_unmap_aliases();
	}

	if (!dst)
		return -ENOMEM;

	dst_maptype = 2;

dstmap_out:
	ret = alg->decompress(rq, dst + rq->pageofs_out);
=======
	dst = erofs_vm_map_ram(rq->out, nrpages_out);
	if (!dst)
		return -ENOMEM;
	dst_maptype = 2;

dstmap_out:
	ret = z_erofs_lz4_decompress_mem(rq, dst + rq->pageofs_out);
>>>>>>> upstream/android-13

	if (!dst_maptype)
		kunmap_atomic(dst);
	else if (dst_maptype == 2)
		vm_unmap_ram(dst, nrpages_out);
	return ret;
}

<<<<<<< HEAD
static int z_erofs_shifted_transform(const struct z_erofs_decompress_req *rq,
				     struct list_head *pagepool)
=======
static int z_erofs_shifted_transform(struct z_erofs_decompress_req *rq,
				     struct page **pagepool)
>>>>>>> upstream/android-13
{
	const unsigned int nrpages_out =
		PAGE_ALIGN(rq->pageofs_out + rq->outputsize) >> PAGE_SHIFT;
	const unsigned int righthalf = PAGE_SIZE - rq->pageofs_out;
	unsigned char *src, *dst;

	if (nrpages_out > 2) {
		DBG_BUGON(1);
		return -EIO;
	}

	if (rq->out[0] == *rq->in) {
		DBG_BUGON(nrpages_out != 1);
		return 0;
	}

	src = kmap_atomic(*rq->in);
<<<<<<< HEAD
	if (!rq->out[0]) {
		dst = NULL;
	} else {
		dst = kmap_atomic(rq->out[0]);
		memcpy(dst + rq->pageofs_out, src, righthalf);
	}

	if (rq->out[1] == *rq->in) {
		memmove(src, src + righthalf, rq->pageofs_out);
	} else if (nrpages_out == 2) {
		if (dst)
			kunmap_atomic(dst);
		DBG_BUGON(!rq->out[1]);
		dst = kmap_atomic(rq->out[1]);
		memcpy(dst, src + righthalf, rq->pageofs_out);
	}
	if (dst)
		kunmap_atomic(dst);
=======
	if (rq->out[0]) {
		dst = kmap_atomic(rq->out[0]);
		memcpy(dst + rq->pageofs_out, src, righthalf);
		kunmap_atomic(dst);
	}

	if (nrpages_out == 2) {
		DBG_BUGON(!rq->out[1]);
		if (rq->out[1] == *rq->in) {
			memmove(src, src + righthalf, rq->pageofs_out);
		} else {
			dst = kmap_atomic(rq->out[1]);
			memcpy(dst, src + righthalf, rq->pageofs_out);
			kunmap_atomic(dst);
		}
	}
>>>>>>> upstream/android-13
	kunmap_atomic(src);
	return 0;
}

<<<<<<< HEAD
int z_erofs_decompress(struct z_erofs_decompress_req *rq,
		       struct list_head *pagepool)
{
	if (rq->alg == Z_EROFS_COMPRESSION_SHIFTED)
		return z_erofs_shifted_transform(rq, pagepool);
	return z_erofs_decompress_generic(rq, pagepool);
}

=======
static struct z_erofs_decompressor decompressors[] = {
	[Z_EROFS_COMPRESSION_SHIFTED] = {
		.decompress = z_erofs_shifted_transform,
		.name = "shifted"
	},
	[Z_EROFS_COMPRESSION_LZ4] = {
		.decompress = z_erofs_lz4_decompress,
		.name = "lz4"
	},
#ifdef CONFIG_EROFS_FS_ZIP_LZMA
	[Z_EROFS_COMPRESSION_LZMA] = {
		.decompress = z_erofs_lzma_decompress,
		.name = "lzma"
	},
#endif
};

int z_erofs_decompress(struct z_erofs_decompress_req *rq,
		       struct page **pagepool)
{
	return decompressors[rq->alg].decompress(rq, pagepool);
}
>>>>>>> upstream/android-13
