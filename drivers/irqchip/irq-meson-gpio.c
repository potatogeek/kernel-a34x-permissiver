<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> upstream/android-13
/*
 * Copyright (c) 2015 Endless Mobile, Inc.
 * Author: Carlo Caione <carlo@endlessm.com>
 * Copyright (c) 2016 BayLibre, SAS.
 * Author: Jerome Brunet <jbrunet@baylibre.com>
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of version 2 of the GNU General Public License as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see <http://www.gnu.org/licenses/>.
 * The full GNU General Public License is included in this distribution
 * in the file called COPYING.
=======
>>>>>>> upstream/android-13
 */

#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linux/io.h>
#include <linux/module.h>
#include <linux/irq.h>
#include <linux/irqdomain.h>
#include <linux/irqchip.h>
#include <linux/of.h>
#include <linux/of_address.h>
<<<<<<< HEAD
=======
#include <linux/of_irq.h>
>>>>>>> upstream/android-13

#define NUM_CHANNEL 8
#define MAX_INPUT_MUX 256

#define REG_EDGE_POL	0x00
#define REG_PIN_03_SEL	0x04
#define REG_PIN_47_SEL	0x08
#define REG_FILTER_SEL	0x0c

<<<<<<< HEAD
#define REG_EDGE_POL_MASK(x)	(BIT(x) | BIT(16 + (x)))
#define REG_EDGE_POL_EDGE(x)	BIT(x)
#define REG_EDGE_POL_LOW(x)	BIT(16 + (x))
#define REG_PIN_SEL_SHIFT(x)	(((x) % 4) * 8)
#define REG_FILTER_SEL_SHIFT(x)	((x) * 4)

struct meson_gpio_irq_params {
	unsigned int nr_hwirq;
};

static const struct meson_gpio_irq_params meson8_params = {
	.nr_hwirq = 134,
};

static const struct meson_gpio_irq_params meson8b_params = {
	.nr_hwirq = 119,
};

static const struct meson_gpio_irq_params gxbb_params = {
	.nr_hwirq = 133,
};

static const struct meson_gpio_irq_params gxl_params = {
	.nr_hwirq = 110,
};

static const struct meson_gpio_irq_params axg_params = {
	.nr_hwirq = 100,
=======
/* use for A1 like chips */
#define REG_PIN_A1_SEL	0x04

/*
 * Note: The S905X3 datasheet reports that BOTH_EDGE is controlled by
 * bits 24 to 31. Tests on the actual HW show that these bits are
 * stuck at 0. Bits 8 to 15 are responsive and have the expected
 * effect.
 */
#define REG_EDGE_POL_EDGE(params, x)	BIT((params)->edge_single_offset + (x))
#define REG_EDGE_POL_LOW(params, x)	BIT((params)->pol_low_offset + (x))
#define REG_BOTH_EDGE(params, x)	BIT((params)->edge_both_offset + (x))
#define REG_EDGE_POL_MASK(params, x)    (	\
		REG_EDGE_POL_EDGE(params, x) |	\
		REG_EDGE_POL_LOW(params, x)  |	\
		REG_BOTH_EDGE(params, x))
#define REG_PIN_SEL_SHIFT(x)	(((x) % 4) * 8)
#define REG_FILTER_SEL_SHIFT(x)	((x) * 4)

struct meson_gpio_irq_controller;
static void meson8_gpio_irq_sel_pin(struct meson_gpio_irq_controller *ctl,
				    unsigned int channel, unsigned long hwirq);
static void meson_gpio_irq_init_dummy(struct meson_gpio_irq_controller *ctl);
static void meson_a1_gpio_irq_sel_pin(struct meson_gpio_irq_controller *ctl,
				      unsigned int channel,
				      unsigned long hwirq);
static void meson_a1_gpio_irq_init(struct meson_gpio_irq_controller *ctl);

struct irq_ctl_ops {
	void (*gpio_irq_sel_pin)(struct meson_gpio_irq_controller *ctl,
				 unsigned int channel, unsigned long hwirq);
	void (*gpio_irq_init)(struct meson_gpio_irq_controller *ctl);
};

struct meson_gpio_irq_params {
	unsigned int nr_hwirq;
	bool support_edge_both;
	unsigned int edge_both_offset;
	unsigned int edge_single_offset;
	unsigned int pol_low_offset;
	unsigned int pin_sel_mask;
	struct irq_ctl_ops ops;
};

