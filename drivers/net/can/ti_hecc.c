/*
 * TI HECC (CAN) device driver
 *
 * This driver supports TI's HECC (High End CAN Controller module) and the
 * specs for the same is available at <http://www.ti.com>
 *
 * Copyright (C) 2009 Texas Instruments Incorporated - http://www.ti.com/
<<<<<<< HEAD
=======
 * Copyright (C) 2019 Jeroen Hofstee <jhofstee@victronenergy.com>
>>>>>>> upstream/android-13
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation version 2.
 *
 * This program is distributed as is WITHOUT ANY WARRANTY of any
 * kind, whether express or implied; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/interrupt.h>
#include <linux/errno.h>
#include <linux/netdevice.h>
#include <linux/skbuff.h>
#include <linux/platform_device.h>
#include <linux/clk.h>
#include <linux/io.h>
#include <linux/of.h>
#include <linux/of_device.h>
#include <linux/regulator/consumer.h>

#include <linux/can/dev.h>
#include <linux/can/error.h>
#include <linux/can/led.h>
<<<<<<< HEAD
=======
#include <linux/can/rx-offload.h>
>>>>>>> upstream/android-13

#define DRV_NAME "ti_hecc"
#define HECC_MODULE_VERSION     "0.7"
MODULE_VERSION(HECC_MODULE_VERSION);
#define DRV_DESC "TI High End CAN Controller Driver " HECC_MODULE_VERSION

/* TX / RX Mailbox Configuration */
#define HECC_MAX_MAILBOXES	32	/* hardware mailboxes - do not change */
#define MAX_TX_PRIO		0x3F	/* hardware value - do not change */

<<<<<<< HEAD
/*
 * Important Note: TX mailbox configuration
=======
/* Important Note: TX mailbox configuration
>>>>>>> upstream/android-13
 * TX mailboxes should be restricted to the number of SKB buffers to avoid
 * maintaining SKB buffers separately. TX mailboxes should be a power of 2
 * for the mailbox logic to work.  Top mailbox numbers are reserved for RX
 * and lower mailboxes for TX.
 *
 * HECC_MAX_TX_MBOX	HECC_MB_TX_SHIFT
 * 4 (default)		2
 * 8			3
 * 16			4
 */
#define HECC_MB_TX_SHIFT	2 /* as per table above */
#define HECC_MAX_TX_MBOX	BIT(HECC_MB_TX_SHIFT)

#define HECC_TX_PRIO_SHIFT	(HECC_MB_TX_SHIFT)
#define HECC_TX_PRIO_MASK	(MAX_TX_PRIO << HECC_MB_TX_SHIFT)
#define HECC_TX_MB_MASK		(HECC_MAX_TX_MBOX - 1)
#define HECC_TX_MASK		((HECC_MAX_TX_MBOX - 1) | HECC_TX_PRIO_MASK)
<<<<<<< HEAD
#define HECC_TX_MBOX_MASK	(~(BIT(HECC_MAX_TX_MBOX) - 1))
#define HECC_DEF_NAPI_WEIGHT	HECC_MAX_RX_MBOX

/*
 * Important Note: RX mailbox configuration
 * RX mailboxes are further logically split into two - main and buffer
 * mailboxes. The goal is to get all packets into main mailboxes as
 * driven by mailbox number and receive priority (higher to lower) and
 * buffer mailboxes are used to receive pkts while main mailboxes are being
 * processed. This ensures in-order packet reception.
 *
 * Here are the recommended values for buffer mailbox. Note that RX mailboxes
 * start after TX mailboxes:
 *
 * HECC_MAX_RX_MBOX		HECC_RX_BUFFER_MBOX	No of buffer mailboxes
 * 28				12			8
 * 16				20			4
 */

#define HECC_MAX_RX_MBOX	(HECC_MAX_MAILBOXES - HECC_MAX_TX_MBOX)
#define HECC_RX_BUFFER_MBOX	12 /* as per table above */
#define HECC_RX_FIRST_MBOX	(HECC_MAX_MAILBOXES - 1)
#define HECC_RX_HIGH_MBOX_MASK	(~(BIT(HECC_RX_BUFFER_MBOX) - 1))
=======

/* RX mailbox configuration
 *
 * The remaining mailboxes are used for reception and are delivered
 * based on their timestamp, to avoid a hardware race when CANME is
 * changed while CAN-bus traffic is being received.
 */
#define HECC_MAX_RX_MBOX	(HECC_MAX_MAILBOXES - HECC_MAX_TX_MBOX)
#define HECC_RX_FIRST_MBOX	(HECC_MAX_MAILBOXES - 1)
#define HECC_RX_LAST_MBOX	(HECC_MAX_TX_MBOX)
>>>>>>> upstream/android-13

/* TI HECC module registers */
#define HECC_CANME		0x0	/* Mailbox enable */
#define HECC_CANMD		0x4	/* Mailbox direction */
#define HECC_CANTRS		0x8	/* Transmit request set */
#define HECC_CANTRR		0xC	/* Transmit request */
#define HECC_CANTA		0x10	/* Transmission acknowledge */
#define HECC_CANAA		0x14	/* Abort acknowledge */
#define HECC_CANRMP		0x18	/* Receive message pending */
<<<<<<< HEAD
#define HECC_CANRML		0x1C	/* Remote message lost */
=======
#define HECC_CANRML		0x1C	/* Receive message lost */
>>>>>>> upstream/android-13
#define HECC_CANRFP		0x20	/* Remote frame pending */
#define HECC_CANGAM		0x24	/* SECC only:Global acceptance mask */
#define HECC_CANMC		0x28	/* Master control */
#define HECC_CANBTC		0x2C	/* Bit timing configuration */
#define HECC_CANES		0x30	/* Error and status */
#define HECC_CANTEC		0x34	/* Transmit error counter */
#define HECC_CANREC		0x38	/* Receive error counter */
#define HECC_CANGIF0		0x3C	/* Global interrupt flag 0 */
#define HECC_CANGIM		0x40	/* Global interrupt mask */
#define HECC_CANGIF1		0x44	/* Global interrupt flag 1 */
#define HECC_CANMIM		0x48	/* Mailbox interrupt mask */
#define HECC_CANMIL		0x4C	/* Mailbox interrupt level */
#define HECC_CANOPC		0x50	/* Overwrite protection control */
#define HECC_CANTIOC		0x54	/* Transmit I/O control */
#define HECC_CANRIOC		0x58	/* Receive I/O control */
#define HECC_CANLNT		0x5C	/* HECC only: Local network time */
#define HECC_CANTOC		0x60	/* HECC only: Time-out control */
#define HECC_CANTOS		0x64	/* HECC only: Time-out status */
#define HECC_CANTIOCE		0x68	/* SCC only:Enhanced TX I/O control */
#define HECC_CANRIOCE		0x6C	/* SCC only:Enhanced RX I/O control */

<<<<<<< HEAD
=======
/* TI HECC RAM registers */
#define HECC_CANMOTS		0x80	/* Message object time stamp */

>>>>>>> upstream/android-13
/* Mailbox registers */
#define HECC_CANMID		0x0
#define HECC_CANMCF		0x4
#define HECC_CANMDL		0x8
#define HECC_CANMDH		0xC

#define HECC_SET_REG		0xFFFFFFFF
#define HECC_CANID_MASK		0x3FF	/* 18 bits mask for extended id's */
#define HECC_CCE_WAIT_COUNT     100	/* Wait for ~1 sec for CCE bit */

#define HECC_CANMC_SCM		BIT(13)	/* SCC compat mode */
#define HECC_CANMC_CCR		BIT(12)	/* Change config request */
#define HECC_CANMC_PDR		BIT(11)	/* Local Power down - for sleep mode */
#define HECC_CANMC_ABO		BIT(7)	/* Auto Bus On */
#define HECC_CANMC_STM		BIT(6)	/* Self test mode - loopback */
#define HECC_CANMC_SRES		BIT(5)	/* Software reset */

