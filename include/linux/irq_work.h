/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_IRQ_WORK_H
#define _LINUX_IRQ_WORK_H

<<<<<<< HEAD
#include <linux/llist.h>
=======
#include <linux/smp_types.h>
>>>>>>> upstream/android-13

/*
 * An entry can be in one of four states:
 *
 * free	     NULL, 0 -> {claimed}       : free to be used
 * claimed   NULL, 3 -> {pending}       : claimed to be enqueued
 * pending   next, 3 -> {busy}          : queued, pending callback
 * busy      NULL, 2 -> {free, claimed} : callback in progress, can be claimed
 */

<<<<<<< HEAD
#define IRQ_WORK_PENDING	BIT(0)
#define IRQ_WORK_BUSY		BIT(1)

/* Doesn't want IPI, wait for tick: */
#define IRQ_WORK_LAZY		BIT(2)

#define IRQ_WORK_CLAIMED	(IRQ_WORK_PENDING | IRQ_WORK_BUSY)

struct irq_work {
	unsigned long flags;
	struct llist_node llnode;
	void (*func)(struct irq_work *);
};

static inline
void init_irq_work(struct irq_work *work, void (*func)(struct irq_work *))
{
	work->flags = 0;
	work->func = func;
}

#define DEFINE_IRQ_WORK(name, _f) struct irq_work name = { .func = (_f), }
=======
struct irq_work {
	struct __call_single_node node;
	void (*func)(struct irq_work *);
};

#define __IRQ_WORK_INIT(_func, _flags) (struct irq_work){	\
	.node = { .u_flags = (_flags), },			\
	.func = (_func),					\
}

#define IRQ_WORK_INIT(_func) __IRQ_WORK_INIT(_func, 0)
#define IRQ_WORK_INIT_LAZY(_func) __IRQ_WORK_INIT(_func, IRQ_WORK_LAZY)
#define IRQ_WORK_INIT_HARD(_func) __IRQ_WORK_INIT(_func, IRQ_WORK_HARD_IRQ)

#define DEFINE_IRQ_WORK(name, _f)				\
	struct irq_work name = IRQ_WORK_INIT(_f)

static inline
void init_irq_work(struct irq_work *work, void (*func)(struct irq_work *))
{
	*work = IRQ_WORK_INIT(func);
}

static inline bool irq_work_is_pending(struct irq_work *work)
{
	return atomic_read(&work->node.a_flags) & IRQ_WORK_PENDING;
}

static inline bool irq_work_is_busy(struct irq_work *work)
{
	return atomic_read(&work->node.a_flags) & IRQ_WORK_BUSY;
}
>>>>>>> upstream/android-13

bool irq_work_queue(struct irq_work *work);
bool irq_work_queue_on(struct irq_work *work, int cpu);

void irq_work_tick(void);
void irq_work_sync(struct irq_work *work);

#ifdef CONFIG_IRQ_WORK
#include <asm/irq_work.h>

void irq_work_run(void);
bool irq_work_needs_cpu(void);
<<<<<<< HEAD
#else
static inline bool irq_work_needs_cpu(void) { return false; }
static inline void irq_work_run(void) { }
=======
void irq_work_single(void *arg);
#else
static inline bool irq_work_needs_cpu(void) { return false; }
static inline void irq_work_run(void) { }
static inline void irq_work_single(void *arg) { }
>>>>>>> upstream/android-13
#endif

#endif /* _LINUX_IRQ_WORK_H */
