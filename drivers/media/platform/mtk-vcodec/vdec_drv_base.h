/* SPDX-License-Identifier: GPL-2.0 */
/*
<<<<<<< HEAD
 * Copyright (c) 2019 MediaTek Inc.
=======
 * Copyright (c) 2016 MediaTek Inc.
 * Author: PC Chen <pc.chen@mediatek.com>
>>>>>>> upstream/android-13
 */

#ifndef _VDEC_DRV_BASE_
#define _VDEC_DRV_BASE_

<<<<<<< HEAD
#include "mtk_vcodec_drv.h"

=======
>>>>>>> upstream/android-13
#include "vdec_drv_if.h"

struct vdec_common_if {
	/**
	 * (*init)() - initialize decode driver
	 * @ctx     : [in] mtk v4l2 context
	 * @h_vdec  : [out] driver handle
	 */
<<<<<<< HEAD
	int (*init)(struct mtk_vcodec_ctx *ctx, unsigned long *h_vdec);
=======
	int (*init)(struct mtk_vcodec_ctx *ctx);
>>>>>>> upstream/android-13

	/**
	 * (*decode)() - trigger decode
	 * @h_vdec  : [in] driver handle
	 * @bs      : [in] input bitstream
	 * @fb      : [in] frame buffer to store decoded frame
<<<<<<< HEAD
	 * @src_chg : [out] some changed flags
	 */
	int (*decode)(unsigned long h_vdec, struct mtk_vcodec_mem *bs,
				  struct vdec_fb *fb, unsigned int *src_chg);
=======
	 * @res_chg : [out] resolution change happen
	 */
	int (*decode)(void *h_vdec, struct mtk_vcodec_mem *bs,
		      struct vdec_fb *fb, bool *res_chg);
>>>>>>> upstream/android-13

	/**
	 * (*get_param)() - get driver's parameter
	 * @h_vdec : [in] driver handle
	 * @type   : [in] input parameter type
	 * @out    : [out] buffer to store query result
	 */
<<<<<<< HEAD
	int (*get_param)(unsigned long h_vdec, enum vdec_get_param_type type,
					 void *out);

	/**
	 * (*set_param)() - set driver's parameter
	 * @h_vdec : [in] driver handle
	 * @type   : [in] input parameter type
	 * @in     : [in] buffer to store query
	 */
	int (*set_param)(unsigned long h_vdec, enum vdec_set_param_type type,
					 void *in);
	/**
	 * (*deinit)() - deinitialize driver.
	 * @h_vdec : [in] driver handle to be deinit
	 */
	void (*deinit)(unsigned long h_vdec);
=======
	int (*get_param)(void *h_vdec, enum vdec_get_param_type type,
			 void *out);

	/**
	 * (*deinit)() - deinitialize driver.
	 * @h_vdec : [in] driver handle to be deinit
	 */
	void (*deinit)(void *h_vdec);
>>>>>>> upstream/android-13
};

#endif
