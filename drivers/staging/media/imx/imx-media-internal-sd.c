<<<<<<< HEAD
/*
 * Media driver for Freescale i.MX5/6 SOC
 *
 * Adds the internal subdevices and the media links between them.
 *
 * Copyright (c) 2016 Mentor Graphics Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
=======
// SPDX-License-Identifier: GPL-2.0+
/*
 * Media driver for Freescale i.MX5/6 SOC
 *
 * Adds the IPU internal subdevices and the media links between them.
 *
 * Copyright (c) 2016 Mentor Graphics Inc.
>>>>>>> upstream/android-13
 */
#include <linux/platform_device.h>
#include "imx-media.h"

<<<<<<< HEAD
enum isd_enum {
	isd_csi0 = 0,
	isd_csi1,
	isd_vdic,
	isd_ic_prp,
	isd_ic_prpenc,
	isd_ic_prpvf,
	num_isd,
};

static const struct internal_subdev_id {
	enum isd_enum index;
	const char *name;
	u32 grp_id;
} isd_id[num_isd] = {
	[isd_csi0] = {
		.index = isd_csi0,
		.grp_id = IMX_MEDIA_GRP_ID_CSI0,
		.name = "imx-ipuv3-csi",
	},
	[isd_csi1] = {
		.index = isd_csi1,
		.grp_id = IMX_MEDIA_GRP_ID_CSI1,
		.name = "imx-ipuv3-csi",
	},
	[isd_vdic] = {
		.index = isd_vdic,
		.grp_id = IMX_MEDIA_GRP_ID_VDIC,
		.name = "imx-ipuv3-vdic",
	},
	[isd_ic_prp] = {
		.index = isd_ic_prp,
		.grp_id = IMX_MEDIA_GRP_ID_IC_PRP,
		.name = "imx-ipuv3-ic",
	},
	[isd_ic_prpenc] = {
		.index = isd_ic_prpenc,
		.grp_id = IMX_MEDIA_GRP_ID_IC_PRPENC,
		.name = "imx-ipuv3-ic",
	},
	[isd_ic_prpvf] = {
		.index = isd_ic_prpvf,
		.grp_id = IMX_MEDIA_GRP_ID_IC_PRPVF,
		.name = "imx-ipuv3-ic",
	},
};

struct internal_subdev;

struct internal_link {
	const struct internal_subdev *remote;
	int local_pad;
	int remote_pad;
};

=======
>>>>>>> upstream/android-13
/* max pads per internal-sd */
#define MAX_INTERNAL_PADS   8
/* max links per internal-sd pad */
#define MAX_INTERNAL_LINKS  8

<<<<<<< HEAD
struct internal_pad {
	struct internal_link link[MAX_INTERNAL_LINKS];
};

