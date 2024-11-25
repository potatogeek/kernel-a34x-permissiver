<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> upstream/android-13
/*
 * Driver for PCA9685 16-channel 12-bit PWM LED controller
 *
 * Copyright (C) 2013 Steffen Trumtrar <s.trumtrar@pengutronix.de>
 * Copyright (C) 2015 Clemens Gruber <clemens.gruber@pqgruber.com>
 *
 * based on the pwm-twl-led.c driver
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published by
 * the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
=======
>>>>>>> upstream/android-13
 */

#include <linux/acpi.h>
#include <linux/gpio/driver.h>
#include <linux/i2c.h>
#include <linux/module.h>
#include <linux/mutex.h>
#include <linux/platform_device.h>
#include <linux/property.h>
#include <linux/pwm.h>
#include <linux/regmap.h>
#include <linux/slab.h>
#include <linux/delay.h>
#include <linux/pm_runtime.h>
#include <linux/bitmap.h>

/*
<<<<<<< HEAD
 * Because the PCA9685 has only one prescaler per chip, changing the period of
 * one channel affects the period of all 16 PWM outputs!
 * However, the ratio between each configured duty cycle and the chip-wide
 * period remains constant, because the OFF time is set in proportion to the
 * counter range.
=======
 * Because the PCA9685 has only one prescaler per chip, only the first channel
 * that is enabled is allowed to change the prescale register.
 * PWM channels requested afterwards must use a period that results in the same
 * prescale setting as the one set by the first requested channel.
 * GPIOs do not count as enabled PWMs as they are not using the prescaler.
>>>>>>> upstream/android-13
 */

#define PCA9685_MODE1		0x00
#define PCA9685_MODE2		0x01
#define PCA9685_SUBADDR1	0x02
#define PCA9685_SUBADDR2	0x03
#define PCA9685_SUBADDR3	0x04
#define PCA9685_ALLCALLADDR	0x05
#define PCA9685_LEDX_ON_L	0x06
#define PCA9685_LEDX_ON_H	0x07
#define PCA9685_LEDX_OFF_L	0x08
#define PCA9685_LEDX_OFF_H	0x09

#define PCA9685_ALL_LED_ON_L	0xFA
#define PCA9685_ALL_LED_ON_H	0xFB
#define PCA9685_ALL_LED_OFF_L	0xFC
#define PCA9685_ALL_LED_OFF_H	0xFD
#define PCA9685_PRESCALE	0xFE

#define PCA9685_PRESCALE_MIN	0x03	/* => max. frequency of 1526 Hz */
#define PCA9685_PRESCALE_MAX	0xFF	/* => min. frequency of 24 Hz */

#define PCA9685_COUNTER_RANGE	4096
<<<<<<< HEAD
#define PCA9685_DEFAULT_PERIOD	5000000	/* Default period_ns = 1/200 Hz */
=======
>>>>>>> upstream/android-13
#define PCA9685_OSC_CLOCK_MHZ	25	/* Internal oscillator with 25 MHz */

#define PCA9685_NUMREGS		0xFF
#define PCA9685_MAXCHAN		0x10

<<<<<<< HEAD
#define LED_FULL		(1 << 4)
#define MODE1_SLEEP		(1 << 4)
#define MODE2_INVRT		(1 << 4)
#define MODE2_OUTDRV		(1 << 2)
=======
#define LED_FULL		BIT(4)
#define MODE1_ALLCALL		BIT(0)
#define MODE1_SUB3		BIT(1)
#define MODE1_SUB2		BIT(2)
#define MODE1_SUB1		BIT(3)
#define MODE1_SLEEP		BIT(4)
#define MODE2_INVRT		BIT(4)
#define MODE2_OUTDRV		BIT(2)
>>>>>>> upstream/android-13

#define LED_N_ON_H(N)	(PCA9685_LEDX_ON_H + (4 * (N)))
#define LED_N_ON_L(N)	(PCA9685_LEDX_ON_L + (4 * (N)))
#define LED_N_OFF_H(N)	(PCA9685_LEDX_OFF_H + (4 * (N)))
#define LED_N_OFF_L(N)	(PCA9685_LEDX_OFF_L + (4 * (N)))

