<<<<<<< HEAD
// SPDX-License-Identifier: GPL-2.0
/*
 * Driver for the NXP ISP1760 chip
 *
=======
/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Driver for the NXP ISP1760 chip
 *
 * Copyright 2021 Linaro, Rui Miguel Silva
>>>>>>> upstream/android-13
 * Copyright 2014 Laurent Pinchart
 * Copyright 2007 Sebastian Siewior
 *
 * Contacts:
 *	Sebastian Siewior <bigeasy@linutronix.de>
 *	Laurent Pinchart <laurent.pinchart@ideasonboard.com>
<<<<<<< HEAD
=======
 *	Rui Miguel Silva <rui.silva@linaro.org>
>>>>>>> upstream/android-13
 */

#ifndef _ISP1760_CORE_H_
#define _ISP1760_CORE_H_

#include <linux/ioport.h>
<<<<<<< HEAD
=======
#include <linux/regmap.h>
>>>>>>> upstream/android-13

#include "isp1760-hcd.h"
#include "isp1760-udc.h"

struct device;
struct gpio_desc;

/*
 * Device flags that can vary from board to board.  All of these
 * indicate the most "atypical" case, so that a devflags of 0 is
 * a sane default configuration.
 */
#define ISP1760_FLAG_BUS_WIDTH_16	0x00000002 /* 16-bit data bus width */
<<<<<<< HEAD
#define ISP1760_FLAG_OTG_EN		0x00000004 /* Port 1 supports OTG */
=======
#define ISP1760_FLAG_PERIPHERAL_EN	0x00000004 /* Port 1 supports Peripheral mode*/
>>>>>>> upstream/android-13
#define ISP1760_FLAG_ANALOG_OC		0x00000008 /* Analog overcurrent */
#define ISP1760_FLAG_DACK_POL_HIGH	0x00000010 /* DACK active high */
#define ISP1760_FLAG_DREQ_POL_HIGH	0x00000020 /* DREQ active high */
#define ISP1760_FLAG_ISP1761		0x00000040 /* Chip is ISP1761 */
#define ISP1760_FLAG_INTR_POL_HIGH	0x00000080 /* Interrupt polarity active high */
#define ISP1760_FLAG_INTR_EDGE_TRIG	0x00000100 /* Interrupt edge triggered */
<<<<<<< HEAD
=======
#define ISP1760_FLAG_ISP1763		0x00000200 /* Chip is ISP1763 */
#define ISP1760_FLAG_BUS_WIDTH_8	0x00000400 /* 8-bit data bus width */
>>>>>>> upstream/android-13

struct isp1760_device {
	struct device *dev;

<<<<<<< HEAD
	void __iomem *regs;
=======
>>>>>>> upstream/android-13
	unsigned int devflags;
	struct gpio_desc *rst_gpio;

	struct isp1760_hcd hcd;
	struct isp1760_udc udc;
};

int isp1760_register(struct resource *mem, int irq, unsigned long irqflags,
		     struct device *dev, unsigned int devflags);
void isp1760_unregister(struct device *dev);

void isp1760_set_pullup(struct isp1760_device *isp, bool enable);

<<<<<<< HEAD
static inline u32 isp1760_read32(void __iomem *base, u32 reg)
{
	return readl(base + reg);
}

static inline void isp1760_write32(void __iomem *base, u32 reg, u32 val)
{
	writel(val, base + reg);
}

=======
static inline u32 isp1760_field_read(struct regmap_field **fields, u32 field)
{
	unsigned int val;

	regmap_field_read(fields[field], &val);

	return val;
}

static inline void isp1760_field_write(struct regmap_field **fields, u32 field,
				       u32 val)
{
	regmap_field_write(fields[field], val);
}

static inline void isp1760_field_set(struct regmap_field **fields, u32 field)
{
	isp1760_field_write(fields, field, 0xFFFFFFFF);
}

static inline void isp1760_field_clear(struct regmap_field **fields, u32 field)
{
	isp1760_field_write(fields, field, 0);
}

static inline u32 isp1760_reg_read(struct regmap *regs, u32 reg)
{
	unsigned int val;

	regmap_read(regs, reg, &val);

	return val;
}

static inline void isp1760_reg_write(struct regmap *regs, u32 reg, u32 val)
{
	regmap_write(regs, reg, val);
}
>>>>>>> upstream/android-13
#endif
