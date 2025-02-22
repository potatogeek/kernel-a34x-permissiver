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
 *    Kevin Tian <kevin.tian@intel.com>
 *    Eddie Dong <eddie.dong@intel.com>
 *
 * Contributors:
 *    Niu Bing <bing.niu@intel.com>
 *    Zhi Wang <zhi.a.wang@intel.com>
 *
 */

#include <linux/types.h>
<<<<<<< HEAD
#include <xen/xen.h>
#include <linux/kthread.h>

#include "i915_drv.h"
=======
#include <linux/kthread.h>

#include "i915_drv.h"
#include "intel_gvt.h"
>>>>>>> upstream/android-13
#include "gvt.h"
#include <linux/vfio.h>
#include <linux/mdev.h>

struct intel_gvt_host intel_gvt_host;

static const char * const supported_hypervisors[] = {
	[INTEL_GVT_HYPERVISOR_XEN] = "XEN",
	[INTEL_GVT_HYPERVISOR_KVM] = "KVM",
};

<<<<<<< HEAD
static struct intel_vgpu_type *intel_gvt_find_vgpu_type(struct intel_gvt *gvt,
		const char *name)
{
	int i;
	struct intel_vgpu_type *t;
	const char *driver_name = dev_driver_string(
			&gvt->dev_priv->drm.pdev->dev);

	for (i = 0; i < gvt->num_types; i++) {
		t = &gvt->types[i];
		if (!strncmp(t->name, name + strlen(driver_name) + 1,
			sizeof(t->name)))
			return t;
	}

	return NULL;
}

static ssize_t available_instances_show(struct kobject *kobj,
					struct device *dev, char *buf)
{
	struct intel_vgpu_type *type;
	unsigned int num = 0;
	void *gvt = kdev_to_i915(dev)->gvt;

	type = intel_gvt_find_vgpu_type(gvt, kobject_name(kobj));
	if (!type)
		num = 0;
	else
		num = type->avail_instance;

	return sprintf(buf, "%u\n", num);
}

static ssize_t device_api_show(struct kobject *kobj, struct device *dev,
		char *buf)
{
	return sprintf(buf, "%s\n", VFIO_DEVICE_API_PCI_STRING);
}

static ssize_t description_show(struct kobject *kobj, struct device *dev,
		char *buf)
{
	struct intel_vgpu_type *type;
	void *gvt = kdev_to_i915(dev)->gvt;

	type = intel_gvt_find_vgpu_type(gvt, kobject_name(kobj));
	if (!type)
		return 0;

	return sprintf(buf, "low_gm_size: %dMB\nhigh_gm_size: %dMB\n"
		       "fence: %d\nresolution: %s\n"
		       "weight: %d\n",
		       BYTES_TO_MB(type->low_gm_size),
		       BYTES_TO_MB(type->high_gm_size),
		       type->fence, vgpu_edid_str(type->resolution),
		       type->weight);
}

static MDEV_TYPE_ATTR_RO(available_instances);
static MDEV_TYPE_ATTR_RO(device_api);
static MDEV_TYPE_ATTR_RO(description);

static struct attribute *gvt_type_attrs[] = {
	&mdev_type_attr_available_instances.attr,
	&mdev_type_attr_device_api.attr,
	&mdev_type_attr_description.attr,
	NULL,
};

static struct attribute_group *gvt_vgpu_type_groups[] = {
	[0 ... NR_MAX_INTEL_VGPU_TYPES - 1] = NULL,
};

static bool intel_get_gvt_attrs(struct attribute ***type_attrs,
		struct attribute_group ***intel_vgpu_type_groups)
{
	*type_attrs = gvt_type_attrs;
	*intel_vgpu_type_groups = gvt_vgpu_type_groups;
	return true;
}

static int intel_gvt_init_vgpu_type_groups(struct intel_gvt *gvt)
{
	int i, j;
	struct intel_vgpu_type *type;
	struct attribute_group *group;

	for (i = 0; i < gvt->num_types; i++) {
		type = &gvt->types[i];

		group = kzalloc(sizeof(struct attribute_group), GFP_KERNEL);
		if (WARN_ON(!group))
			goto unwind;

		group->name = type->name;
		group->attrs = gvt_type_attrs;
		gvt_vgpu_type_groups[i] = group;
	}

	return 0;

unwind:
	for (j = 0; j < i; j++) {
		group = gvt_vgpu_type_groups[j];
		kfree(group);
	}

	return -ENOMEM;
}

