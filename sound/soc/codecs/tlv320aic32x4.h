<<<<<<< HEAD
/*
 * tlv320aic32x4.h
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
=======
/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * tlv320aic32x4.h
>>>>>>> upstream/android-13
 */


#ifndef _TLV320AIC32X4_H
#define _TLV320AIC32X4_H

struct device;
struct regmap_config;

<<<<<<< HEAD
extern const struct regmap_config aic32x4_regmap_config;
int aic32x4_probe(struct device *dev, struct regmap *regmap);
int aic32x4_remove(struct device *dev);
=======
enum aic32x4_type {
	AIC32X4_TYPE_AIC32X4 = 0,
	AIC32X4_TYPE_AIC32X6,
	AIC32X4_TYPE_TAS2505,
};

extern const struct regmap_config aic32x4_regmap_config;
int aic32x4_probe(struct device *dev, struct regmap *regmap);
int aic32x4_remove(struct device *dev);
int aic32x4_register_clocks(struct device *dev, const char *mclk_name);
>>>>>>> upstream/android-13

/* tlv320aic32x4 register space (in decimal to match datasheet) */

#define AIC32X4_REG(page, reg)	((page * 128) + reg)

#define	AIC32X4_PSEL		AIC32X4_REG(0, 0)

#define	AIC32X4_RESET		AIC32X4_REG(0, 1)
#define	AIC32X4_CLKMUX		AIC32X4_REG(0, 4)
#define	AIC32X4_PLLPR		AIC32X4_REG(0, 5)
#define	AIC32X4_PLLJ		AIC32X4_REG(0, 6)
#define	AIC32X4_PLLDMSB		AIC32X4_REG(0, 7)
#define	AIC32X4_PLLDLSB		AIC32X4_REG(0, 8)
#define	AIC32X4_NDAC		AIC32X4_REG(0, 11)
#define	AIC32X4_MDAC		AIC32X4_REG(0, 12)
#define AIC32X4_DOSRMSB		AIC32X4_REG(0, 13)
#define AIC32X4_DOSRLSB		AIC32X4_REG(0, 14)
#define	AIC32X4_NADC		AIC32X4_REG(0, 18)
#define	AIC32X4_MADC		AIC32X4_REG(0, 19)
#define AIC32X4_AOSR		AIC32X4_REG(0, 20)
#define AIC32X4_CLKMUX2		AIC32X4_REG(0, 25)
#define AIC32X4_CLKOUTM		AIC32X4_REG(0, 26)
#define AIC32X4_IFACE1		AIC32X4_REG(0, 27)
#define AIC32X4_IFACE2		AIC32X4_REG(0, 28)
#define AIC32X4_IFACE3		AIC32X4_REG(0, 29)
#define AIC32X4_BCLKN		AIC32X4_REG(0, 30)
#define AIC32X4_IFACE4		AIC32X4_REG(0, 31)
#define AIC32X4_IFACE5		AIC32X4_REG(0, 32)
#define AIC32X4_IFACE6		AIC32X4_REG(0, 33)
#define AIC32X4_GPIOCTL		AIC32X4_REG(0, 52)
#define AIC32X4_DOUTCTL		AIC32X4_REG(0, 53)
#define AIC32X4_DINCTL		AIC32X4_REG(0, 54)
#define AIC32X4_MISOCTL		AIC32X4_REG(0, 55)
#define AIC32X4_SCLKCTL		AIC32X4_REG(0, 56)
#define AIC32X4_DACSPB		AIC32X4_REG(0, 60)
#define AIC32X4_ADCSPB		AIC32X4_REG(0, 61)
#define AIC32X4_DACSETUP	AIC32X4_REG(0, 63)
#define AIC32X4_DACMUTE		AIC32X4_REG(0, 64)
#define AIC32X4_LDACVOL		AIC32X4_REG(0, 65)
#define AIC32X4_RDACVOL		AIC32X4_REG(0, 66)
#define AIC32X4_ADCSETUP	AIC32X4_REG(0, 81)
#define	AIC32X4_ADCFGA		AIC32X4_REG(0, 82)
#define AIC32X4_LADCVOL		AIC32X4_REG(0, 83)
#define AIC32X4_RADCVOL		AIC32X4_REG(0, 84)
#define AIC32X4_LAGC1		AIC32X4_REG(0, 86)
#define AIC32X4_LAGC2		AIC32X4_REG(0, 87)
#define AIC32X4_LAGC3		AIC32X4_REG(0, 88)
#define AIC32X4_LAGC4		AIC32X4_REG(0, 89)
#define AIC32X4_LAGC5		AIC32X4_REG(0, 90)
#define AIC32X4_LAGC6		AIC32X4_REG(0, 91)
#define AIC32X4_LAGC7		AIC32X4_REG(0, 92)
#define AIC32X4_RAGC1		AIC32X4_REG(0, 94)
#define AIC32X4_RAGC2		AIC32X4_REG(0, 95)
#define AIC32X4_RAGC3		AIC32X4_REG(0, 96)
#define AIC32X4_RAGC4		AIC32X4_REG(0, 97)
#define AIC32X4_RAGC5		AIC32X4_REG(0, 98)
#define AIC32X4_RAGC6		AIC32X4_REG(0, 99)
#define AIC32X4_RAGC7		AIC32X4_REG(0, 100)

