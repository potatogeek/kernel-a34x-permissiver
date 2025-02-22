<<<<<<< HEAD
// SPDX-License-Identifier: GPL-2.0
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> upstream/android-13
/*
 * Copyright (c) 2015 MediaTek Inc.
 * Author:
 *  Zhigang.Wei <zhigang.wei@mediatek.com>
 *  Chunfeng.Yun <chunfeng.yun@mediatek.com>
 */

#ifndef _XHCI_MTK_H_
#define _XHCI_MTK_H_

<<<<<<< HEAD
#include "xhci.h"

=======
#include <linux/clk.h>
#include <linux/hashtable.h>

#include "xhci.h"

#define BULK_CLKS_NUM	5

/* support at most 64 ep, use 32 size hash table */
#define SCH_EP_HASH_BITS	5

>>>>>>> upstream/android-13
/**
 * To simplify scheduler algorithm, set a upper limit for ESIT,
 * if a synchromous ep's ESIT is larger than @XHCI_MTK_MAX_ESIT,
 * round down to the limit value, that means allocating more
 * bandwidth to it.
 */
<<<<<<< HEAD
#define XHCI_MTK_MAX_ESIT	64
=======
#define XHCI_MTK_MAX_ESIT	(1 << 6)
#define XHCI_MTK_BW_INDEX(x)	((x) & (XHCI_MTK_MAX_ESIT - 1))

/**
 * @fs_bus_bw: array to keep track of bandwidth already used for FS
 * @ep_list: Endpoints using this TT
 */
struct mu3h_sch_tt {
	u32 fs_bus_bw[XHCI_MTK_MAX_ESIT];
	struct list_head ep_list;
};
>>>>>>> upstream/android-13

/**
 * struct mu3h_sch_bw_info: schedule information for bandwidth domain
 *
 * @bus_bw: array to keep track of bandwidth already used at each uframes
<<<<<<< HEAD
 * @bw_ep_list: eps in the bandwidth domain
=======
>>>>>>> upstream/android-13
 *
 * treat a HS root port as a bandwidth domain, but treat a SS root port as
 * two bandwidth domains, one for IN eps and another for OUT eps.
 */
struct mu3h_sch_bw_info {
	u32 bus_bw[XHCI_MTK_MAX_ESIT];
<<<<<<< HEAD
	struct list_head bw_ep_list;
=======
>>>>>>> upstream/android-13
};

/**
 * struct mu3h_sch_ep_info: schedule information for endpoint
 *
 * @esit: unit is 125us, equal to 2 << Interval field in ep-context
<<<<<<< HEAD
 * @num_budget_microframes: number of continuous uframes
 *		(@repeat==1) scheduled within the interval
 * @bw_cost_per_microframe: bandwidth cost per microframe
 * @endpoint: linked into bandwidth domain which it belongs to
 * @ep: address of usb_host_endpoint struct
=======
 * @num_esit: number of @esit in a period
 * @num_budget_microframes: number of continuous uframes
 *		(@repeat==1) scheduled within the interval
 * @bw_cost_per_microframe: bandwidth cost per microframe
 * @hentry: hash table entry
 * @endpoint: linked into bandwidth domain which it belongs to
 * @tt_endpoint: linked into mu3h_sch_tt's list which it belongs to
 * @bw_info: bandwidth domain which this endpoint belongs
 * @sch_tt: mu3h_sch_tt linked into
 * @ep_type: endpoint type
 * @maxpkt: max packet size of endpoint
 * @ep: address of usb_host_endpoint struct
 * @allocated: the bandwidth is aready allocated from bus_bw
>>>>>>> upstream/android-13
 * @offset: which uframe of the interval that transfer should be
 *		scheduled first time within the interval
 * @repeat: the time gap between two uframes that transfers are
 *		scheduled within a interval. in the simple algorithm, only
 *		assign 0 or 1 to it; 0 means using only one uframe in a
 *		interval, and 1 means using @num_budget_microframes
 *		continuous uframes
 * @pkts: number of packets to be transferred in the scheduled uframes
 * @cs_count: number of CS that host will trigger
 * @burst_mode: burst mode for scheduling. 0: normal burst mode,
 *		distribute the bMaxBurst+1 packets for a single burst
 *		according to @pkts and @repeat, repeate the burst multiple
 *		times; 1: distribute the (bMaxBurst+1)*(Mult+1) packets
 *		according to @pkts and @repeat. normal mode is used by
 *		default
<<<<<<< HEAD
 */
