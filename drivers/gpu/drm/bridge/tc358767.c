<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> upstream/android-13
/*
 * tc358767 eDP bridge driver
 *
 * Copyright (C) 2016 CogentEmbedded Inc
 * Author: Andrey Gusakov <andrey.gusakov@cogentembedded.com>
 *
 * Copyright (C) 2016 Pengutronix, Philipp Zabel <p.zabel@pengutronix.de>
 *
 * Copyright (C) 2016 Zodiac Inflight Innovations
 *
 * Initially based on: drivers/gpu/drm/i2c/tda998x_drv.c
 *
 * Copyright (C) 2012 Texas Instruments
 * Author: Rob Clark <robdclark@gmail.com>
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

=======
 */

#include <linux/bitfield.h>
>>>>>>> upstream/android-13
#include <linux/clk.h>
#include <linux/device.h>
#include <linux/gpio/consumer.h>
#include <linux/i2c.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/regmap.h>
#include <linux/slab.h>

#include <drm/drm_atomic_helper.h>
<<<<<<< HEAD
#include <drm/drm_crtc_helper.h>
=======
#include <drm/drm_bridge.h>
>>>>>>> upstream/android-13
#include <drm/drm_dp_helper.h>
#include <drm/drm_edid.h>
#include <drm/drm_of.h>
#include <drm/drm_panel.h>
<<<<<<< HEAD
=======
#include <drm/drm_print.h>
#include <drm/drm_probe_helper.h>
>>>>>>> upstream/android-13

/* Registers */

/* Display Parallel Interface */
#define DPIPXLFMT		0x0440
#define VS_POL_ACTIVE_LOW		(1 << 10)
#define HS_POL_ACTIVE_LOW		(1 << 9)
#define DE_POL_ACTIVE_HIGH		(0 << 8)
#define SUB_CFG_TYPE_CONFIG1		(0 << 2) /* LSB aligned */
#define SUB_CFG_TYPE_CONFIG2		(1 << 2) /* Loosely Packed */
#define SUB_CFG_TYPE_CONFIG3		(2 << 2) /* LSB aligned 8-bit */
#define DPI_BPP_RGB888			(0 << 0)
#define DPI_BPP_RGB666			(1 << 0)
#define DPI_BPP_RGB565			(2 << 0)

/* Video Path */
#define VPCTRL0			0x0450
<<<<<<< HEAD
=======
#define VSDELAY			GENMASK(31, 20)
>>>>>>> upstream/android-13
#define OPXLFMT_RGB666			(0 << 8)
#define OPXLFMT_RGB888			(1 << 8)
#define FRMSYNC_DISABLED		(0 << 4) /* Video Timing Gen Disabled */
#define FRMSYNC_ENABLED			(1 << 4) /* Video Timing Gen Enabled */
#define MSF_DISABLED			(0 << 0) /* Magic Square FRC disabled */
#define MSF_ENABLED			(1 << 0) /* Magic Square FRC enabled */
#define HTIM01			0x0454
<<<<<<< HEAD
#define HTIM02			0x0458
#define VTIM01			0x045c
#define VTIM02			0x0460
=======
#define HPW			GENMASK(8, 0)
#define HBPR			GENMASK(24, 16)
#define HTIM02			0x0458
#define HDISPR			GENMASK(10, 0)
#define HFPR			GENMASK(24, 16)
#define VTIM01			0x045c
#define VSPR			GENMASK(7, 0)
#define VBPR			GENMASK(23, 16)
#define VTIM02			0x0460
#define VFPR			GENMASK(23, 16)
#define VDISPR			GENMASK(10, 0)
>>>>>>> upstream/android-13
#define VFUEN0			0x0464
#define VFUEN				BIT(0)   /* Video Frame Timing Upload */

/* System */
#define TC_IDREG		0x0500
<<<<<<< HEAD
=======
#define SYSSTAT			0x0508
>>>>>>> upstream/android-13
#define SYSCTRL			0x0510
#define DP0_AUDSRC_NO_INPUT		(0 << 3)
#define DP0_AUDSRC_I2S_RX		(1 << 3)
#define DP0_VIDSRC_NO_INPUT		(0 << 0)
#define DP0_VIDSRC_DSI_RX		(1 << 0)
#define DP0_VIDSRC_DPI_RX		(2 << 0)
#define DP0_VIDSRC_COLOR_BAR		(3 << 0)
<<<<<<< HEAD
=======
#define SYSRSTENB		0x050c
#define ENBI2C				(1 << 0)
#define ENBLCD0				(1 << 2)
#define ENBBM				(1 << 3)
#define ENBDSIRX			(1 << 4)
#define ENBREG				(1 << 5)
#define ENBHDCP				(1 << 8)
#define GPIOM			0x0540
#define GPIOC			0x0544
#define GPIOO			0x0548
#define GPIOI			0x054c
#define INTCTL_G		0x0560
#define INTSTS_G		0x0564

#define INT_SYSERR		BIT(16)
#define INT_GPIO_H(x)		(1 << (x == 0 ? 2 : 10))
#define INT_GPIO_LC(x)		(1 << (x == 0 ? 3 : 11))

#define INT_GP0_LCNT		0x0584
#define INT_GP1_LCNT		0x0588
>>>>>>> upstream/android-13

/* Control */
#define DP0CTL			0x0600
#define VID_MN_GEN			BIT(6)   /* Auto-generate M/N values */
#define EF_EN				BIT(5)   /* Enable Enhanced Framing */
#define VID_EN				BIT(1)   /* Video transmission enable */
#define DP_EN				BIT(0)   /* Enable DPTX function */

/* Clocks */
#define DP0_VIDMNGEN0		0x0610
#define DP0_VIDMNGEN1		0x0614
#define DP0_VMNGENSTATUS	0x0618

/* Main Channel */
#define DP0_SECSAMPLE		0x0640
#define DP0_VIDSYNCDELAY	0x0644
<<<<<<< HEAD
#define DP0_TOTALVAL		0x0648
#define DP0_STARTVAL		0x064c
#define DP0_ACTIVEVAL		0x0650
#define DP0_SYNCVAL		0x0654
=======
#define VID_SYNC_DLY		GENMASK(15, 0)
#define THRESH_DLY		GENMASK(31, 16)

#define DP0_TOTALVAL		0x0648
#define H_TOTAL			GENMASK(15, 0)
#define V_TOTAL			GENMASK(31, 16)
#define DP0_STARTVAL		0x064c
#define H_START			GENMASK(15, 0)
#define V_START			GENMASK(31, 16)
#define DP0_ACTIVEVAL		0x0650
#define H_ACT			GENMASK(15, 0)
#define V_ACT			GENMASK(31, 16)

#define DP0_SYNCVAL		0x0654
#define VS_WIDTH		GENMASK(30, 16)
#define HS_WIDTH		GENMASK(14, 0)
>>>>>>> upstream/android-13
#define SYNCVAL_HS_POL_ACTIVE_LOW	(1 << 15)
#define SYNCVAL_VS_POL_ACTIVE_LOW	(1 << 31)
#define DP0_MISC		0x0658
#define TU_SIZE_RECOMMENDED		(63) /* LSCLK cycles per TU */
<<<<<<< HEAD
=======
#define MAX_TU_SYMBOL		GENMASK(28, 23)
#define TU_SIZE			GENMASK(21, 16)
>>>>>>> upstream/android-13
#define BPC_6				(0 << 5)
#define BPC_8				(1 << 5)

/* AUX channel */
#define DP0_AUXCFG0		0x0660
<<<<<<< HEAD
=======
#define DP0_AUXCFG0_BSIZE	GENMASK(11, 8)
#define DP0_AUXCFG0_ADDR_ONLY	BIT(4)
>>>>>>> upstream/android-13
#define DP0_AUXCFG1		0x0664
#define AUX_RX_FILTER_EN		BIT(16)

#define DP0_AUXADDR		0x0668
#define DP0_AUXWDATA(i)		(0x066c + (i) * 4)
#define DP0_AUXRDATA(i)		(0x067c + (i) * 4)
#define DP0_AUXSTATUS		0x068c
<<<<<<< HEAD
#define AUX_STATUS_MASK			0xf0
#define AUX_STATUS_SHIFT		4
#define AUX_TIMEOUT			BIT(1)
#define AUX_BUSY			BIT(0)
=======
#define AUX_BYTES		GENMASK(15, 8)
#define AUX_STATUS		GENMASK(7, 4)
#define AUX_TIMEOUT		BIT(1)
#define AUX_BUSY		BIT(0)
>>>>>>> upstream/android-13
#define DP0_AUXI2CADR		0x0698

/* Link Training */
#define DP0_SRCCTRL		0x06a0
#define DP0_SRCCTRL_SCRMBLDIS		BIT(13)
#define DP0_SRCCTRL_EN810B		BIT(12)
#define DP0_SRCCTRL_NOTP		(0 << 8)
#define DP0_SRCCTRL_TP1			(1 << 8)
#define DP0_SRCCTRL_TP2			(2 << 8)
#define DP0_SRCCTRL_LANESKEW		BIT(7)
#define DP0_SRCCTRL_SSCG		BIT(3)
#define DP0_SRCCTRL_LANES_1		(0 << 2)
#define DP0_SRCCTRL_LANES_2		(1 << 2)
#define DP0_SRCCTRL_BW27		(1 << 1)
#define DP0_SRCCTRL_BW162		(0 << 1)
#define DP0_SRCCTRL_AUTOCORRECT		BIT(0)
#define DP0_LTSTAT		0x06d0
#define LT_LOOPDONE			BIT(13)
#define LT_STATUS_MASK			(0x1f << 8)
#define LT_CHANNEL1_EQ_BITS		(DP_CHANNEL_EQ_BITS << 4)
#define LT_INTERLANE_ALIGN_DONE		BIT(3)
#define LT_CHANNEL0_EQ_BITS		(DP_CHANNEL_EQ_BITS)
#define DP0_SNKLTCHGREQ		0x06d4
#define DP0_LTLOOPCTRL		0x06d8
#define DP0_SNKLTCTRL		0x06e4

#define DP1_SRCCTRL		0x07a0

/* PHY */
#define DP_PHY_CTRL		0x0800
#define DP_PHY_RST			BIT(28)  /* DP PHY Global Soft Reset */
#define BGREN				BIT(25)  /* AUX PHY BGR Enable */
#define PWR_SW_EN			BIT(24)  /* PHY Power Switch Enable */
#define PHY_M1_RST			BIT(12)  /* Reset PHY1 Main Channel */
#define PHY_RDY				BIT(16)  /* PHY Main Channels Ready */
#define PHY_M0_RST			BIT(8)   /* Reset PHY0 Main Channel */
#define PHY_2LANE			BIT(2)   /* PHY Enable 2 lanes */
#define PHY_A0_EN			BIT(1)   /* PHY Aux Channel0 Enable */
#define PHY_M0_EN			BIT(0)   /* PHY Main Channel0 Enable */

/* PLL */
#define DP0_PLLCTRL		0x0900
#define DP1_PLLCTRL		0x0904	/* not defined in DS */
#define PXL_PLLCTRL		0x0908
#define PLLUPDATE			BIT(2)
#define PLLBYP				BIT(1)
#define PLLEN				BIT(0)
#define PXL_PLLPARAM		0x0914
#define IN_SEL_REFCLK			(0 << 14)
#define SYS_PLLPARAM		0x0918
#define REF_FREQ_38M4			(0 << 8) /* 38.4 MHz */
#define REF_FREQ_19M2			(1 << 8) /* 19.2 MHz */
#define REF_FREQ_26M			(2 << 8) /* 26 MHz */
#define REF_FREQ_13M			(3 << 8) /* 13 MHz */
#define SYSCLK_SEL_LSCLK		(0 << 4)
#define LSCLK_DIV_1			(0 << 0)
#define LSCLK_DIV_2			(1 << 0)

