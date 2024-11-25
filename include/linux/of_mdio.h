<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> upstream/android-13
/*
 * OF helpers for the MDIO (Ethernet PHY) API
 *
 * Copyright (c) 2009 Secret Lab Technologies, Ltd.
<<<<<<< HEAD
 *
 * This file is released under the GPLv2
=======
>>>>>>> upstream/android-13
 */

#ifndef __LINUX_OF_MDIO_H
#define __LINUX_OF_MDIO_H

<<<<<<< HEAD
=======
#include <linux/device.h>
>>>>>>> upstream/android-13
#include <linux/phy.h>
#include <linux/of.h>

#if IS_ENABLED(CONFIG_OF_MDIO)
<<<<<<< HEAD
extern int of_mdiobus_register(struct mii_bus *mdio, struct device_node *np);
extern struct phy_device *of_phy_find_device(struct device_node *phy_np);
extern struct phy_device *of_phy_connect(struct net_device *dev,
					 struct device_node *phy_np,
					 void (*hndlr)(struct net_device *),
					 u32 flags, phy_interface_t iface);
extern struct phy_device *
of_phy_get_and_connect(struct net_device *dev, struct device_node *np,
		       void (*hndlr)(struct net_device *));
struct phy_device *of_phy_attach(struct net_device *dev,
				 struct device_node *phy_np, u32 flags,
				 phy_interface_t iface);

extern struct mii_bus *of_mdio_find_bus(struct device_node *mdio_np);
extern int of_phy_register_fixed_link(struct device_node *np);
extern void of_phy_deregister_fixed_link(struct device_node *np);
extern bool of_phy_is_fixed_link(struct device_node *np);

=======
bool of_mdiobus_child_is_phy(struct device_node *child);
int of_mdiobus_register(struct mii_bus *mdio, struct device_node *np);
int devm_of_mdiobus_register(struct device *dev, struct mii_bus *mdio,
			     struct device_node *np);
struct mdio_device *of_mdio_find_device(struct device_node *np);
struct phy_device *of_phy_find_device(struct device_node *phy_np);
struct phy_device *
of_phy_connect(struct net_device *dev, struct device_node *phy_np,
	       void (*hndlr)(struct net_device *), u32 flags,
	       phy_interface_t iface);
struct phy_device *
of_phy_get_and_connect(struct net_device *dev, struct device_node *np,
		       void (*hndlr)(struct net_device *));

struct mii_bus *of_mdio_find_bus(struct device_node *mdio_np);
int of_phy_register_fixed_link(struct device_node *np);
void of_phy_deregister_fixed_link(struct device_node *np);
bool of_phy_is_fixed_link(struct device_node *np);
int of_mdiobus_phy_device_register(struct mii_bus *mdio, struct phy_device *phy,
				   struct device_node *child, u32 addr);
>>>>>>> upstream/android-13

static inline int of_mdio_parse_addr(struct device *dev,
				     const struct device_node *np)
{
	u32 addr;
	int ret;

	ret = of_property_read_u32(np, "reg", &addr);
	if (ret < 0) {
		dev_err(dev, "%s has invalid PHY address\n", np->full_name);
		return ret;
	}

	/* A PHY must have a reg property in the range [0-31] */
	if (addr >= PHY_MAX_ADDR) {
		dev_err(dev, "%s PHY address %i is too large\n",
			np->full_name, addr);
		return -EINVAL;
	}

	return addr;
}

#else /* CONFIG_OF_MDIO */
<<<<<<< HEAD
=======
static inline bool of_mdiobus_child_is_phy(struct device_node *child)
{
	return false;
}

>>>>>>> upstream/android-13
static inline int of_mdiobus_register(struct mii_bus *mdio, struct device_node *np)
{
	/*
	 * Fall back to the non-DT function to register a bus.
	 * This way, we don't have to keep compat bits around in drivers.
	 */

	return mdiobus_register(mdio);
}

<<<<<<< HEAD
=======
static inline int devm_of_mdiobus_register(struct device *dev,
					   struct mii_bus *mdio,
					   struct device_node *np)
{
	return devm_mdiobus_register(dev, mdio);
}

static inline struct mdio_device *of_mdio_find_device(struct device_node *np)
{
	return NULL;
}

>>>>>>> upstream/android-13
static inline struct phy_device *of_phy_find_device(struct device_node *phy_np)
{
	return NULL;
}

static inline struct phy_device *of_phy_connect(struct net_device *dev,
						struct device_node *phy_np,
						void (*hndlr)(struct net_device *),
						u32 flags, phy_interface_t iface)
{
	return NULL;
}

static inline struct phy_device *
of_phy_get_and_connect(struct net_device *dev, struct device_node *np,
		       void (*hndlr)(struct net_device *))
{
	return NULL;
}

<<<<<<< HEAD
static inline struct phy_device *of_phy_attach(struct net_device *dev,
					       struct device_node *phy_np,
					       u32 flags, phy_interface_t iface)
{
	return NULL;
}

=======
>>>>>>> upstream/android-13
static inline struct mii_bus *of_mdio_find_bus(struct device_node *mdio_np)
{
	return NULL;
}

static inline int of_mdio_parse_addr(struct device *dev,
				     const struct device_node *np)
{
	return -ENOSYS;
}
static inline int of_phy_register_fixed_link(struct device_node *np)
{
	return -ENOSYS;
}
static inline void of_phy_deregister_fixed_link(struct device_node *np)
{
}
static inline bool of_phy_is_fixed_link(struct device_node *np)
{
	return false;
}
<<<<<<< HEAD
=======

static inline int of_mdiobus_phy_device_register(struct mii_bus *mdio,
					    struct phy_device *phy,
					    struct device_node *child, u32 addr)
{
	return -ENOSYS;
}
>>>>>>> upstream/android-13
#endif


#endif /* __LINUX_OF_MDIO_H */
