/*
 * Copyright 1999 Precision Insight, Inc., Cedar Park, Texas.
 * Copyright 2000 VA Linux Systems, Inc., Sunnyvale, California.
 * Copyright (c) 2009-2010, Code Aurora Forum.
 * Copyright 2016 Intel Corp.
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

#ifndef _DRM_DRV_H_
#define _DRM_DRV_H_

#include <linux/list.h>
#include <linux/irqreturn.h>
<<<<<<< HEAD
#include <linux/uuid.h>
=======
>>>>>>> upstream/android-13

#include <drm/drm_device.h>

struct drm_file;
struct drm_gem_object;
struct drm_master;
struct drm_minor;
<<<<<<< HEAD
=======
struct dma_buf;
>>>>>>> upstream/android-13
struct dma_buf_attachment;
struct drm_display_mode;
struct drm_mode_create_dumb;
struct drm_printer;
<<<<<<< HEAD

/* driver capabilities and requirements mask */
#define DRIVER_USE_AGP			0x1
#define DRIVER_LEGACY			0x2
#define DRIVER_PCI_DMA			0x8
#define DRIVER_SG			0x10
#define DRIVER_HAVE_DMA			0x20
#define DRIVER_HAVE_IRQ			0x40
#define DRIVER_IRQ_SHARED		0x80
#define DRIVER_GEM			0x1000
#define DRIVER_MODESET			0x2000
#define DRIVER_PRIME			0x4000
#define DRIVER_RENDER			0x8000
#define DRIVER_ATOMIC			0x10000
#define DRIVER_KMS_LEGACY_CONTEXT	0x20000
#define DRIVER_SYNCOBJ                  0x40000
#define DRIVER_PREFER_XBGR_30BPP        0x80000
=======
struct sg_table;

/**
 * enum drm_driver_feature - feature flags
 *
 * See &drm_driver.driver_features, drm_device.driver_features and
 * drm_core_check_feature().
 */
enum drm_driver_feature {
	/**
	 * @DRIVER_GEM:
	 *
	 * Driver use the GEM memory manager. This should be set for all modern
	 * drivers.
	 */
	DRIVER_GEM			= BIT(0),
	/**
	 * @DRIVER_MODESET:
	 *
	 * Driver supports mode setting interfaces (KMS).
	 */
	DRIVER_MODESET			= BIT(1),
	/**
	 * @DRIVER_RENDER:
	 *
	 * Driver supports dedicated render nodes. See also the :ref:`section on
	 * render nodes <drm_render_node>` for details.
	 */
	DRIVER_RENDER			= BIT(3),
	/**
	 * @DRIVER_ATOMIC:
	 *
	 * Driver supports the full atomic modesetting userspace API. Drivers
	 * which only use atomic internally, but do not support the full
	 * userspace API (e.g. not all properties converted to atomic, or
	 * multi-plane updates are not guaranteed to be tear-free) should not
	 * set this flag.
	 */
	DRIVER_ATOMIC			= BIT(4),
	/**
	 * @DRIVER_SYNCOBJ:
	 *
	 * Driver supports &drm_syncobj for explicit synchronization of command
	 * submission.
	 */
	DRIVER_SYNCOBJ                  = BIT(5),
	/**
	 * @DRIVER_SYNCOBJ_TIMELINE:
	 *
	 * Driver supports the timeline flavor of &drm_syncobj for explicit
	 * synchronization of command submission.
	 */
	DRIVER_SYNCOBJ_TIMELINE         = BIT(6),

	/* IMPORTANT: Below are all the legacy flags, add new ones above. */

	/**
	 * @DRIVER_USE_AGP:
	 *
	 * Set up DRM AGP support, see drm_agp_init(), the DRM core will manage
	 * AGP resources. New drivers don't need this.
	 */
	DRIVER_USE_AGP			= BIT(25),
	/**
	 * @DRIVER_LEGACY:
	 *
	 * Denote a legacy driver using shadow attach. Do not use.
	 */
	DRIVER_LEGACY			= BIT(26),
	/**
	 * @DRIVER_PCI_DMA:
	 *
	 * Driver is capable of PCI DMA, mapping of PCI DMA buffers to userspace
	 * will be enabled. Only for legacy drivers. Do not use.
	 */
	DRIVER_PCI_DMA			= BIT(27),
	/**
	 * @DRIVER_SG:
	 *
	 * Driver can perform scatter/gather DMA, allocation and mapping of
	 * scatter/gather buffers will be enabled. Only for legacy drivers. Do
	 * not use.
	 */
	DRIVER_SG			= BIT(28),

