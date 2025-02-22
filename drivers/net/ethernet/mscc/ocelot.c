// SPDX-License-Identifier: (GPL-2.0 OR MIT)
/*
 * Microsemi Ocelot Switch driver
 *
 * Copyright (c) 2017 Microsemi Corporation
 */
<<<<<<< HEAD
#include <linux/etherdevice.h>
#include <linux/ethtool.h>
#include <linux/if_bridge.h>
#include <linux/if_ether.h>
#include <linux/if_vlan.h>
#include <linux/interrupt.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/netdevice.h>
#include <linux/phy.h>
#include <linux/skbuff.h>
#include <net/arp.h>
#include <net/netevent.h>
#include <net/rtnetlink.h>
#include <net/switchdev.h>

#include "ocelot.h"

/* MAC table entry types.
 * ENTRYTYPE_NORMAL is subject to aging.
 * ENTRYTYPE_LOCKED is not subject to aging.
 * ENTRYTYPE_MACv4 is not subject to aging. For IPv4 multicast.
 * ENTRYTYPE_MACv6 is not subject to aging. For IPv6 multicast.
 */
enum macaccess_entry_type {
	ENTRYTYPE_NORMAL = 0,
	ENTRYTYPE_LOCKED,
	ENTRYTYPE_MACv4,
	ENTRYTYPE_MACv6,
};
=======
#include <linux/dsa/ocelot.h>
#include <linux/if_bridge.h>
#include <linux/ptp_classify.h>
#include <soc/mscc/ocelot_vcap.h>
#include "ocelot.h"
#include "ocelot_vcap.h"

#define TABLE_UPDATE_SLEEP_US 10
#define TABLE_UPDATE_TIMEOUT_US 100000
>>>>>>> upstream/android-13

struct ocelot_mact_entry {
	u8 mac[ETH_ALEN];
	u16 vid;
	enum macaccess_entry_type type;
};

