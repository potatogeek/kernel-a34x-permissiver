<<<<<<< HEAD
// SPDX-License-Identifier: GPL-2.0
/*
 * Driver for the NXP ISP1760 chip
 *
=======
/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Driver for the NXP ISP1760 chip
 *
 * Copyright 2021 Linaro, Rui Miguel Silva
>>>>>>> upstream/android-13
 * Copyright 2014 Laurent Pinchart
 * Copyright 2007 Sebastian Siewior
 *
 * Contacts:
 *     Sebastian Siewior <bigeasy@linutronix.de>
 *     Laurent Pinchart <laurent.pinchart@ideasonboard.com>
<<<<<<< HEAD
 */

#ifndef _ISP1760_REGS_H_
#define _ISP1760_REGS_H_
=======
 *     Rui Miguel Silva <rui.silva@linaro.org>
 */

#ifndef _ISP176x_REGS_H_
#define _ISP176x_REGS_H_
>>>>>>> upstream/android-13

/* -----------------------------------------------------------------------------
 * Host Controller
 */

<<<<<<< HEAD
/* EHCI capability registers */
#define HC_CAPLENGTH		0x000
#define HC_LENGTH(p)		(((p) >> 00) & 0x00ff)	/* bits 7:0 */
#define HC_VERSION(p)		(((p) >> 16) & 0xffff)	/* bits 31:16 */

#define HC_HCSPARAMS		0x004
#define HCS_INDICATOR(p)	((p) & (1 << 16))	/* true: has port indicators */
#define HCS_PPC(p)		((p) & (1 << 4))	/* true: port power control */
#define HCS_N_PORTS(p)		(((p) >> 0) & 0xf)	/* bits 3:0, ports on HC */

#define HC_HCCPARAMS		0x008
#define HCC_ISOC_CACHE(p)       ((p) & (1 << 7))	/* true: can cache isoc frame */
#define HCC_ISOC_THRES(p)       (((p) >> 4) & 0x7)	/* bits 6:4, uframes cached */

/* EHCI operational registers */
#define HC_USBCMD		0x020
#define CMD_LRESET		(1 << 7)		/* partial reset (no ports, etc) */
#define CMD_RESET		(1 << 1)		/* reset HC not bus */
#define CMD_RUN			(1 << 0)		/* start/stop HC */

#define HC_USBSTS		0x024
#define STS_PCD			(1 << 2)		/* port change detect */

#define HC_FRINDEX		0x02c

#define HC_CONFIGFLAG		0x060
#define FLAG_CF			(1 << 0)		/* true: we'll support "high speed" */

#define HC_PORTSC1		0x064
#define PORT_OWNER		(1 << 13)		/* true: companion hc owns this port */
#define PORT_POWER		(1 << 12)		/* true: has power (see PPC) */
#define PORT_USB11(x)		(((x) & (3 << 10)) == (1 << 10))	/* USB 1.1 device */
#define PORT_RESET		(1 << 8)		/* reset port */
#define PORT_SUSPEND		(1 << 7)		/* suspend port */
#define PORT_RESUME		(1 << 6)		/* resume it */
#define PORT_PE			(1 << 2)		/* port enable */
#define PORT_CSC		(1 << 1)		/* connect status change */
#define PORT_CONNECT		(1 << 0)		/* device connected */
#define PORT_RWC_BITS		(PORT_CSC)

#define HC_ISO_PTD_DONEMAP_REG	0x130
#define HC_ISO_PTD_SKIPMAP_REG	0x134
#define HC_ISO_PTD_LASTPTD_REG	0x138
#define HC_INT_PTD_DONEMAP_REG	0x140
#define HC_INT_PTD_SKIPMAP_REG	0x144
#define HC_INT_PTD_LASTPTD_REG	0x148
#define HC_ATL_PTD_DONEMAP_REG	0x150
#define HC_ATL_PTD_SKIPMAP_REG	0x154
#define HC_ATL_PTD_LASTPTD_REG	0x158

