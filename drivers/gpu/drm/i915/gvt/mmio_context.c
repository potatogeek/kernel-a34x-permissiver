/*
 * Copyright(c) 2011-2016 Intel Corporation. All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the next
 * paragraph) shall be included in all copies or substantial portions of the
 * Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * Authors:
 *    Eddie Dong <eddie.dong@intel.com>
 *    Kevin Tian <kevin.tian@intel.com>
 *
 * Contributors:
 *    Zhi Wang <zhi.a.wang@intel.com>
 *    Changbin Du <changbin.du@intel.com>
 *    Zhenyu Wang <zhenyuw@linux.intel.com>
 *    Tina Zhang <tina.zhang@intel.com>
 *    Bing Niu <bing.niu@intel.com>
 *
 */

#include "i915_drv.h"
<<<<<<< HEAD
#include "gvt.h"
#include "trace.h"

/**
 * Defined in Intel Open Source PRM.
 * Ref: https://01.org/linuxgraphics/documentation/hardware-specification-prms
 */
#define TRVATTL3PTRDW(i)	_MMIO(0x4de0 + (i)*4)
#define TRNULLDETCT		_MMIO(0x4de8)
#define TRINVTILEDETCT		_MMIO(0x4dec)
#define TRVADR			_MMIO(0x4df0)
#define TRTTE			_MMIO(0x4df4)
#define RING_EXCC(base)		_MMIO((base) + 0x28)
#define RING_GFX_MODE(base)	_MMIO((base) + 0x29c)
#define VF_GUARDBAND		_MMIO(0x83a4)

=======
#include "gt/intel_context.h"
#include "gt/intel_gpu_commands.h"
#include "gt/intel_ring.h"
#include "gvt.h"
#include "trace.h"

>>>>>>> upstream/android-13
#define GEN9_MOCS_SIZE		64

/* Raw offset is appened to each line for convenience. */
static struct engine_mmio gen8_engine_mmio_list[] __cacheline_aligned = {
<<<<<<< HEAD
	{RCS, GFX_MODE_GEN7, 0xffff, false}, /* 0x229c */
	{RCS, GEN9_CTX_PREEMPT_REG, 0x0, false}, /* 0x2248 */
	{RCS, HWSTAM, 0x0, false}, /* 0x2098 */
	{RCS, INSTPM, 0xffff, true}, /* 0x20c0 */
	{RCS, RING_FORCE_TO_NONPRIV(RENDER_RING_BASE, 0), 0, false}, /* 0x24d0 */
	{RCS, RING_FORCE_TO_NONPRIV(RENDER_RING_BASE, 1), 0, false}, /* 0x24d4 */
	{RCS, RING_FORCE_TO_NONPRIV(RENDER_RING_BASE, 2), 0, false}, /* 0x24d8 */
	{RCS, RING_FORCE_TO_NONPRIV(RENDER_RING_BASE, 3), 0, false}, /* 0x24dc */
	{RCS, RING_FORCE_TO_NONPRIV(RENDER_RING_BASE, 4), 0, false}, /* 0x24e0 */
	{RCS, RING_FORCE_TO_NONPRIV(RENDER_RING_BASE, 5), 0, false}, /* 0x24e4 */
	{RCS, RING_FORCE_TO_NONPRIV(RENDER_RING_BASE, 6), 0, false}, /* 0x24e8 */
	{RCS, RING_FORCE_TO_NONPRIV(RENDER_RING_BASE, 7), 0, false}, /* 0x24ec */
	{RCS, RING_FORCE_TO_NONPRIV(RENDER_RING_BASE, 8), 0, false}, /* 0x24f0 */
	{RCS, RING_FORCE_TO_NONPRIV(RENDER_RING_BASE, 9), 0, false}, /* 0x24f4 */
	{RCS, RING_FORCE_TO_NONPRIV(RENDER_RING_BASE, 10), 0, false}, /* 0x24f8 */
	{RCS, RING_FORCE_TO_NONPRIV(RENDER_RING_BASE, 11), 0, false}, /* 0x24fc */
	{RCS, CACHE_MODE_1, 0xffff, true}, /* 0x7004 */
	{RCS, GEN7_GT_MODE, 0xffff, true}, /* 0x7008 */
	{RCS, CACHE_MODE_0_GEN7, 0xffff, true}, /* 0x7000 */
	{RCS, GEN7_COMMON_SLICE_CHICKEN1, 0xffff, true}, /* 0x7010 */
	{RCS, HDC_CHICKEN0, 0xffff, true}, /* 0x7300 */
	{RCS, VF_GUARDBAND, 0xffff, true}, /* 0x83a4 */

	{BCS, RING_GFX_MODE(BLT_RING_BASE), 0xffff, false}, /* 0x2229c */
	{BCS, RING_MI_MODE(BLT_RING_BASE), 0xffff, false}, /* 0x2209c */
	{BCS, RING_INSTPM(BLT_RING_BASE), 0xffff, false}, /* 0x220c0 */
	{BCS, RING_HWSTAM(BLT_RING_BASE), 0x0, false}, /* 0x22098 */
	{BCS, RING_EXCC(BLT_RING_BASE), 0x0, false}, /* 0x22028 */
	{RCS, INVALID_MMIO_REG, 0, false } /* Terminated */
};

