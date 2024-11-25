<<<<<<< HEAD
/*
 * Copyright (c) 2016 MediaTek Inc.
 * Author: Tiffany Lin <tiffany.lin@mediatek.com>
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
// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (c) 2016 MediaTek Inc.
 * Author: Tiffany Lin <tiffany.lin@mediatek.com>
>>>>>>> upstream/android-13
 */

#include <linux/clk.h>
#include <linux/of_address.h>
#include <linux/of_platform.h>
#include <linux/pm_runtime.h>
#include <soc/mediatek/smi.h>

#include "mtk_vcodec_dec_pm.h"
#include "mtk_vcodec_util.h"
<<<<<<< HEAD
#include "mtk_vpu.h"
=======
>>>>>>> upstream/android-13

int mtk_vcodec_init_dec_pm(struct mtk_vcodec_dev *mtkdev)
{
	struct device_node *node;
	struct platform_device *pdev;
	struct mtk_vcodec_pm *pm;
<<<<<<< HEAD
	int ret = 0;
=======
	struct mtk_vcodec_clk *dec_clk;
	struct mtk_vcodec_clk_info *clk_info;
	int i = 0, ret = 0;
>>>>>>> upstream/android-13

	pdev = mtkdev->plat_dev;
	pm = &mtkdev->pm;
	pm->mtkdev = mtkdev;
<<<<<<< HEAD
=======
	dec_clk = &pm->vdec_clk;
>>>>>>> upstream/android-13
	node = of_parse_phandle(pdev->dev.of_node, "mediatek,larb", 0);
	if (!node) {
		mtk_v4l2_err("of_parse_phandle mediatek,larb fail!");
		return -1;
	}

	pdev = of_find_device_by_node(node);
<<<<<<< HEAD
	if (WARN_ON(!pdev)) {
		of_node_put(node);
=======
	of_node_put(node);
	if (WARN_ON(!pdev)) {
>>>>>>> upstream/android-13
		return -1;
	}
	pm->larbvdec = &pdev->dev;
	pdev = mtkdev->plat_dev;
	pm->dev = &pdev->dev;

<<<<<<< HEAD
	pm->vcodecpll = devm_clk_get(&pdev->dev, "vcodecpll");
	if (IS_ERR(pm->vcodecpll)) {
		mtk_v4l2_err("devm_clk_get vcodecpll fail");
		ret = PTR_ERR(pm->vcodecpll);
	}

	pm->univpll_d2 = devm_clk_get(&pdev->dev, "univpll_d2");
	if (IS_ERR(pm->univpll_d2)) {
		mtk_v4l2_err("devm_clk_get univpll_d2 fail");
		ret = PTR_ERR(pm->univpll_d2);
	}

	pm->clk_cci400_sel = devm_clk_get(&pdev->dev, "clk_cci400_sel");
	if (IS_ERR(pm->clk_cci400_sel)) {
		mtk_v4l2_err("devm_clk_get clk_cci400_sel fail");
		ret = PTR_ERR(pm->clk_cci400_sel);
	}

	pm->vdec_sel = devm_clk_get(&pdev->dev, "vdec_sel");
	if (IS_ERR(pm->vdec_sel)) {
		mtk_v4l2_err("devm_clk_get vdec_sel fail");
		ret = PTR_ERR(pm->vdec_sel);
	}

	pm->vdecpll = devm_clk_get(&pdev->dev, "vdecpll");
	if (IS_ERR(pm->vdecpll)) {
		mtk_v4l2_err("devm_clk_get vdecpll fail");
		ret = PTR_ERR(pm->vdecpll);
	}

	pm->vencpll = devm_clk_get(&pdev->dev, "vencpll");
	if (IS_ERR(pm->vencpll)) {
		mtk_v4l2_err("devm_clk_get vencpll fail");
		ret = PTR_ERR(pm->vencpll);
	}

	pm->venc_lt_sel = devm_clk_get(&pdev->dev, "venc_lt_sel");
	if (IS_ERR(pm->venc_lt_sel)) {
		mtk_v4l2_err("devm_clk_get venc_lt_sel fail");
		ret = PTR_ERR(pm->venc_lt_sel);
	}

	pm->vdec_bus_clk_src = devm_clk_get(&pdev->dev, "vdec_bus_clk_src");
	if (IS_ERR(pm->vdec_bus_clk_src)) {
		mtk_v4l2_err("devm_clk_get vdec_bus_clk_src");
		ret = PTR_ERR(pm->vdec_bus_clk_src);
	}

	pm_runtime_enable(&pdev->dev);

=======
	dec_clk->clk_num =
		of_property_count_strings(pdev->dev.of_node, "clock-names");
	if (dec_clk->clk_num > 0) {
		dec_clk->clk_info = devm_kcalloc(&pdev->dev,
			dec_clk->clk_num, sizeof(*clk_info),
			GFP_KERNEL);
		if (!dec_clk->clk_info) {
			ret = -ENOMEM;
			goto put_device;
		}
	} else {
		mtk_v4l2_err("Failed to get vdec clock count");
		ret = -EINVAL;
		goto put_device;
	}

	for (i = 0; i < dec_clk->clk_num; i++) {
		clk_info = &dec_clk->clk_info[i];
		ret = of_property_read_string_index(pdev->dev.of_node,
			"clock-names", i, &clk_info->clk_name);
		if (ret) {
			mtk_v4l2_err("Failed to get clock name id = %d", i);
			goto put_device;
		}
		clk_info->vcodec_clk = devm_clk_get(&pdev->dev,
			clk_info->clk_name);
		if (IS_ERR(clk_info->vcodec_clk)) {
			mtk_v4l2_err("devm_clk_get (%d)%s fail", i,
				clk_info->clk_name);
			ret = PTR_ERR(clk_info->vcodec_clk);
			goto put_device;
		}
	}

	pm_runtime_enable(&pdev->dev);
	return 0;
put_device:
	put_device(pm->larbvdec);
>>>>>>> upstream/android-13
	return ret;
}

void mtk_vcodec_release_dec_pm(struct mtk_vcodec_dev *dev)
{
	pm_runtime_disable(dev->pm.dev);
	put_device(dev->pm.larbvdec);
}

<<<<<<< HEAD
void mtk_vcodec_dec_pw_on(struct mtk_vcodec_pm *pm)
{
	int ret;

	ret = pm_runtime_get_sync(pm->dev);
	if (ret)
		mtk_v4l2_err("pm_runtime_get_sync fail %d", ret);
=======
int mtk_vcodec_dec_pw_on(struct mtk_vcodec_pm *pm)
{
	int ret;

	ret = pm_runtime_resume_and_get(pm->dev);
	if (ret)
		mtk_v4l2_err("pm_runtime_resume_and_get fail %d", ret);

	return ret;
>>>>>>> upstream/android-13
}

void mtk_vcodec_dec_pw_off(struct mtk_vcodec_pm *pm)
{
	int ret;

	ret = pm_runtime_put_sync(pm->dev);
	if (ret)
		mtk_v4l2_err("pm_runtime_put_sync fail %d", ret);
}

void mtk_vcodec_dec_clock_on(struct mtk_vcodec_pm *pm)
{
<<<<<<< HEAD
	int ret;

	ret = clk_set_rate(pm->vcodecpll, 1482 * 1000000);
	if (ret)
		mtk_v4l2_err("clk_set_rate vcodecpll fail %d", ret);

	ret = clk_set_rate(pm->vencpll, 800 * 1000000);
	if (ret)
		mtk_v4l2_err("clk_set_rate vencpll fail %d", ret);

	ret = clk_prepare_enable(pm->vcodecpll);
	if (ret)
		mtk_v4l2_err("clk_prepare_enable vcodecpll fail %d", ret);

	ret = clk_prepare_enable(pm->vencpll);
	if (ret)
		mtk_v4l2_err("clk_prepare_enable vencpll fail %d", ret);

	ret = clk_prepare_enable(pm->vdec_bus_clk_src);
	if (ret)
		mtk_v4l2_err("clk_prepare_enable vdec_bus_clk_src fail %d",
				ret);

	ret = clk_prepare_enable(pm->venc_lt_sel);
	if (ret)
		mtk_v4l2_err("clk_prepare_enable venc_lt_sel fail %d", ret);

	ret = clk_set_parent(pm->venc_lt_sel, pm->vdec_bus_clk_src);
	if (ret)
		mtk_v4l2_err("clk_set_parent venc_lt_sel vdec_bus_clk_src fail %d",
				ret);

	ret = clk_prepare_enable(pm->univpll_d2);
	if (ret)
		mtk_v4l2_err("clk_prepare_enable univpll_d2 fail %d", ret);

	ret = clk_prepare_enable(pm->clk_cci400_sel);
	if (ret)
		mtk_v4l2_err("clk_prepare_enable clk_cci400_sel fail %d", ret);

	ret = clk_set_parent(pm->clk_cci400_sel, pm->univpll_d2);
	if (ret)
		mtk_v4l2_err("clk_set_parent clk_cci400_sel univpll_d2 fail %d",
				ret);

	ret = clk_prepare_enable(pm->vdecpll);
	if (ret)
		mtk_v4l2_err("clk_prepare_enable vdecpll fail %d", ret);

	ret = clk_prepare_enable(pm->vdec_sel);
	if (ret)
		mtk_v4l2_err("clk_prepare_enable vdec_sel fail %d", ret);

	ret = clk_set_parent(pm->vdec_sel, pm->vdecpll);
	if (ret)
		mtk_v4l2_err("clk_set_parent vdec_sel vdecpll fail %d", ret);

	ret = mtk_smi_larb_get(pm->larbvdec);
	if (ret)
		mtk_v4l2_err("mtk_smi_larb_get larbvdec fail %d", ret);

=======
	struct mtk_vcodec_clk *dec_clk = &pm->vdec_clk;
	int ret, i = 0;

	for (i = 0; i < dec_clk->clk_num; i++) {
		ret = clk_prepare_enable(dec_clk->clk_info[i].vcodec_clk);
		if (ret) {
			mtk_v4l2_err("clk_prepare_enable %d %s fail %d", i,
				dec_clk->clk_info[i].clk_name, ret);
			goto error;
		}
	}

	ret = mtk_smi_larb_get(pm->larbvdec);
	if (ret) {
		mtk_v4l2_err("mtk_smi_larb_get larbvdec fail %d", ret);
		goto error;
	}
	return;

error:
	for (i -= 1; i >= 0; i--)
		clk_disable_unprepare(dec_clk->clk_info[i].vcodec_clk);
>>>>>>> upstream/android-13
}

void mtk_vcodec_dec_clock_off(struct mtk_vcodec_pm *pm)
{
<<<<<<< HEAD
	mtk_smi_larb_put(pm->larbvdec);
	clk_disable_unprepare(pm->vdec_sel);
	clk_disable_unprepare(pm->vdecpll);
	clk_disable_unprepare(pm->univpll_d2);
	clk_disable_unprepare(pm->clk_cci400_sel);
	clk_disable_unprepare(pm->venc_lt_sel);
	clk_disable_unprepare(pm->vdec_bus_clk_src);
	clk_disable_unprepare(pm->vencpll);
	clk_disable_unprepare(pm->vcodecpll);
=======
	struct mtk_vcodec_clk *dec_clk = &pm->vdec_clk;
	int i = 0;

	mtk_smi_larb_put(pm->larbvdec);
	for (i = dec_clk->clk_num - 1; i >= 0; i--)
		clk_disable_unprepare(dec_clk->clk_info[i].vcodec_clk);
>>>>>>> upstream/android-13
}
