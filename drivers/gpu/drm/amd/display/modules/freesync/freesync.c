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

<<<<<<< HEAD
=======
#include <linux/slab.h>

>>>>>>> upstream/android-13
#include "dm_services.h"
#include "dc.h"
#include "mod_freesync.h"
#include "core_types.h"

#define MOD_FREESYNC_MAX_CONCURRENT_STREAMS  32

<<<<<<< HEAD
=======
#define MIN_REFRESH_RANGE 10
>>>>>>> upstream/android-13
/* Refresh rate ramp at a fixed rate of 65 Hz/second */
#define STATIC_SCREEN_RAMP_DELTA_REFRESH_RATE_PER_FRAME ((1000 / 60) * 65)
/* Number of elements in the render times cache array */
#define RENDER_TIMES_MAX_COUNT 10
<<<<<<< HEAD
/* Threshold to exit BTR (to avoid frequent enter-exits at the lower limit) */
#define BTR_EXIT_MARGIN 2000
/* Number of consecutive frames to check before entering/exiting fixed refresh*/
#define FIXED_REFRESH_ENTER_FRAME_COUNT 5
#define FIXED_REFRESH_EXIT_FRAME_COUNT 5

#define FREESYNC_REGISTRY_NAME "freesync_v1"

#define FREESYNC_NO_STATIC_FOR_EXTERNAL_DP_REGKEY "DalFreeSyncNoStaticForExternalDp"

#define FREESYNC_NO_STATIC_FOR_INTERNAL_REGKEY "DalFreeSyncNoStaticForInternal"

#define FREESYNC_DEFAULT_REGKEY "LCDFreeSyncDefault"

struct gradual_static_ramp {
	bool ramp_is_active;
	bool ramp_direction_is_up;
	unsigned int ramp_current_frame_duration_in_ns;
};

struct freesync_time {
	/* video (48Hz feature) related */
	unsigned int update_duration_in_ns;

	/* BTR/fixed refresh related */
	unsigned int prev_time_stamp_in_us;

	unsigned int min_render_time_in_us;
	unsigned int max_render_time_in_us;

	unsigned int render_times_index;
	unsigned int render_times[RENDER_TIMES_MAX_COUNT];

	unsigned int min_window;
	unsigned int max_window;
};

struct below_the_range {
	bool btr_active;
	bool program_btr;

	unsigned int mid_point_in_us;

	unsigned int inserted_frame_duration_in_us;
	unsigned int frames_to_insert;
	unsigned int frame_counter;
};

struct fixed_refresh {
	bool fixed_active;
	bool program_fixed;
	unsigned int frame_counter;
};

struct freesync_range {
	unsigned int min_refresh;
	unsigned int max_frame_duration;
	unsigned int vmax;

	unsigned int max_refresh;
	unsigned int min_frame_duration;
	unsigned int vmin;
};

struct freesync_state {
	bool fullscreen;
	bool static_screen;
	bool video;

	unsigned int vmin;
	unsigned int vmax;

	struct freesync_time time;

	unsigned int nominal_refresh_rate_in_micro_hz;
	bool windowed_fullscreen;

	struct gradual_static_ramp static_ramp;
	struct below_the_range btr;
	struct fixed_refresh fixed_refresh;
	struct freesync_range freesync_range;
};

struct freesync_entity {
	struct dc_stream_state *stream;
	struct mod_freesync_caps *caps;
	struct freesync_state state;
	struct mod_freesync_user_enable user_enable;
};

struct freesync_registry_options {
	bool drr_external_supported;
	bool drr_internal_supported;
	bool lcd_freesync_default_set;
	int lcd_freesync_default_value;
};
=======
/* Threshold to exit/exit BTR (to avoid frequent enter-exits at the lower limit) */
#define BTR_MAX_MARGIN 2500
/* Threshold to change BTR multiplier (to avoid frequent changes) */
#define BTR_DRIFT_MARGIN 2000
/* Threshold to exit fixed refresh rate */
#define FIXED_REFRESH_EXIT_MARGIN_IN_HZ 1
/* Number of consecutive frames to check before entering/exiting fixed refresh */
#define FIXED_REFRESH_ENTER_FRAME_COUNT 5
#define FIXED_REFRESH_EXIT_FRAME_COUNT 10
>>>>>>> upstream/android-13

struct core_freesync {
	struct mod_freesync public;
	struct dc *dc;
<<<<<<< HEAD
	struct freesync_registry_options opts;
	struct freesync_entity *map;
	int num_entities;
=======
>>>>>>> upstream/android-13
};

#define MOD_FREESYNC_TO_CORE(mod_freesync)\
		container_of(mod_freesync, struct core_freesync, public)

struct mod_freesync *mod_freesync_create(struct dc *dc)
{
	struct core_freesync *core_freesync =
			kzalloc(sizeof(struct core_freesync), GFP_KERNEL);

<<<<<<< HEAD

	struct persistent_data_flag flag;

	int i, data = 0;

	if (core_freesync == NULL)
		goto fail_alloc_context;

	core_freesync->map = kcalloc(MOD_FREESYNC_MAX_CONCURRENT_STREAMS,
					sizeof(struct freesync_entity),
					GFP_KERNEL);

	if (core_freesync->map == NULL)
		goto fail_alloc_map;

	for (i = 0; i < MOD_FREESYNC_MAX_CONCURRENT_STREAMS; i++)
		core_freesync->map[i].stream = NULL;

	core_freesync->num_entities = 0;

=======
	if (core_freesync == NULL)
		goto fail_alloc_context;

>>>>>>> upstream/android-13
	if (dc == NULL)
		goto fail_construct;

	core_freesync->dc = dc;
<<<<<<< HEAD

	/* Create initial module folder in registry for freesync enable data */
	flag.save_per_edid = true;
	flag.save_per_link = false;
	dm_write_persistent_data(dc->ctx, NULL, FREESYNC_REGISTRY_NAME,
			NULL, NULL, 0, &flag);
	flag.save_per_edid = false;
	flag.save_per_link = false;

	if (dm_read_persistent_data(dc->ctx, NULL, NULL,
			FREESYNC_NO_STATIC_FOR_INTERNAL_REGKEY,
			&data, sizeof(data), &flag)) {
		core_freesync->opts.drr_internal_supported =
			(data & 1) ? false : true;
	}

	if (dm_read_persistent_data(dc->ctx, NULL, NULL,
			FREESYNC_NO_STATIC_FOR_EXTERNAL_DP_REGKEY,
			&data, sizeof(data), &flag)) {
		core_freesync->opts.drr_external_supported =
				(data & 1) ? false : true;
	}

	if (dm_read_persistent_data(dc->ctx, NULL, NULL,
			FREESYNC_DEFAULT_REGKEY,
			&data, sizeof(data), &flag)) {
		core_freesync->opts.lcd_freesync_default_set = true;
		core_freesync->opts.lcd_freesync_default_value = data;
	} else {
		core_freesync->opts.lcd_freesync_default_set = false;
		core_freesync->opts.lcd_freesync_default_value = 0;
	}

	return &core_freesync->public;

fail_construct:
	kfree(core_freesync->map);

fail_alloc_map:
=======
	return &core_freesync->public;

fail_construct:
>>>>>>> upstream/android-13
	kfree(core_freesync);

fail_alloc_context:
	return NULL;
}

void mod_freesync_destroy(struct mod_freesync *mod_freesync)
{
<<<<<<< HEAD
	if (mod_freesync != NULL) {
		int i;
		struct core_freesync *core_freesync =
				MOD_FREESYNC_TO_CORE(mod_freesync);

		for (i = 0; i < core_freesync->num_entities; i++)
			if (core_freesync->map[i].stream)
				dc_stream_release(core_freesync->map[i].stream);

		kfree(core_freesync->map);

		kfree(core_freesync);
	}
}

/* Given a specific dc_stream* this function finds its equivalent
 * on the core_freesync->map and returns the corresponding index
 */
static unsigned int map_index_from_stream(struct core_freesync *core_freesync,
		struct dc_stream_state *stream)
{
	unsigned int index = 0;

	for (index = 0; index < core_freesync->num_entities; index++) {
		if (core_freesync->map[index].stream == stream) {
			return index;
		}
	}
	/* Could not find stream requested */
	ASSERT(false);
	return index;
}

bool mod_freesync_add_stream(struct mod_freesync *mod_freesync,
		struct dc_stream_state *stream, struct mod_freesync_caps *caps)
{
	struct dc  *dc = NULL;
	struct core_freesync *core_freesync = NULL;
	int persistent_freesync_enable = 0;
	struct persistent_data_flag flag;
	unsigned int nom_refresh_rate_uhz;
	unsigned long long temp;

	if (mod_freesync == NULL)
		return false;

	core_freesync = MOD_FREESYNC_TO_CORE(mod_freesync);
	dc = core_freesync->dc;

	flag.save_per_edid = true;
	flag.save_per_link = false;

	if (core_freesync->num_entities < MOD_FREESYNC_MAX_CONCURRENT_STREAMS) {

		dc_stream_retain(stream);

		temp = stream->timing.pix_clk_khz;
		temp *= 1000ULL * 1000ULL * 1000ULL;
		temp = div_u64(temp, stream->timing.h_total);
		temp = div_u64(temp, stream->timing.v_total);

		nom_refresh_rate_uhz = (unsigned int) temp;

		core_freesync->map[core_freesync->num_entities].stream = stream;
		core_freesync->map[core_freesync->num_entities].caps = caps;

		core_freesync->map[core_freesync->num_entities].state.
			fullscreen = false;
		core_freesync->map[core_freesync->num_entities].state.
			static_screen = false;
		core_freesync->map[core_freesync->num_entities].state.
			video = false;
		core_freesync->map[core_freesync->num_entities].state.time.
			update_duration_in_ns = 0;
		core_freesync->map[core_freesync->num_entities].state.
			static_ramp.ramp_is_active = false;

		/* get persistent data from registry */
		if (dm_read_persistent_data(dc->ctx, stream->sink,
					FREESYNC_REGISTRY_NAME,
					"userenable", &persistent_freesync_enable,
					sizeof(int), &flag)) {
			core_freesync->map[core_freesync->num_entities].user_enable.
				enable_for_gaming =
				(persistent_freesync_enable & 1) ? true : false;
			core_freesync->map[core_freesync->num_entities].user_enable.
				enable_for_static =
				(persistent_freesync_enable & 2) ? true : false;
			core_freesync->map[core_freesync->num_entities].user_enable.
				enable_for_video =
				(persistent_freesync_enable & 4) ? true : false;
		/* If FreeSync display and LCDFreeSyncDefault is set, use as default values write back to userenable */
		} else if (caps->supported && (core_freesync->opts.lcd_freesync_default_set)) {
			core_freesync->map[core_freesync->num_entities].user_enable.enable_for_gaming =
				(core_freesync->opts.lcd_freesync_default_value & 1) ? true : false;
			core_freesync->map[core_freesync->num_entities].user_enable.enable_for_static =
				(core_freesync->opts.lcd_freesync_default_value & 2) ? true : false;
			core_freesync->map[core_freesync->num_entities].user_enable.enable_for_video =
				(core_freesync->opts.lcd_freesync_default_value & 4) ? true : false;
			dm_write_persistent_data(dc->ctx, stream->sink,
						FREESYNC_REGISTRY_NAME,
						"userenable", &core_freesync->opts.lcd_freesync_default_value,
						sizeof(int), &flag);
		} else {
			core_freesync->map[core_freesync->num_entities].user_enable.
					enable_for_gaming = false;
			core_freesync->map[core_freesync->num_entities].user_enable.
					enable_for_static = false;
			core_freesync->map[core_freesync->num_entities].user_enable.
					enable_for_video = false;
		}

		if (caps->supported &&
			nom_refresh_rate_uhz >= caps->min_refresh_in_micro_hz &&
			nom_refresh_rate_uhz <= caps->max_refresh_in_micro_hz)
			stream->ignore_msa_timing_param = 1;

		core_freesync->num_entities++;
		return true;
	}
	return false;
}

bool mod_freesync_remove_stream(struct mod_freesync *mod_freesync,
		struct dc_stream_state *stream)
{
	int i = 0;
	struct core_freesync *core_freesync = NULL;
	unsigned int index = 0;

	if (mod_freesync == NULL)
		return false;

	core_freesync = MOD_FREESYNC_TO_CORE(mod_freesync);
	index = map_index_from_stream(core_freesync, stream);

