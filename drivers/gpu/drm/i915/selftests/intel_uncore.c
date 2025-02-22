/*
 * Copyright © 2016 Intel Corporation
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
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 *
 */

#include "../i915_selftest.h"

static int intel_fw_table_check(const struct intel_forcewake_range *ranges,
				unsigned int num_ranges,
				bool is_watertight)
{
	unsigned int i;
	s32 prev;

	for (i = 0, prev = -1; i < num_ranges; i++, ranges++) {
		/* Check that the table is watertight */
		if (is_watertight && (prev + 1) != (s32)ranges->start) {
			pr_err("%s: entry[%d]:(%x, %x) is not watertight to previous (%x)\n",
			       __func__, i, ranges->start, ranges->end, prev);
			return -EINVAL;
		}

		/* Check that the table never goes backwards */
		if (prev >= (s32)ranges->start) {
			pr_err("%s: entry[%d]:(%x, %x) is less than the previous (%x)\n",
			       __func__, i, ranges->start, ranges->end, prev);
			return -EINVAL;
		}

		/* Check that the entry is valid */
		if (ranges->start >= ranges->end) {
			pr_err("%s: entry[%d]:(%x, %x) has negative length\n",
			       __func__, i, ranges->start, ranges->end);
			return -EINVAL;
		}

		prev = ranges->end;
	}

	return 0;
}

static int intel_shadow_table_check(void)
{
	struct {
		const i915_reg_t *regs;
		unsigned int size;
	} reg_lists[] = {
		{ gen8_shadowed_regs, ARRAY_SIZE(gen8_shadowed_regs) },
		{ gen11_shadowed_regs, ARRAY_SIZE(gen11_shadowed_regs) },
<<<<<<< HEAD
=======
		{ gen12_shadowed_regs, ARRAY_SIZE(gen12_shadowed_regs) },
		{ xehp_shadowed_regs, ARRAY_SIZE(xehp_shadowed_regs) },
>>>>>>> upstream/android-13
	};
	const i915_reg_t *reg;
	unsigned int i, j;
	s32 prev;

	for (j = 0; j < ARRAY_SIZE(reg_lists); ++j) {
		reg = reg_lists[j].regs;
		for (i = 0, prev = -1; i < reg_lists[j].size; i++, reg++) {
			u32 offset = i915_mmio_reg_offset(*reg);

			if (prev >= (s32)offset) {
				pr_err("%s: entry[%d]:(%x) is before previous (%x)\n",
				       __func__, i, offset, prev);
				return -EINVAL;
			}

			prev = offset;
		}
	}

	return 0;
}

int intel_uncore_mock_selftests(void)
{
	struct {
		const struct intel_forcewake_range *ranges;
		unsigned int num_ranges;
		bool is_watertight;
	} fw[] = {
		{ __vlv_fw_ranges, ARRAY_SIZE(__vlv_fw_ranges), false },
		{ __chv_fw_ranges, ARRAY_SIZE(__chv_fw_ranges), false },
		{ __gen9_fw_ranges, ARRAY_SIZE(__gen9_fw_ranges), true },
		{ __gen11_fw_ranges, ARRAY_SIZE(__gen11_fw_ranges), true },
<<<<<<< HEAD
=======
		{ __gen12_fw_ranges, ARRAY_SIZE(__gen12_fw_ranges), true },
		{ __xehp_fw_ranges, ARRAY_SIZE(__xehp_fw_ranges), true },
>>>>>>> upstream/android-13
	};
	int err, i;

	for (i = 0; i < ARRAY_SIZE(fw); i++) {
		err = intel_fw_table_check(fw[i].ranges,
					   fw[i].num_ranges,
					   fw[i].is_watertight);
		if (err)
			return err;
	}

	err = intel_shadow_table_check();
	if (err)
		return err;

	return 0;
}

