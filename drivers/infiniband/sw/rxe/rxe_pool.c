<<<<<<< HEAD
/*
 * Copyright (c) 2016 Mellanox Technologies Ltd. All rights reserved.
 * Copyright (c) 2015 System Fabric Works, Inc. All rights reserved.
 *
 * This software is available to you under a choice of one of two
 * licenses.  You may choose to be licensed under the terms of the GNU
 * General Public License (GPL) Version 2, available from the file
 * COPYING in the main directory of this source tree, or the
 * OpenIB.org BSD license below:
 *
 *	   Redistribution and use in source and binary forms, with or
 *	   without modification, are permitted provided that the following
 *	   conditions are met:
 *
 *		- Redistributions of source code must retain the above
 *		  copyright notice, this list of conditions and the following
 *		  disclaimer.
 *
 *		- Redistributions in binary form must reproduce the above
 *		  copyright notice, this list of conditions and the following
 *		  disclaimer in the documentation and/or other materials
 *		  provided with the distribution.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
=======
// SPDX-License-Identifier: GPL-2.0 OR Linux-OpenIB
/*
 * Copyright (c) 2016 Mellanox Technologies Ltd. All rights reserved.
 * Copyright (c) 2015 System Fabric Works, Inc. All rights reserved.
>>>>>>> upstream/android-13
 */

#include "rxe.h"
#include "rxe_loc.h"

/* info about object pools
<<<<<<< HEAD
 * note that mr and mw share a single index space
 * so that one can map an lkey to the correct type of object
=======
>>>>>>> upstream/android-13
 */
