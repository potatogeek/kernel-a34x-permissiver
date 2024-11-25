<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0
>>>>>>> upstream/android-13
/*
 *  Copyright (C) 2006 Jonathan McDowell <noodles@earth.li>
 *
 *  Derived from drivers/mtd/nand/toto.c (removed in v2.6.28)
 *    Copyright (c) 2003 Texas Instruments
 *    Copyright (c) 2002 Thomas Gleixner <tgxl@linutronix.de>
 *
 *  Converted to platform driver by Janusz Krzysztofik <jkrzyszt@tis.icnet.pl>
 *  Partially stolen from plat_nand.c
 *
<<<<<<< HEAD
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
=======
>>>>>>> upstream/android-13
 *  Overview:
 *   This is a device driver for the NAND flash device found on the
 *   Amstrad E3 (Delta).
 */

#include <linux/slab.h>
#include <linux/module.h>
#include <linux/delay.h>
<<<<<<< HEAD
#include <linux/mtd/mtd.h>
#include <linux/mtd/rawnand.h>
#include <linux/mtd/partitions.h>
#include <linux/gpio.h>
#include <linux/platform_data/gpio-omap.h>

#include <asm/io.h>
#include <asm/sizes.h>

#include <mach/board-ams-delta.h>

#include <mach/hardware.h>
=======
#include <linux/gpio/consumer.h>
#include <linux/mtd/mtd.h>
#include <linux/mtd/nand-gpio.h>
#include <linux/mtd/rawnand.h>
#include <linux/mtd/partitions.h>
#include <linux/of_device.h>
#include <linux/platform_device.h>
#include <linux/sizes.h>
>>>>>>> upstream/android-13

/*
 * MTD structure for E3 (Delta)
 */
<<<<<<< HEAD
static struct mtd_info *ams_delta_mtd = NULL;

/*
 * Define partitions for flash devices
 */

static const struct mtd_partition partition_info[] = {
	{ .name		= "Kernel",
	  .offset	= 0,
	  .size		= 3 * SZ_1M + SZ_512K },
	{ .name		= "u-boot",
	  .offset	= 3 * SZ_1M + SZ_512K,
	  .size		= SZ_256K },
	{ .name		= "u-boot params",
	  .offset	= 3 * SZ_1M + SZ_512K + SZ_256K,
	  .size		= SZ_256K },
	{ .name		= "Amstrad LDR",
	  .offset	= 4 * SZ_1M,
	  .size		= SZ_256K },
	{ .name		= "File system",
	  .offset	= 4 * SZ_1M + 1 * SZ_256K,
	  .size		= 27 * SZ_1M },
	{ .name		= "PBL reserved",
	  .offset	= 32 * SZ_1M - 3 * SZ_256K,
	  .size		=  3 * SZ_256K },
};

static void ams_delta_write_byte(struct mtd_info *mtd, u_char byte)
{
	struct nand_chip *this = mtd_to_nand(mtd);
	void __iomem *io_base = (void __iomem *)nand_get_controller_data(this);

	writew(0, io_base + OMAP_MPUIO_IO_CNTL);
	writew(byte, this->IO_ADDR_W);
	gpio_set_value(AMS_DELTA_GPIO_PIN_NAND_NWE, 0);
	ndelay(40);
	gpio_set_value(AMS_DELTA_GPIO_PIN_NAND_NWE, 1);
}

static u_char ams_delta_read_byte(struct mtd_info *mtd)
{
	u_char res;
	struct nand_chip *this = mtd_to_nand(mtd);
	void __iomem *io_base = (void __iomem *)nand_get_controller_data(this);

	gpio_set_value(AMS_DELTA_GPIO_PIN_NAND_NRE, 0);
	ndelay(40);
	writew(~0, io_base + OMAP_MPUIO_IO_CNTL);
	res = readw(this->IO_ADDR_R);
	gpio_set_value(AMS_DELTA_GPIO_PIN_NAND_NRE, 1);

	return res;
}

static void ams_delta_write_buf(struct mtd_info *mtd, const u_char *buf,
				int len)
{
	int i;

	for (i=0; i<len; i++)
		ams_delta_write_byte(mtd, buf[i]);
}

