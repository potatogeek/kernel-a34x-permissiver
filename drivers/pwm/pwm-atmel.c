<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> upstream/android-13
/*
 * Driver for Atmel Pulse Width Modulation Controller
 *
 * Copyright (C) 2013 Atmel Corporation
 *		 Bo Shen <voice.shen@atmel.com>
 *
<<<<<<< HEAD
 * Licensed under GPLv2.
=======
 * Links to reference manuals for the supported PWM chips can be found in
 * Documentation/arm/microchip.rst.
 *
 * Limitations:
 * - Periods start with the inactive level.
 * - Hardware has to be stopped in general to update settings.
 *
 * Software bugs/possible improvements:
 * - When atmel_pwm_apply() is called with state->enabled=false a change in
 *   state->polarity isn't honored.
 * - Instead of sleeping to wait for a completed period, the interrupt
 *   functionality could be used.
>>>>>>> upstream/android-13
 */

#include <linux/clk.h>
#include <linux/delay.h>
#include <linux/err.h>
#include <linux/io.h>
#include <linux/module.h>
#include <linux/mutex.h>
#include <linux/of.h>
#include <linux/of_device.h>
#include <linux/platform_device.h>
#include <linux/pwm.h>
#include <linux/slab.h>

/* The following is global registers for PWM controller */
#define PWM_ENA			0x04
#define PWM_DIS			0x08
#define PWM_SR			0x0C
#define PWM_ISR			0x1C
/* Bit field in SR */
#define PWM_SR_ALL_CH_ON	0x0F

/* The following register is PWM channel related registers */
#define PWM_CH_REG_OFFSET	0x200
#define PWM_CH_REG_SIZE		0x20

#define PWM_CMR			0x0
/* Bit field in CMR */
#define PWM_CMR_CPOL		(1 << 9)
#define PWM_CMR_UPD_CDTY	(1 << 10)
#define PWM_CMR_CPRE_MSK	0xF

/* The following registers for PWM v1 */
#define PWMV1_CDTY		0x04
#define PWMV1_CPRD		0x08
#define PWMV1_CUPD		0x10

/* The following registers for PWM v2 */
#define PWMV2_CDTY		0x04
#define PWMV2_CDTYUPD		0x08
#define PWMV2_CPRD		0x0C
#define PWMV2_CPRDUPD		0x10

<<<<<<< HEAD
/*
 * Max value for duty and period
 *
 * Although the duty and period register is 32 bit,
 * however only the LSB 16 bits are significant.
 */
#define PWM_MAX_DTY		0xFFFF
#define PWM_MAX_PRD		0xFFFF
#define PRD_MAX_PRES		10
=======
#define PWM_MAX_PRES		10
>>>>>>> upstream/android-13

struct atmel_pwm_registers {
	u8 period;
	u8 period_upd;
	u8 duty;
	u8 duty_upd;
};

<<<<<<< HEAD
=======
struct atmel_pwm_config {
	u32 period_bits;
};

struct atmel_pwm_data {
	struct atmel_pwm_registers regs;
	struct atmel_pwm_config cfg;
};

>>>>>>> upstream/android-13
struct atmel_pwm_chip {
	struct pwm_chip chip;
	struct clk *clk;
	void __iomem *base;
<<<<<<< HEAD
	const struct atmel_pwm_registers *regs;

	unsigned int updated_pwms;
	/* ISR is cleared when read, ensure only one thread does that */
	struct mutex isr_lock;
=======
	const struct atmel_pwm_data *data;

	/*
	 * The hardware supports a mechanism to update a channel's duty cycle at
	 * the end of the currently running period. When such an update is
	 * pending we delay disabling the PWM until the new configuration is
	 * active because otherwise pmw_config(duty_cycle=0); pwm_disable();
	 * might not result in an inactive output.
	 * This bitmask tracks for which channels an update is pending in
	 * hardware.
	 */
	u32 update_pending;

	/* Protects .update_pending */
	spinlock_t lock;
>>>>>>> upstream/android-13
};

static inline struct atmel_pwm_chip *to_atmel_pwm_chip(struct pwm_chip *chip)
{
	return container_of(chip, struct atmel_pwm_chip, chip);
}

static inline u32 atmel_pwm_readl(struct atmel_pwm_chip *chip,
				  unsigned long offset)
{
	return readl_relaxed(chip->base + offset);
}

static inline void atmel_pwm_writel(struct atmel_pwm_chip *chip,
				    unsigned long offset, unsigned long val)
{
	writel_relaxed(val, chip->base + offset);
}

