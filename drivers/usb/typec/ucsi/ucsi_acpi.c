// SPDX-License-Identifier: GPL-2.0
/*
 * UCSI ACPI driver
 *
 * Copyright (C) 2017, Intel Corporation
 * Author: Heikki Krogerus <heikki.krogerus@linux.intel.com>
 */

#include <linux/platform_device.h>
#include <linux/module.h>
#include <linux/acpi.h>

#include "ucsi.h"

#define UCSI_DSM_UUID		"6f8398c2-7ca4-11e4-ad36-631042b5008f"
#define UCSI_DSM_FUNC_WRITE	1
#define UCSI_DSM_FUNC_READ	2

struct ucsi_acpi {
	struct device *dev;
	struct ucsi *ucsi;
<<<<<<< HEAD
	struct ucsi_ppm ppm;
=======
	void __iomem *base;
	struct completion complete;
	unsigned long flags;
>>>>>>> upstream/android-13
	guid_t guid;
};

static int ucsi_acpi_dsm(struct ucsi_acpi *ua, int func)
{
	union acpi_object *obj;

	obj = acpi_evaluate_dsm(ACPI_HANDLE(ua->dev), &ua->guid, 1, func,
				NULL);
	if (!obj) {
		dev_err(ua->dev, "%s: failed to evaluate _DSM %d\n",
			__func__, func);
		return -EIO;
	}

	ACPI_FREE(obj);
	return 0;
}

