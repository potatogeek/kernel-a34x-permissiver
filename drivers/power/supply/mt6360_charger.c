// SPDX-License-Identifier: GPL-2.0
/*
<<<<<<< HEAD
 * Copyright (c) 2019 MediaTek Inc.
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/regmap.h>
#include <linux/platform_device.h>
#include <linux/of.h>
#include <linux/interrupt.h>
#include <linux/extcon-provider.h>
#include <linux/power_supply.h>
#include <linux/regulator/driver.h>

#include <linux/mfd/mt6360-private.h>

/* 0x11 */
#define MT6360_MASK_FSLP	BIT(3)
#define MT6360_SHFT_FSLP	(3)
#define MT6360_MASK_HIZ		BIT(2)
#define MT6360_SHFT_HIZ		(2)
#define MT6360_MASK_OPA_MODE	BIT(0)
#define MT6360_SHFT_OPA_MODE	(0)
/* 0x12 */
#define MT6360_MASK_TE		BIT(4)
#define MT6360_SHFT_TE		(4)
#define MT6360_MASK_IINLMTSEL	(0x0C)
#define MT6360_SHFT_IINLMTSEL	(2)
#define MT6360_MASK_CHG_EN	BIT(0)
#define MT6360_SHFT_CHG_EN	(0)
/* 0x13 */
#define MT6360_MASK_IAICR	(0xFC)
#define MT6360_SHFT_IAICR	(2)
#define MT6360_MASK_ILIM_EN	BIT(0)
/* 0x14 */
#define MT6360_MASK_VOREG	(0xFE)
#define MT6360_SHFT_VOREG	(1)
/* 0x15 */
#define MT6360_MASK_VOBST	(0xFC)
/* 0x16 */
#define MT6360_MASK_VMIVR	(0xFE)
#define MT6360_SHFT_VMIVR	(1)
/* 0x17 */
#define MT6360_MASK_ICHG	(0xFC)
#define MT6360_SHFT_ICHG	(2)
/* 0x18 */
#define MT6360_MASK_IPREC	(0x0F)
#define MT6360_SHFT_IPREC	(0)
/* 0x19 */
#define MT6360_MASK_IEOC	(0xF0)
#define MT6360_SHFT_IEOC	(4)
/* 0x1A */
#define MT6360_MASK_LBP		(0xF0)
#define MT6360_SHFT_LBP		(4)
#define MT6360_MASK_OTG_OC	(0x07)
#define MT6360_SHFT_OTG_OC	(0)
/* 0x1B */
#define MT6360_MASK_SYSREG	(0x1C)
#define MT6360_SHFT_SYSREG	(2)
#define MT6360_MASK_VREC	(0x03)
#define MT6360_SHFT_VREC	(0)
/* 0x1C */
#define MT6360_MASK_WT_FC	(0xE0)
#define MT6360_SHFT_WT_FC	(5)
/* 0x1D */
#define MT6360_MASK_CHG_WDT_EN	BIT(7)
#define MT6360_SHFT_CHG_WDT_EN	(7)
#define MT6360_MASK_CHG_WDT	(0x30)
#define MT6360_SHFT_CHG_WDT	(4)
/* 0x22 */
#define MT6360_MASK_USBCHGEN	BIT(7)
#define MT6360_SHFT_USBCHGEN	(7)
/* 0x27 */
#define MT6360_MASK_USB_STATUS	(0x70)
#define MT6360_SHFT_USB_STATUS	(4)
/* 0x2C */
#define MT6360_MASK_BAT_COMP	(0x38)
#define MT6360_SHFT_BAT_COMP	(3)
#define MT6360_MASK_VCLAMP	(0x07)
#define MT6360_SHFT_VCLAMP	(0)
/* 0x4A */
#define MT6360_MASK_CHG_STAT	(0xC0)
#define MT6360_SHFT_CHG_STAT	(6)
#define MT6360_MASK_VBAT_LVL	BIT(5)
/* 0xE7 */
#define MT6360_MASK_CHRDET_EXT	BIT(4)
#define MT6360_SHFT_CHRDET_EXT	(4)

=======
 * Copyright (c) 2021 MediaTek Inc.
 */

#include <linux/devm-helpers.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/kernel.h>
#include <linux/linear_range.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/platform_device.h>
#include <linux/power_supply.h>
#include <linux/property.h>
#include <linux/regmap.h>
#include <linux/regulator/driver.h>

#define MT6360_PMU_CHG_CTRL1	0x311
#define MT6360_PMU_CHG_CTRL2	0x312
#define MT6360_PMU_CHG_CTRL3	0x313
#define MT6360_PMU_CHG_CTRL4	0x314
#define MT6360_PMU_CHG_CTRL5	0x315
#define MT6360_PMU_CHG_CTRL6	0x316
#define MT6360_PMU_CHG_CTRL7	0x317
#define MT6360_PMU_CHG_CTRL8	0x318
#define MT6360_PMU_CHG_CTRL9	0x319
#define MT6360_PMU_CHG_CTRL10	0x31A
#define MT6360_PMU_DEVICE_TYPE	0x322
#define MT6360_PMU_USB_STATUS1	0x327
#define MT6360_PMU_CHG_STAT	0x34A
#define MT6360_PMU_CHG_CTRL19	0x361
#define MT6360_PMU_FOD_STAT	0x3E7

/* MT6360_PMU_CHG_CTRL1 */
#define MT6360_FSLP_SHFT	(3)
#define MT6360_FSLP_MASK	BIT(MT6360_FSLP_SHFT)
#define MT6360_OPA_MODE_SHFT	(0)
#define MT6360_OPA_MODE_MASK	BIT(MT6360_OPA_MODE_SHFT)
/* MT6360_PMU_CHG_CTRL2 */
#define MT6360_IINLMTSEL_SHFT	(2)
#define MT6360_IINLMTSEL_MASK	GENMASK(3, 2)
/* MT6360_PMU_CHG_CTRL3 */
#define MT6360_IAICR_SHFT	(2)
#define MT6360_IAICR_MASK	GENMASK(7, 2)
#define MT6360_ILIM_EN_MASK	BIT(0)
/* MT6360_PMU_CHG_CTRL4 */
#define MT6360_VOREG_SHFT	(1)
#define MT6360_VOREG_MASK	GENMASK(7, 1)
/* MT6360_PMU_CHG_CTRL5 */
#define MT6360_VOBST_MASK	GENMASK(7, 2)
/* MT6360_PMU_CHG_CTRL6 */
#define MT6360_VMIVR_SHFT      (1)
#define MT6360_VMIVR_MASK      GENMASK(7, 1)
/* MT6360_PMU_CHG_CTRL7 */
#define MT6360_ICHG_SHFT	(2)
#define MT6360_ICHG_MASK	GENMASK(7, 2)
/* MT6360_PMU_CHG_CTRL8 */
#define MT6360_IPREC_SHFT	(0)
#define MT6360_IPREC_MASK	GENMASK(3, 0)
/* MT6360_PMU_CHG_CTRL9 */
#define MT6360_IEOC_SHFT	(4)
#define MT6360_IEOC_MASK	GENMASK(7, 4)
/* MT6360_PMU_CHG_CTRL10 */
#define MT6360_OTG_OC_MASK	GENMASK(3, 0)
/* MT6360_PMU_DEVICE_TYPE */
#define MT6360_USBCHGEN_MASK	BIT(7)
/* MT6360_PMU_USB_STATUS1 */
#define MT6360_USB_STATUS_SHFT	(4)
#define MT6360_USB_STATUS_MASK	GENMASK(6, 4)
/* MT6360_PMU_CHG_STAT */
#define MT6360_CHG_STAT_SHFT	(6)
#define MT6360_CHG_STAT_MASK	GENMASK(7, 6)
#define MT6360_VBAT_LVL_MASK	BIT(5)
/* MT6360_PMU_CHG_CTRL19 */
#define MT6360_VINOVP_SHFT	(5)
#define MT6360_VINOVP_MASK	GENMASK(6, 5)
/* MT6360_PMU_FOD_STAT */
#define MT6360_CHRDET_EXT_MASK	BIT(4)

/* uV */
#define MT6360_VMIVR_MIN	3900000
#define MT6360_VMIVR_MAX	13400000
#define MT6360_VMIVR_STEP	100000
>>>>>>> upstream/android-13
/* uA */
#define MT6360_ICHG_MIN		100000
#define MT6360_ICHG_MAX		5000000
#define MT6360_ICHG_STEP	100000
/* uV */
#define MT6360_VOREG_MIN	3900000
#define MT6360_VOREG_MAX	4710000
#define MT6360_VOREG_STEP	10000
/* uA */
#define MT6360_AICR_MIN		100000
#define MT6360_AICR_MAX		3250000
#define MT6360_AICR_STEP	50000
/* uA */
#define MT6360_IPREC_MIN	100000
#define MT6360_IPREC_MAX	850000
#define MT6360_IPREC_STEP	50000
/* uA */
#define MT6360_IEOC_MIN		100000
#define MT6360_IEOC_MAX		850000
#define MT6360_IEOC_STEP	50000
<<<<<<< HEAD
/* uV */
#define MT6360_OCV_STEP		1250

