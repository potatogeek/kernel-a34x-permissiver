<<<<<<< HEAD
/* abc_hub.c
 *
 * Abnormal Behavior Catcher Hub Driver
 *
 * Copyright (C) 2017 Samsung Electronics
 *
 * Sangsu Ha <sangsu.ha@samsung.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#include <linux/sti/abc_hub.h>
#if IS_ENABLED(CONFIG_SEC_KUNIT)
#include <linux/sti/abc_kunit.h>
#endif

__visible_for_testing
struct device *abc_hub_dev;
EXPORT_SYMBOL_KUNIT(abc_hub_dev);
struct abc_hub_info *abc_hub_pinfo;
EXPORT_SYMBOL_KUNIT(abc_hub_pinfo);
__visible_for_testing
int abc_hub_probed;
EXPORT_SYMBOL_KUNIT(abc_hub_probed);

#if IS_ENABLED(CONFIG_OF)
static int abc_hub_parse_dt(struct device *dev)
{
	struct abc_hub_platform_data *pdata = dev->platform_data;
	struct device_node *np;
#if IS_ENABLED(CONFIG_SEC_ABC_HUB_COND)
	int cond_pin_cnt = 0;
#endif

	np = dev->of_node;
	pdata->nSub = of_get_child_count(np);
	if (!pdata->nSub) {
		dev_err(dev, "There is no dt of sub module\n");
		return -ENODEV;
	}

#if IS_ENABLED(CONFIG_SEC_ABC_HUB_COND)
	cond_pin_cnt = of_gpio_named_count(np, "sec,det_conn_gpios");
#if IS_ENABLED(CONFIG_QCOM_SEC_ABC_DETECT)
	/* Setting PM gpio for QC */
	cond_pin_cnt = cond_pin_cnt + of_gpio_named_count(np, "sec,det_pm_conn_gpios");
#endif
	if (cond_pin_cnt) {
		pdata->cond.init = 1;
	} else {
		pdata->cond.init = 0;
		ABC_PRINT("sub module(cond) is not supported\n");
	}
#endif
#if IS_ENABLED(CONFIG_SEC_ABC_HUB_BOOTC)
	if (parse_bootc_data(dev, pdata, np) < 0)
		ABC_PRINT("sub module(bootc) is not supported\n");
	else
		pdata->bootc_pdata.init = 1;
#endif
	return 0;
}
#endif

#if IS_ENABLED(CONFIG_OF)
static const struct of_device_id abc_hub_dt_match[] = {
	{ .compatible = "samsung,abc_hub" },
	{ }
};
#endif

static int abc_hub_suspend(struct device *dev)
{
	int ret = 0;
	return ret;
}

static int abc_hub_resume(struct device *dev)
{
	int ret = 0;
	return ret;
}

static int abc_hub_remove(struct platform_device *pdev)
{
	return 0;
}

static const struct dev_pm_ops abc_hub_pm = {
	.suspend = abc_hub_suspend,
	.resume = abc_hub_resume,
};

__visible_for_testing
ssize_t enable_store(struct device *dev,
										   struct device_attribute *attr,
										   const char *buf, size_t count)
{
	/* The enabel will be managed for each sub module when sub module becomes over two. */
	if (!strncmp(buf, "1", 1)) {
		ABC_PRINT("abc_hub driver enabled.\n");

		if (sec_abc_wait_enabled() < 0)
			ABC_PRINT("abc driver is not enabled\n");

		abc_hub_pinfo->enabled = ABC_HUB_ENABLED;
#if IS_ENABLED(CONFIG_SEC_ABC_HUB_COND) && !IS_ENABLED(CONFIG_SEC_FACTORY)
		if (abc_hub_pinfo->pdata->cond.init)
			abc_hub_cond_enable(dev, abc_hub_pinfo->enabled);
#endif
#if IS_ENABLED(CONFIG_SEC_ABC_HUB_BOOTC)
		if (abc_hub_pinfo->pdata->bootc_pdata.init)
			abc_hub_bootc_enable(dev, abc_hub_pinfo->enabled);
#endif
	} else if (!strncmp(buf, "0", 1)) {
		ABC_PRINT("abc_hub driver disabled.\n");
		abc_hub_pinfo->enabled = ABC_HUB_DISABLED;
#if IS_ENABLED(CONFIG_SEC_ABC_HUB_COND) && !IS_ENABLED(CONFIG_SEC_FACTORY)
		if (abc_hub_pinfo->pdata->cond.init)
			abc_hub_cond_enable(dev, abc_hub_pinfo->enabled);
#endif
#if IS_ENABLED(CONFIG_SEC_ABC_HUB_BOOTC)
		if (abc_hub_pinfo->pdata->bootc_pdata.init)
			abc_hub_bootc_enable(dev, abc_hub_pinfo->enabled);
#endif
	}
	return count;
}
EXPORT_SYMBOL_KUNIT(enable_store);

