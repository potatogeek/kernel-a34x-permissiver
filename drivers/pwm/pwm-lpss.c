<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> upstream/android-13
/*
 * Intel Low Power Subsystem PWM controller driver
 *
 * Copyright (C) 2014, Intel Corporation
 * Author: Mika Westerberg <mika.westerberg@linux.intel.com>
 * Author: Chew Kean Ho <kean.ho.chew@intel.com>
 * Author: Chang Rebecca Swee Fun <rebecca.swee.fun.chang@intel.com>
 * Author: Chew Chiau Ee <chiau.ee.chew@intel.com>
 * Author: Alan Cox <alan@linux.intel.com>
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
=======
>>>>>>> upstream/android-13
 */

#include <linux/delay.h>
#include <linux/io.h>
#include <linux/iopoll.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/pm_runtime.h>
#include <linux/time.h>

#include "pwm-lpss.h"

#define PWM				0x00000000
#define PWM_ENABLE			BIT(31)
#define PWM_SW_UPDATE			BIT(30)
#define PWM_BASE_UNIT_SHIFT		8
#define PWM_ON_TIME_DIV_MASK		0x000000ff

/* Size of each PWM register space if multiple */
#define PWM_SIZE			0x400

<<<<<<< HEAD
#define MAX_PWMS			4

struct pwm_lpss_chip {
	struct pwm_chip chip;
	void __iomem *regs;
	const struct pwm_lpss_boardinfo *info;
	u32 saved_ctrl[MAX_PWMS];
};

=======
>>>>>>> upstream/android-13
static inline struct pwm_lpss_chip *to_lpwm(struct pwm_chip *chip)
{
	return container_of(chip, struct pwm_lpss_chip, chip);
}

static inline u32 pwm_lpss_read(const struct pwm_device *pwm)
{
	struct pwm_lpss_chip *lpwm = to_lpwm(pwm->chip);

	return readl(lpwm->regs + pwm->hwpwm * PWM_SIZE + PWM);
}

static inline void pwm_lpss_write(const struct pwm_device *pwm, u32 value)
{
	struct pwm_lpss_chip *lpwm = to_lpwm(pwm->chip);

	writel(value, lpwm->regs + pwm->hwpwm * PWM_SIZE + PWM);
}

static int pwm_lpss_wait_for_update(struct pwm_device *pwm)
{
	struct pwm_lpss_chip *lpwm = to_lpwm(pwm->chip);
	const void __iomem *addr = lpwm->regs + pwm->hwpwm * PWM_SIZE + PWM;
	const unsigned int ms = 500 * USEC_PER_MSEC;
	u32 val;
	int err;

	/*
	 * PWM Configuration register has SW_UPDATE bit that is set when a new
	 * configuration is written to the register. The bit is automatically
	 * cleared at the start of the next output cycle by the IP block.
	 *
	 * If one writes a new configuration to the register while it still has
	 * the bit enabled, PWM may freeze. That is, while one can still write
	 * to the register, it won't have an effect. Thus, we try to sleep long
	 * enough that the bit gets cleared and make sure the bit is not
	 * enabled while we update the configuration.
	 */
	err = readl_poll_timeout(addr, val, !(val & PWM_SW_UPDATE), 40, ms);
	if (err)
		dev_err(pwm->chip->dev, "PWM_SW_UPDATE was not cleared\n");

	return err;
}

static inline int pwm_lpss_is_updating(struct pwm_device *pwm)
{
<<<<<<< HEAD
	return (pwm_lpss_read(pwm) & PWM_SW_UPDATE) ? -EBUSY : 0;
=======
	if (pwm_lpss_read(pwm) & PWM_SW_UPDATE) {
		dev_err(pwm->chip->dev, "PWM_SW_UPDATE is still set, skipping update\n");
		return -EBUSY;
	}

	return 0;
>>>>>>> upstream/android-13
}

