<<<<<<< HEAD
/*
 * Copyright (C) 2012 Avionic Design GmbH
 * Copyright (C) 2012 NVIDIA CORPORATION.  All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <drm/drm_atomic_helper.h>
#include <drm/drm_panel.h>
=======
// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright (C) 2012 Avionic Design GmbH
 * Copyright (C) 2012 NVIDIA CORPORATION.  All rights reserved.
 */

#include <drm/drm_atomic_helper.h>
#include <drm/drm_of.h>
#include <drm/drm_panel.h>
#include <drm/drm_simple_kms_helper.h>
>>>>>>> upstream/android-13

#include "drm.h"
#include "dc.h"

#include <media/cec-notifier.h>

int tegra_output_connector_get_modes(struct drm_connector *connector)
{
	struct tegra_output *output = connector_to_output(connector);
	struct edid *edid = NULL;
	int err = 0;

	/*
	 * If the panel provides one or more modes, use them exclusively and
	 * ignore any other means of obtaining a mode.
	 */
	if (output->panel) {
<<<<<<< HEAD
		err = output->panel->funcs->get_modes(output->panel);
=======
		err = drm_panel_get_modes(output->panel, connector);
>>>>>>> upstream/android-13
		if (err > 0)
			return err;
	}

	if (output->edid)
		edid = kmemdup(output->edid, sizeof(*edid), GFP_KERNEL);
	else if (output->ddc)
		edid = drm_get_edid(connector, output->ddc);

<<<<<<< HEAD
	cec_notifier_set_phys_addr_from_edid(output->notifier, edid);
=======
	cec_notifier_set_phys_addr_from_edid(output->cec, edid);
>>>>>>> upstream/android-13
	drm_connector_update_edid_property(connector, edid);

	if (edid) {
		err = drm_add_edid_modes(connector, edid);
		kfree(edid);
	}

	return err;
}