<<<<<<< HEAD
struct pca9685 {
	struct pwm_chip chip;
	struct regmap *regmap;
	int duty_ns;
	int period_ns;
#if IS_ENABLED(CONFIG_GPIOLIB)
	struct mutex lock;
=======
#define REG_ON_H(C)	((C) >= PCA9685_MAXCHAN ? PCA9685_ALL_LED_ON_H : LED_N_ON_H((C)))
#define REG_ON_L(C)	((C) >= PCA9685_MAXCHAN ? PCA9685_ALL_LED_ON_L : LED_N_ON_L((C)))
#define REG_OFF_H(C)	((C) >= PCA9685_MAXCHAN ? PCA9685_ALL_LED_OFF_H : LED_N_OFF_H((C)))
#define REG_OFF_L(C)	((C) >= PCA9685_MAXCHAN ? PCA9685_ALL_LED_OFF_L : LED_N_OFF_L((C)))

struct pca9685 {
	struct pwm_chip chip;
	struct regmap *regmap;
	struct mutex lock;
	DECLARE_BITMAP(pwms_enabled, PCA9685_MAXCHAN + 1);
#if IS_ENABLED(CONFIG_GPIOLIB)
>>>>>>> upstream/android-13
	struct gpio_chip gpio;
	DECLARE_BITMAP(pwms_inuse, PCA9685_MAXCHAN + 1);
#endif
};

static inline struct pca9685 *to_pca(struct pwm_chip *chip)
{
	return container_of(chip, struct pca9685, chip);
}

<<<<<<< HEAD
=======
/* This function is supposed to be called with the lock mutex held */
static bool pca9685_prescaler_can_change(struct pca9685 *pca, int channel)
{
	/* No PWM enabled: Change allowed */
	if (bitmap_empty(pca->pwms_enabled, PCA9685_MAXCHAN + 1))
		return true;
	/* More than one PWM enabled: Change not allowed */
	if (bitmap_weight(pca->pwms_enabled, PCA9685_MAXCHAN + 1) > 1)
		return false;
	/*
	 * Only one PWM enabled: Change allowed if the PWM about to
	 * be changed is the one that is already enabled
	 */
	return test_bit(channel, pca->pwms_enabled);
}

static int pca9685_read_reg(struct pca9685 *pca, unsigned int reg, unsigned int *val)
{
	struct device *dev = pca->chip.dev;
	int err;

	err = regmap_read(pca->regmap, reg, val);
	if (err)
		dev_err(dev, "regmap_read of register 0x%x failed: %pe\n", reg, ERR_PTR(err));

	return err;
}

static int pca9685_write_reg(struct pca9685 *pca, unsigned int reg, unsigned int val)
{
	struct device *dev = pca->chip.dev;
	int err;

	err = regmap_write(pca->regmap, reg, val);
	if (err)
		dev_err(dev, "regmap_write to register 0x%x failed: %pe\n", reg, ERR_PTR(err));

	return err;
}

/* Helper function to set the duty cycle ratio to duty/4096 (e.g. duty=2048 -> 50%) */
static void pca9685_pwm_set_duty(struct pca9685 *pca, int channel, unsigned int duty)
{
	struct pwm_device *pwm = &pca->chip.pwms[channel];
	unsigned int on, off;

	if (duty == 0) {
		/* Set the full OFF bit, which has the highest precedence */
		pca9685_write_reg(pca, REG_OFF_H(channel), LED_FULL);
		return;
	} else if (duty >= PCA9685_COUNTER_RANGE) {
		/* Set the full ON bit and clear the full OFF bit */
		pca9685_write_reg(pca, REG_ON_H(channel), LED_FULL);
		pca9685_write_reg(pca, REG_OFF_H(channel), 0);
		return;
	}


	if (pwm->state.usage_power && channel < PCA9685_MAXCHAN) {
		/*
		 * If usage_power is set, the pca9685 driver will phase shift
		 * the individual channels relative to their channel number.
		 * This improves EMI because the enabled channels no longer
		 * turn on at the same time, while still maintaining the
		 * configured duty cycle / power output.
		 */
		on = channel * PCA9685_COUNTER_RANGE / PCA9685_MAXCHAN;
	} else
		on = 0;

	off = (on + duty) % PCA9685_COUNTER_RANGE;

	/* Set ON time (clears full ON bit) */
	pca9685_write_reg(pca, REG_ON_L(channel), on & 0xff);
	pca9685_write_reg(pca, REG_ON_H(channel), (on >> 8) & 0xf);
	/* Set OFF time (clears full OFF bit) */
	pca9685_write_reg(pca, REG_OFF_L(channel), off & 0xff);
	pca9685_write_reg(pca, REG_OFF_H(channel), (off >> 8) & 0xf);
}

static unsigned int pca9685_pwm_get_duty(struct pca9685 *pca, int channel)
{
	struct pwm_device *pwm = &pca->chip.pwms[channel];
	unsigned int off = 0, on = 0, val = 0;

	if (WARN_ON(channel >= PCA9685_MAXCHAN)) {
		/* HW does not support reading state of "all LEDs" channel */
		return 0;
	}

	pca9685_read_reg(pca, LED_N_OFF_H(channel), &off);
	if (off & LED_FULL) {
		/* Full OFF bit is set */
		return 0;
	}

	pca9685_read_reg(pca, LED_N_ON_H(channel), &on);
	if (on & LED_FULL) {
		/* Full ON bit is set */
		return PCA9685_COUNTER_RANGE;
	}

	pca9685_read_reg(pca, LED_N_OFF_L(channel), &val);
	off = ((off & 0xf) << 8) | (val & 0xff);
	if (!pwm->state.usage_power)
		return off;

	/* Read ON register to calculate duty cycle of staggered output */
	if (pca9685_read_reg(pca, LED_N_ON_L(channel), &val)) {
		/* Reset val to 0 in case reading LED_N_ON_L failed */
		val = 0;
	}
	on = ((on & 0xf) << 8) | (val & 0xff);
	return (off - on) & (PCA9685_COUNTER_RANGE - 1);
}

>>>>>>> upstream/android-13
#if IS_ENABLED(CONFIG_GPIOLIB)
static bool pca9685_pwm_test_and_set_inuse(struct pca9685 *pca, int pwm_idx)
{
	bool is_inuse;

	mutex_lock(&pca->lock);
	if (pwm_idx >= PCA9685_MAXCHAN) {
		/*
<<<<<<< HEAD
		 * "all LEDs" channel:
=======
		 * "All LEDs" channel:
>>>>>>> upstream/android-13
		 * pretend already in use if any of the PWMs are requested
		 */
		if (!bitmap_empty(pca->pwms_inuse, PCA9685_MAXCHAN)) {
			is_inuse = true;
			goto out;
		}
	} else {
		/*
<<<<<<< HEAD
		 * regular channel:
=======
		 * Regular channel:
>>>>>>> upstream/android-13
		 * pretend already in use if the "all LEDs" channel is requested
		 */
		if (test_bit(PCA9685_MAXCHAN, pca->pwms_inuse)) {
			is_inuse = true;
			goto out;
		}
	}
	is_inuse = test_and_set_bit(pwm_idx, pca->pwms_inuse);
out:
	mutex_unlock(&pca->lock);
	return is_inuse;
}

static void pca9685_pwm_clear_inuse(struct pca9685 *pca, int pwm_idx)
{
	mutex_lock(&pca->lock);
	clear_bit(pwm_idx, pca->pwms_inuse);
	mutex_unlock(&pca->lock);
}

static int pca9685_pwm_gpio_request(struct gpio_chip *gpio, unsigned int offset)
{
	struct pca9685 *pca = gpiochip_get_data(gpio);

	if (pca9685_pwm_test_and_set_inuse(pca, offset))
		return -EBUSY;
	pm_runtime_get_sync(pca->chip.dev);
	return 0;
}

static int pca9685_pwm_gpio_get(struct gpio_chip *gpio, unsigned int offset)
{
	struct pca9685 *pca = gpiochip_get_data(gpio);
<<<<<<< HEAD
	struct pwm_device *pwm = &pca->chip.pwms[offset];
	unsigned int value;

	regmap_read(pca->regmap, LED_N_ON_H(pwm->hwpwm), &value);

	return value & LED_FULL;
=======

	return pca9685_pwm_get_duty(pca, offset) != 0;
>>>>>>> upstream/android-13
}

static void pca9685_pwm_gpio_set(struct gpio_chip *gpio, unsigned int offset,
				 int value)
{
	struct pca9685 *pca = gpiochip_get_data(gpio);
<<<<<<< HEAD
	struct pwm_device *pwm = &pca->chip.pwms[offset];
	unsigned int on = value ? LED_FULL : 0;

	/* Clear both OFF registers */
	regmap_write(pca->regmap, LED_N_OFF_L(pwm->hwpwm), 0);
	regmap_write(pca->regmap, LED_N_OFF_H(pwm->hwpwm), 0);

	/* Set the full ON bit */
	regmap_write(pca->regmap, LED_N_ON_H(pwm->hwpwm), on);
=======

	pca9685_pwm_set_duty(pca, offset, value ? PCA9685_COUNTER_RANGE : 0);
>>>>>>> upstream/android-13
}

static void pca9685_pwm_gpio_free(struct gpio_chip *gpio, unsigned int offset)
{
	struct pca9685 *pca = gpiochip_get_data(gpio);

<<<<<<< HEAD
	pca9685_pwm_gpio_set(gpio, offset, 0);
=======
	pca9685_pwm_set_duty(pca, offset, 0);
>>>>>>> upstream/android-13
	pm_runtime_put(pca->chip.dev);
	pca9685_pwm_clear_inuse(pca, offset);
}

static int pca9685_pwm_gpio_get_direction(struct gpio_chip *chip,
					  unsigned int offset)
{
	/* Always out */
<<<<<<< HEAD
	return 0;
=======
	return GPIO_LINE_DIRECTION_OUT;
>>>>>>> upstream/android-13
}

static int pca9685_pwm_gpio_direction_input(struct gpio_chip *gpio,
					    unsigned int offset)
{
	return -EINVAL;
}

static int pca9685_pwm_gpio_direction_output(struct gpio_chip *gpio,
					     unsigned int offset, int value)
{
	pca9685_pwm_gpio_set(gpio, offset, value);

	return 0;
}

/*
 * The PCA9685 has a bit for turning the PWM output full off or on. Some
 * boards like Intel Galileo actually uses these as normal GPIOs so we
 * expose a GPIO chip here which can exclusively take over the underlying
 * PWM channel.
 */
static int pca9685_pwm_gpio_probe(struct pca9685 *pca)
{
	struct device *dev = pca->chip.dev;

<<<<<<< HEAD
	mutex_init(&pca->lock);

=======
>>>>>>> upstream/android-13
	pca->gpio.label = dev_name(dev);
	pca->gpio.parent = dev;
	pca->gpio.request = pca9685_pwm_gpio_request;
	pca->gpio.free = pca9685_pwm_gpio_free;
	pca->gpio.get_direction = pca9685_pwm_gpio_get_direction;
	pca->gpio.direction_input = pca9685_pwm_gpio_direction_input;
	pca->gpio.direction_output = pca9685_pwm_gpio_direction_output;
	pca->gpio.get = pca9685_pwm_gpio_get;
	pca->gpio.set = pca9685_pwm_gpio_set;
	pca->gpio.base = -1;
	pca->gpio.ngpio = PCA9685_MAXCHAN;
	pca->gpio.can_sleep = true;

	return devm_gpiochip_add_data(dev, &pca->gpio, pca);
}
#else
static inline bool pca9685_pwm_test_and_set_inuse(struct pca9685 *pca,
						  int pwm_idx)
{
	return false;
}

static inline void
pca9685_pwm_clear_inuse(struct pca9685 *pca, int pwm_idx)
{
}

static inline int pca9685_pwm_gpio_probe(struct pca9685 *pca)
{
	return 0;
}
#endif

static void pca9685_set_sleep_mode(struct pca9685 *pca, bool enable)
{
<<<<<<< HEAD
	regmap_update_bits(pca->regmap, PCA9685_MODE1,
			   MODE1_SLEEP, enable ? MODE1_SLEEP : 0);
=======
	struct device *dev = pca->chip.dev;
	int err = regmap_update_bits(pca->regmap, PCA9685_MODE1,
				     MODE1_SLEEP, enable ? MODE1_SLEEP : 0);
	if (err) {
		dev_err(dev, "regmap_update_bits of register 0x%x failed: %pe\n",
			PCA9685_MODE1, ERR_PTR(err));
		return;
	}

>>>>>>> upstream/android-13
	if (!enable) {
		/* Wait 500us for the oscillator to be back up */
		udelay(500);
	}
}

<<<<<<< HEAD
static int pca9685_pwm_config(struct pwm_chip *chip, struct pwm_device *pwm,
			      int duty_ns, int period_ns)
{
	struct pca9685 *pca = to_pca(chip);
	unsigned long long duty;
	unsigned int reg;
	int prescale;

	if (period_ns != pca->period_ns) {
		prescale = DIV_ROUND_CLOSEST(PCA9685_OSC_CLOCK_MHZ * period_ns,
					     PCA9685_COUNTER_RANGE * 1000) - 1;

		if (prescale >= PCA9685_PRESCALE_MIN &&
			prescale <= PCA9685_PRESCALE_MAX) {
			/*
			 * putting the chip briefly into SLEEP mode
			 * at this point won't interfere with the
			 * pm_runtime framework, because the pm_runtime
			 * state is guaranteed active here.
			 */
			/* Put chip into sleep mode */
			pca9685_set_sleep_mode(pca, true);

			/* Change the chip-wide output frequency */
			regmap_write(pca->regmap, PCA9685_PRESCALE, prescale);

			/* Wake the chip up */
			pca9685_set_sleep_mode(pca, false);

			pca->period_ns = period_ns;
		} else {
			dev_err(chip->dev,
				"prescaler not set: period out of bounds!\n");
			return -EINVAL;
		}
	}

	pca->duty_ns = duty_ns;

	if (duty_ns < 1) {
		if (pwm->hwpwm >= PCA9685_MAXCHAN)
			reg = PCA9685_ALL_LED_OFF_H;
		else
			reg = LED_N_OFF_H(pwm->hwpwm);

		regmap_write(pca->regmap, reg, LED_FULL);

		return 0;
	}

	if (duty_ns == period_ns) {
		/* Clear both OFF registers */
		if (pwm->hwpwm >= PCA9685_MAXCHAN)
			reg = PCA9685_ALL_LED_OFF_L;
		else
			reg = LED_N_OFF_L(pwm->hwpwm);

		regmap_write(pca->regmap, reg, 0x0);

		if (pwm->hwpwm >= PCA9685_MAXCHAN)
			reg = PCA9685_ALL_LED_OFF_H;
		else
			reg = LED_N_OFF_H(pwm->hwpwm);

		regmap_write(pca->regmap, reg, 0x0);

		/* Set the full ON bit */
		if (pwm->hwpwm >= PCA9685_MAXCHAN)
			reg = PCA9685_ALL_LED_ON_H;
		else
			reg = LED_N_ON_H(pwm->hwpwm);

		regmap_write(pca->regmap, reg, LED_FULL);

		return 0;
	}

	duty = PCA9685_COUNTER_RANGE * (unsigned long long)duty_ns;
	duty = DIV_ROUND_UP_ULL(duty, period_ns);

	if (pwm->hwpwm >= PCA9685_MAXCHAN)
		reg = PCA9685_ALL_LED_OFF_L;
	else
		reg = LED_N_OFF_L(pwm->hwpwm);

	regmap_write(pca->regmap, reg, (int)duty & 0xff);

	if (pwm->hwpwm >= PCA9685_MAXCHAN)
		reg = PCA9685_ALL_LED_OFF_H;
	else
		reg = LED_N_OFF_H(pwm->hwpwm);

	regmap_write(pca->regmap, reg, ((int)duty >> 8) & 0xf);

	/* Clear the full ON bit, otherwise the set OFF time has no effect */
	if (pwm->hwpwm >= PCA9685_MAXCHAN)
		reg = PCA9685_ALL_LED_ON_H;
	else
		reg = LED_N_ON_H(pwm->hwpwm);

	regmap_write(pca->regmap, reg, 0);

	return 0;
}

static int pca9685_pwm_enable(struct pwm_chip *chip, struct pwm_device *pwm)
{
	struct pca9685 *pca = to_pca(chip);
	unsigned int reg;

	/*
	 * The PWM subsystem does not support a pre-delay.
	 * So, set the ON-timeout to 0
	 */
	if (pwm->hwpwm >= PCA9685_MAXCHAN)
		reg = PCA9685_ALL_LED_ON_L;
	else
		reg = LED_N_ON_L(pwm->hwpwm);

	regmap_write(pca->regmap, reg, 0);

	if (pwm->hwpwm >= PCA9685_MAXCHAN)
		reg = PCA9685_ALL_LED_ON_H;
	else
		reg = LED_N_ON_H(pwm->hwpwm);

	regmap_write(pca->regmap, reg, 0);

	/*
	 * Clear the full-off bit.
	 * It has precedence over the others and must be off.
	 */
	if (pwm->hwpwm >= PCA9685_MAXCHAN)
		reg = PCA9685_ALL_LED_OFF_H;
	else
		reg = LED_N_OFF_H(pwm->hwpwm);

	regmap_update_bits(pca->regmap, reg, LED_FULL, 0x0);

	return 0;
}

static void pca9685_pwm_disable(struct pwm_chip *chip, struct pwm_device *pwm)
{
	struct pca9685 *pca = to_pca(chip);
	unsigned int reg;

	if (pwm->hwpwm >= PCA9685_MAXCHAN)
		reg = PCA9685_ALL_LED_OFF_H;
	else
		reg = LED_N_OFF_H(pwm->hwpwm);

	regmap_write(pca->regmap, reg, LED_FULL);

	/* Clear the LED_OFF counter. */
	if (pwm->hwpwm >= PCA9685_MAXCHAN)
		reg = PCA9685_ALL_LED_OFF_L;
	else
		reg = LED_N_OFF_L(pwm->hwpwm);

	regmap_write(pca->regmap, reg, 0x0);
=======
static int __pca9685_pwm_apply(struct pwm_chip *chip, struct pwm_device *pwm,
			       const struct pwm_state *state)
{
	struct pca9685 *pca = to_pca(chip);
	unsigned long long duty, prescale;
	unsigned int val = 0;

	if (state->polarity != PWM_POLARITY_NORMAL)
		return -EINVAL;

	prescale = DIV_ROUND_CLOSEST_ULL(PCA9685_OSC_CLOCK_MHZ * state->period,
					 PCA9685_COUNTER_RANGE * 1000) - 1;
	if (prescale < PCA9685_PRESCALE_MIN || prescale > PCA9685_PRESCALE_MAX) {
		dev_err(chip->dev, "pwm not changed: period out of bounds!\n");
		return -EINVAL;
	}

	if (!state->enabled) {
		pca9685_pwm_set_duty(pca, pwm->hwpwm, 0);
		return 0;
	}

	pca9685_read_reg(pca, PCA9685_PRESCALE, &val);
	if (prescale != val) {
		if (!pca9685_prescaler_can_change(pca, pwm->hwpwm)) {
			dev_err(chip->dev,
				"pwm not changed: periods of enabled pwms must match!\n");
			return -EBUSY;
		}

		/*
		 * Putting the chip briefly into SLEEP mode
		 * at this point won't interfere with the
		 * pm_runtime framework, because the pm_runtime
		 * state is guaranteed active here.
		 */
		/* Put chip into sleep mode */
		pca9685_set_sleep_mode(pca, true);

		/* Change the chip-wide output frequency */
		pca9685_write_reg(pca, PCA9685_PRESCALE, prescale);

		/* Wake the chip up */
		pca9685_set_sleep_mode(pca, false);
	}

	duty = PCA9685_COUNTER_RANGE * state->duty_cycle;
	duty = DIV_ROUND_UP_ULL(duty, state->period);
	pca9685_pwm_set_duty(pca, pwm->hwpwm, duty);
	return 0;
}

static int pca9685_pwm_apply(struct pwm_chip *chip, struct pwm_device *pwm,
			     const struct pwm_state *state)
{
	struct pca9685 *pca = to_pca(chip);
	int ret;

	mutex_lock(&pca->lock);
	ret = __pca9685_pwm_apply(chip, pwm, state);
	if (ret == 0) {
		if (state->enabled)
			set_bit(pwm->hwpwm, pca->pwms_enabled);
		else
			clear_bit(pwm->hwpwm, pca->pwms_enabled);
	}
	mutex_unlock(&pca->lock);

	return ret;
}

static void pca9685_pwm_get_state(struct pwm_chip *chip, struct pwm_device *pwm,
				  struct pwm_state *state)
{
	struct pca9685 *pca = to_pca(chip);
	unsigned long long duty;
	unsigned int val = 0;

	/* Calculate (chip-wide) period from prescale value */
	pca9685_read_reg(pca, PCA9685_PRESCALE, &val);
	/*
	 * PCA9685_OSC_CLOCK_MHZ is 25, i.e. an integer divider of 1000.
	 * The following calculation is therefore only a multiplication
	 * and we are not losing precision.
	 */
	state->period = (PCA9685_COUNTER_RANGE * 1000 / PCA9685_OSC_CLOCK_MHZ) *
			(val + 1);

	/* The (per-channel) polarity is fixed */
	state->polarity = PWM_POLARITY_NORMAL;

	if (pwm->hwpwm >= PCA9685_MAXCHAN) {
		/*
		 * The "all LEDs" channel does not support HW readout
		 * Return 0 and disabled for backwards compatibility
		 */
		state->duty_cycle = 0;
		state->enabled = false;
		return;
	}

	state->enabled = true;
	duty = pca9685_pwm_get_duty(pca, pwm->hwpwm);
	state->duty_cycle = DIV_ROUND_DOWN_ULL(duty * state->period, PCA9685_COUNTER_RANGE);
>>>>>>> upstream/android-13
}

static int pca9685_pwm_request(struct pwm_chip *chip, struct pwm_device *pwm)
{
	struct pca9685 *pca = to_pca(chip);

	if (pca9685_pwm_test_and_set_inuse(pca, pwm->hwpwm))
		return -EBUSY;
<<<<<<< HEAD
=======

	if (pwm->hwpwm < PCA9685_MAXCHAN) {
		/* PWMs - except the "all LEDs" channel - default to enabled */
		mutex_lock(&pca->lock);
		set_bit(pwm->hwpwm, pca->pwms_enabled);
		mutex_unlock(&pca->lock);
	}

>>>>>>> upstream/android-13
	pm_runtime_get_sync(chip->dev);

	return 0;
}

static void pca9685_pwm_free(struct pwm_chip *chip, struct pwm_device *pwm)
{
	struct pca9685 *pca = to_pca(chip);

<<<<<<< HEAD
	pca9685_pwm_disable(chip, pwm);
=======
	mutex_lock(&pca->lock);
	pca9685_pwm_set_duty(pca, pwm->hwpwm, 0);
	clear_bit(pwm->hwpwm, pca->pwms_enabled);
	mutex_unlock(&pca->lock);

>>>>>>> upstream/android-13
	pm_runtime_put(chip->dev);
	pca9685_pwm_clear_inuse(pca, pwm->hwpwm);
}

static const struct pwm_ops pca9685_pwm_ops = {
<<<<<<< HEAD
	.enable = pca9685_pwm_enable,
	.disable = pca9685_pwm_disable,
	.config = pca9685_pwm_config,
=======
	.apply = pca9685_pwm_apply,
	.get_state = pca9685_pwm_get_state,
>>>>>>> upstream/android-13
	.request = pca9685_pwm_request,
	.free = pca9685_pwm_free,
	.owner = THIS_MODULE,
};

static const struct regmap_config pca9685_regmap_i2c_config = {
	.reg_bits = 8,
	.val_bits = 8,
	.max_register = PCA9685_NUMREGS,
	.cache_type = REGCACHE_NONE,
};

static int pca9685_pwm_probe(struct i2c_client *client,
				const struct i2c_device_id *id)
{
	struct pca9685 *pca;
<<<<<<< HEAD
	int ret;
	int mode2;
=======
	unsigned int reg;
	int ret;
>>>>>>> upstream/android-13

	pca = devm_kzalloc(&client->dev, sizeof(*pca), GFP_KERNEL);
	if (!pca)
		return -ENOMEM;

	pca->regmap = devm_regmap_init_i2c(client, &pca9685_regmap_i2c_config);
	if (IS_ERR(pca->regmap)) {
		ret = PTR_ERR(pca->regmap);
		dev_err(&client->dev, "Failed to initialize register map: %d\n",
			ret);
		return ret;
	}
<<<<<<< HEAD
	pca->duty_ns = 0;
	pca->period_ns = PCA9685_DEFAULT_PERIOD;

	i2c_set_clientdata(client, pca);

	regmap_read(pca->regmap, PCA9685_MODE2, &mode2);

	if (device_property_read_bool(&client->dev, "invert"))
		mode2 |= MODE2_INVRT;
	else
		mode2 &= ~MODE2_INVRT;

	if (device_property_read_bool(&client->dev, "open-drain"))
		mode2 &= ~MODE2_OUTDRV;
	else
		mode2 |= MODE2_OUTDRV;

	regmap_write(pca->regmap, PCA9685_MODE2, mode2);

	/* clear all "full off" bits */
	regmap_write(pca->regmap, PCA9685_ALL_LED_OFF_L, 0);
	regmap_write(pca->regmap, PCA9685_ALL_LED_OFF_H, 0);

	pca->chip.ops = &pca9685_pwm_ops;
	/* add an extra channel for ALL_LED */
	pca->chip.npwm = PCA9685_MAXCHAN + 1;

	pca->chip.dev = &client->dev;
	pca->chip.base = -1;
=======

	i2c_set_clientdata(client, pca);

	mutex_init(&pca->lock);

	ret = pca9685_read_reg(pca, PCA9685_MODE2, &reg);
	if (ret)
		return ret;

	if (device_property_read_bool(&client->dev, "invert"))
		reg |= MODE2_INVRT;
	else
		reg &= ~MODE2_INVRT;

	if (device_property_read_bool(&client->dev, "open-drain"))
		reg &= ~MODE2_OUTDRV;
	else
		reg |= MODE2_OUTDRV;

	ret = pca9685_write_reg(pca, PCA9685_MODE2, reg);
	if (ret)
		return ret;

	/* Disable all LED ALLCALL and SUBx addresses to avoid bus collisions */
	pca9685_read_reg(pca, PCA9685_MODE1, &reg);
	reg &= ~(MODE1_ALLCALL | MODE1_SUB1 | MODE1_SUB2 | MODE1_SUB3);
	pca9685_write_reg(pca, PCA9685_MODE1, reg);

	/* Reset OFF/ON registers to POR default */
	pca9685_write_reg(pca, PCA9685_ALL_LED_OFF_L, LED_FULL);
	pca9685_write_reg(pca, PCA9685_ALL_LED_OFF_H, LED_FULL);
	pca9685_write_reg(pca, PCA9685_ALL_LED_ON_L, 0);
	pca9685_write_reg(pca, PCA9685_ALL_LED_ON_H, 0);

	pca->chip.ops = &pca9685_pwm_ops;
	/* Add an extra channel for ALL_LED */
	pca->chip.npwm = PCA9685_MAXCHAN + 1;

	pca->chip.dev = &client->dev;
>>>>>>> upstream/android-13

	ret = pwmchip_add(&pca->chip);
	if (ret < 0)
		return ret;

	ret = pca9685_pwm_gpio_probe(pca);
	if (ret < 0) {
		pwmchip_remove(&pca->chip);
		return ret;
	}

<<<<<<< HEAD
	/* the chip comes out of power-up in the active state */
	pm_runtime_set_active(&client->dev);
	/*
	 * enable will put the chip into suspend, which is what we
	 * want as all outputs are disabled at this point
	 */
	pm_runtime_enable(&client->dev);

=======
	pm_runtime_enable(&client->dev);

	if (pm_runtime_enabled(&client->dev)) {
		/*
		 * Although the chip comes out of power-up in the sleep state,
		 * we force it to sleep in case it was woken up before
		 */
		pca9685_set_sleep_mode(pca, true);
		pm_runtime_set_suspended(&client->dev);
	} else {
		/* Wake the chip up if runtime PM is disabled */
		pca9685_set_sleep_mode(pca, false);
	}

>>>>>>> upstream/android-13
	return 0;
}

static int pca9685_pwm_remove(struct i2c_client *client)
{
	struct pca9685 *pca = i2c_get_clientdata(client);
<<<<<<< HEAD
	int ret;

	ret = pwmchip_remove(&pca->chip);
	if (ret)
		return ret;
	pm_runtime_disable(&client->dev);
	return 0;
}

#ifdef CONFIG_PM
static int pca9685_pwm_runtime_suspend(struct device *dev)
=======

	pwmchip_remove(&pca->chip);

	if (!pm_runtime_enabled(&client->dev)) {
		/* Put chip in sleep state if runtime PM is disabled */
		pca9685_set_sleep_mode(pca, true);
	}

	pm_runtime_disable(&client->dev);

	return 0;
}

static int __maybe_unused pca9685_pwm_runtime_suspend(struct device *dev)
>>>>>>> upstream/android-13
{
	struct i2c_client *client = to_i2c_client(dev);
	struct pca9685 *pca = i2c_get_clientdata(client);

	pca9685_set_sleep_mode(pca, true);
	return 0;
}

<<<<<<< HEAD
static int pca9685_pwm_runtime_resume(struct device *dev)
=======
static int __maybe_unused pca9685_pwm_runtime_resume(struct device *dev)
>>>>>>> upstream/android-13
{
	struct i2c_client *client = to_i2c_client(dev);
	struct pca9685 *pca = i2c_get_clientdata(client);

	pca9685_set_sleep_mode(pca, false);
	return 0;
}
<<<<<<< HEAD
#endif
=======
>>>>>>> upstream/android-13

static const struct i2c_device_id pca9685_id[] = {
	{ "pca9685", 0 },
	{ /* sentinel */ },
};
MODULE_DEVICE_TABLE(i2c, pca9685_id);

#ifdef CONFIG_ACPI
static const struct acpi_device_id pca9685_acpi_ids[] = {
	{ "INT3492", 0 },
	{ /* sentinel */ },
};
MODULE_DEVICE_TABLE(acpi, pca9685_acpi_ids);
#endif

#ifdef CONFIG_OF
static const struct of_device_id pca9685_dt_ids[] = {
	{ .compatible = "nxp,pca9685-pwm", },
	{ /* sentinel */ }
};
MODULE_DEVICE_TABLE(of, pca9685_dt_ids);
#endif

static const struct dev_pm_ops pca9685_pwm_pm = {
	SET_RUNTIME_PM_OPS(pca9685_pwm_runtime_suspend,
			   pca9685_pwm_runtime_resume, NULL)
};

static struct i2c_driver pca9685_i2c_driver = {
	.driver = {
		.name = "pca9685-pwm",
		.acpi_match_table = ACPI_PTR(pca9685_acpi_ids),
		.of_match_table = of_match_ptr(pca9685_dt_ids),
		.pm = &pca9685_pwm_pm,
	},
	.probe = pca9685_pwm_probe,
	.remove = pca9685_pwm_remove,
	.id_table = pca9685_id,
};

module_i2c_driver(pca9685_i2c_driver);

MODULE_AUTHOR("Steffen Trumtrar <s.trumtrar@pengutronix.de>");
MODULE_DESCRIPTION("PWM driver for PCA9685");
MODULE_LICENSE("GPL");
