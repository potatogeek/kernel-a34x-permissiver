<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> upstream/android-13
/*
 * Copyright (C) 2006-2007 PA Semi, Inc
 *
 * Author: Egor Martovetsky <egor@pasemi.com>
 * Maintained by: Olof Johansson <olof@lixom.net>
 *
 * Driver for the PWRficient onchip NAND flash interface
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
=======
>>>>>>> upstream/android-13
 */

#undef DEBUG

#include <linux/slab.h>
#include <linux/module.h>
#include <linux/mtd/mtd.h>
#include <linux/mtd/rawnand.h>
<<<<<<< HEAD
#include <linux/mtd/nand_ecc.h>
=======
>>>>>>> upstream/android-13
#include <linux/of_address.h>
#include <linux/of_irq.h>
#include <linux/of_platform.h>
#include <linux/platform_device.h>
#include <linux/pci.h>

#include <asm/io.h>

#define LBICTRL_LPCCTL_NR		0x00004000
#define CLE_PIN_CTL			15
#define ALE_PIN_CTL			14

static unsigned int lpcctl;
static struct mtd_info *pasemi_nand_mtd;
<<<<<<< HEAD
static const char driver_name[] = "pasemi-nand";

static void pasemi_read_buf(struct mtd_info *mtd, u_char *buf, int len)
{
	struct nand_chip *chip = mtd_to_nand(mtd);

	while (len > 0x800) {
		memcpy_fromio(buf, chip->IO_ADDR_R, 0x800);
		buf += 0x800;
		len -= 0x800;
	}
	memcpy_fromio(buf, chip->IO_ADDR_R, len);
}

static void pasemi_write_buf(struct mtd_info *mtd, const u_char *buf, int len)
{
	struct nand_chip *chip = mtd_to_nand(mtd);

	while (len > 0x800) {
		memcpy_toio(chip->IO_ADDR_R, buf, 0x800);
		buf += 0x800;
		len -= 0x800;
	}
	memcpy_toio(chip->IO_ADDR_R, buf, len);
}

