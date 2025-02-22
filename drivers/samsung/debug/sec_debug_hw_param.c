<<<<<<< HEAD
/*
 *sec_debug_hw_param.c
 *
 * Copyright (c) 2016 Samsung Electronics Co., Ltd
 *              http://www.samsung.com
 *
 *  This program is free software; you can redistribute  it and/or modify it
 *  under  the terms of  the GNU General  Public License as published by the
 *  Free Software Foundation;  either version 2 of the  License, or (at your
 *  option) any later version.
 *
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/sec_debug.h>
#include <linux/sec_class.h>
#include <linux/sec_ext.h>
#include <linux/uaccess.h>

#define MAX_DDR_VENDOR 16
#define DATA_SIZE 1024

#define HW_PARAM_CHECK_SIZE(size)	\
	if (size >= 1024)		\
	return 1024;			\

extern char *sec_debug_extra_info_buf;
extern struct sec_debug_panic_extra_info *sec_debug_extra_info_backup;

unsigned int sec_reset_cnt;

static int __init sec_hw_param_get_reset_count(char *arg)
{
	get_option(&arg, &sec_reset_cnt);
	return 0;
}

early_param("sec_debug.reset_rwc", sec_hw_param_get_reset_count);

static ssize_t sec_hw_param_extra_info_show(struct kobject *kobj,
				struct kobj_attribute *attr, char *buf)
{
	ssize_t info_size = 0;

	if (!sec_debug_extra_info_buf)
		return -ENOENT;

	if (reset_reason == RR_K || reset_reason == RR_D || reset_reason == RR_P) {
		sec_debug_store_extra_info_A();
		strncpy(buf, sec_debug_extra_info_buf, SZ_1K);
		info_size = strlen(buf);
	}
=======
// SPDX-License-Identifier: GPL-2.0-only
/*
 * sec_debug_hw_param.c
 *
 * Copyright (c) 2019 Samsung Electronics Co., Ltd
 *              http://www.samsung.com
 */

#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/kernel.h>
#include <linux/kobject.h>
#include <linux/device.h>
#include <linux/init.h>
#include <linux/io.h>
#include <linux/sec_class.h>
#include <linux/sec_ext.h>
#include <soc/samsung/exynos-pm.h>
#include <soc/samsung/exynos/exynos-soc.h>

#include "sec_debug_internal.h"

/* maximum size of sysfs */
#define DATA_SIZE 1024
#define LOT_STRING_LEN 5

/* function name prefix: secdbg_hprm */
#if defined(CONFIG_SAMSUNG_VST_CAL)
/* from NAD related bootloader work */
static unsigned int vst_result;
module_param(vst_result, unsigned int, 0440);
#endif

char __read_mostly *dram_size;
module_param(dram_size, charp, 0440);

/* this is same with androidboot.dram_info */
char __read_mostly *dram_info;
module_param(dram_info, charp, 0440);

static u32 chipid_reverse_value(u32 value, u32 bitcnt)
{
	int tmp, ret = 0, i;

	for (i = 0; i < bitcnt; i++) {
		tmp = (value >> i) & 0x1;
		ret += tmp << ((bitcnt - 1) - i);
	}

	return ret;
}

static void chipid_dec_to_36(u32 in, char *p)
{
	int mod, i;

	for (i = LOT_STRING_LEN - 1; i >= 1; i--) {
		mod = in % 36;
		in /= 36;
		p[i] = (mod < 10) ? (mod + '0') : (mod - 10 + 'A');
	}

	p[0] = 'N';
	p[LOT_STRING_LEN] = '\0';
}

/* sysfs show functions */
static ssize_t secdbg_hprm_ap_info_show(struct device *dev,
					 struct device_attribute *attr, char *buf)
{
	ssize_t info_size = 0;
	int reverse_id_0 = 0;
	u32 tmp = 0;
	char lot_id[LOT_STRING_LEN + 1];
	char val[32] = {0, };

	reverse_id_0 = chipid_reverse_value(exynos_soc_info.lot_id, 32);
	tmp = (reverse_id_0 >> 11) & 0x1FFFFF;
	chipid_dec_to_36(tmp, lot_id);

	memset(val, 0, 32);
	get_bk_item_val_as_string("RSTCNT", val);
	info_size +=
		snprintf((char *)(buf + info_size), DATA_SIZE - info_size,
				"\"RSTCNT\":\"%s\",", val);

	memset(val, 0, 32);
	get_bk_item_val_as_string("CHI", val);
	info_size +=
		snprintf((char *)(buf + info_size), DATA_SIZE - info_size,
				"\"CHIPID_FAIL\":\"%s\",", val);

