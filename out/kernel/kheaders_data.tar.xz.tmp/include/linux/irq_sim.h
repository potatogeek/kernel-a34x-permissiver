/* SPDX-License-Identifier: GPL-2.0+ */


#ifndef _LINUX_IRQ_SIM_H
#define _LINUX_IRQ_SIM_H

#include <linux/irq_work.h>
#include <linux/device.h>



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

#endif 
