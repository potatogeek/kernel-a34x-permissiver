<<<<<<< HEAD
/*
 * intel_soc_pmic_crc.c - Device access for Crystal Cove PMIC
 *
 * Copyright (C) 2013, 2014 Intel Corporation. All rights reserved.
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
// SPDX-License-Identifier: GPL-2.0
/*
 * Device access for Crystal Cove PMIC
 *
 * Copyright (C) 2013, 2014 Intel Corporation. All rights reserved.
 *
>>>>>>> upstream/android-13
 * Author: Yang, Bin <bin.yang@intel.com>
 * Author: Zhu, Lejun <lejun.zhu@linux.intel.com>
 */

<<<<<<< HEAD
#include <linux/mfd/core.h>
#include <linux/interrupt.h>
#include <linux/regmap.h>
#include <linux/mfd/intel_soc_pmic.h>
=======
#include <linux/interrupt.h>
#include <linux/regmap.h>
#include <linux/mfd/core.h>
#include <linux/mfd/intel_soc_pmic.h>

>>>>>>> upstream/android-13
#include "intel_soc_pmic_core.h"

#define CRYSTAL_COVE_MAX_REGISTER	0xC6

#define CRYSTAL_COVE_REG_IRQLVL1	0x02
#define CRYSTAL_COVE_REG_MIRQLVL1	0x0E

#define CRYSTAL_COVE_IRQ_PWRSRC		0
#define CRYSTAL_COVE_IRQ_THRM		1
#define CRYSTAL_COVE_IRQ_BCU		2
#define CRYSTAL_COVE_IRQ_ADC		3
#define CRYSTAL_COVE_IRQ_CHGR		4
#define CRYSTAL_COVE_IRQ_GPIO		5
#define CRYSTAL_COVE_IRQ_VHDMIOCP	6

<<<<<<< HEAD
static struct resource gpio_resources[] = {
	{
		.name	= "GPIO",
		.start	= CRYSTAL_COVE_IRQ_GPIO,
		.end	= CRYSTAL_COVE_IRQ_GPIO,
		.flags	= IORESOURCE_IRQ,
	},
};

static struct resource pwrsrc_resources[] = {
	{
		.name  = "PWRSRC",
		.start = CRYSTAL_COVE_IRQ_PWRSRC,
		.end   = CRYSTAL_COVE_IRQ_PWRSRC,
		.flags = IORESOURCE_IRQ,
	},
};

static struct resource adc_resources[] = {
	{
		.name  = "ADC",
		.start = CRYSTAL_COVE_IRQ_ADC,
		.end   = CRYSTAL_COVE_IRQ_ADC,
		.flags = IORESOURCE_IRQ,
	},
};

static struct resource thermal_resources[] = {
	{
		.name  = "THERMAL",
		.start = CRYSTAL_COVE_IRQ_THRM,
		.end   = CRYSTAL_COVE_IRQ_THRM,
		.flags = IORESOURCE_IRQ,
	},
};

