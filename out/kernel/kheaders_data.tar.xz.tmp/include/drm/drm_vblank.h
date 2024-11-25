

#ifndef _DRM_VBLANK_H_
#define _DRM_VBLANK_H_

#include <linux/seqlock.h>
#include <linux/idr.h>
#include <linux/poll.h>

#include <drm/drm_file.h>
#include <drm/drm_modes.h>
#include <uapi/drm/drm.h>

struct drm_device;
struct drm_crtc;


struct drm_pending_vblank_event {
	
	struct drm_pending_event base;
	
	unsigned int pipe;
	
	u64 sequence;
	
	union {
		
		struct drm_event base;

		
		struct drm_event_vblank vbl;

		
		struct drm_event_crtc_sequence seq;
	} event;
};


struct drm_vblank_crtc {
	
	struct drm_device *dev;
	
	wait_queue_head_t queue;	
	
	struct timer_list disable_timer;

	
	seqlock_t seqlock;		

	
	u64 count;
	
	ktime_t time;

	
	atomic_t refcount;		
	
	u32 last;
	
	u32 max_vblank_count;
	
	unsigned int inmodeset;		
	
	unsigned int pipe;
	
	int framedur_ns;
	
	int linedur_ns;

	
	struct drm_display_mode hwmode;

	
	bool enabled;
};

int drm_vblank_init(struct drm_device *dev, unsigned int num_crtcs);
u64 drm_crtc_vblank_count(struct drm_crtc *crtc);
u64 drm_crtc_vblank_count_and_time(struct drm_crtc *crtc,
				   ktime_t *vblanktime);
void drm_crtc_send_vblank_event(struct drm_crtc *crtc,
			       struct drm_pending_vblank_event *e);
void drm_crtc_arm_vblank_event(struct drm_crtc *crtc,
			      struct drm_pending_vblank_event *e);
void drm_vblank_set_event(struct drm_pending_vblank_event *e,
			  u64 *seq,
			  ktime_t *now);
bool drm_handle_vblank(struct drm_device *dev, unsigned int pipe);
bool drm_crtc_handle_vblank(struct drm_crtc *crtc);
int drm_crtc_vblank_get(struct drm_crtc *crtc);
void drm_crtc_vblank_put(struct drm_crtc *crtc);
void drm_wait_one_vblank(struct drm_device *dev, unsigned int pipe);
void drm_crtc_wait_one_vblank(struct drm_crtc *crtc);
void drm_crtc_vblank_off(struct drm_crtc *crtc);
void drm_crtc_vblank_reset(struct drm_crtc *crtc);
void drm_crtc_vblank_on(struct drm_crtc *crtc);
u64 drm_crtc_accurate_vblank_count(struct drm_crtc *crtc);
void drm_vblank_restore(struct drm_device *dev, unsigned int pipe);
void drm_crtc_vblank_restore(struct drm_crtc *crtc);

bool drm_calc_vbltimestamp_from_scanoutpos(struct drm_device *dev,
					   unsigned int pipe, int *max_error,
					   ktime_t *vblank_time,
					   bool in_vblank_irq);
void drm_calc_timestamping_constants(struct drm_crtc *crtc,
				     const struct drm_display_mode *mode);
wait_queue_head_t *drm_crtc_vblank_waitqueue(struct drm_crtc *crtc);
void drm_crtc_set_max_vblank_count(struct drm_crtc *crtc,
				   u32 max_vblank_count);
#endif
