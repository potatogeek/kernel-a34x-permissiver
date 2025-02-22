<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0+
>>>>>>> upstream/android-13
/*
 * V4L2 Capture IC Preprocess Subdev for Freescale i.MX5/6 SOC
 *
 * This subdevice handles capture of video frames from the CSI or VDIC,
 * which are routed directly to the Image Converter preprocess tasks,
 * for resizing, colorspace conversion, and rotation.
 *
 * Copyright (c) 2012-2017 Mentor Graphics Inc.
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
=======
>>>>>>> upstream/android-13
 */
#include <linux/delay.h>
#include <linux/interrupt.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/slab.h>
#include <linux/spinlock.h>
#include <linux/timer.h>
#include <media/v4l2-ctrls.h>
#include <media/v4l2-device.h>
#include <media/v4l2-ioctl.h>
#include <media/v4l2-subdev.h>
#include <media/imx.h>
#include "imx-media.h"
#include "imx-ic.h"

/*
 * Min/Max supported width and heights.
 */
<<<<<<< HEAD
#define MIN_W       176
#define MIN_H       144
=======
#define MIN_W        32
#define MIN_H        32
>>>>>>> upstream/android-13
#define MAX_W      4096
#define MAX_H      4096
#define W_ALIGN    4 /* multiple of 16 pixels */
#define H_ALIGN    1 /* multiple of 2 lines */
#define S_ALIGN    1 /* multiple of 2 */

struct prp_priv {
<<<<<<< HEAD
	struct imx_media_dev *md;
=======
>>>>>>> upstream/android-13
	struct imx_ic_priv *ic_priv;
	struct media_pad pad[PRP_NUM_PADS];

	/* lock to protect all members below */
	struct mutex lock;

<<<<<<< HEAD
	/* IPU units we require */
	struct ipu_soc *ipu;

=======
>>>>>>> upstream/android-13
	struct v4l2_subdev *src_sd;
	struct v4l2_subdev *sink_sd_prpenc;
	struct v4l2_subdev *sink_sd_prpvf;

	/* the CSI id at link validate */
	int csi_id;

	struct v4l2_mbus_framefmt format_mbus;
	struct v4l2_fract frame_interval;

	int stream_count;
};

static inline struct prp_priv *sd_to_priv(struct v4l2_subdev *sd)
{
	struct imx_ic_priv *ic_priv = v4l2_get_subdevdata(sd);

<<<<<<< HEAD
	return ic_priv->prp_priv;
=======
	return ic_priv->task_priv;
>>>>>>> upstream/android-13
}

static int prp_start(struct prp_priv *priv)
{
	struct imx_ic_priv *ic_priv = priv->ic_priv;
	bool src_is_vdic;

<<<<<<< HEAD
	priv->ipu = priv->md->ipu[ic_priv->ipu_id];

	/* set IC to receive from CSI or VDI depending on source */
	src_is_vdic = !!(priv->src_sd->grp_id & IMX_MEDIA_GRP_ID_VDIC);

	ipu_set_ic_src_mux(priv->ipu, priv->csi_id, src_is_vdic);
=======
	/* set IC to receive from CSI or VDI depending on source */
	src_is_vdic = !!(priv->src_sd->grp_id & IMX_MEDIA_GRP_ID_IPU_VDIC);

	ipu_set_ic_src_mux(ic_priv->ipu, priv->csi_id, src_is_vdic);
>>>>>>> upstream/android-13

	return 0;
}

static void prp_stop(struct prp_priv *priv)
{
}