static struct engine_mmio gen9_engine_mmio_list[] __cacheline_aligned = {
	{RCS, GFX_MODE_GEN7, 0xffff, false}, /* 0x229c */
	{RCS, GEN9_CTX_PREEMPT_REG, 0x0, false}, /* 0x2248 */
	{RCS, HWSTAM, 0x0, false}, /* 0x2098 */
	{RCS, INSTPM, 0xffff, true}, /* 0x20c0 */
	{RCS, RING_FORCE_TO_NONPRIV(RENDER_RING_BASE, 0), 0, false}, /* 0x24d0 */
	{RCS, RING_FORCE_TO_NONPRIV(RENDER_RING_BASE, 1), 0, false}, /* 0x24d4 */
	{RCS, RING_FORCE_TO_NONPRIV(RENDER_RING_BASE, 2), 0, false}, /* 0x24d8 */
	{RCS, RING_FORCE_TO_NONPRIV(RENDER_RING_BASE, 3), 0, false}, /* 0x24dc */
	{RCS, RING_FORCE_TO_NONPRIV(RENDER_RING_BASE, 4), 0, false}, /* 0x24e0 */
	{RCS, RING_FORCE_TO_NONPRIV(RENDER_RING_BASE, 5), 0, false}, /* 0x24e4 */
	{RCS, RING_FORCE_TO_NONPRIV(RENDER_RING_BASE, 6), 0, false}, /* 0x24e8 */
	{RCS, RING_FORCE_TO_NONPRIV(RENDER_RING_BASE, 7), 0, false}, /* 0x24ec */
	{RCS, RING_FORCE_TO_NONPRIV(RENDER_RING_BASE, 8), 0, false}, /* 0x24f0 */
	{RCS, RING_FORCE_TO_NONPRIV(RENDER_RING_BASE, 9), 0, false}, /* 0x24f4 */
	{RCS, RING_FORCE_TO_NONPRIV(RENDER_RING_BASE, 10), 0, false}, /* 0x24f8 */
	{RCS, RING_FORCE_TO_NONPRIV(RENDER_RING_BASE, 11), 0, false}, /* 0x24fc */
	{RCS, CACHE_MODE_1, 0xffff, true}, /* 0x7004 */
	{RCS, GEN7_GT_MODE, 0xffff, true}, /* 0x7008 */
	{RCS, CACHE_MODE_0_GEN7, 0xffff, true}, /* 0x7000 */
	{RCS, GEN7_COMMON_SLICE_CHICKEN1, 0xffff, true}, /* 0x7010 */
	{RCS, HDC_CHICKEN0, 0xffff, true}, /* 0x7300 */
	{RCS, VF_GUARDBAND, 0xffff, true}, /* 0x83a4 */

	{RCS, GEN8_PRIVATE_PAT_LO, 0, false}, /* 0x40e0 */
	{RCS, GEN8_PRIVATE_PAT_HI, 0, false}, /* 0x40e4 */
	{RCS, GEN8_CS_CHICKEN1, 0xffff, true}, /* 0x2580 */
	{RCS, COMMON_SLICE_CHICKEN2, 0xffff, true}, /* 0x7014 */
	{RCS, GEN9_CS_DEBUG_MODE1, 0xffff, false}, /* 0x20ec */
	{RCS, GEN8_L3SQCREG4, 0, false}, /* 0xb118 */
	{RCS, GEN7_HALF_SLICE_CHICKEN1, 0xffff, true}, /* 0xe100 */
	{RCS, HALF_SLICE_CHICKEN2, 0xffff, true}, /* 0xe180 */
	{RCS, HALF_SLICE_CHICKEN3, 0xffff, true}, /* 0xe184 */
	{RCS, GEN9_HALF_SLICE_CHICKEN5, 0xffff, true}, /* 0xe188 */
	{RCS, GEN9_HALF_SLICE_CHICKEN7, 0xffff, true}, /* 0xe194 */
	{RCS, GEN8_ROW_CHICKEN, 0xffff, true}, /* 0xe4f0 */
	{RCS, TRVATTL3PTRDW(0), 0, false}, /* 0x4de0 */
	{RCS, TRVATTL3PTRDW(1), 0, false}, /* 0x4de4 */
	{RCS, TRNULLDETCT, 0, false}, /* 0x4de8 */
	{RCS, TRINVTILEDETCT, 0, false}, /* 0x4dec */
	{RCS, TRVADR, 0, false}, /* 0x4df0 */
	{RCS, TRTTE, 0, false}, /* 0x4df4 */

	{BCS, RING_GFX_MODE(BLT_RING_BASE), 0xffff, false}, /* 0x2229c */
	{BCS, RING_MI_MODE(BLT_RING_BASE), 0xffff, false}, /* 0x2209c */
	{BCS, RING_INSTPM(BLT_RING_BASE), 0xffff, false}, /* 0x220c0 */
	{BCS, RING_HWSTAM(BLT_RING_BASE), 0x0, false}, /* 0x22098 */
	{BCS, RING_EXCC(BLT_RING_BASE), 0x0, false}, /* 0x22028 */

	{VCS2, RING_EXCC(GEN8_BSD2_RING_BASE), 0xffff, false}, /* 0x1c028 */

	{VECS, RING_EXCC(VEBOX_RING_BASE), 0xffff, false}, /* 0x1a028 */

	{RCS, GEN8_HDC_CHICKEN1, 0xffff, true}, /* 0x7304 */
	{RCS, GEN9_CTX_PREEMPT_REG, 0x0, false}, /* 0x2248 */
	{RCS, GEN7_UCGCTL4, 0x0, false}, /* 0x940c */
	{RCS, GAMT_CHKN_BIT_REG, 0x0, false}, /* 0x4ab8 */

	{RCS, GEN9_GAMT_ECO_REG_RW_IA, 0x0, false}, /* 0x4ab0 */
	{RCS, GEN9_CSFE_CHICKEN1_RCS, 0x0, false}, /* 0x20d4 */