static inline u32 atmel_pwm_ch_readl(struct atmel_pwm_chip *chip,
				     unsigned int ch, unsigned long offset)
{
	unsigned long base = PWM_CH_REG_OFFSET + ch * PWM_CH_REG_SIZE;

<<<<<<< HEAD
	return readl_relaxed(chip->base + base + offset);
=======
	return atmel_pwm_readl(chip, base + offset);
>>>>>>> upstream/android-13
}

static inline void atmel_pwm_ch_writel(struct atmel_pwm_chip *chip,
				       unsigned int ch, unsigned long offset,
				       unsigned long val)
{
	unsigned long base = PWM_CH_REG_OFFSET + ch * PWM_CH_REG_SIZE;

<<<<<<< HEAD
	writel_relaxed(val, chip->base + base + offset);
}

static int atmel_pwm_calculate_cprd_and_pres(struct pwm_chip *chip,
=======
	atmel_pwm_writel(chip, base + offset, val);
}

static void atmel_pwm_update_pending(struct atmel_pwm_chip *chip)
{
	/*
	 * Each channel that has its bit in ISR set started a new period since
	 * ISR was cleared and so there is no more update pending.  Note that
	 * reading ISR clears it, so this needs to handle all channels to not
	 * loose information.
	 */
	u32 isr = atmel_pwm_readl(chip, PWM_ISR);

	chip->update_pending &= ~isr;
}

static void atmel_pwm_set_pending(struct atmel_pwm_chip *chip, unsigned int ch)
{
	spin_lock(&chip->lock);

	/*
	 * Clear pending flags in hardware because otherwise there might still
	 * be a stale flag in ISR.
	 */
	atmel_pwm_update_pending(chip);

	chip->update_pending |= (1 << ch);

	spin_unlock(&chip->lock);
}

static int atmel_pwm_test_pending(struct atmel_pwm_chip *chip, unsigned int ch)
{
	int ret = 0;

	spin_lock(&chip->lock);

	if (chip->update_pending & (1 << ch)) {
		atmel_pwm_update_pending(chip);

		if (chip->update_pending & (1 << ch))
			ret = 1;
	}

	spin_unlock(&chip->lock);

	return ret;
}

static int atmel_pwm_wait_nonpending(struct atmel_pwm_chip *chip, unsigned int ch)
{
	unsigned long timeout = jiffies + 2 * HZ;
	int ret;

	while ((ret = atmel_pwm_test_pending(chip, ch)) &&
	       time_before(jiffies, timeout))
		usleep_range(10, 100);

	return ret ? -ETIMEDOUT : 0;
}

