<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> upstream/android-13
/*
 * Copyright (C) 2009 Nokia Corporation
 * Author: Tomi Valkeinen <tomi.valkeinen@ti.com>
 *
 * VENC settings from TI's DSS driver
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published by
 * the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
=======
>>>>>>> upstream/android-13
 */

#define DSS_SUBSYS_NAME "VENC"

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/clk.h>
#include <linux/err.h>
#include <linux/io.h>
<<<<<<< HEAD
#include <linux/mutex.h>
=======
>>>>>>> upstream/android-13
#include <linux/completion.h>
#include <linux/delay.h>
#include <linux/string.h>
#include <linux/seq_file.h>
#include <linux/platform_device.h>
#include <linux/regulator/consumer.h>
#include <linux/pm_runtime.h>
#include <linux/of.h>
#include <linux/of_graph.h>
#include <linux/component.h>
#include <linux/sys_soc.h>

<<<<<<< HEAD
=======
#include <drm/drm_bridge.h>

>>>>>>> upstream/android-13
#include "omapdss.h"
#include "dss.h"

/* Venc registers */
#define VENC_REV_ID				0x00
#define VENC_STATUS				0x04
#define VENC_F_CONTROL				0x08
#define VENC_VIDOUT_CTRL			0x10
#define VENC_SYNC_CTRL				0x14
#define VENC_LLEN				0x1C
#define VENC_FLENS				0x20
#define VENC_HFLTR_CTRL				0x24
#define VENC_CC_CARR_WSS_CARR			0x28
#define VENC_C_PHASE				0x2C
#define VENC_GAIN_U				0x30
#define VENC_GAIN_V				0x34
#define VENC_GAIN_Y				0x38
#define VENC_BLACK_LEVEL			0x3C
#define VENC_BLANK_LEVEL			0x40
#define VENC_X_COLOR				0x44
#define VENC_M_CONTROL				0x48
#define VENC_BSTAMP_WSS_DATA			0x4C
#define VENC_S_CARR				0x50
#define VENC_LINE21				0x54
#define VENC_LN_SEL				0x58
#define VENC_L21__WC_CTL			0x5C
#define VENC_HTRIGGER_VTRIGGER			0x60
#define VENC_SAVID__EAVID			0x64
#define VENC_FLEN__FAL				0x68
#define VENC_LAL__PHASE_RESET			0x6C
#define VENC_HS_INT_START_STOP_X		0x70
#define VENC_HS_EXT_START_STOP_X		0x74
#define VENC_VS_INT_START_X			0x78
#define VENC_VS_INT_STOP_X__VS_INT_START_Y	0x7C
#define VENC_VS_INT_STOP_Y__VS_EXT_START_X	0x80
#define VENC_VS_EXT_STOP_X__VS_EXT_START_Y	0x84
#define VENC_VS_EXT_STOP_Y			0x88
#define VENC_AVID_START_STOP_X			0x90
#define VENC_AVID_START_STOP_Y			0x94
#define VENC_FID_INT_START_X__FID_INT_START_Y	0xA0
#define VENC_FID_INT_OFFSET_Y__FID_EXT_START_X	0xA4
#define VENC_FID_EXT_START_Y__FID_EXT_OFFSET_Y	0xA8
#define VENC_TVDETGP_INT_START_STOP_X		0xB0
#define VENC_TVDETGP_INT_START_STOP_Y		0xB4
#define VENC_GEN_CTRL				0xB8
#define VENC_OUTPUT_CONTROL			0xC4
#define VENC_OUTPUT_TEST			0xC8
#define VENC_DAC_B__DAC_C			0xC8

struct venc_config {
	u32 f_control;
	u32 vidout_ctrl;
	u32 sync_ctrl;
	u32 llen;
	u32 flens;
	u32 hfltr_ctrl;
	u32 cc_carr_wss_carr;
	u32 c_phase;
	u32 gain_u;
	u32 gain_v;
	u32 gain_y;
	u32 black_level;
	u32 blank_level;
	u32 x_color;
	u32 m_control;
	u32 bstamp_wss_data;
	u32 s_carr;
	u32 line21;
	u32 ln_sel;
	u32 l21__wc_ctl;
	u32 htrigger_vtrigger;
	u32 savid__eavid;
	u32 flen__fal;
	u32 lal__phase_reset;
	u32 hs_int_start_stop_x;
	u32 hs_ext_start_stop_x;
	u32 vs_int_start_x;
	u32 vs_int_stop_x__vs_int_start_y;
	u32 vs_int_stop_y__vs_ext_start_x;
	u32 vs_ext_stop_x__vs_ext_start_y;
	u32 vs_ext_stop_y;
	u32 avid_start_stop_x;
	u32 avid_start_stop_y;
	u32 fid_int_start_x__fid_int_start_y;
	u32 fid_int_offset_y__fid_ext_start_x;
	u32 fid_ext_start_y__fid_ext_offset_y;
	u32 tvdetgp_int_start_stop_x;
	u32 tvdetgp_int_start_stop_y;
	u32 gen_ctrl;
};

/* from TRM */
static const struct venc_config venc_config_pal_trm = {
	.f_control				= 0,
	.vidout_ctrl				= 1,
	.sync_ctrl				= 0x40,
	.llen					= 0x35F, /* 863 */
	.flens					= 0x270, /* 624 */
	.hfltr_ctrl				= 0,
	.cc_carr_wss_carr			= 0x2F7225ED,
	.c_phase				= 0,
	.gain_u					= 0x111,
	.gain_v					= 0x181,
	.gain_y					= 0x140,
	.black_level				= 0x3B,
	.blank_level				= 0x3B,
	.x_color				= 0x7,
	.m_control				= 0x2,
	.bstamp_wss_data			= 0x3F,
	.s_carr					= 0x2A098ACB,
	.line21					= 0,
	.ln_sel					= 0x01290015,
	.l21__wc_ctl				= 0x0000F603,
	.htrigger_vtrigger			= 0,

	.savid__eavid				= 0x06A70108,
	.flen__fal				= 0x00180270,
	.lal__phase_reset			= 0x00040135,
	.hs_int_start_stop_x			= 0x00880358,
	.hs_ext_start_stop_x			= 0x000F035F,
	.vs_int_start_x				= 0x01A70000,
	.vs_int_stop_x__vs_int_start_y		= 0x000001A7,
	.vs_int_stop_y__vs_ext_start_x		= 0x01AF0000,
	.vs_ext_stop_x__vs_ext_start_y		= 0x000101AF,
	.vs_ext_stop_y				= 0x00000025,
	.avid_start_stop_x			= 0x03530083,
	.avid_start_stop_y			= 0x026C002E,
	.fid_int_start_x__fid_int_start_y	= 0x0001008A,
	.fid_int_offset_y__fid_ext_start_x	= 0x002E0138,
	.fid_ext_start_y__fid_ext_offset_y	= 0x01380001,

	.tvdetgp_int_start_stop_x		= 0x00140001,
	.tvdetgp_int_start_stop_y		= 0x00010001,
	.gen_ctrl				= 0x00FF0000,
};

