<<<<<<< HEAD
/*
 * OF helpers for network devices.
 *
 * This file is released under the GPLv2
=======
/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * OF helpers for network devices.
>>>>>>> upstream/android-13
 */

#ifndef __LINUX_OF_NET_H
#define __LINUX_OF_NET_H

<<<<<<< HEAD
#ifdef CONFIG_OF_NET
#include <linux/of.h>

struct net_device;
extern int of_get_phy_mode(struct device_node *np);
extern const void *of_get_mac_address(struct device_node *np);
extern int of_get_nvmem_mac_address(struct device_node *np, void *addr);
extern struct net_device *of_find_net_device_by_node(struct device_node *np);
#else
static inline int of_get_phy_mode(struct device_node *np)
=======
#include <linux/phy.h>

#if defined(CONFIG_OF) && defined(CONFIG_NET)
#include <linux/of.h>

struct net_device;
extern int of_get_phy_mode(struct device_node *np, phy_interface_t *interface);
extern int of_get_mac_address(struct device_node *np, u8 *mac);
extern struct net_device *of_find_net_device_by_node(struct device_node *np);
#else
static inline int of_get_phy_mode(struct device_node *np,
				  phy_interface_t *interface)
>>>>>>> upstream/android-13
{
	return -ENODEV;
}

<<<<<<< HEAD
static inline const void *of_get_mac_address(struct device_node *np)
{
	return NULL;
}

static inline int of_get_nvmem_mac_address(struct device_node *np, void *addr)
=======
static inline int of_get_mac_address(struct device_node *np, u8 *mac)
>>>>>>> upstream/android-13
{
	return -ENODEV;
}

static inline struct net_device *of_find_net_device_by_node(struct device_node *np)
{
	return NULL;
}
#endif

#endif /* __LINUX_OF_NET_H */
