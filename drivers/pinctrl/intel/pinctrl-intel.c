// SPDX-License-Identifier: GPL-2.0
/*
 * Intel pinctrl/GPIO core driver.
 *
 * Copyright (C) 2015, Intel Corporation
 * Authors: Mathias Nyman <mathias.nyman@linux.intel.com>
 *          Mika Westerberg <mika.westerberg@linux.intel.com>
 */

<<<<<<< HEAD
#include <linux/module.h>
#include <linux/interrupt.h>
#include <linux/gpio/driver.h>
#include <linux/log2.h>
#include <linux/platform_device.h>
=======
#include <linux/acpi.h>
#include <linux/gpio/driver.h>
#include <linux/interrupt.h>
#include <linux/log2.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/property.h>
#include <linux/time.h>

>>>>>>> upstream/android-13
#include <linux/pinctrl/pinctrl.h>
#include <linux/pinctrl/pinmux.h>
#include <linux/pinctrl/pinconf.h>
#include <linux/pinctrl/pinconf-generic.h>

#include "../core.h"
#include "pinctrl-intel.h"

/* Offset from regs */
#define REVID				0x000
#define REVID_SHIFT			16
#define REVID_MASK			GENMASK(31, 16)

<<<<<<< HEAD
#define PADBAR				0x00c
#define GPI_IS				0x100

#define PADOWN_BITS			4
#define PADOWN_SHIFT(p)			((p) % 8 * PADOWN_BITS)
#define PADOWN_MASK(p)			(0xf << PADOWN_SHIFT(p))
=======
#define CAPLIST				0x004
#define CAPLIST_ID_SHIFT		16
#define CAPLIST_ID_MASK			GENMASK(23, 16)
#define CAPLIST_ID_GPIO_HW_INFO		1
#define CAPLIST_ID_PWM			2
#define CAPLIST_ID_BLINK		3
#define CAPLIST_ID_EXP			4
#define CAPLIST_NEXT_SHIFT		0
#define CAPLIST_NEXT_MASK		GENMASK(15, 0)

#define PADBAR				0x00c

#define PADOWN_BITS			4
#define PADOWN_SHIFT(p)			((p) % 8 * PADOWN_BITS)
#define PADOWN_MASK(p)			(GENMASK(3, 0) << PADOWN_SHIFT(p))
>>>>>>> upstream/android-13
#define PADOWN_GPP(p)			((p) / 8)

/* Offset from pad_regs */
#define PADCFG0				0x000
#define PADCFG0_RXEVCFG_SHIFT		25
<<<<<<< HEAD
#define PADCFG0_RXEVCFG_MASK		(3 << PADCFG0_RXEVCFG_SHIFT)
=======
#define PADCFG0_RXEVCFG_MASK		GENMASK(26, 25)
>>>>>>> upstream/android-13
#define PADCFG0_RXEVCFG_LEVEL		0
#define PADCFG0_RXEVCFG_EDGE		1
#define PADCFG0_RXEVCFG_DISABLED	2
#define PADCFG0_RXEVCFG_EDGE_BOTH	3
#define PADCFG0_PREGFRXSEL		BIT(24)
#define PADCFG0_RXINV			BIT(23)
#define PADCFG0_GPIROUTIOXAPIC		BIT(20)
#define PADCFG0_GPIROUTSCI		BIT(19)
#define PADCFG0_GPIROUTSMI		BIT(18)
#define PADCFG0_GPIROUTNMI		BIT(17)
#define PADCFG0_PMODE_SHIFT		10
<<<<<<< HEAD
#define PADCFG0_PMODE_MASK		(0xf << PADCFG0_PMODE_SHIFT)
=======
#define PADCFG0_PMODE_MASK		GENMASK(13, 10)
>>>>>>> upstream/android-13
#define PADCFG0_PMODE_GPIO		0
#define PADCFG0_GPIORXDIS		BIT(9)
#define PADCFG0_GPIOTXDIS		BIT(8)
#define PADCFG0_GPIORXSTATE		BIT(1)
#define PADCFG0_GPIOTXSTATE		BIT(0)

#define PADCFG1				0x004
#define PADCFG1_TERM_UP			BIT(13)
#define PADCFG1_TERM_SHIFT		10
<<<<<<< HEAD
#define PADCFG1_TERM_MASK		(7 << PADCFG1_TERM_SHIFT)
#define PADCFG1_TERM_20K		4
#define PADCFG1_TERM_2K			3
#define PADCFG1_TERM_5K			2
#define PADCFG1_TERM_1K			1
=======
#define PADCFG1_TERM_MASK		GENMASK(12, 10)
#define PADCFG1_TERM_20K		BIT(2)
#define PADCFG1_TERM_5K			BIT(1)
#define PADCFG1_TERM_1K			BIT(0)
#define PADCFG1_TERM_833		(BIT(1) | BIT(0))
>>>>>>> upstream/android-13

#define PADCFG2				0x008
#define PADCFG2_DEBEN			BIT(0)
#define PADCFG2_DEBOUNCE_SHIFT		1
#define PADCFG2_DEBOUNCE_MASK		GENMASK(4, 1)

<<<<<<< HEAD
#define DEBOUNCE_PERIOD			31250 /* ns */
=======
#define DEBOUNCE_PERIOD_NSEC		31250
>>>>>>> upstream/android-13

struct intel_pad_context {
	u32 padcfg0;
	u32 padcfg1;
	u32 padcfg2;
};

struct intel_community_context {
	u32 *intmask;
<<<<<<< HEAD
};

struct intel_pinctrl_context {
	struct intel_pad_context *pads;
	struct intel_community_context *communities;
};

/**
 * struct intel_pinctrl - Intel pinctrl private structure
 * @dev: Pointer to the device structure
 * @lock: Lock to serialize register access
 * @pctldesc: Pin controller description
 * @pctldev: Pointer to the pin controller device
 * @chip: GPIO chip in this pin controller
 * @soc: SoC/PCH specific pin configuration data
 * @communities: All communities in this pin controller
 * @ncommunities: Number of communities in this pin controller
 * @context: Configuration saved over system sleep
 * @irq: pinctrl/GPIO chip irq number
 */
struct intel_pinctrl {
	struct device *dev;
	raw_spinlock_t lock;
	struct pinctrl_desc pctldesc;
	struct pinctrl_dev *pctldev;
	struct gpio_chip chip;
	const struct intel_pinctrl_soc_data *soc;
	struct intel_community *communities;
	size_t ncommunities;
	struct intel_pinctrl_context context;
	int irq;
=======
	u32 *hostown;
>>>>>>> upstream/android-13
};

#define pin_to_padno(c, p)	((p) - (c)->pin_base)
#define padgroup_offset(g, p)	((p) - (g)->base)

static struct intel_community *intel_get_community(struct intel_pinctrl *pctrl,
<<<<<<< HEAD
						   unsigned pin)
=======
						   unsigned int pin)
>>>>>>> upstream/android-13
{
	struct intel_community *community;
	int i;

	for (i = 0; i < pctrl->ncommunities; i++) {
		community = &pctrl->communities[i];
		if (pin >= community->pin_base &&
		    pin < community->pin_base + community->npins)
			return community;
	}

	dev_warn(pctrl->dev, "failed to find community for pin %u\n", pin);
	return NULL;
}

static const struct intel_padgroup *
intel_community_get_padgroup(const struct intel_community *community,
<<<<<<< HEAD
			     unsigned pin)
=======
			     unsigned int pin)
>>>>>>> upstream/android-13
{
	int i;

	for (i = 0; i < community->ngpps; i++) {
		const struct intel_padgroup *padgrp = &community->gpps[i];

		if (pin >= padgrp->base && pin < padgrp->base + padgrp->size)
			return padgrp;
	}

	return NULL;
}

