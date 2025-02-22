// SPDX-License-Identifier: GPL-2.0
/*
 * GPIO Greybus driver.
 *
 * Copyright 2014 Google Inc.
 * Copyright 2014 Linaro Ltd.
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/slab.h>
<<<<<<< HEAD
#include <linux/gpio.h>
#include <linux/irq.h>
#include <linux/irqdomain.h>
#include <linux/mutex.h>

#include "greybus.h"
=======
#include <linux/irq.h>
#include <linux/irqdomain.h>
#include <linux/gpio/driver.h>
#include <linux/mutex.h>
#include <linux/greybus.h>

>>>>>>> upstream/android-13
#include "gbphy.h"

struct gb_gpio_line {
	/* The following has to be an array of line_max entries */
	/* --> make them just a flags field */
	u8			active:    1,
				direction: 1,	/* 0 = output, 1 = input */
				value:     1;	/* 0 = low, 1 = high */
	u16			debounce_usec;

	u8			irq_type;
	bool			irq_type_pending;
	bool			masked;
	bool			masked_pending;
};

struct gb_gpio_controller {
	struct gbphy_device	*gbphy_dev;
	struct gb_connection	*connection;
	u8			line_max;	/* max line number */
	struct gb_gpio_line	*lines;

	struct gpio_chip	chip;
	struct irq_chip		irqc;
<<<<<<< HEAD
	struct irq_chip		*irqchip;
	struct irq_domain	*irqdomain;
	unsigned int		irq_base;
	irq_flow_handler_t	irq_handler;
	unsigned int		irq_default_type;
=======
>>>>>>> upstream/android-13
	struct mutex		irq_lock;
};
#define gpio_chip_to_gb_gpio_controller(chip) \
	container_of(chip, struct gb_gpio_controller, chip)
#define irq_data_to_gpio_chip(d) (d->domain->host_data)

static int gb_gpio_line_count_operation(struct gb_gpio_controller *ggc)
{
	struct gb_gpio_line_count_response response;
	int ret;

	ret = gb_operation_sync(ggc->connection, GB_GPIO_TYPE_LINE_COUNT,
				NULL, 0, &response, sizeof(response));
	if (!ret)
		ggc->line_max = response.count;
	return ret;
}

static int gb_gpio_activate_operation(struct gb_gpio_controller *ggc, u8 which)
{
	struct gb_gpio_activate_request request;
	struct gbphy_device *gbphy_dev = ggc->gbphy_dev;
	int ret;

	ret = gbphy_runtime_get_sync(gbphy_dev);
	if (ret)
		return ret;

	request.which = which;
	ret = gb_operation_sync(ggc->connection, GB_GPIO_TYPE_ACTIVATE,
<<<<<<< HEAD
				 &request, sizeof(request), NULL, 0);
=======
				&request, sizeof(request), NULL, 0);
>>>>>>> upstream/android-13
	if (ret) {
		gbphy_runtime_put_autosuspend(gbphy_dev);
		return ret;
	}

	ggc->lines[which].active = true;

	return 0;
}

static void gb_gpio_deactivate_operation(struct gb_gpio_controller *ggc,
<<<<<<< HEAD
					u8 which)
=======
					 u8 which)
>>>>>>> upstream/android-13
{
	struct gbphy_device *gbphy_dev = ggc->gbphy_dev;
	struct device *dev = &gbphy_dev->dev;
	struct gb_gpio_deactivate_request request;
	int ret;

	request.which = which;
	ret = gb_operation_sync(ggc->connection, GB_GPIO_TYPE_DEACTIVATE,
<<<<<<< HEAD
				 &request, sizeof(request), NULL, 0);
=======
				&request, sizeof(request), NULL, 0);
>>>>>>> upstream/android-13
	if (ret) {
		dev_err(dev, "failed to deactivate gpio %u\n", which);
		goto out_pm_put;
	}

	ggc->lines[which].active = false;

out_pm_put:
	gbphy_runtime_put_autosuspend(gbphy_dev);
}

static int gb_gpio_get_direction_operation(struct gb_gpio_controller *ggc,
<<<<<<< HEAD
					u8 which)
=======
					   u8 which)
