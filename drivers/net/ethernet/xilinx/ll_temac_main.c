<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> upstream/android-13
/*
 * Driver for Xilinx TEMAC Ethernet device
 *
 * Copyright (c) 2008 Nissin Systems Co., Ltd.,  Yoshio Kashiwagi
 * Copyright (c) 2005-2008 DLA Systems,  David H. Lynch Jr. <dhlii@dlasys.net>
 * Copyright (c) 2008-2009 Secret Lab Technologies Ltd.
 *
 * This is a driver for the Xilinx ll_temac ipcore which is often used
 * in the Virtex and Spartan series of chips.
 *
 * Notes:
 * - The ll_temac hardware uses indirect access for many of the TEMAC
 *   registers, include the MDIO bus.  However, indirect access to MDIO
 *   registers take considerably more clock cycles than to TEMAC registers.
 *   MDIO accesses are long, so threads doing them should probably sleep
 *   rather than busywait.  However, since only one indirect access can be
 *   in progress at any given time, that means that *all* indirect accesses
 *   could end up sleeping (to wait for an MDIO access to complete).
 *   Fortunately none of the indirect accesses are on the 'hot' path for tx
 *   or rx, so this should be okay.
 *
 * TODO:
 * - Factor out locallink DMA code into separate driver
<<<<<<< HEAD
 * - Fix multicast assignment.
=======
>>>>>>> upstream/android-13
 * - Fix support for hardware checksumming.
 * - Testing.  Lots and lots of testing.
 *
 */

#include <linux/delay.h>
#include <linux/etherdevice.h>
#include <linux/mii.h>
#include <linux/module.h>
#include <linux/mutex.h>
#include <linux/netdevice.h>
<<<<<<< HEAD
=======
#include <linux/if_ether.h>
>>>>>>> upstream/android-13
#include <linux/of.h>
#include <linux/of_device.h>
#include <linux/of_irq.h>
#include <linux/of_mdio.h>
#include <linux/of_net.h>
#include <linux/of_platform.h>
#include <linux/of_address.h>
#include <linux/skbuff.h>
#include <linux/spinlock.h>
#include <linux/tcp.h>      /* needed for sizeof(tcphdr) */
#include <linux/udp.h>      /* needed for sizeof(udphdr) */
#include <linux/phy.h>
#include <linux/in.h>
#include <linux/io.h>
#include <linux/ip.h>
#include <linux/slab.h>
#include <linux/interrupt.h>
<<<<<<< HEAD
#include <linux/dma-mapping.h>

#include "ll_temac.h"

#define TX_BD_NUM   64
#define RX_BD_NUM   128
=======
#include <linux/workqueue.h>
#include <linux/dma-mapping.h>
#include <linux/processor.h>
#include <linux/platform_data/xilinx-ll-temac.h>

#include "ll_temac.h"

/* Descriptors defines for Tx and Rx DMA */
#define TX_BD_NUM_DEFAULT		64
#define RX_BD_NUM_DEFAULT		1024
#define TX_BD_NUM_MAX			4096
#define RX_BD_NUM_MAX			4096
>>>>>>> upstream/android-13

/* ---------------------------------------------------------------------
 * Low level register access functions
 */

<<<<<<< HEAD
u32 temac_ior(struct temac_local *lp, int offset)
{
	return in_be32(lp->regs + offset);
}

void temac_iow(struct temac_local *lp, int offset, u32 value)
{
	out_be32(lp->regs + offset, value);
}

int temac_indirect_busywait(struct temac_local *lp)
{
	unsigned long end = jiffies + 2;

	while (!(temac_ior(lp, XTE_RDY0_OFFSET) & XTE_RDY0_HARD_ACS_RDY_MASK)) {
		if (time_before_eq(end, jiffies)) {
			WARN_ON(1);
			return -ETIMEDOUT;
		}
		msleep(1);
	}
	return 0;
}

/**
 * temac_indirect_in32
 *
 * lp->indirect_mutex must be held when calling this function
 */
u32 temac_indirect_in32(struct temac_local *lp, int reg)
{
	u32 val;

	if (temac_indirect_busywait(lp))
		return -ETIMEDOUT;
	temac_iow(lp, XTE_CTL0_OFFSET, reg);
	if (temac_indirect_busywait(lp))
		return -ETIMEDOUT;
	val = temac_ior(lp, XTE_LSW0_OFFSET);

	return val;
}

/**
 * temac_indirect_out32
 *
 * lp->indirect_mutex must be held when calling this function
 */
void temac_indirect_out32(struct temac_local *lp, int reg, u32 value)
{
	if (temac_indirect_busywait(lp))
		return;
	temac_iow(lp, XTE_LSW0_OFFSET, value);
	temac_iow(lp, XTE_CTL0_OFFSET, CNTLREG_WRITE_ENABLE_MASK | reg);
	temac_indirect_busywait(lp);
}

/**
 * temac_dma_in32 - Memory mapped DMA read, this function expects a
 * register input that is based on DCR word addresses which
 * are then converted to memory mapped byte addresses
 */
static u32 temac_dma_in32(struct temac_local *lp, int reg)
{
	return in_be32(lp->sdma_regs + (reg << 2));
}

/**
 * temac_dma_out32 - Memory mapped DMA read, this function expects a
 * register input that is based on DCR word addresses which
 * are then converted to memory mapped byte addresses
 */
