<<<<<<< HEAD
/*
 * Copyright (C) 2016 Texas Instruments
 * Author: Jyri Sarha <jsarha@ti.com>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published by
 * the Free Software Foundation.
 *
 */

#include <linux/delay.h>
#include <linux/fwnode.h>
#include <linux/gpio/consumer.h>
#include <linux/irq.h>
#include <linux/module.h>
#include <linux/of_graph.h>
#include <linux/platform_device.h>
#include <linux/i2c.h>

#include <drm/drmP.h>
#include <drm/drm_atomic_helper.h>
#include <drm/drm_crtc.h>
#include <drm/drm_crtc_helper.h>
=======
// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright (C) 2016 Texas Instruments
 * Author: Jyri Sarha <jsarha@ti.com>
 */

#include <linux/gpio/consumer.h>
#include <linux/i2c.h>
#include <linux/module.h>
#include <linux/of_graph.h>
#include <linux/platform_device.h>
#include <linux/workqueue.h>

#include <drm/drm_atomic_helper.h>
#include <drm/drm_bridge.h>
#include <drm/drm_crtc.h>
#include <drm/drm_print.h>
#include <drm/drm_probe_helper.h>
>>>>>>> upstream/android-13

#define HOTPLUG_DEBOUNCE_MS		1100

struct tfp410 {
	struct drm_bridge	bridge;
	struct drm_connector	connector;

<<<<<<< HEAD
	struct i2c_adapter	*ddc;
	struct gpio_desc	*hpd;
	struct delayed_work	hpd_work;
=======
	u32			bus_format;
	struct delayed_work	hpd_work;
	struct gpio_desc	*powerdown;

	struct drm_bridge_timings timings;
	struct drm_bridge	*next_bridge;
>>>>>>> upstream/android-13

	struct device *dev;
};

static inline struct tfp410 *
drm_bridge_to_tfp410(struct drm_bridge *bridge)
{
	return container_of(bridge, struct tfp410, bridge);
}

static inline struct tfp410 *
drm_connector_to_tfp410(struct drm_connector *connector)
{
	return container_of(connector, struct tfp410, connector);
}