static struct v4l2_mbus_framefmt *
<<<<<<< HEAD
__prp_get_fmt(struct prp_priv *priv, struct v4l2_subdev_pad_config *cfg,
=======
__prp_get_fmt(struct prp_priv *priv, struct v4l2_subdev_state *sd_state,
>>>>>>> upstream/android-13
	      unsigned int pad, enum v4l2_subdev_format_whence which)
{
	struct imx_ic_priv *ic_priv = priv->ic_priv;

	if (which == V4L2_SUBDEV_FORMAT_TRY)
<<<<<<< HEAD
		return v4l2_subdev_get_try_format(&ic_priv->sd, cfg, pad);
=======
		return v4l2_subdev_get_try_format(&ic_priv->sd, sd_state, pad);
>>>>>>> upstream/android-13
	else
		return &priv->format_mbus;
}

/*
 * V4L2 subdev operations.
 */

static int prp_enum_mbus_code(struct v4l2_subdev *sd,
<<<<<<< HEAD
			      struct v4l2_subdev_pad_config *cfg,
=======
			      struct v4l2_subdev_state *sd_state,
>>>>>>> upstream/android-13
			      struct v4l2_subdev_mbus_code_enum *code)
{
	struct prp_priv *priv = sd_to_priv(sd);
	struct v4l2_mbus_framefmt *infmt;
	int ret = 0;

	mutex_lock(&priv->lock);

	switch (code->pad) {
	case PRP_SINK_PAD:
<<<<<<< HEAD
		ret = imx_media_enum_ipu_format(&code->code, code->index,
						CS_SEL_ANY);
=======
		ret = imx_media_enum_ipu_formats(&code->code, code->index,
						 PIXFMT_SEL_YUV_RGB);
>>>>>>> upstream/android-13
		break;
	case PRP_SRC_PAD_PRPENC:
	case PRP_SRC_PAD_PRPVF:
		if (code->index != 0) {
			ret = -EINVAL;
			goto out;
		}
<<<<<<< HEAD
		infmt = __prp_get_fmt(priv, cfg, PRP_SINK_PAD, code->which);
=======
		infmt = __prp_get_fmt(priv, sd_state, PRP_SINK_PAD,
				      code->which);
>>>>>>> upstream/android-13
		code->code = infmt->code;
		break;
	default:
		ret = -EINVAL;
	}
out:
	mutex_unlock(&priv->lock);
	return ret;
}

static int prp_get_fmt(struct v4l2_subdev *sd,
<<<<<<< HEAD
		       struct v4l2_subdev_pad_config *cfg,
=======
		       struct v4l2_subdev_state *sd_state,
>>>>>>> upstream/android-13
		       struct v4l2_subdev_format *sdformat)
{
	struct prp_priv *priv = sd_to_priv(sd);
	struct v4l2_mbus_framefmt *fmt;
	int ret = 0;

	if (sdformat->pad >= PRP_NUM_PADS)
		return -EINVAL;

	mutex_lock(&priv->lock);

<<<<<<< HEAD
	fmt = __prp_get_fmt(priv, cfg, sdformat->pad, sdformat->which);
=======
	fmt = __prp_get_fmt(priv, sd_state, sdformat->pad, sdformat->which);
>>>>>>> upstream/android-13
	if (!fmt) {
		ret = -EINVAL;
		goto out;
	}

	sdformat->format = *fmt;
out:
	mutex_unlock(&priv->lock);
	return ret;
}

static int prp_set_fmt(struct v4l2_subdev *sd,
<<<<<<< HEAD
		       struct v4l2_subdev_pad_config *cfg,
=======
		       struct v4l2_subdev_state *sd_state,
>>>>>>> upstream/android-13
		       struct v4l2_subdev_format *sdformat)
{
	struct prp_priv *priv = sd_to_priv(sd);
	struct v4l2_mbus_framefmt *fmt, *infmt;
	const struct imx_media_pixfmt *cc;
	int ret = 0;
	u32 code;

	if (sdformat->pad >= PRP_NUM_PADS)
		return -EINVAL;

	mutex_lock(&priv->lock);

	if (priv->stream_count > 0) {
		ret = -EBUSY;
		goto out;
	}

<<<<<<< HEAD
	infmt = __prp_get_fmt(priv, cfg, PRP_SINK_PAD, sdformat->which);
=======
	infmt = __prp_get_fmt(priv, sd_state, PRP_SINK_PAD, sdformat->which);
>>>>>>> upstream/android-13

	switch (sdformat->pad) {
	case PRP_SINK_PAD:
		v4l_bound_align_image(&sdformat->format.width, MIN_W, MAX_W,
				      W_ALIGN, &sdformat->format.height,
				      MIN_H, MAX_H, H_ALIGN, S_ALIGN);

		cc = imx_media_find_ipu_format(sdformat->format.code,
<<<<<<< HEAD
					       CS_SEL_ANY);
		if (!cc) {
			imx_media_enum_ipu_format(&code, 0, CS_SEL_ANY);
			cc = imx_media_find_ipu_format(code, CS_SEL_ANY);
			sdformat->format.code = cc->codes[0];
		}

		imx_media_fill_default_mbus_fields(&sdformat->format, infmt,
						   true);
=======
					       PIXFMT_SEL_YUV_RGB);
		if (!cc) {
			imx_media_enum_ipu_formats(&code, 0,
						   PIXFMT_SEL_YUV_RGB);
			cc = imx_media_find_ipu_format(code,
						       PIXFMT_SEL_YUV_RGB);
			sdformat->format.code = cc->codes[0];
		}

		if (sdformat->format.field == V4L2_FIELD_ANY)
			sdformat->format.field = V4L2_FIELD_NONE;
>>>>>>> upstream/android-13
		break;
	case PRP_SRC_PAD_PRPENC:
	case PRP_SRC_PAD_PRPVF:
		/* Output pads mirror input pad */
		sdformat->format = *infmt;
		break;
	}

<<<<<<< HEAD
	fmt = __prp_get_fmt(priv, cfg, sdformat->pad, sdformat->which);
=======
	imx_media_try_colorimetry(&sdformat->format, true);

	fmt = __prp_get_fmt(priv, sd_state, sdformat->pad, sdformat->which);
>>>>>>> upstream/android-13
	*fmt = sdformat->format;
out:
	mutex_unlock(&priv->lock);
	return ret;
}

