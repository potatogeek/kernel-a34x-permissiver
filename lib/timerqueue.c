<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> upstream/android-13
/*
 *  Generic Timer-queue
 *
 *  Manages a simple queue of timers, ordered by expiration time.
 *  Uses rbtrees for quick list adds and expiration.
 *
 *  NOTE: All of the following functions need to be serialized
 *  to avoid races. No locking is done by this library code.
<<<<<<< HEAD
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
=======
>>>>>>> upstream/android-13
 */

#include <linux/bug.h>
#include <linux/timerqueue.h>
#include <linux/rbtree.h>
#include <linux/export.h>

<<<<<<< HEAD
=======
#define __node_2_tq(_n) \
	rb_entry((_n), struct timerqueue_node, node)

static inline bool __timerqueue_less(struct rb_node *a, const struct rb_node *b)
{
	return __node_2_tq(a)->expires < __node_2_tq(b)->expires;
}

>>>>>>> upstream/android-13
/**
 * timerqueue_add - Adds timer to timerqueue.
 *
 * @head: head of timerqueue
 * @node: timer node to be added
 *
 * Adds the timer node to the timerqueue, sorted by the node's expires
 * value. Returns true if the newly added timer is the first expiring timer in
 * the queue.
 */
bool timerqueue_add(struct timerqueue_head *head, struct timerqueue_node *node)
{
<<<<<<< HEAD
	struct rb_node **p = &head->head.rb_node;
	struct rb_node *parent = NULL;
	struct timerqueue_node  *ptr;

	/* Make sure we don't add nodes that are already added */
	WARN_ON_ONCE(!RB_EMPTY_NODE(&node->node));

	while (*p) {
		parent = *p;
		ptr = rb_entry(parent, struct timerqueue_node, node);
		if (node->expires < ptr->expires)
			p = &(*p)->rb_left;
		else
			p = &(*p)->rb_right;
	}
	rb_link_node(&node->node, parent, p);
	rb_insert_color(&node->node, &head->head);

	if (!head->next || node->expires < head->next->expires) {
		head->next = node;
		return true;
	}
	return false;
=======
	/* Make sure we don't add nodes that are already added */
	WARN_ON_ONCE(!RB_EMPTY_NODE(&node->node));

	return rb_add_cached(&node->node, &head->rb_root, __timerqueue_less);
>>>>>>> upstream/android-13
}
EXPORT_SYMBOL_GPL(timerqueue_add);

/**
 * timerqueue_del - Removes a timer from the timerqueue.
 *
 * @head: head of timerqueue
 * @node: timer node to be removed
 *
 * Removes the timer node from the timerqueue. Returns true if the queue is
 * not empty after the remove.
 */
bool timerqueue_del(struct timerqueue_head *head, struct timerqueue_node *node)
{
	WARN_ON_ONCE(RB_EMPTY_NODE(&node->node));

<<<<<<< HEAD
	/* update next pointer */
	if (head->next == node) {
		struct rb_node *rbn = rb_next(&node->node);

		head->next = rb_entry_safe(rbn, struct timerqueue_node, node);
	}
	rb_erase(&node->node, &head->head);
	RB_CLEAR_NODE(&node->node);
	return head->next != NULL;
=======
	rb_erase_cached(&node->node, &head->rb_root);
	RB_CLEAR_NODE(&node->node);

	return !RB_EMPTY_ROOT(&head->rb_root.rb_root);
>>>>>>> upstream/android-13
}
EXPORT_SYMBOL_GPL(timerqueue_del);

/**
 * timerqueue_iterate_next - Returns the timer after the provided timer
 *
 * @node: Pointer to a timer.
 *
 * Provides the timer that is after the given node. This is used, when
 * necessary, to iterate through the list of timers in a timer list
 * without modifying the list.
 */
struct timerqueue_node *timerqueue_iterate_next(struct timerqueue_node *node)
{
	struct rb_node *next;

	if (!node)
		return NULL;
	next = rb_next(&node->node);
	if (!next)
		return NULL;
	return container_of(next, struct timerqueue_node, node);
}
EXPORT_SYMBOL_GPL(timerqueue_iterate_next);
