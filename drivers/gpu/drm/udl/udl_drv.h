<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> upstream/android-13
/*
 * Copyright (C) 2012 Red Hat
 *
 * based in parts on udlfb.c:
 * Copyright (C) 2009 Roberto De Ioris <roberto@unbit.it>
 * Copyright (C) 2009 Jaya Kumar <jayakumar.lkml@gmail.com>
 * Copyright (C) 2009 Bernie Thompson <bernie@plugable.com>
<<<<<<< HEAD
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License v2. See the file COPYING in the main directory of this archive for
 * more details.
=======
>>>>>>> upstream/android-13
 */

#ifndef UDL_DRV_H
#define UDL_DRV_H

<<<<<<< HEAD
#include <linux/usb.h>
#include <drm/drm_gem.h>
#include <linux/mm_types.h>
=======
#include <linux/mm_types.h>
#include <linux/usb.h>

#include <drm/drm_device.h>
#include <drm/drm_framebuffer.h>
#include <drm/drm_gem.h>
#include <drm/drm_simple_kms_helper.h>

struct drm_mode_create_dumb;
>>>>>>> upstream/android-13

#define DRIVER_NAME		"udl"
#define DRIVER_DESC		"DisplayLink"
#define DRIVER_DATE		"20120220"

#define DRIVER_MAJOR		0
#define DRIVER_MINOR		0
#define DRIVER_PATCHLEVEL	1

<<<<<<< HEAD
#define UDL_BO_CACHEABLE		(1 << 0)
#define UDL_BO_WC		(1 << 1)

=======
>>>>>>> upstream/android-13
struct udl_device;

struct urb_node {
	struct list_head entry;
	struct udl_device *dev;
	struct delayed_work release_urb_work;
	struct urb *urb;
};

struct urb_list {
	struct list_head list;
	spinlock_t lock;
	struct semaphore limit_sem;
	int available;
	int count;
	size_t size;
};

<<<<<<< HEAD
struct udl_fbdev;

struct udl_device {
	struct drm_device drm;
	struct device *dev;
	struct usb_device *udev;
	struct drm_crtc *crtc;
=======
struct udl_device {
	struct drm_device drm;
	struct device *dev;
	struct device *dmadev;

	struct drm_simple_display_pipe display_pipe;
>>>>>>> upstream/android-13

	struct mutex gem_lock;

	int sku_pixel_limit;

	struct urb_list urbs;
<<<<<<< HEAD
	atomic_t lost_pixels; /* 1 = a render op failed. Need screen refresh */

	struct udl_fbdev *fbdev;
	char mode_buf[1024];
	uint32_t mode_buf_len;
	atomic_t bytes_rendered; /* raw pixel-bytes driver asked to render */
	atomic_t bytes_identical; /* saved effort with backbuffer comparison */
	atomic_t bytes_sent; /* to usb, after compression including overhead */
	atomic_t cpu_kcycles_used; /* transpired during pixel processing */
=======

