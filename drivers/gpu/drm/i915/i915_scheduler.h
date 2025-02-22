/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright © 2018 Intel Corporation
 */

#ifndef _I915_SCHEDULER_H_
#define _I915_SCHEDULER_H_

#include <linux/bitops.h>
<<<<<<< HEAD

#include <uapi/drm/i915_drm.h>

enum {
	I915_PRIORITY_MIN = I915_CONTEXT_MIN_USER_PRIORITY - 1,
	I915_PRIORITY_NORMAL = I915_CONTEXT_DEFAULT_PRIORITY,
	I915_PRIORITY_MAX = I915_CONTEXT_MAX_USER_PRIORITY + 1,

	I915_PRIORITY_INVALID = INT_MIN
};

struct i915_sched_attr {
	/**
	 * @priority: execution and service priority
	 *
	 * All clients are equal, but some are more equal than others!
	 *
	 * Requests from a context with a greater (more positive) value of
	 * @priority will be executed before those with a lower @priority
	 * value, forming a simple QoS.
	 *
	 * The &drm_i915_private.kernel_context is assigned the lowest priority.
	 */
	int priority;
};

/*
 * "People assume that time is a strict progression of cause to effect, but
 * actually, from a nonlinear, non-subjective viewpoint, it's more like a big
 * ball of wibbly-wobbly, timey-wimey ... stuff." -The Doctor, 2015
 *
 * Requests exist in a complex web of interdependencies. Each request
 * has to wait for some other request to complete before it is ready to be run
 * (e.g. we have to wait until the pixels have been rendering into a texture
 * before we can copy from it). We track the readiness of a request in terms
 * of fences, but we also need to keep the dependency tree for the lifetime
 * of the request (beyond the life of an individual fence). We use the tree
 * at various points to reorder the requests whilst keeping the requests
 * in order with respect to their various dependencies.
 *
 * There is no active component to the "scheduler". As we know the dependency
 * DAG of each request, we are able to insert it into a sorted queue when it
 * is ready, and are able to reorder its portion of the graph to accommodate
 * dynamic priority changes.
 */
struct i915_sched_node {
	struct list_head signalers_list; /* those before us, we depend upon */
	struct list_head waiters_list; /* those after us, they depend upon us */
	struct list_head link;
	struct i915_sched_attr attr;
};

struct i915_dependency {
	struct i915_sched_node *signaler;
	struct list_head signal_link;
	struct list_head wait_link;
	struct list_head dfs_link;
	unsigned long flags;
#define I915_DEPENDENCY_ALLOC BIT(0)
};
=======
#include <linux/list.h>
#include <linux/kernel.h>

#include "i915_scheduler_types.h"

struct drm_printer;

#define priolist_for_each_request(it, plist) \
	list_for_each_entry(it, &(plist)->requests, sched.link)

#define priolist_for_each_request_consume(it, n, plist) \
	list_for_each_entry_safe(it, n, &(plist)->requests, sched.link)

void i915_sched_node_init(struct i915_sched_node *node);
void i915_sched_node_reinit(struct i915_sched_node *node);

bool __i915_sched_node_add_dependency(struct i915_sched_node *node,
				      struct i915_sched_node *signal,
				      struct i915_dependency *dep,
				      unsigned long flags);

int i915_sched_node_add_dependency(struct i915_sched_node *node,
				   struct i915_sched_node *signal,
				   unsigned long flags);

void i915_sched_node_fini(struct i915_sched_node *node);

void i915_schedule(struct i915_request *request,
		   const struct i915_sched_attr *attr);

struct list_head *
i915_sched_lookup_priolist(struct i915_sched_engine *sched_engine, int prio);

void __i915_priolist_free(struct i915_priolist *p);
static inline void i915_priolist_free(struct i915_priolist *p)
{
	if (p->priority != I915_PRIORITY_NORMAL)
		__i915_priolist_free(p);
}

struct i915_sched_engine *
i915_sched_engine_create(unsigned int subclass);

static inline struct i915_sched_engine *
i915_sched_engine_get(struct i915_sched_engine *sched_engine)
{
	kref_get(&sched_engine->ref);
	return sched_engine;
}

static inline void
i915_sched_engine_put(struct i915_sched_engine *sched_engine)
{
	kref_put(&sched_engine->ref, sched_engine->destroy);
}

static inline bool
i915_sched_engine_is_empty(struct i915_sched_engine *sched_engine)
{
	return RB_EMPTY_ROOT(&sched_engine->queue.rb_root);
}

static inline void
i915_sched_engine_reset_on_empty(struct i915_sched_engine *sched_engine)
{
	if (i915_sched_engine_is_empty(sched_engine))
		sched_engine->no_priolist = false;
}

static inline void
i915_sched_engine_active_lock_bh(struct i915_sched_engine *sched_engine)
{
	local_bh_disable(); /* prevent local softirq and lock recursion */
	tasklet_lock(&sched_engine->tasklet);
}

static inline void
i915_sched_engine_active_unlock_bh(struct i915_sched_engine *sched_engine)
{
	tasklet_unlock(&sched_engine->tasklet);
	local_bh_enable(); /* restore softirq, and kick ksoftirqd! */
}

void i915_request_show_with_schedule(struct drm_printer *m,
				     const struct i915_request *rq,
				     const char *prefix,
				     int indent);

static inline bool
i915_sched_engine_disabled(struct i915_sched_engine *sched_engine)
{
	return sched_engine->disabled(sched_engine);
}

void i915_scheduler_module_exit(void);
int i915_scheduler_module_init(void);
>>>>>>> upstream/android-13

#endif /* _I915_SCHEDULER_H_ */