#define AIC32X4_PWRCFG		AIC32X4_REG(1, 1)
#define AIC32X4_LDOCTL		AIC32X4_REG(1, 2)
<<<<<<< HEAD
=======
#define AIC32X4_LPLAYBACK	AIC32X4_REG(1, 3)
#define AIC32X4_RPLAYBACK	AIC32X4_REG(1, 4)
>>>>>>> upstream/android-13
#define AIC32X4_OUTPWRCTL	AIC32X4_REG(1, 9)
#define AIC32X4_CMMODE		AIC32X4_REG(1, 10)
#define AIC32X4_HPLROUTE	AIC32X4_REG(1, 12)
#define AIC32X4_HPRROUTE	AIC32X4_REG(1, 13)
#define AIC32X4_LOLROUTE	AIC32X4_REG(1, 14)
#define AIC32X4_LORROUTE	AIC32X4_REG(1, 15)
#define	AIC32X4_HPLGAIN		AIC32X4_REG(1, 16)
#define	AIC32X4_HPRGAIN		AIC32X4_REG(1, 17)
#define	AIC32X4_LOLGAIN		AIC32X4_REG(1, 18)
#define	AIC32X4_LORGAIN		AIC32X4_REG(1, 19)
#define AIC32X4_HEADSTART	AIC32X4_REG(1, 20)
<<<<<<< HEAD
=======
#define TAS2505_SPK		AIC32X4_REG(1, 45)
#define TAS2505_SPKVOL1		AIC32X4_REG(1, 46)
#define TAS2505_SPKVOL2		AIC32X4_REG(1, 48)
>>>>>>> upstream/android-13
#define AIC32X4_MICBIAS		AIC32X4_REG(1, 51)
#define AIC32X4_LMICPGAPIN	AIC32X4_REG(1, 52)
#define AIC32X4_LMICPGANIN	AIC32X4_REG(1, 54)
#define AIC32X4_RMICPGAPIN	AIC32X4_REG(1, 55)
#define AIC32X4_RMICPGANIN	AIC32X4_REG(1, 57)
#define AIC32X4_FLOATINGINPUT	AIC32X4_REG(1, 58)
#define AIC32X4_LMICPGAVOL	AIC32X4_REG(1, 59)
#define AIC32X4_RMICPGAVOL	AIC32X4_REG(1, 60)
<<<<<<< HEAD
=======
#define TAS2505_REFPOWERUP	AIC32X4_REG(1, 122)
#define AIC32X4_REFPOWERUP	AIC32X4_REG(1, 123)
>>>>>>> upstream/android-13

/* Bits, masks, and shifts */

/* AIC32X4_CLKMUX */
#define AIC32X4_PLL_CLKIN_MASK		GENMASK(3, 2)
#define AIC32X4_PLL_CLKIN_SHIFT		(2)
#define AIC32X4_PLL_CLKIN_MCLK		(0x00)
#define AIC32X4_PLL_CLKIN_BCKL		(0x01)
#define AIC32X4_PLL_CLKIN_GPIO1		(0x02)
#define AIC32X4_PLL_CLKIN_DIN		(0x03)
#define AIC32X4_CODEC_CLKIN_MASK	GENMASK(1, 0)
#define AIC32X4_CODEC_CLKIN_SHIFT	(0)
#define AIC32X4_CODEC_CLKIN_MCLK	(0x00)
#define AIC32X4_CODEC_CLKIN_BCLK	(0x01)
#define AIC32X4_CODEC_CLKIN_GPIO1	(0x02)
#define AIC32X4_CODEC_CLKIN_PLL		(0x03)

/* AIC32X4_PLLPR */
#define AIC32X4_PLLEN			BIT(7)
#define AIC32X4_PLL_P_MASK		GENMASK(6, 4)
#define AIC32X4_PLL_P_SHIFT		(4)
#define AIC32X4_PLL_R_MASK		GENMASK(3, 0)

/* AIC32X4_NDAC */
#define AIC32X4_NDACEN			BIT(7)
#define AIC32X4_NDAC_MASK		GENMASK(6, 0)

/* AIC32X4_MDAC */
#define AIC32X4_MDACEN			BIT(7)
#define AIC32X4_MDAC_MASK		GENMASK(6, 0)

/* AIC32X4_NADC */
#define AIC32X4_NADCEN			BIT(7)
#define AIC32X4_NADC_MASK		GENMASK(6, 0)

