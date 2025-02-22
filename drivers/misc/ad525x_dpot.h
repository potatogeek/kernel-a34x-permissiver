<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-or-later */
>>>>>>> upstream/android-13
/*
 * Driver for the Analog Devices digital potentiometers
 *
 * Copyright (C) 2010 Michael Hennerich, Analog Devices Inc.
<<<<<<< HEAD
 *
 * Licensed under the GPL-2 or later.
=======
>>>>>>> upstream/android-13
 */

#ifndef _AD_DPOT_H_
#define _AD_DPOT_H_

#include <linux/types.h>

#define DPOT_CONF(features, wipers, max_pos, uid) \
		(((features) << 18) | (((wipers) & 0xFF) << 10) | \
		((max_pos & 0xF) << 6) | (uid & 0x3F))

#define DPOT_UID(conf)		(conf & 0x3F)
#define DPOT_MAX_POS(conf)	((conf >> 6) & 0xF)
#define DPOT_WIPERS(conf)	((conf >> 10) & 0xFF)
#define DPOT_FEAT(conf)		(conf >> 18)

#define BRDAC0			(1 << 0)
#define BRDAC1			(1 << 1)
#define BRDAC2			(1 << 2)
#define BRDAC3			(1 << 3)
#define BRDAC4			(1 << 4)
#define BRDAC5			(1 << 5)
#define MAX_RDACS		6

#define F_CMD_INC		(1 << 0)	/* Features INC/DEC ALL, 6dB */
#define F_CMD_EEP		(1 << 1)	/* Features EEPROM */
#define F_CMD_OTP		(1 << 2)	/* Features OTP */
#define F_CMD_TOL		(1 << 3)	/* RDACS feature Tolerance REG */
#define F_RDACS_RW		(1 << 4)	/* RDACS are Read/Write  */
#define F_RDACS_WONLY		(1 << 5)	/* RDACS are Write only */
#define F_AD_APPDATA		(1 << 6)	/* RDAC Address append to data */
#define F_SPI_8BIT		(1 << 7)	/* All SPI XFERS are 8-bit */
#define F_SPI_16BIT		(1 << 8)	/* All SPI XFERS are 16-bit */
#define F_SPI_24BIT		(1 << 9)	/* All SPI XFERS are 24-bit */

#define F_RDACS_RW_TOL	(F_RDACS_RW | F_CMD_EEP | F_CMD_TOL)
#define F_RDACS_RW_EEP	(F_RDACS_RW | F_CMD_EEP)
#define F_SPI		(F_SPI_8BIT | F_SPI_16BIT | F_SPI_24BIT)

