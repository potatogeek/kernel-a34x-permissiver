<<<<<<< HEAD
/*
 * Copyright 2014 Bart Tanghe <bart.tanghe@thomasmore.be>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2.
=======
// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright 2014 Bart Tanghe <bart.tanghe@thomasmore.be>
>>>>>>> upstream/android-13
 */

#include <linux/clk.h>
#include <linux/err.h>
#include <linux/io.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/platform_device.h>
#include <linux/pwm.h>

#define PWM_CONTROL		0x000
#define PWM_CONTROL_SHIFT(x)	((x) * 8)
#define PWM_CONTROL_MASK	0xff
#define PWM_MODE		0x80		/* set timer in PWM mode */
#define PWM_ENABLE		(1 << 0)
#define PWM_POLARITY		(1 << 4)

#define PERIOD(x)		(((x) * 0x10) + 0x10)
#define DUTY(x)			(((x) * 0x10) + 0x14)

<<<<<<< HEAD
#define MIN_PERIOD		108		/* 9.2 MHz max. PWM clock */
=======
#define PERIOD_MIN		0x2
>>>>>>> upstream/android-13

struct bcm2835_pwm {
	struct pwm_chip chip;
	struct device *dev;
	void __iomem *base;
	struct clk *clk;
};

static inline struct bcm2835_pwm *to_bcm2835_pwm(struct pwm_chip *chip)
{
	return container_of(chip, struct bcm2835_pwm, chip);
}

static int bcm2835_pwm_request(struct pwm_chip *chip, struct pwm_device *pwm)
{
	struct bcm2835_pwm *pc = to_bcm2835_pwm(chip);
	u32 value;

	value = readl(pc->base + PWM_CONTROL);
	value &= ~(PWM_CONTROL_MASK << PWM_CONTROL_SHIFT(pwm->hwpwm));
	value |= (PWM_MODE << PWM_CONTROL_SHIFT(pwm->hwpwm));
	writel(value, pc->base + PWM_CONTROL);

	return 0;
}

static void bcm2835_pwm_free(struct pwm_chip *chip, struct pwm_device *pwm)
{
	struct bcm2835_pwm *pc = to_bcm2835_pwm(chip);
	u32 value;

	value = readl(pc->base + PWM_CONTROL);
	value &= ~(PWM_CONTROL_MASK << PWM_CONTROL_SHIFT(pwm->hwpwm));
	writel(value, pc->base + PWM_CONTROL);
}

