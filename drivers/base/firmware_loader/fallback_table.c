// SPDX-License-Identifier: GPL-2.0

#include <linux/types.h>
#include <linux/kconfig.h>
#include <linux/list.h>
#include <linux/slab.h>
#include <linux/security.h>
#include <linux/highmem.h>
#include <linux/umh.h>
#include <linux/sysctl.h>

#include "fallback.h"
#include "firmware.h"

/*
 * firmware fallback configuration table
 */

<<<<<<< HEAD
/* Module or buit-in */
#ifdef CONFIG_FW_LOADER_USER_HELPER

static unsigned int zero;
static unsigned int one = 1;

=======
>>>>>>> upstream/android-13
struct firmware_fallback_config fw_fallback_config = {
	.force_sysfs_fallback = IS_ENABLED(CONFIG_FW_LOADER_USER_HELPER_FALLBACK),
	.loading_timeout = 60,
	.old_timeout = 60,
};
<<<<<<< HEAD
EXPORT_SYMBOL_GPL(fw_fallback_config);

=======
EXPORT_SYMBOL_NS_GPL(fw_fallback_config, FIRMWARE_LOADER_PRIVATE);

#ifdef CONFIG_SYSCTL
>>>>>>> upstream/android-13
struct ctl_table firmware_config_table[] = {
	{
		.procname	= "force_sysfs_fallback",
		.data		= &fw_fallback_config.force_sysfs_fallback,
		.maxlen         = sizeof(unsigned int),
		.mode           = 0644,
		.proc_handler   = proc_douintvec_minmax,
<<<<<<< HEAD
		.extra1		= &zero,
		.extra2		= &one,
=======
		.extra1		= SYSCTL_ZERO,
		.extra2		= SYSCTL_ONE,
>>>>>>> upstream/android-13
	},
	{
		.procname	= "ignore_sysfs_fallback",
		.data		= &fw_fallback_config.ignore_sysfs_fallback,
		.maxlen         = sizeof(unsigned int),
		.mode           = 0644,
		.proc_handler   = proc_douintvec_minmax,
<<<<<<< HEAD
		.extra1		= &zero,
		.extra2		= &one,
	},
	{ }
};
EXPORT_SYMBOL_GPL(firmware_config_table);

=======
		.extra1		= SYSCTL_ZERO,
		.extra2		= SYSCTL_ONE,
	},
	{ }
};
>>>>>>> upstream/android-13
#endif