#define HECC_CANTIOC_EN		BIT(3)	/* Enable CAN TX I/O pin */
#define HECC_CANRIOC_EN		BIT(3)	/* Enable CAN RX I/O pin */

#define HECC_CANMID_IDE		BIT(31)	/* Extended frame format */
#define HECC_CANMID_AME		BIT(30)	/* Acceptance mask enable */
#define HECC_CANMID_AAM		BIT(29)	/* Auto answer mode */

#define HECC_CANES_FE		BIT(24)	/* form error */
#define HECC_CANES_BE		BIT(23)	/* bit error */
#define HECC_CANES_SA1		BIT(22)	/* stuck at dominant error */
#define HECC_CANES_CRCE		BIT(21)	/* CRC error */
#define HECC_CANES_SE		BIT(20)	/* stuff bit error */
#define HECC_CANES_ACKE		BIT(19)	/* ack error */
#define HECC_CANES_BO		BIT(18)	/* Bus off status */
#define HECC_CANES_EP		BIT(17)	/* Error passive status */
#define HECC_CANES_EW		BIT(16)	/* Error warning status */
#define HECC_CANES_SMA		BIT(5)	/* suspend mode ack */
#define HECC_CANES_CCE		BIT(4)	/* Change config enabled */
#define HECC_CANES_PDA		BIT(3)	/* Power down mode ack */

#define HECC_CANBTC_SAM		BIT(7)	/* sample points */

#define HECC_BUS_ERROR		(HECC_CANES_FE | HECC_CANES_BE |\
				HECC_CANES_CRCE | HECC_CANES_SE |\
				HECC_CANES_ACKE)
<<<<<<< HEAD
=======
#define HECC_CANES_FLAGS	(HECC_BUS_ERROR | HECC_CANES_BO |\
				HECC_CANES_EP | HECC_CANES_EW)
>>>>>>> upstream/android-13

#define HECC_CANMCF_RTR		BIT(4)	/* Remote transmit request */

#define HECC_CANGIF_MAIF	BIT(17)	/* Message alarm interrupt */
#define HECC_CANGIF_TCOIF	BIT(16) /* Timer counter overflow int */
#define HECC_CANGIF_GMIF	BIT(15)	/* Global mailbox interrupt */
#define HECC_CANGIF_AAIF	BIT(14)	/* Abort ack interrupt */
#define HECC_CANGIF_WDIF	BIT(13)	/* Write denied interrupt */
#define HECC_CANGIF_WUIF	BIT(12)	/* Wake up interrupt */
#define HECC_CANGIF_RMLIF	BIT(11)	/* Receive message lost interrupt */
#define HECC_CANGIF_BOIF	BIT(10)	/* Bus off interrupt */
#define HECC_CANGIF_EPIF	BIT(9)	/* Error passive interrupt */
#define HECC_CANGIF_WLIF	BIT(8)	/* Warning level interrupt */
#define HECC_CANGIF_MBOX_MASK	0x1F	/* Mailbox number mask */
#define HECC_CANGIM_I1EN	BIT(1)	/* Int line 1 enable */
#define HECC_CANGIM_I0EN	BIT(0)	/* Int line 0 enable */
#define HECC_CANGIM_DEF_MASK	0x700	/* only busoff/warning/passive */
#define HECC_CANGIM_SIL		BIT(2)	/* system interrupts to int line 1 */

/* CAN Bittiming constants as per HECC specs */
static const struct can_bittiming_const ti_hecc_bittiming_const = {
	.name = DRV_NAME,
	.tseg1_min = 1,
	.tseg1_max = 16,
	.tseg2_min = 1,
	.tseg2_max = 8,
	.sjw_max = 4,
	.brp_min = 1,
	.brp_max = 256,
	.brp_inc = 1,
};

struct ti_hecc_priv {
	struct can_priv can;	/* MUST be first member/field */
<<<<<<< HEAD
	struct napi_struct napi;
=======
	struct can_rx_offload offload;
>>>>>>> upstream/android-13
	struct net_device *ndev;
	struct clk *clk;
	void __iomem *base;
	void __iomem *hecc_ram;
	void __iomem *mbx;
	bool use_hecc1int;
	spinlock_t mbx_lock; /* CANME register needs protection */
	u32 tx_head;
	u32 tx_tail;
<<<<<<< HEAD
	u32 rx_next;
=======
>>>>>>> upstream/android-13
	struct regulator *reg_xceiver;
};

static inline int get_tx_head_mb(struct ti_hecc_priv *priv)
{
	return priv->tx_head & HECC_TX_MB_MASK;
}

static inline int get_tx_tail_mb(struct ti_hecc_priv *priv)
{
	return priv->tx_tail & HECC_TX_MB_MASK;
}

static inline int get_tx_head_prio(struct ti_hecc_priv *priv)
{
	return (priv->tx_head >> HECC_TX_PRIO_SHIFT) & MAX_TX_PRIO;
}

static inline void hecc_write_lam(struct ti_hecc_priv *priv, u32 mbxno, u32 val)
{
	__raw_writel(val, priv->hecc_ram + mbxno * 4);
}

<<<<<<< HEAD
static inline void hecc_write_mbx(struct ti_hecc_priv *priv, u32 mbxno,
	u32 reg, u32 val)
=======
static inline u32 hecc_read_stamp(struct ti_hecc_priv *priv, u32 mbxno)
{
	return __raw_readl(priv->hecc_ram + HECC_CANMOTS + mbxno * 4);
}

static inline void hecc_write_mbx(struct ti_hecc_priv *priv, u32 mbxno,
				  u32 reg, u32 val)
>>>>>>> upstream/android-13
{
	__raw_writel(val, priv->mbx + mbxno * 0x10 + reg);
}

static inline u32 hecc_read_mbx(struct ti_hecc_priv *priv, u32 mbxno, u32 reg)
{
	return __raw_readl(priv->mbx + mbxno * 0x10 + reg);
}

static inline void hecc_write(struct ti_hecc_priv *priv, u32 reg, u32 val)
{
	__raw_writel(val, priv->base + reg);
}

static inline u32 hecc_read(struct ti_hecc_priv *priv, int reg)
{
	return __raw_readl(priv->base + reg);
}

static inline void hecc_set_bit(struct ti_hecc_priv *priv, int reg,
<<<<<<< HEAD
	u32 bit_mask)
=======
				u32 bit_mask)
>>>>>>> upstream/android-13
{
	hecc_write(priv, reg, hecc_read(priv, reg) | bit_mask);
}

static inline void hecc_clear_bit(struct ti_hecc_priv *priv, int reg,
<<<<<<< HEAD
	u32 bit_mask)
=======
				  u32 bit_mask)
>>>>>>> upstream/android-13
{
	hecc_write(priv, reg, hecc_read(priv, reg) & ~bit_mask);
}

static inline u32 hecc_get_bit(struct ti_hecc_priv *priv, int reg, u32 bit_mask)
{
	return (hecc_read(priv, reg) & bit_mask) ? 1 : 0;
}

static int ti_hecc_set_btc(struct ti_hecc_priv *priv)
{
	struct can_bittiming *bit_timing = &priv->can.bittiming;
	u32 can_btc;

	can_btc = (bit_timing->phase_seg2 - 1) & 0x7;
	can_btc |= ((bit_timing->phase_seg1 + bit_timing->prop_seg - 1)
			& 0xF) << 3;
	if (priv->can.ctrlmode & CAN_CTRLMODE_3_SAMPLES) {
		if (bit_timing->brp > 4)
			can_btc |= HECC_CANBTC_SAM;
		else
<<<<<<< HEAD
			netdev_warn(priv->ndev, "WARN: Triple"
				"sampling not set due to h/w limitations");
=======
			netdev_warn(priv->ndev,
				    "WARN: Triple sampling not set due to h/w limitations");
>>>>>>> upstream/android-13
	}
	can_btc |= ((bit_timing->sjw - 1) & 0x3) << 8;
	can_btc |= ((bit_timing->brp - 1) & 0xFF) << 16;

	/* ERM being set to 0 by default meaning resync at falling edge */

	hecc_write(priv, HECC_CANBTC, can_btc);
	netdev_info(priv->ndev, "setting CANBTC=%#x\n", can_btc);

	return 0;
}

