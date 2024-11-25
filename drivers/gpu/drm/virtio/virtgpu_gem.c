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

<<<<<<< HEAD
#include <drm/drmP.h>
#include "virtgpu_drv.h"

void virtio_gpu_gem_free_object(struct drm_gem_object *gem_obj)
{
	struct virtio_gpu_object *obj = gem_to_virtio_gpu_obj(gem_obj);

	if (obj)
		virtio_gpu_object_unref(&obj);
}

struct virtio_gpu_object*
virtio_gpu_alloc_object(struct drm_device *dev,
			struct virtio_gpu_object_params *params,
			struct virtio_gpu_fence *fence)
=======
#include <drm/drm_file.h>
#include <drm/drm_fourcc.h>

#include "virtgpu_drv.h"

static int virtio_gpu_gem_create(struct drm_file *file,
				 struct drm_device *dev,
				 struct virtio_gpu_object_params *params,
				 struct drm_gem_object **obj_p,
				 uint32_t *handle_p)
>>>>>>> upstream/android-13
{
	struct virtio_gpu_device *vgdev = dev->dev_private;
	struct virtio_gpu_object *obj;
	int ret;
<<<<<<< HEAD

	ret = virtio_gpu_object_create(vgdev, params, &obj, fence);
	if (ret)
		return ERR_PTR(ret);

	return obj;
}

int virtio_gpu_gem_create(struct drm_file *file,
			  struct drm_device *dev,
			  struct virtio_gpu_object_params *params,
			  struct drm_gem_object **obj_p,
			  uint32_t *handle_p)
{
	struct virtio_gpu_object *obj;
	int ret;
	u32 handle;

	obj = virtio_gpu_alloc_object(dev, params, NULL);
	if (IS_ERR(obj))
		return PTR_ERR(obj);

	ret = drm_gem_handle_create(file, &obj->gem_base, &handle);
	if (ret) {
		drm_gem_object_release(&obj->gem_base);
		return ret;
	}

	*obj_p = &obj->gem_base;

	/* drop reference from allocate - handle holds it now */
	drm_gem_object_put_unlocked(&obj->gem_base);
=======
	u32 handle;

	ret = virtio_gpu_object_create(vgdev, params, &obj, NULL);
	if (ret < 0)
		return ret;

	ret = drm_gem_handle_create(file, &obj->base.base, &handle);
	if (ret) {
		drm_gem_object_release(&obj->base.base);
		return ret;
	}

	*obj_p = &obj->base.base;

	/* drop reference from allocate - handle holds it now */
	drm_gem_object_put(&obj->base.base);
>>>>>>> upstream/android-13

	*handle_p = handle;
	return 0;
}

int virtio_gpu_mode_dumb_create(struct drm_file *file_priv,
				struct drm_device *dev,
				struct drm_mode_create_dumb *args)
{
	struct drm_gem_object *gobj;
	struct virtio_gpu_object_params params = { 0 };
<<<<<<< HEAD
	int ret;
	uint32_t pitch;

	pitch = args->width * ((args->bpp + 1) / 8);
=======
	struct virtio_gpu_device *vgdev = dev->dev_private;
	int ret;
	uint32_t pitch;

	if (args->bpp != 32)
		return -EINVAL;

	pitch = args->width * 4;
>>>>>>> upstream/android-13
	args->size = pitch * args->height;
	args->size = ALIGN(args->size, PAGE_SIZE);

	params.format = virtio_gpu_translate_format(DRM_FORMAT_HOST_XRGB8888);
	params.width = args->width;
	params.height = args->height;
	params.size = args->size;
	params.dumb = true;
<<<<<<< HEAD
=======

	if (vgdev->has_resource_blob && !vgdev->has_virgl_3d) {
		params.blob_mem = VIRTGPU_BLOB_MEM_GUEST;
		params.blob_flags = VIRTGPU_BLOB_FLAG_USE_SHAREABLE;
		params.blob = true;
	}

>>>>>>> upstream/android-13
	ret = virtio_gpu_gem_create(file_priv, dev, &params, &gobj,
				    &args->handle);
	if (ret)
		goto fail;

	args->pitch = pitch;
	return ret;

fail:
	return ret;
}

int virtio_gpu_mode_dumb_mmap(struct drm_file *file_priv,
			      struct drm_device *dev,
			      uint32_t handle, uint64_t *offset_p)
{
	struct drm_gem_object *gobj;
<<<<<<< HEAD
	struct virtio_gpu_object *obj;
=======
>>>>>>> upstream/android-13

	BUG_ON(!offset_p);
	gobj = drm_gem_object_lookup(file_priv, handle);
	if (gobj == NULL)
		return -ENOENT;
<<<<<<< HEAD
	obj = gem_to_virtio_gpu_obj(gobj);
	*offset_p = virtio_gpu_object_mmap_offset(obj);
	drm_gem_object_put_unlocked(gobj);
=======
	*offset_p = drm_vma_node_offset_addr(&gobj->vma_node);
	drm_gem_object_put(gobj);
>>>>>>> upstream/android-13
	return 0;
}

int virtio_gpu_gem_object_open(struct drm_gem_object *obj,
			       struct drm_file *file)
{
	struct virtio_gpu_device *vgdev = obj->dev->dev_private;
	struct virtio_gpu_fpriv *vfpriv = file->driver_priv;
<<<<<<< HEAD
	struct virtio_gpu_object *qobj = gem_to_virtio_gpu_obj(obj);
	int r;

	if (!vgdev->has_virgl_3d)
		return 0;

	r = virtio_gpu_object_reserve(qobj, false);
	if (r)
		return r;

	virtio_gpu_cmd_context_attach_resource(vgdev, vfpriv->ctx_id,
					       qobj->hw_res_handle);
	virtio_gpu_object_unreserve(qobj);
=======
	struct virtio_gpu_object_array *objs;

