<<<<<<< HEAD
/* Copyright (c) 2016-2018, The Linux Foundation. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
=======
/* SPDX-License-Identifier: GPL-2.0-only */
/* Copyright (c) 2016-2018, The Linux Foundation. All rights reserved.
>>>>>>> upstream/android-13
 */

#ifndef _DPU_HW_INTERRUPTS_H
#define _DPU_HW_INTERRUPTS_H

#include <linux/types.h>

#include "dpu_hwio.h"
#include "dpu_hw_catalog.h"
#include "dpu_hw_util.h"
#include "dpu_hw_mdss.h"

<<<<<<< HEAD
#define IRQ_SOURCE_MDP		BIT(0)
#define IRQ_SOURCE_DSI0		BIT(4)
#define IRQ_SOURCE_DSI1		BIT(5)
#define IRQ_SOURCE_HDMI		BIT(8)
#define IRQ_SOURCE_EDP		BIT(12)
#define IRQ_SOURCE_MHL		BIT(16)

/**
 * dpu_intr_type - HW Interrupt Type
 * @DPU_IRQ_TYPE_WB_ROT_COMP:		WB rotator done
 * @DPU_IRQ_TYPE_WB_WFD_COMP:		WB WFD done
 * @DPU_IRQ_TYPE_PING_PONG_COMP:	PingPong done
 * @DPU_IRQ_TYPE_PING_PONG_RD_PTR:	PingPong read pointer
 * @DPU_IRQ_TYPE_PING_PONG_WR_PTR:	PingPong write pointer
 * @DPU_IRQ_TYPE_PING_PONG_AUTO_REF:	PingPong auto refresh
 * @DPU_IRQ_TYPE_PING_PONG_TEAR_CHECK:	PingPong Tear check
 * @DPU_IRQ_TYPE_PING_PONG_TE_CHECK:	PingPong TE detection
 * @DPU_IRQ_TYPE_INTF_UNDER_RUN:	INTF underrun
 * @DPU_IRQ_TYPE_INTF_VSYNC:		INTF VSYNC
 * @DPU_IRQ_TYPE_CWB_OVERFLOW:		Concurrent WB overflow
 * @DPU_IRQ_TYPE_HIST_VIG_DONE:		VIG Histogram done
 * @DPU_IRQ_TYPE_HIST_VIG_RSTSEQ:	VIG Histogram reset
 * @DPU_IRQ_TYPE_HIST_DSPP_DONE:	DSPP Histogram done
 * @DPU_IRQ_TYPE_HIST_DSPP_RSTSEQ:	DSPP Histogram reset
 * @DPU_IRQ_TYPE_WD_TIMER:		Watchdog timer
 * @DPU_IRQ_TYPE_SFI_VIDEO_IN:		Video static frame INTR into static
 * @DPU_IRQ_TYPE_SFI_VIDEO_OUT:		Video static frame INTR out-of static
 * @DPU_IRQ_TYPE_SFI_CMD_0_IN:		DSI CMD0 static frame INTR into static
 * @DPU_IRQ_TYPE_SFI_CMD_0_OUT:		DSI CMD0 static frame INTR out-of static
 * @DPU_IRQ_TYPE_SFI_CMD_1_IN:		DSI CMD1 static frame INTR into static
 * @DPU_IRQ_TYPE_SFI_CMD_1_OUT:		DSI CMD1 static frame INTR out-of static
 * @DPU_IRQ_TYPE_SFI_CMD_2_IN:		DSI CMD2 static frame INTR into static
 * @DPU_IRQ_TYPE_SFI_CMD_2_OUT:		DSI CMD2 static frame INTR out-of static
 * @DPU_IRQ_TYPE_PROG_LINE:		Programmable Line interrupt
 * @DPU_IRQ_TYPE_AD4_BL_DONE:		AD4 backlight
 * @DPU_IRQ_TYPE_CTL_START:		Control start
 * @DPU_IRQ_TYPE_RESERVED:		Reserved for expansion
 */
