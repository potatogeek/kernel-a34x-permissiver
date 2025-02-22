/*
 * Copyright 2017 Advanced Micro Devices, Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE COPYRIGHT HOLDER(S) OR AUTHOR(S) BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * Authors: AMD
 *
 */

#ifndef _DCN10_IPP_H_
#define _DCN10_IPP_H_

#include "ipp.h"

#define TO_DCN10_IPP(ipp)\
	container_of(ipp, struct dcn10_ipp, base)

#define IPP_REG_LIST_DCN(id) \
	SRI(FORMAT_CONTROL, CNVC_CFG, id), \
	SRI(CNVC_SURFACE_PIXEL_FORMAT, CNVC_CFG, id), \
	SRI(CURSOR0_CONTROL, CNVC_CUR, id), \
	SRI(CURSOR0_COLOR0, CNVC_CUR, id), \
	SRI(CURSOR0_COLOR1, CNVC_CUR, id)

#define IPP_REG_LIST_DCN10(id) \
	IPP_REG_LIST_DCN(id), \
	SRI(CURSOR_SETTINS, HUBPREQ, id), \
	SRI(CURSOR_SURFACE_ADDRESS_HIGH, CURSOR, id), \
	SRI(CURSOR_SURFACE_ADDRESS, CURSOR, id), \
	SRI(CURSOR_SIZE, CURSOR, id), \
	SRI(CURSOR_CONTROL, CURSOR, id), \
	SRI(CURSOR_POSITION, CURSOR, id), \
	SRI(CURSOR_HOT_SPOT, CURSOR, id), \
	SRI(CURSOR_DST_OFFSET, CURSOR, id)

<<<<<<< HEAD
=======
#define IPP_REG_LIST_DCN20(id) \
	IPP_REG_LIST_DCN(id), \
	SRI(CURSOR_SETTINGS, HUBPREQ, id), \
	SRI(CURSOR_SURFACE_ADDRESS_HIGH, CURSOR0_, id), \
	SRI(CURSOR_SURFACE_ADDRESS, CURSOR0_, id), \
	SRI(CURSOR_SIZE, CURSOR0_, id), \
	SRI(CURSOR_CONTROL, CURSOR0_, id), \
	SRI(CURSOR_POSITION, CURSOR0_, id), \
	SRI(CURSOR_HOT_SPOT, CURSOR0_, id), \
	SRI(CURSOR_DST_OFFSET, CURSOR0_, id)

>>>>>>> upstream/android-13
#define CURSOR0_CURSOR_CONTROL__CURSOR_2X_MAGNIFY__SHIFT	0x4
#define CURSOR0_CURSOR_CONTROL__CURSOR_2X_MAGNIFY_MASK		0x00000010L
#define CURSOR1_CURSOR_CONTROL__CURSOR_2X_MAGNIFY__SHIFT	0x4
#define CURSOR1_CURSOR_CONTROL__CURSOR_2X_MAGNIFY_MASK		0x00000010L
#define CURSOR2_CURSOR_CONTROL__CURSOR_2X_MAGNIFY__SHIFT	0x4
#define CURSOR2_CURSOR_CONTROL__CURSOR_2X_MAGNIFY_MASK		0x00000010L
#define CURSOR3_CURSOR_CONTROL__CURSOR_2X_MAGNIFY__SHIFT	0x4
#define CURSOR3_CURSOR_CONTROL__CURSOR_2X_MAGNIFY_MASK		0x00000010L

#define IPP_SF(reg_name, field_name, post_fix)\
	.field_name = reg_name ## __ ## field_name ## post_fix

#define IPP_MASK_SH_LIST_DCN(mask_sh) \
	IPP_SF(CNVC_CFG0_CNVC_SURFACE_PIXEL_FORMAT, CNVC_SURFACE_PIXEL_FORMAT, mask_sh), \
	IPP_SF(CNVC_CFG0_FORMAT_CONTROL, CNVC_BYPASS, mask_sh), \
	IPP_SF(CNVC_CFG0_FORMAT_CONTROL, ALPHA_EN, mask_sh), \
	IPP_SF(CNVC_CFG0_FORMAT_CONTROL, FORMAT_EXPANSION_MODE, mask_sh), \
	IPP_SF(CNVC_CUR0_CURSOR0_CONTROL, CUR0_MODE, mask_sh), \
	IPP_SF(CNVC_CUR0_CURSOR0_COLOR0, CUR0_COLOR0, mask_sh), \
	IPP_SF(CNVC_CUR0_CURSOR0_COLOR1, CUR0_COLOR1, mask_sh), \
	IPP_SF(CNVC_CUR0_CURSOR0_CONTROL, CUR0_EXPANSION_MODE, mask_sh), \
	IPP_SF(CNVC_CUR0_CURSOR0_CONTROL, CUR0_ENABLE, mask_sh)

