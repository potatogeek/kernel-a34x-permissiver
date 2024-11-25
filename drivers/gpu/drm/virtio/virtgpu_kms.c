/*
 * Copyright (C) 2015 Red Hat, Inc.
 * All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice (including the
 * next paragraph) shall be included in all copies or substantial
 * portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE COPYRIGHT OWNER(S) AND/OR ITS SUPPLIERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include <linux/virtio.h>
#include <linux/virtio_config.h>
<<<<<<< HEAD
#include <drm/drmP.h>
=======
#include <linux/virtio_ring.h>

#include <drm/drm_file.h>

>>>>>>> upstream/android-13
#include "virtgpu_drv.h"

static void virtio_gpu_config_changed_work_func(struct work_struct *work)
{
	struct virtio_gpu_device *vgdev =
		container_of(work, struct virtio_gpu_device,
			     config_changed_work);
	u32 events_read, events_clear = 0;

	/* read the config space */
<<<<<<< HEAD
	virtio_cread(vgdev->vdev, struct virtio_gpu_config,
		     events_read, &events_read);
=======
	virtio_cread_le(vgdev->vdev, struct virtio_gpu_config,
			events_read, &events_read);
>>>>>>> upstream/android-13
	if (events_read & VIRTIO_GPU_EVENT_DISPLAY) {
		if (vgdev->has_edid)
			virtio_gpu_cmd_get_edids(vgdev);
		virtio_gpu_cmd_get_display_info(vgdev);
<<<<<<< HEAD
		drm_helper_hpd_irq_event(vgdev->ddev);
		events_clear |= VIRTIO_GPU_EVENT_DISPLAY;
	}
	virtio_cwrite(vgdev->vdev, struct virtio_gpu_config,
		      events_clear, &events_clear);
}

static int virtio_gpu_context_create(struct virtio_gpu_device *vgdev,
				      uint32_t nlen, const char *name)
{
	int handle = ida_alloc(&vgdev->ctx_id_ida, GFP_KERNEL);

	if (handle < 0)
		return handle;
	handle += 1;
	virtio_gpu_cmd_context_create(vgdev, handle, nlen, name);
	return handle;
}

static void virtio_gpu_context_destroy(struct virtio_gpu_device *vgdev,
				      uint32_t ctx_id)
{
	virtio_gpu_cmd_context_destroy(vgdev, ctx_id);
	ida_free(&vgdev->ctx_id_ida, ctx_id - 1);
=======
		virtio_gpu_notify(vgdev);
		drm_helper_hpd_irq_event(vgdev->ddev);
		events_clear |= VIRTIO_GPU_EVENT_DISPLAY;
	}
	virtio_cwrite_le(vgdev->vdev, struct virtio_gpu_config,
			 events_clear, &events_clear);
>>>>>>> upstream/android-13
}

static void virtio_gpu_init_vq(struct virtio_gpu_queue *vgvq,
			       void (*work_func)(struct work_struct *work))
{
	spin_lock_init(&vgvq->qlock);
	init_waitqueue_head(&vgvq->ack_queue);
	INIT_WORK(&vgvq->dequeue_work, work_func);
}

static void virtio_gpu_get_capsets(struct virtio_gpu_device *vgdev,
				   int num_capsets)
{
	int i, ret;

	vgdev->capsets = kcalloc(num_capsets,
				 sizeof(struct virtio_gpu_drv_capset),
				 GFP_KERNEL);
	if (!vgdev->capsets) {
		DRM_ERROR("failed to allocate cap sets\n");
		return;
	}
	for (i = 0; i < num_capsets; i++) {
		virtio_gpu_cmd_get_capset_info(vgdev, i);
<<<<<<< HEAD
=======
		virtio_gpu_notify(vgdev);
>>>>>>> upstream/android-13
		ret = wait_event_timeout(vgdev->resp_wq,
					 vgdev->capsets[i].id > 0, 5 * HZ);
		if (ret == 0) {
			DRM_ERROR("timed out waiting for cap set %d\n", i);
			spin_lock(&vgdev->display_info_lock);
			kfree(vgdev->capsets);
			vgdev->capsets = NULL;
			spin_unlock(&vgdev->display_info_lock);
			return;
		}
		DRM_INFO("cap set %d: id %d, max-version %d, max-size %d\n",
			 i, vgdev->capsets[i].id,
			 vgdev->capsets[i].max_version,
			 vgdev->capsets[i].max_size);
	}
	vgdev->num_capsets = num_capsets;
}

