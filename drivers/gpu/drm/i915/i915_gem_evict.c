/*
 * Copyright © 2008-2010 Intel Corporation
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the next
 * paragraph) shall be included in all copies or substantial portions of the
 * Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 *
 * Authors:
 *    Eric Anholt <eric@anholt.net>
 *    Chris Wilson <chris@chris-wilson.co.uuk>
 *
 */

<<<<<<< HEAD
#include <drm/drmP.h>
#include <drm/i915_drm.h>

#include "i915_drv.h"
#include "intel_drv.h"
=======
#include "gem/i915_gem_context.h"
#include "gt/intel_gt.h"
#include "gt/intel_gt_requests.h"

#include "i915_drv.h"
>>>>>>> upstream/android-13
#include "i915_trace.h"

I915_SELFTEST_DECLARE(static struct igt_evict_ctl {
	bool fail_if_busy:1;
} igt_evict_ctl;)

<<<<<<< HEAD
static bool ggtt_is_idle(struct drm_i915_private *i915)
{
       struct intel_engine_cs *engine;
       enum intel_engine_id id;

       if (i915->gt.active_requests)
	       return false;

       for_each_engine(engine, i915, id) {
	       if (!intel_engine_has_kernel_context(engine))
		       return false;
       }

       return true;
}

static int ggtt_flush(struct drm_i915_private *i915)
{
	int err;

	/* Not everything in the GGTT is tracked via vma (otherwise we
=======
static int ggtt_flush(struct intel_gt *gt)
{
	/*
	 * Not everything in the GGTT is tracked via vma (otherwise we
>>>>>>> upstream/android-13
	 * could evict as required with minimal stalling) so we are forced
	 * to idle the GPU and explicitly retire outstanding requests in
	 * the hopes that we can then remove contexts and the like only
	 * bound by their active reference.
	 */
<<<<<<< HEAD
	err = i915_gem_switch_to_kernel_context(i915);
	if (err)
		return err;

	err = i915_gem_wait_for_idle(i915,
				     I915_WAIT_INTERRUPTIBLE |
				     I915_WAIT_LOCKED,
				     MAX_SCHEDULE_TIMEOUT);
	if (err)
		return err;

	GEM_BUG_ON(!ggtt_is_idle(i915));
	return 0;
=======
	return intel_gt_wait_for_idle(gt, MAX_SCHEDULE_TIMEOUT);
>>>>>>> upstream/android-13
}

static bool
mark_free(struct drm_mm_scan *scan,
	  struct i915_vma *vma,
	  unsigned int flags,
	  struct list_head *unwind)
{
	if (i915_vma_is_pinned(vma))
		return false;

<<<<<<< HEAD
	if (flags & PIN_NONFAULT && i915_vma_has_userfault(vma))
		return false;

=======
>>>>>>> upstream/android-13
	list_add(&vma->evict_link, unwind);
	return drm_mm_scan_add_block(scan, &vma->node);
}

<<<<<<< HEAD
=======
static bool defer_evict(struct i915_vma *vma)
{
	if (i915_vma_is_active(vma))
		return true;

	if (i915_vma_is_scanout(vma))
		return true;

	return false;
}

>>>>>>> upstream/android-13
/**
 * i915_gem_evict_something - Evict vmas to make room for binding a new one
 * @vm: address space to evict from
 * @min_size: size of the desired free space
 * @alignment: alignment constraint of the desired free space
<<<<<<< HEAD
 * @cache_level: cache_level for the desired space
=======
 * @color: color for the desired space
>>>>>>> upstream/android-13
 * @start: start (inclusive) of the range from which to evict objects
 * @end: end (exclusive) of the range from which to evict objects
 * @flags: additional flags to control the eviction algorithm
 *
 * This function will try to evict vmas until a free space satisfying the
 * requirements is found. Callers must check first whether any such hole exists
 * already before calling this function.
 *
 * This function is used by the object/vma binding code.
 *
 * Since this function is only used to free up virtual address space it only
 * ignores pinned vmas, and not object where the backing storage itself is
 * pinned. Hence obj->pages_pin_count does not protect against eviction.
 *
 * To clarify: This is for freeing up virtual address space, not for freeing
 * memory in e.g. the shrinker.
 */
