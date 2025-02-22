<<<<<<< HEAD
/*
 * Copyright (c) 2014-2015, The Linux Foundation. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
=======
// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright (c) 2014-2015, The Linux Foundation. All rights reserved.
>>>>>>> upstream/android-13
 */

#include <linux/of_irq.h>
#include "edp.h"

static irqreturn_t edp_irq(int irq, void *dev_id)
{
	struct msm_edp *edp = dev_id;

	/* Process eDP irq */
	return msm_edp_ctrl_irq(edp->ctrl);
}

static void edp_destroy(struct platform_device *pdev)
{
	struct msm_edp *edp = platform_get_drvdata(pdev);

	if (!edp)
		return;

	if (edp->ctrl) {
		msm_edp_ctrl_destroy(edp->ctrl);
		edp->ctrl = NULL;
	}

	platform_set_drvdata(pdev, NULL);
}

/* construct eDP at bind/probe time, grab all the resources. */
static struct msm_edp *edp_init(struct platform_device *pdev)
{
	struct msm_edp *edp = NULL;
	int ret;

	if (!pdev) {
		pr_err("no eDP device\n");
		ret = -ENXIO;
		goto fail;
	}

	edp = devm_kzalloc(&pdev->dev, sizeof(*edp), GFP_KERNEL);
	if (!edp) {
		ret = -ENOMEM;
		goto fail;
	}
	DBG("eDP probed=%p", edp);

	edp->pdev = pdev;
	platform_set_drvdata(pdev, edp);

	ret = msm_edp_ctrl_init(edp);
	if (ret)
		goto fail;

	return edp;

fail:
	if (edp)
		edp_destroy(pdev);

	return ERR_PTR(ret);
}

static int edp_bind(struct device *dev, struct device *master, void *data)
{
	struct drm_device *drm = dev_get_drvdata(master);
	struct msm_drm_private *priv = drm->dev_private;
	struct msm_edp *edp;

	DBG("");
	edp = edp_init(to_platform_device(dev));
	if (IS_ERR(edp))
		return PTR_ERR(edp);
	priv->edp = edp;

	return 0;
}

static void edp_unbind(struct device *dev, struct device *master, void *data)
{
	struct drm_device *drm = dev_get_drvdata(master);
	struct msm_drm_private *priv = drm->dev_private;

	DBG("");
	if (priv->edp) {
		edp_destroy(to_platform_device(dev));
		priv->edp = NULL;
	}
}

static const struct component_ops edp_ops = {
		.bind   = edp_bind,
		.unbind = edp_unbind,
};

static int edp_dev_probe(struct platform_device *pdev)
{
	DBG("");
	return component_add(&pdev->dev, &edp_ops);
}

static int edp_dev_remove(struct platform_device *pdev)
{
	DBG("");
	component_del(&pdev->dev, &edp_ops);
	return 0;
}

static const struct of_device_id dt_match[] = {
	{ .compatible = "qcom,mdss-edp" },
	{}
};

static struct platform_driver edp_driver = {
	.probe = edp_dev_probe,
	.remove = edp_dev_remove,
	.driver = {
		.name = "msm_edp",
		.of_match_table = dt_match,
	},
};

void __init msm_edp_register(void)
{
	DBG("");
	platform_driver_register(&edp_driver);
}

void __exit msm_edp_unregister(void)
{
	DBG("");
	platform_driver_unregister(&edp_driver);
}

/* Second part of initialization, the drm/kms level modeset_init */
int msm_edp_modeset_init(struct msm_edp *edp, struct drm_device *dev,
				struct drm_encoder *encoder)
{
	struct platform_device *pdev = edp->pdev;
	struct msm_drm_private *priv = dev->dev_private;
	int ret;

	edp->encoder = encoder;
	edp->dev = dev;

	edp->bridge = msm_edp_bridge_init(edp);
	if (IS_ERR(edp->bridge)) {
		ret = PTR_ERR(edp->bridge);
<<<<<<< HEAD
		dev_err(dev->dev, "failed to create eDP bridge: %d\n", ret);
=======
		DRM_DEV_ERROR(dev->dev, "failed to create eDP bridge: %d\n", ret);
>>>>>>> upstream/android-13
		edp->bridge = NULL;
		goto fail;
	}

	edp->connector = msm_edp_connector_init(edp);
	if (IS_ERR(edp->connector)) {
		ret = PTR_ERR(edp->connector);
<<<<<<< HEAD
		dev_err(dev->dev, "failed to create eDP connector: %d\n", ret);
=======
		DRM_DEV_ERROR(dev->dev, "failed to create eDP connector: %d\n", ret);
>>>>>>> upstream/android-13
		edp->connector = NULL;
		goto fail;
	}

	edp->irq = irq_of_parse_and_map(pdev->dev.of_node, 0);
	if (edp->irq < 0) {
		ret = edp->irq;
<<<<<<< HEAD
		dev_err(dev->dev, "failed to get IRQ: %d\n", ret);
=======
		DRM_DEV_ERROR(dev->dev, "failed to get IRQ: %d\n", ret);
>>>>>>> upstream/android-13
		goto fail;
	}

	ret = devm_request_irq(&pdev->dev, edp->irq,
			edp_irq, IRQF_TRIGGER_HIGH | IRQF_ONESHOT,
			"edp_isr", edp);
	if (ret < 0) {
<<<<<<< HEAD
		dev_err(dev->dev, "failed to request IRQ%u: %d\n",
=======
		DRM_DEV_ERROR(dev->dev, "failed to request IRQ%u: %d\n",
>>>>>>> upstream/android-13
				edp->irq, ret);
		goto fail;
	}

<<<<<<< HEAD
	encoder->bridge = edp->bridge;

=======
>>>>>>> upstream/android-13
	priv->bridges[priv->num_bridges++]       = edp->bridge;
	priv->connectors[priv->num_connectors++] = edp->connector;

	return 0;

fail:
	/* bridge/connector are normally destroyed by drm */
	if (edp->bridge) {
		edp_bridge_destroy(edp->bridge);
		edp->bridge = NULL;
	}
	if (edp->connector) {
		edp->connector->funcs->destroy(edp->connector);
		edp->connector = NULL;
	}

	return ret;
}