/* Configuration Register */
#define HC_HW_MODE_CTRL		0x300
#define ALL_ATX_RESET		(1 << 31)
#define HW_ANA_DIGI_OC		(1 << 15)
#define HW_DEV_DMA		(1 << 11)
#define HW_COMN_IRQ		(1 << 10)
#define HW_COMN_DMA		(1 << 9)
#define HW_DATA_BUS_32BIT	(1 << 8)
#define HW_DACK_POL_HIGH	(1 << 6)
#define HW_DREQ_POL_HIGH	(1 << 5)
#define HW_INTR_HIGH_ACT	(1 << 2)
#define HW_INTR_EDGE_TRIG	(1 << 1)
#define HW_GLOBAL_INTR_EN	(1 << 0)

#define HC_CHIP_ID_REG		0x304
#define HC_SCRATCH_REG		0x308

#define HC_RESET_REG		0x30c
#define SW_RESET_RESET_HC	(1 << 1)
#define SW_RESET_RESET_ALL	(1 << 0)

#define HC_BUFFER_STATUS_REG	0x334
#define ISO_BUF_FILL		(1 << 2)
#define INT_BUF_FILL		(1 << 1)
#define ATL_BUF_FILL		(1 << 0)

#define HC_MEMORY_REG		0x33c
#define ISP_BANK(x)		((x) << 16)

#define HC_PORT1_CTRL		0x374
#define PORT1_POWER		(3 << 3)
#define PORT1_INIT1		(1 << 7)
#define PORT1_INIT2		(1 << 23)
#define HW_OTG_CTRL_SET		0x374
#define HW_OTG_CTRL_CLR		0x376
#define HW_OTG_DISABLE		(1 << 10)
#define HW_OTG_SE0_EN		(1 << 9)
#define HW_BDIS_ACON_EN		(1 << 8)
#define HW_SW_SEL_HC_DC		(1 << 7)
#define HW_VBUS_CHRG		(1 << 6)
#define HW_VBUS_DISCHRG		(1 << 5)
#define HW_VBUS_DRV		(1 << 4)
#define HW_SEL_CP_EXT		(1 << 3)
#define HW_DM_PULLDOWN		(1 << 2)
#define HW_DP_PULLDOWN		(1 << 1)
#define HW_DP_PULLUP		(1 << 0)

/* Interrupt Register */
#define HC_INTERRUPT_REG	0x310

#define HC_INTERRUPT_ENABLE	0x314
#define HC_ISO_INT		(1 << 9)
#define HC_ATL_INT		(1 << 8)
#define HC_INTL_INT		(1 << 7)
#define HC_EOT_INT		(1 << 3)
#define HC_SOT_INT		(1 << 1)
#define INTERRUPT_ENABLE_MASK	(HC_INTL_INT | HC_ATL_INT)

#define HC_ISO_IRQ_MASK_OR_REG	0x318
#define HC_INT_IRQ_MASK_OR_REG	0x31c
#define HC_ATL_IRQ_MASK_OR_REG	0x320
#define HC_ISO_IRQ_MASK_AND_REG	0x324
#define HC_INT_IRQ_MASK_AND_REG	0x328
#define HC_ATL_IRQ_MASK_AND_REG	0x32c
=======
/* ISP1760/31 */
/* EHCI capability registers */
#define ISP176x_HC_VERSION		0x002
#define ISP176x_HC_HCSPARAMS		0x004
#define ISP176x_HC_HCCPARAMS		0x008

/* EHCI operational registers */
#define ISP176x_HC_USBCMD		0x020
#define ISP176x_HC_USBSTS		0x024
#define ISP176x_HC_FRINDEX		0x02c

#define ISP176x_HC_CONFIGFLAG		0x060
#define ISP176x_HC_PORTSC1		0x064

