<<<<<<< HEAD
/*
 * RCU segmented callback lists, internal-to-rcu header file
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, you can access it online at
 * http://www.gnu.org/licenses/gpl-2.0.html.
 *
 * Copyright IBM Corporation, 2017
 *
 * Authors: Paul E. McKenney <paulmck@linux.vnet.ibm.com>
=======
/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * RCU segmented callback lists, internal-to-rcu header file
 *
 * Copyright IBM Corporation, 2017
 *
 * Authors: Paul E. McKenney <paulmck@linux.ibm.com>
>>>>>>> upstream/android-13
 */

#include <linux/rcu_segcblist.h>

<<<<<<< HEAD
/*
 * Account for the fact that a previously dequeued callback turned out
 * to be marked as lazy.
 */
static inline void rcu_cblist_dequeued_lazy(struct rcu_cblist *rclp)
{
	rclp->len_lazy--;
}

void rcu_cblist_init(struct rcu_cblist *rclp);
=======
/* Return number of callbacks in the specified callback list. */
static inline long rcu_cblist_n_cbs(struct rcu_cblist *rclp)
{
	return READ_ONCE(rclp->len);
}

/* Return number of callbacks in segmented callback list by summing seglen. */
long rcu_segcblist_n_segment_cbs(struct rcu_segcblist *rsclp);

void rcu_cblist_init(struct rcu_cblist *rclp);
void rcu_cblist_enqueue(struct rcu_cblist *rclp, struct rcu_head *rhp);
void rcu_cblist_flush_enqueue(struct rcu_cblist *drclp,
			      struct rcu_cblist *srclp,
			      struct rcu_head *rhp);
>>>>>>> upstream/android-13
struct rcu_head *rcu_cblist_dequeue(struct rcu_cblist *rclp);

/*
 * Is the specified rcu_segcblist structure empty?
 *
 * But careful!  The fact that the ->head field is NULL does not
 * necessarily imply that there are no callbacks associated with
 * this structure.  When callbacks are being invoked, they are
 * removed as a group.  If callback invocation must be preempted,
 * the remaining callbacks will be added back to the list.  Either
 * way, the counts are updated later.
 *
 * So it is often the case that rcu_segcblist_n_cbs() should be used
 * instead.
 */
static inline bool rcu_segcblist_empty(struct rcu_segcblist *rsclp)
{
<<<<<<< HEAD
	return !rsclp->head;
=======
	return !READ_ONCE(rsclp->head);
>>>>>>> upstream/android-13
}

/* Return number of callbacks in segmented callback list. */
static inline long rcu_segcblist_n_cbs(struct rcu_segcblist *rsclp)
{
<<<<<<< HEAD
	return READ_ONCE(rsclp->len);
}

/* Return number of lazy callbacks in segmented callback list. */
static inline long rcu_segcblist_n_lazy_cbs(struct rcu_segcblist *rsclp)
{
	return rsclp->len_lazy;
}

/* Return number of lazy callbacks in segmented callback list. */
static inline long rcu_segcblist_n_nonlazy_cbs(struct rcu_segcblist *rsclp)
{
	return rsclp->len - rsclp->len_lazy;
=======
#ifdef CONFIG_RCU_NOCB_CPU
	return atomic_long_read(&rsclp->len);
#else
	return READ_ONCE(rsclp->len);
#endif
}

static inline void rcu_segcblist_set_flags(struct rcu_segcblist *rsclp,
					   int flags)
{
	WRITE_ONCE(rsclp->flags, rsclp->flags | flags);
}

static inline void rcu_segcblist_clear_flags(struct rcu_segcblist *rsclp,
					     int flags)
{
	WRITE_ONCE(rsclp->flags, rsclp->flags & ~flags);
}

static inline bool rcu_segcblist_test_flags(struct rcu_segcblist *rsclp,
					    int flags)
{
	return READ_ONCE(rsclp->flags) & flags;
>>>>>>> upstream/android-13
}

/*
 * Is the specified rcu_segcblist enabled, for example, not corresponding
<<<<<<< HEAD
 * to an offline or callback-offloaded CPU?
 */
