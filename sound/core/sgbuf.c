<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> upstream/android-13
/*
 * Scatter-Gather buffer
 *
 *  Copyright (c) by Takashi Iwai <tiwai@suse.de>
<<<<<<< HEAD
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
 *
=======
>>>>>>> upstream/android-13
 */

#include <linux/slab.h>
#include <linux/mm.h>
#include <linux/vmalloc.h>
#include <linux/export.h>
#include <sound/memalloc.h>
<<<<<<< HEAD

=======
#include "memalloc_local.h"

struct snd_sg_page {
	void *buf;
	dma_addr_t addr;
};

struct snd_sg_buf {
	int size;	/* allocated byte size */
	int pages;	/* allocated pages */
	int tblsize;	/* allocated table size */
	struct snd_sg_page *table;	/* address table */
	struct page **page_table;	/* page table (for vmap/vunmap) */
	struct device *dev;
};
>>>>>>> upstream/android-13

/* table entries are align to 32 */
#define SGBUF_TBL_ALIGN		32
#define sgbuf_align_table(tbl)	ALIGN((tbl), SGBUF_TBL_ALIGN)

<<<<<<< HEAD
int snd_free_sgbuf_pages(struct snd_dma_buffer *dmab)
=======
static void snd_dma_sg_free(struct snd_dma_buffer *dmab)
>>>>>>> upstream/android-13
{
	struct snd_sg_buf *sgbuf = dmab->private_data;
	struct snd_dma_buffer tmpb;
	int i;

<<<<<<< HEAD
	if (! sgbuf)
		return -EINVAL;
=======
	if (!sgbuf)
		return;
>>>>>>> upstream/android-13

	vunmap(dmab->area);
	dmab->area = NULL;

	tmpb.dev.type = SNDRV_DMA_TYPE_DEV;
<<<<<<< HEAD
=======
	if (dmab->dev.type == SNDRV_DMA_TYPE_DEV_WC_SG)
		tmpb.dev.type = SNDRV_DMA_TYPE_DEV_WC;
>>>>>>> upstream/android-13
	tmpb.dev.dev = sgbuf->dev;
	for (i = 0; i < sgbuf->pages; i++) {
		if (!(sgbuf->table[i].addr & ~PAGE_MASK))
			continue; /* continuous pages */
		tmpb.area = sgbuf->table[i].buf;
		tmpb.addr = sgbuf->table[i].addr & PAGE_MASK;
		tmpb.bytes = (sgbuf->table[i].addr & ~PAGE_MASK) << PAGE_SHIFT;
		snd_dma_free_pages(&tmpb);
	}

	kfree(sgbuf->table);
	kfree(sgbuf->page_table);
	kfree(sgbuf);
	dmab->private_data = NULL;
<<<<<<< HEAD
	
	return 0;
=======
>>>>>>> upstream/android-13
}

#define MAX_ALLOC_PAGES		32

<<<<<<< HEAD
void *snd_malloc_sgbuf_pages(struct device *device,
			     size_t size, struct snd_dma_buffer *dmab,
			     size_t *res_size)
