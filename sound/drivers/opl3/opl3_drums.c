<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> upstream/android-13
/*
 *  Copyright (c) by Uros Bizjak <uros@kss-loka.si>
 *
 *   OPL2/OPL3/OPL4 FM routines for internal percussion channels
<<<<<<< HEAD
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
 *
=======
>>>>>>> upstream/android-13
 */

#include "opl3_voice.h"

<<<<<<< HEAD
static char snd_opl3_drum_table[47] =
=======
static const char snd_opl3_drum_table[47] =
>>>>>>> upstream/android-13
{
	OPL3_BASSDRUM_ON,  OPL3_BASSDRUM_ON,  OPL3_HIHAT_ON,	/* 35 - 37 */
	OPL3_SNAREDRUM_ON, OPL3_HIHAT_ON,     OPL3_SNAREDRUM_ON, /* 38 - 40 */
	OPL3_BASSDRUM_ON,  OPL3_HIHAT_ON,     OPL3_BASSDRUM_ON,	/* 41 - 43 */
	OPL3_HIHAT_ON,     OPL3_TOMTOM_ON,    OPL3_HIHAT_ON,	/* 44 - 46 */
	OPL3_TOMTOM_ON,    OPL3_TOMTOM_ON,    OPL3_CYMBAL_ON,	/* 47 - 49 */

	OPL3_TOMTOM_ON,    OPL3_CYMBAL_ON,    OPL3_CYMBAL_ON,	/* 50 - 52 */
	OPL3_CYMBAL_ON,    OPL3_CYMBAL_ON,    OPL3_CYMBAL_ON,	/* 53 - 55 */
	OPL3_HIHAT_ON,     OPL3_CYMBAL_ON,    OPL3_TOMTOM_ON,	/* 56 - 58 */
	OPL3_CYMBAL_ON,    OPL3_TOMTOM_ON,    OPL3_TOMTOM_ON,	/* 59 - 61 */
	OPL3_HIHAT_ON,     OPL3_TOMTOM_ON,    OPL3_TOMTOM_ON,	/* 62 - 64 */

	OPL3_TOMTOM_ON,    OPL3_TOMTOM_ON,    OPL3_TOMTOM_ON,	/* 65 - 67 */
	OPL3_TOMTOM_ON,    OPL3_HIHAT_ON,     OPL3_HIHAT_ON,	/* 68 - 70 */
	OPL3_HIHAT_ON,     OPL3_HIHAT_ON,     OPL3_TOMTOM_ON,	/* 71 - 73 */
	OPL3_TOMTOM_ON,    OPL3_TOMTOM_ON,    OPL3_TOMTOM_ON,	/* 74 - 76 */
	OPL3_TOMTOM_ON,    OPL3_TOMTOM_ON,    OPL3_TOMTOM_ON,	/* 77 - 79 */
	OPL3_CYMBAL_ON,    OPL3_CYMBAL_ON			/* 80 - 81 */
};

struct snd_opl3_drum_voice {
	int voice;
	int op;
	unsigned char am_vib;
	unsigned char ksl_level;
	unsigned char attack_decay;
	unsigned char sustain_release;
	unsigned char feedback_connection;
	unsigned char wave_select;
};

struct snd_opl3_drum_note {
	int voice;
	unsigned char fnum;
	unsigned char octave_f;
	unsigned char feedback_connection;
};

<<<<<<< HEAD
static struct snd_opl3_drum_voice bass_op0 = {6, 0, 0x00, 0x32, 0xf8, 0x66, 0x30, 0x00};
static struct snd_opl3_drum_voice bass_op1 = {6, 1, 0x00, 0x03, 0xf6, 0x57, 0x30, 0x00};
static struct snd_opl3_drum_note bass_note = {6, 0x90, 0x09};

static struct snd_opl3_drum_voice hihat = {7, 0, 0x00, 0x03, 0xf0, 0x06, 0x20, 0x00};

static struct snd_opl3_drum_voice snare = {7, 1, 0x00, 0x03, 0xf0, 0x07, 0x20, 0x02};
static struct snd_opl3_drum_note snare_note = {7, 0xf4, 0x0d};

