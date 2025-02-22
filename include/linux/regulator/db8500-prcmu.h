<<<<<<< HEAD
/*
 * Copyright (C) ST-Ericsson SA 2010
 *
 * License Terms: GNU General Public License v2
 *
=======
/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Copyright (C) ST-Ericsson SA 2010
 *
>>>>>>> upstream/android-13
 * Author: Bengt Jonsson <bengt.g.jonsson@stericsson.com> for ST-Ericsson
 *
 * Interface to power domain regulators on DB8500
 */

#ifndef __REGULATOR_H__
#define __REGULATOR_H__

/* Number of DB8500 regulators and regulator enumeration */
enum db8500_regulator_id {
	DB8500_REGULATOR_VAPE,
	DB8500_REGULATOR_VARM,
	DB8500_REGULATOR_VMODEM,
	DB8500_REGULATOR_VPLL,
	DB8500_REGULATOR_VSMPS1,
	DB8500_REGULATOR_VSMPS2,
	DB8500_REGULATOR_VSMPS3,
	DB8500_REGULATOR_VRF1,
	DB8500_REGULATOR_SWITCH_SVAMMDSP,
	DB8500_REGULATOR_SWITCH_SVAMMDSPRET,
	DB8500_REGULATOR_SWITCH_SVAPIPE,
	DB8500_REGULATOR_SWITCH_SIAMMDSP,
	DB8500_REGULATOR_SWITCH_SIAMMDSPRET,
	DB8500_REGULATOR_SWITCH_SIAPIPE,
	DB8500_REGULATOR_SWITCH_SGA,
	DB8500_REGULATOR_SWITCH_B2R2_MCDE,
	DB8500_REGULATOR_SWITCH_ESRAM12,
	DB8500_REGULATOR_SWITCH_ESRAM12RET,
	DB8500_REGULATOR_SWITCH_ESRAM34,
	DB8500_REGULATOR_SWITCH_ESRAM34RET,
	DB8500_NUM_REGULATORS
};

/*
 * Exported interface for CPUIdle only. This function is called with all
 * interrupts turned off.
 */
int power_state_active_is_enabled(void);

#endif