<<<<<<< HEAD
static int intel_uncore_check_forcewake_domains(struct drm_i915_private *dev_priv)
{
#define FW_RANGE 0x40000
=======
static int live_forcewake_ops(void *arg)
{
	static const struct reg {
		const char *name;
		u8 min_graphics_ver;
		u8 max_graphics_ver;
		unsigned long platforms;
		unsigned int offset;
	} registers[] = {
		{
			"RING_START",
			6, 7,
			0x38,
		},
		{
			"RING_MI_MODE",
			8, U8_MAX,
			0x9c,
		}
	};
	const struct reg *r;
	struct intel_gt *gt = arg;
	struct intel_uncore_forcewake_domain *domain;
	struct intel_uncore *uncore = gt->uncore;
	struct intel_engine_cs *engine;
	enum intel_engine_id id;
	intel_wakeref_t wakeref;
	unsigned int tmp;
	int err = 0;

	GEM_BUG_ON(gt->awake);

	/* vlv/chv with their pcu behave differently wrt reads */
	if (IS_VALLEYVIEW(gt->i915) || IS_CHERRYVIEW(gt->i915)) {
		pr_debug("PCU fakes forcewake badly; skipping\n");
		return 0;
	}

	/*
	 * Not quite as reliable across the gen as one would hope.
	 *
	 * Either our theory of operation is incorrect, or there remain
	 * external parties interfering with the powerwells.
	 *
	 * https://bugs.freedesktop.org/show_bug.cgi?id=110210
	 */
	if (!IS_ENABLED(CONFIG_DRM_I915_SELFTEST_BROKEN))
		return 0;

	/* We have to pick carefully to get the exact behaviour we need */
	for (r = registers; r->name; r++)
		if (IS_GRAPHICS_VER(gt->i915, r->min_graphics_ver, r->max_graphics_ver))
			break;
	if (!r->name) {
		pr_debug("Forcewaked register not known for %s; skipping\n",
			 intel_platform_name(INTEL_INFO(gt->i915)->platform));
		return 0;
	}

	wakeref = intel_runtime_pm_get(uncore->rpm);

	for_each_fw_domain(domain, uncore, tmp) {
		smp_store_mb(domain->active, false);
		if (!hrtimer_cancel(&domain->timer))
			continue;

		intel_uncore_fw_release_timer(&domain->timer);
	}

	for_each_engine(engine, gt, id) {
		i915_reg_t mmio = _MMIO(engine->mmio_base + r->offset);
		u32 __iomem *reg = uncore->regs + engine->mmio_base + r->offset;
		enum forcewake_domains fw_domains;
		u32 val;

		if (!engine->default_state)
			continue;

		fw_domains = intel_uncore_forcewake_for_reg(uncore, mmio,
							    FW_REG_READ);
		if (!fw_domains)
			continue;

		for_each_fw_domain_masked(domain, fw_domains, uncore, tmp) {
			if (!domain->wake_count)
				continue;

			pr_err("fw_domain %s still active, aborting test!\n",
			       intel_uncore_forcewake_domain_to_str(domain->id));
			err = -EINVAL;
			goto out_rpm;
		}

		intel_uncore_forcewake_get(uncore, fw_domains);
		val = readl(reg);
		intel_uncore_forcewake_put(uncore, fw_domains);

		/* Flush the forcewake release (delayed onto a timer) */
		for_each_fw_domain_masked(domain, fw_domains, uncore, tmp) {
			smp_store_mb(domain->active, false);
			if (hrtimer_cancel(&domain->timer))
				intel_uncore_fw_release_timer(&domain->timer);

			preempt_disable();
			err = wait_ack_clear(domain, FORCEWAKE_KERNEL);
			preempt_enable();
			if (err) {
				pr_err("Failed to clear fw_domain %s\n",
				       intel_uncore_forcewake_domain_to_str(domain->id));
				goto out_rpm;
			}
		}

		if (!val) {
			pr_err("%s:%s was zero while fw was held!\n",
			       engine->name, r->name);
			err = -EINVAL;
			goto out_rpm;
		}

		/* We then expect the read to return 0 outside of the fw */
		if (wait_for(readl(reg) == 0, 100)) {
			pr_err("%s:%s=%0x, fw_domains 0x%x still up after 100ms!\n",
			       engine->name, r->name, readl(reg), fw_domains);
			err = -ETIMEDOUT;
			goto out_rpm;
		}
	}

out_rpm:
	intel_runtime_pm_put(uncore->rpm, wakeref);
	return err;
}

static int live_forcewake_domains(void *arg)
{
#define FW_RANGE 0x40000
	struct intel_gt *gt = arg;
	struct intel_uncore *uncore = gt->uncore;
>>>>>>> upstream/android-13
	unsigned long *valid;
	u32 offset;
	int err;

<<<<<<< HEAD
	if (!HAS_FPGA_DBG_UNCLAIMED(dev_priv) &&
	    !IS_VALLEYVIEW(dev_priv) &&
	    !IS_CHERRYVIEW(dev_priv))
=======
	if (!HAS_FPGA_DBG_UNCLAIMED(gt->i915) &&
	    !IS_VALLEYVIEW(gt->i915) &&
	    !IS_CHERRYVIEW(gt->i915))
>>>>>>> upstream/android-13
		return 0;

	/*
	 * This test may lockup the machine or cause GPU hangs afterwards.
	 */
	if (!IS_ENABLED(CONFIG_DRM_I915_SELFTEST_BROKEN))
		return 0;

<<<<<<< HEAD
	valid = kcalloc(BITS_TO_LONGS(FW_RANGE), sizeof(*valid),
			GFP_KERNEL);
	if (!valid)
		return -ENOMEM;

	intel_uncore_forcewake_get(dev_priv, FORCEWAKE_ALL);

	check_for_unclaimed_mmio(dev_priv);
	for (offset = 0; offset < FW_RANGE; offset += 4) {
		i915_reg_t reg = { offset };

		(void)I915_READ_FW(reg);
		if (!check_for_unclaimed_mmio(dev_priv))
			set_bit(offset, valid);
	}

	intel_uncore_forcewake_put(dev_priv, FORCEWAKE_ALL);
=======
	valid = bitmap_zalloc(FW_RANGE, GFP_KERNEL);
	if (!valid)
		return -ENOMEM;

	intel_uncore_forcewake_get(uncore, FORCEWAKE_ALL);

	check_for_unclaimed_mmio(uncore);
	for (offset = 0; offset < FW_RANGE; offset += 4) {
		i915_reg_t reg = { offset };

		intel_uncore_posting_read_fw(uncore, reg);
		if (!check_for_unclaimed_mmio(uncore))
			set_bit(offset, valid);
	}

	intel_uncore_forcewake_put(uncore, FORCEWAKE_ALL);
>>>>>>> upstream/android-13

	err = 0;
	for_each_set_bit(offset, valid, FW_RANGE) {
		i915_reg_t reg = { offset };

		iosf_mbi_punit_acquire();
<<<<<<< HEAD
		intel_uncore_forcewake_reset(dev_priv);
		iosf_mbi_punit_release();

		check_for_unclaimed_mmio(dev_priv);

		(void)I915_READ(reg);
		if (check_for_unclaimed_mmio(dev_priv)) {
=======
		intel_uncore_forcewake_reset(uncore);
		iosf_mbi_punit_release();

		check_for_unclaimed_mmio(uncore);

		intel_uncore_posting_read_fw(uncore, reg);
		if (check_for_unclaimed_mmio(uncore)) {
>>>>>>> upstream/android-13
			pr_err("Unclaimed mmio read to register 0x%04x\n",
			       offset);
			err = -EINVAL;
		}
	}

<<<<<<< HEAD
	kfree(valid);
	return err;
}

int intel_uncore_live_selftests(struct drm_i915_private *i915)
{
	int err;

	/* Confirm the table we load is still valid */
	err = intel_fw_table_check(i915->uncore.fw_domains_table,
				   i915->uncore.fw_domains_table_entries,
				   INTEL_GEN(i915) >= 9);
	if (err)
		return err;

	err = intel_uncore_check_forcewake_domains(i915);
	if (err)
		return err;

	return 0;
=======
	bitmap_free(valid);
	return err;
}

static int live_fw_table(void *arg)
{
	struct intel_gt *gt = arg;

	/* Confirm the table we load is still valid */
	return intel_fw_table_check(gt->uncore->fw_domains_table,
				    gt->uncore->fw_domains_table_entries,
				    GRAPHICS_VER(gt->i915) >= 9);
}

int intel_uncore_live_selftests(struct drm_i915_private *i915)
{
	static const struct i915_subtest tests[] = {
		SUBTEST(live_fw_table),
		SUBTEST(live_forcewake_ops),
		SUBTEST(live_forcewake_domains),
	};

	return intel_gt_live_subtests(tests, &i915->gt);
>>>>>>> upstream/android-13
}
