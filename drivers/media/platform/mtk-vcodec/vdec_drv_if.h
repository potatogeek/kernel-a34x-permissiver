/* SPDX-License-Identifier: GPL-2.0 */
/*
<<<<<<< HEAD
 * Copyright (c) 2019 MediaTek Inc.
=======
 * Copyright (c) 2016 MediaTek Inc.
 * Author: PC Chen <pc.chen@mediatek.com>
 *		   Tiffany Lin <tiffany.lin@mediatek.com>
>>>>>>> upstream/android-13
 */

#ifndef _VDEC_DRV_IF_H_
#define _VDEC_DRV_IF_H_

#include "mtk_vcodec_drv.h"
#include "mtk_vcodec_dec.h"
#include "mtk_vcodec_util.h"
<<<<<<< HEAD
#include "vdec_vcu_if.h"
#include "vdec_ipi_msg.h"

/**
 * struct vdec_inst - decoder instance
 * @num_nalu : how many nalus be decoded
 * @ctx      : point to mtk_vcodec_ctx
 * @vcu      : VCU instance
 * @vsi      : VCU shared information
 */
struct vdec_inst {
	unsigned int num_nalu;
	struct mtk_vcodec_ctx *ctx;
	struct vdec_vcu_inst vcu;
	struct vdec_vsi *vsi;
};

/**
 * struct vdec_fb_status  - decoder frame buffer status
 * @FB_ST_NORMAL        : initial state
 * @FB_ST_DISPLAY       : frmae buffer is ready to be displayed
 * @FB_ST_FREE          : frame buffer is not used by decoder any more
 */