static int tfp410_get_modes(struct drm_connector *connector)
{
	struct tfp410 *dvi = drm_connector_to_tfp410(connector);
	struct edid *edid;
	int ret;

<<<<<<< HEAD
	if (!dvi->ddc)
		goto fallback;

	edid = drm_get_edid(connector, dvi->ddc);
	if (!edid) {
		DRM_INFO("EDID read failed. Fallback to standard modes\n");
		goto fallback;
=======
	if (dvi->next_bridge->ops & DRM_BRIDGE_OP_EDID) {
		edid = drm_bridge_get_edid(dvi->next_bridge, connector);
		if (!edid)
			DRM_INFO("EDID read failed. Fallback to standard modes\n");
	} else {
		edid = NULL;
	}

	if (!edid) {
		/*
		 * No EDID, fallback on the XGA standard modes and prefer a mode
		 * pretty much anything can handle.
		 */
		ret = drm_add_modes_noedid(connector, 1920, 1200);
		drm_set_preferred_mode(connector, 1024, 768);
		return ret;
>>>>>>> upstream/android-13
	}

	drm_connector_update_edid_property(connector, edid);

	ret = drm_add_edid_modes(connector, edid);

	kfree(edid);

	return ret;
<<<<<<< HEAD

fallback:
	/* No EDID, fallback on the XGA standard modes */
	ret = drm_add_modes_noedid(connector, 1920, 1200);

	/* And prefer a mode pretty much anything can handle */
	drm_set_preferred_mode(connector, 1024, 768);

	return ret;
=======
>>>>>>> upstream/android-13
}

static const struct drm_connector_helper_funcs tfp410_con_helper_funcs = {
	.get_modes	= tfp410_get_modes,
};

static enum drm_connector_status
tfp410_connector_detect(struct drm_connector *connector, bool force)
{
	struct tfp410 *dvi = drm_connector_to_tfp410(connector);

<<<<<<< HEAD
	if (dvi->hpd) {
		if (gpiod_get_value_cansleep(dvi->hpd))
			return connector_status_connected;
		else
			return connector_status_disconnected;
	}

	if (dvi->ddc) {
		if (drm_probe_ddc(dvi->ddc))
			return connector_status_connected;
		else
			return connector_status_disconnected;
	}

	return connector_status_unknown;
=======
	return drm_bridge_detect(dvi->next_bridge);
>>>>>>> upstream/android-13
}

static const struct drm_connector_funcs tfp410_con_funcs = {
	.detect			= tfp410_connector_detect,
	.fill_modes		= drm_helper_probe_single_connector_modes,
	.destroy		= drm_connector_cleanup,
	.reset			= drm_atomic_helper_connector_reset,
	.atomic_duplicate_state	= drm_atomic_helper_connector_duplicate_state,
	.atomic_destroy_state	= drm_atomic_helper_connector_destroy_state,
};

<<<<<<< HEAD
static int tfp410_attach(struct drm_bridge *bridge)
{
	struct tfp410 *dvi = drm_bridge_to_tfp410(bridge);
	int ret;

	if (!bridge->encoder) {
		dev_err(dvi->dev, "Missing encoder\n");
		return -ENODEV;
	}

	if (dvi->hpd)
		dvi->connector.polled = DRM_CONNECTOR_POLL_HPD;

	drm_connector_helper_add(&dvi->connector,
				 &tfp410_con_helper_funcs);
	ret = drm_connector_init(bridge->dev, &dvi->connector,
				 &tfp410_con_funcs, DRM_MODE_CONNECTOR_HDMIA);
	if (ret) {
		dev_err(dvi->dev, "drm_connector_init() failed: %d\n", ret);
		return ret;
	}

	drm_connector_attach_encoder(&dvi->connector,
					  bridge->encoder);

	return 0;
}

static const struct drm_bridge_funcs tfp410_bridge_funcs = {
	.attach		= tfp410_attach,
};

=======
>>>>>>> upstream/android-13
static void tfp410_hpd_work_func(struct work_struct *work)
{
	struct tfp410 *dvi;

	dvi = container_of(work, struct tfp410, hpd_work.work);

	if (dvi->bridge.dev)
		drm_helper_hpd_irq_event(dvi->bridge.dev);
}

<<<<<<< HEAD
static irqreturn_t tfp410_hpd_irq_thread(int irq, void *arg)
=======
static void tfp410_hpd_callback(void *arg, enum drm_connector_status status)
>>>>>>> upstream/android-13
{
	struct tfp410 *dvi = arg;

	mod_delayed_work(system_wq, &dvi->hpd_work,
<<<<<<< HEAD
			msecs_to_jiffies(HOTPLUG_DEBOUNCE_MS));

	return IRQ_HANDLED;
}

static int tfp410_get_connector_properties(struct tfp410 *dvi)
{
	struct device_node *connector_node, *ddc_phandle;
	int ret = 0;

	/* port@1 is the connector node */
	connector_node = of_graph_get_remote_node(dvi->dev->of_node, 1, -1);
	if (!connector_node)
		return -ENODEV;

	dvi->hpd = fwnode_get_named_gpiod(&connector_node->fwnode,
					"hpd-gpios", 0, GPIOD_IN, "hpd");
	if (IS_ERR(dvi->hpd)) {
		ret = PTR_ERR(dvi->hpd);
		dvi->hpd = NULL;
		if (ret == -ENOENT)
			ret = 0;
		else
			goto fail;
	}

	ddc_phandle = of_parse_phandle(connector_node, "ddc-i2c-bus", 0);
	if (!ddc_phandle)
		goto fail;

	dvi->ddc = of_get_i2c_adapter_by_node(ddc_phandle);
	if (dvi->ddc)
		dev_info(dvi->dev, "Connector's ddc i2c bus found\n");
	else
		ret = -EPROBE_DEFER;

	of_node_put(ddc_phandle);

fail:
	of_node_put(connector_node);
	return ret;
}

static int tfp410_init(struct device *dev)
{
=======
			 msecs_to_jiffies(HOTPLUG_DEBOUNCE_MS));
}

static int tfp410_attach(struct drm_bridge *bridge,
			 enum drm_bridge_attach_flags flags)
{
	struct tfp410 *dvi = drm_bridge_to_tfp410(bridge);
	int ret;

