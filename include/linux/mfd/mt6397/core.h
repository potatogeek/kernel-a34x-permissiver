<<<<<<< HEAD
/*
 * Copyright (c) 2014 MediaTek Inc.
 * Author: Flora Fu, MediaTek
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
=======
/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Copyright (c) 2014 MediaTek Inc.
 * Author: Flora Fu, MediaTek
>>>>>>> upstream/android-13
 */

#ifndef __MFD_MT6397_CORE_H__
#define __MFD_MT6397_CORE_H__

<<<<<<< HEAD
=======
#include <linux/mutex.h>
>>>>>>> upstream/android-13
#include <linux/notifier.h>

enum chip_id {
	MT6323_CHIP_ID = 0x23,
<<<<<<< HEAD
	MT6357_CHIP_ID = 0x57,
=======
>>>>>>> upstream/android-13
	MT6358_CHIP_ID = 0x58,
	MT6359_CHIP_ID = 0x59,
	MT6391_CHIP_ID = 0x91,
	MT6397_CHIP_ID = 0x97,
};

enum mt6397_irq_numbers {
	MT6397_IRQ_SPKL_AB = 0,
	MT6397_IRQ_SPKR_AB,
	MT6397_IRQ_SPKL,
	MT6397_IRQ_SPKR,
	MT6397_IRQ_BAT_L,
	MT6397_IRQ_BAT_H,
	MT6397_IRQ_FG_BAT_L,
	MT6397_IRQ_FG_BAT_H,
	MT6397_IRQ_WATCHDOG,
	MT6397_IRQ_PWRKEY,
	MT6397_IRQ_THR_L,
	MT6397_IRQ_THR_H,
	MT6397_IRQ_VBATON_UNDET,
	MT6397_IRQ_BVALID_DET,
	MT6397_IRQ_CHRDET,
	MT6397_IRQ_OV,
	MT6397_IRQ_LDO,
	MT6397_IRQ_HOMEKEY,
	MT6397_IRQ_ACCDET,
	MT6397_IRQ_AUDIO,
	MT6397_IRQ_RTC,
	MT6397_IRQ_PWRKEY_RSTB,
	MT6397_IRQ_HDMI_SIFM,
	MT6397_IRQ_HDMI_CEC,
	MT6397_IRQ_VCA15,
	MT6397_IRQ_VSRMCA15,
	MT6397_IRQ_VCORE,
	MT6397_IRQ_VGPU,
	MT6397_IRQ_VIO18,
	MT6397_IRQ_VPCA7,
	MT6397_IRQ_VSRMCA7,
	MT6397_IRQ_VDRM,
	MT6397_IRQ_NR,
};

struct mt6397_chip {
	struct device *dev;
	struct regmap *regmap;
	struct notifier_block pm_nb;
	int irq;
	struct irq_domain *irq_domain;
	struct mutex irqlock;
	u16 wake_mask[2];
	u16 irq_masks_cur[2];
	u16 irq_masks_cache[2];
	u16 int_con[2];
	u16 int_status[2];
	u16 chip_id;
	void *irq_data;
};

<<<<<<< HEAD
int mt6358_ipi_init(struct mt6397_chip *chip);
int mt6358_irq_init(struct mt6397_chip *chip);
int mt6397_irq_init(struct mt6397_chip *mt6397);
=======
int mt6358_irq_init(struct mt6397_chip *chip);
int mt6397_irq_init(struct mt6397_chip *chip);
>>>>>>> upstream/android-13

#endif /* __MFD_MT6397_CORE_H__ */