static void ams_delta_read_buf(struct mtd_info *mtd, u_char *buf, int len)
{
	int i;

	for (i=0; i<len; i++)
		buf[i] = ams_delta_read_byte(mtd);
}

/*
 * Command control function
 *
 * ctrl:
 * NAND_NCE: bit 0 -> bit 2
 * NAND_CLE: bit 1 -> bit 7
 * NAND_ALE: bit 2 -> bit 6
 */
static void ams_delta_hwcontrol(struct mtd_info *mtd, int cmd,
				unsigned int ctrl)
{

	if (ctrl & NAND_CTRL_CHANGE) {
		gpio_set_value(AMS_DELTA_GPIO_PIN_NAND_NCE,
				(ctrl & NAND_NCE) == 0);
		gpio_set_value(AMS_DELTA_GPIO_PIN_NAND_CLE,
				(ctrl & NAND_CLE) != 0);
		gpio_set_value(AMS_DELTA_GPIO_PIN_NAND_ALE,
				(ctrl & NAND_ALE) != 0);
	}

	if (cmd != NAND_CMD_NONE)
		ams_delta_write_byte(mtd, cmd);
}

static int ams_delta_nand_ready(struct mtd_info *mtd)
{
	return gpio_get_value(AMS_DELTA_GPIO_PIN_NAND_RB);
}