/* bc12 icl and ccl setting : uA */
#define ICL_CHGTYPE_SDP		500000
#define ICL_CHGTYPE_SDPNSTD	500000
#define ICL_CHGTYPE_CDP		1500000
#define ICL_CHGTYPE_DCP		2000000
#define CCL_CHGTYPE_UNFREEZE	3000000

struct mt6360_chg_platform_data {
	u32 vinovp;
=======

enum {
	MT6360_RANGE_VMIVR,
	MT6360_RANGE_ICHG,
	MT6360_RANGE_VOREG,
	MT6360_RANGE_AICR,
	MT6360_RANGE_IPREC,
	MT6360_RANGE_IEOC,
	MT6360_RANGE_MAX,
};

#define MT6360_LINEAR_RANGE(idx, _min, _min_sel, _max_sel, _step) \
	[idx] = REGULATOR_LINEAR_RANGE(_min, _min_sel, _max_sel, _step)

static const struct linear_range mt6360_chg_range[MT6360_RANGE_MAX] = {
	MT6360_LINEAR_RANGE(MT6360_RANGE_VMIVR, 3900000, 0, 0x5F, 100000),
	MT6360_LINEAR_RANGE(MT6360_RANGE_ICHG, 100000, 0, 0x31, 100000),
	MT6360_LINEAR_RANGE(MT6360_RANGE_VOREG, 3900000, 0, 0x51, 10000),
	MT6360_LINEAR_RANGE(MT6360_RANGE_AICR, 100000, 0, 0x3F, 50000),
	MT6360_LINEAR_RANGE(MT6360_RANGE_IPREC, 100000, 0, 0x0F, 50000),
	MT6360_LINEAR_RANGE(MT6360_RANGE_IEOC, 100000, 0, 0x0F, 50000),
>>>>>>> upstream/android-13
};

struct mt6360_chg_info {
	struct device *dev;
<<<<<<< HEAD
	struct mt6360_chg_platform_data *pdata;
	struct regmap *regmap;
	struct extcon_dev *edev;
	struct power_supply_desc psy_desc;
	struct power_supply *psy;
	struct regulator_desc otg_rdesc;
	struct regulator_dev *otg_rdev;
	struct mutex chgdet_lock;
=======
	struct regmap *regmap;
	struct power_supply_desc psy_desc;
	struct power_supply *psy;
	struct regulator_dev *otg_rdev;
	struct mutex chgdet_lock;
	u32 vinovp;
>>>>>>> upstream/android-13
	bool pwr_rdy;
	bool bc12_en;
	int psy_usb_type;
	struct work_struct chrdet_work;
};

<<<<<<< HEAD
static struct mt6360_chg_platform_data def_platform_data = {
	.vinovp = 6500000,
};

=======
>>>>>>> upstream/android-13
enum mt6360_iinlmtsel {
	MT6360_IINLMTSEL_AICR_3250 = 0,
	MT6360_IINLMTSEL_CHG_TYPE,
	MT6360_IINLMTSEL_AICR,
	MT6360_IINLMTSEL_LOWER_LEVEL,
};

enum mt6360_pmu_chg_type {
	MT6360_CHG_TYPE_NOVBUS = 0,
	MT6360_CHG_TYPE_UNDER_GOING,
	MT6360_CHG_TYPE_SDP,
	MT6360_CHG_TYPE_SDPNSTD,
	MT6360_CHG_TYPE_DCP,
	MT6360_CHG_TYPE_CDP,
<<<<<<< HEAD
=======
	MT6360_CHG_TYPE_DISABLE_BC12,
>>>>>>> upstream/android-13
	MT6360_CHG_TYPE_MAX,
};

static enum power_supply_usb_type mt6360_charger_usb_types[] = {
	POWER_SUPPLY_USB_TYPE_UNKNOWN,
	POWER_SUPPLY_USB_TYPE_SDP,
	POWER_SUPPLY_USB_TYPE_DCP,
	POWER_SUPPLY_USB_TYPE_CDP,
<<<<<<< HEAD
	POWER_SUPPLY_USB_TYPE_C,
	POWER_SUPPLY_USB_TYPE_PD,
	POWER_SUPPLY_USB_TYPE_PD_DRP,
	POWER_SUPPLY_USB_TYPE_APPLE_BRICK_ID
};

static inline unsigned int mt6360_trans_reg_sel(u32 data, u32 min_val, u32 step,
						u32 max_sel)
{
	unsigned int target = 0;

	if (data >= min_val)
		target = (data - min_val) / step;
	if (target > max_sel)
		target = max_sel;
	return target;
}

static inline int mt6360_get_chrdet_ext_stat(struct mt6360_chg_info *mci,
=======
};

static int mt6360_get_chrdet_ext_stat(struct mt6360_chg_info *mci,
>>>>>>> upstream/android-13
					     bool *pwr_rdy)
{
	int ret;
	unsigned int regval;

	ret = regmap_read(mci->regmap, MT6360_PMU_FOD_STAT, &regval);
	if (ret < 0)
		return ret;
<<<<<<< HEAD
	*pwr_rdy = (regval & MT6360_MASK_CHRDET_EXT) ? true : false;
=======
	*pwr_rdy = (regval & MT6360_CHRDET_EXT_MASK) ? true : false;
>>>>>>> upstream/android-13
	return 0;
}

static int mt6360_charger_get_online(struct mt6360_chg_info *mci,
				     union power_supply_propval *val)
{
	int ret;
	bool pwr_rdy;

	ret = mt6360_get_chrdet_ext_stat(mci, &pwr_rdy);
	if (ret < 0)
		return ret;
	val->intval = pwr_rdy ? true : false;
	return 0;
}

static int mt6360_charger_get_status(struct mt6360_chg_info *mci,
				     union power_supply_propval *val)
{
	int status, ret;
	unsigned int regval;
	bool pwr_rdy;

	ret = mt6360_get_chrdet_ext_stat(mci, &pwr_rdy);
	if (ret < 0)
		return ret;
	if (!pwr_rdy) {
		status = POWER_SUPPLY_STATUS_DISCHARGING;
		goto out;
	}

	ret = regmap_read(mci->regmap, MT6360_PMU_CHG_STAT, &regval);
	if (ret < 0)
		return ret;
<<<<<<< HEAD
	regval &= MT6360_MASK_CHG_STAT;
	regval >>= MT6360_SHFT_CHG_STAT;
=======
	regval &= MT6360_CHG_STAT_MASK;
	regval >>= MT6360_CHG_STAT_SHFT;
>>>>>>> upstream/android-13
	switch (regval) {
	case 0x0:
		status = POWER_SUPPLY_STATUS_NOT_CHARGING;
		break;
	case 0x1:
		status = POWER_SUPPLY_STATUS_CHARGING;
		break;
	case 0x2:
		status = POWER_SUPPLY_STATUS_FULL;
		break;
	default:
		ret = -EIO;
	}
out:
	if (!ret)
		val->intval = status;
	return ret;
}

<<<<<<< HEAD
static int mt6360_charger_get_ocv(struct mt6360_chg_info *mci,
				  union power_supply_propval *val)
{
	int ret;
	unsigned int regval[2];

	ret = regmap_raw_read(mci->regmap,
			      MT6360_PMU_ADC_BAT_DATA_H, regval, 2);
	if (ret < 0)
		return ret;
	val->intval = MT6360_OCV_STEP * (regval[0] << 8 | regval[1]);
	return 0;
}

=======
>>>>>>> upstream/android-13
static int mt6360_charger_get_charge_type(struct mt6360_chg_info *mci,
					  union power_supply_propval *val)
{
	int type, ret;
	unsigned int regval;
	u8 chg_stat;

