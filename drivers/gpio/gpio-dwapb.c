<<<<<<< HEAD
/*
 * Copyright (c) 2011 Jamie Iles
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
=======
// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright (c) 2011 Jamie Iles
 *
>>>>>>> upstream/android-13
 * All enquiries to support@picochip.com
 */
#include <linux/acpi.h>
#include <linux/clk.h>
#include <linux/err.h>
#include <linux/gpio/driver.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/io.h>
#include <linux/ioport.h>
#include <linux/irq.h>
<<<<<<< HEAD
#include <linux/irqdomain.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/of_address.h>
#include <linux/of_device.h>
#include <linux/of_irq.h>
#include <linux/platform_device.h>
#include <linux/property.h>
#include <linux/reset.h>
#include <linux/spinlock.h>
#include <linux/platform_data/gpio-dwapb.h>
#include <linux/slab.h>

#include "gpiolib.h"
=======
#include <linux/mod_devicetable.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/platform_device.h>
#include <linux/property.h>
#include <linux/reset.h>
#include <linux/slab.h>
#include <linux/spinlock.h>

#include "gpiolib.h"
#include "gpiolib-acpi.h"
>>>>>>> upstream/android-13

#define GPIO_SWPORTA_DR		0x00
#define GPIO_SWPORTA_DDR	0x04
#define GPIO_SWPORTB_DR		0x0c
#define GPIO_SWPORTB_DDR	0x10
#define GPIO_SWPORTC_DR		0x18
#define GPIO_SWPORTC_DDR	0x1c
#define GPIO_SWPORTD_DR		0x24
#define GPIO_SWPORTD_DDR	0x28
#define GPIO_INTEN		0x30
#define GPIO_INTMASK		0x34
#define GPIO_INTTYPE_LEVEL	0x38
#define GPIO_INT_POLARITY	0x3c
#define GPIO_INTSTATUS		0x40
#define GPIO_PORTA_DEBOUNCE	0x48
#define GPIO_PORTA_EOI		0x4c
#define GPIO_EXT_PORTA		0x50
#define GPIO_EXT_PORTB		0x54
#define GPIO_EXT_PORTC		0x58
#define GPIO_EXT_PORTD		0x5c

#define DWAPB_DRIVER_NAME	"gpio-dwapb"
#define DWAPB_MAX_PORTS		4
<<<<<<< HEAD
=======
#define DWAPB_MAX_GPIOS		32
>>>>>>> upstream/android-13

#define GPIO_EXT_PORT_STRIDE	0x04 /* register stride 32 bits */
#define GPIO_SWPORT_DR_STRIDE	0x0c /* register stride 3*32 bits */
#define GPIO_SWPORT_DDR_STRIDE	0x0c /* register stride 3*32 bits */

#define GPIO_REG_OFFSET_V2	1

#define GPIO_INTMASK_V2		0x44
#define GPIO_INTTYPE_LEVEL_V2	0x34
#define GPIO_INT_POLARITY_V2	0x38
#define GPIO_INTSTATUS_V2	0x3c
#define GPIO_PORTA_EOI_V2	0x40

<<<<<<< HEAD
struct dwapb_gpio;

=======
#define DWAPB_NR_CLOCKS		2

struct dwapb_gpio;

struct dwapb_port_property {
	struct fwnode_handle *fwnode;
	unsigned int idx;
	unsigned int ngpio;
	unsigned int gpio_base;
	int irq[DWAPB_MAX_GPIOS];
};

struct dwapb_platform_data {
	struct dwapb_port_property *properties;
	unsigned int nports;
};

>>>>>>> upstream/android-13
#ifdef CONFIG_PM_SLEEP
/* Store GPIO context across system-wide suspend/resume transitions */
struct dwapb_context {
	u32 data;
	u32 dir;
	u32 ext;
	u32 int_en;
	u32 int_mask;
	u32 int_type;
	u32 int_pol;
	u32 int_deb;
	u32 wake_en;
};
#endif

<<<<<<< HEAD
struct dwapb_gpio_port {
	struct gpio_chip	gc;
	bool			is_registered;
=======
struct dwapb_gpio_port_irqchip {
	struct irq_chip		irqchip;
	unsigned int		nr_irqs;
	unsigned int		irq[DWAPB_MAX_GPIOS];
};

struct dwapb_gpio_port {
	struct gpio_chip	gc;
	struct dwapb_gpio_port_irqchip *pirq;
>>>>>>> upstream/android-13
	struct dwapb_gpio	*gpio;
#ifdef CONFIG_PM_SLEEP
	struct dwapb_context	*ctx;
#endif
	unsigned int		idx;
};
<<<<<<< HEAD
=======
#define to_dwapb_gpio(_gc) \
	(container_of(_gc, struct dwapb_gpio_port, gc)->gpio)
>>>>>>> upstream/android-13

struct dwapb_gpio {
	struct	device		*dev;
	void __iomem		*regs;
	struct dwapb_gpio_port	*ports;
	unsigned int		nr_ports;
<<<<<<< HEAD
	struct irq_domain	*domain;
	unsigned int		flags;
	struct reset_control	*rst;
	struct clk		*clk;
=======
	unsigned int		flags;
	struct reset_control	*rst;
	struct clk_bulk_data	clks[DWAPB_NR_CLOCKS];
>>>>>>> upstream/android-13
};

static inline u32 gpio_reg_v2_convert(unsigned int offset)
{
	switch (offset) {
	case GPIO_INTMASK:
		return GPIO_INTMASK_V2;
	case GPIO_INTTYPE_LEVEL:
		return GPIO_INTTYPE_LEVEL_V2;
	case GPIO_INT_POLARITY:
		return GPIO_INT_POLARITY_V2;
	case GPIO_INTSTATUS:
		return GPIO_INTSTATUS_V2;
	case GPIO_PORTA_EOI:
		return GPIO_PORTA_EOI_V2;
	}

	return offset;
}

static inline u32 gpio_reg_convert(struct dwapb_gpio *gpio, unsigned int offset)
{
	if (gpio->flags & GPIO_REG_OFFSET_V2)
		return gpio_reg_v2_convert(offset);

	return offset;
}

static inline u32 dwapb_read(struct dwapb_gpio *gpio, unsigned int offset)
{
	struct gpio_chip *gc	= &gpio->ports[0].gc;
	void __iomem *reg_base	= gpio->regs;

	return gc->read_reg(reg_base + gpio_reg_convert(gpio, offset));
}

static inline void dwapb_write(struct dwapb_gpio *gpio, unsigned int offset,
			       u32 val)
{
	struct gpio_chip *gc	= &gpio->ports[0].gc;
	void __iomem *reg_base	= gpio->regs;

	gc->write_reg(reg_base + gpio_reg_convert(gpio, offset), val);
}

<<<<<<< HEAD
static int dwapb_gpio_to_irq(struct gpio_chip *gc, unsigned offset)
{
	struct dwapb_gpio_port *port = gpiochip_get_data(gc);
	struct dwapb_gpio *gpio = port->gpio;

	return irq_find_mapping(gpio->domain, offset);
}

=======
>>>>>>> upstream/android-13
static struct dwapb_gpio_port *dwapb_offs_to_port(struct dwapb_gpio *gpio, unsigned int offs)
{
	struct dwapb_gpio_port *port;
	int i;