	{RCS, GEN8_GARBCNTL, 0x0, false}, /* 0xb004 */
	{RCS, GEN7_FF_THREAD_MODE, 0x0, false}, /* 0x20a0 */
	{RCS, FF_SLICE_CS_CHICKEN2, 0xffff, false}, /* 0x20e4 */
	{RCS, INVALID_MMIO_REG, 0, false } /* Terminated */
=======
	{RCS0, GFX_MODE_GEN7, 0xffff, false}, /* 0x229c */
	{RCS0, GEN9_CTX_PREEMPT_REG, 0x0, false}, /* 0x2248 */
	{RCS0, HWSTAM, 0x0, false}, /* 0x2098 */
	{RCS0, INSTPM, 0xffff, true}, /* 0x20c0 */
	{RCS0, RING_FORCE_TO_NONPRIV(RENDER_RING_BASE, 0), 0, false}, /* 0x24d0 */
	{RCS0, RING_FORCE_TO_NONPRIV(RENDER_RING_BASE, 1), 0, false}, /* 0x24d4 */
	{RCS0, RING_FORCE_TO_NONPRIV(RENDER_RING_BASE, 2), 0, false}, /* 0x24d8 */
	{RCS0, RING_FORCE_TO_NONPRIV(RENDER_RING_BASE, 3), 0, false}, /* 0x24dc */
	{RCS0, RING_FORCE_TO_NONPRIV(RENDER_RING_BASE, 4), 0, false}, /* 0x24e0 */
	{RCS0, RING_FORCE_TO_NONPRIV(RENDER_RING_BASE, 5), 0, false}, /* 0x24e4 */
	{RCS0, RING_FORCE_TO_NONPRIV(RENDER_RING_BASE, 6), 0, false}, /* 0x24e8 */
	{RCS0, RING_FORCE_TO_NONPRIV(RENDER_RING_BASE, 7), 0, false}, /* 0x24ec */
	{RCS0, RING_FORCE_TO_NONPRIV(RENDER_RING_BASE, 8), 0, false}, /* 0x24f0 */
	{RCS0, RING_FORCE_TO_NONPRIV(RENDER_RING_BASE, 9), 0, false}, /* 0x24f4 */
	{RCS0, RING_FORCE_TO_NONPRIV(RENDER_RING_BASE, 10), 0, false}, /* 0x24f8 */
	{RCS0, RING_FORCE_TO_NONPRIV(RENDER_RING_BASE, 11), 0, false}, /* 0x24fc */
	{RCS0, CACHE_MODE_1, 0xffff, true}, /* 0x7004 */
	{RCS0, GEN7_GT_MODE, 0xffff, true}, /* 0x7008 */
	{RCS0, CACHE_MODE_0_GEN7, 0xffff, true}, /* 0x7000 */
	{RCS0, GEN7_COMMON_SLICE_CHICKEN1, 0xffff, true}, /* 0x7010 */
	{RCS0, HDC_CHICKEN0, 0xffff, true}, /* 0x7300 */
	{RCS0, VF_GUARDBAND, 0xffff, true}, /* 0x83a4 */

	{BCS0, RING_GFX_MODE(BLT_RING_BASE), 0xffff, false}, /* 0x2229c */
	{BCS0, RING_MI_MODE(BLT_RING_BASE), 0xffff, false}, /* 0x2209c */
	{BCS0, RING_INSTPM(BLT_RING_BASE), 0xffff, false}, /* 0x220c0 */
	{BCS0, RING_HWSTAM(BLT_RING_BASE), 0x0, false}, /* 0x22098 */
	{BCS0, RING_EXCC(BLT_RING_BASE), 0xffff, false}, /* 0x22028 */
	{RCS0, INVALID_MMIO_REG, 0, false } /* Terminated */
};

static struct engine_mmio gen9_engine_mmio_list[] __cacheline_aligned = {
	{RCS0, GFX_MODE_GEN7, 0xffff, false}, /* 0x229c */
	{RCS0, GEN9_CTX_PREEMPT_REG, 0x0, false}, /* 0x2248 */
	{RCS0, HWSTAM, 0x0, false}, /* 0x2098 */
	{RCS0, INSTPM, 0xffff, true}, /* 0x20c0 */
	{RCS0, RING_FORCE_TO_NONPRIV(RENDER_RING_BASE, 0), 0, false}, /* 0x24d0 */
	{RCS0, RING_FORCE_TO_NONPRIV(RENDER_RING_BASE, 1), 0, false}, /* 0x24d4 */
	{RCS0, RING_FORCE_TO_NONPRIV(RENDER_RING_BASE, 2), 0, false}, /* 0x24d8 */
	{RCS0, RING_FORCE_TO_NONPRIV(RENDER_RING_BASE, 3), 0, false}, /* 0x24dc */
	{RCS0, RING_FORCE_TO_NONPRIV(RENDER_RING_BASE, 4), 0, false}, /* 0x24e0 */
	{RCS0, RING_FORCE_TO_NONPRIV(RENDER_RING_BASE, 5), 0, false}, /* 0x24e4 */
	{RCS0, RING_FORCE_TO_NONPRIV(RENDER_RING_BASE, 6), 0, false}, /* 0x24e8 */
	{RCS0, RING_FORCE_TO_NONPRIV(RENDER_RING_BASE, 7), 0, false}, /* 0x24ec */
	{RCS0, RING_FORCE_TO_NONPRIV(RENDER_RING_BASE, 8), 0, false}, /* 0x24f0 */
	{RCS0, RING_FORCE_TO_NONPRIV(RENDER_RING_BASE, 9), 0, false}, /* 0x24f4 */
	{RCS0, RING_FORCE_TO_NONPRIV(RENDER_RING_BASE, 10), 0, false}, /* 0x24f8 */
	{RCS0, RING_FORCE_TO_NONPRIV(RENDER_RING_BASE, 11), 0, false}, /* 0x24fc */
	{RCS0, CACHE_MODE_1, 0xffff, true}, /* 0x7004 */
	{RCS0, GEN7_GT_MODE, 0xffff, true}, /* 0x7008 */
	{RCS0, CACHE_MODE_0_GEN7, 0xffff, true}, /* 0x7000 */
	{RCS0, GEN7_COMMON_SLICE_CHICKEN1, 0xffff, true}, /* 0x7010 */
	{RCS0, HDC_CHICKEN0, 0xffff, true}, /* 0x7300 */
	{RCS0, VF_GUARDBAND, 0xffff, true}, /* 0x83a4 */

