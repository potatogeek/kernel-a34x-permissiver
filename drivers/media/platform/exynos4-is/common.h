<<<<<<< HEAD
/*
 * Copyright (C) 2013 Samsung Electronics Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
=======
/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Copyright (C) 2013 Samsung Electronics Co., Ltd.
>>>>>>> upstream/android-13
 */

#include <linux/device.h>
#include <linux/videodev2.h>
#include <media/media-entity.h>
#include <media/v4l2-subdev.h>

struct v4l2_subdev *fimc_find_remote_sensor(struct media_entity *entity);
<<<<<<< HEAD
void __fimc_vidioc_querycap(struct device *dev, struct v4l2_capability *cap,
			    unsigned int caps);
=======
void __fimc_vidioc_querycap(struct device *dev, struct v4l2_capability *cap);
>>>>>>> upstream/android-13
