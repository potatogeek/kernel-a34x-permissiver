<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> upstream/android-13
/*
 * Driver for Digigram VX soundcards
 *
 * DSP commands
 *
 * Copyright (c) 2002 by Takashi Iwai <tiwai@suse.de>
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
=======
>>>>>>> upstream/android-13
 */

#include <sound/core.h>
#include <sound/pcm.h>
#include <sound/vx_core.h>
#include "vx_cmd.h"

/*
 * Array of DSP commands
 */
<<<<<<< HEAD
static struct vx_cmd_info vx_dsp_cmds[] = {
=======
static const struct vx_cmd_info vx_dsp_cmds[] = {
>>>>>>> upstream/android-13
[CMD_VERSION] =			{ 0x010000, 2, RMH_SSIZE_FIXED, 1 },
[CMD_SUPPORTED] =		{ 0x020000, 1, RMH_SSIZE_FIXED, 2 },
[CMD_TEST_IT] =			{ 0x040000, 1, RMH_SSIZE_FIXED, 1 },
[CMD_SEND_IRQA] =		{ 0x070001, 1, RMH_SSIZE_FIXED, 0 },
[CMD_IBL] =			{ 0x080000, 1, RMH_SSIZE_FIXED, 4 },
[CMD_ASYNC] =			{ 0x0A0000, 1, RMH_SSIZE_ARG, 0 },
[CMD_RES_PIPE] =		{ 0x400000, 1, RMH_SSIZE_FIXED, 0 },
[CMD_FREE_PIPE] =		{ 0x410000, 1, RMH_SSIZE_FIXED, 0 },
[CMD_CONF_PIPE] =		{ 0x42A101, 2, RMH_SSIZE_FIXED, 0 },
[CMD_ABORT_CONF_PIPE] =		{ 0x42A100, 2, RMH_SSIZE_FIXED, 0 },
[CMD_PARAM_OUTPUT_PIPE] =	{ 0x43A000, 2, RMH_SSIZE_FIXED, 0 },
[CMD_STOP_PIPE] =		{ 0x470004, 1, RMH_SSIZE_FIXED, 0 },
[CMD_PIPE_STATE] =		{ 0x480000, 1, RMH_SSIZE_FIXED, 1 },
[CMD_PIPE_SPL_COUNT] =		{ 0x49A000, 2, RMH_SSIZE_FIXED, 2 },
[CMD_CAN_START_PIPE] =		{ 0x4b0000, 1, RMH_SSIZE_FIXED, 1 },
[CMD_SIZE_HBUFFER] =		{ 0x4C0000, 1, RMH_SSIZE_FIXED, 1 },
[CMD_START_STREAM] =		{ 0x80A000, 2, RMH_SSIZE_FIXED, 0 },
[CMD_START_ONE_STREAM] =	{ 0x800000, 1, RMH_SSIZE_FIXED, 0 },
[CMD_PAUSE_STREAM] =		{ 0x81A000, 2, RMH_SSIZE_FIXED, 0 },
[CMD_PAUSE_ONE_STREAM] =	{ 0x810000, 1, RMH_SSIZE_FIXED, 0 },
[CMD_STREAM_OUT_LEVEL_ADJUST] =	{ 0x828000, 2, RMH_SSIZE_FIXED, 0 },
[CMD_STOP_STREAM] =		{ 0x830000, 1, RMH_SSIZE_FIXED, 0 },
[CMD_FORMAT_STREAM_OUT] =	{ 0x868000, 1, RMH_SSIZE_FIXED, 0 },
[CMD_FORMAT_STREAM_IN] =	{ 0x878800, 1, RMH_SSIZE_FIXED, 0 },
[CMD_GET_STREAM_STATE] =	{ 0x890001, 2, RMH_SSIZE_FIXED, 1 },
[CMD_DROP_BYTES_AWAY] =		{ 0x8A8000, 2, RMH_SSIZE_FIXED, 0 },
[CMD_GET_REMAINING_BYTES] =	{ 0x8D0800, 1, RMH_SSIZE_FIXED, 2 },
[CMD_CONNECT_AUDIO] =		{ 0xC10000, 1, RMH_SSIZE_FIXED, 0 },
[CMD_AUDIO_LEVEL_ADJUST] =	{ 0xC2A000, 3, RMH_SSIZE_FIXED, 0 },
[CMD_AUDIO_VU_PIC_METER] =	{ 0xC3A003, 2, RMH_SSIZE_FIXED, 1 },
[CMD_GET_AUDIO_LEVELS] =	{ 0xC4A000, 2, RMH_SSIZE_FIXED, 0 },
[CMD_GET_NOTIFY_EVENT] =	{ 0x4D0000, 1, RMH_SSIZE_ARG, 0 },
[CMD_INFO_NOTIFIED] =		{ 0x0B0000, 1, RMH_SSIZE_FIXED, 2 },
[CMD_ACCESS_IO_FCT] =		{ 0x098000, 1, RMH_SSIZE_ARG, 0 },
[CMD_STATUS_R_BUFFERS] =	{ 0x440000, 1, RMH_SSIZE_ARG, 0 },
[CMD_UPDATE_R_BUFFERS] =	{ 0x848000, 4, RMH_SSIZE_FIXED, 0 },
[CMD_LOAD_EFFECT_CONTEXT] =	{ 0x0c8000, 3, RMH_SSIZE_FIXED, 1 },
[CMD_EFFECT_ONE_PIPE] =		{ 0x458000, 0, RMH_SSIZE_FIXED, 0 },
[CMD_MODIFY_CLOCK] =		{ 0x0d0000, 1, RMH_SSIZE_FIXED, 0 },
[CMD_STREAM1_OUT_SET_N_LEVELS] ={ 0x858000, 3, RMH_SSIZE_FIXED, 0 },
[CMD_PURGE_STREAM_DCMDS] =	{ 0x8b8000, 3, RMH_SSIZE_FIXED, 0 },
[CMD_NOTIFY_PIPE_TIME] =	{ 0x4e0000, 1, RMH_SSIZE_FIXED, 0 },
[CMD_LOAD_EFFECT_CONTEXT_PACKET] = { 0x0c8000, 1, RMH_SSIZE_FIXED, 0 },
[CMD_RELIC_R_BUFFER] =		{ 0x8e0800, 1, RMH_SSIZE_FIXED, 1 },
[CMD_RESYNC_AUDIO_INPUTS] =	{ 0x0e0000, 1, RMH_SSIZE_FIXED, 0 },
[CMD_NOTIFY_STREAM_TIME] =	{ 0x8f0000, 1, RMH_SSIZE_FIXED, 0 },
[CMD_STREAM_SAMPLE_COUNT] =	{ 0x900000, 1, RMH_SSIZE_FIXED, 2 },
[CMD_CONFIG_TIME_CODE] =	{ 0x050000, 2, RMH_SSIZE_FIXED, 0 },
[CMD_GET_TIME_CODE] =		{ 0x060000, 1, RMH_SSIZE_FIXED, 5 },
[CMD_MANAGE_SIGNAL] =		{ 0x0f0000, 1, RMH_SSIZE_FIXED, 0 },
[CMD_PARAMETER_STREAM_OUT] =	{ 0x91A000, 3, RMH_SSIZE_FIXED, 0 },
[CMD_READ_BOARD_FREQ] =		{ 0x030000, 1, RMH_SSIZE_FIXED, 2 },
[CMD_GET_STREAM_LEVELS] =	{ 0x8c0000, 1, RMH_SSIZE_FIXED, 3 },
[CMD_PURGE_PIPE_DCMDS] =	{ 0x4f8000, 3, RMH_SSIZE_FIXED, 0 },
// [CMD_SET_STREAM_OUT_EFFECTS] =	{ 0x888000, 34, RMH_SSIZE_FIXED, 0 },
// [CMD_GET_STREAM_OUT_EFFECTS] =	{ 0x928000, 2, RMH_SSIZE_FIXED, 32 },
[CMD_CONNECT_MONITORING] =	{ 0xC00000, 1, RMH_SSIZE_FIXED, 0 },
[CMD_STREAM2_OUT_SET_N_LEVELS] = { 0x938000, 3, RMH_SSIZE_FIXED, 0 },
[CMD_CANCEL_R_BUFFERS] =	{ 0x948000, 4, RMH_SSIZE_FIXED, 0 },
[CMD_NOTIFY_END_OF_BUFFER] =	{ 0x950000, 1, RMH_SSIZE_FIXED, 0 },
[CMD_GET_STREAM_VU_METER] =	{ 0x95A000, 2, RMH_SSIZE_ARG, 0 },
};

/**
 * vx_init_rmh - initialize the RMH instance
 * @rmh: the rmh pointer to be initialized
 * @cmd: the rmh command to be set
 */
void vx_init_rmh(struct vx_rmh *rmh, unsigned int cmd)
{
	if (snd_BUG_ON(cmd >= CMD_LAST_INDEX))
		return;
	rmh->LgCmd = vx_dsp_cmds[cmd].length;
	rmh->LgStat = vx_dsp_cmds[cmd].st_length;
	rmh->DspStat = vx_dsp_cmds[cmd].st_type;
	rmh->Cmd[0] = vx_dsp_cmds[cmd].opcode;
}

