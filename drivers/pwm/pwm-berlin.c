/*
 * Marvell Berlin PWM driver
 *
 * Copyright (C) 2015 Marvell Technology Group Ltd.
 *
 * Author: Antoine Tenart <antoine.tenart@free-electrons.com>
 *
 * This file is licensed under the terms of the GNU General Public
 * License version 2. This program is licensed "as is" without any
 * warranty of any kind, whether express or implied.
 */

#include <linux/clk.h>
#include <linux/io.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/pwm.h>
#include <linux/slab.h>

#define BERLIN_PWM_EN			0x0
#define  BERLIN_PWM_ENABLE		BIT(0)
#define BERLIN_PWM_CONTROL		0x4
/*
 * The prescaler claims to support 8 different moduli, configured using the
 * low three bits of PWM_CONTROL. (Sequentially, they are 1, 4, 8, 16, 64,
 * 256, 1024, and 4096.)  However, the moduli from 4 to 1024 appear to be
 * implemented by internally shifting TCNT left without adding additional
 * bits. So, the max TCNT that actually works for a modulus of 4 is 0x3fff;
 * for 8, 0x1fff; and so on. This means that those moduli are entirely
 * useless, as we could just do the shift ourselves. The 4096 modulus is
 * implemented with a real prescaler, so we do use that, but we treat it
 * as a flag instead of pretending the modulus is actually configurable.
 */
#define  BERLIN_PWM_PRESCALE_4096	0x7
#define  BERLIN_PWM_INVERT_POLARITY	BIT(3)
#define BERLIN_PWM_DUTY			0x8
#define BERLIN_PWM_TCNT			0xc
#define  BERLIN_PWM_MAX_TCNT		65535

struct berlin_pwm_channel {
	u32 enable;
	u32 ctrl;
	u32 duty;
	u32 tcnt;
};

struct berlin_pwm_chip {
	struct pwm_chip chip;
	struct clk *clk;
	void __iomem *base;
};

static inline struct berlin_pwm_chip *to_berlin_pwm_chip(struct pwm_chip *chip)
{
	return container_of(chip, struct berlin_pwm_chip, chip);
}

<<<<<<< HEAD
static inline u32 berlin_pwm_readl(struct berlin_pwm_chip *chip,
				   unsigned int channel, unsigned long offset)
{
	return readl_relaxed(chip->base + channel * 0x10 + offset);
}