static void pwm_lpss_prepare(struct pwm_lpss_chip *lpwm, struct pwm_device *pwm,
			     int duty_ns, int period_ns)
{
	unsigned long long on_time_div;
	unsigned long c = lpwm->info->clk_rate, base_unit_range;
	unsigned long long base_unit, freq = NSEC_PER_SEC;
<<<<<<< HEAD
	u32 orig_ctrl, ctrl;
=======
	u32 ctrl;
>>>>>>> upstream/android-13

	do_div(freq, period_ns);

	/*
	 * The equation is:
	 * base_unit = round(base_unit_range * freq / c)
	 */
	base_unit_range = BIT(lpwm->info->base_unit_bits);
	freq *= base_unit_range;

	base_unit = DIV_ROUND_CLOSEST_ULL(freq, c);
	/* base_unit must not be 0 and we also want to avoid overflowing it */
	base_unit = clamp_val(base_unit, 1, base_unit_range - 1);

	on_time_div = 255ULL * duty_ns;
	do_div(on_time_div, period_ns);
	on_time_div = 255ULL - on_time_div;

<<<<<<< HEAD
	orig_ctrl = ctrl = pwm_lpss_read(pwm);
=======
	ctrl = pwm_lpss_read(pwm);
>>>>>>> upstream/android-13
	ctrl &= ~PWM_ON_TIME_DIV_MASK;
	ctrl &= ~((base_unit_range - 1) << PWM_BASE_UNIT_SHIFT);
	ctrl |= (u32) base_unit << PWM_BASE_UNIT_SHIFT;
	ctrl |= on_time_div;

<<<<<<< HEAD
	if (orig_ctrl != ctrl) {
		pwm_lpss_write(pwm, ctrl);
		pwm_lpss_write(pwm, ctrl | PWM_SW_UPDATE);
	}
=======
	pwm_lpss_write(pwm, ctrl);
	pwm_lpss_write(pwm, ctrl | PWM_SW_UPDATE);
>>>>>>> upstream/android-13
}

static inline void pwm_lpss_cond_enable(struct pwm_device *pwm, bool cond)
{
	if (cond)
		pwm_lpss_write(pwm, pwm_lpss_read(pwm) | PWM_ENABLE);
}

<<<<<<< HEAD
static int pwm_lpss_apply(struct pwm_chip *chip, struct pwm_device *pwm,
			  struct pwm_state *state)
{
	struct pwm_lpss_chip *lpwm = to_lpwm(chip);
	int ret;
=======
static int pwm_lpss_prepare_enable(struct pwm_lpss_chip *lpwm,
				   struct pwm_device *pwm,
				   const struct pwm_state *state)
{
	int ret;

	ret = pwm_lpss_is_updating(pwm);
	if (ret)
		return ret;

	pwm_lpss_prepare(lpwm, pwm, state->duty_cycle, state->period);
	pwm_lpss_cond_enable(pwm, lpwm->info->bypass == false);
	ret = pwm_lpss_wait_for_update(pwm);
	if (ret)
		return ret;

	pwm_lpss_cond_enable(pwm, lpwm->info->bypass == true);
	return 0;
}

static int pwm_lpss_apply(struct pwm_chip *chip, struct pwm_device *pwm,
			  const struct pwm_state *state)
{
	struct pwm_lpss_chip *lpwm = to_lpwm(chip);
	int ret = 0;
>>>>>>> upstream/android-13

	if (state->enabled) {
		if (!pwm_is_enabled(pwm)) {
			pm_runtime_get_sync(chip->dev);
<<<<<<< HEAD
			ret = pwm_lpss_is_updating(pwm);
			if (ret) {
				pm_runtime_put(chip->dev);
				return ret;
			}
			pwm_lpss_prepare(lpwm, pwm, state->duty_cycle, state->period);
			pwm_lpss_cond_enable(pwm, lpwm->info->bypass == false);
			ret = pwm_lpss_wait_for_update(pwm);
			if (ret) {
				pm_runtime_put(chip->dev);
				return ret;
			}
			pwm_lpss_cond_enable(pwm, lpwm->info->bypass == true);
		} else {
			ret = pwm_lpss_is_updating(pwm);
			if (ret)
				return ret;
			pwm_lpss_prepare(lpwm, pwm, state->duty_cycle, state->period);
			return pwm_lpss_wait_for_update(pwm);
=======
			ret = pwm_lpss_prepare_enable(lpwm, pwm, state);
			if (ret)
				pm_runtime_put(chip->dev);
		} else {
			ret = pwm_lpss_prepare_enable(lpwm, pwm, state);
>>>>>>> upstream/android-13
		}
	} else if (pwm_is_enabled(pwm)) {
		pwm_lpss_write(pwm, pwm_lpss_read(pwm) & ~PWM_ENABLE);
		pm_runtime_put(chip->dev);
	}

<<<<<<< HEAD
	return 0;
=======
	return ret;
}

static void pwm_lpss_get_state(struct pwm_chip *chip, struct pwm_device *pwm,
			       struct pwm_state *state)
{
	struct pwm_lpss_chip *lpwm = to_lpwm(chip);
	unsigned long base_unit_range;
	unsigned long long base_unit, freq, on_time_div;
	u32 ctrl;

	pm_runtime_get_sync(chip->dev);

	base_unit_range = BIT(lpwm->info->base_unit_bits);