struct mu3h_sch_ep_info {
	u32 esit;
	u32 num_budget_microframes;
	u32 bw_cost_per_microframe;
	struct list_head endpoint;
	void *ep;
=======
 * @bw_budget_table: table to record bandwidth budget per microframe
 */
struct mu3h_sch_ep_info {
	u32 esit;
	u32 num_esit;
	u32 num_budget_microframes;
	u32 bw_cost_per_microframe;
	struct list_head endpoint;
	struct hlist_node hentry;
	struct list_head tt_endpoint;
	struct mu3h_sch_bw_info *bw_info;
	struct mu3h_sch_tt *sch_tt;
	u32 ep_type;
	u32 maxpkt;
	struct usb_host_endpoint *ep;
	enum usb_device_speed speed;
	bool allocated;
>>>>>>> upstream/android-13
	/*
	 * mtk xHCI scheduling information put into reserved DWs
	 * in ep context
	 */
	u32 offset;
	u32 repeat;
	u32 pkts;
	u32 cs_count;
	u32 burst_mode;
<<<<<<< HEAD
=======
	u32 bw_budget_table[];
>>>>>>> upstream/android-13
};

#define MU3C_U3_PORT_MAX 4
#define MU3C_U2_PORT_MAX 5

/**
 * struct mu3c_ippc_regs: MTK ssusb ip port control registers
 * @ip_pw_ctr0~3: ip power and clock control registers
 * @ip_pw_sts1~2: ip power and clock status registers
 * @ip_xhci_cap: ip xHCI capability register
 * @u3_ctrl_p[x]: ip usb3 port x control register, only low 4bytes are used
 * @u2_ctrl_p[x]: ip usb2 port x control register, only low 4bytes are used
 * @u2_phy_pll: usb2 phy pll control register
 */
struct mu3c_ippc_regs {
	__le32 ip_pw_ctr0;
	__le32 ip_pw_ctr1;
	__le32 ip_pw_ctr2;
	__le32 ip_pw_ctr3;
	__le32 ip_pw_sts1;
	__le32 ip_pw_sts2;
	__le32 reserved0[3];
	__le32 ip_xhci_cap;
	__le32 reserved1[2];
	__le64 u3_ctrl_p[MU3C_U3_PORT_MAX];
	__le64 u2_ctrl_p[MU3C_U2_PORT_MAX];
	__le32 reserved2;
	__le32 u2_phy_pll;
	__le32 reserved3[33]; /* 0x80 ~ 0xff */
};

struct xhci_hcd_mtk {
	struct device *dev;
	struct usb_hcd *hcd;
	struct mu3h_sch_bw_info *sch_array;
<<<<<<< HEAD
	struct mu3c_ippc_regs __iomem *ippc_regs;
	bool has_ippc;
	int num_u2_ports;
	int num_u3_ports;
	int u3p_dis_msk;
	struct regulator *vusb33;
	struct regulator *vbus;
	struct clk *sys_clk;	/* sys and mac clock */
	struct clk *ref_clk;
	struct clk *mcu_clk;
	struct clk *dma_clk;
	struct regmap *pericfg;
	struct phy **phys;
	int num_phys;
	bool lpm_support;
	bool u2_lpm_disable;
	/* usb remote wakeup */
	bool uwk_en;
=======
	struct list_head bw_ep_chk_list;
	DECLARE_HASHTABLE(sch_ep_hash, SCH_EP_HASH_BITS);
	struct mu3c_ippc_regs __iomem *ippc_regs;
	int num_u2_ports;
	int num_u3_ports;
	int u2p_dis_msk;
	int u3p_dis_msk;
	struct regulator *vusb33;
	struct regulator *vbus;
	struct clk_bulk_data clks[BULK_CLKS_NUM];
	unsigned int has_ippc:1;
	unsigned int lpm_support:1;
	unsigned int u2_lpm_disable:1;
	/* usb remote wakeup */
	unsigned int uwk_en:1;
>>>>>>> upstream/android-13
	struct regmap *uwk;
	u32 uwk_reg_base;
	u32 uwk_vers;
};

static inline struct xhci_hcd_mtk *hcd_to_mtk(struct usb_hcd *hcd)
{
	return dev_get_drvdata(hcd->self.controller);
}

<<<<<<< HEAD
int mtk_xhci_wakelock_lock(struct xhci_hcd_mtk *mtk);
int mtk_xhci_wakelock_unlock(struct xhci_hcd_mtk *mtk);

#if IS_ENABLED(CONFIG_USB_XHCI_MTK)
int xhci_mtk_sch_init(struct xhci_hcd_mtk *mtk);
void xhci_mtk_sch_exit(struct xhci_hcd_mtk *mtk);
int xhci_mtk_add_ep_quirk(struct usb_hcd *hcd, struct usb_device *udev,
		struct usb_host_endpoint *ep);
void xhci_mtk_drop_ep_quirk(struct usb_hcd *hcd, struct usb_device *udev,
		struct usb_host_endpoint *ep);

#else
static inline int xhci_mtk_add_ep_quirk(struct usb_hcd *hcd,
	struct usb_device *udev, struct usb_host_endpoint *ep)
{
	return 0;
}

static inline void xhci_mtk_drop_ep_quirk(struct usb_hcd *hcd,
	struct usb_device *udev, struct usb_host_endpoint *ep)
{
}

#endif
#if IS_ENABLED(CONFIG_MTK_USB_OFFLOAD)
extern void xhci_link_segments(struct xhci_hcd *xhci,
		struct xhci_segment *prev,
		struct xhci_segment *next, enum xhci_ring_type type);
#endif
=======
int xhci_mtk_sch_init(struct xhci_hcd_mtk *mtk);
void xhci_mtk_sch_exit(struct xhci_hcd_mtk *mtk);
int xhci_mtk_add_ep(struct usb_hcd *hcd, struct usb_device *udev,
		    struct usb_host_endpoint *ep);
int xhci_mtk_drop_ep(struct usb_hcd *hcd, struct usb_device *udev,
		     struct usb_host_endpoint *ep);
int xhci_mtk_check_bandwidth(struct usb_hcd *hcd, struct usb_device *udev);
void xhci_mtk_reset_bandwidth(struct usb_hcd *hcd, struct usb_device *udev);

>>>>>>> upstream/android-13
#endif		/* _XHCI_MTK_H_ */
