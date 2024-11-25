/* SPDX-License-Identifier: GPL-2.0 */

#ifndef _DRM_CLIENT_H_
#define _DRM_CLIENT_H_

#include <linux/types.h>

struct drm_client_dev;
struct drm_device;
struct drm_file;
struct drm_framebuffer;
struct drm_gem_object;
struct drm_minor;
struct module;


struct drm_client_funcs {
	
	struct module *owner;

	
	void (*unregister)(struct drm_client_dev *client);

	
	int (*restore)(struct drm_client_dev *client);

	
	int (*hotplug)(struct drm_client_dev *client);
};


struct drm_client_dev {
	
	struct drm_device *dev;

	
	const char *name;

	
	struct list_head list;

	
	const struct drm_client_funcs *funcs;

	
	struct drm_file *file;
};

int drm_client_init(struct drm_device *dev, struct drm_client_dev *client,
		    const char *name, const struct drm_client_funcs *funcs);
void drm_client_release(struct drm_client_dev *client);
void drm_client_add(struct drm_client_dev *client);

void drm_client_dev_unregister(struct drm_device *dev);
void drm_client_dev_hotplug(struct drm_device *dev);
void drm_client_dev_restore(struct drm_device *dev);


struct drm_client_buffer {
	
	struct drm_client_dev *client;

	
	u32 handle;

	
	u32 pitch;

	
	struct drm_gem_object *gem;

	
	void *vaddr;

	
	struct drm_framebuffer *fb;
};

struct drm_client_buffer *
drm_client_framebuffer_create(struct drm_client_dev *client, u32 width, u32 height, u32 format);
void drm_client_framebuffer_delete(struct drm_client_buffer *buffer);

int drm_client_debugfs_init(struct drm_minor *minor);

#endif
