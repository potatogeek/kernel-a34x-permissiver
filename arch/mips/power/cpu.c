<<<<<<< HEAD
/*
 * Suspend support specific for mips.
 *
 * Licensed under the GPLv2
 *
=======
// SPDX-License-Identifier: GPL-2.0-only
/*
 * Suspend support specific for mips.
 *
>>>>>>> upstream/android-13
 * Copyright (C) 2009 Lemote Inc.
 * Author: Hu Hongbing <huhb@lemote.com>
 *	   Wu Zhangjin <wuzhangjin@gmail.com>
 */
#include <asm/sections.h>
#include <asm/fpu.h>
#include <asm/dsp.h>

static u32 saved_status;
struct pt_regs saved_regs;

void save_processor_state(void)
{
	saved_status = read_c0_status();

	if (is_fpu_owner())
		save_fp(current);
<<<<<<< HEAD
	if (cpu_has_dsp)
		save_dsp(current);
=======

	save_dsp(current);
>>>>>>> upstream/android-13
}

void restore_processor_state(void)
{
	write_c0_status(saved_status);

	if (is_fpu_owner())
		restore_fp(current);
<<<<<<< HEAD
	if (cpu_has_dsp)
		restore_dsp(current);
=======

	restore_dsp(current);
>>>>>>> upstream/android-13
}

int pfn_is_nosave(unsigned long pfn)
{
	unsigned long nosave_begin_pfn = PFN_DOWN(__pa(&__nosave_begin));
	unsigned long nosave_end_pfn = PFN_UP(__pa(&__nosave_end));

	return	(pfn >= nosave_begin_pfn) && (pfn < nosave_end_pfn);
}