static int atmel_pwm_calculate_cprd_and_pres(struct pwm_chip *chip,
					     unsigned long clkrate,
>>>>>>> upstream/android-13
					     const struct pwm_state *state,
					     unsigned long *cprd, u32 *pres)
{
	struct atmel_pwm_chip *atmel_pwm = to_atmel_pwm_chip(chip);
	unsigned long long cycles = state->period;
<<<<<<< HEAD

	/* Calculate the period cycles and prescale value */
	cycles *= clk_get_rate(atmel_pwm->clk);
	do_div(cycles, NSEC_PER_SEC);

	for (*pres = 0; cycles > PWM_MAX_PRD; cycles >>= 1)
		(*pres)++;

	if (*pres > PRD_MAX_PRES) {
		dev_err(chip->dev, "pres exceeds the maximum value\n");
		return -EINVAL;
=======
	int shift;

	/* Calculate the period cycles and prescale value */
	cycles *= clkrate;
	do_div(cycles, NSEC_PER_SEC);

	/*
	 * The register for the period length is cfg.period_bits bits wide.
	 * So for each bit the number of clock cycles is wider divide the input
	 * clock frequency by two using pres and shift cprd accordingly.
	 */
	shift = fls(cycles) - atmel_pwm->data->cfg.period_bits;

	if (shift > PWM_MAX_PRES) {
		dev_err(chip->dev, "pres exceeds the maximum value\n");
		return -EINVAL;
	} else if (shift > 0) {
		*pres = shift;
		cycles >>= *pres;
	} else {
		*pres = 0;
>>>>>>> upstream/android-13
	}

	*cprd = cycles;

	return 0;
}

static void atmel_pwm_calculate_cdty(const struct pwm_state *state,
<<<<<<< HEAD
				     unsigned long cprd, unsigned long *cdty)
{
	unsigned long long cycles = state->duty_cycle;

	cycles *= cprd;
	do_div(cycles, state->period);
=======
				     unsigned long clkrate, unsigned long cprd,
				     u32 pres, unsigned long *cdty)
{
	unsigned long long cycles = state->duty_cycle;

	cycles *= clkrate;
	do_div(cycles, NSEC_PER_SEC);
	cycles >>= pres;
>>>>>>> upstream/android-13
	*cdty = cprd - cycles;
}

static void atmel_pwm_update_cdty(struct pwm_chip *chip, struct pwm_device *pwm,
				  unsigned long cdty)
{
	struct atmel_pwm_chip *atmel_pwm = to_atmel_pwm_chip(chip);
	u32 val;

<<<<<<< HEAD
	if (atmel_pwm->regs->duty_upd ==
	    atmel_pwm->regs->period_upd) {
=======
	if (atmel_pwm->data->regs.duty_upd ==
	    atmel_pwm->data->regs.period_upd) {
>>>>>>> upstream/android-13
		val = atmel_pwm_ch_readl(atmel_pwm, pwm->hwpwm, PWM_CMR);
		val &= ~PWM_CMR_UPD_CDTY;
		atmel_pwm_ch_writel(atmel_pwm, pwm->hwpwm, PWM_CMR, val);
	}

	atmel_pwm_ch_writel(atmel_pwm, pwm->hwpwm,
<<<<<<< HEAD
			    atmel_pwm->regs->duty_upd, cdty);
=======
			    atmel_pwm->data->regs.duty_upd, cdty);
	atmel_pwm_set_pending(atmel_pwm, pwm->hwpwm);
>>>>>>> upstream/android-13
}

static void atmel_pwm_set_cprd_cdty(struct pwm_chip *chip,
				    struct pwm_device *pwm,
				    unsigned long cprd, unsigned long cdty)
{
	struct atmel_pwm_chip *atmel_pwm = to_atmel_pwm_chip(chip);

	atmel_pwm_ch_writel(atmel_pwm, pwm->hwpwm,
<<<<<<< HEAD
			    atmel_pwm->regs->duty, cdty);
	atmel_pwm_ch_writel(atmel_pwm, pwm->hwpwm,
			    atmel_pwm->regs->period, cprd);
=======
			    atmel_pwm->data->regs.duty, cdty);
	atmel_pwm_ch_writel(atmel_pwm, pwm->hwpwm,
			    atmel_pwm->data->regs.period, cprd);
>>>>>>> upstream/android-13
}

static void atmel_pwm_disable(struct pwm_chip *chip, struct pwm_device *pwm,
			      bool disable_clk)
{
	struct atmel_pwm_chip *atmel_pwm = to_atmel_pwm_chip(chip);
	unsigned long timeout = jiffies + 2 * HZ;

<<<<<<< HEAD
	/*
	 * Wait for at least a complete period to have passed before disabling a
	 * channel to be sure that CDTY has been updated
	 */
	mutex_lock(&atmel_pwm->isr_lock);
	atmel_pwm->updated_pwms |= atmel_pwm_readl(atmel_pwm, PWM_ISR);

	while (!(atmel_pwm->updated_pwms & (1 << pwm->hwpwm)) &&
	       time_before(jiffies, timeout)) {
		usleep_range(10, 100);
		atmel_pwm->updated_pwms |= atmel_pwm_readl(atmel_pwm, PWM_ISR);
	}

	mutex_unlock(&atmel_pwm->isr_lock);
=======
	atmel_pwm_wait_nonpending(atmel_pwm, pwm->hwpwm);

>>>>>>> upstream/android-13
	atmel_pwm_writel(atmel_pwm, PWM_DIS, 1 << pwm->hwpwm);

	/*
	 * Wait for the PWM channel disable operation to be effective before
	 * stopping the clock.
	 */
	timeout = jiffies + 2 * HZ;

	while ((atmel_pwm_readl(atmel_pwm, PWM_SR) & (1 << pwm->hwpwm)) &&
	       time_before(jiffies, timeout))
		usleep_range(10, 100);

	if (disable_clk)
		clk_disable(atmel_pwm->clk);
}

static int atmel_pwm_apply(struct pwm_chip *chip, struct pwm_device *pwm,
<<<<<<< HEAD
			   struct pwm_state *state)
=======
			   const struct pwm_state *state)
