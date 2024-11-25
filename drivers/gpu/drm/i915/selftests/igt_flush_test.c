/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright © 2018 Intel Corporation
 */

<<<<<<< HEAD
#include "../i915_drv.h"

#include "../i915_selftest.h"
#include "igt_flush_test.h"

int igt_flush_test(struct drm_i915_private *i915, unsigned int flags)
{
	cond_resched();

	if (flags & I915_WAIT_LOCKED &&
	    i915_gem_switch_to_kernel_context(i915)) {
		pr_err("Failed to switch back to kernel context; declaring wedged\n");
		i915_gem_set_wedged(i915);
	}

	if (i915_gem_wait_for_idle(i915, flags, HZ / 5) == -ETIME) {
		pr_err("%pS timed out, cancelling all further testing.\n",
		       __builtin_return_address(0));

		GEM_TRACE("%pS timed out.\n", __builtin_return_address(0));
		GEM_TRACE_DUMP();

		i915_gem_set_wedged(i915);
	}

	return i915_terminally_wedged(&i915->gpu_error) ? -EIO : 0;
=======
#include "gt/intel_gt.h"
#include "gt/intel_gt_requests.h"

#include "i915_drv.h"
#include "i915_selftest.h"

#include "igt_flush_test.h"

int igt_flush_test(struct drm_i915_private *i915)
{
	struct intel_gt *gt = &i915->gt;
	int ret = intel_gt_is_wedged(gt) ? -EIO : 0;

	cond_resched();

	if (intel_gt_wait_for_idle(gt, HZ) == -ETIME) {
		pr_err("%pS timed out, cancelling all further testing.\n",
		       __builtin_return_address(0));

		GEM_TRACE("%pS timed out.\n",
			  __builtin_return_address(0));
		GEM_TRACE_DUMP();

		intel_gt_set_wedged(gt);
		ret = -EIO;
	}

	return ret;
>>>>>>> upstream/android-13
}
