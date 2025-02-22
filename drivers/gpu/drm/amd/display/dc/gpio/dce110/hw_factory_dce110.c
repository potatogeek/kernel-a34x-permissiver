/*
 * Copyright 2013-15 Advanced Micro Devices, Inc.
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

#include "dm_services.h"
<<<<<<< HEAD
#include "include/gpio_types.h"
#include "../hw_factory.h"

=======

#include "include/gpio_types.h"
#include "../hw_factory.h"

#include "../hw_gpio.h"
#include "../hw_ddc.h"
#include "../hw_hpd.h"
#include "../hw_generic.h"

>>>>>>> upstream/android-13
#include "hw_factory_dce110.h"

#include "dce/dce_11_0_d.h"
#include "dce/dce_11_0_sh_mask.h"

/* set field name */
#define SF_HPD(reg_name, field_name, post_fix)\
	.field_name = reg_name ## __ ## field_name ## post_fix

#define REG(reg_name)\
		mm ## reg_name

#define REGI(reg_name, block, id)\
	mm ## block ## id ## _ ## reg_name

<<<<<<< HEAD
#include "../hw_gpio.h"
#include "../hw_ddc.h"
#include "../hw_hpd.h"

=======
>>>>>>> upstream/android-13
#include "reg_helper.h"
#include "../hpd_regs.h"

#define hpd_regs(id) \
{\
	HPD_REG_LIST(id)\
}

static const struct hpd_registers hpd_regs[] = {
	hpd_regs(0),
	hpd_regs(1),
	hpd_regs(2),
	hpd_regs(3),
	hpd_regs(4),
	hpd_regs(5)
};

static const struct hpd_sh_mask hpd_shift = {
		HPD_MASK_SH_LIST(__SHIFT)
};

static const struct hpd_sh_mask hpd_mask = {
		HPD_MASK_SH_LIST(_MASK)
};

#include "../ddc_regs.h"

 /* set field name */
#define SF_DDC(reg_name, field_name, post_fix)\
	.field_name = reg_name ## __ ## field_name ## post_fix

static const struct ddc_registers ddc_data_regs[] = {
	ddc_data_regs(1),
	ddc_data_regs(2),
	ddc_data_regs(3),
	ddc_data_regs(4),
	ddc_data_regs(5),
	ddc_data_regs(6),
	ddc_vga_data_regs,
	ddc_i2c_data_regs
};

static const struct ddc_registers ddc_clk_regs[] = {
	ddc_clk_regs(1),
	ddc_clk_regs(2),
	ddc_clk_regs(3),
	ddc_clk_regs(4),
	ddc_clk_regs(5),
	ddc_clk_regs(6),
	ddc_vga_clk_regs,
	ddc_i2c_clk_regs
};

static const struct ddc_sh_mask ddc_shift = {
		DDC_MASK_SH_LIST(__SHIFT)
};

static const struct ddc_sh_mask ddc_mask = {
		DDC_MASK_SH_LIST(_MASK)
};

static void define_ddc_registers(
		struct hw_gpio_pin *pin,
		uint32_t en)
{
	struct hw_ddc *ddc = HW_DDC_FROM_BASE(pin);

	switch (pin->id) {
	case GPIO_ID_DDC_DATA:
		ddc->regs = &ddc_data_regs[en];
		ddc->base.regs = &ddc_data_regs[en].gpio;
		break;
	case GPIO_ID_DDC_CLOCK:
		ddc->regs = &ddc_clk_regs[en];
		ddc->base.regs = &ddc_clk_regs[en].gpio;
		break;
	default:
		ASSERT_CRITICAL(false);
		return;
	}

	ddc->shifts = &ddc_shift;
	ddc->masks = &ddc_mask;

}

static void define_hpd_registers(struct hw_gpio_pin *pin, uint32_t en)
{
	struct hw_hpd *hpd = HW_HPD_FROM_BASE(pin);

	hpd->regs = &hpd_regs[en];
	hpd->shifts = &hpd_shift;
	hpd->masks = &hpd_mask;
	hpd->base.regs = &hpd_regs[en].gpio;
}

static const struct hw_factory_funcs funcs = {
<<<<<<< HEAD
	.create_ddc_data = dal_hw_ddc_create,
	.create_ddc_clock = dal_hw_ddc_create,
	.create_generic = NULL,
	.create_hpd = dal_hw_hpd_create,
	.create_sync = NULL,
	.create_gsl = NULL,
=======
	.init_ddc_data = dal_hw_ddc_init,
	.init_generic = NULL,
	.init_hpd = dal_hw_hpd_init,
	.get_ddc_pin = dal_hw_ddc_get_pin,
	.get_hpd_pin = dal_hw_hpd_get_pin,
	.get_generic_pin = NULL,
>>>>>>> upstream/android-13
	.define_hpd_registers = define_hpd_registers,
	.define_ddc_registers = define_ddc_registers
};

/*
 * dal_hw_factory_dce110_init
 *
 * @brief
 * Initialize HW factory function pointers and pin info
 *
 * @param
 * struct hw_factory *factory - [out] struct of function pointers
 */
void dal_hw_factory_dce110_init(struct hw_factory *factory)
{
	/*TODO check ASIC CAPs*/
	factory->number_of_pins[GPIO_ID_DDC_DATA] = 8;
	factory->number_of_pins[GPIO_ID_DDC_CLOCK] = 8;
	factory->number_of_pins[GPIO_ID_GENERIC] = 7;
	factory->number_of_pins[GPIO_ID_HPD] = 6;
	factory->number_of_pins[GPIO_ID_GPIO_PAD] = 31;
	factory->number_of_pins[GPIO_ID_VIP_PAD] = 0;
	factory->number_of_pins[GPIO_ID_SYNC] = 2;
	factory->number_of_pins[GPIO_ID_GSL] = 4;

	factory->funcs = &funcs;
}