	/**
	 * @DRIVER_HAVE_DMA:
	 *
	 * Driver supports DMA, the userspace DMA API will be supported. Only
	 * for legacy drivers. Do not use.
	 */
	DRIVER_HAVE_DMA			= BIT(29),
	/**
	 * @DRIVER_HAVE_IRQ:
	 *
	 * Legacy irq support. Only for legacy drivers. Do not use.
	 */
	DRIVER_HAVE_IRQ			= BIT(30),
	/**
	 * @DRIVER_KMS_LEGACY_CONTEXT:
	 *
	 * Used only by nouveau for backwards compatibility with existing
	 * userspace.  Do not use.
	 */
	DRIVER_KMS_LEGACY_CONTEXT	= BIT(31),
};
>>>>>>> upstream/android-13

/**
 * struct drm_driver - DRM driver structure
 *
<<<<<<< HEAD
 * This structure represent the common code for a family of cards. There will
 * one drm_device for each card present in this family. It contains lots of
 * vfunc entries, and a pile of those probably should be moved to more
=======
 * This structure represent the common code for a family of cards. There will be
 * one &struct drm_device for each card present in this family. It contains lots
 * of vfunc entries, and a pile of those probably should be moved to more
>>>>>>> upstream/android-13
 * appropriate places like &drm_mode_config_funcs or into a new operations
 * structure for GEM drivers.
 */
struct drm_driver {
	/**
	 * @load:
	 *
<<<<<<< HEAD
	 * Backward-compatible driver callback to complete
	 * initialization steps after the driver is registered.  For
	 * this reason, may suffer from race conditions and its use is
	 * deprecated for new drivers.  It is therefore only supported
	 * for existing drivers not yet converted to the new scheme.
	 * See drm_dev_init() and drm_dev_register() for proper and
	 * race-free way to set up a &struct drm_device.
=======
	 * Backward-compatible driver callback to complete initialization steps
	 * after the driver is registered.  For this reason, may suffer from
	 * race conditions and its use is deprecated for new drivers.  It is
	 * therefore only supported for existing drivers not yet converted to
	 * the new scheme.  See devm_drm_dev_alloc() and drm_dev_register() for
	 * proper and race-free way to set up a &struct drm_device.
>>>>>>> upstream/android-13
	 *
	 * This is deprecated, do not use!
	 *
	 * Returns:
	 *
	 * Zero on success, non-zero value on failure.
	 */
	int (*load) (struct drm_device *, unsigned long flags);

	/**
	 * @open:
	 *
	 * Driver callback when a new &struct drm_file is opened. Useful for
	 * setting up driver-private data structures like buffer allocators,
	 * execution contexts or similar things. Such driver-private resources
	 * must be released again in @postclose.
	 *
	 * Since the display/modeset side of DRM can only be owned by exactly
	 * one &struct drm_file (see &drm_file.is_master and &drm_device.master)
	 * there should never be a need to set up any modeset related resources
	 * in this callback. Doing so would be a driver design bug.
	 *
	 * Returns:
	 *
	 * 0 on success, a negative error code on failure, which will be
	 * promoted to userspace as the result of the open() system call.
	 */
	int (*open) (struct drm_device *, struct drm_file *);

	/**
	 * @postclose:
	 *
	 * One of the driver callbacks when a new &struct drm_file is closed.
	 * Useful for tearing down driver-private data structures allocated in
	 * @open like buffer allocators, execution contexts or similar things.
	 *
	 * Since the display/modeset side of DRM can only be owned by exactly
	 * one &struct drm_file (see &drm_file.is_master and &drm_device.master)
	 * there should never be a need to tear down any modeset related
	 * resources in this callback. Doing so would be a driver design bug.
	 */
	void (*postclose) (struct drm_device *, struct drm_file *);

