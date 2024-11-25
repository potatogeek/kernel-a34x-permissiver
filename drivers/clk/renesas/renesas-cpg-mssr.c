<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0
>>>>>>> upstream/android-13
/*
 * Renesas Clock Pulse Generator / Module Standby and Software Reset
 *
 * Copyright (C) 2015 Glider bvba
 *
 * Based on clk-mstp.c, clk-rcar-gen2.c, and clk-rcar-gen3.c
 *
 * Copyright (C) 2013 Ideas On Board SPRL
 * Copyright (C) 2015 Renesas Electronics Corp.
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2 of the License.
=======
>>>>>>> upstream/android-13
 */

#include <linux/clk.h>
#include <linux/clk-provider.h>
#include <linux/clk/renesas.h>
#include <linux/delay.h>
#include <linux/device.h>
#include <linux/init.h>
<<<<<<< HEAD
=======
#include <linux/io.h>
>>>>>>> upstream/android-13
#include <linux/mod_devicetable.h>
#include <linux/module.h>
#include <linux/of_address.h>
#include <linux/of_device.h>
#include <linux/platform_device.h>
#include <linux/pm_clock.h>
#include <linux/pm_domain.h>
#include <linux/psci.h>
#include <linux/reset-controller.h>
#include <linux/slab.h>

#include <dt-bindings/clock/renesas-cpg-mssr.h>

#include "renesas-cpg-mssr.h"
#include "clk-div6.h"

#ifdef DEBUG
#define WARN_DEBUG(x)	WARN_ON(x)
#else
#define WARN_DEBUG(x)	do { } while (0)
#endif


/*
 * Module Standby and Software Reset register offets.
 *
 * If the registers exist, these are valid for SH-Mobile, R-Mobile,
 * R-Car Gen2, R-Car Gen3, and RZ/G1.
 * These are NOT valid for R-Car Gen1 and RZ/A1!
 */

/*
 * Module Stop Status Register offsets
 */

static const u16 mstpsr[] = {
	0x030, 0x038, 0x040, 0x048, 0x04C, 0x03C, 0x1C0, 0x1C4,
	0x9A0, 0x9A4, 0x9A8, 0x9AC,
};

<<<<<<< HEAD
#define	MSTPSR(i)	mstpsr[i]

=======
static const u16 mstpsr_for_v3u[] = {
	0x2E00, 0x2E04, 0x2E08, 0x2E0C, 0x2E10, 0x2E14, 0x2E18, 0x2E1C,
	0x2E20, 0x2E24, 0x2E28, 0x2E2C, 0x2E30, 0x2E34, 0x2E38,
};
>>>>>>> upstream/android-13

/*
 * System Module Stop Control Register offsets
 */

static const u16 smstpcr[] = {
	0x130, 0x134, 0x138, 0x13C, 0x140, 0x144, 0x148, 0x14C,
	0x990, 0x994, 0x998, 0x99C,
};

<<<<<<< HEAD
#define	SMSTPCR(i)	smstpcr[i]

=======
static const u16 mstpcr_for_v3u[] = {
	0x2D00, 0x2D04, 0x2D08, 0x2D0C, 0x2D10, 0x2D14, 0x2D18, 0x2D1C,
	0x2D20, 0x2D24, 0x2D28, 0x2D2C, 0x2D30, 0x2D34, 0x2D38,
};

/*
 * Standby Control Register offsets (RZ/A)
 * Base address is FRQCR register
 */

static const u16 stbcr[] = {
	0xFFFF/*dummy*/, 0x010, 0x014, 0x410, 0x414, 0x418, 0x41C, 0x420,
	0x424, 0x428, 0x42C,
};
>>>>>>> upstream/android-13

/*
 * Software Reset Register offsets
 */

static const u16 srcr[] = {
	0x0A0, 0x0A8, 0x0B0, 0x0B8, 0x0BC, 0x0C4, 0x1C8, 0x1CC,
	0x920, 0x924, 0x928, 0x92C,
};

<<<<<<< HEAD
#define	SRCR(i)		srcr[i]


/* Realtime Module Stop Control Register offsets */
#define RMSTPCR(i)	(smstpcr[i] - 0x20)

/* Modem Module Stop Control Register offsets (r8a73a4) */
#define MMSTPCR(i)	(smstpcr[i] + 0x20)

/* Software Reset Clearing Register offsets */
#define	SRSTCLR(i)	(0x940 + (i) * 4)


/**
 * Clock Pulse Generator / Module Standby and Software Reset Private Data
=======
static const u16 srcr_for_v3u[] = {
	0x2C00, 0x2C04, 0x2C08, 0x2C0C, 0x2C10, 0x2C14, 0x2C18, 0x2C1C,
	0x2C20, 0x2C24, 0x2C28, 0x2C2C, 0x2C30, 0x2C34, 0x2C38,
};

/*
 * Software Reset Clearing Register offsets
 */

static const u16 srstclr[] = {
	0x940, 0x944, 0x948, 0x94C, 0x950, 0x954, 0x958, 0x95C,
	0x960, 0x964, 0x968, 0x96C,
};

static const u16 srstclr_for_v3u[] = {
	0x2C80, 0x2C84, 0x2C88, 0x2C8C, 0x2C90, 0x2C94, 0x2C98, 0x2C9C,
	0x2CA0, 0x2CA4, 0x2CA8, 0x2CAC, 0x2CB0, 0x2CB4, 0x2CB8,
};