	if (!vgdev->has_virgl_3d)
		goto out_notify;

	/* the context might still be missing when the first ioctl is
	 * DRM_IOCTL_MODE_CREATE_DUMB or DRM_IOCTL_PRIME_FD_TO_HANDLE
	 */
	virtio_gpu_create_context(obj->dev, file);

	objs = virtio_gpu_array_alloc(1);
	if (!objs)
		return -ENOMEM;
	virtio_gpu_array_add_obj(objs, obj);

	virtio_gpu_cmd_context_attach_resource(vgdev, vfpriv->ctx_id,
					       objs);
out_notify:
	virtio_gpu_notify(vgdev);
>>>>>>> upstream/android-13
	return 0;
}

void virtio_gpu_gem_object_close(struct drm_gem_object *obj,
				 struct drm_file *file)
{
	struct virtio_gpu_device *vgdev = obj->dev->dev_private;
	struct virtio_gpu_fpriv *vfpriv = file->driver_priv;
<<<<<<< HEAD
	struct virtio_gpu_object *qobj = gem_to_virtio_gpu_obj(obj);
	int r;
=======
	struct virtio_gpu_object_array *objs;
>>>>>>> upstream/android-13

	if (!vgdev->has_virgl_3d)
		return;

<<<<<<< HEAD
	r = virtio_gpu_object_reserve(qobj, false);
	if (r)
		return;

	virtio_gpu_cmd_context_detach_resource(vgdev, vfpriv->ctx_id,
						qobj->hw_res_handle);
	virtio_gpu_object_unreserve(qobj);
=======
	objs = virtio_gpu_array_alloc(1);
	if (!objs)
		return;
	virtio_gpu_array_add_obj(objs, obj);

	virtio_gpu_cmd_context_detach_resource(vgdev, vfpriv->ctx_id,
					       objs);
	virtio_gpu_notify(vgdev);
}

struct virtio_gpu_object_array *virtio_gpu_array_alloc(u32 nents)
{
	struct virtio_gpu_object_array *objs;

	objs = kmalloc(struct_size(objs, objs, nents), GFP_KERNEL);
	if (!objs)
		return NULL;

	objs->nents = 0;
	objs->total = nents;
	return objs;
}

static void virtio_gpu_array_free(struct virtio_gpu_object_array *objs)
{
	kfree(objs);
}

struct virtio_gpu_object_array*
virtio_gpu_array_from_handles(struct drm_file *drm_file, u32 *handles, u32 nents)
{
	struct virtio_gpu_object_array *objs;
	u32 i;

	objs = virtio_gpu_array_alloc(nents);
	if (!objs)
		return NULL;

	for (i = 0; i < nents; i++) {
		objs->objs[i] = drm_gem_object_lookup(drm_file, handles[i]);
		if (!objs->objs[i]) {
			objs->nents = i;
			virtio_gpu_array_put_free(objs);
			return NULL;
		}
	}
	objs->nents = i;
	return objs;
}

void virtio_gpu_array_add_obj(struct virtio_gpu_object_array *objs,
			      struct drm_gem_object *obj)
{
	if (WARN_ON_ONCE(objs->nents == objs->total))
		return;

	drm_gem_object_get(obj);
	objs->objs[objs->nents] = obj;
	objs->nents++;
}

int virtio_gpu_array_lock_resv(struct virtio_gpu_object_array *objs)
{
	int ret;

	if (objs->nents == 1) {
		ret = dma_resv_lock_interruptible(objs->objs[0]->resv, NULL);
	} else {
		ret = drm_gem_lock_reservations(objs->objs, objs->nents,
						&objs->ticket);
	}
	return ret;
}

void virtio_gpu_array_unlock_resv(struct virtio_gpu_object_array *objs)
{
	if (objs->nents == 1) {
		dma_resv_unlock(objs->objs[0]->resv);
	} else {
		drm_gem_unlock_reservations(objs->objs, objs->nents,
					    &objs->ticket);
	}
}

void virtio_gpu_array_add_fence(struct virtio_gpu_object_array *objs,
				struct dma_fence *fence)
{
	int i;

	for (i = 0; i < objs->nents; i++)
		dma_resv_add_excl_fence(objs->objs[i]->resv, fence);
}

void virtio_gpu_array_put_free(struct virtio_gpu_object_array *objs)
{
	u32 i;

	if (!objs)
		return;

	for (i = 0; i < objs->nents; i++)
		drm_gem_object_put(objs->objs[i]);
	virtio_gpu_array_free(objs);
}

void virtio_gpu_array_put_free_delayed(struct virtio_gpu_device *vgdev,
				       struct virtio_gpu_object_array *objs)
{
	spin_lock(&vgdev->obj_free_lock);
	list_add_tail(&objs->next, &vgdev->obj_free_list);
	spin_unlock(&vgdev->obj_free_lock);
	schedule_work(&vgdev->obj_free_work);
}

void virtio_gpu_array_put_free_work(struct work_struct *work)
{
	struct virtio_gpu_device *vgdev =
		container_of(work, struct virtio_gpu_device, obj_free_work);
	struct virtio_gpu_object_array *objs;

	spin_lock(&vgdev->obj_free_lock);
	while (!list_empty(&vgdev->obj_free_list)) {
		objs = list_first_entry(&vgdev->obj_free_list,
					struct virtio_gpu_object_array, next);
		list_del(&objs->next);
		spin_unlock(&vgdev->obj_free_lock);
		virtio_gpu_array_put_free(objs);
		spin_lock(&vgdev->obj_free_lock);
	}
	spin_unlock(&vgdev->obj_free_lock);
>>>>>>> upstream/android-13
}