<<<<<<< HEAD
static void __iomem *intel_get_padcfg(struct intel_pinctrl *pctrl, unsigned pin,
				      unsigned reg)
{
	const struct intel_community *community;
	unsigned padno;
=======
static void __iomem *intel_get_padcfg(struct intel_pinctrl *pctrl,
				      unsigned int pin, unsigned int reg)
{
	const struct intel_community *community;
	unsigned int padno;
>>>>>>> upstream/android-13
	size_t nregs;

	community = intel_get_community(pctrl, pin);
	if (!community)
		return NULL;

	padno = pin_to_padno(community, pin);
	nregs = (community->features & PINCTRL_FEATURE_DEBOUNCE) ? 4 : 2;

<<<<<<< HEAD
	if (reg == PADCFG2 && !(community->features & PINCTRL_FEATURE_DEBOUNCE))
=======
	if (reg >= nregs * 4)
>>>>>>> upstream/android-13
		return NULL;

	return community->pad_regs + reg + padno * nregs * 4;
}

<<<<<<< HEAD
static bool intel_pad_owned_by_host(struct intel_pinctrl *pctrl, unsigned pin)
{
	const struct intel_community *community;
	const struct intel_padgroup *padgrp;
	unsigned gpp, offset, gpp_offset;
=======
static bool intel_pad_owned_by_host(struct intel_pinctrl *pctrl, unsigned int pin)
{
	const struct intel_community *community;
	const struct intel_padgroup *padgrp;
	unsigned int gpp, offset, gpp_offset;
>>>>>>> upstream/android-13
	void __iomem *padown;

	community = intel_get_community(pctrl, pin);
	if (!community)
		return false;
	if (!community->padown_offset)
		return true;

	padgrp = intel_community_get_padgroup(community, pin);
	if (!padgrp)
		return false;

	gpp_offset = padgroup_offset(padgrp, pin);
	gpp = PADOWN_GPP(gpp_offset);
	offset = community->padown_offset + padgrp->padown_num * 4 + gpp * 4;
	padown = community->regs + offset;

	return !(readl(padown) & PADOWN_MASK(gpp_offset));
}

<<<<<<< HEAD
static bool intel_pad_acpi_mode(struct intel_pinctrl *pctrl, unsigned pin)
{
	const struct intel_community *community;
	const struct intel_padgroup *padgrp;
	unsigned offset, gpp_offset;
=======
static bool intel_pad_acpi_mode(struct intel_pinctrl *pctrl, unsigned int pin)
{
	const struct intel_community *community;
	const struct intel_padgroup *padgrp;
	unsigned int offset, gpp_offset;
>>>>>>> upstream/android-13
	void __iomem *hostown;

	community = intel_get_community(pctrl, pin);
	if (!community)
		return true;
	if (!community->hostown_offset)
		return false;

	padgrp = intel_community_get_padgroup(community, pin);
	if (!padgrp)
		return true;

	gpp_offset = padgroup_offset(padgrp, pin);
	offset = community->hostown_offset + padgrp->reg_num * 4;
	hostown = community->regs + offset;

	return !(readl(hostown) & BIT(gpp_offset));
}

<<<<<<< HEAD
static bool intel_pad_locked(struct intel_pinctrl *pctrl, unsigned pin)
{
	struct intel_community *community;
	const struct intel_padgroup *padgrp;
	unsigned offset, gpp_offset;
	u32 value;

	community = intel_get_community(pctrl, pin);
	if (!community)
		return true;
	if (!community->padcfglock_offset)
		return false;

	padgrp = intel_community_get_padgroup(community, pin);
	if (!padgrp)
		return true;
=======
/**
 * enum - Locking variants of the pad configuration
 *
 * @PAD_UNLOCKED:	pad is fully controlled by the configuration registers
 * @PAD_LOCKED:		pad configuration registers, except TX state, are locked
 * @PAD_LOCKED_TX:	pad configuration TX state is locked
 * @PAD_LOCKED_FULL:	pad configuration registers are locked completely
 *
 * Locking is considered as read-only mode for corresponding registers and
 * their respective fields. That said, TX state bit is locked separately from
 * the main locking scheme.
 */
enum {
	PAD_UNLOCKED	= 0,
	PAD_LOCKED	= 1,
	PAD_LOCKED_TX	= 2,
	PAD_LOCKED_FULL	= PAD_LOCKED | PAD_LOCKED_TX,
};

static int intel_pad_locked(struct intel_pinctrl *pctrl, unsigned int pin)
{
	struct intel_community *community;
	const struct intel_padgroup *padgrp;
	unsigned int offset, gpp_offset;
	u32 value;
	int ret = PAD_UNLOCKED;

	community = intel_get_community(pctrl, pin);
	if (!community)
		return PAD_LOCKED_FULL;
	if (!community->padcfglock_offset)
		return PAD_UNLOCKED;

	padgrp = intel_community_get_padgroup(community, pin);
	if (!padgrp)
		return PAD_LOCKED_FULL;
>>>>>>> upstream/android-13

	gpp_offset = padgroup_offset(padgrp, pin);

	/*
	 * If PADCFGLOCK and PADCFGLOCKTX bits are both clear for this pad,
	 * the pad is considered unlocked. Any other case means that it is
<<<<<<< HEAD
	 * either fully or partially locked and we don't touch it.
	 */
	offset = community->padcfglock_offset + padgrp->reg_num * 8;
	value = readl(community->regs + offset);
	if (value & BIT(gpp_offset))
		return true;
=======
	 * either fully or partially locked.
	 */
	offset = community->padcfglock_offset + 0 + padgrp->reg_num * 8;
	value = readl(community->regs + offset);
	if (value & BIT(gpp_offset))
		ret |= PAD_LOCKED;
>>>>>>> upstream/android-13

	offset = community->padcfglock_offset + 4 + padgrp->reg_num * 8;
	value = readl(community->regs + offset);
	if (value & BIT(gpp_offset))
<<<<<<< HEAD
		return true;

	return false;
}

static bool intel_pad_usable(struct intel_pinctrl *pctrl, unsigned pin)
{
	return intel_pad_owned_by_host(pctrl, pin) &&
		!intel_pad_locked(pctrl, pin);
=======
		ret |= PAD_LOCKED_TX;

	return ret;
}

static bool intel_pad_is_unlocked(struct intel_pinctrl *pctrl, unsigned int pin)
{
	return (intel_pad_locked(pctrl, pin) & PAD_LOCKED) == PAD_UNLOCKED;
}

static bool intel_pad_usable(struct intel_pinctrl *pctrl, unsigned int pin)
{
	return intel_pad_owned_by_host(pctrl, pin) && intel_pad_is_unlocked(pctrl, pin);
>>>>>>> upstream/android-13
}

static int intel_get_groups_count(struct pinctrl_dev *pctldev)
{
	struct intel_pinctrl *pctrl = pinctrl_dev_get_drvdata(pctldev);

	return pctrl->soc->ngroups;
}

static const char *intel_get_group_name(struct pinctrl_dev *pctldev,
<<<<<<< HEAD
				      unsigned group)
=======
				      unsigned int group)
>>>>>>> upstream/android-13
{
	struct intel_pinctrl *pctrl = pinctrl_dev_get_drvdata(pctldev);

	return pctrl->soc->groups[group].name;
}

<<<<<<< HEAD
static int intel_get_group_pins(struct pinctrl_dev *pctldev, unsigned group,
			      const unsigned **pins, unsigned *npins)
=======
static int intel_get_group_pins(struct pinctrl_dev *pctldev, unsigned int group,
			      const unsigned int **pins, unsigned int *npins)
>>>>>>> upstream/android-13
{
	struct intel_pinctrl *pctrl = pinctrl_dev_get_drvdata(pctldev);

	*pins = pctrl->soc->groups[group].pins;
	*npins = pctrl->soc->groups[group].npins;
	return 0;
}

static void intel_pin_dbg_show(struct pinctrl_dev *pctldev, struct seq_file *s,
<<<<<<< HEAD
			       unsigned pin)
=======
			       unsigned int pin)
>>>>>>> upstream/android-13
{
	struct intel_pinctrl *pctrl = pinctrl_dev_get_drvdata(pctldev);
	void __iomem *padcfg;
	u32 cfg0, cfg1, mode;
<<<<<<< HEAD
	bool locked, acpi;
=======
	int locked;
	bool acpi;
>>>>>>> upstream/android-13

	if (!intel_pad_owned_by_host(pctrl, pin)) {
		seq_puts(s, "not available");
		return;
	}

	cfg0 = readl(intel_get_padcfg(pctrl, pin, PADCFG0));
	cfg1 = readl(intel_get_padcfg(pctrl, pin, PADCFG1));

	mode = (cfg0 & PADCFG0_PMODE_MASK) >> PADCFG0_PMODE_SHIFT;
	if (mode == PADCFG0_PMODE_GPIO)
		seq_puts(s, "GPIO ");
	else
		seq_printf(s, "mode %d ", mode);

	seq_printf(s, "0x%08x 0x%08x", cfg0, cfg1);

	/* Dump the additional PADCFG registers if available */
	padcfg = intel_get_padcfg(pctrl, pin, PADCFG2);
	if (padcfg)
		seq_printf(s, " 0x%08x", readl(padcfg));

	locked = intel_pad_locked(pctrl, pin);
	acpi = intel_pad_acpi_mode(pctrl, pin);

	if (locked || acpi) {
		seq_puts(s, " [");
<<<<<<< HEAD
		if (locked) {
			seq_puts(s, "LOCKED");
			if (acpi)
				seq_puts(s, ", ");
		}
=======
		if (locked)
			seq_puts(s, "LOCKED");
		if ((locked & PAD_LOCKED_FULL) == PAD_LOCKED_TX)
			seq_puts(s, " tx");
		else if ((locked & PAD_LOCKED_FULL) == PAD_LOCKED_FULL)
			seq_puts(s, " full");

		if (locked && acpi)
			seq_puts(s, ", ");

>>>>>>> upstream/android-13
		if (acpi)
			seq_puts(s, "ACPI");
		seq_puts(s, "]");
	}
}

static const struct pinctrl_ops intel_pinctrl_ops = {
	.get_groups_count = intel_get_groups_count,
	.get_group_name = intel_get_group_name,
	.get_group_pins = intel_get_group_pins,
	.pin_dbg_show = intel_pin_dbg_show,
};

static int intel_get_functions_count(struct pinctrl_dev *pctldev)
{
	struct intel_pinctrl *pctrl = pinctrl_dev_get_drvdata(pctldev);

	return pctrl->soc->nfunctions;
}

static const char *intel_get_function_name(struct pinctrl_dev *pctldev,
<<<<<<< HEAD
					   unsigned function)
=======
					   unsigned int function)
>>>>>>> upstream/android-13
{
	struct intel_pinctrl *pctrl = pinctrl_dev_get_drvdata(pctldev);

	return pctrl->soc->functions[function].name;
}

static int intel_get_function_groups(struct pinctrl_dev *pctldev,
<<<<<<< HEAD
				     unsigned function,
				     const char * const **groups,
				     unsigned * const ngroups)
=======
				     unsigned int function,
				     const char * const **groups,
				     unsigned int * const ngroups)
>>>>>>> upstream/android-13
{
	struct intel_pinctrl *pctrl = pinctrl_dev_get_drvdata(pctldev);

	*groups = pctrl->soc->functions[function].groups;
	*ngroups = pctrl->soc->functions[function].ngroups;
	return 0;
}

<<<<<<< HEAD
static int intel_pinmux_set_mux(struct pinctrl_dev *pctldev, unsigned function,
				unsigned group)
=======
static int intel_pinmux_set_mux(struct pinctrl_dev *pctldev,
				unsigned int function, unsigned int group)
>>>>>>> upstream/android-13
{
	struct intel_pinctrl *pctrl = pinctrl_dev_get_drvdata(pctldev);
	const struct intel_pingroup *grp = &pctrl->soc->groups[group];
	unsigned long flags;
	int i;

	raw_spin_lock_irqsave(&pctrl->lock, flags);

	/*
	 * All pins in the groups needs to be accessible and writable
	 * before we can enable the mux for this group.
	 */
	for (i = 0; i < grp->npins; i++) {
		if (!intel_pad_usable(pctrl, grp->pins[i])) {
			raw_spin_unlock_irqrestore(&pctrl->lock, flags);
			return -EBUSY;
		}
	}

	/* Now enable the mux setting for each pin in the group */
	for (i = 0; i < grp->npins; i++) {
		void __iomem *padcfg0;
		u32 value;

		padcfg0 = intel_get_padcfg(pctrl, grp->pins[i], PADCFG0);
		value = readl(padcfg0);

		value &= ~PADCFG0_PMODE_MASK;

		if (grp->modes)
			value |= grp->modes[i] << PADCFG0_PMODE_SHIFT;
		else
			value |= grp->mode << PADCFG0_PMODE_SHIFT;

		writel(value, padcfg0);
	}

	raw_spin_unlock_irqrestore(&pctrl->lock, flags);

	return 0;
}

static void __intel_gpio_set_direction(void __iomem *padcfg0, bool input)
{
	u32 value;

	value = readl(padcfg0);
	if (input) {
		value &= ~PADCFG0_GPIORXDIS;
		value |= PADCFG0_GPIOTXDIS;
	} else {
		value &= ~PADCFG0_GPIOTXDIS;
		value |= PADCFG0_GPIORXDIS;
	}
	writel(value, padcfg0);
}

static int intel_gpio_get_gpio_mode(void __iomem *padcfg0)
{
	return (readl(padcfg0) & PADCFG0_PMODE_MASK) >> PADCFG0_PMODE_SHIFT;
}