/**
 * struct cpg_mssr_priv - Clock Pulse Generator / Module Standby
 *                        and Software Reset Private Data
>>>>>>> upstream/android-13
 *
 * @rcdev: Optional reset controller entity
 * @dev: CPG/MSSR device
 * @base: CPG/MSSR register block base address
<<<<<<< HEAD
 * @rmw_lock: protects RMW register accesses
 * @clks: Array containing all Core and Module Clocks
=======
 * @reg_layout: CPG/MSSR register layout
 * @rmw_lock: protects RMW register accesses
 * @np: Device node in DT for this CPG/MSSR module
>>>>>>> upstream/android-13
 * @num_core_clks: Number of Core Clocks in clks[]
 * @num_mod_clks: Number of Module Clocks in clks[]
 * @last_dt_core_clk: ID of the last Core Clock exported to DT
 * @notifiers: Notifier chain to save/restore clock state for system resume
<<<<<<< HEAD
 * @smstpcr_saved[].mask: Mask of SMSTPCR[] bits under our control
 * @smstpcr_saved[].val: Saved values of SMSTPCR[]
=======
 * @status_regs: Pointer to status registers array
 * @control_regs: Pointer to control registers array
 * @reset_regs: Pointer to reset registers array
 * @reset_clear_regs:  Pointer to reset clearing registers array
 * @smstpcr_saved: [].mask: Mask of SMSTPCR[] bits under our control
 *                 [].val: Saved values of SMSTPCR[]
 * @clks: Array containing all Core and Module Clocks
>>>>>>> upstream/android-13
 */
struct cpg_mssr_priv {
#ifdef CONFIG_RESET_CONTROLLER
	struct reset_controller_dev rcdev;
#endif
	struct device *dev;
	void __iomem *base;
<<<<<<< HEAD
	spinlock_t rmw_lock;

	struct clk **clks;
=======
	enum clk_reg_layout reg_layout;
	spinlock_t rmw_lock;
	struct device_node *np;

>>>>>>> upstream/android-13
	unsigned int num_core_clks;
	unsigned int num_mod_clks;
	unsigned int last_dt_core_clk;

	struct raw_notifier_head notifiers;
<<<<<<< HEAD
	struct {
		u32 mask;
		u32 val;
	} smstpcr_saved[ARRAY_SIZE(smstpcr)];
};

=======
	const u16 *status_regs;
	const u16 *control_regs;
	const u16 *reset_regs;
	const u16 *reset_clear_regs;
	struct {
		u32 mask;
		u32 val;
	} smstpcr_saved[ARRAY_SIZE(mstpsr_for_v3u)];

	struct clk *clks[];
};

static struct cpg_mssr_priv *cpg_mssr_priv;
>>>>>>> upstream/android-13

/**
 * struct mstp_clock - MSTP gating clock
 * @hw: handle between common and hardware-specific interfaces
 * @index: MSTP clock number
 * @priv: CPG/MSSR private data
 */
struct mstp_clock {
	struct clk_hw hw;
	u32 index;
	struct cpg_mssr_priv *priv;
};

#define to_mstp_clock(_hw) container_of(_hw, struct mstp_clock, hw)

static int cpg_mstp_clock_endisable(struct clk_hw *hw, bool enable)
{
	struct mstp_clock *clock = to_mstp_clock(hw);
	struct cpg_mssr_priv *priv = clock->priv;
	unsigned int reg = clock->index / 32;
	unsigned int bit = clock->index % 32;
	struct device *dev = priv->dev;
	u32 bitmask = BIT(bit);
	unsigned long flags;
	unsigned int i;
	u32 value;

	dev_dbg(dev, "MSTP %u%02u/%pC %s\n", reg, bit, hw->clk,
		enable ? "ON" : "OFF");
	spin_lock_irqsave(&priv->rmw_lock, flags);

<<<<<<< HEAD
	value = readl(priv->base + SMSTPCR(reg));
	if (enable)
		value &= ~bitmask;
	else
		value |= bitmask;
	writel(value, priv->base + SMSTPCR(reg));

	spin_unlock_irqrestore(&priv->rmw_lock, flags);

	if (!enable)
		return 0;

	for (i = 1000; i > 0; --i) {
		if (!(readl(priv->base + MSTPSR(reg)) & bitmask))
=======
	if (priv->reg_layout == CLK_REG_LAYOUT_RZ_A) {
		value = readb(priv->base + priv->control_regs[reg]);
		if (enable)
			value &= ~bitmask;
		else
			value |= bitmask;
		writeb(value, priv->base + priv->control_regs[reg]);

		/* dummy read to ensure write has completed */
		readb(priv->base + priv->control_regs[reg]);
		barrier_data(priv->base + priv->control_regs[reg]);
	} else {
		value = readl(priv->base + priv->control_regs[reg]);
		if (enable)
			value &= ~bitmask;
		else
			value |= bitmask;
		writel(value, priv->base + priv->control_regs[reg]);
	}

	spin_unlock_irqrestore(&priv->rmw_lock, flags);

	if (!enable || priv->reg_layout == CLK_REG_LAYOUT_RZ_A)
		return 0;

	for (i = 1000; i > 0; --i) {
		if (!(readl(priv->base + priv->status_regs[reg]) & bitmask))
>>>>>>> upstream/android-13
			break;
		cpu_relax();
	}

	if (!i) {
		dev_err(dev, "Failed to enable SMSTP %p[%d]\n",
<<<<<<< HEAD
			priv->base + SMSTPCR(reg), bit);
=======
			priv->base + priv->control_regs[reg], bit);
>>>>>>> upstream/android-13
		return -ETIMEDOUT;
	}

	return 0;
}

static int cpg_mstp_clock_enable(struct clk_hw *hw)
{
	return cpg_mstp_clock_endisable(hw, true);
}

static void cpg_mstp_clock_disable(struct clk_hw *hw)
{
	cpg_mstp_clock_endisable(hw, false);
}

