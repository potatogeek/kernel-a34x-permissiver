<<<<<<< HEAD
/*
 *
 *
 *  Copyright (C) 2005 Mike Isely <isely@pobox.com>
 *  Copyright (C) 2004 Aurelien Alleaume <slts@free.fr>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
=======
// SPDX-License-Identifier: GPL-2.0-only
/*
 *
 *  Copyright (C) 2005 Mike Isely <isely@pobox.com>
 *  Copyright (C) 2004 Aurelien Alleaume <slts@free.fr>
>>>>>>> upstream/android-13
 */

/*

   This source file is specifically designed to interface with the
   wm8775.

*/

#include "pvrusb2-wm8775.h"


#include "pvrusb2-hdw-internal.h"
#include "pvrusb2-debug.h"
#include <linux/videodev2.h>
#include <media/v4l2-common.h>
#include <linux/errno.h>

void pvr2_wm8775_subdev_update(struct pvr2_hdw *hdw, struct v4l2_subdev *sd)
{
	if (hdw->input_dirty || hdw->force_dirty) {
		u32 input;

		switch (hdw->input_val) {
		case PVR2_CVAL_INPUT_RADIO:
			input = 1;
			break;
		default:
			/* All other cases just use the second input */
			input = 2;
			break;
		}
		pvr2_trace(PVR2_TRACE_CHIPS, "subdev wm8775 set_input(val=%d route=0x%x)",
			   hdw->input_val, input);

		sd->ops->audio->s_routing(sd, input, 0, 0);
	}
}