static void intel_gvt_cleanup_vgpu_type_groups(struct intel_gvt *gvt)
{
	int i;
	struct attribute_group *group;

	for (i = 0; i < gvt->num_types; i++) {
		group = gvt_vgpu_type_groups[i];
		gvt_vgpu_type_groups[i] = NULL;
		kfree(group);
	}
}

=======
>>>>>>> upstream/android-13
static const struct intel_gvt_ops intel_gvt_ops = {
	.emulate_cfg_read = intel_vgpu_emulate_cfg_read,
	.emulate_cfg_write = intel_vgpu_emulate_cfg_write,
	.emulate_mmio_read = intel_vgpu_emulate_mmio_read,
	.emulate_mmio_write = intel_vgpu_emulate_mmio_write,
	.vgpu_create = intel_gvt_create_vgpu,
	.vgpu_destroy = intel_gvt_destroy_vgpu,
	.vgpu_release = intel_gvt_release_vgpu,
	.vgpu_reset = intel_gvt_reset_vgpu,
	.vgpu_activate = intel_gvt_activate_vgpu,
	.vgpu_deactivate = intel_gvt_deactivate_vgpu,
<<<<<<< HEAD
	.gvt_find_vgpu_type = intel_gvt_find_vgpu_type,
	.get_gvt_attrs = intel_get_gvt_attrs,
	.vgpu_query_plane = intel_vgpu_query_plane,
	.vgpu_get_dmabuf = intel_vgpu_get_dmabuf,
	.write_protect_handler = intel_vgpu_page_track_handler,
};

/**
 * intel_gvt_init_host - Load MPT modules and detect if we're running in host
 * @gvt: intel gvt device
 *
 * This function is called at the driver loading stage. If failed to find a
 * loadable MPT module or detect currently we're running in a VM, then GVT-g
 * will be disabled
 *
 * Returns:
 * Zero on success, negative error code if failed.
 *
 */
int intel_gvt_init_host(void)
{
	if (intel_gvt_host.initialized)
		return 0;

	/* Xen DOM U */
	if (xen_domain() && !xen_initial_domain())
		return -ENODEV;

	/* Try to load MPT modules for hypervisors */
	if (xen_initial_domain()) {
		/* In Xen dom0 */
		intel_gvt_host.mpt = try_then_request_module(
				symbol_get(xengt_mpt), "xengt");
		intel_gvt_host.hypervisor_type = INTEL_GVT_HYPERVISOR_XEN;
	} else {
#if IS_ENABLED(CONFIG_DRM_I915_GVT_KVMGT)
		/* not in Xen. Try KVMGT */
		intel_gvt_host.mpt = try_then_request_module(
				symbol_get(kvmgt_mpt), "kvmgt");
		intel_gvt_host.hypervisor_type = INTEL_GVT_HYPERVISOR_KVM;
#endif
	}

	/* Fail to load MPT modules - bail out */
	if (!intel_gvt_host.mpt)
		return -EINVAL;

	gvt_dbg_core("Running with hypervisor %s in host mode\n",
			supported_hypervisors[intel_gvt_host.hypervisor_type]);

	intel_gvt_host.initialized = true;
	return 0;
}

static void init_device_info(struct intel_gvt *gvt)
{
	struct intel_gvt_device_info *info = &gvt->device_info;
	struct pci_dev *pdev = gvt->dev_priv->drm.pdev;
=======
	.vgpu_query_plane = intel_vgpu_query_plane,
	.vgpu_get_dmabuf = intel_vgpu_get_dmabuf,
	.write_protect_handler = intel_vgpu_page_track_handler,
	.emulate_hotplug = intel_vgpu_emulate_hotplug,
};

