/*
 * Copyright © 2016 Intel Corporation
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
 */

#ifndef __I915_GEM_H__
#define __I915_GEM_H__

#include <linux/bug.h>
#include <linux/interrupt.h>

<<<<<<< HEAD
=======
#include <drm/drm_drv.h>

#include "i915_utils.h"

>>>>>>> upstream/android-13
struct drm_i915_private;

#ifdef CONFIG_DRM_I915_DEBUG_GEM

<<<<<<< HEAD
#define GEM_SHOW_DEBUG() (drm_debug & DRM_UT_DRIVER)

#define GEM_BUG_ON(condition) do { if (unlikely((condition))) {	\
		pr_err("%s:%d GEM_BUG_ON(%s)\n", \
		       __func__, __LINE__, __stringify(condition)); \
		GEM_TRACE("%s:%d GEM_BUG_ON(%s)\n", \
			  __func__, __LINE__, __stringify(condition)); \
		BUG(); \
=======
#define GEM_SHOW_DEBUG() drm_debug_enabled(DRM_UT_DRIVER)

#ifdef CONFIG_DRM_I915_DEBUG_GEM_ONCE
#define __GEM_BUG(cond) BUG()
#else
#define __GEM_BUG(cond) \
	WARN(1, "%s:%d GEM_BUG_ON(%s)\n", __func__, __LINE__, __stringify(cond))
#endif

#define GEM_BUG_ON(condition) do { if (unlikely((condition))) {	\
		GEM_TRACE_ERR("%s:%d GEM_BUG_ON(%s)\n", \
			      __func__, __LINE__, __stringify(condition)); \
		GEM_TRACE_DUMP(); \
		__GEM_BUG(condition); \
>>>>>>> upstream/android-13
		} \
	} while(0)
#define GEM_WARN_ON(expr) WARN_ON(expr)

#define GEM_DEBUG_DECL(var) var
#define GEM_DEBUG_EXEC(expr) expr
#define GEM_DEBUG_BUG_ON(expr) GEM_BUG_ON(expr)
<<<<<<< HEAD
=======
#define GEM_DEBUG_WARN_ON(expr) GEM_WARN_ON(expr)
>>>>>>> upstream/android-13

#else

#define GEM_SHOW_DEBUG() (0)

#define GEM_BUG_ON(expr) BUILD_BUG_ON_INVALID(expr)
<<<<<<< HEAD
#define GEM_WARN_ON(expr) (BUILD_BUG_ON_INVALID(expr), 0)
=======
#define GEM_WARN_ON(expr) ({ unlikely(!!(expr)); })
>>>>>>> upstream/android-13

#define GEM_DEBUG_DECL(var)
#define GEM_DEBUG_EXEC(expr) do { } while (0)
#define GEM_DEBUG_BUG_ON(expr)
<<<<<<< HEAD
=======
#define GEM_DEBUG_WARN_ON(expr) ({ BUILD_BUG_ON_INVALID(expr); 0; })
>>>>>>> upstream/android-13
#endif

#if IS_ENABLED(CONFIG_DRM_I915_TRACE_GEM)
#define GEM_TRACE(...) trace_printk(__VA_ARGS__)
<<<<<<< HEAD
#define GEM_TRACE_DUMP() ftrace_dump(DUMP_ALL)
#define GEM_TRACE_DUMP_ON(expr) \
	do { if (expr) ftrace_dump(DUMP_ALL); } while (0)
#else
#define GEM_TRACE(...) do { } while (0)
=======
#define GEM_TRACE_ERR(...) do {						\
	pr_err(__VA_ARGS__);						\
	trace_printk(__VA_ARGS__);					\
} while (0)
#define GEM_TRACE_DUMP() \
	do { ftrace_dump(DUMP_ALL); __add_taint_for_CI(TAINT_WARN); } while (0)
#define GEM_TRACE_DUMP_ON(expr) \
	do { if (expr) GEM_TRACE_DUMP(); } while (0)
#else
#define GEM_TRACE(...) do { } while (0)
#define GEM_TRACE_ERR(...) do { } while (0)
>>>>>>> upstream/android-13
#define GEM_TRACE_DUMP() do { } while (0)
#define GEM_TRACE_DUMP_ON(expr) BUILD_BUG_ON_INVALID(expr)
#endif

<<<<<<< HEAD
#define I915_NUM_ENGINES 8

void i915_gem_park(struct drm_i915_private *i915);
void i915_gem_unpark(struct drm_i915_private *i915);

static inline void __tasklet_disable_sync_once(struct tasklet_struct *t)
{
	if (atomic_inc_return(&t->count) == 1)
		tasklet_unlock_wait(t);
}

static inline void __tasklet_enable_sync_once(struct tasklet_struct *t)
{
	if (atomic_dec_return(&t->count) == 0)
		tasklet_kill(t);
=======
#define I915_GEM_IDLE_TIMEOUT (HZ / 5)

static inline void tasklet_lock(struct tasklet_struct *t)
{
	while (!tasklet_trylock(t))
		cpu_relax();
}

static inline bool tasklet_is_locked(const struct tasklet_struct *t)
{
	return test_bit(TASKLET_STATE_RUN, &t->state);
}

static inline void __tasklet_disable_sync_once(struct tasklet_struct *t)
{
	if (!atomic_fetch_inc(&t->count))
		tasklet_unlock_spin_wait(t);
>>>>>>> upstream/android-13
}

static inline bool __tasklet_is_enabled(const struct tasklet_struct *t)
{
	return !atomic_read(&t->count);
}

<<<<<<< HEAD
=======
static inline bool __tasklet_enable(struct tasklet_struct *t)
{
	return atomic_dec_and_test(&t->count);
}

static inline bool __tasklet_is_scheduled(struct tasklet_struct *t)
{
	return test_bit(TASKLET_STATE_SCHED, &t->state);
}

>>>>>>> upstream/android-13
#endif /* __I915_GEM_H__ */
