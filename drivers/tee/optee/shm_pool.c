<<<<<<< HEAD
/*
 * Copyright (c) 2015, Linaro Limited
 * Copyright (c) 2017, EPAM Systems
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
=======
// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright (c) 2015, Linaro Limited
 * Copyright (c) 2017, EPAM Systems
>>>>>>> upstream/android-13
 */
#include <linux/device.h>
#include <linux/dma-buf.h>
#include <linux/genalloc.h>
#include <linux/slab.h>
#include <linux/tee_drv.h>
#include "optee_private.h"
#include "optee_smc.h"
#include "shm_pool.h"

static int pool_op_alloc(struct tee_shm_pool_mgr *poolm,
			 struct tee_shm *shm, size_t size)
{
	unsigned int order = get_order(size);
	struct page *page;
<<<<<<< HEAD
=======
	int rc = 0;
>>>>>>> upstream/android-13

	page = alloc_pages(GFP_KERNEL | __GFP_ZERO, order);
	if (!page)
		return -ENOMEM;

	shm->kaddr = page_address(page);
	shm->paddr = page_to_phys(page);
	shm->size = PAGE_SIZE << order;

<<<<<<< HEAD
	return 0;
=======
	/*
	 * Shared memory private to the OP-TEE driver doesn't need
	 * to be registered with OP-TEE.
	 */
	if (!(shm->flags & TEE_SHM_PRIV)) {
		unsigned int nr_pages = 1 << order, i;
		struct page **pages;

		pages = kcalloc(nr_pages, sizeof(*pages), GFP_KERNEL);
		if (!pages) {
			rc = -ENOMEM;
			goto err;
		}

		for (i = 0; i < nr_pages; i++)
			pages[i] = page + i;

		shm->flags |= TEE_SHM_REGISTER;
		rc = optee_shm_register(shm->ctx, shm, pages, nr_pages,
					(unsigned long)shm->kaddr);
		kfree(pages);
		if (rc)
			goto err;
	}

	return 0;

err:
	__free_pages(page, order);
	return rc;
>>>>>>> upstream/android-13
}

static void pool_op_free(struct tee_shm_pool_mgr *poolm,
			 struct tee_shm *shm)
{
<<<<<<< HEAD
=======
	if (!(shm->flags & TEE_SHM_PRIV))
		optee_shm_unregister(shm->ctx, shm);

>>>>>>> upstream/android-13
	free_pages((unsigned long)shm->kaddr, get_order(shm->size));
	shm->kaddr = NULL;
}

static void pool_op_destroy_poolmgr(struct tee_shm_pool_mgr *poolm)
{
	kfree(poolm);
}

static const struct tee_shm_pool_mgr_ops pool_ops = {
	.alloc = pool_op_alloc,
	.free = pool_op_free,
	.destroy_poolmgr = pool_op_destroy_poolmgr,
};

/**
 * optee_shm_pool_alloc_pages() - create page-based allocator pool
 *
 * This pool is used when OP-TEE supports dymanic SHM. In this case
 * command buffers and such are allocated from kernel's own memory.
 */
struct tee_shm_pool_mgr *optee_shm_pool_alloc_pages(void)
{
	struct tee_shm_pool_mgr *mgr = kzalloc(sizeof(*mgr), GFP_KERNEL);

	if (!mgr)
		return ERR_PTR(-ENOMEM);

	mgr->ops = &pool_ops;

	return mgr;
}