static void temac_dma_out32(struct temac_local *lp, int reg, u32 value)
{
	out_be32(lp->sdma_regs + (reg << 2), value);
=======
static u32 _temac_ior_be(struct temac_local *lp, int offset)
{
	return ioread32be(lp->regs + offset);
}

static void _temac_iow_be(struct temac_local *lp, int offset, u32 value)
{
	return iowrite32be(value, lp->regs + offset);
}

static u32 _temac_ior_le(struct temac_local *lp, int offset)
{
	return ioread32(lp->regs + offset);
}

static void _temac_iow_le(struct temac_local *lp, int offset, u32 value)
{
	return iowrite32(value, lp->regs + offset);
}

static bool hard_acs_rdy(struct temac_local *lp)
{
	return temac_ior(lp, XTE_RDY0_OFFSET) & XTE_RDY0_HARD_ACS_RDY_MASK;
}

static bool hard_acs_rdy_or_timeout(struct temac_local *lp, ktime_t timeout)
{
	ktime_t cur = ktime_get();

	return hard_acs_rdy(lp) || ktime_after(cur, timeout);
}

/* Poll for maximum 20 ms.  This is similar to the 2 jiffies @ 100 Hz
 * that was used before, and should cover MDIO bus speed down to 3200
 * Hz.
 */
#define HARD_ACS_RDY_POLL_NS (20 * NSEC_PER_MSEC)

/*
 * temac_indirect_busywait - Wait for current indirect register access
 * to complete.
 */
int temac_indirect_busywait(struct temac_local *lp)
{
	ktime_t timeout = ktime_add_ns(ktime_get(), HARD_ACS_RDY_POLL_NS);

	spin_until_cond(hard_acs_rdy_or_timeout(lp, timeout));
	if (WARN_ON(!hard_acs_rdy(lp)))
		return -ETIMEDOUT;
	else
		return 0;
}

/*
 * temac_indirect_in32 - Indirect register read access.  This function
 * must be called without lp->indirect_lock being held.
 */
u32 temac_indirect_in32(struct temac_local *lp, int reg)
{
	unsigned long flags;
	int val;

	spin_lock_irqsave(lp->indirect_lock, flags);
	val = temac_indirect_in32_locked(lp, reg);
	spin_unlock_irqrestore(lp->indirect_lock, flags);
	return val;
}

/*
 * temac_indirect_in32_locked - Indirect register read access.  This
 * function must be called with lp->indirect_lock being held.  Use
 * this together with spin_lock_irqsave/spin_lock_irqrestore to avoid
 * repeated lock/unlock and to ensure uninterrupted access to indirect
 * registers.
 */
u32 temac_indirect_in32_locked(struct temac_local *lp, int reg)
{
	/* This initial wait should normally not spin, as we always
	 * try to wait for indirect access to complete before
	 * releasing the indirect_lock.
	 */
	if (WARN_ON(temac_indirect_busywait(lp)))
		return -ETIMEDOUT;
	/* Initiate read from indirect register */
	temac_iow(lp, XTE_CTL0_OFFSET, reg);
	/* Wait for indirect register access to complete.  We really
	 * should not see timeouts, and could even end up causing
	 * problem for following indirect access, so let's make a bit
	 * of WARN noise.
	 */
	if (WARN_ON(temac_indirect_busywait(lp)))
		return -ETIMEDOUT;
	/* Value is ready now */
	return temac_ior(lp, XTE_LSW0_OFFSET);
}

/*
 * temac_indirect_out32 - Indirect register write access.  This function
 * must be called without lp->indirect_lock being held.
 */
void temac_indirect_out32(struct temac_local *lp, int reg, u32 value)
{
	unsigned long flags;

	spin_lock_irqsave(lp->indirect_lock, flags);
	temac_indirect_out32_locked(lp, reg, value);
	spin_unlock_irqrestore(lp->indirect_lock, flags);
}

/*
 * temac_indirect_out32_locked - Indirect register write access.  This
 * function must be called with lp->indirect_lock being held.  Use
 * this together with spin_lock_irqsave/spin_lock_irqrestore to avoid
 * repeated lock/unlock and to ensure uninterrupted access to indirect
 * registers.
 */
void temac_indirect_out32_locked(struct temac_local *lp, int reg, u32 value)
{
	/* As in temac_indirect_in32_locked(), we should normally not
	 * spin here.  And if it happens, we actually end up silently
	 * ignoring the write request.  Ouch.
	 */
	if (WARN_ON(temac_indirect_busywait(lp)))
		return;
	/* Initiate write to indirect register */
	temac_iow(lp, XTE_LSW0_OFFSET, value);
	temac_iow(lp, XTE_CTL0_OFFSET, CNTLREG_WRITE_ENABLE_MASK | reg);
	/* As in temac_indirect_in32_locked(), we should not see timeouts
	 * here.  And if it happens, we continue before the write has
	 * completed.  Not good.
	 */
	WARN_ON(temac_indirect_busywait(lp));
}

/*
 * temac_dma_in32_* - Memory mapped DMA read, these function expects a
 * register input that is based on DCR word addresses which are then
 * converted to memory mapped byte addresses.  To be assigned to
 * lp->dma_in32.
 */
static u32 temac_dma_in32_be(struct temac_local *lp, int reg)
{
	return ioread32be(lp->sdma_regs + (reg << 2));
}

static u32 temac_dma_in32_le(struct temac_local *lp, int reg)
{
	return ioread32(lp->sdma_regs + (reg << 2));
}

/*
 * temac_dma_out32_* - Memory mapped DMA read, these function expects
 * a register input that is based on DCR word addresses which are then
 * converted to memory mapped byte addresses.  To be assigned to
 * lp->dma_out32.
 */
static void temac_dma_out32_be(struct temac_local *lp, int reg, u32 value)
{
	iowrite32be(value, lp->sdma_regs + (reg << 2));
}

static void temac_dma_out32_le(struct temac_local *lp, int reg, u32 value)
{
	iowrite32(value, lp->sdma_regs + (reg << 2));
>>>>>>> upstream/android-13
}

/* DMA register access functions can be DCR based or memory mapped.
 * The PowerPC 440 is DCR based, the PowerPC 405 and MicroBlaze are both
 * memory mapped.
 */
#ifdef CONFIG_PPC_DCR

<<<<<<< HEAD
/**
=======
/*
>>>>>>> upstream/android-13
 * temac_dma_dcr_in32 - DCR based DMA read
 */
static u32 temac_dma_dcr_in(struct temac_local *lp, int reg)
{
	return dcr_read(lp->sdma_dcrs, reg);
}

<<<<<<< HEAD
/**
=======
/*
>>>>>>> upstream/android-13
 * temac_dma_dcr_out32 - DCR based DMA write
 */
static void temac_dma_dcr_out(struct temac_local *lp, int reg, u32 value)
{
	dcr_write(lp->sdma_dcrs, reg, value);
}

<<<<<<< HEAD
/**
=======
/*
>>>>>>> upstream/android-13
 * temac_dcr_setup - If the DMA is DCR based, then setup the address and
 * I/O  functions
 */
static int temac_dcr_setup(struct temac_local *lp, struct platform_device *op,
				struct device_node *np)
{
	unsigned int dcrs;

	/* setup the dcr address mapping if it's in the device tree */

	dcrs = dcr_resource_start(np, 0);
	if (dcrs != 0) {
		lp->sdma_dcrs = dcr_map(np, dcrs, dcr_resource_len(np, 0));
		lp->dma_in = temac_dma_dcr_in;
		lp->dma_out = temac_dma_dcr_out;
		dev_dbg(&op->dev, "DCR base: %x\n", dcrs);
		return 0;
	}
	/* no DCR in the device tree, indicate a failure */
	return -1;
}

#else

/*
 * temac_dcr_setup - This is a stub for when DCR is not supported,
<<<<<<< HEAD
 * such as with MicroBlaze
=======
 * such as with MicroBlaze and x86
>>>>>>> upstream/android-13
 */
static int temac_dcr_setup(struct temac_local *lp, struct platform_device *op,
				struct device_node *np)
{
	return -1;
}

#endif

<<<<<<< HEAD
/**
=======
/*
>>>>>>> upstream/android-13
 * temac_dma_bd_release - Release buffer descriptor rings
 */
static void temac_dma_bd_release(struct net_device *ndev)
{
	struct temac_local *lp = netdev_priv(ndev);
	int i;

	/* Reset Local Link (DMA) */
	lp->dma_out(lp, DMA_CONTROL_REG, DMA_CONTROL_RST);

<<<<<<< HEAD
	for (i = 0; i < RX_BD_NUM; i++) {
=======
	for (i = 0; i < lp->rx_bd_num; i++) {
>>>>>>> upstream/android-13
		if (!lp->rx_skb[i])
			break;
		else {
			dma_unmap_single(ndev->dev.parent, lp->rx_bd_v[i].phys,
					XTE_MAX_JUMBO_FRAME_SIZE, DMA_FROM_DEVICE);
			dev_kfree_skb(lp->rx_skb[i]);
		}
	}
	if (lp->rx_bd_v)
		dma_free_coherent(ndev->dev.parent,
<<<<<<< HEAD
				sizeof(*lp->rx_bd_v) * RX_BD_NUM,
				lp->rx_bd_v, lp->rx_bd_p);
	if (lp->tx_bd_v)
		dma_free_coherent(ndev->dev.parent,
				sizeof(*lp->tx_bd_v) * TX_BD_NUM,
				lp->tx_bd_v, lp->tx_bd_p);
	kfree(lp->rx_skb);
}

/**
=======
				  sizeof(*lp->rx_bd_v) * lp->rx_bd_num,
				  lp->rx_bd_v, lp->rx_bd_p);
	if (lp->tx_bd_v)
		dma_free_coherent(ndev->dev.parent,
				  sizeof(*lp->tx_bd_v) * lp->tx_bd_num,
				  lp->tx_bd_v, lp->tx_bd_p);
}

/*
>>>>>>> upstream/android-13
 * temac_dma_bd_init - Setup buffer descriptor rings
 */
static int temac_dma_bd_init(struct net_device *ndev)
{
	struct temac_local *lp = netdev_priv(ndev);
	struct sk_buff *skb;
<<<<<<< HEAD
	int i;

	lp->rx_skb = kcalloc(RX_BD_NUM, sizeof(*lp->rx_skb), GFP_KERNEL);
=======
	dma_addr_t skb_dma_addr;
	int i;

	lp->rx_skb = devm_kcalloc(&ndev->dev, lp->rx_bd_num,
				  sizeof(*lp->rx_skb), GFP_KERNEL);
>>>>>>> upstream/android-13
	if (!lp->rx_skb)
		goto out;

	/* allocate the tx and rx ring buffer descriptors. */
	/* returns a virtual address and a physical address. */
<<<<<<< HEAD
	lp->tx_bd_v = dma_zalloc_coherent(ndev->dev.parent,
					  sizeof(*lp->tx_bd_v) * TX_BD_NUM,
					  &lp->tx_bd_p, GFP_KERNEL);
	if (!lp->tx_bd_v)
		goto out;

	lp->rx_bd_v = dma_zalloc_coherent(ndev->dev.parent,
					  sizeof(*lp->rx_bd_v) * RX_BD_NUM,
					  &lp->rx_bd_p, GFP_KERNEL);
	if (!lp->rx_bd_v)
		goto out;

	for (i = 0; i < TX_BD_NUM; i++) {
		lp->tx_bd_v[i].next = lp->tx_bd_p +
				sizeof(*lp->tx_bd_v) * ((i + 1) % TX_BD_NUM);
	}

	for (i = 0; i < RX_BD_NUM; i++) {
		lp->rx_bd_v[i].next = lp->rx_bd_p +
				sizeof(*lp->rx_bd_v) * ((i + 1) % RX_BD_NUM);
=======
	lp->tx_bd_v = dma_alloc_coherent(ndev->dev.parent,
					 sizeof(*lp->tx_bd_v) * lp->tx_bd_num,
					 &lp->tx_bd_p, GFP_KERNEL);
	if (!lp->tx_bd_v)
		goto out;

	lp->rx_bd_v = dma_alloc_coherent(ndev->dev.parent,
					 sizeof(*lp->rx_bd_v) * lp->rx_bd_num,
					 &lp->rx_bd_p, GFP_KERNEL);
	if (!lp->rx_bd_v)
		goto out;

	for (i = 0; i < lp->tx_bd_num; i++) {
		lp->tx_bd_v[i].next = cpu_to_be32(lp->tx_bd_p
			+ sizeof(*lp->tx_bd_v) * ((i + 1) % lp->tx_bd_num));
	}

	for (i = 0; i < lp->rx_bd_num; i++) {
		lp->rx_bd_v[i].next = cpu_to_be32(lp->rx_bd_p
			+ sizeof(*lp->rx_bd_v) * ((i + 1) % lp->rx_bd_num));
>>>>>>> upstream/android-13

		skb = netdev_alloc_skb_ip_align(ndev,
						XTE_MAX_JUMBO_FRAME_SIZE);
		if (!skb)
			goto out;

		lp->rx_skb[i] = skb;
		/* returns physical address of skb->data */
<<<<<<< HEAD
		lp->rx_bd_v[i].phys = dma_map_single(ndev->dev.parent,
						     skb->data,
						     XTE_MAX_JUMBO_FRAME_SIZE,
						     DMA_FROM_DEVICE);
		lp->rx_bd_v[i].len = XTE_MAX_JUMBO_FRAME_SIZE;
		lp->rx_bd_v[i].app0 = STS_CTRL_APP0_IRQONEND;
	}

	lp->dma_out(lp, TX_CHNL_CTRL, 0x10220400 |
					  CHNL_CTRL_IRQ_EN |
					  CHNL_CTRL_IRQ_DLY_EN |
					  CHNL_CTRL_IRQ_COAL_EN);
	/* 0x10220483 */
	/* 0x00100483 */
	lp->dma_out(lp, RX_CHNL_CTRL, 0xff070000 |
					  CHNL_CTRL_IRQ_EN |
					  CHNL_CTRL_IRQ_DLY_EN |
					  CHNL_CTRL_IRQ_COAL_EN |
					  CHNL_CTRL_IRQ_IOE);
	/* 0xff010283 */

	lp->dma_out(lp, RX_CURDESC_PTR,  lp->rx_bd_p);
	lp->dma_out(lp, RX_TAILDESC_PTR,
		       lp->rx_bd_p + (sizeof(*lp->rx_bd_v) * (RX_BD_NUM - 1)));
	lp->dma_out(lp, TX_CURDESC_PTR, lp->tx_bd_p);

	/* Init descriptor indexes */
	lp->tx_bd_ci = 0;
	lp->tx_bd_next = 0;
	lp->tx_bd_tail = 0;
	lp->rx_bd_ci = 0;
=======
		skb_dma_addr = dma_map_single(ndev->dev.parent, skb->data,
					      XTE_MAX_JUMBO_FRAME_SIZE,
					      DMA_FROM_DEVICE);
		if (dma_mapping_error(ndev->dev.parent, skb_dma_addr))
			goto out;
		lp->rx_bd_v[i].phys = cpu_to_be32(skb_dma_addr);
		lp->rx_bd_v[i].len = cpu_to_be32(XTE_MAX_JUMBO_FRAME_SIZE);
		lp->rx_bd_v[i].app0 = cpu_to_be32(STS_CTRL_APP0_IRQONEND);
	}

	/* Configure DMA channel (irq setup) */
	lp->dma_out(lp, TX_CHNL_CTRL,
		    lp->coalesce_delay_tx << 24 | lp->coalesce_count_tx << 16 |
		    0x00000400 | // Use 1 Bit Wide Counters. Currently Not Used!
		    CHNL_CTRL_IRQ_EN | CHNL_CTRL_IRQ_ERR_EN |
		    CHNL_CTRL_IRQ_DLY_EN | CHNL_CTRL_IRQ_COAL_EN);
	lp->dma_out(lp, RX_CHNL_CTRL,
		    lp->coalesce_delay_rx << 24 | lp->coalesce_count_rx << 16 |
		    CHNL_CTRL_IRQ_IOE |
		    CHNL_CTRL_IRQ_EN | CHNL_CTRL_IRQ_ERR_EN |
		    CHNL_CTRL_IRQ_DLY_EN | CHNL_CTRL_IRQ_COAL_EN);

	/* Init descriptor indexes */
	lp->tx_bd_ci = 0;
	lp->tx_bd_tail = 0;
	lp->rx_bd_ci = 0;
	lp->rx_bd_tail = lp->rx_bd_num - 1;

	/* Enable RX DMA transfers */
	wmb();
	lp->dma_out(lp, RX_CURDESC_PTR,  lp->rx_bd_p);
	lp->dma_out(lp, RX_TAILDESC_PTR,
		       lp->rx_bd_p + (sizeof(*lp->rx_bd_v) * lp->rx_bd_tail));

	/* Prepare for TX DMA transfer */
	lp->dma_out(lp, TX_CURDESC_PTR, lp->tx_bd_p);
>>>>>>> upstream/android-13

	return 0;

out:
	temac_dma_bd_release(ndev);
	return -ENOMEM;
}

/* ---------------------------------------------------------------------
 * net_device_ops
 */

static void temac_do_set_mac_address(struct net_device *ndev)
{
	struct temac_local *lp = netdev_priv(ndev);
<<<<<<< HEAD

	/* set up unicast MAC address filter set its mac address */
	mutex_lock(&lp->indirect_mutex);
	temac_indirect_out32(lp, XTE_UAW0_OFFSET,
			     (ndev->dev_addr[0]) |
			     (ndev->dev_addr[1] << 8) |
			     (ndev->dev_addr[2] << 16) |
			     (ndev->dev_addr[3] << 24));
	/* There are reserved bits in EUAW1
	 * so don't affect them Set MAC bits [47:32] in EUAW1 */
	temac_indirect_out32(lp, XTE_UAW1_OFFSET,
			     (ndev->dev_addr[4] & 0x000000ff) |
			     (ndev->dev_addr[5] << 8));
	mutex_unlock(&lp->indirect_mutex);
=======
	unsigned long flags;

	/* set up unicast MAC address filter set its mac address */
	spin_lock_irqsave(lp->indirect_lock, flags);
	temac_indirect_out32_locked(lp, XTE_UAW0_OFFSET,
				    (ndev->dev_addr[0]) |
				    (ndev->dev_addr[1] << 8) |
				    (ndev->dev_addr[2] << 16) |
				    (ndev->dev_addr[3] << 24));
	/* There are reserved bits in EUAW1
	 * so don't affect them Set MAC bits [47:32] in EUAW1 */
	temac_indirect_out32_locked(lp, XTE_UAW1_OFFSET,
				    (ndev->dev_addr[4] & 0x000000ff) |
				    (ndev->dev_addr[5] << 8));
	spin_unlock_irqrestore(lp->indirect_lock, flags);
>>>>>>> upstream/android-13
}

static int temac_init_mac_address(struct net_device *ndev, const void *address)
{
	memcpy(ndev->dev_addr, address, ETH_ALEN);
	if (!is_valid_ether_addr(ndev->dev_addr))
		eth_hw_addr_random(ndev);
	temac_do_set_mac_address(ndev);
	return 0;
}

static int temac_set_mac_address(struct net_device *ndev, void *p)
{
	struct sockaddr *addr = p;

	if (!is_valid_ether_addr(addr->sa_data))
		return -EADDRNOTAVAIL;
	memcpy(ndev->dev_addr, addr->sa_data, ETH_ALEN);
	temac_do_set_mac_address(ndev);
	return 0;
}

static void temac_set_multicast_list(struct net_device *ndev)
{
	struct temac_local *lp = netdev_priv(ndev);
<<<<<<< HEAD
	u32 multi_addr_msw, multi_addr_lsw, val;
	int i;

	mutex_lock(&lp->indirect_mutex);
	if (ndev->flags & (IFF_ALLMULTI | IFF_PROMISC) ||
	    netdev_mc_count(ndev) > MULTICAST_CAM_TABLE_NUM) {
		/*
		 *	We must make the kernel realise we had to move
		 *	into promisc mode or we start all out war on
		 *	the cable. If it was a promisc request the
		 *	flag is already set. If not we assert it.
		 */
		ndev->flags |= IFF_PROMISC;
		temac_indirect_out32(lp, XTE_AFM_OFFSET, XTE_AFM_EPPRM_MASK);
		dev_info(&ndev->dev, "Promiscuous mode enabled.\n");
	} else if (!netdev_mc_empty(ndev)) {
		struct netdev_hw_addr *ha;

		i = 0;
		netdev_for_each_mc_addr(ha, ndev) {
			if (i >= MULTICAST_CAM_TABLE_NUM)
=======
	u32 multi_addr_msw, multi_addr_lsw;
	int i = 0;
	unsigned long flags;
	bool promisc_mode_disabled = false;

	if (ndev->flags & (IFF_PROMISC | IFF_ALLMULTI) ||
	    (netdev_mc_count(ndev) > MULTICAST_CAM_TABLE_NUM)) {
		temac_indirect_out32(lp, XTE_AFM_OFFSET, XTE_AFM_EPPRM_MASK);
		dev_info(&ndev->dev, "Promiscuous mode enabled.\n");
		return;
	}

	spin_lock_irqsave(lp->indirect_lock, flags);

	if (!netdev_mc_empty(ndev)) {
		struct netdev_hw_addr *ha;

		netdev_for_each_mc_addr(ha, ndev) {
			if (WARN_ON(i >= MULTICAST_CAM_TABLE_NUM))
>>>>>>> upstream/android-13
				break;
			multi_addr_msw = ((ha->addr[3] << 24) |
					  (ha->addr[2] << 16) |
					  (ha->addr[1] << 8) |
					  (ha->addr[0]));
<<<<<<< HEAD
			temac_indirect_out32(lp, XTE_MAW0_OFFSET,
					     multi_addr_msw);
			multi_addr_lsw = ((ha->addr[5] << 8) |
					  (ha->addr[4]) | (i << 16));
			temac_indirect_out32(lp, XTE_MAW1_OFFSET,
					     multi_addr_lsw);
			i++;
		}
	} else {
		val = temac_indirect_in32(lp, XTE_AFM_OFFSET);
		temac_indirect_out32(lp, XTE_AFM_OFFSET,
				     val & ~XTE_AFM_EPPRM_MASK);
		temac_indirect_out32(lp, XTE_MAW0_OFFSET, 0);
		temac_indirect_out32(lp, XTE_MAW1_OFFSET, 0);
		dev_info(&ndev->dev, "Promiscuous mode disabled.\n");
	}
	mutex_unlock(&lp->indirect_mutex);
=======
			temac_indirect_out32_locked(lp, XTE_MAW0_OFFSET,
						    multi_addr_msw);
			multi_addr_lsw = ((ha->addr[5] << 8) |
					  (ha->addr[4]) | (i << 16));
			temac_indirect_out32_locked(lp, XTE_MAW1_OFFSET,
						    multi_addr_lsw);
			i++;
		}
	}

	/* Clear all or remaining/unused address table entries */
	while (i < MULTICAST_CAM_TABLE_NUM) {
		temac_indirect_out32_locked(lp, XTE_MAW0_OFFSET, 0);
		temac_indirect_out32_locked(lp, XTE_MAW1_OFFSET, i << 16);
		i++;
	}

	/* Enable address filter block if currently disabled */
	if (temac_indirect_in32_locked(lp, XTE_AFM_OFFSET)
	    & XTE_AFM_EPPRM_MASK) {
		temac_indirect_out32_locked(lp, XTE_AFM_OFFSET, 0);
		promisc_mode_disabled = true;
	}

	spin_unlock_irqrestore(lp->indirect_lock, flags);

	if (promisc_mode_disabled)
		dev_info(&ndev->dev, "Promiscuous mode disabled.\n");
>>>>>>> upstream/android-13
}

static struct temac_option {
	int flg;
	u32 opt;
	u32 reg;
	u32 m_or;
	u32 m_and;
} temac_options[] = {
	/* Turn on jumbo packet support for both Rx and Tx */
	{
		.opt = XTE_OPTION_JUMBO,
		.reg = XTE_TXC_OFFSET,
		.m_or = XTE_TXC_TXJMBO_MASK,
	},
	{
		.opt = XTE_OPTION_JUMBO,
		.reg = XTE_RXC1_OFFSET,
		.m_or =XTE_RXC1_RXJMBO_MASK,
	},
	/* Turn on VLAN packet support for both Rx and Tx */
	{
		.opt = XTE_OPTION_VLAN,
		.reg = XTE_TXC_OFFSET,
		.m_or =XTE_TXC_TXVLAN_MASK,
	},
	{
		.opt = XTE_OPTION_VLAN,
		.reg = XTE_RXC1_OFFSET,
		.m_or =XTE_RXC1_RXVLAN_MASK,
	},
	/* Turn on FCS stripping on receive packets */
	{
		.opt = XTE_OPTION_FCS_STRIP,
		.reg = XTE_RXC1_OFFSET,
		.m_or =XTE_RXC1_RXFCS_MASK,
	},
	/* Turn on FCS insertion on transmit packets */
	{
		.opt = XTE_OPTION_FCS_INSERT,
		.reg = XTE_TXC_OFFSET,
		.m_or =XTE_TXC_TXFCS_MASK,
	},
	/* Turn on length/type field checking on receive packets */
	{
		.opt = XTE_OPTION_LENTYPE_ERR,
		.reg = XTE_RXC1_OFFSET,
		.m_or =XTE_RXC1_RXLT_MASK,
	},
	/* Turn on flow control */
	{
		.opt = XTE_OPTION_FLOW_CONTROL,
		.reg = XTE_FCC_OFFSET,
		.m_or =XTE_FCC_RXFLO_MASK,
	},
	/* Turn on flow control */
	{
		.opt = XTE_OPTION_FLOW_CONTROL,
		.reg = XTE_FCC_OFFSET,
		.m_or =XTE_FCC_TXFLO_MASK,
	},
	/* Turn on promiscuous frame filtering (all frames are received ) */
	{
		.opt = XTE_OPTION_PROMISC,
		.reg = XTE_AFM_OFFSET,
		.m_or =XTE_AFM_EPPRM_MASK,
	},
	/* Enable transmitter if not already enabled */
	{
		.opt = XTE_OPTION_TXEN,
		.reg = XTE_TXC_OFFSET,
		.m_or =XTE_TXC_TXEN_MASK,
	},
	/* Enable receiver? */
	{
		.opt = XTE_OPTION_RXEN,
		.reg = XTE_RXC1_OFFSET,
		.m_or =XTE_RXC1_RXEN_MASK,
	},
	{}
};

<<<<<<< HEAD
/**
=======
/*
>>>>>>> upstream/android-13
 * temac_setoptions
 */
static u32 temac_setoptions(struct net_device *ndev, u32 options)
{
	struct temac_local *lp = netdev_priv(ndev);
	struct temac_option *tp = &temac_options[0];
	int reg;
<<<<<<< HEAD

	mutex_lock(&lp->indirect_mutex);
	while (tp->opt) {
		reg = temac_indirect_in32(lp, tp->reg) & ~tp->m_or;
		if (options & tp->opt)
			reg |= tp->m_or;
		temac_indirect_out32(lp, tp->reg, reg);
		tp++;
	}
	lp->options |= options;
	mutex_unlock(&lp->indirect_mutex);
=======
	unsigned long flags;

	spin_lock_irqsave(lp->indirect_lock, flags);
	while (tp->opt) {
		reg = temac_indirect_in32_locked(lp, tp->reg) & ~tp->m_or;
		if (options & tp->opt) {
			reg |= tp->m_or;
			temac_indirect_out32_locked(lp, tp->reg, reg);
		}
		tp++;
	}
	spin_unlock_irqrestore(lp->indirect_lock, flags);
	lp->options |= options;
>>>>>>> upstream/android-13

	return 0;
}

/* Initialize temac */
static void temac_device_reset(struct net_device *ndev)
{
	struct temac_local *lp = netdev_priv(ndev);
	u32 timeout;
	u32 val;
<<<<<<< HEAD
=======
	unsigned long flags;
>>>>>>> upstream/android-13

	/* Perform a software reset */

	/* 0x300 host enable bit ? */
	/* reset PHY through control register ?:1 */

	dev_dbg(&ndev->dev, "%s()\n", __func__);

<<<<<<< HEAD
	mutex_lock(&lp->indirect_mutex);
=======
>>>>>>> upstream/android-13
	/* Reset the receiver and wait for it to finish reset */
	temac_indirect_out32(lp, XTE_RXC1_OFFSET, XTE_RXC1_RXRST_MASK);
	timeout = 1000;
	while (temac_indirect_in32(lp, XTE_RXC1_OFFSET) & XTE_RXC1_RXRST_MASK) {
		udelay(1);
		if (--timeout == 0) {
			dev_err(&ndev->dev,
				"temac_device_reset RX reset timeout!!\n");
			break;
		}
	}

	/* Reset the transmitter and wait for it to finish reset */
	temac_indirect_out32(lp, XTE_TXC_OFFSET, XTE_TXC_TXRST_MASK);
	timeout = 1000;
	while (temac_indirect_in32(lp, XTE_TXC_OFFSET) & XTE_TXC_TXRST_MASK) {
		udelay(1);
		if (--timeout == 0) {
			dev_err(&ndev->dev,
				"temac_device_reset TX reset timeout!!\n");
			break;
		}
	}

	/* Disable the receiver */
<<<<<<< HEAD
	val = temac_indirect_in32(lp, XTE_RXC1_OFFSET);
	temac_indirect_out32(lp, XTE_RXC1_OFFSET, val & ~XTE_RXC1_RXEN_MASK);
=======
	spin_lock_irqsave(lp->indirect_lock, flags);
	val = temac_indirect_in32_locked(lp, XTE_RXC1_OFFSET);
	temac_indirect_out32_locked(lp, XTE_RXC1_OFFSET,
				    val & ~XTE_RXC1_RXEN_MASK);
	spin_unlock_irqrestore(lp->indirect_lock, flags);
>>>>>>> upstream/android-13

	/* Reset Local Link (DMA) */
	lp->dma_out(lp, DMA_CONTROL_REG, DMA_CONTROL_RST);
	timeout = 1000;
	while (lp->dma_in(lp, DMA_CONTROL_REG) & DMA_CONTROL_RST) {
		udelay(1);
		if (--timeout == 0) {
			dev_err(&ndev->dev,
				"temac_device_reset DMA reset timeout!!\n");
			break;
		}
	}
	lp->dma_out(lp, DMA_CONTROL_REG, DMA_TAIL_ENABLE);

	if (temac_dma_bd_init(ndev)) {
		dev_err(&ndev->dev,
				"temac_device_reset descriptor allocation failed\n");
	}

<<<<<<< HEAD
	temac_indirect_out32(lp, XTE_RXC0_OFFSET, 0);
	temac_indirect_out32(lp, XTE_RXC1_OFFSET, 0);
	temac_indirect_out32(lp, XTE_TXC_OFFSET, 0);
	temac_indirect_out32(lp, XTE_FCC_OFFSET, XTE_FCC_RXFLO_MASK);

	mutex_unlock(&lp->indirect_mutex);
=======
	spin_lock_irqsave(lp->indirect_lock, flags);
	temac_indirect_out32_locked(lp, XTE_RXC0_OFFSET, 0);
	temac_indirect_out32_locked(lp, XTE_RXC1_OFFSET, 0);
	temac_indirect_out32_locked(lp, XTE_TXC_OFFSET, 0);
	temac_indirect_out32_locked(lp, XTE_FCC_OFFSET, XTE_FCC_RXFLO_MASK);
	spin_unlock_irqrestore(lp->indirect_lock, flags);
>>>>>>> upstream/android-13

	/* Sync default options with HW
	 * but leave receiver and transmitter disabled.  */
	temac_setoptions(ndev,
			 lp->options & ~(XTE_OPTION_TXEN | XTE_OPTION_RXEN));

	temac_do_set_mac_address(ndev);

	/* Set address filter table */
	temac_set_multicast_list(ndev);
	if (temac_setoptions(ndev, lp->options))
		dev_err(&ndev->dev, "Error setting TEMAC options\n");

	/* Init Driver variable */
	netif_trans_update(ndev); /* prevent tx timeout */
}

static void temac_adjust_link(struct net_device *ndev)
{
	struct temac_local *lp = netdev_priv(ndev);
	struct phy_device *phy = ndev->phydev;
	u32 mii_speed;
	int link_state;
<<<<<<< HEAD
=======
	unsigned long flags;
>>>>>>> upstream/android-13

	/* hash together the state values to decide if something has changed */
	link_state = phy->speed | (phy->duplex << 1) | phy->link;

<<<<<<< HEAD
	mutex_lock(&lp->indirect_mutex);
	if (lp->last_link != link_state) {
		mii_speed = temac_indirect_in32(lp, XTE_EMCFG_OFFSET);
=======
	if (lp->last_link != link_state) {
		spin_lock_irqsave(lp->indirect_lock, flags);
		mii_speed = temac_indirect_in32_locked(lp, XTE_EMCFG_OFFSET);
>>>>>>> upstream/android-13
		mii_speed &= ~XTE_EMCFG_LINKSPD_MASK;

		switch (phy->speed) {
		case SPEED_1000: mii_speed |= XTE_EMCFG_LINKSPD_1000; break;
		case SPEED_100: mii_speed |= XTE_EMCFG_LINKSPD_100; break;
		case SPEED_10: mii_speed |= XTE_EMCFG_LINKSPD_10; break;
		}

		/* Write new speed setting out to TEMAC */
<<<<<<< HEAD
		temac_indirect_out32(lp, XTE_EMCFG_OFFSET, mii_speed);
		lp->last_link = link_state;
		phy_print_status(phy);
	}
	mutex_unlock(&lp->indirect_mutex);
}

=======
		temac_indirect_out32_locked(lp, XTE_EMCFG_OFFSET, mii_speed);
		spin_unlock_irqrestore(lp->indirect_lock, flags);

		lp->last_link = link_state;
		phy_print_status(phy);
	}
}

#ifdef CONFIG_64BIT

static void ptr_to_txbd(void *p, struct cdmac_bd *bd)
{
	bd->app3 = (u32)(((u64)p) >> 32);
	bd->app4 = (u32)((u64)p & 0xFFFFFFFF);
}

static void *ptr_from_txbd(struct cdmac_bd *bd)
{
	return (void *)(((u64)(bd->app3) << 32) | bd->app4);
}

#else

static void ptr_to_txbd(void *p, struct cdmac_bd *bd)
{
	bd->app4 = (u32)p;
}

static void *ptr_from_txbd(struct cdmac_bd *bd)
{
	return (void *)(bd->app4);
}

#endif

>>>>>>> upstream/android-13
static void temac_start_xmit_done(struct net_device *ndev)
{
	struct temac_local *lp = netdev_priv(ndev);
	struct cdmac_bd *cur_p;
	unsigned int stat = 0;
<<<<<<< HEAD

	cur_p = &lp->tx_bd_v[lp->tx_bd_ci];
	stat = cur_p->app0;

	while (stat & STS_CTRL_APP0_CMPLT) {
		dma_unmap_single(ndev->dev.parent, cur_p->phys, cur_p->len,
				 DMA_TO_DEVICE);
		if (cur_p->app4)
			dev_kfree_skb_irq((struct sk_buff *)cur_p->app4);
		cur_p->app0 = 0;
=======
	struct sk_buff *skb;

	cur_p = &lp->tx_bd_v[lp->tx_bd_ci];
	stat = be32_to_cpu(cur_p->app0);

	while (stat & STS_CTRL_APP0_CMPLT) {
		/* Make sure that the other fields are read after bd is
		 * released by dma
		 */
		rmb();
		dma_unmap_single(ndev->dev.parent, be32_to_cpu(cur_p->phys),
				 be32_to_cpu(cur_p->len), DMA_TO_DEVICE);
		skb = (struct sk_buff *)ptr_from_txbd(cur_p);
		if (skb)
			dev_consume_skb_irq(skb);
>>>>>>> upstream/android-13
		cur_p->app1 = 0;
		cur_p->app2 = 0;
		cur_p->app3 = 0;
		cur_p->app4 = 0;

		ndev->stats.tx_packets++;
<<<<<<< HEAD
		ndev->stats.tx_bytes += cur_p->len;

		lp->tx_bd_ci++;
		if (lp->tx_bd_ci >= TX_BD_NUM)
			lp->tx_bd_ci = 0;

		cur_p = &lp->tx_bd_v[lp->tx_bd_ci];
		stat = cur_p->app0;
	}

=======
		ndev->stats.tx_bytes += be32_to_cpu(cur_p->len);

		/* app0 must be visible last, as it is used to flag
		 * availability of the bd
		 */
		smp_mb();
		cur_p->app0 = 0;

		lp->tx_bd_ci++;
		if (lp->tx_bd_ci >= lp->tx_bd_num)
			lp->tx_bd_ci = 0;

		cur_p = &lp->tx_bd_v[lp->tx_bd_ci];
		stat = be32_to_cpu(cur_p->app0);
	}

	/* Matches barrier in temac_start_xmit */
	smp_mb();

>>>>>>> upstream/android-13
	netif_wake_queue(ndev);
}

static inline int temac_check_tx_bd_space(struct temac_local *lp, int num_frag)
{
	struct cdmac_bd *cur_p;
	int tail;

	tail = lp->tx_bd_tail;
	cur_p = &lp->tx_bd_v[tail];

	do {
		if (cur_p->app0)
			return NETDEV_TX_BUSY;

<<<<<<< HEAD
		tail++;
		if (tail >= TX_BD_NUM)
=======
		/* Make sure to read next bd app0 after this one */
		rmb();

		tail++;
		if (tail >= lp->tx_bd_num)
>>>>>>> upstream/android-13
			tail = 0;

		cur_p = &lp->tx_bd_v[tail];
		num_frag--;
	} while (num_frag >= 0);

	return 0;
}

static netdev_tx_t
temac_start_xmit(struct sk_buff *skb, struct net_device *ndev)
{
	struct temac_local *lp = netdev_priv(ndev);
	struct cdmac_bd *cur_p;
<<<<<<< HEAD
	dma_addr_t start_p, tail_p;
=======
	dma_addr_t tail_p, skb_dma_addr;
>>>>>>> upstream/android-13
	int ii;
	unsigned long num_frag;
	skb_frag_t *frag;

	num_frag = skb_shinfo(skb)->nr_frags;
	frag = &skb_shinfo(skb)->frags[0];
<<<<<<< HEAD
	start_p = lp->tx_bd_p + sizeof(*lp->tx_bd_v) * lp->tx_bd_tail;
	cur_p = &lp->tx_bd_v[lp->tx_bd_tail];

	if (temac_check_tx_bd_space(lp, num_frag)) {
		if (!netif_queue_stopped(ndev))
			netif_stop_queue(ndev);
		return NETDEV_TX_BUSY;
=======
	cur_p = &lp->tx_bd_v[lp->tx_bd_tail];

	if (temac_check_tx_bd_space(lp, num_frag + 1)) {
		if (netif_queue_stopped(ndev))
			return NETDEV_TX_BUSY;

		netif_stop_queue(ndev);

		/* Matches barrier in temac_start_xmit_done */
		smp_mb();

		/* Space might have just been freed - check again */
		if (temac_check_tx_bd_space(lp, num_frag + 1))
			return NETDEV_TX_BUSY;

		netif_wake_queue(ndev);
>>>>>>> upstream/android-13
	}

	cur_p->app0 = 0;
	if (skb->ip_summed == CHECKSUM_PARTIAL) {
		unsigned int csum_start_off = skb_checksum_start_offset(skb);
		unsigned int csum_index_off = csum_start_off + skb->csum_offset;

<<<<<<< HEAD
		cur_p->app0 |= 1; /* TX Checksum Enabled */
		cur_p->app1 = (csum_start_off << 16) | csum_index_off;
		cur_p->app2 = 0;  /* initial checksum seed */
	}

	cur_p->app0 |= STS_CTRL_APP0_SOP;
	cur_p->len = skb_headlen(skb);
	cur_p->phys = dma_map_single(ndev->dev.parent, skb->data,
				     skb_headlen(skb), DMA_TO_DEVICE);
	cur_p->app4 = (unsigned long)skb;

	for (ii = 0; ii < num_frag; ii++) {
		lp->tx_bd_tail++;
		if (lp->tx_bd_tail >= TX_BD_NUM)
			lp->tx_bd_tail = 0;

		cur_p = &lp->tx_bd_v[lp->tx_bd_tail];
		cur_p->phys = dma_map_single(ndev->dev.parent,
					     skb_frag_address(frag),
					     skb_frag_size(frag), DMA_TO_DEVICE);
		cur_p->len = skb_frag_size(frag);
		cur_p->app0 = 0;
		frag++;
	}
	cur_p->app0 |= STS_CTRL_APP0_EOP;

	tail_p = lp->tx_bd_p + sizeof(*lp->tx_bd_v) * lp->tx_bd_tail;
	lp->tx_bd_tail++;
	if (lp->tx_bd_tail >= TX_BD_NUM)
=======
		cur_p->app0 |= cpu_to_be32(0x000001); /* TX Checksum Enabled */
		cur_p->app1 = cpu_to_be32((csum_start_off << 16)
					  | csum_index_off);
		cur_p->app2 = 0;  /* initial checksum seed */
	}

	cur_p->app0 |= cpu_to_be32(STS_CTRL_APP0_SOP);
	skb_dma_addr = dma_map_single(ndev->dev.parent, skb->data,
				      skb_headlen(skb), DMA_TO_DEVICE);
	cur_p->len = cpu_to_be32(skb_headlen(skb));
	if (WARN_ON_ONCE(dma_mapping_error(ndev->dev.parent, skb_dma_addr))) {
		dev_kfree_skb_any(skb);
		ndev->stats.tx_dropped++;
		return NETDEV_TX_OK;
	}
	cur_p->phys = cpu_to_be32(skb_dma_addr);

	for (ii = 0; ii < num_frag; ii++) {
		if (++lp->tx_bd_tail >= lp->tx_bd_num)
			lp->tx_bd_tail = 0;

		cur_p = &lp->tx_bd_v[lp->tx_bd_tail];
		skb_dma_addr = dma_map_single(ndev->dev.parent,
					      skb_frag_address(frag),
					      skb_frag_size(frag),
					      DMA_TO_DEVICE);
		if (dma_mapping_error(ndev->dev.parent, skb_dma_addr)) {
			if (--lp->tx_bd_tail < 0)
				lp->tx_bd_tail = lp->tx_bd_num - 1;
			cur_p = &lp->tx_bd_v[lp->tx_bd_tail];
			while (--ii >= 0) {
				--frag;
				dma_unmap_single(ndev->dev.parent,
						 be32_to_cpu(cur_p->phys),
						 skb_frag_size(frag),
						 DMA_TO_DEVICE);
				if (--lp->tx_bd_tail < 0)
					lp->tx_bd_tail = lp->tx_bd_num - 1;
				cur_p = &lp->tx_bd_v[lp->tx_bd_tail];
			}
			dma_unmap_single(ndev->dev.parent,
					 be32_to_cpu(cur_p->phys),
					 skb_headlen(skb), DMA_TO_DEVICE);
			dev_kfree_skb_any(skb);
			ndev->stats.tx_dropped++;
			return NETDEV_TX_OK;
		}
		cur_p->phys = cpu_to_be32(skb_dma_addr);
		cur_p->len = cpu_to_be32(skb_frag_size(frag));
		cur_p->app0 = 0;
		frag++;
	}
	cur_p->app0 |= cpu_to_be32(STS_CTRL_APP0_EOP);

	/* Mark last fragment with skb address, so it can be consumed
	 * in temac_start_xmit_done()
	 */
	ptr_to_txbd((void *)skb, cur_p);

	tail_p = lp->tx_bd_p + sizeof(*lp->tx_bd_v) * lp->tx_bd_tail;
	lp->tx_bd_tail++;
	if (lp->tx_bd_tail >= lp->tx_bd_num)
>>>>>>> upstream/android-13
		lp->tx_bd_tail = 0;

	skb_tx_timestamp(skb);

	/* Kick off the transfer */
<<<<<<< HEAD
	lp->dma_out(lp, TX_TAILDESC_PTR, tail_p); /* DMA start */

	return NETDEV_TX_OK;
}

=======
	wmb();
	lp->dma_out(lp, TX_TAILDESC_PTR, tail_p); /* DMA start */

	if (temac_check_tx_bd_space(lp, MAX_SKB_FRAGS + 1))
		netif_stop_queue(ndev);

	return NETDEV_TX_OK;
}