	for (i = 0; i < gpio->nr_ports; i++) {
		port = &gpio->ports[i];
<<<<<<< HEAD
		if (port->idx == offs / 32)
=======
		if (port->idx == offs / DWAPB_MAX_GPIOS)
>>>>>>> upstream/android-13
			return port;
	}

	return NULL;
}

static void dwapb_toggle_trigger(struct dwapb_gpio *gpio, unsigned int offs)
{
	struct dwapb_gpio_port *port = dwapb_offs_to_port(gpio, offs);
	struct gpio_chip *gc;
	u32 pol;
	int val;

	if (!port)
		return;
	gc = &port->gc;

	pol = dwapb_read(gpio, GPIO_INT_POLARITY);
	/* Just read the current value right out of the data register */
<<<<<<< HEAD
	val = gc->get(gc, offs % 32);
=======
	val = gc->get(gc, offs % DWAPB_MAX_GPIOS);
>>>>>>> upstream/android-13
	if (val)
		pol &= ~BIT(offs);
	else
		pol |= BIT(offs);

	dwapb_write(gpio, GPIO_INT_POLARITY, pol);
}

static u32 dwapb_do_irq(struct dwapb_gpio *gpio)
{
<<<<<<< HEAD
	u32 irq_status = dwapb_read(gpio, GPIO_INTSTATUS);
	u32 ret = irq_status;

	while (irq_status) {
		int hwirq = fls(irq_status) - 1;
		int gpio_irq = irq_find_mapping(gpio->domain, hwirq);

		generic_handle_irq(gpio_irq);
		irq_status &= ~BIT(hwirq);

		if ((irq_get_trigger_type(gpio_irq) & IRQ_TYPE_SENSE_MASK)
			== IRQ_TYPE_EDGE_BOTH)
			dwapb_toggle_trigger(gpio, hwirq);
	}

	return ret;
=======
	struct gpio_chip *gc = &gpio->ports[0].gc;
	unsigned long irq_status;
	irq_hw_number_t hwirq;

	irq_status = dwapb_read(gpio, GPIO_INTSTATUS);
	for_each_set_bit(hwirq, &irq_status, DWAPB_MAX_GPIOS) {
		int gpio_irq = irq_find_mapping(gc->irq.domain, hwirq);
		u32 irq_type = irq_get_trigger_type(gpio_irq);

		generic_handle_irq(gpio_irq);

		if ((irq_type & IRQ_TYPE_SENSE_MASK) == IRQ_TYPE_EDGE_BOTH)
			dwapb_toggle_trigger(gpio, hwirq);
	}

	return irq_status;
>>>>>>> upstream/android-13
}

static void dwapb_irq_handler(struct irq_desc *desc)
{
	struct dwapb_gpio *gpio = irq_desc_get_handler_data(desc);
	struct irq_chip *chip = irq_desc_get_chip(desc);

<<<<<<< HEAD
	dwapb_do_irq(gpio);

	if (chip->irq_eoi)
		chip->irq_eoi(irq_desc_get_irq_data(desc));
=======
	chained_irq_enter(chip, desc);
	dwapb_do_irq(gpio);
	chained_irq_exit(chip, desc);
}

static irqreturn_t dwapb_irq_handler_mfd(int irq, void *dev_id)
{
	return IRQ_RETVAL(dwapb_do_irq(dev_id));
}

static void dwapb_irq_ack(struct irq_data *d)
{
	struct gpio_chip *gc = irq_data_get_irq_chip_data(d);
	struct dwapb_gpio *gpio = to_dwapb_gpio(gc);
	u32 val = BIT(irqd_to_hwirq(d));
	unsigned long flags;

	spin_lock_irqsave(&gc->bgpio_lock, flags);
	dwapb_write(gpio, GPIO_PORTA_EOI, val);
	spin_unlock_irqrestore(&gc->bgpio_lock, flags);
}

static void dwapb_irq_mask(struct irq_data *d)
{
	struct gpio_chip *gc = irq_data_get_irq_chip_data(d);
	struct dwapb_gpio *gpio = to_dwapb_gpio(gc);
	unsigned long flags;
	u32 val;

	spin_lock_irqsave(&gc->bgpio_lock, flags);
	val = dwapb_read(gpio, GPIO_INTMASK) | BIT(irqd_to_hwirq(d));
	dwapb_write(gpio, GPIO_INTMASK, val);
	spin_unlock_irqrestore(&gc->bgpio_lock, flags);
}

static void dwapb_irq_unmask(struct irq_data *d)
{
	struct gpio_chip *gc = irq_data_get_irq_chip_data(d);
	struct dwapb_gpio *gpio = to_dwapb_gpio(gc);
	unsigned long flags;
	u32 val;

	spin_lock_irqsave(&gc->bgpio_lock, flags);
	val = dwapb_read(gpio, GPIO_INTMASK) & ~BIT(irqd_to_hwirq(d));
	dwapb_write(gpio, GPIO_INTMASK, val);
	spin_unlock_irqrestore(&gc->bgpio_lock, flags);
>>>>>>> upstream/android-13
}

static void dwapb_irq_enable(struct irq_data *d)
{
<<<<<<< HEAD
	struct irq_chip_generic *igc = irq_data_get_irq_chip_data(d);
	struct dwapb_gpio *gpio = igc->private;
	struct gpio_chip *gc = &gpio->ports[0].gc;
=======
	struct gpio_chip *gc = irq_data_get_irq_chip_data(d);
	struct dwapb_gpio *gpio = to_dwapb_gpio(gc);
>>>>>>> upstream/android-13
	unsigned long flags;
	u32 val;

	spin_lock_irqsave(&gc->bgpio_lock, flags);
	val = dwapb_read(gpio, GPIO_INTEN);
<<<<<<< HEAD
	val |= BIT(d->hwirq);
=======
	val |= BIT(irqd_to_hwirq(d));
>>>>>>> upstream/android-13
	dwapb_write(gpio, GPIO_INTEN, val);
	spin_unlock_irqrestore(&gc->bgpio_lock, flags);
}

static void dwapb_irq_disable(struct irq_data *d)
{
<<<<<<< HEAD
	struct irq_chip_generic *igc = irq_data_get_irq_chip_data(d);
	struct dwapb_gpio *gpio = igc->private;
	struct gpio_chip *gc = &gpio->ports[0].gc;
=======
	struct gpio_chip *gc = irq_data_get_irq_chip_data(d);
	struct dwapb_gpio *gpio = to_dwapb_gpio(gc);
>>>>>>> upstream/android-13
	unsigned long flags;
	u32 val;

	spin_lock_irqsave(&gc->bgpio_lock, flags);
	val = dwapb_read(gpio, GPIO_INTEN);
<<<<<<< HEAD
	val &= ~BIT(d->hwirq);
=======
	val &= ~BIT(irqd_to_hwirq(d));
>>>>>>> upstream/android-13
	dwapb_write(gpio, GPIO_INTEN, val);
	spin_unlock_irqrestore(&gc->bgpio_lock, flags);
}

<<<<<<< HEAD
static int dwapb_irq_reqres(struct irq_data *d)
{
	struct irq_chip_generic *igc = irq_data_get_irq_chip_data(d);
	struct dwapb_gpio *gpio = igc->private;
	struct gpio_chip *gc = &gpio->ports[0].gc;
	int ret;

	ret = gpiochip_lock_as_irq(gc, irqd_to_hwirq(d));
	if (ret) {
		dev_err(gpio->dev, "unable to lock HW IRQ %lu for IRQ\n",
			irqd_to_hwirq(d));
		return ret;
	}
	return 0;
}

static void dwapb_irq_relres(struct irq_data *d)
{
	struct irq_chip_generic *igc = irq_data_get_irq_chip_data(d);
	struct dwapb_gpio *gpio = igc->private;
	struct gpio_chip *gc = &gpio->ports[0].gc;

	gpiochip_unlock_as_irq(gc, irqd_to_hwirq(d));
}

static int dwapb_irq_set_type(struct irq_data *d, u32 type)
{
	struct irq_chip_generic *igc = irq_data_get_irq_chip_data(d);
	struct dwapb_gpio *gpio = igc->private;
	struct gpio_chip *gc = &gpio->ports[0].gc;
	int bit = d->hwirq;
	unsigned long level, polarity, flags;

	if (type & ~(IRQ_TYPE_EDGE_RISING | IRQ_TYPE_EDGE_FALLING |
		     IRQ_TYPE_LEVEL_HIGH | IRQ_TYPE_LEVEL_LOW))
		return -EINVAL;

=======
static int dwapb_irq_set_type(struct irq_data *d, u32 type)
{
	struct gpio_chip *gc = irq_data_get_irq_chip_data(d);
	struct dwapb_gpio *gpio = to_dwapb_gpio(gc);
	irq_hw_number_t bit = irqd_to_hwirq(d);
	unsigned long level, polarity, flags;

>>>>>>> upstream/android-13
	spin_lock_irqsave(&gc->bgpio_lock, flags);
	level = dwapb_read(gpio, GPIO_INTTYPE_LEVEL);
	polarity = dwapb_read(gpio, GPIO_INT_POLARITY);

	switch (type) {
	case IRQ_TYPE_EDGE_BOTH:
		level |= BIT(bit);
		dwapb_toggle_trigger(gpio, bit);
		break;
	case IRQ_TYPE_EDGE_RISING:
		level |= BIT(bit);
		polarity |= BIT(bit);
		break;
	case IRQ_TYPE_EDGE_FALLING:
		level |= BIT(bit);
		polarity &= ~BIT(bit);
		break;
	case IRQ_TYPE_LEVEL_HIGH:
		level &= ~BIT(bit);
		polarity |= BIT(bit);
		break;
	case IRQ_TYPE_LEVEL_LOW:
		level &= ~BIT(bit);
		polarity &= ~BIT(bit);
		break;
	}

<<<<<<< HEAD
	irq_setup_alt_chip(d, type);
=======
	if (type & IRQ_TYPE_LEVEL_MASK)
		irq_set_handler_locked(d, handle_level_irq);
	else if (type & IRQ_TYPE_EDGE_BOTH)
		irq_set_handler_locked(d, handle_edge_irq);
>>>>>>> upstream/android-13

	dwapb_write(gpio, GPIO_INTTYPE_LEVEL, level);
	if (type != IRQ_TYPE_EDGE_BOTH)
		dwapb_write(gpio, GPIO_INT_POLARITY, polarity);
	spin_unlock_irqrestore(&gc->bgpio_lock, flags);

	return 0;
}

#ifdef CONFIG_PM_SLEEP
static int dwapb_irq_set_wake(struct irq_data *d, unsigned int enable)
{
<<<<<<< HEAD
	struct irq_chip_generic *igc = irq_data_get_irq_chip_data(d);
	struct dwapb_gpio *gpio = igc->private;
	struct dwapb_context *ctx = gpio->ports[0].ctx;

	if (enable)
		ctx->wake_en |= BIT(d->hwirq);
	else
		ctx->wake_en &= ~BIT(d->hwirq);
=======
	struct gpio_chip *gc = irq_data_get_irq_chip_data(d);
	struct dwapb_gpio *gpio = to_dwapb_gpio(gc);
	struct dwapb_context *ctx = gpio->ports[0].ctx;
	irq_hw_number_t bit = irqd_to_hwirq(d);

	if (enable)
		ctx->wake_en |= BIT(bit);
	else
		ctx->wake_en &= ~BIT(bit);
>>>>>>> upstream/android-13

	return 0;
}
#endif

static int dwapb_gpio_set_debounce(struct gpio_chip *gc,
				   unsigned offset, unsigned debounce)
{
	struct dwapb_gpio_port *port = gpiochip_get_data(gc);
	struct dwapb_gpio *gpio = port->gpio;
	unsigned long flags, val_deb;
	unsigned long mask = BIT(offset);

	spin_lock_irqsave(&gc->bgpio_lock, flags);

	val_deb = dwapb_read(gpio, GPIO_PORTA_DEBOUNCE);
	if (debounce)
<<<<<<< HEAD
		dwapb_write(gpio, GPIO_PORTA_DEBOUNCE, val_deb | mask);
	else
		dwapb_write(gpio, GPIO_PORTA_DEBOUNCE, val_deb & ~mask);
=======
		val_deb |= mask;
	else
		val_deb &= ~mask;
	dwapb_write(gpio, GPIO_PORTA_DEBOUNCE, val_deb);
>>>>>>> upstream/android-13

	spin_unlock_irqrestore(&gc->bgpio_lock, flags);

	return 0;
}

static int dwapb_gpio_set_config(struct gpio_chip *gc, unsigned offset,
				 unsigned long config)
{
	u32 debounce;

	if (pinconf_to_config_param(config) != PIN_CONFIG_INPUT_DEBOUNCE)
		return -ENOTSUPP;

	debounce = pinconf_to_config_argument(config);
	return dwapb_gpio_set_debounce(gc, offset, debounce);
}

<<<<<<< HEAD
static irqreturn_t dwapb_irq_handler_mfd(int irq, void *dev_id)
{
	u32 worked;
	struct dwapb_gpio *gpio = dev_id;

	worked = dwapb_do_irq(gpio);

	return worked ? IRQ_HANDLED : IRQ_NONE;
=======
static int dwapb_convert_irqs(struct dwapb_gpio_port_irqchip *pirq,
			      struct dwapb_port_property *pp)
{
	int i;

	/* Group all available IRQs into an array of parental IRQs. */
	for (i = 0; i < pp->ngpio; ++i) {
		if (!pp->irq[i])
			continue;

		pirq->irq[pirq->nr_irqs++] = pp->irq[i];
	}

	return pirq->nr_irqs ? 0 : -ENOENT;
>>>>>>> upstream/android-13
}

static void dwapb_configure_irqs(struct dwapb_gpio *gpio,
				 struct dwapb_gpio_port *port,
				 struct dwapb_port_property *pp)
{
<<<<<<< HEAD
	struct gpio_chip *gc = &port->gc;
	struct fwnode_handle  *fwnode = pp->fwnode;
	struct irq_chip_generic	*irq_gc = NULL;
	unsigned int hwirq, ngpio = gc->ngpio;
	struct irq_chip_type *ct;
	int err, i;

	gpio->domain = irq_domain_create_linear(fwnode, ngpio,
						 &irq_generic_chip_ops, gpio);
	if (!gpio->domain)
		return;

	err = irq_alloc_domain_generic_chips(gpio->domain, ngpio, 2,
					     DWAPB_DRIVER_NAME, handle_level_irq,
					     IRQ_NOREQUEST, 0,
					     IRQ_GC_INIT_NESTED_LOCK);
	if (err) {
		dev_info(gpio->dev, "irq_alloc_domain_generic_chips failed\n");
		irq_domain_remove(gpio->domain);
		gpio->domain = NULL;
		return;
	}

	irq_gc = irq_get_domain_generic_chip(gpio->domain, 0);
	if (!irq_gc) {
		irq_domain_remove(gpio->domain);
		gpio->domain = NULL;
		return;
	}

	irq_gc->reg_base = gpio->regs;
	irq_gc->private = gpio;

	for (i = 0; i < 2; i++) {
		ct = &irq_gc->chip_types[i];
		ct->chip.irq_ack = irq_gc_ack_set_bit;
		ct->chip.irq_mask = irq_gc_mask_set_bit;
		ct->chip.irq_unmask = irq_gc_mask_clr_bit;
		ct->chip.irq_set_type = dwapb_irq_set_type;
		ct->chip.irq_enable = dwapb_irq_enable;
		ct->chip.irq_disable = dwapb_irq_disable;
		ct->chip.irq_request_resources = dwapb_irq_reqres;
		ct->chip.irq_release_resources = dwapb_irq_relres;
#ifdef CONFIG_PM_SLEEP
		ct->chip.irq_set_wake = dwapb_irq_set_wake;
#endif
		ct->regs.ack = gpio_reg_convert(gpio, GPIO_PORTA_EOI);
		ct->regs.mask = gpio_reg_convert(gpio, GPIO_INTMASK);
		ct->type = IRQ_TYPE_LEVEL_MASK;
	}

	irq_gc->chip_types[0].type = IRQ_TYPE_LEVEL_MASK;
	irq_gc->chip_types[1].type = IRQ_TYPE_EDGE_BOTH;
	irq_gc->chip_types[1].handler = handle_edge_irq;

	if (!pp->irq_shared) {
		int i;

		for (i = 0; i < pp->ngpio; i++) {
			if (pp->irq[i] >= 0)
				irq_set_chained_handler_and_data(pp->irq[i],
						dwapb_irq_handler, gpio);
		}
	} else {
		/*
		 * Request a shared IRQ since where MFD would have devices
		 * using the same irq pin
		 */
=======
	struct dwapb_gpio_port_irqchip *pirq;
	struct gpio_chip *gc = &port->gc;
	struct gpio_irq_chip *girq;
	int err;

	pirq = devm_kzalloc(gpio->dev, sizeof(*pirq), GFP_KERNEL);
	if (!pirq)
		return;

	if (dwapb_convert_irqs(pirq, pp)) {
		dev_warn(gpio->dev, "no IRQ for port%d\n", pp->idx);
		goto err_kfree_pirq;
	}

	girq = &gc->irq;
	girq->handler = handle_bad_irq;
	girq->default_type = IRQ_TYPE_NONE;

	port->pirq = pirq;
	pirq->irqchip.name = DWAPB_DRIVER_NAME;
	pirq->irqchip.irq_ack = dwapb_irq_ack;
	pirq->irqchip.irq_mask = dwapb_irq_mask;
	pirq->irqchip.irq_unmask = dwapb_irq_unmask;
	pirq->irqchip.irq_set_type = dwapb_irq_set_type;
	pirq->irqchip.irq_enable = dwapb_irq_enable;
	pirq->irqchip.irq_disable = dwapb_irq_disable;
#ifdef CONFIG_PM_SLEEP
	pirq->irqchip.irq_set_wake = dwapb_irq_set_wake;
#endif

	/*
	 * Intel ACPI-based platforms mostly have the DesignWare APB GPIO
	 * IRQ lane shared between several devices. In that case the parental
	 * IRQ has to be handled in the shared way so to be properly delivered
	 * to all the connected devices.
	 */
	if (has_acpi_companion(gpio->dev)) {
		girq->num_parents = 0;
		girq->parents = NULL;
		girq->parent_handler = NULL;

>>>>>>> upstream/android-13
		err = devm_request_irq(gpio->dev, pp->irq[0],
				       dwapb_irq_handler_mfd,
				       IRQF_SHARED, DWAPB_DRIVER_NAME, gpio);
		if (err) {
			dev_err(gpio->dev, "error requesting IRQ\n");
<<<<<<< HEAD
			irq_domain_remove(gpio->domain);
			gpio->domain = NULL;
			return;
		}
	}

	for (hwirq = 0 ; hwirq < ngpio ; hwirq++)
		irq_create_mapping(gpio->domain, hwirq);

	port->gc.to_irq = dwapb_gpio_to_irq;
}

static void dwapb_irq_teardown(struct dwapb_gpio *gpio)
{
	struct dwapb_gpio_port *port = &gpio->ports[0];
	struct gpio_chip *gc = &port->gc;
	unsigned int ngpio = gc->ngpio;
	irq_hw_number_t hwirq;

	if (!gpio->domain)
		return;

	for (hwirq = 0 ; hwirq < ngpio ; hwirq++)
		irq_dispose_mapping(irq_find_mapping(gpio->domain, hwirq));

	irq_domain_remove(gpio->domain);
	gpio->domain = NULL;
=======
			goto err_kfree_pirq;
		}
	} else {
		girq->num_parents = pirq->nr_irqs;
		girq->parents = pirq->irq;
		girq->parent_handler_data = gpio;
		girq->parent_handler = dwapb_irq_handler;
	}

	girq->chip = &pirq->irqchip;

	return;

err_kfree_pirq:
	devm_kfree(gpio->dev, pirq);
>>>>>>> upstream/android-13
}

static int dwapb_gpio_add_port(struct dwapb_gpio *gpio,
			       struct dwapb_port_property *pp,
			       unsigned int offs)
{
	struct dwapb_gpio_port *port;
	void __iomem *dat, *set, *dirout;
	int err;

	port = &gpio->ports[offs];
	port->gpio = gpio;
	port->idx = pp->idx;

#ifdef CONFIG_PM_SLEEP
	port->ctx = devm_kzalloc(gpio->dev, sizeof(*port->ctx), GFP_KERNEL);
	if (!port->ctx)
		return -ENOMEM;
#endif

<<<<<<< HEAD
	dat = gpio->regs + GPIO_EXT_PORTA + (pp->idx * GPIO_EXT_PORT_STRIDE);
	set = gpio->regs + GPIO_SWPORTA_DR + (pp->idx * GPIO_SWPORT_DR_STRIDE);
	dirout = gpio->regs + GPIO_SWPORTA_DDR +
		(pp->idx * GPIO_SWPORT_DDR_STRIDE);
=======
	dat = gpio->regs + GPIO_EXT_PORTA + pp->idx * GPIO_EXT_PORT_STRIDE;
	set = gpio->regs + GPIO_SWPORTA_DR + pp->idx * GPIO_SWPORT_DR_STRIDE;
	dirout = gpio->regs + GPIO_SWPORTA_DDR + pp->idx * GPIO_SWPORT_DDR_STRIDE;
>>>>>>> upstream/android-13

	/* This registers 32 GPIO lines per port */
	err = bgpio_init(&port->gc, gpio->dev, 4, dat, set, NULL, dirout,
			 NULL, 0);
	if (err) {
		dev_err(gpio->dev, "failed to init gpio chip for port%d\n",
			port->idx);
		return err;
	}

#ifdef CONFIG_OF_GPIO
	port->gc.of_node = to_of_node(pp->fwnode);
#endif
	port->gc.ngpio = pp->ngpio;
	port->gc.base = pp->gpio_base;

	/* Only port A support debounce */
	if (pp->idx == 0)
		port->gc.set_config = dwapb_gpio_set_config;

<<<<<<< HEAD
	if (pp->has_irq)
		dwapb_configure_irqs(gpio, port, pp);

	err = gpiochip_add_data(&port->gc, port);
=======
	/* Only port A can provide interrupts in all configurations of the IP */
	if (pp->idx == 0)
		dwapb_configure_irqs(gpio, port, pp);

	err = devm_gpiochip_add_data(gpio->dev, &port->gc, port);
>>>>>>> upstream/android-13
	if (err) {
		dev_err(gpio->dev, "failed to register gpiochip for port%d\n",
			port->idx);
		return err;
	}

<<<<<<< HEAD
	/* Add GPIO-signaled ACPI event support */
	acpi_gpiochip_request_interrupts(&port->gc);

	port->is_registered = true;

	return 0;
}

static void dwapb_gpio_unregister(struct dwapb_gpio *gpio)
{
	unsigned int m;

	for (m = 0; m < gpio->nr_ports; ++m) {
		struct dwapb_gpio_port *port = &gpio->ports[m];

		if (!port->is_registered)
			continue;

		acpi_gpiochip_free_interrupts(&port->gc);
		gpiochip_remove(&port->gc);
	}
}

static struct dwapb_platform_data *
dwapb_gpio_get_pdata(struct device *dev)
=======
	return 0;
}

static void dwapb_get_irq(struct device *dev, struct fwnode_handle *fwnode,
			  struct dwapb_port_property *pp)
{
	int irq, j;