#define ISP176x_HC_ISO_PTD_DONEMAP	0x130
#define ISP176x_HC_ISO_PTD_SKIPMAP	0x134
#define ISP176x_HC_ISO_PTD_LASTPTD	0x138
#define ISP176x_HC_INT_PTD_DONEMAP	0x140
#define ISP176x_HC_INT_PTD_SKIPMAP	0x144
#define ISP176x_HC_INT_PTD_LASTPTD	0x148
#define ISP176x_HC_ATL_PTD_DONEMAP	0x150
#define ISP176x_HC_ATL_PTD_SKIPMAP	0x154
#define ISP176x_HC_ATL_PTD_LASTPTD	0x158

/* Configuration Register */
#define ISP176x_HC_HW_MODE_CTRL		0x300
#define ISP176x_HC_CHIP_ID		0x304
#define ISP176x_HC_SCRATCH		0x308
#define ISP176x_HC_RESET		0x30c
#define ISP176x_HC_BUFFER_STATUS	0x334
#define ISP176x_HC_MEMORY		0x33c

/* Interrupt Register */
#define ISP176x_HC_INTERRUPT		0x310
#define ISP176x_HC_INTERRUPT_ENABLE	0x314
#define ISP176x_HC_ISO_IRQ_MASK_OR	0x318
#define ISP176x_HC_INT_IRQ_MASK_OR	0x31c
#define ISP176x_HC_ATL_IRQ_MASK_OR	0x320
#define ISP176x_HC_ISO_IRQ_MASK_AND	0x324
#define ISP176x_HC_INT_IRQ_MASK_AND	0x328
#define ISP176x_HC_ATL_IRQ_MASK_AND	0x32c

#define ISP176x_HC_OTG_CTRL		0x374
#define ISP176x_HC_OTG_CTRL_SET		0x374
#define ISP176x_HC_OTG_CTRL_CLEAR	0x376

enum isp176x_host_controller_fields {
	/* HC_PORTSC1 */
	PORT_OWNER, PORT_POWER, PORT_LSTATUS, PORT_RESET, PORT_SUSPEND,
	PORT_RESUME, PORT_PE, PORT_CSC, PORT_CONNECT,
	/* HC_HCSPARAMS */
	HCS_PPC, HCS_N_PORTS,
	/* HC_HCCPARAMS */
	HCC_ISOC_CACHE, HCC_ISOC_THRES,
	/* HC_USBCMD */
	CMD_LRESET, CMD_RESET, CMD_RUN,
	/* HC_USBSTS */
	STS_PCD,
	/* HC_FRINDEX */
	HC_FRINDEX,
	/* HC_CONFIGFLAG */
	FLAG_CF,
	/* ISO/INT/ATL PTD */
	HC_ISO_PTD_DONEMAP, HC_ISO_PTD_SKIPMAP, HC_ISO_PTD_LASTPTD,
	HC_INT_PTD_DONEMAP, HC_INT_PTD_SKIPMAP, HC_INT_PTD_LASTPTD,
	HC_ATL_PTD_DONEMAP, HC_ATL_PTD_SKIPMAP, HC_ATL_PTD_LASTPTD,
	/* HC_HW_MODE_CTRL */
	ALL_ATX_RESET, HW_ANA_DIGI_OC, HW_DEV_DMA, HW_COMN_IRQ, HW_COMN_DMA,
	HW_DATA_BUS_WIDTH, HW_DACK_POL_HIGH, HW_DREQ_POL_HIGH, HW_INTR_HIGH_ACT,
	HW_INTF_LOCK, HW_INTR_EDGE_TRIG, HW_GLOBAL_INTR_EN,
	/* HC_CHIP_ID */
	HC_CHIP_ID_HIGH, HC_CHIP_ID_LOW, HC_CHIP_REV,
	/* HC_SCRATCH */
	HC_SCRATCH,
	/* HC_RESET */
	SW_RESET_RESET_ATX, SW_RESET_RESET_HC, SW_RESET_RESET_ALL,
	/* HC_BUFFER_STATUS */
	ISO_BUF_FILL, INT_BUF_FILL, ATL_BUF_FILL,
	/* HC_MEMORY */
	MEM_BANK_SEL, MEM_START_ADDR,
	/* HC_DATA */
	HC_DATA,
	/* HC_INTERRUPT */
	HC_INTERRUPT,
	/* HC_INTERRUPT_ENABLE */
	HC_INT_IRQ_ENABLE, HC_ATL_IRQ_ENABLE,
	/* INTERRUPT MASKS */
	HC_ISO_IRQ_MASK_OR, HC_INT_IRQ_MASK_OR, HC_ATL_IRQ_MASK_OR,
	HC_ISO_IRQ_MASK_AND, HC_INT_IRQ_MASK_AND, HC_ATL_IRQ_MASK_AND,
	/* HW_OTG_CTRL_SET */
	HW_OTG_DISABLE, HW_SW_SEL_HC_DC, HW_VBUS_DRV, HW_SEL_CP_EXT,
	HW_DM_PULLDOWN, HW_DP_PULLDOWN, HW_DP_PULLUP, HW_HC_2_DIS,
	/* HW_OTG_CTRL_CLR */
	HW_OTG_DISABLE_CLEAR, HW_SW_SEL_HC_DC_CLEAR, HW_VBUS_DRV_CLEAR,
	HW_SEL_CP_EXT_CLEAR, HW_DM_PULLDOWN_CLEAR, HW_DP_PULLDOWN_CLEAR,
	HW_DP_PULLUP_CLEAR, HW_HC_2_DIS_CLEAR,
	/* Last element */
	HC_FIELD_MAX,
};