enum dpu_intr_type {
	DPU_IRQ_TYPE_WB_ROT_COMP,
	DPU_IRQ_TYPE_WB_WFD_COMP,
	DPU_IRQ_TYPE_PING_PONG_COMP,
	DPU_IRQ_TYPE_PING_PONG_RD_PTR,
	DPU_IRQ_TYPE_PING_PONG_WR_PTR,
	DPU_IRQ_TYPE_PING_PONG_AUTO_REF,
	DPU_IRQ_TYPE_PING_PONG_TEAR_CHECK,
	DPU_IRQ_TYPE_PING_PONG_TE_CHECK,
	DPU_IRQ_TYPE_INTF_UNDER_RUN,
	DPU_IRQ_TYPE_INTF_VSYNC,
	DPU_IRQ_TYPE_CWB_OVERFLOW,
	DPU_IRQ_TYPE_HIST_VIG_DONE,
	DPU_IRQ_TYPE_HIST_VIG_RSTSEQ,
	DPU_IRQ_TYPE_HIST_DSPP_DONE,
	DPU_IRQ_TYPE_HIST_DSPP_RSTSEQ,
	DPU_IRQ_TYPE_WD_TIMER,
	DPU_IRQ_TYPE_SFI_VIDEO_IN,
	DPU_IRQ_TYPE_SFI_VIDEO_OUT,
	DPU_IRQ_TYPE_SFI_CMD_0_IN,
	DPU_IRQ_TYPE_SFI_CMD_0_OUT,
	DPU_IRQ_TYPE_SFI_CMD_1_IN,
	DPU_IRQ_TYPE_SFI_CMD_1_OUT,
	DPU_IRQ_TYPE_SFI_CMD_2_IN,
	DPU_IRQ_TYPE_SFI_CMD_2_OUT,
	DPU_IRQ_TYPE_PROG_LINE,
	DPU_IRQ_TYPE_AD4_BL_DONE,
	DPU_IRQ_TYPE_CTL_START,
	DPU_IRQ_TYPE_RESERVED,
};

=======
/* When making changes be sure to sync with dpu_intr_set */
enum dpu_hw_intr_reg {
	MDP_SSPP_TOP0_INTR,
	MDP_SSPP_TOP0_INTR2,
	MDP_SSPP_TOP0_HIST_INTR,
	MDP_INTF0_INTR,
	MDP_INTF1_INTR,
	MDP_INTF2_INTR,
	MDP_INTF3_INTR,
	MDP_INTF4_INTR,
	MDP_AD4_0_INTR,
	MDP_AD4_1_INTR,
	MDP_INTF0_7xxx_INTR,
	MDP_INTF1_7xxx_INTR,
	MDP_INTF5_7xxx_INTR,
	MDP_INTR_MAX,
};

#define DPU_IRQ_IDX(reg_idx, offset)	(reg_idx * 32 + offset)

>>>>>>> upstream/android-13
struct dpu_hw_intr;

/**
 * Interrupt operations.
 */
struct dpu_hw_intr_ops {
<<<<<<< HEAD
	/**
	 * set_mask - Programs the given interrupt register with the
	 *            given interrupt mask. Register value will get overwritten.
	 * @intr:	HW interrupt handle
	 * @reg_off:	MDSS HW register offset
	 * @irqmask:	IRQ mask value
	 */
	void (*set_mask)(
			struct dpu_hw_intr *intr,
			uint32_t reg,
			uint32_t irqmask);

	/**
	 * irq_idx_lookup - Lookup IRQ index on the HW interrupt type
	 *                 Used for all irq related ops
	 * @intr_type:		Interrupt type defined in dpu_intr_type
	 * @instance_idx:	HW interrupt block instance
	 * @return:		irq_idx or -EINVAL for lookup fail
	 */
	int (*irq_idx_lookup)(
			enum dpu_intr_type intr_type,
			u32 instance_idx);
=======
>>>>>>> upstream/android-13