struct rxe_type_info rxe_type_info[RXE_NUM_TYPES] = {
	[RXE_TYPE_UC] = {
		.name		= "rxe-uc",
		.size		= sizeof(struct rxe_ucontext),
<<<<<<< HEAD
=======
		.elem_offset	= offsetof(struct rxe_ucontext, pelem),
		.flags          = RXE_POOL_NO_ALLOC,
>>>>>>> upstream/android-13
	},
	[RXE_TYPE_PD] = {
		.name		= "rxe-pd",
		.size		= sizeof(struct rxe_pd),
<<<<<<< HEAD
=======
		.elem_offset	= offsetof(struct rxe_pd, pelem),
		.flags		= RXE_POOL_NO_ALLOC,
>>>>>>> upstream/android-13
	},
	[RXE_TYPE_AH] = {
		.name		= "rxe-ah",
		.size		= sizeof(struct rxe_ah),
<<<<<<< HEAD
		.flags		= RXE_POOL_ATOMIC,
=======
		.elem_offset	= offsetof(struct rxe_ah, pelem),
		.flags		= RXE_POOL_NO_ALLOC,
>>>>>>> upstream/android-13
	},
	[RXE_TYPE_SRQ] = {
		.name		= "rxe-srq",
		.size		= sizeof(struct rxe_srq),
<<<<<<< HEAD
		.flags		= RXE_POOL_INDEX,
=======
		.elem_offset	= offsetof(struct rxe_srq, pelem),
		.flags		= RXE_POOL_INDEX | RXE_POOL_NO_ALLOC,
>>>>>>> upstream/android-13
		.min_index	= RXE_MIN_SRQ_INDEX,
		.max_index	= RXE_MAX_SRQ_INDEX,
	},
	[RXE_TYPE_QP] = {
		.name		= "rxe-qp",
		.size		= sizeof(struct rxe_qp),
<<<<<<< HEAD
		.cleanup	= rxe_qp_cleanup,
		.flags		= RXE_POOL_INDEX,
=======
		.elem_offset	= offsetof(struct rxe_qp, pelem),
		.cleanup	= rxe_qp_cleanup,
		.flags		= RXE_POOL_INDEX | RXE_POOL_NO_ALLOC,
>>>>>>> upstream/android-13
		.min_index	= RXE_MIN_QP_INDEX,
		.max_index	= RXE_MAX_QP_INDEX,
	},
	[RXE_TYPE_CQ] = {
		.name		= "rxe-cq",
		.size		= sizeof(struct rxe_cq),
<<<<<<< HEAD
=======
		.elem_offset	= offsetof(struct rxe_cq, pelem),
		.flags          = RXE_POOL_NO_ALLOC,
>>>>>>> upstream/android-13
		.cleanup	= rxe_cq_cleanup,
	},
	[RXE_TYPE_MR] = {
		.name		= "rxe-mr",
<<<<<<< HEAD
		.size		= sizeof(struct rxe_mem),
		.cleanup	= rxe_mem_cleanup,
=======
		.size		= sizeof(struct rxe_mr),
		.elem_offset	= offsetof(struct rxe_mr, pelem),
		.cleanup	= rxe_mr_cleanup,
>>>>>>> upstream/android-13
		.flags		= RXE_POOL_INDEX,
		.max_index	= RXE_MAX_MR_INDEX,
		.min_index	= RXE_MIN_MR_INDEX,
	},
	[RXE_TYPE_MW] = {
		.name		= "rxe-mw",
<<<<<<< HEAD
		.size		= sizeof(struct rxe_mem),
		.flags		= RXE_POOL_INDEX,
=======
		.size		= sizeof(struct rxe_mw),
		.elem_offset	= offsetof(struct rxe_mw, pelem),
		.cleanup	= rxe_mw_cleanup,
		.flags		= RXE_POOL_INDEX | RXE_POOL_NO_ALLOC,
>>>>>>> upstream/android-13
		.max_index	= RXE_MAX_MW_INDEX,
		.min_index	= RXE_MIN_MW_INDEX,
	},
	[RXE_TYPE_MC_GRP] = {
		.name		= "rxe-mc_grp",
		.size		= sizeof(struct rxe_mc_grp),
<<<<<<< HEAD
=======
		.elem_offset	= offsetof(struct rxe_mc_grp, pelem),
>>>>>>> upstream/android-13
		.cleanup	= rxe_mc_cleanup,
		.flags		= RXE_POOL_KEY,
		.key_offset	= offsetof(struct rxe_mc_grp, mgid),
		.key_size	= sizeof(union ib_gid),
	},
	[RXE_TYPE_MC_ELEM] = {
		.name		= "rxe-mc_elem",
		.size		= sizeof(struct rxe_mc_elem),
<<<<<<< HEAD
		.flags		= RXE_POOL_ATOMIC,
=======
		.elem_offset	= offsetof(struct rxe_mc_elem, pelem),
>>>>>>> upstream/android-13
	},
};

static inline const char *pool_name(struct rxe_pool *pool)
{
	return rxe_type_info[pool->type].name;
}

<<<<<<< HEAD
static inline struct kmem_cache *pool_cache(struct rxe_pool *pool)
{
	return rxe_type_info[pool->type].cache;
}

static void rxe_cache_clean(size_t cnt)
{
	int i;
	struct rxe_type_info *type;

	for (i = 0; i < cnt; i++) {
		type = &rxe_type_info[i];
		kmem_cache_destroy(type->cache);
		type->cache = NULL;
	}
}

int rxe_cache_init(void)
{
	int err;
	int i;
	size_t size;
	struct rxe_type_info *type;

	for (i = 0; i < RXE_NUM_TYPES; i++) {
		type = &rxe_type_info[i];
		size = ALIGN(type->size, RXE_POOL_ALIGN);
		type->cache = kmem_cache_create(type->name, size,
				RXE_POOL_ALIGN,
				RXE_POOL_CACHE_FLAGS, NULL);
		if (!type->cache) {
			pr_err("Unable to init kmem cache for %s\n",
			       type->name);
			err = -ENOMEM;
			goto err1;
		}
	}

	return 0;

err1:
	rxe_cache_clean(i);

	return err;
}

void rxe_cache_exit(void)
{
	rxe_cache_clean(RXE_NUM_TYPES);
}

