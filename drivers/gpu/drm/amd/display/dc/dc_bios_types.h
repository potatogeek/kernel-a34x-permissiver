/*
 * Copyright 2016 Advanced Micro Devices, Inc.
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

#ifndef DC_BIOS_TYPES_H
#define DC_BIOS_TYPES_H

/******************************************************************************
 * Interface file for VBIOS implementations.
 *
 * The default implementation is inside DC.
 * Display Manager (which instantiates DC) has the option to supply it's own
 * (external to DC) implementation of VBIOS, which will be called by DC, using
 * this interface.
 * (The intended use is Diagnostics, but other uses may appear.)
 *****************************************************************************/

#include "include/bios_parser_types.h"

struct dc_vbios_funcs {
	uint8_t (*get_connectors_number)(struct dc_bios *bios);

<<<<<<< HEAD
	struct graphics_object_id (*get_encoder_id)(
		struct dc_bios *bios,
		uint32_t i);
	struct graphics_object_id (*get_connector_id)(
		struct dc_bios *bios,
		uint8_t connector_index);
	uint32_t (*get_dst_number)(
		struct dc_bios *bios,
		struct graphics_object_id id);

=======
	struct graphics_object_id (*get_connector_id)(
		struct dc_bios *bios,
		uint8_t connector_index);
>>>>>>> upstream/android-13
	enum bp_result (*get_src_obj)(
		struct dc_bios *bios,
		struct graphics_object_id object_id, uint32_t index,
		struct graphics_object_id *src_object_id);
<<<<<<< HEAD
	enum bp_result (*get_dst_obj)(
		struct dc_bios *bios,
		struct graphics_object_id object_id, uint32_t index,
		struct graphics_object_id *dest_object_id);

=======
>>>>>>> upstream/android-13
	enum bp_result (*get_i2c_info)(
		struct dc_bios *dcb,
		struct graphics_object_id id,
		struct graphics_object_i2c_info *info);
<<<<<<< HEAD

	enum bp_result (*get_voltage_ddc_info)(
		struct dc_bios *bios,
		uint32_t index,
		struct graphics_object_i2c_info *info);
	enum bp_result (*get_thermal_ddc_info)(
		struct dc_bios *bios,
		uint32_t i2c_channel_id,
		struct graphics_object_i2c_info *info);
=======
>>>>>>> upstream/android-13
	enum bp_result (*get_hpd_info)(
		struct dc_bios *bios,
		struct graphics_object_id id,
		struct graphics_object_hpd_info *info);
	enum bp_result (*get_device_tag)(
		struct dc_bios *bios,
		struct graphics_object_id connector_object_id,
		uint32_t device_tag_index,
		struct connector_device_tag_info *info);
<<<<<<< HEAD
	enum bp_result (*get_firmware_info)(
		struct dc_bios *bios,
		struct dc_firmware_info *info);
=======
>>>>>>> upstream/android-13
	enum bp_result (*get_spread_spectrum_info)(
		struct dc_bios *bios,
		enum as_signal_type signal,
		uint32_t index,
		struct spread_spectrum_info *ss_info);
	uint32_t (*get_ss_entry_number)(
		struct dc_bios *bios,
		enum as_signal_type signal);
	enum bp_result (*get_embedded_panel_info)(
		struct dc_bios *bios,
		struct embedded_panel_info *info);
	enum bp_result (*get_gpio_pin_info)(
		struct dc_bios *bios,
		uint32_t gpio_id,
		struct gpio_pin_info *info);
	enum bp_result (*get_encoder_cap_info)(
		struct dc_bios *bios,
		struct graphics_object_id object_id,
		struct bp_encoder_cap_info *info);

<<<<<<< HEAD
	bool (*is_lid_status_changed)(
		struct dc_bios *bios);
	bool (*is_display_config_changed)(
		struct dc_bios *bios);
	bool (*is_accelerated_mode)(
		struct dc_bios *bios);
	uint32_t (*get_vga_enabled_displays)(
		struct dc_bios *bios);
	void (*get_bios_event_info)(
		struct dc_bios *bios,
		struct bios_event_info *info);
	void (*update_requested_backlight_level)(
		struct dc_bios *bios,
		uint32_t backlight_8bit);
	uint32_t (*get_requested_backlight_level)(
		struct dc_bios *bios);
	void (*take_backlight_control)(
		struct dc_bios *bios,
		bool cntl);

	bool (*is_active_display)(
		struct dc_bios *bios,
		enum signal_type signal,
		const struct connector_device_tag_info *device_tag);
	enum controller_id (*get_embedded_display_controller_id)(
		struct dc_bios *bios);
	uint32_t (*get_embedded_display_refresh_rate)(
		struct dc_bios *bios);

=======
	bool (*is_accelerated_mode)(
		struct dc_bios *bios);
>>>>>>> upstream/android-13
	void (*set_scratch_critical_state)(
		struct dc_bios *bios,
		bool state);
	bool (*is_device_id_supported)(
		struct dc_bios *bios,
		struct device_id id);
<<<<<<< HEAD

=======
>>>>>>> upstream/android-13
	/* COMMANDS */