static int ti_hecc_transceiver_switch(const struct ti_hecc_priv *priv,
				      int on)
{
	if (!priv->reg_xceiver)
		return 0;

	if (on)
		return regulator_enable(priv->reg_xceiver);
	else
		return regulator_disable(priv->reg_xceiver);
}

static void ti_hecc_reset(struct net_device *ndev)
{
	u32 cnt;
	struct ti_hecc_priv *priv = netdev_priv(ndev);

	netdev_dbg(ndev, "resetting hecc ...\n");
	hecc_set_bit(priv, HECC_CANMC, HECC_CANMC_SRES);

	/* Set change control request and wait till enabled */
	hecc_set_bit(priv, HECC_CANMC, HECC_CANMC_CCR);

<<<<<<< HEAD
	/*
	 * INFO: It has been observed that at times CCE bit may not be
=======
	/* INFO: It has been observed that at times CCE bit may not be
>>>>>>> upstream/android-13
	 * set and hw seems to be ok even if this bit is not set so
	 * timing out with a timing of 1ms to respect the specs
	 */
	cnt = HECC_CCE_WAIT_COUNT;
	while (!hecc_get_bit(priv, HECC_CANES, HECC_CANES_CCE) && cnt != 0) {
		--cnt;
		udelay(10);
	}

<<<<<<< HEAD
	/*
	 * Note: On HECC, BTC can be programmed only in initialization mode, so
=======
	/* Note: On HECC, BTC can be programmed only in initialization mode, so
>>>>>>> upstream/android-13
	 * it is expected that the can bittiming parameters are set via ip
	 * utility before the device is opened
	 */
	ti_hecc_set_btc(priv);

	/* Clear CCR (and CANMC register) and wait for CCE = 0 enable */
	hecc_write(priv, HECC_CANMC, 0);

<<<<<<< HEAD
	/*
	 * INFO: CAN net stack handles bus off and hence disabling auto-bus-on
	 * hecc_set_bit(priv, HECC_CANMC, HECC_CANMC_ABO);
	 */

	/*
	 * INFO: It has been observed that at times CCE bit may not be
=======
	/* INFO: CAN net stack handles bus off and hence disabling auto-bus-on
	 * hecc_set_bit(priv, HECC_CANMC, HECC_CANMC_ABO);
	 */

	/* INFO: It has been observed that at times CCE bit may not be
>>>>>>> upstream/android-13
	 * set and hw seems to be ok even if this bit is not set so
	 */
	cnt = HECC_CCE_WAIT_COUNT;
	while (hecc_get_bit(priv, HECC_CANES, HECC_CANES_CCE) && cnt != 0) {
		--cnt;
		udelay(10);
	}

	/* Enable TX and RX I/O Control pins */
	hecc_write(priv, HECC_CANTIOC, HECC_CANTIOC_EN);
	hecc_write(priv, HECC_CANRIOC, HECC_CANRIOC_EN);

	/* Clear registers for clean operation */
	hecc_write(priv, HECC_CANTA, HECC_SET_REG);
	hecc_write(priv, HECC_CANRMP, HECC_SET_REG);
	hecc_write(priv, HECC_CANGIF0, HECC_SET_REG);
	hecc_write(priv, HECC_CANGIF1, HECC_SET_REG);
	hecc_write(priv, HECC_CANME, 0);
	hecc_write(priv, HECC_CANMD, 0);

	/* SCC compat mode NOT supported (and not needed too) */
	hecc_set_bit(priv, HECC_CANMC, HECC_CANMC_SCM);
}

static void ti_hecc_start(struct net_device *ndev)
{
	struct ti_hecc_priv *priv = netdev_priv(ndev);
	u32 cnt, mbxno, mbx_mask;

	/* put HECC in initialization mode and set btc */
	ti_hecc_reset(ndev);

<<<<<<< HEAD
	priv->tx_head = priv->tx_tail = HECC_TX_MASK;
	priv->rx_next = HECC_RX_FIRST_MBOX;
=======
	priv->tx_head = HECC_TX_MASK;
	priv->tx_tail = HECC_TX_MASK;
>>>>>>> upstream/android-13

	/* Enable local and global acceptance mask registers */
	hecc_write(priv, HECC_CANGAM, HECC_SET_REG);

	/* Prepare configured mailboxes to receive messages */
	for (cnt = 0; cnt < HECC_MAX_RX_MBOX; cnt++) {
		mbxno = HECC_MAX_MAILBOXES - 1 - cnt;
		mbx_mask = BIT(mbxno);
		hecc_clear_bit(priv, HECC_CANME, mbx_mask);
		hecc_write_mbx(priv, mbxno, HECC_CANMID, HECC_CANMID_AME);
		hecc_write_lam(priv, mbxno, HECC_SET_REG);
		hecc_set_bit(priv, HECC_CANMD, mbx_mask);
		hecc_set_bit(priv, HECC_CANME, mbx_mask);
		hecc_set_bit(priv, HECC_CANMIM, mbx_mask);
	}

<<<<<<< HEAD
	/* Prevent message over-write & Enable interrupts */
	hecc_write(priv, HECC_CANOPC, HECC_SET_REG);
=======
	/* Enable tx interrupts */
	hecc_set_bit(priv, HECC_CANMIM, BIT(HECC_MAX_TX_MBOX) - 1);

	/* Prevent message over-write to create a rx fifo, but not for
	 * the lowest priority mailbox, since that allows detecting
	 * overflows instead of the hardware silently dropping the
	 * messages.
	 */
	mbx_mask = ~BIT(HECC_RX_LAST_MBOX);
	hecc_write(priv, HECC_CANOPC, mbx_mask);

	/* Enable interrupts */
>>>>>>> upstream/android-13
	if (priv->use_hecc1int) {
		hecc_write(priv, HECC_CANMIL, HECC_SET_REG);
		hecc_write(priv, HECC_CANGIM, HECC_CANGIM_DEF_MASK |
			HECC_CANGIM_I1EN | HECC_CANGIM_SIL);
	} else {
		hecc_write(priv, HECC_CANMIL, 0);
		hecc_write(priv, HECC_CANGIM,
<<<<<<< HEAD
			HECC_CANGIM_DEF_MASK | HECC_CANGIM_I0EN);
=======
			   HECC_CANGIM_DEF_MASK | HECC_CANGIM_I0EN);
>>>>>>> upstream/android-13
	}
	priv->can.state = CAN_STATE_ERROR_ACTIVE;
}

static void ti_hecc_stop(struct net_device *ndev)
{
	struct ti_hecc_priv *priv = netdev_priv(ndev);

<<<<<<< HEAD
=======
	/* Disable the CPK; stop sending, erroring and acking */
	hecc_set_bit(priv, HECC_CANMC, HECC_CANMC_CCR);

>>>>>>> upstream/android-13
	/* Disable interrupts and disable mailboxes */
	hecc_write(priv, HECC_CANGIM, 0);
	hecc_write(priv, HECC_CANMIM, 0);
	hecc_write(priv, HECC_CANME, 0);
	priv->can.state = CAN_STATE_STOPPED;
}

static int ti_hecc_do_set_mode(struct net_device *ndev, enum can_mode mode)
{
	int ret = 0;

	switch (mode) {
	case CAN_MODE_START:
		ti_hecc_start(ndev);
		netif_wake_queue(ndev);
		break;
	default:
		ret = -EOPNOTSUPP;
		break;
	}

	return ret;
}