static inline bool rcu_segcblist_is_enabled(struct rcu_segcblist *rsclp)
{
	return !!rsclp->tails[RCU_NEXT_TAIL];
=======
 * to an offline CPU?
 */
static inline bool rcu_segcblist_is_enabled(struct rcu_segcblist *rsclp)
{
	return rcu_segcblist_test_flags(rsclp, SEGCBLIST_ENABLED);
}

/* Is the specified rcu_segcblist offloaded, or is SEGCBLIST_SOFTIRQ_ONLY set? */
static inline bool rcu_segcblist_is_offloaded(struct rcu_segcblist *rsclp)
{
	if (IS_ENABLED(CONFIG_RCU_NOCB_CPU) &&
	    !rcu_segcblist_test_flags(rsclp, SEGCBLIST_SOFTIRQ_ONLY))
		return true;

	return false;
}

static inline bool rcu_segcblist_completely_offloaded(struct rcu_segcblist *rsclp)
{
	int flags = SEGCBLIST_KTHREAD_CB | SEGCBLIST_KTHREAD_GP | SEGCBLIST_OFFLOADED;

	if (IS_ENABLED(CONFIG_RCU_NOCB_CPU) && (rsclp->flags & flags) == flags)
		return true;

	return false;
>>>>>>> upstream/android-13
}

/*
 * Are all segments following the specified segment of the specified
 * rcu_segcblist structure empty of callbacks?  (The specified
 * segment might well contain callbacks.)
 */
static inline bool rcu_segcblist_restempty(struct rcu_segcblist *rsclp, int seg)
{
<<<<<<< HEAD
	return !*rsclp->tails[seg];
}

/*
 * Interim function to return rcu_segcblist head pointer.  Longer term, the
 * rcu_segcblist will be used more pervasively, removing the need for this
 * function.
 */
static inline struct rcu_head *rcu_segcblist_head(struct rcu_segcblist *rsclp)
{
	return rsclp->head;
}

/*
 * Interim function to return rcu_segcblist head pointer.  Longer term, the
 * rcu_segcblist will be used more pervasively, removing the need for this
 * function.
 */
static inline struct rcu_head **rcu_segcblist_tail(struct rcu_segcblist *rsclp)
{
	WARN_ON_ONCE(rcu_segcblist_empty(rsclp));
	return rsclp->tails[RCU_NEXT_TAIL];
}

void rcu_segcblist_init(struct rcu_segcblist *rsclp);
void rcu_segcblist_disable(struct rcu_segcblist *rsclp);
=======
	return !READ_ONCE(*READ_ONCE(rsclp->tails[seg]));
}

/*
 * Is the specified segment of the specified rcu_segcblist structure
 * empty of callbacks?
 */
static inline bool rcu_segcblist_segempty(struct rcu_segcblist *rsclp, int seg)
{
	if (seg == RCU_DONE_TAIL)
		return &rsclp->head == rsclp->tails[RCU_DONE_TAIL];
	return rsclp->tails[seg - 1] == rsclp->tails[seg];
}

void rcu_segcblist_inc_len(struct rcu_segcblist *rsclp);
void rcu_segcblist_add_len(struct rcu_segcblist *rsclp, long v);
void rcu_segcblist_init(struct rcu_segcblist *rsclp);
void rcu_segcblist_disable(struct rcu_segcblist *rsclp);
void rcu_segcblist_offload(struct rcu_segcblist *rsclp, bool offload);
>>>>>>> upstream/android-13
bool rcu_segcblist_ready_cbs(struct rcu_segcblist *rsclp);
bool rcu_segcblist_pend_cbs(struct rcu_segcblist *rsclp);
struct rcu_head *rcu_segcblist_first_cb(struct rcu_segcblist *rsclp);
struct rcu_head *rcu_segcblist_first_pend_cb(struct rcu_segcblist *rsclp);
<<<<<<< HEAD
void rcu_segcblist_enqueue(struct rcu_segcblist *rsclp,
			   struct rcu_head *rhp, bool lazy);
bool rcu_segcblist_entrain(struct rcu_segcblist *rsclp,
			   struct rcu_head *rhp, bool lazy);
void rcu_segcblist_extract_count(struct rcu_segcblist *rsclp,
				 struct rcu_cblist *rclp);
=======
bool rcu_segcblist_nextgp(struct rcu_segcblist *rsclp, unsigned long *lp);
void rcu_segcblist_enqueue(struct rcu_segcblist *rsclp,
			   struct rcu_head *rhp);
bool rcu_segcblist_entrain(struct rcu_segcblist *rsclp,
			   struct rcu_head *rhp);
>>>>>>> upstream/android-13
void rcu_segcblist_extract_done_cbs(struct rcu_segcblist *rsclp,
				    struct rcu_cblist *rclp);
void rcu_segcblist_extract_pend_cbs(struct rcu_segcblist *rsclp,
				    struct rcu_cblist *rclp);
void rcu_segcblist_insert_count(struct rcu_segcblist *rsclp,
				struct rcu_cblist *rclp);
void rcu_segcblist_insert_done_cbs(struct rcu_segcblist *rsclp,
				   struct rcu_cblist *rclp);
void rcu_segcblist_insert_pend_cbs(struct rcu_segcblist *rsclp,
				   struct rcu_cblist *rclp);
void rcu_segcblist_advance(struct rcu_segcblist *rsclp, unsigned long seq);
bool rcu_segcblist_accelerate(struct rcu_segcblist *rsclp, unsigned long seq);
void rcu_segcblist_merge(struct rcu_segcblist *dst_rsclp,
			 struct rcu_segcblist *src_rsclp);
