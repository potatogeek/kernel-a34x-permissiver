<<<<<<< HEAD
/*
 *  Copyright (C) 2009-2010, Lars-Peter Clausen <lars@metafoo.de>
 *  JZ4740 platform IRQ support
 *
 *  This program is free software; you can redistribute it and/or modify it
 *  under  the terms of the GNU General	 Public License as published by the
 *  Free Software Foundation;  either version 2 of the License, or (at your
 *  option) any later version.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  675 Mass Ave, Cambridge, MA 02139, USA.
 *
=======
// SPDX-License-Identifier: GPL-2.0-or-later
/*
 *  Copyright (C) 2009-2010, Lars-Peter Clausen <lars@metafoo.de>
 *  Ingenic XBurst platform IRQ support
>>>>>>> upstream/android-13
 */

#include <linux/errno.h>
#include <linux/init.h>
#include <linux/types.h>
#include <linux/interrupt.h>
#include <linux/ioport.h>
#include <linux/irqchip.h>
<<<<<<< HEAD
#include <linux/irqchip/ingenic.h>
=======
>>>>>>> upstream/android-13
#include <linux/of_address.h>
#include <linux/of_irq.h>
#include <linux/timex.h>
#include <linux/slab.h>
#include <linux/delay.h>

#include <asm/io.h>
<<<<<<< HEAD
#include <asm/mach-jz4740/irq.h>

