<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0+
>>>>>>> upstream/android-13
/*
 * Driver for AMD am79c PHYs
 *
 * Author: Heiko Schocher <hs@denx.de>
 *
 * Copyright (c) 2011 DENX Software Engineering GmbH
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General  Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
 *
=======
>>>>>>> upstream/android-13
 */
#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/mii.h>
#include <linux/phy.h>

#define PHY_ID_AM79C874		0x0022561b

#define MII_AM79C_IR		17	/* Interrupt Status/Control Register */
#define MII_AM79C_IR_EN_LINK	0x0400	/* IR enable Linkstate */
#define MII_AM79C_IR_EN_ANEG	0x0100	/* IR enable Aneg Complete */
#define MII_AM79C_IR_IMASK_INIT	(MII_AM79C_IR_EN_LINK | MII_AM79C_IR_EN_ANEG)

<<<<<<< HEAD
=======
#define MII_AM79C_IR_LINK_DOWN	BIT(2)
#define MII_AM79C_IR_ANEG_DONE	BIT(0)
#define MII_AM79C_IR_IMASK_STAT	(MII_AM79C_IR_LINK_DOWN | MII_AM79C_IR_ANEG_DONE)

>>>>>>> upstream/android-13
MODULE_DESCRIPTION("AMD PHY driver");
MODULE_AUTHOR("Heiko Schocher <hs@denx.de>");
MODULE_LICENSE("GPL");

static int am79c_ack_interrupt(struct phy_device *phydev)
{
	int err;

	err = phy_read(phydev, MII_BMSR);
	if (err < 0)
		return err;

	err = phy_read(phydev, MII_AM79C_IR);
	if (err < 0)
		return err;

	return 0;
}

static int am79c_config_init(struct phy_device *phydev)
{
	return 0;
}

static int am79c_config_intr(struct phy_device *phydev)
{
	int err;

<<<<<<< HEAD
	if (phydev->interrupts == PHY_INTERRUPT_ENABLED)
		err = phy_write(phydev, MII_AM79C_IR, MII_AM79C_IR_IMASK_INIT);
	else
		err = phy_write(phydev, MII_AM79C_IR, 0);
=======
	if (phydev->interrupts == PHY_INTERRUPT_ENABLED) {
		err = am79c_ack_interrupt(phydev);
		if (err)
			return err;

		err = phy_write(phydev, MII_AM79C_IR, MII_AM79C_IR_IMASK_INIT);
	} else {
		err = phy_write(phydev, MII_AM79C_IR, 0);
		if (err)
			return err;

		err = am79c_ack_interrupt(phydev);
	}
>>>>>>> upstream/android-13

	return err;
}

<<<<<<< HEAD
=======
static irqreturn_t am79c_handle_interrupt(struct phy_device *phydev)
{
	int irq_status;

	irq_status = phy_read(phydev, MII_AM79C_IR);
	if (irq_status < 0) {
		phy_error(phydev);
		return IRQ_NONE;
	}

	if (!(irq_status & MII_AM79C_IR_IMASK_STAT))
		return IRQ_NONE;

	phy_trigger_machine(phydev);

	return IRQ_HANDLED;
}

>>>>>>> upstream/android-13
static struct phy_driver am79c_driver[] = { {
	.phy_id		= PHY_ID_AM79C874,
	.name		= "AM79C874",
	.phy_id_mask	= 0xfffffff0,
<<<<<<< HEAD
	.features	= PHY_BASIC_FEATURES,
	.flags		= PHY_HAS_INTERRUPT,
	.config_init	= am79c_config_init,
	.ack_interrupt	= am79c_ack_interrupt,
	.config_intr	= am79c_config_intr,
=======
	/* PHY_BASIC_FEATURES */
	.config_init	= am79c_config_init,
	.config_intr	= am79c_config_intr,
	.handle_interrupt = am79c_handle_interrupt,
>>>>>>> upstream/android-13
} };

module_phy_driver(am79c_driver);

static struct mdio_device_id __maybe_unused amd_tbl[] = {
	{ PHY_ID_AM79C874, 0xfffffff0 },
	{ }
};

MODULE_DEVICE_TABLE(mdio, amd_tbl);