static int ll_temac_recv_buffers_available(struct temac_local *lp)
{
	int available;

	if (!lp->rx_skb[lp->rx_bd_ci])
		return 0;
	available = 1 + lp->rx_bd_tail - lp->rx_bd_ci;
	if (available <= 0)
		available += lp->rx_bd_num;
	return available;
}
>>>>>>> upstream/android-13

static void ll_temac_recv(struct net_device *ndev)
{
	struct temac_local *lp = netdev_priv(ndev);
<<<<<<< HEAD
	struct sk_buff *skb, *new_skb;
	unsigned int bdstat;
	struct cdmac_bd *cur_p;
	dma_addr_t tail_p;
	int length;
	unsigned long flags;

	spin_lock_irqsave(&lp->rx_lock, flags);

	tail_p = lp->rx_bd_p + sizeof(*lp->rx_bd_v) * lp->rx_bd_ci;
	cur_p = &lp->rx_bd_v[lp->rx_bd_ci];

	bdstat = cur_p->app0;
	while ((bdstat & STS_CTRL_APP0_CMPLT)) {

		skb = lp->rx_skb[lp->rx_bd_ci];
		length = cur_p->app4 & 0x3FFF;

		dma_unmap_single(ndev->dev.parent, cur_p->phys, length,
				 DMA_FROM_DEVICE);

=======
	unsigned long flags;
	int rx_bd;
	bool update_tail = false;

	spin_lock_irqsave(&lp->rx_lock, flags);

	/* Process all received buffers, passing them on network
	 * stack.  After this, the buffer descriptors will be in an
	 * un-allocated stage, where no skb is allocated for it, and
	 * they are therefore not available for TEMAC/DMA.
	 */
	do {
		struct cdmac_bd *bd = &lp->rx_bd_v[lp->rx_bd_ci];
		struct sk_buff *skb = lp->rx_skb[lp->rx_bd_ci];
		unsigned int bdstat = be32_to_cpu(bd->app0);
		int length;

		/* While this should not normally happen, we can end
		 * here when GFP_ATOMIC allocations fail, and we
		 * therefore have un-allocated buffers.
		 */
		if (!skb)
			break;

		/* Loop over all completed buffer descriptors */
		if (!(bdstat & STS_CTRL_APP0_CMPLT))
			break;

		dma_unmap_single(ndev->dev.parent, be32_to_cpu(bd->phys),
				 XTE_MAX_JUMBO_FRAME_SIZE, DMA_FROM_DEVICE);
		/* The buffer is not valid for DMA anymore */
		bd->phys = 0;
		bd->len = 0;

		length = be32_to_cpu(bd->app4) & 0x3FFF;
>>>>>>> upstream/android-13
		skb_put(skb, length);
		skb->protocol = eth_type_trans(skb, ndev);
		skb_checksum_none_assert(skb);

		/* if we're doing rx csum offload, set it up */
		if (((lp->temac_features & TEMAC_FEATURE_RX_CSUM) != 0) &&
		    (skb->protocol == htons(ETH_P_IP)) &&
		    (skb->len > 64)) {

<<<<<<< HEAD
			skb->csum = cur_p->app3 & 0xFFFF;
=======
			/* Convert from device endianness (be32) to cpu
			 * endiannes, and if necessary swap the bytes
			 * (back) for proper IP checksum byte order
			 * (be16).
			 */
			skb->csum = htons(be32_to_cpu(bd->app3) & 0xFFFF);
>>>>>>> upstream/android-13
			skb->ip_summed = CHECKSUM_COMPLETE;
		}

		if (!skb_defer_rx_timestamp(skb))
			netif_rx(skb);
<<<<<<< HEAD
=======
		/* The skb buffer is now owned by network stack above */
		lp->rx_skb[lp->rx_bd_ci] = NULL;
>>>>>>> upstream/android-13

		ndev->stats.rx_packets++;
		ndev->stats.rx_bytes += length;

<<<<<<< HEAD
		new_skb = netdev_alloc_skb_ip_align(ndev,
						XTE_MAX_JUMBO_FRAME_SIZE);
		if (!new_skb) {
			spin_unlock_irqrestore(&lp->rx_lock, flags);
			return;
		}

		cur_p->app0 = STS_CTRL_APP0_IRQONEND;
		cur_p->phys = dma_map_single(ndev->dev.parent, new_skb->data,
					     XTE_MAX_JUMBO_FRAME_SIZE,
					     DMA_FROM_DEVICE);
		cur_p->len = XTE_MAX_JUMBO_FRAME_SIZE;
		lp->rx_skb[lp->rx_bd_ci] = new_skb;

		lp->rx_bd_ci++;
		if (lp->rx_bd_ci >= RX_BD_NUM)
			lp->rx_bd_ci = 0;

		cur_p = &lp->rx_bd_v[lp->rx_bd_ci];
		bdstat = cur_p->app0;
	}
	lp->dma_out(lp, RX_TAILDESC_PTR, tail_p);
=======
		rx_bd = lp->rx_bd_ci;
		if (++lp->rx_bd_ci >= lp->rx_bd_num)
			lp->rx_bd_ci = 0;
	} while (rx_bd != lp->rx_bd_tail);

	/* DMA operations will halt when the last buffer descriptor is
	 * processed (ie. the one pointed to by RX_TAILDESC_PTR).
	 * When that happens, no more interrupt events will be
	 * generated.  No IRQ_COAL or IRQ_DLY, and not even an
	 * IRQ_ERR.  To avoid stalling, we schedule a delayed work
	 * when there is a potential risk of that happening.  The work
	 * will call this function, and thus re-schedule itself until
	 * enough buffers are available again.
	 */
	if (ll_temac_recv_buffers_available(lp) < lp->coalesce_count_rx)
		schedule_delayed_work(&lp->restart_work, HZ / 1000);

	/* Allocate new buffers for those buffer descriptors that were
	 * passed to network stack.  Note that GFP_ATOMIC allocations
	 * can fail (e.g. when a larger burst of GFP_ATOMIC
	 * allocations occurs), so while we try to allocate all
	 * buffers in the same interrupt where they were processed, we
	 * continue with what we could get in case of allocation
	 * failure.  Allocation of remaining buffers will be retried
	 * in following calls.
	 */
	while (1) {
		struct sk_buff *skb;
		struct cdmac_bd *bd;
		dma_addr_t skb_dma_addr;

		rx_bd = lp->rx_bd_tail + 1;
		if (rx_bd >= lp->rx_bd_num)
			rx_bd = 0;
		bd = &lp->rx_bd_v[rx_bd];

		if (bd->phys)
			break;	/* All skb's allocated */

		skb = netdev_alloc_skb_ip_align(ndev, XTE_MAX_JUMBO_FRAME_SIZE);
		if (!skb) {
			dev_warn(&ndev->dev, "skb alloc failed\n");
			break;
		}

		skb_dma_addr = dma_map_single(ndev->dev.parent, skb->data,
					      XTE_MAX_JUMBO_FRAME_SIZE,
					      DMA_FROM_DEVICE);
		if (WARN_ON_ONCE(dma_mapping_error(ndev->dev.parent,
						   skb_dma_addr))) {
			dev_kfree_skb_any(skb);
			break;
		}

		bd->phys = cpu_to_be32(skb_dma_addr);
		bd->len = cpu_to_be32(XTE_MAX_JUMBO_FRAME_SIZE);
		bd->app0 = cpu_to_be32(STS_CTRL_APP0_IRQONEND);
		lp->rx_skb[rx_bd] = skb;

		lp->rx_bd_tail = rx_bd;
		update_tail = true;
	}

	/* Move tail pointer when buffers have been allocated */
	if (update_tail) {
		lp->dma_out(lp, RX_TAILDESC_PTR,
			lp->rx_bd_p + sizeof(*lp->rx_bd_v) * lp->rx_bd_tail);
	}
>>>>>>> upstream/android-13

	spin_unlock_irqrestore(&lp->rx_lock, flags);
}

