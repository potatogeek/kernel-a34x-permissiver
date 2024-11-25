<<<<<<< HEAD
/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Copyright (c) 2019 MediaTek Inc.
=======
/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Copyright (c) 2016 MediaTek Inc.
 * Author: Ming Hsiu Tsai <minghsiu.tsai@mediatek.com>
 *         Rick Chang <rick.chang@mediatek.com>
 *         Xia Jiang <xia.jiang@mediatek.com>
>>>>>>> upstream/android-13
 */

#ifndef _MTK_JPEG_CORE_H
#define _MTK_JPEG_CORE_H

#include <linux/interrupt.h>
<<<<<<< HEAD
#include <linux/semaphore.h>
#include <media/v4l2-ctrls.h>
#include <media/v4l2-device.h>
#include <media/v4l2-fh.h>
#include "mmdvfs_pmqos.h"


#define MTK_JPEG_NAME		"mtk-jpeg"

#define MTK_JPEG_FMT_FLAG_DEC_OUTPUT	BIT(0)
#define MTK_JPEG_FMT_FLAG_DEC_CAPTURE	BIT(1)
#define MTK_JPEG_FMT_FLAG_ENC_OUTPUT	BIT(2)
#define MTK_JPEG_FMT_FLAG_ENC_CAPTURE	BIT(3)

#define MTK_JPEG_FMT_TYPE_OUTPUT	1
#define MTK_JPEG_FMT_TYPE_CAPTURE	2

#define MTK_JPEG_MIN_WIDTH	32
#define MTK_JPEG_MIN_HEIGHT	32
#define MTK_JPEG_MAX_WIDTH	65536
#define MTK_JPEG_MAX_HEIGHT	65536

#define MTK_JPEG_DEFAULT_SIZEIMAGE	(1 * 1024 * 1024)

#define MTK_JPEG_ENCODE		0
#define MTK_JPEG_DECODE		1

#define MTK_JPEG_MAX_NCORE       2

#define MTK_JPEG_PORT_INDEX_YRDMA  0
#define MTK_JPEG_PORT_INDEX_CRDMA  1
#define MTK_JPEG_PORT_INDEX_QTBLE  2
#define MTK_JPEG_PORT_INDEX_BSDMA  3



/**
 * enum mtk_jpeg_ctx_state - contex state of jpeg
=======
#include <media/v4l2-ctrls.h>
#include <media/v4l2-device.h>
#include <media/v4l2-fh.h>

#define MTK_JPEG_NAME		"mtk-jpeg"

#define MTK_JPEG_COMP_MAX		3

#define MTK_JPEG_FMT_FLAG_OUTPUT	BIT(0)
#define MTK_JPEG_FMT_FLAG_CAPTURE	BIT(1)

#define MTK_JPEG_MIN_WIDTH	32U
#define MTK_JPEG_MIN_HEIGHT	32U
#define MTK_JPEG_MAX_WIDTH	65535U
#define MTK_JPEG_MAX_HEIGHT	65535U

#define MTK_JPEG_DEFAULT_SIZEIMAGE	(1 * 1024 * 1024)

#define MTK_JPEG_HW_TIMEOUT_MSEC 1000

#define MTK_JPEG_MAX_EXIF_SIZE	(64 * 1024)

/**
 * enum mtk_jpeg_ctx_state - states of the context state machine
 * @MTK_JPEG_INIT:		current state is initialized
 * @MTK_JPEG_RUNNING:		current state is running
 * @MTK_JPEG_SOURCE_CHANGE:	current state is source resolution change
>>>>>>> upstream/android-13
 */
enum mtk_jpeg_ctx_state {
	MTK_JPEG_INIT = 0,
	MTK_JPEG_RUNNING,
	MTK_JPEG_SOURCE_CHANGE,
};

/**
<<<<<<< HEAD
 * enum mtk_jpeg_mode - mode of jpeg
 */
enum mtk_jpeg_mode {
	MTK_JPEG_ENC,
	MTK_JPEG_DEC,
};

