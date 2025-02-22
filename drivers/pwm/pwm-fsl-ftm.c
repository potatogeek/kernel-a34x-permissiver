<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> upstream/android-13
/*
 *  Freescale FlexTimer Module (FTM) PWM Driver
 *
 *  Copyright 2012-2013 Freescale Semiconductor, Inc.
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
=======
>>>>>>> upstream/android-13
 */

#include <linux/clk.h>
#include <linux/err.h>
#include <linux/io.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/mutex.h>
#include <linux/of_address.h>
#include <linux/of_device.h>
#include <linux/platform_device.h>
#include <linux/pm.h>
#include <linux/pwm.h>
#include <linux/regmap.h>
#include <linux/slab.h>
<<<<<<< HEAD

#define FTM_SC		0x00
#define FTM_SC_CLK_MASK_SHIFT	3
#define FTM_SC_CLK_MASK	(3 << FTM_SC_CLK_MASK_SHIFT)
#define FTM_SC_CLK(c)	(((c) + 1) << FTM_SC_CLK_MASK_SHIFT)
#define FTM_SC_PS_MASK	0x7

#define FTM_CNT		0x04
#define FTM_MOD		0x08

#define FTM_CSC_BASE	0x0C
#define FTM_CSC_MSB	BIT(5)
#define FTM_CSC_MSA	BIT(4)
#define FTM_CSC_ELSB	BIT(3)
#define FTM_CSC_ELSA	BIT(2)
#define FTM_CSC(_channel)	(FTM_CSC_BASE + ((_channel) * 8))

#define FTM_CV_BASE	0x10
#define FTM_CV(_channel)	(FTM_CV_BASE + ((_channel) * 8))

#define FTM_CNTIN	0x4C
#define FTM_STATUS	0x50

#define FTM_MODE	0x54
#define FTM_MODE_FTMEN	BIT(0)
#define FTM_MODE_INIT	BIT(2)
#define FTM_MODE_PWMSYNC	BIT(3)

#define FTM_SYNC	0x58
#define FTM_OUTINIT	0x5C
#define FTM_OUTMASK	0x60
#define FTM_COMBINE	0x64
#define FTM_DEADTIME	0x68
#define FTM_EXTTRIG	0x6C
#define FTM_POL		0x70
#define FTM_FMS		0x74
#define FTM_FILTER	0x78
#define FTM_FLTCTRL	0x7C
#define FTM_QDCTRL	0x80
#define FTM_CONF	0x84
#define FTM_FLTPOL	0x88
#define FTM_SYNCONF	0x8C
#define FTM_INVCTRL	0x90
#define FTM_SWOCTRL	0x94
#define FTM_PWMLOAD	0x98
=======
#include <linux/fsl/ftm.h>

#define FTM_SC_CLK(c)	(((c) + 1) << FTM_SC_CLK_MASK_SHIFT)
>>>>>>> upstream/android-13

enum fsl_pwm_clk {
	FSL_PWM_CLK_SYS,
	FSL_PWM_CLK_FIX,
	FSL_PWM_CLK_EXT,
	FSL_PWM_CLK_CNTEN,
	FSL_PWM_CLK_MAX
};

struct fsl_ftm_soc {
	bool has_enable_bits;
};

<<<<<<< HEAD
struct fsl_pwm_chip {
	struct pwm_chip chip;

	struct mutex lock;

	unsigned int cnt_select;
	unsigned int clk_ps;

	struct regmap *regmap;

	int period_ns;
=======
struct fsl_pwm_periodcfg {
	enum fsl_pwm_clk clk_select;
	unsigned int clk_ps;
	unsigned int mod_period;
};

struct fsl_pwm_chip {
	struct pwm_chip chip;
	struct mutex lock;
	struct regmap *regmap;

	/* This value is valid iff a pwm is running */
	struct fsl_pwm_periodcfg period;
>>>>>>> upstream/android-13