<<<<<<< HEAD
=======
/* Function scheduled to ensure a restart in case of DMA halt
 * condition caused by running out of buffer descriptors.
 */
static void ll_temac_restart_work_func(struct work_struct *work)
{
	struct temac_local *lp = container_of(work, struct temac_local,
					      restart_work.work);
	struct net_device *ndev = lp->ndev;

	ll_temac_recv(ndev);
}

>>>>>>> upstream/android-13
static irqreturn_t ll_temac_tx_irq(int irq, void *_ndev)
{
	struct net_device *ndev = _ndev;
	struct temac_local *lp = netdev_priv(ndev);
	unsigned int status;

	status = lp->dma_in(lp, TX_IRQ_REG);
	lp->dma_out(lp, TX_IRQ_REG, status);

	if (status & (IRQ_COAL | IRQ_DLY))
		temac_start_xmit_done(lp->ndev);
<<<<<<< HEAD
	if (status & 0x080)
		dev_err(&ndev->dev, "DMA error 0x%x\n", status);
=======
	if (status & (IRQ_ERR | IRQ_DMAERR))
		dev_err_ratelimited(&ndev->dev,
				    "TX error 0x%x TX_CHNL_STS=0x%08x\n",
				    status, lp->dma_in(lp, TX_CHNL_STS));
>>>>>>> upstream/android-13

	return IRQ_HANDLED;
}