	ret = regmap_read(mci->regmap, MT6360_PMU_CHG_STAT, &regval);
	if (ret < 0)
		return ret;
<<<<<<< HEAD
	chg_stat = (regval & MT6360_MASK_CHG_STAT) >> MT6360_SHFT_CHG_STAT;
	switch (chg_stat) {
	case 0x00: /* Not Charging */
		type = POWER_SUPPLY_CHARGE_TYPE_NONE;
		break;
	case 0x01: /* Charge in Progress */
		if (regval & MT6360_MASK_VBAT_LVL)
=======

	chg_stat = (regval & MT6360_CHG_STAT_MASK) >> MT6360_CHG_STAT_SHFT;
	switch (chg_stat) {
	case 0x01: /* Charge in Progress */
		if (regval & MT6360_VBAT_LVL_MASK)
>>>>>>> upstream/android-13
			type = POWER_SUPPLY_CHARGE_TYPE_FAST;
		else
			type = POWER_SUPPLY_CHARGE_TYPE_TRICKLE;
		break;
<<<<<<< HEAD
	case 0x02: /* Charge Done */
		type = POWER_SUPPLY_CHARGE_TYPE_NONE;
		break;
	case 0x03: /* Charge Fault */
		type = POWER_SUPPLY_STATUS_UNKNOWN;
		break;
	default:
		break;
	}
=======
	case 0x00: /* Not Charging */
	case 0x02: /* Charge Done */
	case 0x03: /* Charge Fault */
	default:
		type = POWER_SUPPLY_CHARGE_TYPE_NONE;
		break;
	}

>>>>>>> upstream/android-13
	val->intval = type;
	return 0;
}

static int mt6360_charger_get_ichg(struct mt6360_chg_info *mci,
				   union power_supply_propval *val)
{
<<<<<<< HEAD
	int ichg, ret;
	unsigned int regval;

	ret = regmap_read(mci->regmap, MT6360_PMU_CHG_CTRL7, &regval);
	if (ret < 0)
		return ret;
	regval = (regval & MT6360_MASK_ICHG) >> MT6360_SHFT_ICHG;
	ichg = MT6360_ICHG_MIN + (regval * MT6360_ICHG_STEP);
	if (ichg > MT6360_ICHG_MAX)
		ichg = MT6360_ICHG_MAX;
	val->intval = ichg;
	return 0;
=======
	int ret;
	u32 sel, value;

	ret = regmap_read(mci->regmap, MT6360_PMU_CHG_CTRL7, &sel);
	if (ret < 0)
		return ret;
	sel = (sel & MT6360_ICHG_MASK) >> MT6360_ICHG_SHFT;
	ret = linear_range_get_value(&mt6360_chg_range[MT6360_RANGE_ICHG], sel, &value);
	if (!ret)
		val->intval = value;
	return ret;
>>>>>>> upstream/android-13
}

static int mt6360_charger_get_max_ichg(struct mt6360_chg_info *mci,
				       union power_supply_propval *val)
{
	val->intval = MT6360_ICHG_MAX;
	return 0;
}

static int mt6360_charger_get_cv(struct mt6360_chg_info *mci,
				 union power_supply_propval *val)
{
<<<<<<< HEAD
	int cv, ret;
	unsigned int regval;

	ret = regmap_read(mci->regmap, MT6360_PMU_CHG_CTRL4, &regval);
	if (ret < 0)
		return ret;
	regval = (regval & MT6360_MASK_VOREG) >> MT6360_SHFT_VOREG;
	cv = MT6360_VOREG_MIN + (regval * MT6360_VOREG_STEP);
	if (cv > MT6360_VOREG_MAX)
		cv = MT6360_VOREG_MAX;
	val->intval = cv;
	return 0;
=======
	int ret;
	u32 sel, value;

	ret = regmap_read(mci->regmap, MT6360_PMU_CHG_CTRL4, &sel);
	if (ret < 0)
		return ret;
	sel = (sel & MT6360_VOREG_MASK) >> MT6360_VOREG_SHFT;
	ret = linear_range_get_value(&mt6360_chg_range[MT6360_RANGE_VOREG], sel, &value);
	if (!ret)
		val->intval = value;
	return ret;
>>>>>>> upstream/android-13
}

static int mt6360_charger_get_max_cv(struct mt6360_chg_info *mci,
				     union power_supply_propval *val)
{
	val->intval = MT6360_VOREG_MAX;
	return 0;
}

static int mt6360_charger_get_aicr(struct mt6360_chg_info *mci,
				   union power_supply_propval *val)
{
	int ret;
<<<<<<< HEAD
	unsigned int regval;

	ret = regmap_read(mci->regmap, MT6360_PMU_CHG_CTRL3, &regval);
	if (ret < 0)
		return ret;
	regval = (regval & MT6360_MASK_IAICR) >> MT6360_SHFT_IAICR;
	val->intval = MT6360_AICR_MIN + (regval * MT6360_AICR_STEP);
	return 0;
=======
	u32 sel, value;

	ret = regmap_read(mci->regmap, MT6360_PMU_CHG_CTRL3, &sel);
	if (ret < 0)
		return ret;
	sel = (sel & MT6360_IAICR_MASK) >> MT6360_IAICR_SHFT;
	ret = linear_range_get_value(&mt6360_chg_range[MT6360_RANGE_AICR], sel, &value);
	if (!ret)
		val->intval = value;
	return ret;
}

static int mt6360_charger_get_mivr(struct mt6360_chg_info *mci,
				   union power_supply_propval *val)
{
	int ret;
	u32 sel, value;

	ret = regmap_read(mci->regmap, MT6360_PMU_CHG_CTRL6, &sel);
	if (ret < 0)
		return ret;
	sel = (sel & MT6360_VMIVR_MASK) >> MT6360_VMIVR_SHFT;
	ret = linear_range_get_value(&mt6360_chg_range[MT6360_RANGE_VMIVR], sel, &value);
	if (!ret)
		val->intval = value;
	return ret;
>>>>>>> upstream/android-13
}

static int mt6360_charger_get_iprechg(struct mt6360_chg_info *mci,
				      union power_supply_propval *val)
{
	int ret;
<<<<<<< HEAD
	unsigned int regval;

	ret = regmap_read(mci->regmap, MT6360_PMU_CHG_CTRL8, &regval);
	if (ret < 0)
		return ret;
	regval = (regval & MT6360_MASK_IPREC) >> MT6360_SHFT_IPREC;
	val->intval = MT6360_IPREC_MIN + (regval * MT6360_IPREC_STEP);
	return 0;
=======
	u32 sel, value;

	ret = regmap_read(mci->regmap, MT6360_PMU_CHG_CTRL8, &sel);
	if (ret < 0)
		return ret;
	sel = (sel & MT6360_IPREC_MASK) >> MT6360_IPREC_SHFT;
	ret = linear_range_get_value(&mt6360_chg_range[MT6360_RANGE_IPREC], sel, &value);
	if (!ret)
		val->intval = value;
	return ret;
>>>>>>> upstream/android-13
}

static int mt6360_charger_get_ieoc(struct mt6360_chg_info *mci,
				   union power_supply_propval *val)
{
	int ret;
<<<<<<< HEAD
	unsigned int regval;

	ret = regmap_read(mci->regmap, MT6360_PMU_CHG_CTRL9, &regval);
	if (ret < 0)
		return ret;
	regval = (regval & MT6360_MASK_IEOC) >> MT6360_SHFT_IEOC;
	val->intval = MT6360_IEOC_MIN + (regval * MT6360_IEOC_STEP);
	return 0;
=======
	u32 sel, value;

