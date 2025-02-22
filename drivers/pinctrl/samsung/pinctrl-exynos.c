// SPDX-License-Identifier: GPL-2.0+
//
// Exynos specific support for Samsung pinctrl/gpiolib driver with eint support.
//
// Copyright (c) 2012 Samsung Electronics Co., Ltd.
//		http://www.samsung.com
// Copyright (c) 2012 Linaro Ltd
//		http://www.linaro.org
//
// Author: Thomas Abraham <thomas.ab@samsung.com>
//
// This file contains the Samsung Exynos specific information required by the
// the Samsung pinctrl/gpiolib driver. It also includes the implementation of
// external gpio and wakeup interrupt support.

#include <linux/device.h>
#include <linux/interrupt.h>
#include <linux/irqdomain.h>
#include <linux/irq.h>
#include <linux/irqchip/chained_irq.h>
<<<<<<< HEAD
=======
#include <linux/module.h>
>>>>>>> upstream/android-13
#include <linux/of.h>
#include <linux/of_irq.h>
#include <linux/slab.h>
#include <linux/spinlock.h>
#include <linux/regmap.h>
<<<<<<< HEAD
=======
#include <linux/mfd/syscon.h>
>>>>>>> upstream/android-13
#include <linux/err.h>
#include <linux/soc/samsung/exynos-pmu.h>
#include <linux/soc/samsung/exynos-regs-pmu.h>

#include <dt-bindings/pinctrl/samsung.h>

#include "pinctrl-samsung.h"
#include "pinctrl-exynos.h"

struct exynos_irq_chip {
	struct irq_chip chip;

	u32 eint_con;
	u32 eint_mask;
	u32 eint_pend;
<<<<<<< HEAD
	u32 eint_wake_mask_value;
=======
	u32 *eint_wake_mask_value;
>>>>>>> upstream/android-13
	u32 eint_wake_mask_reg;
	void (*set_eint_wakeup_mask)(struct samsung_pinctrl_drv_data *drvdata,
				     struct exynos_irq_chip *irq_chip);
};

static inline struct exynos_irq_chip *to_exynos_irq_chip(struct irq_chip *chip)
{
	return container_of(chip, struct exynos_irq_chip, chip);
}

static void exynos_irq_mask(struct irq_data *irqd)
{
	struct irq_chip *chip = irq_data_get_irq_chip(irqd);
	struct exynos_irq_chip *our_chip = to_exynos_irq_chip(chip);
	struct samsung_pin_bank *bank = irq_data_get_irq_chip_data(irqd);
	unsigned long reg_mask = our_chip->eint_mask + bank->eint_offset;
	unsigned int mask;
	unsigned long flags;

<<<<<<< HEAD
	spin_lock_irqsave(&bank->slock, flags);
=======
	raw_spin_lock_irqsave(&bank->slock, flags);
>>>>>>> upstream/android-13

	mask = readl(bank->eint_base + reg_mask);
	mask |= 1 << irqd->hwirq;
	writel(mask, bank->eint_base + reg_mask);

<<<<<<< HEAD
	spin_unlock_irqrestore(&bank->slock, flags);
=======
	raw_spin_unlock_irqrestore(&bank->slock, flags);
>>>>>>> upstream/android-13
}

static void exynos_irq_ack(struct irq_data *irqd)
{
	struct irq_chip *chip = irq_data_get_irq_chip(irqd);
	struct exynos_irq_chip *our_chip = to_exynos_irq_chip(chip);
	struct samsung_pin_bank *bank = irq_data_get_irq_chip_data(irqd);
	unsigned long reg_pend = our_chip->eint_pend + bank->eint_offset;

	writel(1 << irqd->hwirq, bank->eint_base + reg_pend);
}

static void exynos_irq_unmask(struct irq_data *irqd)
{
	struct irq_chip *chip = irq_data_get_irq_chip(irqd);
	struct exynos_irq_chip *our_chip = to_exynos_irq_chip(chip);
	struct samsung_pin_bank *bank = irq_data_get_irq_chip_data(irqd);
	unsigned long reg_mask = our_chip->eint_mask + bank->eint_offset;
	unsigned int mask;
	unsigned long flags;

	/*
	 * Ack level interrupts right before unmask
	 *
	 * If we don't do this we'll get a double-interrupt.  Level triggered
	 * interrupts must not fire an interrupt if the level is not
	 * _currently_ active, even if it was active while the interrupt was
	 * masked.
	 */
	if (irqd_get_trigger_type(irqd) & IRQ_TYPE_LEVEL_MASK)
		exynos_irq_ack(irqd);

<<<<<<< HEAD
	spin_lock_irqsave(&bank->slock, flags);
=======
	raw_spin_lock_irqsave(&bank->slock, flags);
>>>>>>> upstream/android-13

	mask = readl(bank->eint_base + reg_mask);
	mask &= ~(1 << irqd->hwirq);
	writel(mask, bank->eint_base + reg_mask);

<<<<<<< HEAD
	spin_unlock_irqrestore(&bank->slock, flags);
=======
	raw_spin_unlock_irqrestore(&bank->slock, flags);
>>>>>>> upstream/android-13
}

static int exynos_irq_set_type(struct irq_data *irqd, unsigned int type)
{
	struct irq_chip *chip = irq_data_get_irq_chip(irqd);
	struct exynos_irq_chip *our_chip = to_exynos_irq_chip(chip);
	struct samsung_pin_bank *bank = irq_data_get_irq_chip_data(irqd);
	unsigned int shift = EXYNOS_EINT_CON_LEN * irqd->hwirq;
	unsigned int con, trig_type;
	unsigned long reg_con = our_chip->eint_con + bank->eint_offset;

	switch (type) {
	case IRQ_TYPE_EDGE_RISING:
		trig_type = EXYNOS_EINT_EDGE_RISING;
		break;
	case IRQ_TYPE_EDGE_FALLING:
		trig_type = EXYNOS_EINT_EDGE_FALLING;
		break;
	case IRQ_TYPE_EDGE_BOTH:
		trig_type = EXYNOS_EINT_EDGE_BOTH;
		break;
	case IRQ_TYPE_LEVEL_HIGH:
		trig_type = EXYNOS_EINT_LEVEL_HIGH;
		break;
	case IRQ_TYPE_LEVEL_LOW:
		trig_type = EXYNOS_EINT_LEVEL_LOW;
		break;
	default:
		pr_err("unsupported external interrupt type\n");
		return -EINVAL;
	}

	if (type & IRQ_TYPE_EDGE_BOTH)
		irq_set_handler_locked(irqd, handle_edge_irq);
	else
		irq_set_handler_locked(irqd, handle_level_irq);

	con = readl(bank->eint_base + reg_con);
	con &= ~(EXYNOS_EINT_CON_MASK << shift);
	con |= trig_type << shift;
	writel(con, bank->eint_base + reg_con);

	return 0;
}

