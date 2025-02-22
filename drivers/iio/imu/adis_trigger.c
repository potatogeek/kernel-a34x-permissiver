<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> upstream/android-13
/*
 * Common library for ADIS16XXX devices
 *
 * Copyright 2012 Analog Devices Inc.
 *   Author: Lars-Peter Clausen <lars@metafoo.de>
<<<<<<< HEAD
 *
 * Licensed under the GPL-2 or later.
=======
>>>>>>> upstream/android-13
 */

#include <linux/interrupt.h>
#include <linux/kernel.h>
#include <linux/spi/spi.h>
#include <linux/export.h>

#include <linux/iio/iio.h>
#include <linux/iio/trigger.h>
#include <linux/iio/imu/adis.h>

static int adis_data_rdy_trigger_set_state(struct iio_trigger *trig,
						bool state)
{
	struct adis *adis = iio_trigger_get_drvdata(trig);

	return adis_enable_irq(adis, state);
}

static const struct iio_trigger_ops adis_trigger_ops = {
	.set_trigger_state = &adis_data_rdy_trigger_set_state,
};

<<<<<<< HEAD
/**
 * adis_probe_trigger() - Sets up trigger for a adis device
=======
static int adis_validate_irq_flag(struct adis *adis)
{
	unsigned long direction = adis->irq_flag & IRQF_TRIGGER_MASK;
	/*
	 * Typically this devices have data ready either on the rising edge or
	 * on the falling edge of the data ready pin. This checks enforces that
	 * one of those is set in the drivers... It defaults to
	 * IRQF_TRIGGER_RISING for backward compatibility with devices that
	 * don't support changing the pin polarity.
	 */
	if (direction == IRQF_TRIGGER_NONE) {
		adis->irq_flag |= IRQF_TRIGGER_RISING;
		return 0;
	} else if (direction != IRQF_TRIGGER_RISING &&
		   direction != IRQF_TRIGGER_FALLING) {
		dev_err(&adis->spi->dev, "Invalid IRQ mask: %08lx\n",
			adis->irq_flag);
		return -EINVAL;
	}

	return 0;
}

/**
 * devm_adis_probe_trigger() - Sets up trigger for a managed adis device
>>>>>>> upstream/android-13
 * @adis: The adis device
 * @indio_dev: The IIO device
 *
 * Returns 0 on success or a negative error code
<<<<<<< HEAD
 *
 * adis_remove_trigger() should be used to free the trigger.
 */
int adis_probe_trigger(struct adis *adis, struct iio_dev *indio_dev)
{
	int ret;

	adis->trig = iio_trigger_alloc("%s-dev%d", indio_dev->name,
					indio_dev->id);
	if (adis->trig == NULL)
		return -ENOMEM;

	adis->trig->dev.parent = &adis->spi->dev;
	adis->trig->ops = &adis_trigger_ops;
	iio_trigger_set_drvdata(adis->trig, adis);

	ret = request_irq(adis->spi->irq,
			  &iio_trigger_generic_data_rdy_poll,
			  IRQF_TRIGGER_RISING,
			  indio_dev->name,
			  adis->trig);
	if (ret)
		goto error_free_trig;

	ret = iio_trigger_register(adis->trig);

	indio_dev->trig = iio_trigger_get(adis->trig);
	if (ret)
		goto error_free_irq;

	return 0;

error_free_irq:
	free_irq(adis->spi->irq, adis->trig);
error_free_trig:
	iio_trigger_free(adis->trig);
	return ret;
}
EXPORT_SYMBOL_GPL(adis_probe_trigger);

/**
 * adis_remove_trigger() - Remove trigger for a adis devices
 * @adis: The adis device
 *
 * Removes the trigger previously registered with adis_probe_trigger().
 */
void adis_remove_trigger(struct adis *adis)
{
	iio_trigger_unregister(adis->trig);
	free_irq(adis->spi->irq, adis->trig);
	iio_trigger_free(adis->trig);
}
EXPORT_SYMBOL_GPL(adis_remove_trigger);
=======
 */
int devm_adis_probe_trigger(struct adis *adis, struct iio_dev *indio_dev)
{
	int ret;

	adis->trig = devm_iio_trigger_alloc(&adis->spi->dev, "%s-dev%d",
					    indio_dev->name,
					    iio_device_id(indio_dev));
	if (!adis->trig)
		return -ENOMEM;

	adis->trig->ops = &adis_trigger_ops;
	iio_trigger_set_drvdata(adis->trig, adis);

	ret = adis_validate_irq_flag(adis);
	if (ret)
		return ret;

	ret = devm_request_irq(&adis->spi->dev, adis->spi->irq,
			       &iio_trigger_generic_data_rdy_poll,
			       adis->irq_flag,
			       indio_dev->name,
			       adis->trig);
	if (ret)
		return ret;

	return devm_iio_trigger_register(&adis->spi->dev, adis->trig);
}
EXPORT_SYMBOL_GPL(devm_adis_probe_trigger);

>>>>>>> upstream/android-13
