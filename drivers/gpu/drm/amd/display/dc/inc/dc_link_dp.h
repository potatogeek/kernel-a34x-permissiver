/*
 * Copyright 2015 Advanced Micro Devices, Inc.
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

#ifndef __DC_LINK_DP_H__
#define __DC_LINK_DP_H__

#define LINK_TRAINING_ATTEMPTS 4
#define LINK_TRAINING_RETRY_DELAY 50 /* ms */
<<<<<<< HEAD
=======
#define LINK_AUX_DEFAULT_LTTPR_TIMEOUT_PERIOD 3200 /*us*/
#define LINK_AUX_DEFAULT_TIMEOUT_PERIOD 552 /*us*/
#define DP_REPEATER_CONFIGURATION_AND_STATUS_SIZE 0x50
#define TRAINING_AUX_RD_INTERVAL 100 //us
>>>>>>> upstream/android-13

struct dc_link;
struct dc_stream_state;
struct dc_link_settings;

<<<<<<< HEAD
=======
enum {
	LINK_TRAINING_MAX_RETRY_COUNT = 5,
	/* to avoid infinite loop where-in the receiver
	 * switches between different VS
	 */
	LINK_TRAINING_MAX_CR_RETRY = 100,
	/*
	 * Some receivers fail to train on first try and are good
	 * on subsequent tries. 2 retries should be plenty. If we
	 * don't have a successful training then we don't expect to
	 * ever get one.
	 */
	LINK_TRAINING_MAX_VERIFY_RETRY = 2,
	PEAK_FACTOR_X1000 = 1006,
};

>>>>>>> upstream/android-13
bool dp_verify_link_cap(
	struct dc_link *link,
	struct dc_link_settings *known_limit_link_setting,
	int *fail_count);

<<<<<<< HEAD
=======
bool dp_verify_link_cap_with_retries(
	struct dc_link *link,
	struct dc_link_settings *known_limit_link_setting,
	int attempts);

bool dp_verify_mst_link_cap(
	struct dc_link *link);

>>>>>>> upstream/android-13
bool dp_validate_mode_timing(
	struct dc_link *link,
	const struct dc_crtc_timing *timing);

<<<<<<< HEAD
=======
bool decide_edp_link_settings(struct dc_link *link,
		struct dc_link_settings *link_setting,
		uint32_t req_bw);

>>>>>>> upstream/android-13
void decide_link_settings(
	struct dc_stream_state *stream,
	struct dc_link_settings *link_setting);

bool perform_link_training_with_retries(
<<<<<<< HEAD
	struct dc_link *link,
	const struct dc_link_settings *link_setting,
	bool skip_video_pattern,
	int attempts);
=======
	const struct dc_link_settings *link_setting,
	bool skip_video_pattern,
	int attempts,
	struct pipe_ctx *pipe_ctx,
	enum signal_type signal,
	bool do_fallback);

bool hpd_rx_irq_check_link_loss_status(
	struct dc_link *link,
	union hpd_irq_data *hpd_irq_dpcd_data);
>>>>>>> upstream/android-13

bool is_mst_supported(struct dc_link *link);

bool detect_dp_sink_caps(struct dc_link *link);

void detect_edp_sink_caps(struct dc_link *link);

bool is_dp_active_dongle(const struct dc_link *link);

<<<<<<< HEAD
void dp_enable_mst_on_sink(struct dc_link *link, bool enable);

=======
bool is_dp_branch_device(const struct dc_link *link);

bool is_edp_ilr_optimization_required(struct dc_link *link, struct dc_crtc_timing *crtc_timing);

void dp_enable_mst_on_sink(struct dc_link *link, bool enable);

enum dp_panel_mode dp_get_panel_mode(struct dc_link *link);
void dp_set_panel_mode(struct dc_link *link, enum dp_panel_mode panel_mode);

bool dp_overwrite_extended_receiver_cap(struct dc_link *link);

void dpcd_set_source_specific_data(struct dc_link *link);
/* Write DPCD link configuration data. */
enum dc_status dpcd_set_link_settings(
	struct dc_link *link,
	const struct link_training_settings *lt_settings);
/* Write DPCD drive settings. */
enum dc_status dpcd_set_lane_settings(
	struct dc_link *link,
	const struct link_training_settings *link_training_setting,
	uint32_t offset);
/* Read training status and adjustment requests from DPCD. */
enum dc_status dp_get_lane_status_and_drive_settings(
	struct dc_link *link,
	const struct link_training_settings *link_training_setting,
	union lane_status *ln_status,
	union lane_align_status_updated *ln_status_updated,
	struct link_training_settings *req_settings,
	uint32_t offset);

void dp_wait_for_training_aux_rd_interval(
	struct dc_link *link,
	uint32_t wait_in_micro_secs);

bool dp_is_cr_done(enum dc_lane_count ln_count,
	union lane_status *dpcd_lane_status);

enum link_training_result dp_get_cr_failure(enum dc_lane_count ln_count,
	union lane_status *dpcd_lane_status);

bool dp_is_ch_eq_done(enum dc_lane_count ln_count,
	union lane_status *dpcd_lane_status);
bool dp_is_symbol_locked(enum dc_lane_count ln_count,
	union lane_status *dpcd_lane_status);
bool dp_is_interlane_aligned(union lane_align_status_updated align_status);

bool dp_is_max_vs_reached(
	const struct link_training_settings *lt_settings);

void dp_update_drive_settings(
	struct link_training_settings *dest,
	struct link_training_settings src);

uint32_t dp_translate_training_aux_read_interval(uint32_t dpcd_aux_read_interval);

enum dpcd_training_patterns
	dc_dp_training_pattern_to_dpcd_training_pattern(
	struct dc_link *link,
	enum dc_dp_training_pattern pattern);

uint8_t dc_dp_initialize_scrambling_data_symbols(
	struct dc_link *link,
	enum dc_dp_training_pattern pattern);

enum dc_status dp_set_fec_ready(struct dc_link *link, bool ready);
void dp_set_fec_enable(struct dc_link *link, bool enable);
bool dp_set_dsc_enable(struct pipe_ctx *pipe_ctx, bool enable);
bool dp_set_dsc_pps_sdp(struct pipe_ctx *pipe_ctx, bool enable);
void dp_set_dsc_on_stream(struct pipe_ctx *pipe_ctx, bool enable);
bool dp_update_dsc_config(struct pipe_ctx *pipe_ctx);
bool dp_set_dsc_on_rx(struct pipe_ctx *pipe_ctx, bool enable);

/* Initialize output parameter lt_settings. */
void dp_decide_training_settings(
	struct dc_link *link,
	const struct dc_link_settings *link_setting,
	struct link_training_settings *lt_settings);

/* Convert PHY repeater count read from DPCD uint8_t. */
uint8_t dp_convert_to_count(uint8_t lttpr_repeater_count);

/* Check DPCD training status registers to detect link loss. */
enum link_training_result dp_check_link_loss_status(
		struct dc_link *link,
		const struct link_training_settings *link_training_setting);

enum dc_status dpcd_configure_lttpr_mode(
		struct dc_link *link,
		struct link_training_settings *lt_settings);

enum dp_link_encoding dp_get_link_encoding_format(const struct dc_link_settings *link_settings);
bool dp_retrieve_lttpr_cap(struct dc_link *link);
>>>>>>> upstream/android-13
#endif /* __DC_LINK_DP_H__ */