/* Test & Debug */
#define TSTCTL			0x0a00
<<<<<<< HEAD
=======
#define COLOR_R			GENMASK(31, 24)
#define COLOR_G			GENMASK(23, 16)
#define COLOR_B			GENMASK(15, 8)
#define ENI2CFILTER		BIT(4)
#define COLOR_BAR_MODE		GENMASK(1, 0)
#define COLOR_BAR_MODE_BARS	2
>>>>>>> upstream/android-13
#define PLL_DBG			0x0a04

static bool tc_test_pattern;
module_param_named(test, tc_test_pattern, bool, 0644);

struct tc_edp_link {
<<<<<<< HEAD
	struct drm_dp_link	base;
	u8			assr;
	int			scrambler_dis;
	int			spread;
	int			coding8b10b;
	u8			swing;
	u8			preemp;
=======
	u8			dpcd[DP_RECEIVER_CAP_SIZE];
	unsigned int		rate;
	u8			num_lanes;
	u8			assr;
	bool			scrambler_dis;
	bool			spread;
>>>>>>> upstream/android-13
};

struct tc_data {
	struct device		*dev;
	struct regmap		*regmap;
	struct drm_dp_aux	aux;

	struct drm_bridge	bridge;
<<<<<<< HEAD
	struct drm_connector	connector;
	struct drm_panel	*panel;
=======
	struct drm_bridge	*panel_bridge;
	struct drm_connector	connector;
>>>>>>> upstream/android-13

	/* link settings */
	struct tc_edp_link	link;

<<<<<<< HEAD
	/* display edid */
	struct edid		*edid;
	/* current mode */
	struct drm_display_mode	*mode;
=======
	/* current mode */
	struct drm_display_mode	mode;
>>>>>>> upstream/android-13

	u32			rev;
	u8			assr;

	struct gpio_desc	*sd_gpio;
	struct gpio_desc	*reset_gpio;
	struct clk		*refclk;
<<<<<<< HEAD
=======

	/* do we have IRQ */
	bool			have_irq;

	/* HPD pin number (0 or 1) or -ENODEV */
	int			hpd_pin;
>>>>>>> upstream/android-13
};

static inline struct tc_data *aux_to_tc(struct drm_dp_aux *a)
{
	return container_of(a, struct tc_data, aux);
}

static inline struct tc_data *bridge_to_tc(struct drm_bridge *b)
{
	return container_of(b, struct tc_data, bridge);
}

static inline struct tc_data *connector_to_tc(struct drm_connector *c)
{
	return container_of(c, struct tc_data, connector);
}

<<<<<<< HEAD
/* Simple macros to avoid repeated error checks */
#define tc_write(reg, var)					\
	do {							\
		ret = regmap_write(tc->regmap, reg, var);	\
		if (ret)					\
			goto err;				\
	} while (0)
#define tc_read(reg, var)					\
	do {							\
		ret = regmap_read(tc->regmap, reg, var);	\
		if (ret)					\
			goto err;				\
	} while (0)

