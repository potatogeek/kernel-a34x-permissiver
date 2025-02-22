<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> upstream/android-13
/*
 * ixp4xx PATA/Compact Flash driver
 * Copyright (C) 2006-07 Tower Technologies
 * Author: Alessandro Zummo <a.zummo@towertech.it>
 *
 * An ATA driver to handle a Compact Flash connected
 * to the ixp4xx expansion bus in TrueIDE mode. The CF
 * must have it chip selects connected to two CS lines
 * on the ixp4xx. In the irq is not available, you might
 * want to modify both this driver and libata to run in
 * polling mode.
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 */

#include <linux/kernel.h>
=======
 */

#include <linux/kernel.h>
#include <linux/mfd/syscon.h>
>>>>>>> upstream/android-13
#include <linux/module.h>
#include <linux/libata.h>
#include <linux/irq.h>
#include <linux/platform_device.h>
<<<<<<< HEAD
#include <scsi/scsi_host.h>

#define DRV_NAME	"pata_ixp4xx_cf"
#define DRV_VERSION	"0.2"

static int ixp4xx_set_mode(struct ata_link *link, struct ata_device **error)
{
	struct ata_device *dev;

	ata_for_each_dev(dev, link, ENABLED) {
		ata_dev_info(dev, "configured for PIO0\n");
		dev->pio_mode = XFER_PIO_0;
		dev->xfer_mode = XFER_PIO_0;
		dev->xfer_shift = ATA_SHIFT_PIO;
		dev->flags |= ATA_DFLAG_PIO;
	}
	return 0;
}

static unsigned int ixp4xx_mmio_data_xfer(struct ata_queued_cmd *qc,
				unsigned char *buf, unsigned int buflen, int rw)
=======
#include <linux/regmap.h>
#include <scsi/scsi_host.h>

#define DRV_NAME	"pata_ixp4xx_cf"
#define DRV_VERSION	"1.0"

struct ixp4xx_pata {
	struct ata_host *host;
	struct regmap *rmap;
	u32 cmd_csreg;
	void __iomem *cmd;
	void __iomem *ctl;
};

#define IXP4XX_EXP_TIMING_STRIDE	0x04
/* The timings for the chipselect is in bits 29..16 */
#define IXP4XX_EXP_T1_T5_MASK	GENMASK(29, 16)
#define IXP4XX_EXP_PIO_0_8	0x0a470000
#define IXP4XX_EXP_PIO_1_8	0x06430000
#define IXP4XX_EXP_PIO_2_8	0x02410000
#define IXP4XX_EXP_PIO_3_8	0x00820000
#define IXP4XX_EXP_PIO_4_8	0x00400000
#define IXP4XX_EXP_PIO_0_16	0x29640000
#define IXP4XX_EXP_PIO_1_16	0x05030000
#define IXP4XX_EXP_PIO_2_16	0x00b20000
#define IXP4XX_EXP_PIO_3_16	0x00820000
#define IXP4XX_EXP_PIO_4_16	0x00400000
#define IXP4XX_EXP_BW_MASK	(BIT(6)|BIT(0))
#define IXP4XX_EXP_BYTE_RD16	BIT(6) /* Byte reads on half-word devices */
#define IXP4XX_EXP_BYTE_EN	BIT(0) /* Use 8bit data bus if set */

static void ixp4xx_set_8bit_timing(struct ixp4xx_pata *ixpp, u8 pio_mode)
{
	switch (pio_mode) {
	case XFER_PIO_0:
		regmap_update_bits(ixpp->rmap, ixpp->cmd_csreg,
				   IXP4XX_EXP_T1_T5_MASK, IXP4XX_EXP_PIO_0_8);
		break;
	case XFER_PIO_1:
		regmap_update_bits(ixpp->rmap, ixpp->cmd_csreg,
				   IXP4XX_EXP_T1_T5_MASK, IXP4XX_EXP_PIO_1_8);
		break;
	case XFER_PIO_2:
		regmap_update_bits(ixpp->rmap, ixpp->cmd_csreg,
				   IXP4XX_EXP_T1_T5_MASK, IXP4XX_EXP_PIO_2_8);
		break;
	case XFER_PIO_3:
		regmap_update_bits(ixpp->rmap, ixpp->cmd_csreg,
				   IXP4XX_EXP_T1_T5_MASK, IXP4XX_EXP_PIO_3_8);
		break;
	case XFER_PIO_4:
		regmap_update_bits(ixpp->rmap, ixpp->cmd_csreg,
				   IXP4XX_EXP_T1_T5_MASK, IXP4XX_EXP_PIO_4_8);
		break;
	default:
		break;
	}
	regmap_update_bits(ixpp->rmap, ixpp->cmd_csreg,
			   IXP4XX_EXP_BW_MASK, IXP4XX_EXP_BYTE_RD16|IXP4XX_EXP_BYTE_EN);
}