	/**
	 * @lastclose:
	 *
	 * Called when the last &struct drm_file has been closed and there's
	 * currently no userspace client for the &struct drm_device.
	 *
	 * Modern drivers should only use this to force-restore the fbdev
	 * framebuffer using drm_fb_helper_restore_fbdev_mode_unlocked().
	 * Anything else would indicate there's something seriously wrong.
	 * Modern drivers can also use this to execute delayed power switching
	 * state changes, e.g. in conjunction with the :ref:`vga_switcheroo`
	 * infrastructure.
	 *
	 * This is called after @postclose hook has been called.
	 *
	 * NOTE:
	 *
	 * All legacy drivers use this callback to de-initialize the hardware.
	 * This is purely because of the shadow-attach model, where the DRM
	 * kernel driver does not really own the hardware. Instead ownershipe is
	 * handled with the help of userspace through an inheritedly racy dance
	 * to set/unset the VT into raw mode.
	 *
	 * Legacy drivers initialize the hardware in the @firstopen callback,
	 * which isn't even called for modern drivers.
	 */
	void (*lastclose) (struct drm_device *);

	/**
	 * @unload:
	 *
	 * Reverse the effects of the driver load callback.  Ideally,
	 * the clean up performed by the driver should happen in the
	 * reverse order of the initialization.  Similarly to the load
	 * hook, this handler is deprecated and its usage should be
	 * dropped in favor of an open-coded teardown function at the
	 * driver layer.  See drm_dev_unregister() and drm_dev_put()
	 * for the proper way to remove a &struct drm_device.
	 *
	 * The unload() hook is called right after unregistering
	 * the device.
	 *
	 */
	void (*unload) (struct drm_device *);

	/**
	 * @release:
	 *
	 * Optional callback for destroying device data after the final
<<<<<<< HEAD
	 * reference is released, i.e. the device is being destroyed. Drivers
	 * using this callback are responsible for calling drm_dev_fini()
	 * to finalize the device and then freeing the struct themselves.
=======
	 * reference is released, i.e. the device is being destroyed.
	 *
	 * This is deprecated, clean up all memory allocations associated with a
	 * &drm_device using drmm_add_action(), drmm_kmalloc() and related
	 * managed resources functions.
>>>>>>> upstream/android-13
	 */
	void (*release) (struct drm_device *);

	/**
<<<<<<< HEAD
	 * @get_vblank_counter:
	 *
	 * Driver callback for fetching a raw hardware vblank counter for the
	 * CRTC specified with the pipe argument.  If a device doesn't have a
	 * hardware counter, the driver can simply leave the hook as NULL.
	 * The DRM core will account for missed vblank events while interrupts
	 * where disabled based on system timestamps.
	 *
	 * Wraparound handling and loss of events due to modesetting is dealt
	 * with in the DRM core code, as long as drivers call
	 * drm_crtc_vblank_off() and drm_crtc_vblank_on() when disabling or
	 * enabling a CRTC.
	 *
	 * This is deprecated and should not be used by new drivers.
	 * Use &drm_crtc_funcs.get_vblank_counter instead.
	 *
	 * Returns:
	 *
	 * Raw vblank counter value.
	 */
	u32 (*get_vblank_counter) (struct drm_device *dev, unsigned int pipe);

	/**
	 * @enable_vblank:
	 *
	 * Enable vblank interrupts for the CRTC specified with the pipe
	 * argument.
	 *
	 * This is deprecated and should not be used by new drivers.
	 * Use &drm_crtc_funcs.enable_vblank instead.
	 *
	 * Returns:
	 *
	 * Zero on success, appropriate errno if the given @crtc's vblank
	 * interrupt cannot be enabled.
	 */
	int (*enable_vblank) (struct drm_device *dev, unsigned int pipe);

	/**
	 * @disable_vblank:
	 *
	 * Disable vblank interrupts for the CRTC specified with the pipe
	 * argument.
	 *
	 * This is deprecated and should not be used by new drivers.
	 * Use &drm_crtc_funcs.disable_vblank instead.
	 */
	void (*disable_vblank) (struct drm_device *dev, unsigned int pipe);

