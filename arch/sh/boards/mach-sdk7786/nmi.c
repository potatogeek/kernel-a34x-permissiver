<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0
>>>>>>> upstream/android-13
/*
 * SDK7786 FPGA NMI Support.
 *
 * Copyright (C) 2010  Paul Mundt
<<<<<<< HEAD
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
=======
>>>>>>> upstream/android-13
 */
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/string.h>
#include <mach/fpga.h>

enum {
	NMI_MODE_MANUAL,
	NMI_MODE_AUX,
	NMI_MODE_MASKED,
	NMI_MODE_ANY,
	NMI_MODE_UNKNOWN,
};

/*
 * Default to the manual NMI switch.
 */
static unsigned int __initdata nmi_mode = NMI_MODE_ANY;

static int __init nmi_mode_setup(char *str)
{
	if (!str)
		return 0;

	if (strcmp(str, "manual") == 0)
		nmi_mode = NMI_MODE_MANUAL;
	else if (strcmp(str, "aux") == 0)
		nmi_mode = NMI_MODE_AUX;
	else if (strcmp(str, "masked") == 0)
		nmi_mode = NMI_MODE_MASKED;
	else if (strcmp(str, "any") == 0)
		nmi_mode = NMI_MODE_ANY;
	else {
		nmi_mode = NMI_MODE_UNKNOWN;
<<<<<<< HEAD
		pr_warning("Unknown NMI mode %s\n", str);
=======
		pr_warn("Unknown NMI mode %s\n", str);
>>>>>>> upstream/android-13
	}

	printk("Set NMI mode to %d\n", nmi_mode);
	return 0;
}
early_param("nmi_mode", nmi_mode_setup);

void __init sdk7786_nmi_init(void)
{
	unsigned int source, mask, tmp;

	switch (nmi_mode) {
	case NMI_MODE_MANUAL:
		source = NMISR_MAN_NMI;
		mask = NMIMR_MAN_NMIM;
		break;
	case NMI_MODE_AUX:
		source = NMISR_AUX_NMI;
		mask = NMIMR_AUX_NMIM;
		break;
	case NMI_MODE_ANY:
		source = NMISR_MAN_NMI | NMISR_AUX_NMI;
		mask = NMIMR_MAN_NMIM | NMIMR_AUX_NMIM;
		break;
	case NMI_MODE_MASKED:
	case NMI_MODE_UNKNOWN:
	default:
		source = mask = 0;
		break;
	}

	/* Set the NMI source */
	tmp = fpga_read_reg(NMISR);
	tmp &= ~NMISR_MASK;
	tmp |= source;
	fpga_write_reg(tmp, NMISR);

	/* And the IRQ masking */
	fpga_write_reg(NMIMR_MASK ^ mask, NMIMR);
}