/* from TRM */
static const struct venc_config venc_config_ntsc_trm = {
	.f_control				= 0,
	.vidout_ctrl				= 1,
	.sync_ctrl				= 0x8040,
	.llen					= 0x359,
	.flens					= 0x20C,
	.hfltr_ctrl				= 0,
	.cc_carr_wss_carr			= 0x043F2631,
	.c_phase				= 0,
	.gain_u					= 0x102,
	.gain_v					= 0x16C,
	.gain_y					= 0x12F,
	.black_level				= 0x43,
	.blank_level				= 0x38,
	.x_color				= 0x7,
	.m_control				= 0x1,
	.bstamp_wss_data			= 0x38,
	.s_carr					= 0x21F07C1F,
	.line21					= 0,
	.ln_sel					= 0x01310011,
	.l21__wc_ctl				= 0x0000F003,
	.htrigger_vtrigger			= 0,

	.savid__eavid				= 0x069300F4,
	.flen__fal				= 0x0016020C,
	.lal__phase_reset			= 0x00060107,
	.hs_int_start_stop_x			= 0x008E0350,
	.hs_ext_start_stop_x			= 0x000F0359,
	.vs_int_start_x				= 0x01A00000,
	.vs_int_stop_x__vs_int_start_y		= 0x020701A0,
	.vs_int_stop_y__vs_ext_start_x		= 0x01AC0024,
	.vs_ext_stop_x__vs_ext_start_y		= 0x020D01AC,
	.vs_ext_stop_y				= 0x00000006,
	.avid_start_stop_x			= 0x03480078,
	.avid_start_stop_y			= 0x02060024,
	.fid_int_start_x__fid_int_start_y	= 0x0001008A,
	.fid_int_offset_y__fid_ext_start_x	= 0x01AC0106,
	.fid_ext_start_y__fid_ext_offset_y	= 0x01060006,

	.tvdetgp_int_start_stop_x		= 0x00140001,
	.tvdetgp_int_start_stop_y		= 0x00010001,
	.gen_ctrl				= 0x00F90000,
};

<<<<<<< HEAD
static const struct venc_config venc_config_pal_bdghi = {
	.f_control				= 0,
	.vidout_ctrl				= 0,
	.sync_ctrl				= 0,
	.hfltr_ctrl				= 0,
	.x_color				= 0,
	.line21					= 0,
	.ln_sel					= 21,
	.htrigger_vtrigger			= 0,
	.tvdetgp_int_start_stop_x		= 0x00140001,
	.tvdetgp_int_start_stop_y		= 0x00010001,
	.gen_ctrl				= 0x00FB0000,

	.llen					= 864-1,
	.flens					= 625-1,
	.cc_carr_wss_carr			= 0x2F7625ED,
	.c_phase				= 0xDF,
	.gain_u					= 0x111,
	.gain_v					= 0x181,
	.gain_y					= 0x140,
	.black_level				= 0x3e,
	.blank_level				= 0x3e,
	.m_control				= 0<<2 | 1<<1,
	.bstamp_wss_data			= 0x42,
	.s_carr					= 0x2a098acb,
	.l21__wc_ctl				= 0<<13 | 0x16<<8 | 0<<0,
	.savid__eavid				= 0x06A70108,
	.flen__fal				= 23<<16 | 624<<0,
	.lal__phase_reset			= 2<<17 | 310<<0,
	.hs_int_start_stop_x			= 0x00920358,
	.hs_ext_start_stop_x			= 0x000F035F,
	.vs_int_start_x				= 0x1a7<<16,
	.vs_int_stop_x__vs_int_start_y		= 0x000601A7,
	.vs_int_stop_y__vs_ext_start_x		= 0x01AF0036,
	.vs_ext_stop_x__vs_ext_start_y		= 0x27101af,
	.vs_ext_stop_y				= 0x05,
	.avid_start_stop_x			= 0x03530082,
	.avid_start_stop_y			= 0x0270002E,
	.fid_int_start_x__fid_int_start_y	= 0x0005008A,
	.fid_int_offset_y__fid_ext_start_x	= 0x002E0138,
	.fid_ext_start_y__fid_ext_offset_y	= 0x01380005,
};

=======
>>>>>>> upstream/android-13
enum venc_videomode {
	VENC_MODE_UNKNOWN,
	VENC_MODE_PAL,
	VENC_MODE_NTSC,
};

<<<<<<< HEAD
static const struct videomode omap_dss_pal_vm = {
	.hactive	= 720,
	.vactive	= 574,
	.pixelclock	= 13500000,
	.hsync_len	= 64,
	.hfront_porch	= 12,
	.hback_porch	= 68,
	.vsync_len	= 5,
	.vfront_porch	= 5,
	.vback_porch	= 41,

	.flags		= DISPLAY_FLAGS_INTERLACED | DISPLAY_FLAGS_HSYNC_LOW |
			  DISPLAY_FLAGS_VSYNC_LOW | DISPLAY_FLAGS_DE_HIGH |
			  DISPLAY_FLAGS_PIXDATA_POSEDGE |
			  DISPLAY_FLAGS_SYNC_NEGEDGE,
};

