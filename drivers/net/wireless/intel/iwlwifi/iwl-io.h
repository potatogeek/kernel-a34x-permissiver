<<<<<<< HEAD
/******************************************************************************
 *
 * Copyright(c) 2003 - 2014 Intel Corporation. All rights reserved.
 *
 * Portions of this file are derived from the ipw3945 project.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of version 2 of the GNU General Public License as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110, USA
 *
 * The full GNU General Public License is included in this distribution in the
 * file called LICENSE.
 *
 * Contact Information:
 *  Intel Linux Wireless <linuxwifi@intel.com>
 * Intel Corporation, 5200 N.E. Elam Young Parkway, Hillsboro, OR 97124-6497
 *
 *****************************************************************************/

=======
/* SPDX-License-Identifier: GPL-2.0 OR BSD-3-Clause */
/*
 * Copyright (C) 2018-2020 Intel Corporation
 */
>>>>>>> upstream/android-13
#ifndef __iwl_io_h__
#define __iwl_io_h__

#include "iwl-devtrace.h"
#include "iwl-trans.h"

void iwl_write8(struct iwl_trans *trans, u32 ofs, u8 val);
void iwl_write32(struct iwl_trans *trans, u32 ofs, u32 val);
void iwl_write64(struct iwl_trans *trans, u64 ofs, u64 val);
u32 iwl_read32(struct iwl_trans *trans, u32 ofs);

static inline void iwl_set_bit(struct iwl_trans *trans, u32 reg, u32 mask)
{
	iwl_trans_set_bits_mask(trans, reg, mask, mask);
}

static inline void iwl_clear_bit(struct iwl_trans *trans, u32 reg, u32 mask)
{
	iwl_trans_set_bits_mask(trans, reg, mask, 0);
}

int iwl_poll_bit(struct iwl_trans *trans, u32 addr,
		 u32 bits, u32 mask, int timeout);
int iwl_poll_direct_bit(struct iwl_trans *trans, u32 addr, u32 mask,
			int timeout);

u32 iwl_read_direct32(struct iwl_trans *trans, u32 reg);
void iwl_write_direct32(struct iwl_trans *trans, u32 reg, u32 value);
void iwl_write_direct64(struct iwl_trans *trans, u64 reg, u64 value);


u32 iwl_read_prph_no_grab(struct iwl_trans *trans, u32 ofs);
u32 iwl_read_prph(struct iwl_trans *trans, u32 ofs);
void iwl_write_prph_no_grab(struct iwl_trans *trans, u32 ofs, u32 val);
void iwl_write_prph64_no_grab(struct iwl_trans *trans, u64 ofs, u64 val);
<<<<<<< HEAD
void iwl_write_prph(struct iwl_trans *trans, u32 ofs, u32 val);
=======
void iwl_write_prph_delay(struct iwl_trans *trans, u32 ofs,
			  u32 val, u32 delay_ms);
static inline void iwl_write_prph(struct iwl_trans *trans, u32 ofs, u32 val)
{
	iwl_write_prph_delay(trans, ofs, val, 0);
}

>>>>>>> upstream/android-13
int iwl_poll_prph_bit(struct iwl_trans *trans, u32 addr,
		      u32 bits, u32 mask, int timeout);
void iwl_set_bits_prph(struct iwl_trans *trans, u32 ofs, u32 mask);
void iwl_set_bits_mask_prph(struct iwl_trans *trans, u32 ofs,
			    u32 bits, u32 mask);
void iwl_clear_bits_prph(struct iwl_trans *trans, u32 ofs, u32 mask);
void iwl_force_nmi(struct iwl_trans *trans);

<<<<<<< HEAD
/* Error handling */
int iwl_dump_fh(struct iwl_trans *trans, char **buf);

=======
int iwl_finish_nic_init(struct iwl_trans *trans,
			const struct iwl_cfg_trans_params *cfg_trans);

/* Error handling */
int iwl_dump_fh(struct iwl_trans *trans, char **buf);

/*
 * UMAC periphery address space changed from 0xA00000 to 0xD00000 starting from
 * device family AX200. So peripheries used in families above and below AX200
 * should go through iwl_..._umac_..._prph.
 */
static inline u32 iwl_umac_prph(struct iwl_trans *trans, u32 ofs)
{
	return ofs + trans->trans_cfg->umac_prph_offset;
}

static inline u32 iwl_read_umac_prph_no_grab(struct iwl_trans *trans, u32 ofs)
{
	return iwl_read_prph_no_grab(trans, ofs +
				     trans->trans_cfg->umac_prph_offset);
}

static inline u32 iwl_read_umac_prph(struct iwl_trans *trans, u32 ofs)
{
	return iwl_read_prph(trans, ofs + trans->trans_cfg->umac_prph_offset);
}

static inline void iwl_write_umac_prph_no_grab(struct iwl_trans *trans, u32 ofs,
					       u32 val)
{
	iwl_write_prph_no_grab(trans,  ofs + trans->trans_cfg->umac_prph_offset,
			       val);
}

static inline void iwl_write_umac_prph(struct iwl_trans *trans, u32 ofs,
				       u32 val)
{
	iwl_write_prph(trans,  ofs + trans->trans_cfg->umac_prph_offset, val);
}

static inline int iwl_poll_umac_prph_bit(struct iwl_trans *trans, u32 addr,
					 u32 bits, u32 mask, int timeout)
{
	return iwl_poll_prph_bit(trans, addr +
				 trans->trans_cfg->umac_prph_offset,
				 bits, mask, timeout);
}

>>>>>>> upstream/android-13
#endif