	for (j = 0; j < pp->ngpio; j++) {
		if (has_acpi_companion(dev))
			irq = platform_get_irq_optional(to_platform_device(dev), j);
		else
			irq = fwnode_irq_get(fwnode, j);
		if (irq > 0)
			pp->irq[j] = irq;
	}
}

static struct dwapb_platform_data *dwapb_gpio_get_pdata(struct device *dev)
>>>>>>> upstream/android-13
{
	struct fwnode_handle *fwnode;
	struct dwapb_platform_data *pdata;
	struct dwapb_port_property *pp;
	int nports;
<<<<<<< HEAD
	int i, j;
=======
	int i;
>>>>>>> upstream/android-13

	nports = device_get_child_node_count(dev);
	if (nports == 0)
		return ERR_PTR(-ENODEV);

	pdata = devm_kzalloc(dev, sizeof(*pdata), GFP_KERNEL);
	if (!pdata)
		return ERR_PTR(-ENOMEM);

	pdata->properties = devm_kcalloc(dev, nports, sizeof(*pp), GFP_KERNEL);
	if (!pdata->properties)
		return ERR_PTR(-ENOMEM);

	pdata->nports = nports;

	i = 0;
	device_for_each_child_node(dev, fwnode)  {
<<<<<<< HEAD
		struct device_node *np = NULL;

=======
>>>>>>> upstream/android-13
		pp = &pdata->properties[i++];
		pp->fwnode = fwnode;

		if (fwnode_property_read_u32(fwnode, "reg", &pp->idx) ||
		    pp->idx >= DWAPB_MAX_PORTS) {
			dev_err(dev,
				"missing/invalid port index for port%d\n", i);
			fwnode_handle_put(fwnode);
			return ERR_PTR(-EINVAL);
		}

<<<<<<< HEAD
		if (fwnode_property_read_u32(fwnode, "snps,nr-gpios",
					 &pp->ngpio)) {
			dev_info(dev,
				 "failed to get number of gpios for port%d\n",
				 i);
			pp->ngpio = 32;
		}

		pp->irq_shared	= false;
		pp->gpio_base	= -1;

=======
		if (fwnode_property_read_u32(fwnode, "ngpios", &pp->ngpio) &&
		    fwnode_property_read_u32(fwnode, "snps,nr-gpios", &pp->ngpio)) {
			dev_info(dev,
				 "failed to get number of gpios for port%d\n",
				 i);
			pp->ngpio = DWAPB_MAX_GPIOS;
		}

		pp->gpio_base	= -1;

		/* For internal use only, new platforms mustn't exercise this */
		if (is_software_node(fwnode))
			fwnode_property_read_u32(fwnode, "gpio-base", &pp->gpio_base);

>>>>>>> upstream/android-13
		/*
		 * Only port A can provide interrupts in all configurations of
		 * the IP.
		 */
<<<<<<< HEAD
		if (pp->idx != 0)
			continue;

		if (dev->of_node && fwnode_property_read_bool(fwnode,
						  "interrupt-controller")) {
			np = to_of_node(fwnode);
		}

		for (j = 0; j < pp->ngpio; j++) {
			pp->irq[j] = -ENXIO;

			if (np)
				pp->irq[j] = of_irq_get(np, j);
			else if (has_acpi_companion(dev))
				pp->irq[j] = platform_get_irq(to_platform_device(dev), j);

			if (pp->irq[j] >= 0)
				pp->has_irq = true;
		}

		if (!pp->has_irq)
			dev_warn(dev, "no irq for port%d\n", pp->idx);
=======
		if (pp->idx == 0)
			dwapb_get_irq(dev, fwnode, pp);
>>>>>>> upstream/android-13
	}

	return pdata;
}

<<<<<<< HEAD
=======
static void dwapb_assert_reset(void *data)
{
	struct dwapb_gpio *gpio = data;

	reset_control_assert(gpio->rst);
}

static int dwapb_get_reset(struct dwapb_gpio *gpio)
{
	int err;

	gpio->rst = devm_reset_control_get_optional_shared(gpio->dev, NULL);
	if (IS_ERR(gpio->rst))
		return dev_err_probe(gpio->dev, PTR_ERR(gpio->rst),
				     "Cannot get reset descriptor\n");

	err = reset_control_deassert(gpio->rst);
	if (err) {
		dev_err(gpio->dev, "Cannot deassert reset lane\n");
		return err;
	}

	return devm_add_action_or_reset(gpio->dev, dwapb_assert_reset, gpio);
}

static void dwapb_disable_clks(void *data)
{
	struct dwapb_gpio *gpio = data;

	clk_bulk_disable_unprepare(DWAPB_NR_CLOCKS, gpio->clks);
}

static int dwapb_get_clks(struct dwapb_gpio *gpio)
{
	int err;

	/* Optional bus and debounce clocks */
	gpio->clks[0].id = "bus";
	gpio->clks[1].id = "db";
	err = devm_clk_bulk_get_optional(gpio->dev, DWAPB_NR_CLOCKS,
					 gpio->clks);
	if (err) {
		dev_err(gpio->dev, "Cannot get APB/Debounce clocks\n");
		return err;
	}

	err = clk_bulk_prepare_enable(DWAPB_NR_CLOCKS, gpio->clks);
	if (err) {
		dev_err(gpio->dev, "Cannot enable APB/Debounce clocks\n");
		return err;
	}

	return devm_add_action_or_reset(gpio->dev, dwapb_disable_clks, gpio);
}

>>>>>>> upstream/android-13
static const struct of_device_id dwapb_of_match[] = {
	{ .compatible = "snps,dw-apb-gpio", .data = (void *)0},
	{ .compatible = "apm,xgene-gpio-v2", .data = (void *)GPIO_REG_OFFSET_V2},
	{ /* Sentinel */ }
};
MODULE_DEVICE_TABLE(of, dwapb_of_match);

static const struct acpi_device_id dwapb_acpi_match[] = {
	{"HISI0181", 0},
	{"APMC0D07", 0},
	{"APMC0D81", GPIO_REG_OFFSET_V2},
	{ }
};
MODULE_DEVICE_TABLE(acpi, dwapb_acpi_match);

static int dwapb_gpio_probe(struct platform_device *pdev)
{
	unsigned int i;
<<<<<<< HEAD
	struct resource *res;
	struct dwapb_gpio *gpio;
	int err;
	struct device *dev = &pdev->dev;
	struct dwapb_platform_data *pdata = dev_get_platdata(dev);

	if (!pdata) {
		pdata = dwapb_gpio_get_pdata(dev);
		if (IS_ERR(pdata))
			return PTR_ERR(pdata);
	}

	if (!pdata->nports)
		return -ENODEV;
=======
	struct dwapb_gpio *gpio;
	int err;
	struct dwapb_platform_data *pdata;
	struct device *dev = &pdev->dev;

	pdata = dwapb_gpio_get_pdata(dev);
	if (IS_ERR(pdata))
		return PTR_ERR(pdata);
>>>>>>> upstream/android-13

	gpio = devm_kzalloc(&pdev->dev, sizeof(*gpio), GFP_KERNEL);
	if (!gpio)
		return -ENOMEM;

	gpio->dev = &pdev->dev;
	gpio->nr_ports = pdata->nports;

<<<<<<< HEAD
	gpio->rst = devm_reset_control_get_optional_shared(dev, NULL);
	if (IS_ERR(gpio->rst))
		return PTR_ERR(gpio->rst);

	reset_control_deassert(gpio->rst);
=======
	err = dwapb_get_reset(gpio);
	if (err)
		return err;
>>>>>>> upstream/android-13

	gpio->ports = devm_kcalloc(&pdev->dev, gpio->nr_ports,
				   sizeof(*gpio->ports), GFP_KERNEL);
	if (!gpio->ports)
		return -ENOMEM;

<<<<<<< HEAD
	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	gpio->regs = devm_ioremap_resource(&pdev->dev, res);
	if (IS_ERR(gpio->regs))
		return PTR_ERR(gpio->regs);

	/* Optional bus clock */
	gpio->clk = devm_clk_get(&pdev->dev, "bus");
	if (!IS_ERR(gpio->clk)) {
		err = clk_prepare_enable(gpio->clk);
		if (err) {
			dev_info(&pdev->dev, "Cannot enable clock\n");
			return err;
		}
	}

	gpio->flags = 0;
	if (dev->of_node) {
		gpio->flags = (uintptr_t)of_device_get_match_data(dev);
	} else if (has_acpi_companion(dev)) {
		const struct acpi_device_id *acpi_id;

		acpi_id = acpi_match_device(dwapb_acpi_match, dev);
		if (acpi_id) {
			if (acpi_id->driver_data)
				gpio->flags = acpi_id->driver_data;
		}
	}
=======
	gpio->regs = devm_platform_ioremap_resource(pdev, 0);
	if (IS_ERR(gpio->regs))
		return PTR_ERR(gpio->regs);

	err = dwapb_get_clks(gpio);
	if (err)
		return err;

	gpio->flags = (uintptr_t)device_get_match_data(dev);
>>>>>>> upstream/android-13

	for (i = 0; i < gpio->nr_ports; i++) {
		err = dwapb_gpio_add_port(gpio, &pdata->properties[i], i);
		if (err)
<<<<<<< HEAD
			goto out_unregister;
	}
	platform_set_drvdata(pdev, gpio);

	return 0;

out_unregister:
	dwapb_gpio_unregister(gpio);
	dwapb_irq_teardown(gpio);
	clk_disable_unprepare(gpio->clk);

	return err;
}

static int dwapb_gpio_remove(struct platform_device *pdev)
{
	struct dwapb_gpio *gpio = platform_get_drvdata(pdev);

	dwapb_gpio_unregister(gpio);
	dwapb_irq_teardown(gpio);
	reset_control_assert(gpio->rst);
	clk_disable_unprepare(gpio->clk);

	return 0;
=======
			return err;
	}