/**
 * enum jpeg_enc_yuv_fmt - yuv format of jpeg enc
 */
enum jpeg_enc_yuv_fmt {
	JPEG_YUV_FORMAT_YUYV = 0,
	JPEG_YUV_FORMAT_YVYU = 1,
	JPEG_YUV_FORMAT_NV12 = 2,
	JPEG_YUV_FORMAT_NV21 = 3,
};

/**
 * enum JPEG_ENCODE_QUALITY_ENUM - number of jpeg encoder quality
 */
enum JPEG_ENCODE_QUALITY_ENUM {
	JPEG_ENCODE_QUALITY_Q38 = 0x0,
	JPEG_ENCODE_QUALITY_Q60 = 0x1,
	JPEG_ENCODE_QUALITY_Q85 = 0x2,
	JPEG_ENCODE_QUALITY_Q95 = 0x3,
	JPEG_ENCODE_QUALITY_Q30 = 0x4,
	JPEG_ENCODE_QUALITY_Q66 = 0x5,
	JPEG_ENCODE_QUALITY_Q78 = 0x6,
	JPEG_ENCODE_QUALITY_Q90 = 0x7,
	JPEG_ENCODE_QUALITY_Q34 = 0x8,
	JPEG_ENCODE_QUALITY_Q52 = 0xA,
	JPEG_ENCODE_QUALITY_Q82 = 0xB,
	JPEG_ENCODE_QUALITY_Q24 = 0xC,
	JPEG_ENCODE_QUALITY_Q44 = 0xE,
	JPEG_ENCODE_QUALITY_Q72 = 0xF,
	JPEG_ENCODE_QUALITY_Q97 = 0x10,
	JPEG_ENCODE_QUALITY_ALL = 0xFFFFFFFF
};

/**
 * mtk_jpeg_variant - mtk jpeg driver variant
 * @is_encoder:		driver mode is jpeg encoder
 * @clk_names:		clock names
 * @num_clocks:		numbers of clock
 */
struct mtk_jpeg_variant {
	unsigned int jpeg_mode;
};

/**
 * struct mt_jpeg - JPEG IP abstraction
=======
 * struct mtk_jpeg_variant - mtk jpeg driver variant
 * @clks:			clock names
 * @num_clks:			numbers of clock
 * @formats:			jpeg driver's internal color format
 * @num_formats:		number of formats
 * @qops:			the callback of jpeg vb2_ops
 * @irq_handler:		jpeg irq handler callback
 * @hw_reset:			jpeg hardware reset callback
 * @m2m_ops:			the callback of jpeg v4l2_m2m_ops
 * @dev_name:			jpeg device name
 * @ioctl_ops:			the callback of jpeg v4l2_ioctl_ops
 * @out_q_default_fourcc:	output queue default fourcc
 * @cap_q_default_fourcc:	capture queue default fourcc
 */
struct mtk_jpeg_variant {
	struct clk_bulk_data *clks;
	int num_clks;
	struct mtk_jpeg_fmt *formats;
	int num_formats;
	const struct vb2_ops *qops;
	irqreturn_t (*irq_handler)(int irq, void *priv);
	void (*hw_reset)(void __iomem *base);
	const struct v4l2_m2m_ops *m2m_ops;
	const char *dev_name;
	const struct v4l2_ioctl_ops *ioctl_ops;
	u32 out_q_default_fourcc;
	u32 cap_q_default_fourcc;
};

/**
 * struct mtk_jpeg_dev - JPEG IP abstraction
>>>>>>> upstream/android-13
 * @lock:		the mutex protecting this structure
 * @hw_lock:		spinlock protecting the hw device resource
 * @workqueue:		decode work queue
 * @dev:		JPEG device
 * @v4l2_dev:		v4l2 device for mem2mem mode
 * @m2m_dev:		v4l2 mem2mem device data
 * @alloc_ctx:		videobuf2 memory allocator's context
<<<<<<< HEAD
 * @vfd_jpeg:		video device node for jpeg mem2mem mode
 * @reg_base:		JPEG registers mapping
 * @clk_jpeg:		JPEG hw working clock
 * @clk_jpeg_smi:	JPEG SMI bus clock
 * @larb:		SMI device
 * @mode:		compression (encode) operation or decompression (decode)
 */