#define INIT_MESON_COMMON(irqs, init, sel)			\
	.nr_hwirq = irqs,					\
	.ops = {						\
		.gpio_irq_init = init,				\
		.gpio_irq_sel_pin = sel,			\
	},

#define INIT_MESON8_COMMON_DATA(irqs)				\
	INIT_MESON_COMMON(irqs, meson_gpio_irq_init_dummy,	\
			  meson8_gpio_irq_sel_pin)		\
	.edge_single_offset = 0,				\
	.pol_low_offset = 16,					\
	.pin_sel_mask = 0xff,					\

#define INIT_MESON_A1_COMMON_DATA(irqs)				\
	INIT_MESON_COMMON(irqs, meson_a1_gpio_irq_init,		\
			  meson_a1_gpio_irq_sel_pin)		\
	.support_edge_both = true,				\
	.edge_both_offset = 16,					\
	.edge_single_offset = 8,				\
	.pol_low_offset = 0,					\
	.pin_sel_mask = 0x7f,					\

static const struct meson_gpio_irq_params meson8_params = {
	INIT_MESON8_COMMON_DATA(134)
};

static const struct meson_gpio_irq_params meson8b_params = {
	INIT_MESON8_COMMON_DATA(119)
};

static const struct meson_gpio_irq_params gxbb_params = {
	INIT_MESON8_COMMON_DATA(133)
};

static const struct meson_gpio_irq_params gxl_params = {
	INIT_MESON8_COMMON_DATA(110)
};

static const struct meson_gpio_irq_params axg_params = {
	INIT_MESON8_COMMON_DATA(100)
};

static const struct meson_gpio_irq_params sm1_params = {
	INIT_MESON8_COMMON_DATA(100)
	.support_edge_both = true,
	.edge_both_offset = 8,
};

static const struct meson_gpio_irq_params a1_params = {
	INIT_MESON_A1_COMMON_DATA(62)
>>>>>>> upstream/android-13
};

static const struct of_device_id meson_irq_gpio_matches[] = {
	{ .compatible = "amlogic,meson8-gpio-intc", .data = &meson8_params },
	{ .compatible = "amlogic,meson8b-gpio-intc", .data = &meson8b_params },
	{ .compatible = "amlogic,meson-gxbb-gpio-intc", .data = &gxbb_params },
	{ .compatible = "amlogic,meson-gxl-gpio-intc", .data = &gxl_params },
	{ .compatible = "amlogic,meson-axg-gpio-intc", .data = &axg_params },
	{ .compatible = "amlogic,meson-g12a-gpio-intc", .data = &axg_params },
<<<<<<< HEAD
=======
	{ .compatible = "amlogic,meson-sm1-gpio-intc", .data = &sm1_params },
	{ .compatible = "amlogic,meson-a1-gpio-intc", .data = &a1_params },
>>>>>>> upstream/android-13
	{ }
};

struct meson_gpio_irq_controller {
<<<<<<< HEAD
	unsigned int nr_hwirq;
	void __iomem *base;
=======
	const struct meson_gpio_irq_params *params;
	void __iomem *base;
	struct irq_domain *domain;
>>>>>>> upstream/android-13
	u32 channel_irqs[NUM_CHANNEL];
	DECLARE_BITMAP(channel_map, NUM_CHANNEL);
	spinlock_t lock;
};

static void meson_gpio_irq_update_bits(struct meson_gpio_irq_controller *ctl,
				       unsigned int reg, u32 mask, u32 val)
{
<<<<<<< HEAD
	u32 tmp;

=======
	unsigned long flags;
	u32 tmp;

	spin_lock_irqsave(&ctl->lock, flags);

>>>>>>> upstream/android-13
	tmp = readl_relaxed(ctl->base + reg);
	tmp &= ~mask;
	tmp |= val;
	writel_relaxed(tmp, ctl->base + reg);
<<<<<<< HEAD
}

static unsigned int meson_gpio_irq_channel_to_reg(unsigned int channel)
{
	return (channel < 4) ? REG_PIN_03_SEL : REG_PIN_47_SEL;
=======

	spin_unlock_irqrestore(&ctl->lock, flags);
}

static void meson_gpio_irq_init_dummy(struct meson_gpio_irq_controller *ctl)
{
}