static void init_device_info(struct intel_gvt *gvt)
{
	struct intel_gvt_device_info *info = &gvt->device_info;
	struct pci_dev *pdev = to_pci_dev(gvt->gt->i915->drm.dev);
>>>>>>> upstream/android-13

	info->max_support_vgpus = 8;
	info->cfg_space_size = PCI_CFG_SPACE_EXP_SIZE;
	info->mmio_size = 2 * 1024 * 1024;
	info->mmio_bar = 0;
	info->gtt_start_offset = 8 * 1024 * 1024;
	info->gtt_entry_size = 8;
	info->gtt_entry_size_shift = 3;
	info->gmadr_bytes_in_cmd = 8;
	info->max_surface_size = 36 * 1024 * 1024;
	info->msi_cap_offset = pdev->msi_cap;
}

<<<<<<< HEAD
=======
static void intel_gvt_test_and_emulate_vblank(struct intel_gvt *gvt)
{
	struct intel_vgpu *vgpu;
	int id;

	mutex_lock(&gvt->lock);
	idr_for_each_entry((&(gvt)->vgpu_idr), (vgpu), (id)) {
		if (test_and_clear_bit(INTEL_GVT_REQUEST_EMULATE_VBLANK + id,
				       (void *)&gvt->service_request)) {
			if (vgpu->active)
				intel_vgpu_emulate_vblank(vgpu);
		}
	}
	mutex_unlock(&gvt->lock);
}

>>>>>>> upstream/android-13
static int gvt_service_thread(void *data)
{
	struct intel_gvt *gvt = (struct intel_gvt *)data;
	int ret;

	gvt_dbg_core("service thread start\n");

	while (!kthread_should_stop()) {
		ret = wait_event_interruptible(gvt->service_thread_wq,
				kthread_should_stop() || gvt->service_request);

		if (kthread_should_stop())
			break;

		if (WARN_ONCE(ret, "service thread is waken up by signal.\n"))
			continue;

<<<<<<< HEAD
		if (test_and_clear_bit(INTEL_GVT_REQUEST_EMULATE_VBLANK,
					(void *)&gvt->service_request))
			intel_gvt_emulate_vblank(gvt);
=======
		intel_gvt_test_and_emulate_vblank(gvt);
>>>>>>> upstream/android-13

		if (test_bit(INTEL_GVT_REQUEST_SCHED,
				(void *)&gvt->service_request) ||
			test_bit(INTEL_GVT_REQUEST_EVENT_SCHED,
					(void *)&gvt->service_request)) {
			intel_gvt_schedule(gvt);
		}
	}

	return 0;
}

static void clean_service_thread(struct intel_gvt *gvt)
{
	kthread_stop(gvt->service_thread);
}

static int init_service_thread(struct intel_gvt *gvt)
{
	init_waitqueue_head(&gvt->service_thread_wq);

	gvt->service_thread = kthread_run(gvt_service_thread,
			gvt, "gvt_service_thread");
	if (IS_ERR(gvt->service_thread)) {
		gvt_err("fail to start service thread.\n");
		return PTR_ERR(gvt->service_thread);
	}
	return 0;
}

/**
 * intel_gvt_clean_device - clean a GVT device
<<<<<<< HEAD
 * @gvt: intel gvt device
=======
 * @i915: i915 private
>>>>>>> upstream/android-13
 *
 * This function is called at the driver unloading stage, to free the
 * resources owned by a GVT device.
 *
 */