	ctrl = pwm_lpss_read(pwm);
	on_time_div = 255 - (ctrl & PWM_ON_TIME_DIV_MASK);
	base_unit = (ctrl >> PWM_BASE_UNIT_SHIFT) & (base_unit_range - 1);

	freq = base_unit * lpwm->info->clk_rate;
	do_div(freq, base_unit_range);
	if (freq == 0)
		state->period = NSEC_PER_SEC;
	else
		state->period = NSEC_PER_SEC / (unsigned long)freq;

	on_time_div *= state->period;
	do_div(on_time_div, 255);
	state->duty_cycle = on_time_div;

	state->polarity = PWM_POLARITY_NORMAL;
	state->enabled = !!(ctrl & PWM_ENABLE);

	pm_runtime_put(chip->dev);
>>>>>>> upstream/android-13
}

static const struct pwm_ops pwm_lpss_ops = {
	.apply = pwm_lpss_apply,
<<<<<<< HEAD
=======
	.get_state = pwm_lpss_get_state,
>>>>>>> upstream/android-13
	.owner = THIS_MODULE,
};

struct pwm_lpss_chip *pwm_lpss_probe(struct device *dev, struct resource *r,
				     const struct pwm_lpss_boardinfo *info)
{
	struct pwm_lpss_chip *lpwm;
	unsigned long c;
<<<<<<< HEAD
	int ret;
=======
	int i, ret;
	u32 ctrl;
>>>>>>> upstream/android-13

	if (WARN_ON(info->npwm > MAX_PWMS))
		return ERR_PTR(-ENODEV);

	lpwm = devm_kzalloc(dev, sizeof(*lpwm), GFP_KERNEL);
	if (!lpwm)
		return ERR_PTR(-ENOMEM);

	lpwm->regs = devm_ioremap_resource(dev, r);
	if (IS_ERR(lpwm->regs))
		return ERR_CAST(lpwm->regs);

	lpwm->info = info;

	c = lpwm->info->clk_rate;
	if (!c)
		return ERR_PTR(-EINVAL);

	lpwm->chip.dev = dev;
	lpwm->chip.ops = &pwm_lpss_ops;
<<<<<<< HEAD
	lpwm->chip.base = -1;
	lpwm->chip.npwm = info->npwm;

	ret = pwmchip_add(&lpwm->chip);
=======
	lpwm->chip.npwm = info->npwm;

	ret = devm_pwmchip_add(dev, &lpwm->chip);
>>>>>>> upstream/android-13
	if (ret) {
		dev_err(dev, "failed to add PWM chip: %d\n", ret);
		return ERR_PTR(ret);
	}

<<<<<<< HEAD
=======
	for (i = 0; i < lpwm->info->npwm; i++) {
		ctrl = pwm_lpss_read(&lpwm->chip.pwms[i]);
		if (ctrl & PWM_ENABLE)
			pm_runtime_get(dev);
	}

>>>>>>> upstream/android-13
	return lpwm;
}
EXPORT_SYMBOL_GPL(pwm_lpss_probe);

<<<<<<< HEAD
int pwm_lpss_remove(struct pwm_lpss_chip *lpwm)
{
	int i;

	for (i = 0; i < lpwm->info->npwm; i++) {
		if (pwm_is_enabled(&lpwm->chip.pwms[i]))
			pm_runtime_put(lpwm->chip.dev);
	}
	return pwmchip_remove(&lpwm->chip);
}
EXPORT_SYMBOL_GPL(pwm_lpss_remove);

int pwm_lpss_suspend(struct device *dev)
{
	struct pwm_lpss_chip *lpwm = dev_get_drvdata(dev);
	int i;

	for (i = 0; i < lpwm->info->npwm; i++)
		lpwm->saved_ctrl[i] = readl(lpwm->regs + i * PWM_SIZE + PWM);

	return 0;
}
EXPORT_SYMBOL_GPL(pwm_lpss_suspend);

int pwm_lpss_resume(struct device *dev)
{
	struct pwm_lpss_chip *lpwm = dev_get_drvdata(dev);
	int i;

	for (i = 0; i < lpwm->info->npwm; i++)
		writel(lpwm->saved_ctrl[i], lpwm->regs + i * PWM_SIZE + PWM);

	return 0;
}
EXPORT_SYMBOL_GPL(pwm_lpss_resume);

=======
>>>>>>> upstream/android-13
MODULE_DESCRIPTION("PWM driver for Intel LPSS");
MODULE_AUTHOR("Mika Westerberg <mika.westerberg@linux.intel.com>");
MODULE_LICENSE("GPL v2");