static int exynos_irq_request_resources(struct irq_data *irqd)
{
	struct samsung_pin_bank *bank = irq_data_get_irq_chip_data(irqd);
	const struct samsung_pin_bank_type *bank_type = bank->type;
	unsigned long reg_con, flags;
<<<<<<< HEAD
	unsigned int shift, mask, con;
=======
	unsigned int shift, mask, con, pin;
>>>>>>> upstream/android-13
	int ret;

	ret = gpiochip_lock_as_irq(&bank->gpio_chip, irqd->hwirq);
	if (ret) {
		dev_err(bank->gpio_chip.parent,
			"unable to lock pin %s-%lu IRQ\n",
			bank->name, irqd->hwirq);
		return ret;
	}

	reg_con = bank->pctl_offset + bank_type->reg_offset[PINCFG_TYPE_FUNC];
	shift = irqd->hwirq * bank_type->fld_width[PINCFG_TYPE_FUNC];
	mask = (1 << bank_type->fld_width[PINCFG_TYPE_FUNC]) - 1;

<<<<<<< HEAD
	spin_lock_irqsave(&bank->slock, flags);
=======
	raw_spin_lock_irqsave(&bank->slock, flags);
>>>>>>> upstream/android-13

	con = readl(bank->pctl_base + reg_con);
	con &= ~(mask << shift);
	con |= EXYNOS_PIN_FUNC_EINT << shift;
	writel(con, bank->pctl_base + reg_con);

<<<<<<< HEAD
	spin_unlock_irqrestore(&bank->slock, flags);
=======
	pin = bank->grange.pin_base + irqd->hwirq - bank->drvdata->pin_base;
	bank->drvdata->pin_groups[pin].state[PINCFG_TYPE_FUNC] = EXYNOS_PIN_FUNC_EINT;
	raw_spin_unlock_irqrestore(&bank->slock, flags);
>>>>>>> upstream/android-13

	return 0;
}

static void exynos_irq_release_resources(struct irq_data *irqd)
{
	struct samsung_pin_bank *bank = irq_data_get_irq_chip_data(irqd);
	const struct samsung_pin_bank_type *bank_type = bank->type;
	unsigned long reg_con, flags;
	unsigned int shift, mask, con;

	reg_con = bank->pctl_offset + bank_type->reg_offset[PINCFG_TYPE_FUNC];
	shift = irqd->hwirq * bank_type->fld_width[PINCFG_TYPE_FUNC];
	mask = (1 << bank_type->fld_width[PINCFG_TYPE_FUNC]) - 1;

<<<<<<< HEAD
	spin_lock_irqsave(&bank->slock, flags);
=======
	raw_spin_lock_irqsave(&bank->slock, flags);
>>>>>>> upstream/android-13

	con = readl(bank->pctl_base + reg_con);
	con &= ~(mask << shift);
	con |= EXYNOS_PIN_FUNC_INPUT << shift;
	writel(con, bank->pctl_base + reg_con);

<<<<<<< HEAD
	spin_unlock_irqrestore(&bank->slock, flags);
=======
	raw_spin_unlock_irqrestore(&bank->slock, flags);
>>>>>>> upstream/android-13

	gpiochip_unlock_as_irq(&bank->gpio_chip, irqd->hwirq);
}

/*
 * irq_chip for gpio interrupts.
<<<<<<< HEAD
 */