>>>>>>> upstream/android-13
{
	struct device *dev = &ggc->gbphy_dev->dev;
	struct gb_gpio_get_direction_request request;
	struct gb_gpio_get_direction_response response;
	int ret;
	u8 direction;

	request.which = which;
	ret = gb_operation_sync(ggc->connection, GB_GPIO_TYPE_GET_DIRECTION,
				&request, sizeof(request),
				&response, sizeof(response));
	if (ret)
		return ret;

	direction = response.direction;
	if (direction && direction != 1) {
		dev_warn(dev, "gpio %u direction was %u (should be 0 or 1)\n",
			 which, direction);
	}
	ggc->lines[which].direction = direction ? 1 : 0;
	return 0;
}

static int gb_gpio_direction_in_operation(struct gb_gpio_controller *ggc,
<<<<<<< HEAD
					u8 which)
=======
					  u8 which)
>>>>>>> upstream/android-13
{
	struct gb_gpio_direction_in_request request;
	int ret;

	request.which = which;
	ret = gb_operation_sync(ggc->connection, GB_GPIO_TYPE_DIRECTION_IN,
				&request, sizeof(request), NULL, 0);
	if (!ret)
		ggc->lines[which].direction = 1;
	return ret;
}

static int gb_gpio_direction_out_operation(struct gb_gpio_controller *ggc,
<<<<<<< HEAD
					u8 which, bool value_high)
=======
					   u8 which, bool value_high)
>>>>>>> upstream/android-13
{
	struct gb_gpio_direction_out_request request;
	int ret;

	request.which = which;
	request.value = value_high ? 1 : 0;
	ret = gb_operation_sync(ggc->connection, GB_GPIO_TYPE_DIRECTION_OUT,
				&request, sizeof(request), NULL, 0);
	if (!ret)
		ggc->lines[which].direction = 0;
	return ret;
}

static int gb_gpio_get_value_operation(struct gb_gpio_controller *ggc,
<<<<<<< HEAD
					u8 which)
=======
				       u8 which)
>>>>>>> upstream/android-13
{
	struct device *dev = &ggc->gbphy_dev->dev;
	struct gb_gpio_get_value_request request;
	struct gb_gpio_get_value_response response;
	int ret;
	u8 value;

	request.which = which;
	ret = gb_operation_sync(ggc->connection, GB_GPIO_TYPE_GET_VALUE,
				&request, sizeof(request),
				&response, sizeof(response));
	if (ret) {
		dev_err(dev, "failed to get value of gpio %u\n", which);
		return ret;
	}

	value = response.value;
	if (value && value != 1) {
		dev_warn(dev, "gpio %u value was %u (should be 0 or 1)\n",
			 which, value);
	}
	ggc->lines[which].value = value ? 1 : 0;
	return 0;
}

static void gb_gpio_set_value_operation(struct gb_gpio_controller *ggc,
					u8 which, bool value_high)
{
	struct device *dev = &ggc->gbphy_dev->dev;
	struct gb_gpio_set_value_request request;
	int ret;

	if (ggc->lines[which].direction == 1) {
		dev_warn(dev, "refusing to set value of input gpio %u\n",
			 which);
		return;
	}

	request.which = which;
	request.value = value_high ? 1 : 0;
	ret = gb_operation_sync(ggc->connection, GB_GPIO_TYPE_SET_VALUE,
				&request, sizeof(request), NULL, 0);
	if (ret) {
		dev_err(dev, "failed to set value of gpio %u\n", which);
		return;
	}

	ggc->lines[which].value = request.value;
}

static int gb_gpio_set_debounce_operation(struct gb_gpio_controller *ggc,
<<<<<<< HEAD
					u8 which, u16 debounce_usec)
=======
					  u8 which, u16 debounce_usec)
>>>>>>> upstream/android-13
{
	struct gb_gpio_set_debounce_request request;
	int ret;

	request.which = which;
	request.usec = cpu_to_le16(debounce_usec);
	ret = gb_operation_sync(ggc->connection, GB_GPIO_TYPE_SET_DEBOUNCE,
				&request, sizeof(request), NULL, 0);
	if (!ret)
		ggc->lines[which].debounce_usec = debounce_usec;
	return ret;
}

