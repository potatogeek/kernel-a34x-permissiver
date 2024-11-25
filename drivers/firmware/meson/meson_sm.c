<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> upstream/android-13
/*
 * Amlogic Secure Monitor driver
 *
 * Copyright (C) 2016 Endless Mobile, Inc.
 * Author: Carlo Caione <carlo@endlessm.com>
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
=======
>>>>>>> upstream/android-13
 */

#define pr_fmt(fmt) "meson-sm: " fmt

#include <linux/arm-smccc.h>
#include <linux/bug.h>
#include <linux/io.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/of_device.h>
#include <linux/platform_device.h>
#include <linux/printk.h>
#include <linux/types.h>
#include <linux/sizes.h>
<<<<<<< HEAD
=======
 #include <linux/slab.h>
>>>>>>> upstream/android-13

#include <linux/firmware/meson/meson_sm.h>

struct meson_sm_cmd {
	unsigned int index;
	u32 smc_id;
};
#define CMD(d, s) { .index = (d), .smc_id = (s), }

struct meson_sm_chip {
	unsigned int shmem_size;
	u32 cmd_shmem_in_base;
	u32 cmd_shmem_out_base;
	struct meson_sm_cmd cmd[];
};

<<<<<<< HEAD
struct meson_sm_chip gxbb_chip = {
=======
static const struct meson_sm_chip gxbb_chip = {
>>>>>>> upstream/android-13
	.shmem_size		= SZ_4K,
	.cmd_shmem_in_base	= 0x82000020,
	.cmd_shmem_out_base	= 0x82000021,
	.cmd = {
		CMD(SM_EFUSE_READ,	0x82000030),
		CMD(SM_EFUSE_WRITE,	0x82000031),
		CMD(SM_EFUSE_USER_MAX,	0x82000033),
<<<<<<< HEAD
=======
		CMD(SM_GET_CHIP_ID,	0x82000044),
		CMD(SM_A1_PWRC_SET,	0x82000093),
		CMD(SM_A1_PWRC_GET,	0x82000095),
>>>>>>> upstream/android-13
		{ /* sentinel */ },
	},
};

struct meson_sm_firmware {
	const struct meson_sm_chip *chip;
	void __iomem *sm_shmem_in_base;
	void __iomem *sm_shmem_out_base;
};

<<<<<<< HEAD
static struct meson_sm_firmware fw;

=======
>>>>>>> upstream/android-13
static u32 meson_sm_get_cmd(const struct meson_sm_chip *chip,
			    unsigned int cmd_index)
{
	const struct meson_sm_cmd *cmd = chip->cmd;

	while (cmd->smc_id && cmd->index != cmd_index)
		cmd++;

	return cmd->smc_id;
}

static u32 __meson_sm_call(u32 cmd, u32 arg0, u32 arg1, u32 arg2,
			   u32 arg3, u32 arg4)
{
	struct arm_smccc_res res;

	arm_smccc_smc(cmd, arg0, arg1, arg2, arg3, arg4, 0, 0, &res);
	return res.a0;
}

static void __iomem *meson_sm_map_shmem(u32 cmd_shmem, unsigned int size)
{
	u32 sm_phy_base;

	sm_phy_base = __meson_sm_call(cmd_shmem, 0, 0, 0, 0, 0);
	if (!sm_phy_base)
		return 0;

	return ioremap_cache(sm_phy_base, size);
}

/**
 * meson_sm_call - generic SMC32 call to the secure-monitor
 *
<<<<<<< HEAD
=======
 * @fw:		Pointer to secure-monitor firmware
>>>>>>> upstream/android-13
 * @cmd_index:	Index of the SMC32 function ID
 * @ret:	Returned value
 * @arg0:	SMC32 Argument 0
 * @arg1:	SMC32 Argument 1
 * @arg2:	SMC32 Argument 2
 * @arg3:	SMC32 Argument 3
 * @arg4:	SMC32 Argument 4
 *
 * Return:	0 on success, a negative value on error
 */
<<<<<<< HEAD
int meson_sm_call(unsigned int cmd_index, u32 *ret, u32 arg0,
		  u32 arg1, u32 arg2, u32 arg3, u32 arg4)
{
	u32 cmd, lret;

	if (!fw.chip)
		return -ENOENT;

	cmd = meson_sm_get_cmd(fw.chip, cmd_index);
=======
int meson_sm_call(struct meson_sm_firmware *fw, unsigned int cmd_index,
		  u32 *ret, u32 arg0, u32 arg1, u32 arg2, u32 arg3, u32 arg4)
{
	u32 cmd, lret;

	if (!fw->chip)
		return -ENOENT;

	cmd = meson_sm_get_cmd(fw->chip, cmd_index);
>>>>>>> upstream/android-13
	if (!cmd)
		return -EINVAL;

	lret = __meson_sm_call(cmd, arg0, arg1, arg2, arg3, arg4);

	if (ret)
		*ret = lret;

	return 0;
}
EXPORT_SYMBOL(meson_sm_call);

/**
 * meson_sm_call_read - retrieve data from secure-monitor
 *
<<<<<<< HEAD
=======
 * @fw:		Pointer to secure-monitor firmware
>>>>>>> upstream/android-13
 * @buffer:	Buffer to store the retrieved data
 * @bsize:	Size of the buffer
 * @cmd_index:	Index of the SMC32 function ID
 * @arg0:	SMC32 Argument 0
 * @arg1:	SMC32 Argument 1
 * @arg2:	SMC32 Argument 2
 * @arg3:	SMC32 Argument 3
 * @arg4:	SMC32 Argument 4
 *
 * Return:	size of read data on success, a negative value on error
 *		When 0 is returned there is no guarantee about the amount of
 *		data read and bsize bytes are copied in buffer.
 */
<<<<<<< HEAD
int meson_sm_call_read(void *buffer, unsigned int bsize, unsigned int cmd_index,
		       u32 arg0, u32 arg1, u32 arg2, u32 arg3, u32 arg4)
=======
int meson_sm_call_read(struct meson_sm_firmware *fw, void *buffer,
		       unsigned int bsize, unsigned int cmd_index, u32 arg0,
		       u32 arg1, u32 arg2, u32 arg3, u32 arg4)
>>>>>>> upstream/android-13
{
	u32 size;
	int ret;

<<<<<<< HEAD
	if (!fw.chip)
		return -ENOENT;

	if (!fw.chip->cmd_shmem_out_base)
		return -EINVAL;

	if (bsize > fw.chip->shmem_size)
		return -EINVAL;

	if (meson_sm_call(cmd_index, &size, arg0, arg1, arg2, arg3, arg4) < 0)
=======
	if (!fw->chip)
		return -ENOENT;

	if (!fw->chip->cmd_shmem_out_base)
		return -EINVAL;

	if (bsize > fw->chip->shmem_size)
		return -EINVAL;

	if (meson_sm_call(fw, cmd_index, &size, arg0, arg1, arg2, arg3, arg4) < 0)
>>>>>>> upstream/android-13
		return -EINVAL;

	if (size > bsize)
		return -EINVAL;

	ret = size;

	if (!size)
		size = bsize;

	if (buffer)
<<<<<<< HEAD
		memcpy(buffer, fw.sm_shmem_out_base, size);
=======
		memcpy(buffer, fw->sm_shmem_out_base, size);
>>>>>>> upstream/android-13

	return ret;
}
EXPORT_SYMBOL(meson_sm_call_read);

/**
 * meson_sm_call_write - send data to secure-monitor
 *
<<<<<<< HEAD
=======
 * @fw:		Pointer to secure-monitor firmware
>>>>>>> upstream/android-13
 * @buffer:	Buffer containing data to send
 * @size:	Size of the data to send
 * @cmd_index:	Index of the SMC32 function ID
 * @arg0:	SMC32 Argument 0
 * @arg1:	SMC32 Argument 1
 * @arg2:	SMC32 Argument 2
 * @arg3:	SMC32 Argument 3
 * @arg4:	SMC32 Argument 4
 *
 * Return:	size of sent data on success, a negative value on error
 */
<<<<<<< HEAD
int meson_sm_call_write(void *buffer, unsigned int size, unsigned int cmd_index,
			u32 arg0, u32 arg1, u32 arg2, u32 arg3, u32 arg4)
{
	u32 written;

	if (!fw.chip)
		return -ENOENT;

	if (size > fw.chip->shmem_size)
		return -EINVAL;

	if (!fw.chip->cmd_shmem_in_base)
		return -EINVAL;

	memcpy(fw.sm_shmem_in_base, buffer, size);

	if (meson_sm_call(cmd_index, &written, arg0, arg1, arg2, arg3, arg4) < 0)
=======
int meson_sm_call_write(struct meson_sm_firmware *fw, void *buffer,
			unsigned int size, unsigned int cmd_index, u32 arg0,
			u32 arg1, u32 arg2, u32 arg3, u32 arg4)
{
	u32 written;

	if (!fw->chip)
		return -ENOENT;

	if (size > fw->chip->shmem_size)
		return -EINVAL;

	if (!fw->chip->cmd_shmem_in_base)
		return -EINVAL;

	memcpy(fw->sm_shmem_in_base, buffer, size);

	if (meson_sm_call(fw, cmd_index, &written, arg0, arg1, arg2, arg3, arg4) < 0)
>>>>>>> upstream/android-13
		return -EINVAL;

	if (!written)
		return -EINVAL;

	return written;
}
EXPORT_SYMBOL(meson_sm_call_write);

<<<<<<< HEAD
=======
/**
 * meson_sm_get - get pointer to meson_sm_firmware structure.
 *
 * @sm_node:		Pointer to the secure-monitor Device Tree node.
 *
 * Return:		NULL is the secure-monitor device is not ready.
 */
struct meson_sm_firmware *meson_sm_get(struct device_node *sm_node)
{
	struct platform_device *pdev = of_find_device_by_node(sm_node);

	if (!pdev)
		return NULL;

	return platform_get_drvdata(pdev);
}
EXPORT_SYMBOL_GPL(meson_sm_get);

#define SM_CHIP_ID_LENGTH	119
#define SM_CHIP_ID_OFFSET	4
#define SM_CHIP_ID_SIZE		12

static ssize_t serial_show(struct device *dev, struct device_attribute *attr,
			 char *buf)
{
	struct platform_device *pdev = to_platform_device(dev);
	struct meson_sm_firmware *fw;
	uint8_t *id_buf;
	int ret;

	fw = platform_get_drvdata(pdev);

	id_buf = kmalloc(SM_CHIP_ID_LENGTH, GFP_KERNEL);
	if (!id_buf)
		return -ENOMEM;

	ret = meson_sm_call_read(fw, id_buf, SM_CHIP_ID_LENGTH, SM_GET_CHIP_ID,
				 0, 0, 0, 0, 0);
	if (ret < 0) {
		kfree(id_buf);
		return ret;
	}

	ret = sprintf(buf, "%12phN\n", &id_buf[SM_CHIP_ID_OFFSET]);

	kfree(id_buf);

	return ret;
}

static DEVICE_ATTR_RO(serial);

static struct attribute *meson_sm_sysfs_attributes[] = {
	&dev_attr_serial.attr,
	NULL,
};

static const struct attribute_group meson_sm_sysfs_attr_group = {
	.attrs = meson_sm_sysfs_attributes,
};

>>>>>>> upstream/android-13
static const struct of_device_id meson_sm_ids[] = {
	{ .compatible = "amlogic,meson-gxbb-sm", .data = &gxbb_chip },
	{ /* sentinel */ },
};

static int __init meson_sm_probe(struct platform_device *pdev)
{
<<<<<<< HEAD
	const struct meson_sm_chip *chip;

	chip = of_match_device(meson_sm_ids, &pdev->dev)->data;

	if (chip->cmd_shmem_in_base) {
		fw.sm_shmem_in_base = meson_sm_map_shmem(chip->cmd_shmem_in_base,
							 chip->shmem_size);
		if (WARN_ON(!fw.sm_shmem_in_base))
=======
	struct device *dev = &pdev->dev;
	const struct meson_sm_chip *chip;
	struct meson_sm_firmware *fw;

	fw = devm_kzalloc(dev, sizeof(*fw), GFP_KERNEL);
	if (!fw)
		return -ENOMEM;

	chip = of_match_device(meson_sm_ids, dev)->data;

	if (chip->cmd_shmem_in_base) {
		fw->sm_shmem_in_base = meson_sm_map_shmem(chip->cmd_shmem_in_base,
							  chip->shmem_size);
		if (WARN_ON(!fw->sm_shmem_in_base))
>>>>>>> upstream/android-13
			goto out;
	}

	if (chip->cmd_shmem_out_base) {
<<<<<<< HEAD
		fw.sm_shmem_out_base = meson_sm_map_shmem(chip->cmd_shmem_out_base,
							  chip->shmem_size);
		if (WARN_ON(!fw.sm_shmem_out_base))
			goto out_in_base;
	}

	fw.chip = chip;
	pr_info("secure-monitor enabled\n");

	return 0;

out_in_base:
	iounmap(fw.sm_shmem_in_base);
=======
		fw->sm_shmem_out_base = meson_sm_map_shmem(chip->cmd_shmem_out_base,
							   chip->shmem_size);
		if (WARN_ON(!fw->sm_shmem_out_base))
			goto out_in_base;
	}

	fw->chip = chip;

	platform_set_drvdata(pdev, fw);

	pr_info("secure-monitor enabled\n");

	if (sysfs_create_group(&pdev->dev.kobj, &meson_sm_sysfs_attr_group))
		goto out_in_base;

	return 0;

out_in_base:
	iounmap(fw->sm_shmem_in_base);
>>>>>>> upstream/android-13
out:
	return -EINVAL;
}

static struct platform_driver meson_sm_driver = {
	.driver = {
		.name = "meson-sm",
		.of_match_table = of_match_ptr(meson_sm_ids),
	},
};
module_platform_driver_probe(meson_sm_driver, meson_sm_probe);
<<<<<<< HEAD
=======
MODULE_LICENSE("GPL v2");
>>>>>>> upstream/android-13
