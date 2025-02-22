<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> upstream/android-13
/*
 * MFD core driver for Ricoh RN5T618 PMIC
 *
 * Copyright (C) 2014 Beniamino Galvani <b.galvani@gmail.com>
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
=======
>>>>>>> upstream/android-13
 */

#ifndef __LINUX_MFD_RN5T618_H
#define __LINUX_MFD_RN5T618_H

#include <linux/regmap.h>

#define RN5T618_LSIVER			0x00
#define RN5T618_OTPVER			0x01
#define RN5T618_IODAC			0x02
#define RN5T618_VINDAC			0x03
#define RN5T618_OUT32KEN		0x05
#define RN5T618_CPUCNT			0x06
#define RN5T618_PSWR			0x07
#define RN5T618_PONHIS			0x09
#define RN5T618_POFFHIS			0x0a
#define RN5T618_WATCHDOG		0x0b
#define RN5T618_WATCHDOGCNT		0x0c
#define RN5T618_PWRFUNC			0x0d
#define RN5T618_SLPCNT			0x0e
#define RN5T618_REPCNT			0x0f
#define RN5T618_PWRONTIMSET		0x10
#define RN5T618_NOETIMSETCNT		0x11
#define RN5T618_PWRIREN			0x12
#define RN5T618_PWRIRQ			0x13
#define RN5T618_PWRMON			0x14
#define RN5T618_PWRIRSEL		0x15
#define RN5T618_DC1_SLOT		0x16
#define RN5T618_DC2_SLOT		0x17
#define RN5T618_DC3_SLOT		0x18
#define RN5T618_DC4_SLOT		0x19
#define RN5T618_LDO1_SLOT		0x1b
#define RN5T618_LDO2_SLOT		0x1c
#define RN5T618_LDO3_SLOT		0x1d
#define RN5T618_LDO4_SLOT		0x1e
#define RN5T618_LDO5_SLOT		0x1f
#define RN5T618_PSO0_SLOT		0x25
#define RN5T618_PSO1_SLOT		0x26
#define RN5T618_PSO2_SLOT		0x27
#define RN5T618_PSO3_SLOT		0x28
#define RN5T618_LDORTC1_SLOT		0x2a
#define RN5T618_DC1CTL			0x2c
#define RN5T618_DC1CTL2			0x2d
#define RN5T618_DC2CTL			0x2e
#define RN5T618_DC2CTL2			0x2f
#define RN5T618_DC3CTL			0x30
#define RN5T618_DC3CTL2			0x31
#define RN5T618_DC4CTL			0x32
#define RN5T618_DC4CTL2			0x33
#define RN5T618_DC5CTL			0x34
#define RN5T618_DC5CTL2			0x35
#define RN5T618_DC1DAC			0x36
#define RN5T618_DC2DAC			0x37
#define RN5T618_DC3DAC			0x38
#define RN5T618_DC4DAC			0x39
#define RN5T618_DC5DAC			0x3a
#define RN5T618_DC1DAC_SLP		0x3b
#define RN5T618_DC2DAC_SLP		0x3c
#define RN5T618_DC3DAC_SLP		0x3d
#define RN5T618_DC4DAC_SLP		0x3e
#define RN5T618_DCIREN			0x40
#define RN5T618_DCIRQ			0x41
#define RN5T618_DCIRMON			0x42
#define RN5T618_LDOEN1			0x44
#define RN5T618_LDOEN2			0x45
#define RN5T618_LDODIS			0x46
#define RN5T618_LDO1DAC			0x4c
#define RN5T618_LDO2DAC			0x4d
#define RN5T618_LDO3DAC			0x4e
#define RN5T618_LDO4DAC			0x4f
#define RN5T618_LDO5DAC			0x50
#define RN5T618_LDO6DAC			0x51
#define RN5T618_LDO7DAC			0x52
#define RN5T618_LDO8DAC			0x53
#define RN5T618_LDO9DAC			0x54
#define RN5T618_LDO10DAC		0x55
#define RN5T618_LDORTCDAC		0x56
#define RN5T618_LDORTC2DAC		0x57
#define RN5T618_LDO1DAC_SLP		0x58
#define RN5T618_LDO2DAC_SLP		0x59
#define RN5T618_LDO3DAC_SLP		0x5a
#define RN5T618_LDO4DAC_SLP		0x5b
#define RN5T618_LDO5DAC_SLP		0x5c
#define RN5T618_ADCCNT1			0x64
#define RN5T618_ADCCNT2			0x65
#define RN5T618_ADCCNT3			0x66
#define RN5T618_ILIMDATAH		0x68
#define RN5T618_ILIMDATAL		0x69
#define RN5T618_VBATDATAH		0x6a
#define RN5T618_VBATDATAL		0x6b
#define RN5T618_VADPDATAH		0x6c
#define RN5T618_VADPDATAL		0x6d
#define RN5T618_VUSBDATAH		0x6e
#define RN5T618_VUSBDATAL		0x6f
#define RN5T618_VSYSDATAH		0x70
#define RN5T618_VSYSDATAL		0x71
#define RN5T618_VTHMDATAH		0x72
#define RN5T618_VTHMDATAL		0x73
#define RN5T618_AIN1DATAH		0x74
#define RN5T618_AIN1DATAL		0x75
#define RN5T618_AIN0DATAH		0x76
#define RN5T618_AIN0DATAL		0x77
#define RN5T618_ILIMTHL			0x78
#define RN5T618_ILIMTHH			0x79
#define RN5T618_VBATTHL			0x7a
#define RN5T618_VBATTHH			0x7b
#define RN5T618_VADPTHL			0x7c
#define RN5T618_VADPTHH			0x7d
#define RN5T618_VUSBTHL			0x7e
#define RN5T618_VUSBTHH			0x7f
#define RN5T618_VSYSTHL			0x80
#define RN5T618_VSYSTHH			0x81
#define RN5T618_VTHMTHL			0x82
#define RN5T618_VTHMTHH			0x83
#define RN5T618_AIN1THL			0x84
#define RN5T618_AIN1THH			0x85
#define RN5T618_AIN0THL			0x86
#define RN5T618_AIN0THH			0x87
#define RN5T618_EN_ADCIR1		0x88
#define RN5T618_EN_ADCIR2		0x89
#define RN5T618_EN_ADCIR3		0x8a
#define RN5T618_IR_ADC1			0x8c
#define RN5T618_IR_ADC2			0x8d
#define RN5T618_IR_ADC3			0x8e
#define RN5T618_IOSEL			0x90
#define RN5T618_IOOUT			0x91
#define RN5T618_GPEDGE1			0x92
#define RN5T618_GPEDGE2			0x93
#define RN5T618_EN_GPIR			0x94
#define RN5T618_IR_GPR			0x95
#define RN5T618_IR_GPF			0x96
#define RN5T618_MON_IOIN		0x97
#define RN5T618_GPLED_FUNC		0x98
#define RN5T618_INTPOL			0x9c
#define RN5T618_INTEN			0x9d
#define RN5T618_INTMON			0x9e
<<<<<<< HEAD
=======