static int prp_link_setup(struct media_entity *entity,
			  const struct media_pad *local,
			  const struct media_pad *remote, u32 flags)
{
	struct v4l2_subdev *sd = media_entity_to_v4l2_subdev(entity);
	struct imx_ic_priv *ic_priv = v4l2_get_subdevdata(sd);
<<<<<<< HEAD
	struct prp_priv *priv = ic_priv->prp_priv;
	struct v4l2_subdev *remote_sd;
	int ret = 0;

	dev_dbg(ic_priv->dev, "link setup %s -> %s", remote->entity->name,
		local->entity->name);
=======
	struct prp_priv *priv = ic_priv->task_priv;
	struct v4l2_subdev *remote_sd;
	int ret = 0;

	dev_dbg(ic_priv->ipu_dev, "%s: link setup %s -> %s",
		ic_priv->sd.name, remote->entity->name, local->entity->name);
>>>>>>> upstream/android-13

	remote_sd = media_entity_to_v4l2_subdev(remote->entity);

	mutex_lock(&priv->lock);

	if (local->flags & MEDIA_PAD_FL_SINK) {
		if (flags & MEDIA_LNK_FL_ENABLED) {
			if (priv->src_sd) {
				ret = -EBUSY;
				goto out;
			}
<<<<<<< HEAD
			if (priv->sink_sd_prpenc && (remote_sd->grp_id &
						     IMX_MEDIA_GRP_ID_VDIC)) {
=======
			if (priv->sink_sd_prpenc &&
			    (remote_sd->grp_id & IMX_MEDIA_GRP_ID_IPU_VDIC)) {
>>>>>>> upstream/android-13
				ret = -EINVAL;
				goto out;
			}
			priv->src_sd = remote_sd;
		} else {
			priv->src_sd = NULL;
		}

		goto out;
	}

	/* this is a source pad */
	if (flags & MEDIA_LNK_FL_ENABLED) {
		switch (local->index) {
		case PRP_SRC_PAD_PRPENC:
			if (priv->sink_sd_prpenc) {
				ret = -EBUSY;
				goto out;
			}
			if (priv->src_sd && (priv->src_sd->grp_id &
<<<<<<< HEAD
					     IMX_MEDIA_GRP_ID_VDIC)) {
=======
					     IMX_MEDIA_GRP_ID_IPU_VDIC)) {
>>>>>>> upstream/android-13
				ret = -EINVAL;
				goto out;
			}
			priv->sink_sd_prpenc = remote_sd;
			break;
		case PRP_SRC_PAD_PRPVF:
			if (priv->sink_sd_prpvf) {
				ret = -EBUSY;
				goto out;
			}
			priv->sink_sd_prpvf = remote_sd;
			break;
		default:
			ret = -EINVAL;
		}
	} else {
		switch (local->index) {
		case PRP_SRC_PAD_PRPENC:
			priv->sink_sd_prpenc = NULL;
			break;
		case PRP_SRC_PAD_PRPVF:
			priv->sink_sd_prpvf = NULL;
			break;
		default:
			ret = -EINVAL;
		}
	}

out:
	mutex_unlock(&priv->lock);
	return ret;
}

static int prp_link_validate(struct v4l2_subdev *sd,
			     struct media_link *link,
			     struct v4l2_subdev_format *source_fmt,
			     struct v4l2_subdev_format *sink_fmt)
{
	struct imx_ic_priv *ic_priv = v4l2_get_subdevdata(sd);
<<<<<<< HEAD
	struct prp_priv *priv = ic_priv->prp_priv;
=======
	struct prp_priv *priv = ic_priv->task_priv;
>>>>>>> upstream/android-13
	struct v4l2_subdev *csi;
	int ret;

