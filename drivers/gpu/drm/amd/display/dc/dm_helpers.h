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

/**
 * This file defines helper functions provided by the Display Manager to
 * Display Core.
 */
#ifndef __DM_HELPERS__
#define __DM_HELPERS__

#include "dc_types.h"
#include "dc.h"

struct dp_mst_stream_allocation_table;
<<<<<<< HEAD
=======
struct aux_payload;
enum aux_return_code_type;

/*
 * Allocate memory accessible by the GPU
 *
 * frame buffer allocations must be aligned to a 4096-byte boundary
 *
 * Returns virtual address, sets addr to physical address
 */
void *dm_helpers_allocate_gpu_mem(
		struct dc_context *ctx,
		enum dc_gpu_mem_alloc_type type,
		size_t size,
		long long *addr);

/*
 * Free the GPU-accessible memory at the virtual address pvMem
 */
void dm_helpers_free_gpu_mem(
		struct dc_context *ctx,
		enum dc_gpu_mem_alloc_type type,
		void *pvMem);
>>>>>>> upstream/android-13

enum dc_edid_status dm_helpers_parse_edid_caps(
	struct dc_context *ctx,
	const struct dc_edid *edid,
	struct dc_edid_caps *edid_caps);


/*
 * Update DP branch info
 */
void dm_helpers_dp_update_branch_info(
		struct dc_context *ctx,
		const struct dc_link *link);

/*
 * Writes payload allocation table in immediate downstream device.
 */
bool dm_helpers_dp_mst_write_payload_allocation_table(
		struct dc_context *ctx,
		const struct dc_stream_state *stream,
		struct dp_mst_stream_allocation_table *proposed_table,
		bool enable);

/*
<<<<<<< HEAD
=======
 * poll pending down reply
 */
void dm_helpers_dp_mst_poll_pending_down_reply(
	struct dc_context *ctx,
	const struct dc_link *link);

/*
>>>>>>> upstream/android-13
 * Clear payload allocation table before enable MST DP link.
 */
void dm_helpers_dp_mst_clear_payload_allocation_table(
	struct dc_context *ctx,
	const struct dc_link *link);

/*
 * Polls for ACT (allocation change trigger) handled and
 */
<<<<<<< HEAD
bool dm_helpers_dp_mst_poll_for_allocation_change_trigger(
=======
enum act_return_status dm_helpers_dp_mst_poll_for_allocation_change_trigger(
>>>>>>> upstream/android-13
		struct dc_context *ctx,
		const struct dc_stream_state *stream);
/*
 * Sends ALLOCATE_PAYLOAD message.
 */
bool dm_helpers_dp_mst_send_payload_allocation(
		struct dc_context *ctx,
		const struct dc_stream_state *stream,
		bool enable);

bool dm_helpers_dp_mst_start_top_mgr(
		struct dc_context *ctx,
		const struct dc_link *link,
		bool boot);

void dm_helpers_dp_mst_stop_top_mgr(
		struct dc_context *ctx,
<<<<<<< HEAD
		const struct dc_link *link);
=======
		struct dc_link *link);
>>>>>>> upstream/android-13
/**
 * OS specific aux read callback.
 */
bool dm_helpers_dp_read_dpcd(
		struct dc_context *ctx,
		const struct dc_link *link,
		uint32_t address,
		uint8_t *data,
		uint32_t size);

/**
 * OS specific aux write callback.
 */
bool dm_helpers_dp_write_dpcd(
		struct dc_context *ctx,
		const struct dc_link *link,
		uint32_t address,
		const uint8_t *data,
		uint32_t size);

bool dm_helpers_submit_i2c(
		struct dc_context *ctx,
		const struct dc_link *link,
		struct i2c_command *cmd);

<<<<<<< HEAD
bool dm_helpers_is_dp_sink_present(
		struct dc_link *link);

=======
bool dm_helpers_dp_write_dsc_enable(
		struct dc_context *ctx,
		const struct dc_stream_state *stream,
		bool enable
);
bool dm_helpers_is_dp_sink_present(
		struct dc_link *link);

void dm_helpers_mst_enable_stream_features(const struct dc_stream_state *stream);

>>>>>>> upstream/android-13
enum dc_edid_status dm_helpers_read_local_edid(
		struct dc_context *ctx,
		struct dc_link *link,
		struct dc_sink *sink);

void dm_set_dcn_clocks(
		struct dc_context *ctx,
		struct dc_clocks *clks);

<<<<<<< HEAD
=======
bool dm_helpers_dmub_outbox_interrupt_control(struct dc_context *ctx, bool enable);

void dm_helpers_smu_timeout(struct dc_context *ctx, unsigned int msg_id, unsigned int param, unsigned int timeout_us);

// 0x1 = Result_OK, 0xFE = Result_UnkmownCmd
#define IS_SMU_TIMEOUT(result) \
	(!(result == 0x1 || result == 0xFE))

int dm_helper_dmub_aux_transfer_sync(
		struct dc_context *ctx,
		const struct dc_link *link,
		struct aux_payload *payload,
		enum aux_return_code_type *operation_result);
>>>>>>> upstream/android-13
#endif /* __DM_HELPERS__ */
