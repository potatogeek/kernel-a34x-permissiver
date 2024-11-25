/* SPDX-License-Identifier: GPL-2.0 */
/*
<<<<<<< HEAD
 * Copyright (c) 2019 MediaTek Inc.
 */
=======
* Copyright (c) 2016 MediaTek Inc.
* Author: PC Chen <pc.chen@mediatek.com>
*         Tiffany Lin <tiffany.lin@mediatek.com>
*/
>>>>>>> upstream/android-13

#ifndef _MTK_VCODEC_ENC_H_
#define _MTK_VCODEC_ENC_H_

#include <media/videobuf2-core.h>
<<<<<<< HEAD
#include <media/videobuf2-v4l2.h>

/**
 * enum eos_types  - encoder different eos types
 * @NON_EOS     : no eos, normal frame
 * @EOS_WITH_DATA      : early eos , mean this frame need to encode
 * @EOS : byteused of the last frame is zero
 */
enum eos_types {
	NON_EOS = 0,
	EOS_WITH_DATA,
	EOS
};

/**
 * struct mtk_video_enc_buf - Private data related to each VB2 buffer.
 * @vb: Pointer to related VB2 buffer.
 * @list:       list that buffer link to
 * @param_change: Types of encode parameter change before encoding this
 *                              buffer
 * @enc_params: Encode parameters changed before encode this buffer
 * @flags:  flags derived from v4l2_buffer for cache operations
 */
struct mtk_video_enc_buf {
	struct vb2_v4l2_buffer vb;
	struct list_head list;
	u32 param_change;
	struct mtk_enc_params enc_params;
	enum eos_types lastframe;
	int    flags;
	struct mtk_vcodec_mem bs_buf;
	struct venc_frm_buf frm_buf;
	unsigned int roimap;
	bool has_meta;
	unsigned int qpmap;
	struct dma_buf *meta_dma;
=======
#include <media/v4l2-mem2mem.h>

#define MTK_VENC_IRQ_STATUS_SPS	0x1
#define MTK_VENC_IRQ_STATUS_PPS	0x2
#define MTK_VENC_IRQ_STATUS_FRM	0x4
#define MTK_VENC_IRQ_STATUS_DRAM	0x8
#define MTK_VENC_IRQ_STATUS_PAUSE	0x10
#define MTK_VENC_IRQ_STATUS_SWITCH	0x20

#define MTK_VENC_IRQ_STATUS_OFFSET	0x05C
#define MTK_VENC_IRQ_ACK_OFFSET	0x060

/**
 * struct mtk_video_enc_buf - Private data related to each VB2 buffer.
 * @m2m_buf:	M2M buffer
 * @list:	list that buffer link to
 * @param_change: Types of encode parameter change before encoding this
 *				buffer
 * @enc_params: Encode parameters changed before encode this buffer
 */
struct mtk_video_enc_buf {
	struct v4l2_m2m_buffer m2m_buf;

	u32 param_change;
	struct mtk_enc_params enc_params;
>>>>>>> upstream/android-13
};

extern const struct v4l2_ioctl_ops mtk_venc_ioctl_ops;
extern const struct v4l2_m2m_ops mtk_venc_m2m_ops;

<<<<<<< HEAD
void mtk_venc_unlock(struct mtk_vcodec_ctx *ctx, u32 hw_id);
int mtk_venc_lock(struct mtk_vcodec_ctx *ctx, u32 hw_id, bool sec);
int mtk_vcodec_enc_queue_init(void *priv, struct vb2_queue *src_vq,
	struct vb2_queue *dst_vq);
void mtk_vcodec_enc_empty_queues(struct file *file, struct mtk_vcodec_ctx *ctx);
=======
int mtk_venc_unlock(struct mtk_vcodec_ctx *ctx);
int mtk_venc_lock(struct mtk_vcodec_ctx *ctx);
int mtk_vcodec_enc_queue_init(void *priv, struct vb2_queue *src_vq,
			      struct vb2_queue *dst_vq);
>>>>>>> upstream/android-13
void mtk_vcodec_enc_release(struct mtk_vcodec_ctx *ctx);
int mtk_vcodec_enc_ctrls_setup(struct mtk_vcodec_ctx *ctx);
void mtk_vcodec_enc_set_default_params(struct mtk_vcodec_ctx *ctx);

#endif /* _MTK_VCODEC_ENC_H_ */