__visible_for_testing
ssize_t enable_show(struct device *dev,
										  struct device_attribute *attr,
										  char *buf)
{
	return sprintf(buf, "%d\n", abc_hub_pinfo->enabled);
}
EXPORT_SYMBOL_KUNIT(enable_show);

static DEVICE_ATTR_RW(enable);

#if IS_ENABLED(CONFIG_SEC_ABC_HUB_BOOTC)
__visible_for_testing
ssize_t bootc_offset_store(struct device *dev,
								struct device_attribute *attr,
								const char *buf, size_t count)
{
	char module[BOOTC_OFFSET_STR_MAX] = {0,};
	char *cur;
	char delim = ',';
	int offset = 0;
	int i = 0;

	if (strlen(buf) >= BOOTC_OFFSET_STR_MAX || count >= (unsigned int)BOOTC_OFFSET_STR_MAX) {
		ABC_PRINT("buf length is over(MAX:%d)!!\n", BOOTC_OFFSET_STR_MAX);
		return count;
	}

	cur = strchr(buf, (int)delim);
	if (cur) {
		memcpy(module, buf, cur - buf);
		if (kstrtoint(cur + 1, 0, &offset)) {
			ABC_PRINT("failed to get offest\n");
			return count;
		}
	} else {
		ABC_PRINT("failed to get module\n");
		return count;
	}

	ABC_PRINT("module(%s), offset(%d)\n", module, offset);

	if (offset >= 0) {
		for (i = 0; i < BOOTC_OFFSET_DATA_CNT; i++) {
			if (strcmp(module, abc_hub_pinfo->pdata->bootc_pdata.offset_data[i].module) == 0) {
				abc_hub_pinfo->pdata->bootc_pdata.offset_data[i].offset = offset;
				break;
			}
		}
	}

	return count;
}
EXPORT_SYMBOL_KUNIT(bootc_offset_store);

__visible_for_testing
ssize_t bootc_offset_show(struct device *dev,
						  struct device_attribute *attr,
						  char *buf)
{
	int res = 0;
	int i;

	for (i = 0; i < BOOTC_OFFSET_DATA_CNT; i++) {
		res += sprintf(buf, "%s,%d\n", abc_hub_pinfo->pdata->bootc_pdata.offset_data[i].module,
				abc_hub_pinfo->pdata->bootc_pdata.offset_data[i].offset);
	}

	return res;
}
EXPORT_SYMBOL_KUNIT(bootc_offset_show);

static DEVICE_ATTR_RW(bootc_offset);

__visible_for_testing
ssize_t bootc_time_store(struct device *dev,
						 struct device_attribute *attr,
						 const char *buf,
						 size_t count)
{
	if (kstrtoint(buf, 0, &(abc_hub_pinfo->pdata->bootc_pdata.bootc_time))) {
		ABC_PRINT("failed to get bootc_time\n");
		abc_hub_pinfo->pdata->bootc_pdata.bootc_time = -1;
		return count;
	}

	ABC_PRINT("bootc_time(%d)\n", abc_hub_pinfo->pdata->bootc_pdata.bootc_time);

	return count;
}
EXPORT_SYMBOL_KUNIT(bootc_time_store);

__visible_for_testing
ssize_t bootc_time_show(struct device *dev,
						struct device_attribute *attr,
						char *buf)
{
	int res = sprintf(buf, "%d\n", abc_hub_pinfo->pdata->bootc_pdata.bootc_time);

	return res;
}
EXPORT_SYMBOL_KUNIT(bootc_time_show);

static DEVICE_ATTR_RW(bootc_time);
#endif

int abc_hub_get_enabled(void)
{

	if (!abc_hub_probed)
		return 0;

	return abc_hub_pinfo->enabled;
}
EXPORT_SYMBOL(abc_hub_get_enabled);

