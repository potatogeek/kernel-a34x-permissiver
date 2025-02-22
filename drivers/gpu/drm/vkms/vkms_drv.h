/* SPDX-License-Identifier: GPL-2.0+ */

#ifndef _VKMS_DRV_H_
#define _VKMS_DRV_H_

<<<<<<< HEAD
#include <drm/drmP.h>
#include <drm/drm.h>
#include <drm/drm_gem.h>
#include <drm/drm_encoder.h>
#include <linux/hrtimer.h>

#define XRES_MIN    32
#define YRES_MIN    32
=======
#include <linux/hrtimer.h>

#include <drm/drm.h>
#include <drm/drm_gem.h>
#include <drm/drm_gem_atomic_helper.h>
#include <drm/drm_encoder.h>
#include <drm/drm_writeback.h>

#define XRES_MIN    20
#define YRES_MIN    20
>>>>>>> upstream/android-13

#define XRES_DEF  1024
#define YRES_DEF   768

#define XRES_MAX  8192
#define YRES_MAX  8192

<<<<<<< HEAD
static const u32 vkms_formats[] = {
	DRM_FORMAT_XRGB8888,
=======
struct vkms_writeback_job {
	struct dma_buf_map map[DRM_FORMAT_MAX_PLANES];
	struct dma_buf_map data[DRM_FORMAT_MAX_PLANES];
};

struct vkms_composer {
	struct drm_framebuffer fb;
	struct drm_rect src, dst;
	struct dma_buf_map map[4];
	unsigned int offset;
	unsigned int pitch;
	unsigned int cpp;
};

/**
 * vkms_plane_state - Driver specific plane state
 * @base: base plane state
 * @composer: data required for composing computation
 */
struct vkms_plane_state {
	struct drm_shadow_plane_state base;
	struct vkms_composer *composer;
};

struct vkms_plane {
	struct drm_plane base;
};

/**
 * vkms_crtc_state - Driver specific CRTC state
 * @base: base CRTC state
 * @composer_work: work struct to compose and add CRC entries
 * @n_frame_start: start frame number for computed CRC
 * @n_frame_end: end frame number for computed CRC
 */
struct vkms_crtc_state {
	struct drm_crtc_state base;
	struct work_struct composer_work;

	int num_active_planes;
	/* stack of active planes for crc computation, should be in z order */
	struct vkms_plane_state **active_planes;
	struct vkms_writeback_job *active_writeback;

	/* below four are protected by vkms_output.composer_lock */
	bool crc_pending;
	bool wb_pending;
	u64 frame_start;
	u64 frame_end;
>>>>>>> upstream/android-13
};

struct vkms_output {
	struct drm_crtc crtc;
	struct drm_encoder encoder;
	struct drm_connector connector;
<<<<<<< HEAD
	struct hrtimer vblank_hrtimer;
	ktime_t period_ns;
	struct drm_pending_vblank_event *event;
=======
	struct drm_writeback_connector wb_connector;
	struct hrtimer vblank_hrtimer;
	ktime_t period_ns;
	struct drm_pending_vblank_event *event;
	/* ordered wq for composer_work */
	struct workqueue_struct *composer_workq;
	/* protects concurrent access to composer */
	spinlock_t lock;

	/* protected by @lock */
	bool composer_enabled;
	struct vkms_crtc_state *composer_state;

	spinlock_t composer_lock;
};

struct vkms_device;

struct vkms_config {
	bool writeback;
	bool cursor;
	bool overlay;
	/* only set when instantiated */
	struct vkms_device *dev;
>>>>>>> upstream/android-13
};

struct vkms_device {
	struct drm_device drm;
	struct platform_device *platform;
	struct vkms_output output;
<<<<<<< HEAD
};

struct vkms_gem_object {
	struct drm_gem_object gem;
	struct mutex pages_lock; /* Page lock used in page fault handler */
	struct page **pages;
=======
	const struct vkms_config *config;
>>>>>>> upstream/android-13
};

#define drm_crtc_to_vkms_output(target) \
	container_of(target, struct vkms_output, crtc)

#define drm_device_to_vkms_device(target) \
	container_of(target, struct vkms_device, drm)

<<<<<<< HEAD
=======
#define to_vkms_crtc_state(target)\
	container_of(target, struct vkms_crtc_state, base)

#define to_vkms_plane_state(target)\
	container_of(target, struct vkms_plane_state, base.base)

>>>>>>> upstream/android-13
/* CRTC */
int vkms_crtc_init(struct drm_device *dev, struct drm_crtc *crtc,
		   struct drm_plane *primary, struct drm_plane *cursor);

<<<<<<< HEAD
bool vkms_get_vblank_timestamp(struct drm_device *dev, unsigned int pipe,
			       int *max_error, ktime_t *vblank_time,
			       bool in_vblank_irq);

int vkms_output_init(struct vkms_device *vkmsdev);

struct drm_plane *vkms_plane_init(struct vkms_device *vkmsdev);

/* Gem stuff */
int vkms_gem_fault(struct vm_fault *vmf);

int vkms_dumb_create(struct drm_file *file, struct drm_device *dev,
		     struct drm_mode_create_dumb *args);

int vkms_dumb_map(struct drm_file *file, struct drm_device *dev,
		  u32 handle, u64 *offset);

void vkms_gem_free_object(struct drm_gem_object *obj);
=======
int vkms_output_init(struct vkms_device *vkmsdev, int index);

struct vkms_plane *vkms_plane_init(struct vkms_device *vkmsdev,
				   enum drm_plane_type type, int index);

/* CRC Support */
const char *const *vkms_get_crc_sources(struct drm_crtc *crtc,
					size_t *count);
int vkms_set_crc_source(struct drm_crtc *crtc, const char *src_name);
int vkms_verify_crc_source(struct drm_crtc *crtc, const char *source_name,
			   size_t *values_cnt);

/* Composer Support */
void vkms_composer_worker(struct work_struct *work);
void vkms_set_composer(struct vkms_output *out, bool enabled);

/* Writeback */
int vkms_enable_writeback_connector(struct vkms_device *vkmsdev);
>>>>>>> upstream/android-13

#endif /* _VKMS_DRV_H_ */
