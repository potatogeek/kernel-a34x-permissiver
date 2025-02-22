<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> upstream/android-13
/*
 * NAU8810 ALSA SoC audio driver
 *
 * Copyright 2016 Nuvoton Technology Corp.
 * Author: David Lin <ctlin0@nuvoton.com>
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
=======
>>>>>>> upstream/android-13
 */

#ifndef __NAU8810_H__
#define __NAU8810_H__

#define NAU8810_REG_RESET		0x00
#define NAU8810_REG_POWER1		0x01
#define NAU8810_REG_POWER2		0x02
#define NAU8810_REG_POWER3		0x03
#define NAU8810_REG_IFACE		0x04
#define NAU8810_REG_COMP		0x05
#define NAU8810_REG_CLOCK		0x06
#define NAU8810_REG_SMPLR		0x07
#define NAU8810_REG_DAC		0x0A
#define NAU8810_REG_DACGAIN		0x0B
#define NAU8810_REG_ADC		0x0E
#define NAU8810_REG_ADCGAIN		0x0F
#define NAU8810_REG_EQ1		0x12
#define NAU8810_REG_EQ2		0x13
#define NAU8810_REG_EQ3		0x14
#define NAU8810_REG_EQ4		0x15
#define NAU8810_REG_EQ5		0x16
#define NAU8810_REG_DACLIM1		0x18
#define NAU8810_REG_DACLIM2		0x19
#define NAU8810_REG_NOTCH1		0x1B
#define NAU8810_REG_NOTCH2		0x1C
#define NAU8810_REG_NOTCH3		0x1D
#define NAU8810_REG_NOTCH4		0x1E
#define NAU8810_REG_ALC1		0x20
#define NAU8810_REG_ALC2		0x21
#define NAU8810_REG_ALC3		0x22
#define NAU8810_REG_NOISEGATE		0x23
#define NAU8810_REG_PLLN		0x24
#define NAU8810_REG_PLLK1		0x25
#define NAU8810_REG_PLLK2		0x26
#define NAU8810_REG_PLLK3		0x27
#define NAU8810_REG_ATTEN		0x28
#define NAU8810_REG_INPUT_SIGNAL	0x2C
#define NAU8810_REG_PGAGAIN		0x2D
#define NAU8810_REG_ADCBOOST		0x2F
#define NAU8810_REG_OUTPUT		0x31
#define NAU8810_REG_SPKMIX		0x32
#define NAU8810_REG_SPKGAIN		0x36
#define NAU8810_REG_MONOMIX		0x38
#define NAU8810_REG_POWER4		0x3A
#define NAU8810_REG_TSLOTCTL1		0x3B
#define NAU8810_REG_TSLOTCTL2		0x3C
#define NAU8810_REG_DEVICE_REVID	0x3E
#define NAU8810_REG_I2C_DEVICEID	0x3F
#define NAU8810_REG_ADDITIONID	0x40
#define NAU8810_REG_RESERVE		0x41
#define NAU8810_REG_OUTCTL		0x45
#define NAU8810_REG_ALC1ENHAN1	0x46
#define NAU8810_REG_ALC1ENHAN2	0x47
#define NAU8810_REG_MISCCTL		0x49
#define NAU8810_REG_OUTTIEOFF		0x4B
#define NAU8810_REG_AGCP2POUT	0x4C
#define NAU8810_REG_AGCPOUT		0x4D
#define NAU8810_REG_AMTCTL		0x4E
#define NAU8810_REG_OUTTIEOFFMAN	0x4F
#define NAU8810_REG_MAX		NAU8810_REG_OUTTIEOFFMAN


/* NAU8810_REG_POWER1 (0x1) */
#define NAU8810_DCBUF_EN		(0x1 << 8)
<<<<<<< HEAD
=======
#define NAU8810_AUX_EN_SFT		6
>>>>>>> upstream/android-13
#define NAU8810_PLL_EN_SFT		5
#define NAU8810_MICBIAS_EN_SFT	4
#define NAU8810_ABIAS_EN		(0x1 << 3)
#define NAU8810_IOBUF_EN		(0x1 << 2)
#define NAU8810_REFIMP_MASK		0x3
#define NAU8810_REFIMP_DIS		0x0
#define NAU8810_REFIMP_80K		0x1
#define NAU8810_REFIMP_300K		0x2
#define NAU8810_REFIMP_3K		0x3

/* NAU8810_REG_POWER2 (0x2) */
#define NAU8810_BST_EN_SFT		4
#define NAU8810_PGA_EN_SFT		2
#define NAU8810_ADC_EN_SFT		0

/* NAU8810_REG_POWER3 (0x3) */
#define NAU8810_DAC_EN_SFT		0
#define NAU8810_SPKMX_EN_SFT		2
#define NAU8810_MOUTMX_EN_SFT	3
#define NAU8810_PSPK_EN_SFT		5
#define NAU8810_NSPK_EN_SFT		6
#define NAU8810_MOUT_EN_SFT		7