	char mode_buf[1024];
	uint32_t mode_buf_len;
>>>>>>> upstream/android-13
};

#define to_udl(x) container_of(x, struct udl_device, drm)

<<<<<<< HEAD
struct udl_gem_object {
	struct drm_gem_object base;
	struct page **pages;
	void *vmapping;
	struct sg_table *sg;
	unsigned int flags;
};

#define to_udl_bo(x) container_of(x, struct udl_gem_object, base)

struct udl_framebuffer {
	struct drm_framebuffer base;
	struct udl_gem_object *obj;
	bool active_16; /* active on the 16-bit channel */
};

#define to_udl_fb(x) container_of(x, struct udl_framebuffer, base)

/* modeset */
int udl_modeset_init(struct drm_device *dev);
void udl_modeset_restore(struct drm_device *dev);
void udl_modeset_cleanup(struct drm_device *dev);
int udl_connector_init(struct drm_device *dev, struct drm_encoder *encoder);

struct drm_encoder *udl_encoder_init(struct drm_device *dev);
=======
static inline struct usb_device *udl_to_usb_device(struct udl_device *udl)
{
	return interface_to_usbdev(to_usb_interface(udl->drm.dev));
}

/* modeset */
int udl_modeset_init(struct drm_device *dev);
struct drm_connector *udl_connector_init(struct drm_device *dev);
>>>>>>> upstream/android-13

struct urb *udl_get_urb(struct drm_device *dev);

int udl_submit_urb(struct drm_device *dev, struct urb *urb, size_t len);
void udl_urb_completion(struct urb *urb);

int udl_init(struct udl_device *udl);
<<<<<<< HEAD
void udl_fini(struct drm_device *dev);

int udl_fbdev_init(struct drm_device *dev);
void udl_fbdev_cleanup(struct drm_device *dev);
void udl_fbdev_unplug(struct drm_device *dev);
struct drm_framebuffer *
udl_fb_user_fb_create(struct drm_device *dev,
		      struct drm_file *file,
		      const struct drm_mode_fb_cmd2 *mode_cmd);

int udl_render_hline(struct drm_device *dev, int log_bpp, struct urb **urb_ptr,
		     const char *front, char **urb_buf_ptr,
		     u32 byte_offset, u32 device_byte_offset, u32 byte_width,
		     int *ident_ptr, int *sent_ptr);

int udl_dumb_create(struct drm_file *file_priv,
		    struct drm_device *dev,
		    struct drm_mode_create_dumb *args);
int udl_gem_mmap(struct drm_file *file_priv, struct drm_device *dev,
		 uint32_t handle, uint64_t *offset);

void udl_gem_free_object(struct drm_gem_object *gem_obj);
struct udl_gem_object *udl_gem_alloc_object(struct drm_device *dev,
					    size_t size);
struct dma_buf *udl_gem_prime_export(struct drm_device *dev,
				     struct drm_gem_object *obj, int flags);
struct drm_gem_object *udl_gem_prime_import(struct drm_device *dev,
				struct dma_buf *dma_buf);

int udl_gem_get_pages(struct udl_gem_object *obj);
void udl_gem_put_pages(struct udl_gem_object *obj);
int udl_gem_vmap(struct udl_gem_object *obj);
void udl_gem_vunmap(struct udl_gem_object *obj);
int udl_drm_gem_mmap(struct file *filp, struct vm_area_struct *vma);
vm_fault_t udl_gem_fault(struct vm_fault *vmf);

int udl_handle_damage(struct udl_framebuffer *fb, int x, int y,
		      int width, int height);
=======

int udl_render_hline(struct drm_device *dev, int log_bpp, struct urb **urb_ptr,
		     const char *front, char **urb_buf_ptr,
		     u32 byte_offset, u32 device_byte_offset, u32 byte_width);
>>>>>>> upstream/android-13

int udl_drop_usb(struct drm_device *dev);

#define CMD_WRITE_RAW8   "\xAF\x60" /**< 8 bit raw write command. */
#define CMD_WRITE_RL8    "\xAF\x61" /**< 8 bit run length command. */
#define CMD_WRITE_COPY8  "\xAF\x62" /**< 8 bit copy command. */
#define CMD_WRITE_RLX8   "\xAF\x63" /**< 8 bit extended run length command. */

#define CMD_WRITE_RAW16  "\xAF\x68" /**< 16 bit raw write command. */
#define CMD_WRITE_RL16   "\xAF\x69" /**< 16 bit run length command. */
#define CMD_WRITE_COPY16 "\xAF\x6A" /**< 16 bit copy command. */
#define CMD_WRITE_RLX16  "\xAF\x6B" /**< 16 bit extended run length command. */

<<<<<<< HEAD
=======
/* On/Off for driving the DisplayLink framebuffer to the display */
#define UDL_REG_BLANK_MODE		0x1f

#define UDL_BLANK_MODE_ON		0x00 /* hsync and vsync on, visible */
#define UDL_BLANK_MODE_BLANKED		0x01 /* hsync and vsync on, blanked */
#define UDL_BLANK_MODE_VSYNC_OFF	0x03 /* vsync off, blanked */
#define UDL_BLANK_MODE_HSYNC_OFF	0x05 /* hsync off, blanked */
#define UDL_BLANK_MODE_POWERDOWN	0x07 /* powered off; requires modeset */

>>>>>>> upstream/android-13
#endif
