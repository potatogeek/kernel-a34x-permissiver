<<<<<<< HEAD
/*
 * Copyright (c) 2012-2016, The Linux Foundation. All rights reserved.
 * Copyright (C) 2017 Linaro Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
=======
// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright (c) 2012-2016, The Linux Foundation. All rights reserved.
 * Copyright (C) 2017 Linaro Ltd.
>>>>>>> upstream/android-13
 */
#include <linux/types.h>
#include <media/v4l2-ctrls.h>

#include "core.h"
<<<<<<< HEAD
=======
#include "helpers.h"
>>>>>>> upstream/android-13
#include "vdec.h"

static int vdec_op_s_ctrl(struct v4l2_ctrl *ctrl)
{
	struct venus_inst *inst = ctrl_to_inst(ctrl);
	struct vdec_controls *ctr = &inst->controls.dec;

	switch (ctrl->id) {
	case V4L2_CID_MPEG_VIDEO_DECODER_MPEG4_DEBLOCK_FILTER:
		ctr->post_loop_deb_mode = ctrl->val;
		break;
	case V4L2_CID_MPEG_VIDEO_H264_PROFILE:
	case V4L2_CID_MPEG_VIDEO_MPEG4_PROFILE:
	case V4L2_CID_MPEG_VIDEO_VP8_PROFILE:
<<<<<<< HEAD
=======
	case V4L2_CID_MPEG_VIDEO_VP9_PROFILE:
>>>>>>> upstream/android-13
		ctr->profile = ctrl->val;
		break;
	case V4L2_CID_MPEG_VIDEO_H264_LEVEL:
	case V4L2_CID_MPEG_VIDEO_MPEG4_LEVEL:
<<<<<<< HEAD
		ctr->level = ctrl->val;
		break;
=======
	case V4L2_CID_MPEG_VIDEO_VP9_LEVEL:
		ctr->level = ctrl->val;
		break;
	case V4L2_CID_MPEG_VIDEO_DEC_DISPLAY_DELAY:
		ctr->display_delay = ctrl->val;
		break;
	case V4L2_CID_MPEG_VIDEO_DEC_DISPLAY_DELAY_ENABLE:
		ctr->display_delay_enable = ctrl->val;
		break;
	case V4L2_CID_MPEG_VIDEO_DEC_CONCEAL_COLOR:
		ctr->conceal_color = *ctrl->p_new.p_s64;
		break;
>>>>>>> upstream/android-13
	default:
		return -EINVAL;
	}

	return 0;
}

static int vdec_op_g_volatile_ctrl(struct v4l2_ctrl *ctrl)
{
	struct venus_inst *inst = ctrl_to_inst(ctrl);
	struct vdec_controls *ctr = &inst->controls.dec;
<<<<<<< HEAD
	union hfi_get_property hprop;
	u32 ptype = HFI_PROPERTY_PARAM_PROFILE_LEVEL_CURRENT;
=======
	struct hfi_buffer_requirements bufreq;
	enum hfi_version ver = inst->core->res->hfi_version;
	u32 profile, level;
>>>>>>> upstream/android-13
	int ret;

	switch (ctrl->id) {
	case V4L2_CID_MPEG_VIDEO_H264_PROFILE:
	case V4L2_CID_MPEG_VIDEO_MPEG4_PROFILE:
	case V4L2_CID_MPEG_VIDEO_VP8_PROFILE:
<<<<<<< HEAD
		ret = hfi_session_get_property(inst, ptype, &hprop);
		if (!ret)
			ctr->profile = hprop.profile_level.profile;
=======
	case V4L2_CID_MPEG_VIDEO_VP9_PROFILE:
		ret = venus_helper_get_profile_level(inst, &profile, &level);
		if (!ret)
			ctr->profile = profile;
>>>>>>> upstream/android-13
		ctrl->val = ctr->profile;
		break;
	case V4L2_CID_MPEG_VIDEO_H264_LEVEL:
	case V4L2_CID_MPEG_VIDEO_MPEG4_LEVEL:
<<<<<<< HEAD
		ret = hfi_session_get_property(inst, ptype, &hprop);
		if (!ret)
			ctr->level = hprop.profile_level.level;
=======
	case V4L2_CID_MPEG_VIDEO_VP9_LEVEL:
		ret = venus_helper_get_profile_level(inst, &profile, &level);
		if (!ret)
			ctr->level = level;
>>>>>>> upstream/android-13
		ctrl->val = ctr->level;
		break;
	case V4L2_CID_MPEG_VIDEO_DECODER_MPEG4_DEBLOCK_FILTER:
		ctrl->val = ctr->post_loop_deb_mode;
		break;
	case V4L2_CID_MIN_BUFFERS_FOR_CAPTURE:
<<<<<<< HEAD
		ctrl->val = inst->num_output_bufs;
		break;
	default:
		return -EINVAL;
	};
=======
		ret = venus_helper_get_bufreq(inst, HFI_BUFFER_OUTPUT, &bufreq);
		if (!ret)
			ctrl->val = HFI_BUFREQ_COUNT_MIN(&bufreq, ver);
		break;
	default:
		return -EINVAL;
	}
