// SPDX-License-Identifier: GPL-2.0
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <pthread.h>
#include <unistd.h>
#include <assert.h>

#include <linux/gfp.h>
#include <linux/poison.h>
#include <linux/slab.h>
#include <linux/radix-tree.h>
#include <urcu/uatomic.h>

int nr_allocated;
int preempt_count;
int kmalloc_verbose;
int test_verbose;

struct kmem_cache {
	pthread_mutex_t lock;
<<<<<<< HEAD
	int size;
=======
	unsigned int size;
	unsigned int align;
>>>>>>> upstream/android-13
	int nr_objs;
	void *objs;
	void (*ctor)(void *);
};

<<<<<<< HEAD
void *kmem_cache_alloc(struct kmem_cache *cachep, int flags)
{
	struct radix_tree_node *node;

	if (!(flags & __GFP_DIRECT_RECLAIM))
=======
void *kmem_cache_alloc(struct kmem_cache *cachep, int gfp)
{
	void *p;

	if (!(gfp & __GFP_DIRECT_RECLAIM))
>>>>>>> upstream/android-13
		return NULL;

	pthread_mutex_lock(&cachep->lock);
	if (cachep->nr_objs) {
<<<<<<< HEAD
		cachep->nr_objs--;
		node = cachep->objs;
		cachep->objs = node->parent;
		pthread_mutex_unlock(&cachep->lock);
		node->parent = NULL;
	} else {
		pthread_mutex_unlock(&cachep->lock);
		node = malloc(cachep->size);
		if (cachep->ctor)
			cachep->ctor(node);
=======
		struct radix_tree_node *node = cachep->objs;
		cachep->nr_objs--;
		cachep->objs = node->parent;
		pthread_mutex_unlock(&cachep->lock);
		node->parent = NULL;
		p = node;
	} else {
		pthread_mutex_unlock(&cachep->lock);
		if (cachep->align)
			posix_memalign(&p, cachep->align, cachep->size);
		else
			p = malloc(cachep->size);
		if (cachep->ctor)
			cachep->ctor(p);
		else if (gfp & __GFP_ZERO)
			memset(p, 0, cachep->size);
>>>>>>> upstream/android-13
	}

	uatomic_inc(&nr_allocated);
	if (kmalloc_verbose)
<<<<<<< HEAD
		printf("Allocating %p from slab\n", node);
	return node;
=======
		printf("Allocating %p from slab\n", p);
	return p;
>>>>>>> upstream/android-13
}

void kmem_cache_free(struct kmem_cache *cachep, void *objp)
{
	assert(objp);
	uatomic_dec(&nr_allocated);
	if (kmalloc_verbose)
		printf("Freeing %p to slab\n", objp);
	pthread_mutex_lock(&cachep->lock);
<<<<<<< HEAD
	if (cachep->nr_objs > 10) {
=======
	if (cachep->nr_objs > 10 || cachep->align) {
>>>>>>> upstream/android-13
		memset(objp, POISON_FREE, cachep->size);
		free(objp);
	} else {
		struct radix_tree_node *node = objp;
		cachep->nr_objs++;
		node->parent = cachep->objs;
		cachep->objs = node;
	}
	pthread_mutex_unlock(&cachep->lock);
}

void *kmalloc(size_t size, gfp_t gfp)
{
	void *ret;

	if (!(gfp & __GFP_DIRECT_RECLAIM))
		return NULL;

	ret = malloc(size);
	uatomic_inc(&nr_allocated);
	if (kmalloc_verbose)
		printf("Allocating %p from malloc\n", ret);
	if (gfp & __GFP_ZERO)
		memset(ret, 0, size);
	return ret;
}

void kfree(void *p)
{
	if (!p)
		return;
	uatomic_dec(&nr_allocated);
	if (kmalloc_verbose)
		printf("Freeing %p to malloc\n", p);
	free(p);
}

struct kmem_cache *
<<<<<<< HEAD
kmem_cache_create(const char *name, size_t size, size_t offset,
	unsigned long flags, void (*ctor)(void *))
=======
kmem_cache_create(const char *name, unsigned int size, unsigned int align,
		unsigned int flags, void (*ctor)(void *))
>>>>>>> upstream/android-13
{
	struct kmem_cache *ret = malloc(sizeof(*ret));

	pthread_mutex_init(&ret->lock, NULL);
	ret->size = size;
<<<<<<< HEAD
=======
	ret->align = align;
>>>>>>> upstream/android-13
	ret->nr_objs = 0;
	ret->objs = NULL;
	ret->ctor = ctor;
	return ret;
}