static struct exynos_irq_chip exynos_gpio_irq_chip = {
	.chip = {
		.name = "exynos_gpio_irq_chip",
		.irq_unmask = exynos_irq_unmask,
=======
 * For normal interrupt action, interrupt mask / unmask must be paired.
 * When S2R process, irq mask cannot be done without irq_disable callback.
 * We added irq mask & unmask to disable & enable callback to
 * set the interrupt mask and unmask value completely.
 */
static const struct exynos_irq_chip exynos_gpio_irq_chip = {
	.chip = {
		.name = "exynos_gpio_irq_chip",
		.irq_enable = exynos_irq_unmask,
		.irq_unmask = exynos_irq_unmask,
		.irq_disable = exynos_irq_mask,
>>>>>>> upstream/android-13
		.irq_mask = exynos_irq_mask,
		.irq_ack = exynos_irq_ack,
		.irq_set_type = exynos_irq_set_type,
		.irq_request_resources = exynos_irq_request_resources,
		.irq_release_resources = exynos_irq_release_resources,
	},
	.eint_con = EXYNOS_GPIO_ECON_OFFSET,
	.eint_mask = EXYNOS_GPIO_EMASK_OFFSET,
	.eint_pend = EXYNOS_GPIO_EPEND_OFFSET,
	/* eint_wake_mask_value not used */
};

static int exynos_eint_irq_map(struct irq_domain *h, unsigned int virq,
					irq_hw_number_t hw)
{
	struct samsung_pin_bank *b = h->host_data;

	irq_set_chip_data(virq, b);
	irq_set_chip_and_handler(virq, &b->irq_chip->chip,
					handle_level_irq);
	return 0;
}

/*
 * irq domain callbacks for external gpio and wakeup interrupt controllers.
 */
static const struct irq_domain_ops exynos_eint_irqd_ops = {
	.map	= exynos_eint_irq_map,
	.xlate	= irq_domain_xlate_twocell,
};

static irqreturn_t exynos_eint_gpio_irq(int irq, void *data)
{
	struct samsung_pinctrl_drv_data *d = data;
	struct samsung_pin_bank *bank = d->pin_banks;
<<<<<<< HEAD
	unsigned int svc, group, pin, virq;
=======
	unsigned int svc, group, pin;
	int ret;
>>>>>>> upstream/android-13

	svc = readl(bank->eint_base + EXYNOS_SVC_OFFSET);
	group = EXYNOS_SVC_GROUP(svc);
	pin = svc & EXYNOS_SVC_NUM_MASK;

	if (!group)
		return IRQ_HANDLED;
	bank += (group - 1);

<<<<<<< HEAD
	virq = irq_linear_revmap(bank->irq_domain, pin);
	if (!virq)
		return IRQ_NONE;
	generic_handle_irq(virq);
=======
	ret = generic_handle_domain_irq(bank->irq_domain, pin);
	if (ret)
		return IRQ_NONE;

>>>>>>> upstream/android-13
	return IRQ_HANDLED;
}

struct exynos_eint_gpio_save {
	u32 eint_con;
	u32 eint_fltcon0;
	u32 eint_fltcon1;
	u32 eint_mask;
};

<<<<<<< HEAD
=======
static void exynos_eint_flt_config(int en, int sel, int width,
				   struct samsung_pinctrl_drv_data *d,
				   struct samsung_pin_bank *bank)
{
	unsigned int flt_reg, flt_con;
	unsigned int val, shift;
	int i;
	int loop_cnt;

	flt_con = 0;

	if (en)
		flt_con |= EXYNOS_EINT_FLTCON_EN;

	if (sel)
		flt_con |= EXYNOS_EINT_FLTCON_SEL;

	flt_con |= EXYNOS_EINT_FLTCON_WIDTH(width);

	flt_reg = EXYNOS_GPIO_EFLTCON_OFFSET + bank->fltcon_offset;

	if (bank->nr_pins > 4)
		/* if nr_pins > 4, we should set FLTCON0 register fully. (pin0 ~ 3) */
		/* So, we shoud loop 4 times in case of FLTCON0. */
		loop_cnt = 4;
	else
		loop_cnt = bank->nr_pins;

	val = readl(d->virt_base + flt_reg);

	for (i = 0; i < loop_cnt; i++) {
		shift = i * EXYNOS_EINT_FLTCON_LEN;
		val &= ~(EXYNOS_EINT_FLTCON_MASK << shift);
		val |= (flt_con << shift);
	}

	writel(val, d->virt_base + flt_reg);

	/* if nr_pins > 4, we should also set FLTCON1 register like FLTCON0. (pin4 ~ ) */
	if (bank->nr_pins > 4)
		writel(val, d->virt_base + flt_reg + 0x4);
};

>>>>>>> upstream/android-13
/*
 * exynos_eint_gpio_init() - setup handling of external gpio interrupts.
 * @d: driver data of samsung pinctrl driver.
 */
int exynos_eint_gpio_init(struct samsung_pinctrl_drv_data *d)
{
	struct samsung_pin_bank *bank;
	struct device *dev = d->dev;
	int ret;
	int i;

	if (!d->irq) {
		dev_err(dev, "irq number not available\n");
		return -EINVAL;
	}

	ret = devm_request_irq(dev, d->irq, exynos_eint_gpio_irq,
					0, dev_name(dev), d);
	if (ret) {
		dev_err(dev, "irq request failed\n");
		return -ENXIO;
	}

	bank = d->pin_banks;
	for (i = 0; i < d->nr_banks; ++i, ++bank) {
		if (bank->eint_type != EINT_TYPE_GPIO)
			continue;
<<<<<<< HEAD
=======

		bank->irq_chip = devm_kmemdup(dev, &exynos_gpio_irq_chip,
					   sizeof(*bank->irq_chip), GFP_KERNEL);
		if (!bank->irq_chip) {
			ret = -ENOMEM;
			goto err_domains;
		}
		bank->irq_chip->chip.name = bank->name;

>>>>>>> upstream/android-13
		bank->irq_domain = irq_domain_add_linear(bank->of_node,
				bank->nr_pins, &exynos_eint_irqd_ops, bank);
		if (!bank->irq_domain) {
			dev_err(dev, "gpio irq domain add failed\n");
			ret = -ENXIO;
			goto err_domains;
		}

		bank->soc_priv = devm_kzalloc(d->dev,
			sizeof(struct exynos_eint_gpio_save), GFP_KERNEL);
		if (!bank->soc_priv) {
			irq_domain_remove(bank->irq_domain);
			ret = -ENOMEM;
			goto err_domains;
		}

<<<<<<< HEAD
		bank->irq_chip = &exynos_gpio_irq_chip;
=======
		/* There is no filter selection register except for alive block */
		/* Except for alive block, digital filter is default setting */
		exynos_eint_flt_config(EXYNOS_EINT_FLTCON_EN, 0, 0, d, bank);
>>>>>>> upstream/android-13
	}

	return 0;

err_domains:
	for (--i, --bank; i >= 0; --i, --bank) {
		if (bank->eint_type != EINT_TYPE_GPIO)
			continue;
		irq_domain_remove(bank->irq_domain);
	}

	return ret;
}

<<<<<<< HEAD
static u32 exynos_eint_wake_mask = 0xffffffff;

u32 exynos_get_eint_wake_mask(void)
{
	return exynos_eint_wake_mask;
}

static int exynos_wkup_irq_set_wake(struct irq_data *irqd, unsigned int on)
{
	struct irq_chip *chip = irq_data_get_irq_chip(irqd);
	struct exynos_irq_chip *our_chip = to_exynos_irq_chip(chip);
	struct samsung_pin_bank *bank = irq_data_get_irq_chip_data(irqd);
	unsigned long bit = 1UL << (2 * bank->eint_offset + irqd->hwirq);

	pr_info("wake %s for irq %d\n", on ? "enabled" : "disabled", irqd->irq);

	if (!on)
		exynos_eint_wake_mask |= bit;
	else
		exynos_eint_wake_mask &= ~bit;
	our_chip->eint_wake_mask_value = exynos_eint_wake_mask;
=======
static DECLARE_BITMAP(exynos_eint_wake_mask_array_ul,
			EXYNOS_EINT_WKUP_MASK_NUM * 32) = {
			 [0 ... BITS_TO_LONGS(EXYNOS_EINT_WKUP_MASK_NUM * 32) - 1] = ~0UL };

u32 exynos_eint_wake_mask_array[EXYNOS_EINT_WKUP_MASK_NUM];
EXPORT_SYMBOL(exynos_eint_wake_mask_array);

static DECLARE_BITMAP(exynos_cmgp_eint_wake_mask_array_ul,
			EXYNOS_EINT_WKUP_MASK_NUM * 32) = {
			[0 ... BITS_TO_LONGS(EXYNOS_EINT_WKUP_MASK_NUM * 32) - 1] = ~0UL };
u32 exynos_cmgp_eint_wake_mask_array[EXYNOS_EINT_WKUP_MASK_NUM];
EXPORT_SYMBOL(exynos_cmgp_eint_wake_mask_array);

static int exynos_wkup_irq_set_wake(struct irq_data *irqd, unsigned int on)
{
	struct samsung_pin_bank *bank = irq_data_get_irq_chip_data(irqd);
	struct samsung_pinctrl_drv_data *d = bank->drvdata;
	u32 bit = 0;
	int i;

	bit = bank->eint_num + irqd->hwirq;
	pr_info("%s: bit = %d, bank->eint_num = %d, irqd->hwirq = %d",
			__func__, bit, bank->eint_num, irqd->hwirq);

	if (!on)
		exynos_eint_wake_mask_array_ul[BIT_WORD(bit)] |= BIT_MASK(bit);
	else
		exynos_eint_wake_mask_array_ul[BIT_WORD(bit)] &= ~BIT_MASK(bit);

	bitmap_to_arr32(exynos_eint_wake_mask_array,
			exynos_eint_wake_mask_array_ul,
			EXYNOS_EINT_WKUP_MASK_NUM * 32);

	dev_info(d->dev, "wake %s for irq %d\n", on ? "enabled" : "disabled",
		 irqd->irq);
	dev_info(d->dev, "(%s:0x%x) dirq = %d, eint_num = %u\n",
		 bank->name, bank->eint_offset, d->irq, bank->eint_num);

	for (i = 0; i < EXYNOS_EINT_WKUP_MASK_NUM; i++)
		dev_info(d->dev, "exynos_eint_wake_mask value index %d = (0x%X)\n",
		i, exynos_eint_wake_mask_array[i]);

	return 0;
}

static int exynos_cmgp_wkup_irq_set_wake(struct irq_data *irqd, unsigned int on)
{
	struct samsung_pin_bank *bank = irq_data_get_irq_chip_data(irqd);
	struct samsung_pinctrl_drv_data *d = bank->drvdata;
	unsigned long bit = 1UL << bank->sysreg_cmgp_bit;
	int i = 0;

	regmap_update_bits(d->sysreg_cmgp, bank->sysreg_cmgp_offs, bit, bit);

	bit = bank->eint_num;
	pr_info("%s: bit = %d, bank->eint_num = %d, irqd->hwirq = %d",
			__func__, bit, bank->eint_num, irqd->hwirq);
	if (!on)
		exynos_cmgp_eint_wake_mask_array_ul[BIT_WORD(bit)] |= BIT_MASK(bit);
	else
		exynos_cmgp_eint_wake_mask_array_ul[BIT_WORD(bit)] &= ~BIT_MASK(bit);

	bitmap_to_arr32(exynos_cmgp_eint_wake_mask_array,
			exynos_cmgp_eint_wake_mask_array_ul,
			EXYNOS_EINT_WKUP_MASK_NUM * 32);

	dev_info(d->dev, "wake %s for irq %d\n", on ? "enabled" : "disabled",
		 irqd->irq);
	dev_info(d->dev, "(%s:0x%x) dirq = %d, eint_num = %u\n",
		 bank->name, bank->eint_offset, d->irq, bank->eint_num);

	for (i = 0; i < EXYNOS_EINT_WKUP_MASK_NUM; i++)
		dev_info(d->dev, "exynos_cmgp_eint_wake_mask value index %d = (0x%X)\n",
		i, exynos_cmgp_eint_wake_mask_array[i]);
>>>>>>> upstream/android-13

	return 0;
}

static void
exynos_pinctrl_set_eint_wakeup_mask(struct samsung_pinctrl_drv_data *drvdata,
				    struct exynos_irq_chip *irq_chip)
{
	struct regmap *pmu_regs;

	if (!drvdata->retention_ctrl || !drvdata->retention_ctrl->priv) {
		dev_warn(drvdata->dev,
			 "No retention data configured bank with external wakeup interrupt. Wake-up mask will not be set.\n");
		return;
	}

	pmu_regs = drvdata->retention_ctrl->priv;
	dev_info(drvdata->dev,
		 "Setting external wakeup interrupt mask: 0x%x\n",
<<<<<<< HEAD
		 irq_chip->eint_wake_mask_value);

	regmap_write(pmu_regs, irq_chip->eint_wake_mask_reg,
		     irq_chip->eint_wake_mask_value);
=======
		 *irq_chip->eint_wake_mask_value);

	regmap_write(pmu_regs, irq_chip->eint_wake_mask_reg,
		     *irq_chip->eint_wake_mask_value);
>>>>>>> upstream/android-13
}

static void
s5pv210_pinctrl_set_eint_wakeup_mask(struct samsung_pinctrl_drv_data *drvdata,
				    struct exynos_irq_chip *irq_chip)

{
	void __iomem *clk_base;

	if (!drvdata->retention_ctrl || !drvdata->retention_ctrl->priv) {
		dev_warn(drvdata->dev,
			 "No retention data configured bank with external wakeup interrupt. Wake-up mask will not be set.\n");
		return;
	}


	clk_base = (void __iomem *) drvdata->retention_ctrl->priv;

<<<<<<< HEAD
	__raw_writel(irq_chip->eint_wake_mask_value,
		     clk_base + irq_chip->eint_wake_mask_reg);
}

=======
	__raw_writel(*irq_chip->eint_wake_mask_value,
		     clk_base + irq_chip->eint_wake_mask_reg);
}

static u32 eint_wake_mask_value = EXYNOS_EINT_WAKEUP_MASK_DISABLED;

>>>>>>> upstream/android-13
/*
 * irq_chip for wakeup interrupts
 */
static const struct exynos_irq_chip s5pv210_wkup_irq_chip __initconst = {
	.chip = {
		.name = "s5pv210_wkup_irq_chip",
		.irq_unmask = exynos_irq_unmask,
		.irq_mask = exynos_irq_mask,
		.irq_ack = exynos_irq_ack,
		.irq_set_type = exynos_irq_set_type,
		.irq_set_wake = exynos_wkup_irq_set_wake,
		.irq_request_resources = exynos_irq_request_resources,
		.irq_release_resources = exynos_irq_release_resources,
	},
	.eint_con = EXYNOS_WKUP_ECON_OFFSET,
	.eint_mask = EXYNOS_WKUP_EMASK_OFFSET,
	.eint_pend = EXYNOS_WKUP_EPEND_OFFSET,
<<<<<<< HEAD
	.eint_wake_mask_value = EXYNOS_EINT_WAKEUP_MASK_DISABLED,
=======
	.eint_wake_mask_value = &eint_wake_mask_value,
>>>>>>> upstream/android-13
	/* Only differences with exynos4210_wkup_irq_chip: */
	.eint_wake_mask_reg = S5PV210_EINT_WAKEUP_MASK,
	.set_eint_wakeup_mask = s5pv210_pinctrl_set_eint_wakeup_mask,
};

static const struct exynos_irq_chip exynos4210_wkup_irq_chip __initconst = {
	.chip = {
		.name = "exynos4210_wkup_irq_chip",
		.irq_unmask = exynos_irq_unmask,
		.irq_mask = exynos_irq_mask,
		.irq_ack = exynos_irq_ack,
		.irq_set_type = exynos_irq_set_type,
		.irq_set_wake = exynos_wkup_irq_set_wake,
		.irq_request_resources = exynos_irq_request_resources,
		.irq_release_resources = exynos_irq_release_resources,
	},
	.eint_con = EXYNOS_WKUP_ECON_OFFSET,
	.eint_mask = EXYNOS_WKUP_EMASK_OFFSET,
	.eint_pend = EXYNOS_WKUP_EPEND_OFFSET,
<<<<<<< HEAD
	.eint_wake_mask_value = EXYNOS_EINT_WAKEUP_MASK_DISABLED,
=======
	.eint_wake_mask_value = &eint_wake_mask_value,
>>>>>>> upstream/android-13
	.eint_wake_mask_reg = EXYNOS_EINT_WAKEUP_MASK,
	.set_eint_wakeup_mask = exynos_pinctrl_set_eint_wakeup_mask,
};

static const struct exynos_irq_chip exynos7_wkup_irq_chip __initconst = {
	.chip = {
		.name = "exynos7_wkup_irq_chip",
		.irq_unmask = exynos_irq_unmask,
		.irq_mask = exynos_irq_mask,
		.irq_ack = exynos_irq_ack,
		.irq_set_type = exynos_irq_set_type,
		.irq_set_wake = exynos_wkup_irq_set_wake,
		.irq_request_resources = exynos_irq_request_resources,
		.irq_release_resources = exynos_irq_release_resources,
	},
	.eint_con = EXYNOS7_WKUP_ECON_OFFSET,
	.eint_mask = EXYNOS7_WKUP_EMASK_OFFSET,
	.eint_pend = EXYNOS7_WKUP_EPEND_OFFSET,
<<<<<<< HEAD
	.eint_wake_mask_value = EXYNOS_EINT_WAKEUP_MASK_DISABLED,
=======
	.eint_wake_mask_value = &eint_wake_mask_value,
	.eint_wake_mask_reg = EXYNOS5433_EINT_WAKEUP_MASK,
	.set_eint_wakeup_mask = exynos_pinctrl_set_eint_wakeup_mask,
};

static const struct exynos_irq_chip exynos_cmgp_wkup_irq_chip __initconst = {
	.chip = {
		.name = "exynos_cmgp_wkup_irq_chip",
		.irq_unmask = exynos_irq_unmask,
		.irq_mask = exynos_irq_mask,
		.irq_ack = exynos_irq_ack,
		.irq_set_type = exynos_irq_set_type,
		.irq_set_wake = exynos_cmgp_wkup_irq_set_wake,
		.irq_request_resources = exynos_irq_request_resources,
		.irq_release_resources = exynos_irq_release_resources,
	},
	.eint_con = EXYNOS7_WKUP_ECON_OFFSET,
	.eint_mask = EXYNOS7_WKUP_EMASK_OFFSET,
	.eint_pend = EXYNOS7_WKUP_EPEND_OFFSET,
	.eint_wake_mask_value = &eint_wake_mask_value,
>>>>>>> upstream/android-13
	.eint_wake_mask_reg = EXYNOS5433_EINT_WAKEUP_MASK,
	.set_eint_wakeup_mask = exynos_pinctrl_set_eint_wakeup_mask,
};

/* list of external wakeup controllers supported */
static const struct of_device_id exynos_wkup_irq_ids[] = {
	{ .compatible = "samsung,s5pv210-wakeup-eint",
			.data = &s5pv210_wkup_irq_chip },
	{ .compatible = "samsung,exynos4210-wakeup-eint",
			.data = &exynos4210_wkup_irq_chip },
	{ .compatible = "samsung,exynos7-wakeup-eint",
			.data = &exynos7_wkup_irq_chip },
<<<<<<< HEAD
=======
	{ .compatible = "samsung,exynos-cmgp-wakeup-eint",
			.data = &exynos_cmgp_wkup_irq_chip },
>>>>>>> upstream/android-13
	{ }
};

/* interrupt handler for wakeup interrupts 0..15 */
static void exynos_irq_eint0_15(struct irq_desc *desc)
{
	struct exynos_weint_data *eintd = irq_desc_get_handler_data(desc);
	struct samsung_pin_bank *bank = eintd->bank;
	struct irq_chip *chip = irq_desc_get_chip(desc);
<<<<<<< HEAD
	int eint_irq;

	chained_irq_enter(chip, desc);

	eint_irq = irq_linear_revmap(bank->irq_domain, eintd->irq);
	generic_handle_irq(eint_irq);
=======

	chained_irq_enter(chip, desc);

	generic_handle_domain_irq(bank->irq_domain, eintd->irq);
>>>>>>> upstream/android-13

	chained_irq_exit(chip, desc);
}

static inline void exynos_irq_demux_eint(unsigned int pend,
						struct irq_domain *domain)
{
	unsigned int irq;