static irqreturn_t ll_temac_rx_irq(int irq, void *_ndev)
{
	struct net_device *ndev = _ndev;
	struct temac_local *lp = netdev_priv(ndev);
	unsigned int status;

	/* Read and clear the status registers */
	status = lp->dma_in(lp, RX_IRQ_REG);
	lp->dma_out(lp, RX_IRQ_REG, status);

	if (status & (IRQ_COAL | IRQ_DLY))
		ll_temac_recv(lp->ndev);
<<<<<<< HEAD
=======
	if (status & (IRQ_ERR | IRQ_DMAERR))
		dev_err_ratelimited(&ndev->dev,
				    "RX error 0x%x RX_CHNL_STS=0x%08x\n",
				    status, lp->dma_in(lp, RX_CHNL_STS));
>>>>>>> upstream/android-13

	return IRQ_HANDLED;
}

static int temac_open(struct net_device *ndev)
{
	struct temac_local *lp = netdev_priv(ndev);
	struct phy_device *phydev = NULL;
	int rc;

	dev_dbg(&ndev->dev, "temac_open()\n");

	if (lp->phy_node) {
		phydev = of_phy_connect(lp->ndev, lp->phy_node,
					temac_adjust_link, 0, 0);
		if (!phydev) {
			dev_err(lp->dev, "of_phy_connect() failed\n");
			return -ENODEV;
		}
<<<<<<< HEAD

=======
		phy_start(phydev);
	} else if (strlen(lp->phy_name) > 0) {
		phydev = phy_connect(lp->ndev, lp->phy_name, temac_adjust_link,
				     lp->phy_interface);
		if (IS_ERR(phydev)) {
			dev_err(lp->dev, "phy_connect() failed\n");
			return PTR_ERR(phydev);
		}
>>>>>>> upstream/android-13
		phy_start(phydev);
	}

	temac_device_reset(ndev);

	rc = request_irq(lp->tx_irq, ll_temac_tx_irq, 0, ndev->name, ndev);
	if (rc)
		goto err_tx_irq;
	rc = request_irq(lp->rx_irq, ll_temac_rx_irq, 0, ndev->name, ndev);
	if (rc)
		goto err_rx_irq;

	return 0;

 err_rx_irq:
	free_irq(lp->tx_irq, ndev);
 err_tx_irq:
	if (phydev)
		phy_disconnect(phydev);
	dev_err(lp->dev, "request_irq() failed\n");
	return rc;
}

