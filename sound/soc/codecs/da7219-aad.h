<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-or-later */
>>>>>>> upstream/android-13
/*
 * da7219-aad.h - DA7322 ASoC AAD Driver
 *
 * Copyright (c) 2015 Dialog Semiconductor Ltd.
 *
 * Author: Adam Thomson <Adam.Thomson.Opensource@diasemi.com>
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General  Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
=======
>>>>>>> upstream/android-13
 */

#ifndef __DA7219_AAD_H
#define __DA7219_AAD_H

#include <linux/timer.h>
#include <sound/soc.h>
#include <sound/jack.h>
#include <sound/da7219-aad.h>

/*
 * Registers
 */

#define DA7219_ACCDET_STATUS_A		0xC0
#define DA7219_ACCDET_STATUS_B		0xC1
#define DA7219_ACCDET_IRQ_EVENT_A	0xC2
#define DA7219_ACCDET_IRQ_EVENT_B	0xC3
#define DA7219_ACCDET_IRQ_MASK_A	0xC4
#define DA7219_ACCDET_IRQ_MASK_B	0xC5
#define DA7219_ACCDET_CONFIG_1		0xC6
#define DA7219_ACCDET_CONFIG_2		0xC7
#define DA7219_ACCDET_CONFIG_3		0xC8
#define DA7219_ACCDET_CONFIG_4		0xC9
#define DA7219_ACCDET_CONFIG_5		0xCA
#define DA7219_ACCDET_CONFIG_6		0xCB
#define DA7219_ACCDET_CONFIG_7		0xCC
#define DA7219_ACCDET_CONFIG_8		0xCD


/*
 * Bit Fields
 */

/* DA7219_ACCDET_STATUS_A = 0xC0 */
#define DA7219_JACK_INSERTION_STS_SHIFT	0
#define DA7219_JACK_INSERTION_STS_MASK	(0x1 << 0)
#define DA7219_JACK_TYPE_STS_SHIFT	1
#define DA7219_JACK_TYPE_STS_MASK	(0x1 << 1)
#define DA7219_JACK_PIN_ORDER_STS_SHIFT	2
#define DA7219_JACK_PIN_ORDER_STS_MASK	(0x1 << 2)
#define DA7219_MICBIAS_UP_STS_SHIFT	3
#define DA7219_MICBIAS_UP_STS_MASK	(0x1 << 3)

/* DA7219_ACCDET_STATUS_B = 0xC1 */
#define DA7219_BUTTON_TYPE_STS_SHIFT	0
#define DA7219_BUTTON_TYPE_STS_MASK	(0xFF << 0)

/* DA7219_ACCDET_IRQ_EVENT_A = 0xC2 */
#define DA7219_E_JACK_INSERTED_SHIFT		0
#define DA7219_E_JACK_INSERTED_MASK		(0x1 << 0)
#define DA7219_E_JACK_REMOVED_SHIFT		1
#define DA7219_E_JACK_REMOVED_MASK		(0x1 << 1)
#define DA7219_E_JACK_DETECT_COMPLETE_SHIFT	2
#define DA7219_E_JACK_DETECT_COMPLETE_MASK	(0x1 << 2)

/* DA7219_ACCDET_IRQ_EVENT_B = 0xC3 */
#define DA7219_E_BUTTON_A_PRESSED_SHIFT		0
#define DA7219_E_BUTTON_A_PRESSED_MASK		(0x1 << 0)
#define DA7219_E_BUTTON_B_PRESSED_SHIFT		1
#define DA7219_E_BUTTON_B_PRESSED_MASK		(0x1 << 1)
#define DA7219_E_BUTTON_C_PRESSED_SHIFT		2
#define DA7219_E_BUTTON_C_PRESSED_MASK		(0x1 << 2)
#define DA7219_E_BUTTON_D_PRESSED_SHIFT		3
#define DA7219_E_BUTTON_D_PRESSED_MASK		(0x1 << 3)
#define DA7219_E_BUTTON_D_RELEASED_SHIFT	4
#define DA7219_E_BUTTON_D_RELEASED_MASK		(0x1 << 4)
#define DA7219_E_BUTTON_C_RELEASED_SHIFT	5
#define DA7219_E_BUTTON_C_RELEASED_MASK		(0x1 << 5)
#define DA7219_E_BUTTON_B_RELEASED_SHIFT	6
#define DA7219_E_BUTTON_B_RELEASED_MASK		(0x1 << 6)
#define DA7219_E_BUTTON_A_RELEASED_SHIFT	7
#define DA7219_E_BUTTON_A_RELEASED_MASK		(0x1 << 7)