static struct resource bcu_resources[] = {
	{
		.name  = "BCU",
		.start = CRYSTAL_COVE_IRQ_BCU,
		.end   = CRYSTAL_COVE_IRQ_BCU,
		.flags = IORESOURCE_IRQ,
	},
=======
static const struct resource gpio_resources[] = {
	DEFINE_RES_IRQ_NAMED(CRYSTAL_COVE_IRQ_GPIO, "GPIO"),
};

static const struct resource pwrsrc_resources[] = {
	DEFINE_RES_IRQ_NAMED(CRYSTAL_COVE_IRQ_PWRSRC, "PWRSRC"),
};

static const struct resource adc_resources[] = {
	DEFINE_RES_IRQ_NAMED(CRYSTAL_COVE_IRQ_ADC, "ADC"),
};

static const struct resource thermal_resources[] = {
	DEFINE_RES_IRQ_NAMED(CRYSTAL_COVE_IRQ_THRM, "THERMAL"),
};

static const struct resource bcu_resources[] = {
	DEFINE_RES_IRQ_NAMED(CRYSTAL_COVE_IRQ_BCU, "BCU"),
>>>>>>> upstream/android-13
};

static struct mfd_cell crystal_cove_byt_dev[] = {
	{
		.name = "crystal_cove_pwrsrc",
		.num_resources = ARRAY_SIZE(pwrsrc_resources),
		.resources = pwrsrc_resources,
	},
	{
		.name = "crystal_cove_adc",
		.num_resources = ARRAY_SIZE(adc_resources),
		.resources = adc_resources,
	},
	{
		.name = "crystal_cove_thermal",
		.num_resources = ARRAY_SIZE(thermal_resources),
		.resources = thermal_resources,
	},
	{
		.name = "crystal_cove_bcu",
		.num_resources = ARRAY_SIZE(bcu_resources),
		.resources = bcu_resources,
	},
	{
		.name = "crystal_cove_gpio",
		.num_resources = ARRAY_SIZE(gpio_resources),
		.resources = gpio_resources,
	},
	{
<<<<<<< HEAD
		.name = "crystal_cove_pmic",
=======
		.name = "byt_crystal_cove_pmic",
>>>>>>> upstream/android-13
	},
	{
		.name = "crystal_cove_pwm",
	},
};

static struct mfd_cell crystal_cove_cht_dev[] = {
	{
		.name = "crystal_cove_gpio",
		.num_resources = ARRAY_SIZE(gpio_resources),
		.resources = gpio_resources,
	},
	{
<<<<<<< HEAD
=======
		.name = "cht_crystal_cove_pmic",
	},
	{
>>>>>>> upstream/android-13
		.name = "crystal_cove_pwm",
	},
};

static const struct regmap_config crystal_cove_regmap_config = {
	.reg_bits = 8,
	.val_bits = 8,

	.max_register = CRYSTAL_COVE_MAX_REGISTER,
	.cache_type = REGCACHE_NONE,
};

static const struct regmap_irq crystal_cove_irqs[] = {
<<<<<<< HEAD
	[CRYSTAL_COVE_IRQ_PWRSRC] = {
		.mask = BIT(CRYSTAL_COVE_IRQ_PWRSRC),
	},
	[CRYSTAL_COVE_IRQ_THRM] = {
		.mask = BIT(CRYSTAL_COVE_IRQ_THRM),
	},
	[CRYSTAL_COVE_IRQ_BCU] = {
		.mask = BIT(CRYSTAL_COVE_IRQ_BCU),
	},
	[CRYSTAL_COVE_IRQ_ADC] = {
		.mask = BIT(CRYSTAL_COVE_IRQ_ADC),
	},
	[CRYSTAL_COVE_IRQ_CHGR] = {
		.mask = BIT(CRYSTAL_COVE_IRQ_CHGR),
	},
	[CRYSTAL_COVE_IRQ_GPIO] = {
		.mask = BIT(CRYSTAL_COVE_IRQ_GPIO),
	},
	[CRYSTAL_COVE_IRQ_VHDMIOCP] = {
		.mask = BIT(CRYSTAL_COVE_IRQ_VHDMIOCP),
	},
=======
	REGMAP_IRQ_REG(CRYSTAL_COVE_IRQ_PWRSRC, 0, BIT(CRYSTAL_COVE_IRQ_PWRSRC)),
	REGMAP_IRQ_REG(CRYSTAL_COVE_IRQ_THRM, 0, BIT(CRYSTAL_COVE_IRQ_THRM)),
	REGMAP_IRQ_REG(CRYSTAL_COVE_IRQ_BCU, 0, BIT(CRYSTAL_COVE_IRQ_BCU)),
	REGMAP_IRQ_REG(CRYSTAL_COVE_IRQ_ADC, 0, BIT(CRYSTAL_COVE_IRQ_ADC)),
	REGMAP_IRQ_REG(CRYSTAL_COVE_IRQ_CHGR, 0, BIT(CRYSTAL_COVE_IRQ_CHGR)),
	REGMAP_IRQ_REG(CRYSTAL_COVE_IRQ_GPIO, 0, BIT(CRYSTAL_COVE_IRQ_GPIO)),
	REGMAP_IRQ_REG(CRYSTAL_COVE_IRQ_VHDMIOCP, 0, BIT(CRYSTAL_COVE_IRQ_VHDMIOCP)),
>>>>>>> upstream/android-13
};

static const struct regmap_irq_chip crystal_cove_irq_chip = {
	.name = "Crystal Cove",
	.irqs = crystal_cove_irqs,
	.num_irqs = ARRAY_SIZE(crystal_cove_irqs),
	.num_regs = 1,
	.status_base = CRYSTAL_COVE_REG_IRQLVL1,
	.mask_base = CRYSTAL_COVE_REG_MIRQLVL1,
};

struct intel_soc_pmic_config intel_soc_pmic_config_byt_crc = {
	.irq_flags = IRQF_TRIGGER_RISING,
	.cell_dev = crystal_cove_byt_dev,
	.n_cell_devs = ARRAY_SIZE(crystal_cove_byt_dev),
	.regmap_config = &crystal_cove_regmap_config,
	.irq_chip = &crystal_cove_irq_chip,
};

struct intel_soc_pmic_config intel_soc_pmic_config_cht_crc = {
	.irq_flags = IRQF_TRIGGER_RISING,
	.cell_dev = crystal_cove_cht_dev,
	.n_cell_devs = ARRAY_SIZE(crystal_cove_cht_dev),
	.regmap_config = &crystal_cove_regmap_config,
	.irq_chip = &crystal_cove_irq_chip,
};