	{RCS0, GEN8_PRIVATE_PAT_LO, 0, false}, /* 0x40e0 */
	{RCS0, GEN8_PRIVATE_PAT_HI, 0, false}, /* 0x40e4 */
	{RCS0, GEN8_CS_CHICKEN1, 0xffff, true}, /* 0x2580 */
	{RCS0, COMMON_SLICE_CHICKEN2, 0xffff, true}, /* 0x7014 */
	{RCS0, GEN9_CS_DEBUG_MODE1, 0xffff, false}, /* 0x20ec */
	{RCS0, GEN8_L3SQCREG4, 0, false}, /* 0xb118 */
	{RCS0, GEN9_SCRATCH1, 0, false}, /* 0xb11c */
	{RCS0, GEN9_SCRATCH_LNCF1, 0, false}, /* 0xb008 */
	{RCS0, GEN7_HALF_SLICE_CHICKEN1, 0xffff, true}, /* 0xe100 */
	{RCS0, HALF_SLICE_CHICKEN2, 0xffff, true}, /* 0xe180 */
	{RCS0, HALF_SLICE_CHICKEN3, 0xffff, true}, /* 0xe184 */
	{RCS0, GEN9_HALF_SLICE_CHICKEN5, 0xffff, true}, /* 0xe188 */
	{RCS0, GEN9_HALF_SLICE_CHICKEN7, 0xffff, true}, /* 0xe194 */
	{RCS0, GEN8_ROW_CHICKEN, 0xffff, true}, /* 0xe4f0 */
	{RCS0, TRVATTL3PTRDW(0), 0, true}, /* 0x4de0 */
	{RCS0, TRVATTL3PTRDW(1), 0, true}, /* 0x4de4 */
	{RCS0, TRNULLDETCT, 0, true}, /* 0x4de8 */
	{RCS0, TRINVTILEDETCT, 0, true}, /* 0x4dec */
	{RCS0, TRVADR, 0, true}, /* 0x4df0 */
	{RCS0, TRTTE, 0, true}, /* 0x4df4 */
	{RCS0, _MMIO(0x4dfc), 0, true},

	{BCS0, RING_GFX_MODE(BLT_RING_BASE), 0xffff, false}, /* 0x2229c */
	{BCS0, RING_MI_MODE(BLT_RING_BASE), 0xffff, false}, /* 0x2209c */
	{BCS0, RING_INSTPM(BLT_RING_BASE), 0xffff, false}, /* 0x220c0 */
	{BCS0, RING_HWSTAM(BLT_RING_BASE), 0x0, false}, /* 0x22098 */
	{BCS0, RING_EXCC(BLT_RING_BASE), 0xffff, false}, /* 0x22028 */

	{VCS1, RING_EXCC(GEN8_BSD2_RING_BASE), 0xffff, false}, /* 0x1c028 */

	{VECS0, RING_EXCC(VEBOX_RING_BASE), 0xffff, false}, /* 0x1a028 */

	{RCS0, GEN8_HDC_CHICKEN1, 0xffff, true}, /* 0x7304 */
	{RCS0, GEN9_CTX_PREEMPT_REG, 0x0, false}, /* 0x2248 */
	{RCS0, GEN7_UCGCTL4, 0x0, false}, /* 0x940c */
	{RCS0, GAMT_CHKN_BIT_REG, 0x0, false}, /* 0x4ab8 */

	{RCS0, GEN9_GAMT_ECO_REG_RW_IA, 0x0, false}, /* 0x4ab0 */
	{RCS0, GEN9_CSFE_CHICKEN1_RCS, 0xffff, false}, /* 0x20d4 */
	{RCS0, _MMIO(0x20D8), 0xffff, true}, /* 0x20d8 */

	{RCS0, GEN8_GARBCNTL, 0x0, false}, /* 0xb004 */
	{RCS0, GEN7_FF_THREAD_MODE, 0x0, false}, /* 0x20a0 */
	{RCS0, FF_SLICE_CS_CHICKEN2, 0xffff, false}, /* 0x20e4 */
	{RCS0, INVALID_MMIO_REG, 0, false } /* Terminated */
>>>>>>> upstream/android-13
};

static struct {
	bool initialized;
	u32 control_table[I915_NUM_ENGINES][GEN9_MOCS_SIZE];
	u32 l3cc_table[GEN9_MOCS_SIZE / 2];
} gen9_render_mocs;

