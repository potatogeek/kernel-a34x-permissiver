<<<<<<< HEAD
/*
 * Copyright (c) 2015, The Linux Foundation. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
=======
// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (c) 2015, The Linux Foundation. All rights reserved.
>>>>>>> upstream/android-13
 */

#include <linux/platform_device.h>
#include <linux/delay.h>
#include <linux/bitops.h>
#include <linux/regmap.h>
#include <linux/thermal.h>
#include "tsens.h"

<<<<<<< HEAD
#define CAL_MDEGC		30000

=======
>>>>>>> upstream/android-13
#define CONFIG_ADDR		0x3640
#define CONFIG_ADDR_8660	0x3620
/* CONFIG_ADDR bitmasks */
#define CONFIG			0x9b
#define CONFIG_MASK		0xf
#define CONFIG_8660		1
#define CONFIG_SHIFT_8660	28
#define CONFIG_MASK_8660	(3 << CONFIG_SHIFT_8660)

<<<<<<< HEAD
#define STATUS_CNTL_ADDR_8064	0x3660
=======
>>>>>>> upstream/android-13
#define CNTL_ADDR		0x3620
/* CNTL_ADDR bitmasks */
#define EN			BIT(0)
#define SW_RST			BIT(1)
<<<<<<< HEAD
#define SENSOR0_EN		BIT(3)
#define SLP_CLK_ENA		BIT(26)
#define SLP_CLK_ENA_8660	BIT(24)
#define MEASURE_PERIOD		1
#define SENSOR0_SHIFT		3

/* INT_STATUS_ADDR bitmasks */
#define MIN_STATUS_MASK		BIT(0)
#define LOWER_STATUS_CLR	BIT(1)
#define UPPER_STATUS_CLR	BIT(2)
#define MAX_STATUS_MASK		BIT(3)

#define THRESHOLD_ADDR		0x3624
/* THRESHOLD_ADDR bitmasks */
#define THRESHOLD_MAX_LIMIT_SHIFT	24
#define THRESHOLD_MIN_LIMIT_SHIFT	16
#define THRESHOLD_UPPER_LIMIT_SHIFT	8
#define THRESHOLD_LOWER_LIMIT_SHIFT	0

/* Initial temperature threshold values */
#define LOWER_LIMIT_TH		0x50
#define UPPER_LIMIT_TH		0xdf
#define MIN_LIMIT_TH		0x0
#define MAX_LIMIT_TH		0xff

#define S0_STATUS_ADDR		0x3628
#define INT_STATUS_ADDR		0x363c
#define TRDY_MASK		BIT(7)
#define TIMEOUT_US		100

