/* SPDX-License-Identifier: GPL-2.0 */
/*
<<<<<<< HEAD
 * Copyright (c) 2019 MediaTek Inc.
=======
 * Copyright (c) 2016 MediaTek Inc.
 * Author: PoChun Lin <pochun.lin@mediatek.com>
>>>>>>> upstream/android-13
 */

#ifndef _VENC_VPU_IF_H_
#define _VENC_VPU_IF_H_

<<<<<<< HEAD
#include "mtk_vpu.h"
=======
#include "mtk_vcodec_fw.h"
>>>>>>> upstream/android-13
#include "venc_drv_if.h"

/*
 * struct venc_vpu_inst - encoder VPU driver instance
 * @wq_hd: wait queue used for vpu cmd trigger then wait vpu interrupt done
 * @signaled: flag used for checking vpu interrupt done
 * @failure: flag to show vpu cmd succeeds or not
 * @state: enum venc_ipi_msg_enc_state
 * @bs_size: bitstream size for skip frame case usage
 * @is_key_frm: key frame flag
 * @inst_addr: VPU instance addr
 * @vsi: driver structure allocated by VPU side and shared to AP side for
<<<<<<< HEAD
 *       control and info share
=======
 *	 control and info share
>>>>>>> upstream/android-13
 * @id: the id of inter-processor interrupt
 * @ctx: context for v4l2 layer integration
 * @dev: device for v4l2 layer integration
 */
struct venc_vpu_inst {
	wait_queue_head_t wq_hd;
	int signaled;
	int failure;
	int state;
	int bs_size;
	int is_key_frm;
	unsigned int inst_addr;
	void *vsi;
<<<<<<< HEAD
	enum ipi_id id;
	struct mtk_vcodec_ctx *ctx;
	struct platform_device *dev;
=======
	int id;
	struct mtk_vcodec_ctx *ctx;
>>>>>>> upstream/android-13
};

int vpu_enc_init(struct venc_vpu_inst *vpu);
int vpu_enc_set_param(struct venc_vpu_inst *vpu,
<<<<<<< HEAD
					  enum venc_set_param_type id,
					  struct venc_enc_param *param);
int vpu_enc_encode(struct venc_vpu_inst *vpu, unsigned int bs_mode,
				   struct venc_frm_buf *frm_buf,
				   struct mtk_vcodec_mem *bs_buf,
				   unsigned int *bs_size);
=======
		      enum venc_set_param_type id,
		      struct venc_enc_param *param);
int vpu_enc_encode(struct venc_vpu_inst *vpu, unsigned int bs_mode,
		   struct venc_frm_buf *frm_buf,
		   struct mtk_vcodec_mem *bs_buf,
		   unsigned int *bs_size,
		   struct venc_frame_info *frame_info);
>>>>>>> upstream/android-13
int vpu_enc_deinit(struct venc_vpu_inst *vpu);

#endif
