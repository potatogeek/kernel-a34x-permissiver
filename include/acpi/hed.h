<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> upstream/android-13
/*
 * hed.h - ACPI Hardware Error Device
 *
 * Copyright (C) 2009, Intel Corp.
 *	Author: Huang Ying <ying.huang@intel.com>
<<<<<<< HEAD
 *
 * This file is released under the GPLv2.
=======
>>>>>>> upstream/android-13
 */

#ifndef ACPI_HED_H
#define ACPI_HED_H

#include <linux/notifier.h>

int register_acpi_hed_notifier(struct notifier_block *nb);
void unregister_acpi_hed_notifier(struct notifier_block *nb);

#endif
