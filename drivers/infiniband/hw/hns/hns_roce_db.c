/* SPDX-License-Identifier: (GPL-2.0 OR BSD-2-Clause) */
/*
 * Copyright (c) 2017 Hisilicon Limited.
 * Copyright (c) 2007, 2008 Mellanox Technologies. All rights reserved.
 */

#include <linux/platform_device.h>
#include <rdma/ib_umem.h>
#include "hns_roce_device.h"

int hns_roce_db_map_user(struct hns_roce_ucontext *context, unsigned long virt,
			 struct hns_roce_db *db)
{
<<<<<<< HEAD
	struct hns_roce_user_db_page *page;
=======
	unsigned long page_addr = virt & PAGE_MASK;
	struct hns_roce_user_db_page *page;
	unsigned int offset;
>>>>>>> upstream/android-13
	int ret = 0;

	mutex_lock(&context->page_mutex);

	list_for_each_entry(page, &context->page_list, list)
<<<<<<< HEAD
		if (page->user_virt == (virt & PAGE_MASK))
=======
		if (page->user_virt == page_addr)
>>>>>>> upstream/android-13
			goto found;

	page = kmalloc(sizeof(*page), GFP_KERNEL);
	if (!page) {
		ret = -ENOMEM;
		goto out;
	}

	refcount_set(&page->refcount, 1);
<<<<<<< HEAD
	page->user_virt = (virt & PAGE_MASK);
	page->umem = ib_umem_get(&context->ibucontext, virt & PAGE_MASK,
				 PAGE_SIZE, 0, 0);
=======
	page->user_virt = page_addr;
	page->umem = ib_umem_get(context->ibucontext.device, page_addr,
				 PAGE_SIZE, 0);
>>>>>>> upstream/android-13
	if (IS_ERR(page->umem)) {
		ret = PTR_ERR(page->umem);
		kfree(page);
		goto out;
	}

	list_add(&page->list, &context->page_list);

found:
<<<<<<< HEAD
	db->dma = sg_dma_address(page->umem->sg_head.sgl) +
		  (virt & ~PAGE_MASK);
	page->umem->sg_head.sgl->offset = virt & ~PAGE_MASK;
	db->virt_addr = sg_virt(page->umem->sg_head.sgl);
=======
	offset = virt - page_addr;
	db->dma = sg_dma_address(page->umem->sgt_append.sgt.sgl) + offset;
	db->virt_addr = sg_virt(page->umem->sgt_append.sgt.sgl) + offset;
>>>>>>> upstream/android-13
	db->u.user_page = page;
	refcount_inc(&page->refcount);

out:
	mutex_unlock(&context->page_mutex);

	return ret;
}
<<<<<<< HEAD
EXPORT_SYMBOL(hns_roce_db_map_user);
=======
>>>>>>> upstream/android-13

void hns_roce_db_unmap_user(struct hns_roce_ucontext *context,
			    struct hns_roce_db *db)
{
	mutex_lock(&context->page_mutex);

	refcount_dec(&db->u.user_page->refcount);
	if (refcount_dec_if_one(&db->u.user_page->refcount)) {
		list_del(&db->u.user_page->list);
		ib_umem_release(db->u.user_page->umem);
		kfree(db->u.user_page);
	}

	mutex_unlock(&context->page_mutex);
}
<<<<<<< HEAD
EXPORT_SYMBOL(hns_roce_db_unmap_user);
=======
>>>>>>> upstream/android-13

static struct hns_roce_db_pgdir *hns_roce_alloc_db_pgdir(
					struct device *dma_device)
{
	struct hns_roce_db_pgdir *pgdir;

	pgdir = kzalloc(sizeof(*pgdir), GFP_KERNEL);
	if (!pgdir)
		return NULL;

<<<<<<< HEAD
	bitmap_fill(pgdir->order1, HNS_ROCE_DB_PER_PAGE / 2);
=======
	bitmap_fill(pgdir->order1,
		    HNS_ROCE_DB_PER_PAGE / HNS_ROCE_DB_TYPE_COUNT);
>>>>>>> upstream/android-13
	pgdir->bits[0] = pgdir->order0;
	pgdir->bits[1] = pgdir->order1;
	pgdir->page = dma_alloc_coherent(dma_device, PAGE_SIZE,
					 &pgdir->db_dma, GFP_KERNEL);
	if (!pgdir->page) {
		kfree(pgdir);
		return NULL;
	}