	dc_stream_release(core_freesync->map[index].stream);
	core_freesync->map[index].stream = NULL;
	/* To remove this entity, shift everything after down */
	for (i = index; i < core_freesync->num_entities - 1; i++)
		core_freesync->map[i] = core_freesync->map[i + 1];
	core_freesync->num_entities--;
	return true;
}

static void adjust_vmin_vmax(struct core_freesync *core_freesync,
				struct dc_stream_state **streams,
				int num_streams,
				int map_index,
				unsigned int v_total_min,
				unsigned int v_total_max)
{
	if (num_streams == 0 || streams == NULL || num_streams > 1)
		return;

	core_freesync->map[map_index].state.vmin = v_total_min;
	core_freesync->map[map_index].state.vmax = v_total_max;

	dc_stream_adjust_vmin_vmax(core_freesync->dc, streams,
				num_streams, v_total_min,
				v_total_max);
}


static void update_stream_freesync_context(struct core_freesync *core_freesync,
		struct dc_stream_state *stream)
{
	unsigned int index;
	struct freesync_context *ctx;

	ctx = &stream->freesync_ctx;

	index = map_index_from_stream(core_freesync, stream);

	ctx->supported = core_freesync->map[index].caps->supported;
	ctx->enabled = (core_freesync->map[index].user_enable.enable_for_gaming ||
		core_freesync->map[index].user_enable.enable_for_video ||
		core_freesync->map[index].user_enable.enable_for_static);
	ctx->active = (core_freesync->map[index].state.fullscreen ||
		core_freesync->map[index].state.video ||
		core_freesync->map[index].state.static_ramp.ramp_is_active);
	ctx->min_refresh_in_micro_hz =
			core_freesync->map[index].caps->min_refresh_in_micro_hz;
	ctx->nominal_refresh_in_micro_hz = core_freesync->
		map[index].state.nominal_refresh_rate_in_micro_hz;

}

static void update_stream(struct core_freesync *core_freesync,
		struct dc_stream_state *stream)
{
	unsigned int index = map_index_from_stream(core_freesync, stream);
	if (core_freesync->map[index].caps->supported) {
		stream->ignore_msa_timing_param = 1;
		update_stream_freesync_context(core_freesync, stream);
	}
}

static void calc_freesync_range(struct core_freesync *core_freesync,
		struct dc_stream_state *stream,
		struct freesync_state *state,
		unsigned int min_refresh_in_uhz,
		unsigned int max_refresh_in_uhz)
{
	unsigned int min_frame_duration_in_ns = 0, max_frame_duration_in_ns = 0;
	unsigned int index = map_index_from_stream(core_freesync, stream);
	uint32_t vtotal = stream->timing.v_total;

	if ((min_refresh_in_uhz == 0) || (max_refresh_in_uhz == 0)) {
		state->freesync_range.min_refresh =
				state->nominal_refresh_rate_in_micro_hz;
		state->freesync_range.max_refresh =
				state->nominal_refresh_rate_in_micro_hz;

		state->freesync_range.max_frame_duration = 0;
		state->freesync_range.min_frame_duration = 0;

		state->freesync_range.vmax = vtotal;
		state->freesync_range.vmin = vtotal;

		return;
	}

	min_frame_duration_in_ns = ((unsigned int) (div64_u64(
					(1000000000ULL * 1000000),
					max_refresh_in_uhz)));
	max_frame_duration_in_ns = ((unsigned int) (div64_u64(
		(1000000000ULL * 1000000),
		min_refresh_in_uhz)));

	state->freesync_range.min_refresh = min_refresh_in_uhz;
	state->freesync_range.max_refresh = max_refresh_in_uhz;

	state->freesync_range.max_frame_duration = max_frame_duration_in_ns;
	state->freesync_range.min_frame_duration = min_frame_duration_in_ns;

	state->freesync_range.vmax = div64_u64(div64_u64(((unsigned long long)(
		max_frame_duration_in_ns) * stream->timing.pix_clk_khz),
		stream->timing.h_total), 1000000);
	state->freesync_range.vmin = div64_u64(div64_u64(((unsigned long long)(
		min_frame_duration_in_ns) * stream->timing.pix_clk_khz),
		stream->timing.h_total), 1000000);

	/* vmin/vmax cannot be less than vtotal */
	if (state->freesync_range.vmin < vtotal) {
		/* Error of 1 is permissible */
		ASSERT((state->freesync_range.vmin + 1) >= vtotal);
		state->freesync_range.vmin = vtotal;
	}

	if (state->freesync_range.vmax < vtotal) {
		/* Error of 1 is permissible */
		ASSERT((state->freesync_range.vmax + 1) >= vtotal);
		state->freesync_range.vmax = vtotal;
	}

	/* Determine whether BTR can be supported */
	if (max_frame_duration_in_ns >=
			2 * min_frame_duration_in_ns)
		core_freesync->map[index].caps->btr_supported = true;
	else
		core_freesync->map[index].caps->btr_supported = false;

	/* Cache the time variables */
	state->time.max_render_time_in_us =
		max_frame_duration_in_ns / 1000;
	state->time.min_render_time_in_us =
		min_frame_duration_in_ns / 1000;
	state->btr.mid_point_in_us =
		(max_frame_duration_in_ns +
		min_frame_duration_in_ns) / 2000;
}

static void calc_v_total_from_duration(struct dc_stream_state *stream,
		unsigned int duration_in_ns, int *v_total_nominal)
{
	*v_total_nominal = div64_u64(div64_u64(((unsigned long long)(
				duration_in_ns) * stream->timing.pix_clk_khz),
				stream->timing.h_total), 1000000);
}

static void calc_v_total_for_static_ramp(struct core_freesync *core_freesync,
		struct dc_stream_state *stream,
		unsigned int index, int *v_total)
{
	unsigned int frame_duration = 0;

	struct gradual_static_ramp *static_ramp_variables =
				&core_freesync->map[index].state.static_ramp;

	/* Calc ratio between new and current frame duration with 3 digit */
	unsigned int frame_duration_ratio = div64_u64(1000000,
		(1000 +  div64_u64(((unsigned long long)(
		STATIC_SCREEN_RAMP_DELTA_REFRESH_RATE_PER_FRAME) *
		static_ramp_variables->ramp_current_frame_duration_in_ns),
		1000000000)));

	/* Calculate delta between new and current frame duration in ns */
	unsigned int frame_duration_delta = div64_u64(((unsigned long long)(
		static_ramp_variables->ramp_current_frame_duration_in_ns) *
=======
	struct core_freesync *core_freesync = NULL;
	if (mod_freesync == NULL)
		return;
	core_freesync = MOD_FREESYNC_TO_CORE(mod_freesync);
	kfree(core_freesync);
}

#if 0 /* Unused currently */
static unsigned int calc_refresh_in_uhz_from_duration(
		unsigned int duration_in_ns)
{
	unsigned int refresh_in_uhz =
			((unsigned int)(div64_u64((1000000000ULL * 1000000),
					duration_in_ns)));
	return refresh_in_uhz;
}
#endif

static unsigned int calc_duration_in_us_from_refresh_in_uhz(
		unsigned int refresh_in_uhz)
{
	unsigned int duration_in_us =
			((unsigned int)(div64_u64((1000000000ULL * 1000),
					refresh_in_uhz)));
	return duration_in_us;
}

static unsigned int calc_duration_in_us_from_v_total(
		const struct dc_stream_state *stream,
		const struct mod_vrr_params *in_vrr,
		unsigned int v_total)
{
	unsigned int duration_in_us =
			(unsigned int)(div64_u64(((unsigned long long)(v_total)
				* 10000) * stream->timing.h_total,
					stream->timing.pix_clk_100hz));

	return duration_in_us;
}

unsigned int mod_freesync_calc_v_total_from_refresh(
		const struct dc_stream_state *stream,
		unsigned int refresh_in_uhz)
{
	unsigned int v_total;
	unsigned int frame_duration_in_ns;

	frame_duration_in_ns =
			((unsigned int)(div64_u64((1000000000ULL * 1000000),
					refresh_in_uhz)));

	v_total = div64_u64(div64_u64(((unsigned long long)(
			frame_duration_in_ns) * (stream->timing.pix_clk_100hz / 10)),
			stream->timing.h_total), 1000000);

	/* v_total cannot be less than nominal */
	if (v_total < stream->timing.v_total) {
		ASSERT(v_total < stream->timing.v_total);
		v_total = stream->timing.v_total;
	}

	return v_total;
}

static unsigned int calc_v_total_from_duration(
		const struct dc_stream_state *stream,
		const struct mod_vrr_params *vrr,
		unsigned int duration_in_us)
{
	unsigned int v_total = 0;

	if (duration_in_us < vrr->min_duration_in_us)
		duration_in_us = vrr->min_duration_in_us;

	if (duration_in_us > vrr->max_duration_in_us)
		duration_in_us = vrr->max_duration_in_us;

	v_total = div64_u64(div64_u64(((unsigned long long)(
				duration_in_us) * (stream->timing.pix_clk_100hz / 10)),
				stream->timing.h_total), 1000);

	/* v_total cannot be less than nominal */
	if (v_total < stream->timing.v_total) {
		ASSERT(v_total < stream->timing.v_total);
		v_total = stream->timing.v_total;
	}

	return v_total;
}

static void update_v_total_for_static_ramp(
		struct core_freesync *core_freesync,
		const struct dc_stream_state *stream,
		struct mod_vrr_params *in_out_vrr)
{
	unsigned int v_total = 0;
	unsigned int current_duration_in_us =
			calc_duration_in_us_from_v_total(
				stream, in_out_vrr,
				in_out_vrr->adjust.v_total_max);
	unsigned int target_duration_in_us =
			calc_duration_in_us_from_refresh_in_uhz(
				in_out_vrr->fixed.target_refresh_in_uhz);
	bool ramp_direction_is_up = (current_duration_in_us >
				target_duration_in_us) ? true : false;

	/* Calculate ratio between new and current frame duration with 3 digit */
	unsigned int frame_duration_ratio = div64_u64(1000000,
		(1000 +  div64_u64(((unsigned long long)(
		STATIC_SCREEN_RAMP_DELTA_REFRESH_RATE_PER_FRAME) *
		current_duration_in_us),
		1000000)));

	/* Calculate delta between new and current frame duration in us */
	unsigned int frame_duration_delta = div64_u64(((unsigned long long)(
		current_duration_in_us) *
>>>>>>> upstream/android-13
		(1000 - frame_duration_ratio)), 1000);

	/* Adjust frame duration delta based on ratio between current and
	 * standard frame duration (frame duration at 60 Hz refresh rate).
	 */
	unsigned int ramp_rate_interpolated = div64_u64(((unsigned long long)(
<<<<<<< HEAD
		frame_duration_delta) * static_ramp_variables->
		ramp_current_frame_duration_in_ns), 16666666);

	/* Going to a higher refresh rate (lower frame duration) */
	if (static_ramp_variables->ramp_direction_is_up) {
		/* reduce frame duration */
		static_ramp_variables->ramp_current_frame_duration_in_ns -=
			ramp_rate_interpolated;

		/* min frame duration */
		frame_duration = ((unsigned int) (div64_u64(
			(1000000000ULL * 1000000),
			core_freesync->map[index].state.
			nominal_refresh_rate_in_micro_hz)));

		/* adjust for frame duration below min */
		if (static_ramp_variables->ramp_current_frame_duration_in_ns <=
			frame_duration) {

			static_ramp_variables->ramp_is_active = false;
			static_ramp_variables->
				ramp_current_frame_duration_in_ns =
				frame_duration;
		}
	/* Going to a lower refresh rate (larger frame duration) */
	} else {
		/* increase frame duration */
		static_ramp_variables->ramp_current_frame_duration_in_ns +=
			ramp_rate_interpolated;

		/* max frame duration */
		frame_duration = ((unsigned int) (div64_u64(
			(1000000000ULL * 1000000),
			core_freesync->map[index].caps->min_refresh_in_micro_hz)));

		/* adjust for frame duration above max */
		if (static_ramp_variables->ramp_current_frame_duration_in_ns >=
			frame_duration) {

			static_ramp_variables->ramp_is_active = false;
			static_ramp_variables->
				ramp_current_frame_duration_in_ns =
				frame_duration;
		}
	}

	calc_v_total_from_duration(stream, static_ramp_variables->
		ramp_current_frame_duration_in_ns, v_total);
}

static void reset_freesync_state_variables(struct freesync_state* state)
{
	state->static_ramp.ramp_is_active = false;
	if (state->nominal_refresh_rate_in_micro_hz)
		state->static_ramp.ramp_current_frame_duration_in_ns =
			((unsigned int) (div64_u64(
			(1000000000ULL * 1000000),
			state->nominal_refresh_rate_in_micro_hz)));

	state->btr.btr_active = false;
	state->btr.frame_counter = 0;
	state->btr.frames_to_insert = 0;
	state->btr.inserted_frame_duration_in_us = 0;
	state->btr.program_btr = false;

	state->fixed_refresh.fixed_active = false;
	state->fixed_refresh.program_fixed = false;
}
/*
 * Sets freesync mode on a stream depending on current freesync state.
 */
static bool set_freesync_on_streams(struct core_freesync *core_freesync,
		struct dc_stream_state **streams, int num_streams)
{
	int v_total_nominal = 0, v_total_min = 0, v_total_max = 0;
	unsigned int stream_idx, map_index = 0;
	struct freesync_state *state;

	if (num_streams == 0 || streams == NULL || num_streams > 1)
		return false;

	for (stream_idx = 0; stream_idx < num_streams; stream_idx++) {

		map_index = map_index_from_stream(core_freesync,
				streams[stream_idx]);

		state = &core_freesync->map[map_index].state;

		if (core_freesync->map[map_index].caps->supported) {

			/* Fullscreen has the topmost priority. If the
			 * fullscreen bit is set, we are in a fullscreen
			 * application where it should not matter if it is
			 * static screen. We should not check the static_screen
			 * or video bit.
			 *
			 * Special cases of fullscreen include btr and fixed
			 * refresh. We program btr on every flip and involves
			 * programming full range right before the last inserted frame.
			 * However, we do not want to program the full freesync range
			 * when fixed refresh is active, because we only program
			 * that logic once and this will override it.
			 */
			if (core_freesync->map[map_index].user_enable.
				enable_for_gaming == true &&
				state->fullscreen == true &&
				state->fixed_refresh.fixed_active == false) {
				/* Enable freesync */

				v_total_min = state->freesync_range.vmin;
				v_total_max = state->freesync_range.vmax;

				/* Update the freesync context for the stream */
				update_stream_freesync_context(core_freesync,
						streams[stream_idx]);

				adjust_vmin_vmax(core_freesync, streams,
						num_streams, map_index,
						v_total_min,
						v_total_max);

				return true;

			} else if (core_freesync->map[map_index].user_enable.
				enable_for_video && state->video == true) {
				/* Enable 48Hz feature */

				calc_v_total_from_duration(streams[stream_idx],
					state->time.update_duration_in_ns,
					&v_total_nominal);

				/* Program only if v_total_nominal is in range*/
				if (v_total_nominal >=
					streams[stream_idx]->timing.v_total) {

					/* Update the freesync context for
					 * the stream
					 */
					update_stream_freesync_context(
						core_freesync,
						streams[stream_idx]);

					adjust_vmin_vmax(
						core_freesync, streams,
						num_streams, map_index,
						v_total_nominal,
						v_total_nominal);
				}
				return true;

			} else {
				/* Disable freesync */
				v_total_nominal = streams[stream_idx]->
					timing.v_total;

				/* Update the freesync context for
				 * the stream
				 */
				update_stream_freesync_context(
					core_freesync,
					streams[stream_idx]);

				adjust_vmin_vmax(core_freesync, streams,
						num_streams, map_index,
						v_total_nominal,
						v_total_nominal);

				/* Reset the cached variables */
				reset_freesync_state_variables(state);

				return true;
			}
		} else {
			/* Disable freesync */
			v_total_nominal = streams[stream_idx]->
				timing.v_total;
			/*
			 * we have to reset drr always even sink does
			 * not support freesync because a former stream has
			 * be programmed
			 */
			adjust_vmin_vmax(core_freesync, streams,
						num_streams, map_index,
						v_total_nominal,
						v_total_nominal);
			/* Reset the cached variables */
			reset_freesync_state_variables(state);
		}

=======
		frame_duration_delta) * current_duration_in_us), 16666);

	/* Going to a higher refresh rate (lower frame duration) */
	if (ramp_direction_is_up) {
		/* Reduce frame duration */
		current_duration_in_us -= ramp_rate_interpolated;

		/* Adjust for frame duration below min */
		if (current_duration_in_us <= target_duration_in_us) {
			in_out_vrr->fixed.ramping_active = false;
			in_out_vrr->fixed.ramping_done = true;
			current_duration_in_us =
				calc_duration_in_us_from_refresh_in_uhz(
				in_out_vrr->fixed.target_refresh_in_uhz);
		}
	/* Going to a lower refresh rate (larger frame duration) */
	} else {
		/* Increase frame duration */
		current_duration_in_us += ramp_rate_interpolated;

		/* Adjust for frame duration above max */
		if (current_duration_in_us >= target_duration_in_us) {
			in_out_vrr->fixed.ramping_active = false;
			in_out_vrr->fixed.ramping_done = true;
			current_duration_in_us =
				calc_duration_in_us_from_refresh_in_uhz(
				in_out_vrr->fixed.target_refresh_in_uhz);
		}
	}

	v_total = div64_u64(div64_u64(((unsigned long long)(
			current_duration_in_us) * (stream->timing.pix_clk_100hz / 10)),
				stream->timing.h_total), 1000);

	/* v_total cannot be less than nominal */
	if (v_total < stream->timing.v_total)
		v_total = stream->timing.v_total;

	in_out_vrr->adjust.v_total_min = v_total;
	in_out_vrr->adjust.v_total_max = v_total;
}