static void intel_gpio_set_gpio_mode(void __iomem *padcfg0)
{
	u32 value;

<<<<<<< HEAD
	/* Put the pad into GPIO mode */
	value = readl(padcfg0) & ~PADCFG0_PMODE_MASK;
	/* Disable SCI/SMI/NMI generation */
	value &= ~(PADCFG0_GPIROUTIOXAPIC | PADCFG0_GPIROUTSCI);
	value &= ~(PADCFG0_GPIROUTSMI | PADCFG0_GPIROUTNMI);
=======
	value = readl(padcfg0);

	/* Put the pad into GPIO mode */
	value &= ~PADCFG0_PMODE_MASK;
	value |= PADCFG0_PMODE_GPIO;

	/* Disable TX buffer and enable RX (this will be input) */
	value &= ~PADCFG0_GPIORXDIS;
	value |= PADCFG0_GPIOTXDIS;

	/* Disable SCI/SMI/NMI generation */
	value &= ~(PADCFG0_GPIROUTIOXAPIC | PADCFG0_GPIROUTSCI);
	value &= ~(PADCFG0_GPIROUTSMI | PADCFG0_GPIROUTNMI);

>>>>>>> upstream/android-13
	writel(value, padcfg0);
}

static int intel_gpio_request_enable(struct pinctrl_dev *pctldev,
				     struct pinctrl_gpio_range *range,
<<<<<<< HEAD
				     unsigned pin)
=======
				     unsigned int pin)
>>>>>>> upstream/android-13
{
	struct intel_pinctrl *pctrl = pinctrl_dev_get_drvdata(pctldev);
	void __iomem *padcfg0;
	unsigned long flags;

<<<<<<< HEAD
	raw_spin_lock_irqsave(&pctrl->lock, flags);

	if (!intel_pad_usable(pctrl, pin)) {
=======
	padcfg0 = intel_get_padcfg(pctrl, pin, PADCFG0);

	raw_spin_lock_irqsave(&pctrl->lock, flags);

	if (!intel_pad_owned_by_host(pctrl, pin)) {
>>>>>>> upstream/android-13
		raw_spin_unlock_irqrestore(&pctrl->lock, flags);
		return -EBUSY;
	}

<<<<<<< HEAD
	padcfg0 = intel_get_padcfg(pctrl, pin, PADCFG0);
=======
	if (!intel_pad_is_unlocked(pctrl, pin)) {
		raw_spin_unlock_irqrestore(&pctrl->lock, flags);
		return 0;
	}
>>>>>>> upstream/android-13

	/*
	 * If pin is already configured in GPIO mode, we assume that
	 * firmware provides correct settings. In such case we avoid
	 * potential glitches on the pin. Otherwise, for the pin in
	 * alternative mode, consumer has to supply respective flags.
	 */
	if (intel_gpio_get_gpio_mode(padcfg0) == PADCFG0_PMODE_GPIO) {
		raw_spin_unlock_irqrestore(&pctrl->lock, flags);
		return 0;
	}

	intel_gpio_set_gpio_mode(padcfg0);

<<<<<<< HEAD
	/* Disable TX buffer and enable RX (this will be input) */
	__intel_gpio_set_direction(padcfg0, true);

=======
>>>>>>> upstream/android-13
	raw_spin_unlock_irqrestore(&pctrl->lock, flags);

	return 0;
}

static int intel_gpio_set_direction(struct pinctrl_dev *pctldev,
				    struct pinctrl_gpio_range *range,
<<<<<<< HEAD
				    unsigned pin, bool input)
=======
				    unsigned int pin, bool input)
>>>>>>> upstream/android-13
{
	struct intel_pinctrl *pctrl = pinctrl_dev_get_drvdata(pctldev);
	void __iomem *padcfg0;
	unsigned long flags;

<<<<<<< HEAD
	raw_spin_lock_irqsave(&pctrl->lock, flags);

	padcfg0 = intel_get_padcfg(pctrl, pin, PADCFG0);
	__intel_gpio_set_direction(padcfg0, input);

=======
	padcfg0 = intel_get_padcfg(pctrl, pin, PADCFG0);

	raw_spin_lock_irqsave(&pctrl->lock, flags);
	__intel_gpio_set_direction(padcfg0, input);
>>>>>>> upstream/android-13
	raw_spin_unlock_irqrestore(&pctrl->lock, flags);

	return 0;
}

static const struct pinmux_ops intel_pinmux_ops = {
	.get_functions_count = intel_get_functions_count,
	.get_function_name = intel_get_function_name,
	.get_function_groups = intel_get_function_groups,
	.set_mux = intel_pinmux_set_mux,
	.gpio_request_enable = intel_gpio_request_enable,
	.gpio_set_direction = intel_gpio_set_direction,
};