static const struct videomode omap_dss_ntsc_vm = {
	.hactive	= 720,
	.vactive	= 482,
	.pixelclock	= 13500000,
	.hsync_len	= 64,
	.hfront_porch	= 16,
	.hback_porch	= 58,
	.vsync_len	= 6,
	.vfront_porch	= 6,
	.vback_porch	= 31,

	.flags		= DISPLAY_FLAGS_INTERLACED | DISPLAY_FLAGS_HSYNC_LOW |
			  DISPLAY_FLAGS_VSYNC_LOW | DISPLAY_FLAGS_DE_HIGH |
			  DISPLAY_FLAGS_PIXDATA_POSEDGE |
			  DISPLAY_FLAGS_SYNC_NEGEDGE,
};

static enum venc_videomode venc_get_videomode(const struct videomode *vm)
{
	if (!(vm->flags & DISPLAY_FLAGS_INTERLACED))
		return VENC_MODE_UNKNOWN;

	if (vm->pixelclock == omap_dss_pal_vm.pixelclock &&
	    vm->hactive == omap_dss_pal_vm.hactive &&
	    vm->vactive == omap_dss_pal_vm.vactive)
		return VENC_MODE_PAL;

	if (vm->pixelclock == omap_dss_ntsc_vm.pixelclock &&
	    vm->hactive == omap_dss_ntsc_vm.hactive &&
	    vm->vactive == omap_dss_ntsc_vm.vactive)
		return VENC_MODE_NTSC;

	return VENC_MODE_UNKNOWN;
}

struct venc_device {
	struct platform_device *pdev;
	void __iomem *base;
	struct mutex venc_lock;
	u32 wss_data;
=======
static const struct drm_display_mode omap_dss_pal_mode = {
	.hdisplay	= 720,
	.hsync_start	= 732,
	.hsync_end	= 796,
	.htotal		= 864,
	.vdisplay	= 574,
	.vsync_start	= 579,
	.vsync_end	= 584,
	.vtotal		= 625,
	.clock		= 13500,

	.flags		= DRM_MODE_FLAG_INTERLACE | DRM_MODE_FLAG_NHSYNC |
			  DRM_MODE_FLAG_NVSYNC,
};

static const struct drm_display_mode omap_dss_ntsc_mode = {
	.hdisplay	= 720,
	.hsync_start	= 736,
	.hsync_end	= 800,
	.htotal		= 858,
	.vdisplay	= 482,
	.vsync_start	= 488,
	.vsync_end	= 494,
	.vtotal		= 525,
	.clock		= 13500,

	.flags		= DRM_MODE_FLAG_INTERLACE | DRM_MODE_FLAG_NHSYNC |
			  DRM_MODE_FLAG_NVSYNC,
};

struct venc_device {
	struct platform_device *pdev;
	void __iomem *base;
>>>>>>> upstream/android-13
	struct regulator *vdda_dac_reg;
	struct dss_device *dss;

	struct dss_debugfs_entry *debugfs;

	struct clk	*tv_dac_clk;

<<<<<<< HEAD
	struct videomode vm;
=======
	const struct venc_config *config;
>>>>>>> upstream/android-13
	enum omap_dss_venc_type type;
	bool invert_polarity;
	bool requires_tv_dac_clk;

	struct omap_dss_device output;
<<<<<<< HEAD
};

#define dssdev_to_venc(dssdev) container_of(dssdev, struct venc_device, output)
=======
	struct drm_bridge bridge;
};

#define drm_bridge_to_venc(b) container_of(b, struct venc_device, bridge)
>>>>>>> upstream/android-13

static inline void venc_write_reg(struct venc_device *venc, int idx, u32 val)
{
	__raw_writel(val, venc->base + idx);
}

static inline u32 venc_read_reg(struct venc_device *venc, int idx)
{
	u32 l = __raw_readl(venc->base + idx);
	return l;
}

static void venc_write_config(struct venc_device *venc,
			      const struct venc_config *config)
{
	DSSDBG("write venc conf\n");

	venc_write_reg(venc, VENC_LLEN, config->llen);
	venc_write_reg(venc, VENC_FLENS, config->flens);
	venc_write_reg(venc, VENC_CC_CARR_WSS_CARR, config->cc_carr_wss_carr);
	venc_write_reg(venc, VENC_C_PHASE, config->c_phase);
	venc_write_reg(venc, VENC_GAIN_U, config->gain_u);
	venc_write_reg(venc, VENC_GAIN_V, config->gain_v);
	venc_write_reg(venc, VENC_GAIN_Y, config->gain_y);
	venc_write_reg(venc, VENC_BLACK_LEVEL, config->black_level);
	venc_write_reg(venc, VENC_BLANK_LEVEL, config->blank_level);
	venc_write_reg(venc, VENC_M_CONTROL, config->m_control);
<<<<<<< HEAD
	venc_write_reg(venc, VENC_BSTAMP_WSS_DATA, config->bstamp_wss_data |
		       venc->wss_data);
=======
	venc_write_reg(venc, VENC_BSTAMP_WSS_DATA, config->bstamp_wss_data);
>>>>>>> upstream/android-13
	venc_write_reg(venc, VENC_S_CARR, config->s_carr);
	venc_write_reg(venc, VENC_L21__WC_CTL, config->l21__wc_ctl);
	venc_write_reg(venc, VENC_SAVID__EAVID, config->savid__eavid);
	venc_write_reg(venc, VENC_FLEN__FAL, config->flen__fal);
	venc_write_reg(venc, VENC_LAL__PHASE_RESET, config->lal__phase_reset);
	venc_write_reg(venc, VENC_HS_INT_START_STOP_X,
		       config->hs_int_start_stop_x);
	venc_write_reg(venc, VENC_HS_EXT_START_STOP_X,
		       config->hs_ext_start_stop_x);
	venc_write_reg(venc, VENC_VS_INT_START_X, config->vs_int_start_x);
	venc_write_reg(venc, VENC_VS_INT_STOP_X__VS_INT_START_Y,
		       config->vs_int_stop_x__vs_int_start_y);
	venc_write_reg(venc, VENC_VS_INT_STOP_Y__VS_EXT_START_X,
		       config->vs_int_stop_y__vs_ext_start_x);
	venc_write_reg(venc, VENC_VS_EXT_STOP_X__VS_EXT_START_Y,
		       config->vs_ext_stop_x__vs_ext_start_y);
	venc_write_reg(venc, VENC_VS_EXT_STOP_Y, config->vs_ext_stop_y);
	venc_write_reg(venc, VENC_AVID_START_STOP_X, config->avid_start_stop_x);
	venc_write_reg(venc, VENC_AVID_START_STOP_Y, config->avid_start_stop_y);
	venc_write_reg(venc, VENC_FID_INT_START_X__FID_INT_START_Y,
		       config->fid_int_start_x__fid_int_start_y);
	venc_write_reg(venc, VENC_FID_INT_OFFSET_Y__FID_EXT_START_X,
		       config->fid_int_offset_y__fid_ext_start_x);
	venc_write_reg(venc, VENC_FID_EXT_START_Y__FID_EXT_OFFSET_Y,
		       config->fid_ext_start_y__fid_ext_offset_y);

	venc_write_reg(venc, VENC_DAC_B__DAC_C,
		       venc_read_reg(venc, VENC_DAC_B__DAC_C));
	venc_write_reg(venc, VENC_VIDOUT_CTRL, config->vidout_ctrl);
	venc_write_reg(venc, VENC_HFLTR_CTRL, config->hfltr_ctrl);
	venc_write_reg(venc, VENC_X_COLOR, config->x_color);
	venc_write_reg(venc, VENC_LINE21, config->line21);
	venc_write_reg(venc, VENC_LN_SEL, config->ln_sel);
	venc_write_reg(venc, VENC_HTRIGGER_VTRIGGER, config->htrigger_vtrigger);
	venc_write_reg(venc, VENC_TVDETGP_INT_START_STOP_X,
		       config->tvdetgp_int_start_stop_x);
	venc_write_reg(venc, VENC_TVDETGP_INT_START_STOP_Y,
		       config->tvdetgp_int_start_stop_y);
	venc_write_reg(venc, VENC_GEN_CTRL, config->gen_ctrl);
	venc_write_reg(venc, VENC_F_CONTROL, config->f_control);
	venc_write_reg(venc, VENC_SYNC_CTRL, config->sync_ctrl);
}