enum vdec_fb_status {
	FB_ST_NORMAL            = 0,
	FB_ST_DISPLAY           = (1 << 0),
	FB_ST_FREE              = (1 << 1)
=======


/**
 * enum vdec_fb_status  - decoder frame buffer status
 * @FB_ST_NORMAL: initial state
 * @FB_ST_DISPLAY: frame buffer is ready to be displayed
 * @FB_ST_FREE: frame buffer is not used by decoder any more
 */
enum vdec_fb_status {
	FB_ST_NORMAL		= 0,
	FB_ST_DISPLAY		= (1 << 0),
	FB_ST_FREE		= (1 << 1)
};

/* For GET_PARAM_DISP_FRAME_BUFFER and GET_PARAM_FREE_FRAME_BUFFER,
 * the caller does not own the returned buffer. The buffer will not be
 *				released before vdec_if_deinit.
 * GET_PARAM_DISP_FRAME_BUFFER	: get next displayable frame buffer,
 *				struct vdec_fb**
 * GET_PARAM_FREE_FRAME_BUFFER	: get non-referenced framebuffer, vdec_fb**
 * GET_PARAM_PIC_INFO		: get picture info, struct vdec_pic_info*
 * GET_PARAM_CROP_INFO		: get crop info, struct v4l2_crop*
 * GET_PARAM_DPB_SIZE		: get dpb size, unsigned int*
 */
enum vdec_get_param_type {
	GET_PARAM_DISP_FRAME_BUFFER,
	GET_PARAM_FREE_FRAME_BUFFER,
	GET_PARAM_PIC_INFO,
	GET_PARAM_CROP_INFO,
	GET_PARAM_DPB_SIZE
>>>>>>> upstream/android-13
};

/**
 * struct vdec_fb_node  - decoder frame buffer node
<<<<<<< HEAD
 * @list        : list to hold this node
 * @fb  : point to frame buffer (vdec_fb), fb could point to frame buffer and
 *      working buffer this is for maintain buffers in different state
=======
 * @list	: list to hold this node
 * @fb	: point to frame buffer (vdec_fb), fb could point to frame buffer and
 *	working buffer this is for maintain buffers in different state
>>>>>>> upstream/android-13
 */
struct vdec_fb_node {
	struct list_head list;
	struct vdec_fb *fb;
};

<<<<<<< HEAD
extern struct mtk_video_fmt
	mtk_vdec_formats[MTK_MAX_DEC_CODECS_SUPPORT];
extern struct mtk_codec_framesizes
	mtk_vdec_framesizes[MTK_MAX_DEC_CODECS_SUPPORT];

/**
 * vdec_if_init() - initialize decode driver
 * @ctx : [in] v4l2 context
 * @fourcc      : [in] video format fourcc, V4L2_PIX_FMT_H264/VP8/VP9..
=======
extern const struct vdec_common_if vdec_h264_if;
extern const struct vdec_common_if vdec_vp8_if;
extern const struct vdec_common_if vdec_vp9_if;

/**
 * vdec_if_init() - initialize decode driver
 * @ctx	: [in] v4l2 context
 * @fourcc	: [in] video format fourcc, V4L2_PIX_FMT_H264/VP8/VP9..
>>>>>>> upstream/android-13
 */
int vdec_if_init(struct mtk_vcodec_ctx *ctx, unsigned int fourcc);

/**
 * vdec_if_deinit() - deinitialize decode driver
<<<<<<< HEAD
 * @ctx : [in] v4l2 context
=======
 * @ctx	: [in] v4l2 context
>>>>>>> upstream/android-13
 *
 */
void vdec_if_deinit(struct mtk_vcodec_ctx *ctx);

/**
 * vdec_if_decode() - trigger decode
<<<<<<< HEAD
 * @ctx : [in] v4l2 context
 * @bs  : [in] input bitstream
 * @fb  : [in] frame buffer to store decoded frame, when null menas parse
 *      header only
 * @res_chg     : [out] resolution change happens if current bs have different
 *      picture width/height
 * Note: To flush the decoder when reaching EOF, set input bitstream as NULL.
 */
int vdec_if_decode(struct mtk_vcodec_ctx *ctx, struct mtk_vcodec_mem *bs,
				   struct vdec_fb *fb, unsigned int *src_chg);

/**
 * vdec_if_get_param() - get driver's parameter
 * @ctx : [in] v4l2 context
 * @type        : [in] input parameter type
 * @out : [out] buffer to store query result
 */
int vdec_if_get_param(struct mtk_vcodec_ctx *ctx, enum vdec_get_param_type type,
					  void *out);

/*
 * vdec_if_set_param - Set parameter to driver
 * @ctx : [in] v4l2 context
 * @type        : [in] input parameter type
 * @out : [in] input parameter
 * Return: 0 if setting param successfully, otherwise it is failed.
 */
int vdec_if_set_param(struct mtk_vcodec_ctx *ctx,
					  enum vdec_set_param_type type,
					  void *in);


void vdec_decode_prepare(void *ctx_prepare,
	unsigned int hw_id);
void vdec_decode_unprepare(void *ctx_unprepare,
	unsigned int hw_id);
void vdec_check_release_lock(void *ctx_check);
=======
 * @ctx	: [in] v4l2 context
 * @bs	: [in] input bitstream
 * @fb	: [in] frame buffer to store decoded frame, when null means parse
 *	header only
 * @res_chg	: [out] resolution change happens if current bs have different
 *	picture width/height
 * Note: To flush the decoder when reaching EOF, set input bitstream as NULL.
 *
 * Return: 0 on success. -EIO on unrecoverable error.
 */
int vdec_if_decode(struct mtk_vcodec_ctx *ctx, struct mtk_vcodec_mem *bs,
		   struct vdec_fb *fb, bool *res_chg);

/**
 * vdec_if_get_param() - get driver's parameter
 * @ctx	: [in] v4l2 context
 * @type	: [in] input parameter type
 * @out	: [out] buffer to store query result
 */
int vdec_if_get_param(struct mtk_vcodec_ctx *ctx, enum vdec_get_param_type type,
		      void *out);
>>>>>>> upstream/android-13

#endif
