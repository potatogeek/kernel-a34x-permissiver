/* SPDX-License-Identifier: GPL-2.0 WITH Linux-syscall-note */
#ifndef _UAPI__LINUX_BRIDGE_EBT_802_3_H
#define _UAPI__LINUX_BRIDGE_EBT_802_3_H

#include <linux/types.h>
#include <linux/if_ether.h>

#define EBT_802_3_SAP 0x01
#define EBT_802_3_TYPE 0x02

#define EBT_802_3_MATCH "802_3"


#define CHECK_TYPE 0xaa


#define IS_UI 0x03

#define EBT_802_3_MASK (EBT_802_3_SAP | EBT_802_3_TYPE | EBT_802_3)


struct hdr_ui {
	__u8 dsap;
	__u8 ssap;
	__u8 ctrl;
	__u8 orig[3];
	__be16 type;
};

struct hdr_ni {
	__u8 dsap;
	__u8 ssap;
	__be16 ctrl;
	__u8  orig[3];
	__be16 type;
};

struct ebt_802_3_hdr {
	__u8  daddr[ETH_ALEN];
	__u8  saddr[ETH_ALEN];
	__be16 len;
	union {
		struct hdr_ui ui;
		struct hdr_ni ni;
	} llc;
};


struct ebt_802_3_info {
	__u8  sap;
	__be16 type;
	__u8  bitmask;
	__u8  invflags;
};

#endif 
