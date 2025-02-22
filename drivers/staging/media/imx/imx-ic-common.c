<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0+
>>>>>>> upstream/android-13
/*
 * V4L2 Image Converter Subdev for Freescale i.MX5/6 SOC
 *
 * Copyright (c) 2014-2016 Mentor Graphics Inc.
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include <linux/module.h>
#include <linux/platform_device.h>
=======
 */
>>>>>>> upstream/android-13
#include <media/v4l2-device.h>
#include <media/v4l2-subdev.h>
#include "imx-media.h"
#include "imx-ic.h"

#define IC_TASK_PRP IC_NUM_TASKS
#define IC_NUM_OPS  (IC_NUM_TASKS + 1)

static struct imx_ic_ops *ic_ops[IC_NUM_OPS] = {
	[IC_TASK_PRP]            = &imx_ic_prp_ops,
	[IC_TASK_ENCODER]        = &imx_ic_prpencvf_ops,
	[IC_TASK_VIEWFINDER]     = &imx_ic_prpencvf_ops,
};

<<<<<<< HEAD
static int imx_ic_probe(struct platform_device *pdev)
{
	struct imx_media_internal_sd_platformdata *pdata;
	struct imx_ic_priv *priv;
	int ret;

	priv = devm_kzalloc(&pdev->dev, sizeof(*priv), GFP_KERNEL);
	if (!priv)
		return -ENOMEM;

	platform_set_drvdata(pdev, &priv->sd);
	priv->dev = &pdev->dev;

	/* get our ipu_id, grp_id and IC task id */
	pdata = priv->dev->platform_data;
	priv->ipu_id = pdata->ipu_id;
	switch (pdata->grp_id) {
	case IMX_MEDIA_GRP_ID_IC_PRP:
		priv->task_id = IC_TASK_PRP;
		break;
	case IMX_MEDIA_GRP_ID_IC_PRPENC:
		priv->task_id = IC_TASK_ENCODER;
		break;
	case IMX_MEDIA_GRP_ID_IC_PRPVF:
		priv->task_id = IC_TASK_VIEWFINDER;
		break;
	default:
		return -EINVAL;
=======
struct v4l2_subdev *imx_media_ic_register(struct v4l2_device *v4l2_dev,
					  struct device *ipu_dev,
					  struct ipu_soc *ipu,
					  u32 grp_id)
{
	struct imx_ic_priv *priv;
	int ret;

	priv = devm_kzalloc(ipu_dev, sizeof(*priv), GFP_KERNEL);
	if (!priv)
		return ERR_PTR(-ENOMEM);

	priv->ipu_dev = ipu_dev;
	priv->ipu = ipu;

	/* get our IC task id */
	switch (grp_id) {
	case IMX_MEDIA_GRP_ID_IPU_IC_PRP:
		priv->task_id = IC_TASK_PRP;
		break;
	case IMX_MEDIA_GRP_ID_IPU_IC_PRPENC:
		priv->task_id = IC_TASK_ENCODER;
		break;
	case IMX_MEDIA_GRP_ID_IPU_IC_PRPVF:
		priv->task_id = IC_TASK_VIEWFINDER;
		break;
	default:
		return ERR_PTR(-EINVAL);
>>>>>>> upstream/android-13
	}

	v4l2_subdev_init(&priv->sd, ic_ops[priv->task_id]->subdev_ops);
	v4l2_set_subdevdata(&priv->sd, priv);
	priv->sd.internal_ops = ic_ops[priv->task_id]->internal_ops;
	priv->sd.entity.ops = ic_ops[priv->task_id]->entity_ops;
	priv->sd.entity.function = MEDIA_ENT_F_PROC_VIDEO_SCALER;
<<<<<<< HEAD
	priv->sd.dev = &pdev->dev;
	priv->sd.owner = THIS_MODULE;
	priv->sd.flags = V4L2_SUBDEV_FL_HAS_DEVNODE | V4L2_SUBDEV_FL_HAS_EVENTS;
	priv->sd.grp_id = pdata->grp_id;
	strncpy(priv->sd.name, pdata->sd_name, sizeof(priv->sd.name));

	ret = ic_ops[priv->task_id]->init(priv);
	if (ret)
		return ret;

	ret = v4l2_async_register_subdev(&priv->sd);
	if (ret)
		ic_ops[priv->task_id]->remove(priv);

	return ret;
}

static int imx_ic_remove(struct platform_device *pdev)
{
	struct v4l2_subdev *sd = platform_get_drvdata(pdev);
=======
	priv->sd.owner = ipu_dev->driver->owner;
	priv->sd.flags = V4L2_SUBDEV_FL_HAS_DEVNODE | V4L2_SUBDEV_FL_HAS_EVENTS;
	priv->sd.grp_id = grp_id;
	imx_media_grp_id_to_sd_name(priv->sd.name, sizeof(priv->sd.name),
				    priv->sd.grp_id, ipu_get_num(ipu));

	ret = ic_ops[priv->task_id]->init(priv);
	if (ret)
		return ERR_PTR(ret);

	ret = v4l2_device_register_subdev(v4l2_dev, &priv->sd);
	if (ret) {
		ic_ops[priv->task_id]->remove(priv);
		return ERR_PTR(ret);
	}

	return &priv->sd;
}

int imx_media_ic_unregister(struct v4l2_subdev *sd)
{
>>>>>>> upstream/android-13
	struct imx_ic_priv *priv = container_of(sd, struct imx_ic_priv, sd);

	v4l2_info(sd, "Removing\n");

	ic_ops[priv->task_id]->remove(priv);

<<<<<<< HEAD
	v4l2_async_unregister_subdev(sd);
=======
	v4l2_device_unregister_subdev(sd);
>>>>>>> upstream/android-13
	media_entity_cleanup(&sd->entity);

	return 0;
}
<<<<<<< HEAD

static const struct platform_device_id imx_ic_ids[] = {
	{ .name = "imx-ipuv3-ic" },
	{ },
};
MODULE_DEVICE_TABLE(platform, imx_ic_ids);

static struct platform_driver imx_ic_driver = {
	.probe = imx_ic_probe,
	.remove = imx_ic_remove,
	.id_table = imx_ic_ids,
	.driver = {
		.name = "imx-ipuv3-ic",
	},
};
module_platform_driver(imx_ic_driver);

MODULE_DESCRIPTION("i.MX IC subdev driver");
MODULE_AUTHOR("Steve Longerbeam <steve_longerbeam@mentor.com>");
MODULE_LICENSE("GPL");
MODULE_ALIAS("platform:imx-ipuv3-ic");
=======
>>>>>>> upstream/android-13