int virtio_gpu_init(struct drm_device *dev)
{
	static vq_callback_t *callbacks[] = {
		virtio_gpu_ctrl_ack, virtio_gpu_cursor_ack
	};
	static const char * const names[] = { "control", "cursor" };

	struct virtio_gpu_device *vgdev;
	/* this will expand later */
	struct virtqueue *vqs[2];
	u32 num_scanouts, num_capsets;
<<<<<<< HEAD
	int ret;
=======
	int ret = 0;
>>>>>>> upstream/android-13

	if (!virtio_has_feature(dev_to_virtio(dev->dev), VIRTIO_F_VERSION_1))
		return -ENODEV;

	vgdev = kzalloc(sizeof(struct virtio_gpu_device), GFP_KERNEL);
	if (!vgdev)
		return -ENOMEM;

	vgdev->ddev = dev;
	dev->dev_private = vgdev;
	vgdev->vdev = dev_to_virtio(dev->dev);
	vgdev->dev = dev->dev;

	spin_lock_init(&vgdev->display_info_lock);
<<<<<<< HEAD
=======
	spin_lock_init(&vgdev->resource_export_lock);
	spin_lock_init(&vgdev->host_visible_lock);
>>>>>>> upstream/android-13
	ida_init(&vgdev->ctx_id_ida);
	ida_init(&vgdev->resource_ida);
	init_waitqueue_head(&vgdev->resp_wq);
	virtio_gpu_init_vq(&vgdev->ctrlq, virtio_gpu_dequeue_ctrl_func);
	virtio_gpu_init_vq(&vgdev->cursorq, virtio_gpu_dequeue_cursor_func);

	vgdev->fence_drv.context = dma_fence_context_alloc(1);
	spin_lock_init(&vgdev->fence_drv.lock);
	INIT_LIST_HEAD(&vgdev->fence_drv.fences);
	INIT_LIST_HEAD(&vgdev->cap_cache);
	INIT_WORK(&vgdev->config_changed_work,
		  virtio_gpu_config_changed_work_func);

<<<<<<< HEAD
#ifdef __LITTLE_ENDIAN
	if (virtio_has_feature(vgdev->vdev, VIRTIO_GPU_F_VIRGL))
		vgdev->has_virgl_3d = true;
	DRM_INFO("virgl 3d acceleration %s\n",
		 vgdev->has_virgl_3d ? "enabled" : "not supported by host");
#else
	DRM_INFO("virgl 3d acceleration not supported by guest\n");
#endif
	if (virtio_has_feature(vgdev->vdev, VIRTIO_GPU_F_EDID)) {
		vgdev->has_edid = true;
		DRM_INFO("EDID support available.\n");
	}
=======
	INIT_WORK(&vgdev->obj_free_work,
		  virtio_gpu_array_put_free_work);
	INIT_LIST_HEAD(&vgdev->obj_free_list);
	spin_lock_init(&vgdev->obj_free_lock);

#ifdef __LITTLE_ENDIAN
	if (virtio_has_feature(vgdev->vdev, VIRTIO_GPU_F_VIRGL))
		vgdev->has_virgl_3d = true;
#endif
	if (virtio_has_feature(vgdev->vdev, VIRTIO_GPU_F_EDID)) {
		vgdev->has_edid = true;
	}
	if (virtio_has_feature(vgdev->vdev, VIRTIO_RING_F_INDIRECT_DESC)) {
		vgdev->has_indirect = true;
	}
	if (virtio_has_feature(vgdev->vdev, VIRTIO_GPU_F_RESOURCE_UUID)) {
		vgdev->has_resource_assign_uuid = true;
	}
	if (virtio_has_feature(vgdev->vdev, VIRTIO_GPU_F_RESOURCE_BLOB)) {
		vgdev->has_resource_blob = true;
	}
	if (virtio_get_shm_region(vgdev->vdev, &vgdev->host_visible_region,
				  VIRTIO_GPU_SHM_ID_HOST_VISIBLE)) {
		if (!devm_request_mem_region(&vgdev->vdev->dev,
					     vgdev->host_visible_region.addr,
					     vgdev->host_visible_region.len,
					     dev_name(&vgdev->vdev->dev))) {
			DRM_ERROR("Could not reserve host visible region\n");
			ret = -EBUSY;
			goto err_vqs;
		}

		DRM_INFO("Host memory window: 0x%lx +0x%lx\n",
			 (unsigned long)vgdev->host_visible_region.addr,
			 (unsigned long)vgdev->host_visible_region.len);
		vgdev->has_host_visible = true;
		drm_mm_init(&vgdev->host_visible_mm,
			    (unsigned long)vgdev->host_visible_region.addr,
			    (unsigned long)vgdev->host_visible_region.len);
	}

	DRM_INFO("features: %cvirgl %cedid %cresource_blob %chost_visible\n",
		 vgdev->has_virgl_3d    ? '+' : '-',
		 vgdev->has_edid        ? '+' : '-',
		 vgdev->has_resource_blob ? '+' : '-',
		 vgdev->has_host_visible ? '+' : '-');
>>>>>>> upstream/android-13

	ret = virtio_find_vqs(vgdev->vdev, 2, vqs, callbacks, names, NULL);
	if (ret) {
		DRM_ERROR("failed to find virt queues\n");
		goto err_vqs;
	}
	vgdev->ctrlq.vq = vqs[0];
	vgdev->cursorq.vq = vqs[1];
	ret = virtio_gpu_alloc_vbufs(vgdev);
	if (ret) {
		DRM_ERROR("failed to alloc vbufs\n");
		goto err_vbufs;
	}

<<<<<<< HEAD
	ret = virtio_gpu_ttm_init(vgdev);
	if (ret) {
		DRM_ERROR("failed to init ttm %d\n", ret);
		goto err_ttm;
	}

	/* get display info */
	virtio_cread(vgdev->vdev, struct virtio_gpu_config,
		     num_scanouts, &num_scanouts);
=======
	/* get display info */
	virtio_cread_le(vgdev->vdev, struct virtio_gpu_config,
			num_scanouts, &num_scanouts);
>>>>>>> upstream/android-13
	vgdev->num_scanouts = min_t(uint32_t, num_scanouts,
				    VIRTIO_GPU_MAX_SCANOUTS);
	if (!vgdev->num_scanouts) {
		DRM_ERROR("num_scanouts is zero\n");
		ret = -EINVAL;
		goto err_scanouts;
	}
	DRM_INFO("number of scanouts: %d\n", num_scanouts);

<<<<<<< HEAD
	virtio_cread(vgdev->vdev, struct virtio_gpu_config,
		     num_capsets, &num_capsets);
	DRM_INFO("number of cap sets: %d\n", num_capsets);

	virtio_gpu_modeset_init(vgdev);

	virtio_device_ready(vgdev->vdev);
	vgdev->vqs_ready = true;
=======
	virtio_cread_le(vgdev->vdev, struct virtio_gpu_config,
			num_capsets, &num_capsets);
	DRM_INFO("number of cap sets: %d\n", num_capsets);

	ret = virtio_gpu_modeset_init(vgdev);
	if (ret) {
		DRM_ERROR("modeset init failed\n");
		goto err_scanouts;
	}

	virtio_device_ready(vgdev->vdev);
>>>>>>> upstream/android-13

	if (num_capsets)
		virtio_gpu_get_capsets(vgdev, num_capsets);
	if (vgdev->has_edid)
		virtio_gpu_cmd_get_edids(vgdev);
	virtio_gpu_cmd_get_display_info(vgdev);
<<<<<<< HEAD
=======
	virtio_gpu_notify(vgdev);
>>>>>>> upstream/android-13
	wait_event_timeout(vgdev->resp_wq, !vgdev->display_info_pending,
			   5 * HZ);
	return 0;

err_scanouts:
<<<<<<< HEAD
	virtio_gpu_ttm_fini(vgdev);
err_ttm:
=======
>>>>>>> upstream/android-13
	virtio_gpu_free_vbufs(vgdev);
err_vbufs:
	vgdev->vdev->config->del_vqs(vgdev->vdev);
err_vqs:
<<<<<<< HEAD
=======
	dev->dev_private = NULL;
>>>>>>> upstream/android-13
	kfree(vgdev);
	return ret;
}