/* ISP1763 */
/* EHCI operational registers */
#define ISP1763_HC_USBCMD		0x8c
#define ISP1763_HC_USBSTS		0x90
#define ISP1763_HC_FRINDEX		0x98

#define ISP1763_HC_CONFIGFLAG		0x9c
#define ISP1763_HC_PORTSC1		0xa0

#define ISP1763_HC_ISO_PTD_DONEMAP	0xa4
#define ISP1763_HC_ISO_PTD_SKIPMAP	0xa6
#define ISP1763_HC_ISO_PTD_LASTPTD	0xa8
#define ISP1763_HC_INT_PTD_DONEMAP	0xaa
#define ISP1763_HC_INT_PTD_SKIPMAP	0xac
#define ISP1763_HC_INT_PTD_LASTPTD	0xae
#define ISP1763_HC_ATL_PTD_DONEMAP	0xb0
#define ISP1763_HC_ATL_PTD_SKIPMAP	0xb2
#define ISP1763_HC_ATL_PTD_LASTPTD	0xb4

/* Configuration Register */
#define ISP1763_HC_HW_MODE_CTRL		0xb6
#define ISP1763_HC_CHIP_REV		0x70
#define ISP1763_HC_CHIP_ID		0x72
#define ISP1763_HC_SCRATCH		0x78
#define ISP1763_HC_RESET		0xb8
#define ISP1763_HC_BUFFER_STATUS	0xba
#define ISP1763_HC_MEMORY		0xc4
#define ISP1763_HC_DATA			0xc6

/* Interrupt Register */
#define ISP1763_HC_INTERRUPT		0xd4
#define ISP1763_HC_INTERRUPT_ENABLE	0xd6
#define ISP1763_HC_ISO_IRQ_MASK_OR	0xd8
#define ISP1763_HC_INT_IRQ_MASK_OR	0xda
#define ISP1763_HC_ATL_IRQ_MASK_OR	0xdc
#define ISP1763_HC_ISO_IRQ_MASK_AND	0xde
#define ISP1763_HC_INT_IRQ_MASK_AND	0xe0
#define ISP1763_HC_ATL_IRQ_MASK_AND	0xe2

