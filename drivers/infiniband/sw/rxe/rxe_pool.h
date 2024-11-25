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
/* SPDX-License-Identifier: GPL-2.0 OR Linux-OpenIB */
/*
 * Copyright (c) 2016 Mellanox Technologies Ltd. All rights reserved.
 * Copyright (c) 2015 System Fabric Works, Inc. All rights reserved.
>>>>>>> upstream/android-13
 */

#ifndef RXE_POOL_H
#define RXE_POOL_H

#define RXE_POOL_ALIGN		(16)
#define RXE_POOL_CACHE_FLAGS	(0)

enum rxe_pool_flags {
<<<<<<< HEAD
	RXE_POOL_ATOMIC		= BIT(0),
	RXE_POOL_INDEX		= BIT(1),
	RXE_POOL_KEY		= BIT(2),
=======
	RXE_POOL_INDEX		= BIT(1),
	RXE_POOL_KEY		= BIT(2),
	RXE_POOL_NO_ALLOC	= BIT(4),
>>>>>>> upstream/android-13
};

enum rxe_elem_type {
	RXE_TYPE_UC,
	RXE_TYPE_PD,
	RXE_TYPE_AH,
	RXE_TYPE_SRQ,
	RXE_TYPE_QP,
	RXE_TYPE_CQ,
	RXE_TYPE_MR,
	RXE_TYPE_MW,
	RXE_TYPE_MC_GRP,
	RXE_TYPE_MC_ELEM,
	RXE_NUM_TYPES,		/* keep me last */
};

struct rxe_pool_entry;

struct rxe_type_info {
	const char		*name;
	size_t			size;
<<<<<<< HEAD
=======
	size_t			elem_offset;
>>>>>>> upstream/android-13
	void			(*cleanup)(struct rxe_pool_entry *obj);
	enum rxe_pool_flags	flags;
	u32			max_index;
	u32			min_index;
	size_t			key_offset;
	size_t			key_size;
<<<<<<< HEAD
	struct kmem_cache	*cache;
=======
>>>>>>> upstream/android-13
};

extern struct rxe_type_info rxe_type_info[];

<<<<<<< HEAD
enum rxe_pool_state {
	rxe_pool_invalid,
	rxe_pool_valid,
};

=======
>>>>>>> upstream/android-13
struct rxe_pool_entry {
	struct rxe_pool		*pool;
	struct kref		ref_cnt;
	struct list_head	list;

<<<<<<< HEAD
	/* only used if indexed or keyed */
	struct rb_node		node;
=======
	/* only used if keyed */
	struct rb_node		key_node;

	/* only used if indexed */
	struct rb_node		index_node;
>>>>>>> upstream/android-13
	u32			index;
};

struct rxe_pool {
	struct rxe_dev		*rxe;
<<<<<<< HEAD
	spinlock_t              pool_lock; /* pool spinlock */
	size_t			elem_size;
	struct kref		ref_cnt;
	void			(*cleanup)(struct rxe_pool_entry *obj);
	enum rxe_pool_state	state;
=======
	rwlock_t		pool_lock; /* protects pool add/del/search */
	size_t			elem_size;
	void			(*cleanup)(struct rxe_pool_entry *obj);
>>>>>>> upstream/android-13
	enum rxe_pool_flags	flags;
	enum rxe_elem_type	type;

	unsigned int		max_elem;
	atomic_t		num_elem;

<<<<<<< HEAD
	/* only used if indexed or keyed */
	struct rb_root		tree;
	unsigned long		*table;
	size_t			table_size;
	u32			max_index;
	u32			min_index;
	u32			last;
	size_t			key_offset;
	size_t			key_size;
};

/* initialize slab caches for managed objects */
int rxe_cache_init(void);

/* cleanup slab caches for managed objects */
void rxe_cache_exit(void);

=======
	/* only used if indexed */
	struct {
		struct rb_root		tree;
		unsigned long		*table;
		size_t			table_size;
		u32			last;
		u32			max_index;
		u32			min_index;
	} index;

	/* only used if keyed */
	struct {
		struct rb_root		tree;
		size_t			key_offset;
		size_t			key_size;
	} key;
};