static struct snd_opl3_drum_voice tomtom = {8, 0, 0x02, 0x03, 0xf0, 0x06, 0x10, 0x00};
static struct snd_opl3_drum_note tomtom_note = {8, 0xf4, 0x09};

static struct snd_opl3_drum_voice cymbal = {8, 1, 0x04, 0x03, 0xf0, 0x06, 0x10, 0x00};
=======
static const struct snd_opl3_drum_voice bass_op0 = {6, 0, 0x00, 0x32, 0xf8, 0x66, 0x30, 0x00};
static const struct snd_opl3_drum_voice bass_op1 = {6, 1, 0x00, 0x03, 0xf6, 0x57, 0x30, 0x00};
static const struct snd_opl3_drum_note bass_note = {6, 0x90, 0x09};

static const struct snd_opl3_drum_voice hihat = {7, 0, 0x00, 0x03, 0xf0, 0x06, 0x20, 0x00};

static const struct snd_opl3_drum_voice snare = {7, 1, 0x00, 0x03, 0xf0, 0x07, 0x20, 0x02};
static const struct snd_opl3_drum_note snare_note = {7, 0xf4, 0x0d};

static const struct snd_opl3_drum_voice tomtom = {8, 0, 0x02, 0x03, 0xf0, 0x06, 0x10, 0x00};
static const struct snd_opl3_drum_note tomtom_note = {8, 0xf4, 0x09};

static const struct snd_opl3_drum_voice cymbal = {8, 1, 0x04, 0x03, 0xf0, 0x06, 0x10, 0x00};
>>>>>>> upstream/android-13

/*
 * set drum voice characteristics
 */
static void snd_opl3_drum_voice_set(struct snd_opl3 *opl3,
<<<<<<< HEAD
				    struct snd_opl3_drum_voice *data)
=======
				    const struct snd_opl3_drum_voice *data)
>>>>>>> upstream/android-13
{
	unsigned char op_offset = snd_opl3_regmap[data->voice][data->op];
	unsigned char voice_offset = data->voice;
	unsigned short opl3_reg;
	
	/* Set OPL3 AM_VIB register */ 
	opl3_reg = OPL3_LEFT | (OPL3_REG_AM_VIB + op_offset);
	opl3->command(opl3, opl3_reg, data->am_vib);

	/* Set OPL3 KSL_LEVEL register */ 
	opl3_reg = OPL3_LEFT | (OPL3_REG_KSL_LEVEL + op_offset);
	opl3->command(opl3, opl3_reg, data->ksl_level);

	/* Set OPL3 ATTACK_DECAY register */ 
	opl3_reg = OPL3_LEFT | (OPL3_REG_ATTACK_DECAY + op_offset);
	opl3->command(opl3, opl3_reg, data->attack_decay);

	/* Set OPL3 SUSTAIN_RELEASE register */ 
	opl3_reg = OPL3_LEFT | (OPL3_REG_SUSTAIN_RELEASE + op_offset);
	opl3->command(opl3, opl3_reg, data->sustain_release);

	/* Set OPL3 FEEDBACK_CONNECTION register */ 
	opl3_reg = OPL3_LEFT | (OPL3_REG_FEEDBACK_CONNECTION + voice_offset);
	opl3->command(opl3, opl3_reg, data->feedback_connection);

	/* Select waveform */
	opl3_reg = OPL3_LEFT | (OPL3_REG_WAVE_SELECT + op_offset);
	opl3->command(opl3, opl3_reg, data->wave_select);
}

/*
 * Set drum voice pitch
 */
static void snd_opl3_drum_note_set(struct snd_opl3 *opl3,
<<<<<<< HEAD
				   struct snd_opl3_drum_note *data)
=======
				   const struct snd_opl3_drum_note *data)