static void ixp4xx_set_16bit_timing(struct ixp4xx_pata *ixpp, u8 pio_mode)
{
	switch (pio_mode){
	case XFER_PIO_0:
		regmap_update_bits(ixpp->rmap, ixpp->cmd_csreg,
				   IXP4XX_EXP_T1_T5_MASK, IXP4XX_EXP_PIO_0_16);
		break;
	case XFER_PIO_1:
		regmap_update_bits(ixpp->rmap, ixpp->cmd_csreg,
				   IXP4XX_EXP_T1_T5_MASK, IXP4XX_EXP_PIO_1_16);
		break;
	case XFER_PIO_2:
		regmap_update_bits(ixpp->rmap, ixpp->cmd_csreg,
				   IXP4XX_EXP_T1_T5_MASK, IXP4XX_EXP_PIO_2_16);
		break;
	case XFER_PIO_3:
		regmap_update_bits(ixpp->rmap, ixpp->cmd_csreg,
				   IXP4XX_EXP_T1_T5_MASK, IXP4XX_EXP_PIO_3_16);
		break;
	case XFER_PIO_4:
		regmap_update_bits(ixpp->rmap, ixpp->cmd_csreg,
				   IXP4XX_EXP_T1_T5_MASK, IXP4XX_EXP_PIO_4_16);
		break;
	default:
		break;
	}
	regmap_update_bits(ixpp->rmap, ixpp->cmd_csreg,
			   IXP4XX_EXP_BW_MASK, IXP4XX_EXP_BYTE_RD16);
}

/* This sets up the timing on the chipselect CMD accordingly */
static void ixp4xx_set_piomode(struct ata_port *ap, struct ata_device *adev)
{
	struct ixp4xx_pata *ixpp = ap->host->private_data;

	ata_dev_printk(adev, KERN_INFO, "configured for PIO%d 8bit\n",
		       adev->pio_mode - XFER_PIO_0);
	ixp4xx_set_8bit_timing(ixpp, adev->pio_mode);
}


static unsigned int ixp4xx_mmio_data_xfer(struct ata_queued_cmd *qc,
					  unsigned char *buf, unsigned int buflen, int rw)
>>>>>>> upstream/android-13
{
	unsigned int i;
	unsigned int words = buflen >> 1;
	u16 *buf16 = (u16 *) buf;
<<<<<<< HEAD
	struct ata_port *ap = qc->dev->link->ap;
	void __iomem *mmio = ap->ioaddr.data_addr;
	struct ixp4xx_pata_data *data = dev_get_platdata(ap->host->dev);
=======
	struct ata_device *adev = qc->dev;
	struct ata_port *ap = qc->dev->link->ap;
	void __iomem *mmio = ap->ioaddr.data_addr;
	struct ixp4xx_pata *ixpp = ap->host->private_data;
	unsigned long flags;

	ata_dev_printk(adev, KERN_DEBUG, "%s %d bytes\n", (rw == READ) ? "READ" : "WRITE",
		       buflen);
	spin_lock_irqsave(ap->lock, flags);
>>>>>>> upstream/android-13

	/* set the expansion bus in 16bit mode and restore
	 * 8 bit mode after the transaction.
	 */
<<<<<<< HEAD
	*data->cs0_cfg &= ~(0x01);
	udelay(100);
=======
	ixp4xx_set_16bit_timing(ixpp, adev->pio_mode);
	udelay(5);
>>>>>>> upstream/android-13

	/* Transfer multiple of 2 bytes */
	if (rw == READ)
		for (i = 0; i < words; i++)
			buf16[i] = readw(mmio);
	else
		for (i = 0; i < words; i++)
			writew(buf16[i], mmio);

	/* Transfer trailing 1 byte, if any. */
	if (unlikely(buflen & 0x01)) {
		u16 align_buf[1] = { 0 };
		unsigned char *trailing_buf = buf + buflen - 1;

		if (rw == READ) {
			align_buf[0] = readw(mmio);
			memcpy(trailing_buf, align_buf, 1);
		} else {
			memcpy(align_buf, trailing_buf, 1);
			writew(align_buf[0], mmio);
		}
		words++;
	}

<<<<<<< HEAD
	udelay(100);
	*data->cs0_cfg |= 0x01;
=======
	ixp4xx_set_8bit_timing(ixpp, adev->pio_mode);
	udelay(5);

	spin_unlock_irqrestore(ap->lock, flags);
>>>>>>> upstream/android-13

	return words << 1;
}

static struct scsi_host_template ixp4xx_sht = {
	ATA_PIO_SHT(DRV_NAME),
};

