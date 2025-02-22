// SPDX-License-Identifier: GPL-2.0
#include <linux/bug.h>

void check(void)
{
	/*
	 * These kconfig symbols must be set to "m" for nfit_test to
	 * load and operate.
	 */
	BUILD_BUG_ON(!IS_MODULE(CONFIG_LIBNVDIMM));
	BUILD_BUG_ON(!IS_MODULE(CONFIG_BLK_DEV_PMEM));
	BUILD_BUG_ON(!IS_MODULE(CONFIG_ND_BTT));
	BUILD_BUG_ON(!IS_MODULE(CONFIG_ND_PFN));
	BUILD_BUG_ON(!IS_MODULE(CONFIG_ND_BLK));
<<<<<<< HEAD
	BUILD_BUG_ON(!IS_MODULE(CONFIG_ACPI_NFIT));
=======
	if (IS_ENABLED(CONFIG_ACPI_NFIT))
		BUILD_BUG_ON(!IS_MODULE(CONFIG_ACPI_NFIT));
>>>>>>> upstream/android-13
	BUILD_BUG_ON(!IS_MODULE(CONFIG_DEV_DAX));
	BUILD_BUG_ON(!IS_MODULE(CONFIG_DEV_DAX_PMEM));
}