>>>>>>> upstream/android-13

	return 0;
}

static const struct v4l2_ctrl_ops vdec_ctrl_ops = {
	.s_ctrl = vdec_op_s_ctrl,
	.g_volatile_ctrl = vdec_op_g_volatile_ctrl,
};

int vdec_ctrl_init(struct venus_inst *inst)
{
	struct v4l2_ctrl *ctrl;
	int ret;

<<<<<<< HEAD
	ret = v4l2_ctrl_handler_init(&inst->ctrl_handler, 7);
=======
	ret = v4l2_ctrl_handler_init(&inst->ctrl_handler, 12);
>>>>>>> upstream/android-13
	if (ret)
		return ret;

	ctrl = v4l2_ctrl_new_std_menu(&inst->ctrl_handler, &vdec_ctrl_ops,
		V4L2_CID_MPEG_VIDEO_MPEG4_PROFILE,
		V4L2_MPEG_VIDEO_MPEG4_PROFILE_ADVANCED_CODING_EFFICIENCY,
		~((1 << V4L2_MPEG_VIDEO_MPEG4_PROFILE_SIMPLE) |
		  (1 << V4L2_MPEG_VIDEO_MPEG4_PROFILE_ADVANCED_SIMPLE)),
		V4L2_MPEG_VIDEO_MPEG4_PROFILE_SIMPLE);
	if (ctrl)
		ctrl->flags |= V4L2_CTRL_FLAG_VOLATILE;

	ctrl = v4l2_ctrl_new_std_menu(&inst->ctrl_handler, &vdec_ctrl_ops,
				      V4L2_CID_MPEG_VIDEO_MPEG4_LEVEL,
				      V4L2_MPEG_VIDEO_MPEG4_LEVEL_5,
				      0, V4L2_MPEG_VIDEO_MPEG4_LEVEL_0);
	if (ctrl)
		ctrl->flags |= V4L2_CTRL_FLAG_VOLATILE;

	ctrl = v4l2_ctrl_new_std_menu(&inst->ctrl_handler, &vdec_ctrl_ops,
		V4L2_CID_MPEG_VIDEO_H264_PROFILE,
		V4L2_MPEG_VIDEO_H264_PROFILE_MULTIVIEW_HIGH,
		~((1 << V4L2_MPEG_VIDEO_H264_PROFILE_BASELINE) |
		  (1 << V4L2_MPEG_VIDEO_H264_PROFILE_CONSTRAINED_BASELINE) |
		  (1 << V4L2_MPEG_VIDEO_H264_PROFILE_MAIN) |
		  (1 << V4L2_MPEG_VIDEO_H264_PROFILE_HIGH) |
		  (1 << V4L2_MPEG_VIDEO_H264_PROFILE_STEREO_HIGH) |
		  (1 << V4L2_MPEG_VIDEO_H264_PROFILE_MULTIVIEW_HIGH)),
		V4L2_MPEG_VIDEO_H264_PROFILE_BASELINE);
	if (ctrl)
		ctrl->flags |= V4L2_CTRL_FLAG_VOLATILE;

	ctrl = v4l2_ctrl_new_std_menu(&inst->ctrl_handler, &vdec_ctrl_ops,
				      V4L2_CID_MPEG_VIDEO_H264_LEVEL,
				      V4L2_MPEG_VIDEO_H264_LEVEL_5_1,
				      0, V4L2_MPEG_VIDEO_H264_LEVEL_1_0);
	if (ctrl)
		ctrl->flags |= V4L2_CTRL_FLAG_VOLATILE;

	ctrl = v4l2_ctrl_new_std_menu(&inst->ctrl_handler, &vdec_ctrl_ops,
				      V4L2_CID_MPEG_VIDEO_VP8_PROFILE,
				      V4L2_MPEG_VIDEO_VP8_PROFILE_3,
				      0, V4L2_MPEG_VIDEO_VP8_PROFILE_0);
	if (ctrl)
		ctrl->flags |= V4L2_CTRL_FLAG_VOLATILE;

<<<<<<< HEAD
=======
	ctrl = v4l2_ctrl_new_std_menu(&inst->ctrl_handler, &vdec_ctrl_ops,
				      V4L2_CID_MPEG_VIDEO_VP9_PROFILE,
				      V4L2_MPEG_VIDEO_VP9_PROFILE_3,
				      0, V4L2_MPEG_VIDEO_VP9_PROFILE_0);
	if (ctrl)
		ctrl->flags |= V4L2_CTRL_FLAG_VOLATILE;

	ctrl = v4l2_ctrl_new_std_menu(&inst->ctrl_handler, &vdec_ctrl_ops,
				      V4L2_CID_MPEG_VIDEO_VP9_LEVEL,
				      V4L2_MPEG_VIDEO_VP9_LEVEL_6_2,
				      0, V4L2_MPEG_VIDEO_VP9_LEVEL_1_0);
	if (ctrl)
		ctrl->flags |= V4L2_CTRL_FLAG_VOLATILE;

>>>>>>> upstream/android-13
	v4l2_ctrl_new_std(&inst->ctrl_handler, &vdec_ctrl_ops,
		V4L2_CID_MPEG_VIDEO_DECODER_MPEG4_DEBLOCK_FILTER, 0, 1, 1, 0);

	ctrl = v4l2_ctrl_new_std(&inst->ctrl_handler, &vdec_ctrl_ops,
		V4L2_CID_MIN_BUFFERS_FOR_CAPTURE, 1, 32, 1, 1);
	if (ctrl)
		ctrl->flags |= V4L2_CTRL_FLAG_VOLATILE;

<<<<<<< HEAD
=======
	v4l2_ctrl_new_std(&inst->ctrl_handler, &vdec_ctrl_ops,
			  V4L2_CID_MPEG_VIDEO_DEC_DISPLAY_DELAY,
			  0, 16383, 1, 0);

	v4l2_ctrl_new_std(&inst->ctrl_handler, &vdec_ctrl_ops,
			  V4L2_CID_MPEG_VIDEO_DEC_DISPLAY_DELAY_ENABLE,
			  0, 1, 1, 0);

	v4l2_ctrl_new_std(&inst->ctrl_handler, &vdec_ctrl_ops,
			  V4L2_CID_MPEG_VIDEO_DEC_CONCEAL_COLOR, 0,
			  0xffffffffffffLL, 1, 0x8000800010LL);

>>>>>>> upstream/android-13
	ret = inst->ctrl_handler.error;
	if (ret) {
		v4l2_ctrl_handler_free(&inst->ctrl_handler);
		return ret;
	}

	return 0;
}

void vdec_ctrl_deinit(struct venus_inst *inst)
{
	v4l2_ctrl_handler_free(&inst->ctrl_handler);
}
