<<<<<<< HEAD
/*
 * Copyright (C) 2016 Red Hat
 * Author: Rob Clark <robdclark@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published by
 * the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
=======
// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright (C) 2016 Red Hat
 * Author: Rob Clark <robdclark@gmail.com>
>>>>>>> upstream/android-13
 */

#include "msm_drv.h"
#include "msm_gem.h"
<<<<<<< HEAD

static bool msm_gem_shrinker_lock(struct drm_device *dev, bool *unlock)
{
	/* NOTE: we are *closer* to being able to get rid of
	 * mutex_trylock_recursive().. the msm_gem code itself does
	 * not need struct_mutex, although codepaths that can trigger
	 * shrinker are still called in code-paths that hold the
	 * struct_mutex.
	 *
	 * Also, msm_obj->madv is protected by struct_mutex.
	 *
	 * The next step is probably split out a seperate lock for
	 * protecting inactive_list, so that shrinker does not need
	 * struct_mutex.
	 */
	switch (mutex_trylock_recursive(&dev->struct_mutex)) {
	case MUTEX_TRYLOCK_FAILED:
		return false;

	case MUTEX_TRYLOCK_SUCCESS:
		*unlock = true;
		return true;

	case MUTEX_TRYLOCK_RECURSIVE:
		*unlock = false;
		return true;
	}

	BUG();
=======
#include "msm_gpu.h"
#include "msm_gpu_trace.h"

/* Default disabled for now until it has some more testing on the different
 * iommu combinations that can be paired with the driver:
 */
bool enable_eviction = false;
MODULE_PARM_DESC(enable_eviction, "Enable swappable GEM buffers");
module_param(enable_eviction, bool, 0600);

static bool can_swap(void)
{
	return enable_eviction && get_nr_swap_pages() > 0;
>>>>>>> upstream/android-13
}

static unsigned long
msm_gem_shrinker_count(struct shrinker *shrinker, struct shrink_control *sc)
{
	struct msm_drm_private *priv =
		container_of(shrinker, struct msm_drm_private, shrinker);
<<<<<<< HEAD
	struct drm_device *dev = priv->dev;
	struct msm_gem_object *msm_obj;
	unsigned long count = 0;
	bool unlock;

	if (!msm_gem_shrinker_lock(dev, &unlock))
		return 0;

	list_for_each_entry(msm_obj, &priv->inactive_list, mm_list) {
		if (is_purgeable(msm_obj))
			count += msm_obj->base.size >> PAGE_SHIFT;
	}

	if (unlock)
		mutex_unlock(&dev->struct_mutex);
=======
	unsigned count = priv->shrinkable_count;

	if (can_swap())
		count += priv->evictable_count;
>>>>>>> upstream/android-13

	return count;
}

<<<<<<< HEAD
=======
static bool
purge(struct msm_gem_object *msm_obj)
{
	if (!is_purgeable(msm_obj))
		return false;

	/*
	 * This will move the obj out of still_in_list to
	 * the purged list
	 */
	msm_gem_purge(&msm_obj->base);

	return true;
}

static bool
evict(struct msm_gem_object *msm_obj)
{
	if (is_unevictable(msm_obj))
		return false;

	msm_gem_evict(&msm_obj->base);

	return true;
}

static unsigned long
scan(struct msm_drm_private *priv, unsigned nr_to_scan, struct list_head *list,
		bool (*shrink)(struct msm_gem_object *msm_obj))
{
	unsigned freed = 0;
	struct list_head still_in_list;

	INIT_LIST_HEAD(&still_in_list);

	mutex_lock(&priv->mm_lock);

	while (freed < nr_to_scan) {
		struct msm_gem_object *msm_obj = list_first_entry_or_null(
				list, typeof(*msm_obj), mm_list);

		if (!msm_obj)
			break;

		list_move_tail(&msm_obj->mm_list, &still_in_list);

		/*
		 * If it is in the process of being freed, msm_gem_free_object
		 * can be blocked on mm_lock waiting to remove it.  So just
		 * skip it.
		 */
		if (!kref_get_unless_zero(&msm_obj->base.refcount))
			continue;

		/*
		 * Now that we own a reference, we can drop mm_lock for the
		 * rest of the loop body, to reduce contention with the
		 * retire_submit path (which could make more objects purgeable)
		 */

		mutex_unlock(&priv->mm_lock);

		/*
		 * Note that this still needs to be trylock, since we can
		 * hit shrinker in response to trying to get backing pages
		 * for this obj (ie. while it's lock is already held)
		 */
		if (!msm_gem_trylock(&msm_obj->base))
			goto tail;

		if (shrink(msm_obj))
			freed += msm_obj->base.size >> PAGE_SHIFT;

		msm_gem_unlock(&msm_obj->base);

tail:
		drm_gem_object_put(&msm_obj->base);
		mutex_lock(&priv->mm_lock);
	}

	list_splice_tail(&still_in_list, list);
	mutex_unlock(&priv->mm_lock);

	return freed;
}

>>>>>>> upstream/android-13
static unsigned long
msm_gem_shrinker_scan(struct shrinker *shrinker, struct shrink_control *sc)
{
	struct msm_drm_private *priv =
		container_of(shrinker, struct msm_drm_private, shrinker);
<<<<<<< HEAD
	struct drm_device *dev = priv->dev;
	struct msm_gem_object *msm_obj;
	unsigned long freed = 0;
	bool unlock;

	if (!msm_gem_shrinker_lock(dev, &unlock))
		return SHRINK_STOP;

	list_for_each_entry(msm_obj, &priv->inactive_list, mm_list) {
		if (freed >= sc->nr_to_scan)
			break;
		if (is_purgeable(msm_obj)) {
			msm_gem_purge(&msm_obj->base, OBJ_LOCK_SHRINKER);
			freed += msm_obj->base.size >> PAGE_SHIFT;
		}
	}

	if (unlock)
		mutex_unlock(&dev->struct_mutex);

	if (freed > 0)
		pr_info_ratelimited("Purging %lu bytes\n", freed << PAGE_SHIFT);

	return freed;
=======
	unsigned long freed;

	freed = scan(priv, sc->nr_to_scan, &priv->inactive_dontneed, purge);

	if (freed > 0)
		trace_msm_gem_purge(freed << PAGE_SHIFT);

	if (can_swap() && freed < sc->nr_to_scan) {
		int evicted = scan(priv, sc->nr_to_scan - freed,
				&priv->inactive_willneed, evict);

		if (evicted > 0)
			trace_msm_gem_evict(evicted << PAGE_SHIFT);

		freed += evicted;
	}

	return (freed > 0) ? freed : SHRINK_STOP;
}

#ifdef CONFIG_DEBUG_FS
unsigned long
msm_gem_shrinker_shrink(struct drm_device *dev, unsigned long nr_to_scan)
{
	struct msm_drm_private *priv = dev->dev_private;
	struct shrink_control sc = {
		.nr_to_scan = nr_to_scan,
	};
	int ret;

	fs_reclaim_acquire(GFP_KERNEL);
	ret = msm_gem_shrinker_scan(&priv->shrinker, &sc);
	fs_reclaim_release(GFP_KERNEL);

	return ret;
}
#endif

/* since we don't know any better, lets bail after a few
 * and if necessary the shrinker will be invoked again.
 * Seems better than unmapping *everything*
 */
static const int vmap_shrink_limit = 15;

static bool
vmap_shrink(struct msm_gem_object *msm_obj)
{
	if (!is_vunmapable(msm_obj))
		return false;

	msm_gem_vunmap(&msm_obj->base);

	return true;
>>>>>>> upstream/android-13
}

static int
msm_gem_shrinker_vmap(struct notifier_block *nb, unsigned long event, void *ptr)
{
	struct msm_drm_private *priv =
		container_of(nb, struct msm_drm_private, vmap_notifier);
<<<<<<< HEAD
	struct drm_device *dev = priv->dev;
	struct msm_gem_object *msm_obj;
	unsigned unmapped = 0;
	bool unlock;

	if (!msm_gem_shrinker_lock(dev, &unlock))
		return NOTIFY_DONE;

	list_for_each_entry(msm_obj, &priv->inactive_list, mm_list) {
		if (is_vunmapable(msm_obj)) {
			msm_gem_vunmap(&msm_obj->base, OBJ_LOCK_SHRINKER);
			/* since we don't know any better, lets bail after a few
			 * and if necessary the shrinker will be invoked again.
			 * Seems better than unmapping *everything*
			 */
			if (++unmapped >= 15)
				break;
		}
	}

	if (unlock)
		mutex_unlock(&dev->struct_mutex);

	*(unsigned long *)ptr += unmapped;

	if (unmapped > 0)
		pr_info_ratelimited("Purging %u vmaps\n", unmapped);
=======
	struct list_head *mm_lists[] = {
		&priv->inactive_dontneed,
		&priv->inactive_willneed,
		priv->gpu ? &priv->gpu->active_list : NULL,
		NULL,
	};
	unsigned idx, unmapped = 0;

	for (idx = 0; mm_lists[idx] && unmapped < vmap_shrink_limit; idx++) {
		unmapped += scan(priv, vmap_shrink_limit - unmapped,
				mm_lists[idx], vmap_shrink);
	}

	*(unsigned long *)ptr += unmapped;

	if (unmapped > 0)
		trace_msm_gem_purge_vmaps(unmapped);
>>>>>>> upstream/android-13

	return NOTIFY_DONE;
}

/**
 * msm_gem_shrinker_init - Initialize msm shrinker
<<<<<<< HEAD
 * @dev_priv: msm device
=======
 * @dev: drm device
>>>>>>> upstream/android-13
 *
 * This function registers and sets up the msm shrinker.
 */
void msm_gem_shrinker_init(struct drm_device *dev)
{
	struct msm_drm_private *priv = dev->dev_private;
	priv->shrinker.count_objects = msm_gem_shrinker_count;
	priv->shrinker.scan_objects = msm_gem_shrinker_scan;
	priv->shrinker.seeks = DEFAULT_SEEKS;
	WARN_ON(register_shrinker(&priv->shrinker));

	priv->vmap_notifier.notifier_call = msm_gem_shrinker_vmap;
	WARN_ON(register_vmap_purge_notifier(&priv->vmap_notifier));
}

/**
 * msm_gem_shrinker_cleanup - Clean up msm shrinker
<<<<<<< HEAD
 * @dev_priv: msm device
=======
 * @dev: drm device
>>>>>>> upstream/android-13
 *
 * This function unregisters the msm shrinker.
 */
void msm_gem_shrinker_cleanup(struct drm_device *dev)
{
	struct msm_drm_private *priv = dev->dev_private;

	if (priv->shrinker.nr_deferred) {
		WARN_ON(unregister_vmap_purge_notifier(&priv->vmap_notifier));
		unregister_shrinker(&priv->shrinker);
	}
}