	/**
	 * enable_irq - Enable IRQ based on lookup IRQ index
	 * @intr:	HW interrupt handle
	 * @irq_idx:	Lookup irq index return from irq_idx_lookup
	 * @return:	0 for success, otherwise failure
	 */
<<<<<<< HEAD
	int (*enable_irq)(
=======
	int (*enable_irq_locked)(
>>>>>>> upstream/android-13
			struct dpu_hw_intr *intr,
			int irq_idx);

	/**
	 * disable_irq - Disable IRQ based on lookup IRQ index
	 * @intr:	HW interrupt handle
	 * @irq_idx:	Lookup irq index return from irq_idx_lookup
	 * @return:	0 for success, otherwise failure
	 */
<<<<<<< HEAD
	int (*disable_irq)(
=======
	int (*disable_irq_locked)(
>>>>>>> upstream/android-13
			struct dpu_hw_intr *intr,
			int irq_idx);

	/**
	 * clear_all_irqs - Clears all the interrupts (i.e. acknowledges
	 *                  any asserted IRQs). Useful during reset.
	 * @intr:	HW interrupt handle
	 * @return:	0 for success, otherwise failure
	 */
	int (*clear_all_irqs)(
			struct dpu_hw_intr *intr);

	/**
	 * disable_all_irqs - Disables all the interrupts. Useful during reset.
	 * @intr:	HW interrupt handle
	 * @return:	0 for success, otherwise failure
	 */
	int (*disable_all_irqs)(
			struct dpu_hw_intr *intr);

	/**
	 * dispatch_irqs - IRQ dispatcher will call the given callback
	 *                 function when a matching interrupt status bit is
	 *                 found in the irq mapping table.
	 * @intr:	HW interrupt handle
	 * @cbfunc:	Callback function pointer
	 * @arg:	Argument to pass back during callback
	 */
	void (*dispatch_irqs)(
			struct dpu_hw_intr *intr,
			void (*cbfunc)(void *arg, int irq_idx),
			void *arg);

	/**
<<<<<<< HEAD
	 * get_interrupt_statuses - Gets and store value from all interrupt
	 *                          status registers that are currently fired.
	 * @intr:	HW interrupt handle
	 */
	void (*get_interrupt_statuses)(
			struct dpu_hw_intr *intr);

	/**
	 * clear_interrupt_status - Clears HW interrupt status based on given
	 *                          lookup IRQ index.
	 * @intr:	HW interrupt handle
	 * @irq_idx:	Lookup irq index return from irq_idx_lookup
	 */
	void (*clear_interrupt_status)(
			struct dpu_hw_intr *intr,
			int irq_idx);

	/**
	 * clear_intr_status_nolock() - clears the HW interrupts without lock
	 * @intr:	HW interrupt handle
	 * @irq_idx:	Lookup irq index return from irq_idx_lookup
	 */
	void (*clear_intr_status_nolock)(
			struct dpu_hw_intr *intr,
			int irq_idx);

	/**
=======
>>>>>>> upstream/android-13
	 * get_interrupt_status - Gets HW interrupt status, and clear if set,
	 *                        based on given lookup IRQ index.
	 * @intr:	HW interrupt handle
	 * @irq_idx:	Lookup irq index return from irq_idx_lookup
	 * @clear:	True to clear irq after read
	 */
	u32 (*get_interrupt_status)(
			struct dpu_hw_intr *intr,
			int irq_idx,
			bool clear);

	/**
<<<<<<< HEAD
	 * get_valid_interrupts - Gets a mask of all valid interrupt sources
	 *                        within DPU. These are actually status bits
	 *                        within interrupt registers that specify the
	 *                        source of the interrupt in IRQs. For example,
	 *                        valid interrupt sources can be MDP, DSI,
	 *                        HDMI etc.
	 * @intr:	HW interrupt handle
	 * @mask:	Returning the interrupt source MASK
	 * @return:	0 for success, otherwise failure
	 */
	int (*get_valid_interrupts)(
			struct dpu_hw_intr *intr,
			uint32_t *mask);
=======
	 * lock - take the IRQ lock
	 * @intr:	HW interrupt handle
	 * @return:	irq_flags for the taken spinlock
	 */
	unsigned long (*lock)(
			struct dpu_hw_intr *intr);

	/**
	 * unlock - take the IRQ lock
	 * @intr:	HW interrupt handle
	 * @irq_flags:  the irq_flags returned from lock
	 */
	void (*unlock)(
			struct dpu_hw_intr *intr, unsigned long irq_flags);
>>>>>>> upstream/android-13
};

/**
 * struct dpu_hw_intr: hw interrupts handling data structure
 * @hw:               virtual address mapping
 * @ops:              function pointer mapping for IRQ handling
 * @cache_irq_mask:   array of IRQ enable masks reg storage created during init
 * @save_irq_status:  array of IRQ status reg storage created during init
<<<<<<< HEAD
 * @irq_idx_tbl_size: total number of irq_idx mapped in the hw_interrupts
=======
 * @total_irqs: total number of irq_idx mapped in the hw_interrupts
>>>>>>> upstream/android-13
 * @irq_lock:         spinlock for accessing IRQ resources
 */
struct dpu_hw_intr {
	struct dpu_hw_blk_reg_map hw;
	struct dpu_hw_intr_ops ops;
	u32 *cache_irq_mask;
	u32 *save_irq_status;
<<<<<<< HEAD
	u32 irq_idx_tbl_size;
	spinlock_t irq_lock;
=======
	u32 total_irqs;
	spinlock_t irq_lock;
	unsigned long irq_mask;
>>>>>>> upstream/android-13
};

/**
 * dpu_hw_intr_init(): Initializes the interrupts hw object
 * @addr: mapped register io address of MDP
 * @m :   pointer to mdss catalog data
 */
struct dpu_hw_intr *dpu_hw_intr_init(void __iomem *addr,
		struct dpu_mdss_cfg *m);

/**
 * dpu_hw_intr_destroy(): Cleanup interrutps hw object
 * @intr: pointer to interrupts hw object
 */
void dpu_hw_intr_destroy(struct dpu_hw_intr *intr);
#endif