static void _gb_gpio_irq_mask(struct gb_gpio_controller *ggc, u8 hwirq)
{
	struct device *dev = &ggc->gbphy_dev->dev;
	struct gb_gpio_irq_mask_request request;
	int ret;

	request.which = hwirq;
	ret = gb_operation_sync(ggc->connection,
				GB_GPIO_TYPE_IRQ_MASK,
				&request, sizeof(request), NULL, 0);
	if (ret)
		dev_err(dev, "failed to mask irq: %d\n", ret);
}

static void _gb_gpio_irq_unmask(struct gb_gpio_controller *ggc, u8 hwirq)
{
	struct device *dev = &ggc->gbphy_dev->dev;
	struct gb_gpio_irq_unmask_request request;
	int ret;

	request.which = hwirq;
	ret = gb_operation_sync(ggc->connection,
				GB_GPIO_TYPE_IRQ_UNMASK,
				&request, sizeof(request), NULL, 0);
	if (ret)
		dev_err(dev, "failed to unmask irq: %d\n", ret);
}

static void _gb_gpio_irq_set_type(struct gb_gpio_controller *ggc,
<<<<<<< HEAD
					u8 hwirq, u8 type)
=======
				  u8 hwirq, u8 type)
>>>>>>> upstream/android-13
{
	struct device *dev = &ggc->gbphy_dev->dev;
	struct gb_gpio_irq_type_request request;
	int ret;

	request.which = hwirq;
	request.type = type;

	ret = gb_operation_sync(ggc->connection,
				GB_GPIO_TYPE_IRQ_TYPE,
				&request, sizeof(request), NULL, 0);
	if (ret)
		dev_err(dev, "failed to set irq type: %d\n", ret);
}

static void gb_gpio_irq_mask(struct irq_data *d)
{
	struct gpio_chip *chip = irq_data_to_gpio_chip(d);
	struct gb_gpio_controller *ggc = gpio_chip_to_gb_gpio_controller(chip);
	struct gb_gpio_line *line = &ggc->lines[d->hwirq];

	line->masked = true;
	line->masked_pending = true;
}

static void gb_gpio_irq_unmask(struct irq_data *d)
{
	struct gpio_chip *chip = irq_data_to_gpio_chip(d);
	struct gb_gpio_controller *ggc = gpio_chip_to_gb_gpio_controller(chip);
	struct gb_gpio_line *line = &ggc->lines[d->hwirq];

	line->masked = false;
	line->masked_pending = true;
}

static int gb_gpio_irq_set_type(struct irq_data *d, unsigned int type)
{
	struct gpio_chip *chip = irq_data_to_gpio_chip(d);
	struct gb_gpio_controller *ggc = gpio_chip_to_gb_gpio_controller(chip);
	struct gb_gpio_line *line = &ggc->lines[d->hwirq];
	struct device *dev = &ggc->gbphy_dev->dev;
	u8 irq_type;

	switch (type) {
	case IRQ_TYPE_NONE:
		irq_type = GB_GPIO_IRQ_TYPE_NONE;
		break;
	case IRQ_TYPE_EDGE_RISING:
		irq_type = GB_GPIO_IRQ_TYPE_EDGE_RISING;
		break;
	case IRQ_TYPE_EDGE_FALLING:
		irq_type = GB_GPIO_IRQ_TYPE_EDGE_FALLING;
		break;
	case IRQ_TYPE_EDGE_BOTH:
		irq_type = GB_GPIO_IRQ_TYPE_EDGE_BOTH;
		break;
	case IRQ_TYPE_LEVEL_LOW:
		irq_type = GB_GPIO_IRQ_TYPE_LEVEL_LOW;
		break;
	case IRQ_TYPE_LEVEL_HIGH:
		irq_type = GB_GPIO_IRQ_TYPE_LEVEL_HIGH;
		break;
	default:
		dev_err(dev, "unsupported irq type: %u\n", type);
		return -EINVAL;
	}

	line->irq_type = irq_type;
	line->irq_type_pending = true;

	return 0;
}

static void gb_gpio_irq_bus_lock(struct irq_data *d)
{
	struct gpio_chip *chip = irq_data_to_gpio_chip(d);
	struct gb_gpio_controller *ggc = gpio_chip_to_gb_gpio_controller(chip);

	mutex_lock(&ggc->irq_lock);
}

