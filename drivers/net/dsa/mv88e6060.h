<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-or-later */
>>>>>>> upstream/android-13
/*
 * drivers/net/dsa/mv88e6060.h - Marvell 88e6060 switch chip support
 * Copyright (c) 2015 Neil Armstrong
 *
 * Based on mv88e6xxx.h
 * Copyright (c) 2008 Marvell Semiconductor
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
=======
>>>>>>> upstream/android-13
 */

#ifndef __MV88E6060_H
#define __MV88E6060_H

#define MV88E6060_PORTS	6

#define REG_PORT(p)		(0x8 + (p))
#define PORT_STATUS		0x00
#define PORT_STATUS_PAUSE_EN	BIT(15)
#define PORT_STATUS_MY_PAUSE	BIT(14)
#define PORT_STATUS_FC		(PORT_STATUS_MY_PAUSE | PORT_STATUS_PAUSE_EN)
#define PORT_STATUS_RESOLVED	BIT(13)
#define PORT_STATUS_LINK	BIT(12)
#define PORT_STATUS_PORTMODE	BIT(11)
#define PORT_STATUS_PHYMODE	BIT(10)
#define PORT_STATUS_DUPLEX	BIT(9)
#define PORT_STATUS_SPEED	BIT(8)
#define PORT_SWITCH_ID		0x03
#define PORT_SWITCH_ID_6060	0x0600
#define PORT_SWITCH_ID_6060_MASK	0xfff0
#define PORT_SWITCH_ID_6060_R1	0x0601
#define PORT_SWITCH_ID_6060_R2	0x0602
#define PORT_CONTROL		0x04
#define PORT_CONTROL_FORCE_FLOW_CTRL	BIT(15)
#define PORT_CONTROL_TRAILER	BIT(14)
#define PORT_CONTROL_HEADER	BIT(11)
#define PORT_CONTROL_INGRESS_MODE	BIT(8)
#define PORT_CONTROL_VLAN_TUNNEL	BIT(7)
#define PORT_CONTROL_STATE_MASK	0x03
#define PORT_CONTROL_STATE_DISABLED	0x00
#define PORT_CONTROL_STATE_BLOCKING	0x01
#define PORT_CONTROL_STATE_LEARNING	0x02
#define PORT_CONTROL_STATE_FORWARDING	0x03
#define PORT_VLAN_MAP		0x06
#define PORT_VLAN_MAP_DBNUM_SHIFT	12
#define PORT_VLAN_MAP_TABLE_MASK	0x1f
#define PORT_ASSOC_VECTOR	0x0b
#define PORT_ASSOC_VECTOR_MONITOR	BIT(15)
#define PORT_ASSOC_VECTOR_PAV_MASK	0x1f
#define PORT_RX_CNTR		0x10
#define PORT_TX_CNTR		0x11