	struct clk *ipg_clk;
	struct clk *clk[FSL_PWM_CLK_MAX];

	const struct fsl_ftm_soc *soc;
};

static inline struct fsl_pwm_chip *to_fsl_chip(struct pwm_chip *chip)
{
	return container_of(chip, struct fsl_pwm_chip, chip);
}

<<<<<<< HEAD
=======
static void ftm_clear_write_protection(struct fsl_pwm_chip *fpc)
{
	u32 val;

	regmap_read(fpc->regmap, FTM_FMS, &val);
	if (val & FTM_FMS_WPEN)
		regmap_update_bits(fpc->regmap, FTM_MODE, FTM_MODE_WPDIS,
				   FTM_MODE_WPDIS);
}

static void ftm_set_write_protection(struct fsl_pwm_chip *fpc)
{
	regmap_update_bits(fpc->regmap, FTM_FMS, FTM_FMS_WPEN, FTM_FMS_WPEN);
}

static bool fsl_pwm_periodcfg_are_equal(const struct fsl_pwm_periodcfg *a,
					const struct fsl_pwm_periodcfg *b)
{
	if (a->clk_select != b->clk_select)
		return false;
	if (a->clk_ps != b->clk_ps)
		return false;
	if (a->mod_period != b->mod_period)
		return false;
	return true;
}

>>>>>>> upstream/android-13
static int fsl_pwm_request(struct pwm_chip *chip, struct pwm_device *pwm)
{
	int ret;
	struct fsl_pwm_chip *fpc = to_fsl_chip(chip);

	ret = clk_prepare_enable(fpc->ipg_clk);
	if (!ret && fpc->soc->has_enable_bits) {
		mutex_lock(&fpc->lock);
		regmap_update_bits(fpc->regmap, FTM_SC, BIT(pwm->hwpwm + 16),
				   BIT(pwm->hwpwm + 16));
		mutex_unlock(&fpc->lock);
	}

	return ret;
}

static void fsl_pwm_free(struct pwm_chip *chip, struct pwm_device *pwm)
{
	struct fsl_pwm_chip *fpc = to_fsl_chip(chip);

	if (fpc->soc->has_enable_bits) {
		mutex_lock(&fpc->lock);
		regmap_update_bits(fpc->regmap, FTM_SC, BIT(pwm->hwpwm + 16),
				   0);
		mutex_unlock(&fpc->lock);
	}

	clk_disable_unprepare(fpc->ipg_clk);
}

<<<<<<< HEAD
static int fsl_pwm_calculate_default_ps(struct fsl_pwm_chip *fpc,
					enum fsl_pwm_clk index)
{
	unsigned long sys_rate, cnt_rate;
	unsigned long long ratio;

	sys_rate = clk_get_rate(fpc->clk[FSL_PWM_CLK_SYS]);
	if (!sys_rate)
		return -EINVAL;

	cnt_rate = clk_get_rate(fpc->clk[fpc->cnt_select]);
	if (!cnt_rate)
		return -EINVAL;

	switch (index) {
	case FSL_PWM_CLK_SYS:
		fpc->clk_ps = 1;
		break;
	case FSL_PWM_CLK_FIX:
		ratio = 2 * cnt_rate - 1;
		do_div(ratio, sys_rate);
		fpc->clk_ps = ratio;
		break;
	case FSL_PWM_CLK_EXT:
		ratio = 4 * cnt_rate - 1;
		do_div(ratio, sys_rate);
		fpc->clk_ps = ratio;
		break;
	default:
		return -EINVAL;
	}

