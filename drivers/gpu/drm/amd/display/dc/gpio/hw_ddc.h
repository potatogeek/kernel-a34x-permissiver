/*
 * Copyright 2012-15 Advanced Micro Devices, Inc.
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

#ifndef __DAL_HW_DDC_H__
#define __DAL_HW_DDC_H__

#include "ddc_regs.h"

struct hw_ddc {
	struct hw_gpio base;
	const struct ddc_registers *regs;
	const struct ddc_sh_mask *shifts;
	const struct ddc_sh_mask *masks;
};

#define HW_DDC_FROM_BASE(hw_gpio) \
	container_of((HW_GPIO_FROM_BASE(hw_gpio)), struct hw_ddc, base)

<<<<<<< HEAD
struct hw_gpio_pin *dal_hw_ddc_create(
=======
void dal_hw_ddc_init(
	struct hw_ddc **hw_ddc,
>>>>>>> upstream/android-13
	struct dc_context *ctx,
	enum gpio_id id,
	uint32_t en);

<<<<<<< HEAD
=======
struct hw_gpio_pin *dal_hw_ddc_get_pin(struct gpio *gpio);

>>>>>>> upstream/android-13
#endif