	/**
	 * @get_scanout_position:
	 *
	 * Called by vblank timestamping code.
	 *
	 * Returns the current display scanout position from a crtc, and an
	 * optional accurate ktime_get() timestamp of when position was
	 * measured. Note that this is a helper callback which is only used if a
	 * driver uses drm_calc_vbltimestamp_from_scanoutpos() for the
	 * @get_vblank_timestamp callback.
	 *
	 * Parameters:
	 *
	 * dev:
	 *     DRM device.
	 * pipe:
	 *     Id of the crtc to query.
	 * in_vblank_irq:
	 *     True when called from drm_crtc_handle_vblank().  Some drivers
	 *     need to apply some workarounds for gpu-specific vblank irq quirks
	 *     if flag is set.
	 * vpos:
	 *     Target location for current vertical scanout position.
	 * hpos:
	 *     Target location for current horizontal scanout position.
	 * stime:
	 *     Target location for timestamp taken immediately before
	 *     scanout position query. Can be NULL to skip timestamp.
	 * etime:
	 *     Target location for timestamp taken immediately after
	 *     scanout position query. Can be NULL to skip timestamp.
	 * mode:
	 *     Current display timings.
	 *
	 * Returns vpos as a positive number while in active scanout area.
	 * Returns vpos as a negative number inside vblank, counting the number
	 * of scanlines to go until end of vblank, e.g., -1 means "one scanline
	 * until start of active scanout / end of vblank."
	 *
	 * Returns:
	 *
	 * True on success, false if a reliable scanout position counter could
	 * not be read out.
	 *
	 * FIXME:
	 *
	 * Since this is a helper to implement @get_vblank_timestamp, we should
	 * move it to &struct drm_crtc_helper_funcs, like all the other
	 * helper-internal hooks.
	 */
	bool (*get_scanout_position) (struct drm_device *dev, unsigned int pipe,
				      bool in_vblank_irq, int *vpos, int *hpos,
				      ktime_t *stime, ktime_t *etime,
				      const struct drm_display_mode *mode);

	/**
	 * @get_vblank_timestamp:
	 *
	 * Called by drm_get_last_vbltimestamp(). Should return a precise
	 * timestamp when the most recent VBLANK interval ended or will end.
	 *
	 * Specifically, the timestamp in @vblank_time should correspond as
	 * closely as possible to the time when the first video scanline of
	 * the video frame after the end of VBLANK will start scanning out,
	 * the time immediately after end of the VBLANK interval. If the
	 * @crtc is currently inside VBLANK, this will be a time in the future.
	 * If the @crtc is currently scanning out a frame, this will be the
	 * past start time of the current scanout. This is meant to adhere
	 * to the OpenML OML_sync_control extension specification.
	 *
	 * Paramters:
	 *
	 * dev:
	 *     dev DRM device handle.
	 * pipe:
	 *     crtc for which timestamp should be returned.
	 * max_error:
	 *     Maximum allowable timestamp error in nanoseconds.
	 *     Implementation should strive to provide timestamp
	 *     with an error of at most max_error nanoseconds.
	 *     Returns true upper bound on error for timestamp.
	 * vblank_time:
	 *     Target location for returned vblank timestamp.
	 * in_vblank_irq:
	 *     True when called from drm_crtc_handle_vblank().  Some drivers
	 *     need to apply some workarounds for gpu-specific vblank irq quirks
	 *     if flag is set.
	 *
	 * Returns:
	 *
	 * True on success, false on failure, which means the core should
	 * fallback to a simple timestamp taken in drm_crtc_handle_vblank().
	 *
	 * FIXME:
	 *
	 * We should move this hook to &struct drm_crtc_funcs like all the other
	 * vblank hooks.
	 */
	bool (*get_vblank_timestamp) (struct drm_device *dev, unsigned int pipe,
				     int *max_error,
				     ktime_t *vblank_time,
				     bool in_vblank_irq);

	/**
	 * @irq_handler:
	 *
	 * Interrupt handler called when using drm_irq_install(). Not used by
	 * drivers which implement their own interrupt handling.
	 */
	irqreturn_t(*irq_handler) (int irq, void *arg);

	/**
	 * @irq_preinstall:
	 *
	 * Optional callback used by drm_irq_install() which is called before
	 * the interrupt handler is registered. This should be used to clear out
	 * any pending interrupts (from e.g. firmware based drives) and reset
	 * the interrupt handling registers.
	 */
	void (*irq_preinstall) (struct drm_device *dev);

	/**
	 * @irq_postinstall:
	 *
	 * Optional callback used by drm_irq_install() which is called after
	 * the interrupt handler is registered. This should be used to enable
	 * interrupt generation in the hardware.
	 */
	int (*irq_postinstall) (struct drm_device *dev);

	/**
	 * @irq_uninstall:
	 *
	 * Optional callback used by drm_irq_uninstall() which is called before
	 * the interrupt handler is unregistered. This should be used to disable
	 * interrupt generation in the hardware.
	 */
	void (*irq_uninstall) (struct drm_device *dev);

