<<<<<<< HEAD
/*
 * net/dsa/mv88e6060.c - Driver for Marvell 88e6060 switch chips
 * Copyright (c) 2008-2009 Marvell Semiconductor
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
=======
// SPDX-License-Identifier: GPL-2.0+
/*
 * net/dsa/mv88e6060.c - Driver for Marvell 88e6060 switch chips
 * Copyright (c) 2008-2009 Marvell Semiconductor
>>>>>>> upstream/android-13
 */

#include <linux/delay.h>
#include <linux/etherdevice.h>
#include <linux/jiffies.h>
#include <linux/list.h>
#include <linux/module.h>
#include <linux/netdevice.h>
#include <linux/phy.h>
#include <net/dsa.h>
#include "mv88e6060.h"

<<<<<<< HEAD
static int reg_read(struct dsa_switch *ds, int addr, int reg)
{
	struct mv88e6060_priv *priv = ds->priv;

	return mdiobus_read_nested(priv->bus, priv->sw_addr + addr, reg);
}

#define REG_READ(addr, reg)					\
	({							\
		int __ret;					\
								\
		__ret = reg_read(ds, addr, reg);		\
		if (__ret < 0)					\
			return __ret;				\
		__ret;						\
	})


static int reg_write(struct dsa_switch *ds, int addr, int reg, u16 val)
{
	struct mv88e6060_priv *priv = ds->priv;

	return mdiobus_write_nested(priv->bus, priv->sw_addr + addr, reg, val);
}

#define REG_WRITE(addr, reg, val)				\
	({							\
		int __ret;					\
								\
		__ret = reg_write(ds, addr, reg, val);		\
		if (__ret < 0)					\
			return __ret;				\
	})

=======
static int reg_read(struct mv88e6060_priv *priv, int addr, int reg)
{
	return mdiobus_read_nested(priv->bus, priv->sw_addr + addr, reg);
}

static int reg_write(struct mv88e6060_priv *priv, int addr, int reg, u16 val)
{
	return mdiobus_write_nested(priv->bus, priv->sw_addr + addr, reg, val);
}

>>>>>>> upstream/android-13
static const char *mv88e6060_get_name(struct mii_bus *bus, int sw_addr)
{
	int ret;

	ret = mdiobus_read(bus, sw_addr + REG_PORT(0), PORT_SWITCH_ID);
	if (ret >= 0) {
		if (ret == PORT_SWITCH_ID_6060)
			return "Marvell 88E6060 (A0)";
		if (ret == PORT_SWITCH_ID_6060_R1 ||
		    ret == PORT_SWITCH_ID_6060_R2)
			return "Marvell 88E6060 (B0)";
		if ((ret & PORT_SWITCH_ID_6060_MASK) == PORT_SWITCH_ID_6060)
			return "Marvell 88E6060";
	}

	return NULL;
}

static enum dsa_tag_protocol mv88e6060_get_tag_protocol(struct dsa_switch *ds,
<<<<<<< HEAD
							int port)
=======
							int port,
							enum dsa_tag_protocol m)
>>>>>>> upstream/android-13
{
	return DSA_TAG_PROTO_TRAILER;
}

<<<<<<< HEAD
static const char *mv88e6060_drv_probe(struct device *dsa_dev,
				       struct device *host_dev, int sw_addr,
				       void **_priv)
{
	struct mii_bus *bus = dsa_host_dev_to_mii_bus(host_dev);
	struct mv88e6060_priv *priv;
	const char *name;

	name = mv88e6060_get_name(bus, sw_addr);
	if (name) {
		priv = devm_kzalloc(dsa_dev, sizeof(*priv), GFP_KERNEL);
		if (!priv)
			return NULL;
		*_priv = priv;
		priv->bus = bus;
		priv->sw_addr = sw_addr;
	}

	return name;
}

