<<<<<<< HEAD
/*
 * Copyright (C) 2013-2016 Red Hat
 * Author: Rob Clark <robdclark@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published by
 * the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifdef CONFIG_DEBUG_FS
#include <linux/debugfs.h>
=======
// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright (C) 2013-2016 Red Hat
 * Author: Rob Clark <robdclark@gmail.com>
 */

#ifdef CONFIG_DEBUG_FS

#include <linux/debugfs.h>

#include <drm/drm_debugfs.h>
#include <drm/drm_file.h>

>>>>>>> upstream/android-13
#include "msm_drv.h"
#include "msm_gpu.h"
#include "msm_kms.h"
#include "msm_debugfs.h"

struct msm_gpu_show_priv {
	struct msm_gpu_state *state;
	struct drm_device *dev;
};

static int msm_gpu_show(struct seq_file *m, void *arg)
{
	struct drm_printer p = drm_seq_file_printer(m);
	struct msm_gpu_show_priv *show_priv = m->private;
	struct msm_drm_private *priv = show_priv->dev->dev_private;
	struct msm_gpu *gpu = priv->gpu;
	int ret;

	ret = mutex_lock_interruptible(&show_priv->dev->struct_mutex);
	if (ret)
		return ret;

	drm_printf(&p, "%s Status:\n", gpu->name);
	gpu->funcs->show(gpu, show_priv->state, &p);

	mutex_unlock(&show_priv->dev->struct_mutex);

	return 0;
}

static int msm_gpu_release(struct inode *inode, struct file *file)
{
	struct seq_file *m = file->private_data;
	struct msm_gpu_show_priv *show_priv = m->private;
	struct msm_drm_private *priv = show_priv->dev->dev_private;
	struct msm_gpu *gpu = priv->gpu;

	mutex_lock(&show_priv->dev->struct_mutex);
	gpu->funcs->gpu_state_put(show_priv->state);
	mutex_unlock(&show_priv->dev->struct_mutex);

	kfree(show_priv);

	return single_release(inode, file);
}

static int msm_gpu_open(struct inode *inode, struct file *file)
{
	struct drm_device *dev = inode->i_private;
	struct msm_drm_private *priv = dev->dev_private;
	struct msm_gpu *gpu = priv->gpu;
	struct msm_gpu_show_priv *show_priv;
	int ret;

<<<<<<< HEAD
	if (!gpu)
=======
	if (!gpu || !gpu->funcs->gpu_state_get)
>>>>>>> upstream/android-13
		return -ENODEV;

	show_priv = kmalloc(sizeof(*show_priv), GFP_KERNEL);
	if (!show_priv)
		return -ENOMEM;

	ret = mutex_lock_interruptible(&dev->struct_mutex);
	if (ret)
		goto free_priv;

	pm_runtime_get_sync(&gpu->pdev->dev);
<<<<<<< HEAD
=======
	msm_gpu_hw_init(gpu);
>>>>>>> upstream/android-13
	show_priv->state = gpu->funcs->gpu_state_get(gpu);
	pm_runtime_put_sync(&gpu->pdev->dev);

	mutex_unlock(&dev->struct_mutex);

	if (IS_ERR(show_priv->state)) {
		ret = PTR_ERR(show_priv->state);
		goto free_priv;
	}

	show_priv->dev = dev;

	ret = single_open(file, msm_gpu_show, show_priv);
	if (ret)
		goto free_priv;

	return 0;

free_priv:
	kfree(show_priv);
	return ret;
}

static const struct file_operations msm_gpu_fops = {
	.owner = THIS_MODULE,
	.open = msm_gpu_open,
	.read = seq_read,
	.llseek = seq_lseek,
	.release = msm_gpu_release,
};