<<<<<<< HEAD
static int bcm2835_pwm_config(struct pwm_chip *chip, struct pwm_device *pwm,
			      int duty_ns, int period_ns)
{
	struct bcm2835_pwm *pc = to_bcm2835_pwm(chip);
	unsigned long rate = clk_get_rate(pc->clk);
	unsigned long scaler;
=======
static int bcm2835_pwm_apply(struct pwm_chip *chip, struct pwm_device *pwm,
			     const struct pwm_state *state)
{

	struct bcm2835_pwm *pc = to_bcm2835_pwm(chip);
	unsigned long rate = clk_get_rate(pc->clk);
	unsigned long long period_cycles;
	u64 max_period;

	u32 val;
>>>>>>> upstream/android-13

	if (!rate) {
		dev_err(pc->dev, "failed to get clock rate\n");
		return -EINVAL;
	}

<<<<<<< HEAD
	scaler = NSEC_PER_SEC / rate;

	if (period_ns <= MIN_PERIOD) {
		dev_err(pc->dev, "period %d not supported, minimum %d\n",
			period_ns, MIN_PERIOD);
		return -EINVAL;
	}

	writel(duty_ns / scaler, pc->base + DUTY(pwm->hwpwm));
	writel(period_ns / scaler, pc->base + PERIOD(pwm->hwpwm));

	return 0;
}

static int bcm2835_pwm_enable(struct pwm_chip *chip, struct pwm_device *pwm)
{
	struct bcm2835_pwm *pc = to_bcm2835_pwm(chip);
	u32 value;

	value = readl(pc->base + PWM_CONTROL);
	value |= PWM_ENABLE << PWM_CONTROL_SHIFT(pwm->hwpwm);
	writel(value, pc->base + PWM_CONTROL);

	return 0;
}

static void bcm2835_pwm_disable(struct pwm_chip *chip, struct pwm_device *pwm)
{
	struct bcm2835_pwm *pc = to_bcm2835_pwm(chip);
	u32 value;

	value = readl(pc->base + PWM_CONTROL);
	value &= ~(PWM_ENABLE << PWM_CONTROL_SHIFT(pwm->hwpwm));
	writel(value, pc->base + PWM_CONTROL);
}

static int bcm2835_set_polarity(struct pwm_chip *chip, struct pwm_device *pwm,
				enum pwm_polarity polarity)
{
	struct bcm2835_pwm *pc = to_bcm2835_pwm(chip);
	u32 value;

	value = readl(pc->base + PWM_CONTROL);

	if (polarity == PWM_POLARITY_NORMAL)
		value &= ~(PWM_POLARITY << PWM_CONTROL_SHIFT(pwm->hwpwm));
	else
		value |= PWM_POLARITY << PWM_CONTROL_SHIFT(pwm->hwpwm);

	writel(value, pc->base + PWM_CONTROL);
=======
	/*
	 * period_cycles must be a 32 bit value, so period * rate / NSEC_PER_SEC
	 * must be <= U32_MAX. As U32_MAX * NSEC_PER_SEC < U64_MAX the
	 * multiplication period * rate doesn't overflow.
	 * To calculate the maximal possible period that guarantees the
	 * above inequality:
	 *
	 *     round(period * rate / NSEC_PER_SEC) <= U32_MAX
	 * <=> period * rate / NSEC_PER_SEC < U32_MAX + 0.5
	 * <=> period * rate < (U32_MAX + 0.5) * NSEC_PER_SEC
	 * <=> period < ((U32_MAX + 0.5) * NSEC_PER_SEC) / rate
	 * <=> period < ((U32_MAX * NSEC_PER_SEC + NSEC_PER_SEC/2) / rate
	 * <=> period <= ceil((U32_MAX * NSEC_PER_SEC + NSEC_PER_SEC/2) / rate) - 1
	 */
	max_period = DIV_ROUND_UP_ULL((u64)U32_MAX * NSEC_PER_SEC + NSEC_PER_SEC / 2, rate) - 1;

	if (state->period > max_period)
		return -EINVAL;

	/* set period */
	period_cycles = DIV_ROUND_CLOSEST_ULL(state->period * rate, NSEC_PER_SEC);

	/* don't accept a period that is too small */
	if (period_cycles < PERIOD_MIN)
		return -EINVAL;

	writel(period_cycles, pc->base + PERIOD(pwm->hwpwm));

	/* set duty cycle */
	val = DIV_ROUND_CLOSEST_ULL(state->duty_cycle * rate, NSEC_PER_SEC);
	writel(val, pc->base + DUTY(pwm->hwpwm));

	/* set polarity */
	val = readl(pc->base + PWM_CONTROL);

	if (state->polarity == PWM_POLARITY_NORMAL)
		val &= ~(PWM_POLARITY << PWM_CONTROL_SHIFT(pwm->hwpwm));
	else
		val |= PWM_POLARITY << PWM_CONTROL_SHIFT(pwm->hwpwm);

	/* enable/disable */
	if (state->enabled)
		val |= PWM_ENABLE << PWM_CONTROL_SHIFT(pwm->hwpwm);
	else
		val &= ~(PWM_ENABLE << PWM_CONTROL_SHIFT(pwm->hwpwm));

	writel(val, pc->base + PWM_CONTROL);
>>>>>>> upstream/android-13

	return 0;
}

static const struct pwm_ops bcm2835_pwm_ops = {
	.request = bcm2835_pwm_request,
	.free = bcm2835_pwm_free,
<<<<<<< HEAD
	.config = bcm2835_pwm_config,
	.enable = bcm2835_pwm_enable,
	.disable = bcm2835_pwm_disable,
	.set_polarity = bcm2835_set_polarity,
=======
	.apply = bcm2835_pwm_apply,
>>>>>>> upstream/android-13
	.owner = THIS_MODULE,
};

static int bcm2835_pwm_probe(struct platform_device *pdev)
{
	struct bcm2835_pwm *pc;
<<<<<<< HEAD
	struct resource *res;
=======
>>>>>>> upstream/android-13
	int ret;

	pc = devm_kzalloc(&pdev->dev, sizeof(*pc), GFP_KERNEL);
	if (!pc)
		return -ENOMEM;

	pc->dev = &pdev->dev;

<<<<<<< HEAD
	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	pc->base = devm_ioremap_resource(&pdev->dev, res);
=======
	pc->base = devm_platform_ioremap_resource(pdev, 0);
>>>>>>> upstream/android-13
	if (IS_ERR(pc->base))
		return PTR_ERR(pc->base);

	pc->clk = devm_clk_get(&pdev->dev, NULL);
<<<<<<< HEAD
	if (IS_ERR(pc->clk)) {
		dev_err(&pdev->dev, "clock not found: %ld\n", PTR_ERR(pc->clk));
		return PTR_ERR(pc->clk);
	}
=======
	if (IS_ERR(pc->clk))
		return dev_err_probe(&pdev->dev, PTR_ERR(pc->clk),
				     "clock not found\n");
>>>>>>> upstream/android-13

	ret = clk_prepare_enable(pc->clk);
	if (ret)
		return ret;

	pc->chip.dev = &pdev->dev;
	pc->chip.ops = &bcm2835_pwm_ops;
<<<<<<< HEAD
	pc->chip.base = -1;
	pc->chip.npwm = 2;
	pc->chip.of_xlate = of_pwm_xlate_with_flags;
	pc->chip.of_pwm_n_cells = 3;
=======
	pc->chip.npwm = 2;
>>>>>>> upstream/android-13

	platform_set_drvdata(pdev, pc);

	ret = pwmchip_add(&pc->chip);
	if (ret < 0)
		goto add_fail;

	return 0;

add_fail:
	clk_disable_unprepare(pc->clk);
	return ret;
}

static int bcm2835_pwm_remove(struct platform_device *pdev)
{
	struct bcm2835_pwm *pc = platform_get_drvdata(pdev);

<<<<<<< HEAD
	clk_disable_unprepare(pc->clk);

	return pwmchip_remove(&pc->chip);
=======
	pwmchip_remove(&pc->chip);

	clk_disable_unprepare(pc->clk);

	return 0;
>>>>>>> upstream/android-13
}

static const struct of_device_id bcm2835_pwm_of_match[] = {
	{ .compatible = "brcm,bcm2835-pwm", },
	{ /* sentinel */ }
};
MODULE_DEVICE_TABLE(of, bcm2835_pwm_of_match);

static struct platform_driver bcm2835_pwm_driver = {
	.driver = {
		.name = "bcm2835-pwm",
		.of_match_table = bcm2835_pwm_of_match,
	},
	.probe = bcm2835_pwm_probe,
	.remove = bcm2835_pwm_remove,
};
module_platform_driver(bcm2835_pwm_driver);

MODULE_AUTHOR("Bart Tanghe <bart.tanghe@thomasmore.be>");
MODULE_DESCRIPTION("Broadcom BCM2835 PWM driver");
MODULE_LICENSE("GPL v2");
