/*
 * Copyright © 2012 Intel Corporation
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
 * Authors:
 *    Ben Widawsky <ben@bwidawsk.net>
 *
 */

#include <linux/device.h>
#include <linux/module.h>
#include <linux/stat.h>
#include <linux/sysfs.h>
<<<<<<< HEAD
#include "intel_drv.h"
#include "i915_drv.h"
=======

#include "gt/intel_rc6.h"
#include "gt/intel_rps.h"
#include "gt/sysfs_engines.h"

#include "i915_drv.h"
#include "i915_sysfs.h"
#include "intel_pm.h"
#include "intel_sideband.h"
>>>>>>> upstream/android-13

static inline struct drm_i915_private *kdev_minor_to_i915(struct device *kdev)
{
	struct drm_minor *minor = dev_get_drvdata(kdev);
	return to_i915(minor->dev);
}

#ifdef CONFIG_PM
static u32 calc_residency(struct drm_i915_private *dev_priv,
			  i915_reg_t reg)
{
<<<<<<< HEAD
	u64 res;

	intel_runtime_pm_get(dev_priv);
	res = intel_rc6_residency_us(dev_priv, reg);
	intel_runtime_pm_put(dev_priv);
=======
	intel_wakeref_t wakeref;
	u64 res = 0;

	with_intel_runtime_pm(&dev_priv->runtime_pm, wakeref)
		res = intel_rc6_residency_us(&dev_priv->gt.rc6, reg);
>>>>>>> upstream/android-13

	return DIV_ROUND_CLOSEST_ULL(res, 1000);
}

<<<<<<< HEAD
static ssize_t
show_rc6_mask(struct device *kdev, struct device_attribute *attr, char *buf)
=======
static ssize_t rc6_enable_show(struct device *kdev,
			       struct device_attribute *attr, char *buf)
>>>>>>> upstream/android-13
{
	struct drm_i915_private *dev_priv = kdev_minor_to_i915(kdev);
	unsigned int mask;

	mask = 0;
	if (HAS_RC6(dev_priv))
		mask |= BIT(0);
	if (HAS_RC6p(dev_priv))
		mask |= BIT(1);
	if (HAS_RC6pp(dev_priv))
		mask |= BIT(2);

<<<<<<< HEAD
	return snprintf(buf, PAGE_SIZE, "%x\n", mask);
}

static ssize_t
show_rc6_ms(struct device *kdev, struct device_attribute *attr, char *buf)
{
	struct drm_i915_private *dev_priv = kdev_minor_to_i915(kdev);
	u32 rc6_residency = calc_residency(dev_priv, GEN6_GT_GFX_RC6);
	return snprintf(buf, PAGE_SIZE, "%u\n", rc6_residency);
}

static ssize_t
show_rc6p_ms(struct device *kdev, struct device_attribute *attr, char *buf)
{
	struct drm_i915_private *dev_priv = kdev_minor_to_i915(kdev);
	u32 rc6p_residency = calc_residency(dev_priv, GEN6_GT_GFX_RC6p);
	return snprintf(buf, PAGE_SIZE, "%u\n", rc6p_residency);
}

static ssize_t
show_rc6pp_ms(struct device *kdev, struct device_attribute *attr, char *buf)
{
	struct drm_i915_private *dev_priv = kdev_minor_to_i915(kdev);
	u32 rc6pp_residency = calc_residency(dev_priv, GEN6_GT_GFX_RC6pp);
	return snprintf(buf, PAGE_SIZE, "%u\n", rc6pp_residency);
}

static ssize_t
show_media_rc6_ms(struct device *kdev, struct device_attribute *attr, char *buf)
{
	struct drm_i915_private *dev_priv = kdev_minor_to_i915(kdev);
	u32 rc6_residency = calc_residency(dev_priv, VLV_GT_MEDIA_RC6);
	return snprintf(buf, PAGE_SIZE, "%u\n", rc6_residency);
}

static DEVICE_ATTR(rc6_enable, S_IRUGO, show_rc6_mask, NULL);
static DEVICE_ATTR(rc6_residency_ms, S_IRUGO, show_rc6_ms, NULL);
static DEVICE_ATTR(rc6p_residency_ms, S_IRUGO, show_rc6p_ms, NULL);
static DEVICE_ATTR(rc6pp_residency_ms, S_IRUGO, show_rc6pp_ms, NULL);
static DEVICE_ATTR(media_rc6_residency_ms, S_IRUGO, show_media_rc6_ms, NULL);
=======
	return sysfs_emit(buf, "%x\n", mask);
}

