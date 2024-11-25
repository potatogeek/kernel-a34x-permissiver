<<<<<<< HEAD
/*
 * aQuantia Corporation Network Driver
 * Copyright (C) 2014-2017 aQuantia Corporation. All rights reserved
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
=======
/* SPDX-License-Identifier: GPL-2.0-only */
/* Atlantic Network Driver
 *
 * Copyright (C) 2014-2019 aQuantia Corporation
 * Copyright (C) 2019-2020 Marvell International Ltd.
>>>>>>> upstream/android-13
 */

/* File aq_hw_utils.h: Declaration of helper functions used across hardware
 * layer.
 */

#ifndef AQ_HW_UTILS_H
#define AQ_HW_UTILS_H

<<<<<<< HEAD
=======
#include <linux/iopoll.h>

>>>>>>> upstream/android-13
#include "aq_common.h"

#ifndef HIDWORD
#define LODWORD(_qw)    ((u32)(_qw))
#define HIDWORD(_qw)    ((u32)(((_qw) >> 32) & 0xffffffff))
#endif

#define AQ_HW_SLEEP(_US_) mdelay(_US_)

<<<<<<< HEAD
#define AQ_HW_WAIT_FOR(_B_, _US_, _N_) \
do { \
	unsigned int AQ_HW_WAIT_FOR_i; \
	for (AQ_HW_WAIT_FOR_i = _N_; (!(_B_)) && (AQ_HW_WAIT_FOR_i);\
	--AQ_HW_WAIT_FOR_i) {\
		udelay(_US_); \
	} \
	if (!AQ_HW_WAIT_FOR_i) {\
		err = -ETIME; \
	} \
} while (0)

=======
>>>>>>> upstream/android-13
#define aq_pr_err(...) pr_err(AQ_CFG_DRV_NAME ": " __VA_ARGS__)
#define aq_pr_trace(...) pr_info(AQ_CFG_DRV_NAME ": " __VA_ARGS__)

struct aq_hw_s;

void aq_hw_write_reg_bit(struct aq_hw_s *aq_hw, u32 addr, u32 msk,
			 u32 shift, u32 val);
u32 aq_hw_read_reg_bit(struct aq_hw_s *aq_hw, u32 addr, u32 msk, u32 shift);
u32 aq_hw_read_reg(struct aq_hw_s *hw, u32 reg);
void aq_hw_write_reg(struct aq_hw_s *hw, u32 reg, u32 value);
<<<<<<< HEAD
int aq_hw_err_from_flags(struct aq_hw_s *hw);
=======
u64 aq_hw_read_reg64(struct aq_hw_s *hw, u32 reg);
void aq_hw_write_reg64(struct aq_hw_s *hw, u32 reg, u64 value);
int aq_hw_err_from_flags(struct aq_hw_s *hw);
int aq_hw_num_tcs(struct aq_hw_s *hw);
int aq_hw_q_per_tc(struct aq_hw_s *hw);
>>>>>>> upstream/android-13

#endif /* AQ_HW_UTILS_H */