	ret = drm_bridge_attach(bridge->encoder, dvi->next_bridge, bridge,
				DRM_BRIDGE_ATTACH_NO_CONNECTOR);
	if (ret < 0)
		return ret;

	if (flags & DRM_BRIDGE_ATTACH_NO_CONNECTOR)
		return 0;

	if (!bridge->encoder) {
		dev_err(dvi->dev, "Missing encoder\n");
		return -ENODEV;
	}

	if (dvi->next_bridge->ops & DRM_BRIDGE_OP_DETECT)
		dvi->connector.polled = DRM_CONNECTOR_POLL_HPD;
	else
		dvi->connector.polled = DRM_CONNECTOR_POLL_CONNECT | DRM_CONNECTOR_POLL_DISCONNECT;

	if (dvi->next_bridge->ops & DRM_BRIDGE_OP_HPD) {
		INIT_DELAYED_WORK(&dvi->hpd_work, tfp410_hpd_work_func);
		drm_bridge_hpd_enable(dvi->next_bridge, tfp410_hpd_callback,
				      dvi);
	}

	drm_connector_helper_add(&dvi->connector,
				 &tfp410_con_helper_funcs);
	ret = drm_connector_init_with_ddc(bridge->dev, &dvi->connector,
					  &tfp410_con_funcs,
					  dvi->next_bridge->type,
					  dvi->next_bridge->ddc);
	if (ret) {
		dev_err(dvi->dev, "drm_connector_init_with_ddc() failed: %d\n",
			ret);
		return ret;
	}

	drm_display_info_set_bus_formats(&dvi->connector.display_info,
					 &dvi->bus_format, 1);

	drm_connector_attach_encoder(&dvi->connector, bridge->encoder);

	return 0;
}

static void tfp410_detach(struct drm_bridge *bridge)
{
	struct tfp410 *dvi = drm_bridge_to_tfp410(bridge);

	if (dvi->connector.dev && dvi->next_bridge->ops & DRM_BRIDGE_OP_HPD) {
		drm_bridge_hpd_disable(dvi->next_bridge);
		cancel_delayed_work_sync(&dvi->hpd_work);
	}
}

static void tfp410_enable(struct drm_bridge *bridge)
{
	struct tfp410 *dvi = drm_bridge_to_tfp410(bridge);

	gpiod_set_value_cansleep(dvi->powerdown, 0);
}

static void tfp410_disable(struct drm_bridge *bridge)
{
	struct tfp410 *dvi = drm_bridge_to_tfp410(bridge);

	gpiod_set_value_cansleep(dvi->powerdown, 1);
}

static enum drm_mode_status tfp410_mode_valid(struct drm_bridge *bridge,
					      const struct drm_display_info *info,
					      const struct drm_display_mode *mode)
{
	if (mode->clock < 25000)
		return MODE_CLOCK_LOW;

	if (mode->clock > 165000)
		return MODE_CLOCK_HIGH;

	return MODE_OK;
}

static const struct drm_bridge_funcs tfp410_bridge_funcs = {
	.attach		= tfp410_attach,
	.detach		= tfp410_detach,
	.enable		= tfp410_enable,
	.disable	= tfp410_disable,
	.mode_valid	= tfp410_mode_valid,
};

static const struct drm_bridge_timings tfp410_default_timings = {
	.input_bus_flags = DRM_BUS_FLAG_PIXDATA_SAMPLE_POSEDGE
			 | DRM_BUS_FLAG_DE_HIGH,
	.setup_time_ps = 1200,
	.hold_time_ps = 1300,
};