=======
>>>>>>> upstream/android-13
static int rxe_pool_init_index(struct rxe_pool *pool, u32 max, u32 min)
{
	int err = 0;
	size_t size;

	if ((max - min + 1) < pool->max_elem) {
		pr_warn("not enough indices for max_elem\n");
		err = -EINVAL;
		goto out;
	}

<<<<<<< HEAD
	pool->max_index = max;
	pool->min_index = min;

	size = BITS_TO_LONGS(max - min + 1) * sizeof(long);
	pool->table = kmalloc(size, GFP_KERNEL);
	if (!pool->table) {
=======
	pool->index.max_index = max;
	pool->index.min_index = min;

	size = BITS_TO_LONGS(max - min + 1) * sizeof(long);
	pool->index.table = kmalloc(size, GFP_KERNEL);
	if (!pool->index.table) {
>>>>>>> upstream/android-13
		err = -ENOMEM;
		goto out;
	}

<<<<<<< HEAD
	pool->table_size = size;
	bitmap_zero(pool->table, max - min + 1);
=======
	pool->index.table_size = size;
	bitmap_zero(pool->index.table, max - min + 1);
>>>>>>> upstream/android-13

out:
	return err;
}

int rxe_pool_init(
	struct rxe_dev		*rxe,
	struct rxe_pool		*pool,
	enum rxe_elem_type	type,
	unsigned int		max_elem)
{
	int			err = 0;
	size_t			size = rxe_type_info[type].size;

	memset(pool, 0, sizeof(*pool));

	pool->rxe		= rxe;
	pool->type		= type;
	pool->max_elem		= max_elem;
	pool->elem_size		= ALIGN(size, RXE_POOL_ALIGN);
	pool->flags		= rxe_type_info[type].flags;
<<<<<<< HEAD
	pool->tree		= RB_ROOT;
=======
	pool->index.tree	= RB_ROOT;
	pool->key.tree		= RB_ROOT;
>>>>>>> upstream/android-13
	pool->cleanup		= rxe_type_info[type].cleanup;

	atomic_set(&pool->num_elem, 0);

<<<<<<< HEAD
	kref_init(&pool->ref_cnt);

	spin_lock_init(&pool->pool_lock);
=======
	rwlock_init(&pool->pool_lock);
>>>>>>> upstream/android-13

	if (rxe_type_info[type].flags & RXE_POOL_INDEX) {
		err = rxe_pool_init_index(pool,
					  rxe_type_info[type].max_index,
					  rxe_type_info[type].min_index);
		if (err)
			goto out;
	}

	if (rxe_type_info[type].flags & RXE_POOL_KEY) {
<<<<<<< HEAD
		pool->key_offset = rxe_type_info[type].key_offset;
		pool->key_size = rxe_type_info[type].key_size;
	}

	pool->state = rxe_pool_valid;

=======
		pool->key.key_offset = rxe_type_info[type].key_offset;
		pool->key.key_size = rxe_type_info[type].key_size;
	}

>>>>>>> upstream/android-13
out:
	return err;
}

<<<<<<< HEAD
static void rxe_pool_release(struct kref *kref)
{
	struct rxe_pool *pool = container_of(kref, struct rxe_pool, ref_cnt);

	pool->state = rxe_pool_invalid;
	kfree(pool->table);
}

static void rxe_pool_put(struct rxe_pool *pool)
{
	kref_put(&pool->ref_cnt, rxe_pool_release);
}