int
i915_gem_evict_something(struct i915_address_space *vm,
			 u64 min_size, u64 alignment,
<<<<<<< HEAD
			 unsigned cache_level,
			 u64 start, u64 end,
			 unsigned flags)
{
	struct drm_i915_private *dev_priv = vm->i915;
	struct drm_mm_scan scan;
	struct list_head eviction_list;
	struct list_head *phases[] = {
		&vm->inactive_list,
		&vm->active_list,
		NULL,
	}, **phase;
	struct i915_vma *vma, *next;
	struct drm_mm_node *node;
	enum drm_mm_insert_mode mode;
	int ret;

	lockdep_assert_held(&vm->i915->drm.struct_mutex);
	trace_i915_gem_evict(vm, min_size, alignment, flags);

	/*
	 * The goal is to evict objects and amalgamate space in LRU order.
	 * The oldest idle objects reside on the inactive list, which is in
	 * retirement order. The next objects to retire are those in flight,
	 * on the active list, again in retirement order.
	 *
	 * The retirement sequence is thus:
	 *   1. Inactive objects (already retired)
	 *   2. Active objects (will stall on unbinding)
	 *
	 * On each list, the oldest objects lie at the HEAD with the freshest
	 * object on the TAIL.
=======
			 unsigned long color,
			 u64 start, u64 end,
			 unsigned flags)
{
	struct drm_mm_scan scan;
	struct list_head eviction_list;
	struct i915_vma *vma, *next;
	struct drm_mm_node *node;
	enum drm_mm_insert_mode mode;
	struct i915_vma *active;
	int ret;

	lockdep_assert_held(&vm->mutex);
	trace_i915_gem_evict(vm, min_size, alignment, flags);

	/*
	 * The goal is to evict objects and amalgamate space in rough LRU order.
	 * Since both active and inactive objects reside on the same list,
	 * in a mix of creation and last scanned order, as we process the list
	 * we sort it into inactive/active, which keeps the active portion
	 * in a rough MRU order.
	 *
	 * The retirement sequence is thus:
	 *   1. Inactive objects (already retired, random order)
	 *   2. Active objects (will stall on unbinding, oldest scanned first)
>>>>>>> upstream/android-13
	 */
	mode = DRM_MM_INSERT_BEST;
	if (flags & PIN_HIGH)
		mode = DRM_MM_INSERT_HIGH;
	if (flags & PIN_MAPPABLE)
		mode = DRM_MM_INSERT_LOW;
	drm_mm_scan_init_with_range(&scan, &vm->mm,
<<<<<<< HEAD
				    min_size, alignment, cache_level,
				    start, end, mode);

	/*
	 * Retire before we search the active list. Although we have
	 * reasonable accuracy in our retirement lists, we may have
	 * a stray pin (preventing eviction) that can only be resolved by
	 * retiring.
	 */
	if (!(flags & PIN_NONBLOCK))
		i915_retire_requests(dev_priv);
	else
		phases[1] = NULL;

search_again:
	INIT_LIST_HEAD(&eviction_list);
	phase = phases;
	do {
		list_for_each_entry(vma, *phase, vm_link)
			if (mark_free(&scan, vma, flags, &eviction_list))
				goto found;
	} while (*++phase);
=======
				    min_size, alignment, color,
				    start, end, mode);

	intel_gt_retire_requests(vm->gt);

search_again:
	active = NULL;
	INIT_LIST_HEAD(&eviction_list);
	list_for_each_entry_safe(vma, next, &vm->bound_list, vm_link) {
		if (vma == active) { /* now seen this vma twice */
			if (flags & PIN_NONBLOCK)
				break;

			active = ERR_PTR(-EAGAIN);
		}

		/*
		 * We keep this list in a rough least-recently scanned order
		 * of active elements (inactive elements are cheap to reap).
		 * New entries are added to the end, and we move anything we
		 * scan to the end. The assumption is that the working set
		 * of applications is either steady state (and thanks to the
		 * userspace bo cache it almost always is) or volatile and
		 * frequently replaced after a frame, which are self-evicting!
		 * Given that assumption, the MRU order of the scan list is
		 * fairly static, and keeping it in least-recently scan order
		 * is suitable.
		 *
		 * To notice when we complete one full cycle, we record the
		 * first active element seen, before moving it to the tail.
		 */
		if (active != ERR_PTR(-EAGAIN) && defer_evict(vma)) {
			if (!active)
				active = vma;

			list_move_tail(&vma->vm_link, &vm->bound_list);
			continue;
		}

		if (mark_free(&scan, vma, flags, &eviction_list))
			goto found;
	}