	enum bp_result (*encoder_control)(
		struct dc_bios *bios,
		struct bp_encoder_control *cntl);
	enum bp_result (*transmitter_control)(
		struct dc_bios *bios,
		struct bp_transmitter_control *cntl);
<<<<<<< HEAD
	enum bp_result (*crt_control)(
		struct dc_bios *bios,
		enum engine_id engine_id,
		bool enable,
		uint32_t pixel_clock);
=======
>>>>>>> upstream/android-13
	enum bp_result (*enable_crtc)(
		struct dc_bios *bios,
		enum controller_id id,
		bool enable);
	enum bp_result (*adjust_pixel_clock)(
		struct dc_bios *bios,
		struct bp_adjust_pixel_clock_parameters *bp_params);
	enum bp_result (*set_pixel_clock)(
		struct dc_bios *bios,
		struct bp_pixel_clock_parameters *bp_params);
	enum bp_result (*set_dce_clock)(
		struct dc_bios *bios,
		struct bp_set_dce_clock_parameters *bp_params);
<<<<<<< HEAD
	unsigned int (*get_smu_clock_info)(
		struct dc_bios *bios);
=======
>>>>>>> upstream/android-13
	enum bp_result (*enable_spread_spectrum_on_ppll)(
		struct dc_bios *bios,
		struct bp_spread_spectrum_parameters *bp_params,
		bool enable);
	enum bp_result (*program_crtc_timing)(
		struct dc_bios *bios,
		struct bp_hw_crtc_timing_parameters *bp_params);
<<<<<<< HEAD

	enum bp_result (*crtc_source_select)(
		struct dc_bios *bios,
		struct bp_crtc_source_select *bp_params);
	enum bp_result (*program_display_engine_pll)(
		struct dc_bios *bios,
		struct bp_pixel_clock_parameters *bp_params);

	enum signal_type (*dac_load_detect)(
		struct dc_bios *bios,
		struct graphics_object_id encoder,
		struct graphics_object_id connector,
		enum signal_type display_signal);

=======
	enum bp_result (*program_display_engine_pll)(
		struct dc_bios *bios,
		struct bp_pixel_clock_parameters *bp_params);
>>>>>>> upstream/android-13
	enum bp_result (*enable_disp_power_gating)(
		struct dc_bios *bios,
		enum controller_id controller_id,
		enum bp_pipe_control_action action);

<<<<<<< HEAD
	void (*post_init)(struct dc_bios *bios);

=======
>>>>>>> upstream/android-13
	void (*bios_parser_destroy)(struct dc_bios **dcb);

	enum bp_result (*get_board_layout_info)(
		struct dc_bios *dcb,
		struct board_layout_info *board_layout_info);
<<<<<<< HEAD
=======
	uint16_t (*pack_data_tables)(
		struct dc_bios *dcb,
		void *dst);

	enum bp_result (*get_atom_dc_golden_table)(
			struct dc_bios *dcb);

	enum bp_result (*enable_lvtma_control)(
		struct dc_bios *bios,
		uint8_t uc_pwr_on,
		uint8_t panel_instance);

	enum bp_result (*get_soc_bb_info)(
		struct dc_bios *dcb,
		struct bp_soc_bb_info *soc_bb_info);

	enum bp_result (*get_disp_connector_caps_info)(
			struct dc_bios *dcb,
			struct graphics_object_id object_id,
			struct bp_disp_connector_caps_info *info);
	enum bp_result (*get_lttpr_caps)(
			struct dc_bios *dcb,
			uint8_t *dce_caps);
	enum bp_result (*get_lttpr_interop)(
			struct dc_bios *dcb,
			uint8_t *dce_caps);
>>>>>>> upstream/android-13
};

struct bios_registers {
	uint32_t BIOS_SCRATCH_3;
	uint32_t BIOS_SCRATCH_6;
};

struct dc_bios {
	const struct dc_vbios_funcs *funcs;

	uint8_t *bios;
	uint32_t bios_size;

	uint8_t *bios_local_image;

	struct dc_context *ctx;
	const struct bios_registers *regs;
	struct integrated_info *integrated_info;
<<<<<<< HEAD
=======
	struct dc_firmware_info fw_info;
	bool fw_info_valid;
	struct dc_vram_info vram_info;
	struct dc_golden_table golden_table;
>>>>>>> upstream/android-13
};

#endif /* DC_BIOS_TYPES_H */
