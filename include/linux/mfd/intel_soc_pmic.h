<<<<<<< HEAD
/*
 * intel_soc_pmic.h - Intel SoC PMIC Driver
 *
 * Copyright (C) 2012-2014 Intel Corporation. All rights reserved.
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
/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Intel SoC PMIC Driver
 *
 * Copyright (C) 2012-2014 Intel Corporation. All rights reserved.
 *
>>>>>>> upstream/android-13
 * Author: Yang, Bin <bin.yang@intel.com>
 * Author: Zhu, Lejun <lejun.zhu@linux.intel.com>
 */

#ifndef __INTEL_SOC_PMIC_H__
#define __INTEL_SOC_PMIC_H__

#include <linux/regmap.h>

<<<<<<< HEAD
=======
/**
 * struct intel_soc_pmic - Intel SoC PMIC data
 * @irq: Master interrupt number of the parent PMIC device
 * @regmap: Pointer to the parent PMIC device regmap structure
 * @irq_chip_data: IRQ chip data for the PMIC itself
 * @irq_chip_data_pwrbtn: Chained IRQ chip data for the Power Button
 * @irq_chip_data_tmu: Chained IRQ chip data for the Time Management Unit
 * @irq_chip_data_bcu: Chained IRQ chip data for the Burst Control Unit
 * @irq_chip_data_adc: Chained IRQ chip data for the General Purpose ADC
 * @irq_chip_data_chgr: Chained IRQ chip data for the External Charger
 * @irq_chip_data_crit: Chained IRQ chip data for the Critical Event Handler
 * @dev: Pointer to the parent PMIC device
 * @scu: Pointer to the SCU IPC device data structure
 */
>>>>>>> upstream/android-13
struct intel_soc_pmic {
	int irq;
	struct regmap *regmap;
	struct regmap_irq_chip_data *irq_chip_data;
	struct regmap_irq_chip_data *irq_chip_data_pwrbtn;
	struct regmap_irq_chip_data *irq_chip_data_tmu;
	struct regmap_irq_chip_data *irq_chip_data_bcu;
	struct regmap_irq_chip_data *irq_chip_data_adc;
	struct regmap_irq_chip_data *irq_chip_data_chgr;
	struct regmap_irq_chip_data *irq_chip_data_crit;
	struct device *dev;
<<<<<<< HEAD
};

=======
	struct intel_scu_ipc_dev *scu;
};

int intel_soc_pmic_exec_mipi_pmic_seq_element(u16 i2c_address, u32 reg_address,
					      u32 value, u32 mask);

>>>>>>> upstream/android-13
#endif	/* __INTEL_SOC_PMIC_H__ */