static void gb_gpio_irq_bus_sync_unlock(struct irq_data *d)
{
	struct gpio_chip *chip = irq_data_to_gpio_chip(d);
	struct gb_gpio_controller *ggc = gpio_chip_to_gb_gpio_controller(chip);
	struct gb_gpio_line *line = &ggc->lines[d->hwirq];

	if (line->irq_type_pending) {
		_gb_gpio_irq_set_type(ggc, d->hwirq, line->irq_type);
		line->irq_type_pending = false;
	}

	if (line->masked_pending) {
		if (line->masked)
			_gb_gpio_irq_mask(ggc, d->hwirq);
		else
			_gb_gpio_irq_unmask(ggc, d->hwirq);
		line->masked_pending = false;
	}

	mutex_unlock(&ggc->irq_lock);
}

static int gb_gpio_request_handler(struct gb_operation *op)
{
	struct gb_connection *connection = op->connection;
	struct gb_gpio_controller *ggc = gb_connection_get_data(connection);
	struct device *dev = &ggc->gbphy_dev->dev;
	struct gb_message *request;
	struct gb_gpio_irq_event_request *event;
	u8 type = op->type;
<<<<<<< HEAD
	int irq;
	struct irq_desc *desc;
=======
	int irq, ret;
>>>>>>> upstream/android-13

	if (type != GB_GPIO_TYPE_IRQ_EVENT) {
		dev_err(dev, "unsupported unsolicited request: %u\n", type);
		return -EINVAL;
	}

	request = op->request;

	if (request->payload_size < sizeof(*event)) {
		dev_err(dev, "short event received (%zu < %zu)\n",
			request->payload_size, sizeof(*event));
		return -EINVAL;
	}

	event = request->payload;
	if (event->which > ggc->line_max) {
		dev_err(dev, "invalid hw irq: %d\n", event->which);
		return -EINVAL;
	}

<<<<<<< HEAD
	irq = irq_find_mapping(ggc->irqdomain, event->which);
=======
	irq = irq_find_mapping(ggc->chip.irq.domain, event->which);
>>>>>>> upstream/android-13
	if (!irq) {
		dev_err(dev, "failed to find IRQ\n");
		return -EINVAL;
	}
<<<<<<< HEAD
	desc = irq_to_desc(irq);
	if (!desc) {
		dev_err(dev, "failed to look up irq\n");
		return -EINVAL;
	}

	local_irq_disable();
	generic_handle_irq_desc(desc);
	local_irq_enable();

	return 0;
=======

	local_irq_disable();
	ret = generic_handle_irq(irq);
	local_irq_enable();

	if (ret)
		dev_err(dev, "failed to invoke irq handler\n");

	return ret;
>>>>>>> upstream/android-13
}

static int gb_gpio_request(struct gpio_chip *chip, unsigned int offset)
{
	struct gb_gpio_controller *ggc = gpio_chip_to_gb_gpio_controller(chip);

	return gb_gpio_activate_operation(ggc, (u8)offset);
}

static void gb_gpio_free(struct gpio_chip *chip, unsigned int offset)
{
	struct gb_gpio_controller *ggc = gpio_chip_to_gb_gpio_controller(chip);

	gb_gpio_deactivate_operation(ggc, (u8)offset);
}

static int gb_gpio_get_direction(struct gpio_chip *chip, unsigned int offset)
{
	struct gb_gpio_controller *ggc = gpio_chip_to_gb_gpio_controller(chip);
	u8 which;
	int ret;

	which = (u8)offset;
	ret = gb_gpio_get_direction_operation(ggc, which);
	if (ret)
		return ret;

	return ggc->lines[which].direction ? 1 : 0;
}

static int gb_gpio_direction_input(struct gpio_chip *chip, unsigned int offset)
{
	struct gb_gpio_controller *ggc = gpio_chip_to_gb_gpio_controller(chip);

	return gb_gpio_direction_in_operation(ggc, (u8)offset);
}

static int gb_gpio_direction_output(struct gpio_chip *chip, unsigned int offset,
				    int value)
{
	struct gb_gpio_controller *ggc = gpio_chip_to_gb_gpio_controller(chip);

	return gb_gpio_direction_out_operation(ggc, (u8)offset, !!value);
}