	while (pend) {
		irq = fls(pend) - 1;
<<<<<<< HEAD
		generic_handle_irq(irq_find_mapping(domain, irq));
=======
		generic_handle_domain_irq(domain, irq);
>>>>>>> upstream/android-13
		pend &= ~(1 << irq);
	}
}

/* interrupt handler for wakeup interrupt 16 */
static void exynos_irq_demux_eint16_31(struct irq_desc *desc)
{
	struct irq_chip *chip = irq_desc_get_chip(desc);
	struct exynos_muxed_weint_data *eintd = irq_desc_get_handler_data(desc);
	unsigned int pend;
	unsigned int mask;
	int i;

	chained_irq_enter(chip, desc);

	for (i = 0; i < eintd->nr_banks; ++i) {
		struct samsung_pin_bank *b = eintd->banks[i];
		pend = readl(b->eint_base + b->irq_chip->eint_pend
				+ b->eint_offset);
		mask = readl(b->eint_base + b->irq_chip->eint_mask
				+ b->eint_offset);
		exynos_irq_demux_eint(pend & ~mask, b->irq_domain);
	}

	chained_irq_exit(chip, desc);
}

<<<<<<< HEAD
=======

static int eint_num;
>>>>>>> upstream/android-13
/*
 * exynos_eint_wkup_init() - setup handling of external wakeup interrupts.
 * @d: driver data of samsung pinctrl driver.
 */
int exynos_eint_wkup_init(struct samsung_pinctrl_drv_data *d)
{
	struct device *dev = d->dev;
	struct device_node *wkup_np = NULL;
	struct device_node *np;
	struct samsung_pin_bank *bank;
	struct exynos_weint_data *weint_data;
	struct exynos_muxed_weint_data *muxed_data;
<<<<<<< HEAD
	struct exynos_irq_chip *irq_chip;
=======
	const struct exynos_irq_chip *irq_chip;
>>>>>>> upstream/android-13
	unsigned int muxed_banks = 0;
	unsigned int i;
	int idx, irq;

	for_each_child_of_node(dev->of_node, np) {
		const struct of_device_id *match;

		match = of_match_node(exynos_wkup_irq_ids, np);
		if (match) {
<<<<<<< HEAD
			irq_chip = kmemdup(match->data,
				sizeof(*irq_chip), GFP_KERNEL);
			if (!irq_chip) {
				of_node_put(np);
				return -ENOMEM;
			}
=======
			irq_chip = match->data;
>>>>>>> upstream/android-13
			wkup_np = np;
			break;
		}
	}
	if (!wkup_np)
		return -ENODEV;

	bank = d->pin_banks;
	for (i = 0; i < d->nr_banks; ++i, ++bank) {
		if (bank->eint_type != EINT_TYPE_WKUP)
			continue;

<<<<<<< HEAD
=======
		bank->irq_chip = devm_kmemdup(dev, irq_chip, sizeof(*irq_chip),
					      GFP_KERNEL);
		if (!bank->irq_chip) {
			of_node_put(wkup_np);
			return -ENOMEM;
		}
		bank->irq_chip->chip.name = bank->name;

		/* Only alive block has filter selection register. */
		/* Setting Digital Filter */
		exynos_eint_flt_config(EXYNOS_EINT_FLTCON_EN, EXYNOS_EINT_FLTCON_SEL, 0, d, bank);

>>>>>>> upstream/android-13
		bank->irq_domain = irq_domain_add_linear(bank->of_node,
				bank->nr_pins, &exynos_eint_irqd_ops, bank);
		if (!bank->irq_domain) {
			dev_err(dev, "wkup irq domain add failed\n");
			of_node_put(wkup_np);
			return -ENXIO;
		}

<<<<<<< HEAD
		bank->irq_chip = irq_chip;
=======
		bank->eint_num = eint_num;
		eint_num = eint_num + bank->nr_pins;
>>>>>>> upstream/android-13

		if (!of_find_property(bank->of_node, "interrupts", NULL)) {
			bank->eint_type = EINT_TYPE_WKUP_MUX;
			++muxed_banks;
			continue;
		}

		weint_data = devm_kcalloc(dev,
					  bank->nr_pins, sizeof(*weint_data),
					  GFP_KERNEL);
		if (!weint_data) {
			of_node_put(wkup_np);
			return -ENOMEM;
		}

		for (idx = 0; idx < bank->nr_pins; ++idx) {
<<<<<<< HEAD
=======
			struct device_node *usi_node;
			int usi_cnt, i;
			int used_for_usi = 0;

>>>>>>> upstream/android-13
			irq = irq_of_parse_and_map(bank->of_node, idx);
			if (!irq) {
				dev_err(dev, "irq number for eint-%s-%d not found\n",
							bank->name, idx);
				continue;
			}
<<<<<<< HEAD
=======

			if (!of_device_is_available(bank->of_node)) {
				dev_info(dev, "irq number for eint-%s disabled\n",
						bank->name);
				continue;
			}

			/*
			 * In case of USI and GPIO uses same irq,
			 * GPIO must not call irq_set_chained_handler_and_data.
			 * To do this, add "usi = <&hsi2c_n>".
			 */
			usi_cnt = of_count_phandle_with_args(bank->of_node, "usi", NULL);
			for (i = 0; i < usi_cnt; ++i) {
				usi_node = of_parse_phandle(bank->of_node, "usi", i);
				if (usi_node && of_device_is_available(usi_node)) {
					dev_info(dev, "irq request for eint-%s is SKIPPED. It is used for usi %s\n",
							bank->name, usi_node->full_name);
					used_for_usi = 1;
					continue;
				}
			}

			if (used_for_usi)
				continue;

>>>>>>> upstream/android-13
			weint_data[idx].irq = idx;
			weint_data[idx].bank = bank;
			irq_set_chained_handler_and_data(irq,
							 exynos_irq_eint0_15,
							 &weint_data[idx]);
		}
	}

<<<<<<< HEAD
=======
	/* Initialize to 0 to find the base of wakeup source
	 * other than ALIVE GPIO.
	 */
	eint_num = 0;

	d->sysreg_cmgp = syscon_regmap_lookup_by_phandle(dev->of_node,
			"samsung,syscon-phandle");
	if (IS_ERR(d->sysreg_cmgp)) {
		dev_info(dev, "sysreg_cmgp is not registered\n");
		d->sysreg_cmgp = 0;
	}

>>>>>>> upstream/android-13
	if (!muxed_banks) {
		of_node_put(wkup_np);
		return 0;
	}

	irq = irq_of_parse_and_map(wkup_np, 0);
	of_node_put(wkup_np);
	if (!irq) {
		dev_err(dev, "irq number for muxed EINTs not found\n");
		return 0;
	}

	muxed_data = devm_kzalloc(dev, sizeof(*muxed_data)
		+ muxed_banks*sizeof(struct samsung_pin_bank *), GFP_KERNEL);
	if (!muxed_data)
		return -ENOMEM;

	irq_set_chained_handler_and_data(irq, exynos_irq_demux_eint16_31,
					 muxed_data);

	bank = d->pin_banks;
	idx = 0;
	for (i = 0; i < d->nr_banks; ++i, ++bank) {
		if (bank->eint_type != EINT_TYPE_WKUP_MUX)
			continue;

		muxed_data->banks[idx++] = bank;
	}
	muxed_data->nr_banks = muxed_banks;

	return 0;
}

static void exynos_pinctrl_suspend_bank(
				struct samsung_pinctrl_drv_data *drvdata,
				struct samsung_pin_bank *bank)
{
	struct exynos_eint_gpio_save *save = bank->soc_priv;
	void __iomem *regs = bank->eint_base;