	ret = v4l2_subdev_link_validate_default(sd, link,
						source_fmt, sink_fmt);
	if (ret)
		return ret;

<<<<<<< HEAD
	csi = imx_media_find_upstream_subdev(priv->md, &ic_priv->sd.entity,
					     IMX_MEDIA_GRP_ID_CSI);
=======
	csi = imx_media_pipeline_subdev(&ic_priv->sd.entity,
					IMX_MEDIA_GRP_ID_IPU_CSI, true);
>>>>>>> upstream/android-13
	if (IS_ERR(csi))
		csi = NULL;

	mutex_lock(&priv->lock);

<<<<<<< HEAD
	if (priv->src_sd->grp_id & IMX_MEDIA_GRP_ID_VDIC) {
=======
	if (priv->src_sd->grp_id & IMX_MEDIA_GRP_ID_IPU_VDIC) {
>>>>>>> upstream/android-13
		/*
		 * the ->PRPENC link cannot be enabled if the source
		 * is the VDIC
		 */
		if (priv->sink_sd_prpenc) {
			ret = -EINVAL;
			goto out;
		}
	} else {
		/* the source is a CSI */
		if (!csi) {
			ret = -EINVAL;
			goto out;
		}
	}

	if (csi) {
		switch (csi->grp_id) {
<<<<<<< HEAD
		case IMX_MEDIA_GRP_ID_CSI0:
			priv->csi_id = 0;
			break;
		case IMX_MEDIA_GRP_ID_CSI1:
=======
		case IMX_MEDIA_GRP_ID_IPU_CSI0:
			priv->csi_id = 0;
			break;
		case IMX_MEDIA_GRP_ID_IPU_CSI1:
>>>>>>> upstream/android-13
			priv->csi_id = 1;
			break;
		default:
			ret = -EINVAL;
		}
	} else {
		priv->csi_id = 0;
	}

out:
	mutex_unlock(&priv->lock);
	return ret;
}

static int prp_s_stream(struct v4l2_subdev *sd, int enable)
{
	struct imx_ic_priv *ic_priv = v4l2_get_subdevdata(sd);
<<<<<<< HEAD
	struct prp_priv *priv = ic_priv->prp_priv;
=======
	struct prp_priv *priv = ic_priv->task_priv;
>>>>>>> upstream/android-13
	int ret = 0;

	mutex_lock(&priv->lock);

	if (!priv->src_sd || (!priv->sink_sd_prpenc && !priv->sink_sd_prpvf)) {
		ret = -EPIPE;
		goto out;
	}

	/*
	 * enable/disable streaming only if stream_count is
	 * going from 0 to 1 / 1 to 0.
	 */
	if (priv->stream_count != !enable)
		goto update_count;

<<<<<<< HEAD
	dev_dbg(ic_priv->dev, "stream %s\n", enable ? "ON" : "OFF");
=======
	dev_dbg(ic_priv->ipu_dev, "%s: stream %s\n", sd->name,
		enable ? "ON" : "OFF");
>>>>>>> upstream/android-13

	if (enable)
		ret = prp_start(priv);
	else
		prp_stop(priv);
	if (ret)
		goto out;

	/* start/stop upstream */
	ret = v4l2_subdev_call(priv->src_sd, video, s_stream, enable);
	ret = (ret && ret != -ENOIOCTLCMD) ? ret : 0;
	if (ret) {
		if (enable)
			prp_stop(priv);
		goto out;
	}

update_count:
	priv->stream_count += enable ? 1 : -1;
	if (priv->stream_count < 0)
		priv->stream_count = 0;
out:
	mutex_unlock(&priv->lock);
	return ret;
}

static int prp_g_frame_interval(struct v4l2_subdev *sd,
				struct v4l2_subdev_frame_interval *fi)
{
	struct prp_priv *priv = sd_to_priv(sd);

	if (fi->pad >= PRP_NUM_PADS)
		return -EINVAL;

	mutex_lock(&priv->lock);
	fi->interval = priv->frame_interval;
	mutex_unlock(&priv->lock);

	return 0;
}

static int prp_s_frame_interval(struct v4l2_subdev *sd,
				struct v4l2_subdev_frame_interval *fi)
{
	struct prp_priv *priv = sd_to_priv(sd);

	if (fi->pad >= PRP_NUM_PADS)
		return -EINVAL;

<<<<<<< HEAD
	/* No limits on frame interval */
	mutex_lock(&priv->lock);
	priv->frame_interval = fi->interval;
=======
	mutex_lock(&priv->lock);

	/* No limits on valid frame intervals */
	if (fi->interval.numerator == 0 || fi->interval.denominator == 0)
		fi->interval = priv->frame_interval;
	else
		priv->frame_interval = fi->interval;

>>>>>>> upstream/android-13
	mutex_unlock(&priv->lock);