#define ISP1763_HC_OTG_CTRL_SET		0xe4
#define ISP1763_HC_OTG_CTRL_CLEAR	0xe6
>>>>>>> upstream/android-13

/* -----------------------------------------------------------------------------
 * Peripheral Controller
 */

<<<<<<< HEAD
/* Initialization Registers */
#define DC_ADDRESS			0x0200
#define DC_DEVEN			(1 << 7)

#define DC_MODE				0x020c
#define DC_DMACLKON			(1 << 9)
#define DC_VBUSSTAT			(1 << 8)
#define DC_CLKAON			(1 << 7)
#define DC_SNDRSU			(1 << 6)
#define DC_GOSUSP			(1 << 5)
#define DC_SFRESET			(1 << 4)
#define DC_GLINTENA			(1 << 3)
#define DC_WKUPCS			(1 << 2)

#define DC_INTCONF			0x0210
#define DC_CDBGMOD_ACK_NAK		(0 << 6)
#define DC_CDBGMOD_ACK			(1 << 6)
#define DC_CDBGMOD_ACK_1NAK		(2 << 6)
#define DC_DDBGMODIN_ACK_NAK		(0 << 4)
#define DC_DDBGMODIN_ACK		(1 << 4)
#define DC_DDBGMODIN_ACK_1NAK		(2 << 4)
#define DC_DDBGMODOUT_ACK_NYET_NAK	(0 << 2)
#define DC_DDBGMODOUT_ACK_NYET		(1 << 2)
#define DC_DDBGMODOUT_ACK_NYET_1NAK	(2 << 2)
#define DC_INTLVL			(1 << 1)
#define DC_INTPOL			(1 << 0)

#define DC_DEBUG			0x0212
#define DC_INTENABLE			0x0214
#define DC_IEPTX(n)			(1 << (11 + 2 * (n)))
#define DC_IEPRX(n)			(1 << (10 + 2 * (n)))
#define DC_IEPRXTX(n)			(3 << (10 + 2 * (n)))
#define DC_IEP0SETUP			(1 << 8)
#define DC_IEVBUS			(1 << 7)
#define DC_IEDMA			(1 << 6)
#define DC_IEHS_STA			(1 << 5)
#define DC_IERESM			(1 << 4)
#define DC_IESUSP			(1 << 3)
#define DC_IEPSOF			(1 << 2)
#define DC_IESOF			(1 << 1)
#define DC_IEBRST			(1 << 0)

/* Data Flow Registers */
#define DC_EPINDEX			0x022c
#define DC_EP0SETUP			(1 << 5)
#define DC_ENDPIDX(n)			((n) << 1)
#define DC_EPDIR			(1 << 0)

#define DC_CTRLFUNC			0x0228
#define DC_CLBUF			(1 << 4)
#define DC_VENDP			(1 << 3)
#define DC_DSEN				(1 << 2)
#define DC_STATUS			(1 << 1)
#define DC_STALL			(1 << 0)

#define DC_DATAPORT			0x0220
#define DC_BUFLEN			0x021c
#define DC_DATACOUNT_MASK		0xffff
#define DC_BUFSTAT			0x021e
#define DC_EPMAXPKTSZ			0x0204

#define DC_EPTYPE			0x0208
#define DC_NOEMPKT			(1 << 4)
#define DC_EPENABLE			(1 << 3)
#define DC_DBLBUF			(1 << 2)
#define DC_ENDPTYP_ISOC			(1 << 0)
#define DC_ENDPTYP_BULK			(2 << 0)
#define DC_ENDPTYP_INTERRUPT		(3 << 0)

/* DMA Registers */
#define DC_DMACMD			0x0230
#define DC_DMATXCOUNT			0x0234
#define DC_DMACONF			0x0238
#define DC_DMAHW			0x023c
#define DC_DMAINTREASON			0x0250
#define DC_DMAINTEN			0x0254
#define DC_DMAEP			0x0258
#define DC_DMABURSTCOUNT		0x0264