>>>>>>> upstream/android-13
{
	struct atmel_pwm_chip *atmel_pwm = to_atmel_pwm_chip(chip);
	struct pwm_state cstate;
	unsigned long cprd, cdty;
	u32 pres, val;
	int ret;

	pwm_get_state(pwm, &cstate);

	if (state->enabled) {
<<<<<<< HEAD
		if (cstate.enabled &&
		    cstate.polarity == state->polarity &&
		    cstate.period == state->period) {
			cprd = atmel_pwm_ch_readl(atmel_pwm, pwm->hwpwm,
						  atmel_pwm->regs->period);
			atmel_pwm_calculate_cdty(state, cprd, &cdty);
=======
		unsigned long clkrate = clk_get_rate(atmel_pwm->clk);

		if (cstate.enabled &&
		    cstate.polarity == state->polarity &&
		    cstate.period == state->period) {
			u32 cmr = atmel_pwm_ch_readl(atmel_pwm, pwm->hwpwm, PWM_CMR);

			cprd = atmel_pwm_ch_readl(atmel_pwm, pwm->hwpwm,
						  atmel_pwm->data->regs.period);
			pres = cmr & PWM_CMR_CPRE_MSK;

			atmel_pwm_calculate_cdty(state, clkrate, cprd, pres, &cdty);
>>>>>>> upstream/android-13
			atmel_pwm_update_cdty(chip, pwm, cdty);
			return 0;
		}

<<<<<<< HEAD
		ret = atmel_pwm_calculate_cprd_and_pres(chip, state, &cprd,
=======
		ret = atmel_pwm_calculate_cprd_and_pres(chip, clkrate, state, &cprd,
>>>>>>> upstream/android-13
							&pres);
		if (ret) {
			dev_err(chip->dev,
				"failed to calculate cprd and prescaler\n");
			return ret;
		}

<<<<<<< HEAD
		atmel_pwm_calculate_cdty(state, cprd, &cdty);
=======
		atmel_pwm_calculate_cdty(state, clkrate, cprd, pres, &cdty);
>>>>>>> upstream/android-13

		if (cstate.enabled) {
			atmel_pwm_disable(chip, pwm, false);
		} else {
			ret = clk_enable(atmel_pwm->clk);
			if (ret) {
				dev_err(chip->dev, "failed to enable clock\n");
				return ret;
			}
		}

		/* It is necessary to preserve CPOL, inside CMR */
		val = atmel_pwm_ch_readl(atmel_pwm, pwm->hwpwm, PWM_CMR);
		val = (val & ~PWM_CMR_CPRE_MSK) | (pres & PWM_CMR_CPRE_MSK);
		if (state->polarity == PWM_POLARITY_NORMAL)
			val &= ~PWM_CMR_CPOL;
		else
			val |= PWM_CMR_CPOL;
		atmel_pwm_ch_writel(atmel_pwm, pwm->hwpwm, PWM_CMR, val);
		atmel_pwm_set_cprd_cdty(chip, pwm, cprd, cdty);
<<<<<<< HEAD
		mutex_lock(&atmel_pwm->isr_lock);
		atmel_pwm->updated_pwms |= atmel_pwm_readl(atmel_pwm, PWM_ISR);
		atmel_pwm->updated_pwms &= ~(1 << pwm->hwpwm);
		mutex_unlock(&atmel_pwm->isr_lock);
=======
>>>>>>> upstream/android-13
		atmel_pwm_writel(atmel_pwm, PWM_ENA, 1 << pwm->hwpwm);
	} else if (cstate.enabled) {
		atmel_pwm_disable(chip, pwm, true);
	}

	return 0;
}

<<<<<<< HEAD
static const struct pwm_ops atmel_pwm_ops = {
	.apply = atmel_pwm_apply,
	.owner = THIS_MODULE,
};

static const struct atmel_pwm_registers atmel_pwm_regs_v1 = {
	.period		= PWMV1_CPRD,
	.period_upd	= PWMV1_CUPD,
	.duty		= PWMV1_CDTY,
	.duty_upd	= PWMV1_CUPD,
};

static const struct atmel_pwm_registers atmel_pwm_regs_v2 = {
	.period		= PWMV2_CPRD,
	.period_upd	= PWMV2_CPRDUPD,
	.duty		= PWMV2_CDTY,
	.duty_upd	= PWMV2_CDTYUPD,
};

static const struct platform_device_id atmel_pwm_devtypes[] = {
	{
		.name = "at91sam9rl-pwm",
		.driver_data = (kernel_ulong_t)&atmel_pwm_regs_v1,
	}, {
		.name = "sama5d3-pwm",
		.driver_data = (kernel_ulong_t)&atmel_pwm_regs_v2,
	}, {
		/* sentinel */
	},
};
MODULE_DEVICE_TABLE(platform, atmel_pwm_devtypes);
=======
static void atmel_pwm_get_state(struct pwm_chip *chip, struct pwm_device *pwm,
				struct pwm_state *state)
{
	struct atmel_pwm_chip *atmel_pwm = to_atmel_pwm_chip(chip);
	u32 sr, cmr;

	sr = atmel_pwm_readl(atmel_pwm, PWM_SR);
	cmr = atmel_pwm_ch_readl(atmel_pwm, pwm->hwpwm, PWM_CMR);

	if (sr & (1 << pwm->hwpwm)) {
		unsigned long rate = clk_get_rate(atmel_pwm->clk);
		u32 cdty, cprd, pres;
		u64 tmp;

		pres = cmr & PWM_CMR_CPRE_MSK;

		cprd = atmel_pwm_ch_readl(atmel_pwm, pwm->hwpwm,
					  atmel_pwm->data->regs.period);
		tmp = (u64)cprd * NSEC_PER_SEC;
		tmp <<= pres;
		state->period = DIV64_U64_ROUND_UP(tmp, rate);

		/* Wait for an updated duty_cycle queued in hardware */
		atmel_pwm_wait_nonpending(atmel_pwm, pwm->hwpwm);

		cdty = atmel_pwm_ch_readl(atmel_pwm, pwm->hwpwm,
					  atmel_pwm->data->regs.duty);
		tmp = (u64)(cprd - cdty) * NSEC_PER_SEC;
		tmp <<= pres;
		state->duty_cycle = DIV64_U64_ROUND_UP(tmp, rate);

		state->enabled = true;
	} else {
		state->enabled = false;
	}

	if (cmr & PWM_CMR_CPOL)
		state->polarity = PWM_POLARITY_INVERSED;
	else
		state->polarity = PWM_POLARITY_NORMAL;
}

static const struct pwm_ops atmel_pwm_ops = {
	.apply = atmel_pwm_apply,
	.get_state = atmel_pwm_get_state,
	.owner = THIS_MODULE,
};

static const struct atmel_pwm_data atmel_sam9rl_pwm_data = {
	.regs = {
		.period		= PWMV1_CPRD,
		.period_upd	= PWMV1_CUPD,
		.duty		= PWMV1_CDTY,
		.duty_upd	= PWMV1_CUPD,
	},
	.cfg = {
		/* 16 bits to keep period and duty. */
		.period_bits	= 16,
	},
};

static const struct atmel_pwm_data atmel_sama5_pwm_data = {
	.regs = {
		.period		= PWMV2_CPRD,
		.period_upd	= PWMV2_CPRDUPD,
		.duty		= PWMV2_CDTY,
		.duty_upd	= PWMV2_CDTYUPD,
	},
	.cfg = {
		/* 16 bits to keep period and duty. */
		.period_bits	= 16,
	},
};

static const struct atmel_pwm_data mchp_sam9x60_pwm_data = {
	.regs = {
		.period		= PWMV1_CPRD,
		.period_upd	= PWMV1_CUPD,
		.duty		= PWMV1_CDTY,
		.duty_upd	= PWMV1_CUPD,
	},
	.cfg = {
		/* 32 bits to keep period and duty. */
		.period_bits	= 32,
	},
};
>>>>>>> upstream/android-13

static const struct of_device_id atmel_pwm_dt_ids[] = {
	{
		.compatible = "atmel,at91sam9rl-pwm",
<<<<<<< HEAD
		.data = &atmel_pwm_regs_v1,
	}, {
		.compatible = "atmel,sama5d3-pwm",
		.data = &atmel_pwm_regs_v2,
	}, {
		.compatible = "atmel,sama5d2-pwm",
		.data = &atmel_pwm_regs_v2,
=======
		.data = &atmel_sam9rl_pwm_data,
	}, {
		.compatible = "atmel,sama5d3-pwm",
		.data = &atmel_sama5_pwm_data,
	}, {
		.compatible = "atmel,sama5d2-pwm",
		.data = &atmel_sama5_pwm_data,
	}, {
		.compatible = "microchip,sam9x60-pwm",
		.data = &mchp_sam9x60_pwm_data,
>>>>>>> upstream/android-13
	}, {
		/* sentinel */
	},
};
MODULE_DEVICE_TABLE(of, atmel_pwm_dt_ids);

<<<<<<< HEAD
static inline const struct atmel_pwm_registers *
atmel_pwm_get_driver_data(struct platform_device *pdev)
{
	const struct platform_device_id *id;

	if (pdev->dev.of_node)
		return of_device_get_match_data(&pdev->dev);

	id = platform_get_device_id(pdev);

	return (struct atmel_pwm_registers *)id->driver_data;
}

static int atmel_pwm_probe(struct platform_device *pdev)
{
	const struct atmel_pwm_registers *regs;
	struct atmel_pwm_chip *atmel_pwm;
	struct resource *res;
	int ret;

	regs = atmel_pwm_get_driver_data(pdev);
	if (!regs)
		return -ENODEV;

=======
static int atmel_pwm_probe(struct platform_device *pdev)
{
	struct atmel_pwm_chip *atmel_pwm;
	int ret;

>>>>>>> upstream/android-13
	atmel_pwm = devm_kzalloc(&pdev->dev, sizeof(*atmel_pwm), GFP_KERNEL);
	if (!atmel_pwm)
		return -ENOMEM;

<<<<<<< HEAD
	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	atmel_pwm->base = devm_ioremap_resource(&pdev->dev, res);
=======
	atmel_pwm->data = of_device_get_match_data(&pdev->dev);

	atmel_pwm->update_pending = 0;
	spin_lock_init(&atmel_pwm->lock);

	atmel_pwm->base = devm_platform_ioremap_resource(pdev, 0);
>>>>>>> upstream/android-13
	if (IS_ERR(atmel_pwm->base))
		return PTR_ERR(atmel_pwm->base);

	atmel_pwm->clk = devm_clk_get(&pdev->dev, NULL);
	if (IS_ERR(atmel_pwm->clk))
		return PTR_ERR(atmel_pwm->clk);

	ret = clk_prepare(atmel_pwm->clk);
	if (ret) {
		dev_err(&pdev->dev, "failed to prepare PWM clock\n");
		return ret;
	}

	atmel_pwm->chip.dev = &pdev->dev;
	atmel_pwm->chip.ops = &atmel_pwm_ops;
<<<<<<< HEAD

	if (pdev->dev.of_node) {
		atmel_pwm->chip.of_xlate = of_pwm_xlate_with_flags;
		atmel_pwm->chip.of_pwm_n_cells = 3;
	}

	atmel_pwm->chip.base = -1;
	atmel_pwm->chip.npwm = 4;
	atmel_pwm->regs = regs;
	atmel_pwm->updated_pwms = 0;
	mutex_init(&atmel_pwm->isr_lock);
=======
	atmel_pwm->chip.npwm = 4;
>>>>>>> upstream/android-13

	ret = pwmchip_add(&atmel_pwm->chip);
	if (ret < 0) {
		dev_err(&pdev->dev, "failed to add PWM chip %d\n", ret);
		goto unprepare_clk;
	}

	platform_set_drvdata(pdev, atmel_pwm);

	return ret;

unprepare_clk:
	clk_unprepare(atmel_pwm->clk);
	return ret;
}

static int atmel_pwm_remove(struct platform_device *pdev)
{
	struct atmel_pwm_chip *atmel_pwm = platform_get_drvdata(pdev);

<<<<<<< HEAD
	clk_unprepare(atmel_pwm->clk);
	mutex_destroy(&atmel_pwm->isr_lock);

	return pwmchip_remove(&atmel_pwm->chip);
=======
	pwmchip_remove(&atmel_pwm->chip);

	clk_unprepare(atmel_pwm->clk);

	return 0;
>>>>>>> upstream/android-13
}

static struct platform_driver atmel_pwm_driver = {
	.driver = {
		.name = "atmel-pwm",
		.of_match_table = of_match_ptr(atmel_pwm_dt_ids),
	},
<<<<<<< HEAD
	.id_table = atmel_pwm_devtypes,
=======
>>>>>>> upstream/android-13
	.probe = atmel_pwm_probe,
	.remove = atmel_pwm_remove,
};
module_platform_driver(atmel_pwm_driver);

MODULE_ALIAS("platform:atmel-pwm");
MODULE_AUTHOR("Bo Shen <voice.shen@atmel.com>");
MODULE_DESCRIPTION("Atmel PWM driver");
MODULE_LICENSE("GPL v2");
