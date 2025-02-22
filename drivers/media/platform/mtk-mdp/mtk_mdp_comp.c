<<<<<<< HEAD
/*
 * Copyright (c) 2016 MediaTek Inc.
 * Author: Ming Hsiu Tsai <minghsiu.tsai@mediatek.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
=======
// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright (c) 2016 MediaTek Inc.
 * Author: Ming Hsiu Tsai <minghsiu.tsai@mediatek.com>
>>>>>>> upstream/android-13
 */

#include <linux/clk.h>
#include <linux/device.h>
#include <linux/of.h>
#include <linux/of_address.h>
#include <linux/of_platform.h>
<<<<<<< HEAD
=======
#include <soc/mediatek/smi.h>
>>>>>>> upstream/android-13

#include "mtk_mdp_comp.h"


<<<<<<< HEAD
static const char * const mtk_mdp_comp_stem[MTK_MDP_COMP_TYPE_MAX] = {
	"mdp_rdma",
	"mdp_rsz",
	"mdp_wdma",
	"mdp_wrot",
};

struct mtk_mdp_comp_match {
	enum mtk_mdp_comp_type type;
	int alias_id;
};

static const struct mtk_mdp_comp_match mtk_mdp_matches[MTK_MDP_COMP_ID_MAX] = {
	{ MTK_MDP_RDMA,	0 },
	{ MTK_MDP_RDMA,	1 },
	{ MTK_MDP_RSZ,	0 },
	{ MTK_MDP_RSZ,	1 },
	{ MTK_MDP_RSZ,	2 },
	{ MTK_MDP_WDMA,	0 },
	{ MTK_MDP_WROT,	0 },
	{ MTK_MDP_WROT,	1 },
};

int mtk_mdp_comp_get_id(struct device *dev, struct device_node *node,
			enum mtk_mdp_comp_type comp_type)
{
	int id = of_alias_get_id(node, mtk_mdp_comp_stem[comp_type]);
	int i;

	for (i = 0; i < ARRAY_SIZE(mtk_mdp_matches); i++) {
		if (comp_type == mtk_mdp_matches[i].type &&
		    id == mtk_mdp_matches[i].alias_id)
			return i;
	}

	dev_err(dev, "Failed to get id. type: %d, id: %d\n", comp_type, id);

	return -EINVAL;
}

=======
>>>>>>> upstream/android-13
void mtk_mdp_comp_clock_on(struct device *dev, struct mtk_mdp_comp *comp)
{
	int i, err;

<<<<<<< HEAD
=======
	if (comp->larb_dev) {
		err = mtk_smi_larb_get(comp->larb_dev);
		if (err)
			dev_err(dev,
				"failed to get larb, err %d. type:%d\n",
				err, comp->type);
	}

>>>>>>> upstream/android-13
	for (i = 0; i < ARRAY_SIZE(comp->clk); i++) {
		if (IS_ERR(comp->clk[i]))
			continue;
		err = clk_prepare_enable(comp->clk[i]);
		if (err)
			dev_err(dev,
<<<<<<< HEAD
			"failed to enable clock, err %d. type:%d id:%d i:%d\n",
				err, comp->type, comp->id, i);
=======
			"failed to enable clock, err %d. type:%d i:%d\n",
				err, comp->type, i);
>>>>>>> upstream/android-13
	}
}

void mtk_mdp_comp_clock_off(struct device *dev, struct mtk_mdp_comp *comp)
{
	int i;

	for (i = 0; i < ARRAY_SIZE(comp->clk); i++) {
		if (IS_ERR(comp->clk[i]))
			continue;
		clk_disable_unprepare(comp->clk[i]);
	}
<<<<<<< HEAD
}

int mtk_mdp_comp_init(struct device *dev, struct device_node *node,
		      struct mtk_mdp_comp *comp, enum mtk_mdp_comp_id comp_id)
{
	int i;

	if (comp_id < 0 || comp_id >= MTK_MDP_COMP_ID_MAX) {
		dev_err(dev, "Invalid comp_id %d\n", comp_id);
		return -EINVAL;
	}

	comp->dev_node = of_node_get(node);
	comp->id = comp_id;
	comp->type = mtk_mdp_matches[comp_id].type;
	comp->regs = of_iomap(node, 0);

	for (i = 0; i < ARRAY_SIZE(comp->clk); i++) {
		comp->clk[i] = of_clk_get(node, i);
=======

	if (comp->larb_dev)
		mtk_smi_larb_put(comp->larb_dev);
}

int mtk_mdp_comp_init(struct device *dev, struct device_node *node,
		      struct mtk_mdp_comp *comp,
		      enum mtk_mdp_comp_type comp_type)
{
	struct device_node *larb_node;
	struct platform_device *larb_pdev;
	int ret;
	int i;

	comp->dev_node = of_node_get(node);
	comp->type = comp_type;

	for (i = 0; i < ARRAY_SIZE(comp->clk); i++) {
		comp->clk[i] = of_clk_get(node, i);
		if (IS_ERR(comp->clk[i])) {
			if (PTR_ERR(comp->clk[i]) != -EPROBE_DEFER)
				dev_err(dev, "Failed to get clock\n");
			ret = PTR_ERR(comp->clk[i]);
			goto put_dev;
		}
>>>>>>> upstream/android-13

		/* Only RDMA needs two clocks */
		if (comp->type != MTK_MDP_RDMA)
			break;
	}

<<<<<<< HEAD
	return 0;
=======
	/* Only DMA capable components need the LARB property */
	comp->larb_dev = NULL;
	if (comp->type != MTK_MDP_RDMA &&
	    comp->type != MTK_MDP_WDMA &&
	    comp->type != MTK_MDP_WROT)
		return 0;

	larb_node = of_parse_phandle(node, "mediatek,larb", 0);
	if (!larb_node) {
		dev_err(dev,
			"Missing mediadek,larb phandle in %pOF node\n", node);
		ret = -EINVAL;
		goto put_dev;
	}

	larb_pdev = of_find_device_by_node(larb_node);
	if (!larb_pdev) {
		dev_warn(dev, "Waiting for larb device %pOF\n", larb_node);
		of_node_put(larb_node);
		ret = -EPROBE_DEFER;
		goto put_dev;
	}
	of_node_put(larb_node);

	comp->larb_dev = &larb_pdev->dev;

	return 0;

put_dev:
	of_node_put(comp->dev_node);

	return ret;
>>>>>>> upstream/android-13
}

void mtk_mdp_comp_deinit(struct device *dev, struct mtk_mdp_comp *comp)
{
	of_node_put(comp->dev_node);
}
