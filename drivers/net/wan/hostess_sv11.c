<<<<<<< HEAD
/*
 *	Comtrol SV11 card driver
=======
// SPDX-License-Identifier: GPL-2.0-only
/*	Comtrol SV11 card driver
>>>>>>> upstream/android-13
 *
 *	This is a slightly odd Z85230 synchronous driver. All you need to
 *	know basically is
 *
 *	Its a genuine Z85230
 *
 *	It supports DMA using two DMA channels in SYNC mode. The driver doesn't
 *	use these facilities
<<<<<<< HEAD
 *	
=======
 *
>>>>>>> upstream/android-13
 *	The control port is at io+1, the data at io+3 and turning off the DMA
 *	is done by writing 0 to io+4
 *
 *	The hardware does the bus handling to avoid the need for delays between
 *	touching control registers.
 *
 *	Port B isn't wired (why - beats me)
 *
 *	Generic HDLC port Copyright (C) 2008 Krzysztof Halasa <khc@pm.waw.pl>
 */

#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/mm.h>
#include <linux/net.h>
#include <linux/skbuff.h>
#include <linux/netdevice.h>
#include <linux/if_arp.h>
#include <linux/delay.h>
#include <linux/hdlc.h>
#include <linux/ioport.h>
#include <linux/slab.h>
#include <net/arp.h>

#include <asm/irq.h>
#include <asm/io.h>
#include <asm/dma.h>
#include <asm/byteorder.h>
#include "z85230.h"

static int dma;

<<<<<<< HEAD
/*
 *	Network driver support routines
 */

static inline struct z8530_dev* dev_to_sv(struct net_device *dev)
=======
/*	Network driver support routines
 */

static inline struct z8530_dev *dev_to_sv(struct net_device *dev)
>>>>>>> upstream/android-13
{
	return (struct z8530_dev *)dev_to_hdlc(dev)->priv;
}

<<<<<<< HEAD
/*
 *	Frame receive. Simple for our card as we do HDLC and there
=======
/*	Frame receive. Simple for our card as we do HDLC and there
>>>>>>> upstream/android-13
 *	is no funny garbage involved
 */

static void hostess_input(struct z8530_channel *c, struct sk_buff *skb)
{
	/* Drop the CRC - it's not a good idea to try and negotiate it ;) */
	skb_trim(skb, skb->len - 2);
	skb->protocol = hdlc_type_trans(skb, c->netdevice);
	skb_reset_mac_header(skb);
	skb->dev = c->netdevice;
<<<<<<< HEAD
	/*
	 *	Send it to the PPP layer. We don't have time to process
=======
	/*	Send it to the PPP layer. We don't have time to process
>>>>>>> upstream/android-13
	 *	it right now.
	 */
	netif_rx(skb);
}

<<<<<<< HEAD
/*
 *	We've been placed in the UP state
=======
/*	We've been placed in the UP state
>>>>>>> upstream/android-13
 */