static void apply_below_the_range(struct core_freesync *core_freesync,
		const struct dc_stream_state *stream,
		unsigned int last_render_time_in_us,
		struct mod_vrr_params *in_out_vrr)
{
	unsigned int inserted_frame_duration_in_us = 0;
	unsigned int mid_point_frames_ceil = 0;
	unsigned int mid_point_frames_floor = 0;
	unsigned int frame_time_in_us = 0;
	unsigned int delta_from_mid_point_in_us_1 = 0xFFFFFFFF;
	unsigned int delta_from_mid_point_in_us_2 = 0xFFFFFFFF;
	unsigned int frames_to_insert = 0;
	unsigned int delta_from_mid_point_delta_in_us;
	unsigned int max_render_time_in_us =
			in_out_vrr->max_duration_in_us - in_out_vrr->btr.margin_in_us;

	/* Program BTR */
	if ((last_render_time_in_us + in_out_vrr->btr.margin_in_us / 2) < max_render_time_in_us) {
		/* Exit Below the Range */
		if (in_out_vrr->btr.btr_active) {
			in_out_vrr->btr.frame_counter = 0;
			in_out_vrr->btr.btr_active = false;
		}
	} else if (last_render_time_in_us > (max_render_time_in_us + in_out_vrr->btr.margin_in_us / 2)) {
		/* Enter Below the Range */
		if (!in_out_vrr->btr.btr_active) {
			in_out_vrr->btr.btr_active = true;
		}
	}

	/* BTR set to "not active" so disengage */
	if (!in_out_vrr->btr.btr_active) {
		in_out_vrr->btr.inserted_duration_in_us = 0;
		in_out_vrr->btr.frames_to_insert = 0;
		in_out_vrr->btr.frame_counter = 0;

		/* Restore FreeSync */
		in_out_vrr->adjust.v_total_min =
			mod_freesync_calc_v_total_from_refresh(stream,
				in_out_vrr->max_refresh_in_uhz);
		in_out_vrr->adjust.v_total_max =
			mod_freesync_calc_v_total_from_refresh(stream,
				in_out_vrr->min_refresh_in_uhz);
	/* BTR set to "active" so engage */
	} else {

		/* Calculate number of midPoint frames that could fit within
		 * the render time interval - take ceil of this value
		 */
		mid_point_frames_ceil = (last_render_time_in_us +
				in_out_vrr->btr.mid_point_in_us - 1) /
					in_out_vrr->btr.mid_point_in_us;

		if (mid_point_frames_ceil > 0) {
			frame_time_in_us = last_render_time_in_us /
				mid_point_frames_ceil;
			delta_from_mid_point_in_us_1 =
				(in_out_vrr->btr.mid_point_in_us >
				frame_time_in_us) ?
				(in_out_vrr->btr.mid_point_in_us - frame_time_in_us) :
				(frame_time_in_us - in_out_vrr->btr.mid_point_in_us);
		}

		/* Calculate number of midPoint frames that could fit within
		 * the render time interval - take floor of this value
		 */
		mid_point_frames_floor = last_render_time_in_us /
				in_out_vrr->btr.mid_point_in_us;

		if (mid_point_frames_floor > 0) {

			frame_time_in_us = last_render_time_in_us /
				mid_point_frames_floor;
			delta_from_mid_point_in_us_2 =
				(in_out_vrr->btr.mid_point_in_us >
				frame_time_in_us) ?
				(in_out_vrr->btr.mid_point_in_us - frame_time_in_us) :
				(frame_time_in_us - in_out_vrr->btr.mid_point_in_us);
		}

		/* Choose number of frames to insert based on how close it
		 * can get to the mid point of the variable range.
		 *  - Delta for CEIL: delta_from_mid_point_in_us_1
		 *  - Delta for FLOOR: delta_from_mid_point_in_us_2
		 */
		if ((last_render_time_in_us / mid_point_frames_ceil) < in_out_vrr->min_duration_in_us) {
			/* Check for out of range.
			 * If using CEIL produces a value that is out of range,
			 * then we are forced to use FLOOR.
			 */
			frames_to_insert = mid_point_frames_floor;
		} else if (mid_point_frames_floor < 2) {
			/* Check if FLOOR would result in non-LFC. In this case
			 * choose to use CEIL
			 */
			frames_to_insert = mid_point_frames_ceil;
		} else if (delta_from_mid_point_in_us_1 < delta_from_mid_point_in_us_2) {
			/* If choosing CEIL results in a frame duration that is
			 * closer to the mid point of the range.
			 * Choose CEIL
			 */
			frames_to_insert = mid_point_frames_ceil;
		} else {
			/* If choosing FLOOR results in a frame duration that is
			 * closer to the mid point of the range.
			 * Choose FLOOR
			 */
			frames_to_insert = mid_point_frames_floor;
		}

		/* Prefer current frame multiplier when BTR is enabled unless it drifts
		 * too far from the midpoint
		 */
		if (delta_from_mid_point_in_us_1 < delta_from_mid_point_in_us_2) {
			delta_from_mid_point_delta_in_us = delta_from_mid_point_in_us_2 -
					delta_from_mid_point_in_us_1;
		} else {
			delta_from_mid_point_delta_in_us = delta_from_mid_point_in_us_1 -
					delta_from_mid_point_in_us_2;
		}
		if (in_out_vrr->btr.frames_to_insert != 0 &&
				delta_from_mid_point_delta_in_us < BTR_DRIFT_MARGIN) {
			if (((last_render_time_in_us / in_out_vrr->btr.frames_to_insert) <
					max_render_time_in_us) &&
				((last_render_time_in_us / in_out_vrr->btr.frames_to_insert) >
					in_out_vrr->min_duration_in_us))
				frames_to_insert = in_out_vrr->btr.frames_to_insert;
		}

		/* Either we've calculated the number of frames to insert,
		 * or we need to insert min duration frames
		 */
		if (last_render_time_in_us / frames_to_insert <
				in_out_vrr->min_duration_in_us){
			frames_to_insert -= (frames_to_insert > 1) ?
					1 : 0;
		}

		if (frames_to_insert > 0)
			inserted_frame_duration_in_us = last_render_time_in_us /
							frames_to_insert;

		if (inserted_frame_duration_in_us < in_out_vrr->min_duration_in_us)
			inserted_frame_duration_in_us = in_out_vrr->min_duration_in_us;

		/* Cache the calculated variables */
		in_out_vrr->btr.inserted_duration_in_us =
			inserted_frame_duration_in_us;
		in_out_vrr->btr.frames_to_insert = frames_to_insert;
		in_out_vrr->btr.frame_counter = frames_to_insert;
	}
}

static void apply_fixed_refresh(struct core_freesync *core_freesync,
		const struct dc_stream_state *stream,
		unsigned int last_render_time_in_us,
		struct mod_vrr_params *in_out_vrr)
{
	bool update = false;
	unsigned int max_render_time_in_us = in_out_vrr->max_duration_in_us;

	/* Compute the exit refresh rate and exit frame duration */
	unsigned int exit_refresh_rate_in_milli_hz = ((1000000000/max_render_time_in_us)
			+ (1000*FIXED_REFRESH_EXIT_MARGIN_IN_HZ));
	unsigned int exit_frame_duration_in_us = 1000000000/exit_refresh_rate_in_milli_hz;