/* event string format
 *
 * ex) MODULE=tsp@ERROR=power_status_mismatch
 *	 MODULE=tsp@ERROR=power_status_mismatch@EXT_LOG=fw_ver(0108)
 *
 */
void abc_hub_send_event(char *str)
{

	if (!abc_hub_probed) {
		ABC_PRINT_KUNIT("ABC Hub driver is not initialized!\n");
		return;
	}

	if (abc_hub_pinfo->enabled == ABC_HUB_DISABLED) {
		ABC_PRINT_KUNIT("ABC Hub is disabled!\n");
		return;
	}
#if IS_ENABLED(CONFIG_SEC_KUNIT)
	ABC_PRINT_KUNIT("%s", str);
	return;
#endif
	/* It just sends event to abc driver. The function will be added for gathering hw param big data. */
	sec_abc_send_event(str);
}
EXPORT_SYMBOL(abc_hub_send_event);

static int abc_hub_probe(struct platform_device *pdev)
{
	struct abc_hub_platform_data *pdata;
	int ret = 0;

	ABC_PRINT("start");

	abc_hub_probed = false;

	if (pdev->dev.of_node) {
		pdata = devm_kzalloc(&pdev->dev,
					 sizeof(struct abc_hub_platform_data), GFP_KERNEL);

		if (!pdata) {
			dev_err(&pdev->dev, "Failed to allocate platform data\n");
			ret = -ENOMEM;
			goto out;
		}

		pdev->dev.platform_data = pdata;
		ret = abc_hub_parse_dt(&pdev->dev);
		if (ret) {
			dev_err(&pdev->dev, "Failed to parse dt data\n");
			goto err_parse_dt;
		}

		ABC_PRINT("parse dt done\n");
	} else {
		pdata = pdev->dev.platform_data;
	}

	if (!pdata) {
		dev_err(&pdev->dev, "There are no platform data\n");
		ret = -EINVAL;
		goto out;
	}

	abc_hub_pinfo = kzalloc(sizeof(*abc_hub_pinfo), GFP_KERNEL);

	if (!abc_hub_pinfo) {
		ret = -ENOMEM;
		goto err_alloc_abc_hub_pinfo;
	}
#if IS_ENABLED(CONFIG_DRV_SAMSUNG)
	abc_hub_pinfo->dev = sec_device_create(abc_hub_pinfo, "sec_abc_hub");
#else
	abc_hub_pinfo->dev = device_create(sec_class, NULL, 0, NULL, "sec_abc_hub");
#endif
	if (IS_ERR(abc_hub_pinfo->dev)) {
		ABC_PRINT("Failed to create device(sec_abc_hub)!\n");
		ret = -ENODEV;
		goto err_create_device;
	}
	abc_hub_dev = abc_hub_pinfo->dev;

	ret = device_create_file(abc_hub_pinfo->dev, &dev_attr_enable);
	if (ret) {
		ABC_PRINT("Failed to create device enabled file\n");
		ret = -ENODEV;
		goto err_create_abc_hub_enable_sysfs;
	}

#if IS_ENABLED(CONFIG_SEC_ABC_HUB_BOOTC)
	ret = device_create_file(abc_hub_pinfo->dev, &dev_attr_bootc_offset);
	if (ret) {
		ABC_PRINT("Failed to create device bootc_offset file\n");
		ret = -ENODEV;
		goto err_create_abc_hub_bootc_offset_sysfs;
	}

	ret = device_create_file(abc_hub_pinfo->dev, &dev_attr_bootc_time);
	if (ret) {
		ABC_PRINT("Failed to create device bootc_time file\n");
		ret = -ENODEV;
		goto err_create_abc_hub_bootc_time_sysfs;
	}
#endif
	abc_hub_pinfo->pdata = pdata;
	platform_set_drvdata(pdev, abc_hub_pinfo);

#if IS_ENABLED(CONFIG_SEC_ABC_HUB_BOOTC)
	if (pdata->bootc_pdata.init) {
		if (abc_hub_bootc_init(abc_hub_dev) < 0) {
			dev_err(&pdev->dev, "abc_hub_booc_init fail\n");
			pdata->bootc_pdata.init = 0;
		}
	}
#endif

	abc_hub_probed = true;
	ABC_PRINT("Success");
	return ret;

#if IS_ENABLED(CONFIG_SEC_ABC_HUB_BOOTC)
err_create_abc_hub_bootc_time_sysfs:
	device_remove_file(abc_hub_pinfo->dev, &dev_attr_bootc_offset);
err_create_abc_hub_bootc_offset_sysfs:
	device_remove_file(abc_hub_pinfo->dev, &dev_attr_enable);
#endif
err_create_abc_hub_enable_sysfs:
#if IS_ENABLED(CONFIG_DRV_SAMSUNG)
	sec_device_destroy(abc_hub_dev->devt);
#else
	device_destroy(sec_class, abc_hub_dev->devt);
#endif
err_create_device:
	kfree(abc_hub_pinfo);
err_alloc_abc_hub_pinfo:
err_parse_dt:
	devm_kfree(&pdev->dev, pdata);
	pdev->dev.platform_data =  NULL;
out:
	return ret;
}

