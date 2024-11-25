// SPDX-License-Identifier: GPL-2.0
/*
<<<<<<< HEAD
 * Copyright (c) 2019 MediaTek Inc.
 */

#include <linux/i2c.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/mfd/core.h>
#include <linux/module.h>
#include <linux/of_irq.h>
#include <linux/of_platform.h>
#include <linux/version.h>

#include <linux/mfd/mt6360.h>
#include <linux/mfd/mt6360-private.h>

/* reg 0 -> 0 ~ 7 */
#define MT6360_CHG_TREG_EVT		(4)
#define MT6360_CHG_AICR_EVT		(5)
#define MT6360_CHG_MIVR_EVT		(6)
#define MT6360_PWR_RDY_EVT		(7)
/* REG 1 -> 8 ~ 15 */
#define MT6360_CHG_BATSYSUV_EVT		(9)
#define MT6360_FLED_CHG_VINOVP_EVT	(11)
#define MT6360_CHG_VSYSUV_EVT		(12)
#define MT6360_CHG_VSYSOV_EVT		(13)
#define MT6360_CHG_VBATOV_EVT		(14)
#define MT6360_CHG_VBUSOV_EVT		(15)
/* REG 2 -> 16 ~ 23 */
/* REG 3 -> 24 ~ 31 */
#define MT6360_WD_PMU_DET		(25)
#define MT6360_WD_PMU_DONE		(26)
#define MT6360_CHG_TMRI			(27)
#define MT6360_CHG_ADPBADI		(29)
#define MT6360_CHG_RVPI			(30)
#define MT6360_OTPI			(31)
/* REG 4 -> 32 ~ 39 */
#define MT6360_CHG_AICCMEASL		(32)
#define MT6360_CHGDET_DONEI		(34)
#define MT6360_WDTMRI			(35)
#define MT6360_SSFINISHI		(36)
#define MT6360_CHG_RECHGI		(37)
#define MT6360_CHG_TERMI		(38)
#define MT6360_CHG_IEOCI		(39)
/* REG 5 -> 40 ~ 47 */
#define MT6360_PUMPX_DONEI		(40)
#define MT6360_BAT_OVP_ADC_EVT		(41)
#define MT6360_TYPEC_OTP_EVT		(42)
#define MT6360_ADC_WAKEUP_EVT		(43)
#define MT6360_ADC_DONEI		(44)
#define MT6360_BST_BATUVI		(45)
#define MT6360_BST_VBUSOVI		(46)
#define MT6360_BST_OLPI			(47)
/* REG 6 -> 48 ~ 55 */
#define MT6360_ATTACH_I			(48)
#define MT6360_DETACH_I			(49)
#define MT6360_QC30_STPDONE		(51)
#define MT6360_QC_VBUSDET_DONE		(52)
#define MT6360_HVDCP_DET		(53)
#define MT6360_CHGDETI			(54)
#define MT6360_DCDTI			(55)
/* REG 7 -> 56 ~ 63 */
#define MT6360_FOD_DONE_EVT		(56)
#define MT6360_FOD_OV_EVT		(57)
#define MT6360_CHRDET_UVP_EVT		(58)
#define MT6360_CHRDET_OVP_EVT		(59)
#define MT6360_CHRDET_EXT_EVT		(60)
#define MT6360_FOD_LR_EVT		(61)
#define MT6360_FOD_HR_EVT		(62)
#define MT6360_FOD_DISCHG_FAIL_EVT	(63)
/* REG 8 -> 64 ~ 71 */
#define MT6360_USBID_EVT		(64)
#define MT6360_APWDTRST_EVT		(65)
#define MT6360_EN_EVT			(66)
#define MT6360_QONB_RST_EVT		(67)
#define MT6360_MRSTB_EVT		(68)
#define MT6360_OTP_EVT			(69)
#define MT6360_VDDAOV_EVT		(70)
#define MT6360_SYSUV_EVT		(71)
/* REG 9 -> 72 ~ 79 */
#define MT6360_FLED_STRBPIN_EVT		(72)
#define MT6360_FLED_TORPIN_EVT		(73)
#define MT6360_FLED_TX_EVT		(74)
#define MT6360_FLED_LVF_EVT		(75)
#define MT6360_FLED2_SHORT_EVT		(78)
#define MT6360_FLED1_SHORT_EVT		(79)
/* REG 10 -> 80 ~ 87 */
#define MT6360_FLED2_STRB_EVT		(80)
#define MT6360_FLED1_STRB_EVT		(81)
#define MT6360_FLED2_STRB_TO_EVT	(82)
#define MT6360_FLED1_STRB_TO_EVT	(83)
#define MT6360_FLED2_TOR_EVT		(84)
#define MT6360_FLED1_TOR_EVT		(85)
/* REG 11 -> 88 ~ 95 */
/* REG 12 -> 96 ~ 103 */
#define MT6360_BUCK1_PGB_EVT		(96)
#define MT6360_BUCK1_OC_EVT		(100)
#define MT6360_BUCK1_OV_EVT		(101)
#define MT6360_BUCK1_UV_EVT		(102)
/* REG 13 -> 104 ~ 111 */
#define MT6360_BUCK2_PGB_EVT		(104)
#define MT6360_BUCK2_OC_EVT		(108)
#define MT6360_BUCK2_OV_EVT		(109)
#define MT6360_BUCK2_UV_EVT		(110)
/* REG 14 -> 112 ~ 119 */
#define MT6360_LDO1_OC_EVT		(113)
#define MT6360_LDO2_OC_EVT		(114)
#define MT6360_LDO3_OC_EVT		(115)
#define MT6360_LDO5_OC_EVT		(117)
#define MT6360_LDO6_OC_EVT		(118)
#define MT6360_LDO7_OC_EVT		(119)
/* REG 15 -> 120 ~ 127 */
#define MT6360_LDO1_PGB_EVT		(121)
#define MT6360_LDO2_PGB_EVT		(122)
#define MT6360_LDO3_PGB_EVT		(123)
#define MT6360_LDO5_PGB_EVT		(125)
#define MT6360_LDO6_PGB_EVT		(126)
#define MT6360_LDO7_PGB_EVT		(127)

#define MT6360_REGMAP_IRQ_REG(_irq_evt)		\
	REGMAP_IRQ_REG(_irq_evt, (_irq_evt) / 8, BIT((_irq_evt) % 8))

