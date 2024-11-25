/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __LINUX_COMPLETION_H
#define __LINUX_COMPLETION_H

/*
 * (C) Copyright 2001 Linus Torvalds
 *
 * Atomic wait-for-completion handler data structures.
 * See kernel/sched/completion.c for details.
 */

<<<<<<< HEAD
#include <linux/wait.h>
=======
#include <linux/swait.h>
>>>>>>> upstream/android-13

/*
 * struct completion - structure used to maintain state for a "completion"
 *
 * This is the opaque structure used to maintain the state for a "completion".
 * Completions currently use a FIFO to queue threads that have to wait for
 * the "completion" event.
 *
 * See also:  complete(), wait_for_completion() (and friends _timeout,
 * _interruptible, _interruptible_timeout, and _killable), init_completion(),
 * reinit_completion(), and macros DECLARE_COMPLETION(),
 * DECLARE_COMPLETION_ONSTACK().
 */
struct completion {
	unsigned int done;
<<<<<<< HEAD
	wait_queue_head_t wait;
};

#define init_completion_map(x, m) __init_completion(x)
#define init_completion(x) __init_completion(x)
=======
	struct swait_queue_head wait;
};

#define init_completion_map(x, m) init_completion(x)
>>>>>>> upstream/android-13
static inline void complete_acquire(struct completion *x) {}
static inline void complete_release(struct completion *x) {}

#define COMPLETION_INITIALIZER(work) \
<<<<<<< HEAD
	{ 0, __WAIT_QUEUE_HEAD_INITIALIZER((work).wait) }
=======
	{ 0, __SWAIT_QUEUE_HEAD_INITIALIZER((work).wait) }
>>>>>>> upstream/android-13

#define COMPLETION_INITIALIZER_ONSTACK_MAP(work, map) \
	(*({ init_completion_map(&(work), &(map)); &(work); }))

#define COMPLETION_INITIALIZER_ONSTACK(work) \
	(*({ init_completion(&work); &work; }))

/**
 * DECLARE_COMPLETION - declare and initialize a completion structure
 * @work:  identifier for the completion structure
 *
 * This macro declares and initializes a completion structure. Generally used
 * for static declarations. You should use the _ONSTACK variant for automatic
 * variables.
 */
#define DECLARE_COMPLETION(work) \
	struct completion work = COMPLETION_INITIALIZER(work)

/*
 * Lockdep needs to run a non-constant initializer for on-stack
 * completions - so we use the _ONSTACK() variant for those that
 * are on the kernel stack:
 */
/**
 * DECLARE_COMPLETION_ONSTACK - declare and initialize a completion structure
 * @work:  identifier for the completion structure
 *
 * This macro declares and initializes a completion structure on the kernel
 * stack.
 */
#ifdef CONFIG_LOCKDEP
# define DECLARE_COMPLETION_ONSTACK(work) \
	struct completion work = COMPLETION_INITIALIZER_ONSTACK(work)
# define DECLARE_COMPLETION_ONSTACK_MAP(work, map) \
	struct completion work = COMPLETION_INITIALIZER_ONSTACK_MAP(work, map)
#else
# define DECLARE_COMPLETION_ONSTACK(work) DECLARE_COMPLETION(work)
# define DECLARE_COMPLETION_ONSTACK_MAP(work, map) DECLARE_COMPLETION(work)
#endif

/**
 * init_completion - Initialize a dynamically allocated completion
 * @x:  pointer to completion structure that is to be initialized
 *
 * This inline function will initialize a dynamically created completion
 * structure.
 */
<<<<<<< HEAD
static inline void __init_completion(struct completion *x)
{
	x->done = 0;
	init_waitqueue_head(&x->wait);
=======
static inline void init_completion(struct completion *x)
{
	x->done = 0;
	init_swait_queue_head(&x->wait);
>>>>>>> upstream/android-13
}

/**
 * reinit_completion - reinitialize a completion structure
 * @x:  pointer to completion structure that is to be reinitialized
 *
 * This inline function should be used to reinitialize a completion structure so it can
 * be reused. This is especially important after complete_all() is used.
 */
static inline void reinit_completion(struct completion *x)
{
	x->done = 0;
}

extern void wait_for_completion(struct completion *);
extern void wait_for_completion_io(struct completion *);
extern int wait_for_completion_interruptible(struct completion *x);
extern int wait_for_completion_killable(struct completion *x);
extern unsigned long wait_for_completion_timeout(struct completion *x,
						   unsigned long timeout);
extern unsigned long wait_for_completion_io_timeout(struct completion *x,
						    unsigned long timeout);
extern long wait_for_completion_interruptible_timeout(
	struct completion *x, unsigned long timeout);
extern long wait_for_completion_killable_timeout(
	struct completion *x, unsigned long timeout);
extern bool try_wait_for_completion(struct completion *x);
extern bool completion_done(struct completion *x);

extern void complete(struct completion *);
extern void complete_all(struct completion *);

#endif