int rxe_pool_cleanup(struct rxe_pool *pool)
{
	unsigned long flags;

	spin_lock_irqsave(&pool->pool_lock, flags);
	pool->state = rxe_pool_invalid;
	if (atomic_read(&pool->num_elem) > 0)
		pr_warn("%s pool destroyed with unfree'd elem\n",
			pool_name(pool));
	spin_unlock_irqrestore(&pool->pool_lock, flags);

	rxe_pool_put(pool);

	return 0;
=======
void rxe_pool_cleanup(struct rxe_pool *pool)
{
	if (atomic_read(&pool->num_elem) > 0)
		pr_warn("%s pool destroyed with unfree'd elem\n",
			pool_name(pool));

	kfree(pool->index.table);
>>>>>>> upstream/android-13
}

static u32 alloc_index(struct rxe_pool *pool)
{
	u32 index;
<<<<<<< HEAD
	u32 range = pool->max_index - pool->min_index + 1;

	index = find_next_zero_bit(pool->table, range, pool->last);
	if (index >= range)
		index = find_first_zero_bit(pool->table, range);

	WARN_ON_ONCE(index >= range);
	set_bit(index, pool->table);
	pool->last = index;
	return index + pool->min_index;
}

static void insert_index(struct rxe_pool *pool, struct rxe_pool_entry *new)
{
	struct rb_node **link = &pool->tree.rb_node;
=======
	u32 range = pool->index.max_index - pool->index.min_index + 1;

	index = find_next_zero_bit(pool->index.table, range, pool->index.last);
	if (index >= range)
		index = find_first_zero_bit(pool->index.table, range);

	WARN_ON_ONCE(index >= range);
	set_bit(index, pool->index.table);
	pool->index.last = index;
	return index + pool->index.min_index;
}

static int rxe_insert_index(struct rxe_pool *pool, struct rxe_pool_entry *new)
{
	struct rb_node **link = &pool->index.tree.rb_node;
>>>>>>> upstream/android-13
	struct rb_node *parent = NULL;
	struct rxe_pool_entry *elem;

	while (*link) {
		parent = *link;
<<<<<<< HEAD
		elem = rb_entry(parent, struct rxe_pool_entry, node);

		if (elem->index == new->index) {
			pr_warn("element already exists!\n");
			goto out;
=======
		elem = rb_entry(parent, struct rxe_pool_entry, index_node);

		if (elem->index == new->index) {
			pr_warn("element already exists!\n");
			return -EINVAL;
>>>>>>> upstream/android-13
		}

		if (elem->index > new->index)
			link = &(*link)->rb_left;
		else
			link = &(*link)->rb_right;
	}

<<<<<<< HEAD
	rb_link_node(&new->node, parent, link);
	rb_insert_color(&new->node, &pool->tree);
out:
	return;
}

static void insert_key(struct rxe_pool *pool, struct rxe_pool_entry *new)
{
	struct rb_node **link = &pool->tree.rb_node;
=======
	rb_link_node(&new->index_node, parent, link);
	rb_insert_color(&new->index_node, &pool->index.tree);

	return 0;
}

static int rxe_insert_key(struct rxe_pool *pool, struct rxe_pool_entry *new)
{
	struct rb_node **link = &pool->key.tree.rb_node;
>>>>>>> upstream/android-13
	struct rb_node *parent = NULL;
	struct rxe_pool_entry *elem;
	int cmp;

	while (*link) {
		parent = *link;
<<<<<<< HEAD
		elem = rb_entry(parent, struct rxe_pool_entry, node);

		cmp = memcmp((u8 *)elem + pool->key_offset,
			     (u8 *)new + pool->key_offset, pool->key_size);

		if (cmp == 0) {
			pr_warn("key already exists!\n");
			goto out;
=======
		elem = rb_entry(parent, struct rxe_pool_entry, key_node);

		cmp = memcmp((u8 *)elem + pool->key.key_offset,
			     (u8 *)new + pool->key.key_offset, pool->key.key_size);

		if (cmp == 0) {
			pr_warn("key already exists!\n");
			return -EINVAL;
>>>>>>> upstream/android-13
		}

		if (cmp > 0)
			link = &(*link)->rb_left;
		else
			link = &(*link)->rb_right;
	}

<<<<<<< HEAD
	rb_link_node(&new->node, parent, link);
	rb_insert_color(&new->node, &pool->tree);
out:
	return;
}

void rxe_add_key(void *arg, void *key)
{
	struct rxe_pool_entry *elem = arg;
	struct rxe_pool *pool = elem->pool;
	unsigned long flags;

	spin_lock_irqsave(&pool->pool_lock, flags);
	memcpy((u8 *)elem + pool->key_offset, key, pool->key_size);
	insert_key(pool, elem);
	spin_unlock_irqrestore(&pool->pool_lock, flags);
}

void rxe_drop_key(void *arg)
{
	struct rxe_pool_entry *elem = arg;
	struct rxe_pool *pool = elem->pool;
	unsigned long flags;

	spin_lock_irqsave(&pool->pool_lock, flags);
	rb_erase(&elem->node, &pool->tree);
	spin_unlock_irqrestore(&pool->pool_lock, flags);
}

void rxe_add_index(void *arg)
{
	struct rxe_pool_entry *elem = arg;
	struct rxe_pool *pool = elem->pool;
	unsigned long flags;

	spin_lock_irqsave(&pool->pool_lock, flags);
	elem->index = alloc_index(pool);
	insert_index(pool, elem);
	spin_unlock_irqrestore(&pool->pool_lock, flags);
}

void rxe_drop_index(void *arg)
{
	struct rxe_pool_entry *elem = arg;
	struct rxe_pool *pool = elem->pool;
	unsigned long flags;

	spin_lock_irqsave(&pool->pool_lock, flags);
	clear_bit(elem->index - pool->min_index, pool->table);
	rb_erase(&elem->node, &pool->tree);
	spin_unlock_irqrestore(&pool->pool_lock, flags);
}

void *rxe_alloc(struct rxe_pool *pool)
{
	struct rxe_pool_entry *elem;
	unsigned long flags;

	might_sleep_if(!(pool->flags & RXE_POOL_ATOMIC));

	spin_lock_irqsave(&pool->pool_lock, flags);
	if (pool->state != rxe_pool_valid) {
		spin_unlock_irqrestore(&pool->pool_lock, flags);
		return NULL;
	}
	kref_get(&pool->ref_cnt);
	spin_unlock_irqrestore(&pool->pool_lock, flags);

	kref_get(&pool->rxe->ref_cnt);

	if (atomic_inc_return(&pool->num_elem) > pool->max_elem)
		goto out_put_pool;

	elem = kmem_cache_zalloc(pool_cache(pool),
				 (pool->flags & RXE_POOL_ATOMIC) ?
				 GFP_ATOMIC : GFP_KERNEL);
	if (!elem)
		goto out_put_pool;
=======
	rb_link_node(&new->key_node, parent, link);
	rb_insert_color(&new->key_node, &pool->key.tree);

	return 0;
}

int __rxe_add_key_locked(struct rxe_pool_entry *elem, void *key)
{
	struct rxe_pool *pool = elem->pool;
	int err;

	memcpy((u8 *)elem + pool->key.key_offset, key, pool->key.key_size);
	err = rxe_insert_key(pool, elem);

	return err;
}

int __rxe_add_key(struct rxe_pool_entry *elem, void *key)
{
	struct rxe_pool *pool = elem->pool;
	unsigned long flags;
	int err;

	write_lock_irqsave(&pool->pool_lock, flags);
	err = __rxe_add_key_locked(elem, key);
	write_unlock_irqrestore(&pool->pool_lock, flags);

	return err;
}

void __rxe_drop_key_locked(struct rxe_pool_entry *elem)
{
	struct rxe_pool *pool = elem->pool;

	rb_erase(&elem->key_node, &pool->key.tree);
}

void __rxe_drop_key(struct rxe_pool_entry *elem)
{
	struct rxe_pool *pool = elem->pool;
	unsigned long flags;

	write_lock_irqsave(&pool->pool_lock, flags);
	__rxe_drop_key_locked(elem);
	write_unlock_irqrestore(&pool->pool_lock, flags);
}

int __rxe_add_index_locked(struct rxe_pool_entry *elem)
{
	struct rxe_pool *pool = elem->pool;
	int err;

	elem->index = alloc_index(pool);
	err = rxe_insert_index(pool, elem);

	return err;
}

int __rxe_add_index(struct rxe_pool_entry *elem)
{
	struct rxe_pool *pool = elem->pool;
	unsigned long flags;
	int err;

	write_lock_irqsave(&pool->pool_lock, flags);
	err = __rxe_add_index_locked(elem);
	write_unlock_irqrestore(&pool->pool_lock, flags);

	return err;
}

void __rxe_drop_index_locked(struct rxe_pool_entry *elem)
{
	struct rxe_pool *pool = elem->pool;

	clear_bit(elem->index - pool->index.min_index, pool->index.table);
	rb_erase(&elem->index_node, &pool->index.tree);
}

void __rxe_drop_index(struct rxe_pool_entry *elem)
{
	struct rxe_pool *pool = elem->pool;
	unsigned long flags;

	write_lock_irqsave(&pool->pool_lock, flags);
	__rxe_drop_index_locked(elem);
	write_unlock_irqrestore(&pool->pool_lock, flags);
}

void *rxe_alloc_locked(struct rxe_pool *pool)
{
	struct rxe_type_info *info = &rxe_type_info[pool->type];
	struct rxe_pool_entry *elem;
	u8 *obj;

	if (atomic_inc_return(&pool->num_elem) > pool->max_elem)
		goto out_cnt;

	obj = kzalloc(info->size, GFP_ATOMIC);
	if (!obj)
		goto out_cnt;

	elem = (struct rxe_pool_entry *)(obj + info->elem_offset);
>>>>>>> upstream/android-13

	elem->pool = pool;
	kref_init(&elem->ref_cnt);

<<<<<<< HEAD
	return elem;

out_put_pool:
	atomic_dec(&pool->num_elem);
	rxe_dev_put(pool->rxe);
	rxe_pool_put(pool);
	return NULL;
}

=======
	return obj;

out_cnt:
	atomic_dec(&pool->num_elem);
	return NULL;
}

void *rxe_alloc(struct rxe_pool *pool)
{
	struct rxe_type_info *info = &rxe_type_info[pool->type];
	struct rxe_pool_entry *elem;
	u8 *obj;

	if (atomic_inc_return(&pool->num_elem) > pool->max_elem)
		goto out_cnt;

	obj = kzalloc(info->size, GFP_KERNEL);
	if (!obj)
		goto out_cnt;

	elem = (struct rxe_pool_entry *)(obj + info->elem_offset);

	elem->pool = pool;
	kref_init(&elem->ref_cnt);

	return obj;

out_cnt:
	atomic_dec(&pool->num_elem);
	return NULL;
}

int __rxe_add_to_pool(struct rxe_pool *pool, struct rxe_pool_entry *elem)
{
	if (atomic_inc_return(&pool->num_elem) > pool->max_elem)
		goto out_cnt;

	elem->pool = pool;
	kref_init(&elem->ref_cnt);

	return 0;

out_cnt:
	atomic_dec(&pool->num_elem);
	return -EINVAL;
}

>>>>>>> upstream/android-13
void rxe_elem_release(struct kref *kref)
{
	struct rxe_pool_entry *elem =
		container_of(kref, struct rxe_pool_entry, ref_cnt);
	struct rxe_pool *pool = elem->pool;
<<<<<<< HEAD
=======
	struct rxe_type_info *info = &rxe_type_info[pool->type];
	u8 *obj;
>>>>>>> upstream/android-13

	if (pool->cleanup)
		pool->cleanup(elem);

<<<<<<< HEAD
	kmem_cache_free(pool_cache(pool), elem);
	atomic_dec(&pool->num_elem);
	rxe_dev_put(pool->rxe);
	rxe_pool_put(pool);
}

void *rxe_pool_get_index(struct rxe_pool *pool, u32 index)
{
	struct rb_node *node = NULL;
	struct rxe_pool_entry *elem = NULL;
	unsigned long flags;

	spin_lock_irqsave(&pool->pool_lock, flags);

	if (pool->state != rxe_pool_valid)
		goto out;

	node = pool->tree.rb_node;

	while (node) {
		elem = rb_entry(node, struct rxe_pool_entry, node);
=======
	if (!(pool->flags & RXE_POOL_NO_ALLOC)) {
		obj = (u8 *)elem - info->elem_offset;
		kfree(obj);
	}

	atomic_dec(&pool->num_elem);
}

void *rxe_pool_get_index_locked(struct rxe_pool *pool, u32 index)
{
	struct rxe_type_info *info = &rxe_type_info[pool->type];
	struct rb_node *node;
	struct rxe_pool_entry *elem;
	u8 *obj;

	node = pool->index.tree.rb_node;

	while (node) {
		elem = rb_entry(node, struct rxe_pool_entry, index_node);
>>>>>>> upstream/android-13

		if (elem->index > index)
			node = node->rb_left;
		else if (elem->index < index)
			node = node->rb_right;
		else
			break;
	}

<<<<<<< HEAD
	if (node)
		kref_get(&elem->ref_cnt);

out:
	spin_unlock_irqrestore(&pool->pool_lock, flags);
	return node ? elem : NULL;
}

void *rxe_pool_get_key(struct rxe_pool *pool, void *key)
{
	struct rb_node *node = NULL;
	struct rxe_pool_entry *elem = NULL;
	int cmp;
	unsigned long flags;

	spin_lock_irqsave(&pool->pool_lock, flags);

	if (pool->state != rxe_pool_valid)
		goto out;

	node = pool->tree.rb_node;

	while (node) {
		elem = rb_entry(node, struct rxe_pool_entry, node);

		cmp = memcmp((u8 *)elem + pool->key_offset,
			     key, pool->key_size);
=======
	if (node) {
		kref_get(&elem->ref_cnt);
		obj = (u8 *)elem - info->elem_offset;
	} else {
		obj = NULL;
	}

	return obj;
}

void *rxe_pool_get_index(struct rxe_pool *pool, u32 index)
{
	u8 *obj;
	unsigned long flags;

	read_lock_irqsave(&pool->pool_lock, flags);
	obj = rxe_pool_get_index_locked(pool, index);
	read_unlock_irqrestore(&pool->pool_lock, flags);

	return obj;
}

void *rxe_pool_get_key_locked(struct rxe_pool *pool, void *key)
{
	struct rxe_type_info *info = &rxe_type_info[pool->type];
	struct rb_node *node;
	struct rxe_pool_entry *elem;
	u8 *obj;
	int cmp;

	node = pool->key.tree.rb_node;

	while (node) {
		elem = rb_entry(node, struct rxe_pool_entry, key_node);

		cmp = memcmp((u8 *)elem + pool->key.key_offset,
			     key, pool->key.key_size);
>>>>>>> upstream/android-13

		if (cmp > 0)
			node = node->rb_left;
		else if (cmp < 0)
			node = node->rb_right;
		else
			break;
	}

<<<<<<< HEAD
	if (node)
		kref_get(&elem->ref_cnt);

out:
	spin_unlock_irqrestore(&pool->pool_lock, flags);
	return node ? elem : NULL;
=======
	if (node) {
		kref_get(&elem->ref_cnt);
		obj = (u8 *)elem - info->elem_offset;
	} else {
		obj = NULL;
	}

	return obj;
}

void *rxe_pool_get_key(struct rxe_pool *pool, void *key)
{
	u8 *obj;
	unsigned long flags;

	read_lock_irqsave(&pool->pool_lock, flags);
	obj = rxe_pool_get_key_locked(pool, key);
	read_unlock_irqrestore(&pool->pool_lock, flags);

	return obj;
>>>>>>> upstream/android-13
}
