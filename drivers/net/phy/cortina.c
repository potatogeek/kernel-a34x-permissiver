<<<<<<< HEAD
/*
 *    Copyright 2017 NXP
 *
 *    This program is free software; you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation; either version 2 of the License, or
 *    (at your option) any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
=======
// SPDX-License-Identifier: GPL-2.0+
/*
 *    Copyright 2017 NXP
 *
>>>>>>> upstream/android-13
 *    CORTINA is a registered trademark of Cortina Systems, Inc.
 *
 */
#include <linux/module.h>
#include <linux/phy.h>

#define PHY_ID_CS4340	0x13e51002

#define VILLA_GLOBAL_CHIP_ID_LSB			0x0
#define VILLA_GLOBAL_CHIP_ID_MSB			0x1

#define VILLA_GLOBAL_GPIO_1_INTS			0x017

static int cortina_read_reg(struct phy_device *phydev, u16 regnum)
{
<<<<<<< HEAD
	return mdiobus_read(phydev->mdio.bus, phydev->mdio.addr,
			    MII_ADDR_C45 | regnum);
=======
	return mdiobus_c45_read(phydev->mdio.bus, phydev->mdio.addr, 0, regnum);
>>>>>>> upstream/android-13
}

static int cortina_read_status(struct phy_device *phydev)
{
	int gpio_int_status, ret = 0;

	gpio_int_status = cortina_read_reg(phydev, VILLA_GLOBAL_GPIO_1_INTS);
	if (gpio_int_status < 0) {
		ret = gpio_int_status;
		goto err;
	}

	if (gpio_int_status & 0x8) {
		/* up when edc_convergedS set */
		phydev->speed = SPEED_10000;
		phydev->duplex = DUPLEX_FULL;
		phydev->link = 1;
	} else {
		phydev->link = 0;
	}

err:
	return ret;
}

static int cortina_probe(struct phy_device *phydev)
{
	u32 phy_id = 0;
	int id_lsb = 0, id_msb = 0;

	/* Read device id from phy registers. */
	id_lsb = cortina_read_reg(phydev, VILLA_GLOBAL_CHIP_ID_LSB);
	if (id_lsb < 0)
		return -ENXIO;

	phy_id = id_lsb << 16;

	id_msb = cortina_read_reg(phydev, VILLA_GLOBAL_CHIP_ID_MSB);
	if (id_msb < 0)
		return -ENXIO;

	phy_id |= id_msb;

	/* Make sure the device tree binding matched the driver with the
	 * right device.
	 */
	if (phy_id != phydev->drv->phy_id) {
		phydev_err(phydev, "Error matching phy with %s driver\n",
			   phydev->drv->name);
		return -ENODEV;
	}

	return 0;
}

static struct phy_driver cortina_driver[] = {
{
	.phy_id		= PHY_ID_CS4340,
	.phy_id_mask	= 0xffffffff,
	.name		= "Cortina CS4340",
<<<<<<< HEAD
	.config_init	= gen10g_config_init,
	.config_aneg	= gen10g_config_aneg,
	.read_status	= cortina_read_status,
	.soft_reset	= gen10g_no_soft_reset,
=======
	.features       = PHY_10GBIT_FEATURES,
	.config_aneg	= gen10g_config_aneg,
	.read_status	= cortina_read_status,
>>>>>>> upstream/android-13
	.probe		= cortina_probe,
},
};

module_phy_driver(cortina_driver);

static struct mdio_device_id __maybe_unused cortina_tbl[] = {
	{ PHY_ID_CS4340, 0xffffffff},
	{},
};

MODULE_DEVICE_TABLE(mdio, cortina_tbl);

MODULE_DESCRIPTION("Cortina EDC CDR 10G Ethernet PHY driver");
MODULE_AUTHOR("NXP");
MODULE_LICENSE("GPL");
