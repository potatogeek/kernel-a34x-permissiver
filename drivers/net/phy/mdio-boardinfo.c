<<<<<<< HEAD
/*
 * mdio-boardinfo - Collect pre-declarations for MDIO devices
 *
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General  Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
=======
// SPDX-License-Identifier: GPL-2.0+
/*
 * mdio-boardinfo - Collect pre-declarations for MDIO devices
>>>>>>> upstream/android-13
 */

#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/export.h>
#include <linux/mutex.h>
#include <linux/list.h>

#include "mdio-boardinfo.h"

static LIST_HEAD(mdio_board_list);
static DEFINE_MUTEX(mdio_board_lock);

/**
 * mdiobus_setup_mdiodev_from_board_info - create and setup MDIO devices
 * from pre-collected board specific MDIO information
<<<<<<< HEAD
 * @mdiodev: MDIO device pointer
=======
 * @bus: Bus the board_info belongs to
 * @cb: Callback to create device on bus
>>>>>>> upstream/android-13
 * Context: can sleep
 */
void mdiobus_setup_mdiodev_from_board_info(struct mii_bus *bus,
					   int (*cb)
					   (struct mii_bus *bus,
					    struct mdio_board_info *bi))
{
	struct mdio_board_entry *be;
	struct mdio_board_entry *tmp;
	struct mdio_board_info *bi;
	int ret;

	mutex_lock(&mdio_board_lock);
	list_for_each_entry_safe(be, tmp, &mdio_board_list, list) {
		bi = &be->board_info;

		if (strcmp(bus->id, bi->bus_id))
			continue;

		mutex_unlock(&mdio_board_lock);
		ret = cb(bus, bi);
		mutex_lock(&mdio_board_lock);
		if (ret)
			continue;

	}
	mutex_unlock(&mdio_board_lock);
}
EXPORT_SYMBOL(mdiobus_setup_mdiodev_from_board_info);

/**
<<<<<<< HEAD
 * mdio_register_board_info - register MDIO devices for a given board
=======
 * mdiobus_register_board_info - register MDIO devices for a given board
>>>>>>> upstream/android-13
 * @info: array of devices descriptors
 * @n: number of descriptors provided
 * Context: can sleep
 *
 * The board info passed can be marked with __initdata but be pointers
 * such as platform_data etc. are copied as-is
 */
int mdiobus_register_board_info(const struct mdio_board_info *info,
				unsigned int n)
{
	struct mdio_board_entry *be;
	unsigned int i;

	be = kcalloc(n, sizeof(*be), GFP_KERNEL);
	if (!be)
		return -ENOMEM;

	for (i = 0; i < n; i++, be++, info++) {
		memcpy(&be->board_info, info, sizeof(*info));
		mutex_lock(&mdio_board_lock);
		list_add_tail(&be->list, &mdio_board_list);
		mutex_unlock(&mdio_board_lock);
	}

	return 0;
}
EXPORT_SYMBOL(mdiobus_register_board_info);