	ret = regmap_read(mci->regmap, MT6360_PMU_CHG_CTRL9, &sel);
	if (ret < 0)
		return ret;
	sel = (sel & MT6360_IEOC_MASK) >> MT6360_IEOC_SHFT;
	ret = linear_range_get_value(&mt6360_chg_range[MT6360_RANGE_IEOC], sel, &value);
	if (!ret)
		val->intval = value;
	return ret;
>>>>>>> upstream/android-13
}

static int mt6360_charger_set_online(struct mt6360_chg_info *mci,
				     const union power_supply_propval *val)
{
	u8 force_sleep = val->intval ? 0 : 1;

<<<<<<< HEAD
	return regmap_update_bits(mci->regmap, MT6360_PMU_CHG_CTRL1,
				  MT6360_MASK_FSLP,
				  force_sleep << MT6360_SHFT_FSLP);
=======
	return regmap_update_bits(mci->regmap,
				  MT6360_PMU_CHG_CTRL1,
				  MT6360_FSLP_MASK,
				  force_sleep << MT6360_FSLP_SHFT);
>>>>>>> upstream/android-13
}

static int mt6360_charger_set_ichg(struct mt6360_chg_info *mci,
				   const union power_supply_propval *val)
{
<<<<<<< HEAD
	unsigned int regval;

	regval = mt6360_trans_reg_sel(val->intval, 100000, 100000, 0x31);
	return regmap_update_bits(mci->regmap, MT6360_PMU_CHG_CTRL7,
				  MT6360_MASK_ICHG, regval << MT6360_SHFT_ICHG);
=======
	u32 sel;

	linear_range_get_selector_within(&mt6360_chg_range[MT6360_RANGE_ICHG], val->intval, &sel);
	return regmap_update_bits(mci->regmap,
				  MT6360_PMU_CHG_CTRL7,
				  MT6360_ICHG_MASK,
				  sel << MT6360_ICHG_SHFT);
>>>>>>> upstream/android-13
}

static int mt6360_charger_set_cv(struct mt6360_chg_info *mci,
				 const union power_supply_propval *val)
{
<<<<<<< HEAD
	unsigned int regval;

	regval = mt6360_trans_reg_sel(val->intval, 3900000, 10000, 0x51);
	return regmap_update_bits(mci->regmap,
				  MT6360_PMU_CHG_CTRL4,
				  MT6360_MASK_VOREG,
				  regval << MT6360_SHFT_VOREG);
=======
	u32 sel;

	linear_range_get_selector_within(&mt6360_chg_range[MT6360_RANGE_VOREG], val->intval, &sel);
	return regmap_update_bits(mci->regmap,
				  MT6360_PMU_CHG_CTRL4,
				  MT6360_VOREG_MASK,
				  sel << MT6360_VOREG_SHFT);
>>>>>>> upstream/android-13
}

static int mt6360_charger_set_aicr(struct mt6360_chg_info *mci,
				   const union power_supply_propval *val)
{
<<<<<<< HEAD
	unsigned int regval;

	regval = mt6360_trans_reg_sel(val->intval, 100000, 50000, 0x3F);
	return regmap_update_bits(mci->regmap,
				  MT6360_PMU_CHG_CTRL3,
				  MT6360_MASK_IAICR,
				  regval << MT6360_SHFT_IAICR);
=======
	u32 sel;

	linear_range_get_selector_within(&mt6360_chg_range[MT6360_RANGE_AICR], val->intval, &sel);
	return regmap_update_bits(mci->regmap,
				  MT6360_PMU_CHG_CTRL3,
				  MT6360_IAICR_MASK,
				  sel << MT6360_IAICR_SHFT);
}

static int mt6360_charger_set_mivr(struct mt6360_chg_info *mci,
				   const union power_supply_propval *val)
{
	u32 sel;

	linear_range_get_selector_within(&mt6360_chg_range[MT6360_RANGE_VMIVR], val->intval, &sel);
	return regmap_update_bits(mci->regmap,
				  MT6360_PMU_CHG_CTRL3,
				  MT6360_VMIVR_MASK,
				  sel << MT6360_VMIVR_SHFT);
>>>>>>> upstream/android-13
}

static int mt6360_charger_set_iprechg(struct mt6360_chg_info *mci,
				      const union power_supply_propval *val)
{
<<<<<<< HEAD
	unsigned int regval;

	regval = mt6360_trans_reg_sel(val->intval, 100000, 50000, 0x0F);
	return regmap_update_bits(mci->regmap,
				  MT6360_PMU_CHG_CTRL8,
				  MT6360_MASK_IPREC,
				  regval << MT6360_SHFT_IPREC);
=======
	u32 sel;

	linear_range_get_selector_within(&mt6360_chg_range[MT6360_RANGE_IPREC], val->intval, &sel);
	return regmap_update_bits(mci->regmap,
				  MT6360_PMU_CHG_CTRL8,
				  MT6360_IPREC_MASK,
				  sel << MT6360_IPREC_SHFT);
>>>>>>> upstream/android-13
}

static int mt6360_charger_set_ieoc(struct mt6360_chg_info *mci,
				   const union power_supply_propval *val)
{
<<<<<<< HEAD
	unsigned int regval;

	regval = mt6360_trans_reg_sel(val->intval, 100000, 50000, 0x0F);
	return regmap_update_bits(mci->regmap,
				  MT6360_PMU_CHG_CTRL9,
				  MT6360_MASK_IEOC,
				  regval << MT6360_SHFT_IEOC);
=======
	u32 sel;

	linear_range_get_selector_within(&mt6360_chg_range[MT6360_RANGE_IEOC], val->intval, &sel);
	return regmap_update_bits(mci->regmap,
				  MT6360_PMU_CHG_CTRL9,
				  MT6360_IEOC_MASK,
				  sel << MT6360_IEOC_SHFT);
>>>>>>> upstream/android-13
}

static int mt6360_charger_get_property(struct power_supply *psy,
				       enum power_supply_property psp,
				       union power_supply_propval *val)
{
	struct mt6360_chg_info *mci = power_supply_get_drvdata(psy);
	int ret = 0;

<<<<<<< HEAD
	dev_dbg(mci->dev, "%s: prop = %d\n", __func__, psp);
=======
>>>>>>> upstream/android-13
	switch (psp) {
	case POWER_SUPPLY_PROP_ONLINE:
		ret = mt6360_charger_get_online(mci, val);
		break;
	case POWER_SUPPLY_PROP_STATUS:
		ret = mt6360_charger_get_status(mci, val);
		break;
<<<<<<< HEAD
	case POWER_SUPPLY_PROP_VOLTAGE_OCV:
		ret = mt6360_charger_get_ocv(mci, val);
		break;
=======
>>>>>>> upstream/android-13
	case POWER_SUPPLY_PROP_CHARGE_TYPE:
		ret = mt6360_charger_get_charge_type(mci, val);
		break;
	case POWER_SUPPLY_PROP_CONSTANT_CHARGE_CURRENT:
		ret = mt6360_charger_get_ichg(mci, val);
		break;
	case POWER_SUPPLY_PROP_CONSTANT_CHARGE_CURRENT_MAX:
		ret = mt6360_charger_get_max_ichg(mci, val);
		break;
	case POWER_SUPPLY_PROP_CONSTANT_CHARGE_VOLTAGE:
		ret = mt6360_charger_get_cv(mci, val);
		break;
	case POWER_SUPPLY_PROP_CONSTANT_CHARGE_VOLTAGE_MAX:
		ret = mt6360_charger_get_max_cv(mci, val);
		break;
	case POWER_SUPPLY_PROP_INPUT_CURRENT_LIMIT:
		ret = mt6360_charger_get_aicr(mci, val);
		break;
<<<<<<< HEAD
=======
	case POWER_SUPPLY_PROP_INPUT_VOLTAGE_LIMIT:
		ret = mt6360_charger_get_mivr(mci, val);
		break;
>>>>>>> upstream/android-13
	case POWER_SUPPLY_PROP_PRECHARGE_CURRENT:
		ret = mt6360_charger_get_iprechg(mci, val);
		break;
	case POWER_SUPPLY_PROP_CHARGE_TERM_CURRENT:
		ret = mt6360_charger_get_ieoc(mci, val);
		break;
	case POWER_SUPPLY_PROP_USB_TYPE:
		val->intval = mci->psy_usb_type;
		break;
	default:
		ret = -ENODATA;
	}
	return ret;
}

static int mt6360_charger_set_property(struct power_supply *psy,
				       enum power_supply_property psp,
				       const union power_supply_propval *val)
{
	struct mt6360_chg_info *mci = power_supply_get_drvdata(psy);
	int ret;

<<<<<<< HEAD
	dev_dbg(mci->dev, "%s: prop = %d\n", __func__, psp);
=======
>>>>>>> upstream/android-13
	switch (psp) {
	case POWER_SUPPLY_PROP_ONLINE:
		ret = mt6360_charger_set_online(mci, val);
		break;
	case POWER_SUPPLY_PROP_CONSTANT_CHARGE_CURRENT:
		ret = mt6360_charger_set_ichg(mci, val);
		break;
	case POWER_SUPPLY_PROP_CONSTANT_CHARGE_VOLTAGE:
		ret = mt6360_charger_set_cv(mci, val);
		break;
	case POWER_SUPPLY_PROP_INPUT_CURRENT_LIMIT:
		ret = mt6360_charger_set_aicr(mci, val);
		break;
<<<<<<< HEAD
=======
	case POWER_SUPPLY_PROP_INPUT_VOLTAGE_LIMIT:
		ret = mt6360_charger_set_mivr(mci, val);
		break;
>>>>>>> upstream/android-13
	case POWER_SUPPLY_PROP_PRECHARGE_CURRENT:
		ret = mt6360_charger_set_iprechg(mci, val);
		break;
	case POWER_SUPPLY_PROP_CHARGE_TERM_CURRENT:
		ret = mt6360_charger_set_ieoc(mci, val);
		break;
	default:
		ret = -EINVAL;
	}
	return ret;
}

static int mt6360_charger_property_is_writeable(struct power_supply *psy,
					       enum power_supply_property psp)
{
	switch (psp) {
	case POWER_SUPPLY_PROP_ONLINE:
	case POWER_SUPPLY_PROP_CONSTANT_CHARGE_CURRENT:
	case POWER_SUPPLY_PROP_CONSTANT_CHARGE_VOLTAGE:
	case POWER_SUPPLY_PROP_INPUT_CURRENT_LIMIT:
<<<<<<< HEAD
=======
	case POWER_SUPPLY_PROP_INPUT_VOLTAGE_LIMIT:
>>>>>>> upstream/android-13
	case POWER_SUPPLY_PROP_PRECHARGE_CURRENT:
	case POWER_SUPPLY_PROP_CHARGE_TERM_CURRENT:
		return 1;
	default:
		return 0;
	}
}

static enum power_supply_property mt6360_charger_properties[] = {
	POWER_SUPPLY_PROP_ONLINE,
	POWER_SUPPLY_PROP_STATUS,
	POWER_SUPPLY_PROP_CHARGE_TYPE,
<<<<<<< HEAD
	POWER_SUPPLY_PROP_VOLTAGE_OCV,
=======
>>>>>>> upstream/android-13
	POWER_SUPPLY_PROP_CONSTANT_CHARGE_CURRENT,
	POWER_SUPPLY_PROP_CONSTANT_CHARGE_CURRENT_MAX,
	POWER_SUPPLY_PROP_CONSTANT_CHARGE_VOLTAGE,
	POWER_SUPPLY_PROP_CONSTANT_CHARGE_VOLTAGE_MAX,
	POWER_SUPPLY_PROP_INPUT_CURRENT_LIMIT,
<<<<<<< HEAD
=======
	POWER_SUPPLY_PROP_INPUT_VOLTAGE_LIMIT,
>>>>>>> upstream/android-13
	POWER_SUPPLY_PROP_PRECHARGE_CURRENT,
	POWER_SUPPLY_PROP_CHARGE_TERM_CURRENT,
	POWER_SUPPLY_PROP_USB_TYPE,
};

<<<<<<< HEAD
static char *mt6360_charger_supplied_to[] = {
	"battery"
};

=======
>>>>>>> upstream/android-13
static const struct power_supply_desc mt6360_charger_desc = {
	.type			= POWER_SUPPLY_TYPE_USB,
	.properties		= mt6360_charger_properties,
	.num_properties		= ARRAY_SIZE(mt6360_charger_properties),
	.get_property		= mt6360_charger_get_property,
	.set_property		= mt6360_charger_set_property,
	.property_is_writeable	= mt6360_charger_property_is_writeable,
	.usb_types		= mt6360_charger_usb_types,
	.num_usb_types		= ARRAY_SIZE(mt6360_charger_usb_types),
};

static const struct regulator_ops mt6360_chg_otg_ops = {
	.list_voltage = regulator_list_voltage_linear,
	.enable = regulator_enable_regmap,
	.disable = regulator_disable_regmap,
	.is_enabled = regulator_is_enabled_regmap,
	.set_voltage_sel = regulator_set_voltage_sel_regmap,
	.get_voltage_sel = regulator_get_voltage_sel_regmap,
};

static const struct regulator_desc mt6360_otg_rdesc = {
	.of_match = "usb-otg-vbus",
	.name = "usb-otg-vbus",
	.ops = &mt6360_chg_otg_ops,
	.owner = THIS_MODULE,
	.type = REGULATOR_VOLTAGE,
	.min_uV = 4425000,
<<<<<<< HEAD
	.uV_step = 25000, /* step  25mV */
	.n_voltages = 57, /* 4425mV to 5825mV */
	.vsel_reg = MT6360_PMU_CHG_CTRL5,
	.vsel_mask = MT6360_MASK_VOBST,
	.enable_reg = MT6360_PMU_CHG_CTRL1,
	.enable_mask = MT6360_MASK_OPA_MODE,
};

static const unsigned int mt6360_extcon_cable[] = {
	EXTCON_USB,
	EXTCON_USB_HOST,
	EXTCON_CHG_USB_SDP,
	EXTCON_CHG_USB_DCP,
	EXTCON_CHG_USB_CDP,
	EXTCON_CHG_USB_SLOW, /* For NonStandard Charger */
	EXTCON_NONE,
};

static irqreturn_t mt6360_pmu_chg_treg_evt_handler(int irq, void *data)
{
	struct mt6360_chg_info *mci = data;

	dev_warn(mci->dev, "%s\n", __func__);
	return IRQ_HANDLED;
}

static irqreturn_t mt6360_pmu_pwr_rdy_evt_handler(int irq, void *data)
{
	struct mt6360_chg_info *mci = data;

	dev_dbg(mci->dev, "%s\n", __func__);
	return IRQ_HANDLED;
}

static irqreturn_t mt6360_pmu_chg_batsysuv_evt_handler(int irq, void *data)
{
	struct mt6360_chg_info *mci = data;

	dev_warn(mci->dev, "%s\n", __func__);
	return IRQ_HANDLED;
}

static irqreturn_t mt6360_pmu_chg_vsysuv_evt_handler(int irq, void *data)
{
	struct mt6360_chg_info *mci = data;

	dev_warn(mci->dev, "%s\n", __func__);
	return IRQ_HANDLED;
}

static irqreturn_t mt6360_pmu_chg_vsysov_evt_handler(int irq, void *data)
{
	struct mt6360_chg_info *mci = data;

	dev_warn(mci->dev, "%s\n", __func__);
	return IRQ_HANDLED;
}

static irqreturn_t mt6360_pmu_chg_vbatov_evt_handler(int irq, void *data)
{
	struct mt6360_chg_info *mci = data;

	dev_warn(mci->dev, "%s\n", __func__);
	return IRQ_HANDLED;
}

static irqreturn_t mt6360_pmu_chg_vbusov_evt_handler(int irq, void *data)
{
	struct mt6360_chg_info *mci = data;

	dev_warn(mci->dev, "%s\n", __func__);
	return IRQ_HANDLED;
}

static irqreturn_t mt6360_pmu_chg_aiccmeasl_handler(int irq, void *data)
{
	struct mt6360_chg_info *mci = data;

	dev_dbg(mci->dev, "%s\n", __func__);
	return IRQ_HANDLED;
}

static irqreturn_t mt6360_pmu_wdtmri_handler(int irq, void *data)
{
	struct mt6360_chg_info *mci = data;

	dev_dbg(mci->dev, "%s\n", __func__);
	return IRQ_HANDLED;
}

static irqreturn_t mt6360_pmu_chg_rechgi_handler(int irq, void *data)
{
	struct mt6360_chg_info *mci = data;

	dev_dbg(mci->dev, "%s\n", __func__);
	return IRQ_HANDLED;
}

static irqreturn_t mt6360_pmu_chg_termi_handler(int irq, void *data)
{
	struct mt6360_chg_info *mci = data;

	dev_dbg(mci->dev, "%s\n", __func__);
	return IRQ_HANDLED;
}

static irqreturn_t mt6360_pmu_chg_ieoci_handler(int irq, void *data)
{
	struct mt6360_chg_info *mci = data;

	dev_dbg(mci->dev, "%s\n", __func__);
	return IRQ_HANDLED;
}

static irqreturn_t mt6360_pmu_pumpx_donei_handler(int irq, void *data)
{
	struct mt6360_chg_info *mci = data;

	dev_dbg(mci->dev, "%s\n", __func__);
	return IRQ_HANDLED;
}

static irqreturn_t mt6360_pmu_attach_i_handler(int irq, void *data)
{
	struct mt6360_chg_info *mci = data;
	int ret, chg_type = EXTCON_NONE;
	unsigned int usb_status;
	int last_usb_type;
	int current_limit = 0;
	union power_supply_propval val;

	dev_dbg(mci->dev, "%s\n", __func__);
	mutex_lock(&mci->chgdet_lock);
	if (!mci->bc12_en) {
		dev_info(mci->dev, "%s: bc12 disabled, ignore irq\n", __func__);
=======
	.uV_step = 25000,
	.n_voltages = 57,
	.vsel_reg = MT6360_PMU_CHG_CTRL5,
	.vsel_mask = MT6360_VOBST_MASK,
	.enable_reg = MT6360_PMU_CHG_CTRL1,
	.enable_mask = MT6360_OPA_MODE_MASK,
};

static irqreturn_t mt6360_pmu_attach_i_handler(int irq, void *data)
{
	struct mt6360_chg_info *mci = data;
	int ret;
	unsigned int usb_status;
	int last_usb_type;

	mutex_lock(&mci->chgdet_lock);
	if (!mci->bc12_en) {
		dev_warn(mci->dev, "Received attach interrupt, bc12 disabled, ignore irq\n");
>>>>>>> upstream/android-13
		goto out;
	}
	last_usb_type = mci->psy_usb_type;
	/* Plug in */
	ret = regmap_read(mci->regmap, MT6360_PMU_USB_STATUS1, &usb_status);
	if (ret < 0)
		goto out;
<<<<<<< HEAD
	usb_status &= MT6360_MASK_USB_STATUS;
	usb_status >>= MT6360_SHFT_USB_STATUS;
	switch (usb_status) {
	case MT6360_CHG_TYPE_UNDER_GOING:
		dev_info(mci->dev, "%s: under going...\n", __func__);
		goto out;
	case MT6360_CHG_TYPE_SDP:
		chg_type = EXTCON_CHG_USB_SDP;
		mci->psy_usb_type = POWER_SUPPLY_USB_TYPE_SDP;
		current_limit = ICL_CHGTYPE_SDP;
		break;
	case MT6360_CHG_TYPE_SDPNSTD:
		chg_type = EXTCON_CHG_USB_SLOW;
		mci->psy_usb_type = POWER_SUPPLY_USB_TYPE_SDP;
		current_limit = ICL_CHGTYPE_SDPNSTD;
		break;
	case MT6360_CHG_TYPE_CDP:
		chg_type = EXTCON_CHG_USB_CDP;
		mci->psy_usb_type = POWER_SUPPLY_USB_TYPE_CDP;
		current_limit = ICL_CHGTYPE_CDP;
		break;
	case MT6360_CHG_TYPE_DCP:
		chg_type = EXTCON_CHG_USB_DCP;
		mci->psy_usb_type = POWER_SUPPLY_USB_TYPE_DCP;
		current_limit = ICL_CHGTYPE_DCP;
		break;
	default:
		dev_warn(mci->dev,
			 "%s: disconnect or unknown status\n", __func__);
		goto out;
	}

	dev_info(mci->dev, "%s: chg_type = %d\n", __func__, chg_type);
	if (chg_type >= EXTCON_CHG_USB_SDP) {
		val.intval = current_limit;
		mt6360_charger_set_aicr(mci, &val);
		val.intval =  CCL_CHGTYPE_UNFREEZE;
		mt6360_charger_set_ichg(mci, &val);
	}

	if (chg_type == EXTCON_CHG_USB_SDP || chg_type == EXTCON_CHG_USB_CDP) {
		extcon_set_state_sync(mci->edev, EXTCON_USB_HOST, false);
		extcon_set_state_sync(mci->edev, EXTCON_USB, true);
	}
	extcon_set_state_sync(mci->edev, chg_type, true);
=======
	usb_status &= MT6360_USB_STATUS_MASK;
	usb_status >>= MT6360_USB_STATUS_SHFT;
	switch (usb_status) {
	case MT6360_CHG_TYPE_NOVBUS:
		dev_dbg(mci->dev, "Received attach interrupt, no vbus\n");
		goto out;
	case MT6360_CHG_TYPE_UNDER_GOING:
		dev_dbg(mci->dev, "Received attach interrupt, under going...\n");
		goto out;
	case MT6360_CHG_TYPE_SDP:
		mci->psy_usb_type = POWER_SUPPLY_USB_TYPE_SDP;
		break;
	case MT6360_CHG_TYPE_SDPNSTD:
		mci->psy_usb_type = POWER_SUPPLY_USB_TYPE_SDP;
		break;
	case MT6360_CHG_TYPE_CDP:
		mci->psy_usb_type = POWER_SUPPLY_USB_TYPE_CDP;
		break;
	case MT6360_CHG_TYPE_DCP:
		mci->psy_usb_type = POWER_SUPPLY_USB_TYPE_DCP;
		break;
	case MT6360_CHG_TYPE_DISABLE_BC12:
		dev_dbg(mci->dev, "Received attach interrupt, bc12 detect not enable\n");
		goto out;
	default:
		mci->psy_usb_type = POWER_SUPPLY_USB_TYPE_UNKNOWN;
		dev_dbg(mci->dev, "Received attach interrupt, reserved address\n");
		goto out;
	}

	dev_dbg(mci->dev, "Received attach interrupt, chg_type = %d\n", mci->psy_usb_type);
>>>>>>> upstream/android-13
	if (last_usb_type != mci->psy_usb_type)
		power_supply_changed(mci->psy);
out:
	mutex_unlock(&mci->chgdet_lock);
	return IRQ_HANDLED;
}

static void mt6360_handle_chrdet_ext_evt(struct mt6360_chg_info *mci)
{
<<<<<<< HEAD
	int i, ret;
	bool pwr_rdy;
	int last_usb_type;
=======
	int ret;
	bool pwr_rdy;
>>>>>>> upstream/android-13

	mutex_lock(&mci->chgdet_lock);
	ret = mt6360_get_chrdet_ext_stat(mci, &pwr_rdy);
	if (ret < 0)
		goto out;
	if (mci->pwr_rdy == pwr_rdy) {
<<<<<<< HEAD
		dev_info(mci->dev,
			 "%s: pwr_rdy is same(%d)\n", __func__, pwr_rdy);
		goto out;
	}
	mci->pwr_rdy = pwr_rdy;
	last_usb_type = mci->psy_usb_type;
	dev_info(mci->dev, "%s: pwr_rdy = %d\n", __func__, pwr_rdy);
	if (!pwr_rdy) {
		for (i = 0; i < ARRAY_SIZE(mt6360_extcon_cable) - 1; i++) {
			extcon_set_state_sync(mci->edev,
					      mt6360_extcon_cable[i], false);
		}
		mci->psy_usb_type = POWER_SUPPLY_USB_TYPE_UNKNOWN;
		if (last_usb_type != mci->psy_usb_type)
			power_supply_changed(mci->psy);
=======
		dev_dbg(mci->dev, "Received vbus interrupt, pwr_rdy is same(%d)\n", pwr_rdy);
		goto out;
	}
	mci->pwr_rdy = pwr_rdy;
	dev_dbg(mci->dev, "Received vbus interrupt, pwr_rdy = %d\n", pwr_rdy);
	if (!pwr_rdy) {
		mci->psy_usb_type = POWER_SUPPLY_USB_TYPE_UNKNOWN;
		power_supply_changed(mci->psy);
>>>>>>> upstream/android-13

	}
	ret = regmap_update_bits(mci->regmap,
				 MT6360_PMU_DEVICE_TYPE,
<<<<<<< HEAD
				 MT6360_MASK_USBCHGEN,
				 pwr_rdy ? MT6360_MASK_USBCHGEN : 0);
=======
				 MT6360_USBCHGEN_MASK,
				 pwr_rdy ? MT6360_USBCHGEN_MASK : 0);
>>>>>>> upstream/android-13
	if (ret < 0)
		goto out;
	mci->bc12_en = pwr_rdy;
out:
	mutex_unlock(&mci->chgdet_lock);
}

static void mt6360_chrdet_work(struct work_struct *work)
{
	struct mt6360_chg_info *mci = (struct mt6360_chg_info *)container_of(
				     work, struct mt6360_chg_info, chrdet_work);

<<<<<<< HEAD
	dev_info(mci->dev, "%s\n", __func__);
=======
>>>>>>> upstream/android-13
	mt6360_handle_chrdet_ext_evt(mci);
}

static irqreturn_t mt6360_pmu_chrdet_ext_evt_handler(int irq, void *data)
{
	struct mt6360_chg_info *mci = data;

<<<<<<< HEAD
	dev_info(mci->dev, "%s\n", __func__);
=======
>>>>>>> upstream/android-13
	mt6360_handle_chrdet_ext_evt(mci);
	return IRQ_HANDLED;
}

<<<<<<< HEAD
static const struct mt6360_pmu_irq_desc mt6360_chg_irq_desc[] = {
	{ "chg_treg_evt", mt6360_pmu_chg_treg_evt_handler },
	{ "pwr_rdy_evt", mt6360_pmu_pwr_rdy_evt_handler },
	{ "chg_batsysuv_evt", mt6360_pmu_chg_batsysuv_evt_handler },
	{ "chg_vsysuv_evt", mt6360_pmu_chg_vsysuv_evt_handler },
	{ "chg_vsysov_evt", mt6360_pmu_chg_vsysov_evt_handler },
	{ "chg_vbatov_evt", mt6360_pmu_chg_vbatov_evt_handler },
	{ "chg_vbusov_evt", mt6360_pmu_chg_vbusov_evt_handler },
	{ "chg_aiccmeasl", mt6360_pmu_chg_aiccmeasl_handler },
	{ "wdtmri", mt6360_pmu_wdtmri_handler },
	{ "chg_rechgi", mt6360_pmu_chg_rechgi_handler },
	{ "chg_termi", mt6360_pmu_chg_termi_handler },
	{ "chg_ieoci", mt6360_pmu_chg_ieoci_handler },
	{ "pumpx_donei", mt6360_pmu_pumpx_donei_handler },
	{ "attach_i", mt6360_pmu_attach_i_handler },
	{ "chrdet_ext_evt", mt6360_pmu_chrdet_ext_evt_handler },
};

static int mt6360_chg_irq_register(struct platform_device *pdev)
{
	const struct mt6360_pmu_irq_desc *irq_desc;
	int i, irq, ret;

	for (i = 0; i < ARRAY_SIZE(mt6360_chg_irq_desc); i++) {
		irq_desc = mt6360_chg_irq_desc + i;
		if (unlikely(!irq_desc->name))
			continue;
		irq = platform_get_irq_byname(pdev, irq_desc->name);
		if (irq < 0)
			continue;
		ret = devm_request_threaded_irq(&pdev->dev, irq, NULL,
						irq_desc->irq_handler,
						IRQF_TRIGGER_FALLING,
						irq_desc->name,
						platform_get_drvdata(pdev));
		if (ret < 0) {
			dev_err(&pdev->dev,
				"request %s irq fail\n", irq_desc->name);
			return ret;
		}
	}
	return 0;
}

struct mt6360_field_info {
	struct device_attribute dev_attr;
	u8 reg;
	u8 mask;
	u8 shft;
};

static ssize_t mt6360_sysfs_show(struct device *dev,
		struct device_attribute *attr, char *buf);
static ssize_t mt6360_sysfs_store(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t count);

#define MT6360_FIELD_RW(_name, _reg, _nbit)			\
{								\
	.dev_attr = __ATTR(_name, 0664,				\
		       mt6360_sysfs_show, mt6360_sysfs_store),	\
	.reg	= MT6360_PMU_##_reg,				\
	.mask	= MT6360_MASK_##_nbit,				\
	.shft	= MT6360_SHFT_##_nbit,				\
}

#define MT6360_FIELD_RO(_name, _reg, _nbit)			\
{								\
	.dev_attr = __ATTR(_name, 0444,				\
			mt6360_sysfs_show, NULL),		\
	.reg	= MT6360_PMU_##_reg,				\
	.mask	= MT6360_MASK_##_nbit,				\
	.shft	= MT6360_SHFT_##_nbit,				\
}

static struct mt6360_field_info mt6360_field_tbl[] = {
	MT6360_FIELD_RW(hiz,		CHG_CTRL1,	HIZ),
	MT6360_FIELD_RW(vmivr,		CHG_CTRL6,	VMIVR),
	MT6360_FIELD_RW(iaicr,		CHG_CTRL3,	IAICR),
	MT6360_FIELD_RW(SYSREG,		CHG_CTRL11,	SYSREG),
	MT6360_FIELD_RW(otg_oc,		CHG_CTRL10,	OTG_OC),
	MT6360_FIELD_RW(ichg,		CHG_CTRL17,	ICHG),
	MT6360_FIELD_RW(iprec,		CHG_CTRL8,	IPREC),
	MT6360_FIELD_RW(ieoc,		CHG_CTRL9,	IEOC),
	MT6360_FIELD_RW(voreg,		CHG_CTRL4,	VOREG),
	MT6360_FIELD_RW(lbp,		CHG_CTRL10,	LBP),
	MT6360_FIELD_RW(vrec,		CHG_CTRL11,	VREC),
	MT6360_FIELD_RW(te,		CHG_CTRL2,	TE),
	MT6360_FIELD_RW(chg_wdt_en,	CHG_CTRL13,	CHG_WDT_EN),
	MT6360_FIELD_RW(chg_wdt,	CHG_CTRL13,	CHG_WDT),
	MT6360_FIELD_RW(wt_fc,		CHG_CTRL12,	WT_FC),
	MT6360_FIELD_RW(bat_comp,	CHG_CTRL18,	BAT_COMP),
	MT6360_FIELD_RW(vclamp,		CHG_CTRL18,	VCLAMP),
	MT6360_FIELD_RW(usbchgen,	DEVICE_TYPE,	USBCHGEN),
	MT6360_FIELD_RW(chg_en,		CHG_CTRL2,	CHG_EN),
	MT6360_FIELD_RO(chrdet_ext,	FOD_STAT,	CHRDET_EXT),
};

static struct attribute *mt6360_attrs[ARRAY_SIZE(mt6360_field_tbl) + 1];

static const struct attribute_group mt6360_attr_group = {
	.attrs = mt6360_attrs,
};

static ssize_t mt6360_sysfs_show(struct device *dev,
				 struct device_attribute *attr, char *buf)
{
	struct mt6360_chg_info *mci = dev_get_drvdata(dev->parent);
	struct mt6360_field_info *info = (void *)attr;
	unsigned int regval;
	int ret;

