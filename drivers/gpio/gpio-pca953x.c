<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> upstream/android-13
/*
 *  PCA953x 4/8/16/24/40 bit I/O ports
 *
 *  Copyright (C) 2005 Ben Gardner <bgardner@wabtec.com>
 *  Copyright (C) 2007 Marvell International Ltd.
 *
 *  Derived from drivers/i2c/chips/pca9539.c
<<<<<<< HEAD
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; version 2 of the License.
 */

#include <linux/acpi.h>
=======
 */

#include <linux/acpi.h>
#include <linux/bitmap.h>
>>>>>>> upstream/android-13
#include <linux/gpio/driver.h>
#include <linux/gpio/consumer.h>
#include <linux/i2c.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/module.h>
#include <linux/of_platform.h>
#include <linux/platform_data/pca953x.h>
<<<<<<< HEAD
=======
#include <linux/regmap.h>
>>>>>>> upstream/android-13
#include <linux/regulator/consumer.h>
#include <linux/slab.h>

#include <asm/unaligned.h>

#define PCA953X_INPUT		0x00
#define PCA953X_OUTPUT		0x01
#define PCA953X_INVERT		0x02
#define PCA953X_DIRECTION	0x03

<<<<<<< HEAD
#define REG_ADDR_AI		0x80
=======
#define REG_ADDR_MASK		GENMASK(5, 0)
#define REG_ADDR_EXT		BIT(6)
#define REG_ADDR_AI		BIT(7)
>>>>>>> upstream/android-13

#define PCA957X_IN		0x00
#define PCA957X_INVRT		0x01
#define PCA957X_BKEN		0x02
#define PCA957X_PUPD		0x03
#define PCA957X_CFG		0x04
#define PCA957X_OUT		0x05
#define PCA957X_MSK		0x06
#define PCA957X_INTS		0x07

#define PCAL953X_OUT_STRENGTH	0x20
#define PCAL953X_IN_LATCH	0x22
#define PCAL953X_PULL_EN	0x23
#define PCAL953X_PULL_SEL	0x24
#define PCAL953X_INT_MASK	0x25
#define PCAL953X_INT_STAT	0x26
#define PCAL953X_OUT_CONF	0x27

#define PCAL6524_INT_EDGE	0x28
#define PCAL6524_INT_CLR	0x2a
#define PCAL6524_IN_STATUS	0x2b
#define PCAL6524_OUT_INDCONF	0x2c
#define PCAL6524_DEBOUNCE	0x2d

<<<<<<< HEAD
#define PCA_GPIO_MASK		0x00FF

#define PCAL_GPIO_MASK		0x1f
#define PCAL_PINCTRL_MASK	0x60

#define PCA_INT			0x0100
#define PCA_PCAL		0x0200
#define PCA_LATCH_INT (PCA_PCAL | PCA_INT)
#define PCA953X_TYPE		0x1000
#define PCA957X_TYPE		0x2000
#define PCA_TYPE_MASK		0xF000
=======
#define PCA_GPIO_MASK		GENMASK(7, 0)

#define PCAL_GPIO_MASK		GENMASK(4, 0)
#define PCAL_PINCTRL_MASK	GENMASK(6, 5)

#define PCA_INT			BIT(8)
#define PCA_PCAL		BIT(9)
#define PCA_LATCH_INT		(PCA_PCAL | PCA_INT)
#define PCA953X_TYPE		BIT(12)
#define PCA957X_TYPE		BIT(13)
#define PCA_TYPE_MASK		GENMASK(15, 12)
>>>>>>> upstream/android-13

#define PCA_CHIP_TYPE(x)	((x) & PCA_TYPE_MASK)

static const struct i2c_device_id pca953x_id[] = {
<<<<<<< HEAD
	{ "pca9505", 40 | PCA953X_TYPE | PCA_INT, },
=======
	{ "pca6416", 16 | PCA953X_TYPE | PCA_INT, },
	{ "pca9505", 40 | PCA953X_TYPE | PCA_INT, },
	{ "pca9506", 40 | PCA953X_TYPE | PCA_INT, },
>>>>>>> upstream/android-13
	{ "pca9534", 8  | PCA953X_TYPE | PCA_INT, },
	{ "pca9535", 16 | PCA953X_TYPE | PCA_INT, },
	{ "pca9536", 4  | PCA953X_TYPE, },
	{ "pca9537", 4  | PCA953X_TYPE | PCA_INT, },
	{ "pca9538", 8  | PCA953X_TYPE | PCA_INT, },
	{ "pca9539", 16 | PCA953X_TYPE | PCA_INT, },
	{ "pca9554", 8  | PCA953X_TYPE | PCA_INT, },
	{ "pca9555", 16 | PCA953X_TYPE | PCA_INT, },
	{ "pca9556", 8  | PCA953X_TYPE, },
	{ "pca9557", 8  | PCA953X_TYPE, },
	{ "pca9574", 8  | PCA957X_TYPE | PCA_INT, },
	{ "pca9575", 16 | PCA957X_TYPE | PCA_INT, },
	{ "pca9698", 40 | PCA953X_TYPE, },

<<<<<<< HEAD
	{ "pcal6524", 24 | PCA953X_TYPE | PCA_INT | PCA_PCAL, },
	{ "pcal9555a", 16 | PCA953X_TYPE | PCA_INT | PCA_PCAL, },
=======
	{ "pcal6416", 16 | PCA953X_TYPE | PCA_LATCH_INT, },
	{ "pcal6524", 24 | PCA953X_TYPE | PCA_LATCH_INT, },
	{ "pcal9535", 16 | PCA953X_TYPE | PCA_LATCH_INT, },
	{ "pcal9554b", 8  | PCA953X_TYPE | PCA_LATCH_INT, },
	{ "pcal9555a", 16 | PCA953X_TYPE | PCA_LATCH_INT, },
>>>>>>> upstream/android-13

	{ "max7310", 8  | PCA953X_TYPE, },
	{ "max7312", 16 | PCA953X_TYPE | PCA_INT, },
	{ "max7313", 16 | PCA953X_TYPE | PCA_INT, },
	{ "max7315", 8  | PCA953X_TYPE | PCA_INT, },
	{ "max7318", 16 | PCA953X_TYPE | PCA_INT, },
	{ "pca6107", 8  | PCA953X_TYPE | PCA_INT, },
	{ "tca6408", 8  | PCA953X_TYPE | PCA_INT, },
	{ "tca6416", 16 | PCA953X_TYPE | PCA_INT, },
	{ "tca6424", 24 | PCA953X_TYPE | PCA_INT, },
	{ "tca9539", 16 | PCA953X_TYPE | PCA_INT, },
	{ "tca9554", 8  | PCA953X_TYPE | PCA_INT, },
	{ "xra1202", 8  | PCA953X_TYPE },
	{ }
};
MODULE_DEVICE_TABLE(i2c, pca953x_id);