static int tfp410_parse_timings(struct tfp410 *dvi, bool i2c)
{
	struct drm_bridge_timings *timings = &dvi->timings;
	struct device_node *ep;
	u32 pclk_sample = 0;
	u32 bus_width = 24;
	u32 deskew = 0;

	/* Start with defaults. */
	*timings = tfp410_default_timings;

	if (i2c)
		/*
		 * In I2C mode timings are configured through the I2C interface.
		 * As the driver doesn't support I2C configuration yet, we just
		 * go with the defaults (BSEL=1, DSEL=1, DKEN=0, EDGE=1).
		 */
		return 0;

	/*
	 * In non-I2C mode, timings are configured through the BSEL, DSEL, DKEN
	 * and EDGE pins. They are specified in DT through endpoint properties
	 * and vendor-specific properties.
	 */
	ep = of_graph_get_endpoint_by_regs(dvi->dev->of_node, 0, 0);
	if (!ep)
		return -EINVAL;

	/* Get the sampling edge from the endpoint. */
	of_property_read_u32(ep, "pclk-sample", &pclk_sample);
	of_property_read_u32(ep, "bus-width", &bus_width);
	of_node_put(ep);

	timings->input_bus_flags = DRM_BUS_FLAG_DE_HIGH;

	switch (pclk_sample) {
	case 0:
		timings->input_bus_flags |= DRM_BUS_FLAG_PIXDATA_SAMPLE_NEGEDGE
					 |  DRM_BUS_FLAG_SYNC_SAMPLE_NEGEDGE;
		break;
	case 1:
		timings->input_bus_flags |= DRM_BUS_FLAG_PIXDATA_SAMPLE_POSEDGE
					 |  DRM_BUS_FLAG_SYNC_SAMPLE_POSEDGE;
		break;
	default:
		return -EINVAL;
	}

	switch (bus_width) {
	case 12:
		dvi->bus_format = MEDIA_BUS_FMT_RGB888_2X12_LE;
		break;
	case 24:
		dvi->bus_format = MEDIA_BUS_FMT_RGB888_1X24;
		break;
	default:
		return -EINVAL;
	}

	/* Get the setup and hold time from vendor-specific properties. */
	of_property_read_u32(dvi->dev->of_node, "ti,deskew", &deskew);
	if (deskew > 7)
		return -EINVAL;

	timings->setup_time_ps = 1200 - 350 * ((s32)deskew - 4);
	timings->hold_time_ps = max(0, 1300 + 350 * ((s32)deskew - 4));

	return 0;
}

static int tfp410_init(struct device *dev, bool i2c)
{
	struct device_node *node;
>>>>>>> upstream/android-13
	struct tfp410 *dvi;
	int ret;

	if (!dev->of_node) {
		dev_err(dev, "device-tree data is missing\n");
		return -ENXIO;
	}

	dvi = devm_kzalloc(dev, sizeof(*dvi), GFP_KERNEL);
	if (!dvi)
		return -ENOMEM;
<<<<<<< HEAD
=======

	dvi->dev = dev;
>>>>>>> upstream/android-13
	dev_set_drvdata(dev, dvi);

	dvi->bridge.funcs = &tfp410_bridge_funcs;
	dvi->bridge.of_node = dev->of_node;
<<<<<<< HEAD
	dvi->dev = dev;

	ret = tfp410_get_connector_properties(dvi);
	if (ret)
		goto fail;

	if (dvi->hpd) {
		INIT_DELAYED_WORK(&dvi->hpd_work, tfp410_hpd_work_func);

		ret = devm_request_threaded_irq(dev, gpiod_to_irq(dvi->hpd),
			NULL, tfp410_hpd_irq_thread, IRQF_TRIGGER_RISING |
			IRQF_TRIGGER_FALLING | IRQF_ONESHOT,
			"hdmi-hpd", dvi);
		if (ret) {
			DRM_ERROR("failed to register hpd interrupt\n");
			goto fail;
		}
	}

	drm_bridge_add(&dvi->bridge);

	return 0;
fail:
	i2c_put_adapter(dvi->ddc);
	if (dvi->hpd)
		gpiod_put(dvi->hpd);
	return ret;
=======
	dvi->bridge.timings = &dvi->timings;
	dvi->bridge.type = DRM_MODE_CONNECTOR_DVID;

	ret = tfp410_parse_timings(dvi, i2c);
	if (ret)
		return ret;

	/* Get the next bridge, connected to port@1. */
	node = of_graph_get_remote_node(dev->of_node, 1, -1);
	if (!node)
		return -ENODEV;

	dvi->next_bridge = of_drm_find_bridge(node);
	of_node_put(node);

	if (!dvi->next_bridge)
		return -EPROBE_DEFER;

	/* Get the powerdown GPIO. */
	dvi->powerdown = devm_gpiod_get_optional(dev, "powerdown",
						 GPIOD_OUT_HIGH);
	if (IS_ERR(dvi->powerdown)) {
		dev_err(dev, "failed to parse powerdown gpio\n");
		return PTR_ERR(dvi->powerdown);
	}

	/*  Register the DRM bridge. */
	drm_bridge_add(&dvi->bridge);

	return 0;
>>>>>>> upstream/android-13
}