static int ti_hecc_get_berr_counter(const struct net_device *ndev,
<<<<<<< HEAD
					struct can_berr_counter *bec)
=======
				    struct can_berr_counter *bec)
>>>>>>> upstream/android-13
{
	struct ti_hecc_priv *priv = netdev_priv(ndev);

	bec->txerr = hecc_read(priv, HECC_CANTEC);
	bec->rxerr = hecc_read(priv, HECC_CANREC);

	return 0;
}

<<<<<<< HEAD
/*
 * ti_hecc_xmit: HECC Transmit
 *
 * The transmit mailboxes start from 0 to HECC_MAX_TX_MBOX. In HECC the
 * priority of the mailbox for tranmission is dependent upon priority setting
=======
/* ti_hecc_xmit: HECC Transmit
 *
 * The transmit mailboxes start from 0 to HECC_MAX_TX_MBOX. In HECC the
 * priority of the mailbox for transmission is dependent upon priority setting
>>>>>>> upstream/android-13
 * field in mailbox registers. The mailbox with highest value in priority field
 * is transmitted first. Only when two mailboxes have the same value in
 * priority field the highest numbered mailbox is transmitted first.
 *
 * To utilize the HECC priority feature as described above we start with the
 * highest numbered mailbox with highest priority level and move on to the next
 * mailbox with the same priority level and so on. Once we loop through all the
 * transmit mailboxes we choose the next priority level (lower) and so on
 * until we reach the lowest priority level on the lowest numbered mailbox
 * when we stop transmission until all mailboxes are transmitted and then
 * restart at highest numbered mailbox with highest priority.
 *
 * Two counters (head and tail) are used to track the next mailbox to transmit
 * and to track the echo buffer for already transmitted mailbox. The queue
 * is stopped when all the mailboxes are busy or when there is a priority
 * value roll-over happens.
 */
static netdev_tx_t ti_hecc_xmit(struct sk_buff *skb, struct net_device *ndev)
{
	struct ti_hecc_priv *priv = netdev_priv(ndev);
	struct can_frame *cf = (struct can_frame *)skb->data;
	u32 mbxno, mbx_mask, data;
	unsigned long flags;

	if (can_dropped_invalid_skb(ndev, skb))
		return NETDEV_TX_OK;

	mbxno = get_tx_head_mb(priv);
	mbx_mask = BIT(mbxno);
	spin_lock_irqsave(&priv->mbx_lock, flags);
	if (unlikely(hecc_read(priv, HECC_CANME) & mbx_mask)) {
		spin_unlock_irqrestore(&priv->mbx_lock, flags);
		netif_stop_queue(ndev);
		netdev_err(priv->ndev,
<<<<<<< HEAD
			"BUG: TX mbx not ready tx_head=%08X, tx_tail=%08X\n",
			priv->tx_head, priv->tx_tail);
=======
			   "BUG: TX mbx not ready tx_head=%08X, tx_tail=%08X\n",
			   priv->tx_head, priv->tx_tail);
>>>>>>> upstream/android-13
		return NETDEV_TX_BUSY;
	}
	spin_unlock_irqrestore(&priv->mbx_lock, flags);

	/* Prepare mailbox for transmission */
<<<<<<< HEAD
	data = cf->can_dlc | (get_tx_head_prio(priv) << 8);
=======
	data = cf->len | (get_tx_head_prio(priv) << 8);
>>>>>>> upstream/android-13
	if (cf->can_id & CAN_RTR_FLAG) /* Remote transmission request */
		data |= HECC_CANMCF_RTR;
	hecc_write_mbx(priv, mbxno, HECC_CANMCF, data);

	if (cf->can_id & CAN_EFF_FLAG) /* Extended frame format */
		data = (cf->can_id & CAN_EFF_MASK) | HECC_CANMID_IDE;
	else /* Standard frame format */
		data = (cf->can_id & CAN_SFF_MASK) << 18;
	hecc_write_mbx(priv, mbxno, HECC_CANMID, data);
	hecc_write_mbx(priv, mbxno, HECC_CANMDL,
<<<<<<< HEAD
		be32_to_cpu(*(__be32 *)(cf->data)));
	if (cf->can_dlc > 4)
		hecc_write_mbx(priv, mbxno, HECC_CANMDH,
			be32_to_cpu(*(__be32 *)(cf->data + 4)));
	else
		*(u32 *)(cf->data + 4) = 0;
	can_put_echo_skb(skb, ndev, mbxno);
=======
		       be32_to_cpu(*(__be32 *)(cf->data)));
	if (cf->len > 4)
		hecc_write_mbx(priv, mbxno, HECC_CANMDH,
			       be32_to_cpu(*(__be32 *)(cf->data + 4)));
	else
		*(u32 *)(cf->data + 4) = 0;
	can_put_echo_skb(skb, ndev, mbxno, 0);
>>>>>>> upstream/android-13

	spin_lock_irqsave(&priv->mbx_lock, flags);
	--priv->tx_head;
	if ((hecc_read(priv, HECC_CANME) & BIT(get_tx_head_mb(priv))) ||
<<<<<<< HEAD
		(priv->tx_head & HECC_TX_MASK) == HECC_TX_MASK) {
=======
	    (priv->tx_head & HECC_TX_MASK) == HECC_TX_MASK) {
>>>>>>> upstream/android-13
		netif_stop_queue(ndev);
	}
	hecc_set_bit(priv, HECC_CANME, mbx_mask);
	spin_unlock_irqrestore(&priv->mbx_lock, flags);

<<<<<<< HEAD
	hecc_clear_bit(priv, HECC_CANMD, mbx_mask);
	hecc_set_bit(priv, HECC_CANMIM, mbx_mask);
=======
>>>>>>> upstream/android-13
	hecc_write(priv, HECC_CANTRS, mbx_mask);

	return NETDEV_TX_OK;
}