#define RN5T618_RTC_SECONDS     0xA0
#define RN5T618_RTC_MDAY        0xA4
#define RN5T618_RTC_MONTH       0xA5
#define RN5T618_RTC_YEAR        0xA6
#define RN5T618_RTC_ADJUST      0xA7
#define RN5T618_RTC_ALARM_Y_SEC 0xA8
#define RN5T618_RTC_DAL_MONTH   0xAC
#define RN5T618_RTC_CTRL1       0xAE
#define RN5T618_RTC_CTRL2       0xAF

>>>>>>> upstream/android-13
#define RN5T618_PREVINDAC		0xb0
#define RN5T618_BATDAC			0xb1
#define RN5T618_CHGCTL1			0xb3
#define RN5T618_CHGCTL2			0xb4
#define RN5T618_VSYSSET			0xb5
#define RN5T618_REGISET1		0xb6
#define RN5T618_REGISET2		0xb7
#define RN5T618_CHGISET			0xb8
#define RN5T618_TIMSET			0xb9
#define RN5T618_BATSET1			0xba
#define RN5T618_BATSET2			0xbb
#define RN5T618_DIESET			0xbc
#define RN5T618_CHGSTATE		0xbd
#define RN5T618_CHGCTRL_IRFMASK		0xbe
#define RN5T618_CHGSTAT_IRFMASK1	0xbf
#define RN5T618_CHGSTAT_IRFMASK2	0xc0
#define RN5T618_CHGERR_IRFMASK		0xc1
#define RN5T618_CHGCTRL_IRR		0xc2
#define RN5T618_CHGSTAT_IRR1		0xc3
#define RN5T618_CHGSTAT_IRR2		0xc4
#define RN5T618_CHGERR_IRR		0xc5
#define RN5T618_CHGCTRL_MONI		0xc6
#define RN5T618_CHGSTAT_MONI1		0xc7
#define RN5T618_CHGSTAT_MONI2		0xc8
#define RN5T618_CHGERR_MONI		0xc9
#define RN5T618_CHGCTRL_DETMOD1		0xca
#define RN5T618_CHGCTRL_DETMOD2		0xcb
#define RN5T618_CHGSTAT_DETMOD1		0xcc
#define RN5T618_CHGSTAT_DETMOD2		0xcd
#define RN5T618_CHGSTAT_DETMOD3		0xce
#define RN5T618_CHGERR_DETMOD1		0xcf
#define RN5T618_CHGERR_DETMOD2		0xd0
#define RN5T618_CHGOSCCTL		0xd4
#define RN5T618_CHGOSCSCORESET1		0xd5
#define RN5T618_CHGOSCSCORESET2		0xd6
#define RN5T618_CHGOSCSCORESET3		0xd7
#define RN5T618_CHGOSCFREQSET1		0xd8
#define RN5T618_CHGOSCFREQSET2		0xd9
<<<<<<< HEAD
=======
#define RN5T618_GCHGDET			0xda
>>>>>>> upstream/android-13
#define RN5T618_CONTROL			0xe0
#define RN5T618_SOC			0xe1
#define RN5T618_RE_CAP_H		0xe2
#define RN5T618_RE_CAP_L		0xe3
#define RN5T618_FA_CAP_H		0xe4
#define RN5T618_FA_CAP_L		0xe5
#define RN5T618_AGE			0xe6
#define RN5T618_TT_EMPTY_H		0xe7
#define RN5T618_TT_EMPTY_L		0xe8
#define RN5T618_TT_FULL_H		0xe9
#define RN5T618_TT_FULL_L		0xea
#define RN5T618_VOLTAGE_1		0xeb
#define RN5T618_VOLTAGE_0		0xec
#define RN5T618_TEMP_1			0xed
#define RN5T618_TEMP_0			0xee
#define RN5T618_CC_CTRL			0xef
#define RN5T618_CC_COUNT2		0xf0
#define RN5T618_CC_COUNT1		0xf1
#define RN5T618_CC_COUNT0		0xf2
#define RN5T618_CC_SUMREG3		0xf3
#define RN5T618_CC_SUMREG2		0xf4
#define RN5T618_CC_SUMREG1		0xf5
#define RN5T618_CC_SUMREG0		0xf6
#define RN5T618_CC_OFFREG1		0xf7
#define RN5T618_CC_OFFREG0		0xf8
#define RN5T618_CC_GAINREG1		0xf9
#define RN5T618_CC_GAINREG0		0xfa
#define RN5T618_CC_AVEREG1		0xfb
#define RN5T618_CC_AVEREG0		0xfc
#define RN5T618_MAX_REG			0xfc