static struct platform_driver abc_hub_driver = {
	.probe = abc_hub_probe,
	.remove = abc_hub_remove,
	.driver = {
		.name = "abc_hub",
		.owner = THIS_MODULE,
#if IS_ENABLED(CONFIG_PM)
		.pm	= &abc_hub_pm,
#endif
#if IS_ENABLED(CONFIG_OF)
		.of_match_table = of_match_ptr(abc_hub_dt_match),
#endif
	},
};

static int __init abc_hub_init(void)
{
	ABC_PRINT("init");

	return platform_driver_register(&abc_hub_driver);
}

static void __exit abc_hub_exit(void)
{
	return platform_driver_unregister(&abc_hub_driver);
}

module_init(abc_hub_init);
module_exit(abc_hub_exit);

MODULE_DESCRIPTION("Samsung ABC Hub Driver");
MODULE_AUTHOR("Samsung Electronics");
MODULE_LICENSE("GPL");
=======
/* abc_hub.c
 *
 * Abnormal Behavior Catcher Hub Driver
 *
 * Copyright (C) 2017 Samsung Electronics
 *
 * Sangsu Ha <sangsu.ha@samsung.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#include <linux/sti/abc_hub.h>
#if IS_ENABLED(CONFIG_SEC_KUNIT)
#include <linux/sti/abc_kunit.h>
#endif

#if !IS_ENABLED(CONFIG_DRV_SAMSUNG)
static atomic_t sec_dev;
#endif
__visible_for_testing
struct device *abc_hub_dev;
__visible_for_testing
int abc_hub_probed;
#if IS_ENABLED(CONFIG_SEC_KUNIT)
void abc_hub_test_get_uevent_str(char *uevent_str);

void abc_hub_test_get_uevent_str(char *uevent_str)
{
	strlcpy(abc_hub_kunit_test_uevent_str, uevent_str, sizeof(abc_hub_kunit_test_uevent_str));
}
#endif

#if IS_ENABLED(CONFIG_OF)
static int abc_hub_parse_dt(struct device *dev)
{
	struct abc_hub_platform_data *pdata = dev->platform_data;
	struct device_node *np;
#if IS_ENABLED(CONFIG_SEC_ABC_HUB_COND)
	int cond_pin_cnt = 0;
#endif

	np = dev->of_node;
	pdata->nSub = of_get_child_count(np);
	if (!pdata->nSub) {
		dev_err(dev, "There is no dt of sub module\n");
		return -ENODEV;
	}

#if IS_ENABLED(CONFIG_SEC_ABC_HUB_COND)
	cond_pin_cnt = of_gpio_named_count(np, "sec,det_conn_gpios");
#if IS_ENABLED(CONFIG_QCOM_SEC_ABC_DETECT)
	/* Setting PM gpio for QC */
	cond_pin_cnt = cond_pin_cnt + of_gpio_named_count(np, "sec,det_pm_conn_gpios");
#endif
	if (cond_pin_cnt) {
		pdata->cond.init = 1;
	} else {
		pdata->cond.init = 0;
		dev_info(dev, "sub module(cond) is not supported\n");
	}
#endif
#if IS_ENABLED(CONFIG_SEC_ABC_HUB_BOOTC)
	if (parse_bootc_data(dev, pdata, np) < 0)
		dev_info(dev, "sub module(bootc) is not supported\n");
	else
		pdata->bootc_pdata.init = 1;