<<<<<<< HEAD
static int ti_hecc_rx_pkt(struct ti_hecc_priv *priv, int mbxno)
{
	struct net_device_stats *stats = &priv->ndev->stats;
	struct can_frame *cf;
	struct sk_buff *skb;
	u32 data, mbx_mask;
	unsigned long flags;

	skb = alloc_can_skb(priv->ndev, &cf);
	if (!skb) {
		if (printk_ratelimit())
			netdev_err(priv->ndev,
				"ti_hecc_rx_pkt: alloc_can_skb() failed\n");
		return -ENOMEM;
	}

	mbx_mask = BIT(mbxno);
=======
static inline
struct ti_hecc_priv *rx_offload_to_priv(struct can_rx_offload *offload)
{
	return container_of(offload, struct ti_hecc_priv, offload);
}

static struct sk_buff *ti_hecc_mailbox_read(struct can_rx_offload *offload,
					    unsigned int mbxno, u32 *timestamp,
					    bool drop)
{
	struct ti_hecc_priv *priv = rx_offload_to_priv(offload);
	struct sk_buff *skb;
	struct can_frame *cf;
	u32 data, mbx_mask;

	mbx_mask = BIT(mbxno);

	if (unlikely(drop)) {
		skb = ERR_PTR(-ENOBUFS);
		goto mark_as_read;
	}

	skb = alloc_can_skb(offload->dev, &cf);
	if (unlikely(!skb)) {
		skb = ERR_PTR(-ENOMEM);
		goto mark_as_read;
	}

>>>>>>> upstream/android-13
	data = hecc_read_mbx(priv, mbxno, HECC_CANMID);
	if (data & HECC_CANMID_IDE)
		cf->can_id = (data & CAN_EFF_MASK) | CAN_EFF_FLAG;
	else
		cf->can_id = (data >> 18) & CAN_SFF_MASK;
<<<<<<< HEAD
	data = hecc_read_mbx(priv, mbxno, HECC_CANMCF);
	if (data & HECC_CANMCF_RTR)
		cf->can_id |= CAN_RTR_FLAG;
	cf->can_dlc = get_can_dlc(data & 0xF);
	data = hecc_read_mbx(priv, mbxno, HECC_CANMDL);
	*(__be32 *)(cf->data) = cpu_to_be32(data);
	if (cf->can_dlc > 4) {
		data = hecc_read_mbx(priv, mbxno, HECC_CANMDH);
		*(__be32 *)(cf->data + 4) = cpu_to_be32(data);
	}
	spin_lock_irqsave(&priv->mbx_lock, flags);
	hecc_clear_bit(priv, HECC_CANME, mbx_mask);
	hecc_write(priv, HECC_CANRMP, mbx_mask);
	/* enable mailbox only if it is part of rx buffer mailboxes */
	if (priv->rx_next < HECC_RX_BUFFER_MBOX)
		hecc_set_bit(priv, HECC_CANME, mbx_mask);
	spin_unlock_irqrestore(&priv->mbx_lock, flags);

	stats->rx_bytes += cf->can_dlc;
	can_led_event(priv->ndev, CAN_LED_EVENT_RX);
	netif_receive_skb(skb);
	stats->rx_packets++;

	return 0;
}

/*
 * ti_hecc_rx_poll - HECC receive pkts
 *
 * The receive mailboxes start from highest numbered mailbox till last xmit
 * mailbox. On CAN frame reception the hardware places the data into highest
 * numbered mailbox that matches the CAN ID filter. Since all receive mailboxes
 * have same filtering (ALL CAN frames) packets will arrive in the highest
 * available RX mailbox and we need to ensure in-order packet reception.
 *
 * To ensure the packets are received in the right order we logically divide
 * the RX mailboxes into main and buffer mailboxes. Packets are received as per
 * mailbox priotity (higher to lower) in the main bank and once it is full we
 * disable further reception into main mailboxes. While the main mailboxes are
 * processed in NAPI, further packets are received in buffer mailboxes.
 *
 * We maintain a RX next mailbox counter to process packets and once all main
 * mailboxe packets are passed to the upper stack we enable all of them but
 * continue to process packets received in buffer mailboxes. With each packet
 * received from buffer mailbox we enable it immediately so as to handle the
 * overflow from higher mailboxes.
 */
static int ti_hecc_rx_poll(struct napi_struct *napi, int quota)
{
	struct net_device *ndev = napi->dev;
	struct ti_hecc_priv *priv = netdev_priv(ndev);
	u32 num_pkts = 0;
	u32 mbx_mask;
	unsigned long pending_pkts, flags;

	if (!netif_running(ndev))
		return 0;

	while ((pending_pkts = hecc_read(priv, HECC_CANRMP)) &&
		num_pkts < quota) {
		mbx_mask = BIT(priv->rx_next); /* next rx mailbox to process */
		if (mbx_mask & pending_pkts) {
			if (ti_hecc_rx_pkt(priv, priv->rx_next) < 0)
				return num_pkts;
			++num_pkts;
		} else if (priv->rx_next > HECC_RX_BUFFER_MBOX) {
			break; /* pkt not received yet */
		}
		--priv->rx_next;
		if (priv->rx_next == HECC_RX_BUFFER_MBOX) {
			/* enable high bank mailboxes */
			spin_lock_irqsave(&priv->mbx_lock, flags);
			mbx_mask = hecc_read(priv, HECC_CANME);
			mbx_mask |= HECC_RX_HIGH_MBOX_MASK;
			hecc_write(priv, HECC_CANME, mbx_mask);
			spin_unlock_irqrestore(&priv->mbx_lock, flags);
		} else if (priv->rx_next == HECC_MAX_TX_MBOX - 1) {
			priv->rx_next = HECC_RX_FIRST_MBOX;
			break;
		}
	}

	/* Enable packet interrupt if all pkts are handled */
	if (hecc_read(priv, HECC_CANRMP) == 0) {
		napi_complete(napi);
		/* Re-enable RX mailbox interrupts */
		mbx_mask = hecc_read(priv, HECC_CANMIM);
		mbx_mask |= HECC_TX_MBOX_MASK;
		hecc_write(priv, HECC_CANMIM, mbx_mask);
	} else {
		/* repoll is done only if whole budget is used */
		num_pkts = quota;
	}

	return num_pkts;
}

static int ti_hecc_error(struct net_device *ndev, int int_status,
	int err_status)
{
	struct ti_hecc_priv *priv = netdev_priv(ndev);
	struct net_device_stats *stats = &ndev->stats;
	struct can_frame *cf;
	struct sk_buff *skb;

	/* propagate the error condition to the can stack */
	skb = alloc_can_err_skb(ndev, &cf);
	if (!skb) {
		if (printk_ratelimit())
			netdev_err(priv->ndev,
				"ti_hecc_error: alloc_can_err_skb() failed\n");
		return -ENOMEM;
	}

	if (int_status & HECC_CANGIF_WLIF) { /* warning level int */
		if ((int_status & HECC_CANGIF_BOIF) == 0) {
			priv->can.state = CAN_STATE_ERROR_WARNING;
			++priv->can.can_stats.error_warning;
			cf->can_id |= CAN_ERR_CRTL;
			if (hecc_read(priv, HECC_CANTEC) > 96)
				cf->data[1] |= CAN_ERR_CRTL_TX_WARNING;
			if (hecc_read(priv, HECC_CANREC) > 96)
				cf->data[1] |= CAN_ERR_CRTL_RX_WARNING;
		}
		hecc_set_bit(priv, HECC_CANES, HECC_CANES_EW);
		netdev_dbg(priv->ndev, "Error Warning interrupt\n");
		hecc_clear_bit(priv, HECC_CANMC, HECC_CANMC_CCR);
	}

	if (int_status & HECC_CANGIF_EPIF) { /* error passive int */
		if ((int_status & HECC_CANGIF_BOIF) == 0) {
			priv->can.state = CAN_STATE_ERROR_PASSIVE;
			++priv->can.can_stats.error_passive;
			cf->can_id |= CAN_ERR_CRTL;
			if (hecc_read(priv, HECC_CANTEC) > 127)
				cf->data[1] |= CAN_ERR_CRTL_TX_PASSIVE;
			if (hecc_read(priv, HECC_CANREC) > 127)
				cf->data[1] |= CAN_ERR_CRTL_RX_PASSIVE;
		}
		hecc_set_bit(priv, HECC_CANES, HECC_CANES_EP);
		netdev_dbg(priv->ndev, "Error passive interrupt\n");
		hecc_clear_bit(priv, HECC_CANMC, HECC_CANMC_CCR);
	}

	/*
	 * Need to check busoff condition in error status register too to
	 * ensure warning interrupts don't hog the system
	 */
	if ((int_status & HECC_CANGIF_BOIF) || (err_status & HECC_CANES_BO)) {
		priv->can.state = CAN_STATE_BUS_OFF;
		cf->can_id |= CAN_ERR_BUSOFF;
		hecc_set_bit(priv, HECC_CANES, HECC_CANES_BO);
		hecc_clear_bit(priv, HECC_CANMC, HECC_CANMC_CCR);
		/* Disable all interrupts in bus-off to avoid int hog */
		hecc_write(priv, HECC_CANGIM, 0);
		++priv->can.can_stats.bus_off;
		can_bus_off(ndev);
	}

	if (err_status & HECC_BUS_ERROR) {
		++priv->can.can_stats.bus_error;
		cf->can_id |= CAN_ERR_BUSERROR | CAN_ERR_PROT;
		if (err_status & HECC_CANES_FE) {
			hecc_set_bit(priv, HECC_CANES, HECC_CANES_FE);
			cf->data[2] |= CAN_ERR_PROT_FORM;
		}
		if (err_status & HECC_CANES_BE) {
			hecc_set_bit(priv, HECC_CANES, HECC_CANES_BE);
			cf->data[2] |= CAN_ERR_PROT_BIT;
		}
		if (err_status & HECC_CANES_SE) {
			hecc_set_bit(priv, HECC_CANES, HECC_CANES_SE);
			cf->data[2] |= CAN_ERR_PROT_STUFF;
		}
		if (err_status & HECC_CANES_CRCE) {
			hecc_set_bit(priv, HECC_CANES, HECC_CANES_CRCE);
			cf->data[3] = CAN_ERR_PROT_LOC_CRC_SEQ;
		}
		if (err_status & HECC_CANES_ACKE) {
			hecc_set_bit(priv, HECC_CANES, HECC_CANES_ACKE);
			cf->data[3] = CAN_ERR_PROT_LOC_ACK;
		}
	}

	stats->rx_packets++;
	stats->rx_bytes += cf->can_dlc;
	netif_rx(skb);
=======

	data = hecc_read_mbx(priv, mbxno, HECC_CANMCF);
	if (data & HECC_CANMCF_RTR)
		cf->can_id |= CAN_RTR_FLAG;
	cf->len = can_cc_dlc2len(data & 0xF);

	data = hecc_read_mbx(priv, mbxno, HECC_CANMDL);
	*(__be32 *)(cf->data) = cpu_to_be32(data);
	if (cf->len > 4) {
		data = hecc_read_mbx(priv, mbxno, HECC_CANMDH);
		*(__be32 *)(cf->data + 4) = cpu_to_be32(data);
	}

	*timestamp = hecc_read_stamp(priv, mbxno);

	/* Check for FIFO overrun.
	 *
	 * All but the last RX mailbox have activated overwrite
	 * protection. So skip check for overrun, if we're not
	 * handling the last RX mailbox.
	 *
	 * As the overwrite protection for the last RX mailbox is
	 * disabled, the CAN core might update while we're reading
	 * it. This means the skb might be inconsistent.
	 *
	 * Return an error to let rx-offload discard this CAN frame.
	 */
	if (unlikely(mbxno == HECC_RX_LAST_MBOX &&
		     hecc_read(priv, HECC_CANRML) & mbx_mask))
		skb = ERR_PTR(-ENOBUFS);

 mark_as_read:
	hecc_write(priv, HECC_CANRMP, mbx_mask);

	return skb;
}

static int ti_hecc_error(struct net_device *ndev, int int_status,
			 int err_status)
{
	struct ti_hecc_priv *priv = netdev_priv(ndev);
	struct can_frame *cf;
	struct sk_buff *skb;
	u32 timestamp;
	int err;

	if (err_status & HECC_BUS_ERROR) {
		/* propagate the error condition to the can stack */
		skb = alloc_can_err_skb(ndev, &cf);
		if (!skb) {
			if (net_ratelimit())
				netdev_err(priv->ndev,
					   "%s: alloc_can_err_skb() failed\n",
					   __func__);
			return -ENOMEM;
		}

		++priv->can.can_stats.bus_error;
		cf->can_id |= CAN_ERR_BUSERROR | CAN_ERR_PROT;
		if (err_status & HECC_CANES_FE)
			cf->data[2] |= CAN_ERR_PROT_FORM;
		if (err_status & HECC_CANES_BE)
			cf->data[2] |= CAN_ERR_PROT_BIT;
		if (err_status & HECC_CANES_SE)
			cf->data[2] |= CAN_ERR_PROT_STUFF;
		if (err_status & HECC_CANES_CRCE)
			cf->data[3] = CAN_ERR_PROT_LOC_CRC_SEQ;
		if (err_status & HECC_CANES_ACKE)
			cf->data[3] = CAN_ERR_PROT_LOC_ACK;

		timestamp = hecc_read(priv, HECC_CANLNT);
		err = can_rx_offload_queue_sorted(&priv->offload, skb,
						  timestamp);
		if (err)
			ndev->stats.rx_fifo_errors++;
	}

	hecc_write(priv, HECC_CANES, HECC_CANES_FLAGS);
>>>>>>> upstream/android-13

	return 0;
}

<<<<<<< HEAD
=======
static void ti_hecc_change_state(struct net_device *ndev,
				 enum can_state rx_state,
				 enum can_state tx_state)
{
	struct ti_hecc_priv *priv = netdev_priv(ndev);
	struct can_frame *cf;
	struct sk_buff *skb;
	u32 timestamp;
	int err;

	skb = alloc_can_err_skb(priv->ndev, &cf);
	if (unlikely(!skb)) {
		priv->can.state = max(tx_state, rx_state);
		return;
	}

	can_change_state(priv->ndev, cf, tx_state, rx_state);

	if (max(tx_state, rx_state) != CAN_STATE_BUS_OFF) {
		cf->data[6] = hecc_read(priv, HECC_CANTEC);
		cf->data[7] = hecc_read(priv, HECC_CANREC);
	}

	timestamp = hecc_read(priv, HECC_CANLNT);
	err = can_rx_offload_queue_sorted(&priv->offload, skb, timestamp);
	if (err)
		ndev->stats.rx_fifo_errors++;
}

>>>>>>> upstream/android-13
static irqreturn_t ti_hecc_interrupt(int irq, void *dev_id)
{
	struct net_device *ndev = (struct net_device *)dev_id;
	struct ti_hecc_priv *priv = netdev_priv(ndev);
	struct net_device_stats *stats = &ndev->stats;
<<<<<<< HEAD
	u32 mbxno, mbx_mask, int_status, err_status;
	unsigned long ack, flags;

	int_status = hecc_read(priv,
		(priv->use_hecc1int) ? HECC_CANGIF1 : HECC_CANGIF0);
=======
	u32 mbxno, mbx_mask, int_status, err_status, stamp;
	unsigned long flags, rx_pending;
	u32 handled = 0;

	int_status = hecc_read(priv,
			       priv->use_hecc1int ?
			       HECC_CANGIF1 : HECC_CANGIF0);
>>>>>>> upstream/android-13

	if (!int_status)
		return IRQ_NONE;

	err_status = hecc_read(priv, HECC_CANES);
<<<<<<< HEAD
	if (err_status & (HECC_BUS_ERROR | HECC_CANES_BO |
		HECC_CANES_EP | HECC_CANES_EW))
			ti_hecc_error(ndev, int_status, err_status);
=======
	if (unlikely(err_status & HECC_CANES_FLAGS))
		ti_hecc_error(ndev, int_status, err_status);

	if (unlikely(int_status & HECC_CANGIM_DEF_MASK)) {
		enum can_state rx_state, tx_state;
		u32 rec = hecc_read(priv, HECC_CANREC);
		u32 tec = hecc_read(priv, HECC_CANTEC);

		if (int_status & HECC_CANGIF_WLIF) {
			handled |= HECC_CANGIF_WLIF;
			rx_state = rec >= tec ? CAN_STATE_ERROR_WARNING : 0;
			tx_state = rec <= tec ? CAN_STATE_ERROR_WARNING : 0;
			netdev_dbg(priv->ndev, "Error Warning interrupt\n");
			ti_hecc_change_state(ndev, rx_state, tx_state);
		}

		if (int_status & HECC_CANGIF_EPIF) {
			handled |= HECC_CANGIF_EPIF;
			rx_state = rec >= tec ? CAN_STATE_ERROR_PASSIVE : 0;
			tx_state = rec <= tec ? CAN_STATE_ERROR_PASSIVE : 0;
			netdev_dbg(priv->ndev, "Error passive interrupt\n");
			ti_hecc_change_state(ndev, rx_state, tx_state);
		}

		if (int_status & HECC_CANGIF_BOIF) {
			handled |= HECC_CANGIF_BOIF;
			rx_state = CAN_STATE_BUS_OFF;
			tx_state = CAN_STATE_BUS_OFF;
			netdev_dbg(priv->ndev, "Bus off interrupt\n");

			/* Disable all interrupts */
			hecc_write(priv, HECC_CANGIM, 0);
			can_bus_off(ndev);
			ti_hecc_change_state(ndev, rx_state, tx_state);
		}
	} else if (unlikely(priv->can.state != CAN_STATE_ERROR_ACTIVE)) {
		enum can_state new_state, tx_state, rx_state;
		u32 rec = hecc_read(priv, HECC_CANREC);
		u32 tec = hecc_read(priv, HECC_CANTEC);

		if (rec >= 128 || tec >= 128)
			new_state = CAN_STATE_ERROR_PASSIVE;
		else if (rec >= 96 || tec >= 96)
			new_state = CAN_STATE_ERROR_WARNING;
		else
			new_state = CAN_STATE_ERROR_ACTIVE;

		if (new_state < priv->can.state) {
			rx_state = rec >= tec ? new_state : 0;
			tx_state = rec <= tec ? new_state : 0;
			ti_hecc_change_state(ndev, rx_state, tx_state);
		}
	}
>>>>>>> upstream/android-13

	if (int_status & HECC_CANGIF_GMIF) {
		while (priv->tx_tail - priv->tx_head > 0) {
			mbxno = get_tx_tail_mb(priv);
			mbx_mask = BIT(mbxno);
			if (!(mbx_mask & hecc_read(priv, HECC_CANTA)))
				break;
<<<<<<< HEAD
			hecc_clear_bit(priv, HECC_CANMIM, mbx_mask);
=======
>>>>>>> upstream/android-13
			hecc_write(priv, HECC_CANTA, mbx_mask);
			spin_lock_irqsave(&priv->mbx_lock, flags);
			hecc_clear_bit(priv, HECC_CANME, mbx_mask);
			spin_unlock_irqrestore(&priv->mbx_lock, flags);
<<<<<<< HEAD
			stats->tx_bytes += hecc_read_mbx(priv, mbxno,
						HECC_CANMCF) & 0xF;
			stats->tx_packets++;
			can_led_event(ndev, CAN_LED_EVENT_TX);
			can_get_echo_skb(ndev, mbxno);
=======
			stamp = hecc_read_stamp(priv, mbxno);
			stats->tx_bytes +=
				can_rx_offload_get_echo_skb(&priv->offload,
							    mbxno, stamp, NULL);
			stats->tx_packets++;
			can_led_event(ndev, CAN_LED_EVENT_TX);
>>>>>>> upstream/android-13
			--priv->tx_tail;
		}

		/* restart queue if wrap-up or if queue stalled on last pkt */
<<<<<<< HEAD
		if (((priv->tx_head == priv->tx_tail) &&
		((priv->tx_head & HECC_TX_MASK) != HECC_TX_MASK)) ||
		(((priv->tx_tail & HECC_TX_MASK) == HECC_TX_MASK) &&
		((priv->tx_head & HECC_TX_MASK) == HECC_TX_MASK)))
			netif_wake_queue(ndev);

		/* Disable RX mailbox interrupts and let NAPI reenable them */
		if (hecc_read(priv, HECC_CANRMP)) {
			ack = hecc_read(priv, HECC_CANMIM);
			ack &= BIT(HECC_MAX_TX_MBOX) - 1;
			hecc_write(priv, HECC_CANMIM, ack);
			napi_schedule(&priv->napi);
=======
		if ((priv->tx_head == priv->tx_tail &&
		     ((priv->tx_head & HECC_TX_MASK) != HECC_TX_MASK)) ||
		    (((priv->tx_tail & HECC_TX_MASK) == HECC_TX_MASK) &&
		     ((priv->tx_head & HECC_TX_MASK) == HECC_TX_MASK)))
			netif_wake_queue(ndev);

		/* offload RX mailboxes and let NAPI deliver them */
		while ((rx_pending = hecc_read(priv, HECC_CANRMP))) {
			can_rx_offload_irq_offload_timestamp(&priv->offload,
							     rx_pending);
>>>>>>> upstream/android-13
		}
	}

	/* clear all interrupt conditions - read back to avoid spurious ints */
	if (priv->use_hecc1int) {
<<<<<<< HEAD
		hecc_write(priv, HECC_CANGIF1, HECC_SET_REG);
		int_status = hecc_read(priv, HECC_CANGIF1);
	} else {
		hecc_write(priv, HECC_CANGIF0, HECC_SET_REG);
		int_status = hecc_read(priv, HECC_CANGIF0);
	}

=======
		hecc_write(priv, HECC_CANGIF1, handled);
		int_status = hecc_read(priv, HECC_CANGIF1);
	} else {
		hecc_write(priv, HECC_CANGIF0, handled);
		int_status = hecc_read(priv, HECC_CANGIF0);
	}

	can_rx_offload_irq_finish(&priv->offload);

>>>>>>> upstream/android-13
	return IRQ_HANDLED;
}

static int ti_hecc_open(struct net_device *ndev)
{
	struct ti_hecc_priv *priv = netdev_priv(ndev);
	int err;

	err = request_irq(ndev->irq, ti_hecc_interrupt, IRQF_SHARED,
<<<<<<< HEAD
			ndev->name, ndev);
=======
			  ndev->name, ndev);
>>>>>>> upstream/android-13
	if (err) {
		netdev_err(ndev, "error requesting interrupt\n");
		return err;
	}

	ti_hecc_transceiver_switch(priv, 1);

	/* Open common can device */
	err = open_candev(ndev);
	if (err) {
		netdev_err(ndev, "open_candev() failed %d\n", err);
		ti_hecc_transceiver_switch(priv, 0);
		free_irq(ndev->irq, ndev);
		return err;
	}

	can_led_event(ndev, CAN_LED_EVENT_OPEN);

	ti_hecc_start(ndev);
<<<<<<< HEAD
	napi_enable(&priv->napi);
=======
	can_rx_offload_enable(&priv->offload);
>>>>>>> upstream/android-13
	netif_start_queue(ndev);

	return 0;
}