	/**
	 * @master_create:
	 *
	 * Called whenever a new master is created. Only used by vmwgfx.
	 */
	int (*master_create)(struct drm_device *dev, struct drm_master *master);

	/**
	 * @master_destroy:
	 *
	 * Called whenever a master is destroyed. Only used by vmwgfx.
	 */
	void (*master_destroy)(struct drm_device *dev, struct drm_master *master);

	/**
=======
>>>>>>> upstream/android-13
	 * @master_set:
	 *
	 * Called whenever the minor master is set. Only used by vmwgfx.
	 */
<<<<<<< HEAD
	int (*master_set)(struct drm_device *dev, struct drm_file *file_priv,
			  bool from_open);
=======
	void (*master_set)(struct drm_device *dev, struct drm_file *file_priv,
			   bool from_open);
>>>>>>> upstream/android-13
	/**
	 * @master_drop:
	 *
	 * Called whenever the minor master is dropped. Only used by vmwgfx.
	 */
	void (*master_drop)(struct drm_device *dev, struct drm_file *file_priv);

	/**
	 * @debugfs_init:
	 *
	 * Allows drivers to create driver-specific debugfs files.
	 */
<<<<<<< HEAD
	int (*debugfs_init)(struct drm_minor *minor);

	/**
	 * @gem_free_object: deconstructor for drm_gem_objects
	 *
	 * This is deprecated and should not be used by new drivers. Use
	 * @gem_free_object_unlocked instead.
	 */
	void (*gem_free_object) (struct drm_gem_object *obj);

	/**
	 * @gem_free_object_unlocked: deconstructor for drm_gem_objects
	 *
	 * This is for drivers which are not encumbered with &drm_device.struct_mutex
	 * legacy locking schemes. Use this hook instead of @gem_free_object.
	 */
	void (*gem_free_object_unlocked) (struct drm_gem_object *obj);

	/**
	 * @gem_open_object:
	 *
	 * Driver hook called upon gem handle creation
	 */
	int (*gem_open_object) (struct drm_gem_object *, struct drm_file *);

	/**
	 * @gem_close_object:
	 *
	 * Driver hook called upon gem handle release
	 */
	void (*gem_close_object) (struct drm_gem_object *, struct drm_file *);

	/**
	 * @gem_print_info:
	 *
	 * If driver subclasses struct &drm_gem_object, it can implement this
	 * optional hook for printing additional driver specific info.
	 *
	 * drm_printf_indent() should be used in the callback passing it the
	 * indent argument.
	 *
	 * This callback is called from drm_gem_print_info().
	 */
	void (*gem_print_info)(struct drm_printer *p, unsigned int indent,
			       const struct drm_gem_object *obj);
=======
	void (*debugfs_init)(struct drm_minor *minor);
>>>>>>> upstream/android-13

	/**
	 * @gem_create_object: constructor for gem objects
	 *
<<<<<<< HEAD
	 * Hook for allocating the GEM object struct, for use by core
	 * helpers.
=======
	 * Hook for allocating the GEM object struct, for use by the CMA and
	 * SHMEM GEM helpers.
>>>>>>> upstream/android-13
	 */
	struct drm_gem_object *(*gem_create_object)(struct drm_device *dev,
						    size_t size);

<<<<<<< HEAD
	/* prime: */
	/**
	 * @prime_handle_to_fd:
	 *
	 * export handle -> fd (see drm_gem_prime_handle_to_fd() helper)
=======
	/**
	 * @prime_handle_to_fd:
	 *
	 * Main PRIME export function. Should be implemented with
	 * drm_gem_prime_handle_to_fd() for GEM based drivers.
	 *
	 * For an in-depth discussion see :ref:`PRIME buffer sharing
	 * documentation <prime_buffer_sharing>`.
>>>>>>> upstream/android-13
	 */
	int (*prime_handle_to_fd)(struct drm_device *dev, struct drm_file *file_priv,
				uint32_t handle, uint32_t flags, int *prime_fd);
	/**
	 * @prime_fd_to_handle:
	 *
<<<<<<< HEAD
	 * import fd -> handle (see drm_gem_prime_fd_to_handle() helper)
	 */
	int (*prime_fd_to_handle)(struct drm_device *dev, struct drm_file *file_priv,
				int prime_fd, uint32_t *handle);
	/**
	 * @gem_prime_export:
	 *
	 * export GEM -> dmabuf
	 */
	struct dma_buf * (*gem_prime_export)(struct drm_device *dev,
				struct drm_gem_object *obj, int flags);
	/**
	 * @gem_prime_import:
	 *
	 * import dmabuf -> GEM
	 */
	struct drm_gem_object * (*gem_prime_import)(struct drm_device *dev,
				struct dma_buf *dma_buf);
	int (*gem_prime_pin)(struct drm_gem_object *obj);
	void (*gem_prime_unpin)(struct drm_gem_object *obj);
	struct reservation_object * (*gem_prime_res_obj)(
				struct drm_gem_object *obj);
	struct sg_table *(*gem_prime_get_sg_table)(struct drm_gem_object *obj);
=======
	 * Main PRIME import function. Should be implemented with
	 * drm_gem_prime_fd_to_handle() for GEM based drivers.
	 *
	 * For an in-depth discussion see :ref:`PRIME buffer sharing
	 * documentation <prime_buffer_sharing>`.
	 */
	int (*prime_fd_to_handle)(struct drm_device *dev, struct drm_file *file_priv,
				int prime_fd, uint32_t *handle);

