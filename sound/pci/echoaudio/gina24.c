<<<<<<< HEAD
/*
 *  ALSA driver for Echoaudio soundcards.
 *  Copyright (C) 2003-2004 Giuliano Pochini <pochini@shiny.it>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; version 2 of the License.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
=======
// SPDX-License-Identifier: GPL-2.0-only
/*
 *  ALSA driver for Echoaudio soundcards.
 *  Copyright (C) 2003-2004 Giuliano Pochini <pochini@shiny.it>
>>>>>>> upstream/android-13
 */

#define ECHO24_FAMILY
#define ECHOCARD_GINA24
#define ECHOCARD_NAME "Gina24"
#define ECHOCARD_HAS_MONITOR
#define ECHOCARD_HAS_ASIC
#define ECHOCARD_HAS_INPUT_NOMINAL_LEVEL
#define ECHOCARD_HAS_OUTPUT_NOMINAL_LEVEL
#define ECHOCARD_HAS_SUPER_INTERLEAVE
#define ECHOCARD_HAS_DIGITAL_IO
#define ECHOCARD_HAS_DIGITAL_IN_AUTOMUTE
#define ECHOCARD_HAS_DIGITAL_MODE_SWITCH
#define ECHOCARD_HAS_EXTERNAL_CLOCK
#define ECHOCARD_HAS_ADAT	6
#define ECHOCARD_HAS_STEREO_BIG_ENDIAN32

/* Pipe indexes */
#define PX_ANALOG_OUT	0	/* 8 */
#define PX_DIGITAL_OUT	8	/* 8 */
#define PX_ANALOG_IN	16	/* 2 */
#define PX_DIGITAL_IN	18	/* 8 */
#define PX_NUM		26

/* Bus indexes */
#define BX_ANALOG_OUT	0	/* 8 */
#define BX_DIGITAL_OUT	8	/* 8 */
#define BX_ANALOG_IN	16	/* 2 */
#define BX_DIGITAL_IN	18	/* 8 */
#define BX_NUM		26


#include <linux/delay.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/pci.h>
#include <linux/module.h>
#include <linux/firmware.h>
#include <linux/slab.h>
#include <linux/io.h>
#include <sound/core.h>
#include <sound/info.h>
#include <sound/control.h>
#include <sound/tlv.h>
#include <sound/pcm.h>
#include <sound/pcm_params.h>
#include <sound/asoundef.h>
#include <sound/initval.h>
#include <linux/atomic.h>
#include "echoaudio.h"

MODULE_FIRMWARE("ea/loader_dsp.fw");
MODULE_FIRMWARE("ea/gina24_301_dsp.fw");
MODULE_FIRMWARE("ea/gina24_361_dsp.fw");
MODULE_FIRMWARE("ea/gina24_301_asic.fw");
MODULE_FIRMWARE("ea/gina24_361_asic.fw");

#define FW_361_LOADER		0
#define FW_GINA24_301_DSP	1
#define FW_GINA24_361_DSP	2
#define FW_GINA24_301_ASIC	3
#define FW_GINA24_361_ASIC	4

static const struct firmware card_fw[] = {
	{0, "loader_dsp.fw"},
	{0, "gina24_301_dsp.fw"},
	{0, "gina24_361_dsp.fw"},
	{0, "gina24_301_asic.fw"},
	{0, "gina24_361_asic.fw"}
};

static const struct pci_device_id snd_echo_ids[] = {
	{0x1057, 0x1801, 0xECC0, 0x0050, 0, 0, 0},	/* DSP 56301 Gina24 rev.0 */
	{0x1057, 0x1801, 0xECC0, 0x0051, 0, 0, 0},	/* DSP 56301 Gina24 rev.1 */
	{0x1057, 0x3410, 0xECC0, 0x0050, 0, 0, 0},	/* DSP 56361 Gina24 rev.0 */
	{0x1057, 0x3410, 0xECC0, 0x0051, 0, 0, 0},	/* DSP 56361 Gina24 rev.1 */
	{0,}
};

<<<<<<< HEAD
static struct snd_pcm_hardware pcm_hardware_skel = {
=======
static const struct snd_pcm_hardware pcm_hardware_skel = {
>>>>>>> upstream/android-13
	.info = SNDRV_PCM_INFO_MMAP |
		SNDRV_PCM_INFO_INTERLEAVED |
		SNDRV_PCM_INFO_BLOCK_TRANSFER |
		SNDRV_PCM_INFO_MMAP_VALID |
		SNDRV_PCM_INFO_PAUSE |
		SNDRV_PCM_INFO_SYNC_START,
	.formats =	SNDRV_PCM_FMTBIT_U8 |
			SNDRV_PCM_FMTBIT_S16_LE |
			SNDRV_PCM_FMTBIT_S24_3LE |
			SNDRV_PCM_FMTBIT_S32_LE |
			SNDRV_PCM_FMTBIT_S32_BE,
	.rates = 	SNDRV_PCM_RATE_8000_48000 |
			SNDRV_PCM_RATE_88200 |
			SNDRV_PCM_RATE_96000,
	.rate_min = 8000,
	.rate_max = 96000,
	.channels_min = 1,
	.channels_max = 8,
	.buffer_bytes_max = 262144,
	.period_bytes_min = 32,
	.period_bytes_max = 131072,
	.periods_min = 2,
	.periods_max = 220,
	/* One page (4k) contains 512 instructions. I don't know if the hw
	supports lists longer than this. In this case periods_max=220 is a
	safe limit to make sure the list never exceeds 512 instructions.
	220 ~= (512 - 1 - (BUFFER_BYTES_MAX / PAGE_SIZE)) / 2 */
};

#include "gina24_dsp.c"
#include "echoaudio_dsp.c"
#include "echoaudio_gml.c"
#include "echoaudio.c"