static int ti_hecc_close(struct net_device *ndev)
{
	struct ti_hecc_priv *priv = netdev_priv(ndev);

	netif_stop_queue(ndev);
<<<<<<< HEAD
	napi_disable(&priv->napi);
=======
	can_rx_offload_disable(&priv->offload);
>>>>>>> upstream/android-13
	ti_hecc_stop(ndev);
	free_irq(ndev->irq, ndev);
	close_candev(ndev);
	ti_hecc_transceiver_switch(priv, 0);

	can_led_event(ndev, CAN_LED_EVENT_STOP);

	return 0;
}

static const struct net_device_ops ti_hecc_netdev_ops = {
	.ndo_open		= ti_hecc_open,
	.ndo_stop		= ti_hecc_close,
	.ndo_start_xmit		= ti_hecc_xmit,
	.ndo_change_mtu		= can_change_mtu,
};

static const struct of_device_id ti_hecc_dt_ids[] = {
	{
		.compatible = "ti,am3517-hecc",
	},
	{ }
};
MODULE_DEVICE_TABLE(of, ti_hecc_dt_ids);

static int ti_hecc_probe(struct platform_device *pdev)
{
	struct net_device *ndev = (struct net_device *)0;
	struct ti_hecc_priv *priv;
	struct device_node *np = pdev->dev.of_node;
<<<<<<< HEAD
	struct resource *res, *irq;
=======
	struct resource *irq;
>>>>>>> upstream/android-13
	struct regulator *reg_xceiver;
	int err = -ENODEV;

	if (!IS_ENABLED(CONFIG_OF) || !np)
		return -EINVAL;

	reg_xceiver = devm_regulator_get(&pdev->dev, "xceiver");
	if (PTR_ERR(reg_xceiver) == -EPROBE_DEFER)
		return -EPROBE_DEFER;
	else if (IS_ERR(reg_xceiver))
		reg_xceiver = NULL;

	ndev = alloc_candev(sizeof(struct ti_hecc_priv), HECC_MAX_TX_MBOX);
	if (!ndev) {
		dev_err(&pdev->dev, "alloc_candev failed\n");
		return -ENOMEM;
	}
	priv = netdev_priv(ndev);

	/* handle hecc memory */
<<<<<<< HEAD
	res = platform_get_resource_byname(pdev, IORESOURCE_MEM, "hecc");
	if (!res) {
		dev_err(&pdev->dev, "can't get IORESOURCE_MEM hecc\n");
		return -EINVAL;
	}

	priv->base = devm_ioremap_resource(&pdev->dev, res);
=======
	priv->base = devm_platform_ioremap_resource_byname(pdev, "hecc");
>>>>>>> upstream/android-13
	if (IS_ERR(priv->base)) {
		dev_err(&pdev->dev, "hecc ioremap failed\n");
		err = PTR_ERR(priv->base);
		goto probe_exit_candev;
	}

	/* handle hecc-ram memory */
<<<<<<< HEAD
	res = platform_get_resource_byname(pdev, IORESOURCE_MEM, "hecc-ram");
	if (!res) {
		dev_err(&pdev->dev, "can't get IORESOURCE_MEM hecc-ram\n");
		return -EINVAL;
	}

	priv->hecc_ram = devm_ioremap_resource(&pdev->dev, res);
=======
	priv->hecc_ram = devm_platform_ioremap_resource_byname(pdev,
							       "hecc-ram");
>>>>>>> upstream/android-13
	if (IS_ERR(priv->hecc_ram)) {
		dev_err(&pdev->dev, "hecc-ram ioremap failed\n");
		err = PTR_ERR(priv->hecc_ram);
		goto probe_exit_candev;
	}

	/* handle mbx memory */
<<<<<<< HEAD
	res = platform_get_resource_byname(pdev, IORESOURCE_MEM, "mbx");
	if (!res) {
		dev_err(&pdev->dev, "can't get IORESOURCE_MEM mbx\n");
		return -EINVAL;
	}

	priv->mbx = devm_ioremap_resource(&pdev->dev, res);
=======
	priv->mbx = devm_platform_ioremap_resource_byname(pdev, "mbx");
>>>>>>> upstream/android-13
	if (IS_ERR(priv->mbx)) {
		dev_err(&pdev->dev, "mbx ioremap failed\n");
		err = PTR_ERR(priv->mbx);
		goto probe_exit_candev;
	}

	irq = platform_get_resource(pdev, IORESOURCE_IRQ, 0);
	if (!irq) {
		dev_err(&pdev->dev, "No irq resource\n");
		goto probe_exit_candev;
	}

	priv->ndev = ndev;
	priv->reg_xceiver = reg_xceiver;
	priv->use_hecc1int = of_property_read_bool(np, "ti,use-hecc1int");

	priv->can.bittiming_const = &ti_hecc_bittiming_const;
	priv->can.do_set_mode = ti_hecc_do_set_mode;
	priv->can.do_get_berr_counter = ti_hecc_get_berr_counter;
	priv->can.ctrlmode_supported = CAN_CTRLMODE_3_SAMPLES;

	spin_lock_init(&priv->mbx_lock);
	ndev->irq = irq->start;
	ndev->flags |= IFF_ECHO;
	platform_set_drvdata(pdev, ndev);
	SET_NETDEV_DEV(ndev, &pdev->dev);
	ndev->netdev_ops = &ti_hecc_netdev_ops;

	priv->clk = clk_get(&pdev->dev, "hecc_ck");
	if (IS_ERR(priv->clk)) {
		dev_err(&pdev->dev, "No clock available\n");
		err = PTR_ERR(priv->clk);
		priv->clk = NULL;
		goto probe_exit_candev;
	}
	priv->can.clock.freq = clk_get_rate(priv->clk);
<<<<<<< HEAD
	netif_napi_add(ndev, &priv->napi, ti_hecc_rx_poll,
		HECC_DEF_NAPI_WEIGHT);
=======
>>>>>>> upstream/android-13

	err = clk_prepare_enable(priv->clk);
	if (err) {
		dev_err(&pdev->dev, "clk_prepare_enable() failed\n");
<<<<<<< HEAD
		goto probe_exit_clk;
=======
		goto probe_exit_release_clk;
	}

	priv->offload.mailbox_read = ti_hecc_mailbox_read;
	priv->offload.mb_first = HECC_RX_FIRST_MBOX;
	priv->offload.mb_last = HECC_RX_LAST_MBOX;
	err = can_rx_offload_add_timestamp(ndev, &priv->offload);
	if (err) {
		dev_err(&pdev->dev, "can_rx_offload_add_timestamp() failed\n");
		goto probe_exit_disable_clk;
>>>>>>> upstream/android-13
	}

	err = register_candev(ndev);
	if (err) {
		dev_err(&pdev->dev, "register_candev() failed\n");
<<<<<<< HEAD
		goto probe_exit_clk;
=======
		goto probe_exit_offload;
>>>>>>> upstream/android-13
	}

	devm_can_led_init(ndev);

	dev_info(&pdev->dev, "device registered (reg_base=%p, irq=%u)\n",
<<<<<<< HEAD
		priv->base, (u32) ndev->irq);

	return 0;

probe_exit_clk:
=======
		 priv->base, (u32)ndev->irq);

	return 0;