static int hostess_open(struct net_device *d)
{
	struct z8530_dev *sv11 = dev_to_sv(d);
	int err = -1;

<<<<<<< HEAD
	/*
	 *	Link layer up
	 */
	switch (dma) {
		case 0:
			err = z8530_sync_open(d, &sv11->chanA);
			break;
		case 1:
			err = z8530_sync_dma_open(d, &sv11->chanA);
			break;
		case 2:
			err = z8530_sync_txdma_open(d, &sv11->chanA);
			break;
=======
	/*	Link layer up
	 */
	switch (dma) {
	case 0:
		err = z8530_sync_open(d, &sv11->chanA);
		break;
	case 1:
		err = z8530_sync_dma_open(d, &sv11->chanA);
		break;
	case 2:
		err = z8530_sync_txdma_open(d, &sv11->chanA);
		break;
>>>>>>> upstream/android-13
	}

	if (err)
		return err;

	err = hdlc_open(d);
	if (err) {
		switch (dma) {
<<<<<<< HEAD
			case 0:
				z8530_sync_close(d, &sv11->chanA);
				break;
			case 1:
				z8530_sync_dma_close(d, &sv11->chanA);
				break;
			case 2:
				z8530_sync_txdma_close(d, &sv11->chanA);
				break;
=======
		case 0:
			z8530_sync_close(d, &sv11->chanA);
			break;
		case 1:
			z8530_sync_dma_close(d, &sv11->chanA);
			break;
		case 2:
			z8530_sync_txdma_close(d, &sv11->chanA);
			break;
>>>>>>> upstream/android-13
		}
		return err;
	}
	sv11->chanA.rx_function = hostess_input;

	/*
	 *	Go go go
	 */

	netif_start_queue(d);
	return 0;
}

static int hostess_close(struct net_device *d)
{
	struct z8530_dev *sv11 = dev_to_sv(d);
<<<<<<< HEAD
	/*
	 *	Discard new frames
=======
	/*	Discard new frames
>>>>>>> upstream/android-13
	 */
	sv11->chanA.rx_function = z8530_null_rx;

	hdlc_close(d);
	netif_stop_queue(d);

	switch (dma) {
<<<<<<< HEAD
		case 0:
			z8530_sync_close(d, &sv11->chanA);
			break;
		case 1:
			z8530_sync_dma_close(d, &sv11->chanA);
			break;
		case 2:
			z8530_sync_txdma_close(d, &sv11->chanA);
			break;
=======
	case 0:
		z8530_sync_close(d, &sv11->chanA);
		break;
	case 1:
		z8530_sync_dma_close(d, &sv11->chanA);
		break;
	case 2:
		z8530_sync_txdma_close(d, &sv11->chanA);
		break;
>>>>>>> upstream/android-13
	}
	return 0;
}

<<<<<<< HEAD
static int hostess_ioctl(struct net_device *d, struct ifreq *ifr, int cmd)
{
	/* struct z8530_dev *sv11=dev_to_sv(d);
	   z8530_ioctl(d,&sv11->chanA,ifr,cmd) */
	return hdlc_ioctl(d, ifr, cmd);
}

/*
 *	Passed network frames, fire them downwind.
 */

static netdev_tx_t hostess_queue_xmit(struct sk_buff *skb,
					    struct net_device *d)
=======
/*	Passed network frames, fire them downwind.
 */

static netdev_tx_t hostess_queue_xmit(struct sk_buff *skb,
				      struct net_device *d)
>>>>>>> upstream/android-13
{
	return z8530_queue_xmit(&dev_to_sv(d)->chanA, skb);
}

static int hostess_attach(struct net_device *dev, unsigned short encoding,
			  unsigned short parity)
{
	if (encoding == ENCODING_NRZ && parity == PARITY_CRC16_PR1_CCITT)
		return 0;
	return -EINVAL;
}

<<<<<<< HEAD
/*
 *	Description block for a Comtrol Hostess SV11 card
=======
/*	Description block for a Comtrol Hostess SV11 card
>>>>>>> upstream/android-13
 */

static const struct net_device_ops hostess_ops = {
	.ndo_open       = hostess_open,
	.ndo_stop       = hostess_close,
	.ndo_start_xmit = hdlc_start_xmit,
<<<<<<< HEAD
	.ndo_do_ioctl   = hostess_ioctl,
=======
	.ndo_siocwandev = hdlc_ioctl,
>>>>>>> upstream/android-13
};

static struct z8530_dev *sv11_init(int iobase, int irq)
{
	struct z8530_dev *sv;
	struct net_device *netdev;
<<<<<<< HEAD
	/*
	 *	Get the needed I/O space
=======
	/*	Get the needed I/O space
>>>>>>> upstream/android-13
	 */

	if (!request_region(iobase, 8, "Comtrol SV11")) {
		pr_warn("I/O 0x%X already in use\n", iobase);
		return NULL;
	}

	sv = kzalloc(sizeof(struct z8530_dev), GFP_KERNEL);
	if (!sv)
		goto err_kzalloc;

<<<<<<< HEAD
	/*
	 *	Stuff in the I/O addressing
=======
	/*	Stuff in the I/O addressing
>>>>>>> upstream/android-13
	 */

	sv->active = 0;

	sv->chanA.ctrlio = iobase + 1;
	sv->chanA.dataio = iobase + 3;
	sv->chanB.ctrlio = -1;
	sv->chanB.dataio = -1;
	sv->chanA.irqs = &z8530_nop;
	sv->chanB.irqs = &z8530_nop;

	outb(0, iobase + 4);		/* DMA off */

	/* We want a fast IRQ for this device. Actually we'd like an even faster
<<<<<<< HEAD
	   IRQ ;) - This is one driver RtLinux is made for */
=======
	 * IRQ ;) - This is one driver RtLinux is made for
	 */
>>>>>>> upstream/android-13

	if (request_irq(irq, z8530_interrupt, 0,
			"Hostess SV11", sv) < 0) {
		pr_warn("IRQ %d already in use\n", irq);
		goto err_irq;
	}

	sv->irq = irq;
	sv->chanA.private = sv;
	sv->chanA.dev = sv;
	sv->chanB.dev = sv;

	if (dma) {
<<<<<<< HEAD
		/*
		 *	You can have DMA off or 1 and 3 thats the lot
=======
		/*	You can have DMA off or 1 and 3 thats the lot
>>>>>>> upstream/android-13
		 *	on the Comtrol.
		 */
		sv->chanA.txdma = 3;
		sv->chanA.rxdma = 1;
		outb(0x03 | 0x08, iobase + 4);		/* DMA on */
		if (request_dma(sv->chanA.txdma, "Hostess SV/11 (TX)"))
			goto err_txdma;

		if (dma == 1)
			if (request_dma(sv->chanA.rxdma, "Hostess SV/11 (RX)"))
				goto err_rxdma;
	}

	/* Kill our private IRQ line the hostess can end up chattering
<<<<<<< HEAD
	   until the configuration is set */
	disable_irq(irq);

	/*
	 *	Begin normal initialise
=======
	 * until the configuration is set
	 */
	disable_irq(irq);

	/*	Begin normal initialise
>>>>>>> upstream/android-13
	 */

	if (z8530_init(sv)) {
		pr_err("Z8530 series device not found\n");
		enable_irq(irq);
		goto free_dma;
	}
	z8530_channel_load(&sv->chanB, z8530_dead_port);
	if (sv->type == Z85C30)
		z8530_channel_load(&sv->chanA, z8530_hdlc_kilostream);
	else
		z8530_channel_load(&sv->chanA, z8530_hdlc_kilostream_85230);

	enable_irq(irq);

<<<<<<< HEAD
	/*
	 *	Now we can take the IRQ
=======
	/*	Now we can take the IRQ
>>>>>>> upstream/android-13
	 */

	sv->chanA.netdevice = netdev = alloc_hdlcdev(sv);
	if (!netdev)
		goto free_dma;

	dev_to_hdlc(netdev)->attach = hostess_attach;
	dev_to_hdlc(netdev)->xmit = hostess_queue_xmit;
	netdev->netdev_ops = &hostess_ops;
	netdev->base_addr = iobase;
	netdev->irq = irq;

	if (register_hdlc_device(netdev)) {
		pr_err("unable to register HDLC device\n");
		free_netdev(netdev);
		goto free_dma;
	}

	z8530_describe(sv, "I/O", iobase);
	sv->active = 1;
	return sv;

free_dma:
	if (dma == 1)
		free_dma(sv->chanA.rxdma);
err_rxdma:
	if (dma)
		free_dma(sv->chanA.txdma);
err_txdma:
	free_irq(irq, sv);
err_irq:
	kfree(sv);
err_kzalloc:
	release_region(iobase, 8);
	return NULL;
}