>>>>>>> upstream/android-13

	/* Nothing found, clean up and bail out! */
	list_for_each_entry_safe(vma, next, &eviction_list, evict_link) {
		ret = drm_mm_scan_remove_block(&scan, &vma->node);
		BUG_ON(ret);
	}

	/*
	 * Can we unpin some objects such as idle hw contents,
	 * or pending flips? But since only the GGTT has global entries
	 * such as scanouts, rinbuffers and contexts, we can skip the
	 * purge when inspecting per-process local address spaces.
	 */
	if (!i915_is_ggtt(vm) || flags & PIN_NONBLOCK)
		return -ENOSPC;

	/*
	 * Not everything in the GGTT is tracked via VMA using
	 * i915_vma_move_to_active(), otherwise we could evict as required
	 * with minimal stalling. Instead we are forced to idle the GPU and
	 * explicitly retire outstanding requests which will then remove
	 * the pinning for active objects such as contexts and ring,
	 * enabling us to evict them on the next iteration.
	 *
	 * To ensure that all user contexts are evictable, we perform
	 * a switch to the perma-pinned kernel context. This all also gives
	 * us a termination condition, when the last retired context is
	 * the kernel's there is no more we can evict.
	 */
<<<<<<< HEAD
	if (!ggtt_is_idle(dev_priv)) {
		if (I915_SELFTEST_ONLY(igt_evict_ctl.fail_if_busy))
			return -EBUSY;

		ret = ggtt_flush(dev_priv);
		if (ret)
			return ret;

		cond_resched();
		goto search_again;
	}

	/*
	 * If we still have pending pageflip completions, drop
	 * back to userspace to give our workqueues time to
	 * acquire our locks and unpin the old scanouts.
	 */
	return intel_has_pending_fb_unpin(dev_priv) ? -EAGAIN : -ENOSPC;
=======
	if (I915_SELFTEST_ONLY(igt_evict_ctl.fail_if_busy))
		return -EBUSY;

	ret = ggtt_flush(vm->gt);
	if (ret)
		return ret;

	cond_resched();

	flags |= PIN_NONBLOCK;
	goto search_again;
>>>>>>> upstream/android-13

found:
	/* drm_mm doesn't allow any other other operations while
	 * scanning, therefore store to-be-evicted objects on a
	 * temporary list and take a reference for all before
	 * calling unbind (which may remove the active reference
	 * of any of our objects, thus corrupting the list).
	 */
	list_for_each_entry_safe(vma, next, &eviction_list, evict_link) {
		if (drm_mm_scan_remove_block(&scan, &vma->node))
			__i915_vma_pin(vma);
		else
			list_del(&vma->evict_link);
	}

	/* Unbinding will emit any required flushes */
	ret = 0;
	list_for_each_entry_safe(vma, next, &eviction_list, evict_link) {
		__i915_vma_unpin(vma);
		if (ret == 0)
<<<<<<< HEAD
			ret = i915_vma_unbind(vma);
=======
			ret = __i915_vma_unbind(vma);
>>>>>>> upstream/android-13
	}

	while (ret == 0 && (node = drm_mm_scan_color_evict(&scan))) {
		vma = container_of(node, struct i915_vma, node);
<<<<<<< HEAD
		ret = i915_vma_unbind(vma);
=======

		/* If we find any non-objects (!vma), we cannot evict them */
		if (vma->node.color != I915_COLOR_UNEVICTABLE)
			ret = __i915_vma_unbind(vma);
		else
			ret = -ENOSPC; /* XXX search failed, try again? */
>>>>>>> upstream/android-13
	}

	return ret;
}