static int tfp410_fini(struct device *dev)
{
	struct tfp410 *dvi = dev_get_drvdata(dev);

<<<<<<< HEAD
	cancel_delayed_work_sync(&dvi->hpd_work);

	drm_bridge_remove(&dvi->bridge);

	if (dvi->ddc)
		i2c_put_adapter(dvi->ddc);
	if (dvi->hpd)
		gpiod_put(dvi->hpd);

=======
	drm_bridge_remove(&dvi->bridge);

>>>>>>> upstream/android-13
	return 0;
}

static int tfp410_probe(struct platform_device *pdev)
{
<<<<<<< HEAD
	return tfp410_init(&pdev->dev);
=======
	return tfp410_init(&pdev->dev, false);
>>>>>>> upstream/android-13
}

static int tfp410_remove(struct platform_device *pdev)
{
	return tfp410_fini(&pdev->dev);
}

static const struct of_device_id tfp410_match[] = {
	{ .compatible = "ti,tfp410" },
	{},
};
MODULE_DEVICE_TABLE(of, tfp410_match);

static struct platform_driver tfp410_platform_driver = {
	.probe	= tfp410_probe,
	.remove	= tfp410_remove,
	.driver	= {
		.name		= "tfp410-bridge",
		.of_match_table	= tfp410_match,
	},
};

#if IS_ENABLED(CONFIG_I2C)
/* There is currently no i2c functionality. */
static int tfp410_i2c_probe(struct i2c_client *client,
			    const struct i2c_device_id *id)
{
	int reg;

	if (!client->dev.of_node ||
	    of_property_read_u32(client->dev.of_node, "reg", &reg)) {
		dev_err(&client->dev,
			"Can't get i2c reg property from device-tree\n");
		return -ENXIO;
	}

<<<<<<< HEAD
	return tfp410_init(&client->dev);
=======
	return tfp410_init(&client->dev, true);
>>>>>>> upstream/android-13
}

static int tfp410_i2c_remove(struct i2c_client *client)
{
	return tfp410_fini(&client->dev);
}

static const struct i2c_device_id tfp410_i2c_ids[] = {
	{ "tfp410", 0 },
	{ }
};
MODULE_DEVICE_TABLE(i2c, tfp410_i2c_ids);

static struct i2c_driver tfp410_i2c_driver = {
	.driver = {
		.name	= "tfp410",
		.of_match_table = of_match_ptr(tfp410_match),
	},
	.id_table	= tfp410_i2c_ids,
	.probe		= tfp410_i2c_probe,
	.remove		= tfp410_i2c_remove,
};
#endif /* IS_ENABLED(CONFIG_I2C) */

static struct {
	uint i2c:1;
	uint platform:1;
}  tfp410_registered_driver;

static int __init tfp410_module_init(void)
{
	int ret;

#if IS_ENABLED(CONFIG_I2C)
	ret = i2c_add_driver(&tfp410_i2c_driver);
	if (ret)
		pr_err("%s: registering i2c driver failed: %d",
		       __func__, ret);
	else
		tfp410_registered_driver.i2c = 1;
#endif

	ret = platform_driver_register(&tfp410_platform_driver);
	if (ret)
		pr_err("%s: registering platform driver failed: %d",
		       __func__, ret);
	else
		tfp410_registered_driver.platform = 1;

	if (tfp410_registered_driver.i2c ||
	    tfp410_registered_driver.platform)
		return 0;

	return ret;
}
module_init(tfp410_module_init);

static void __exit tfp410_module_exit(void)
{
#if IS_ENABLED(CONFIG_I2C)
	if (tfp410_registered_driver.i2c)
		i2c_del_driver(&tfp410_i2c_driver);
#endif
	if (tfp410_registered_driver.platform)
		platform_driver_unregister(&tfp410_platform_driver);
}
module_exit(tfp410_module_exit);

MODULE_AUTHOR("Jyri Sarha <jsarha@ti.com>");
MODULE_DESCRIPTION("TI TFP410 DVI bridge driver");
MODULE_LICENSE("GPL");