	if (last_render_time_in_us < exit_frame_duration_in_us) {
		/* Exit Fixed Refresh mode */
		if (in_out_vrr->fixed.fixed_active) {
			in_out_vrr->fixed.frame_counter++;

			if (in_out_vrr->fixed.frame_counter >
					FIXED_REFRESH_EXIT_FRAME_COUNT) {
				in_out_vrr->fixed.frame_counter = 0;
				in_out_vrr->fixed.fixed_active = false;
				in_out_vrr->fixed.target_refresh_in_uhz = 0;
				update = true;
			}
		} else
			in_out_vrr->fixed.frame_counter = 0;
	} else if (last_render_time_in_us > max_render_time_in_us) {
		/* Enter Fixed Refresh mode */
		if (!in_out_vrr->fixed.fixed_active) {
			in_out_vrr->fixed.frame_counter++;

			if (in_out_vrr->fixed.frame_counter >
					FIXED_REFRESH_ENTER_FRAME_COUNT) {
				in_out_vrr->fixed.frame_counter = 0;
				in_out_vrr->fixed.fixed_active = true;
				in_out_vrr->fixed.target_refresh_in_uhz =
						in_out_vrr->max_refresh_in_uhz;
				update = true;
			}
		} else
			in_out_vrr->fixed.frame_counter = 0;
	}

	if (update) {
		if (in_out_vrr->fixed.fixed_active) {
			in_out_vrr->adjust.v_total_min =
				mod_freesync_calc_v_total_from_refresh(
				stream, in_out_vrr->max_refresh_in_uhz);
			in_out_vrr->adjust.v_total_max =
					in_out_vrr->adjust.v_total_min;
		} else {
			in_out_vrr->adjust.v_total_min =
				mod_freesync_calc_v_total_from_refresh(stream,
					in_out_vrr->max_refresh_in_uhz);
			in_out_vrr->adjust.v_total_max =
				mod_freesync_calc_v_total_from_refresh(stream,
					in_out_vrr->min_refresh_in_uhz);
		}
	}
}

static bool vrr_settings_require_update(struct core_freesync *core_freesync,
		struct mod_freesync_config *in_config,
		unsigned int min_refresh_in_uhz,
		unsigned int max_refresh_in_uhz,
		struct mod_vrr_params *in_vrr)
{
	if (in_vrr->state != in_config->state) {
		return true;
	} else if (in_vrr->state == VRR_STATE_ACTIVE_FIXED &&
			in_vrr->fixed.target_refresh_in_uhz !=
					in_config->fixed_refresh_in_uhz) {
		return true;
	} else if (in_vrr->min_refresh_in_uhz != min_refresh_in_uhz) {
		return true;
	} else if (in_vrr->max_refresh_in_uhz != max_refresh_in_uhz) {
		return true;
>>>>>>> upstream/android-13
	}

	return false;
}

<<<<<<< HEAD
static void set_static_ramp_variables(struct core_freesync *core_freesync,
		unsigned int index, bool enable_static_screen)
{
	unsigned int frame_duration = 0;
	unsigned int nominal_refresh_rate = core_freesync->map[index].state.
			nominal_refresh_rate_in_micro_hz;
	unsigned int min_refresh_rate= core_freesync->map[index].caps->
			min_refresh_in_micro_hz;
	struct gradual_static_ramp *static_ramp_variables =
			&core_freesync->map[index].state.static_ramp;

	/* If we are ENABLING static screen, refresh rate should go DOWN.
	 * If we are DISABLING static screen, refresh rate should go UP.
	 */
	if (enable_static_screen)
		static_ramp_variables->ramp_direction_is_up = false;
	else
		static_ramp_variables->ramp_direction_is_up = true;

	/* If ramp is not active, set initial frame duration depending on
	 * whether we are enabling/disabling static screen mode. If the ramp is
	 * already active, ramp should continue in the opposite direction
	 * starting with the current frame duration
	 */
	if (!static_ramp_variables->ramp_is_active) {
		if (enable_static_screen == true) {
			/* Going to lower refresh rate, so start from max
			 * refresh rate (min frame duration)
			 */
			frame_duration = ((unsigned int) (div64_u64(
				(1000000000ULL * 1000000),
				nominal_refresh_rate)));
		} else {
			/* Going to higher refresh rate, so start from min
			 * refresh rate (max frame duration)
			 */
			frame_duration = ((unsigned int) (div64_u64(
				(1000000000ULL * 1000000),
				min_refresh_rate)));
		}
		static_ramp_variables->
			ramp_current_frame_duration_in_ns = frame_duration;

		static_ramp_variables->ramp_is_active = true;
	}
}

void mod_freesync_handle_v_update(struct mod_freesync *mod_freesync,
		struct dc_stream_state **streams, int num_streams)
{
	unsigned int index, v_total, inserted_frame_v_total = 0;
	unsigned int min_frame_duration_in_ns, vmax, vmin = 0;
	struct freesync_state *state;
	struct core_freesync *core_freesync = NULL;
	struct dc_static_screen_events triggers = {0};

	if (mod_freesync == NULL)
		return;

	core_freesync = MOD_FREESYNC_TO_CORE(mod_freesync);

	if (core_freesync->num_entities == 0)
		return;

	index = map_index_from_stream(core_freesync,
		streams[0]);

	if (core_freesync->map[index].caps->supported == false)
		return;

	state = &core_freesync->map[index].state;

	/* Below the Range Logic */

	/* Only execute if in fullscreen mode */
	if (state->fullscreen == true &&
		core_freesync->map[index].user_enable.enable_for_gaming &&
		core_freesync->map[index].caps->btr_supported &&
		state->btr.btr_active) {

		/* TODO: pass in flag for Pre-DCE12 ASIC
		 * in order for frame variable duration to take affect,
		 * it needs to be done one VSYNC early, which is at
		 * frameCounter == 1.
		 * For DCE12 and newer updates to V_TOTAL_MIN/MAX
		 * will take affect on current frame
		 */
		if (state->btr.frames_to_insert == state->btr.frame_counter) {

			min_frame_duration_in_ns = ((unsigned int) (div64_u64(
					(1000000000ULL * 1000000),
					state->nominal_refresh_rate_in_micro_hz)));

			vmin = state->freesync_range.vmin;

			inserted_frame_v_total = vmin;

			if (min_frame_duration_in_ns / 1000)
				inserted_frame_v_total =
					state->btr.inserted_frame_duration_in_us *
					vmin / (min_frame_duration_in_ns / 1000);

			/* Set length of inserted frames as v_total_max*/
			vmax = inserted_frame_v_total;
			vmin = inserted_frame_v_total;

			/* Program V_TOTAL */
			adjust_vmin_vmax(core_freesync, streams,
						num_streams, index,
						vmin, vmax);
		}

		if (state->btr.frame_counter > 0)
			state->btr.frame_counter--;

		/* Restore FreeSync */
		if (state->btr.frame_counter == 0)
			set_freesync_on_streams(core_freesync, streams, num_streams);
	}

	/* If in fullscreen freesync mode or in video, do not program
	 * static screen ramp values
	 */
	if (state->fullscreen == true || state->video == true) {

		state->static_ramp.ramp_is_active = false;

		return;
	}

	/* Gradual Static Screen Ramping Logic */

	/* Execute if ramp is active and user enabled freesync static screen*/
	if (state->static_ramp.ramp_is_active &&
		core_freesync->map[index].user_enable.enable_for_static) {

		calc_v_total_for_static_ramp(core_freesync, streams[0],
				index, &v_total);

		/* Update the freesync context for the stream */
		update_stream_freesync_context(core_freesync, streams[0]);

		/* Program static screen ramp values */
		adjust_vmin_vmax(core_freesync, streams,
					num_streams, index,
					v_total,
					v_total);

		triggers.overlay_update = true;
		triggers.surface_update = true;

		dc_stream_set_static_screen_events(core_freesync->dc, streams,
						   num_streams, &triggers);
	}
}

void mod_freesync_update_state(struct mod_freesync *mod_freesync,
		struct dc_stream_state **streams, int num_streams,
		struct mod_freesync_params *freesync_params)
{
	bool freesync_program_required = false;
	unsigned int stream_index;
	struct freesync_state *state;
	struct core_freesync *core_freesync = NULL;
	struct dc_static_screen_events triggers = {0};

	if (mod_freesync == NULL)
		return;

	core_freesync = MOD_FREESYNC_TO_CORE(mod_freesync);

	if (core_freesync->num_entities == 0)
		return;

	for(stream_index = 0; stream_index < num_streams; stream_index++) {

		unsigned int map_index = map_index_from_stream(core_freesync,
				streams[stream_index]);

		bool is_embedded = dc_is_embedded_signal(
				streams[stream_index]->sink->sink_signal);

		struct freesync_registry_options *opts = &core_freesync->opts;

		state = &core_freesync->map[map_index].state;

		switch (freesync_params->state){
		case FREESYNC_STATE_FULLSCREEN:
			state->fullscreen = freesync_params->enable;
			freesync_program_required = true;
			state->windowed_fullscreen =
					freesync_params->windowed_fullscreen;
			break;
		case FREESYNC_STATE_STATIC_SCREEN:
			/* Static screen ramp is disabled by default, but can
			 * be enabled through regkey.
			 */
			if ((is_embedded && opts->drr_internal_supported) ||
				(!is_embedded && opts->drr_external_supported))

				if (state->static_screen !=
						freesync_params->enable) {

					/* Change the state flag */
					state->static_screen =
							freesync_params->enable;

					/* Update static screen ramp */
					set_static_ramp_variables(core_freesync,
						map_index,
						freesync_params->enable);
				}
			/* We program the ramp starting next VUpdate */
			break;
		case FREESYNC_STATE_VIDEO:
			/* Change core variables only if there is a change*/
			if(freesync_params->update_duration_in_ns !=
				state->time.update_duration_in_ns) {

				state->video = freesync_params->enable;
				state->time.update_duration_in_ns =
					freesync_params->update_duration_in_ns;

				freesync_program_required = true;
			}
			break;
		case FREESYNC_STATE_NONE:
			/* handle here to avoid warning */
			break;
		}
	}

	/* Update mask */
	triggers.overlay_update = true;
	triggers.surface_update = true;

	dc_stream_set_static_screen_events(core_freesync->dc, streams,
					   num_streams, &triggers);

	if (freesync_program_required)
		/* Program freesync according to current state*/
		set_freesync_on_streams(core_freesync, streams, num_streams);
}


bool mod_freesync_get_state(struct mod_freesync *mod_freesync,
		struct dc_stream_state *stream,
		struct mod_freesync_params *freesync_params)
{
	unsigned int index = 0;
	struct core_freesync *core_freesync = NULL;

	if (mod_freesync == NULL)
		return false;

	core_freesync = MOD_FREESYNC_TO_CORE(mod_freesync);
	index = map_index_from_stream(core_freesync, stream);

	if (core_freesync->map[index].state.fullscreen) {
		freesync_params->state = FREESYNC_STATE_FULLSCREEN;
		freesync_params->enable = true;
	} else if (core_freesync->map[index].state.static_screen) {
		freesync_params->state = FREESYNC_STATE_STATIC_SCREEN;
		freesync_params->enable = true;
	} else if (core_freesync->map[index].state.video) {
		freesync_params->state = FREESYNC_STATE_VIDEO;
		freesync_params->enable = true;
	} else {
		freesync_params->state = FREESYNC_STATE_NONE;
		freesync_params->enable = false;
	}

	freesync_params->update_duration_in_ns =
		core_freesync->map[index].state.time.update_duration_in_ns;

	freesync_params->windowed_fullscreen =
			core_freesync->map[index].state.windowed_fullscreen;

	return true;
}

bool mod_freesync_set_user_enable(struct mod_freesync *mod_freesync,
		struct dc_stream_state **streams, int num_streams,
		struct mod_freesync_user_enable *user_enable)
{
	unsigned int stream_index, map_index;
	int persistent_data = 0;
	struct persistent_data_flag flag;
	struct dc  *dc = NULL;
	struct core_freesync *core_freesync = NULL;

	if (mod_freesync == NULL)
		return false;

	core_freesync = MOD_FREESYNC_TO_CORE(mod_freesync);
	dc = core_freesync->dc;

	flag.save_per_edid = true;
	flag.save_per_link = false;

