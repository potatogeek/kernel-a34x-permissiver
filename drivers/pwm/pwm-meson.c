<<<<<<< HEAD
/*
 * This file is provided under a dual BSD/GPLv2 license.  When using or
 * redistributing this file, you may do so under either license.
 *
 * GPL LICENSE SUMMARY
=======
// SPDX-License-Identifier: GPL-2.0 OR BSD-3-Clause
/*
 * PWM controller driver for Amlogic Meson SoCs.
 *
 * This PWM is only a set of Gates, Dividers and Counters:
 * PWM output is achieved by calculating a clock that permits calculating
 * two periods (low and high). The counter then has to be set to switch after
 * N cycles for the first half period.
 * The hardware has no "polarity" setting. This driver reverses the period
 * cycles (the low length is inverted with the high length) for
 * PWM_POLARITY_INVERSED. This means that .get_state cannot read the polarity
 * from the hardware.
 * Setting the duty cycle will disable and re-enable the PWM output.
 * Disabling the PWM stops the output immediately (without waiting for the
 * current period to complete first).
 *
 * The public S912 (GXM) datasheet contains some documentation for this PWM
 * controller starting on page 543:
 * https://dl.khadas.com/Hardware/VIM2/Datasheet/S912_Datasheet_V0.220170314publicversion-Wesion.pdf
 * An updated version of this IP block is found in S922X (G12B) SoCs. The
 * datasheet contains the description for this IP block revision starting at
 * page 1084:
 * https://dn.odroid.com/S922X/ODROID-N2/Datasheet/S922X_Public_Datasheet_V0.2.pdf
>>>>>>> upstream/android-13
 *
 * Copyright (c) 2016 BayLibre, SAS.
 * Author: Neil Armstrong <narmstrong@baylibre.com>
 * Copyright (C) 2014 Amlogic, Inc.
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of version 2 of the GNU General Public License as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see <http://www.gnu.org/licenses/>.
 * The full GNU General Public License is included in this distribution
 * in the file called COPYING.
 *
 * BSD LICENSE
 *
 * Copyright (c) 2016 BayLibre, SAS.
 * Author: Neil Armstrong <narmstrong@baylibre.com>
 * Copyright (C) 2014 Amlogic, Inc.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in
 *     the documentation and/or other materials provided with the
 *     distribution.
 *   * Neither the name of Intel Corporation nor the names of its
 *     contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

=======
 */

#include <linux/bitfield.h>
#include <linux/bits.h>
>>>>>>> upstream/android-13
#include <linux/clk.h>
#include <linux/clk-provider.h>
#include <linux/err.h>
#include <linux/io.h>
#include <linux/kernel.h>
<<<<<<< HEAD
=======
#include <linux/math64.h>
>>>>>>> upstream/android-13
#include <linux/module.h>
#include <linux/of.h>
#include <linux/of_device.h>
#include <linux/platform_device.h>
#include <linux/pwm.h>
#include <linux/slab.h>
#include <linux/spinlock.h>

#define REG_PWM_A		0x0
#define REG_PWM_B		0x4
<<<<<<< HEAD
#define PWM_HIGH_SHIFT		16
=======
#define PWM_LOW_MASK		GENMASK(15, 0)
#define PWM_HIGH_MASK		GENMASK(31, 16)
>>>>>>> upstream/android-13

#define REG_MISC_AB		0x8
#define MISC_B_CLK_EN		BIT(23)
#define MISC_A_CLK_EN		BIT(15)
#define MISC_CLK_DIV_MASK	0x7f
#define MISC_B_CLK_DIV_SHIFT	16
#define MISC_A_CLK_DIV_SHIFT	8
#define MISC_B_CLK_SEL_SHIFT	6
#define MISC_A_CLK_SEL_SHIFT	4
<<<<<<< HEAD
#define MISC_CLK_SEL_WIDTH	2
#define MISC_B_EN		BIT(1)
#define MISC_A_EN		BIT(0)