/* DA7219_ACCDET_IRQ_MASK_A = 0xC4 */
#define DA7219_M_JACK_INSERTED_SHIFT		0
#define DA7219_M_JACK_INSERTED_MASK		(0x1 << 0)
#define DA7219_M_JACK_REMOVED_SHIFT		1
#define DA7219_M_JACK_REMOVED_MASK		(0x1 << 1)
#define DA7219_M_JACK_DETECT_COMPLETE_SHIFT	2
#define DA7219_M_JACK_DETECT_COMPLETE_MASK	(0x1 << 2)

/* DA7219_ACCDET_IRQ_MASK_B = 0xC5 */
#define DA7219_M_BUTTON_A_PRESSED_SHIFT		0
#define DA7219_M_BUTTON_A_PRESSED_MASK		(0x1 << 0)
#define DA7219_M_BUTTON_B_PRESSED_SHIFT		1
#define DA7219_M_BUTTON_B_PRESSED_MASK		(0x1 << 1)
#define DA7219_M_BUTTON_C_PRESSED_SHIFT		2
#define DA7219_M_BUTTON_C_PRESSED_MASK		(0x1 << 2)
#define DA7219_M_BUTTON_D_PRESSED_SHIFT		3
#define DA7219_M_BUTTON_D_PRESSED_MASK		(0x1 << 3)
#define DA7219_M_BUTTON_D_RELEASED_SHIFT	4
#define DA7219_M_BUTTON_D_RELEASED_MASK		(0x1 << 4)
#define DA7219_M_BUTTON_C_RELEASED_SHIFT	5
#define DA7219_M_BUTTON_C_RELEASED_MASK		(0x1 << 5)
#define DA7219_M_BUTTON_B_RELEASED_SHIFT	6
#define DA7219_M_BUTTON_B_RELEASED_MASK		(0x1 << 6)
#define DA7219_M_BUTTON_A_RELEASED_SHIFT	7
#define DA7219_M_BUTTON_A_RELEASED_MASK		(0x1 << 7)

/* DA7219_ACCDET_CONFIG_1 = 0xC6 */
#define DA7219_ACCDET_EN_SHIFT		0
#define DA7219_ACCDET_EN_MASK		(0x1 << 0)
#define DA7219_BUTTON_CONFIG_SHIFT	1
#define DA7219_BUTTON_CONFIG_MASK	(0x7 << 1)
#define DA7219_MIC_DET_THRESH_SHIFT	4
#define DA7219_MIC_DET_THRESH_MASK	(0x3 << 4)
#define DA7219_JACK_TYPE_DET_EN_SHIFT	6
#define DA7219_JACK_TYPE_DET_EN_MASK	(0x1 << 6)
#define DA7219_PIN_ORDER_DET_EN_SHIFT	7
#define DA7219_PIN_ORDER_DET_EN_MASK	(0x1 << 7)

/* DA7219_ACCDET_CONFIG_2 = 0xC7 */
#define DA7219_ACCDET_PAUSE_SHIFT	0
#define DA7219_ACCDET_PAUSE_MASK	(0x1 << 0)
#define DA7219_JACKDET_DEBOUNCE_SHIFT	1
#define DA7219_JACKDET_DEBOUNCE_MASK	(0x7 << 1)
#define DA7219_JACK_DETECT_RATE_SHIFT	4
#define DA7219_JACK_DETECT_RATE_MASK	(0x3 << 4)
#define DA7219_JACKDET_REM_DEB_SHIFT	6
#define DA7219_JACKDET_REM_DEB_MASK	(0x3 << 6)

/* DA7219_ACCDET_CONFIG_3 = 0xC8 */
#define DA7219_A_D_BUTTON_THRESH_SHIFT	0
#define DA7219_A_D_BUTTON_THRESH_MASK	(0xFF << 0)