/* General Registers */
#define DC_INTERRUPT			0x0218
#define DC_CHIPID			0x0270
#define DC_FRAMENUM			0x0274
#define DC_SCRATCH			0x0278
#define DC_UNLOCKDEV			0x027c
#define DC_INTPULSEWIDTH		0x0280
#define DC_TESTMODE			0x0284
=======
#define DC_IEPTX(n)			(1 << (11 + 2 * (n)))
#define DC_IEPRX(n)			(1 << (10 + 2 * (n)))
#define DC_IEPRXTX(n)			(3 << (10 + 2 * (n)))

#define ISP176x_DC_CDBGMOD_ACK		BIT(6)
#define ISP176x_DC_DDBGMODIN_ACK	BIT(4)
#define ISP176x_DC_DDBGMODOUT_ACK	BIT(2)

#define ISP176x_DC_IEP0SETUP		BIT(8)
#define ISP176x_DC_IEVBUS		BIT(7)
#define ISP176x_DC_IEHS_STA		BIT(5)
#define ISP176x_DC_IERESM		BIT(4)
#define ISP176x_DC_IESUSP		BIT(3)
#define ISP176x_DC_IEBRST		BIT(0)

#define ISP176x_HW_OTG_DISABLE_CLEAR	BIT(26)
#define ISP176x_HW_SW_SEL_HC_DC_CLEAR	BIT(23)
#define ISP176x_HW_VBUS_DRV_CLEAR	BIT(20)
#define ISP176x_HW_SEL_CP_EXT_CLEAR	BIT(19)
#define ISP176x_HW_DM_PULLDOWN_CLEAR	BIT(18)
#define ISP176x_HW_DP_PULLDOWN_CLEAR	BIT(17)
#define ISP176x_HW_DP_PULLUP_CLEAR	BIT(16)
#define ISP176x_HW_OTG_DISABLE		BIT(10)
#define ISP176x_HW_SW_SEL_HC_DC		BIT(7)
#define ISP176x_HW_VBUS_DRV		BIT(4)
#define ISP176x_HW_SEL_CP_EXT		BIT(3)
#define ISP176x_HW_DM_PULLDOWN		BIT(2)
#define ISP176x_HW_DP_PULLDOWN		BIT(1)
#define ISP176x_HW_DP_PULLUP		BIT(0)

#define ISP176x_DC_ENDPTYP_ISOC		0x01
#define ISP176x_DC_ENDPTYP_BULK		0x02
#define ISP176x_DC_ENDPTYP_INTERRUPT	0x03

/* Initialization Registers */
#define ISP176x_DC_ADDRESS		0x0200
#define ISP176x_DC_MODE			0x020c
#define ISP176x_DC_INTCONF		0x0210
#define ISP176x_DC_DEBUG		0x0212
#define ISP176x_DC_INTENABLE		0x0214

/* Data Flow Registers */
#define ISP176x_DC_EPMAXPKTSZ		0x0204
#define ISP176x_DC_EPTYPE		0x0208

#define ISP176x_DC_BUFLEN		0x021c
#define ISP176x_DC_BUFSTAT		0x021e
#define ISP176x_DC_DATAPORT		0x0220

#define ISP176x_DC_CTRLFUNC		0x0228
#define ISP176x_DC_EPINDEX		0x022c

/* DMA Registers */
#define ISP176x_DC_DMACMD		0x0230
#define ISP176x_DC_DMATXCOUNT		0x0234
#define ISP176x_DC_DMACONF		0x0238
#define ISP176x_DC_DMAHW		0x023c
#define ISP176x_DC_DMAINTREASON		0x0250
#define ISP176x_DC_DMAINTEN		0x0254
#define ISP176x_DC_DMAEP		0x0258
#define ISP176x_DC_DMABURSTCOUNT	0x0264