	platform_set_drvdata(pdev, gpio);

	return 0;
>>>>>>> upstream/android-13
}

#ifdef CONFIG_PM_SLEEP
static int dwapb_gpio_suspend(struct device *dev)
{
<<<<<<< HEAD
	struct platform_device *pdev = to_platform_device(dev);
	struct dwapb_gpio *gpio = platform_get_drvdata(pdev);
=======
	struct dwapb_gpio *gpio = dev_get_drvdata(dev);
>>>>>>> upstream/android-13
	struct gpio_chip *gc	= &gpio->ports[0].gc;
	unsigned long flags;
	int i;

	spin_lock_irqsave(&gc->bgpio_lock, flags);
	for (i = 0; i < gpio->nr_ports; i++) {
		unsigned int offset;
		unsigned int idx = gpio->ports[i].idx;
		struct dwapb_context *ctx = gpio->ports[i].ctx;

<<<<<<< HEAD
		BUG_ON(!ctx);

=======
>>>>>>> upstream/android-13
		offset = GPIO_SWPORTA_DDR + idx * GPIO_SWPORT_DDR_STRIDE;
		ctx->dir = dwapb_read(gpio, offset);

		offset = GPIO_SWPORTA_DR + idx * GPIO_SWPORT_DR_STRIDE;
		ctx->data = dwapb_read(gpio, offset);

		offset = GPIO_EXT_PORTA + idx * GPIO_EXT_PORT_STRIDE;
		ctx->ext = dwapb_read(gpio, offset);

		/* Only port A can provide interrupts */
		if (idx == 0) {
			ctx->int_mask	= dwapb_read(gpio, GPIO_INTMASK);
			ctx->int_en	= dwapb_read(gpio, GPIO_INTEN);
			ctx->int_pol	= dwapb_read(gpio, GPIO_INT_POLARITY);
			ctx->int_type	= dwapb_read(gpio, GPIO_INTTYPE_LEVEL);
			ctx->int_deb	= dwapb_read(gpio, GPIO_PORTA_DEBOUNCE);

			/* Mask out interrupts */
<<<<<<< HEAD
			dwapb_write(gpio, GPIO_INTMASK,
				    0xffffffff & ~ctx->wake_en);
=======
			dwapb_write(gpio, GPIO_INTMASK, ~ctx->wake_en);
>>>>>>> upstream/android-13
		}
	}
	spin_unlock_irqrestore(&gc->bgpio_lock, flags);

<<<<<<< HEAD
	clk_disable_unprepare(gpio->clk);
=======
	clk_bulk_disable_unprepare(DWAPB_NR_CLOCKS, gpio->clks);
>>>>>>> upstream/android-13

	return 0;
}