	memset(val, 0, 32);
	get_bk_item_val_as_string("LPI", val);
	info_size +=
		snprintf((char *)(buf + info_size), DATA_SIZE - info_size,
				"\"LPI_TIMEOUT\":\"%s\",", val);

	memset(val, 0, 32);
	get_bk_item_val_as_string("CDI", val);
	info_size +=
		snprintf((char *)(buf + info_size), DATA_SIZE - info_size,
				"\"CODE_DIFF\":\"%s\",", val);

	memset(val, 0, 32);
	get_bk_item_val_as_string("BIN", val);
	info_size +=
		snprintf((char *)(buf + info_size), DATA_SIZE - info_size,
				"\"BIN\":\"%s\",", val);

	info_size +=
		snprintf((char *)(buf + info_size), DATA_SIZE - info_size,
				"\"ASB\":\"%d\",", id_get_asb_ver());

	info_size +=
		snprintf((char *)(buf + info_size), DATA_SIZE - info_size,
				"\"PSITE\":\"%d\",", id_get_product_line());
	info_size +=
		snprintf((char *)(buf + info_size), DATA_SIZE - info_size,
				"\"LOT_ID\":\"%s\",", lot_id);
#if defined(CONFIG_SAMSUNG_VST_CAL)
	info_size +=
		snprintf((char *)(buf + info_size), DATA_SIZE - info_size,
				"\"VST_RESULT\":\"%d\",", vst_result);
	info_size +=
		snprintf((char *)(buf + info_size), DATA_SIZE - info_size,
				"\"VST_ADJUST\":\"%d\",", volt_vst_cal_bdata);
#endif
>>>>>>> upstream/android-13

	return info_size;
}

<<<<<<< HEAD
static ssize_t sec_hw_param_extrb_info_show(struct kobject *kobj,
					    struct kobj_attribute *attr, char *buf)
{
	ssize_t info_size = 0;

	if (!sec_debug_extra_info_buf)
		return -ENOENT;

	if (reset_reason == RR_K || reset_reason == RR_D || reset_reason == RR_P) {
		sec_debug_store_extra_info_B();
		strncpy(buf, sec_debug_extra_info_buf, SZ_1K);
		info_size = strlen(buf);
	}
=======
static ssize_t secdbg_hprm_ddr_info_show(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	ssize_t info_size = 0;

	info_size += snprintf((char *)(buf), DATA_SIZE, "\"DDRV\":\"%s\"", dram_info);
>>>>>>> upstream/android-13

	return info_size;
}

<<<<<<< HEAD
static ssize_t sec_hw_param_extrc_info_show(struct kobject *kobj,
					    struct kobj_attribute *attr, char *buf)
{
	ssize_t info_size = 0;

	if (!sec_debug_extra_info_buf)
		return -ENOENT;

	if (reset_reason == RR_K || reset_reason == RR_D || reset_reason == RR_P) {
		sec_debug_store_extra_info_C();
		strncpy(buf, sec_debug_extra_info_buf, SZ_1K);
		info_size = strlen(buf);
	}
=======
static ssize_t secdbg_hprm_extra_info_show(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	ssize_t info_size = 0;

	secdbg_exin_get_extra_info_A(buf);
	info_size = strlen(buf);
>>>>>>> upstream/android-13

	return info_size;
}

<<<<<<< HEAD
static ssize_t sec_hw_param_extrm_info_show(struct kobject *kobj,
					    struct kobj_attribute *attr, char *buf)
{
	ssize_t info_size = 0;

	if (!sec_debug_extra_info_buf)
		return -ENOENT;

