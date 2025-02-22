<<<<<<< HEAD
/*
 * Copyright (C) 2015 Intel Corporation. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License version
 * 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
=======
// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright (C) 2015 Intel Corporation. All rights reserved.
 *
>>>>>>> upstream/android-13
 * Author: Shobhit Kumar <shobhit.kumar@intel.com>
 */

#include <linux/platform_device.h>
#include <linux/regmap.h>
#include <linux/mfd/intel_soc_pmic.h>
#include <linux/pwm.h>

#define PWM0_CLK_DIV		0x4B
#define  PWM_OUTPUT_ENABLE	BIT(7)
#define  PWM_DIV_CLK_0		0x00 /* DIVIDECLK = BASECLK */
#define  PWM_DIV_CLK_100	0x63 /* DIVIDECLK = BASECLK/100 */
#define  PWM_DIV_CLK_128	0x7F /* DIVIDECLK = BASECLK/128 */

#define PWM0_DUTY_CYCLE		0x4E
#define BACKLIGHT_EN		0x51

#define PWM_MAX_LEVEL		0xFF

<<<<<<< HEAD
#define PWM_BASE_CLK		6000000  /* 6 MHz */
#define PWM_MAX_PERIOD_NS	21333    /* 46.875KHz */
=======
#define PWM_BASE_CLK_MHZ	6	/* 6 MHz */
#define PWM_MAX_PERIOD_NS	5461334	/* 183 Hz */
>>>>>>> upstream/android-13

/**
 * struct crystalcove_pwm - Crystal Cove PWM controller
 * @chip: the abstract pwm_chip structure.
 * @regmap: the regmap from the parent device.
 */
struct crystalcove_pwm {
	struct pwm_chip chip;
	struct regmap *regmap;
};

static inline struct crystalcove_pwm *to_crc_pwm(struct pwm_chip *pc)
{
	return container_of(pc, struct crystalcove_pwm, chip);
}

<<<<<<< HEAD
static int crc_pwm_enable(struct pwm_chip *c, struct pwm_device *pwm)
{
	struct crystalcove_pwm *crc_pwm = to_crc_pwm(c);

	regmap_write(crc_pwm->regmap, BACKLIGHT_EN, 1);

	return 0;
}

static void crc_pwm_disable(struct pwm_chip *c, struct pwm_device *pwm)
{
	struct crystalcove_pwm *crc_pwm = to_crc_pwm(c);

	regmap_write(crc_pwm->regmap, BACKLIGHT_EN, 0);
}

