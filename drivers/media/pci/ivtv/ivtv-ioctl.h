<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-or-later */
>>>>>>> upstream/android-13
/*
    ioctl system call
    Copyright (C) 2003-2004  Kevin Thayer <nufan_wfk at yahoo.com>
    Copyright (C) 2005-2007  Hans Verkuil <hverkuil@xs4all.nl>

<<<<<<< HEAD
    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
=======
>>>>>>> upstream/android-13
 */

#ifndef IVTV_IOCTL_H
#define IVTV_IOCTL_H

u16 ivtv_service2vbi(int type);
void ivtv_expand_service_set(struct v4l2_sliced_vbi_format *fmt, int is_pal);
u16 ivtv_get_service_set(struct v4l2_sliced_vbi_format *fmt);
void ivtv_set_osd_alpha(struct ivtv *itv);
int ivtv_set_speed(struct ivtv *itv, int speed);
void ivtv_set_funcs(struct video_device *vdev);
void ivtv_s_std_enc(struct ivtv *itv, v4l2_std_id std);
void ivtv_s_std_dec(struct ivtv *itv, v4l2_std_id std);
int ivtv_s_frequency(struct file *file, void *fh, const struct v4l2_frequency *vf);
int ivtv_s_input(struct file *file, void *fh, unsigned int inp);

#endif
