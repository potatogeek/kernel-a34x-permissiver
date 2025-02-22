/*
<<<<<<< HEAD
 * Copyright (C) 2012-2019 Samsung Electronics, Inc.
=======
 * Copyright (c) 2012 Samsung Electronics Co., Ltd All Rights Reserved
>>>>>>> upstream/android-13
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include <linux/err.h>
#include <linux/gfp.h>
#include <linux/kernel.h>
<<<<<<< HEAD
=======
#include <linux/limits.h>
>>>>>>> upstream/android-13
#include <linux/mm.h>
#include <linux/slab.h>
#include <linux/string.h>
#include <linux/vmalloc.h>
#include <asm/pgtable.h>

#include "tzdev_internal.h"
#include "core/iwio.h"
<<<<<<< HEAD
=======
#include "core/iwio_impl.h"
>>>>>>> upstream/android-13
#include "core/log.h"

struct iw_channel {
	struct list_head list;
	void *vaddr;
	struct page **pages;
	unsigned int num_pages;
<<<<<<< HEAD
};

=======
	void *impl_data;
};

static const struct tzdev_iwio_impl *impl;

>>>>>>> upstream/android-13
static LIST_HEAD(iw_channel_list);
static DEFINE_SPINLOCK(iw_channel_list_lock);

#ifdef CONFIG_TZDEV_SK_MULTICORE

static DEFINE_PER_CPU(struct tz_iwio_aux_channel *, aux_channel);

#define aux_channel_get(ch)		get_cpu_var(ch)
#define aux_channel_put(ch)		put_cpu_var(ch)
#define aux_channel_init(ch, cpu)	per_cpu(ch, cpu)

#else /* CONFIG_TZDEV_SK_MULTICORE */

static DEFINE_MUTEX(aux_channel_lock);
static struct tz_iwio_aux_channel *aux_channel[1];

static struct tz_iwio_aux_channel *aux_channel_get(struct tz_iwio_aux_channel *ch[])
{
	mutex_lock(&aux_channel_lock);
	return ch[0];
}

#define aux_channel_put(ch)		mutex_unlock(&aux_channel_lock)
#define aux_channel_init(ch, cpu)	ch[cpu]

#endif /* CONFIG_TZDEV_SK_MULTICORE */

