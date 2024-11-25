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
/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Copyright (c) 2016 MediaTek Inc.
 * Author: Ming Hsiu Tsai <minghsiu.tsai@mediatek.com>
>>>>>>> upstream/android-13
 */

#ifndef __MTK_MDP_COMP_H__
#define __MTK_MDP_COMP_H__

/**
 * enum mtk_mdp_comp_type - the MDP component
 * @MTK_MDP_RDMA:	Read DMA
 * @MTK_MDP_RSZ:	Riszer
 * @MTK_MDP_WDMA:	Write DMA
 * @MTK_MDP_WROT:	Write DMA with rotation
 */
enum mtk_mdp_comp_type {
	MTK_MDP_RDMA,
	MTK_MDP_RSZ,
	MTK_MDP_WDMA,
	MTK_MDP_WROT,
<<<<<<< HEAD
	MTK_MDP_COMP_TYPE_MAX,
};

enum mtk_mdp_comp_id {
	MTK_MDP_COMP_RDMA0,
	MTK_MDP_COMP_RDMA1,
	MTK_MDP_COMP_RSZ0,
	MTK_MDP_COMP_RSZ1,
	MTK_MDP_COMP_RSZ2,
	MTK_MDP_COMP_WDMA,
	MTK_MDP_COMP_WROT0,
	MTK_MDP_COMP_WROT1,
	MTK_MDP_COMP_ID_MAX,
=======
>>>>>>> upstream/android-13
};

/**
 * struct mtk_mdp_comp - the MDP's function component data
<<<<<<< HEAD
 * @dev_node:	component device node
 * @clk:	clocks required for component
 * @regs:	Mapped address of component registers.
 * @type:	component type
 * @id:		component ID
 */
struct mtk_mdp_comp {
	struct device_node	*dev_node;
	struct clk		*clk[2];
	void __iomem		*regs;
	enum mtk_mdp_comp_type	type;
	enum mtk_mdp_comp_id	id;
};

int mtk_mdp_comp_init(struct device *dev, struct device_node *node,
		      struct mtk_mdp_comp *comp, enum mtk_mdp_comp_id comp_id);
void mtk_mdp_comp_deinit(struct device *dev, struct mtk_mdp_comp *comp);
int mtk_mdp_comp_get_id(struct device *dev, struct device_node *node,
			enum mtk_mdp_comp_type comp_type);
=======
 * @node:	list node to track sibing MDP components
 * @dev_node:	component device node
 * @clk:	clocks required for component
 * @larb_dev:	SMI device required for component
 * @type:	component type
 */
struct mtk_mdp_comp {
	struct list_head	node;
	struct device_node	*dev_node;
	struct clk		*clk[2];
	struct device		*larb_dev;
	enum mtk_mdp_comp_type	type;
};

int mtk_mdp_comp_init(struct device *dev, struct device_node *node,
		      struct mtk_mdp_comp *comp,
		      enum mtk_mdp_comp_type comp_type);
void mtk_mdp_comp_deinit(struct device *dev, struct mtk_mdp_comp *comp);
>>>>>>> upstream/android-13
void mtk_mdp_comp_clock_on(struct device *dev, struct mtk_mdp_comp *comp);
void mtk_mdp_comp_clock_off(struct device *dev, struct mtk_mdp_comp *comp);


#endif /* __MTK_MDP_COMP_H__ */