static void meson8_gpio_irq_sel_pin(struct meson_gpio_irq_controller *ctl,
				    unsigned int channel, unsigned long hwirq)
{
	unsigned int reg_offset;
	unsigned int bit_offset;

	reg_offset = (channel < 4) ? REG_PIN_03_SEL : REG_PIN_47_SEL;
	bit_offset = REG_PIN_SEL_SHIFT(channel);

	meson_gpio_irq_update_bits(ctl, reg_offset,
				   ctl->params->pin_sel_mask << bit_offset,
				   hwirq << bit_offset);
}

static void meson_a1_gpio_irq_sel_pin(struct meson_gpio_irq_controller *ctl,
				      unsigned int channel,
				      unsigned long hwirq)
{
	unsigned int reg_offset;
	unsigned int bit_offset;

	bit_offset = ((channel % 2) == 0) ? 0 : 16;
	reg_offset = REG_PIN_A1_SEL + ((channel / 2) << 2);

	meson_gpio_irq_update_bits(ctl, reg_offset,
				   ctl->params->pin_sel_mask << bit_offset,
				   hwirq << bit_offset);
}

/* For a1 or later chips like a1 there is a switch to enable/disable irq */
static void meson_a1_gpio_irq_init(struct meson_gpio_irq_controller *ctl)
{
	meson_gpio_irq_update_bits(ctl, REG_EDGE_POL, BIT(31), BIT(31));
>>>>>>> upstream/android-13
}

static int
meson_gpio_irq_request_channel(struct meson_gpio_irq_controller *ctl,
			       unsigned long  hwirq,
			       u32 **channel_hwirq)
{
<<<<<<< HEAD
	unsigned int reg, idx;

	spin_lock(&ctl->lock);
=======
	unsigned long flags;
	unsigned int idx;

	spin_lock_irqsave(&ctl->lock, flags);
>>>>>>> upstream/android-13

	/* Find a free channel */
	idx = find_first_zero_bit(ctl->channel_map, NUM_CHANNEL);
	if (idx >= NUM_CHANNEL) {
<<<<<<< HEAD
		spin_unlock(&ctl->lock);
=======
		spin_unlock_irqrestore(&ctl->lock, flags);
>>>>>>> upstream/android-13
		pr_err("No channel available\n");
		return -ENOSPC;
	}

	/* Mark the channel as used */
	set_bit(idx, ctl->channel_map);

<<<<<<< HEAD
=======
	spin_unlock_irqrestore(&ctl->lock, flags);

>>>>>>> upstream/android-13
	/*
	 * Setup the mux of the channel to route the signal of the pad
	 * to the appropriate input of the GIC
	 */
<<<<<<< HEAD
	reg = meson_gpio_irq_channel_to_reg(idx);
	meson_gpio_irq_update_bits(ctl, reg,
				   0xff << REG_PIN_SEL_SHIFT(idx),
				   hwirq << REG_PIN_SEL_SHIFT(idx));

	/*
	 * Get the hwirq number assigned to this channel through
	 * a pointer the channel_irq table. The added benifit of this
=======
	ctl->params->ops.gpio_irq_sel_pin(ctl, idx, hwirq);

	/*
	 * Get the hwirq number assigned to this channel through
	 * a pointer the channel_irq table. The added benefit of this
>>>>>>> upstream/android-13
	 * method is that we can also retrieve the channel index with
	 * it, using the table base.
	 */
	*channel_hwirq = &(ctl->channel_irqs[idx]);

<<<<<<< HEAD
	spin_unlock(&ctl->lock);

=======
>>>>>>> upstream/android-13
	pr_debug("hwirq %lu assigned to channel %d - irq %u\n",
		 hwirq, idx, **channel_hwirq);

	return 0;
}

static unsigned int
meson_gpio_irq_get_channel_idx(struct meson_gpio_irq_controller *ctl,
			       u32 *channel_hwirq)
{
	return channel_hwirq - ctl->channel_irqs;
}

static void
meson_gpio_irq_release_channel(struct meson_gpio_irq_controller *ctl,
			       u32 *channel_hwirq)
{
	unsigned int idx;

	idx = meson_gpio_irq_get_channel_idx(ctl, channel_hwirq);
	clear_bit(idx, ctl->channel_map);
}

