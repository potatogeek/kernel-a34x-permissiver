<<<<<<< HEAD
/*
 *  Copyright © 2008 Ilya Yanok, Emcraft Systems
 *
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
=======
// SPDX-License-Identifier: GPL-2.0-only
/*
 *  Copyright © 2008 Ilya Yanok, Emcraft Systems
>>>>>>> upstream/android-13
 */

#include <linux/slab.h>
#include <linux/module.h>
#include <linux/mtd/mtd.h>
#include <linux/mtd/rawnand.h>
#include <linux/mtd/partitions.h>
#include <linux/of_address.h>
#include <linux/of_platform.h>
#include <linux/io.h>

#define FPGA_NAND_CMD_MASK		(0x7 << 28)
#define FPGA_NAND_CMD_COMMAND		(0x0 << 28)
#define FPGA_NAND_CMD_ADDR		(0x1 << 28)
#define FPGA_NAND_CMD_READ		(0x2 << 28)
#define FPGA_NAND_CMD_WRITE		(0x3 << 28)
#define FPGA_NAND_BUSY			(0x1 << 15)
#define FPGA_NAND_ENABLE		(0x1 << 31)
#define FPGA_NAND_DATA_SHIFT		16

struct socrates_nand_host {
<<<<<<< HEAD
=======
	struct nand_controller	controller;
>>>>>>> upstream/android-13
	struct nand_chip	nand_chip;
	void __iomem		*io_base;
	struct device		*dev;
};

/**
 * socrates_nand_write_buf -  write buffer to chip
<<<<<<< HEAD
 * @mtd:	MTD device structure
 * @buf:	data buffer
 * @len:	number of bytes to write
 */
