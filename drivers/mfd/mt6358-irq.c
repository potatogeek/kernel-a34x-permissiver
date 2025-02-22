// SPDX-License-Identifier: GPL-2.0
//
<<<<<<< HEAD
// Copyright (c) 2019 MediaTek Inc.

#include <linux/interrupt.h>
#include <linux/mfd/mt6357/core.h>
#include <linux/mfd/mt6357/registers.h>
=======
// Copyright (c) 2020 MediaTek Inc.

#include <linux/interrupt.h>
>>>>>>> upstream/android-13
#include <linux/mfd/mt6358/core.h>
#include <linux/mfd/mt6358/registers.h>
#include <linux/mfd/mt6359/core.h>
#include <linux/mfd/mt6359/registers.h>
#include <linux/mfd/mt6397/core.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/of_device.h>
#include <linux/of_irq.h>
#include <linux/platform_device.h>
#include <linux/regmap.h>
<<<<<<< HEAD
#include <linux/wakeup_reason.h>

#define DEBUG_PMIC_IRQ	1

struct irq_top_t {
	int hwirq_base;
	unsigned int num_int_regs;
	unsigned int en_reg;
	unsigned int en_reg_shift;
	unsigned int sta_reg;
	unsigned int sta_reg_shift;
	unsigned int top_offset;
};

#if DEBUG_PMIC_IRQ
struct irq_name_t {
	const char *name;
};

static struct irq_name_t mt6357_irq_name[] = MT6357_IRQ_NAME_GEN();
static struct irq_name_t mt6358_irq_name[] = MT6358_IRQ_NAME_GEN();
static struct irq_name_t mt6359_irq_name[] = MT6359_IRQ_NAME_GEN();
#endif

struct pmic_irq_data {
	unsigned int num_top;
	unsigned int num_pmic_irqs;
	unsigned int reg_width;
	unsigned short top_int_status_reg;
	bool *enable_hwirq;
	bool *cache_hwirq;
	struct irq_top_t *pmic_ints;
#if DEBUG_PMIC_IRQ
	struct irq_name_t *irq_name;
#endif
};

static struct irq_top_t mt6357_ints[] = {
	MT6357_TOP_GEN(BUCK),
	MT6357_TOP_GEN(LDO),
	MT6357_TOP_GEN(PSC),
	MT6357_TOP_GEN(SCK),
	MT6357_TOP_GEN(BM),
	MT6357_TOP_GEN(HK),
	MT6357_TOP_GEN(AUD),
	MT6357_TOP_GEN(MISC),
};

