// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (c) 2019 MediaTek Inc.
 * Author: Wendell Lin <wendell.lin@mediatek.com>
 */

<<<<<<< HEAD
=======
#include <linux/module.h>
>>>>>>> upstream/android-13
#include <linux/clk-provider.h>
#include <linux/of.h>
#include <linux/of_address.h>
#include <linux/of_device.h>
#include <linux/platform_device.h>
<<<<<<< HEAD
#include <linux/slab.h>
=======
>>>>>>> upstream/android-13

#include "clk-mtk.h"
#include "clk-gate.h"

#include <dt-bindings/clock/mt6779-clk.h>

<<<<<<< HEAD
#define MT_CLKMGR_MODULE_INIT	0
#define CCF_SUBSYS_DEBUG		1


#define GATE_AUDIO0(_id, _name, _parent, _shift) {	\
	.id = _id,				\
	.name = _name,				\
	.parent_name = _parent,			\
	.regs = &audio0_cg_regs,		\
	.shift = _shift,			\
	.ops = &mtk_clk_gate_ops_no_setclr,	\
}

#define GATE_AUDIO1(_id, _name, _parent, _shift) {	\
	.id = _id,				\
	.name = _name,				\
	.parent_name = _parent,			\
	.regs = &audio1_cg_regs,		\
	.shift = _shift,			\
	.ops = &mtk_clk_gate_ops_no_setclr,	\
}

=======
>>>>>>> upstream/android-13
static const struct mtk_gate_regs audio0_cg_regs = {
	.set_ofs = 0x0,
	.clr_ofs = 0x0,
	.sta_ofs = 0x0,
};

static const struct mtk_gate_regs audio1_cg_regs = {
	.set_ofs = 0x4,
	.clr_ofs = 0x4,
	.sta_ofs = 0x4,
};