static int meson_gpio_irq_type_setup(struct meson_gpio_irq_controller *ctl,
				     unsigned int type,
				     u32 *channel_hwirq)
{
	u32 val = 0;
	unsigned int idx;
<<<<<<< HEAD

=======
	const struct meson_gpio_irq_params *params;

	params = ctl->params;
>>>>>>> upstream/android-13
	idx = meson_gpio_irq_get_channel_idx(ctl, channel_hwirq);

	/*
	 * The controller has a filter block to operate in either LEVEL or
	 * EDGE mode, then signal is sent to the GIC. To enable LEVEL_LOW and
	 * EDGE_FALLING support (which the GIC does not support), the filter
	 * block is also able to invert the input signal it gets before
	 * providing it to the GIC.
	 */
	type &= IRQ_TYPE_SENSE_MASK;

<<<<<<< HEAD
	if (type == IRQ_TYPE_EDGE_BOTH)
		return -EINVAL;

	if (type & (IRQ_TYPE_EDGE_RISING | IRQ_TYPE_EDGE_FALLING))
		val |= REG_EDGE_POL_EDGE(idx);

	if (type & (IRQ_TYPE_LEVEL_LOW | IRQ_TYPE_EDGE_FALLING))
		val |= REG_EDGE_POL_LOW(idx);

	spin_lock(&ctl->lock);

	meson_gpio_irq_update_bits(ctl, REG_EDGE_POL,
				   REG_EDGE_POL_MASK(idx), val);

	spin_unlock(&ctl->lock);
=======
	/*
	 * New controller support EDGE_BOTH trigger. This setting takes
	 * precedence over the other edge/polarity settings
	 */
	if (type == IRQ_TYPE_EDGE_BOTH) {
		if (!params->support_edge_both)
			return -EINVAL;

		val |= REG_BOTH_EDGE(params, idx);
	} else {
		if (type & (IRQ_TYPE_EDGE_RISING | IRQ_TYPE_EDGE_FALLING))
			val |= REG_EDGE_POL_EDGE(params, idx);

		if (type & (IRQ_TYPE_LEVEL_LOW | IRQ_TYPE_EDGE_FALLING))
			val |= REG_EDGE_POL_LOW(params, idx);
	}

	meson_gpio_irq_update_bits(ctl, REG_EDGE_POL,
				   REG_EDGE_POL_MASK(params, idx), val);
>>>>>>> upstream/android-13

	return 0;
}

static unsigned int meson_gpio_irq_type_output(unsigned int type)
{
	unsigned int sense = type & IRQ_TYPE_SENSE_MASK;

	type &= ~IRQ_TYPE_SENSE_MASK;

	/*
	 * The polarity of the signal provided to the GIC should always
	 * be high.
	 */
	if (sense & (IRQ_TYPE_LEVEL_HIGH | IRQ_TYPE_LEVEL_LOW))
		type |= IRQ_TYPE_LEVEL_HIGH;
<<<<<<< HEAD
	else if (sense & (IRQ_TYPE_EDGE_RISING | IRQ_TYPE_EDGE_FALLING))
=======
	else
>>>>>>> upstream/android-13
		type |= IRQ_TYPE_EDGE_RISING;

	return type;
}

static int meson_gpio_irq_set_type(struct irq_data *data, unsigned int type)
{
	struct meson_gpio_irq_controller *ctl = data->domain->host_data;
	u32 *channel_hwirq = irq_data_get_irq_chip_data(data);
	int ret;

	ret = meson_gpio_irq_type_setup(ctl, type, channel_hwirq);
	if (ret)
		return ret;

	return irq_chip_set_type_parent(data,
					meson_gpio_irq_type_output(type));
}

static struct irq_chip meson_gpio_irq_chip = {
	.name			= "meson-gpio-irqchip",
	.irq_mask		= irq_chip_mask_parent,
	.irq_unmask		= irq_chip_unmask_parent,
	.irq_eoi		= irq_chip_eoi_parent,
	.irq_set_type		= meson_gpio_irq_set_type,
	.irq_retrigger		= irq_chip_retrigger_hierarchy,
#ifdef CONFIG_SMP
	.irq_set_affinity	= irq_chip_set_affinity_parent,
#endif
	.flags			= IRQCHIP_SET_TYPE_MASKED,
};