	if (reset_reason == RR_K || reset_reason == RR_D || reset_reason == RR_P) {
		sec_debug_store_extra_info_M();
		strncpy(buf, sec_debug_extra_info_buf, SZ_1K);
		info_size = strlen(buf);
	}
=======
static ssize_t secdbg_hprm_extrb_info_show(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	ssize_t info_size = 0;

	secdbg_exin_get_extra_info_B(buf);
	info_size = strlen(buf);
>>>>>>> upstream/android-13

	return info_size;
}

<<<<<<< HEAD
static struct kobj_attribute sec_hw_param_extra_info_attr =
		__ATTR(extra_info, 0440, sec_hw_param_extra_info_show, NULL);

static struct kobj_attribute sec_hw_param_extrb_info_attr =
		__ATTR(extrb_info, 0440, sec_hw_param_extrb_info_show, NULL);

static struct kobj_attribute sec_hw_param_extrc_info_attr =
		__ATTR(extrc_info, 0440, sec_hw_param_extrc_info_show, NULL);

static struct kobj_attribute sec_hw_param_extrm_info_attr =
		__ATTR(extrm_info, 0440, sec_hw_param_extrm_info_show, NULL);

static struct attribute *sec_hw_param_attributes[] = {
	&sec_hw_param_extra_info_attr.attr,
	&sec_hw_param_extrb_info_attr.attr,
	&sec_hw_param_extrc_info_attr.attr,
	&sec_hw_param_extrm_info_attr.attr,
	NULL,
};

static struct attribute_group sec_hw_param_attr_group = {
	.attrs = sec_hw_param_attributes,
};

static int __init sec_hw_param_init(void)
=======
static ssize_t secdbg_hprm_extrc_info_show(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	ssize_t info_size = 0;

	secdbg_exin_get_extra_info_C(buf);
	info_size = strlen(buf);

	return info_size;
}

static ssize_t secdbg_hprm_extrm_info_show(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	ssize_t info_size = 0;

	secdbg_exin_get_extra_info_M(buf);
	info_size = strlen(buf);

	return info_size;
}

static ssize_t secdbg_hprm_extrf_info_show(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	ssize_t info_size = 0;

	secdbg_exin_get_extra_info_F(buf);
	info_size = strlen(buf);

	return info_size;
}

static ssize_t secdbg_hprm_extrt_info_show(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	ssize_t info_size = 0;

	secdbg_exin_get_extra_info_T(buf);
	info_size = strlen(buf);

	return info_size;
}

static ssize_t secdbg_hprm_thermal_info_show(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	ssize_t info_size = 0;

	return info_size;
}

static DEVICE_ATTR(ap_info, 0440, secdbg_hprm_ap_info_show, NULL);
static DEVICE_ATTR(ddr_info, 0440, secdbg_hprm_ddr_info_show, NULL);
static DEVICE_ATTR(extra_info, 0440, secdbg_hprm_extra_info_show, NULL);
static DEVICE_ATTR(extrb_info, 0440, secdbg_hprm_extrb_info_show, NULL);
static DEVICE_ATTR(extrc_info, 0440, secdbg_hprm_extrc_info_show, NULL);
static DEVICE_ATTR(extrm_info, 0440, secdbg_hprm_extrm_info_show, NULL);
static DEVICE_ATTR(extrf_info, 0440, secdbg_hprm_extrf_info_show, NULL);
static DEVICE_ATTR(extrt_info, 0440, secdbg_hprm_extrt_info_show, NULL);
static DEVICE_ATTR(thermal_info, 0440, secdbg_hprm_thermal_info_show, NULL);

static struct attribute *secdbg_hprm_attributes[] = {
	&dev_attr_ap_info.attr,
	&dev_attr_ddr_info.attr,
	&dev_attr_extra_info.attr,
	&dev_attr_extrb_info.attr,
	&dev_attr_extrc_info.attr,
	&dev_attr_extrm_info.attr,
	&dev_attr_extrf_info.attr,
	&dev_attr_extrt_info.attr,
	&dev_attr_thermal_info.attr,
	NULL,
};

static struct attribute_group secdbg_hprm_attr_group = {
	.attrs = secdbg_hprm_attributes,
};

static void secdbg_hprm_set_hw_exin(void)
{
	secdbg_exin_set_hwid(id_get_asb_ver(), id_get_product_line(), dram_info);
}

static int __init secdbg_hw_param_init(void)
>>>>>>> upstream/android-13
{
	int ret = 0;
	struct device *dev;

<<<<<<< HEAD
	dev = sec_device_create(NULL, "sec_hw_param");

	ret = sysfs_create_group(&dev->kobj, &sec_hw_param_attr_group);
=======
	pr_info("%s: start\n", __func__);
	pr_info("%s: from cmdline: dram_size: %s\n", __func__, dram_size);
	pr_info("%s: from cmdline: dram_info: %s\n", __func__, dram_info);

	secdbg_hprm_set_hw_exin();

	dev = sec_device_create(NULL, "sec_hw_param");

	ret = sysfs_create_group(&dev->kobj, &secdbg_hprm_attr_group);
>>>>>>> upstream/android-13
	if (ret)
		pr_err("%s : could not create sysfs noden", __func__);

	return 0;
}
<<<<<<< HEAD

device_initcall(sec_hw_param_init);
=======
module_init(secdbg_hw_param_init);

MODULE_DESCRIPTION("Samsung Debug HW Parameter driver");
MODULE_LICENSE("GPL v2");
>>>>>>> upstream/android-13
