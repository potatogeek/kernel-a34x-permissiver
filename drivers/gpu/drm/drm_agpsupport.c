<<<<<<< HEAD
/**
=======
/*
>>>>>>> upstream/android-13
 * \file drm_agpsupport.c
 * DRM support for AGP/GART backend
 *
 * \author Rickard E. (Rik) Faith <faith@valinux.com>
 * \author Gareth Hughes <gareth@valinux.com>
 */

/*
 * Copyright 1999 Precision Insight, Inc., Cedar Park, Texas.
 * Copyright 2000 VA Linux Systems, Inc., Sunnyvale, California.
 * All Rights Reserved.
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
 * VA LINUX SYSTEMS AND/OR ITS SUPPLIERS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

<<<<<<< HEAD
#include <drm/drmP.h>
#include <linux/module.h>
#include <linux/slab.h>
#include "drm_legacy.h"

#include <asm/agp.h>

/**
 * Get AGP information.
 *
 * \param inode device inode.
 * \param file_priv DRM file private.
 * \param cmd command.
 * \param arg pointer to a (output) drm_agp_info structure.
=======
#include <linux/module.h>
#include <linux/pci.h>
#include <linux/slab.h>

#if IS_ENABLED(CONFIG_AGP)
#include <asm/agp.h>
#endif

#include <drm/drm_device.h>
#include <drm/drm_drv.h>
#include <drm/drm_file.h>
#include <drm/drm_print.h>

#include "drm_legacy.h"

#if IS_ENABLED(CONFIG_AGP)

/*
 * Get AGP information.
 *
>>>>>>> upstream/android-13
 * \return zero on success or a negative number on failure.
 *
 * Verifies the AGP device has been initialized and acquired and fills in the
 * drm_agp_info structure with the information in drm_agp_head::agp_info.
 */
<<<<<<< HEAD
int drm_agp_info(struct drm_device *dev, struct drm_agp_info *info)
=======
int drm_legacy_agp_info(struct drm_device *dev, struct drm_agp_info *info)
>>>>>>> upstream/android-13
{
	struct agp_kern_info *kern;

	if (!dev->agp || !dev->agp->acquired)
		return -EINVAL;

	kern = &dev->agp->agp_info;
	info->agp_version_major = kern->version.major;
	info->agp_version_minor = kern->version.minor;
	info->mode = kern->mode;
	info->aperture_base = kern->aper_base;
	info->aperture_size = kern->aper_size * 1024 * 1024;
	info->memory_allowed = kern->max_memory << PAGE_SHIFT;
	info->memory_used = kern->current_memory << PAGE_SHIFT;
	info->id_vendor = kern->device->vendor;
	info->id_device = kern->device->device;

	return 0;
}
<<<<<<< HEAD
EXPORT_SYMBOL(drm_agp_info);

int drm_agp_info_ioctl(struct drm_device *dev, void *data,
		       struct drm_file *file_priv)
=======
EXPORT_SYMBOL(drm_legacy_agp_info);

int drm_legacy_agp_info_ioctl(struct drm_device *dev, void *data,
			      struct drm_file *file_priv)
>>>>>>> upstream/android-13
{
	struct drm_agp_info *info = data;
	int err;

<<<<<<< HEAD
	err = drm_agp_info(dev, info);
=======
	err = drm_legacy_agp_info(dev, info);
>>>>>>> upstream/android-13
	if (err)
		return err;

	return 0;
}

<<<<<<< HEAD
/**
=======
/*
>>>>>>> upstream/android-13
 * Acquire the AGP device.
 *
 * \param dev DRM device that is to acquire AGP.
 * \return zero on success or a negative number on failure.
 *
 * Verifies the AGP device hasn't been acquired before and calls
 * \c agp_backend_acquire.
 */