static int tz_iwio_alloc_aux_channel(int cpu)
{
	struct tz_iwio_aux_channel *ch;
	struct page *page;
	int ret;

	page = alloc_page(GFP_KERNEL);
	if (!page) {
		log_error(tzdev_iwio, "Failed to allocate aux_channel[%d] page\n", cpu);
		return -ENOMEM;
	}

	ch = page_address(page);

<<<<<<< HEAD
	ret = tzdev_smc_connect_aux(page_to_pfn(page));
	if (ret) {
		log_error(tzdev_iwio, "Failed to connect aux_channel[%d] page, error=%d\n",
				cpu, ret);
		__free_page(page);
=======
	ret = impl->connect_aux_channel(page);
	if (ret) {
		log_error(tzdev_iwio, "Failed to connect aux_channel[%d] page, error=%d\n",
				cpu, ret);
		if (ret != -EADDRNOTAVAIL)
			__free_page(page);

>>>>>>> upstream/android-13
		return ret;
	}

	if (cpu_possible(cpu))
		aux_channel_init(aux_channel, cpu) = ch;

	log_debug(tzdev_iwio, "AUX Channel[%d] = 0x%pK\n", cpu, ch);

	return 0;
}

struct tz_iwio_aux_channel *tz_iwio_get_aux_channel(void)
{
	return aux_channel_get(aux_channel);
}

void tz_iwio_put_aux_channel(void)
{
	aux_channel_put(aux_channel);
}

void *tz_iwio_alloc_iw_channel(unsigned int mode, unsigned int num_pages,
	tz_iwio_callback_t pre_callback, tz_iwio_callback_cleanup_t pre_callback_cleanup,
	void *callback_data)
{
	void *buffer;
	struct page **pages;
<<<<<<< HEAD
	sk_pfn_t *pfns;
	struct tz_iwio_aux_channel *aux_ch;
	struct iw_channel *new_ch;
	unsigned int i, j;
	unsigned long offset, num_pfns;
	unsigned long pfns_in_buf = TZ_IWIO_AUX_BUF_SIZE / sizeof(sk_pfn_t);
=======
	struct iw_channel *new_ch;
	unsigned int i, j;
>>>>>>> upstream/android-13
	int ret;

	might_sleep();

<<<<<<< HEAD
	new_ch = kmalloc(sizeof(struct iw_channel), GFP_KERNEL);
=======
	new_ch = kzalloc(sizeof(struct iw_channel), GFP_KERNEL);
>>>>>>> upstream/android-13
	if (!new_ch) {
		log_error(tzdev_iwio, "IW channel structure allocation failed\n");
		return ERR_PTR(-ENOMEM);
	}

<<<<<<< HEAD
=======
	if (impl->data_size) {
		new_ch->impl_data = kzalloc(impl->data_size, GFP_KERNEL);
		if (!new_ch->impl_data) {
			ret = -ENOMEM;
			goto free_iw_channel_structure;
		}
	}

>>>>>>> upstream/android-13
	pages = kcalloc(num_pages, sizeof(struct page *), GFP_KERNEL);
	if (!pages) {
		log_error(tzdev_iwio, "IW channel pages buffer allocation failed\n");
		ret = -ENOMEM;
<<<<<<< HEAD
		goto free_iw_channel_structure;
	}

	pfns = kmalloc(num_pages * sizeof(sk_pfn_t), GFP_KERNEL);
	if (!pfns) {
		log_error(tzdev_iwio, "IW channel pfns buffer allocation failed\n");
		ret = -ENOMEM;
		goto free_pages_arr;
=======
		goto free_impl_data;
>>>>>>> upstream/android-13
	}

	/* Allocate non-contiguous buffer to reduce page allocator pressure */
	for (i = 0; i < num_pages; i++) {
		pages[i] = alloc_page(GFP_KERNEL);
		if (!pages[i]) {
			log_error(tzdev_iwio, "IW channel pages aloocation failed\n");
			ret = -ENOMEM;
<<<<<<< HEAD
			goto free_pfns_arr;
		}
		pfns[i] = page_to_pfn(pages[i]);
=======
			goto free_pages;
		}
>>>>>>> upstream/android-13
	}

	buffer = vmap(pages, num_pages, VM_MAP, PAGE_KERNEL);
	if (!buffer) {
		log_error(tzdev_iwio, "IW buffer map failed\n");
		ret = -EINVAL;
<<<<<<< HEAD
		goto free_pfns_arr;
=======
		goto free_pages;
>>>>>>> upstream/android-13
	}

	/* Call pre-callback */
	if (pre_callback) {
		ret = pre_callback(buffer, num_pages, callback_data);
		if (ret) {
			log_error(tzdev_iwio, "pre_callback failed, error=%d\n", ret);
			goto unmap_buffer;
		}
	}

<<<<<<< HEAD
	/* Push PFNs list into aux channel */
	aux_ch = tz_iwio_get_aux_channel();

	for (offset = 0; offset < num_pages; offset += pfns_in_buf) {
		num_pfns = min(pfns_in_buf, num_pages - offset);

		memcpy(aux_ch->buffer, pfns + offset,
				num_pfns * sizeof(sk_pfn_t));

		if ((ret = tzdev_smc_connect(mode, num_pfns))) {
			tz_iwio_put_aux_channel();
			log_error(tzdev_iwio, "IW buffer registration failed, error=%d\n", ret);
			goto pre_callback_cleanup;
		}
=======
	ret = impl->connect(mode, pages, num_pages, new_ch->impl_data);
	if (ret) {
		log_error(tzdev_iwio, "IW buffer registration failed, error=%d\n", ret);
		goto pre_callback_cleanup;
>>>>>>> upstream/android-13
	}

	new_ch->pages = pages;
	new_ch->num_pages = num_pages;
	new_ch->vaddr = buffer;

	log_debug(tzdev_iwio, "IWIO channel ch=%pK created.\n", new_ch);

	/* Add new channel to global list */
	spin_lock(&iw_channel_list_lock);
	list_add_tail(&new_ch->list, &iw_channel_list);
	spin_unlock(&iw_channel_list_lock);

<<<<<<< HEAD
	tz_iwio_put_aux_channel();

	kfree(pfns);

=======
>>>>>>> upstream/android-13
	return buffer;

pre_callback_cleanup:
	if (pre_callback_cleanup)
		pre_callback_cleanup(buffer, num_pages, callback_data);

unmap_buffer:
	vunmap(buffer);

<<<<<<< HEAD
free_pfns_arr:
	kfree(pfns);

	for (j = 0; j < i; j++)
		__free_page(pages[j]);

free_pages_arr:
	kfree(pages);

=======
free_pages:
	if (ret != -EADDRNOTAVAIL)
		for (j = 0; j < i; j++)
			__free_page(pages[j]);
	kfree(pages);

free_impl_data:
	kfree(new_ch->impl_data);

>>>>>>> upstream/android-13
free_iw_channel_structure:
	kfree(new_ch);

	return ERR_PTR(ret);
}

void tz_iwio_free_iw_channel(void *ch)
{
<<<<<<< HEAD
=======
	int ret;
>>>>>>> upstream/android-13
	struct iw_channel *iw;
	unsigned int i, found = 0;

	spin_lock(&iw_channel_list_lock);
	list_for_each_entry(iw, &iw_channel_list, list) {
		if (iw->vaddr == ch) {
			list_del(&iw->list);
			found = 1;
			break;
		}
	}
	spin_unlock(&iw_channel_list_lock);

	if (!found) {
		log_debug(tzdev_iwio, "IWIO channel ch=%pK not found.\n", ch);
		return;
	}

	log_debug(tzdev_iwio, "IWIO channel ch=%pK destroyed.\n", ch);

	vunmap(ch);

<<<<<<< HEAD
	for (i = 0; i < iw->num_pages; i++)
		__free_page(iw->pages[i]);

=======
	ret = impl->deinit(iw->impl_data);
	if (ret != -EADDRNOTAVAIL)
		for (i = 0; i < iw->num_pages; i++)
			__free_page(iw->pages[i]);

	kfree(iw->impl_data);
>>>>>>> upstream/android-13
	kfree(iw->pages);
	kfree(iw);
}

int tz_iwio_init(void)
{
	int ret;
	unsigned int i;

<<<<<<< HEAD
=======
	impl = tzdev_get_iwio_impl();

>>>>>>> upstream/android-13
	for (i = 0; i < NR_SW_CPU_IDS; ++i) {
		ret = tz_iwio_alloc_aux_channel(i);
		if (ret) {
			log_info(tzdev_iwio, "Failed to initialize AUX channel[%u], error=%d\n",
					i, ret);
			return ret;
		}
	}
	log_info(tzdev_iwio, "IWIO initialization done.\n");

	return 0;
}