<<<<<<< HEAD
static inline int ocelot_mact_wait_for_completion(struct ocelot *ocelot)
{
	unsigned int val, timeout = 10;

	/* Wait for the issued mac table command to be completed, or timeout.
	 * When the command read from  ANA_TABLES_MACACCESS is
	 * MACACCESS_CMD_IDLE, the issued command completed successfully.
	 */
	do {
		val = ocelot_read(ocelot, ANA_TABLES_MACACCESS);
		val &= ANA_TABLES_MACACCESS_MAC_TABLE_CMD_M;
	} while (val != MACACCESS_CMD_IDLE && timeout--);

	if (!timeout)
		return -ETIMEDOUT;

	return 0;
=======
static inline u32 ocelot_mact_read_macaccess(struct ocelot *ocelot)
{
	return ocelot_read(ocelot, ANA_TABLES_MACACCESS);
}

static inline int ocelot_mact_wait_for_completion(struct ocelot *ocelot)
{
	u32 val;

	return readx_poll_timeout(ocelot_mact_read_macaccess,
		ocelot, val,
		(val & ANA_TABLES_MACACCESS_MAC_TABLE_CMD_M) ==
		MACACCESS_CMD_IDLE,
		TABLE_UPDATE_SLEEP_US, TABLE_UPDATE_TIMEOUT_US);
>>>>>>> upstream/android-13
}

static void ocelot_mact_select(struct ocelot *ocelot,
			       const unsigned char mac[ETH_ALEN],
			       unsigned int vid)
{
	u32 macl = 0, mach = 0;

	/* Set the MAC address to handle and the vlan associated in a format
	 * understood by the hardware.
	 */
	mach |= vid    << 16;
	mach |= mac[0] << 8;
	mach |= mac[1] << 0;
	macl |= mac[2] << 24;
	macl |= mac[3] << 16;
	macl |= mac[4] << 8;
	macl |= mac[5] << 0;

	ocelot_write(ocelot, macl, ANA_TABLES_MACLDATA);
	ocelot_write(ocelot, mach, ANA_TABLES_MACHDATA);

}

<<<<<<< HEAD
static int ocelot_mact_learn(struct ocelot *ocelot, int port,
			     const unsigned char mac[ETH_ALEN],
			     unsigned int vid,
			     enum macaccess_entry_type type)
{
	ocelot_mact_select(ocelot, mac, vid);

	/* Issue a write command */
	ocelot_write(ocelot, ANA_TABLES_MACACCESS_VALID |
			     ANA_TABLES_MACACCESS_DEST_IDX(port) |
			     ANA_TABLES_MACACCESS_ENTRYTYPE(type) |
			     ANA_TABLES_MACACCESS_MAC_TABLE_CMD(MACACCESS_CMD_LEARN),
			     ANA_TABLES_MACACCESS);

	return ocelot_mact_wait_for_completion(ocelot);
}

static int ocelot_mact_forget(struct ocelot *ocelot,
			      const unsigned char mac[ETH_ALEN],
			      unsigned int vid)
=======
int ocelot_mact_learn(struct ocelot *ocelot, int port,
		      const unsigned char mac[ETH_ALEN],
		      unsigned int vid, enum macaccess_entry_type type)
{
	u32 cmd = ANA_TABLES_MACACCESS_VALID |
		ANA_TABLES_MACACCESS_DEST_IDX(port) |
		ANA_TABLES_MACACCESS_ENTRYTYPE(type) |
		ANA_TABLES_MACACCESS_MAC_TABLE_CMD(MACACCESS_CMD_LEARN);
	unsigned int mc_ports;

	/* Set MAC_CPU_COPY if the CPU port is used by a multicast entry */
	if (type == ENTRYTYPE_MACv4)
		mc_ports = (mac[1] << 8) | mac[2];
	else if (type == ENTRYTYPE_MACv6)
		mc_ports = (mac[0] << 8) | mac[1];
	else
		mc_ports = 0;

	if (mc_ports & BIT(ocelot->num_phys_ports))
		cmd |= ANA_TABLES_MACACCESS_MAC_CPU_COPY;

	ocelot_mact_select(ocelot, mac, vid);

	/* Issue a write command */
	ocelot_write(ocelot, cmd, ANA_TABLES_MACACCESS);

	return ocelot_mact_wait_for_completion(ocelot);
}
EXPORT_SYMBOL(ocelot_mact_learn);

int ocelot_mact_forget(struct ocelot *ocelot,
		       const unsigned char mac[ETH_ALEN], unsigned int vid)
>>>>>>> upstream/android-13
{
	ocelot_mact_select(ocelot, mac, vid);

	/* Issue a forget command */
	ocelot_write(ocelot,
		     ANA_TABLES_MACACCESS_MAC_TABLE_CMD(MACACCESS_CMD_FORGET),
		     ANA_TABLES_MACACCESS);

	return ocelot_mact_wait_for_completion(ocelot);
}
<<<<<<< HEAD
=======
EXPORT_SYMBOL(ocelot_mact_forget);
>>>>>>> upstream/android-13

static void ocelot_mact_init(struct ocelot *ocelot)
{
	/* Configure the learning mode entries attributes:
	 * - Do not copy the frame to the CPU extraction queues.
	 * - Use the vlan and mac_cpoy for dmac lookup.
	 */
	ocelot_rmw(ocelot, 0,
		   ANA_AGENCTRL_LEARN_CPU_COPY | ANA_AGENCTRL_IGNORE_DMAC_FLAGS
		   | ANA_AGENCTRL_LEARN_FWD_KILL
		   | ANA_AGENCTRL_LEARN_IGNORE_VLAN,
		   ANA_AGENCTRL);

	/* Clear the MAC table */
	ocelot_write(ocelot, MACACCESS_CMD_INIT, ANA_TABLES_MACACCESS);
}

<<<<<<< HEAD
static inline int ocelot_vlant_wait_for_completion(struct ocelot *ocelot)
{
	unsigned int val, timeout = 10;

	/* Wait for the issued vlan table command to be completed, or timeout.
	 * When the command read from ANA_TABLES_VLANACCESS is
	 * VLANACCESS_CMD_IDLE, the issued command completed successfully.
	 */
	do {
		val = ocelot_read(ocelot, ANA_TABLES_VLANACCESS);
		val &= ANA_TABLES_VLANACCESS_VLAN_TBL_CMD_M;
	} while (val != ANA_TABLES_VLANACCESS_CMD_IDLE && timeout--);

	if (!timeout)
		return -ETIMEDOUT;

	return 0;
=======
static void ocelot_vcap_enable(struct ocelot *ocelot, int port)
{
	ocelot_write_gix(ocelot, ANA_PORT_VCAP_S2_CFG_S2_ENA |
			 ANA_PORT_VCAP_S2_CFG_S2_IP6_CFG(0xa),
			 ANA_PORT_VCAP_S2_CFG, port);

	ocelot_write_gix(ocelot, ANA_PORT_VCAP_CFG_S1_ENA,
			 ANA_PORT_VCAP_CFG, port);

	ocelot_rmw_gix(ocelot, REW_PORT_CFG_ES0_EN,
		       REW_PORT_CFG_ES0_EN,
		       REW_PORT_CFG, port);
}

static inline u32 ocelot_vlant_read_vlanaccess(struct ocelot *ocelot)
{
	return ocelot_read(ocelot, ANA_TABLES_VLANACCESS);
}

static inline int ocelot_vlant_wait_for_completion(struct ocelot *ocelot)
{
	u32 val;

	return readx_poll_timeout(ocelot_vlant_read_vlanaccess,
		ocelot,
		val,
		(val & ANA_TABLES_VLANACCESS_VLAN_TBL_CMD_M) ==
		ANA_TABLES_VLANACCESS_CMD_IDLE,
		TABLE_UPDATE_SLEEP_US, TABLE_UPDATE_TIMEOUT_US);
>>>>>>> upstream/android-13
}

static int ocelot_vlant_set_mask(struct ocelot *ocelot, u16 vid, u32 mask)
{
	/* Select the VID to configure */
	ocelot_write(ocelot, ANA_TABLES_VLANTIDX_V_INDEX(vid),
		     ANA_TABLES_VLANTIDX);
	/* Set the vlan port members mask and issue a write command */
	ocelot_write(ocelot, ANA_TABLES_VLANACCESS_VLAN_PORT_MASK(mask) |
			     ANA_TABLES_VLANACCESS_CMD_WRITE,
		     ANA_TABLES_VLANACCESS);

	return ocelot_vlant_wait_for_completion(ocelot);
}

<<<<<<< HEAD
static void ocelot_vlan_mode(struct ocelot_port *port,
			     netdev_features_t features)
{
	struct ocelot *ocelot = port->ocelot;
	u8 p = port->chip_port;
	u32 val;

	/* Filtering */
	val = ocelot_read(ocelot, ANA_VLANMASK);
	if (features & NETIF_F_HW_VLAN_CTAG_FILTER)
		val |= BIT(p);
	else
		val &= ~BIT(p);
	ocelot_write(ocelot, val, ANA_VLANMASK);
}

static void ocelot_vlan_port_apply(struct ocelot *ocelot,
				   struct ocelot_port *port)
{
	u32 val;

	/* Ingress clasification (ANA_PORT_VLAN_CFG) */
	/* Default vlan to clasify for untagged frames (may be zero) */
	val = ANA_PORT_VLAN_CFG_VLAN_VID(port->pvid);
	if (port->vlan_aware)
		val |= ANA_PORT_VLAN_CFG_VLAN_AWARE_ENA |
		       ANA_PORT_VLAN_CFG_VLAN_POP_CNT(1);

	ocelot_rmw_gix(ocelot, val,
		       ANA_PORT_VLAN_CFG_VLAN_VID_M |
		       ANA_PORT_VLAN_CFG_VLAN_AWARE_ENA |
		       ANA_PORT_VLAN_CFG_VLAN_POP_CNT_M,
		       ANA_PORT_VLAN_CFG, port->chip_port);

	/* Drop frames with multicast source address */
	val = ANA_PORT_DROP_CFG_DROP_MC_SMAC_ENA;
	if (port->vlan_aware && !port->vid)
		/* If port is vlan-aware and tagged, drop untagged and priority
		 * tagged frames.
		 */
		val |= ANA_PORT_DROP_CFG_DROP_UNTAGGED_ENA |
		       ANA_PORT_DROP_CFG_DROP_PRIO_S_TAGGED_ENA |
		       ANA_PORT_DROP_CFG_DROP_PRIO_C_TAGGED_ENA;
	ocelot_write_gix(ocelot, val, ANA_PORT_DROP_CFG, port->chip_port);

	/* Egress configuration (REW_TAG_CFG): VLAN tag type to 8021Q. */
	val = REW_TAG_CFG_TAG_TPID_CFG(0);

	if (port->vlan_aware) {
		if (port->vid)
			/* Tag all frames except when VID == DEFAULT_VLAN */
			val |= REW_TAG_CFG_TAG_CFG(1);
		else
			/* Tag all frames */
			val |= REW_TAG_CFG_TAG_CFG(3);
	}
	ocelot_rmw_gix(ocelot, val,
		       REW_TAG_CFG_TAG_TPID_CFG_M |
		       REW_TAG_CFG_TAG_CFG_M,
		       REW_TAG_CFG, port->chip_port);

	/* Set default VLAN and tag type to 8021Q. */
	val = REW_PORT_VLAN_CFG_PORT_TPID(ETH_P_8021Q) |
	      REW_PORT_VLAN_CFG_PORT_VID(port->vid);
	ocelot_rmw_gix(ocelot, val,
		       REW_PORT_VLAN_CFG_PORT_TPID_M |
		       REW_PORT_VLAN_CFG_PORT_VID_M,
		       REW_PORT_VLAN_CFG, port->chip_port);
}

static int ocelot_vlan_vid_add(struct net_device *dev, u16 vid, bool pvid,
			       bool untagged)
{
	struct ocelot_port *port = netdev_priv(dev);
	struct ocelot *ocelot = port->ocelot;
	int ret;

	/* Add the port MAC address to with the right VLAN information */
	ocelot_mact_learn(ocelot, PGID_CPU, dev->dev_addr, vid,
			  ENTRYTYPE_LOCKED);

	/* Make the port a member of the VLAN */
	ocelot->vlan_mask[vid] |= BIT(port->chip_port);
	ret = ocelot_vlant_set_mask(ocelot, vid, ocelot->vlan_mask[vid]);
	if (ret)
		return ret;

	/* Default ingress vlan classification */
	if (pvid)
		port->pvid = vid;

	/* Untagged egress vlan clasification */
	if (untagged && port->vid != vid) {
		if (port->vid) {
			dev_err(ocelot->dev,
				"Port already has a native VLAN: %d\n",
				port->vid);
			return -EBUSY;
		}
		port->vid = vid;
	}

	ocelot_vlan_port_apply(ocelot, port);

	return 0;
}

static int ocelot_vlan_vid_del(struct net_device *dev, u16 vid)
{
	struct ocelot_port *port = netdev_priv(dev);
	struct ocelot *ocelot = port->ocelot;
	int ret;

	/* 8021q removes VID 0 on module unload for all interfaces
	 * with VLAN filtering feature. We need to keep it to receive
	 * untagged traffic.
	 */
	if (vid == 0)
		return 0;

	/* Del the port MAC address to with the right VLAN information */
	ocelot_mact_forget(ocelot, dev->dev_addr, vid);

	/* Stop the port from being a member of the vlan */
	ocelot->vlan_mask[vid] &= ~BIT(port->chip_port);
	ret = ocelot_vlant_set_mask(ocelot, vid, ocelot->vlan_mask[vid]);
	if (ret)
		return ret;

	/* Ingress */
	if (port->pvid == vid)
		port->pvid = 0;

	/* Egress */
	if (port->vid == vid)
		port->vid = 0;

	ocelot_vlan_port_apply(ocelot, port);

	return 0;
}

static void ocelot_vlan_init(struct ocelot *ocelot)
{
=======
static void ocelot_port_set_native_vlan(struct ocelot *ocelot, int port,
					struct ocelot_vlan native_vlan)
{
	struct ocelot_port *ocelot_port = ocelot->ports[port];
	u32 val = 0;

	ocelot_port->native_vlan = native_vlan;

	ocelot_rmw_gix(ocelot, REW_PORT_VLAN_CFG_PORT_VID(native_vlan.vid),
		       REW_PORT_VLAN_CFG_PORT_VID_M,
		       REW_PORT_VLAN_CFG, port);

	if (ocelot_port->vlan_aware) {
		if (native_vlan.valid)
			/* Tag all frames except when VID == DEFAULT_VLAN */
			val = REW_TAG_CFG_TAG_CFG(1);
		else
			/* Tag all frames */
			val = REW_TAG_CFG_TAG_CFG(3);
	} else {
		/* Port tagging disabled. */
		val = REW_TAG_CFG_TAG_CFG(0);
	}
	ocelot_rmw_gix(ocelot, val,
		       REW_TAG_CFG_TAG_CFG_M,
		       REW_TAG_CFG, port);
}

/* Default vlan to clasify for untagged frames (may be zero) */
static void ocelot_port_set_pvid(struct ocelot *ocelot, int port,
				 struct ocelot_vlan pvid_vlan)
{
	struct ocelot_port *ocelot_port = ocelot->ports[port];
	u32 val = 0;

	ocelot_port->pvid_vlan = pvid_vlan;

	if (!ocelot_port->vlan_aware)
		pvid_vlan.vid = 0;

	ocelot_rmw_gix(ocelot,
		       ANA_PORT_VLAN_CFG_VLAN_VID(pvid_vlan.vid),
		       ANA_PORT_VLAN_CFG_VLAN_VID_M,
		       ANA_PORT_VLAN_CFG, port);

	/* If there's no pvid, we should drop not only untagged traffic (which
	 * happens automatically), but also 802.1p traffic which gets
	 * classified to VLAN 0, but that is always in our RX filter, so it
	 * would get accepted were it not for this setting.
	 */
	if (!pvid_vlan.valid && ocelot_port->vlan_aware)
		val = ANA_PORT_DROP_CFG_DROP_PRIO_S_TAGGED_ENA |
		      ANA_PORT_DROP_CFG_DROP_PRIO_C_TAGGED_ENA;

	ocelot_rmw_gix(ocelot, val,
		       ANA_PORT_DROP_CFG_DROP_PRIO_S_TAGGED_ENA |
		       ANA_PORT_DROP_CFG_DROP_PRIO_C_TAGGED_ENA,
		       ANA_PORT_DROP_CFG, port);
}

static int ocelot_vlan_member_set(struct ocelot *ocelot, u32 vlan_mask, u16 vid)
{
	int err;

	err = ocelot_vlant_set_mask(ocelot, vid, vlan_mask);
	if (err)
		return err;

	ocelot->vlan_mask[vid] = vlan_mask;

	return 0;
}

static int ocelot_vlan_member_add(struct ocelot *ocelot, int port, u16 vid)
{
	return ocelot_vlan_member_set(ocelot,
				      ocelot->vlan_mask[vid] | BIT(port),
				      vid);
}

static int ocelot_vlan_member_del(struct ocelot *ocelot, int port, u16 vid)
{
	return ocelot_vlan_member_set(ocelot,
				      ocelot->vlan_mask[vid] & ~BIT(port),
				      vid);
}

int ocelot_port_vlan_filtering(struct ocelot *ocelot, int port,
			       bool vlan_aware, struct netlink_ext_ack *extack)
{
	struct ocelot_vcap_block *block = &ocelot->block[VCAP_IS1];
	struct ocelot_port *ocelot_port = ocelot->ports[port];
	struct ocelot_vcap_filter *filter;
	u32 val;

	list_for_each_entry(filter, &block->rules, list) {
		if (filter->ingress_port_mask & BIT(port) &&
		    filter->action.vid_replace_ena) {
			NL_SET_ERR_MSG_MOD(extack,
					   "Cannot change VLAN state with vlan modify rules active");
			return -EBUSY;
		}
	}

	ocelot_port->vlan_aware = vlan_aware;

	if (vlan_aware)
		val = ANA_PORT_VLAN_CFG_VLAN_AWARE_ENA |
		      ANA_PORT_VLAN_CFG_VLAN_POP_CNT(1);
	else
		val = 0;
	ocelot_rmw_gix(ocelot, val,
		       ANA_PORT_VLAN_CFG_VLAN_AWARE_ENA |
		       ANA_PORT_VLAN_CFG_VLAN_POP_CNT_M,
		       ANA_PORT_VLAN_CFG, port);

	ocelot_port_set_pvid(ocelot, port, ocelot_port->pvid_vlan);
	ocelot_port_set_native_vlan(ocelot, port, ocelot_port->native_vlan);

	return 0;
}
EXPORT_SYMBOL(ocelot_port_vlan_filtering);

int ocelot_vlan_prepare(struct ocelot *ocelot, int port, u16 vid, bool pvid,
			bool untagged, struct netlink_ext_ack *extack)
{
	struct ocelot_port *ocelot_port = ocelot->ports[port];

	/* Deny changing the native VLAN, but always permit deleting it */
	if (untagged && ocelot_port->native_vlan.vid != vid &&
	    ocelot_port->native_vlan.valid) {
		NL_SET_ERR_MSG_MOD(extack,
				   "Port already has a native VLAN");
		return -EBUSY;
	}

	return 0;
}
EXPORT_SYMBOL(ocelot_vlan_prepare);

int ocelot_vlan_add(struct ocelot *ocelot, int port, u16 vid, bool pvid,
		    bool untagged)
{
	int err;

	err = ocelot_vlan_member_add(ocelot, port, vid);
	if (err)
		return err;

	/* Default ingress vlan classification */
	if (pvid) {
		struct ocelot_vlan pvid_vlan;

		pvid_vlan.vid = vid;
		pvid_vlan.valid = true;
		ocelot_port_set_pvid(ocelot, port, pvid_vlan);
	}

	/* Untagged egress vlan clasification */
	if (untagged) {
		struct ocelot_vlan native_vlan;

		native_vlan.vid = vid;
		native_vlan.valid = true;
		ocelot_port_set_native_vlan(ocelot, port, native_vlan);
	}

	return 0;
}
EXPORT_SYMBOL(ocelot_vlan_add);

int ocelot_vlan_del(struct ocelot *ocelot, int port, u16 vid)
{
	struct ocelot_port *ocelot_port = ocelot->ports[port];
	int err;

	err = ocelot_vlan_member_del(ocelot, port, vid);
	if (err)
		return err;

	/* Ingress */
	if (ocelot_port->pvid_vlan.vid == vid) {
		struct ocelot_vlan pvid_vlan = {0};

		ocelot_port_set_pvid(ocelot, port, pvid_vlan);
	}

	/* Egress */
	if (ocelot_port->native_vlan.vid == vid) {
		struct ocelot_vlan native_vlan = {0};

		ocelot_port_set_native_vlan(ocelot, port, native_vlan);
	}

	return 0;
}
EXPORT_SYMBOL(ocelot_vlan_del);

static void ocelot_vlan_init(struct ocelot *ocelot)
{
	unsigned long all_ports = GENMASK(ocelot->num_phys_ports - 1, 0);
>>>>>>> upstream/android-13
	u16 port, vid;

	/* Clear VLAN table, by default all ports are members of all VLANs */
	ocelot_write(ocelot, ANA_TABLES_VLANACCESS_CMD_INIT,
		     ANA_TABLES_VLANACCESS);
	ocelot_vlant_wait_for_completion(ocelot);

	/* Configure the port VLAN memberships */
<<<<<<< HEAD
	for (vid = 1; vid < VLAN_N_VID; vid++) {
		ocelot->vlan_mask[vid] = 0;
		ocelot_vlant_set_mask(ocelot, vid, ocelot->vlan_mask[vid]);
	}
=======
	for (vid = 1; vid < VLAN_N_VID; vid++)
		ocelot_vlan_member_set(ocelot, 0, vid);
>>>>>>> upstream/android-13

	/* Because VLAN filtering is enabled, we need VID 0 to get untagged
	 * traffic.  It is added automatically if 8021q module is loaded, but
	 * we can't rely on it since module may be not loaded.
	 */
<<<<<<< HEAD
	ocelot->vlan_mask[0] = GENMASK(ocelot->num_phys_ports - 1, 0);
	ocelot_vlant_set_mask(ocelot, 0, ocelot->vlan_mask[0]);

	/* Configure the CPU port to be VLAN aware */
	ocelot_write_gix(ocelot, ANA_PORT_VLAN_CFG_VLAN_VID(0) |
				 ANA_PORT_VLAN_CFG_VLAN_AWARE_ENA |
				 ANA_PORT_VLAN_CFG_VLAN_POP_CNT(1),
			 ANA_PORT_VLAN_CFG, ocelot->num_phys_ports);
=======
	ocelot_vlan_member_set(ocelot, all_ports, 0);
>>>>>>> upstream/android-13

	/* Set vlan ingress filter mask to all ports but the CPU port by
	 * default.
	 */
<<<<<<< HEAD
	ocelot_write(ocelot, GENMASK(9, 0), ANA_VLANMASK);
=======
	ocelot_write(ocelot, all_ports, ANA_VLANMASK);
>>>>>>> upstream/android-13

	for (port = 0; port < ocelot->num_phys_ports; port++) {
		ocelot_write_gix(ocelot, 0, REW_PORT_VLAN_CFG, port);
		ocelot_write_gix(ocelot, 0, REW_TAG_CFG, port);
	}
}

<<<<<<< HEAD
/* Watermark encode
 * Bit 8:   Unit; 0:1, 1:16
 * Bit 7-0: Value to be multiplied with unit
 */
static u16 ocelot_wm_enc(u16 value)
{
	if (value >= BIT(8))
		return BIT(8) | (value / 16);

	return value;
}

static void ocelot_port_adjust_link(struct net_device *dev)
{
	struct ocelot_port *port = netdev_priv(dev);
	struct ocelot *ocelot = port->ocelot;
	u8 p = port->chip_port;
	int speed, atop_wm, mode = 0;

	switch (dev->phydev->speed) {
	case SPEED_10:
		speed = OCELOT_SPEED_10;
		break;
	case SPEED_100:
		speed = OCELOT_SPEED_100;
		break;
	case SPEED_1000:
		speed = OCELOT_SPEED_1000;
		mode = DEV_MAC_MODE_CFG_GIGA_MODE_ENA;
		break;
	case SPEED_2500:
		speed = OCELOT_SPEED_2500;
		mode = DEV_MAC_MODE_CFG_GIGA_MODE_ENA;
		break;
	default:
		netdev_err(dev, "Unsupported PHY speed: %d\n",
			   dev->phydev->speed);
		return;
	}

	phy_print_status(dev->phydev);

	if (!dev->phydev->link)
		return;

	/* Only full duplex supported for now */
	ocelot_port_writel(port, DEV_MAC_MODE_CFG_FDX_ENA |
			   mode, DEV_MAC_MODE_CFG);

	/* Set MAC IFG Gaps
	 * FDX: TX_IFG = 5, RX_IFG1 = RX_IFG2 = 0
	 * !FDX: TX_IFG = 5, RX_IFG1 = RX_IFG2 = 5
	 */
	ocelot_port_writel(port, DEV_MAC_IFG_CFG_TX_IFG(5), DEV_MAC_IFG_CFG);

	/* Load seed (0) and set MAC HDX late collision  */
	ocelot_port_writel(port, DEV_MAC_HDX_CFG_LATE_COL_POS(67) |
			   DEV_MAC_HDX_CFG_SEED_LOAD,
			   DEV_MAC_HDX_CFG);
	mdelay(1);
	ocelot_port_writel(port, DEV_MAC_HDX_CFG_LATE_COL_POS(67),
			   DEV_MAC_HDX_CFG);

	/* Disable HDX fast control */
	ocelot_port_writel(port, DEV_PORT_MISC_HDX_FAST_DIS, DEV_PORT_MISC);

	/* SGMII only for now */
	ocelot_port_writel(port, PCS1G_MODE_CFG_SGMII_MODE_ENA, PCS1G_MODE_CFG);
	ocelot_port_writel(port, PCS1G_SD_CFG_SD_SEL, PCS1G_SD_CFG);

	/* Enable PCS */
	ocelot_port_writel(port, PCS1G_CFG_PCS_ENA, PCS1G_CFG);

	/* No aneg on SGMII */
	ocelot_port_writel(port, 0, PCS1G_ANEG_CFG);

	/* No loopback */
	ocelot_port_writel(port, 0, PCS1G_LB_CFG);

	/* Set Max Length and maximum tags allowed */
	ocelot_port_writel(port, VLAN_ETH_FRAME_LEN, DEV_MAC_MAXLEN_CFG);
	ocelot_port_writel(port, DEV_MAC_TAGS_CFG_TAG_ID(ETH_P_8021AD) |
			   DEV_MAC_TAGS_CFG_VLAN_AWR_ENA |
			   DEV_MAC_TAGS_CFG_VLAN_LEN_AWR_ENA,
			   DEV_MAC_TAGS_CFG);

	/* Enable MAC module */
	ocelot_port_writel(port, DEV_MAC_ENA_CFG_RX_ENA |
			   DEV_MAC_ENA_CFG_TX_ENA, DEV_MAC_ENA_CFG);

	/* Take MAC, Port, Phy (intern) and PCS (SGMII/Serdes) clock out of
	 * reset */
	ocelot_port_writel(port, DEV_CLOCK_CFG_LINK_SPEED(speed),
			   DEV_CLOCK_CFG);

	/* Set SMAC of Pause frame (00:00:00:00:00:00) */
	ocelot_port_writel(port, 0, DEV_MAC_FC_MAC_HIGH_CFG);
	ocelot_port_writel(port, 0, DEV_MAC_FC_MAC_LOW_CFG);

	/* No PFC */
	ocelot_write_gix(ocelot, ANA_PFC_PFC_CFG_FC_LINK_SPEED(speed),
			 ANA_PFC_PFC_CFG, p);

	/* Set Pause WM hysteresis
	 * 152 = 6 * VLAN_ETH_FRAME_LEN / OCELOT_BUFFER_CELL_SZ
	 * 101 = 4 * VLAN_ETH_FRAME_LEN / OCELOT_BUFFER_CELL_SZ
	 */
	ocelot_write_rix(ocelot, SYS_PAUSE_CFG_PAUSE_ENA |
			 SYS_PAUSE_CFG_PAUSE_STOP(101) |
			 SYS_PAUSE_CFG_PAUSE_START(152), SYS_PAUSE_CFG, p);

	/* Core: Enable port for frame transfer */
	ocelot_write_rix(ocelot, QSYS_SWITCH_PORT_MODE_INGRESS_DROP_MODE |
			 QSYS_SWITCH_PORT_MODE_SCH_NEXT_CFG(1) |
			 QSYS_SWITCH_PORT_MODE_PORT_ENA,
			 QSYS_SWITCH_PORT_MODE, p);

	/* Flow control */
	ocelot_write_rix(ocelot, SYS_MAC_FC_CFG_PAUSE_VAL_CFG(0xffff) |
			 SYS_MAC_FC_CFG_RX_FC_ENA | SYS_MAC_FC_CFG_TX_FC_ENA |
			 SYS_MAC_FC_CFG_ZERO_PAUSE_ENA |
			 SYS_MAC_FC_CFG_FC_LATENCY_CFG(0x7) |
			 SYS_MAC_FC_CFG_FC_LINK_SPEED(speed),
			 SYS_MAC_FC_CFG, p);
	ocelot_write_rix(ocelot, 0, ANA_POL_FLOWC, p);

	/* Tail dropping watermark */
	atop_wm = (ocelot->shared_queue_sz - 9 * VLAN_ETH_FRAME_LEN) / OCELOT_BUFFER_CELL_SZ;
	ocelot_write_rix(ocelot, ocelot_wm_enc(9 * VLAN_ETH_FRAME_LEN),
			 SYS_ATOP, p);
	ocelot_write(ocelot, ocelot_wm_enc(atop_wm), SYS_ATOP_TOT_CFG);
}

static int ocelot_port_open(struct net_device *dev)
{
	struct ocelot_port *port = netdev_priv(dev);
	struct ocelot *ocelot = port->ocelot;
	int err;

	/* Enable receiving frames on the port, and activate auto-learning of
	 * MAC addresses.
	 */
	ocelot_write_gix(ocelot, ANA_PORT_PORT_CFG_LEARNAUTO |
			 ANA_PORT_PORT_CFG_RECV_ENA |
			 ANA_PORT_PORT_CFG_PORTID_VAL(port->chip_port),
			 ANA_PORT_PORT_CFG, port->chip_port);

	err = phy_connect_direct(dev, port->phy, &ocelot_port_adjust_link,
				 PHY_INTERFACE_MODE_NA);
	if (err) {
		netdev_err(dev, "Could not attach to PHY\n");
		return err;
	}

	dev->phydev = port->phy;

	phy_attached_info(port->phy);
	phy_start(port->phy);
	return 0;
}

static int ocelot_port_stop(struct net_device *dev)
{
	struct ocelot_port *port = netdev_priv(dev);

	phy_disconnect(port->phy);

	dev->phydev = NULL;

	ocelot_port_writel(port, 0, DEV_MAC_ENA_CFG);
	ocelot_rmw_rix(port->ocelot, 0, QSYS_SWITCH_PORT_MODE_PORT_ENA,
			 QSYS_SWITCH_PORT_MODE, port->chip_port);
	return 0;
}

/* Generate the IFH for frame injection
 *
 * The IFH is a 128bit-value
 * bit 127: bypass the analyzer processing
 * bit 56-67: destination mask
 * bit 28-29: pop_cnt: 3 disables all rewriting of the frame
 * bit 20-27: cpu extraction queue mask
 * bit 16: tag type 0: C-tag, 1: S-tag
 * bit 0-11: VID
 */
static int ocelot_gen_ifh(u32 *ifh, struct frame_info *info)
{
	ifh[0] = IFH_INJ_BYPASS;
	ifh[1] = (0xf00 & info->port) >> 8;
	ifh[2] = (0xff & info->port) << 24;
	ifh[3] = (info->tag_type << 16) | info->vid;
=======
static u32 ocelot_read_eq_avail(struct ocelot *ocelot, int port)
{
	return ocelot_read_rix(ocelot, QSYS_SW_STATUS, port);
}

static int ocelot_port_flush(struct ocelot *ocelot, int port)
{
	unsigned int pause_ena;
	int err, val;

	/* Disable dequeuing from the egress queues */
	ocelot_rmw_rix(ocelot, QSYS_PORT_MODE_DEQUEUE_DIS,
		       QSYS_PORT_MODE_DEQUEUE_DIS,
		       QSYS_PORT_MODE, port);

	/* Disable flow control */
	ocelot_fields_read(ocelot, port, SYS_PAUSE_CFG_PAUSE_ENA, &pause_ena);
	ocelot_fields_write(ocelot, port, SYS_PAUSE_CFG_PAUSE_ENA, 0);

	/* Disable priority flow control */
	ocelot_fields_write(ocelot, port,
			    QSYS_SWITCH_PORT_MODE_TX_PFC_ENA, 0);

	/* Wait at least the time it takes to receive a frame of maximum length
	 * at the port.
	 * Worst-case delays for 10 kilobyte jumbo frames are:
	 * 8 ms on a 10M port
	 * 800 μs on a 100M port
	 * 80 μs on a 1G port
	 * 32 μs on a 2.5G port
	 */
	usleep_range(8000, 10000);

	/* Disable half duplex backpressure. */
	ocelot_rmw_rix(ocelot, 0, SYS_FRONT_PORT_MODE_HDX_MODE,
		       SYS_FRONT_PORT_MODE, port);

	/* Flush the queues associated with the port. */
	ocelot_rmw_gix(ocelot, REW_PORT_CFG_FLUSH_ENA, REW_PORT_CFG_FLUSH_ENA,
		       REW_PORT_CFG, port);

	/* Enable dequeuing from the egress queues. */
	ocelot_rmw_rix(ocelot, 0, QSYS_PORT_MODE_DEQUEUE_DIS, QSYS_PORT_MODE,
		       port);

	/* Wait until flushing is complete. */
	err = read_poll_timeout(ocelot_read_eq_avail, val, !val,
				100, 2000000, false, ocelot, port);

	/* Clear flushing again. */
	ocelot_rmw_gix(ocelot, 0, REW_PORT_CFG_FLUSH_ENA, REW_PORT_CFG, port);

	/* Re-enable flow control */
	ocelot_fields_write(ocelot, port, SYS_PAUSE_CFG_PAUSE_ENA, pause_ena);

	return err;
}

void ocelot_phylink_mac_link_down(struct ocelot *ocelot, int port,
				  unsigned int link_an_mode,
				  phy_interface_t interface,
				  unsigned long quirks)
{
	struct ocelot_port *ocelot_port = ocelot->ports[port];
	int err;

	ocelot_port_rmwl(ocelot_port, 0, DEV_MAC_ENA_CFG_RX_ENA,
			 DEV_MAC_ENA_CFG);

	ocelot_fields_write(ocelot, port, QSYS_SWITCH_PORT_MODE_PORT_ENA, 0);

	err = ocelot_port_flush(ocelot, port);
	if (err)
		dev_err(ocelot->dev, "failed to flush port %d: %d\n",
			port, err);

	/* Put the port in reset. */
	if (interface != PHY_INTERFACE_MODE_QSGMII ||
	    !(quirks & OCELOT_QUIRK_QSGMII_PORTS_MUST_BE_UP))
		ocelot_port_rmwl(ocelot_port,
				 DEV_CLOCK_CFG_MAC_TX_RST |
				 DEV_CLOCK_CFG_MAC_RX_RST,
				 DEV_CLOCK_CFG_MAC_TX_RST |
				 DEV_CLOCK_CFG_MAC_RX_RST,
				 DEV_CLOCK_CFG);
}
EXPORT_SYMBOL_GPL(ocelot_phylink_mac_link_down);

void ocelot_phylink_mac_link_up(struct ocelot *ocelot, int port,
				struct phy_device *phydev,
				unsigned int link_an_mode,
				phy_interface_t interface,
				int speed, int duplex,
				bool tx_pause, bool rx_pause,
				unsigned long quirks)
{
	struct ocelot_port *ocelot_port = ocelot->ports[port];
	int mac_speed, mode = 0;
	u32 mac_fc_cfg;

	/* The MAC might be integrated in systems where the MAC speed is fixed
	 * and it's the PCS who is performing the rate adaptation, so we have
	 * to write "1000Mbps" into the LINK_SPEED field of DEV_CLOCK_CFG
	 * (which is also its default value).
	 */
	if ((quirks & OCELOT_QUIRK_PCS_PERFORMS_RATE_ADAPTATION) ||
	    speed == SPEED_1000) {
		mac_speed = OCELOT_SPEED_1000;
		mode = DEV_MAC_MODE_CFG_GIGA_MODE_ENA;
	} else if (speed == SPEED_2500) {
		mac_speed = OCELOT_SPEED_2500;
		mode = DEV_MAC_MODE_CFG_GIGA_MODE_ENA;
	} else if (speed == SPEED_100) {
		mac_speed = OCELOT_SPEED_100;
	} else {
		mac_speed = OCELOT_SPEED_10;
	}

	if (duplex == DUPLEX_FULL)
		mode |= DEV_MAC_MODE_CFG_FDX_ENA;

	ocelot_port_writel(ocelot_port, mode, DEV_MAC_MODE_CFG);

	/* Take port out of reset by clearing the MAC_TX_RST, MAC_RX_RST and
	 * PORT_RST bits in DEV_CLOCK_CFG.
	 */
	ocelot_port_writel(ocelot_port, DEV_CLOCK_CFG_LINK_SPEED(mac_speed),
			   DEV_CLOCK_CFG);

	switch (speed) {
	case SPEED_10:
		mac_fc_cfg = SYS_MAC_FC_CFG_FC_LINK_SPEED(OCELOT_SPEED_10);
		break;
	case SPEED_100:
		mac_fc_cfg = SYS_MAC_FC_CFG_FC_LINK_SPEED(OCELOT_SPEED_100);
		break;
	case SPEED_1000:
	case SPEED_2500:
		mac_fc_cfg = SYS_MAC_FC_CFG_FC_LINK_SPEED(OCELOT_SPEED_1000);
		break;
	default:
		dev_err(ocelot->dev, "Unsupported speed on port %d: %d\n",
			port, speed);
		return;
	}

	/* Handle RX pause in all cases, with 2500base-X this is used for rate
	 * adaptation.
	 */
	mac_fc_cfg |= SYS_MAC_FC_CFG_RX_FC_ENA;

	if (tx_pause)
		mac_fc_cfg |= SYS_MAC_FC_CFG_TX_FC_ENA |
			      SYS_MAC_FC_CFG_PAUSE_VAL_CFG(0xffff) |
			      SYS_MAC_FC_CFG_FC_LATENCY_CFG(0x7) |
			      SYS_MAC_FC_CFG_ZERO_PAUSE_ENA;

	/* Flow control. Link speed is only used here to evaluate the time
	 * specification in incoming pause frames.
	 */
	ocelot_write_rix(ocelot, mac_fc_cfg, SYS_MAC_FC_CFG, port);

	ocelot_write_rix(ocelot, 0, ANA_POL_FLOWC, port);

	/* Don't attempt to send PAUSE frames on the NPI port, it's broken */
	if (port != ocelot->npi)
		ocelot_fields_write(ocelot, port, SYS_PAUSE_CFG_PAUSE_ENA,
				    tx_pause);

	/* Undo the effects of ocelot_phylink_mac_link_down:
	 * enable MAC module
	 */
	ocelot_port_writel(ocelot_port, DEV_MAC_ENA_CFG_RX_ENA |
			   DEV_MAC_ENA_CFG_TX_ENA, DEV_MAC_ENA_CFG);

	/* Core: Enable port for frame transfer */
	ocelot_fields_write(ocelot, port,
			    QSYS_SWITCH_PORT_MODE_PORT_ENA, 1);
}
EXPORT_SYMBOL_GPL(ocelot_phylink_mac_link_up);

static int ocelot_port_add_txtstamp_skb(struct ocelot *ocelot, int port,
					struct sk_buff *clone)
{
	struct ocelot_port *ocelot_port = ocelot->ports[port];
	unsigned long flags;

	spin_lock_irqsave(&ocelot->ts_id_lock, flags);

	if (ocelot_port->ptp_skbs_in_flight == OCELOT_MAX_PTP_ID ||
	    ocelot->ptp_skbs_in_flight == OCELOT_PTP_FIFO_SIZE) {
		spin_unlock_irqrestore(&ocelot->ts_id_lock, flags);
		return -EBUSY;
	}

	skb_shinfo(clone)->tx_flags |= SKBTX_IN_PROGRESS;
	/* Store timestamp ID in OCELOT_SKB_CB(clone)->ts_id */
	OCELOT_SKB_CB(clone)->ts_id = ocelot_port->ts_id;

	ocelot_port->ts_id++;
	if (ocelot_port->ts_id == OCELOT_MAX_PTP_ID)
		ocelot_port->ts_id = 0;

	ocelot_port->ptp_skbs_in_flight++;
	ocelot->ptp_skbs_in_flight++;

	skb_queue_tail(&ocelot_port->tx_skbs, clone);

	spin_unlock_irqrestore(&ocelot->ts_id_lock, flags);
>>>>>>> upstream/android-13

	return 0;
}

<<<<<<< HEAD
static int ocelot_port_xmit(struct sk_buff *skb, struct net_device *dev)
{
	struct ocelot_port *port = netdev_priv(dev);
	struct ocelot *ocelot = port->ocelot;
	u32 val, ifh[IFH_LEN];
	struct frame_info info = {};
	u8 grp = 0; /* Send everything on CPU group 0 */
	unsigned int i, count, last;

	val = ocelot_read(ocelot, QS_INJ_STATUS);
	if (!(val & QS_INJ_STATUS_FIFO_RDY(BIT(grp))) ||
	    (val & QS_INJ_STATUS_WMARK_REACHED(BIT(grp))))
		return NETDEV_TX_BUSY;

	ocelot_write_rix(ocelot, QS_INJ_CTRL_GAP_SIZE(1) |
			 QS_INJ_CTRL_SOF, QS_INJ_CTRL, grp);

	info.port = BIT(port->chip_port);
	info.tag_type = IFH_TAG_TYPE_C;
	info.vid = skb_vlan_tag_get(skb);
	ocelot_gen_ifh(ifh, &info);

	for (i = 0; i < IFH_LEN; i++)
		ocelot_write_rix(ocelot, (__force u32)cpu_to_be32(ifh[i]),
				 QS_INJ_WR, grp);

	count = (skb->len + 3) / 4;
	last = skb->len % 4;
	for (i = 0; i < count; i++) {
		ocelot_write_rix(ocelot, ((u32 *)skb->data)[i], QS_INJ_WR, grp);
	}
=======
static bool ocelot_ptp_is_onestep_sync(struct sk_buff *skb,
				       unsigned int ptp_class)
{
	struct ptp_header *hdr;
	u8 msgtype, twostep;

	hdr = ptp_parse_header(skb, ptp_class);
	if (!hdr)
		return false;

	msgtype = ptp_get_msgtype(hdr, ptp_class);
	twostep = hdr->flag_field[0] & 0x2;

	if (msgtype == PTP_MSGTYPE_SYNC && twostep == 0)
		return true;

	return false;
}

int ocelot_port_txtstamp_request(struct ocelot *ocelot, int port,
				 struct sk_buff *skb,
				 struct sk_buff **clone)
{
	struct ocelot_port *ocelot_port = ocelot->ports[port];
	u8 ptp_cmd = ocelot_port->ptp_cmd;
	unsigned int ptp_class;
	int err;

	/* Don't do anything if PTP timestamping not enabled */
	if (!ptp_cmd)
		return 0;

	ptp_class = ptp_classify_raw(skb);
	if (ptp_class == PTP_CLASS_NONE)
		return -EINVAL;

	/* Store ptp_cmd in OCELOT_SKB_CB(skb)->ptp_cmd */
	if (ptp_cmd == IFH_REW_OP_ORIGIN_PTP) {
		if (ocelot_ptp_is_onestep_sync(skb, ptp_class)) {
			OCELOT_SKB_CB(skb)->ptp_cmd = ptp_cmd;
			return 0;
		}

		/* Fall back to two-step timestamping */
		ptp_cmd = IFH_REW_OP_TWO_STEP_PTP;
	}

	if (ptp_cmd == IFH_REW_OP_TWO_STEP_PTP) {
		*clone = skb_clone_sk(skb);
		if (!(*clone))
			return -ENOMEM;

		err = ocelot_port_add_txtstamp_skb(ocelot, port, *clone);
		if (err)
			return err;

		OCELOT_SKB_CB(skb)->ptp_cmd = ptp_cmd;
		OCELOT_SKB_CB(*clone)->ptp_class = ptp_class;
	}

	return 0;
}
EXPORT_SYMBOL(ocelot_port_txtstamp_request);

static void ocelot_get_hwtimestamp(struct ocelot *ocelot,
				   struct timespec64 *ts)
{
	unsigned long flags;
	u32 val;

	spin_lock_irqsave(&ocelot->ptp_clock_lock, flags);

	/* Read current PTP time to get seconds */
	val = ocelot_read_rix(ocelot, PTP_PIN_CFG, TOD_ACC_PIN);

	val &= ~(PTP_PIN_CFG_SYNC | PTP_PIN_CFG_ACTION_MASK | PTP_PIN_CFG_DOM);
	val |= PTP_PIN_CFG_ACTION(PTP_PIN_ACTION_SAVE);
	ocelot_write_rix(ocelot, val, PTP_PIN_CFG, TOD_ACC_PIN);
	ts->tv_sec = ocelot_read_rix(ocelot, PTP_PIN_TOD_SEC_LSB, TOD_ACC_PIN);

	/* Read packet HW timestamp from FIFO */
	val = ocelot_read(ocelot, SYS_PTP_TXSTAMP);
	ts->tv_nsec = SYS_PTP_TXSTAMP_PTP_TXSTAMP(val);

	/* Sec has incremented since the ts was registered */
	if ((ts->tv_sec & 0x1) != !!(val & SYS_PTP_TXSTAMP_PTP_TXSTAMP_SEC))
		ts->tv_sec--;

	spin_unlock_irqrestore(&ocelot->ptp_clock_lock, flags);
}

static bool ocelot_validate_ptp_skb(struct sk_buff *clone, u16 seqid)
{
	struct ptp_header *hdr;

	hdr = ptp_parse_header(clone, OCELOT_SKB_CB(clone)->ptp_class);
	if (WARN_ON(!hdr))
		return false;

	return seqid == ntohs(hdr->sequence_id);
}

void ocelot_get_txtstamp(struct ocelot *ocelot)
{
	int budget = OCELOT_PTP_QUEUE_SZ;

	while (budget--) {
		struct sk_buff *skb, *skb_tmp, *skb_match = NULL;
		struct skb_shared_hwtstamps shhwtstamps;
		u32 val, id, seqid, txport;
		struct ocelot_port *port;
		struct timespec64 ts;
		unsigned long flags;

		val = ocelot_read(ocelot, SYS_PTP_STATUS);

		/* Check if a timestamp can be retrieved */
		if (!(val & SYS_PTP_STATUS_PTP_MESS_VLD))
			break;

		WARN_ON(val & SYS_PTP_STATUS_PTP_OVFL);

		/* Retrieve the ts ID and Tx port */
		id = SYS_PTP_STATUS_PTP_MESS_ID_X(val);
		txport = SYS_PTP_STATUS_PTP_MESS_TXPORT_X(val);
		seqid = SYS_PTP_STATUS_PTP_MESS_SEQ_ID(val);

		port = ocelot->ports[txport];

		spin_lock(&ocelot->ts_id_lock);
		port->ptp_skbs_in_flight--;
		ocelot->ptp_skbs_in_flight--;
		spin_unlock(&ocelot->ts_id_lock);

		/* Retrieve its associated skb */
try_again:
		spin_lock_irqsave(&port->tx_skbs.lock, flags);

		skb_queue_walk_safe(&port->tx_skbs, skb, skb_tmp) {
			if (OCELOT_SKB_CB(skb)->ts_id != id)
				continue;
			__skb_unlink(skb, &port->tx_skbs);
			skb_match = skb;
			break;
		}

		spin_unlock_irqrestore(&port->tx_skbs.lock, flags);

		if (WARN_ON(!skb_match))
			continue;

		if (!ocelot_validate_ptp_skb(skb_match, seqid)) {
			dev_err_ratelimited(ocelot->dev,
					    "port %d received stale TX timestamp for seqid %d, discarding\n",
					    txport, seqid);
			dev_kfree_skb_any(skb);
			goto try_again;
		}

		/* Get the h/w timestamp */
		ocelot_get_hwtimestamp(ocelot, &ts);

		/* Set the timestamp into the skb */
		memset(&shhwtstamps, 0, sizeof(shhwtstamps));
		shhwtstamps.hwtstamp = ktime_set(ts.tv_sec, ts.tv_nsec);
		skb_complete_tx_timestamp(skb_match, &shhwtstamps);

		/* Next ts */
		ocelot_write(ocelot, SYS_PTP_NXT_PTP_NXT, SYS_PTP_NXT);
	}
}
EXPORT_SYMBOL(ocelot_get_txtstamp);

static int ocelot_rx_frame_word(struct ocelot *ocelot, u8 grp, bool ifh,
				u32 *rval)
{
	u32 bytes_valid, val;

	val = ocelot_read_rix(ocelot, QS_XTR_RD, grp);
	if (val == XTR_NOT_READY) {
		if (ifh)
			return -EIO;

		do {
			val = ocelot_read_rix(ocelot, QS_XTR_RD, grp);
		} while (val == XTR_NOT_READY);
	}

	switch (val) {
	case XTR_ABORT:
		return -EIO;
	case XTR_EOF_0:
	case XTR_EOF_1:
	case XTR_EOF_2:
	case XTR_EOF_3:
	case XTR_PRUNED:
		bytes_valid = XTR_VALID_BYTES(val);
		val = ocelot_read_rix(ocelot, QS_XTR_RD, grp);
		if (val == XTR_ESCAPE)
			*rval = ocelot_read_rix(ocelot, QS_XTR_RD, grp);
		else
			*rval = val;

		return bytes_valid;
	case XTR_ESCAPE:
		*rval = ocelot_read_rix(ocelot, QS_XTR_RD, grp);

		return 4;
	default:
		*rval = val;

		return 4;
	}
}

static int ocelot_xtr_poll_xfh(struct ocelot *ocelot, int grp, u32 *xfh)
{
	int i, err = 0;

	for (i = 0; i < OCELOT_TAG_LEN / 4; i++) {
		err = ocelot_rx_frame_word(ocelot, grp, true, &xfh[i]);
		if (err != 4)
			return (err < 0) ? err : -EIO;
	}

	return 0;
}

int ocelot_xtr_poll_frame(struct ocelot *ocelot, int grp, struct sk_buff **nskb)
{
	struct skb_shared_hwtstamps *shhwtstamps;
	u64 tod_in_ns, full_ts_in_ns;
	u64 timestamp, src_port, len;
	u32 xfh[OCELOT_TAG_LEN / 4];
	struct net_device *dev;
	struct timespec64 ts;
	struct sk_buff *skb;
	int sz, buf_len;
	u32 val, *buf;
	int err;

	err = ocelot_xtr_poll_xfh(ocelot, grp, xfh);
	if (err)
		return err;

	ocelot_xfh_get_src_port(xfh, &src_port);
	ocelot_xfh_get_len(xfh, &len);
	ocelot_xfh_get_rew_val(xfh, &timestamp);

	if (WARN_ON(src_port >= ocelot->num_phys_ports))
		return -EINVAL;

	dev = ocelot->ops->port_to_netdev(ocelot, src_port);
	if (!dev)
		return -EINVAL;

	skb = netdev_alloc_skb(dev, len);
	if (unlikely(!skb)) {
		netdev_err(dev, "Unable to allocate sk_buff\n");
		return -ENOMEM;
	}

	buf_len = len - ETH_FCS_LEN;
	buf = (u32 *)skb_put(skb, buf_len);

	len = 0;
	do {
		sz = ocelot_rx_frame_word(ocelot, grp, false, &val);
		if (sz < 0) {
			err = sz;
			goto out_free_skb;
		}
		*buf++ = val;
		len += sz;
	} while (len < buf_len);

	/* Read the FCS */
	sz = ocelot_rx_frame_word(ocelot, grp, false, &val);
	if (sz < 0) {
		err = sz;
		goto out_free_skb;
	}

	/* Update the statistics if part of the FCS was read before */
	len -= ETH_FCS_LEN - sz;

	if (unlikely(dev->features & NETIF_F_RXFCS)) {
		buf = (u32 *)skb_put(skb, ETH_FCS_LEN);
		*buf = val;
	}

	if (ocelot->ptp) {
		ocelot_ptp_gettime64(&ocelot->ptp_info, &ts);

		tod_in_ns = ktime_set(ts.tv_sec, ts.tv_nsec);
		if ((tod_in_ns & 0xffffffff) < timestamp)
			full_ts_in_ns = (((tod_in_ns >> 32) - 1) << 32) |
					timestamp;
		else
			full_ts_in_ns = (tod_in_ns & GENMASK_ULL(63, 32)) |
					timestamp;

		shhwtstamps = skb_hwtstamps(skb);
		memset(shhwtstamps, 0, sizeof(struct skb_shared_hwtstamps));
		shhwtstamps->hwtstamp = full_ts_in_ns;
	}

	/* Everything we see on an interface that is in the HW bridge
	 * has already been forwarded.
	 */
	if (ocelot->ports[src_port]->bridge)
		skb->offload_fwd_mark = 1;

	skb->protocol = eth_type_trans(skb, dev);

	*nskb = skb;

	return 0;

out_free_skb:
	kfree_skb(skb);
	return err;
}
EXPORT_SYMBOL(ocelot_xtr_poll_frame);

bool ocelot_can_inject(struct ocelot *ocelot, int grp)
{
	u32 val = ocelot_read(ocelot, QS_INJ_STATUS);

	if (!(val & QS_INJ_STATUS_FIFO_RDY(BIT(grp))))
		return false;
	if (val & QS_INJ_STATUS_WMARK_REACHED(BIT(grp)))
		return false;

	return true;
}
EXPORT_SYMBOL(ocelot_can_inject);

void ocelot_port_inject_frame(struct ocelot *ocelot, int port, int grp,
			      u32 rew_op, struct sk_buff *skb)
{
	u32 ifh[OCELOT_TAG_LEN / 4] = {0};
	unsigned int i, count, last;

	ocelot_write_rix(ocelot, QS_INJ_CTRL_GAP_SIZE(1) |
			 QS_INJ_CTRL_SOF, QS_INJ_CTRL, grp);

	ocelot_ifh_set_bypass(ifh, 1);
	ocelot_ifh_set_dest(ifh, BIT_ULL(port));
	ocelot_ifh_set_tag_type(ifh, IFH_TAG_TYPE_C);
	ocelot_ifh_set_vid(ifh, skb_vlan_tag_get(skb));
	ocelot_ifh_set_rew_op(ifh, rew_op);

	for (i = 0; i < OCELOT_TAG_LEN / 4; i++)
		ocelot_write_rix(ocelot, ifh[i], QS_INJ_WR, grp);

	count = DIV_ROUND_UP(skb->len, 4);
	last = skb->len % 4;
	for (i = 0; i < count; i++)
		ocelot_write_rix(ocelot, ((u32 *)skb->data)[i], QS_INJ_WR, grp);
>>>>>>> upstream/android-13

	/* Add padding */
	while (i < (OCELOT_BUFFER_CELL_SZ / 4)) {
		ocelot_write_rix(ocelot, 0, QS_INJ_WR, grp);
		i++;
	}

	/* Indicate EOF and valid bytes in last word */
	ocelot_write_rix(ocelot, QS_INJ_CTRL_GAP_SIZE(1) |
			 QS_INJ_CTRL_VLD_BYTES(skb->len < OCELOT_BUFFER_CELL_SZ ? 0 : last) |
			 QS_INJ_CTRL_EOF,
			 QS_INJ_CTRL, grp);

	/* Add dummy CRC */
	ocelot_write_rix(ocelot, 0, QS_INJ_WR, grp);
	skb_tx_timestamp(skb);

<<<<<<< HEAD
	dev->stats.tx_packets++;
	dev->stats.tx_bytes += skb->len;
	dev_kfree_skb_any(skb);

	return NETDEV_TX_OK;
}

static void ocelot_mact_mc_reset(struct ocelot_port *port)
{
	struct ocelot *ocelot = port->ocelot;
	struct netdev_hw_addr *ha, *n;

	/* Free and forget all the MAC addresses stored in the port private mc
	 * list. These are mc addresses that were previously added by calling
	 * ocelot_mact_mc_add().
	 */
	list_for_each_entry_safe(ha, n, &port->mc, list) {
		ocelot_mact_forget(ocelot, ha->addr, port->pvid);
		list_del(&ha->list);
		kfree(ha);
	}
}

static int ocelot_mact_mc_add(struct ocelot_port *port,
			      struct netdev_hw_addr *hw_addr)
{
	struct ocelot *ocelot = port->ocelot;
	struct netdev_hw_addr *ha = kzalloc(sizeof(*ha), GFP_ATOMIC);

	if (!ha)
		return -ENOMEM;

	memcpy(ha, hw_addr, sizeof(*ha));
	list_add_tail(&ha->list, &port->mc);

	ocelot_mact_learn(ocelot, PGID_CPU, ha->addr, port->pvid,
			  ENTRYTYPE_LOCKED);

	return 0;
}

static void ocelot_set_rx_mode(struct net_device *dev)
{
	struct ocelot_port *port = netdev_priv(dev);
	struct ocelot *ocelot = port->ocelot;
	struct netdev_hw_addr *ha;
	int i;
	u32 val;

	/* This doesn't handle promiscuous mode because the bridge core is
	 * setting IFF_PROMISC on all slave interfaces and all frames would be
	 * forwarded to the CPU port.
	 */
	val = GENMASK(ocelot->num_phys_ports - 1, 0);
	for (i = ocelot->num_phys_ports + 1; i < PGID_CPU; i++)
		ocelot_write_rix(ocelot, val, ANA_PGID_PGID, i);

	/* Handle the device multicast addresses. First remove all the
	 * previously installed addresses and then add the latest ones to the
	 * mac table.
	 */
	ocelot_mact_mc_reset(port);
	netdev_for_each_mc_addr(ha, dev)
		ocelot_mact_mc_add(port, ha);
}

static int ocelot_port_get_phys_port_name(struct net_device *dev,
					  char *buf, size_t len)
{
	struct ocelot_port *port = netdev_priv(dev);
	int ret;

	ret = snprintf(buf, len, "p%d", port->chip_port);
	if (ret >= len)
		return -EINVAL;

	return 0;
}

static int ocelot_port_set_mac_address(struct net_device *dev, void *p)
{
	struct ocelot_port *port = netdev_priv(dev);
	struct ocelot *ocelot = port->ocelot;
	const struct sockaddr *addr = p;

	/* Learn the new net device MAC address in the mac table. */
	ocelot_mact_learn(ocelot, PGID_CPU, addr->sa_data, port->pvid,
			  ENTRYTYPE_LOCKED);
	/* Then forget the previous one. */
	ocelot_mact_forget(ocelot, dev->dev_addr, port->pvid);

	ether_addr_copy(dev->dev_addr, addr->sa_data);
	return 0;
}

static void ocelot_get_stats64(struct net_device *dev,
			       struct rtnl_link_stats64 *stats)
{
	struct ocelot_port *port = netdev_priv(dev);
	struct ocelot *ocelot = port->ocelot;

	/* Configure the port to read the stats from */
	ocelot_write(ocelot, SYS_STAT_CFG_STAT_VIEW(port->chip_port),
		     SYS_STAT_CFG);

	/* Get Rx stats */
	stats->rx_bytes = ocelot_read(ocelot, SYS_COUNT_RX_OCTETS);
	stats->rx_packets = ocelot_read(ocelot, SYS_COUNT_RX_SHORTS) +
			    ocelot_read(ocelot, SYS_COUNT_RX_FRAGMENTS) +
			    ocelot_read(ocelot, SYS_COUNT_RX_JABBERS) +
			    ocelot_read(ocelot, SYS_COUNT_RX_LONGS) +
			    ocelot_read(ocelot, SYS_COUNT_RX_64) +
			    ocelot_read(ocelot, SYS_COUNT_RX_65_127) +
			    ocelot_read(ocelot, SYS_COUNT_RX_128_255) +
			    ocelot_read(ocelot, SYS_COUNT_RX_256_1023) +
			    ocelot_read(ocelot, SYS_COUNT_RX_1024_1526) +
			    ocelot_read(ocelot, SYS_COUNT_RX_1527_MAX);
	stats->multicast = ocelot_read(ocelot, SYS_COUNT_RX_MULTICAST);
	stats->rx_dropped = dev->stats.rx_dropped;

	/* Get Tx stats */
	stats->tx_bytes = ocelot_read(ocelot, SYS_COUNT_TX_OCTETS);
	stats->tx_packets = ocelot_read(ocelot, SYS_COUNT_TX_64) +
			    ocelot_read(ocelot, SYS_COUNT_TX_65_127) +
			    ocelot_read(ocelot, SYS_COUNT_TX_128_511) +
			    ocelot_read(ocelot, SYS_COUNT_TX_512_1023) +
			    ocelot_read(ocelot, SYS_COUNT_TX_1024_1526) +
			    ocelot_read(ocelot, SYS_COUNT_TX_1527_MAX);
	stats->tx_dropped = ocelot_read(ocelot, SYS_COUNT_TX_DROPS) +
			    ocelot_read(ocelot, SYS_COUNT_TX_AGING);
	stats->collisions = ocelot_read(ocelot, SYS_COUNT_TX_COLLISION);
}

static int ocelot_fdb_add(struct ndmsg *ndm, struct nlattr *tb[],
			  struct net_device *dev, const unsigned char *addr,
			  u16 vid, u16 flags)
{
	struct ocelot_port *port = netdev_priv(dev);
	struct ocelot *ocelot = port->ocelot;

	if (!vid) {
		if (!port->vlan_aware)
			/* If the bridge is not VLAN aware and no VID was
			 * provided, set it to pvid to ensure the MAC entry
			 * matches incoming untagged packets
			 */
			vid = port->pvid;
		else
			/* If the bridge is VLAN aware a VID must be provided as
			 * otherwise the learnt entry wouldn't match any frame.
			 */
			return -EINVAL;
	}

	return ocelot_mact_learn(ocelot, port->chip_port, addr, vid,
				 ENTRYTYPE_LOCKED);
}

static int ocelot_fdb_del(struct ndmsg *ndm, struct nlattr *tb[],
			  struct net_device *dev,
			  const unsigned char *addr, u16 vid)
{
	struct ocelot_port *port = netdev_priv(dev);
	struct ocelot *ocelot = port->ocelot;

	return ocelot_mact_forget(ocelot, addr, vid);
}

struct ocelot_dump_ctx {
	struct net_device *dev;
	struct sk_buff *skb;
	struct netlink_callback *cb;
	int idx;
};

static int ocelot_fdb_do_dump(struct ocelot_mact_entry *entry,
			      struct ocelot_dump_ctx *dump)
{
=======
	skb->dev->stats.tx_packets++;
	skb->dev->stats.tx_bytes += skb->len;
}
EXPORT_SYMBOL(ocelot_port_inject_frame);

void ocelot_drain_cpu_queue(struct ocelot *ocelot, int grp)
{
	while (ocelot_read(ocelot, QS_XTR_DATA_PRESENT) & BIT(grp))
		ocelot_read_rix(ocelot, QS_XTR_RD, grp);
}
EXPORT_SYMBOL(ocelot_drain_cpu_queue);

int ocelot_fdb_add(struct ocelot *ocelot, int port,
		   const unsigned char *addr, u16 vid)
{
	int pgid = port;

	if (port == ocelot->npi)
		pgid = PGID_CPU;

	return ocelot_mact_learn(ocelot, pgid, addr, vid, ENTRYTYPE_LOCKED);
}
EXPORT_SYMBOL(ocelot_fdb_add);

int ocelot_fdb_del(struct ocelot *ocelot, int port,
		   const unsigned char *addr, u16 vid)
{
	return ocelot_mact_forget(ocelot, addr, vid);
}
EXPORT_SYMBOL(ocelot_fdb_del);

int ocelot_port_fdb_do_dump(const unsigned char *addr, u16 vid,
			    bool is_static, void *data)
{
	struct ocelot_dump_ctx *dump = data;
>>>>>>> upstream/android-13
	u32 portid = NETLINK_CB(dump->cb->skb).portid;
	u32 seq = dump->cb->nlh->nlmsg_seq;
	struct nlmsghdr *nlh;
	struct ndmsg *ndm;

	if (dump->idx < dump->cb->args[2])
		goto skip;

	nlh = nlmsg_put(dump->skb, portid, seq, RTM_NEWNEIGH,
			sizeof(*ndm), NLM_F_MULTI);
	if (!nlh)
		return -EMSGSIZE;

	ndm = nlmsg_data(nlh);
	ndm->ndm_family  = AF_BRIDGE;
	ndm->ndm_pad1    = 0;
	ndm->ndm_pad2    = 0;
	ndm->ndm_flags   = NTF_SELF;
	ndm->ndm_type    = 0;
	ndm->ndm_ifindex = dump->dev->ifindex;
<<<<<<< HEAD
	ndm->ndm_state   = NUD_REACHABLE;

	if (nla_put(dump->skb, NDA_LLADDR, ETH_ALEN, entry->mac))
		goto nla_put_failure;

	if (entry->vid && nla_put_u16(dump->skb, NDA_VLAN, entry->vid))
=======
	ndm->ndm_state   = is_static ? NUD_NOARP : NUD_REACHABLE;

	if (nla_put(dump->skb, NDA_LLADDR, ETH_ALEN, addr))
		goto nla_put_failure;

	if (vid && nla_put_u16(dump->skb, NDA_VLAN, vid))
>>>>>>> upstream/android-13
		goto nla_put_failure;

	nlmsg_end(dump->skb, nlh);

skip:
	dump->idx++;
	return 0;

nla_put_failure:
	nlmsg_cancel(dump->skb, nlh);
	return -EMSGSIZE;
}
<<<<<<< HEAD

static inline int ocelot_mact_read(struct ocelot_port *port, int row, int col,
				   struct ocelot_mact_entry *entry)
{
	struct ocelot *ocelot = port->ocelot;
	char mac[ETH_ALEN];
	u32 val, dst, macl, mach;
=======
EXPORT_SYMBOL(ocelot_port_fdb_do_dump);

static int ocelot_mact_read(struct ocelot *ocelot, int port, int row, int col,
			    struct ocelot_mact_entry *entry)
{
	u32 val, dst, macl, mach;
	char mac[ETH_ALEN];
>>>>>>> upstream/android-13

	/* Set row and column to read from */
	ocelot_field_write(ocelot, ANA_TABLES_MACTINDX_M_INDEX, row);
	ocelot_field_write(ocelot, ANA_TABLES_MACTINDX_BUCKET, col);

	/* Issue a read command */
	ocelot_write(ocelot,
		     ANA_TABLES_MACACCESS_MAC_TABLE_CMD(MACACCESS_CMD_READ),
		     ANA_TABLES_MACACCESS);

	if (ocelot_mact_wait_for_completion(ocelot))
		return -ETIMEDOUT;

	/* Read the entry flags */
	val = ocelot_read(ocelot, ANA_TABLES_MACACCESS);
	if (!(val & ANA_TABLES_MACACCESS_VALID))
		return -EINVAL;

	/* If the entry read has another port configured as its destination,
	 * do not report it.
	 */
	dst = (val & ANA_TABLES_MACACCESS_DEST_IDX_M) >> 3;
<<<<<<< HEAD
	if (dst != port->chip_port)
=======
	if (dst != port)
>>>>>>> upstream/android-13
		return -EINVAL;

	/* Get the entry's MAC address and VLAN id */
	macl = ocelot_read(ocelot, ANA_TABLES_MACLDATA);
	mach = ocelot_read(ocelot, ANA_TABLES_MACHDATA);

	mac[0] = (mach >> 8)  & 0xff;
	mac[1] = (mach >> 0)  & 0xff;
	mac[2] = (macl >> 24) & 0xff;
	mac[3] = (macl >> 16) & 0xff;
	mac[4] = (macl >> 8)  & 0xff;
	mac[5] = (macl >> 0)  & 0xff;

	entry->vid = (mach >> 16) & 0xfff;
	ether_addr_copy(entry->mac, mac);

	return 0;
}

<<<<<<< HEAD
static int ocelot_fdb_dump(struct sk_buff *skb, struct netlink_callback *cb,
			   struct net_device *dev,
			   struct net_device *filter_dev, int *idx)
{
	struct ocelot_port *port = netdev_priv(dev);
	int i, j, ret = 0;
	struct ocelot_dump_ctx dump = {
		.dev = dev,
		.skb = skb,
		.cb = cb,
		.idx = *idx,
	};

	struct ocelot_mact_entry entry;

	/* Loop through all the mac tables entries. There are 1024 rows of 4
	 * entries.
	 */
	for (i = 0; i < 1024; i++) {
		for (j = 0; j < 4; j++) {
			ret = ocelot_mact_read(port, i, j, &entry);
=======
int ocelot_fdb_dump(struct ocelot *ocelot, int port,
		    dsa_fdb_dump_cb_t *cb, void *data)
{
	int i, j;

	/* Loop through all the mac tables entries. */
	for (i = 0; i < ocelot->num_mact_rows; i++) {
		for (j = 0; j < 4; j++) {
			struct ocelot_mact_entry entry;
			bool is_static;
			int ret;

			ret = ocelot_mact_read(ocelot, port, i, j, &entry);
>>>>>>> upstream/android-13
			/* If the entry is invalid (wrong port, invalid...),
			 * skip it.
			 */
			if (ret == -EINVAL)
				continue;
			else if (ret)
<<<<<<< HEAD
				goto end;

			ret = ocelot_fdb_do_dump(&entry, &dump);
			if (ret)
				goto end;
		}
	}

end:
	*idx = dump.idx;
	return ret;
}

static int ocelot_vlan_rx_add_vid(struct net_device *dev, __be16 proto,
				  u16 vid)
{
	return ocelot_vlan_vid_add(dev, vid, false, false);
}

static int ocelot_vlan_rx_kill_vid(struct net_device *dev, __be16 proto,
				   u16 vid)
{
	return ocelot_vlan_vid_del(dev, vid);
}

static int ocelot_set_features(struct net_device *dev,
			       netdev_features_t features)
{
	struct ocelot_port *port = netdev_priv(dev);
	netdev_features_t changed = dev->features ^ features;

	if (changed & NETIF_F_HW_VLAN_CTAG_FILTER)
		ocelot_vlan_mode(port, features);

	return 0;
}

static const struct net_device_ops ocelot_port_netdev_ops = {
	.ndo_open			= ocelot_port_open,
	.ndo_stop			= ocelot_port_stop,
	.ndo_start_xmit			= ocelot_port_xmit,
	.ndo_set_rx_mode		= ocelot_set_rx_mode,
	.ndo_get_phys_port_name		= ocelot_port_get_phys_port_name,
	.ndo_set_mac_address		= ocelot_port_set_mac_address,
	.ndo_get_stats64		= ocelot_get_stats64,
	.ndo_fdb_add			= ocelot_fdb_add,
	.ndo_fdb_del			= ocelot_fdb_del,
	.ndo_fdb_dump			= ocelot_fdb_dump,
	.ndo_vlan_rx_add_vid		= ocelot_vlan_rx_add_vid,
	.ndo_vlan_rx_kill_vid		= ocelot_vlan_rx_kill_vid,
	.ndo_set_features		= ocelot_set_features,
};

static void ocelot_get_strings(struct net_device *netdev, u32 sset, u8 *data)
{
	struct ocelot_port *port = netdev_priv(netdev);
	struct ocelot *ocelot = port->ocelot;
=======
				return ret;

			is_static = (entry.type == ENTRYTYPE_LOCKED);

			ret = cb(entry.mac, entry.vid, is_static, data);
			if (ret)
				return ret;
		}
	}

	return 0;
}
EXPORT_SYMBOL(ocelot_fdb_dump);

int ocelot_hwstamp_get(struct ocelot *ocelot, int port, struct ifreq *ifr)
{
	return copy_to_user(ifr->ifr_data, &ocelot->hwtstamp_config,
			    sizeof(ocelot->hwtstamp_config)) ? -EFAULT : 0;
}
EXPORT_SYMBOL(ocelot_hwstamp_get);

int ocelot_hwstamp_set(struct ocelot *ocelot, int port, struct ifreq *ifr)
{
	struct ocelot_port *ocelot_port = ocelot->ports[port];
	struct hwtstamp_config cfg;

	if (copy_from_user(&cfg, ifr->ifr_data, sizeof(cfg)))
		return -EFAULT;

	/* reserved for future extensions */
	if (cfg.flags)
		return -EINVAL;

	/* Tx type sanity check */
	switch (cfg.tx_type) {
	case HWTSTAMP_TX_ON:
		ocelot_port->ptp_cmd = IFH_REW_OP_TWO_STEP_PTP;
		break;
	case HWTSTAMP_TX_ONESTEP_SYNC:
		/* IFH_REW_OP_ONE_STEP_PTP updates the correctional field, we
		 * need to update the origin time.
		 */
		ocelot_port->ptp_cmd = IFH_REW_OP_ORIGIN_PTP;
		break;
	case HWTSTAMP_TX_OFF:
		ocelot_port->ptp_cmd = 0;
		break;
	default:
		return -ERANGE;
	}

	mutex_lock(&ocelot->ptp_lock);

	switch (cfg.rx_filter) {
	case HWTSTAMP_FILTER_NONE:
		break;
	case HWTSTAMP_FILTER_PTP_V2_L4_EVENT:
	case HWTSTAMP_FILTER_PTP_V2_L4_SYNC:
	case HWTSTAMP_FILTER_PTP_V2_L4_DELAY_REQ:
	case HWTSTAMP_FILTER_PTP_V2_L2_EVENT:
	case HWTSTAMP_FILTER_PTP_V2_L2_SYNC:
	case HWTSTAMP_FILTER_PTP_V2_L2_DELAY_REQ:
	case HWTSTAMP_FILTER_PTP_V2_EVENT:
	case HWTSTAMP_FILTER_PTP_V2_SYNC:
	case HWTSTAMP_FILTER_PTP_V2_DELAY_REQ:
		cfg.rx_filter = HWTSTAMP_FILTER_PTP_V2_EVENT;
		break;
	default:
		mutex_unlock(&ocelot->ptp_lock);
		return -ERANGE;
	}

	/* Commit back the result & save it */
	memcpy(&ocelot->hwtstamp_config, &cfg, sizeof(cfg));
	mutex_unlock(&ocelot->ptp_lock);

	return copy_to_user(ifr->ifr_data, &cfg, sizeof(cfg)) ? -EFAULT : 0;
}
EXPORT_SYMBOL(ocelot_hwstamp_set);

void ocelot_get_strings(struct ocelot *ocelot, int port, u32 sset, u8 *data)
{
>>>>>>> upstream/android-13
	int i;

	if (sset != ETH_SS_STATS)
		return;

	for (i = 0; i < ocelot->num_stats; i++)
		memcpy(data + i * ETH_GSTRING_LEN, ocelot->stats_layout[i].name,
		       ETH_GSTRING_LEN);
}
<<<<<<< HEAD

static void ocelot_check_stats(struct work_struct *work)
{
	struct delayed_work *del_work = to_delayed_work(work);
	struct ocelot *ocelot = container_of(del_work, struct ocelot, stats_work);
	int i, j;

	mutex_lock(&ocelot->stats_lock);

=======
EXPORT_SYMBOL(ocelot_get_strings);

/* Caller must hold &ocelot->stats_lock */
static void ocelot_update_stats(struct ocelot *ocelot)
{
	int i, j;

>>>>>>> upstream/android-13
	for (i = 0; i < ocelot->num_phys_ports; i++) {
		/* Configure the port to read the stats from */
		ocelot_write(ocelot, SYS_STAT_CFG_STAT_VIEW(i), SYS_STAT_CFG);

		for (j = 0; j < ocelot->num_stats; j++) {
			u32 val;
			unsigned int idx = i * ocelot->num_stats + j;

			val = ocelot_read_rix(ocelot, SYS_COUNT_RX_OCTETS,
					      ocelot->stats_layout[j].offset);

			if (val < (ocelot->stats[idx] & U32_MAX))
				ocelot->stats[idx] += (u64)1 << 32;

			ocelot->stats[idx] = (ocelot->stats[idx] &
					      ~(u64)U32_MAX) + val;
		}
	}
<<<<<<< HEAD

	cancel_delayed_work(&ocelot->stats_work);
	queue_delayed_work(ocelot->stats_queue, &ocelot->stats_work,
			   OCELOT_STATS_CHECK_DELAY);

	mutex_unlock(&ocelot->stats_lock);
}

static void ocelot_get_ethtool_stats(struct net_device *dev,
				     struct ethtool_stats *stats, u64 *data)
{
	struct ocelot_port *port = netdev_priv(dev);
	struct ocelot *ocelot = port->ocelot;
	int i;

	/* check and update now */
	ocelot_check_stats(&ocelot->stats_work.work);

	/* Copy all counters */
	for (i = 0; i < ocelot->num_stats; i++)
		*data++ = ocelot->stats[port->chip_port * ocelot->num_stats + i];
}

static int ocelot_get_sset_count(struct net_device *dev, int sset)
{
	struct ocelot_port *port = netdev_priv(dev);
	struct ocelot *ocelot = port->ocelot;

	if (sset != ETH_SS_STATS)
		return -EOPNOTSUPP;
	return ocelot->num_stats;
}

static const struct ethtool_ops ocelot_ethtool_ops = {
	.get_strings		= ocelot_get_strings,
	.get_ethtool_stats	= ocelot_get_ethtool_stats,
	.get_sset_count		= ocelot_get_sset_count,
	.get_link_ksettings	= phy_ethtool_get_link_ksettings,
	.set_link_ksettings	= phy_ethtool_set_link_ksettings,
};

static int ocelot_port_attr_get(struct net_device *dev,
				struct switchdev_attr *attr)
{
	struct ocelot_port *ocelot_port = netdev_priv(dev);
	struct ocelot *ocelot = ocelot_port->ocelot;

	switch (attr->id) {
	case SWITCHDEV_ATTR_ID_PORT_PARENT_ID:
		attr->u.ppid.id_len = sizeof(ocelot->base_mac);
		memcpy(&attr->u.ppid.id, &ocelot->base_mac,
		       attr->u.ppid.id_len);
		break;
	default:
		return -EOPNOTSUPP;
	}

	return 0;
}

static int ocelot_port_attr_stp_state_set(struct ocelot_port *ocelot_port,
					  struct switchdev_trans *trans,
					  u8 state)
{
	struct ocelot *ocelot = ocelot_port->ocelot;
	u32 port_cfg;
	int port, i;

	if (switchdev_trans_ph_prepare(trans))
		return 0;

	if (!(BIT(ocelot_port->chip_port) & ocelot->bridge_mask))
		return 0;

	port_cfg = ocelot_read_gix(ocelot, ANA_PORT_PORT_CFG,
				   ocelot_port->chip_port);

	switch (state) {
	case BR_STATE_FORWARDING:
		ocelot->bridge_fwd_mask |= BIT(ocelot_port->chip_port);
		/* Fallthrough */
	case BR_STATE_LEARNING:
		port_cfg |= ANA_PORT_PORT_CFG_LEARN_ENA;
		break;

	default:
		port_cfg &= ~ANA_PORT_PORT_CFG_LEARN_ENA;
		ocelot->bridge_fwd_mask &= ~BIT(ocelot_port->chip_port);
		break;
	}

	ocelot_write_gix(ocelot, port_cfg, ANA_PORT_PORT_CFG,
			 ocelot_port->chip_port);
=======
}

static void ocelot_check_stats_work(struct work_struct *work)
{
	struct delayed_work *del_work = to_delayed_work(work);
	struct ocelot *ocelot = container_of(del_work, struct ocelot,
					     stats_work);

	mutex_lock(&ocelot->stats_lock);
	ocelot_update_stats(ocelot);
	mutex_unlock(&ocelot->stats_lock);

	queue_delayed_work(ocelot->stats_queue, &ocelot->stats_work,
			   OCELOT_STATS_CHECK_DELAY);
}

void ocelot_get_ethtool_stats(struct ocelot *ocelot, int port, u64 *data)
{
	int i;

	mutex_lock(&ocelot->stats_lock);

	/* check and update now */
	ocelot_update_stats(ocelot);

	/* Copy all counters */
	for (i = 0; i < ocelot->num_stats; i++)
		*data++ = ocelot->stats[port * ocelot->num_stats + i];

	mutex_unlock(&ocelot->stats_lock);
}
EXPORT_SYMBOL(ocelot_get_ethtool_stats);

int ocelot_get_sset_count(struct ocelot *ocelot, int port, int sset)
{
	if (sset != ETH_SS_STATS)
		return -EOPNOTSUPP;

	return ocelot->num_stats;
}
EXPORT_SYMBOL(ocelot_get_sset_count);

int ocelot_get_ts_info(struct ocelot *ocelot, int port,
		       struct ethtool_ts_info *info)
{
	info->phc_index = ocelot->ptp_clock ?
			  ptp_clock_index(ocelot->ptp_clock) : -1;
	if (info->phc_index == -1) {
		info->so_timestamping |= SOF_TIMESTAMPING_TX_SOFTWARE |
					 SOF_TIMESTAMPING_RX_SOFTWARE |
					 SOF_TIMESTAMPING_SOFTWARE;
		return 0;
	}
	info->so_timestamping |= SOF_TIMESTAMPING_TX_SOFTWARE |
				 SOF_TIMESTAMPING_RX_SOFTWARE |
				 SOF_TIMESTAMPING_SOFTWARE |
				 SOF_TIMESTAMPING_TX_HARDWARE |
				 SOF_TIMESTAMPING_RX_HARDWARE |
				 SOF_TIMESTAMPING_RAW_HARDWARE;
	info->tx_types = BIT(HWTSTAMP_TX_OFF) | BIT(HWTSTAMP_TX_ON) |
			 BIT(HWTSTAMP_TX_ONESTEP_SYNC);
	info->rx_filters = BIT(HWTSTAMP_FILTER_NONE) |
			   BIT(HWTSTAMP_FILTER_PTP_V2_EVENT) |
			   BIT(HWTSTAMP_FILTER_PTP_V2_L2_EVENT) |
			   BIT(HWTSTAMP_FILTER_PTP_V2_L4_EVENT);

	return 0;
}
EXPORT_SYMBOL(ocelot_get_ts_info);

static u32 ocelot_get_bond_mask(struct ocelot *ocelot, struct net_device *bond)
{
	u32 mask = 0;
	int port;

	for (port = 0; port < ocelot->num_phys_ports; port++) {
		struct ocelot_port *ocelot_port = ocelot->ports[port];

		if (!ocelot_port)
			continue;

		if (ocelot_port->bond == bond)
			mask |= BIT(port);
	}

	return mask;
}

static u32 ocelot_get_bridge_fwd_mask(struct ocelot *ocelot, int src_port,
				      struct net_device *bridge)
{
	struct ocelot_port *ocelot_port = ocelot->ports[src_port];
	u32 mask = 0;
	int port;

	if (!ocelot_port || ocelot_port->bridge != bridge ||
	    ocelot_port->stp_state != BR_STATE_FORWARDING)
		return 0;

	for (port = 0; port < ocelot->num_phys_ports; port++) {
		ocelot_port = ocelot->ports[port];

		if (!ocelot_port)
			continue;

		if (ocelot_port->stp_state == BR_STATE_FORWARDING &&
		    ocelot_port->bridge == bridge)
			mask |= BIT(port);
	}

	return mask;
}

static u32 ocelot_get_dsa_8021q_cpu_mask(struct ocelot *ocelot)
{
	u32 mask = 0;
	int port;

	for (port = 0; port < ocelot->num_phys_ports; port++) {
		struct ocelot_port *ocelot_port = ocelot->ports[port];

		if (!ocelot_port)
			continue;

		if (ocelot_port->is_dsa_8021q_cpu)
			mask |= BIT(port);
	}

	return mask;
}

void ocelot_apply_bridge_fwd_mask(struct ocelot *ocelot)
{
	unsigned long cpu_fwd_mask;
	int port;

	/* If a DSA tag_8021q CPU exists, it needs to be included in the
	 * regular forwarding path of the front ports regardless of whether
	 * those are bridged or standalone.
	 * If DSA tag_8021q is not used, this returns 0, which is fine because
	 * the hardware-based CPU port module can be a destination for packets
	 * even if it isn't part of PGID_SRC.
	 */
	cpu_fwd_mask = ocelot_get_dsa_8021q_cpu_mask(ocelot);
>>>>>>> upstream/android-13

	/* Apply FWD mask. The loop is needed to add/remove the current port as
	 * a source for the other ports.
	 */
	for (port = 0; port < ocelot->num_phys_ports; port++) {
<<<<<<< HEAD
		if (ocelot->bridge_fwd_mask & BIT(port)) {
			unsigned long mask = ocelot->bridge_fwd_mask & ~BIT(port);

			for (i = 0; i < ocelot->num_phys_ports; i++) {
				unsigned long bond_mask = ocelot->lags[i];

				if (!bond_mask)
					continue;

				if (bond_mask & BIT(port)) {
					mask &= ~bond_mask;
					break;
				}
			}

			ocelot_write_rix(ocelot,
					 BIT(ocelot->num_phys_ports) | mask,
					 ANA_PGID_PGID, PGID_SRC + port);
		} else {
			/* Only the CPU port, this is compatible with link
			 * aggregation.
			 */
			ocelot_write_rix(ocelot,
					 BIT(ocelot->num_phys_ports),
					 ANA_PGID_PGID, PGID_SRC + port);
		}
	}

	return 0;
}

static void ocelot_port_attr_ageing_set(struct ocelot_port *ocelot_port,
					unsigned long ageing_clock_t)
{
	struct ocelot *ocelot = ocelot_port->ocelot;
	unsigned long ageing_jiffies = clock_t_to_jiffies(ageing_clock_t);
	u32 ageing_time = jiffies_to_msecs(ageing_jiffies) / 1000;

	ocelot_write(ocelot, ANA_AUTOAGE_AGE_PERIOD(ageing_time / 2),
		     ANA_AUTOAGE);
}

static void ocelot_port_attr_mc_set(struct ocelot_port *port, bool mc)
{
	struct ocelot *ocelot = port->ocelot;
	u32 val = ocelot_read_gix(ocelot, ANA_PORT_CPU_FWD_CFG,
				  port->chip_port);

	if (mc)
		val |= ANA_PORT_CPU_FWD_CFG_CPU_IGMP_REDIR_ENA |
		       ANA_PORT_CPU_FWD_CFG_CPU_MLD_REDIR_ENA |
		       ANA_PORT_CPU_FWD_CFG_CPU_IPMC_CTRL_COPY_ENA;
	else
		val &= ~(ANA_PORT_CPU_FWD_CFG_CPU_IGMP_REDIR_ENA |
			 ANA_PORT_CPU_FWD_CFG_CPU_MLD_REDIR_ENA |
			 ANA_PORT_CPU_FWD_CFG_CPU_IPMC_CTRL_COPY_ENA);

	ocelot_write_gix(ocelot, val, ANA_PORT_CPU_FWD_CFG, port->chip_port);
}

static int ocelot_port_attr_set(struct net_device *dev,
				const struct switchdev_attr *attr,
				struct switchdev_trans *trans)
{
	struct ocelot_port *ocelot_port = netdev_priv(dev);
	int err = 0;

	switch (attr->id) {
	case SWITCHDEV_ATTR_ID_PORT_STP_STATE:
		ocelot_port_attr_stp_state_set(ocelot_port, trans,
					       attr->u.stp_state);
		break;
	case SWITCHDEV_ATTR_ID_BRIDGE_AGEING_TIME:
		ocelot_port_attr_ageing_set(ocelot_port, attr->u.ageing_time);
		break;
	case SWITCHDEV_ATTR_ID_BRIDGE_VLAN_FILTERING:
		ocelot_port->vlan_aware = attr->u.vlan_filtering;
		ocelot_vlan_port_apply(ocelot_port->ocelot, ocelot_port);
		break;
	case SWITCHDEV_ATTR_ID_BRIDGE_MC_DISABLED:
		ocelot_port_attr_mc_set(ocelot_port, !attr->u.mc_disabled);
		break;
	default:
		err = -EOPNOTSUPP;
		break;
	}

	return err;
}

static int ocelot_port_obj_add_vlan(struct net_device *dev,
				    const struct switchdev_obj_port_vlan *vlan,
				    struct switchdev_trans *trans)
{
	int ret;
	u16 vid;

	for (vid = vlan->vid_begin; vid <= vlan->vid_end; vid++) {
		ret = ocelot_vlan_vid_add(dev, vid,
					  vlan->flags & BRIDGE_VLAN_INFO_PVID,
					  vlan->flags & BRIDGE_VLAN_INFO_UNTAGGED);
		if (ret)
			return ret;
	}

	return 0;
}

static int ocelot_port_vlan_del_vlan(struct net_device *dev,
				     const struct switchdev_obj_port_vlan *vlan)
{
	int ret;
	u16 vid;

	for (vid = vlan->vid_begin; vid <= vlan->vid_end; vid++) {
		ret = ocelot_vlan_vid_del(dev, vid);

		if (ret)
			return ret;
	}

	return 0;
}
=======
		struct ocelot_port *ocelot_port = ocelot->ports[port];
		unsigned long mask;

		if (!ocelot_port) {
			/* Unused ports can't send anywhere */
			mask = 0;
		} else if (ocelot_port->is_dsa_8021q_cpu) {
			/* The DSA tag_8021q CPU ports need to be able to
			 * forward packets to all other ports except for
			 * themselves
			 */
			mask = GENMASK(ocelot->num_phys_ports - 1, 0);
			mask &= ~cpu_fwd_mask;
		} else if (ocelot_port->bridge) {
			struct net_device *bridge = ocelot_port->bridge;
			struct net_device *bond = ocelot_port->bond;

			mask = ocelot_get_bridge_fwd_mask(ocelot, port, bridge);
			mask |= cpu_fwd_mask;
			mask &= ~BIT(port);
			if (bond)
				mask &= ~ocelot_get_bond_mask(ocelot, bond);
		} else {
			/* Standalone ports forward only to DSA tag_8021q CPU
			 * ports (if those exist), or to the hardware CPU port
			 * module otherwise.
			 */
			mask = cpu_fwd_mask;
		}

		ocelot_write_rix(ocelot, mask, ANA_PGID_PGID, PGID_SRC + port);
	}
}
EXPORT_SYMBOL(ocelot_apply_bridge_fwd_mask);

void ocelot_bridge_stp_state_set(struct ocelot *ocelot, int port, u8 state)
{
	struct ocelot_port *ocelot_port = ocelot->ports[port];
	u32 learn_ena = 0;

	ocelot_port->stp_state = state;

	if ((state == BR_STATE_LEARNING || state == BR_STATE_FORWARDING) &&
	    ocelot_port->learn_ena)
		learn_ena = ANA_PORT_PORT_CFG_LEARN_ENA;

	ocelot_rmw_gix(ocelot, learn_ena, ANA_PORT_PORT_CFG_LEARN_ENA,
		       ANA_PORT_PORT_CFG, port);

	ocelot_apply_bridge_fwd_mask(ocelot);
}
EXPORT_SYMBOL(ocelot_bridge_stp_state_set);

void ocelot_set_ageing_time(struct ocelot *ocelot, unsigned int msecs)
{
	unsigned int age_period = ANA_AUTOAGE_AGE_PERIOD(msecs / 2000);

	/* Setting AGE_PERIOD to zero effectively disables automatic aging,
	 * which is clearly not what our intention is. So avoid that.
	 */
	if (!age_period)
		age_period = 1;

	ocelot_rmw(ocelot, age_period, ANA_AUTOAGE_AGE_PERIOD_M, ANA_AUTOAGE);
}
EXPORT_SYMBOL(ocelot_set_ageing_time);
>>>>>>> upstream/android-13

static struct ocelot_multicast *ocelot_multicast_get(struct ocelot *ocelot,
						     const unsigned char *addr,
						     u16 vid)
{
	struct ocelot_multicast *mc;

	list_for_each_entry(mc, &ocelot->multicast, list) {
		if (ether_addr_equal(mc->addr, addr) && mc->vid == vid)
			return mc;
	}

	return NULL;
}

<<<<<<< HEAD
static int ocelot_port_obj_add_mdb(struct net_device *dev,
				   const struct switchdev_obj_port_mdb *mdb,
				   struct switchdev_trans *trans)
{
	struct ocelot_port *port = netdev_priv(dev);
	struct ocelot *ocelot = port->ocelot;
	struct ocelot_multicast *mc;
	unsigned char addr[ETH_ALEN];
	u16 vid = mdb->vid;
	bool new = false;

	if (!vid)
		vid = port->pvid;

	mc = ocelot_multicast_get(ocelot, mdb->addr, vid);
	if (!mc) {
=======
static enum macaccess_entry_type ocelot_classify_mdb(const unsigned char *addr)
{
	if (addr[0] == 0x01 && addr[1] == 0x00 && addr[2] == 0x5e)
		return ENTRYTYPE_MACv4;
	if (addr[0] == 0x33 && addr[1] == 0x33)
		return ENTRYTYPE_MACv6;
	return ENTRYTYPE_LOCKED;
}

static struct ocelot_pgid *ocelot_pgid_alloc(struct ocelot *ocelot, int index,
					     unsigned long ports)
{
	struct ocelot_pgid *pgid;

	pgid = kzalloc(sizeof(*pgid), GFP_KERNEL);
	if (!pgid)
		return ERR_PTR(-ENOMEM);

	pgid->ports = ports;
	pgid->index = index;
	refcount_set(&pgid->refcount, 1);
	list_add_tail(&pgid->list, &ocelot->pgids);

	return pgid;
}

static void ocelot_pgid_free(struct ocelot *ocelot, struct ocelot_pgid *pgid)
{
	if (!refcount_dec_and_test(&pgid->refcount))
		return;

	list_del(&pgid->list);
	kfree(pgid);
}

static struct ocelot_pgid *ocelot_mdb_get_pgid(struct ocelot *ocelot,
					       const struct ocelot_multicast *mc)
{
	struct ocelot_pgid *pgid;
	int index;

	/* According to VSC7514 datasheet 3.9.1.5 IPv4 Multicast Entries and
	 * 3.9.1.6 IPv6 Multicast Entries, "Instead of a lookup in the
	 * destination mask table (PGID), the destination set is programmed as
	 * part of the entry MAC address.", and the DEST_IDX is set to 0.
	 */
	if (mc->entry_type == ENTRYTYPE_MACv4 ||
	    mc->entry_type == ENTRYTYPE_MACv6)
		return ocelot_pgid_alloc(ocelot, 0, mc->ports);

	list_for_each_entry(pgid, &ocelot->pgids, list) {
		/* When searching for a nonreserved multicast PGID, ignore the
		 * dummy PGID of zero that we have for MACv4/MACv6 entries
		 */
		if (pgid->index && pgid->ports == mc->ports) {
			refcount_inc(&pgid->refcount);
			return pgid;
		}
	}

	/* Search for a free index in the nonreserved multicast PGID area */
	for_each_nonreserved_multicast_dest_pgid(ocelot, index) {
		bool used = false;

		list_for_each_entry(pgid, &ocelot->pgids, list) {
			if (pgid->index == index) {
				used = true;
				break;
			}
		}

		if (!used)
			return ocelot_pgid_alloc(ocelot, index, mc->ports);
	}

	return ERR_PTR(-ENOSPC);
}

static void ocelot_encode_ports_to_mdb(unsigned char *addr,
				       struct ocelot_multicast *mc)
{
	ether_addr_copy(addr, mc->addr);

	if (mc->entry_type == ENTRYTYPE_MACv4) {
		addr[0] = 0;
		addr[1] = mc->ports >> 8;
		addr[2] = mc->ports & 0xff;
	} else if (mc->entry_type == ENTRYTYPE_MACv6) {
		addr[0] = mc->ports >> 8;
		addr[1] = mc->ports & 0xff;
	}
}

int ocelot_port_mdb_add(struct ocelot *ocelot, int port,
			const struct switchdev_obj_port_mdb *mdb)
{
	unsigned char addr[ETH_ALEN];
	struct ocelot_multicast *mc;
	struct ocelot_pgid *pgid;
	u16 vid = mdb->vid;

	if (port == ocelot->npi)
		port = ocelot->num_phys_ports;

	mc = ocelot_multicast_get(ocelot, mdb->addr, vid);
	if (!mc) {
		/* New entry */
>>>>>>> upstream/android-13
		mc = devm_kzalloc(ocelot->dev, sizeof(*mc), GFP_KERNEL);
		if (!mc)
			return -ENOMEM;

<<<<<<< HEAD
		memcpy(mc->addr, mdb->addr, ETH_ALEN);
		mc->vid = vid;

		list_add_tail(&mc->list, &ocelot->multicast);
		new = true;
	}

	memcpy(addr, mc->addr, ETH_ALEN);
	addr[0] = 0;

	if (!new) {
		addr[2] = mc->ports << 0;
		addr[1] = mc->ports << 8;
		ocelot_mact_forget(ocelot, addr, vid);
	}

	mc->ports |= BIT(port->chip_port);
	addr[2] = mc->ports << 0;
	addr[1] = mc->ports << 8;

	return ocelot_mact_learn(ocelot, 0, addr, vid, ENTRYTYPE_MACv4);
}

static int ocelot_port_obj_del_mdb(struct net_device *dev,
				   const struct switchdev_obj_port_mdb *mdb)
{
	struct ocelot_port *port = netdev_priv(dev);
	struct ocelot *ocelot = port->ocelot;
	struct ocelot_multicast *mc;
	unsigned char addr[ETH_ALEN];
	u16 vid = mdb->vid;

	if (!vid)
		vid = port->pvid;
=======
		mc->entry_type = ocelot_classify_mdb(mdb->addr);
		ether_addr_copy(mc->addr, mdb->addr);
		mc->vid = vid;

		list_add_tail(&mc->list, &ocelot->multicast);
	} else {
		/* Existing entry. Clean up the current port mask from
		 * hardware now, because we'll be modifying it.
		 */
		ocelot_pgid_free(ocelot, mc->pgid);
		ocelot_encode_ports_to_mdb(addr, mc);
		ocelot_mact_forget(ocelot, addr, vid);
	}

	mc->ports |= BIT(port);

	pgid = ocelot_mdb_get_pgid(ocelot, mc);
	if (IS_ERR(pgid)) {
		dev_err(ocelot->dev,
			"Cannot allocate PGID for mdb %pM vid %d\n",
			mc->addr, mc->vid);
		devm_kfree(ocelot->dev, mc);
		return PTR_ERR(pgid);
	}
	mc->pgid = pgid;

	ocelot_encode_ports_to_mdb(addr, mc);

	if (mc->entry_type != ENTRYTYPE_MACv4 &&
	    mc->entry_type != ENTRYTYPE_MACv6)
		ocelot_write_rix(ocelot, pgid->ports, ANA_PGID_PGID,
				 pgid->index);

	return ocelot_mact_learn(ocelot, pgid->index, addr, vid,
				 mc->entry_type);
}
EXPORT_SYMBOL(ocelot_port_mdb_add);

int ocelot_port_mdb_del(struct ocelot *ocelot, int port,
			const struct switchdev_obj_port_mdb *mdb)
{
	unsigned char addr[ETH_ALEN];
	struct ocelot_multicast *mc;
	struct ocelot_pgid *pgid;
	u16 vid = mdb->vid;

	if (port == ocelot->npi)
		port = ocelot->num_phys_ports;
>>>>>>> upstream/android-13

	mc = ocelot_multicast_get(ocelot, mdb->addr, vid);
	if (!mc)
		return -ENOENT;

<<<<<<< HEAD
	memcpy(addr, mc->addr, ETH_ALEN);
	addr[2] = mc->ports << 0;
	addr[1] = mc->ports << 8;
	addr[0] = 0;
	ocelot_mact_forget(ocelot, addr, vid);

	mc->ports &= ~BIT(port->chip_port);
=======
	ocelot_encode_ports_to_mdb(addr, mc);
	ocelot_mact_forget(ocelot, addr, vid);

	ocelot_pgid_free(ocelot, mc->pgid);
	mc->ports &= ~BIT(port);
>>>>>>> upstream/android-13
	if (!mc->ports) {
		list_del(&mc->list);
		devm_kfree(ocelot->dev, mc);
		return 0;
	}

<<<<<<< HEAD
	addr[2] = mc->ports << 0;
	addr[1] = mc->ports << 8;

	return ocelot_mact_learn(ocelot, 0, addr, vid, ENTRYTYPE_MACv4);
}

static int ocelot_port_obj_add(struct net_device *dev,
			       const struct switchdev_obj *obj,
			       struct switchdev_trans *trans)
{
	int ret = 0;

	switch (obj->id) {
	case SWITCHDEV_OBJ_ID_PORT_VLAN:
		ret = ocelot_port_obj_add_vlan(dev,
					       SWITCHDEV_OBJ_PORT_VLAN(obj),
					       trans);
		break;
	case SWITCHDEV_OBJ_ID_PORT_MDB:
		ret = ocelot_port_obj_add_mdb(dev, SWITCHDEV_OBJ_PORT_MDB(obj),
					      trans);
		break;
	default:
		return -EOPNOTSUPP;
	}

	return ret;
}

static int ocelot_port_obj_del(struct net_device *dev,
			       const struct switchdev_obj *obj)
{
	int ret = 0;

	switch (obj->id) {
	case SWITCHDEV_OBJ_ID_PORT_VLAN:
		ret = ocelot_port_vlan_del_vlan(dev,
						SWITCHDEV_OBJ_PORT_VLAN(obj));
		break;
	case SWITCHDEV_OBJ_ID_PORT_MDB:
		ret = ocelot_port_obj_del_mdb(dev, SWITCHDEV_OBJ_PORT_MDB(obj));
		break;
	default:
		return -EOPNOTSUPP;
	}

	return ret;
}

static const struct switchdev_ops ocelot_port_switchdev_ops = {
	.switchdev_port_attr_get	= ocelot_port_attr_get,
	.switchdev_port_attr_set	= ocelot_port_attr_set,
	.switchdev_port_obj_add		= ocelot_port_obj_add,
	.switchdev_port_obj_del		= ocelot_port_obj_del,
};

static int ocelot_port_bridge_join(struct ocelot_port *ocelot_port,
				   struct net_device *bridge)
{
	struct ocelot *ocelot = ocelot_port->ocelot;

	if (!ocelot->bridge_mask) {
		ocelot->hw_bridge_dev = bridge;
	} else {
		if (ocelot->hw_bridge_dev != bridge)
			/* This is adding the port to a second bridge, this is
			 * unsupported */
			return -ENODEV;
	}

	ocelot->bridge_mask |= BIT(ocelot_port->chip_port);

	return 0;
}

static void ocelot_port_bridge_leave(struct ocelot_port *ocelot_port,
				     struct net_device *bridge)
{
	struct ocelot *ocelot = ocelot_port->ocelot;

	ocelot->bridge_mask &= ~BIT(ocelot_port->chip_port);

	if (!ocelot->bridge_mask)
		ocelot->hw_bridge_dev = NULL;

	/* Clear bridge vlan settings before calling ocelot_vlan_port_apply */
	ocelot_port->vlan_aware = 0;
	ocelot_port->pvid = 0;
	ocelot_port->vid = 0;
}

static void ocelot_set_aggr_pgids(struct ocelot *ocelot)
{
	int i, port, lag;

	/* Reset destination and aggregation PGIDS */
	for (port = 0; port < ocelot->num_phys_ports; port++)
		ocelot_write_rix(ocelot, BIT(port), ANA_PGID_PGID, port);

	for (i = PGID_AGGR; i < PGID_SRC; i++)
		ocelot_write_rix(ocelot, GENMASK(ocelot->num_phys_ports - 1, 0),
				 ANA_PGID_PGID, i);

	/* Now, set PGIDs for each LAG */
	for (lag = 0; lag < ocelot->num_phys_ports; lag++) {
		unsigned long bond_mask;
		int aggr_count = 0;
		u8 aggr_idx[16];

		bond_mask = ocelot->lags[lag];
		if (!bond_mask)
			continue;

		for_each_set_bit(port, &bond_mask, ocelot->num_phys_ports) {
			// Destination mask
			ocelot_write_rix(ocelot, bond_mask,
					 ANA_PGID_PGID, port);
			aggr_idx[aggr_count] = port;
			aggr_count++;
		}

		for (i = PGID_AGGR; i < PGID_SRC; i++) {
=======
	/* We have a PGID with fewer ports now */
	pgid = ocelot_mdb_get_pgid(ocelot, mc);
	if (IS_ERR(pgid))
		return PTR_ERR(pgid);
	mc->pgid = pgid;

	ocelot_encode_ports_to_mdb(addr, mc);

	if (mc->entry_type != ENTRYTYPE_MACv4 &&
	    mc->entry_type != ENTRYTYPE_MACv6)
		ocelot_write_rix(ocelot, pgid->ports, ANA_PGID_PGID,
				 pgid->index);

	return ocelot_mact_learn(ocelot, pgid->index, addr, vid,
				 mc->entry_type);
}
EXPORT_SYMBOL(ocelot_port_mdb_del);

void ocelot_port_bridge_join(struct ocelot *ocelot, int port,
			     struct net_device *bridge)
{
	struct ocelot_port *ocelot_port = ocelot->ports[port];

	ocelot_port->bridge = bridge;

	ocelot_apply_bridge_fwd_mask(ocelot);
}
EXPORT_SYMBOL(ocelot_port_bridge_join);

void ocelot_port_bridge_leave(struct ocelot *ocelot, int port,
			      struct net_device *bridge)
{
	struct ocelot_port *ocelot_port = ocelot->ports[port];
	struct ocelot_vlan pvid = {0}, native_vlan = {0};

	ocelot_port->bridge = NULL;

	ocelot_port_set_pvid(ocelot, port, pvid);
	ocelot_port_set_native_vlan(ocelot, port, native_vlan);
	ocelot_apply_bridge_fwd_mask(ocelot);
}
EXPORT_SYMBOL(ocelot_port_bridge_leave);

static void ocelot_set_aggr_pgids(struct ocelot *ocelot)
{
	unsigned long visited = GENMASK(ocelot->num_phys_ports - 1, 0);
	int i, port, lag;

	/* Reset destination and aggregation PGIDS */
	for_each_unicast_dest_pgid(ocelot, port)
		ocelot_write_rix(ocelot, BIT(port), ANA_PGID_PGID, port);

	for_each_aggr_pgid(ocelot, i)
		ocelot_write_rix(ocelot, GENMASK(ocelot->num_phys_ports - 1, 0),
				 ANA_PGID_PGID, i);

	/* The visited ports bitmask holds the list of ports offloading any
	 * bonding interface. Initially we mark all these ports as unvisited,
	 * then every time we visit a port in this bitmask, we know that it is
	 * the lowest numbered port, i.e. the one whose logical ID == physical
	 * port ID == LAG ID. So we mark as visited all further ports in the
	 * bitmask that are offloading the same bonding interface. This way,
	 * we set up the aggregation PGIDs only once per bonding interface.
	 */
	for (port = 0; port < ocelot->num_phys_ports; port++) {
		struct ocelot_port *ocelot_port = ocelot->ports[port];

		if (!ocelot_port || !ocelot_port->bond)
			continue;

		visited &= ~BIT(port);
	}

	/* Now, set PGIDs for each active LAG */
	for (lag = 0; lag < ocelot->num_phys_ports; lag++) {
		struct net_device *bond = ocelot->ports[lag]->bond;
		int num_active_ports = 0;
		unsigned long bond_mask;
		u8 aggr_idx[16];

		if (!bond || (visited & BIT(lag)))
			continue;

		bond_mask = ocelot_get_bond_mask(ocelot, bond);

		for_each_set_bit(port, &bond_mask, ocelot->num_phys_ports) {
			struct ocelot_port *ocelot_port = ocelot->ports[port];

			// Destination mask
			ocelot_write_rix(ocelot, bond_mask,
					 ANA_PGID_PGID, port);

			if (ocelot_port->lag_tx_active)
				aggr_idx[num_active_ports++] = port;
		}

		for_each_aggr_pgid(ocelot, i) {
>>>>>>> upstream/android-13
			u32 ac;

			ac = ocelot_read_rix(ocelot, ANA_PGID_PGID, i);
			ac &= ~bond_mask;
<<<<<<< HEAD
			ac |= BIT(aggr_idx[i % aggr_count]);
			ocelot_write_rix(ocelot, ac, ANA_PGID_PGID, i);
		}
	}
}

static void ocelot_setup_lag(struct ocelot *ocelot, int lag)
{
	unsigned long bond_mask = ocelot->lags[lag];
	unsigned int p;

	for_each_set_bit(p, &bond_mask, ocelot->num_phys_ports) {
		u32 port_cfg = ocelot_read_gix(ocelot, ANA_PORT_PORT_CFG, p);

		port_cfg &= ~ANA_PORT_PORT_CFG_PORTID_VAL_M;

		/* Use lag port as logical port for port i */
		ocelot_write_gix(ocelot, port_cfg |
				 ANA_PORT_PORT_CFG_PORTID_VAL(lag),
				 ANA_PORT_PORT_CFG, p);
	}
}

static int ocelot_port_lag_join(struct ocelot_port *ocelot_port,
				struct net_device *bond)
{
	struct ocelot *ocelot = ocelot_port->ocelot;
	int p = ocelot_port->chip_port;
	int lag, lp;
	struct net_device *ndev;
	u32 bond_mask = 0;

	rcu_read_lock();
	for_each_netdev_in_bond_rcu(bond, ndev) {
		struct ocelot_port *port = netdev_priv(ndev);

		bond_mask |= BIT(port->chip_port);
	}
	rcu_read_unlock();

	lp = __ffs(bond_mask);

	/* If the new port is the lowest one, use it as the logical port from
	 * now on
	 */
	if (p == lp) {
		lag = p;
		ocelot->lags[p] = bond_mask;
		bond_mask &= ~BIT(p);
		if (bond_mask) {
			lp = __ffs(bond_mask);
			ocelot->lags[lp] = 0;
		}
	} else {
		lag = lp;
		ocelot->lags[lp] |= BIT(p);
	}

	ocelot_setup_lag(ocelot, lag);
=======
			/* Don't do division by zero if there was no active
			 * port. Just make all aggregation codes zero.
			 */
			if (num_active_ports)
				ac |= BIT(aggr_idx[i % num_active_ports]);
			ocelot_write_rix(ocelot, ac, ANA_PGID_PGID, i);
		}

		/* Mark all ports in the same LAG as visited to avoid applying
		 * the same config again.
		 */
		for (port = lag; port < ocelot->num_phys_ports; port++) {
			struct ocelot_port *ocelot_port = ocelot->ports[port];

			if (!ocelot_port)
				continue;

			if (ocelot_port->bond == bond)
				visited |= BIT(port);
		}
	}
}

/* When offloading a bonding interface, the switch ports configured under the
 * same bond must have the same logical port ID, equal to the physical port ID
 * of the lowest numbered physical port in that bond. Otherwise, in standalone/
 * bridged mode, each port has a logical port ID equal to its physical port ID.
 */
static void ocelot_setup_logical_port_ids(struct ocelot *ocelot)
{
	int port;

	for (port = 0; port < ocelot->num_phys_ports; port++) {
		struct ocelot_port *ocelot_port = ocelot->ports[port];
		struct net_device *bond;

		if (!ocelot_port)
			continue;

		bond = ocelot_port->bond;
		if (bond) {
			int lag = __ffs(ocelot_get_bond_mask(ocelot, bond));

			ocelot_rmw_gix(ocelot,
				       ANA_PORT_PORT_CFG_PORTID_VAL(lag),
				       ANA_PORT_PORT_CFG_PORTID_VAL_M,
				       ANA_PORT_PORT_CFG, port);
		} else {
			ocelot_rmw_gix(ocelot,
				       ANA_PORT_PORT_CFG_PORTID_VAL(port),
				       ANA_PORT_PORT_CFG_PORTID_VAL_M,
				       ANA_PORT_PORT_CFG, port);
		}
	}
}

int ocelot_port_lag_join(struct ocelot *ocelot, int port,
			 struct net_device *bond,
			 struct netdev_lag_upper_info *info)
{
	if (info->tx_type != NETDEV_LAG_TX_TYPE_HASH)
		return -EOPNOTSUPP;

	ocelot->ports[port]->bond = bond;

	ocelot_setup_logical_port_ids(ocelot);
	ocelot_apply_bridge_fwd_mask(ocelot);
>>>>>>> upstream/android-13
	ocelot_set_aggr_pgids(ocelot);

	return 0;
}
<<<<<<< HEAD

static void ocelot_port_lag_leave(struct ocelot_port *ocelot_port,
				  struct net_device *bond)
{
	struct ocelot *ocelot = ocelot_port->ocelot;
	int p = ocelot_port->chip_port;
	u32 port_cfg;
	int i;

	/* Remove port from any lag */
	for (i = 0; i < ocelot->num_phys_ports; i++)
		ocelot->lags[i] &= ~BIT(ocelot_port->chip_port);

	/* if it was the logical port of the lag, move the lag config to the
	 * next port
	 */
	if (ocelot->lags[p]) {
		int n = __ffs(ocelot->lags[p]);

		ocelot->lags[n] = ocelot->lags[p];
		ocelot->lags[p] = 0;

		ocelot_setup_lag(ocelot, n);
	}

	port_cfg = ocelot_read_gix(ocelot, ANA_PORT_PORT_CFG, p);
	port_cfg &= ~ANA_PORT_PORT_CFG_PORTID_VAL_M;
	ocelot_write_gix(ocelot, port_cfg | ANA_PORT_PORT_CFG_PORTID_VAL(p),
			 ANA_PORT_PORT_CFG, p);

	ocelot_set_aggr_pgids(ocelot);
}

/* Checks if the net_device instance given to us originate from our driver. */
static bool ocelot_netdevice_dev_check(const struct net_device *dev)
{
	return dev->netdev_ops == &ocelot_port_netdev_ops;
}

static int ocelot_netdevice_port_event(struct net_device *dev,
				       unsigned long event,
				       struct netdev_notifier_changeupper_info *info)
{
	struct ocelot_port *ocelot_port = netdev_priv(dev);
	int err = 0;

	switch (event) {
	case NETDEV_CHANGEUPPER:
		if (netif_is_bridge_master(info->upper_dev)) {
			if (info->linking)
				err = ocelot_port_bridge_join(ocelot_port,
							      info->upper_dev);
			else
				ocelot_port_bridge_leave(ocelot_port,
							 info->upper_dev);

			ocelot_vlan_port_apply(ocelot_port->ocelot,
					       ocelot_port);
		}
		if (netif_is_lag_master(info->upper_dev)) {
			if (info->linking)
				err = ocelot_port_lag_join(ocelot_port,
							   info->upper_dev);
			else
				ocelot_port_lag_leave(ocelot_port,
						      info->upper_dev);
		}
		break;
	default:
		break;
	}

	return err;
}

static int ocelot_netdevice_event(struct notifier_block *unused,
				  unsigned long event, void *ptr)
{
	struct netdev_notifier_changeupper_info *info = ptr;
	struct net_device *dev = netdev_notifier_info_to_dev(ptr);
	int ret = 0;

	if (event == NETDEV_PRECHANGEUPPER &&
	    ocelot_netdevice_dev_check(dev) &&
	    netif_is_lag_master(info->upper_dev)) {
		struct netdev_lag_upper_info *lag_upper_info = info->upper_info;
		struct netlink_ext_ack *extack;

		if (lag_upper_info &&
		    lag_upper_info->tx_type != NETDEV_LAG_TX_TYPE_HASH) {
			extack = netdev_notifier_info_to_extack(&info->info);
			NL_SET_ERR_MSG_MOD(extack, "LAG device using unsupported Tx type");

			ret = -EINVAL;
			goto notify;
		}
	}

	if (netif_is_lag_master(dev)) {
		struct net_device *slave;
		struct list_head *iter;

		netdev_for_each_lower_dev(dev, slave, iter) {
			ret = ocelot_netdevice_port_event(slave, event, info);
			if (ret)
				goto notify;
		}
	} else {
		ret = ocelot_netdevice_port_event(dev, event, info);
	}

notify:
	return notifier_from_errno(ret);
}

struct notifier_block ocelot_netdevice_nb __read_mostly = {
	.notifier_call = ocelot_netdevice_event,
};
EXPORT_SYMBOL(ocelot_netdevice_nb);

int ocelot_probe_port(struct ocelot *ocelot, u8 port,
		      void __iomem *regs,
		      struct phy_device *phy)
{
	struct ocelot_port *ocelot_port;
	struct net_device *dev;
	int err;

	dev = alloc_etherdev(sizeof(struct ocelot_port));
	if (!dev)
		return -ENOMEM;
	SET_NETDEV_DEV(dev, ocelot->dev);
	ocelot_port = netdev_priv(dev);
	ocelot_port->dev = dev;
	ocelot_port->ocelot = ocelot;
	ocelot_port->regs = regs;
	ocelot_port->chip_port = port;
	ocelot_port->phy = phy;
	INIT_LIST_HEAD(&ocelot_port->mc);
	ocelot->ports[port] = ocelot_port;

	dev->netdev_ops = &ocelot_port_netdev_ops;
	dev->ethtool_ops = &ocelot_ethtool_ops;
	dev->switchdev_ops = &ocelot_port_switchdev_ops;

	dev->hw_features |= NETIF_F_HW_VLAN_CTAG_FILTER;
	dev->features |= NETIF_F_HW_VLAN_CTAG_FILTER;

	memcpy(dev->dev_addr, ocelot->base_mac, ETH_ALEN);
	dev->dev_addr[ETH_ALEN - 1] += port;
	ocelot_mact_learn(ocelot, PGID_CPU, dev->dev_addr, ocelot_port->pvid,
			  ENTRYTYPE_LOCKED);

	err = register_netdev(dev);
	if (err) {
		dev_err(ocelot->dev, "register_netdev failed\n");
		goto err_register_netdev;
	}

	/* Basic L2 initialization */
	ocelot_vlan_port_apply(ocelot, ocelot_port);

	return 0;

err_register_netdev:
	free_netdev(dev);
	return err;
}
EXPORT_SYMBOL(ocelot_probe_port);

int ocelot_init(struct ocelot *ocelot)
{
	u32 port;
	int i, cpu = ocelot->num_phys_ports;
	char queue_name[32];

	ocelot->lags = devm_kcalloc(ocelot->dev, ocelot->num_phys_ports,
				    sizeof(u32), GFP_KERNEL);
	if (!ocelot->lags)
		return -ENOMEM;
=======
EXPORT_SYMBOL(ocelot_port_lag_join);

void ocelot_port_lag_leave(struct ocelot *ocelot, int port,
			   struct net_device *bond)
{
	ocelot->ports[port]->bond = NULL;

	ocelot_setup_logical_port_ids(ocelot);
	ocelot_apply_bridge_fwd_mask(ocelot);
	ocelot_set_aggr_pgids(ocelot);
}
EXPORT_SYMBOL(ocelot_port_lag_leave);

void ocelot_port_lag_change(struct ocelot *ocelot, int port, bool lag_tx_active)
{
	struct ocelot_port *ocelot_port = ocelot->ports[port];

	ocelot_port->lag_tx_active = lag_tx_active;

	/* Rebalance the LAGs */
	ocelot_set_aggr_pgids(ocelot);
}
EXPORT_SYMBOL(ocelot_port_lag_change);

/* Configure the maximum SDU (L2 payload) on RX to the value specified in @sdu.
 * The length of VLAN tags is accounted for automatically via DEV_MAC_TAGS_CFG.
 * In the special case that it's the NPI port that we're configuring, the
 * length of the tag and optional prefix needs to be accounted for privately,
 * in order to be able to sustain communication at the requested @sdu.
 */
void ocelot_port_set_maxlen(struct ocelot *ocelot, int port, size_t sdu)
{
	struct ocelot_port *ocelot_port = ocelot->ports[port];
	int maxlen = sdu + ETH_HLEN + ETH_FCS_LEN;
	int pause_start, pause_stop;
	int atop, atop_tot;

	if (port == ocelot->npi) {
		maxlen += OCELOT_TAG_LEN;

		if (ocelot->npi_inj_prefix == OCELOT_TAG_PREFIX_SHORT)
			maxlen += OCELOT_SHORT_PREFIX_LEN;
		else if (ocelot->npi_inj_prefix == OCELOT_TAG_PREFIX_LONG)
			maxlen += OCELOT_LONG_PREFIX_LEN;
	}

	ocelot_port_writel(ocelot_port, maxlen, DEV_MAC_MAXLEN_CFG);

	/* Set Pause watermark hysteresis */
	pause_start = 6 * maxlen / OCELOT_BUFFER_CELL_SZ;
	pause_stop = 4 * maxlen / OCELOT_BUFFER_CELL_SZ;
	ocelot_fields_write(ocelot, port, SYS_PAUSE_CFG_PAUSE_START,
			    pause_start);
	ocelot_fields_write(ocelot, port, SYS_PAUSE_CFG_PAUSE_STOP,
			    pause_stop);

	/* Tail dropping watermarks */
	atop_tot = (ocelot->packet_buffer_size - 9 * maxlen) /
		   OCELOT_BUFFER_CELL_SZ;
	atop = (9 * maxlen) / OCELOT_BUFFER_CELL_SZ;
	ocelot_write_rix(ocelot, ocelot->ops->wm_enc(atop), SYS_ATOP, port);
	ocelot_write(ocelot, ocelot->ops->wm_enc(atop_tot), SYS_ATOP_TOT_CFG);
}
EXPORT_SYMBOL(ocelot_port_set_maxlen);

int ocelot_get_max_mtu(struct ocelot *ocelot, int port)
{
	int max_mtu = 65535 - ETH_HLEN - ETH_FCS_LEN;

	if (port == ocelot->npi) {
		max_mtu -= OCELOT_TAG_LEN;

		if (ocelot->npi_inj_prefix == OCELOT_TAG_PREFIX_SHORT)
			max_mtu -= OCELOT_SHORT_PREFIX_LEN;
		else if (ocelot->npi_inj_prefix == OCELOT_TAG_PREFIX_LONG)
			max_mtu -= OCELOT_LONG_PREFIX_LEN;
	}

	return max_mtu;
}
EXPORT_SYMBOL(ocelot_get_max_mtu);

static void ocelot_port_set_learning(struct ocelot *ocelot, int port,
				     bool enabled)
{
	struct ocelot_port *ocelot_port = ocelot->ports[port];
	u32 val = 0;

	if (enabled)
		val = ANA_PORT_PORT_CFG_LEARN_ENA;

	ocelot_rmw_gix(ocelot, val, ANA_PORT_PORT_CFG_LEARN_ENA,
		       ANA_PORT_PORT_CFG, port);

	ocelot_port->learn_ena = enabled;
}

static void ocelot_port_set_ucast_flood(struct ocelot *ocelot, int port,
					bool enabled)
{
	u32 val = 0;

	if (enabled)
		val = BIT(port);

	ocelot_rmw_rix(ocelot, val, BIT(port), ANA_PGID_PGID, PGID_UC);
}

static void ocelot_port_set_mcast_flood(struct ocelot *ocelot, int port,
					bool enabled)
{
	u32 val = 0;

	if (enabled)
		val = BIT(port);

	ocelot_rmw_rix(ocelot, val, BIT(port), ANA_PGID_PGID, PGID_MC);
	ocelot_rmw_rix(ocelot, val, BIT(port), ANA_PGID_PGID, PGID_MCIPV4);
	ocelot_rmw_rix(ocelot, val, BIT(port), ANA_PGID_PGID, PGID_MCIPV6);
}

static void ocelot_port_set_bcast_flood(struct ocelot *ocelot, int port,
					bool enabled)
{
	u32 val = 0;

	if (enabled)
		val = BIT(port);

	ocelot_rmw_rix(ocelot, val, BIT(port), ANA_PGID_PGID, PGID_BC);
}

int ocelot_port_pre_bridge_flags(struct ocelot *ocelot, int port,
				 struct switchdev_brport_flags flags)
{
	if (flags.mask & ~(BR_LEARNING | BR_FLOOD | BR_MCAST_FLOOD |
			   BR_BCAST_FLOOD))
		return -EINVAL;

	return 0;
}
EXPORT_SYMBOL(ocelot_port_pre_bridge_flags);

void ocelot_port_bridge_flags(struct ocelot *ocelot, int port,
			      struct switchdev_brport_flags flags)
{
	if (flags.mask & BR_LEARNING)
		ocelot_port_set_learning(ocelot, port,
					 !!(flags.val & BR_LEARNING));

	if (flags.mask & BR_FLOOD)
		ocelot_port_set_ucast_flood(ocelot, port,
					    !!(flags.val & BR_FLOOD));

	if (flags.mask & BR_MCAST_FLOOD)
		ocelot_port_set_mcast_flood(ocelot, port,
					    !!(flags.val & BR_MCAST_FLOOD));

	if (flags.mask & BR_BCAST_FLOOD)
		ocelot_port_set_bcast_flood(ocelot, port,
					    !!(flags.val & BR_BCAST_FLOOD));
}
EXPORT_SYMBOL(ocelot_port_bridge_flags);

void ocelot_init_port(struct ocelot *ocelot, int port)
{
	struct ocelot_port *ocelot_port = ocelot->ports[port];

	skb_queue_head_init(&ocelot_port->tx_skbs);

	/* Basic L2 initialization */

	/* Set MAC IFG Gaps
	 * FDX: TX_IFG = 5, RX_IFG1 = RX_IFG2 = 0
	 * !FDX: TX_IFG = 5, RX_IFG1 = RX_IFG2 = 5
	 */
	ocelot_port_writel(ocelot_port, DEV_MAC_IFG_CFG_TX_IFG(5),
			   DEV_MAC_IFG_CFG);

	/* Load seed (0) and set MAC HDX late collision  */
	ocelot_port_writel(ocelot_port, DEV_MAC_HDX_CFG_LATE_COL_POS(67) |
			   DEV_MAC_HDX_CFG_SEED_LOAD,
			   DEV_MAC_HDX_CFG);
	mdelay(1);
	ocelot_port_writel(ocelot_port, DEV_MAC_HDX_CFG_LATE_COL_POS(67),
			   DEV_MAC_HDX_CFG);

	/* Set Max Length and maximum tags allowed */
	ocelot_port_set_maxlen(ocelot, port, ETH_DATA_LEN);
	ocelot_port_writel(ocelot_port, DEV_MAC_TAGS_CFG_TAG_ID(ETH_P_8021AD) |
			   DEV_MAC_TAGS_CFG_VLAN_AWR_ENA |
			   DEV_MAC_TAGS_CFG_VLAN_DBL_AWR_ENA |
			   DEV_MAC_TAGS_CFG_VLAN_LEN_AWR_ENA,
			   DEV_MAC_TAGS_CFG);

	/* Set SMAC of Pause frame (00:00:00:00:00:00) */
	ocelot_port_writel(ocelot_port, 0, DEV_MAC_FC_MAC_HIGH_CFG);
	ocelot_port_writel(ocelot_port, 0, DEV_MAC_FC_MAC_LOW_CFG);

	/* Enable transmission of pause frames */
	ocelot_fields_write(ocelot, port, SYS_PAUSE_CFG_PAUSE_ENA, 1);

	/* Drop frames with multicast source address */
	ocelot_rmw_gix(ocelot, ANA_PORT_DROP_CFG_DROP_MC_SMAC_ENA,
		       ANA_PORT_DROP_CFG_DROP_MC_SMAC_ENA,
		       ANA_PORT_DROP_CFG, port);

	/* Set default VLAN and tag type to 8021Q. */
	ocelot_rmw_gix(ocelot, REW_PORT_VLAN_CFG_PORT_TPID(ETH_P_8021Q),
		       REW_PORT_VLAN_CFG_PORT_TPID_M,
		       REW_PORT_VLAN_CFG, port);

	/* Disable source address learning for standalone mode */
	ocelot_port_set_learning(ocelot, port, false);

	/* Set the port's initial logical port ID value, enable receiving
	 * frames on it, and configure the MAC address learning type to
	 * automatic.
	 */
	ocelot_write_gix(ocelot, ANA_PORT_PORT_CFG_LEARNAUTO |
			 ANA_PORT_PORT_CFG_RECV_ENA |
			 ANA_PORT_PORT_CFG_PORTID_VAL(port),
			 ANA_PORT_PORT_CFG, port);

	/* Enable vcap lookups */
	ocelot_vcap_enable(ocelot, port);
}
EXPORT_SYMBOL(ocelot_init_port);

/* Configure and enable the CPU port module, which is a set of queues
 * accessible through register MMIO, frame DMA or Ethernet (in case
 * NPI mode is used).
 */
static void ocelot_cpu_port_init(struct ocelot *ocelot)
{
	int cpu = ocelot->num_phys_ports;

	/* The unicast destination PGID for the CPU port module is unused */
	ocelot_write_rix(ocelot, 0, ANA_PGID_PGID, cpu);
	/* Instead set up a multicast destination PGID for traffic copied to
	 * the CPU. Whitelisted MAC addresses like the port netdevice MAC
	 * addresses will be copied to the CPU via this PGID.
	 */
	ocelot_write_rix(ocelot, BIT(cpu), ANA_PGID_PGID, PGID_CPU);
	ocelot_write_gix(ocelot, ANA_PORT_PORT_CFG_RECV_ENA |
			 ANA_PORT_PORT_CFG_PORTID_VAL(cpu),
			 ANA_PORT_PORT_CFG, cpu);

	/* Enable CPU port module */
	ocelot_fields_write(ocelot, cpu, QSYS_SWITCH_PORT_MODE_PORT_ENA, 1);
	/* CPU port Injection/Extraction configuration */
	ocelot_fields_write(ocelot, cpu, SYS_PORT_MODE_INCL_XTR_HDR,
			    OCELOT_TAG_PREFIX_NONE);
	ocelot_fields_write(ocelot, cpu, SYS_PORT_MODE_INCL_INJ_HDR,
			    OCELOT_TAG_PREFIX_NONE);

	/* Configure the CPU port to be VLAN aware */
	ocelot_write_gix(ocelot, ANA_PORT_VLAN_CFG_VLAN_VID(0) |
				 ANA_PORT_VLAN_CFG_VLAN_AWARE_ENA |
				 ANA_PORT_VLAN_CFG_VLAN_POP_CNT(1),
			 ANA_PORT_VLAN_CFG, cpu);
}

static void ocelot_detect_features(struct ocelot *ocelot)
{
	int mmgt, eq_ctrl;

	/* For Ocelot, Felix, Seville, Serval etc, SYS:MMGT:MMGT:FREECNT holds
	 * the number of 240-byte free memory words (aka 4-cell chunks) and not
	 * 192 bytes as the documentation incorrectly says.
	 */
	mmgt = ocelot_read(ocelot, SYS_MMGT);
	ocelot->packet_buffer_size = 240 * SYS_MMGT_FREECNT(mmgt);

	eq_ctrl = ocelot_read(ocelot, QSYS_EQ_CTRL);
	ocelot->num_frame_refs = QSYS_MMGT_EQ_CTRL_FP_FREE_CNT(eq_ctrl);
}

int ocelot_init(struct ocelot *ocelot)
{
	char queue_name[32];
	int i, ret;
	u32 port;

	if (ocelot->ops->reset) {
		ret = ocelot->ops->reset(ocelot);
		if (ret) {
			dev_err(ocelot->dev, "Switch reset failed\n");
			return ret;
		}
	}
>>>>>>> upstream/android-13

	ocelot->stats = devm_kcalloc(ocelot->dev,
				     ocelot->num_phys_ports * ocelot->num_stats,
				     sizeof(u64), GFP_KERNEL);
	if (!ocelot->stats)
		return -ENOMEM;

	mutex_init(&ocelot->stats_lock);
<<<<<<< HEAD
=======
	mutex_init(&ocelot->ptp_lock);
	spin_lock_init(&ocelot->ptp_clock_lock);
	spin_lock_init(&ocelot->ts_id_lock);
>>>>>>> upstream/android-13
	snprintf(queue_name, sizeof(queue_name), "%s-stats",
		 dev_name(ocelot->dev));
	ocelot->stats_queue = create_singlethread_workqueue(queue_name);
	if (!ocelot->stats_queue)
		return -ENOMEM;

<<<<<<< HEAD
	ocelot_mact_init(ocelot);
	ocelot_vlan_init(ocelot);
=======
	ocelot->owq = alloc_ordered_workqueue("ocelot-owq", 0);
	if (!ocelot->owq) {
		destroy_workqueue(ocelot->stats_queue);
		return -ENOMEM;
	}

	INIT_LIST_HEAD(&ocelot->multicast);
	INIT_LIST_HEAD(&ocelot->pgids);
	ocelot_detect_features(ocelot);
	ocelot_mact_init(ocelot);
	ocelot_vlan_init(ocelot);
	ocelot_vcap_init(ocelot);
	ocelot_cpu_port_init(ocelot);
>>>>>>> upstream/android-13

	for (port = 0; port < ocelot->num_phys_ports; port++) {
		/* Clear all counters (5 groups) */
		ocelot_write(ocelot, SYS_STAT_CFG_STAT_VIEW(port) |
				     SYS_STAT_CFG_STAT_CLEAR_SHOT(0x7f),
			     SYS_STAT_CFG);
	}

	/* Only use S-Tag */
	ocelot_write(ocelot, ETH_P_8021AD, SYS_VLAN_ETYPE_CFG);

	/* Aggregation mode */
	ocelot_write(ocelot, ANA_AGGR_CFG_AC_SMAC_ENA |
			     ANA_AGGR_CFG_AC_DMAC_ENA |
			     ANA_AGGR_CFG_AC_IP4_SIPDIP_ENA |
<<<<<<< HEAD
			     ANA_AGGR_CFG_AC_IP4_TCPUDP_ENA, ANA_AGGR_CFG);
=======
			     ANA_AGGR_CFG_AC_IP4_TCPUDP_ENA |
			     ANA_AGGR_CFG_AC_IP6_FLOW_LBL_ENA |
			     ANA_AGGR_CFG_AC_IP6_TCPUDP_ENA,
			     ANA_AGGR_CFG);
>>>>>>> upstream/android-13

	/* Set MAC age time to default value. The entry is aged after
	 * 2*AGE_PERIOD
	 */
	ocelot_write(ocelot,
		     ANA_AUTOAGE_AGE_PERIOD(BR_DEFAULT_AGEING_TIME / 2 / HZ),
		     ANA_AUTOAGE);

	/* Disable learning for frames discarded by VLAN ingress filtering */
	regmap_field_write(ocelot->regfields[ANA_ADVLEARN_VLAN_CHK], 1);

	/* Setup frame ageing - fixed value "2 sec" - in 6.5 us units */
	ocelot_write(ocelot, SYS_FRM_AGING_AGE_TX_ENA |
		     SYS_FRM_AGING_MAX_AGE(307692), SYS_FRM_AGING);

	/* Setup flooding PGIDs */
<<<<<<< HEAD
	ocelot_write_rix(ocelot, ANA_FLOODING_FLD_MULTICAST(PGID_MC) |
			 ANA_FLOODING_FLD_BROADCAST(PGID_MC) |
			 ANA_FLOODING_FLD_UNICAST(PGID_UC),
			 ANA_FLOODING, 0);
=======
	for (i = 0; i < ocelot->num_flooding_pgids; i++)
		ocelot_write_rix(ocelot, ANA_FLOODING_FLD_MULTICAST(PGID_MC) |
				 ANA_FLOODING_FLD_BROADCAST(PGID_BC) |
				 ANA_FLOODING_FLD_UNICAST(PGID_UC),
				 ANA_FLOODING, i);
>>>>>>> upstream/android-13
	ocelot_write(ocelot, ANA_FLOODING_IPMC_FLD_MC6_DATA(PGID_MCIPV6) |
		     ANA_FLOODING_IPMC_FLD_MC6_CTRL(PGID_MC) |
		     ANA_FLOODING_IPMC_FLD_MC4_DATA(PGID_MCIPV4) |
		     ANA_FLOODING_IPMC_FLD_MC4_CTRL(PGID_MC),
		     ANA_FLOODING_IPMC);

	for (port = 0; port < ocelot->num_phys_ports; port++) {
		/* Transmit the frame to the local port. */
		ocelot_write_rix(ocelot, BIT(port), ANA_PGID_PGID, port);
		/* Do not forward BPDU frames to the front ports. */
		ocelot_write_gix(ocelot,
				 ANA_PORT_CPU_FWD_BPDU_CFG_BPDU_REDIR_ENA(0xffff),
				 ANA_PORT_CPU_FWD_BPDU_CFG,
				 port);
		/* Ensure bridging is disabled */
		ocelot_write_rix(ocelot, 0, ANA_PGID_PGID, PGID_SRC + port);
	}

<<<<<<< HEAD
	/* Configure and enable the CPU port. */
	ocelot_write_rix(ocelot, 0, ANA_PGID_PGID, cpu);
	ocelot_write_rix(ocelot, BIT(cpu), ANA_PGID_PGID, PGID_CPU);
	ocelot_write_gix(ocelot, ANA_PORT_PORT_CFG_RECV_ENA |
			 ANA_PORT_PORT_CFG_PORTID_VAL(cpu),
			 ANA_PORT_PORT_CFG, cpu);

	/* Allow broadcast MAC frames. */
	for (i = ocelot->num_phys_ports + 1; i < PGID_CPU; i++) {
=======
	for_each_nonreserved_multicast_dest_pgid(ocelot, i) {
>>>>>>> upstream/android-13
		u32 val = ANA_PGID_PGID_PGID(GENMASK(ocelot->num_phys_ports - 1, 0));

		ocelot_write_rix(ocelot, val, ANA_PGID_PGID, i);
	}
<<<<<<< HEAD
	ocelot_write_rix(ocelot,
			 ANA_PGID_PGID_PGID(GENMASK(ocelot->num_phys_ports, 0)),
			 ANA_PGID_PGID, PGID_MC);
	ocelot_write_rix(ocelot, 0, ANA_PGID_PGID, PGID_MCIPV4);
	ocelot_write_rix(ocelot, 0, ANA_PGID_PGID, PGID_MCIPV6);

	/* CPU port Injection/Extraction configuration */
	ocelot_write_rix(ocelot, QSYS_SWITCH_PORT_MODE_INGRESS_DROP_MODE |
			 QSYS_SWITCH_PORT_MODE_SCH_NEXT_CFG(1) |
			 QSYS_SWITCH_PORT_MODE_PORT_ENA,
			 QSYS_SWITCH_PORT_MODE, cpu);
	ocelot_write_rix(ocelot, SYS_PORT_MODE_INCL_XTR_HDR(1) |
			 SYS_PORT_MODE_INCL_INJ_HDR(1), SYS_PORT_MODE, cpu);
=======

	ocelot_write_rix(ocelot, 0, ANA_PGID_PGID, PGID_BLACKHOLE);

	/* Allow broadcast and unknown L2 multicast to the CPU. */
	ocelot_rmw_rix(ocelot, ANA_PGID_PGID_PGID(BIT(ocelot->num_phys_ports)),
		       ANA_PGID_PGID_PGID(BIT(ocelot->num_phys_ports)),
		       ANA_PGID_PGID, PGID_MC);
	ocelot_rmw_rix(ocelot, ANA_PGID_PGID_PGID(BIT(ocelot->num_phys_ports)),
		       ANA_PGID_PGID_PGID(BIT(ocelot->num_phys_ports)),
		       ANA_PGID_PGID, PGID_BC);
	ocelot_write_rix(ocelot, 0, ANA_PGID_PGID, PGID_MCIPV4);
	ocelot_write_rix(ocelot, 0, ANA_PGID_PGID, PGID_MCIPV6);

>>>>>>> upstream/android-13
	/* Allow manual injection via DEVCPU_QS registers, and byte swap these
	 * registers endianness.
	 */
	ocelot_write_rix(ocelot, QS_INJ_GRP_CFG_BYTE_SWAP |
			 QS_INJ_GRP_CFG_MODE(1), QS_INJ_GRP_CFG, 0);
	ocelot_write_rix(ocelot, QS_XTR_GRP_CFG_BYTE_SWAP |
			 QS_XTR_GRP_CFG_MODE(1), QS_XTR_GRP_CFG, 0);
	ocelot_write(ocelot, ANA_CPUQ_CFG_CPUQ_MIRROR(2) |
		     ANA_CPUQ_CFG_CPUQ_LRN(2) |
		     ANA_CPUQ_CFG_CPUQ_MAC_COPY(2) |
		     ANA_CPUQ_CFG_CPUQ_SRC_COPY(2) |
		     ANA_CPUQ_CFG_CPUQ_LOCKED_PORTMOVE(2) |
		     ANA_CPUQ_CFG_CPUQ_ALLBRIDGE(6) |
		     ANA_CPUQ_CFG_CPUQ_IPMC_CTRL(6) |
		     ANA_CPUQ_CFG_CPUQ_IGMP(6) |
		     ANA_CPUQ_CFG_CPUQ_MLD(6), ANA_CPUQ_CFG);
	for (i = 0; i < 16; i++)
		ocelot_write_rix(ocelot, ANA_CPUQ_8021_CFG_CPUQ_GARP_VAL(6) |
				 ANA_CPUQ_8021_CFG_CPUQ_BPDU_VAL(6),
				 ANA_CPUQ_8021_CFG, i);

<<<<<<< HEAD
	INIT_DELAYED_WORK(&ocelot->stats_work, ocelot_check_stats);
	queue_delayed_work(ocelot->stats_queue, &ocelot->stats_work,
			   OCELOT_STATS_CHECK_DELAY);
=======
	INIT_DELAYED_WORK(&ocelot->stats_work, ocelot_check_stats_work);
	queue_delayed_work(ocelot->stats_queue, &ocelot->stats_work,
			   OCELOT_STATS_CHECK_DELAY);

>>>>>>> upstream/android-13
	return 0;
}
EXPORT_SYMBOL(ocelot_init);

void ocelot_deinit(struct ocelot *ocelot)
{
	cancel_delayed_work(&ocelot->stats_work);
	destroy_workqueue(ocelot->stats_queue);
<<<<<<< HEAD
=======
	destroy_workqueue(ocelot->owq);
>>>>>>> upstream/android-13
	mutex_destroy(&ocelot->stats_lock);
}
EXPORT_SYMBOL(ocelot_deinit);

<<<<<<< HEAD
=======
void ocelot_deinit_port(struct ocelot *ocelot, int port)
{
	struct ocelot_port *ocelot_port = ocelot->ports[port];

	skb_queue_purge(&ocelot_port->tx_skbs);
}
EXPORT_SYMBOL(ocelot_deinit_port);

>>>>>>> upstream/android-13
MODULE_LICENSE("Dual MIT/GPL");
