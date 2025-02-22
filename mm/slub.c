// SPDX-License-Identifier: GPL-2.0
/*
 * SLUB: A slab allocator that limits cache line use instead of queuing
 * objects in per cpu and per node lists.
 *
<<<<<<< HEAD
 * The allocator synchronizes using per slab locks or atomic operatios
=======
 * The allocator synchronizes using per slab locks or atomic operations
>>>>>>> upstream/android-13
 * and only uses a centralized lock to manage a pool of partial slabs.
 *
 * (C) 2007 SGI, Christoph Lameter
 * (C) 2011 Linux Foundation, Christoph Lameter
 */

#include <linux/mm.h>
#include <linux/swap.h> /* struct reclaim_state */
#include <linux/module.h>
#include <linux/bit_spinlock.h>
#include <linux/interrupt.h>
<<<<<<< HEAD
=======
#include <linux/swab.h>
>>>>>>> upstream/android-13
#include <linux/bitops.h>
#include <linux/slab.h>
#include "slab.h"
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/kasan.h>
#include <linux/cpu.h>
#include <linux/cpuset.h>
#include <linux/mempolicy.h>
#include <linux/ctype.h>
#include <linux/debugobjects.h>
#include <linux/kallsyms.h>
<<<<<<< HEAD
=======
#include <linux/kfence.h>
>>>>>>> upstream/android-13
#include <linux/memory.h>
#include <linux/math64.h>
#include <linux/fault-inject.h>
#include <linux/stacktrace.h>
#include <linux/prefetch.h>
#include <linux/memcontrol.h>
#include <linux/random.h>
<<<<<<< HEAD

=======
#include <kunit/test.h>
#include <linux/sec_debug.h>

#include <linux/debugfs.h>
>>>>>>> upstream/android-13
#include <trace/events/kmem.h>

#include "internal.h"

<<<<<<< HEAD
#ifdef CONFIG_RKP
#include <linux/rkp.h>
#endif

#ifdef CONFIG_KDP
#include <linux/kdp.h>
#endif

#ifdef CONFIG_SEC_DEBUG_AUTO_COMMENT
#include <linux/sec_debug.h>
#endif

/*
 * Lock order:
 *   1. slab_mutex (Global Mutex)
 *   2. node->list_lock
 *   3. slab_lock(page) (Only on some arches and for debugging)
=======
/*
 * Lock order:
 *   1. slab_mutex (Global Mutex)
 *   2. node->list_lock (Spinlock)
 *   3. kmem_cache->cpu_slab->lock (Local lock)
 *   4. slab_lock(page) (Only on some arches or for debugging)
 *   5. object_map_lock (Only for debugging)
>>>>>>> upstream/android-13
 *
 *   slab_mutex
 *
 *   The role of the slab_mutex is to protect the list of all the slabs
 *   and to synchronize major metadata changes to slab cache structures.
<<<<<<< HEAD
=======
 *   Also synchronizes memory hotplug callbacks.
 *
 *   slab_lock
 *
 *   The slab_lock is a wrapper around the page lock, thus it is a bit
 *   spinlock.
>>>>>>> upstream/android-13
 *
 *   The slab_lock is only used for debugging and on arches that do not
 *   have the ability to do a cmpxchg_double. It only protects:
 *	A. page->freelist	-> List of object free in a page
 *	B. page->inuse		-> Number of objects in use
 *	C. page->objects	-> Number of objects in page
 *	D. page->frozen		-> frozen state
 *
<<<<<<< HEAD
 *   If a slab is frozen then it is exempt from list management. It is not
 *   on any list. The processor that froze the slab is the one who can
 *   perform list operations on the page. Other processors may put objects
 *   onto the freelist but the processor that froze the slab is the only
 *   one that can retrieve the objects from the page's freelist.
=======
 *   Frozen slabs
 *
 *   If a slab is frozen then it is exempt from list management. It is not
 *   on any list except per cpu partial list. The processor that froze the
 *   slab is the one who can perform list operations on the page. Other
 *   processors may put objects onto the freelist but the processor that
 *   froze the slab is the only one that can retrieve the objects from the
 *   page's freelist.
 *
 *   list_lock
>>>>>>> upstream/android-13
 *
 *   The list_lock protects the partial and full list on each node and
 *   the partial slab counter. If taken then no new slabs may be added or
 *   removed from the lists nor make the number of partial slabs be modified.
 *   (Note that the total number of slabs is an atomic value that may be
 *   modified without taking the list lock).
 *
 *   The list_lock is a centralized lock and thus we avoid taking it as
 *   much as possible. As long as SLUB does not have to handle partial
 *   slabs, operations can continue without any centralized lock. F.e.
 *   allocating a long series of objects that fill up slabs does not require
 *   the list lock.
<<<<<<< HEAD
 *   Interrupts are disabled during allocation and deallocation in order to
 *   make the slab allocator safe to use in the context of an irq. In addition
 *   interrupts are disabled to ensure that the processor does not change
 *   while handling per_cpu slabs, due to kernel preemption.
=======
 *
 *   cpu_slab->lock local lock
 *
 *   This locks protect slowpath manipulation of all kmem_cache_cpu fields
 *   except the stat counters. This is a percpu structure manipulated only by
 *   the local cpu, so the lock protects against being preempted or interrupted
 *   by an irq. Fast path operations rely on lockless operations instead.
 *   On PREEMPT_RT, the local lock does not actually disable irqs (and thus
 *   prevent the lockless operations), so fastpath operations also need to take
 *   the lock and are no longer lockless.
 *
 *   lockless fastpaths
 *
 *   The fast path allocation (slab_alloc_node()) and freeing (do_slab_free())
 *   are fully lockless when satisfied from the percpu slab (and when
 *   cmpxchg_double is possible to use, otherwise slab_lock is taken).
 *   They also don't disable preemption or migration or irqs. They rely on
 *   the transaction id (tid) field to detect being preempted or moved to
 *   another cpu.
 *
 *   irq, preemption, migration considerations
 *
 *   Interrupts are disabled as part of list_lock or local_lock operations, or
 *   around the slab_lock operation, in order to make the slab allocator safe
 *   to use in the context of an irq.
 *
 *   In addition, preemption (or migration on PREEMPT_RT) is disabled in the
 *   allocation slowpath, bulk allocation, and put_cpu_partial(), so that the
 *   local cpu doesn't change in the process and e.g. the kmem_cache_cpu pointer
 *   doesn't have to be revalidated in each section protected by the local lock.
>>>>>>> upstream/android-13
 *
 * SLUB assigns one slab for allocation to each processor.
 * Allocations only occur from these slabs called cpu slabs.
 *
 * Slabs with free elements are kept on a partial list and during regular
 * operations no list for full slabs is used. If an object in a full slab is
 * freed then the slab will show up again on the partial lists.
 * We track full slabs for debugging purposes though because otherwise we
 * cannot scan all objects.
 *
 * Slabs are freed when they become empty. Teardown and setup is
 * minimal so we rely on the page allocators per cpu caches for
 * fast frees and allocs.
 *
<<<<<<< HEAD
 * Overloading of page flags that are otherwise used for LRU management.
 *
 * PageActive 		The slab is frozen and exempt from list processing.
=======
 * page->frozen		The slab is frozen and exempt from list processing.
>>>>>>> upstream/android-13
 * 			This means that the slab is dedicated to a purpose
 * 			such as satisfying allocations for a specific
 * 			processor. Objects may be freed in the slab while
 * 			it is frozen but slab_free will then skip the usual
 * 			list operations. It is up to the processor holding
 * 			the slab to integrate the slab into the slab lists
 * 			when the slab is no longer needed.
 *
 * 			One use of this flag is to mark slabs that are
 * 			used for allocations. Then such a slab becomes a cpu
 * 			slab. The cpu slab may be equipped with an additional
 * 			freelist that allows lockless access to
 * 			free objects in addition to the regular freelist
 * 			that requires the slab lock.
 *
<<<<<<< HEAD
 * PageError		Slab requires special handling due to debug
=======
 * SLAB_DEBUG_FLAGS	Slab requires special handling due to debug
>>>>>>> upstream/android-13
 * 			options set. This moves	slab handling out of
 * 			the fast path and disables lockless freelists.
 */

<<<<<<< HEAD
static inline int kmem_cache_debug(struct kmem_cache *s)
{
#ifdef CONFIG_SLUB_DEBUG
	return unlikely(s->flags & SLAB_DEBUG_FLAGS);
#else
	return 0;
#endif
=======
/*
 * We could simply use migrate_disable()/enable() but as long as it's a
 * function call even on !PREEMPT_RT, use inline preempt_disable() there.
 */
#ifndef CONFIG_PREEMPT_RT
#define slub_get_cpu_ptr(var)	get_cpu_ptr(var)
#define slub_put_cpu_ptr(var)	put_cpu_ptr(var)
#else
#define slub_get_cpu_ptr(var)		\
({					\
	migrate_disable();		\
	this_cpu_ptr(var);		\
})
#define slub_put_cpu_ptr(var)		\
do {					\
	(void)(var);			\
	migrate_enable();		\
} while (0)
#endif

#ifdef CONFIG_SLUB_DEBUG
#ifdef CONFIG_SLUB_DEBUG_ON
DEFINE_STATIC_KEY_TRUE(slub_debug_enabled);
#else
DEFINE_STATIC_KEY_FALSE(slub_debug_enabled);
#endif
#endif		/* CONFIG_SLUB_DEBUG */

static inline bool kmem_cache_debug(struct kmem_cache *s)
{
	return kmem_cache_debug_flags(s, SLAB_DEBUG_FLAGS);
>>>>>>> upstream/android-13
}

void *fixup_red_left(struct kmem_cache *s, void *p)
{
<<<<<<< HEAD
	if (kmem_cache_debug(s) && s->flags & SLAB_RED_ZONE)
=======
	if (kmem_cache_debug_flags(s, SLAB_RED_ZONE))
>>>>>>> upstream/android-13
		p += s->red_left_pad;

	return p;
}

static inline bool kmem_cache_has_cpu_partial(struct kmem_cache *s)
{
#ifdef CONFIG_SLUB_CPU_PARTIAL
	return !kmem_cache_debug(s);
#else
	return false;
#endif
}

/*
 * Issues still to be resolved:
 *
 * - Support PAGE_ALLOC_DEBUG. Should be easy to do.
 *
 * - Variable sizing of the per node arrays
 */

<<<<<<< HEAD
/* Enable to test recovery from slab corruption on boot */
#undef SLUB_RESILIENCY_TEST

=======
>>>>>>> upstream/android-13
/* Enable to log cmpxchg failures */
#undef SLUB_DEBUG_CMPXCHG

/*
<<<<<<< HEAD
 * Mininum number of partial slabs. These will be left on the partial
=======
 * Minimum number of partial slabs. These will be left on the partial
>>>>>>> upstream/android-13
 * lists even if they are empty. kmem_cache_shrink may reclaim them.
 */
#define MIN_PARTIAL 5

/*
 * Maximum number of desirable partial slabs.
 * The existence of more partial slabs makes kmem_cache_shrink
 * sort the partial list by the number of objects in use.
 */
#define MAX_PARTIAL 10

#define DEBUG_DEFAULT_FLAGS (SLAB_CONSISTENCY_CHECKS | SLAB_RED_ZONE | \
				SLAB_POISON | SLAB_STORE_USER)

/*
 * These debug flags cannot use CMPXCHG because there might be consistency
 * issues when checking or reading debug information
 */
#define SLAB_NO_CMPXCHG (SLAB_CONSISTENCY_CHECKS | SLAB_STORE_USER | \
				SLAB_TRACE)


/*
 * Debugging flags that require metadata to be stored in the slab.  These get
 * disabled when slub_debug=O is used and a cache's min order increases with
 * metadata.
 */
#define DEBUG_METADATA_FLAGS (SLAB_RED_ZONE | SLAB_POISON | SLAB_STORE_USER)

#define OO_SHIFT	16
#define OO_MASK		((1 << OO_SHIFT) - 1)
#define MAX_OBJS_PER_PAGE	32767 /* since page.objects is u15 */

/* Internal SLUB flags */
/* Poison object */
#define __OBJECT_POISON		((slab_flags_t __force)0x80000000U)
/* Use cmpxchg_double */
#define __CMPXCHG_DOUBLE	((slab_flags_t __force)0x40000000U)

<<<<<<< HEAD
/*
 * Tracking user of a slab.
 */
#define TRACK_ADDRS_COUNT 16
struct track {
	unsigned long addr;	/* Called from address */
#ifdef CONFIG_STACKTRACE
	unsigned long addrs[TRACK_ADDRS_COUNT];	/* Called from address */
#endif
	int cpu;		/* Was running on cpu */
	int pid;		/* Pid context */
	unsigned long when;	/* When did the operation occur */
};

enum track_item { TRACK_ALLOC, TRACK_FREE };

#ifdef CONFIG_SYSFS
static int sysfs_slab_add(struct kmem_cache *);
static int sysfs_slab_alias(struct kmem_cache *, const char *);
static void memcg_propagate_slab_attrs(struct kmem_cache *s);
static void sysfs_slab_remove(struct kmem_cache *s);
=======
#ifdef CONFIG_SYSFS
static int sysfs_slab_add(struct kmem_cache *);
static int sysfs_slab_alias(struct kmem_cache *, const char *);
>>>>>>> upstream/android-13
#else
static inline int sysfs_slab_add(struct kmem_cache *s) { return 0; }
static inline int sysfs_slab_alias(struct kmem_cache *s, const char *p)
							{ return 0; }
<<<<<<< HEAD
static inline void memcg_propagate_slab_attrs(struct kmem_cache *s) { }
static inline void sysfs_slab_remove(struct kmem_cache *s) { }
=======
#endif

#if defined(CONFIG_DEBUG_FS) && defined(CONFIG_SLUB_DEBUG)
static void debugfs_slab_add(struct kmem_cache *);
#else
static inline void debugfs_slab_add(struct kmem_cache *s) { }
#endif

#ifdef CONFIG_SEC_DEBUG_BUG_ON_SLUB_CORRUPTION
static inline void secdbg_slub_bug(void)
{
	BUG();
}
#else
static inline void secdbg_slub_bug(void)
{
}
>>>>>>> upstream/android-13
#endif

static inline void stat(const struct kmem_cache *s, enum stat_item si)
{
#ifdef CONFIG_SLUB_STATS
	/*
	 * The rmw is racy on a preemptible kernel but this is acceptable, so
	 * avoid this_cpu_add()'s irq-disable overhead.
	 */
	raw_cpu_inc(s->cpu_slab->stat[si]);
#endif
}

<<<<<<< HEAD
=======
/*
 * Tracks for which NUMA nodes we have kmem_cache_nodes allocated.
 * Corresponds to node_state[N_NORMAL_MEMORY], but can temporarily
 * differ during memory hotplug/hotremove operations.
 * Protected by slab_mutex.
 */
static nodemask_t slab_nodes;

>>>>>>> upstream/android-13
/********************************************************************
 * 			Core slab cache functions
 *******************************************************************/

/*
 * Returns freelist pointer (ptr). With hardening, this is obfuscated
 * with an XOR of the address where the pointer is held and a per-cache
 * random number.
 */
static inline void *freelist_ptr(const struct kmem_cache *s, void *ptr,
				 unsigned long ptr_addr)
{
#ifdef CONFIG_SLAB_FREELIST_HARDENED
	/*
<<<<<<< HEAD
	 * When CONFIG_KASAN_SW_TAGS is enabled, ptr_addr might be tagged.
=======
	 * When CONFIG_KASAN_SW/HW_TAGS is enabled, ptr_addr might be tagged.
>>>>>>> upstream/android-13
	 * Normally, this doesn't cause any issues, as both set_freepointer()
	 * and get_freepointer() are called with a pointer with the same tag.
	 * However, there are some issues with CONFIG_SLUB_DEBUG code. For
	 * example, when __free_slub() iterates over objects in a cache, it
	 * passes untagged pointers to check_object(). check_object() in turns
	 * calls get_freepointer() with an untagged pointer, which causes the
	 * freepointer to be restored incorrectly.
	 */
	return (void *)((unsigned long)ptr ^ s->random ^
			swab((unsigned long)kasan_reset_tag((void *)ptr_addr)));
#else
	return ptr;
#endif
}

/* Returns the freelist pointer recorded at location ptr_addr. */
static inline void *freelist_dereference(const struct kmem_cache *s,
					 void *ptr_addr)
{
	return freelist_ptr(s, (void *)*(unsigned long *)(ptr_addr),
			    (unsigned long)ptr_addr);
}

static inline void *get_freepointer(struct kmem_cache *s, void *object)
{
<<<<<<< HEAD
=======
	object = kasan_reset_tag(object);
>>>>>>> upstream/android-13
	return freelist_dereference(s, object + s->offset);
}

static void prefetch_freepointer(const struct kmem_cache *s, void *object)
{
	prefetch(object + s->offset);
}

static inline void *get_freepointer_safe(struct kmem_cache *s, void *object)
{
	unsigned long freepointer_addr;
	void *p;

<<<<<<< HEAD
	if (!debug_pagealloc_enabled())
		return get_freepointer(s, object);

	freepointer_addr = (unsigned long)object + s->offset;
	probe_kernel_read(&p, (void **)freepointer_addr, sizeof(p));
=======
	if (!debug_pagealloc_enabled_static())
		return get_freepointer(s, object);

	object = kasan_reset_tag(object);
	freepointer_addr = (unsigned long)object + s->offset;
	copy_from_kernel_nofault(&p, (void **)freepointer_addr, sizeof(p));
>>>>>>> upstream/android-13
	return freelist_ptr(s, p, freepointer_addr);
}

static inline void set_freepointer(struct kmem_cache *s, void *object, void *fp)
{
	unsigned long freeptr_addr = (unsigned long)object + s->offset;
<<<<<<< HEAD
#ifdef CONFIG_KDP
	u64 key = 0;
#ifdef CONFIG_SLAB_FREELIST_HARDENED
	key = (u64)s->random;
#endif
#endif
=======
>>>>>>> upstream/android-13

#ifdef CONFIG_SLAB_FREELIST_HARDENED
	BUG_ON(object == fp); /* naive detection of double free or corruption */
#endif

<<<<<<< HEAD
#ifdef CONFIG_KDP
	if (kdp_enable && is_kdp_kmem_cache(s)) {
		uh_call(UH_APP_KDP, SET_FREEPTR, (u64)object, (u64)s->offset, (u64)fp,
				(u64)freelist_ptr(s, fp, freeptr_addr));
	} else
#endif
=======
	freeptr_addr = (unsigned long)kasan_reset_tag((void *)freeptr_addr);
>>>>>>> upstream/android-13
	*(void **)freeptr_addr = freelist_ptr(s, fp, freeptr_addr);
}

/* Loop over all objects in a slab */
#define for_each_object(__p, __s, __addr, __objects) \
	for (__p = fixup_red_left(__s, __addr); \
		__p < (__addr) + (__objects) * (__s)->size; \
		__p += (__s)->size)

<<<<<<< HEAD
/* Determine object index from a given position */
static inline unsigned int slab_index(void *p, struct kmem_cache *s, void *addr)
{
	return (kasan_reset_tag(p) - addr) / s->size;
}

=======
>>>>>>> upstream/android-13
static inline unsigned int order_objects(unsigned int order, unsigned int size)
{
	return ((unsigned int)PAGE_SIZE << order) / size;
}

static inline struct kmem_cache_order_objects oo_make(unsigned int order,
		unsigned int size)
{
	struct kmem_cache_order_objects x = {
		(order << OO_SHIFT) + order_objects(order, size)
	};

	return x;
}

static inline unsigned int oo_order(struct kmem_cache_order_objects x)
{
	return x.x >> OO_SHIFT;
}

static inline unsigned int oo_objects(struct kmem_cache_order_objects x)
{
	return x.x & OO_MASK;
}

/*
 * Per slab locking using the pagelock
 */
<<<<<<< HEAD
static __always_inline void slab_lock(struct page *page)
=======
static __always_inline void __slab_lock(struct page *page)
>>>>>>> upstream/android-13
{
	VM_BUG_ON_PAGE(PageTail(page), page);
	bit_spin_lock(PG_locked, &page->flags);
}

<<<<<<< HEAD
static __always_inline void slab_unlock(struct page *page)
=======
static __always_inline void __slab_unlock(struct page *page)
>>>>>>> upstream/android-13
{
	VM_BUG_ON_PAGE(PageTail(page), page);
	__bit_spin_unlock(PG_locked, &page->flags);
}

<<<<<<< HEAD
/* Interrupts must be disabled (for the fallback code to work right) */
=======
static __always_inline void slab_lock(struct page *page, unsigned long *flags)
{
	if (IS_ENABLED(CONFIG_PREEMPT_RT))
		local_irq_save(*flags);
	__slab_lock(page);
}

static __always_inline void slab_unlock(struct page *page, unsigned long *flags)
{
	__slab_unlock(page);
	if (IS_ENABLED(CONFIG_PREEMPT_RT))
		local_irq_restore(*flags);
}

/*
 * Interrupts must be disabled (for the fallback code to work right), typically
 * by an _irqsave() lock variant. Except on PREEMPT_RT where locks are different
 * so we disable interrupts as part of slab_[un]lock().
 */
>>>>>>> upstream/android-13
static inline bool __cmpxchg_double_slab(struct kmem_cache *s, struct page *page,
		void *freelist_old, unsigned long counters_old,
		void *freelist_new, unsigned long counters_new,
		const char *n)
{
<<<<<<< HEAD
	VM_BUG_ON(!irqs_disabled());
=======
	if (!IS_ENABLED(CONFIG_PREEMPT_RT))
		lockdep_assert_irqs_disabled();
>>>>>>> upstream/android-13
#if defined(CONFIG_HAVE_CMPXCHG_DOUBLE) && \
    defined(CONFIG_HAVE_ALIGNED_STRUCT_PAGE)
	if (s->flags & __CMPXCHG_DOUBLE) {
		if (cmpxchg_double(&page->freelist, &page->counters,
				   freelist_old, counters_old,
				   freelist_new, counters_new))
			return true;
	} else
#endif
	{
<<<<<<< HEAD
		slab_lock(page);
=======
		/* init to 0 to prevent spurious warnings */
		unsigned long flags = 0;

		slab_lock(page, &flags);
>>>>>>> upstream/android-13
		if (page->freelist == freelist_old &&
					page->counters == counters_old) {
			page->freelist = freelist_new;
			page->counters = counters_new;
<<<<<<< HEAD
			slab_unlock(page);
			return true;
		}
		slab_unlock(page);
=======
			slab_unlock(page, &flags);
			return true;
		}
		slab_unlock(page, &flags);
>>>>>>> upstream/android-13
	}

	cpu_relax();
	stat(s, CMPXCHG_DOUBLE_FAIL);

#ifdef SLUB_DEBUG_CMPXCHG
	pr_info("%s %s: cmpxchg double redo ", n, s->name);
#endif

	return false;
}

static inline bool cmpxchg_double_slab(struct kmem_cache *s, struct page *page,
		void *freelist_old, unsigned long counters_old,
		void *freelist_new, unsigned long counters_new,
		const char *n)
{
#if defined(CONFIG_HAVE_CMPXCHG_DOUBLE) && \
    defined(CONFIG_HAVE_ALIGNED_STRUCT_PAGE)
	if (s->flags & __CMPXCHG_DOUBLE) {
		if (cmpxchg_double(&page->freelist, &page->counters,
				   freelist_old, counters_old,
				   freelist_new, counters_new))
			return true;
	} else
#endif
	{
		unsigned long flags;

		local_irq_save(flags);
<<<<<<< HEAD
		slab_lock(page);
=======
		__slab_lock(page);
>>>>>>> upstream/android-13
		if (page->freelist == freelist_old &&
					page->counters == counters_old) {
			page->freelist = freelist_new;
			page->counters = counters_new;
<<<<<<< HEAD
			slab_unlock(page);
			local_irq_restore(flags);
			return true;
		}
		slab_unlock(page);
=======
			__slab_unlock(page);
			local_irq_restore(flags);
			return true;
		}
		__slab_unlock(page);
>>>>>>> upstream/android-13
		local_irq_restore(flags);
	}

	cpu_relax();
	stat(s, CMPXCHG_DOUBLE_FAIL);

#ifdef SLUB_DEBUG_CMPXCHG
	pr_info("%s %s: cmpxchg double redo ", n, s->name);
#endif

	return false;
}

#ifdef CONFIG_SLUB_DEBUG
<<<<<<< HEAD
=======
static unsigned long object_map[BITS_TO_LONGS(MAX_OBJS_PER_PAGE)];
static DEFINE_RAW_SPINLOCK(object_map_lock);

static void __fill_map(unsigned long *obj_map, struct kmem_cache *s,
		       struct page *page)
{
	void *addr = page_address(page);
	void *p;

	bitmap_zero(obj_map, page->objects);

	for (p = page->freelist; p; p = get_freepointer(s, p))
		set_bit(__obj_to_index(s, addr, p), obj_map);
}

static bool slab_add_kunit_errors(void)
{
	struct kunit_resource *resource;

	if (likely(!current->kunit_test))
		return false;

	resource = kunit_find_named_resource(current->kunit_test, "slab_errors");
	if (!resource)
		return false;

	(*(int *)resource->data)++;
	kunit_put_resource(resource);
	return true;
}

>>>>>>> upstream/android-13
/*
 * Determine a map of object in use on a page.
 *
 * Node listlock must be held to guarantee that the page does
 * not vanish from under us.
 */
<<<<<<< HEAD
static void get_map(struct kmem_cache *s, struct page *page, unsigned long *map)
{
	void *p;
	void *addr = page_address(page);

#ifdef CONFIG_KDP
	if (is_kdp_kmem_cache(s))
		return;
#endif
	for (p = page->freelist; p; p = get_freepointer(s, p))
		set_bit(slab_index(p, s, addr), map);
=======
static unsigned long *get_map(struct kmem_cache *s, struct page *page)
	__acquires(&object_map_lock)
{
	VM_BUG_ON(!irqs_disabled());

	raw_spin_lock(&object_map_lock);

	__fill_map(object_map, s, page);

	return object_map;
}

static void put_map(unsigned long *map) __releases(&object_map_lock)
{
	VM_BUG_ON(map != object_map);
	raw_spin_unlock(&object_map_lock);
>>>>>>> upstream/android-13
}

static inline unsigned int size_from_object(struct kmem_cache *s)
{
	if (s->flags & SLAB_RED_ZONE)
		return s->size - s->red_left_pad;

	return s->size;
}

static inline void *restore_red_left(struct kmem_cache *s, void *p)
{
	if (s->flags & SLAB_RED_ZONE)
		p -= s->red_left_pad;

	return p;
}

/*
 * Debug settings:
 */
#if defined(CONFIG_SLUB_DEBUG_ON)
static slab_flags_t slub_debug = DEBUG_DEFAULT_FLAGS;
#else
static slab_flags_t slub_debug;
#endif

<<<<<<< HEAD
static char *slub_debug_slabs;
=======
static char *slub_debug_string;
>>>>>>> upstream/android-13
static int disable_higher_order_debug;

/*
 * slub is about to manipulate internal object metadata.  This memory lies
 * outside the range of the allocated object, so accessing it would normally
 * be reported by kasan as a bounds error.  metadata_access_enable() is used
 * to tell kasan that these accesses are OK.
 */
static inline void metadata_access_enable(void)
{
	kasan_disable_current();
}

static inline void metadata_access_disable(void)
{
	kasan_enable_current();
}

/*
 * Object debugging
 */

/* Verify that a pointer has an address that is valid within a slab page */
static inline int check_valid_pointer(struct kmem_cache *s,
				struct page *page, void *object)
{
	void *base;

	if (!object)
		return 1;

	base = page_address(page);
	object = kasan_reset_tag(object);
	object = restore_red_left(s, object);
	if (object < base || object >= base + page->objects * s->size ||
		(object - base) % s->size) {
		return 0;
	}

	return 1;
}

static void print_section(char *level, char *text, u8 *addr,
			  unsigned int length)
{
	metadata_access_enable();
<<<<<<< HEAD
	print_hex_dump(level, text, DUMP_PREFIX_ADDRESS, 16, 1, addr,
			length, 1);
	metadata_access_disable();
}

=======
	print_hex_dump(level, text, DUMP_PREFIX_ADDRESS,
			16, 1, kasan_reset_tag((void *)addr), length, 1);
	metadata_access_disable();
}

/*
 * See comment in calculate_sizes().
 */
static inline bool freeptr_outside_object(struct kmem_cache *s)
{
	return s->offset >= s->inuse;
}

/*
 * Return offset of the end of info block which is inuse + free pointer if
 * not overlapping with object.
 */
static inline unsigned int get_info_end(struct kmem_cache *s)
{
	if (freeptr_outside_object(s))
		return s->inuse + sizeof(void *);
	else
		return s->inuse;
}

>>>>>>> upstream/android-13
static struct track *get_track(struct kmem_cache *s, void *object,
	enum track_item alloc)
{
	struct track *p;

<<<<<<< HEAD
	if (s->offset)
		p = object + s->offset + sizeof(void *);
	else
		p = object + s->inuse;

	return p + alloc;
}

=======
	p = object + get_info_end(s);

	return kasan_reset_tag(p + alloc);
}

/*
 * This function will be used to loop through all the slab objects in
 * a page to give track structure for each object, the function fn will
 * be using this track structure and extract required info into its private
 * data, the return value will be the number of track structures that are
 * processed.
 */
unsigned long get_each_object_track(struct kmem_cache *s,
		struct page *page, enum track_item alloc,
		int (*fn)(const struct kmem_cache *, const void *,
		const struct track *, void *), void *private)
{
	void *p;
	struct track *t;
	int ret;
	unsigned long num_track = 0;
	unsigned long flags = 0;

	if (!slub_debug || !(s->flags & SLAB_STORE_USER))
		return 0;

	slab_lock(page, &flags);
	for_each_object(p, s, page_address(page), page->objects) {
		t = get_track(s, p, alloc);
		metadata_access_enable();
		ret = fn(s, p, t, private);
		metadata_access_disable();
		if (ret < 0)
			break;
		num_track += 1;
	}
	slab_unlock(page, &flags);
	return num_track;
}
EXPORT_SYMBOL_NS_GPL(get_each_object_track, MINIDUMP);

>>>>>>> upstream/android-13
static void set_track(struct kmem_cache *s, void *object,
			enum track_item alloc, unsigned long addr)
{
	struct track *p = get_track(s, object, alloc);

<<<<<<< HEAD
#ifdef CONFIG_KDP
	if (is_kdp_kmem_cache(s))
		return;
#endif
	if (addr) {
#ifdef CONFIG_STACKTRACE
		struct stack_trace trace;
		int i;

		trace.nr_entries = 0;
		trace.max_entries = TRACK_ADDRS_COUNT;
		trace.entries = p->addrs;
		trace.skip = 3;
		metadata_access_enable();
		save_stack_trace(&trace);
		metadata_access_disable();

		/* See rant in lockdep.c */
		if (trace.nr_entries != 0 &&
		    trace.entries[trace.nr_entries - 1] == ULONG_MAX)
			trace.nr_entries--;

		for (i = trace.nr_entries; i < TRACK_ADDRS_COUNT; i++)
			p->addrs[i] = 0;
=======
	if (addr) {
#ifdef CONFIG_STACKTRACE
		unsigned int nr_entries;

		metadata_access_enable();
		nr_entries = stack_trace_save(kasan_reset_tag(p->addrs),
					      TRACK_ADDRS_COUNT, 3);
		metadata_access_disable();

		if (nr_entries < TRACK_ADDRS_COUNT)
			p->addrs[nr_entries] = 0;
>>>>>>> upstream/android-13
#endif
		p->addr = addr;
		p->cpu = smp_processor_id();
		p->pid = current->pid;
		p->when = jiffies;
<<<<<<< HEAD
	} else
		memset(p, 0, sizeof(struct track));
=======
	} else {
		memset(p, 0, sizeof(struct track));
	}
>>>>>>> upstream/android-13
}

static void init_tracking(struct kmem_cache *s, void *object)
{
	if (!(s->flags & SLAB_STORE_USER))
		return;

	set_track(s, object, TRACK_FREE, 0UL);
	set_track(s, object, TRACK_ALLOC, 0UL);
}

static void print_track(const char *s, struct track *t, unsigned long pr_time)
{
	if (!t->addr)
		return;

<<<<<<< HEAD
#ifdef CONFIG_SEC_DEBUG_AUTO_COMMENT
	pr_auto(ASL7, "INFO: %s in %pS age=%lu cpu=%u pid=%d\n",
	       s, (void *)t->addr, pr_time - t->when, t->cpu, t->pid);
#else
	pr_err("INFO: %s in %pS age=%lu cpu=%u pid=%d\n",
	       s, (void *)t->addr, pr_time - t->when, t->cpu, t->pid);
#endif

=======
	pr_auto(ASL7, "%s in %pS age=%lu cpu=%u pid=%d\n",
	       s, (void *)t->addr, pr_time - t->when, t->cpu, t->pid);
>>>>>>> upstream/android-13
#ifdef CONFIG_STACKTRACE
	{
		int i;
		for (i = 0; i < TRACK_ADDRS_COUNT; i++)
			if (t->addrs[i])
				pr_err("\t%pS\n", (void *)t->addrs[i]);
			else
				break;
	}
#endif
}

<<<<<<< HEAD
static void print_tracking(struct kmem_cache *s, void *object)
=======
void print_tracking(struct kmem_cache *s, void *object)
>>>>>>> upstream/android-13
{
	unsigned long pr_time = jiffies;
	if (!(s->flags & SLAB_STORE_USER))
		return;

	print_track("Allocated", get_track(s, object, TRACK_ALLOC), pr_time);
	print_track("Freed", get_track(s, object, TRACK_FREE), pr_time);
}

static void print_page_info(struct page *page)
{
<<<<<<< HEAD
	pr_err("INFO: Slab 0x%px objects=%u used=%u fp=0x%px flags=0x%04lx\n",
	       page, page->objects, page->inuse, page->freelist, page->flags);
=======
	pr_err("Slab 0x%p objects=%u used=%u fp=0x%p flags=%#lx(%pGp)\n",
	       page, page->objects, page->inuse, page->freelist,
	       page->flags, &page->flags);
>>>>>>> upstream/android-13

}

