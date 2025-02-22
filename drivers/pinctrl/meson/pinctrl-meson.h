<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> upstream/android-13
/*
 * Pin controller and GPIO driver for Amlogic Meson SoCs
 *
 * Copyright (C) 2014 Beniamino Galvani <b.galvani@gmail.com>
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <linux/gpio.h>
=======
 */

#include <linux/gpio/driver.h>
>>>>>>> upstream/android-13
#include <linux/pinctrl/pinctrl.h>
#include <linux/platform_device.h>
#include <linux/regmap.h>
#include <linux/types.h>
<<<<<<< HEAD
=======
#include <linux/module.h>

struct meson_pinctrl;
>>>>>>> upstream/android-13

/**
 * struct meson_pmx_group - a pinmux group
 *
 * @name:	group name
 * @pins:	pins in the group
 * @num_pins:	number of pins in the group
 * @is_gpio:	whether the group is a single GPIO group
 * @reg:	register offset for the group in the domain mux registers
 * @bit		bit index enabling the group
 * @domain:	index of the domain this group belongs to
 */
struct meson_pmx_group {
	const char *name;
	const unsigned int *pins;
	unsigned int num_pins;
	const void *data;
};

/**
 * struct meson_pmx_func - a pinmux function
 *
 * @name:	function name
 * @groups:	groups in the function
 * @num_groups:	number of groups in the function
 */
struct meson_pmx_func {
	const char *name;
	const char * const *groups;
	unsigned int num_groups;
};

/**
 * struct meson_reg_desc - a register descriptor
 *
 * @reg:	register offset in the regmap
 * @bit:	bit index in register
 *
 * The structure describes the information needed to control pull,
 * pull-enable, direction, etc. for a single pin
 */
struct meson_reg_desc {
	unsigned int reg;
	unsigned int bit;
};

/**
 * enum meson_reg_type - type of registers encoded in @meson_reg_desc
 */
enum meson_reg_type {
	REG_PULLEN,
	REG_PULL,
	REG_DIR,
	REG_OUT,
	REG_IN,
<<<<<<< HEAD
=======
	REG_DS,
>>>>>>> upstream/android-13
	NUM_REG,
};

/**
<<<<<<< HEAD
=======
 * enum meson_pinconf_drv - value of drive-strength supported
 */
enum meson_pinconf_drv {
	MESON_PINCONF_DRV_500UA,
	MESON_PINCONF_DRV_2500UA,
	MESON_PINCONF_DRV_3000UA,
	MESON_PINCONF_DRV_4000UA,
};

/**
>>>>>>> upstream/android-13
 * struct meson bank
 *
 * @name:	bank name
 * @first:	first pin of the bank
 * @last:	last pin of the bank
 * @irq:	hwirq base number of the bank
 * @regs:	array of register descriptors
 *
 * A bank represents a set of pins controlled by a contiguous set of
 * bits in the domain registers. The structure specifies which bits in
 * the regmap control the different functionalities. Each member of
 * the @regs array refers to the first pin of the bank.
 */
struct meson_bank {
	const char *name;
	unsigned int first;
	unsigned int last;
	int irq_first;
	int irq_last;
	struct meson_reg_desc regs[NUM_REG];
};

struct meson_pinctrl_data {
	const char *name;
	const struct pinctrl_pin_desc *pins;
	struct meson_pmx_group *groups;
	struct meson_pmx_func *funcs;
	unsigned int num_pins;
	unsigned int num_groups;
	unsigned int num_funcs;
	struct meson_bank *banks;
	unsigned int num_banks;
	const struct pinmux_ops *pmx_ops;
	void *pmx_data;
<<<<<<< HEAD
=======
	int (*parse_dt)(struct meson_pinctrl *pc);
>>>>>>> upstream/android-13
};

struct meson_pinctrl {
	struct device *dev;
	struct pinctrl_dev *pcdev;
	struct pinctrl_desc desc;
	struct meson_pinctrl_data *data;
	struct regmap *reg_mux;
	struct regmap *reg_pullen;
	struct regmap *reg_pull;
	struct regmap *reg_gpio;
<<<<<<< HEAD
=======
	struct regmap *reg_ds;
>>>>>>> upstream/android-13
	struct gpio_chip chip;
	struct device_node *of_node;
};

#define FUNCTION(fn)							\
	{								\
		.name = #fn,						\
		.groups = fn ## _groups,				\
		.num_groups = ARRAY_SIZE(fn ## _groups),		\
	}

<<<<<<< HEAD
#define BANK(n, f, l, fi, li, per, peb, pr, pb, dr, db, or, ob, ir, ib)	\
=======
#define BANK_DS(n, f, l, fi, li, per, peb, pr, pb, dr, db, or, ob, ir, ib,     \
		dsr, dsb)                                                      \
>>>>>>> upstream/android-13
	{								\
		.name		= n,					\
		.first		= f,					\
		.last		= l,					\
		.irq_first	= fi,					\
		.irq_last	= li,					\
		.regs = {						\
			[REG_PULLEN]	= { per, peb },			\
			[REG_PULL]	= { pr, pb },			\
			[REG_DIR]	= { dr, db },			\
			[REG_OUT]	= { or, ob },			\
			[REG_IN]	= { ir, ib },			\
<<<<<<< HEAD
		},							\
	 }

=======
			[REG_DS]	= { dsr, dsb },			\
		},							\
	 }

#define BANK(n, f, l, fi, li, per, peb, pr, pb, dr, db, or, ob, ir, ib) \
	BANK_DS(n, f, l, fi, li, per, peb, pr, pb, dr, db, or, ob, ir, ib, 0, 0)

>>>>>>> upstream/android-13
#define MESON_PIN(x) PINCTRL_PIN(x, #x)

/* Common pmx functions */
int meson_pmx_get_funcs_count(struct pinctrl_dev *pcdev);
const char *meson_pmx_get_func_name(struct pinctrl_dev *pcdev,
				    unsigned selector);
int meson_pmx_get_groups(struct pinctrl_dev *pcdev,
			 unsigned selector,
			 const char * const **groups,
			 unsigned * const num_groups);

/* Common probe function */
int meson_pinctrl_probe(struct platform_device *pdev);
<<<<<<< HEAD
=======
/* Common ao groups extra dt parse function for SoCs before g12a  */
int meson8_aobus_parse_dt_extra(struct meson_pinctrl *pc);
/* Common extra dt parse function for SoCs like A1  */
int meson_a1_parse_dt_extra(struct meson_pinctrl *pc);
>>>>>>> upstream/android-13