static void sv11_shutdown(struct z8530_dev *dev)
{
	unregister_hdlc_device(dev->chanA.netdevice);
	z8530_shutdown(dev);
	free_irq(dev->irq, dev);
	if (dma) {
		if (dma == 1)
			free_dma(dev->chanA.rxdma);
		free_dma(dev->chanA.txdma);
	}
	release_region(dev->chanA.ctrlio - 1, 8);
	free_netdev(dev->chanA.netdevice);
	kfree(dev);
}

static int io = 0x200;
static int irq = 9;

module_param_hw(io, int, ioport, 0);
MODULE_PARM_DESC(io, "The I/O base of the Comtrol Hostess SV11 card");
module_param_hw(dma, int, dma, 0);
MODULE_PARM_DESC(dma, "Set this to 1 to use DMA1/DMA3 for TX/RX");
module_param_hw(irq, int, irq, 0);
MODULE_PARM_DESC(irq, "The interrupt line setting for the Comtrol Hostess SV11 card");

MODULE_AUTHOR("Alan Cox");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Modular driver for the Comtrol Hostess SV11");

static struct z8530_dev *sv11_unit;

<<<<<<< HEAD
int init_module(void)
{
	if ((sv11_unit = sv11_init(io, irq)) == NULL)
		return -ENODEV;
	return 0;
}

void cleanup_module(void)
=======
static int sv11_module_init(void)
{
	sv11_unit = sv11_init(io, irq);
	if (!sv11_unit)
		return -ENODEV;
	return 0;
}
module_init(sv11_module_init);

static void sv11_module_cleanup(void)
>>>>>>> upstream/android-13
{
	if (sv11_unit)
		sv11_shutdown(sv11_unit);
}
<<<<<<< HEAD
=======
module_exit(sv11_module_cleanup);
>>>>>>> upstream/android-13