static inline void berlin_pwm_writel(struct berlin_pwm_chip *chip,
				     unsigned int channel, u32 value,
				     unsigned long offset)
{
	writel_relaxed(value, chip->base + channel * 0x10 + offset);
=======
static inline u32 berlin_pwm_readl(struct berlin_pwm_chip *bpc,
				   unsigned int channel, unsigned long offset)
{
	return readl_relaxed(bpc->base + channel * 0x10 + offset);
}

static inline void berlin_pwm_writel(struct berlin_pwm_chip *bpc,
				     unsigned int channel, u32 value,
				     unsigned long offset)
{
	writel_relaxed(value, bpc->base + channel * 0x10 + offset);
>>>>>>> upstream/android-13
}

static int berlin_pwm_request(struct pwm_chip *chip, struct pwm_device *pwm)
{
	struct berlin_pwm_channel *channel;

	channel = kzalloc(sizeof(*channel), GFP_KERNEL);
	if (!channel)
		return -ENOMEM;

	return pwm_set_chip_data(pwm, channel);
}

static void berlin_pwm_free(struct pwm_chip *chip, struct pwm_device *pwm)
{
	struct berlin_pwm_channel *channel = pwm_get_chip_data(pwm);

	kfree(channel);
}

<<<<<<< HEAD
static int berlin_pwm_config(struct pwm_chip *chip, struct pwm_device *pwm_dev,
			     int duty_ns, int period_ns)
{
	struct berlin_pwm_chip *pwm = to_berlin_pwm_chip(chip);
=======
static int berlin_pwm_config(struct pwm_chip *chip, struct pwm_device *pwm,
			     u64 duty_ns, u64 period_ns)
{
	struct berlin_pwm_chip *bpc = to_berlin_pwm_chip(chip);
>>>>>>> upstream/android-13
	bool prescale_4096 = false;
	u32 value, duty, period;
	u64 cycles;

<<<<<<< HEAD
	cycles = clk_get_rate(pwm->clk);
=======
	cycles = clk_get_rate(bpc->clk);
>>>>>>> upstream/android-13
	cycles *= period_ns;
	do_div(cycles, NSEC_PER_SEC);

	if (cycles > BERLIN_PWM_MAX_TCNT) {
		prescale_4096 = true;
		cycles >>= 12; // Prescaled by 4096

		if (cycles > BERLIN_PWM_MAX_TCNT)
			return -ERANGE;
	}

	period = cycles;
	cycles *= duty_ns;
	do_div(cycles, period_ns);
	duty = cycles;

<<<<<<< HEAD
	value = berlin_pwm_readl(pwm, pwm_dev->hwpwm, BERLIN_PWM_CONTROL);
=======
	value = berlin_pwm_readl(bpc, pwm->hwpwm, BERLIN_PWM_CONTROL);
>>>>>>> upstream/android-13
	if (prescale_4096)
		value |= BERLIN_PWM_PRESCALE_4096;
	else
		value &= ~BERLIN_PWM_PRESCALE_4096;
<<<<<<< HEAD
	berlin_pwm_writel(pwm, pwm_dev->hwpwm, value, BERLIN_PWM_CONTROL);

	berlin_pwm_writel(pwm, pwm_dev->hwpwm, duty, BERLIN_PWM_DUTY);
	berlin_pwm_writel(pwm, pwm_dev->hwpwm, period, BERLIN_PWM_TCNT);
=======
	berlin_pwm_writel(bpc, pwm->hwpwm, value, BERLIN_PWM_CONTROL);

	berlin_pwm_writel(bpc, pwm->hwpwm, duty, BERLIN_PWM_DUTY);
	berlin_pwm_writel(bpc, pwm->hwpwm, period, BERLIN_PWM_TCNT);
>>>>>>> upstream/android-13

	return 0;
}

static int berlin_pwm_set_polarity(struct pwm_chip *chip,
<<<<<<< HEAD
				   struct pwm_device *pwm_dev,
				   enum pwm_polarity polarity)
{
	struct berlin_pwm_chip *pwm = to_berlin_pwm_chip(chip);
	u32 value;

	value = berlin_pwm_readl(pwm, pwm_dev->hwpwm, BERLIN_PWM_CONTROL);
=======
				   struct pwm_device *pwm,
				   enum pwm_polarity polarity)
{
	struct berlin_pwm_chip *bpc = to_berlin_pwm_chip(chip);
	u32 value;

	value = berlin_pwm_readl(bpc, pwm->hwpwm, BERLIN_PWM_CONTROL);
>>>>>>> upstream/android-13

	if (polarity == PWM_POLARITY_NORMAL)
		value &= ~BERLIN_PWM_INVERT_POLARITY;
	else
		value |= BERLIN_PWM_INVERT_POLARITY;

<<<<<<< HEAD
	berlin_pwm_writel(pwm, pwm_dev->hwpwm, value, BERLIN_PWM_CONTROL);
=======
	berlin_pwm_writel(bpc, pwm->hwpwm, value, BERLIN_PWM_CONTROL);
>>>>>>> upstream/android-13

	return 0;
}

<<<<<<< HEAD
static int berlin_pwm_enable(struct pwm_chip *chip, struct pwm_device *pwm_dev)
{
	struct berlin_pwm_chip *pwm = to_berlin_pwm_chip(chip);
	u32 value;

	value = berlin_pwm_readl(pwm, pwm_dev->hwpwm, BERLIN_PWM_EN);
	value |= BERLIN_PWM_ENABLE;
	berlin_pwm_writel(pwm, pwm_dev->hwpwm, value, BERLIN_PWM_EN);
=======
static int berlin_pwm_enable(struct pwm_chip *chip, struct pwm_device *pwm)
{
	struct berlin_pwm_chip *bpc = to_berlin_pwm_chip(chip);
	u32 value;

	value = berlin_pwm_readl(bpc, pwm->hwpwm, BERLIN_PWM_EN);
	value |= BERLIN_PWM_ENABLE;
	berlin_pwm_writel(bpc, pwm->hwpwm, value, BERLIN_PWM_EN);
>>>>>>> upstream/android-13

	return 0;
}

static void berlin_pwm_disable(struct pwm_chip *chip,
<<<<<<< HEAD
			       struct pwm_device *pwm_dev)
{
	struct berlin_pwm_chip *pwm = to_berlin_pwm_chip(chip);
	u32 value;

	value = berlin_pwm_readl(pwm, pwm_dev->hwpwm, BERLIN_PWM_EN);
	value &= ~BERLIN_PWM_ENABLE;
	berlin_pwm_writel(pwm, pwm_dev->hwpwm, value, BERLIN_PWM_EN);
=======
			       struct pwm_device *pwm)
{
	struct berlin_pwm_chip *bpc = to_berlin_pwm_chip(chip);
	u32 value;

	value = berlin_pwm_readl(bpc, pwm->hwpwm, BERLIN_PWM_EN);
	value &= ~BERLIN_PWM_ENABLE;
	berlin_pwm_writel(bpc, pwm->hwpwm, value, BERLIN_PWM_EN);
}

static int berlin_pwm_apply(struct pwm_chip *chip, struct pwm_device *pwm,
			    const struct pwm_state *state)
{
	int err;
	bool enabled = pwm->state.enabled;

	if (state->polarity != pwm->state.polarity) {
		if (enabled) {
			berlin_pwm_disable(chip, pwm);
			enabled = false;
		}

		err = berlin_pwm_set_polarity(chip, pwm, state->polarity);
		if (err)
			return err;
	}

	if (!state->enabled) {
		if (enabled)
			berlin_pwm_disable(chip, pwm);
		return 0;
	}

	err = berlin_pwm_config(chip, pwm, state->duty_cycle, state->period);
	if (err)
		return err;

	if (!enabled)
		return berlin_pwm_enable(chip, pwm);

	return 0;
>>>>>>> upstream/android-13
}

static const struct pwm_ops berlin_pwm_ops = {
	.request = berlin_pwm_request,
	.free = berlin_pwm_free,
<<<<<<< HEAD
	.config = berlin_pwm_config,
	.set_polarity = berlin_pwm_set_polarity,
	.enable = berlin_pwm_enable,
	.disable = berlin_pwm_disable,
=======
	.apply = berlin_pwm_apply,
>>>>>>> upstream/android-13
	.owner = THIS_MODULE,
};

static const struct of_device_id berlin_pwm_match[] = {
	{ .compatible = "marvell,berlin-pwm" },
	{ },
};
MODULE_DEVICE_TABLE(of, berlin_pwm_match);

static int berlin_pwm_probe(struct platform_device *pdev)
{
<<<<<<< HEAD
	struct berlin_pwm_chip *pwm;
	struct resource *res;
	int ret;

	pwm = devm_kzalloc(&pdev->dev, sizeof(*pwm), GFP_KERNEL);
	if (!pwm)
		return -ENOMEM;

	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	pwm->base = devm_ioremap_resource(&pdev->dev, res);
	if (IS_ERR(pwm->base))
		return PTR_ERR(pwm->base);

	pwm->clk = devm_clk_get(&pdev->dev, NULL);
	if (IS_ERR(pwm->clk))
		return PTR_ERR(pwm->clk);

	ret = clk_prepare_enable(pwm->clk);
	if (ret)
		return ret;

	pwm->chip.dev = &pdev->dev;
	pwm->chip.ops = &berlin_pwm_ops;
	pwm->chip.base = -1;
	pwm->chip.npwm = 4;
	pwm->chip.of_xlate = of_pwm_xlate_with_flags;
	pwm->chip.of_pwm_n_cells = 3;

	ret = pwmchip_add(&pwm->chip);
	if (ret < 0) {
		dev_err(&pdev->dev, "failed to add PWM chip: %d\n", ret);
		clk_disable_unprepare(pwm->clk);
		return ret;
	}

	platform_set_drvdata(pdev, pwm);
=======
	struct berlin_pwm_chip *bpc;
	int ret;

	bpc = devm_kzalloc(&pdev->dev, sizeof(*bpc), GFP_KERNEL);
	if (!bpc)
		return -ENOMEM;

	bpc->base = devm_platform_ioremap_resource(pdev, 0);
	if (IS_ERR(bpc->base))
		return PTR_ERR(bpc->base);

	bpc->clk = devm_clk_get(&pdev->dev, NULL);
	if (IS_ERR(bpc->clk))
		return PTR_ERR(bpc->clk);

	ret = clk_prepare_enable(bpc->clk);
	if (ret)
		return ret;

	bpc->chip.dev = &pdev->dev;
	bpc->chip.ops = &berlin_pwm_ops;
	bpc->chip.npwm = 4;

	ret = pwmchip_add(&bpc->chip);
	if (ret < 0) {
		dev_err(&pdev->dev, "failed to add PWM chip: %d\n", ret);
		clk_disable_unprepare(bpc->clk);
		return ret;
	}

	platform_set_drvdata(pdev, bpc);
>>>>>>> upstream/android-13

	return 0;
}

static int berlin_pwm_remove(struct platform_device *pdev)
{
<<<<<<< HEAD
	struct berlin_pwm_chip *pwm = platform_get_drvdata(pdev);
	int ret;

	ret = pwmchip_remove(&pwm->chip);
	clk_disable_unprepare(pwm->clk);

	return ret;
=======
	struct berlin_pwm_chip *bpc = platform_get_drvdata(pdev);

	pwmchip_remove(&bpc->chip);

	clk_disable_unprepare(bpc->clk);

	return 0;
>>>>>>> upstream/android-13
}

#ifdef CONFIG_PM_SLEEP
static int berlin_pwm_suspend(struct device *dev)
{
<<<<<<< HEAD
	struct berlin_pwm_chip *pwm = dev_get_drvdata(dev);
	unsigned int i;

	for (i = 0; i < pwm->chip.npwm; i++) {
		struct berlin_pwm_channel *channel;

		channel = pwm_get_chip_data(&pwm->chip.pwms[i]);
		if (!channel)
			continue;

		channel->enable = berlin_pwm_readl(pwm, i, BERLIN_PWM_ENABLE);
		channel->ctrl = berlin_pwm_readl(pwm, i, BERLIN_PWM_CONTROL);
		channel->duty = berlin_pwm_readl(pwm, i, BERLIN_PWM_DUTY);
		channel->tcnt = berlin_pwm_readl(pwm, i, BERLIN_PWM_TCNT);
	}

	clk_disable_unprepare(pwm->clk);
=======
	struct berlin_pwm_chip *bpc = dev_get_drvdata(dev);
	unsigned int i;

	for (i = 0; i < bpc->chip.npwm; i++) {
		struct berlin_pwm_channel *channel;

		channel = pwm_get_chip_data(&bpc->chip.pwms[i]);
		if (!channel)
			continue;

		channel->enable = berlin_pwm_readl(bpc, i, BERLIN_PWM_ENABLE);
		channel->ctrl = berlin_pwm_readl(bpc, i, BERLIN_PWM_CONTROL);
		channel->duty = berlin_pwm_readl(bpc, i, BERLIN_PWM_DUTY);
		channel->tcnt = berlin_pwm_readl(bpc, i, BERLIN_PWM_TCNT);
	}

	clk_disable_unprepare(bpc->clk);
>>>>>>> upstream/android-13

	return 0;
}

static int berlin_pwm_resume(struct device *dev)
{
<<<<<<< HEAD
	struct berlin_pwm_chip *pwm = dev_get_drvdata(dev);
	unsigned int i;
	int ret;

	ret = clk_prepare_enable(pwm->clk);
	if (ret)
		return ret;

	for (i = 0; i < pwm->chip.npwm; i++) {
		struct berlin_pwm_channel *channel;

		channel = pwm_get_chip_data(&pwm->chip.pwms[i]);
		if (!channel)
			continue;

		berlin_pwm_writel(pwm, i, channel->ctrl, BERLIN_PWM_CONTROL);
		berlin_pwm_writel(pwm, i, channel->duty, BERLIN_PWM_DUTY);
		berlin_pwm_writel(pwm, i, channel->tcnt, BERLIN_PWM_TCNT);
		berlin_pwm_writel(pwm, i, channel->enable, BERLIN_PWM_ENABLE);
=======
	struct berlin_pwm_chip *bpc = dev_get_drvdata(dev);
	unsigned int i;
	int ret;

	ret = clk_prepare_enable(bpc->clk);
	if (ret)
		return ret;

	for (i = 0; i < bpc->chip.npwm; i++) {
		struct berlin_pwm_channel *channel;

		channel = pwm_get_chip_data(&bpc->chip.pwms[i]);
		if (!channel)
			continue;

		berlin_pwm_writel(bpc, i, channel->ctrl, BERLIN_PWM_CONTROL);
		berlin_pwm_writel(bpc, i, channel->duty, BERLIN_PWM_DUTY);
		berlin_pwm_writel(bpc, i, channel->tcnt, BERLIN_PWM_TCNT);
		berlin_pwm_writel(bpc, i, channel->enable, BERLIN_PWM_ENABLE);
>>>>>>> upstream/android-13
	}

	return 0;
}
#endif

static SIMPLE_DEV_PM_OPS(berlin_pwm_pm_ops, berlin_pwm_suspend,
			 berlin_pwm_resume);

static struct platform_driver berlin_pwm_driver = {
	.probe = berlin_pwm_probe,
	.remove = berlin_pwm_remove,
	.driver = {
		.name = "berlin-pwm",
		.of_match_table = berlin_pwm_match,
		.pm = &berlin_pwm_pm_ops,
	},
};
module_platform_driver(berlin_pwm_driver);

MODULE_AUTHOR("Antoine Tenart <antoine.tenart@free-electrons.com>");
MODULE_DESCRIPTION("Marvell Berlin PWM driver");
MODULE_LICENSE("GPL v2");