static int suspend_8960(struct tsens_device *tmdev)
{
	int ret;
	unsigned int mask;
	struct regmap *map = tmdev->map;

	ret = regmap_read(map, THRESHOLD_ADDR, &tmdev->ctx.threshold);
	if (ret)
		return ret;

	ret = regmap_read(map, CNTL_ADDR, &tmdev->ctx.control);
	if (ret)
		return ret;

	if (tmdev->num_sensors > 1)
=======

#define MEASURE_PERIOD		BIT(18)
#define SLP_CLK_ENA		BIT(26)
#define SLP_CLK_ENA_8660	BIT(24)
#define SENSOR0_SHIFT		3

#define THRESHOLD_ADDR		0x3624

#define INT_STATUS_ADDR		0x363c

#define S0_STATUS_OFF		0x3628
#define S1_STATUS_OFF		0x362c
#define S2_STATUS_OFF		0x3630
#define S3_STATUS_OFF		0x3634
#define S4_STATUS_OFF		0x3638
#define S5_STATUS_OFF		0x3664  /* Sensors 5-10 found on apq8064/msm8960 */
#define S6_STATUS_OFF		0x3668
#define S7_STATUS_OFF		0x366c
#define S8_STATUS_OFF		0x3670
#define S9_STATUS_OFF		0x3674
#define S10_STATUS_OFF		0x3678

/* Original slope - 350 to compensate mC to C inaccuracy */
static u32 tsens_msm8960_slope[] = {
			826, 826, 804, 826,
			761, 782, 782, 849,
			782, 849, 782
			};

static int suspend_8960(struct tsens_priv *priv)
{
	int ret;
	unsigned int mask;
	struct regmap *map = priv->tm_map;

	ret = regmap_read(map, THRESHOLD_ADDR, &priv->ctx.threshold);
	if (ret)
		return ret;

	ret = regmap_read(map, CNTL_ADDR, &priv->ctx.control);
	if (ret)
		return ret;

	if (priv->num_sensors > 1)
>>>>>>> upstream/android-13
		mask = SLP_CLK_ENA | EN;
	else
		mask = SLP_CLK_ENA_8660 | EN;

	ret = regmap_update_bits(map, CNTL_ADDR, mask, 0);
	if (ret)
		return ret;

	return 0;
}

<<<<<<< HEAD
static int resume_8960(struct tsens_device *tmdev)
{
	int ret;
	struct regmap *map = tmdev->map;
=======
static int resume_8960(struct tsens_priv *priv)
{
	int ret;
	struct regmap *map = priv->tm_map;
>>>>>>> upstream/android-13

	ret = regmap_update_bits(map, CNTL_ADDR, SW_RST, SW_RST);
	if (ret)
		return ret;

	/*
	 * Separate CONFIG restore is not needed only for 8660 as
	 * config is part of CTRL Addr and its restored as such
	 */
<<<<<<< HEAD
	if (tmdev->num_sensors > 1) {
=======
	if (priv->num_sensors > 1) {
>>>>>>> upstream/android-13
		ret = regmap_update_bits(map, CONFIG_ADDR, CONFIG_MASK, CONFIG);
		if (ret)
			return ret;
	}

<<<<<<< HEAD
	ret = regmap_write(map, THRESHOLD_ADDR, tmdev->ctx.threshold);
	if (ret)
		return ret;

	ret = regmap_write(map, CNTL_ADDR, tmdev->ctx.control);
=======
	ret = regmap_write(map, THRESHOLD_ADDR, priv->ctx.threshold);
	if (ret)
		return ret;

	ret = regmap_write(map, CNTL_ADDR, priv->ctx.control);
>>>>>>> upstream/android-13
	if (ret)
		return ret;

	return 0;
}

<<<<<<< HEAD
static int enable_8960(struct tsens_device *tmdev, int id)
{
	int ret;
	u32 reg, mask;

	ret = regmap_read(tmdev->map, CNTL_ADDR, &reg);
	if (ret)
		return ret;

	mask = BIT(id + SENSOR0_SHIFT);
	ret = regmap_write(tmdev->map, CNTL_ADDR, reg | SW_RST);
	if (ret)
		return ret;

	if (tmdev->num_sensors > 1)
=======
static int enable_8960(struct tsens_priv *priv, int id)
{
	int ret;
	u32 reg, mask = BIT(id);

	ret = regmap_read(priv->tm_map, CNTL_ADDR, &reg);
	if (ret)
		return ret;

	/* HARDWARE BUG:
	 * On platforms with more than 6 sensors, all remaining sensors
	 * must be enabled together, otherwise undefined results are expected.
	 * (Sensor 6-7 disabled, Sensor 3 disabled...) In the original driver,
	 * all the sensors are enabled in one step hence this bug is not
	 * triggered.
	 */
	if (id > 5)
		mask = GENMASK(10, 6);

	mask <<= SENSOR0_SHIFT;

	/* Sensors already enabled. Skip. */
	if ((reg & mask) == mask)
		return 0;

	ret = regmap_write(priv->tm_map, CNTL_ADDR, reg | SW_RST);
	if (ret)
		return ret;

	reg |= MEASURE_PERIOD;

	if (priv->num_sensors > 1)
>>>>>>> upstream/android-13
		reg |= mask | SLP_CLK_ENA | EN;
	else
		reg |= mask | SLP_CLK_ENA_8660 | EN;

<<<<<<< HEAD
	ret = regmap_write(tmdev->map, CNTL_ADDR, reg);
=======
	ret = regmap_write(priv->tm_map, CNTL_ADDR, reg);
>>>>>>> upstream/android-13
	if (ret)
		return ret;

	return 0;
}

<<<<<<< HEAD
static void disable_8960(struct tsens_device *tmdev)
=======
static void disable_8960(struct tsens_priv *priv)
>>>>>>> upstream/android-13
{
	int ret;
	u32 reg_cntl;
	u32 mask;

<<<<<<< HEAD
	mask = GENMASK(tmdev->num_sensors - 1, 0);
	mask <<= SENSOR0_SHIFT;
	mask |= EN;

	ret = regmap_read(tmdev->map, CNTL_ADDR, &reg_cntl);
=======
	mask = GENMASK(priv->num_sensors - 1, 0);
	mask <<= SENSOR0_SHIFT;
	mask |= EN;

	ret = regmap_read(priv->tm_map, CNTL_ADDR, &reg_cntl);
>>>>>>> upstream/android-13
	if (ret)
		return;

	reg_cntl &= ~mask;

<<<<<<< HEAD
	if (tmdev->num_sensors > 1)
=======
	if (priv->num_sensors > 1)
>>>>>>> upstream/android-13
		reg_cntl &= ~SLP_CLK_ENA;
	else
		reg_cntl &= ~SLP_CLK_ENA_8660;

<<<<<<< HEAD
	regmap_write(tmdev->map, CNTL_ADDR, reg_cntl);
}

static int init_8960(struct tsens_device *tmdev)
{
	int ret, i;
	u32 reg_cntl;

	tmdev->map = dev_get_regmap(tmdev->dev, NULL);
	if (!tmdev->map)
		return -ENODEV;

	/*
	 * The status registers for each sensor are discontiguous
	 * because some SoCs have 5 sensors while others have more
	 * but the control registers stay in the same place, i.e
	 * directly after the first 5 status registers.
	 */
	for (i = 0; i < tmdev->num_sensors; i++) {
		if (i >= 5)
			tmdev->sensor[i].status = S0_STATUS_ADDR + 40;
		tmdev->sensor[i].status += i * 4;
	}

	reg_cntl = SW_RST;
	ret = regmap_update_bits(tmdev->map, CNTL_ADDR, SW_RST, reg_cntl);
	if (ret)
		return ret;

	if (tmdev->num_sensors > 1) {
		reg_cntl |= SLP_CLK_ENA | (MEASURE_PERIOD << 18);
		reg_cntl &= ~SW_RST;
		ret = regmap_update_bits(tmdev->map, CONFIG_ADDR,
					 CONFIG_MASK, CONFIG);
	} else {
		reg_cntl |= SLP_CLK_ENA_8660 | (MEASURE_PERIOD << 16);
		reg_cntl &= ~CONFIG_MASK_8660;
		reg_cntl |= CONFIG_8660 << CONFIG_SHIFT_8660;
	}

	reg_cntl |= GENMASK(tmdev->num_sensors - 1, 0) << SENSOR0_SHIFT;
	ret = regmap_write(tmdev->map, CNTL_ADDR, reg_cntl);
	if (ret)
		return ret;

	reg_cntl |= EN;
	ret = regmap_write(tmdev->map, CNTL_ADDR, reg_cntl);
	if (ret)
		return ret;

	return 0;
}

static int calibrate_8960(struct tsens_device *tmdev)
{
	int i;
	char *data;

	ssize_t num_read = tmdev->num_sensors;
	struct tsens_sensor *s = tmdev->sensor;

	data = qfprom_read(tmdev->dev, "calib");
	if (IS_ERR(data))
		data = qfprom_read(tmdev->dev, "calib_backup");
	if (IS_ERR(data))
		return PTR_ERR(data);

	for (i = 0; i < num_read; i++, s++)
		s->offset = data[i];
=======
	regmap_write(priv->tm_map, CNTL_ADDR, reg_cntl);
}

static int calibrate_8960(struct tsens_priv *priv)
{
	int i;
	char *data;
	u32 p1[11];

	data = qfprom_read(priv->dev, "calib");
	if (IS_ERR(data))
		data = qfprom_read(priv->dev, "calib_backup");
	if (IS_ERR(data))
		return PTR_ERR(data);

	for (i = 0; i < priv->num_sensors; i++) {
		p1[i] = data[i];
		priv->sensor[i].slope = tsens_msm8960_slope[i];
	}

	compute_intercept_slope(priv, p1, NULL, ONE_PT_CALIB);

	kfree(data);
>>>>>>> upstream/android-13

	return 0;
}

<<<<<<< HEAD
/* Temperature on y axis and ADC-code on x-axis */
static inline int code_to_mdegC(u32 adc_code, const struct tsens_sensor *s)
{
	int slope, offset;

	slope = thermal_zone_get_slope(s->tzd);
	offset = CAL_MDEGC - slope * s->offset;

	return adc_code * slope + offset;
}

static int get_temp_8960(struct tsens_device *tmdev, int id, int *temp)
{
	int ret;
	u32 code, trdy;
	const struct tsens_sensor *s = &tmdev->sensor[id];
	unsigned long timeout;

	timeout = jiffies + usecs_to_jiffies(TIMEOUT_US);
	do {
		ret = regmap_read(tmdev->map, INT_STATUS_ADDR, &trdy);
		if (ret)
			return ret;
		if (!(trdy & TRDY_MASK))
			continue;
		ret = regmap_read(tmdev->map, s->status, &code);
		if (ret)
			return ret;
		*temp = code_to_mdegC(code, s);
		return 0;
	} while (time_before(jiffies, timeout));

	return -ETIMEDOUT;
}

static const struct tsens_ops ops_8960 = {
	.init		= init_8960,
	.calibrate	= calibrate_8960,
	.get_temp	= get_temp_8960,
=======
static const struct reg_field tsens_8960_regfields[MAX_REGFIELDS] = {
	/* ----- SROT ------ */
	/* No VERSION information */

	/* CNTL */
	[TSENS_EN]     = REG_FIELD(CNTL_ADDR,  0, 0),
	[TSENS_SW_RST] = REG_FIELD(CNTL_ADDR,  1, 1),
	/* 8960 has 5 sensors, 8660 has 11, we only handle 5 */
	[SENSOR_EN]    = REG_FIELD(CNTL_ADDR,  3, 7),

	/* ----- TM ------ */
	/* INTERRUPT ENABLE */
	/* NO INTERRUPT ENABLE */

	/* Single UPPER/LOWER TEMPERATURE THRESHOLD for all sensors */
	[LOW_THRESH_0]   = REG_FIELD(THRESHOLD_ADDR,  0,  7),
	[UP_THRESH_0]    = REG_FIELD(THRESHOLD_ADDR,  8, 15),
	/* MIN_THRESH_0 and MAX_THRESH_0 are not present in the regfield
	 * Recycle CRIT_THRESH_0 and 1 to set the required regs to hardcoded temp
	 * MIN_THRESH_0 -> CRIT_THRESH_1
	 * MAX_THRESH_0 -> CRIT_THRESH_0
	 */
	[CRIT_THRESH_1]   = REG_FIELD(THRESHOLD_ADDR, 16, 23),
	[CRIT_THRESH_0]   = REG_FIELD(THRESHOLD_ADDR, 24, 31),

	/* UPPER/LOWER INTERRUPT [CLEAR/STATUS] */
	/* 1 == clear, 0 == normal operation */
	[LOW_INT_CLEAR_0]   = REG_FIELD(CNTL_ADDR,  9,  9),
	[UP_INT_CLEAR_0]    = REG_FIELD(CNTL_ADDR, 10, 10),

	/* NO CRITICAL INTERRUPT SUPPORT on 8960 */

	/* Sn_STATUS */
	[LAST_TEMP_0]  = REG_FIELD(S0_STATUS_OFF,  0,  7),
	[LAST_TEMP_1]  = REG_FIELD(S1_STATUS_OFF,  0,  7),
	[LAST_TEMP_2]  = REG_FIELD(S2_STATUS_OFF,  0,  7),
	[LAST_TEMP_3]  = REG_FIELD(S3_STATUS_OFF,  0,  7),
	[LAST_TEMP_4]  = REG_FIELD(S4_STATUS_OFF,  0,  7),
	[LAST_TEMP_5]  = REG_FIELD(S5_STATUS_OFF,  0,  7),
	[LAST_TEMP_6]  = REG_FIELD(S6_STATUS_OFF,  0,  7),
	[LAST_TEMP_7]  = REG_FIELD(S7_STATUS_OFF,  0,  7),
	[LAST_TEMP_8]  = REG_FIELD(S8_STATUS_OFF,  0,  7),
	[LAST_TEMP_9]  = REG_FIELD(S9_STATUS_OFF,  0,  7),
	[LAST_TEMP_10] = REG_FIELD(S10_STATUS_OFF, 0,  7),

	/* No VALID field on 8960 */
	/* TSENS_INT_STATUS bits: 1 == threshold violated */
	[MIN_STATUS_0] = REG_FIELD(INT_STATUS_ADDR, 0, 0),
	[LOWER_STATUS_0] = REG_FIELD(INT_STATUS_ADDR, 1, 1),
	[UPPER_STATUS_0] = REG_FIELD(INT_STATUS_ADDR, 2, 2),
	/* No CRITICAL field on 8960 */
	[MAX_STATUS_0] = REG_FIELD(INT_STATUS_ADDR, 3, 3),

	/* TRDY: 1=ready, 0=in progress */
	[TRDY] = REG_FIELD(INT_STATUS_ADDR, 7, 7),
};

static const struct tsens_ops ops_8960 = {
	.init		= init_common,
	.calibrate	= calibrate_8960,
	.get_temp	= get_temp_common,
>>>>>>> upstream/android-13
	.enable		= enable_8960,
	.disable	= disable_8960,
	.suspend	= suspend_8960,
	.resume		= resume_8960,
};

<<<<<<< HEAD
const struct tsens_data data_8960 = {
	.num_sensors	= 11,
	.ops		= &ops_8960,
=======
static struct tsens_features tsens_8960_feat = {
	.ver_major	= VER_0,
	.crit_int	= 0,
	.adc		= 1,
	.srot_split	= 0,
	.max_sensors	= 11,
};

struct tsens_plat_data data_8960 = {
	.num_sensors	= 11,
	.ops		= &ops_8960,
	.feat		= &tsens_8960_feat,
	.fields		= tsens_8960_regfields,
>>>>>>> upstream/android-13
};