	return 0;
}

static unsigned long fsl_pwm_calculate_cycles(struct fsl_pwm_chip *fpc,
					      unsigned long period_ns)
{
	unsigned long long c, c0;

	c = clk_get_rate(fpc->clk[fpc->cnt_select]);
	c = c * period_ns;
	do_div(c, 1000000000UL);

	do {
		c0 = c;
		do_div(c0, (1 << fpc->clk_ps));
		if (c0 <= 0xFFFF)
			return (unsigned long)c0;
	} while (++fpc->clk_ps < 8);

	return 0;
}

static unsigned long fsl_pwm_calculate_period_cycles(struct fsl_pwm_chip *fpc,
						     unsigned long period_ns,
						     enum fsl_pwm_clk index)
{
	int ret;

	ret = fsl_pwm_calculate_default_ps(fpc, index);
	if (ret) {
		dev_err(fpc->chip.dev,
			"failed to calculate default prescaler: %d\n",
			ret);
		return 0;
	}

	return fsl_pwm_calculate_cycles(fpc, period_ns);
}

static unsigned long fsl_pwm_calculate_period(struct fsl_pwm_chip *fpc,
					      unsigned long period_ns)
{
	enum fsl_pwm_clk m0, m1;
	unsigned long fix_rate, ext_rate, cycles;

	cycles = fsl_pwm_calculate_period_cycles(fpc, period_ns,
			FSL_PWM_CLK_SYS);
	if (cycles) {
		fpc->cnt_select = FSL_PWM_CLK_SYS;
		return cycles;
	}
=======
static unsigned int fsl_pwm_ticks_to_ns(struct fsl_pwm_chip *fpc,
					  unsigned int ticks)
{
	unsigned long rate;
	unsigned long long exval;

	rate = clk_get_rate(fpc->clk[fpc->period.clk_select]);
	exval = ticks;
	exval *= 1000000000UL;
	do_div(exval, rate >> fpc->period.clk_ps);
	return exval;
}

static bool fsl_pwm_calculate_period_clk(struct fsl_pwm_chip *fpc,
					 unsigned int period_ns,
					 enum fsl_pwm_clk index,
					 struct fsl_pwm_periodcfg *periodcfg
					 )
{
	unsigned long long c;
	unsigned int ps;

	c = clk_get_rate(fpc->clk[index]);
	c = c * period_ns;
	do_div(c, 1000000000UL);

	if (c == 0)
		return false;

	for (ps = 0; ps < 8 ; ++ps, c >>= 1) {
		if (c <= 0x10000) {
			periodcfg->clk_select = index;
			periodcfg->clk_ps = ps;
			periodcfg->mod_period = c - 1;
			return true;
		}
	}
	return false;
}

static bool fsl_pwm_calculate_period(struct fsl_pwm_chip *fpc,
				     unsigned int period_ns,
				     struct fsl_pwm_periodcfg *periodcfg)
{
	enum fsl_pwm_clk m0, m1;
	unsigned long fix_rate, ext_rate;
	bool ret;

	ret = fsl_pwm_calculate_period_clk(fpc, period_ns, FSL_PWM_CLK_SYS,
					   periodcfg);
	if (ret)
		return true;
>>>>>>> upstream/android-13

	fix_rate = clk_get_rate(fpc->clk[FSL_PWM_CLK_FIX]);
	ext_rate = clk_get_rate(fpc->clk[FSL_PWM_CLK_EXT]);

	if (fix_rate > ext_rate) {
		m0 = FSL_PWM_CLK_FIX;
		m1 = FSL_PWM_CLK_EXT;
	} else {
		m0 = FSL_PWM_CLK_EXT;
		m1 = FSL_PWM_CLK_FIX;
	}

<<<<<<< HEAD
	cycles = fsl_pwm_calculate_period_cycles(fpc, period_ns, m0);
	if (cycles) {
		fpc->cnt_select = m0;
		return cycles;
	}

	fpc->cnt_select = m1;

	return fsl_pwm_calculate_period_cycles(fpc, period_ns, m1);
}

static unsigned long fsl_pwm_calculate_duty(struct fsl_pwm_chip *fpc,
					    unsigned long period_ns,
					    unsigned long duty_ns)
{
	unsigned long long duty;
	u32 val;

	regmap_read(fpc->regmap, FTM_MOD, &val);
	duty = (unsigned long long)duty_ns * (val + 1);
	do_div(duty, period_ns);

	return (unsigned long)duty;
}

static int fsl_pwm_config(struct pwm_chip *chip, struct pwm_device *pwm,
			  int duty_ns, int period_ns)
{
	struct fsl_pwm_chip *fpc = to_fsl_chip(chip);
	u32 period, duty;

	mutex_lock(&fpc->lock);

	/*
	 * The Freescale FTM controller supports only a single period for
	 * all PWM channels, therefore incompatible changes need to be
	 * refused.
	 */
	if (fpc->period_ns && fpc->period_ns != period_ns) {
		dev_err(fpc->chip.dev,
			"conflicting period requested for PWM %u\n",
			pwm->hwpwm);
		mutex_unlock(&fpc->lock);
		return -EBUSY;
	}

	if (!fpc->period_ns && duty_ns) {
		period = fsl_pwm_calculate_period(fpc, period_ns);
		if (!period) {
			dev_err(fpc->chip.dev, "failed to calculate period\n");
			mutex_unlock(&fpc->lock);
			return -EINVAL;
		}

		regmap_update_bits(fpc->regmap, FTM_SC, FTM_SC_PS_MASK,
				   fpc->clk_ps);
		regmap_write(fpc->regmap, FTM_MOD, period - 1);

		fpc->period_ns = period_ns;
	}

	mutex_unlock(&fpc->lock);

	duty = fsl_pwm_calculate_duty(fpc, period_ns, duty_ns);
=======
	ret = fsl_pwm_calculate_period_clk(fpc, period_ns, m0, periodcfg);
	if (ret)
		return true;

	return fsl_pwm_calculate_period_clk(fpc, period_ns, m1, periodcfg);
}

static unsigned int fsl_pwm_calculate_duty(struct fsl_pwm_chip *fpc,
					   unsigned int duty_ns)
{
	unsigned long long duty;

	unsigned int period = fpc->period.mod_period + 1;
	unsigned int period_ns = fsl_pwm_ticks_to_ns(fpc, period);

	duty = (unsigned long long)duty_ns * period;
	do_div(duty, period_ns);

	return (unsigned int)duty;
}

static bool fsl_pwm_is_any_pwm_enabled(struct fsl_pwm_chip *fpc,
				       struct pwm_device *pwm)
{
	u32 val;

	regmap_read(fpc->regmap, FTM_OUTMASK, &val);
	if (~val & 0xFF)
		return true;
	else
		return false;
}

static bool fsl_pwm_is_other_pwm_enabled(struct fsl_pwm_chip *fpc,
					 struct pwm_device *pwm)
{
	u32 val;

	regmap_read(fpc->regmap, FTM_OUTMASK, &val);
	if (~(val | BIT(pwm->hwpwm)) & 0xFF)
		return true;
	else
		return false;
}

static int fsl_pwm_apply_config(struct fsl_pwm_chip *fpc,
				struct pwm_device *pwm,
				const struct pwm_state *newstate)
{
	unsigned int duty;
	u32 reg_polarity;

	struct fsl_pwm_periodcfg periodcfg;
	bool do_write_period = false;

	if (!fsl_pwm_calculate_period(fpc, newstate->period, &periodcfg)) {
		dev_err(fpc->chip.dev, "failed to calculate new period\n");
		return -EINVAL;
	}

	if (!fsl_pwm_is_any_pwm_enabled(fpc, pwm))
		do_write_period = true;
	/*
	 * The Freescale FTM controller supports only a single period for
	 * all PWM channels, therefore verify if the newly computed period
	 * is different than the current period being used. In such case
	 * we allow to change the period only if no other pwm is running.
	 */
	else if (!fsl_pwm_periodcfg_are_equal(&fpc->period, &periodcfg)) {
		if (fsl_pwm_is_other_pwm_enabled(fpc, pwm)) {
			dev_err(fpc->chip.dev,
				"Cannot change period for PWM %u, disable other PWMs first\n",
				pwm->hwpwm);
			return -EBUSY;
		}
		if (fpc->period.clk_select != periodcfg.clk_select) {
			int ret;
			enum fsl_pwm_clk oldclk = fpc->period.clk_select;
			enum fsl_pwm_clk newclk = periodcfg.clk_select;

			ret = clk_prepare_enable(fpc->clk[newclk]);
			if (ret)
				return ret;
			clk_disable_unprepare(fpc->clk[oldclk]);
		}
		do_write_period = true;
	}

	ftm_clear_write_protection(fpc);

	if (do_write_period) {
		regmap_update_bits(fpc->regmap, FTM_SC, FTM_SC_CLK_MASK,
				   FTM_SC_CLK(periodcfg.clk_select));
		regmap_update_bits(fpc->regmap, FTM_SC, FTM_SC_PS_MASK,
				   periodcfg.clk_ps);
		regmap_write(fpc->regmap, FTM_MOD, periodcfg.mod_period);

		fpc->period = periodcfg;
	}

	duty = fsl_pwm_calculate_duty(fpc, newstate->duty_cycle);
>>>>>>> upstream/android-13

	regmap_write(fpc->regmap, FTM_CSC(pwm->hwpwm),
		     FTM_CSC_MSB | FTM_CSC_ELSB);
	regmap_write(fpc->regmap, FTM_CV(pwm->hwpwm), duty);

<<<<<<< HEAD
	return 0;
}

static int fsl_pwm_set_polarity(struct pwm_chip *chip,
				struct pwm_device *pwm,
				enum pwm_polarity polarity)
{
	struct fsl_pwm_chip *fpc = to_fsl_chip(chip);
	u32 val;

	regmap_read(fpc->regmap, FTM_POL, &val);

	if (polarity == PWM_POLARITY_INVERSED)
		val |= BIT(pwm->hwpwm);
	else
		val &= ~BIT(pwm->hwpwm);

	regmap_write(fpc->regmap, FTM_POL, val);

	return 0;
}

static int fsl_counter_clock_enable(struct fsl_pwm_chip *fpc)
{
	int ret;

	/* select counter clock source */
	regmap_update_bits(fpc->regmap, FTM_SC, FTM_SC_CLK_MASK,
			   FTM_SC_CLK(fpc->cnt_select));

	ret = clk_prepare_enable(fpc->clk[fpc->cnt_select]);
	if (ret)
		return ret;

	ret = clk_prepare_enable(fpc->clk[FSL_PWM_CLK_CNTEN]);
	if (ret) {
		clk_disable_unprepare(fpc->clk[fpc->cnt_select]);
		return ret;
	}

	return 0;
}

static int fsl_pwm_enable(struct pwm_chip *chip, struct pwm_device *pwm)
{
	struct fsl_pwm_chip *fpc = to_fsl_chip(chip);
	int ret;

	mutex_lock(&fpc->lock);
	regmap_update_bits(fpc->regmap, FTM_OUTMASK, BIT(pwm->hwpwm), 0);

	ret = fsl_counter_clock_enable(fpc);
	mutex_unlock(&fpc->lock);

	return ret;
}

static void fsl_pwm_disable(struct pwm_chip *chip, struct pwm_device *pwm)
{
	struct fsl_pwm_chip *fpc = to_fsl_chip(chip);
	u32 val;

	mutex_lock(&fpc->lock);
	regmap_update_bits(fpc->regmap, FTM_OUTMASK, BIT(pwm->hwpwm),
			   BIT(pwm->hwpwm));

	clk_disable_unprepare(fpc->clk[FSL_PWM_CLK_CNTEN]);
	clk_disable_unprepare(fpc->clk[fpc->cnt_select]);

	regmap_read(fpc->regmap, FTM_OUTMASK, &val);
	if ((val & 0xFF) == 0xFF)
		fpc->period_ns = 0;

	mutex_unlock(&fpc->lock);
}

static const struct pwm_ops fsl_pwm_ops = {
	.request = fsl_pwm_request,
	.free = fsl_pwm_free,
	.config = fsl_pwm_config,
	.set_polarity = fsl_pwm_set_polarity,
	.enable = fsl_pwm_enable,
	.disable = fsl_pwm_disable,
=======
	reg_polarity = 0;
	if (newstate->polarity == PWM_POLARITY_INVERSED)
		reg_polarity = BIT(pwm->hwpwm);

	regmap_update_bits(fpc->regmap, FTM_POL, BIT(pwm->hwpwm), reg_polarity);

	ftm_set_write_protection(fpc);

	return 0;
}

static int fsl_pwm_apply(struct pwm_chip *chip, struct pwm_device *pwm,
			 const struct pwm_state *newstate)
{
	struct fsl_pwm_chip *fpc = to_fsl_chip(chip);
	struct pwm_state *oldstate = &pwm->state;
	int ret = 0;

	/*
	 * oldstate to newstate : action
	 *
	 * disabled to disabled : ignore
	 * enabled to disabled : disable
	 * enabled to enabled : update settings
	 * disabled to enabled : update settings + enable
	 */

	mutex_lock(&fpc->lock);

	if (!newstate->enabled) {
		if (oldstate->enabled) {
			regmap_update_bits(fpc->regmap, FTM_OUTMASK,
					   BIT(pwm->hwpwm), BIT(pwm->hwpwm));
			clk_disable_unprepare(fpc->clk[FSL_PWM_CLK_CNTEN]);
			clk_disable_unprepare(fpc->clk[fpc->period.clk_select]);
		}

		goto end_mutex;
	}

	ret = fsl_pwm_apply_config(fpc, pwm, newstate);
	if (ret)
		goto end_mutex;

	/* check if need to enable */
	if (!oldstate->enabled) {
		ret = clk_prepare_enable(fpc->clk[fpc->period.clk_select]);
		if (ret)
			goto end_mutex;

		ret = clk_prepare_enable(fpc->clk[FSL_PWM_CLK_CNTEN]);
		if (ret) {
			clk_disable_unprepare(fpc->clk[fpc->period.clk_select]);
			goto end_mutex;
		}

		regmap_update_bits(fpc->regmap, FTM_OUTMASK, BIT(pwm->hwpwm),
				   0);
	}

end_mutex:
	mutex_unlock(&fpc->lock);
	return ret;
}

static const struct pwm_ops fsl_pwm_ops = {
	.request = fsl_pwm_request,
	.free = fsl_pwm_free,
	.apply = fsl_pwm_apply,
>>>>>>> upstream/android-13
	.owner = THIS_MODULE,
};

static int fsl_pwm_init(struct fsl_pwm_chip *fpc)
{
	int ret;

	ret = clk_prepare_enable(fpc->ipg_clk);
	if (ret)
		return ret;

	regmap_write(fpc->regmap, FTM_CNTIN, 0x00);
	regmap_write(fpc->regmap, FTM_OUTINIT, 0x00);
	regmap_write(fpc->regmap, FTM_OUTMASK, 0xFF);

	clk_disable_unprepare(fpc->ipg_clk);

	return 0;
}

static bool fsl_pwm_volatile_reg(struct device *dev, unsigned int reg)
{
	switch (reg) {
<<<<<<< HEAD
=======
	case FTM_FMS:
	case FTM_MODE:
>>>>>>> upstream/android-13
	case FTM_CNT:
		return true;
	}
	return false;
}

static const struct regmap_config fsl_pwm_regmap_config = {
	.reg_bits = 32,
	.reg_stride = 4,
	.val_bits = 32,

	.max_register = FTM_PWMLOAD,
	.volatile_reg = fsl_pwm_volatile_reg,
	.cache_type = REGCACHE_FLAT,
};

static int fsl_pwm_probe(struct platform_device *pdev)
{
	struct fsl_pwm_chip *fpc;
<<<<<<< HEAD
	struct resource *res;
=======
>>>>>>> upstream/android-13
	void __iomem *base;
	int ret;

	fpc = devm_kzalloc(&pdev->dev, sizeof(*fpc), GFP_KERNEL);
	if (!fpc)
		return -ENOMEM;

	mutex_init(&fpc->lock);

	fpc->soc = of_device_get_match_data(&pdev->dev);
	fpc->chip.dev = &pdev->dev;

<<<<<<< HEAD
	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	base = devm_ioremap_resource(&pdev->dev, res);
=======
	base = devm_platform_ioremap_resource(pdev, 0);
>>>>>>> upstream/android-13
	if (IS_ERR(base))
		return PTR_ERR(base);

	fpc->regmap = devm_regmap_init_mmio_clk(&pdev->dev, "ftm_sys", base,
						&fsl_pwm_regmap_config);
	if (IS_ERR(fpc->regmap)) {
		dev_err(&pdev->dev, "regmap init failed\n");
		return PTR_ERR(fpc->regmap);
	}

	fpc->clk[FSL_PWM_CLK_SYS] = devm_clk_get(&pdev->dev, "ftm_sys");
	if (IS_ERR(fpc->clk[FSL_PWM_CLK_SYS])) {
		dev_err(&pdev->dev, "failed to get \"ftm_sys\" clock\n");
		return PTR_ERR(fpc->clk[FSL_PWM_CLK_SYS]);
	}

	fpc->clk[FSL_PWM_CLK_FIX] = devm_clk_get(fpc->chip.dev, "ftm_fix");
	if (IS_ERR(fpc->clk[FSL_PWM_CLK_FIX]))
		return PTR_ERR(fpc->clk[FSL_PWM_CLK_FIX]);

	fpc->clk[FSL_PWM_CLK_EXT] = devm_clk_get(fpc->chip.dev, "ftm_ext");
	if (IS_ERR(fpc->clk[FSL_PWM_CLK_EXT]))
		return PTR_ERR(fpc->clk[FSL_PWM_CLK_EXT]);

	fpc->clk[FSL_PWM_CLK_CNTEN] =
				devm_clk_get(fpc->chip.dev, "ftm_cnt_clk_en");
	if (IS_ERR(fpc->clk[FSL_PWM_CLK_CNTEN]))
		return PTR_ERR(fpc->clk[FSL_PWM_CLK_CNTEN]);

	/*
	 * ipg_clk is the interface clock for the IP. If not provided, use the
	 * ftm_sys clock as the default.
	 */
	fpc->ipg_clk = devm_clk_get(&pdev->dev, "ipg");
	if (IS_ERR(fpc->ipg_clk))
		fpc->ipg_clk = fpc->clk[FSL_PWM_CLK_SYS];


	fpc->chip.ops = &fsl_pwm_ops;
<<<<<<< HEAD
	fpc->chip.of_xlate = of_pwm_xlate_with_flags;
	fpc->chip.of_pwm_n_cells = 3;
	fpc->chip.base = -1;
	fpc->chip.npwm = 8;

	ret = pwmchip_add(&fpc->chip);
=======
	fpc->chip.npwm = 8;

	ret = devm_pwmchip_add(&pdev->dev, &fpc->chip);
>>>>>>> upstream/android-13
	if (ret < 0) {
		dev_err(&pdev->dev, "failed to add PWM chip: %d\n", ret);
		return ret;
	}

	platform_set_drvdata(pdev, fpc);

	return fsl_pwm_init(fpc);
}

<<<<<<< HEAD
static int fsl_pwm_remove(struct platform_device *pdev)
{
	struct fsl_pwm_chip *fpc = platform_get_drvdata(pdev);

	return pwmchip_remove(&fpc->chip);
}

=======
>>>>>>> upstream/android-13
#ifdef CONFIG_PM_SLEEP
static int fsl_pwm_suspend(struct device *dev)
{
	struct fsl_pwm_chip *fpc = dev_get_drvdata(dev);
	int i;

	regcache_cache_only(fpc->regmap, true);
	regcache_mark_dirty(fpc->regmap);

	for (i = 0; i < fpc->chip.npwm; i++) {
		struct pwm_device *pwm = &fpc->chip.pwms[i];

		if (!test_bit(PWMF_REQUESTED, &pwm->flags))
			continue;

		clk_disable_unprepare(fpc->ipg_clk);

		if (!pwm_is_enabled(pwm))
			continue;

		clk_disable_unprepare(fpc->clk[FSL_PWM_CLK_CNTEN]);
<<<<<<< HEAD
		clk_disable_unprepare(fpc->clk[fpc->cnt_select]);
=======
		clk_disable_unprepare(fpc->clk[fpc->period.clk_select]);
>>>>>>> upstream/android-13
	}

	return 0;
}

static int fsl_pwm_resume(struct device *dev)
{
	struct fsl_pwm_chip *fpc = dev_get_drvdata(dev);
	int i;

	for (i = 0; i < fpc->chip.npwm; i++) {
		struct pwm_device *pwm = &fpc->chip.pwms[i];

		if (!test_bit(PWMF_REQUESTED, &pwm->flags))
			continue;

		clk_prepare_enable(fpc->ipg_clk);

		if (!pwm_is_enabled(pwm))
			continue;

<<<<<<< HEAD
		clk_prepare_enable(fpc->clk[fpc->cnt_select]);
=======
		clk_prepare_enable(fpc->clk[fpc->period.clk_select]);
>>>>>>> upstream/android-13
		clk_prepare_enable(fpc->clk[FSL_PWM_CLK_CNTEN]);
	}

	/* restore all registers from cache */
	regcache_cache_only(fpc->regmap, false);
	regcache_sync(fpc->regmap);

	return 0;
}
#endif

static const struct dev_pm_ops fsl_pwm_pm_ops = {
	SET_SYSTEM_SLEEP_PM_OPS(fsl_pwm_suspend, fsl_pwm_resume)
};

static const struct fsl_ftm_soc vf610_ftm_pwm = {
	.has_enable_bits = false,
};

static const struct fsl_ftm_soc imx8qm_ftm_pwm = {
	.has_enable_bits = true,
};

static const struct of_device_id fsl_pwm_dt_ids[] = {
	{ .compatible = "fsl,vf610-ftm-pwm", .data = &vf610_ftm_pwm },
	{ .compatible = "fsl,imx8qm-ftm-pwm", .data = &imx8qm_ftm_pwm },
	{ /* sentinel */ }
};
MODULE_DEVICE_TABLE(of, fsl_pwm_dt_ids);

static struct platform_driver fsl_pwm_driver = {
	.driver = {
		.name = "fsl-ftm-pwm",
		.of_match_table = fsl_pwm_dt_ids,
		.pm = &fsl_pwm_pm_ops,
	},
	.probe = fsl_pwm_probe,
<<<<<<< HEAD
	.remove = fsl_pwm_remove,
=======
>>>>>>> upstream/android-13
};
module_platform_driver(fsl_pwm_driver);

MODULE_DESCRIPTION("Freescale FlexTimer Module PWM Driver");
MODULE_AUTHOR("Xiubo Li <Li.Xiubo@freescale.com>");
MODULE_ALIAS("platform:fsl-ftm-pwm");
MODULE_LICENSE("GPL");
