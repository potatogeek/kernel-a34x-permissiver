<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> upstream/android-13
/*
 * debugfs.c - ACPI debugfs interface to userspace.
 */

#include <linux/export.h>
#include <linux/init.h>
#include <linux/debugfs.h>
#include <linux/acpi.h>

#include "internal.h"

<<<<<<< HEAD
#define _COMPONENT		ACPI_SYSTEM_COMPONENT
ACPI_MODULE_NAME("debugfs");

=======
>>>>>>> upstream/android-13
struct dentry *acpi_debugfs_dir;
EXPORT_SYMBOL_GPL(acpi_debugfs_dir);

void __init acpi_debugfs_init(void)
{
	acpi_debugfs_dir = debugfs_create_dir("acpi", NULL);
}