static int cpg_mstp_clock_is_enabled(struct clk_hw *hw)
{
	struct mstp_clock *clock = to_mstp_clock(hw);
	struct cpg_mssr_priv *priv = clock->priv;
	u32 value;

<<<<<<< HEAD
	value = readl(priv->base + MSTPSR(clock->index / 32));
=======
	if (priv->reg_layout == CLK_REG_LAYOUT_RZ_A)
		value = readb(priv->base + priv->control_regs[clock->index / 32]);
	else
		value = readl(priv->base + priv->status_regs[clock->index / 32]);
>>>>>>> upstream/android-13

	return !(value & BIT(clock->index % 32));
}

static const struct clk_ops cpg_mstp_clock_ops = {
	.enable = cpg_mstp_clock_enable,
	.disable = cpg_mstp_clock_disable,
	.is_enabled = cpg_mstp_clock_is_enabled,
};

static
struct clk *cpg_mssr_clk_src_twocell_get(struct of_phandle_args *clkspec,
					 void *data)
{
	unsigned int clkidx = clkspec->args[1];
	struct cpg_mssr_priv *priv = data;
	struct device *dev = priv->dev;
	unsigned int idx;
	const char *type;
	struct clk *clk;
<<<<<<< HEAD
=======
	int range_check;
>>>>>>> upstream/android-13

	switch (clkspec->args[0]) {
	case CPG_CORE:
		type = "core";
		if (clkidx > priv->last_dt_core_clk) {
			dev_err(dev, "Invalid %s clock index %u\n", type,
			       clkidx);
			return ERR_PTR(-EINVAL);
		}
		clk = priv->clks[clkidx];
		break;

	case CPG_MOD:
		type = "module";
<<<<<<< HEAD
		idx = MOD_CLK_PACK(clkidx);
		if (clkidx % 100 > 31 || idx >= priv->num_mod_clks) {
=======
		if (priv->reg_layout == CLK_REG_LAYOUT_RZ_A) {
			idx = MOD_CLK_PACK_10(clkidx);
			range_check = 7 - (clkidx % 10);
		} else {
			idx = MOD_CLK_PACK(clkidx);
			range_check = 31 - (clkidx % 100);
		}
		if (range_check < 0 || idx >= priv->num_mod_clks) {
>>>>>>> upstream/android-13
			dev_err(dev, "Invalid %s clock index %u\n", type,
				clkidx);
			return ERR_PTR(-EINVAL);
		}
		clk = priv->clks[priv->num_core_clks + idx];
		break;

	default:
		dev_err(dev, "Invalid CPG clock type %u\n", clkspec->args[0]);
		return ERR_PTR(-EINVAL);
	}

	if (IS_ERR(clk))
		dev_err(dev, "Cannot get %s clock %u: %ld", type, clkidx,
		       PTR_ERR(clk));
	else
		dev_dbg(dev, "clock (%u, %u) is %pC at %lu Hz\n",
			clkspec->args[0], clkspec->args[1], clk,
			clk_get_rate(clk));
	return clk;
}

static void __init cpg_mssr_register_core_clk(const struct cpg_core_clk *core,
					      const struct cpg_mssr_info *info,
					      struct cpg_mssr_priv *priv)
{
	struct clk *clk = ERR_PTR(-ENOTSUPP), *parent;
	struct device *dev = priv->dev;
	unsigned int id = core->id, div = core->div;
	const char *parent_name;

	WARN_DEBUG(id >= priv->num_core_clks);
	WARN_DEBUG(PTR_ERR(priv->clks[id]) != -ENOENT);

	if (!core->name) {
		/* Skip NULLified clock */
		return;
	}

	switch (core->type) {
	case CLK_TYPE_IN:
<<<<<<< HEAD
		clk = of_clk_get_by_name(priv->dev->of_node, core->name);
=======
		clk = of_clk_get_by_name(priv->np, core->name);
>>>>>>> upstream/android-13
		break;

	case CLK_TYPE_FF:
	case CLK_TYPE_DIV6P1:
	case CLK_TYPE_DIV6_RO:
		WARN_DEBUG(core->parent >= priv->num_core_clks);
		parent = priv->clks[core->parent];
		if (IS_ERR(parent)) {
			clk = parent;
			goto fail;
		}

		parent_name = __clk_get_name(parent);

		if (core->type == CLK_TYPE_DIV6_RO)
			/* Multiply with the DIV6 register value */
			div *= (readl(priv->base + core->offset) & 0x3f) + 1;

		if (core->type == CLK_TYPE_DIV6P1) {
			clk = cpg_div6_register(core->name, 1, &parent_name,
						priv->base + core->offset,
						&priv->notifiers);
		} else {
			clk = clk_register_fixed_factor(NULL, core->name,
							parent_name, 0,
							core->mult, div);
		}
		break;

<<<<<<< HEAD
=======
	case CLK_TYPE_FR:
		clk = clk_register_fixed_rate(NULL, core->name, NULL, 0,
					      core->mult);
		break;

>>>>>>> upstream/android-13
	default:
		if (info->cpg_clk_register)
			clk = info->cpg_clk_register(dev, core, info,
						     priv->clks, priv->base,
						     &priv->notifiers);
		else
			dev_err(dev, "%s has unsupported core clock type %u\n",
				core->name, core->type);
		break;
	}

	if (IS_ERR_OR_NULL(clk))
		goto fail;

	dev_dbg(dev, "Core clock %pC at %lu Hz\n", clk, clk_get_rate(clk));
	priv->clks[id] = clk;
	return;

fail:
	dev_err(dev, "Failed to register %s clock %s: %ld\n", "core",
		core->name, PTR_ERR(clk));
}

static void __init cpg_mssr_register_mod_clk(const struct mssr_mod_clk *mod,
					     const struct cpg_mssr_info *info,
					     struct cpg_mssr_priv *priv)
{
	struct mstp_clock *clock = NULL;
	struct device *dev = priv->dev;
	unsigned int id = mod->id;
	struct clk_init_data init = {};
	struct clk *parent, *clk;
	const char *parent_name;
	unsigned int i;

	WARN_DEBUG(id < priv->num_core_clks);
	WARN_DEBUG(id >= priv->num_core_clks + priv->num_mod_clks);
	WARN_DEBUG(mod->parent >= priv->num_core_clks + priv->num_mod_clks);
	WARN_DEBUG(PTR_ERR(priv->clks[id]) != -ENOENT);

	if (!mod->name) {
		/* Skip NULLified clock */
		return;
	}

	parent = priv->clks[mod->parent];
	if (IS_ERR(parent)) {
		clk = parent;
		goto fail;
	}

	clock = kzalloc(sizeof(*clock), GFP_KERNEL);
	if (!clock) {
		clk = ERR_PTR(-ENOMEM);
		goto fail;
	}

	init.name = mod->name;
	init.ops = &cpg_mstp_clock_ops;
<<<<<<< HEAD
	init.flags = CLK_IS_BASIC | CLK_SET_RATE_PARENT;
	for (i = 0; i < info->num_crit_mod_clks; i++)
		if (id == info->crit_mod_clks[i]) {
			dev_dbg(dev, "MSTP %s setting CLK_IS_CRITICAL\n",
				mod->name);
			init.flags |= CLK_IS_CRITICAL;
			break;
		}

=======
	init.flags = CLK_SET_RATE_PARENT;
>>>>>>> upstream/android-13
	parent_name = __clk_get_name(parent);
	init.parent_names = &parent_name;
	init.num_parents = 1;

	clock->index = id - priv->num_core_clks;
	clock->priv = priv;
	clock->hw.init = &init;

<<<<<<< HEAD
=======
	for (i = 0; i < info->num_crit_mod_clks; i++)
		if (id == info->crit_mod_clks[i] &&
		    cpg_mstp_clock_is_enabled(&clock->hw)) {
			dev_dbg(dev, "MSTP %s setting CLK_IS_CRITICAL\n",
				mod->name);
			init.flags |= CLK_IS_CRITICAL;
			break;
		}

>>>>>>> upstream/android-13
	clk = clk_register(NULL, &clock->hw);
	if (IS_ERR(clk))
		goto fail;

	dev_dbg(dev, "Module clock %pC at %lu Hz\n", clk, clk_get_rate(clk));
	priv->clks[id] = clk;
	priv->smstpcr_saved[clock->index / 32].mask |= BIT(clock->index % 32);
	return;

fail:
	dev_err(dev, "Failed to register %s clock %s: %ld\n", "module",
		mod->name, PTR_ERR(clk));
	kfree(clock);
}

struct cpg_mssr_clk_domain {
	struct generic_pm_domain genpd;
<<<<<<< HEAD
	struct device_node *np;
	unsigned int num_core_pm_clks;
	unsigned int core_pm_clks[0];
=======
	unsigned int num_core_pm_clks;
	unsigned int core_pm_clks[];
>>>>>>> upstream/android-13
};

static struct cpg_mssr_clk_domain *cpg_mssr_clk_domain;

static bool cpg_mssr_is_pm_clk(const struct of_phandle_args *clkspec,
			       struct cpg_mssr_clk_domain *pd)
{
	unsigned int i;

<<<<<<< HEAD
	if (clkspec->np != pd->np || clkspec->args_count != 2)
=======
	if (clkspec->np != pd->genpd.dev.of_node || clkspec->args_count != 2)
>>>>>>> upstream/android-13
		return false;

	switch (clkspec->args[0]) {
	case CPG_CORE:
		for (i = 0; i < pd->num_core_pm_clks; i++)
			if (clkspec->args[1] == pd->core_pm_clks[i])
				return true;
		return false;

	case CPG_MOD:
		return true;

	default:
		return false;
	}
}

int cpg_mssr_attach_dev(struct generic_pm_domain *unused, struct device *dev)
{
	struct cpg_mssr_clk_domain *pd = cpg_mssr_clk_domain;
	struct device_node *np = dev->of_node;
	struct of_phandle_args clkspec;
	struct clk *clk;
	int i = 0;
	int error;

	if (!pd) {
		dev_dbg(dev, "CPG/MSSR clock domain not yet available\n");
		return -EPROBE_DEFER;
	}

	while (!of_parse_phandle_with_args(np, "clocks", "#clock-cells", i,
					   &clkspec)) {
		if (cpg_mssr_is_pm_clk(&clkspec, pd))
			goto found;

		of_node_put(clkspec.np);
		i++;
	}

	return 0;

found:
	clk = of_clk_get_from_provider(&clkspec);
	of_node_put(clkspec.np);

	if (IS_ERR(clk))
		return PTR_ERR(clk);

	error = pm_clk_create(dev);
<<<<<<< HEAD
	if (error) {
		dev_err(dev, "pm_clk_create failed %d\n", error);
		goto fail_put;
	}

	error = pm_clk_add_clk(dev, clk);
	if (error) {
		dev_err(dev, "pm_clk_add_clk %pC failed %d\n", clk, error);
		goto fail_destroy;
	}
=======
	if (error)
		goto fail_put;

	error = pm_clk_add_clk(dev, clk);
	if (error)
		goto fail_destroy;
>>>>>>> upstream/android-13

	return 0;

fail_destroy:
	pm_clk_destroy(dev);
fail_put:
	clk_put(clk);
	return error;
}

void cpg_mssr_detach_dev(struct generic_pm_domain *unused, struct device *dev)
{
	if (!pm_clk_no_clocks(dev))
		pm_clk_destroy(dev);
}

static int __init cpg_mssr_add_clk_domain(struct device *dev,
					  const unsigned int *core_pm_clks,
					  unsigned int num_core_pm_clks)
{
	struct device_node *np = dev->of_node;
	struct generic_pm_domain *genpd;
	struct cpg_mssr_clk_domain *pd;
	size_t pm_size = num_core_pm_clks * sizeof(core_pm_clks[0]);

	pd = devm_kzalloc(dev, sizeof(*pd) + pm_size, GFP_KERNEL);
	if (!pd)
		return -ENOMEM;

<<<<<<< HEAD
	pd->np = np;
=======
>>>>>>> upstream/android-13
	pd->num_core_pm_clks = num_core_pm_clks;
	memcpy(pd->core_pm_clks, core_pm_clks, pm_size);

	genpd = &pd->genpd;
	genpd->name = np->name;
	genpd->flags = GENPD_FLAG_PM_CLK | GENPD_FLAG_ALWAYS_ON |
		       GENPD_FLAG_ACTIVE_WAKEUP;
	genpd->attach_dev = cpg_mssr_attach_dev;
	genpd->detach_dev = cpg_mssr_detach_dev;
	pm_genpd_init(genpd, &pm_domain_always_on_gov, false);
	cpg_mssr_clk_domain = pd;

	of_genpd_add_provider_simple(np, genpd);
	return 0;
}

#ifdef CONFIG_RESET_CONTROLLER

#define rcdev_to_priv(x)	container_of(x, struct cpg_mssr_priv, rcdev)

static int cpg_mssr_reset(struct reset_controller_dev *rcdev,
			  unsigned long id)
{
	struct cpg_mssr_priv *priv = rcdev_to_priv(rcdev);
	unsigned int reg = id / 32;
	unsigned int bit = id % 32;
	u32 bitmask = BIT(bit);

	dev_dbg(priv->dev, "reset %u%02u\n", reg, bit);

	/* Reset module */
<<<<<<< HEAD
	writel(bitmask, priv->base + SRCR(reg));
=======
	writel(bitmask, priv->base + priv->reset_regs[reg]);
>>>>>>> upstream/android-13

	/* Wait for at least one cycle of the RCLK clock (@ ca. 32 kHz) */
	udelay(35);

	/* Release module from reset state */
<<<<<<< HEAD
	writel(bitmask, priv->base + SRSTCLR(reg));
=======
	writel(bitmask, priv->base + priv->reset_clear_regs[reg]);
>>>>>>> upstream/android-13

	return 0;
}

static int cpg_mssr_assert(struct reset_controller_dev *rcdev, unsigned long id)
{
	struct cpg_mssr_priv *priv = rcdev_to_priv(rcdev);
	unsigned int reg = id / 32;
	unsigned int bit = id % 32;
	u32 bitmask = BIT(bit);

	dev_dbg(priv->dev, "assert %u%02u\n", reg, bit);

<<<<<<< HEAD
	writel(bitmask, priv->base + SRCR(reg));
=======
	writel(bitmask, priv->base + priv->reset_regs[reg]);
>>>>>>> upstream/android-13
	return 0;
}

static int cpg_mssr_deassert(struct reset_controller_dev *rcdev,
			     unsigned long id)
{
	struct cpg_mssr_priv *priv = rcdev_to_priv(rcdev);
	unsigned int reg = id / 32;
	unsigned int bit = id % 32;
	u32 bitmask = BIT(bit);

	dev_dbg(priv->dev, "deassert %u%02u\n", reg, bit);

<<<<<<< HEAD
	writel(bitmask, priv->base + SRSTCLR(reg));
=======
	writel(bitmask, priv->base + priv->reset_clear_regs[reg]);
>>>>>>> upstream/android-13
	return 0;
}

static int cpg_mssr_status(struct reset_controller_dev *rcdev,
			   unsigned long id)
{
	struct cpg_mssr_priv *priv = rcdev_to_priv(rcdev);
	unsigned int reg = id / 32;
	unsigned int bit = id % 32;
	u32 bitmask = BIT(bit);

<<<<<<< HEAD
	return !!(readl(priv->base + SRCR(reg)) & bitmask);
=======
	return !!(readl(priv->base + priv->reset_regs[reg]) & bitmask);
>>>>>>> upstream/android-13
}

static const struct reset_control_ops cpg_mssr_reset_ops = {
	.reset = cpg_mssr_reset,
	.assert = cpg_mssr_assert,
	.deassert = cpg_mssr_deassert,
	.status = cpg_mssr_status,
};

static int cpg_mssr_reset_xlate(struct reset_controller_dev *rcdev,
				const struct of_phandle_args *reset_spec)
{
	struct cpg_mssr_priv *priv = rcdev_to_priv(rcdev);
	unsigned int unpacked = reset_spec->args[0];
	unsigned int idx = MOD_CLK_PACK(unpacked);

	if (unpacked % 100 > 31 || idx >= rcdev->nr_resets) {
		dev_err(priv->dev, "Invalid reset index %u\n", unpacked);
		return -EINVAL;
	}

	return idx;
}

static int cpg_mssr_reset_controller_register(struct cpg_mssr_priv *priv)
{
	priv->rcdev.ops = &cpg_mssr_reset_ops;
	priv->rcdev.of_node = priv->dev->of_node;
	priv->rcdev.of_reset_n_cells = 1;
	priv->rcdev.of_xlate = cpg_mssr_reset_xlate;
	priv->rcdev.nr_resets = priv->num_mod_clks;
	return devm_reset_controller_register(priv->dev, &priv->rcdev);
}

#else /* !CONFIG_RESET_CONTROLLER */
static inline int cpg_mssr_reset_controller_register(struct cpg_mssr_priv *priv)
{
	return 0;
}
#endif /* !CONFIG_RESET_CONTROLLER */


static const struct of_device_id cpg_mssr_match[] = {
<<<<<<< HEAD
=======
#ifdef CONFIG_CLK_R7S9210
	{
		.compatible = "renesas,r7s9210-cpg-mssr",
		.data = &r7s9210_cpg_mssr_info,
	},
#endif
#ifdef CONFIG_CLK_R8A7742
	{
		.compatible = "renesas,r8a7742-cpg-mssr",
		.data = &r8a7742_cpg_mssr_info,
	},
#endif
>>>>>>> upstream/android-13
#ifdef CONFIG_CLK_R8A7743
	{
		.compatible = "renesas,r8a7743-cpg-mssr",
		.data = &r8a7743_cpg_mssr_info,
	},
<<<<<<< HEAD
=======
	/* RZ/G1N is (almost) identical to RZ/G1M w.r.t. clocks. */
	{
		.compatible = "renesas,r8a7744-cpg-mssr",
		.data = &r8a7743_cpg_mssr_info,
	},
>>>>>>> upstream/android-13
#endif
#ifdef CONFIG_CLK_R8A7745
	{
		.compatible = "renesas,r8a7745-cpg-mssr",
		.data = &r8a7745_cpg_mssr_info,
	},
#endif
#ifdef CONFIG_CLK_R8A77470
	{
		.compatible = "renesas,r8a77470-cpg-mssr",
		.data = &r8a77470_cpg_mssr_info,
	},
#endif
<<<<<<< HEAD
=======
#ifdef CONFIG_CLK_R8A774A1
	{
		.compatible = "renesas,r8a774a1-cpg-mssr",
		.data = &r8a774a1_cpg_mssr_info,
	},
#endif
#ifdef CONFIG_CLK_R8A774B1
	{
		.compatible = "renesas,r8a774b1-cpg-mssr",
		.data = &r8a774b1_cpg_mssr_info,
	},
#endif
#ifdef CONFIG_CLK_R8A774C0
	{
		.compatible = "renesas,r8a774c0-cpg-mssr",
		.data = &r8a774c0_cpg_mssr_info,
	},
#endif
#ifdef CONFIG_CLK_R8A774E1
	{
		.compatible = "renesas,r8a774e1-cpg-mssr",
		.data = &r8a774e1_cpg_mssr_info,
	},
#endif
>>>>>>> upstream/android-13
#ifdef CONFIG_CLK_R8A7790
	{
		.compatible = "renesas,r8a7790-cpg-mssr",
		.data = &r8a7790_cpg_mssr_info,
	},
#endif
#ifdef CONFIG_CLK_R8A7791
	{
		.compatible = "renesas,r8a7791-cpg-mssr",
		.data = &r8a7791_cpg_mssr_info,
	},
	/* R-Car M2-N is (almost) identical to R-Car M2-W w.r.t. clocks. */
	{
		.compatible = "renesas,r8a7793-cpg-mssr",
		.data = &r8a7791_cpg_mssr_info,
	},
#endif
#ifdef CONFIG_CLK_R8A7792
	{
		.compatible = "renesas,r8a7792-cpg-mssr",
		.data = &r8a7792_cpg_mssr_info,
	},
#endif
#ifdef CONFIG_CLK_R8A7794
	{
		.compatible = "renesas,r8a7794-cpg-mssr",
		.data = &r8a7794_cpg_mssr_info,
	},
#endif
#ifdef CONFIG_CLK_R8A7795
	{
		.compatible = "renesas,r8a7795-cpg-mssr",
		.data = &r8a7795_cpg_mssr_info,
	},
#endif
<<<<<<< HEAD
#ifdef CONFIG_CLK_R8A7796
=======
#ifdef CONFIG_CLK_R8A77960
>>>>>>> upstream/android-13
	{
		.compatible = "renesas,r8a7796-cpg-mssr",
		.data = &r8a7796_cpg_mssr_info,
	},
#endif
<<<<<<< HEAD
=======
#ifdef CONFIG_CLK_R8A77961
	{
		.compatible = "renesas,r8a77961-cpg-mssr",
		.data = &r8a7796_cpg_mssr_info,
	},
#endif
>>>>>>> upstream/android-13
#ifdef CONFIG_CLK_R8A77965
	{
		.compatible = "renesas,r8a77965-cpg-mssr",
		.data = &r8a77965_cpg_mssr_info,
	},
#endif
#ifdef CONFIG_CLK_R8A77970
	{
		.compatible = "renesas,r8a77970-cpg-mssr",
		.data = &r8a77970_cpg_mssr_info,
	},
#endif
#ifdef CONFIG_CLK_R8A77980
	{
		.compatible = "renesas,r8a77980-cpg-mssr",
		.data = &r8a77980_cpg_mssr_info,
	},
#endif
#ifdef CONFIG_CLK_R8A77990
	{
		.compatible = "renesas,r8a77990-cpg-mssr",
		.data = &r8a77990_cpg_mssr_info,
	},
#endif
#ifdef CONFIG_CLK_R8A77995
	{
		.compatible = "renesas,r8a77995-cpg-mssr",
		.data = &r8a77995_cpg_mssr_info,
	},
#endif
<<<<<<< HEAD
=======
#ifdef CONFIG_CLK_R8A779A0
	{
		.compatible = "renesas,r8a779a0-cpg-mssr",
		.data = &r8a779a0_cpg_mssr_info,
	},
#endif
>>>>>>> upstream/android-13
	{ /* sentinel */ }
};

static void cpg_mssr_del_clk_provider(void *data)
{
	of_clk_del_provider(data);
}

#if defined(CONFIG_PM_SLEEP) && defined(CONFIG_ARM_PSCI_FW)
static int cpg_mssr_suspend_noirq(struct device *dev)
{
	struct cpg_mssr_priv *priv = dev_get_drvdata(dev);
	unsigned int reg;

	/* This is the best we can do to check for the presence of PSCI */
	if (!psci_ops.cpu_suspend)
		return 0;

	/* Save module registers with bits under our control */
	for (reg = 0; reg < ARRAY_SIZE(priv->smstpcr_saved); reg++) {
		if (priv->smstpcr_saved[reg].mask)
			priv->smstpcr_saved[reg].val =
<<<<<<< HEAD
				readl(priv->base + SMSTPCR(reg));
=======
				priv->reg_layout == CLK_REG_LAYOUT_RZ_A ?
				readb(priv->base + priv->control_regs[reg]) :
				readl(priv->base + priv->control_regs[reg]);
>>>>>>> upstream/android-13
	}

	/* Save core clocks */
	raw_notifier_call_chain(&priv->notifiers, PM_EVENT_SUSPEND, NULL);

	return 0;
}

static int cpg_mssr_resume_noirq(struct device *dev)
{
	struct cpg_mssr_priv *priv = dev_get_drvdata(dev);
	unsigned int reg, i;
	u32 mask, oldval, newval;

	/* This is the best we can do to check for the presence of PSCI */
	if (!psci_ops.cpu_suspend)
		return 0;

	/* Restore core clocks */
	raw_notifier_call_chain(&priv->notifiers, PM_EVENT_RESUME, NULL);

	/* Restore module clocks */
	for (reg = 0; reg < ARRAY_SIZE(priv->smstpcr_saved); reg++) {
		mask = priv->smstpcr_saved[reg].mask;
		if (!mask)
			continue;

<<<<<<< HEAD
		oldval = readl(priv->base + SMSTPCR(reg));
=======
		if (priv->reg_layout == CLK_REG_LAYOUT_RZ_A)
			oldval = readb(priv->base + priv->control_regs[reg]);
		else
			oldval = readl(priv->base + priv->control_regs[reg]);
>>>>>>> upstream/android-13
		newval = oldval & ~mask;
		newval |= priv->smstpcr_saved[reg].val & mask;
		if (newval == oldval)
			continue;

<<<<<<< HEAD
		writel(newval, priv->base + SMSTPCR(reg));
=======
		if (priv->reg_layout == CLK_REG_LAYOUT_RZ_A) {
			writeb(newval, priv->base + priv->control_regs[reg]);
			/* dummy read to ensure write has completed */
			readb(priv->base + priv->control_regs[reg]);
			barrier_data(priv->base + priv->control_regs[reg]);
			continue;
		} else
			writel(newval, priv->base + priv->control_regs[reg]);
>>>>>>> upstream/android-13

		/* Wait until enabled clocks are really enabled */
		mask &= ~priv->smstpcr_saved[reg].val;
		if (!mask)
			continue;

		for (i = 1000; i > 0; --i) {
<<<<<<< HEAD
			oldval = readl(priv->base + MSTPSR(reg));
=======
			oldval = readl(priv->base + priv->status_regs[reg]);
>>>>>>> upstream/android-13
			if (!(oldval & mask))
				break;
			cpu_relax();
		}

		if (!i)
<<<<<<< HEAD
			dev_warn(dev, "Failed to enable SMSTP %p[0x%x]\n",
				 priv->base + SMSTPCR(reg), oldval & mask);
=======
			dev_warn(dev, "Failed to enable %s%u[0x%x]\n",
				 priv->reg_layout == CLK_REG_LAYOUT_RZ_A ?
				 "STB" : "SMSTP", reg, oldval & mask);
>>>>>>> upstream/android-13
	}

	return 0;
}

static const struct dev_pm_ops cpg_mssr_pm = {
	SET_NOIRQ_SYSTEM_SLEEP_PM_OPS(cpg_mssr_suspend_noirq,
				      cpg_mssr_resume_noirq)
};
#define DEV_PM_OPS	&cpg_mssr_pm
#else
#define DEV_PM_OPS	NULL
#endif /* CONFIG_PM_SLEEP && CONFIG_ARM_PSCI_FW */

<<<<<<< HEAD
static int __init cpg_mssr_probe(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	struct device_node *np = dev->of_node;
	const struct cpg_mssr_info *info;
	struct cpg_mssr_priv *priv;
	unsigned int nclks, i;
	struct resource *res;
	struct clk **clks;
	int error;

	info = of_device_get_match_data(dev);
=======
static int __init cpg_mssr_common_init(struct device *dev,
				       struct device_node *np,
				       const struct cpg_mssr_info *info)
{
	struct cpg_mssr_priv *priv;
	unsigned int nclks, i;
	int error;

>>>>>>> upstream/android-13
	if (info->init) {
		error = info->init(dev);
		if (error)
			return error;
	}

<<<<<<< HEAD
	priv = devm_kzalloc(dev, sizeof(*priv), GFP_KERNEL);
	if (!priv)
		return -ENOMEM;

	priv->dev = dev;
	spin_lock_init(&priv->rmw_lock);

	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	priv->base = devm_ioremap_resource(dev, res);
	if (IS_ERR(priv->base))
		return PTR_ERR(priv->base);

	nclks = info->num_total_core_clks + info->num_hw_mod_clks;
	clks = devm_kmalloc_array(dev, nclks, sizeof(*clks), GFP_KERNEL);
	if (!clks)
		return -ENOMEM;

	dev_set_drvdata(dev, priv);
	priv->clks = clks;
=======
	nclks = info->num_total_core_clks + info->num_hw_mod_clks;
	priv = kzalloc(struct_size(priv, clks, nclks), GFP_KERNEL);
	if (!priv)
		return -ENOMEM;

	priv->np = np;
	priv->dev = dev;
	spin_lock_init(&priv->rmw_lock);

	priv->base = of_iomap(np, 0);
	if (!priv->base) {
		error = -ENOMEM;
		goto out_err;
	}

	cpg_mssr_priv = priv;
>>>>>>> upstream/android-13
	priv->num_core_clks = info->num_total_core_clks;
	priv->num_mod_clks = info->num_hw_mod_clks;
	priv->last_dt_core_clk = info->last_dt_core_clk;
	RAW_INIT_NOTIFIER_HEAD(&priv->notifiers);
<<<<<<< HEAD

	for (i = 0; i < nclks; i++)
		clks[i] = ERR_PTR(-ENOENT);
=======
	priv->reg_layout = info->reg_layout;
	if (priv->reg_layout == CLK_REG_LAYOUT_RCAR_GEN2_AND_GEN3) {
		priv->status_regs = mstpsr;
		priv->control_regs = smstpcr;
		priv->reset_regs = srcr;
		priv->reset_clear_regs = srstclr;
	} else if (priv->reg_layout == CLK_REG_LAYOUT_RZ_A) {
		priv->control_regs = stbcr;
	} else if (priv->reg_layout == CLK_REG_LAYOUT_RCAR_V3U) {
		priv->status_regs = mstpsr_for_v3u;
		priv->control_regs = mstpcr_for_v3u;
		priv->reset_regs = srcr_for_v3u;
		priv->reset_clear_regs = srstclr_for_v3u;
	} else {
		error = -EINVAL;
		goto out_err;
	}

	for (i = 0; i < nclks; i++)
		priv->clks[i] = ERR_PTR(-ENOENT);

	error = of_clk_add_provider(np, cpg_mssr_clk_src_twocell_get, priv);
	if (error)
		goto out_err;

	return 0;

out_err:
	if (priv->base)
		iounmap(priv->base);
	kfree(priv);

	return error;
}

void __init cpg_mssr_early_init(struct device_node *np,
				const struct cpg_mssr_info *info)
{
	int error;
	int i;

	error = cpg_mssr_common_init(NULL, np, info);
	if (error)
		return;

	for (i = 0; i < info->num_early_core_clks; i++)
		cpg_mssr_register_core_clk(&info->early_core_clks[i], info,
					   cpg_mssr_priv);

	for (i = 0; i < info->num_early_mod_clks; i++)
		cpg_mssr_register_mod_clk(&info->early_mod_clks[i], info,
					  cpg_mssr_priv);

}

static int __init cpg_mssr_probe(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	struct device_node *np = dev->of_node;
	const struct cpg_mssr_info *info;
	struct cpg_mssr_priv *priv;
	unsigned int i;
	int error;

	info = of_device_get_match_data(dev);

	if (!cpg_mssr_priv) {
		error = cpg_mssr_common_init(dev, dev->of_node, info);
		if (error)
			return error;
	}

	priv = cpg_mssr_priv;
	priv->dev = dev;
	dev_set_drvdata(dev, priv);
>>>>>>> upstream/android-13

	for (i = 0; i < info->num_core_clks; i++)
		cpg_mssr_register_core_clk(&info->core_clks[i], info, priv);

	for (i = 0; i < info->num_mod_clks; i++)
		cpg_mssr_register_mod_clk(&info->mod_clks[i], info, priv);

<<<<<<< HEAD
	error = of_clk_add_provider(np, cpg_mssr_clk_src_twocell_get, priv);
	if (error)
		return error;

=======
>>>>>>> upstream/android-13
	error = devm_add_action_or_reset(dev,
					 cpg_mssr_del_clk_provider,
					 np);
	if (error)
		return error;

	error = cpg_mssr_add_clk_domain(dev, info->core_pm_clks,
					info->num_core_pm_clks);
	if (error)
		return error;

<<<<<<< HEAD
=======
	/* Reset Controller not supported for Standby Control SoCs */
	if (priv->reg_layout == CLK_REG_LAYOUT_RZ_A)
		return 0;

>>>>>>> upstream/android-13
	error = cpg_mssr_reset_controller_register(priv);
	if (error)
		return error;

	return 0;
}

static struct platform_driver cpg_mssr_driver = {
	.driver		= {
		.name	= "renesas-cpg-mssr",
		.of_match_table = cpg_mssr_match,
		.pm = DEV_PM_OPS,
	},
};

static int __init cpg_mssr_init(void)
{
	return platform_driver_probe(&cpg_mssr_driver, cpg_mssr_probe);
}

subsys_initcall(cpg_mssr_init);

void __init cpg_core_nullify_range(struct cpg_core_clk *core_clks,
				   unsigned int num_core_clks,
				   unsigned int first_clk,
				   unsigned int last_clk)
{
	unsigned int i;

	for (i = 0; i < num_core_clks; i++)
		if (core_clks[i].id >= first_clk &&
		    core_clks[i].id <= last_clk)
			core_clks[i].name = NULL;
}

void __init mssr_mod_nullify(struct mssr_mod_clk *mod_clks,
			     unsigned int num_mod_clks,
			     const unsigned int *clks, unsigned int n)
{
	unsigned int i, j;

	for (i = 0, j = 0; i < num_mod_clks && j < n; i++)
		if (mod_clks[i].id == clks[j]) {
			mod_clks[i].name = NULL;
			j++;
		}
}

void __init mssr_mod_reparent(struct mssr_mod_clk *mod_clks,
			      unsigned int num_mod_clks,
			      const struct mssr_mod_reparent *clks,
			      unsigned int n)
{
	unsigned int i, j;

	for (i = 0, j = 0; i < num_mod_clks && j < n; i++)
		if (mod_clks[i].id == clks[j].clk) {
			mod_clks[i].parent = clks[j].parent;
			j++;
		}
}

MODULE_DESCRIPTION("Renesas CPG/MSSR Driver");
MODULE_LICENSE("GPL v2");
