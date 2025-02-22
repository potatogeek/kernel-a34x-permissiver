<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> upstream/android-13
/*
 * Intel SKL IPC Support
 *
 * Copyright (C) 2014-15, Intel Corporation.
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as version 2, as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
=======
>>>>>>> upstream/android-13
 */

#ifndef __SKL_IPC_H
#define __SKL_IPC_H

#include <linux/irqreturn.h>
#include "../common/sst-ipc.h"
<<<<<<< HEAD

struct sst_dsp;
struct skl_sst;
=======
#include "skl-sst-dsp.h"

struct sst_dsp;
>>>>>>> upstream/android-13
struct sst_generic_ipc;

enum skl_ipc_pipeline_state {
	PPL_INVALID_STATE =	0,
	PPL_UNINITIALIZED =	1,
	PPL_RESET =		2,
	PPL_PAUSED =		3,
	PPL_RUNNING =		4,
	PPL_ERROR_STOP =	5,
	PPL_SAVED =		6,
	PPL_RESTORED =		7
};

struct skl_ipc_dxstate_info {
	u32 core_mask;
	u32 dx_mask;
};

struct skl_ipc_header {
	u32 primary;
	u32 extension;
};

struct skl_dsp_cores {
	unsigned int count;
	enum skl_dsp_states *state;
	int *usage_count;
};

/**
 * skl_d0i3_data: skl D0i3 counters data struct
 *
 * @streaming: Count of usecases that can attempt streaming D0i3
 * @non_streaming: Count of usecases that can attempt non-streaming D0i3
 * @non_d0i3: Count of usecases that cannot attempt D0i3
 * @state: current state
 * @work: D0i3 worker thread
 */
struct skl_d0i3_data {
	int streaming;
	int non_streaming;
	int non_d0i3;
	enum skl_dsp_d0i3_states state;
	struct delayed_work work;
};

#define SKL_LIB_NAME_LENGTH 128
#define SKL_MAX_LIB 16

struct skl_lib_info {
	char name[SKL_LIB_NAME_LENGTH];
	const struct firmware *fw;
};

<<<<<<< HEAD
struct skl_sst {
	struct device *dev;
	struct sst_dsp *dsp;

	/* boot */
	wait_queue_head_t boot_wait;
	bool boot_complete;

	/* module load */
	wait_queue_head_t mod_load_wait;
	bool mod_load_complete;
	bool mod_load_status;

	/* IPC messaging */
	struct sst_generic_ipc ipc;

	/* callback for miscbdge */
	void (*enable_miscbdcge)(struct device *dev, bool enable);
	/* Is CGCTL.MISCBDCGE disabled */
	bool miscbdcg_disabled;

	/* Populate module information */
	struct list_head uuid_list;

	/* Is firmware loaded */
	bool fw_loaded;

	/* first boot ? */
	bool is_first_boot;

	/* multi-core */
	struct skl_dsp_cores cores;

	/* library info */
	struct skl_lib_info  lib_info[SKL_MAX_LIB];
	int lib_count;

	/* Callback to update D0i3C register */
	void (*update_d0i3c)(struct device *dev, bool enable);

	struct skl_d0i3_data d0i3;

	const struct skl_dsp_ops *dsp_ops;

	/* Callback to update dynamic clock and power gating registers */
	void (*clock_power_gating)(struct device *dev, bool enable);
};

=======
>>>>>>> upstream/android-13
struct skl_ipc_init_instance_msg {
	u32 module_id;
	u32 instance_id;
	u16 param_data_size;
	u8 ppl_instance_id;
	u8 core_id;
	u8 domain;
};

struct skl_ipc_bind_unbind_msg {
	u32 module_id;
	u32 instance_id;
	u32 dst_module_id;
	u32 dst_instance_id;
	u8 src_queue;
	u8 dst_queue;
	bool bind;
};

struct skl_ipc_large_config_msg {
	u32 module_id;
	u32 instance_id;
	u32 large_param_id;
	u32 param_data_size;
};

struct skl_ipc_d0ix_msg {
	u32 module_id;
	u32 instance_id;
	u8 streaming;
	u8 wake;
};

#define SKL_IPC_BOOT_MSECS		3000

#define SKL_IPC_D3_MASK	0
#define SKL_IPC_D0_MASK	3

irqreturn_t skl_dsp_irq_thread_handler(int irq, void *context);