struct mtk_jpeg_dev {
	struct mutex		lock;
	struct semaphore    sem;
	spinlock_t		hw_lock[MTK_JPEG_MAX_NCORE];
=======
 * @vdev:		video device node for jpeg mem2mem mode
 * @reg_base:		JPEG registers mapping
 * @larb:		SMI device
 * @job_timeout_work:	IRQ timeout structure
 * @variant:		driver variant to be used
 */
struct mtk_jpeg_dev {
	struct mutex		lock;
	spinlock_t		hw_lock;
>>>>>>> upstream/android-13
	struct workqueue_struct	*workqueue;
	struct device		*dev;
	struct v4l2_device	v4l2_dev;
	struct v4l2_m2m_dev	*m2m_dev;
	void			*alloc_ctx;
<<<<<<< HEAD
	struct video_device	*vfd_jpeg;
	void __iomem		*reg_base[MTK_JPEG_MAX_NCORE];
	struct clk		*clk_jpeg[MTK_JPEG_MAX_NCORE];
	struct clk		*clk_jpeg_smi;
	struct device		*larb[MTK_JPEG_MAX_NCORE];
	enum mtk_jpeg_mode  mode;
	u32			larb_id[MTK_JPEG_MAX_NCORE];
	u32			ncore;
	u32         isused[MTK_JPEG_MAX_NCORE];
	u32         port_y_rdma[MTK_JPEG_MAX_NCORE];
	u32         port_c_rdma[MTK_JPEG_MAX_NCORE];
	u32         port_qtbl[MTK_JPEG_MAX_NCORE];
	u32         port_bsdma[MTK_JPEG_MAX_NCORE];
	int         irq[MTK_JPEG_MAX_NCORE];
	struct plist_head jpegenc_rlist;
	struct mm_qos_request jpeg_y_rdma;
	struct mm_qos_request jpeg_c_rdma;
	struct mm_qos_request jpeg_qtbl;
	struct mm_qos_request jpeg_bsdma;
=======
	struct video_device	*vdev;
	void __iomem		*reg_base;
	struct device		*larb;
	struct delayed_work job_timeout_work;
>>>>>>> upstream/android-13
	const struct mtk_jpeg_variant *variant;
};

/**
<<<<<<< HEAD
 * struct jpeg_fmt - driver's internal color format data
 * @fourcc:	the fourcc code, 0 if not applicable
=======
 * struct mtk_jpeg_fmt - driver's internal color format data
 * @fourcc:	the fourcc code, 0 if not applicable
 * @hw_format:	hardware format value
>>>>>>> upstream/android-13
 * @h_sample:	horizontal sample count of plane in 4 * 4 pixel image
 * @v_sample:	vertical sample count of plane in 4 * 4 pixel image
 * @colplanes:	number of color planes (1 for packed formats)
 * @h_align:	horizontal alignment order (align to 2^h_align)
 * @v_align:	vertical alignment order (align to 2^v_align)
 * @flags:	flags describing format applicability
 */
struct mtk_jpeg_fmt {
	u32	fourcc;
<<<<<<< HEAD
=======
	u32	hw_format;
>>>>>>> upstream/android-13
	int	h_sample[VIDEO_MAX_PLANES];
	int	v_sample[VIDEO_MAX_PLANES];
	int	colplanes;
	int	h_align;
	int	v_align;
	u32	flags;
};

/**
<<<<<<< HEAD
 * mtk_jpeg_q_data - parameters of one queue
 * @fmt:	  driver-specific format of this queue
 * @w:		  image width
 * @h:		  image height
 * @bytesperline: distance in bytes between the leftmost pixels in two adjacent
 *                lines
 * @sizeimage:	  image buffer size in bytes
 */
struct mtk_jpeg_q_data {
	struct mtk_jpeg_fmt	*fmt;
	u32			w;
	u32			h;
	u32			bytesperline[VIDEO_MAX_PLANES];
	u32			sizeimage[VIDEO_MAX_PLANES];
	u32			align_h;
};

/**
 * jpeg_enc_param - parameters of jpeg encode control
 * @enable_exif:	EXIF enable for jpeg encode mode
 * @enc_quality:	destination image quality in encode mode
 * @restart_interval:	JPEG restart interval for JPEG encoding
 */
struct jpeg_enc_param {
	u32 enable_exif;
	u32 enc_quality;
	u32 restart_interval;
	u32 dst_offset;
};

/**
 * mtk_jpeg_enc_param:  General jpeg encoding parameters
 * @enc_w:		image width
 * @enc_h:		image height
 * @enable_exif:	EXIF enable for jpeg encode mode
 * @enc_quality:	destination image quality in encode mode
 * @enc_format:		input image format
 * @restart_interval:	JPEG restart interval for JPEG encoding
 * @img_stride:		jpeg encoder image stride
 * @mem_stride:		jpeg encoder memory stride
 * @total_encdu:	total 8x8 block number
 */
struct mtk_jpeg_enc_param {
	u32 enc_w;
	u32 enc_h;
	u32 enable_exif;
	u32 enc_quality;
	u32 enc_format;
	u32 restart_interval;
	u32 img_stride;
	u32 mem_stride;
	u32 total_encdu;
	u32 align_h;
};
/**
 * mtk_jpeg_ctx - the device context data
=======
 * struct mtk_jpeg_q_data - parameters of one queue
 * @fmt:	  driver-specific format of this queue
 * @pix_mp:	  multiplanar format
 * @enc_crop_rect:	jpeg encoder crop information
 */
struct mtk_jpeg_q_data {
	struct mtk_jpeg_fmt	*fmt;
	struct v4l2_pix_format_mplane pix_mp;
	struct v4l2_rect enc_crop_rect;
};

/**
 * struct mtk_jpeg_ctx - the device context data
>>>>>>> upstream/android-13
 * @jpeg:		JPEG IP device for this context
 * @out_q:		source (output) queue information
 * @cap_q:		destination (capture) queue queue information
 * @fh:			V4L2 file handle
 * @state:		state of the context
<<<<<<< HEAD
 * @jpeg_param:		jpeg encode parameters
 * @ctrl_hdl:		controls handler
 * @colorspace: enum v4l2_colorspace; supplemental to pixelformat
 * @ycbcr_enc: enum v4l2_ycbcr_encoding, Y'CbCr encoding
 * @quantization: enum v4l2_quantization, colorspace quantization
 * @xfer_func: enum v4l2_xfer_func, colorspace transfer function
=======
 * @enable_exif:	enable exif mode of jpeg encoder
 * @enc_quality:	jpeg encoder quality
 * @restart_interval:	jpeg encoder restart interval
 * @ctrl_hdl:		controls handler
>>>>>>> upstream/android-13
 */
struct mtk_jpeg_ctx {
	struct mtk_jpeg_dev		*jpeg;
	struct mtk_jpeg_q_data		out_q;
	struct mtk_jpeg_q_data		cap_q;
	struct v4l2_fh			fh;
	enum mtk_jpeg_ctx_state		state;
<<<<<<< HEAD
	struct jpeg_enc_param		jpeg_param;
	struct v4l2_ctrl_handler	ctrl_hdl;

	enum v4l2_colorspace colorspace;
	enum v4l2_ycbcr_encoding ycbcr_enc;
	enum v4l2_quantization quantization;
	enum v4l2_xfer_func xfer_func;
	u32  coreid;
=======
	bool enable_exif;
	u8 enc_quality;
	u8 restart_interval;
	struct v4l2_ctrl_handler ctrl_hdl;
>>>>>>> upstream/android-13
};

#endif /* _MTK_JPEG_CORE_H */
