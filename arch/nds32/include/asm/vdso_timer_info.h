<<<<<<< HEAD
// SPDX-License-Identifier: GPL-2.0
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> upstream/android-13
// Copyright (C) 2005-2017 Andes Technology Corporation

extern struct timer_info_t timer_info;
#define EMPTY_VALUE ~(0UL)
#define EMPTY_TIMER_MAPPING EMPTY_VALUE
#define EMPTY_REG_OFFSET EMPTY_VALUE

struct timer_info_t
{
	bool cycle_count_down;
	unsigned long mapping_base;
	unsigned long cycle_count_reg_offset;
};
