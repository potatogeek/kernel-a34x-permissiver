/*
 * Copyright (C) 2015 Red Hat, Inc.
 * All Rights Reserved.
 *
 * Authors:
 *    Dave Airlie
 *    Alon Levy
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
 */

<<<<<<< HEAD
#include <drm/drmP.h>
#include <drm/virtgpu_drm.h>
#include <drm/ttm/ttm_execbuf_util.h>
#include <linux/sync_file.h>

#include "virtgpu_drv.h"

static void convert_to_hw_box(struct virtio_gpu_box *dst,
			      const struct drm_virtgpu_3d_box *src)
{
	dst->x = cpu_to_le32(src->x);
	dst->y = cpu_to_le32(src->y);
	dst->z = cpu_to_le32(src->z);
	dst->w = cpu_to_le32(src->w);
	dst->h = cpu_to_le32(src->h);
	dst->d = cpu_to_le32(src->d);
}

static int virtio_gpu_map_ioctl(struct drm_device *dev, void *data,
				struct drm_file *file_priv)
=======
#include <linux/file.h>
#include <linux/sync_file.h>
#include <linux/uaccess.h>

#include <drm/drm_file.h>
#include <drm/virtgpu_drm.h>

#include "virtgpu_drv.h"

#define VIRTGPU_BLOB_FLAG_USE_MASK (VIRTGPU_BLOB_FLAG_USE_MAPPABLE | \
				    VIRTGPU_BLOB_FLAG_USE_SHAREABLE | \
				    VIRTGPU_BLOB_FLAG_USE_CROSS_DEVICE)

void virtio_gpu_create_context(struct drm_device *dev, struct drm_file *file)
{
	struct virtio_gpu_device *vgdev = dev->dev_private;
	struct virtio_gpu_fpriv *vfpriv = file->driver_priv;
	char dbgname[TASK_COMM_LEN];

	mutex_lock(&vfpriv->context_lock);
	if (vfpriv->context_created)
		goto out_unlock;

	get_task_comm(dbgname, current);
	virtio_gpu_cmd_context_create(vgdev, vfpriv->ctx_id,
				      strlen(dbgname), dbgname);
	vfpriv->context_created = true;

out_unlock:
	mutex_unlock(&vfpriv->context_lock);
}

static int virtio_gpu_map_ioctl(struct drm_device *dev, void *data,
				struct drm_file *file)