static void venc_reset(struct venc_device *venc)
{
	int t = 1000;

	venc_write_reg(venc, VENC_F_CONTROL, 1<<8);
	while (venc_read_reg(venc, VENC_F_CONTROL) & (1<<8)) {
		if (--t == 0) {
			DSSERR("Failed to reset venc\n");
			return;
		}
	}

#ifdef CONFIG_OMAP2_DSS_SLEEP_AFTER_VENC_RESET
	/* the magical sleep that makes things work */
	/* XXX more info? What bug this circumvents? */
	msleep(20);
#endif
}

static int venc_runtime_get(struct venc_device *venc)
{
	int r;

	DSSDBG("venc_runtime_get\n");

	r = pm_runtime_get_sync(&venc->pdev->dev);
<<<<<<< HEAD
	WARN_ON(r < 0);
	return r < 0 ? r : 0;
=======
	if (WARN_ON(r < 0)) {
		pm_runtime_put_noidle(&venc->pdev->dev);
		return r;
	}
	return 0;
>>>>>>> upstream/android-13
}

static void venc_runtime_put(struct venc_device *venc)
{
	int r;

	DSSDBG("venc_runtime_put\n");

	r = pm_runtime_put_sync(&venc->pdev->dev);
	WARN_ON(r < 0 && r != -ENOSYS);
}

<<<<<<< HEAD
static const struct venc_config *venc_timings_to_config(struct videomode *vm)
{
	switch (venc_get_videomode(vm)) {
	default:
		WARN_ON_ONCE(1);
	case VENC_MODE_PAL:
		return &venc_config_pal_trm;
	case VENC_MODE_NTSC:
		return &venc_config_ntsc_trm;
	}
}

=======
>>>>>>> upstream/android-13
static int venc_power_on(struct venc_device *venc)
{
	u32 l;
	int r;

	r = venc_runtime_get(venc);
	if (r)
		goto err0;

	venc_reset(venc);
<<<<<<< HEAD
	venc_write_config(venc, venc_timings_to_config(&venc->vm));
=======
	venc_write_config(venc, venc->config);
>>>>>>> upstream/android-13

	dss_set_venc_output(venc->dss, venc->type);
	dss_set_dac_pwrdn_bgz(venc->dss, 1);

	l = 0;

	if (venc->type == OMAP_DSS_VENC_TYPE_COMPOSITE)
		l |= 1 << 1;
	else /* S-Video */
		l |= (1 << 0) | (1 << 2);

	if (venc->invert_polarity == false)
		l |= 1 << 3;

	venc_write_reg(venc, VENC_OUTPUT_CONTROL, l);

<<<<<<< HEAD
	dss_mgr_set_timings(&venc->output, &venc->vm);

=======
>>>>>>> upstream/android-13
	r = regulator_enable(venc->vdda_dac_reg);
	if (r)
		goto err1;

	r = dss_mgr_enable(&venc->output);
	if (r)
		goto err2;

	return 0;

err2:
	regulator_disable(venc->vdda_dac_reg);
err1:
	venc_write_reg(venc, VENC_OUTPUT_CONTROL, 0);
	dss_set_dac_pwrdn_bgz(venc->dss, 0);

	venc_runtime_put(venc);
err0:
	return r;
}

static void venc_power_off(struct venc_device *venc)
{
	venc_write_reg(venc, VENC_OUTPUT_CONTROL, 0);
	dss_set_dac_pwrdn_bgz(venc->dss, 0);

	dss_mgr_disable(&venc->output);

	regulator_disable(venc->vdda_dac_reg);

	venc_runtime_put(venc);
}

<<<<<<< HEAD
static int venc_display_enable(struct omap_dss_device *dssdev)
{
	struct venc_device *venc = dssdev_to_venc(dssdev);
	int r;

	DSSDBG("venc_display_enable\n");

	mutex_lock(&venc->venc_lock);

	if (!dssdev->dispc_channel_connected) {
		DSSERR("Failed to enable display: no output/manager\n");
		r = -ENODEV;
		goto err0;
	}

	r = venc_power_on(venc);
	if (r)
		goto err0;

	venc->wss_data = 0;

	mutex_unlock(&venc->venc_lock);

	return 0;
err0:
	mutex_unlock(&venc->venc_lock);
	return r;
}

