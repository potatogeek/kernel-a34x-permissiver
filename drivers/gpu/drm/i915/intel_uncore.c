/*
 * Copyright © 2013 Intel Corporation
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
 */

<<<<<<< HEAD
#include "i915_drv.h"
#include "intel_drv.h"
#include "i915_vgpu.h"

#include <asm/iosf_mbi.h>
#include <linux/pm_runtime.h>
=======
#include <linux/pm_runtime.h>
#include <asm/iosf_mbi.h>

#include "gt/intel_lrc_reg.h" /* for shadow reg list */

#include "i915_drv.h"
#include "i915_trace.h"
#include "i915_vgpu.h"
#include "intel_pm.h"
>>>>>>> upstream/android-13

#define FORCEWAKE_ACK_TIMEOUT_MS 50
#define GT_FIFO_TIMEOUT_MS	 10

<<<<<<< HEAD
#define __raw_posting_read(dev_priv__, reg__) (void)__raw_i915_read32((dev_priv__), (reg__))
=======
#define __raw_posting_read(...) ((void)__raw_uncore_read32(__VA_ARGS__))

void
intel_uncore_mmio_debug_init_early(struct intel_uncore_mmio_debug *mmio_debug)
{
	spin_lock_init(&mmio_debug->lock);
	mmio_debug->unclaimed_mmio_check = 1;
}

static void mmio_debug_suspend(struct intel_uncore_mmio_debug *mmio_debug)
{
	lockdep_assert_held(&mmio_debug->lock);

	/* Save and disable mmio debugging for the user bypass */
	if (!mmio_debug->suspend_count++) {
		mmio_debug->saved_mmio_check = mmio_debug->unclaimed_mmio_check;
		mmio_debug->unclaimed_mmio_check = 0;
	}
}

static void mmio_debug_resume(struct intel_uncore_mmio_debug *mmio_debug)
{
	lockdep_assert_held(&mmio_debug->lock);

	if (!--mmio_debug->suspend_count)
		mmio_debug->unclaimed_mmio_check = mmio_debug->saved_mmio_check;
}
>>>>>>> upstream/android-13

static const char * const forcewake_domain_names[] = {
	"render",
	"blitter",
	"media",
	"vdbox0",
	"vdbox1",
	"vdbox2",
	"vdbox3",
<<<<<<< HEAD
	"vebox0",
	"vebox1",
=======
	"vdbox4",
	"vdbox5",
	"vdbox6",
	"vdbox7",
	"vebox0",
	"vebox1",
	"vebox2",
	"vebox3",
>>>>>>> upstream/android-13
};

const char *
intel_uncore_forcewake_domain_to_str(const enum forcewake_domain_id id)
{
	BUILD_BUG_ON(ARRAY_SIZE(forcewake_domain_names) != FW_DOMAIN_ID_COUNT);

	if (id >= 0 && id < FW_DOMAIN_ID_COUNT)
		return forcewake_domain_names[id];

	WARN_ON(id);

	return "unknown";
}

<<<<<<< HEAD
static inline void
fw_domain_reset(struct drm_i915_private *i915,
		const struct intel_uncore_forcewake_domain *d)
=======
#define fw_ack(d) readl((d)->reg_ack)
#define fw_set(d, val) writel(_MASKED_BIT_ENABLE((val)), (d)->reg_set)
#define fw_clear(d, val) writel(_MASKED_BIT_DISABLE((val)), (d)->reg_set)

static inline void
fw_domain_reset(const struct intel_uncore_forcewake_domain *d)
>>>>>>> upstream/android-13
{
	/*
	 * We don't really know if the powerwell for the forcewake domain we are
	 * trying to reset here does exist at this point (engines could be fused
	 * off in ICL+), so no waiting for acks
	 */
<<<<<<< HEAD
	__raw_i915_write32(i915, d->reg_set, i915->uncore.fw_reset);
=======
	/* WaRsClearFWBitsAtReset:bdw,skl */
	fw_clear(d, 0xffff);
>>>>>>> upstream/android-13
}

static inline void
fw_domain_arm_timer(struct intel_uncore_forcewake_domain *d)
{
<<<<<<< HEAD
=======
	GEM_BUG_ON(d->uncore->fw_domains_timer & d->mask);
	d->uncore->fw_domains_timer |= d->mask;
>>>>>>> upstream/android-13
	d->wake_count++;
	hrtimer_start_range_ns(&d->timer,
			       NSEC_PER_MSEC,
			       NSEC_PER_MSEC,
			       HRTIMER_MODE_REL);
}