static int crc_pwm_config(struct pwm_chip *c, struct pwm_device *pwm,
			  int duty_ns, int period_ns)
{
	struct crystalcove_pwm *crc_pwm = to_crc_pwm(c);
	struct device *dev = crc_pwm->chip.dev;
	int level;

	if (period_ns > PWM_MAX_PERIOD_NS) {
=======
static int crc_pwm_calc_clk_div(int period_ns)
{
	int clk_div;

	clk_div = PWM_BASE_CLK_MHZ * period_ns / (256 * NSEC_PER_USEC);
	/* clk_div 1 - 128, maps to register values 0-127 */
	if (clk_div > 0)
		clk_div--;

	return clk_div;
}

static int crc_pwm_apply(struct pwm_chip *chip, struct pwm_device *pwm,
			 const struct pwm_state *state)
{
	struct crystalcove_pwm *crc_pwm = to_crc_pwm(chip);
	struct device *dev = crc_pwm->chip.dev;
	int err;

	if (state->period > PWM_MAX_PERIOD_NS) {
>>>>>>> upstream/android-13
		dev_err(dev, "un-supported period_ns\n");
		return -EINVAL;
	}

<<<<<<< HEAD
	if (pwm_get_period(pwm) != period_ns) {
		int clk_div;

		/* changing the clk divisor, need to disable fisrt */
		crc_pwm_disable(c, pwm);
		clk_div = PWM_BASE_CLK * period_ns / NSEC_PER_SEC;

		regmap_write(crc_pwm->regmap, PWM0_CLK_DIV,
					clk_div | PWM_OUTPUT_ENABLE);

		/* enable back */
		crc_pwm_enable(c, pwm);
	}

	/* change the pwm duty cycle */
	level = duty_ns * PWM_MAX_LEVEL / period_ns;
	regmap_write(crc_pwm->regmap, PWM0_DUTY_CYCLE, level);
=======
	if (state->polarity != PWM_POLARITY_NORMAL)
		return -EINVAL;

	if (pwm_is_enabled(pwm) && !state->enabled) {
		err = regmap_write(crc_pwm->regmap, BACKLIGHT_EN, 0);
		if (err) {
			dev_err(dev, "Error writing BACKLIGHT_EN %d\n", err);
			return err;
		}
	}

	if (pwm_get_duty_cycle(pwm) != state->duty_cycle ||
	    pwm_get_period(pwm) != state->period) {
		u64 level = state->duty_cycle * PWM_MAX_LEVEL;

		do_div(level, state->period);

		err = regmap_write(crc_pwm->regmap, PWM0_DUTY_CYCLE, level);
		if (err) {
			dev_err(dev, "Error writing PWM0_DUTY_CYCLE %d\n", err);
			return err;
		}
	}

	if (pwm_is_enabled(pwm) && state->enabled &&
	    pwm_get_period(pwm) != state->period) {
		/* changing the clk divisor, clear PWM_OUTPUT_ENABLE first */
		err = regmap_write(crc_pwm->regmap, PWM0_CLK_DIV, 0);
		if (err) {
			dev_err(dev, "Error writing PWM0_CLK_DIV %d\n", err);
			return err;
		}
	}

	if (pwm_get_period(pwm) != state->period ||
	    pwm_is_enabled(pwm) != state->enabled) {
		int clk_div = crc_pwm_calc_clk_div(state->period);
		int pwm_output_enable = state->enabled ? PWM_OUTPUT_ENABLE : 0;

		err = regmap_write(crc_pwm->regmap, PWM0_CLK_DIV,
				   clk_div | pwm_output_enable);
		if (err) {
			dev_err(dev, "Error writing PWM0_CLK_DIV %d\n", err);
			return err;
		}
	}

	if (!pwm_is_enabled(pwm) && state->enabled) {
		err = regmap_write(crc_pwm->regmap, BACKLIGHT_EN, 1);
		if (err) {
			dev_err(dev, "Error writing BACKLIGHT_EN %d\n", err);
			return err;
		}
	}
>>>>>>> upstream/android-13

	return 0;
}

<<<<<<< HEAD
static const struct pwm_ops crc_pwm_ops = {
	.config = crc_pwm_config,
	.enable = crc_pwm_enable,
	.disable = crc_pwm_disable,
=======
static void crc_pwm_get_state(struct pwm_chip *chip, struct pwm_device *pwm,
			      struct pwm_state *state)
{
	struct crystalcove_pwm *crc_pwm = to_crc_pwm(chip);
	struct device *dev = crc_pwm->chip.dev;
	unsigned int clk_div, clk_div_reg, duty_cycle_reg;
	int error;

	error = regmap_read(crc_pwm->regmap, PWM0_CLK_DIV, &clk_div_reg);
	if (error) {
		dev_err(dev, "Error reading PWM0_CLK_DIV %d\n", error);
		return;
	}

	error = regmap_read(crc_pwm->regmap, PWM0_DUTY_CYCLE, &duty_cycle_reg);
	if (error) {
		dev_err(dev, "Error reading PWM0_DUTY_CYCLE %d\n", error);
		return;
	}

	clk_div = (clk_div_reg & ~PWM_OUTPUT_ENABLE) + 1;

	state->period =
		DIV_ROUND_UP(clk_div * NSEC_PER_USEC * 256, PWM_BASE_CLK_MHZ);
	state->duty_cycle =
		DIV_ROUND_UP_ULL(duty_cycle_reg * state->period, PWM_MAX_LEVEL);
	state->polarity = PWM_POLARITY_NORMAL;
	state->enabled = !!(clk_div_reg & PWM_OUTPUT_ENABLE);
}

static const struct pwm_ops crc_pwm_ops = {
	.apply = crc_pwm_apply,
	.get_state = crc_pwm_get_state,
>>>>>>> upstream/android-13
};

static int crystalcove_pwm_probe(struct platform_device *pdev)
{
	struct crystalcove_pwm *pwm;
	struct device *dev = pdev->dev.parent;
	struct intel_soc_pmic *pmic = dev_get_drvdata(dev);

	pwm = devm_kzalloc(&pdev->dev, sizeof(*pwm), GFP_KERNEL);
	if (!pwm)
		return -ENOMEM;

	pwm->chip.dev = &pdev->dev;
	pwm->chip.ops = &crc_pwm_ops;
<<<<<<< HEAD
	pwm->chip.base = -1;
=======
>>>>>>> upstream/android-13
	pwm->chip.npwm = 1;

	/* get the PMIC regmap */
	pwm->regmap = pmic->regmap;

<<<<<<< HEAD
	platform_set_drvdata(pdev, pwm);

	return pwmchip_add(&pwm->chip);
}

static int crystalcove_pwm_remove(struct platform_device *pdev)
{
	struct crystalcove_pwm *pwm = platform_get_drvdata(pdev);

	return pwmchip_remove(&pwm->chip);
=======
	return devm_pwmchip_add(&pdev->dev, &pwm->chip);
>>>>>>> upstream/android-13
}

static struct platform_driver crystalcove_pwm_driver = {
	.probe = crystalcove_pwm_probe,
<<<<<<< HEAD
	.remove = crystalcove_pwm_remove,
=======
>>>>>>> upstream/android-13
	.driver = {
		.name = "crystal_cove_pwm",
	},
};

builtin_platform_driver(crystalcove_pwm_driver);
