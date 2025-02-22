<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> upstream/android-13
/*
 * Copyright Everest Semiconductor Co.,Ltd
 *
 * Author: David Yang <yangxiaohua@everest-semi.com>
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
=======
>>>>>>> upstream/android-13
 */

#ifndef _ES8316_H
#define _ES8316_H

/*
 * ES8316 register space
 */

/* Reset Control */
#define ES8316_RESET		0x00

/* Clock Management */
#define ES8316_CLKMGR_CLKSW	0x01
#define ES8316_CLKMGR_CLKSEL	0x02
#define ES8316_CLKMGR_ADCOSR	0x03
#define ES8316_CLKMGR_ADCDIV1	0x04
#define ES8316_CLKMGR_ADCDIV2	0x05
#define ES8316_CLKMGR_DACDIV1	0x06
#define ES8316_CLKMGR_DACDIV2	0x07
#define ES8316_CLKMGR_CPDIV	0x08

/* Serial Data Port Control */
#define ES8316_SERDATA1		0x09
#define ES8316_SERDATA_ADC	0x0a
#define ES8316_SERDATA_DAC	0x0b

/* System Control */
#define ES8316_SYS_VMIDSEL	0x0c
#define ES8316_SYS_PDN		0x0d
#define ES8316_SYS_LP1		0x0e
#define ES8316_SYS_LP2		0x0f
#define ES8316_SYS_VMIDLOW	0x10
#define ES8316_SYS_VSEL		0x11
#define ES8316_SYS_REF		0x12

/* Headphone Mixer */
#define ES8316_HPMIX_SEL	0x13
#define ES8316_HPMIX_SWITCH	0x14
#define ES8316_HPMIX_PDN	0x15
#define ES8316_HPMIX_VOL	0x16

/* Charge Pump Headphone driver */
#define ES8316_CPHP_OUTEN	0x17
#define ES8316_CPHP_ICAL_VOL	0x18
#define ES8316_CPHP_PDN1	0x19
#define ES8316_CPHP_PDN2	0x1a
#define ES8316_CPHP_LDOCTL	0x1b

/* Calibration */
#define ES8316_CAL_TYPE		0x1c
#define ES8316_CAL_SET		0x1d
#define ES8316_CAL_HPLIV	0x1e
#define ES8316_CAL_HPRIV	0x1f
#define ES8316_CAL_HPLMV	0x20
#define ES8316_CAL_HPRMV	0x21

/* ADC Control */
#define ES8316_ADC_PDN_LINSEL	0x22
#define ES8316_ADC_PGAGAIN	0x23
#define ES8316_ADC_D2SEPGA	0x24
#define ES8316_ADC_DMIC		0x25
#define ES8316_ADC_MUTE		0x26
#define ES8316_ADC_VOLUME	0x27
#define ES8316_ADC_ALC1		0x29
#define ES8316_ADC_ALC2		0x2a
#define ES8316_ADC_ALC3		0x2b
#define ES8316_ADC_ALC4		0x2c
#define ES8316_ADC_ALC5		0x2d
#define ES8316_ADC_ALC_NG	0x2e

/* DAC Control */
#define ES8316_DAC_PDN		0x2f
#define ES8316_DAC_SET1		0x30
#define ES8316_DAC_SET2		0x31
#define ES8316_DAC_SET3		0x32
#define ES8316_DAC_VOLL		0x33
#define ES8316_DAC_VOLR		0x34

/* GPIO */
#define ES8316_GPIO_SEL		0x4d
#define ES8316_GPIO_DEBOUNCE	0x4e
#define ES8316_GPIO_FLAG	0x4f

/* Test mode */
#define ES8316_TESTMODE		0x50
#define ES8316_TEST1		0x51
#define ES8316_TEST2		0x52
#define ES8316_TEST3		0x53

/*
 * Field definitions
 */

/* ES8316_RESET */
#define ES8316_RESET_CSM_ON		0x80

/* ES8316_CLKMGR_CLKSW */
#define ES8316_CLKMGR_CLKSW_MCLK_ON	0x40
#define ES8316_CLKMGR_CLKSW_BCLK_ON	0x20

/* ES8316_SERDATA1 */
#define ES8316_SERDATA1_MASTER		0x80
#define ES8316_SERDATA1_BCLK_INV	0x20

/* ES8316_SERDATA_ADC and _DAC */
#define ES8316_SERDATA2_FMT_MASK	0x3
#define ES8316_SERDATA2_FMT_I2S		0x00
#define ES8316_SERDATA2_FMT_LEFTJ	0x01
#define ES8316_SERDATA2_FMT_RIGHTJ	0x02
#define ES8316_SERDATA2_FMT_PCM		0x03
#define ES8316_SERDATA2_ADCLRP		0x20
#define ES8316_SERDATA2_LEN_MASK	0x1c
#define ES8316_SERDATA2_LEN_24		0x00
#define ES8316_SERDATA2_LEN_20		0x04
#define ES8316_SERDATA2_LEN_18		0x08
#define ES8316_SERDATA2_LEN_16		0x0c
#define ES8316_SERDATA2_LEN_32		0x10

<<<<<<< HEAD
=======
/* ES8316_GPIO_DEBOUNCE	*/
#define ES8316_GPIO_ENABLE_INTERRUPT		0x02

/* ES8316_GPIO_FLAG */
#define ES8316_GPIO_FLAG_GM_NOT_SHORTED		0x02
#define ES8316_GPIO_FLAG_HP_NOT_INSERTED	0x04

>>>>>>> upstream/android-13
#endif