<<<<<<< HEAD
void intel_gvt_clean_device(struct drm_i915_private *dev_priv)
{
	struct intel_gvt *gvt = to_gvt(dev_priv);

	if (WARN_ON(!gvt))
		return;

	intel_gvt_destroy_idle_vgpu(gvt->idle_vgpu);
	intel_gvt_hypervisor_host_exit(&dev_priv->drm.pdev->dev, gvt);
	intel_gvt_cleanup_vgpu_type_groups(gvt);
=======
void intel_gvt_clean_device(struct drm_i915_private *i915)
{
	struct intel_gvt *gvt = fetch_and_zero(&i915->gvt);

	if (drm_WARN_ON(&i915->drm, !gvt))
		return;

	intel_gvt_destroy_idle_vgpu(gvt->idle_vgpu);
>>>>>>> upstream/android-13
	intel_gvt_clean_vgpu_types(gvt);

	intel_gvt_debugfs_clean(gvt);
	clean_service_thread(gvt);
	intel_gvt_clean_cmd_parser(gvt);
	intel_gvt_clean_sched_policy(gvt);
	intel_gvt_clean_workload_scheduler(gvt);
	intel_gvt_clean_gtt(gvt);
<<<<<<< HEAD
	intel_gvt_clean_irq(gvt);
=======
>>>>>>> upstream/android-13
	intel_gvt_free_firmware(gvt);
	intel_gvt_clean_mmio_info(gvt);
	idr_destroy(&gvt->vgpu_idr);

<<<<<<< HEAD
	kfree(dev_priv->gvt);
	dev_priv->gvt = NULL;
=======
	kfree(i915->gvt);
>>>>>>> upstream/android-13
}

/**
 * intel_gvt_init_device - initialize a GVT device
<<<<<<< HEAD
 * @dev_priv: drm i915 private data
=======
 * @i915: drm i915 private data
>>>>>>> upstream/android-13
 *
 * This function is called at the initialization stage, to initialize
 * necessary GVT components.
 *
 * Returns:
 * Zero on success, negative error code if failed.
 *
 */
<<<<<<< HEAD
int intel_gvt_init_device(struct drm_i915_private *dev_priv)
=======
int intel_gvt_init_device(struct drm_i915_private *i915)
>>>>>>> upstream/android-13
{
	struct intel_gvt *gvt;
	struct intel_vgpu *vgpu;
	int ret;

<<<<<<< HEAD
	/*
	 * Cannot initialize GVT device without intel_gvt_host gets
	 * initialized first.
	 */
	if (WARN_ON(!intel_gvt_host.initialized))
		return -EINVAL;

	if (WARN_ON(dev_priv->gvt))
=======
	if (drm_WARN_ON(&i915->drm, i915->gvt))
>>>>>>> upstream/android-13
		return -EEXIST;

	gvt = kzalloc(sizeof(struct intel_gvt), GFP_KERNEL);
	if (!gvt)
		return -ENOMEM;

	gvt_dbg_core("init gvt device\n");

<<<<<<< HEAD
	idr_init(&gvt->vgpu_idr);
	spin_lock_init(&gvt->scheduler.mmio_context_lock);
	mutex_init(&gvt->lock);
	mutex_init(&gvt->sched_lock);
	gvt->dev_priv = dev_priv;
=======
	idr_init_base(&gvt->vgpu_idr, 1);
	spin_lock_init(&gvt->scheduler.mmio_context_lock);
	mutex_init(&gvt->lock);
	mutex_init(&gvt->sched_lock);
	gvt->gt = &i915->gt;
	i915->gvt = gvt;
>>>>>>> upstream/android-13

	init_device_info(gvt);

	ret = intel_gvt_setup_mmio_info(gvt);
	if (ret)
		goto out_clean_idr;

	intel_gvt_init_engine_mmio_context(gvt);

	ret = intel_gvt_load_firmware(gvt);
	if (ret)
		goto out_clean_mmio_info;

	ret = intel_gvt_init_irq(gvt);
	if (ret)
		goto out_free_firmware;

	ret = intel_gvt_init_gtt(gvt);
	if (ret)
<<<<<<< HEAD
		goto out_clean_irq;
=======
		goto out_free_firmware;
>>>>>>> upstream/android-13

	ret = intel_gvt_init_workload_scheduler(gvt);
	if (ret)
		goto out_clean_gtt;

	ret = intel_gvt_init_sched_policy(gvt);
	if (ret)
		goto out_clean_workload_scheduler;

	ret = intel_gvt_init_cmd_parser(gvt);
	if (ret)
		goto out_clean_sched_policy;

	ret = init_service_thread(gvt);
	if (ret)
		goto out_clean_cmd_parser;

	ret = intel_gvt_init_vgpu_types(gvt);
	if (ret)
		goto out_clean_thread;

<<<<<<< HEAD
	ret = intel_gvt_init_vgpu_type_groups(gvt);
	if (ret) {
		gvt_err("failed to init vgpu type groups: %d\n", ret);
		goto out_clean_types;
	}

	ret = intel_gvt_hypervisor_host_init(&dev_priv->drm.pdev->dev, gvt,
				&intel_gvt_ops);
	if (ret) {
		gvt_err("failed to register gvt-g host device: %d\n", ret);
		goto out_clean_types;
	}

=======
>>>>>>> upstream/android-13
	vgpu = intel_gvt_create_idle_vgpu(gvt);
	if (IS_ERR(vgpu)) {
		ret = PTR_ERR(vgpu);
		gvt_err("failed to create idle vgpu\n");
		goto out_clean_types;
	}
	gvt->idle_vgpu = vgpu;

<<<<<<< HEAD
	ret = intel_gvt_debugfs_init(gvt);
	if (ret)
		gvt_err("debugfs registeration failed, go on.\n");

	gvt_dbg_core("gvt device initialization is done\n");
	dev_priv->gvt = gvt;
=======
	intel_gvt_debugfs_init(gvt);

	gvt_dbg_core("gvt device initialization is done\n");
	intel_gvt_host.dev = i915->drm.dev;
	intel_gvt_host.initialized = true;
>>>>>>> upstream/android-13
	return 0;

out_clean_types:
	intel_gvt_clean_vgpu_types(gvt);
out_clean_thread:
	clean_service_thread(gvt);
out_clean_cmd_parser:
	intel_gvt_clean_cmd_parser(gvt);
out_clean_sched_policy:
	intel_gvt_clean_sched_policy(gvt);
out_clean_workload_scheduler:
	intel_gvt_clean_workload_scheduler(gvt);
out_clean_gtt:
	intel_gvt_clean_gtt(gvt);
<<<<<<< HEAD
out_clean_irq:
	intel_gvt_clean_irq(gvt);
=======
>>>>>>> upstream/android-13
out_free_firmware:
	intel_gvt_free_firmware(gvt);
out_clean_mmio_info:
	intel_gvt_clean_mmio_info(gvt);
out_clean_idr:
	idr_destroy(&gvt->vgpu_idr);
	kfree(gvt);
<<<<<<< HEAD
	return ret;
}

#if IS_ENABLED(CONFIG_DRM_I915_GVT_KVMGT)
MODULE_SOFTDEP("pre: kvmgt");
#endif
=======
	i915->gvt = NULL;
	return ret;
}