	save->eint_con = readl(regs + EXYNOS_GPIO_ECON_OFFSET
						+ bank->eint_offset);
<<<<<<< HEAD
	save->eint_fltcon0 = readl(regs + EXYNOS_GPIO_EFLTCON_OFFSET
						+ 2 * bank->eint_offset);
	save->eint_fltcon1 = readl(regs + EXYNOS_GPIO_EFLTCON_OFFSET
						+ 2 * bank->eint_offset + 4);
	save->eint_mask = readl(regs + bank->irq_chip->eint_mask
						+ bank->eint_offset);

	pr_debug("%s: save     con %#010x\n", bank->name, save->eint_con);
	pr_debug("%s: save fltcon0 %#010x\n", bank->name, save->eint_fltcon0);
	pr_debug("%s: save fltcon1 %#010x\n", bank->name, save->eint_fltcon1);
	pr_debug("%s: save    mask %#010x\n", bank->name, save->eint_mask);
=======

	save->eint_mask = readl(regs + bank->irq_chip->eint_mask
						+ bank->eint_offset);

	save->eint_fltcon0 = readl(regs + EXYNOS_GPIO_EFLTCON_OFFSET
						+ bank->fltcon_offset);
	if (bank->nr_pins > 4)
		save->eint_fltcon1 = readl(regs + EXYNOS_GPIO_EFLTCON_OFFSET
							+ bank->fltcon_offset + 4);

