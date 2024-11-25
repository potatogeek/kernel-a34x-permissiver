<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0+
>>>>>>> upstream/android-13
/*
 * V4L2 Media Controller Driver for Freescale i.MX5/6 SOC
 *
 * Copyright (c) 2016 Mentor Graphics Inc.
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
=======
>>>>>>> upstream/android-13
 */
#include <linux/module.h>
#include "imx-media.h"

<<<<<<< HEAD
/*
 * List of supported pixel formats for the subdevs.
 *
 * In all of these tables, the non-mbus formats (with no
 * mbus codes) must all fall at the end of the table.
 */

static const struct imx_media_pixfmt yuv_formats[] = {
	{
		.fourcc	= V4L2_PIX_FMT_UYVY,
		.codes  = {
			MEDIA_BUS_FMT_UYVY8_2X8,
			MEDIA_BUS_FMT_UYVY8_1X16
		},
=======
#define IMX_BUS_FMTS(fmt...) (const u32[]) {fmt, 0}

/*
 * List of supported pixel formats for the subdevs.
 */
static const struct imx_media_pixfmt pixel_formats[] = {
	/*** YUV formats start here ***/
	{
		.fourcc	= V4L2_PIX_FMT_UYVY,
		.codes  = IMX_BUS_FMTS(
			MEDIA_BUS_FMT_UYVY8_2X8,
			MEDIA_BUS_FMT_UYVY8_1X16
		),
>>>>>>> upstream/android-13
		.cs     = IPUV3_COLORSPACE_YUV,
		.bpp    = 16,
	}, {
		.fourcc	= V4L2_PIX_FMT_YUYV,
<<<<<<< HEAD
		.codes  = {
			MEDIA_BUS_FMT_YUYV8_2X8,
			MEDIA_BUS_FMT_YUYV8_1X16
		},
		.cs     = IPUV3_COLORSPACE_YUV,
		.bpp    = 16,
	},
	/***
	 * non-mbus YUV formats start here. NOTE! when adding non-mbus
	 * formats, NUM_NON_MBUS_YUV_FORMATS must be updated below.
	 ***/
	{
=======
		.codes  = IMX_BUS_FMTS(
			MEDIA_BUS_FMT_YUYV8_2X8,
			MEDIA_BUS_FMT_YUYV8_1X16
		),
		.cs     = IPUV3_COLORSPACE_YUV,
		.bpp    = 16,
	}, {
>>>>>>> upstream/android-13
		.fourcc	= V4L2_PIX_FMT_YUV420,
		.cs     = IPUV3_COLORSPACE_YUV,
		.bpp    = 12,
		.planar = true,
	}, {
		.fourcc = V4L2_PIX_FMT_YVU420,
		.cs     = IPUV3_COLORSPACE_YUV,
		.bpp    = 12,
		.planar = true,
	}, {
		.fourcc = V4L2_PIX_FMT_YUV422P,
		.cs     = IPUV3_COLORSPACE_YUV,
		.bpp    = 16,
		.planar = true,
	}, {
		.fourcc = V4L2_PIX_FMT_NV12,
		.cs     = IPUV3_COLORSPACE_YUV,
		.bpp    = 12,
		.planar = true,
	}, {
		.fourcc = V4L2_PIX_FMT_NV16,
		.cs     = IPUV3_COLORSPACE_YUV,
		.bpp    = 16,
		.planar = true,
<<<<<<< HEAD
	},
};

#define NUM_NON_MBUS_YUV_FORMATS 5
#define NUM_YUV_FORMATS ARRAY_SIZE(yuv_formats)
#define NUM_MBUS_YUV_FORMATS (NUM_YUV_FORMATS - NUM_NON_MBUS_YUV_FORMATS)

static const struct imx_media_pixfmt rgb_formats[] = {
	{
		.fourcc	= V4L2_PIX_FMT_RGB565,
		.codes  = {MEDIA_BUS_FMT_RGB565_2X8_LE},
=======
	}, {
		.fourcc = V4L2_PIX_FMT_YUV32,
		.codes  = IMX_BUS_FMTS(MEDIA_BUS_FMT_AYUV8_1X32),
		.cs     = IPUV3_COLORSPACE_YUV,
		.bpp    = 32,
		.ipufmt = true,
	},
	/*** RGB formats start here ***/
	{
		.fourcc	= V4L2_PIX_FMT_RGB565,
		.codes  = IMX_BUS_FMTS(MEDIA_BUS_FMT_RGB565_2X8_LE),
>>>>>>> upstream/android-13
		.cs     = IPUV3_COLORSPACE_RGB,
		.bpp    = 16,
		.cycles = 2,
	}, {
		.fourcc	= V4L2_PIX_FMT_RGB24,
<<<<<<< HEAD
		.codes  = {
			MEDIA_BUS_FMT_RGB888_1X24,
			MEDIA_BUS_FMT_RGB888_2X12_LE
		},
		.cs     = IPUV3_COLORSPACE_RGB,
		.bpp    = 24,
	}, {
		.fourcc	= V4L2_PIX_FMT_RGB32,
		.codes  = {MEDIA_BUS_FMT_ARGB8888_1X32},
		.cs     = IPUV3_COLORSPACE_RGB,
		.bpp    = 32,
		.ipufmt = true,
=======
		.codes  = IMX_BUS_FMTS(
			MEDIA_BUS_FMT_RGB888_1X24,
			MEDIA_BUS_FMT_RGB888_2X12_LE
		),
		.cs     = IPUV3_COLORSPACE_RGB,
		.bpp    = 24,
	}, {
		.fourcc	= V4L2_PIX_FMT_BGR24,
		.cs     = IPUV3_COLORSPACE_RGB,
		.bpp    = 24,
	}, {
		.fourcc	= V4L2_PIX_FMT_XRGB32,
		.codes  = IMX_BUS_FMTS(MEDIA_BUS_FMT_ARGB8888_1X32),
		.cs     = IPUV3_COLORSPACE_RGB,
		.bpp    = 32,
	}, {
		.fourcc	= V4L2_PIX_FMT_XRGB32,
		.codes  = IMX_BUS_FMTS(MEDIA_BUS_FMT_ARGB8888_1X32),
		.cs     = IPUV3_COLORSPACE_RGB,
		.bpp    = 32,
		.ipufmt = true,
	}, {
		.fourcc	= V4L2_PIX_FMT_XBGR32,
		.cs     = IPUV3_COLORSPACE_RGB,
		.bpp    = 32,
	}, {
		.fourcc	= V4L2_PIX_FMT_BGRX32,
		.cs     = IPUV3_COLORSPACE_RGB,
		.bpp    = 32,
	}, {
		.fourcc	= V4L2_PIX_FMT_RGBX32,
		.cs     = IPUV3_COLORSPACE_RGB,
		.bpp    = 32,
>>>>>>> upstream/android-13
	},
	/*** raw bayer and grayscale formats start here ***/
	{
		.fourcc = V4L2_PIX_FMT_SBGGR8,
<<<<<<< HEAD
		.codes  = {MEDIA_BUS_FMT_SBGGR8_1X8},
=======
		.codes  = IMX_BUS_FMTS(MEDIA_BUS_FMT_SBGGR8_1X8),
>>>>>>> upstream/android-13
		.cs     = IPUV3_COLORSPACE_RGB,
		.bpp    = 8,
		.bayer  = true,
	}, {
		.fourcc = V4L2_PIX_FMT_SGBRG8,
<<<<<<< HEAD
		.codes  = {MEDIA_BUS_FMT_SGBRG8_1X8},
=======
		.codes  = IMX_BUS_FMTS(MEDIA_BUS_FMT_SGBRG8_1X8),
>>>>>>> upstream/android-13
		.cs     = IPUV3_COLORSPACE_RGB,
		.bpp    = 8,
		.bayer  = true,
	}, {
		.fourcc = V4L2_PIX_FMT_SGRBG8,
<<<<<<< HEAD
		.codes  = {MEDIA_BUS_FMT_SGRBG8_1X8},
=======
		.codes  = IMX_BUS_FMTS(MEDIA_BUS_FMT_SGRBG8_1X8),
>>>>>>> upstream/android-13
		.cs     = IPUV3_COLORSPACE_RGB,
		.bpp    = 8,
		.bayer  = true,
	}, {
		.fourcc = V4L2_PIX_FMT_SRGGB8,
<<<<<<< HEAD
		.codes  = {MEDIA_BUS_FMT_SRGGB8_1X8},
=======
		.codes  = IMX_BUS_FMTS(MEDIA_BUS_FMT_SRGGB8_1X8),
>>>>>>> upstream/android-13
		.cs     = IPUV3_COLORSPACE_RGB,
		.bpp    = 8,
		.bayer  = true,
	}, {
		.fourcc = V4L2_PIX_FMT_SBGGR16,
<<<<<<< HEAD
		.codes  = {
=======
		.codes  = IMX_BUS_FMTS(
>>>>>>> upstream/android-13
			MEDIA_BUS_FMT_SBGGR10_1X10,
			MEDIA_BUS_FMT_SBGGR12_1X12,
			MEDIA_BUS_FMT_SBGGR14_1X14,
			MEDIA_BUS_FMT_SBGGR16_1X16
<<<<<<< HEAD
		},
=======
		),
>>>>>>> upstream/android-13
		.cs     = IPUV3_COLORSPACE_RGB,
		.bpp    = 16,
		.bayer  = true,
	}, {
		.fourcc = V4L2_PIX_FMT_SGBRG16,
<<<<<<< HEAD
		.codes  = {
			MEDIA_BUS_FMT_SGBRG10_1X10,
			MEDIA_BUS_FMT_SGBRG12_1X12,
			MEDIA_BUS_FMT_SGBRG14_1X14,
			MEDIA_BUS_FMT_SGBRG16_1X16,
		},
=======
		.codes  = IMX_BUS_FMTS(
			MEDIA_BUS_FMT_SGBRG10_1X10,
			MEDIA_BUS_FMT_SGBRG12_1X12,
			MEDIA_BUS_FMT_SGBRG14_1X14,
			MEDIA_BUS_FMT_SGBRG16_1X16
		),
>>>>>>> upstream/android-13
		.cs     = IPUV3_COLORSPACE_RGB,
		.bpp    = 16,
		.bayer  = true,
	}, {
		.fourcc = V4L2_PIX_FMT_SGRBG16,
<<<<<<< HEAD
		.codes  = {
			MEDIA_BUS_FMT_SGRBG10_1X10,
			MEDIA_BUS_FMT_SGRBG12_1X12,
			MEDIA_BUS_FMT_SGRBG14_1X14,
			MEDIA_BUS_FMT_SGRBG16_1X16,
		},
=======
		.codes  = IMX_BUS_FMTS(
			MEDIA_BUS_FMT_SGRBG10_1X10,
			MEDIA_BUS_FMT_SGRBG12_1X12,
			MEDIA_BUS_FMT_SGRBG14_1X14,
			MEDIA_BUS_FMT_SGRBG16_1X16
		),
>>>>>>> upstream/android-13
		.cs     = IPUV3_COLORSPACE_RGB,
		.bpp    = 16,
		.bayer  = true,
	}, {
		.fourcc = V4L2_PIX_FMT_SRGGB16,
<<<<<<< HEAD
		.codes  = {
			MEDIA_BUS_FMT_SRGGB10_1X10,
			MEDIA_BUS_FMT_SRGGB12_1X12,
			MEDIA_BUS_FMT_SRGGB14_1X14,
			MEDIA_BUS_FMT_SRGGB16_1X16,
		},
=======
		.codes  = IMX_BUS_FMTS(
			MEDIA_BUS_FMT_SRGGB10_1X10,
			MEDIA_BUS_FMT_SRGGB12_1X12,
			MEDIA_BUS_FMT_SRGGB14_1X14,
			MEDIA_BUS_FMT_SRGGB16_1X16
		),
>>>>>>> upstream/android-13
		.cs     = IPUV3_COLORSPACE_RGB,
		.bpp    = 16,
		.bayer  = true,
	}, {
		.fourcc = V4L2_PIX_FMT_GREY,
<<<<<<< HEAD
		.codes = {MEDIA_BUS_FMT_Y8_1X8},
=======
		.codes = IMX_BUS_FMTS(
			MEDIA_BUS_FMT_Y8_1X8,
			MEDIA_BUS_FMT_Y10_1X10,
			MEDIA_BUS_FMT_Y12_1X12
		),
>>>>>>> upstream/android-13
		.cs     = IPUV3_COLORSPACE_RGB,
		.bpp    = 8,
		.bayer  = true,
	}, {
<<<<<<< HEAD
		.fourcc = V4L2_PIX_FMT_Y16,
		.codes = {
			MEDIA_BUS_FMT_Y10_1X10,
			MEDIA_BUS_FMT_Y12_1X12,
		},
=======
		.fourcc = V4L2_PIX_FMT_Y10,
		.codes = IMX_BUS_FMTS(MEDIA_BUS_FMT_Y10_1X10),
		.cs     = IPUV3_COLORSPACE_RGB,
		.bpp    = 16,
		.bayer  = true,
	}, {
		.fourcc = V4L2_PIX_FMT_Y12,
		.codes = IMX_BUS_FMTS(MEDIA_BUS_FMT_Y12_1X12),
>>>>>>> upstream/android-13
		.cs     = IPUV3_COLORSPACE_RGB,
		.bpp    = 16,
		.bayer  = true,
	},
<<<<<<< HEAD
	/***
	 * non-mbus RGB formats start here. NOTE! when adding non-mbus
	 * formats, NUM_NON_MBUS_RGB_FORMATS must be updated below.
	 ***/
	{
		.fourcc	= V4L2_PIX_FMT_BGR24,
		.cs     = IPUV3_COLORSPACE_RGB,
		.bpp    = 24,
	}, {
		.fourcc	= V4L2_PIX_FMT_BGR32,
		.cs     = IPUV3_COLORSPACE_RGB,
		.bpp    = 32,
	},
};

#define NUM_NON_MBUS_RGB_FORMATS 2
#define NUM_RGB_FORMATS ARRAY_SIZE(rgb_formats)
#define NUM_MBUS_RGB_FORMATS (NUM_RGB_FORMATS - NUM_NON_MBUS_RGB_FORMATS)

static const struct imx_media_pixfmt ipu_yuv_formats[] = {
	{
		.fourcc = V4L2_PIX_FMT_YUV32,
		.codes  = {MEDIA_BUS_FMT_AYUV8_1X32},
		.cs     = IPUV3_COLORSPACE_YUV,
		.bpp    = 32,
		.ipufmt = true,
	},
};

#define NUM_IPU_YUV_FORMATS ARRAY_SIZE(ipu_yuv_formats)

static const struct imx_media_pixfmt ipu_rgb_formats[] = {
	{
		.fourcc	= V4L2_PIX_FMT_RGB32,
		.codes  = {MEDIA_BUS_FMT_ARGB8888_1X32},
		.cs     = IPUV3_COLORSPACE_RGB,
		.bpp    = 32,
		.ipufmt = true,
	},
};

#define NUM_IPU_RGB_FORMATS ARRAY_SIZE(ipu_rgb_formats)

static void init_mbus_colorimetry(struct v4l2_mbus_framefmt *mbus,
				  const struct imx_media_pixfmt *fmt)
{
	mbus->colorspace = (fmt->cs == IPUV3_COLORSPACE_RGB) ?
		V4L2_COLORSPACE_SRGB : V4L2_COLORSPACE_SMPTE170M;
	mbus->xfer_func = V4L2_MAP_XFER_FUNC_DEFAULT(mbus->colorspace);
	mbus->ycbcr_enc = V4L2_MAP_YCBCR_ENC_DEFAULT(mbus->colorspace);
	mbus->quantization =
		V4L2_MAP_QUANTIZATION_DEFAULT(fmt->cs == IPUV3_COLORSPACE_RGB,
					      mbus->colorspace,
					      mbus->ycbcr_enc);
}

static const
struct imx_media_pixfmt *__find_format(u32 fourcc,
				       u32 code,
				       bool allow_non_mbus,
				       bool allow_bayer,
				       const struct imx_media_pixfmt *array,
				       u32 array_size)
{
	const struct imx_media_pixfmt *fmt;
	int i, j;

	for (i = 0; i < array_size; i++) {
		fmt = &array[i];

		if ((!allow_non_mbus && !fmt->codes[0]) ||
		    (!allow_bayer && fmt->bayer))
			continue;

		if (fourcc && fmt->fourcc == fourcc)
			return fmt;

		if (!code)
=======
};

/*
 * Search in the pixel_formats[] array for an entry with the given fourcc
 * that matches the requested selection criteria and return it.
 *
 * @fourcc: Search for an entry with the given fourcc pixel format.
 * @fmt_sel: Allow entries only with the given selection criteria.
 */
const struct imx_media_pixfmt *
imx_media_find_pixel_format(u32 fourcc, enum imx_pixfmt_sel fmt_sel)
{
	bool sel_ipu = fmt_sel & PIXFMT_SEL_IPU;
	unsigned int i;

	fmt_sel &= ~PIXFMT_SEL_IPU;

	for (i = 0; i < ARRAY_SIZE(pixel_formats); i++) {
		const struct imx_media_pixfmt *fmt = &pixel_formats[i];
		enum imx_pixfmt_sel sel;

		if (sel_ipu != fmt->ipufmt)
			continue;

		sel = fmt->bayer ? PIXFMT_SEL_BAYER :
			((fmt->cs == IPUV3_COLORSPACE_YUV) ?
			 PIXFMT_SEL_YUV : PIXFMT_SEL_RGB);

		if ((fmt_sel & sel) && fmt->fourcc == fourcc)
			return fmt;
	}

	return NULL;
}
EXPORT_SYMBOL_GPL(imx_media_find_pixel_format);

/*
 * Search in the pixel_formats[] array for an entry with the given media
 * bus code that matches the requested selection criteria and return it.
 *
 * @code: Search for an entry with the given media-bus code.
 * @fmt_sel: Allow entries only with the given selection criteria.
 */
const struct imx_media_pixfmt *
imx_media_find_mbus_format(u32 code, enum imx_pixfmt_sel fmt_sel)
{
	bool sel_ipu = fmt_sel & PIXFMT_SEL_IPU;
	unsigned int i;

	fmt_sel &= ~PIXFMT_SEL_IPU;

	for (i = 0; i < ARRAY_SIZE(pixel_formats); i++) {
		const struct imx_media_pixfmt *fmt = &pixel_formats[i];
		enum imx_pixfmt_sel sel;
		unsigned int j;

		if (sel_ipu != fmt->ipufmt)
			continue;

		sel = fmt->bayer ? PIXFMT_SEL_BAYER :
			((fmt->cs == IPUV3_COLORSPACE_YUV) ?
			 PIXFMT_SEL_YUV : PIXFMT_SEL_RGB);

		if (!(fmt_sel & sel) || !fmt->codes)
>>>>>>> upstream/android-13
			continue;

		for (j = 0; fmt->codes[j]; j++) {
			if (code == fmt->codes[j])
				return fmt;
		}
	}
<<<<<<< HEAD
	return NULL;
}

static const struct imx_media_pixfmt *find_format(u32 fourcc,
						  u32 code,
						  enum codespace_sel cs_sel,
						  bool allow_non_mbus,
						  bool allow_bayer)
{
	const struct imx_media_pixfmt *ret;

	switch (cs_sel) {
	case CS_SEL_YUV:
		return __find_format(fourcc, code, allow_non_mbus, allow_bayer,
				     yuv_formats, NUM_YUV_FORMATS);
	case CS_SEL_RGB:
		return __find_format(fourcc, code, allow_non_mbus, allow_bayer,
				     rgb_formats, NUM_RGB_FORMATS);
	case CS_SEL_ANY:
		ret = __find_format(fourcc, code, allow_non_mbus, allow_bayer,
				    yuv_formats, NUM_YUV_FORMATS);
		if (ret)
			return ret;
		return __find_format(fourcc, code, allow_non_mbus, allow_bayer,
				     rgb_formats, NUM_RGB_FORMATS);
	default:
		return NULL;
	}
}

static int enum_format(u32 *fourcc, u32 *code, u32 index,
		       enum codespace_sel cs_sel,
		       bool allow_non_mbus,
		       bool allow_bayer)
{
	const struct imx_media_pixfmt *fmt;
	u32 mbus_yuv_sz = NUM_MBUS_YUV_FORMATS;
	u32 mbus_rgb_sz = NUM_MBUS_RGB_FORMATS;
	u32 yuv_sz = NUM_YUV_FORMATS;
	u32 rgb_sz = NUM_RGB_FORMATS;

	switch (cs_sel) {
	case CS_SEL_YUV:
		if (index >= yuv_sz ||
		    (!allow_non_mbus && index >= mbus_yuv_sz))
			return -EINVAL;
		fmt = &yuv_formats[index];
		break;
	case CS_SEL_RGB:
		if (index >= rgb_sz ||
		    (!allow_non_mbus && index >= mbus_rgb_sz))
			return -EINVAL;
		fmt = &rgb_formats[index];
		if (!allow_bayer && fmt->bayer)
			return -EINVAL;
		break;
	case CS_SEL_ANY:
		if (!allow_non_mbus) {
			if (index >= mbus_yuv_sz) {
				index -= mbus_yuv_sz;
				if (index >= mbus_rgb_sz)
					return -EINVAL;
				fmt = &rgb_formats[index];
				if (!allow_bayer && fmt->bayer)
					return -EINVAL;
			} else {
				fmt = &yuv_formats[index];
			}
		} else {
			if (index >= yuv_sz + rgb_sz)
				return -EINVAL;
			if (index >= yuv_sz) {
				fmt = &rgb_formats[index - yuv_sz];
				if (!allow_bayer && fmt->bayer)
					return -EINVAL;
			} else {
				fmt = &yuv_formats[index];
			}
		}
		break;
	default:
		return -EINVAL;
	}

	if (fourcc)
		*fourcc = fmt->fourcc;
	if (code)
		*code = fmt->codes[0];

	return 0;
}

const struct imx_media_pixfmt *
imx_media_find_format(u32 fourcc, enum codespace_sel cs_sel, bool allow_bayer)
{
	return find_format(fourcc, 0, cs_sel, true, allow_bayer);
}
EXPORT_SYMBOL_GPL(imx_media_find_format);

int imx_media_enum_format(u32 *fourcc, u32 index, enum codespace_sel cs_sel)
{
	return enum_format(fourcc, NULL, index, cs_sel, true, false);
}
EXPORT_SYMBOL_GPL(imx_media_enum_format);

const struct imx_media_pixfmt *
imx_media_find_mbus_format(u32 code, enum codespace_sel cs_sel,
			   bool allow_bayer)
{
	return find_format(0, code, cs_sel, false, allow_bayer);
}
EXPORT_SYMBOL_GPL(imx_media_find_mbus_format);

int imx_media_enum_mbus_format(u32 *code, u32 index, enum codespace_sel cs_sel,
			       bool allow_bayer)
{
	return enum_format(NULL, code, index, cs_sel, false, allow_bayer);
}
EXPORT_SYMBOL_GPL(imx_media_enum_mbus_format);

const struct imx_media_pixfmt *
imx_media_find_ipu_format(u32 code, enum codespace_sel cs_sel)
{
	const struct imx_media_pixfmt *array, *fmt, *ret = NULL;
	u32 array_size;
	int i, j;

	switch (cs_sel) {
	case CS_SEL_YUV:
		array_size = NUM_IPU_YUV_FORMATS;
		array = ipu_yuv_formats;
		break;
	case CS_SEL_RGB:
		array_size = NUM_IPU_RGB_FORMATS;
		array = ipu_rgb_formats;
		break;
	case CS_SEL_ANY:
		array_size = NUM_IPU_YUV_FORMATS + NUM_IPU_RGB_FORMATS;
		array = ipu_yuv_formats;
		break;
	default:
		return NULL;
	}

	for (i = 0; i < array_size; i++) {
		if (cs_sel == CS_SEL_ANY && i >= NUM_IPU_YUV_FORMATS)
			fmt = &ipu_rgb_formats[i - NUM_IPU_YUV_FORMATS];
		else
			fmt = &array[i];

		for (j = 0; code && fmt->codes[j]; j++) {
			if (code == fmt->codes[j]) {
				ret = fmt;
				goto out;
			}
		}
	}

out:
	return ret;
}
EXPORT_SYMBOL_GPL(imx_media_find_ipu_format);

int imx_media_enum_ipu_format(u32 *code, u32 index, enum codespace_sel cs_sel)
{
	switch (cs_sel) {
	case CS_SEL_YUV:
		if (index >= NUM_IPU_YUV_FORMATS)
			return -EINVAL;
		*code = ipu_yuv_formats[index].codes[0];
		break;
	case CS_SEL_RGB:
		if (index >= NUM_IPU_RGB_FORMATS)
			return -EINVAL;
		*code = ipu_rgb_formats[index].codes[0];
		break;
	case CS_SEL_ANY:
		if (index >= NUM_IPU_YUV_FORMATS + NUM_IPU_RGB_FORMATS)
			return -EINVAL;
		if (index >= NUM_IPU_YUV_FORMATS) {
			index -= NUM_IPU_YUV_FORMATS;
			*code = ipu_rgb_formats[index].codes[0];
		} else {
			*code = ipu_yuv_formats[index].codes[0];
		}
		break;
	default:
		return -EINVAL;
	}

	return 0;
}
EXPORT_SYMBOL_GPL(imx_media_enum_ipu_format);
=======

	return NULL;
}
EXPORT_SYMBOL_GPL(imx_media_find_mbus_format);

/*
 * Enumerate entries in the pixel_formats[] array that match the
 * requested selection criteria. Return the fourcc that matches the
 * selection criteria at the requested match index.
 *
 * @fourcc: The returned fourcc that matches the search criteria at
 *          the requested match index.
 * @index: The requested match index.
 * @fmt_sel: Include in the enumeration entries with the given selection
 *           criteria.
 * @code: If non-zero, only include in the enumeration entries matching this
 *	media bus code.
 */
int imx_media_enum_pixel_formats(u32 *fourcc, u32 index,
				 enum imx_pixfmt_sel fmt_sel, u32 code)
{
	bool sel_ipu = fmt_sel & PIXFMT_SEL_IPU;
	unsigned int i;

	fmt_sel &= ~PIXFMT_SEL_IPU;

	for (i = 0; i < ARRAY_SIZE(pixel_formats); i++) {
		const struct imx_media_pixfmt *fmt = &pixel_formats[i];
		enum imx_pixfmt_sel sel;

		if (sel_ipu != fmt->ipufmt)
			continue;

		sel = fmt->bayer ? PIXFMT_SEL_BAYER :
			((fmt->cs == IPUV3_COLORSPACE_YUV) ?
			 PIXFMT_SEL_YUV : PIXFMT_SEL_RGB);

		if (!(fmt_sel & sel))
			continue;

		/*
		 * If a media bus code is specified, only consider formats that
		 * match it.
		 */
		if (code) {
			unsigned int j;

			if (!fmt->codes)
				continue;

			for (j = 0; fmt->codes[j]; j++) {
				if (code == fmt->codes[j])
					break;
			}

			if (!fmt->codes[j])
				continue;
		}

		if (index == 0) {
			*fourcc = fmt->fourcc;
			return 0;
		}

		index--;
	}

	return -EINVAL;
}
EXPORT_SYMBOL_GPL(imx_media_enum_pixel_formats);

/*
 * Enumerate entries in the pixel_formats[] array that match the
 * requested search criteria. Return the media-bus code that matches
 * the search criteria at the requested match index.
 *
 * @code: The returned media-bus code that matches the search criteria at
 *        the requested match index.
 * @index: The requested match index.
 * @fmt_sel: Include in the enumeration entries with the given selection
 *           criteria.
 */
int imx_media_enum_mbus_formats(u32 *code, u32 index,
				enum imx_pixfmt_sel fmt_sel)
{
	bool sel_ipu = fmt_sel & PIXFMT_SEL_IPU;
	unsigned int i;

	fmt_sel &= ~PIXFMT_SEL_IPU;

	for (i = 0; i < ARRAY_SIZE(pixel_formats); i++) {
		const struct imx_media_pixfmt *fmt = &pixel_formats[i];
		enum imx_pixfmt_sel sel;
		unsigned int j;

		if (sel_ipu != fmt->ipufmt)
			continue;

		sel = fmt->bayer ? PIXFMT_SEL_BAYER :
			((fmt->cs == IPUV3_COLORSPACE_YUV) ?
			 PIXFMT_SEL_YUV : PIXFMT_SEL_RGB);

		if (!(fmt_sel & sel) || !fmt->codes)
			continue;

		for (j = 0; fmt->codes[j]; j++) {
			if (index == 0) {
				*code = fmt->codes[j];
				return 0;
			}

			index--;
		}
	}

	return -EINVAL;
}
EXPORT_SYMBOL_GPL(imx_media_enum_mbus_formats);
>>>>>>> upstream/android-13

int imx_media_init_mbus_fmt(struct v4l2_mbus_framefmt *mbus,
			    u32 width, u32 height, u32 code, u32 field,
			    const struct imx_media_pixfmt **cc)
{
	const struct imx_media_pixfmt *lcc;

	mbus->width = width;
	mbus->height = height;
	mbus->field = field;
<<<<<<< HEAD
	if (code == 0)
		imx_media_enum_mbus_format(&code, 0, CS_SEL_YUV, false);
	lcc = imx_media_find_mbus_format(code, CS_SEL_ANY, false);
	if (!lcc) {
		lcc = imx_media_find_ipu_format(code, CS_SEL_ANY);
=======

	if (code == 0)
		imx_media_enum_mbus_formats(&code, 0, PIXFMT_SEL_YUV);

	lcc = imx_media_find_mbus_format(code, PIXFMT_SEL_ANY);
	if (!lcc) {
		lcc = imx_media_find_ipu_format(code, PIXFMT_SEL_YUV_RGB);
>>>>>>> upstream/android-13
		if (!lcc)
			return -EINVAL;
	}

	mbus->code = code;
<<<<<<< HEAD
	init_mbus_colorimetry(mbus, lcc);
=======

	mbus->colorspace = V4L2_COLORSPACE_SRGB;
	mbus->xfer_func = V4L2_MAP_XFER_FUNC_DEFAULT(mbus->colorspace);
	mbus->ycbcr_enc = V4L2_MAP_YCBCR_ENC_DEFAULT(mbus->colorspace);
	mbus->quantization =
		V4L2_MAP_QUANTIZATION_DEFAULT(lcc->cs == IPUV3_COLORSPACE_RGB,
					      mbus->colorspace,
					      mbus->ycbcr_enc);

>>>>>>> upstream/android-13
	if (cc)
		*cc = lcc;

	return 0;
}
EXPORT_SYMBOL_GPL(imx_media_init_mbus_fmt);

/*
 * Initializes the TRY format to the ACTIVE format on all pads
 * of a subdev. Can be used as the .init_cfg pad operation.
 */
int imx_media_init_cfg(struct v4l2_subdev *sd,
<<<<<<< HEAD
		       struct v4l2_subdev_pad_config *cfg)
=======
		       struct v4l2_subdev_state *sd_state)
>>>>>>> upstream/android-13
{
	struct v4l2_mbus_framefmt *mf_try;
	struct v4l2_subdev_format format;
	unsigned int pad;
	int ret;

	for (pad = 0; pad < sd->entity.num_pads; pad++) {
		memset(&format, 0, sizeof(format));

		format.pad = pad;
		format.which = V4L2_SUBDEV_FORMAT_ACTIVE;
		ret = v4l2_subdev_call(sd, pad, get_fmt, NULL, &format);
		if (ret)
			continue;

<<<<<<< HEAD
		mf_try = v4l2_subdev_get_try_format(sd, cfg, pad);
=======
		mf_try = v4l2_subdev_get_try_format(sd, sd_state, pad);
>>>>>>> upstream/android-13
		*mf_try = format.format;
	}

	return 0;
}
EXPORT_SYMBOL_GPL(imx_media_init_cfg);

/*
<<<<<<< HEAD
 * Check whether the field and colorimetry parameters in tryfmt are
 * uninitialized, and if so fill them with the values from fmt,
 * or if tryfmt->colorspace has been initialized, all the default
 * colorimetry params can be derived from tryfmt->colorspace.
=======
 * Default the colorspace in tryfmt to SRGB if set to an unsupported
 * colorspace or not initialized. Then set the remaining colorimetry
 * parameters based on the colorspace if they are uninitialized.
>>>>>>> upstream/android-13
 *
 * tryfmt->code must be set on entry.
 *
 * If this format is destined to be routed through the Image Converter,
<<<<<<< HEAD
 * quantization and Y`CbCr encoding must be fixed. The IC expects and
 * produces fixed quantization and Y`CbCr encoding at its input and output
 * (full range for RGB, limited range for YUV, and V4L2_YCBCR_ENC_601).
 */
void imx_media_fill_default_mbus_fields(struct v4l2_mbus_framefmt *tryfmt,
					struct v4l2_mbus_framefmt *fmt,
					bool ic_route)
=======
 * Y`CbCr encoding must be fixed. The IC supports only BT.601 Y`CbCr
 * or Rec.709 Y`CbCr encoding.
 */
void imx_media_try_colorimetry(struct v4l2_mbus_framefmt *tryfmt,
			       bool ic_route)
>>>>>>> upstream/android-13
{
	const struct imx_media_pixfmt *cc;
	bool is_rgb = false;

<<<<<<< HEAD
	cc = imx_media_find_mbus_format(tryfmt->code, CS_SEL_ANY, true);
	if (!cc)
		cc = imx_media_find_ipu_format(tryfmt->code, CS_SEL_ANY);
	if (cc && cc->cs != IPUV3_COLORSPACE_YUV)
		is_rgb = true;

	/* fill field if necessary */
	if (tryfmt->field == V4L2_FIELD_ANY)
		tryfmt->field = fmt->field;

	/* fill colorimetry if necessary */
	if (tryfmt->colorspace == V4L2_COLORSPACE_DEFAULT) {
		tryfmt->colorspace = fmt->colorspace;
		tryfmt->xfer_func = fmt->xfer_func;
		tryfmt->ycbcr_enc = fmt->ycbcr_enc;
		tryfmt->quantization = fmt->quantization;
	} else {
		if (tryfmt->xfer_func == V4L2_XFER_FUNC_DEFAULT) {
			tryfmt->xfer_func =
				V4L2_MAP_XFER_FUNC_DEFAULT(tryfmt->colorspace);
		}
=======
	cc = imx_media_find_mbus_format(tryfmt->code, PIXFMT_SEL_ANY);
	if (!cc)
		cc = imx_media_find_ipu_format(tryfmt->code,
					       PIXFMT_SEL_YUV_RGB);

	if (cc && cc->cs == IPUV3_COLORSPACE_RGB)
		is_rgb = true;

	switch (tryfmt->colorspace) {
	case V4L2_COLORSPACE_SMPTE170M:
	case V4L2_COLORSPACE_REC709:
	case V4L2_COLORSPACE_JPEG:
	case V4L2_COLORSPACE_SRGB:
	case V4L2_COLORSPACE_BT2020:
	case V4L2_COLORSPACE_OPRGB:
	case V4L2_COLORSPACE_DCI_P3:
	case V4L2_COLORSPACE_RAW:
		break;
	default:
		tryfmt->colorspace = V4L2_COLORSPACE_SRGB;
		break;
	}

	if (tryfmt->xfer_func == V4L2_XFER_FUNC_DEFAULT)
		tryfmt->xfer_func =
			V4L2_MAP_XFER_FUNC_DEFAULT(tryfmt->colorspace);

	if (ic_route) {
		if (tryfmt->ycbcr_enc != V4L2_YCBCR_ENC_601 &&
		    tryfmt->ycbcr_enc != V4L2_YCBCR_ENC_709)
			tryfmt->ycbcr_enc = V4L2_YCBCR_ENC_601;
	} else {
>>>>>>> upstream/android-13
		if (tryfmt->ycbcr_enc == V4L2_YCBCR_ENC_DEFAULT) {
			tryfmt->ycbcr_enc =
				V4L2_MAP_YCBCR_ENC_DEFAULT(tryfmt->colorspace);
		}
<<<<<<< HEAD
		if (tryfmt->quantization == V4L2_QUANTIZATION_DEFAULT) {
			tryfmt->quantization =
				V4L2_MAP_QUANTIZATION_DEFAULT(
					is_rgb, tryfmt->colorspace,
					tryfmt->ycbcr_enc);
		}
	}

	if (ic_route) {
		tryfmt->quantization = is_rgb ?
			V4L2_QUANTIZATION_FULL_RANGE :
			V4L2_QUANTIZATION_LIM_RANGE;
		tryfmt->ycbcr_enc = V4L2_YCBCR_ENC_601;
	}
}
EXPORT_SYMBOL_GPL(imx_media_fill_default_mbus_fields);

int imx_media_mbus_fmt_to_pix_fmt(struct v4l2_pix_format *pix,
				  struct v4l2_mbus_framefmt *mbus,
				  const struct imx_media_pixfmt *cc)
{
	u32 stride;

	if (!cc) {
		cc = imx_media_find_ipu_format(mbus->code, CS_SEL_ANY);
		if (!cc)
			cc = imx_media_find_mbus_format(mbus->code, CS_SEL_ANY,
							true);
=======
	}

	if (tryfmt->quantization == V4L2_QUANTIZATION_DEFAULT)
		tryfmt->quantization =
			V4L2_MAP_QUANTIZATION_DEFAULT(is_rgb,
						      tryfmt->colorspace,
						      tryfmt->ycbcr_enc);
}
EXPORT_SYMBOL_GPL(imx_media_try_colorimetry);

int imx_media_mbus_fmt_to_pix_fmt(struct v4l2_pix_format *pix,
				  const struct v4l2_mbus_framefmt *mbus,
				  const struct imx_media_pixfmt *cc)
{
	u32 width;
	u32 stride;

	if (!cc) {
		cc = imx_media_find_ipu_format(mbus->code,
					       PIXFMT_SEL_YUV_RGB);
		if (!cc)
			cc = imx_media_find_mbus_format(mbus->code,
							PIXFMT_SEL_ANY);
>>>>>>> upstream/android-13
		if (!cc)
			return -EINVAL;
	}

	/*
	 * TODO: the IPU currently does not support the AYUV32 format,
	 * so until it does convert to a supported YUV format.
	 */
	if (cc->ipufmt && cc->cs == IPUV3_COLORSPACE_YUV) {
		u32 code;

<<<<<<< HEAD
		imx_media_enum_mbus_format(&code, 0, CS_SEL_YUV, false);
		cc = imx_media_find_mbus_format(code, CS_SEL_YUV, false);
	}

	stride = cc->planar ? mbus->width : (mbus->width * cc->bpp) >> 3;

	pix->width = mbus->width;
=======
		imx_media_enum_mbus_formats(&code, 0, PIXFMT_SEL_YUV);
		cc = imx_media_find_mbus_format(code, PIXFMT_SEL_YUV);
	}

	/* Round up width for minimum burst size */
	width = round_up(mbus->width, 8);

	/* Round up stride for IDMAC line start address alignment */
	if (cc->planar)
		stride = round_up(width, 16);
	else
		stride = round_up((width * cc->bpp) >> 3, 8);

	pix->width = width;
>>>>>>> upstream/android-13
	pix->height = mbus->height;
	pix->pixelformat = cc->fourcc;
	pix->colorspace = mbus->colorspace;
	pix->xfer_func = mbus->xfer_func;
	pix->ycbcr_enc = mbus->ycbcr_enc;
	pix->quantization = mbus->quantization;
	pix->field = mbus->field;
	pix->bytesperline = stride;
<<<<<<< HEAD
	pix->sizeimage = (pix->width * pix->height * cc->bpp) >> 3;
=======
	pix->sizeimage = cc->planar ? ((stride * pix->height * cc->bpp) >> 3) :
			 stride * pix->height;
>>>>>>> upstream/android-13

	return 0;
}
EXPORT_SYMBOL_GPL(imx_media_mbus_fmt_to_pix_fmt);

int imx_media_mbus_fmt_to_ipu_image(struct ipu_image *image,
<<<<<<< HEAD
				    struct v4l2_mbus_framefmt *mbus)
=======
				    const struct v4l2_mbus_framefmt *mbus)
>>>>>>> upstream/android-13
{
	int ret;

	memset(image, 0, sizeof(*image));

	ret = imx_media_mbus_fmt_to_pix_fmt(&image->pix, mbus, NULL);
	if (ret)
		return ret;

	image->rect.width = mbus->width;
	image->rect.height = mbus->height;

	return 0;
}
EXPORT_SYMBOL_GPL(imx_media_mbus_fmt_to_ipu_image);

int imx_media_ipu_image_to_mbus_fmt(struct v4l2_mbus_framefmt *mbus,
<<<<<<< HEAD
				    struct ipu_image *image)
{
	const struct imx_media_pixfmt *fmt;

	fmt = imx_media_find_format(image->pix.pixelformat, CS_SEL_ANY, true);
	if (!fmt)
=======
				    const struct ipu_image *image)
{
	const struct imx_media_pixfmt *fmt;

	fmt = imx_media_find_pixel_format(image->pix.pixelformat,
					  PIXFMT_SEL_ANY);
	if (!fmt || !fmt->codes || !fmt->codes[0])
>>>>>>> upstream/android-13
		return -EINVAL;

	memset(mbus, 0, sizeof(*mbus));
	mbus->width = image->pix.width;
	mbus->height = image->pix.height;
	mbus->code = fmt->codes[0];
	mbus->field = image->pix.field;
	mbus->colorspace = image->pix.colorspace;
	mbus->xfer_func = image->pix.xfer_func;
	mbus->ycbcr_enc = image->pix.ycbcr_enc;
	mbus->quantization = image->pix.quantization;

	return 0;
}
EXPORT_SYMBOL_GPL(imx_media_ipu_image_to_mbus_fmt);

<<<<<<< HEAD
void imx_media_free_dma_buf(struct imx_media_dev *imxmd,
			    struct imx_media_dma_buf *buf)
{
	if (buf->virt)
		dma_free_coherent(imxmd->md.dev, buf->len,
				  buf->virt, buf->phys);
=======
void imx_media_free_dma_buf(struct device *dev,
			    struct imx_media_dma_buf *buf)
{
	if (buf->virt)
		dma_free_coherent(dev, buf->len, buf->virt, buf->phys);
>>>>>>> upstream/android-13

	buf->virt = NULL;
	buf->phys = 0;
}
EXPORT_SYMBOL_GPL(imx_media_free_dma_buf);

<<<<<<< HEAD
int imx_media_alloc_dma_buf(struct imx_media_dev *imxmd,
			    struct imx_media_dma_buf *buf,
			    int size)
{
	imx_media_free_dma_buf(imxmd, buf);

	buf->len = PAGE_ALIGN(size);
	buf->virt = dma_alloc_coherent(imxmd->md.dev, buf->len, &buf->phys,
				       GFP_DMA | GFP_KERNEL);
	if (!buf->virt) {
		dev_err(imxmd->md.dev, "failed to alloc dma buffer\n");
=======
int imx_media_alloc_dma_buf(struct device *dev,
			    struct imx_media_dma_buf *buf,
			    int size)
{
	imx_media_free_dma_buf(dev, buf);

	buf->len = PAGE_ALIGN(size);
	buf->virt = dma_alloc_coherent(dev, buf->len, &buf->phys,
				       GFP_DMA | GFP_KERNEL);
	if (!buf->virt) {
		dev_err(dev, "%s: failed\n", __func__);
>>>>>>> upstream/android-13
		return -ENOMEM;
	}

	return 0;
}
EXPORT_SYMBOL_GPL(imx_media_alloc_dma_buf);

/* form a subdev name given a group id and ipu id */
void imx_media_grp_id_to_sd_name(char *sd_name, int sz, u32 grp_id, int ipu_id)
{
	int id;

	switch (grp_id) {
<<<<<<< HEAD
	case IMX_MEDIA_GRP_ID_CSI0...IMX_MEDIA_GRP_ID_CSI1:
		id = (grp_id >> IMX_MEDIA_GRP_ID_CSI_BIT) - 1;
		snprintf(sd_name, sz, "ipu%d_csi%d", ipu_id + 1, id);
		break;
	case IMX_MEDIA_GRP_ID_VDIC:
		snprintf(sd_name, sz, "ipu%d_vdic", ipu_id + 1);
		break;
	case IMX_MEDIA_GRP_ID_IC_PRP:
		snprintf(sd_name, sz, "ipu%d_ic_prp", ipu_id + 1);
		break;
	case IMX_MEDIA_GRP_ID_IC_PRPENC:
		snprintf(sd_name, sz, "ipu%d_ic_prpenc", ipu_id + 1);
		break;
	case IMX_MEDIA_GRP_ID_IC_PRPVF:
=======
	case IMX_MEDIA_GRP_ID_IPU_CSI0...IMX_MEDIA_GRP_ID_IPU_CSI1:
		id = (grp_id >> IMX_MEDIA_GRP_ID_IPU_CSI_BIT) - 1;
		snprintf(sd_name, sz, "ipu%d_csi%d", ipu_id + 1, id);
		break;
	case IMX_MEDIA_GRP_ID_IPU_VDIC:
		snprintf(sd_name, sz, "ipu%d_vdic", ipu_id + 1);
		break;
	case IMX_MEDIA_GRP_ID_IPU_IC_PRP:
		snprintf(sd_name, sz, "ipu%d_ic_prp", ipu_id + 1);
		break;
	case IMX_MEDIA_GRP_ID_IPU_IC_PRPENC:
		snprintf(sd_name, sz, "ipu%d_ic_prpenc", ipu_id + 1);
		break;
	case IMX_MEDIA_GRP_ID_IPU_IC_PRPVF:
>>>>>>> upstream/android-13
		snprintf(sd_name, sz, "ipu%d_ic_prpvf", ipu_id + 1);
		break;
	default:
		break;
	}
}
EXPORT_SYMBOL_GPL(imx_media_grp_id_to_sd_name);

struct v4l2_subdev *
imx_media_find_subdev_by_fwnode(struct imx_media_dev *imxmd,
				struct fwnode_handle *fwnode)
{
	struct v4l2_subdev *sd;

	list_for_each_entry(sd, &imxmd->v4l2_dev.subdevs, list) {
		if (sd->fwnode == fwnode)
			return sd;
	}

	return NULL;
}
EXPORT_SYMBOL_GPL(imx_media_find_subdev_by_fwnode);

struct v4l2_subdev *
imx_media_find_subdev_by_devname(struct imx_media_dev *imxmd,
				 const char *devname)
{
	struct v4l2_subdev *sd;

	list_for_each_entry(sd, &imxmd->v4l2_dev.subdevs, list) {
		if (!strcmp(devname, dev_name(sd->dev)))
			return sd;
	}

	return NULL;
}
EXPORT_SYMBOL_GPL(imx_media_find_subdev_by_devname);

/*
<<<<<<< HEAD
 * Adds a video device to the master video device list. This is called by
 * an async subdev that owns a video device when it is registered.
 */
int imx_media_add_video_device(struct imx_media_dev *imxmd,
			       struct imx_media_video_dev *vdev)
=======
 * Adds a video device to the master video device list. This is called
 * when a video device is registered.
 */
void imx_media_add_video_device(struct imx_media_dev *imxmd,
				struct imx_media_video_dev *vdev)
>>>>>>> upstream/android-13
{
	mutex_lock(&imxmd->mutex);

	list_add_tail(&vdev->list, &imxmd->vdev_list);

	mutex_unlock(&imxmd->mutex);
<<<<<<< HEAD
	return 0;
=======
>>>>>>> upstream/android-13
}
EXPORT_SYMBOL_GPL(imx_media_add_video_device);

/*
<<<<<<< HEAD
 * Search upstream/downstream for a subdevice in the current pipeline
 * with given grp_id, starting from start_entity. Returns the subdev's
 * source/sink pad that it was reached from. If grp_id is zero, just
 * returns the nearest source/sink pad to start_entity. Must be called
 * with mdev->graph_mutex held.
 */
static struct media_pad *
find_pipeline_pad(struct imx_media_dev *imxmd,
		  struct media_entity *start_entity,
		  u32 grp_id, bool upstream)
{
	struct media_entity *me = start_entity;
	struct media_pad *pad = NULL;
=======
 * Search upstream/downstream for a subdevice or video device pad in the
 * current pipeline, starting from start_entity. Returns the device's
 * source/sink pad that it was reached from. Must be called with
 * mdev->graph_mutex held.
 *
 * If grp_id != 0, finds a subdevice's pad of given grp_id.
 * Else If buftype != 0, finds a video device's pad of given buffer type.
 * Else, returns the nearest source/sink pad to start_entity.
 */
struct media_pad *
imx_media_pipeline_pad(struct media_entity *start_entity, u32 grp_id,
		       enum v4l2_buf_type buftype, bool upstream)
{
	struct media_entity *me = start_entity;
	struct media_pad *pad = NULL;
	struct video_device *vfd;
>>>>>>> upstream/android-13
	struct v4l2_subdev *sd;
	int i;

	for (i = 0; i < me->num_pads; i++) {
		struct media_pad *spad = &me->pads[i];

		if ((upstream && !(spad->flags & MEDIA_PAD_FL_SINK)) ||
		    (!upstream && !(spad->flags & MEDIA_PAD_FL_SOURCE)))
			continue;

		pad = media_entity_remote_pad(spad);
<<<<<<< HEAD
		if (!pad || !is_media_entity_v4l2_subdev(pad->entity))
			continue;

		if (grp_id != 0) {
			sd = media_entity_to_v4l2_subdev(pad->entity);
			if (sd->grp_id & grp_id)
				return pad;

			return find_pipeline_pad(imxmd, pad->entity,
						 grp_id, upstream);
=======
		if (!pad)
			continue;

		if (grp_id) {
			if (is_media_entity_v4l2_subdev(pad->entity)) {
				sd = media_entity_to_v4l2_subdev(pad->entity);
				if (sd->grp_id & grp_id)
					return pad;
			}

			return imx_media_pipeline_pad(pad->entity, grp_id,
						      buftype, upstream);
		} else if (buftype) {
			if (is_media_entity_v4l2_video_device(pad->entity)) {
				vfd = media_entity_to_video_device(pad->entity);
				if (buftype == vfd->queue->type)
					return pad;
			}

			return imx_media_pipeline_pad(pad->entity, grp_id,
						      buftype, upstream);
>>>>>>> upstream/android-13
		} else {
			return pad;
		}
	}

	return NULL;
}
<<<<<<< HEAD

/*
 * Search upstream for a subdev in the current pipeline with
 * given grp_id. Must be called with mdev->graph_mutex held.
 */
static struct v4l2_subdev *
find_upstream_subdev(struct imx_media_dev *imxmd,
		     struct media_entity *start_entity,
		     u32 grp_id)
{
	struct v4l2_subdev *sd;
	struct media_pad *pad;

	if (is_media_entity_v4l2_subdev(start_entity)) {
		sd = media_entity_to_v4l2_subdev(start_entity);
		if (sd->grp_id & grp_id)
			return sd;
	}

	pad = find_pipeline_pad(imxmd, start_entity, grp_id, true);

	return pad ? media_entity_to_v4l2_subdev(pad->entity) : NULL;
=======
EXPORT_SYMBOL_GPL(imx_media_pipeline_pad);

/*
 * Search upstream/downstream for a subdev or video device in the current
 * pipeline. Must be called with mdev->graph_mutex held.
 */
static struct media_entity *
find_pipeline_entity(struct media_entity *start, u32 grp_id,
		     enum v4l2_buf_type buftype, bool upstream)
{
	struct media_pad *pad = NULL;
	struct video_device *vfd;
	struct v4l2_subdev *sd;

	if (grp_id && is_media_entity_v4l2_subdev(start)) {
		sd = media_entity_to_v4l2_subdev(start);
		if (sd->grp_id & grp_id)
			return &sd->entity;
	} else if (buftype && is_media_entity_v4l2_video_device(start)) {
		vfd = media_entity_to_video_device(start);
		if (buftype == vfd->queue->type)
			return &vfd->entity;
	}

	pad = imx_media_pipeline_pad(start, grp_id, buftype, upstream);

	return pad ? pad->entity : NULL;
>>>>>>> upstream/android-13
}

/*
 * Find the upstream mipi-csi2 virtual channel reached from the given
 * start entity in the current pipeline.
 * Must be called with mdev->graph_mutex held.
 */
<<<<<<< HEAD
int imx_media_find_mipi_csi2_channel(struct imx_media_dev *imxmd,
				     struct media_entity *start_entity)
=======
int imx_media_pipeline_csi2_channel(struct media_entity *start_entity)
>>>>>>> upstream/android-13
{
	struct media_pad *pad;
	int ret = -EPIPE;

<<<<<<< HEAD
	pad = find_pipeline_pad(imxmd, start_entity, IMX_MEDIA_GRP_ID_CSI2,
				true);
	if (pad) {
		ret = pad->index - 1;
		dev_dbg(imxmd->md.dev, "found vc%d from %s\n",
			ret, start_entity->name);
	}

	return ret;
}
EXPORT_SYMBOL_GPL(imx_media_find_mipi_csi2_channel);

/*
 * Find a source pad reached upstream from the given start entity in
 * the current pipeline. Must be called with mdev->graph_mutex held.
 */
struct media_pad *
imx_media_find_upstream_pad(struct imx_media_dev *imxmd,
			    struct media_entity *start_entity,
			    u32 grp_id)
{
	struct media_pad *pad;

	pad = find_pipeline_pad(imxmd, start_entity, grp_id, true);
	if (!pad)
		return ERR_PTR(-ENODEV);

	return pad;
}
EXPORT_SYMBOL_GPL(imx_media_find_upstream_pad);
=======
	pad = imx_media_pipeline_pad(start_entity, IMX_MEDIA_GRP_ID_CSI2,
				     0, true);
	if (pad)
		ret = pad->index - 1;

	return ret;
}
EXPORT_SYMBOL_GPL(imx_media_pipeline_csi2_channel);
>>>>>>> upstream/android-13

/*
 * Find a subdev reached upstream from the given start entity in
 * the current pipeline.
 * Must be called with mdev->graph_mutex held.
 */
struct v4l2_subdev *
<<<<<<< HEAD
imx_media_find_upstream_subdev(struct imx_media_dev *imxmd,
			       struct media_entity *start_entity,
			       u32 grp_id)
{
	struct v4l2_subdev *sd;

	sd = find_upstream_subdev(imxmd, start_entity, grp_id);
	if (!sd)
		return ERR_PTR(-ENODEV);

	return sd;
}
EXPORT_SYMBOL_GPL(imx_media_find_upstream_subdev);
=======
imx_media_pipeline_subdev(struct media_entity *start_entity, u32 grp_id,
			  bool upstream)
{
	struct media_entity *me;

	me = find_pipeline_entity(start_entity, grp_id, 0, upstream);
	if (!me)
		return ERR_PTR(-ENODEV);

	return media_entity_to_v4l2_subdev(me);
}
EXPORT_SYMBOL_GPL(imx_media_pipeline_subdev);

/*
 * Find a subdev reached upstream from the given start entity in
 * the current pipeline.
 * Must be called with mdev->graph_mutex held.
 */
struct video_device *
imx_media_pipeline_video_device(struct media_entity *start_entity,
				enum v4l2_buf_type buftype, bool upstream)
{
	struct media_entity *me;

	me = find_pipeline_entity(start_entity, 0, buftype, upstream);
	if (!me)
		return ERR_PTR(-ENODEV);

	return media_entity_to_video_device(me);
}
EXPORT_SYMBOL_GPL(imx_media_pipeline_video_device);

/*
 * Find a fwnode endpoint that maps to the given subdevice's pad.
 * If there are multiple endpoints that map to the pad, only the
 * first endpoint encountered is returned.
 *
 * On success the refcount of the returned fwnode endpoint is
 * incremented.
 */
struct fwnode_handle *imx_media_get_pad_fwnode(struct media_pad *pad)
{
	struct fwnode_handle *endpoint;
	struct v4l2_subdev *sd;

	if (!is_media_entity_v4l2_subdev(pad->entity))
		return ERR_PTR(-ENODEV);

	sd = media_entity_to_v4l2_subdev(pad->entity);

	fwnode_graph_for_each_endpoint(dev_fwnode(sd->dev), endpoint) {
		int pad_idx = media_entity_get_fwnode_pad(&sd->entity,
							  endpoint,
							  pad->flags);
		if (pad_idx < 0)
			continue;

		if (pad_idx == pad->index)
			return endpoint;
	}

	return ERR_PTR(-ENODEV);
}
EXPORT_SYMBOL_GPL(imx_media_get_pad_fwnode);
>>>>>>> upstream/android-13

/*
 * Turn current pipeline streaming on/off starting from entity.
 */
int imx_media_pipeline_set_stream(struct imx_media_dev *imxmd,
				  struct media_entity *entity,
				  bool on)
{
	struct v4l2_subdev *sd;
	int ret = 0;

	if (!is_media_entity_v4l2_subdev(entity))
		return -EINVAL;
	sd = media_entity_to_v4l2_subdev(entity);

	mutex_lock(&imxmd->md.graph_mutex);

	if (on) {
		ret = __media_pipeline_start(entity, &imxmd->pipe);
		if (ret)
			goto out;
		ret = v4l2_subdev_call(sd, video, s_stream, 1);
		if (ret)
			__media_pipeline_stop(entity);
	} else {
		v4l2_subdev_call(sd, video, s_stream, 0);
		if (entity->pipe)
			__media_pipeline_stop(entity);
	}

out:
	mutex_unlock(&imxmd->md.graph_mutex);
	return ret;
}
EXPORT_SYMBOL_GPL(imx_media_pipeline_set_stream);

MODULE_DESCRIPTION("i.MX5/6 v4l2 media controller driver");
MODULE_AUTHOR("Steve Longerbeam <steve_longerbeam@mentor.com>");
MODULE_LICENSE("GPL");