enum dpot_devid {
	AD5258_ID = DPOT_CONF(F_RDACS_RW_TOL, BRDAC0, 6, 0), /* I2C */
	AD5259_ID = DPOT_CONF(F_RDACS_RW_TOL, BRDAC0, 8, 1),
	AD5251_ID = DPOT_CONF(F_RDACS_RW_TOL | F_CMD_INC,
			BRDAC1 | BRDAC3, 6, 2),
	AD5252_ID = DPOT_CONF(F_RDACS_RW_TOL | F_CMD_INC,
			BRDAC1 | BRDAC3, 8, 3),
	AD5253_ID = DPOT_CONF(F_RDACS_RW_TOL | F_CMD_INC,
			BRDAC0 | BRDAC1 | BRDAC2 | BRDAC3, 6, 4),
	AD5254_ID = DPOT_CONF(F_RDACS_RW_TOL | F_CMD_INC,
			BRDAC0 | BRDAC1 | BRDAC2 | BRDAC3, 8, 5),
	AD5255_ID = DPOT_CONF(F_RDACS_RW_TOL | F_CMD_INC,
			BRDAC0 | BRDAC1 | BRDAC2, 9, 6),
	AD5160_ID = DPOT_CONF(F_RDACS_WONLY | F_AD_APPDATA | F_SPI_8BIT,
			BRDAC0, 8, 7), /* SPI */
	AD5161_ID = DPOT_CONF(F_RDACS_WONLY | F_AD_APPDATA | F_SPI_8BIT,
			BRDAC0, 8, 8),
	AD5162_ID = DPOT_CONF(F_RDACS_WONLY | F_AD_APPDATA | F_SPI_16BIT,
			BRDAC0 | BRDAC1, 8, 9),
	AD5165_ID = DPOT_CONF(F_RDACS_WONLY | F_AD_APPDATA | F_SPI_8BIT,
			BRDAC0, 8, 10),
	AD5200_ID = DPOT_CONF(F_RDACS_WONLY | F_AD_APPDATA | F_SPI_8BIT,
			BRDAC0, 8, 11),
	AD5201_ID = DPOT_CONF(F_RDACS_WONLY | F_AD_APPDATA | F_SPI_8BIT,
			BRDAC0, 5, 12),
	AD5203_ID = DPOT_CONF(F_RDACS_WONLY | F_AD_APPDATA | F_SPI_8BIT,
			BRDAC0 | BRDAC1 | BRDAC2 | BRDAC3, 6, 13),
	AD5204_ID = DPOT_CONF(F_RDACS_WONLY | F_AD_APPDATA | F_SPI_16BIT,
			BRDAC0 | BRDAC1 | BRDAC2 | BRDAC3, 8, 14),
	AD5206_ID = DPOT_CONF(F_RDACS_WONLY | F_AD_APPDATA | F_SPI_16BIT,
			BRDAC0 | BRDAC1 | BRDAC2 | BRDAC3 | BRDAC4 | BRDAC5,
			8, 15),
	AD5207_ID = DPOT_CONF(F_RDACS_WONLY | F_AD_APPDATA | F_SPI_16BIT,
			BRDAC0 | BRDAC1, 8, 16),
	AD5231_ID = DPOT_CONF(F_RDACS_RW_EEP | F_CMD_INC | F_SPI_24BIT,
			BRDAC0, 10, 17),
	AD5232_ID = DPOT_CONF(F_RDACS_RW_EEP | F_CMD_INC | F_SPI_16BIT,
			BRDAC0 | BRDAC1, 8, 18),
	AD5233_ID = DPOT_CONF(F_RDACS_RW_EEP | F_CMD_INC | F_SPI_16BIT,
			BRDAC0 | BRDAC1 | BRDAC2 | BRDAC3, 6, 19),
	AD5235_ID = DPOT_CONF(F_RDACS_RW_EEP | F_CMD_INC | F_SPI_24BIT,
			BRDAC0 | BRDAC1, 10, 20),
	AD5260_ID = DPOT_CONF(F_RDACS_WONLY | F_AD_APPDATA | F_SPI_8BIT,
			BRDAC0, 8, 21),
	AD5262_ID = DPOT_CONF(F_RDACS_WONLY | F_AD_APPDATA | F_SPI_16BIT,
			BRDAC0 | BRDAC1, 8, 22),
	AD5263_ID = DPOT_CONF(F_RDACS_WONLY | F_AD_APPDATA | F_SPI_16BIT,
			BRDAC0 | BRDAC1 | BRDAC2 | BRDAC3, 8, 23),
	AD5290_ID = DPOT_CONF(F_RDACS_WONLY | F_AD_APPDATA | F_SPI_8BIT,
			BRDAC0, 8, 24),
	AD5291_ID = DPOT_CONF(F_RDACS_RW | F_SPI_16BIT | F_CMD_OTP,
			BRDAC0, 8, 25),
	AD5292_ID = DPOT_CONF(F_RDACS_RW | F_SPI_16BIT | F_CMD_OTP,
			BRDAC0, 10, 26),
	AD5293_ID = DPOT_CONF(F_RDACS_RW | F_SPI_16BIT, BRDAC0, 10, 27),
	AD7376_ID = DPOT_CONF(F_RDACS_WONLY | F_AD_APPDATA | F_SPI_8BIT,
			BRDAC0, 7, 28),
	AD8400_ID = DPOT_CONF(F_RDACS_WONLY | F_AD_APPDATA | F_SPI_16BIT,
			BRDAC0, 8, 29),
	AD8402_ID = DPOT_CONF(F_RDACS_WONLY | F_AD_APPDATA | F_SPI_16BIT,
			BRDAC0 | BRDAC1, 8, 30),
	AD8403_ID = DPOT_CONF(F_RDACS_WONLY | F_AD_APPDATA | F_SPI_16BIT,
			BRDAC0 | BRDAC1 | BRDAC2, 8, 31),
	ADN2850_ID = DPOT_CONF(F_RDACS_RW_EEP | F_CMD_INC | F_SPI_24BIT,
			BRDAC0 | BRDAC1, 10, 32),
	AD5241_ID = DPOT_CONF(F_RDACS_RW, BRDAC0, 8, 33),
	AD5242_ID = DPOT_CONF(F_RDACS_RW, BRDAC0 | BRDAC1, 8, 34),
	AD5243_ID = DPOT_CONF(F_RDACS_RW, BRDAC0 | BRDAC1, 8, 35),
	AD5245_ID = DPOT_CONF(F_RDACS_RW, BRDAC0, 8, 36),
	AD5246_ID = DPOT_CONF(F_RDACS_RW, BRDAC0, 7, 37),
	AD5247_ID = DPOT_CONF(F_RDACS_RW, BRDAC0, 7, 38),
	AD5248_ID = DPOT_CONF(F_RDACS_RW, BRDAC0 | BRDAC1, 8, 39),
	AD5280_ID = DPOT_CONF(F_RDACS_RW, BRDAC0, 8, 40),
	AD5282_ID = DPOT_CONF(F_RDACS_RW, BRDAC0 | BRDAC1, 8, 41),
	ADN2860_ID = DPOT_CONF(F_RDACS_RW_TOL | F_CMD_INC,
			BRDAC0 | BRDAC1 | BRDAC2, 9, 42),
	AD5273_ID = DPOT_CONF(F_RDACS_RW | F_CMD_OTP, BRDAC0, 6, 43),
	AD5171_ID = DPOT_CONF(F_RDACS_RW | F_CMD_OTP, BRDAC0, 6, 44),
	AD5170_ID = DPOT_CONF(F_RDACS_RW | F_CMD_OTP, BRDAC0, 8, 45),
	AD5172_ID = DPOT_CONF(F_RDACS_RW | F_CMD_OTP, BRDAC0 | BRDAC1, 8, 46),
	AD5173_ID = DPOT_CONF(F_RDACS_RW | F_CMD_OTP, BRDAC0 | BRDAC1, 8, 47),
	AD5270_ID = DPOT_CONF(F_RDACS_RW | F_CMD_OTP | F_SPI_16BIT,
			BRDAC0, 10, 48),
	AD5271_ID = DPOT_CONF(F_RDACS_RW | F_CMD_OTP | F_SPI_16BIT,
			BRDAC0, 8, 49),
	AD5272_ID = DPOT_CONF(F_RDACS_RW | F_CMD_OTP, BRDAC0, 10, 50),
	AD5274_ID = DPOT_CONF(F_RDACS_RW | F_CMD_OTP, BRDAC0, 8, 51),
};