static void virtio_gpu_cleanup_cap_cache(struct virtio_gpu_device *vgdev)
{
	struct virtio_gpu_drv_cap_cache *cache_ent, *tmp;

	list_for_each_entry_safe(cache_ent, tmp, &vgdev->cap_cache, head) {
		kfree(cache_ent->caps_cache);
		kfree(cache_ent);
	}
}

void virtio_gpu_deinit(struct drm_device *dev)
{
	struct virtio_gpu_device *vgdev = dev->dev_private;

<<<<<<< HEAD
	vgdev->vqs_ready = false;
=======
	flush_work(&vgdev->obj_free_work);
>>>>>>> upstream/android-13
	flush_work(&vgdev->ctrlq.dequeue_work);
	flush_work(&vgdev->cursorq.dequeue_work);
	flush_work(&vgdev->config_changed_work);
	vgdev->vdev->config->reset(vgdev->vdev);
	vgdev->vdev->config->del_vqs(vgdev->vdev);
<<<<<<< HEAD

	virtio_gpu_modeset_fini(vgdev);
	virtio_gpu_ttm_fini(vgdev);
	virtio_gpu_free_vbufs(vgdev);
	virtio_gpu_cleanup_cap_cache(vgdev);
=======
}

void virtio_gpu_release(struct drm_device *dev)
{
	struct virtio_gpu_device *vgdev = dev->dev_private;

	if (!vgdev)
		return;

	virtio_gpu_modeset_fini(vgdev);
	virtio_gpu_free_vbufs(vgdev);
	virtio_gpu_cleanup_cap_cache(vgdev);

	if (vgdev->has_host_visible)
		drm_mm_takedown(&vgdev->host_visible_mm);

>>>>>>> upstream/android-13
	kfree(vgdev->capsets);
	kfree(vgdev);
}