static int temac_stop(struct net_device *ndev)
{
	struct temac_local *lp = netdev_priv(ndev);
	struct phy_device *phydev = ndev->phydev;

	dev_dbg(&ndev->dev, "temac_close()\n");

<<<<<<< HEAD
=======
	cancel_delayed_work_sync(&lp->restart_work);

>>>>>>> upstream/android-13
	free_irq(lp->tx_irq, ndev);
	free_irq(lp->rx_irq, ndev);

	if (phydev)
		phy_disconnect(phydev);

	temac_dma_bd_release(ndev);

	return 0;
}

#ifdef CONFIG_NET_POLL_CONTROLLER
static void
temac_poll_controller(struct net_device *ndev)
{
	struct temac_local *lp = netdev_priv(ndev);

	disable_irq(lp->tx_irq);
	disable_irq(lp->rx_irq);

	ll_temac_rx_irq(lp->tx_irq, ndev);
	ll_temac_tx_irq(lp->rx_irq, ndev);

	enable_irq(lp->tx_irq);
	enable_irq(lp->rx_irq);
}
#endif

<<<<<<< HEAD
static int temac_ioctl(struct net_device *ndev, struct ifreq *rq, int cmd)
{
	if (!netif_running(ndev))
		return -EINVAL;

	if (!ndev->phydev)
		return -EINVAL;

	return phy_mii_ioctl(ndev->phydev, rq, cmd);
}

=======
>>>>>>> upstream/android-13
static const struct net_device_ops temac_netdev_ops = {
	.ndo_open = temac_open,
	.ndo_stop = temac_stop,
	.ndo_start_xmit = temac_start_xmit,
<<<<<<< HEAD
	.ndo_set_mac_address = temac_set_mac_address,
	.ndo_validate_addr = eth_validate_addr,
	.ndo_do_ioctl = temac_ioctl,
=======
	.ndo_set_rx_mode = temac_set_multicast_list,
	.ndo_set_mac_address = temac_set_mac_address,
	.ndo_validate_addr = eth_validate_addr,
	.ndo_eth_ioctl = phy_do_ioctl_running,
>>>>>>> upstream/android-13
#ifdef CONFIG_NET_POLL_CONTROLLER
	.ndo_poll_controller = temac_poll_controller,
#endif
};