>>>>>>> upstream/android-13
{
	unsigned char voice_offset = data->voice;
	unsigned short opl3_reg;

	/* Set OPL3 FNUM_LOW register */ 
	opl3_reg = OPL3_LEFT | (OPL3_REG_FNUM_LOW + voice_offset);
	opl3->command(opl3, opl3_reg, data->fnum);

	/* Set OPL3 KEYON_BLOCK register */ 
	opl3_reg = OPL3_LEFT | (OPL3_REG_KEYON_BLOCK + voice_offset);
	opl3->command(opl3, opl3_reg, data->octave_f);
}

/*
 * Set drum voice volume and position
 */
static void snd_opl3_drum_vol_set(struct snd_opl3 *opl3,
<<<<<<< HEAD
				  struct snd_opl3_drum_voice *data,
=======
				  const struct snd_opl3_drum_voice *data,
>>>>>>> upstream/android-13
				  int vel, struct snd_midi_channel *chan)
{
	unsigned char op_offset = snd_opl3_regmap[data->voice][data->op];
	unsigned char voice_offset = data->voice;
	unsigned char reg_val;
	unsigned short opl3_reg;

	/* Set OPL3 KSL_LEVEL register */ 
	reg_val = data->ksl_level;
	snd_opl3_calc_volume(&reg_val, vel, chan);
	opl3_reg = OPL3_LEFT | (OPL3_REG_KSL_LEVEL + op_offset);
	opl3->command(opl3, opl3_reg, reg_val);

	/* Set OPL3 FEEDBACK_CONNECTION register */ 
	/* Set output voice connection */
	reg_val = data->feedback_connection | OPL3_STEREO_BITS;
	if (chan->gm_pan < 43)
		reg_val &= ~OPL3_VOICE_TO_RIGHT;
	if (chan->gm_pan > 85)
		reg_val &= ~OPL3_VOICE_TO_LEFT;
	opl3_reg = OPL3_LEFT | (OPL3_REG_FEEDBACK_CONNECTION + voice_offset);
	opl3->command(opl3, opl3_reg, reg_val);
}

/*
 * Loads drum voices at init time
 */
void snd_opl3_load_drums(struct snd_opl3 *opl3)
{
	snd_opl3_drum_voice_set(opl3, &bass_op0);
	snd_opl3_drum_voice_set(opl3, &bass_op1);
	snd_opl3_drum_note_set(opl3, &bass_note);

	snd_opl3_drum_voice_set(opl3, &hihat);

	snd_opl3_drum_voice_set(opl3, &snare);
	snd_opl3_drum_note_set(opl3, &snare_note);

	snd_opl3_drum_voice_set(opl3, &tomtom);
	snd_opl3_drum_note_set(opl3, &tomtom_note);

	snd_opl3_drum_voice_set(opl3, &cymbal);
}

/*
 * Switch drum voice on or off
 */
void snd_opl3_drum_switch(struct snd_opl3 *opl3, int note, int vel, int on_off,
			  struct snd_midi_channel *chan)
{
	unsigned char drum_mask;
<<<<<<< HEAD
	struct snd_opl3_drum_voice *drum_voice;
=======
	const struct snd_opl3_drum_voice *drum_voice;
>>>>>>> upstream/android-13

	if (!(opl3->drum_reg & OPL3_PERCUSSION_ENABLE))
		return;

	if ((note < 35) || (note > 81))
		return;
	drum_mask = snd_opl3_drum_table[note - 35];

	if (on_off) {
		switch (drum_mask) {
		case OPL3_BASSDRUM_ON:
			drum_voice = &bass_op1;
			break;
		case OPL3_HIHAT_ON:
			drum_voice = &hihat;
			break;
		case OPL3_SNAREDRUM_ON:
			drum_voice = &snare;
			break;
		case OPL3_TOMTOM_ON:
			drum_voice = &tomtom;
			break;
		case OPL3_CYMBAL_ON:
			drum_voice = &cymbal;
			break;
		default:
			drum_voice = &tomtom;
		}

		snd_opl3_drum_vol_set(opl3, drum_voice, vel, chan);
		opl3->drum_reg |= drum_mask;
	} else {
		opl3->drum_reg &= ~drum_mask;
	}
	opl3->command(opl3, OPL3_LEFT | OPL3_REG_PERCUSSION,
			 opl3->drum_reg);
}