	/**
	 * @gem_prime_import:
	 *
	 * Import hook for GEM drivers.
	 *
	 * This defaults to drm_gem_prime_import() if not set.
	 */
	struct drm_gem_object * (*gem_prime_import)(struct drm_device *dev,
				struct dma_buf *dma_buf);
	/**
	 * @gem_prime_import_sg_table:
	 *
	 * Optional hook used by the PRIME helper functions
	 * drm_gem_prime_import() respectively drm_gem_prime_import_dev().
	 */
>>>>>>> upstream/android-13
	struct drm_gem_object *(*gem_prime_import_sg_table)(
				struct drm_device *dev,
				struct dma_buf_attachment *attach,
				struct sg_table *sgt);
<<<<<<< HEAD
	void *(*gem_prime_vmap)(struct drm_gem_object *obj);
	void (*gem_prime_vunmap)(struct drm_gem_object *obj, void *vaddr);
=======
	/**
	 * @gem_prime_mmap:
	 *
	 * mmap hook for GEM drivers, used to implement dma-buf mmap in the
	 * PRIME helpers.
	 *
	 * FIXME: There's way too much duplication going on here, and also moved
	 * to &drm_gem_object_funcs.
	 */
>>>>>>> upstream/android-13
	int (*gem_prime_mmap)(struct drm_gem_object *obj,
				struct vm_area_struct *vma);

	/**
<<<<<<< HEAD
	 * @gem_prime_get_uuid
	 *
	 * get_uuid hook for GEM drivers. Retrieves the virtio uuid of the
	 * given GEM buffer.
	 */
	int (*gem_prime_get_uuid)(struct drm_gem_object *obj,
				  uuid_t *uuid);

	/**
=======
>>>>>>> upstream/android-13
	 * @dumb_create:
	 *
	 * This creates a new dumb buffer in the driver's backing storage manager (GEM,
	 * TTM or something else entirely) and returns the resulting buffer handle. This
	 * handle can then be wrapped up into a framebuffer modeset object.
	 *
	 * Note that userspace is not allowed to use such objects for render
	 * acceleration - drivers must create their own private ioctls for such a use
	 * case.
	 *
	 * Width, height and depth are specified in the &drm_mode_create_dumb
	 * argument. The callback needs to fill the handle, pitch and size for
	 * the created buffer.
	 *
	 * Called by the user via ioctl.
	 *
	 * Returns:
	 *
	 * Zero on success, negative errno on failure.
	 */
	int (*dumb_create)(struct drm_file *file_priv,
			   struct drm_device *dev,
			   struct drm_mode_create_dumb *args);
	/**
	 * @dumb_map_offset:
	 *
	 * Allocate an offset in the drm device node's address space to be able to
<<<<<<< HEAD
	 * memory map a dumb buffer. GEM-based drivers must use
	 * drm_gem_create_mmap_offset() to implement this.
=======
	 * memory map a dumb buffer.
	 *
	 * The default implementation is drm_gem_create_mmap_offset(). GEM based
	 * drivers must not overwrite this.
>>>>>>> upstream/android-13
	 *
	 * Called by the user via ioctl.
	 *
	 * Returns:
	 *
	 * Zero on success, negative errno on failure.
	 */
	int (*dumb_map_offset)(struct drm_file *file_priv,
			       struct drm_device *dev, uint32_t handle,
			       uint64_t *offset);
	/**
	 * @dumb_destroy:
	 *
	 * This destroys the userspace handle for the given dumb backing storage buffer.
	 * Since buffer objects must be reference counted in the kernel a buffer object
	 * won't be immediately freed if a framebuffer modeset object still uses it.
	 *
	 * Called by the user via ioctl.
	 *
<<<<<<< HEAD
=======
	 * The default implementation is drm_gem_dumb_destroy(). GEM based drivers
	 * must not overwrite this.
	 *
>>>>>>> upstream/android-13
	 * Returns:
	 *
	 * Zero on success, negative errno on failure.
	 */
	int (*dumb_destroy)(struct drm_file *file_priv,
			    struct drm_device *dev,
			    uint32_t handle);

<<<<<<< HEAD
	/**
	 * @gem_vm_ops: Driver private ops for this object
	 */
	const struct vm_operations_struct *gem_vm_ops;

=======
>>>>>>> upstream/android-13
	/** @major: driver major number */
	int major;
	/** @minor: driver minor number */
	int minor;
	/** @patchlevel: driver patch level */
	int patchlevel;
	/** @name: driver name */
	char *name;
	/** @desc: driver description */
	char *desc;
	/** @date: driver date */
	char *date;

<<<<<<< HEAD
	/** @driver_features: driver features */
=======
	/**
	 * @driver_features:
	 * Driver features, see &enum drm_driver_feature. Drivers can disable
	 * some features on a per-instance basis using
	 * &drm_device.driver_features.
	 */
>>>>>>> upstream/android-13
	u32 driver_features;