static void slab_bug(struct kmem_cache *s, char *fmt, ...)
{
	struct va_format vaf;
	va_list args;

	va_start(args, fmt);
	vaf.fmt = fmt;
	vaf.va = &args;
<<<<<<< HEAD

#ifdef CONFIG_SEC_DEBUG_AUTO_COMMENT
	pr_auto(ASL7, "=============================================================================\n");
	pr_auto(ASL7, "BUG %s (%s): %pV\n", s->name, print_tainted(), &vaf);
	pr_auto(ASL7, "-----------------------------------------------------------------------------\n\n");
#else
	pr_err("=============================================================================\n");
	pr_err("BUG %s (%s): %pV\n", s->name, print_tainted(), &vaf);
	pr_err("-----------------------------------------------------------------------------\n\n");
#endif

	add_taint(TAINT_BAD_PAGE, LOCKDEP_NOW_UNRELIABLE);
	va_end(args);
}

=======
	pr_auto(ASL7, "=============================================================================\n");
	pr_auto(ASL7, "BUG %s (%s): %pV\n", s->name, print_tainted(), &vaf);
	pr_auto(ASL7, "-----------------------------------------------------------------------------\n\n");
	va_end(args);
}

__printf(2, 3)
>>>>>>> upstream/android-13
static void slab_fix(struct kmem_cache *s, char *fmt, ...)
{
	struct va_format vaf;
	va_list args;

<<<<<<< HEAD
=======
	if (slab_add_kunit_errors())
		return;

>>>>>>> upstream/android-13
	va_start(args, fmt);
	vaf.fmt = fmt;
	vaf.va = &args;
	pr_err("FIX %s: %pV\n", s->name, &vaf);
	va_end(args);
}

static bool freelist_corrupted(struct kmem_cache *s, struct page *page,
			       void **freelist, void *nextfree)
{
	if ((s->flags & SLAB_CONSISTENCY_CHECKS) &&
	    !check_valid_pointer(s, page, nextfree) && freelist) {
		object_err(s, page, *freelist, "Freechain corrupt");
		*freelist = NULL;
		slab_fix(s, "Isolate corrupted freechain");
		return true;
	}

	return false;
}