/* ---------------------------------------------------------------------
 * SYSFS device attributes
 */
static ssize_t temac_show_llink_regs(struct device *dev,
				     struct device_attribute *attr, char *buf)
{
	struct net_device *ndev = dev_get_drvdata(dev);
	struct temac_local *lp = netdev_priv(ndev);
	int i, len = 0;

	for (i = 0; i < 0x11; i++)
		len += sprintf(buf + len, "%.8x%s", lp->dma_in(lp, i),
			       (i % 8) == 7 ? "\n" : " ");
	len += sprintf(buf + len, "\n");

	return len;
}

static DEVICE_ATTR(llink_regs, 0440, temac_show_llink_regs, NULL);

static struct attribute *temac_device_attrs[] = {
	&dev_attr_llink_regs.attr,
	NULL,
};

static const struct attribute_group temac_attr_group = {
	.attrs = temac_device_attrs,
};

<<<<<<< HEAD
/* ethtool support */
static const struct ethtool_ops temac_ethtool_ops = {
=======
/* ---------------------------------------------------------------------
 * ethtool support
 */

static void ll_temac_ethtools_get_ringparam(struct net_device *ndev,
					    struct ethtool_ringparam *ering)
{
	struct temac_local *lp = netdev_priv(ndev);

	ering->rx_max_pending = RX_BD_NUM_MAX;
	ering->rx_mini_max_pending = 0;
	ering->rx_jumbo_max_pending = 0;
	ering->tx_max_pending = TX_BD_NUM_MAX;
	ering->rx_pending = lp->rx_bd_num;
	ering->rx_mini_pending = 0;
	ering->rx_jumbo_pending = 0;
	ering->tx_pending = lp->tx_bd_num;
}

static int ll_temac_ethtools_set_ringparam(struct net_device *ndev,
					   struct ethtool_ringparam *ering)
{
	struct temac_local *lp = netdev_priv(ndev);

	if (ering->rx_pending > RX_BD_NUM_MAX ||
	    ering->rx_mini_pending ||
	    ering->rx_jumbo_pending ||
	    ering->rx_pending > TX_BD_NUM_MAX)
		return -EINVAL;

	if (netif_running(ndev))
		return -EBUSY;

	lp->rx_bd_num = ering->rx_pending;
	lp->tx_bd_num = ering->tx_pending;
	return 0;
}

static int
ll_temac_ethtools_get_coalesce(struct net_device *ndev,
			       struct ethtool_coalesce *ec,
			       struct kernel_ethtool_coalesce *kernel_coal,
			       struct netlink_ext_ack *extack)
{
	struct temac_local *lp = netdev_priv(ndev);

	ec->rx_max_coalesced_frames = lp->coalesce_count_rx;
	ec->tx_max_coalesced_frames = lp->coalesce_count_tx;
	ec->rx_coalesce_usecs = (lp->coalesce_delay_rx * 512) / 100;
	ec->tx_coalesce_usecs = (lp->coalesce_delay_tx * 512) / 100;
	return 0;
}

static int
ll_temac_ethtools_set_coalesce(struct net_device *ndev,
			       struct ethtool_coalesce *ec,
			       struct kernel_ethtool_coalesce *kernel_coal,
			       struct netlink_ext_ack *extack)
{
	struct temac_local *lp = netdev_priv(ndev);

	if (netif_running(ndev)) {
		netdev_err(ndev,
			   "Please stop netif before applying configuration\n");
		return -EFAULT;
	}

	if (ec->rx_max_coalesced_frames)
		lp->coalesce_count_rx = ec->rx_max_coalesced_frames;
	if (ec->tx_max_coalesced_frames)
		lp->coalesce_count_tx = ec->tx_max_coalesced_frames;
	/* With typical LocalLink clock speed of 200 MHz and
	 * C_PRESCALAR=1023, each delay count corresponds to 5.12 us.
	 */
	if (ec->rx_coalesce_usecs)
		lp->coalesce_delay_rx =
			min(255U, (ec->rx_coalesce_usecs * 100) / 512);
	if (ec->tx_coalesce_usecs)
		lp->coalesce_delay_tx =
			min(255U, (ec->tx_coalesce_usecs * 100) / 512);

	return 0;
}

static const struct ethtool_ops temac_ethtool_ops = {
	.supported_coalesce_params = ETHTOOL_COALESCE_USECS |
				     ETHTOOL_COALESCE_MAX_FRAMES,
>>>>>>> upstream/android-13
	.nway_reset = phy_ethtool_nway_reset,
	.get_link = ethtool_op_get_link,
	.get_ts_info = ethtool_op_get_ts_info,
	.get_link_ksettings = phy_ethtool_get_link_ksettings,
	.set_link_ksettings = phy_ethtool_set_link_ksettings,
<<<<<<< HEAD
};

static int temac_of_probe(struct platform_device *op)
{
	struct device_node *np;
	struct temac_local *lp;
	struct net_device *ndev;
	const void *addr;
	__be32 *p;
	int rc = 0;

	/* Init network device structure */
	ndev = alloc_etherdev(sizeof(*lp));
	if (!ndev)
		return -ENOMEM;

	platform_set_drvdata(op, ndev);
	SET_NETDEV_DEV(ndev, &op->dev);
	ndev->flags &= ~IFF_MULTICAST;  /* clear multicast */
=======
	.get_ringparam	= ll_temac_ethtools_get_ringparam,
	.set_ringparam	= ll_temac_ethtools_set_ringparam,
	.get_coalesce	= ll_temac_ethtools_get_coalesce,
	.set_coalesce	= ll_temac_ethtools_set_coalesce,
};

static int temac_probe(struct platform_device *pdev)
{
	struct ll_temac_platform_data *pdata = dev_get_platdata(&pdev->dev);
	struct device_node *temac_np = dev_of_node(&pdev->dev), *dma_np;
	struct temac_local *lp;
	struct net_device *ndev;
	u8 addr[ETH_ALEN];
	__be32 *p;
	bool little_endian;
	int rc = 0;

	/* Init network device structure */
	ndev = devm_alloc_etherdev(&pdev->dev, sizeof(*lp));
	if (!ndev)
		return -ENOMEM;

	platform_set_drvdata(pdev, ndev);
	SET_NETDEV_DEV(ndev, &pdev->dev);
>>>>>>> upstream/android-13
	ndev->features = NETIF_F_SG;
	ndev->netdev_ops = &temac_netdev_ops;
	ndev->ethtool_ops = &temac_ethtool_ops;
#if 0
	ndev->features |= NETIF_F_IP_CSUM; /* Can checksum TCP/UDP over IPv4. */
	ndev->features |= NETIF_F_HW_CSUM; /* Can checksum all the packets. */
	ndev->features |= NETIF_F_IPV6_CSUM; /* Can checksum IPV6 TCP/UDP */
	ndev->features |= NETIF_F_HIGHDMA; /* Can DMA to high memory. */
	ndev->features |= NETIF_F_HW_VLAN_CTAG_TX; /* Transmit VLAN hw accel */
	ndev->features |= NETIF_F_HW_VLAN_CTAG_RX; /* Receive VLAN hw acceleration */
	ndev->features |= NETIF_F_HW_VLAN_CTAG_FILTER; /* Receive VLAN filtering */
	ndev->features |= NETIF_F_VLAN_CHALLENGED; /* cannot handle VLAN pkts */
	ndev->features |= NETIF_F_GSO; /* Enable software GSO. */
	ndev->features |= NETIF_F_MULTI_QUEUE; /* Has multiple TX/RX queues */
	ndev->features |= NETIF_F_LRO; /* large receive offload */
#endif

	/* setup temac private info structure */
	lp = netdev_priv(ndev);
	lp->ndev = ndev;
<<<<<<< HEAD
	lp->dev = &op->dev;
	lp->options = XTE_OPTION_DEFAULTS;
	spin_lock_init(&lp->rx_lock);
	mutex_init(&lp->indirect_mutex);

	/* map device registers */
	lp->regs = of_iomap(op->dev.of_node, 0);
	if (!lp->regs) {
		dev_err(&op->dev, "could not map temac regs.\n");
		rc = -ENOMEM;
		goto nodev;
=======
	lp->dev = &pdev->dev;
	lp->options = XTE_OPTION_DEFAULTS;
	lp->rx_bd_num = RX_BD_NUM_DEFAULT;
	lp->tx_bd_num = TX_BD_NUM_DEFAULT;
	spin_lock_init(&lp->rx_lock);
	INIT_DELAYED_WORK(&lp->restart_work, ll_temac_restart_work_func);

	/* Setup mutex for synchronization of indirect register access */
	if (pdata) {
		if (!pdata->indirect_lock) {
			dev_err(&pdev->dev,
				"indirect_lock missing in platform_data\n");
			return -EINVAL;
		}
		lp->indirect_lock = pdata->indirect_lock;
	} else {
		lp->indirect_lock = devm_kmalloc(&pdev->dev,
						 sizeof(*lp->indirect_lock),
						 GFP_KERNEL);
		if (!lp->indirect_lock)
			return -ENOMEM;
		spin_lock_init(lp->indirect_lock);
	}

	/* map device registers */
	lp->regs = devm_platform_ioremap_resource_byname(pdev, 0);
	if (IS_ERR(lp->regs)) {
		dev_err(&pdev->dev, "could not map TEMAC registers\n");
		return -ENOMEM;
	}

	/* Select register access functions with the specified
	 * endianness mode.  Default for OF devices is big-endian.
	 */
	little_endian = false;
	if (temac_np) {
		if (of_get_property(temac_np, "little-endian", NULL))
			little_endian = true;
	} else if (pdata) {
		little_endian = pdata->reg_little_endian;
	}
	if (little_endian) {
		lp->temac_ior = _temac_ior_le;
		lp->temac_iow = _temac_iow_le;
	} else {
		lp->temac_ior = _temac_ior_be;
		lp->temac_iow = _temac_iow_be;
>>>>>>> upstream/android-13
	}

	/* Setup checksum offload, but default to off if not specified */
	lp->temac_features = 0;
<<<<<<< HEAD
	p = (__be32 *)of_get_property(op->dev.of_node, "xlnx,txcsum", NULL);
	if (p && be32_to_cpu(*p)) {
		lp->temac_features |= TEMAC_FEATURE_TX_CSUM;
		/* Can checksum TCP/UDP over IPv4. */
		ndev->features |= NETIF_F_IP_CSUM;
	}
	p = (__be32 *)of_get_property(op->dev.of_node, "xlnx,rxcsum", NULL);
	if (p && be32_to_cpu(*p))
		lp->temac_features |= TEMAC_FEATURE_RX_CSUM;

	/* Find the DMA node, map the DMA registers, and decode the DMA IRQs */
	np = of_parse_phandle(op->dev.of_node, "llink-connected", 0);
	if (!np) {
		dev_err(&op->dev, "could not find DMA node\n");
		rc = -ENODEV;
		goto err_iounmap;
	}

	/* Setup the DMA register accesses, could be DCR or memory mapped */
	if (temac_dcr_setup(lp, op, np)) {

		/* no DCR in the device tree, try non-DCR */
		lp->sdma_regs = of_iomap(np, 0);
		if (lp->sdma_regs) {
			lp->dma_in = temac_dma_in32;
			lp->dma_out = temac_dma_out32;
			dev_dbg(&op->dev, "MEM base: %p\n", lp->sdma_regs);
		} else {
			dev_err(&op->dev, "unable to map DMA registers\n");
			of_node_put(np);
			goto err_iounmap;
		}
	}

	lp->rx_irq = irq_of_parse_and_map(np, 0);
	lp->tx_irq = irq_of_parse_and_map(np, 1);

	of_node_put(np); /* Finished with the DMA node; drop the reference */

	if (!lp->rx_irq || !lp->tx_irq) {
		dev_err(&op->dev, "could not determine irqs\n");
		rc = -ENOMEM;
		goto err_iounmap_2;
	}


	/* Retrieve the MAC address */
	addr = of_get_mac_address(op->dev.of_node);
	if (!addr) {
		dev_err(&op->dev, "could not find MAC address\n");
		rc = -ENODEV;
		goto err_iounmap_2;
	}
	temac_init_mac_address(ndev, addr);

	rc = temac_mdio_setup(lp, op->dev.of_node);
	if (rc)
		dev_warn(&op->dev, "error registering MDIO bus\n");

	lp->phy_node = of_parse_phandle(op->dev.of_node, "phy-handle", 0);
	if (lp->phy_node)
		dev_dbg(lp->dev, "using PHY node %pOF (%p)\n", np, np);
=======
	if (temac_np) {
		p = (__be32 *)of_get_property(temac_np, "xlnx,txcsum", NULL);
		if (p && be32_to_cpu(*p))
			lp->temac_features |= TEMAC_FEATURE_TX_CSUM;
		p = (__be32 *)of_get_property(temac_np, "xlnx,rxcsum", NULL);
		if (p && be32_to_cpu(*p))
			lp->temac_features |= TEMAC_FEATURE_RX_CSUM;
	} else if (pdata) {
		if (pdata->txcsum)
			lp->temac_features |= TEMAC_FEATURE_TX_CSUM;
		if (pdata->rxcsum)
			lp->temac_features |= TEMAC_FEATURE_RX_CSUM;
	}
	if (lp->temac_features & TEMAC_FEATURE_TX_CSUM)
		/* Can checksum TCP/UDP over IPv4. */
		ndev->features |= NETIF_F_IP_CSUM;

	/* Defaults for IRQ delay/coalescing setup.  These are
	 * configuration values, so does not belong in device-tree.
	 */
	lp->coalesce_delay_tx = 0x10;
	lp->coalesce_count_tx = 0x22;
	lp->coalesce_delay_rx = 0xff;
	lp->coalesce_count_rx = 0x07;

	/* Setup LocalLink DMA */
	if (temac_np) {
		/* Find the DMA node, map the DMA registers, and
		 * decode the DMA IRQs.
		 */
		dma_np = of_parse_phandle(temac_np, "llink-connected", 0);
		if (!dma_np) {
			dev_err(&pdev->dev, "could not find DMA node\n");
			return -ENODEV;
		}

		/* Setup the DMA register accesses, could be DCR or
		 * memory mapped.
		 */
		if (temac_dcr_setup(lp, pdev, dma_np)) {
			/* no DCR in the device tree, try non-DCR */
			lp->sdma_regs = devm_of_iomap(&pdev->dev, dma_np, 0,
						      NULL);
			if (IS_ERR(lp->sdma_regs)) {
				dev_err(&pdev->dev,
					"unable to map DMA registers\n");
				of_node_put(dma_np);
				return PTR_ERR(lp->sdma_regs);
			}
			if (of_get_property(dma_np, "little-endian", NULL)) {
				lp->dma_in = temac_dma_in32_le;
				lp->dma_out = temac_dma_out32_le;
			} else {
				lp->dma_in = temac_dma_in32_be;
				lp->dma_out = temac_dma_out32_be;
			}
			dev_dbg(&pdev->dev, "MEM base: %p\n", lp->sdma_regs);
		}

		/* Get DMA RX and TX interrupts */
		lp->rx_irq = irq_of_parse_and_map(dma_np, 0);
		lp->tx_irq = irq_of_parse_and_map(dma_np, 1);

		/* Finished with the DMA node; drop the reference */
		of_node_put(dma_np);
	} else if (pdata) {
		/* 2nd memory resource specifies DMA registers */
		lp->sdma_regs = devm_platform_ioremap_resource(pdev, 1);
		if (IS_ERR(lp->sdma_regs)) {
			dev_err(&pdev->dev,
				"could not map DMA registers\n");
			return PTR_ERR(lp->sdma_regs);
		}
		if (pdata->dma_little_endian) {
			lp->dma_in = temac_dma_in32_le;
			lp->dma_out = temac_dma_out32_le;
		} else {
			lp->dma_in = temac_dma_in32_be;
			lp->dma_out = temac_dma_out32_be;
		}

		/* Get DMA RX and TX interrupts */
		lp->rx_irq = platform_get_irq(pdev, 0);
		lp->tx_irq = platform_get_irq(pdev, 1);

		/* IRQ delay/coalescing setup */
		if (pdata->tx_irq_timeout || pdata->tx_irq_count) {
			lp->coalesce_delay_tx = pdata->tx_irq_timeout;
			lp->coalesce_count_tx = pdata->tx_irq_count;
		}
		if (pdata->rx_irq_timeout || pdata->rx_irq_count) {
			lp->coalesce_delay_rx = pdata->rx_irq_timeout;
			lp->coalesce_count_rx = pdata->rx_irq_count;
		}
	}

	/* Error handle returned DMA RX and TX interrupts */
	if (lp->rx_irq < 0) {
		if (lp->rx_irq != -EPROBE_DEFER)
			dev_err(&pdev->dev, "could not get DMA RX irq\n");
		return lp->rx_irq;
	}
	if (lp->tx_irq < 0) {
		if (lp->tx_irq != -EPROBE_DEFER)
			dev_err(&pdev->dev, "could not get DMA TX irq\n");
		return lp->tx_irq;
	}

	if (temac_np) {
		/* Retrieve the MAC address */
		rc = of_get_mac_address(temac_np, addr);
		if (rc) {
			dev_err(&pdev->dev, "could not find MAC address\n");
			return -ENODEV;
		}
		temac_init_mac_address(ndev, addr);
	} else if (pdata) {
		temac_init_mac_address(ndev, pdata->mac_addr);
	}

	rc = temac_mdio_setup(lp, pdev);
	if (rc)
		dev_warn(&pdev->dev, "error registering MDIO bus\n");

	if (temac_np) {
		lp->phy_node = of_parse_phandle(temac_np, "phy-handle", 0);
		if (lp->phy_node)
			dev_dbg(lp->dev, "using PHY node %pOF\n", temac_np);
	} else if (pdata) {
		snprintf(lp->phy_name, sizeof(lp->phy_name),
			 PHY_ID_FMT, lp->mii_bus->id, pdata->phy_addr);
		lp->phy_interface = pdata->phy_interface;
	}
>>>>>>> upstream/android-13

	/* Add the device attributes */
	rc = sysfs_create_group(&lp->dev->kobj, &temac_attr_group);
	if (rc) {
		dev_err(lp->dev, "Error creating sysfs files\n");
<<<<<<< HEAD
		goto err_iounmap_2;
=======
		goto err_sysfs_create;
>>>>>>> upstream/android-13
	}

	rc = register_netdev(lp->ndev);
	if (rc) {
		dev_err(lp->dev, "register_netdev() error (%i)\n", rc);
		goto err_register_ndev;
	}

	return 0;

<<<<<<< HEAD
 err_register_ndev:
	sysfs_remove_group(&lp->dev->kobj, &temac_attr_group);
 err_iounmap_2:
	if (lp->sdma_regs)
		iounmap(lp->sdma_regs);
 err_iounmap:
	iounmap(lp->regs);
 nodev:
	free_netdev(ndev);
	ndev = NULL;
	return rc;
}

static int temac_of_remove(struct platform_device *op)
{
	struct net_device *ndev = platform_get_drvdata(op);
	struct temac_local *lp = netdev_priv(ndev);

	temac_mdio_teardown(lp);
	unregister_netdev(ndev);
	sysfs_remove_group(&lp->dev->kobj, &temac_attr_group);
	of_node_put(lp->phy_node);
	lp->phy_node = NULL;
	iounmap(lp->regs);
	if (lp->sdma_regs)
		iounmap(lp->sdma_regs);
	free_netdev(ndev);
=======
err_register_ndev:
	sysfs_remove_group(&lp->dev->kobj, &temac_attr_group);
err_sysfs_create:
	if (lp->phy_node)
		of_node_put(lp->phy_node);
	temac_mdio_teardown(lp);
	return rc;
}

static int temac_remove(struct platform_device *pdev)
{
	struct net_device *ndev = platform_get_drvdata(pdev);
	struct temac_local *lp = netdev_priv(ndev);

	unregister_netdev(ndev);
	sysfs_remove_group(&lp->dev->kobj, &temac_attr_group);
	if (lp->phy_node)
		of_node_put(lp->phy_node);
	temac_mdio_teardown(lp);
>>>>>>> upstream/android-13
	return 0;
}

static const struct of_device_id temac_of_match[] = {
	{ .compatible = "xlnx,xps-ll-temac-1.01.b", },
	{ .compatible = "xlnx,xps-ll-temac-2.00.a", },
	{ .compatible = "xlnx,xps-ll-temac-2.02.a", },
	{ .compatible = "xlnx,xps-ll-temac-2.03.a", },
	{},
};
MODULE_DEVICE_TABLE(of, temac_of_match);

<<<<<<< HEAD
static struct platform_driver temac_of_driver = {
	.probe = temac_of_probe,
	.remove = temac_of_remove,
=======
static struct platform_driver temac_driver = {
	.probe = temac_probe,
	.remove = temac_remove,
>>>>>>> upstream/android-13
	.driver = {
		.name = "xilinx_temac",
		.of_match_table = temac_of_match,
	},
};

<<<<<<< HEAD
module_platform_driver(temac_of_driver);
=======
module_platform_driver(temac_driver);
>>>>>>> upstream/android-13

MODULE_DESCRIPTION("Xilinx LL_TEMAC Ethernet driver");
MODULE_AUTHOR("Yoshio Kashiwagi");
MODULE_LICENSE("GPL");