probe_exit_offload:
	can_rx_offload_del(&priv->offload);
probe_exit_disable_clk:
	clk_disable_unprepare(priv->clk);
probe_exit_release_clk:
>>>>>>> upstream/android-13
	clk_put(priv->clk);
probe_exit_candev:
	free_candev(ndev);

	return err;
}

static int ti_hecc_remove(struct platform_device *pdev)
{
	struct net_device *ndev = platform_get_drvdata(pdev);
	struct ti_hecc_priv *priv = netdev_priv(ndev);

	unregister_candev(ndev);
	clk_disable_unprepare(priv->clk);
	clk_put(priv->clk);
<<<<<<< HEAD
=======
	can_rx_offload_del(&priv->offload);
>>>>>>> upstream/android-13
	free_candev(ndev);

	return 0;
}

#ifdef CONFIG_PM
static int ti_hecc_suspend(struct platform_device *pdev, pm_message_t state)
{
	struct net_device *dev = platform_get_drvdata(pdev);
	struct ti_hecc_priv *priv = netdev_priv(dev);

	if (netif_running(dev)) {
		netif_stop_queue(dev);
		netif_device_detach(dev);
	}

	hecc_set_bit(priv, HECC_CANMC, HECC_CANMC_PDR);
	priv->can.state = CAN_STATE_SLEEPING;

	clk_disable_unprepare(priv->clk);

	return 0;
}