static inline int tc_poll_timeout(struct regmap *map, unsigned int addr,
=======
static inline int tc_poll_timeout(struct tc_data *tc, unsigned int addr,
>>>>>>> upstream/android-13
				  unsigned int cond_mask,
				  unsigned int cond_value,
				  unsigned long sleep_us, u64 timeout_us)
{
<<<<<<< HEAD
	ktime_t timeout = ktime_add_us(ktime_get(), timeout_us);
	unsigned int val;
	int ret;

	for (;;) {
		ret = regmap_read(map, addr, &val);
		if (ret)
			break;
		if ((val & cond_mask) == cond_value)
			break;
		if (timeout_us && ktime_compare(ktime_get(), timeout) > 0) {
			ret = regmap_read(map, addr, &val);
			break;
		}
		if (sleep_us)
			usleep_range((sleep_us >> 2) + 1, sleep_us);
	}
	return ret ?: (((val & cond_mask) == cond_value) ? 0 : -ETIMEDOUT);
}

static int tc_aux_wait_busy(struct tc_data *tc, unsigned int timeout_ms)
{
	return tc_poll_timeout(tc->regmap, DP0_AUXSTATUS, AUX_BUSY, 0,
			       1000, 1000 * timeout_ms);
}

static int tc_aux_get_status(struct tc_data *tc, u8 *reply)
{
	int ret;
	u32 value;

	ret = regmap_read(tc->regmap, DP0_AUXSTATUS, &value);
	if (ret < 0)
		return ret;
	if (value & AUX_BUSY) {
		if (value & AUX_TIMEOUT) {
			dev_err(tc->dev, "i2c access timeout!\n");
			return -ETIMEDOUT;
		}
		return -EBUSY;
	}

	*reply = (value & AUX_STATUS_MASK) >> AUX_STATUS_SHIFT;
	return 0;
=======
	unsigned int val;

	return regmap_read_poll_timeout(tc->regmap, addr, val,
					(val & cond_mask) == cond_value,
					sleep_us, timeout_us);
}

static int tc_aux_wait_busy(struct tc_data *tc)
{
	return tc_poll_timeout(tc, DP0_AUXSTATUS, AUX_BUSY, 0, 100, 100000);
}

static int tc_aux_write_data(struct tc_data *tc, const void *data,
			     size_t size)
{
	u32 auxwdata[DP_AUX_MAX_PAYLOAD_BYTES / sizeof(u32)] = { 0 };
	int ret, count = ALIGN(size, sizeof(u32));

	memcpy(auxwdata, data, size);

	ret = regmap_raw_write(tc->regmap, DP0_AUXWDATA(0), auxwdata, count);
	if (ret)
		return ret;

	return size;
}

static int tc_aux_read_data(struct tc_data *tc, void *data, size_t size)
{
	u32 auxrdata[DP_AUX_MAX_PAYLOAD_BYTES / sizeof(u32)];
	int ret, count = ALIGN(size, sizeof(u32));

	ret = regmap_raw_read(tc->regmap, DP0_AUXRDATA(0), auxrdata, count);
	if (ret)
		return ret;

	memcpy(data, auxrdata, size);

	return size;
}

static u32 tc_auxcfg0(struct drm_dp_aux_msg *msg, size_t size)
{
	u32 auxcfg0 = msg->request;

	if (size)
		auxcfg0 |= FIELD_PREP(DP0_AUXCFG0_BSIZE, size - 1);
	else
		auxcfg0 |= DP0_AUXCFG0_ADDR_ONLY;

	return auxcfg0;
>>>>>>> upstream/android-13
}

static ssize_t tc_aux_transfer(struct drm_dp_aux *aux,
			       struct drm_dp_aux_msg *msg)
{
	struct tc_data *tc = aux_to_tc(aux);
	size_t size = min_t(size_t, DP_AUX_MAX_PAYLOAD_BYTES - 1, msg->size);
	u8 request = msg->request & ~DP_AUX_I2C_MOT;
<<<<<<< HEAD
	u8 *buf = msg->buffer;
	u32 tmp = 0;
	int i = 0;
	int ret;

	if (size == 0)
		return 0;

	ret = tc_aux_wait_busy(tc, 100);
	if (ret)
		goto err;

	if (request == DP_AUX_I2C_WRITE || request == DP_AUX_NATIVE_WRITE) {
		/* Store data */
		while (i < size) {
			if (request == DP_AUX_NATIVE_WRITE)
				tmp = tmp | (buf[i] << (8 * (i & 0x3)));
			else
				tmp = (tmp << 8) | buf[i];
			i++;
			if (((i % 4) == 0) || (i == size)) {
				tc_write(DP0_AUXWDATA((i - 1) >> 2), tmp);
				tmp = 0;
			}
		}
	} else if (request != DP_AUX_I2C_READ &&
		   request != DP_AUX_NATIVE_READ) {
=======
	u32 auxstatus;
	int ret;

	ret = tc_aux_wait_busy(tc);
	if (ret)
		return ret;

	switch (request) {
	case DP_AUX_NATIVE_READ:
	case DP_AUX_I2C_READ:
		break;
	case DP_AUX_NATIVE_WRITE:
	case DP_AUX_I2C_WRITE:
		if (size) {
			ret = tc_aux_write_data(tc, msg->buffer, size);
			if (ret < 0)
				return ret;
		}
		break;
	default:
>>>>>>> upstream/android-13
		return -EINVAL;
	}

	/* Store address */
<<<<<<< HEAD
	tc_write(DP0_AUXADDR, msg->address);
	/* Start transfer */
	tc_write(DP0_AUXCFG0, ((size - 1) << 8) | request);

	ret = tc_aux_wait_busy(tc, 100);
	if (ret)
		goto err;

	ret = tc_aux_get_status(tc, &msg->reply);
	if (ret)
		goto err;

	if (request == DP_AUX_I2C_READ || request == DP_AUX_NATIVE_READ) {
		/* Read data */
		while (i < size) {
			if ((i % 4) == 0)
				tc_read(DP0_AUXRDATA(i >> 2), &tmp);
			buf[i] = tmp & 0xff;
			tmp = tmp >> 8;
			i++;
		}
	}

	return size;
err:
	return ret;
=======
	ret = regmap_write(tc->regmap, DP0_AUXADDR, msg->address);
	if (ret)
		return ret;
	/* Start transfer */
	ret = regmap_write(tc->regmap, DP0_AUXCFG0, tc_auxcfg0(msg, size));
	if (ret)
		return ret;

	ret = tc_aux_wait_busy(tc);
	if (ret)
		return ret;

	ret = regmap_read(tc->regmap, DP0_AUXSTATUS, &auxstatus);
	if (ret)
		return ret;

	if (auxstatus & AUX_TIMEOUT)
		return -ETIMEDOUT;
	/*
	 * For some reason address-only DP_AUX_I2C_WRITE (MOT), still
	 * reports 1 byte transferred in its status. To deal we that
	 * we ignore aux_bytes field if we know that this was an
	 * address-only transfer
	 */
	if (size)
		size = FIELD_GET(AUX_BYTES, auxstatus);
	msg->reply = FIELD_GET(AUX_STATUS, auxstatus);

	switch (request) {
	case DP_AUX_NATIVE_READ:
	case DP_AUX_I2C_READ:
		if (size)
			return tc_aux_read_data(tc, msg->buffer, size);
		break;
	}

	return size;
>>>>>>> upstream/android-13
}

static const char * const training_pattern1_errors[] = {
	"No errors",
	"Aux write error",
	"Aux read error",
	"Max voltage reached error",
	"Loop counter expired error",
	"res", "res", "res"
};

static const char * const training_pattern2_errors[] = {
	"No errors",
	"Aux write error",
	"Aux read error",
	"Clock recovery failed error",
	"Loop counter expired error",
	"res", "res", "res"
};

static u32 tc_srcctrl(struct tc_data *tc)
{
	/*
	 * No training pattern, skew lane 1 data by two LSCLK cycles with
	 * respect to lane 0 data, AutoCorrect Mode = 0
	 */
<<<<<<< HEAD
	u32 reg = DP0_SRCCTRL_NOTP | DP0_SRCCTRL_LANESKEW;

	if (tc->link.scrambler_dis)
		reg |= DP0_SRCCTRL_SCRMBLDIS;	/* Scrambler Disabled */
	if (tc->link.coding8b10b)
		/* Enable 8/10B Encoder (TxData[19:16] not used) */
		reg |= DP0_SRCCTRL_EN810B;
	if (tc->link.spread)
		reg |= DP0_SRCCTRL_SSCG;	/* Spread Spectrum Enable */
	if (tc->link.base.num_lanes == 2)
		reg |= DP0_SRCCTRL_LANES_2;	/* Two Main Channel Lanes */
	if (tc->link.base.rate != 162000)
=======
	u32 reg = DP0_SRCCTRL_NOTP | DP0_SRCCTRL_LANESKEW | DP0_SRCCTRL_EN810B;

	if (tc->link.scrambler_dis)
		reg |= DP0_SRCCTRL_SCRMBLDIS;	/* Scrambler Disabled */
	if (tc->link.spread)
		reg |= DP0_SRCCTRL_SSCG;	/* Spread Spectrum Enable */
	if (tc->link.num_lanes == 2)
		reg |= DP0_SRCCTRL_LANES_2;	/* Two Main Channel Lanes */
	if (tc->link.rate != 162000)
>>>>>>> upstream/android-13
		reg |= DP0_SRCCTRL_BW27;	/* 2.7 Gbps link */
	return reg;
}

<<<<<<< HEAD
static void tc_wait_pll_lock(struct tc_data *tc)
{
	/* Wait for PLL to lock: up to 2.09 ms, depending on refclk */
	usleep_range(3000, 6000);
=======
static int tc_pllupdate(struct tc_data *tc, unsigned int pllctrl)
{
	int ret;

	ret = regmap_write(tc->regmap, pllctrl, PLLUPDATE | PLLEN);
	if (ret)
		return ret;

	/* Wait for PLL to lock: up to 2.09 ms, depending on refclk */
	usleep_range(3000, 6000);

	return 0;
>>>>>>> upstream/android-13
}

static int tc_pxl_pll_en(struct tc_data *tc, u32 refclk, u32 pixelclock)
{
	int ret;
	int i_pre, best_pre = 1;
	int i_post, best_post = 1;
	int div, best_div = 1;
	int mul, best_mul = 1;
	int delta, best_delta;
	int ext_div[] = {1, 2, 3, 5, 7};
	int best_pixelclock = 0;
	int vco_hi = 0;
<<<<<<< HEAD
=======
	u32 pxl_pllparam;
>>>>>>> upstream/android-13

	dev_dbg(tc->dev, "PLL: requested %d pixelclock, ref %d\n", pixelclock,
		refclk);
	best_delta = pixelclock;
	/* Loop over all possible ext_divs, skipping invalid configurations */
	for (i_pre = 0; i_pre < ARRAY_SIZE(ext_div); i_pre++) {
		/*
		 * refclk / ext_pre_div should be in the 1 to 200 MHz range.
		 * We don't allow any refclk > 200 MHz, only check lower bounds.
		 */
		if (refclk / ext_div[i_pre] < 1000000)
			continue;
		for (i_post = 0; i_post < ARRAY_SIZE(ext_div); i_post++) {
			for (div = 1; div <= 16; div++) {
				u32 clk;
				u64 tmp;

				tmp = pixelclock * ext_div[i_pre] *
				      ext_div[i_post] * div;
				do_div(tmp, refclk);
				mul = tmp;

				/* Check limits */
				if ((mul < 1) || (mul > 128))
					continue;

				clk = (refclk / ext_div[i_pre] / div) * mul;
				/*
				 * refclk * mul / (ext_pre_div * pre_div)
				 * should be in the 150 to 650 MHz range
				 */
				if ((clk > 650000000) || (clk < 150000000))
					continue;

				clk = clk / ext_div[i_post];
				delta = clk - pixelclock;

				if (abs(delta) < abs(best_delta)) {
					best_pre = i_pre;
					best_post = i_post;
					best_div = div;
					best_mul = mul;
					best_delta = delta;
					best_pixelclock = clk;
				}
			}
		}
	}
	if (best_pixelclock == 0) {
		dev_err(tc->dev, "Failed to calc clock for %d pixelclock\n",
			pixelclock);
		return -EINVAL;
	}

	dev_dbg(tc->dev, "PLL: got %d, delta %d\n", best_pixelclock,
		best_delta);
	dev_dbg(tc->dev, "PLL: %d / %d / %d * %d / %d\n", refclk,
		ext_div[best_pre], best_div, best_mul, ext_div[best_post]);

	/* if VCO >= 300 MHz */
	if (refclk / ext_div[best_pre] / best_div * best_mul >= 300000000)
		vco_hi = 1;
	/* see DS */
	if (best_div == 16)
		best_div = 0;
	if (best_mul == 128)
		best_mul = 0;

	/* Power up PLL and switch to bypass */
<<<<<<< HEAD
	tc_write(PXL_PLLCTRL, PLLBYP | PLLEN);

	tc_write(PXL_PLLPARAM,
		 (vco_hi << 24) |		/* For PLL VCO >= 300 MHz = 1 */
		 (ext_div[best_pre] << 20) |	/* External Pre-divider */
		 (ext_div[best_post] << 16) |	/* External Post-divider */
		 IN_SEL_REFCLK |		/* Use RefClk as PLL input */
		 (best_div << 8) |		/* Divider for PLL RefClk */
		 (best_mul << 0));		/* Multiplier for PLL */

	/* Force PLL parameter update and disable bypass */
	tc_write(PXL_PLLCTRL, PLLUPDATE | PLLEN);

	tc_wait_pll_lock(tc);

	return 0;
err:
	return ret;
=======
	ret = regmap_write(tc->regmap, PXL_PLLCTRL, PLLBYP | PLLEN);
	if (ret)
		return ret;

	pxl_pllparam  = vco_hi << 24; /* For PLL VCO >= 300 MHz = 1 */
	pxl_pllparam |= ext_div[best_pre] << 20; /* External Pre-divider */
	pxl_pllparam |= ext_div[best_post] << 16; /* External Post-divider */
	pxl_pllparam |= IN_SEL_REFCLK; /* Use RefClk as PLL input */
	pxl_pllparam |= best_div << 8; /* Divider for PLL RefClk */
	pxl_pllparam |= best_mul; /* Multiplier for PLL */

	ret = regmap_write(tc->regmap, PXL_PLLPARAM, pxl_pllparam);
	if (ret)
		return ret;

	/* Force PLL parameter update and disable bypass */
	return tc_pllupdate(tc, PXL_PLLCTRL);
>>>>>>> upstream/android-13
}

static int tc_pxl_pll_dis(struct tc_data *tc)
{
	/* Enable PLL bypass, power down PLL */
	return regmap_write(tc->regmap, PXL_PLLCTRL, PLLBYP);
}

static int tc_stream_clock_calc(struct tc_data *tc)
{
<<<<<<< HEAD
	int ret;
=======
>>>>>>> upstream/android-13
	/*
	 * If the Stream clock and Link Symbol clock are
	 * asynchronous with each other, the value of M changes over
	 * time. This way of generating link clock and stream
	 * clock is called Asynchronous Clock mode. The value M
	 * must change while the value N stays constant. The
	 * value of N in this Asynchronous Clock mode must be set
	 * to 2^15 or 32,768.
	 *
	 * LSCLK = 1/10 of high speed link clock
	 *
	 * f_STRMCLK = M/N * f_LSCLK
	 * M/N = f_STRMCLK / f_LSCLK
	 *
	 */
<<<<<<< HEAD
	tc_write(DP0_VIDMNGEN1, 32768);

	return 0;
err:
	return ret;
}

static int tc_aux_link_setup(struct tc_data *tc)
{
	unsigned long rate;
	u32 value;
	int ret;
	u32 dp_phy_ctrl;
=======
	return regmap_write(tc->regmap, DP0_VIDMNGEN1, 32768);
}

static int tc_set_syspllparam(struct tc_data *tc)
{
	unsigned long rate;
	u32 pllparam = SYSCLK_SEL_LSCLK | LSCLK_DIV_2;
>>>>>>> upstream/android-13

	rate = clk_get_rate(tc->refclk);
	switch (rate) {
	case 38400000:
<<<<<<< HEAD
		value = REF_FREQ_38M4;
		break;
	case 26000000:
		value = REF_FREQ_26M;
		break;
	case 19200000:
		value = REF_FREQ_19M2;
		break;
	case 13000000:
		value = REF_FREQ_13M;
=======
		pllparam |= REF_FREQ_38M4;
		break;
	case 26000000:
		pllparam |= REF_FREQ_26M;
		break;
	case 19200000:
		pllparam |= REF_FREQ_19M2;
		break;
	case 13000000:
		pllparam |= REF_FREQ_13M;
>>>>>>> upstream/android-13
		break;
	default:
		dev_err(tc->dev, "Invalid refclk rate: %lu Hz\n", rate);
		return -EINVAL;
	}

<<<<<<< HEAD
	/* Setup DP-PHY / PLL */
	value |= SYSCLK_SEL_LSCLK | LSCLK_DIV_2;
	tc_write(SYS_PLLPARAM, value);

	dp_phy_ctrl = BGREN | PWR_SW_EN | PHY_A0_EN;
	if (tc->link.base.num_lanes == 2)
		dp_phy_ctrl |= PHY_2LANE;
	tc_write(DP_PHY_CTRL, dp_phy_ctrl);

=======
	return regmap_write(tc->regmap, SYS_PLLPARAM, pllparam);
}

static int tc_aux_link_setup(struct tc_data *tc)
{
	int ret;
	u32 dp0_auxcfg1;

	/* Setup DP-PHY / PLL */
	ret = tc_set_syspllparam(tc);
	if (ret)
		goto err;

	ret = regmap_write(tc->regmap, DP_PHY_CTRL,
			   BGREN | PWR_SW_EN | PHY_A0_EN);
	if (ret)
		goto err;
>>>>>>> upstream/android-13
	/*
	 * Initially PLLs are in bypass. Force PLL parameter update,
	 * disable PLL bypass, enable PLL
	 */
<<<<<<< HEAD
	tc_write(DP0_PLLCTRL, PLLUPDATE | PLLEN);
	tc_wait_pll_lock(tc);

	tc_write(DP1_PLLCTRL, PLLUPDATE | PLLEN);
	tc_wait_pll_lock(tc);

	ret = tc_poll_timeout(tc->regmap, DP_PHY_CTRL, PHY_RDY, PHY_RDY, 1,
			      1000);
	if (ret == -ETIMEDOUT) {
		dev_err(tc->dev, "Timeout waiting for PHY to become ready");
		return ret;
	} else if (ret)
		goto err;

	/* Setup AUX link */
	tc_write(DP0_AUXCFG1, AUX_RX_FILTER_EN |
		 (0x06 << 8) |	/* Aux Bit Period Calculator Threshold */
		 (0x3f << 0));	/* Aux Response Timeout Timer */
=======
	ret = tc_pllupdate(tc, DP0_PLLCTRL);
	if (ret)
		goto err;

	ret = tc_pllupdate(tc, DP1_PLLCTRL);
	if (ret)
		goto err;

	ret = tc_poll_timeout(tc, DP_PHY_CTRL, PHY_RDY, PHY_RDY, 100, 100000);
	if (ret == -ETIMEDOUT) {
		dev_err(tc->dev, "Timeout waiting for PHY to become ready");
		return ret;
	} else if (ret) {
		goto err;
	}

	/* Setup AUX link */
	dp0_auxcfg1  = AUX_RX_FILTER_EN;
	dp0_auxcfg1 |= 0x06 << 8; /* Aux Bit Period Calculator Threshold */
	dp0_auxcfg1 |= 0x3f << 0; /* Aux Response Timeout Timer */

	ret = regmap_write(tc->regmap, DP0_AUXCFG1, dp0_auxcfg1);
	if (ret)
		goto err;
>>>>>>> upstream/android-13

	return 0;
err:
	dev_err(tc->dev, "tc_aux_link_setup failed: %d\n", ret);
	return ret;
}

static int tc_get_display_props(struct tc_data *tc)
{
<<<<<<< HEAD
	int ret;
	/* temp buffer */
	u8 tmp[8];

	/* Read DP Rx Link Capability */
	ret = drm_dp_link_probe(&tc->aux, &tc->link.base);
	if (ret < 0)
		goto err_dpcd_read;
	if (tc->link.base.rate != 162000 && tc->link.base.rate != 270000) {
		dev_dbg(tc->dev, "Falling to 2.7 Gbps rate\n");
		tc->link.base.rate = 270000;
	}

	if (tc->link.base.num_lanes > 2) {
		dev_dbg(tc->dev, "Falling to 2 lanes\n");
		tc->link.base.num_lanes = 2;
	}

	ret = drm_dp_dpcd_readb(&tc->aux, DP_MAX_DOWNSPREAD, tmp);
	if (ret < 0)
		goto err_dpcd_read;
	tc->link.spread = tmp[0] & BIT(0); /* 0.5% down spread */

	ret = drm_dp_dpcd_readb(&tc->aux, DP_MAIN_LINK_CHANNEL_CODING, tmp);
	if (ret < 0)
		goto err_dpcd_read;
	tc->link.coding8b10b = tmp[0] & BIT(0);
	tc->link.scrambler_dis = 0;
	/* read assr */
	ret = drm_dp_dpcd_readb(&tc->aux, DP_EDP_CONFIGURATION_SET, tmp);
	if (ret < 0)
		goto err_dpcd_read;
	tc->link.assr = tmp[0] & DP_ALTERNATE_SCRAMBLER_RESET_ENABLE;

	dev_dbg(tc->dev, "DPCD rev: %d.%d, rate: %s, lanes: %d, framing: %s\n",
		tc->link.base.revision >> 4, tc->link.base.revision & 0x0f,
		(tc->link.base.rate == 162000) ? "1.62Gbps" : "2.7Gbps",
		tc->link.base.num_lanes,
		(tc->link.base.capabilities & DP_LINK_CAP_ENHANCED_FRAMING) ?
		"enhanced" : "non-enhanced");
	dev_dbg(tc->dev, "ANSI 8B/10B: %d\n", tc->link.coding8b10b);
=======
	u8 revision, num_lanes;
	unsigned int rate;
	int ret;
	u8 reg;

	/* Read DP Rx Link Capability */
	ret = drm_dp_dpcd_read(&tc->aux, DP_DPCD_REV, tc->link.dpcd,
			       DP_RECEIVER_CAP_SIZE);
	if (ret < 0)
		goto err_dpcd_read;

	revision = tc->link.dpcd[DP_DPCD_REV];
	rate = drm_dp_max_link_rate(tc->link.dpcd);
	num_lanes = drm_dp_max_lane_count(tc->link.dpcd);

	if (rate != 162000 && rate != 270000) {
		dev_dbg(tc->dev, "Falling to 2.7 Gbps rate\n");
		rate = 270000;
	}

	tc->link.rate = rate;

	if (num_lanes > 2) {
		dev_dbg(tc->dev, "Falling to 2 lanes\n");
		num_lanes = 2;
	}

	tc->link.num_lanes = num_lanes;

	ret = drm_dp_dpcd_readb(&tc->aux, DP_MAX_DOWNSPREAD, &reg);
	if (ret < 0)
		goto err_dpcd_read;
	tc->link.spread = reg & DP_MAX_DOWNSPREAD_0_5;

	ret = drm_dp_dpcd_readb(&tc->aux, DP_MAIN_LINK_CHANNEL_CODING, &reg);
	if (ret < 0)
		goto err_dpcd_read;

	tc->link.scrambler_dis = false;
	/* read assr */
	ret = drm_dp_dpcd_readb(&tc->aux, DP_EDP_CONFIGURATION_SET, &reg);
	if (ret < 0)
		goto err_dpcd_read;
	tc->link.assr = reg & DP_ALTERNATE_SCRAMBLER_RESET_ENABLE;

	dev_dbg(tc->dev, "DPCD rev: %d.%d, rate: %s, lanes: %d, framing: %s\n",
		revision >> 4, revision & 0x0f,
		(tc->link.rate == 162000) ? "1.62Gbps" : "2.7Gbps",
		tc->link.num_lanes,
		drm_dp_enhanced_frame_cap(tc->link.dpcd) ?
		"enhanced" : "default");
	dev_dbg(tc->dev, "Downspread: %s, scrambler: %s\n",
		tc->link.spread ? "0.5%" : "0.0%",
		tc->link.scrambler_dis ? "disabled" : "enabled");
>>>>>>> upstream/android-13
	dev_dbg(tc->dev, "Display ASSR: %d, TC358767 ASSR: %d\n",
		tc->link.assr, tc->assr);

	return 0;

err_dpcd_read:
	dev_err(tc->dev, "failed to read DPCD: %d\n", ret);
	return ret;
}

<<<<<<< HEAD
static int tc_set_video_mode(struct tc_data *tc, struct drm_display_mode *mode)
=======
static int tc_set_video_mode(struct tc_data *tc,
			     const struct drm_display_mode *mode)
>>>>>>> upstream/android-13
{
	int ret;
	int vid_sync_dly;
	int max_tu_symbol;

	int left_margin = mode->htotal - mode->hsync_end;
	int right_margin = mode->hsync_start - mode->hdisplay;
	int hsync_len = mode->hsync_end - mode->hsync_start;
	int upper_margin = mode->vtotal - mode->vsync_end;
	int lower_margin = mode->vsync_start - mode->vdisplay;
	int vsync_len = mode->vsync_end - mode->vsync_start;
<<<<<<< HEAD
=======
	u32 dp0_syncval;
	u32 bits_per_pixel = 24;
	u32 in_bw, out_bw;
>>>>>>> upstream/android-13

	/*
	 * Recommended maximum number of symbols transferred in a transfer unit:
	 * DIV_ROUND_UP((input active video bandwidth in bytes) * tu_size,
	 *              (output active video bandwidth in bytes))
	 * Must be less than tu_size.
	 */
<<<<<<< HEAD
	max_tu_symbol = TU_SIZE_RECOMMENDED - 1;
=======

	in_bw = mode->clock * bits_per_pixel / 8;
	out_bw = tc->link.num_lanes * tc->link.rate;
	max_tu_symbol = DIV_ROUND_UP(in_bw * TU_SIZE_RECOMMENDED, out_bw);
>>>>>>> upstream/android-13

	dev_dbg(tc->dev, "set mode %dx%d\n",
		mode->hdisplay, mode->vdisplay);
	dev_dbg(tc->dev, "H margin %d,%d sync %d\n",
		left_margin, right_margin, hsync_len);
	dev_dbg(tc->dev, "V margin %d,%d sync %d\n",
		upper_margin, lower_margin, vsync_len);
	dev_dbg(tc->dev, "total: %dx%d\n", mode->htotal, mode->vtotal);


	/*
	 * LCD Ctl Frame Size
	 * datasheet is not clear of vsdelay in case of DPI
	 * assume we do not need any delay when DPI is a source of
	 * sync signals
	 */
<<<<<<< HEAD
	tc_write(VPCTRL0, (0 << 20) /* VSDELAY */ |
		 OPXLFMT_RGB888 | FRMSYNC_DISABLED | MSF_DISABLED);
	tc_write(HTIM01, (ALIGN(left_margin, 2) << 16) | /* H back porch */
			 (ALIGN(hsync_len, 2) << 0));	 /* Hsync */
	tc_write(HTIM02, (ALIGN(right_margin, 2) << 16) |  /* H front porch */
			 (ALIGN(mode->hdisplay, 2) << 0)); /* width */
	tc_write(VTIM01, (upper_margin << 16) |		/* V back porch */
			 (vsync_len << 0));		/* Vsync */
	tc_write(VTIM02, (lower_margin << 16) |		/* V front porch */
			 (mode->vdisplay << 0));	/* height */
	tc_write(VFUEN0, VFUEN);		/* update settings */

	/* Test pattern settings */
	tc_write(TSTCTL,
		 (120 << 24) |	/* Red Color component value */
		 (20 << 16) |	/* Green Color component value */
		 (99 << 8) |	/* Blue Color component value */
		 (1 << 4) |	/* Enable I2C Filter */
		 (2 << 0) |	/* Color bar Mode */
		 0);

	/* DP Main Stream Attributes */
	vid_sync_dly = hsync_len + left_margin + mode->hdisplay;
	tc_write(DP0_VIDSYNCDELAY,
		 (max_tu_symbol << 16) |	/* thresh_dly */
		 (vid_sync_dly << 0));

	tc_write(DP0_TOTALVAL, (mode->vtotal << 16) | (mode->htotal));

	tc_write(DP0_STARTVAL,
		 ((upper_margin + vsync_len) << 16) |
		 ((left_margin + hsync_len) << 0));

	tc_write(DP0_ACTIVEVAL, (mode->vdisplay << 16) | (mode->hdisplay));

	tc_write(DP0_SYNCVAL, (vsync_len << 16) | (hsync_len << 0) |
		 ((mode->flags & DRM_MODE_FLAG_NHSYNC) ? SYNCVAL_HS_POL_ACTIVE_LOW : 0) |
		 ((mode->flags & DRM_MODE_FLAG_NVSYNC) ? SYNCVAL_VS_POL_ACTIVE_LOW : 0));

	tc_write(DPIPXLFMT, VS_POL_ACTIVE_LOW | HS_POL_ACTIVE_LOW |
		 DE_POL_ACTIVE_HIGH | SUB_CFG_TYPE_CONFIG1 | DPI_BPP_RGB888);

	tc_write(DP0_MISC, (max_tu_symbol << 23) | (TU_SIZE_RECOMMENDED << 16) |
			   BPC_8);

	return 0;
err:
	return ret;
}

static int tc_link_training(struct tc_data *tc, int pattern)
{
	const char * const *errors;
	u32 srcctrl = tc_srcctrl(tc) | DP0_SRCCTRL_SCRMBLDIS |
		      DP0_SRCCTRL_AUTOCORRECT;
	int timeout;
	int retry;
	u32 value;
	int ret;

	if (pattern == DP_TRAINING_PATTERN_1) {
		srcctrl |= DP0_SRCCTRL_TP1;
		errors = training_pattern1_errors;
	} else {
		srcctrl |= DP0_SRCCTRL_TP2;
		errors = training_pattern2_errors;
	}

	/* Set DPCD 0x102 for Training Part 1 or 2 */
	tc_write(DP0_SNKLTCTRL, DP_LINK_SCRAMBLING_DISABLE | pattern);

	tc_write(DP0_LTLOOPCTRL,
		 (0x0f << 28) |	/* Defer Iteration Count */
		 (0x0f << 24) |	/* Loop Iteration Count */
		 (0x0d << 0));	/* Loop Timer Delay */

	retry = 5;
	do {
		/* Set DP0 Training Pattern */
		tc_write(DP0_SRCCTRL, srcctrl);

		/* Enable DP0 to start Link Training */
		tc_write(DP0CTL, DP_EN);

		/* wait */
		timeout = 1000;
		do {
			tc_read(DP0_LTSTAT, &value);
			udelay(1);
		} while ((!(value & LT_LOOPDONE)) && (--timeout));
		if (timeout == 0) {
			dev_err(tc->dev, "Link training timeout!\n");
		} else {
			int pattern = (value >> 11) & 0x3;
			int error = (value >> 8) & 0x7;

			dev_dbg(tc->dev,
				"Link training phase %d done after %d uS: %s\n",
				pattern, 1000 - timeout, errors[error]);
			if (pattern == DP_TRAINING_PATTERN_1 && error == 0)
				break;
			if (pattern == DP_TRAINING_PATTERN_2) {
				value &= LT_CHANNEL1_EQ_BITS |
					 LT_INTERLANE_ALIGN_DONE |
					 LT_CHANNEL0_EQ_BITS;
				/* in case of two lanes */
				if ((tc->link.base.num_lanes == 2) &&
				    (value == (LT_CHANNEL1_EQ_BITS |
					       LT_INTERLANE_ALIGN_DONE |
					       LT_CHANNEL0_EQ_BITS)))
					break;
				/* in case of one line */
				if ((tc->link.base.num_lanes == 1) &&
				    (value == (LT_INTERLANE_ALIGN_DONE |
					       LT_CHANNEL0_EQ_BITS)))
					break;
			}
		}
		/* restart */
		tc_write(DP0CTL, 0);
		usleep_range(10, 20);
	} while (--retry);
	if (retry == 0) {
		dev_err(tc->dev, "Failed to finish training phase %d\n",
			pattern);
	}

	return 0;
err:
	return ret;
}

static int tc_main_link_setup(struct tc_data *tc)
{
	struct drm_dp_aux *aux = &tc->aux;
	struct device *dev = tc->dev;
	unsigned int rate;
	u32 dp_phy_ctrl;
	int timeout;
	u32 value;
	int ret;
	u8 tmp[8];

	/* display mode should be set at this point */
	if (!tc->mode)
		return -EINVAL;

	tc_write(DP0_SRCCTRL, tc_srcctrl(tc));
	/* SSCG and BW27 on DP1 must be set to the same as on DP0 */
	tc_write(DP1_SRCCTRL,
		 (tc->link.spread ? DP0_SRCCTRL_SSCG : 0) |
		 ((tc->link.base.rate != 162000) ? DP0_SRCCTRL_BW27 : 0));

	rate = clk_get_rate(tc->refclk);
	switch (rate) {
	case 38400000:
		value = REF_FREQ_38M4;
		break;
	case 26000000:
		value = REF_FREQ_26M;
		break;
	case 19200000:
		value = REF_FREQ_19M2;
		break;
	case 13000000:
		value = REF_FREQ_13M;
		break;
	default:
		return -EINVAL;
	}
	value |= SYSCLK_SEL_LSCLK | LSCLK_DIV_2;
	tc_write(SYS_PLLPARAM, value);

	/* Setup Main Link */
	dp_phy_ctrl = BGREN | PWR_SW_EN | PHY_A0_EN | PHY_M0_EN;
	if (tc->link.base.num_lanes == 2)
		dp_phy_ctrl |= PHY_2LANE;
	tc_write(DP_PHY_CTRL, dp_phy_ctrl);
	msleep(100);

	/* PLL setup */
	tc_write(DP0_PLLCTRL, PLLUPDATE | PLLEN);
	tc_wait_pll_lock(tc);

	tc_write(DP1_PLLCTRL, PLLUPDATE | PLLEN);
	tc_wait_pll_lock(tc);

	/* PXL PLL setup */
	if (tc_test_pattern) {
		ret = tc_pxl_pll_en(tc, clk_get_rate(tc->refclk),
				    1000 * tc->mode->clock);
		if (ret)
			goto err;
	}

	/* Reset/Enable Main Links */
	dp_phy_ctrl |= DP_PHY_RST | PHY_M1_RST | PHY_M0_RST;
	tc_write(DP_PHY_CTRL, dp_phy_ctrl);
	usleep_range(100, 200);
	dp_phy_ctrl &= ~(DP_PHY_RST | PHY_M1_RST | PHY_M0_RST);
	tc_write(DP_PHY_CTRL, dp_phy_ctrl);

	timeout = 1000;
	do {
		tc_read(DP_PHY_CTRL, &value);
		udelay(1);
	} while ((!(value & PHY_RDY)) && (--timeout));

	if (timeout == 0) {
		dev_err(dev, "timeout waiting for phy become ready");
		return -ETIMEDOUT;
=======
	ret = regmap_write(tc->regmap, VPCTRL0,
			   FIELD_PREP(VSDELAY, 0) |
			   OPXLFMT_RGB888 | FRMSYNC_DISABLED | MSF_DISABLED);
	if (ret)
		return ret;

	ret = regmap_write(tc->regmap, HTIM01,
			   FIELD_PREP(HBPR, ALIGN(left_margin, 2)) |
			   FIELD_PREP(HPW, ALIGN(hsync_len, 2)));
	if (ret)
		return ret;

	ret = regmap_write(tc->regmap, HTIM02,
			   FIELD_PREP(HDISPR, ALIGN(mode->hdisplay, 2)) |
			   FIELD_PREP(HFPR, ALIGN(right_margin, 2)));
	if (ret)
		return ret;

	ret = regmap_write(tc->regmap, VTIM01,
			   FIELD_PREP(VBPR, upper_margin) |
			   FIELD_PREP(VSPR, vsync_len));
	if (ret)
		return ret;

	ret = regmap_write(tc->regmap, VTIM02,
			   FIELD_PREP(VFPR, lower_margin) |
			   FIELD_PREP(VDISPR, mode->vdisplay));
	if (ret)
		return ret;

	ret = regmap_write(tc->regmap, VFUEN0, VFUEN); /* update settings */
	if (ret)
		return ret;

	/* Test pattern settings */
	ret = regmap_write(tc->regmap, TSTCTL,
			   FIELD_PREP(COLOR_R, 120) |
			   FIELD_PREP(COLOR_G, 20) |
			   FIELD_PREP(COLOR_B, 99) |
			   ENI2CFILTER |
			   FIELD_PREP(COLOR_BAR_MODE, COLOR_BAR_MODE_BARS));
	if (ret)
		return ret;

	/* DP Main Stream Attributes */
	vid_sync_dly = hsync_len + left_margin + mode->hdisplay;
	ret = regmap_write(tc->regmap, DP0_VIDSYNCDELAY,
		 FIELD_PREP(THRESH_DLY, max_tu_symbol) |
		 FIELD_PREP(VID_SYNC_DLY, vid_sync_dly));

	ret = regmap_write(tc->regmap, DP0_TOTALVAL,
			   FIELD_PREP(H_TOTAL, mode->htotal) |
			   FIELD_PREP(V_TOTAL, mode->vtotal));
	if (ret)
		return ret;

	ret = regmap_write(tc->regmap, DP0_STARTVAL,
			   FIELD_PREP(H_START, left_margin + hsync_len) |
			   FIELD_PREP(V_START, upper_margin + vsync_len));
	if (ret)
		return ret;

	ret = regmap_write(tc->regmap, DP0_ACTIVEVAL,
			   FIELD_PREP(V_ACT, mode->vdisplay) |
			   FIELD_PREP(H_ACT, mode->hdisplay));
	if (ret)
		return ret;

	dp0_syncval = FIELD_PREP(VS_WIDTH, vsync_len) |
		      FIELD_PREP(HS_WIDTH, hsync_len);

	if (mode->flags & DRM_MODE_FLAG_NVSYNC)
		dp0_syncval |= SYNCVAL_VS_POL_ACTIVE_LOW;

	if (mode->flags & DRM_MODE_FLAG_NHSYNC)
		dp0_syncval |= SYNCVAL_HS_POL_ACTIVE_LOW;

	ret = regmap_write(tc->regmap, DP0_SYNCVAL, dp0_syncval);
	if (ret)
		return ret;

	ret = regmap_write(tc->regmap, DPIPXLFMT,
			   VS_POL_ACTIVE_LOW | HS_POL_ACTIVE_LOW |
			   DE_POL_ACTIVE_HIGH | SUB_CFG_TYPE_CONFIG1 |
			   DPI_BPP_RGB888);
	if (ret)
		return ret;

	ret = regmap_write(tc->regmap, DP0_MISC,
			   FIELD_PREP(MAX_TU_SYMBOL, max_tu_symbol) |
			   FIELD_PREP(TU_SIZE, TU_SIZE_RECOMMENDED) |
			   BPC_8);
	if (ret)
		return ret;

	return 0;
}

static int tc_wait_link_training(struct tc_data *tc)
{
	u32 value;
	int ret;

	ret = tc_poll_timeout(tc, DP0_LTSTAT, LT_LOOPDONE,
			      LT_LOOPDONE, 500, 100000);
	if (ret) {
		dev_err(tc->dev, "Link training timeout waiting for LT_LOOPDONE!\n");
		return ret;
	}

	ret = regmap_read(tc->regmap, DP0_LTSTAT, &value);
	if (ret)
		return ret;

	return (value >> 8) & 0x7;
}

static int tc_main_link_enable(struct tc_data *tc)
{
	struct drm_dp_aux *aux = &tc->aux;
	struct device *dev = tc->dev;
	u32 dp_phy_ctrl;
	u32 value;
	int ret;
	u8 tmp[DP_LINK_STATUS_SIZE];

	dev_dbg(tc->dev, "link enable\n");

	ret = regmap_read(tc->regmap, DP0CTL, &value);
	if (ret)
		return ret;

	if (WARN_ON(value & DP_EN)) {
		ret = regmap_write(tc->regmap, DP0CTL, 0);
		if (ret)
			return ret;
	}

	ret = regmap_write(tc->regmap, DP0_SRCCTRL, tc_srcctrl(tc));
	if (ret)
		return ret;
	/* SSCG and BW27 on DP1 must be set to the same as on DP0 */
	ret = regmap_write(tc->regmap, DP1_SRCCTRL,
		 (tc->link.spread ? DP0_SRCCTRL_SSCG : 0) |
		 ((tc->link.rate != 162000) ? DP0_SRCCTRL_BW27 : 0));
	if (ret)
		return ret;

	ret = tc_set_syspllparam(tc);
	if (ret)
		return ret;

	/* Setup Main Link */
	dp_phy_ctrl = BGREN | PWR_SW_EN | PHY_A0_EN | PHY_M0_EN;
	if (tc->link.num_lanes == 2)
		dp_phy_ctrl |= PHY_2LANE;

	ret = regmap_write(tc->regmap, DP_PHY_CTRL, dp_phy_ctrl);
	if (ret)
		return ret;

	/* PLL setup */
	ret = tc_pllupdate(tc, DP0_PLLCTRL);
	if (ret)
		return ret;

	ret = tc_pllupdate(tc, DP1_PLLCTRL);
	if (ret)
		return ret;

	/* Reset/Enable Main Links */
	dp_phy_ctrl |= DP_PHY_RST | PHY_M1_RST | PHY_M0_RST;
	ret = regmap_write(tc->regmap, DP_PHY_CTRL, dp_phy_ctrl);
	usleep_range(100, 200);
	dp_phy_ctrl &= ~(DP_PHY_RST | PHY_M1_RST | PHY_M0_RST);
	ret = regmap_write(tc->regmap, DP_PHY_CTRL, dp_phy_ctrl);

	ret = tc_poll_timeout(tc, DP_PHY_CTRL, PHY_RDY, PHY_RDY, 500, 100000);
	if (ret) {
		dev_err(dev, "timeout waiting for phy become ready");
		return ret;
>>>>>>> upstream/android-13
	}

	/* Set misc: 8 bits per color */
	ret = regmap_update_bits(tc->regmap, DP0_MISC, BPC_8, BPC_8);
	if (ret)
<<<<<<< HEAD
		goto err;
=======
		return ret;
>>>>>>> upstream/android-13

	/*
	 * ASSR mode
	 * on TC358767 side ASSR configured through strap pin
	 * seems there is no way to change this setting from SW
	 *
	 * check is tc configured for same mode
	 */
	if (tc->assr != tc->link.assr) {
		dev_dbg(dev, "Trying to set display to ASSR: %d\n",
			tc->assr);
		/* try to set ASSR on display side */
		tmp[0] = tc->assr;
		ret = drm_dp_dpcd_writeb(aux, DP_EDP_CONFIGURATION_SET, tmp[0]);
		if (ret < 0)
			goto err_dpcd_read;
		/* read back */
		ret = drm_dp_dpcd_readb(aux, DP_EDP_CONFIGURATION_SET, tmp);
		if (ret < 0)
			goto err_dpcd_read;

		if (tmp[0] != tc->assr) {
			dev_dbg(dev, "Failed to switch display ASSR to %d, falling back to unscrambled mode\n",
<<<<<<< HEAD
				 tc->assr);
			/* trying with disabled scrambler */
			tc->link.scrambler_dis = 1;
=======
				tc->assr);
			/* trying with disabled scrambler */
			tc->link.scrambler_dis = true;
>>>>>>> upstream/android-13
		}
	}

	/* Setup Link & DPRx Config for Training */
<<<<<<< HEAD
	ret = drm_dp_link_configure(aux, &tc->link.base);
=======
	tmp[0] = drm_dp_link_rate_to_bw_code(tc->link.rate);
	tmp[1] = tc->link.num_lanes;

	if (drm_dp_enhanced_frame_cap(tc->link.dpcd))
		tmp[1] |= DP_LANE_COUNT_ENHANCED_FRAME_EN;

	ret = drm_dp_dpcd_write(aux, DP_LINK_BW_SET, tmp, 2);
>>>>>>> upstream/android-13
	if (ret < 0)
		goto err_dpcd_write;

	/* DOWNSPREAD_CTRL */
	tmp[0] = tc->link.spread ? DP_SPREAD_AMP_0_5 : 0x00;
	/* MAIN_LINK_CHANNEL_CODING_SET */
<<<<<<< HEAD
	tmp[1] =  tc->link.coding8b10b ? DP_SET_ANSI_8B10B : 0x00;
=======
	tmp[1] =  DP_SET_ANSI_8B10B;
>>>>>>> upstream/android-13
	ret = drm_dp_dpcd_write(aux, DP_DOWNSPREAD_CTRL, tmp, 2);
	if (ret < 0)
		goto err_dpcd_write;

<<<<<<< HEAD
	ret = tc_link_training(tc, DP_TRAINING_PATTERN_1);
	if (ret)
		goto err;

	ret = tc_link_training(tc, DP_TRAINING_PATTERN_2);
	if (ret)
		goto err;
=======
	/* Reset voltage-swing & pre-emphasis */
	tmp[0] = tmp[1] = DP_TRAIN_VOLTAGE_SWING_LEVEL_0 |
			  DP_TRAIN_PRE_EMPH_LEVEL_0;
	ret = drm_dp_dpcd_write(aux, DP_TRAINING_LANE0_SET, tmp, 2);
	if (ret < 0)
		goto err_dpcd_write;

	/* Clock-Recovery */

	/* Set DPCD 0x102 for Training Pattern 1 */
	ret = regmap_write(tc->regmap, DP0_SNKLTCTRL,
			   DP_LINK_SCRAMBLING_DISABLE |
			   DP_TRAINING_PATTERN_1);
	if (ret)
		return ret;

	ret = regmap_write(tc->regmap, DP0_LTLOOPCTRL,
			   (15 << 28) |	/* Defer Iteration Count */
			   (15 << 24) |	/* Loop Iteration Count */
			   (0xd << 0));	/* Loop Timer Delay */
	if (ret)
		return ret;

	ret = regmap_write(tc->regmap, DP0_SRCCTRL,
			   tc_srcctrl(tc) | DP0_SRCCTRL_SCRMBLDIS |
			   DP0_SRCCTRL_AUTOCORRECT |
			   DP0_SRCCTRL_TP1);
	if (ret)
		return ret;

	/* Enable DP0 to start Link Training */
	ret = regmap_write(tc->regmap, DP0CTL,
			   (drm_dp_enhanced_frame_cap(tc->link.dpcd) ?
				EF_EN : 0) | DP_EN);
	if (ret)
		return ret;

	/* wait */

	ret = tc_wait_link_training(tc);
	if (ret < 0)
		return ret;

	if (ret) {
		dev_err(tc->dev, "Link training phase 1 failed: %s\n",
			training_pattern1_errors[ret]);
		return -ENODEV;
	}

	/* Channel Equalization */

	/* Set DPCD 0x102 for Training Pattern 2 */
	ret = regmap_write(tc->regmap, DP0_SNKLTCTRL,
			   DP_LINK_SCRAMBLING_DISABLE |
			   DP_TRAINING_PATTERN_2);
	if (ret)
		return ret;

	ret = regmap_write(tc->regmap, DP0_SRCCTRL,
			   tc_srcctrl(tc) | DP0_SRCCTRL_SCRMBLDIS |
			   DP0_SRCCTRL_AUTOCORRECT |
			   DP0_SRCCTRL_TP2);
	if (ret)
		return ret;

	/* wait */
	ret = tc_wait_link_training(tc);
	if (ret < 0)
		return ret;

	if (ret) {
		dev_err(tc->dev, "Link training phase 2 failed: %s\n",
			training_pattern2_errors[ret]);
		return -ENODEV;
	}

	/*
	 * Toshiba's documentation suggests to first clear DPCD 0x102, then
	 * clear the training pattern bit in DP0_SRCCTRL. Testing shows
	 * that the link sometimes drops if those steps are done in that order,
	 * but if the steps are done in reverse order, the link stays up.
	 *
	 * So we do the steps differently than documented here.
	 */

	/* Clear Training Pattern, set AutoCorrect Mode = 1 */
	ret = regmap_write(tc->regmap, DP0_SRCCTRL, tc_srcctrl(tc) |
			   DP0_SRCCTRL_AUTOCORRECT);
	if (ret)
		return ret;
>>>>>>> upstream/android-13

	/* Clear DPCD 0x102 */
	/* Note: Can Not use DP0_SNKLTCTRL (0x06E4) short cut */
	tmp[0] = tc->link.scrambler_dis ? DP_LINK_SCRAMBLING_DISABLE : 0x00;
	ret = drm_dp_dpcd_writeb(aux, DP_TRAINING_PATTERN_SET, tmp[0]);
	if (ret < 0)
		goto err_dpcd_write;

<<<<<<< HEAD
	/* Clear Training Pattern, set AutoCorrect Mode = 1 */
	tc_write(DP0_SRCCTRL, tc_srcctrl(tc) | DP0_SRCCTRL_AUTOCORRECT);

	/* Wait */
	timeout = 100;
	do {
		udelay(1);
		/* Read DPCD 0x202-0x207 */
		ret = drm_dp_dpcd_read_link_status(aux, tmp + 2);
		if (ret < 0)
			goto err_dpcd_read;
	} while ((--timeout) &&
		 !(drm_dp_channel_eq_ok(tmp + 2,  tc->link.base.num_lanes)));

	if (timeout == 0) {
		/* Read DPCD 0x200-0x201 */
		ret = drm_dp_dpcd_read(aux, DP_SINK_COUNT, tmp, 2);
		if (ret < 0)
			goto err_dpcd_read;
		dev_err(dev, "channel(s) EQ not ok\n");
		dev_info(dev, "0x0200 SINK_COUNT: 0x%02x\n", tmp[0]);
		dev_info(dev, "0x0201 DEVICE_SERVICE_IRQ_VECTOR: 0x%02x\n",
			 tmp[1]);
		dev_info(dev, "0x0202 LANE0_1_STATUS: 0x%02x\n", tmp[2]);
		dev_info(dev, "0x0204 LANE_ALIGN_STATUS_UPDATED: 0x%02x\n",
			 tmp[4]);
		dev_info(dev, "0x0205 SINK_STATUS: 0x%02x\n", tmp[5]);
		dev_info(dev, "0x0206 ADJUST_REQUEST_LANE0_1: 0x%02x\n",
			 tmp[6]);

		return -EAGAIN;
	}

	ret = tc_set_video_mode(tc, tc->mode);
	if (ret)
		goto err;

	/* Set M/N */
	ret = tc_stream_clock_calc(tc);
	if (ret)
		goto err;
=======
	/* Check link status */
	ret = drm_dp_dpcd_read_link_status(aux, tmp);
	if (ret < 0)
		goto err_dpcd_read;

	ret = 0;

	value = tmp[0] & DP_CHANNEL_EQ_BITS;

	if (value != DP_CHANNEL_EQ_BITS) {
		dev_err(tc->dev, "Lane 0 failed: %x\n", value);
		ret = -ENODEV;
	}

	if (tc->link.num_lanes == 2) {
		value = (tmp[0] >> 4) & DP_CHANNEL_EQ_BITS;

		if (value != DP_CHANNEL_EQ_BITS) {
			dev_err(tc->dev, "Lane 1 failed: %x\n", value);
			ret = -ENODEV;
		}

		if (!(tmp[2] & DP_INTERLANE_ALIGN_DONE)) {
			dev_err(tc->dev, "Interlane align failed\n");
			ret = -ENODEV;
		}
	}

	if (ret) {
		dev_err(dev, "0x0202 LANE0_1_STATUS:            0x%02x\n", tmp[0]);
		dev_err(dev, "0x0203 LANE2_3_STATUS             0x%02x\n", tmp[1]);
		dev_err(dev, "0x0204 LANE_ALIGN_STATUS_UPDATED: 0x%02x\n", tmp[2]);
		dev_err(dev, "0x0205 SINK_STATUS:               0x%02x\n", tmp[3]);
		dev_err(dev, "0x0206 ADJUST_REQUEST_LANE0_1:    0x%02x\n", tmp[4]);
		dev_err(dev, "0x0207 ADJUST_REQUEST_LANE2_3:    0x%02x\n", tmp[5]);
		return ret;
	}
>>>>>>> upstream/android-13

	return 0;
err_dpcd_read:
	dev_err(tc->dev, "Failed to read DPCD: %d\n", ret);
	return ret;
err_dpcd_write:
	dev_err(tc->dev, "Failed to write DPCD: %d\n", ret);
<<<<<<< HEAD
err:
	return ret;
}

static int tc_main_link_stream(struct tc_data *tc, int state)
=======
	return ret;
}

static int tc_main_link_disable(struct tc_data *tc)
{
	int ret;

	dev_dbg(tc->dev, "link disable\n");

	ret = regmap_write(tc->regmap, DP0_SRCCTRL, 0);
	if (ret)
		return ret;

	return regmap_write(tc->regmap, DP0CTL, 0);
}

static int tc_stream_enable(struct tc_data *tc)
>>>>>>> upstream/android-13
{
	int ret;
	u32 value;

<<<<<<< HEAD
	dev_dbg(tc->dev, "stream: %d\n", state);

	if (state) {
		value = VID_MN_GEN | DP_EN;
		if (tc->link.base.capabilities & DP_LINK_CAP_ENHANCED_FRAMING)
			value |= EF_EN;
		tc_write(DP0CTL, value);
		/*
		 * VID_EN assertion should be delayed by at least N * LSCLK
		 * cycles from the time VID_MN_GEN is enabled in order to
		 * generate stable values for VID_M. LSCLK is 270 MHz or
		 * 162 MHz, VID_N is set to 32768 in  tc_stream_clock_calc(),
		 * so a delay of at least 203 us should suffice.
		 */
		usleep_range(500, 1000);
		value |= VID_EN;
		tc_write(DP0CTL, value);
		/* Set input interface */
		value = DP0_AUDSRC_NO_INPUT;
		if (tc_test_pattern)
			value |= DP0_VIDSRC_COLOR_BAR;
		else
			value |= DP0_VIDSRC_DPI_RX;
		tc_write(SYSCTRL, value);
	} else {
		tc_write(DP0CTL, 0);
	}

	return 0;
err:
	return ret;
}

static void tc_bridge_pre_enable(struct drm_bridge *bridge)
{
	struct tc_data *tc = bridge_to_tc(bridge);

	drm_panel_prepare(tc->panel);
=======
	dev_dbg(tc->dev, "enable video stream\n");

	/* PXL PLL setup */
	if (tc_test_pattern) {
		ret = tc_pxl_pll_en(tc, clk_get_rate(tc->refclk),
				    1000 * tc->mode.clock);
		if (ret)
			return ret;
	}

	ret = tc_set_video_mode(tc, &tc->mode);
	if (ret)
		return ret;

	/* Set M/N */
	ret = tc_stream_clock_calc(tc);
	if (ret)
		return ret;

	value = VID_MN_GEN | DP_EN;
	if (drm_dp_enhanced_frame_cap(tc->link.dpcd))
		value |= EF_EN;
	ret = regmap_write(tc->regmap, DP0CTL, value);
	if (ret)
		return ret;
	/*
	 * VID_EN assertion should be delayed by at least N * LSCLK
	 * cycles from the time VID_MN_GEN is enabled in order to
	 * generate stable values for VID_M. LSCLK is 270 MHz or
	 * 162 MHz, VID_N is set to 32768 in  tc_stream_clock_calc(),
	 * so a delay of at least 203 us should suffice.
	 */
	usleep_range(500, 1000);
	value |= VID_EN;
	ret = regmap_write(tc->regmap, DP0CTL, value);
	if (ret)
		return ret;
	/* Set input interface */
	value = DP0_AUDSRC_NO_INPUT;
	if (tc_test_pattern)
		value |= DP0_VIDSRC_COLOR_BAR;
	else
		value |= DP0_VIDSRC_DPI_RX;
	ret = regmap_write(tc->regmap, SYSCTRL, value);
	if (ret)
		return ret;

	return 0;
}

static int tc_stream_disable(struct tc_data *tc)
{
	int ret;

	dev_dbg(tc->dev, "disable video stream\n");

	ret = regmap_update_bits(tc->regmap, DP0CTL, VID_EN, 0);
	if (ret)
		return ret;

	tc_pxl_pll_dis(tc);

	return 0;
>>>>>>> upstream/android-13
}

static void tc_bridge_enable(struct drm_bridge *bridge)
{
	struct tc_data *tc = bridge_to_tc(bridge);
	int ret;

<<<<<<< HEAD
	ret = tc_main_link_setup(tc);
	if (ret < 0) {
		dev_err(tc->dev, "main link setup error: %d\n", ret);
		return;
	}

	ret = tc_main_link_stream(tc, 1);
	if (ret < 0) {
		dev_err(tc->dev, "main link stream start error: %d\n", ret);
		return;
	}

	drm_panel_enable(tc->panel);
=======
	ret = tc_get_display_props(tc);
	if (ret < 0) {
		dev_err(tc->dev, "failed to read display props: %d\n", ret);
		return;
	}

	ret = tc_main_link_enable(tc);
	if (ret < 0) {
		dev_err(tc->dev, "main link enable error: %d\n", ret);
		return;
	}

	ret = tc_stream_enable(tc);
	if (ret < 0) {
		dev_err(tc->dev, "main link stream start error: %d\n", ret);
		tc_main_link_disable(tc);
		return;
	}
>>>>>>> upstream/android-13
}

static void tc_bridge_disable(struct drm_bridge *bridge)
{
	struct tc_data *tc = bridge_to_tc(bridge);
	int ret;

<<<<<<< HEAD
	drm_panel_disable(tc->panel);

	ret = tc_main_link_stream(tc, 0);
	if (ret < 0)
		dev_err(tc->dev, "main link stream stop error: %d\n", ret);
}

static void tc_bridge_post_disable(struct drm_bridge *bridge)
{
	struct tc_data *tc = bridge_to_tc(bridge);

	drm_panel_unprepare(tc->panel);
=======
	ret = tc_stream_disable(tc);
	if (ret < 0)
		dev_err(tc->dev, "main link stream stop error: %d\n", ret);

	ret = tc_main_link_disable(tc);
	if (ret < 0)
		dev_err(tc->dev, "main link disable error: %d\n", ret);
>>>>>>> upstream/android-13
}

static bool tc_bridge_mode_fixup(struct drm_bridge *bridge,
				 const struct drm_display_mode *mode,
				 struct drm_display_mode *adj)
{
	/* Fixup sync polarities, both hsync and vsync are active low */
	adj->flags = mode->flags;
	adj->flags |= (DRM_MODE_FLAG_NHSYNC | DRM_MODE_FLAG_NVSYNC);
	adj->flags &= ~(DRM_MODE_FLAG_PHSYNC | DRM_MODE_FLAG_PVSYNC);

	return true;
}

<<<<<<< HEAD
static enum drm_mode_status tc_connector_mode_valid(struct drm_connector *connector,
				   struct drm_display_mode *mode)
{
	struct tc_data *tc = connector_to_tc(connector);
=======
static enum drm_mode_status tc_mode_valid(struct drm_bridge *bridge,
					  const struct drm_display_info *info,
					  const struct drm_display_mode *mode)
{
	struct tc_data *tc = bridge_to_tc(bridge);
>>>>>>> upstream/android-13
	u32 req, avail;
	u32 bits_per_pixel = 24;

	/* DPI interface clock limitation: upto 154 MHz */
	if (mode->clock > 154000)
		return MODE_CLOCK_HIGH;

	req = mode->clock * bits_per_pixel / 8;
<<<<<<< HEAD
	avail = tc->link.base.num_lanes * tc->link.base.rate;
=======
	avail = tc->link.num_lanes * tc->link.rate;
>>>>>>> upstream/android-13

	if (req > avail)
		return MODE_BAD;

	return MODE_OK;
}

static void tc_bridge_mode_set(struct drm_bridge *bridge,
<<<<<<< HEAD
			       struct drm_display_mode *mode,
			       struct drm_display_mode *adj)
{
	struct tc_data *tc = bridge_to_tc(bridge);

	tc->mode = mode;
=======
			       const struct drm_display_mode *mode,
			       const struct drm_display_mode *adj)
{
	struct tc_data *tc = bridge_to_tc(bridge);

	tc->mode = *mode;
}

static struct edid *tc_get_edid(struct drm_bridge *bridge,
				struct drm_connector *connector)
{
	struct tc_data *tc = bridge_to_tc(bridge);

	return drm_get_edid(connector, &tc->aux.ddc);
>>>>>>> upstream/android-13
}

static int tc_connector_get_modes(struct drm_connector *connector)
{
	struct tc_data *tc = connector_to_tc(connector);
<<<<<<< HEAD
	struct edid *edid;
	unsigned int count;
=======
	int num_modes;
	struct edid *edid;
>>>>>>> upstream/android-13
	int ret;

	ret = tc_get_display_props(tc);
	if (ret < 0) {
		dev_err(tc->dev, "failed to read display props: %d\n", ret);
		return 0;
	}

<<<<<<< HEAD
	if (tc->panel && tc->panel->funcs && tc->panel->funcs->get_modes) {
		count = tc->panel->funcs->get_modes(tc->panel);
		if (count > 0)
			return count;
	}

	edid = drm_get_edid(connector, &tc->aux.ddc);

	kfree(tc->edid);
	tc->edid = edid;
	if (!edid)
		return 0;

	drm_connector_update_edid_property(connector, edid);
	count = drm_add_edid_modes(connector, edid);

	return count;
}

static void tc_connector_set_polling(struct tc_data *tc,
				     struct drm_connector *connector)
{
	/* TODO: add support for HPD */
	connector->polled = DRM_CONNECTOR_POLL_CONNECT |
			    DRM_CONNECTOR_POLL_DISCONNECT;
}

static struct drm_encoder *
tc_connector_best_encoder(struct drm_connector *connector)
{
	struct tc_data *tc = connector_to_tc(connector);

	return tc->bridge.encoder;
=======
	if (tc->panel_bridge) {
		num_modes = drm_bridge_get_modes(tc->panel_bridge, connector);
		if (num_modes > 0)
			return num_modes;
	}

	edid = tc_get_edid(&tc->bridge, connector);
	num_modes = drm_add_edid_modes(connector, edid);
	kfree(edid);

	return num_modes;
>>>>>>> upstream/android-13
}

static const struct drm_connector_helper_funcs tc_connector_helper_funcs = {
	.get_modes = tc_connector_get_modes,
<<<<<<< HEAD
	.mode_valid = tc_connector_mode_valid,
	.best_encoder = tc_connector_best_encoder,
};

static const struct drm_connector_funcs tc_connector_funcs = {
=======
};

static enum drm_connector_status tc_bridge_detect(struct drm_bridge *bridge)
{
	struct tc_data *tc = bridge_to_tc(bridge);
	bool conn;
	u32 val;
	int ret;

	ret = regmap_read(tc->regmap, GPIOI, &val);
	if (ret)
		return connector_status_unknown;

	conn = val & BIT(tc->hpd_pin);

	if (conn)
		return connector_status_connected;
	else
		return connector_status_disconnected;
}

static enum drm_connector_status
tc_connector_detect(struct drm_connector *connector, bool force)
{
	struct tc_data *tc = connector_to_tc(connector);

	if (tc->hpd_pin >= 0)
		return tc_bridge_detect(&tc->bridge);

	if (tc->panel_bridge)
		return connector_status_connected;
	else
		return connector_status_unknown;
}

static const struct drm_connector_funcs tc_connector_funcs = {
	.detect = tc_connector_detect,
>>>>>>> upstream/android-13
	.fill_modes = drm_helper_probe_single_connector_modes,
	.destroy = drm_connector_cleanup,
	.reset = drm_atomic_helper_connector_reset,
	.atomic_duplicate_state = drm_atomic_helper_connector_duplicate_state,
	.atomic_destroy_state = drm_atomic_helper_connector_destroy_state,
};

<<<<<<< HEAD
static int tc_bridge_attach(struct drm_bridge *bridge)
=======
static int tc_bridge_attach(struct drm_bridge *bridge,
			    enum drm_bridge_attach_flags flags)
>>>>>>> upstream/android-13
{
	u32 bus_format = MEDIA_BUS_FMT_RGB888_1X24;
	struct tc_data *tc = bridge_to_tc(bridge);
	struct drm_device *drm = bridge->dev;
	int ret;

<<<<<<< HEAD
	/* Create eDP connector */
	drm_connector_helper_add(&tc->connector, &tc_connector_helper_funcs);
	ret = drm_connector_init(drm, &tc->connector, &tc_connector_funcs,
				 DRM_MODE_CONNECTOR_eDP);
	if (ret)
		return ret;

	if (tc->panel)
		drm_panel_attach(tc->panel, &tc->connector);
=======
	if (tc->panel_bridge) {
		/* If a connector is required then this driver shall create it */
		ret = drm_bridge_attach(tc->bridge.encoder, tc->panel_bridge,
					&tc->bridge, flags | DRM_BRIDGE_ATTACH_NO_CONNECTOR);
		if (ret)
			return ret;
	}

	if (flags & DRM_BRIDGE_ATTACH_NO_CONNECTOR)
		return 0;

	tc->aux.drm_dev = drm;
	ret = drm_dp_aux_register(&tc->aux);
	if (ret < 0)
		return ret;

	/* Create DP/eDP connector */
	drm_connector_helper_add(&tc->connector, &tc_connector_helper_funcs);
	ret = drm_connector_init(drm, &tc->connector, &tc_connector_funcs, tc->bridge.type);
	if (ret)
		goto aux_unregister;

	/* Don't poll if don't have HPD connected */
	if (tc->hpd_pin >= 0) {
		if (tc->have_irq)
			tc->connector.polled = DRM_CONNECTOR_POLL_HPD;
		else
			tc->connector.polled = DRM_CONNECTOR_POLL_CONNECT |
					       DRM_CONNECTOR_POLL_DISCONNECT;
	}
>>>>>>> upstream/android-13

	drm_display_info_set_bus_formats(&tc->connector.display_info,
					 &bus_format, 1);
	tc->connector.display_info.bus_flags =
		DRM_BUS_FLAG_DE_HIGH |
<<<<<<< HEAD
		DRM_BUS_FLAG_PIXDATA_NEGEDGE |
		DRM_BUS_FLAG_SYNC_NEGEDGE;
	drm_connector_attach_encoder(&tc->connector, tc->bridge.encoder);

	return 0;
=======
		DRM_BUS_FLAG_PIXDATA_DRIVE_NEGEDGE |
		DRM_BUS_FLAG_SYNC_DRIVE_NEGEDGE;
	drm_connector_attach_encoder(&tc->connector, tc->bridge.encoder);

	return 0;
aux_unregister:
	drm_dp_aux_unregister(&tc->aux);
	return ret;
}

static void tc_bridge_detach(struct drm_bridge *bridge)
{
	drm_dp_aux_unregister(&bridge_to_tc(bridge)->aux);
>>>>>>> upstream/android-13
}

static const struct drm_bridge_funcs tc_bridge_funcs = {
	.attach = tc_bridge_attach,
<<<<<<< HEAD
	.mode_set = tc_bridge_mode_set,
	.pre_enable = tc_bridge_pre_enable,
	.enable = tc_bridge_enable,
	.disable = tc_bridge_disable,
	.post_disable = tc_bridge_post_disable,
	.mode_fixup = tc_bridge_mode_fixup,
=======
	.detach = tc_bridge_detach,
	.mode_valid = tc_mode_valid,
	.mode_set = tc_bridge_mode_set,
	.enable = tc_bridge_enable,
	.disable = tc_bridge_disable,
	.mode_fixup = tc_bridge_mode_fixup,
	.detect = tc_bridge_detect,
	.get_edid = tc_get_edid,
>>>>>>> upstream/android-13
};

static bool tc_readable_reg(struct device *dev, unsigned int reg)
{
	return reg != SYSCTRL;
}

static const struct regmap_range tc_volatile_ranges[] = {
	regmap_reg_range(DP0_AUXWDATA(0), DP0_AUXSTATUS),
	regmap_reg_range(DP0_LTSTAT, DP0_SNKLTCHGREQ),
	regmap_reg_range(DP_PHY_CTRL, DP_PHY_CTRL),
	regmap_reg_range(DP0_PLLCTRL, PXL_PLLCTRL),
	regmap_reg_range(VFUEN0, VFUEN0),
<<<<<<< HEAD
=======
	regmap_reg_range(INTSTS_G, INTSTS_G),
	regmap_reg_range(GPIOI, GPIOI),
>>>>>>> upstream/android-13
};

static const struct regmap_access_table tc_volatile_table = {
	.yes_ranges = tc_volatile_ranges,
	.n_yes_ranges = ARRAY_SIZE(tc_volatile_ranges),
};

static bool tc_writeable_reg(struct device *dev, unsigned int reg)
{
	return (reg != TC_IDREG) &&
	       (reg != DP0_LTSTAT) &&
	       (reg != DP0_SNKLTCHGREQ);
}

static const struct regmap_config tc_regmap_config = {
	.name = "tc358767",
	.reg_bits = 16,
	.val_bits = 32,
	.reg_stride = 4,
	.max_register = PLL_DBG,
	.cache_type = REGCACHE_RBTREE,
	.readable_reg = tc_readable_reg,
	.volatile_table = &tc_volatile_table,
	.writeable_reg = tc_writeable_reg,
	.reg_format_endian = REGMAP_ENDIAN_BIG,
	.val_format_endian = REGMAP_ENDIAN_LITTLE,
};

<<<<<<< HEAD
static int tc_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
	struct device *dev = &client->dev;
=======
static irqreturn_t tc_irq_handler(int irq, void *arg)
{
	struct tc_data *tc = arg;
	u32 val;
	int r;

	r = regmap_read(tc->regmap, INTSTS_G, &val);
	if (r)
		return IRQ_NONE;

	if (!val)
		return IRQ_NONE;

	if (val & INT_SYSERR) {
		u32 stat = 0;

		regmap_read(tc->regmap, SYSSTAT, &stat);

		dev_err(tc->dev, "syserr %x\n", stat);
	}

	if (tc->hpd_pin >= 0 && tc->bridge.dev) {
		/*
		 * H is triggered when the GPIO goes high.
		 *
		 * LC is triggered when the GPIO goes low and stays low for
		 * the duration of LCNT
		 */
		bool h = val & INT_GPIO_H(tc->hpd_pin);
		bool lc = val & INT_GPIO_LC(tc->hpd_pin);

		dev_dbg(tc->dev, "GPIO%d: %s %s\n", tc->hpd_pin,
			h ? "H" : "", lc ? "LC" : "");

		if (h || lc)
			drm_kms_helper_hotplug_event(tc->bridge.dev);
	}

	regmap_write(tc->regmap, INTSTS_G, val);

	return IRQ_HANDLED;
}

static int tc_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
	struct device *dev = &client->dev;
	struct drm_panel *panel;
>>>>>>> upstream/android-13
	struct tc_data *tc;
	int ret;

	tc = devm_kzalloc(dev, sizeof(*tc), GFP_KERNEL);
	if (!tc)
		return -ENOMEM;

	tc->dev = dev;

	/* port@2 is the output port */
<<<<<<< HEAD
	ret = drm_of_find_panel_or_bridge(dev->of_node, 2, 0, &tc->panel, NULL);
	if (ret && ret != -ENODEV)
		return ret;

=======
	ret = drm_of_find_panel_or_bridge(dev->of_node, 2, 0, &panel, NULL);
	if (ret && ret != -ENODEV)
		return ret;

	if (panel) {
		struct drm_bridge *panel_bridge;

		panel_bridge = devm_drm_panel_bridge_add(dev, panel);
		if (IS_ERR(panel_bridge))
			return PTR_ERR(panel_bridge);

		tc->panel_bridge = panel_bridge;
		tc->bridge.type = DRM_MODE_CONNECTOR_eDP;
	} else {
		tc->bridge.type = DRM_MODE_CONNECTOR_DisplayPort;
	}

>>>>>>> upstream/android-13
	/* Shut down GPIO is optional */
	tc->sd_gpio = devm_gpiod_get_optional(dev, "shutdown", GPIOD_OUT_HIGH);
	if (IS_ERR(tc->sd_gpio))
		return PTR_ERR(tc->sd_gpio);

	if (tc->sd_gpio) {
		gpiod_set_value_cansleep(tc->sd_gpio, 0);
		usleep_range(5000, 10000);
	}

	/* Reset GPIO is optional */
	tc->reset_gpio = devm_gpiod_get_optional(dev, "reset", GPIOD_OUT_LOW);
	if (IS_ERR(tc->reset_gpio))
		return PTR_ERR(tc->reset_gpio);

	if (tc->reset_gpio) {
		gpiod_set_value_cansleep(tc->reset_gpio, 1);
		usleep_range(5000, 10000);
	}

	tc->refclk = devm_clk_get(dev, "ref");
	if (IS_ERR(tc->refclk)) {
		ret = PTR_ERR(tc->refclk);
		dev_err(dev, "Failed to get refclk: %d\n", ret);
		return ret;
	}

	tc->regmap = devm_regmap_init_i2c(client, &tc_regmap_config);
	if (IS_ERR(tc->regmap)) {
		ret = PTR_ERR(tc->regmap);
		dev_err(dev, "Failed to initialize regmap: %d\n", ret);
		return ret;
	}

<<<<<<< HEAD
=======
	ret = of_property_read_u32(dev->of_node, "toshiba,hpd-pin",
				   &tc->hpd_pin);
	if (ret) {
		tc->hpd_pin = -ENODEV;
	} else {
		if (tc->hpd_pin < 0 || tc->hpd_pin > 1) {
			dev_err(dev, "failed to parse HPD number\n");
			return ret;
		}
	}

	if (client->irq > 0) {
		/* enable SysErr */
		regmap_write(tc->regmap, INTCTL_G, INT_SYSERR);

		ret = devm_request_threaded_irq(dev, client->irq,
						NULL, tc_irq_handler,
						IRQF_ONESHOT,
						"tc358767-irq", tc);
		if (ret) {
			dev_err(dev, "failed to register dp interrupt\n");
			return ret;
		}

		tc->have_irq = true;
	}

>>>>>>> upstream/android-13
	ret = regmap_read(tc->regmap, TC_IDREG, &tc->rev);
	if (ret) {
		dev_err(tc->dev, "can not read device ID: %d\n", ret);
		return ret;
	}

	if ((tc->rev != 0x6601) && (tc->rev != 0x6603)) {
		dev_err(tc->dev, "invalid device ID: 0x%08x\n", tc->rev);
		return -EINVAL;
	}

	tc->assr = (tc->rev == 0x6601); /* Enable ASSR for eDP panels */

<<<<<<< HEAD
=======
	if (!tc->reset_gpio) {
		/*
		 * If the reset pin isn't present, do a software reset. It isn't
		 * as thorough as the hardware reset, as we can't reset the I2C
		 * communication block for obvious reasons, but it's getting the
		 * chip into a defined state.
		 */
		regmap_update_bits(tc->regmap, SYSRSTENB,
				ENBLCD0 | ENBBM | ENBDSIRX | ENBREG | ENBHDCP,
				0);
		regmap_update_bits(tc->regmap, SYSRSTENB,
				ENBLCD0 | ENBBM | ENBDSIRX | ENBREG | ENBHDCP,
				ENBLCD0 | ENBBM | ENBDSIRX | ENBREG | ENBHDCP);
		usleep_range(5000, 10000);
	}

	if (tc->hpd_pin >= 0) {
		u32 lcnt_reg = tc->hpd_pin == 0 ? INT_GP0_LCNT : INT_GP1_LCNT;
		u32 h_lc = INT_GPIO_H(tc->hpd_pin) | INT_GPIO_LC(tc->hpd_pin);

		/* Set LCNT to 2ms */
		regmap_write(tc->regmap, lcnt_reg,
			     clk_get_rate(tc->refclk) * 2 / 1000);
		/* We need the "alternate" mode for HPD */
		regmap_write(tc->regmap, GPIOM, BIT(tc->hpd_pin));

		if (tc->have_irq) {
			/* enable H & LC */
			regmap_update_bits(tc->regmap, INTCTL_G, h_lc, h_lc);
		}
	}

>>>>>>> upstream/android-13
	ret = tc_aux_link_setup(tc);
	if (ret)
		return ret;

	/* Register DP AUX channel */
	tc->aux.name = "TC358767 AUX i2c adapter";
	tc->aux.dev = tc->dev;
	tc->aux.transfer = tc_aux_transfer;
<<<<<<< HEAD
	ret = drm_dp_aux_register(&tc->aux);
	if (ret)
		return ret;

	ret = tc_get_display_props(tc);
	if (ret)
		goto err_unregister_aux;

	tc_connector_set_polling(tc, &tc->connector);

	tc->bridge.funcs = &tc_bridge_funcs;
=======
	drm_dp_aux_init(&tc->aux);

	tc->bridge.funcs = &tc_bridge_funcs;
	if (tc->hpd_pin >= 0)
		tc->bridge.ops |= DRM_BRIDGE_OP_DETECT;
	tc->bridge.ops |= DRM_BRIDGE_OP_EDID;

>>>>>>> upstream/android-13
	tc->bridge.of_node = dev->of_node;
	drm_bridge_add(&tc->bridge);

	i2c_set_clientdata(client, tc);

	return 0;
<<<<<<< HEAD
err_unregister_aux:
	drm_dp_aux_unregister(&tc->aux);
	return ret;
=======
>>>>>>> upstream/android-13
}

static int tc_remove(struct i2c_client *client)
{
	struct tc_data *tc = i2c_get_clientdata(client);

	drm_bridge_remove(&tc->bridge);
<<<<<<< HEAD
	drm_dp_aux_unregister(&tc->aux);

	tc_pxl_pll_dis(tc);
=======
>>>>>>> upstream/android-13

	return 0;
}

static const struct i2c_device_id tc358767_i2c_ids[] = {
	{ "tc358767", 0 },
	{ }
};
MODULE_DEVICE_TABLE(i2c, tc358767_i2c_ids);

static const struct of_device_id tc358767_of_ids[] = {
	{ .compatible = "toshiba,tc358767", },
	{ }
};
MODULE_DEVICE_TABLE(of, tc358767_of_ids);

static struct i2c_driver tc358767_driver = {
	.driver = {
		.name = "tc358767",
		.of_match_table = tc358767_of_ids,
	},
	.id_table = tc358767_i2c_ids,
	.probe = tc_probe,
	.remove	= tc_remove,
};
module_i2c_driver(tc358767_driver);

MODULE_AUTHOR("Andrey Gusakov <andrey.gusakov@cogentembedded.com>");
MODULE_DESCRIPTION("tc358767 eDP encoder driver");
MODULE_LICENSE("GPL");