static void print_trailer(struct kmem_cache *s, struct page *page, u8 *p)
{
	unsigned int off;	/* Offset of last byte */
	u8 *addr = page_address(page);

	print_tracking(s, p);

	print_page_info(page);

<<<<<<< HEAD
#ifdef CONFIG_SEC_DEBUG_AUTO_COMMENT
	pr_auto(ASL7, "INFO: Object 0x%px @offset=%tu fp=0x%px\n\n",
		   p, p - addr, get_freepointer(s, p));
#else
	pr_err("INFO: Object 0x%px @offset=%tu fp=0x%px\n\n",
	       p, p - addr, get_freepointer(s, p));
#endif

	if (s->flags & SLAB_RED_ZONE)
		print_section(KERN_ERR, "Redzone ", p - s->red_left_pad,
=======
	pr_auto(ASL7, "Object 0x%p @offset=%tu fp=0x%p\n\n",
	       p, p - addr, get_freepointer(s, p));

	if (s->flags & SLAB_RED_ZONE)
		print_section(KERN_ERR, "Redzone  ", p - s->red_left_pad,
>>>>>>> upstream/android-13
			      s->red_left_pad);
	else if (p > addr + 16)
		print_section(KERN_ERR, "Bytes b4 ", p - 16, 16);

<<<<<<< HEAD
	print_section(KERN_ERR, "Object ", p,
		      min_t(unsigned int, s->object_size, PAGE_SIZE));
	if (s->flags & SLAB_RED_ZONE)
		print_section(KERN_ERR, "Redzone ", p + s->object_size,
			s->inuse - s->object_size);

	if (s->offset)
		off = s->offset + sizeof(void *);
	else
		off = s->inuse;
=======
	print_section(KERN_ERR,         "Object   ", p,
		      min_t(unsigned int, s->object_size, PAGE_SIZE));
	if (s->flags & SLAB_RED_ZONE)
		print_section(KERN_ERR, "Redzone  ", p + s->object_size,
			s->inuse - s->object_size);

	off = get_info_end(s);
>>>>>>> upstream/android-13

	if (s->flags & SLAB_STORE_USER)
		off += 2 * sizeof(struct track);

	off += kasan_metadata_size(s);

	if (off != size_from_object(s))
		/* Beginning of the filler is the free pointer */
<<<<<<< HEAD
		print_section(KERN_ERR, "Padding ", p + off,
			      size_from_object(s) - off);

	WARN_ON(1);
=======
		print_section(KERN_ERR, "Padding  ", p + off,
			      size_from_object(s) - off);

	dump_stack();
>>>>>>> upstream/android-13
}

void object_err(struct kmem_cache *s, struct page *page,
			u8 *object, char *reason)
{
<<<<<<< HEAD
#ifdef CONFIG_SEC_DEBUG_AUTO_COMMENT
	pr_auto_once(7);
#endif

	slab_bug(s, "%s", reason);
	print_trailer(s, page, object);

#ifdef CONFIG_SEC_DEBUG_AUTO_COMMENT
	pr_auto_disable(7);
#endif
=======
	if (slab_add_kunit_errors())
		return;

	pr_auto_once(7);
	slab_bug(s, "%s", reason);
	print_trailer(s, page, object);
	add_taint(TAINT_BAD_PAGE, LOCKDEP_NOW_UNRELIABLE);
	pr_auto_disable(7);
>>>>>>> upstream/android-13
}

static __printf(3, 4) void slab_err(struct kmem_cache *s, struct page *page,
			const char *fmt, ...)
{
	va_list args;
	char buf[100];

<<<<<<< HEAD
#ifdef CONFIG_SEC_DEBUG_AUTO_COMMENT
	pr_auto_once(7);
#endif
=======
	if (slab_add_kunit_errors())
		return;

	pr_auto_once(7);
>>>>>>> upstream/android-13
	va_start(args, fmt);
	vsnprintf(buf, sizeof(buf), fmt, args);
	va_end(args);
	slab_bug(s, "%s", buf);
	print_page_info(page);
<<<<<<< HEAD
	WARN_ON(1);
#ifdef CONFIG_SEC_DEBUG_AUTO_COMMENT
	pr_auto_disable(7);
#endif
=======
	dump_stack();
	add_taint(TAINT_BAD_PAGE, LOCKDEP_NOW_UNRELIABLE);
	pr_auto_disable(7);
>>>>>>> upstream/android-13
}

static void init_object(struct kmem_cache *s, void *object, u8 val)
{
<<<<<<< HEAD
	u8 *p = object;

#ifdef CONFIG_KDP
	if (is_kdp_kmem_cache(s))
		return;
#endif
=======
	u8 *p = kasan_reset_tag(object);

>>>>>>> upstream/android-13
	if (s->flags & SLAB_RED_ZONE)
		memset(p - s->red_left_pad, val, s->red_left_pad);

	if (s->flags & __OBJECT_POISON) {
		memset(p, POISON_FREE, s->object_size - 1);
		p[s->object_size - 1] = POISON_END;
	}

	if (s->flags & SLAB_RED_ZONE)
		memset(p + s->object_size, val, s->inuse - s->object_size);
}

static void restore_bytes(struct kmem_cache *s, char *message, u8 data,
						void *from, void *to)
{
<<<<<<< HEAD
	slab_fix(s, "Restoring 0x%px-0x%px=0x%x\n", from, to - 1, data);
=======
	/* for debugging bitflip, we don't restore slub padding value. */
	BUG_ON(1);
	slab_fix(s, "Restoring %s 0x%p-0x%p=0x%x", message, from, to - 1, data);
>>>>>>> upstream/android-13
	memset(from, data, to - from);
}

static int check_bytes_and_report(struct kmem_cache *s, struct page *page,
			u8 *object, char *what,
			u8 *start, unsigned int value, unsigned int bytes)
{
	u8 *fault;
	u8 *end;
	u8 *addr = page_address(page);

	metadata_access_enable();
<<<<<<< HEAD
#ifdef CONFIG_KDP
	if (is_kdp_kmem_cache(s))
		return 1;
#endif
	fault = memchr_inv(start, value, bytes);
=======
	fault = memchr_inv(kasan_reset_tag(start), value, bytes);
>>>>>>> upstream/android-13
	metadata_access_disable();
	if (!fault)
		return 1;

	end = start + bytes;
	while (end > fault && end[-1] == value)
		end--;

<<<<<<< HEAD
#ifdef CONFIG_SEC_DEBUG_AUTO_COMMENT
	pr_auto_once(7);
	slab_bug(s, "%s overwritten", what);
	pr_auto(ASL7, "INFO: 0x%px-0x%px @offset=%tu. First byte 0x%x instead of 0x%x\n",
						fault, end - 1, fault - addr,
						fault[0], value);
	print_trailer(s, page, object);
	pr_auto_disable(7);
#else
	slab_bug(s, "%s overwritten", what);
	pr_err("INFO: 0x%px-0x%px @offset=%tu. First byte 0x%x instead of 0x%x\n",
					fault, end - 1, fault - addr,
					fault[0], value);
	print_trailer(s, page, object);
#endif

#ifdef CONFIG_SEC_DEBUG_BUG_ON_SLUB_CORRUPTION
	BUG();
#endif
=======
	if (slab_add_kunit_errors())
		goto skip_bug_print;

	pr_auto_once(7);
	slab_bug(s, "%s overwritten", what);
	pr_auto(ASL7, "0x%p-0x%p @offset=%tu. First byte 0x%x instead of 0x%x\n",
					fault, end - 1, fault - addr,
					fault[0], value);
	print_trailer(s, page, object);
	add_taint(TAINT_BAD_PAGE, LOCKDEP_NOW_UNRELIABLE);
	pr_auto_disable(7);
	secdbg_slub_bug();

skip_bug_print:
>>>>>>> upstream/android-13
	restore_bytes(s, what, value, fault, end);
	return 0;
}

/*
 * Object layout:
 *
 * object address
 * 	Bytes of the object to be managed.
 * 	If the freepointer may overlay the object then the free
<<<<<<< HEAD
 * 	pointer is the first word of the object.
=======
 *	pointer is at the middle of the object.
>>>>>>> upstream/android-13
 *
 * 	Poisoning uses 0x6b (POISON_FREE) and the last byte is
 * 	0xa5 (POISON_END)
 *
 * object + s->object_size
 * 	Padding to reach word boundary. This is also used for Redzoning.
 * 	Padding is extended by another word if Redzoning is enabled and
 * 	object_size == inuse.
 *
 * 	We fill with 0xbb (RED_INACTIVE) for inactive objects and with
 * 	0xcc (RED_ACTIVE) for objects in use.
 *
 * object + s->inuse
 * 	Meta data starts here.
 *
 * 	A. Free pointer (if we cannot overwrite object on free)
 * 	B. Tracking data for SLAB_STORE_USER
<<<<<<< HEAD
 * 	C. Padding to reach required alignment boundary or at mininum
=======
 *	C. Padding to reach required alignment boundary or at minimum
>>>>>>> upstream/android-13
 * 		one word if debugging is on to be able to detect writes
 * 		before the word boundary.
 *
 *	Padding is done using 0x5a (POISON_INUSE)
 *
 * object + s->size
 * 	Nothing is used beyond s->size.
 *
 * If slabcaches are merged then the object_size and inuse boundaries are mostly
 * ignored. And therefore no slab options that rely on these boundaries
 * may be used with merged slabcaches.
 */

static int check_pad_bytes(struct kmem_cache *s, struct page *page, u8 *p)
{
<<<<<<< HEAD
	unsigned long off = s->inuse;	/* The end of info */

	if (s->offset)
		/* Freepointer is placed after the object. */
		off += sizeof(void *);
=======
	unsigned long off = get_info_end(s);	/* The end of info */
>>>>>>> upstream/android-13

	if (s->flags & SLAB_STORE_USER)
		/* We also have user information there */
		off += 2 * sizeof(struct track);

	off += kasan_metadata_size(s);

	if (size_from_object(s) == off)
		return 1;

	return check_bytes_and_report(s, page, p, "Object padding",
			p + off, POISON_INUSE, size_from_object(s) - off);
}

/* Check the pad bytes at the end of a slab page */
static int slab_pad_check(struct kmem_cache *s, struct page *page)
{
	u8 *start;
	u8 *fault;
	u8 *end;
	u8 *pad;
	int length;
	int remainder;

	if (!(s->flags & SLAB_POISON))
		return 1;

<<<<<<< HEAD
#ifdef CONFIG_KDP
	if (is_kdp_kmem_cache(s))
		return 1;
#endif
	start = page_address(page);
	length = PAGE_SIZE << compound_order(page);
=======
	start = page_address(page);
	length = page_size(page);
>>>>>>> upstream/android-13
	end = start + length;
	remainder = length % s->size;
	if (!remainder)
		return 1;

	pad = end - remainder;
	metadata_access_enable();
<<<<<<< HEAD
	fault = memchr_inv(pad, POISON_INUSE, remainder);
=======
	fault = memchr_inv(kasan_reset_tag(pad), POISON_INUSE, remainder);
>>>>>>> upstream/android-13
	metadata_access_disable();
	if (!fault)
		return 1;
	while (end > fault && end[-1] == POISON_INUSE)
		end--;

<<<<<<< HEAD
	slab_err(s, page, "Padding overwritten. 0x%px-0x%px @offset=%tu",
			fault, end - 1, fault - start);
	print_section(KERN_ERR, "Padding ", pad, remainder);

#ifdef CONFIG_SEC_DEBUG_BUG_ON_SLUB_CORRUPTION
	BUG();
#endif
=======
	slab_err(s, page, "Padding overwritten. 0x%p-0x%p @offset=%tu",
			fault, end - 1, fault - start);
	print_section(KERN_ERR, "Padding ", pad, remainder);
	secdbg_slub_bug();
>>>>>>> upstream/android-13

	restore_bytes(s, "slab padding", POISON_INUSE, fault, end);
	return 0;
}

static int check_object(struct kmem_cache *s, struct page *page,
					void *object, u8 val)
{
	u8 *p = object;
	u8 *endobject = object + s->object_size;

	if (s->flags & SLAB_RED_ZONE) {
<<<<<<< HEAD
		if (!check_bytes_and_report(s, page, object, "Redzone",
			object - s->red_left_pad, val, s->red_left_pad))
			return 0;

		if (!check_bytes_and_report(s, page, object, "Redzone",
=======
		if (!check_bytes_and_report(s, page, object, "Left Redzone",
			object - s->red_left_pad, val, s->red_left_pad))
			return 0;

		if (!check_bytes_and_report(s, page, object, "Right Redzone",
>>>>>>> upstream/android-13
			endobject, val, s->inuse - s->object_size))
			return 0;
	} else {
		if ((s->flags & SLAB_POISON) && s->object_size < s->inuse) {
			check_bytes_and_report(s, page, p, "Alignment padding",
				endobject, POISON_INUSE,
				s->inuse - s->object_size);
		}
	}

	if (s->flags & SLAB_POISON) {
		if (val != SLUB_RED_ACTIVE && (s->flags & __OBJECT_POISON) &&
			(!check_bytes_and_report(s, page, p, "Poison", p,
					POISON_FREE, s->object_size - 1) ||
<<<<<<< HEAD
			 !check_bytes_and_report(s, page, p, "Poison",
=======
			 !check_bytes_and_report(s, page, p, "End Poison",
>>>>>>> upstream/android-13
				p + s->object_size - 1, POISON_END, 1)))
			return 0;
		/*
		 * check_pad_bytes cleans up on its own.
		 */
		check_pad_bytes(s, page, p);
	}

<<<<<<< HEAD
	if (!s->offset && val == SLUB_RED_ACTIVE)
=======
	if (!freeptr_outside_object(s) && val == SLUB_RED_ACTIVE)
>>>>>>> upstream/android-13
		/*
		 * Object and freepointer overlap. Cannot check
		 * freepointer while object is allocated.
		 */
		return 1;

	/* Check free pointer validity */
	if (!check_valid_pointer(s, page, get_freepointer(s, p))) {
		object_err(s, page, p, "Freepointer corrupt");
<<<<<<< HEAD
#ifdef CONFIG_SEC_DEBUG_BUG_ON_SLUB_CORRUPTION
		BUG();
#endif
=======
		secdbg_slub_bug();
>>>>>>> upstream/android-13
		/*
		 * No choice but to zap it and thus lose the remainder
		 * of the free objects in this slab. May cause
		 * another error because the object count is now wrong.
		 */
		set_freepointer(s, p, NULL);
		return 0;
	}
	return 1;
}

static int check_slab(struct kmem_cache *s, struct page *page)
{
	int maxobj;

<<<<<<< HEAD
	VM_BUG_ON(!irqs_disabled());

=======
>>>>>>> upstream/android-13
	if (!PageSlab(page)) {
		slab_err(s, page, "Not a valid slab page");
		return 0;
	}

<<<<<<< HEAD
#ifdef CONFIG_KDP
	/* Skip this function for now */
	if (is_kdp_kmem_cache(s))
		return 1;
#endif
=======
>>>>>>> upstream/android-13
	maxobj = order_objects(compound_order(page), s->size);
	if (page->objects > maxobj) {
		slab_err(s, page, "objects %u > max %u",
			page->objects, maxobj);
		return 0;
	}
	if (page->inuse > page->objects) {
		slab_err(s, page, "inuse %u > max %u",
			page->inuse, page->objects);
		return 0;
	}
	/* Slab_pad_check fixes things up after itself */
	slab_pad_check(s, page);
	return 1;
}

/*
 * Determine if a certain object on a page is on the freelist. Must hold the
 * slab lock to guarantee that the chains are in a consistent state.
 */
static int on_freelist(struct kmem_cache *s, struct page *page, void *search)
{
	int nr = 0;
	void *fp;
	void *object = NULL;
	int max_objects;

	fp = page->freelist;
<<<<<<< HEAD
#ifdef CONFIG_KDP
	if (is_kdp_kmem_cache(s))
		return 0;
#endif
=======
>>>>>>> upstream/android-13
	while (fp && nr <= page->objects) {
		if (fp == search)
			return 1;
		if (!check_valid_pointer(s, page, fp)) {
			if (object) {
				object_err(s, page, object,
					"Freechain corrupt");
<<<<<<< HEAD
#ifdef CONFIG_SEC_DEBUG_BUG_ON_SLUB_CORRUPTION
				BUG();
#endif
				set_freepointer(s, object, NULL);
			} else {
				slab_err(s, page, "Freepointer corrupt");
#ifdef CONFIG_SEC_DEBUG_BUG_ON_SLUB_CORRUPTION
				BUG();
#endif
=======
				secdbg_slub_bug();
				set_freepointer(s, object, NULL);
			} else {
				slab_err(s, page, "Freepointer corrupt");
				secdbg_slub_bug();
>>>>>>> upstream/android-13
				page->freelist = NULL;
				page->inuse = page->objects;
				slab_fix(s, "Freelist cleared");
				return 0;
			}
			break;
		}
		object = fp;
		fp = get_freepointer(s, object);
		nr++;
	}

	max_objects = order_objects(compound_order(page), s->size);
	if (max_objects > MAX_OBJS_PER_PAGE)
		max_objects = MAX_OBJS_PER_PAGE;

	if (page->objects != max_objects) {
		slab_err(s, page, "Wrong number of objects. Found %d but should be %d",
			 page->objects, max_objects);
<<<<<<< HEAD
#ifdef CONFIG_SEC_DEBUG_BUG_ON_SLUB_CORRUPTION
		BUG();
#endif
		page->objects = max_objects;
		slab_fix(s, "Number of objects adjusted.");
=======
		secdbg_slub_bug();
		page->objects = max_objects;
		slab_fix(s, "Number of objects adjusted");
>>>>>>> upstream/android-13
	}
	if (page->inuse != page->objects - nr) {
		slab_err(s, page, "Wrong object count. Counter is %d but counted were %d",
			 page->inuse, page->objects - nr);
<<<<<<< HEAD
#ifdef CONFIG_SEC_DEBUG_BUG_ON_SLUB_CORRUPTION
		BUG();
#endif
		page->inuse = page->objects - nr;
		slab_fix(s, "Object count adjusted.");
=======
		secdbg_slub_bug();
		page->inuse = page->objects - nr;
		slab_fix(s, "Object count adjusted");
>>>>>>> upstream/android-13
	}
	return search == NULL;
}

static void trace(struct kmem_cache *s, struct page *page, void *object,
								int alloc)
{
	if (s->flags & SLAB_TRACE) {
<<<<<<< HEAD
		pr_info("TRACE %s %s 0x%px inuse=%d fp=0x%px\n",
=======
		pr_info("TRACE %s %s 0x%p inuse=%d fp=0x%p\n",
>>>>>>> upstream/android-13
			s->name,
			alloc ? "alloc" : "free",
			object, page->inuse,
			page->freelist);

		if (!alloc)
			print_section(KERN_INFO, "Object ", (void *)object,
					s->object_size);

<<<<<<< HEAD
		WARN_ON(1);
=======
		dump_stack();
>>>>>>> upstream/android-13
	}
}

/*
 * Tracking of fully allocated slabs for debugging purposes.
 */
static void add_full(struct kmem_cache *s,
	struct kmem_cache_node *n, struct page *page)
{
<<<<<<< HEAD
#ifdef CONFIG_KDP
	if (is_kdp_kmem_cache(s))
		return;
#endif
=======
>>>>>>> upstream/android-13
	if (!(s->flags & SLAB_STORE_USER))
		return;

	lockdep_assert_held(&n->list_lock);
<<<<<<< HEAD
	list_add(&page->lru, &n->full);
=======
	list_add(&page->slab_list, &n->full);
>>>>>>> upstream/android-13
}

static void remove_full(struct kmem_cache *s, struct kmem_cache_node *n, struct page *page)
{
<<<<<<< HEAD
#ifdef CONFIG_KDP
	if (is_kdp_kmem_cache(s))
		return;
#endif
=======
>>>>>>> upstream/android-13
	if (!(s->flags & SLAB_STORE_USER))
		return;

	lockdep_assert_held(&n->list_lock);
<<<<<<< HEAD
	list_del(&page->lru);
=======
	list_del(&page->slab_list);
>>>>>>> upstream/android-13
}

/* Tracking of the number of slabs for debugging purposes */
static inline unsigned long slabs_node(struct kmem_cache *s, int node)
{
	struct kmem_cache_node *n = get_node(s, node);

	return atomic_long_read(&n->nr_slabs);
}

static inline unsigned long node_nr_slabs(struct kmem_cache_node *n)
{
	return atomic_long_read(&n->nr_slabs);
}

static inline void inc_slabs_node(struct kmem_cache *s, int node, int objects)
{
	struct kmem_cache_node *n = get_node(s, node);

	/*
	 * May be called early in order to allocate a slab for the
	 * kmem_cache_node structure. Solve the chicken-egg
	 * dilemma by deferring the increment of the count during
	 * bootstrap (see early_kmem_cache_node_alloc).
	 */
	if (likely(n)) {
		atomic_long_inc(&n->nr_slabs);
		atomic_long_add(objects, &n->total_objects);
	}
}
static inline void dec_slabs_node(struct kmem_cache *s, int node, int objects)
{
	struct kmem_cache_node *n = get_node(s, node);

	atomic_long_dec(&n->nr_slabs);
	atomic_long_sub(objects, &n->total_objects);
}

/* Object debug checks for alloc/free paths */
static void setup_object_debug(struct kmem_cache *s, struct page *page,
								void *object)
{
<<<<<<< HEAD
	if (!(s->flags & (SLAB_STORE_USER|SLAB_RED_ZONE|__OBJECT_POISON)))
=======
	if (!kmem_cache_debug_flags(s, SLAB_STORE_USER|SLAB_RED_ZONE|__OBJECT_POISON))
>>>>>>> upstream/android-13
		return;

	init_object(s, object, SLUB_RED_INACTIVE);
	init_tracking(s, object);
}

<<<<<<< HEAD
static void setup_page_debug(struct kmem_cache *s, void *addr, int order)
{
	if (!(s->flags & SLAB_POISON))
		return;

	metadata_access_enable();
	memset(addr, POISON_INUSE, PAGE_SIZE << order);
=======
static
void setup_page_debug(struct kmem_cache *s, struct page *page, void *addr)
{
	if (!kmem_cache_debug_flags(s, SLAB_POISON))
		return;

	metadata_access_enable();
	memset(kasan_reset_tag(addr), POISON_INUSE, page_size(page));
>>>>>>> upstream/android-13
	metadata_access_disable();
}

static inline int alloc_consistency_checks(struct kmem_cache *s,
<<<<<<< HEAD
					struct page *page,
					void *object, unsigned long addr)
{
#ifdef CONFIG_KDP
	if (is_kdp_kmem_cache(s))
		return 0;
#endif
=======
					struct page *page, void *object)
{
>>>>>>> upstream/android-13
	if (!check_slab(s, page))
		return 0;

	if (!check_valid_pointer(s, page, object)) {
		object_err(s, page, object, "Freelist Pointer check fails");
		return 0;
	}

	if (!check_object(s, page, object, SLUB_RED_INACTIVE))
		return 0;

	return 1;
}

static noinline int alloc_debug_processing(struct kmem_cache *s,
					struct page *page,
					void *object, unsigned long addr)
{
	if (s->flags & SLAB_CONSISTENCY_CHECKS) {
<<<<<<< HEAD
		if (!alloc_consistency_checks(s, page, object, addr))
=======
		if (!alloc_consistency_checks(s, page, object))
>>>>>>> upstream/android-13
			goto bad;
	}

	/* Success perform special debug activities for allocs */
	if (s->flags & SLAB_STORE_USER)
		set_track(s, object, TRACK_ALLOC, addr);
	trace(s, page, object, 1);
	init_object(s, object, SLUB_RED_ACTIVE);
	return 1;

bad:
<<<<<<< HEAD
#ifdef CONFIG_SEC_DEBUG_BUG_ON_SLUB_CORRUPTION
	BUG();
#endif
=======
	secdbg_slub_bug();
>>>>>>> upstream/android-13
	if (PageSlab(page)) {
		/*
		 * If this is a slab page then lets do the best we can
		 * to avoid issues in the future. Marking all objects
		 * as used avoids touching the remaining objects.
		 */
		slab_fix(s, "Marking all objects used");
		page->inuse = page->objects;
		page->freelist = NULL;
	}
	return 0;
}

static inline int free_consistency_checks(struct kmem_cache *s,
		struct page *page, void *object, unsigned long addr)
{
	if (!check_valid_pointer(s, page, object)) {
<<<<<<< HEAD
		slab_err(s, page, "Invalid object pointer 0x%px", object);
=======
		slab_err(s, page, "Invalid object pointer 0x%p", object);
>>>>>>> upstream/android-13
		return 0;
	}

	if (on_freelist(s, page, object)) {
		object_err(s, page, object, "Object already free");
		return 0;
	}

	if (!check_object(s, page, object, SLUB_RED_ACTIVE))
		return 0;

	if (unlikely(s != page->slab_cache)) {
		if (!PageSlab(page)) {
<<<<<<< HEAD
			slab_err(s, page, "Attempt to free object(0x%px) outside of slab",
				 object);
		} else if (!page->slab_cache) {
			pr_err("SLUB <none>: no slab for object 0x%px.\n",
=======
			slab_err(s, page, "Attempt to free object(0x%p) outside of slab",
				 object);
		} else if (!page->slab_cache) {
			pr_err("SLUB <none>: no slab for object 0x%p.\n",
>>>>>>> upstream/android-13
			       object);
			dump_stack();
		} else
			object_err(s, page, object,
					"page slab pointer corrupt.");
		return 0;
	}
	return 1;
}

/* Supports checking bulk free of a constructed freelist */
static noinline int free_debug_processing(
	struct kmem_cache *s, struct page *page,
	void *head, void *tail, int bulk_cnt,
	unsigned long addr)
{
	struct kmem_cache_node *n = get_node(s, page_to_nid(page));
	void *object = head;
	int cnt = 0;
<<<<<<< HEAD
	unsigned long uninitialized_var(flags);
	int ret = 0;

#ifdef CONFIG_KDP
	if (is_kdp_kmem_cache(s))
		return 0;
#endif
	spin_lock_irqsave(&n->list_lock, flags);
	slab_lock(page);
=======
	unsigned long flags, flags2;
	int ret = 0;

	spin_lock_irqsave(&n->list_lock, flags);
	slab_lock(page, &flags2);
>>>>>>> upstream/android-13

	if (s->flags & SLAB_CONSISTENCY_CHECKS) {
		if (!check_slab(s, page))
			goto out;
	}

next_object:
	cnt++;

	if (s->flags & SLAB_CONSISTENCY_CHECKS) {
		if (!free_consistency_checks(s, page, object, addr))
			goto out;
	}

	if (s->flags & SLAB_STORE_USER)
		set_track(s, object, TRACK_FREE, addr);
	trace(s, page, object, 0);
	/* Freepointer not overwritten by init_object(), SLAB_POISON moved it */
	init_object(s, object, SLUB_RED_INACTIVE);

	/* Reached end of constructed freelist yet? */
	if (object != tail) {
		object = get_freepointer(s, object);
		goto next_object;
	}
	ret = 1;

out:
	if (cnt != bulk_cnt) {
		slab_err(s, page, "Bulk freelist count(%d) invalid(%d)\n",
			 bulk_cnt, cnt);
<<<<<<< HEAD
#ifdef CONFIG_SEC_DEBUG_BUG_ON_SLUB_CORRUPTION
		BUG();
#endif
	}

	slab_unlock(page);
	spin_unlock_irqrestore(&n->list_lock, flags);
	if (!ret) {
#ifdef CONFIG_SEC_DEBUG_BUG_ON_SLUB_CORRUPTION
		BUG();
#endif
=======
		secdbg_slub_bug();
	}

	slab_unlock(page, &flags2);
	spin_unlock_irqrestore(&n->list_lock, flags);
	if (!ret) {
		secdbg_slub_bug();
>>>>>>> upstream/android-13
		slab_fix(s, "Object at 0x%p not freed", object);
	}
	return ret;
}

<<<<<<< HEAD
static int __init setup_slub_debug(char *str)
{
	slub_debug = DEBUG_DEFAULT_FLAGS;
	if (*str++ != '=' || !*str)
		/*
		 * No options specified. Switch on full debugging.
		 */
		goto out;

	if (*str == ',')
=======
/*
 * Parse a block of slub_debug options. Blocks are delimited by ';'
 *
 * @str:    start of block
 * @flags:  returns parsed flags, or DEBUG_DEFAULT_FLAGS if none specified
 * @slabs:  return start of list of slabs, or NULL when there's no list
 * @init:   assume this is initial parsing and not per-kmem-create parsing
 *
 * returns the start of next block if there's any, or NULL
 */
static char *
parse_slub_debug_flags(char *str, slab_flags_t *flags, char **slabs, bool init)
{
	bool higher_order_disable = false;

	/* Skip any completely empty blocks */
	while (*str && *str == ';')
		str++;

	if (*str == ',') {
>>>>>>> upstream/android-13
		/*
		 * No options but restriction on slabs. This means full
		 * debugging for slabs matching a pattern.
		 */
<<<<<<< HEAD
		goto check_slabs;

	slub_debug = 0;
	if (*str == '-')
		/*
		 * Switch off all debugging measures.
		 */
		goto out;

	/*
	 * Determine which debug features should be switched on
	 */
	for (; *str && *str != ','; str++) {
		switch (tolower(*str)) {
		case 'f':
			slub_debug |= SLAB_CONSISTENCY_CHECKS;
			break;
		case 'z':
			slub_debug |= SLAB_RED_ZONE;
			break;
		case 'p':
			slub_debug |= SLAB_POISON;
			break;
		case 'u':
			slub_debug |= SLAB_STORE_USER;
			break;
		case 't':
			slub_debug |= SLAB_TRACE;
			break;
		case 'a':
			slub_debug |= SLAB_FAILSLAB;
=======
		*flags = DEBUG_DEFAULT_FLAGS;
		goto check_slabs;
	}
	*flags = 0;

	/* Determine which debug features should be switched on */
	for (; *str && *str != ',' && *str != ';'; str++) {
		switch (tolower(*str)) {
		case '-':
			*flags = 0;
			break;
		case 'f':
			*flags |= SLAB_CONSISTENCY_CHECKS;
			break;
		case 'z':
			*flags |= SLAB_RED_ZONE;
			break;
		case 'p':
			*flags |= SLAB_POISON;
			break;
		case 'u':
			*flags |= SLAB_STORE_USER;
			break;
		case 't':
			*flags |= SLAB_TRACE;
			break;
		case 'a':
			*flags |= SLAB_FAILSLAB;
>>>>>>> upstream/android-13
			break;
		case 'o':
			/*
			 * Avoid enabling debugging on caches if its minimum
			 * order would increase as a result.
			 */
<<<<<<< HEAD
			disable_higher_order_debug = 1;
			break;
		default:
			pr_err("slub_debug option '%c' unknown. skipped\n",
			       *str);
		}
	}

check_slabs:
	if (*str == ',')
		slub_debug_slabs = str + 1;
out:
=======
			higher_order_disable = true;
			break;
		default:
			if (init)
				pr_err("slub_debug option '%c' unknown. skipped\n", *str);
		}
	}
check_slabs:
	if (*str == ',')
		*slabs = ++str;
	else
		*slabs = NULL;

	/* Skip over the slab list */
	while (*str && *str != ';')
		str++;

	/* Skip any completely empty blocks */
	while (*str && *str == ';')
		str++;

	if (init && higher_order_disable)
		disable_higher_order_debug = 1;

	if (*str)
		return str;
	else
		return NULL;
}

static int __init setup_slub_debug(char *str)
{
	slab_flags_t flags;
	slab_flags_t global_flags;
	char *saved_str;
	char *slab_list;
	bool global_slub_debug_changed = false;
	bool slab_list_specified = false;

	global_flags = DEBUG_DEFAULT_FLAGS;
	if (*str++ != '=' || !*str)
		/*
		 * No options specified. Switch on full debugging.
		 */
		goto out;

	saved_str = str;
	while (str) {
		str = parse_slub_debug_flags(str, &flags, &slab_list, true);

		if (!slab_list) {
			global_flags = flags;
			global_slub_debug_changed = true;
		} else {
			slab_list_specified = true;
		}
	}

	/*
	 * For backwards compatibility, a single list of flags with list of
	 * slabs means debugging is only changed for those slabs, so the global
	 * slub_debug should be unchanged (0 or DEBUG_DEFAULT_FLAGS, depending
	 * on CONFIG_SLUB_DEBUG_ON). We can extended that to multiple lists as
	 * long as there is no option specifying flags without a slab list.
	 */
	if (slab_list_specified) {
		if (!global_slub_debug_changed)
			global_flags = slub_debug;
		slub_debug_string = saved_str;
	}
out:
	slub_debug = global_flags;
	if (slub_debug != 0 || slub_debug_string)
		static_branch_enable(&slub_debug_enabled);
	else
		static_branch_disable(&slub_debug_enabled);
>>>>>>> upstream/android-13
	if ((static_branch_unlikely(&init_on_alloc) ||
	     static_branch_unlikely(&init_on_free)) &&
	    (slub_debug & SLAB_POISON))
		pr_info("mem auto-init: SLAB_POISON will take precedence over init_on_alloc/init_on_free\n");
	return 1;
}

__setup("slub_debug", setup_slub_debug);

<<<<<<< HEAD
static const char *exclusion_list[] = {
        "zspage",
        "zs_handle",
        "zswap_entry",
        "avtab_node",
        "vm_area_struct",
        "anon_vma_chain",
        "anon_vma"
};

static int is_kmem_cache_excluded(const char *str)
{
        int i, excluded=0;

        for (i = 0; i < ARRAY_SIZE(exclusion_list); i++)
        {
                if(!strncmp(str, exclusion_list[i], strlen(exclusion_list[i]))) {
                        excluded = 1;
                        break;
                }
        }
        return excluded;
}

slab_flags_t kmem_cache_flags(unsigned int object_size,
	slab_flags_t flags, const char *name,
	void (*ctor)(void *))
{
#ifdef CONFIG_KDP
	if (name &&
		(!strncmp(name, CRED_JAR_RO, strlen(CRED_JAR_RO)) ||
		 !strncmp(name, TSEC_JAR, strlen(TSEC_JAR)) ||
		 !strncmp(name, VFSMNT_JAR, strlen(VFSMNT_JAR))))
		return flags;
#endif
	/*
	 * Enable debugging if selected on the kernel commandline.
	 */
	if (slub_debug && (!slub_debug_slabs || (name &&
		!strncmp(slub_debug_slabs, name, strlen(slub_debug_slabs))))) {
		flags |= slub_debug;

		if (name && is_kmem_cache_excluded(name))
			flags &= ~SLAB_STORE_USER;
	}

	return flags;
=======
/*
 * kmem_cache_flags - apply debugging options to the cache
 * @object_size:	the size of an object without meta data
 * @flags:		flags to set
 * @name:		name of the cache
 *
 * Debug option(s) are applied to @flags. In addition to the debug
 * option(s), if a slab name (or multiple) is specified i.e.
 * slub_debug=<Debug-Options>,<slab name1>,<slab name2> ...
 * then only the select slabs will receive the debug option(s).
 */
slab_flags_t kmem_cache_flags(unsigned int object_size,
	slab_flags_t flags, const char *name)
{
	char *iter;
	size_t len;
	char *next_block;
	slab_flags_t block_flags;
	slab_flags_t slub_debug_local = slub_debug;

	/*
	 * If the slab cache is for debugging (e.g. kmemleak) then
	 * don't store user (stack trace) information by default,
	 * but let the user enable it via the command line below.
	 */
	if (flags & SLAB_NOLEAKTRACE)
		slub_debug_local &= ~SLAB_STORE_USER;

	len = strlen(name);
	next_block = slub_debug_string;
	/* Go through all blocks of debug options, see if any matches our slab's name */
	while (next_block) {
		next_block = parse_slub_debug_flags(next_block, &block_flags, &iter, false);
		if (!iter)
			continue;
		/* Found a block that has a slab list, search it */
		while (*iter) {
			char *end, *glob;
			size_t cmplen;

			end = strchrnul(iter, ',');
			if (next_block && next_block < end)
				end = next_block - 1;

			glob = strnchr(iter, end - iter, '*');
			if (glob)
				cmplen = glob - iter;
			else
				cmplen = max_t(size_t, len, (end - iter));

			if (!strncmp(name, iter, cmplen)) {
				flags |= block_flags;
				return flags;
			}

			if (!*end || *end == ';')
				break;
			iter = end + 1;
		}
	}

	return flags | slub_debug_local;
>>>>>>> upstream/android-13
}
#else /* !CONFIG_SLUB_DEBUG */
static inline void setup_object_debug(struct kmem_cache *s,
			struct page *page, void *object) {}
<<<<<<< HEAD
static inline void setup_page_debug(struct kmem_cache *s,
			void *addr, int order) {}
=======
static inline
void setup_page_debug(struct kmem_cache *s, struct page *page, void *addr) {}
>>>>>>> upstream/android-13

static inline int alloc_debug_processing(struct kmem_cache *s,
	struct page *page, void *object, unsigned long addr) { return 0; }

static inline int free_debug_processing(
	struct kmem_cache *s, struct page *page,
	void *head, void *tail, int bulk_cnt,
	unsigned long addr) { return 0; }

static inline int slab_pad_check(struct kmem_cache *s, struct page *page)
			{ return 1; }
static inline int check_object(struct kmem_cache *s, struct page *page,
			void *object, u8 val) { return 1; }
static inline void add_full(struct kmem_cache *s, struct kmem_cache_node *n,
					struct page *page) {}
static inline void remove_full(struct kmem_cache *s, struct kmem_cache_node *n,
					struct page *page) {}
slab_flags_t kmem_cache_flags(unsigned int object_size,
<<<<<<< HEAD
	slab_flags_t flags, const char *name,
	void (*ctor)(void *))
=======
	slab_flags_t flags, const char *name)
>>>>>>> upstream/android-13
{
	return flags;
}
#define slub_debug 0

#define disable_higher_order_debug 0

static inline unsigned long slabs_node(struct kmem_cache *s, int node)
							{ return 0; }
static inline unsigned long node_nr_slabs(struct kmem_cache_node *n)
							{ return 0; }
static inline void inc_slabs_node(struct kmem_cache *s, int node,
							int objects) {}
static inline void dec_slabs_node(struct kmem_cache *s, int node,
							int objects) {}

static bool freelist_corrupted(struct kmem_cache *s, struct page *page,
			       void **freelist, void *nextfree)
{
	return false;
}
#endif /* CONFIG_SLUB_DEBUG */

/*
 * Hooks for other subsystems that check memory allocations. In a typical
 * production configuration these hooks all should produce no code at all.
 */
static inline void *kmalloc_large_node_hook(void *ptr, size_t size, gfp_t flags)
{
	ptr = kasan_kmalloc_large(ptr, size, flags);
<<<<<<< HEAD
=======
	/* As ptr might get tagged, call kmemleak hook after KASAN. */
>>>>>>> upstream/android-13
	kmemleak_alloc(ptr, size, 1, flags);
	return ptr;
}

static __always_inline void kfree_hook(void *x)
{
	kmemleak_free(x);
<<<<<<< HEAD
	kasan_kfree_large(x, _RET_IP_);
}

static __always_inline bool slab_free_hook(struct kmem_cache *s, void *x)
{
	kmemleak_free_recursive(x, s->flags);

	/*
	 * Trouble is that we may no longer disable interrupts in the fast path
	 * So in order to make the debug calls that expect irqs to be
	 * disabled we need to disable interrupts temporarily.
	 */
#ifdef CONFIG_LOCKDEP
	{
		unsigned long flags;

		local_irq_save(flags);
		debug_check_no_locks_freed(x, s->object_size);
		local_irq_restore(flags);
	}
#endif
	if (!(s->flags & SLAB_DEBUG_OBJECTS))
		debug_check_no_obj_freed(x, s->object_size);

	/* KASAN might put x into memory quarantine, delaying its reuse */
	return kasan_slab_free(s, x, _RET_IP_);
}

static inline bool slab_free_freelist_hook(struct kmem_cache *s,
					   void **head, void **tail)
=======
	kasan_kfree_large(x);
}

static __always_inline bool slab_free_hook(struct kmem_cache *s,
						void *x, bool init)
{
	kmemleak_free_recursive(x, s->flags);

	debug_check_no_locks_freed(x, s->object_size);

	if (!(s->flags & SLAB_DEBUG_OBJECTS))
		debug_check_no_obj_freed(x, s->object_size);

	/* Use KCSAN to help debug racy use-after-free. */
	if (!(s->flags & SLAB_TYPESAFE_BY_RCU))
		__kcsan_check_access(x, s->object_size,
				     KCSAN_ACCESS_WRITE | KCSAN_ACCESS_ASSERT);

	/*
	 * As memory initialization might be integrated into KASAN,
	 * kasan_slab_free and initialization memset's must be
	 * kept together to avoid discrepancies in behavior.
	 *
	 * The initialization memset's clear the object and the metadata,
	 * but don't touch the SLAB redzone.
	 */
	if (init) {
		int rsize;

		if (!kasan_has_integrated_init())
			memset(kasan_reset_tag(x), 0, s->object_size);
		rsize = (s->flags & SLAB_RED_ZONE) ? s->red_left_pad : 0;
		memset((char *)kasan_reset_tag(x) + s->inuse, 0,
		       s->size - s->inuse - rsize);
	}
	/* KASAN might put x into memory quarantine, delaying its reuse. */
	return kasan_slab_free(s, x, init);
}

static inline bool slab_free_freelist_hook(struct kmem_cache *s,
					   void **head, void **tail,
					   int *cnt)
>>>>>>> upstream/android-13
{

	void *object;
	void *next = *head;
	void *old_tail = *tail ? *tail : *head;
<<<<<<< HEAD
	int rsize;
=======

	if (is_kfence_address(next)) {
		slab_free_hook(s, next, false);
		return true;
	}
>>>>>>> upstream/android-13

	/* Head and tail of the reconstructed freelist */
	*head = NULL;
	*tail = NULL;

	do {
		object = next;
		next = get_freepointer(s, object);

<<<<<<< HEAD
		if (slab_want_init_on_free(s)) {
			/*
			 * Clear the object and the metadata, but don't touch
			 * the redzone.
			 */
			memset(object, 0, s->object_size);
			rsize = (s->flags & SLAB_RED_ZONE) ? s->red_left_pad
							   : 0;
			memset((char *)object + s->inuse, 0,
			       s->size - s->inuse - rsize);

		}
		/* If object's reuse doesn't have to be delayed */
		if (!slab_free_hook(s, object)) {
=======
		/* If object's reuse doesn't have to be delayed */
		if (!slab_free_hook(s, object, slab_want_init_on_free(s))) {
>>>>>>> upstream/android-13
			/* Move object to the new freelist */
			set_freepointer(s, object, *head);
			*head = object;
			if (!*tail)
				*tail = object;
<<<<<<< HEAD
=======
		} else {
			/*
			 * Adjust the reconstructed freelist depth
			 * accordingly if object's reuse is delayed.
			 */
			--(*cnt);
>>>>>>> upstream/android-13
		}
	} while (object != old_tail);

	if (*head == *tail)
		*tail = NULL;

	return *head != NULL;
}

static void *setup_object(struct kmem_cache *s, struct page *page,
				void *object)
{
	setup_object_debug(s, page, object);
	object = kasan_init_slab_obj(s, object);
	if (unlikely(s->ctor)) {
		kasan_unpoison_object_data(s, object);
		s->ctor(object);
		kasan_poison_object_data(s, object);
	}
	return object;
}

/*
 * Slab allocation and freeing
 */
static inline struct page *alloc_slab_page(struct kmem_cache *s,
		gfp_t flags, int node, struct kmem_cache_order_objects oo)
{
	struct page *page;
	unsigned int order = oo_order(oo);

	if (node == NUMA_NO_NODE)
		page = alloc_pages(flags, order);
	else
		page = __alloc_pages_node(node, flags, order);

<<<<<<< HEAD
	if (page && memcg_charge_slab(page, flags, order, s)) {
		__free_pages(page, order);
		page = NULL;
	}

=======
>>>>>>> upstream/android-13
	return page;
}

#ifdef CONFIG_SLAB_FREELIST_RANDOM
/* Pre-initialize the random sequence cache */
static int init_cache_random_seq(struct kmem_cache *s)
{
	unsigned int count = oo_objects(s->oo);
	int err;

	/* Bailout if already initialised */
	if (s->random_seq)
		return 0;

	err = cache_random_seq_create(s, count, GFP_KERNEL);
	if (err) {
		pr_err("SLUB: Unable to initialize free list for %s\n",
			s->name);
		return err;
	}

	/* Transform to an offset on the set of pages */
	if (s->random_seq) {
		unsigned int i;

		for (i = 0; i < count; i++)
			s->random_seq[i] *= s->size;
	}
	return 0;
}

/* Initialize each random sequence freelist per cache */
static void __init init_freelist_randomization(void)
{
	struct kmem_cache *s;

	mutex_lock(&slab_mutex);

	list_for_each_entry(s, &slab_caches, list)
		init_cache_random_seq(s);

	mutex_unlock(&slab_mutex);
}

/* Get the next entry on the pre-computed freelist randomized */
static void *next_freelist_entry(struct kmem_cache *s, struct page *page,
				unsigned long *pos, void *start,
				unsigned long page_limit,
				unsigned long freelist_count)
{
	unsigned int idx;

	/*
	 * If the target page allocation failed, the number of objects on the
	 * page might be smaller than the usual size defined by the cache.
	 */
	do {
		idx = s->random_seq[*pos];
		*pos += 1;
		if (*pos >= freelist_count)
			*pos = 0;
	} while (unlikely(idx >= page_limit));

	return (char *)start + idx;
}

/* Shuffle the single linked freelist based on a random pre-computed sequence */
static bool shuffle_freelist(struct kmem_cache *s, struct page *page)
{
	void *start;
	void *cur;
	void *next;
	unsigned long idx, pos, page_limit, freelist_count;

	if (page->objects < 2 || !s->random_seq)
		return false;

	freelist_count = oo_objects(s->oo);
	pos = get_random_int() % freelist_count;

	page_limit = page->objects * s->size;
	start = fixup_red_left(s, page_address(page));

	/* First entry is used as the base of the freelist */
	cur = next_freelist_entry(s, page, &pos, start, page_limit,
				freelist_count);
	cur = setup_object(s, page, cur);
	page->freelist = cur;

	for (idx = 1; idx < page->objects; idx++) {
		next = next_freelist_entry(s, page, &pos, start, page_limit,
			freelist_count);
		next = setup_object(s, page, next);
		set_freepointer(s, cur, next);
		cur = next;
	}
	set_freepointer(s, cur, NULL);

	return true;
}
#else
static inline int init_cache_random_seq(struct kmem_cache *s)
{
	return 0;
}
static inline void init_freelist_randomization(void) { }
static inline bool shuffle_freelist(struct kmem_cache *s, struct page *page)
{
	return false;
}
#endif /* CONFIG_SLAB_FREELIST_RANDOM */

static struct page *allocate_slab(struct kmem_cache *s, gfp_t flags, int node)
{
	struct page *page;
	struct kmem_cache_order_objects oo = s->oo;
	gfp_t alloc_gfp;
	void *start, *p, *next;
<<<<<<< HEAD
	int idx, order;
	bool shuffle;
#if defined(CONFIG_KDP) && defined(CONFIG_RKP)
	void *virt_page = NULL;
#endif
	flags &= gfp_allowed_mask;

	if (gfpflags_allow_blocking(flags))
		local_irq_enable();
=======
	int idx;
	bool shuffle;

	flags &= gfp_allowed_mask;
>>>>>>> upstream/android-13

	flags |= s->allocflags;

	/*
	 * Let the initial higher-order allocation fail under memory pressure
	 * so we fall-back to the minimum order allocation.
	 */
	alloc_gfp = (flags | __GFP_NOWARN | __GFP_NORETRY) & ~__GFP_NOFAIL;
	if ((alloc_gfp & __GFP_DIRECT_RECLAIM) && oo_order(oo) > oo_order(s->min))
		alloc_gfp = (alloc_gfp | __GFP_NOMEMALLOC) & ~(__GFP_RECLAIM|__GFP_NOFAIL);
<<<<<<< HEAD
#if defined(CONFIG_KDP) && defined(CONFIG_RKP)
	if (is_kdp_kmem_cache(s)) {
		virt_page = rkp_ro_alloc();
		if(!virt_page)
			goto def_alloc;

		page = virt_to_page(virt_page);
		oo = s->min;
	} else {
def_alloc:
#endif
=======

>>>>>>> upstream/android-13
	page = alloc_slab_page(s, alloc_gfp, node, oo);
	if (unlikely(!page)) {
		oo = s->min;
		alloc_gfp = flags;
		/*
		 * Allocation may have failed due to fragmentation.
		 * Try a lower order alloc if possible
		 */
		page = alloc_slab_page(s, alloc_gfp, node, oo);
		if (unlikely(!page))
			goto out;
		stat(s, ORDER_FALLBACK);
	}
<<<<<<< HEAD
#if defined(CONFIG_KDP) && defined(CONFIG_RKP)
	}
#endif
	page->objects = oo_objects(oo);

	order = compound_order(page);
=======

	page->objects = oo_objects(oo);

	account_slab_page(page, oo_order(oo), s, flags);

>>>>>>> upstream/android-13
	page->slab_cache = s;
	__SetPageSlab(page);
	if (page_is_pfmemalloc(page))
		SetPageSlabPfmemalloc(page);

	kasan_poison_slab(page);

	start = page_address(page);

<<<<<<< HEAD
	setup_page_debug(s, start, order);

#ifdef CONFIG_KDP
	/*
	 * We modify the following so that slab alloc for protected data
	 * types are allocated from our own pool.
	 */
	if (s->name) {
#ifdef CONFIG_KDP
		u64 sc, va_page, type;

		va_page = (u64)__va(page_to_phys(page));

		if (!strncmp(s->name, CRED_JAR_RO, strlen(CRED_JAR_RO)))
			type = CRED_JAR_TYPE;
		else if (!strncmp(s->name, TSEC_JAR, strlen(TSEC_JAR)))
			type = TSEC_JAR_TYPE;
		else if (!strncmp(s->name, VFSMNT_JAR, strlen(VFSMNT_JAR)))
			type = VFSMNT_JAR_TYPE;
		else
			type = UNKNOWN_JAR_TYPE;

		if (type) {
			for (sc = 0; sc < (1 << oo_order(oo)); sc++) {
				uh_call(UH_APP_KDP, SET_SLAB_RO, va_page, type, 0, 0);
				va_page += PAGE_SIZE;
			}
		}
#else
		u64 sc, va_page;
		va_page = (u64)__va(page_to_phys(page));

		if (!strncmp(s->name, CRED_JAR_RO, strlen(CRED_JAR_RO))) {
			for(sc = 0; sc < (1 << oo_order(oo)); sc++) {
				uh_call(UH_APP_KDP, SET_CRED_RO, va_page, 0, 0, 0);
				va_page += PAGE_SIZE;
			}
		} else if (!strncmp(s->name, TSEC_JAR, strlen(TSEC_JAR))) {
			for(sc = 0; sc < (1 << oo_order(oo)); sc++) {
				uh_call(UH_APP_KDP, SER_SP_RO, va_page, 0, 0, 0);
				va_page += PAGE_SIZE;
			}
		} else if (!strncmp(s->name, VFSMNT_JAR, strlen(VFSMNT_JAR))) {
			for(sc = 0; sc < (1 << oo_order(oo)); sc++) {
				uh_call(UH_APP_KDP, SET_NS_RO, va_page, 0, 0, 0);
				va_page += PAGE_SIZE;
			}
		}
#endif
	}
#endif
=======
	setup_page_debug(s, page, start);

>>>>>>> upstream/android-13
	shuffle = shuffle_freelist(s, page);

	if (!shuffle) {
		start = fixup_red_left(s, start);
		start = setup_object(s, page, start);
		page->freelist = start;
		for (idx = 0, p = start; idx < page->objects - 1; idx++) {
			next = p + s->size;
			next = setup_object(s, page, next);
			set_freepointer(s, p, next);
			p = next;
		}
		set_freepointer(s, p, NULL);
	}

	page->inuse = page->objects;
	page->frozen = 1;

out:
<<<<<<< HEAD
	if (gfpflags_allow_blocking(flags))
		local_irq_disable();
	if (!page)
		return NULL;

	mod_lruvec_page_state(page,
		(s->flags & SLAB_RECLAIM_ACCOUNT) ?
		NR_SLAB_RECLAIMABLE : NR_SLAB_UNRECLAIMABLE,
		1 << oo_order(oo));

=======
	if (!page)
		return NULL;

>>>>>>> upstream/android-13
	inc_slabs_node(s, page_to_nid(page), page->objects);

	return page;
}

static struct page *new_slab(struct kmem_cache *s, gfp_t flags, int node)
{
<<<<<<< HEAD
	if (unlikely(flags & GFP_SLAB_BUG_MASK)) {
		gfp_t invalid_mask = flags & GFP_SLAB_BUG_MASK;
		flags &= ~GFP_SLAB_BUG_MASK;
		pr_warn("Unexpected gfp: %#x (%pGg). Fixing up to gfp: %#x (%pGg). Fix your code!\n",
				invalid_mask, &invalid_mask, flags, &flags);
		dump_stack();
	}
=======
	if (unlikely(flags & GFP_SLAB_BUG_MASK))
		flags = kmalloc_fix_flags(flags);

	WARN_ON_ONCE(s->ctor && (flags & __GFP_ZERO));
>>>>>>> upstream/android-13

	return allocate_slab(s,
		flags & (GFP_RECLAIM_MASK | GFP_CONSTRAINT_MASK), node);
}

<<<<<<< HEAD
#ifdef CONFIG_KDP
spinlock_t ro_pages_lock = __SPIN_LOCK_UNLOCKED();

static void free_ro_pages(struct kmem_cache *s,struct page *page, int order)
{
	unsigned long flags;
	unsigned long long sc,va_page;

	sc = 0;
	va_page = (unsigned long long)__va(page_to_phys(page));
#ifdef CONFIG_RKP
	if (is_rkp_ro_buffer(va_page)) {
		for (sc = 0; sc < (1 << order); sc++) {
			uh_call(UH_APP_KDP, PGD_RWX, va_page, 0, 0, 0);
			rkp_ro_free((void *)va_page);
			va_page += PAGE_SIZE;
		}
		return;
	}
#endif
	spin_lock_irqsave(&ro_pages_lock,flags);
	for (sc = 0; sc < (1 << order); sc++) {
		uh_call(UH_APP_KDP, PGD_RWX, va_page, 0, 0, 0);
		va_page += PAGE_SIZE;
	}
	memcg_uncharge_slab(page, order, s);
	kasan_alloc_pages(page, order);
	__free_pages(page, order);
	spin_unlock_irqrestore(&ro_pages_lock,flags);
}
#endif /*CONFIG_KDP*/

=======
>>>>>>> upstream/android-13
static void __free_slab(struct kmem_cache *s, struct page *page)
{
	int order = compound_order(page);
	int pages = 1 << order;

<<<<<<< HEAD
	if (s->flags & SLAB_CONSISTENCY_CHECKS) {
=======
	if (kmem_cache_debug_flags(s, SLAB_CONSISTENCY_CHECKS)) {
>>>>>>> upstream/android-13
		void *p;

		slab_pad_check(s, page);
		for_each_object(p, s, page_address(page),
						page->objects)
			check_object(s, page, p, SLUB_RED_INACTIVE);
	}

<<<<<<< HEAD
	mod_lruvec_page_state(page,
		(s->flags & SLAB_RECLAIM_ACCOUNT) ?
		NR_SLAB_RECLAIMABLE : NR_SLAB_UNRECLAIMABLE,
		-pages);

	__ClearPageSlabPfmemalloc(page);
	__ClearPageSlab(page);

	page->mapping = NULL;
	if (current->reclaim_state)
		current->reclaim_state->reclaimed_slab += pages;
#ifdef CONFIG_KDP
	/* We free the protected pages here. */
	if (is_kdp_kmem_cache(s)) {
		free_ro_pages(s, page, order);
		return;
	}
#endif
	memcg_uncharge_slab(page, order, s);
=======
	__ClearPageSlabPfmemalloc(page);
	__ClearPageSlab(page);
	/* In union with page->mapping where page allocator expects NULL */
	page->slab_cache = NULL;
	if (current->reclaim_state)
		current->reclaim_state->reclaimed_slab += pages;
	unaccount_slab_page(page, order, s);
>>>>>>> upstream/android-13
	__free_pages(page, order);
}

static void rcu_free_slab(struct rcu_head *h)
{
	struct page *page = container_of(h, struct page, rcu_head);

	__free_slab(page->slab_cache, page);
}

static void free_slab(struct kmem_cache *s, struct page *page)
{
	if (unlikely(s->flags & SLAB_TYPESAFE_BY_RCU)) {
		call_rcu(&page->rcu_head, rcu_free_slab);
	} else
		__free_slab(s, page);
}

static void discard_slab(struct kmem_cache *s, struct page *page)
{
	dec_slabs_node(s, page_to_nid(page), page->objects);
	free_slab(s, page);
}

/*
 * Management of partially allocated slabs.
 */
static inline void
__add_partial(struct kmem_cache_node *n, struct page *page, int tail)
{
	n->nr_partial++;
	if (tail == DEACTIVATE_TO_TAIL)
<<<<<<< HEAD
		list_add_tail(&page->lru, &n->partial);
	else
		list_add(&page->lru, &n->partial);
=======
		list_add_tail(&page->slab_list, &n->partial);
	else
		list_add(&page->slab_list, &n->partial);
>>>>>>> upstream/android-13
}

static inline void add_partial(struct kmem_cache_node *n,
				struct page *page, int tail)
{
	lockdep_assert_held(&n->list_lock);
	__add_partial(n, page, tail);
}

static inline void remove_partial(struct kmem_cache_node *n,
					struct page *page)
{
	lockdep_assert_held(&n->list_lock);
<<<<<<< HEAD
	list_del(&page->lru);
=======
	list_del(&page->slab_list);
>>>>>>> upstream/android-13
	n->nr_partial--;
}

/*
 * Remove slab from the partial list, freeze it and
 * return the pointer to the freelist.
 *
 * Returns a list of objects or NULL if it fails.
 */
static inline void *acquire_slab(struct kmem_cache *s,
		struct kmem_cache_node *n, struct page *page,
		int mode, int *objects)
{
	void *freelist;
	unsigned long counters;
	struct page new;

	lockdep_assert_held(&n->list_lock);

	/*
	 * Zap the freelist and set the frozen bit.
	 * The old freelist is the list of objects for the
	 * per cpu allocation list.
	 */
	freelist = page->freelist;
	counters = page->counters;
	new.counters = counters;
	*objects = new.objects - new.inuse;
	if (mode) {
		new.inuse = page->objects;
		new.freelist = NULL;
	} else {
		new.freelist = freelist;
	}

	VM_BUG_ON(new.frozen);
	new.frozen = 1;

	if (!__cmpxchg_double_slab(s, page,
			freelist, counters,
			new.freelist, new.counters,
			"acquire_slab"))
		return NULL;

	remove_partial(n, page);
	WARN_ON(!freelist);
	return freelist;
}

<<<<<<< HEAD
static void put_cpu_partial(struct kmem_cache *s, struct page *page, int drain);
=======
#ifdef CONFIG_SLUB_CPU_PARTIAL
static void put_cpu_partial(struct kmem_cache *s, struct page *page, int drain);
#else
static inline void put_cpu_partial(struct kmem_cache *s, struct page *page,
				   int drain) { }
#endif
>>>>>>> upstream/android-13
static inline bool pfmemalloc_match(struct page *page, gfp_t gfpflags);

/*
 * Try to allocate a partial slab from a specific node.
 */
static void *get_partial_node(struct kmem_cache *s, struct kmem_cache_node *n,
<<<<<<< HEAD
				struct kmem_cache_cpu *c, gfp_t flags)
=======
			      struct page **ret_page, gfp_t gfpflags)
>>>>>>> upstream/android-13
{
	struct page *page, *page2;
	void *object = NULL;
	unsigned int available = 0;
<<<<<<< HEAD
=======
	unsigned long flags;
>>>>>>> upstream/android-13
	int objects;

	/*
	 * Racy check. If we mistakenly see no partial slabs then we
	 * just allocate an empty slab. If we mistakenly try to get a
<<<<<<< HEAD
	 * partial slab and there is none available then get_partials()
=======
	 * partial slab and there is none available then get_partial()
>>>>>>> upstream/android-13
	 * will return NULL.
	 */
	if (!n || !n->nr_partial)
		return NULL;

<<<<<<< HEAD
	spin_lock(&n->list_lock);
	list_for_each_entry_safe(page, page2, &n->partial, lru) {
		void *t;

		if (!pfmemalloc_match(page, flags))
=======
	spin_lock_irqsave(&n->list_lock, flags);
	list_for_each_entry_safe(page, page2, &n->partial, slab_list) {
		void *t;

		if (!pfmemalloc_match(page, gfpflags))
>>>>>>> upstream/android-13
			continue;

		t = acquire_slab(s, n, page, object == NULL, &objects);
		if (!t)
			break;

		available += objects;
		if (!object) {
<<<<<<< HEAD
			c->page = page;
=======
			*ret_page = page;
>>>>>>> upstream/android-13
			stat(s, ALLOC_FROM_PARTIAL);
			object = t;
		} else {
			put_cpu_partial(s, page, 0);
			stat(s, CPU_PARTIAL_NODE);
		}
		if (!kmem_cache_has_cpu_partial(s)
			|| available > slub_cpu_partial(s) / 2)
			break;

	}
<<<<<<< HEAD
	spin_unlock(&n->list_lock);
=======
	spin_unlock_irqrestore(&n->list_lock, flags);
>>>>>>> upstream/android-13
	return object;
}

/*
 * Get a page from somewhere. Search in increasing NUMA distances.
 */
static void *get_any_partial(struct kmem_cache *s, gfp_t flags,
<<<<<<< HEAD
		struct kmem_cache_cpu *c)
=======
			     struct page **ret_page)
>>>>>>> upstream/android-13
{
#ifdef CONFIG_NUMA
	struct zonelist *zonelist;
	struct zoneref *z;
	struct zone *zone;
<<<<<<< HEAD
	enum zone_type high_zoneidx = gfp_zone(flags);
=======
	enum zone_type highest_zoneidx = gfp_zone(flags);
>>>>>>> upstream/android-13
	void *object;
	unsigned int cpuset_mems_cookie;

	/*
	 * The defrag ratio allows a configuration of the tradeoffs between
	 * inter node defragmentation and node local allocations. A lower
	 * defrag_ratio increases the tendency to do local allocations
	 * instead of attempting to obtain partial slabs from other nodes.
	 *
	 * If the defrag_ratio is set to 0 then kmalloc() always
	 * returns node local objects. If the ratio is higher then kmalloc()
	 * may return off node objects because partial slabs are obtained
	 * from other nodes and filled up.
	 *
	 * If /sys/kernel/slab/xx/remote_node_defrag_ratio is set to 100
	 * (which makes defrag_ratio = 1000) then every (well almost)
	 * allocation will first attempt to defrag slab caches on other nodes.
	 * This means scanning over all nodes to look for partial slabs which
	 * may be expensive if we do it every time we are trying to find a slab
	 * with available objects.
	 */
	if (!s->remote_node_defrag_ratio ||
			get_cycles() % 1024 > s->remote_node_defrag_ratio)
		return NULL;

	do {
		cpuset_mems_cookie = read_mems_allowed_begin();
		zonelist = node_zonelist(mempolicy_slab_node(), flags);
<<<<<<< HEAD
		for_each_zone_zonelist(zone, z, zonelist, high_zoneidx) {
=======
		for_each_zone_zonelist(zone, z, zonelist, highest_zoneidx) {
>>>>>>> upstream/android-13
			struct kmem_cache_node *n;

			n = get_node(s, zone_to_nid(zone));

			if (n && cpuset_zone_allowed(zone, flags) &&
					n->nr_partial > s->min_partial) {
<<<<<<< HEAD
				object = get_partial_node(s, n, c, flags);
=======
				object = get_partial_node(s, n, ret_page, flags);
>>>>>>> upstream/android-13
				if (object) {
					/*
					 * Don't check read_mems_allowed_retry()
					 * here - if mems_allowed was updated in
					 * parallel, that was a harmless race
					 * between allocation and the cpuset
					 * update
					 */
					return object;
				}
			}
		}
	} while (read_mems_allowed_retry(cpuset_mems_cookie));
<<<<<<< HEAD
#endif
=======
#endif	/* CONFIG_NUMA */
>>>>>>> upstream/android-13
	return NULL;
}

/*
 * Get a partial page, lock it and return it.
 */
static void *get_partial(struct kmem_cache *s, gfp_t flags, int node,
<<<<<<< HEAD
		struct kmem_cache_cpu *c)
=======
			 struct page **ret_page)
>>>>>>> upstream/android-13
{
	void *object;
	int searchnode = node;

	if (node == NUMA_NO_NODE)
		searchnode = numa_mem_id();

<<<<<<< HEAD
	object = get_partial_node(s, get_node(s, searchnode), c, flags);
	if (object || node != NUMA_NO_NODE)
		return object;

	return get_any_partial(s, flags, c);
}

#ifdef CONFIG_PREEMPT
/*
 * Calculate the next globally unique transaction for disambiguiation
=======
	object = get_partial_node(s, get_node(s, searchnode), ret_page, flags);
	if (object || node != NUMA_NO_NODE)
		return object;

	return get_any_partial(s, flags, ret_page);
}

#ifdef CONFIG_PREEMPTION
/*
 * Calculate the next globally unique transaction for disambiguation
>>>>>>> upstream/android-13
 * during cmpxchg. The transactions start with the cpu number and are then
 * incremented by CONFIG_NR_CPUS.
 */
#define TID_STEP  roundup_pow_of_two(CONFIG_NR_CPUS)
#else
/*
 * No preemption supported therefore also no need to check for
 * different cpus.
 */
#define TID_STEP 1
#endif

static inline unsigned long next_tid(unsigned long tid)
{
	return tid + TID_STEP;
}

<<<<<<< HEAD
=======
#ifdef SLUB_DEBUG_CMPXCHG
>>>>>>> upstream/android-13
static inline unsigned int tid_to_cpu(unsigned long tid)
{
	return tid % TID_STEP;
}

static inline unsigned long tid_to_event(unsigned long tid)
{
	return tid / TID_STEP;
}
<<<<<<< HEAD
=======
#endif
>>>>>>> upstream/android-13

static inline unsigned int init_tid(int cpu)
{
	return cpu;
}

static inline void note_cmpxchg_failure(const char *n,
		const struct kmem_cache *s, unsigned long tid)
{
#ifdef SLUB_DEBUG_CMPXCHG
	unsigned long actual_tid = __this_cpu_read(s->cpu_slab->tid);

	pr_info("%s %s: cmpxchg redo ", n, s->name);

<<<<<<< HEAD
#ifdef CONFIG_PREEMPT
=======
#ifdef CONFIG_PREEMPTION
>>>>>>> upstream/android-13
	if (tid_to_cpu(tid) != tid_to_cpu(actual_tid))
		pr_warn("due to cpu change %d -> %d\n",
			tid_to_cpu(tid), tid_to_cpu(actual_tid));
	else
#endif
	if (tid_to_event(tid) != tid_to_event(actual_tid))
		pr_warn("due to cpu running other code. Event %ld->%ld\n",
			tid_to_event(tid), tid_to_event(actual_tid));
	else
		pr_warn("for unknown reason: actual=%lx was=%lx target=%lx\n",
			actual_tid, tid, next_tid(tid));
#endif
	stat(s, CMPXCHG_DOUBLE_CPU_FAIL);
}

static void init_kmem_cache_cpus(struct kmem_cache *s)
{
	int cpu;
<<<<<<< HEAD

	for_each_possible_cpu(cpu)
		per_cpu_ptr(s->cpu_slab, cpu)->tid = init_tid(cpu);
}

/*
 * Remove the cpu slab
 */
static void deactivate_slab(struct kmem_cache *s, struct page *page,
				void *freelist, struct kmem_cache_cpu *c)
{
	enum slab_modes { M_NONE, M_PARTIAL, M_FULL, M_FREE };
	struct kmem_cache_node *n = get_node(s, page_to_nid(page));
	int lock = 0;
	enum slab_modes l = M_NONE, m = M_NONE;
	void *nextfree;
	int tail = DEACTIVATE_TO_HEAD;
=======
	struct kmem_cache_cpu *c;

	for_each_possible_cpu(cpu) {
		c = per_cpu_ptr(s->cpu_slab, cpu);
		local_lock_init(&c->lock);
		c->tid = init_tid(cpu);
	}
}

/*
 * Finishes removing the cpu slab. Merges cpu's freelist with page's freelist,
 * unfreezes the slabs and puts it on the proper list.
 * Assumes the slab has been already safely taken away from kmem_cache_cpu
 * by the caller.
 */
static void deactivate_slab(struct kmem_cache *s, struct page *page,
			    void *freelist)
{
	enum slab_modes { M_NONE, M_PARTIAL, M_FULL, M_FREE };
	struct kmem_cache_node *n = get_node(s, page_to_nid(page));
	int lock = 0, free_delta = 0;
	enum slab_modes l = M_NONE, m = M_NONE;
	void *nextfree, *freelist_iter, *freelist_tail;
	int tail = DEACTIVATE_TO_HEAD;
	unsigned long flags = 0;
>>>>>>> upstream/android-13
	struct page new;
	struct page old;

	if (page->freelist) {
		stat(s, DEACTIVATE_REMOTE_FREES);
		tail = DEACTIVATE_TO_TAIL;
	}

	/*
<<<<<<< HEAD
	 * Stage one: Free all available per cpu objects back
	 * to the page freelist while it is still frozen. Leave the
	 * last one.
	 *
	 * There is no need to take the list->lock because the page
	 * is still frozen.
	 */
	while (freelist && (nextfree = get_freepointer(s, freelist))) {
		void *prior;
		unsigned long counters;

		/*
		 * If 'nextfree' is invalid, it is possible that the object at
		 * 'freelist' is already corrupted.  So isolate all objects
		 * starting at 'freelist'.
		 */
		if (freelist_corrupted(s, page, &freelist, nextfree))
			break;

		do {
			prior = page->freelist;
			counters = page->counters;
			set_freepointer(s, freelist, prior);
			new.counters = counters;
			new.inuse--;
			VM_BUG_ON(!new.frozen);

		} while (!__cmpxchg_double_slab(s, page,
			prior, counters,
			freelist, new.counters,
			"drain percpu freelist"));

		freelist = nextfree;
	}

	/*
	 * Stage two: Ensure that the page is unfrozen while the
	 * list presence reflects the actual number of objects
	 * during unfreeze.
=======
	 * Stage one: Count the objects on cpu's freelist as free_delta and
	 * remember the last object in freelist_tail for later splicing.
	 */
	freelist_tail = NULL;
	freelist_iter = freelist;
	while (freelist_iter) {
		nextfree = get_freepointer(s, freelist_iter);

		/*
		 * If 'nextfree' is invalid, it is possible that the object at
		 * 'freelist_iter' is already corrupted.  So isolate all objects
		 * starting at 'freelist_iter' by skipping them.
		 */
		if (freelist_corrupted(s, page, &freelist_iter, nextfree))
			break;

		freelist_tail = freelist_iter;
		free_delta++;

		freelist_iter = nextfree;
	}

	/*
	 * Stage two: Unfreeze the page while splicing the per-cpu
	 * freelist to the head of page's freelist.
	 *
	 * Ensure that the page is unfrozen while the list presence
	 * reflects the actual number of objects during unfreeze.
>>>>>>> upstream/android-13
	 *
	 * We setup the list membership and then perform a cmpxchg
	 * with the count. If there is a mismatch then the page
	 * is not unfrozen but the page is on the wrong list.
	 *
	 * Then we restart the process which may have to remove
	 * the page from the list that we just put it on again
	 * because the number of objects in the slab may have
	 * changed.
	 */
redo:

<<<<<<< HEAD
	old.freelist = page->freelist;
	old.counters = page->counters;
=======
	old.freelist = READ_ONCE(page->freelist);
	old.counters = READ_ONCE(page->counters);
>>>>>>> upstream/android-13
	VM_BUG_ON(!old.frozen);

	/* Determine target state of the slab */
	new.counters = old.counters;
<<<<<<< HEAD
	if (freelist) {
		new.inuse--;
		set_freepointer(s, freelist, old.freelist);
=======
	if (freelist_tail) {
		new.inuse -= free_delta;
		set_freepointer(s, freelist_tail, old.freelist);
>>>>>>> upstream/android-13
		new.freelist = freelist;
	} else
		new.freelist = old.freelist;

	new.frozen = 0;

	if (!new.inuse && n->nr_partial >= s->min_partial)
		m = M_FREE;
	else if (new.freelist) {
		m = M_PARTIAL;
		if (!lock) {
			lock = 1;
			/*
<<<<<<< HEAD
			 * Taking the spinlock removes the possiblity
			 * that acquire_slab() will see a slab page that
			 * is frozen
			 */
			spin_lock(&n->list_lock);
		}
	} else {
		m = M_FULL;
		if (kmem_cache_debug(s) && !lock) {
=======
			 * Taking the spinlock removes the possibility
			 * that acquire_slab() will see a slab page that
			 * is frozen
			 */
			spin_lock_irqsave(&n->list_lock, flags);
		}
	} else {
		m = M_FULL;
		if (kmem_cache_debug_flags(s, SLAB_STORE_USER) && !lock) {
>>>>>>> upstream/android-13
			lock = 1;
			/*
			 * This also ensures that the scanning of full
			 * slabs from diagnostic functions will not see
			 * any frozen slabs.
			 */
<<<<<<< HEAD
			spin_lock(&n->list_lock);
=======
			spin_lock_irqsave(&n->list_lock, flags);
>>>>>>> upstream/android-13
		}
	}

	if (l != m) {
<<<<<<< HEAD

		if (l == M_PARTIAL)

			remove_partial(n, page);

		else if (l == M_FULL)

			remove_full(s, n, page);

		if (m == M_PARTIAL) {

			add_partial(n, page, tail);
			stat(s, tail);

		} else if (m == M_FULL) {

			stat(s, DEACTIVATE_FULL);
			add_full(s, n, page);

		}
	}

	l = m;
	if (!__cmpxchg_double_slab(s, page,
=======
		if (l == M_PARTIAL)
			remove_partial(n, page);
		else if (l == M_FULL)
			remove_full(s, n, page);

		if (m == M_PARTIAL)
			add_partial(n, page, tail);
		else if (m == M_FULL)
			add_full(s, n, page);
	}

	l = m;
	if (!cmpxchg_double_slab(s, page,
>>>>>>> upstream/android-13
				old.freelist, old.counters,
				new.freelist, new.counters,
				"unfreezing slab"))
		goto redo;

	if (lock)
<<<<<<< HEAD
		spin_unlock(&n->list_lock);

	if (m == M_FREE) {
=======
		spin_unlock_irqrestore(&n->list_lock, flags);

	if (m == M_PARTIAL)
		stat(s, tail);
	else if (m == M_FULL)
		stat(s, DEACTIVATE_FULL);
	else if (m == M_FREE) {
>>>>>>> upstream/android-13
		stat(s, DEACTIVATE_EMPTY);
		discard_slab(s, page);
		stat(s, FREE_SLAB);
	}
<<<<<<< HEAD

	c->page = NULL;
	c->freelist = NULL;
}

/*
 * Unfreeze all the cpu partial slabs.
 *
 * This function must be called with interrupts disabled
 * for the cpu using c (or some other guarantee must be there
 * to guarantee no concurrent accesses).
 */
static void unfreeze_partials(struct kmem_cache *s,
		struct kmem_cache_cpu *c)
{
#ifdef CONFIG_SLUB_CPU_PARTIAL
	struct kmem_cache_node *n = NULL, *n2 = NULL;
	struct page *page, *discard_page = NULL;

	while ((page = c->partial)) {
		struct page new;
		struct page old;

		c->partial = page->next;
=======
}

#ifdef CONFIG_SLUB_CPU_PARTIAL
static void __unfreeze_partials(struct kmem_cache *s, struct page *partial_page)
{
	struct kmem_cache_node *n = NULL, *n2 = NULL;
	struct page *page, *discard_page = NULL;
	unsigned long flags = 0;

	while (partial_page) {
		struct page new;
		struct page old;

		page = partial_page;
		partial_page = page->next;
>>>>>>> upstream/android-13

		n2 = get_node(s, page_to_nid(page));
		if (n != n2) {
			if (n)
<<<<<<< HEAD
				spin_unlock(&n->list_lock);

			n = n2;
			spin_lock(&n->list_lock);
=======
				spin_unlock_irqrestore(&n->list_lock, flags);

			n = n2;
			spin_lock_irqsave(&n->list_lock, flags);
>>>>>>> upstream/android-13
		}

		do {

			old.freelist = page->freelist;
			old.counters = page->counters;
			VM_BUG_ON(!old.frozen);

			new.counters = old.counters;
			new.freelist = old.freelist;

			new.frozen = 0;

		} while (!__cmpxchg_double_slab(s, page,
				old.freelist, old.counters,
				new.freelist, new.counters,
				"unfreezing slab"));

		if (unlikely(!new.inuse && n->nr_partial >= s->min_partial)) {
			page->next = discard_page;
			discard_page = page;
		} else {
			add_partial(n, page, DEACTIVATE_TO_TAIL);
			stat(s, FREE_ADD_PARTIAL);
		}
	}

	if (n)
<<<<<<< HEAD
		spin_unlock(&n->list_lock);
=======
		spin_unlock_irqrestore(&n->list_lock, flags);
>>>>>>> upstream/android-13

	while (discard_page) {
		page = discard_page;
		discard_page = discard_page->next;

		stat(s, DEACTIVATE_EMPTY);
		discard_slab(s, page);
		stat(s, FREE_SLAB);
	}
<<<<<<< HEAD
#endif
}

/*
 * Put a page that was just frozen (in __slab_free) into a partial page
 * slot if available.
=======
}

/*
 * Unfreeze all the cpu partial slabs.
 */
static void unfreeze_partials(struct kmem_cache *s)
{
	struct page *partial_page;
	unsigned long flags;

	local_lock_irqsave(&s->cpu_slab->lock, flags);
	partial_page = this_cpu_read(s->cpu_slab->partial);
	this_cpu_write(s->cpu_slab->partial, NULL);
	local_unlock_irqrestore(&s->cpu_slab->lock, flags);

	if (partial_page)
		__unfreeze_partials(s, partial_page);
}

static void unfreeze_partials_cpu(struct kmem_cache *s,
				  struct kmem_cache_cpu *c)
{
	struct page *partial_page;

	partial_page = slub_percpu_partial(c);
	c->partial = NULL;

	if (partial_page)
		__unfreeze_partials(s, partial_page);
}

/*
 * Put a page that was just frozen (in __slab_free|get_partial_node) into a
 * partial page slot if available.
>>>>>>> upstream/android-13
 *
 * If we did not find a slot then simply move all the partials to the
 * per node partial list.
 */
static void put_cpu_partial(struct kmem_cache *s, struct page *page, int drain)
{
<<<<<<< HEAD
#ifdef CONFIG_SLUB_CPU_PARTIAL
	struct page *oldpage;
	int pages;
	int pobjects;

	preempt_disable();
	do {
		pages = 0;
		pobjects = 0;
		oldpage = this_cpu_read(s->cpu_slab->partial);

		if (oldpage) {
			pobjects = oldpage->pobjects;
			pages = oldpage->pages;
			if (drain && pobjects > s->cpu_partial) {
				unsigned long flags;
				/*
				 * partial array is full. Move the existing
				 * set to the per node partial list.
				 */
				local_irq_save(flags);
				unfreeze_partials(s, this_cpu_ptr(s->cpu_slab));
				local_irq_restore(flags);
				oldpage = NULL;
				pobjects = 0;
				pages = 0;
				stat(s, CPU_PARTIAL_DRAIN);
			}
		}

		pages++;
		pobjects += page->objects - page->inuse;

		page->pages = pages;
		page->pobjects = pobjects;
		page->next = oldpage;

	} while (this_cpu_cmpxchg(s->cpu_slab->partial, oldpage, page)
								!= oldpage);
	if (unlikely(!s->cpu_partial)) {
		unsigned long flags;

		local_irq_save(flags);
		unfreeze_partials(s, this_cpu_ptr(s->cpu_slab));
		local_irq_restore(flags);
	}
	preempt_enable();
#endif
}

static inline void flush_slab(struct kmem_cache *s, struct kmem_cache_cpu *c)
{
	stat(s, CPUSLAB_FLUSH);
	deactivate_slab(s, c->page, c->freelist, c);

	c->tid = next_tid(c->tid);
}

/*
 * Flush cpu slab.
 *
 * Called from IPI handler with interrupts disabled.
 */
static inline void __flush_cpu_slab(struct kmem_cache *s, int cpu)
{
	struct kmem_cache_cpu *c = per_cpu_ptr(s->cpu_slab, cpu);

	if (likely(c)) {
		if (c->page)
			flush_slab(s, c);

		unfreeze_partials(s, c);
	}
}

static void flush_cpu_slab(void *d)
{
	struct kmem_cache *s = d;

	__flush_cpu_slab(s, smp_processor_id());
}

static bool has_cpu_slab(int cpu, void *info)
{
	struct kmem_cache *s = info;
=======
	struct page *oldpage;
	struct page *page_to_unfreeze = NULL;
	unsigned long flags;
	int pages = 0;
	int pobjects = 0;

	local_lock_irqsave(&s->cpu_slab->lock, flags);

	oldpage = this_cpu_read(s->cpu_slab->partial);

	if (oldpage) {
		if (drain && oldpage->pobjects > slub_cpu_partial(s)) {
			/*
			 * Partial array is full. Move the existing set to the
			 * per node partial list. Postpone the actual unfreezing
			 * outside of the critical section.
			 */
			page_to_unfreeze = oldpage;
			oldpage = NULL;
		} else {
			pobjects = oldpage->pobjects;
			pages = oldpage->pages;
		}
	}

	pages++;
	pobjects += page->objects - page->inuse;

	page->pages = pages;
	page->pobjects = pobjects;
	page->next = oldpage;

	this_cpu_write(s->cpu_slab->partial, page);

	local_unlock_irqrestore(&s->cpu_slab->lock, flags);

	if (page_to_unfreeze) {
		__unfreeze_partials(s, page_to_unfreeze);
		stat(s, CPU_PARTIAL_DRAIN);
	}
}

#else	/* CONFIG_SLUB_CPU_PARTIAL */

static inline void unfreeze_partials(struct kmem_cache *s) { }
static inline void unfreeze_partials_cpu(struct kmem_cache *s,
				  struct kmem_cache_cpu *c) { }

#endif	/* CONFIG_SLUB_CPU_PARTIAL */

static inline void flush_slab(struct kmem_cache *s, struct kmem_cache_cpu *c)
{
	unsigned long flags;
	struct page *page;
	void *freelist;

	local_lock_irqsave(&s->cpu_slab->lock, flags);

	page = c->page;
	freelist = c->freelist;

	c->page = NULL;
	c->freelist = NULL;
	c->tid = next_tid(c->tid);

	local_unlock_irqrestore(&s->cpu_slab->lock, flags);

	if (page) {
		deactivate_slab(s, page, freelist);
		stat(s, CPUSLAB_FLUSH);
	}
}

static inline void __flush_cpu_slab(struct kmem_cache *s, int cpu)
{
	struct kmem_cache_cpu *c = per_cpu_ptr(s->cpu_slab, cpu);
	void *freelist = c->freelist;
	struct page *page = c->page;

	c->page = NULL;
	c->freelist = NULL;
	c->tid = next_tid(c->tid);

	if (page) {
		deactivate_slab(s, page, freelist);
		stat(s, CPUSLAB_FLUSH);
	}

	unfreeze_partials_cpu(s, c);
}

struct slub_flush_work {
	struct work_struct work;
	struct kmem_cache *s;
	bool skip;
};

/*
 * Flush cpu slab.
 *
 * Called from CPU work handler with migration disabled.
 */
static void flush_cpu_slab(struct work_struct *w)
{
	struct kmem_cache *s;
	struct kmem_cache_cpu *c;
	struct slub_flush_work *sfw;

	sfw = container_of(w, struct slub_flush_work, work);

	s = sfw->s;
	c = this_cpu_ptr(s->cpu_slab);

	if (c->page)
		flush_slab(s, c);

	unfreeze_partials(s);
}

static bool has_cpu_slab(int cpu, struct kmem_cache *s)
{
>>>>>>> upstream/android-13
	struct kmem_cache_cpu *c = per_cpu_ptr(s->cpu_slab, cpu);

	return c->page || slub_percpu_partial(c);
}

<<<<<<< HEAD
static void flush_all(struct kmem_cache *s)
{
	on_each_cpu_cond(has_cpu_slab, flush_cpu_slab, s, 1, GFP_ATOMIC);
=======
static DEFINE_MUTEX(flush_lock);
static DEFINE_PER_CPU(struct slub_flush_work, slub_flush);

static void flush_all_cpus_locked(struct kmem_cache *s)
{
	struct slub_flush_work *sfw;
	unsigned int cpu;

	lockdep_assert_cpus_held();
	mutex_lock(&flush_lock);

	for_each_online_cpu(cpu) {
		sfw = &per_cpu(slub_flush, cpu);
		if (!has_cpu_slab(cpu, s)) {
			sfw->skip = true;
			continue;
		}
		INIT_WORK(&sfw->work, flush_cpu_slab);
		sfw->skip = false;
		sfw->s = s;
		schedule_work_on(cpu, &sfw->work);
	}

	for_each_online_cpu(cpu) {
		sfw = &per_cpu(slub_flush, cpu);
		if (sfw->skip)
			continue;
		flush_work(&sfw->work);
	}

	mutex_unlock(&flush_lock);
}

static void flush_all(struct kmem_cache *s)
{
	cpus_read_lock();
	flush_all_cpus_locked(s);
	cpus_read_unlock();
>>>>>>> upstream/android-13
}

/*
 * Use the cpu notifier to insure that the cpu slabs are flushed when
 * necessary.
 */
static int slub_cpu_dead(unsigned int cpu)
{
	struct kmem_cache *s;
<<<<<<< HEAD
	unsigned long flags;

	mutex_lock(&slab_mutex);
	list_for_each_entry(s, &slab_caches, list) {
		local_irq_save(flags);
		__flush_cpu_slab(s, cpu);
		local_irq_restore(flags);
	}
=======

	mutex_lock(&slab_mutex);
	list_for_each_entry(s, &slab_caches, list)
		__flush_cpu_slab(s, cpu);
>>>>>>> upstream/android-13
	mutex_unlock(&slab_mutex);
	return 0;
}

/*
 * Check if the objects in a per cpu structure fit numa
 * locality expectations.
 */
static inline int node_match(struct page *page, int node)
{
#ifdef CONFIG_NUMA
<<<<<<< HEAD
	if (!page || (node != NUMA_NO_NODE && page_to_nid(page) != node))
=======
	if (node != NUMA_NO_NODE && page_to_nid(page) != node)
>>>>>>> upstream/android-13
		return 0;
#endif
	return 1;
}

#ifdef CONFIG_SLUB_DEBUG
static int count_free(struct page *page)
{
	return page->objects - page->inuse;
}

static inline unsigned long node_nr_objs(struct kmem_cache_node *n)
{
	return atomic_long_read(&n->total_objects);
}
#endif /* CONFIG_SLUB_DEBUG */

#if defined(CONFIG_SLUB_DEBUG) || defined(CONFIG_SYSFS)
static unsigned long count_partial(struct kmem_cache_node *n,
					int (*get_count)(struct page *))
{
	unsigned long flags;
	unsigned long x = 0;
	struct page *page;

	spin_lock_irqsave(&n->list_lock, flags);
<<<<<<< HEAD
	list_for_each_entry(page, &n->partial, lru)
=======
	list_for_each_entry(page, &n->partial, slab_list)
>>>>>>> upstream/android-13
		x += get_count(page);
	spin_unlock_irqrestore(&n->list_lock, flags);
	return x;
}
#endif /* CONFIG_SLUB_DEBUG || CONFIG_SYSFS */

static noinline void
slab_out_of_memory(struct kmem_cache *s, gfp_t gfpflags, int nid)
{
#ifdef CONFIG_SLUB_DEBUG
	static DEFINE_RATELIMIT_STATE(slub_oom_rs, DEFAULT_RATELIMIT_INTERVAL,
				      DEFAULT_RATELIMIT_BURST);
	int node;
	struct kmem_cache_node *n;

	if ((gfpflags & __GFP_NOWARN) || !__ratelimit(&slub_oom_rs))
		return;

	pr_warn("SLUB: Unable to allocate memory on node %d, gfp=%#x(%pGg)\n",
		nid, gfpflags, &gfpflags);
	pr_warn("  cache: %s, object size: %u, buffer size: %u, default order: %u, min order: %u\n",
		s->name, s->object_size, s->size, oo_order(s->oo),
		oo_order(s->min));

	if (oo_order(s->min) > get_order(s->object_size))
		pr_warn("  %s debugging increased min order, use slub_debug=O to disable.\n",
			s->name);

	for_each_kmem_cache_node(s, node, n) {
		unsigned long nr_slabs;
		unsigned long nr_objs;
		unsigned long nr_free;

		nr_free  = count_partial(n, count_free);
		nr_slabs = node_nr_slabs(n);
		nr_objs  = node_nr_objs(n);

		pr_warn("  node %d: slabs: %ld, objs: %ld, free: %ld\n",
			node, nr_slabs, nr_objs, nr_free);
	}
#endif
}

<<<<<<< HEAD
static inline void *new_slab_objects(struct kmem_cache *s, gfp_t flags,
			int node, struct kmem_cache_cpu **pc)
{
	void *freelist;
	struct kmem_cache_cpu *c = *pc;
	struct page *page;

	WARN_ON_ONCE(s->ctor && (flags & __GFP_ZERO));

	freelist = get_partial(s, flags, node, c);

	if (freelist)
		return freelist;

	page = new_slab(s, flags, node);
	if (page) {
		c = raw_cpu_ptr(s->cpu_slab);
		if (c->page)
			flush_slab(s, c);

		/*
		 * No other reference to the page yet so we can
		 * muck around with it freely without cmpxchg
		 */
		freelist = page->freelist;
		page->freelist = NULL;

		stat(s, ALLOC_SLAB);
		c->page = page;
		*pc = c;
	} else
		freelist = NULL;

	return freelist;
}

=======
>>>>>>> upstream/android-13
static inline bool pfmemalloc_match(struct page *page, gfp_t gfpflags)
{
	if (unlikely(PageSlabPfmemalloc(page)))
		return gfp_pfmemalloc_allowed(gfpflags);

	return true;
}

/*
<<<<<<< HEAD
=======
 * A variant of pfmemalloc_match() that tests page flags without asserting
 * PageSlab. Intended for opportunistic checks before taking a lock and
 * rechecking that nobody else freed the page under us.
 */
static inline bool pfmemalloc_match_unsafe(struct page *page, gfp_t gfpflags)
{
	if (unlikely(__PageSlabPfmemalloc(page)))
		return gfp_pfmemalloc_allowed(gfpflags);

	return true;
}

/*
>>>>>>> upstream/android-13
 * Check the page->freelist of a page and either transfer the freelist to the
 * per cpu freelist or deactivate the page.
 *
 * The page is still frozen if the return value is not NULL.
 *
 * If this function returns NULL then the page has been unfrozen.
<<<<<<< HEAD
 *
 * This function must be called with interrupt disabled.
=======
>>>>>>> upstream/android-13
 */
static inline void *get_freelist(struct kmem_cache *s, struct page *page)
{
	struct page new;
	unsigned long counters;
	void *freelist;

<<<<<<< HEAD
=======
	lockdep_assert_held(this_cpu_ptr(&s->cpu_slab->lock));

>>>>>>> upstream/android-13
	do {
		freelist = page->freelist;
		counters = page->counters;

		new.counters = counters;
		VM_BUG_ON(!new.frozen);

		new.inuse = page->objects;
		new.frozen = freelist != NULL;

	} while (!__cmpxchg_double_slab(s, page,
		freelist, counters,
		NULL, new.counters,
		"get_freelist"));

	return freelist;
}

/*
 * Slow path. The lockless freelist is empty or we need to perform
 * debugging duties.
 *
 * Processing is still very fast if new objects have been freed to the
 * regular freelist. In that case we simply take over the regular freelist
 * as the lockless freelist and zap the regular freelist.
 *
 * If that is not working then we fall back to the partial lists. We take the
 * first element of the freelist as the object to allocate now and move the
 * rest of the freelist to the lockless freelist.
 *
 * And if we were unable to get a new slab from the partial slab lists then
 * we need to allocate a new slab. This is the slowest path since it involves
 * a call to the page allocator and the setup of a new slab.
 *
<<<<<<< HEAD
 * Version of __slab_alloc to use when we know that interrupts are
=======
 * Version of __slab_alloc to use when we know that preemption is
>>>>>>> upstream/android-13
 * already disabled (which is the case for bulk allocation).
 */
static void *___slab_alloc(struct kmem_cache *s, gfp_t gfpflags, int node,
			  unsigned long addr, struct kmem_cache_cpu *c)
{
	void *freelist;
	struct page *page;
<<<<<<< HEAD

	page = c->page;
=======
	unsigned long flags;

	stat(s, ALLOC_SLOWPATH);

reread_page:

	page = READ_ONCE(c->page);
>>>>>>> upstream/android-13
	if (!page) {
		/*
		 * if the node is not online or has no normal memory, just
		 * ignore the node constraint
		 */
		if (unlikely(node != NUMA_NO_NODE &&
<<<<<<< HEAD
			     !node_state(node, N_NORMAL_MEMORY)))
=======
			     !node_isset(node, slab_nodes)))
>>>>>>> upstream/android-13
			node = NUMA_NO_NODE;
		goto new_slab;
	}
redo:

	if (unlikely(!node_match(page, node))) {
		/*
		 * same as above but node_match() being false already
		 * implies node != NUMA_NO_NODE
		 */
<<<<<<< HEAD
		if (!node_state(node, N_NORMAL_MEMORY)) {
=======
		if (!node_isset(node, slab_nodes)) {
>>>>>>> upstream/android-13
			node = NUMA_NO_NODE;
			goto redo;
		} else {
			stat(s, ALLOC_NODE_MISMATCH);
<<<<<<< HEAD
			deactivate_slab(s, page, c->freelist, c);
			goto new_slab;
=======
			goto deactivate_slab;
>>>>>>> upstream/android-13
		}
	}

	/*
	 * By rights, we should be searching for a slab page that was
	 * PFMEMALLOC but right now, we are losing the pfmemalloc
	 * information when the page leaves the per-cpu allocator
	 */
<<<<<<< HEAD
	if (unlikely(!pfmemalloc_match(page, gfpflags))) {
		deactivate_slab(s, page, c->freelist, c);
		goto new_slab;
	}

	/* must check again c->freelist in case of cpu migration or IRQ */
=======
	if (unlikely(!pfmemalloc_match_unsafe(page, gfpflags)))
		goto deactivate_slab;

	/* must check again c->page in case we got preempted and it changed */
	local_lock_irqsave(&s->cpu_slab->lock, flags);
	if (unlikely(page != c->page)) {
		local_unlock_irqrestore(&s->cpu_slab->lock, flags);
		goto reread_page;
	}
>>>>>>> upstream/android-13
	freelist = c->freelist;
	if (freelist)
		goto load_freelist;

	freelist = get_freelist(s, page);

	if (!freelist) {
		c->page = NULL;
<<<<<<< HEAD
=======
		local_unlock_irqrestore(&s->cpu_slab->lock, flags);
>>>>>>> upstream/android-13
		stat(s, DEACTIVATE_BYPASS);
		goto new_slab;
	}

	stat(s, ALLOC_REFILL);

load_freelist:
<<<<<<< HEAD
=======

	lockdep_assert_held(this_cpu_ptr(&s->cpu_slab->lock));

>>>>>>> upstream/android-13
	/*
	 * freelist is pointing to the list of objects to be used.
	 * page is pointing to the page from which the objects are obtained.
	 * That page must be frozen for per cpu allocations to work.
	 */
	VM_BUG_ON(!c->page->frozen);
	c->freelist = get_freepointer(s, freelist);
	c->tid = next_tid(c->tid);
<<<<<<< HEAD
	return freelist;

new_slab:

	if (slub_percpu_partial(c)) {
		page = c->page = slub_percpu_partial(c);
		slub_set_percpu_partial(c, page);
=======
	local_unlock_irqrestore(&s->cpu_slab->lock, flags);
	return freelist;

deactivate_slab:

	local_lock_irqsave(&s->cpu_slab->lock, flags);
	if (page != c->page) {
		local_unlock_irqrestore(&s->cpu_slab->lock, flags);
		goto reread_page;
	}
	freelist = c->freelist;
	c->page = NULL;
	c->freelist = NULL;
	local_unlock_irqrestore(&s->cpu_slab->lock, flags);
	deactivate_slab(s, page, freelist);

new_slab:

	if (slub_percpu_partial(c)) {
		local_lock_irqsave(&s->cpu_slab->lock, flags);
		if (unlikely(c->page)) {
			local_unlock_irqrestore(&s->cpu_slab->lock, flags);
			goto reread_page;
		}
		if (unlikely(!slub_percpu_partial(c))) {
			local_unlock_irqrestore(&s->cpu_slab->lock, flags);
			/* we were preempted and partial list got empty */
			goto new_objects;
		}

		page = c->page = slub_percpu_partial(c);
		slub_set_percpu_partial(c, page);
		local_unlock_irqrestore(&s->cpu_slab->lock, flags);
>>>>>>> upstream/android-13
		stat(s, CPU_PARTIAL_ALLOC);
		goto redo;
	}

<<<<<<< HEAD
	freelist = new_slab_objects(s, gfpflags, node, &c);

	if (unlikely(!freelist)) {
=======
new_objects:

	freelist = get_partial(s, gfpflags, node, &page);
	if (freelist)
		goto check_new_page;

	slub_put_cpu_ptr(s->cpu_slab);
	page = new_slab(s, gfpflags, node);
	c = slub_get_cpu_ptr(s->cpu_slab);

	if (unlikely(!page)) {
>>>>>>> upstream/android-13
		slab_out_of_memory(s, gfpflags, node);
		return NULL;
	}

<<<<<<< HEAD
	page = c->page;
	if (likely(!kmem_cache_debug(s) && pfmemalloc_match(page, gfpflags)))
		goto load_freelist;

	/* Only entered in the debug case */
	if (kmem_cache_debug(s) &&
			!alloc_debug_processing(s, page, freelist, addr))
		goto new_slab;	/* Slab failed checks. Next slab needed */

	deactivate_slab(s, page, get_freepointer(s, freelist), c);
=======
	/*
	 * No other reference to the page yet so we can
	 * muck around with it freely without cmpxchg
	 */
	freelist = page->freelist;
	page->freelist = NULL;

	stat(s, ALLOC_SLAB);

check_new_page:

	if (kmem_cache_debug(s)) {
		if (!alloc_debug_processing(s, page, freelist, addr)) {
			/* Slab failed checks. Next slab needed */
			goto new_slab;
		} else {
			/*
			 * For debug case, we don't load freelist so that all
			 * allocations go through alloc_debug_processing()
			 */
			goto return_single;
		}
	}

	if (unlikely(!pfmemalloc_match(page, gfpflags)))
		/*
		 * For !pfmemalloc_match() case we don't load freelist so that
		 * we don't make further mismatched allocations easier.
		 */
		goto return_single;

retry_load_page:

	local_lock_irqsave(&s->cpu_slab->lock, flags);
	if (unlikely(c->page)) {
		void *flush_freelist = c->freelist;
		struct page *flush_page = c->page;

		c->page = NULL;
		c->freelist = NULL;
		c->tid = next_tid(c->tid);

		local_unlock_irqrestore(&s->cpu_slab->lock, flags);

		deactivate_slab(s, flush_page, flush_freelist);

		stat(s, CPUSLAB_FLUSH);

		goto retry_load_page;
	}
	c->page = page;

	goto load_freelist;

return_single:

	deactivate_slab(s, page, get_freepointer(s, freelist));
>>>>>>> upstream/android-13
	return freelist;
}

/*
<<<<<<< HEAD
 * Another one that disabled interrupt and compensates for possible
 * cpu changes by refetching the per cpu area pointer.
=======
 * A wrapper for ___slab_alloc() for contexts where preemption is not yet
 * disabled. Compensates for possible cpu changes by refetching the per cpu area
 * pointer.
>>>>>>> upstream/android-13
 */
static void *__slab_alloc(struct kmem_cache *s, gfp_t gfpflags, int node,
			  unsigned long addr, struct kmem_cache_cpu *c)
{
	void *p;
<<<<<<< HEAD
	unsigned long flags;

	local_irq_save(flags);
#ifdef CONFIG_PREEMPT
	/*
	 * We may have been preempted and rescheduled on a different
	 * cpu before disabling interrupts. Need to reload cpu area
	 * pointer.
	 */
	c = this_cpu_ptr(s->cpu_slab);
#endif

	p = ___slab_alloc(s, gfpflags, node, addr, c);
	local_irq_restore(flags);
=======

#ifdef CONFIG_PREEMPT_COUNT
	/*
	 * We may have been preempted and rescheduled on a different
	 * cpu before disabling preemption. Need to reload cpu area
	 * pointer.
	 */
	c = slub_get_cpu_ptr(s->cpu_slab);
#endif

	p = ___slab_alloc(s, gfpflags, node, addr, c);
#ifdef CONFIG_PREEMPT_COUNT
	slub_put_cpu_ptr(s->cpu_slab);
#endif
>>>>>>> upstream/android-13
	return p;
}

/*
 * If the object has been wiped upon free, make sure it's fully initialized by
 * zeroing out freelist pointer.
 */
static __always_inline void maybe_wipe_obj_freeptr(struct kmem_cache *s,
						   void *obj)
{
	if (unlikely(slab_want_init_on_free(s)) && obj)
<<<<<<< HEAD
		memset((void *)((char *)obj + s->offset), 0, sizeof(void *));
=======
		memset((void *)((char *)kasan_reset_tag(obj) + s->offset),
			0, sizeof(void *));
>>>>>>> upstream/android-13
}

/*
 * Inlined fastpath so that allocation functions (kmalloc, kmem_cache_alloc)
 * have the fastpath folded into their functions. So no function call
 * overhead for requests that can be satisfied on the fastpath.
 *
 * The fastpath works by first checking if the lockless freelist can be used.
 * If not then __slab_alloc is called for slow processing.
 *
 * Otherwise we can simply pick the next object from the lockless free list.
 */
static __always_inline void *slab_alloc_node(struct kmem_cache *s,
<<<<<<< HEAD
		gfp_t gfpflags, int node, unsigned long addr)
=======
		gfp_t gfpflags, int node, unsigned long addr, size_t orig_size)
>>>>>>> upstream/android-13
{
	void *object;
	struct kmem_cache_cpu *c;
	struct page *page;
	unsigned long tid;
<<<<<<< HEAD

	s = slab_pre_alloc_hook(s, gfpflags);
	if (!s)
		return NULL;
=======
	struct obj_cgroup *objcg = NULL;
	bool init = false;

	s = slab_pre_alloc_hook(s, &objcg, 1, gfpflags);
	if (!s)
		return NULL;

	object = kfence_alloc(s, orig_size, gfpflags);
	if (unlikely(object))
		goto out;

>>>>>>> upstream/android-13
redo:
	/*
	 * Must read kmem_cache cpu data via this cpu ptr. Preemption is
	 * enabled. We may switch back and forth between cpus while
	 * reading from one cpu area. That does not matter as long
	 * as we end up on the original cpu again when doing the cmpxchg.
	 *
<<<<<<< HEAD
	 * We should guarantee that tid and kmem_cache are retrieved on
	 * the same cpu. It could be different if CONFIG_PREEMPT so we need
	 * to check if it is matched or not.
	 */
	do {
		tid = this_cpu_read(s->cpu_slab->tid);
		c = raw_cpu_ptr(s->cpu_slab);
	} while (IS_ENABLED(CONFIG_PREEMPT) &&
		 unlikely(tid != READ_ONCE(c->tid)));
=======
	 * We must guarantee that tid and kmem_cache_cpu are retrieved on the
	 * same cpu. We read first the kmem_cache_cpu pointer and use it to read
	 * the tid. If we are preempted and switched to another cpu between the
	 * two reads, it's OK as the two are still associated with the same cpu
	 * and cmpxchg later will validate the cpu.
	 */
	c = raw_cpu_ptr(s->cpu_slab);
	tid = READ_ONCE(c->tid);
>>>>>>> upstream/android-13

	/*
	 * Irqless object alloc/free algorithm used here depends on sequence
	 * of fetching cpu_slab's data. tid should be fetched before anything
	 * on c to guarantee that object and page associated with previous tid
	 * won't be used with current tid. If we fetch tid first, object and
	 * page could be one associated with next tid and our alloc/free
	 * request will be failed. In this case, we will retry. So, no problem.
	 */
	barrier();

	/*
	 * The transaction ids are globally unique per cpu and per operation on
	 * a per cpu queue. Thus they can be guarantee that the cmpxchg_double
	 * occurs on the right processor and that there was no operation on the
	 * linked list in between.
	 */

	object = c->freelist;
	page = c->page;
<<<<<<< HEAD
	if (unlikely(!object || !node_match(page, node))) {
		object = __slab_alloc(s, gfpflags, node, addr, c);
		stat(s, ALLOC_SLOWPATH);
=======
	/*
	 * We cannot use the lockless fastpath on PREEMPT_RT because if a
	 * slowpath has taken the local_lock_irqsave(), it is not protected
	 * against a fast path operation in an irq handler. So we need to take
	 * the slow path which uses local_lock. It is still relatively fast if
	 * there is a suitable cpu freelist.
	 */
	if (IS_ENABLED(CONFIG_PREEMPT_RT) ||
	    unlikely(!object || !page || !node_match(page, node))) {
		object = __slab_alloc(s, gfpflags, node, addr, c);
>>>>>>> upstream/android-13
	} else {
		void *next_object = get_freepointer_safe(s, object);

		/*
		 * The cmpxchg will only match if there was no additional
		 * operation and if we are on the right processor.
		 *
		 * The cmpxchg does the following atomically (without lock
		 * semantics!)
		 * 1. Relocate first pointer to the current per cpu area.
		 * 2. Verify that tid and freelist have not been changed
		 * 3. If they were not changed replace tid and freelist
		 *
		 * Since this is without lock semantics the protection is only
		 * against code executing on this cpu *not* from access by
		 * other cpus.
		 */
		if (unlikely(!this_cpu_cmpxchg_double(
				s->cpu_slab->freelist, s->cpu_slab->tid,
				object, tid,
				next_object, next_tid(tid)))) {

			note_cmpxchg_failure("slab_alloc", s, tid);
			goto redo;
		}
		prefetch_freepointer(s, next_object);
		stat(s, ALLOC_FASTPATH);
	}

	maybe_wipe_obj_freeptr(s, object);
<<<<<<< HEAD

	if (unlikely(slab_want_init_on_alloc(gfpflags, s)) && object)
		memset(object, 0, s->object_size);

	slab_post_alloc_hook(s, gfpflags, 1, &object);
=======
	init = slab_want_init_on_alloc(gfpflags, s);

out:
	slab_post_alloc_hook(s, objcg, gfpflags, 1, &object, init);
>>>>>>> upstream/android-13

	return object;
}

static __always_inline void *slab_alloc(struct kmem_cache *s,
<<<<<<< HEAD
		gfp_t gfpflags, unsigned long addr)
{
	return slab_alloc_node(s, gfpflags, NUMA_NO_NODE, addr);
=======
		gfp_t gfpflags, unsigned long addr, size_t orig_size)
{
	return slab_alloc_node(s, gfpflags, NUMA_NO_NODE, addr, orig_size);
>>>>>>> upstream/android-13
}

void *kmem_cache_alloc(struct kmem_cache *s, gfp_t gfpflags)
{
<<<<<<< HEAD
	void *ret = slab_alloc(s, gfpflags, _RET_IP_);
=======
	void *ret = slab_alloc(s, gfpflags, _RET_IP_, s->object_size);
>>>>>>> upstream/android-13

	trace_kmem_cache_alloc(_RET_IP_, ret, s->object_size,
				s->size, gfpflags);

	return ret;
}
EXPORT_SYMBOL(kmem_cache_alloc);

#ifdef CONFIG_TRACING
void *kmem_cache_alloc_trace(struct kmem_cache *s, gfp_t gfpflags, size_t size)
{
<<<<<<< HEAD
	void *ret = slab_alloc(s, gfpflags, _RET_IP_);
=======
	void *ret = slab_alloc(s, gfpflags, _RET_IP_, size);
>>>>>>> upstream/android-13
	trace_kmalloc(_RET_IP_, ret, size, s->size, gfpflags);
	ret = kasan_kmalloc(s, ret, size, gfpflags);
	return ret;
}
EXPORT_SYMBOL(kmem_cache_alloc_trace);
#endif

#ifdef CONFIG_NUMA
void *kmem_cache_alloc_node(struct kmem_cache *s, gfp_t gfpflags, int node)
{
<<<<<<< HEAD
	void *ret = slab_alloc_node(s, gfpflags, node, _RET_IP_);
=======
	void *ret = slab_alloc_node(s, gfpflags, node, _RET_IP_, s->object_size);
>>>>>>> upstream/android-13

	trace_kmem_cache_alloc_node(_RET_IP_, ret,
				    s->object_size, s->size, gfpflags, node);

	return ret;
}
EXPORT_SYMBOL(kmem_cache_alloc_node);

#ifdef CONFIG_TRACING
void *kmem_cache_alloc_node_trace(struct kmem_cache *s,
				    gfp_t gfpflags,
				    int node, size_t size)
{
<<<<<<< HEAD
	void *ret = slab_alloc_node(s, gfpflags, node, _RET_IP_);
=======
	void *ret = slab_alloc_node(s, gfpflags, node, _RET_IP_, size);
>>>>>>> upstream/android-13

	trace_kmalloc_node(_RET_IP_, ret,
			   size, s->size, gfpflags, node);

	ret = kasan_kmalloc(s, ret, size, gfpflags);
	return ret;
}
EXPORT_SYMBOL(kmem_cache_alloc_node_trace);
#endif
<<<<<<< HEAD
#endif
=======
#endif	/* CONFIG_NUMA */
>>>>>>> upstream/android-13

/*
 * Slow path handling. This may still be called frequently since objects
 * have a longer lifetime than the cpu slabs in most processing loads.
 *
 * So we still attempt to reduce cache line usage. Just take the slab
 * lock and free the item. If there is no additional partial page
 * handling required then we can return immediately.
 */
static void __slab_free(struct kmem_cache *s, struct page *page,
			void *head, void *tail, int cnt,
			unsigned long addr)

{
	void *prior;
	int was_frozen;
	struct page new;
	unsigned long counters;
	struct kmem_cache_node *n = NULL;
<<<<<<< HEAD
	unsigned long uninitialized_var(flags);

	stat(s, FREE_SLOWPATH);

=======
	unsigned long flags;

	stat(s, FREE_SLOWPATH);

	if (kfence_free(head))
		return;

>>>>>>> upstream/android-13
	if (kmem_cache_debug(s) &&
	    !free_debug_processing(s, page, head, tail, cnt, addr))
		return;

	do {
		if (unlikely(n)) {
			spin_unlock_irqrestore(&n->list_lock, flags);
			n = NULL;
		}
		prior = page->freelist;
		counters = page->counters;
		set_freepointer(s, tail, prior);
		new.counters = counters;
		was_frozen = new.frozen;
		new.inuse -= cnt;
		if ((!new.inuse || !prior) && !was_frozen) {

			if (kmem_cache_has_cpu_partial(s) && !prior) {

				/*
				 * Slab was on no list before and will be
				 * partially empty
				 * We can defer the list move and instead
				 * freeze it.
				 */
				new.frozen = 1;

			} else { /* Needs to be taken off a list */

				n = get_node(s, page_to_nid(page));
				/*
				 * Speculatively acquire the list_lock.
				 * If the cmpxchg does not succeed then we may
				 * drop the list_lock without any processing.
				 *
				 * Otherwise the list_lock will synchronize with
				 * other processors updating the list of slabs.
				 */
				spin_lock_irqsave(&n->list_lock, flags);

			}
		}

	} while (!cmpxchg_double_slab(s, page,
		prior, counters,
		head, new.counters,
		"__slab_free"));

	if (likely(!n)) {

<<<<<<< HEAD
		/*
		 * If we just froze the page then put it onto the
		 * per cpu partial list.
		 */
		if (new.frozen && !was_frozen) {
			put_cpu_partial(s, page, 1);
			stat(s, CPU_PARTIAL_FREE);
		}
		/*
		 * The list lock was not taken therefore no list
		 * activity can be necessary.
		 */
		if (was_frozen)
			stat(s, FREE_FROZEN);
=======
		if (likely(was_frozen)) {
			/*
			 * The list lock was not taken therefore no list
			 * activity can be necessary.
			 */
			stat(s, FREE_FROZEN);
		} else if (new.frozen) {
			/*
			 * If we just froze the page then put it onto the
			 * per cpu partial list.
			 */
			put_cpu_partial(s, page, 1);
			stat(s, CPU_PARTIAL_FREE);
		}

>>>>>>> upstream/android-13
		return;
	}

	if (unlikely(!new.inuse && n->nr_partial >= s->min_partial))
		goto slab_empty;

	/*
	 * Objects left in the slab. If it was not on the partial list before
	 * then add it.
	 */
	if (!kmem_cache_has_cpu_partial(s) && unlikely(!prior)) {
<<<<<<< HEAD
		if (kmem_cache_debug(s))
			remove_full(s, n, page);
=======
		remove_full(s, n, page);
>>>>>>> upstream/android-13
		add_partial(n, page, DEACTIVATE_TO_TAIL);
		stat(s, FREE_ADD_PARTIAL);
	}
	spin_unlock_irqrestore(&n->list_lock, flags);
	return;

slab_empty:
	if (prior) {
		/*
		 * Slab on the partial list.
		 */
		remove_partial(n, page);
		stat(s, FREE_REMOVE_PARTIAL);
	} else {
		/* Slab must be on the full list */
		remove_full(s, n, page);
	}

	spin_unlock_irqrestore(&n->list_lock, flags);
	stat(s, FREE_SLAB);
	discard_slab(s, page);
}

/*
 * Fastpath with forced inlining to produce a kfree and kmem_cache_free that
 * can perform fastpath freeing without additional function calls.
 *
 * The fastpath is only possible if we are freeing to the current cpu slab
 * of this processor. This typically the case if we have just allocated
 * the item before.
 *
 * If fastpath is not possible then fall back to __slab_free where we deal
 * with all sorts of special processing.
 *
 * Bulk free of a freelist with several objects (all pointing to the
 * same page) possible by specifying head and tail ptr, plus objects
 * count (cnt). Bulk free indicated by tail pointer being set.
 */
static __always_inline void do_slab_free(struct kmem_cache *s,
				struct page *page, void *head, void *tail,
				int cnt, unsigned long addr)
{
	void *tail_obj = tail ? : head;
	struct kmem_cache_cpu *c;
	unsigned long tid;
<<<<<<< HEAD
=======

	/* memcg_slab_free_hook() is already called for bulk free. */
	if (!tail)
		memcg_slab_free_hook(s, &head, 1);
>>>>>>> upstream/android-13
redo:
	/*
	 * Determine the currently cpus per cpu slab.
	 * The cpu may change afterward. However that does not matter since
	 * data is retrieved via this pointer. If we are on the same cpu
	 * during the cmpxchg then the free will succeed.
	 */
<<<<<<< HEAD
	do {
		tid = this_cpu_read(s->cpu_slab->tid);
		c = raw_cpu_ptr(s->cpu_slab);
	} while (IS_ENABLED(CONFIG_PREEMPT) &&
		 unlikely(tid != READ_ONCE(c->tid)));
=======
	c = raw_cpu_ptr(s->cpu_slab);
	tid = READ_ONCE(c->tid);
>>>>>>> upstream/android-13

	/* Same with comment on barrier() in slab_alloc_node() */
	barrier();

	if (likely(page == c->page)) {
<<<<<<< HEAD
=======
#ifndef CONFIG_PREEMPT_RT
>>>>>>> upstream/android-13
		void **freelist = READ_ONCE(c->freelist);

		set_freepointer(s, tail_obj, freelist);

		if (unlikely(!this_cpu_cmpxchg_double(
				s->cpu_slab->freelist, s->cpu_slab->tid,
				freelist, tid,
				head, next_tid(tid)))) {

			note_cmpxchg_failure("slab_free", s, tid);
			goto redo;
		}
<<<<<<< HEAD
=======
#else /* CONFIG_PREEMPT_RT */
		/*
		 * We cannot use the lockless fastpath on PREEMPT_RT because if
		 * a slowpath has taken the local_lock_irqsave(), it is not
		 * protected against a fast path operation in an irq handler. So
		 * we need to take the local_lock. We shouldn't simply defer to
		 * __slab_free() as that wouldn't use the cpu freelist at all.
		 */
		void **freelist;

		local_lock(&s->cpu_slab->lock);
		c = this_cpu_ptr(s->cpu_slab);
		if (unlikely(page != c->page)) {
			local_unlock(&s->cpu_slab->lock);
			goto redo;
		}
		tid = c->tid;
		freelist = c->freelist;

		set_freepointer(s, tail_obj, freelist);
		c->freelist = head;
		c->tid = next_tid(tid);

		local_unlock(&s->cpu_slab->lock);
#endif
>>>>>>> upstream/android-13
		stat(s, FREE_FASTPATH);
	} else
		__slab_free(s, page, head, tail_obj, cnt, addr);

}

static __always_inline void slab_free(struct kmem_cache *s, struct page *page,
				      void *head, void *tail, int cnt,
				      unsigned long addr)
{
	/*
	 * With KASAN enabled slab_free_freelist_hook modifies the freelist
	 * to remove objects, whose reuse must be delayed.
	 */
<<<<<<< HEAD
	if (slab_free_freelist_hook(s, &head, &tail))
=======
	if (slab_free_freelist_hook(s, &head, &tail, &cnt))
>>>>>>> upstream/android-13
		do_slab_free(s, page, head, tail, cnt, addr);
}

#ifdef CONFIG_KASAN_GENERIC
void ___cache_free(struct kmem_cache *cache, void *x, unsigned long addr)
{
	do_slab_free(cache, virt_to_head_page(x), x, NULL, 1, addr);
}
#endif

void kmem_cache_free(struct kmem_cache *s, void *x)
{
	s = cache_from_obj(s, x);
	if (!s)
		return;
	slab_free(s, virt_to_head_page(x), x, NULL, 1, _RET_IP_);
<<<<<<< HEAD
	trace_kmem_cache_free(_RET_IP_, x);
=======
	trace_kmem_cache_free(_RET_IP_, x, s->name);
>>>>>>> upstream/android-13
}
EXPORT_SYMBOL(kmem_cache_free);

struct detached_freelist {
	struct page *page;
	void *tail;
	void *freelist;
	int cnt;
	struct kmem_cache *s;
};

<<<<<<< HEAD
=======
static inline void free_nonslab_page(struct page *page, void *object)
{
	unsigned int order = compound_order(page);

	VM_BUG_ON_PAGE(!PageCompound(page), page);
	kfree_hook(object);
	mod_lruvec_page_state(page, NR_SLAB_UNRECLAIMABLE_B, -(PAGE_SIZE << order));
	__free_pages(page, order);
}

>>>>>>> upstream/android-13
/*
 * This function progressively scans the array with free objects (with
 * a limited look ahead) and extract objects belonging to the same
 * page.  It builds a detached freelist directly within the given
 * page/objects.  This can happen without any need for
 * synchronization, because the objects are owned by running process.
 * The freelist is build up as a single linked list in the objects.
 * The idea is, that this detached freelist can then be bulk
 * transferred to the real freelist(s), but only requiring a single
 * synchronization primitive.  Look ahead in the array is limited due
 * to performance reasons.
 */
static inline
int build_detached_freelist(struct kmem_cache *s, size_t size,
			    void **p, struct detached_freelist *df)
{
	size_t first_skipped_index = 0;
	int lookahead = 3;
	void *object;
	struct page *page;

	/* Always re-init detached_freelist */
	df->page = NULL;

	do {
		object = p[--size];
		/* Do we need !ZERO_OR_NULL_PTR(object) here? (for kfree) */
	} while (!object && size);

	if (!object)
		return 0;

	page = virt_to_head_page(object);
	if (!s) {
		/* Handle kalloc'ed objects */
		if (unlikely(!PageSlab(page))) {
<<<<<<< HEAD
			BUG_ON(!PageCompound(page));
			kfree_hook(object);
			__free_pages(page, compound_order(page));
=======
			free_nonslab_page(page, object);
>>>>>>> upstream/android-13
			p[size] = NULL; /* mark object processed */
			return size;
		}
		/* Derive kmem_cache from object */
		df->s = page->slab_cache;
	} else {
		df->s = cache_from_obj(s, object); /* Support for memcg */
	}

<<<<<<< HEAD
=======
	if (is_kfence_address(object)) {
		slab_free_hook(df->s, object, false);
		__kfence_free(object);
		p[size] = NULL; /* mark object processed */
		return size;
	}

>>>>>>> upstream/android-13
	/* Start new detached freelist */
	df->page = page;
	set_freepointer(df->s, object, NULL);
	df->tail = object;
	df->freelist = object;
	p[size] = NULL; /* mark object processed */
	df->cnt = 1;

	while (size) {
		object = p[--size];
		if (!object)
			continue; /* Skip processed objects */

		/* df->page is always set at this point */
		if (df->page == virt_to_head_page(object)) {
			/* Opportunity build freelist */
			set_freepointer(df->s, object, df->freelist);
			df->freelist = object;
			df->cnt++;
			p[size] = NULL; /* mark object processed */

			continue;
		}

		/* Limit look ahead search */
		if (!--lookahead)
			break;

		if (!first_skipped_index)
			first_skipped_index = size + 1;
	}

	return first_skipped_index;
}

/* Note that interrupts must be enabled when calling this function. */
void kmem_cache_free_bulk(struct kmem_cache *s, size_t size, void **p)
{
	if (WARN_ON(!size))
		return;

<<<<<<< HEAD
=======
	memcg_slab_free_hook(s, p, size);
>>>>>>> upstream/android-13
	do {
		struct detached_freelist df;

		size = build_detached_freelist(s, size, p, &df);
		if (!df.page)
			continue;

<<<<<<< HEAD
		slab_free(df.s, df.page, df.freelist, df.tail, df.cnt,_RET_IP_);
=======
		slab_free(df.s, df.page, df.freelist, df.tail, df.cnt, _RET_IP_);
>>>>>>> upstream/android-13
	} while (likely(size));
}
EXPORT_SYMBOL(kmem_cache_free_bulk);

/* Note that interrupts must be enabled when calling this function. */
int kmem_cache_alloc_bulk(struct kmem_cache *s, gfp_t flags, size_t size,
			  void **p)
{
	struct kmem_cache_cpu *c;
	int i;
<<<<<<< HEAD

	/* memcg and kmem_cache debug support */
	s = slab_pre_alloc_hook(s, flags);
=======
	struct obj_cgroup *objcg = NULL;

	/* memcg and kmem_cache debug support */
	s = slab_pre_alloc_hook(s, &objcg, size, flags);
>>>>>>> upstream/android-13
	if (unlikely(!s))
		return false;
	/*
	 * Drain objects in the per cpu slab, while disabling local
	 * IRQs, which protects against PREEMPT and interrupts
	 * handlers invoking normal fastpath.
	 */
<<<<<<< HEAD
	local_irq_disable();
	c = this_cpu_ptr(s->cpu_slab);

	for (i = 0; i < size; i++) {
		void *object = c->freelist;

=======
	c = slub_get_cpu_ptr(s->cpu_slab);
	local_lock_irq(&s->cpu_slab->lock);

	for (i = 0; i < size; i++) {
		void *object = kfence_alloc(s, s->object_size, flags);

		if (unlikely(object)) {
			p[i] = object;
			continue;
		}

		object = c->freelist;
>>>>>>> upstream/android-13
		if (unlikely(!object)) {
			/*
			 * We may have removed an object from c->freelist using
			 * the fastpath in the previous iteration; in that case,
			 * c->tid has not been bumped yet.
			 * Since ___slab_alloc() may reenable interrupts while
			 * allocating memory, we should bump c->tid now.
			 */
			c->tid = next_tid(c->tid);

<<<<<<< HEAD
=======
			local_unlock_irq(&s->cpu_slab->lock);

>>>>>>> upstream/android-13
			/*
			 * Invoking slow path likely have side-effect
			 * of re-populating per CPU c->freelist
			 */
			p[i] = ___slab_alloc(s, flags, NUMA_NO_NODE,
					    _RET_IP_, c);
			if (unlikely(!p[i]))
				goto error;

			c = this_cpu_ptr(s->cpu_slab);
			maybe_wipe_obj_freeptr(s, p[i]);

<<<<<<< HEAD
=======
			local_lock_irq(&s->cpu_slab->lock);

>>>>>>> upstream/android-13
			continue; /* goto for-loop */
		}
		c->freelist = get_freepointer(s, object);
		p[i] = object;
		maybe_wipe_obj_freeptr(s, p[i]);
	}
	c->tid = next_tid(c->tid);
<<<<<<< HEAD
	local_irq_enable();

	/* Clear memory outside IRQ disabled fastpath loop */
	if (unlikely(slab_want_init_on_alloc(flags, s))) {
		int j;

		for (j = 0; j < i; j++)
			memset(p[j], 0, s->object_size);
	}

	/* memcg and kmem_cache debug support */
	slab_post_alloc_hook(s, flags, size, p);
	return i;
error:
	local_irq_enable();
	slab_post_alloc_hook(s, flags, i, p);
=======
	local_unlock_irq(&s->cpu_slab->lock);
	slub_put_cpu_ptr(s->cpu_slab);

	/*
	 * memcg and kmem_cache debug support and memory initialization.
	 * Done outside of the IRQ disabled fastpath loop.
	 */
	slab_post_alloc_hook(s, objcg, flags, size, p,
				slab_want_init_on_alloc(flags, s));
	return i;
error:
	slub_put_cpu_ptr(s->cpu_slab);
	slab_post_alloc_hook(s, objcg, flags, i, p, false);
>>>>>>> upstream/android-13
	__kmem_cache_free_bulk(s, i, p);
	return 0;
}
EXPORT_SYMBOL(kmem_cache_alloc_bulk);


/*
 * Object placement in a slab is made very easy because we always start at
 * offset 0. If we tune the size of the object to the alignment then we can
 * get the required alignment by putting one properly sized object after
 * another.
 *
 * Notice that the allocation order determines the sizes of the per cpu
 * caches. Each processor has always one slab available for allocations.
 * Increasing the allocation order reduces the number of times that slabs
 * must be moved on and off the partial lists and is therefore a factor in
 * locking overhead.
 */

/*
<<<<<<< HEAD
 * Mininum / Maximum order of slab pages. This influences locking overhead
=======
 * Minimum / Maximum order of slab pages. This influences locking overhead
>>>>>>> upstream/android-13
 * and slab fragmentation. A higher order reduces the number of partial slabs
 * and increases the number of allocations possible without having to
 * take the list_lock.
 */
static unsigned int slub_min_order;
static unsigned int slub_max_order = PAGE_ALLOC_COSTLY_ORDER;
static unsigned int slub_min_objects;

/*
 * Calculate the order of allocation given an slab object size.
 *
 * The order of allocation has significant impact on performance and other
 * system components. Generally order 0 allocations should be preferred since
 * order 0 does not cause fragmentation in the page allocator. Larger objects
 * be problematic to put into order 0 slabs because there may be too much
 * unused space left. We go to a higher order if more than 1/16th of the slab
 * would be wasted.
 *
 * In order to reach satisfactory performance we must ensure that a minimum
 * number of objects is in one slab. Otherwise we may generate too much
 * activity on the partial lists which requires taking the list_lock. This is
 * less a concern for large slabs though which are rarely used.
 *
 * slub_max_order specifies the order where we begin to stop considering the
 * number of objects in a slab as critical. If we reach slub_max_order then
 * we try to keep the page order as low as possible. So we accept more waste
 * of space in favor of a small page order.
 *
 * Higher order allocations also allow the placement of more objects in a
 * slab and thereby reduce object handling overhead. If the user has
<<<<<<< HEAD
 * requested a higher mininum order then we start with that one instead of
=======
 * requested a higher minimum order then we start with that one instead of
>>>>>>> upstream/android-13
 * the smallest order which will fit the object.
 */
static inline unsigned int slab_order(unsigned int size,
		unsigned int min_objects, unsigned int max_order,
		unsigned int fract_leftover)
{
	unsigned int min_order = slub_min_order;
	unsigned int order;

	if (order_objects(min_order, size) > MAX_OBJS_PER_PAGE)
		return get_order(size * MAX_OBJS_PER_PAGE) - 1;

	for (order = max(min_order, (unsigned int)get_order(min_objects * size));
			order <= max_order; order++) {

		unsigned int slab_size = (unsigned int)PAGE_SIZE << order;
		unsigned int rem;

		rem = slab_size % size;

		if (rem <= slab_size / fract_leftover)
			break;
	}

	return order;
}

static inline int calculate_order(unsigned int size)
{
	unsigned int order;
	unsigned int min_objects;
	unsigned int max_objects;
<<<<<<< HEAD
=======
	unsigned int nr_cpus;
>>>>>>> upstream/android-13

	/*
	 * Attempt to find best configuration for a slab. This
	 * works by first attempting to generate a layout with
	 * the best configuration and backing off gradually.
	 *
	 * First we increase the acceptable waste in a slab. Then
	 * we reduce the minimum objects required in a slab.
	 */
	min_objects = slub_min_objects;
<<<<<<< HEAD
	if (!min_objects)
		min_objects = 4 * (fls(nr_cpu_ids) + 1);
=======
	if (!min_objects) {
		/*
		 * Some architectures will only update present cpus when
		 * onlining them, so don't trust the number if it's just 1. But
		 * we also don't want to use nr_cpu_ids always, as on some other
		 * architectures, there can be many possible cpus, but never
		 * onlined. Here we compromise between trying to avoid too high
		 * order on systems that appear larger than they are, and too
		 * low order on systems that appear smaller than they are.
		 */
		nr_cpus = num_present_cpus();
		if (nr_cpus <= 1)
			nr_cpus = nr_cpu_ids;
		min_objects = 4 * (fls(nr_cpus) + 1);
	}
>>>>>>> upstream/android-13
	max_objects = order_objects(slub_max_order, size);
	min_objects = min(min_objects, max_objects);

	while (min_objects > 1) {
		unsigned int fraction;

		fraction = 16;
		while (fraction >= 4) {
			order = slab_order(size, min_objects,
					slub_max_order, fraction);
			if (order <= slub_max_order)
				return order;
			fraction /= 2;
		}
		min_objects--;
	}

	/*
	 * We were unable to place multiple objects in a slab. Now
	 * lets see if we can place a single object there.
	 */
	order = slab_order(size, 1, slub_max_order, 1);
	if (order <= slub_max_order)
		return order;

	/*
	 * Doh this slab cannot be placed using slub_max_order.
	 */
	order = slab_order(size, 1, MAX_ORDER, 1);
	if (order < MAX_ORDER)
		return order;
	return -ENOSYS;
}

static void
init_kmem_cache_node(struct kmem_cache_node *n)
{
	n->nr_partial = 0;
	spin_lock_init(&n->list_lock);
	INIT_LIST_HEAD(&n->partial);
#ifdef CONFIG_SLUB_DEBUG
	atomic_long_set(&n->nr_slabs, 0);
	atomic_long_set(&n->total_objects, 0);
	INIT_LIST_HEAD(&n->full);
#endif
}

static inline int alloc_kmem_cache_cpus(struct kmem_cache *s)
{
	BUILD_BUG_ON(PERCPU_DYNAMIC_EARLY_SIZE <
			KMALLOC_SHIFT_HIGH * sizeof(struct kmem_cache_cpu));

	/*
	 * Must align to double word boundary for the double cmpxchg
	 * instructions to work; see __pcpu_double_call_return_bool().
	 */
	s->cpu_slab = __alloc_percpu(sizeof(struct kmem_cache_cpu),
				     2 * sizeof(void *));

	if (!s->cpu_slab)
		return 0;

	init_kmem_cache_cpus(s);

	return 1;
}

static struct kmem_cache *kmem_cache_node;

/*
 * No kmalloc_node yet so do it by hand. We know that this is the first
 * slab on the node for this slabcache. There are no concurrent accesses
 * possible.
 *
 * Note that this function only works on the kmem_cache_node
 * when allocating for the kmem_cache_node. This is used for bootstrapping
 * memory on a fresh node that has no slab structures yet.
 */
static void early_kmem_cache_node_alloc(int node)
{
	struct page *page;
	struct kmem_cache_node *n;

	BUG_ON(kmem_cache_node->size < sizeof(struct kmem_cache_node));

	page = new_slab(kmem_cache_node, GFP_NOWAIT, node);

	BUG_ON(!page);
	if (page_to_nid(page) != node) {
		pr_err("SLUB: Unable to allocate memory from node %d\n", node);
		pr_err("SLUB: Allocating a useless per node structure in order to be able to continue\n");
	}

	n = page->freelist;
	BUG_ON(!n);
#ifdef CONFIG_SLUB_DEBUG
	init_object(kmem_cache_node, n, SLUB_RED_ACTIVE);
	init_tracking(kmem_cache_node, n);
#endif
<<<<<<< HEAD
	n = kasan_kmalloc(kmem_cache_node, n, sizeof(struct kmem_cache_node),
		      GFP_KERNEL);
=======
	n = kasan_slab_alloc(kmem_cache_node, n, GFP_KERNEL, false);
>>>>>>> upstream/android-13
	page->freelist = get_freepointer(kmem_cache_node, n);
	page->inuse = 1;
	page->frozen = 0;
	kmem_cache_node->node[node] = n;
	init_kmem_cache_node(n);
	inc_slabs_node(kmem_cache_node, node, page->objects);

	/*
	 * No locks need to be taken here as it has just been
	 * initialized and there is no concurrent access.
	 */
	__add_partial(n, page, DEACTIVATE_TO_HEAD);
}

static void free_kmem_cache_nodes(struct kmem_cache *s)
{
	int node;
	struct kmem_cache_node *n;

	for_each_kmem_cache_node(s, node, n) {
		s->node[node] = NULL;
		kmem_cache_free(kmem_cache_node, n);
	}
}

void __kmem_cache_release(struct kmem_cache *s)
{
	cache_random_seq_destroy(s);
	free_percpu(s->cpu_slab);
	free_kmem_cache_nodes(s);
}

static int init_kmem_cache_nodes(struct kmem_cache *s)
{
	int node;

<<<<<<< HEAD
	for_each_node_state(node, N_NORMAL_MEMORY) {
=======
	for_each_node_mask(node, slab_nodes) {
>>>>>>> upstream/android-13
		struct kmem_cache_node *n;

		if (slab_state == DOWN) {
			early_kmem_cache_node_alloc(node);
			continue;
		}
		n = kmem_cache_alloc_node(kmem_cache_node,
						GFP_KERNEL, node);

		if (!n) {
			free_kmem_cache_nodes(s);
			return 0;
		}

		init_kmem_cache_node(n);
		s->node[node] = n;
	}
	return 1;
}

static void set_min_partial(struct kmem_cache *s, unsigned long min)
{
	if (min < MIN_PARTIAL)
		min = MIN_PARTIAL;
	else if (min > MAX_PARTIAL)
		min = MAX_PARTIAL;
	s->min_partial = min;
}

static void set_cpu_partial(struct kmem_cache *s)
{
#ifdef CONFIG_SLUB_CPU_PARTIAL
	/*
	 * cpu_partial determined the maximum number of objects kept in the
	 * per cpu partial lists of a processor.
	 *
	 * Per cpu partial lists mainly contain slabs that just have one
	 * object freed. If they are used for allocation then they can be
	 * filled up again with minimal effort. The slab will never hit the
	 * per node partial lists and therefore no locking will be required.
	 *
	 * This setting also determines
	 *
	 * A) The number of objects from per cpu partial slabs dumped to the
	 *    per node list when we reach the limit.
	 * B) The number of objects in cpu partial slabs to extract from the
	 *    per node list when we run out of per cpu objects. We only fetch
	 *    50% to keep some capacity around for frees.
	 */
	if (!kmem_cache_has_cpu_partial(s))
<<<<<<< HEAD
		s->cpu_partial = 0;
	else if (s->size >= PAGE_SIZE)
		s->cpu_partial = 2;
	else if (s->size >= 1024)
		s->cpu_partial = 6;
	else if (s->size >= 256)
		s->cpu_partial = 13;
	else
		s->cpu_partial = 30;
=======
		slub_set_cpu_partial(s, 0);
	else if (s->size >= PAGE_SIZE)
		slub_set_cpu_partial(s, 2);
	else if (s->size >= 1024)
		slub_set_cpu_partial(s, 6);
	else if (s->size >= 256)
		slub_set_cpu_partial(s, 13);
	else
		slub_set_cpu_partial(s, 30);
>>>>>>> upstream/android-13
#endif
}

/*
 * calculate_sizes() determines the order and the distribution of data within
 * a slab object.
 */
static int calculate_sizes(struct kmem_cache *s, int forced_order)
{
	slab_flags_t flags = s->flags;
	unsigned int size = s->object_size;
	unsigned int order;

	/*
	 * Round up object size to the next word boundary. We can only
	 * place the free pointer at word boundaries and this determines
	 * the possible location of the free pointer.
	 */
	size = ALIGN(size, sizeof(void *));

#ifdef CONFIG_SLUB_DEBUG
	/*
	 * Determine if we can poison the object itself. If the user of
	 * the slab may touch the object after free or before allocation
	 * then we should never poison the object itself.
	 */
	if ((flags & SLAB_POISON) && !(flags & SLAB_TYPESAFE_BY_RCU) &&
			!s->ctor)
		s->flags |= __OBJECT_POISON;
	else
		s->flags &= ~__OBJECT_POISON;


	/*
	 * If we are Redzoning then check if there is some space between the
	 * end of the object and the free pointer. If not then add an
	 * additional word to have some bytes to store Redzone information.
	 */
	if ((flags & SLAB_RED_ZONE) && size == s->object_size)
		size += sizeof(void *);
#endif

	/*
	 * With that we have determined the number of bytes in actual use
<<<<<<< HEAD
	 * by the object. This is the potential offset to the free pointer.
	 */
	s->inuse = size;

	if (((flags & (SLAB_TYPESAFE_BY_RCU | SLAB_POISON)) ||
		s->ctor)) {
=======
	 * by the object and redzoning.
	 */
	s->inuse = size;

	if ((flags & (SLAB_TYPESAFE_BY_RCU | SLAB_POISON)) ||
	    ((flags & SLAB_RED_ZONE) && s->object_size < sizeof(void *)) ||
	    s->ctor) {
>>>>>>> upstream/android-13
		/*
		 * Relocate free pointer after the object if it is not
		 * permitted to overwrite the first word of the object on
		 * kmem_cache_free.
		 *
		 * This is the case if we do RCU, have a constructor or
<<<<<<< HEAD
		 * destructor or are poisoning the objects.
		 */
		s->offset = size;
		size += sizeof(void *);
=======
		 * destructor, are poisoning the objects, or are
		 * redzoning an object smaller than sizeof(void *).
		 *
		 * The assumption that s->offset >= s->inuse means free
		 * pointer is outside of the object is used in the
		 * freeptr_outside_object() function. If that is no
		 * longer true, the function needs to be modified.
		 */
		s->offset = size;
		size += sizeof(void *);
	} else {
		/*
		 * Store freelist pointer near middle of object to keep
		 * it away from the edges of the object to avoid small
		 * sized over/underflows from neighboring allocations.
		 */
		s->offset = ALIGN_DOWN(s->object_size / 2, sizeof(void *));
>>>>>>> upstream/android-13
	}

#ifdef CONFIG_SLUB_DEBUG
	if (flags & SLAB_STORE_USER)
		/*
		 * Need to store information about allocs and frees after
		 * the object.
		 */
		size += 2 * sizeof(struct track);
#endif

	kasan_cache_create(s, &size, &s->flags);
#ifdef CONFIG_SLUB_DEBUG
	if (flags & SLAB_RED_ZONE) {
		/*
		 * Add some empty padding so that we can catch
		 * overwrites from earlier objects rather than let
		 * tracking information or the free pointer be
		 * corrupted if a user writes before the start
		 * of the object.
		 */
		size += sizeof(void *);

		s->red_left_pad = sizeof(void *);
		s->red_left_pad = ALIGN(s->red_left_pad, s->align);
		size += s->red_left_pad;
	}
#endif

	/*
	 * SLUB stores one object immediately after another beginning from
	 * offset 0. In order to align the objects we have to simply size
	 * each object to conform to the alignment.
	 */
	size = ALIGN(size, s->align);
	s->size = size;
<<<<<<< HEAD
=======
	s->reciprocal_size = reciprocal_value(size);
>>>>>>> upstream/android-13
	if (forced_order >= 0)
		order = forced_order;
	else
		order = calculate_order(size);

	if ((int)order < 0)
		return 0;

	s->allocflags = 0;
	if (order)
		s->allocflags |= __GFP_COMP;

	if (s->flags & SLAB_CACHE_DMA)
		s->allocflags |= GFP_DMA;

	if (s->flags & SLAB_CACHE_DMA32)
		s->allocflags |= GFP_DMA32;

	if (s->flags & SLAB_RECLAIM_ACCOUNT)
		s->allocflags |= __GFP_RECLAIMABLE;

	/*
	 * Determine the number of objects per slab
	 */
	s->oo = oo_make(order, size);
	s->min = oo_make(get_order(size), size);
	if (oo_objects(s->oo) > oo_objects(s->max))
		s->max = s->oo;

	return !!oo_objects(s->oo);
}

static int kmem_cache_open(struct kmem_cache *s, slab_flags_t flags)
{
<<<<<<< HEAD
	s->flags = kmem_cache_flags(s->size, flags, s->name, s->ctor);
=======
	s->flags = kmem_cache_flags(s->size, flags, s->name);
>>>>>>> upstream/android-13
#ifdef CONFIG_SLAB_FREELIST_HARDENED
	s->random = get_random_long();
#endif

	if (!calculate_sizes(s, -1))
		goto error;
	if (disable_higher_order_debug) {
		/*
		 * Disable debugging flags that store metadata if the min slab
		 * order increased.
		 */
		if (get_order(s->size) > get_order(s->object_size)) {
			s->flags &= ~DEBUG_METADATA_FLAGS;
			s->offset = 0;
			if (!calculate_sizes(s, -1))
				goto error;
		}
	}

#if defined(CONFIG_HAVE_CMPXCHG_DOUBLE) && \
    defined(CONFIG_HAVE_ALIGNED_STRUCT_PAGE)
	if (system_has_cmpxchg_double() && (s->flags & SLAB_NO_CMPXCHG) == 0)
		/* Enable fast mode */
		s->flags |= __CMPXCHG_DOUBLE;
#endif

	/*
	 * The larger the object size is, the more pages we want on the partial
	 * list to avoid pounding the page allocator excessively.
	 */
	set_min_partial(s, ilog2(s->size) / 2);

	set_cpu_partial(s);

#ifdef CONFIG_NUMA
	s->remote_node_defrag_ratio = 1000;
#endif

	/* Initialize the pre-computed randomized freelist if slab is up */
	if (slab_state >= UP) {
		if (init_cache_random_seq(s))
			goto error;
	}

	if (!init_kmem_cache_nodes(s))
		goto error;

	if (alloc_kmem_cache_cpus(s))
		return 0;

<<<<<<< HEAD
	free_kmem_cache_nodes(s);
error:
	if (flags & SLAB_PANIC)
		panic("Cannot create slab %s size=%u realsize=%u order=%u offset=%u flags=%lx\n",
		      s->name, s->size, s->size,
		      oo_order(s->oo), s->offset, (unsigned long)flags);
=======
error:
	__kmem_cache_release(s);
>>>>>>> upstream/android-13
	return -EINVAL;
}

static void list_slab_objects(struct kmem_cache *s, struct page *page,
<<<<<<< HEAD
							const char *text)
{
#ifdef CONFIG_SLUB_DEBUG
	void *addr = page_address(page);
	void *p;
	unsigned long *map = kcalloc(BITS_TO_LONGS(page->objects),
				     sizeof(long),
				     GFP_ATOMIC);
	if (!map)
		return;
	slab_err(s, page, text, s->name);
	slab_lock(page);

	get_map(s, page, map);
	for_each_object(p, s, addr, page->objects) {

		if (!test_bit(slab_index(p, s, addr), map)) {
			pr_err("INFO: Object 0x%px @offset=%tu\n", p, p - addr);
			print_tracking(s, p);
		}
	}
	slab_unlock(page);
	kfree(map);
=======
			      const char *text)
{
#ifdef CONFIG_SLUB_DEBUG
	void *addr = page_address(page);
	unsigned long flags;
	unsigned long *map;
	void *p;

	slab_err(s, page, text, s->name);
	slab_lock(page, &flags);

	map = get_map(s, page);
	for_each_object(p, s, addr, page->objects) {

		if (!test_bit(__obj_to_index(s, addr, p), map)) {
			pr_err("Object 0x%p @offset=%tu\n", p, p - addr);
			print_tracking(s, p);
		}
	}
	put_map(map);
	slab_unlock(page, &flags);
>>>>>>> upstream/android-13
#endif
}

/*
 * Attempt to free all partial slabs on a node.
 * This is called from __kmem_cache_shutdown(). We must take list_lock
 * because sysfs file might still access partial list after the shutdowning.
 */
static void free_partial(struct kmem_cache *s, struct kmem_cache_node *n)
{
	LIST_HEAD(discard);
	struct page *page, *h;

	BUG_ON(irqs_disabled());
	spin_lock_irq(&n->list_lock);
<<<<<<< HEAD
	list_for_each_entry_safe(page, h, &n->partial, lru) {
		if (!page->inuse) {
			remove_partial(n, page);
			list_add(&page->lru, &discard);
		} else {
			list_slab_objects(s, page,
			"Objects remaining in %s on __kmem_cache_shutdown()");
=======
	list_for_each_entry_safe(page, h, &n->partial, slab_list) {
		if (!page->inuse) {
			remove_partial(n, page);
			list_add(&page->slab_list, &discard);
		} else {
			list_slab_objects(s, page,
			  "Objects remaining in %s on __kmem_cache_shutdown()");
>>>>>>> upstream/android-13
		}
	}
	spin_unlock_irq(&n->list_lock);

<<<<<<< HEAD
	list_for_each_entry_safe(page, h, &discard, lru)
=======
	list_for_each_entry_safe(page, h, &discard, slab_list)
>>>>>>> upstream/android-13
		discard_slab(s, page);
}

bool __kmem_cache_empty(struct kmem_cache *s)
{
	int node;
	struct kmem_cache_node *n;

	for_each_kmem_cache_node(s, node, n)
		if (n->nr_partial || slabs_node(s, node))
			return false;
	return true;
}

/*
 * Release all resources used by a slab cache.
 */
int __kmem_cache_shutdown(struct kmem_cache *s)
{
	int node;
	struct kmem_cache_node *n;

<<<<<<< HEAD
	flush_all(s);
=======
	flush_all_cpus_locked(s);
>>>>>>> upstream/android-13
	/* Attempt to free all objects */
	for_each_kmem_cache_node(s, node, n) {
		free_partial(s, n);
		if (n->nr_partial || slabs_node(s, node))
			return 1;
	}
<<<<<<< HEAD
	sysfs_slab_remove(s);
	return 0;
}

=======
	return 0;
}

#ifdef CONFIG_PRINTK
void __kmem_obj_info(struct kmem_obj_info *kpp, void *object, struct page *page)
{
	void *base;
	int __maybe_unused i;
	unsigned int objnr;
	void *objp;
	void *objp0;
	struct kmem_cache *s = page->slab_cache;
	struct track __maybe_unused *trackp;

	kpp->kp_ptr = object;
	kpp->kp_page = page;
	kpp->kp_slab_cache = s;
	base = page_address(page);
	objp0 = kasan_reset_tag(object);
#ifdef CONFIG_SLUB_DEBUG
	objp = restore_red_left(s, objp0);
#else
	objp = objp0;
#endif
	objnr = obj_to_index(s, page, objp);
	kpp->kp_data_offset = (unsigned long)((char *)objp0 - (char *)objp);
	objp = base + s->size * objnr;
	kpp->kp_objp = objp;
	if (WARN_ON_ONCE(objp < base || objp >= base + page->objects * s->size || (objp - base) % s->size) ||
	    !(s->flags & SLAB_STORE_USER))
		return;
#ifdef CONFIG_SLUB_DEBUG
	objp = fixup_red_left(s, objp);
	trackp = get_track(s, objp, TRACK_ALLOC);
	kpp->kp_ret = (void *)trackp->addr;
#ifdef CONFIG_STACKTRACE
	for (i = 0; i < KS_ADDRS_COUNT && i < TRACK_ADDRS_COUNT; i++) {
		kpp->kp_stack[i] = (void *)trackp->addrs[i];
		if (!kpp->kp_stack[i])
			break;
	}

	trackp = get_track(s, objp, TRACK_FREE);
	for (i = 0; i < KS_ADDRS_COUNT && i < TRACK_ADDRS_COUNT; i++) {
		kpp->kp_free_stack[i] = (void *)trackp->addrs[i];
		if (!kpp->kp_free_stack[i])
			break;
	}
#endif
#endif
}
#endif

>>>>>>> upstream/android-13
/********************************************************************
 *		Kmalloc subsystem
 *******************************************************************/

static int __init setup_slub_min_order(char *str)
{
	get_option(&str, (int *)&slub_min_order);

	return 1;
}

__setup("slub_min_order=", setup_slub_min_order);

static int __init setup_slub_max_order(char *str)
{
	get_option(&str, (int *)&slub_max_order);
	slub_max_order = min(slub_max_order, (unsigned int)MAX_ORDER - 1);

	return 1;
}

__setup("slub_max_order=", setup_slub_max_order);

static int __init setup_slub_min_objects(char *str)
{
	get_option(&str, (int *)&slub_min_objects);

	return 1;
}

__setup("slub_min_objects=", setup_slub_min_objects);

void *__kmalloc(size_t size, gfp_t flags)
{
	struct kmem_cache *s;
	void *ret;

	if (unlikely(size > KMALLOC_MAX_CACHE_SIZE))
		return kmalloc_large(size, flags);

	s = kmalloc_slab(size, flags);

	if (unlikely(ZERO_OR_NULL_PTR(s)))
		return s;

<<<<<<< HEAD
	ret = slab_alloc(s, flags, _RET_IP_);
=======
	ret = slab_alloc(s, flags, _RET_IP_, size);
>>>>>>> upstream/android-13

	trace_kmalloc(_RET_IP_, ret, size, s->size, flags);

	ret = kasan_kmalloc(s, ret, size, flags);

	return ret;
}
EXPORT_SYMBOL(__kmalloc);

#ifdef CONFIG_NUMA
static void *kmalloc_large_node(size_t size, gfp_t flags, int node)
{
	struct page *page;
	void *ptr = NULL;
<<<<<<< HEAD

	flags |= __GFP_COMP;
	page = alloc_pages_node(node, flags, get_order(size));
	if (page)
		ptr = page_address(page);
=======
	unsigned int order = get_order(size);

	flags |= __GFP_COMP;
	page = alloc_pages_node(node, flags, order);
	if (page) {
		ptr = page_address(page);
		mod_lruvec_page_state(page, NR_SLAB_UNRECLAIMABLE_B,
				      PAGE_SIZE << order);
	}
>>>>>>> upstream/android-13

	return kmalloc_large_node_hook(ptr, size, flags);
}

void *__kmalloc_node(size_t size, gfp_t flags, int node)
{
	struct kmem_cache *s;
	void *ret;

	if (unlikely(size > KMALLOC_MAX_CACHE_SIZE)) {
		ret = kmalloc_large_node(size, flags, node);

		trace_kmalloc_node(_RET_IP_, ret,
				   size, PAGE_SIZE << get_order(size),
				   flags, node);

		return ret;
	}

	s = kmalloc_slab(size, flags);

	if (unlikely(ZERO_OR_NULL_PTR(s)))
		return s;

<<<<<<< HEAD
	ret = slab_alloc_node(s, flags, node, _RET_IP_);
=======
	ret = slab_alloc_node(s, flags, node, _RET_IP_, size);
>>>>>>> upstream/android-13

	trace_kmalloc_node(_RET_IP_, ret, size, s->size, flags, node);

	ret = kasan_kmalloc(s, ret, size, flags);

	return ret;
}
EXPORT_SYMBOL(__kmalloc_node);
<<<<<<< HEAD
#endif
=======
#endif	/* CONFIG_NUMA */
>>>>>>> upstream/android-13

#ifdef CONFIG_HARDENED_USERCOPY
/*
 * Rejects incorrectly sized objects and objects that are to be copied
 * to/from userspace but do not fall entirely within the containing slab
 * cache's usercopy region.
 *
 * Returns NULL if check passes, otherwise const char * to name of cache
 * to indicate an error.
 */
void __check_heap_object(const void *ptr, unsigned long n, struct page *page,
			 bool to_user)
{
	struct kmem_cache *s;
	unsigned int offset;
	size_t object_size;
<<<<<<< HEAD
=======
	bool is_kfence = is_kfence_address(ptr);
>>>>>>> upstream/android-13

	ptr = kasan_reset_tag(ptr);

	/* Find object and usable object size. */
	s = page->slab_cache;

	/* Reject impossible pointers. */
	if (ptr < page_address(page))
		usercopy_abort("SLUB object not in SLUB page?!", NULL,
			       to_user, 0, n);

	/* Find offset within object. */
<<<<<<< HEAD
	offset = (ptr - page_address(page)) % s->size;

	/* Adjust for redzone and reject if within the redzone. */
	if (kmem_cache_debug(s) && s->flags & SLAB_RED_ZONE) {
=======
	if (is_kfence)
		offset = ptr - kfence_object_start(ptr);
	else
		offset = (ptr - page_address(page)) % s->size;

	/* Adjust for redzone and reject if within the redzone. */
	if (!is_kfence && kmem_cache_debug_flags(s, SLAB_RED_ZONE)) {
>>>>>>> upstream/android-13
		if (offset < s->red_left_pad)
			usercopy_abort("SLUB object in left red zone",
				       s->name, to_user, offset, n);
		offset -= s->red_left_pad;
	}

	/* Allow address range falling entirely within usercopy region. */
	if (offset >= s->useroffset &&
	    offset - s->useroffset <= s->usersize &&
	    n <= s->useroffset - offset + s->usersize)
		return;

	/*
	 * If the copy is still within the allocated object, produce
	 * a warning instead of rejecting the copy. This is intended
	 * to be a temporary method to find any missing usercopy
	 * whitelists.
	 */
	object_size = slab_ksize(s);
	if (usercopy_fallback &&
	    offset <= object_size && n <= object_size - offset) {
		usercopy_warn("SLUB object", s->name, to_user, offset, n);
		return;
	}

	usercopy_abort("SLUB object", s->name, to_user, offset, n);
}
#endif /* CONFIG_HARDENED_USERCOPY */

<<<<<<< HEAD
static size_t __ksize(const void *object)
=======
size_t __ksize(const void *object)
>>>>>>> upstream/android-13
{
	struct page *page;

	if (unlikely(object == ZERO_SIZE_PTR))
		return 0;

	page = virt_to_head_page(object);

	if (unlikely(!PageSlab(page))) {
		WARN_ON(!PageCompound(page));
<<<<<<< HEAD
		return PAGE_SIZE << compound_order(page);
=======
		return page_size(page);
>>>>>>> upstream/android-13
	}

	return slab_ksize(page->slab_cache);
}
<<<<<<< HEAD

size_t ksize(const void *object)
{
	size_t size = __ksize(object);
	/* We assume that ksize callers could use whole allocated area,
	 * so we need to unpoison this area.
	 */
	kasan_unpoison_shadow(object, size);
	return size;
}
EXPORT_SYMBOL(ksize);
=======
EXPORT_SYMBOL(__ksize);
>>>>>>> upstream/android-13

void kfree(const void *x)
{
	struct page *page;
	void *object = (void *)x;

	trace_kfree(_RET_IP_, x);

	if (unlikely(ZERO_OR_NULL_PTR(x)))
		return;

	page = virt_to_head_page(x);
	if (unlikely(!PageSlab(page))) {
<<<<<<< HEAD
		BUG_ON(!PageCompound(page));
		kfree_hook(object);
		__free_pages(page, compound_order(page));
=======
		free_nonslab_page(page, object);
>>>>>>> upstream/android-13
		return;
	}
	slab_free(page->slab_cache, page, object, NULL, 1, _RET_IP_);
}
EXPORT_SYMBOL(kfree);

#define SHRINK_PROMOTE_MAX 32

/*
 * kmem_cache_shrink discards empty slabs and promotes the slabs filled
 * up most to the head of the partial lists. New allocations will then
 * fill those up and thus they can be removed from the partial lists.
 *
 * The slabs with the least items are placed last. This results in them
 * being allocated from last increasing the chance that the last objects
 * are freed in them.
 */
<<<<<<< HEAD
int __kmem_cache_shrink(struct kmem_cache *s)
=======
static int __kmem_cache_do_shrink(struct kmem_cache *s)
>>>>>>> upstream/android-13
{
	int node;
	int i;
	struct kmem_cache_node *n;
	struct page *page;
	struct page *t;
	struct list_head discard;
	struct list_head promote[SHRINK_PROMOTE_MAX];
	unsigned long flags;
	int ret = 0;

<<<<<<< HEAD
	flush_all(s);
=======
>>>>>>> upstream/android-13
	for_each_kmem_cache_node(s, node, n) {
		INIT_LIST_HEAD(&discard);
		for (i = 0; i < SHRINK_PROMOTE_MAX; i++)
			INIT_LIST_HEAD(promote + i);

		spin_lock_irqsave(&n->list_lock, flags);

		/*
		 * Build lists of slabs to discard or promote.
		 *
		 * Note that concurrent frees may occur while we hold the
		 * list_lock. page->inuse here is the upper limit.
		 */
<<<<<<< HEAD
		list_for_each_entry_safe(page, t, &n->partial, lru) {
=======
		list_for_each_entry_safe(page, t, &n->partial, slab_list) {
>>>>>>> upstream/android-13
			int free = page->objects - page->inuse;

			/* Do not reread page->inuse */
			barrier();

			/* We do not keep full slabs on the list */
			BUG_ON(free <= 0);

			if (free == page->objects) {
<<<<<<< HEAD
				list_move(&page->lru, &discard);
				n->nr_partial--;
			} else if (free <= SHRINK_PROMOTE_MAX)
				list_move(&page->lru, promote + free - 1);
=======
				list_move(&page->slab_list, &discard);
				n->nr_partial--;
			} else if (free <= SHRINK_PROMOTE_MAX)
				list_move(&page->slab_list, promote + free - 1);
>>>>>>> upstream/android-13
		}

		/*
		 * Promote the slabs filled up most to the head of the
		 * partial list.
		 */
		for (i = SHRINK_PROMOTE_MAX - 1; i >= 0; i--)
			list_splice(promote + i, &n->partial);

		spin_unlock_irqrestore(&n->list_lock, flags);

		/* Release empty slabs */
<<<<<<< HEAD
		list_for_each_entry_safe(page, t, &discard, lru)
=======
		list_for_each_entry_safe(page, t, &discard, slab_list)
>>>>>>> upstream/android-13
			discard_slab(s, page);

		if (slabs_node(s, node))
			ret = 1;
	}

	return ret;
}

<<<<<<< HEAD
#ifdef CONFIG_MEMCG
static void kmemcg_cache_deact_after_rcu(struct kmem_cache *s)
{
	/*
	 * Called with all the locks held after a sched RCU grace period.
	 * Even if @s becomes empty after shrinking, we can't know that @s
	 * doesn't have allocations already in-flight and thus can't
	 * destroy @s until the associated memcg is released.
	 *
	 * However, let's remove the sysfs files for empty caches here.
	 * Each cache has a lot of interface files which aren't
	 * particularly useful for empty draining caches; otherwise, we can
	 * easily end up with millions of unnecessary sysfs files on
	 * systems which have a lot of memory and transient cgroups.
	 */
	if (!__kmem_cache_shrink(s))
		sysfs_slab_remove(s);
}

void __kmemcg_cache_deactivate(struct kmem_cache *s)
{
	/*
	 * Disable empty slabs caching. Used to avoid pinning offline
	 * memory cgroups by kmem pages that can be freed.
	 */
	slub_set_cpu_partial(s, 0);
	s->min_partial = 0;

	/*
	 * s->cpu_partial is checked locklessly (see put_cpu_partial), so
	 * we have to make sure the change is visible before shrinking.
	 */
	slab_deactivate_memcg_cache_rcu_sched(s, kmemcg_cache_deact_after_rcu);
}
#endif

=======
int __kmem_cache_shrink(struct kmem_cache *s)
{
	flush_all(s);
	return __kmem_cache_do_shrink(s);
}

>>>>>>> upstream/android-13
static int slab_mem_going_offline_callback(void *arg)
{
	struct kmem_cache *s;

	mutex_lock(&slab_mutex);
<<<<<<< HEAD
	list_for_each_entry(s, &slab_caches, list)
		__kmem_cache_shrink(s);
=======
	list_for_each_entry(s, &slab_caches, list) {
		flush_all_cpus_locked(s);
		__kmem_cache_do_shrink(s);
	}
>>>>>>> upstream/android-13
	mutex_unlock(&slab_mutex);

	return 0;
}

static void slab_mem_offline_callback(void *arg)
{
<<<<<<< HEAD
	struct kmem_cache_node *n;
	struct kmem_cache *s;
=======
>>>>>>> upstream/android-13
	struct memory_notify *marg = arg;
	int offline_node;

	offline_node = marg->status_change_nid_normal;

	/*
	 * If the node still has available memory. we need kmem_cache_node
	 * for it yet.
	 */
	if (offline_node < 0)
		return;

	mutex_lock(&slab_mutex);
<<<<<<< HEAD
	list_for_each_entry(s, &slab_caches, list) {
		n = get_node(s, offline_node);
		if (n) {
			/*
			 * if n->nr_slabs > 0, slabs still exist on the node
			 * that is going down. We were unable to free them,
			 * and offline_pages() function shouldn't call this
			 * callback. So, we must fail.
			 */
			BUG_ON(slabs_node(s, offline_node));

			s->node[offline_node] = NULL;
			kmem_cache_free(kmem_cache_node, n);
		}
	}
=======
	node_clear(offline_node, slab_nodes);
	/*
	 * We no longer free kmem_cache_node structures here, as it would be
	 * racy with all get_node() users, and infeasible to protect them with
	 * slab_mutex.
	 */
>>>>>>> upstream/android-13
	mutex_unlock(&slab_mutex);
}

static int slab_mem_going_online_callback(void *arg)
{
	struct kmem_cache_node *n;
	struct kmem_cache *s;
	struct memory_notify *marg = arg;
	int nid = marg->status_change_nid_normal;
	int ret = 0;

	/*
	 * If the node's memory is already available, then kmem_cache_node is
	 * already created. Nothing to do.
	 */
	if (nid < 0)
		return 0;

	/*
	 * We are bringing a node online. No memory is available yet. We must
	 * allocate a kmem_cache_node structure in order to bring the node
	 * online.
	 */
	mutex_lock(&slab_mutex);
	list_for_each_entry(s, &slab_caches, list) {
		/*
<<<<<<< HEAD
=======
		 * The structure may already exist if the node was previously
		 * onlined and offlined.
		 */
		if (get_node(s, nid))
			continue;
		/*
>>>>>>> upstream/android-13
		 * XXX: kmem_cache_alloc_node will fallback to other nodes
		 *      since memory is not yet available from the node that
		 *      is brought up.
		 */
		n = kmem_cache_alloc(kmem_cache_node, GFP_KERNEL);
		if (!n) {
			ret = -ENOMEM;
			goto out;
		}
		init_kmem_cache_node(n);
		s->node[nid] = n;
	}
<<<<<<< HEAD
=======
	/*
	 * Any cache created after this point will also have kmem_cache_node
	 * initialized for the new node.
	 */
	node_set(nid, slab_nodes);
>>>>>>> upstream/android-13
out:
	mutex_unlock(&slab_mutex);
	return ret;
}

static int slab_memory_callback(struct notifier_block *self,
				unsigned long action, void *arg)
{
	int ret = 0;

	switch (action) {
	case MEM_GOING_ONLINE:
		ret = slab_mem_going_online_callback(arg);
		break;
	case MEM_GOING_OFFLINE:
		ret = slab_mem_going_offline_callback(arg);
		break;
	case MEM_OFFLINE:
	case MEM_CANCEL_ONLINE:
		slab_mem_offline_callback(arg);
		break;
	case MEM_ONLINE:
	case MEM_CANCEL_OFFLINE:
		break;
	}
	if (ret)
		ret = notifier_from_errno(ret);
	else
		ret = NOTIFY_OK;
	return ret;
}

static struct notifier_block slab_memory_callback_nb = {
	.notifier_call = slab_memory_callback,
	.priority = SLAB_CALLBACK_PRI,
};

/********************************************************************
 *			Basic setup of slabs
 *******************************************************************/

/*
 * Used for early kmem_cache structures that were allocated using
 * the page allocator. Allocate them properly then fix up the pointers
 * that may be pointing to the wrong kmem_cache structure.
 */

static struct kmem_cache * __init bootstrap(struct kmem_cache *static_cache)
{
	int node;
	struct kmem_cache *s = kmem_cache_zalloc(kmem_cache, GFP_NOWAIT);
	struct kmem_cache_node *n;

	memcpy(s, static_cache, kmem_cache->object_size);

	/*
	 * This runs very early, and only the boot processor is supposed to be
	 * up.  Even if it weren't true, IRQs are not up so we couldn't fire
	 * IPIs around.
	 */
	__flush_cpu_slab(s, smp_processor_id());
	for_each_kmem_cache_node(s, node, n) {
		struct page *p;

<<<<<<< HEAD
		list_for_each_entry(p, &n->partial, lru)
			p->slab_cache = s;

#ifdef CONFIG_SLUB_DEBUG
#ifndef CONFIG_KDP
		list_for_each_entry(p, &n->full, lru)
			p->slab_cache = s;
#endif
#endif
	}
	slab_init_memcg_params(s);
	list_add(&s->list, &slab_caches);
	memcg_link_cache(s);
=======
		list_for_each_entry(p, &n->partial, slab_list)
			p->slab_cache = s;

#ifdef CONFIG_SLUB_DEBUG
		list_for_each_entry(p, &n->full, slab_list)
			p->slab_cache = s;
#endif
	}
	list_add(&s->list, &slab_caches);
>>>>>>> upstream/android-13
	return s;
}

void __init kmem_cache_init(void)
{
	static __initdata struct kmem_cache boot_kmem_cache,
		boot_kmem_cache_node;
<<<<<<< HEAD
=======
	int node;
>>>>>>> upstream/android-13

	if (debug_guardpage_minorder())
		slub_max_order = 0;

<<<<<<< HEAD
	kmem_cache_node = &boot_kmem_cache_node;
	kmem_cache = &boot_kmem_cache;

=======
	/* Print slub debugging pointers without hashing */
	if (__slub_debug_enabled())
		no_hash_pointers_enable(NULL);

	kmem_cache_node = &boot_kmem_cache_node;
	kmem_cache = &boot_kmem_cache;

	/*
	 * Initialize the nodemask for which we will allocate per node
	 * structures. Here we don't need taking slab_mutex yet.
	 */
	for_each_node_state(node, N_NORMAL_MEMORY)
		node_set(node, slab_nodes);

>>>>>>> upstream/android-13
	create_boot_cache(kmem_cache_node, "kmem_cache_node",
		sizeof(struct kmem_cache_node), SLAB_HWCACHE_ALIGN, 0, 0);

	register_hotmemory_notifier(&slab_memory_callback_nb);

	/* Able to allocate the per node structures */
	slab_state = PARTIAL;

	create_boot_cache(kmem_cache, "kmem_cache",
			offsetof(struct kmem_cache, node) +
				nr_node_ids * sizeof(struct kmem_cache_node *),
		       SLAB_HWCACHE_ALIGN, 0, 0);

	kmem_cache = bootstrap(&boot_kmem_cache);
	kmem_cache_node = bootstrap(&boot_kmem_cache_node);

	/* Now we can use the kmem_cache to allocate kmalloc slabs */
	setup_kmalloc_cache_index_table();
	create_kmalloc_caches(0);

	/* Setup random freelists for each cache */
	init_freelist_randomization();

	cpuhp_setup_state_nocalls(CPUHP_SLUB_DEAD, "slub:dead", NULL,
				  slub_cpu_dead);

<<<<<<< HEAD
	pr_info("SLUB: HWalign=%d, Order=%u-%u, MinObjects=%u, CPUs=%u, Nodes=%d\n",
=======
	pr_info("SLUB: HWalign=%d, Order=%u-%u, MinObjects=%u, CPUs=%u, Nodes=%u\n",
>>>>>>> upstream/android-13
		cache_line_size(),
		slub_min_order, slub_max_order, slub_min_objects,
		nr_cpu_ids, nr_node_ids);
}

void __init kmem_cache_init_late(void)
{
}

struct kmem_cache *
__kmem_cache_alias(const char *name, unsigned int size, unsigned int align,
		   slab_flags_t flags, void (*ctor)(void *))
{
<<<<<<< HEAD
	struct kmem_cache *s, *c;
=======
	struct kmem_cache *s;
>>>>>>> upstream/android-13

	s = find_mergeable(size, align, flags, name, ctor);
	if (s) {
		s->refcount++;

		/*
		 * Adjust the object sizes so that we clear
		 * the complete object on kzalloc.
		 */
		s->object_size = max(s->object_size, size);
		s->inuse = max(s->inuse, ALIGN(size, sizeof(void *)));

<<<<<<< HEAD
		for_each_memcg_cache(c, s) {
			c->object_size = s->object_size;
			c->inuse = max(c->inuse, ALIGN(size, sizeof(void *)));
		}

=======
>>>>>>> upstream/android-13
		if (sysfs_slab_alias(s, name)) {
			s->refcount--;
			s = NULL;
		}
	}

	return s;
}

int __kmem_cache_create(struct kmem_cache *s, slab_flags_t flags)
{
	int err;

	err = kmem_cache_open(s, flags);
	if (err)
		return err;

	/* Mutex is not taken during early boot */
	if (slab_state <= UP)
		return 0;

<<<<<<< HEAD
	memcg_propagate_slab_attrs(s);
	err = sysfs_slab_add(s);
	if (err)
		__kmem_cache_release(s);

	return err;
=======
	err = sysfs_slab_add(s);
	if (err) {
		__kmem_cache_release(s);
		return err;
	}

	if (s->flags & SLAB_STORE_USER)
		debugfs_slab_add(s);

	return 0;
>>>>>>> upstream/android-13
}

void *__kmalloc_track_caller(size_t size, gfp_t gfpflags, unsigned long caller)
{
	struct kmem_cache *s;
	void *ret;

	if (unlikely(size > KMALLOC_MAX_CACHE_SIZE))
		return kmalloc_large(size, gfpflags);

	s = kmalloc_slab(size, gfpflags);

	if (unlikely(ZERO_OR_NULL_PTR(s)))
		return s;

<<<<<<< HEAD
	ret = slab_alloc(s, gfpflags, caller);
=======
	ret = slab_alloc(s, gfpflags, caller, size);
>>>>>>> upstream/android-13

	/* Honor the call site pointer we received. */
	trace_kmalloc(caller, ret, size, s->size, gfpflags);

	return ret;
}
EXPORT_SYMBOL(__kmalloc_track_caller);

#ifdef CONFIG_NUMA
void *__kmalloc_node_track_caller(size_t size, gfp_t gfpflags,
					int node, unsigned long caller)
{
	struct kmem_cache *s;
	void *ret;

	if (unlikely(size > KMALLOC_MAX_CACHE_SIZE)) {
		ret = kmalloc_large_node(size, gfpflags, node);

		trace_kmalloc_node(caller, ret,
				   size, PAGE_SIZE << get_order(size),
				   gfpflags, node);

		return ret;
	}

	s = kmalloc_slab(size, gfpflags);

	if (unlikely(ZERO_OR_NULL_PTR(s)))
		return s;

<<<<<<< HEAD
	ret = slab_alloc_node(s, gfpflags, node, caller);
=======
	ret = slab_alloc_node(s, gfpflags, node, caller, size);
>>>>>>> upstream/android-13

	/* Honor the call site pointer we received. */
	trace_kmalloc_node(caller, ret, size, s->size, gfpflags, node);

	return ret;
}
EXPORT_SYMBOL(__kmalloc_node_track_caller);
#endif

#ifdef CONFIG_SYSFS
static int count_inuse(struct page *page)
{
	return page->inuse;
}

static int count_total(struct page *page)
{
	return page->objects;
}
#endif

#ifdef CONFIG_SLUB_DEBUG
<<<<<<< HEAD
static int validate_slab(struct kmem_cache *s, struct page *page,
						unsigned long *map)
{
	void *p;
	void *addr = page_address(page);

	if (!check_slab(s, page) ||
			!on_freelist(s, page, NULL))
		return 0;

	/* Now we know that a valid freelist exists */
	bitmap_zero(map, page->objects);

	get_map(s, page, map);
	for_each_object(p, s, addr, page->objects) {
		if (test_bit(slab_index(p, s, addr), map))
			if (!check_object(s, page, p, SLUB_RED_INACTIVE))
				return 0;
	}

	for_each_object(p, s, addr, page->objects)
		if (!test_bit(slab_index(p, s, addr), map))
			if (!check_object(s, page, p, SLUB_RED_ACTIVE))
				return 0;
	return 1;
}

static void validate_slab_slab(struct kmem_cache *s, struct page *page,
						unsigned long *map)
{
	slab_lock(page);
	validate_slab(s, page, map);
	slab_unlock(page);
}

static int validate_slab_node(struct kmem_cache *s,
		struct kmem_cache_node *n, unsigned long *map)
=======
static void validate_slab(struct kmem_cache *s, struct page *page,
			  unsigned long *obj_map)
{
	void *p;
	void *addr = page_address(page);
	unsigned long flags;

	slab_lock(page, &flags);

	if (!check_slab(s, page) || !on_freelist(s, page, NULL))
		goto unlock;

	/* Now we know that a valid freelist exists */
	__fill_map(obj_map, s, page);
	for_each_object(p, s, addr, page->objects) {
		u8 val = test_bit(__obj_to_index(s, addr, p), obj_map) ?
			 SLUB_RED_INACTIVE : SLUB_RED_ACTIVE;

		if (!check_object(s, page, p, val))
			break;
	}
unlock:
	slab_unlock(page, &flags);
}

static int validate_slab_node(struct kmem_cache *s,
		struct kmem_cache_node *n, unsigned long *obj_map)
>>>>>>> upstream/android-13
{
	unsigned long count = 0;
	struct page *page;
	unsigned long flags;

	spin_lock_irqsave(&n->list_lock, flags);

<<<<<<< HEAD
	list_for_each_entry(page, &n->partial, lru) {
		validate_slab_slab(s, page, map);
		count++;
	}
	if (count != n->nr_partial)
		pr_err("SLUB %s: %ld partial slabs counted but counter=%ld\n",
		       s->name, count, n->nr_partial);
=======
	list_for_each_entry(page, &n->partial, slab_list) {
		validate_slab(s, page, obj_map);
		count++;
	}
	if (count != n->nr_partial) {
		pr_err("SLUB %s: %ld partial slabs counted but counter=%ld\n",
		       s->name, count, n->nr_partial);
		slab_add_kunit_errors();
	}
>>>>>>> upstream/android-13

	if (!(s->flags & SLAB_STORE_USER))
		goto out;

<<<<<<< HEAD
	list_for_each_entry(page, &n->full, lru) {
		validate_slab_slab(s, page, map);
		count++;
	}
	if (count != atomic_long_read(&n->nr_slabs))
		pr_err("SLUB: %s %ld slabs counted but counter=%ld\n",
		       s->name, count, atomic_long_read(&n->nr_slabs));
=======
	list_for_each_entry(page, &n->full, slab_list) {
		validate_slab(s, page, obj_map);
		count++;
	}
	if (count != atomic_long_read(&n->nr_slabs)) {
		pr_err("SLUB: %s %ld slabs counted but counter=%ld\n",
		       s->name, count, atomic_long_read(&n->nr_slabs));
		slab_add_kunit_errors();
	}
>>>>>>> upstream/android-13

out:
	spin_unlock_irqrestore(&n->list_lock, flags);
	return count;
}

<<<<<<< HEAD
static long validate_slab_cache(struct kmem_cache *s)
{
	int node;
	unsigned long count = 0;
	unsigned long *map = kmalloc_array(BITS_TO_LONGS(oo_objects(s->max)),
					   sizeof(unsigned long),
					   GFP_KERNEL);
	struct kmem_cache_node *n;

	if (!map)
=======
long validate_slab_cache(struct kmem_cache *s)
{
	int node;
	unsigned long count = 0;
	struct kmem_cache_node *n;
	unsigned long *obj_map;

	obj_map = bitmap_alloc(oo_objects(s->oo), GFP_KERNEL);
	if (!obj_map)
>>>>>>> upstream/android-13
		return -ENOMEM;

	flush_all(s);
	for_each_kmem_cache_node(s, node, n)
<<<<<<< HEAD
		count += validate_slab_node(s, n, map);
	kfree(map);
	return count;
}
=======
		count += validate_slab_node(s, n, obj_map);

	bitmap_free(obj_map);

	return count;
}
EXPORT_SYMBOL(validate_slab_cache);

#ifdef CONFIG_DEBUG_FS
>>>>>>> upstream/android-13
/*
 * Generate lists of code addresses where slabcache objects are allocated
 * and freed.
 */

struct location {
	unsigned long count;
	unsigned long addr;
	long long sum_time;
	long min_time;
	long max_time;
	long min_pid;
	long max_pid;
	DECLARE_BITMAP(cpus, NR_CPUS);
	nodemask_t nodes;
};

struct loc_track {
	unsigned long max;
	unsigned long count;
	struct location *loc;
<<<<<<< HEAD
};

=======
	loff_t idx;
};

static struct dentry *slab_debugfs_root;

>>>>>>> upstream/android-13
static void free_loc_track(struct loc_track *t)
{
	if (t->max)
		free_pages((unsigned long)t->loc,
			get_order(sizeof(struct location) * t->max));
}

static int alloc_loc_track(struct loc_track *t, unsigned long max, gfp_t flags)
{
	struct location *l;
	int order;

	order = get_order(sizeof(struct location) * max);

	l = (void *)__get_free_pages(flags, order);
	if (!l)
		return 0;

	if (t->count) {
		memcpy(l, t->loc, sizeof(struct location) * t->count);
		free_loc_track(t);
	}
	t->max = max;
	t->loc = l;
	return 1;
}

static int add_location(struct loc_track *t, struct kmem_cache *s,
				const struct track *track)
{
	long start, end, pos;
	struct location *l;
	unsigned long caddr;
	unsigned long age = jiffies - track->when;

	start = -1;
	end = t->count;

	for ( ; ; ) {
		pos = start + (end - start + 1) / 2;

		/*
		 * There is nothing at "end". If we end up there
		 * we need to add something to before end.
		 */
		if (pos == end)
			break;

		caddr = t->loc[pos].addr;
		if (track->addr == caddr) {

			l = &t->loc[pos];
			l->count++;
			if (track->when) {
				l->sum_time += age;
				if (age < l->min_time)
					l->min_time = age;
				if (age > l->max_time)
					l->max_time = age;

				if (track->pid < l->min_pid)
					l->min_pid = track->pid;
				if (track->pid > l->max_pid)
					l->max_pid = track->pid;

				cpumask_set_cpu(track->cpu,
						to_cpumask(l->cpus));
			}
			node_set(page_to_nid(virt_to_page(track)), l->nodes);
			return 1;
		}

		if (track->addr < caddr)
			end = pos;
		else
			start = pos;
	}

	/*
	 * Not found. Insert new tracking element.
	 */
	if (t->count >= t->max && !alloc_loc_track(t, 2 * t->max, GFP_ATOMIC))
		return 0;

	l = t->loc + pos;
	if (pos < t->count)
		memmove(l + 1, l,
			(t->count - pos) * sizeof(struct location));
	t->count++;
	l->count = 1;
	l->addr = track->addr;
	l->sum_time = age;
	l->min_time = age;
	l->max_time = age;
	l->min_pid = track->pid;
	l->max_pid = track->pid;
	cpumask_clear(to_cpumask(l->cpus));
	cpumask_set_cpu(track->cpu, to_cpumask(l->cpus));
	nodes_clear(l->nodes);
	node_set(page_to_nid(virt_to_page(track)), l->nodes);
	return 1;
}

static void process_slab(struct loc_track *t, struct kmem_cache *s,
		struct page *page, enum track_item alloc,
<<<<<<< HEAD
		unsigned long *map)
=======
		unsigned long *obj_map)
>>>>>>> upstream/android-13
{
	void *addr = page_address(page);
	void *p;

<<<<<<< HEAD
	bitmap_zero(map, page->objects);
	get_map(s, page, map);

	for_each_object(p, s, addr, page->objects)
		if (!test_bit(slab_index(p, s, addr), map))
			add_location(t, s, get_track(s, p, alloc));
}

static int list_locations(struct kmem_cache *s, char *buf,
					enum track_item alloc)
{
	int len = 0;
	unsigned long i;
	struct loc_track t = { 0, 0, NULL };
	int node;
	unsigned long *map = kmalloc_array(BITS_TO_LONGS(oo_objects(s->max)),
					   sizeof(unsigned long),
					   GFP_KERNEL);
	struct kmem_cache_node *n;

	if (!map || !alloc_loc_track(&t, PAGE_SIZE / sizeof(struct location),
				     GFP_KERNEL)) {
		kfree(map);
		return sprintf(buf, "Out of memory\n");
	}
	/* Push back cpu slabs */
	flush_all(s);

	for_each_kmem_cache_node(s, node, n) {
		unsigned long flags;
		struct page *page;

		if (!atomic_long_read(&n->nr_slabs))
			continue;

		spin_lock_irqsave(&n->list_lock, flags);
		list_for_each_entry(page, &n->partial, lru)
			process_slab(&t, s, page, alloc, map);
		list_for_each_entry(page, &n->full, lru)
			process_slab(&t, s, page, alloc, map);
		spin_unlock_irqrestore(&n->list_lock, flags);
	}

	for (i = 0; i < t.count; i++) {
		struct location *l = &t.loc[i];

		if (len > PAGE_SIZE - KSYM_SYMBOL_LEN - 100)
			break;
		len += sprintf(buf + len, "%7ld ", l->count);

		if (l->addr)
			len += sprintf(buf + len, "%pS", (void *)l->addr);
		else
			len += sprintf(buf + len, "<not-available>");

		if (l->sum_time != l->min_time) {
			len += sprintf(buf + len, " age=%ld/%ld/%ld",
				l->min_time,
				(long)div_u64(l->sum_time, l->count),
				l->max_time);
		} else
			len += sprintf(buf + len, " age=%ld",
				l->min_time);

		if (l->min_pid != l->max_pid)
			len += sprintf(buf + len, " pid=%ld-%ld",
				l->min_pid, l->max_pid);
		else
			len += sprintf(buf + len, " pid=%ld",
				l->min_pid);

		if (num_online_cpus() > 1 &&
				!cpumask_empty(to_cpumask(l->cpus)) &&
				len < PAGE_SIZE - 60)
			len += scnprintf(buf + len, PAGE_SIZE - len - 50,
					 " cpus=%*pbl",
					 cpumask_pr_args(to_cpumask(l->cpus)));

		if (nr_online_nodes > 1 && !nodes_empty(l->nodes) &&
				len < PAGE_SIZE - 60)
			len += scnprintf(buf + len, PAGE_SIZE - len - 50,
					 " nodes=%*pbl",
					 nodemask_pr_args(&l->nodes));

		len += sprintf(buf + len, "\n");
	}

	free_loc_track(&t);
	kfree(map);
	if (!t.count)
		len += sprintf(buf, "No data\n");
	return len;
}
#endif

#ifdef SLUB_RESILIENCY_TEST
static void __init resiliency_test(void)
{
	u8 *p;
	int type = KMALLOC_NORMAL;

	BUILD_BUG_ON(KMALLOC_MIN_SIZE > 16 || KMALLOC_SHIFT_HIGH < 10);

	pr_err("SLUB resiliency testing\n");
	pr_err("-----------------------\n");
	pr_err("A. Corruption after allocation\n");

	p = kzalloc(16, GFP_KERNEL);
	p[16] = 0x12;
	pr_err("\n1. kmalloc-16: Clobber Redzone/next pointer 0x12->0x%p\n\n",
	       p + 16);

	validate_slab_cache(kmalloc_caches[type][4]);

	/* Hmmm... The next two are dangerous */
	p = kzalloc(32, GFP_KERNEL);
	p[32 + sizeof(void *)] = 0x34;
	pr_err("\n2. kmalloc-32: Clobber next pointer/next slab 0x34 -> -0x%p\n",
	       p);
	pr_err("If allocated object is overwritten then not detectable\n\n");

	validate_slab_cache(kmalloc_caches[type][5]);
	p = kzalloc(64, GFP_KERNEL);
	p += 64 + (get_cycles() & 0xff) * sizeof(void *);
	*p = 0x56;
	pr_err("\n3. kmalloc-64: corrupting random byte 0x56->0x%p\n",
	       p);
	pr_err("If allocated object is overwritten then not detectable\n\n");
	validate_slab_cache(kmalloc_caches[type][6]);

	pr_err("\nB. Corruption after free\n");
	p = kzalloc(128, GFP_KERNEL);
	kfree(p);
	*p = 0x78;
	pr_err("1. kmalloc-128: Clobber first word 0x78->0x%p\n\n", p);
	validate_slab_cache(kmalloc_caches[type][7]);

	p = kzalloc(256, GFP_KERNEL);
	kfree(p);
	p[50] = 0x9a;
	pr_err("\n2. kmalloc-256: Clobber 50th byte 0x9a->0x%p\n\n", p);
	validate_slab_cache(kmalloc_caches[type][8]);

	p = kzalloc(512, GFP_KERNEL);
	kfree(p);
	p[512] = 0xab;
	pr_err("\n3. kmalloc-512: Clobber redzone 0xab->0x%p\n\n", p);
	validate_slab_cache(kmalloc_caches[type][9]);
}
#else
#ifdef CONFIG_SYSFS
static void resiliency_test(void) {};
#endif
#endif
=======
	__fill_map(obj_map, s, page);

	for_each_object(p, s, addr, page->objects)
		if (!test_bit(__obj_to_index(s, addr, p), obj_map))
			add_location(t, s, get_track(s, p, alloc));
}
#endif  /* CONFIG_DEBUG_FS   */
#endif	/* CONFIG_SLUB_DEBUG */
>>>>>>> upstream/android-13

#ifdef CONFIG_SYSFS
enum slab_stat_type {
	SL_ALL,			/* All slabs */
	SL_PARTIAL,		/* Only partially allocated slabs */
	SL_CPU,			/* Only slabs used for cpu caches */
	SL_OBJECTS,		/* Determine allocated objects not slabs */
	SL_TOTAL		/* Determine object capacity not slabs */
};

#define SO_ALL		(1 << SL_ALL)
#define SO_PARTIAL	(1 << SL_PARTIAL)
#define SO_CPU		(1 << SL_CPU)
#define SO_OBJECTS	(1 << SL_OBJECTS)
#define SO_TOTAL	(1 << SL_TOTAL)

<<<<<<< HEAD
#ifdef CONFIG_MEMCG
static bool memcg_sysfs_enabled = IS_ENABLED(CONFIG_SLUB_MEMCG_SYSFS_ON);

static int __init setup_slub_memcg_sysfs(char *str)
{
	int v;

	if (get_option(&str, &v) > 0)
		memcg_sysfs_enabled = v;

	return 1;
}

__setup("slub_memcg_sysfs=", setup_slub_memcg_sysfs);
#endif

static ssize_t show_slab_objects(struct kmem_cache *s,
			    char *buf, unsigned long flags)
=======
static ssize_t show_slab_objects(struct kmem_cache *s,
				 char *buf, unsigned long flags)
>>>>>>> upstream/android-13
{
	unsigned long total = 0;
	int node;
	int x;
	unsigned long *nodes;
<<<<<<< HEAD
=======
	int len = 0;
>>>>>>> upstream/android-13

	nodes = kcalloc(nr_node_ids, sizeof(unsigned long), GFP_KERNEL);
	if (!nodes)
		return -ENOMEM;

	if (flags & SO_CPU) {
		int cpu;

		for_each_possible_cpu(cpu) {
			struct kmem_cache_cpu *c = per_cpu_ptr(s->cpu_slab,
							       cpu);
			int node;
			struct page *page;

			page = READ_ONCE(c->page);
			if (!page)
				continue;

			node = page_to_nid(page);
			if (flags & SO_TOTAL)
				x = page->objects;
			else if (flags & SO_OBJECTS)
				x = page->inuse;
			else
				x = 1;

			total += x;
			nodes[node] += x;

			page = slub_percpu_partial_read_once(c);
			if (page) {
				node = page_to_nid(page);
				if (flags & SO_TOTAL)
					WARN_ON_ONCE(1);
				else if (flags & SO_OBJECTS)
					WARN_ON_ONCE(1);
				else
					x = page->pages;
				total += x;
				nodes[node] += x;
			}
		}
	}

	/*
	 * It is impossible to take "mem_hotplug_lock" here with "kernfs_mutex"
	 * already held which will conflict with an existing lock order:
	 *
	 * mem_hotplug_lock->slab_mutex->kernfs_mutex
	 *
	 * We don't really need mem_hotplug_lock (to hold off
	 * slab_mem_going_offline_callback) here because slab's memory hot
	 * unplug code doesn't destroy the kmem_cache->node[] data.
	 */

#ifdef CONFIG_SLUB_DEBUG
	if (flags & SO_ALL) {
		struct kmem_cache_node *n;

		for_each_kmem_cache_node(s, node, n) {

			if (flags & SO_TOTAL)
				x = atomic_long_read(&n->total_objects);
			else if (flags & SO_OBJECTS)
				x = atomic_long_read(&n->total_objects) -
					count_partial(n, count_free);
			else
				x = atomic_long_read(&n->nr_slabs);
			total += x;
			nodes[node] += x;
		}

	} else
#endif
	if (flags & SO_PARTIAL) {
		struct kmem_cache_node *n;

		for_each_kmem_cache_node(s, node, n) {
			if (flags & SO_TOTAL)
				x = count_partial(n, count_total);
			else if (flags & SO_OBJECTS)
				x = count_partial(n, count_inuse);
			else
				x = n->nr_partial;
			total += x;
			nodes[node] += x;
		}
	}
<<<<<<< HEAD
	x = sprintf(buf, "%lu", total);
#ifdef CONFIG_NUMA
	for (node = 0; node < nr_node_ids; node++)
		if (nodes[node])
			x += sprintf(buf + x, " N%d=%lu",
					node, nodes[node]);
#endif
	kfree(nodes);
	return x + sprintf(buf + x, "\n");
}

#ifdef CONFIG_SLUB_DEBUG
static int any_slab_objects(struct kmem_cache *s)
{
	int node;
	struct kmem_cache_node *n;

	for_each_kmem_cache_node(s, node, n)
		if (atomic_long_read(&n->total_objects))
			return 1;

	return 0;
}
#endif

=======

	len += sysfs_emit_at(buf, len, "%lu", total);
#ifdef CONFIG_NUMA
	for (node = 0; node < nr_node_ids; node++) {
		if (nodes[node])
			len += sysfs_emit_at(buf, len, " N%d=%lu",
					     node, nodes[node]);
	}
#endif
	len += sysfs_emit_at(buf, len, "\n");
	kfree(nodes);

	return len;
}

>>>>>>> upstream/android-13
#define to_slab_attr(n) container_of(n, struct slab_attribute, attr)
#define to_slab(n) container_of(n, struct kmem_cache, kobj)

struct slab_attribute {
	struct attribute attr;
	ssize_t (*show)(struct kmem_cache *s, char *buf);
	ssize_t (*store)(struct kmem_cache *s, const char *x, size_t count);
};

#define SLAB_ATTR_RO(_name) \
	static struct slab_attribute _name##_attr = \
	__ATTR(_name, 0400, _name##_show, NULL)

#define SLAB_ATTR(_name) \
	static struct slab_attribute _name##_attr =  \
	__ATTR(_name, 0600, _name##_show, _name##_store)

static ssize_t slab_size_show(struct kmem_cache *s, char *buf)
{
<<<<<<< HEAD
	return sprintf(buf, "%u\n", s->size);
=======
	return sysfs_emit(buf, "%u\n", s->size);
>>>>>>> upstream/android-13
}
SLAB_ATTR_RO(slab_size);

static ssize_t align_show(struct kmem_cache *s, char *buf)
{
<<<<<<< HEAD
	return sprintf(buf, "%u\n", s->align);
=======
	return sysfs_emit(buf, "%u\n", s->align);
>>>>>>> upstream/android-13
}
SLAB_ATTR_RO(align);

static ssize_t object_size_show(struct kmem_cache *s, char *buf)
{
<<<<<<< HEAD
	return sprintf(buf, "%u\n", s->object_size);
=======
	return sysfs_emit(buf, "%u\n", s->object_size);
>>>>>>> upstream/android-13
}
SLAB_ATTR_RO(object_size);

static ssize_t objs_per_slab_show(struct kmem_cache *s, char *buf)
{
<<<<<<< HEAD
	return sprintf(buf, "%u\n", oo_objects(s->oo));
}
SLAB_ATTR_RO(objs_per_slab);

static ssize_t order_store(struct kmem_cache *s,
				const char *buf, size_t length)
{
	unsigned int order;
	int err;

	err = kstrtouint(buf, 10, &order);
	if (err)
		return err;

	if (order > slub_max_order || order < slub_min_order)
		return -EINVAL;

	calculate_sizes(s, order);
	return length;
}

static ssize_t order_show(struct kmem_cache *s, char *buf)
{
	return sprintf(buf, "%u\n", oo_order(s->oo));
}
SLAB_ATTR(order);

static ssize_t min_partial_show(struct kmem_cache *s, char *buf)
{
	return sprintf(buf, "%lu\n", s->min_partial);
=======
	return sysfs_emit(buf, "%u\n", oo_objects(s->oo));
}
SLAB_ATTR_RO(objs_per_slab);

static ssize_t order_show(struct kmem_cache *s, char *buf)
{
	return sysfs_emit(buf, "%u\n", oo_order(s->oo));
}
SLAB_ATTR_RO(order);

static ssize_t min_partial_show(struct kmem_cache *s, char *buf)
{
	return sysfs_emit(buf, "%lu\n", s->min_partial);
>>>>>>> upstream/android-13
}

static ssize_t min_partial_store(struct kmem_cache *s, const char *buf,
				 size_t length)
{
	unsigned long min;
	int err;

	err = kstrtoul(buf, 10, &min);
	if (err)
		return err;

	set_min_partial(s, min);
	return length;
}
SLAB_ATTR(min_partial);

static ssize_t cpu_partial_show(struct kmem_cache *s, char *buf)
{
<<<<<<< HEAD
	return sprintf(buf, "%u\n", slub_cpu_partial(s));
=======
	return sysfs_emit(buf, "%u\n", slub_cpu_partial(s));
>>>>>>> upstream/android-13
}

static ssize_t cpu_partial_store(struct kmem_cache *s, const char *buf,
				 size_t length)
{
	unsigned int objects;
	int err;

	err = kstrtouint(buf, 10, &objects);
	if (err)
		return err;
	if (objects && !kmem_cache_has_cpu_partial(s))
		return -EINVAL;

	slub_set_cpu_partial(s, objects);
	flush_all(s);
	return length;
}
SLAB_ATTR(cpu_partial);

static ssize_t ctor_show(struct kmem_cache *s, char *buf)
{
	if (!s->ctor)
		return 0;
<<<<<<< HEAD
	return sprintf(buf, "%pS\n", s->ctor);
=======
	return sysfs_emit(buf, "%pS\n", s->ctor);
>>>>>>> upstream/android-13
}
SLAB_ATTR_RO(ctor);

static ssize_t aliases_show(struct kmem_cache *s, char *buf)
{
<<<<<<< HEAD
	return sprintf(buf, "%d\n", s->refcount < 0 ? 0 : s->refcount - 1);
=======
	return sysfs_emit(buf, "%d\n", s->refcount < 0 ? 0 : s->refcount - 1);
>>>>>>> upstream/android-13
}
SLAB_ATTR_RO(aliases);

static ssize_t partial_show(struct kmem_cache *s, char *buf)
{
	return show_slab_objects(s, buf, SO_PARTIAL);
}
SLAB_ATTR_RO(partial);

static ssize_t cpu_slabs_show(struct kmem_cache *s, char *buf)
{
	return show_slab_objects(s, buf, SO_CPU);
}
SLAB_ATTR_RO(cpu_slabs);

static ssize_t objects_show(struct kmem_cache *s, char *buf)
{
	return show_slab_objects(s, buf, SO_ALL|SO_OBJECTS);
}
SLAB_ATTR_RO(objects);

static ssize_t objects_partial_show(struct kmem_cache *s, char *buf)
{
	return show_slab_objects(s, buf, SO_PARTIAL|SO_OBJECTS);
}
SLAB_ATTR_RO(objects_partial);

static ssize_t slabs_cpu_partial_show(struct kmem_cache *s, char *buf)
{
	int objects = 0;
	int pages = 0;
	int cpu;
<<<<<<< HEAD
	int len;
=======
	int len = 0;
>>>>>>> upstream/android-13

	for_each_online_cpu(cpu) {
		struct page *page;

		page = slub_percpu_partial(per_cpu_ptr(s->cpu_slab, cpu));

		if (page) {
			pages += page->pages;
			objects += page->pobjects;
		}
	}

<<<<<<< HEAD
	len = sprintf(buf, "%d(%d)", objects, pages);
=======
	len += sysfs_emit_at(buf, len, "%d(%d)", objects, pages);
>>>>>>> upstream/android-13

#ifdef CONFIG_SMP
	for_each_online_cpu(cpu) {
		struct page *page;

		page = slub_percpu_partial(per_cpu_ptr(s->cpu_slab, cpu));
<<<<<<< HEAD

		if (page && len < PAGE_SIZE - 20)
			len += sprintf(buf + len, " C%d=%d(%d)", cpu,
				page->pobjects, page->pages);
	}
#endif
	return len + sprintf(buf + len, "\n");
=======
		if (page)
			len += sysfs_emit_at(buf, len, " C%d=%d(%d)",
					     cpu, page->pobjects, page->pages);
	}
#endif
	len += sysfs_emit_at(buf, len, "\n");

	return len;
>>>>>>> upstream/android-13
}
SLAB_ATTR_RO(slabs_cpu_partial);

static ssize_t reclaim_account_show(struct kmem_cache *s, char *buf)
{
<<<<<<< HEAD
	return sprintf(buf, "%d\n", !!(s->flags & SLAB_RECLAIM_ACCOUNT));
}

static ssize_t reclaim_account_store(struct kmem_cache *s,
				const char *buf, size_t length)
{
	s->flags &= ~SLAB_RECLAIM_ACCOUNT;
	if (buf[0] == '1')
		s->flags |= SLAB_RECLAIM_ACCOUNT;
	return length;
}
SLAB_ATTR(reclaim_account);

static ssize_t hwcache_align_show(struct kmem_cache *s, char *buf)
{
	return sprintf(buf, "%d\n", !!(s->flags & SLAB_HWCACHE_ALIGN));
=======
	return sysfs_emit(buf, "%d\n", !!(s->flags & SLAB_RECLAIM_ACCOUNT));
}
SLAB_ATTR_RO(reclaim_account);

static ssize_t hwcache_align_show(struct kmem_cache *s, char *buf)
{
	return sysfs_emit(buf, "%d\n", !!(s->flags & SLAB_HWCACHE_ALIGN));
>>>>>>> upstream/android-13
}
SLAB_ATTR_RO(hwcache_align);

#ifdef CONFIG_ZONE_DMA
static ssize_t cache_dma_show(struct kmem_cache *s, char *buf)
{
<<<<<<< HEAD
	return sprintf(buf, "%d\n", !!(s->flags & SLAB_CACHE_DMA));
=======
	return sysfs_emit(buf, "%d\n", !!(s->flags & SLAB_CACHE_DMA));
>>>>>>> upstream/android-13
}
SLAB_ATTR_RO(cache_dma);
#endif

<<<<<<< HEAD
#ifdef CONFIG_ZONE_DMA32
static ssize_t cache_dma32_show(struct kmem_cache *s, char *buf)
{
	return sprintf(buf, "%d\n", !!(s->flags & SLAB_CACHE_DMA32));
}
SLAB_ATTR_RO(cache_dma32);
#endif

static ssize_t usersize_show(struct kmem_cache *s, char *buf)
{
	return sprintf(buf, "%u\n", s->usersize);
=======
static ssize_t usersize_show(struct kmem_cache *s, char *buf)
{
	return sysfs_emit(buf, "%u\n", s->usersize);
>>>>>>> upstream/android-13
}
SLAB_ATTR_RO(usersize);

static ssize_t destroy_by_rcu_show(struct kmem_cache *s, char *buf)
{
<<<<<<< HEAD
	return sprintf(buf, "%d\n", !!(s->flags & SLAB_TYPESAFE_BY_RCU));
=======
	return sysfs_emit(buf, "%d\n", !!(s->flags & SLAB_TYPESAFE_BY_RCU));
>>>>>>> upstream/android-13
}
SLAB_ATTR_RO(destroy_by_rcu);

#ifdef CONFIG_SLUB_DEBUG
static ssize_t slabs_show(struct kmem_cache *s, char *buf)
{
	return show_slab_objects(s, buf, SO_ALL);
}
SLAB_ATTR_RO(slabs);

static ssize_t total_objects_show(struct kmem_cache *s, char *buf)
{
	return show_slab_objects(s, buf, SO_ALL|SO_TOTAL);
}
SLAB_ATTR_RO(total_objects);

static ssize_t sanity_checks_show(struct kmem_cache *s, char *buf)
{
<<<<<<< HEAD
	return sprintf(buf, "%d\n", !!(s->flags & SLAB_CONSISTENCY_CHECKS));
}

static ssize_t sanity_checks_store(struct kmem_cache *s,
				const char *buf, size_t length)
{
	s->flags &= ~SLAB_CONSISTENCY_CHECKS;
	if (buf[0] == '1') {
		s->flags &= ~__CMPXCHG_DOUBLE;
		s->flags |= SLAB_CONSISTENCY_CHECKS;
	}
	return length;
}
SLAB_ATTR(sanity_checks);

static ssize_t trace_show(struct kmem_cache *s, char *buf)
{
	return sprintf(buf, "%d\n", !!(s->flags & SLAB_TRACE));
}

static ssize_t trace_store(struct kmem_cache *s, const char *buf,
							size_t length)
{
	/*
	 * Tracing a merged cache is going to give confusing results
	 * as well as cause other issues like converting a mergeable
	 * cache into an umergeable one.
	 */
	if (s->refcount > 1)
		return -EINVAL;

	s->flags &= ~SLAB_TRACE;
	if (buf[0] == '1') {
		s->flags &= ~__CMPXCHG_DOUBLE;
		s->flags |= SLAB_TRACE;
	}
	return length;
}
SLAB_ATTR(trace);

static ssize_t red_zone_show(struct kmem_cache *s, char *buf)
{
	return sprintf(buf, "%d\n", !!(s->flags & SLAB_RED_ZONE));
}

static ssize_t red_zone_store(struct kmem_cache *s,
				const char *buf, size_t length)
{
	if (any_slab_objects(s))
		return -EBUSY;

	s->flags &= ~SLAB_RED_ZONE;
	if (buf[0] == '1') {
		s->flags |= SLAB_RED_ZONE;
	}
	calculate_sizes(s, -1);
	return length;
}
SLAB_ATTR(red_zone);

static ssize_t poison_show(struct kmem_cache *s, char *buf)
{
	return sprintf(buf, "%d\n", !!(s->flags & SLAB_POISON));
}

static ssize_t poison_store(struct kmem_cache *s,
				const char *buf, size_t length)
{
	if (any_slab_objects(s))
		return -EBUSY;

	s->flags &= ~SLAB_POISON;
	if (buf[0] == '1') {
		s->flags |= SLAB_POISON;
	}
	calculate_sizes(s, -1);
	return length;
}
SLAB_ATTR(poison);

static ssize_t store_user_show(struct kmem_cache *s, char *buf)
{
	return sprintf(buf, "%d\n", !!(s->flags & SLAB_STORE_USER));
}

static ssize_t store_user_store(struct kmem_cache *s,
				const char *buf, size_t length)
{
	if (any_slab_objects(s))
		return -EBUSY;

	s->flags &= ~SLAB_STORE_USER;
	if (buf[0] == '1') {
		s->flags &= ~__CMPXCHG_DOUBLE;
		s->flags |= SLAB_STORE_USER;
	}
	calculate_sizes(s, -1);
	return length;
}
SLAB_ATTR(store_user);
=======
	return sysfs_emit(buf, "%d\n", !!(s->flags & SLAB_CONSISTENCY_CHECKS));
}
SLAB_ATTR_RO(sanity_checks);

static ssize_t trace_show(struct kmem_cache *s, char *buf)
{
	return sysfs_emit(buf, "%d\n", !!(s->flags & SLAB_TRACE));
}
SLAB_ATTR_RO(trace);

static ssize_t red_zone_show(struct kmem_cache *s, char *buf)
{
	return sysfs_emit(buf, "%d\n", !!(s->flags & SLAB_RED_ZONE));
}

SLAB_ATTR_RO(red_zone);

static ssize_t poison_show(struct kmem_cache *s, char *buf)
{
	return sysfs_emit(buf, "%d\n", !!(s->flags & SLAB_POISON));
}

SLAB_ATTR_RO(poison);

static ssize_t store_user_show(struct kmem_cache *s, char *buf)
{
	return sysfs_emit(buf, "%d\n", !!(s->flags & SLAB_STORE_USER));
}

SLAB_ATTR_RO(store_user);
>>>>>>> upstream/android-13

static ssize_t validate_show(struct kmem_cache *s, char *buf)
{
	return 0;
}

static ssize_t validate_store(struct kmem_cache *s,
			const char *buf, size_t length)
{
	int ret = -EINVAL;

	if (buf[0] == '1') {
		ret = validate_slab_cache(s);
		if (ret >= 0)
			ret = length;
	}
	return ret;
}
SLAB_ATTR(validate);

<<<<<<< HEAD
static ssize_t alloc_calls_show(struct kmem_cache *s, char *buf)
{
	if (!(s->flags & SLAB_STORE_USER))
		return -ENOSYS;
	return list_locations(s, buf, TRACK_ALLOC);
}
SLAB_ATTR_RO(alloc_calls);

static ssize_t free_calls_show(struct kmem_cache *s, char *buf)
{
	if (!(s->flags & SLAB_STORE_USER))
		return -ENOSYS;
	return list_locations(s, buf, TRACK_FREE);
}
SLAB_ATTR_RO(free_calls);
=======
>>>>>>> upstream/android-13
#endif /* CONFIG_SLUB_DEBUG */

#ifdef CONFIG_FAILSLAB
static ssize_t failslab_show(struct kmem_cache *s, char *buf)
{
<<<<<<< HEAD
	return sprintf(buf, "%d\n", !!(s->flags & SLAB_FAILSLAB));
}

static ssize_t failslab_store(struct kmem_cache *s, const char *buf,
							size_t length)
{
	if (s->refcount > 1)
		return -EINVAL;

	s->flags &= ~SLAB_FAILSLAB;
	if (buf[0] == '1')
		s->flags |= SLAB_FAILSLAB;
	return length;
}
SLAB_ATTR(failslab);
=======
	return sysfs_emit(buf, "%d\n", !!(s->flags & SLAB_FAILSLAB));
}
SLAB_ATTR_RO(failslab);
>>>>>>> upstream/android-13
#endif

static ssize_t shrink_show(struct kmem_cache *s, char *buf)
{
	return 0;
}

static ssize_t shrink_store(struct kmem_cache *s,
			const char *buf, size_t length)
{
	if (buf[0] == '1')
		kmem_cache_shrink(s);
	else
		return -EINVAL;
	return length;
}
SLAB_ATTR(shrink);

#ifdef CONFIG_NUMA
static ssize_t remote_node_defrag_ratio_show(struct kmem_cache *s, char *buf)
{
<<<<<<< HEAD
	return sprintf(buf, "%u\n", s->remote_node_defrag_ratio / 10);
=======
	return sysfs_emit(buf, "%u\n", s->remote_node_defrag_ratio / 10);
>>>>>>> upstream/android-13
}

static ssize_t remote_node_defrag_ratio_store(struct kmem_cache *s,
				const char *buf, size_t length)
{
	unsigned int ratio;
	int err;

	err = kstrtouint(buf, 10, &ratio);
	if (err)
		return err;
	if (ratio > 100)
		return -ERANGE;

	s->remote_node_defrag_ratio = ratio * 10;

	return length;
}
SLAB_ATTR(remote_node_defrag_ratio);
#endif

#ifdef CONFIG_SLUB_STATS
static int show_stat(struct kmem_cache *s, char *buf, enum stat_item si)
{
	unsigned long sum  = 0;
	int cpu;
<<<<<<< HEAD
	int len;
=======
	int len = 0;
>>>>>>> upstream/android-13
	int *data = kmalloc_array(nr_cpu_ids, sizeof(int), GFP_KERNEL);

	if (!data)
		return -ENOMEM;

	for_each_online_cpu(cpu) {
		unsigned x = per_cpu_ptr(s->cpu_slab, cpu)->stat[si];

		data[cpu] = x;
		sum += x;
	}

<<<<<<< HEAD
	len = sprintf(buf, "%lu", sum);

#ifdef CONFIG_SMP
	for_each_online_cpu(cpu) {
		if (data[cpu] && len < PAGE_SIZE - 20)
			len += sprintf(buf + len, " C%d=%u", cpu, data[cpu]);
	}
#endif
	kfree(data);
	return len + sprintf(buf + len, "\n");
=======
	len += sysfs_emit_at(buf, len, "%lu", sum);

#ifdef CONFIG_SMP
	for_each_online_cpu(cpu) {
		if (data[cpu])
			len += sysfs_emit_at(buf, len, " C%d=%u",
					     cpu, data[cpu]);
	}
#endif
	kfree(data);
	len += sysfs_emit_at(buf, len, "\n");

	return len;
>>>>>>> upstream/android-13
}

static void clear_stat(struct kmem_cache *s, enum stat_item si)
{
	int cpu;

	for_each_online_cpu(cpu)
		per_cpu_ptr(s->cpu_slab, cpu)->stat[si] = 0;
}

#define STAT_ATTR(si, text) 					\
static ssize_t text##_show(struct kmem_cache *s, char *buf)	\
{								\
	return show_stat(s, buf, si);				\
}								\
static ssize_t text##_store(struct kmem_cache *s,		\
				const char *buf, size_t length)	\
{								\
	if (buf[0] != '0')					\
		return -EINVAL;					\
	clear_stat(s, si);					\
	return length;						\
}								\
SLAB_ATTR(text);						\

STAT_ATTR(ALLOC_FASTPATH, alloc_fastpath);
STAT_ATTR(ALLOC_SLOWPATH, alloc_slowpath);
STAT_ATTR(FREE_FASTPATH, free_fastpath);
STAT_ATTR(FREE_SLOWPATH, free_slowpath);
STAT_ATTR(FREE_FROZEN, free_frozen);
STAT_ATTR(FREE_ADD_PARTIAL, free_add_partial);
STAT_ATTR(FREE_REMOVE_PARTIAL, free_remove_partial);
STAT_ATTR(ALLOC_FROM_PARTIAL, alloc_from_partial);
STAT_ATTR(ALLOC_SLAB, alloc_slab);
STAT_ATTR(ALLOC_REFILL, alloc_refill);
STAT_ATTR(ALLOC_NODE_MISMATCH, alloc_node_mismatch);
STAT_ATTR(FREE_SLAB, free_slab);
STAT_ATTR(CPUSLAB_FLUSH, cpuslab_flush);
STAT_ATTR(DEACTIVATE_FULL, deactivate_full);
STAT_ATTR(DEACTIVATE_EMPTY, deactivate_empty);
STAT_ATTR(DEACTIVATE_TO_HEAD, deactivate_to_head);
STAT_ATTR(DEACTIVATE_TO_TAIL, deactivate_to_tail);
STAT_ATTR(DEACTIVATE_REMOTE_FREES, deactivate_remote_frees);
STAT_ATTR(DEACTIVATE_BYPASS, deactivate_bypass);
STAT_ATTR(ORDER_FALLBACK, order_fallback);
STAT_ATTR(CMPXCHG_DOUBLE_CPU_FAIL, cmpxchg_double_cpu_fail);
STAT_ATTR(CMPXCHG_DOUBLE_FAIL, cmpxchg_double_fail);
STAT_ATTR(CPU_PARTIAL_ALLOC, cpu_partial_alloc);
STAT_ATTR(CPU_PARTIAL_FREE, cpu_partial_free);
STAT_ATTR(CPU_PARTIAL_NODE, cpu_partial_node);
STAT_ATTR(CPU_PARTIAL_DRAIN, cpu_partial_drain);
<<<<<<< HEAD
#endif
=======
#endif	/* CONFIG_SLUB_STATS */
>>>>>>> upstream/android-13

static struct attribute *slab_attrs[] = {
	&slab_size_attr.attr,
	&object_size_attr.attr,
	&objs_per_slab_attr.attr,
	&order_attr.attr,
	&min_partial_attr.attr,
	&cpu_partial_attr.attr,
	&objects_attr.attr,
	&objects_partial_attr.attr,
	&partial_attr.attr,
	&cpu_slabs_attr.attr,
	&ctor_attr.attr,
	&aliases_attr.attr,
	&align_attr.attr,
	&hwcache_align_attr.attr,
	&reclaim_account_attr.attr,
	&destroy_by_rcu_attr.attr,
	&shrink_attr.attr,
	&slabs_cpu_partial_attr.attr,
#ifdef CONFIG_SLUB_DEBUG
	&total_objects_attr.attr,
	&slabs_attr.attr,
	&sanity_checks_attr.attr,
	&trace_attr.attr,
	&red_zone_attr.attr,
	&poison_attr.attr,
	&store_user_attr.attr,
	&validate_attr.attr,
<<<<<<< HEAD
	&alloc_calls_attr.attr,
	&free_calls_attr.attr,
=======
>>>>>>> upstream/android-13
#endif
#ifdef CONFIG_ZONE_DMA
	&cache_dma_attr.attr,
#endif
<<<<<<< HEAD
#ifdef CONFIG_ZONE_DMA32
	&cache_dma32_attr.attr,
#endif
=======
>>>>>>> upstream/android-13
#ifdef CONFIG_NUMA
	&remote_node_defrag_ratio_attr.attr,
#endif
#ifdef CONFIG_SLUB_STATS
	&alloc_fastpath_attr.attr,
	&alloc_slowpath_attr.attr,
	&free_fastpath_attr.attr,
	&free_slowpath_attr.attr,
	&free_frozen_attr.attr,
	&free_add_partial_attr.attr,
	&free_remove_partial_attr.attr,
	&alloc_from_partial_attr.attr,
	&alloc_slab_attr.attr,
	&alloc_refill_attr.attr,
	&alloc_node_mismatch_attr.attr,
	&free_slab_attr.attr,
	&cpuslab_flush_attr.attr,
	&deactivate_full_attr.attr,
	&deactivate_empty_attr.attr,
	&deactivate_to_head_attr.attr,
	&deactivate_to_tail_attr.attr,
	&deactivate_remote_frees_attr.attr,
	&deactivate_bypass_attr.attr,
	&order_fallback_attr.attr,
	&cmpxchg_double_fail_attr.attr,
	&cmpxchg_double_cpu_fail_attr.attr,
	&cpu_partial_alloc_attr.attr,
	&cpu_partial_free_attr.attr,
	&cpu_partial_node_attr.attr,
	&cpu_partial_drain_attr.attr,
#endif
#ifdef CONFIG_FAILSLAB
	&failslab_attr.attr,
#endif
	&usersize_attr.attr,

	NULL
};

static const struct attribute_group slab_attr_group = {
	.attrs = slab_attrs,
};

static ssize_t slab_attr_show(struct kobject *kobj,
				struct attribute *attr,
				char *buf)
{
	struct slab_attribute *attribute;
	struct kmem_cache *s;
	int err;

	attribute = to_slab_attr(attr);
	s = to_slab(kobj);

	if (!attribute->show)
		return -EIO;

	err = attribute->show(s, buf);

	return err;
}

static ssize_t slab_attr_store(struct kobject *kobj,
				struct attribute *attr,
				const char *buf, size_t len)
{
	struct slab_attribute *attribute;
	struct kmem_cache *s;
	int err;

	attribute = to_slab_attr(attr);
	s = to_slab(kobj);

	if (!attribute->store)
		return -EIO;

	err = attribute->store(s, buf, len);
<<<<<<< HEAD
#ifdef CONFIG_MEMCG
	if (slab_state >= FULL && err >= 0 && is_root_cache(s)) {
		struct kmem_cache *c;

		mutex_lock(&slab_mutex);
		if (s->max_attr_size < len)
			s->max_attr_size = len;

		/*
		 * This is a best effort propagation, so this function's return
		 * value will be determined by the parent cache only. This is
		 * basically because not all attributes will have a well
		 * defined semantics for rollbacks - most of the actions will
		 * have permanent effects.
		 *
		 * Returning the error value of any of the children that fail
		 * is not 100 % defined, in the sense that users seeing the
		 * error code won't be able to know anything about the state of
		 * the cache.
		 *
		 * Only returning the error code for the parent cache at least
		 * has well defined semantics. The cache being written to
		 * directly either failed or succeeded, in which case we loop
		 * through the descendants with best-effort propagation.
		 */
		for_each_memcg_cache(c, s)
			attribute->store(c, buf, len);
		mutex_unlock(&slab_mutex);
	}
#endif
	return err;
}

static void memcg_propagate_slab_attrs(struct kmem_cache *s)
{
#ifdef CONFIG_MEMCG
	int i;
	char *buffer = NULL;
	struct kmem_cache *root_cache;

	if (is_root_cache(s))
		return;

	root_cache = s->memcg_params.root_cache;

	/*
	 * This mean this cache had no attribute written. Therefore, no point
	 * in copying default values around
	 */
	if (!root_cache->max_attr_size)
		return;

	for (i = 0; i < ARRAY_SIZE(slab_attrs); i++) {
		char mbuf[64];
		char *buf;
		struct slab_attribute *attr = to_slab_attr(slab_attrs[i]);
		ssize_t len;

		if (!attr || !attr->store || !attr->show)
			continue;

		/*
		 * It is really bad that we have to allocate here, so we will
		 * do it only as a fallback. If we actually allocate, though,
		 * we can just use the allocated buffer until the end.
		 *
		 * Most of the slub attributes will tend to be very small in
		 * size, but sysfs allows buffers up to a page, so they can
		 * theoretically happen.
		 */
		if (buffer)
			buf = buffer;
		else if (root_cache->max_attr_size < ARRAY_SIZE(mbuf) &&
			 !IS_ENABLED(CONFIG_SLUB_STATS))
			buf = mbuf;
		else {
			buffer = (char *) get_zeroed_page(GFP_KERNEL);
			if (WARN_ON(!buffer))
				continue;
			buf = buffer;
		}

		len = attr->show(root_cache, buf);
		if (len > 0)
			attr->store(s, buf, len);
	}

	if (buffer)
		free_page((unsigned long)buffer);
#endif
}

=======
	return err;
}

>>>>>>> upstream/android-13
static void kmem_cache_release(struct kobject *k)
{
	slab_kmem_cache_release(to_slab(k));
}

static const struct sysfs_ops slab_sysfs_ops = {
	.show = slab_attr_show,
	.store = slab_attr_store,
};

static struct kobj_type slab_ktype = {
	.sysfs_ops = &slab_sysfs_ops,
	.release = kmem_cache_release,
};

<<<<<<< HEAD
static int uevent_filter(struct kset *kset, struct kobject *kobj)
{
	struct kobj_type *ktype = get_ktype(kobj);

	if (ktype == &slab_ktype)
		return 1;
	return 0;
}

static const struct kset_uevent_ops slab_uevent_ops = {
	.filter = uevent_filter,
};

=======
>>>>>>> upstream/android-13
static struct kset *slab_kset;

static inline struct kset *cache_kset(struct kmem_cache *s)
{
<<<<<<< HEAD
#ifdef CONFIG_MEMCG
	if (!is_root_cache(s))
		return s->memcg_params.root_cache->memcg_kset;
#endif
=======
>>>>>>> upstream/android-13
	return slab_kset;
}

#define ID_STR_LENGTH 64

/* Create a unique string id for a slab cache:
 *
 * Format	:[flags-]size
 */
static char *create_unique_id(struct kmem_cache *s)
{
	char *name = kmalloc(ID_STR_LENGTH, GFP_KERNEL);
	char *p = name;

	BUG_ON(!name);

	*p++ = ':';
	/*
	 * First flags affecting slabcache operations. We will only
	 * get here for aliasable slabs so we do not need to support
	 * too many flags. The flags here must cover all flags that
	 * are matched during merging to guarantee that the id is
	 * unique.
	 */
	if (s->flags & SLAB_CACHE_DMA)
		*p++ = 'd';
	if (s->flags & SLAB_CACHE_DMA32)
		*p++ = 'D';
	if (s->flags & SLAB_RECLAIM_ACCOUNT)
		*p++ = 'a';
	if (s->flags & SLAB_CONSISTENCY_CHECKS)
		*p++ = 'F';
	if (s->flags & SLAB_ACCOUNT)
		*p++ = 'A';
	if (p != name + 1)
		*p++ = '-';
	p += sprintf(p, "%07u", s->size);

	BUG_ON(p > name + ID_STR_LENGTH - 1);
	return name;
}

<<<<<<< HEAD
static void sysfs_slab_remove_workfn(struct work_struct *work)
{
	struct kmem_cache *s =
		container_of(work, struct kmem_cache, kobj_remove_work);

	if (!s->kobj.state_in_sysfs)
		/*
		 * For a memcg cache, this may be called during
		 * deactivation and again on shutdown.  Remove only once.
		 * A cache is never shut down before deactivation is
		 * complete, so no need to worry about synchronization.
		 */
		goto out;

#ifdef CONFIG_MEMCG
	kset_unregister(s->memcg_kset);
#endif
	kobject_uevent(&s->kobj, KOBJ_REMOVE);
out:
	kobject_put(&s->kobj);
}

=======
>>>>>>> upstream/android-13
static int sysfs_slab_add(struct kmem_cache *s)
{
	int err;
	const char *name;
	struct kset *kset = cache_kset(s);
	int unmergeable = slab_unmergeable(s);

<<<<<<< HEAD
	INIT_WORK(&s->kobj_remove_work, sysfs_slab_remove_workfn);

=======
>>>>>>> upstream/android-13
	if (!kset) {
		kobject_init(&s->kobj, &slab_ktype);
		return 0;
	}

	if (!unmergeable && disable_higher_order_debug &&
			(slub_debug & DEBUG_METADATA_FLAGS))
		unmergeable = 1;

	if (unmergeable) {
		/*
		 * Slabcache can never be merged so we can use the name proper.
		 * This is typically the case for debug situations. In that
		 * case we can catch duplicate names easily.
		 */
		sysfs_remove_link(&slab_kset->kobj, s->name);
		name = s->name;
	} else {
		/*
		 * Create a unique name for the slab as a target
		 * for the symlinks.
		 */
		name = create_unique_id(s);
	}

	s->kobj.kset = kset;
	err = kobject_init_and_add(&s->kobj, &slab_ktype, NULL, "%s", name);
	if (err)
		goto out;

	err = sysfs_create_group(&s->kobj, &slab_attr_group);
	if (err)
		goto out_del_kobj;

<<<<<<< HEAD
#ifdef CONFIG_MEMCG
	if (is_root_cache(s) && memcg_sysfs_enabled) {
		s->memcg_kset = kset_create_and_add("cgroup", NULL, &s->kobj);
		if (!s->memcg_kset) {
			err = -ENOMEM;
			goto out_del_kobj;
		}
	}
#endif

	kobject_uevent(&s->kobj, KOBJ_ADD);
=======
>>>>>>> upstream/android-13
	if (!unmergeable) {
		/* Setup first alias */
		sysfs_slab_alias(s, s->name);
	}
out:
	if (!unmergeable)
		kfree(name);
	return err;
out_del_kobj:
	kobject_del(&s->kobj);
	goto out;
}

<<<<<<< HEAD
static void sysfs_slab_remove(struct kmem_cache *s)
{
	if (slab_state < FULL)
		/*
		 * Sysfs has not been setup yet so no need to remove the
		 * cache from sysfs.
		 */
		return;

	kobject_get(&s->kobj);
	schedule_work(&s->kobj_remove_work);
}

=======
>>>>>>> upstream/android-13
void sysfs_slab_unlink(struct kmem_cache *s)
{
	if (slab_state >= FULL)
		kobject_del(&s->kobj);
}

void sysfs_slab_release(struct kmem_cache *s)
{
	if (slab_state >= FULL)
		kobject_put(&s->kobj);
}

/*
 * Need to buffer aliases during bootup until sysfs becomes
 * available lest we lose that information.
 */
struct saved_alias {
	struct kmem_cache *s;
	const char *name;
	struct saved_alias *next;
};

static struct saved_alias *alias_list;

static int sysfs_slab_alias(struct kmem_cache *s, const char *name)
{
	struct saved_alias *al;

	if (slab_state == FULL) {
		/*
		 * If we have a leftover link then remove it.
		 */
		sysfs_remove_link(&slab_kset->kobj, name);
		return sysfs_create_link(&slab_kset->kobj, &s->kobj, name);
	}

	al = kmalloc(sizeof(struct saved_alias), GFP_KERNEL);
	if (!al)
		return -ENOMEM;

	al->s = s;
	al->name = name;
	al->next = alias_list;
	alias_list = al;
	return 0;
}

static int __init slab_sysfs_init(void)
{
	struct kmem_cache *s;
	int err;

	mutex_lock(&slab_mutex);

<<<<<<< HEAD
	slab_kset = kset_create_and_add("slab", &slab_uevent_ops, kernel_kobj);
=======
	slab_kset = kset_create_and_add("slab", NULL, kernel_kobj);
>>>>>>> upstream/android-13
	if (!slab_kset) {
		mutex_unlock(&slab_mutex);
		pr_err("Cannot register slab subsystem.\n");
		return -ENOSYS;
	}

	slab_state = FULL;

	list_for_each_entry(s, &slab_caches, list) {
		err = sysfs_slab_add(s);
		if (err)
			pr_err("SLUB: Unable to add boot slab %s to sysfs\n",
			       s->name);
	}

	while (alias_list) {
		struct saved_alias *al = alias_list;

		alias_list = alias_list->next;
		err = sysfs_slab_alias(al->s, al->name);
		if (err)
			pr_err("SLUB: Unable to add boot slab alias %s to sysfs\n",
			       al->name);
		kfree(al);
	}

	mutex_unlock(&slab_mutex);
<<<<<<< HEAD
	resiliency_test();
=======
>>>>>>> upstream/android-13
	return 0;
}

__initcall(slab_sysfs_init);
#endif /* CONFIG_SYSFS */

<<<<<<< HEAD
=======
#if defined(CONFIG_SLUB_DEBUG) && defined(CONFIG_DEBUG_FS)
static int slab_debugfs_show(struct seq_file *seq, void *v)
{
	struct loc_track *t = seq->private;
	struct location *l;
	unsigned long idx;

	idx = (unsigned long) t->idx;
	if (idx < t->count) {
		l = &t->loc[idx];

		seq_printf(seq, "%7ld ", l->count);

		if (l->addr)
			seq_printf(seq, "%pS", (void *)l->addr);
		else
			seq_puts(seq, "<not-available>");

		if (l->sum_time != l->min_time) {
			seq_printf(seq, " age=%ld/%llu/%ld",
				l->min_time, div_u64(l->sum_time, l->count),
				l->max_time);
		} else
			seq_printf(seq, " age=%ld", l->min_time);

		if (l->min_pid != l->max_pid)
			seq_printf(seq, " pid=%ld-%ld", l->min_pid, l->max_pid);
		else
			seq_printf(seq, " pid=%ld",
				l->min_pid);

		if (num_online_cpus() > 1 && !cpumask_empty(to_cpumask(l->cpus)))
			seq_printf(seq, " cpus=%*pbl",
				 cpumask_pr_args(to_cpumask(l->cpus)));

		if (nr_online_nodes > 1 && !nodes_empty(l->nodes))
			seq_printf(seq, " nodes=%*pbl",
				 nodemask_pr_args(&l->nodes));

		seq_puts(seq, "\n");
	}

	if (!idx && !t->count)
		seq_puts(seq, "No data\n");

	return 0;
}

static void slab_debugfs_stop(struct seq_file *seq, void *v)
{
}

static void *slab_debugfs_next(struct seq_file *seq, void *v, loff_t *ppos)
{
	struct loc_track *t = seq->private;

	t->idx = ++(*ppos);
	if (*ppos <= t->count)
		return ppos;

	return NULL;
}

static void *slab_debugfs_start(struct seq_file *seq, loff_t *ppos)
{
	struct loc_track *t = seq->private;

	t->idx = *ppos;
	return ppos;
}

static const struct seq_operations slab_debugfs_sops = {
	.start  = slab_debugfs_start,
	.next   = slab_debugfs_next,
	.stop   = slab_debugfs_stop,
	.show   = slab_debugfs_show,
};

static int slab_debug_trace_open(struct inode *inode, struct file *filep)
{

	struct kmem_cache_node *n;
	enum track_item alloc;
	int node;
	struct loc_track *t = __seq_open_private(filep, &slab_debugfs_sops,
						sizeof(struct loc_track));
	struct kmem_cache *s = file_inode(filep)->i_private;
	unsigned long *obj_map;

	if (!t)
		return -ENOMEM;

	obj_map = bitmap_alloc(oo_objects(s->oo), GFP_KERNEL);
	if (!obj_map) {
		seq_release_private(inode, filep);
		return -ENOMEM;
	}

	if (strcmp(filep->f_path.dentry->d_name.name, "alloc_traces") == 0)
		alloc = TRACK_ALLOC;
	else
		alloc = TRACK_FREE;

	if (!alloc_loc_track(t, PAGE_SIZE / sizeof(struct location), GFP_KERNEL)) {
		bitmap_free(obj_map);
		seq_release_private(inode, filep);
		return -ENOMEM;
	}

	for_each_kmem_cache_node(s, node, n) {
		unsigned long flags;
		struct page *page;

		if (!atomic_long_read(&n->nr_slabs))
			continue;

		spin_lock_irqsave(&n->list_lock, flags);
		list_for_each_entry(page, &n->partial, slab_list)
			process_slab(t, s, page, alloc, obj_map);
		list_for_each_entry(page, &n->full, slab_list)
			process_slab(t, s, page, alloc, obj_map);
		spin_unlock_irqrestore(&n->list_lock, flags);
	}

	bitmap_free(obj_map);
	return 0;
}

static int slab_debug_trace_release(struct inode *inode, struct file *file)
{
	struct seq_file *seq = file->private_data;
	struct loc_track *t = seq->private;

	free_loc_track(t);
	return seq_release_private(inode, file);
}

static const struct file_operations slab_debugfs_fops = {
	.open    = slab_debug_trace_open,
	.read    = seq_read,
	.llseek  = seq_lseek,
	.release = slab_debug_trace_release,
};

static void debugfs_slab_add(struct kmem_cache *s)
{
	struct dentry *slab_cache_dir;

	if (unlikely(!slab_debugfs_root))
		return;

	slab_cache_dir = debugfs_create_dir(s->name, slab_debugfs_root);

	debugfs_create_file("alloc_traces", 0400,
		slab_cache_dir, s, &slab_debugfs_fops);

	debugfs_create_file("free_traces", 0400,
		slab_cache_dir, s, &slab_debugfs_fops);
}

void debugfs_slab_release(struct kmem_cache *s)
{
	debugfs_remove_recursive(debugfs_lookup(s->name, slab_debugfs_root));
}

static int __init slab_debugfs_init(void)
{
	struct kmem_cache *s;

	slab_debugfs_root = debugfs_create_dir("slab", NULL);

	list_for_each_entry(s, &slab_caches, list)
		if (s->flags & SLAB_STORE_USER)
			debugfs_slab_add(s);

	return 0;

}
__initcall(slab_debugfs_init);
#endif
>>>>>>> upstream/android-13
/*
 * The /proc/slabinfo ABI
 */
#ifdef CONFIG_SLUB_DEBUG
void get_slabinfo(struct kmem_cache *s, struct slabinfo *sinfo)
{
	unsigned long nr_slabs = 0;
	unsigned long nr_objs = 0;
	unsigned long nr_free = 0;
	int node;
	struct kmem_cache_node *n;

	for_each_kmem_cache_node(s, node, n) {
		nr_slabs += node_nr_slabs(n);
		nr_objs += node_nr_objs(n);
		nr_free += count_partial(n, count_free);
	}

	sinfo->active_objs = nr_objs - nr_free;
	sinfo->num_objs = nr_objs;
	sinfo->active_slabs = nr_slabs;
	sinfo->num_slabs = nr_slabs;
	sinfo->objects_per_slab = oo_objects(s->oo);
	sinfo->cache_order = oo_order(s->oo);
}
<<<<<<< HEAD
=======
EXPORT_SYMBOL_NS_GPL(get_slabinfo, MINIDUMP);
>>>>>>> upstream/android-13

void slabinfo_show_stats(struct seq_file *m, struct kmem_cache *s)
{
}

ssize_t slabinfo_write(struct file *file, const char __user *buffer,
		       size_t count, loff_t *ppos)
{
	return -EIO;
}
#endif /* CONFIG_SLUB_DEBUG */
