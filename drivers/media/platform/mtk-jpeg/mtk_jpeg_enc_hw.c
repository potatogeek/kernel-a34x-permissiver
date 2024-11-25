<<<<<<< HEAD
// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (c) 2019 MediaTek Inc.
=======
// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright (c) 2019 MediaTek Inc.
 * Author: Xia Jiang <xia.jiang@mediatek.com>
 *
>>>>>>> upstream/android-13
 */

#include <linux/io.h>
#include <linux/kernel.h>
#include <media/videobuf2-core.h>
<<<<<<< HEAD

#include "mtk_jpeg_enc_hw.h"

void mtk_jpeg_enc_reset(void __iomem *base)
{
	writel(0x00, base + JPGENC_RSTB);
	writel(0x01, base + JPGENC_RSTB);
	writel(0x00, base + JPGENC_CODEC_SEL);
}

u32 mtk_jpeg_enc_get_int_status(void __iomem *base)
{
	u32 ret;

	ret = readl(base + JPGENC_INT_STS) &
			JPEG_DRV_ENC_INT_STATUS_MASK_ALLIRQ;
	if (ret)
		writel(0, base + JPGENC_INT_STS);

	return ret;
}

u32 mtk_jpeg_enc_get_file_size(const void __iomem *base) //for dst size
{
	return readl(base + JPGENC_DMA_ADDR0) - readl(base + JPGENC_DST_ADDR0);
}

u32 mtk_jpeg_enc_enum_result(void __iomem *base, u32 irq_status, u32 *fileSize)
{
	*fileSize = mtk_jpeg_enc_get_file_size(base);

	if (irq_status & JPEG_DRV_ENC_INT_STATUS_DONE) {
		pr_info("irq status done file size %d\n", *fileSize);
		return 0;
	} else if (irq_status & JPEG_DRV_ENC_INT_STATUS_STALL) {
		pr_info("irq status stall\n");
		return 1;
	} else if (irq_status & JPEG_DRV_ENC_INT_STATUS_VCODEC_IRQ) {
		pr_info("irq status vcodec irq\n");
		return 2;
	}
	return 3;
}

static void mtk_jpeg_enc_set_blk_num(void __iomem *base, u32 blk_num)
{
	writel((blk_num), base + JPGENC_BLK_NUM);
}

static void mtk_jpeg_enc_set_encFormat(void __iomem *base, u32 encFormat)
{
	u32 val;
	u32 u4Value;

	val = (encFormat & 3) << 3;

	u4Value = readl(base + JPGENC_CTRL);
	u4Value &= ~JPEG_ENC_CTRL_YUV_BIT;
	u4Value |= val;
	writel((u4Value), base + JPGENC_CTRL);
}

static void mtk_jpeg_enc_set_img_size(void __iomem *base, u32 width, u32 height)
{
	u32 u4Value;

	u4Value = (width << 16) | height;
	writel((u4Value), base + JPGENC_IMG_SIZE);
}

static void mtk_jpeg_enc_set_src_img(void __iomem *base, u32 width,
					     u32 height, u32 yuv_format,
					     u32 totalEncDU)
{

	mtk_jpeg_enc_set_img_size(base, width, height);
	mtk_jpeg_enc_set_encFormat(base, yuv_format);
	mtk_jpeg_enc_set_blk_num(base, totalEncDU);
}

static void mtk_jpeg_enc_set_image_stride(void __iomem *base, u32 img_stride)
{
	writel((img_stride), base + JPGENC_IMG_STRIDE);
}

static void mtk_jpeg_enc_set_memory_stride(void __iomem *base, u32 mem_stride)
{
	writel((mem_stride), base + JPGENC_STRIDE);
}

static void mtk_jpeg_enc_set_luma_addr(void __iomem *base, u32 src_luma_addr)
{
	writel((src_luma_addr), base + JPGENC_SRC_LUMA_ADDR);
}

static void mtk_jpeg_enc_set_chroma_addr(void __iomem *base,
				u32 src_chroma_addr)
{
	writel((src_chroma_addr), base + JPGENC_SRC_CHROMA_ADDR);
}