	ret = regmap_read(mci->regmap, info->reg, &regval);
	if (ret < 0)
		return ret;
	regval = (regval & info->mask) >> info->shft;
	return scnprintf(buf, PAGE_SIZE, "%02x\n", regval);
}

static ssize_t mt6360_sysfs_store(struct device *dev,
		   struct device_attribute *attr, const char *buf, size_t count)
{
	struct mt6360_chg_info *mci = dev_get_drvdata(dev->parent);
	struct mt6360_field_info *info = (void *)attr;
	int ret, tmp;

	if (kstrtoint(buf, 10, &tmp) < 0) {
		dev_err(dev, "failed to parsing number\n");
		return -EINVAL;
	}
	ret = regmap_update_bits(mci->regmap,
				 info->reg, info->mask, tmp << info->shft);
	if (ret < 0)
		return ret;
	return count;
}

static int mt6360_sysfs_create_group(struct mt6360_chg_info *mci)
{
	int i, tb_size = ARRAY_SIZE(mt6360_field_tbl);

	for (i = 0; i < tb_size; i++)
		mt6360_attrs[i] = &(mt6360_field_tbl[i].dev_attr.attr);
	return devm_device_add_group(&mci->psy->dev, &mt6360_attr_group);
}

static int mt6360_chg_init_setting(struct mt6360_chg_info *mci)
{
	int ret;

	/* Disable bc12 */
	ret = regmap_update_bits(mci->regmap, MT6360_PMU_DEVICE_TYPE,
				 MT6360_MASK_USBCHGEN, 0);
	if (ret < 0) {
		dev_err(mci->dev, "%s: disable bc12 fail\n", __func__);
		goto out;
	}
	/* Set input current limit select by AICR */
	ret = regmap_update_bits(mci->regmap, MT6360_PMU_CHG_CTRL2,
				 MT6360_MASK_IINLMTSEL,
				 MT6360_IINLMTSEL_AICR <<
					MT6360_SHFT_IINLMTSEL);
	if (ret < 0) {
		dev_err(mci->dev,
			"%s: switch iinlmtsel to aicr fail\n", __func__);
		goto out;
	}
	usleep_range(5000, 6000);
	/* Disable ilim */
	ret = regmap_update_bits(mci->regmap, MT6360_PMU_CHG_CTRL3,
				 MT6360_MASK_ILIM_EN, 0);
	if (ret < 0) {
		dev_err(mci->dev,
			"%s: switch iinlmtsel to aicr fail\n", __func__);
		goto out;
	}
out:
	return ret;
}

u32 mt6360_vinovp_trans_to_sel(u32 val)
=======
static int mt6360_chg_irq_register(struct platform_device *pdev)
{
	const struct {
		const char *name;
		irq_handler_t handler;
	} irq_descs[] = {
		{ "attach_i", mt6360_pmu_attach_i_handler },
		{ "chrdet_ext_evt", mt6360_pmu_chrdet_ext_evt_handler }
	};
	int i, ret;

	for (i = 0; i < ARRAY_SIZE(irq_descs); i++) {
		ret = platform_get_irq_byname(pdev, irq_descs[i].name);
		if (ret < 0)
			return ret;

		ret = devm_request_threaded_irq(&pdev->dev, ret, NULL,
						irq_descs[i].handler,
						IRQF_TRIGGER_FALLING | IRQF_ONESHOT,
						irq_descs[i].name,
						platform_get_drvdata(pdev));
		if (ret < 0)
			return dev_err_probe(&pdev->dev, ret, "Failed to request %s irq\n",
					     irq_descs[i].name);
	}

	return 0;
}

static u32 mt6360_vinovp_trans_to_sel(u32 val)
>>>>>>> upstream/android-13
{
	u32 vinovp_tbl[] = { 5500000, 6500000, 11000000, 14500000 };
	int i;

	/* Select the smaller and equal supported value */
	for (i = 0; i < ARRAY_SIZE(vinovp_tbl)-1; i++) {
		if (val < vinovp_tbl[i+1])
			break;
	}
	return i;
}

<<<<<<< HEAD
static const struct mt6360_pdata_prop mt6360_pdata_props[] = {
	MT6360_PDATA_VALPROP(vinovp, struct mt6360_chg_platform_data,
			     MT6360_PMU_CHG_CTRL19, 5, 0x60,
			     mt6360_vinovp_trans_to_sel, 0),
};

static const struct mt6360_val_prop mt6360_val_props[] = {
	MT6360_DT_VALPROP(vinovp, struct mt6360_chg_platform_data),
};

static int mt6360_charger_probe(struct platform_device *pdev)
{
	struct mt6360_chg_platform_data *pdata = dev_get_platdata(&pdev->dev);
	struct mt6360_chg_info *mci;
	struct power_supply_config charger_cfg = {};
	struct regulator_config config = { };
	struct device_node *np = pdev->dev.of_node;
=======
static int mt6360_chg_init_setting(struct mt6360_chg_info *mci)
{
	int ret;
	u32 sel;

	sel = mt6360_vinovp_trans_to_sel(mci->vinovp);
	ret = regmap_update_bits(mci->regmap, MT6360_PMU_CHG_CTRL19,
				  MT6360_VINOVP_MASK, sel << MT6360_VINOVP_SHFT);
	if (ret)
		return dev_err_probe(mci->dev, ret, "%s: Failed to apply vinovp\n", __func__);
	ret = regmap_update_bits(mci->regmap, MT6360_PMU_DEVICE_TYPE,
				 MT6360_USBCHGEN_MASK, 0);
	if (ret)
		return dev_err_probe(mci->dev, ret, "%s: Failed to disable bc12\n", __func__);
	ret = regmap_update_bits(mci->regmap, MT6360_PMU_CHG_CTRL2,
				 MT6360_IINLMTSEL_MASK,
				 MT6360_IINLMTSEL_AICR <<
					MT6360_IINLMTSEL_SHFT);
	if (ret)
		return dev_err_probe(mci->dev, ret,
				     "%s: Failed to switch iinlmtsel to aicr\n", __func__);
	usleep_range(5000, 6000);
	ret = regmap_update_bits(mci->regmap, MT6360_PMU_CHG_CTRL3,
				 MT6360_ILIM_EN_MASK, 0);
	if (ret)
		return dev_err_probe(mci->dev, ret,
				     "%s: Failed to disable ilim\n", __func__);
	ret = regmap_update_bits(mci->regmap, MT6360_PMU_CHG_CTRL10,
				 MT6360_OTG_OC_MASK, MT6360_OTG_OC_MASK);
	if (ret)
		return dev_err_probe(mci->dev, ret,
				     "%s: Failed to config otg oc to 3A\n", __func__);
	return 0;
}

static int mt6360_charger_probe(struct platform_device *pdev)
{
	struct mt6360_chg_info *mci;
	struct power_supply_config charger_cfg = {};
	struct regulator_config config = { };
>>>>>>> upstream/android-13
	int ret;

	mci = devm_kzalloc(&pdev->dev, sizeof(*mci), GFP_KERNEL);
	if (!mci)
		return -ENOMEM;
<<<<<<< HEAD
	if (np) {
		pdata = devm_kzalloc(&pdev->dev, sizeof(*pdata), GFP_KERNEL);
		if (!pdata)
			return -ENOMEM;
		memcpy(pdata, &def_platform_data, sizeof(*pdata));
		mt6360_dt_parser_helper(np, (void *)pdata, mt6360_val_props,
					ARRAY_SIZE(mt6360_val_props));
	}
	if (!pdata) {
		dev_err(&pdev->dev, "no platform data specified\n");
		return -EINVAL;
	}
	mci->dev = &pdev->dev;
	mutex_init(&mci->chgdet_lock);
	platform_set_drvdata(pdev, mci);
	INIT_WORK(&mci->chrdet_work, &mt6360_chrdet_work);

	/* get parent regmap */
	mci->regmap = dev_get_regmap(pdev->dev.parent, NULL);
	if (!mci->regmap) {
		dev_err(&pdev->dev, "Fail to get parent regmap\n");
		return -ENODEV;
	}
	/* apply platform data */
	ret = mt6360_pdata_apply_helper(mci->regmap, pdata, mt6360_pdata_props,
					ARRAY_SIZE(mt6360_pdata_props));
	if (ret < 0) {
		dev_err(&pdev->dev, "apply pdata fail\n");
		return ret;
	}
	/* extcon */
	mci->edev = devm_extcon_dev_allocate(&pdev->dev, mt6360_extcon_cable);
	if (IS_ERR(mci->edev)) {
		dev_err(&pdev->dev, "Fail to allocate memory for extcon\n");
		return -ENOMEM;
	}
	ret = devm_extcon_dev_register(&pdev->dev, mci->edev);
	if (ret < 0) {
		dev_err(&pdev->dev, "Fail to register extcon dev\n");
		return ret;
	}
	/* power supply */
=======

	mci->dev = &pdev->dev;
	mci->vinovp = 6500000;
	mutex_init(&mci->chgdet_lock);
	platform_set_drvdata(pdev, mci);
	devm_work_autocancel(&pdev->dev, &mci->chrdet_work, mt6360_chrdet_work);

	ret = device_property_read_u32(&pdev->dev, "richtek,vinovp-microvolt", &mci->vinovp);
	if (ret)
		dev_warn(&pdev->dev, "Failed to parse vinovp in DT, keep default 6.5v\n");

	mci->regmap = dev_get_regmap(pdev->dev.parent, NULL);
	if (!mci->regmap)
		return dev_err_probe(&pdev->dev, -ENODEV, "Failed to get parent regmap\n");

	ret = mt6360_chg_init_setting(mci);
	if (ret)
		return dev_err_probe(&pdev->dev, ret, "Failed to initial setting\n");

>>>>>>> upstream/android-13
	memcpy(&mci->psy_desc, &mt6360_charger_desc, sizeof(mci->psy_desc));
	mci->psy_desc.name = dev_name(&pdev->dev);
	charger_cfg.drv_data = mci;
	charger_cfg.of_node = pdev->dev.of_node;
<<<<<<< HEAD
	charger_cfg.supplied_to = mt6360_charger_supplied_to;
	charger_cfg.num_supplicants = ARRAY_SIZE(mt6360_charger_supplied_to);
	mci->psy = devm_power_supply_register(&pdev->dev,
					      &mci->psy_desc, &charger_cfg);
	if (IS_ERR(mci->psy)) {
		dev_err(&pdev->dev, "Fail to register power supply dev\n");
		return PTR_ERR(mci->psy);
	}
	/* Initial Setting */
	ret = mt6360_chg_init_setting(mci);
	if (ret < 0) {
		dev_err(&pdev->dev, "initial setting fail\n");
		return ret;
	}
	/* Always check pwr_rdy once avoid already vbus exist */
	schedule_work(&mci->chrdet_work);
	/* irqs */
	ret = mt6360_chg_irq_register(pdev);
	if (ret < 0) {
		dev_err(&pdev->dev, "Fail to register irqs\n");
		return ret;
	}
	/* otg regulator */
=======
	mci->psy = devm_power_supply_register(&pdev->dev,
					      &mci->psy_desc, &charger_cfg);
	if (IS_ERR(mci->psy))
		return dev_err_probe(&pdev->dev, PTR_ERR(mci->psy),
				     "Failed to register power supply dev\n");


	ret = mt6360_chg_irq_register(pdev);
	if (ret)
		return dev_err_probe(&pdev->dev, ret, "Failed to register irqs\n");

>>>>>>> upstream/android-13
	config.dev = &pdev->dev;
	config.regmap = mci->regmap;
	mci->otg_rdev = devm_regulator_register(&pdev->dev, &mt6360_otg_rdesc,
						&config);
	if (IS_ERR(mci->otg_rdev))
		return PTR_ERR(mci->otg_rdev);
<<<<<<< HEAD
	/* create sysfs attr */
	ret = mt6360_sysfs_create_group(mci);
	if (ret < 0) {
		dev_err(&pdev->dev,
			"%s: create sysfs attrs fail\n", __func__);
		return ret;
	}
	dev_info(&pdev->dev, "Successfully probed\n");
=======

	schedule_work(&mci->chrdet_work);

>>>>>>> upstream/android-13
	return 0;
}

static const struct of_device_id __maybe_unused mt6360_charger_of_id[] = {
<<<<<<< HEAD
	{ .compatible = "mediatek,mt6360_chg", },
=======
	{ .compatible = "mediatek,mt6360-chg", },
>>>>>>> upstream/android-13
	{},
};
MODULE_DEVICE_TABLE(of, mt6360_charger_of_id);

static const struct platform_device_id mt6360_charger_id[] = {
<<<<<<< HEAD
	{ "mt6360_chg", 0 },
=======
	{ "mt6360-chg", 0 },
>>>>>>> upstream/android-13
	{},
};
MODULE_DEVICE_TABLE(platform, mt6360_charger_id);

static struct platform_driver mt6360_charger_driver = {
	.driver = {
<<<<<<< HEAD
		.name = "mt6360_charger",
		.owner = THIS_MODULE,
=======
		.name = "mt6360-chg",
>>>>>>> upstream/android-13
		.of_match_table = of_match_ptr(mt6360_charger_of_id),
	},
	.probe = mt6360_charger_probe,
	.id_table = mt6360_charger_id,
};
module_platform_driver(mt6360_charger_driver);

<<<<<<< HEAD
MODULE_AUTHOR("CY_Huang <cy_huang@richtek.com>");
MODULE_DESCRIPTION("MT6360 Charger Driver");
MODULE_LICENSE("GPL");
MODULE_VERSION("1.0.0");
=======
MODULE_AUTHOR("Gene Chen <gene_chen@richtek.com>");
MODULE_DESCRIPTION("MT6360 Charger Driver");
MODULE_LICENSE("GPL");
>>>>>>> upstream/android-13