<<<<<<< HEAD
static const struct mtk_gate audio_clks[] = {
	/* AUDIO0 */
	GATE_AUDIO0(CLK_AUD_AFE, "aud_afe", "audio_sel",
		2),
	GATE_AUDIO0(CLK_AUD_22M, "aud_22m", "aud_eng1_sel",
		8),
	GATE_AUDIO0(CLK_AUD_24M, "aud_24m", "aud_eng2_sel",
		9),
	GATE_AUDIO0(CLK_AUD_APLL2_TUNER, "aud_apll2_tuner", "aud_eng2_sel",
		18),
	GATE_AUDIO0(CLK_AUD_APLL_TUNER, "aud_apll_tuner", "aud_eng1_sel",
		19),
	GATE_AUDIO0(CLK_AUD_TDM, "aud_tdm", "aud_eng1_sel",
		20),
	GATE_AUDIO0(CLK_AUD_ADC, "aud_adc", "audio_sel",
		24),
	GATE_AUDIO0(CLK_AUD_DAC, "aud_dac", "audio_sel",
		25),
	GATE_AUDIO0(CLK_AUD_DAC_PREDIS, "aud_dac_predis", "audio_sel",
		26),
	GATE_AUDIO0(CLK_AUD_TML, "aud_tml", "audio_sel",
		27),
	GATE_AUDIO0(CLK_AUD_NLE, "aud_nle", "audio_sel",
		28),
	/* AUDIO1: hf_faudio_ck/hf_faud_engen1_ck/hf_faud_engen2_ck */
	GATE_AUDIO1(CLK_AUD_I2S1_BCLK_SW, "aud_i2s1_bclk", "audio_sel",
		4),
	GATE_AUDIO1(CLK_AUD_I2S2_BCLK_SW, "aud_i2s2_bclk", "audio_sel",
		5),
	GATE_AUDIO1(CLK_AUD_I2S3_BCLK_SW, "aud_i2s3_bclk", "audio_sel",
		6),
	GATE_AUDIO1(CLK_AUD_I2S4_BCLK_SW, "aud_i2s4_bclk", "audio_sel",
		7),
	GATE_AUDIO1(CLK_AUD_I2S5_BCLK_SW, "aud_i2s5_bclk", "audio_sel",
		8),

	GATE_AUDIO1(CLK_AUD_CONN_I2S_ASRC, "aud_conn_i2s", "audio_sel",
		12),
	GATE_AUDIO1(CLK_AUD_GENERAL1_ASRC, "aud_general1", "audio_sel",
		13),
	GATE_AUDIO1(CLK_AUD_GENERAL2_ASRC, "aud_general2", "audio_sel",
		14),
	GATE_AUDIO1(CLK_AUD_DAC_HIRES, "aud_dac_hires", "audio_h_sel",
		15),
	GATE_AUDIO1(CLK_AUD_ADC_HIRES, "aud_adc_hires", "audio_h_sel",
		16),
	GATE_AUDIO1(CLK_AUD_ADC_HIRES_TML, "aud_adc_hires_tml", "audio_h_sel",
		17),

	GATE_AUDIO1(CLK_AUD_PDN_ADDA6_ADC, "aud_pdn_adda6_adc", "audio_sel",
		20),
	GATE_AUDIO1(CLK_AUD_ADDA6_ADC_HIRES, "aud_adda6_adc_hires",
		"audio_h_sel",
		21),
	GATE_AUDIO1(CLK_AUD_3RD_DAC, "aud_3rd_dac", "audio_sel",
		28),
	GATE_AUDIO1(CLK_AUD_3RD_DAC_PREDIS, "aud_3rd_dac_predis", "audio_sel",
		29),
	GATE_AUDIO1(CLK_AUD_3RD_DAC_TML, "aud_3rd_dac_tml", "audio_sel",
		30),
	GATE_AUDIO1(CLK_AUD_3RD_DAC_HIRES, "aud_3rd_dac_hires", "audio_h_sel",
		31),
=======
#define GATE_AUDIO0(_id, _name, _parent, _shift)		\
	GATE_MTK(_id, _name, _parent, &audio0_cg_regs, _shift,	\
		&mtk_clk_gate_ops_no_setclr)
#define GATE_AUDIO1(_id, _name, _parent, _shift)		\
	GATE_MTK(_id, _name, _parent, &audio1_cg_regs, _shift,	\
		&mtk_clk_gate_ops_no_setclr)

static const struct mtk_gate audio_clks[] = {
	/* AUDIO0 */
	GATE_AUDIO0(CLK_AUD_AFE, "aud_afe", "audio_sel", 2),
	GATE_AUDIO0(CLK_AUD_22M, "aud_22m", "aud_eng1_sel", 8),
	GATE_AUDIO0(CLK_AUD_24M, "aud_24m", "aud_eng2_sel", 9),
	GATE_AUDIO0(CLK_AUD_APLL2_TUNER, "aud_apll2_tuner",
		    "aud_eng2_sel", 18),
	GATE_AUDIO0(CLK_AUD_APLL_TUNER, "aud_apll_tuner",
		    "aud_eng1_sel", 19),
	GATE_AUDIO0(CLK_AUD_TDM, "aud_tdm", "aud_eng1_sel", 20),
	GATE_AUDIO0(CLK_AUD_ADC, "aud_adc", "audio_sel", 24),
	GATE_AUDIO0(CLK_AUD_DAC, "aud_dac", "audio_sel", 25),
	GATE_AUDIO0(CLK_AUD_DAC_PREDIS, "aud_dac_predis",
		    "audio_sel", 26),
	GATE_AUDIO0(CLK_AUD_TML, "aud_tml", "audio_sel", 27),
	GATE_AUDIO0(CLK_AUD_NLE, "aud_nle", "audio_sel", 28),
	/* AUDIO1 */
	GATE_AUDIO1(CLK_AUD_I2S1_BCLK_SW, "aud_i2s1_bclk",
		    "audio_sel", 4),
	GATE_AUDIO1(CLK_AUD_I2S2_BCLK_SW, "aud_i2s2_bclk",
		    "audio_sel", 5),
	GATE_AUDIO1(CLK_AUD_I2S3_BCLK_SW, "aud_i2s3_bclk",
		    "audio_sel", 6),
	GATE_AUDIO1(CLK_AUD_I2S4_BCLK_SW, "aud_i2s4_bclk",
		    "audio_sel", 7),
	GATE_AUDIO1(CLK_AUD_I2S5_BCLK_SW, "aud_i2s5_bclk",
		    "audio_sel", 8),
	GATE_AUDIO1(CLK_AUD_CONN_I2S_ASRC, "aud_conn_i2s",
		    "audio_sel", 12),
	GATE_AUDIO1(CLK_AUD_GENERAL1_ASRC, "aud_general1",
		    "audio_sel", 13),
	GATE_AUDIO1(CLK_AUD_GENERAL2_ASRC, "aud_general2",
		    "audio_sel", 14),
	GATE_AUDIO1(CLK_AUD_DAC_HIRES, "aud_dac_hires",
		    "audio_h_sel", 15),
	GATE_AUDIO1(CLK_AUD_ADC_HIRES, "aud_adc_hires",
		    "audio_h_sel", 16),
	GATE_AUDIO1(CLK_AUD_ADC_HIRES_TML, "aud_adc_hires_tml",
		    "audio_h_sel", 17),
	GATE_AUDIO1(CLK_AUD_PDN_ADDA6_ADC, "aud_pdn_adda6_adc",
		    "audio_sel", 20),
	GATE_AUDIO1(CLK_AUD_ADDA6_ADC_HIRES, "aud_adda6_adc_hires",
		    "audio_h_sel",
		    21),
	GATE_AUDIO1(CLK_AUD_3RD_DAC, "aud_3rd_dac", "audio_sel",
		    28),
	GATE_AUDIO1(CLK_AUD_3RD_DAC_PREDIS, "aud_3rd_dac_predis",
		    "audio_sel", 29),
	GATE_AUDIO1(CLK_AUD_3RD_DAC_TML, "aud_3rd_dac_tml",
		    "audio_sel", 30),
	GATE_AUDIO1(CLK_AUD_3RD_DAC_HIRES, "aud_3rd_dac_hires",
		    "audio_h_sel", 31),
>>>>>>> upstream/android-13
};

static const struct of_device_id of_match_clk_mt6779_aud[] = {
	{ .compatible = "mediatek,mt6779-audio", },
	{}
};

static int clk_mt6779_aud_probe(struct platform_device *pdev)
{
	struct clk_onecell_data *clk_data;
	struct device_node *node = pdev->dev.of_node;
<<<<<<< HEAD
	int ret;

	clk_data = mtk_alloc_clk_data(CLK_AUD_NR_CLK);
	if (!clk_data) {
		pr_notice("%s(): alloc clk data failed\n", __func__);
		return -ENOMEM;
	}

#if CCF_SUBSYS_DEBUG
	pr_info("%s(): clk data number: %d\n", __func__, clk_data->clk_num);
#endif
=======

	clk_data = mtk_alloc_clk_data(CLK_AUD_NR_CLK);
>>>>>>> upstream/android-13

	mtk_clk_register_gates(node, audio_clks, ARRAY_SIZE(audio_clks),
			       clk_data);

<<<<<<< HEAD
	ret = of_clk_add_provider(node, of_clk_src_onecell_get, clk_data);

	if (ret) {
		pr_notice("%s(): could not register clock provider: %d\n",
					__func__, ret);

		kfree(clk_data);
	}

	return ret;
=======
	return of_clk_add_provider(node, of_clk_src_onecell_get, clk_data);
>>>>>>> upstream/android-13
}

static struct platform_driver clk_mt6779_aud_drv = {
	.probe = clk_mt6779_aud_probe,
	.driver = {
		.name = "clk-mt6779-aud",
		.of_match_table = of_match_clk_mt6779_aud,
	},
};

<<<<<<< HEAD
#if MT_CLKMGR_MODULE_INIT

builtin_platform_driver(clk_mt6779_aud_drv);

#else

static int __init clk_mt6779_aud_platform_init(void)
{
	return platform_driver_register(&clk_mt6779_aud_drv);
}

arch_initcall_sync(clk_mt6779_aud_platform_init);

#endif /* MT_CLKMGR_MODULE_INIT */

=======
module_platform_driver(clk_mt6779_aud_drv);
MODULE_LICENSE("GPL");
>>>>>>> upstream/android-13