#define RN5T618_REPCNT_REPWRON		BIT(0)
#define RN5T618_SLPCNT_SWPWROFF		BIT(0)
#define RN5T618_WATCHDOG_WDOGEN		BIT(2)
#define RN5T618_WATCHDOG_WDOGTIM_M	(BIT(0) | BIT(1))
#define RN5T618_WATCHDOG_WDOGTIM_S	0
#define RN5T618_PWRIRQ_IR_WDOG		BIT(6)

enum {
	RN5T618_DCDC1,
	RN5T618_DCDC2,
	RN5T618_DCDC3,
	RN5T618_DCDC4,
	RN5T618_DCDC5,
	RN5T618_LDO1,
	RN5T618_LDO2,
	RN5T618_LDO3,
	RN5T618_LDO4,
	RN5T618_LDO5,
	RN5T618_LDO6,
	RN5T618_LDO7,
	RN5T618_LDO8,
	RN5T618_LDO9,
	RN5T618_LDO10,
	RN5T618_LDORTC1,
	RN5T618_LDORTC2,
	RN5T618_REG_NUM,
};

enum {
	RN5T567 = 0,
	RN5T618,
	RC5T619,
};

<<<<<<< HEAD
struct rn5t618 {
	struct regmap *regmap;
	long variant;
=======
/* RN5T618 IRQ definitions */
enum {
	RN5T618_IRQ_SYS = 0,
	RN5T618_IRQ_DCDC,
	RN5T618_IRQ_RTC,
	RN5T618_IRQ_ADC,
	RN5T618_IRQ_GPIO,
	RN5T618_IRQ_CHG,
	RN5T618_NR_IRQS,
};

struct rn5t618 {
	struct regmap *regmap;
	struct device *dev;
	long variant;

	int irq;
	struct regmap_irq_chip_data *irq_data;
>>>>>>> upstream/android-13
};

#endif /* __LINUX_MFD_RN5T618_H */