static int mv88e6060_switch_reset(struct dsa_switch *ds)
=======
static int mv88e6060_switch_reset(struct mv88e6060_priv *priv)
>>>>>>> upstream/android-13
{
	int i;
	int ret;
	unsigned long timeout;

	/* Set all ports to the disabled state. */
	for (i = 0; i < MV88E6060_PORTS; i++) {
<<<<<<< HEAD
		ret = REG_READ(REG_PORT(i), PORT_CONTROL);
		REG_WRITE(REG_PORT(i), PORT_CONTROL,
			  ret & ~PORT_CONTROL_STATE_MASK);
=======
		ret = reg_read(priv, REG_PORT(i), PORT_CONTROL);
		if (ret < 0)
			return ret;
		ret = reg_write(priv, REG_PORT(i), PORT_CONTROL,
				ret & ~PORT_CONTROL_STATE_MASK);
		if (ret)
			return ret;
>>>>>>> upstream/android-13
	}

	/* Wait for transmit queues to drain. */
	usleep_range(2000, 4000);

	/* Reset the switch. */
<<<<<<< HEAD
	REG_WRITE(REG_GLOBAL, GLOBAL_ATU_CONTROL,
		  GLOBAL_ATU_CONTROL_SWRESET |
		  GLOBAL_ATU_CONTROL_LEARNDIS);
=======
	ret = reg_write(priv, REG_GLOBAL, GLOBAL_ATU_CONTROL,
			GLOBAL_ATU_CONTROL_SWRESET |
			GLOBAL_ATU_CONTROL_LEARNDIS);
	if (ret)
		return ret;
>>>>>>> upstream/android-13

	/* Wait up to one second for reset to complete. */
	timeout = jiffies + 1 * HZ;
	while (time_before(jiffies, timeout)) {
<<<<<<< HEAD
		ret = REG_READ(REG_GLOBAL, GLOBAL_STATUS);
=======
		ret = reg_read(priv, REG_GLOBAL, GLOBAL_STATUS);
		if (ret < 0)
			return ret;

>>>>>>> upstream/android-13
		if (ret & GLOBAL_STATUS_INIT_READY)
			break;

		usleep_range(1000, 2000);
	}
	if (time_after(jiffies, timeout))
		return -ETIMEDOUT;

	return 0;
}