static int gb_gpio_get(struct gpio_chip *chip, unsigned int offset)
{
	struct gb_gpio_controller *ggc = gpio_chip_to_gb_gpio_controller(chip);
	u8 which;
	int ret;

	which = (u8)offset;
	ret = gb_gpio_get_value_operation(ggc, which);
	if (ret)
		return ret;

	return ggc->lines[which].value;
}

static void gb_gpio_set(struct gpio_chip *chip, unsigned int offset, int value)
{
	struct gb_gpio_controller *ggc = gpio_chip_to_gb_gpio_controller(chip);

	gb_gpio_set_value_operation(ggc, (u8)offset, !!value);
}

static int gb_gpio_set_config(struct gpio_chip *chip, unsigned int offset,
			      unsigned long config)
{
	struct gb_gpio_controller *ggc = gpio_chip_to_gb_gpio_controller(chip);
	u32 debounce;

	if (pinconf_to_config_param(config) != PIN_CONFIG_INPUT_DEBOUNCE)
		return -ENOTSUPP;

	debounce = pinconf_to_config_argument(config);
	if (debounce > U16_MAX)
		return -EINVAL;

	return gb_gpio_set_debounce_operation(ggc, (u8)offset, (u16)debounce);
}

static int gb_gpio_controller_setup(struct gb_gpio_controller *ggc)
{
	int ret;

	/* Now find out how many lines there are */
	ret = gb_gpio_line_count_operation(ggc);
	if (ret)
		return ret;

	ggc->lines = kcalloc(ggc->line_max + 1, sizeof(*ggc->lines),
			     GFP_KERNEL);
	if (!ggc->lines)
		return -ENOMEM;

	return ret;
}

<<<<<<< HEAD
/**
 * gb_gpio_irq_map() - maps an IRQ into a GB gpio irqchip
 * @d: the irqdomain used by this irqchip
 * @irq: the global irq number used by this GB gpio irqchip irq
 * @hwirq: the local IRQ/GPIO line offset on this GB gpio
 *
 * This function will set up the mapping for a certain IRQ line on a
 * GB gpio by assigning the GB gpio as chip data, and using the irqchip
 * stored inside the GB gpio.
 */
static int gb_gpio_irq_map(struct irq_domain *domain, unsigned int irq,
			   irq_hw_number_t hwirq)
{
	struct gpio_chip *chip = domain->host_data;
	struct gb_gpio_controller *ggc = gpio_chip_to_gb_gpio_controller(chip);

	irq_set_chip_data(irq, ggc);
	irq_set_chip_and_handler(irq, ggc->irqchip, ggc->irq_handler);
	irq_set_noprobe(irq);
	/*
	 * No set-up of the hardware will happen if IRQ_TYPE_NONE
	 * is passed as default type.
	 */
	if (ggc->irq_default_type != IRQ_TYPE_NONE)
		irq_set_irq_type(irq, ggc->irq_default_type);

	return 0;
}

static void gb_gpio_irq_unmap(struct irq_domain *d, unsigned int irq)
{
	irq_set_chip_and_handler(irq, NULL, NULL);
	irq_set_chip_data(irq, NULL);
}

static const struct irq_domain_ops gb_gpio_domain_ops = {
	.map	= gb_gpio_irq_map,
	.unmap	= gb_gpio_irq_unmap,
};

/**
 * gb_gpio_irqchip_remove() - removes an irqchip added to a gb_gpio_controller
 * @ggc: the gb_gpio_controller to remove the irqchip from
 *
 * This is called only from gb_gpio_remove()
 */
static void gb_gpio_irqchip_remove(struct gb_gpio_controller *ggc)
{
	unsigned int offset;

	/* Remove all IRQ mappings and delete the domain */
	if (ggc->irqdomain) {
		for (offset = 0; offset < (ggc->line_max + 1); offset++)
			irq_dispose_mapping(irq_find_mapping(ggc->irqdomain,
							     offset));
		irq_domain_remove(ggc->irqdomain);
	}

	if (ggc->irqchip)
		ggc->irqchip = NULL;
}