enum drm_connector_status
tegra_output_connector_detect(struct drm_connector *connector, bool force)
{
	struct tegra_output *output = connector_to_output(connector);
	enum drm_connector_status status = connector_status_unknown;

<<<<<<< HEAD
	if (gpio_is_valid(output->hpd_gpio)) {
		if (output->hpd_gpio_flags & OF_GPIO_ACTIVE_LOW) {
			if (gpio_get_value(output->hpd_gpio) != 0)
				status = connector_status_disconnected;
			else
				status = connector_status_connected;
		} else {
			if (gpio_get_value(output->hpd_gpio) == 0)
				status = connector_status_disconnected;
			else
				status = connector_status_connected;
		}
=======
	if (output->hpd_gpio) {
		if (gpiod_get_value(output->hpd_gpio) == 0)
			status = connector_status_disconnected;
		else
			status = connector_status_connected;
>>>>>>> upstream/android-13
	} else {
		if (!output->panel)
			status = connector_status_disconnected;
		else
			status = connector_status_connected;
	}

	if (status != connector_status_connected)
<<<<<<< HEAD
		cec_notifier_phys_addr_invalidate(output->notifier);
=======
		cec_notifier_phys_addr_invalidate(output->cec);
>>>>>>> upstream/android-13

	return status;
}

void tegra_output_connector_destroy(struct drm_connector *connector)
{
<<<<<<< HEAD
=======
	struct tegra_output *output = connector_to_output(connector);

	if (output->cec)
		cec_notifier_conn_unregister(output->cec);

>>>>>>> upstream/android-13
	drm_connector_unregister(connector);
	drm_connector_cleanup(connector);
}

<<<<<<< HEAD
void tegra_output_encoder_destroy(struct drm_encoder *encoder)
{
	drm_encoder_cleanup(encoder);
}

=======
>>>>>>> upstream/android-13
static irqreturn_t hpd_irq(int irq, void *data)
{
	struct tegra_output *output = data;

	if (output->connector.dev)
		drm_helper_hpd_irq_event(output->connector.dev);

	return IRQ_HANDLED;
}

int tegra_output_probe(struct tegra_output *output)
{
	struct device_node *ddc, *panel;
<<<<<<< HEAD
=======
	unsigned long flags;
>>>>>>> upstream/android-13
	int err, size;

	if (!output->of_node)
		output->of_node = output->dev->of_node;

<<<<<<< HEAD
	panel = of_parse_phandle(output->of_node, "nvidia,panel", 0);
	if (panel) {
		output->panel = of_drm_find_panel(panel);
		if (IS_ERR(output->panel))
			return PTR_ERR(output->panel);

		of_node_put(panel);
=======
	err = drm_of_find_panel_or_bridge(output->of_node, -1, -1,
					  &output->panel, &output->bridge);
	if (err && err != -ENODEV)
		return err;

	panel = of_parse_phandle(output->of_node, "nvidia,panel", 0);
	if (panel) {
		/*
		 * Don't mix nvidia,panel phandle with the graph in a
		 * device-tree.
		 */
		WARN_ON(output->panel || output->bridge);

		output->panel = of_drm_find_panel(panel);
		of_node_put(panel);

		if (IS_ERR(output->panel))
			return PTR_ERR(output->panel);
>>>>>>> upstream/android-13
	}

	output->edid = of_get_property(output->of_node, "nvidia,edid", &size);

	ddc = of_parse_phandle(output->of_node, "nvidia,ddc-i2c-bus", 0);
	if (ddc) {
<<<<<<< HEAD
		output->ddc = of_find_i2c_adapter_by_node(ddc);
		if (!output->ddc) {
			err = -EPROBE_DEFER;
			of_node_put(ddc);
			return err;
		}

		of_node_put(ddc);
	}

	output->hpd_gpio = of_get_named_gpio_flags(output->of_node,
						   "nvidia,hpd-gpio", 0,
						   &output->hpd_gpio_flags);
	if (gpio_is_valid(output->hpd_gpio)) {
		unsigned long flags;

		err = gpio_request_one(output->hpd_gpio, GPIOF_DIR_IN,
				       "HDMI hotplug detect");
		if (err < 0) {
			dev_err(output->dev, "gpio_request_one(): %d\n", err);
			return err;
		}

		err = gpio_to_irq(output->hpd_gpio);
		if (err < 0) {
			dev_err(output->dev, "gpio_to_irq(): %d\n", err);
			gpio_free(output->hpd_gpio);
=======
		output->ddc = of_get_i2c_adapter_by_node(ddc);
		of_node_put(ddc);

		if (!output->ddc) {
			err = -EPROBE_DEFER;
			return err;
		}
	}

	output->hpd_gpio = devm_gpiod_get_from_of_node(output->dev,
						       output->of_node,
						       "nvidia,hpd-gpio", 0,
						       GPIOD_IN,
						       "HDMI hotplug detect");
	if (IS_ERR(output->hpd_gpio)) {
		if (PTR_ERR(output->hpd_gpio) != -ENOENT)
			return PTR_ERR(output->hpd_gpio);

		output->hpd_gpio = NULL;
	}

	if (output->hpd_gpio) {
		err = gpiod_to_irq(output->hpd_gpio);
		if (err < 0) {
			dev_err(output->dev, "gpiod_to_irq(): %d\n", err);
>>>>>>> upstream/android-13
			return err;
		}

		output->hpd_irq = err;

		flags = IRQF_TRIGGER_RISING | IRQF_TRIGGER_FALLING |
			IRQF_ONESHOT;

		err = request_threaded_irq(output->hpd_irq, NULL, hpd_irq,
					   flags, "hpd", output);
		if (err < 0) {
			dev_err(output->dev, "failed to request IRQ#%u: %d\n",
				output->hpd_irq, err);
<<<<<<< HEAD
			gpio_free(output->hpd_gpio);
=======
>>>>>>> upstream/android-13
			return err;
		}

		output->connector.polled = DRM_CONNECTOR_POLL_HPD;

		/*
		 * Disable the interrupt until the connector has been
		 * initialized to avoid a race in the hotplug interrupt
		 * handler.
		 */
		disable_irq(output->hpd_irq);
	}

	return 0;
}

void tegra_output_remove(struct tegra_output *output)
{
<<<<<<< HEAD
	if (gpio_is_valid(output->hpd_gpio)) {
		free_irq(output->hpd_irq, output);
		gpio_free(output->hpd_gpio);
	}

	if (output->ddc)
		put_device(&output->ddc->dev);
=======
	if (output->hpd_gpio)
		free_irq(output->hpd_irq, output);

	if (output->ddc)
		i2c_put_adapter(output->ddc);
>>>>>>> upstream/android-13
}

int tegra_output_init(struct drm_device *drm, struct tegra_output *output)
{
<<<<<<< HEAD
	int err;

	if (output->panel) {
		err = drm_panel_attach(output->panel, &output->connector);
		if (err < 0)
			return err;
	}
=======
	int connector_type;
>>>>>>> upstream/android-13

	/*
	 * The connector is now registered and ready to receive hotplug events
	 * so the hotplug interrupt can be enabled.
	 */
<<<<<<< HEAD
	if (gpio_is_valid(output->hpd_gpio))
		enable_irq(output->hpd_irq);

=======
	if (output->hpd_gpio)
		enable_irq(output->hpd_irq);

	connector_type = output->connector.connector_type;
	/*
	 * Create a CEC notifier for HDMI connector.
	 */
	if (connector_type == DRM_MODE_CONNECTOR_HDMIA ||
	    connector_type == DRM_MODE_CONNECTOR_HDMIB) {
		struct cec_connector_info conn_info;

		cec_fill_conn_info_from_drm(&conn_info, &output->connector);
		output->cec = cec_notifier_conn_register(output->dev, NULL,
							 &conn_info);
		if (!output->cec)
			return -ENOMEM;
	}

>>>>>>> upstream/android-13
	return 0;
}

void tegra_output_exit(struct tegra_output *output)
{
	/*
	 * The connector is going away, so the interrupt must be disabled to
	 * prevent the hotplug interrupt handler from potentially crashing.
	 */
<<<<<<< HEAD
	if (gpio_is_valid(output->hpd_gpio))
		disable_irq(output->hpd_irq);

	if (output->panel)
		drm_panel_detach(output->panel);
=======
	if (output->hpd_gpio)
		disable_irq(output->hpd_irq);
>>>>>>> upstream/android-13
}

void tegra_output_find_possible_crtcs(struct tegra_output *output,
				      struct drm_device *drm)
{
	struct device *dev = output->dev;
	struct drm_crtc *crtc;
	unsigned int mask = 0;

	drm_for_each_crtc(crtc, drm) {
		struct tegra_dc *dc = to_tegra_dc(crtc);

		if (tegra_dc_has_output(dc, dev))
			mask |= drm_crtc_mask(crtc);
	}

	if (mask == 0) {
		dev_warn(dev, "missing output definition for heads in DT\n");
		mask = 0x3;
	}

	output->encoder.possible_crtcs = mask;
}
<<<<<<< HEAD
=======

int tegra_output_suspend(struct tegra_output *output)
{
	if (output->hpd_irq)
		disable_irq(output->hpd_irq);

	return 0;
}

int tegra_output_resume(struct tegra_output *output)
{
	if (output->hpd_irq)
		enable_irq(output->hpd_irq);

	return 0;
}
>>>>>>> upstream/android-13