static int meson_gpio_irq_domain_translate(struct irq_domain *domain,
					   struct irq_fwspec *fwspec,
					   unsigned long *hwirq,
					   unsigned int *type)
{
	if (is_of_node(fwspec->fwnode) && fwspec->param_count == 2) {
		*hwirq	= fwspec->param[0];
		*type	= fwspec->param[1];
		return 0;
	}

	return -EINVAL;
}

static int meson_gpio_irq_allocate_gic_irq(struct irq_domain *domain,
					   unsigned int virq,
					   u32 hwirq,
					   unsigned int type)
{
	struct irq_fwspec fwspec;

	fwspec.fwnode = domain->parent->fwnode;
	fwspec.param_count = 3;
	fwspec.param[0] = 0;	/* SPI */
	fwspec.param[1] = hwirq;
	fwspec.param[2] = meson_gpio_irq_type_output(type);

	return irq_domain_alloc_irqs_parent(domain, virq, 1, &fwspec);
}

static int meson_gpio_irq_domain_alloc(struct irq_domain *domain,
				       unsigned int virq,
				       unsigned int nr_irqs,
				       void *data)
{
	struct irq_fwspec *fwspec = data;
	struct meson_gpio_irq_controller *ctl = domain->host_data;
	unsigned long hwirq;
	u32 *channel_hwirq;
	unsigned int type;
	int ret;

	if (WARN_ON(nr_irqs != 1))
		return -EINVAL;

	ret = meson_gpio_irq_domain_translate(domain, fwspec, &hwirq, &type);
	if (ret)
		return ret;

	ret = meson_gpio_irq_request_channel(ctl, hwirq, &channel_hwirq);
	if (ret)
		return ret;

	ret = meson_gpio_irq_allocate_gic_irq(domain, virq,
					      *channel_hwirq, type);
	if (ret < 0) {
		pr_err("failed to allocate gic irq %u\n", *channel_hwirq);
		meson_gpio_irq_release_channel(ctl, channel_hwirq);
		return ret;
	}

	irq_domain_set_hwirq_and_chip(domain, virq, hwirq,
				      &meson_gpio_irq_chip, channel_hwirq);

	return 0;
}

static void meson_gpio_irq_domain_free(struct irq_domain *domain,
				       unsigned int virq,
				       unsigned int nr_irqs)
{
	struct meson_gpio_irq_controller *ctl = domain->host_data;
	struct irq_data *irq_data;
	u32 *channel_hwirq;

	if (WARN_ON(nr_irqs != 1))
		return;

	irq_domain_free_irqs_parent(domain, virq, 1);

	irq_data = irq_domain_get_irq_data(domain, virq);
	channel_hwirq = irq_data_get_irq_chip_data(irq_data);

	meson_gpio_irq_release_channel(ctl, channel_hwirq);
}

static const struct irq_domain_ops meson_gpio_irq_domain_ops = {
	.alloc		= meson_gpio_irq_domain_alloc,
	.free		= meson_gpio_irq_domain_free,
	.translate	= meson_gpio_irq_domain_translate,
};

<<<<<<< HEAD
static int __init meson_gpio_irq_parse_dt(struct device_node *node,
					  struct meson_gpio_irq_controller *ctl)
{
	const struct of_device_id *match;
	const struct meson_gpio_irq_params *params;
=======
static int meson_gpio_irq_parse_dt(struct device_node *node,
				   struct meson_gpio_irq_controller *ctl)
{
	const struct of_device_id *match;
>>>>>>> upstream/android-13
	int ret;

	match = of_match_node(meson_irq_gpio_matches, node);
	if (!match)
		return -ENODEV;

<<<<<<< HEAD
	params = match->data;
	ctl->nr_hwirq = params->nr_hwirq;
=======
	ctl->params = match->data;
>>>>>>> upstream/android-13

	ret = of_property_read_variable_u32_array(node,
						  "amlogic,channel-interrupts",
						  ctl->channel_irqs,
						  NUM_CHANNEL,
						  NUM_CHANNEL);
	if (ret < 0) {
		pr_err("can't get %d channel interrupts\n", NUM_CHANNEL);
		return ret;
	}

<<<<<<< HEAD
	return 0;
}

static int __init meson_gpio_irq_of_init(struct device_node *node,
					 struct device_node *parent)
{
	struct irq_domain *domain, *parent_domain;
	struct meson_gpio_irq_controller *ctl;
	int ret;