<<<<<<< HEAD
int drm_agp_acquire(struct drm_device *dev)
{
=======
int drm_legacy_agp_acquire(struct drm_device *dev)
{
	struct pci_dev *pdev = to_pci_dev(dev->dev);

>>>>>>> upstream/android-13
	if (!dev->agp)
		return -ENODEV;
	if (dev->agp->acquired)
		return -EBUSY;
<<<<<<< HEAD
	dev->agp->bridge = agp_backend_acquire(dev->pdev);
=======
	dev->agp->bridge = agp_backend_acquire(pdev);
>>>>>>> upstream/android-13
	if (!dev->agp->bridge)
		return -ENODEV;
	dev->agp->acquired = 1;
	return 0;
}
<<<<<<< HEAD
EXPORT_SYMBOL(drm_agp_acquire);

/**
 * Acquire the AGP device (ioctl).
 *
 * \param inode device inode.
 * \param file_priv DRM file private.
 * \param cmd command.
 * \param arg user argument.
=======
EXPORT_SYMBOL(drm_legacy_agp_acquire);

/*
 * Acquire the AGP device (ioctl).
 *
>>>>>>> upstream/android-13
 * \return zero on success or a negative number on failure.
 *
 * Verifies the AGP device hasn't been acquired before and calls
 * \c agp_backend_acquire.
 */
<<<<<<< HEAD
int drm_agp_acquire_ioctl(struct drm_device *dev, void *data,
			  struct drm_file *file_priv)
{
	return drm_agp_acquire((struct drm_device *) file_priv->minor->dev);
}

/**
=======
int drm_legacy_agp_acquire_ioctl(struct drm_device *dev, void *data,
				 struct drm_file *file_priv)
{
	return drm_legacy_agp_acquire((struct drm_device *)file_priv->minor->dev);
}

/*
>>>>>>> upstream/android-13
 * Release the AGP device.
 *
 * \param dev DRM device that is to release AGP.
 * \return zero on success or a negative number on failure.
 *
 * Verifies the AGP device has been acquired and calls \c agp_backend_release.
 */
<<<<<<< HEAD
int drm_agp_release(struct drm_device *dev)
=======
int drm_legacy_agp_release(struct drm_device *dev)
>>>>>>> upstream/android-13
{
	if (!dev->agp || !dev->agp->acquired)
		return -EINVAL;
	agp_backend_release(dev->agp->bridge);
	dev->agp->acquired = 0;
	return 0;
}
<<<<<<< HEAD
EXPORT_SYMBOL(drm_agp_release);

int drm_agp_release_ioctl(struct drm_device *dev, void *data,
			  struct drm_file *file_priv)
{
	return drm_agp_release(dev);
}

/**
=======
EXPORT_SYMBOL(drm_legacy_agp_release);

int drm_legacy_agp_release_ioctl(struct drm_device *dev, void *data,
				 struct drm_file *file_priv)
{
	return drm_legacy_agp_release(dev);
}

/*
>>>>>>> upstream/android-13
 * Enable the AGP bus.
 *
 * \param dev DRM device that has previously acquired AGP.
 * \param mode Requested AGP mode.
 * \return zero on success or a negative number on failure.
 *
 * Verifies the AGP device has been acquired but not enabled, and calls
 * \c agp_enable.
 */
<<<<<<< HEAD
int drm_agp_enable(struct drm_device *dev, struct drm_agp_mode mode)
=======
int drm_legacy_agp_enable(struct drm_device *dev, struct drm_agp_mode mode)
>>>>>>> upstream/android-13
{
	if (!dev->agp || !dev->agp->acquired)
		return -EINVAL;

	dev->agp->mode = mode.mode;
	agp_enable(dev->agp->bridge, mode.mode);
	dev->agp->enabled = 1;
	return 0;
}
<<<<<<< HEAD
EXPORT_SYMBOL(drm_agp_enable);

int drm_agp_enable_ioctl(struct drm_device *dev, void *data,
			 struct drm_file *file_priv)
{
	struct drm_agp_mode *mode = data;

	return drm_agp_enable(dev, *mode);
}

/**
 * Allocate AGP memory.
 *
 * \param inode device inode.
 * \param file_priv file private pointer.
 * \param cmd command.
 * \param arg pointer to a drm_agp_buffer structure.
=======
EXPORT_SYMBOL(drm_legacy_agp_enable);

int drm_legacy_agp_enable_ioctl(struct drm_device *dev, void *data,
				struct drm_file *file_priv)
{
	struct drm_agp_mode *mode = data;

	return drm_legacy_agp_enable(dev, *mode);
}

/*
 * Allocate AGP memory.
 *
>>>>>>> upstream/android-13
 * \return zero on success or a negative number on failure.
 *
 * Verifies the AGP device is present and has been acquired, allocates the
 * memory via agp_allocate_memory() and creates a drm_agp_mem entry for it.
 */
<<<<<<< HEAD
int drm_agp_alloc(struct drm_device *dev, struct drm_agp_buffer *request)
=======
int drm_legacy_agp_alloc(struct drm_device *dev, struct drm_agp_buffer *request)
>>>>>>> upstream/android-13
{
	struct drm_agp_mem *entry;
	struct agp_memory *memory;
	unsigned long pages;
	u32 type;

	if (!dev->agp || !dev->agp->acquired)
		return -EINVAL;
	entry = kzalloc(sizeof(*entry), GFP_KERNEL);
	if (!entry)
		return -ENOMEM;

<<<<<<< HEAD
	pages = (request->size + PAGE_SIZE - 1) / PAGE_SIZE;
=======
	pages = DIV_ROUND_UP(request->size, PAGE_SIZE);
>>>>>>> upstream/android-13
	type = (u32) request->type;
	memory = agp_allocate_memory(dev->agp->bridge, pages, type);
	if (!memory) {
		kfree(entry);
		return -ENOMEM;
	}

	entry->handle = (unsigned long)memory->key + 1;
	entry->memory = memory;
	entry->bound = 0;
	entry->pages = pages;
	list_add(&entry->head, &dev->agp->memory);

	request->handle = entry->handle;
	request->physical = memory->physical;

	return 0;
}
<<<<<<< HEAD
EXPORT_SYMBOL(drm_agp_alloc);


int drm_agp_alloc_ioctl(struct drm_device *dev, void *data,
=======
EXPORT_SYMBOL(drm_legacy_agp_alloc);


int drm_legacy_agp_alloc_ioctl(struct drm_device *dev, void *data,
>>>>>>> upstream/android-13
			struct drm_file *file_priv)
{
	struct drm_agp_buffer *request = data;

<<<<<<< HEAD
	return drm_agp_alloc(dev, request);
}

/**
=======
	return drm_legacy_agp_alloc(dev, request);
}

/*
>>>>>>> upstream/android-13
 * Search for the AGP memory entry associated with a handle.
 *
 * \param dev DRM device structure.
 * \param handle AGP memory handle.
 * \return pointer to the drm_agp_mem structure associated with \p handle.
 *
 * Walks through drm_agp_head::memory until finding a matching handle.
 */
<<<<<<< HEAD
static struct drm_agp_mem *drm_agp_lookup_entry(struct drm_device *dev,
						unsigned long handle)
=======
static struct drm_agp_mem *drm_legacy_agp_lookup_entry(struct drm_device *dev,
						       unsigned long handle)
>>>>>>> upstream/android-13
{
	struct drm_agp_mem *entry;

	list_for_each_entry(entry, &dev->agp->memory, head) {
		if (entry->handle == handle)
			return entry;
	}
	return NULL;
}

<<<<<<< HEAD
/**
 * Unbind AGP memory from the GATT (ioctl).
 *
 * \param inode device inode.
 * \param file_priv DRM file private.
 * \param cmd command.
 * \param arg pointer to a drm_agp_binding structure.
=======
/*
 * Unbind AGP memory from the GATT (ioctl).
 *
>>>>>>> upstream/android-13
 * \return zero on success or a negative number on failure.
 *
 * Verifies the AGP device is present and acquired, looks-up the AGP memory
 * entry and passes it to the unbind_agp() function.
 */
<<<<<<< HEAD
int drm_agp_unbind(struct drm_device *dev, struct drm_agp_binding *request)
=======
int drm_legacy_agp_unbind(struct drm_device *dev, struct drm_agp_binding *request)
>>>>>>> upstream/android-13
{
	struct drm_agp_mem *entry;
	int ret;

	if (!dev->agp || !dev->agp->acquired)
		return -EINVAL;
<<<<<<< HEAD
	entry = drm_agp_lookup_entry(dev, request->handle);
	if (!entry || !entry->bound)
		return -EINVAL;
	ret = drm_unbind_agp(entry->memory);
=======
	entry = drm_legacy_agp_lookup_entry(dev, request->handle);
	if (!entry || !entry->bound)
		return -EINVAL;
	ret = agp_unbind_memory(entry->memory);
>>>>>>> upstream/android-13
	if (ret == 0)
		entry->bound = 0;
	return ret;
}
<<<<<<< HEAD
EXPORT_SYMBOL(drm_agp_unbind);


int drm_agp_unbind_ioctl(struct drm_device *dev, void *data,
			 struct drm_file *file_priv)
{
	struct drm_agp_binding *request = data;

	return drm_agp_unbind(dev, request);
}

/**
 * Bind AGP memory into the GATT (ioctl)
 *
 * \param inode device inode.
 * \param file_priv DRM file private.
 * \param cmd command.
 * \param arg pointer to a drm_agp_binding structure.
=======
EXPORT_SYMBOL(drm_legacy_agp_unbind);


int drm_legacy_agp_unbind_ioctl(struct drm_device *dev, void *data,
				struct drm_file *file_priv)
{
	struct drm_agp_binding *request = data;

	return drm_legacy_agp_unbind(dev, request);
}

/*
 * Bind AGP memory into the GATT (ioctl)
 *
>>>>>>> upstream/android-13
 * \return zero on success or a negative number on failure.
 *
 * Verifies the AGP device is present and has been acquired and that no memory
 * is currently bound into the GATT. Looks-up the AGP memory entry and passes
 * it to bind_agp() function.
 */
<<<<<<< HEAD
int drm_agp_bind(struct drm_device *dev, struct drm_agp_binding *request)
=======
int drm_legacy_agp_bind(struct drm_device *dev, struct drm_agp_binding *request)
>>>>>>> upstream/android-13
{
	struct drm_agp_mem *entry;
	int retcode;
	int page;

	if (!dev->agp || !dev->agp->acquired)
		return -EINVAL;
<<<<<<< HEAD
	entry = drm_agp_lookup_entry(dev, request->handle);
	if (!entry || entry->bound)
		return -EINVAL;
	page = (request->offset + PAGE_SIZE - 1) / PAGE_SIZE;
	retcode = drm_bind_agp(entry->memory, page);
=======
	entry = drm_legacy_agp_lookup_entry(dev, request->handle);
	if (!entry || entry->bound)
		return -EINVAL;
	page = DIV_ROUND_UP(request->offset, PAGE_SIZE);
	retcode = agp_bind_memory(entry->memory, page);
>>>>>>> upstream/android-13
	if (retcode)
		return retcode;
	entry->bound = dev->agp->base + (page << PAGE_SHIFT);
	DRM_DEBUG("base = 0x%lx entry->bound = 0x%lx\n",
		  dev->agp->base, entry->bound);
	return 0;
}
<<<<<<< HEAD
EXPORT_SYMBOL(drm_agp_bind);


int drm_agp_bind_ioctl(struct drm_device *dev, void *data,
		       struct drm_file *file_priv)
{
	struct drm_agp_binding *request = data;

	return drm_agp_bind(dev, request);
}

/**
 * Free AGP memory (ioctl).
 *
 * \param inode device inode.
 * \param file_priv DRM file private.
 * \param cmd command.
 * \param arg pointer to a drm_agp_buffer structure.
 * \return zero on success or a negative number on failure.
 *
 * Verifies the AGP device is present and has been acquired and looks up the
 * AGP memory entry. If the memory it's currently bound, unbind it via
 * unbind_agp(). Frees it via free_agp() as well as the entry itself
 * and unlinks from the doubly linked list it's inserted in.
 */
int drm_agp_free(struct drm_device *dev, struct drm_agp_buffer *request)
=======
EXPORT_SYMBOL(drm_legacy_agp_bind);


int drm_legacy_agp_bind_ioctl(struct drm_device *dev, void *data,
			      struct drm_file *file_priv)
{
	struct drm_agp_binding *request = data;

	return drm_legacy_agp_bind(dev, request);
}

/*
 * Free AGP memory (ioctl).
 *
 * \return zero on success or a negative number on failure.
 *
 * Verifies the AGP device is present and has been acquired and looks up the
 * AGP memory entry. If the memory is currently bound, unbind it via
 * unbind_agp(). Frees it via free_agp() as well as the entry itself
 * and unlinks from the doubly linked list it's inserted in.
 */
int drm_legacy_agp_free(struct drm_device *dev, struct drm_agp_buffer *request)
>>>>>>> upstream/android-13
{
	struct drm_agp_mem *entry;

	if (!dev->agp || !dev->agp->acquired)
		return -EINVAL;
<<<<<<< HEAD
	entry = drm_agp_lookup_entry(dev, request->handle);
	if (!entry)
		return -EINVAL;
	if (entry->bound)
		drm_unbind_agp(entry->memory);

	list_del(&entry->head);

	drm_free_agp(entry->memory, entry->pages);
	kfree(entry);
	return 0;
}
EXPORT_SYMBOL(drm_agp_free);


int drm_agp_free_ioctl(struct drm_device *dev, void *data,
		       struct drm_file *file_priv)
{
	struct drm_agp_buffer *request = data;

	return drm_agp_free(dev, request);
}

/**
=======
	entry = drm_legacy_agp_lookup_entry(dev, request->handle);
	if (!entry)
		return -EINVAL;
	if (entry->bound)
		agp_unbind_memory(entry->memory);

	list_del(&entry->head);

	agp_free_memory(entry->memory);
	kfree(entry);
	return 0;
}
EXPORT_SYMBOL(drm_legacy_agp_free);


int drm_legacy_agp_free_ioctl(struct drm_device *dev, void *data,
			      struct drm_file *file_priv)
{
	struct drm_agp_buffer *request = data;

	return drm_legacy_agp_free(dev, request);
}

/*
>>>>>>> upstream/android-13
 * Initialize the AGP resources.
 *
 * \return pointer to a drm_agp_head structure.
 *
 * Gets the drm_agp_t structure which is made available by the agpgart module
 * via the inter_module_* functions. Creates and initializes a drm_agp_head
 * structure.
 *
 * Note that final cleanup of the kmalloced structure is directly done in
 * drm_pci_agp_destroy.
 */
<<<<<<< HEAD
struct drm_agp_head *drm_agp_init(struct drm_device *dev)
{
=======
struct drm_agp_head *drm_legacy_agp_init(struct drm_device *dev)
{
	struct pci_dev *pdev = to_pci_dev(dev->dev);
>>>>>>> upstream/android-13
	struct drm_agp_head *head = NULL;

	head = kzalloc(sizeof(*head), GFP_KERNEL);
	if (!head)
		return NULL;
<<<<<<< HEAD
	head->bridge = agp_find_bridge(dev->pdev);
	if (!head->bridge) {
		head->bridge = agp_backend_acquire(dev->pdev);
=======
	head->bridge = agp_find_bridge(pdev);
	if (!head->bridge) {
		head->bridge = agp_backend_acquire(pdev);
>>>>>>> upstream/android-13
		if (!head->bridge) {
			kfree(head);
			return NULL;
		}
		agp_copy_info(head->bridge, &head->agp_info);
		agp_backend_release(head->bridge);
	} else {
		agp_copy_info(head->bridge, &head->agp_info);
	}
	if (head->agp_info.chipset == NOT_SUPPORTED) {
		kfree(head);
		return NULL;
	}
	INIT_LIST_HEAD(&head->memory);
	head->cant_use_aperture = head->agp_info.cant_use_aperture;
	head->page_mask = head->agp_info.page_mask;
	head->base = head->agp_info.aper_base;
	return head;
}
/* Only exported for i810.ko */
<<<<<<< HEAD
EXPORT_SYMBOL(drm_agp_init);
=======
EXPORT_SYMBOL(drm_legacy_agp_init);
>>>>>>> upstream/android-13

/**
 * drm_legacy_agp_clear - Clear AGP resource list
 * @dev: DRM device
 *
 * Iterate over all AGP resources and remove them. But keep the AGP head
 * intact so it can still be used. It is safe to call this if AGP is disabled or
 * was already removed.
 *
 * Cleanup is only done for drivers who have DRIVER_LEGACY set.
 */
void drm_legacy_agp_clear(struct drm_device *dev)
{
	struct drm_agp_mem *entry, *tempe;

	if (!dev->agp)
		return;
	if (!drm_core_check_feature(dev, DRIVER_LEGACY))
		return;

	list_for_each_entry_safe(entry, tempe, &dev->agp->memory, head) {
		if (entry->bound)
<<<<<<< HEAD
			drm_unbind_agp(entry->memory);
		drm_free_agp(entry->memory, entry->pages);
=======
			agp_unbind_memory(entry->memory);
		agp_free_memory(entry->memory);
>>>>>>> upstream/android-13
		kfree(entry);
	}
	INIT_LIST_HEAD(&dev->agp->memory);

	if (dev->agp->acquired)
<<<<<<< HEAD
		drm_agp_release(dev);
=======
		drm_legacy_agp_release(dev);
>>>>>>> upstream/android-13

	dev->agp->acquired = 0;
	dev->agp->enabled = 0;
}

<<<<<<< HEAD
/**
 * Binds a collection of pages into AGP memory at the given offset, returning
 * the AGP memory structure containing them.
 *
 * No reference is held on the pages during this time -- it is up to the
 * caller to handle that.
 */
struct agp_memory *
drm_agp_bind_pages(struct drm_device *dev,
		   struct page **pages,
		   unsigned long num_pages,
		   uint32_t gtt_offset,
		   u32 type)
{
	struct agp_memory *mem;
	int ret, i;

	DRM_DEBUG("\n");

	mem = agp_allocate_memory(dev->agp->bridge, num_pages,
				      type);
	if (mem == NULL) {
		DRM_ERROR("Failed to allocate memory for %ld pages\n",
			  num_pages);
		return NULL;
	}

	for (i = 0; i < num_pages; i++)
		mem->pages[i] = pages[i];
	mem->page_count = num_pages;

	mem->is_flushed = true;
	ret = agp_bind_memory(mem, gtt_offset / PAGE_SIZE);
	if (ret != 0) {
		DRM_ERROR("Failed to bind AGP memory: %d\n", ret);
		agp_free_memory(mem);
		return NULL;
	}

	return mem;
}
EXPORT_SYMBOL(drm_agp_bind_pages);
=======
#endif
>>>>>>> upstream/android-13