static int dwapb_gpio_resume(struct device *dev)
{
<<<<<<< HEAD
	struct platform_device *pdev = to_platform_device(dev);
	struct dwapb_gpio *gpio = platform_get_drvdata(pdev);
	struct gpio_chip *gc	= &gpio->ports[0].gc;
	unsigned long flags;
	int i;

	if (!IS_ERR(gpio->clk))
		clk_prepare_enable(gpio->clk);
=======
	struct dwapb_gpio *gpio = dev_get_drvdata(dev);
	struct gpio_chip *gc	= &gpio->ports[0].gc;
	unsigned long flags;
	int i, err;

	err = clk_bulk_prepare_enable(DWAPB_NR_CLOCKS, gpio->clks);
	if (err) {
		dev_err(gpio->dev, "Cannot reenable APB/Debounce clocks\n");
		return err;
	}
>>>>>>> upstream/android-13

	spin_lock_irqsave(&gc->bgpio_lock, flags);
	for (i = 0; i < gpio->nr_ports; i++) {
		unsigned int offset;
		unsigned int idx = gpio->ports[i].idx;
		struct dwapb_context *ctx = gpio->ports[i].ctx;

<<<<<<< HEAD
		BUG_ON(!ctx);

=======
>>>>>>> upstream/android-13
		offset = GPIO_SWPORTA_DR + idx * GPIO_SWPORT_DR_STRIDE;
		dwapb_write(gpio, offset, ctx->data);

		offset = GPIO_SWPORTA_DDR + idx * GPIO_SWPORT_DDR_STRIDE;
		dwapb_write(gpio, offset, ctx->dir);

		offset = GPIO_EXT_PORTA + idx * GPIO_EXT_PORT_STRIDE;
		dwapb_write(gpio, offset, ctx->ext);

		/* Only port A can provide interrupts */
		if (idx == 0) {
			dwapb_write(gpio, GPIO_INTTYPE_LEVEL, ctx->int_type);
			dwapb_write(gpio, GPIO_INT_POLARITY, ctx->int_pol);
			dwapb_write(gpio, GPIO_PORTA_DEBOUNCE, ctx->int_deb);
			dwapb_write(gpio, GPIO_INTEN, ctx->int_en);
			dwapb_write(gpio, GPIO_INTMASK, ctx->int_mask);

			/* Clear out spurious interrupts */
			dwapb_write(gpio, GPIO_PORTA_EOI, 0xffffffff);
		}
	}
	spin_unlock_irqrestore(&gc->bgpio_lock, flags);

	return 0;
}
#endif

static SIMPLE_DEV_PM_OPS(dwapb_gpio_pm_ops, dwapb_gpio_suspend,
			 dwapb_gpio_resume);

static struct platform_driver dwapb_gpio_driver = {
	.driver		= {
		.name	= DWAPB_DRIVER_NAME,
		.pm	= &dwapb_gpio_pm_ops,
<<<<<<< HEAD
		.of_match_table = of_match_ptr(dwapb_of_match),
		.acpi_match_table = ACPI_PTR(dwapb_acpi_match),
	},
	.probe		= dwapb_gpio_probe,
	.remove		= dwapb_gpio_remove,
=======
		.of_match_table = dwapb_of_match,
		.acpi_match_table = dwapb_acpi_match,
	},
	.probe		= dwapb_gpio_probe,
>>>>>>> upstream/android-13
};

module_platform_driver(dwapb_gpio_driver);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Jamie Iles");
MODULE_DESCRIPTION("Synopsys DesignWare APB GPIO driver");
MODULE_ALIAS("platform:" DWAPB_DRIVER_NAME);