=======
static void *snd_dma_sg_alloc(struct snd_dma_buffer *dmab, size_t size)
>>>>>>> upstream/android-13
{
	struct snd_sg_buf *sgbuf;
	unsigned int i, pages, chunk, maxpages;
	struct snd_dma_buffer tmpb;
	struct snd_sg_page *table;
	struct page **pgtable;
<<<<<<< HEAD

	dmab->area = NULL;
	dmab->addr = 0;
	dmab->private_data = sgbuf = kzalloc(sizeof(*sgbuf), GFP_KERNEL);
	if (! sgbuf)
		return NULL;
	sgbuf->dev = device;
=======
	int type = SNDRV_DMA_TYPE_DEV;
	pgprot_t prot = PAGE_KERNEL;
	void *area;

	dmab->private_data = sgbuf = kzalloc(sizeof(*sgbuf), GFP_KERNEL);
	if (!sgbuf)
		return NULL;
	if (dmab->dev.type == SNDRV_DMA_TYPE_DEV_WC_SG) {
		type = SNDRV_DMA_TYPE_DEV_WC;
#ifdef pgprot_noncached
		prot = pgprot_noncached(PAGE_KERNEL);
#endif
	}
	sgbuf->dev = dmab->dev.dev;
>>>>>>> upstream/android-13
	pages = snd_sgbuf_aligned_pages(size);
	sgbuf->tblsize = sgbuf_align_table(pages);
	table = kcalloc(sgbuf->tblsize, sizeof(*table), GFP_KERNEL);
	if (!table)
		goto _failed;
	sgbuf->table = table;
	pgtable = kcalloc(sgbuf->tblsize, sizeof(*pgtable), GFP_KERNEL);
	if (!pgtable)
		goto _failed;
	sgbuf->page_table = pgtable;

	/* allocate pages */
	maxpages = MAX_ALLOC_PAGES;
	while (pages > 0) {
		chunk = pages;
		/* don't be too eager to take a huge chunk */
		if (chunk > maxpages)
			chunk = maxpages;
		chunk <<= PAGE_SHIFT;
<<<<<<< HEAD
		if (snd_dma_alloc_pages_fallback(SNDRV_DMA_TYPE_DEV, device,
						 chunk, &tmpb) < 0) {
			if (!sgbuf->pages)
				goto _failed;
			if (!res_size)
				goto _failed;
=======
		if (snd_dma_alloc_pages_fallback(type, dmab->dev.dev,
						 chunk, &tmpb) < 0) {
			if (!sgbuf->pages)
				goto _failed;
>>>>>>> upstream/android-13
			size = sgbuf->pages * PAGE_SIZE;
			break;
		}
		chunk = tmpb.bytes >> PAGE_SHIFT;
		for (i = 0; i < chunk; i++) {
			table->buf = tmpb.area;
			table->addr = tmpb.addr;
			if (!i)
				table->addr |= chunk; /* mark head */
			table++;
			*pgtable++ = virt_to_page(tmpb.area);
			tmpb.area += PAGE_SIZE;
			tmpb.addr += PAGE_SIZE;
		}
		sgbuf->pages += chunk;
		pages -= chunk;
		if (chunk < maxpages)
			maxpages = chunk;
	}

	sgbuf->size = size;
<<<<<<< HEAD
	dmab->area = vmap(sgbuf->page_table, sgbuf->pages, VM_MAP, PAGE_KERNEL);
	if (! dmab->area)
		goto _failed;
	if (res_size)
		*res_size = sgbuf->size;
	return dmab->area;

 _failed:
	snd_free_sgbuf_pages(dmab); /* free the table */
	return NULL;
}

/*
 * compute the max chunk size with continuous pages on sg-buffer
 */
unsigned int snd_sgbuf_get_chunk_size(struct snd_dma_buffer *dmab,
				      unsigned int ofs, unsigned int size)
=======
	area = vmap(sgbuf->page_table, sgbuf->pages, VM_MAP, prot);
	if (!area)
		goto _failed;
	return area;

 _failed:
	snd_dma_sg_free(dmab); /* free the table */
	return NULL;
}

static dma_addr_t snd_dma_sg_get_addr(struct snd_dma_buffer *dmab,
				      size_t offset)
{
	struct snd_sg_buf *sgbuf = dmab->private_data;
	dma_addr_t addr;

	addr = sgbuf->table[offset >> PAGE_SHIFT].addr;
	addr &= ~((dma_addr_t)PAGE_SIZE - 1);
	return addr + offset % PAGE_SIZE;
}

static struct page *snd_dma_sg_get_page(struct snd_dma_buffer *dmab,
					size_t offset)
{
	struct snd_sg_buf *sgbuf = dmab->private_data;
	unsigned int idx = offset >> PAGE_SHIFT;

	if (idx >= (unsigned int)sgbuf->pages)
		return NULL;
	return sgbuf->page_table[idx];
}

static unsigned int snd_dma_sg_get_chunk_size(struct snd_dma_buffer *dmab,
					      unsigned int ofs,
					      unsigned int size)
>>>>>>> upstream/android-13
{
	struct snd_sg_buf *sg = dmab->private_data;
	unsigned int start, end, pg;

	start = ofs >> PAGE_SHIFT;
	end = (ofs + size - 1) >> PAGE_SHIFT;
	/* check page continuity */
	pg = sg->table[start].addr >> PAGE_SHIFT;
	for (;;) {
		start++;
		if (start > end)
			break;
		pg++;
		if ((sg->table[start].addr >> PAGE_SHIFT) != pg)
			return (start << PAGE_SHIFT) - ofs;
	}
	/* ok, all on continuous pages */
	return size;
}
<<<<<<< HEAD
EXPORT_SYMBOL(snd_sgbuf_get_chunk_size);
=======

static int snd_dma_sg_mmap(struct snd_dma_buffer *dmab,
			   struct vm_area_struct *area)
{
	if (dmab->dev.type == SNDRV_DMA_TYPE_DEV_WC_SG)
		area->vm_page_prot = pgprot_writecombine(area->vm_page_prot);
	return -ENOENT; /* continue with the default mmap handler */
}

const struct snd_malloc_ops snd_dma_sg_ops = {
	.alloc = snd_dma_sg_alloc,
	.free = snd_dma_sg_free,
	.get_addr = snd_dma_sg_get_addr,
	.get_page = snd_dma_sg_get_page,
	.get_chunk_size = snd_dma_sg_get_chunk_size,
	.mmap = snd_dma_sg_mmap,
};
>>>>>>> upstream/android-13
