/*
 * Copyright 2017 Advanced Micro Devices, Inc.
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
 */
/*
 * link_encoder.h
 *
 *  Created on: Oct 6, 2015
 *      Author: yonsun
 */

#ifndef LINK_ENCODER_H_
#define LINK_ENCODER_H_

#include "grph_object_defs.h"
#include "signal_types.h"
#include "dc_types.h"

struct dc_context;
struct encoder_set_dp_phy_pattern_param;
struct link_mst_stream_allocation_table;
struct dc_link_settings;
struct link_training_settings;
struct pipe_ctx;

struct encoder_init_data {
	enum channel_id channel;
	struct graphics_object_id connector;
	enum hpd_source_id hpd_source;
	/* TODO: in DAL2, here was pointer to EventManagerInterface */
	struct graphics_object_id encoder;
	struct dc_context *ctx;
	enum transmitter transmitter;
};

struct encoder_feature_support {
	union {
		struct {
			uint32_t IS_HBR2_CAPABLE:1;
			uint32_t IS_HBR3_CAPABLE:1;
			uint32_t IS_TPS3_CAPABLE:1;
			uint32_t IS_TPS4_CAPABLE:1;
<<<<<<< HEAD
			uint32_t IS_YCBCR_CAPABLE:1;
			uint32_t HDMI_6GB_EN:1;
=======
			uint32_t HDMI_6GB_EN:1;
			uint32_t DP_IS_USB_C:1;
>>>>>>> upstream/android-13
		} bits;
		uint32_t raw;
	} flags;

	enum dc_color_depth max_hdmi_deep_color;
	unsigned int max_hdmi_pixel_clock;
<<<<<<< HEAD
	bool ycbcr420_supported;
=======
	bool hdmi_ycbcr420_supported;
	bool dp_ycbcr420_supported;
	bool fec_supported;
>>>>>>> upstream/android-13
};

union dpcd_psr_configuration {
	struct {
		unsigned char ENABLE                    : 1;
		unsigned char TRANSMITTER_ACTIVE_IN_PSR : 1;
		unsigned char CRC_VERIFICATION          : 1;
		unsigned char FRAME_CAPTURE_INDICATION  : 1;
		/* For eDP 1.4, PSR v2*/
		unsigned char LINE_CAPTURE_INDICATION   : 1;
		/* For eDP 1.4, PSR v2*/
		unsigned char IRQ_HPD_WITH_CRC_ERROR    : 1;
		unsigned char RESERVED                  : 2;
	} bits;
	unsigned char raw;
};

union psr_error_status {
	struct {
		unsigned char LINK_CRC_ERROR        :1;
		unsigned char RFB_STORAGE_ERROR     :1;
<<<<<<< HEAD
		unsigned char RESERVED              :6;
=======
		unsigned char VSC_SDP_ERROR         :1;
		unsigned char RESERVED              :5;
>>>>>>> upstream/android-13
	} bits;
	unsigned char raw;
};

union psr_sink_psr_status {
	struct {
	unsigned char SINK_SELF_REFRESH_STATUS  :3;
	unsigned char RESERVED                  :5;
	} bits;
	unsigned char raw;
};

struct link_encoder {
	const struct link_encoder_funcs *funcs;
	int32_t aux_channel_offset;
	struct dc_context *ctx;
	struct graphics_object_id id;
	struct graphics_object_id connector;
	uint32_t output_signals;
	enum engine_id preferred_engine;
	struct encoder_feature_support features;
	enum transmitter transmitter;
	enum hpd_source_id hpd_source;
<<<<<<< HEAD
};

struct link_encoder_funcs {
=======
	bool usbc_combo_phy;
};

struct link_enc_state {

		uint32_t dphy_fec_en;
		uint32_t dphy_fec_ready_shadow;
		uint32_t dphy_fec_active_status;
		uint32_t dp_link_training_complete;

};

enum encoder_type_select {
	ENCODER_TYPE_DIG = 0,
	ENCODER_TYPE_HDMI_FRL = 1,
	ENCODER_TYPE_DP_128B132B = 2
};

struct link_encoder_funcs {
	void (*read_state)(
			struct link_encoder *enc, struct link_enc_state *s);
>>>>>>> upstream/android-13
	bool (*validate_output_with_stream)(
		struct link_encoder *enc, const struct dc_stream_state *stream);
	void (*hw_init)(struct link_encoder *enc);
	void (*setup)(struct link_encoder *enc,
		enum signal_type signal);
	void (*enable_tmds_output)(struct link_encoder *enc,
		enum clock_source_id clock_source,
		enum dc_color_depth color_depth,
		enum signal_type signal,
		uint32_t pixel_clock);
	void (*enable_dp_output)(struct link_encoder *enc,
		const struct dc_link_settings *link_settings,
		enum clock_source_id clock_source);
	void (*enable_dp_mst_output)(struct link_encoder *enc,
		const struct dc_link_settings *link_settings,
		enum clock_source_id clock_source);
<<<<<<< HEAD
=======
	void (*enable_lvds_output)(struct link_encoder *enc,
		enum clock_source_id clock_source,
		uint32_t pixel_clock);
>>>>>>> upstream/android-13
	void (*disable_output)(struct link_encoder *link_enc,
		enum signal_type signal);
	void (*dp_set_lane_settings)(struct link_encoder *enc,
		const struct link_training_settings *link_settings);
	void (*dp_set_phy_pattern)(struct link_encoder *enc,
		const struct encoder_set_dp_phy_pattern_param *para);
	void (*update_mst_stream_allocation_table)(
		struct link_encoder *enc,
		const struct link_mst_stream_allocation_table *table);
	void (*psr_program_dp_dphy_fast_training)(struct link_encoder *enc,
			bool exit_link_training_required);
	void (*psr_program_secondary_packet)(struct link_encoder *enc,
				unsigned int sdp_transmit_line_num_deadline);
	void (*connect_dig_be_to_fe)(struct link_encoder *enc,
		enum engine_id engine,
		bool connect);
	void (*enable_hpd)(struct link_encoder *enc);
	void (*disable_hpd)(struct link_encoder *enc);
	bool (*is_dig_enabled)(struct link_encoder *enc);
<<<<<<< HEAD
	void (*destroy)(struct link_encoder **enc);
=======
	unsigned int (*get_dig_frontend)(struct link_encoder *enc);
	void (*destroy)(struct link_encoder **enc);

	void (*fec_set_enable)(struct link_encoder *enc,
		bool enable);

	void (*fec_set_ready)(struct link_encoder *enc,
		bool ready);

	bool (*fec_is_active)(struct link_encoder *enc);
	bool (*is_in_alt_mode) (struct link_encoder *enc);

	void (*get_max_link_cap)(struct link_encoder *enc,
		struct dc_link_settings *link_settings);

	enum signal_type (*get_dig_mode)(
		struct link_encoder *enc);
	void (*set_dio_phy_mux)(
		struct link_encoder *enc,
		enum encoder_type_select sel,
		uint32_t hpo_inst);
};

/*
 * Used to track assignments of links (display endpoints) to link encoders.
 *
 * Entry in link_enc_assignments table in struct resource_context.
 * Entries only marked valid once encoder assigned to a link and invalidated once unassigned.
 * Uses engine ID as identifier since PHY ID not relevant for USB4 DPIA endpoint.
 */
struct link_enc_assignment {
	bool valid;
	struct display_endpoint_id ep_id;
	enum engine_id eng_id;
>>>>>>> upstream/android-13
};

#endif /* LINK_ENCODER_H_ */