	return 0;
}

<<<<<<< HEAD
/*
 * retrieve our pads parsed from the OF graph by the media device
 */
static int prp_registered(struct v4l2_subdev *sd)
{
	struct prp_priv *priv = sd_to_priv(sd);
	int i, ret;
	u32 code;

	/* get media device */
	priv->md = dev_get_drvdata(sd->v4l2_dev->dev);

	for (i = 0; i < PRP_NUM_PADS; i++) {
		priv->pad[i].flags = (i == PRP_SINK_PAD) ?
			MEDIA_PAD_FL_SINK : MEDIA_PAD_FL_SOURCE;
	}

=======
static int prp_registered(struct v4l2_subdev *sd)
{
	struct prp_priv *priv = sd_to_priv(sd);
	u32 code;

>>>>>>> upstream/android-13
	/* init default frame interval */
	priv->frame_interval.numerator = 1;
	priv->frame_interval.denominator = 30;

	/* set a default mbus format  */
<<<<<<< HEAD
	imx_media_enum_ipu_format(&code, 0, CS_SEL_YUV);
	ret = imx_media_init_mbus_fmt(&priv->format_mbus, 640, 480, code,
				      V4L2_FIELD_NONE, NULL);
	if (ret)
		return ret;

	return media_entity_pads_init(&sd->entity, PRP_NUM_PADS, priv->pad);
=======
	imx_media_enum_ipu_formats(&code, 0, PIXFMT_SEL_YUV);

	return imx_media_init_mbus_fmt(&priv->format_mbus,
				       IMX_MEDIA_DEF_PIX_WIDTH,
				       IMX_MEDIA_DEF_PIX_HEIGHT, code,
				       V4L2_FIELD_NONE, NULL);
>>>>>>> upstream/android-13
}

static const struct v4l2_subdev_pad_ops prp_pad_ops = {
	.init_cfg = imx_media_init_cfg,
	.enum_mbus_code = prp_enum_mbus_code,
	.get_fmt = prp_get_fmt,
	.set_fmt = prp_set_fmt,
	.link_validate = prp_link_validate,
};

static const struct v4l2_subdev_video_ops prp_video_ops = {
	.g_frame_interval = prp_g_frame_interval,
	.s_frame_interval = prp_s_frame_interval,
	.s_stream = prp_s_stream,
};

static const struct media_entity_operations prp_entity_ops = {
	.link_setup = prp_link_setup,
	.link_validate = v4l2_subdev_link_validate,
};

static const struct v4l2_subdev_ops prp_subdev_ops = {
	.video = &prp_video_ops,
	.pad = &prp_pad_ops,
};

static const struct v4l2_subdev_internal_ops prp_internal_ops = {
	.registered = prp_registered,
};

static int prp_init(struct imx_ic_priv *ic_priv)
{
	struct prp_priv *priv;
<<<<<<< HEAD

	priv = devm_kzalloc(ic_priv->dev, sizeof(*priv), GFP_KERNEL);
=======
	int i;

	priv = devm_kzalloc(ic_priv->ipu_dev, sizeof(*priv), GFP_KERNEL);
>>>>>>> upstream/android-13
	if (!priv)
		return -ENOMEM;

	mutex_init(&priv->lock);
<<<<<<< HEAD
	ic_priv->prp_priv = priv;
	priv->ic_priv = ic_priv;

	return 0;
=======
	ic_priv->task_priv = priv;
	priv->ic_priv = ic_priv;

	for (i = 0; i < PRP_NUM_PADS; i++)
		priv->pad[i].flags = (i == PRP_SINK_PAD) ?
			MEDIA_PAD_FL_SINK : MEDIA_PAD_FL_SOURCE;

	return media_entity_pads_init(&ic_priv->sd.entity, PRP_NUM_PADS,
				      priv->pad);
>>>>>>> upstream/android-13
}

static void prp_remove(struct imx_ic_priv *ic_priv)
{
<<<<<<< HEAD
	struct prp_priv *priv = ic_priv->prp_priv;
=======
	struct prp_priv *priv = ic_priv->task_priv;
>>>>>>> upstream/android-13

	mutex_destroy(&priv->lock);
}

struct imx_ic_ops imx_ic_prp_ops = {
	.subdev_ops = &prp_subdev_ops,
	.internal_ops = &prp_internal_ops,
	.entity_ops = &prp_entity_ops,
	.init = prp_init,
	.remove = prp_remove,
};