/**
 * gb_gpio_irqchip_add() - adds an irqchip to a gpio chip
 * @chip: the gpio chip to add the irqchip to
 * @irqchip: the irqchip to add to the adapter
 * @first_irq: if not dynamically assigned, the base (first) IRQ to
 * allocate gpio irqs from
 * @handler: the irq handler to use (often a predefined irq core function)
 * @type: the default type for IRQs on this irqchip, pass IRQ_TYPE_NONE
 * to have the core avoid setting up any default type in the hardware.
 *
 * This function closely associates a certain irqchip with a certain
 * gpio chip, providing an irq domain to translate the local IRQs to
 * global irqs, and making sure that the gpio chip
 * is passed as chip data to all related functions. Driver callbacks
 * need to use container_of() to get their local state containers back
 * from the gpio chip passed as chip data. An irqdomain will be stored
 * in the gpio chip that shall be used by the driver to handle IRQ number
 * translation. The gpio chip will need to be initialized and registered
 * before calling this function.
 */
static int gb_gpio_irqchip_add(struct gpio_chip *chip,
			 struct irq_chip *irqchip,
			 unsigned int first_irq,
			 irq_flow_handler_t handler,
			 unsigned int type)
{
	struct gb_gpio_controller *ggc;
	unsigned int offset;
	unsigned int irq_base;

	if (!chip || !irqchip)
		return -EINVAL;

	ggc = gpio_chip_to_gb_gpio_controller(chip);

	ggc->irqchip = irqchip;
	ggc->irq_handler = handler;
	ggc->irq_default_type = type;
	ggc->irqdomain = irq_domain_add_simple(NULL,
					ggc->line_max + 1, first_irq,
					&gb_gpio_domain_ops, chip);
	if (!ggc->irqdomain) {
		ggc->irqchip = NULL;
		return -EINVAL;
	}

	/*
	 * Prepare the mapping since the irqchip shall be orthogonal to
	 * any gpio calls. If the first_irq was zero, this is
	 * necessary to allocate descriptors for all IRQs.
	 */
	for (offset = 0; offset < (ggc->line_max + 1); offset++) {
		irq_base = irq_create_mapping(ggc->irqdomain, offset);
		if (offset == 0)
			ggc->irq_base = irq_base;
	}

	return 0;
}

static int gb_gpio_to_irq(struct gpio_chip *chip, unsigned int offset)
{
	struct gb_gpio_controller *ggc = gpio_chip_to_gb_gpio_controller(chip);

	return irq_find_mapping(ggc->irqdomain, offset);
}