static struct irq_top_t mt6358_ints[] = {
=======

#define MTK_PMIC_REG_WIDTH 16

static const struct irq_top_t mt6358_ints[] = {
>>>>>>> upstream/android-13
	MT6358_TOP_GEN(BUCK),
	MT6358_TOP_GEN(LDO),
	MT6358_TOP_GEN(PSC),
	MT6358_TOP_GEN(SCK),
	MT6358_TOP_GEN(BM),
	MT6358_TOP_GEN(HK),
	MT6358_TOP_GEN(AUD),
	MT6358_TOP_GEN(MISC),
};

<<<<<<< HEAD
static struct irq_top_t mt6359_ints[] = {
=======
static const struct irq_top_t mt6359_ints[] = {
>>>>>>> upstream/android-13
	MT6359_TOP_GEN(BUCK),
	MT6359_TOP_GEN(LDO),
	MT6359_TOP_GEN(PSC),
	MT6359_TOP_GEN(SCK),
	MT6359_TOP_GEN(BM),
	MT6359_TOP_GEN(HK),
	MT6359_TOP_GEN(AUD),
	MT6359_TOP_GEN(MISC),
};

<<<<<<< HEAD
=======
static struct pmic_irq_data mt6358_irqd = {
	.num_top = ARRAY_SIZE(mt6358_ints),
	.num_pmic_irqs = MT6358_IRQ_NR,
	.top_int_status_reg = MT6358_TOP_INT_STATUS0,
	.pmic_ints = mt6358_ints,
};

static struct pmic_irq_data mt6359_irqd = {
	.num_top = ARRAY_SIZE(mt6359_ints),
	.num_pmic_irqs = MT6359_IRQ_NR,
	.top_int_status_reg = MT6359_TOP_INT_STATUS0,
	.pmic_ints = mt6359_ints,
};

>>>>>>> upstream/android-13
static void pmic_irq_enable(struct irq_data *data)
{
	unsigned int hwirq = irqd_to_hwirq(data);
	struct mt6397_chip *chip = irq_data_get_irq_chip_data(data);
	struct pmic_irq_data *irqd = chip->irq_data;

	irqd->enable_hwirq[hwirq] = true;
}

static void pmic_irq_disable(struct irq_data *data)
{
	unsigned int hwirq = irqd_to_hwirq(data);
	struct mt6397_chip *chip = irq_data_get_irq_chip_data(data);
	struct pmic_irq_data *irqd = chip->irq_data;

	irqd->enable_hwirq[hwirq] = false;
}

static void pmic_irq_lock(struct irq_data *data)
{
	struct mt6397_chip *chip = irq_data_get_irq_chip_data(data);

	mutex_lock(&chip->irqlock);
}

static void pmic_irq_sync_unlock(struct irq_data *data)
{
<<<<<<< HEAD
	unsigned int i, top_gp, en_reg, int_regs, shift;
=======
	unsigned int i, top_gp, gp_offset, en_reg, int_regs, shift;
>>>>>>> upstream/android-13
	struct mt6397_chip *chip = irq_data_get_irq_chip_data(data);
	struct pmic_irq_data *irqd = chip->irq_data;

	for (i = 0; i < irqd->num_pmic_irqs; i++) {
		if (irqd->enable_hwirq[i] == irqd->cache_hwirq[i])
			continue;

<<<<<<< HEAD
		top_gp = 0;
		while ((top_gp + 1) < irqd->num_top &&
			i >= irqd->pmic_ints[top_gp + 1].hwirq_base)
			top_gp++;

		if (top_gp >= irqd->num_top) {
			mutex_unlock(&chip->irqlock);
			dev_err(chip->dev,
				"Failed to get top_group: %d\n", top_gp);
			return;
		}

		int_regs = (i - irqd->pmic_ints[top_gp].hwirq_base) /
			    irqd->reg_width;
		en_reg = irqd->pmic_ints[top_gp].en_reg +
			irqd->pmic_ints[top_gp].en_reg_shift * int_regs;
		shift = (i - irqd->pmic_ints[top_gp].hwirq_base) %
			irqd->reg_width;
		regmap_update_bits(chip->regmap, en_reg, BIT(shift),
				   irqd->enable_hwirq[i] << shift);
=======
		/* Find out the IRQ group */
		top_gp = 0;
		while ((top_gp + 1) < irqd->num_top &&
		       i >= irqd->pmic_ints[top_gp + 1].hwirq_base)
			top_gp++;

		/* Find the IRQ registers */
		gp_offset = i - irqd->pmic_ints[top_gp].hwirq_base;
		int_regs = gp_offset / MTK_PMIC_REG_WIDTH;
		shift = gp_offset % MTK_PMIC_REG_WIDTH;
		en_reg = irqd->pmic_ints[top_gp].en_reg +
			 (irqd->pmic_ints[top_gp].en_reg_shift * int_regs);

		regmap_update_bits(chip->regmap, en_reg, BIT(shift),
				   irqd->enable_hwirq[i] << shift);

>>>>>>> upstream/android-13
		irqd->cache_hwirq[i] = irqd->enable_hwirq[i];
	}
	mutex_unlock(&chip->irqlock);
}

static struct irq_chip mt6358_irq_chip = {
	.name = "mt6358-irq",
	.flags = IRQCHIP_SKIP_SET_WAKE,
	.irq_enable = pmic_irq_enable,
	.irq_disable = pmic_irq_disable,
	.irq_bus_lock = pmic_irq_lock,
	.irq_bus_sync_unlock = pmic_irq_sync_unlock,
};

static void mt6358_irq_sp_handler(struct mt6397_chip *chip,
				  unsigned int top_gp)
{
<<<<<<< HEAD
	unsigned int sta_reg, irq_status = 0;
	unsigned int hwirq, virq;
	int ret, i, j;
=======
	unsigned int irq_status, sta_reg, status;
	unsigned int hwirq, virq;
	int i, j, ret;
>>>>>>> upstream/android-13
	struct pmic_irq_data *irqd = chip->irq_data;

	for (i = 0; i < irqd->pmic_ints[top_gp].num_int_regs; i++) {
		sta_reg = irqd->pmic_ints[top_gp].sta_reg +
			irqd->pmic_ints[top_gp].sta_reg_shift * i;
<<<<<<< HEAD
		ret = regmap_read(chip->regmap, sta_reg, &irq_status);
		if (ret) {
			dev_err(chip->dev,
				"Failed to read irq status: %d\n", ret);
=======

		ret = regmap_read(chip->regmap, sta_reg, &irq_status);
		if (ret) {
			dev_err(chip->dev,
				"Failed to read IRQ status, ret=%d\n", ret);
>>>>>>> upstream/android-13
			return;
		}

		if (!irq_status)
			continue;

<<<<<<< HEAD
		for (j = 0; j < irqd->reg_width ; j++) {
			if ((irq_status & BIT(j)) == 0)
				continue;
			hwirq = irqd->pmic_ints[top_gp].hwirq_base +
				irqd->reg_width * i + j;
			virq = irq_find_mapping(chip->irq_domain, hwirq);
#if DEBUG_PMIC_IRQ
			dev_info(chip->dev,
				"Reg[0x%x]=0x%x,name=%s,hwirq=%d,type=%d\n",
				sta_reg, irq_status,
				irqd->irq_name[hwirq], hwirq,
				irq_get_trigger_type(virq));
			if (!strncmp(irqd->irq_name[hwirq].name, "chrdet_edge", 11)) {
				regmap_write(chip->regmap, sta_reg, BIT(j));
				irq_status &= ~BIT(j);
			}
			log_threaded_irq_wakeup_reason(virq, chip->irq);
#endif
#ifdef CONFIG_SEC_PM
			log_threaded_irq_wakeup_reason(virq, chip->irq);
#endif /* CONFIG_SEC_PM  */
			if (virq)
				handle_nested_irq(virq);
		}
=======
		status = irq_status;
		do {
			j = __ffs(status);

			hwirq = irqd->pmic_ints[top_gp].hwirq_base +
				MTK_PMIC_REG_WIDTH * i + j;

			virq = irq_find_mapping(chip->irq_domain, hwirq);
			if (virq)
				handle_nested_irq(virq);

			status &= ~BIT(j);
		} while (status);
>>>>>>> upstream/android-13

		regmap_write(chip->regmap, sta_reg, irq_status);
	}
}

static irqreturn_t mt6358_irq_handler(int irq, void *data)
{
	struct mt6397_chip *chip = data;
	struct pmic_irq_data *irqd = chip->irq_data;
<<<<<<< HEAD
	unsigned int top_irq_status = 0;
	unsigned int i = 0;
	int ret = 0;

	pm_stay_awake(chip->dev);
=======
	unsigned int bit, i, top_irq_status = 0;
	int ret;

>>>>>>> upstream/android-13
	ret = regmap_read(chip->regmap,
			  irqd->top_int_status_reg,
			  &top_irq_status);
	if (ret) {
<<<<<<< HEAD
		pm_relax(chip->dev);
		dev_err(chip->dev, "Can't read TOP_INT_STATUS ret=%d\n", ret);
=======
		dev_err(chip->dev,
			"Failed to read status from the device, ret=%d\n", ret);
>>>>>>> upstream/android-13
		return IRQ_NONE;
	}

	for (i = 0; i < irqd->num_top; i++) {
<<<<<<< HEAD
		if (top_irq_status & BIT(irqd->pmic_ints[i].top_offset))
			mt6358_irq_sp_handler(chip, i);
	}
	pm_relax(chip->dev);
=======
		bit = BIT(irqd->pmic_ints[i].top_offset);
		if (top_irq_status & bit) {
			mt6358_irq_sp_handler(chip, i);
			top_irq_status &= ~bit;
			if (!top_irq_status)
				break;
		}
	}

>>>>>>> upstream/android-13
	return IRQ_HANDLED;
}

static int pmic_irq_domain_map(struct irq_domain *d, unsigned int irq,
			       irq_hw_number_t hw)
{
	struct mt6397_chip *mt6397 = d->host_data;

	irq_set_chip_data(irq, mt6397);
	irq_set_chip_and_handler(irq, &mt6358_irq_chip, handle_level_irq);
	irq_set_nested_thread(irq, 1);
	irq_set_noprobe(irq);

	return 0;
}

static const struct irq_domain_ops mt6358_irq_domain_ops = {
	.map = pmic_irq_domain_map,
	.xlate = irq_domain_xlate_twocell,
};

int mt6358_irq_init(struct mt6397_chip *chip)
{
	int i, j, ret;
	struct pmic_irq_data *irqd;

<<<<<<< HEAD
	irqd = devm_kzalloc(chip->dev, sizeof(*irqd), GFP_KERNEL);
	if (!irqd)
		return -ENOMEM;

	chip->irq_data = irqd;

	mutex_init(&chip->irqlock);
	switch (chip->chip_id) {
	case MT6357_CHIP_ID:
		irqd->num_top = ARRAY_SIZE(mt6357_ints);
		irqd->num_pmic_irqs = MT6357_IRQ_NR;
		irqd->reg_width = MT6357_REG_WIDTH;
		irqd->top_int_status_reg = MT6357_TOP_INT_STATUS0;
		irqd->pmic_ints = mt6357_ints;
#if DEBUG_PMIC_IRQ
		irqd->irq_name = mt6357_irq_name;
#endif
		break;
	case MT6358_CHIP_ID:
		irqd->num_top = ARRAY_SIZE(mt6358_ints);
		irqd->num_pmic_irqs = MT6358_IRQ_NR;
		irqd->reg_width = MT6358_REG_WIDTH;
		irqd->top_int_status_reg = MT6358_TOP_INT_STATUS0;
		irqd->pmic_ints = mt6358_ints;
#if DEBUG_PMIC_IRQ
		irqd->irq_name = mt6358_irq_name;
#endif
		break;
	case MT6359_CHIP_ID:
		irqd->num_top = ARRAY_SIZE(mt6359_ints);
		irqd->num_pmic_irqs = MT6359_IRQ_NR;
		irqd->reg_width = MT6359_REG_WIDTH;
		irqd->top_int_status_reg = MT6359_TOP_INT_STATUS0;
		irqd->pmic_ints = mt6359_ints;
#if DEBUG_PMIC_IRQ
		irqd->irq_name = mt6359_irq_name;
#endif
		break;
	default:
		dev_err(chip->dev, "unsupported chip: 0x%x\n", chip->chip_id);
		ret = -ENODEV;
		break;
	}

	irqd->enable_hwirq = devm_kcalloc(chip->dev,
					  irqd->num_pmic_irqs,
					  sizeof(bool),
=======
	switch (chip->chip_id) {
	case MT6358_CHIP_ID:
		chip->irq_data = &mt6358_irqd;
		break;

	case MT6359_CHIP_ID:
		chip->irq_data = &mt6359_irqd;
		break;

	default:
		dev_err(chip->dev, "unsupported chip: 0x%x\n", chip->chip_id);
		return -ENODEV;
	}

	mutex_init(&chip->irqlock);
	irqd = chip->irq_data;
	irqd->enable_hwirq = devm_kcalloc(chip->dev,
					  irqd->num_pmic_irqs,
					  sizeof(*irqd->enable_hwirq),
>>>>>>> upstream/android-13
					  GFP_KERNEL);
	if (!irqd->enable_hwirq)
		return -ENOMEM;

	irqd->cache_hwirq = devm_kcalloc(chip->dev,
					 irqd->num_pmic_irqs,
<<<<<<< HEAD
					 sizeof(bool),
=======
					 sizeof(*irqd->cache_hwirq),
>>>>>>> upstream/android-13
					 GFP_KERNEL);
	if (!irqd->cache_hwirq)
		return -ENOMEM;

<<<<<<< HEAD
	/* Disable all interrupt for initializing */
=======
	/* Disable all interrupts for initializing */
>>>>>>> upstream/android-13
	for (i = 0; i < irqd->num_top; i++) {
		for (j = 0; j < irqd->pmic_ints[i].num_int_regs; j++)
			regmap_write(chip->regmap,
				     irqd->pmic_ints[i].en_reg +
				     irqd->pmic_ints[i].en_reg_shift * j, 0);
	}

	chip->irq_domain = irq_domain_add_linear(chip->dev->of_node,
						 irqd->num_pmic_irqs,
						 &mt6358_irq_domain_ops, chip);
	if (!chip->irq_domain) {
<<<<<<< HEAD
		dev_err(chip->dev, "could not create irq domain\n");
=======
		dev_err(chip->dev, "Could not create IRQ domain\n");
>>>>>>> upstream/android-13
		return -ENODEV;
	}

	ret = devm_request_threaded_irq(chip->dev, chip->irq, NULL,
<<<<<<< HEAD
					mt6358_irq_handler,
					IRQF_ONESHOT,
					mt6358_irq_chip.name, chip);
	if (ret) {
		dev_err(chip->dev, "failed to register irq=%d; err: %d\n",
=======
					mt6358_irq_handler, IRQF_ONESHOT,
					mt6358_irq_chip.name, chip);
	if (ret) {
		dev_err(chip->dev, "Failed to register IRQ=%d, ret=%d\n",
>>>>>>> upstream/android-13
			chip->irq, ret);
		return ret;
	}

	enable_irq_wake(chip->irq);
	return ret;
}