static void socrates_nand_write_buf(struct mtd_info *mtd,
		const uint8_t *buf, int len)
{
	int i;
	struct nand_chip *this = mtd_to_nand(mtd);
=======
 * @this:	NAND chip object
 * @buf:	data buffer
 * @len:	number of bytes to write
 */
static void socrates_nand_write_buf(struct nand_chip *this, const uint8_t *buf,
				    int len)
{
	int i;
>>>>>>> upstream/android-13
	struct socrates_nand_host *host = nand_get_controller_data(this);

	for (i = 0; i < len; i++) {
		out_be32(host->io_base, FPGA_NAND_ENABLE |
				FPGA_NAND_CMD_WRITE |
				(buf[i] << FPGA_NAND_DATA_SHIFT));
	}
}

/**
 * socrates_nand_read_buf -  read chip data into buffer
<<<<<<< HEAD
 * @mtd:	MTD device structure
 * @buf:	buffer to store date
 * @len:	number of bytes to read
 */
static void socrates_nand_read_buf(struct mtd_info *mtd, uint8_t *buf, int len)
{
	int i;
	struct nand_chip *this = mtd_to_nand(mtd);
=======
 * @this:	NAND chip object
 * @buf:	buffer to store date
 * @len:	number of bytes to read
 */
static void socrates_nand_read_buf(struct nand_chip *this, uint8_t *buf,
				   int len)
{
	int i;
>>>>>>> upstream/android-13
	struct socrates_nand_host *host = nand_get_controller_data(this);
	uint32_t val;

	val = FPGA_NAND_ENABLE | FPGA_NAND_CMD_READ;

	out_be32(host->io_base, val);
	for (i = 0; i < len; i++) {
		buf[i] = (in_be32(host->io_base) >>
				FPGA_NAND_DATA_SHIFT) & 0xff;
	}
}

/**
 * socrates_nand_read_byte -  read one byte from the chip
 * @mtd:	MTD device structure
 */
<<<<<<< HEAD
static uint8_t socrates_nand_read_byte(struct mtd_info *mtd)
{
	uint8_t byte;
	socrates_nand_read_buf(mtd, &byte, sizeof(byte));
	return byte;
}

/**
 * socrates_nand_read_word -  read one word from the chip
 * @mtd:	MTD device structure
 */
static uint16_t socrates_nand_read_word(struct mtd_info *mtd)
{
	uint16_t word;
	socrates_nand_read_buf(mtd, (uint8_t *)&word, sizeof(word));
	return word;
}

/*
 * Hardware specific access to control-lines
 */
static void socrates_nand_cmd_ctrl(struct mtd_info *mtd, int cmd,
		unsigned int ctrl)
{
	struct nand_chip *nand_chip = mtd_to_nand(mtd);
=======
static uint8_t socrates_nand_read_byte(struct nand_chip *this)
{
	uint8_t byte;
	socrates_nand_read_buf(this, &byte, sizeof(byte));
	return byte;
}

/*
 * Hardware specific access to control-lines
 */
static void socrates_nand_cmd_ctrl(struct nand_chip *nand_chip, int cmd,
				   unsigned int ctrl)
{
>>>>>>> upstream/android-13
	struct socrates_nand_host *host = nand_get_controller_data(nand_chip);
	uint32_t val;

	if (cmd == NAND_CMD_NONE)
		return;

	if (ctrl & NAND_CLE)
		val = FPGA_NAND_CMD_COMMAND;
	else
		val = FPGA_NAND_CMD_ADDR;

	if (ctrl & NAND_NCE)
		val |= FPGA_NAND_ENABLE;

	val |= (cmd & 0xff) << FPGA_NAND_DATA_SHIFT;

	out_be32(host->io_base, val);
}

/*
 * Read the Device Ready pin.
 */
<<<<<<< HEAD
static int socrates_nand_device_ready(struct mtd_info *mtd)
{
	struct nand_chip *nand_chip = mtd_to_nand(mtd);
=======
static int socrates_nand_device_ready(struct nand_chip *nand_chip)
{
>>>>>>> upstream/android-13
	struct socrates_nand_host *host = nand_get_controller_data(nand_chip);

	if (in_be32(host->io_base) & FPGA_NAND_BUSY)
		return 0; /* busy */
	return 1;
}

<<<<<<< HEAD
=======
static int socrates_attach_chip(struct nand_chip *chip)
{
	if (chip->ecc.engine_type == NAND_ECC_ENGINE_TYPE_SOFT &&
	    chip->ecc.algo == NAND_ECC_ALGO_UNKNOWN)
		chip->ecc.algo = NAND_ECC_ALGO_HAMMING;

	return 0;
}

static const struct nand_controller_ops socrates_ops = {
	.attach_chip = socrates_attach_chip,
};

>>>>>>> upstream/android-13
/*
 * Probe for the NAND device.
 */
static int socrates_nand_probe(struct platform_device *ofdev)
{
	struct socrates_nand_host *host;
	struct mtd_info *mtd;
	struct nand_chip *nand_chip;
	int res;

	/* Allocate memory for the device structure (and zero it) */
	host = devm_kzalloc(&ofdev->dev, sizeof(*host), GFP_KERNEL);
	if (!host)
		return -ENOMEM;

	host->io_base = of_iomap(ofdev->dev.of_node, 0);
	if (host->io_base == NULL) {
		dev_err(&ofdev->dev, "ioremap failed\n");
		return -EIO;
	}

	nand_chip = &host->nand_chip;
	mtd = nand_to_mtd(nand_chip);
	host->dev = &ofdev->dev;

<<<<<<< HEAD
=======
	nand_controller_init(&host->controller);
	host->controller.ops = &socrates_ops;
	nand_chip->controller = &host->controller;

>>>>>>> upstream/android-13
	/* link the private data structures */
	nand_set_controller_data(nand_chip, host);
	nand_set_flash_node(nand_chip, ofdev->dev.of_node);
	mtd->name = "socrates_nand";
	mtd->dev.parent = &ofdev->dev;

<<<<<<< HEAD
	/*should never be accessed directly */
	nand_chip->IO_ADDR_R = (void *)0xdeadbeef;
	nand_chip->IO_ADDR_W = (void *)0xdeadbeef;

	nand_chip->cmd_ctrl = socrates_nand_cmd_ctrl;
	nand_chip->read_byte = socrates_nand_read_byte;
	nand_chip->read_word = socrates_nand_read_word;
	nand_chip->write_buf = socrates_nand_write_buf;
	nand_chip->read_buf = socrates_nand_read_buf;
	nand_chip->dev_ready = socrates_nand_device_ready;

	nand_chip->ecc.mode = NAND_ECC_SOFT;	/* enable ECC */
	nand_chip->ecc.algo = NAND_ECC_HAMMING;

	/* TODO: I have no idea what real delay is. */
	nand_chip->chip_delay = 20;		/* 20us command delay time */
=======
	nand_chip->legacy.cmd_ctrl = socrates_nand_cmd_ctrl;
	nand_chip->legacy.read_byte = socrates_nand_read_byte;
	nand_chip->legacy.write_buf = socrates_nand_write_buf;
	nand_chip->legacy.read_buf = socrates_nand_read_buf;
	nand_chip->legacy.dev_ready = socrates_nand_device_ready;

	/* TODO: I have no idea what real delay is. */
	nand_chip->legacy.chip_delay = 20;	/* 20us command delay time */

	/*
	 * This driver assumes that the default ECC engine should be TYPE_SOFT.
	 * Set ->engine_type before registering the NAND devices in order to
	 * provide a driver specific default value.
	 */
	nand_chip->ecc.engine_type = NAND_ECC_ENGINE_TYPE_SOFT;
>>>>>>> upstream/android-13

	dev_set_drvdata(&ofdev->dev, host);

	res = nand_scan(nand_chip, 1);
	if (res)
		goto out;

	res = mtd_device_register(mtd, NULL, 0);
	if (!res)
		return res;

	nand_cleanup(nand_chip);

out:
	iounmap(host->io_base);
	return res;
}

/*
 * Remove a NAND device.
 */
static int socrates_nand_remove(struct platform_device *ofdev)
{
	struct socrates_nand_host *host = dev_get_drvdata(&ofdev->dev);
<<<<<<< HEAD

	nand_release(&host->nand_chip);
=======
	struct nand_chip *chip = &host->nand_chip;
	int ret;

	ret = mtd_device_unregister(nand_to_mtd(chip));
	WARN_ON(ret);
	nand_cleanup(chip);
>>>>>>> upstream/android-13

	iounmap(host->io_base);

	return 0;
}

static const struct of_device_id socrates_nand_match[] =
{
	{
		.compatible   = "abb,socrates-nand",
	},
	{},
};

MODULE_DEVICE_TABLE(of, socrates_nand_match);

static struct platform_driver socrates_nand_driver = {
	.driver = {
		.name = "socrates_nand",
		.of_match_table = socrates_nand_match,
	},
	.probe		= socrates_nand_probe,
	.remove		= socrates_nand_remove,
};

module_platform_driver(socrates_nand_driver);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Ilya Yanok");
MODULE_DESCRIPTION("NAND driver for Socrates board");