static const struct gpio _mandatory_gpio[] = {
	{
		.gpio	= AMS_DELTA_GPIO_PIN_NAND_NCE,
		.flags	= GPIOF_OUT_INIT_HIGH,
		.label	= "nand_nce",
	},
	{
		.gpio	= AMS_DELTA_GPIO_PIN_NAND_NRE,
		.flags	= GPIOF_OUT_INIT_HIGH,
		.label	= "nand_nre",
	},
	{
		.gpio	= AMS_DELTA_GPIO_PIN_NAND_NWP,
		.flags	= GPIOF_OUT_INIT_HIGH,
		.label	= "nand_nwp",
	},
	{
		.gpio	= AMS_DELTA_GPIO_PIN_NAND_NWE,
		.flags	= GPIOF_OUT_INIT_HIGH,
		.label	= "nand_nwe",
	},
	{
		.gpio	= AMS_DELTA_GPIO_PIN_NAND_ALE,
		.flags	= GPIOF_OUT_INIT_LOW,
		.label	= "nand_ale",
	},
	{
		.gpio	= AMS_DELTA_GPIO_PIN_NAND_CLE,
		.flags	= GPIOF_OUT_INIT_LOW,
		.label	= "nand_cle",
	},
=======
struct gpio_nand {
	struct nand_controller	base;
	struct nand_chip	nand_chip;
	struct gpio_desc	*gpiod_rdy;
	struct gpio_desc	*gpiod_nce;
	struct gpio_desc	*gpiod_nre;
	struct gpio_desc	*gpiod_nwp;
	struct gpio_desc	*gpiod_nwe;
	struct gpio_desc	*gpiod_ale;
	struct gpio_desc	*gpiod_cle;
	struct gpio_descs	*data_gpiods;
	bool			data_in;
	unsigned int		tRP;
	unsigned int		tWP;
	u8			(*io_read)(struct gpio_nand *this);
	void			(*io_write)(struct gpio_nand *this, u8 byte);
};

static void gpio_nand_write_commit(struct gpio_nand *priv)
{
	gpiod_set_value(priv->gpiod_nwe, 1);
	ndelay(priv->tWP);
	gpiod_set_value(priv->gpiod_nwe, 0);
}

static void gpio_nand_io_write(struct gpio_nand *priv, u8 byte)
{
	struct gpio_descs *data_gpiods = priv->data_gpiods;
	DECLARE_BITMAP(values, BITS_PER_TYPE(byte)) = { byte, };

	gpiod_set_raw_array_value(data_gpiods->ndescs, data_gpiods->desc,
				  data_gpiods->info, values);

	gpio_nand_write_commit(priv);
}

static void gpio_nand_dir_output(struct gpio_nand *priv, u8 byte)
{
	struct gpio_descs *data_gpiods = priv->data_gpiods;
	DECLARE_BITMAP(values, BITS_PER_TYPE(byte)) = { byte, };
	int i;

	for (i = 0; i < data_gpiods->ndescs; i++)
		gpiod_direction_output_raw(data_gpiods->desc[i],
					   test_bit(i, values));

	gpio_nand_write_commit(priv);

	priv->data_in = false;
}

static u8 gpio_nand_io_read(struct gpio_nand *priv)
{
	u8 res;
	struct gpio_descs *data_gpiods = priv->data_gpiods;
	DECLARE_BITMAP(values, BITS_PER_TYPE(res)) = { 0, };

	gpiod_set_value(priv->gpiod_nre, 1);
	ndelay(priv->tRP);

	gpiod_get_raw_array_value(data_gpiods->ndescs, data_gpiods->desc,
				  data_gpiods->info, values);

	gpiod_set_value(priv->gpiod_nre, 0);

	res = values[0];
	return res;
}

static void gpio_nand_dir_input(struct gpio_nand *priv)
{
	struct gpio_descs *data_gpiods = priv->data_gpiods;
	int i;

	for (i = 0; i < data_gpiods->ndescs; i++)
		gpiod_direction_input(data_gpiods->desc[i]);

	priv->data_in = true;
}

static void gpio_nand_write_buf(struct gpio_nand *priv, const u8 *buf, int len)
{
	int i = 0;

	if (len > 0 && priv->data_in)
		gpio_nand_dir_output(priv, buf[i++]);

	while (i < len)
		priv->io_write(priv, buf[i++]);
}

static void gpio_nand_read_buf(struct gpio_nand *priv, u8 *buf, int len)
{
	int i;

	if (priv->data_gpiods && !priv->data_in)
		gpio_nand_dir_input(priv);

	for (i = 0; i < len; i++)
		buf[i] = priv->io_read(priv);
}

static void gpio_nand_ctrl_cs(struct gpio_nand *priv, bool assert)
{
	gpiod_set_value(priv->gpiod_nce, assert);
}

static int gpio_nand_exec_op(struct nand_chip *this,
			     const struct nand_operation *op, bool check_only)
{
	struct gpio_nand *priv = nand_get_controller_data(this);
	const struct nand_op_instr *instr;
	int ret = 0;

	if (check_only)
		return 0;

	gpio_nand_ctrl_cs(priv, 1);

	for (instr = op->instrs; instr < op->instrs + op->ninstrs; instr++) {
		switch (instr->type) {
		case NAND_OP_CMD_INSTR:
			gpiod_set_value(priv->gpiod_cle, 1);
			gpio_nand_write_buf(priv, &instr->ctx.cmd.opcode, 1);
			gpiod_set_value(priv->gpiod_cle, 0);
			break;

		case NAND_OP_ADDR_INSTR:
			gpiod_set_value(priv->gpiod_ale, 1);
			gpio_nand_write_buf(priv, instr->ctx.addr.addrs,
					    instr->ctx.addr.naddrs);
			gpiod_set_value(priv->gpiod_ale, 0);
			break;

		case NAND_OP_DATA_IN_INSTR:
			gpio_nand_read_buf(priv, instr->ctx.data.buf.in,
					   instr->ctx.data.len);
			break;

		case NAND_OP_DATA_OUT_INSTR:
			gpio_nand_write_buf(priv, instr->ctx.data.buf.out,
					    instr->ctx.data.len);
			break;

		case NAND_OP_WAITRDY_INSTR:
			ret = priv->gpiod_rdy ?
			      nand_gpio_waitrdy(this, priv->gpiod_rdy,
						instr->ctx.waitrdy.timeout_ms) :
			      nand_soft_waitrdy(this,
						instr->ctx.waitrdy.timeout_ms);
			break;
		}

		if (ret)
			break;
	}

	gpio_nand_ctrl_cs(priv, 0);

	return ret;
}

static int gpio_nand_setup_interface(struct nand_chip *this, int csline,
				     const struct nand_interface_config *cf)
{
	struct gpio_nand *priv = nand_get_controller_data(this);
	const struct nand_sdr_timings *sdr = nand_get_sdr_timings(cf);
	struct device *dev = &nand_to_mtd(this)->dev;

	if (IS_ERR(sdr))
		return PTR_ERR(sdr);

	if (csline == NAND_DATA_IFACE_CHECK_ONLY)
		return 0;

	if (priv->gpiod_nre) {
		priv->tRP = DIV_ROUND_UP(sdr->tRP_min, 1000);
		dev_dbg(dev, "using %u ns read pulse width\n", priv->tRP);
	}

	priv->tWP = DIV_ROUND_UP(sdr->tWP_min, 1000);
	dev_dbg(dev, "using %u ns write pulse width\n", priv->tWP);

	return 0;
}

static int gpio_nand_attach_chip(struct nand_chip *chip)
{
	if (chip->ecc.engine_type == NAND_ECC_ENGINE_TYPE_SOFT &&
	    chip->ecc.algo == NAND_ECC_ALGO_UNKNOWN)
		chip->ecc.algo = NAND_ECC_ALGO_HAMMING;

	return 0;
}

static const struct nand_controller_ops gpio_nand_ops = {
	.exec_op = gpio_nand_exec_op,
	.attach_chip = gpio_nand_attach_chip,
	.setup_interface = gpio_nand_setup_interface,
>>>>>>> upstream/android-13
};

/*
 * Main initialization routine
 */
<<<<<<< HEAD
static int ams_delta_init(struct platform_device *pdev)
{
	struct nand_chip *this;
	struct resource *res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	void __iomem *io_base;
	int err = 0;

	if (!res)
		return -ENXIO;

	/* Allocate memory for MTD device structure and private data */
	this = kzalloc(sizeof(struct nand_chip), GFP_KERNEL);
	if (!this) {
		pr_warn("Unable to allocate E3 NAND MTD device structure.\n");
		err = -ENOMEM;
		goto out;
	}

	ams_delta_mtd = nand_to_mtd(this);
	ams_delta_mtd->owner = THIS_MODULE;

	/*
	 * Don't try to request the memory region from here,
	 * it should have been already requested from the
	 * gpio-omap driver and requesting it again would fail.
	 */

	io_base = ioremap(res->start, resource_size(res));
	if (io_base == NULL) {
		dev_err(&pdev->dev, "ioremap failed\n");
		err = -EIO;
		goto out_free;
	}

	nand_set_controller_data(this, (void *)io_base);

	/* Set address of NAND IO lines */
	this->IO_ADDR_R = io_base + OMAP_MPUIO_INPUT_LATCH;
	this->IO_ADDR_W = io_base + OMAP_MPUIO_OUTPUT;
	this->read_byte = ams_delta_read_byte;
	this->write_buf = ams_delta_write_buf;
	this->read_buf = ams_delta_read_buf;
	this->cmd_ctrl = ams_delta_hwcontrol;
	if (gpio_request(AMS_DELTA_GPIO_PIN_NAND_RB, "nand_rdy") == 0) {
		this->dev_ready = ams_delta_nand_ready;
	} else {
		this->dev_ready = NULL;
		pr_notice("Couldn't request gpio for Delta NAND ready.\n");
	}
	/* 25 us command delay time */
	this->chip_delay = 30;
	this->ecc.mode = NAND_ECC_SOFT;
	this->ecc.algo = NAND_ECC_HAMMING;

	platform_set_drvdata(pdev, io_base);

	/* Set chip enabled, but  */
	err = gpio_request_array(_mandatory_gpio, ARRAY_SIZE(_mandatory_gpio));
	if (err)
		goto out_gpio;
=======
static int gpio_nand_probe(struct platform_device *pdev)
{
	struct gpio_nand_platdata *pdata = dev_get_platdata(&pdev->dev);
	const struct mtd_partition *partitions = NULL;
	int num_partitions = 0;
	struct gpio_nand *priv;
	struct nand_chip *this;
	struct mtd_info *mtd;
	int (*probe)(struct platform_device *pdev, struct gpio_nand *priv);
	int err = 0;

	if (pdata) {
		partitions = pdata->parts;
		num_partitions = pdata->num_parts;
	}

	/* Allocate memory for MTD device structure and private data */
	priv = devm_kzalloc(&pdev->dev, sizeof(struct gpio_nand),
			    GFP_KERNEL);
	if (!priv)
		return -ENOMEM;

	this = &priv->nand_chip;

	mtd = nand_to_mtd(this);
	mtd->dev.parent = &pdev->dev;

	nand_set_controller_data(this, priv);
	nand_set_flash_node(this, pdev->dev.of_node);

	priv->gpiod_rdy = devm_gpiod_get_optional(&pdev->dev, "rdy", GPIOD_IN);
	if (IS_ERR(priv->gpiod_rdy)) {
		err = PTR_ERR(priv->gpiod_rdy);
		dev_warn(&pdev->dev, "RDY GPIO request failed (%d)\n", err);
		return err;
	}

	platform_set_drvdata(pdev, priv);

	/* Set chip enabled but write protected */
	priv->gpiod_nwp = devm_gpiod_get_optional(&pdev->dev, "nwp",
						  GPIOD_OUT_HIGH);
	if (IS_ERR(priv->gpiod_nwp)) {
		err = PTR_ERR(priv->gpiod_nwp);
		dev_err(&pdev->dev, "NWP GPIO request failed (%d)\n", err);
		return err;
	}

	priv->gpiod_nce = devm_gpiod_get_optional(&pdev->dev, "nce",
						  GPIOD_OUT_LOW);
	if (IS_ERR(priv->gpiod_nce)) {
		err = PTR_ERR(priv->gpiod_nce);
		dev_err(&pdev->dev, "NCE GPIO request failed (%d)\n", err);
		return err;
	}

	priv->gpiod_nre = devm_gpiod_get_optional(&pdev->dev, "nre",
						  GPIOD_OUT_LOW);
	if (IS_ERR(priv->gpiod_nre)) {
		err = PTR_ERR(priv->gpiod_nre);
		dev_err(&pdev->dev, "NRE GPIO request failed (%d)\n", err);
		return err;
	}

	priv->gpiod_nwe = devm_gpiod_get_optional(&pdev->dev, "nwe",
						  GPIOD_OUT_LOW);
	if (IS_ERR(priv->gpiod_nwe)) {
		err = PTR_ERR(priv->gpiod_nwe);
		dev_err(&pdev->dev, "NWE GPIO request failed (%d)\n", err);
		return err;
	}

	priv->gpiod_ale = devm_gpiod_get(&pdev->dev, "ale", GPIOD_OUT_LOW);
	if (IS_ERR(priv->gpiod_ale)) {
		err = PTR_ERR(priv->gpiod_ale);
		dev_err(&pdev->dev, "ALE GPIO request failed (%d)\n", err);
		return err;
	}

	priv->gpiod_cle = devm_gpiod_get(&pdev->dev, "cle", GPIOD_OUT_LOW);
	if (IS_ERR(priv->gpiod_cle)) {
		err = PTR_ERR(priv->gpiod_cle);
		dev_err(&pdev->dev, "CLE GPIO request failed (%d)\n", err);
		return err;
	}

	/* Request array of data pins, initialize them as input */
	priv->data_gpiods = devm_gpiod_get_array_optional(&pdev->dev, "data",
							  GPIOD_IN);
	if (IS_ERR(priv->data_gpiods)) {
		err = PTR_ERR(priv->data_gpiods);
		dev_err(&pdev->dev, "data GPIO request failed: %d\n", err);
		return err;
	}
	if (priv->data_gpiods) {
		if (!priv->gpiod_nwe) {
			dev_err(&pdev->dev,
				"mandatory NWE pin not provided by platform\n");
			return -ENODEV;
		}

		priv->io_read = gpio_nand_io_read;
		priv->io_write = gpio_nand_io_write;
		priv->data_in = true;
	}

	if (pdev->id_entry)
		probe = (void *) pdev->id_entry->driver_data;
	else
		probe = of_device_get_match_data(&pdev->dev);
	if (probe)
		err = probe(pdev, priv);
	if (err)
		return err;

	if (!priv->io_read || !priv->io_write) {
		dev_err(&pdev->dev, "incomplete device configuration\n");
		return -ENODEV;
	}

	/* Initialize the NAND controller object embedded in gpio_nand. */
	priv->base.ops = &gpio_nand_ops;
	nand_controller_init(&priv->base);
	this->controller = &priv->base;

	/*
	 * FIXME: We should release write protection only after nand_scan() to
	 * be on the safe side but we can't do that until we have a generic way
	 * to assert/deassert WP from the core.  Even if the core shouldn't
	 * write things in the nand_scan() path, it should have control on this
	 * pin just in case we ever need to disable write protection during
	 * chip detection/initialization.
	 */
	/* Release write protection */
	gpiod_set_value(priv->gpiod_nwp, 0);

	/*
	 * This driver assumes that the default ECC engine should be TYPE_SOFT.
	 * Set ->engine_type before registering the NAND devices in order to
	 * provide a driver specific default value.
	 */
	this->ecc.engine_type = NAND_ECC_ENGINE_TYPE_SOFT;
>>>>>>> upstream/android-13

	/* Scan to find existence of the device */
	err = nand_scan(this, 1);
	if (err)
<<<<<<< HEAD
		goto out_mtd;

	/* Register the partitions */
	mtd_device_register(ams_delta_mtd, partition_info,
			    ARRAY_SIZE(partition_info));

	goto out;

 out_mtd:
	gpio_free_array(_mandatory_gpio, ARRAY_SIZE(_mandatory_gpio));
out_gpio:
	gpio_free(AMS_DELTA_GPIO_PIN_NAND_RB);
	iounmap(io_base);
out_free:
	kfree(this);
 out:
=======
		return err;

	/* Register the partitions */
	err = mtd_device_register(mtd, partitions, num_partitions);
	if (err)
		goto err_nand_cleanup;

	return 0;

err_nand_cleanup:
	nand_cleanup(this);

>>>>>>> upstream/android-13
	return err;
}

/*
 * Clean up routine
 */
<<<<<<< HEAD
static int ams_delta_cleanup(struct platform_device *pdev)
{
	void __iomem *io_base = platform_get_drvdata(pdev);

	/* Release resources, unregister device */
	nand_release(mtd_to_nand(ams_delta_mtd));

	gpio_free_array(_mandatory_gpio, ARRAY_SIZE(_mandatory_gpio));
	gpio_free(AMS_DELTA_GPIO_PIN_NAND_RB);
	iounmap(io_base);

	/* Free the MTD device structure */
	kfree(mtd_to_nand(ams_delta_mtd));
=======
static int gpio_nand_remove(struct platform_device *pdev)
{
	struct gpio_nand *priv = platform_get_drvdata(pdev);
	struct mtd_info *mtd = nand_to_mtd(&priv->nand_chip);
	int ret;

	/* Apply write protection */
	gpiod_set_value(priv->gpiod_nwp, 1);

	/* Unregister device */
	ret = mtd_device_unregister(mtd);
	WARN_ON(ret);
	nand_cleanup(mtd_to_nand(mtd));
>>>>>>> upstream/android-13

	return 0;
}

<<<<<<< HEAD
static struct platform_driver ams_delta_nand_driver = {
	.probe		= ams_delta_init,
	.remove		= ams_delta_cleanup,
	.driver		= {
		.name	= "ams-delta-nand",
	},
};

module_platform_driver(ams_delta_nand_driver);

MODULE_LICENSE("GPL");
=======
#ifdef CONFIG_OF
static const struct of_device_id gpio_nand_of_id_table[] = {
	{
		/* sentinel */
	},
};
MODULE_DEVICE_TABLE(of, gpio_nand_of_id_table);
#endif

static const struct platform_device_id gpio_nand_plat_id_table[] = {
	{
		.name	= "ams-delta-nand",
	}, {
		/* sentinel */
	},
};
MODULE_DEVICE_TABLE(platform, gpio_nand_plat_id_table);

static struct platform_driver gpio_nand_driver = {
	.probe		= gpio_nand_probe,
	.remove		= gpio_nand_remove,
	.id_table	= gpio_nand_plat_id_table,
	.driver		= {
		.name	= "ams-delta-nand",
		.of_match_table = of_match_ptr(gpio_nand_of_id_table),
	},
};

module_platform_driver(gpio_nand_driver);

MODULE_LICENSE("GPL v2");
>>>>>>> upstream/android-13
MODULE_AUTHOR("Jonathan McDowell <noodles@earth.li>");
MODULE_DESCRIPTION("Glue layer for NAND flash on Amstrad E3 (Delta)");