static void mtk_jpeg_enc_set_src_buf(void __iomem *base, u32 img_stride,
				    u32 mem_stride, u32 srcAddr, u32 srcAddr_C)
{

	mtk_jpeg_enc_set_image_stride(base, img_stride);
	mtk_jpeg_enc_set_memory_stride(base, mem_stride);
	mtk_jpeg_enc_set_luma_addr(base, srcAddr);
	mtk_jpeg_enc_set_chroma_addr(base, srcAddr_C);
}

static void mtk_jpeg_enc_set_dst_buf(void __iomem *base, u32 dst_addr,
			u32 stall_size, u32 init_offset, u32 offset_mask)
{

	pr_info("%s dst_addr 0x%x  stall_size %d init_offset 0x%x\n", __func__,
		dst_addr, stall_size, init_offset);

	writel((init_offset & (~0xF)), base + JPGENC_OFFSET_ADDR);
	writel((offset_mask & 0xF), base + JPGENC_BYTE_OFFSET_MASK);
	writel((dst_addr & (~0xF)), base + JPGENC_DST_ADDR0);
	writel(((dst_addr + stall_size) & (~0xF)), base + JPGENC_STALL_ADDR0);
}

static void mtk_jpeg_enc_set_quality(void __iomem *base, u32 quality)
{
	u32 u4Value;

	u4Value = readl(base + JPGENC_QUALITY);
	u4Value = (u4Value & 0xFFFF0000) | quality;
	writel((u4Value), base + JPGENC_QUALITY);
}

static void mtk_jpeg_enc_set_restart_interval(void __iomem *base,
					u32 restart_interval)
{
	u32 Value;

	Value = readl(base + JPGENC_CTRL);
	if (restart_interval != 0) {
		Value |= JPEG_ENC_CTRL_RESTART_EN_BIT;
		writel((Value), base + JPGENC_CTRL);
	} else {
		Value &= ~JPEG_ENC_CTRL_RESTART_EN_BIT;
		writel((Value), base + JPGENC_CTRL);
	}
	writel((restart_interval), base + JPGENC_RST_MCU_NUM);
}

static void mtk_jpeg_enc_set_EncodeMode(void __iomem *base, u32 exif_en)
{
	u32 u4Value;

	u4Value = readl(base + JPGENC_CTRL);
	u4Value &= ~(JPEG_ENC_CTRL_FILE_FORMAT_BIT);
	writel((u4Value), base + JPGENC_CTRL);

	if (exif_en) {
		u4Value = readl(base + JPGENC_CTRL);
		u4Value |= JPEG_ENC_EN_JFIF_EXIF;
		writel((u4Value), base + JPGENC_CTRL);
	}
}

