/* SPDX-License-Identifier: GPL-2.0 */
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

#ifndef _VENC_DRV_BASE_
#define _VENC_DRV_BASE_

#include "mtk_vcodec_drv.h"

#include "venc_drv_if.h"

struct venc_common_if {
	/**
	 * (*init)() - initialize driver
<<<<<<< HEAD
	 * @ctx:        [in] mtk v4l2 context
	 * @handle: [out] driver handle
	 */
	int (*init)(struct mtk_vcodec_ctx *ctx, unsigned long *handle);
=======
	 * @ctx:	[in] mtk v4l2 context
	 * @handle: [out] driver handle
	 */
	int (*init)(struct mtk_vcodec_ctx *ctx);
>>>>>>> upstream/android-13

	/**
	 * (*encode)() - trigger encode
	 * @handle: [in] driver handle
	 * @opt: [in] encode option
	 * @frm_buf: [in] frame buffer to store input frame
	 * @bs_buf: [in] bitstream buffer to store output bitstream
	 * @result: [out] encode result
	 */
<<<<<<< HEAD
	int (*encode)(unsigned long handle, enum venc_start_opt opt,
				  struct venc_frm_buf *frm_buf,
				  struct mtk_vcodec_mem *bs_buf,
				  struct venc_done_result *result);

	/**
	 * (*get_param)() - set driver's parameter
	 * @handle: [in] driver handle
	 * @type: [in] parameter type
	 * @in: [in] buffer to store the parameter
	 */
	int (*get_param)(unsigned long handle, enum venc_get_param_type type,
					 void *out);
=======
	int (*encode)(void *handle, enum venc_start_opt opt,
		      struct venc_frm_buf *frm_buf,
		      struct mtk_vcodec_mem *bs_buf,
		      struct venc_done_result *result);
>>>>>>> upstream/android-13

	/**
	 * (*set_param)() - set driver's parameter
	 * @handle: [in] driver handle
	 * @type: [in] parameter type
	 * @in: [in] buffer to store the parameter
	 */
<<<<<<< HEAD
	int (*set_param)(unsigned long handle, enum venc_set_param_type type,
					 struct venc_enc_param *in);
=======
	int (*set_param)(void *handle, enum venc_set_param_type type,
			 struct venc_enc_param *in);
>>>>>>> upstream/android-13

	/**
	 * (*deinit)() - deinitialize driver.
	 * @handle: [in] driver handle
	 */
<<<<<<< HEAD
	int (*deinit)(unsigned long handle);
=======
	int (*deinit)(void *handle);
>>>>>>> upstream/android-13
};

#endif
