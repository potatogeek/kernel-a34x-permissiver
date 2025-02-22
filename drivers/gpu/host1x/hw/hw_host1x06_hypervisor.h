<<<<<<< HEAD
/*
 * Copyright (c) 2017 NVIDIA Corporation.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
=======
/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Copyright (c) 2017 NVIDIA Corporation.
>>>>>>> upstream/android-13
 */

#define HOST1X_HV_SYNCPT_PROT_EN			0x1ac4
#define HOST1X_HV_SYNCPT_PROT_EN_CH_EN			BIT(1)
#define HOST1X_HV_CH_KERNEL_FILTER_GBUFFER(x)		(0x2020 + (x * 4))
#define HOST1X_HV_CMDFIFO_PEEK_CTRL			0x233c
#define HOST1X_HV_CMDFIFO_PEEK_CTRL_ADDR(x)		(x)
#define HOST1X_HV_CMDFIFO_PEEK_CTRL_CHANNEL(x)		((x) << 16)
#define HOST1X_HV_CMDFIFO_PEEK_CTRL_ENABLE		BIT(31)
#define HOST1X_HV_CMDFIFO_PEEK_READ			0x2340
#define HOST1X_HV_CMDFIFO_PEEK_PTRS			0x2344
#define HOST1X_HV_CMDFIFO_PEEK_PTRS_WR_PTR_V(x)		(((x) >> 16) & 0xfff)
#define HOST1X_HV_CMDFIFO_PEEK_PTRS_RD_PTR_V(x)		((x) & 0xfff)
#define HOST1X_HV_CMDFIFO_SETUP(x)			(0x2588 + (x * 4))
#define HOST1X_HV_CMDFIFO_SETUP_LIMIT_V(x)		(((x) >> 16) & 0xfff)
#define HOST1X_HV_CMDFIFO_SETUP_BASE_V(x)		((x) & 0xfff)
#define HOST1X_HV_ICG_EN_OVERRIDE			0x2aa8