	for(stream_index = 0; stream_index < num_streams;
			stream_index++){

		map_index = map_index_from_stream(core_freesync,
				streams[stream_index]);

		core_freesync->map[map_index].user_enable = *user_enable;

		/* Write persistent data in registry*/
		if (core_freesync->map[map_index].user_enable.
				enable_for_gaming)
			persistent_data = persistent_data | 1;
		if (core_freesync->map[map_index].user_enable.
				enable_for_static)
			persistent_data = persistent_data | 2;
		if (core_freesync->map[map_index].user_enable.
				enable_for_video)
			persistent_data = persistent_data | 4;

		dm_write_persistent_data(dc->ctx,
					streams[stream_index]->sink,
					FREESYNC_REGISTRY_NAME,
					"userenable",
					&persistent_data,
					sizeof(int),
					&flag);
	}

	set_freesync_on_streams(core_freesync, streams, num_streams);

	return true;
}

bool mod_freesync_get_user_enable(struct mod_freesync *mod_freesync,
		struct dc_stream_state *stream,
		struct mod_freesync_user_enable *user_enable)
{
	unsigned int index = 0;
	struct core_freesync *core_freesync = NULL;

	if (mod_freesync == NULL)
		return false;

	core_freesync = MOD_FREESYNC_TO_CORE(mod_freesync);
	index = map_index_from_stream(core_freesync, stream);

	*user_enable = core_freesync->map[index].user_enable;

	return true;
}

bool mod_freesync_get_static_ramp_active(struct mod_freesync *mod_freesync,
		struct dc_stream_state *stream,
		bool *is_ramp_active)
{
	unsigned int index = 0;
	struct core_freesync *core_freesync = NULL;

	if (mod_freesync == NULL)
		return false;

	core_freesync = MOD_FREESYNC_TO_CORE(mod_freesync);
	index = map_index_from_stream(core_freesync, stream);

	*is_ramp_active =
		core_freesync->map[index].state.static_ramp.ramp_is_active;

	return true;
}

bool mod_freesync_override_min_max(struct mod_freesync *mod_freesync,
		struct dc_stream_state *streams,
		unsigned int min_refresh,
		unsigned int max_refresh,
		struct mod_freesync_caps *caps)
{
	unsigned int index = 0;
	struct core_freesync *core_freesync;
	struct freesync_state *state;

	if (mod_freesync == NULL)
		return false;

	core_freesync = MOD_FREESYNC_TO_CORE(mod_freesync);
	index = map_index_from_stream(core_freesync, streams);
	state = &core_freesync->map[index].state;

	if (max_refresh == 0)
		max_refresh = state->nominal_refresh_rate_in_micro_hz;

	if (min_refresh == 0) {
		/* Restore defaults */
		calc_freesync_range(core_freesync, streams, state,
			core_freesync->map[index].caps->
			min_refresh_in_micro_hz,
			state->nominal_refresh_rate_in_micro_hz);
	} else {
		calc_freesync_range(core_freesync, streams,
				state,
				min_refresh,
				max_refresh);

		/* Program vtotal min/max */
		adjust_vmin_vmax(core_freesync, &streams, 1, index,
				state->freesync_range.vmin,
				state->freesync_range.vmax);
	}

	if (min_refresh != 0 &&
			dc_is_embedded_signal(streams->sink->sink_signal) &&
			(max_refresh - min_refresh >= 10000000)) {
		caps->supported = true;
		caps->min_refresh_in_micro_hz = min_refresh;
		caps->max_refresh_in_micro_hz = max_refresh;
	}

	/* Update the stream */
	update_stream(core_freesync, streams);

	return true;
}

bool mod_freesync_get_min_max(struct mod_freesync *mod_freesync,
		struct dc_stream_state *stream,
		unsigned int *min_refresh,
		unsigned int *max_refresh)
{
	unsigned int index = 0;
	struct core_freesync *core_freesync = NULL;

	if (mod_freesync == NULL)
		return false;

	core_freesync = MOD_FREESYNC_TO_CORE(mod_freesync);
	index = map_index_from_stream(core_freesync, stream);

	*min_refresh =
		core_freesync->map[index].state.freesync_range.min_refresh;
	*max_refresh =
		core_freesync->map[index].state.freesync_range.max_refresh;

	return true;
}

