<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> upstream/android-13
/*
 *  Copyright 2007 Red Hat, Inc.
 *  by Peter Jones <pjones@redhat.com>
 *  Copyright 2007 IBM, Inc.
 *  by Konrad Rzeszutek <konradr@linux.vnet.ibm.com>
 *  Copyright 2008
 *  by Konrad Rzeszutek <ketuzsezr@darnok.org>
 *
 * This code exposes the iSCSI Boot Format Table to userland via sysfs.
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License v2.0 as published by
 * the Free Software Foundation
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
=======
>>>>>>> upstream/android-13
 */

#ifndef ISCSI_IBFT_H
#define ISCSI_IBFT_H

<<<<<<< HEAD
#include <linux/acpi.h>

/*
 * Logical location of iSCSI Boot Format Table.
 * If the value is NULL there is no iBFT on the machine.
 */
extern struct acpi_table_ibft *ibft_addr;

/*
 * Routine used to find and reserve the iSCSI Boot Format Table. The
 * mapped address is set in the ibft_addr variable.
 */
#ifdef CONFIG_ISCSI_IBFT_FIND
unsigned long find_ibft_region(unsigned long *sizep);
#else
static inline unsigned long find_ibft_region(unsigned long *sizep)
{
	*sizep = 0;
	return 0;
}
=======
#include <linux/types.h>

/*
 * Physical location of iSCSI Boot Format Table.
 * If the value is 0 there is no iBFT on the machine.
 */
extern phys_addr_t ibft_phys_addr;

/*
 * Routine used to find and reserve the iSCSI Boot Format Table. The
 * physical address is set in the ibft_phys_addr variable.
 */
#ifdef CONFIG_ISCSI_IBFT_FIND
void reserve_ibft_region(void);
#else
static inline void reserve_ibft_region(void) {}
>>>>>>> upstream/android-13
#endif

#endif /* ISCSI_IBFT_H */