/* General Registers */
#define ISP176x_DC_INTERRUPT		0x0218
#define ISP176x_DC_CHIPID		0x0270
#define ISP176x_DC_FRAMENUM		0x0274
#define ISP176x_DC_SCRATCH		0x0278
#define ISP176x_DC_UNLOCKDEV		0x027c
#define ISP176x_DC_INTPULSEWIDTH	0x0280
#define ISP176x_DC_TESTMODE		0x0284

enum isp176x_device_controller_fields {
	/* DC_ADDRESS */
	DC_DEVEN, DC_DEVADDR,
	/* DC_MODE */
	DC_VBUSSTAT, DC_SFRESET, DC_GLINTENA,
	/* DC_INTCONF */
	DC_CDBGMOD_ACK, DC_DDBGMODIN_ACK, DC_DDBGMODOUT_ACK, DC_INTPOL,
	/* DC_INTENABLE */
	DC_IEPRXTX_7, DC_IEPRXTX_6, DC_IEPRXTX_5, DC_IEPRXTX_4, DC_IEPRXTX_3,
	DC_IEPRXTX_2, DC_IEPRXTX_1, DC_IEPRXTX_0,
	DC_IEP0SETUP, DC_IEVBUS, DC_IEHS_STA, DC_IERESM, DC_IESUSP, DC_IEBRST,
	/* DC_EPINDEX */
	DC_EP0SETUP, DC_ENDPIDX, DC_EPDIR,
	/* DC_CTRLFUNC */
	DC_CLBUF, DC_VENDP, DC_DSEN, DC_STATUS, DC_STALL,
	/* DC_BUFLEN */
	DC_BUFLEN,
	/* DC_EPMAXPKTSZ */
	DC_FFOSZ,
	/* DC_EPTYPE */
	DC_EPENABLE, DC_ENDPTYP,
	/* DC_FRAMENUM */
	DC_FRAMENUM, DC_UFRAMENUM,
	/* DC_CHIP_ID */
	DC_CHIP_ID_HIGH, DC_CHIP_ID_LOW,
	/* DC_SCRATCH */
	DC_SCRATCH,
	/* Last element */
	DC_FIELD_MAX,
};

/* ISP1763 */
/* Initialization Registers */
#define ISP1763_DC_ADDRESS		0x00
#define ISP1763_DC_MODE			0x0c
#define ISP1763_DC_INTCONF		0x10
#define ISP1763_DC_INTENABLE		0x14

/* Data Flow Registers */
#define ISP1763_DC_EPMAXPKTSZ		0x04
#define ISP1763_DC_EPTYPE		0x08

#define ISP1763_DC_BUFLEN		0x1c
#define ISP1763_DC_BUFSTAT		0x1e
#define ISP1763_DC_DATAPORT		0x20

#define ISP1763_DC_CTRLFUNC		0x28
#define ISP1763_DC_EPINDEX		0x2c

/* DMA Registers */
#define ISP1763_DC_DMACMD		0x30
#define ISP1763_DC_DMATXCOUNT		0x34
#define ISP1763_DC_DMACONF		0x38
#define ISP1763_DC_DMAHW		0x3c
#define ISP1763_DC_DMAINTREASON		0x50
#define ISP1763_DC_DMAINTEN		0x54
#define ISP1763_DC_DMAEP		0x58
#define ISP1763_DC_DMABURSTCOUNT	0x64

/* General Registers */
#define ISP1763_DC_INTERRUPT		0x18
#define ISP1763_DC_CHIPID_LOW		0x70
#define ISP1763_DC_CHIPID_HIGH		0x72
#define ISP1763_DC_FRAMENUM		0x74
#define ISP1763_DC_SCRATCH		0x78
#define ISP1763_DC_UNLOCKDEV		0x7c
#define ISP1763_DC_INTPULSEWIDTH	0x80
#define ISP1763_DC_TESTMODE		0x84
>>>>>>> upstream/android-13

#endif