static ssize_t rc6_residency_ms_show(struct device *kdev,
				     struct device_attribute *attr, char *buf)
{
	struct drm_i915_private *dev_priv = kdev_minor_to_i915(kdev);
	u32 rc6_residency = calc_residency(dev_priv, GEN6_GT_GFX_RC6);
	return sysfs_emit(buf, "%u\n", rc6_residency);
}

static ssize_t rc6p_residency_ms_show(struct device *kdev,
				      struct device_attribute *attr, char *buf)
{
	struct drm_i915_private *dev_priv = kdev_minor_to_i915(kdev);
	u32 rc6p_residency = calc_residency(dev_priv, GEN6_GT_GFX_RC6p);
	return sysfs_emit(buf, "%u\n", rc6p_residency);
}

static ssize_t rc6pp_residency_ms_show(struct device *kdev,
				       struct device_attribute *attr, char *buf)
{
	struct drm_i915_private *dev_priv = kdev_minor_to_i915(kdev);
	u32 rc6pp_residency = calc_residency(dev_priv, GEN6_GT_GFX_RC6pp);
	return sysfs_emit(buf, "%u\n", rc6pp_residency);
}

static ssize_t media_rc6_residency_ms_show(struct device *kdev,
					   struct device_attribute *attr, char *buf)
{
	struct drm_i915_private *dev_priv = kdev_minor_to_i915(kdev);
	u32 rc6_residency = calc_residency(dev_priv, VLV_GT_MEDIA_RC6);
	return sysfs_emit(buf, "%u\n", rc6_residency);
}

static DEVICE_ATTR_RO(rc6_enable);
static DEVICE_ATTR_RO(rc6_residency_ms);
static DEVICE_ATTR_RO(rc6p_residency_ms);
static DEVICE_ATTR_RO(rc6pp_residency_ms);
static DEVICE_ATTR_RO(media_rc6_residency_ms);
>>>>>>> upstream/android-13

static struct attribute *rc6_attrs[] = {
	&dev_attr_rc6_enable.attr,
	&dev_attr_rc6_residency_ms.attr,
	NULL
};

static const struct attribute_group rc6_attr_group = {
	.name = power_group_name,
	.attrs =  rc6_attrs
};

static struct attribute *rc6p_attrs[] = {
	&dev_attr_rc6p_residency_ms.attr,
	&dev_attr_rc6pp_residency_ms.attr,
	NULL
};

static const struct attribute_group rc6p_attr_group = {
	.name = power_group_name,
	.attrs =  rc6p_attrs
};

static struct attribute *media_rc6_attrs[] = {
	&dev_attr_media_rc6_residency_ms.attr,
	NULL
};

static const struct attribute_group media_rc6_attr_group = {
	.name = power_group_name,
	.attrs =  media_rc6_attrs
};
#endif

