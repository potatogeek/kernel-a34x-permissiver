/* SPDX-License-Identifier: GPL-2.0 */
#ifndef BME680_H_
#define BME680_H_

#define BME680_REG_CHIP_ID			0xD0
<<<<<<< HEAD
#define BME680_CHIP_ID_VAL			0x61
#define BME680_REG_SOFT_RESET			0xE0
#define BME680_CMD_SOFTRESET			0xB6
=======
#define   BME680_CHIP_ID_VAL			0x61
#define BME680_REG_SOFT_RESET			0xE0
#define   BME680_CMD_SOFTRESET			0xB6
>>>>>>> upstream/android-13
#define BME680_REG_STATUS			0x73
#define   BME680_SPI_MEM_PAGE_BIT		BIT(4)
#define     BME680_SPI_MEM_PAGE_1_VAL		1

#define BME680_REG_TEMP_MSB			0x22
#define BME680_REG_PRESS_MSB			0x1F
#define BM6880_REG_HUMIDITY_MSB			0x25
#define BME680_REG_GAS_MSB			0x2A
#define BME680_REG_GAS_R_LSB			0x2B
#define   BME680_GAS_STAB_BIT			BIT(4)
<<<<<<< HEAD
=======
#define   BME680_GAS_RANGE_MASK			GENMASK(3, 0)
>>>>>>> upstream/android-13

#define BME680_REG_CTRL_HUMIDITY		0x72
#define   BME680_OSRS_HUMIDITY_MASK		GENMASK(2, 0)

#define BME680_REG_CTRL_MEAS			0x74
#define   BME680_OSRS_TEMP_MASK			GENMASK(7, 5)
#define   BME680_OSRS_PRESS_MASK		GENMASK(4, 2)
#define   BME680_MODE_MASK			GENMASK(1, 0)
<<<<<<< HEAD

#define BME680_MODE_FORCED			1
#define BME680_MODE_SLEEP			0
=======
#define     BME680_MODE_FORCED			1
#define     BME680_MODE_SLEEP			0
>>>>>>> upstream/android-13

#define BME680_REG_CONFIG			0x75
#define   BME680_FILTER_MASK			GENMASK(4, 2)
#define     BME680_FILTER_COEFF_VAL		BIT(1)

/* TEMP/PRESS/HUMID reading skipped */
#define BME680_MEAS_SKIPPED			0x8000

#define BME680_MAX_OVERFLOW_VAL			0x40000000
#define BME680_HUM_REG_SHIFT_VAL		4
<<<<<<< HEAD
#define BME680_BIT_H1_DATA_MSK			0x0F

#define BME680_REG_RES_HEAT_RANGE		0x02
#define BME680_RHRANGE_MSK			0x30
#define BME680_REG_RES_HEAT_VAL			0x00
#define BME680_REG_RANGE_SW_ERR			0x04
#define BME680_RSERROR_MSK			0xF0
#define BME680_REG_RES_HEAT_0			0x5A
#define BME680_REG_GAS_WAIT_0			0x64
#define BME680_GAS_RANGE_MASK			0x0F
=======
#define BME680_BIT_H1_DATA_MASK			GENMASK(3, 0)

#define BME680_REG_RES_HEAT_RANGE		0x02
#define   BME680_RHRANGE_MASK			GENMASK(5, 4)
#define BME680_REG_RES_HEAT_VAL			0x00
#define BME680_REG_RANGE_SW_ERR			0x04
#define   BME680_RSERROR_MASK			GENMASK(7, 4)
#define BME680_REG_RES_HEAT_0			0x5A
#define BME680_REG_GAS_WAIT_0			0x64
>>>>>>> upstream/android-13
#define BME680_ADC_GAS_RES_SHIFT		6
#define BME680_AMB_TEMP				25

#define BME680_REG_CTRL_GAS_1			0x71
#define   BME680_RUN_GAS_MASK			BIT(4)
#define   BME680_NB_CONV_MASK			GENMASK(3, 0)
<<<<<<< HEAD
#define     BME680_RUN_GAS_EN_BIT		BIT(4)
#define     BME680_NB_CONV_0_VAL		0
=======
>>>>>>> upstream/android-13

#define BME680_REG_MEAS_STAT_0			0x1D
#define   BME680_GAS_MEAS_BIT			BIT(6)

/* Calibration Parameters */
#define BME680_T2_LSB_REG	0x8A
#define BME680_T3_REG		0x8C
#define BME680_P1_LSB_REG	0x8E
#define BME680_P2_LSB_REG	0x90
#define BME680_P3_REG		0x92
#define BME680_P4_LSB_REG	0x94
#define BME680_P5_LSB_REG	0x96
#define BME680_P7_REG		0x98
#define BME680_P6_REG		0x99
#define BME680_P8_LSB_REG	0x9C
#define BME680_P9_LSB_REG	0x9E
#define BME680_P10_REG		0xA0
#define BME680_H2_LSB_REG	0xE2
#define BME680_H2_MSB_REG	0xE1
#define BME680_H1_MSB_REG	0xE3
#define BME680_H1_LSB_REG	0xE2
#define BME680_H3_REG		0xE4
#define BME680_H4_REG		0xE5
#define BME680_H5_REG		0xE6
#define BME680_H6_REG		0xE7
#define BME680_H7_REG		0xE8
#define BME680_T1_LSB_REG	0xE9
#define BME680_GH2_LSB_REG	0xEB
#define BME680_GH1_REG		0xED
#define BME680_GH3_REG		0xEE

extern const struct regmap_config bme680_regmap_config;

int bme680_core_probe(struct device *dev, struct regmap *regmap,
		      const char *name);

#endif  /* BME680_H_ */