/* NAU8810_REG_IFACE (0x4) */
#define NAU8810_AIFMT_SFT		3
#define NAU8810_AIFMT_MASK		(0x3 << NAU8810_AIFMT_SFT)
#define NAU8810_AIFMT_RIGHT		(0x0 << NAU8810_AIFMT_SFT)
#define NAU8810_AIFMT_LEFT		(0x1 << NAU8810_AIFMT_SFT)
#define NAU8810_AIFMT_I2S		(0x2 << NAU8810_AIFMT_SFT)
#define NAU8810_AIFMT_PCM_A		(0x3 << NAU8810_AIFMT_SFT)
#define NAU8810_WLEN_SFT		5
#define NAU8810_WLEN_MASK		(0x3 << NAU8810_WLEN_SFT)
#define NAU8810_WLEN_16		(0x0 << NAU8810_WLEN_SFT)
#define NAU8810_WLEN_20		(0x1 << NAU8810_WLEN_SFT)
#define NAU8810_WLEN_24		(0x2 << NAU8810_WLEN_SFT)
#define NAU8810_WLEN_32		(0x3 << NAU8810_WLEN_SFT)
#define NAU8810_FSP_IF			(0x1 << 7)
#define NAU8810_BCLKP_IB		(0x1 << 8)

/* NAU8810_REG_COMP (0x5) */
#define NAU8810_ADDAP_SFT		0
#define NAU8810_ADCCM_SFT		1
#define NAU8810_DACCM_SFT		3

/* NAU8810_REG_CLOCK (0x6) */
#define NAU8810_CLKIO_MASK		0x1
#define NAU8810_CLKIO_SLAVE		0x0
#define NAU8810_CLKIO_MASTER		0x1
#define NAU8810_BCLKSEL_SFT		2
#define NAU8810_BCLKSEL_MASK		(0x7 << NAU8810_BCLKSEL_SFT)
#define NAU8810_BCLKDIV_1		(0x0 << NAU8810_BCLKSEL_SFT)
#define NAU8810_BCLKDIV_2		(0x1 << NAU8810_BCLKSEL_SFT)
#define NAU8810_BCLKDIV_4		(0x2 << NAU8810_BCLKSEL_SFT)
#define NAU8810_BCLKDIV_8		(0x3 << NAU8810_BCLKSEL_SFT)
#define NAU8810_BCLKDIV_16		(0x4 << NAU8810_BCLKSEL_SFT)
#define NAU8810_BCLKDIV_32		(0x5 << NAU8810_BCLKSEL_SFT)
#define NAU8810_MCLKSEL_SFT		5
#define NAU8810_MCLKSEL_MASK		(0x7 << NAU8810_MCLKSEL_SFT)
#define NAU8810_CLKM_SFT		8
#define NAU8810_CLKM_MASK		(0x1 << NAU8810_CLKM_SFT)
#define NAU8810_CLKM_MCLK		(0x0 << NAU8810_CLKM_SFT)
#define NAU8810_CLKM_PLL		(0x1 << NAU8810_CLKM_SFT)

/* NAU8810_REG_SMPLR (0x7) */
#define NAU8810_SMPLR_SFT		1
#define NAU8810_SMPLR_MASK		(0x7 << NAU8810_SMPLR_SFT)
#define NAU8810_SMPLR_48K		(0x0 << NAU8810_SMPLR_SFT)
#define NAU8810_SMPLR_32K		(0x1 << NAU8810_SMPLR_SFT)
#define NAU8810_SMPLR_24K		(0x2 << NAU8810_SMPLR_SFT)
#define NAU8810_SMPLR_16K		(0x3 << NAU8810_SMPLR_SFT)
#define NAU8810_SMPLR_12K		(0x4 << NAU8810_SMPLR_SFT)
#define NAU8810_SMPLR_8K		(0x5 << NAU8810_SMPLR_SFT)

/* NAU8810_REG_DAC (0xA) */
#define NAU8810_DACPL_SFT		0
#define NAU8810_DACOS_SFT		3
#define NAU8810_DEEMP_SFT		4

/* NAU8810_REG_DACGAIN (0xB) */
#define NAU8810_DACGAIN_SFT		0

/* NAU8810_REG_ADC (0xE) */
#define NAU8810_ADCPL_SFT		0
#define NAU8810_ADCOS_SFT		3
#define NAU8810_HPF_SFT		4
#define NAU8810_HPFEN_SFT		8

/* NAU8810_REG_ADCGAIN (0xF) */
#define NAU8810_ADCGAIN_SFT		0

/* NAU8810_REG_EQ1 (0x12) */
#define NAU8810_EQ1GC_SFT		0
#define NAU8810_EQ1CF_SFT		5
#define NAU8810_EQM_SFT		8

/* NAU8810_REG_EQ2 (0x13) */
#define NAU8810_EQ2GC_SFT		0
#define NAU8810_EQ2CF_SFT		5
#define NAU8810_EQ2BW_SFT		8