#define IPP_MASK_SH_LIST_DCN10(mask_sh) \
	IPP_MASK_SH_LIST_DCN(mask_sh),\
	IPP_SF(HUBPREQ0_CURSOR_SETTINS, CURSOR0_DST_Y_OFFSET, mask_sh), \
	IPP_SF(HUBPREQ0_CURSOR_SETTINS, CURSOR0_CHUNK_HDL_ADJUST, mask_sh), \
	IPP_SF(CURSOR0_CURSOR_SURFACE_ADDRESS_HIGH, CURSOR_SURFACE_ADDRESS_HIGH, mask_sh), \
	IPP_SF(CURSOR0_CURSOR_SURFACE_ADDRESS, CURSOR_SURFACE_ADDRESS, mask_sh), \
	IPP_SF(CURSOR0_CURSOR_SIZE, CURSOR_WIDTH, mask_sh), \
	IPP_SF(CURSOR0_CURSOR_SIZE, CURSOR_HEIGHT, mask_sh), \
	IPP_SF(CURSOR0_CURSOR_CONTROL, CURSOR_MODE, mask_sh), \
	IPP_SF(CURSOR0_CURSOR_CONTROL, CURSOR_2X_MAGNIFY, mask_sh), \
	IPP_SF(CURSOR0_CURSOR_CONTROL, CURSOR_PITCH, mask_sh), \
	IPP_SF(CURSOR0_CURSOR_CONTROL, CURSOR_LINES_PER_CHUNK, mask_sh), \
	IPP_SF(CURSOR0_CURSOR_CONTROL, CURSOR_ENABLE, mask_sh), \
	IPP_SF(CURSOR0_CURSOR_POSITION, CURSOR_X_POSITION, mask_sh), \
	IPP_SF(CURSOR0_CURSOR_POSITION, CURSOR_Y_POSITION, mask_sh), \
	IPP_SF(CURSOR0_CURSOR_HOT_SPOT, CURSOR_HOT_SPOT_X, mask_sh), \
	IPP_SF(CURSOR0_CURSOR_HOT_SPOT, CURSOR_HOT_SPOT_Y, mask_sh), \
	IPP_SF(CURSOR0_CURSOR_DST_OFFSET, CURSOR_DST_X_OFFSET, mask_sh), \
	IPP_SF(CNVC_CFG0_FORMAT_CONTROL, OUTPUT_FP, mask_sh)

<<<<<<< HEAD
=======
#define IPP_MASK_SH_LIST_DCN20(mask_sh) \
	IPP_MASK_SH_LIST_DCN(mask_sh), \
	IPP_SF(HUBPREQ0_CURSOR_SETTINGS, CURSOR0_DST_Y_OFFSET, mask_sh), \
	IPP_SF(HUBPREQ0_CURSOR_SETTINGS, CURSOR0_CHUNK_HDL_ADJUST, mask_sh), \
	IPP_SF(CURSOR0_0_CURSOR_SURFACE_ADDRESS_HIGH, CURSOR_SURFACE_ADDRESS_HIGH, mask_sh), \
	IPP_SF(CURSOR0_0_CURSOR_SURFACE_ADDRESS, CURSOR_SURFACE_ADDRESS, mask_sh), \
	IPP_SF(CURSOR0_0_CURSOR_SIZE, CURSOR_WIDTH, mask_sh), \
	IPP_SF(CURSOR0_0_CURSOR_SIZE, CURSOR_HEIGHT, mask_sh), \
	IPP_SF(CURSOR0_0_CURSOR_CONTROL, CURSOR_MODE, mask_sh), \
	IPP_SF(CURSOR0_0_CURSOR_CONTROL, CURSOR_2X_MAGNIFY, mask_sh), \
	IPP_SF(CURSOR0_0_CURSOR_CONTROL, CURSOR_PITCH, mask_sh), \
	IPP_SF(CURSOR0_0_CURSOR_CONTROL, CURSOR_LINES_PER_CHUNK, mask_sh), \
	IPP_SF(CURSOR0_0_CURSOR_CONTROL, CURSOR_ENABLE, mask_sh), \
	IPP_SF(CURSOR0_0_CURSOR_POSITION, CURSOR_X_POSITION, mask_sh), \
	IPP_SF(CURSOR0_0_CURSOR_POSITION, CURSOR_Y_POSITION, mask_sh), \
	IPP_SF(CURSOR0_0_CURSOR_HOT_SPOT, CURSOR_HOT_SPOT_X, mask_sh), \
	IPP_SF(CURSOR0_0_CURSOR_HOT_SPOT, CURSOR_HOT_SPOT_Y, mask_sh), \
	IPP_SF(CURSOR0_0_CURSOR_DST_OFFSET, CURSOR_DST_X_OFFSET, mask_sh)