int
intel_gvt_pm_resume(struct intel_gvt *gvt)
{
	intel_gvt_restore_fence(gvt);
	intel_gvt_restore_mmio(gvt);
	intel_gvt_restore_ggtt(gvt);
	return 0;
}

int
intel_gvt_register_hypervisor(const struct intel_gvt_mpt *m)
{
	int ret;
	void *gvt;

	if (!intel_gvt_host.initialized)
		return -ENODEV;

	if (m->type != INTEL_GVT_HYPERVISOR_KVM &&
	    m->type != INTEL_GVT_HYPERVISOR_XEN)
		return -EINVAL;

	/* Get a reference for device model module */
	if (!try_module_get(THIS_MODULE))
		return -ENODEV;

	intel_gvt_host.mpt = m;
	intel_gvt_host.hypervisor_type = m->type;
	gvt = (void *)kdev_to_i915(intel_gvt_host.dev)->gvt;

	ret = intel_gvt_hypervisor_host_init(intel_gvt_host.dev, gvt,
					     &intel_gvt_ops);
	if (ret < 0) {
		gvt_err("Failed to init %s hypervisor module\n",
			supported_hypervisors[intel_gvt_host.hypervisor_type]);
		module_put(THIS_MODULE);
		return -ENODEV;
	}
	gvt_dbg_core("Running with hypervisor %s in host mode\n",
		     supported_hypervisors[intel_gvt_host.hypervisor_type]);
	return 0;
}
EXPORT_SYMBOL_GPL(intel_gvt_register_hypervisor);

void
intel_gvt_unregister_hypervisor(void)
{
	void *gvt = (void *)kdev_to_i915(intel_gvt_host.dev)->gvt;
	intel_gvt_hypervisor_host_exit(intel_gvt_host.dev, gvt);
	module_put(THIS_MODULE);
}
EXPORT_SYMBOL_GPL(intel_gvt_unregister_hypervisor);
>>>>>>> upstream/android-13
