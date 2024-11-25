<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> upstream/android-13
/*
 * Copyright (C) ST-Ericsson SA 2010
 *
 * Author: Bengt Jonsson <bengt.jonsson@stericsson.com> for ST-Ericsson,
 *	   Jonas Aaberg <jonas.aberg@stericsson.com> for ST-Ericsson
<<<<<<< HEAD
 *
 * License Terms: GNU General Public License v2
 *
=======
>>>>>>> upstream/android-13
 */

#ifndef DBX500_REGULATOR_H
#define DBX500_REGULATOR_H

#include <linux/platform_device.h>

/**
 * struct dbx500_regulator_info - dbx500 regulator information
<<<<<<< HEAD
 * @dev: device pointer
 * @desc: regulator description
 * @rdev: regulator device pointer
=======
 * @desc: regulator description
>>>>>>> upstream/android-13
 * @is_enabled: status of the regulator
 * @epod_id: id for EPOD (power domain)
 * @is_ramret: RAM retention switch for EPOD (power domain)
 *
 */
struct dbx500_regulator_info {
<<<<<<< HEAD
	struct device *dev;
	struct regulator_desc desc;
	struct regulator_dev *rdev;
=======
	struct regulator_desc desc;
>>>>>>> upstream/android-13
	bool is_enabled;
	u16 epod_id;
	bool is_ramret;
	bool exclude_from_power_state;
};

void power_state_active_enable(void);
int power_state_active_disable(void);


#ifdef CONFIG_REGULATOR_DEBUG
int ux500_regulator_debug_init(struct platform_device *pdev,
			       struct dbx500_regulator_info *regulator_info,
			       int num_regulators);

int ux500_regulator_debug_exit(void);
#else

static inline int ux500_regulator_debug_init(struct platform_device *pdev,
			     struct dbx500_regulator_info *regulator_info,
			     int num_regulators)
{
	return 0;
}

static inline int ux500_regulator_debug_exit(void)
{
	return 0;
}

#endif
#endif
