/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * DMI defines for use by IPMI
 */
<<<<<<< HEAD

#ifdef CONFIG_IPMI_DMI_DECODE
int ipmi_dmi_get_slave_addr(enum si_type si_type, u32 flags,
=======
#include "ipmi_si.h"

#ifdef CONFIG_IPMI_DMI_DECODE
int ipmi_dmi_get_slave_addr(enum si_type si_type, unsigned int space,
>>>>>>> upstream/android-13
			    unsigned long base_addr);
#endif