/**
<<<<<<< HEAD
 * i915_gem_evict_for_vma - Evict vmas to make room for binding a new one
=======
 * i915_gem_evict_for_node - Evict vmas to make room for binding a new one
>>>>>>> upstream/android-13
 * @vm: address space to evict from
 * @target: range (and color) to evict for
 * @flags: additional flags to control the eviction algorithm
 *
 * This function will try to evict vmas that overlap the target node.
 *
 * To clarify: This is for freeing up virtual address space, not for freeing
 * memory in e.g. the shrinker.
 */
int i915_gem_evict_for_node(struct i915_address_space *vm,
			    struct drm_mm_node *target,
			    unsigned int flags)
{
	LIST_HEAD(eviction_list);
	struct drm_mm_node *node;
	u64 start = target->start;
	u64 end = start + target->size;
	struct i915_vma *vma, *next;
<<<<<<< HEAD
	bool check_color;
	int ret = 0;

	lockdep_assert_held(&vm->i915->drm.struct_mutex);
=======
	int ret = 0;

	lockdep_assert_held(&vm->mutex);
>>>>>>> upstream/android-13
	GEM_BUG_ON(!IS_ALIGNED(start, I915_GTT_PAGE_SIZE));
	GEM_BUG_ON(!IS_ALIGNED(end, I915_GTT_PAGE_SIZE));

	trace_i915_gem_evict_node(vm, target, flags);

<<<<<<< HEAD
	/* Retire before we search the active list. Although we have
=======
	/*
	 * Retire before we search the active list. Although we have
>>>>>>> upstream/android-13
	 * reasonable accuracy in our retirement lists, we may have
	 * a stray pin (preventing eviction) that can only be resolved by
	 * retiring.
	 */
<<<<<<< HEAD
	if (!(flags & PIN_NONBLOCK))
		i915_retire_requests(vm->i915);

	check_color = vm->mm.color_adjust;
	if (check_color) {
=======
	intel_gt_retire_requests(vm->gt);

	if (i915_vm_has_cache_coloring(vm)) {
>>>>>>> upstream/android-13
		/* Expand search to cover neighbouring guard pages (or lack!) */
		if (start)
			start -= I915_GTT_PAGE_SIZE;

		/* Always look at the page afterwards to avoid the end-of-GTT */
		end += I915_GTT_PAGE_SIZE;
	}
	GEM_BUG_ON(start >= end);

	drm_mm_for_each_node_in_range(node, &vm->mm, start, end) {
		/* If we find any non-objects (!vma), we cannot evict them */
		if (node->color == I915_COLOR_UNEVICTABLE) {
			ret = -ENOSPC;
			break;
		}

<<<<<<< HEAD
		GEM_BUG_ON(!node->allocated);
		vma = container_of(node, typeof(*vma), node);

		/* If we are using coloring to insert guard pages between
=======
		GEM_BUG_ON(!drm_mm_node_allocated(node));
		vma = container_of(node, typeof(*vma), node);

		/*
		 * If we are using coloring to insert guard pages between
>>>>>>> upstream/android-13
		 * different cache domains within the address space, we have
		 * to check whether the objects on either side of our range
		 * abutt and conflict. If they are in conflict, then we evict
		 * those as well to make room for our guard pages.
		 */
<<<<<<< HEAD
		if (check_color) {
=======
		if (i915_vm_has_cache_coloring(vm)) {
>>>>>>> upstream/android-13
			if (node->start + node->size == target->start) {
				if (node->color == target->color)
					continue;
			}
			if (node->start == target->start + target->size) {
				if (node->color == target->color)
					continue;
			}
		}

<<<<<<< HEAD
		if (flags & PIN_NONBLOCK &&
		    (i915_vma_is_pinned(vma) || i915_vma_is_active(vma))) {
			ret = -ENOSPC;
			break;
		}

		if (flags & PIN_NONFAULT && i915_vma_has_userfault(vma)) {
			ret = -ENOSPC;
			break;
		}

		/* Overlap of objects in the same batch? */
		if (i915_vma_is_pinned(vma)) {
			ret = -ENOSPC;
			if (vma->exec_flags &&
			    *vma->exec_flags & EXEC_OBJECT_PINNED)
				ret = -EINVAL;
			break;
		}

		/* Never show fear in the face of dragons!
=======
		if (i915_vma_is_pinned(vma)) {
			ret = -ENOSPC;
			break;
		}

		if (flags & PIN_NONBLOCK && i915_vma_is_active(vma)) {
			ret = -ENOSPC;
			break;
		}

		/*
		 * Never show fear in the face of dragons!
>>>>>>> upstream/android-13
		 *
		 * We cannot directly remove this node from within this
		 * iterator and as with i915_gem_evict_something() we employ
		 * the vma pin_count in order to prevent the action of
		 * unbinding one vma from freeing (by dropping its active
		 * reference) another in our eviction list.
		 */
		__i915_vma_pin(vma);
		list_add(&vma->evict_link, &eviction_list);
	}

	list_for_each_entry_safe(vma, next, &eviction_list, evict_link) {
		__i915_vma_unpin(vma);
		if (ret == 0)
<<<<<<< HEAD
			ret = i915_vma_unbind(vma);
=======
			ret = __i915_vma_unbind(vma);
>>>>>>> upstream/android-13
	}

	return ret;
}

/**
 * i915_gem_evict_vm - Evict all idle vmas from a vm
 * @vm: Address space to cleanse
 *
 * This function evicts all vmas from a vm.
 *
 * This is used by the execbuf code as a last-ditch effort to defragment the
 * address space.
 *
 * To clarify: This is for freeing up virtual address space, not for freeing
 * memory in e.g. the shrinker.
 */
int i915_gem_evict_vm(struct i915_address_space *vm)
{
<<<<<<< HEAD
	struct list_head *phases[] = {
		&vm->inactive_list,
		&vm->active_list,
		NULL
	}, **phase;
	struct list_head eviction_list;
	struct i915_vma *vma, *next;
	int ret;

	lockdep_assert_held(&vm->i915->drm.struct_mutex);
=======
	int ret = 0;

	lockdep_assert_held(&vm->mutex);
>>>>>>> upstream/android-13
	trace_i915_gem_evict_vm(vm);

	/* Switch back to the default context in order to unpin
	 * the existing context objects. However, such objects only
	 * pin themselves inside the global GTT and performing the
	 * switch otherwise is ineffective.
	 */
	if (i915_is_ggtt(vm)) {
<<<<<<< HEAD
		ret = ggtt_flush(vm->i915);
=======
		ret = ggtt_flush(vm->gt);
>>>>>>> upstream/android-13
		if (ret)
			return ret;
	}

<<<<<<< HEAD
	INIT_LIST_HEAD(&eviction_list);
	phase = phases;
	do {
		list_for_each_entry(vma, *phase, vm_link) {
=======
	do {
		struct i915_vma *vma, *vn;
		LIST_HEAD(eviction_list);

		list_for_each_entry(vma, &vm->bound_list, vm_link) {
>>>>>>> upstream/android-13
			if (i915_vma_is_pinned(vma))
				continue;

			__i915_vma_pin(vma);
			list_add(&vma->evict_link, &eviction_list);
		}
<<<<<<< HEAD
	} while (*++phase);

	ret = 0;
	list_for_each_entry_safe(vma, next, &eviction_list, evict_link) {
		__i915_vma_unpin(vma);
		if (ret == 0)
			ret = i915_vma_unbind(vma);
	}
=======
		if (list_empty(&eviction_list))
			break;

		ret = 0;
		list_for_each_entry_safe(vma, vn, &eviction_list, evict_link) {
			__i915_vma_unpin(vma);
			if (ret == 0)
				ret = __i915_vma_unbind(vma);
			if (ret != -EINTR) /* "Get me out of here!" */
				ret = 0;
		}
	} while (ret == 0);

>>>>>>> upstream/android-13
	return ret;
}

#if IS_ENABLED(CONFIG_DRM_I915_SELFTEST)
#include "selftests/i915_gem_evict.c"
#endif
