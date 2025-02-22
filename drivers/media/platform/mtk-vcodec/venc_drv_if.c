// SPDX-License-Identifier: GPL-2.0
/*
<<<<<<< HEAD
 * Copyright (c) 2019 MediaTek Inc.
=======
 * Copyright (c) 2016 MediaTek Inc.
 * Author: Daniel Hsiao <daniel.hsiao@mediatek.com>
 *	Jungchang Tsao <jungchang.tsao@mediatek.com>
 *	Tiffany Lin <tiffany.lin@mediatek.com>
>>>>>>> upstream/android-13
 */

#include <linux/interrupt.h>
#include <linux/kernel.h>
#include <linux/slab.h>

#include "venc_drv_base.h"
#include "venc_drv_if.h"

#include "mtk_vcodec_enc.h"
#include "mtk_vcodec_enc_pm.h"

<<<<<<< HEAD
#ifdef CONFIG_VIDEO_MEDIATEK_VCU
#include "mtk_vcu.h"
const struct venc_common_if *get_enc_common_if(void);
const struct venc_common_if *get_enc_log_if(void);
#endif

#ifdef CONFIG_VIDEO_MEDIATEK_VPU
#include "mtk_vpu.h"
const struct venc_common_if *get_h264_enc_comm_if(void);
const struct venc_common_if *get_vp8_enc_comm_if(void);
#endif