bool mod_freesync_get_vmin_vmax(struct mod_freesync *mod_freesync,
		struct dc_stream_state *stream,
		unsigned int *vmin,
		unsigned int *vmax)
{
	unsigned int index = 0;
	struct core_freesync *core_freesync = NULL;

	if (mod_freesync == NULL)
		return false;

	core_freesync = MOD_FREESYNC_TO_CORE(mod_freesync);
	index = map_index_from_stream(core_freesync, stream);

	*vmin =
		core_freesync->map[index].state.freesync_range.vmin;
	*vmax =
		core_freesync->map[index].state.freesync_range.vmax;
=======
bool mod_freesync_get_vmin_vmax(struct mod_freesync *mod_freesync,
		const struct dc_stream_state *stream,
		unsigned int *vmin,
		unsigned int *vmax)
{
	*vmin = stream->adjust.v_total_min;
	*vmax = stream->adjust.v_total_max;
>>>>>>> upstream/android-13

	return true;
}

bool mod_freesync_get_v_position(struct mod_freesync *mod_freesync,
		struct dc_stream_state *stream,
		unsigned int *nom_v_pos,
		unsigned int *v_pos)
{
<<<<<<< HEAD
	unsigned int index = 0;
=======
>>>>>>> upstream/android-13
	struct core_freesync *core_freesync = NULL;
	struct crtc_position position;

	if (mod_freesync == NULL)
		return false;

	core_freesync = MOD_FREESYNC_TO_CORE(mod_freesync);
<<<<<<< HEAD
	index = map_index_from_stream(core_freesync, stream);
=======
>>>>>>> upstream/android-13

	if (dc_stream_get_crtc_position(core_freesync->dc, &stream, 1,
					&position.vertical_count,
					&position.nominal_vcount)) {

		*nom_v_pos = position.nominal_vcount;
		*v_pos = position.vertical_count;

		return true;
	}

	return false;
}

<<<<<<< HEAD
void mod_freesync_notify_mode_change(struct mod_freesync *mod_freesync,
		struct dc_stream_state **streams, int num_streams)
{
	unsigned int stream_index, map_index;
	struct freesync_state *state;
	struct core_freesync *core_freesync = NULL;
	struct dc_static_screen_events triggers = {0};
	unsigned long long temp = 0;
=======
static void build_vrr_infopacket_data_v1(const struct mod_vrr_params *vrr,
		struct dc_info_packet *infopacket,
		bool freesync_on_desktop)
{
	/* PB1 = 0x1A (24bit AMD IEEE OUI (0x00001A) - Byte 0) */
	infopacket->sb[1] = 0x1A;

	/* PB2 = 0x00 (24bit AMD IEEE OUI (0x00001A) - Byte 1) */
	infopacket->sb[2] = 0x00;

	/* PB3 = 0x00 (24bit AMD IEEE OUI (0x00001A) - Byte 2) */
	infopacket->sb[3] = 0x00;

	/* PB4 = Reserved */

	/* PB5 = Reserved */

	/* PB6 = [Bits 7:3 = Reserved] */

	/* PB6 = [Bit 0 = FreeSync Supported] */
	if (vrr->state != VRR_STATE_UNSUPPORTED)
		infopacket->sb[6] |= 0x01;

	/* PB6 = [Bit 1 = FreeSync Enabled] */
	if (vrr->state != VRR_STATE_DISABLED &&
			vrr->state != VRR_STATE_UNSUPPORTED)
		infopacket->sb[6] |= 0x02;

	if (freesync_on_desktop) {
		/* PB6 = [Bit 2 = FreeSync Active] */
		if (vrr->state != VRR_STATE_DISABLED &&
			vrr->state != VRR_STATE_UNSUPPORTED)
			infopacket->sb[6] |= 0x04;
	} else {
		if (vrr->state == VRR_STATE_ACTIVE_VARIABLE ||
			vrr->state == VRR_STATE_ACTIVE_FIXED)
			infopacket->sb[6] |= 0x04;
	}

	// For v1 & 2 infoframes program nominal if non-fs mode, otherwise full range
	/* PB7 = FreeSync Minimum refresh rate (Hz) */
	if (vrr->state == VRR_STATE_ACTIVE_VARIABLE ||
			vrr->state == VRR_STATE_ACTIVE_FIXED) {
		infopacket->sb[7] = (unsigned char)((vrr->min_refresh_in_uhz + 500000) / 1000000);
	} else {
		infopacket->sb[7] = (unsigned char)((vrr->max_refresh_in_uhz + 500000) / 1000000);
	}

	/* PB8 = FreeSync Maximum refresh rate (Hz)
	 * Note: We should never go above the field rate of the mode timing set.
	 */
	infopacket->sb[8] = (unsigned char)((vrr->max_refresh_in_uhz + 500000) / 1000000);

	/* FreeSync HDR */
	infopacket->sb[9] = 0;
	infopacket->sb[10] = 0;
}

static void build_vrr_infopacket_data_v3(const struct mod_vrr_params *vrr,
		struct dc_info_packet *infopacket)
{
	unsigned int min_refresh;
	unsigned int max_refresh;
	unsigned int fixed_refresh;
	unsigned int min_programmed;
	unsigned int max_programmed;

	/* PB1 = 0x1A (24bit AMD IEEE OUI (0x00001A) - Byte 0) */
	infopacket->sb[1] = 0x1A;

	/* PB2 = 0x00 (24bit AMD IEEE OUI (0x00001A) - Byte 1) */
	infopacket->sb[2] = 0x00;

	/* PB3 = 0x00 (24bit AMD IEEE OUI (0x00001A) - Byte 2) */
	infopacket->sb[3] = 0x00;

	/* PB4 = Reserved */

	/* PB5 = Reserved */

	/* PB6 = [Bits 7:3 = Reserved] */

	/* PB6 = [Bit 0 = FreeSync Supported] */
	if (vrr->state != VRR_STATE_UNSUPPORTED)
		infopacket->sb[6] |= 0x01;

	/* PB6 = [Bit 1 = FreeSync Enabled] */
	if (vrr->state != VRR_STATE_DISABLED &&
			vrr->state != VRR_STATE_UNSUPPORTED)
		infopacket->sb[6] |= 0x02;

	/* PB6 = [Bit 2 = FreeSync Active] */
	if (vrr->state == VRR_STATE_ACTIVE_VARIABLE ||
			vrr->state == VRR_STATE_ACTIVE_FIXED)
		infopacket->sb[6] |= 0x04;

	min_refresh = (vrr->min_refresh_in_uhz + 500000) / 1000000;
	max_refresh = (vrr->max_refresh_in_uhz + 500000) / 1000000;
	fixed_refresh = (vrr->fixed_refresh_in_uhz + 500000) / 1000000;

	min_programmed = (vrr->state == VRR_STATE_ACTIVE_FIXED) ? fixed_refresh :
			(vrr->state == VRR_STATE_ACTIVE_VARIABLE) ? min_refresh :
			(vrr->state == VRR_STATE_INACTIVE) ? min_refresh :
			max_refresh; // Non-fs case, program nominal range

	max_programmed = (vrr->state == VRR_STATE_ACTIVE_FIXED) ? fixed_refresh :
			(vrr->state == VRR_STATE_ACTIVE_VARIABLE) ? max_refresh :
			max_refresh;// Non-fs case, program nominal range

	/* PB7 = FreeSync Minimum refresh rate (Hz) */
	infopacket->sb[7] = min_programmed & 0xFF;

	/* PB8 = FreeSync Maximum refresh rate (Hz) */
	infopacket->sb[8] = max_programmed & 0xFF;

	/* PB11 : MSB FreeSync Minimum refresh rate [Hz] - bits 9:8 */
	infopacket->sb[11] = (min_programmed >> 8) & 0x03;

	/* PB12 : MSB FreeSync Maximum refresh rate [Hz] - bits 9:8 */
	infopacket->sb[12] = (max_programmed >> 8) & 0x03;

	/* PB16 : Reserved bits 7:1, FixedRate bit 0 */
	infopacket->sb[16] = (vrr->state == VRR_STATE_ACTIVE_FIXED) ? 1 : 0;

	//FreeSync HDR
	infopacket->sb[9] = 0;
	infopacket->sb[10] = 0;
}

static void build_vrr_infopacket_fs2_data(enum color_transfer_func app_tf,
		struct dc_info_packet *infopacket)
{
	if (app_tf != TRANSFER_FUNC_UNKNOWN) {
		infopacket->valid = true;

		infopacket->sb[6] |= 0x08;  // PB6 = [Bit 3 = Native Color Active]

		if (app_tf == TRANSFER_FUNC_GAMMA_22) {
			infopacket->sb[9] |= 0x04;  // PB6 = [Bit 2 = Gamma 2.2 EOTF Active]
		}
	}
}

static void build_vrr_infopacket_header_v1(enum signal_type signal,
		struct dc_info_packet *infopacket,
		unsigned int *payload_size)
{
	if (dc_is_hdmi_signal(signal)) {

		/* HEADER */

		/* HB0  = Packet Type = 0x83 (Source Product
		 *	  Descriptor InfoFrame)
		 */
		infopacket->hb0 = DC_HDMI_INFOFRAME_TYPE_SPD;

		/* HB1  = Version = 0x01 */
		infopacket->hb1 = 0x01;

		/* HB2  = [Bits 7:5 = 0] [Bits 4:0 = Length = 0x08] */
		infopacket->hb2 = 0x08;

		*payload_size = 0x08;

	} else if (dc_is_dp_signal(signal)) {

		/* HEADER */

		/* HB0  = Secondary-data Packet ID = 0 - Only non-zero
		 *	  when used to associate audio related info packets
		 */
		infopacket->hb0 = 0x00;

		/* HB1  = Packet Type = 0x83 (Source Product
		 *	  Descriptor InfoFrame)
		 */
		infopacket->hb1 = DC_HDMI_INFOFRAME_TYPE_SPD;

		/* HB2  = [Bits 7:0 = Least significant eight bits -
		 *	  For INFOFRAME, the value must be 1Bh]
		 */
		infopacket->hb2 = 0x1B;

		/* HB3  = [Bits 7:2 = INFOFRAME SDP Version Number = 0x1]
		 *	  [Bits 1:0 = Most significant two bits = 0x00]
		 */
		infopacket->hb3 = 0x04;

		*payload_size = 0x1B;
	}
}

static void build_vrr_infopacket_header_v2(enum signal_type signal,
		struct dc_info_packet *infopacket,
		unsigned int *payload_size)
{
	if (dc_is_hdmi_signal(signal)) {

		/* HEADER */

		/* HB0  = Packet Type = 0x83 (Source Product
		 *	  Descriptor InfoFrame)
		 */
		infopacket->hb0 = DC_HDMI_INFOFRAME_TYPE_SPD;

		/* HB1  = Version = 0x02 */
		infopacket->hb1 = 0x02;

		/* HB2  = [Bits 7:5 = 0] [Bits 4:0 = Length = 0x09] */
		infopacket->hb2 = 0x09;

		*payload_size = 0x0A;

	} else if (dc_is_dp_signal(signal)) {

		/* HEADER */

		/* HB0  = Secondary-data Packet ID = 0 - Only non-zero
		 *	  when used to associate audio related info packets
		 */
		infopacket->hb0 = 0x00;

		/* HB1  = Packet Type = 0x83 (Source Product
		 *	  Descriptor InfoFrame)
		 */
		infopacket->hb1 = DC_HDMI_INFOFRAME_TYPE_SPD;

		/* HB2  = [Bits 7:0 = Least significant eight bits -
		 *	  For INFOFRAME, the value must be 1Bh]
		 */
		infopacket->hb2 = 0x1B;

		/* HB3  = [Bits 7:2 = INFOFRAME SDP Version Number = 0x2]
		 *	  [Bits 1:0 = Most significant two bits = 0x00]
		 */
		infopacket->hb3 = 0x08;

		*payload_size = 0x1B;
	}
}

static void build_vrr_infopacket_header_v3(enum signal_type signal,
		struct dc_info_packet *infopacket,
		unsigned int *payload_size)
{
	unsigned char version;

	version = 3;
	if (dc_is_hdmi_signal(signal)) {

		/* HEADER */

		/* HB0  = Packet Type = 0x83 (Source Product
		 *	  Descriptor InfoFrame)
		 */
		infopacket->hb0 = DC_HDMI_INFOFRAME_TYPE_SPD;

		/* HB1  = Version = 0x03 */
		infopacket->hb1 = version;

		/* HB2  = [Bits 7:5 = 0] [Bits 4:0 = Length] */
		*payload_size = 0x10;
		infopacket->hb2 = *payload_size - 1; //-1 for checksum

	} else if (dc_is_dp_signal(signal)) {

		/* HEADER */

		/* HB0  = Secondary-data Packet ID = 0 - Only non-zero
		 *	  when used to associate audio related info packets
		 */
		infopacket->hb0 = 0x00;

		/* HB1  = Packet Type = 0x83 (Source Product
		 *	  Descriptor InfoFrame)
		 */
		infopacket->hb1 = DC_HDMI_INFOFRAME_TYPE_SPD;

		/* HB2  = [Bits 7:0 = Least significant eight bits -
		 *	  For INFOFRAME, the value must be 1Bh]
		 */
		infopacket->hb2 = 0x1B;

		/* HB3  = [Bits 7:2 = INFOFRAME SDP Version Number = 0x2]
		 *	  [Bits 1:0 = Most significant two bits = 0x00]
		 */

		infopacket->hb3 = (version & 0x3F) << 2;

		*payload_size = 0x1B;
	}
}

static void build_vrr_infopacket_checksum(unsigned int *payload_size,
		struct dc_info_packet *infopacket)
{
	/* Calculate checksum */
	unsigned int idx = 0;
	unsigned char checksum = 0;

	checksum += infopacket->hb0;
	checksum += infopacket->hb1;
	checksum += infopacket->hb2;
	checksum += infopacket->hb3;

	for (idx = 1; idx <= *payload_size; idx++)
		checksum += infopacket->sb[idx];

	/* PB0 = Checksum (one byte complement) */
	infopacket->sb[0] = (unsigned char)(0x100 - checksum);

	infopacket->valid = true;
}

static void build_vrr_infopacket_v1(enum signal_type signal,
		const struct mod_vrr_params *vrr,
		struct dc_info_packet *infopacket,
		bool freesync_on_desktop)
{
	/* SPD info packet for FreeSync */
	unsigned int payload_size = 0;

	build_vrr_infopacket_header_v1(signal, infopacket, &payload_size);
	build_vrr_infopacket_data_v1(vrr, infopacket, freesync_on_desktop);
	build_vrr_infopacket_checksum(&payload_size, infopacket);

	infopacket->valid = true;
}

static void build_vrr_infopacket_v2(enum signal_type signal,
		const struct mod_vrr_params *vrr,
		enum color_transfer_func app_tf,
		struct dc_info_packet *infopacket,
		bool freesync_on_desktop)
{
	unsigned int payload_size = 0;

	build_vrr_infopacket_header_v2(signal, infopacket, &payload_size);
	build_vrr_infopacket_data_v1(vrr, infopacket, freesync_on_desktop);

	build_vrr_infopacket_fs2_data(app_tf, infopacket);

	build_vrr_infopacket_checksum(&payload_size, infopacket);

	infopacket->valid = true;
}
#ifndef TRIM_FSFT
static void build_vrr_infopacket_fast_transport_data(
	bool ftActive,
	unsigned int ftOutputRate,
	struct dc_info_packet *infopacket)
{
	/* PB9 : bit7 - fast transport Active*/
	unsigned char activeBit = (ftActive) ? 1 << 7 : 0;

	infopacket->sb[1] &= ~activeBit;  //clear bit
	infopacket->sb[1] |=  activeBit;  //set bit

	/* PB13 : Target Output Pixel Rate [kHz] - bits 7:0  */
	infopacket->sb[13] = ftOutputRate & 0xFF;

	/* PB14 : Target Output Pixel Rate [kHz] - bits 15:8  */
	infopacket->sb[14] = (ftOutputRate >> 8) & 0xFF;

	/* PB15 : Target Output Pixel Rate [kHz] - bits 23:16  */
	infopacket->sb[15] = (ftOutputRate >> 16) & 0xFF;

}
#endif

static void build_vrr_infopacket_v3(enum signal_type signal,
		const struct mod_vrr_params *vrr,
#ifndef TRIM_FSFT
		bool ftActive, unsigned int ftOutputRate,
#endif
		enum color_transfer_func app_tf,
		struct dc_info_packet *infopacket)
{
	unsigned int payload_size = 0;

	build_vrr_infopacket_header_v3(signal, infopacket, &payload_size);
	build_vrr_infopacket_data_v3(vrr, infopacket);

	build_vrr_infopacket_fs2_data(app_tf, infopacket);

#ifndef TRIM_FSFT
	build_vrr_infopacket_fast_transport_data(
			ftActive,
			ftOutputRate,
			infopacket);
#endif

	build_vrr_infopacket_checksum(&payload_size, infopacket);

	infopacket->valid = true;
}

static void build_vrr_infopacket_sdp_v1_3(enum vrr_packet_type packet_type,
										struct dc_info_packet *infopacket)
{
	uint8_t idx = 0, size = 0;

	size = ((packet_type == PACKET_TYPE_FS_V1) ? 0x08 :
			(packet_type == PACKET_TYPE_FS_V3) ? 0x10 :
												0x09);

	for (idx = infopacket->hb2; idx > 1; idx--) // Data Byte Count: 0x1B
		infopacket->sb[idx] = infopacket->sb[idx-1];

	infopacket->sb[1] = size;                         // Length
	infopacket->sb[0] = (infopacket->hb3 >> 2) & 0x3F;//Version
	infopacket->hb3   = (0x13 << 2);                  // Header,SDP 1.3
	infopacket->hb2   = 0x1D;
}

void mod_freesync_build_vrr_infopacket(struct mod_freesync *mod_freesync,
		const struct dc_stream_state *stream,
		const struct mod_vrr_params *vrr,
		enum vrr_packet_type packet_type,
		enum color_transfer_func app_tf,
		struct dc_info_packet *infopacket,
		bool pack_sdp_v1_3)
{
	/* SPD info packet for FreeSync
	 * VTEM info packet for HdmiVRR
	 * Check if Freesync is supported. Return if false. If true,
	 * set the corresponding bit in the info packet
	 */
	if (!vrr->send_info_frame)
		return;

	switch (packet_type) {
	case PACKET_TYPE_FS_V3:
#ifndef TRIM_FSFT
		// always populate with pixel rate.
		build_vrr_infopacket_v3(
				stream->signal, vrr,
				stream->timing.flags.FAST_TRANSPORT,
				(stream->timing.flags.FAST_TRANSPORT) ?
						stream->timing.fast_transport_output_rate_100hz :
						stream->timing.pix_clk_100hz,
				app_tf, infopacket);
#else
		build_vrr_infopacket_v3(stream->signal, vrr, app_tf, infopacket);
#endif
		break;
	case PACKET_TYPE_FS_V2:
		build_vrr_infopacket_v2(stream->signal, vrr, app_tf, infopacket, stream->freesync_on_desktop);
		break;
	case PACKET_TYPE_VRR:
	case PACKET_TYPE_FS_V1:
	default:
		build_vrr_infopacket_v1(stream->signal, vrr, infopacket, stream->freesync_on_desktop);
	}

	if (true == pack_sdp_v1_3 &&
		true == dc_is_dp_signal(stream->signal) &&
		packet_type != PACKET_TYPE_VRR &&
		packet_type != PACKET_TYPE_VTEM)
		build_vrr_infopacket_sdp_v1_3(packet_type, infopacket);
}

void mod_freesync_build_vrr_params(struct mod_freesync *mod_freesync,
		const struct dc_stream_state *stream,
		struct mod_freesync_config *in_config,
		struct mod_vrr_params *in_out_vrr)
{
	struct core_freesync *core_freesync = NULL;
	unsigned long long nominal_field_rate_in_uhz = 0;
	unsigned long long rounded_nominal_in_uhz = 0;
	unsigned int refresh_range = 0;
	unsigned long long min_refresh_in_uhz = 0;
	unsigned long long max_refresh_in_uhz = 0;
>>>>>>> upstream/android-13

	if (mod_freesync == NULL)
		return;

	core_freesync = MOD_FREESYNC_TO_CORE(mod_freesync);

<<<<<<< HEAD
	for (stream_index = 0; stream_index < num_streams; stream_index++) {
		map_index = map_index_from_stream(core_freesync,
				streams[stream_index]);

		state = &core_freesync->map[map_index].state;

		/* Update the field rate for new timing */
		temp = streams[stream_index]->timing.pix_clk_khz;
		temp *= 1000ULL * 1000ULL * 1000ULL;
		temp = div_u64(temp,
				streams[stream_index]->timing.h_total);
		temp = div_u64(temp,
				streams[stream_index]->timing.v_total);
		state->nominal_refresh_rate_in_micro_hz =
				(unsigned int) temp;

		if (core_freesync->map[map_index].caps->supported) {

			/* Update the stream */
			update_stream(core_freesync, streams[stream_index]);

			/* Calculate vmin/vmax and refresh rate for
			 * current mode
			 */
			calc_freesync_range(core_freesync, *streams, state,
				core_freesync->map[map_index].caps->
				min_refresh_in_micro_hz,
				state->nominal_refresh_rate_in_micro_hz);

			/* Update mask */
			triggers.overlay_update = true;
			triggers.surface_update = true;

			dc_stream_set_static_screen_events(core_freesync->dc,
							   streams, num_streams,
							   &triggers);
		}
	}

	/* Program freesync according to current state*/
	set_freesync_on_streams(core_freesync, streams, num_streams);
}

/* Add the timestamps to the cache and determine whether BTR programming
 * is required, depending on the times calculated
 */
static void update_timestamps(struct core_freesync *core_freesync,
		const struct dc_stream_state *stream, unsigned int map_index,
		unsigned int last_render_time_in_us)
{
	struct freesync_state *state = &core_freesync->map[map_index].state;

	state->time.render_times[state->time.render_times_index] =
			last_render_time_in_us;
	state->time.render_times_index++;

	if (state->time.render_times_index >= RENDER_TIMES_MAX_COUNT)
		state->time.render_times_index = 0;

	if (last_render_time_in_us + BTR_EXIT_MARGIN <
		state->time.max_render_time_in_us) {

		/* Exit Below the Range */
		if (state->btr.btr_active) {

			state->btr.program_btr = true;
			state->btr.btr_active = false;
			state->btr.frame_counter = 0;

		/* Exit Fixed Refresh mode */
		} else if (state->fixed_refresh.fixed_active) {

			state->fixed_refresh.frame_counter++;

			if (state->fixed_refresh.frame_counter >
					FIXED_REFRESH_EXIT_FRAME_COUNT) {
				state->fixed_refresh.frame_counter = 0;
				state->fixed_refresh.program_fixed = true;
				state->fixed_refresh.fixed_active = false;
			}
		}

	} else if (last_render_time_in_us > state->time.max_render_time_in_us) {

		/* Enter Below the Range */
		if (!state->btr.btr_active &&
			core_freesync->map[map_index].caps->btr_supported) {

			state->btr.program_btr = true;
			state->btr.btr_active = true;

		/* Enter Fixed Refresh mode */
		} else if (!state->fixed_refresh.fixed_active &&
			!core_freesync->map[map_index].caps->btr_supported) {

			state->fixed_refresh.frame_counter++;

			if (state->fixed_refresh.frame_counter >
					FIXED_REFRESH_ENTER_FRAME_COUNT) {
				state->fixed_refresh.frame_counter = 0;
				state->fixed_refresh.program_fixed = true;
				state->fixed_refresh.fixed_active = true;
			}
		}
	}

	/* When Below the Range is active, must react on every frame */
	if (state->btr.btr_active)
		state->btr.program_btr = true;
}

static void apply_below_the_range(struct core_freesync *core_freesync,
		struct dc_stream_state *stream, unsigned int map_index,
		unsigned int last_render_time_in_us)
{
	unsigned int inserted_frame_duration_in_us = 0;
	unsigned int mid_point_frames_ceil = 0;
	unsigned int mid_point_frames_floor = 0;
	unsigned int frame_time_in_us = 0;
	unsigned int delta_from_mid_point_in_us_1 = 0xFFFFFFFF;
	unsigned int delta_from_mid_point_in_us_2 = 0xFFFFFFFF;
	unsigned int frames_to_insert = 0;
	unsigned int min_frame_duration_in_ns = 0;
	struct freesync_state *state = &core_freesync->map[map_index].state;

	if (!state->btr.program_btr)
		return;

	state->btr.program_btr = false;

	min_frame_duration_in_ns = ((unsigned int) (div64_u64(
		(1000000000ULL * 1000000),
		state->nominal_refresh_rate_in_micro_hz)));

	/* Program BTR */

	/* BTR set to "not active" so disengage */
	if (!state->btr.btr_active)

		/* Restore FreeSync */
		set_freesync_on_streams(core_freesync, &stream, 1);

	/* BTR set to "active" so engage */
	else {

		/* Calculate number of midPoint frames that could fit within
		 * the render time interval- take ceil of this value
		 */
		mid_point_frames_ceil = (last_render_time_in_us +
			state->btr.mid_point_in_us- 1) /
			state->btr.mid_point_in_us;

		if (mid_point_frames_ceil > 0) {

			frame_time_in_us = last_render_time_in_us /
				mid_point_frames_ceil;
			delta_from_mid_point_in_us_1 =
				(state->btr.mid_point_in_us >
				frame_time_in_us) ?
				(state->btr.mid_point_in_us - frame_time_in_us):
				(frame_time_in_us - state->btr.mid_point_in_us);
		}

		/* Calculate number of midPoint frames that could fit within
		 * the render time interval- take floor of this value
		 */
		mid_point_frames_floor = last_render_time_in_us /
			state->btr.mid_point_in_us;

		if (mid_point_frames_floor > 0) {

			frame_time_in_us = last_render_time_in_us /
				mid_point_frames_floor;
			delta_from_mid_point_in_us_2 =
				(state->btr.mid_point_in_us >
				frame_time_in_us) ?
				(state->btr.mid_point_in_us - frame_time_in_us):
				(frame_time_in_us - state->btr.mid_point_in_us);
		}

		/* Choose number of frames to insert based on how close it
		 * can get to the mid point of the variable range.
		 */
		if (delta_from_mid_point_in_us_1 < delta_from_mid_point_in_us_2)
			frames_to_insert = mid_point_frames_ceil;
		else
			frames_to_insert = mid_point_frames_floor;

		/* Either we've calculated the number of frames to insert,
		 * or we need to insert min duration frames
		 */
		if (frames_to_insert > 0)
			inserted_frame_duration_in_us = last_render_time_in_us /
							frames_to_insert;

		if (inserted_frame_duration_in_us <
			state->time.min_render_time_in_us)

			inserted_frame_duration_in_us =
				state->time.min_render_time_in_us;

		/* Cache the calculated variables */
		state->btr.inserted_frame_duration_in_us =
			inserted_frame_duration_in_us;
		state->btr.frames_to_insert = frames_to_insert;
		state->btr.frame_counter = frames_to_insert;

	}
}

static void apply_fixed_refresh(struct core_freesync *core_freesync,
		struct dc_stream_state *stream, unsigned int map_index)
{
	unsigned int vmin = 0, vmax = 0;
	struct freesync_state *state = &core_freesync->map[map_index].state;

	if (!state->fixed_refresh.program_fixed)
		return;

	state->fixed_refresh.program_fixed = false;

	/* Program Fixed Refresh */

	/* Fixed Refresh set to "not active" so disengage */
	if (!state->fixed_refresh.fixed_active) {
		set_freesync_on_streams(core_freesync, &stream, 1);

	/* Fixed Refresh set to "active" so engage (fix to max) */
	} else {

		vmin = state->freesync_range.vmin;
		vmax = vmin;
		adjust_vmin_vmax(core_freesync, &stream, map_index,
					1, vmin, vmax);
	}
}

void mod_freesync_pre_update_plane_addresses(struct mod_freesync *mod_freesync,
		struct dc_stream_state **streams, int num_streams,
		unsigned int curr_time_stamp_in_us)
{
	unsigned int stream_index, map_index, last_render_time_in_us = 0;
	struct core_freesync *core_freesync = NULL;

	if (mod_freesync == NULL)
		return;

	core_freesync = MOD_FREESYNC_TO_CORE(mod_freesync);

	for (stream_index = 0; stream_index < num_streams; stream_index++) {

		map_index = map_index_from_stream(core_freesync,
						streams[stream_index]);

		if (core_freesync->map[map_index].caps->supported) {

			last_render_time_in_us = curr_time_stamp_in_us -
					core_freesync->map[map_index].state.time.
					prev_time_stamp_in_us;

			/* Add the timestamps to the cache and determine
			 * whether BTR program is required
			 */
			update_timestamps(core_freesync, streams[stream_index],
					map_index, last_render_time_in_us);

			if (core_freesync->map[map_index].state.fullscreen &&
				core_freesync->map[map_index].user_enable.
				enable_for_gaming) {

				if (core_freesync->map[map_index].caps->btr_supported) {

					apply_below_the_range(core_freesync,
						streams[stream_index], map_index,
						last_render_time_in_us);
				} else {
					apply_fixed_refresh(core_freesync,
						streams[stream_index], map_index);
				}
			}

			core_freesync->map[map_index].state.time.
				prev_time_stamp_in_us = curr_time_stamp_in_us;
		}

=======
	/* Calculate nominal field rate for stream */
	nominal_field_rate_in_uhz =
			mod_freesync_calc_nominal_field_rate(stream);

	min_refresh_in_uhz = in_config->min_refresh_in_uhz;
	max_refresh_in_uhz = in_config->max_refresh_in_uhz;

	/* Full range may be larger than current video timing, so cap at nominal */
	if (max_refresh_in_uhz > nominal_field_rate_in_uhz)
		max_refresh_in_uhz = nominal_field_rate_in_uhz;

	/* Full range may be larger than current video timing, so cap at nominal */
	if (min_refresh_in_uhz > max_refresh_in_uhz)
		min_refresh_in_uhz = max_refresh_in_uhz;

	/* If a monitor reports exactly max refresh of 2x of min, enforce it on nominal */
	rounded_nominal_in_uhz =
			div_u64(nominal_field_rate_in_uhz + 50000, 100000) * 100000;
	if (in_config->max_refresh_in_uhz == (2 * in_config->min_refresh_in_uhz) &&
		in_config->max_refresh_in_uhz == rounded_nominal_in_uhz)
		min_refresh_in_uhz = div_u64(nominal_field_rate_in_uhz, 2);

	if (!vrr_settings_require_update(core_freesync,
			in_config, (unsigned int)min_refresh_in_uhz, (unsigned int)max_refresh_in_uhz,
			in_out_vrr))
		return;

	in_out_vrr->state = in_config->state;
	in_out_vrr->send_info_frame = in_config->vsif_supported;

	if (in_config->state == VRR_STATE_UNSUPPORTED) {
		in_out_vrr->state = VRR_STATE_UNSUPPORTED;
		in_out_vrr->supported = false;
		in_out_vrr->adjust.v_total_min = stream->timing.v_total;
		in_out_vrr->adjust.v_total_max = stream->timing.v_total;

		return;

	} else {
		in_out_vrr->min_refresh_in_uhz = (unsigned int)min_refresh_in_uhz;
		in_out_vrr->max_duration_in_us =
				calc_duration_in_us_from_refresh_in_uhz(
						(unsigned int)min_refresh_in_uhz);

		in_out_vrr->max_refresh_in_uhz = (unsigned int)max_refresh_in_uhz;
		in_out_vrr->min_duration_in_us =
				calc_duration_in_us_from_refresh_in_uhz(
						(unsigned int)max_refresh_in_uhz);

		if (in_config->state == VRR_STATE_ACTIVE_FIXED)
			in_out_vrr->fixed_refresh_in_uhz = in_config->fixed_refresh_in_uhz;
		else
			in_out_vrr->fixed_refresh_in_uhz = 0;

		refresh_range = div_u64(in_out_vrr->max_refresh_in_uhz + 500000, 1000000) -
+				div_u64(in_out_vrr->min_refresh_in_uhz + 500000, 1000000);

		in_out_vrr->supported = true;
	}

	in_out_vrr->fixed.ramping_active = in_config->ramping;

	in_out_vrr->btr.btr_enabled = in_config->btr;

	if (in_out_vrr->max_refresh_in_uhz < (2 * in_out_vrr->min_refresh_in_uhz))
		in_out_vrr->btr.btr_enabled = false;
	else {
		in_out_vrr->btr.margin_in_us = in_out_vrr->max_duration_in_us -
				2 * in_out_vrr->min_duration_in_us;
		if (in_out_vrr->btr.margin_in_us > BTR_MAX_MARGIN)
			in_out_vrr->btr.margin_in_us = BTR_MAX_MARGIN;
	}

	in_out_vrr->btr.btr_active = false;
	in_out_vrr->btr.inserted_duration_in_us = 0;
	in_out_vrr->btr.frames_to_insert = 0;
	in_out_vrr->btr.frame_counter = 0;
	in_out_vrr->fixed.fixed_active = false;
	in_out_vrr->fixed.target_refresh_in_uhz = 0;

	in_out_vrr->btr.mid_point_in_us =
				(in_out_vrr->min_duration_in_us +
				 in_out_vrr->max_duration_in_us) / 2;

	if (in_out_vrr->state == VRR_STATE_UNSUPPORTED) {
		in_out_vrr->adjust.v_total_min = stream->timing.v_total;
		in_out_vrr->adjust.v_total_max = stream->timing.v_total;
	} else if (in_out_vrr->state == VRR_STATE_DISABLED) {
		in_out_vrr->adjust.v_total_min = stream->timing.v_total;
		in_out_vrr->adjust.v_total_max = stream->timing.v_total;
	} else if (in_out_vrr->state == VRR_STATE_INACTIVE) {
		in_out_vrr->adjust.v_total_min = stream->timing.v_total;
		in_out_vrr->adjust.v_total_max = stream->timing.v_total;
	} else if (in_out_vrr->state == VRR_STATE_ACTIVE_VARIABLE &&
			refresh_range >= MIN_REFRESH_RANGE) {

		in_out_vrr->adjust.v_total_min =
			mod_freesync_calc_v_total_from_refresh(stream,
				in_out_vrr->max_refresh_in_uhz);
		in_out_vrr->adjust.v_total_max =
			mod_freesync_calc_v_total_from_refresh(stream,
				in_out_vrr->min_refresh_in_uhz);
	} else if (in_out_vrr->state == VRR_STATE_ACTIVE_FIXED) {
		in_out_vrr->fixed.target_refresh_in_uhz =
				in_out_vrr->fixed_refresh_in_uhz;
		if (in_out_vrr->fixed.ramping_active &&
				in_out_vrr->fixed.fixed_active) {
			/* Do not update vtotals if ramping is already active
			 * in order to continue ramp from current refresh.
			 */
			in_out_vrr->fixed.fixed_active = true;
		} else {
			in_out_vrr->fixed.fixed_active = true;
			in_out_vrr->adjust.v_total_min =
				mod_freesync_calc_v_total_from_refresh(stream,
					in_out_vrr->fixed.target_refresh_in_uhz);
			in_out_vrr->adjust.v_total_max =
				in_out_vrr->adjust.v_total_min;
		}
	} else {
		in_out_vrr->state = VRR_STATE_INACTIVE;
		in_out_vrr->adjust.v_total_min = stream->timing.v_total;
		in_out_vrr->adjust.v_total_max = stream->timing.v_total;
	}
}

void mod_freesync_handle_preflip(struct mod_freesync *mod_freesync,
		const struct dc_plane_state *plane,
		const struct dc_stream_state *stream,
		unsigned int curr_time_stamp_in_us,
		struct mod_vrr_params *in_out_vrr)
{
	struct core_freesync *core_freesync = NULL;
	unsigned int last_render_time_in_us = 0;
	unsigned int average_render_time_in_us = 0;

	if (mod_freesync == NULL)
		return;

	core_freesync = MOD_FREESYNC_TO_CORE(mod_freesync);

	if (in_out_vrr->supported &&
			in_out_vrr->state == VRR_STATE_ACTIVE_VARIABLE) {
		unsigned int i = 0;
		unsigned int oldest_index = plane->time.index + 1;

		if (oldest_index >= DC_PLANE_UPDATE_TIMES_MAX)
			oldest_index = 0;

		last_render_time_in_us = curr_time_stamp_in_us -
				plane->time.prev_update_time_in_us;

		/* Sum off all entries except oldest one */
		for (i = 0; i < DC_PLANE_UPDATE_TIMES_MAX; i++) {
			average_render_time_in_us +=
					plane->time.time_elapsed_in_us[i];
		}
		average_render_time_in_us -=
				plane->time.time_elapsed_in_us[oldest_index];

		/* Add render time for current flip */
		average_render_time_in_us += last_render_time_in_us;
		average_render_time_in_us /= DC_PLANE_UPDATE_TIMES_MAX;

		if (in_out_vrr->btr.btr_enabled) {
			apply_below_the_range(core_freesync,
					stream,
					last_render_time_in_us,
					in_out_vrr);
		} else {
			apply_fixed_refresh(core_freesync,
				stream,
				last_render_time_in_us,
				in_out_vrr);
		}

	}
}

void mod_freesync_handle_v_update(struct mod_freesync *mod_freesync,
		const struct dc_stream_state *stream,
		struct mod_vrr_params *in_out_vrr)
{
	struct core_freesync *core_freesync = NULL;

	if ((mod_freesync == NULL) || (stream == NULL) || (in_out_vrr == NULL))
		return;

	core_freesync = MOD_FREESYNC_TO_CORE(mod_freesync);

	if (in_out_vrr->supported == false)
		return;

	/* Below the Range Logic */

	/* Only execute if in fullscreen mode */
	if (in_out_vrr->state == VRR_STATE_ACTIVE_VARIABLE &&
					in_out_vrr->btr.btr_active) {
		/* TODO: pass in flag for Pre-DCE12 ASIC
		 * in order for frame variable duration to take affect,
		 * it needs to be done one VSYNC early, which is at
		 * frameCounter == 1.
		 * For DCE12 and newer updates to V_TOTAL_MIN/MAX
		 * will take affect on current frame
		 */
		if (in_out_vrr->btr.frames_to_insert ==
				in_out_vrr->btr.frame_counter) {
			in_out_vrr->adjust.v_total_min =
				calc_v_total_from_duration(stream,
				in_out_vrr,
				in_out_vrr->btr.inserted_duration_in_us);
			in_out_vrr->adjust.v_total_max =
				in_out_vrr->adjust.v_total_min;
		}

		if (in_out_vrr->btr.frame_counter > 0)
			in_out_vrr->btr.frame_counter--;

		/* Restore FreeSync */
		if (in_out_vrr->btr.frame_counter == 0) {
			in_out_vrr->adjust.v_total_min =
				mod_freesync_calc_v_total_from_refresh(stream,
				in_out_vrr->max_refresh_in_uhz);
			in_out_vrr->adjust.v_total_max =
				mod_freesync_calc_v_total_from_refresh(stream,
				in_out_vrr->min_refresh_in_uhz);
		}
	}

	/* If in fullscreen freesync mode or in video, do not program
	 * static screen ramp values
	 */
	if (in_out_vrr->state == VRR_STATE_ACTIVE_VARIABLE)
		in_out_vrr->fixed.ramping_active = false;

	/* Gradual Static Screen Ramping Logic
	 * Execute if ramp is active and user enabled freesync static screen
	 */
	if (in_out_vrr->state == VRR_STATE_ACTIVE_FIXED &&
				in_out_vrr->fixed.ramping_active) {
		update_v_total_for_static_ramp(
				core_freesync, stream, in_out_vrr);
>>>>>>> upstream/android-13
	}
}

void mod_freesync_get_settings(struct mod_freesync *mod_freesync,
<<<<<<< HEAD
		struct dc_stream_state **streams, int num_streams,
=======
		const struct mod_vrr_params *vrr,
>>>>>>> upstream/android-13
		unsigned int *v_total_min, unsigned int *v_total_max,
		unsigned int *event_triggers,
		unsigned int *window_min, unsigned int *window_max,
		unsigned int *lfc_mid_point_in_us,
		unsigned int *inserted_frames,
		unsigned int *inserted_duration_in_us)
{
<<<<<<< HEAD
	unsigned int stream_index, map_index;
	struct core_freesync *core_freesync = NULL;

	if (mod_freesync == NULL)
		return;

	core_freesync = MOD_FREESYNC_TO_CORE(mod_freesync);

	for (stream_index = 0; stream_index < num_streams; stream_index++) {

		map_index = map_index_from_stream(core_freesync,
						streams[stream_index]);

		if (core_freesync->map[map_index].caps->supported) {
			struct freesync_state state =
					core_freesync->map[map_index].state;
			*v_total_min = state.vmin;
			*v_total_max = state.vmax;
			*event_triggers = 0;
			*window_min = state.time.min_window;
			*window_max = state.time.max_window;
			*lfc_mid_point_in_us = state.btr.mid_point_in_us;
			*inserted_frames = state.btr.frames_to_insert;
			*inserted_duration_in_us =
					state.btr.inserted_frame_duration_in_us;
		}

	}
}

=======
	if (mod_freesync == NULL)
		return;

	if (vrr->supported) {
		*v_total_min = vrr->adjust.v_total_min;
		*v_total_max = vrr->adjust.v_total_max;
		*event_triggers = 0;
		*lfc_mid_point_in_us = vrr->btr.mid_point_in_us;
		*inserted_frames = vrr->btr.frames_to_insert;
		*inserted_duration_in_us = vrr->btr.inserted_duration_in_us;
	}
}

unsigned long long mod_freesync_calc_nominal_field_rate(
			const struct dc_stream_state *stream)
{
	unsigned long long nominal_field_rate_in_uhz = 0;
	unsigned int total = stream->timing.h_total * stream->timing.v_total;

	/* Calculate nominal field rate for stream, rounded up to nearest integer */
	nominal_field_rate_in_uhz = stream->timing.pix_clk_100hz;
	nominal_field_rate_in_uhz *= 100000000ULL;

	nominal_field_rate_in_uhz =	div_u64(nominal_field_rate_in_uhz, total);

	return nominal_field_rate_in_uhz;
}

unsigned long long mod_freesync_calc_field_rate_from_timing(
		unsigned int vtotal, unsigned int htotal, unsigned int pix_clk)
{
	unsigned long long field_rate_in_uhz = 0;
	unsigned int total = htotal * vtotal;

	/* Calculate nominal field rate for stream, rounded up to nearest integer */
	field_rate_in_uhz = pix_clk;
	field_rate_in_uhz *= 1000000ULL;

	field_rate_in_uhz =	div_u64(field_rate_in_uhz, total);

	return field_rate_in_uhz;
}

bool mod_freesync_is_valid_range(uint32_t min_refresh_cap_in_uhz,
		uint32_t max_refresh_cap_in_uhz,
		uint32_t nominal_field_rate_in_uhz) 
{

	/* Typically nominal refresh calculated can have some fractional part.
	 * Allow for some rounding error of actual video timing by taking floor
	 * of caps and request. Round the nominal refresh rate.
	 *
	 * Dividing will convert everything to units in Hz although input
	 * variable name is in uHz!
	 *
	 * Also note, this takes care of rounding error on the nominal refresh
	 * so by rounding error we only expect it to be off by a small amount,
	 * such as < 0.1 Hz. i.e. 143.9xxx or 144.1xxx.
	 *
	 * Example 1. Caps    Min = 40 Hz, Max = 144 Hz
	 *            Request Min = 40 Hz, Max = 144 Hz
	 *                    Nominal = 143.5x Hz rounded to 144 Hz
	 *            This function should allow this as valid request
	 *
	 * Example 2. Caps    Min = 40 Hz, Max = 144 Hz
	 *            Request Min = 40 Hz, Max = 144 Hz
	 *                    Nominal = 144.4x Hz rounded to 144 Hz
	 *            This function should allow this as valid request
	 *
	 * Example 3. Caps    Min = 40 Hz, Max = 144 Hz
	 *            Request Min = 40 Hz, Max = 144 Hz
	 *                    Nominal = 120.xx Hz rounded to 120 Hz
	 *            This function should return NOT valid since the requested
	 *            max is greater than current timing's nominal
	 *
	 * Example 4. Caps    Min = 40 Hz, Max = 120 Hz
	 *            Request Min = 40 Hz, Max = 120 Hz
	 *                    Nominal = 144.xx Hz rounded to 144 Hz
	 *            This function should return NOT valid since the nominal
	 *            is greater than the capability's max refresh
	 */
	nominal_field_rate_in_uhz =
			div_u64(nominal_field_rate_in_uhz + 500000, 1000000);
	min_refresh_cap_in_uhz /= 1000000;
	max_refresh_cap_in_uhz /= 1000000;

	/* Check nominal is within range */
	if (nominal_field_rate_in_uhz > max_refresh_cap_in_uhz ||
		nominal_field_rate_in_uhz < min_refresh_cap_in_uhz)
		return false;

	/* If nominal is less than max, limit the max allowed refresh rate */
	if (nominal_field_rate_in_uhz < max_refresh_cap_in_uhz)
		max_refresh_cap_in_uhz = nominal_field_rate_in_uhz;

	/* Check min is within range */
	if (min_refresh_cap_in_uhz > max_refresh_cap_in_uhz)
		return false;

	/* For variable range, check for at least 10 Hz range */
	if (nominal_field_rate_in_uhz - min_refresh_cap_in_uhz < 10)
		return false;

	return true;
}
>>>>>>> upstream/android-13