<<<<<<< HEAD
static int ucsi_acpi_cmd(struct ucsi_ppm *ppm, struct ucsi_control *ctrl)
{
	struct ucsi_acpi *ua = container_of(ppm, struct ucsi_acpi, ppm);

	ppm->data->ctrl.raw_cmd = ctrl->raw_cmd;
=======
static int ucsi_acpi_read(struct ucsi *ucsi, unsigned int offset,
			  void *val, size_t val_len)
{
	struct ucsi_acpi *ua = ucsi_get_drvdata(ucsi);
	int ret;

	ret = ucsi_acpi_dsm(ua, UCSI_DSM_FUNC_READ);
	if (ret)
		return ret;

	memcpy(val, (const void __force *)(ua->base + offset), val_len);

	return 0;
}

static int ucsi_acpi_async_write(struct ucsi *ucsi, unsigned int offset,
				 const void *val, size_t val_len)
{
	struct ucsi_acpi *ua = ucsi_get_drvdata(ucsi);

	memcpy((void __force *)(ua->base + offset), val, val_len);
>>>>>>> upstream/android-13

	return ucsi_acpi_dsm(ua, UCSI_DSM_FUNC_WRITE);
}

<<<<<<< HEAD
static int ucsi_acpi_sync(struct ucsi_ppm *ppm)
{
	struct ucsi_acpi *ua = container_of(ppm, struct ucsi_acpi, ppm);

	return ucsi_acpi_dsm(ua, UCSI_DSM_FUNC_READ);
}

static void ucsi_acpi_notify(acpi_handle handle, u32 event, void *data)
{
	struct ucsi_acpi *ua = data;

	ucsi_notify(ua->ucsi);
=======
static int ucsi_acpi_sync_write(struct ucsi *ucsi, unsigned int offset,
				const void *val, size_t val_len)
{
	struct ucsi_acpi *ua = ucsi_get_drvdata(ucsi);
	int ret;

	set_bit(COMMAND_PENDING, &ua->flags);

	ret = ucsi_acpi_async_write(ucsi, offset, val, val_len);
	if (ret)
		goto out_clear_bit;

	if (!wait_for_completion_timeout(&ua->complete, 60 * HZ))
		ret = -ETIMEDOUT;

out_clear_bit:
	clear_bit(COMMAND_PENDING, &ua->flags);

	return ret;
}

static const struct ucsi_operations ucsi_acpi_ops = {
	.read = ucsi_acpi_read,
	.sync_write = ucsi_acpi_sync_write,
	.async_write = ucsi_acpi_async_write
};

static void ucsi_acpi_notify(acpi_handle handle, u32 event, void *data)
{
	struct ucsi_acpi *ua = data;
	u32 cci;
	int ret;

	ret = ucsi_acpi_read(ua->ucsi, UCSI_CCI, &cci, sizeof(cci));
	if (ret)
		return;

	if (UCSI_CCI_CONNECTOR(cci))
		ucsi_connector_change(ua->ucsi, UCSI_CCI_CONNECTOR(cci));

	if (test_bit(COMMAND_PENDING, &ua->flags) &&
	    cci & (UCSI_CCI_ACK_COMPLETE | UCSI_CCI_COMMAND_COMPLETE))
		complete(&ua->complete);
>>>>>>> upstream/android-13
}

static int ucsi_acpi_probe(struct platform_device *pdev)
{
	struct acpi_device *adev = ACPI_COMPANION(&pdev->dev);
	struct ucsi_acpi *ua;
	struct resource *res;
	acpi_status status;
	int ret;

	if (adev->dep_unmet)
		return -EPROBE_DEFER;

	ua = devm_kzalloc(&pdev->dev, sizeof(*ua), GFP_KERNEL);
	if (!ua)
		return -ENOMEM;

	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (!res) {
		dev_err(&pdev->dev, "missing memory resource\n");
		return -ENODEV;
	}

<<<<<<< HEAD
	/* This will make sure we can use ioremap_nocache() */
=======
	/* This will make sure we can use ioremap() */
>>>>>>> upstream/android-13
	status = acpi_release_memory(ACPI_HANDLE(&pdev->dev), res, 1);
	if (ACPI_FAILURE(status))
		return -ENOMEM;

	/*
	 * NOTE: The memory region for the data structures is used also in an
	 * operation region, which means ACPI has already reserved it. Therefore
	 * it can not be requested here, and we can not use
	 * devm_ioremap_resource().
	 */
<<<<<<< HEAD
	ua->ppm.data = devm_ioremap(&pdev->dev, res->start, resource_size(res));
	if (!ua->ppm.data)
		return -ENOMEM;

	if (!ua->ppm.data->version)
		return -ENODEV;

=======
	ua->base = devm_ioremap(&pdev->dev, res->start, resource_size(res));
	if (!ua->base)
		return -ENOMEM;

>>>>>>> upstream/android-13
	ret = guid_parse(UCSI_DSM_UUID, &ua->guid);
	if (ret)
		return ret;

<<<<<<< HEAD
	ua->ppm.cmd = ucsi_acpi_cmd;
	ua->ppm.sync = ucsi_acpi_sync;
	ua->dev = &pdev->dev;

=======
	init_completion(&ua->complete);
	ua->dev = &pdev->dev;

	ua->ucsi = ucsi_create(&pdev->dev, &ucsi_acpi_ops);
	if (IS_ERR(ua->ucsi))
		return PTR_ERR(ua->ucsi);

	ucsi_set_drvdata(ua->ucsi, ua);

>>>>>>> upstream/android-13
	status = acpi_install_notify_handler(ACPI_HANDLE(&pdev->dev),
					     ACPI_DEVICE_NOTIFY,
					     ucsi_acpi_notify, ua);
	if (ACPI_FAILURE(status)) {
		dev_err(&pdev->dev, "failed to install notify handler\n");
<<<<<<< HEAD
		return -ENODEV;
	}

	ua->ucsi = ucsi_register_ppm(&pdev->dev, &ua->ppm);
	if (IS_ERR(ua->ucsi)) {
		acpi_remove_notify_handler(ACPI_HANDLE(&pdev->dev),
					   ACPI_DEVICE_NOTIFY,
					   ucsi_acpi_notify);
		return PTR_ERR(ua->ucsi);
=======
		ucsi_destroy(ua->ucsi);
		return -ENODEV;
	}

	ret = ucsi_register(ua->ucsi);
	if (ret) {
		acpi_remove_notify_handler(ACPI_HANDLE(&pdev->dev),
					   ACPI_DEVICE_NOTIFY,
					   ucsi_acpi_notify);
		ucsi_destroy(ua->ucsi);
		return ret;
>>>>>>> upstream/android-13
	}

	platform_set_drvdata(pdev, ua);

	return 0;
}

static int ucsi_acpi_remove(struct platform_device *pdev)
{
	struct ucsi_acpi *ua = platform_get_drvdata(pdev);

<<<<<<< HEAD
	ucsi_unregister_ppm(ua->ucsi);
=======
	ucsi_unregister(ua->ucsi);
	ucsi_destroy(ua->ucsi);
>>>>>>> upstream/android-13

	acpi_remove_notify_handler(ACPI_HANDLE(&pdev->dev), ACPI_DEVICE_NOTIFY,
				   ucsi_acpi_notify);

	return 0;
}

static const struct acpi_device_id ucsi_acpi_match[] = {
	{ "PNP0CA0", 0 },
	{ },
};
MODULE_DEVICE_TABLE(acpi, ucsi_acpi_match);

static struct platform_driver ucsi_acpi_platform_driver = {
	.driver = {
		.name = "ucsi_acpi",
		.acpi_match_table = ACPI_PTR(ucsi_acpi_match),
	},
	.probe = ucsi_acpi_probe,
	.remove = ucsi_acpi_remove,
};

module_platform_driver(ucsi_acpi_platform_driver);

MODULE_AUTHOR("Heikki Krogerus <heikki.krogerus@linux.intel.com>");
MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("UCSI ACPI driver");
