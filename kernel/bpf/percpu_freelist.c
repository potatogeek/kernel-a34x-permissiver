<<<<<<< HEAD
/* Copyright (c) 2016 Facebook
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of version 2 of the GNU General Public
 * License as published by the Free Software Foundation.
=======
// SPDX-License-Identifier: GPL-2.0-only
/* Copyright (c) 2016 Facebook
>>>>>>> upstream/android-13
 */
#include "percpu_freelist.h"

int pcpu_freelist_init(struct pcpu_freelist *s)
{
	int cpu;

	s->freelist = alloc_percpu(struct pcpu_freelist_head);
	if (!s->freelist)
		return -ENOMEM;

	for_each_possible_cpu(cpu) {
		struct pcpu_freelist_head *head = per_cpu_ptr(s->freelist, cpu);

		raw_spin_lock_init(&head->lock);
		head->first = NULL;
	}
<<<<<<< HEAD
=======
	raw_spin_lock_init(&s->extralist.lock);
	s->extralist.first = NULL;
>>>>>>> upstream/android-13
	return 0;
}

void pcpu_freelist_destroy(struct pcpu_freelist *s)
{
	free_percpu(s->freelist);
}

<<<<<<< HEAD
=======
static inline void pcpu_freelist_push_node(struct pcpu_freelist_head *head,
					   struct pcpu_freelist_node *node)
{
	node->next = head->first;
	head->first = node;
}

>>>>>>> upstream/android-13
static inline void ___pcpu_freelist_push(struct pcpu_freelist_head *head,
					 struct pcpu_freelist_node *node)
{
	raw_spin_lock(&head->lock);
<<<<<<< HEAD
	node->next = head->first;
	head->first = node;
	raw_spin_unlock(&head->lock);
}

void __pcpu_freelist_push(struct pcpu_freelist *s,
			struct pcpu_freelist_node *node)
{
	struct pcpu_freelist_head *head = this_cpu_ptr(s->freelist);

	___pcpu_freelist_push(head, node);
=======
	pcpu_freelist_push_node(head, node);
	raw_spin_unlock(&head->lock);
}

static inline bool pcpu_freelist_try_push_extra(struct pcpu_freelist *s,
						struct pcpu_freelist_node *node)
{
	if (!raw_spin_trylock(&s->extralist.lock))
		return false;

	pcpu_freelist_push_node(&s->extralist, node);
	raw_spin_unlock(&s->extralist.lock);
	return true;
}

static inline void ___pcpu_freelist_push_nmi(struct pcpu_freelist *s,
					     struct pcpu_freelist_node *node)
{
	int cpu, orig_cpu;

	orig_cpu = cpu = raw_smp_processor_id();
	while (1) {
		struct pcpu_freelist_head *head;

		head = per_cpu_ptr(s->freelist, cpu);
		if (raw_spin_trylock(&head->lock)) {
			pcpu_freelist_push_node(head, node);
			raw_spin_unlock(&head->lock);
			return;
		}
		cpu = cpumask_next(cpu, cpu_possible_mask);
		if (cpu >= nr_cpu_ids)
			cpu = 0;

		/* cannot lock any per cpu lock, try extralist */
		if (cpu == orig_cpu &&
		    pcpu_freelist_try_push_extra(s, node))
			return;
	}
}

void __pcpu_freelist_push(struct pcpu_freelist *s,
			struct pcpu_freelist_node *node)
{
	if (in_nmi())
		___pcpu_freelist_push_nmi(s, node);
	else
		___pcpu_freelist_push(this_cpu_ptr(s->freelist), node);
>>>>>>> upstream/android-13
}

void pcpu_freelist_push(struct pcpu_freelist *s,
			struct pcpu_freelist_node *node)
{
	unsigned long flags;

	local_irq_save(flags);
	__pcpu_freelist_push(s, node);
	local_irq_restore(flags);
}