	/**
	 * @ioctls:
	 *
	 * Array of driver-private IOCTL description entries. See the chapter on
	 * :ref:`IOCTL support in the userland interfaces
	 * chapter<drm_driver_ioctl>` for the full details.
	 */

	const struct drm_ioctl_desc *ioctls;
	/** @num_ioctls: Number of entries in @ioctls. */
	int num_ioctls;

	/**
	 * @fops:
	 *
	 * File operations for the DRM device node. See the discussion in
	 * :ref:`file operations<drm_driver_fops>` for in-depth coverage and
	 * some examples.
	 */
	const struct file_operations *fops;

<<<<<<< HEAD
	/* Everything below here is for legacy driver, never use! */
	/* private: */

	/* List of devices hanging off this driver with stealth attach. */
	struct list_head legacy_dev_list;
=======
#ifdef CONFIG_DRM_LEGACY
	/* Everything below here is for legacy driver, never use! */
	/* private: */

>>>>>>> upstream/android-13
	int (*firstopen) (struct drm_device *);
	void (*preclose) (struct drm_device *, struct drm_file *file_priv);
	int (*dma_ioctl) (struct drm_device *dev, void *data, struct drm_file *file_priv);
	int (*dma_quiescent) (struct drm_device *);
	int (*context_dtor) (struct drm_device *dev, int context);
<<<<<<< HEAD
	int dev_priv_size;
};

extern unsigned int drm_debug;

int drm_dev_init(struct drm_device *dev,
		 struct drm_driver *driver,
		 struct device *parent);
void drm_dev_fini(struct drm_device *dev);

struct drm_device *drm_dev_alloc(struct drm_driver *driver,
=======
	irqreturn_t (*irq_handler)(int irq, void *arg);
	void (*irq_preinstall)(struct drm_device *dev);
	int (*irq_postinstall)(struct drm_device *dev);
	void (*irq_uninstall)(struct drm_device *dev);
	u32 (*get_vblank_counter)(struct drm_device *dev, unsigned int pipe);
	int (*enable_vblank)(struct drm_device *dev, unsigned int pipe);
	void (*disable_vblank)(struct drm_device *dev, unsigned int pipe);
	int dev_priv_size;
#endif
};

void *__devm_drm_dev_alloc(struct device *parent,
			   const struct drm_driver *driver,
			   size_t size, size_t offset);

/**
 * devm_drm_dev_alloc - Resource managed allocation of a &drm_device instance
 * @parent: Parent device object
 * @driver: DRM driver
 * @type: the type of the struct which contains struct &drm_device
 * @member: the name of the &drm_device within @type.
 *
 * This allocates and initialize a new DRM device. No device registration is done.
 * Call drm_dev_register() to advertice the device to user space and register it
 * with other core subsystems. This should be done last in the device
 * initialization sequence to make sure userspace can't access an inconsistent
 * state.
 *
 * The initial ref-count of the object is 1. Use drm_dev_get() and
 * drm_dev_put() to take and drop further ref-counts.
 *
 * It is recommended that drivers embed &struct drm_device into their own device
 * structure.
 *
 * Note that this manages the lifetime of the resulting &drm_device
 * automatically using devres. The DRM device initialized with this function is
 * automatically put on driver detach using drm_dev_put().
 *
 * RETURNS:
 * Pointer to new DRM device, or ERR_PTR on failure.
 */