static int ti_hecc_resume(struct platform_device *pdev)
{
	struct net_device *dev = platform_get_drvdata(pdev);
	struct ti_hecc_priv *priv = netdev_priv(dev);
	int err;

	err = clk_prepare_enable(priv->clk);
	if (err)
		return err;

	hecc_clear_bit(priv, HECC_CANMC, HECC_CANMC_PDR);
	priv->can.state = CAN_STATE_ERROR_ACTIVE;

	if (netif_running(dev)) {
		netif_device_attach(dev);
		netif_start_queue(dev);
	}

	return 0;
}
#else
#define ti_hecc_suspend NULL
#define ti_hecc_resume NULL
#endif

/* TI HECC netdevice driver: platform driver structure */
static struct platform_driver ti_hecc_driver = {
	.driver = {
		.name    = DRV_NAME,
		.of_match_table = ti_hecc_dt_ids,
	},
	.probe = ti_hecc_probe,
	.remove = ti_hecc_remove,
	.suspend = ti_hecc_suspend,
	.resume = ti_hecc_resume,
};

module_platform_driver(ti_hecc_driver);

MODULE_AUTHOR("Anant Gole <anantgole@ti.com>");
MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION(DRV_DESC);
MODULE_ALIAS("platform:" DRV_NAME);