>>>>>>> upstream/android-13
{
	struct virtio_gpu_device *vgdev = dev->dev_private;
	struct drm_virtgpu_map *virtio_gpu_map = data;

<<<<<<< HEAD
	return virtio_gpu_mode_dumb_mmap(file_priv, vgdev->ddev,
=======
	return virtio_gpu_mode_dumb_mmap(file, vgdev->ddev,
>>>>>>> upstream/android-13
					 virtio_gpu_map->handle,
					 &virtio_gpu_map->offset);
}

<<<<<<< HEAD
int virtio_gpu_object_list_validate(struct ww_acquire_ctx *ticket,
				    struct list_head *head)
{
	struct ttm_operation_ctx ctx = { false, false };
	struct ttm_validate_buffer *buf;
	struct ttm_buffer_object *bo;
	struct virtio_gpu_object *qobj;
	int ret;

	ret = ttm_eu_reserve_buffers(ticket, head, true, NULL);
	if (ret != 0)
		return ret;

	list_for_each_entry(buf, head, head) {
		bo = buf->bo;
		qobj = container_of(bo, struct virtio_gpu_object, tbo);
		ret = ttm_bo_validate(bo, &qobj->placement, &ctx);
		if (ret) {
			ttm_eu_backoff_reservation(ticket, head);
			return ret;
		}
	}
	return 0;
}

void virtio_gpu_unref_list(struct list_head *head)
{
	struct ttm_validate_buffer *buf;
	struct ttm_buffer_object *bo;
	struct virtio_gpu_object *qobj;

	list_for_each_entry(buf, head, head) {
		bo = buf->bo;
		qobj = container_of(bo, struct virtio_gpu_object, tbo);

		drm_gem_object_put_unlocked(&qobj->gem_base);
	}
}

=======
>>>>>>> upstream/android-13
/*
 * Usage of execbuffer:
 * Relocations need to take into account the full VIRTIO_GPUDrawable size.
 * However, the command as passed from user space must *not* contain the initial
 * VIRTIO_GPUReleaseInfo struct (first XXX bytes)
 */
static int virtio_gpu_execbuffer_ioctl(struct drm_device *dev, void *data,
<<<<<<< HEAD
				 struct drm_file *drm_file)
{
	struct drm_virtgpu_execbuffer *exbuf = data;
	struct virtio_gpu_device *vgdev = dev->dev_private;
	struct virtio_gpu_fpriv *vfpriv = drm_file->driver_priv;
	struct drm_gem_object *gobj;
	struct virtio_gpu_fence *out_fence;
	struct virtio_gpu_object *qobj;
	int ret;
	uint32_t *bo_handles = NULL;
	void __user *user_bo_handles = NULL;
	struct list_head validate_list;
	struct ttm_validate_buffer *buflist = NULL;
	int i;
	struct ww_acquire_ctx ticket;
=======
				 struct drm_file *file)
{
	struct drm_virtgpu_execbuffer *exbuf = data;
	struct virtio_gpu_device *vgdev = dev->dev_private;
	struct virtio_gpu_fpriv *vfpriv = file->driver_priv;
	struct virtio_gpu_fence *out_fence;
	int ret;
	uint32_t *bo_handles = NULL;
	void __user *user_bo_handles = NULL;
	struct virtio_gpu_object_array *buflist = NULL;
>>>>>>> upstream/android-13
	struct sync_file *sync_file;
	int in_fence_fd = exbuf->fence_fd;
	int out_fence_fd = -1;
	void *buf;

	if (vgdev->has_virgl_3d == false)
		return -ENOSYS;

	if ((exbuf->flags & ~VIRTGPU_EXECBUF_FLAGS))
		return -EINVAL;

	exbuf->fence_fd = -1;

<<<<<<< HEAD
=======
	virtio_gpu_create_context(dev, file);
>>>>>>> upstream/android-13
	if (exbuf->flags & VIRTGPU_EXECBUF_FENCE_FD_IN) {
		struct dma_fence *in_fence;

		in_fence = sync_file_get_fence(in_fence_fd);

		if (!in_fence)
			return -EINVAL;

		/*
		 * Wait if the fence is from a foreign context, or if the fence
		 * array contains any fence from a foreign context.
		 */
		ret = 0;
		if (!dma_fence_match_context(in_fence, vgdev->fence_drv.context))
			ret = dma_fence_wait(in_fence, true);

		dma_fence_put(in_fence);
		if (ret)
			return ret;
	}

	if (exbuf->flags & VIRTGPU_EXECBUF_FENCE_FD_OUT) {
		out_fence_fd = get_unused_fd_flags(O_CLOEXEC);
		if (out_fence_fd < 0)
			return out_fence_fd;
	}

<<<<<<< HEAD
	INIT_LIST_HEAD(&validate_list);
	if (exbuf->num_bo_handles) {

		bo_handles = kvmalloc_array(exbuf->num_bo_handles,
					   sizeof(uint32_t), GFP_KERNEL);
		buflist = kvmalloc_array(exbuf->num_bo_handles,
					   sizeof(struct ttm_validate_buffer),
					   GFP_KERNEL | __GFP_ZERO);
		if (!bo_handles || !buflist) {
=======
	if (exbuf->num_bo_handles) {
		bo_handles = kvmalloc_array(exbuf->num_bo_handles,
					    sizeof(uint32_t), GFP_KERNEL);
		if (!bo_handles) {
>>>>>>> upstream/android-13
			ret = -ENOMEM;
			goto out_unused_fd;
		}

		user_bo_handles = u64_to_user_ptr(exbuf->bo_handles);
		if (copy_from_user(bo_handles, user_bo_handles,
				   exbuf->num_bo_handles * sizeof(uint32_t))) {
			ret = -EFAULT;
			goto out_unused_fd;
		}

<<<<<<< HEAD
		for (i = 0; i < exbuf->num_bo_handles; i++) {
			gobj = drm_gem_object_lookup(drm_file, bo_handles[i]);
			if (!gobj) {
				ret = -ENOENT;
				goto out_unused_fd;
			}

			qobj = gem_to_virtio_gpu_obj(gobj);
			buflist[i].bo = &qobj->tbo;

			list_add(&buflist[i].head, &validate_list);
=======
		buflist = virtio_gpu_array_from_handles(file, bo_handles,
							exbuf->num_bo_handles);
		if (!buflist) {
			ret = -ENOENT;
			goto out_unused_fd;
>>>>>>> upstream/android-13
		}
		kvfree(bo_handles);
		bo_handles = NULL;
	}

<<<<<<< HEAD
	ret = virtio_gpu_object_list_validate(&ticket, &validate_list);
	if (ret)
		goto out_free;

	buf = vmemdup_user(u64_to_user_ptr(exbuf->command), exbuf->size);
	if (IS_ERR(buf)) {
		ret = PTR_ERR(buf);
		goto out_unresv;
=======
	buf = vmemdup_user(u64_to_user_ptr(exbuf->command), exbuf->size);
	if (IS_ERR(buf)) {
		ret = PTR_ERR(buf);
		goto out_unused_fd;
	}

	if (buflist) {
		ret = virtio_gpu_array_lock_resv(buflist);
		if (ret)
			goto out_memdup;
>>>>>>> upstream/android-13
	}

	out_fence = virtio_gpu_fence_alloc(vgdev);
	if(!out_fence) {
		ret = -ENOMEM;
<<<<<<< HEAD
		goto out_memdup;
=======
		goto out_unresv;
>>>>>>> upstream/android-13
	}

	if (out_fence_fd >= 0) {
		sync_file = sync_file_create(&out_fence->f);
		if (!sync_file) {
			dma_fence_put(&out_fence->f);
			ret = -ENOMEM;
<<<<<<< HEAD
			goto out_memdup;
=======
			goto out_unresv;
>>>>>>> upstream/android-13
		}

		exbuf->fence_fd = out_fence_fd;
		fd_install(out_fence_fd, sync_file->file);
	}

	virtio_gpu_cmd_submit(vgdev, buf, exbuf->size,
<<<<<<< HEAD
			      vfpriv->ctx_id, out_fence);

	ttm_eu_fence_buffer_objects(&ticket, &validate_list, &out_fence->f);

	/* fence the command bo */
	virtio_gpu_unref_list(&validate_list);
	kvfree(buflist);
	return 0;

out_memdup:
	kvfree(buf);
out_unresv:
	ttm_eu_backoff_reservation(&ticket, &validate_list);
out_free:
	virtio_gpu_unref_list(&validate_list);
out_unused_fd:
	kvfree(bo_handles);
	kvfree(buflist);
=======
			      vfpriv->ctx_id, buflist, out_fence);
	dma_fence_put(&out_fence->f);
	virtio_gpu_notify(vgdev);
	return 0;

out_unresv:
	if (buflist)
		virtio_gpu_array_unlock_resv(buflist);
out_memdup:
	kvfree(buf);
out_unused_fd:
	kvfree(bo_handles);
	if (buflist)
		virtio_gpu_array_put_free(buflist);
>>>>>>> upstream/android-13

	if (out_fence_fd >= 0)
		put_unused_fd(out_fence_fd);

	return ret;
}

static int virtio_gpu_getparam_ioctl(struct drm_device *dev, void *data,
<<<<<<< HEAD
				     struct drm_file *file_priv)
=======
				     struct drm_file *file)
>>>>>>> upstream/android-13
{
	struct virtio_gpu_device *vgdev = dev->dev_private;
	struct drm_virtgpu_getparam *param = data;
	int value;

	switch (param->param) {
	case VIRTGPU_PARAM_3D_FEATURES:
<<<<<<< HEAD
		value = vgdev->has_virgl_3d == true ? 1 : 0;
=======
		value = vgdev->has_virgl_3d ? 1 : 0;
>>>>>>> upstream/android-13
		break;
	case VIRTGPU_PARAM_CAPSET_QUERY_FIX:
		value = 1;
		break;
<<<<<<< HEAD
=======
	case VIRTGPU_PARAM_RESOURCE_BLOB:
		value = vgdev->has_resource_blob ? 1 : 0;
		break;
	case VIRTGPU_PARAM_HOST_VISIBLE:
		value = vgdev->has_host_visible ? 1 : 0;
		break;
	case VIRTGPU_PARAM_CROSS_DEVICE:
		value = vgdev->has_resource_assign_uuid ? 1 : 0;
		break;
>>>>>>> upstream/android-13
	default:
		return -EINVAL;
	}
	if (copy_to_user(u64_to_user_ptr(param->value), &value, sizeof(int)))
		return -EFAULT;

	return 0;
}

static int virtio_gpu_resource_create_ioctl(struct drm_device *dev, void *data,
<<<<<<< HEAD
					    struct drm_file *file_priv)
=======
					    struct drm_file *file)
>>>>>>> upstream/android-13
{
	struct virtio_gpu_device *vgdev = dev->dev_private;
	struct drm_virtgpu_resource_create *rc = data;
	struct virtio_gpu_fence *fence;
	int ret;
	struct virtio_gpu_object *qobj;
	struct drm_gem_object *obj;
	uint32_t handle = 0;
	struct virtio_gpu_object_params params = { 0 };

<<<<<<< HEAD
	if (vgdev->has_virgl_3d == false) {
=======
	if (vgdev->has_virgl_3d) {
		virtio_gpu_create_context(dev, file);
		params.virgl = true;
		params.target = rc->target;
		params.bind = rc->bind;
		params.depth = rc->depth;
		params.array_size = rc->array_size;
		params.last_level = rc->last_level;
		params.nr_samples = rc->nr_samples;
		params.flags = rc->flags;
	} else {
>>>>>>> upstream/android-13
		if (rc->depth > 1)
			return -EINVAL;
		if (rc->nr_samples > 1)
			return -EINVAL;
		if (rc->last_level > 1)
			return -EINVAL;
		if (rc->target != 2)
			return -EINVAL;
		if (rc->array_size > 1)
			return -EINVAL;
	}

	params.format = rc->format;
	params.width = rc->width;
	params.height = rc->height;
	params.size = rc->size;
<<<<<<< HEAD
	if (vgdev->has_virgl_3d) {
		params.virgl = true;
		params.target = rc->target;
		params.bind = rc->bind;
		params.depth = rc->depth;
		params.array_size = rc->array_size;
		params.last_level = rc->last_level;
		params.nr_samples = rc->nr_samples;
		params.flags = rc->flags;
	}
=======
>>>>>>> upstream/android-13
	/* allocate a single page size object */
	if (params.size == 0)
		params.size = PAGE_SIZE;

	fence = virtio_gpu_fence_alloc(vgdev);
	if (!fence)
		return -ENOMEM;
<<<<<<< HEAD
	qobj = virtio_gpu_alloc_object(dev, &params, fence);
	dma_fence_put(&fence->f);
	if (IS_ERR(qobj))
		return PTR_ERR(qobj);
	obj = &qobj->gem_base;

	ret = drm_gem_handle_create(file_priv, obj, &handle);
=======
	ret = virtio_gpu_object_create(vgdev, &params, &qobj, fence);
	dma_fence_put(&fence->f);
	if (ret < 0)
		return ret;
	obj = &qobj->base.base;

	ret = drm_gem_handle_create(file, obj, &handle);
>>>>>>> upstream/android-13
	if (ret) {
		drm_gem_object_release(obj);
		return ret;
	}
<<<<<<< HEAD
	drm_gem_object_put_unlocked(obj);
=======
	drm_gem_object_put(obj);
>>>>>>> upstream/android-13

	rc->res_handle = qobj->hw_res_handle; /* similiar to a VM address */
	rc->bo_handle = handle;
	return 0;
}

static int virtio_gpu_resource_info_ioctl(struct drm_device *dev, void *data,
<<<<<<< HEAD
					  struct drm_file *file_priv)
=======
					  struct drm_file *file)
>>>>>>> upstream/android-13
{
	struct drm_virtgpu_resource_info *ri = data;
	struct drm_gem_object *gobj = NULL;
	struct virtio_gpu_object *qobj = NULL;

<<<<<<< HEAD
	gobj = drm_gem_object_lookup(file_priv, ri->bo_handle);
=======
	gobj = drm_gem_object_lookup(file, ri->bo_handle);
>>>>>>> upstream/android-13
	if (gobj == NULL)
		return -ENOENT;

	qobj = gem_to_virtio_gpu_obj(gobj);

<<<<<<< HEAD
	ri->size = qobj->gem_base.size;
	ri->res_handle = qobj->hw_res_handle;
	drm_gem_object_put_unlocked(gobj);
=======
	ri->size = qobj->base.base.size;
	ri->res_handle = qobj->hw_res_handle;
	if (qobj->host3d_blob || qobj->guest_blob)
		ri->blob_mem = qobj->blob_mem;

	drm_gem_object_put(gobj);
>>>>>>> upstream/android-13
	return 0;
}

static int virtio_gpu_transfer_from_host_ioctl(struct drm_device *dev,
					       void *data,
					       struct drm_file *file)
{
	struct virtio_gpu_device *vgdev = dev->dev_private;
	struct virtio_gpu_fpriv *vfpriv = file->driver_priv;
	struct drm_virtgpu_3d_transfer_from_host *args = data;
<<<<<<< HEAD
	struct ttm_operation_ctx ctx = { true, false };
	struct drm_gem_object *gobj = NULL;
	struct virtio_gpu_object *qobj = NULL;
	struct virtio_gpu_fence *fence;
	int ret;
	u32 offset = args->offset;
	struct virtio_gpu_box box;
=======
	struct virtio_gpu_object *bo;
	struct virtio_gpu_object_array *objs;
	struct virtio_gpu_fence *fence;
	int ret;
	u32 offset = args->offset;
>>>>>>> upstream/android-13

	if (vgdev->has_virgl_3d == false)
		return -ENOSYS;

<<<<<<< HEAD
	gobj = drm_gem_object_lookup(file, args->bo_handle);
	if (gobj == NULL)
		return -ENOENT;

	qobj = gem_to_virtio_gpu_obj(gobj);

	ret = virtio_gpu_object_reserve(qobj, false);
	if (ret)
		goto out;

	ret = ttm_bo_validate(&qobj->tbo, &qobj->placement, &ctx);
	if (unlikely(ret))
		goto out_unres;

	convert_to_hw_box(&box, &args->box);
=======
	virtio_gpu_create_context(dev, file);
	objs = virtio_gpu_array_from_handles(file, &args->bo_handle, 1);
	if (objs == NULL)
		return -ENOENT;

	bo = gem_to_virtio_gpu_obj(objs->objs[0]);
	if (bo->guest_blob && !bo->host3d_blob) {
		ret = -EINVAL;
		goto err_put_free;
	}

	if (!bo->host3d_blob && (args->stride || args->layer_stride)) {
		ret = -EINVAL;
		goto err_put_free;
	}

	ret = virtio_gpu_array_lock_resv(objs);
	if (ret != 0)
		goto err_put_free;
>>>>>>> upstream/android-13

	fence = virtio_gpu_fence_alloc(vgdev);
	if (!fence) {
		ret = -ENOMEM;
<<<<<<< HEAD
		goto out_unres;
	}
	virtio_gpu_cmd_transfer_from_host_3d
		(vgdev, qobj->hw_res_handle,
		 vfpriv->ctx_id, offset, args->level,
		 &box, fence);
	reservation_object_add_excl_fence(qobj->tbo.resv,
					  &fence->f);

	dma_fence_put(&fence->f);
out_unres:
	virtio_gpu_object_unreserve(qobj);
out:
	drm_gem_object_put_unlocked(gobj);
=======
		goto err_unlock;
	}

	virtio_gpu_cmd_transfer_from_host_3d
		(vgdev, vfpriv->ctx_id, offset, args->level, args->stride,
		 args->layer_stride, &args->box, objs, fence);
	dma_fence_put(&fence->f);
	virtio_gpu_notify(vgdev);
	return 0;

err_unlock:
	virtio_gpu_array_unlock_resv(objs);
err_put_free:
	virtio_gpu_array_put_free(objs);
>>>>>>> upstream/android-13
	return ret;
}

static int virtio_gpu_transfer_to_host_ioctl(struct drm_device *dev, void *data,
					     struct drm_file *file)
{
	struct virtio_gpu_device *vgdev = dev->dev_private;
	struct virtio_gpu_fpriv *vfpriv = file->driver_priv;
	struct drm_virtgpu_3d_transfer_to_host *args = data;
<<<<<<< HEAD
	struct ttm_operation_ctx ctx = { true, false };
	struct drm_gem_object *gobj = NULL;
	struct virtio_gpu_object *qobj = NULL;
	struct virtio_gpu_fence *fence;
	struct virtio_gpu_box box;
	int ret;
	u32 offset = args->offset;

	gobj = drm_gem_object_lookup(file, args->bo_handle);
	if (gobj == NULL)
		return -ENOENT;

	qobj = gem_to_virtio_gpu_obj(gobj);

	ret = virtio_gpu_object_reserve(qobj, false);
	if (ret)
		goto out;

	ret = ttm_bo_validate(&qobj->tbo, &qobj->placement, &ctx);
	if (unlikely(ret))
		goto out_unres;

	convert_to_hw_box(&box, &args->box);
	if (!vgdev->has_virgl_3d) {
		virtio_gpu_cmd_transfer_to_host_2d
			(vgdev, qobj, offset,
			 box.w, box.h, box.x, box.y, NULL);
	} else {
		fence = virtio_gpu_fence_alloc(vgdev);
		if (!fence) {
			ret = -ENOMEM;
			goto out_unres;
		}
		virtio_gpu_cmd_transfer_to_host_3d
			(vgdev, qobj,
			 vfpriv ? vfpriv->ctx_id : 0, offset,
			 args->level, &box, fence);
		reservation_object_add_excl_fence(qobj->tbo.resv,
						  &fence->f);
		dma_fence_put(&fence->f);
	}

out_unres:
	virtio_gpu_object_unreserve(qobj);
out:
	drm_gem_object_put_unlocked(gobj);
=======
	struct virtio_gpu_object *bo;
	struct virtio_gpu_object_array *objs;
	struct virtio_gpu_fence *fence;
	int ret;
	u32 offset = args->offset;

	objs = virtio_gpu_array_from_handles(file, &args->bo_handle, 1);
	if (objs == NULL)
		return -ENOENT;

	bo = gem_to_virtio_gpu_obj(objs->objs[0]);
	if (bo->guest_blob && !bo->host3d_blob) {
		ret = -EINVAL;
		goto err_put_free;
	}

	if (!vgdev->has_virgl_3d) {
		virtio_gpu_cmd_transfer_to_host_2d
			(vgdev, offset,
			 args->box.w, args->box.h, args->box.x, args->box.y,
			 objs, NULL);
	} else {
		virtio_gpu_create_context(dev, file);

		if (!bo->host3d_blob && (args->stride || args->layer_stride)) {
			ret = -EINVAL;
			goto err_put_free;
		}

		ret = virtio_gpu_array_lock_resv(objs);
		if (ret != 0)
			goto err_put_free;

		ret = -ENOMEM;
		fence = virtio_gpu_fence_alloc(vgdev);
		if (!fence)
			goto err_unlock;

		virtio_gpu_cmd_transfer_to_host_3d
			(vgdev,
			 vfpriv ? vfpriv->ctx_id : 0, offset, args->level,
			 args->stride, args->layer_stride, &args->box, objs,
			 fence);
		dma_fence_put(&fence->f);
	}
	virtio_gpu_notify(vgdev);
	return 0;

err_unlock:
	virtio_gpu_array_unlock_resv(objs);
err_put_free:
	virtio_gpu_array_put_free(objs);
>>>>>>> upstream/android-13
	return ret;
}

static int virtio_gpu_wait_ioctl(struct drm_device *dev, void *data,
<<<<<<< HEAD
			    struct drm_file *file)
{
	struct drm_virtgpu_3d_wait *args = data;
	struct drm_gem_object *gobj = NULL;
	struct virtio_gpu_object *qobj = NULL;
	int ret;
	bool nowait = false;

	gobj = drm_gem_object_lookup(file, args->handle);
	if (gobj == NULL)
		return -ENOENT;

	qobj = gem_to_virtio_gpu_obj(gobj);

	if (args->flags & VIRTGPU_WAIT_NOWAIT)
		nowait = true;
	ret = virtio_gpu_object_wait(qobj, nowait);

	drm_gem_object_put_unlocked(gobj);
=======
				 struct drm_file *file)
{
	struct drm_virtgpu_3d_wait *args = data;
	struct drm_gem_object *obj;
	long timeout = 15 * HZ;
	int ret;

	obj = drm_gem_object_lookup(file, args->handle);
	if (obj == NULL)
		return -ENOENT;

	if (args->flags & VIRTGPU_WAIT_NOWAIT) {
		ret = dma_resv_test_signaled(obj->resv, true);
	} else {
		ret = dma_resv_wait_timeout(obj->resv, true, true, timeout);
	}
	if (ret == 0)
		ret = -EBUSY;
	else if (ret > 0)
		ret = 0;

	drm_gem_object_put(obj);
>>>>>>> upstream/android-13
	return ret;
}

static int virtio_gpu_get_caps_ioctl(struct drm_device *dev,
				void *data, struct drm_file *file)
{
	struct virtio_gpu_device *vgdev = dev->dev_private;
	struct drm_virtgpu_get_caps *args = data;
	unsigned size, host_caps_size;
	int i;
	int found_valid = -1;
	int ret;
	struct virtio_gpu_drv_cap_cache *cache_ent;
	void *ptr;

	if (vgdev->num_capsets == 0)
		return -ENOSYS;

	/* don't allow userspace to pass 0 */
	if (args->size == 0)
		return -EINVAL;

	spin_lock(&vgdev->display_info_lock);
	for (i = 0; i < vgdev->num_capsets; i++) {
		if (vgdev->capsets[i].id == args->cap_set_id) {
			if (vgdev->capsets[i].max_version >= args->cap_set_ver) {
				found_valid = i;
				break;
			}
		}
	}

	if (found_valid == -1) {
		spin_unlock(&vgdev->display_info_lock);
		return -EINVAL;
	}

	host_caps_size = vgdev->capsets[found_valid].max_size;
	/* only copy to user the minimum of the host caps size or the guest caps size */
	size = min(args->size, host_caps_size);

	list_for_each_entry(cache_ent, &vgdev->cap_cache, head) {
		if (cache_ent->id == args->cap_set_id &&
		    cache_ent->version == args->cap_set_ver) {
			spin_unlock(&vgdev->display_info_lock);
			goto copy_exit;
		}
	}
	spin_unlock(&vgdev->display_info_lock);

	/* not in cache - need to talk to hw */
	virtio_gpu_cmd_get_capset(vgdev, found_valid, args->cap_set_ver,
				  &cache_ent);
<<<<<<< HEAD
=======
	virtio_gpu_notify(vgdev);
>>>>>>> upstream/android-13

copy_exit:
	ret = wait_event_timeout(vgdev->resp_wq,
				 atomic_read(&cache_ent->is_valid), 5 * HZ);
	if (!ret)
		return -EBUSY;

	/* is_valid check must proceed before copy of the cache entry. */
	smp_rmb();

	ptr = cache_ent->caps_cache;

	if (copy_to_user(u64_to_user_ptr(args->addr), ptr, size))
		return -EFAULT;

	return 0;
}

<<<<<<< HEAD
struct drm_ioctl_desc virtio_gpu_ioctls[DRM_VIRTIO_NUM_IOCTLS] = {
	DRM_IOCTL_DEF_DRV(VIRTGPU_MAP, virtio_gpu_map_ioctl,
			  DRM_AUTH | DRM_RENDER_ALLOW),

	DRM_IOCTL_DEF_DRV(VIRTGPU_EXECBUFFER, virtio_gpu_execbuffer_ioctl,
			  DRM_AUTH | DRM_RENDER_ALLOW),

	DRM_IOCTL_DEF_DRV(VIRTGPU_GETPARAM, virtio_gpu_getparam_ioctl,
			  DRM_AUTH | DRM_RENDER_ALLOW),

	DRM_IOCTL_DEF_DRV(VIRTGPU_RESOURCE_CREATE,
			  virtio_gpu_resource_create_ioctl,
			  DRM_AUTH | DRM_RENDER_ALLOW),

	DRM_IOCTL_DEF_DRV(VIRTGPU_RESOURCE_INFO, virtio_gpu_resource_info_ioctl,
			  DRM_AUTH | DRM_RENDER_ALLOW),
=======
static int verify_blob(struct virtio_gpu_device *vgdev,
		       struct virtio_gpu_fpriv *vfpriv,
		       struct virtio_gpu_object_params *params,
		       struct drm_virtgpu_resource_create_blob *rc_blob,
		       bool *guest_blob, bool *host3d_blob)
{
	if (!vgdev->has_resource_blob)
		return -EINVAL;

	if ((rc_blob->blob_flags & ~VIRTGPU_BLOB_FLAG_USE_MASK) ||
	    !rc_blob->blob_flags)
		return -EINVAL;

	if (rc_blob->blob_flags & VIRTGPU_BLOB_FLAG_USE_CROSS_DEVICE) {
		if (!vgdev->has_resource_assign_uuid)
			return -EINVAL;
	}

	switch (rc_blob->blob_mem) {
	case VIRTGPU_BLOB_MEM_GUEST:
		*guest_blob = true;
		break;
	case VIRTGPU_BLOB_MEM_HOST3D_GUEST:
		*guest_blob = true;
		fallthrough;
	case VIRTGPU_BLOB_MEM_HOST3D:
		*host3d_blob = true;
		break;
	default:
		return -EINVAL;
	}

	if (*host3d_blob) {
		if (!vgdev->has_virgl_3d)
			return -EINVAL;

		/* Must be dword aligned. */
		if (rc_blob->cmd_size % 4 != 0)
			return -EINVAL;

		params->ctx_id = vfpriv->ctx_id;
		params->blob_id = rc_blob->blob_id;
	} else {
		if (rc_blob->blob_id != 0)
			return -EINVAL;

		if (rc_blob->cmd_size != 0)
			return -EINVAL;
	}

	params->blob_mem = rc_blob->blob_mem;
	params->size = rc_blob->size;
	params->blob = true;
	params->blob_flags = rc_blob->blob_flags;
	return 0;
}

static int virtio_gpu_resource_create_blob_ioctl(struct drm_device *dev,
						 void *data,
						 struct drm_file *file)
{
	int ret = 0;
	uint32_t handle = 0;
	bool guest_blob = false;
	bool host3d_blob = false;
	struct drm_gem_object *obj;
	struct virtio_gpu_object *bo;
	struct virtio_gpu_object_params params = { 0 };
	struct virtio_gpu_device *vgdev = dev->dev_private;
	struct virtio_gpu_fpriv *vfpriv = file->driver_priv;
	struct drm_virtgpu_resource_create_blob *rc_blob = data;

	if (verify_blob(vgdev, vfpriv, &params, rc_blob,
			&guest_blob, &host3d_blob))
		return -EINVAL;

	if (vgdev->has_virgl_3d)
		virtio_gpu_create_context(dev, file);

	if (rc_blob->cmd_size) {
		void *buf;

		buf = memdup_user(u64_to_user_ptr(rc_blob->cmd),
				  rc_blob->cmd_size);

		if (IS_ERR(buf))
			return PTR_ERR(buf);

		virtio_gpu_cmd_submit(vgdev, buf, rc_blob->cmd_size,
				      vfpriv->ctx_id, NULL, NULL);
	}

	if (guest_blob)
		ret = virtio_gpu_object_create(vgdev, &params, &bo, NULL);
	else if (!guest_blob && host3d_blob)
		ret = virtio_gpu_vram_create(vgdev, &params, &bo);
	else
		return -EINVAL;

	if (ret < 0)
		return ret;

	bo->guest_blob = guest_blob;
	bo->host3d_blob = host3d_blob;
	bo->blob_mem = rc_blob->blob_mem;
	bo->blob_flags = rc_blob->blob_flags;

	obj = &bo->base.base;
	if (params.blob_flags & VIRTGPU_BLOB_FLAG_USE_CROSS_DEVICE) {
		ret = virtio_gpu_resource_assign_uuid(vgdev, bo);
		if (ret) {
			drm_gem_object_release(obj);
			return ret;
		}
	}

	ret = drm_gem_handle_create(file, obj, &handle);
	if (ret) {
		drm_gem_object_release(obj);
		return ret;
	}
	drm_gem_object_put(obj);

	rc_blob->res_handle = bo->hw_res_handle;
	rc_blob->bo_handle = handle;

	return 0;
}

struct drm_ioctl_desc virtio_gpu_ioctls[DRM_VIRTIO_NUM_IOCTLS] = {
	DRM_IOCTL_DEF_DRV(VIRTGPU_MAP, virtio_gpu_map_ioctl,
			  DRM_RENDER_ALLOW),

	DRM_IOCTL_DEF_DRV(VIRTGPU_EXECBUFFER, virtio_gpu_execbuffer_ioctl,
			  DRM_RENDER_ALLOW),

	DRM_IOCTL_DEF_DRV(VIRTGPU_GETPARAM, virtio_gpu_getparam_ioctl,
			  DRM_RENDER_ALLOW),

	DRM_IOCTL_DEF_DRV(VIRTGPU_RESOURCE_CREATE,
			  virtio_gpu_resource_create_ioctl,
			  DRM_RENDER_ALLOW),

	DRM_IOCTL_DEF_DRV(VIRTGPU_RESOURCE_INFO, virtio_gpu_resource_info_ioctl,
			  DRM_RENDER_ALLOW),
>>>>>>> upstream/android-13

	/* make transfer async to the main ring? - no sure, can we
	 * thread these in the underlying GL
	 */
	DRM_IOCTL_DEF_DRV(VIRTGPU_TRANSFER_FROM_HOST,
			  virtio_gpu_transfer_from_host_ioctl,
<<<<<<< HEAD
			  DRM_AUTH | DRM_RENDER_ALLOW),
	DRM_IOCTL_DEF_DRV(VIRTGPU_TRANSFER_TO_HOST,
			  virtio_gpu_transfer_to_host_ioctl,
			  DRM_AUTH | DRM_RENDER_ALLOW),

	DRM_IOCTL_DEF_DRV(VIRTGPU_WAIT, virtio_gpu_wait_ioctl,
			  DRM_AUTH | DRM_RENDER_ALLOW),

	DRM_IOCTL_DEF_DRV(VIRTGPU_GET_CAPS, virtio_gpu_get_caps_ioctl,
			  DRM_AUTH | DRM_RENDER_ALLOW),
=======
			  DRM_RENDER_ALLOW),
	DRM_IOCTL_DEF_DRV(VIRTGPU_TRANSFER_TO_HOST,
			  virtio_gpu_transfer_to_host_ioctl,
			  DRM_RENDER_ALLOW),

	DRM_IOCTL_DEF_DRV(VIRTGPU_WAIT, virtio_gpu_wait_ioctl,
			  DRM_RENDER_ALLOW),

	DRM_IOCTL_DEF_DRV(VIRTGPU_GET_CAPS, virtio_gpu_get_caps_ioctl,
			  DRM_RENDER_ALLOW),

	DRM_IOCTL_DEF_DRV(VIRTGPU_RESOURCE_CREATE_BLOB,
			  virtio_gpu_resource_create_blob_ioctl,
			  DRM_RENDER_ALLOW),
>>>>>>> upstream/android-13
};
