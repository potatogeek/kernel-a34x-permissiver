<<<<<<< HEAD
/*
 * Based on strlist.c by:
 * (c) 2009 Arnaldo Carvalho de Melo <acme@redhat.com>
 *
 * Licensed under the GPLv2.
=======
// SPDX-License-Identifier: GPL-2.0-only
/*
 * Based on strlist.c by:
 * (c) 2009 Arnaldo Carvalho de Melo <acme@redhat.com>
>>>>>>> upstream/android-13
 */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

#include "rblist.h"

int rblist__add_node(struct rblist *rblist, const void *new_entry)
{
<<<<<<< HEAD
	struct rb_node **p = &rblist->entries.rb_node;
	struct rb_node *parent = NULL, *new_node;
=======
	struct rb_node **p = &rblist->entries.rb_root.rb_node;
	struct rb_node *parent = NULL, *new_node;
	bool leftmost = true;
>>>>>>> upstream/android-13

	while (*p != NULL) {
		int rc;

		parent = *p;

		rc = rblist->node_cmp(parent, new_entry);
		if (rc > 0)
			p = &(*p)->rb_left;
<<<<<<< HEAD
		else if (rc < 0)
			p = &(*p)->rb_right;
=======
		else if (rc < 0) {
			p = &(*p)->rb_right;
			leftmost = false;
		}
>>>>>>> upstream/android-13
		else
			return -EEXIST;
	}

	new_node = rblist->node_new(rblist, new_entry);
	if (new_node == NULL)
		return -ENOMEM;

	rb_link_node(new_node, parent, p);
<<<<<<< HEAD
	rb_insert_color(new_node, &rblist->entries);
=======
	rb_insert_color_cached(new_node, &rblist->entries, leftmost);
>>>>>>> upstream/android-13
	++rblist->nr_entries;

	return 0;
}

void rblist__remove_node(struct rblist *rblist, struct rb_node *rb_node)
{
<<<<<<< HEAD
	rb_erase(rb_node, &rblist->entries);
=======
	rb_erase_cached(rb_node, &rblist->entries);
>>>>>>> upstream/android-13
	--rblist->nr_entries;
	rblist->node_delete(rblist, rb_node);
}

static struct rb_node *__rblist__findnew(struct rblist *rblist,
					 const void *entry,
					 bool create)
{
<<<<<<< HEAD
	struct rb_node **p = &rblist->entries.rb_node;
	struct rb_node *parent = NULL, *new_node = NULL;
=======
	struct rb_node **p = &rblist->entries.rb_root.rb_node;
	struct rb_node *parent = NULL, *new_node = NULL;
	bool leftmost = true;
>>>>>>> upstream/android-13

	while (*p != NULL) {
		int rc;

		parent = *p;

		rc = rblist->node_cmp(parent, entry);
		if (rc > 0)
			p = &(*p)->rb_left;
<<<<<<< HEAD
		else if (rc < 0)
			p = &(*p)->rb_right;
=======
		else if (rc < 0) {
			p = &(*p)->rb_right;
			leftmost = false;
		}
>>>>>>> upstream/android-13
		else
			return parent;
	}

	if (create) {
		new_node = rblist->node_new(rblist, entry);
		if (new_node) {
			rb_link_node(new_node, parent, p);
<<<<<<< HEAD
			rb_insert_color(new_node, &rblist->entries);
=======
			rb_insert_color_cached(new_node,
					       &rblist->entries, leftmost);
>>>>>>> upstream/android-13
			++rblist->nr_entries;
		}
	}

	return new_node;
}

struct rb_node *rblist__find(struct rblist *rblist, const void *entry)
{
	return __rblist__findnew(rblist, entry, false);
}

struct rb_node *rblist__findnew(struct rblist *rblist, const void *entry)
{
	return __rblist__findnew(rblist, entry, true);
}

void rblist__init(struct rblist *rblist)
{
	if (rblist != NULL) {
<<<<<<< HEAD
		rblist->entries	 = RB_ROOT;
=======
		rblist->entries	 = RB_ROOT_CACHED;
>>>>>>> upstream/android-13
		rblist->nr_entries = 0;
	}

	return;
}

void rblist__exit(struct rblist *rblist)
{
<<<<<<< HEAD
	struct rb_node *pos, *next = rb_first(&rblist->entries);
=======
	struct rb_node *pos, *next = rb_first_cached(&rblist->entries);
>>>>>>> upstream/android-13

	while (next) {
		pos = next;
		next = rb_next(pos);
		rblist__remove_node(rblist, pos);
	}
}

void rblist__delete(struct rblist *rblist)
{
	if (rblist != NULL) {
		rblist__exit(rblist);
		free(rblist);
	}
}

struct rb_node *rblist__entry(const struct rblist *rblist, unsigned int idx)
{
	struct rb_node *node;

<<<<<<< HEAD
	for (node = rb_first(&rblist->entries); node; node = rb_next(node)) {
=======
	for (node = rb_first_cached(&rblist->entries); node;
	     node = rb_next(node)) {
>>>>>>> upstream/android-13
		if (!idx--)
			return node;
	}

	return NULL;
}