	return pgdir;
}

static int hns_roce_alloc_db_from_pgdir(struct hns_roce_db_pgdir *pgdir,
					struct hns_roce_db *db, int order)
{
<<<<<<< HEAD
	int o;
	int i;
=======
	unsigned long o;
	unsigned long i;
>>>>>>> upstream/android-13

	for (o = order; o <= 1; ++o) {
		i = find_first_bit(pgdir->bits[o], HNS_ROCE_DB_PER_PAGE >> o);
		if (i < HNS_ROCE_DB_PER_PAGE >> o)
			goto found;
	}

	return -ENOMEM;

found:
	clear_bit(i, pgdir->bits[o]);

	i <<= o;

	if (o > order)
		set_bit(i ^ 1, pgdir->bits[order]);

	db->u.pgdir	= pgdir;
	db->index	= i;
	db->db_record	= pgdir->page + db->index;
<<<<<<< HEAD
	db->dma		= pgdir->db_dma  + db->index * 4;
=======
	db->dma		= pgdir->db_dma  + db->index * HNS_ROCE_DB_UNIT_SIZE;
>>>>>>> upstream/android-13
	db->order	= order;

	return 0;
}

int hns_roce_alloc_db(struct hns_roce_dev *hr_dev, struct hns_roce_db *db,
		      int order)
{
	struct hns_roce_db_pgdir *pgdir;
	int ret = 0;

	mutex_lock(&hr_dev->pgdir_mutex);

	list_for_each_entry(pgdir, &hr_dev->pgdir_list, list)
		if (!hns_roce_alloc_db_from_pgdir(pgdir, db, order))
			goto out;

	pgdir = hns_roce_alloc_db_pgdir(hr_dev->dev);
	if (!pgdir) {
		ret = -ENOMEM;
		goto out;
	}

	list_add(&pgdir->list, &hr_dev->pgdir_list);

	/* This should never fail -- we just allocated an empty page: */
	WARN_ON(hns_roce_alloc_db_from_pgdir(pgdir, db, order));

out:
	mutex_unlock(&hr_dev->pgdir_mutex);

	return ret;
}
<<<<<<< HEAD
EXPORT_SYMBOL_GPL(hns_roce_alloc_db);

void hns_roce_free_db(struct hns_roce_dev *hr_dev, struct hns_roce_db *db)
{
	int o;
	int i;
=======

void hns_roce_free_db(struct hns_roce_dev *hr_dev, struct hns_roce_db *db)
{
	unsigned long o;
	unsigned long i;
>>>>>>> upstream/android-13

	mutex_lock(&hr_dev->pgdir_mutex);

	o = db->order;
	i = db->index;

	if (db->order == 0 && test_bit(i ^ 1, db->u.pgdir->order0)) {
		clear_bit(i ^ 1, db->u.pgdir->order0);
		++o;
	}

	i >>= o;
	set_bit(i, db->u.pgdir->bits[o]);

<<<<<<< HEAD
	if (bitmap_full(db->u.pgdir->order1, HNS_ROCE_DB_PER_PAGE / 2)) {
=======
	if (bitmap_full(db->u.pgdir->order1,
			HNS_ROCE_DB_PER_PAGE / HNS_ROCE_DB_TYPE_COUNT)) {
>>>>>>> upstream/android-13
		dma_free_coherent(hr_dev->dev, PAGE_SIZE, db->u.pgdir->page,
				  db->u.pgdir->db_dma);
		list_del(&db->u.pgdir->list);
		kfree(db->u.pgdir);
	}

	mutex_unlock(&hr_dev->pgdir_mutex);
}
<<<<<<< HEAD
EXPORT_SYMBOL_GPL(hns_roce_free_db);
=======
>>>>>>> upstream/android-13