<<<<<<< HEAD
static int msm_gem_show(struct drm_device *dev, struct seq_file *m)
{
	struct msm_drm_private *priv = dev->dev_private;
	struct msm_gpu *gpu = priv->gpu;

	if (gpu) {
		seq_printf(m, "Active Objects (%s):\n", gpu->name);
		msm_gem_describe_objects(&gpu->active_list, m);
	}

	seq_printf(m, "Inactive Objects:\n");
	msm_gem_describe_objects(&priv->inactive_list, m);
=======
static unsigned long last_shrink_freed;

static int
shrink_get(void *data, u64 *val)
{
	*val = last_shrink_freed;

	return 0;
}

static int
shrink_set(void *data, u64 val)
{
	struct drm_device *dev = data;

	last_shrink_freed = msm_gem_shrinker_shrink(dev, val);

	return 0;
}

DEFINE_SIMPLE_ATTRIBUTE(shrink_fops,
			shrink_get, shrink_set,
			"0x%08llx\n");


static int msm_gem_show(struct drm_device *dev, struct seq_file *m)
{
	struct msm_drm_private *priv = dev->dev_private;
	int ret;

	ret = mutex_lock_interruptible(&priv->obj_lock);
	if (ret)
		return ret;

	msm_gem_describe_objects(&priv->objects, m);

	mutex_unlock(&priv->obj_lock);
>>>>>>> upstream/android-13

	return 0;
}

static int msm_mm_show(struct drm_device *dev, struct seq_file *m)
{
	struct drm_printer p = drm_seq_file_printer(m);

	drm_mm_print(&dev->vma_offset_manager->vm_addr_space_mm, &p);

	return 0;
}

static int msm_fb_show(struct drm_device *dev, struct seq_file *m)
{
	struct msm_drm_private *priv = dev->dev_private;
	struct drm_framebuffer *fb, *fbdev_fb = NULL;

	if (priv->fbdev) {
		seq_printf(m, "fbcon ");
		fbdev_fb = priv->fbdev->fb;
		msm_framebuffer_describe(fbdev_fb, m);
	}

	mutex_lock(&dev->mode_config.fb_lock);
	list_for_each_entry(fb, &dev->mode_config.fb_list, head) {
		if (fb == fbdev_fb)
			continue;

		seq_printf(m, "user ");
		msm_framebuffer_describe(fb, m);
	}
	mutex_unlock(&dev->mode_config.fb_lock);

	return 0;
}

static int show_locked(struct seq_file *m, void *arg)
{
	struct drm_info_node *node = (struct drm_info_node *) m->private;
	struct drm_device *dev = node->minor->dev;
	int (*show)(struct drm_device *dev, struct seq_file *m) =
			node->info_ent->data;
	int ret;

	ret = mutex_lock_interruptible(&dev->struct_mutex);
	if (ret)
		return ret;

	ret = show(dev, m);

	mutex_unlock(&dev->struct_mutex);

	return ret;
}

static struct drm_info_list msm_debugfs_list[] = {
		{"gem", show_locked, 0, msm_gem_show},
		{ "mm", show_locked, 0, msm_mm_show },
		{ "fb", show_locked, 0, msm_fb_show },
};

static int late_init_minor(struct drm_minor *minor)
{
	int ret;

	if (!minor)
		return 0;

	ret = msm_rd_debugfs_init(minor);
	if (ret) {
<<<<<<< HEAD
		dev_err(minor->dev->dev, "could not install rd debugfs\n");
=======
		DRM_DEV_ERROR(minor->dev->dev, "could not install rd debugfs\n");
>>>>>>> upstream/android-13
		return ret;
	}

	ret = msm_perf_debugfs_init(minor);
	if (ret) {
<<<<<<< HEAD
		dev_err(minor->dev->dev, "could not install perf debugfs\n");
=======
		DRM_DEV_ERROR(minor->dev->dev, "could not install perf debugfs\n");
>>>>>>> upstream/android-13
		return ret;
	}

	return 0;
}

int msm_debugfs_late_init(struct drm_device *dev)
{
	int ret;
	ret = late_init_minor(dev->primary);
	if (ret)
		return ret;
	ret = late_init_minor(dev->render);
	return ret;
}

<<<<<<< HEAD
int msm_debugfs_init(struct drm_minor *minor)
{
	struct drm_device *dev = minor->dev;
	struct msm_drm_private *priv = dev->dev_private;
	int ret;

	ret = drm_debugfs_create_files(msm_debugfs_list,
			ARRAY_SIZE(msm_debugfs_list),
			minor->debugfs_root, minor);

	if (ret) {
		dev_err(dev->dev, "could not install msm_debugfs_list\n");
		return ret;
	}
=======
void msm_debugfs_init(struct drm_minor *minor)
{
	struct drm_device *dev = minor->dev;
	struct msm_drm_private *priv = dev->dev_private;

	drm_debugfs_create_files(msm_debugfs_list,
				 ARRAY_SIZE(msm_debugfs_list),
				 minor->debugfs_root, minor);
>>>>>>> upstream/android-13

	debugfs_create_file("gpu", S_IRUSR, minor->debugfs_root,
		dev, &msm_gpu_fops);

<<<<<<< HEAD
	if (priv->kms->funcs->debugfs_init) {
		ret = priv->kms->funcs->debugfs_init(priv->kms, minor);
		if (ret)
			return ret;
	}

	return ret;
=======
	debugfs_create_u32("hangcheck_period_ms", 0600, minor->debugfs_root,
		&priv->hangcheck_period);

	debugfs_create_file("shrink", S_IRWXU, minor->debugfs_root,
		dev, &shrink_fops);

	if (priv->kms && priv->kms->funcs->debugfs_init)
		priv->kms->funcs->debugfs_init(priv->kms, minor);
>>>>>>> upstream/android-13
}
#endif