#define MT6360_MFD_CELL(_name)					\
	{							\
		.name = #_name,					\
		.of_compatible = "mediatek," #_name,		\
		.num_resources = ARRAY_SIZE(_name##_resources),	\
		.resources = _name##_resources,			\
	}

static const struct regmap_irq mt6360_pmu_irqs[] =  {
	MT6360_REGMAP_IRQ_REG(MT6360_CHG_TREG_EVT),
	MT6360_REGMAP_IRQ_REG(MT6360_CHG_AICR_EVT),
	MT6360_REGMAP_IRQ_REG(MT6360_CHG_MIVR_EVT),
	MT6360_REGMAP_IRQ_REG(MT6360_PWR_RDY_EVT),
	MT6360_REGMAP_IRQ_REG(MT6360_CHG_BATSYSUV_EVT),
	MT6360_REGMAP_IRQ_REG(MT6360_FLED_CHG_VINOVP_EVT),
	MT6360_REGMAP_IRQ_REG(MT6360_CHG_VSYSUV_EVT),
	MT6360_REGMAP_IRQ_REG(MT6360_CHG_VSYSOV_EVT),
	MT6360_REGMAP_IRQ_REG(MT6360_CHG_VBATOV_EVT),
	MT6360_REGMAP_IRQ_REG(MT6360_CHG_VBUSOV_EVT),
	MT6360_REGMAP_IRQ_REG(MT6360_WD_PMU_DET),
	MT6360_REGMAP_IRQ_REG(MT6360_WD_PMU_DONE),
	MT6360_REGMAP_IRQ_REG(MT6360_CHG_TMRI),
	MT6360_REGMAP_IRQ_REG(MT6360_CHG_ADPBADI),
	MT6360_REGMAP_IRQ_REG(MT6360_CHG_RVPI),
	MT6360_REGMAP_IRQ_REG(MT6360_OTPI),
	MT6360_REGMAP_IRQ_REG(MT6360_CHG_AICCMEASL),
	MT6360_REGMAP_IRQ_REG(MT6360_CHGDET_DONEI),
	MT6360_REGMAP_IRQ_REG(MT6360_WDTMRI),
	MT6360_REGMAP_IRQ_REG(MT6360_SSFINISHI),
	MT6360_REGMAP_IRQ_REG(MT6360_CHG_RECHGI),
	MT6360_REGMAP_IRQ_REG(MT6360_CHG_TERMI),
	MT6360_REGMAP_IRQ_REG(MT6360_CHG_IEOCI),
	MT6360_REGMAP_IRQ_REG(MT6360_PUMPX_DONEI),
	MT6360_REGMAP_IRQ_REG(MT6360_CHG_TREG_EVT),
	MT6360_REGMAP_IRQ_REG(MT6360_BAT_OVP_ADC_EVT),
	MT6360_REGMAP_IRQ_REG(MT6360_TYPEC_OTP_EVT),
	MT6360_REGMAP_IRQ_REG(MT6360_ADC_WAKEUP_EVT),
	MT6360_REGMAP_IRQ_REG(MT6360_ADC_DONEI),
	MT6360_REGMAP_IRQ_REG(MT6360_BST_BATUVI),
	MT6360_REGMAP_IRQ_REG(MT6360_BST_VBUSOVI),
	MT6360_REGMAP_IRQ_REG(MT6360_BST_OLPI),
	MT6360_REGMAP_IRQ_REG(MT6360_ATTACH_I),
	MT6360_REGMAP_IRQ_REG(MT6360_DETACH_I),
	MT6360_REGMAP_IRQ_REG(MT6360_QC30_STPDONE),
	MT6360_REGMAP_IRQ_REG(MT6360_QC_VBUSDET_DONE),
	MT6360_REGMAP_IRQ_REG(MT6360_HVDCP_DET),
	MT6360_REGMAP_IRQ_REG(MT6360_CHGDETI),
	MT6360_REGMAP_IRQ_REG(MT6360_DCDTI),
	MT6360_REGMAP_IRQ_REG(MT6360_FOD_DONE_EVT),
	MT6360_REGMAP_IRQ_REG(MT6360_FOD_OV_EVT),
	MT6360_REGMAP_IRQ_REG(MT6360_CHRDET_UVP_EVT),
	MT6360_REGMAP_IRQ_REG(MT6360_CHRDET_OVP_EVT),
	MT6360_REGMAP_IRQ_REG(MT6360_CHRDET_EXT_EVT),
	MT6360_REGMAP_IRQ_REG(MT6360_FOD_LR_EVT),
	MT6360_REGMAP_IRQ_REG(MT6360_FOD_HR_EVT),
	MT6360_REGMAP_IRQ_REG(MT6360_FOD_DISCHG_FAIL_EVT),
	MT6360_REGMAP_IRQ_REG(MT6360_USBID_EVT),
	MT6360_REGMAP_IRQ_REG(MT6360_APWDTRST_EVT),
	MT6360_REGMAP_IRQ_REG(MT6360_EN_EVT),
	MT6360_REGMAP_IRQ_REG(MT6360_QONB_RST_EVT),
	MT6360_REGMAP_IRQ_REG(MT6360_MRSTB_EVT),
	MT6360_REGMAP_IRQ_REG(MT6360_OTP_EVT),
	MT6360_REGMAP_IRQ_REG(MT6360_VDDAOV_EVT),
	MT6360_REGMAP_IRQ_REG(MT6360_SYSUV_EVT),
	MT6360_REGMAP_IRQ_REG(MT6360_FLED_STRBPIN_EVT),
	MT6360_REGMAP_IRQ_REG(MT6360_FLED_TORPIN_EVT),
	MT6360_REGMAP_IRQ_REG(MT6360_FLED_TX_EVT),
	MT6360_REGMAP_IRQ_REG(MT6360_FLED_LVF_EVT),
	MT6360_REGMAP_IRQ_REG(MT6360_FLED2_SHORT_EVT),
	MT6360_REGMAP_IRQ_REG(MT6360_FLED1_SHORT_EVT),
	MT6360_REGMAP_IRQ_REG(MT6360_FLED2_STRB_EVT),
	MT6360_REGMAP_IRQ_REG(MT6360_FLED1_STRB_EVT),
	MT6360_REGMAP_IRQ_REG(MT6360_FLED2_STRB_TO_EVT),
	MT6360_REGMAP_IRQ_REG(MT6360_FLED1_STRB_TO_EVT),
	MT6360_REGMAP_IRQ_REG(MT6360_FLED2_TOR_EVT),
	MT6360_REGMAP_IRQ_REG(MT6360_FLED1_TOR_EVT),
	MT6360_REGMAP_IRQ_REG(MT6360_BUCK1_PGB_EVT),
	MT6360_REGMAP_IRQ_REG(MT6360_BUCK1_OC_EVT),
	MT6360_REGMAP_IRQ_REG(MT6360_BUCK1_OV_EVT),
	MT6360_REGMAP_IRQ_REG(MT6360_BUCK1_UV_EVT),
	MT6360_REGMAP_IRQ_REG(MT6360_BUCK2_PGB_EVT),
	MT6360_REGMAP_IRQ_REG(MT6360_BUCK2_OC_EVT),
	MT6360_REGMAP_IRQ_REG(MT6360_BUCK2_OV_EVT),
	MT6360_REGMAP_IRQ_REG(MT6360_BUCK2_UV_EVT),
	MT6360_REGMAP_IRQ_REG(MT6360_LDO1_OC_EVT),
	MT6360_REGMAP_IRQ_REG(MT6360_LDO2_OC_EVT),
	MT6360_REGMAP_IRQ_REG(MT6360_LDO3_OC_EVT),
	MT6360_REGMAP_IRQ_REG(MT6360_LDO5_OC_EVT),
	MT6360_REGMAP_IRQ_REG(MT6360_LDO6_OC_EVT),
	MT6360_REGMAP_IRQ_REG(MT6360_LDO7_OC_EVT),
	MT6360_REGMAP_IRQ_REG(MT6360_LDO1_PGB_EVT),
	MT6360_REGMAP_IRQ_REG(MT6360_LDO2_PGB_EVT),
	MT6360_REGMAP_IRQ_REG(MT6360_LDO3_PGB_EVT),
	MT6360_REGMAP_IRQ_REG(MT6360_LDO5_PGB_EVT),
	MT6360_REGMAP_IRQ_REG(MT6360_LDO6_PGB_EVT),
	MT6360_REGMAP_IRQ_REG(MT6360_LDO7_PGB_EVT),
};

static int mt6360_pmu_handle_post_irq(void *irq_drv_data)
{
	struct mt6360_pmu_info *mpi = irq_drv_data;

	return regmap_update_bits(mpi->regmap,
		MT6360_PMU_IRQ_SET, MT6360_IRQ_RETRIG, MT6360_IRQ_RETRIG);
}

static const struct regmap_irq_chip mt6360_pmu_irq_chip = {
	.irqs = mt6360_pmu_irqs,
	.num_irqs = ARRAY_SIZE(mt6360_pmu_irqs),
=======
 * Copyright (c) 2020 MediaTek Inc.
 *
 * Author: Gene Chen <gene_chen@richtek.com>
 */

#include <linux/crc8.h>
#include <linux/i2c.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/kernel.h>
#include <linux/mfd/core.h>
#include <linux/module.h>
#include <linux/regmap.h>
#include <linux/slab.h>

enum {
	MT6360_SLAVE_TCPC = 0,
	MT6360_SLAVE_PMIC,
	MT6360_SLAVE_LDO,
	MT6360_SLAVE_PMU,
	MT6360_SLAVE_MAX,
};

struct mt6360_ddata {
	struct i2c_client *i2c[MT6360_SLAVE_MAX];
	struct device *dev;
	struct regmap *regmap;
	struct regmap_irq_chip_data *irq_data;
	unsigned int chip_rev;
	u8 crc8_tbl[CRC8_TABLE_SIZE];
};

#define MT6360_TCPC_SLAVEID		0x4E
#define MT6360_PMIC_SLAVEID		0x1A
#define MT6360_LDO_SLAVEID		0x64
#define MT6360_PMU_SLAVEID		0x34

#define MT6360_REG_TCPCSTART		0x00
#define MT6360_REG_TCPCEND		0xFF
#define MT6360_REG_PMICSTART		0x100
#define MT6360_REG_PMICEND		0x13B
#define MT6360_REG_LDOSTART		0x200
#define MT6360_REG_LDOEND		0x21C
#define MT6360_REG_PMUSTART		0x300
#define MT6360_PMU_DEV_INFO		0x300
#define MT6360_PMU_CHG_IRQ1		0x3D0
#define MT6360_PMU_CHG_MASK1		0x3F0
#define MT6360_REG_PMUEND		0x3FF

#define MT6360_PMU_IRQ_REGNUM		16

#define CHIP_VEN_MASK			0xF0
#define CHIP_VEN_MT6360			0x50
#define CHIP_REV_MASK			0x0F

#define MT6360_ADDRESS_MASK		0x3F
#define MT6360_DATA_SIZE_1_BYTE		0x00
#define MT6360_DATA_SIZE_2_BYTES	0x40
#define MT6360_DATA_SIZE_3_BYTES	0x80
#define MT6360_DATA_SIZE_4_BYTES	0xC0

#define MT6360_CRC8_POLYNOMIAL		0x7

#define MT6360_CRC_I2C_ADDR_SIZE	1
#define MT6360_CRC_REG_ADDR_SIZE	1
/* prealloca read size = i2c device addr + i2c reg addr + val ... + crc8 */
#define MT6360_ALLOC_READ_SIZE(_size)	(_size + 3)
/* prealloca write size = i2c device addr + i2c reg addr + val ... + crc8 + dummy byte */
#define MT6360_ALLOC_WRITE_SIZE(_size)	(_size + 4)
#define MT6360_CRC_PREDATA_OFFSET	(MT6360_CRC_I2C_ADDR_SIZE + MT6360_CRC_REG_ADDR_SIZE)
#define MT6360_CRC_CRC8_SIZE		1
#define MT6360_CRC_DUMMY_BYTE_SIZE	1
#define MT6360_REGMAP_REG_BYTE_SIZE	2
#define I2C_ADDR_XLATE_8BIT(_addr, _rw)	(((_addr & 0x7F) << 1) + _rw)

/* reg 0 -> 0 ~ 7 */
#define MT6360_CHG_TREG_EVT		4
#define MT6360_CHG_AICR_EVT		5
#define MT6360_CHG_MIVR_EVT		6
#define MT6360_PWR_RDY_EVT		7
/* REG 1 -> 8 ~ 15 */
#define MT6360_CHG_BATSYSUV_EVT		9
#define MT6360_FLED_CHG_VINOVP_EVT	11
#define MT6360_CHG_VSYSUV_EVT		12
#define MT6360_CHG_VSYSOV_EVT		13
#define MT6360_CHG_VBATOV_EVT		14
#define MT6360_CHG_VBUSOV_EVT		15
/* REG 2 -> 16 ~ 23 */
/* REG 3 -> 24 ~ 31 */
#define MT6360_WD_PMU_DET		25
#define MT6360_WD_PMU_DONE		26
#define MT6360_CHG_TMRI			27
#define MT6360_CHG_ADPBADI		29
#define MT6360_CHG_RVPI			30
#define MT6360_OTPI			31
/* REG 4 -> 32 ~ 39 */
#define MT6360_CHG_AICCMEASL		32
#define MT6360_CHGDET_DONEI		34
#define MT6360_WDTMRI			35
#define MT6360_SSFINISHI		36
#define MT6360_CHG_RECHGI		37
#define MT6360_CHG_TERMI		38
#define MT6360_CHG_IEOCI		39
/* REG 5 -> 40 ~ 47 */
#define MT6360_PUMPX_DONEI		40
#define MT6360_BAT_OVP_ADC_EVT		41
#define MT6360_TYPEC_OTP_EVT		42
#define MT6360_ADC_WAKEUP_EVT		43
#define MT6360_ADC_DONEI		44
#define MT6360_BST_BATUVI		45
#define MT6360_BST_VBUSOVI		46
#define MT6360_BST_OLPI			47
/* REG 6 -> 48 ~ 55 */
#define MT6360_ATTACH_I			48
#define MT6360_DETACH_I			49
#define MT6360_QC30_STPDONE		51
#define MT6360_QC_VBUSDET_DONE		52
#define MT6360_HVDCP_DET		53
#define MT6360_CHGDETI			54
#define MT6360_DCDTI			55
/* REG 7 -> 56 ~ 63 */
#define MT6360_FOD_DONE_EVT		56
#define MT6360_FOD_OV_EVT		57
#define MT6360_CHRDET_UVP_EVT		58
#define MT6360_CHRDET_OVP_EVT		59
#define MT6360_CHRDET_EXT_EVT		60
#define MT6360_FOD_LR_EVT		61
#define MT6360_FOD_HR_EVT		62
#define MT6360_FOD_DISCHG_FAIL_EVT	63
/* REG 8 -> 64 ~ 71 */
#define MT6360_USBID_EVT		64
#define MT6360_APWDTRST_EVT		65
#define MT6360_EN_EVT			66
#define MT6360_QONB_RST_EVT		67
#define MT6360_MRSTB_EVT		68
#define MT6360_OTP_EVT			69
#define MT6360_VDDAOV_EVT		70
#define MT6360_SYSUV_EVT		71
/* REG 9 -> 72 ~ 79 */
#define MT6360_FLED_STRBPIN_EVT		72
#define MT6360_FLED_TORPIN_EVT		73
#define MT6360_FLED_TX_EVT		74
#define MT6360_FLED_LVF_EVT		75
#define MT6360_FLED2_SHORT_EVT		78
#define MT6360_FLED1_SHORT_EVT		79
/* REG 10 -> 80 ~ 87 */
#define MT6360_FLED2_STRB_EVT		80
#define MT6360_FLED1_STRB_EVT		81
#define MT6360_FLED2_STRB_TO_EVT	82
#define MT6360_FLED1_STRB_TO_EVT	83
#define MT6360_FLED2_TOR_EVT		84
#define MT6360_FLED1_TOR_EVT		85
/* REG 11 -> 88 ~ 95 */
/* REG 12 -> 96 ~ 103 */
#define MT6360_BUCK1_PGB_EVT		96
#define MT6360_BUCK1_OC_EVT		100
#define MT6360_BUCK1_OV_EVT		101
#define MT6360_BUCK1_UV_EVT		102
/* REG 13 -> 104 ~ 111 */
#define MT6360_BUCK2_PGB_EVT		104
#define MT6360_BUCK2_OC_EVT		108
#define MT6360_BUCK2_OV_EVT		109
#define MT6360_BUCK2_UV_EVT		110
/* REG 14 -> 112 ~ 119 */
#define MT6360_LDO1_OC_EVT		113
#define MT6360_LDO2_OC_EVT		114
#define MT6360_LDO3_OC_EVT		115
#define MT6360_LDO5_OC_EVT		117
#define MT6360_LDO6_OC_EVT		118
#define MT6360_LDO7_OC_EVT		119
/* REG 15 -> 120 ~ 127 */
#define MT6360_LDO1_PGB_EVT		121
#define MT6360_LDO2_PGB_EVT		122
#define MT6360_LDO3_PGB_EVT		123
#define MT6360_LDO5_PGB_EVT		125
#define MT6360_LDO6_PGB_EVT		126
#define MT6360_LDO7_PGB_EVT		127

static const struct regmap_irq mt6360_irqs[] =  {
	REGMAP_IRQ_REG_LINE(MT6360_CHG_TREG_EVT, 8),
	REGMAP_IRQ_REG_LINE(MT6360_CHG_AICR_EVT, 8),
	REGMAP_IRQ_REG_LINE(MT6360_CHG_MIVR_EVT, 8),
	REGMAP_IRQ_REG_LINE(MT6360_PWR_RDY_EVT, 8),
	REGMAP_IRQ_REG_LINE(MT6360_CHG_BATSYSUV_EVT, 8),
	REGMAP_IRQ_REG_LINE(MT6360_FLED_CHG_VINOVP_EVT, 8),
	REGMAP_IRQ_REG_LINE(MT6360_CHG_VSYSUV_EVT, 8),
	REGMAP_IRQ_REG_LINE(MT6360_CHG_VSYSOV_EVT, 8),
	REGMAP_IRQ_REG_LINE(MT6360_CHG_VBATOV_EVT, 8),
	REGMAP_IRQ_REG_LINE(MT6360_CHG_VBUSOV_EVT, 8),
	REGMAP_IRQ_REG_LINE(MT6360_WD_PMU_DET, 8),
	REGMAP_IRQ_REG_LINE(MT6360_WD_PMU_DONE, 8),
	REGMAP_IRQ_REG_LINE(MT6360_CHG_TMRI, 8),
	REGMAP_IRQ_REG_LINE(MT6360_CHG_ADPBADI, 8),
	REGMAP_IRQ_REG_LINE(MT6360_CHG_RVPI, 8),
	REGMAP_IRQ_REG_LINE(MT6360_OTPI, 8),
	REGMAP_IRQ_REG_LINE(MT6360_CHG_AICCMEASL, 8),
	REGMAP_IRQ_REG_LINE(MT6360_CHGDET_DONEI, 8),
	REGMAP_IRQ_REG_LINE(MT6360_WDTMRI, 8),
	REGMAP_IRQ_REG_LINE(MT6360_SSFINISHI, 8),
	REGMAP_IRQ_REG_LINE(MT6360_CHG_RECHGI, 8),
	REGMAP_IRQ_REG_LINE(MT6360_CHG_TERMI, 8),
	REGMAP_IRQ_REG_LINE(MT6360_CHG_IEOCI, 8),
	REGMAP_IRQ_REG_LINE(MT6360_PUMPX_DONEI, 8),
	REGMAP_IRQ_REG_LINE(MT6360_BAT_OVP_ADC_EVT, 8),
	REGMAP_IRQ_REG_LINE(MT6360_TYPEC_OTP_EVT, 8),
	REGMAP_IRQ_REG_LINE(MT6360_ADC_WAKEUP_EVT, 8),
	REGMAP_IRQ_REG_LINE(MT6360_ADC_DONEI, 8),
	REGMAP_IRQ_REG_LINE(MT6360_BST_BATUVI, 8),
	REGMAP_IRQ_REG_LINE(MT6360_BST_VBUSOVI, 8),
	REGMAP_IRQ_REG_LINE(MT6360_BST_OLPI, 8),
	REGMAP_IRQ_REG_LINE(MT6360_ATTACH_I, 8),
	REGMAP_IRQ_REG_LINE(MT6360_DETACH_I, 8),
	REGMAP_IRQ_REG_LINE(MT6360_QC30_STPDONE, 8),
	REGMAP_IRQ_REG_LINE(MT6360_QC_VBUSDET_DONE, 8),
	REGMAP_IRQ_REG_LINE(MT6360_HVDCP_DET, 8),
	REGMAP_IRQ_REG_LINE(MT6360_CHGDETI, 8),
	REGMAP_IRQ_REG_LINE(MT6360_DCDTI, 8),
	REGMAP_IRQ_REG_LINE(MT6360_FOD_DONE_EVT, 8),
	REGMAP_IRQ_REG_LINE(MT6360_FOD_OV_EVT, 8),
	REGMAP_IRQ_REG_LINE(MT6360_CHRDET_UVP_EVT, 8),
	REGMAP_IRQ_REG_LINE(MT6360_CHRDET_OVP_EVT, 8),
	REGMAP_IRQ_REG_LINE(MT6360_CHRDET_EXT_EVT, 8),
	REGMAP_IRQ_REG_LINE(MT6360_FOD_LR_EVT, 8),
	REGMAP_IRQ_REG_LINE(MT6360_FOD_HR_EVT, 8),
	REGMAP_IRQ_REG_LINE(MT6360_FOD_DISCHG_FAIL_EVT, 8),
	REGMAP_IRQ_REG_LINE(MT6360_USBID_EVT, 8),
	REGMAP_IRQ_REG_LINE(MT6360_APWDTRST_EVT, 8),
	REGMAP_IRQ_REG_LINE(MT6360_EN_EVT, 8),
	REGMAP_IRQ_REG_LINE(MT6360_QONB_RST_EVT, 8),
	REGMAP_IRQ_REG_LINE(MT6360_MRSTB_EVT, 8),
	REGMAP_IRQ_REG_LINE(MT6360_OTP_EVT, 8),
	REGMAP_IRQ_REG_LINE(MT6360_VDDAOV_EVT, 8),
	REGMAP_IRQ_REG_LINE(MT6360_SYSUV_EVT, 8),
	REGMAP_IRQ_REG_LINE(MT6360_FLED_STRBPIN_EVT, 8),
	REGMAP_IRQ_REG_LINE(MT6360_FLED_TORPIN_EVT, 8),
	REGMAP_IRQ_REG_LINE(MT6360_FLED_TX_EVT, 8),
	REGMAP_IRQ_REG_LINE(MT6360_FLED_LVF_EVT, 8),
	REGMAP_IRQ_REG_LINE(MT6360_FLED2_SHORT_EVT, 8),
	REGMAP_IRQ_REG_LINE(MT6360_FLED1_SHORT_EVT, 8),
	REGMAP_IRQ_REG_LINE(MT6360_FLED2_STRB_EVT, 8),
	REGMAP_IRQ_REG_LINE(MT6360_FLED1_STRB_EVT, 8),
	REGMAP_IRQ_REG_LINE(MT6360_FLED2_STRB_TO_EVT, 8),
	REGMAP_IRQ_REG_LINE(MT6360_FLED1_STRB_TO_EVT, 8),
	REGMAP_IRQ_REG_LINE(MT6360_FLED2_TOR_EVT, 8),
	REGMAP_IRQ_REG_LINE(MT6360_FLED1_TOR_EVT, 8),
	REGMAP_IRQ_REG_LINE(MT6360_BUCK1_PGB_EVT, 8),
	REGMAP_IRQ_REG_LINE(MT6360_BUCK1_OC_EVT, 8),
	REGMAP_IRQ_REG_LINE(MT6360_BUCK1_OV_EVT, 8),
	REGMAP_IRQ_REG_LINE(MT6360_BUCK1_UV_EVT, 8),
	REGMAP_IRQ_REG_LINE(MT6360_BUCK2_PGB_EVT, 8),
	REGMAP_IRQ_REG_LINE(MT6360_BUCK2_OC_EVT, 8),
	REGMAP_IRQ_REG_LINE(MT6360_BUCK2_OV_EVT, 8),
	REGMAP_IRQ_REG_LINE(MT6360_BUCK2_UV_EVT, 8),
	REGMAP_IRQ_REG_LINE(MT6360_LDO1_OC_EVT, 8),
	REGMAP_IRQ_REG_LINE(MT6360_LDO2_OC_EVT, 8),
	REGMAP_IRQ_REG_LINE(MT6360_LDO3_OC_EVT, 8),
	REGMAP_IRQ_REG_LINE(MT6360_LDO5_OC_EVT, 8),
	REGMAP_IRQ_REG_LINE(MT6360_LDO6_OC_EVT, 8),
	REGMAP_IRQ_REG_LINE(MT6360_LDO7_OC_EVT, 8),
	REGMAP_IRQ_REG_LINE(MT6360_LDO1_PGB_EVT, 8),
	REGMAP_IRQ_REG_LINE(MT6360_LDO2_PGB_EVT, 8),
	REGMAP_IRQ_REG_LINE(MT6360_LDO3_PGB_EVT, 8),
	REGMAP_IRQ_REG_LINE(MT6360_LDO5_PGB_EVT, 8),
	REGMAP_IRQ_REG_LINE(MT6360_LDO6_PGB_EVT, 8),
	REGMAP_IRQ_REG_LINE(MT6360_LDO7_PGB_EVT, 8),
};

static const struct regmap_irq_chip mt6360_irq_chip = {
	.name = "mt6360_irqs",
	.irqs = mt6360_irqs,
	.num_irqs = ARRAY_SIZE(mt6360_irqs),
>>>>>>> upstream/android-13
	.num_regs = MT6360_PMU_IRQ_REGNUM,
	.mask_base = MT6360_PMU_CHG_MASK1,
	.status_base = MT6360_PMU_CHG_IRQ1,
	.ack_base = MT6360_PMU_CHG_IRQ1,
	.init_ack_masked = true,
	.use_ack = true,
<<<<<<< HEAD
	.handle_post_irq = mt6360_pmu_handle_post_irq,
};

static const struct regmap_config mt6360_pmu_regmap_config = {
	.reg_bits = 8,
	.val_bits = 8,
	.max_register = MT6360_PMU_MAXREG,
=======
>>>>>>> upstream/android-13
};

static const struct resource mt6360_adc_resources[] = {
	DEFINE_RES_IRQ_NAMED(MT6360_ADC_DONEI, "adc_donei"),
};

static const struct resource mt6360_chg_resources[] = {
	DEFINE_RES_IRQ_NAMED(MT6360_CHG_TREG_EVT, "chg_treg_evt"),
	DEFINE_RES_IRQ_NAMED(MT6360_PWR_RDY_EVT, "pwr_rdy_evt"),
	DEFINE_RES_IRQ_NAMED(MT6360_CHG_BATSYSUV_EVT, "chg_batsysuv_evt"),
	DEFINE_RES_IRQ_NAMED(MT6360_CHG_VSYSUV_EVT, "chg_vsysuv_evt"),
	DEFINE_RES_IRQ_NAMED(MT6360_CHG_VSYSOV_EVT, "chg_vsysov_evt"),
	DEFINE_RES_IRQ_NAMED(MT6360_CHG_VBATOV_EVT, "chg_vbatov_evt"),
	DEFINE_RES_IRQ_NAMED(MT6360_CHG_VBUSOV_EVT, "chg_vbusov_evt"),
	DEFINE_RES_IRQ_NAMED(MT6360_CHG_AICCMEASL, "chg_aiccmeasl"),
	DEFINE_RES_IRQ_NAMED(MT6360_WDTMRI, "wdtmri"),
	DEFINE_RES_IRQ_NAMED(MT6360_CHG_RECHGI, "chg_rechgi"),
	DEFINE_RES_IRQ_NAMED(MT6360_CHG_TERMI, "chg_termi"),
	DEFINE_RES_IRQ_NAMED(MT6360_CHG_IEOCI, "chg_ieoci"),
	DEFINE_RES_IRQ_NAMED(MT6360_PUMPX_DONEI, "pumpx_donei"),
	DEFINE_RES_IRQ_NAMED(MT6360_ATTACH_I, "attach_i"),
	DEFINE_RES_IRQ_NAMED(MT6360_CHRDET_EXT_EVT, "chrdet_ext_evt"),
};

static const struct resource mt6360_led_resources[] = {
	DEFINE_RES_IRQ_NAMED(MT6360_FLED_CHG_VINOVP_EVT, "fled_chg_vinovp_evt"),
	DEFINE_RES_IRQ_NAMED(MT6360_FLED_LVF_EVT, "fled_lvf_evt"),
	DEFINE_RES_IRQ_NAMED(MT6360_FLED2_SHORT_EVT, "fled2_short_evt"),
	DEFINE_RES_IRQ_NAMED(MT6360_FLED1_SHORT_EVT, "fled1_short_evt"),
	DEFINE_RES_IRQ_NAMED(MT6360_FLED2_STRB_TO_EVT, "fled2_strb_to_evt"),
	DEFINE_RES_IRQ_NAMED(MT6360_FLED1_STRB_TO_EVT, "fled1_strb_to_evt"),
};

<<<<<<< HEAD
static const struct resource mt6360_pmic_resources[] = {
=======
static const struct resource mt6360_regulator_resources[] = {
>>>>>>> upstream/android-13
	DEFINE_RES_IRQ_NAMED(MT6360_BUCK1_PGB_EVT, "buck1_pgb_evt"),
	DEFINE_RES_IRQ_NAMED(MT6360_BUCK1_OC_EVT, "buck1_oc_evt"),
	DEFINE_RES_IRQ_NAMED(MT6360_BUCK1_OV_EVT, "buck1_ov_evt"),
	DEFINE_RES_IRQ_NAMED(MT6360_BUCK1_UV_EVT, "buck1_uv_evt"),
	DEFINE_RES_IRQ_NAMED(MT6360_BUCK2_PGB_EVT, "buck2_pgb_evt"),
	DEFINE_RES_IRQ_NAMED(MT6360_BUCK2_OC_EVT, "buck2_oc_evt"),
	DEFINE_RES_IRQ_NAMED(MT6360_BUCK2_OV_EVT, "buck2_ov_evt"),
	DEFINE_RES_IRQ_NAMED(MT6360_BUCK2_UV_EVT, "buck2_uv_evt"),
<<<<<<< HEAD
	DEFINE_RES_IRQ_NAMED(MT6360_LDO6_OC_EVT, "ldo6_oc_evt"),
	DEFINE_RES_IRQ_NAMED(MT6360_LDO7_OC_EVT, "ldo7_oc_evt"),
	DEFINE_RES_IRQ_NAMED(MT6360_LDO6_PGB_EVT, "ldo6_pgb_evt"),
	DEFINE_RES_IRQ_NAMED(MT6360_LDO7_PGB_EVT, "ldo7_pgb_evt"),
};

static const struct resource mt6360_ldo_resources[] = {
=======
>>>>>>> upstream/android-13
	DEFINE_RES_IRQ_NAMED(MT6360_LDO1_OC_EVT, "ldo1_oc_evt"),
	DEFINE_RES_IRQ_NAMED(MT6360_LDO2_OC_EVT, "ldo2_oc_evt"),
	DEFINE_RES_IRQ_NAMED(MT6360_LDO3_OC_EVT, "ldo3_oc_evt"),
	DEFINE_RES_IRQ_NAMED(MT6360_LDO5_OC_EVT, "ldo5_oc_evt"),
<<<<<<< HEAD
=======
	DEFINE_RES_IRQ_NAMED(MT6360_LDO6_OC_EVT, "ldo6_oc_evt"),
	DEFINE_RES_IRQ_NAMED(MT6360_LDO7_OC_EVT, "ldo7_oc_evt"),
>>>>>>> upstream/android-13
	DEFINE_RES_IRQ_NAMED(MT6360_LDO1_PGB_EVT, "ldo1_pgb_evt"),
	DEFINE_RES_IRQ_NAMED(MT6360_LDO2_PGB_EVT, "ldo2_pgb_evt"),
	DEFINE_RES_IRQ_NAMED(MT6360_LDO3_PGB_EVT, "ldo3_pgb_evt"),
	DEFINE_RES_IRQ_NAMED(MT6360_LDO5_PGB_EVT, "ldo5_pgb_evt"),
<<<<<<< HEAD
};

static const struct mfd_cell mt6360_devs[] = {
	MT6360_MFD_CELL(mt6360_adc),
	MT6360_MFD_CELL(mt6360_chg),
	MT6360_MFD_CELL(mt6360_led),
	MT6360_MFD_CELL(mt6360_pmic),
	MT6360_MFD_CELL(mt6360_ldo),
	/* tcpc dev */
	{
		.name = "mt6360_tcpc",
		.of_compatible = "mediatek,mt6360_tcpc",
	},
	/* debug dev */
	{ .name = "mt6360_dbg", },
};

static const unsigned short mt6360_slave_addr[MT6360_SLAVE_MAX] = {
	MT6360_PMU_SLAVEID,
	MT6360_PMIC_SLAVEID,
	MT6360_LDO_SLAVEID,
	MT6360_TCPC_SLAVEID,
};

static int mt6360_pmu_probe(struct i2c_client *client,
			    const struct i2c_device_id *id)
{
	struct mt6360_pmu_info *mpi;
	unsigned int reg_data = 0;
	int i, ret;

	mpi = devm_kzalloc(&client->dev, sizeof(*mpi), GFP_KERNEL);
	if (!mpi)
		return -ENOMEM;
	mpi->dev = &client->dev;
	i2c_set_clientdata(client, mpi);

	/* regmap regiser */
	mpi->regmap = devm_regmap_init_i2c(client, &mt6360_pmu_regmap_config);
	if (IS_ERR(mpi->regmap)) {
		dev_err(&client->dev, "regmap register fail\n");
		return PTR_ERR(mpi->regmap);
	}
	/* chip id check */
	ret = regmap_read(mpi->regmap, MT6360_PMU_DEV_INFO, &reg_data);
	if (ret < 0) {
		dev_err(&client->dev, "device not found\n");
		return ret;
	}
	if ((reg_data & CHIP_VEN_MASK) != CHIP_VEN_MT6360) {
		dev_err(&client->dev, "not mt6360 chip\n");
		return -ENODEV;
	}
	mpi->chip_rev = reg_data & CHIP_REV_MASK;
	/* irq register */
	memcpy(&mpi->irq_chip, &mt6360_pmu_irq_chip, sizeof(mpi->irq_chip));
	mpi->irq_chip.name = dev_name(&client->dev);
	mpi->irq_chip.irq_drv_data = mpi;
	ret = devm_regmap_add_irq_chip(&client->dev, mpi->regmap, client->irq,
				       IRQF_TRIGGER_FALLING, 0, &mpi->irq_chip,
				       &mpi->irq_data);
	if (ret < 0) {
		dev_err(&client->dev, "regmap irq chip add fail\n");
		return ret;
	}
	/* new i2c slave device */
	for (i = 0; i < MT6360_SLAVE_MAX; i++) {
		if (mt6360_slave_addr[i] == client->addr) {
			mpi->i2c[i] = client;
			continue;
		}
		mpi->i2c[i] = i2c_new_dummy(client->adapter,
					    mt6360_slave_addr[i]);
		if (!mpi->i2c[i]) {
			dev_err(&client->dev, "new i2c dev [%d] fail\n", i);
			ret = -ENODEV;
			goto out;
		}
		i2c_set_clientdata(mpi->i2c[i], mpi);
	}
	/* mfd cell register */
	ret = devm_mfd_add_devices(&client->dev, PLATFORM_DEVID_AUTO,
				   mt6360_devs, ARRAY_SIZE(mt6360_devs), NULL,
				   0, regmap_irq_get_domain(mpi->irq_data));
	if (ret < 0) {
		dev_err(&client->dev, "mfd add cells fail\n");
		goto out;
	}
	dev_info(&client->dev, "Successfully probed\n");
	return 0;
out:
	while (--i >= 0) {
		if (mpi->i2c[i]->addr == client->addr)
			continue;
		i2c_unregister_device(mpi->i2c[i]);
	}
	return ret;
}

static int mt6360_pmu_remove(struct i2c_client *client)
{
	struct mt6360_pmu_info *mpi = i2c_get_clientdata(client);
	int i;

	for (i = 0; i < MT6360_SLAVE_MAX; i++) {
		if (mpi->i2c[i]->addr == client->addr)
			continue;
		i2c_unregister_device(mpi->i2c[i]);
	}
	return 0;
}

static int __maybe_unused mt6360_pmu_suspend(struct device *dev)
=======
	DEFINE_RES_IRQ_NAMED(MT6360_LDO6_PGB_EVT, "ldo6_pgb_evt"),
	DEFINE_RES_IRQ_NAMED(MT6360_LDO7_PGB_EVT, "ldo7_pgb_evt"),
};

static const struct mfd_cell mt6360_devs[] = {
	MFD_CELL_OF("mt6360-adc", mt6360_adc_resources,
		    NULL, 0, 0, "mediatek,mt6360-adc"),
	MFD_CELL_OF("mt6360-chg", mt6360_chg_resources,
		    NULL, 0, 0, "mediatek,mt6360-chg"),
	MFD_CELL_OF("mt6360-led", mt6360_led_resources,
		    NULL, 0, 0, "mediatek,mt6360-led"),
	MFD_CELL_RES("mt6360-regulator", mt6360_regulator_resources),
	MFD_CELL_OF("mt6360-tcpc", NULL,
		    NULL, 0, 0, "mediatek,mt6360-tcpc"),
};

static int mt6360_check_vendor_info(struct mt6360_ddata *ddata)
{
	u32 info;
	int ret;

	ret = regmap_read(ddata->regmap, MT6360_PMU_DEV_INFO, &info);
	if (ret < 0)
		return ret;

	if ((info & CHIP_VEN_MASK) != CHIP_VEN_MT6360) {
		dev_err(ddata->dev, "Device not supported\n");
		return -ENODEV;
	}

	ddata->chip_rev = info & CHIP_REV_MASK;

	return 0;
}

static const unsigned short mt6360_slave_addr[MT6360_SLAVE_MAX] = {
	MT6360_TCPC_SLAVEID,
	MT6360_PMIC_SLAVEID,
	MT6360_LDO_SLAVEID,
	MT6360_PMU_SLAVEID,
};

static int mt6360_xlate_pmicldo_addr(u8 *addr, int rw_size)
{
	/* Address is already in encoded [5:0] */
	*addr &= MT6360_ADDRESS_MASK;

	switch (rw_size) {
	case 1:
		*addr |= MT6360_DATA_SIZE_1_BYTE;
		break;
	case 2:
		*addr |= MT6360_DATA_SIZE_2_BYTES;
		break;
	case 3:
		*addr |= MT6360_DATA_SIZE_3_BYTES;
		break;
	case 4:
		*addr |= MT6360_DATA_SIZE_4_BYTES;
		break;
	default:
		return -EINVAL;
	}

	return 0;
}

static int mt6360_regmap_read(void *context, const void *reg, size_t reg_size,
			      void *val, size_t val_size)
{
	struct mt6360_ddata *ddata = context;
	u8 bank = *(u8 *)reg;
	u8 reg_addr = *(u8 *)(reg + 1);
	struct i2c_client *i2c = ddata->i2c[bank];
	bool crc_needed = false;
	u8 *buf;
	int buf_len = MT6360_ALLOC_READ_SIZE(val_size);
	int read_size = val_size;
	u8 crc;
	int ret;

	if (bank == MT6360_SLAVE_PMIC || bank == MT6360_SLAVE_LDO) {
		crc_needed = true;
		ret = mt6360_xlate_pmicldo_addr(&reg_addr, val_size);
		if (ret < 0)
			return ret;
		read_size += MT6360_CRC_CRC8_SIZE;
	}

	buf = kzalloc(buf_len, GFP_KERNEL);
	if (!buf)
		return -ENOMEM;

	buf[0] = I2C_ADDR_XLATE_8BIT(i2c->addr, I2C_SMBUS_READ);
	buf[1] = reg_addr;

	ret = i2c_smbus_read_i2c_block_data(i2c, reg_addr, read_size,
					    buf + MT6360_CRC_PREDATA_OFFSET);
	if (ret < 0)
		goto out;
	else if (ret != read_size) {
		ret = -EIO;
		goto out;
	}

	if (crc_needed) {
		crc = crc8(ddata->crc8_tbl, buf, val_size + MT6360_CRC_PREDATA_OFFSET, 0);
		if (crc != buf[val_size + MT6360_CRC_PREDATA_OFFSET]) {
			ret = -EIO;
			goto out;
		}
	}

	memcpy(val, buf + MT6360_CRC_PREDATA_OFFSET, val_size);
out:
	kfree(buf);
	return (ret < 0) ? ret : 0;
}

static int mt6360_regmap_write(void *context, const void *val, size_t val_size)
{
	struct mt6360_ddata *ddata = context;
	u8 bank = *(u8 *)val;
	u8 reg_addr = *(u8 *)(val + 1);
	struct i2c_client *i2c = ddata->i2c[bank];
	bool crc_needed = false;
	u8 *buf;
	int buf_len = MT6360_ALLOC_WRITE_SIZE(val_size);
	int write_size = val_size - MT6360_REGMAP_REG_BYTE_SIZE;
	int ret;

	if (bank == MT6360_SLAVE_PMIC || bank == MT6360_SLAVE_LDO) {
		crc_needed = true;
		ret = mt6360_xlate_pmicldo_addr(&reg_addr, val_size - MT6360_REGMAP_REG_BYTE_SIZE);
		if (ret < 0)
			return ret;
	}

	buf = kzalloc(buf_len, GFP_KERNEL);
	if (!buf)
		return -ENOMEM;

	buf[0] = I2C_ADDR_XLATE_8BIT(i2c->addr, I2C_SMBUS_WRITE);
	buf[1] = reg_addr;
	memcpy(buf + MT6360_CRC_PREDATA_OFFSET, val + MT6360_REGMAP_REG_BYTE_SIZE, write_size);

	if (crc_needed) {
		buf[val_size] = crc8(ddata->crc8_tbl, buf, val_size, 0);
		write_size += (MT6360_CRC_CRC8_SIZE + MT6360_CRC_DUMMY_BYTE_SIZE);
	}

	ret = i2c_smbus_write_i2c_block_data(i2c, reg_addr, write_size,
					     buf + MT6360_CRC_PREDATA_OFFSET);

	kfree(buf);
	return ret;
}

static const struct regmap_bus mt6360_regmap_bus = {
	.read		= mt6360_regmap_read,
	.write		= mt6360_regmap_write,

	/* Due to PMIC and LDO CRC access size limit */
	.max_raw_read	= 4,
	.max_raw_write	= 4,
};

static bool mt6360_is_readwrite_reg(struct device *dev, unsigned int reg)
{
	switch (reg) {
	case MT6360_REG_TCPCSTART ... MT6360_REG_TCPCEND:
		fallthrough;
	case MT6360_REG_PMICSTART ... MT6360_REG_PMICEND:
		fallthrough;
	case MT6360_REG_LDOSTART ... MT6360_REG_LDOEND:
		fallthrough;
	case MT6360_REG_PMUSTART ... MT6360_REG_PMUEND:
		return true;
	}

	return false;
}

static const struct regmap_config mt6360_regmap_config = {
	.reg_bits		= 16,
	.val_bits		= 8,
	.reg_format_endian	= REGMAP_ENDIAN_BIG,
	.max_register		= MT6360_REG_PMUEND,
	.writeable_reg		= mt6360_is_readwrite_reg,
	.readable_reg		= mt6360_is_readwrite_reg,
};

static int mt6360_probe(struct i2c_client *client)
{
	struct mt6360_ddata *ddata;
	int i, ret;

	ddata = devm_kzalloc(&client->dev, sizeof(*ddata), GFP_KERNEL);
	if (!ddata)
		return -ENOMEM;

	ddata->dev = &client->dev;
	i2c_set_clientdata(client, ddata);

	for (i = 0; i < MT6360_SLAVE_MAX - 1; i++) {
		ddata->i2c[i] = devm_i2c_new_dummy_device(&client->dev,
							  client->adapter,
							  mt6360_slave_addr[i]);
		if (IS_ERR(ddata->i2c[i])) {
			dev_err(&client->dev,
				"Failed to get new dummy I2C device for address 0x%x",
				mt6360_slave_addr[i]);
			return PTR_ERR(ddata->i2c[i]);
		}
	}
	ddata->i2c[MT6360_SLAVE_MAX - 1] = client;

	crc8_populate_msb(ddata->crc8_tbl, MT6360_CRC8_POLYNOMIAL);
	ddata->regmap = devm_regmap_init(ddata->dev, &mt6360_regmap_bus, ddata,
					 &mt6360_regmap_config);
	if (IS_ERR(ddata->regmap)) {
		dev_err(&client->dev, "Failed to register regmap\n");
		return PTR_ERR(ddata->regmap);
	}

	ret = mt6360_check_vendor_info(ddata);
	if (ret)
		return ret;

	ret = devm_regmap_add_irq_chip(&client->dev, ddata->regmap, client->irq,
				       0, 0, &mt6360_irq_chip,
				       &ddata->irq_data);
	if (ret) {
		dev_err(&client->dev, "Failed to add Regmap IRQ Chip\n");
		return ret;
	}

	ret = devm_mfd_add_devices(&client->dev, PLATFORM_DEVID_AUTO,
				   mt6360_devs, ARRAY_SIZE(mt6360_devs), NULL,
				   0, regmap_irq_get_domain(ddata->irq_data));
	if (ret) {
		dev_err(&client->dev,
			"Failed to register subordinate devices\n");
		return ret;
	}

	return 0;
}

static int __maybe_unused mt6360_suspend(struct device *dev)
>>>>>>> upstream/android-13
{
	struct i2c_client *i2c = to_i2c_client(dev);

	if (device_may_wakeup(dev))
		enable_irq_wake(i2c->irq);
<<<<<<< HEAD
	disable_irq(i2c->irq);
	return 0;
}

static int __maybe_unused mt6360_pmu_resume(struct device *dev)
=======

	return 0;
}

static int __maybe_unused mt6360_resume(struct device *dev)
>>>>>>> upstream/android-13
{

	struct i2c_client *i2c = to_i2c_client(dev);

<<<<<<< HEAD
	enable_irq(i2c->irq);
	if (device_may_wakeup(dev))
		disable_irq_wake(i2c->irq);
	return 0;
}

static SIMPLE_DEV_PM_OPS(mt6360_pmu_pm_ops,
			 mt6360_pmu_suspend, mt6360_pmu_resume);

static const struct of_device_id __maybe_unused mt6360_pmu_of_id[] = {
	{ .compatible = "mediatek,mt6360_pmu", },
	{},
};
MODULE_DEVICE_TABLE(of, mt6360_pmu_of_id);

static const struct i2c_device_id mt6360_pmu_id[] = {
	{ "mt6360_pmu", 0 },
	{},
};
MODULE_DEVICE_TABLE(i2c, mt6360_pmu_id);

static struct i2c_driver mt6360_pmu_driver = {
	.driver = {
		.name = "mt6360_pmu",
		.owner = THIS_MODULE,
		.pm = &mt6360_pmu_pm_ops,
		.of_match_table = of_match_ptr(mt6360_pmu_of_id),
	},
	.probe = mt6360_pmu_probe,
	.remove = mt6360_pmu_remove,
	.id_table = mt6360_pmu_id,
};
module_i2c_driver(mt6360_pmu_driver);

MODULE_AUTHOR("CY_Huang <cy_huang@richtek.com>");
MODULE_DESCRIPTION("MT6360 PMU I2C Driver");
MODULE_LICENSE("GPL");
MODULE_VERSION("1.0.0");
=======
	if (device_may_wakeup(dev))
		disable_irq_wake(i2c->irq);

	return 0;
}

static SIMPLE_DEV_PM_OPS(mt6360_pm_ops, mt6360_suspend, mt6360_resume);

static const struct of_device_id __maybe_unused mt6360_of_id[] = {
	{ .compatible = "mediatek,mt6360", },
	{},
};
MODULE_DEVICE_TABLE(of, mt6360_of_id);

static struct i2c_driver mt6360_driver = {
	.driver = {
		.name = "mt6360",
		.pm = &mt6360_pm_ops,
		.of_match_table = of_match_ptr(mt6360_of_id),
	},
	.probe_new = mt6360_probe,
};
module_i2c_driver(mt6360_driver);

MODULE_AUTHOR("Gene Chen <gene_chen@richtek.com>");
MODULE_DESCRIPTION("MT6360 I2C Driver");
MODULE_LICENSE("GPL v2");
>>>>>>> upstream/android-13