<<<<<<< HEAD
static void load_render_mocs(struct drm_i915_private *dev_priv)
{
	i915_reg_t offset;
	u32 regs[] = {
		[RCS] = 0xc800,
		[VCS] = 0xc900,
		[VCS2] = 0xca00,
		[BCS] = 0xcc00,
		[VECS] = 0xcb00,
	};
	int ring_id, i;

	for (ring_id = 0; ring_id < ARRAY_SIZE(regs); ring_id++) {
		offset.reg = regs[ring_id];
		for (i = 0; i < GEN9_MOCS_SIZE; i++) {
			gen9_render_mocs.control_table[ring_id][i] =
				I915_READ_FW(offset);
=======
static u32 gen9_mocs_mmio_offset_list[] = {
	[RCS0]  = 0xc800,
	[VCS0]  = 0xc900,
	[VCS1]  = 0xca00,
	[BCS0]  = 0xcc00,
	[VECS0] = 0xcb00,
};

static void load_render_mocs(const struct intel_engine_cs *engine)
{
	struct intel_gvt *gvt = engine->i915->gvt;
	struct intel_uncore *uncore = engine->uncore;
	u32 cnt = gvt->engine_mmio_list.mocs_mmio_offset_list_cnt;
	u32 *regs = gvt->engine_mmio_list.mocs_mmio_offset_list;
	i915_reg_t offset;
	int ring_id, i;

	/* Platform doesn't have mocs mmios. */
	if (!regs)
		return;

	for (ring_id = 0; ring_id < cnt; ring_id++) {
		if (!HAS_ENGINE(engine->gt, ring_id))
			continue;

		offset.reg = regs[ring_id];
		for (i = 0; i < GEN9_MOCS_SIZE; i++) {
			gen9_render_mocs.control_table[ring_id][i] =
				intel_uncore_read_fw(uncore, offset);
>>>>>>> upstream/android-13
			offset.reg += 4;
		}
	}

	offset.reg = 0xb020;
	for (i = 0; i < GEN9_MOCS_SIZE / 2; i++) {
		gen9_render_mocs.l3cc_table[i] =
<<<<<<< HEAD
			I915_READ_FW(offset);
=======
			intel_uncore_read_fw(uncore, offset);
>>>>>>> upstream/android-13
		offset.reg += 4;
	}
	gen9_render_mocs.initialized = true;
}

static int
restore_context_mmio_for_inhibit(struct intel_vgpu *vgpu,
				 struct i915_request *req)
{
	u32 *cs;
	int ret;
	struct engine_mmio *mmio;
	struct intel_gvt *gvt = vgpu->gvt;
	int ring_id = req->engine->id;
	int count = gvt->engine_mmio_list.ctx_mmio_count[ring_id];

	if (count == 0)
		return 0;

	ret = req->engine->emit_flush(req, EMIT_BARRIER);
	if (ret)
		return ret;

	cs = intel_ring_begin(req, count * 2 + 2);
	if (IS_ERR(cs))
		return PTR_ERR(cs);

	*cs++ = MI_LOAD_REGISTER_IMM(count);
	for (mmio = gvt->engine_mmio_list.mmio;
	     i915_mmio_reg_valid(mmio->reg); mmio++) {
<<<<<<< HEAD
		if (mmio->ring_id != ring_id ||
		    !mmio->in_context)
			continue;

		*cs++ = i915_mmio_reg_offset(mmio->reg);
		*cs++ = vgpu_vreg_t(vgpu, mmio->reg) |
				(mmio->mask << 16);
=======
		if (mmio->id != ring_id || !mmio->in_context)
			continue;

		*cs++ = i915_mmio_reg_offset(mmio->reg);
		*cs++ = vgpu_vreg_t(vgpu, mmio->reg) | (mmio->mask << 16);
>>>>>>> upstream/android-13
		gvt_dbg_core("add lri reg pair 0x%x:0x%x in inhibit ctx, vgpu:%d, rind_id:%d\n",
			      *(cs-2), *(cs-1), vgpu->id, ring_id);
	}

	*cs++ = MI_NOOP;
	intel_ring_advance(req, cs);

	ret = req->engine->emit_flush(req, EMIT_BARRIER);
	if (ret)
		return ret;

	return 0;
}

static int
restore_render_mocs_control_for_inhibit(struct intel_vgpu *vgpu,
					struct i915_request *req)
{
	unsigned int index;
	u32 *cs;

	cs = intel_ring_begin(req, 2 * GEN9_MOCS_SIZE + 2);
	if (IS_ERR(cs))
		return PTR_ERR(cs);

	*cs++ = MI_LOAD_REGISTER_IMM(GEN9_MOCS_SIZE);

	for (index = 0; index < GEN9_MOCS_SIZE; index++) {
		*cs++ = i915_mmio_reg_offset(GEN9_GFX_MOCS(index));
		*cs++ = vgpu_vreg_t(vgpu, GEN9_GFX_MOCS(index));
		gvt_dbg_core("add lri reg pair 0x%x:0x%x in inhibit ctx, vgpu:%d, rind_id:%d\n",
			      *(cs-2), *(cs-1), vgpu->id, req->engine->id);

	}

	*cs++ = MI_NOOP;
	intel_ring_advance(req, cs);

	return 0;
}

static int
restore_render_mocs_l3cc_for_inhibit(struct intel_vgpu *vgpu,
				     struct i915_request *req)
{
	unsigned int index;
	u32 *cs;

	cs = intel_ring_begin(req, 2 * GEN9_MOCS_SIZE / 2 + 2);
	if (IS_ERR(cs))
		return PTR_ERR(cs);

	*cs++ = MI_LOAD_REGISTER_IMM(GEN9_MOCS_SIZE / 2);

	for (index = 0; index < GEN9_MOCS_SIZE / 2; index++) {
		*cs++ = i915_mmio_reg_offset(GEN9_LNCFCMOCS(index));
		*cs++ = vgpu_vreg_t(vgpu, GEN9_LNCFCMOCS(index));
		gvt_dbg_core("add lri reg pair 0x%x:0x%x in inhibit ctx, vgpu:%d, rind_id:%d\n",
			      *(cs-2), *(cs-1), vgpu->id, req->engine->id);

	}

	*cs++ = MI_NOOP;
	intel_ring_advance(req, cs);

	return 0;
}

/*
 * Use lri command to initialize the mmio which is in context state image for
 * inhibit context, it contains tracked engine mmio, render_mocs and
 * render_mocs_l3cc.
 */
int intel_vgpu_restore_inhibit_context(struct intel_vgpu *vgpu,
				       struct i915_request *req)
{
	int ret;
	u32 *cs;

	cs = intel_ring_begin(req, 2);
	if (IS_ERR(cs))
		return PTR_ERR(cs);

	*cs++ = MI_ARB_ON_OFF | MI_ARB_DISABLE;
	*cs++ = MI_NOOP;
	intel_ring_advance(req, cs);

	ret = restore_context_mmio_for_inhibit(vgpu, req);
	if (ret)
		goto out;

	/* no MOCS register in context except render engine */
<<<<<<< HEAD
	if (req->engine->id != RCS)
=======
	if (req->engine->id != RCS0)
>>>>>>> upstream/android-13
		goto out;

	ret = restore_render_mocs_control_for_inhibit(vgpu, req);
	if (ret)
		goto out;

	ret = restore_render_mocs_l3cc_for_inhibit(vgpu, req);
	if (ret)
		goto out;

out:
	cs = intel_ring_begin(req, 2);
	if (IS_ERR(cs))
		return PTR_ERR(cs);

	*cs++ = MI_ARB_ON_OFF | MI_ARB_ENABLE;
	*cs++ = MI_NOOP;
	intel_ring_advance(req, cs);

	return ret;
}

<<<<<<< HEAD
static void handle_tlb_pending_event(struct intel_vgpu *vgpu, int ring_id)
{
	struct drm_i915_private *dev_priv = vgpu->gvt->dev_priv;
	struct intel_vgpu_submission *s = &vgpu->submission;
	enum forcewake_domains fw;
	i915_reg_t reg;
	u32 regs[] = {
		[RCS] = 0x4260,
		[VCS] = 0x4264,
		[VCS2] = 0x4268,
		[BCS] = 0x426c,
		[VECS] = 0x4270,
	};

	if (WARN_ON(ring_id >= ARRAY_SIZE(regs)))
		return;

	if (!test_and_clear_bit(ring_id, (void *)s->tlb_handle_pending))
		return;

	reg = _MMIO(regs[ring_id]);
=======
static u32 gen8_tlb_mmio_offset_list[] = {
	[RCS0]  = 0x4260,
	[VCS0]  = 0x4264,
	[VCS1]  = 0x4268,
	[BCS0]  = 0x426c,
	[VECS0] = 0x4270,
};

static void handle_tlb_pending_event(struct intel_vgpu *vgpu,
				     const struct intel_engine_cs *engine)
{
	struct intel_uncore *uncore = engine->uncore;
	struct intel_vgpu_submission *s = &vgpu->submission;
	u32 *regs = vgpu->gvt->engine_mmio_list.tlb_mmio_offset_list;
	u32 cnt = vgpu->gvt->engine_mmio_list.tlb_mmio_offset_list_cnt;
	enum forcewake_domains fw;
	i915_reg_t reg;

	if (!regs)
		return;

	if (drm_WARN_ON(&engine->i915->drm, engine->id >= cnt))
		return;

	if (!test_and_clear_bit(engine->id, (void *)s->tlb_handle_pending))
		return;

	reg = _MMIO(regs[engine->id]);
>>>>>>> upstream/android-13

	/* WaForceWakeRenderDuringMmioTLBInvalidate:skl
	 * we need to put a forcewake when invalidating RCS TLB caches,
	 * otherwise device can go to RC6 state and interrupt invalidation
	 * process
	 */
<<<<<<< HEAD
	fw = intel_uncore_forcewake_for_reg(dev_priv, reg,
					    FW_REG_READ | FW_REG_WRITE);
	if (ring_id == RCS && (IS_SKYLAKE(dev_priv) ||
			IS_KABYLAKE(dev_priv) || IS_BROXTON(dev_priv)))
		fw |= FORCEWAKE_RENDER;

	intel_uncore_forcewake_get(dev_priv, fw);

	I915_WRITE_FW(reg, 0x1);

	if (wait_for_atomic((I915_READ_FW(reg) == 0), 50))
		gvt_vgpu_err("timeout in invalidate ring (%d) tlb\n", ring_id);
	else
		vgpu_vreg_t(vgpu, reg) = 0;

	intel_uncore_forcewake_put(dev_priv, fw);

	gvt_dbg_core("invalidate TLB for ring %d\n", ring_id);
}

static void switch_mocs(struct intel_vgpu *pre, struct intel_vgpu *next,
			int ring_id)
{
	struct drm_i915_private *dev_priv;
	i915_reg_t offset, l3_offset;
	u32 old_v, new_v;

	u32 regs[] = {
		[RCS] = 0xc800,
		[VCS] = 0xc900,
		[VCS2] = 0xca00,
		[BCS] = 0xcc00,
		[VECS] = 0xcb00,
	};
	int i;

	dev_priv = pre ? pre->gvt->dev_priv : next->gvt->dev_priv;
	if (WARN_ON(ring_id >= ARRAY_SIZE(regs)))
		return;

	if ((IS_KABYLAKE(dev_priv) || IS_BROXTON(dev_priv)) && ring_id == RCS)
		return;

	if (!pre && !gen9_render_mocs.initialized)
		load_render_mocs(dev_priv);

	offset.reg = regs[ring_id];
=======
	fw = intel_uncore_forcewake_for_reg(uncore, reg,
					    FW_REG_READ | FW_REG_WRITE);
	if (engine->id == RCS0 && GRAPHICS_VER(engine->i915) >= 9)
		fw |= FORCEWAKE_RENDER;

	intel_uncore_forcewake_get(uncore, fw);

	intel_uncore_write_fw(uncore, reg, 0x1);

	if (wait_for_atomic(intel_uncore_read_fw(uncore, reg) == 0, 50))
		gvt_vgpu_err("timeout in invalidate ring %s tlb\n",
			     engine->name);
	else
		vgpu_vreg_t(vgpu, reg) = 0;

	intel_uncore_forcewake_put(uncore, fw);

	gvt_dbg_core("invalidate TLB for ring %s\n", engine->name);
}

static void switch_mocs(struct intel_vgpu *pre, struct intel_vgpu *next,
			const struct intel_engine_cs *engine)
{
	u32 regs[] = {
		[RCS0]  = 0xc800,
		[VCS0]  = 0xc900,
		[VCS1]  = 0xca00,
		[BCS0]  = 0xcc00,
		[VECS0] = 0xcb00,
	};
	struct intel_uncore *uncore = engine->uncore;
	i915_reg_t offset, l3_offset;
	u32 old_v, new_v;
	int i;

	if (drm_WARN_ON(&engine->i915->drm, engine->id >= ARRAY_SIZE(regs)))
		return;

	if (engine->id == RCS0 && GRAPHICS_VER(engine->i915) == 9)
		return;

	if (!pre && !gen9_render_mocs.initialized)
		load_render_mocs(engine);

	offset.reg = regs[engine->id];
>>>>>>> upstream/android-13
	for (i = 0; i < GEN9_MOCS_SIZE; i++) {
		if (pre)
			old_v = vgpu_vreg_t(pre, offset);
		else
<<<<<<< HEAD
			old_v = gen9_render_mocs.control_table[ring_id][i];
		if (next)
			new_v = vgpu_vreg_t(next, offset);
		else
			new_v = gen9_render_mocs.control_table[ring_id][i];

		if (old_v != new_v)
			I915_WRITE_FW(offset, new_v);
=======
			old_v = gen9_render_mocs.control_table[engine->id][i];
		if (next)
			new_v = vgpu_vreg_t(next, offset);
		else
			new_v = gen9_render_mocs.control_table[engine->id][i];

		if (old_v != new_v)
			intel_uncore_write_fw(uncore, offset, new_v);
>>>>>>> upstream/android-13

		offset.reg += 4;
	}

<<<<<<< HEAD
	if (ring_id == RCS) {
=======
	if (engine->id == RCS0) {
>>>>>>> upstream/android-13
		l3_offset.reg = 0xb020;
		for (i = 0; i < GEN9_MOCS_SIZE / 2; i++) {
			if (pre)
				old_v = vgpu_vreg_t(pre, l3_offset);
			else
				old_v = gen9_render_mocs.l3cc_table[i];
			if (next)
				new_v = vgpu_vreg_t(next, l3_offset);
			else
				new_v = gen9_render_mocs.l3cc_table[i];

			if (old_v != new_v)
<<<<<<< HEAD
				I915_WRITE_FW(l3_offset, new_v);
=======
				intel_uncore_write_fw(uncore, l3_offset, new_v);
>>>>>>> upstream/android-13

			l3_offset.reg += 4;
		}
	}
}

#define CTX_CONTEXT_CONTROL_VAL	0x03

bool is_inhibit_context(struct intel_context *ce)
{
	const u32 *reg_state = ce->lrc_reg_state;
	u32 inhibit_mask =
		_MASKED_BIT_ENABLE(CTX_CTRL_ENGINE_CTX_RESTORE_INHIBIT);

	return inhibit_mask ==
		(reg_state[CTX_CONTEXT_CONTROL_VAL] & inhibit_mask);
}

/* Switch ring mmio values (context). */
static void switch_mmio(struct intel_vgpu *pre,
			struct intel_vgpu *next,
<<<<<<< HEAD
			int ring_id)
{
	struct drm_i915_private *dev_priv;
=======
			const struct intel_engine_cs *engine)
{
	struct intel_uncore *uncore = engine->uncore;
>>>>>>> upstream/android-13
	struct intel_vgpu_submission *s;
	struct engine_mmio *mmio;
	u32 old_v, new_v;

<<<<<<< HEAD
	dev_priv = pre ? pre->gvt->dev_priv : next->gvt->dev_priv;
	if (IS_SKYLAKE(dev_priv)
		|| IS_KABYLAKE(dev_priv)
		|| IS_BROXTON(dev_priv))
		switch_mocs(pre, next, ring_id);

	for (mmio = dev_priv->gvt->engine_mmio_list.mmio;
	     i915_mmio_reg_valid(mmio->reg); mmio++) {
		if (mmio->ring_id != ring_id)
			continue;
		/*
		 * No need to do save or restore of the mmio which is in context
		 * state image on kabylake, it's initialized by lri command and
		 * save or restore with context together.
		 */
		if ((IS_KABYLAKE(dev_priv) || IS_BROXTON(dev_priv))
			&& mmio->in_context)
=======
	if (GRAPHICS_VER(engine->i915) >= 9)
		switch_mocs(pre, next, engine);

	for (mmio = engine->i915->gvt->engine_mmio_list.mmio;
	     i915_mmio_reg_valid(mmio->reg); mmio++) {
		if (mmio->id != engine->id)
			continue;
		/*
		 * No need to do save or restore of the mmio which is in context
		 * state image on gen9, it's initialized by lri command and
		 * save or restore with context together.
		 */
		if (GRAPHICS_VER(engine->i915) == 9 && mmio->in_context)
>>>>>>> upstream/android-13
			continue;

		// save
		if (pre) {
<<<<<<< HEAD
			vgpu_vreg_t(pre, mmio->reg) = I915_READ_FW(mmio->reg);
			if (mmio->mask)
				vgpu_vreg_t(pre, mmio->reg) &=
						~(mmio->mask << 16);
			old_v = vgpu_vreg_t(pre, mmio->reg);
		} else
			old_v = mmio->value = I915_READ_FW(mmio->reg);
=======
			vgpu_vreg_t(pre, mmio->reg) =
				intel_uncore_read_fw(uncore, mmio->reg);
			if (mmio->mask)
				vgpu_vreg_t(pre, mmio->reg) &=
					~(mmio->mask << 16);
			old_v = vgpu_vreg_t(pre, mmio->reg);
		} else {
			old_v = mmio->value =
				intel_uncore_read_fw(uncore, mmio->reg);
		}
>>>>>>> upstream/android-13

		// restore
		if (next) {
			s = &next->submission;
			/*
			 * No need to restore the mmio which is in context state
			 * image if it's not inhibit context, it will restore
			 * itself.
			 */
			if (mmio->in_context &&
<<<<<<< HEAD
			    !is_inhibit_context(&s->shadow_ctx->__engine[ring_id]))
=======
			    !is_inhibit_context(s->shadow[engine->id]))
>>>>>>> upstream/android-13
				continue;

			if (mmio->mask)
				new_v = vgpu_vreg_t(next, mmio->reg) |
<<<<<<< HEAD
							(mmio->mask << 16);
=======
					(mmio->mask << 16);
>>>>>>> upstream/android-13
			else
				new_v = vgpu_vreg_t(next, mmio->reg);
		} else {
			if (mmio->in_context)
				continue;
			if (mmio->mask)
				new_v = mmio->value | (mmio->mask << 16);
			else
				new_v = mmio->value;
		}

<<<<<<< HEAD
		I915_WRITE_FW(mmio->reg, new_v);
=======
		intel_uncore_write_fw(uncore, mmio->reg, new_v);
>>>>>>> upstream/android-13

		trace_render_mmio(pre ? pre->id : 0,
				  next ? next->id : 0,
				  "switch",
				  i915_mmio_reg_offset(mmio->reg),
				  old_v, new_v);
	}

	if (next)
<<<<<<< HEAD
		handle_tlb_pending_event(next, ring_id);
=======
		handle_tlb_pending_event(next, engine);
>>>>>>> upstream/android-13
}

/**
 * intel_gvt_switch_render_mmio - switch mmio context of specific engine
 * @pre: the last vGPU that own the engine
 * @next: the vGPU to switch to
<<<<<<< HEAD
 * @ring_id: specify the engine
=======
 * @engine: the engine
>>>>>>> upstream/android-13
 *
 * If pre is null indicates that host own the engine. If next is null
 * indicates that we are switching to host workload.
 */
void intel_gvt_switch_mmio(struct intel_vgpu *pre,
<<<<<<< HEAD
			   struct intel_vgpu *next, int ring_id)
{
	struct drm_i915_private *dev_priv;

	if (WARN_ON(!pre && !next))
		return;

	gvt_dbg_render("switch ring %d from %s to %s\n", ring_id,
		       pre ? "vGPU" : "host", next ? "vGPU" : "HOST");

	dev_priv = pre ? pre->gvt->dev_priv : next->gvt->dev_priv;

=======
			   struct intel_vgpu *next,
			   const struct intel_engine_cs *engine)
{
	if (WARN(!pre && !next, "switch ring %s from host to HOST\n",
		 engine->name))
		return;

	gvt_dbg_render("switch ring %s from %s to %s\n", engine->name,
		       pre ? "vGPU" : "host", next ? "vGPU" : "HOST");

>>>>>>> upstream/android-13
	/**
	 * We are using raw mmio access wrapper to improve the
	 * performace for batch mmio read/write, so we need
	 * handle forcewake mannually.
	 */
<<<<<<< HEAD
	intel_uncore_forcewake_get(dev_priv, FORCEWAKE_ALL);
	switch_mmio(pre, next, ring_id);
	intel_uncore_forcewake_put(dev_priv, FORCEWAKE_ALL);
=======
	intel_uncore_forcewake_get(engine->uncore, FORCEWAKE_ALL);
	switch_mmio(pre, next, engine);
	intel_uncore_forcewake_put(engine->uncore, FORCEWAKE_ALL);
>>>>>>> upstream/android-13
}

/**
 * intel_gvt_init_engine_mmio_context - Initiate the engine mmio list
 * @gvt: GVT device
 *
 */
void intel_gvt_init_engine_mmio_context(struct intel_gvt *gvt)
{
	struct engine_mmio *mmio;

<<<<<<< HEAD
	if (IS_SKYLAKE(gvt->dev_priv) ||
		IS_KABYLAKE(gvt->dev_priv) ||
		IS_BROXTON(gvt->dev_priv))
		gvt->engine_mmio_list.mmio = gen9_engine_mmio_list;
	else
		gvt->engine_mmio_list.mmio = gen8_engine_mmio_list;
=======
	if (GRAPHICS_VER(gvt->gt->i915) >= 9) {
		gvt->engine_mmio_list.mmio = gen9_engine_mmio_list;
		gvt->engine_mmio_list.tlb_mmio_offset_list = gen8_tlb_mmio_offset_list;
		gvt->engine_mmio_list.tlb_mmio_offset_list_cnt = ARRAY_SIZE(gen8_tlb_mmio_offset_list);
		gvt->engine_mmio_list.mocs_mmio_offset_list = gen9_mocs_mmio_offset_list;
		gvt->engine_mmio_list.mocs_mmio_offset_list_cnt = ARRAY_SIZE(gen9_mocs_mmio_offset_list);
	} else {
		gvt->engine_mmio_list.mmio = gen8_engine_mmio_list;
		gvt->engine_mmio_list.tlb_mmio_offset_list = gen8_tlb_mmio_offset_list;
		gvt->engine_mmio_list.tlb_mmio_offset_list_cnt = ARRAY_SIZE(gen8_tlb_mmio_offset_list);
	}
>>>>>>> upstream/android-13

	for (mmio = gvt->engine_mmio_list.mmio;
	     i915_mmio_reg_valid(mmio->reg); mmio++) {
		if (mmio->in_context) {
<<<<<<< HEAD
			gvt->engine_mmio_list.ctx_mmio_count[mmio->ring_id]++;
			intel_gvt_mmio_set_in_ctx(gvt, mmio->reg.reg);
=======
			gvt->engine_mmio_list.ctx_mmio_count[mmio->id]++;
			intel_gvt_mmio_set_sr_in_ctx(gvt, mmio->reg.reg);
>>>>>>> upstream/android-13
		}
	}
}