int virtio_gpu_driver_open(struct drm_device *dev, struct drm_file *file)
{
	struct virtio_gpu_device *vgdev = dev->dev_private;
	struct virtio_gpu_fpriv *vfpriv;
<<<<<<< HEAD
	int id;
	char dbgname[TASK_COMM_LEN];
=======
	int handle;
>>>>>>> upstream/android-13

	/* can't create contexts without 3d renderer */
	if (!vgdev->has_virgl_3d)
		return 0;

	/* allocate a virt GPU context for this opener */
	vfpriv = kzalloc(sizeof(*vfpriv), GFP_KERNEL);
	if (!vfpriv)
		return -ENOMEM;

<<<<<<< HEAD
	get_task_comm(dbgname, current);
	id = virtio_gpu_context_create(vgdev, strlen(dbgname), dbgname);
	if (id < 0) {
		kfree(vfpriv);
		return id;
	}

	vfpriv->ctx_id = id;
=======
	mutex_init(&vfpriv->context_lock);

	handle = ida_alloc(&vgdev->ctx_id_ida, GFP_KERNEL);
	if (handle < 0) {
		kfree(vfpriv);
		return handle;
	}

	vfpriv->ctx_id = handle + 1;
>>>>>>> upstream/android-13
	file->driver_priv = vfpriv;
	return 0;
}

void virtio_gpu_driver_postclose(struct drm_device *dev, struct drm_file *file)
{
	struct virtio_gpu_device *vgdev = dev->dev_private;
<<<<<<< HEAD
	struct virtio_gpu_fpriv *vfpriv;
=======
	struct virtio_gpu_fpriv *vfpriv = file->driver_priv;
>>>>>>> upstream/android-13

	if (!vgdev->has_virgl_3d)
		return;

<<<<<<< HEAD
	vfpriv = file->driver_priv;

	virtio_gpu_context_destroy(vgdev, vfpriv->ctx_id);
=======
	if (vfpriv->context_created) {
		virtio_gpu_cmd_context_destroy(vgdev, vfpriv->ctx_id);
		virtio_gpu_notify(vgdev);
	}

	ida_free(&vgdev->ctx_id_ida, vfpriv->ctx_id - 1);
	mutex_destroy(&vfpriv->context_lock);
>>>>>>> upstream/android-13
	kfree(vfpriv);
	file->driver_priv = NULL;
}
