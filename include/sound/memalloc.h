<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-or-later */
>>>>>>> upstream/android-13
/*
 *  Copyright (c) by Jaroslav Kysela <perex@perex.cz>
 *                   Takashi Iwai <tiwai@suse.de>
 * 
 *  Generic memory allocators
<<<<<<< HEAD
 *
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

#ifndef __SOUND_MEMALLOC_H
#define __SOUND_MEMALLOC_H

#include <asm/page.h>

struct device;
<<<<<<< HEAD
=======
struct vm_area_struct;
>>>>>>> upstream/android-13

/*
 * buffer device info
 */
struct snd_dma_device {
	int type;			/* SNDRV_DMA_TYPE_XXX */
	struct device *dev;		/* generic device */
};

<<<<<<< HEAD
#define snd_dma_pci_data(pci)	(&(pci)->dev)
#define snd_dma_isa_data()	NULL
=======
>>>>>>> upstream/android-13
#define snd_dma_continuous_data(x)	((struct device *)(__force unsigned long)(x))


/*
 * buffer types
 */
#define SNDRV_DMA_TYPE_UNKNOWN		0	/* not defined */
#define SNDRV_DMA_TYPE_CONTINUOUS	1	/* continuous no-DMA memory */
#define SNDRV_DMA_TYPE_DEV		2	/* generic device continuous */
<<<<<<< HEAD
#ifdef CONFIG_SND_DMA_SGBUF
#define SNDRV_DMA_TYPE_DEV_SG		3	/* generic device SG-buffer */
#else
#define SNDRV_DMA_TYPE_DEV_SG	SNDRV_DMA_TYPE_DEV /* no SG-buf support */
=======
#define SNDRV_DMA_TYPE_DEV_WC		5	/* continuous write-combined */
#ifdef CONFIG_SND_DMA_SGBUF
#define SNDRV_DMA_TYPE_DEV_SG		3	/* generic device SG-buffer */
#define SNDRV_DMA_TYPE_DEV_WC_SG	6	/* SG write-combined */
#else
#define SNDRV_DMA_TYPE_DEV_SG	SNDRV_DMA_TYPE_DEV /* no SG-buf support */
#define SNDRV_DMA_TYPE_DEV_WC_SG	SNDRV_DMA_TYPE_DEV_WC
>>>>>>> upstream/android-13
#endif
#ifdef CONFIG_GENERIC_ALLOCATOR
#define SNDRV_DMA_TYPE_DEV_IRAM		4	/* generic device iram-buffer */
#else
#define SNDRV_DMA_TYPE_DEV_IRAM	SNDRV_DMA_TYPE_DEV
#endif
<<<<<<< HEAD
=======
#define SNDRV_DMA_TYPE_VMALLOC		7	/* vmalloc'ed buffer */
>>>>>>> upstream/android-13

/*
 * info for buffer allocation
 */
struct snd_dma_buffer {
	struct snd_dma_device dev;	/* device type */
	unsigned char *area;	/* virtual pointer */
	dma_addr_t addr;	/* physical address */
	size_t bytes;		/* buffer size in bytes */
	void *private_data;	/* private for allocator; don't touch */
};

/*
 * return the pages matching with the given byte size
 */
static inline unsigned int snd_sgbuf_aligned_pages(size_t size)
{
	return (size + PAGE_SIZE - 1) >> PAGE_SHIFT;
}

<<<<<<< HEAD
#ifdef CONFIG_SND_DMA_SGBUF
/*
 * Scatter-Gather generic device pages
 */
void *snd_malloc_sgbuf_pages(struct device *device,
			     size_t size, struct snd_dma_buffer *dmab,
			     size_t *res_size);
int snd_free_sgbuf_pages(struct snd_dma_buffer *dmab);

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

/*
 * return the physical address at the corresponding offset
 */
static inline dma_addr_t snd_sgbuf_get_addr(struct snd_dma_buffer *dmab,
					   size_t offset)
{
	struct snd_sg_buf *sgbuf = dmab->private_data;
	dma_addr_t addr = sgbuf->table[offset >> PAGE_SHIFT].addr;
	addr &= ~((dma_addr_t)PAGE_SIZE - 1);
	return addr + offset % PAGE_SIZE;
}

/*
 * return the virtual address at the corresponding offset
 */
static inline void *snd_sgbuf_get_ptr(struct snd_dma_buffer *dmab,
				     size_t offset)
{
	struct snd_sg_buf *sgbuf = dmab->private_data;
	return sgbuf->table[offset >> PAGE_SHIFT].buf + offset % PAGE_SIZE;
}

unsigned int snd_sgbuf_get_chunk_size(struct snd_dma_buffer *dmab,
				      unsigned int ofs, unsigned int size);
#else
/* non-SG versions */
static inline dma_addr_t snd_sgbuf_get_addr(struct snd_dma_buffer *dmab,
					    size_t offset)
{
	return dmab->addr + offset;
}

static inline void *snd_sgbuf_get_ptr(struct snd_dma_buffer *dmab,
				      size_t offset)
{
	return dmab->area + offset;
}

#define snd_sgbuf_get_chunk_size(dmab, ofs, size)	(size)

#endif /* CONFIG_SND_DMA_SGBUF */

=======
>>>>>>> upstream/android-13
/* allocate/release a buffer */
int snd_dma_alloc_pages(int type, struct device *dev, size_t size,
			struct snd_dma_buffer *dmab);
int snd_dma_alloc_pages_fallback(int type, struct device *dev, size_t size,
                                 struct snd_dma_buffer *dmab);
void snd_dma_free_pages(struct snd_dma_buffer *dmab);
<<<<<<< HEAD

/* basic memory allocation functions */
void *snd_malloc_pages(size_t size, gfp_t gfp_flags);
void snd_free_pages(void *ptr, size_t size);
=======
int snd_dma_buffer_mmap(struct snd_dma_buffer *dmab,
			struct vm_area_struct *area);

dma_addr_t snd_sgbuf_get_addr(struct snd_dma_buffer *dmab, size_t offset);
struct page *snd_sgbuf_get_page(struct snd_dma_buffer *dmab, size_t offset);
unsigned int snd_sgbuf_get_chunk_size(struct snd_dma_buffer *dmab,
				      unsigned int ofs, unsigned int size);

/* device-managed memory allocator */
struct snd_dma_buffer *snd_devm_alloc_pages(struct device *dev, int type,
					    size_t size);
>>>>>>> upstream/android-13

#endif /* __SOUND_MEMALLOC_H */

