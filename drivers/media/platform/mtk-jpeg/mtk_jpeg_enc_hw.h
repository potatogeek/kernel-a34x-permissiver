<<<<<<< HEAD
/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Copyright (c) 2019 MediaTek Inc.
=======
/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Copyright (c) 2019 MediaTek Inc.
 * Author: Xia Jiang <xia.jiang@mediatek.com>
 *
>>>>>>> upstream/android-13
 */

#ifndef _MTK_JPEG_ENC_HW_H
#define _MTK_JPEG_ENC_HW_H

#include <media/videobuf2-core.h>
<<<<<<< HEAD
#include "mtk_jpeg_core.h"
#include "mtk_jpeg_enc_reg.h"

#define JPEG_ENC_DST_ADDR_OFFSET_MASK 0x0f
#define JPEG_ENC_CTRL_YUV_BIT 0x18
#define JPEG_ENC_CTRL_RESTART_EN_BIT            0x400
#define JPEG_ENC_CTRL_FILE_FORMAT_BIT           0x20
#define JPEG_ENC_EN_JFIF_EXIF                   (1 << 5)
#define JPEG_ENC_CTRL_INT_EN_BIT                0x04
#define JPEG_ENC_CTRL_ENABLE_BIT                0x01
#define JPEG_ENC_CTRL_RDMA_PADDING_EN           (1 << 20)
#define JPEG_ENC_CTRL_RDMA_RIGHT_PADDING_EN     (1 << 29)
#define JPEG_ENC_CTRL_RDMA_PADDING_0_EN         (1 << 30)

enum {
	MTK_JPEG_ENC_RESULT_DONE		= 0,
	MTK_JPEG_ENC_RESULT_STALL		= 1,
	MTK_JPEG_ENC_RESULT_VCODEC_IRQ		= 2,
	MTK_JPEG_ENC_RESULT_ERROR_UNKNOWN	= 3
};

struct mtk_jpeg_enc_bs {
	dma_addr_t	dma_addr;
	size_t		size;
	u32			dma_addr_offset;
	u32			dma_addr_offsetmask;
};
struct mtk_jpeg_mem {
	dma_addr_t	dma_addr;
	size_t		size;
};
struct mtk_jpeg_enc_fb {
	struct mtk_jpeg_mem	fb_addr[MTK_JPEG_COMP_MAX];
	u32			num_planes;
};
void mtk_jpeg_enc_reset(void __iomem *base);
u32 mtk_jpeg_enc_get_int_status(void __iomem *base);
u32 mtk_jpeg_enc_get_file_size(const void __iomem *base);
u32 mtk_jpeg_enc_enum_result(void __iomem *base, u32 irq_status, u32 *fileSize);
void mtk_jpeg_enc_start(void __iomem *enc_reg_base);
void mtk_jpeg_enc_set_config(void __iomem *base,
				  struct mtk_jpeg_enc_param *config,
				  struct mtk_jpeg_enc_bs *bs,
				  struct mtk_jpeg_enc_fb *fb);
#endif /* _MTK_JPEG_HW_H */
=======

#include "mtk_jpeg_core.h"

#define JPEG_ENC_INT_STATUS_DONE	BIT(0)
#define JPEG_ENC_INT_STATUS_MASK_ALLIRQ	0x13

#define JPEG_ENC_DST_ADDR_OFFSET_MASK	GENMASK(3, 0)

#define JPEG_ENC_CTRL_YUV_FORMAT_MASK	0x18
#define JPEG_ENC_CTRL_RESTART_EN_BIT	BIT(10)
#define JPEG_ENC_CTRL_FILE_FORMAT_BIT	BIT(5)
#define JPEG_ENC_CTRL_INT_EN_BIT	BIT(2)
#define JPEG_ENC_CTRL_ENABLE_BIT	BIT(0)
#define JPEG_ENC_RESET_BIT		BIT(0)

#define JPEG_ENC_YUV_FORMAT_YUYV	0
#define JPEG_ENC_YUV_FORMAT_YVYU	1
#define JPEG_ENC_YUV_FORMAT_NV12	2
#define JEPG_ENC_YUV_FORMAT_NV21	3

#define JPEG_ENC_QUALITY_Q60		0x0
#define JPEG_ENC_QUALITY_Q80		0x1
#define JPEG_ENC_QUALITY_Q90		0x2
#define JPEG_ENC_QUALITY_Q95		0x3
#define JPEG_ENC_QUALITY_Q39		0x4
#define JPEG_ENC_QUALITY_Q68		0x5
#define JPEG_ENC_QUALITY_Q84		0x6
#define JPEG_ENC_QUALITY_Q92		0x7
#define JPEG_ENC_QUALITY_Q48		0x8
#define JPEG_ENC_QUALITY_Q74		0xa
#define JPEG_ENC_QUALITY_Q87		0xb
#define JPEG_ENC_QUALITY_Q34		0xc
#define JPEG_ENC_QUALITY_Q64		0xe
#define JPEG_ENC_QUALITY_Q82		0xf
#define JPEG_ENC_QUALITY_Q97		0x10

#define JPEG_ENC_RSTB			0x100
#define JPEG_ENC_CTRL			0x104
#define JPEG_ENC_QUALITY		0x108
#define JPEG_ENC_BLK_NUM		0x10C
#define JPEG_ENC_BLK_CNT		0x110
#define JPEG_ENC_INT_STS		0x11c
#define JPEG_ENC_DST_ADDR0		0x120
#define JPEG_ENC_DMA_ADDR0		0x124
#define JPEG_ENC_STALL_ADDR0		0x128
#define JPEG_ENC_OFFSET_ADDR		0x138
#define JPEG_ENC_RST_MCU_NUM		0x150
#define JPEG_ENC_IMG_SIZE		0x154
#define JPEG_ENC_DEBUG_INFO0		0x160
#define JPEG_ENC_DEBUG_INFO1		0x164
#define JPEG_ENC_TOTAL_CYCLE		0x168
#define JPEG_ENC_BYTE_OFFSET_MASK	0x16c
#define JPEG_ENC_SRC_LUMA_ADDR		0x170
#define JPEG_ENC_SRC_CHROMA_ADDR	0x174
#define JPEG_ENC_STRIDE			0x178
#define JPEG_ENC_IMG_STRIDE		0x17c
#define JPEG_ENC_DCM_CTRL		0x300
#define JPEG_ENC_CODEC_SEL		0x314
#define JPEG_ENC_ULTRA_THRES		0x318

/**
 * struct mtk_jpeg_enc_qlt - JPEG encoder quality data
 * @quality_param:	quality value
 * @hardware_value:	hardware value of quality
 */
struct mtk_jpeg_enc_qlt {
	u8	quality_param;
	u8	hardware_value;
};

void mtk_jpeg_enc_reset(void __iomem *base);
u32 mtk_jpeg_enc_get_file_size(void __iomem *base);
void mtk_jpeg_enc_start(void __iomem *enc_reg_base);
void mtk_jpeg_set_enc_src(struct mtk_jpeg_ctx *ctx,  void __iomem *base,
			  struct vb2_buffer *src_buf);
void mtk_jpeg_set_enc_dst(struct mtk_jpeg_ctx *ctx, void __iomem *base,
			  struct vb2_buffer *dst_buf);
void mtk_jpeg_set_enc_params(struct mtk_jpeg_ctx *ctx,  void __iomem *base);

#endif /* _MTK_JPEG_ENC_HW_H */
>>>>>>> upstream/android-13