static struct ata_port_operations ixp4xx_port_ops = {
	.inherits		= &ata_sff_port_ops,
	.sff_data_xfer		= ixp4xx_mmio_data_xfer,
	.cable_detect		= ata_cable_40wire,
<<<<<<< HEAD
	.set_mode		= ixp4xx_set_mode,
};

static void ixp4xx_setup_port(struct ata_port *ap,
			      struct ixp4xx_pata_data *data,
			      unsigned long raw_cs0, unsigned long raw_cs1)
{
	struct ata_ioports *ioaddr = &ap->ioaddr;
	unsigned long raw_cmd = raw_cs0;
	unsigned long raw_ctl = raw_cs1 + 0x06;

	ioaddr->cmd_addr	= data->cs0;
	ioaddr->altstatus_addr	= data->cs1 + 0x06;
	ioaddr->ctl_addr	= data->cs1 + 0x06;

	ata_sff_std_ports(ioaddr);

#ifndef __ARMEB__

	/* adjust the addresses to handle the address swizzling of the
	 * ixp4xx in little endian mode.
	 */

	*(unsigned long *)&ioaddr->data_addr		^= 0x02;
	*(unsigned long *)&ioaddr->cmd_addr		^= 0x03;
	*(unsigned long *)&ioaddr->altstatus_addr	^= 0x03;
	*(unsigned long *)&ioaddr->ctl_addr		^= 0x03;
	*(unsigned long *)&ioaddr->error_addr		^= 0x03;
	*(unsigned long *)&ioaddr->feature_addr		^= 0x03;
	*(unsigned long *)&ioaddr->nsect_addr		^= 0x03;
	*(unsigned long *)&ioaddr->lbal_addr		^= 0x03;
	*(unsigned long *)&ioaddr->lbam_addr		^= 0x03;
	*(unsigned long *)&ioaddr->lbah_addr		^= 0x03;
	*(unsigned long *)&ioaddr->device_addr		^= 0x03;
	*(unsigned long *)&ioaddr->status_addr		^= 0x03;
	*(unsigned long *)&ioaddr->command_addr		^= 0x03;

	raw_cmd ^= 0x03;
	raw_ctl ^= 0x03;
#endif
=======
	.set_piomode		= ixp4xx_set_piomode,
};

static struct ata_port_info ixp4xx_port_info = {
	.flags		= ATA_FLAG_NO_ATAPI,
	.pio_mask	= ATA_PIO4,
	.port_ops	= &ixp4xx_port_ops,
};

static void ixp4xx_setup_port(struct ata_port *ap,
			      struct ixp4xx_pata *ixpp,
			      unsigned long raw_cmd, unsigned long raw_ctl)
{
	struct ata_ioports *ioaddr = &ap->ioaddr;

	raw_ctl += 0x06;
	ioaddr->cmd_addr	= ixpp->cmd;
	ioaddr->altstatus_addr	= ixpp->ctl + 0x06;
	ioaddr->ctl_addr	= ixpp->ctl + 0x06;

	ata_sff_std_ports(ioaddr);

	if (!IS_ENABLED(CONFIG_CPU_BIG_ENDIAN)) {
		/* adjust the addresses to handle the address swizzling of the
		 * ixp4xx in little endian mode.
		 */

		*(unsigned long *)&ioaddr->data_addr		^= 0x02;
		*(unsigned long *)&ioaddr->cmd_addr		^= 0x03;
		*(unsigned long *)&ioaddr->altstatus_addr	^= 0x03;
		*(unsigned long *)&ioaddr->ctl_addr		^= 0x03;
		*(unsigned long *)&ioaddr->error_addr		^= 0x03;
		*(unsigned long *)&ioaddr->feature_addr		^= 0x03;
		*(unsigned long *)&ioaddr->nsect_addr		^= 0x03;
		*(unsigned long *)&ioaddr->lbal_addr		^= 0x03;
		*(unsigned long *)&ioaddr->lbam_addr		^= 0x03;
		*(unsigned long *)&ioaddr->lbah_addr		^= 0x03;
		*(unsigned long *)&ioaddr->device_addr		^= 0x03;
		*(unsigned long *)&ioaddr->status_addr		^= 0x03;
		*(unsigned long *)&ioaddr->command_addr		^= 0x03;

		raw_cmd ^= 0x03;
		raw_ctl ^= 0x03;
	}
>>>>>>> upstream/android-13

	ata_port_desc(ap, "cmd 0x%lx ctl 0x%lx", raw_cmd, raw_ctl);
}

