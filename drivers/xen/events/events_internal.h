<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-or-later */
>>>>>>> upstream/android-13
/*
 * Xen Event Channels (internal header)
 *
 * Copyright (C) 2013 Citrix Systems R&D Ltd.
<<<<<<< HEAD
 *
 * This source code is licensed under the GNU General Public License,
 * Version 2 or later.  See the file COPYING for more details.
=======
>>>>>>> upstream/android-13
 */
#ifndef __EVENTS_INTERNAL_H__
#define __EVENTS_INTERNAL_H__

<<<<<<< HEAD
/* Interrupt types. */
enum xen_irq_type {
	IRQT_UNBOUND = 0,
	IRQT_PIRQ,
	IRQT_VIRQ,
	IRQT_IPI,
	IRQT_EVTCHN
};

/*
 * Packed IRQ information:
 * type - enum xen_irq_type
 * event channel - irq->event channel mapping
 * cpu - cpu this event channel is bound to
 * index - type-specific information:
 *    PIRQ - vector, with MSB being "needs EIO", or physical IRQ of the HVM
 *           guest, or GSI (real passthrough IRQ) of the device.
 *    VIRQ - virq number
 *    IPI - IPI vector
 *    EVTCHN -
 */
struct irq_info {
	struct list_head list;
	struct list_head eoi_list;
	short refcnt;
	short spurious_cnt;
	short type;		/* type */
	u8 mask_reason;		/* Why is event channel masked */
#define EVT_MASK_REASON_EXPLICIT	0x01
#define EVT_MASK_REASON_TEMPORARY	0x02
#define EVT_MASK_REASON_EOI_PENDING	0x04
	u8 is_active;		/* Is event just being handled? */
	unsigned irq;
	unsigned int evtchn;	/* event channel */
	unsigned short cpu;	/* cpu bound */
	unsigned short eoi_cpu;	/* EOI must happen on this cpu */
	unsigned int irq_epoch;	/* If eoi_cpu valid: irq_epoch of event */
	u64 eoi_time;		/* Time in jiffies when to EOI. */
	raw_spinlock_t lock;

	union {
		unsigned short virq;
		enum ipi_vector ipi;
		struct {
			unsigned short pirq;
			unsigned short gsi;
			unsigned char vector;
			unsigned char flags;
			uint16_t domid;
		} pirq;
	} u;
};

#define PIRQ_NEEDS_EOI	(1 << 0)
#define PIRQ_SHAREABLE	(1 << 1)
#define PIRQ_MSI_GROUP	(1 << 2)

=======
>>>>>>> upstream/android-13
struct evtchn_loop_ctrl;

struct evtchn_ops {
	unsigned (*max_channels)(void);
	unsigned (*nr_channels)(void);

<<<<<<< HEAD
	int (*setup)(struct irq_info *info);
	void (*remove)(evtchn_port_t port, unsigned int cpu);
	void (*bind_to_cpu)(struct irq_info *info, unsigned cpu);

	void (*clear_pending)(unsigned port);
	void (*set_pending)(unsigned port);
	bool (*is_pending)(unsigned port);
	void (*mask)(unsigned port);
	void (*unmask)(unsigned port);
=======
	int (*setup)(evtchn_port_t port);
	void (*remove)(evtchn_port_t port, unsigned int cpu);
	void (*bind_to_cpu)(evtchn_port_t evtchn, unsigned int cpu,
			    unsigned int old_cpu);

	void (*clear_pending)(evtchn_port_t port);
	void (*set_pending)(evtchn_port_t port);
	bool (*is_pending)(evtchn_port_t port);
	void (*mask)(evtchn_port_t port);
	void (*unmask)(evtchn_port_t port);
>>>>>>> upstream/android-13

	void (*handle_events)(unsigned cpu, struct evtchn_loop_ctrl *ctrl);
	void (*resume)(void);

	int (*percpu_init)(unsigned int cpu);
	int (*percpu_deinit)(unsigned int cpu);
};

extern const struct evtchn_ops *evtchn_ops;

<<<<<<< HEAD
extern int **evtchn_to_irq;
int get_evtchn_to_irq(unsigned int evtchn);
void handle_irq_for_port(evtchn_port_t port, struct evtchn_loop_ctrl *ctrl);

struct irq_info *info_for_irq(unsigned irq);
unsigned cpu_from_irq(unsigned irq);
unsigned cpu_from_evtchn(unsigned int evtchn);
=======
int get_evtchn_to_irq(evtchn_port_t evtchn);
void handle_irq_for_port(evtchn_port_t port, struct evtchn_loop_ctrl *ctrl);

unsigned int cpu_from_evtchn(evtchn_port_t evtchn);
>>>>>>> upstream/android-13

static inline unsigned xen_evtchn_max_channels(void)
{
	return evtchn_ops->max_channels();
}

/*
 * Do any ABI specific setup for a bound event channel before it can
 * be unmasked and used.
 */
<<<<<<< HEAD
static inline int xen_evtchn_port_setup(struct irq_info *info)
{
	if (evtchn_ops->setup)
		return evtchn_ops->setup(info);
=======
static inline int xen_evtchn_port_setup(evtchn_port_t evtchn)
{
	if (evtchn_ops->setup)
		return evtchn_ops->setup(evtchn);
>>>>>>> upstream/android-13
	return 0;
}

static inline void xen_evtchn_port_remove(evtchn_port_t evtchn,
					  unsigned int cpu)
{
	if (evtchn_ops->remove)
		evtchn_ops->remove(evtchn, cpu);
}

<<<<<<< HEAD
static inline void xen_evtchn_port_bind_to_cpu(struct irq_info *info,
					       unsigned cpu)
{
	evtchn_ops->bind_to_cpu(info, cpu);
}

static inline void clear_evtchn(unsigned port)
=======
static inline void xen_evtchn_port_bind_to_cpu(evtchn_port_t evtchn,
					       unsigned int cpu,
					       unsigned int old_cpu)
{
	evtchn_ops->bind_to_cpu(evtchn, cpu, old_cpu);
}

static inline void clear_evtchn(evtchn_port_t port)
>>>>>>> upstream/android-13
{
	evtchn_ops->clear_pending(port);
}

<<<<<<< HEAD
static inline void set_evtchn(unsigned port)
=======
static inline void set_evtchn(evtchn_port_t port)
>>>>>>> upstream/android-13
{
	evtchn_ops->set_pending(port);
}

<<<<<<< HEAD
static inline bool test_evtchn(unsigned port)
=======
static inline bool test_evtchn(evtchn_port_t port)
>>>>>>> upstream/android-13
{
	return evtchn_ops->is_pending(port);
}

<<<<<<< HEAD
static inline void mask_evtchn(unsigned port)
=======
static inline void mask_evtchn(evtchn_port_t port)
>>>>>>> upstream/android-13
{
	return evtchn_ops->mask(port);
}

<<<<<<< HEAD
static inline void unmask_evtchn(unsigned port)
=======
static inline void unmask_evtchn(evtchn_port_t port)
>>>>>>> upstream/android-13
{
	return evtchn_ops->unmask(port);
}

static inline void xen_evtchn_handle_events(unsigned cpu,
					    struct evtchn_loop_ctrl *ctrl)
{
	return evtchn_ops->handle_events(cpu, ctrl);
}

static inline void xen_evtchn_resume(void)
{
	if (evtchn_ops->resume)
		evtchn_ops->resume();
}

void xen_evtchn_2l_init(void);
int xen_evtchn_fifo_init(void);

#endif /* #ifndef __EVENTS_INTERNAL_H__ */