#define REG_GLOBAL		0x0f
#define GLOBAL_STATUS		0x00
#define GLOBAL_STATUS_SW_MODE_MASK	(0x3 << 12)
#define GLOBAL_STATUS_SW_MODE_0	(0x0 << 12)
#define GLOBAL_STATUS_SW_MODE_1	(0x1 << 12)
#define GLOBAL_STATUS_SW_MODE_2	(0x2 << 12)
#define GLOBAL_STATUS_SW_MODE_3	(0x3 << 12)
#define GLOBAL_STATUS_INIT_READY	BIT(11)
#define GLOBAL_STATUS_ATU_FULL		BIT(3)
#define GLOBAL_STATUS_ATU_DONE		BIT(2)
#define GLOBAL_STATUS_PHY_INT	BIT(1)
#define GLOBAL_STATUS_EEINT	BIT(0)
#define GLOBAL_MAC_01		0x01
#define GLOBAL_MAC_01_DIFF_ADDR	BIT(8)
#define GLOBAL_MAC_23		0x02
#define GLOBAL_MAC_45		0x03
#define GLOBAL_CONTROL		0x04
#define GLOBAL_CONTROL_DISCARD_EXCESS	BIT(13)
#define GLOBAL_CONTROL_MAX_FRAME_1536	BIT(10)
#define GLOBAL_CONTROL_RELOAD_EEPROM	BIT(9)
#define GLOBAL_CONTROL_CTRMODE		BIT(8)
#define GLOBAL_CONTROL_ATU_FULL_EN	BIT(3)
#define GLOBAL_CONTROL_ATU_DONE_EN	BIT(2)
#define GLOBAL_CONTROL_PHYINT_EN	BIT(1)
#define GLOBAL_CONTROL_EEPROM_DONE_EN	BIT(0)
#define GLOBAL_ATU_CONTROL	0x0a
#define GLOBAL_ATU_CONTROL_SWRESET	BIT(15)
#define GLOBAL_ATU_CONTROL_LEARNDIS	BIT(14)
#define GLOBAL_ATU_CONTROL_ATUSIZE_256	(0x0 << 12)
#define GLOBAL_ATU_CONTROL_ATUSIZE_512	(0x1 << 12)
#define GLOBAL_ATU_CONTROL_ATUSIZE_1024	(0x2 << 12)
#define GLOBAL_ATU_CONTROL_ATE_AGE_SHIFT	4
#define GLOBAL_ATU_CONTROL_ATE_AGE_MASK	(0xff << 4)
#define GLOBAL_ATU_CONTROL_ATE_AGE_5MIN	(0x13 << 4)
#define GLOBAL_ATU_OP		0x0b
#define GLOBAL_ATU_OP_BUSY	BIT(15)
#define GLOBAL_ATU_OP_NOP		(0 << 12)
#define GLOBAL_ATU_OP_FLUSH_ALL	((1 << 12) | GLOBAL_ATU_OP_BUSY)
#define GLOBAL_ATU_OP_FLUSH_UNLOCKED	((2 << 12) | GLOBAL_ATU_OP_BUSY)
#define GLOBAL_ATU_OP_LOAD_DB		((3 << 12) | GLOBAL_ATU_OP_BUSY)
#define GLOBAL_ATU_OP_GET_NEXT_DB	((4 << 12) | GLOBAL_ATU_OP_BUSY)
#define GLOBAL_ATU_OP_FLUSH_DB		((5 << 12) | GLOBAL_ATU_OP_BUSY)
#define GLOBAL_ATU_OP_FLUSH_UNLOCKED_DB ((6 << 12) | GLOBAL_ATU_OP_BUSY)
#define GLOBAL_ATU_DATA		0x0c
#define GLOBAL_ATU_DATA_PORT_VECTOR_MASK	0x3f0
#define GLOBAL_ATU_DATA_PORT_VECTOR_SHIFT	4
#define GLOBAL_ATU_DATA_STATE_MASK		0x0f
#define GLOBAL_ATU_DATA_STATE_UNUSED		0x00
#define GLOBAL_ATU_DATA_STATE_UC_STATIC		0x0e
#define GLOBAL_ATU_DATA_STATE_UC_LOCKED		0x0f
#define GLOBAL_ATU_DATA_STATE_MC_STATIC		0x07
#define GLOBAL_ATU_DATA_STATE_MC_LOCKED		0x0e
#define GLOBAL_ATU_MAC_01	0x0d
#define GLOBAL_ATU_MAC_23	0x0e
#define GLOBAL_ATU_MAC_45	0x0f

struct mv88e6060_priv {
	/* MDIO bus and address on bus to use. When in single chip
	 * mode, address is 0, and the switch uses multiple addresses
	 * on the bus.  When in multi-chip mode, the switch uses a
	 * single address which contains two registers used for
	 * indirect access to more registers.
	 */
	struct mii_bus *bus;
	int sw_addr;
<<<<<<< HEAD
=======
	struct dsa_switch *ds;
>>>>>>> upstream/android-13
};

#endif