/* DA7219_ACCDET_CONFIG_4 = 0xC9 */
#define DA7219_D_B_BUTTON_THRESH_SHIFT	0
#define DA7219_D_B_BUTTON_THRESH_MASK	(0xFF << 0)

/* DA7219_ACCDET_CONFIG_5 = 0xCA */
#define DA7219_B_C_BUTTON_THRESH_SHIFT	0
#define DA7219_B_C_BUTTON_THRESH_MASK	(0xFF << 0)

/* DA7219_ACCDET_CONFIG_6 = 0xCB */
#define DA7219_C_MIC_BUTTON_THRESH_SHIFT	0
#define DA7219_C_MIC_BUTTON_THRESH_MASK		(0xFF << 0)

/* DA7219_ACCDET_CONFIG_7 = 0xCC */
#define DA7219_BUTTON_AVERAGE_SHIFT	0
#define DA7219_BUTTON_AVERAGE_MASK	(0x3 << 0)
#define DA7219_ADC_1_BIT_REPEAT_SHIFT	2
#define DA7219_ADC_1_BIT_REPEAT_MASK	(0x3 << 2)
#define DA7219_PIN_ORDER_FORCE_SHIFT	4
#define DA7219_PIN_ORDER_FORCE_MASK	(0x1 << 4)
#define DA7219_JACK_TYPE_FORCE_SHIFT	5
#define DA7219_JACK_TYPE_FORCE_MASK	(0x1 << 5)

/* DA7219_ACCDET_CONFIG_8 = 0xCD */
#define DA7219_HPTEST_EN_SHIFT		0
#define DA7219_HPTEST_EN_MASK		(0x1 << 0)
#define DA7219_HPTEST_RES_SEL_SHIFT	1
#define DA7219_HPTEST_RES_SEL_MASK	(0x3 << 1)
#define DA7219_HPTEST_RES_SEL_1KOHMS	(0x0 << 1)
#define DA7219_HPTEST_COMP_SHIFT	4
#define DA7219_HPTEST_COMP_MASK		(0x1 << 4)


#define DA7219_AAD_MAX_BUTTONS		4
#define DA7219_AAD_REPORT_ALL_MASK	(SND_JACK_MECHANICAL |			\
					 SND_JACK_HEADSET | SND_JACK_LINEOUT |	\
					 SND_JACK_BTN_0 | SND_JACK_BTN_1 |	\
					 SND_JACK_BTN_2 | SND_JACK_BTN_3)

#define DA7219_AAD_MICBIAS_CHK_DELAY	10
#define DA7219_AAD_MICBIAS_CHK_RETRIES	5

#define DA7219_AAD_HPTEST_RAMP_FREQ		0x28
#define DA7219_AAD_HPTEST_RAMP_FREQ_INT_OSC	0x4D
#define DA7219_AAD_HPTEST_PERIOD		65
#define DA7219_AAD_HPTEST_INT_OSC_PATH_DELAY	20

enum da7219_aad_event_regs {
	DA7219_AAD_IRQ_REG_A = 0,
	DA7219_AAD_IRQ_REG_B,
	DA7219_AAD_IRQ_REG_MAX,
};

/* Private data */
struct da7219_aad_priv {
	struct snd_soc_component *component;
	int irq;

	u8 micbias_pulse_lvl;
	u32 micbias_pulse_time;

	u8 btn_cfg;

	struct work_struct btn_det_work;
	struct work_struct hptest_work;

	struct snd_soc_jack *jack;
	bool micbias_resume_enable;
	bool jack_inserted;
};

/* AAD control */
void da7219_aad_jack_det(struct snd_soc_component *component, struct snd_soc_jack *jack);

/* Suspend/Resume */
void da7219_aad_suspend(struct snd_soc_component *component);
void da7219_aad_resume(struct snd_soc_component *component);

/* Init/Exit */
int da7219_aad_init(struct snd_soc_component *component);
void da7219_aad_exit(struct snd_soc_component *component);

<<<<<<< HEAD
=======
/* I2C Probe */
int da7219_aad_probe(struct i2c_client *i2c);

>>>>>>> upstream/android-13
#endif /* __DA7219_AAD_H */