#endif
	return 0;
}
#endif

#if IS_ENABLED(CONFIG_OF)
static const struct of_device_id abc_hub_dt_match[] = {
	{ .compatible = "samsung,abc_hub" },
	{ }
};
#endif

static int abc_hub_suspend(struct device *dev)
{
	int ret = 0;
	return ret;
}

static int abc_hub_resume(struct device *dev)
{
	int ret = 0;
	return ret;
}

static int abc_hub_remove(struct platform_device *pdev)
{
	return 0;
}

static const struct dev_pm_ops abc_hub_pm = {
	.suspend = abc_hub_suspend,
	.resume = abc_hub_resume,
};

__visible_for_testing
ssize_t enable_store(struct device *dev,
										   struct device_attribute *attr,
										   const char *buf, size_t count)
{
	struct abc_hub_info *pinfo = dev_get_drvdata(dev);

	/* The enabel will be managed for each sub module when sub module becomes over two. */
	if (!strncmp(buf, "1", 1)) {
		dev_info(dev, "abc_hub driver enabled.\n");

		if (sec_abc_wait_enabled() < 0)
			dev_err(dev, "abc driver is not enabled\n");

		pinfo->enabled = ABC_HUB_ENABLED;
#if IS_ENABLED(CONFIG_SEC_ABC_HUB_COND) && !IS_ENABLED(CONFIG_SEC_FACTORY)
		if (pinfo->pdata->cond.init)
			abc_hub_cond_enable(dev, pinfo->enabled);
#endif
#if IS_ENABLED(CONFIG_SEC_ABC_HUB_BOOTC)
		if (pinfo->pdata->bootc_pdata.init)
			abc_hub_bootc_enable(dev, pinfo->enabled);
#endif
	} else if (!strncmp(buf, "0", 1)) {
		dev_info(dev, "abc_hub driver disabled.\n");
		pinfo->enabled = ABC_HUB_DISABLED;
#if IS_ENABLED(CONFIG_SEC_ABC_HUB_COND) && !IS_ENABLED(CONFIG_SEC_FACTORY)
		if (pinfo->pdata->cond.init)
			abc_hub_cond_enable(dev, pinfo->enabled);
#endif
#if IS_ENABLED(CONFIG_SEC_ABC_HUB_BOOTC)
		if (pinfo->pdata->bootc_pdata.init)
			abc_hub_bootc_enable(dev, pinfo->enabled);
#endif
	}
	return count;
}

__visible_for_testing
ssize_t enable_show(struct device *dev,
										  struct device_attribute *attr,
										  char *buf)
{
	struct abc_hub_info *pinfo = dev_get_drvdata(dev);

	return sprintf(buf, "%d\n", pinfo->enabled);
}

static DEVICE_ATTR_RW(enable);

#if IS_ENABLED(CONFIG_SEC_ABC_HUB_BOOTC)
__visible_for_testing
ssize_t bootc_offset_store(struct device *dev,
								struct device_attribute *attr,
								const char *buf, size_t count)
{
	struct abc_hub_info *pinfo = dev_get_drvdata(dev);
	char module[BOOTC_OFFSET_STR_MAX] = {0,};
	char *cur;
	char delim = ',';
	int offset = 0;
	int i = 0;

	if (strlen(buf) >= BOOTC_OFFSET_STR_MAX || count >= (unsigned int)BOOTC_OFFSET_STR_MAX) {
		pr_err("%s: buf length is over(MAX:%d)!!\n", __func__, BOOTC_OFFSET_STR_MAX);
		return count;
	}

	cur = strchr(buf, (int)delim);
	if (cur) {
		memcpy(module, buf, cur - buf);
		if (kstrtoint(cur + 1, 0, &offset)) {
			pr_err("%s: failed to get offest\n", __func__);
			return count;
		}
	} else {
		pr_err("%s: failed to get module\n", __func__);
		return count;
	}

	pr_info("%s: module(%s), offset(%d)\n", __func__, module, offset);

	if (offset >= 0) {
		for (i = 0; i < BOOTC_OFFSET_DATA_CNT; i++) {
			if (strcmp(module, pinfo->pdata->bootc_pdata.offset_data[i].module) == 0) {
				pinfo->pdata->bootc_pdata.offset_data[i].offset = offset;
				break;
			}
		}
	}

	return count;
}