<<<<<<< HEAD
int skl_ipc_create_pipeline(struct sst_generic_ipc *sst_ipc,
		u16 ppl_mem_size, u8 ppl_type, u8 instance_id, u8 lp_mode);

int skl_ipc_delete_pipeline(struct sst_generic_ipc *sst_ipc, u8 instance_id);

int skl_ipc_set_pipeline_state(struct sst_generic_ipc *sst_ipc,
=======
int skl_ipc_create_pipeline(struct sst_generic_ipc *ipc,
		u16 ppl_mem_size, u8 ppl_type, u8 instance_id, u8 lp_mode);

int skl_ipc_delete_pipeline(struct sst_generic_ipc *ipc, u8 instance_id);

int skl_ipc_set_pipeline_state(struct sst_generic_ipc *ipc,
>>>>>>> upstream/android-13
		u8 instance_id,	enum skl_ipc_pipeline_state state);

int skl_ipc_save_pipeline(struct sst_generic_ipc *ipc,
		u8 instance_id, int dma_id);

int skl_ipc_restore_pipeline(struct sst_generic_ipc *ipc, u8 instance_id);

<<<<<<< HEAD
int skl_ipc_init_instance(struct sst_generic_ipc *sst_ipc,
		struct skl_ipc_init_instance_msg *msg, void *param_data);

int skl_ipc_bind_unbind(struct sst_generic_ipc *sst_ipc,
=======
int skl_ipc_init_instance(struct sst_generic_ipc *ipc,
		struct skl_ipc_init_instance_msg *msg, void *param_data);

int skl_ipc_bind_unbind(struct sst_generic_ipc *ipc,
>>>>>>> upstream/android-13
		struct skl_ipc_bind_unbind_msg *msg);

int skl_ipc_load_modules(struct sst_generic_ipc *ipc,
				u8 module_cnt, void *data);

int skl_ipc_unload_modules(struct sst_generic_ipc *ipc,
				u8 module_cnt, void *data);

int skl_ipc_set_dx(struct sst_generic_ipc *ipc,
		u8 instance_id, u16 module_id, struct skl_ipc_dxstate_info *dx);

int skl_ipc_set_large_config(struct sst_generic_ipc *ipc,
		struct skl_ipc_large_config_msg *msg, u32 *param);

int skl_ipc_get_large_config(struct sst_generic_ipc *ipc,
<<<<<<< HEAD
		struct skl_ipc_large_config_msg *msg, u32 *param);
=======
		struct skl_ipc_large_config_msg *msg,
		u32 **payload, size_t *bytes);
>>>>>>> upstream/android-13

int skl_sst_ipc_load_library(struct sst_generic_ipc *ipc,
			u8 dma_id, u8 table_id, bool wait);

int skl_ipc_set_d0ix(struct sst_generic_ipc *ipc,
		struct skl_ipc_d0ix_msg *msg);

int skl_ipc_check_D0i0(struct sst_dsp *dsp, bool state);

<<<<<<< HEAD
void skl_ipc_int_enable(struct sst_dsp *dsp);
void skl_ipc_op_int_enable(struct sst_dsp *ctx);
void skl_ipc_op_int_disable(struct sst_dsp *ctx);
void skl_ipc_int_disable(struct sst_dsp *dsp);

bool skl_ipc_int_status(struct sst_dsp *dsp);
void skl_ipc_free(struct sst_generic_ipc *ipc);
int skl_ipc_init(struct device *dev, struct skl_sst *skl);
=======
void skl_ipc_int_enable(struct sst_dsp *ctx);
void skl_ipc_op_int_enable(struct sst_dsp *ctx);
void skl_ipc_op_int_disable(struct sst_dsp *ctx);
void skl_ipc_int_disable(struct sst_dsp *ctx);

bool skl_ipc_int_status(struct sst_dsp *ctx);
void skl_ipc_free(struct sst_generic_ipc *ipc);
int skl_ipc_init(struct device *dev, struct skl_dev *skl);
>>>>>>> upstream/android-13
void skl_clear_module_cnt(struct sst_dsp *ctx);

void skl_ipc_process_reply(struct sst_generic_ipc *ipc,
		struct skl_ipc_header header);
int skl_ipc_process_notification(struct sst_generic_ipc *ipc,
		struct skl_ipc_header header);
void skl_ipc_tx_data_copy(struct ipc_message *msg, char *tx_data,
		size_t tx_size);
#endif /* __SKL_IPC_H */