static void venc_display_disable(struct omap_dss_device *dssdev)
{
	struct venc_device *venc = dssdev_to_venc(dssdev);

	DSSDBG("venc_display_disable\n");

	mutex_lock(&venc->venc_lock);

	venc_power_off(venc);

	mutex_unlock(&venc->venc_lock);
}

static void venc_set_timings(struct omap_dss_device *dssdev,
			     struct videomode *vm)
{
	struct venc_device *venc = dssdev_to_venc(dssdev);
	struct videomode actual_vm;

	DSSDBG("venc_set_timings\n");

	mutex_lock(&venc->venc_lock);

	switch (venc_get_videomode(vm)) {
	default:
		WARN_ON_ONCE(1);
	case VENC_MODE_PAL:
		actual_vm = omap_dss_pal_vm;
		break;
	case VENC_MODE_NTSC:
		actual_vm = omap_dss_ntsc_vm;
		break;
	}

	/* Reset WSS data when the TV standard changes. */
	if (memcmp(&venc->vm, &actual_vm, sizeof(actual_vm)))
		venc->wss_data = 0;

	venc->vm = actual_vm;

	dispc_set_tv_pclk(venc->dss->dispc, 13500000);

	mutex_unlock(&venc->venc_lock);
}

static int venc_check_timings(struct omap_dss_device *dssdev,
			      struct videomode *vm)
{
	DSSDBG("venc_check_timings\n");

	switch (venc_get_videomode(vm)) {
	case VENC_MODE_PAL:
	case VENC_MODE_NTSC:
		return 0;
	default:
		return -EINVAL;
	}
}

static void venc_get_timings(struct omap_dss_device *dssdev,
			     struct videomode *vm)
{
	struct venc_device *venc = dssdev_to_venc(dssdev);

	mutex_lock(&venc->venc_lock);

	*vm = venc->vm;

	mutex_unlock(&venc->venc_lock);
}

static u32 venc_get_wss(struct omap_dss_device *dssdev)
{
	struct venc_device *venc = dssdev_to_venc(dssdev);

	/* Invert due to VENC_L21_WC_CTL:INV=1 */
	return (venc->wss_data >> 8) ^ 0xfffff;
}

static int venc_set_wss(struct omap_dss_device *dssdev, u32 wss)
{
	struct venc_device *venc = dssdev_to_venc(dssdev);
	const struct venc_config *config;
	int r;

	DSSDBG("venc_set_wss\n");

	mutex_lock(&venc->venc_lock);

	config = venc_timings_to_config(&venc->vm);

	/* Invert due to VENC_L21_WC_CTL:INV=1 */
	venc->wss_data = (wss ^ 0xfffff) << 8;

	r = venc_runtime_get(venc);
	if (r)
		goto err;

	venc_write_reg(venc, VENC_BSTAMP_WSS_DATA, config->bstamp_wss_data |
		       venc->wss_data);

	venc_runtime_put(venc);

err:
	mutex_unlock(&venc->venc_lock);

	return r;
}

