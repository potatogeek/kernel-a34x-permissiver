<<<<<<< HEAD
/*
 *	Driver for Broadcom 63xx SOCs integrated PHYs
 *
 *	This program is free software; you can redistribute it and/or
 *	modify it under the terms of the GNU General Public License
 *	as published by the Free Software Foundation; either version
 *	2 of the License, or (at your option) any later version.
=======
// SPDX-License-Identifier: GPL-2.0+
/*
 *	Driver for Broadcom 63xx SOCs integrated PHYs
>>>>>>> upstream/android-13
 */
#include "bcm-phy-lib.h"
#include <linux/module.h>
#include <linux/phy.h>

#define MII_BCM63XX_IR		0x1a	/* interrupt register */
#define MII_BCM63XX_IR_EN	0x4000	/* global interrupt enable */
#define MII_BCM63XX_IR_DUPLEX	0x0800	/* duplex changed */
#define MII_BCM63XX_IR_SPEED	0x0400	/* speed changed */
#define MII_BCM63XX_IR_LINK	0x0200	/* link changed */
#define MII_BCM63XX_IR_GMASK	0x0100	/* global interrupt mask */

MODULE_DESCRIPTION("Broadcom 63xx internal PHY driver");
MODULE_AUTHOR("Maxime Bizon <mbizon@freebox.fr>");
MODULE_LICENSE("GPL");

static int bcm63xx_config_intr(struct phy_device *phydev)
{
	int reg, err;

	reg = phy_read(phydev, MII_BCM63XX_IR);
	if (reg < 0)
		return reg;

<<<<<<< HEAD
	if (phydev->interrupts == PHY_INTERRUPT_ENABLED)
		reg &= ~MII_BCM63XX_IR_GMASK;
	else
		reg |= MII_BCM63XX_IR_GMASK;

	err = phy_write(phydev, MII_BCM63XX_IR, reg);
=======
	if (phydev->interrupts == PHY_INTERRUPT_ENABLED) {
		err = bcm_phy_ack_intr(phydev);
		if (err)
			return err;

		reg &= ~MII_BCM63XX_IR_GMASK;
		err = phy_write(phydev, MII_BCM63XX_IR, reg);
	} else {
		reg |= MII_BCM63XX_IR_GMASK;
		err = phy_write(phydev, MII_BCM63XX_IR, reg);
		if (err)
			return err;

		err = bcm_phy_ack_intr(phydev);
	}

>>>>>>> upstream/android-13
	return err;
}

static int bcm63xx_config_init(struct phy_device *phydev)
{
	int reg, err;

<<<<<<< HEAD
=======
	/* ASYM_PAUSE bit is marked RO in datasheet, so don't cheat */
	linkmode_set_bit(ETHTOOL_LINK_MODE_Pause_BIT, phydev->supported);

>>>>>>> upstream/android-13
	reg = phy_read(phydev, MII_BCM63XX_IR);
	if (reg < 0)
		return reg;

	/* Mask interrupts globally.  */
	reg |= MII_BCM63XX_IR_GMASK;
	err = phy_write(phydev, MII_BCM63XX_IR, reg);
	if (err < 0)
		return err;

	/* Unmask events we are interested in  */
	reg = ~(MII_BCM63XX_IR_DUPLEX |
		MII_BCM63XX_IR_SPEED |
		MII_BCM63XX_IR_LINK) |
		MII_BCM63XX_IR_EN;
	return phy_write(phydev, MII_BCM63XX_IR, reg);
}

static struct phy_driver bcm63xx_driver[] = {
{
	.phy_id		= 0x00406000,
	.phy_id_mask	= 0xfffffc00,
	.name		= "Broadcom BCM63XX (1)",
<<<<<<< HEAD
	/* ASYM_PAUSE bit is marked RO in datasheet, so don't cheat */
	.features	= (PHY_BASIC_FEATURES | SUPPORTED_Pause),
	.flags		= PHY_HAS_INTERRUPT | PHY_IS_INTERNAL,
	.config_init	= bcm63xx_config_init,
	.ack_interrupt	= bcm_phy_ack_intr,
	.config_intr	= bcm63xx_config_intr,
=======
	/* PHY_BASIC_FEATURES */
	.flags		= PHY_IS_INTERNAL,
	.config_init	= bcm63xx_config_init,
	.config_intr	= bcm63xx_config_intr,
	.handle_interrupt = bcm_phy_handle_interrupt,
>>>>>>> upstream/android-13
}, {
	/* same phy as above, with just a different OUI */
	.phy_id		= 0x002bdc00,
	.phy_id_mask	= 0xfffffc00,
	.name		= "Broadcom BCM63XX (2)",
<<<<<<< HEAD
	.features	= (PHY_BASIC_FEATURES | SUPPORTED_Pause),
	.flags		= PHY_HAS_INTERRUPT | PHY_IS_INTERNAL,
	.config_init	= bcm63xx_config_init,
	.ack_interrupt	= bcm_phy_ack_intr,
	.config_intr	= bcm63xx_config_intr,
=======
	/* PHY_BASIC_FEATURES */
	.flags		= PHY_IS_INTERNAL,
	.config_init	= bcm63xx_config_init,
	.config_intr	= bcm63xx_config_intr,
	.handle_interrupt = bcm_phy_handle_interrupt,
>>>>>>> upstream/android-13
} };

module_phy_driver(bcm63xx_driver);

static struct mdio_device_id __maybe_unused bcm63xx_tbl[] = {
	{ 0x00406000, 0xfffffc00 },
	{ 0x002bdc00, 0xfffffc00 },
	{ }
};

MODULE_DEVICE_TABLE(mdio, bcm63xx_tbl);