#define devm_drm_dev_alloc(parent, driver, type, member) \
	((type *) __devm_drm_dev_alloc(parent, driver, sizeof(type), \
				       offsetof(type, member)))

struct drm_device *drm_dev_alloc(const struct drm_driver *driver,
>>>>>>> upstream/android-13
				 struct device *parent);
int drm_dev_register(struct drm_device *dev, unsigned long flags);
void drm_dev_unregister(struct drm_device *dev);

void drm_dev_get(struct drm_device *dev);
void drm_dev_put(struct drm_device *dev);
<<<<<<< HEAD
void drm_dev_unref(struct drm_device *dev);
=======
>>>>>>> upstream/android-13
void drm_put_dev(struct drm_device *dev);
bool drm_dev_enter(struct drm_device *dev, int *idx);
void drm_dev_exit(int idx);
void drm_dev_unplug(struct drm_device *dev);

/**
 * drm_dev_is_unplugged - is a DRM device unplugged
 * @dev: DRM device
 *
 * This function can be called to check whether a hotpluggable is unplugged.
 * Unplugging itself is singalled through drm_dev_unplug(). If a device is
 * unplugged, these two functions guarantee that any store before calling
 * drm_dev_unplug() is visible to callers of this function after it completes
<<<<<<< HEAD
=======
 *
 * WARNING: This function fundamentally races against drm_dev_unplug(). It is
 * recommended that drivers instead use the underlying drm_dev_enter() and
 * drm_dev_exit() function pairs.
>>>>>>> upstream/android-13
 */
static inline bool drm_dev_is_unplugged(struct drm_device *dev)
{
	int idx;

	if (drm_dev_enter(dev, &idx)) {
		drm_dev_exit(idx);
		return false;
	}

	return true;
}

/**
<<<<<<< HEAD
=======
 * drm_core_check_all_features - check driver feature flags mask
 * @dev: DRM device to check
 * @features: feature flag(s) mask
 *
 * This checks @dev for driver features, see &drm_driver.driver_features,
 * &drm_device.driver_features, and the various &enum drm_driver_feature flags.
 *
 * Returns true if all features in the @features mask are supported, false
 * otherwise.
 */
static inline bool drm_core_check_all_features(const struct drm_device *dev,
					       u32 features)
{
	u32 supported = dev->driver->driver_features & dev->driver_features;

	return features && (supported & features) == features;
}

/**
>>>>>>> upstream/android-13
 * drm_core_check_feature - check driver feature flags
 * @dev: DRM device to check
 * @feature: feature flag
 *
<<<<<<< HEAD
 * This checks @dev for driver features, see &drm_driver.driver_features and the
 * various DRIVER_\* flags.
 *
 * Returns true if the @feature is supported, false otherwise.
 */
static inline bool drm_core_check_feature(struct drm_device *dev, int feature)
{
	return dev->driver->driver_features & feature;
=======
 * This checks @dev for driver features, see &drm_driver.driver_features,
 * &drm_device.driver_features, and the various &enum drm_driver_feature flags.
 *
 * Returns true if the @feature is supported, false otherwise.
 */
static inline bool drm_core_check_feature(const struct drm_device *dev,
					  enum drm_driver_feature feature)
{
	return drm_core_check_all_features(dev, feature);
>>>>>>> upstream/android-13
}

/**
 * drm_drv_uses_atomic_modeset - check if the driver implements
 * atomic_commit()
 * @dev: DRM device
 *
 * This check is useful if drivers do not have DRIVER_ATOMIC set but
 * have atomic modesetting internally implemented.
 */
static inline bool drm_drv_uses_atomic_modeset(struct drm_device *dev)
{
	return drm_core_check_feature(dev, DRIVER_ATOMIC) ||
		(dev->mode_config.funcs && dev->mode_config.funcs->atomic_commit != NULL);
}


int drm_dev_set_unique(struct drm_device *dev, const char *name);


#endif