__visible_for_testing
ssize_t bootc_offset_show(struct device *dev,
						  struct device_attribute *attr,
						  char *buf)
{
	struct abc_hub_info *pinfo = dev_get_drvdata(dev);
	int res = 0;
	int i;

	for (i = 0; i < BOOTC_OFFSET_DATA_CNT; i++) {
		res += sprintf(buf, "%s,%d\n", pinfo->pdata->bootc_pdata.offset_data[i].module,
				pinfo->pdata->bootc_pdata.offset_data[i].offset);
	}

	return res;
}

static DEVICE_ATTR_RW(bootc_offset);

__visible_for_testing
ssize_t bootc_time_store(struct device *dev,
						 struct device_attribute *attr,
						 const char *buf,
						 size_t count)
{
	struct abc_hub_info *pinfo = dev_get_drvdata(dev);

	if (kstrtoint(buf, 0, &(pinfo->pdata->bootc_pdata.bootc_time))) {
		pr_err("%s: failed to get bootc_time\n", __func__);
		pinfo->pdata->bootc_pdata.bootc_time = -1;
		return count;
	}

	pr_info("%s: bootc_time(%d)\n", __func__, pinfo->pdata->bootc_pdata.bootc_time);

	return count;
}

__visible_for_testing
ssize_t bootc_time_show(struct device *dev,
						struct device_attribute *attr,
						char *buf)
{
	struct abc_hub_info *pinfo = dev_get_drvdata(dev);

	int res = sprintf(buf, "%d\n", pinfo->pdata->bootc_pdata.bootc_time);

	return res;
}

static DEVICE_ATTR_RW(bootc_time);
#endif

int abc_hub_get_enabled(void)
{
	struct abc_hub_info *pinfo;

	if (!abc_hub_probed)
		return 0;

	pinfo = dev_get_drvdata(abc_hub_dev);

	return pinfo->enabled;
}
EXPORT_SYMBOL(abc_hub_get_enabled);

/* event string format
 *
 * ex) MODULE=tsp@ERROR=power_status_mismatch
 *	 MODULE=tsp@ERROR=power_status_mismatch@EXT_LOG=fw_ver(0108)
 *
 */
void abc_hub_send_event(char *str)
{
	struct abc_hub_info *pinfo;

	if (!abc_hub_probed) {
		dev_err(abc_hub_dev, "ABC Hub driver is not initialized!\n");
#if IS_ENABLED(CONFIG_SEC_KUNIT)
		abc_hub_test_get_uevent_str("ABC Hub driver is not initialized!\n");
#endif
		return;
	}

	pinfo = dev_get_drvdata(abc_hub_dev);

	if (pinfo->enabled == ABC_HUB_DISABLED) {
		dev_info(abc_hub_dev, "ABC Hub is disabled!\n");
#if IS_ENABLED(CONFIG_SEC_KUNIT)
		abc_hub_test_get_uevent_str("ABC Hub is disabled!\n");
#endif
		return;
	}
#if IS_ENABLED(CONFIG_SEC_KUNIT)
	abc_hub_test_get_uevent_str(str);
#endif
	/* It just sends event to abc driver. The function will be added for gathering hw param big data. */
	sec_abc_send_event(str);
}
EXPORT_SYMBOL(abc_hub_send_event);