struct ingenic_intc_data {
	void __iomem *base;
=======

struct ingenic_intc_data {
	void __iomem *base;
	struct irq_domain *domain;
>>>>>>> upstream/android-13
	unsigned num_chips;
};

#define JZ_REG_INTC_STATUS	0x00
#define JZ_REG_INTC_MASK	0x04
#define JZ_REG_INTC_SET_MASK	0x08
#define JZ_REG_INTC_CLEAR_MASK	0x0c
#define JZ_REG_INTC_PENDING	0x10
#define CHIP_SIZE		0x20

static irqreturn_t intc_cascade(int irq, void *data)
{
	struct ingenic_intc_data *intc = irq_get_handler_data(irq);
<<<<<<< HEAD
	uint32_t irq_reg;
	unsigned i;

	for (i = 0; i < intc->num_chips; i++) {
		irq_reg = readl(intc->base + (i * CHIP_SIZE) +
				JZ_REG_INTC_PENDING);
		if (!irq_reg)
			continue;

		generic_handle_irq(__fls(irq_reg) + (i * 32) + JZ4740_IRQ_BASE);
=======
	struct irq_domain *domain = intc->domain;
	struct irq_chip_generic *gc;
	uint32_t pending;
	unsigned i;

	for (i = 0; i < intc->num_chips; i++) {
		gc = irq_get_domain_generic_chip(domain, i * 32);

		pending = irq_reg_readl(gc, JZ_REG_INTC_PENDING);
		if (!pending)
			continue;

		while (pending) {
			int bit = __fls(pending);

			generic_handle_domain_irq(domain, bit + (i * 32));
			pending &= ~BIT(bit);
		}
>>>>>>> upstream/android-13
	}

	return IRQ_HANDLED;
}

<<<<<<< HEAD
static void intc_irq_set_mask(struct irq_chip_generic *gc, uint32_t mask)
{
	struct irq_chip_regs *regs = &gc->chip_types->regs;

	writel(mask, gc->reg_base + regs->enable);
	writel(~mask, gc->reg_base + regs->disable);
}

void ingenic_intc_irq_suspend(struct irq_data *data)
{
	struct irq_chip_generic *gc = irq_data_get_irq_chip_data(data);
	intc_irq_set_mask(gc, gc->wake_active);
}

void ingenic_intc_irq_resume(struct irq_data *data)
{
	struct irq_chip_generic *gc = irq_data_get_irq_chip_data(data);
	intc_irq_set_mask(gc, gc->mask_cache);
}

static struct irqaction intc_cascade_action = {
	.handler = intc_cascade,
	.name = "SoC intc cascade interrupt",
};

=======
>>>>>>> upstream/android-13
static int __init ingenic_intc_of_init(struct device_node *node,
				       unsigned num_chips)
{
	struct ingenic_intc_data *intc;
	struct irq_chip_generic *gc;
	struct irq_chip_type *ct;
	struct irq_domain *domain;
	int parent_irq, err = 0;
	unsigned i;

	intc = kzalloc(sizeof(*intc), GFP_KERNEL);
	if (!intc) {
		err = -ENOMEM;
		goto out_err;
	}

	parent_irq = irq_of_parse_and_map(node, 0);
	if (!parent_irq) {
		err = -EINVAL;
		goto out_free;
	}

	err = irq_set_handler_data(parent_irq, intc);
	if (err)
		goto out_unmap_irq;

	intc->num_chips = num_chips;
	intc->base = of_iomap(node, 0);
	if (!intc->base) {
		err = -ENODEV;
		goto out_unmap_irq;
	}

<<<<<<< HEAD
	domain = irq_domain_add_legacy(node, num_chips * 32,
				       JZ4740_IRQ_BASE, 0,
				       &irq_domain_simple_ops, NULL);
=======
	domain = irq_domain_add_linear(node, num_chips * 32,
				       &irq_generic_chip_ops, NULL);
>>>>>>> upstream/android-13
	if (!domain) {
		err = -ENOMEM;
		goto out_unmap_base;
	}

<<<<<<< HEAD
	for (i = 0; i < num_chips; i++) {
		/* Mask all irqs */
		writel(0xffffffff, intc->base + (i * CHIP_SIZE) +
		       JZ_REG_INTC_SET_MASK);

		gc = irq_alloc_generic_chip("INTC", 1,
					    JZ4740_IRQ_BASE + (i * 32),
					    intc->base + (i * CHIP_SIZE),
					    handle_level_irq);

		gc->wake_enabled = IRQ_MSK(32);
=======
	intc->domain = domain;

	err = irq_alloc_domain_generic_chips(domain, 32, 1, "INTC",
					     handle_level_irq, 0,
					     IRQ_NOPROBE | IRQ_LEVEL, 0);
	if (err)
		goto out_domain_remove;

	for (i = 0; i < num_chips; i++) {
		gc = irq_get_domain_generic_chip(domain, i * 32);

		gc->wake_enabled = IRQ_MSK(32);
		gc->reg_base = intc->base + (i * CHIP_SIZE);
>>>>>>> upstream/android-13

		ct = gc->chip_types;
		ct->regs.enable = JZ_REG_INTC_CLEAR_MASK;
		ct->regs.disable = JZ_REG_INTC_SET_MASK;
		ct->chip.irq_unmask = irq_gc_unmask_enable_reg;
		ct->chip.irq_mask = irq_gc_mask_disable_reg;
		ct->chip.irq_mask_ack = irq_gc_mask_disable_reg;
		ct->chip.irq_set_wake = irq_gc_set_wake;
<<<<<<< HEAD
		ct->chip.irq_suspend = ingenic_intc_irq_suspend;
		ct->chip.irq_resume = ingenic_intc_irq_resume;

		irq_setup_generic_chip(gc, IRQ_MSK(32), 0, 0,
				       IRQ_NOPROBE | IRQ_LEVEL);
	}

	setup_irq(parent_irq, &intc_cascade_action);
	return 0;

=======
		ct->chip.flags = IRQCHIP_MASK_ON_SUSPEND;

		/* Mask all irqs */
		irq_reg_writel(gc, IRQ_MSK(32), JZ_REG_INTC_SET_MASK);
	}

	if (request_irq(parent_irq, intc_cascade, IRQF_NO_SUSPEND,
			"SoC intc cascade interrupt", NULL))
		pr_err("Failed to register SoC intc cascade interrupt\n");
	return 0;

out_domain_remove:
	irq_domain_remove(domain);
>>>>>>> upstream/android-13
out_unmap_base:
	iounmap(intc->base);
out_unmap_irq:
	irq_dispose_mapping(parent_irq);
out_free:
	kfree(intc);
out_err:
	return err;
}

static int __init intc_1chip_of_init(struct device_node *node,
				     struct device_node *parent)
{
	return ingenic_intc_of_init(node, 1);
}
IRQCHIP_DECLARE(jz4740_intc, "ingenic,jz4740-intc", intc_1chip_of_init);
IRQCHIP_DECLARE(jz4725b_intc, "ingenic,jz4725b-intc", intc_1chip_of_init);

static int __init intc_2chip_of_init(struct device_node *node,
	struct device_node *parent)
{
	return ingenic_intc_of_init(node, 2);
}
<<<<<<< HEAD
=======
IRQCHIP_DECLARE(jz4760_intc, "ingenic,jz4760-intc", intc_2chip_of_init);
>>>>>>> upstream/android-13
IRQCHIP_DECLARE(jz4770_intc, "ingenic,jz4770-intc", intc_2chip_of_init);
IRQCHIP_DECLARE(jz4775_intc, "ingenic,jz4775-intc", intc_2chip_of_init);
IRQCHIP_DECLARE(jz4780_intc, "ingenic,jz4780-intc", intc_2chip_of_init);
