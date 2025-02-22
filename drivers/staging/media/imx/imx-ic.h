<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0+ */
>>>>>>> upstream/android-13
/*
 * V4L2 Image Converter Subdev for Freescale i.MX5/6 SOC
 *
 * Copyright (c) 2016 Mentor Graphics Inc.
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
=======
>>>>>>> upstream/android-13
 */
#ifndef _IMX_IC_H
#define _IMX_IC_H

#include <media/v4l2-subdev.h>

struct imx_ic_priv {
<<<<<<< HEAD
	struct device *dev;
	struct v4l2_subdev sd;
	int    ipu_id;
	int    task_id;
	void   *prp_priv;
=======
	struct device *ipu_dev;
	struct ipu_soc *ipu;
	struct v4l2_subdev sd;
	int    task_id;
>>>>>>> upstream/android-13
	void   *task_priv;
};

struct imx_ic_ops {
	const struct v4l2_subdev_ops *subdev_ops;
	const struct v4l2_subdev_internal_ops *internal_ops;
	const struct media_entity_operations *entity_ops;

	int (*init)(struct imx_ic_priv *ic_priv);
	void (*remove)(struct imx_ic_priv *ic_priv);
};

extern struct imx_ic_ops imx_ic_prp_ops;
extern struct imx_ic_ops imx_ic_prpencvf_ops;
<<<<<<< HEAD
extern struct imx_ic_ops imx_ic_pp_ops;
=======
>>>>>>> upstream/android-13

#endif