<<<<<<< HEAD
static int mv88e6060_setup_global(struct dsa_switch *ds)
{
=======
static int mv88e6060_setup_global(struct mv88e6060_priv *priv)
{
	int ret;

>>>>>>> upstream/android-13
	/* Disable discarding of frames with excessive collisions,
	 * set the maximum frame size to 1536 bytes, and mask all
	 * interrupt sources.
	 */
<<<<<<< HEAD
	REG_WRITE(REG_GLOBAL, GLOBAL_CONTROL, GLOBAL_CONTROL_MAX_FRAME_1536);

	/* Disable automatic address learning.
	 */
	REG_WRITE(REG_GLOBAL, GLOBAL_ATU_CONTROL,
		  GLOBAL_ATU_CONTROL_LEARNDIS);

	return 0;
}

static int mv88e6060_setup_port(struct dsa_switch *ds, int p)
{
	int addr = REG_PORT(p);
=======
	ret = reg_write(priv, REG_GLOBAL, GLOBAL_CONTROL,
			GLOBAL_CONTROL_MAX_FRAME_1536);
	if (ret)
		return ret;

	/* Disable automatic address learning.
	 */
	return reg_write(priv, REG_GLOBAL, GLOBAL_ATU_CONTROL,
			 GLOBAL_ATU_CONTROL_LEARNDIS);
}

static int mv88e6060_setup_port(struct mv88e6060_priv *priv, int p)
{
	int addr = REG_PORT(p);
	int ret;
>>>>>>> upstream/android-13

	/* Do not force flow control, disable Ingress and Egress
	 * Header tagging, disable VLAN tunneling, and set the port
	 * state to Forwarding.  Additionally, if this is the CPU
	 * port, enable Ingress and Egress Trailer tagging mode.
	 */
<<<<<<< HEAD
	REG_WRITE(addr, PORT_CONTROL,
		  dsa_is_cpu_port(ds, p) ?
=======
	ret = reg_write(priv, addr, PORT_CONTROL,
			dsa_is_cpu_port(priv->ds, p) ?
>>>>>>> upstream/android-13
			PORT_CONTROL_TRAILER |
			PORT_CONTROL_INGRESS_MODE |
			PORT_CONTROL_STATE_FORWARDING :
			PORT_CONTROL_STATE_FORWARDING);
<<<<<<< HEAD
=======
	if (ret)
		return ret;
>>>>>>> upstream/android-13

	/* Port based VLAN map: give each port its own address
	 * database, allow the CPU port to talk to each of the 'real'
	 * ports, and allow each of the 'real' ports to only talk to
	 * the CPU port.
	 */
<<<<<<< HEAD
	REG_WRITE(addr, PORT_VLAN_MAP,
		  ((p & 0xf) << PORT_VLAN_MAP_DBNUM_SHIFT) |
		   (dsa_is_cpu_port(ds, p) ? dsa_user_ports(ds) :
		    BIT(dsa_to_port(ds, p)->cpu_dp->index)));
=======
	ret = reg_write(priv, addr, PORT_VLAN_MAP,
			((p & 0xf) << PORT_VLAN_MAP_DBNUM_SHIFT) |
			(dsa_is_cpu_port(priv->ds, p) ?
			 dsa_user_ports(priv->ds) :
			 BIT(dsa_to_port(priv->ds, p)->cpu_dp->index)));
	if (ret)
		return ret;
>>>>>>> upstream/android-13

	/* Port Association Vector: when learning source addresses
	 * of packets, add the address to the address database using
	 * a port bitmap that has only the bit for this port set and
	 * the other bits clear.
	 */
<<<<<<< HEAD
	REG_WRITE(addr, PORT_ASSOC_VECTOR, BIT(p));

	return 0;
}

static int mv88e6060_setup_addr(struct dsa_switch *ds)
{
	u8 addr[ETH_ALEN];
=======
	return reg_write(priv, addr, PORT_ASSOC_VECTOR, BIT(p));
}

static int mv88e6060_setup_addr(struct mv88e6060_priv *priv)
{
	u8 addr[ETH_ALEN];
	int ret;
>>>>>>> upstream/android-13
	u16 val;

	eth_random_addr(addr);

	val = addr[0] << 8 | addr[1];

	/* The multicast bit is always transmitted as a zero, so the switch uses
	 * bit 8 for "DiffAddr", where 0 means all ports transmit the same SA.
	 */
	val &= 0xfeff;

<<<<<<< HEAD
	REG_WRITE(REG_GLOBAL, GLOBAL_MAC_01, val);
	REG_WRITE(REG_GLOBAL, GLOBAL_MAC_23, (addr[2] << 8) | addr[3]);
	REG_WRITE(REG_GLOBAL, GLOBAL_MAC_45, (addr[4] << 8) | addr[5]);

	return 0;
=======
	ret = reg_write(priv, REG_GLOBAL, GLOBAL_MAC_01, val);
	if (ret)
		return ret;

	ret = reg_write(priv, REG_GLOBAL, GLOBAL_MAC_23,
			(addr[2] << 8) | addr[3]);
	if (ret)
		return ret;

	return reg_write(priv, REG_GLOBAL, GLOBAL_MAC_45,
			 (addr[4] << 8) | addr[5]);
>>>>>>> upstream/android-13
}

static int mv88e6060_setup(struct dsa_switch *ds)
{
<<<<<<< HEAD
	int ret;
	int i;

	ret = mv88e6060_switch_reset(ds);
=======
	struct mv88e6060_priv *priv = ds->priv;
	int ret;
	int i;

	priv->ds = ds;

	ret = mv88e6060_switch_reset(priv);
>>>>>>> upstream/android-13
	if (ret < 0)
		return ret;

	/* @@@ initialise atu */

<<<<<<< HEAD
	ret = mv88e6060_setup_global(ds);
	if (ret < 0)
		return ret;

	ret = mv88e6060_setup_addr(ds);
=======
	ret = mv88e6060_setup_global(priv);
	if (ret < 0)
		return ret;

	ret = mv88e6060_setup_addr(priv);
>>>>>>> upstream/android-13
	if (ret < 0)
		return ret;

	for (i = 0; i < MV88E6060_PORTS; i++) {
<<<<<<< HEAD
		ret = mv88e6060_setup_port(ds, i);
=======
		ret = mv88e6060_setup_port(priv, i);
>>>>>>> upstream/android-13
		if (ret < 0)
			return ret;
	}

	return 0;
}

static int mv88e6060_port_to_phy_addr(int port)
{
	if (port >= 0 && port < MV88E6060_PORTS)
		return port;
	return -1;
}

static int mv88e6060_phy_read(struct dsa_switch *ds, int port, int regnum)
{
<<<<<<< HEAD
=======
	struct mv88e6060_priv *priv = ds->priv;
>>>>>>> upstream/android-13
	int addr;

	addr = mv88e6060_port_to_phy_addr(port);
	if (addr == -1)
		return 0xffff;

<<<<<<< HEAD
	return reg_read(ds, addr, regnum);
=======
	return reg_read(priv, addr, regnum);
>>>>>>> upstream/android-13
}

static int
mv88e6060_phy_write(struct dsa_switch *ds, int port, int regnum, u16 val)
{
<<<<<<< HEAD
=======
	struct mv88e6060_priv *priv = ds->priv;
>>>>>>> upstream/android-13
	int addr;

	addr = mv88e6060_port_to_phy_addr(port);
	if (addr == -1)
		return 0xffff;

<<<<<<< HEAD
	return reg_write(ds, addr, regnum, val);
=======
	return reg_write(priv, addr, regnum, val);
>>>>>>> upstream/android-13
}

static const struct dsa_switch_ops mv88e6060_switch_ops = {
	.get_tag_protocol = mv88e6060_get_tag_protocol,
<<<<<<< HEAD
	.probe		= mv88e6060_drv_probe,
=======
>>>>>>> upstream/android-13
	.setup		= mv88e6060_setup,
	.phy_read	= mv88e6060_phy_read,
	.phy_write	= mv88e6060_phy_write,
};

<<<<<<< HEAD
static struct dsa_switch_driver mv88e6060_switch_drv = {
	.ops		= &mv88e6060_switch_ops,
};

static int __init mv88e6060_init(void)
{
	register_switch_driver(&mv88e6060_switch_drv);
	return 0;
}
module_init(mv88e6060_init);

static void __exit mv88e6060_cleanup(void)
{
	unregister_switch_driver(&mv88e6060_switch_drv);
}
module_exit(mv88e6060_cleanup);
=======
static int mv88e6060_probe(struct mdio_device *mdiodev)
{
	struct device *dev = &mdiodev->dev;
	struct mv88e6060_priv *priv;
	struct dsa_switch *ds;
	const char *name;

	priv = devm_kzalloc(dev, sizeof(*priv), GFP_KERNEL);
	if (!priv)
		return -ENOMEM;

	priv->bus = mdiodev->bus;
	priv->sw_addr = mdiodev->addr;

	name = mv88e6060_get_name(priv->bus, priv->sw_addr);
	if (!name)
		return -ENODEV;

	dev_info(dev, "switch %s detected\n", name);

	ds = devm_kzalloc(dev, sizeof(*ds), GFP_KERNEL);
	if (!ds)
		return -ENOMEM;

	ds->dev = dev;
	ds->num_ports = MV88E6060_PORTS;
	ds->priv = priv;
	ds->dev = dev;
	ds->ops = &mv88e6060_switch_ops;

	dev_set_drvdata(dev, ds);

	return dsa_register_switch(ds);
}

static void mv88e6060_remove(struct mdio_device *mdiodev)
{
	struct dsa_switch *ds = dev_get_drvdata(&mdiodev->dev);

	if (!ds)
		return;

	dsa_unregister_switch(ds);

	dev_set_drvdata(&mdiodev->dev, NULL);
}

static void mv88e6060_shutdown(struct mdio_device *mdiodev)
{
	struct dsa_switch *ds = dev_get_drvdata(&mdiodev->dev);

	if (!ds)
		return;

	dsa_switch_shutdown(ds);

	dev_set_drvdata(&mdiodev->dev, NULL);
}

static const struct of_device_id mv88e6060_of_match[] = {
	{
		.compatible = "marvell,mv88e6060",
	},
	{ /* sentinel */ },
};

static struct mdio_driver mv88e6060_driver = {
	.probe	= mv88e6060_probe,
	.remove = mv88e6060_remove,
	.shutdown = mv88e6060_shutdown,
	.mdiodrv.driver = {
		.name = "mv88e6060",
		.of_match_table = mv88e6060_of_match,
	},
};

mdio_module_driver(mv88e6060_driver);
>>>>>>> upstream/android-13

MODULE_AUTHOR("Lennert Buytenhek <buytenh@wantstofly.org>");
MODULE_DESCRIPTION("Driver for Marvell 88E6060 ethernet switch chip");
MODULE_LICENSE("GPL");
MODULE_ALIAS("platform:mv88e6060");