>>>>>>> upstream/android-13
#define IPP_DCN10_REG_FIELD_LIST(type) \
	type CNVC_SURFACE_PIXEL_FORMAT; \
	type CNVC_BYPASS; \
	type ALPHA_EN; \
	type FORMAT_EXPANSION_MODE; \
	type CURSOR0_DST_Y_OFFSET; \
	type CURSOR0_CHUNK_HDL_ADJUST; \
	type CUR0_MODE; \
	type CUR0_COLOR0; \
	type CUR0_COLOR1; \
	type CUR0_EXPANSION_MODE; \
	type CURSOR_SURFACE_ADDRESS_HIGH; \
	type CURSOR_SURFACE_ADDRESS; \
	type CURSOR_WIDTH; \
	type CURSOR_HEIGHT; \
	type CURSOR_MODE; \
	type CURSOR_2X_MAGNIFY; \
	type CURSOR_PITCH; \
	type CURSOR_LINES_PER_CHUNK; \
	type CURSOR_ENABLE; \
	type CUR0_ENABLE; \
	type CURSOR_X_POSITION; \
	type CURSOR_Y_POSITION; \
	type CURSOR_HOT_SPOT_X; \
	type CURSOR_HOT_SPOT_Y; \
	type CURSOR_DST_X_OFFSET; \
	type OUTPUT_FP

struct dcn10_ipp_shift {
	IPP_DCN10_REG_FIELD_LIST(uint8_t);
};

struct dcn10_ipp_mask {
	IPP_DCN10_REG_FIELD_LIST(uint32_t);
};

struct dcn10_ipp_registers {
	uint32_t CURSOR_SETTINS;
	uint32_t CURSOR_SETTINGS;
	uint32_t CNVC_SURFACE_PIXEL_FORMAT;
	uint32_t CURSOR0_CONTROL;
	uint32_t CURSOR0_COLOR0;
	uint32_t CURSOR0_COLOR1;
	uint32_t FORMAT_CONTROL;
	uint32_t CURSOR_SURFACE_ADDRESS_HIGH;
	uint32_t CURSOR_SURFACE_ADDRESS;
	uint32_t CURSOR_SIZE;
	uint32_t CURSOR_CONTROL;
	uint32_t CURSOR_POSITION;
	uint32_t CURSOR_HOT_SPOT;
	uint32_t CURSOR_DST_OFFSET;
};

struct dcn10_ipp {
	struct input_pixel_processor base;

	const struct dcn10_ipp_registers *regs;
	const struct dcn10_ipp_shift *ipp_shift;
	const struct dcn10_ipp_mask *ipp_mask;

	struct dc_cursor_attributes curs_attr;
};

void dcn10_ipp_construct(struct dcn10_ipp *ippn10,
	struct dc_context *ctx,
	int inst,
	const struct dcn10_ipp_registers *regs,
	const struct dcn10_ipp_shift *ipp_shift,
	const struct dcn10_ipp_mask *ipp_mask);

<<<<<<< HEAD
=======
void dcn20_ipp_construct(struct dcn10_ipp *ippn10,
	struct dc_context *ctx,
	int inst,
	const struct dcn10_ipp_registers *regs,
	const struct dcn10_ipp_shift *ipp_shift,
	const struct dcn10_ipp_mask *ipp_mask);

>>>>>>> upstream/android-13
#endif /* _DCN10_IPP_H_ */