/* NAU8810_REG_EQ3 (0x14) */
#define NAU8810_EQ3GC_SFT		0
#define NAU8810_EQ3CF_SFT		5
#define NAU8810_EQ3BW_SFT		8

/* NAU8810_REG_EQ4 (0x15) */
#define NAU8810_EQ4GC_SFT		0
#define NAU8810_EQ4CF_SFT		5
#define NAU8810_EQ4BW_SFT		8

/* NAU8810_REG_EQ5 (0x16) */
#define NAU8810_EQ5GC_SFT		0
#define NAU8810_EQ5CF_SFT		5

/* NAU8810_REG_DACLIM1 (0x18) */
#define NAU8810_DACLIMATK_SFT		0
#define NAU8810_DACLIMDCY_SFT		4
#define NAU8810_DACLIMEN_SFT		8

/* NAU8810_REG_DACLIM2 (0x19) */
#define NAU8810_DACLIMBST_SFT		0
#define NAU8810_DACLIMTHL_SFT		4

/* NAU8810_REG_ALC1 (0x20) */
#define NAU8810_ALCMINGAIN_SFT	0
#define NAU8810_ALCMXGAIN_SFT		3
#define NAU8810_ALCEN_SFT		8

/* NAU8810_REG_ALC2 (0x21) */
#define NAU8810_ALCSL_SFT		0
#define NAU8810_ALCHT_SFT		4
#define NAU8810_ALCZC_SFT		8

/* NAU8810_REG_ALC3 (0x22) */
#define NAU8810_ALCATK_SFT		0
#define NAU8810_ALCDCY_SFT		4
#define NAU8810_ALCM_SFT		8

/* NAU8810_REG_NOISEGATE (0x23) */
#define NAU8810_ALCNTH_SFT		0
#define NAU8810_ALCNEN_SFT		3

/* NAU8810_REG_PLLN (0x24) */
#define NAU8810_PLLN_MASK		0xF
#define NAU8810_PLLMCLK_DIV2		(0x1 << 4)

/* NAU8810_REG_PLLK1 (0x25) */
#define NAU8810_PLLK1_SFT		18
#define NAU8810_PLLK1_MASK		0x3F

/* NAU8810_REG_PLLK2 (0x26) */
#define NAU8810_PLLK2_SFT		9
#define NAU8810_PLLK2_MASK		0x1FF

/* NAU8810_REG_PLLK3 (0x27) */
#define NAU8810_PLLK3_MASK		0x1FF

/* NAU8810_REG_INPUT_SIGNAL (0x2C) */
#define NAU8810_PMICPGA_SFT		0
<<<<<<< HEAD
#define NAU8810_NMICPGA_SFT		1
=======
#define NAU8810_PMICPGA_EN		(0x1 << NAU8810_PMICPGA_SFT)
#define NAU8810_NMICPGA_SFT		1
#define NAU8810_NMICPGA_EN		(0x1 << NAU8810_NMICPGA_SFT)
#define NAU8810_AUXPGA_SFT		2
>>>>>>> upstream/android-13

/* NAU8810_REG_PGAGAIN (0x2D) */
#define NAU8810_PGAGAIN_SFT		0
#define NAU8810_PGAMT_SFT		6
#define NAU8810_PGAZC_SFT		7

/* NAU8810_REG_ADCBOOST (0x2F) */
<<<<<<< HEAD
#define NAU8810_PMICBSTGAIN_SFT	4
=======
#define NAU8810_AUXBSTGAIN_SFT	0
#define NAU8810_PMICBSTGAIN_SFT	4
#define NAU8810_PMICBSTGAIN_MASK	(0x7 << NAU8810_PMICBSTGAIN_SFT)
>>>>>>> upstream/android-13
#define NAU8810_PGABST_SFT		8

/* NAU8810_REG_SPKMIX (0x32) */
#define NAU8810_DACSPK_SFT		0
#define NAU8810_BYPSPK_SFT		1
<<<<<<< HEAD
=======
#define NAU8810_AUXSPK_SFT		5
>>>>>>> upstream/android-13

/* NAU8810_REG_SPKGAIN (0x36) */
#define NAU8810_SPKGAIN_SFT		0
#define NAU8810_SPKMT_SFT		6
#define NAU8810_SPKZC_SFT		7

/* NAU8810_REG_MONOMIX (0x38) */
#define NAU8810_DACMOUT_SFT		0
#define NAU8810_BYPMOUT_SFT		1
<<<<<<< HEAD
=======
#define NAU8810_AUXMOUT_SFT		2
>>>>>>> upstream/android-13
#define NAU8810_MOUTMXMT_SFT		6


/* System Clock Source */
enum {
	NAU8810_SCLK_MCLK,
	NAU8810_SCLK_PLL,
};

struct nau8810_pll {
	int pre_factor;
	int mclk_scaler;
	int pll_frac;
	int pll_int;
};

struct nau8810 {
	struct device *dev;
	struct regmap *regmap;
	struct nau8810_pll pll;
	int sysclk;
	int clk_id;
};

#endif