static int venc_init_regulator(struct venc_device *venc)
{
	struct regulator *vdda_dac;

	if (venc->vdda_dac_reg != NULL)
		return 0;

	vdda_dac = devm_regulator_get(&venc->pdev->dev, "vdda");
	if (IS_ERR(vdda_dac)) {
		if (PTR_ERR(vdda_dac) != -EPROBE_DEFER)
			DSSERR("can't get VDDA_DAC regulator\n");
		return PTR_ERR(vdda_dac);
	}

	venc->vdda_dac_reg = vdda_dac;

	return 0;
=======
static enum venc_videomode venc_get_videomode(const struct drm_display_mode *mode)
{
	if (!(mode->flags & DRM_MODE_FLAG_INTERLACE))
		return VENC_MODE_UNKNOWN;

	if (mode->clock == omap_dss_pal_mode.clock &&
	    mode->hdisplay == omap_dss_pal_mode.hdisplay &&
	    mode->vdisplay == omap_dss_pal_mode.vdisplay)
		return VENC_MODE_PAL;

	if (mode->clock == omap_dss_ntsc_mode.clock &&
	    mode->hdisplay == omap_dss_ntsc_mode.hdisplay &&
	    mode->vdisplay == omap_dss_ntsc_mode.vdisplay)
		return VENC_MODE_NTSC;

	return VENC_MODE_UNKNOWN;
>>>>>>> upstream/android-13
}

static int venc_dump_regs(struct seq_file *s, void *p)
{
	struct venc_device *venc = s->private;

#define DUMPREG(venc, r) \
	seq_printf(s, "%-35s %08x\n", #r, venc_read_reg(venc, r))

	if (venc_runtime_get(venc))
		return 0;

	DUMPREG(venc, VENC_F_CONTROL);
	DUMPREG(venc, VENC_VIDOUT_CTRL);
	DUMPREG(venc, VENC_SYNC_CTRL);
	DUMPREG(venc, VENC_LLEN);
	DUMPREG(venc, VENC_FLENS);
	DUMPREG(venc, VENC_HFLTR_CTRL);
	DUMPREG(venc, VENC_CC_CARR_WSS_CARR);
	DUMPREG(venc, VENC_C_PHASE);
	DUMPREG(venc, VENC_GAIN_U);
	DUMPREG(venc, VENC_GAIN_V);
	DUMPREG(venc, VENC_GAIN_Y);
	DUMPREG(venc, VENC_BLACK_LEVEL);
	DUMPREG(venc, VENC_BLANK_LEVEL);
	DUMPREG(venc, VENC_X_COLOR);
	DUMPREG(venc, VENC_M_CONTROL);
	DUMPREG(venc, VENC_BSTAMP_WSS_DATA);
	DUMPREG(venc, VENC_S_CARR);
	DUMPREG(venc, VENC_LINE21);
	DUMPREG(venc, VENC_LN_SEL);
	DUMPREG(venc, VENC_L21__WC_CTL);
	DUMPREG(venc, VENC_HTRIGGER_VTRIGGER);
	DUMPREG(venc, VENC_SAVID__EAVID);
	DUMPREG(venc, VENC_FLEN__FAL);
	DUMPREG(venc, VENC_LAL__PHASE_RESET);
	DUMPREG(venc, VENC_HS_INT_START_STOP_X);
	DUMPREG(venc, VENC_HS_EXT_START_STOP_X);
	DUMPREG(venc, VENC_VS_INT_START_X);
	DUMPREG(venc, VENC_VS_INT_STOP_X__VS_INT_START_Y);
	DUMPREG(venc, VENC_VS_INT_STOP_Y__VS_EXT_START_X);
	DUMPREG(venc, VENC_VS_EXT_STOP_X__VS_EXT_START_Y);
	DUMPREG(venc, VENC_VS_EXT_STOP_Y);
	DUMPREG(venc, VENC_AVID_START_STOP_X);
	DUMPREG(venc, VENC_AVID_START_STOP_Y);
	DUMPREG(venc, VENC_FID_INT_START_X__FID_INT_START_Y);
	DUMPREG(venc, VENC_FID_INT_OFFSET_Y__FID_EXT_START_X);
	DUMPREG(venc, VENC_FID_EXT_START_Y__FID_EXT_OFFSET_Y);
	DUMPREG(venc, VENC_TVDETGP_INT_START_STOP_X);
	DUMPREG(venc, VENC_TVDETGP_INT_START_STOP_Y);
	DUMPREG(venc, VENC_GEN_CTRL);
	DUMPREG(venc, VENC_OUTPUT_CONTROL);
	DUMPREG(venc, VENC_OUTPUT_TEST);

	venc_runtime_put(venc);

#undef DUMPREG
	return 0;
}

static int venc_get_clocks(struct venc_device *venc)
{
	struct clk *clk;

	if (venc->requires_tv_dac_clk) {
		clk = devm_clk_get(&venc->pdev->dev, "tv_dac_clk");
		if (IS_ERR(clk)) {
			DSSERR("can't get tv_dac_clk\n");
			return PTR_ERR(clk);
		}
	} else {
		clk = NULL;
	}

	venc->tv_dac_clk = clk;

	return 0;
}

<<<<<<< HEAD
static int venc_connect(struct omap_dss_device *dssdev,
		struct omap_dss_device *dst)
{
	struct venc_device *venc = dssdev_to_venc(dssdev);
	int r;

	r = venc_init_regulator(venc);
	if (r)
		return r;

	r = dss_mgr_connect(&venc->output, dssdev);
	if (r)
		return r;

	r = omapdss_output_set_device(dssdev, dst);
	if (r) {
		DSSERR("failed to connect output to new device: %s\n",
				dst->name);
		dss_mgr_disconnect(&venc->output, dssdev);
		return r;
	}
=======
/* -----------------------------------------------------------------------------
 * DRM Bridge Operations
 */

static int venc_bridge_attach(struct drm_bridge *bridge,
			      enum drm_bridge_attach_flags flags)
{
	struct venc_device *venc = drm_bridge_to_venc(bridge);

	if (!(flags & DRM_BRIDGE_ATTACH_NO_CONNECTOR))
		return -EINVAL;

	return drm_bridge_attach(bridge->encoder, venc->output.next_bridge,
				 bridge, flags);
}

static enum drm_mode_status
venc_bridge_mode_valid(struct drm_bridge *bridge,
		       const struct drm_display_info *info,
		       const struct drm_display_mode *mode)
{
	switch (venc_get_videomode(mode)) {
	case VENC_MODE_PAL:
	case VENC_MODE_NTSC:
		return MODE_OK;

	default:
		return MODE_BAD;
	}
}

static bool venc_bridge_mode_fixup(struct drm_bridge *bridge,
				   const struct drm_display_mode *mode,
				   struct drm_display_mode *adjusted_mode)
{
	const struct drm_display_mode *venc_mode;

	switch (venc_get_videomode(adjusted_mode)) {
	case VENC_MODE_PAL:
		venc_mode = &omap_dss_pal_mode;
		break;

	case VENC_MODE_NTSC:
		venc_mode = &omap_dss_ntsc_mode;
		break;

	default:
		return false;
	}

	drm_mode_copy(adjusted_mode, venc_mode);
	drm_mode_set_crtcinfo(adjusted_mode, CRTC_INTERLACE_HALVE_V);
	drm_mode_set_name(adjusted_mode);

	return true;
}

static void venc_bridge_mode_set(struct drm_bridge *bridge,
				 const struct drm_display_mode *mode,
				 const struct drm_display_mode *adjusted_mode)
{
	struct venc_device *venc = drm_bridge_to_venc(bridge);
	enum venc_videomode venc_mode = venc_get_videomode(adjusted_mode);

	switch (venc_mode) {
	default:
		WARN_ON_ONCE(1);
		fallthrough;
	case VENC_MODE_PAL:
		venc->config = &venc_config_pal_trm;
		break;

	case VENC_MODE_NTSC:
		venc->config = &venc_config_ntsc_trm;
		break;
	}

	dispc_set_tv_pclk(venc->dss->dispc, 13500000);
}

static void venc_bridge_enable(struct drm_bridge *bridge)
{
	struct venc_device *venc = drm_bridge_to_venc(bridge);

	venc_power_on(venc);
}

static void venc_bridge_disable(struct drm_bridge *bridge)
{
	struct venc_device *venc = drm_bridge_to_venc(bridge);

	venc_power_off(venc);
}

static int venc_bridge_get_modes(struct drm_bridge *bridge,
				 struct drm_connector *connector)
{
	static const struct drm_display_mode *modes[] = {
		&omap_dss_pal_mode,
		&omap_dss_ntsc_mode,
	};
	unsigned int i;

	for (i = 0; i < ARRAY_SIZE(modes); ++i) {
		struct drm_display_mode *mode;

		mode = drm_mode_duplicate(connector->dev, modes[i]);
		if (!mode)
			return i;

		mode->type = DRM_MODE_TYPE_DRIVER | DRM_MODE_TYPE_PREFERRED;
		drm_mode_set_name(mode);
		drm_mode_probed_add(connector, mode);
	}

	return ARRAY_SIZE(modes);
}

static const struct drm_bridge_funcs venc_bridge_funcs = {
	.attach = venc_bridge_attach,
	.mode_valid = venc_bridge_mode_valid,
	.mode_fixup = venc_bridge_mode_fixup,
	.mode_set = venc_bridge_mode_set,
	.enable = venc_bridge_enable,
	.disable = venc_bridge_disable,
	.get_modes = venc_bridge_get_modes,
};

static void venc_bridge_init(struct venc_device *venc)
{
	venc->bridge.funcs = &venc_bridge_funcs;
	venc->bridge.of_node = venc->pdev->dev.of_node;
	venc->bridge.ops = DRM_BRIDGE_OP_MODES;
	venc->bridge.type = DRM_MODE_CONNECTOR_SVIDEO;
	venc->bridge.interlace_allowed = true;

	drm_bridge_add(&venc->bridge);
}

static void venc_bridge_cleanup(struct venc_device *venc)
{
	drm_bridge_remove(&venc->bridge);
}

/* -----------------------------------------------------------------------------
 * Component Bind & Unbind
 */

static int venc_bind(struct device *dev, struct device *master, void *data)
{
	struct dss_device *dss = dss_get_device(master);
	struct venc_device *venc = dev_get_drvdata(dev);
	u8 rev_id;
	int r;

	venc->dss = dss;

	r = venc_runtime_get(venc);
	if (r)
		return r;

	rev_id = (u8)(venc_read_reg(venc, VENC_REV_ID) & 0xff);
	dev_dbg(dev, "OMAP VENC rev %d\n", rev_id);

	venc_runtime_put(venc);

	venc->debugfs = dss_debugfs_create_file(dss, "venc", venc_dump_regs,
						venc);
>>>>>>> upstream/android-13

	return 0;
}

<<<<<<< HEAD
static void venc_disconnect(struct omap_dss_device *dssdev,
		struct omap_dss_device *dst)
{
	struct venc_device *venc = dssdev_to_venc(dssdev);

	WARN_ON(dst != dssdev->dst);

	if (dst != dssdev->dst)
		return;

	omapdss_output_unset_device(dssdev);

	dss_mgr_disconnect(&venc->output, dssdev);
}

static const struct omapdss_atv_ops venc_ops = {
	.connect = venc_connect,
	.disconnect = venc_disconnect,

	.enable = venc_display_enable,
	.disable = venc_display_disable,

	.check_timings = venc_check_timings,
	.set_timings = venc_set_timings,
	.get_timings = venc_get_timings,

	.set_wss = venc_set_wss,
	.get_wss = venc_get_wss,
};

static void venc_init_output(struct venc_device *venc)
{
	struct omap_dss_device *out = &venc->output;

	out->dev = &venc->pdev->dev;
	out->id = OMAP_DSS_OUTPUT_VENC;
	out->output_type = OMAP_DISPLAY_TYPE_VENC;
	out->name = "venc.0";
	out->dispc_channel = OMAP_DSS_CHANNEL_DIGIT;
	out->ops.atv = &venc_ops;
	out->owner = THIS_MODULE;

	omapdss_register_output(out);
=======
static void venc_unbind(struct device *dev, struct device *master, void *data)
{
	struct venc_device *venc = dev_get_drvdata(dev);

	dss_debugfs_remove_file(venc->debugfs);
}

static const struct component_ops venc_component_ops = {
	.bind	= venc_bind,
	.unbind	= venc_unbind,
};

/* -----------------------------------------------------------------------------
 * Probe & Remove, Suspend & Resume
 */

static int venc_init_output(struct venc_device *venc)
{
	struct omap_dss_device *out = &venc->output;
	int r;

	venc_bridge_init(venc);

	out->dev = &venc->pdev->dev;
	out->id = OMAP_DSS_OUTPUT_VENC;
	out->type = OMAP_DISPLAY_TYPE_VENC;
	out->name = "venc.0";
	out->dispc_channel = OMAP_DSS_CHANNEL_DIGIT;
	out->of_port = 0;

	r = omapdss_device_init_output(out, &venc->bridge);
	if (r < 0) {
		venc_bridge_cleanup(venc);
		return r;
	}

	omapdss_device_register(out);

	return 0;
>>>>>>> upstream/android-13
}

static void venc_uninit_output(struct venc_device *venc)
{
<<<<<<< HEAD
	omapdss_unregister_output(&venc->output);
=======
	omapdss_device_unregister(&venc->output);
	omapdss_device_cleanup_output(&venc->output);

	venc_bridge_cleanup(venc);
>>>>>>> upstream/android-13
}

static int venc_probe_of(struct venc_device *venc)
{
	struct device_node *node = venc->pdev->dev.of_node;
	struct device_node *ep;
	u32 channels;
	int r;

	ep = of_graph_get_endpoint_by_regs(node, 0, 0);
	if (!ep)
		return 0;

	venc->invert_polarity = of_property_read_bool(ep, "ti,invert-polarity");

	r = of_property_read_u32(ep, "ti,channels", &channels);
	if (r) {
		dev_err(&venc->pdev->dev,
			"failed to read property 'ti,channels': %d\n", r);
		goto err;
	}

	switch (channels) {
	case 1:
		venc->type = OMAP_DSS_VENC_TYPE_COMPOSITE;
		break;
	case 2:
		venc->type = OMAP_DSS_VENC_TYPE_SVIDEO;
		break;
	default:
<<<<<<< HEAD
		dev_err(&venc->pdev->dev, "bad channel propert '%d'\n",
=======
		dev_err(&venc->pdev->dev, "bad channel property '%d'\n",
>>>>>>> upstream/android-13
			channels);
		r = -EINVAL;
		goto err;
	}

	of_node_put(ep);

	return 0;

err:
	of_node_put(ep);
	return r;
}

<<<<<<< HEAD
/* VENC HW IP initialisation */
=======
>>>>>>> upstream/android-13
static const struct soc_device_attribute venc_soc_devices[] = {
	{ .machine = "OMAP3[45]*" },
	{ .machine = "AM35*" },
	{ /* sentinel */ }
};

<<<<<<< HEAD
static int venc_bind(struct device *dev, struct device *master, void *data)
{
	struct platform_device *pdev = to_platform_device(dev);
	struct dss_device *dss = dss_get_device(master);
	struct venc_device *venc;
	u8 rev_id;
=======
static int venc_probe(struct platform_device *pdev)
{
	struct venc_device *venc;
>>>>>>> upstream/android-13
	struct resource *venc_mem;
	int r;

	venc = kzalloc(sizeof(*venc), GFP_KERNEL);
	if (!venc)
		return -ENOMEM;

	venc->pdev = pdev;
<<<<<<< HEAD
	venc->dss = dss;
	dev_set_drvdata(dev, venc);
=======

	platform_set_drvdata(pdev, venc);
>>>>>>> upstream/android-13

	/* The OMAP34xx, OMAP35xx and AM35xx VENC require the TV DAC clock. */
	if (soc_device_match(venc_soc_devices))
		venc->requires_tv_dac_clk = true;

<<<<<<< HEAD
	mutex_init(&venc->venc_lock);

	venc->wss_data = 0;
=======
	venc->config = &venc_config_pal_trm;
>>>>>>> upstream/android-13

	venc_mem = platform_get_resource(venc->pdev, IORESOURCE_MEM, 0);
	venc->base = devm_ioremap_resource(&pdev->dev, venc_mem);
	if (IS_ERR(venc->base)) {
		r = PTR_ERR(venc->base);
		goto err_free;
	}

<<<<<<< HEAD
=======
	venc->vdda_dac_reg = devm_regulator_get(&pdev->dev, "vdda");
	if (IS_ERR(venc->vdda_dac_reg)) {
		r = PTR_ERR(venc->vdda_dac_reg);
		if (r != -EPROBE_DEFER)
			DSSERR("can't get VDDA_DAC regulator\n");
		goto err_free;
	}

>>>>>>> upstream/android-13
	r = venc_get_clocks(venc);
	if (r)
		goto err_free;

<<<<<<< HEAD
	pm_runtime_enable(&pdev->dev);

	r = venc_runtime_get(venc);
	if (r)
		goto err_runtime_get;

	rev_id = (u8)(venc_read_reg(venc, VENC_REV_ID) & 0xff);
	dev_dbg(&pdev->dev, "OMAP VENC rev %d\n", rev_id);

	venc_runtime_put(venc);

	r = venc_probe_of(venc);
	if (r) {
		DSSERR("Invalid DT data\n");
		goto err_probe_of;
	}

	venc->debugfs = dss_debugfs_create_file(dss, "venc", venc_dump_regs,
						venc);

	venc_init_output(venc);

	return 0;

err_probe_of:
err_runtime_get:
=======
	r = venc_probe_of(venc);
	if (r)
		goto err_free;

	pm_runtime_enable(&pdev->dev);

	r = venc_init_output(venc);
	if (r)
		goto err_pm_disable;

	r = component_add(&pdev->dev, &venc_component_ops);
	if (r)
		goto err_uninit_output;

	return 0;

err_uninit_output:
	venc_uninit_output(venc);
err_pm_disable:
>>>>>>> upstream/android-13
	pm_runtime_disable(&pdev->dev);
err_free:
	kfree(venc);
	return r;
}

<<<<<<< HEAD
static void venc_unbind(struct device *dev, struct device *master, void *data)
{
	struct venc_device *venc = dev_get_drvdata(dev);

	dss_debugfs_remove_file(venc->debugfs);

	venc_uninit_output(venc);

	pm_runtime_disable(dev);

	kfree(venc);
}

static const struct component_ops venc_component_ops = {
	.bind	= venc_bind,
	.unbind	= venc_unbind,
};

static int venc_probe(struct platform_device *pdev)
{
	return component_add(&pdev->dev, &venc_component_ops);
}

static int venc_remove(struct platform_device *pdev)
{
	component_del(&pdev->dev, &venc_component_ops);
=======
static int venc_remove(struct platform_device *pdev)
{
	struct venc_device *venc = platform_get_drvdata(pdev);

	component_del(&pdev->dev, &venc_component_ops);

	venc_uninit_output(venc);

	pm_runtime_disable(&pdev->dev);

	kfree(venc);
>>>>>>> upstream/android-13
	return 0;
}

static int venc_runtime_suspend(struct device *dev)
{
	struct venc_device *venc = dev_get_drvdata(dev);

	if (venc->tv_dac_clk)
		clk_disable_unprepare(venc->tv_dac_clk);

<<<<<<< HEAD
	dispc_runtime_put(venc->dss->dispc);

=======
>>>>>>> upstream/android-13
	return 0;
}

static int venc_runtime_resume(struct device *dev)
{
	struct venc_device *venc = dev_get_drvdata(dev);
<<<<<<< HEAD
	int r;

	r = dispc_runtime_get(venc->dss->dispc);
	if (r < 0)
		return r;
=======
>>>>>>> upstream/android-13

	if (venc->tv_dac_clk)
		clk_prepare_enable(venc->tv_dac_clk);

	return 0;
}

static const struct dev_pm_ops venc_pm_ops = {
	.runtime_suspend = venc_runtime_suspend,
	.runtime_resume = venc_runtime_resume,
<<<<<<< HEAD
=======
	SET_LATE_SYSTEM_SLEEP_PM_OPS(pm_runtime_force_suspend, pm_runtime_force_resume)
>>>>>>> upstream/android-13
};

static const struct of_device_id venc_of_match[] = {
	{ .compatible = "ti,omap2-venc", },
	{ .compatible = "ti,omap3-venc", },
	{ .compatible = "ti,omap4-venc", },
	{},
};

struct platform_driver omap_venchw_driver = {
	.probe		= venc_probe,
	.remove		= venc_remove,
	.driver         = {
		.name   = "omapdss_venc",
		.pm	= &venc_pm_ops,
		.of_match_table = venc_of_match,
		.suppress_bind_attrs = true,
	},
};