<<<<<<< HEAD
static int l3_access_valid(struct drm_i915_private *dev_priv, loff_t offset)
{
	if (!HAS_L3_DPF(dev_priv))
		return -EPERM;

	if (offset % 4 != 0)
=======
static int l3_access_valid(struct drm_i915_private *i915, loff_t offset)
{
	if (!HAS_L3_DPF(i915))
		return -EPERM;

	if (!IS_ALIGNED(offset, sizeof(u32)))
>>>>>>> upstream/android-13
		return -EINVAL;

	if (offset >= GEN7_L3LOG_SIZE)
		return -ENXIO;

	return 0;
}

static ssize_t
i915_l3_read(struct file *filp, struct kobject *kobj,
	     struct bin_attribute *attr, char *buf,
	     loff_t offset, size_t count)
{
	struct device *kdev = kobj_to_dev(kobj);
<<<<<<< HEAD
	struct drm_i915_private *dev_priv = kdev_minor_to_i915(kdev);
	struct drm_device *dev = &dev_priv->drm;
	int slice = (int)(uintptr_t)attr->private;
	int ret;

	count = round_down(count, 4);

	ret = l3_access_valid(dev_priv, offset);
	if (ret)
		return ret;

	count = min_t(size_t, GEN7_L3LOG_SIZE - offset, count);

	ret = i915_mutex_lock_interruptible(dev);
	if (ret)
		return ret;

	if (dev_priv->l3_parity.remap_info[slice])
		memcpy(buf,
		       dev_priv->l3_parity.remap_info[slice] + (offset/4),
		       count);
	else
		memset(buf, 0, count);

	mutex_unlock(&dev->struct_mutex);
=======
	struct drm_i915_private *i915 = kdev_minor_to_i915(kdev);
	int slice = (int)(uintptr_t)attr->private;
	int ret;

	ret = l3_access_valid(i915, offset);
	if (ret)
		return ret;

	count = round_down(count, sizeof(u32));
	count = min_t(size_t, GEN7_L3LOG_SIZE - offset, count);
	memset(buf, 0, count);

	spin_lock(&i915->gem.contexts.lock);
	if (i915->l3_parity.remap_info[slice])
		memcpy(buf,
		       i915->l3_parity.remap_info[slice] + offset / sizeof(u32),
		       count);
	spin_unlock(&i915->gem.contexts.lock);
>>>>>>> upstream/android-13

	return count;
}

static ssize_t
i915_l3_write(struct file *filp, struct kobject *kobj,
	      struct bin_attribute *attr, char *buf,
	      loff_t offset, size_t count)
{
	struct device *kdev = kobj_to_dev(kobj);
<<<<<<< HEAD
	struct drm_i915_private *dev_priv = kdev_minor_to_i915(kdev);
	struct drm_device *dev = &dev_priv->drm;
	struct i915_gem_context *ctx;
	int slice = (int)(uintptr_t)attr->private;
	u32 **remap_info;
	int ret;

	ret = l3_access_valid(dev_priv, offset);
	if (ret)
		return ret;

	ret = i915_mutex_lock_interruptible(dev);
	if (ret)
		return ret;

	remap_info = &dev_priv->l3_parity.remap_info[slice];
	if (!*remap_info) {
		*remap_info = kzalloc(GEN7_L3LOG_SIZE, GFP_KERNEL);
		if (!*remap_info) {
			ret = -ENOMEM;
			goto out;
		}
	}

	/* TODO: Ideally we really want a GPU reset here to make sure errors
	 * aren't propagated. Since I cannot find a stable way to reset the GPU
	 * at this point it is left as a TODO.
	*/
	memcpy(*remap_info + (offset/4), buf, count);

	/* NB: We defer the remapping until we switch to the context */
	list_for_each_entry(ctx, &dev_priv->contexts.list, link)
		ctx->remap_slice |= (1<<slice);

	ret = count;

out:
	mutex_unlock(&dev->struct_mutex);

	return ret;
=======
	struct drm_i915_private *i915 = kdev_minor_to_i915(kdev);
	int slice = (int)(uintptr_t)attr->private;
	u32 *remap_info, *freeme = NULL;
	struct i915_gem_context *ctx;
	int ret;

	ret = l3_access_valid(i915, offset);
	if (ret)
		return ret;

	if (count < sizeof(u32))
		return -EINVAL;

	remap_info = kzalloc(GEN7_L3LOG_SIZE, GFP_KERNEL);
	if (!remap_info)
		return -ENOMEM;

	spin_lock(&i915->gem.contexts.lock);

	if (i915->l3_parity.remap_info[slice]) {
		freeme = remap_info;
		remap_info = i915->l3_parity.remap_info[slice];
	} else {
		i915->l3_parity.remap_info[slice] = remap_info;
	}

	count = round_down(count, sizeof(u32));
	memcpy(remap_info + offset / sizeof(u32), buf, count);

	/* NB: We defer the remapping until we switch to the context */
	list_for_each_entry(ctx, &i915->gem.contexts.list, link)
		ctx->remap_slice |= BIT(slice);

	spin_unlock(&i915->gem.contexts.lock);
	kfree(freeme);

	/*
	 * TODO: Ideally we really want a GPU reset here to make sure errors
	 * aren't propagated. Since I cannot find a stable way to reset the GPU
	 * at this point it is left as a TODO.
	*/

	return count;
>>>>>>> upstream/android-13
}

static const struct bin_attribute dpf_attrs = {
	.attr = {.name = "l3_parity", .mode = (S_IRUSR | S_IWUSR)},
	.size = GEN7_L3LOG_SIZE,
	.read = i915_l3_read,
	.write = i915_l3_write,
	.mmap = NULL,
	.private = (void *)0
};

static const struct bin_attribute dpf_attrs_1 = {
	.attr = {.name = "l3_parity_slice_1", .mode = (S_IRUSR | S_IWUSR)},
	.size = GEN7_L3LOG_SIZE,
	.read = i915_l3_read,
	.write = i915_l3_write,
	.mmap = NULL,
	.private = (void *)1
};

static ssize_t gt_act_freq_mhz_show(struct device *kdev,
				    struct device_attribute *attr, char *buf)
{
<<<<<<< HEAD
	struct drm_i915_private *dev_priv = kdev_minor_to_i915(kdev);
	int ret;

	intel_runtime_pm_get(dev_priv);

	mutex_lock(&dev_priv->pcu_lock);
	if (IS_VALLEYVIEW(dev_priv) || IS_CHERRYVIEW(dev_priv)) {
		u32 freq;
		freq = vlv_punit_read(dev_priv, PUNIT_REG_GPU_FREQ_STS);
		ret = intel_gpu_freq(dev_priv, (freq >> 8) & 0xff);
	} else {
		ret = intel_gpu_freq(dev_priv,
				     intel_get_cagf(dev_priv,
						    I915_READ(GEN6_RPSTAT1)));
	}
	mutex_unlock(&dev_priv->pcu_lock);

	intel_runtime_pm_put(dev_priv);

	return snprintf(buf, PAGE_SIZE, "%d\n", ret);
=======
	struct drm_i915_private *i915 = kdev_minor_to_i915(kdev);
	struct intel_rps *rps = &i915->gt.rps;

	return sysfs_emit(buf, "%d\n", intel_rps_read_actual_frequency(rps));
>>>>>>> upstream/android-13
}

static ssize_t gt_cur_freq_mhz_show(struct device *kdev,
				    struct device_attribute *attr, char *buf)
{
<<<<<<< HEAD
	struct drm_i915_private *dev_priv = kdev_minor_to_i915(kdev);

	return snprintf(buf, PAGE_SIZE, "%d\n",
			intel_gpu_freq(dev_priv,
				       dev_priv->gt_pm.rps.cur_freq));
=======
	struct drm_i915_private *i915 = kdev_minor_to_i915(kdev);
	struct intel_rps *rps = &i915->gt.rps;

	return sysfs_emit(buf, "%d\n", intel_rps_get_requested_frequency(rps));
>>>>>>> upstream/android-13
}

static ssize_t gt_boost_freq_mhz_show(struct device *kdev, struct device_attribute *attr, char *buf)
{
<<<<<<< HEAD
	struct drm_i915_private *dev_priv = kdev_minor_to_i915(kdev);

	return snprintf(buf, PAGE_SIZE, "%d\n",
			intel_gpu_freq(dev_priv,
				       dev_priv->gt_pm.rps.boost_freq));
=======
	struct drm_i915_private *i915 = kdev_minor_to_i915(kdev);
	struct intel_rps *rps = &i915->gt.rps;

	return sysfs_emit(buf, "%d\n", intel_gpu_freq(rps, rps->boost_freq));
>>>>>>> upstream/android-13
}

static ssize_t gt_boost_freq_mhz_store(struct device *kdev,
				       struct device_attribute *attr,
				       const char *buf, size_t count)
{
	struct drm_i915_private *dev_priv = kdev_minor_to_i915(kdev);
<<<<<<< HEAD
	struct intel_rps *rps = &dev_priv->gt_pm.rps;
=======
	struct intel_rps *rps = &dev_priv->gt.rps;
>>>>>>> upstream/android-13
	bool boost = false;
	ssize_t ret;
	u32 val;

	ret = kstrtou32(buf, 0, &val);
	if (ret)
		return ret;

	/* Validate against (static) hardware limits */
<<<<<<< HEAD
	val = intel_freq_opcode(dev_priv, val);
	if (val < rps->min_freq || val > rps->max_freq)
		return -EINVAL;

	mutex_lock(&dev_priv->pcu_lock);
=======
	val = intel_freq_opcode(rps, val);
	if (val < rps->min_freq || val > rps->max_freq)
		return -EINVAL;

	mutex_lock(&rps->lock);
>>>>>>> upstream/android-13
	if (val != rps->boost_freq) {
		rps->boost_freq = val;
		boost = atomic_read(&rps->num_waiters);
	}
<<<<<<< HEAD
	mutex_unlock(&dev_priv->pcu_lock);
=======
	mutex_unlock(&rps->lock);
>>>>>>> upstream/android-13
	if (boost)
		schedule_work(&rps->work);

	return count;
}

static ssize_t vlv_rpe_freq_mhz_show(struct device *kdev,
				     struct device_attribute *attr, char *buf)
{
	struct drm_i915_private *dev_priv = kdev_minor_to_i915(kdev);
<<<<<<< HEAD

	return snprintf(buf, PAGE_SIZE, "%d\n",
			intel_gpu_freq(dev_priv,
				       dev_priv->gt_pm.rps.efficient_freq));
=======
	struct intel_rps *rps = &dev_priv->gt.rps;

	return sysfs_emit(buf, "%d\n", intel_gpu_freq(rps, rps->efficient_freq));
>>>>>>> upstream/android-13
}

static ssize_t gt_max_freq_mhz_show(struct device *kdev, struct device_attribute *attr, char *buf)
{
	struct drm_i915_private *dev_priv = kdev_minor_to_i915(kdev);
<<<<<<< HEAD

	return snprintf(buf, PAGE_SIZE, "%d\n",
			intel_gpu_freq(dev_priv,
				       dev_priv->gt_pm.rps.max_freq_softlimit));
=======
	struct intel_gt *gt = &dev_priv->gt;
	struct intel_rps *rps = &gt->rps;

	return sysfs_emit(buf, "%d\n", intel_rps_get_max_frequency(rps));
>>>>>>> upstream/android-13
}

static ssize_t gt_max_freq_mhz_store(struct device *kdev,
				     struct device_attribute *attr,
				     const char *buf, size_t count)
{
	struct drm_i915_private *dev_priv = kdev_minor_to_i915(kdev);
<<<<<<< HEAD
	struct intel_rps *rps = &dev_priv->gt_pm.rps;
	u32 val;
	ssize_t ret;
=======
	struct intel_gt *gt = &dev_priv->gt;
	struct intel_rps *rps = &gt->rps;
	ssize_t ret;
	u32 val;
>>>>>>> upstream/android-13

	ret = kstrtou32(buf, 0, &val);
	if (ret)
		return ret;

<<<<<<< HEAD
	intel_runtime_pm_get(dev_priv);

	mutex_lock(&dev_priv->pcu_lock);

	val = intel_freq_opcode(dev_priv, val);

	if (val < rps->min_freq ||
	    val > rps->max_freq ||
	    val < rps->min_freq_softlimit) {
		mutex_unlock(&dev_priv->pcu_lock);
		intel_runtime_pm_put(dev_priv);
		return -EINVAL;
	}

	if (val > rps->rp0_freq)
		DRM_DEBUG("User requested overclocking to %d\n",
			  intel_gpu_freq(dev_priv, val));

	rps->max_freq_softlimit = val;

	val = clamp_t(int, rps->cur_freq,
		      rps->min_freq_softlimit,
		      rps->max_freq_softlimit);

	/* We still need *_set_rps to process the new max_delay and
	 * update the interrupt limits and PMINTRMSK even though
	 * frequency request may be unchanged. */
	ret = intel_set_rps(dev_priv, val);

	mutex_unlock(&dev_priv->pcu_lock);

	intel_runtime_pm_put(dev_priv);
=======
	ret = intel_rps_set_max_frequency(rps, val);
>>>>>>> upstream/android-13

	return ret ?: count;
}

static ssize_t gt_min_freq_mhz_show(struct device *kdev, struct device_attribute *attr, char *buf)
{
<<<<<<< HEAD
	struct drm_i915_private *dev_priv = kdev_minor_to_i915(kdev);

	return snprintf(buf, PAGE_SIZE, "%d\n",
			intel_gpu_freq(dev_priv,
				       dev_priv->gt_pm.rps.min_freq_softlimit));
=======
	struct drm_i915_private *i915 = kdev_minor_to_i915(kdev);
	struct intel_gt *gt = &i915->gt;
	struct intel_rps *rps = &gt->rps;

	return sysfs_emit(buf, "%d\n", intel_rps_get_min_frequency(rps));
>>>>>>> upstream/android-13
}

static ssize_t gt_min_freq_mhz_store(struct device *kdev,
				     struct device_attribute *attr,
				     const char *buf, size_t count)
{
<<<<<<< HEAD
	struct drm_i915_private *dev_priv = kdev_minor_to_i915(kdev);
	struct intel_rps *rps = &dev_priv->gt_pm.rps;
	u32 val;
	ssize_t ret;
=======
	struct drm_i915_private *i915 = kdev_minor_to_i915(kdev);
	struct intel_rps *rps = &i915->gt.rps;
	ssize_t ret;
	u32 val;
>>>>>>> upstream/android-13

	ret = kstrtou32(buf, 0, &val);
	if (ret)
		return ret;

<<<<<<< HEAD
	intel_runtime_pm_get(dev_priv);

	mutex_lock(&dev_priv->pcu_lock);

	val = intel_freq_opcode(dev_priv, val);

	if (val < rps->min_freq ||
	    val > rps->max_freq ||
	    val > rps->max_freq_softlimit) {
		mutex_unlock(&dev_priv->pcu_lock);
		intel_runtime_pm_put(dev_priv);
		return -EINVAL;
	}

	rps->min_freq_softlimit = val;

	val = clamp_t(int, rps->cur_freq,
		      rps->min_freq_softlimit,
		      rps->max_freq_softlimit);

	/* We still need *_set_rps to process the new min_delay and
	 * update the interrupt limits and PMINTRMSK even though
	 * frequency request may be unchanged. */
	ret = intel_set_rps(dev_priv, val);

	mutex_unlock(&dev_priv->pcu_lock);

	intel_runtime_pm_put(dev_priv);
=======
	ret = intel_rps_set_min_frequency(rps, val);
>>>>>>> upstream/android-13

	return ret ?: count;
}

static DEVICE_ATTR_RO(gt_act_freq_mhz);
static DEVICE_ATTR_RO(gt_cur_freq_mhz);
static DEVICE_ATTR_RW(gt_boost_freq_mhz);
static DEVICE_ATTR_RW(gt_max_freq_mhz);
static DEVICE_ATTR_RW(gt_min_freq_mhz);

static DEVICE_ATTR_RO(vlv_rpe_freq_mhz);

static ssize_t gt_rp_mhz_show(struct device *kdev, struct device_attribute *attr, char *buf);
static DEVICE_ATTR(gt_RP0_freq_mhz, S_IRUGO, gt_rp_mhz_show, NULL);
static DEVICE_ATTR(gt_RP1_freq_mhz, S_IRUGO, gt_rp_mhz_show, NULL);
static DEVICE_ATTR(gt_RPn_freq_mhz, S_IRUGO, gt_rp_mhz_show, NULL);

/* For now we have a static number of RP states */
static ssize_t gt_rp_mhz_show(struct device *kdev, struct device_attribute *attr, char *buf)
{
	struct drm_i915_private *dev_priv = kdev_minor_to_i915(kdev);
<<<<<<< HEAD
	struct intel_rps *rps = &dev_priv->gt_pm.rps;
	u32 val;

	if (attr == &dev_attr_gt_RP0_freq_mhz)
		val = intel_gpu_freq(dev_priv, rps->rp0_freq);
	else if (attr == &dev_attr_gt_RP1_freq_mhz)
		val = intel_gpu_freq(dev_priv, rps->rp1_freq);
	else if (attr == &dev_attr_gt_RPn_freq_mhz)
		val = intel_gpu_freq(dev_priv, rps->min_freq);
	else
		BUG();

	return snprintf(buf, PAGE_SIZE, "%d\n", val);
}

static const struct attribute *gen6_attrs[] = {
=======
	struct intel_rps *rps = &dev_priv->gt.rps;
	u32 val;

	if (attr == &dev_attr_gt_RP0_freq_mhz)
		val = intel_rps_get_rp0_frequency(rps);
	else if (attr == &dev_attr_gt_RP1_freq_mhz)
		val = intel_rps_get_rp1_frequency(rps);
	else if (attr == &dev_attr_gt_RPn_freq_mhz)
		val = intel_rps_get_rpn_frequency(rps);
	else
		BUG();

	return sysfs_emit(buf, "%d\n", val);
}

static const struct attribute * const gen6_attrs[] = {
>>>>>>> upstream/android-13
	&dev_attr_gt_act_freq_mhz.attr,
	&dev_attr_gt_cur_freq_mhz.attr,
	&dev_attr_gt_boost_freq_mhz.attr,
	&dev_attr_gt_max_freq_mhz.attr,
	&dev_attr_gt_min_freq_mhz.attr,
	&dev_attr_gt_RP0_freq_mhz.attr,
	&dev_attr_gt_RP1_freq_mhz.attr,
	&dev_attr_gt_RPn_freq_mhz.attr,
	NULL,
};

<<<<<<< HEAD
static const struct attribute *vlv_attrs[] = {
=======
static const struct attribute * const vlv_attrs[] = {
>>>>>>> upstream/android-13
	&dev_attr_gt_act_freq_mhz.attr,
	&dev_attr_gt_cur_freq_mhz.attr,
	&dev_attr_gt_boost_freq_mhz.attr,
	&dev_attr_gt_max_freq_mhz.attr,
	&dev_attr_gt_min_freq_mhz.attr,
	&dev_attr_gt_RP0_freq_mhz.attr,
	&dev_attr_gt_RP1_freq_mhz.attr,
	&dev_attr_gt_RPn_freq_mhz.attr,
	&dev_attr_vlv_rpe_freq_mhz.attr,
	NULL,
};

#if IS_ENABLED(CONFIG_DRM_I915_CAPTURE_ERROR)

static ssize_t error_state_read(struct file *filp, struct kobject *kobj,
				struct bin_attribute *attr, char *buf,
				loff_t off, size_t count)
{

	struct device *kdev = kobj_to_dev(kobj);
<<<<<<< HEAD
	struct drm_i915_private *dev_priv = kdev_minor_to_i915(kdev);
	struct drm_i915_error_state_buf error_str;
	struct i915_gpu_state *gpu;
	ssize_t ret;

	ret = i915_error_state_buf_init(&error_str, dev_priv, count, off);
	if (ret)
		return ret;

	gpu = i915_first_error_state(dev_priv);
	ret = i915_error_state_to_str(&error_str, gpu);
	if (ret)
		goto out;

	ret = count < error_str.bytes ? count : error_str.bytes;
	memcpy(buf, error_str.buf, ret);

out:
	i915_gpu_state_put(gpu);
	i915_error_state_buf_release(&error_str);
=======
	struct drm_i915_private *i915 = kdev_minor_to_i915(kdev);
	struct i915_gpu_coredump *gpu;
	ssize_t ret;

	gpu = i915_first_error_state(i915);
	if (IS_ERR(gpu)) {
		ret = PTR_ERR(gpu);
	} else if (gpu) {
		ret = i915_gpu_coredump_copy_to_buffer(gpu, buf, off, count);
		i915_gpu_coredump_put(gpu);
	} else {
		const char *str = "No error state collected\n";
		size_t len = strlen(str);

		ret = min_t(size_t, count, len - off);
		memcpy(buf, str + off, ret);
	}
>>>>>>> upstream/android-13

	return ret;
}

static ssize_t error_state_write(struct file *file, struct kobject *kobj,
				 struct bin_attribute *attr, char *buf,
				 loff_t off, size_t count)
{
	struct device *kdev = kobj_to_dev(kobj);
	struct drm_i915_private *dev_priv = kdev_minor_to_i915(kdev);

<<<<<<< HEAD
	DRM_DEBUG_DRIVER("Resetting error state\n");
=======
	drm_dbg(&dev_priv->drm, "Resetting error state\n");
>>>>>>> upstream/android-13
	i915_reset_error_state(dev_priv);

	return count;
}

static const struct bin_attribute error_state_attr = {
	.attr.name = "error",
	.attr.mode = S_IRUSR | S_IWUSR,
	.size = 0,
	.read = error_state_read,
	.write = error_state_write,
};

static void i915_setup_error_capture(struct device *kdev)
{
	if (sysfs_create_bin_file(&kdev->kobj, &error_state_attr))
		DRM_ERROR("error_state sysfs setup failed\n");
}

static void i915_teardown_error_capture(struct device *kdev)
{
	sysfs_remove_bin_file(&kdev->kobj, &error_state_attr);
}
#else
static void i915_setup_error_capture(struct device *kdev) {}
static void i915_teardown_error_capture(struct device *kdev) {}
#endif

void i915_setup_sysfs(struct drm_i915_private *dev_priv)
{
	struct device *kdev = dev_priv->drm.primary->kdev;
	int ret;

#ifdef CONFIG_PM
	if (HAS_RC6(dev_priv)) {
		ret = sysfs_merge_group(&kdev->kobj,
					&rc6_attr_group);
		if (ret)
<<<<<<< HEAD
			DRM_ERROR("RC6 residency sysfs setup failed\n");
=======
			drm_err(&dev_priv->drm,
				"RC6 residency sysfs setup failed\n");
>>>>>>> upstream/android-13
	}
	if (HAS_RC6p(dev_priv)) {
		ret = sysfs_merge_group(&kdev->kobj,
					&rc6p_attr_group);
		if (ret)
<<<<<<< HEAD
			DRM_ERROR("RC6p residency sysfs setup failed\n");
=======
			drm_err(&dev_priv->drm,
				"RC6p residency sysfs setup failed\n");
>>>>>>> upstream/android-13
	}
	if (IS_VALLEYVIEW(dev_priv) || IS_CHERRYVIEW(dev_priv)) {
		ret = sysfs_merge_group(&kdev->kobj,
					&media_rc6_attr_group);
		if (ret)
<<<<<<< HEAD
			DRM_ERROR("Media RC6 residency sysfs setup failed\n");
=======
			drm_err(&dev_priv->drm,
				"Media RC6 residency sysfs setup failed\n");
>>>>>>> upstream/android-13
	}
#endif
	if (HAS_L3_DPF(dev_priv)) {
		ret = device_create_bin_file(kdev, &dpf_attrs);
		if (ret)
<<<<<<< HEAD
			DRM_ERROR("l3 parity sysfs setup failed\n");
=======
			drm_err(&dev_priv->drm,
				"l3 parity sysfs setup failed\n");
>>>>>>> upstream/android-13

		if (NUM_L3_SLICES(dev_priv) > 1) {
			ret = device_create_bin_file(kdev,
						     &dpf_attrs_1);
			if (ret)
<<<<<<< HEAD
				DRM_ERROR("l3 parity slice 1 setup failed\n");
=======
				drm_err(&dev_priv->drm,
					"l3 parity slice 1 setup failed\n");
>>>>>>> upstream/android-13
		}
	}

	ret = 0;
	if (IS_VALLEYVIEW(dev_priv) || IS_CHERRYVIEW(dev_priv))
		ret = sysfs_create_files(&kdev->kobj, vlv_attrs);
<<<<<<< HEAD
	else if (INTEL_GEN(dev_priv) >= 6)
		ret = sysfs_create_files(&kdev->kobj, gen6_attrs);
	if (ret)
		DRM_ERROR("RPS sysfs setup failed\n");

	i915_setup_error_capture(kdev);
=======
	else if (GRAPHICS_VER(dev_priv) >= 6)
		ret = sysfs_create_files(&kdev->kobj, gen6_attrs);
	if (ret)
		drm_err(&dev_priv->drm, "RPS sysfs setup failed\n");

	i915_setup_error_capture(kdev);

	intel_engines_add_sysfs(dev_priv);
>>>>>>> upstream/android-13
}

void i915_teardown_sysfs(struct drm_i915_private *dev_priv)
{
	struct device *kdev = dev_priv->drm.primary->kdev;

	i915_teardown_error_capture(kdev);

	if (IS_VALLEYVIEW(dev_priv) || IS_CHERRYVIEW(dev_priv))
		sysfs_remove_files(&kdev->kobj, vlv_attrs);
	else
		sysfs_remove_files(&kdev->kobj, gen6_attrs);
	device_remove_bin_file(kdev,  &dpf_attrs_1);
	device_remove_bin_file(kdev,  &dpf_attrs);
#ifdef CONFIG_PM
	sysfs_unmerge_group(&kdev->kobj, &rc6_attr_group);
	sysfs_unmerge_group(&kdev->kobj, &rc6p_attr_group);
#endif
}
