<<<<<<< HEAD
/*
 *  Digital Audio (PCM) abstract layer
 *  Copyright (c) by Jaroslav Kysela <perex@perex.cz>
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
// SPDX-License-Identifier: GPL-2.0-or-later
/*
 *  Digital Audio (PCM) abstract layer
 *  Copyright (c) by Jaroslav Kysela <perex@perex.cz>
>>>>>>> upstream/android-13
 */

#include <linux/io.h>
#include <linux/time.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/moduleparam.h>
#include <linux/vmalloc.h>
#include <linux/export.h>
#include <sound/core.h>
#include <sound/pcm.h>
#include <sound/info.h>
#include <sound/initval.h>
<<<<<<< HEAD
=======
#include "pcm_local.h"
>>>>>>> upstream/android-13

static int preallocate_dma = 1;
module_param(preallocate_dma, int, 0444);
MODULE_PARM_DESC(preallocate_dma, "Preallocate DMA memory when the PCM devices are initialized.");

static int maximum_substreams = 4;
module_param(maximum_substreams, int, 0444);
MODULE_PARM_DESC(maximum_substreams, "Maximum substreams with preallocated DMA memory.");

static const size_t snd_minimum_buffer = 16384;

<<<<<<< HEAD
=======
static unsigned long max_alloc_per_card = 32UL * 1024UL * 1024UL;
module_param(max_alloc_per_card, ulong, 0644);
MODULE_PARM_DESC(max_alloc_per_card, "Max total allocation bytes per card.");

static int do_alloc_pages(struct snd_card *card, int type, struct device *dev,
			  size_t size, struct snd_dma_buffer *dmab)
{
	int err;

	if (max_alloc_per_card &&
	    card->total_pcm_alloc_bytes + size > max_alloc_per_card)
		return -ENOMEM;

	err = snd_dma_alloc_pages(type, dev, size, dmab);
	if (!err) {
		mutex_lock(&card->memory_mutex);
		card->total_pcm_alloc_bytes += dmab->bytes;
		mutex_unlock(&card->memory_mutex);
	}
	return err;
}

static void do_free_pages(struct snd_card *card, struct snd_dma_buffer *dmab)
{
	if (!dmab->area)
		return;
	mutex_lock(&card->memory_mutex);
	WARN_ON(card->total_pcm_alloc_bytes < dmab->bytes);
	card->total_pcm_alloc_bytes -= dmab->bytes;
	mutex_unlock(&card->memory_mutex);
	snd_dma_free_pages(dmab);
	dmab->area = NULL;
}
>>>>>>> upstream/android-13

/*
 * try to allocate as the large pages as possible.
 * stores the resultant memory size in *res_size.
 *
 * the minimum size is snd_minimum_buffer.  it should be power of 2.
 */