static const unsigned int mux_reg_shifts[] = {
	MISC_A_CLK_SEL_SHIFT,
	MISC_B_CLK_SEL_SHIFT
=======
#define MISC_CLK_SEL_MASK	0x3
#define MISC_B_EN		BIT(1)
#define MISC_A_EN		BIT(0)

#define MESON_NUM_PWMS		2

static struct meson_pwm_channel_data {
	u8		reg_offset;
	u8		clk_sel_shift;
	u8		clk_div_shift;
	u32		clk_en_mask;
	u32		pwm_en_mask;
} meson_pwm_per_channel_data[MESON_NUM_PWMS] = {
	{
		.reg_offset	= REG_PWM_A,
		.clk_sel_shift	= MISC_A_CLK_SEL_SHIFT,
		.clk_div_shift	= MISC_A_CLK_DIV_SHIFT,
		.clk_en_mask	= MISC_A_CLK_EN,
		.pwm_en_mask	= MISC_A_EN,
	},
	{
		.reg_offset	= REG_PWM_B,
		.clk_sel_shift	= MISC_B_CLK_SEL_SHIFT,
		.clk_div_shift	= MISC_B_CLK_DIV_SHIFT,
		.clk_en_mask	= MISC_B_CLK_EN,
		.pwm_en_mask	= MISC_B_EN,
	}
>>>>>>> upstream/android-13
};

struct meson_pwm_channel {
	unsigned int hi;
	unsigned int lo;
	u8 pre_div;

<<<<<<< HEAD
	struct pwm_state state;

=======
>>>>>>> upstream/android-13
	struct clk *clk_parent;
	struct clk_mux mux;
	struct clk *clk;
};

struct meson_pwm_data {
	const char * const *parent_names;
	unsigned int num_parents;
};

struct meson_pwm {
	struct pwm_chip chip;
	const struct meson_pwm_data *data;
<<<<<<< HEAD
	void __iomem *base;
	u8 inverter_mask;
=======
	struct meson_pwm_channel channels[MESON_NUM_PWMS];
	void __iomem *base;
>>>>>>> upstream/android-13
	/*
	 * Protects register (write) access to the REG_MISC_AB register
	 * that is shared between the two PWMs.
	 */
	spinlock_t lock;
};

static inline struct meson_pwm *to_meson_pwm(struct pwm_chip *chip)
{
	return container_of(chip, struct meson_pwm, chip);
}

static int meson_pwm_request(struct pwm_chip *chip, struct pwm_device *pwm)
{
<<<<<<< HEAD
	struct meson_pwm_channel *channel = pwm_get_chip_data(pwm);
	struct device *dev = chip->dev;
	int err;

	if (!channel)
		return -ENODEV;
=======
	struct meson_pwm *meson = to_meson_pwm(chip);
	struct meson_pwm_channel *channel;
	struct device *dev = chip->dev;
	int err;

	channel = pwm_get_chip_data(pwm);
	if (channel)
		return 0;

	channel = &meson->channels[pwm->hwpwm];
>>>>>>> upstream/android-13

	if (channel->clk_parent) {
		err = clk_set_parent(channel->clk, channel->clk_parent);
		if (err < 0) {
			dev_err(dev, "failed to set parent %s for %s: %d\n",
				__clk_get_name(channel->clk_parent),
				__clk_get_name(channel->clk), err);
<<<<<<< HEAD
				return err;
=======
			return err;
>>>>>>> upstream/android-13
		}
	}

	err = clk_prepare_enable(channel->clk);
	if (err < 0) {
		dev_err(dev, "failed to enable clock %s: %d\n",
			__clk_get_name(channel->clk), err);
		return err;
	}

<<<<<<< HEAD
	chip->ops->get_state(chip, pwm, &channel->state);

	return 0;
=======
	return pwm_set_chip_data(pwm, channel);
>>>>>>> upstream/android-13
}

static void meson_pwm_free(struct pwm_chip *chip, struct pwm_device *pwm)
{
	struct meson_pwm_channel *channel = pwm_get_chip_data(pwm);

	if (channel)
		clk_disable_unprepare(channel->clk);
}

<<<<<<< HEAD
static int meson_pwm_calc(struct meson_pwm *meson,
			  struct meson_pwm_channel *channel, unsigned int id,
			  unsigned int duty, unsigned int period)
{
	unsigned int pre_div, cnt, duty_cnt;
	unsigned long fin_freq = -1;
	u64 fin_ps;

	if (~(meson->inverter_mask >> id) & 0x1)
		duty = period - duty;

	if (period == channel->state.period &&
	    duty == channel->state.duty_cycle)
		return 0;

=======
static int meson_pwm_calc(struct meson_pwm *meson, struct pwm_device *pwm,
			  const struct pwm_state *state)
{
	struct meson_pwm_channel *channel = pwm_get_chip_data(pwm);
	unsigned int duty, period, pre_div, cnt, duty_cnt;
	unsigned long fin_freq;

	duty = state->duty_cycle;
	period = state->period;

	if (state->polarity == PWM_POLARITY_INVERSED)
		duty = period - duty;

>>>>>>> upstream/android-13
	fin_freq = clk_get_rate(channel->clk);
	if (fin_freq == 0) {
		dev_err(meson->chip.dev, "invalid source clock frequency\n");
		return -EINVAL;
	}

	dev_dbg(meson->chip.dev, "fin_freq: %lu Hz\n", fin_freq);
<<<<<<< HEAD
	fin_ps = (u64)NSEC_PER_SEC * 1000;
	do_div(fin_ps, fin_freq);

	/* Calc pre_div with the period */
	for (pre_div = 0; pre_div <= MISC_CLK_DIV_MASK; pre_div++) {
		cnt = DIV_ROUND_CLOSEST_ULL((u64)period * 1000,
					    fin_ps * (pre_div + 1));
		dev_dbg(meson->chip.dev, "fin_ps=%llu pre_div=%u cnt=%u\n",
			fin_ps, pre_div, cnt);
		if (cnt <= 0xffff)
			break;
	}

=======

	pre_div = div64_u64(fin_freq * (u64)period, NSEC_PER_SEC * 0xffffLL);
>>>>>>> upstream/android-13
	if (pre_div > MISC_CLK_DIV_MASK) {
		dev_err(meson->chip.dev, "unable to get period pre_div\n");
		return -EINVAL;
	}

<<<<<<< HEAD
=======
	cnt = div64_u64(fin_freq * (u64)period, NSEC_PER_SEC * (pre_div + 1));
	if (cnt > 0xffff) {
		dev_err(meson->chip.dev, "unable to get period cnt\n");
		return -EINVAL;
	}

>>>>>>> upstream/android-13
	dev_dbg(meson->chip.dev, "period=%u pre_div=%u cnt=%u\n", period,
		pre_div, cnt);

	if (duty == period) {
		channel->pre_div = pre_div;
		channel->hi = cnt;
		channel->lo = 0;
	} else if (duty == 0) {
		channel->pre_div = pre_div;
		channel->hi = 0;
		channel->lo = cnt;
	} else {
		/* Then check is we can have the duty with the same pre_div */
<<<<<<< HEAD
		duty_cnt = DIV_ROUND_CLOSEST_ULL((u64)duty * 1000,
						 fin_ps * (pre_div + 1));
=======
		duty_cnt = div64_u64(fin_freq * (u64)duty,
				     NSEC_PER_SEC * (pre_div + 1));
>>>>>>> upstream/android-13
		if (duty_cnt > 0xffff) {
			dev_err(meson->chip.dev, "unable to get duty cycle\n");
			return -EINVAL;
		}

		dev_dbg(meson->chip.dev, "duty=%u pre_div=%u duty_cnt=%u\n",
			duty, pre_div, duty_cnt);

		channel->pre_div = pre_div;
		channel->hi = duty_cnt;
		channel->lo = cnt - duty_cnt;
	}

	return 0;
}

<<<<<<< HEAD
static void meson_pwm_enable(struct meson_pwm *meson,
			     struct meson_pwm_channel *channel,
			     unsigned int id)
{
	u32 value, clk_shift, clk_enable, enable;
	unsigned int offset;
	unsigned long flags;

	switch (id) {
	case 0:
		clk_shift = MISC_A_CLK_DIV_SHIFT;
		clk_enable = MISC_A_CLK_EN;
		enable = MISC_A_EN;
		offset = REG_PWM_A;
		break;

	case 1:
		clk_shift = MISC_B_CLK_DIV_SHIFT;
		clk_enable = MISC_B_CLK_EN;
		enable = MISC_B_EN;
		offset = REG_PWM_B;
		break;

	default:
		return;
	}
=======
static void meson_pwm_enable(struct meson_pwm *meson, struct pwm_device *pwm)
{
	struct meson_pwm_channel *channel = pwm_get_chip_data(pwm);
	struct meson_pwm_channel_data *channel_data;
	unsigned long flags;
	u32 value;

	channel_data = &meson_pwm_per_channel_data[pwm->hwpwm];
>>>>>>> upstream/android-13

	spin_lock_irqsave(&meson->lock, flags);

	value = readl(meson->base + REG_MISC_AB);
<<<<<<< HEAD
	value &= ~(MISC_CLK_DIV_MASK << clk_shift);
	value |= channel->pre_div << clk_shift;
	value |= clk_enable;
	writel(value, meson->base + REG_MISC_AB);

	value = (channel->hi << PWM_HIGH_SHIFT) | channel->lo;
	writel(value, meson->base + offset);

	value = readl(meson->base + REG_MISC_AB);
	value |= enable;
=======
	value &= ~(MISC_CLK_DIV_MASK << channel_data->clk_div_shift);
	value |= channel->pre_div << channel_data->clk_div_shift;
	value |= channel_data->clk_en_mask;
	writel(value, meson->base + REG_MISC_AB);

	value = FIELD_PREP(PWM_HIGH_MASK, channel->hi) |
		FIELD_PREP(PWM_LOW_MASK, channel->lo);
	writel(value, meson->base + channel_data->reg_offset);

	value = readl(meson->base + REG_MISC_AB);
	value |= channel_data->pwm_en_mask;
>>>>>>> upstream/android-13
	writel(value, meson->base + REG_MISC_AB);

	spin_unlock_irqrestore(&meson->lock, flags);
}

<<<<<<< HEAD
static void meson_pwm_disable(struct meson_pwm *meson, unsigned int id)
{
	u32 value, enable;
	unsigned long flags;

	switch (id) {
	case 0:
		enable = MISC_A_EN;
		break;

	case 1:
		enable = MISC_B_EN;
		break;

	default:
		return;
	}
=======
static void meson_pwm_disable(struct meson_pwm *meson, struct pwm_device *pwm)
{
	unsigned long flags;
	u32 value;
>>>>>>> upstream/android-13

	spin_lock_irqsave(&meson->lock, flags);

	value = readl(meson->base + REG_MISC_AB);
<<<<<<< HEAD
	value &= ~enable;
=======
	value &= ~meson_pwm_per_channel_data[pwm->hwpwm].pwm_en_mask;
>>>>>>> upstream/android-13
	writel(value, meson->base + REG_MISC_AB);

	spin_unlock_irqrestore(&meson->lock, flags);
}

static int meson_pwm_apply(struct pwm_chip *chip, struct pwm_device *pwm,
<<<<<<< HEAD
			   struct pwm_state *state)
=======
			   const struct pwm_state *state)
>>>>>>> upstream/android-13
{
	struct meson_pwm_channel *channel = pwm_get_chip_data(pwm);
	struct meson_pwm *meson = to_meson_pwm(chip);
	int err = 0;

	if (!state)
		return -EINVAL;

	if (!state->enabled) {
<<<<<<< HEAD
		meson_pwm_disable(meson, pwm->hwpwm);
		channel->state.enabled = false;

		return 0;
	}

	if (state->period != channel->state.period ||
	    state->duty_cycle != channel->state.duty_cycle ||
	    state->polarity != channel->state.polarity) {
		if (state->polarity != channel->state.polarity) {
			if (state->polarity == PWM_POLARITY_NORMAL)
				meson->inverter_mask |= BIT(pwm->hwpwm);
			else
				meson->inverter_mask &= ~BIT(pwm->hwpwm);
		}

		err = meson_pwm_calc(meson, channel, pwm->hwpwm,
				     state->duty_cycle, state->period);
		if (err < 0)
			return err;

		channel->state.polarity = state->polarity;
		channel->state.period = state->period;
		channel->state.duty_cycle = state->duty_cycle;
	}

	if (state->enabled && !channel->state.enabled) {
		meson_pwm_enable(meson, channel, pwm->hwpwm);
		channel->state.enabled = true;
=======
		if (state->polarity == PWM_POLARITY_INVERSED) {
			/*
			 * This IP block revision doesn't have an "always high"
			 * setting which we can use for "inverted disabled".
			 * Instead we achieve this using the same settings
			 * that we use a pre_div of 0 (to get the shortest
			 * possible duration for one "count") and
			 * "period == duty_cycle". This results in a signal
			 * which is LOW for one "count", while being HIGH for
			 * the rest of the (so the signal is HIGH for slightly
			 * less than 100% of the period, but this is the best
			 * we can achieve).
			 */
			channel->pre_div = 0;
			channel->hi = ~0;
			channel->lo = 0;

			meson_pwm_enable(meson, pwm);
		} else {
			meson_pwm_disable(meson, pwm);
		}
	} else {
		err = meson_pwm_calc(meson, pwm, state);
		if (err < 0)
			return err;

		meson_pwm_enable(meson, pwm);
>>>>>>> upstream/android-13
	}

	return 0;
}

<<<<<<< HEAD
=======
static unsigned int meson_pwm_cnt_to_ns(struct pwm_chip *chip,
					struct pwm_device *pwm, u32 cnt)
{
	struct meson_pwm *meson = to_meson_pwm(chip);
	struct meson_pwm_channel *channel;
	unsigned long fin_freq;
	u32 fin_ns;

	/* to_meson_pwm() can only be used after .get_state() is called */
	channel = &meson->channels[pwm->hwpwm];

	fin_freq = clk_get_rate(channel->clk);
	if (fin_freq == 0)
		return 0;

	fin_ns = div_u64(NSEC_PER_SEC, fin_freq);

	return cnt * fin_ns * (channel->pre_div + 1);
}

>>>>>>> upstream/android-13
static void meson_pwm_get_state(struct pwm_chip *chip, struct pwm_device *pwm,
				struct pwm_state *state)
{
	struct meson_pwm *meson = to_meson_pwm(chip);
<<<<<<< HEAD
	u32 value, mask;
=======
	struct meson_pwm_channel_data *channel_data;
	struct meson_pwm_channel *channel;
	u32 value, tmp;
>>>>>>> upstream/android-13

	if (!state)
		return;

<<<<<<< HEAD
	switch (pwm->hwpwm) {
	case 0:
		mask = MISC_A_EN;
		break;

	case 1:
		mask = MISC_B_EN;
		break;

	default:
		return;
	}

	value = readl(meson->base + REG_MISC_AB);
	state->enabled = (value & mask) != 0;
=======
	channel = &meson->channels[pwm->hwpwm];
	channel_data = &meson_pwm_per_channel_data[pwm->hwpwm];

	value = readl(meson->base + REG_MISC_AB);

	tmp = channel_data->pwm_en_mask | channel_data->clk_en_mask;
	state->enabled = (value & tmp) == tmp;

	tmp = value >> channel_data->clk_div_shift;
	channel->pre_div = FIELD_GET(MISC_CLK_DIV_MASK, tmp);

	value = readl(meson->base + channel_data->reg_offset);

	channel->lo = FIELD_GET(PWM_LOW_MASK, value);
	channel->hi = FIELD_GET(PWM_HIGH_MASK, value);

	if (channel->lo == 0) {
		state->period = meson_pwm_cnt_to_ns(chip, pwm, channel->hi);
		state->duty_cycle = state->period;
	} else if (channel->lo >= channel->hi) {
		state->period = meson_pwm_cnt_to_ns(chip, pwm,
						    channel->lo + channel->hi);
		state->duty_cycle = meson_pwm_cnt_to_ns(chip, pwm,
							channel->hi);
	} else {
		state->period = 0;
		state->duty_cycle = 0;
	}
>>>>>>> upstream/android-13
}

static const struct pwm_ops meson_pwm_ops = {
	.request = meson_pwm_request,
	.free = meson_pwm_free,
	.apply = meson_pwm_apply,
	.get_state = meson_pwm_get_state,
	.owner = THIS_MODULE,
};

static const char * const pwm_meson8b_parent_names[] = {
	"xtal", "vid_pll", "fclk_div4", "fclk_div3"
};

static const struct meson_pwm_data pwm_meson8b_data = {
	.parent_names = pwm_meson8b_parent_names,
	.num_parents = ARRAY_SIZE(pwm_meson8b_parent_names),
};

static const char * const pwm_gxbb_parent_names[] = {
	"xtal", "hdmi_pll", "fclk_div4", "fclk_div3"
};

static const struct meson_pwm_data pwm_gxbb_data = {
	.parent_names = pwm_gxbb_parent_names,
	.num_parents = ARRAY_SIZE(pwm_gxbb_parent_names),
};

/*
 * Only the 2 first inputs of the GXBB AO PWMs are valid
 * The last 2 are grounded
 */
static const char * const pwm_gxbb_ao_parent_names[] = {
	"xtal", "clk81"
};

static const struct meson_pwm_data pwm_gxbb_ao_data = {
	.parent_names = pwm_gxbb_ao_parent_names,
	.num_parents = ARRAY_SIZE(pwm_gxbb_ao_parent_names),
};

static const char * const pwm_axg_ee_parent_names[] = {
	"xtal", "fclk_div5", "fclk_div4", "fclk_div3"
};

static const struct meson_pwm_data pwm_axg_ee_data = {
	.parent_names = pwm_axg_ee_parent_names,
	.num_parents = ARRAY_SIZE(pwm_axg_ee_parent_names),
};

static const char * const pwm_axg_ao_parent_names[] = {
	"aoclk81", "xtal", "fclk_div4", "fclk_div5"
};

static const struct meson_pwm_data pwm_axg_ao_data = {
	.parent_names = pwm_axg_ao_parent_names,
	.num_parents = ARRAY_SIZE(pwm_axg_ao_parent_names),
};

<<<<<<< HEAD
=======
static const char * const pwm_g12a_ao_ab_parent_names[] = {
	"xtal", "aoclk81", "fclk_div4", "fclk_div5"
};

static const struct meson_pwm_data pwm_g12a_ao_ab_data = {
	.parent_names = pwm_g12a_ao_ab_parent_names,
	.num_parents = ARRAY_SIZE(pwm_g12a_ao_ab_parent_names),
};

static const char * const pwm_g12a_ao_cd_parent_names[] = {
	"xtal", "aoclk81",
};

static const struct meson_pwm_data pwm_g12a_ao_cd_data = {
	.parent_names = pwm_g12a_ao_cd_parent_names,
	.num_parents = ARRAY_SIZE(pwm_g12a_ao_cd_parent_names),
};

static const char * const pwm_g12a_ee_parent_names[] = {
	"xtal", "hdmi_pll", "fclk_div4", "fclk_div3"
};

static const struct meson_pwm_data pwm_g12a_ee_data = {
	.parent_names = pwm_g12a_ee_parent_names,
	.num_parents = ARRAY_SIZE(pwm_g12a_ee_parent_names),
};

>>>>>>> upstream/android-13
static const struct of_device_id meson_pwm_matches[] = {
	{
		.compatible = "amlogic,meson8b-pwm",
		.data = &pwm_meson8b_data
	},
	{
		.compatible = "amlogic,meson-gxbb-pwm",
		.data = &pwm_gxbb_data
	},
	{
		.compatible = "amlogic,meson-gxbb-ao-pwm",
		.data = &pwm_gxbb_ao_data
	},
	{
		.compatible = "amlogic,meson-axg-ee-pwm",
		.data = &pwm_axg_ee_data
	},
	{
		.compatible = "amlogic,meson-axg-ao-pwm",
		.data = &pwm_axg_ao_data
	},
<<<<<<< HEAD
=======
	{
		.compatible = "amlogic,meson-g12a-ee-pwm",
		.data = &pwm_g12a_ee_data
	},
	{
		.compatible = "amlogic,meson-g12a-ao-pwm-ab",
		.data = &pwm_g12a_ao_ab_data
	},
	{
		.compatible = "amlogic,meson-g12a-ao-pwm-cd",
		.data = &pwm_g12a_ao_cd_data
	},
>>>>>>> upstream/android-13
	{},
};
MODULE_DEVICE_TABLE(of, meson_pwm_matches);

<<<<<<< HEAD
static int meson_pwm_init_channels(struct meson_pwm *meson,
				   struct meson_pwm_channel *channels)
=======
static int meson_pwm_init_channels(struct meson_pwm *meson)
>>>>>>> upstream/android-13
{
	struct device *dev = meson->chip.dev;
	struct clk_init_data init;
	unsigned int i;
	char name[255];
	int err;

	for (i = 0; i < meson->chip.npwm; i++) {
<<<<<<< HEAD
		struct meson_pwm_channel *channel = &channels[i];
=======
		struct meson_pwm_channel *channel = &meson->channels[i];
>>>>>>> upstream/android-13

		snprintf(name, sizeof(name), "%s#mux%u", dev_name(dev), i);

		init.name = name;
		init.ops = &clk_mux_ops;
<<<<<<< HEAD
		init.flags = CLK_IS_BASIC;
=======
		init.flags = 0;
>>>>>>> upstream/android-13
		init.parent_names = meson->data->parent_names;
		init.num_parents = meson->data->num_parents;

		channel->mux.reg = meson->base + REG_MISC_AB;
<<<<<<< HEAD
		channel->mux.shift = mux_reg_shifts[i];
		channel->mux.mask = BIT(MISC_CLK_SEL_WIDTH) - 1;
=======
		channel->mux.shift =
				meson_pwm_per_channel_data[i].clk_sel_shift;
		channel->mux.mask = MISC_CLK_SEL_MASK;
>>>>>>> upstream/android-13
		channel->mux.flags = 0;
		channel->mux.lock = &meson->lock;
		channel->mux.table = NULL;
		channel->mux.hw.init = &init;

		channel->clk = devm_clk_register(dev, &channel->mux.hw);
		if (IS_ERR(channel->clk)) {
			err = PTR_ERR(channel->clk);
			dev_err(dev, "failed to register %s: %d\n", name, err);
			return err;
		}

		snprintf(name, sizeof(name), "clkin%u", i);

<<<<<<< HEAD
		channel->clk_parent = devm_clk_get(dev, name);
		if (IS_ERR(channel->clk_parent)) {
			err = PTR_ERR(channel->clk_parent);
			if (err == -EPROBE_DEFER)
				return err;

			channel->clk_parent = NULL;
		}
=======
		channel->clk_parent = devm_clk_get_optional(dev, name);
		if (IS_ERR(channel->clk_parent))
			return PTR_ERR(channel->clk_parent);
>>>>>>> upstream/android-13
	}

	return 0;
}

<<<<<<< HEAD
static void meson_pwm_add_channels(struct meson_pwm *meson,
				   struct meson_pwm_channel *channels)
{
	unsigned int i;

	for (i = 0; i < meson->chip.npwm; i++)
		pwm_set_chip_data(&meson->chip.pwms[i], &channels[i]);
}

static int meson_pwm_probe(struct platform_device *pdev)
{
	struct meson_pwm_channel *channels;
	struct meson_pwm *meson;
	struct resource *regs;
=======
static int meson_pwm_probe(struct platform_device *pdev)
{
	struct meson_pwm *meson;
>>>>>>> upstream/android-13
	int err;

	meson = devm_kzalloc(&pdev->dev, sizeof(*meson), GFP_KERNEL);
	if (!meson)
		return -ENOMEM;

<<<<<<< HEAD
	regs = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	meson->base = devm_ioremap_resource(&pdev->dev, regs);
=======
	meson->base = devm_platform_ioremap_resource(pdev, 0);
>>>>>>> upstream/android-13
	if (IS_ERR(meson->base))
		return PTR_ERR(meson->base);

	spin_lock_init(&meson->lock);
	meson->chip.dev = &pdev->dev;
	meson->chip.ops = &meson_pwm_ops;
<<<<<<< HEAD
	meson->chip.base = -1;
	meson->chip.npwm = 2;
	meson->chip.of_xlate = of_pwm_xlate_with_flags;
	meson->chip.of_pwm_n_cells = 3;

	meson->data = of_device_get_match_data(&pdev->dev);
	meson->inverter_mask = BIT(meson->chip.npwm) - 1;

	channels = devm_kcalloc(&pdev->dev, meson->chip.npwm,
				sizeof(*channels), GFP_KERNEL);
	if (!channels)
		return -ENOMEM;

	err = meson_pwm_init_channels(meson, channels);
	if (err < 0)
		return err;

	err = pwmchip_add(&meson->chip);
=======
	meson->chip.npwm = MESON_NUM_PWMS;

	meson->data = of_device_get_match_data(&pdev->dev);

	err = meson_pwm_init_channels(meson);
	if (err < 0)
		return err;

	err = devm_pwmchip_add(&pdev->dev, &meson->chip);
>>>>>>> upstream/android-13
	if (err < 0) {
		dev_err(&pdev->dev, "failed to register PWM chip: %d\n", err);
		return err;
	}

<<<<<<< HEAD
	meson_pwm_add_channels(meson, channels);

	platform_set_drvdata(pdev, meson);

	return 0;
}

static int meson_pwm_remove(struct platform_device *pdev)
{
	struct meson_pwm *meson = platform_get_drvdata(pdev);

	return pwmchip_remove(&meson->chip);
}

=======
	return 0;
}

>>>>>>> upstream/android-13
static struct platform_driver meson_pwm_driver = {
	.driver = {
		.name = "meson-pwm",
		.of_match_table = meson_pwm_matches,
	},
	.probe = meson_pwm_probe,
<<<<<<< HEAD
	.remove = meson_pwm_remove,
=======
>>>>>>> upstream/android-13
};
module_platform_driver(meson_pwm_driver);

MODULE_DESCRIPTION("Amlogic Meson PWM Generator driver");
MODULE_AUTHOR("Neil Armstrong <narmstrong@baylibre.com>");
MODULE_LICENSE("Dual BSD/GPL");