void pcpu_freelist_populate(struct pcpu_freelist *s, void *buf, u32 elem_size,
			    u32 nr_elems)
{
	struct pcpu_freelist_head *head;
<<<<<<< HEAD
	unsigned long flags;
=======
>>>>>>> upstream/android-13
	int i, cpu, pcpu_entries;

	pcpu_entries = nr_elems / num_possible_cpus() + 1;
	i = 0;

<<<<<<< HEAD
	/* disable irq to workaround lockdep false positive
	 * in bpf usage pcpu_freelist_populate() will never race
	 * with pcpu_freelist_push()
	 */
	local_irq_save(flags);
	for_each_possible_cpu(cpu) {
again:
		head = per_cpu_ptr(s->freelist, cpu);
		___pcpu_freelist_push(head, buf);
=======
	for_each_possible_cpu(cpu) {
again:
		head = per_cpu_ptr(s->freelist, cpu);
		/* No locking required as this is not visible yet. */
		pcpu_freelist_push_node(head, buf);
>>>>>>> upstream/android-13
		i++;
		buf += elem_size;
		if (i == nr_elems)
			break;
		if (i % pcpu_entries)
			goto again;
	}
<<<<<<< HEAD
	local_irq_restore(flags);
}

struct pcpu_freelist_node *__pcpu_freelist_pop(struct pcpu_freelist *s)
=======
}

static struct pcpu_freelist_node *___pcpu_freelist_pop(struct pcpu_freelist *s)
>>>>>>> upstream/android-13
{
	struct pcpu_freelist_head *head;
	struct pcpu_freelist_node *node;
	int orig_cpu, cpu;

	orig_cpu = cpu = raw_smp_processor_id();
	while (1) {
		head = per_cpu_ptr(s->freelist, cpu);
		raw_spin_lock(&head->lock);
		node = head->first;
		if (node) {
			head->first = node->next;
			raw_spin_unlock(&head->lock);
			return node;
		}
		raw_spin_unlock(&head->lock);
		cpu = cpumask_next(cpu, cpu_possible_mask);
		if (cpu >= nr_cpu_ids)
			cpu = 0;
		if (cpu == orig_cpu)
<<<<<<< HEAD
			return NULL;
	}
=======
			break;
	}

	/* per cpu lists are all empty, try extralist */
	raw_spin_lock(&s->extralist.lock);
	node = s->extralist.first;
	if (node)
		s->extralist.first = node->next;
	raw_spin_unlock(&s->extralist.lock);
	return node;
}

static struct pcpu_freelist_node *
___pcpu_freelist_pop_nmi(struct pcpu_freelist *s)
{
	struct pcpu_freelist_head *head;
	struct pcpu_freelist_node *node;
	int orig_cpu, cpu;

	orig_cpu = cpu = raw_smp_processor_id();
	while (1) {
		head = per_cpu_ptr(s->freelist, cpu);
		if (raw_spin_trylock(&head->lock)) {
			node = head->first;
			if (node) {
				head->first = node->next;
				raw_spin_unlock(&head->lock);
				return node;
			}
			raw_spin_unlock(&head->lock);
		}
		cpu = cpumask_next(cpu, cpu_possible_mask);
		if (cpu >= nr_cpu_ids)
			cpu = 0;
		if (cpu == orig_cpu)
			break;
	}

	/* cannot pop from per cpu lists, try extralist */
	if (!raw_spin_trylock(&s->extralist.lock))
		return NULL;
	node = s->extralist.first;
	if (node)
		s->extralist.first = node->next;
	raw_spin_unlock(&s->extralist.lock);
	return node;
}

struct pcpu_freelist_node *__pcpu_freelist_pop(struct pcpu_freelist *s)
{
	if (in_nmi())
		return ___pcpu_freelist_pop_nmi(s);
	return ___pcpu_freelist_pop(s);
>>>>>>> upstream/android-13
}

struct pcpu_freelist_node *pcpu_freelist_pop(struct pcpu_freelist *s)
{
	struct pcpu_freelist_node *ret;
	unsigned long flags;

	local_irq_save(flags);
	ret = __pcpu_freelist_pop(s);
	local_irq_restore(flags);
	return ret;
}