<<<<<<< HEAD
static int intel_config_get(struct pinctrl_dev *pctldev, unsigned pin,
			    unsigned long *config)
{
	struct intel_pinctrl *pctrl = pinctrl_dev_get_drvdata(pctldev);
	enum pin_config_param param = pinconf_to_config_param(*config);
	const struct intel_community *community;
	u32 value, term;
	u32 arg = 0;

	if (!intel_pad_owned_by_host(pctrl, pin))
		return -ENOTSUPP;

	community = intel_get_community(pctrl, pin);
	value = readl(intel_get_padcfg(pctrl, pin, PADCFG1));
=======
static int intel_config_get_pull(struct intel_pinctrl *pctrl, unsigned int pin,
				 enum pin_config_param param, u32 *arg)
{
	const struct intel_community *community;
	void __iomem *padcfg1;
	unsigned long flags;
	u32 value, term;

	community = intel_get_community(pctrl, pin);
	padcfg1 = intel_get_padcfg(pctrl, pin, PADCFG1);

	raw_spin_lock_irqsave(&pctrl->lock, flags);
	value = readl(padcfg1);
	raw_spin_unlock_irqrestore(&pctrl->lock, flags);

>>>>>>> upstream/android-13
	term = (value & PADCFG1_TERM_MASK) >> PADCFG1_TERM_SHIFT;

	switch (param) {
	case PIN_CONFIG_BIAS_DISABLE:
		if (term)
			return -EINVAL;
		break;

	case PIN_CONFIG_BIAS_PULL_UP:
		if (!term || !(value & PADCFG1_TERM_UP))
			return -EINVAL;

		switch (term) {
<<<<<<< HEAD
		case PADCFG1_TERM_1K:
			arg = 1000;
			break;
		case PADCFG1_TERM_2K:
			arg = 2000;
			break;
		case PADCFG1_TERM_5K:
			arg = 5000;
			break;
		case PADCFG1_TERM_20K:
			arg = 20000;
=======
		case PADCFG1_TERM_833:
			*arg = 833;
			break;
		case PADCFG1_TERM_1K:
			*arg = 1000;
			break;
		case PADCFG1_TERM_5K:
			*arg = 5000;
			break;
		case PADCFG1_TERM_20K:
			*arg = 20000;
>>>>>>> upstream/android-13
			break;
		}

		break;

	case PIN_CONFIG_BIAS_PULL_DOWN:
		if (!term || value & PADCFG1_TERM_UP)
			return -EINVAL;

		switch (term) {
<<<<<<< HEAD
		case PADCFG1_TERM_1K:
			if (!(community->features & PINCTRL_FEATURE_1K_PD))
				return -EINVAL;
			arg = 1000;
			break;
		case PADCFG1_TERM_5K:
			arg = 5000;
			break;
		case PADCFG1_TERM_20K:
			arg = 20000;
=======
		case PADCFG1_TERM_833:
			if (!(community->features & PINCTRL_FEATURE_1K_PD))
				return -EINVAL;
			*arg = 833;
			break;
		case PADCFG1_TERM_1K:
			if (!(community->features & PINCTRL_FEATURE_1K_PD))
				return -EINVAL;
			*arg = 1000;
			break;
		case PADCFG1_TERM_5K:
			*arg = 5000;
			break;
		case PADCFG1_TERM_20K:
			*arg = 20000;
>>>>>>> upstream/android-13
			break;
		}

		break;

<<<<<<< HEAD
	case PIN_CONFIG_INPUT_DEBOUNCE: {
		void __iomem *padcfg2;
		u32 v;

		padcfg2 = intel_get_padcfg(pctrl, pin, PADCFG2);
		if (!padcfg2)
			return -ENOTSUPP;

		v = readl(padcfg2);
		if (!(v & PADCFG2_DEBEN))
			return -EINVAL;

		v = (v & PADCFG2_DEBOUNCE_MASK) >> PADCFG2_DEBOUNCE_SHIFT;
		arg = BIT(v) * DEBOUNCE_PERIOD / 1000;

		break;
	}

=======
	default:
		return -EINVAL;
	}

	return 0;
}

static int intel_config_get_debounce(struct intel_pinctrl *pctrl, unsigned int pin,
				     enum pin_config_param param, u32 *arg)
{
	void __iomem *padcfg2;
	unsigned long flags;
	unsigned long v;
	u32 value2;

	padcfg2 = intel_get_padcfg(pctrl, pin, PADCFG2);
	if (!padcfg2)
		return -ENOTSUPP;

	raw_spin_lock_irqsave(&pctrl->lock, flags);
	value2 = readl(padcfg2);
	raw_spin_unlock_irqrestore(&pctrl->lock, flags);
	if (!(value2 & PADCFG2_DEBEN))
		return -EINVAL;

	v = (value2 & PADCFG2_DEBOUNCE_MASK) >> PADCFG2_DEBOUNCE_SHIFT;
	*arg = BIT(v) * DEBOUNCE_PERIOD_NSEC / NSEC_PER_USEC;

	return 0;
}

static int intel_config_get(struct pinctrl_dev *pctldev, unsigned int pin,
			    unsigned long *config)
{
	struct intel_pinctrl *pctrl = pinctrl_dev_get_drvdata(pctldev);
	enum pin_config_param param = pinconf_to_config_param(*config);
	u32 arg = 0;
	int ret;

	if (!intel_pad_owned_by_host(pctrl, pin))
		return -ENOTSUPP;

	switch (param) {
	case PIN_CONFIG_BIAS_DISABLE:
	case PIN_CONFIG_BIAS_PULL_UP:
	case PIN_CONFIG_BIAS_PULL_DOWN:
		ret = intel_config_get_pull(pctrl, pin, param, &arg);
		if (ret)
			return ret;
		break;

	case PIN_CONFIG_INPUT_DEBOUNCE:
		ret = intel_config_get_debounce(pctrl, pin, param, &arg);
		if (ret)
			return ret;
		break;

>>>>>>> upstream/android-13
	default:
		return -ENOTSUPP;
	}

	*config = pinconf_to_config_packed(param, arg);
	return 0;
}

<<<<<<< HEAD
static int intel_config_set_pull(struct intel_pinctrl *pctrl, unsigned pin,
				 unsigned long config)
{
	unsigned param = pinconf_to_config_param(config);
	unsigned arg = pinconf_to_config_argument(config);
=======
static int intel_config_set_pull(struct intel_pinctrl *pctrl, unsigned int pin,
				 unsigned long config)
{
	unsigned int param = pinconf_to_config_param(config);
	unsigned int arg = pinconf_to_config_argument(config);
>>>>>>> upstream/android-13
	const struct intel_community *community;
	void __iomem *padcfg1;
	unsigned long flags;
	int ret = 0;
	u32 value;

<<<<<<< HEAD
	raw_spin_lock_irqsave(&pctrl->lock, flags);

	community = intel_get_community(pctrl, pin);
	padcfg1 = intel_get_padcfg(pctrl, pin, PADCFG1);
=======
	community = intel_get_community(pctrl, pin);
	padcfg1 = intel_get_padcfg(pctrl, pin, PADCFG1);

	raw_spin_lock_irqsave(&pctrl->lock, flags);

>>>>>>> upstream/android-13
	value = readl(padcfg1);

	switch (param) {
	case PIN_CONFIG_BIAS_DISABLE:
		value &= ~(PADCFG1_TERM_MASK | PADCFG1_TERM_UP);
		break;

	case PIN_CONFIG_BIAS_PULL_UP:
		value &= ~PADCFG1_TERM_MASK;

		value |= PADCFG1_TERM_UP;

		/* Set default strength value in case none is given */
		if (arg == 1)
			arg = 5000;

		switch (arg) {
		case 20000:
			value |= PADCFG1_TERM_20K << PADCFG1_TERM_SHIFT;
			break;
		case 5000:
			value |= PADCFG1_TERM_5K << PADCFG1_TERM_SHIFT;
			break;
<<<<<<< HEAD
		case 2000:
			value |= PADCFG1_TERM_2K << PADCFG1_TERM_SHIFT;
			break;
		case 1000:
			value |= PADCFG1_TERM_1K << PADCFG1_TERM_SHIFT;
			break;
=======
		case 1000:
			value |= PADCFG1_TERM_1K << PADCFG1_TERM_SHIFT;
			break;
		case 833:
			value |= PADCFG1_TERM_833 << PADCFG1_TERM_SHIFT;
			break;
>>>>>>> upstream/android-13
		default:
			ret = -EINVAL;
		}

		break;

	case PIN_CONFIG_BIAS_PULL_DOWN:
		value &= ~(PADCFG1_TERM_UP | PADCFG1_TERM_MASK);

		/* Set default strength value in case none is given */
		if (arg == 1)
			arg = 5000;

		switch (arg) {
		case 20000:
			value |= PADCFG1_TERM_20K << PADCFG1_TERM_SHIFT;
			break;
		case 5000:
			value |= PADCFG1_TERM_5K << PADCFG1_TERM_SHIFT;
			break;
		case 1000:
			if (!(community->features & PINCTRL_FEATURE_1K_PD)) {
				ret = -EINVAL;
				break;
			}
			value |= PADCFG1_TERM_1K << PADCFG1_TERM_SHIFT;
			break;
<<<<<<< HEAD
=======
		case 833:
			if (!(community->features & PINCTRL_FEATURE_1K_PD)) {
				ret = -EINVAL;
				break;
			}
			value |= PADCFG1_TERM_833 << PADCFG1_TERM_SHIFT;
			break;
>>>>>>> upstream/android-13
		default:
			ret = -EINVAL;
		}

		break;
	}

	if (!ret)
		writel(value, padcfg1);

	raw_spin_unlock_irqrestore(&pctrl->lock, flags);

	return ret;
}

<<<<<<< HEAD
static int intel_config_set_debounce(struct intel_pinctrl *pctrl, unsigned pin,
				     unsigned debounce)
=======
static int intel_config_set_debounce(struct intel_pinctrl *pctrl,
				     unsigned int pin, unsigned int debounce)
>>>>>>> upstream/android-13
{
	void __iomem *padcfg0, *padcfg2;
	unsigned long flags;
	u32 value0, value2;
<<<<<<< HEAD
	int ret = 0;
=======
>>>>>>> upstream/android-13

	padcfg2 = intel_get_padcfg(pctrl, pin, PADCFG2);
	if (!padcfg2)
		return -ENOTSUPP;

	padcfg0 = intel_get_padcfg(pctrl, pin, PADCFG0);

	raw_spin_lock_irqsave(&pctrl->lock, flags);

	value0 = readl(padcfg0);
	value2 = readl(padcfg2);

	/* Disable glitch filter and debouncer */
	value0 &= ~PADCFG0_PREGFRXSEL;
	value2 &= ~(PADCFG2_DEBEN | PADCFG2_DEBOUNCE_MASK);

	if (debounce) {
		unsigned long v;

<<<<<<< HEAD
		v = order_base_2(debounce * 1000 / DEBOUNCE_PERIOD);
		if (v < 3 || v > 15) {
			ret = -EINVAL;
			goto exit_unlock;
		} else {
			/* Enable glitch filter and debouncer */
			value0 |= PADCFG0_PREGFRXSEL;
			value2 |= v << PADCFG2_DEBOUNCE_SHIFT;
			value2 |= PADCFG2_DEBEN;
		}
=======
		v = order_base_2(debounce * NSEC_PER_USEC / DEBOUNCE_PERIOD_NSEC);
		if (v < 3 || v > 15) {
			raw_spin_unlock_irqrestore(&pctrl->lock, flags);
			return -EINVAL;
		}

		/* Enable glitch filter and debouncer */
		value0 |= PADCFG0_PREGFRXSEL;
		value2 |= v << PADCFG2_DEBOUNCE_SHIFT;
		value2 |= PADCFG2_DEBEN;
>>>>>>> upstream/android-13
	}

	writel(value0, padcfg0);
	writel(value2, padcfg2);

<<<<<<< HEAD
exit_unlock:
	raw_spin_unlock_irqrestore(&pctrl->lock, flags);

	return ret;
}

static int intel_config_set(struct pinctrl_dev *pctldev, unsigned pin,
			  unsigned long *configs, unsigned nconfigs)
=======
	raw_spin_unlock_irqrestore(&pctrl->lock, flags);

	return 0;
}

static int intel_config_set(struct pinctrl_dev *pctldev, unsigned int pin,
			  unsigned long *configs, unsigned int nconfigs)
>>>>>>> upstream/android-13
{
	struct intel_pinctrl *pctrl = pinctrl_dev_get_drvdata(pctldev);
	int i, ret;

	if (!intel_pad_usable(pctrl, pin))
		return -ENOTSUPP;

	for (i = 0; i < nconfigs; i++) {
		switch (pinconf_to_config_param(configs[i])) {
		case PIN_CONFIG_BIAS_DISABLE:
		case PIN_CONFIG_BIAS_PULL_UP:
		case PIN_CONFIG_BIAS_PULL_DOWN:
			ret = intel_config_set_pull(pctrl, pin, configs[i]);
			if (ret)
				return ret;
			break;

		case PIN_CONFIG_INPUT_DEBOUNCE:
			ret = intel_config_set_debounce(pctrl, pin,
				pinconf_to_config_argument(configs[i]));
			if (ret)
				return ret;
			break;

		default:
			return -ENOTSUPP;
		}
	}

	return 0;
}

static const struct pinconf_ops intel_pinconf_ops = {
	.is_generic = true,
	.pin_config_get = intel_config_get,
	.pin_config_set = intel_config_set,
};

static const struct pinctrl_desc intel_pinctrl_desc = {
	.pctlops = &intel_pinctrl_ops,
	.pmxops = &intel_pinmux_ops,
	.confops = &intel_pinconf_ops,
	.owner = THIS_MODULE,
};

/**
 * intel_gpio_to_pin() - Translate from GPIO offset to pin number
 * @pctrl: Pinctrl structure
 * @offset: GPIO offset from gpiolib
<<<<<<< HEAD
 * @commmunity: Community is filled here if not %NULL
=======
 * @community: Community is filled here if not %NULL
>>>>>>> upstream/android-13
 * @padgrp: Pad group is filled here if not %NULL
 *
 * When coming through gpiolib irqchip, the GPIO offset is not
 * automatically translated to pinctrl pin number. This function can be
 * used to find out the corresponding pinctrl pin.
 */
<<<<<<< HEAD
static int intel_gpio_to_pin(struct intel_pinctrl *pctrl, unsigned offset,
=======
static int intel_gpio_to_pin(struct intel_pinctrl *pctrl, unsigned int offset,
>>>>>>> upstream/android-13
			     const struct intel_community **community,
			     const struct intel_padgroup **padgrp)
{
	int i;

	for (i = 0; i < pctrl->ncommunities; i++) {
		const struct intel_community *comm = &pctrl->communities[i];
		int j;

		for (j = 0; j < comm->ngpps; j++) {
			const struct intel_padgroup *pgrp = &comm->gpps[j];

<<<<<<< HEAD
			if (pgrp->gpio_base < 0)
=======
			if (pgrp->gpio_base == INTEL_GPIO_BASE_NOMAP)
>>>>>>> upstream/android-13
				continue;

			if (offset >= pgrp->gpio_base &&
			    offset < pgrp->gpio_base + pgrp->size) {
				int pin;

				pin = pgrp->base + offset - pgrp->gpio_base;
				if (community)
					*community = comm;
				if (padgrp)
					*padgrp = pgrp;

				return pin;
			}
		}
	}

	return -EINVAL;
}

<<<<<<< HEAD
static int intel_gpio_get(struct gpio_chip *chip, unsigned offset)
=======
/**
 * intel_pin_to_gpio() - Translate from pin number to GPIO offset
 * @pctrl: Pinctrl structure
 * @pin: pin number
 *
 * Translate the pin number of pinctrl to GPIO offset
 */
static __maybe_unused int intel_pin_to_gpio(struct intel_pinctrl *pctrl, int pin)
{
	const struct intel_community *community;
	const struct intel_padgroup *padgrp;

	community = intel_get_community(pctrl, pin);
	if (!community)
		return -EINVAL;

	padgrp = intel_community_get_padgroup(community, pin);
	if (!padgrp)
		return -EINVAL;

	return pin - padgrp->base + padgrp->gpio_base;
}

static int intel_gpio_get(struct gpio_chip *chip, unsigned int offset)
>>>>>>> upstream/android-13
{
	struct intel_pinctrl *pctrl = gpiochip_get_data(chip);
	void __iomem *reg;
	u32 padcfg0;
	int pin;

	pin = intel_gpio_to_pin(pctrl, offset, NULL, NULL);
	if (pin < 0)
		return -EINVAL;

	reg = intel_get_padcfg(pctrl, pin, PADCFG0);
	if (!reg)
		return -EINVAL;

	padcfg0 = readl(reg);
	if (!(padcfg0 & PADCFG0_GPIOTXDIS))
		return !!(padcfg0 & PADCFG0_GPIOTXSTATE);

	return !!(padcfg0 & PADCFG0_GPIORXSTATE);
}

<<<<<<< HEAD
static void intel_gpio_set(struct gpio_chip *chip, unsigned offset, int value)
=======
static void intel_gpio_set(struct gpio_chip *chip, unsigned int offset,
			   int value)
>>>>>>> upstream/android-13
{
	struct intel_pinctrl *pctrl = gpiochip_get_data(chip);
	unsigned long flags;
	void __iomem *reg;
	u32 padcfg0;
	int pin;

	pin = intel_gpio_to_pin(pctrl, offset, NULL, NULL);
	if (pin < 0)
		return;

	reg = intel_get_padcfg(pctrl, pin, PADCFG0);
	if (!reg)
		return;

	raw_spin_lock_irqsave(&pctrl->lock, flags);
	padcfg0 = readl(reg);
	if (value)
		padcfg0 |= PADCFG0_GPIOTXSTATE;
	else
		padcfg0 &= ~PADCFG0_GPIOTXSTATE;
	writel(padcfg0, reg);
	raw_spin_unlock_irqrestore(&pctrl->lock, flags);
}

static int intel_gpio_get_direction(struct gpio_chip *chip, unsigned int offset)
{
	struct intel_pinctrl *pctrl = gpiochip_get_data(chip);
<<<<<<< HEAD
=======
	unsigned long flags;
>>>>>>> upstream/android-13
	void __iomem *reg;
	u32 padcfg0;
	int pin;

	pin = intel_gpio_to_pin(pctrl, offset, NULL, NULL);
	if (pin < 0)
		return -EINVAL;

	reg = intel_get_padcfg(pctrl, pin, PADCFG0);
	if (!reg)
		return -EINVAL;

<<<<<<< HEAD
	padcfg0 = readl(reg);

	if (padcfg0 & PADCFG0_PMODE_MASK)
		return -EINVAL;

	return !!(padcfg0 & PADCFG0_GPIOTXDIS);
}

static int intel_gpio_direction_input(struct gpio_chip *chip, unsigned offset)
=======
	raw_spin_lock_irqsave(&pctrl->lock, flags);
	padcfg0 = readl(reg);
	raw_spin_unlock_irqrestore(&pctrl->lock, flags);
	if (padcfg0 & PADCFG0_PMODE_MASK)
		return -EINVAL;

	if (padcfg0 & PADCFG0_GPIOTXDIS)
		return GPIO_LINE_DIRECTION_IN;

	return GPIO_LINE_DIRECTION_OUT;
}

static int intel_gpio_direction_input(struct gpio_chip *chip, unsigned int offset)
>>>>>>> upstream/android-13
{
	return pinctrl_gpio_direction_input(chip->base + offset);
}

<<<<<<< HEAD
static int intel_gpio_direction_output(struct gpio_chip *chip, unsigned offset,
=======
static int intel_gpio_direction_output(struct gpio_chip *chip, unsigned int offset,
>>>>>>> upstream/android-13
				       int value)
{
	intel_gpio_set(chip, offset, value);
	return pinctrl_gpio_direction_output(chip->base + offset);
}

static const struct gpio_chip intel_gpio_chip = {
	.owner = THIS_MODULE,
	.request = gpiochip_generic_request,
	.free = gpiochip_generic_free,
	.get_direction = intel_gpio_get_direction,
	.direction_input = intel_gpio_direction_input,
	.direction_output = intel_gpio_direction_output,
	.get = intel_gpio_get,
	.set = intel_gpio_set,
	.set_config = gpiochip_generic_config,
};

static void intel_gpio_irq_ack(struct irq_data *d)
{
	struct gpio_chip *gc = irq_data_get_irq_chip_data(d);
	struct intel_pinctrl *pctrl = gpiochip_get_data(gc);
	const struct intel_community *community;
	const struct intel_padgroup *padgrp;
	int pin;

	pin = intel_gpio_to_pin(pctrl, irqd_to_hwirq(d), &community, &padgrp);
	if (pin >= 0) {
<<<<<<< HEAD
		unsigned gpp, gpp_offset, is_offset;
=======
		unsigned int gpp, gpp_offset, is_offset;
>>>>>>> upstream/android-13

		gpp = padgrp->reg_num;
		gpp_offset = padgroup_offset(padgrp, pin);
		is_offset = community->is_offset + gpp * 4;

		raw_spin_lock(&pctrl->lock);
		writel(BIT(gpp_offset), community->regs + is_offset);
		raw_spin_unlock(&pctrl->lock);
	}
}

<<<<<<< HEAD
static void intel_gpio_irq_enable(struct irq_data *d)
{
	struct gpio_chip *gc = irq_data_get_irq_chip_data(d);
	struct intel_pinctrl *pctrl = gpiochip_get_data(gc);
	const struct intel_community *community;
	const struct intel_padgroup *padgrp;
	int pin;

	pin = intel_gpio_to_pin(pctrl, irqd_to_hwirq(d), &community, &padgrp);
	if (pin >= 0) {
		unsigned gpp, gpp_offset, is_offset;
		unsigned long flags;
		u32 value;

		gpp = padgrp->reg_num;
		gpp_offset = padgroup_offset(padgrp, pin);
		is_offset = community->is_offset + gpp * 4;

		raw_spin_lock_irqsave(&pctrl->lock, flags);
		/* Clear interrupt status first to avoid unexpected interrupt */
		writel(BIT(gpp_offset), community->regs + is_offset);

		value = readl(community->regs + community->ie_offset + gpp * 4);
		value |= BIT(gpp_offset);
		writel(value, community->regs + community->ie_offset + gpp * 4);
		raw_spin_unlock_irqrestore(&pctrl->lock, flags);
	}
}

=======
>>>>>>> upstream/android-13
static void intel_gpio_irq_mask_unmask(struct irq_data *d, bool mask)
{
	struct gpio_chip *gc = irq_data_get_irq_chip_data(d);
	struct intel_pinctrl *pctrl = gpiochip_get_data(gc);
	const struct intel_community *community;
	const struct intel_padgroup *padgrp;
	int pin;

	pin = intel_gpio_to_pin(pctrl, irqd_to_hwirq(d), &community, &padgrp);
	if (pin >= 0) {
<<<<<<< HEAD
		unsigned gpp, gpp_offset;
		unsigned long flags;
		void __iomem *reg;
=======
		unsigned int gpp, gpp_offset;
		unsigned long flags;
		void __iomem *reg, *is;
>>>>>>> upstream/android-13
		u32 value;

		gpp = padgrp->reg_num;
		gpp_offset = padgroup_offset(padgrp, pin);

		reg = community->regs + community->ie_offset + gpp * 4;
<<<<<<< HEAD

		raw_spin_lock_irqsave(&pctrl->lock, flags);
=======
		is = community->regs + community->is_offset + gpp * 4;

		raw_spin_lock_irqsave(&pctrl->lock, flags);

		/* Clear interrupt status first to avoid unexpected interrupt */
		writel(BIT(gpp_offset), is);

>>>>>>> upstream/android-13
		value = readl(reg);
		if (mask)
			value &= ~BIT(gpp_offset);
		else
			value |= BIT(gpp_offset);
		writel(value, reg);
		raw_spin_unlock_irqrestore(&pctrl->lock, flags);
	}
}

static void intel_gpio_irq_mask(struct irq_data *d)
{
	intel_gpio_irq_mask_unmask(d, true);
}

static void intel_gpio_irq_unmask(struct irq_data *d)
{
	intel_gpio_irq_mask_unmask(d, false);
}

<<<<<<< HEAD
static int intel_gpio_irq_type(struct irq_data *d, unsigned type)
{
	struct gpio_chip *gc = irq_data_get_irq_chip_data(d);
	struct intel_pinctrl *pctrl = gpiochip_get_data(gc);
	unsigned pin = intel_gpio_to_pin(pctrl, irqd_to_hwirq(d), NULL, NULL);
=======
static int intel_gpio_irq_type(struct irq_data *d, unsigned int type)
{
	struct gpio_chip *gc = irq_data_get_irq_chip_data(d);
	struct intel_pinctrl *pctrl = gpiochip_get_data(gc);
	unsigned int pin = intel_gpio_to_pin(pctrl, irqd_to_hwirq(d), NULL, NULL);
>>>>>>> upstream/android-13
	unsigned long flags;
	void __iomem *reg;
	u32 value;

	reg = intel_get_padcfg(pctrl, pin, PADCFG0);
	if (!reg)
		return -EINVAL;

	/*
	 * If the pin is in ACPI mode it is still usable as a GPIO but it
	 * cannot be used as IRQ because GPI_IS status bit will not be
	 * updated by the host controller hardware.
	 */
	if (intel_pad_acpi_mode(pctrl, pin)) {
		dev_warn(pctrl->dev, "pin %u cannot be used as IRQ\n", pin);
		return -EPERM;
	}

	raw_spin_lock_irqsave(&pctrl->lock, flags);

	intel_gpio_set_gpio_mode(reg);

	value = readl(reg);

	value &= ~(PADCFG0_RXEVCFG_MASK | PADCFG0_RXINV);

	if ((type & IRQ_TYPE_EDGE_BOTH) == IRQ_TYPE_EDGE_BOTH) {
		value |= PADCFG0_RXEVCFG_EDGE_BOTH << PADCFG0_RXEVCFG_SHIFT;
	} else if (type & IRQ_TYPE_EDGE_FALLING) {
		value |= PADCFG0_RXEVCFG_EDGE << PADCFG0_RXEVCFG_SHIFT;
		value |= PADCFG0_RXINV;
	} else if (type & IRQ_TYPE_EDGE_RISING) {
		value |= PADCFG0_RXEVCFG_EDGE << PADCFG0_RXEVCFG_SHIFT;
	} else if (type & IRQ_TYPE_LEVEL_MASK) {
		if (type & IRQ_TYPE_LEVEL_LOW)
			value |= PADCFG0_RXINV;
	} else {
		value |= PADCFG0_RXEVCFG_DISABLED << PADCFG0_RXEVCFG_SHIFT;
	}

	writel(value, reg);

	if (type & IRQ_TYPE_EDGE_BOTH)
		irq_set_handler_locked(d, handle_edge_irq);
	else if (type & IRQ_TYPE_LEVEL_MASK)
		irq_set_handler_locked(d, handle_level_irq);

	raw_spin_unlock_irqrestore(&pctrl->lock, flags);

	return 0;
}

static int intel_gpio_irq_wake(struct irq_data *d, unsigned int on)
{
	struct gpio_chip *gc = irq_data_get_irq_chip_data(d);
	struct intel_pinctrl *pctrl = gpiochip_get_data(gc);
<<<<<<< HEAD
	unsigned pin = intel_gpio_to_pin(pctrl, irqd_to_hwirq(d), NULL, NULL);
=======
	unsigned int pin = intel_gpio_to_pin(pctrl, irqd_to_hwirq(d), NULL, NULL);
>>>>>>> upstream/android-13

	if (on)
		enable_irq_wake(pctrl->irq);
	else
		disable_irq_wake(pctrl->irq);

	dev_dbg(pctrl->dev, "%sable wake for pin %u\n", on ? "en" : "dis", pin);
	return 0;
}

<<<<<<< HEAD
static irqreturn_t intel_gpio_community_irq_handler(struct intel_pinctrl *pctrl,
	const struct intel_community *community)
{
	struct gpio_chip *gc = &pctrl->chip;
	irqreturn_t ret = IRQ_NONE;
	int gpp;
=======
static int intel_gpio_community_irq_handler(struct intel_pinctrl *pctrl,
					    const struct intel_community *community)
{
	struct gpio_chip *gc = &pctrl->chip;
	unsigned int gpp;
	int ret = 0;
>>>>>>> upstream/android-13

	for (gpp = 0; gpp < community->ngpps; gpp++) {
		const struct intel_padgroup *padgrp = &community->gpps[gpp];
		unsigned long pending, enabled, gpp_offset;

<<<<<<< HEAD
=======
		raw_spin_lock(&pctrl->lock);

>>>>>>> upstream/android-13
		pending = readl(community->regs + community->is_offset +
				padgrp->reg_num * 4);
		enabled = readl(community->regs + community->ie_offset +
				padgrp->reg_num * 4);

<<<<<<< HEAD
=======
		raw_spin_unlock(&pctrl->lock);

>>>>>>> upstream/android-13
		/* Only interrupts that are enabled */
		pending &= enabled;

		for_each_set_bit(gpp_offset, &pending, padgrp->size) {
<<<<<<< HEAD
			unsigned irq;
=======
			unsigned int irq;
>>>>>>> upstream/android-13

			irq = irq_find_mapping(gc->irq.domain,
					       padgrp->gpio_base + gpp_offset);
			generic_handle_irq(irq);
<<<<<<< HEAD

			ret |= IRQ_HANDLED;
		}
=======
		}

		ret += pending ? 1 : 0;
>>>>>>> upstream/android-13
	}

	return ret;
}

static irqreturn_t intel_gpio_irq(int irq, void *data)
{
	const struct intel_community *community;
	struct intel_pinctrl *pctrl = data;
<<<<<<< HEAD
	irqreturn_t ret = IRQ_NONE;
	int i;
=======
	unsigned int i;
	int ret = 0;
>>>>>>> upstream/android-13

	/* Need to check all communities for pending interrupts */
	for (i = 0; i < pctrl->ncommunities; i++) {
		community = &pctrl->communities[i];
<<<<<<< HEAD
		ret |= intel_gpio_community_irq_handler(pctrl, community);
	}

	return ret;
}

static struct irq_chip intel_gpio_irqchip = {
	.name = "intel-gpio",
	.irq_enable = intel_gpio_irq_enable,
	.irq_ack = intel_gpio_irq_ack,
	.irq_mask = intel_gpio_irq_mask,
	.irq_unmask = intel_gpio_irq_unmask,
	.irq_set_type = intel_gpio_irq_type,
	.irq_set_wake = intel_gpio_irq_wake,
	.flags = IRQCHIP_MASK_ON_SUSPEND,
};

static int intel_gpio_add_pin_ranges(struct intel_pinctrl *pctrl,
				     const struct intel_community *community)
=======
		ret += intel_gpio_community_irq_handler(pctrl, community);
	}

	return IRQ_RETVAL(ret);
}

static void intel_gpio_irq_init(struct intel_pinctrl *pctrl)
{
	int i;

	for (i = 0; i < pctrl->ncommunities; i++) {
		const struct intel_community *community;
		void __iomem *base;
		unsigned int gpp;

		community = &pctrl->communities[i];
		base = community->regs;

		for (gpp = 0; gpp < community->ngpps; gpp++) {
			/* Mask and clear all interrupts */
			writel(0, base + community->ie_offset + gpp * 4);
			writel(0xffff, base + community->is_offset + gpp * 4);
		}
	}
}

static int intel_gpio_irq_init_hw(struct gpio_chip *gc)
{
	struct intel_pinctrl *pctrl = gpiochip_get_data(gc);

	/*
	 * Make sure the interrupt lines are in a proper state before
	 * further configuration.
	 */
	intel_gpio_irq_init(pctrl);

	return 0;
}

static int intel_gpio_add_community_ranges(struct intel_pinctrl *pctrl,
				const struct intel_community *community)
>>>>>>> upstream/android-13
{
	int ret = 0, i;

	for (i = 0; i < community->ngpps; i++) {
		const struct intel_padgroup *gpp = &community->gpps[i];

<<<<<<< HEAD
		if (gpp->gpio_base < 0)
=======
		if (gpp->gpio_base == INTEL_GPIO_BASE_NOMAP)
>>>>>>> upstream/android-13
			continue;

		ret = gpiochip_add_pin_range(&pctrl->chip, dev_name(pctrl->dev),
					     gpp->gpio_base, gpp->base,
					     gpp->size);
		if (ret)
			return ret;
	}

	return ret;
}

<<<<<<< HEAD
static unsigned intel_gpio_ngpio(const struct intel_pinctrl *pctrl)
{
	const struct intel_community *community;
	unsigned ngpio = 0;
=======
static int intel_gpio_add_pin_ranges(struct gpio_chip *gc)
{
	struct intel_pinctrl *pctrl = gpiochip_get_data(gc);
	int ret, i;

	for (i = 0; i < pctrl->ncommunities; i++) {
		struct intel_community *community = &pctrl->communities[i];

		ret = intel_gpio_add_community_ranges(pctrl, community);
		if (ret) {
			dev_err(pctrl->dev, "failed to add GPIO pin range\n");
			return ret;
		}
	}

	return 0;
}

static unsigned int intel_gpio_ngpio(const struct intel_pinctrl *pctrl)
{
	const struct intel_community *community;
	unsigned int ngpio = 0;
>>>>>>> upstream/android-13
	int i, j;

	for (i = 0; i < pctrl->ncommunities; i++) {
		community = &pctrl->communities[i];
		for (j = 0; j < community->ngpps; j++) {
			const struct intel_padgroup *gpp = &community->gpps[j];

<<<<<<< HEAD
			if (gpp->gpio_base < 0)
=======
			if (gpp->gpio_base == INTEL_GPIO_BASE_NOMAP)
>>>>>>> upstream/android-13
				continue;

			if (gpp->gpio_base + gpp->size > ngpio)
				ngpio = gpp->gpio_base + gpp->size;
		}
	}

	return ngpio;
}

static int intel_gpio_probe(struct intel_pinctrl *pctrl, int irq)
{
<<<<<<< HEAD
	int ret, i;

	pctrl->chip = intel_gpio_chip;

=======
	int ret;
	struct gpio_irq_chip *girq;

	pctrl->chip = intel_gpio_chip;

	/* Setup GPIO chip */
>>>>>>> upstream/android-13
	pctrl->chip.ngpio = intel_gpio_ngpio(pctrl);
	pctrl->chip.label = dev_name(pctrl->dev);
	pctrl->chip.parent = pctrl->dev;
	pctrl->chip.base = -1;
<<<<<<< HEAD
	pctrl->irq = irq;

	ret = devm_gpiochip_add_data(pctrl->dev, &pctrl->chip, pctrl);
	if (ret) {
		dev_err(pctrl->dev, "failed to register gpiochip\n");
		return ret;
	}

	for (i = 0; i < pctrl->ncommunities; i++) {
		struct intel_community *community = &pctrl->communities[i];

		ret = intel_gpio_add_pin_ranges(pctrl, community);
		if (ret) {
			dev_err(pctrl->dev, "failed to add GPIO pin range\n");
			return ret;
		}
	}

	/*
	 * We need to request the interrupt here (instead of providing chip
	 * to the irq directly) because on some platforms several GPIO
	 * controllers share the same interrupt line.
=======
	pctrl->chip.add_pin_ranges = intel_gpio_add_pin_ranges;
	pctrl->irq = irq;

	/* Setup IRQ chip */
	pctrl->irqchip.name = dev_name(pctrl->dev);
	pctrl->irqchip.irq_ack = intel_gpio_irq_ack;
	pctrl->irqchip.irq_mask = intel_gpio_irq_mask;
	pctrl->irqchip.irq_unmask = intel_gpio_irq_unmask;
	pctrl->irqchip.irq_set_type = intel_gpio_irq_type;
	pctrl->irqchip.irq_set_wake = intel_gpio_irq_wake;
	pctrl->irqchip.flags = IRQCHIP_MASK_ON_SUSPEND;

	/*
	 * On some platforms several GPIO controllers share the same interrupt
	 * line.
>>>>>>> upstream/android-13
	 */
	ret = devm_request_irq(pctrl->dev, irq, intel_gpio_irq,
			       IRQF_SHARED | IRQF_NO_THREAD,
			       dev_name(pctrl->dev), pctrl);
	if (ret) {
		dev_err(pctrl->dev, "failed to request interrupt\n");
		return ret;
	}

<<<<<<< HEAD
	ret = gpiochip_irqchip_add(&pctrl->chip, &intel_gpio_irqchip, 0,
				   handle_bad_irq, IRQ_TYPE_NONE);
	if (ret) {
		dev_err(pctrl->dev, "failed to add irqchip\n");
		return ret;
	}

	gpiochip_set_chained_irqchip(&pctrl->chip, &intel_gpio_irqchip, irq,
				     NULL);
	return 0;
}

static int intel_pinctrl_add_padgroups(struct intel_pinctrl *pctrl,
				       struct intel_community *community)
{
	struct intel_padgroup *gpps;
	unsigned npins = community->npins;
	unsigned padown_num = 0;
	size_t ngpps, i;

	if (community->gpps)
		ngpps = community->ngpps;
	else
		ngpps = DIV_ROUND_UP(community->npins, community->gpp_size);
=======
	girq = &pctrl->chip.irq;
	girq->chip = &pctrl->irqchip;
	/* This will let us handle the IRQ in the driver */
	girq->parent_handler = NULL;
	girq->num_parents = 0;
	girq->default_type = IRQ_TYPE_NONE;
	girq->handler = handle_bad_irq;
	girq->init_hw = intel_gpio_irq_init_hw;

	ret = devm_gpiochip_add_data(pctrl->dev, &pctrl->chip, pctrl);
	if (ret) {
		dev_err(pctrl->dev, "failed to register gpiochip\n");
		return ret;
	}

	return 0;
}

static int intel_pinctrl_add_padgroups_by_gpps(struct intel_pinctrl *pctrl,
					       struct intel_community *community)
{
	struct intel_padgroup *gpps;
	unsigned int padown_num = 0;
	size_t i, ngpps = community->ngpps;
>>>>>>> upstream/android-13

	gpps = devm_kcalloc(pctrl->dev, ngpps, sizeof(*gpps), GFP_KERNEL);
	if (!gpps)
		return -ENOMEM;

	for (i = 0; i < ngpps; i++) {
<<<<<<< HEAD
		if (community->gpps) {
			gpps[i] = community->gpps[i];
		} else {
			unsigned gpp_size = community->gpp_size;

			gpps[i].reg_num = i;
			gpps[i].base = community->pin_base + i * gpp_size;
			gpps[i].size = min(gpp_size, npins);
			npins -= gpps[i].size;
		}
=======
		gpps[i] = community->gpps[i];
>>>>>>> upstream/android-13

		if (gpps[i].size > 32)
			return -EINVAL;

<<<<<<< HEAD
		if (!gpps[i].gpio_base)
			gpps[i].gpio_base = gpps[i].base;

		gpps[i].padown_num = padown_num;
=======
		/* Special treatment for GPIO base */
		switch (gpps[i].gpio_base) {
			case INTEL_GPIO_BASE_MATCH:
				gpps[i].gpio_base = gpps[i].base;
				break;
			case INTEL_GPIO_BASE_ZERO:
				gpps[i].gpio_base = 0;
				break;
			case INTEL_GPIO_BASE_NOMAP:
				break;
			default:
				break;
		}

		gpps[i].padown_num = padown_num;
		padown_num += DIV_ROUND_UP(gpps[i].size * 4, 32);
	}

	community->gpps = gpps;

	return 0;
}

static int intel_pinctrl_add_padgroups_by_size(struct intel_pinctrl *pctrl,
					       struct intel_community *community)
{
	struct intel_padgroup *gpps;
	unsigned int npins = community->npins;
	unsigned int padown_num = 0;
	size_t i, ngpps = DIV_ROUND_UP(npins, community->gpp_size);

	if (community->gpp_size > 32)
		return -EINVAL;

	gpps = devm_kcalloc(pctrl->dev, ngpps, sizeof(*gpps), GFP_KERNEL);
	if (!gpps)
		return -ENOMEM;

	for (i = 0; i < ngpps; i++) {
		unsigned int gpp_size = community->gpp_size;

		gpps[i].reg_num = i;
		gpps[i].base = community->pin_base + i * gpp_size;
		gpps[i].size = min(gpp_size, npins);
		npins -= gpps[i].size;

		gpps[i].gpio_base = gpps[i].base;
		gpps[i].padown_num = padown_num;
>>>>>>> upstream/android-13

		/*
		 * In older hardware the number of padown registers per
		 * group is fixed regardless of the group size.
		 */
		if (community->gpp_num_padown_regs)
			padown_num += community->gpp_num_padown_regs;
		else
			padown_num += DIV_ROUND_UP(gpps[i].size * 4, 32);
	}

	community->ngpps = ngpps;
	community->gpps = gpps;

	return 0;
}

static int intel_pinctrl_pm_init(struct intel_pinctrl *pctrl)
{
#ifdef CONFIG_PM_SLEEP
	const struct intel_pinctrl_soc_data *soc = pctrl->soc;
	struct intel_community_context *communities;
	struct intel_pad_context *pads;
	int i;

	pads = devm_kcalloc(pctrl->dev, soc->npins, sizeof(*pads), GFP_KERNEL);
	if (!pads)
		return -ENOMEM;

	communities = devm_kcalloc(pctrl->dev, pctrl->ncommunities,
				   sizeof(*communities), GFP_KERNEL);
	if (!communities)
		return -ENOMEM;


	for (i = 0; i < pctrl->ncommunities; i++) {
		struct intel_community *community = &pctrl->communities[i];
<<<<<<< HEAD
		u32 *intmask;
=======
		u32 *intmask, *hostown;
>>>>>>> upstream/android-13

		intmask = devm_kcalloc(pctrl->dev, community->ngpps,
				       sizeof(*intmask), GFP_KERNEL);
		if (!intmask)
			return -ENOMEM;

		communities[i].intmask = intmask;
<<<<<<< HEAD
=======

		hostown = devm_kcalloc(pctrl->dev, community->ngpps,
				       sizeof(*hostown), GFP_KERNEL);
		if (!hostown)
			return -ENOMEM;

		communities[i].hostown = hostown;
>>>>>>> upstream/android-13
	}

	pctrl->context.pads = pads;
	pctrl->context.communities = communities;
#endif

	return 0;
}

<<<<<<< HEAD
int intel_pinctrl_probe(struct platform_device *pdev,
			const struct intel_pinctrl_soc_data *soc_data)
=======
static int intel_pinctrl_probe(struct platform_device *pdev,
			       const struct intel_pinctrl_soc_data *soc_data)
>>>>>>> upstream/android-13
{
	struct intel_pinctrl *pctrl;
	int i, ret, irq;

<<<<<<< HEAD
	if (!soc_data)
		return -EINVAL;

=======
>>>>>>> upstream/android-13
	pctrl = devm_kzalloc(&pdev->dev, sizeof(*pctrl), GFP_KERNEL);
	if (!pctrl)
		return -ENOMEM;

	pctrl->dev = &pdev->dev;
	pctrl->soc = soc_data;
	raw_spin_lock_init(&pctrl->lock);

	/*
	 * Make a copy of the communities which we can use to hold pointers
	 * to the registers.
	 */
	pctrl->ncommunities = pctrl->soc->ncommunities;
	pctrl->communities = devm_kcalloc(&pdev->dev, pctrl->ncommunities,
				  sizeof(*pctrl->communities), GFP_KERNEL);
	if (!pctrl->communities)
		return -ENOMEM;

	for (i = 0; i < pctrl->ncommunities; i++) {
		struct intel_community *community = &pctrl->communities[i];
<<<<<<< HEAD
		struct resource *res;
		void __iomem *regs;
		u32 padbar;

		*community = pctrl->soc->communities[i];

		res = platform_get_resource(pdev, IORESOURCE_MEM,
					    community->barno);
		regs = devm_ioremap_resource(&pdev->dev, res);
=======
		void __iomem *regs;
		u32 offset;
		u32 value;

		*community = pctrl->soc->communities[i];

		regs = devm_platform_ioremap_resource(pdev, community->barno);
>>>>>>> upstream/android-13
		if (IS_ERR(regs))
			return PTR_ERR(regs);

		/*
<<<<<<< HEAD
		 * Determine community features based on the revision if
		 * not specified already.
		 */
		if (!community->features) {
			u32 rev;

			rev = (readl(regs + REVID) & REVID_MASK) >> REVID_SHIFT;
			if (rev >= 0x94) {
				community->features |= PINCTRL_FEATURE_DEBOUNCE;
				community->features |= PINCTRL_FEATURE_1K_PD;
			}
		}

		/* Read offset of the pad configuration registers */
		padbar = readl(regs + PADBAR);

		community->regs = regs;
		community->pad_regs = regs + padbar;

		if (!community->is_offset)
			community->is_offset = GPI_IS;

		ret = intel_pinctrl_add_padgroups(pctrl, community);
=======
		 * Determine community features based on the revision.
		 * A value of all ones means the device is not present.
		 */
		value = readl(regs + REVID);
		if (value == ~0u)
			return -ENODEV;
		if (((value & REVID_MASK) >> REVID_SHIFT) >= 0x94) {
			community->features |= PINCTRL_FEATURE_DEBOUNCE;
			community->features |= PINCTRL_FEATURE_1K_PD;
		}

		/* Determine community features based on the capabilities */
		offset = CAPLIST;
		do {
			value = readl(regs + offset);
			switch ((value & CAPLIST_ID_MASK) >> CAPLIST_ID_SHIFT) {
			case CAPLIST_ID_GPIO_HW_INFO:
				community->features |= PINCTRL_FEATURE_GPIO_HW_INFO;
				break;
			case CAPLIST_ID_PWM:
				community->features |= PINCTRL_FEATURE_PWM;
				break;
			case CAPLIST_ID_BLINK:
				community->features |= PINCTRL_FEATURE_BLINK;
				break;
			case CAPLIST_ID_EXP:
				community->features |= PINCTRL_FEATURE_EXP;
				break;
			default:
				break;
			}
			offset = (value & CAPLIST_NEXT_MASK) >> CAPLIST_NEXT_SHIFT;
		} while (offset);

		dev_dbg(&pdev->dev, "Community%d features: %#08x\n", i, community->features);

		/* Read offset of the pad configuration registers */
		offset = readl(regs + PADBAR);

		community->regs = regs;
		community->pad_regs = regs + offset;

		if (community->gpps)
			ret = intel_pinctrl_add_padgroups_by_gpps(pctrl, community);
		else
			ret = intel_pinctrl_add_padgroups_by_size(pctrl, community);
>>>>>>> upstream/android-13
		if (ret)
			return ret;
	}

	irq = platform_get_irq(pdev, 0);
<<<<<<< HEAD
	if (irq < 0) {
		dev_err(&pdev->dev, "failed to get interrupt number\n");
		return irq;
	}
=======
	if (irq < 0)
		return irq;
>>>>>>> upstream/android-13

	ret = intel_pinctrl_pm_init(pctrl);
	if (ret)
		return ret;

	pctrl->pctldesc = intel_pinctrl_desc;
	pctrl->pctldesc.name = dev_name(&pdev->dev);
	pctrl->pctldesc.pins = pctrl->soc->pins;
	pctrl->pctldesc.npins = pctrl->soc->npins;

	pctrl->pctldev = devm_pinctrl_register(&pdev->dev, &pctrl->pctldesc,
					       pctrl);
	if (IS_ERR(pctrl->pctldev)) {
		dev_err(&pdev->dev, "failed to register pinctrl driver\n");
		return PTR_ERR(pctrl->pctldev);
	}

	ret = intel_gpio_probe(pctrl, irq);
	if (ret)
		return ret;

	platform_set_drvdata(pdev, pctrl);

	return 0;
}
<<<<<<< HEAD
EXPORT_SYMBOL_GPL(intel_pinctrl_probe);

#ifdef CONFIG_PM_SLEEP
static bool intel_pinctrl_should_save(struct intel_pinctrl *pctrl, unsigned pin)
=======

int intel_pinctrl_probe_by_hid(struct platform_device *pdev)
{
	const struct intel_pinctrl_soc_data *data;

	data = device_get_match_data(&pdev->dev);
	if (!data)
		return -ENODATA;

	return intel_pinctrl_probe(pdev, data);
}
EXPORT_SYMBOL_GPL(intel_pinctrl_probe_by_hid);

int intel_pinctrl_probe_by_uid(struct platform_device *pdev)
{
	const struct intel_pinctrl_soc_data *data;

	data = intel_pinctrl_get_soc_data(pdev);
	if (IS_ERR(data))
		return PTR_ERR(data);

	return intel_pinctrl_probe(pdev, data);
}
EXPORT_SYMBOL_GPL(intel_pinctrl_probe_by_uid);

const struct intel_pinctrl_soc_data *intel_pinctrl_get_soc_data(struct platform_device *pdev)
{
	const struct intel_pinctrl_soc_data *data = NULL;
	const struct intel_pinctrl_soc_data **table;
	struct acpi_device *adev;
	unsigned int i;

	adev = ACPI_COMPANION(&pdev->dev);
	if (adev) {
		const void *match = device_get_match_data(&pdev->dev);

		table = (const struct intel_pinctrl_soc_data **)match;
		for (i = 0; table[i]; i++) {
			if (!strcmp(adev->pnp.unique_id, table[i]->uid)) {
				data = table[i];
				break;
			}
		}
	} else {
		const struct platform_device_id *id;

		id = platform_get_device_id(pdev);
		if (!id)
			return ERR_PTR(-ENODEV);

		table = (const struct intel_pinctrl_soc_data **)id->driver_data;
		data = table[pdev->id];
	}

	return data ?: ERR_PTR(-ENODATA);
}
EXPORT_SYMBOL_GPL(intel_pinctrl_get_soc_data);

#ifdef CONFIG_PM_SLEEP
static bool intel_pinctrl_should_save(struct intel_pinctrl *pctrl, unsigned int pin)
>>>>>>> upstream/android-13
{
	const struct pin_desc *pd = pin_desc_get(pctrl->pctldev, pin);

	if (!pd || !intel_pad_usable(pctrl, pin))
		return false;

	/*
	 * Only restore the pin if it is actually in use by the kernel (or
	 * by userspace). It is possible that some pins are used by the
	 * BIOS during resume and those are not always locked down so leave
	 * them alone.
	 */
	if (pd->mux_owner || pd->gpio_owner ||
<<<<<<< HEAD
	    gpiochip_line_is_irq(&pctrl->chip, pin))
=======
	    gpiochip_line_is_irq(&pctrl->chip, intel_pin_to_gpio(pctrl, pin)))
>>>>>>> upstream/android-13
		return true;

	return false;
}

<<<<<<< HEAD
int intel_pinctrl_suspend(struct device *dev)
{
	struct platform_device *pdev = to_platform_device(dev);
	struct intel_pinctrl *pctrl = platform_get_drvdata(pdev);
=======
int intel_pinctrl_suspend_noirq(struct device *dev)
{
	struct intel_pinctrl *pctrl = dev_get_drvdata(dev);
>>>>>>> upstream/android-13
	struct intel_community_context *communities;
	struct intel_pad_context *pads;
	int i;

	pads = pctrl->context.pads;
	for (i = 0; i < pctrl->soc->npins; i++) {
		const struct pinctrl_pin_desc *desc = &pctrl->soc->pins[i];
		void __iomem *padcfg;
		u32 val;

		if (!intel_pinctrl_should_save(pctrl, desc->number))
			continue;

		val = readl(intel_get_padcfg(pctrl, desc->number, PADCFG0));
		pads[i].padcfg0 = val & ~PADCFG0_GPIORXSTATE;
		val = readl(intel_get_padcfg(pctrl, desc->number, PADCFG1));
		pads[i].padcfg1 = val;

		padcfg = intel_get_padcfg(pctrl, desc->number, PADCFG2);
		if (padcfg)
			pads[i].padcfg2 = readl(padcfg);
	}

	communities = pctrl->context.communities;
	for (i = 0; i < pctrl->ncommunities; i++) {
		struct intel_community *community = &pctrl->communities[i];
		void __iomem *base;
<<<<<<< HEAD
		unsigned gpp;
=======
		unsigned int gpp;
>>>>>>> upstream/android-13

		base = community->regs + community->ie_offset;
		for (gpp = 0; gpp < community->ngpps; gpp++)
			communities[i].intmask[gpp] = readl(base + gpp * 4);
<<<<<<< HEAD
=======

		base = community->regs + community->hostown_offset;
		for (gpp = 0; gpp < community->ngpps; gpp++)
			communities[i].hostown[gpp] = readl(base + gpp * 4);
>>>>>>> upstream/android-13
	}

	return 0;
}
<<<<<<< HEAD
EXPORT_SYMBOL_GPL(intel_pinctrl_suspend);

static void intel_gpio_irq_init(struct intel_pinctrl *pctrl)
{
	size_t i;

	for (i = 0; i < pctrl->ncommunities; i++) {
		const struct intel_community *community;
		void __iomem *base;
		unsigned gpp;

		community = &pctrl->communities[i];
		base = community->regs;

		for (gpp = 0; gpp < community->ngpps; gpp++) {
			/* Mask and clear all interrupts */
			writel(0, base + community->ie_offset + gpp * 4);
			writel(0xffff, base + community->is_offset + gpp * 4);
		}
	}
}

int intel_pinctrl_resume(struct device *dev)
{
	struct platform_device *pdev = to_platform_device(dev);
	struct intel_pinctrl *pctrl = platform_get_drvdata(pdev);
=======
EXPORT_SYMBOL_GPL(intel_pinctrl_suspend_noirq);

static bool intel_gpio_update_reg(void __iomem *reg, u32 mask, u32 value)
{
	u32 curr, updated;

	curr = readl(reg);

	updated = (curr & ~mask) | (value & mask);
	if (curr == updated)
		return false;

	writel(updated, reg);
	return true;
}

static void intel_restore_hostown(struct intel_pinctrl *pctrl, unsigned int c,
				  void __iomem *base, unsigned int gpp, u32 saved)
{
	const struct intel_community *community = &pctrl->communities[c];
	const struct intel_padgroup *padgrp = &community->gpps[gpp];
	struct device *dev = pctrl->dev;
	const char *dummy;
	u32 requested = 0;
	unsigned int i;

	if (padgrp->gpio_base == INTEL_GPIO_BASE_NOMAP)
		return;

	for_each_requested_gpio_in_range(&pctrl->chip, i, padgrp->gpio_base, padgrp->size, dummy)
		requested |= BIT(i);

	if (!intel_gpio_update_reg(base + gpp * 4, requested, saved))
		return;

	dev_dbg(dev, "restored hostown %u/%u %#08x\n", c, gpp, readl(base + gpp * 4));
}

static void intel_restore_intmask(struct intel_pinctrl *pctrl, unsigned int c,
				  void __iomem *base, unsigned int gpp, u32 saved)
{
	struct device *dev = pctrl->dev;

	if (!intel_gpio_update_reg(base + gpp * 4, ~0U, saved))
		return;

	dev_dbg(dev, "restored mask %u/%u %#08x\n", c, gpp, readl(base + gpp * 4));
}

static void intel_restore_padcfg(struct intel_pinctrl *pctrl, unsigned int pin,
				 unsigned int reg, u32 saved)
{
	u32 mask = (reg == PADCFG0) ? PADCFG0_GPIORXSTATE : 0;
	unsigned int n = reg / sizeof(u32);
	struct device *dev = pctrl->dev;
	void __iomem *padcfg;

	padcfg = intel_get_padcfg(pctrl, pin, reg);
	if (!padcfg)
		return;

	if (!intel_gpio_update_reg(padcfg, ~mask, saved))
		return;

	dev_dbg(dev, "restored pin %u padcfg%u %#08x\n", pin, n, readl(padcfg));
}

int intel_pinctrl_resume_noirq(struct device *dev)
{
	struct intel_pinctrl *pctrl = dev_get_drvdata(dev);
>>>>>>> upstream/android-13
	const struct intel_community_context *communities;
	const struct intel_pad_context *pads;
	int i;

	/* Mask all interrupts */
	intel_gpio_irq_init(pctrl);

	pads = pctrl->context.pads;
	for (i = 0; i < pctrl->soc->npins; i++) {
		const struct pinctrl_pin_desc *desc = &pctrl->soc->pins[i];
<<<<<<< HEAD
		void __iomem *padcfg;
		u32 val;
=======
>>>>>>> upstream/android-13

		if (!intel_pinctrl_should_save(pctrl, desc->number))
			continue;

<<<<<<< HEAD
		padcfg = intel_get_padcfg(pctrl, desc->number, PADCFG0);
		val = readl(padcfg) & ~PADCFG0_GPIORXSTATE;
		if (val != pads[i].padcfg0) {
			writel(pads[i].padcfg0, padcfg);
			dev_dbg(dev, "restored pin %u padcfg0 %#08x\n",
				desc->number, readl(padcfg));
		}

		padcfg = intel_get_padcfg(pctrl, desc->number, PADCFG1);
		val = readl(padcfg);
		if (val != pads[i].padcfg1) {
			writel(pads[i].padcfg1, padcfg);
			dev_dbg(dev, "restored pin %u padcfg1 %#08x\n",
				desc->number, readl(padcfg));
		}

		padcfg = intel_get_padcfg(pctrl, desc->number, PADCFG2);
		if (padcfg) {
			val = readl(padcfg);
			if (val != pads[i].padcfg2) {
				writel(pads[i].padcfg2, padcfg);
				dev_dbg(dev, "restored pin %u padcfg2 %#08x\n",
					desc->number, readl(padcfg));
			}
		}
=======
		intel_restore_padcfg(pctrl, desc->number, PADCFG0, pads[i].padcfg0);
		intel_restore_padcfg(pctrl, desc->number, PADCFG1, pads[i].padcfg1);
		intel_restore_padcfg(pctrl, desc->number, PADCFG2, pads[i].padcfg2);
>>>>>>> upstream/android-13
	}

	communities = pctrl->context.communities;
	for (i = 0; i < pctrl->ncommunities; i++) {
		struct intel_community *community = &pctrl->communities[i];
		void __iomem *base;
<<<<<<< HEAD
		unsigned gpp;

		base = community->regs + community->ie_offset;
		for (gpp = 0; gpp < community->ngpps; gpp++) {
			writel(communities[i].intmask[gpp], base + gpp * 4);
			dev_dbg(dev, "restored mask %d/%u %#08x\n", i, gpp,
				readl(base + gpp * 4));
		}
=======
		unsigned int gpp;

		base = community->regs + community->ie_offset;
		for (gpp = 0; gpp < community->ngpps; gpp++)
			intel_restore_intmask(pctrl, i, base, gpp, communities[i].intmask[gpp]);

		base = community->regs + community->hostown_offset;
		for (gpp = 0; gpp < community->ngpps; gpp++)
			intel_restore_hostown(pctrl, i, base, gpp, communities[i].hostown[gpp]);
>>>>>>> upstream/android-13
	}

	return 0;
}
<<<<<<< HEAD
EXPORT_SYMBOL_GPL(intel_pinctrl_resume);
=======
EXPORT_SYMBOL_GPL(intel_pinctrl_resume_noirq);
>>>>>>> upstream/android-13
#endif

MODULE_AUTHOR("Mathias Nyman <mathias.nyman@linux.intel.com>");
MODULE_AUTHOR("Mika Westerberg <mika.westerberg@linux.intel.com>");
MODULE_DESCRIPTION("Intel pinctrl/GPIO core driver");
MODULE_LICENSE("GPL v2");