=======
>>>>>>> upstream/android-13
static int gb_gpio_probe(struct gbphy_device *gbphy_dev,
			 const struct gbphy_device_id *id)
{
	struct gb_connection *connection;
	struct gb_gpio_controller *ggc;
	struct gpio_chip *gpio;
<<<<<<< HEAD
=======
	struct gpio_irq_chip *girq;
>>>>>>> upstream/android-13
	struct irq_chip *irqc;
	int ret;

	ggc = kzalloc(sizeof(*ggc), GFP_KERNEL);
	if (!ggc)
		return -ENOMEM;

<<<<<<< HEAD
	connection = gb_connection_create(gbphy_dev->bundle,
					  le16_to_cpu(gbphy_dev->cport_desc->id),
					  gb_gpio_request_handler);
=======
	connection =
		gb_connection_create(gbphy_dev->bundle,
				     le16_to_cpu(gbphy_dev->cport_desc->id),
				     gb_gpio_request_handler);
>>>>>>> upstream/android-13
	if (IS_ERR(connection)) {
		ret = PTR_ERR(connection);
		goto exit_ggc_free;
	}

	ggc->connection = connection;
	gb_connection_set_data(connection, ggc);
	ggc->gbphy_dev = gbphy_dev;
	gb_gbphy_set_data(gbphy_dev, ggc);

	ret = gb_connection_enable_tx(connection);
	if (ret)
		goto exit_connection_destroy;

	ret = gb_gpio_controller_setup(ggc);
	if (ret)
		goto exit_connection_disable;

	irqc = &ggc->irqc;
	irqc->irq_mask = gb_gpio_irq_mask;
	irqc->irq_unmask = gb_gpio_irq_unmask;
	irqc->irq_set_type = gb_gpio_irq_set_type;
	irqc->irq_bus_lock = gb_gpio_irq_bus_lock;
	irqc->irq_bus_sync_unlock = gb_gpio_irq_bus_sync_unlock;
	irqc->name = "greybus_gpio";

	mutex_init(&ggc->irq_lock);

	gpio = &ggc->chip;

	gpio->label = "greybus_gpio";
	gpio->parent = &gbphy_dev->dev;
	gpio->owner = THIS_MODULE;

	gpio->request = gb_gpio_request;
	gpio->free = gb_gpio_free;
	gpio->get_direction = gb_gpio_get_direction;
	gpio->direction_input = gb_gpio_direction_input;
	gpio->direction_output = gb_gpio_direction_output;
	gpio->get = gb_gpio_get;
	gpio->set = gb_gpio_set;
	gpio->set_config = gb_gpio_set_config;
<<<<<<< HEAD
	gpio->to_irq = gb_gpio_to_irq;
=======
>>>>>>> upstream/android-13
	gpio->base = -1;		/* Allocate base dynamically */
	gpio->ngpio = ggc->line_max + 1;
	gpio->can_sleep = true;

<<<<<<< HEAD
=======
	girq = &gpio->irq;
	girq->chip = irqc;
	/* The event comes from the outside so no parent handler */
	girq->parent_handler = NULL;
	girq->num_parents = 0;
	girq->parents = NULL;
	girq->default_type = IRQ_TYPE_NONE;
	girq->handler = handle_level_irq;

>>>>>>> upstream/android-13
	ret = gb_connection_enable(connection);
	if (ret)
		goto exit_line_free;

<<<<<<< HEAD
	ret = gb_gpio_irqchip_add(gpio, irqc, 0,
				   handle_level_irq, IRQ_TYPE_NONE);
	if (ret) {
		dev_err(&gbphy_dev->dev, "failed to add irq chip: %d\n", ret);
		goto exit_line_free;
	}

	ret = gpiochip_add(gpio);
	if (ret) {
		dev_err(&gbphy_dev->dev, "failed to add gpio chip: %d\n", ret);
		goto exit_gpio_irqchip_remove;
=======
	ret = gpiochip_add(gpio);
	if (ret) {
		dev_err(&gbphy_dev->dev, "failed to add gpio chip: %d\n", ret);
		goto exit_line_free;
>>>>>>> upstream/android-13
	}

	gbphy_runtime_put_autosuspend(gbphy_dev);
	return 0;

<<<<<<< HEAD
exit_gpio_irqchip_remove:
	gb_gpio_irqchip_remove(ggc);
=======
>>>>>>> upstream/android-13
exit_line_free:
	kfree(ggc->lines);
exit_connection_disable:
	gb_connection_disable(connection);
exit_connection_destroy:
	gb_connection_destroy(connection);
exit_ggc_free:
	kfree(ggc);
	return ret;
}

static void gb_gpio_remove(struct gbphy_device *gbphy_dev)
{
	struct gb_gpio_controller *ggc = gb_gbphy_get_data(gbphy_dev);
	struct gb_connection *connection = ggc->connection;
	int ret;

	ret = gbphy_runtime_get_sync(gbphy_dev);
	if (ret)
		gbphy_runtime_get_noresume(gbphy_dev);

	gb_connection_disable_rx(connection);
	gpiochip_remove(&ggc->chip);
<<<<<<< HEAD
	gb_gpio_irqchip_remove(ggc);
=======
>>>>>>> upstream/android-13
	gb_connection_disable(connection);
	gb_connection_destroy(connection);
	kfree(ggc->lines);
	kfree(ggc);
}

static const struct gbphy_device_id gb_gpio_id_table[] = {
	{ GBPHY_PROTOCOL(GREYBUS_PROTOCOL_GPIO) },
	{ },
};
MODULE_DEVICE_TABLE(gbphy, gb_gpio_id_table);

static struct gbphy_driver gpio_driver = {
	.name		= "gpio",
	.probe		= gb_gpio_probe,
	.remove		= gb_gpio_remove,
	.id_table	= gb_gpio_id_table,
};

module_gbphy_driver(gpio_driver);
MODULE_LICENSE("GPL v2");