/* AIC32X4_MADC */
#define AIC32X4_MADCEN			BIT(7)
#define AIC32X4_MADC_MASK		GENMASK(6, 0)

/* AIC32X4_BCLKN */
#define AIC32X4_BCLKEN			BIT(7)
#define AIC32X4_BCLK_MASK		GENMASK(6, 0)

/* AIC32X4_IFACE1 */
#define AIC32X4_IFACE1_DATATYPE_MASK	GENMASK(7, 6)
#define AIC32X4_IFACE1_DATATYPE_SHIFT	(6)
#define AIC32X4_I2S_MODE		(0x00)
#define AIC32X4_DSP_MODE		(0x01)
#define AIC32X4_RIGHT_JUSTIFIED_MODE	(0x02)
#define AIC32X4_LEFT_JUSTIFIED_MODE	(0x03)
#define AIC32X4_IFACE1_DATALEN_MASK	GENMASK(5, 4)
#define AIC32X4_IFACE1_DATALEN_SHIFT	(4)
#define AIC32X4_WORD_LEN_16BITS		(0x00)
#define AIC32X4_WORD_LEN_20BITS		(0x01)
#define AIC32X4_WORD_LEN_24BITS		(0x02)
#define AIC32X4_WORD_LEN_32BITS		(0x03)
#define AIC32X4_IFACE1_MASTER_MASK	GENMASK(3, 2)
#define AIC32X4_BCLKMASTER		BIT(2)
#define AIC32X4_WCLKMASTER		BIT(3)

/* AIC32X4_IFACE2 */
#define AIC32X4_DATA_OFFSET_MASK	GENMASK(7, 0)

/* AIC32X4_IFACE3 */
#define AIC32X4_BCLKINV_MASK		BIT(3)
#define AIC32X4_BDIVCLK_MASK		GENMASK(1, 0)
#define AIC32X4_BDIVCLK_SHIFT		(0)
#define AIC32X4_DAC2BCLK		(0x00)
#define AIC32X4_DACMOD2BCLK		(0x01)
#define AIC32X4_ADC2BCLK		(0x02)
#define AIC32X4_ADCMOD2BCLK		(0x03)

/* AIC32X4_DACSETUP */
#define AIC32X4_DAC_CHAN_MASK		GENMASK(5, 2)
#define AIC32X4_LDAC2RCHN		BIT(5)
#define AIC32X4_LDAC2LCHN		BIT(4)
#define AIC32X4_RDAC2LCHN		BIT(3)
#define AIC32X4_RDAC2RCHN		BIT(2)

/* AIC32X4_DACMUTE */
#define AIC32X4_MUTEON			0x0C

/* AIC32X4_ADCSETUP */
#define AIC32X4_LADC_EN			BIT(7)
#define AIC32X4_RADC_EN			BIT(6)

/* AIC32X4_PWRCFG */
#define AIC32X4_AVDDWEAKDISABLE		BIT(3)

/* AIC32X4_LDOCTL */
#define AIC32X4_LDOCTLEN		BIT(0)

/* AIC32X4_CMMODE */
#define AIC32X4_LDOIN_18_36		BIT(0)
#define AIC32X4_LDOIN2HP		BIT(1)

/* AIC32X4_MICBIAS */
#define AIC32X4_MICBIAS_LDOIN		BIT(3)
#define AIC32X4_MICBIAS_2075V		0x60
<<<<<<< HEAD
=======
#define AIC32x4_MICBIAS_MASK            GENMASK(6, 3)
>>>>>>> upstream/android-13

/* AIC32X4_LMICPGANIN */
#define AIC32X4_LMICPGANIN_IN2R_10K	0x10
#define AIC32X4_LMICPGANIN_CM1L_10K	0x40

/* AIC32X4_RMICPGANIN */
#define AIC32X4_RMICPGANIN_IN1L_10K	0x10
#define AIC32X4_RMICPGANIN_CM1R_10K	0x40

<<<<<<< HEAD
=======
/* AIC32X4_REFPOWERUP */
#define AIC32X4_REFPOWERUP_SLOW		0x04
#define AIC32X4_REFPOWERUP_40MS		0x05
#define AIC32X4_REFPOWERUP_80MS		0x06
#define AIC32X4_REFPOWERUP_120MS	0x07

/* Common mask and enable for all of the dividers */
#define AIC32X4_DIVEN           BIT(7)
#define AIC32X4_DIV_MASK        GENMASK(6, 0)

/* Clock Limits */
#define AIC32X4_MAX_DOSR_FREQ		6200000
#define AIC32X4_MIN_DOSR_FREQ		2800000
#define AIC32X4_MAX_CODEC_CLKIN_FREQ    110000000
#define AIC32X4_MAX_PLL_CLKIN		20000000

>>>>>>> upstream/android-13
#endif				/* _TLV320AIC32X4_H */