>>>>>>> upstream/android-13
/* initialize a pool of objects with given limit on
 * number of elements. gets parameters from rxe_type_info
 * pool elements will be allocated out of a slab cache
 */
int rxe_pool_init(struct rxe_dev *rxe, struct rxe_pool *pool,
		  enum rxe_elem_type type, u32 max_elem);

/* free resources from object pool */
<<<<<<< HEAD
int rxe_pool_cleanup(struct rxe_pool *pool);

/* allocate an object from pool */
void *rxe_alloc(struct rxe_pool *pool);

/* assign an index to an indexed object and insert object into
 *  pool's rb tree
 */
void rxe_add_index(void *elem);

/* drop an index and remove object from rb tree */
void rxe_drop_index(void *elem);

/* assign a key to a keyed object and insert object into
 *  pool's rb tree
 */
void rxe_add_key(void *elem, void *key);

/* remove elem from rb tree */
void rxe_drop_key(void *elem);

/* lookup an indexed object from index. takes a reference on object */
void *rxe_pool_get_index(struct rxe_pool *pool, u32 index);

/* lookup keyed object from key. takes a reference on the object */
=======
void rxe_pool_cleanup(struct rxe_pool *pool);

/* allocate an object from pool holding and not holding the pool lock */
void *rxe_alloc_locked(struct rxe_pool *pool);

void *rxe_alloc(struct rxe_pool *pool);

/* connect already allocated object to pool */
int __rxe_add_to_pool(struct rxe_pool *pool, struct rxe_pool_entry *elem);

#define rxe_add_to_pool(pool, obj) __rxe_add_to_pool(pool, &(obj)->pelem)

/* assign an index to an indexed object and insert object into
 *  pool's rb tree holding and not holding the pool_lock
 */
int __rxe_add_index_locked(struct rxe_pool_entry *elem);

#define rxe_add_index_locked(obj) __rxe_add_index_locked(&(obj)->pelem)

int __rxe_add_index(struct rxe_pool_entry *elem);

#define rxe_add_index(obj) __rxe_add_index(&(obj)->pelem)

/* drop an index and remove object from rb tree
 * holding and not holding the pool_lock
 */
void __rxe_drop_index_locked(struct rxe_pool_entry *elem);

#define rxe_drop_index_locked(obj) __rxe_drop_index_locked(&(obj)->pelem)

void __rxe_drop_index(struct rxe_pool_entry *elem);

#define rxe_drop_index(obj) __rxe_drop_index(&(obj)->pelem)

/* assign a key to a keyed object and insert object into
 * pool's rb tree holding and not holding pool_lock
 */
int __rxe_add_key_locked(struct rxe_pool_entry *elem, void *key);

#define rxe_add_key_locked(obj, key) __rxe_add_key_locked(&(obj)->pelem, key)

int __rxe_add_key(struct rxe_pool_entry *elem, void *key);

#define rxe_add_key(obj, key) __rxe_add_key(&(obj)->pelem, key)

/* remove elem from rb tree holding and not holding the pool_lock */
void __rxe_drop_key_locked(struct rxe_pool_entry *elem);

#define rxe_drop_key_locked(obj) __rxe_drop_key_locked(&(obj)->pelem)

void __rxe_drop_key(struct rxe_pool_entry *elem);

#define rxe_drop_key(obj) __rxe_drop_key(&(obj)->pelem)

/* lookup an indexed object from index holding and not holding the pool_lock.
 * takes a reference on object
 */
void *rxe_pool_get_index_locked(struct rxe_pool *pool, u32 index);

void *rxe_pool_get_index(struct rxe_pool *pool, u32 index);

/* lookup keyed object from key holding and not holding the pool_lock.
 * takes a reference on the objecti
 */
void *rxe_pool_get_key_locked(struct rxe_pool *pool, void *key);

>>>>>>> upstream/android-13
void *rxe_pool_get_key(struct rxe_pool *pool, void *key);

/* cleanup an object when all references are dropped */
void rxe_elem_release(struct kref *kref);

/* take a reference on an object */
#define rxe_add_ref(elem) kref_get(&(elem)->pelem.ref_cnt)

/* drop a reference on an object */
#define rxe_drop_ref(elem) kref_put(&(elem)->pelem.ref_cnt, rxe_elem_release)

#endif /* RXE_POOL_H */