	pr_debug("%s: save     con %#010x\n", bank->name, save->eint_con);
	pr_debug("%s: save    mask %#010x\n", bank->name, save->eint_mask);
	pr_debug("%s: save fltcon0 %#010x\n", bank->name, save->eint_fltcon0);
	if (bank->nr_pins > 4)
		pr_debug("%s: save fltcon1 %#010x\n", bank->name, save->eint_fltcon1);
>>>>>>> upstream/android-13
}

void exynos_pinctrl_suspend(struct samsung_pinctrl_drv_data *drvdata)
{
	struct samsung_pin_bank *bank = drvdata->pin_banks;
	struct exynos_irq_chip *irq_chip = NULL;
<<<<<<< HEAD
=======
	struct samsung_pinctrl_drv_data *d = bank->drvdata;
>>>>>>> upstream/android-13
	int i;

	for (i = 0; i < drvdata->nr_banks; ++i, ++bank) {
		if (bank->eint_type == EINT_TYPE_GPIO)
			exynos_pinctrl_suspend_bank(drvdata, bank);
<<<<<<< HEAD
		else if (bank->eint_type == EINT_TYPE_WKUP) {
=======
		else if (bank->eint_type == EINT_TYPE_WKUP ||
			bank->eint_type == EINT_TYPE_WKUP_MUX) {
			/* Setting Analog Filter */
			exynos_eint_flt_config(EXYNOS_EINT_FLTCON_EN,
					0, 0, d, bank);

>>>>>>> upstream/android-13
			if (!irq_chip) {
				irq_chip = bank->irq_chip;
				irq_chip->set_eint_wakeup_mask(drvdata,
							       irq_chip);
<<<<<<< HEAD
			} else if (bank->irq_chip != irq_chip) {
				dev_warn(drvdata->dev,
					 "More than one external wakeup interrupt chip configured (bank: %s). This is not supported by hardware nor by driver.\n",
					 bank->name);
=======
>>>>>>> upstream/android-13
			}
		}
	}
}

static void exynos_pinctrl_resume_bank(
				struct samsung_pinctrl_drv_data *drvdata,
				struct samsung_pin_bank *bank)
{
	struct exynos_eint_gpio_save *save = bank->soc_priv;
	void __iomem *regs = bank->eint_base;

	pr_debug("%s:     con %#010x => %#010x\n", bank->name,
			readl(regs + EXYNOS_GPIO_ECON_OFFSET
			+ bank->eint_offset), save->eint_con);
<<<<<<< HEAD
	pr_debug("%s: fltcon0 %#010x => %#010x\n", bank->name,
			readl(regs + EXYNOS_GPIO_EFLTCON_OFFSET
			+ 2 * bank->eint_offset), save->eint_fltcon0);
	pr_debug("%s: fltcon1 %#010x => %#010x\n", bank->name,
			readl(regs + EXYNOS_GPIO_EFLTCON_OFFSET
			+ 2 * bank->eint_offset + 4), save->eint_fltcon1);
	pr_debug("%s:    mask %#010x => %#010x\n", bank->name,
			readl(regs + bank->irq_chip->eint_mask
			+ bank->eint_offset), save->eint_mask);

	writel(save->eint_con, regs + EXYNOS_GPIO_ECON_OFFSET
						+ bank->eint_offset);
	writel(save->eint_fltcon0, regs + EXYNOS_GPIO_EFLTCON_OFFSET
						+ 2 * bank->eint_offset);
	writel(save->eint_fltcon1, regs + EXYNOS_GPIO_EFLTCON_OFFSET
						+ 2 * bank->eint_offset + 4);
	writel(save->eint_mask, regs + bank->irq_chip->eint_mask
						+ bank->eint_offset);
=======
	pr_debug("%s:    mask %#010x => %#010x\n", bank->name,
			readl(regs + bank->irq_chip->eint_mask
			+ bank->eint_offset), save->eint_mask);
	pr_debug("%s: fltcon0 %#010x => %#010x\n", bank->name,
			readl(regs + EXYNOS_GPIO_EFLTCON_OFFSET
			+ bank->fltcon_offset), save->eint_fltcon0);
	if (bank->nr_pins > 4) {
	pr_debug("%s: fltcon1 %#010x => %#010x\n", bank->name,
			readl(regs + EXYNOS_GPIO_EFLTCON_OFFSET
			+ bank->fltcon_offset + 4), save->eint_fltcon1);
	}

	writel(save->eint_con, regs + EXYNOS_GPIO_ECON_OFFSET
						+ bank->eint_offset);
	writel(save->eint_mask, regs + bank->irq_chip->eint_mask
						+ bank->eint_offset);
	writel(save->eint_fltcon0, regs + EXYNOS_GPIO_EFLTCON_OFFSET
						+ bank->fltcon_offset);
	if (bank->nr_pins > 4) {
		writel(save->eint_fltcon1, regs + EXYNOS_GPIO_EFLTCON_OFFSET
							+ bank->fltcon_offset + 4);
	}
>>>>>>> upstream/android-13
}

void exynos_pinctrl_resume(struct samsung_pinctrl_drv_data *drvdata)
{
	struct samsung_pin_bank *bank = drvdata->pin_banks;
<<<<<<< HEAD
	int i;

	for (i = 0; i < drvdata->nr_banks; ++i, ++bank)
		if (bank->eint_type == EINT_TYPE_GPIO)
			exynos_pinctrl_resume_bank(drvdata, bank);
=======
	struct samsung_pinctrl_drv_data *d = bank->drvdata;
	int i;

	for (i = 0; i < drvdata->nr_banks; ++i, ++bank) {
		if (bank->eint_type == EINT_TYPE_GPIO) {
			exynos_pinctrl_resume_bank(drvdata, bank);
		} else if (bank->eint_type == EINT_TYPE_WKUP ||
			bank->eint_type == EINT_TYPE_WKUP_MUX) {
			/* Only alive block has filter selection register. */
			/* Setting Digital Filter */
			exynos_eint_flt_config(EXYNOS_EINT_FLTCON_EN,
					EXYNOS_EINT_FLTCON_SEL, 0, d, bank);
		}
	}
>>>>>>> upstream/android-13
}

static void exynos_retention_enable(struct samsung_pinctrl_drv_data *drvdata)
{
	if (drvdata->retention_ctrl->refcnt)
		atomic_inc(drvdata->retention_ctrl->refcnt);
}

static void exynos_retention_disable(struct samsung_pinctrl_drv_data *drvdata)
{
	struct samsung_retention_ctrl *ctrl = drvdata->retention_ctrl;
	struct regmap *pmu_regs = ctrl->priv;
	int i;

	if (ctrl->refcnt && !atomic_dec_and_test(ctrl->refcnt))
		return;

	for (i = 0; i < ctrl->nr_regs; i++)
		regmap_write(pmu_regs, ctrl->regs[i], ctrl->value);
}

struct samsung_retention_ctrl *
exynos_retention_init(struct samsung_pinctrl_drv_data *drvdata,
		      const struct samsung_retention_data *data)
{
	struct samsung_retention_ctrl *ctrl;
	struct regmap *pmu_regs;
	int i;

	ctrl = devm_kzalloc(drvdata->dev, sizeof(*ctrl), GFP_KERNEL);
	if (!ctrl)
		return ERR_PTR(-ENOMEM);

	pmu_regs = exynos_get_pmu_regmap();
	if (IS_ERR(pmu_regs))
		return ERR_CAST(pmu_regs);

	ctrl->priv = pmu_regs;
	ctrl->regs = data->regs;
	ctrl->nr_regs = data->nr_regs;
	ctrl->value = data->value;
	ctrl->refcnt = data->refcnt;
	ctrl->enable = exynos_retention_enable;
	ctrl->disable = exynos_retention_disable;

	/* Ensure that retention is disabled on driver init */
	for (i = 0; i < ctrl->nr_regs; i++)
		regmap_write(pmu_regs, ctrl->regs[i], ctrl->value);

	return ctrl;
}
<<<<<<< HEAD
=======
MODULE_LICENSE("GPL");
>>>>>>> upstream/android-13
