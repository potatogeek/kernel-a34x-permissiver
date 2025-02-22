/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Platform data for the Intel TCO Watchdog
 */

#ifndef _ITCO_WDT_H_
#define _ITCO_WDT_H_

/* Watchdog resources */
#define ICH_RES_IO_TCO		0
#define ICH_RES_IO_SMI		1
#define ICH_RES_MEM_OFF		2
#define ICH_RES_MEM_GCS_PMC	0

<<<<<<< HEAD
struct itco_wdt_platform_data {
	char name[32];
	unsigned int version;
	/* private data to be passed to update_no_reboot_bit API */
	void *no_reboot_priv;
	/* pointer for platform specific no reboot update function */
	int (*update_no_reboot_bit)(void *priv, bool set);
=======
/**
 * struct itco_wdt_platform_data - iTCO_wdt platform data
 * @name: Name of the platform
 * @version: iTCO version
 * @no_reboot_use_pmc: Use PMC BXT API to set and clear NO_REBOOT bit
 */
struct itco_wdt_platform_data {
	char name[32];
	unsigned int version;
	bool no_reboot_use_pmc;
>>>>>>> upstream/android-13
};

#endif /* _ITCO_WDT_H_ */