static int ixp4xx_pata_probe(struct platform_device *pdev)
{
<<<<<<< HEAD
	unsigned int irq;
	struct resource *cs0, *cs1;
	struct ata_host *host;
	struct ata_port *ap;
	struct ixp4xx_pata_data *data = dev_get_platdata(&pdev->dev);
	int ret;

	cs0 = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	cs1 = platform_get_resource(pdev, IORESOURCE_MEM, 1);

	if (!cs0 || !cs1)
		return -EINVAL;

	/* allocate host */
	host = ata_host_alloc(&pdev->dev, 1);
	if (!host)
		return -ENOMEM;

	/* acquire resources and fill host */
	ret = dma_set_coherent_mask(&pdev->dev, DMA_BIT_MASK(32));
	if (ret)
		return ret;

	data->cs0 = devm_ioremap(&pdev->dev, cs0->start, 0x1000);
	data->cs1 = devm_ioremap(&pdev->dev, cs1->start, 0x1000);

	if (!data->cs0 || !data->cs1)
=======
	struct resource *cmd, *ctl;
	struct ata_port_info pi = ixp4xx_port_info;
	const struct ata_port_info *ppi[] = { &pi, NULL };
	struct device *dev = &pdev->dev;
	struct device_node *np = dev->of_node;
	struct ixp4xx_pata *ixpp;
	u32 csindex;
	int ret;
	int irq;

	cmd = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	ctl = platform_get_resource(pdev, IORESOURCE_MEM, 1);

	if (!cmd || !ctl)
		return -EINVAL;

	ixpp = devm_kzalloc(dev, sizeof(*ixpp), GFP_KERNEL);
	if (!ixpp)
		return -ENOMEM;

	ixpp->rmap = syscon_node_to_regmap(np->parent);
	if (IS_ERR(ixpp->rmap))
		return dev_err_probe(dev, PTR_ERR(ixpp->rmap), "no regmap\n");
	/* Inspect our address to figure out what chipselect the CMD is on */
	ret = of_property_read_u32_index(np, "reg", 0, &csindex);
	if (ret)
		return dev_err_probe(dev, ret, "can't inspect CMD address\n");
	dev_info(dev, "using CS%d for PIO timing configuration\n", csindex);
	ixpp->cmd_csreg = csindex * IXP4XX_EXP_TIMING_STRIDE;

	ixpp->host = ata_host_alloc_pinfo(dev, ppi, 1);
	if (!ixpp->host)
		return -ENOMEM;
	ixpp->host->private_data = ixpp;

	ret = dma_set_coherent_mask(dev, DMA_BIT_MASK(32));
	if (ret)
		return ret;

	ixpp->cmd = devm_ioremap_resource(dev, cmd);
	ixpp->ctl = devm_ioremap_resource(dev, ctl);
	if (IS_ERR(ixpp->cmd) || IS_ERR(ixpp->ctl))
>>>>>>> upstream/android-13
		return -ENOMEM;

	irq = platform_get_irq(pdev, 0);
	if (irq > 0)
		irq_set_irq_type(irq, IRQ_TYPE_EDGE_RISING);
	else if (irq < 0)
		return irq;
	else
		return -EINVAL;

<<<<<<< HEAD
	/* Setup expansion bus chip selects */
	*data->cs0_cfg = data->cs0_bits;
	*data->cs1_cfg = data->cs1_bits;

	ap = host->ports[0];

	ap->ops	= &ixp4xx_port_ops;
	ap->pio_mask = ATA_PIO4;
	ap->flags |= ATA_FLAG_NO_ATAPI;

	ixp4xx_setup_port(ap, data, cs0->start, cs1->start);

	ata_print_version_once(&pdev->dev, DRV_VERSION);

	/* activate host */
	return ata_host_activate(host, irq, ata_sff_interrupt, 0, &ixp4xx_sht);
}

static struct platform_driver ixp4xx_pata_platform_driver = {
	.driver	 = {
		.name   = DRV_NAME,
=======
	/* Just one port to set up */
	ixp4xx_setup_port(ixpp->host->ports[0], ixpp, cmd->start, ctl->start);

	ata_print_version_once(dev, DRV_VERSION);

	return ata_host_activate(ixpp->host, irq, ata_sff_interrupt, 0, &ixp4xx_sht);
}

static const struct of_device_id ixp4xx_pata_of_match[] = {
	{ .compatible = "intel,ixp4xx-compact-flash", },
	{ },
};

static struct platform_driver ixp4xx_pata_platform_driver = {
	.driver	 = {
		.name   = DRV_NAME,
		.of_match_table = ixp4xx_pata_of_match,
>>>>>>> upstream/android-13
	},
	.probe		= ixp4xx_pata_probe,
	.remove		= ata_platform_remove_one,
};

module_platform_driver(ixp4xx_pata_platform_driver);

MODULE_AUTHOR("Alessandro Zummo <a.zummo@towertech.it>");
MODULE_DESCRIPTION("low-level driver for ixp4xx Compact Flash PATA");
MODULE_LICENSE("GPL");
MODULE_VERSION(DRV_VERSION);
MODULE_ALIAS("platform:" DRV_NAME);