<<<<<<< HEAD
static int preallocate_pcm_pages(struct snd_pcm_substream *substream, size_t size)
{
	struct snd_dma_buffer *dmab = &substream->dma_buffer;
=======
static int preallocate_pcm_pages(struct snd_pcm_substream *substream,
				 size_t size, bool no_fallback)
{
	struct snd_dma_buffer *dmab = &substream->dma_buffer;
	struct snd_card *card = substream->pcm->card;
>>>>>>> upstream/android-13
	size_t orig_size = size;
	int err;

	do {
<<<<<<< HEAD
		if ((err = snd_dma_alloc_pages(dmab->dev.type, dmab->dev.dev,
					       size, dmab)) < 0) {
			if (err != -ENOMEM)
				return err; /* fatal error */
		} else
			return 0;
=======
		err = do_alloc_pages(card, dmab->dev.type, dmab->dev.dev,
				     size, dmab);
		if (err != -ENOMEM)
			return err;
		if (no_fallback)
			break;
>>>>>>> upstream/android-13
		size >>= 1;
	} while (size >= snd_minimum_buffer);
	dmab->bytes = 0; /* tell error */
	pr_warn("ALSA pcmC%dD%d%c,%d:%s: cannot preallocate for size %zu\n",
		substream->pcm->card->number, substream->pcm->device,
		substream->stream ? 'c' : 'p', substream->number,
		substream->pcm->name, orig_size);
<<<<<<< HEAD
	return 0;
}

/*
 * release the preallocated buffer if not yet done.
 */
static void snd_pcm_lib_preallocate_dma_free(struct snd_pcm_substream *substream)
{
	if (substream->dma_buffer.area == NULL)
		return;
	snd_dma_free_pages(&substream->dma_buffer);
	substream->dma_buffer.area = NULL;
=======
	return -ENOMEM;
>>>>>>> upstream/android-13
}

/**
 * snd_pcm_lib_preallocate_free - release the preallocated buffer of the specified substream.
 * @substream: the pcm substream instance
 *
 * Releases the pre-allocated buffer of the given substream.
<<<<<<< HEAD
 *
 * Return: Zero if successful, or a negative error code on failure.
 */
int snd_pcm_lib_preallocate_free(struct snd_pcm_substream *substream)
{
	snd_pcm_lib_preallocate_dma_free(substream);
#ifdef CONFIG_SND_VERBOSE_PROCFS
	snd_info_free_entry(substream->proc_prealloc_max_entry);
	substream->proc_prealloc_max_entry = NULL;
	snd_info_free_entry(substream->proc_prealloc_entry);
	substream->proc_prealloc_entry = NULL;
#endif
	return 0;
=======
 */
void snd_pcm_lib_preallocate_free(struct snd_pcm_substream *substream)
{
	do_free_pages(substream->pcm->card, &substream->dma_buffer);
>>>>>>> upstream/android-13
}

/**
 * snd_pcm_lib_preallocate_free_for_all - release all pre-allocated buffers on the pcm
 * @pcm: the pcm instance
 *
 * Releases all the pre-allocated buffers on the given pcm.
<<<<<<< HEAD
 *
 * Return: Zero if successful, or a negative error code on failure.
 */
int snd_pcm_lib_preallocate_free_for_all(struct snd_pcm *pcm)
=======
 */
void snd_pcm_lib_preallocate_free_for_all(struct snd_pcm *pcm)
>>>>>>> upstream/android-13
{
	struct snd_pcm_substream *substream;
	int stream;

<<<<<<< HEAD
	for (stream = 0; stream < 2; stream++)
		for (substream = pcm->streams[stream].substream; substream; substream = substream->next)
			snd_pcm_lib_preallocate_free(substream);
	return 0;
=======
	for_each_pcm_substream(pcm, stream, substream)
		snd_pcm_lib_preallocate_free(substream);
>>>>>>> upstream/android-13
}
EXPORT_SYMBOL(snd_pcm_lib_preallocate_free_for_all);

#ifdef CONFIG_SND_VERBOSE_PROCFS
/*
 * read callback for prealloc proc file
 *
 * prints the current allocated size in kB.
 */
static void snd_pcm_lib_preallocate_proc_read(struct snd_info_entry *entry,
					      struct snd_info_buffer *buffer)
{
	struct snd_pcm_substream *substream = entry->private_data;
	snd_iprintf(buffer, "%lu\n", (unsigned long) substream->dma_buffer.bytes / 1024);
}

/*
 * read callback for prealloc_max proc file
 *
 * prints the maximum allowed size in kB.
 */
static void snd_pcm_lib_preallocate_max_proc_read(struct snd_info_entry *entry,
						  struct snd_info_buffer *buffer)
{
	struct snd_pcm_substream *substream = entry->private_data;
	snd_iprintf(buffer, "%lu\n", (unsigned long) substream->dma_max / 1024);
}

/*
 * write callback for prealloc proc file
 *
 * accepts the preallocation size in kB.
 */
static void snd_pcm_lib_preallocate_proc_write(struct snd_info_entry *entry,
					       struct snd_info_buffer *buffer)
{
	struct snd_pcm_substream *substream = entry->private_data;
<<<<<<< HEAD
=======
	struct snd_card *card = substream->pcm->card;
>>>>>>> upstream/android-13
	char line[64], str[64];
	size_t size;
	struct snd_dma_buffer new_dmab;

<<<<<<< HEAD
	if (substream->runtime) {
		buffer->error = -EBUSY;
		return;
=======
	mutex_lock(&substream->pcm->open_mutex);
	if (substream->runtime) {
		buffer->error = -EBUSY;
		goto unlock;
>>>>>>> upstream/android-13
	}
	if (!snd_info_get_line(buffer, line, sizeof(line))) {
		snd_info_get_str(str, line, sizeof(str));
		size = simple_strtoul(str, NULL, 10) * 1024;
		if ((size != 0 && size < 8192) || size > substream->dma_max) {
			buffer->error = -EINVAL;
<<<<<<< HEAD
			return;
		}
		if (substream->dma_buffer.bytes == size)
			return;
		memset(&new_dmab, 0, sizeof(new_dmab));
		new_dmab.dev = substream->dma_buffer.dev;
		if (size > 0) {
			if (snd_dma_alloc_pages(substream->dma_buffer.dev.type,
						substream->dma_buffer.dev.dev,
						size, &new_dmab) < 0) {
				buffer->error = -ENOMEM;
				return;
=======
			goto unlock;
		}
		if (substream->dma_buffer.bytes == size)
			goto unlock;
		memset(&new_dmab, 0, sizeof(new_dmab));
		new_dmab.dev = substream->dma_buffer.dev;
		if (size > 0) {
			if (do_alloc_pages(card,
					   substream->dma_buffer.dev.type,
					   substream->dma_buffer.dev.dev,
					   size, &new_dmab) < 0) {
				buffer->error = -ENOMEM;
				pr_debug("ALSA pcmC%dD%d%c,%d:%s: cannot preallocate for size %zu\n",
					 substream->pcm->card->number, substream->pcm->device,
					 substream->stream ? 'c' : 'p', substream->number,
					 substream->pcm->name, size);
				goto unlock;
>>>>>>> upstream/android-13
			}
			substream->buffer_bytes_max = size;
		} else {
			substream->buffer_bytes_max = UINT_MAX;
		}
		if (substream->dma_buffer.area)
<<<<<<< HEAD
			snd_dma_free_pages(&substream->dma_buffer);
=======
			do_free_pages(card, &substream->dma_buffer);
>>>>>>> upstream/android-13
		substream->dma_buffer = new_dmab;
	} else {
		buffer->error = -EINVAL;
	}
<<<<<<< HEAD
=======
 unlock:
	mutex_unlock(&substream->pcm->open_mutex);
>>>>>>> upstream/android-13
}

static inline void preallocate_info_init(struct snd_pcm_substream *substream)
{
	struct snd_info_entry *entry;

<<<<<<< HEAD
	if ((entry = snd_info_create_card_entry(substream->pcm->card, "prealloc", substream->proc_root)) != NULL) {
		entry->c.text.read = snd_pcm_lib_preallocate_proc_read;
		entry->c.text.write = snd_pcm_lib_preallocate_proc_write;
		entry->mode |= 0200;
		entry->private_data = substream;
		if (snd_info_register(entry) < 0) {
			snd_info_free_entry(entry);
			entry = NULL;
		}
	}
	substream->proc_prealloc_entry = entry;
	if ((entry = snd_info_create_card_entry(substream->pcm->card, "prealloc_max", substream->proc_root)) != NULL) {
		entry->c.text.read = snd_pcm_lib_preallocate_max_proc_read;
		entry->private_data = substream;
		if (snd_info_register(entry) < 0) {
			snd_info_free_entry(entry);
			entry = NULL;
		}
	}
	substream->proc_prealloc_max_entry = entry;
}

#else /* !CONFIG_SND_VERBOSE_PROCFS */
#define preallocate_info_init(s)
=======
	entry = snd_info_create_card_entry(substream->pcm->card, "prealloc",
					   substream->proc_root);
	if (entry) {
		snd_info_set_text_ops(entry, substream,
				      snd_pcm_lib_preallocate_proc_read);
		entry->c.text.write = snd_pcm_lib_preallocate_proc_write;
		entry->mode |= 0200;
	}
	entry = snd_info_create_card_entry(substream->pcm->card, "prealloc_max",
					   substream->proc_root);
	if (entry)
		snd_info_set_text_ops(entry, substream,
				      snd_pcm_lib_preallocate_max_proc_read);
}

#else /* !CONFIG_SND_VERBOSE_PROCFS */
static inline void preallocate_info_init(struct snd_pcm_substream *substream)
{
}
>>>>>>> upstream/android-13
#endif /* CONFIG_SND_VERBOSE_PROCFS */

/*
 * pre-allocate the buffer and create a proc file for the substream
 */
<<<<<<< HEAD
static int snd_pcm_lib_preallocate_pages1(struct snd_pcm_substream *substream,
					  size_t size, size_t max)
{

	if (size > 0 && preallocate_dma && substream->number < maximum_substreams)
		preallocate_pcm_pages(substream, size);
=======
static int preallocate_pages(struct snd_pcm_substream *substream,
			      int type, struct device *data,
			      size_t size, size_t max, bool managed)
{
	int err;

	if (snd_BUG_ON(substream->dma_buffer.dev.type))
		return -EINVAL;

	substream->dma_buffer.dev.type = type;
	substream->dma_buffer.dev.dev = data;

	if (size > 0) {
		if (!max) {
			/* no fallback, only also inform -ENOMEM */
			err = preallocate_pcm_pages(substream, size, true);
			if (err < 0)
				return err;
		} else if (preallocate_dma &&
			   substream->number < maximum_substreams) {
			err = preallocate_pcm_pages(substream, size, false);
			if (err < 0 && err != -ENOMEM)
				return err;
		}
	}
>>>>>>> upstream/android-13

	if (substream->dma_buffer.bytes > 0)
		substream->buffer_bytes_max = substream->dma_buffer.bytes;
	substream->dma_max = max;
<<<<<<< HEAD
	preallocate_info_init(substream);
	return 0;
}

=======
	if (max > 0)
		preallocate_info_init(substream);
	if (managed)
		substream->managed_buffer_alloc = 1;
	return 0;
}

static int preallocate_pages_for_all(struct snd_pcm *pcm, int type,
				      void *data, size_t size, size_t max,
				      bool managed)
{
	struct snd_pcm_substream *substream;
	int stream, err;

	for_each_pcm_substream(pcm, stream, substream) {
		err = preallocate_pages(substream, type, data, size, max, managed);
		if (err < 0)
			return err;
	}
	return 0;
}
>>>>>>> upstream/android-13

/**
 * snd_pcm_lib_preallocate_pages - pre-allocation for the given DMA type
 * @substream: the pcm substream instance
 * @type: DMA type (SNDRV_DMA_TYPE_*)
 * @data: DMA type dependent data
 * @size: the requested pre-allocation size in bytes
 * @max: the max. allowed pre-allocation size
 *
 * Do pre-allocation for the given DMA buffer type.
<<<<<<< HEAD
 *
 * Return: Zero if successful, or a negative error code on failure.
 */
int snd_pcm_lib_preallocate_pages(struct snd_pcm_substream *substream,
				  int type, struct device *data,
				  size_t size, size_t max)
{
	substream->dma_buffer.dev.type = type;
	substream->dma_buffer.dev.dev = data;
	return snd_pcm_lib_preallocate_pages1(substream, size, max);
=======
 */
void snd_pcm_lib_preallocate_pages(struct snd_pcm_substream *substream,
				  int type, struct device *data,
				  size_t size, size_t max)
{
	preallocate_pages(substream, type, data, size, max, false);
>>>>>>> upstream/android-13
}
EXPORT_SYMBOL(snd_pcm_lib_preallocate_pages);

/**
 * snd_pcm_lib_preallocate_pages_for_all - pre-allocation for continuous memory type (all substreams)
 * @pcm: the pcm instance
 * @type: DMA type (SNDRV_DMA_TYPE_*)
 * @data: DMA type dependent data
 * @size: the requested pre-allocation size in bytes
 * @max: the max. allowed pre-allocation size
 *
 * Do pre-allocation to all substreams of the given pcm for the
 * specified DMA type.
<<<<<<< HEAD
 *
 * Return: Zero if successful, or a negative error code on failure.
 */
int snd_pcm_lib_preallocate_pages_for_all(struct snd_pcm *pcm,
					  int type, void *data,
					  size_t size, size_t max)
{
	struct snd_pcm_substream *substream;
	int stream, err;

	for (stream = 0; stream < 2; stream++)
		for (substream = pcm->streams[stream].substream; substream; substream = substream->next)
			if ((err = snd_pcm_lib_preallocate_pages(substream, type, data, size, max)) < 0)
				return err;
	return 0;
}
EXPORT_SYMBOL(snd_pcm_lib_preallocate_pages_for_all);

#ifdef CONFIG_SND_DMA_SGBUF
/**
 * snd_pcm_sgbuf_ops_page - get the page struct at the given offset
 * @substream: the pcm substream instance
 * @offset: the buffer offset
 *
 * Used as the page callback of PCM ops.
 *
 * Return: The page struct at the given buffer offset. %NULL on failure.
 */
struct page *snd_pcm_sgbuf_ops_page(struct snd_pcm_substream *substream, unsigned long offset)
{
	struct snd_sg_buf *sgbuf = snd_pcm_substream_sgbuf(substream);

	unsigned int idx = offset >> PAGE_SHIFT;
	if (idx >= (unsigned int)sgbuf->pages)
		return NULL;
	return sgbuf->page_table[idx];
}
EXPORT_SYMBOL(snd_pcm_sgbuf_ops_page);
#endif /* CONFIG_SND_DMA_SGBUF */
=======
 */
void snd_pcm_lib_preallocate_pages_for_all(struct snd_pcm *pcm,
					  int type, void *data,
					  size_t size, size_t max)
{
	preallocate_pages_for_all(pcm, type, data, size, max, false);
}
EXPORT_SYMBOL(snd_pcm_lib_preallocate_pages_for_all);

/**
 * snd_pcm_set_managed_buffer - set up buffer management for a substream
 * @substream: the pcm substream instance
 * @type: DMA type (SNDRV_DMA_TYPE_*)
 * @data: DMA type dependent data
 * @size: the requested pre-allocation size in bytes
 * @max: the max. allowed pre-allocation size
 *
 * Do pre-allocation for the given DMA buffer type, and set the managed
 * buffer allocation mode to the given substream.
 * In this mode, PCM core will allocate a buffer automatically before PCM
 * hw_params ops call, and release the buffer after PCM hw_free ops call
 * as well, so that the driver doesn't need to invoke the allocation and
 * the release explicitly in its callback.
 * When a buffer is actually allocated before the PCM hw_params call, it
 * turns on the runtime buffer_changed flag for drivers changing their h/w
 * parameters accordingly.
 *
 * When @size is non-zero and @max is zero, this tries to allocate for only
 * the exact buffer size without fallback, and may return -ENOMEM.
 * Otherwise, the function tries to allocate smaller chunks if the allocation
 * fails.  This is the behavior of snd_pcm_set_fixed_buffer().
 *
 * When both @size and @max are zero, the function only sets up the buffer
 * for later dynamic allocations. It's used typically for buffers with
 * SNDRV_DMA_TYPE_VMALLOC type.
 *
 * Upon successful buffer allocation and setup, the function returns 0.
 */
int snd_pcm_set_managed_buffer(struct snd_pcm_substream *substream, int type,
				struct device *data, size_t size, size_t max)
{
	return preallocate_pages(substream, type, data, size, max, true);
}
EXPORT_SYMBOL(snd_pcm_set_managed_buffer);

/**
 * snd_pcm_set_managed_buffer_all - set up buffer management for all substreams
 *	for all substreams
 * @pcm: the pcm instance
 * @type: DMA type (SNDRV_DMA_TYPE_*)
 * @data: DMA type dependent data
 * @size: the requested pre-allocation size in bytes
 * @max: the max. allowed pre-allocation size
 *
 * Do pre-allocation to all substreams of the given pcm for the specified DMA
 * type and size, and set the managed_buffer_alloc flag to each substream.
 */
int snd_pcm_set_managed_buffer_all(struct snd_pcm *pcm, int type,
				   struct device *data,
				   size_t size, size_t max)
{
	return preallocate_pages_for_all(pcm, type, data, size, max, true);
}
EXPORT_SYMBOL(snd_pcm_set_managed_buffer_all);
>>>>>>> upstream/android-13

/**
 * snd_pcm_lib_malloc_pages - allocate the DMA buffer
 * @substream: the substream to allocate the DMA buffer to
 * @size: the requested buffer size in bytes
 *
 * Allocates the DMA buffer on the BUS type given earlier to
 * snd_pcm_lib_preallocate_xxx_pages().
 *
 * Return: 1 if the buffer is changed, 0 if not changed, or a negative
 * code on failure.
 */
int snd_pcm_lib_malloc_pages(struct snd_pcm_substream *substream, size_t size)
{
<<<<<<< HEAD
=======
	struct snd_card *card;
>>>>>>> upstream/android-13
	struct snd_pcm_runtime *runtime;
	struct snd_dma_buffer *dmab = NULL;

	if (PCM_RUNTIME_CHECK(substream))
		return -EINVAL;
	if (snd_BUG_ON(substream->dma_buffer.dev.type ==
		       SNDRV_DMA_TYPE_UNKNOWN))
		return -EINVAL;
	runtime = substream->runtime;
<<<<<<< HEAD
=======
	card = substream->pcm->card;
>>>>>>> upstream/android-13

	if (runtime->dma_buffer_p) {
		/* perphaps, we might free the large DMA memory region
		   to save some space here, but the actual solution
		   costs us less time */
		if (runtime->dma_buffer_p->bytes >= size) {
			runtime->dma_bytes = size;
			return 0;	/* ok, do not change */
		}
		snd_pcm_lib_free_pages(substream);
	}
	if (substream->dma_buffer.area != NULL &&
	    substream->dma_buffer.bytes >= size) {
		dmab = &substream->dma_buffer; /* use the pre-allocated buffer */
	} else {
<<<<<<< HEAD
=======
		/* dma_max=0 means the fixed size preallocation */
		if (substream->dma_buffer.area && !substream->dma_max)
			return -ENOMEM;
>>>>>>> upstream/android-13
		dmab = kzalloc(sizeof(*dmab), GFP_KERNEL);
		if (! dmab)
			return -ENOMEM;
		dmab->dev = substream->dma_buffer.dev;
<<<<<<< HEAD
		if (snd_dma_alloc_pages(substream->dma_buffer.dev.type,
					substream->dma_buffer.dev.dev,
					size, dmab) < 0) {
			kfree(dmab);
=======
		if (do_alloc_pages(card,
				   substream->dma_buffer.dev.type,
				   substream->dma_buffer.dev.dev,
				   size, dmab) < 0) {
			kfree(dmab);
			pr_debug("ALSA pcmC%dD%d%c,%d:%s: cannot preallocate for size %zu\n",
				 substream->pcm->card->number, substream->pcm->device,
				 substream->stream ? 'c' : 'p', substream->number,
				 substream->pcm->name, size);
>>>>>>> upstream/android-13
			return -ENOMEM;
		}
	}
	snd_pcm_set_runtime_buffer(substream, dmab);
	runtime->dma_bytes = size;
	return 1;			/* area was changed */
}
EXPORT_SYMBOL(snd_pcm_lib_malloc_pages);

/**
 * snd_pcm_lib_free_pages - release the allocated DMA buffer.
 * @substream: the substream to release the DMA buffer
 *
 * Releases the DMA buffer allocated via snd_pcm_lib_malloc_pages().
 *
 * Return: Zero if successful, or a negative error code on failure.
 */
int snd_pcm_lib_free_pages(struct snd_pcm_substream *substream)
{
<<<<<<< HEAD
=======
	struct snd_card *card = substream->pcm->card;
>>>>>>> upstream/android-13
	struct snd_pcm_runtime *runtime;

	if (PCM_RUNTIME_CHECK(substream))
		return -EINVAL;
	runtime = substream->runtime;
	if (runtime->dma_area == NULL)
		return 0;
	if (runtime->dma_buffer_p != &substream->dma_buffer) {
		/* it's a newly allocated buffer.  release it now. */
<<<<<<< HEAD
		snd_dma_free_pages(runtime->dma_buffer_p);
=======
		do_free_pages(card, runtime->dma_buffer_p);
>>>>>>> upstream/android-13
		kfree(runtime->dma_buffer_p);
	}
	snd_pcm_set_runtime_buffer(substream, NULL);
	return 0;
}
EXPORT_SYMBOL(snd_pcm_lib_free_pages);

int _snd_pcm_lib_alloc_vmalloc_buffer(struct snd_pcm_substream *substream,
				      size_t size, gfp_t gfp_flags)
{
	struct snd_pcm_runtime *runtime;

	if (PCM_RUNTIME_CHECK(substream))
		return -EINVAL;
	runtime = substream->runtime;
	if (runtime->dma_area) {
		if (runtime->dma_bytes >= size)
			return 0; /* already large enough */
		vfree(runtime->dma_area);
	}
<<<<<<< HEAD
	runtime->dma_area = __vmalloc(size, gfp_flags, PAGE_KERNEL);
=======
	runtime->dma_area = __vmalloc(size, gfp_flags);
>>>>>>> upstream/android-13
	if (!runtime->dma_area)
		return -ENOMEM;
	runtime->dma_bytes = size;
	return 1;
}
EXPORT_SYMBOL(_snd_pcm_lib_alloc_vmalloc_buffer);

/**
 * snd_pcm_lib_free_vmalloc_buffer - free vmalloc buffer
 * @substream: the substream with a buffer allocated by
 *	snd_pcm_lib_alloc_vmalloc_buffer()
 *
 * Return: Zero if successful, or a negative error code on failure.
 */
int snd_pcm_lib_free_vmalloc_buffer(struct snd_pcm_substream *substream)
{
	struct snd_pcm_runtime *runtime;

	if (PCM_RUNTIME_CHECK(substream))
		return -EINVAL;
	runtime = substream->runtime;
	vfree(runtime->dma_area);
	runtime->dma_area = NULL;
	return 0;
}
EXPORT_SYMBOL(snd_pcm_lib_free_vmalloc_buffer);

/**
 * snd_pcm_lib_get_vmalloc_page - map vmalloc buffer offset to page struct
 * @substream: the substream with a buffer allocated by
 *	snd_pcm_lib_alloc_vmalloc_buffer()
 * @offset: offset in the buffer
 *
 * This function is to be used as the page callback in the PCM ops.
 *
 * Return: The page struct, or %NULL on failure.
 */
struct page *snd_pcm_lib_get_vmalloc_page(struct snd_pcm_substream *substream,
					  unsigned long offset)
{
	return vmalloc_to_page(substream->runtime->dma_area + offset);
}
EXPORT_SYMBOL(snd_pcm_lib_get_vmalloc_page);