=======
>>>>>>> upstream/android-13
int venc_if_init(struct mtk_vcodec_ctx *ctx, unsigned int fourcc)
{
	int ret = 0;

<<<<<<< HEAD
	ctx->oal_vcodec = 0;
	mtk_venc_init_ctx_pm(ctx);

#ifdef CONFIG_VIDEO_MEDIATEK_VCU
	switch (fourcc) {
	case V4L2_PIX_FMT_H264:
	case V4L2_PIX_FMT_H265:
	case V4L2_PIX_FMT_HEIF:
	case V4L2_PIX_FMT_MPEG4:
	case V4L2_PIX_FMT_H263:
		ctx->enc_if = get_enc_common_if();
		ctx->oal_vcodec = 0;
		break;
	case V4L2_CID_MPEG_MTK_LOG:
		ctx->enc_if = get_enc_log_if();
		return 0;
	default:
		return -EINVAL;
	}
#endif
#ifdef CONFIG_VIDEO_MEDIATEK_VPU
	switch (fourcc) {
	case V4L2_PIX_FMT_VP8:
		ctx->enc_if = get_vp8_enc_comm_if();
		break;
	case V4L2_PIX_FMT_H264:
		ctx->enc_if = get_h264_enc_comm_if();
=======
	switch (fourcc) {
	case V4L2_PIX_FMT_VP8:
		ctx->enc_if = &venc_vp8_if;
		break;
	case V4L2_PIX_FMT_H264:
		ctx->enc_if = &venc_h264_if;
>>>>>>> upstream/android-13
		break;
	default:
		return -EINVAL;
	}
<<<<<<< HEAD
#endif
	ret = ctx->enc_if->init(ctx, (unsigned long *)&ctx->drv_handle);

	return ret;
}

int venc_if_get_param(struct mtk_vcodec_ctx *ctx, enum venc_get_param_type type,
					  void *out)
{
	struct venc_inst *inst = NULL;
	int ret = 0;
	int drv_handle_exist = 1;

	if (!ctx->drv_handle) {
		inst = kzalloc(sizeof(struct venc_inst), GFP_KERNEL);
		if (!inst)
			return -ENOMEM;
		inst->ctx = ctx;
		ctx->drv_handle = (unsigned long)(inst);
		ctx->enc_if = get_enc_common_if();
		mtk_vcodec_add_ctx_list(ctx);
		drv_handle_exist = 0;
		mtk_v4l2_debug(0, "%s init drv_handle = 0x%lx",
			__func__, ctx->drv_handle);
	}

	ret = ctx->enc_if->get_param(ctx->drv_handle, type, out);

	if (!drv_handle_exist) {
		mtk_vcodec_del_ctx_list(ctx);
		kfree(inst);
		ctx->drv_handle = 0;
		ctx->enc_if = NULL;
	}
=======

	mtk_venc_lock(ctx);
	mtk_vcodec_enc_clock_on(&ctx->dev->pm);
	ret = ctx->enc_if->init(ctx);
	mtk_vcodec_enc_clock_off(&ctx->dev->pm);
	mtk_venc_unlock(ctx);
>>>>>>> upstream/android-13

	return ret;
}

int venc_if_set_param(struct mtk_vcodec_ctx *ctx,
<<<<<<< HEAD
	enum venc_set_param_type type, struct venc_enc_param *in)
{
	int ret = 0;

	ret = ctx->enc_if->set_param(ctx->drv_handle, type, in);
=======
		enum venc_set_param_type type, struct venc_enc_param *in)
{
	int ret = 0;

	mtk_venc_lock(ctx);
	mtk_vcodec_enc_clock_on(&ctx->dev->pm);
	ret = ctx->enc_if->set_param(ctx->drv_handle, type, in);
	mtk_vcodec_enc_clock_off(&ctx->dev->pm);
	mtk_venc_unlock(ctx);
>>>>>>> upstream/android-13

	return ret;
}

<<<<<<< HEAD
void venc_encode_prepare(void *ctx_prepare,
	unsigned int core_id, unsigned long *flags)
{
	struct mtk_vcodec_ctx *ctx = (struct mtk_vcodec_ctx *)ctx_prepare;

	if (ctx == NULL || core_id >= MTK_VENC_HW_NUM)
		return;

	mtk_venc_pmqos_prelock(ctx, core_id);
	spin_lock_irqsave(&ctx->dev->irqlock, *flags);
	ctx->dev->curr_enc_ctx[0] = ctx;
	spin_unlock_irqrestore(&ctx->dev->irqlock, *flags);
	mtk_vcodec_enc_clock_on(ctx, core_id);
	mtk_venc_pmqos_begin_frame(ctx, core_id);
}
EXPORT_SYMBOL_GPL(venc_encode_prepare);

int venc_lock(void *ctx_lock, int core_id, bool sec)
{
	struct mtk_vcodec_ctx *ctx = (struct mtk_vcodec_ctx *)ctx_lock;

	return mtk_venc_lock(ctx, core_id, sec);
}
EXPORT_SYMBOL_GPL(venc_lock);

void venc_encode_unprepare(void *ctx_unprepare,
	unsigned int core_id, unsigned long *flags)
{
	struct mtk_vcodec_ctx *ctx = (struct mtk_vcodec_ctx *)ctx_unprepare;

	if (ctx == NULL || core_id >= MTK_VENC_HW_NUM)
		return;

	if (ctx->dev->enc_sem[core_id].count != 0) {
		mtk_v4l2_err("HW not prepared, enc_sem[%d].count = %d",
			core_id, ctx->dev->enc_sem[core_id].count);
		return;
	}

	mtk_venc_pmqos_end_frame(ctx, core_id);
	mtk_vcodec_enc_clock_off(ctx, core_id);
	spin_lock_irqsave(&ctx->dev->irqlock, *flags);
	ctx->dev->curr_enc_ctx[0] = NULL;
	spin_unlock_irqrestore(&ctx->dev->irqlock, *flags);
}
EXPORT_SYMBOL_GPL(venc_encode_unprepare);

void venc_unlock(void *ctx_unlock, int core_id)
{
	struct mtk_vcodec_ctx *ctx = (struct mtk_vcodec_ctx *)ctx_unlock;

	mtk_venc_unlock(ctx, core_id);
}
EXPORT_SYMBOL_GPL(venc_unlock);

void venc_encode_pmqos_gce_begin(void *ctx_begin,
	unsigned int core_id, int job_cnt)
{
	mtk_venc_pmqos_gce_flush(ctx_begin, core_id, job_cnt);
}
EXPORT_SYMBOL_GPL(venc_encode_pmqos_gce_begin);

void venc_encode_pmqos_gce_end(void *ctx_end,
	unsigned int core_id, int job_cnt)
{
	mtk_venc_pmqos_gce_done(ctx_end, core_id, job_cnt);
}
EXPORT_SYMBOL_GPL(venc_encode_pmqos_gce_end);

int venc_if_encode(struct mtk_vcodec_ctx *ctx,
	enum venc_start_opt opt, struct venc_frm_buf *frm_buf,
	struct mtk_vcodec_mem *bs_buf,
	struct venc_done_result *result)
{
	int ret = 0;

	if (ctx->drv_handle == 0)
		return 0;

	ret = ctx->enc_if->encode(ctx->drv_handle, opt, frm_buf,
							  bs_buf, result);

=======
int venc_if_encode(struct mtk_vcodec_ctx *ctx,
		   enum venc_start_opt opt, struct venc_frm_buf *frm_buf,
		   struct mtk_vcodec_mem *bs_buf,
		   struct venc_done_result *result)
{
	int ret = 0;
	unsigned long flags;

	mtk_venc_lock(ctx);

	spin_lock_irqsave(&ctx->dev->irqlock, flags);
	ctx->dev->curr_ctx = ctx;
	spin_unlock_irqrestore(&ctx->dev->irqlock, flags);

	mtk_vcodec_enc_clock_on(&ctx->dev->pm);
	ret = ctx->enc_if->encode(ctx->drv_handle, opt, frm_buf,
				  bs_buf, result);
	mtk_vcodec_enc_clock_off(&ctx->dev->pm);

	spin_lock_irqsave(&ctx->dev->irqlock, flags);
	ctx->dev->curr_ctx = NULL;
	spin_unlock_irqrestore(&ctx->dev->irqlock, flags);

	mtk_venc_unlock(ctx);
>>>>>>> upstream/android-13
	return ret;
}

int venc_if_deinit(struct mtk_vcodec_ctx *ctx)
{
	int ret = 0;

<<<<<<< HEAD
	if (ctx->drv_handle == 0)
		return 0;

	ret = ctx->enc_if->deinit(ctx->drv_handle);

	ctx->drv_handle = 0;

	mtk_venc_deinit_ctx_pm(ctx);
=======
	if (!ctx->drv_handle)
		return 0;

	mtk_venc_lock(ctx);
	mtk_vcodec_enc_clock_on(&ctx->dev->pm);
	ret = ctx->enc_if->deinit(ctx->drv_handle);
	mtk_vcodec_enc_clock_off(&ctx->dev->pm);
	mtk_venc_unlock(ctx);

	ctx->drv_handle = NULL;
>>>>>>> upstream/android-13

	return ret;
}
