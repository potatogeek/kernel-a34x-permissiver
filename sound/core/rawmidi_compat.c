<<<<<<< HEAD
/*
 *   32bit -> 64bit ioctl wrapper for raw MIDI API
 *   Copyright (c) by Takashi Iwai <tiwai@suse.de>
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
// SPDX-License-Identifier: GPL-2.0-or-later
/*
 *   32bit -> 64bit ioctl wrapper for raw MIDI API
 *   Copyright (c) by Takashi Iwai <tiwai@suse.de>
>>>>>>> upstream/android-13
 */

/* This file included from rawmidi.c */

#include <linux/compat.h>

struct snd_rawmidi_params32 {
	s32 stream;
	u32 buffer_size;
	u32 avail_min;
	unsigned int no_active_sensing; /* avoid bit-field */
<<<<<<< HEAD
	unsigned char reserved[16];
=======
	unsigned int mode;
	unsigned char reserved[12];
>>>>>>> upstream/android-13
} __attribute__((packed));

static int snd_rawmidi_ioctl_params_compat(struct snd_rawmidi_file *rfile,
					   struct snd_rawmidi_params32 __user *src)
{
	struct snd_rawmidi_params params;
	unsigned int val;

	if (get_user(params.stream, &src->stream) ||
	    get_user(params.buffer_size, &src->buffer_size) ||
	    get_user(params.avail_min, &src->avail_min) ||
<<<<<<< HEAD
=======
	    get_user(params.mode, &src->mode) ||
>>>>>>> upstream/android-13
	    get_user(val, &src->no_active_sensing))
		return -EFAULT;
	params.no_active_sensing = val;
	switch (params.stream) {
	case SNDRV_RAWMIDI_STREAM_OUTPUT:
		if (!rfile->output)
			return -EINVAL;
		return snd_rawmidi_output_params(rfile->output, &params);
	case SNDRV_RAWMIDI_STREAM_INPUT:
		if (!rfile->input)
			return -EINVAL;
		return snd_rawmidi_input_params(rfile->input, &params);
	}
	return -EINVAL;
}

<<<<<<< HEAD
struct snd_rawmidi_status32 {
	s32 stream;
	struct compat_timespec tstamp;
=======
struct compat_snd_rawmidi_status64 {
	s32 stream;
	u8 rsvd[4]; /* alignment */
	s64 tstamp_sec;
	s64 tstamp_nsec;
>>>>>>> upstream/android-13
	u32 avail;
	u32 xruns;
	unsigned char reserved[16];
} __attribute__((packed));

<<<<<<< HEAD
static int snd_rawmidi_ioctl_status_compat(struct snd_rawmidi_file *rfile,
					   struct snd_rawmidi_status32 __user *src)
{
	int err;
	struct snd_rawmidi_status status;
=======
static int snd_rawmidi_ioctl_status_compat64(struct snd_rawmidi_file *rfile,
					     struct compat_snd_rawmidi_status64 __user *src)
{
	int err;
	struct snd_rawmidi_status64 status;
	struct compat_snd_rawmidi_status64 compat_status;
>>>>>>> upstream/android-13

	if (get_user(status.stream, &src->stream))
		return -EFAULT;

	switch (status.stream) {
	case SNDRV_RAWMIDI_STREAM_OUTPUT:
		if (!rfile->output)
			return -EINVAL;
		err = snd_rawmidi_output_status(rfile->output, &status);
		break;
	case SNDRV_RAWMIDI_STREAM_INPUT:
		if (!rfile->input)
			return -EINVAL;
		err = snd_rawmidi_input_status(rfile->input, &status);
		break;
	default:
		return -EINVAL;
	}
	if (err < 0)
		return err;

<<<<<<< HEAD
	if (compat_put_timespec(&status.tstamp, &src->tstamp) ||
	    put_user(status.avail, &src->avail) ||
	    put_user(status.xruns, &src->xruns))
=======
	compat_status = (struct compat_snd_rawmidi_status64) {
		.stream = status.stream,
		.tstamp_sec = status.tstamp_sec,
		.tstamp_nsec = status.tstamp_nsec,
		.avail = status.avail,
		.xruns = status.xruns,
	};

	if (copy_to_user(src, &compat_status, sizeof(*src)))
>>>>>>> upstream/android-13
		return -EFAULT;

