/* SPDX-License-Identifier: GPL-2.0 */
/*
<<<<<<< HEAD
 * Copyright (C) 2019 MediaTek Inc.
=======
 * Copyright (C) 2021 MediaTek Inc.
>>>>>>> upstream/android-13
 * Author: Argus Lin <argus.lin@mediatek.com>
 */

#ifndef _ACCDET_H_
#define _ACCDET_H_

#include <linux/ctype.h>
#include <linux/string.h>

#define ACCDET_DEVNAME "accdet"
<<<<<<< HEAD
/* SW ENV define */
#define NO_KEY			(0x0)
#define UP_KEY			(0x01)
#define MD_KEY			(0x02)
#define DW_KEY			(0x04)
#define AS_KEY			(0x08)
=======
>>>>>>> upstream/android-13

#define HEADSET_MODE_1		(1)
#define HEADSET_MODE_2		(2)
#define HEADSET_MODE_6		(6)

<<<<<<< HEAD
/* IOCTL */
#define ACCDET_IOC_MAGIC 'A'
#define ACCDET_INIT _IO(ACCDET_IOC_MAGIC, 0)
#define SET_CALL_STATE _IO(ACCDET_IOC_MAGIC, 1)
#define GET_BUTTON_STATUS _IO(ACCDET_IOC_MAGIC, 2)

/* 400us, Accdet irq clear timeout  */
#define ACCDET_TIME_OUT		0x61A80

/* cable type recognized by accdet, and report to WiredAccessoryManager */
enum accdet_report_state {
	NO_DEVICE =		0,
	HEADSET_MIC =		1,
	HEADSET_NO_MIC =	2,
	HEADSET_FIVE_POLE =	3,
	LINE_OUT_DEVICE =	4,
};

/* accdet status got from accdet FSM  */
enum accdet_status {
	PLUG_OUT =		0,
	MIC_BIAS =		1,
	HOOK_SWITCH =		2,
	BI_MIC_BIAS =		3,
	LINE_OUT =		4,
	STAND_BY =		5
};

enum accdet_eint_ID {
	NO_PMIC_EINT =		0,
	PMIC_EINT0 =		1,
	PMIC_EINT1 =		2,
	PMIC_BIEINT =		3,
};

/* EINT state when moisture enable  */
=======
#define MT6359_ACCDET_NUM_BUTTONS 4
#define MT6359_ACCDET_JACK_MASK (SND_JACK_HEADPHONE | \
				SND_JACK_HEADSET | \
				SND_JACK_BTN_0 | \
				SND_JACK_BTN_1 | \
				SND_JACK_BTN_2 | \
				SND_JACK_BTN_3)
#define MT6359_ACCDET_BTN_MASK (SND_JACK_BTN_0 | \
				SND_JACK_BTN_1 | \
				SND_JACK_BTN_2 | \
				SND_JACK_BTN_3)

>>>>>>> upstream/android-13
enum eint_moisture_status {
	M_PLUG_IN =		0,
	M_WATER_IN =		1,
	M_HP_PLUG_IN =		2,
	M_PLUG_OUT =		3,
	M_NO_ACT =		4,
	M_UNKNOWN =		5,
};

<<<<<<< HEAD
=======
enum {
	accdet_state000 = 0,
	accdet_state001,
	accdet_state010,
	accdet_state011,
	accdet_auxadc,
	eint_state000,
	eint_state001,
	eint_state010,
	eint_state011,
	eint_inverter_state000,
};

>>>>>>> upstream/android-13
struct three_key_threshold {
	unsigned int mid;
	unsigned int up;
	unsigned int down;
};
<<<<<<< HEAD
=======

>>>>>>> upstream/android-13
struct four_key_threshold {
	unsigned int mid;
	unsigned int voice;
	unsigned int up;
	unsigned int down;
};

struct pwm_deb_settings {
	unsigned int pwm_width;
	unsigned int pwm_thresh;
	unsigned int fall_delay;
	unsigned int rise_delay;
	unsigned int debounce0;
	unsigned int debounce1;
	unsigned int debounce3;
	unsigned int debounce4;
	unsigned int eint_pwm_width;
	unsigned int eint_pwm_thresh;
	unsigned int eint_debounce0;
	unsigned int eint_debounce1;
	unsigned int eint_debounce2;
	unsigned int eint_debounce3;
	unsigned int eint_inverter_debounce;

};

<<<<<<< HEAD
struct head_dts_data {
=======
struct dts_data {
>>>>>>> upstream/android-13
	unsigned int mic_vol;
	unsigned int mic_mode;
	unsigned int plugout_deb;
	unsigned int eint_pol;
<<<<<<< HEAD
	struct pwm_deb_settings pwm_deb;
=======
	struct pwm_deb_settings *pwm_deb;
>>>>>>> upstream/android-13
	struct three_key_threshold three_key;
	struct four_key_threshold four_key;
	unsigned int moisture_detect_enable;
	unsigned int eint_detect_mode;
	unsigned int eint_use_ext_res;
<<<<<<< HEAD
=======
	unsigned int eint_comp_vth;
>>>>>>> upstream/android-13
	unsigned int moisture_detect_mode;
	unsigned int moisture_comp_vth;
	unsigned int moisture_comp_vref2;
	unsigned int moisture_use_ext_res;
};

<<<<<<< HEAD
enum {
	accdet_state000 = 0,
	accdet_state001,
	accdet_state010,
	accdet_state011,
	accdet_auxadc,
	eint_state000,
	eint_state001,
	eint_state010,
	eint_state011,
	eint_inverter_state000,
};

/* just be called by audio module for DC trim */
extern void accdet_late_init(unsigned long data);
extern const struct file_operations *accdet_get_fops(void);
extern void mt_accdet_suspend(void);
extern void mt_accdet_resume(void);
extern void accdet_set_debounce(int state, unsigned int debounce);
extern void accdet_modify_vref_volt(void);

=======
struct mt6359_accdet {
	struct snd_soc_jack *jack;
	struct device *dev;
	struct regmap *regmap;
	struct dts_data *data;
	unsigned int caps;
	int accdet_irq;
	int accdet_eint0;
	int accdet_eint1;
	struct mutex res_lock; /* lock protection */
	bool jack_plugged;
	unsigned int jack_type;
	unsigned int btn_type;
	unsigned int accdet_status;
	unsigned int pre_accdet_status;
	unsigned int cali_voltage;
	unsigned int jd_sts;
	struct work_struct accdet_work;
	struct workqueue_struct *accdet_workqueue;
	struct work_struct jd_work;
	struct workqueue_struct *jd_workqueue;
};

int mt6359_accdet_enable_jack_detect(struct snd_soc_component *component,
				     struct snd_soc_jack *jack);
>>>>>>> upstream/android-13
#endif