<<<<<<< HEAD
static const struct acpi_device_id pca953x_acpi_ids[] = {
	{ "INT3491", 16 | PCA953X_TYPE | PCA_INT | PCA_PCAL, },
=======
#ifdef CONFIG_GPIO_PCA953X_IRQ

#include <linux/dmi.h>

static const struct acpi_gpio_params pca953x_irq_gpios = { 0, 0, true };

static const struct acpi_gpio_mapping pca953x_acpi_irq_gpios[] = {
	{ "irq-gpios", &pca953x_irq_gpios, 1, ACPI_GPIO_QUIRK_ABSOLUTE_NUMBER },
	{ }
};

static int pca953x_acpi_get_irq(struct device *dev)
{
	int ret;

	ret = devm_acpi_dev_add_driver_gpios(dev, pca953x_acpi_irq_gpios);
	if (ret)
		dev_warn(dev, "can't add GPIO ACPI mapping\n");

	ret = acpi_dev_gpio_irq_get_by(ACPI_COMPANION(dev), "irq-gpios", 0);
	if (ret < 0)
		return ret;

	dev_info(dev, "ACPI interrupt quirk (IRQ %d)\n", ret);
	return ret;
}

static const struct dmi_system_id pca953x_dmi_acpi_irq_info[] = {
	{
		/*
		 * On Intel Galileo Gen 2 board the IRQ pin of one of
		 * the I²C GPIO expanders, which has GpioInt() resource,
		 * is provided as an absolute number instead of being
		 * relative. Since first controller (gpio-sch.c) and
		 * second (gpio-dwapb.c) are at the fixed bases, we may
		 * safely refer to the number in the global space to get
		 * an IRQ out of it.
		 */
		.matches = {
			DMI_EXACT_MATCH(DMI_BOARD_NAME, "GalileoGen2"),
		},
	},
	{}
};
#endif

static const struct acpi_device_id pca953x_acpi_ids[] = {
	{ "INT3491", 16 | PCA953X_TYPE | PCA_LATCH_INT, },
>>>>>>> upstream/android-13
	{ }
};
MODULE_DEVICE_TABLE(acpi, pca953x_acpi_ids);

#define MAX_BANK 5
#define BANK_SZ 8
<<<<<<< HEAD
=======
#define MAX_LINE	(MAX_BANK * BANK_SZ)
>>>>>>> upstream/android-13

#define NBANK(chip) DIV_ROUND_UP(chip->gpio_chip.ngpio, BANK_SZ)

struct pca953x_reg_config {
	int direction;
	int output;
	int input;
<<<<<<< HEAD
=======
	int invert;
>>>>>>> upstream/android-13
};

static const struct pca953x_reg_config pca953x_regs = {
	.direction = PCA953X_DIRECTION,
	.output = PCA953X_OUTPUT,
	.input = PCA953X_INPUT,
<<<<<<< HEAD
=======
	.invert = PCA953X_INVERT,
>>>>>>> upstream/android-13
};

static const struct pca953x_reg_config pca957x_regs = {
	.direction = PCA957X_CFG,
	.output = PCA957X_OUT,
	.input = PCA957X_IN,
<<<<<<< HEAD
=======
	.invert = PCA957X_INVRT,
>>>>>>> upstream/android-13
};

struct pca953x_chip {
	unsigned gpio_start;
<<<<<<< HEAD
	u8 reg_output[MAX_BANK];
	u8 reg_direction[MAX_BANK];
	struct mutex i2c_lock;

#ifdef CONFIG_GPIO_PCA953X_IRQ
	struct mutex irq_lock;
	u8 irq_mask[MAX_BANK];
	u8 irq_stat[MAX_BANK];
	u8 irq_trig_raise[MAX_BANK];
	u8 irq_trig_fall[MAX_BANK];
#endif
=======
	struct mutex i2c_lock;
	struct regmap *regmap;

#ifdef CONFIG_GPIO_PCA953X_IRQ
	struct mutex irq_lock;
	DECLARE_BITMAP(irq_mask, MAX_LINE);
	DECLARE_BITMAP(irq_stat, MAX_LINE);
	DECLARE_BITMAP(irq_trig_raise, MAX_LINE);
	DECLARE_BITMAP(irq_trig_fall, MAX_LINE);
	struct irq_chip irq_chip;
#endif
	atomic_t wakeup_path;
>>>>>>> upstream/android-13

	struct i2c_client *client;
	struct gpio_chip gpio_chip;
	const char *const *names;
	unsigned long driver_data;
	struct regulator *regulator;

	const struct pca953x_reg_config *regs;
<<<<<<< HEAD

	int (*write_regs)(struct pca953x_chip *, int, u8 *);
	int (*read_regs)(struct pca953x_chip *, int, u8 *);
};

static int pca953x_read_single(struct pca953x_chip *chip, int reg, u32 *val,
				int off)
{
	int ret;
	int bank_shift = fls((chip->gpio_chip.ngpio - 1) / BANK_SZ);
	int offset = off / BANK_SZ;

	ret = i2c_smbus_read_byte_data(chip->client,
				(reg << bank_shift) + offset);
	*val = ret;

	if (ret < 0) {
		dev_err(&chip->client->dev, "failed reading register\n");
		return ret;
	}

	return 0;
}

static int pca953x_write_single(struct pca953x_chip *chip, int reg, u32 val,
				int off)
{
	int ret;
	int bank_shift = fls((chip->gpio_chip.ngpio - 1) / BANK_SZ);
	int offset = off / BANK_SZ;

	ret = i2c_smbus_write_byte_data(chip->client,
					(reg << bank_shift) + offset, val);

=======
};

static int pca953x_bank_shift(struct pca953x_chip *chip)
{
	return fls((chip->gpio_chip.ngpio - 1) / BANK_SZ);
}

#define PCA953x_BANK_INPUT	BIT(0)
#define PCA953x_BANK_OUTPUT	BIT(1)
#define PCA953x_BANK_POLARITY	BIT(2)
#define PCA953x_BANK_CONFIG	BIT(3)

#define PCA957x_BANK_INPUT	BIT(0)
#define PCA957x_BANK_POLARITY	BIT(1)
#define PCA957x_BANK_BUSHOLD	BIT(2)
#define PCA957x_BANK_CONFIG	BIT(4)
#define PCA957x_BANK_OUTPUT	BIT(5)

#define PCAL9xxx_BANK_IN_LATCH	BIT(8 + 2)
#define PCAL9xxx_BANK_PULL_EN	BIT(8 + 3)
#define PCAL9xxx_BANK_PULL_SEL	BIT(8 + 4)
#define PCAL9xxx_BANK_IRQ_MASK	BIT(8 + 5)
#define PCAL9xxx_BANK_IRQ_STAT	BIT(8 + 6)

/*
 * We care about the following registers:
 * - Standard set, below 0x40, each port can be replicated up to 8 times
 *   - PCA953x standard
 *     Input port			0x00 + 0 * bank_size	R
 *     Output port			0x00 + 1 * bank_size	RW
 *     Polarity Inversion port		0x00 + 2 * bank_size	RW
 *     Configuration port		0x00 + 3 * bank_size	RW
 *   - PCA957x with mixed up registers
 *     Input port			0x00 + 0 * bank_size	R
 *     Polarity Inversion port		0x00 + 1 * bank_size	RW
 *     Bus hold port			0x00 + 2 * bank_size	RW
 *     Configuration port		0x00 + 4 * bank_size	RW
 *     Output port			0x00 + 5 * bank_size	RW
 *
 * - Extended set, above 0x40, often chip specific.
 *   - PCAL6524/PCAL9555A with custom PCAL IRQ handling:
 *     Input latch register		0x40 + 2 * bank_size	RW
 *     Pull-up/pull-down enable reg	0x40 + 3 * bank_size    RW
 *     Pull-up/pull-down select reg	0x40 + 4 * bank_size    RW
 *     Interrupt mask register		0x40 + 5 * bank_size	RW
 *     Interrupt status register	0x40 + 6 * bank_size	R
 *
 * - Registers with bit 0x80 set, the AI bit
 *   The bit is cleared and the registers fall into one of the
 *   categories above.
 */

static bool pca953x_check_register(struct pca953x_chip *chip, unsigned int reg,
				   u32 checkbank)
{
	int bank_shift = pca953x_bank_shift(chip);
	int bank = (reg & REG_ADDR_MASK) >> bank_shift;
	int offset = reg & (BIT(bank_shift) - 1);

	/* Special PCAL extended register check. */
	if (reg & REG_ADDR_EXT) {
		if (!(chip->driver_data & PCA_PCAL))
			return false;
		bank += 8;
	}

	/* Register is not in the matching bank. */
	if (!(BIT(bank) & checkbank))
		return false;

	/* Register is not within allowed range of bank. */
	if (offset >= NBANK(chip))
		return false;

	return true;
}

static bool pca953x_readable_register(struct device *dev, unsigned int reg)
{
	struct pca953x_chip *chip = dev_get_drvdata(dev);
	u32 bank;

	if (PCA_CHIP_TYPE(chip->driver_data) == PCA953X_TYPE) {
		bank = PCA953x_BANK_INPUT | PCA953x_BANK_OUTPUT |
		       PCA953x_BANK_POLARITY | PCA953x_BANK_CONFIG;
	} else {
		bank = PCA957x_BANK_INPUT | PCA957x_BANK_OUTPUT |
		       PCA957x_BANK_POLARITY | PCA957x_BANK_CONFIG |
		       PCA957x_BANK_BUSHOLD;
	}

	if (chip->driver_data & PCA_PCAL) {
		bank |= PCAL9xxx_BANK_IN_LATCH | PCAL9xxx_BANK_PULL_EN |
			PCAL9xxx_BANK_PULL_SEL | PCAL9xxx_BANK_IRQ_MASK |
			PCAL9xxx_BANK_IRQ_STAT;
	}

	return pca953x_check_register(chip, reg, bank);
}

static bool pca953x_writeable_register(struct device *dev, unsigned int reg)
{
	struct pca953x_chip *chip = dev_get_drvdata(dev);
	u32 bank;

	if (PCA_CHIP_TYPE(chip->driver_data) == PCA953X_TYPE) {
		bank = PCA953x_BANK_OUTPUT | PCA953x_BANK_POLARITY |
			PCA953x_BANK_CONFIG;
	} else {
		bank = PCA957x_BANK_OUTPUT | PCA957x_BANK_POLARITY |
			PCA957x_BANK_CONFIG | PCA957x_BANK_BUSHOLD;
	}

	if (chip->driver_data & PCA_PCAL)
		bank |= PCAL9xxx_BANK_IN_LATCH | PCAL9xxx_BANK_PULL_EN |
			PCAL9xxx_BANK_PULL_SEL | PCAL9xxx_BANK_IRQ_MASK;

	return pca953x_check_register(chip, reg, bank);
}

static bool pca953x_volatile_register(struct device *dev, unsigned int reg)
{
	struct pca953x_chip *chip = dev_get_drvdata(dev);
	u32 bank;

	if (PCA_CHIP_TYPE(chip->driver_data) == PCA953X_TYPE)
		bank = PCA953x_BANK_INPUT;
	else
		bank = PCA957x_BANK_INPUT;

	if (chip->driver_data & PCA_PCAL)
		bank |= PCAL9xxx_BANK_IRQ_STAT;

	return pca953x_check_register(chip, reg, bank);
}

static const struct regmap_config pca953x_i2c_regmap = {
	.reg_bits = 8,
	.val_bits = 8,

	.readable_reg = pca953x_readable_register,
	.writeable_reg = pca953x_writeable_register,
	.volatile_reg = pca953x_volatile_register,

	.disable_locking = true,
	.cache_type = REGCACHE_RBTREE,
	.max_register = 0x7f,
};

static const struct regmap_config pca953x_ai_i2c_regmap = {
	.reg_bits = 8,
	.val_bits = 8,

	.read_flag_mask = REG_ADDR_AI,
	.write_flag_mask = REG_ADDR_AI,

	.readable_reg = pca953x_readable_register,
	.writeable_reg = pca953x_writeable_register,
	.volatile_reg = pca953x_volatile_register,

	.disable_locking = true,
	.cache_type = REGCACHE_RBTREE,
	.max_register = 0x7f,
};

static u8 pca953x_recalc_addr(struct pca953x_chip *chip, int reg, int off)
{
	int bank_shift = pca953x_bank_shift(chip);
	int addr = (reg & PCAL_GPIO_MASK) << bank_shift;
	int pinctrl = (reg & PCAL_PINCTRL_MASK) << 1;
	u8 regaddr = pinctrl | addr | (off / BANK_SZ);

	return regaddr;
}

static int pca953x_write_regs(struct pca953x_chip *chip, int reg, unsigned long *val)
{
	u8 regaddr = pca953x_recalc_addr(chip, reg, 0);
	u8 value[MAX_BANK];
	int i, ret;

	for (i = 0; i < NBANK(chip); i++)
		value[i] = bitmap_get_value8(val, i * BANK_SZ);

	ret = regmap_bulk_write(chip->regmap, regaddr, value, NBANK(chip));
>>>>>>> upstream/android-13
	if (ret < 0) {
		dev_err(&chip->client->dev, "failed writing register\n");
		return ret;
	}

	return 0;
}

<<<<<<< HEAD
static int pca953x_write_regs_8(struct pca953x_chip *chip, int reg, u8 *val)
{
	return i2c_smbus_write_byte_data(chip->client, reg, *val);
}

static int pca953x_write_regs_16(struct pca953x_chip *chip, int reg, u8 *val)
{
	u16 word = get_unaligned((u16 *)val);

	return i2c_smbus_write_word_data(chip->client, reg << 1, word);
}

static int pca957x_write_regs_16(struct pca953x_chip *chip, int reg, u8 *val)
{
	int ret;

	ret = i2c_smbus_write_byte_data(chip->client, reg << 1, val[0]);
	if (ret < 0)
		return ret;

	return i2c_smbus_write_byte_data(chip->client, (reg << 1) + 1, val[1]);
}

static int pca953x_write_regs_24(struct pca953x_chip *chip, int reg, u8 *val)
{
	int bank_shift = fls((chip->gpio_chip.ngpio - 1) / BANK_SZ);
	int addr = (reg & PCAL_GPIO_MASK) << bank_shift;
	int pinctrl = (reg & PCAL_PINCTRL_MASK) << 1;

	return i2c_smbus_write_i2c_block_data(chip->client,
					      pinctrl | addr | REG_ADDR_AI,
					      NBANK(chip), val);
}

static int pca953x_write_regs(struct pca953x_chip *chip, int reg, u8 *val)
{
	int ret = 0;

	ret = chip->write_regs(chip, reg, val);
	if (ret < 0) {
		dev_err(&chip->client->dev, "failed writing register\n");
		return ret;
	}

	return 0;
}

static int pca953x_read_regs_8(struct pca953x_chip *chip, int reg, u8 *val)
{
	int ret;

	ret = i2c_smbus_read_byte_data(chip->client, reg);
	*val = ret;

	return ret;
}

static int pca953x_read_regs_16(struct pca953x_chip *chip, int reg, u8 *val)
{
	int ret;

	ret = i2c_smbus_read_word_data(chip->client, reg << 1);
	put_unaligned(ret, (u16 *)val);

	return ret;
}

static int pca953x_read_regs_24(struct pca953x_chip *chip, int reg, u8 *val)
{
	int bank_shift = fls((chip->gpio_chip.ngpio - 1) / BANK_SZ);
	int addr = (reg & PCAL_GPIO_MASK) << bank_shift;
	int pinctrl = (reg & PCAL_PINCTRL_MASK) << 1;

	return i2c_smbus_read_i2c_block_data(chip->client,
					     pinctrl | addr | REG_ADDR_AI,
					     NBANK(chip), val);
}

static int pca953x_read_regs(struct pca953x_chip *chip, int reg, u8 *val)
{
	int ret;

	ret = chip->read_regs(chip, reg, val);
=======
static int pca953x_read_regs(struct pca953x_chip *chip, int reg, unsigned long *val)
{
	u8 regaddr = pca953x_recalc_addr(chip, reg, 0);
	u8 value[MAX_BANK];
	int i, ret;

	ret = regmap_bulk_read(chip->regmap, regaddr, value, NBANK(chip));
>>>>>>> upstream/android-13
	if (ret < 0) {
		dev_err(&chip->client->dev, "failed reading register\n");
		return ret;
	}

<<<<<<< HEAD
=======
	for (i = 0; i < NBANK(chip); i++)
		bitmap_set_value8(val, value[i], i * BANK_SZ);

>>>>>>> upstream/android-13
	return 0;
}

static int pca953x_gpio_direction_input(struct gpio_chip *gc, unsigned off)
{
	struct pca953x_chip *chip = gpiochip_get_data(gc);
<<<<<<< HEAD
	u8 reg_val;
	int ret;

	mutex_lock(&chip->i2c_lock);
	reg_val = chip->reg_direction[off / BANK_SZ] | (1u << (off % BANK_SZ));

	ret = pca953x_write_single(chip, chip->regs->direction, reg_val, off);
	if (ret)
		goto exit;

	chip->reg_direction[off / BANK_SZ] = reg_val;
exit:
=======
	u8 dirreg = pca953x_recalc_addr(chip, chip->regs->direction, off);
	u8 bit = BIT(off % BANK_SZ);
	int ret;

	mutex_lock(&chip->i2c_lock);
	ret = regmap_write_bits(chip->regmap, dirreg, bit, bit);
>>>>>>> upstream/android-13
	mutex_unlock(&chip->i2c_lock);
	return ret;
}

static int pca953x_gpio_direction_output(struct gpio_chip *gc,
		unsigned off, int val)
{
	struct pca953x_chip *chip = gpiochip_get_data(gc);
<<<<<<< HEAD
	u8 reg_val;
=======
	u8 dirreg = pca953x_recalc_addr(chip, chip->regs->direction, off);
	u8 outreg = pca953x_recalc_addr(chip, chip->regs->output, off);
	u8 bit = BIT(off % BANK_SZ);
>>>>>>> upstream/android-13
	int ret;

	mutex_lock(&chip->i2c_lock);
	/* set output level */
<<<<<<< HEAD
	if (val)
		reg_val = chip->reg_output[off / BANK_SZ]
			| (1u << (off % BANK_SZ));
	else
		reg_val = chip->reg_output[off / BANK_SZ]
			& ~(1u << (off % BANK_SZ));

	ret = pca953x_write_single(chip, chip->regs->output, reg_val, off);
	if (ret)
		goto exit;

	chip->reg_output[off / BANK_SZ] = reg_val;

	/* then direction */
	reg_val = chip->reg_direction[off / BANK_SZ] & ~(1u << (off % BANK_SZ));
	ret = pca953x_write_single(chip, chip->regs->direction, reg_val, off);
	if (ret)
		goto exit;

	chip->reg_direction[off / BANK_SZ] = reg_val;
=======
	ret = regmap_write_bits(chip->regmap, outreg, bit, val ? bit : 0);
	if (ret)
		goto exit;

	/* then direction */
	ret = regmap_write_bits(chip->regmap, dirreg, bit, 0);
>>>>>>> upstream/android-13
exit:
	mutex_unlock(&chip->i2c_lock);
	return ret;
}

static int pca953x_gpio_get_value(struct gpio_chip *gc, unsigned off)
{
	struct pca953x_chip *chip = gpiochip_get_data(gc);
<<<<<<< HEAD
=======
	u8 inreg = pca953x_recalc_addr(chip, chip->regs->input, off);
	u8 bit = BIT(off % BANK_SZ);
>>>>>>> upstream/android-13
	u32 reg_val;
	int ret;

	mutex_lock(&chip->i2c_lock);
<<<<<<< HEAD
	ret = pca953x_read_single(chip, chip->regs->input, &reg_val, off);
	mutex_unlock(&chip->i2c_lock);
	if (ret < 0) {
		/* NOTE:  diagnostic already emitted; that's all we should
		 * do unless gpio_*_value_cansleep() calls become different
		 * from their nonsleeping siblings (and report faults).
		 */
		return 0;
	}

	return (reg_val & (1u << (off % BANK_SZ))) ? 1 : 0;
=======
	ret = regmap_read(chip->regmap, inreg, &reg_val);
	mutex_unlock(&chip->i2c_lock);
	if (ret < 0)
		return ret;

	return !!(reg_val & bit);
>>>>>>> upstream/android-13
}

static void pca953x_gpio_set_value(struct gpio_chip *gc, unsigned off, int val)
{
	struct pca953x_chip *chip = gpiochip_get_data(gc);
<<<<<<< HEAD
	u8 reg_val;
	int ret;

	mutex_lock(&chip->i2c_lock);
	if (val)
		reg_val = chip->reg_output[off / BANK_SZ]
			| (1u << (off % BANK_SZ));
	else
		reg_val = chip->reg_output[off / BANK_SZ]
			& ~(1u << (off % BANK_SZ));

	ret = pca953x_write_single(chip, chip->regs->output, reg_val, off);
	if (ret)
		goto exit;

	chip->reg_output[off / BANK_SZ] = reg_val;
exit:
=======
	u8 outreg = pca953x_recalc_addr(chip, chip->regs->output, off);
	u8 bit = BIT(off % BANK_SZ);

	mutex_lock(&chip->i2c_lock);
	regmap_write_bits(chip->regmap, outreg, bit, val ? bit : 0);
>>>>>>> upstream/android-13
	mutex_unlock(&chip->i2c_lock);
}

static int pca953x_gpio_get_direction(struct gpio_chip *gc, unsigned off)
{
	struct pca953x_chip *chip = gpiochip_get_data(gc);
<<<<<<< HEAD
=======
	u8 dirreg = pca953x_recalc_addr(chip, chip->regs->direction, off);
	u8 bit = BIT(off % BANK_SZ);
>>>>>>> upstream/android-13
	u32 reg_val;
	int ret;

	mutex_lock(&chip->i2c_lock);
<<<<<<< HEAD
	ret = pca953x_read_single(chip, chip->regs->direction, &reg_val, off);
=======
	ret = regmap_read(chip->regmap, dirreg, &reg_val);
>>>>>>> upstream/android-13
	mutex_unlock(&chip->i2c_lock);
	if (ret < 0)
		return ret;

<<<<<<< HEAD
	return !!(reg_val & (1u << (off % BANK_SZ)));
=======
	if (reg_val & bit)
		return GPIO_LINE_DIRECTION_IN;

	return GPIO_LINE_DIRECTION_OUT;
}

static int pca953x_gpio_get_multiple(struct gpio_chip *gc,
				     unsigned long *mask, unsigned long *bits)
{
	struct pca953x_chip *chip = gpiochip_get_data(gc);
	DECLARE_BITMAP(reg_val, MAX_LINE);
	int ret;

	mutex_lock(&chip->i2c_lock);
	ret = pca953x_read_regs(chip, chip->regs->input, reg_val);
	mutex_unlock(&chip->i2c_lock);
	if (ret)
		return ret;

	bitmap_replace(bits, bits, reg_val, mask, gc->ngpio);
	return 0;
>>>>>>> upstream/android-13
}

static void pca953x_gpio_set_multiple(struct gpio_chip *gc,
				      unsigned long *mask, unsigned long *bits)
{
	struct pca953x_chip *chip = gpiochip_get_data(gc);
<<<<<<< HEAD
	unsigned int bank_mask, bank_val;
	int bank_shift, bank;
	u8 reg_val[MAX_BANK];
	int ret;

	bank_shift = fls((chip->gpio_chip.ngpio - 1) / BANK_SZ);

	mutex_lock(&chip->i2c_lock);
	memcpy(reg_val, chip->reg_output, NBANK(chip));
	for (bank = 0; bank < NBANK(chip); bank++) {
		bank_mask = mask[bank / sizeof(*mask)] >>
			   ((bank % sizeof(*mask)) * 8);
		if (bank_mask) {
			bank_val = bits[bank / sizeof(*bits)] >>
				  ((bank % sizeof(*bits)) * 8);
			bank_val &= bank_mask;
			reg_val[bank] = (reg_val[bank] & ~bank_mask) | bank_val;
		}
	}

	ret = i2c_smbus_write_i2c_block_data(chip->client,
					     chip->regs->output << bank_shift,
					     NBANK(chip), reg_val);
	if (ret)
		goto exit;

	memcpy(chip->reg_output, reg_val, NBANK(chip));
=======
	DECLARE_BITMAP(reg_val, MAX_LINE);
	int ret;

	mutex_lock(&chip->i2c_lock);
	ret = pca953x_read_regs(chip, chip->regs->output, reg_val);
	if (ret)
		goto exit;

	bitmap_replace(reg_val, reg_val, bits, mask, gc->ngpio);

	pca953x_write_regs(chip, chip->regs->output, reg_val);
>>>>>>> upstream/android-13
exit:
	mutex_unlock(&chip->i2c_lock);
}

<<<<<<< HEAD
=======
static int pca953x_gpio_set_pull_up_down(struct pca953x_chip *chip,
					 unsigned int offset,
					 unsigned long config)
{
	u8 pull_en_reg = pca953x_recalc_addr(chip, PCAL953X_PULL_EN, offset);
	u8 pull_sel_reg = pca953x_recalc_addr(chip, PCAL953X_PULL_SEL, offset);
	u8 bit = BIT(offset % BANK_SZ);
	int ret;

	/*
	 * pull-up/pull-down configuration requires PCAL extended
	 * registers
	 */
	if (!(chip->driver_data & PCA_PCAL))
		return -ENOTSUPP;

	mutex_lock(&chip->i2c_lock);

	/* Configure pull-up/pull-down */
	if (config == PIN_CONFIG_BIAS_PULL_UP)
		ret = regmap_write_bits(chip->regmap, pull_sel_reg, bit, bit);
	else if (config == PIN_CONFIG_BIAS_PULL_DOWN)
		ret = regmap_write_bits(chip->regmap, pull_sel_reg, bit, 0);
	else
		ret = 0;
	if (ret)
		goto exit;

	/* Disable/Enable pull-up/pull-down */
	if (config == PIN_CONFIG_BIAS_DISABLE)
		ret = regmap_write_bits(chip->regmap, pull_en_reg, bit, 0);
	else
		ret = regmap_write_bits(chip->regmap, pull_en_reg, bit, bit);

exit:
	mutex_unlock(&chip->i2c_lock);
	return ret;
}

static int pca953x_gpio_set_config(struct gpio_chip *gc, unsigned int offset,
				   unsigned long config)
{
	struct pca953x_chip *chip = gpiochip_get_data(gc);

	switch (pinconf_to_config_param(config)) {
	case PIN_CONFIG_BIAS_PULL_UP:
	case PIN_CONFIG_BIAS_PULL_PIN_DEFAULT:
	case PIN_CONFIG_BIAS_PULL_DOWN:
	case PIN_CONFIG_BIAS_DISABLE:
		return pca953x_gpio_set_pull_up_down(chip, offset, config);
	default:
		return -ENOTSUPP;
	}
}

>>>>>>> upstream/android-13
static void pca953x_setup_gpio(struct pca953x_chip *chip, int gpios)
{
	struct gpio_chip *gc;

	gc = &chip->gpio_chip;

	gc->direction_input  = pca953x_gpio_direction_input;
	gc->direction_output = pca953x_gpio_direction_output;
	gc->get = pca953x_gpio_get_value;
	gc->set = pca953x_gpio_set_value;
	gc->get_direction = pca953x_gpio_get_direction;
<<<<<<< HEAD
	gc->set_multiple = pca953x_gpio_set_multiple;
=======
	gc->get_multiple = pca953x_gpio_get_multiple;
	gc->set_multiple = pca953x_gpio_set_multiple;
	gc->set_config = pca953x_gpio_set_config;
>>>>>>> upstream/android-13
	gc->can_sleep = true;

	gc->base = chip->gpio_start;
	gc->ngpio = gpios;
<<<<<<< HEAD
	gc->label = chip->client->name;
=======
	gc->label = dev_name(&chip->client->dev);
>>>>>>> upstream/android-13
	gc->parent = &chip->client->dev;
	gc->owner = THIS_MODULE;
	gc->names = chip->names;
}

#ifdef CONFIG_GPIO_PCA953X_IRQ
static void pca953x_irq_mask(struct irq_data *d)
{
	struct gpio_chip *gc = irq_data_get_irq_chip_data(d);
	struct pca953x_chip *chip = gpiochip_get_data(gc);
<<<<<<< HEAD

	chip->irq_mask[d->hwirq / BANK_SZ] &= ~(1 << (d->hwirq % BANK_SZ));
=======
	irq_hw_number_t hwirq = irqd_to_hwirq(d);

	clear_bit(hwirq, chip->irq_mask);
>>>>>>> upstream/android-13
}

static void pca953x_irq_unmask(struct irq_data *d)
{
	struct gpio_chip *gc = irq_data_get_irq_chip_data(d);
	struct pca953x_chip *chip = gpiochip_get_data(gc);
<<<<<<< HEAD

	chip->irq_mask[d->hwirq / BANK_SZ] |= 1 << (d->hwirq % BANK_SZ);
=======
	irq_hw_number_t hwirq = irqd_to_hwirq(d);

	set_bit(hwirq, chip->irq_mask);
}

static int pca953x_irq_set_wake(struct irq_data *d, unsigned int on)
{
	struct gpio_chip *gc = irq_data_get_irq_chip_data(d);
	struct pca953x_chip *chip = gpiochip_get_data(gc);

	if (on)
		atomic_inc(&chip->wakeup_path);
	else
		atomic_dec(&chip->wakeup_path);

	return irq_set_irq_wake(chip->client->irq, on);
>>>>>>> upstream/android-13
}

static void pca953x_irq_bus_lock(struct irq_data *d)
{
	struct gpio_chip *gc = irq_data_get_irq_chip_data(d);
	struct pca953x_chip *chip = gpiochip_get_data(gc);

	mutex_lock(&chip->irq_lock);
}

static void pca953x_irq_bus_sync_unlock(struct irq_data *d)
{
	struct gpio_chip *gc = irq_data_get_irq_chip_data(d);
	struct pca953x_chip *chip = gpiochip_get_data(gc);
<<<<<<< HEAD
	u8 new_irqs;
	int level, i;
	u8 invert_irq_mask[MAX_BANK];
=======
	DECLARE_BITMAP(irq_mask, MAX_LINE);
	DECLARE_BITMAP(reg_direction, MAX_LINE);
	int level;
>>>>>>> upstream/android-13

	if (chip->driver_data & PCA_PCAL) {
		/* Enable latch on interrupt-enabled inputs */
		pca953x_write_regs(chip, PCAL953X_IN_LATCH, chip->irq_mask);

<<<<<<< HEAD
		for (i = 0; i < NBANK(chip); i++)
			invert_irq_mask[i] = ~chip->irq_mask[i];

		/* Unmask enabled interrupts */
		pca953x_write_regs(chip, PCAL953X_INT_MASK, invert_irq_mask);
	}

	/* Look for any newly setup interrupt */
	for (i = 0; i < NBANK(chip); i++) {
		new_irqs = chip->irq_trig_fall[i] | chip->irq_trig_raise[i];
		new_irqs &= ~chip->reg_direction[i];

		while (new_irqs) {
			level = __ffs(new_irqs);
			pca953x_gpio_direction_input(&chip->gpio_chip,
							level + (BANK_SZ * i));
			new_irqs &= ~(1 << level);
		}
	}
=======
		bitmap_complement(irq_mask, chip->irq_mask, gc->ngpio);

		/* Unmask enabled interrupts */
		pca953x_write_regs(chip, PCAL953X_INT_MASK, irq_mask);
	}

	/* Switch direction to input if needed */
	pca953x_read_regs(chip, chip->regs->direction, reg_direction);

	bitmap_or(irq_mask, chip->irq_trig_fall, chip->irq_trig_raise, gc->ngpio);
	bitmap_complement(reg_direction, reg_direction, gc->ngpio);
	bitmap_and(irq_mask, irq_mask, reg_direction, gc->ngpio);

	/* Look for any newly setup interrupt */
	for_each_set_bit(level, irq_mask, gc->ngpio)
		pca953x_gpio_direction_input(&chip->gpio_chip, level);
>>>>>>> upstream/android-13

	mutex_unlock(&chip->irq_lock);
}

static int pca953x_irq_set_type(struct irq_data *d, unsigned int type)
{
	struct gpio_chip *gc = irq_data_get_irq_chip_data(d);
	struct pca953x_chip *chip = gpiochip_get_data(gc);
<<<<<<< HEAD
	int bank_nb = d->hwirq / BANK_SZ;
	u8 mask = 1 << (d->hwirq % BANK_SZ);
=======
	irq_hw_number_t hwirq = irqd_to_hwirq(d);
>>>>>>> upstream/android-13

	if (!(type & IRQ_TYPE_EDGE_BOTH)) {
		dev_err(&chip->client->dev, "irq %d: unsupported type %d\n",
			d->irq, type);
		return -EINVAL;
	}

<<<<<<< HEAD
	if (type & IRQ_TYPE_EDGE_FALLING)
		chip->irq_trig_fall[bank_nb] |= mask;
	else
		chip->irq_trig_fall[bank_nb] &= ~mask;

	if (type & IRQ_TYPE_EDGE_RISING)
		chip->irq_trig_raise[bank_nb] |= mask;
	else
		chip->irq_trig_raise[bank_nb] &= ~mask;
=======
	assign_bit(hwirq, chip->irq_trig_fall, type & IRQ_TYPE_EDGE_FALLING);
	assign_bit(hwirq, chip->irq_trig_raise, type & IRQ_TYPE_EDGE_RISING);
>>>>>>> upstream/android-13

	return 0;
}

static void pca953x_irq_shutdown(struct irq_data *d)
{
	struct gpio_chip *gc = irq_data_get_irq_chip_data(d);
	struct pca953x_chip *chip = gpiochip_get_data(gc);
<<<<<<< HEAD
	u8 mask = 1 << (d->hwirq % BANK_SZ);

	chip->irq_trig_raise[d->hwirq / BANK_SZ] &= ~mask;
	chip->irq_trig_fall[d->hwirq / BANK_SZ] &= ~mask;
}

static struct irq_chip pca953x_irq_chip = {
	.name			= "pca953x",
	.irq_mask		= pca953x_irq_mask,
	.irq_unmask		= pca953x_irq_unmask,
	.irq_bus_lock		= pca953x_irq_bus_lock,
	.irq_bus_sync_unlock	= pca953x_irq_bus_sync_unlock,
	.irq_set_type		= pca953x_irq_set_type,
	.irq_shutdown		= pca953x_irq_shutdown,
};

static bool pca953x_irq_pending(struct pca953x_chip *chip, u8 *pending)
{
	u8 cur_stat[MAX_BANK];
	u8 old_stat[MAX_BANK];
	bool pending_seen = false;
	bool trigger_seen = false;
	u8 trigger[MAX_BANK];
	int ret, i;
=======
	irq_hw_number_t hwirq = irqd_to_hwirq(d);

	clear_bit(hwirq, chip->irq_trig_raise);
	clear_bit(hwirq, chip->irq_trig_fall);
}

static bool pca953x_irq_pending(struct pca953x_chip *chip, unsigned long *pending)
{
	struct gpio_chip *gc = &chip->gpio_chip;
	DECLARE_BITMAP(reg_direction, MAX_LINE);
	DECLARE_BITMAP(old_stat, MAX_LINE);
	DECLARE_BITMAP(cur_stat, MAX_LINE);
	DECLARE_BITMAP(new_stat, MAX_LINE);
	DECLARE_BITMAP(trigger, MAX_LINE);
	int ret;
>>>>>>> upstream/android-13

	if (chip->driver_data & PCA_PCAL) {
		/* Read the current interrupt status from the device */
		ret = pca953x_read_regs(chip, PCAL953X_INT_STAT, trigger);
		if (ret)
			return false;

		/* Check latched inputs and clear interrupt status */
<<<<<<< HEAD
		ret = pca953x_read_regs(chip, PCA953X_INPUT, cur_stat);
		if (ret)
			return false;

		for (i = 0; i < NBANK(chip); i++) {
			/* Apply filter for rising/falling edge selection */
			pending[i] = (~cur_stat[i] & chip->irq_trig_fall[i]) |
				(cur_stat[i] & chip->irq_trig_raise[i]);
			pending[i] &= trigger[i];
			if (pending[i])
				pending_seen = true;
		}

		return pending_seen;
=======
		ret = pca953x_read_regs(chip, chip->regs->input, cur_stat);
		if (ret)
			return false;

		/* Apply filter for rising/falling edge selection */
		bitmap_replace(new_stat, chip->irq_trig_fall, chip->irq_trig_raise, cur_stat, gc->ngpio);

		bitmap_and(pending, new_stat, trigger, gc->ngpio);

		return !bitmap_empty(pending, gc->ngpio);
>>>>>>> upstream/android-13
	}

	ret = pca953x_read_regs(chip, chip->regs->input, cur_stat);
	if (ret)
		return false;

	/* Remove output pins from the equation */
<<<<<<< HEAD
	for (i = 0; i < NBANK(chip); i++)
		cur_stat[i] &= chip->reg_direction[i];

	memcpy(old_stat, chip->irq_stat, NBANK(chip));

	for (i = 0; i < NBANK(chip); i++) {
		trigger[i] = (cur_stat[i] ^ old_stat[i]) & chip->irq_mask[i];
		if (trigger[i])
			trigger_seen = true;
	}

	if (!trigger_seen)
		return false;

	memcpy(chip->irq_stat, cur_stat, NBANK(chip));

	for (i = 0; i < NBANK(chip); i++) {
		pending[i] = (old_stat[i] & chip->irq_trig_fall[i]) |
			(cur_stat[i] & chip->irq_trig_raise[i]);
		pending[i] &= trigger[i];
		if (pending[i])
			pending_seen = true;
	}

	return pending_seen;
=======
	pca953x_read_regs(chip, chip->regs->direction, reg_direction);

	bitmap_copy(old_stat, chip->irq_stat, gc->ngpio);

	bitmap_and(new_stat, cur_stat, reg_direction, gc->ngpio);
	bitmap_xor(cur_stat, new_stat, old_stat, gc->ngpio);
	bitmap_and(trigger, cur_stat, chip->irq_mask, gc->ngpio);

	bitmap_copy(chip->irq_stat, new_stat, gc->ngpio);

	if (bitmap_empty(trigger, gc->ngpio))
		return false;

	bitmap_and(cur_stat, chip->irq_trig_fall, old_stat, gc->ngpio);
	bitmap_and(old_stat, chip->irq_trig_raise, new_stat, gc->ngpio);
	bitmap_or(new_stat, old_stat, cur_stat, gc->ngpio);
	bitmap_and(pending, new_stat, trigger, gc->ngpio);

	return !bitmap_empty(pending, gc->ngpio);
>>>>>>> upstream/android-13
}

static irqreturn_t pca953x_irq_handler(int irq, void *devid)
{
	struct pca953x_chip *chip = devid;
<<<<<<< HEAD
	u8 pending[MAX_BANK];
	u8 level;
	unsigned nhandled = 0;
	int i;

	if (!pca953x_irq_pending(chip, pending))
		return IRQ_NONE;

	for (i = 0; i < NBANK(chip); i++) {
		while (pending[i]) {
			level = __ffs(pending[i]);
			handle_nested_irq(irq_find_mapping(chip->gpio_chip.irq.domain,
							level + (BANK_SZ * i)));
			pending[i] &= ~(1 << level);
			nhandled++;
		}
	}

	return (nhandled > 0) ? IRQ_HANDLED : IRQ_NONE;
}

static int pca953x_irq_setup(struct pca953x_chip *chip,
			     int irq_base)
{
	struct i2c_client *client = chip->client;
	int ret, i;

	if (client->irq && irq_base != -1
			&& (chip->driver_data & PCA_INT)) {
		ret = pca953x_read_regs(chip,
					chip->regs->input, chip->irq_stat);
		if (ret)
			return ret;

		/*
		 * There is no way to know which GPIO line generated the
		 * interrupt.  We have to rely on the previous read for
		 * this purpose.
		 */
		for (i = 0; i < NBANK(chip); i++)
			chip->irq_stat[i] &= chip->reg_direction[i];
		mutex_init(&chip->irq_lock);

		ret = devm_request_threaded_irq(&client->dev,
					client->irq,
					   NULL,
					   pca953x_irq_handler,
					   IRQF_TRIGGER_LOW | IRQF_ONESHOT |
						   IRQF_SHARED,
					   dev_name(&client->dev), chip);
		if (ret) {
			dev_err(&client->dev, "failed to request irq %d\n",
				client->irq);
			return ret;
		}

		ret =  gpiochip_irqchip_add_nested(&chip->gpio_chip,
						   &pca953x_irq_chip,
						   irq_base,
						   handle_simple_irq,
						   IRQ_TYPE_NONE);
		if (ret) {
			dev_err(&client->dev,
				"could not connect irqchip to gpiochip\n");
			return ret;
		}

		gpiochip_set_nested_irqchip(&chip->gpio_chip,
					    &pca953x_irq_chip,
					    client->irq);
=======
	struct gpio_chip *gc = &chip->gpio_chip;
	DECLARE_BITMAP(pending, MAX_LINE);
	int level;
	bool ret;

	bitmap_zero(pending, MAX_LINE);

	mutex_lock(&chip->i2c_lock);
	ret = pca953x_irq_pending(chip, pending);
	mutex_unlock(&chip->i2c_lock);

	if (ret) {
		ret = 0;

		for_each_set_bit(level, pending, gc->ngpio) {
			int nested_irq = irq_find_mapping(gc->irq.domain, level);

			if (unlikely(nested_irq <= 0)) {
				dev_warn_ratelimited(gc->parent, "unmapped interrupt %d\n", level);
				continue;
			}

			handle_nested_irq(nested_irq);
			ret = 1;
		}
	}

	return IRQ_RETVAL(ret);
}

static int pca953x_irq_setup(struct pca953x_chip *chip, int irq_base)
{
	struct i2c_client *client = chip->client;
	struct irq_chip *irq_chip = &chip->irq_chip;
	DECLARE_BITMAP(reg_direction, MAX_LINE);
	DECLARE_BITMAP(irq_stat, MAX_LINE);
	struct gpio_irq_chip *girq;
	int ret;

	if (dmi_first_match(pca953x_dmi_acpi_irq_info)) {
		ret = pca953x_acpi_get_irq(&client->dev);
		if (ret > 0)
			client->irq = ret;
	}

	if (!client->irq)
		return 0;

	if (irq_base == -1)
		return 0;

	if (!(chip->driver_data & PCA_INT))
		return 0;

	ret = pca953x_read_regs(chip, chip->regs->input, irq_stat);
	if (ret)
		return ret;

	/*
	 * There is no way to know which GPIO line generated the
	 * interrupt.  We have to rely on the previous read for
	 * this purpose.
	 */
	pca953x_read_regs(chip, chip->regs->direction, reg_direction);
	bitmap_and(chip->irq_stat, irq_stat, reg_direction, chip->gpio_chip.ngpio);
	mutex_init(&chip->irq_lock);

	irq_chip->name = dev_name(&client->dev);
	irq_chip->irq_mask = pca953x_irq_mask;
	irq_chip->irq_unmask = pca953x_irq_unmask;
	irq_chip->irq_set_wake = pca953x_irq_set_wake;
	irq_chip->irq_bus_lock = pca953x_irq_bus_lock;
	irq_chip->irq_bus_sync_unlock = pca953x_irq_bus_sync_unlock;
	irq_chip->irq_set_type = pca953x_irq_set_type;
	irq_chip->irq_shutdown = pca953x_irq_shutdown;

	girq = &chip->gpio_chip.irq;
	girq->chip = irq_chip;
	/* This will let us handle the parent IRQ in the driver */
	girq->parent_handler = NULL;
	girq->num_parents = 0;
	girq->parents = NULL;
	girq->default_type = IRQ_TYPE_NONE;
	girq->handler = handle_simple_irq;
	girq->threaded = true;
	girq->first = irq_base; /* FIXME: get rid of this */

	ret = devm_request_threaded_irq(&client->dev, client->irq,
					NULL, pca953x_irq_handler,
					IRQF_ONESHOT | IRQF_SHARED,
					dev_name(&client->dev), chip);
	if (ret) {
		dev_err(&client->dev, "failed to request irq %d\n",
			client->irq);
		return ret;
>>>>>>> upstream/android-13
	}

	return 0;
}

#else /* CONFIG_GPIO_PCA953X_IRQ */
static int pca953x_irq_setup(struct pca953x_chip *chip,
			     int irq_base)
{
	struct i2c_client *client = chip->client;

	if (client->irq && irq_base != -1 && (chip->driver_data & PCA_INT))
		dev_warn(&client->dev, "interrupt support not compiled in\n");

	return 0;
}
#endif

<<<<<<< HEAD
static int device_pca953x_init(struct pca953x_chip *chip, u32 invert)
{
	int ret;
	u8 val[MAX_BANK];

	chip->regs = &pca953x_regs;

	ret = pca953x_read_regs(chip, chip->regs->output, chip->reg_output);
	if (ret)
		goto out;

	ret = pca953x_read_regs(chip, chip->regs->direction,
				chip->reg_direction);
=======
static int device_pca95xx_init(struct pca953x_chip *chip, u32 invert)
{
	DECLARE_BITMAP(val, MAX_LINE);
	int ret;

	ret = regcache_sync_region(chip->regmap, chip->regs->output,
				   chip->regs->output + NBANK(chip));
	if (ret)
		goto out;

	ret = regcache_sync_region(chip->regmap, chip->regs->direction,
				   chip->regs->direction + NBANK(chip));
>>>>>>> upstream/android-13
	if (ret)
		goto out;

	/* set platform specific polarity inversion */
	if (invert)
<<<<<<< HEAD
		memset(val, 0xFF, NBANK(chip));
	else
		memset(val, 0, NBANK(chip));

	ret = pca953x_write_regs(chip, PCA953X_INVERT, val);
=======
		bitmap_fill(val, MAX_LINE);
	else
		bitmap_zero(val, MAX_LINE);

	ret = pca953x_write_regs(chip, chip->regs->invert, val);
>>>>>>> upstream/android-13
out:
	return ret;
}

static int device_pca957x_init(struct pca953x_chip *chip, u32 invert)
{
<<<<<<< HEAD
	int ret;
	u8 val[MAX_BANK];

	chip->regs = &pca957x_regs;

	ret = pca953x_read_regs(chip, chip->regs->output, chip->reg_output);
	if (ret)
		goto out;
	ret = pca953x_read_regs(chip, chip->regs->direction,
				chip->reg_direction);
	if (ret)
		goto out;

	/* set platform specific polarity inversion */
	if (invert)
		memset(val, 0xFF, NBANK(chip));
	else
		memset(val, 0, NBANK(chip));
	ret = pca953x_write_regs(chip, PCA957X_INVRT, val);
=======
	DECLARE_BITMAP(val, MAX_LINE);
	unsigned int i;
	int ret;

	ret = device_pca95xx_init(chip, invert);
>>>>>>> upstream/android-13
	if (ret)
		goto out;

	/* To enable register 6, 7 to control pull up and pull down */
<<<<<<< HEAD
	memset(val, 0x02, NBANK(chip));
=======
	for (i = 0; i < NBANK(chip); i++)
		bitmap_set_value8(val, 0x02, i * BANK_SZ);

>>>>>>> upstream/android-13
	ret = pca953x_write_regs(chip, PCA957X_BKEN, val);
	if (ret)
		goto out;

	return 0;
out:
	return ret;
}

<<<<<<< HEAD
static const struct of_device_id pca953x_dt_ids[];

static int pca953x_probe(struct i2c_client *client,
				   const struct i2c_device_id *i2c_id)
=======
static int pca953x_probe(struct i2c_client *client,
			 const struct i2c_device_id *i2c_id)
>>>>>>> upstream/android-13
{
	struct pca953x_platform_data *pdata;
	struct pca953x_chip *chip;
	int irq_base = 0;
	int ret;
	u32 invert = 0;
	struct regulator *reg;
<<<<<<< HEAD

	chip = devm_kzalloc(&client->dev,
			sizeof(struct pca953x_chip), GFP_KERNEL);
=======
	const struct regmap_config *regmap_config;

	chip = devm_kzalloc(&client->dev, sizeof(*chip), GFP_KERNEL);
>>>>>>> upstream/android-13
	if (chip == NULL)
		return -ENOMEM;

	pdata = dev_get_platdata(&client->dev);
	if (pdata) {
		irq_base = pdata->irq_base;
		chip->gpio_start = pdata->gpio_base;
		invert = pdata->invert;
		chip->names = pdata->names;
	} else {
		struct gpio_desc *reset_gpio;

		chip->gpio_start = -1;
		irq_base = 0;

		/*
		 * See if we need to de-assert a reset pin.
		 *
		 * There is no known ACPI-enabled platforms that are
		 * using "reset" GPIO. Otherwise any of those platform
		 * must use _DSD method with corresponding property.
		 */
		reset_gpio = devm_gpiod_get_optional(&client->dev, "reset",
						     GPIOD_OUT_LOW);
		if (IS_ERR(reset_gpio))
			return PTR_ERR(reset_gpio);
	}

	chip->client = client;

	reg = devm_regulator_get(&client->dev, "vcc");
<<<<<<< HEAD
	if (IS_ERR(reg)) {
		ret = PTR_ERR(reg);
		if (ret != -EPROBE_DEFER)
			dev_err(&client->dev, "reg get err: %d\n", ret);
		return ret;
	}
=======
	if (IS_ERR(reg))
		return dev_err_probe(&client->dev, PTR_ERR(reg), "reg get err\n");

>>>>>>> upstream/android-13
	ret = regulator_enable(reg);
	if (ret) {
		dev_err(&client->dev, "reg en err: %d\n", ret);
		return ret;
	}
	chip->regulator = reg;

	if (i2c_id) {
		chip->driver_data = i2c_id->driver_data;
	} else {
<<<<<<< HEAD
		const struct acpi_device_id *acpi_id;
		struct device *dev = &client->dev;

		chip->driver_data = (uintptr_t)of_device_get_match_data(dev);
		if (!chip->driver_data) {
			acpi_id = acpi_match_device(pca953x_acpi_ids, dev);
			if (!acpi_id) {
				ret = -ENODEV;
				goto err_exit;
			}

			chip->driver_data = acpi_id->driver_data;
		}
	}

=======
		const void *match;

		match = device_get_match_data(&client->dev);
		if (!match) {
			ret = -ENODEV;
			goto err_exit;
		}

		chip->driver_data = (uintptr_t)match;
	}

	i2c_set_clientdata(client, chip);

	pca953x_setup_gpio(chip, chip->driver_data & PCA_GPIO_MASK);

	if (NBANK(chip) > 2 || PCA_CHIP_TYPE(chip->driver_data) == PCA957X_TYPE) {
		dev_info(&client->dev, "using AI\n");
		regmap_config = &pca953x_ai_i2c_regmap;
	} else {
		dev_info(&client->dev, "using no AI\n");
		regmap_config = &pca953x_i2c_regmap;
	}

	chip->regmap = devm_regmap_init_i2c(client, regmap_config);
	if (IS_ERR(chip->regmap)) {
		ret = PTR_ERR(chip->regmap);
		goto err_exit;
	}

	regcache_mark_dirty(chip->regmap);

>>>>>>> upstream/android-13
	mutex_init(&chip->i2c_lock);
	/*
	 * In case we have an i2c-mux controlled by a GPIO provided by an
	 * expander using the same driver higher on the device tree, read the
	 * i2c adapter nesting depth and use the retrieved value as lockdep
	 * subclass for chip->i2c_lock.
	 *
	 * REVISIT: This solution is not complete. It protects us from lockdep
	 * false positives when the expander controlling the i2c-mux is on
	 * a different level on the device tree, but not when it's on the same
	 * level on a different branch (in which case the subclass number
	 * would be the same).
	 *
	 * TODO: Once a correct solution is developed, a similar fix should be
	 * applied to all other i2c-controlled GPIO expanders (and potentially
	 * regmap-i2c).
	 */
	lockdep_set_subclass(&chip->i2c_lock,
			     i2c_adapter_depth(client->adapter));

	/* initialize cached registers from their original values.
	 * we can't share this chip with another i2c master.
	 */
<<<<<<< HEAD
	pca953x_setup_gpio(chip, chip->driver_data & PCA_GPIO_MASK);

	if (chip->gpio_chip.ngpio <= 8) {
		chip->write_regs = pca953x_write_regs_8;
		chip->read_regs = pca953x_read_regs_8;
	} else if (chip->gpio_chip.ngpio >= 24) {
		chip->write_regs = pca953x_write_regs_24;
		chip->read_regs = pca953x_read_regs_24;
	} else {
		if (PCA_CHIP_TYPE(chip->driver_data) == PCA953X_TYPE)
			chip->write_regs = pca953x_write_regs_16;
		else
			chip->write_regs = pca957x_write_regs_16;
		chip->read_regs = pca953x_read_regs_16;
	}

	if (PCA_CHIP_TYPE(chip->driver_data) == PCA953X_TYPE)
		ret = device_pca953x_init(chip, invert);
	else
		ret = device_pca957x_init(chip, invert);
	if (ret)
		goto err_exit;

	ret = devm_gpiochip_add_data(&client->dev, &chip->gpio_chip, chip);
=======

	if (PCA_CHIP_TYPE(chip->driver_data) == PCA953X_TYPE) {
		chip->regs = &pca953x_regs;
		ret = device_pca95xx_init(chip, invert);
	} else {
		chip->regs = &pca957x_regs;
		ret = device_pca957x_init(chip, invert);
	}
>>>>>>> upstream/android-13
	if (ret)
		goto err_exit;

	ret = pca953x_irq_setup(chip, irq_base);
	if (ret)
		goto err_exit;

<<<<<<< HEAD
	if (pdata && pdata->setup) {
		ret = pdata->setup(client, chip->gpio_chip.base,
				chip->gpio_chip.ngpio, pdata->context);
=======
	ret = devm_gpiochip_add_data(&client->dev, &chip->gpio_chip, chip);
	if (ret)
		goto err_exit;

	if (pdata && pdata->setup) {
		ret = pdata->setup(client, chip->gpio_chip.base,
				   chip->gpio_chip.ngpio, pdata->context);
>>>>>>> upstream/android-13
		if (ret < 0)
			dev_warn(&client->dev, "setup failed, %d\n", ret);
	}

<<<<<<< HEAD
	i2c_set_clientdata(client, chip);
=======
>>>>>>> upstream/android-13
	return 0;

err_exit:
	regulator_disable(chip->regulator);
	return ret;
}

static int pca953x_remove(struct i2c_client *client)
{
	struct pca953x_platform_data *pdata = dev_get_platdata(&client->dev);
	struct pca953x_chip *chip = i2c_get_clientdata(client);
	int ret;

	if (pdata && pdata->teardown) {
		ret = pdata->teardown(client, chip->gpio_chip.base,
<<<<<<< HEAD
				chip->gpio_chip.ngpio, pdata->context);
		if (ret < 0)
			dev_err(&client->dev, "%s failed, %d\n",
					"teardown", ret);
=======
				      chip->gpio_chip.ngpio, pdata->context);
		if (ret < 0)
			dev_err(&client->dev, "teardown failed, %d\n", ret);
>>>>>>> upstream/android-13
	} else {
		ret = 0;
	}

	regulator_disable(chip->regulator);

	return ret;
}

<<<<<<< HEAD
=======
#ifdef CONFIG_PM_SLEEP
static int pca953x_regcache_sync(struct device *dev)
{
	struct pca953x_chip *chip = dev_get_drvdata(dev);
	int ret;

	/*
	 * The ordering between direction and output is important,
	 * sync these registers first and only then sync the rest.
	 */
	ret = regcache_sync_region(chip->regmap, chip->regs->direction,
				   chip->regs->direction + NBANK(chip));
	if (ret) {
		dev_err(dev, "Failed to sync GPIO dir registers: %d\n", ret);
		return ret;
	}

	ret = regcache_sync_region(chip->regmap, chip->regs->output,
				   chip->regs->output + NBANK(chip));
	if (ret) {
		dev_err(dev, "Failed to sync GPIO out registers: %d\n", ret);
		return ret;
	}

#ifdef CONFIG_GPIO_PCA953X_IRQ
	if (chip->driver_data & PCA_PCAL) {
		ret = regcache_sync_region(chip->regmap, PCAL953X_IN_LATCH,
					   PCAL953X_IN_LATCH + NBANK(chip));
		if (ret) {
			dev_err(dev, "Failed to sync INT latch registers: %d\n",
				ret);
			return ret;
		}

		ret = regcache_sync_region(chip->regmap, PCAL953X_INT_MASK,
					   PCAL953X_INT_MASK + NBANK(chip));
		if (ret) {
			dev_err(dev, "Failed to sync INT mask registers: %d\n",
				ret);
			return ret;
		}
	}
#endif

	return 0;
}

static int pca953x_suspend(struct device *dev)
{
	struct pca953x_chip *chip = dev_get_drvdata(dev);

	regcache_cache_only(chip->regmap, true);

	if (atomic_read(&chip->wakeup_path))
		device_set_wakeup_path(dev);
	else
		regulator_disable(chip->regulator);

	return 0;
}

static int pca953x_resume(struct device *dev)
{
	struct pca953x_chip *chip = dev_get_drvdata(dev);
	int ret;

	if (!atomic_read(&chip->wakeup_path)) {
		ret = regulator_enable(chip->regulator);
		if (ret) {
			dev_err(dev, "Failed to enable regulator: %d\n", ret);
			return 0;
		}
	}

	regcache_cache_only(chip->regmap, false);
	regcache_mark_dirty(chip->regmap);
	ret = pca953x_regcache_sync(dev);
	if (ret)
		return ret;

	ret = regcache_sync(chip->regmap);
	if (ret) {
		dev_err(dev, "Failed to restore register map: %d\n", ret);
		return ret;
	}

	return 0;
}
#endif

>>>>>>> upstream/android-13
/* convenience to stop overlong match-table lines */
#define OF_953X(__nrgpio, __int) (void *)(__nrgpio | PCA953X_TYPE | __int)
#define OF_957X(__nrgpio, __int) (void *)(__nrgpio | PCA957X_TYPE | __int)

static const struct of_device_id pca953x_dt_ids[] = {
<<<<<<< HEAD
	{ .compatible = "nxp,pca9505", .data = OF_953X(40, PCA_INT), },
=======
	{ .compatible = "nxp,pca6416", .data = OF_953X(16, PCA_INT), },
	{ .compatible = "nxp,pca9505", .data = OF_953X(40, PCA_INT), },
	{ .compatible = "nxp,pca9506", .data = OF_953X(40, PCA_INT), },
>>>>>>> upstream/android-13
	{ .compatible = "nxp,pca9534", .data = OF_953X( 8, PCA_INT), },
	{ .compatible = "nxp,pca9535", .data = OF_953X(16, PCA_INT), },
	{ .compatible = "nxp,pca9536", .data = OF_953X( 4, 0), },
	{ .compatible = "nxp,pca9537", .data = OF_953X( 4, PCA_INT), },
	{ .compatible = "nxp,pca9538", .data = OF_953X( 8, PCA_INT), },
	{ .compatible = "nxp,pca9539", .data = OF_953X(16, PCA_INT), },
	{ .compatible = "nxp,pca9554", .data = OF_953X( 8, PCA_INT), },
	{ .compatible = "nxp,pca9555", .data = OF_953X(16, PCA_INT), },
	{ .compatible = "nxp,pca9556", .data = OF_953X( 8, 0), },
	{ .compatible = "nxp,pca9557", .data = OF_953X( 8, 0), },
	{ .compatible = "nxp,pca9574", .data = OF_957X( 8, PCA_INT), },
	{ .compatible = "nxp,pca9575", .data = OF_957X(16, PCA_INT), },
	{ .compatible = "nxp,pca9698", .data = OF_953X(40, 0), },

<<<<<<< HEAD
	{ .compatible = "nxp,pcal6524", .data = OF_953X(24, PCA_LATCH_INT), },
=======
	{ .compatible = "nxp,pcal6416", .data = OF_953X(16, PCA_LATCH_INT), },
	{ .compatible = "nxp,pcal6524", .data = OF_953X(24, PCA_LATCH_INT), },
	{ .compatible = "nxp,pcal9535", .data = OF_953X(16, PCA_LATCH_INT), },
	{ .compatible = "nxp,pcal9554b", .data = OF_953X( 8, PCA_LATCH_INT), },
>>>>>>> upstream/android-13
	{ .compatible = "nxp,pcal9555a", .data = OF_953X(16, PCA_LATCH_INT), },

	{ .compatible = "maxim,max7310", .data = OF_953X( 8, 0), },
	{ .compatible = "maxim,max7312", .data = OF_953X(16, PCA_INT), },
	{ .compatible = "maxim,max7313", .data = OF_953X(16, PCA_INT), },
	{ .compatible = "maxim,max7315", .data = OF_953X( 8, PCA_INT), },
	{ .compatible = "maxim,max7318", .data = OF_953X(16, PCA_INT), },

	{ .compatible = "ti,pca6107", .data = OF_953X( 8, PCA_INT), },
	{ .compatible = "ti,pca9536", .data = OF_953X( 4, 0), },
	{ .compatible = "ti,tca6408", .data = OF_953X( 8, PCA_INT), },
	{ .compatible = "ti,tca6416", .data = OF_953X(16, PCA_INT), },
	{ .compatible = "ti,tca6424", .data = OF_953X(24, PCA_INT), },
<<<<<<< HEAD

	{ .compatible = "onnn,pca9654", .data = OF_953X( 8, PCA_INT), },
=======
	{ .compatible = "ti,tca9539", .data = OF_953X(16, PCA_INT), },

	{ .compatible = "onnn,cat9554", .data = OF_953X( 8, PCA_INT), },
	{ .compatible = "onnn,pca9654", .data = OF_953X( 8, PCA_INT), },
	{ .compatible = "onnn,pca9655", .data = OF_953X(16, PCA_INT), },
>>>>>>> upstream/android-13

	{ .compatible = "exar,xra1202", .data = OF_953X( 8, 0), },
	{ }
};

MODULE_DEVICE_TABLE(of, pca953x_dt_ids);

<<<<<<< HEAD
static struct i2c_driver pca953x_driver = {
	.driver = {
		.name	= "pca953x",
		.of_match_table = pca953x_dt_ids,
		.acpi_match_table = ACPI_PTR(pca953x_acpi_ids),
=======
static SIMPLE_DEV_PM_OPS(pca953x_pm_ops, pca953x_suspend, pca953x_resume);

static struct i2c_driver pca953x_driver = {
	.driver = {
		.name	= "pca953x",
		.pm	= &pca953x_pm_ops,
		.of_match_table = pca953x_dt_ids,
		.acpi_match_table = pca953x_acpi_ids,
>>>>>>> upstream/android-13
	},
	.probe		= pca953x_probe,
	.remove		= pca953x_remove,
	.id_table	= pca953x_id,
};

static int __init pca953x_init(void)
{
	return i2c_add_driver(&pca953x_driver);
}
/* register after i2c postcore initcall and before
 * subsys initcalls that may rely on these GPIOs
 */
subsys_initcall(pca953x_init);

static void __exit pca953x_exit(void)
{
	i2c_del_driver(&pca953x_driver);
}
module_exit(pca953x_exit);

MODULE_AUTHOR("eric miao <eric.miao@marvell.com>");
MODULE_DESCRIPTION("GPIO expander driver for PCA953x");
MODULE_LICENSE("GPL");