static void pasemi_hwcontrol(struct mtd_info *mtd, int cmd,
			     unsigned int ctrl)
{
	struct nand_chip *chip = mtd_to_nand(mtd);

=======
static struct nand_controller controller;
static const char driver_name[] = "pasemi-nand";

static void pasemi_read_buf(struct nand_chip *chip, u_char *buf, int len)
{
	while (len > 0x800) {
		memcpy_fromio(buf, chip->legacy.IO_ADDR_R, 0x800);
		buf += 0x800;
		len -= 0x800;
	}
	memcpy_fromio(buf, chip->legacy.IO_ADDR_R, len);
}

static void pasemi_write_buf(struct nand_chip *chip, const u_char *buf,
			     int len)
{
	while (len > 0x800) {
		memcpy_toio(chip->legacy.IO_ADDR_R, buf, 0x800);
		buf += 0x800;
		len -= 0x800;
	}
	memcpy_toio(chip->legacy.IO_ADDR_R, buf, len);
}

static void pasemi_hwcontrol(struct nand_chip *chip, int cmd,
			     unsigned int ctrl)
{
>>>>>>> upstream/android-13
	if (cmd == NAND_CMD_NONE)
		return;

	if (ctrl & NAND_CLE)
<<<<<<< HEAD
		out_8(chip->IO_ADDR_W + (1 << CLE_PIN_CTL), cmd);
	else
		out_8(chip->IO_ADDR_W + (1 << ALE_PIN_CTL), cmd);
=======
		out_8(chip->legacy.IO_ADDR_W + (1 << CLE_PIN_CTL), cmd);
	else
		out_8(chip->legacy.IO_ADDR_W + (1 << ALE_PIN_CTL), cmd);
>>>>>>> upstream/android-13

	/* Push out posted writes */
	eieio();
	inl(lpcctl);
}

<<<<<<< HEAD
int pasemi_device_ready(struct mtd_info *mtd)
=======
static int pasemi_device_ready(struct nand_chip *chip)
>>>>>>> upstream/android-13
{
	return !!(inl(lpcctl) & LBICTRL_LPCCTL_NR);
}

<<<<<<< HEAD
=======
static int pasemi_attach_chip(struct nand_chip *chip)
{
	if (chip->ecc.engine_type == NAND_ECC_ENGINE_TYPE_SOFT &&
	    chip->ecc.algo == NAND_ECC_ALGO_UNKNOWN)
		chip->ecc.algo = NAND_ECC_ALGO_HAMMING;

	return 0;
}

static const struct nand_controller_ops pasemi_ops = {
	.attach_chip = pasemi_attach_chip,
};

>>>>>>> upstream/android-13
static int pasemi_nand_probe(struct platform_device *ofdev)
{
	struct device *dev = &ofdev->dev;
	struct pci_dev *pdev;
	struct device_node *np = dev->of_node;
	struct resource res;
	struct nand_chip *chip;
	int err = 0;

	err = of_address_to_resource(np, 0, &res);

	if (err)
		return -EINVAL;

	/* We only support one device at the moment */
	if (pasemi_nand_mtd)
		return -ENODEV;

	dev_dbg(dev, "pasemi_nand at %pR\n", &res);

	/* Allocate memory for MTD device structure and private data */
	chip = kzalloc(sizeof(struct nand_chip), GFP_KERNEL);
	if (!chip) {
		err = -ENOMEM;
		goto out;
	}

<<<<<<< HEAD
=======
	controller.ops = &pasemi_ops;
	nand_controller_init(&controller);
	chip->controller = &controller;

>>>>>>> upstream/android-13
	pasemi_nand_mtd = nand_to_mtd(chip);

	/* Link the private data with the MTD structure */
	pasemi_nand_mtd->dev.parent = dev;

<<<<<<< HEAD
	chip->IO_ADDR_R = of_iomap(np, 0);
	chip->IO_ADDR_W = chip->IO_ADDR_R;

	if (!chip->IO_ADDR_R) {
=======
	chip->legacy.IO_ADDR_R = of_iomap(np, 0);
	chip->legacy.IO_ADDR_W = chip->legacy.IO_ADDR_R;

	if (!chip->legacy.IO_ADDR_R) {
>>>>>>> upstream/android-13
		err = -EIO;
		goto out_mtd;
	}

	pdev = pci_get_device(PCI_VENDOR_ID_PASEMI, 0xa008, NULL);
	if (!pdev) {
		err = -ENODEV;
		goto out_ior;
	}

	lpcctl = pci_resource_start(pdev, 0);
	pci_dev_put(pdev);

	if (!request_region(lpcctl, 4, driver_name)) {
		err = -EBUSY;
		goto out_ior;
	}

<<<<<<< HEAD
	chip->cmd_ctrl = pasemi_hwcontrol;
	chip->dev_ready = pasemi_device_ready;
	chip->read_buf = pasemi_read_buf;
	chip->write_buf = pasemi_write_buf;
	chip->chip_delay = 0;
	chip->ecc.mode = NAND_ECC_SOFT;
	chip->ecc.algo = NAND_ECC_HAMMING;
=======
	chip->legacy.cmd_ctrl = pasemi_hwcontrol;
	chip->legacy.dev_ready = pasemi_device_ready;
	chip->legacy.read_buf = pasemi_read_buf;
	chip->legacy.write_buf = pasemi_write_buf;
	chip->legacy.chip_delay = 0;
>>>>>>> upstream/android-13

	/* Enable the following for a flash based bad block table */
	chip->bbt_options = NAND_BBT_USE_FLASH;

<<<<<<< HEAD
=======
	/*
	 * This driver assumes that the default ECC engine should be TYPE_SOFT.
	 * Set ->engine_type before registering the NAND devices in order to
	 * provide a driver specific default value.
	 */
	chip->ecc.engine_type = NAND_ECC_ENGINE_TYPE_SOFT;

>>>>>>> upstream/android-13
	/* Scan to find existence of the device */
	err = nand_scan(chip, 1);
	if (err)
		goto out_lpc;

	if (mtd_device_register(pasemi_nand_mtd, NULL, 0)) {
		dev_err(dev, "Unable to register MTD device\n");
		err = -ENODEV;
		goto out_cleanup_nand;
	}

	dev_info(dev, "PA Semi NAND flash at %pR, control at I/O %x\n", &res,
		 lpcctl);

	return 0;

 out_cleanup_nand:
	nand_cleanup(chip);
 out_lpc:
	release_region(lpcctl, 4);
 out_ior:
<<<<<<< HEAD
	iounmap(chip->IO_ADDR_R);
=======
	iounmap(chip->legacy.IO_ADDR_R);
>>>>>>> upstream/android-13
 out_mtd:
	kfree(chip);
 out:
	return err;
}

static int pasemi_nand_remove(struct platform_device *ofdev)
{
	struct nand_chip *chip;
<<<<<<< HEAD
=======
	int ret;
>>>>>>> upstream/android-13

	if (!pasemi_nand_mtd)
		return 0;

	chip = mtd_to_nand(pasemi_nand_mtd);

	/* Release resources, unregister device */
<<<<<<< HEAD
	nand_release(chip);

	release_region(lpcctl, 4);

	iounmap(chip->IO_ADDR_R);
=======
	ret = mtd_device_unregister(pasemi_nand_mtd);
	WARN_ON(ret);
	nand_cleanup(chip);

	release_region(lpcctl, 4);

	iounmap(chip->legacy.IO_ADDR_R);
>>>>>>> upstream/android-13

	/* Free the MTD device structure */
	kfree(chip);

	pasemi_nand_mtd = NULL;

	return 0;
}

static const struct of_device_id pasemi_nand_match[] =
{
	{
		.compatible   = "pasemi,localbus-nand",
	},
	{},
};

MODULE_DEVICE_TABLE(of, pasemi_nand_match);

static struct platform_driver pasemi_nand_driver =
{
	.driver = {
		.name = driver_name,
		.of_match_table = pasemi_nand_match,
	},
	.probe		= pasemi_nand_probe,
	.remove		= pasemi_nand_remove,
};

module_platform_driver(pasemi_nand_driver);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Egor Martovetsky <egor@pasemi.com>");
MODULE_DESCRIPTION("NAND flash interface driver for PA Semi PWRficient");