	if (!parent) {
		pr_err("missing parent interrupt node\n");
=======
	ctl->params->ops.gpio_irq_init(ctl);

	return 0;
}

static int meson_gpio_intc_probe(struct platform_device *pdev)
{
	struct device_node *node = pdev->dev.of_node, *parent;
	struct meson_gpio_irq_controller *ctl;
	struct irq_domain *parent_domain;
	struct resource *res;
	int ret;

	parent = of_irq_find_parent(node);
	if (!parent) {
		dev_err(&pdev->dev, "missing parent interrupt node\n");
>>>>>>> upstream/android-13
		return -ENODEV;
	}

	parent_domain = irq_find_host(parent);
	if (!parent_domain) {
<<<<<<< HEAD
		pr_err("unable to obtain parent domain\n");
		return -ENXIO;
	}

	ctl = kzalloc(sizeof(*ctl), GFP_KERNEL);
=======
		dev_err(&pdev->dev, "unable to obtain parent domain\n");
		return -ENXIO;
	}

	ctl = devm_kzalloc(&pdev->dev, sizeof(*ctl), GFP_KERNEL);
>>>>>>> upstream/android-13
	if (!ctl)
		return -ENOMEM;

	spin_lock_init(&ctl->lock);

<<<<<<< HEAD
	ctl->base = of_iomap(node, 0);
	if (!ctl->base) {
		ret = -ENOMEM;
		goto free_ctl;
	}

	ret = meson_gpio_irq_parse_dt(node, ctl);
	if (ret)
		goto free_channel_irqs;

	domain = irq_domain_create_hierarchy(parent_domain, 0, ctl->nr_hwirq,
					     of_node_to_fwnode(node),
					     &meson_gpio_irq_domain_ops,
					     ctl);
	if (!domain) {
		pr_err("failed to add domain\n");
		ret = -ENODEV;
		goto free_channel_irqs;
	}

	pr_info("%d to %d gpio interrupt mux initialized\n",
		ctl->nr_hwirq, NUM_CHANNEL);

	return 0;

free_channel_irqs:
	iounmap(ctl->base);
free_ctl:
	kfree(ctl);

	return ret;
}

IRQCHIP_DECLARE(meson_gpio_intc, "amlogic,meson-gpio-intc",
		meson_gpio_irq_of_init);
=======
	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	ctl->base = devm_ioremap_resource(&pdev->dev, res);
	if (IS_ERR(ctl->base))
		return PTR_ERR(ctl->base);

	ret = meson_gpio_irq_parse_dt(node, ctl);
	if (ret)
		return ret;

	ctl->domain = irq_domain_create_hierarchy(parent_domain, 0,
						  ctl->params->nr_hwirq,
						  of_node_to_fwnode(node),
						  &meson_gpio_irq_domain_ops,
						  ctl);
	if (!ctl->domain) {
		dev_err(&pdev->dev, "failed to add domain\n");
		return -ENODEV;
	}

	platform_set_drvdata(pdev, ctl);

	dev_info(&pdev->dev, "%d to %d gpio interrupt mux initialized\n",
		 ctl->params->nr_hwirq, NUM_CHANNEL);

	return 0;
}

static int meson_gpio_intc_remove(struct platform_device *pdev)
{
	struct meson_gpio_irq_controller *ctl = platform_get_drvdata(pdev);

	irq_domain_remove(ctl->domain);

	return 0;
}

static const struct of_device_id meson_gpio_intc_of_match[] = {
	{ .compatible = "amlogic,meson-gpio-intc", },
	{},
};
MODULE_DEVICE_TABLE(of, meson_gpio_intc_of_match);

static struct platform_driver meson_gpio_intc_driver = {
	.probe  = meson_gpio_intc_probe,
	.remove = meson_gpio_intc_remove,
	.driver = {
		.name = "meson-gpio-intc",
		.of_match_table = meson_gpio_intc_of_match,
	},
};
module_platform_driver(meson_gpio_intc_driver);

MODULE_AUTHOR("Jerome Brunet <jbrunet@baylibre.com>");
MODULE_LICENSE("GPL v2");
MODULE_ALIAS("platform:meson-gpio-intc");
>>>>>>> upstream/android-13