static void mtk_jpeg_enc_set_ctrl_cfg(void __iomem *base, u32 exif_en,
				u32 quality, u32 restart_interval)
{
	mtk_jpeg_enc_set_quality(base, quality);

	mtk_jpeg_enc_set_restart_interval(base, restart_interval);

	mtk_jpeg_enc_set_EncodeMode(base, exif_en);
=======
#include <media/videobuf2-dma-contig.h>

#include "mtk_jpeg_enc_hw.h"

static const struct mtk_jpeg_enc_qlt mtk_jpeg_enc_quality[] = {
	{.quality_param = 34, .hardware_value = JPEG_ENC_QUALITY_Q34},
	{.quality_param = 39, .hardware_value = JPEG_ENC_QUALITY_Q39},
	{.quality_param = 48, .hardware_value = JPEG_ENC_QUALITY_Q48},
	{.quality_param = 60, .hardware_value = JPEG_ENC_QUALITY_Q60},
	{.quality_param = 64, .hardware_value = JPEG_ENC_QUALITY_Q64},
	{.quality_param = 68, .hardware_value = JPEG_ENC_QUALITY_Q68},
	{.quality_param = 74, .hardware_value = JPEG_ENC_QUALITY_Q74},
	{.quality_param = 80, .hardware_value = JPEG_ENC_QUALITY_Q80},
	{.quality_param = 82, .hardware_value = JPEG_ENC_QUALITY_Q82},
	{.quality_param = 84, .hardware_value = JPEG_ENC_QUALITY_Q84},
	{.quality_param = 87, .hardware_value = JPEG_ENC_QUALITY_Q87},
	{.quality_param = 90, .hardware_value = JPEG_ENC_QUALITY_Q90},
	{.quality_param = 92, .hardware_value = JPEG_ENC_QUALITY_Q92},
	{.quality_param = 95, .hardware_value = JPEG_ENC_QUALITY_Q95},
	{.quality_param = 97, .hardware_value = JPEG_ENC_QUALITY_Q97},
};

void mtk_jpeg_enc_reset(void __iomem *base)
{
	writel(0, base + JPEG_ENC_RSTB);
	writel(JPEG_ENC_RESET_BIT, base + JPEG_ENC_RSTB);
	writel(0, base + JPEG_ENC_CODEC_SEL);
}

u32 mtk_jpeg_enc_get_file_size(void __iomem *base)
{
	return readl(base + JPEG_ENC_DMA_ADDR0) -
	       readl(base + JPEG_ENC_DST_ADDR0);
>>>>>>> upstream/android-13
}

void mtk_jpeg_enc_start(void __iomem *base)
{
<<<<<<< HEAD
	u32 u4Value;

	u4Value = readl(base + JPGENC_CTRL);
	u4Value |= (JPEG_ENC_CTRL_INT_EN_BIT | JPEG_ENC_CTRL_ENABLE_BIT);
	u4Value |= JPEG_ENC_CTRL_RDMA_PADDING_EN;
	u4Value |= JPEG_ENC_CTRL_RDMA_RIGHT_PADDING_EN;
	u4Value &= ~JPEG_ENC_CTRL_RDMA_PADDING_0_EN;

	writel((u4Value), base + JPGENC_CTRL);
}

void mtk_jpeg_enc_set_config(void __iomem *base,
				  struct mtk_jpeg_enc_param *config,
				  struct mtk_jpeg_enc_bs *bs,
				  struct mtk_jpeg_enc_fb *fb)
{
	mtk_jpeg_enc_set_src_img(base, config->enc_w, config->enc_h,
				config->enc_format, config->total_encdu);


	if (((config->enc_format == JPEG_YUV_FORMAT_NV12) ||
		(config->enc_format == JPEG_YUV_FORMAT_NV21)) &&
		fb->num_planes == 1) {

		pr_info("%s set 2 plane by add offset w %d h %d\n", __func__,
			config->img_stride, config->align_h);

		mtk_jpeg_enc_set_src_buf(base, config->img_stride,
			 config->mem_stride,
			 fb->fb_addr[0].dma_addr,
			 fb->fb_addr[0].dma_addr +
			 config->align_h*config->mem_stride);
	} else {
		mtk_jpeg_enc_set_src_buf(base, config->img_stride,
			 config->mem_stride,
			 fb->fb_addr[0].dma_addr,
			 fb->fb_addr[1].dma_addr);
	}



	mtk_jpeg_enc_set_dst_buf(base, bs->dma_addr, bs->size,
				bs->dma_addr_offset, bs->dma_addr_offsetmask);
	mtk_jpeg_enc_set_ctrl_cfg(base, config->enable_exif,
				config->enc_quality, config->restart_interval);
=======
	u32 value;

	value = readl(base + JPEG_ENC_CTRL);
	value |= JPEG_ENC_CTRL_INT_EN_BIT | JPEG_ENC_CTRL_ENABLE_BIT;
	writel(value, base + JPEG_ENC_CTRL);
}

void mtk_jpeg_set_enc_src(struct mtk_jpeg_ctx *ctx,  void __iomem *base,
			  struct vb2_buffer *src_buf)
{
	int i;
	dma_addr_t dma_addr;

	for (i = 0; i < src_buf->num_planes; i++) {
		dma_addr = vb2_dma_contig_plane_dma_addr(src_buf, i) +
			   src_buf->planes[i].data_offset;
		if (!i)
			writel(dma_addr, base + JPEG_ENC_SRC_LUMA_ADDR);
		else
			writel(dma_addr, base + JPEG_ENC_SRC_CHROMA_ADDR);
	}
}

void mtk_jpeg_set_enc_dst(struct mtk_jpeg_ctx *ctx, void __iomem *base,
			  struct vb2_buffer *dst_buf)
{
	dma_addr_t dma_addr;
	size_t size;
	u32 dma_addr_offset;
	u32 dma_addr_offsetmask;

	dma_addr = vb2_dma_contig_plane_dma_addr(dst_buf, 0);
	dma_addr_offset = ctx->enable_exif ? MTK_JPEG_MAX_EXIF_SIZE : 0;
	dma_addr_offsetmask = dma_addr & JPEG_ENC_DST_ADDR_OFFSET_MASK;
	size = vb2_plane_size(dst_buf, 0);

	writel(dma_addr_offset & ~0xf, base + JPEG_ENC_OFFSET_ADDR);
	writel(dma_addr_offsetmask & 0xf, base + JPEG_ENC_BYTE_OFFSET_MASK);
	writel(dma_addr & ~0xf, base + JPEG_ENC_DST_ADDR0);
	writel((dma_addr + size) & ~0xf, base + JPEG_ENC_STALL_ADDR0);
}

void mtk_jpeg_set_enc_params(struct mtk_jpeg_ctx *ctx,  void __iomem *base)
{
	u32 value;
	u32 width = ctx->out_q.enc_crop_rect.width;
	u32 height = ctx->out_q.enc_crop_rect.height;
	u32 enc_format = ctx->out_q.fmt->fourcc;
	u32 bytesperline = ctx->out_q.pix_mp.plane_fmt[0].bytesperline;
	u32 blk_num;
	u32 img_stride;
	u32 mem_stride;
	u32 i, enc_quality;

	value = width << 16 | height;
	writel(value, base + JPEG_ENC_IMG_SIZE);

	if (enc_format == V4L2_PIX_FMT_NV12M ||
	    enc_format == V4L2_PIX_FMT_NV21M)
	    /*
	     * Total 8 x 8 block number of luma and chroma.
	     * The number of blocks is counted from 0.
	     */
		blk_num = DIV_ROUND_UP(width, 16) *
			  DIV_ROUND_UP(height, 16) * 6 - 1;
	else
		blk_num = DIV_ROUND_UP(width, 16) *
			  DIV_ROUND_UP(height, 8) * 4 - 1;
	writel(blk_num, base + JPEG_ENC_BLK_NUM);

	if (enc_format == V4L2_PIX_FMT_NV12M ||
	    enc_format == V4L2_PIX_FMT_NV21M) {
		/* 4:2:0 */
		img_stride = round_up(width, 16);
		mem_stride = bytesperline;
	} else {
		/* 4:2:2 */
		img_stride = round_up(width * 2, 32);
		mem_stride = img_stride;
	}
	writel(img_stride, base + JPEG_ENC_IMG_STRIDE);
	writel(mem_stride, base + JPEG_ENC_STRIDE);

	enc_quality = mtk_jpeg_enc_quality[0].hardware_value;
	for (i = 0; i < ARRAY_SIZE(mtk_jpeg_enc_quality); i++) {
		if (ctx->enc_quality <= mtk_jpeg_enc_quality[i].quality_param) {
			enc_quality = mtk_jpeg_enc_quality[i].hardware_value;
			break;
		}
	}
	writel(enc_quality, base + JPEG_ENC_QUALITY);

	value = readl(base + JPEG_ENC_CTRL);
	value &= ~JPEG_ENC_CTRL_YUV_FORMAT_MASK;
	value |= (ctx->out_q.fmt->hw_format & 3) << 3;
	if (ctx->enable_exif)
		value |= JPEG_ENC_CTRL_FILE_FORMAT_BIT;
	else
		value &= ~JPEG_ENC_CTRL_FILE_FORMAT_BIT;
	if (ctx->restart_interval)
		value |= JPEG_ENC_CTRL_RESTART_EN_BIT;
	else
		value &= ~JPEG_ENC_CTRL_RESTART_EN_BIT;
	writel(value, base + JPEG_ENC_CTRL);

	writel(ctx->restart_interval, base + JPEG_ENC_RST_MCU_NUM);
>>>>>>> upstream/android-13
}