#define DPOT_RDAC0		0
#define DPOT_RDAC1		1
#define DPOT_RDAC2		2
#define DPOT_RDAC3		3
#define DPOT_RDAC4		4
#define DPOT_RDAC5		5

#define DPOT_RDAC_MASK		0x1F

#define DPOT_REG_TOL		0x18
#define DPOT_TOL_RDAC0		(DPOT_REG_TOL | DPOT_RDAC0)
#define DPOT_TOL_RDAC1		(DPOT_REG_TOL | DPOT_RDAC1)
#define DPOT_TOL_RDAC2		(DPOT_REG_TOL | DPOT_RDAC2)
#define DPOT_TOL_RDAC3		(DPOT_REG_TOL | DPOT_RDAC3)
#define DPOT_TOL_RDAC4		(DPOT_REG_TOL | DPOT_RDAC4)
#define DPOT_TOL_RDAC5		(DPOT_REG_TOL | DPOT_RDAC5)

/* RDAC-to-EEPROM Interface Commands */
#define DPOT_ADDR_RDAC		(0x0 << 5)
#define DPOT_ADDR_EEPROM	(0x1 << 5)
#define DPOT_ADDR_OTP		(0x1 << 6)
#define DPOT_ADDR_CMD		(0x1 << 7)
#define DPOT_ADDR_OTP_EN	(0x1 << 9)

#define DPOT_DEC_ALL_6DB	(DPOT_ADDR_CMD | (0x4 << 3))
#define DPOT_INC_ALL_6DB	(DPOT_ADDR_CMD | (0x9 << 3))
#define DPOT_DEC_ALL		(DPOT_ADDR_CMD | (0x6 << 3))
#define DPOT_INC_ALL		(DPOT_ADDR_CMD | (0xB << 3))

#define DPOT_SPI_RDAC		0xB0
#define DPOT_SPI_EEPROM		0x30
#define DPOT_SPI_READ_RDAC	0xA0
#define DPOT_SPI_READ_EEPROM	0x90
#define DPOT_SPI_DEC_ALL_6DB	0x50
#define DPOT_SPI_INC_ALL_6DB	0xD0
#define DPOT_SPI_DEC_ALL	0x70
#define DPOT_SPI_INC_ALL	0xF0

/* AD5291/2/3 use special commands */
#define DPOT_AD5291_RDAC	0x01
#define DPOT_AD5291_READ_RDAC	0x02
#define DPOT_AD5291_STORE_XTPM	0x03
#define DPOT_AD5291_CTRLREG	0x06
#define DPOT_AD5291_UNLOCK_CMD	0x03

/* AD5270/1/2/4 use special commands */
#define DPOT_AD5270_1_2_4_RDAC		0x01
#define DPOT_AD5270_1_2_4_READ_RDAC	0x02
#define DPOT_AD5270_1_2_4_STORE_XTPM	0x03
#define DPOT_AD5270_1_2_4_CTRLREG	0x07
#define DPOT_AD5270_1_2_4_UNLOCK_CMD	0x03

#define DPOT_AD5282_RDAC_AB	0x80

#define DPOT_AD5273_FUSE	0x80
#define DPOT_AD5170_2_3_FUSE	0x20
#define DPOT_AD5170_2_3_OW	0x08
#define DPOT_AD5172_3_A0	0x08
#define DPOT_AD5170_2FUSE	0x80

struct dpot_data;

struct ad_dpot_bus_ops {
	int (*read_d8)(void *client);
	int (*read_r8d8)(void *client, u8 reg);
	int (*read_r8d16)(void *client, u8 reg);
	int (*write_d8)(void *client, u8 val);
	int (*write_r8d8)(void *client, u8 reg, u8 val);
	int (*write_r8d16)(void *client, u8 reg, u16 val);
};

struct ad_dpot_bus_data {
	void *client;
	const struct ad_dpot_bus_ops *bops;
};

int ad_dpot_probe(struct device *dev, struct ad_dpot_bus_data *bdata,
		  unsigned long devid, const char *name);
int ad_dpot_remove(struct device *dev);

#endif
