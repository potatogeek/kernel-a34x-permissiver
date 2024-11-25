// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (C) 2018 Linaro Ltd.
 *
 * Author: Stanimir Varbanov <stanimir.varbanov@linaro.org>
 */
#include <linux/bitops.h>
#include <linux/kernel.h>

#include "core.h"
#include "hfi_helper.h"
#include "hfi_parser.h"

<<<<<<< HEAD
typedef void (*func)(struct venus_caps *cap, const void *data,
=======
typedef void (*func)(struct hfi_plat_caps *cap, const void *data,
>>>>>>> upstream/android-13
		     unsigned int size);

static void init_codecs(struct venus_core *core)
{
<<<<<<< HEAD
	struct venus_caps *caps = core->caps, *cap;
=======
	struct hfi_plat_caps *caps = core->caps, *cap;
>>>>>>> upstream/android-13
	unsigned long bit;

	for_each_set_bit(bit, &core->dec_codecs, MAX_CODEC_NUM) {
		cap = &caps[core->codecs_count++];
		cap->codec = BIT(bit);
		cap->domain = VIDC_SESSION_TYPE_DEC;
		cap->valid = false;
	}

	for_each_set_bit(bit, &core->enc_codecs, MAX_CODEC_NUM) {
		cap = &caps[core->codecs_count++];
		cap->codec = BIT(bit);
		cap->domain = VIDC_SESSION_TYPE_ENC;
		cap->valid = false;
	}
}

<<<<<<< HEAD
static void for_each_codec(struct venus_caps *caps, unsigned int caps_num,
			   u32 codecs, u32 domain, func cb, void *data,
			   unsigned int size)
{
	struct venus_caps *cap;
=======
static void for_each_codec(struct hfi_plat_caps *caps, unsigned int caps_num,
			   u32 codecs, u32 domain, func cb, void *data,
			   unsigned int size)
{
	struct hfi_plat_caps *cap;
>>>>>>> upstream/android-13
	unsigned int i;

	for (i = 0; i < caps_num; i++) {
		cap = &caps[i];
		if (cap->valid && cap->domain == domain)
			continue;
		if (cap->codec & codecs && cap->domain == domain)
			cb(cap, data, size);
	}
}

static void
<<<<<<< HEAD
fill_buf_mode(struct venus_caps *cap, const void *data, unsigned int num)
=======
fill_buf_mode(struct hfi_plat_caps *cap, const void *data, unsigned int num)
>>>>>>> upstream/android-13
{
	const u32 *type = data;

	if (*type == HFI_BUFFER_MODE_DYNAMIC)
		cap->cap_bufs_mode_dynamic = true;
}

static void
parse_alloc_mode(struct venus_core *core, u32 codecs, u32 domain, void *data)
{
	struct hfi_buffer_alloc_mode_supported *mode = data;
	u32 num_entries = mode->num_entries;
	u32 *type;

	if (num_entries > MAX_ALLOC_MODE_ENTRIES)
		return;

	type = mode->data;

	while (num_entries--) {
		if (mode->buffer_type == HFI_BUFFER_OUTPUT ||
		    mode->buffer_type == HFI_BUFFER_OUTPUT2)
			for_each_codec(core->caps, ARRAY_SIZE(core->caps),
				       codecs, domain, fill_buf_mode, type, 1);

		type++;
	}
}

<<<<<<< HEAD
static void fill_profile_level(struct venus_caps *cap, const void *data,
=======
static void fill_profile_level(struct hfi_plat_caps *cap, const void *data,
>>>>>>> upstream/android-13
			       unsigned int num)
{
	const struct hfi_profile_level *pl = data;

	memcpy(&cap->pl[cap->num_pl], pl, num * sizeof(*pl));
	cap->num_pl += num;
}

static void
parse_profile_level(struct venus_core *core, u32 codecs, u32 domain, void *data)
{
	struct hfi_profile_level_supported *pl = data;
	struct hfi_profile_level *proflevel = pl->profile_level;
	struct hfi_profile_level pl_arr[HFI_MAX_PROFILE_COUNT] = {};

	if (pl->profile_count > HFI_MAX_PROFILE_COUNT)
		return;

	memcpy(pl_arr, proflevel, pl->profile_count * sizeof(*proflevel));

	for_each_codec(core->caps, ARRAY_SIZE(core->caps), codecs, domain,
		       fill_profile_level, pl_arr, pl->profile_count);
}

static void
<<<<<<< HEAD
fill_caps(struct venus_caps *cap, const void *data, unsigned int num)
=======
fill_caps(struct hfi_plat_caps *cap, const void *data, unsigned int num)
>>>>>>> upstream/android-13
{
	const struct hfi_capability *caps = data;

	memcpy(&cap->caps[cap->num_caps], caps, num * sizeof(*caps));
	cap->num_caps += num;
}

static void
parse_caps(struct venus_core *core, u32 codecs, u32 domain, void *data)
{
	struct hfi_capabilities *caps = data;
	struct hfi_capability *cap = caps->data;
	u32 num_caps = caps->num_capabilities;
	struct hfi_capability caps_arr[MAX_CAP_ENTRIES] = {};

	if (num_caps > MAX_CAP_ENTRIES)
		return;

	memcpy(caps_arr, cap, num_caps * sizeof(*cap));

	for_each_codec(core->caps, ARRAY_SIZE(core->caps), codecs, domain,
		       fill_caps, caps_arr, num_caps);
}

<<<<<<< HEAD
static void fill_raw_fmts(struct venus_caps *cap, const void *fmts,
=======
static void fill_raw_fmts(struct hfi_plat_caps *cap, const void *fmts,
>>>>>>> upstream/android-13
			  unsigned int num_fmts)
{
	const struct raw_formats *formats = fmts;

	memcpy(&cap->fmts[cap->num_fmts], formats, num_fmts * sizeof(*formats));
	cap->num_fmts += num_fmts;
}

static void
parse_raw_formats(struct venus_core *core, u32 codecs, u32 domain, void *data)
{
	struct hfi_uncompressed_format_supported *fmt = data;
	struct hfi_uncompressed_plane_info *pinfo = fmt->plane_info;
	struct hfi_uncompressed_plane_constraints *constr;
	struct raw_formats rawfmts[MAX_FMT_ENTRIES] = {};
	u32 entries = fmt->format_entries;
	unsigned int i = 0;
	u32 num_planes;

	while (entries) {
		num_planes = pinfo->num_planes;

		rawfmts[i].fmt = pinfo->format;
		rawfmts[i].buftype = fmt->buffer_type;
		i++;

		if (pinfo->num_planes > MAX_PLANES)
			break;

		pinfo = (void *)pinfo + sizeof(*constr) * num_planes +
			2 * sizeof(u32);
		entries--;
	}

	for_each_codec(core->caps, ARRAY_SIZE(core->caps), codecs, domain,
		       fill_raw_fmts, rawfmts, i);
}

static void parse_codecs(struct venus_core *core, void *data)
{
	struct hfi_codec_supported *codecs = data;

	core->dec_codecs = codecs->dec_codecs;
	core->enc_codecs = codecs->enc_codecs;

	if (IS_V1(core)) {
		core->dec_codecs &= ~HFI_VIDEO_CODEC_HEVC;
		core->dec_codecs &= ~HFI_VIDEO_CODEC_SPARK;
		core->enc_codecs &= ~HFI_VIDEO_CODEC_HEVC;
	}
}

static void parse_max_sessions(struct venus_core *core, const void *data)
{
	const struct hfi_max_sessions_supported *sessions = data;

	core->max_sessions_supported = sessions->max_sessions;
}

static void parse_codecs_mask(u32 *codecs, u32 *domain, void *data)
{
	struct hfi_codec_mask_supported *mask = data;

	*codecs = mask->codecs;
	*domain = mask->video_domains;
}

static void parser_init(struct venus_inst *inst, u32 *codecs, u32 *domain)
{
	if (!inst || !IS_V1(inst->core))
		return;

	*codecs = inst->hfi_codec;
	*domain = inst->session_type;
}

static void parser_fini(struct venus_inst *inst, u32 codecs, u32 domain)
{
<<<<<<< HEAD
	struct venus_caps *caps, *cap;
=======
	struct hfi_plat_caps *caps, *cap;
>>>>>>> upstream/android-13
	unsigned int i;
	u32 dom;

	if (!inst || !IS_V1(inst->core))
		return;

	caps = inst->core->caps;
	dom = inst->session_type;

	for (i = 0; i < MAX_CODEC_NUM; i++) {
		cap = &caps[i];
		if (cap->codec & codecs && cap->domain == dom)
			cap->valid = true;
	}
}

<<<<<<< HEAD
=======
static int hfi_platform_parser(struct venus_core *core, struct venus_inst *inst)
{
	const struct hfi_platform *plat;
	const struct hfi_plat_caps *caps = NULL;
	u32 enc_codecs, dec_codecs, count = 0;
	unsigned int entries;

	plat = hfi_platform_get(core->res->hfi_version);
	if (!plat)
		return -EINVAL;

	if (inst)
		return 0;

	if (plat->codecs)
		plat->codecs(&enc_codecs, &dec_codecs, &count);

	if (plat->capabilities)
		caps = plat->capabilities(&entries);

	if (!caps || !entries || !count)
		return -EINVAL;

	core->enc_codecs = enc_codecs;
	core->dec_codecs = dec_codecs;
	core->codecs_count = count;
	core->max_sessions_supported = MAX_SESSIONS;
	memset(core->caps, 0, sizeof(*caps) * MAX_CODEC_NUM);
	memcpy(core->caps, caps, sizeof(*caps) * entries);

	return 0;
}

>>>>>>> upstream/android-13
u32 hfi_parser(struct venus_core *core, struct venus_inst *inst, void *buf,
	       u32 size)
{
	unsigned int words_count = size >> 2;
	u32 *word = buf, *data, codecs = 0, domain = 0;
<<<<<<< HEAD
=======
	int ret;

	ret = hfi_platform_parser(core, inst);
	if (!ret)
		return HFI_ERR_NONE;
>>>>>>> upstream/android-13

	if (size % 4)
		return HFI_ERR_SYS_INSUFFICIENT_RESOURCES;

	parser_init(inst, &codecs, &domain);

<<<<<<< HEAD
=======
	if (core->res->hfi_version > HFI_VERSION_1XX) {
		core->codecs_count = 0;
		memset(core->caps, 0, sizeof(core->caps));
	}

>>>>>>> upstream/android-13
	while (words_count) {
		data = word + 1;

		switch (*word) {
		case HFI_PROPERTY_PARAM_CODEC_SUPPORTED:
			parse_codecs(core, data);
			init_codecs(core);
			break;
		case HFI_PROPERTY_PARAM_MAX_SESSIONS_SUPPORTED:
			parse_max_sessions(core, data);
			break;
		case HFI_PROPERTY_PARAM_CODEC_MASK_SUPPORTED:
			parse_codecs_mask(&codecs, &domain, data);
			break;
		case HFI_PROPERTY_PARAM_UNCOMPRESSED_FORMAT_SUPPORTED:
			parse_raw_formats(core, codecs, domain, data);
			break;
		case HFI_PROPERTY_PARAM_CAPABILITY_SUPPORTED:
			parse_caps(core, codecs, domain, data);
			break;
		case HFI_PROPERTY_PARAM_PROFILE_LEVEL_SUPPORTED:
			parse_profile_level(core, codecs, domain, data);
			break;
		case HFI_PROPERTY_PARAM_BUFFER_ALLOC_MODE_SUPPORTED:
			parse_alloc_mode(core, codecs, domain, data);
			break;
		default:
			break;
		}

		word++;
		words_count--;
	}

<<<<<<< HEAD
=======
	if (!core->max_sessions_supported)
		core->max_sessions_supported = MAX_SESSIONS;

>>>>>>> upstream/android-13
	parser_fini(inst, codecs, domain);

	return HFI_ERR_NONE;
}