static const struct internal_subdev {
	const struct internal_subdev_id *id;
	struct internal_pad pad[MAX_INTERNAL_PADS];
} int_subdev[num_isd] = {
	[isd_csi0] = {
		.id = &isd_id[isd_csi0],
		.pad[CSI_SRC_PAD_DIRECT] = {
			.link = {
				{
					.local_pad = CSI_SRC_PAD_DIRECT,
					.remote = &int_subdev[isd_ic_prp],
					.remote_pad = PRP_SINK_PAD,
				}, {
					.local_pad = CSI_SRC_PAD_DIRECT,
					.remote = &int_subdev[isd_vdic],
=======
struct internal_subdev;

struct internal_link {
	int remote;
	int local_pad;
	int remote_pad;
};

struct internal_pad {
	int num_links;
	struct internal_link link[MAX_INTERNAL_LINKS];
};

struct internal_subdev {
	u32 grp_id;
	struct internal_pad pad[MAX_INTERNAL_PADS];

	struct v4l2_subdev * (*sync_register)(struct v4l2_device *v4l2_dev,
					      struct device *ipu_dev,
					      struct ipu_soc *ipu,
					      u32 grp_id);
	int (*sync_unregister)(struct v4l2_subdev *sd);
};

static const struct internal_subdev int_subdev[NUM_IPU_SUBDEVS] = {
	[IPU_CSI0] = {
		.grp_id = IMX_MEDIA_GRP_ID_IPU_CSI0,
		.pad[CSI_SRC_PAD_DIRECT] = {
			.num_links = 2,
			.link = {
				{
					.local_pad = CSI_SRC_PAD_DIRECT,
					.remote = IPU_IC_PRP,
					.remote_pad = PRP_SINK_PAD,
				}, {
					.local_pad = CSI_SRC_PAD_DIRECT,
					.remote = IPU_VDIC,
>>>>>>> upstream/android-13
					.remote_pad = VDIC_SINK_PAD_DIRECT,
				},
			},
		},
	},

<<<<<<< HEAD
	[isd_csi1] = {
		.id = &isd_id[isd_csi1],
		.pad[CSI_SRC_PAD_DIRECT] = {
			.link = {
				{
					.local_pad = CSI_SRC_PAD_DIRECT,
					.remote = &int_subdev[isd_ic_prp],
					.remote_pad = PRP_SINK_PAD,
				}, {
					.local_pad = CSI_SRC_PAD_DIRECT,
					.remote = &int_subdev[isd_vdic],
=======
	[IPU_CSI1] = {
		.grp_id = IMX_MEDIA_GRP_ID_IPU_CSI1,
		.pad[CSI_SRC_PAD_DIRECT] = {
			.num_links = 2,
			.link = {
				{
					.local_pad = CSI_SRC_PAD_DIRECT,
					.remote = IPU_IC_PRP,
					.remote_pad = PRP_SINK_PAD,
				}, {
					.local_pad = CSI_SRC_PAD_DIRECT,
					.remote = IPU_VDIC,
>>>>>>> upstream/android-13
					.remote_pad = VDIC_SINK_PAD_DIRECT,
				},
			},
		},
	},

<<<<<<< HEAD
	[isd_vdic] = {
		.id = &isd_id[isd_vdic],
		.pad[VDIC_SRC_PAD_DIRECT] = {
			.link = {
				{
					.local_pad = VDIC_SRC_PAD_DIRECT,
					.remote = &int_subdev[isd_ic_prp],
=======
	[IPU_VDIC] = {
		.grp_id = IMX_MEDIA_GRP_ID_IPU_VDIC,
		.sync_register = imx_media_vdic_register,
		.sync_unregister = imx_media_vdic_unregister,
		.pad[VDIC_SRC_PAD_DIRECT] = {
			.num_links = 1,
			.link = {
				{
					.local_pad = VDIC_SRC_PAD_DIRECT,
					.remote = IPU_IC_PRP,
>>>>>>> upstream/android-13
					.remote_pad = PRP_SINK_PAD,
				},
			},
		},
	},

<<<<<<< HEAD
	[isd_ic_prp] = {
		.id = &isd_id[isd_ic_prp],
		.pad[PRP_SRC_PAD_PRPENC] = {
			.link = {
				{
					.local_pad = PRP_SRC_PAD_PRPENC,
					.remote = &int_subdev[isd_ic_prpenc],
					.remote_pad = 0,
=======
	[IPU_IC_PRP] = {
		.grp_id = IMX_MEDIA_GRP_ID_IPU_IC_PRP,
		.sync_register = imx_media_ic_register,
		.sync_unregister = imx_media_ic_unregister,
		.pad[PRP_SRC_PAD_PRPENC] = {
			.num_links = 1,
			.link = {
				{
					.local_pad = PRP_SRC_PAD_PRPENC,
					.remote = IPU_IC_PRPENC,
					.remote_pad = PRPENCVF_SINK_PAD,
>>>>>>> upstream/android-13
				},
			},
		},
		.pad[PRP_SRC_PAD_PRPVF] = {
<<<<<<< HEAD
			.link = {
				{
					.local_pad = PRP_SRC_PAD_PRPVF,
					.remote = &int_subdev[isd_ic_prpvf],
					.remote_pad = 0,
=======
			.num_links = 1,
			.link = {
				{
					.local_pad = PRP_SRC_PAD_PRPVF,
					.remote = IPU_IC_PRPVF,
					.remote_pad = PRPENCVF_SINK_PAD,
>>>>>>> upstream/android-13
				},
			},
		},
	},

<<<<<<< HEAD
	[isd_ic_prpenc] = {
		.id = &isd_id[isd_ic_prpenc],
	},

	[isd_ic_prpvf] = {
		.id = &isd_id[isd_ic_prpvf],
	},
};

/* form a device name given an internal subdev and ipu id */
static inline void isd_to_devname(char *devname, int sz,
				  const struct internal_subdev *isd,
				  int ipu_id)
{
	int pdev_id = ipu_id * num_isd + isd->id->index;

	snprintf(devname, sz, "%s.%d", isd->id->name, pdev_id);
}

static const struct internal_subdev *find_intsd_by_grp_id(u32 grp_id)
{
	enum isd_enum i;

	for (i = 0; i < num_isd; i++) {
		const struct internal_subdev *isd = &int_subdev[i];

		if (isd->id->grp_id == grp_id)
			return isd;
	}

	return NULL;
}

static struct v4l2_subdev *find_sink(struct imx_media_dev *imxmd,
				     struct v4l2_subdev *src,
				     const struct internal_link *link)
{
	char sink_devname[32];
	int ipu_id;

	/*
	 * retrieve IPU id from subdev name, note: can't get this from
	 * struct imx_media_internal_sd_platformdata because if src is
	 * a CSI, it has different struct ipu_client_platformdata which
	 * does not contain IPU id.
	 */
	if (sscanf(src->name, "ipu%d", &ipu_id) != 1)
		return NULL;

	isd_to_devname(sink_devname, sizeof(sink_devname),
		       link->remote, ipu_id - 1);

	return imx_media_find_subdev_by_devname(imxmd, sink_devname);
}

static int create_ipu_internal_link(struct imx_media_dev *imxmd,
				    struct v4l2_subdev *src,
				    const struct internal_link *link)
{
	struct v4l2_subdev *sink;
	int ret;

	sink = find_sink(imxmd, src, link);
	if (!sink)
		return -ENODEV;

	v4l2_info(&imxmd->v4l2_dev, "%s:%d -> %s:%d\n",
		  src->name, link->local_pad,
		  sink->name, link->remote_pad);
=======
	[IPU_IC_PRPENC] = {
		.grp_id = IMX_MEDIA_GRP_ID_IPU_IC_PRPENC,
		.sync_register = imx_media_ic_register,
		.sync_unregister = imx_media_ic_unregister,
	},

	[IPU_IC_PRPVF] = {
		.grp_id = IMX_MEDIA_GRP_ID_IPU_IC_PRPVF,
		.sync_register = imx_media_ic_register,
		.sync_unregister = imx_media_ic_unregister,
	},
};

static int create_internal_link(struct imx_media_dev *imxmd,
				struct v4l2_subdev *src,
				struct v4l2_subdev *sink,
				const struct internal_link *link)
{
	int ret;

	/* skip if this link already created */
	if (media_entity_find_link(&src->entity.pads[link->local_pad],
				   &sink->entity.pads[link->remote_pad]))
		return 0;

	dev_dbg(imxmd->md.dev, "%s:%d -> %s:%d\n",
		src->name, link->local_pad,
		sink->name, link->remote_pad);
>>>>>>> upstream/android-13

	ret = media_create_pad_link(&src->entity, link->local_pad,
				    &sink->entity, link->remote_pad, 0);
	if (ret)
<<<<<<< HEAD
		v4l2_err(&imxmd->v4l2_dev,
			 "create_pad_link failed: %d\n", ret);
=======
		v4l2_err(&imxmd->v4l2_dev, "%s failed: %d\n", __func__, ret);
>>>>>>> upstream/android-13

	return ret;
}

<<<<<<< HEAD
int imx_media_create_internal_links(struct imx_media_dev *imxmd,
				    struct v4l2_subdev *sd)
{
	const struct internal_subdev *intsd;
=======
static int create_ipu_internal_links(struct imx_media_dev *imxmd,
				     const struct internal_subdev *intsd,
				     struct v4l2_subdev *sd,
				     int ipu_id)
{
>>>>>>> upstream/android-13
	const struct internal_pad *intpad;
	const struct internal_link *link;
	struct media_pad *pad;
	int i, j, ret;

<<<<<<< HEAD
	intsd = find_intsd_by_grp_id(sd->grp_id);
	if (!intsd)
		return -ENODEV;

=======
>>>>>>> upstream/android-13
	/* create the source->sink links */
	for (i = 0; i < sd->entity.num_pads; i++) {
		intpad = &intsd->pad[i];
		pad = &sd->entity.pads[i];

		if (!(pad->flags & MEDIA_PAD_FL_SOURCE))
			continue;

<<<<<<< HEAD
		for (j = 0; ; j++) {
			link = &intpad->link[j];

			if (!link->remote)
				break;

			ret = create_ipu_internal_link(imxmd, sd, link);
=======
		for (j = 0; j < intpad->num_links; j++) {
			struct v4l2_subdev *sink;

			link = &intpad->link[j];
			sink = imxmd->sync_sd[ipu_id][link->remote];

			ret = create_internal_link(imxmd, sd, sink, link);
>>>>>>> upstream/android-13
			if (ret)
				return ret;
		}
	}

	return 0;
}

<<<<<<< HEAD
/* register an internal subdev as a platform device */
static int add_internal_subdev(struct imx_media_dev *imxmd,
			       const struct internal_subdev *isd,
			       int ipu_id)
{
	struct imx_media_internal_sd_platformdata pdata;
	struct platform_device_info pdevinfo = {};
	struct platform_device *pdev;

	pdata.grp_id = isd->id->grp_id;

	/* the id of IPU this subdev will control */
	pdata.ipu_id = ipu_id;

	/* create subdev name */
	imx_media_grp_id_to_sd_name(pdata.sd_name, sizeof(pdata.sd_name),
				    pdata.grp_id, ipu_id);

	pdevinfo.name = isd->id->name;
	pdevinfo.id = ipu_id * num_isd + isd->id->index;
	pdevinfo.parent = imxmd->md.dev;
	pdevinfo.data = &pdata;
	pdevinfo.size_data = sizeof(pdata);
	pdevinfo.dma_mask = DMA_BIT_MASK(32);

	pdev = platform_device_register_full(&pdevinfo);
	if (IS_ERR(pdev))
		return PTR_ERR(pdev);

	return imx_media_add_async_subdev(imxmd, NULL, pdev);
}

/* adds the internal subdevs in one ipu */
static int add_ipu_internal_subdevs(struct imx_media_dev *imxmd, int ipu_id)
{
	enum isd_enum i;

	for (i = 0; i < num_isd; i++) {
		const struct internal_subdev *isd = &int_subdev[i];
		int ret;

		/*
		 * the CSIs are represented in the device-tree, so those
		 * devices are already added to the async subdev list by
		 * of_parse_subdev().
		 */
		switch (isd->id->grp_id) {
		case IMX_MEDIA_GRP_ID_CSI0:
		case IMX_MEDIA_GRP_ID_CSI1:
			ret = 0;
			break;
		default:
			ret = add_internal_subdev(imxmd, isd, ipu_id);
			break;
		}

		if (ret)
			return ret;
	}

	return 0;
}

int imx_media_add_internal_subdevs(struct imx_media_dev *imxmd)
{
	int ret;

	ret = add_ipu_internal_subdevs(imxmd, 0);
	if (ret)
		goto remove;

	ret = add_ipu_internal_subdevs(imxmd, 1);
	if (ret)
		goto remove;

	return 0;

remove:
	imx_media_remove_internal_subdevs(imxmd);
	return ret;
}

void imx_media_remove_internal_subdevs(struct imx_media_dev *imxmd)
{
	struct imx_media_async_subdev *imxasd;

	list_for_each_entry(imxasd, &imxmd->asd_list, list) {
		if (!imxasd->pdev)
			continue;

		platform_device_unregister(imxasd->pdev);
	}
=======
int imx_media_register_ipu_internal_subdevs(struct imx_media_dev *imxmd,
					    struct v4l2_subdev *csi)
{
	struct device *ipu_dev = csi->dev->parent;
	const struct internal_subdev *intsd;
	struct v4l2_subdev *sd;
	struct ipu_soc *ipu;
	int i, ipu_id, ret;

	ipu = dev_get_drvdata(ipu_dev);
	if (!ipu) {
		v4l2_err(&imxmd->v4l2_dev, "invalid IPU device!\n");
		return -ENODEV;
	}

	ipu_id = ipu_get_num(ipu);
	if (ipu_id > 1) {
		v4l2_err(&imxmd->v4l2_dev, "invalid IPU id %d!\n", ipu_id);
		return -ENODEV;
	}

	mutex_lock(&imxmd->mutex);

	/* record this IPU */
	if (!imxmd->ipu[ipu_id])
		imxmd->ipu[ipu_id] = ipu;

	/* register the synchronous subdevs */
	for (i = 0; i < NUM_IPU_SUBDEVS; i++) {
		intsd = &int_subdev[i];

		sd = imxmd->sync_sd[ipu_id][i];

		/*
		 * skip if this sync subdev already registered or its
		 * not a sync subdev (one of the CSIs)
		 */
		if (sd || !intsd->sync_register)
			continue;

		mutex_unlock(&imxmd->mutex);
		sd = intsd->sync_register(&imxmd->v4l2_dev, ipu_dev, ipu,
					  intsd->grp_id);
		mutex_lock(&imxmd->mutex);
		if (IS_ERR(sd)) {
			ret = PTR_ERR(sd);
			goto err_unwind;
		}

		imxmd->sync_sd[ipu_id][i] = sd;
	}

	/*
	 * all the sync subdevs are registered, create the media links
	 * between them.
	 */
	for (i = 0; i < NUM_IPU_SUBDEVS; i++) {
		intsd = &int_subdev[i];

		if (intsd->grp_id == csi->grp_id) {
			sd = csi;
		} else {
			sd = imxmd->sync_sd[ipu_id][i];
			if (!sd)
				continue;
		}

		ret = create_ipu_internal_links(imxmd, intsd, sd, ipu_id);
		if (ret) {
			mutex_unlock(&imxmd->mutex);
			imx_media_unregister_ipu_internal_subdevs(imxmd);
			return ret;
		}
	}

	mutex_unlock(&imxmd->mutex);
	return 0;

err_unwind:
	while (--i >= 0) {
		intsd = &int_subdev[i];
		sd = imxmd->sync_sd[ipu_id][i];
		if (!sd || !intsd->sync_unregister)
			continue;
		mutex_unlock(&imxmd->mutex);
		intsd->sync_unregister(sd);
		mutex_lock(&imxmd->mutex);
	}

	mutex_unlock(&imxmd->mutex);
	return ret;
}

void imx_media_unregister_ipu_internal_subdevs(struct imx_media_dev *imxmd)
{
	const struct internal_subdev *intsd;
	struct v4l2_subdev *sd;
	int i, j;

	mutex_lock(&imxmd->mutex);

	for (i = 0; i < 2; i++) {
		for (j = 0; j < NUM_IPU_SUBDEVS; j++) {
			intsd = &int_subdev[j];
			sd = imxmd->sync_sd[i][j];

			if (!sd || !intsd->sync_unregister)
				continue;

			mutex_unlock(&imxmd->mutex);
			intsd->sync_unregister(sd);
			mutex_lock(&imxmd->mutex);
		}
	}

	mutex_unlock(&imxmd->mutex);
>>>>>>> upstream/android-13
}