static inline int
<<<<<<< HEAD
__wait_for_ack(const struct drm_i915_private *i915,
	       const struct intel_uncore_forcewake_domain *d,
	       const u32 ack,
	       const u32 value)
{
	return wait_for_atomic((__raw_i915_read32(i915, d->reg_ack) & ack) == value,
=======
__wait_for_ack(const struct intel_uncore_forcewake_domain *d,
	       const u32 ack,
	       const u32 value)
{
	return wait_for_atomic((fw_ack(d) & ack) == value,
>>>>>>> upstream/android-13
			       FORCEWAKE_ACK_TIMEOUT_MS);
}

static inline int
<<<<<<< HEAD
wait_ack_clear(const struct drm_i915_private *i915,
	       const struct intel_uncore_forcewake_domain *d,
	       const u32 ack)
{
	return __wait_for_ack(i915, d, ack, 0);
}

static inline int
wait_ack_set(const struct drm_i915_private *i915,
	     const struct intel_uncore_forcewake_domain *d,
	     const u32 ack)
{
	return __wait_for_ack(i915, d, ack, ack);
}

static inline void
fw_domain_wait_ack_clear(const struct drm_i915_private *i915,
			 const struct intel_uncore_forcewake_domain *d)
{
	if (wait_ack_clear(i915, d, FORCEWAKE_KERNEL))
		DRM_ERROR("%s: timed out waiting for forcewake ack to clear.\n",
			  intel_uncore_forcewake_domain_to_str(d->id));
=======
wait_ack_clear(const struct intel_uncore_forcewake_domain *d,
	       const u32 ack)
{
	return __wait_for_ack(d, ack, 0);
}

static inline int
wait_ack_set(const struct intel_uncore_forcewake_domain *d,
	     const u32 ack)
{
	return __wait_for_ack(d, ack, ack);
}

static inline void
fw_domain_wait_ack_clear(const struct intel_uncore_forcewake_domain *d)
{
	if (wait_ack_clear(d, FORCEWAKE_KERNEL)) {
		DRM_ERROR("%s: timed out waiting for forcewake ack to clear.\n",
			  intel_uncore_forcewake_domain_to_str(d->id));
		add_taint_for_CI(d->uncore->i915, TAINT_WARN); /* CI now unreliable */
	}
>>>>>>> upstream/android-13
}

enum ack_type {
	ACK_CLEAR = 0,
	ACK_SET
};

static int
<<<<<<< HEAD
fw_domain_wait_ack_with_fallback(const struct drm_i915_private *i915,
				 const struct intel_uncore_forcewake_domain *d,
=======
fw_domain_wait_ack_with_fallback(const struct intel_uncore_forcewake_domain *d,
>>>>>>> upstream/android-13
				 const enum ack_type type)
{
	const u32 ack_bit = FORCEWAKE_KERNEL;
	const u32 value = type == ACK_SET ? ack_bit : 0;
	unsigned int pass;
	bool ack_detected;

	/*
	 * There is a possibility of driver's wake request colliding
	 * with hardware's own wake requests and that can cause
	 * hardware to not deliver the driver's ack message.
	 *
	 * Use a fallback bit toggle to kick the gpu state machine
	 * in the hope that the original ack will be delivered along with
	 * the fallback ack.
	 *
	 * This workaround is described in HSDES #1604254524 and it's known as:
	 * WaRsForcewakeAddDelayForAck:skl,bxt,kbl,glk,cfl,cnl,icl
	 * although the name is a bit misleading.
	 */

	pass = 1;
	do {
<<<<<<< HEAD
		wait_ack_clear(i915, d, FORCEWAKE_KERNEL_FALLBACK);

		__raw_i915_write32(i915, d->reg_set,
				   _MASKED_BIT_ENABLE(FORCEWAKE_KERNEL_FALLBACK));
		/* Give gt some time to relax before the polling frenzy */
		udelay(10 * pass);
		wait_ack_set(i915, d, FORCEWAKE_KERNEL_FALLBACK);

		ack_detected = (__raw_i915_read32(i915, d->reg_ack) & ack_bit) == value;

		__raw_i915_write32(i915, d->reg_set,
				   _MASKED_BIT_DISABLE(FORCEWAKE_KERNEL_FALLBACK));
=======
		wait_ack_clear(d, FORCEWAKE_KERNEL_FALLBACK);

		fw_set(d, FORCEWAKE_KERNEL_FALLBACK);
		/* Give gt some time to relax before the polling frenzy */
		udelay(10 * pass);
		wait_ack_set(d, FORCEWAKE_KERNEL_FALLBACK);

		ack_detected = (fw_ack(d) & ack_bit) == value;

		fw_clear(d, FORCEWAKE_KERNEL_FALLBACK);
>>>>>>> upstream/android-13
	} while (!ack_detected && pass++ < 10);

	DRM_DEBUG_DRIVER("%s had to use fallback to %s ack, 0x%x (passes %u)\n",
			 intel_uncore_forcewake_domain_to_str(d->id),
			 type == ACK_SET ? "set" : "clear",
<<<<<<< HEAD
			 __raw_i915_read32(i915, d->reg_ack),
=======
			 fw_ack(d),
>>>>>>> upstream/android-13
			 pass);

	return ack_detected ? 0 : -ETIMEDOUT;
}

static inline void
<<<<<<< HEAD
fw_domain_wait_ack_clear_fallback(const struct drm_i915_private *i915,
				  const struct intel_uncore_forcewake_domain *d)
{
	if (likely(!wait_ack_clear(i915, d, FORCEWAKE_KERNEL)))
		return;

	if (fw_domain_wait_ack_with_fallback(i915, d, ACK_CLEAR))
		fw_domain_wait_ack_clear(i915, d);
}

static inline void
fw_domain_get(struct drm_i915_private *i915,
	      const struct intel_uncore_forcewake_domain *d)
{
	__raw_i915_write32(i915, d->reg_set, i915->uncore.fw_set);
}

static inline void
fw_domain_wait_ack_set(const struct drm_i915_private *i915,
		       const struct intel_uncore_forcewake_domain *d)
{
	if (wait_ack_set(i915, d, FORCEWAKE_KERNEL))
		DRM_ERROR("%s: timed out waiting for forcewake ack request.\n",
			  intel_uncore_forcewake_domain_to_str(d->id));
}

static inline void
fw_domain_wait_ack_set_fallback(const struct drm_i915_private *i915,
				const struct intel_uncore_forcewake_domain *d)
{
	if (likely(!wait_ack_set(i915, d, FORCEWAKE_KERNEL)))
		return;

	if (fw_domain_wait_ack_with_fallback(i915, d, ACK_SET))
		fw_domain_wait_ack_set(i915, d);
}

static inline void
fw_domain_put(const struct drm_i915_private *i915,
	      const struct intel_uncore_forcewake_domain *d)
{
	__raw_i915_write32(i915, d->reg_set, i915->uncore.fw_clear);
}

static void
fw_domains_get(struct drm_i915_private *i915, enum forcewake_domains fw_domains)
=======
fw_domain_wait_ack_clear_fallback(const struct intel_uncore_forcewake_domain *d)
{
	if (likely(!wait_ack_clear(d, FORCEWAKE_KERNEL)))
		return;

	if (fw_domain_wait_ack_with_fallback(d, ACK_CLEAR))
		fw_domain_wait_ack_clear(d);
}

static inline void
fw_domain_get(const struct intel_uncore_forcewake_domain *d)
{
	fw_set(d, FORCEWAKE_KERNEL);
}

static inline void
fw_domain_wait_ack_set(const struct intel_uncore_forcewake_domain *d)
{
	if (wait_ack_set(d, FORCEWAKE_KERNEL)) {
		DRM_ERROR("%s: timed out waiting for forcewake ack request.\n",
			  intel_uncore_forcewake_domain_to_str(d->id));
		add_taint_for_CI(d->uncore->i915, TAINT_WARN); /* CI now unreliable */
	}
}

static inline void
fw_domain_wait_ack_set_fallback(const struct intel_uncore_forcewake_domain *d)
{
	if (likely(!wait_ack_set(d, FORCEWAKE_KERNEL)))
		return;

	if (fw_domain_wait_ack_with_fallback(d, ACK_SET))
		fw_domain_wait_ack_set(d);
}

static inline void
fw_domain_put(const struct intel_uncore_forcewake_domain *d)
{
	fw_clear(d, FORCEWAKE_KERNEL);
}

static void
fw_domains_get(struct intel_uncore *uncore, enum forcewake_domains fw_domains)
>>>>>>> upstream/android-13
{
	struct intel_uncore_forcewake_domain *d;
	unsigned int tmp;

<<<<<<< HEAD
	GEM_BUG_ON(fw_domains & ~i915->uncore.fw_domains);

	for_each_fw_domain_masked(d, fw_domains, i915, tmp) {
		fw_domain_wait_ack_clear(i915, d);
		fw_domain_get(i915, d);
	}

	for_each_fw_domain_masked(d, fw_domains, i915, tmp)
		fw_domain_wait_ack_set(i915, d);

	i915->uncore.fw_domains_active |= fw_domains;
}

static void
fw_domains_get_with_fallback(struct drm_i915_private *i915,
=======
	GEM_BUG_ON(fw_domains & ~uncore->fw_domains);

	for_each_fw_domain_masked(d, fw_domains, uncore, tmp) {
		fw_domain_wait_ack_clear(d);
		fw_domain_get(d);
	}

	for_each_fw_domain_masked(d, fw_domains, uncore, tmp)
		fw_domain_wait_ack_set(d);

	uncore->fw_domains_active |= fw_domains;
}

static void
fw_domains_get_with_fallback(struct intel_uncore *uncore,
>>>>>>> upstream/android-13
			     enum forcewake_domains fw_domains)
{
	struct intel_uncore_forcewake_domain *d;
	unsigned int tmp;

<<<<<<< HEAD
	GEM_BUG_ON(fw_domains & ~i915->uncore.fw_domains);

	for_each_fw_domain_masked(d, fw_domains, i915, tmp) {
		fw_domain_wait_ack_clear_fallback(i915, d);
		fw_domain_get(i915, d);
	}

	for_each_fw_domain_masked(d, fw_domains, i915, tmp)
		fw_domain_wait_ack_set_fallback(i915, d);

	i915->uncore.fw_domains_active |= fw_domains;
}

static void
fw_domains_put(struct drm_i915_private *i915, enum forcewake_domains fw_domains)
=======
	GEM_BUG_ON(fw_domains & ~uncore->fw_domains);

	for_each_fw_domain_masked(d, fw_domains, uncore, tmp) {
		fw_domain_wait_ack_clear_fallback(d);
		fw_domain_get(d);
	}

	for_each_fw_domain_masked(d, fw_domains, uncore, tmp)
		fw_domain_wait_ack_set_fallback(d);

	uncore->fw_domains_active |= fw_domains;
}

static void
fw_domains_put(struct intel_uncore *uncore, enum forcewake_domains fw_domains)
>>>>>>> upstream/android-13
{
	struct intel_uncore_forcewake_domain *d;
	unsigned int tmp;

<<<<<<< HEAD
	GEM_BUG_ON(fw_domains & ~i915->uncore.fw_domains);

	for_each_fw_domain_masked(d, fw_domains, i915, tmp)
		fw_domain_put(i915, d);

	i915->uncore.fw_domains_active &= ~fw_domains;
}

static void
fw_domains_reset(struct drm_i915_private *i915,
=======
	GEM_BUG_ON(fw_domains & ~uncore->fw_domains);

	for_each_fw_domain_masked(d, fw_domains, uncore, tmp)
		fw_domain_put(d);

	uncore->fw_domains_active &= ~fw_domains;
}

static void
fw_domains_reset(struct intel_uncore *uncore,
>>>>>>> upstream/android-13
		 enum forcewake_domains fw_domains)
{
	struct intel_uncore_forcewake_domain *d;
	unsigned int tmp;

	if (!fw_domains)
		return;

<<<<<<< HEAD
	GEM_BUG_ON(fw_domains & ~i915->uncore.fw_domains);

	for_each_fw_domain_masked(d, fw_domains, i915, tmp)
		fw_domain_reset(i915, d);
}

static void __gen6_gt_wait_for_thread_c0(struct drm_i915_private *dev_priv)
{
	/* w/a for a sporadic read returning 0 by waiting for the GT
	 * thread to wake up.
	 */
	if (wait_for_atomic_us((__raw_i915_read32(dev_priv, GEN6_GT_THREAD_STATUS_REG) &
				GEN6_GT_THREAD_STATUS_CORE_MASK) == 0, 500))
		DRM_ERROR("GT thread status wait timed out\n");
}

static void fw_domains_get_with_thread_status(struct drm_i915_private *dev_priv,
					      enum forcewake_domains fw_domains)
{
	fw_domains_get(dev_priv, fw_domains);

	/* WaRsForcewakeWaitTC0:snb,ivb,hsw,bdw,vlv */
	__gen6_gt_wait_for_thread_c0(dev_priv);
}

static inline u32 fifo_free_entries(struct drm_i915_private *dev_priv)
{
	u32 count = __raw_i915_read32(dev_priv, GTFIFOCTL);
=======
	GEM_BUG_ON(fw_domains & ~uncore->fw_domains);

	for_each_fw_domain_masked(d, fw_domains, uncore, tmp)
		fw_domain_reset(d);
}

static inline u32 gt_thread_status(struct intel_uncore *uncore)
{
	u32 val;

	val = __raw_uncore_read32(uncore, GEN6_GT_THREAD_STATUS_REG);
	val &= GEN6_GT_THREAD_STATUS_CORE_MASK;

	return val;
}

static void __gen6_gt_wait_for_thread_c0(struct intel_uncore *uncore)
{
	/*
	 * w/a for a sporadic read returning 0 by waiting for the GT
	 * thread to wake up.
	 */
	drm_WARN_ONCE(&uncore->i915->drm,
		      wait_for_atomic_us(gt_thread_status(uncore) == 0, 5000),
		      "GT thread status wait timed out\n");
}

static void fw_domains_get_with_thread_status(struct intel_uncore *uncore,
					      enum forcewake_domains fw_domains)
{
	fw_domains_get(uncore, fw_domains);

	/* WaRsForcewakeWaitTC0:snb,ivb,hsw,bdw,vlv */
	__gen6_gt_wait_for_thread_c0(uncore);
}

static inline u32 fifo_free_entries(struct intel_uncore *uncore)
{
	u32 count = __raw_uncore_read32(uncore, GTFIFOCTL);
>>>>>>> upstream/android-13

	return count & GT_FIFO_FREE_ENTRIES_MASK;
}

<<<<<<< HEAD
static void __gen6_gt_wait_for_fifo(struct drm_i915_private *dev_priv)
=======
static void __gen6_gt_wait_for_fifo(struct intel_uncore *uncore)
>>>>>>> upstream/android-13
{
	u32 n;

	/* On VLV, FIFO will be shared by both SW and HW.
	 * So, we need to read the FREE_ENTRIES everytime */
<<<<<<< HEAD
	if (IS_VALLEYVIEW(dev_priv))
		n = fifo_free_entries(dev_priv);
	else
		n = dev_priv->uncore.fifo_count;

	if (n <= GT_FIFO_NUM_RESERVED_ENTRIES) {
		if (wait_for_atomic((n = fifo_free_entries(dev_priv)) >
				    GT_FIFO_NUM_RESERVED_ENTRIES,
				    GT_FIFO_TIMEOUT_MS)) {
			DRM_DEBUG("GT_FIFO timeout, entries: %u\n", n);
=======
	if (IS_VALLEYVIEW(uncore->i915))
		n = fifo_free_entries(uncore);
	else
		n = uncore->fifo_count;

	if (n <= GT_FIFO_NUM_RESERVED_ENTRIES) {
		if (wait_for_atomic((n = fifo_free_entries(uncore)) >
				    GT_FIFO_NUM_RESERVED_ENTRIES,
				    GT_FIFO_TIMEOUT_MS)) {
			drm_dbg(&uncore->i915->drm,
				"GT_FIFO timeout, entries: %u\n", n);
>>>>>>> upstream/android-13
			return;
		}
	}

<<<<<<< HEAD
	dev_priv->uncore.fifo_count = n - 1;
=======
	uncore->fifo_count = n - 1;
>>>>>>> upstream/android-13
}

static enum hrtimer_restart
intel_uncore_fw_release_timer(struct hrtimer *timer)
{
	struct intel_uncore_forcewake_domain *domain =
	       container_of(timer, struct intel_uncore_forcewake_domain, timer);
<<<<<<< HEAD
	struct drm_i915_private *dev_priv =
		container_of(domain, struct drm_i915_private, uncore.fw_domain[domain->id]);
	unsigned long irqflags;

	assert_rpm_device_not_suspended(dev_priv);
=======
	struct intel_uncore *uncore = domain->uncore;
	unsigned long irqflags;

	assert_rpm_device_not_suspended(uncore->rpm);
>>>>>>> upstream/android-13

	if (xchg(&domain->active, false))
		return HRTIMER_RESTART;

<<<<<<< HEAD
	spin_lock_irqsave(&dev_priv->uncore.lock, irqflags);
	if (WARN_ON(domain->wake_count == 0))
		domain->wake_count++;

	if (--domain->wake_count == 0)
		dev_priv->uncore.funcs.force_wake_put(dev_priv, domain->mask);

	spin_unlock_irqrestore(&dev_priv->uncore.lock, irqflags);
=======
	spin_lock_irqsave(&uncore->lock, irqflags);

	uncore->fw_domains_timer &= ~domain->mask;

	GEM_BUG_ON(!domain->wake_count);
	if (--domain->wake_count == 0)
		uncore->funcs.force_wake_put(uncore, domain->mask);

	spin_unlock_irqrestore(&uncore->lock, irqflags);
>>>>>>> upstream/android-13

	return HRTIMER_NORESTART;
}

/* Note callers must have acquired the PUNIT->PMIC bus, before calling this. */
static unsigned int
<<<<<<< HEAD
intel_uncore_forcewake_reset(struct drm_i915_private *dev_priv)
=======
intel_uncore_forcewake_reset(struct intel_uncore *uncore)
>>>>>>> upstream/android-13
{
	unsigned long irqflags;
	struct intel_uncore_forcewake_domain *domain;
	int retry_count = 100;
	enum forcewake_domains fw, active_domains;

	iosf_mbi_assert_punit_acquired();

	/* Hold uncore.lock across reset to prevent any register access
	 * with forcewake not set correctly. Wait until all pending
	 * timers are run before holding.
	 */
	while (1) {
		unsigned int tmp;

		active_domains = 0;

<<<<<<< HEAD
		for_each_fw_domain(domain, dev_priv, tmp) {
=======
		for_each_fw_domain(domain, uncore, tmp) {
>>>>>>> upstream/android-13
			smp_store_mb(domain->active, false);
			if (hrtimer_cancel(&domain->timer) == 0)
				continue;

			intel_uncore_fw_release_timer(&domain->timer);
		}

<<<<<<< HEAD
		spin_lock_irqsave(&dev_priv->uncore.lock, irqflags);

		for_each_fw_domain(domain, dev_priv, tmp) {
=======
		spin_lock_irqsave(&uncore->lock, irqflags);

		for_each_fw_domain(domain, uncore, tmp) {
>>>>>>> upstream/android-13
			if (hrtimer_active(&domain->timer))
				active_domains |= domain->mask;
		}

		if (active_domains == 0)
			break;

		if (--retry_count == 0) {
<<<<<<< HEAD
			DRM_ERROR("Timed out waiting for forcewake timers to finish\n");
			break;
		}

		spin_unlock_irqrestore(&dev_priv->uncore.lock, irqflags);
		cond_resched();
	}

	WARN_ON(active_domains);

	fw = dev_priv->uncore.fw_domains_active;
	if (fw)
		dev_priv->uncore.funcs.force_wake_put(dev_priv, fw);

	fw_domains_reset(dev_priv, dev_priv->uncore.fw_domains);
	assert_forcewakes_inactive(dev_priv);

	spin_unlock_irqrestore(&dev_priv->uncore.lock, irqflags);
=======
			drm_err(&uncore->i915->drm, "Timed out waiting for forcewake timers to finish\n");
			break;
		}

		spin_unlock_irqrestore(&uncore->lock, irqflags);
		cond_resched();
	}

	drm_WARN_ON(&uncore->i915->drm, active_domains);

	fw = uncore->fw_domains_active;
	if (fw)
		uncore->funcs.force_wake_put(uncore, fw);

	fw_domains_reset(uncore, uncore->fw_domains);
	assert_forcewakes_inactive(uncore);

	spin_unlock_irqrestore(&uncore->lock, irqflags);
>>>>>>> upstream/android-13

	return fw; /* track the lost user forcewake domains */
}

<<<<<<< HEAD
static u64 gen9_edram_size(struct drm_i915_private *dev_priv)
{
	const unsigned int ways[8] = { 4, 8, 12, 16, 16, 16, 16, 16 };
	const unsigned int sets[4] = { 1, 1, 2, 2 };
	const u32 cap = dev_priv->edram_cap;

	return EDRAM_NUM_BANKS(cap) *
		ways[EDRAM_WAYS_IDX(cap)] *
		sets[EDRAM_SETS_IDX(cap)] *
		1024 * 1024;
}

u64 intel_uncore_edram_size(struct drm_i915_private *dev_priv)
{
	if (!HAS_EDRAM(dev_priv))
		return 0;

	/* The needed capability bits for size calculation
	 * are not there with pre gen9 so return 128MB always.
	 */
	if (INTEL_GEN(dev_priv) < 9)
		return 128 * 1024 * 1024;

	return gen9_edram_size(dev_priv);
}

static void intel_uncore_edram_detect(struct drm_i915_private *dev_priv)
{
	if (IS_HASWELL(dev_priv) ||
	    IS_BROADWELL(dev_priv) ||
	    INTEL_GEN(dev_priv) >= 9) {
		dev_priv->edram_cap = __raw_i915_read32(dev_priv,
							HSW_EDRAM_CAP);

		/* NB: We can't write IDICR yet because we do not have gt funcs
		 * set up */
	} else {
		dev_priv->edram_cap = 0;
	}

	if (HAS_EDRAM(dev_priv))
		DRM_INFO("Found %lluMB of eDRAM\n",
			 intel_uncore_edram_size(dev_priv) / (1024 * 1024));
}

static bool
fpga_check_for_unclaimed_mmio(struct drm_i915_private *dev_priv)
{
	u32 dbg;

	dbg = __raw_i915_read32(dev_priv, FPGA_DBG);
	if (likely(!(dbg & FPGA_DBG_RM_NOCLAIM)))
		return false;

	__raw_i915_write32(dev_priv, FPGA_DBG, FPGA_DBG_RM_NOCLAIM);
=======
static bool
fpga_check_for_unclaimed_mmio(struct intel_uncore *uncore)
{
	u32 dbg;

	dbg = __raw_uncore_read32(uncore, FPGA_DBG);
	if (likely(!(dbg & FPGA_DBG_RM_NOCLAIM)))
		return false;

	/*
	 * Bugs in PCI programming (or failing hardware) can occasionally cause
	 * us to lose access to the MMIO BAR.  When this happens, register
	 * reads will come back with 0xFFFFFFFF for every register and things
	 * go bad very quickly.  Let's try to detect that special case and at
	 * least try to print a more informative message about what has
	 * happened.
	 *
	 * During normal operation the FPGA_DBG register has several unused
	 * bits that will always read back as 0's so we can use them as canaries
	 * to recognize when MMIO accesses are just busted.
	 */
	if (unlikely(dbg == ~0))
		drm_err(&uncore->i915->drm,
			"Lost access to MMIO BAR; all registers now read back as 0xFFFFFFFF!\n");

	__raw_uncore_write32(uncore, FPGA_DBG, FPGA_DBG_RM_NOCLAIM);
>>>>>>> upstream/android-13

	return true;
}

static bool
<<<<<<< HEAD
vlv_check_for_unclaimed_mmio(struct drm_i915_private *dev_priv)
{
	u32 cer;

	cer = __raw_i915_read32(dev_priv, CLAIM_ER);
	if (likely(!(cer & (CLAIM_ER_OVERFLOW | CLAIM_ER_CTR_MASK))))
		return false;

	__raw_i915_write32(dev_priv, CLAIM_ER, CLAIM_ER_CLR);
=======
vlv_check_for_unclaimed_mmio(struct intel_uncore *uncore)
{
	u32 cer;

	cer = __raw_uncore_read32(uncore, CLAIM_ER);
	if (likely(!(cer & (CLAIM_ER_OVERFLOW | CLAIM_ER_CTR_MASK))))
		return false;

	__raw_uncore_write32(uncore, CLAIM_ER, CLAIM_ER_CLR);
>>>>>>> upstream/android-13

	return true;
}

static bool
<<<<<<< HEAD
gen6_check_for_fifo_debug(struct drm_i915_private *dev_priv)
{
	u32 fifodbg;

	fifodbg = __raw_i915_read32(dev_priv, GTFIFODBG);

	if (unlikely(fifodbg)) {
		DRM_DEBUG_DRIVER("GTFIFODBG = 0x08%x\n", fifodbg);
		__raw_i915_write32(dev_priv, GTFIFODBG, fifodbg);
=======
gen6_check_for_fifo_debug(struct intel_uncore *uncore)
{
	u32 fifodbg;

	fifodbg = __raw_uncore_read32(uncore, GTFIFODBG);

	if (unlikely(fifodbg)) {
		drm_dbg(&uncore->i915->drm, "GTFIFODBG = 0x08%x\n", fifodbg);
		__raw_uncore_write32(uncore, GTFIFODBG, fifodbg);
>>>>>>> upstream/android-13
	}

	return fifodbg;
}

static bool
<<<<<<< HEAD
check_for_unclaimed_mmio(struct drm_i915_private *dev_priv)
{
	bool ret = false;

	if (HAS_FPGA_DBG_UNCLAIMED(dev_priv))
		ret |= fpga_check_for_unclaimed_mmio(dev_priv);

	if (IS_VALLEYVIEW(dev_priv) || IS_CHERRYVIEW(dev_priv))
		ret |= vlv_check_for_unclaimed_mmio(dev_priv);

	if (IS_GEN6(dev_priv) || IS_GEN7(dev_priv))
		ret |= gen6_check_for_fifo_debug(dev_priv);
=======
check_for_unclaimed_mmio(struct intel_uncore *uncore)
{
	bool ret = false;

	lockdep_assert_held(&uncore->debug->lock);

	if (uncore->debug->suspend_count)
		return false;

	if (intel_uncore_has_fpga_dbg_unclaimed(uncore))
		ret |= fpga_check_for_unclaimed_mmio(uncore);

	if (intel_uncore_has_dbg_unclaimed(uncore))
		ret |= vlv_check_for_unclaimed_mmio(uncore);

	if (intel_uncore_has_fifo(uncore))
		ret |= gen6_check_for_fifo_debug(uncore);
>>>>>>> upstream/android-13

	return ret;
}

<<<<<<< HEAD
static void __intel_uncore_early_sanitize(struct drm_i915_private *dev_priv,
					  unsigned int restore_forcewake)
{
	/* clear out unclaimed reg detection bit */
	if (check_for_unclaimed_mmio(dev_priv))
		DRM_DEBUG("unclaimed mmio detected on uncore init, clearing\n");

	/* WaDisableShadowRegForCpd:chv */
	if (IS_CHERRYVIEW(dev_priv)) {
		__raw_i915_write32(dev_priv, GTFIFOCTL,
				   __raw_i915_read32(dev_priv, GTFIFOCTL) |
				   GT_FIFO_CTL_BLOCK_ALL_POLICY_STALL |
				   GT_FIFO_CTL_RC6_POLICY_STALL);
	}

	iosf_mbi_punit_acquire();
	intel_uncore_forcewake_reset(dev_priv);
	if (restore_forcewake) {
		spin_lock_irq(&dev_priv->uncore.lock);
		dev_priv->uncore.funcs.force_wake_get(dev_priv,
						      restore_forcewake);

		if (IS_GEN6(dev_priv) || IS_GEN7(dev_priv))
			dev_priv->uncore.fifo_count =
				fifo_free_entries(dev_priv);
		spin_unlock_irq(&dev_priv->uncore.lock);
=======
static void forcewake_early_sanitize(struct intel_uncore *uncore,
				     unsigned int restore_forcewake)
{
	GEM_BUG_ON(!intel_uncore_has_forcewake(uncore));

	/* WaDisableShadowRegForCpd:chv */
	if (IS_CHERRYVIEW(uncore->i915)) {
		__raw_uncore_write32(uncore, GTFIFOCTL,
				     __raw_uncore_read32(uncore, GTFIFOCTL) |
				     GT_FIFO_CTL_BLOCK_ALL_POLICY_STALL |
				     GT_FIFO_CTL_RC6_POLICY_STALL);
	}

	iosf_mbi_punit_acquire();
	intel_uncore_forcewake_reset(uncore);
	if (restore_forcewake) {
		spin_lock_irq(&uncore->lock);
		uncore->funcs.force_wake_get(uncore, restore_forcewake);

		if (intel_uncore_has_fifo(uncore))
			uncore->fifo_count = fifo_free_entries(uncore);
		spin_unlock_irq(&uncore->lock);
>>>>>>> upstream/android-13
	}
	iosf_mbi_punit_release();
}

<<<<<<< HEAD
void intel_uncore_suspend(struct drm_i915_private *dev_priv)
{
	iosf_mbi_punit_acquire();
	iosf_mbi_unregister_pmic_bus_access_notifier_unlocked(
		&dev_priv->uncore.pmic_bus_access_nb);
	dev_priv->uncore.fw_domains_saved =
		intel_uncore_forcewake_reset(dev_priv);
	iosf_mbi_punit_release();
}

void intel_uncore_resume_early(struct drm_i915_private *dev_priv)
{
	unsigned int restore_forcewake;

	restore_forcewake = fetch_and_zero(&dev_priv->uncore.fw_domains_saved);
	__intel_uncore_early_sanitize(dev_priv, restore_forcewake);

	iosf_mbi_register_pmic_bus_access_notifier(
		&dev_priv->uncore.pmic_bus_access_nb);
	i915_check_and_clear_faults(dev_priv);
}

void intel_uncore_runtime_resume(struct drm_i915_private *dev_priv)
{
	iosf_mbi_register_pmic_bus_access_notifier(
		&dev_priv->uncore.pmic_bus_access_nb);
}

void intel_uncore_sanitize(struct drm_i915_private *dev_priv)
{
	/* BIOS often leaves RC6 enabled, but disable it for hw init */
	intel_sanitize_gt_powersave(dev_priv);
}

static void __intel_uncore_forcewake_get(struct drm_i915_private *dev_priv,
=======
void intel_uncore_suspend(struct intel_uncore *uncore)
{
	if (!intel_uncore_has_forcewake(uncore))
		return;

	iosf_mbi_punit_acquire();
	iosf_mbi_unregister_pmic_bus_access_notifier_unlocked(
		&uncore->pmic_bus_access_nb);
	uncore->fw_domains_saved = intel_uncore_forcewake_reset(uncore);
	iosf_mbi_punit_release();
}

void intel_uncore_resume_early(struct intel_uncore *uncore)
{
	unsigned int restore_forcewake;

	if (intel_uncore_unclaimed_mmio(uncore))
		drm_dbg(&uncore->i915->drm, "unclaimed mmio detected on resume, clearing\n");

	if (!intel_uncore_has_forcewake(uncore))
		return;

	restore_forcewake = fetch_and_zero(&uncore->fw_domains_saved);
	forcewake_early_sanitize(uncore, restore_forcewake);

	iosf_mbi_register_pmic_bus_access_notifier(&uncore->pmic_bus_access_nb);
}

void intel_uncore_runtime_resume(struct intel_uncore *uncore)
{
	if (!intel_uncore_has_forcewake(uncore))
		return;

	iosf_mbi_register_pmic_bus_access_notifier(&uncore->pmic_bus_access_nb);
}

static void __intel_uncore_forcewake_get(struct intel_uncore *uncore,
>>>>>>> upstream/android-13
					 enum forcewake_domains fw_domains)
{
	struct intel_uncore_forcewake_domain *domain;
	unsigned int tmp;

<<<<<<< HEAD
	fw_domains &= dev_priv->uncore.fw_domains;

	for_each_fw_domain_masked(domain, fw_domains, dev_priv, tmp) {
=======
	fw_domains &= uncore->fw_domains;

	for_each_fw_domain_masked(domain, fw_domains, uncore, tmp) {
>>>>>>> upstream/android-13
		if (domain->wake_count++) {
			fw_domains &= ~domain->mask;
			domain->active = true;
		}
	}

	if (fw_domains)
<<<<<<< HEAD
		dev_priv->uncore.funcs.force_wake_get(dev_priv, fw_domains);
=======
		uncore->funcs.force_wake_get(uncore, fw_domains);
>>>>>>> upstream/android-13
}

/**
 * intel_uncore_forcewake_get - grab forcewake domain references
<<<<<<< HEAD
 * @dev_priv: i915 device instance
=======
 * @uncore: the intel_uncore structure
>>>>>>> upstream/android-13
 * @fw_domains: forcewake domains to get reference on
 *
 * This function can be used get GT's forcewake domain references.
 * Normal register access will handle the forcewake domains automatically.
 * However if some sequence requires the GT to not power down a particular
 * forcewake domains this function should be called at the beginning of the
 * sequence. And subsequently the reference should be dropped by symmetric
 * call to intel_unforce_forcewake_put(). Usually caller wants all the domains
 * to be kept awake so the @fw_domains would be then FORCEWAKE_ALL.
 */
<<<<<<< HEAD
void intel_uncore_forcewake_get(struct drm_i915_private *dev_priv,
=======
void intel_uncore_forcewake_get(struct intel_uncore *uncore,
>>>>>>> upstream/android-13
				enum forcewake_domains fw_domains)
{
	unsigned long irqflags;

<<<<<<< HEAD
	if (!dev_priv->uncore.funcs.force_wake_get)
		return;

	assert_rpm_wakelock_held(dev_priv);

	spin_lock_irqsave(&dev_priv->uncore.lock, irqflags);
	__intel_uncore_forcewake_get(dev_priv, fw_domains);
	spin_unlock_irqrestore(&dev_priv->uncore.lock, irqflags);
=======
	if (!uncore->funcs.force_wake_get)
		return;

	assert_rpm_wakelock_held(uncore->rpm);

	spin_lock_irqsave(&uncore->lock, irqflags);
	__intel_uncore_forcewake_get(uncore, fw_domains);
	spin_unlock_irqrestore(&uncore->lock, irqflags);
>>>>>>> upstream/android-13
}

/**
 * intel_uncore_forcewake_user_get - claim forcewake on behalf of userspace
<<<<<<< HEAD
 * @dev_priv: i915 device instance
=======
 * @uncore: the intel_uncore structure
>>>>>>> upstream/android-13
 *
 * This function is a wrapper around intel_uncore_forcewake_get() to acquire
 * the GT powerwell and in the process disable our debugging for the
 * duration of userspace's bypass.
 */
<<<<<<< HEAD
void intel_uncore_forcewake_user_get(struct drm_i915_private *dev_priv)
{
	spin_lock_irq(&dev_priv->uncore.lock);
	if (!dev_priv->uncore.user_forcewake.count++) {
		intel_uncore_forcewake_get__locked(dev_priv, FORCEWAKE_ALL);

		/* Save and disable mmio debugging for the user bypass */
		dev_priv->uncore.user_forcewake.saved_mmio_check =
			dev_priv->uncore.unclaimed_mmio_check;
		dev_priv->uncore.user_forcewake.saved_mmio_debug =
			i915_modparams.mmio_debug;

		dev_priv->uncore.unclaimed_mmio_check = 0;
		i915_modparams.mmio_debug = 0;
	}
	spin_unlock_irq(&dev_priv->uncore.lock);
=======
void intel_uncore_forcewake_user_get(struct intel_uncore *uncore)
{
	spin_lock_irq(&uncore->lock);
	if (!uncore->user_forcewake_count++) {
		intel_uncore_forcewake_get__locked(uncore, FORCEWAKE_ALL);
		spin_lock(&uncore->debug->lock);
		mmio_debug_suspend(uncore->debug);
		spin_unlock(&uncore->debug->lock);
	}
	spin_unlock_irq(&uncore->lock);
>>>>>>> upstream/android-13
}

/**
 * intel_uncore_forcewake_user_put - release forcewake on behalf of userspace
<<<<<<< HEAD
 * @dev_priv: i915 device instance
=======
 * @uncore: the intel_uncore structure
>>>>>>> upstream/android-13
 *
 * This function complements intel_uncore_forcewake_user_get() and releases
 * the GT powerwell taken on behalf of the userspace bypass.
 */
<<<<<<< HEAD
void intel_uncore_forcewake_user_put(struct drm_i915_private *dev_priv)
{
	spin_lock_irq(&dev_priv->uncore.lock);
	if (!--dev_priv->uncore.user_forcewake.count) {
		if (intel_uncore_unclaimed_mmio(dev_priv))
			dev_info(dev_priv->drm.dev,
				 "Invalid mmio detected during user access\n");

		dev_priv->uncore.unclaimed_mmio_check =
			dev_priv->uncore.user_forcewake.saved_mmio_check;
		i915_modparams.mmio_debug =
			dev_priv->uncore.user_forcewake.saved_mmio_debug;

		intel_uncore_forcewake_put__locked(dev_priv, FORCEWAKE_ALL);
	}
	spin_unlock_irq(&dev_priv->uncore.lock);
=======
void intel_uncore_forcewake_user_put(struct intel_uncore *uncore)
{
	spin_lock_irq(&uncore->lock);
	if (!--uncore->user_forcewake_count) {
		spin_lock(&uncore->debug->lock);
		mmio_debug_resume(uncore->debug);

		if (check_for_unclaimed_mmio(uncore))
			drm_info(&uncore->i915->drm,
				 "Invalid mmio detected during user access\n");
		spin_unlock(&uncore->debug->lock);

		intel_uncore_forcewake_put__locked(uncore, FORCEWAKE_ALL);
	}
	spin_unlock_irq(&uncore->lock);
>>>>>>> upstream/android-13
}

/**
 * intel_uncore_forcewake_get__locked - grab forcewake domain references
<<<<<<< HEAD
 * @dev_priv: i915 device instance
=======
 * @uncore: the intel_uncore structure
>>>>>>> upstream/android-13
 * @fw_domains: forcewake domains to get reference on
 *
 * See intel_uncore_forcewake_get(). This variant places the onus
 * on the caller to explicitly handle the dev_priv->uncore.lock spinlock.
 */
<<<<<<< HEAD
void intel_uncore_forcewake_get__locked(struct drm_i915_private *dev_priv,
					enum forcewake_domains fw_domains)
{
	lockdep_assert_held(&dev_priv->uncore.lock);

	if (!dev_priv->uncore.funcs.force_wake_get)
		return;

	__intel_uncore_forcewake_get(dev_priv, fw_domains);
}

static void __intel_uncore_forcewake_put(struct drm_i915_private *dev_priv,
					 enum forcewake_domains fw_domains)
=======
void intel_uncore_forcewake_get__locked(struct intel_uncore *uncore,
					enum forcewake_domains fw_domains)
{
	lockdep_assert_held(&uncore->lock);

	if (!uncore->funcs.force_wake_get)
		return;

	__intel_uncore_forcewake_get(uncore, fw_domains);
}

static void __intel_uncore_forcewake_put(struct intel_uncore *uncore,
					 enum forcewake_domains fw_domains,
					 bool delayed)
>>>>>>> upstream/android-13
{
	struct intel_uncore_forcewake_domain *domain;
	unsigned int tmp;

<<<<<<< HEAD
	fw_domains &= dev_priv->uncore.fw_domains;

	for_each_fw_domain_masked(domain, fw_domains, dev_priv, tmp) {
		if (WARN_ON(domain->wake_count == 0))
			continue;
=======
	fw_domains &= uncore->fw_domains;

	for_each_fw_domain_masked(domain, fw_domains, uncore, tmp) {
		GEM_BUG_ON(!domain->wake_count);
>>>>>>> upstream/android-13

		if (--domain->wake_count) {
			domain->active = true;
			continue;
		}

<<<<<<< HEAD
		fw_domain_arm_timer(domain);
=======
		if (delayed &&
		    !(domain->uncore->fw_domains_timer & domain->mask))
			fw_domain_arm_timer(domain);
		else
			uncore->funcs.force_wake_put(uncore, domain->mask);
>>>>>>> upstream/android-13
	}
}

/**
 * intel_uncore_forcewake_put - release a forcewake domain reference
<<<<<<< HEAD
 * @dev_priv: i915 device instance
=======
 * @uncore: the intel_uncore structure
>>>>>>> upstream/android-13
 * @fw_domains: forcewake domains to put references
 *
 * This function drops the device-level forcewakes for specified
 * domains obtained by intel_uncore_forcewake_get().
 */
<<<<<<< HEAD
void intel_uncore_forcewake_put(struct drm_i915_private *dev_priv,
=======
void intel_uncore_forcewake_put(struct intel_uncore *uncore,
>>>>>>> upstream/android-13
				enum forcewake_domains fw_domains)
{
	unsigned long irqflags;

<<<<<<< HEAD
	if (!dev_priv->uncore.funcs.force_wake_put)
		return;

	spin_lock_irqsave(&dev_priv->uncore.lock, irqflags);
	__intel_uncore_forcewake_put(dev_priv, fw_domains);
	spin_unlock_irqrestore(&dev_priv->uncore.lock, irqflags);
=======
	if (!uncore->funcs.force_wake_put)
		return;

	spin_lock_irqsave(&uncore->lock, irqflags);
	__intel_uncore_forcewake_put(uncore, fw_domains, false);
	spin_unlock_irqrestore(&uncore->lock, irqflags);
}

void intel_uncore_forcewake_put_delayed(struct intel_uncore *uncore,
					enum forcewake_domains fw_domains)
{
	unsigned long irqflags;

	if (!uncore->funcs.force_wake_put)
		return;

	spin_lock_irqsave(&uncore->lock, irqflags);
	__intel_uncore_forcewake_put(uncore, fw_domains, true);
	spin_unlock_irqrestore(&uncore->lock, irqflags);
}

/**
 * intel_uncore_forcewake_flush - flush the delayed release
 * @uncore: the intel_uncore structure
 * @fw_domains: forcewake domains to flush
 */
void intel_uncore_forcewake_flush(struct intel_uncore *uncore,
				  enum forcewake_domains fw_domains)
{
	struct intel_uncore_forcewake_domain *domain;
	unsigned int tmp;

	if (!uncore->funcs.force_wake_put)
		return;

	fw_domains &= uncore->fw_domains;
	for_each_fw_domain_masked(domain, fw_domains, uncore, tmp) {
		WRITE_ONCE(domain->active, false);
		if (hrtimer_cancel(&domain->timer))
			intel_uncore_fw_release_timer(&domain->timer);
	}
>>>>>>> upstream/android-13
}

/**
 * intel_uncore_forcewake_put__locked - grab forcewake domain references
<<<<<<< HEAD
 * @dev_priv: i915 device instance
=======
 * @uncore: the intel_uncore structure
>>>>>>> upstream/android-13
 * @fw_domains: forcewake domains to get reference on
 *
 * See intel_uncore_forcewake_put(). This variant places the onus
 * on the caller to explicitly handle the dev_priv->uncore.lock spinlock.
 */
<<<<<<< HEAD
void intel_uncore_forcewake_put__locked(struct drm_i915_private *dev_priv,
					enum forcewake_domains fw_domains)
{
	lockdep_assert_held(&dev_priv->uncore.lock);

	if (!dev_priv->uncore.funcs.force_wake_put)
		return;

	__intel_uncore_forcewake_put(dev_priv, fw_domains);
}

void assert_forcewakes_inactive(struct drm_i915_private *dev_priv)
{
	if (!dev_priv->uncore.funcs.force_wake_get)
		return;

	WARN(dev_priv->uncore.fw_domains_active,
	     "Expected all fw_domains to be inactive, but %08x are still on\n",
	     dev_priv->uncore.fw_domains_active);
}

void assert_forcewakes_active(struct drm_i915_private *dev_priv,
			      enum forcewake_domains fw_domains)
{
	if (!dev_priv->uncore.funcs.force_wake_get)
		return;

	assert_rpm_wakelock_held(dev_priv);

	fw_domains &= dev_priv->uncore.fw_domains;
	WARN(fw_domains & ~dev_priv->uncore.fw_domains_active,
	     "Expected %08x fw_domains to be active, but %08x are off\n",
	     fw_domains, fw_domains & ~dev_priv->uncore.fw_domains_active);
=======
void intel_uncore_forcewake_put__locked(struct intel_uncore *uncore,
					enum forcewake_domains fw_domains)
{
	lockdep_assert_held(&uncore->lock);

	if (!uncore->funcs.force_wake_put)
		return;

	__intel_uncore_forcewake_put(uncore, fw_domains, false);
}

void assert_forcewakes_inactive(struct intel_uncore *uncore)
{
	if (!uncore->funcs.force_wake_get)
		return;

	drm_WARN(&uncore->i915->drm, uncore->fw_domains_active,
		 "Expected all fw_domains to be inactive, but %08x are still on\n",
		 uncore->fw_domains_active);
}

void assert_forcewakes_active(struct intel_uncore *uncore,
			      enum forcewake_domains fw_domains)
{
	struct intel_uncore_forcewake_domain *domain;
	unsigned int tmp;

	if (!IS_ENABLED(CONFIG_DRM_I915_DEBUG_RUNTIME_PM))
		return;

	if (!uncore->funcs.force_wake_get)
		return;

	spin_lock_irq(&uncore->lock);

	assert_rpm_wakelock_held(uncore->rpm);

	fw_domains &= uncore->fw_domains;
	drm_WARN(&uncore->i915->drm, fw_domains & ~uncore->fw_domains_active,
		 "Expected %08x fw_domains to be active, but %08x are off\n",
		 fw_domains, fw_domains & ~uncore->fw_domains_active);

	/*
	 * Check that the caller has an explicit wakeref and we don't mistake
	 * it for the auto wakeref.
	 */
	for_each_fw_domain_masked(domain, fw_domains, uncore, tmp) {
		unsigned int actual = READ_ONCE(domain->wake_count);
		unsigned int expect = 1;

		if (uncore->fw_domains_timer & domain->mask)
			expect++; /* pending automatic release */

		if (drm_WARN(&uncore->i915->drm, actual < expect,
			     "Expected domain %d to be held awake by caller, count=%d\n",
			     domain->id, actual))
			break;
	}

	spin_unlock_irq(&uncore->lock);
>>>>>>> upstream/android-13
}

/* We give fast paths for the really cool registers */
#define NEEDS_FORCE_WAKE(reg) ((reg) < 0x40000)

<<<<<<< HEAD
#define GEN11_NEEDS_FORCE_WAKE(reg) \
	((reg) < 0x40000 || ((reg) >= 0x1c0000 && (reg) < 0x1dc000))

#define __gen6_reg_read_fw_domains(offset) \
=======
#define __gen6_reg_read_fw_domains(uncore, offset) \
>>>>>>> upstream/android-13
({ \
	enum forcewake_domains __fwd; \
	if (NEEDS_FORCE_WAKE(offset)) \
		__fwd = FORCEWAKE_RENDER; \
	else \
		__fwd = 0; \
	__fwd; \
})

static int fw_range_cmp(u32 offset, const struct intel_forcewake_range *entry)
{
	if (offset < entry->start)
		return -1;
	else if (offset > entry->end)
		return 1;
	else
		return 0;
}

/* Copied and "macroized" from lib/bsearch.c */
#define BSEARCH(key, base, num, cmp) ({                                 \
	unsigned int start__ = 0, end__ = (num);                        \
	typeof(base) result__ = NULL;                                   \
	while (start__ < end__) {                                       \
		unsigned int mid__ = start__ + (end__ - start__) / 2;   \
		int ret__ = (cmp)((key), (base) + mid__);               \
		if (ret__ < 0) {                                        \
			end__ = mid__;                                  \
		} else if (ret__ > 0) {                                 \
			start__ = mid__ + 1;                            \
		} else {                                                \
			result__ = (base) + mid__;                      \
			break;                                          \
		}                                                       \
	}                                                               \
	result__;                                                       \
})

static enum forcewake_domains
<<<<<<< HEAD
find_fw_domain(struct drm_i915_private *dev_priv, u32 offset)
=======
find_fw_domain(struct intel_uncore *uncore, u32 offset)
>>>>>>> upstream/android-13
{
	const struct intel_forcewake_range *entry;

	entry = BSEARCH(offset,
<<<<<<< HEAD
			dev_priv->uncore.fw_domains_table,
			dev_priv->uncore.fw_domains_table_entries,
=======
			uncore->fw_domains_table,
			uncore->fw_domains_table_entries,
>>>>>>> upstream/android-13
			fw_range_cmp);

	if (!entry)
		return 0;

	/*
	 * The list of FW domains depends on the SKU in gen11+ so we
	 * can't determine it statically. We use FORCEWAKE_ALL and
	 * translate it here to the list of available domains.
	 */
	if (entry->domains == FORCEWAKE_ALL)
<<<<<<< HEAD
		return dev_priv->uncore.fw_domains;

	WARN(entry->domains & ~dev_priv->uncore.fw_domains,
	     "Uninitialized forcewake domain(s) 0x%x accessed at 0x%x\n",
	     entry->domains & ~dev_priv->uncore.fw_domains, offset);
=======
		return uncore->fw_domains;

	drm_WARN(&uncore->i915->drm, entry->domains & ~uncore->fw_domains,
		 "Uninitialized forcewake domain(s) 0x%x accessed at 0x%x\n",
		 entry->domains & ~uncore->fw_domains, offset);
>>>>>>> upstream/android-13

	return entry->domains;
}

#define GEN_FW_RANGE(s, e, d) \
	{ .start = (s), .end = (e), .domains = (d) }

<<<<<<< HEAD
#define HAS_FWTABLE(dev_priv) \
	(INTEL_GEN(dev_priv) >= 9 || \
	 IS_CHERRYVIEW(dev_priv) || \
	 IS_VALLEYVIEW(dev_priv))

=======
>>>>>>> upstream/android-13
/* *Must* be sorted by offset ranges! See intel_fw_table_check(). */
static const struct intel_forcewake_range __vlv_fw_ranges[] = {
	GEN_FW_RANGE(0x2000, 0x3fff, FORCEWAKE_RENDER),
	GEN_FW_RANGE(0x5000, 0x7fff, FORCEWAKE_RENDER),
	GEN_FW_RANGE(0xb000, 0x11fff, FORCEWAKE_RENDER),
	GEN_FW_RANGE(0x12000, 0x13fff, FORCEWAKE_MEDIA),
	GEN_FW_RANGE(0x22000, 0x23fff, FORCEWAKE_MEDIA),
	GEN_FW_RANGE(0x2e000, 0x2ffff, FORCEWAKE_RENDER),
	GEN_FW_RANGE(0x30000, 0x3ffff, FORCEWAKE_MEDIA),
};

<<<<<<< HEAD
#define __fwtable_reg_read_fw_domains(offset) \
({ \
	enum forcewake_domains __fwd = 0; \
	if (NEEDS_FORCE_WAKE((offset))) \
		__fwd = find_fw_domain(dev_priv, offset); \
	__fwd; \
})

#define __gen11_fwtable_reg_read_fw_domains(offset) \
({ \
	enum forcewake_domains __fwd = 0; \
	if (GEN11_NEEDS_FORCE_WAKE((offset))) \
		__fwd = find_fw_domain(dev_priv, offset); \
	__fwd; \
})
=======
#define __fwtable_reg_read_fw_domains(uncore, offset) \
({ \
	enum forcewake_domains __fwd = 0; \
	if (NEEDS_FORCE_WAKE((offset))) \
		__fwd = find_fw_domain(uncore, offset); \
	__fwd; \
})

#define __gen11_fwtable_reg_read_fw_domains(uncore, offset) \
	find_fw_domain(uncore, offset)

#define __gen12_fwtable_reg_read_fw_domains(uncore, offset) \
	find_fw_domain(uncore, offset)
>>>>>>> upstream/android-13

/* *Must* be sorted by offset! See intel_shadow_table_check(). */
static const i915_reg_t gen8_shadowed_regs[] = {
	RING_TAIL(RENDER_RING_BASE),	/* 0x2000 (base) */
	GEN6_RPNSWREQ,			/* 0xA008 */
	GEN6_RC_VIDEO_FREQ,		/* 0xA00C */
	RING_TAIL(GEN6_BSD_RING_BASE),	/* 0x12000 (base) */
	RING_TAIL(VEBOX_RING_BASE),	/* 0x1a000 (base) */
	RING_TAIL(BLT_RING_BASE),	/* 0x22000 (base) */
	/* TODO: Other registers are not yet used */
};

static const i915_reg_t gen11_shadowed_regs[] = {
<<<<<<< HEAD
	RING_TAIL(RENDER_RING_BASE),		/* 0x2000 (base) */
	GEN6_RPNSWREQ,				/* 0xA008 */
	GEN6_RC_VIDEO_FREQ,			/* 0xA00C */
	RING_TAIL(BLT_RING_BASE),		/* 0x22000 (base) */
	RING_TAIL(GEN11_BSD_RING_BASE),		/* 0x1C0000 (base) */
	RING_TAIL(GEN11_BSD2_RING_BASE),	/* 0x1C4000 (base) */
	RING_TAIL(GEN11_VEBOX_RING_BASE),	/* 0x1C8000 (base) */
	RING_TAIL(GEN11_BSD3_RING_BASE),	/* 0x1D0000 (base) */
	RING_TAIL(GEN11_BSD4_RING_BASE),	/* 0x1D4000 (base) */
	RING_TAIL(GEN11_VEBOX2_RING_BASE),	/* 0x1D8000 (base) */
=======
	RING_TAIL(RENDER_RING_BASE),			/* 0x2000 (base) */
	RING_EXECLIST_CONTROL(RENDER_RING_BASE),        /* 0x2550 */
	GEN6_RPNSWREQ,					/* 0xA008 */
	GEN6_RC_VIDEO_FREQ,				/* 0xA00C */
	RING_TAIL(BLT_RING_BASE),			/* 0x22000 (base) */
	RING_EXECLIST_CONTROL(BLT_RING_BASE),		/* 0x22550 */
	RING_TAIL(GEN11_BSD_RING_BASE),			/* 0x1C0000 (base) */
	RING_EXECLIST_CONTROL(GEN11_BSD_RING_BASE),	/* 0x1C0550 */
	RING_TAIL(GEN11_BSD2_RING_BASE),		/* 0x1C4000 (base) */
	RING_EXECLIST_CONTROL(GEN11_BSD2_RING_BASE),	/* 0x1C4550 */
	RING_TAIL(GEN11_VEBOX_RING_BASE),		/* 0x1C8000 (base) */
	RING_EXECLIST_CONTROL(GEN11_VEBOX_RING_BASE),	/* 0x1C8550 */
	RING_TAIL(GEN11_BSD3_RING_BASE),		/* 0x1D0000 (base) */
	RING_EXECLIST_CONTROL(GEN11_BSD3_RING_BASE),	/* 0x1D0550 */
	RING_TAIL(GEN11_BSD4_RING_BASE),		/* 0x1D4000 (base) */
	RING_EXECLIST_CONTROL(GEN11_BSD4_RING_BASE),	/* 0x1D4550 */
	RING_TAIL(GEN11_VEBOX2_RING_BASE),		/* 0x1D8000 (base) */
	RING_EXECLIST_CONTROL(GEN11_VEBOX2_RING_BASE),	/* 0x1D8550 */
	/* TODO: Other registers are not yet used */
};

static const i915_reg_t gen12_shadowed_regs[] = {
	RING_TAIL(RENDER_RING_BASE),			/* 0x2000 (base) */
	RING_EXECLIST_CONTROL(RENDER_RING_BASE),	/* 0x2550 */
	GEN6_RPNSWREQ,					/* 0xA008 */
	GEN6_RC_VIDEO_FREQ,				/* 0xA00C */
	RING_TAIL(BLT_RING_BASE),			/* 0x22000 (base) */
	RING_EXECLIST_CONTROL(BLT_RING_BASE),		/* 0x22550 */
	RING_TAIL(GEN11_BSD_RING_BASE),			/* 0x1C0000 (base) */
	RING_EXECLIST_CONTROL(GEN11_BSD_RING_BASE),	/* 0x1C0550 */
	RING_TAIL(GEN11_BSD2_RING_BASE),		/* 0x1C4000 (base) */
	RING_EXECLIST_CONTROL(GEN11_BSD2_RING_BASE),	/* 0x1C4550 */
	RING_TAIL(GEN11_VEBOX_RING_BASE),		/* 0x1C8000 (base) */
	RING_EXECLIST_CONTROL(GEN11_VEBOX_RING_BASE),	/* 0x1C8550 */
	RING_TAIL(GEN11_BSD3_RING_BASE),		/* 0x1D0000 (base) */
	RING_EXECLIST_CONTROL(GEN11_BSD3_RING_BASE),	/* 0x1D0550 */
	RING_TAIL(GEN11_BSD4_RING_BASE),		/* 0x1D4000 (base) */
	RING_EXECLIST_CONTROL(GEN11_BSD4_RING_BASE),	/* 0x1D4550 */
	RING_TAIL(GEN11_VEBOX2_RING_BASE),		/* 0x1D8000 (base) */
	RING_EXECLIST_CONTROL(GEN11_VEBOX2_RING_BASE),	/* 0x1D8550 */
	/* TODO: Other registers are not yet used */
};

static const i915_reg_t xehp_shadowed_regs[] = {
	RING_TAIL(RENDER_RING_BASE),			/* 0x2000 (base) */
	RING_EXECLIST_CONTROL(RENDER_RING_BASE),        /* 0x2550 */
	GEN6_RPNSWREQ,					/* 0xA008 */
	GEN6_RC_VIDEO_FREQ,				/* 0xA00C */
	RING_TAIL(BLT_RING_BASE),			/* 0x22000 (base) */
	RING_EXECLIST_CONTROL(BLT_RING_BASE),		/* 0x22550 */
	RING_TAIL(GEN11_BSD_RING_BASE),			/* 0x1C0000 (base) */
	RING_EXECLIST_CONTROL(GEN11_BSD_RING_BASE),	/* 0x1C0550 */
	RING_TAIL(GEN11_BSD2_RING_BASE),		/* 0x1C4000 (base) */
	RING_EXECLIST_CONTROL(GEN11_BSD2_RING_BASE),	/* 0x1C4550 */
	RING_TAIL(GEN11_VEBOX_RING_BASE),		/* 0x1C8000 (base) */
	RING_EXECLIST_CONTROL(GEN11_VEBOX_RING_BASE),	/* 0x1C8550 */
	RING_TAIL(GEN11_BSD3_RING_BASE),		/* 0x1D0000 (base) */
	RING_EXECLIST_CONTROL(GEN11_BSD3_RING_BASE),	/* 0x1D0550 */
	RING_TAIL(GEN11_BSD4_RING_BASE),		/* 0x1D4000 (base) */
	RING_EXECLIST_CONTROL(GEN11_BSD4_RING_BASE),	/* 0x1D4550 */
	RING_TAIL(GEN11_VEBOX2_RING_BASE),		/* 0x1D8000 (base) */
	RING_EXECLIST_CONTROL(GEN11_VEBOX2_RING_BASE),	/* 0x1D8550 */
	RING_TAIL(XEHP_BSD5_RING_BASE),			/* 0x1E0000 (base) */
	RING_EXECLIST_CONTROL(XEHP_BSD5_RING_BASE),	/* 0x1E0550 */
	RING_TAIL(XEHP_BSD6_RING_BASE),			/* 0x1E4000 (base) */
	RING_EXECLIST_CONTROL(XEHP_BSD6_RING_BASE),	/* 0x1E4550 */
	RING_TAIL(XEHP_VEBOX3_RING_BASE),		/* 0x1E8000 (base) */
	RING_EXECLIST_CONTROL(XEHP_VEBOX3_RING_BASE),	/* 0x1E8550 */
	RING_TAIL(XEHP_BSD7_RING_BASE),			/* 0x1F0000 (base) */
	RING_EXECLIST_CONTROL(XEHP_BSD7_RING_BASE),	/* 0x1F0550 */
	RING_TAIL(XEHP_BSD8_RING_BASE),			/* 0x1F4000 (base) */
	RING_EXECLIST_CONTROL(XEHP_BSD8_RING_BASE),	/* 0x1F4550 */
	RING_TAIL(XEHP_VEBOX4_RING_BASE),		/* 0x1F8000 (base) */
	RING_EXECLIST_CONTROL(XEHP_VEBOX4_RING_BASE),	/* 0x1F8550 */
>>>>>>> upstream/android-13
	/* TODO: Other registers are not yet used */
};

static int mmio_reg_cmp(u32 key, const i915_reg_t *reg)
{
	u32 offset = i915_mmio_reg_offset(*reg);

	if (key < offset)
		return -1;
	else if (key > offset)
		return 1;
	else
		return 0;
}

<<<<<<< HEAD
#define __is_genX_shadowed(x) \
static bool is_gen##x##_shadowed(u32 offset) \
{ \
	const i915_reg_t *regs = gen##x##_shadowed_regs; \
	return BSEARCH(offset, regs, ARRAY_SIZE(gen##x##_shadowed_regs), \
		       mmio_reg_cmp); \
}

__is_genX_shadowed(8)
__is_genX_shadowed(11)

#define __gen8_reg_write_fw_domains(offset) \
=======
#define __is_X_shadowed(x) \
static bool is_##x##_shadowed(u32 offset) \
{ \
	const i915_reg_t *regs = x##_shadowed_regs; \
	return BSEARCH(offset, regs, ARRAY_SIZE(x##_shadowed_regs), \
		       mmio_reg_cmp); \
}

__is_X_shadowed(gen8)
__is_X_shadowed(gen11)
__is_X_shadowed(gen12)
__is_X_shadowed(xehp)

static enum forcewake_domains
gen6_reg_write_fw_domains(struct intel_uncore *uncore, i915_reg_t reg)
{
	return FORCEWAKE_RENDER;
}

#define __gen8_reg_write_fw_domains(uncore, offset) \
>>>>>>> upstream/android-13
({ \
	enum forcewake_domains __fwd; \
	if (NEEDS_FORCE_WAKE(offset) && !is_gen8_shadowed(offset)) \
		__fwd = FORCEWAKE_RENDER; \
	else \
		__fwd = 0; \
	__fwd; \
})

/* *Must* be sorted by offset ranges! See intel_fw_table_check(). */
static const struct intel_forcewake_range __chv_fw_ranges[] = {
	GEN_FW_RANGE(0x2000, 0x3fff, FORCEWAKE_RENDER),
	GEN_FW_RANGE(0x4000, 0x4fff, FORCEWAKE_RENDER | FORCEWAKE_MEDIA),
	GEN_FW_RANGE(0x5200, 0x7fff, FORCEWAKE_RENDER),
	GEN_FW_RANGE(0x8000, 0x82ff, FORCEWAKE_RENDER | FORCEWAKE_MEDIA),
	GEN_FW_RANGE(0x8300, 0x84ff, FORCEWAKE_RENDER),
	GEN_FW_RANGE(0x8500, 0x85ff, FORCEWAKE_RENDER | FORCEWAKE_MEDIA),
	GEN_FW_RANGE(0x8800, 0x88ff, FORCEWAKE_MEDIA),
	GEN_FW_RANGE(0x9000, 0xafff, FORCEWAKE_RENDER | FORCEWAKE_MEDIA),
	GEN_FW_RANGE(0xb000, 0xb47f, FORCEWAKE_RENDER),
	GEN_FW_RANGE(0xd000, 0xd7ff, FORCEWAKE_MEDIA),
	GEN_FW_RANGE(0xe000, 0xe7ff, FORCEWAKE_RENDER),
	GEN_FW_RANGE(0xf000, 0xffff, FORCEWAKE_RENDER | FORCEWAKE_MEDIA),
	GEN_FW_RANGE(0x12000, 0x13fff, FORCEWAKE_MEDIA),
	GEN_FW_RANGE(0x1a000, 0x1bfff, FORCEWAKE_MEDIA),
	GEN_FW_RANGE(0x1e800, 0x1e9ff, FORCEWAKE_MEDIA),
	GEN_FW_RANGE(0x30000, 0x37fff, FORCEWAKE_MEDIA),
};

<<<<<<< HEAD
#define __fwtable_reg_write_fw_domains(offset) \
({ \
	enum forcewake_domains __fwd = 0; \
	if (NEEDS_FORCE_WAKE((offset)) && !is_gen8_shadowed(offset)) \
		__fwd = find_fw_domain(dev_priv, offset); \
	__fwd; \
})

#define __gen11_fwtable_reg_write_fw_domains(offset) \
({ \
	enum forcewake_domains __fwd = 0; \
	if (GEN11_NEEDS_FORCE_WAKE((offset)) && !is_gen11_shadowed(offset)) \
		__fwd = find_fw_domain(dev_priv, offset); \
=======
#define __fwtable_reg_write_fw_domains(uncore, offset) \
({ \
	enum forcewake_domains __fwd = 0; \
	if (NEEDS_FORCE_WAKE((offset)) && !is_gen8_shadowed(offset)) \
		__fwd = find_fw_domain(uncore, offset); \
	__fwd; \
})

#define __gen11_fwtable_reg_write_fw_domains(uncore, offset) \
({ \
	enum forcewake_domains __fwd = 0; \
	const u32 __offset = (offset); \
	if (!is_gen11_shadowed(__offset)) \
		__fwd = find_fw_domain(uncore, __offset); \
	__fwd; \
})

#define __gen12_fwtable_reg_write_fw_domains(uncore, offset) \
({ \
	enum forcewake_domains __fwd = 0; \
	const u32 __offset = (offset); \
	if (!is_gen12_shadowed(__offset)) \
		__fwd = find_fw_domain(uncore, __offset); \
	__fwd; \
})

#define __xehp_fwtable_reg_write_fw_domains(uncore, offset) \
({ \
	enum forcewake_domains __fwd = 0; \
	const u32 __offset = (offset); \
	if (!is_xehp_shadowed(__offset)) \
		__fwd = find_fw_domain(uncore, __offset); \
>>>>>>> upstream/android-13
	__fwd; \
})

/* *Must* be sorted by offset ranges! See intel_fw_table_check(). */
static const struct intel_forcewake_range __gen9_fw_ranges[] = {
<<<<<<< HEAD
	GEN_FW_RANGE(0x0, 0xaff, FORCEWAKE_BLITTER),
	GEN_FW_RANGE(0xb00, 0x1fff, 0), /* uncore range */
	GEN_FW_RANGE(0x2000, 0x26ff, FORCEWAKE_RENDER),
	GEN_FW_RANGE(0x2700, 0x2fff, FORCEWAKE_BLITTER),
	GEN_FW_RANGE(0x3000, 0x3fff, FORCEWAKE_RENDER),
	GEN_FW_RANGE(0x4000, 0x51ff, FORCEWAKE_BLITTER),
	GEN_FW_RANGE(0x5200, 0x7fff, FORCEWAKE_RENDER),
	GEN_FW_RANGE(0x8000, 0x812f, FORCEWAKE_BLITTER),
	GEN_FW_RANGE(0x8130, 0x813f, FORCEWAKE_MEDIA),
	GEN_FW_RANGE(0x8140, 0x815f, FORCEWAKE_RENDER),
	GEN_FW_RANGE(0x8160, 0x82ff, FORCEWAKE_BLITTER),
	GEN_FW_RANGE(0x8300, 0x84ff, FORCEWAKE_RENDER),
	GEN_FW_RANGE(0x8500, 0x87ff, FORCEWAKE_BLITTER),
	GEN_FW_RANGE(0x8800, 0x89ff, FORCEWAKE_MEDIA),
	GEN_FW_RANGE(0x8a00, 0x8bff, FORCEWAKE_BLITTER),
	GEN_FW_RANGE(0x8c00, 0x8cff, FORCEWAKE_RENDER),
	GEN_FW_RANGE(0x8d00, 0x93ff, FORCEWAKE_BLITTER),
	GEN_FW_RANGE(0x9400, 0x97ff, FORCEWAKE_RENDER | FORCEWAKE_MEDIA),
	GEN_FW_RANGE(0x9800, 0xafff, FORCEWAKE_BLITTER),
	GEN_FW_RANGE(0xb000, 0xb47f, FORCEWAKE_RENDER),
	GEN_FW_RANGE(0xb480, 0xcfff, FORCEWAKE_BLITTER),
	GEN_FW_RANGE(0xd000, 0xd7ff, FORCEWAKE_MEDIA),
	GEN_FW_RANGE(0xd800, 0xdfff, FORCEWAKE_BLITTER),
	GEN_FW_RANGE(0xe000, 0xe8ff, FORCEWAKE_RENDER),
	GEN_FW_RANGE(0xe900, 0x11fff, FORCEWAKE_BLITTER),
	GEN_FW_RANGE(0x12000, 0x13fff, FORCEWAKE_MEDIA),
	GEN_FW_RANGE(0x14000, 0x19fff, FORCEWAKE_BLITTER),
	GEN_FW_RANGE(0x1a000, 0x1e9ff, FORCEWAKE_MEDIA),
	GEN_FW_RANGE(0x1ea00, 0x243ff, FORCEWAKE_BLITTER),
	GEN_FW_RANGE(0x24400, 0x247ff, FORCEWAKE_RENDER),
	GEN_FW_RANGE(0x24800, 0x2ffff, FORCEWAKE_BLITTER),
=======
	GEN_FW_RANGE(0x0, 0xaff, FORCEWAKE_GT),
	GEN_FW_RANGE(0xb00, 0x1fff, 0), /* uncore range */
	GEN_FW_RANGE(0x2000, 0x26ff, FORCEWAKE_RENDER),
	GEN_FW_RANGE(0x2700, 0x2fff, FORCEWAKE_GT),
	GEN_FW_RANGE(0x3000, 0x3fff, FORCEWAKE_RENDER),
	GEN_FW_RANGE(0x4000, 0x51ff, FORCEWAKE_GT),
	GEN_FW_RANGE(0x5200, 0x7fff, FORCEWAKE_RENDER),
	GEN_FW_RANGE(0x8000, 0x812f, FORCEWAKE_GT),
	GEN_FW_RANGE(0x8130, 0x813f, FORCEWAKE_MEDIA),
	GEN_FW_RANGE(0x8140, 0x815f, FORCEWAKE_RENDER),
	GEN_FW_RANGE(0x8160, 0x82ff, FORCEWAKE_GT),
	GEN_FW_RANGE(0x8300, 0x84ff, FORCEWAKE_RENDER),
	GEN_FW_RANGE(0x8500, 0x87ff, FORCEWAKE_GT),
	GEN_FW_RANGE(0x8800, 0x89ff, FORCEWAKE_MEDIA),
	GEN_FW_RANGE(0x8a00, 0x8bff, FORCEWAKE_GT),
	GEN_FW_RANGE(0x8c00, 0x8cff, FORCEWAKE_RENDER),
	GEN_FW_RANGE(0x8d00, 0x93ff, FORCEWAKE_GT),
	GEN_FW_RANGE(0x9400, 0x97ff, FORCEWAKE_RENDER | FORCEWAKE_MEDIA),
	GEN_FW_RANGE(0x9800, 0xafff, FORCEWAKE_GT),
	GEN_FW_RANGE(0xb000, 0xb47f, FORCEWAKE_RENDER),
	GEN_FW_RANGE(0xb480, 0xcfff, FORCEWAKE_GT),
	GEN_FW_RANGE(0xd000, 0xd7ff, FORCEWAKE_MEDIA),
	GEN_FW_RANGE(0xd800, 0xdfff, FORCEWAKE_GT),
	GEN_FW_RANGE(0xe000, 0xe8ff, FORCEWAKE_RENDER),
	GEN_FW_RANGE(0xe900, 0x11fff, FORCEWAKE_GT),
	GEN_FW_RANGE(0x12000, 0x13fff, FORCEWAKE_MEDIA),
	GEN_FW_RANGE(0x14000, 0x19fff, FORCEWAKE_GT),
	GEN_FW_RANGE(0x1a000, 0x1e9ff, FORCEWAKE_MEDIA),
	GEN_FW_RANGE(0x1ea00, 0x243ff, FORCEWAKE_GT),
	GEN_FW_RANGE(0x24400, 0x247ff, FORCEWAKE_RENDER),
	GEN_FW_RANGE(0x24800, 0x2ffff, FORCEWAKE_GT),
>>>>>>> upstream/android-13
	GEN_FW_RANGE(0x30000, 0x3ffff, FORCEWAKE_MEDIA),
};

/* *Must* be sorted by offset ranges! See intel_fw_table_check(). */
static const struct intel_forcewake_range __gen11_fw_ranges[] = {
<<<<<<< HEAD
	GEN_FW_RANGE(0x0, 0xaff, FORCEWAKE_BLITTER),
	GEN_FW_RANGE(0xb00, 0x1fff, 0), /* uncore range */
	GEN_FW_RANGE(0x2000, 0x26ff, FORCEWAKE_RENDER),
	GEN_FW_RANGE(0x2700, 0x2fff, FORCEWAKE_BLITTER),
	GEN_FW_RANGE(0x3000, 0x3fff, FORCEWAKE_RENDER),
	GEN_FW_RANGE(0x4000, 0x51ff, FORCEWAKE_BLITTER),
	GEN_FW_RANGE(0x5200, 0x7fff, FORCEWAKE_RENDER),
	GEN_FW_RANGE(0x8000, 0x813f, FORCEWAKE_BLITTER),
	GEN_FW_RANGE(0x8140, 0x815f, FORCEWAKE_RENDER),
	GEN_FW_RANGE(0x8160, 0x82ff, FORCEWAKE_BLITTER),
	GEN_FW_RANGE(0x8300, 0x84ff, FORCEWAKE_RENDER),
	GEN_FW_RANGE(0x8500, 0x8bff, FORCEWAKE_BLITTER),
	GEN_FW_RANGE(0x8c00, 0x8cff, FORCEWAKE_RENDER),
	GEN_FW_RANGE(0x8d00, 0x93ff, FORCEWAKE_BLITTER),
	GEN_FW_RANGE(0x9400, 0x97ff, FORCEWAKE_ALL),
	GEN_FW_RANGE(0x9800, 0xafff, FORCEWAKE_BLITTER),
	GEN_FW_RANGE(0xb000, 0xb47f, FORCEWAKE_RENDER),
	GEN_FW_RANGE(0xb480, 0xdfff, FORCEWAKE_BLITTER),
	GEN_FW_RANGE(0xe000, 0xe8ff, FORCEWAKE_RENDER),
	GEN_FW_RANGE(0xe900, 0x243ff, FORCEWAKE_BLITTER),
	GEN_FW_RANGE(0x24400, 0x247ff, FORCEWAKE_RENDER),
	GEN_FW_RANGE(0x24800, 0x3ffff, FORCEWAKE_BLITTER),
	GEN_FW_RANGE(0x40000, 0x1bffff, 0),
	GEN_FW_RANGE(0x1c0000, 0x1c3fff, FORCEWAKE_MEDIA_VDBOX0),
	GEN_FW_RANGE(0x1c4000, 0x1c7fff, FORCEWAKE_MEDIA_VDBOX1),
	GEN_FW_RANGE(0x1c8000, 0x1cbfff, FORCEWAKE_MEDIA_VEBOX0),
	GEN_FW_RANGE(0x1cc000, 0x1cffff, FORCEWAKE_BLITTER),
	GEN_FW_RANGE(0x1d0000, 0x1d3fff, FORCEWAKE_MEDIA_VDBOX2),
	GEN_FW_RANGE(0x1d4000, 0x1d7fff, FORCEWAKE_MEDIA_VDBOX3),
	GEN_FW_RANGE(0x1d8000, 0x1dbfff, FORCEWAKE_MEDIA_VEBOX1)
};

static void
ilk_dummy_write(struct drm_i915_private *dev_priv)
=======
	GEN_FW_RANGE(0x0, 0x1fff, 0), /* uncore range */
	GEN_FW_RANGE(0x2000, 0x26ff, FORCEWAKE_RENDER),
	GEN_FW_RANGE(0x2700, 0x2fff, FORCEWAKE_GT),
	GEN_FW_RANGE(0x3000, 0x3fff, FORCEWAKE_RENDER),
	GEN_FW_RANGE(0x4000, 0x51ff, FORCEWAKE_GT),
	GEN_FW_RANGE(0x5200, 0x7fff, FORCEWAKE_RENDER),
	GEN_FW_RANGE(0x8000, 0x813f, FORCEWAKE_GT),
	GEN_FW_RANGE(0x8140, 0x815f, FORCEWAKE_RENDER),
	GEN_FW_RANGE(0x8160, 0x82ff, FORCEWAKE_GT),
	GEN_FW_RANGE(0x8300, 0x84ff, FORCEWAKE_RENDER),
	GEN_FW_RANGE(0x8500, 0x87ff, FORCEWAKE_GT),
	GEN_FW_RANGE(0x8800, 0x8bff, 0),
	GEN_FW_RANGE(0x8c00, 0x8cff, FORCEWAKE_RENDER),
	GEN_FW_RANGE(0x8d00, 0x94cf, FORCEWAKE_GT),
	GEN_FW_RANGE(0x94d0, 0x955f, FORCEWAKE_RENDER),
	GEN_FW_RANGE(0x9560, 0x95ff, 0),
	GEN_FW_RANGE(0x9600, 0xafff, FORCEWAKE_GT),
	GEN_FW_RANGE(0xb000, 0xb47f, FORCEWAKE_RENDER),
	GEN_FW_RANGE(0xb480, 0xdeff, FORCEWAKE_GT),
	GEN_FW_RANGE(0xdf00, 0xe8ff, FORCEWAKE_RENDER),
	GEN_FW_RANGE(0xe900, 0x16dff, FORCEWAKE_GT),
	GEN_FW_RANGE(0x16e00, 0x19fff, FORCEWAKE_RENDER),
	GEN_FW_RANGE(0x1a000, 0x23fff, FORCEWAKE_GT),
	GEN_FW_RANGE(0x24000, 0x2407f, 0),
	GEN_FW_RANGE(0x24080, 0x2417f, FORCEWAKE_GT),
	GEN_FW_RANGE(0x24180, 0x242ff, FORCEWAKE_RENDER),
	GEN_FW_RANGE(0x24300, 0x243ff, FORCEWAKE_GT),
	GEN_FW_RANGE(0x24400, 0x24fff, FORCEWAKE_RENDER),
	GEN_FW_RANGE(0x25000, 0x3ffff, FORCEWAKE_GT),
	GEN_FW_RANGE(0x40000, 0x1bffff, 0),
	GEN_FW_RANGE(0x1c0000, 0x1c3fff, FORCEWAKE_MEDIA_VDBOX0),
	GEN_FW_RANGE(0x1c4000, 0x1c7fff, 0),
	GEN_FW_RANGE(0x1c8000, 0x1cffff, FORCEWAKE_MEDIA_VEBOX0),
	GEN_FW_RANGE(0x1d0000, 0x1d3fff, FORCEWAKE_MEDIA_VDBOX2),
	GEN_FW_RANGE(0x1d4000, 0x1dbfff, 0)
};

/*
 * *Must* be sorted by offset ranges! See intel_fw_table_check().
 *
 * Note that the spec lists several reserved/unused ranges that don't
 * actually contain any registers.  In the table below we'll combine those
 * reserved ranges with either the preceding or following range to keep the
 * table small and lookups fast.
 */
static const struct intel_forcewake_range __gen12_fw_ranges[] = {
	GEN_FW_RANGE(0x0, 0x1fff, 0), /*
		0x0   -  0xaff: reserved
		0xb00 - 0x1fff: always on */
	GEN_FW_RANGE(0x2000, 0x26ff, FORCEWAKE_RENDER),
	GEN_FW_RANGE(0x2700, 0x27ff, FORCEWAKE_GT),
	GEN_FW_RANGE(0x2800, 0x2aff, FORCEWAKE_RENDER),
	GEN_FW_RANGE(0x2b00, 0x2fff, FORCEWAKE_GT),
	GEN_FW_RANGE(0x3000, 0x3fff, FORCEWAKE_RENDER),
	GEN_FW_RANGE(0x4000, 0x51ff, FORCEWAKE_GT), /*
		0x4000 - 0x48ff: gt
		0x4900 - 0x51ff: reserved */
	GEN_FW_RANGE(0x5200, 0x7fff, FORCEWAKE_RENDER), /*
		0x5200 - 0x53ff: render
		0x5400 - 0x54ff: reserved
		0x5500 - 0x7fff: render */
	GEN_FW_RANGE(0x8000, 0x813f, FORCEWAKE_GT),
	GEN_FW_RANGE(0x8140, 0x815f, FORCEWAKE_RENDER),
	GEN_FW_RANGE(0x8160, 0x81ff, 0), /*
		0x8160 - 0x817f: reserved
		0x8180 - 0x81ff: always on */
	GEN_FW_RANGE(0x8200, 0x82ff, FORCEWAKE_GT),
	GEN_FW_RANGE(0x8300, 0x84ff, FORCEWAKE_RENDER),
	GEN_FW_RANGE(0x8500, 0x94cf, FORCEWAKE_GT), /*
		0x8500 - 0x87ff: gt
		0x8800 - 0x8fff: reserved
		0x9000 - 0x947f: gt
		0x9480 - 0x94cf: reserved */
	GEN_FW_RANGE(0x94d0, 0x955f, FORCEWAKE_RENDER),
	GEN_FW_RANGE(0x9560, 0x97ff, 0), /*
		0x9560 - 0x95ff: always on
		0x9600 - 0x97ff: reserved */
	GEN_FW_RANGE(0x9800, 0xafff, FORCEWAKE_GT),
	GEN_FW_RANGE(0xb000, 0xb3ff, FORCEWAKE_RENDER),
	GEN_FW_RANGE(0xb400, 0xcfff, FORCEWAKE_GT), /*
		0xb400 - 0xbf7f: gt
		0xb480 - 0xbfff: reserved
		0xc000 - 0xcfff: gt */
	GEN_FW_RANGE(0xd000, 0xd7ff, 0),
	GEN_FW_RANGE(0xd800, 0xd8ff, FORCEWAKE_RENDER),
	GEN_FW_RANGE(0xd900, 0xdbff, FORCEWAKE_GT),
	GEN_FW_RANGE(0xdc00, 0xefff, FORCEWAKE_RENDER), /*
		0xdc00 - 0xddff: render
		0xde00 - 0xde7f: reserved
		0xde80 - 0xe8ff: render
		0xe900 - 0xefff: reserved */
	GEN_FW_RANGE(0xf000, 0x147ff, FORCEWAKE_GT), /*
		 0xf000 - 0xffff: gt
		0x10000 - 0x147ff: reserved */
	GEN_FW_RANGE(0x14800, 0x1ffff, FORCEWAKE_RENDER), /*
		0x14800 - 0x14fff: render
		0x15000 - 0x16dff: reserved
		0x16e00 - 0x1bfff: render
		0x1c000 - 0x1ffff: reserved */
	GEN_FW_RANGE(0x20000, 0x20fff, FORCEWAKE_MEDIA_VDBOX0),
	GEN_FW_RANGE(0x21000, 0x21fff, FORCEWAKE_MEDIA_VDBOX2),
	GEN_FW_RANGE(0x22000, 0x23fff, FORCEWAKE_GT),
	GEN_FW_RANGE(0x24000, 0x2417f, 0), /*
		0x24000 - 0x2407f: always on
		0x24080 - 0x2417f: reserved */
	GEN_FW_RANGE(0x24180, 0x249ff, FORCEWAKE_GT), /*
		0x24180 - 0x241ff: gt
		0x24200 - 0x249ff: reserved */
	GEN_FW_RANGE(0x24a00, 0x251ff, FORCEWAKE_RENDER), /*
		0x24a00 - 0x24a7f: render
		0x24a80 - 0x251ff: reserved */
	GEN_FW_RANGE(0x25200, 0x255ff, FORCEWAKE_GT), /*
		0x25200 - 0x252ff: gt
		0x25300 - 0x255ff: reserved */
	GEN_FW_RANGE(0x25600, 0x2567f, FORCEWAKE_MEDIA_VDBOX0),
	GEN_FW_RANGE(0x25680, 0x259ff, FORCEWAKE_MEDIA_VDBOX2), /*
		0x25680 - 0x256ff: VD2
		0x25700 - 0x259ff: reserved */
	GEN_FW_RANGE(0x25a00, 0x25a7f, FORCEWAKE_MEDIA_VDBOX0),
	GEN_FW_RANGE(0x25a80, 0x2ffff, FORCEWAKE_MEDIA_VDBOX2), /*
		0x25a80 - 0x25aff: VD2
		0x25b00 - 0x2ffff: reserved */
	GEN_FW_RANGE(0x30000, 0x3ffff, FORCEWAKE_GT),
	GEN_FW_RANGE(0x40000, 0x1bffff, 0),
	GEN_FW_RANGE(0x1c0000, 0x1c3fff, FORCEWAKE_MEDIA_VDBOX0), /*
		0x1c0000 - 0x1c2bff: VD0
		0x1c2c00 - 0x1c2cff: reserved
		0x1c2d00 - 0x1c2dff: VD0
		0x1c2e00 - 0x1c3eff: reserved
		0x1c3f00 - 0x1c3fff: VD0 */
	GEN_FW_RANGE(0x1c4000, 0x1c7fff, 0),
	GEN_FW_RANGE(0x1c8000, 0x1cbfff, FORCEWAKE_MEDIA_VEBOX0), /*
		0x1c8000 - 0x1ca0ff: VE0
		0x1ca100 - 0x1cbeff: reserved
		0x1cbf00 - 0x1cbfff: VE0 */
	GEN_FW_RANGE(0x1cc000, 0x1cffff, FORCEWAKE_MEDIA_VDBOX0), /*
		0x1cc000 - 0x1ccfff: VD0
		0x1cd000 - 0x1cffff: reserved */
	GEN_FW_RANGE(0x1d0000, 0x1d3fff, FORCEWAKE_MEDIA_VDBOX2), /*
		0x1d0000 - 0x1d2bff: VD2
		0x1d2c00 - 0x1d2cff: reserved
		0x1d2d00 - 0x1d2dff: VD2
		0x1d2e00 - 0x1d3eff: reserved
		0x1d3f00 - 0x1d3fff: VD2 */
};

/*
 * Graphics IP version 12.55 brings a slight change to the 0xd800 range,
 * switching it from the GT domain to the render domain.
 *
 * *Must* be sorted by offset ranges! See intel_fw_table_check().
 */
#define XEHP_FWRANGES(FW_RANGE_D800)					\
	GEN_FW_RANGE(0x0, 0x1fff, 0), /*					\
		  0x0 -  0xaff: reserved					\
		0xb00 - 0x1fff: always on */					\
	GEN_FW_RANGE(0x2000, 0x26ff, FORCEWAKE_RENDER),				\
	GEN_FW_RANGE(0x2700, 0x4aff, FORCEWAKE_GT),				\
	GEN_FW_RANGE(0x4b00, 0x51ff, 0), /*					\
		0x4b00 - 0x4fff: reserved					\
		0x5000 - 0x51ff: always on */					\
	GEN_FW_RANGE(0x5200, 0x7fff, FORCEWAKE_RENDER),				\
	GEN_FW_RANGE(0x8000, 0x813f, FORCEWAKE_GT),				\
	GEN_FW_RANGE(0x8140, 0x815f, FORCEWAKE_RENDER),				\
	GEN_FW_RANGE(0x8160, 0x81ff, 0), /*					\
		0x8160 - 0x817f: reserved					\
		0x8180 - 0x81ff: always on */					\
	GEN_FW_RANGE(0x8200, 0x82ff, FORCEWAKE_GT),				\
	GEN_FW_RANGE(0x8300, 0x84ff, FORCEWAKE_RENDER),				\
	GEN_FW_RANGE(0x8500, 0x8cff, FORCEWAKE_GT), /*				\
		0x8500 - 0x87ff: gt						\
		0x8800 - 0x8c7f: reserved					\
		0x8c80 - 0x8cff: gt (DG2 only) */				\
	GEN_FW_RANGE(0x8d00, 0x8fff, FORCEWAKE_RENDER), /*			\
		0x8d00 - 0x8dff: render (DG2 only)				\
		0x8e00 - 0x8fff: reserved */					\
	GEN_FW_RANGE(0x9000, 0x94cf, FORCEWAKE_GT), /*				\
		0x9000 - 0x947f: gt						\
		0x9480 - 0x94cf: reserved */					\
	GEN_FW_RANGE(0x94d0, 0x955f, FORCEWAKE_RENDER),				\
	GEN_FW_RANGE(0x9560, 0x967f, 0), /*					\
		0x9560 - 0x95ff: always on					\
		0x9600 - 0x967f: reserved */					\
	GEN_FW_RANGE(0x9680, 0x97ff, FORCEWAKE_RENDER), /*			\
		0x9680 - 0x96ff: render (DG2 only)				\
		0x9700 - 0x97ff: reserved */					\
	GEN_FW_RANGE(0x9800, 0xcfff, FORCEWAKE_GT), /*				\
		0x9800 - 0xb4ff: gt						\
		0xb500 - 0xbfff: reserved					\
		0xc000 - 0xcfff: gt */						\
	GEN_FW_RANGE(0xd000, 0xd7ff, 0),					\
	GEN_FW_RANGE(0xd800, 0xd87f, FW_RANGE_D800),			\
	GEN_FW_RANGE(0xd880, 0xdbff, FORCEWAKE_GT),				\
	GEN_FW_RANGE(0xdc00, 0xdcff, FORCEWAKE_RENDER),				\
	GEN_FW_RANGE(0xdd00, 0xde7f, FORCEWAKE_GT), /*				\
		0xdd00 - 0xddff: gt						\
		0xde00 - 0xde7f: reserved */					\
	GEN_FW_RANGE(0xde80, 0xe8ff, FORCEWAKE_RENDER), /*			\
		0xde80 - 0xdfff: render						\
		0xe000 - 0xe0ff: reserved					\
		0xe100 - 0xe8ff: render */					\
	GEN_FW_RANGE(0xe900, 0xffff, FORCEWAKE_GT), /*				\
		0xe900 - 0xe9ff: gt						\
		0xea00 - 0xefff: reserved					\
		0xf000 - 0xffff: gt */						\
	GEN_FW_RANGE(0x10000, 0x12fff, 0), /*					\
		0x10000 - 0x11fff: reserved					\
		0x12000 - 0x127ff: always on					\
		0x12800 - 0x12fff: reserved */					\
	GEN_FW_RANGE(0x13000, 0x131ff, FORCEWAKE_MEDIA_VDBOX0), /* DG2 only */	\
	GEN_FW_RANGE(0x13200, 0x13fff, FORCEWAKE_MEDIA_VDBOX2), /*		\
		0x13200 - 0x133ff: VD2 (DG2 only)				\
		0x13400 - 0x13fff: reserved */					\
	GEN_FW_RANGE(0x14000, 0x141ff, FORCEWAKE_MEDIA_VDBOX0), /* XEHPSDV only */	\
	GEN_FW_RANGE(0x14200, 0x143ff, FORCEWAKE_MEDIA_VDBOX2), /* XEHPSDV only */	\
	GEN_FW_RANGE(0x14400, 0x145ff, FORCEWAKE_MEDIA_VDBOX4), /* XEHPSDV only */	\
	GEN_FW_RANGE(0x14600, 0x147ff, FORCEWAKE_MEDIA_VDBOX6), /* XEHPSDV only */	\
	GEN_FW_RANGE(0x14800, 0x14fff, FORCEWAKE_RENDER),			\
	GEN_FW_RANGE(0x15000, 0x16dff, FORCEWAKE_GT), /*			\
		0x15000 - 0x15fff: gt (DG2 only)				\
		0x16000 - 0x16dff: reserved */					\
	GEN_FW_RANGE(0x16e00, 0x1ffff, FORCEWAKE_RENDER),			\
	GEN_FW_RANGE(0x20000, 0x21fff, FORCEWAKE_MEDIA_VDBOX0), /*		\
		0x20000 - 0x20fff: VD0 (XEHPSDV only)				\
		0x21000 - 0x21fff: reserved */					\
	GEN_FW_RANGE(0x22000, 0x23fff, FORCEWAKE_GT),				\
	GEN_FW_RANGE(0x24000, 0x2417f, 0), /*					\
		0x24000 - 0x2407f: always on					\
		0x24080 - 0x2417f: reserved */					\
	GEN_FW_RANGE(0x24180, 0x249ff, FORCEWAKE_GT), /*			\
		0x24180 - 0x241ff: gt						\
		0x24200 - 0x249ff: reserved */					\
	GEN_FW_RANGE(0x24a00, 0x251ff, FORCEWAKE_RENDER), /*			\
		0x24a00 - 0x24a7f: render					\
		0x24a80 - 0x251ff: reserved */					\
	GEN_FW_RANGE(0x25200, 0x25fff, FORCEWAKE_GT), /*			\
		0x25200 - 0x252ff: gt						\
		0x25300 - 0x25fff: reserved */					\
	GEN_FW_RANGE(0x26000, 0x2ffff, FORCEWAKE_RENDER), /*			\
		0x26000 - 0x27fff: render					\
		0x28000 - 0x29fff: reserved					\
		0x2a000 - 0x2ffff: undocumented */				\
	GEN_FW_RANGE(0x30000, 0x3ffff, FORCEWAKE_GT),				\
	GEN_FW_RANGE(0x40000, 0x1bffff, 0),					\
	GEN_FW_RANGE(0x1c0000, 0x1c3fff, FORCEWAKE_MEDIA_VDBOX0), /*		\
		0x1c0000 - 0x1c2bff: VD0					\
		0x1c2c00 - 0x1c2cff: reserved					\
		0x1c2d00 - 0x1c2dff: VD0					\
		0x1c2e00 - 0x1c3eff: VD0 (DG2 only)				\
		0x1c3f00 - 0x1c3fff: VD0 */					\
	GEN_FW_RANGE(0x1c4000, 0x1c7fff, FORCEWAKE_MEDIA_VDBOX1), /*		\
		0x1c4000 - 0x1c6bff: VD1					\
		0x1c6c00 - 0x1c6cff: reserved					\
		0x1c6d00 - 0x1c6dff: VD1					\
		0x1c6e00 - 0x1c7fff: reserved */				\
	GEN_FW_RANGE(0x1c8000, 0x1cbfff, FORCEWAKE_MEDIA_VEBOX0), /*		\
		0x1c8000 - 0x1ca0ff: VE0					\
		0x1ca100 - 0x1cbfff: reserved */				\
	GEN_FW_RANGE(0x1cc000, 0x1ccfff, FORCEWAKE_MEDIA_VDBOX0),		\
	GEN_FW_RANGE(0x1cd000, 0x1cdfff, FORCEWAKE_MEDIA_VDBOX2),		\
	GEN_FW_RANGE(0x1ce000, 0x1cefff, FORCEWAKE_MEDIA_VDBOX4),		\
	GEN_FW_RANGE(0x1cf000, 0x1cffff, FORCEWAKE_MEDIA_VDBOX6),		\
	GEN_FW_RANGE(0x1d0000, 0x1d3fff, FORCEWAKE_MEDIA_VDBOX2), /*		\
		0x1d0000 - 0x1d2bff: VD2					\
		0x1d2c00 - 0x1d2cff: reserved					\
		0x1d2d00 - 0x1d2dff: VD2					\
		0x1d2e00 - 0x1d3dff: VD2 (DG2 only)				\
		0x1d3e00 - 0x1d3eff: reserved					\
		0x1d3f00 - 0x1d3fff: VD2 */					\
	GEN_FW_RANGE(0x1d4000, 0x1d7fff, FORCEWAKE_MEDIA_VDBOX3), /*		\
		0x1d4000 - 0x1d6bff: VD3					\
		0x1d6c00 - 0x1d6cff: reserved					\
		0x1d6d00 - 0x1d6dff: VD3					\
		0x1d6e00 - 0x1d7fff: reserved */				\
	GEN_FW_RANGE(0x1d8000, 0x1dffff, FORCEWAKE_MEDIA_VEBOX1), /*		\
		0x1d8000 - 0x1da0ff: VE1					\
		0x1da100 - 0x1dffff: reserved */				\
	GEN_FW_RANGE(0x1e0000, 0x1e3fff, FORCEWAKE_MEDIA_VDBOX4), /*		\
		0x1e0000 - 0x1e2bff: VD4					\
		0x1e2c00 - 0x1e2cff: reserved					\
		0x1e2d00 - 0x1e2dff: VD4					\
		0x1e2e00 - 0x1e3eff: reserved					\
		0x1e3f00 - 0x1e3fff: VD4 */					\
	GEN_FW_RANGE(0x1e4000, 0x1e7fff, FORCEWAKE_MEDIA_VDBOX5), /*		\
		0x1e4000 - 0x1e6bff: VD5					\
		0x1e6c00 - 0x1e6cff: reserved					\
		0x1e6d00 - 0x1e6dff: VD5					\
		0x1e6e00 - 0x1e7fff: reserved */				\
	GEN_FW_RANGE(0x1e8000, 0x1effff, FORCEWAKE_MEDIA_VEBOX2), /*		\
		0x1e8000 - 0x1ea0ff: VE2					\
		0x1ea100 - 0x1effff: reserved */				\
	GEN_FW_RANGE(0x1f0000, 0x1f3fff, FORCEWAKE_MEDIA_VDBOX6), /*		\
		0x1f0000 - 0x1f2bff: VD6					\
		0x1f2c00 - 0x1f2cff: reserved					\
		0x1f2d00 - 0x1f2dff: VD6					\
		0x1f2e00 - 0x1f3eff: reserved					\
		0x1f3f00 - 0x1f3fff: VD6 */					\
	GEN_FW_RANGE(0x1f4000, 0x1f7fff, FORCEWAKE_MEDIA_VDBOX7), /*		\
		0x1f4000 - 0x1f6bff: VD7					\
		0x1f6c00 - 0x1f6cff: reserved					\
		0x1f6d00 - 0x1f6dff: VD7					\
		0x1f6e00 - 0x1f7fff: reserved */				\
	GEN_FW_RANGE(0x1f8000, 0x1fa0ff, FORCEWAKE_MEDIA_VEBOX3),

static const struct intel_forcewake_range __xehp_fw_ranges[] = {
	XEHP_FWRANGES(FORCEWAKE_GT)
};

static const struct intel_forcewake_range __dg2_fw_ranges[] = {
	XEHP_FWRANGES(FORCEWAKE_RENDER)
};

static void
ilk_dummy_write(struct intel_uncore *uncore)
>>>>>>> upstream/android-13
{
	/* WaIssueDummyWriteToWakeupFromRC6:ilk Issue a dummy write to wake up
	 * the chip from rc6 before touching it for real. MI_MODE is masked,
	 * hence harmless to write 0 into. */
<<<<<<< HEAD
	__raw_i915_write32(dev_priv, MI_MODE, 0);
}

static void
__unclaimed_reg_debug(struct drm_i915_private *dev_priv,
=======
	__raw_uncore_write32(uncore, MI_MODE, 0);
}

static void
__unclaimed_reg_debug(struct intel_uncore *uncore,
>>>>>>> upstream/android-13
		      const i915_reg_t reg,
		      const bool read,
		      const bool before)
{
<<<<<<< HEAD
	if (WARN(check_for_unclaimed_mmio(dev_priv) && !before,
		 "Unclaimed %s register 0x%x\n",
		 read ? "read from" : "write to",
		 i915_mmio_reg_offset(reg)))
		/* Only report the first N failures */
		i915_modparams.mmio_debug--;
}

static inline void
unclaimed_reg_debug(struct drm_i915_private *dev_priv,
=======
	if (drm_WARN(&uncore->i915->drm,
		     check_for_unclaimed_mmio(uncore) && !before,
		     "Unclaimed %s register 0x%x\n",
		     read ? "read from" : "write to",
		     i915_mmio_reg_offset(reg)))
		/* Only report the first N failures */
		uncore->i915->params.mmio_debug--;
}

static inline void
unclaimed_reg_debug(struct intel_uncore *uncore,
>>>>>>> upstream/android-13
		    const i915_reg_t reg,
		    const bool read,
		    const bool before)
{
<<<<<<< HEAD
	if (likely(!i915_modparams.mmio_debug))
		return;

	__unclaimed_reg_debug(dev_priv, reg, read, before);
}

#define GEN2_READ_HEADER(x) \
	u##x val = 0; \
	assert_rpm_wakelock_held(dev_priv);
=======
	if (likely(!uncore->i915->params.mmio_debug))
		return;

	/* interrupts are disabled and re-enabled around uncore->lock usage */
	lockdep_assert_held(&uncore->lock);

	if (before)
		spin_lock(&uncore->debug->lock);

	__unclaimed_reg_debug(uncore, reg, read, before);

	if (!before)
		spin_unlock(&uncore->debug->lock);
}

#define __vgpu_read(x) \
static u##x \
vgpu_read##x(struct intel_uncore *uncore, i915_reg_t reg, bool trace) { \
	u##x val = __raw_uncore_read##x(uncore, reg); \
	trace_i915_reg_rw(false, reg, val, sizeof(val), trace); \
	return val; \
}
__vgpu_read(8)
__vgpu_read(16)
__vgpu_read(32)
__vgpu_read(64)

#define GEN2_READ_HEADER(x) \
	u##x val = 0; \
	assert_rpm_wakelock_held(uncore->rpm);
>>>>>>> upstream/android-13

#define GEN2_READ_FOOTER \
	trace_i915_reg_rw(false, reg, val, sizeof(val), trace); \
	return val

#define __gen2_read(x) \
static u##x \
<<<<<<< HEAD
gen2_read##x(struct drm_i915_private *dev_priv, i915_reg_t reg, bool trace) { \
	GEN2_READ_HEADER(x); \
	val = __raw_i915_read##x(dev_priv, reg); \
=======
gen2_read##x(struct intel_uncore *uncore, i915_reg_t reg, bool trace) { \
	GEN2_READ_HEADER(x); \
	val = __raw_uncore_read##x(uncore, reg); \
>>>>>>> upstream/android-13
	GEN2_READ_FOOTER; \
}

#define __gen5_read(x) \
static u##x \
<<<<<<< HEAD
gen5_read##x(struct drm_i915_private *dev_priv, i915_reg_t reg, bool trace) { \
	GEN2_READ_HEADER(x); \
	ilk_dummy_write(dev_priv); \
	val = __raw_i915_read##x(dev_priv, reg); \
=======
gen5_read##x(struct intel_uncore *uncore, i915_reg_t reg, bool trace) { \
	GEN2_READ_HEADER(x); \
	ilk_dummy_write(uncore); \
	val = __raw_uncore_read##x(uncore, reg); \
>>>>>>> upstream/android-13
	GEN2_READ_FOOTER; \
}

__gen5_read(8)
__gen5_read(16)
__gen5_read(32)
__gen5_read(64)
__gen2_read(8)
__gen2_read(16)
__gen2_read(32)
__gen2_read(64)

#undef __gen5_read
#undef __gen2_read

#undef GEN2_READ_FOOTER
#undef GEN2_READ_HEADER

#define GEN6_READ_HEADER(x) \
	u32 offset = i915_mmio_reg_offset(reg); \
	unsigned long irqflags; \
	u##x val = 0; \
<<<<<<< HEAD
	assert_rpm_wakelock_held(dev_priv); \
	spin_lock_irqsave(&dev_priv->uncore.lock, irqflags); \
	unclaimed_reg_debug(dev_priv, reg, true, true)

#define GEN6_READ_FOOTER \
	unclaimed_reg_debug(dev_priv, reg, true, false); \
	spin_unlock_irqrestore(&dev_priv->uncore.lock, irqflags); \
	trace_i915_reg_rw(false, reg, val, sizeof(val), trace); \
	return val

static noinline void ___force_wake_auto(struct drm_i915_private *dev_priv,
=======
	assert_rpm_wakelock_held(uncore->rpm); \
	spin_lock_irqsave(&uncore->lock, irqflags); \
	unclaimed_reg_debug(uncore, reg, true, true)

#define GEN6_READ_FOOTER \
	unclaimed_reg_debug(uncore, reg, true, false); \
	spin_unlock_irqrestore(&uncore->lock, irqflags); \
	trace_i915_reg_rw(false, reg, val, sizeof(val), trace); \
	return val

static noinline void ___force_wake_auto(struct intel_uncore *uncore,
>>>>>>> upstream/android-13
					enum forcewake_domains fw_domains)
{
	struct intel_uncore_forcewake_domain *domain;
	unsigned int tmp;

<<<<<<< HEAD
	GEM_BUG_ON(fw_domains & ~dev_priv->uncore.fw_domains);

	for_each_fw_domain_masked(domain, fw_domains, dev_priv, tmp)
		fw_domain_arm_timer(domain);

	dev_priv->uncore.funcs.force_wake_get(dev_priv, fw_domains);
}

static inline void __force_wake_auto(struct drm_i915_private *dev_priv,
				     enum forcewake_domains fw_domains)
{
	if (WARN_ON(!fw_domains))
		return;

	/* Turn on all requested but inactive supported forcewake domains. */
	fw_domains &= dev_priv->uncore.fw_domains;
	fw_domains &= ~dev_priv->uncore.fw_domains_active;

	if (fw_domains)
		___force_wake_auto(dev_priv, fw_domains);
=======
	GEM_BUG_ON(fw_domains & ~uncore->fw_domains);

	for_each_fw_domain_masked(domain, fw_domains, uncore, tmp)
		fw_domain_arm_timer(domain);

	uncore->funcs.force_wake_get(uncore, fw_domains);
}

static inline void __force_wake_auto(struct intel_uncore *uncore,
				     enum forcewake_domains fw_domains)
{
	GEM_BUG_ON(!fw_domains);

	/* Turn on all requested but inactive supported forcewake domains. */
	fw_domains &= uncore->fw_domains;
	fw_domains &= ~uncore->fw_domains_active;

	if (fw_domains)
		___force_wake_auto(uncore, fw_domains);
>>>>>>> upstream/android-13
}

#define __gen_read(func, x) \
static u##x \
<<<<<<< HEAD
func##_read##x(struct drm_i915_private *dev_priv, i915_reg_t reg, bool trace) { \
	enum forcewake_domains fw_engine; \
	GEN6_READ_HEADER(x); \
	fw_engine = __##func##_reg_read_fw_domains(offset); \
	if (fw_engine) \
		__force_wake_auto(dev_priv, fw_engine); \
	val = __raw_i915_read##x(dev_priv, reg); \
	GEN6_READ_FOOTER; \
}
#define __gen6_read(x) __gen_read(gen6, x)
#define __fwtable_read(x) __gen_read(fwtable, x)
#define __gen11_fwtable_read(x) __gen_read(gen11_fwtable, x)

__gen11_fwtable_read(8)
__gen11_fwtable_read(16)
__gen11_fwtable_read(32)
__gen11_fwtable_read(64)
__fwtable_read(8)
__fwtable_read(16)
__fwtable_read(32)
__fwtable_read(64)
__gen6_read(8)
__gen6_read(16)
__gen6_read(32)
__gen6_read(64)

#undef __gen11_fwtable_read
#undef __fwtable_read
#undef __gen6_read
=======
func##_read##x(struct intel_uncore *uncore, i915_reg_t reg, bool trace) { \
	enum forcewake_domains fw_engine; \
	GEN6_READ_HEADER(x); \
	fw_engine = __##func##_reg_read_fw_domains(uncore, offset); \
	if (fw_engine) \
		__force_wake_auto(uncore, fw_engine); \
	val = __raw_uncore_read##x(uncore, reg); \
	GEN6_READ_FOOTER; \
}

#define __gen_reg_read_funcs(func) \
static enum forcewake_domains \
func##_reg_read_fw_domains(struct intel_uncore *uncore, i915_reg_t reg) { \
	return __##func##_reg_read_fw_domains(uncore, i915_mmio_reg_offset(reg)); \
} \
\
__gen_read(func, 8) \
__gen_read(func, 16) \
__gen_read(func, 32) \
__gen_read(func, 64)

__gen_reg_read_funcs(gen12_fwtable);
__gen_reg_read_funcs(gen11_fwtable);
__gen_reg_read_funcs(fwtable);
__gen_reg_read_funcs(gen6);

#undef __gen_reg_read_funcs
>>>>>>> upstream/android-13
#undef GEN6_READ_FOOTER
#undef GEN6_READ_HEADER

#define GEN2_WRITE_HEADER \
	trace_i915_reg_rw(true, reg, val, sizeof(val), trace); \
<<<<<<< HEAD
	assert_rpm_wakelock_held(dev_priv); \
=======
	assert_rpm_wakelock_held(uncore->rpm); \
>>>>>>> upstream/android-13

#define GEN2_WRITE_FOOTER

#define __gen2_write(x) \
static void \
<<<<<<< HEAD
gen2_write##x(struct drm_i915_private *dev_priv, i915_reg_t reg, u##x val, bool trace) { \
	GEN2_WRITE_HEADER; \
	__raw_i915_write##x(dev_priv, reg, val); \
=======
gen2_write##x(struct intel_uncore *uncore, i915_reg_t reg, u##x val, bool trace) { \
	GEN2_WRITE_HEADER; \
	__raw_uncore_write##x(uncore, reg, val); \
>>>>>>> upstream/android-13
	GEN2_WRITE_FOOTER; \
}

#define __gen5_write(x) \
static void \
<<<<<<< HEAD
gen5_write##x(struct drm_i915_private *dev_priv, i915_reg_t reg, u##x val, bool trace) { \
	GEN2_WRITE_HEADER; \
	ilk_dummy_write(dev_priv); \
	__raw_i915_write##x(dev_priv, reg, val); \
=======
gen5_write##x(struct intel_uncore *uncore, i915_reg_t reg, u##x val, bool trace) { \
	GEN2_WRITE_HEADER; \
	ilk_dummy_write(uncore); \
	__raw_uncore_write##x(uncore, reg, val); \
>>>>>>> upstream/android-13
	GEN2_WRITE_FOOTER; \
}

__gen5_write(8)
__gen5_write(16)
__gen5_write(32)
__gen2_write(8)
__gen2_write(16)
__gen2_write(32)

#undef __gen5_write
#undef __gen2_write

#undef GEN2_WRITE_FOOTER
#undef GEN2_WRITE_HEADER

#define GEN6_WRITE_HEADER \
	u32 offset = i915_mmio_reg_offset(reg); \
	unsigned long irqflags; \
	trace_i915_reg_rw(true, reg, val, sizeof(val), trace); \
<<<<<<< HEAD
	assert_rpm_wakelock_held(dev_priv); \
	spin_lock_irqsave(&dev_priv->uncore.lock, irqflags); \
	unclaimed_reg_debug(dev_priv, reg, false, true)

#define GEN6_WRITE_FOOTER \
	unclaimed_reg_debug(dev_priv, reg, false, false); \
	spin_unlock_irqrestore(&dev_priv->uncore.lock, irqflags)

#define __gen6_write(x) \
static void \
gen6_write##x(struct drm_i915_private *dev_priv, i915_reg_t reg, u##x val, bool trace) { \
	GEN6_WRITE_HEADER; \
	if (NEEDS_FORCE_WAKE(offset)) \
		__gen6_gt_wait_for_fifo(dev_priv); \
	__raw_i915_write##x(dev_priv, reg, val); \
	GEN6_WRITE_FOOTER; \
}

#define __gen_write(func, x) \
static void \
func##_write##x(struct drm_i915_private *dev_priv, i915_reg_t reg, u##x val, bool trace) { \
	enum forcewake_domains fw_engine; \
	GEN6_WRITE_HEADER; \
	fw_engine = __##func##_reg_write_fw_domains(offset); \
	if (fw_engine) \
		__force_wake_auto(dev_priv, fw_engine); \
	__raw_i915_write##x(dev_priv, reg, val); \
	GEN6_WRITE_FOOTER; \
}
#define __gen8_write(x) __gen_write(gen8, x)
#define __fwtable_write(x) __gen_write(fwtable, x)
#define __gen11_fwtable_write(x) __gen_write(gen11_fwtable, x)

__gen11_fwtable_write(8)
__gen11_fwtable_write(16)
__gen11_fwtable_write(32)
__fwtable_write(8)
__fwtable_write(16)
__fwtable_write(32)
__gen8_write(8)
__gen8_write(16)
__gen8_write(32)
=======
	assert_rpm_wakelock_held(uncore->rpm); \
	spin_lock_irqsave(&uncore->lock, irqflags); \
	unclaimed_reg_debug(uncore, reg, false, true)

#define GEN6_WRITE_FOOTER \
	unclaimed_reg_debug(uncore, reg, false, false); \
	spin_unlock_irqrestore(&uncore->lock, irqflags)

#define __gen6_write(x) \
static void \
gen6_write##x(struct intel_uncore *uncore, i915_reg_t reg, u##x val, bool trace) { \
	GEN6_WRITE_HEADER; \
	if (NEEDS_FORCE_WAKE(offset)) \
		__gen6_gt_wait_for_fifo(uncore); \
	__raw_uncore_write##x(uncore, reg, val); \
	GEN6_WRITE_FOOTER; \
}
>>>>>>> upstream/android-13
__gen6_write(8)
__gen6_write(16)
__gen6_write(32)

<<<<<<< HEAD
#undef __gen11_fwtable_write
#undef __fwtable_write
#undef __gen8_write
#undef __gen6_write
#undef GEN6_WRITE_FOOTER
#undef GEN6_WRITE_HEADER

#define ASSIGN_WRITE_MMIO_VFUNCS(i915, x) \
do { \
	(i915)->uncore.funcs.mmio_writeb = x##_write8; \
	(i915)->uncore.funcs.mmio_writew = x##_write16; \
	(i915)->uncore.funcs.mmio_writel = x##_write32; \
} while (0)

#define ASSIGN_READ_MMIO_VFUNCS(i915, x) \
do { \
	(i915)->uncore.funcs.mmio_readb = x##_read8; \
	(i915)->uncore.funcs.mmio_readw = x##_read16; \
	(i915)->uncore.funcs.mmio_readl = x##_read32; \
	(i915)->uncore.funcs.mmio_readq = x##_read64; \
} while (0)


static void fw_domain_init(struct drm_i915_private *dev_priv,
			   enum forcewake_domain_id domain_id,
			   i915_reg_t reg_set,
			   i915_reg_t reg_ack)
{
	struct intel_uncore_forcewake_domain *d;

	if (WARN_ON(domain_id >= FW_DOMAIN_ID_COUNT))
		return;

	d = &dev_priv->uncore.fw_domain[domain_id];

	WARN_ON(d->wake_count);

	WARN_ON(!i915_mmio_reg_valid(reg_set));
	WARN_ON(!i915_mmio_reg_valid(reg_ack));

	d->wake_count = 0;
	d->reg_set = reg_set;
	d->reg_ack = reg_ack;
=======
#define __gen_write(func, x) \
static void \
func##_write##x(struct intel_uncore *uncore, i915_reg_t reg, u##x val, bool trace) { \
	enum forcewake_domains fw_engine; \
	GEN6_WRITE_HEADER; \
	fw_engine = __##func##_reg_write_fw_domains(uncore, offset); \
	if (fw_engine) \
		__force_wake_auto(uncore, fw_engine); \
	__raw_uncore_write##x(uncore, reg, val); \
	GEN6_WRITE_FOOTER; \
}

#define __gen_reg_write_funcs(func) \
static enum forcewake_domains \
func##_reg_write_fw_domains(struct intel_uncore *uncore, i915_reg_t reg) { \
	return __##func##_reg_write_fw_domains(uncore, i915_mmio_reg_offset(reg)); \
} \
\
__gen_write(func, 8) \
__gen_write(func, 16) \
__gen_write(func, 32)

__gen_reg_write_funcs(xehp_fwtable);
__gen_reg_write_funcs(gen12_fwtable);
__gen_reg_write_funcs(gen11_fwtable);
__gen_reg_write_funcs(fwtable);
__gen_reg_write_funcs(gen8);

#undef __gen_reg_write_funcs
#undef GEN6_WRITE_FOOTER
#undef GEN6_WRITE_HEADER

#define __vgpu_write(x) \
static void \
vgpu_write##x(struct intel_uncore *uncore, i915_reg_t reg, u##x val, bool trace) { \
	trace_i915_reg_rw(true, reg, val, sizeof(val), trace); \
	__raw_uncore_write##x(uncore, reg, val); \
}
__vgpu_write(8)
__vgpu_write(16)
__vgpu_write(32)

#define ASSIGN_RAW_WRITE_MMIO_VFUNCS(uncore, x) \
do { \
	(uncore)->funcs.mmio_writeb = x##_write8; \
	(uncore)->funcs.mmio_writew = x##_write16; \
	(uncore)->funcs.mmio_writel = x##_write32; \
} while (0)

#define ASSIGN_RAW_READ_MMIO_VFUNCS(uncore, x) \
do { \
	(uncore)->funcs.mmio_readb = x##_read8; \
	(uncore)->funcs.mmio_readw = x##_read16; \
	(uncore)->funcs.mmio_readl = x##_read32; \
	(uncore)->funcs.mmio_readq = x##_read64; \
} while (0)

#define ASSIGN_WRITE_MMIO_VFUNCS(uncore, x) \
do { \
	ASSIGN_RAW_WRITE_MMIO_VFUNCS((uncore), x); \
	(uncore)->funcs.write_fw_domains = x##_reg_write_fw_domains; \
} while (0)

#define ASSIGN_READ_MMIO_VFUNCS(uncore, x) \
do { \
	ASSIGN_RAW_READ_MMIO_VFUNCS(uncore, x); \
	(uncore)->funcs.read_fw_domains = x##_reg_read_fw_domains; \
} while (0)

static int __fw_domain_init(struct intel_uncore *uncore,
			    enum forcewake_domain_id domain_id,
			    i915_reg_t reg_set,
			    i915_reg_t reg_ack)
{
	struct intel_uncore_forcewake_domain *d;

	GEM_BUG_ON(domain_id >= FW_DOMAIN_ID_COUNT);
	GEM_BUG_ON(uncore->fw_domain[domain_id]);

	if (i915_inject_probe_failure(uncore->i915))
		return -ENOMEM;

	d = kzalloc(sizeof(*d), GFP_KERNEL);
	if (!d)
		return -ENOMEM;

	drm_WARN_ON(&uncore->i915->drm, !i915_mmio_reg_valid(reg_set));
	drm_WARN_ON(&uncore->i915->drm, !i915_mmio_reg_valid(reg_ack));

	d->uncore = uncore;
	d->wake_count = 0;
	d->reg_set = uncore->regs + i915_mmio_reg_offset(reg_set);
	d->reg_ack = uncore->regs + i915_mmio_reg_offset(reg_ack);
>>>>>>> upstream/android-13

	d->id = domain_id;

	BUILD_BUG_ON(FORCEWAKE_RENDER != (1 << FW_DOMAIN_ID_RENDER));
<<<<<<< HEAD
	BUILD_BUG_ON(FORCEWAKE_BLITTER != (1 << FW_DOMAIN_ID_BLITTER));
=======
	BUILD_BUG_ON(FORCEWAKE_GT != (1 << FW_DOMAIN_ID_GT));
>>>>>>> upstream/android-13
	BUILD_BUG_ON(FORCEWAKE_MEDIA != (1 << FW_DOMAIN_ID_MEDIA));
	BUILD_BUG_ON(FORCEWAKE_MEDIA_VDBOX0 != (1 << FW_DOMAIN_ID_MEDIA_VDBOX0));
	BUILD_BUG_ON(FORCEWAKE_MEDIA_VDBOX1 != (1 << FW_DOMAIN_ID_MEDIA_VDBOX1));
	BUILD_BUG_ON(FORCEWAKE_MEDIA_VDBOX2 != (1 << FW_DOMAIN_ID_MEDIA_VDBOX2));
	BUILD_BUG_ON(FORCEWAKE_MEDIA_VDBOX3 != (1 << FW_DOMAIN_ID_MEDIA_VDBOX3));
<<<<<<< HEAD
	BUILD_BUG_ON(FORCEWAKE_MEDIA_VEBOX0 != (1 << FW_DOMAIN_ID_MEDIA_VEBOX0));
	BUILD_BUG_ON(FORCEWAKE_MEDIA_VEBOX1 != (1 << FW_DOMAIN_ID_MEDIA_VEBOX1));

=======
	BUILD_BUG_ON(FORCEWAKE_MEDIA_VDBOX4 != (1 << FW_DOMAIN_ID_MEDIA_VDBOX4));
	BUILD_BUG_ON(FORCEWAKE_MEDIA_VDBOX5 != (1 << FW_DOMAIN_ID_MEDIA_VDBOX5));
	BUILD_BUG_ON(FORCEWAKE_MEDIA_VDBOX6 != (1 << FW_DOMAIN_ID_MEDIA_VDBOX6));
	BUILD_BUG_ON(FORCEWAKE_MEDIA_VDBOX7 != (1 << FW_DOMAIN_ID_MEDIA_VDBOX7));
	BUILD_BUG_ON(FORCEWAKE_MEDIA_VEBOX0 != (1 << FW_DOMAIN_ID_MEDIA_VEBOX0));
	BUILD_BUG_ON(FORCEWAKE_MEDIA_VEBOX1 != (1 << FW_DOMAIN_ID_MEDIA_VEBOX1));
	BUILD_BUG_ON(FORCEWAKE_MEDIA_VEBOX2 != (1 << FW_DOMAIN_ID_MEDIA_VEBOX2));
	BUILD_BUG_ON(FORCEWAKE_MEDIA_VEBOX3 != (1 << FW_DOMAIN_ID_MEDIA_VEBOX3));
>>>>>>> upstream/android-13

	d->mask = BIT(domain_id);

	hrtimer_init(&d->timer, CLOCK_MONOTONIC, HRTIMER_MODE_REL);
	d->timer.function = intel_uncore_fw_release_timer;

<<<<<<< HEAD
	dev_priv->uncore.fw_domains |= BIT(domain_id);

	fw_domain_reset(dev_priv, d);
}

static void fw_domain_fini(struct drm_i915_private *dev_priv,
=======
	uncore->fw_domains |= BIT(domain_id);

	fw_domain_reset(d);

	uncore->fw_domain[domain_id] = d;

	return 0;
}

static void fw_domain_fini(struct intel_uncore *uncore,
>>>>>>> upstream/android-13
			   enum forcewake_domain_id domain_id)
{
	struct intel_uncore_forcewake_domain *d;

<<<<<<< HEAD
	if (WARN_ON(domain_id >= FW_DOMAIN_ID_COUNT))
		return;

	d = &dev_priv->uncore.fw_domain[domain_id];

	WARN_ON(d->wake_count);
	WARN_ON(hrtimer_cancel(&d->timer));
	memset(d, 0, sizeof(*d));

	dev_priv->uncore.fw_domains &= ~BIT(domain_id);
}

static void intel_uncore_fw_domains_init(struct drm_i915_private *dev_priv)
{
	if (INTEL_GEN(dev_priv) <= 5 || intel_vgpu_active(dev_priv))
		return;

	if (IS_GEN6(dev_priv)) {
		dev_priv->uncore.fw_reset = 0;
		dev_priv->uncore.fw_set = FORCEWAKE_KERNEL;
		dev_priv->uncore.fw_clear = 0;
	} else {
		/* WaRsClearFWBitsAtReset:bdw,skl */
		dev_priv->uncore.fw_reset = _MASKED_BIT_DISABLE(0xffff);
		dev_priv->uncore.fw_set = _MASKED_BIT_ENABLE(FORCEWAKE_KERNEL);
		dev_priv->uncore.fw_clear = _MASKED_BIT_DISABLE(FORCEWAKE_KERNEL);
	}

	if (INTEL_GEN(dev_priv) >= 11) {
		int i;

		dev_priv->uncore.funcs.force_wake_get =
			fw_domains_get_with_fallback;
		dev_priv->uncore.funcs.force_wake_put = fw_domains_put;
		fw_domain_init(dev_priv, FW_DOMAIN_ID_RENDER,
			       FORCEWAKE_RENDER_GEN9,
			       FORCEWAKE_ACK_RENDER_GEN9);
		fw_domain_init(dev_priv, FW_DOMAIN_ID_BLITTER,
			       FORCEWAKE_BLITTER_GEN9,
			       FORCEWAKE_ACK_BLITTER_GEN9);
		for (i = 0; i < I915_MAX_VCS; i++) {
			if (!HAS_ENGINE(dev_priv, _VCS(i)))
				continue;

			fw_domain_init(dev_priv, FW_DOMAIN_ID_MEDIA_VDBOX0 + i,
=======
	GEM_BUG_ON(domain_id >= FW_DOMAIN_ID_COUNT);

	d = fetch_and_zero(&uncore->fw_domain[domain_id]);
	if (!d)
		return;

	uncore->fw_domains &= ~BIT(domain_id);
	drm_WARN_ON(&uncore->i915->drm, d->wake_count);
	drm_WARN_ON(&uncore->i915->drm, hrtimer_cancel(&d->timer));
	kfree(d);
}

static void intel_uncore_fw_domains_fini(struct intel_uncore *uncore)
{
	struct intel_uncore_forcewake_domain *d;
	int tmp;

	for_each_fw_domain(d, uncore, tmp)
		fw_domain_fini(uncore, d->id);
}

static int intel_uncore_fw_domains_init(struct intel_uncore *uncore)
{
	struct drm_i915_private *i915 = uncore->i915;
	int ret = 0;

	GEM_BUG_ON(!intel_uncore_has_forcewake(uncore));

#define fw_domain_init(uncore__, id__, set__, ack__) \
	(ret ?: (ret = __fw_domain_init((uncore__), (id__), (set__), (ack__))))

	if (GRAPHICS_VER(i915) >= 11) {
		/* we'll prune the domains of missing engines later */
		intel_engine_mask_t emask = INTEL_INFO(i915)->platform_engine_mask;
		int i;

		uncore->funcs.force_wake_get = fw_domains_get_with_fallback;
		uncore->funcs.force_wake_put = fw_domains_put;
		fw_domain_init(uncore, FW_DOMAIN_ID_RENDER,
			       FORCEWAKE_RENDER_GEN9,
			       FORCEWAKE_ACK_RENDER_GEN9);
		fw_domain_init(uncore, FW_DOMAIN_ID_GT,
			       FORCEWAKE_GT_GEN9,
			       FORCEWAKE_ACK_GT_GEN9);

		for (i = 0; i < I915_MAX_VCS; i++) {
			if (!__HAS_ENGINE(emask, _VCS(i)))
				continue;

			fw_domain_init(uncore, FW_DOMAIN_ID_MEDIA_VDBOX0 + i,
>>>>>>> upstream/android-13
				       FORCEWAKE_MEDIA_VDBOX_GEN11(i),
				       FORCEWAKE_ACK_MEDIA_VDBOX_GEN11(i));
		}
		for (i = 0; i < I915_MAX_VECS; i++) {
<<<<<<< HEAD
			if (!HAS_ENGINE(dev_priv, _VECS(i)))
				continue;

			fw_domain_init(dev_priv, FW_DOMAIN_ID_MEDIA_VEBOX0 + i,
				       FORCEWAKE_MEDIA_VEBOX_GEN11(i),
				       FORCEWAKE_ACK_MEDIA_VEBOX_GEN11(i));
		}
	} else if (IS_GEN9(dev_priv) || IS_GEN10(dev_priv)) {
		dev_priv->uncore.funcs.force_wake_get =
			fw_domains_get_with_fallback;
		dev_priv->uncore.funcs.force_wake_put = fw_domains_put;
		fw_domain_init(dev_priv, FW_DOMAIN_ID_RENDER,
			       FORCEWAKE_RENDER_GEN9,
			       FORCEWAKE_ACK_RENDER_GEN9);
		fw_domain_init(dev_priv, FW_DOMAIN_ID_BLITTER,
			       FORCEWAKE_BLITTER_GEN9,
			       FORCEWAKE_ACK_BLITTER_GEN9);
		fw_domain_init(dev_priv, FW_DOMAIN_ID_MEDIA,
			       FORCEWAKE_MEDIA_GEN9, FORCEWAKE_ACK_MEDIA_GEN9);
	} else if (IS_VALLEYVIEW(dev_priv) || IS_CHERRYVIEW(dev_priv)) {
		dev_priv->uncore.funcs.force_wake_get = fw_domains_get;
		dev_priv->uncore.funcs.force_wake_put = fw_domains_put;
		fw_domain_init(dev_priv, FW_DOMAIN_ID_RENDER,
			       FORCEWAKE_VLV, FORCEWAKE_ACK_VLV);
		fw_domain_init(dev_priv, FW_DOMAIN_ID_MEDIA,
			       FORCEWAKE_MEDIA_VLV, FORCEWAKE_ACK_MEDIA_VLV);
	} else if (IS_HASWELL(dev_priv) || IS_BROADWELL(dev_priv)) {
		dev_priv->uncore.funcs.force_wake_get =
			fw_domains_get_with_thread_status;
		dev_priv->uncore.funcs.force_wake_put = fw_domains_put;
		fw_domain_init(dev_priv, FW_DOMAIN_ID_RENDER,
			       FORCEWAKE_MT, FORCEWAKE_ACK_HSW);
	} else if (IS_IVYBRIDGE(dev_priv)) {
=======
			if (!__HAS_ENGINE(emask, _VECS(i)))
				continue;

			fw_domain_init(uncore, FW_DOMAIN_ID_MEDIA_VEBOX0 + i,
				       FORCEWAKE_MEDIA_VEBOX_GEN11(i),
				       FORCEWAKE_ACK_MEDIA_VEBOX_GEN11(i));
		}
	} else if (IS_GRAPHICS_VER(i915, 9, 10)) {
		uncore->funcs.force_wake_get = fw_domains_get_with_fallback;
		uncore->funcs.force_wake_put = fw_domains_put;
		fw_domain_init(uncore, FW_DOMAIN_ID_RENDER,
			       FORCEWAKE_RENDER_GEN9,
			       FORCEWAKE_ACK_RENDER_GEN9);
		fw_domain_init(uncore, FW_DOMAIN_ID_GT,
			       FORCEWAKE_GT_GEN9,
			       FORCEWAKE_ACK_GT_GEN9);
		fw_domain_init(uncore, FW_DOMAIN_ID_MEDIA,
			       FORCEWAKE_MEDIA_GEN9, FORCEWAKE_ACK_MEDIA_GEN9);
	} else if (IS_VALLEYVIEW(i915) || IS_CHERRYVIEW(i915)) {
		uncore->funcs.force_wake_get = fw_domains_get;
		uncore->funcs.force_wake_put = fw_domains_put;
		fw_domain_init(uncore, FW_DOMAIN_ID_RENDER,
			       FORCEWAKE_VLV, FORCEWAKE_ACK_VLV);
		fw_domain_init(uncore, FW_DOMAIN_ID_MEDIA,
			       FORCEWAKE_MEDIA_VLV, FORCEWAKE_ACK_MEDIA_VLV);
	} else if (IS_HASWELL(i915) || IS_BROADWELL(i915)) {
		uncore->funcs.force_wake_get =
			fw_domains_get_with_thread_status;
		uncore->funcs.force_wake_put = fw_domains_put;
		fw_domain_init(uncore, FW_DOMAIN_ID_RENDER,
			       FORCEWAKE_MT, FORCEWAKE_ACK_HSW);
	} else if (IS_IVYBRIDGE(i915)) {
>>>>>>> upstream/android-13
		u32 ecobus;

		/* IVB configs may use multi-threaded forcewake */

		/* A small trick here - if the bios hasn't configured
		 * MT forcewake, and if the device is in RC6, then
		 * force_wake_mt_get will not wake the device and the
		 * ECOBUS read will return zero. Which will be
		 * (correctly) interpreted by the test below as MT
		 * forcewake being disabled.
		 */
<<<<<<< HEAD
		dev_priv->uncore.funcs.force_wake_get =
			fw_domains_get_with_thread_status;
		dev_priv->uncore.funcs.force_wake_put = fw_domains_put;
=======
		uncore->funcs.force_wake_get =
			fw_domains_get_with_thread_status;
		uncore->funcs.force_wake_put = fw_domains_put;
>>>>>>> upstream/android-13

		/* We need to init first for ECOBUS access and then
		 * determine later if we want to reinit, in case of MT access is
		 * not working. In this stage we don't know which flavour this
		 * ivb is, so it is better to reset also the gen6 fw registers
		 * before the ecobus check.
		 */

<<<<<<< HEAD
		__raw_i915_write32(dev_priv, FORCEWAKE, 0);
		__raw_posting_read(dev_priv, ECOBUS);

		fw_domain_init(dev_priv, FW_DOMAIN_ID_RENDER,
			       FORCEWAKE_MT, FORCEWAKE_MT_ACK);

		spin_lock_irq(&dev_priv->uncore.lock);
		fw_domains_get_with_thread_status(dev_priv, FORCEWAKE_RENDER);
		ecobus = __raw_i915_read32(dev_priv, ECOBUS);
		fw_domains_put(dev_priv, FORCEWAKE_RENDER);
		spin_unlock_irq(&dev_priv->uncore.lock);

		if (!(ecobus & FORCEWAKE_MT_ENABLE)) {
			DRM_INFO("No MT forcewake available on Ivybridge, this can result in issues\n");
			DRM_INFO("when using vblank-synced partial screen updates.\n");
			fw_domain_init(dev_priv, FW_DOMAIN_ID_RENDER,
				       FORCEWAKE, FORCEWAKE_ACK);
		}
	} else if (IS_GEN6(dev_priv)) {
		dev_priv->uncore.funcs.force_wake_get =
			fw_domains_get_with_thread_status;
		dev_priv->uncore.funcs.force_wake_put = fw_domains_put;
		fw_domain_init(dev_priv, FW_DOMAIN_ID_RENDER,
			       FORCEWAKE, FORCEWAKE_ACK);
	}

	/* All future platforms are expected to require complex power gating */
	WARN_ON(dev_priv->uncore.fw_domains == 0);
}

#define ASSIGN_FW_DOMAINS_TABLE(d) \
{ \
	dev_priv->uncore.fw_domains_table = \
			(struct intel_forcewake_range *)(d); \
	dev_priv->uncore.fw_domains_table_entries = ARRAY_SIZE((d)); \
=======
		__raw_uncore_write32(uncore, FORCEWAKE, 0);
		__raw_posting_read(uncore, ECOBUS);

		ret = __fw_domain_init(uncore, FW_DOMAIN_ID_RENDER,
				       FORCEWAKE_MT, FORCEWAKE_MT_ACK);
		if (ret)
			goto out;

		spin_lock_irq(&uncore->lock);
		fw_domains_get_with_thread_status(uncore, FORCEWAKE_RENDER);
		ecobus = __raw_uncore_read32(uncore, ECOBUS);
		fw_domains_put(uncore, FORCEWAKE_RENDER);
		spin_unlock_irq(&uncore->lock);

		if (!(ecobus & FORCEWAKE_MT_ENABLE)) {
			drm_info(&i915->drm, "No MT forcewake available on Ivybridge, this can result in issues\n");
			drm_info(&i915->drm, "when using vblank-synced partial screen updates.\n");
			fw_domain_fini(uncore, FW_DOMAIN_ID_RENDER);
			fw_domain_init(uncore, FW_DOMAIN_ID_RENDER,
				       FORCEWAKE, FORCEWAKE_ACK);
		}
	} else if (GRAPHICS_VER(i915) == 6) {
		uncore->funcs.force_wake_get =
			fw_domains_get_with_thread_status;
		uncore->funcs.force_wake_put = fw_domains_put;
		fw_domain_init(uncore, FW_DOMAIN_ID_RENDER,
			       FORCEWAKE, FORCEWAKE_ACK);
	}

#undef fw_domain_init

	/* All future platforms are expected to require complex power gating */
	drm_WARN_ON(&i915->drm, !ret && uncore->fw_domains == 0);

out:
	if (ret)
		intel_uncore_fw_domains_fini(uncore);

	return ret;
}

#define ASSIGN_FW_DOMAINS_TABLE(uncore, d) \
{ \
	(uncore)->fw_domains_table = \
			(struct intel_forcewake_range *)(d); \
	(uncore)->fw_domains_table_entries = ARRAY_SIZE((d)); \
>>>>>>> upstream/android-13
}

static int i915_pmic_bus_access_notifier(struct notifier_block *nb,
					 unsigned long action, void *data)
{
<<<<<<< HEAD
	struct drm_i915_private *dev_priv = container_of(nb,
			struct drm_i915_private, uncore.pmic_bus_access_nb);
=======
	struct intel_uncore *uncore = container_of(nb,
			struct intel_uncore, pmic_bus_access_nb);
>>>>>>> upstream/android-13

	switch (action) {
	case MBI_PMIC_BUS_ACCESS_BEGIN:
		/*
		 * forcewake all now to make sure that we don't need to do a
		 * forcewake later which on systems where this notifier gets
		 * called requires the punit to access to the shared pmic i2c
		 * bus, which will be busy after this notification, leading to:
		 * "render: timed out waiting for forcewake ack request."
		 * errors.
		 *
		 * The notifier is unregistered during intel_runtime_suspend(),
		 * so it's ok to access the HW here without holding a RPM
		 * wake reference -> disable wakeref asserts for the time of
		 * the access.
		 */
<<<<<<< HEAD
		disable_rpm_wakeref_asserts(dev_priv);
		intel_uncore_forcewake_get(dev_priv, FORCEWAKE_ALL);
		enable_rpm_wakeref_asserts(dev_priv);
		break;
	case MBI_PMIC_BUS_ACCESS_END:
		intel_uncore_forcewake_put(dev_priv, FORCEWAKE_ALL);
=======
		disable_rpm_wakeref_asserts(uncore->rpm);
		intel_uncore_forcewake_get(uncore, FORCEWAKE_ALL);
		enable_rpm_wakeref_asserts(uncore->rpm);
		break;
	case MBI_PMIC_BUS_ACCESS_END:
		intel_uncore_forcewake_put(uncore, FORCEWAKE_ALL);
>>>>>>> upstream/android-13
		break;
	}

	return NOTIFY_OK;
}

<<<<<<< HEAD
void intel_uncore_init(struct drm_i915_private *dev_priv)
{
	i915_check_vgpu(dev_priv);

	intel_uncore_edram_detect(dev_priv);
	intel_uncore_fw_domains_init(dev_priv);
	__intel_uncore_early_sanitize(dev_priv, 0);

	dev_priv->uncore.unclaimed_mmio_check = 1;
	dev_priv->uncore.pmic_bus_access_nb.notifier_call =
		i915_pmic_bus_access_notifier;

	if (IS_GEN(dev_priv, 2, 4) || intel_vgpu_active(dev_priv)) {
		ASSIGN_WRITE_MMIO_VFUNCS(dev_priv, gen2);
		ASSIGN_READ_MMIO_VFUNCS(dev_priv, gen2);
	} else if (IS_GEN5(dev_priv)) {
		ASSIGN_WRITE_MMIO_VFUNCS(dev_priv, gen5);
		ASSIGN_READ_MMIO_VFUNCS(dev_priv, gen5);
	} else if (IS_GEN(dev_priv, 6, 7)) {
		ASSIGN_WRITE_MMIO_VFUNCS(dev_priv, gen6);

		if (IS_VALLEYVIEW(dev_priv)) {
			ASSIGN_FW_DOMAINS_TABLE(__vlv_fw_ranges);
			ASSIGN_READ_MMIO_VFUNCS(dev_priv, fwtable);
		} else {
			ASSIGN_READ_MMIO_VFUNCS(dev_priv, gen6);
		}
	} else if (IS_GEN8(dev_priv)) {
		if (IS_CHERRYVIEW(dev_priv)) {
			ASSIGN_FW_DOMAINS_TABLE(__chv_fw_ranges);
			ASSIGN_WRITE_MMIO_VFUNCS(dev_priv, fwtable);
			ASSIGN_READ_MMIO_VFUNCS(dev_priv, fwtable);

		} else {
			ASSIGN_WRITE_MMIO_VFUNCS(dev_priv, gen8);
			ASSIGN_READ_MMIO_VFUNCS(dev_priv, gen6);
		}
	} else if (IS_GEN(dev_priv, 9, 10)) {
		ASSIGN_FW_DOMAINS_TABLE(__gen9_fw_ranges);
		ASSIGN_WRITE_MMIO_VFUNCS(dev_priv, fwtable);
		ASSIGN_READ_MMIO_VFUNCS(dev_priv, fwtable);
	} else {
		ASSIGN_FW_DOMAINS_TABLE(__gen11_fw_ranges);
		ASSIGN_WRITE_MMIO_VFUNCS(dev_priv, gen11_fwtable);
		ASSIGN_READ_MMIO_VFUNCS(dev_priv, gen11_fwtable);
	}

	iosf_mbi_register_pmic_bus_access_notifier(
		&dev_priv->uncore.pmic_bus_access_nb);
=======
static int uncore_mmio_setup(struct intel_uncore *uncore)
{
	struct drm_i915_private *i915 = uncore->i915;
	struct pci_dev *pdev = to_pci_dev(i915->drm.dev);
	int mmio_bar;
	int mmio_size;

	mmio_bar = GRAPHICS_VER(i915) == 2 ? 1 : 0;
	/*
	 * Before gen4, the registers and the GTT are behind different BARs.
	 * However, from gen4 onwards, the registers and the GTT are shared
	 * in the same BAR, so we want to restrict this ioremap from
	 * clobbering the GTT which we want ioremap_wc instead. Fortunately,
	 * the register BAR remains the same size for all the earlier
	 * generations up to Ironlake.
	 * For dgfx chips register range is expanded to 4MB.
	 */
	if (GRAPHICS_VER(i915) < 5)
		mmio_size = 512 * 1024;
	else if (IS_DGFX(i915))
		mmio_size = 4 * 1024 * 1024;
	else
		mmio_size = 2 * 1024 * 1024;

	uncore->regs = pci_iomap(pdev, mmio_bar, mmio_size);
	if (uncore->regs == NULL) {
		drm_err(&i915->drm, "failed to map registers\n");
		return -EIO;
	}

	return 0;
}

static void uncore_mmio_cleanup(struct intel_uncore *uncore)
{
	struct pci_dev *pdev = to_pci_dev(uncore->i915->drm.dev);

	pci_iounmap(pdev, uncore->regs);
}

void intel_uncore_init_early(struct intel_uncore *uncore,
			     struct drm_i915_private *i915)
{
	spin_lock_init(&uncore->lock);
	uncore->i915 = i915;
	uncore->rpm = &i915->runtime_pm;
	uncore->debug = &i915->mmio_debug;
}

static void uncore_raw_init(struct intel_uncore *uncore)
{
	GEM_BUG_ON(intel_uncore_has_forcewake(uncore));

	if (intel_vgpu_active(uncore->i915)) {
		ASSIGN_RAW_WRITE_MMIO_VFUNCS(uncore, vgpu);
		ASSIGN_RAW_READ_MMIO_VFUNCS(uncore, vgpu);
	} else if (GRAPHICS_VER(uncore->i915) == 5) {
		ASSIGN_RAW_WRITE_MMIO_VFUNCS(uncore, gen5);
		ASSIGN_RAW_READ_MMIO_VFUNCS(uncore, gen5);
	} else {
		ASSIGN_RAW_WRITE_MMIO_VFUNCS(uncore, gen2);
		ASSIGN_RAW_READ_MMIO_VFUNCS(uncore, gen2);
	}
}

static int uncore_forcewake_init(struct intel_uncore *uncore)
{
	struct drm_i915_private *i915 = uncore->i915;
	int ret;

	GEM_BUG_ON(!intel_uncore_has_forcewake(uncore));

	ret = intel_uncore_fw_domains_init(uncore);
	if (ret)
		return ret;
	forcewake_early_sanitize(uncore, 0);

	if (GRAPHICS_VER_FULL(i915) >= IP_VER(12, 55)) {
		ASSIGN_FW_DOMAINS_TABLE(uncore, __dg2_fw_ranges);
		ASSIGN_WRITE_MMIO_VFUNCS(uncore, xehp_fwtable);
		ASSIGN_READ_MMIO_VFUNCS(uncore, gen11_fwtable);
	} else if (GRAPHICS_VER_FULL(i915) >= IP_VER(12, 50)) {
		ASSIGN_FW_DOMAINS_TABLE(uncore, __xehp_fw_ranges);
		ASSIGN_WRITE_MMIO_VFUNCS(uncore, xehp_fwtable);
		ASSIGN_READ_MMIO_VFUNCS(uncore, gen11_fwtable);
	} else if (GRAPHICS_VER(i915) >= 12) {
		ASSIGN_FW_DOMAINS_TABLE(uncore, __gen12_fw_ranges);
		ASSIGN_WRITE_MMIO_VFUNCS(uncore, gen12_fwtable);
		ASSIGN_READ_MMIO_VFUNCS(uncore, gen12_fwtable);
	} else if (GRAPHICS_VER(i915) == 11) {
		ASSIGN_FW_DOMAINS_TABLE(uncore, __gen11_fw_ranges);
		ASSIGN_WRITE_MMIO_VFUNCS(uncore, gen11_fwtable);
		ASSIGN_READ_MMIO_VFUNCS(uncore, gen11_fwtable);
	} else if (IS_GRAPHICS_VER(i915, 9, 10)) {
		ASSIGN_FW_DOMAINS_TABLE(uncore, __gen9_fw_ranges);
		ASSIGN_WRITE_MMIO_VFUNCS(uncore, fwtable);
		ASSIGN_READ_MMIO_VFUNCS(uncore, fwtable);
	} else if (IS_CHERRYVIEW(i915)) {
		ASSIGN_FW_DOMAINS_TABLE(uncore, __chv_fw_ranges);
		ASSIGN_WRITE_MMIO_VFUNCS(uncore, fwtable);
		ASSIGN_READ_MMIO_VFUNCS(uncore, fwtable);
	} else if (GRAPHICS_VER(i915) == 8) {
		ASSIGN_WRITE_MMIO_VFUNCS(uncore, gen8);
		ASSIGN_READ_MMIO_VFUNCS(uncore, gen6);
	} else if (IS_VALLEYVIEW(i915)) {
		ASSIGN_FW_DOMAINS_TABLE(uncore, __vlv_fw_ranges);
		ASSIGN_WRITE_MMIO_VFUNCS(uncore, gen6);
		ASSIGN_READ_MMIO_VFUNCS(uncore, fwtable);
	} else if (IS_GRAPHICS_VER(i915, 6, 7)) {
		ASSIGN_WRITE_MMIO_VFUNCS(uncore, gen6);
		ASSIGN_READ_MMIO_VFUNCS(uncore, gen6);
	}

	uncore->pmic_bus_access_nb.notifier_call = i915_pmic_bus_access_notifier;
	iosf_mbi_register_pmic_bus_access_notifier(&uncore->pmic_bus_access_nb);

	return 0;
}

int intel_uncore_init_mmio(struct intel_uncore *uncore)
{
	struct drm_i915_private *i915 = uncore->i915;
	int ret;

	ret = uncore_mmio_setup(uncore);
	if (ret)
		return ret;

	/*
	 * The boot firmware initializes local memory and assesses its health.
	 * If memory training fails, the punit will have been instructed to
	 * keep the GT powered down; we won't be able to communicate with it
	 * and we should not continue with driver initialization.
	 */
	if (IS_DGFX(i915) &&
	    !(__raw_uncore_read32(uncore, GU_CNTL) & LMEM_INIT)) {
		drm_err(&i915->drm, "LMEM not initialized by firmware\n");
		return -ENODEV;
	}

	if (GRAPHICS_VER(i915) > 5 && !intel_vgpu_active(i915))
		uncore->flags |= UNCORE_HAS_FORCEWAKE;

	if (!intel_uncore_has_forcewake(uncore)) {
		uncore_raw_init(uncore);
	} else {
		ret = uncore_forcewake_init(uncore);
		if (ret)
			goto out_mmio_cleanup;
	}

	/* make sure fw funcs are set if and only if we have fw*/
	GEM_BUG_ON(intel_uncore_has_forcewake(uncore) != !!uncore->funcs.force_wake_get);
	GEM_BUG_ON(intel_uncore_has_forcewake(uncore) != !!uncore->funcs.force_wake_put);
	GEM_BUG_ON(intel_uncore_has_forcewake(uncore) != !!uncore->funcs.read_fw_domains);
	GEM_BUG_ON(intel_uncore_has_forcewake(uncore) != !!uncore->funcs.write_fw_domains);

	if (HAS_FPGA_DBG_UNCLAIMED(i915))
		uncore->flags |= UNCORE_HAS_FPGA_DBG_UNCLAIMED;

	if (IS_VALLEYVIEW(i915) || IS_CHERRYVIEW(i915))
		uncore->flags |= UNCORE_HAS_DBG_UNCLAIMED;

	if (IS_GRAPHICS_VER(i915, 6, 7))
		uncore->flags |= UNCORE_HAS_FIFO;

	/* clear out unclaimed reg detection bit */
	if (intel_uncore_unclaimed_mmio(uncore))
		drm_dbg(&i915->drm, "unclaimed mmio detected on uncore init, clearing\n");

	return 0;

out_mmio_cleanup:
	uncore_mmio_cleanup(uncore);

	return ret;
>>>>>>> upstream/android-13
}

/*
 * We might have detected that some engines are fused off after we initialized
 * the forcewake domains. Prune them, to make sure they only reference existing
 * engines.
 */
<<<<<<< HEAD
void intel_uncore_prune(struct drm_i915_private *dev_priv)
{
	if (INTEL_GEN(dev_priv) >= 11) {
		enum forcewake_domains fw_domains = dev_priv->uncore.fw_domains;
		enum forcewake_domain_id domain_id;
		int i;

		for (i = 0; i < I915_MAX_VCS; i++) {
			domain_id = FW_DOMAIN_ID_MEDIA_VDBOX0 + i;

			if (HAS_ENGINE(dev_priv, _VCS(i)))
				continue;

			if (fw_domains & BIT(domain_id))
				fw_domain_fini(dev_priv, domain_id);
		}

		for (i = 0; i < I915_MAX_VECS; i++) {
			domain_id = FW_DOMAIN_ID_MEDIA_VEBOX0 + i;

			if (HAS_ENGINE(dev_priv, _VECS(i)))
				continue;

			if (fw_domains & BIT(domain_id))
				fw_domain_fini(dev_priv, domain_id);
		}
	}
}

void intel_uncore_fini(struct drm_i915_private *dev_priv)
{
	/* Paranoia: make sure we have disabled everything before we exit. */
	intel_uncore_sanitize(dev_priv);

	iosf_mbi_punit_acquire();
	iosf_mbi_unregister_pmic_bus_access_notifier_unlocked(
		&dev_priv->uncore.pmic_bus_access_nb);
	intel_uncore_forcewake_reset(dev_priv);
	iosf_mbi_punit_release();
=======
void intel_uncore_prune_engine_fw_domains(struct intel_uncore *uncore,
					  struct intel_gt *gt)
{
	enum forcewake_domains fw_domains = uncore->fw_domains;
	enum forcewake_domain_id domain_id;
	int i;

	if (!intel_uncore_has_forcewake(uncore) || GRAPHICS_VER(uncore->i915) < 11)
		return;

	for (i = 0; i < I915_MAX_VCS; i++) {
		domain_id = FW_DOMAIN_ID_MEDIA_VDBOX0 + i;

		if (HAS_ENGINE(gt, _VCS(i)))
			continue;

		/*
		 * Starting with XeHP, the power well for an even-numbered
		 * VDBOX is also used for shared units within the
		 * media slice such as SFC.  So even if the engine
		 * itself is fused off, we still need to initialize
		 * the forcewake domain if any of the other engines
		 * in the same media slice are present.
		 */
		if (GRAPHICS_VER_FULL(uncore->i915) >= IP_VER(12, 50) && i % 2 == 0) {
			if ((i + 1 < I915_MAX_VCS) && HAS_ENGINE(gt, _VCS(i + 1)))
				continue;

			if (HAS_ENGINE(gt, _VECS(i / 2)))
				continue;
		}

		if (fw_domains & BIT(domain_id))
			fw_domain_fini(uncore, domain_id);
	}

	for (i = 0; i < I915_MAX_VECS; i++) {
		domain_id = FW_DOMAIN_ID_MEDIA_VEBOX0 + i;

		if (HAS_ENGINE(gt, _VECS(i)))
			continue;

		if (fw_domains & BIT(domain_id))
			fw_domain_fini(uncore, domain_id);
	}
}

void intel_uncore_fini_mmio(struct intel_uncore *uncore)
{
	if (intel_uncore_has_forcewake(uncore)) {
		iosf_mbi_punit_acquire();
		iosf_mbi_unregister_pmic_bus_access_notifier_unlocked(
			&uncore->pmic_bus_access_nb);
		intel_uncore_forcewake_reset(uncore);
		intel_uncore_fw_domains_fini(uncore);
		iosf_mbi_punit_release();
	}

	uncore_mmio_cleanup(uncore);
>>>>>>> upstream/android-13
}

static const struct reg_whitelist {
	i915_reg_t offset_ldw;
	i915_reg_t offset_udw;
<<<<<<< HEAD
	u16 gen_mask;
=======
	u8 min_graphics_ver;
	u8 max_graphics_ver;
>>>>>>> upstream/android-13
	u8 size;
} reg_read_whitelist[] = { {
	.offset_ldw = RING_TIMESTAMP(RENDER_RING_BASE),
	.offset_udw = RING_TIMESTAMP_UDW(RENDER_RING_BASE),
<<<<<<< HEAD
	.gen_mask = INTEL_GEN_MASK(4, 11),
=======
	.min_graphics_ver = 4,
	.max_graphics_ver = 12,
>>>>>>> upstream/android-13
	.size = 8
} };

int i915_reg_read_ioctl(struct drm_device *dev,
			void *data, struct drm_file *file)
{
<<<<<<< HEAD
	struct drm_i915_private *dev_priv = to_i915(dev);
	struct drm_i915_reg_read *reg = data;
	struct reg_whitelist const *entry;
=======
	struct drm_i915_private *i915 = to_i915(dev);
	struct intel_uncore *uncore = &i915->uncore;
	struct drm_i915_reg_read *reg = data;
	struct reg_whitelist const *entry;
	intel_wakeref_t wakeref;
>>>>>>> upstream/android-13
	unsigned int flags;
	int remain;
	int ret = 0;

	entry = reg_read_whitelist;
	remain = ARRAY_SIZE(reg_read_whitelist);
	while (remain) {
		u32 entry_offset = i915_mmio_reg_offset(entry->offset_ldw);

		GEM_BUG_ON(!is_power_of_2(entry->size));
		GEM_BUG_ON(entry->size > 8);
		GEM_BUG_ON(entry_offset & (entry->size - 1));

<<<<<<< HEAD
		if (INTEL_INFO(dev_priv)->gen_mask & entry->gen_mask &&
=======
		if (IS_GRAPHICS_VER(i915, entry->min_graphics_ver, entry->max_graphics_ver) &&
>>>>>>> upstream/android-13
		    entry_offset == (reg->offset & -entry->size))
			break;
		entry++;
		remain--;
	}

	if (!remain)
		return -EINVAL;

	flags = reg->offset & (entry->size - 1);

<<<<<<< HEAD
	intel_runtime_pm_get(dev_priv);
	if (entry->size == 8 && flags == I915_REG_READ_8B_WA)
		reg->val = I915_READ64_2x32(entry->offset_ldw,
					    entry->offset_udw);
	else if (entry->size == 8 && flags == 0)
		reg->val = I915_READ64(entry->offset_ldw);
	else if (entry->size == 4 && flags == 0)
		reg->val = I915_READ(entry->offset_ldw);
	else if (entry->size == 2 && flags == 0)
		reg->val = I915_READ16(entry->offset_ldw);
	else if (entry->size == 1 && flags == 0)
		reg->val = I915_READ8(entry->offset_ldw);
	else
		ret = -EINVAL;
	intel_runtime_pm_put(dev_priv);
=======
	with_intel_runtime_pm(&i915->runtime_pm, wakeref) {
		if (entry->size == 8 && flags == I915_REG_READ_8B_WA)
			reg->val = intel_uncore_read64_2x32(uncore,
							    entry->offset_ldw,
							    entry->offset_udw);
		else if (entry->size == 8 && flags == 0)
			reg->val = intel_uncore_read64(uncore,
						       entry->offset_ldw);
		else if (entry->size == 4 && flags == 0)
			reg->val = intel_uncore_read(uncore, entry->offset_ldw);
		else if (entry->size == 2 && flags == 0)
			reg->val = intel_uncore_read16(uncore,
						       entry->offset_ldw);
		else if (entry->size == 1 && flags == 0)
			reg->val = intel_uncore_read8(uncore,
						      entry->offset_ldw);
		else
			ret = -EINVAL;
	}
>>>>>>> upstream/android-13

	return ret;
}

<<<<<<< HEAD
static void gen3_stop_engine(struct intel_engine_cs *engine)
{
	struct drm_i915_private *dev_priv = engine->i915;
	const u32 base = engine->mmio_base;

	if (intel_engine_stop_cs(engine))
		DRM_DEBUG_DRIVER("%s: timed out on STOP_RING\n", engine->name);

	I915_WRITE_FW(RING_HEAD(base), I915_READ_FW(RING_TAIL(base)));
	POSTING_READ_FW(RING_HEAD(base)); /* paranoia */

	I915_WRITE_FW(RING_HEAD(base), 0);
	I915_WRITE_FW(RING_TAIL(base), 0);
	POSTING_READ_FW(RING_TAIL(base));

	/* The ring must be empty before it is disabled */
	I915_WRITE_FW(RING_CTL(base), 0);

	/* Check acts as a post */
	if (I915_READ_FW(RING_HEAD(base)) != 0)
		DRM_DEBUG_DRIVER("%s: ring head not parked\n",
				 engine->name);
}

static void i915_stop_engines(struct drm_i915_private *dev_priv,
			      unsigned engine_mask)
{
	struct intel_engine_cs *engine;
	enum intel_engine_id id;

	if (INTEL_GEN(dev_priv) < 3)
		return;

	for_each_engine_masked(engine, dev_priv, engine_mask, id)
		gen3_stop_engine(engine);
}

static bool i915_in_reset(struct pci_dev *pdev)
{
	u8 gdrst;

	pci_read_config_byte(pdev, I915_GDRST, &gdrst);
	return gdrst & GRDOM_RESET_STATUS;
}

static int i915_do_reset(struct drm_i915_private *dev_priv, unsigned engine_mask)
{
	struct pci_dev *pdev = dev_priv->drm.pdev;
	int err;

	/* Assert reset for at least 20 usec, and wait for acknowledgement. */
	pci_write_config_byte(pdev, I915_GDRST, GRDOM_RESET_ENABLE);
	usleep_range(50, 200);
	err = wait_for(i915_in_reset(pdev), 500);

	/* Clear the reset request. */
	pci_write_config_byte(pdev, I915_GDRST, 0);
	usleep_range(50, 200);
	if (!err)
		err = wait_for(!i915_in_reset(pdev), 500);

	return err;
}

static bool g4x_reset_complete(struct pci_dev *pdev)
{
	u8 gdrst;

	pci_read_config_byte(pdev, I915_GDRST, &gdrst);
	return (gdrst & GRDOM_RESET_ENABLE) == 0;
}

static int g33_do_reset(struct drm_i915_private *dev_priv, unsigned engine_mask)
{
	struct pci_dev *pdev = dev_priv->drm.pdev;

	pci_write_config_byte(pdev, I915_GDRST, GRDOM_RESET_ENABLE);
	return wait_for(g4x_reset_complete(pdev), 500);
}

static int g4x_do_reset(struct drm_i915_private *dev_priv, unsigned engine_mask)
{
	struct pci_dev *pdev = dev_priv->drm.pdev;
	int ret;

	/* WaVcpClkGateDisableForMediaReset:ctg,elk */
	I915_WRITE(VDECCLK_GATE_D,
		   I915_READ(VDECCLK_GATE_D) | VCP_UNIT_CLOCK_GATE_DISABLE);
	POSTING_READ(VDECCLK_GATE_D);

	pci_write_config_byte(pdev, I915_GDRST,
			      GRDOM_MEDIA | GRDOM_RESET_ENABLE);
	ret =  wait_for(g4x_reset_complete(pdev), 500);
	if (ret) {
		DRM_DEBUG_DRIVER("Wait for media reset failed\n");
		goto out;
	}

	pci_write_config_byte(pdev, I915_GDRST,
			      GRDOM_RENDER | GRDOM_RESET_ENABLE);
	ret =  wait_for(g4x_reset_complete(pdev), 500);
	if (ret) {
		DRM_DEBUG_DRIVER("Wait for render reset failed\n");
		goto out;
	}

out:
	pci_write_config_byte(pdev, I915_GDRST, 0);

	I915_WRITE(VDECCLK_GATE_D,
		   I915_READ(VDECCLK_GATE_D) & ~VCP_UNIT_CLOCK_GATE_DISABLE);
	POSTING_READ(VDECCLK_GATE_D);

	return ret;
}

static int ironlake_do_reset(struct drm_i915_private *dev_priv,
			     unsigned engine_mask)
{
	int ret;

	I915_WRITE(ILK_GDSR, ILK_GRDOM_RENDER | ILK_GRDOM_RESET_ENABLE);
	ret = intel_wait_for_register(dev_priv,
				      ILK_GDSR, ILK_GRDOM_RESET_ENABLE, 0,
				      500);
	if (ret) {
		DRM_DEBUG_DRIVER("Wait for render reset failed\n");
		goto out;
	}

	I915_WRITE(ILK_GDSR, ILK_GRDOM_MEDIA | ILK_GRDOM_RESET_ENABLE);
	ret = intel_wait_for_register(dev_priv,
				      ILK_GDSR, ILK_GRDOM_RESET_ENABLE, 0,
				      500);
	if (ret) {
		DRM_DEBUG_DRIVER("Wait for media reset failed\n");
		goto out;
	}

out:
	I915_WRITE(ILK_GDSR, 0);
	POSTING_READ(ILK_GDSR);
	return ret;
}

/* Reset the hardware domains (GENX_GRDOM_*) specified by mask */
static int gen6_hw_domain_reset(struct drm_i915_private *dev_priv,
				u32 hw_domain_mask)
{
	int err;

	/* GEN6_GDRST is not in the gt power well, no need to check
	 * for fifo space for the write or forcewake the chip for
	 * the read
	 */
	__raw_i915_write32(dev_priv, GEN6_GDRST, hw_domain_mask);

	/* Wait for the device to ack the reset requests */
	err = __intel_wait_for_register_fw(dev_priv,
					   GEN6_GDRST, hw_domain_mask, 0,
					   500, 0,
					   NULL);
	if (err)
		DRM_DEBUG_DRIVER("Wait for 0x%08x engines reset failed\n",
				 hw_domain_mask);

	return err;
}

/**
 * gen6_reset_engines - reset individual engines
 * @dev_priv: i915 device
 * @engine_mask: mask of intel_ring_flag() engines or ALL_ENGINES for full reset
 *
 * This function will reset the individual engines that are set in engine_mask.
 * If you provide ALL_ENGINES as mask, full global domain reset will be issued.
 *
 * Note: It is responsibility of the caller to handle the difference between
 * asking full domain reset versus reset for all available individual engines.
 *
 * Returns 0 on success, nonzero on error.
 */
static int gen6_reset_engines(struct drm_i915_private *dev_priv,
			      unsigned engine_mask)
{
	struct intel_engine_cs *engine;
	const u32 hw_engine_mask[I915_NUM_ENGINES] = {
		[RCS] = GEN6_GRDOM_RENDER,
		[BCS] = GEN6_GRDOM_BLT,
		[VCS] = GEN6_GRDOM_MEDIA,
		[VCS2] = GEN8_GRDOM_MEDIA2,
		[VECS] = GEN6_GRDOM_VECS,
	};
	u32 hw_mask;

	if (engine_mask == ALL_ENGINES) {
		hw_mask = GEN6_GRDOM_FULL;
	} else {
		unsigned int tmp;

		hw_mask = 0;
		for_each_engine_masked(engine, dev_priv, engine_mask, tmp)
			hw_mask |= hw_engine_mask[engine->id];
	}

	return gen6_hw_domain_reset(dev_priv, hw_mask);
}

/**
 * gen11_reset_engines - reset individual engines
 * @dev_priv: i915 device
 * @engine_mask: mask of intel_ring_flag() engines or ALL_ENGINES for full reset
 *
 * This function will reset the individual engines that are set in engine_mask.
 * If you provide ALL_ENGINES as mask, full global domain reset will be issued.
 *
 * Note: It is responsibility of the caller to handle the difference between
 * asking full domain reset versus reset for all available individual engines.
 *
 * Returns 0 on success, nonzero on error.
 */
static int gen11_reset_engines(struct drm_i915_private *dev_priv,
			       unsigned engine_mask)
{
	struct intel_engine_cs *engine;
	const u32 hw_engine_mask[I915_NUM_ENGINES] = {
		[RCS] = GEN11_GRDOM_RENDER,
		[BCS] = GEN11_GRDOM_BLT,
		[VCS] = GEN11_GRDOM_MEDIA,
		[VCS2] = GEN11_GRDOM_MEDIA2,
		[VCS3] = GEN11_GRDOM_MEDIA3,
		[VCS4] = GEN11_GRDOM_MEDIA4,
		[VECS] = GEN11_GRDOM_VECS,
		[VECS2] = GEN11_GRDOM_VECS2,
	};
	u32 hw_mask;

	BUILD_BUG_ON(VECS2 + 1 != I915_NUM_ENGINES);

	if (engine_mask == ALL_ENGINES) {
		hw_mask = GEN11_GRDOM_FULL;
	} else {
		unsigned int tmp;

		hw_mask = 0;
		for_each_engine_masked(engine, dev_priv, engine_mask, tmp)
			hw_mask |= hw_engine_mask[engine->id];
	}

	return gen6_hw_domain_reset(dev_priv, hw_mask);
}

/**
 * __intel_wait_for_register_fw - wait until register matches expected state
 * @dev_priv: the i915 device
=======
/**
 * __intel_wait_for_register_fw - wait until register matches expected state
 * @uncore: the struct intel_uncore
>>>>>>> upstream/android-13
 * @reg: the register to read
 * @mask: mask to apply to register value
 * @value: expected value
 * @fast_timeout_us: fast timeout in microsecond for atomic/tight wait
 * @slow_timeout_ms: slow timeout in millisecond
 * @out_value: optional placeholder to hold registry value
 *
 * This routine waits until the target register @reg contains the expected
 * @value after applying the @mask, i.e. it waits until ::
 *
<<<<<<< HEAD
 *     (I915_READ_FW(reg) & mask) == value
=======
 *     (intel_uncore_read_fw(uncore, reg) & mask) == value
>>>>>>> upstream/android-13
 *
 * Otherwise, the wait will timeout after @slow_timeout_ms milliseconds.
 * For atomic context @slow_timeout_ms must be zero and @fast_timeout_us
 * must be not larger than 20,0000 microseconds.
 *
 * Note that this routine assumes the caller holds forcewake asserted, it is
 * not suitable for very long waits. See intel_wait_for_register() if you
 * wish to wait without holding forcewake for the duration (i.e. you expect
 * the wait to be slow).
 *
<<<<<<< HEAD
 * Returns 0 if the register matches the desired condition, or -ETIMEOUT.
 */
int __intel_wait_for_register_fw(struct drm_i915_private *dev_priv,
=======
 * Return: 0 if the register matches the desired condition, or -ETIMEDOUT.
 */
int __intel_wait_for_register_fw(struct intel_uncore *uncore,
>>>>>>> upstream/android-13
				 i915_reg_t reg,
				 u32 mask,
				 u32 value,
				 unsigned int fast_timeout_us,
				 unsigned int slow_timeout_ms,
				 u32 *out_value)
{
<<<<<<< HEAD
	u32 uninitialized_var(reg_value);
#define done (((reg_value = I915_READ_FW(reg)) & mask) == value)
=======
	u32 reg_value = 0;
#define done (((reg_value = intel_uncore_read_fw(uncore, reg)) & mask) == value)
>>>>>>> upstream/android-13
	int ret;

	/* Catch any overuse of this function */
	might_sleep_if(slow_timeout_ms);
	GEM_BUG_ON(fast_timeout_us > 20000);
<<<<<<< HEAD
=======
	GEM_BUG_ON(!fast_timeout_us && !slow_timeout_ms);
>>>>>>> upstream/android-13

	ret = -ETIMEDOUT;
	if (fast_timeout_us && fast_timeout_us <= 20000)
		ret = _wait_for_atomic(done, fast_timeout_us, 0);
	if (ret && slow_timeout_ms)
		ret = wait_for(done, slow_timeout_ms);

	if (out_value)
		*out_value = reg_value;

	return ret;
#undef done
}

/**
 * __intel_wait_for_register - wait until register matches expected state
<<<<<<< HEAD
 * @dev_priv: the i915 device
=======
 * @uncore: the struct intel_uncore
>>>>>>> upstream/android-13
 * @reg: the register to read
 * @mask: mask to apply to register value
 * @value: expected value
 * @fast_timeout_us: fast timeout in microsecond for atomic/tight wait
 * @slow_timeout_ms: slow timeout in millisecond
 * @out_value: optional placeholder to hold registry value
 *
 * This routine waits until the target register @reg contains the expected
 * @value after applying the @mask, i.e. it waits until ::
 *
<<<<<<< HEAD
 *     (I915_READ(reg) & mask) == value
 *
 * Otherwise, the wait will timeout after @timeout_ms milliseconds.
 *
 * Returns 0 if the register matches the desired condition, or -ETIMEOUT.
 */
int __intel_wait_for_register(struct drm_i915_private *dev_priv,
			    i915_reg_t reg,
			    u32 mask,
			    u32 value,
			    unsigned int fast_timeout_us,
			    unsigned int slow_timeout_ms,
			    u32 *out_value)
{
	unsigned fw =
		intel_uncore_forcewake_for_reg(dev_priv, reg, FW_REG_READ);
=======
 *     (intel_uncore_read(uncore, reg) & mask) == value
 *
 * Otherwise, the wait will timeout after @timeout_ms milliseconds.
 *
 * Return: 0 if the register matches the desired condition, or -ETIMEDOUT.
 */
int __intel_wait_for_register(struct intel_uncore *uncore,
			      i915_reg_t reg,
			      u32 mask,
			      u32 value,
			      unsigned int fast_timeout_us,
			      unsigned int slow_timeout_ms,
			      u32 *out_value)
{
	unsigned fw =
		intel_uncore_forcewake_for_reg(uncore, reg, FW_REG_READ);
>>>>>>> upstream/android-13
	u32 reg_value;
	int ret;

	might_sleep_if(slow_timeout_ms);

<<<<<<< HEAD
	spin_lock_irq(&dev_priv->uncore.lock);
	intel_uncore_forcewake_get__locked(dev_priv, fw);

	ret = __intel_wait_for_register_fw(dev_priv,
					   reg, mask, value,
					   fast_timeout_us, 0, &reg_value);

	intel_uncore_forcewake_put__locked(dev_priv, fw);
	spin_unlock_irq(&dev_priv->uncore.lock);

	if (ret && slow_timeout_ms)
		ret = __wait_for(reg_value = I915_READ_NOTRACE(reg),
				 (reg_value & mask) == value,
				 slow_timeout_ms * 1000, 10, 1000);

=======
	spin_lock_irq(&uncore->lock);
	intel_uncore_forcewake_get__locked(uncore, fw);

	ret = __intel_wait_for_register_fw(uncore,
					   reg, mask, value,
					   fast_timeout_us, 0, &reg_value);

	intel_uncore_forcewake_put__locked(uncore, fw);
	spin_unlock_irq(&uncore->lock);

	if (ret && slow_timeout_ms)
		ret = __wait_for(reg_value = intel_uncore_read_notrace(uncore,
								       reg),
				 (reg_value & mask) == value,
				 slow_timeout_ms * 1000, 10, 1000);

	/* just trace the final value */
	trace_i915_reg_rw(false, reg, reg_value, sizeof(reg_value), true);

>>>>>>> upstream/android-13
	if (out_value)
		*out_value = reg_value;

	return ret;
}

<<<<<<< HEAD
static int gen8_reset_engine_start(struct intel_engine_cs *engine)
{
	struct drm_i915_private *dev_priv = engine->i915;
	int ret;

	I915_WRITE_FW(RING_RESET_CTL(engine->mmio_base),
		      _MASKED_BIT_ENABLE(RESET_CTL_REQUEST_RESET));

	ret = __intel_wait_for_register_fw(dev_priv,
					   RING_RESET_CTL(engine->mmio_base),
					   RESET_CTL_READY_TO_RESET,
					   RESET_CTL_READY_TO_RESET,
					   700, 0,
					   NULL);
	if (ret)
		DRM_ERROR("%s: reset request timeout\n", engine->name);
=======
bool intel_uncore_unclaimed_mmio(struct intel_uncore *uncore)
{
	bool ret;

	spin_lock_irq(&uncore->debug->lock);
	ret = check_for_unclaimed_mmio(uncore);
	spin_unlock_irq(&uncore->debug->lock);
>>>>>>> upstream/android-13

	return ret;
}

<<<<<<< HEAD
static void gen8_reset_engine_cancel(struct intel_engine_cs *engine)
{
	struct drm_i915_private *dev_priv = engine->i915;

	I915_WRITE_FW(RING_RESET_CTL(engine->mmio_base),
		      _MASKED_BIT_DISABLE(RESET_CTL_REQUEST_RESET));
}

static int gen8_reset_engines(struct drm_i915_private *dev_priv,
			      unsigned engine_mask)
{
	struct intel_engine_cs *engine;
	unsigned int tmp;
	int ret;

	for_each_engine_masked(engine, dev_priv, engine_mask, tmp) {
		if (gen8_reset_engine_start(engine)) {
			ret = -EIO;
			goto not_ready;
		}
	}

	if (INTEL_GEN(dev_priv) >= 11)
		ret = gen11_reset_engines(dev_priv, engine_mask);
	else
		ret = gen6_reset_engines(dev_priv, engine_mask);

not_ready:
	for_each_engine_masked(engine, dev_priv, engine_mask, tmp)
		gen8_reset_engine_cancel(engine);

	return ret;
}

typedef int (*reset_func)(struct drm_i915_private *, unsigned engine_mask);

static reset_func intel_get_gpu_reset(struct drm_i915_private *dev_priv)
{
	if (!i915_modparams.reset)
		return NULL;

	if (INTEL_GEN(dev_priv) >= 8)
		return gen8_reset_engines;
	else if (INTEL_GEN(dev_priv) >= 6)
		return gen6_reset_engines;
	else if (IS_GEN5(dev_priv))
		return ironlake_do_reset;
	else if (IS_G4X(dev_priv))
		return g4x_do_reset;
	else if (IS_G33(dev_priv) || IS_PINEVIEW(dev_priv))
		return g33_do_reset;
	else if (INTEL_GEN(dev_priv) >= 3)
		return i915_do_reset;
	else
		return NULL;
}

int intel_gpu_reset(struct drm_i915_private *dev_priv, unsigned engine_mask)
{
	reset_func reset = intel_get_gpu_reset(dev_priv);
	int retry;
	int ret;

	/*
	 * We want to perform per-engine reset from atomic context (e.g.
	 * softirq), which imposes the constraint that we cannot sleep.
	 * However, experience suggests that spending a bit of time waiting
	 * for a reset helps in various cases, so for a full-device reset
	 * we apply the opposite rule and wait if we want to. As we should
	 * always follow up a failed per-engine reset with a full device reset,
	 * being a little faster, stricter and more error prone for the
	 * atomic case seems an acceptable compromise.
	 *
	 * Unfortunately this leads to a bimodal routine, when the goal was
	 * to have a single reset function that worked for resetting any
	 * number of engines simultaneously.
	 */
	might_sleep_if(engine_mask == ALL_ENGINES);

	/*
	 * If the power well sleeps during the reset, the reset
	 * request may be dropped and never completes (causing -EIO).
	 */
	intel_uncore_forcewake_get(dev_priv, FORCEWAKE_ALL);
	for (retry = 0; retry < 3; retry++) {

		/*
		 * We stop engines, otherwise we might get failed reset and a
		 * dead gpu (on elk). Also as modern gpu as kbl can suffer
		 * from system hang if batchbuffer is progressing when
		 * the reset is issued, regardless of READY_TO_RESET ack.
		 * Thus assume it is best to stop engines on all gens
		 * where we have a gpu reset.
		 *
		 * WaKBLVECSSemaphoreWaitPoll:kbl (on ALL_ENGINES)
		 *
		 * WaMediaResetMainRingCleanup:ctg,elk (presumably)
		 *
		 * FIXME: Wa for more modern gens needs to be validated
		 */
		i915_stop_engines(dev_priv, engine_mask);

		ret = -ENODEV;
		if (reset) {
			GEM_TRACE("engine_mask=%x\n", engine_mask);
			ret = reset(dev_priv, engine_mask);
		}
		if (ret != -ETIMEDOUT || engine_mask != ALL_ENGINES)
			break;

		cond_resched();
	}
	intel_uncore_forcewake_put(dev_priv, FORCEWAKE_ALL);

	return ret;
}

bool intel_has_gpu_reset(struct drm_i915_private *dev_priv)
{
	return intel_get_gpu_reset(dev_priv) != NULL;
}

bool intel_has_reset_engine(struct drm_i915_private *dev_priv)
{
	return (dev_priv->info.has_reset_engine &&
		i915_modparams.reset >= 2);
}

int intel_reset_guc(struct drm_i915_private *dev_priv)
{
	u32 guc_domain = INTEL_GEN(dev_priv) >= 11 ? GEN11_GRDOM_GUC :
						     GEN9_GRDOM_GUC;
	int ret;

	GEM_BUG_ON(!HAS_GUC(dev_priv));

	intel_uncore_forcewake_get(dev_priv, FORCEWAKE_ALL);
	ret = gen6_hw_domain_reset(dev_priv, guc_domain);
	intel_uncore_forcewake_put(dev_priv, FORCEWAKE_ALL);

	return ret;
}

bool intel_uncore_unclaimed_mmio(struct drm_i915_private *dev_priv)
{
	return check_for_unclaimed_mmio(dev_priv);
}

bool
intel_uncore_arm_unclaimed_mmio_detection(struct drm_i915_private *dev_priv)
{
	if (unlikely(i915_modparams.mmio_debug ||
		     dev_priv->uncore.unclaimed_mmio_check <= 0))
		return false;

	if (unlikely(intel_uncore_unclaimed_mmio(dev_priv))) {
		DRM_DEBUG("Unclaimed register detected, "
			  "enabling oneshot unclaimed register reporting. "
			  "Please use i915.mmio_debug=N for more information.\n");
		i915_modparams.mmio_debug++;
		dev_priv->uncore.unclaimed_mmio_check--;
		return true;
	}

	return false;
}

static enum forcewake_domains
intel_uncore_forcewake_for_read(struct drm_i915_private *dev_priv,
				i915_reg_t reg)
{
	u32 offset = i915_mmio_reg_offset(reg);
	enum forcewake_domains fw_domains;

	if (INTEL_GEN(dev_priv) >= 11) {
		fw_domains = __gen11_fwtable_reg_read_fw_domains(offset);
	} else if (HAS_FWTABLE(dev_priv)) {
		fw_domains = __fwtable_reg_read_fw_domains(offset);
	} else if (INTEL_GEN(dev_priv) >= 6) {
		fw_domains = __gen6_reg_read_fw_domains(offset);
	} else {
		WARN_ON(!IS_GEN(dev_priv, 2, 5));
		fw_domains = 0;
	}

	WARN_ON(fw_domains & ~dev_priv->uncore.fw_domains);

	return fw_domains;
}

static enum forcewake_domains
intel_uncore_forcewake_for_write(struct drm_i915_private *dev_priv,
				 i915_reg_t reg)
{
	u32 offset = i915_mmio_reg_offset(reg);
	enum forcewake_domains fw_domains;

	if (INTEL_GEN(dev_priv) >= 11) {
		fw_domains = __gen11_fwtable_reg_write_fw_domains(offset);
	} else if (HAS_FWTABLE(dev_priv) && !IS_VALLEYVIEW(dev_priv)) {
		fw_domains = __fwtable_reg_write_fw_domains(offset);
	} else if (IS_GEN8(dev_priv)) {
		fw_domains = __gen8_reg_write_fw_domains(offset);
	} else if (IS_GEN(dev_priv, 6, 7)) {
		fw_domains = FORCEWAKE_RENDER;
	} else {
		WARN_ON(!IS_GEN(dev_priv, 2, 5));
		fw_domains = 0;
	}

	WARN_ON(fw_domains & ~dev_priv->uncore.fw_domains);

	return fw_domains;
=======
bool
intel_uncore_arm_unclaimed_mmio_detection(struct intel_uncore *uncore)
{
	bool ret = false;

	spin_lock_irq(&uncore->debug->lock);

	if (unlikely(uncore->debug->unclaimed_mmio_check <= 0))
		goto out;

	if (unlikely(check_for_unclaimed_mmio(uncore))) {
		if (!uncore->i915->params.mmio_debug) {
			drm_dbg(&uncore->i915->drm,
				"Unclaimed register detected, "
				"enabling oneshot unclaimed register reporting. "
				"Please use i915.mmio_debug=N for more information.\n");
			uncore->i915->params.mmio_debug++;
		}
		uncore->debug->unclaimed_mmio_check--;
		ret = true;
	}

out:
	spin_unlock_irq(&uncore->debug->lock);

	return ret;
>>>>>>> upstream/android-13
}

/**
 * intel_uncore_forcewake_for_reg - which forcewake domains are needed to access
 * 				    a register
<<<<<<< HEAD
 * @dev_priv: pointer to struct drm_i915_private
=======
 * @uncore: pointer to struct intel_uncore
>>>>>>> upstream/android-13
 * @reg: register in question
 * @op: operation bitmask of FW_REG_READ and/or FW_REG_WRITE
 *
 * Returns a set of forcewake domains required to be taken with for example
 * intel_uncore_forcewake_get for the specified register to be accessible in the
 * specified mode (read, write or read/write) with raw mmio accessors.
 *
 * NOTE: On Gen6 and Gen7 write forcewake domain (FORCEWAKE_RENDER) requires the
 * callers to do FIFO management on their own or risk losing writes.
 */
enum forcewake_domains
<<<<<<< HEAD
intel_uncore_forcewake_for_reg(struct drm_i915_private *dev_priv,
=======
intel_uncore_forcewake_for_reg(struct intel_uncore *uncore,
>>>>>>> upstream/android-13
			       i915_reg_t reg, unsigned int op)
{
	enum forcewake_domains fw_domains = 0;

<<<<<<< HEAD
	WARN_ON(!op);

	if (intel_vgpu_active(dev_priv))
		return 0;

	if (op & FW_REG_READ)
		fw_domains = intel_uncore_forcewake_for_read(dev_priv, reg);

	if (op & FW_REG_WRITE)
		fw_domains |= intel_uncore_forcewake_for_write(dev_priv, reg);
=======
	drm_WARN_ON(&uncore->i915->drm, !op);

	if (!intel_uncore_has_forcewake(uncore))
		return 0;

	if (op & FW_REG_READ)
		fw_domains = uncore->funcs.read_fw_domains(uncore, reg);

	if (op & FW_REG_WRITE)
		fw_domains |= uncore->funcs.write_fw_domains(uncore, reg);

	drm_WARN_ON(&uncore->i915->drm, fw_domains & ~uncore->fw_domains);
>>>>>>> upstream/android-13

	return fw_domains;
}

<<<<<<< HEAD
=======
u32 intel_uncore_read_with_mcr_steering_fw(struct intel_uncore *uncore,
					   i915_reg_t reg,
					   int slice, int subslice)
{
	u32 mcr_mask, mcr_ss, mcr, old_mcr, val;

	lockdep_assert_held(&uncore->lock);

	if (GRAPHICS_VER(uncore->i915) >= 11) {
		mcr_mask = GEN11_MCR_SLICE_MASK | GEN11_MCR_SUBSLICE_MASK;
		mcr_ss = GEN11_MCR_SLICE(slice) | GEN11_MCR_SUBSLICE(subslice);
	} else {
		mcr_mask = GEN8_MCR_SLICE_MASK | GEN8_MCR_SUBSLICE_MASK;
		mcr_ss = GEN8_MCR_SLICE(slice) | GEN8_MCR_SUBSLICE(subslice);
	}

	old_mcr = mcr = intel_uncore_read_fw(uncore, GEN8_MCR_SELECTOR);

	mcr &= ~mcr_mask;
	mcr |= mcr_ss;
	intel_uncore_write_fw(uncore, GEN8_MCR_SELECTOR, mcr);

	val = intel_uncore_read_fw(uncore, reg);

	mcr &= ~mcr_mask;
	mcr |= old_mcr & mcr_mask;

	intel_uncore_write_fw(uncore, GEN8_MCR_SELECTOR, mcr);

	return val;
}

u32 intel_uncore_read_with_mcr_steering(struct intel_uncore *uncore,
					i915_reg_t reg, int slice, int subslice)
{
	enum forcewake_domains fw_domains;
	u32 val;

	fw_domains = intel_uncore_forcewake_for_reg(uncore, reg,
						    FW_REG_READ);
	fw_domains |= intel_uncore_forcewake_for_reg(uncore,
						     GEN8_MCR_SELECTOR,
						     FW_REG_READ | FW_REG_WRITE);

	spin_lock_irq(&uncore->lock);
	intel_uncore_forcewake_get__locked(uncore, fw_domains);

	val = intel_uncore_read_with_mcr_steering_fw(uncore, reg, slice, subslice);

	intel_uncore_forcewake_put__locked(uncore, fw_domains);
	spin_unlock_irq(&uncore->lock);

	return val;
}

>>>>>>> upstream/android-13
#if IS_ENABLED(CONFIG_DRM_I915_SELFTEST)
#include "selftests/mock_uncore.c"
#include "selftests/intel_uncore.c"
#endif
