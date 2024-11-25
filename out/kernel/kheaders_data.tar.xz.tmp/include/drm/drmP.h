

#ifndef _DRM_P_H_
#define _DRM_P_H_

#include <linux/agp_backend.h>
#include <linux/cdev.h>
#include <linux/dma-mapping.h>
#include <linux/file.h>
#include <linux/fs.h>
#include <linux/highmem.h>
#include <linux/idr.h>
#include <linux/init.h>
#include <linux/io.h>
#include <linux/jiffies.h>
#include <linux/kernel.h>
#include <linux/kref.h>
#include <linux/miscdevice.h>
#include <linux/mm.h>
#include <linux/mutex.h>
#include <linux/platform_device.h>
#include <linux/poll.h>
#include <linux/ratelimit.h>
#include <linux/sched.h>
#include <linux/slab.h>
#include <linux/types.h>
#include <linux/vmalloc.h>
#include <linux/workqueue.h>
#include <linux/dma-fence.h>
#include <linux/module.h>

#include <asm/mman.h>
#include <asm/pgalloc.h>
#include <linux/uaccess.h>

#include <uapi/drm/drm.h>
#include <uapi/drm/drm_mode.h>

#include <drm/drm_agpsupport.h>
#include <drm/drm_crtc.h>
#include <drm/drm_fourcc.h>
#include <drm/drm_global.h>
#include <drm/drm_hashtab.h>
#include <drm/drm_mm.h>
#include <drm/drm_os_linux.h>
#include <drm/drm_sarea.h>
#include <drm/drm_drv.h>
#include <drm/drm_prime.h>
#include <drm/drm_print.h>
#include <drm/drm_pci.h>
#include <drm/drm_file.h>
#include <drm/drm_debugfs.h>
#include <drm/drm_ioctl.h>
#include <drm/drm_sysfs.h>
#include <drm/drm_vblank.h>
#include <drm/drm_irq.h>
#include <drm/drm_device.h>

struct module;

struct device_node;
struct videomode;
struct reservation_object;
struct dma_buf_attachment;

struct pci_dev;
struct pci_controller;

#define DRM_IF_VERSION(maj, min) (maj << 16 | min)

#define DRM_SWITCH_POWER_ON 0
#define DRM_SWITCH_POWER_OFF 1
#define DRM_SWITCH_POWER_CHANGING 2
#define DRM_SWITCH_POWER_DYNAMIC_OFF 3


static inline bool drm_can_sleep(void)
{
	if (in_atomic() || in_dbg_master() || irqs_disabled())
		return false;
	return true;
}


#define for_each_if(condition) if (!(condition)) {} else

#endif