	return 0;
}

<<<<<<< HEAD
#ifdef CONFIG_X86_X32
/* X32 ABI has 64bit timespec and 64bit alignment */
struct snd_rawmidi_status_x32 {
	s32 stream;
	u32 rsvd; /* alignment */
	struct timespec tstamp;
	u32 avail;
	u32 xruns;
	unsigned char reserved[16];
} __attribute__((packed));

#define put_timespec(src, dst) copy_to_user(dst, src, sizeof(*dst))

static int snd_rawmidi_ioctl_status_x32(struct snd_rawmidi_file *rfile,
					struct snd_rawmidi_status_x32 __user *src)
{
	int err;
	struct snd_rawmidi_status status;

	if (get_user(status.stream, &src->stream))
		return -EFAULT;

	switch (status.stream) {
	case SNDRV_RAWMIDI_STREAM_OUTPUT:
		if (!rfile->output)
			return -EINVAL;
		err = snd_rawmidi_output_status(rfile->output, &status);
		break;
	case SNDRV_RAWMIDI_STREAM_INPUT:
		if (!rfile->input)
			return -EINVAL;
		err = snd_rawmidi_input_status(rfile->input, &status);
		break;
	default:
		return -EINVAL;
	}
	if (err < 0)
		return err;

	if (put_timespec(&status.tstamp, &src->tstamp) ||
	    put_user(status.avail, &src->avail) ||
	    put_user(status.xruns, &src->xruns))
		return -EFAULT;

	return 0;
}
#endif /* CONFIG_X86_X32 */

enum {
	SNDRV_RAWMIDI_IOCTL_PARAMS32 = _IOWR('W', 0x10, struct snd_rawmidi_params32),
	SNDRV_RAWMIDI_IOCTL_STATUS32 = _IOWR('W', 0x20, struct snd_rawmidi_status32),
#ifdef CONFIG_X86_X32
	SNDRV_RAWMIDI_IOCTL_STATUS_X32 = _IOWR('W', 0x20, struct snd_rawmidi_status_x32),
#endif /* CONFIG_X86_X32 */
=======
enum {
	SNDRV_RAWMIDI_IOCTL_PARAMS32 = _IOWR('W', 0x10, struct snd_rawmidi_params32),
	SNDRV_RAWMIDI_IOCTL_STATUS_COMPAT32 = _IOWR('W', 0x20, struct snd_rawmidi_status32),
	SNDRV_RAWMIDI_IOCTL_STATUS_COMPAT64 = _IOWR('W', 0x20, struct compat_snd_rawmidi_status64),
>>>>>>> upstream/android-13
};

static long snd_rawmidi_ioctl_compat(struct file *file, unsigned int cmd, unsigned long arg)
{
	struct snd_rawmidi_file *rfile;
	void __user *argp = compat_ptr(arg);

	rfile = file->private_data;
	switch (cmd) {
	case SNDRV_RAWMIDI_IOCTL_PVERSION:
	case SNDRV_RAWMIDI_IOCTL_INFO:
	case SNDRV_RAWMIDI_IOCTL_DROP:
	case SNDRV_RAWMIDI_IOCTL_DRAIN:
		return snd_rawmidi_ioctl(file, cmd, (unsigned long)argp);
	case SNDRV_RAWMIDI_IOCTL_PARAMS32:
		return snd_rawmidi_ioctl_params_compat(rfile, argp);
<<<<<<< HEAD
	case SNDRV_RAWMIDI_IOCTL_STATUS32:
		return snd_rawmidi_ioctl_status_compat(rfile, argp);
#ifdef CONFIG_X86_X32
	case SNDRV_RAWMIDI_IOCTL_STATUS_X32:
		return snd_rawmidi_ioctl_status_x32(rfile, argp);
#endif /* CONFIG_X86_X32 */
=======
	case SNDRV_RAWMIDI_IOCTL_STATUS_COMPAT32:
		return snd_rawmidi_ioctl_status32(rfile, argp);
	case SNDRV_RAWMIDI_IOCTL_STATUS_COMPAT64:
		return snd_rawmidi_ioctl_status_compat64(rfile, argp);
>>>>>>> upstream/android-13
	}
	return -ENOIOCTLCMD;
}