static int abc_hub_probe(struct platform_device *pdev)
{
	struct abc_hub_platform_data *pdata;
	struct abc_hub_info *pinfo;
	int ret = 0;

	dev_info(&pdev->dev, "%s\n", __func__);

	abc_hub_probed = false;

	if (pdev->dev.of_node) {
		pdata = devm_kzalloc(&pdev->dev,
					 sizeof(struct abc_hub_platform_data), GFP_KERNEL);

		if (!pdata) {
			dev_err(&pdev->dev, "Failed to allocate platform data\n");
			ret = -ENOMEM;
			goto out;
		}

		pdev->dev.platform_data = pdata;
		ret = abc_hub_parse_dt(&pdev->dev);
		if (ret) {
			dev_err(&pdev->dev, "Failed to parse dt data\n");
			goto err_parse_dt;
		}

		pr_info("%s: parse dt done\n", __func__);
	} else {
		pdata = pdev->dev.platform_data;
	}

	if (!pdata) {
		dev_err(&pdev->dev, "There are no platform data\n");
		ret = -EINVAL;
		goto out;
	}

	pinfo = kzalloc(sizeof(*pinfo), GFP_KERNEL);

	if (!pinfo) {
		ret = -ENOMEM;
		goto err_alloc_pinfo;
	}
#if IS_ENABLED(CONFIG_DRV_SAMSUNG)
	pinfo->dev = sec_device_create(pinfo, "sec_abc_hub");
#else
	pinfo->dev = device_create(sec_class, NULL, atomic_inc_return(&sec_dev), pinfo, "sec_abc_hub");
	pr_info("%s devt %d\n", __func__, pinfo->dev->devt);
#endif
	if (IS_ERR(pinfo->dev)) {
		pr_err("%s Failed to create device(sec_abc_hub)!\n", __func__);
		ret = -ENODEV;
		goto err_create_device;
	}
	abc_hub_dev = pinfo->dev;

	ret = device_create_file(pinfo->dev, &dev_attr_enable);
	if (ret) {
		pr_err("%s: Failed to create device enabled file\n", __func__);
		ret = -ENODEV;
		goto err_create_abc_hub_enable_sysfs;
	}

#if IS_ENABLED(CONFIG_SEC_ABC_HUB_BOOTC)
	ret = device_create_file(pinfo->dev, &dev_attr_bootc_offset);
	if (ret) {
		pr_err("%s: Failed to create device bootc_offset file\n", __func__);
		ret = -ENODEV;
		goto err_create_abc_hub_bootc_offset_sysfs;
	}

	ret = device_create_file(pinfo->dev, &dev_attr_bootc_time);
	if (ret) {
		pr_err("%s: Failed to create device bootc_time file\n", __func__);
		ret = -ENODEV;
		goto err_create_abc_hub_bootc_time_sysfs;
	}
#endif
	pinfo->pdata = pdata;
	platform_set_drvdata(pdev, pinfo);

#if IS_ENABLED(CONFIG_SEC_ABC_HUB_BOOTC)
	if (pdata->bootc_pdata.init) {
		if (abc_hub_bootc_init(abc_hub_dev) < 0) {
			dev_err(&pdev->dev, "abc_hub_booc_init fail\n");
			pdata->bootc_pdata.init = 0;
		}
	}
#endif

	abc_hub_probed = true;
	dev_info(&pdev->dev, "%s success!\n", __func__);
	return ret;

#if IS_ENABLED(CONFIG_SEC_ABC_HUB_BOOTC)
err_create_abc_hub_bootc_time_sysfs:
	device_remove_file(pinfo->dev, &dev_attr_bootc_offset);
err_create_abc_hub_bootc_offset_sysfs:
	device_remove_file(pinfo->dev, &dev_attr_enable);
#endif
err_create_abc_hub_enable_sysfs:
#if IS_ENABLED(CONFIG_DRV_SAMSUNG)
	sec_device_destroy(abc_hub_dev->devt);
#else
	device_destroy(sec_class, abc_hub_dev->devt);
#endif
err_create_device:
	kfree(pinfo);
err_alloc_pinfo:
err_parse_dt:
	devm_kfree(&pdev->dev, pdata);
	pdev->dev.platform_data =  NULL;
out:
	return ret;
}

static struct platform_driver abc_hub_driver = {
	.probe = abc_hub_probe,
	.remove = abc_hub_remove,
	.driver = {
		.name = "abc_hub",
		.owner = THIS_MODULE,
#if IS_ENABLED(CONFIG_PM)
		.pm	= &abc_hub_pm,
#endif
#if IS_ENABLED(CONFIG_OF)
		.of_match_table = of_match_ptr(abc_hub_dt_match),
#endif
	},
};

static int __init abc_hub_init(void)
{
	pr_info("%s\n", __func__);

	return platform_driver_register(&abc_hub_driver);
}

static void __exit abc_hub_exit(void)
{
	return platform_driver_unregister(&abc_hub_driver);
}

module_init(abc_hub_init);
module_exit(abc_hub_exit);

MODULE_DESCRIPTION("Samsung ABC Hub Driver");
MODULE_AUTHOR("Samsung Electronics");
MODULE_LICENSE("GPL");
>>>>>>> upstream/android-13
