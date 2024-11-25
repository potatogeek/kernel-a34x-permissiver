/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * Copyright (C) 2017-2018 Bartosz Golaszewski <brgl@bgdev.pl>
<<<<<<< HEAD
=======
 * Copyright (C) 2020 Bartosz Golaszewski <bgolaszewski@baylibre.com>
>>>>>>> upstream/android-13
 */

#ifndef _LINUX_IRQ_SIM_H
#define _LINUX_IRQ_SIM_H

<<<<<<< HEAD
#include <linux/irq_work.h>
#include <linux/device.h>
=======
#include <linux/device.h>
#include <linux/fwnode.h>
#include <linux/irqdomain.h>
>>>>>>> upstream/android-13

/*
 * Provides a framework for allocating simulated interrupts which can be
 * requested like normal irqs and enqueued from process context.
 */

<<<<<<< HEAD
struct irq_sim_work_ctx {
	struct irq_work		work;
	int			irq;
};

struct irq_sim_irq_ctx {
	int			irqnum;
	bool			enabled;
};

struct irq_sim {
	struct irq_sim_work_ctx	work_ctx;
	int			irq_base;
	unsigned int		irq_count;
	struct irq_sim_irq_ctx	*irqs;
};

int irq_sim_init(struct irq_sim *sim, unsigned int num_irqs);
int devm_irq_sim_init(struct device *dev, struct irq_sim *sim,
		      unsigned int num_irqs);
void irq_sim_fini(struct irq_sim *sim);
void irq_sim_fire(struct irq_sim *sim, unsigned int offset);
int irq_sim_irqnum(struct irq_sim *sim, unsigned int offset);
=======
struct irq_domain *irq_domain_create_sim(struct fwnode_handle *fwnode,
					 unsigned int num_irqs);
struct irq_domain *devm_irq_domain_create_sim(struct device *dev,
					      struct fwnode_handle *fwnode,
					      unsigned int num_irqs);
void irq_domain_remove_sim(struct irq_domain *domain);
>>>>>>> upstream/android-13

#endif /* _LINUX_IRQ_SIM_H */
