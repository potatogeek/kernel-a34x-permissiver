/*
 * omap_vout.c
 *
 * Copyright (C) 2005-2010 Texas Instruments.
 *
 * This file is licensed under the terms of the GNU General Public License
 * version 2. This program is licensed "as is" without any warranty of any
 * kind, whether express or implied.
 *
 * Leveraged code from the OMAP2 camera driver
 * Video-for-Linux (Version 2) camera capture driver for
 * the OMAP24xx camera controller.
 *
 * Author: Andy Lowe (source@mvista.com)
 *
 * Copyright (C) 2004 MontaVista Software, Inc.
 * Copyright (C) 2010 Texas Instruments.
 *
 * History:
 * 20-APR-2006 Khasim		Modified VRFB based Rotation,
 *				The image data is always read from 0 degree
 *				view and written
 *				to the virtual space of desired rotation angle
 * 4-DEC-2006  Jian		Changed to support better memory management
 *
 * 17-Nov-2008 Hardik		Changed driver to use video_ioctl2
 *
 * 23-Feb-2010 Vaibhav H	Modified to use new DSS2 interface
 *
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/vmalloc.h>
#include <linux/sched.h>
#include <linux/types.h>
#include <linux/platform_device.h>
#include <linux/irq.h>
#include <linux/videodev2.h>
#include <linux/dma-mapping.h>
#include <linux/slab.h>

<<<<<<< HEAD
#include <media/videobuf-dma-contig.h>
#include <media/v4l2-device.h>
#include <media/v4l2-ioctl.h>
=======
#include <media/v4l2-device.h>
#include <media/v4l2-ioctl.h>
#include <media/v4l2-event.h>
>>>>>>> upstream/android-13

#include <video/omapvrfb.h>
#include <video/omapfb_dss.h>

#include "omap_voutlib.h"
#include "omap_voutdef.h"
#include "omap_vout_vrfb.h"

MODULE_AUTHOR("Texas Instruments");
MODULE_DESCRIPTION("OMAP Video for Linux Video out driver");
MODULE_LICENSE("GPL");

/* Driver Configuration macros */
#define VOUT_NAME		"omap_vout"

enum omap_vout_channels {
	OMAP_VIDEO1,
	OMAP_VIDEO2,
};

<<<<<<< HEAD
static struct videobuf_queue_ops video_vbq_ops;
/* Variables configurable through module params*/
static u32 video1_numbuffers = 3;
static u32 video2_numbuffers = 3;
static u32 video1_bufsize = OMAP_VOUT_MAX_BUF_SIZE;
static u32 video2_bufsize = OMAP_VOUT_MAX_BUF_SIZE;
=======
/* Variables configurable through module params*/
>>>>>>> upstream/android-13
static bool vid1_static_vrfb_alloc;
static bool vid2_static_vrfb_alloc;
static bool debug;

/* Module parameters */
<<<<<<< HEAD
module_param(video1_numbuffers, uint, S_IRUGO);
MODULE_PARM_DESC(video1_numbuffers,
	"Number of buffers to be allocated at init time for Video1 device.");

module_param(video2_numbuffers, uint, S_IRUGO);
MODULE_PARM_DESC(video2_numbuffers,
	"Number of buffers to be allocated at init time for Video2 device.");

module_param(video1_bufsize, uint, S_IRUGO);
MODULE_PARM_DESC(video1_bufsize,
	"Size of the buffer to be allocated for video1 device");

module_param(video2_bufsize, uint, S_IRUGO);
MODULE_PARM_DESC(video2_bufsize,
	"Size of the buffer to be allocated for video2 device");

=======
>>>>>>> upstream/android-13
module_param(vid1_static_vrfb_alloc, bool, S_IRUGO);
MODULE_PARM_DESC(vid1_static_vrfb_alloc,
	"Static allocation of the VRFB buffer for video1 device");

module_param(vid2_static_vrfb_alloc, bool, S_IRUGO);
MODULE_PARM_DESC(vid2_static_vrfb_alloc,
	"Static allocation of the VRFB buffer for video2 device");

module_param(debug, bool, S_IRUGO);
MODULE_PARM_DESC(debug, "Debug level (0-1)");

/* list of image formats supported by OMAP2 video pipelines */
static const struct v4l2_fmtdesc omap_formats[] = {
	{
		/* Note:  V4L2 defines RGB565 as:
		 *
		 *      Byte 0                    Byte 1
		 *      g2 g1 g0 r4 r3 r2 r1 r0   b4 b3 b2 b1 b0 g5 g4 g3
		 *
		 * We interpret RGB565 as:
		 *
		 *      Byte 0                    Byte 1
		 *      g2 g1 g0 b4 b3 b2 b1 b0   r4 r3 r2 r1 r0 g5 g4 g3
		 */
<<<<<<< HEAD
		.description = "RGB565, le",
=======
>>>>>>> upstream/android-13
		.pixelformat = V4L2_PIX_FMT_RGB565,
	},
	{
		/* Note:  V4L2 defines RGB32 as: RGB-8-8-8-8  we use
		 *  this for RGB24 unpack mode, the last 8 bits are ignored
		 * */
<<<<<<< HEAD
		.description = "RGB32, le",
=======
>>>>>>> upstream/android-13
		.pixelformat = V4L2_PIX_FMT_RGB32,
	},
	{
		/* Note:  V4L2 defines RGB24 as: RGB-8-8-8  we use
		 *        this for RGB24 packed mode
		 *
		 */
<<<<<<< HEAD
		.description = "RGB24, le",
		.pixelformat = V4L2_PIX_FMT_RGB24,
	},
	{
		.description = "YUYV (YUV 4:2:2), packed",
		.pixelformat = V4L2_PIX_FMT_YUYV,
	},
	{
		.description = "UYVY, packed",
=======
		.pixelformat = V4L2_PIX_FMT_RGB24,
	},
	{
		.pixelformat = V4L2_PIX_FMT_YUYV,
	},
	{
>>>>>>> upstream/android-13
		.pixelformat = V4L2_PIX_FMT_UYVY,
	},
};

#define NUM_OUTPUT_FORMATS (ARRAY_SIZE(omap_formats))

/*
 * Try format
 */
static int omap_vout_try_format(struct v4l2_pix_format *pix)
{
	int ifmt, bpp = 0;

	pix->height = clamp(pix->height, (u32)VID_MIN_HEIGHT,
						(u32)VID_MAX_HEIGHT);
	pix->width = clamp(pix->width, (u32)VID_MIN_WIDTH, (u32)VID_MAX_WIDTH);

	for (ifmt = 0; ifmt < NUM_OUTPUT_FORMATS; ifmt++) {
		if (pix->pixelformat == omap_formats[ifmt].pixelformat)
			break;
	}

	if (ifmt == NUM_OUTPUT_FORMATS)
		ifmt = 0;

	pix->pixelformat = omap_formats[ifmt].pixelformat;
<<<<<<< HEAD
	pix->field = V4L2_FIELD_ANY;
=======
	pix->field = V4L2_FIELD_NONE;
>>>>>>> upstream/android-13

	switch (pix->pixelformat) {
	case V4L2_PIX_FMT_YUYV:
	case V4L2_PIX_FMT_UYVY:
	default:
<<<<<<< HEAD
		pix->colorspace = V4L2_COLORSPACE_JPEG;
=======
		pix->colorspace = V4L2_COLORSPACE_SRGB;
>>>>>>> upstream/android-13
		bpp = YUYV_BPP;
		break;
	case V4L2_PIX_FMT_RGB565:
	case V4L2_PIX_FMT_RGB565X:
		pix->colorspace = V4L2_COLORSPACE_SRGB;
		bpp = RGB565_BPP;
		break;
	case V4L2_PIX_FMT_RGB24:
		pix->colorspace = V4L2_COLORSPACE_SRGB;
		bpp = RGB24_BPP;
		break;
	case V4L2_PIX_FMT_RGB32:
	case V4L2_PIX_FMT_BGR32:
		pix->colorspace = V4L2_COLORSPACE_SRGB;
		bpp = RGB32_BPP;
		break;
	}
	pix->bytesperline = pix->width * bpp;
	pix->sizeimage = pix->bytesperline * pix->height;

	return bpp;
}

/*
<<<<<<< HEAD
 * omap_vout_get_userptr: Convert user space virtual address to physical
 * address.
 */
static int omap_vout_get_userptr(struct videobuf_buffer *vb, long virtp,
				 u32 *physp)
{
	struct frame_vector *vec;
	int ret;

	/* For kernel direct-mapped memory, take the easy way */
	if (virtp >= PAGE_OFFSET) {
		*physp = virt_to_phys((void *)virtp);
		return 0;
	}

	vec = frame_vector_create(1);
	if (!vec)
		return -ENOMEM;

	ret = get_vaddr_frames(virtp, 1, FOLL_WRITE, vec);
	if (ret != 1) {
		frame_vector_destroy(vec);
		return -EINVAL;
	}
	*physp = __pfn_to_phys(frame_vector_pfns(vec)[0]);
	vb->priv = vec;

	return 0;
}

/*
 * Free the V4L2 buffers
 */
void omap_vout_free_buffers(struct omap_vout_device *vout)
{
	int i, numbuffers;

	/* Allocate memory for the buffers */
	numbuffers = (vout->vid) ?  video2_numbuffers : video1_numbuffers;
	vout->buffer_size = (vout->vid) ? video2_bufsize : video1_bufsize;

	for (i = 0; i < numbuffers; i++) {
		omap_vout_free_buffer(vout->buf_virt_addr[i],
				vout->buffer_size);
		vout->buf_phy_addr[i] = 0;
		vout->buf_virt_addr[i] = 0;
	}
}

/*
=======
>>>>>>> upstream/android-13
 * Convert V4L2 rotation to DSS rotation
 *	V4L2 understand 0, 90, 180, 270.
 *	Convert to 0, 1, 2 and 3 respectively for DSS
 */
static int v4l2_rot_to_dss_rot(int v4l2_rotation,
			enum dss_rotation *rotation, bool mirror)
{
	int ret = 0;

	switch (v4l2_rotation) {
	case 90:
		*rotation = dss_rotation_90_degree;
		break;
	case 180:
		*rotation = dss_rotation_180_degree;
		break;
	case 270:
		*rotation = dss_rotation_270_degree;
		break;
	case 0:
		*rotation = dss_rotation_0_degree;
		break;
	default:
		ret = -EINVAL;
	}
	return ret;
}

static int omap_vout_calculate_offset(struct omap_vout_device *vout)
{
	struct omapvideo_info *ovid;
	struct v4l2_rect *crop = &vout->crop;
	struct v4l2_pix_format *pix = &vout->pix;
	int *cropped_offset = &vout->cropped_offset;
	int ps = 2, line_length = 0;

	ovid = &vout->vid_info;

	if (ovid->rotation_type == VOUT_ROT_VRFB) {
		omap_vout_calculate_vrfb_offset(vout);
	} else {
		vout->line_length = line_length = pix->width;

		if (V4L2_PIX_FMT_YUYV == pix->pixelformat ||
			V4L2_PIX_FMT_UYVY == pix->pixelformat)
			ps = 2;
		else if (V4L2_PIX_FMT_RGB32 == pix->pixelformat)
			ps = 4;
		else if (V4L2_PIX_FMT_RGB24 == pix->pixelformat)
			ps = 3;

		vout->ps = ps;

		*cropped_offset = (line_length * ps) *
			crop->top + crop->left * ps;
	}

	v4l2_dbg(1, debug, &vout->vid_dev->v4l2_dev, "%s Offset:%x\n",
			__func__, vout->cropped_offset);

	return 0;
}

/*
 * Convert V4L2 pixel format to DSS pixel format
 */
static int video_mode_to_dss_mode(struct omap_vout_device *vout)
{
	struct omap_overlay *ovl;
	struct omapvideo_info *ovid;
	struct v4l2_pix_format *pix = &vout->pix;
	enum omap_color_mode mode;

	ovid = &vout->vid_info;
	ovl = ovid->overlays[0];

	switch (pix->pixelformat) {
	case V4L2_PIX_FMT_YUYV:
		mode = OMAP_DSS_COLOR_YUV2;
		break;
	case V4L2_PIX_FMT_UYVY:
		mode = OMAP_DSS_COLOR_UYVY;
		break;
	case V4L2_PIX_FMT_RGB565:
		mode = OMAP_DSS_COLOR_RGB16;
		break;
	case V4L2_PIX_FMT_RGB24:
		mode = OMAP_DSS_COLOR_RGB24P;
		break;
	case V4L2_PIX_FMT_RGB32:
		mode = (ovl->id == OMAP_DSS_VIDEO1) ?
			OMAP_DSS_COLOR_RGB24U : OMAP_DSS_COLOR_ARGB32;
		break;
	case V4L2_PIX_FMT_BGR32:
		mode = OMAP_DSS_COLOR_RGBX32;
		break;
	default:
		mode = -EINVAL;
		break;
	}
	return mode;
}

/*
 * Setup the overlay
 */
static int omapvid_setup_overlay(struct omap_vout_device *vout,
		struct omap_overlay *ovl, int posx, int posy, int outw,
		int outh, u32 addr)
{
	int ret = 0;
	struct omap_overlay_info info;
	int cropheight, cropwidth, pixwidth;

	if ((ovl->caps & OMAP_DSS_OVL_CAP_SCALE) == 0 &&
			(outw != vout->pix.width || outh != vout->pix.height)) {
		ret = -EINVAL;
		goto setup_ovl_err;
	}

	vout->dss_mode = video_mode_to_dss_mode(vout);
	if (vout->dss_mode == -EINVAL) {
		ret = -EINVAL;
		goto setup_ovl_err;
	}

	/* Setup the input plane parameters according to
	 * rotation value selected.
	 */
	if (is_rotation_90_or_270(vout)) {
		cropheight = vout->crop.width;
		cropwidth = vout->crop.height;
		pixwidth = vout->pix.height;
	} else {
		cropheight = vout->crop.height;
		cropwidth = vout->crop.width;
		pixwidth = vout->pix.width;
	}

	ovl->get_overlay_info(ovl, &info);
	info.paddr = addr;
	info.width = cropwidth;
	info.height = cropheight;
	info.color_mode = vout->dss_mode;
	info.mirror = vout->mirror;
	info.pos_x = posx;
	info.pos_y = posy;
	info.out_width = outw;
	info.out_height = outh;
	info.global_alpha = vout->win.global_alpha;
	if (!is_rotation_enabled(vout)) {
		info.rotation = 0;
		info.rotation_type = OMAP_DSS_ROT_DMA;
		info.screen_width = pixwidth;
	} else {
		info.rotation = vout->rotation;
		info.rotation_type = OMAP_DSS_ROT_VRFB;
		info.screen_width = 2048;
	}

	v4l2_dbg(1, debug, &vout->vid_dev->v4l2_dev,
		"%s enable=%d addr=%pad width=%d\n height=%d color_mode=%d\n"
		"rotation=%d mirror=%d posx=%d posy=%d out_width = %d \n"
		"out_height=%d rotation_type=%d screen_width=%d\n", __func__,
		ovl->is_enabled(ovl), &info.paddr, info.width, info.height,
		info.color_mode, info.rotation, info.mirror, info.pos_x,
		info.pos_y, info.out_width, info.out_height, info.rotation_type,
		info.screen_width);

	ret = ovl->set_overlay_info(ovl, &info);
	if (ret)
		goto setup_ovl_err;

	return 0;

setup_ovl_err:
	v4l2_warn(&vout->vid_dev->v4l2_dev, "setup_overlay failed\n");
	return ret;
}

/*
 * Initialize the overlay structure
 */
static int omapvid_init(struct omap_vout_device *vout, u32 addr)
{
	int ret = 0, i;
	struct v4l2_window *win;
	struct omap_overlay *ovl;
	int posx, posy, outw, outh;
	struct omap_video_timings *timing;
	struct omapvideo_info *ovid = &vout->vid_info;

	win = &vout->win;
	for (i = 0; i < ovid->num_overlays; i++) {
		struct omap_dss_device *dssdev;

		ovl = ovid->overlays[i];
		dssdev = ovl->get_device(ovl);

		if (!dssdev)
			return -EINVAL;

		timing = &dssdev->panel.timings;

		outw = win->w.width;
		outh = win->w.height;
		switch (vout->rotation) {
		case dss_rotation_90_degree:
			/* Invert the height and width for 90
			 * and 270 degree rotation
			 */
			swap(outw, outh);
			posy = (timing->y_res - win->w.width) - win->w.left;
			posx = win->w.top;
			break;

		case dss_rotation_180_degree:
			posx = (timing->x_res - win->w.width) - win->w.left;
			posy = (timing->y_res - win->w.height) - win->w.top;
			break;

		case dss_rotation_270_degree:
			swap(outw, outh);
			posy = win->w.left;
			posx = (timing->x_res - win->w.height) - win->w.top;
			break;

		default:
			posx = win->w.left;
			posy = win->w.top;
			break;
		}

		ret = omapvid_setup_overlay(vout, ovl, posx, posy,
				outw, outh, addr);
		if (ret)
			goto omapvid_init_err;
	}
	return 0;

omapvid_init_err:
	v4l2_warn(&vout->vid_dev->v4l2_dev, "apply_changes failed\n");
	return ret;
}

/*
 * Apply the changes set the go bit of DSS
 */
static int omapvid_apply_changes(struct omap_vout_device *vout)
{
	int i;
	struct omap_overlay *ovl;
	struct omapvideo_info *ovid = &vout->vid_info;

	for (i = 0; i < ovid->num_overlays; i++) {
		struct omap_dss_device *dssdev;

		ovl = ovid->overlays[i];
		dssdev = ovl->get_device(ovl);
		if (!dssdev)
			return -EINVAL;
		ovl->manager->apply(ovl->manager);
	}

	return 0;
}

static int omapvid_handle_interlace_display(struct omap_vout_device *vout,
<<<<<<< HEAD
		unsigned int irqstatus, struct timeval timevalue)
=======
		unsigned int irqstatus, u64 ts)
>>>>>>> upstream/android-13
{
	u32 fid;

	if (vout->first_int) {
		vout->first_int = 0;
		goto err;
	}

	if (irqstatus & DISPC_IRQ_EVSYNC_ODD)
		fid = 1;
	else if (irqstatus & DISPC_IRQ_EVSYNC_EVEN)
		fid = 0;
	else
		goto err;

	vout->field_id ^= 1;
	if (fid != vout->field_id) {
		if (fid == 0)
			vout->field_id = fid;
	} else if (0 == fid) {
		if (vout->cur_frm == vout->next_frm)
			goto err;

<<<<<<< HEAD
		vout->cur_frm->ts = timevalue;
		vout->cur_frm->state = VIDEOBUF_DONE;
		wake_up_interruptible(&vout->cur_frm->done);
=======
		vout->cur_frm->vbuf.vb2_buf.timestamp = ts;
		vout->cur_frm->vbuf.sequence = vout->sequence++;
		vb2_buffer_done(&vout->cur_frm->vbuf.vb2_buf, VB2_BUF_STATE_DONE);
>>>>>>> upstream/android-13
		vout->cur_frm = vout->next_frm;
	} else {
		if (list_empty(&vout->dma_queue) ||
				(vout->cur_frm != vout->next_frm))
			goto err;
	}

	return vout->field_id;
err:
	return 0;
}

static void omap_vout_isr(void *arg, unsigned int irqstatus)
{
	int ret, fid, mgr_id;
	u32 addr, irq;
	struct omap_overlay *ovl;
<<<<<<< HEAD
	struct timeval timevalue;
=======
	u64 ts;
>>>>>>> upstream/android-13
	struct omapvideo_info *ovid;
	struct omap_dss_device *cur_display;
	struct omap_vout_device *vout = (struct omap_vout_device *)arg;

<<<<<<< HEAD
	if (!vout->streaming)
		return;

=======
>>>>>>> upstream/android-13
	ovid = &vout->vid_info;
	ovl = ovid->overlays[0];

	mgr_id = ovl->manager->id;

	/* get the display device attached to the overlay */
	cur_display = ovl->get_device(ovl);

	if (!cur_display)
		return;

	spin_lock(&vout->vbq_lock);
<<<<<<< HEAD
	v4l2_get_timestamp(&timevalue);
=======
	ts = ktime_get_ns();
>>>>>>> upstream/android-13

	switch (cur_display->type) {
	case OMAP_DISPLAY_TYPE_DSI:
	case OMAP_DISPLAY_TYPE_DPI:
	case OMAP_DISPLAY_TYPE_DVI:
		if (mgr_id == OMAP_DSS_CHANNEL_LCD)
			irq = DISPC_IRQ_VSYNC;
		else if (mgr_id == OMAP_DSS_CHANNEL_LCD2)
			irq = DISPC_IRQ_VSYNC2;
		else
			goto vout_isr_err;

		if (!(irqstatus & irq))
			goto vout_isr_err;
		break;
	case OMAP_DISPLAY_TYPE_VENC:
		fid = omapvid_handle_interlace_display(vout, irqstatus,
<<<<<<< HEAD
				timevalue);
=======
				ts);
>>>>>>> upstream/android-13
		if (!fid)
			goto vout_isr_err;
		break;
	case OMAP_DISPLAY_TYPE_HDMI:
		if (!(irqstatus & DISPC_IRQ_EVSYNC_EVEN))
			goto vout_isr_err;
		break;
	default:
		goto vout_isr_err;
	}

	if (!vout->first_int && (vout->cur_frm != vout->next_frm)) {
<<<<<<< HEAD
		vout->cur_frm->ts = timevalue;
		vout->cur_frm->state = VIDEOBUF_DONE;
		wake_up_interruptible(&vout->cur_frm->done);
=======
		vout->cur_frm->vbuf.vb2_buf.timestamp = ts;
		vout->cur_frm->vbuf.sequence = vout->sequence++;
		vb2_buffer_done(&vout->cur_frm->vbuf.vb2_buf, VB2_BUF_STATE_DONE);
>>>>>>> upstream/android-13
		vout->cur_frm = vout->next_frm;
	}

	vout->first_int = 0;
	if (list_empty(&vout->dma_queue))
		goto vout_isr_err;

	vout->next_frm = list_entry(vout->dma_queue.next,
<<<<<<< HEAD
			struct videobuf_buffer, queue);
	list_del(&vout->next_frm->queue);

	vout->next_frm->state = VIDEOBUF_ACTIVE;

	addr = (unsigned long) vout->queued_buf_addr[vout->next_frm->i]
=======
			struct omap_vout_buffer, queue);
	list_del(&vout->next_frm->queue);

	addr = (unsigned long)vout->queued_buf_addr[vout->next_frm->vbuf.vb2_buf.index]
>>>>>>> upstream/android-13
		+ vout->cropped_offset;

	/* First save the configuration in ovelray structure */
	ret = omapvid_init(vout, addr);
	if (ret) {
		printk(KERN_ERR VOUT_NAME
			"failed to set overlay info\n");
		goto vout_isr_err;
	}

	/* Enable the pipeline and set the Go bit */
	ret = omapvid_apply_changes(vout);
	if (ret)
		printk(KERN_ERR VOUT_NAME "failed to change mode\n");

vout_isr_err:
	spin_unlock(&vout->vbq_lock);
}

<<<<<<< HEAD
/* Video buffer call backs */

/*
 * Buffer setup function is called by videobuf layer when REQBUF ioctl is
 * called. This is used to setup buffers and return size and count of
 * buffers allocated. After the call to this buffer, videobuf layer will
 * setup buffer queue depending on the size and count of buffers
 */
static int omap_vout_buffer_setup(struct videobuf_queue *q, unsigned int *count,
			  unsigned int *size)
{
	int startindex = 0, i, j;
	u32 phy_addr = 0, virt_addr = 0;
	struct omap_vout_device *vout = q->priv_data;
	struct omapvideo_info *ovid = &vout->vid_info;
	int vid_max_buf_size;

	if (!vout)
		return -EINVAL;

	vid_max_buf_size = vout->vid == OMAP_VIDEO1 ? video1_bufsize :
		video2_bufsize;

	if (V4L2_BUF_TYPE_VIDEO_OUTPUT != q->type)
		return -EINVAL;

	startindex = (vout->vid == OMAP_VIDEO1) ?
		video1_numbuffers : video2_numbuffers;
	if (V4L2_MEMORY_MMAP == vout->memory && *count < startindex)
		*count = startindex;

	if (ovid->rotation_type == VOUT_ROT_VRFB) {
		if (omap_vout_vrfb_buffer_setup(vout, count, startindex))
			return -ENOMEM;
	}

	if (V4L2_MEMORY_MMAP != vout->memory)
		return 0;

	/* Now allocated the V4L2 buffers */
	*size = PAGE_ALIGN(vout->pix.width * vout->pix.height * vout->bpp);
	startindex = (vout->vid == OMAP_VIDEO1) ?
		video1_numbuffers : video2_numbuffers;

	/* Check the size of the buffer */
	if (*size > vid_max_buf_size) {
		v4l2_err(&vout->vid_dev->v4l2_dev,
				"buffer allocation mismatch [%u] [%u]\n",
				*size, vout->buffer_size);
		return -ENOMEM;
	}

	for (i = startindex; i < *count; i++) {
		vout->buffer_size = *size;

		virt_addr = omap_vout_alloc_buffer(vout->buffer_size,
				&phy_addr);
		if (!virt_addr) {
			if (ovid->rotation_type == VOUT_ROT_NONE)
				break;

			if (!is_rotation_enabled(vout))
				break;

			/* Free the VRFB buffers if no space for V4L2 buffers */
			for (j = i; j < *count; j++) {
				omap_vout_free_buffer(vout->smsshado_virt_addr[j],
						      vout->smsshado_size);
				vout->smsshado_virt_addr[j] = 0;
				vout->smsshado_phy_addr[j] = 0;
			}
		}
		vout->buf_virt_addr[i] = virt_addr;
		vout->buf_phy_addr[i] = phy_addr;
	}
	*count = vout->buffer_allocated = i;

	return 0;
}

/*
 * Free the V4L2 buffers additionally allocated than default
 * number of buffers
 */
static void omap_vout_free_extra_buffers(struct omap_vout_device *vout)
{
	int num_buffers = 0, i;

	num_buffers = (vout->vid == OMAP_VIDEO1) ?
		video1_numbuffers : video2_numbuffers;

	for (i = num_buffers; i < vout->buffer_allocated; i++) {
		if (vout->buf_virt_addr[i])
			omap_vout_free_buffer(vout->buf_virt_addr[i],
					vout->buffer_size);

		vout->buf_virt_addr[i] = 0;
		vout->buf_phy_addr[i] = 0;
	}
	vout->buffer_allocated = num_buffers;
}

/*
 * This function will be called when VIDIOC_QBUF ioctl is called.
 * It prepare buffers before give out for the display. This function
 * converts user space virtual address into physical address if userptr memory
 * exchange mechanism is used. If rotation is enabled, it copies entire
 * buffer into VRFB memory space before giving it to the DSS.
 */
static int omap_vout_buffer_prepare(struct videobuf_queue *q,
			struct videobuf_buffer *vb,
			enum v4l2_field field)
{
	struct omap_vout_device *vout = q->priv_data;
	struct omapvideo_info *ovid = &vout->vid_info;

	if (VIDEOBUF_NEEDS_INIT == vb->state) {
		vb->width = vout->pix.width;
		vb->height = vout->pix.height;
		vb->size = vb->width * vb->height * vout->bpp;
		vb->field = field;
	}
	vb->state = VIDEOBUF_PREPARED;
	/* if user pointer memory mechanism is used, get the physical
	 * address of the buffer
	 */
	if (V4L2_MEMORY_USERPTR == vb->memory) {
		int ret;

		if (0 == vb->baddr)
			return -EINVAL;
		/* Physical address */
		ret = omap_vout_get_userptr(vb, vb->baddr,
				(u32 *)&vout->queued_buf_addr[vb->i]);
		if (ret < 0)
			return ret;
	} else {
		unsigned long addr, dma_addr;
		unsigned long size;

		addr = (unsigned long) vout->buf_virt_addr[vb->i];
		size = (unsigned long) vb->size;

		dma_addr = dma_map_single(vout->vid_dev->v4l2_dev.dev, (void *) addr,
				size, DMA_TO_DEVICE);
		if (dma_mapping_error(vout->vid_dev->v4l2_dev.dev, dma_addr))
			v4l2_err(&vout->vid_dev->v4l2_dev,
				 "dma_map_single failed\n");

		vout->queued_buf_addr[vb->i] = (u8 *)vout->buf_phy_addr[vb->i];
	}

	if (ovid->rotation_type == VOUT_ROT_VRFB)
		return omap_vout_prepare_vrfb(vout, vb);
	else
		return 0;
}

/*
 * Buffer queue function will be called from the videobuf layer when _QBUF
 * ioctl is called. It is used to enqueue buffer, which is ready to be
 * displayed.
 */
static void omap_vout_buffer_queue(struct videobuf_queue *q,
			  struct videobuf_buffer *vb)
{
	struct omap_vout_device *vout = q->priv_data;

	/* Driver is also maintainig a queue. So enqueue buffer in the driver
	 * queue */
	list_add_tail(&vb->queue, &vout->dma_queue);

	vb->state = VIDEOBUF_QUEUED;
}

/*
 * Buffer release function is called from videobuf layer to release buffer
 * which are already allocated
 */
static void omap_vout_buffer_release(struct videobuf_queue *q,
			    struct videobuf_buffer *vb)
{
	vb->state = VIDEOBUF_NEEDS_INIT;
	if (vb->memory == V4L2_MEMORY_USERPTR && vb->priv) {
		struct frame_vector *vec = vb->priv;

		put_vaddr_frames(vec);
		frame_vector_destroy(vec);
	}
}

/*
 *  File operations
 */
static __poll_t omap_vout_poll(struct file *file,
				   struct poll_table_struct *wait)
{
	struct omap_vout_device *vout = file->private_data;
	struct videobuf_queue *q = &vout->vbq;

	return videobuf_poll_stream(file, q, wait);
}

static void omap_vout_vm_open(struct vm_area_struct *vma)
{
	struct omap_vout_device *vout = vma->vm_private_data;

	v4l2_dbg(1, debug, &vout->vid_dev->v4l2_dev,
		"vm_open [vma=%08lx-%08lx]\n", vma->vm_start, vma->vm_end);
	vout->mmap_count++;
}

static void omap_vout_vm_close(struct vm_area_struct *vma)
{
	struct omap_vout_device *vout = vma->vm_private_data;

	v4l2_dbg(1, debug, &vout->vid_dev->v4l2_dev,
		"vm_close [vma=%08lx-%08lx]\n", vma->vm_start, vma->vm_end);
	vout->mmap_count--;
}

static const struct vm_operations_struct omap_vout_vm_ops = {
	.open	= omap_vout_vm_open,
	.close	= omap_vout_vm_close,
};

static int omap_vout_mmap(struct file *file, struct vm_area_struct *vma)
{
	int i;
	void *pos;
	unsigned long start = vma->vm_start;
	unsigned long size = (vma->vm_end - vma->vm_start);
	struct omap_vout_device *vout = file->private_data;
	struct videobuf_queue *q = &vout->vbq;

	v4l2_dbg(1, debug, &vout->vid_dev->v4l2_dev,
			" %s pgoff=0x%lx, start=0x%lx, end=0x%lx\n", __func__,
			vma->vm_pgoff, vma->vm_start, vma->vm_end);

	/* look for the buffer to map */
	for (i = 0; i < VIDEO_MAX_FRAME; i++) {
		if (NULL == q->bufs[i])
			continue;
		if (V4L2_MEMORY_MMAP != q->bufs[i]->memory)
			continue;
		if (q->bufs[i]->boff == (vma->vm_pgoff << PAGE_SHIFT))
			break;
	}

	if (VIDEO_MAX_FRAME == i) {
		v4l2_dbg(1, debug, &vout->vid_dev->v4l2_dev,
				"offset invalid [offset=0x%lx]\n",
				(vma->vm_pgoff << PAGE_SHIFT));
		return -EINVAL;
	}
	/* Check the size of the buffer */
	if (size > vout->buffer_size) {
		v4l2_err(&vout->vid_dev->v4l2_dev,
				"insufficient memory [%lu] [%u]\n",
				size, vout->buffer_size);
		return -ENOMEM;
	}

	q->bufs[i]->baddr = vma->vm_start;

	vma->vm_flags |= VM_DONTEXPAND | VM_DONTDUMP;
	vma->vm_page_prot = pgprot_writecombine(vma->vm_page_prot);
	vma->vm_ops = &omap_vout_vm_ops;
	vma->vm_private_data = (void *) vout;
	pos = (void *)vout->buf_virt_addr[i];
	vma->vm_pgoff = virt_to_phys((void *)pos) >> PAGE_SHIFT;
	while (size > 0) {
		unsigned long pfn;
		pfn = virt_to_phys((void *) pos) >> PAGE_SHIFT;
		if (remap_pfn_range(vma, start, pfn, PAGE_SIZE, PAGE_SHARED))
			return -EAGAIN;
		start += PAGE_SIZE;
		pos += PAGE_SIZE;
		size -= PAGE_SIZE;
	}
	vout->mmap_count++;
	v4l2_dbg(1, debug, &vout->vid_dev->v4l2_dev, "Exiting %s\n", __func__);

	return 0;
}

static int omap_vout_release(struct file *file)
{
	unsigned int ret, i;
	struct videobuf_queue *q;
	struct omapvideo_info *ovid;
	struct omap_vout_device *vout = file->private_data;

	v4l2_dbg(1, debug, &vout->vid_dev->v4l2_dev, "Entering %s\n", __func__);
	ovid = &vout->vid_info;

	if (!vout)
		return 0;

	q = &vout->vbq;
	/* Disable all the overlay managers connected with this interface */
	for (i = 0; i < ovid->num_overlays; i++) {
		struct omap_overlay *ovl = ovid->overlays[i];
		struct omap_dss_device *dssdev = ovl->get_device(ovl);

		if (dssdev)
			ovl->disable(ovl);
	}
	/* Turn off the pipeline */
	ret = omapvid_apply_changes(vout);
	if (ret)
		v4l2_warn(&vout->vid_dev->v4l2_dev,
				"Unable to apply changes\n");

	/* Free all buffers */
	omap_vout_free_extra_buffers(vout);

	/* Free the VRFB buffers only if they are allocated
	 * during reqbufs.  Don't free if init time allocated
	 */
	if (ovid->rotation_type == VOUT_ROT_VRFB) {
		if (!vout->vrfb_static_allocation)
			omap_vout_free_vrfb_buffers(vout);
	}
	videobuf_mmap_free(q);

	/* Even if apply changes fails we should continue
	   freeing allocated memory */
	if (vout->streaming) {
		u32 mask = 0;

		mask = DISPC_IRQ_VSYNC | DISPC_IRQ_EVSYNC_EVEN |
			DISPC_IRQ_EVSYNC_ODD | DISPC_IRQ_VSYNC2;
		omap_dispc_unregister_isr(omap_vout_isr, vout, mask);
		vout->streaming = false;

		videobuf_streamoff(q);
		videobuf_queue_cancel(q);
	}

	if (vout->mmap_count != 0)
		vout->mmap_count = 0;

	vout->opened -= 1;
	file->private_data = NULL;

	if (vout->buffer_allocated)
		videobuf_mmap_free(q);

	v4l2_dbg(1, debug, &vout->vid_dev->v4l2_dev, "Exiting %s\n", __func__);
	return ret;
}

static int omap_vout_open(struct file *file)
{
	struct videobuf_queue *q;
	struct omap_vout_device *vout = NULL;

	vout = video_drvdata(file);

	if (vout == NULL)
		return -ENODEV;

	v4l2_dbg(1, debug, &vout->vid_dev->v4l2_dev, "Entering %s\n", __func__);

	/* for now, we only support single open */
	if (vout->opened)
		return -EBUSY;

	vout->opened += 1;

	file->private_data = vout;
	vout->type = V4L2_BUF_TYPE_VIDEO_OUTPUT;

	q = &vout->vbq;
	video_vbq_ops.buf_setup = omap_vout_buffer_setup;
	video_vbq_ops.buf_prepare = omap_vout_buffer_prepare;
	video_vbq_ops.buf_release = omap_vout_buffer_release;
	video_vbq_ops.buf_queue = omap_vout_buffer_queue;
	spin_lock_init(&vout->vbq_lock);

	videobuf_queue_dma_contig_init(q, &video_vbq_ops, q->dev,
			&vout->vbq_lock, vout->type, V4L2_FIELD_NONE,
			sizeof(struct videobuf_buffer), vout, NULL);

	v4l2_dbg(1, debug, &vout->vid_dev->v4l2_dev, "Exiting %s\n", __func__);
	return 0;
}
=======
>>>>>>> upstream/android-13

/*
 * V4L2 ioctls
 */
static int vidioc_querycap(struct file *file, void *fh,
		struct v4l2_capability *cap)
{
<<<<<<< HEAD
	struct omap_vout_device *vout = fh;

	strlcpy(cap->driver, VOUT_NAME, sizeof(cap->driver));
	strlcpy(cap->card, vout->vfd->name, sizeof(cap->card));
	cap->bus_info[0] = '\0';
	cap->device_caps = V4L2_CAP_STREAMING | V4L2_CAP_VIDEO_OUTPUT |
		V4L2_CAP_VIDEO_OUTPUT_OVERLAY;
	cap->capabilities = cap->device_caps | V4L2_CAP_DEVICE_CAPS;

=======
	struct omap_vout_device *vout = video_drvdata(file);

	strscpy(cap->driver, VOUT_NAME, sizeof(cap->driver));
	strscpy(cap->card, vout->vfd->name, sizeof(cap->card));
	snprintf(cap->bus_info, sizeof(cap->bus_info),
		 "platform:%s.%d", VOUT_NAME, vout->vid);
>>>>>>> upstream/android-13
	return 0;
}

static int vidioc_enum_fmt_vid_out(struct file *file, void *fh,
			struct v4l2_fmtdesc *fmt)
{
	int index = fmt->index;

	if (index >= NUM_OUTPUT_FORMATS)
		return -EINVAL;

	fmt->flags = omap_formats[index].flags;
<<<<<<< HEAD
	strlcpy(fmt->description, omap_formats[index].description,
			sizeof(fmt->description));
=======
>>>>>>> upstream/android-13
	fmt->pixelformat = omap_formats[index].pixelformat;

	return 0;
}

static int vidioc_g_fmt_vid_out(struct file *file, void *fh,
			struct v4l2_format *f)
{
<<<<<<< HEAD
	struct omap_vout_device *vout = fh;
=======
	struct omap_vout_device *vout = video_drvdata(file);
>>>>>>> upstream/android-13

	f->fmt.pix = vout->pix;
	return 0;

}

static int vidioc_try_fmt_vid_out(struct file *file, void *fh,
			struct v4l2_format *f)
{
	struct omap_overlay *ovl;
	struct omapvideo_info *ovid;
	struct omap_video_timings *timing;
<<<<<<< HEAD
	struct omap_vout_device *vout = fh;
=======
	struct omap_vout_device *vout = video_drvdata(file);
>>>>>>> upstream/android-13
	struct omap_dss_device *dssdev;

	ovid = &vout->vid_info;
	ovl = ovid->overlays[0];
	/* get the display device attached to the overlay */
	dssdev = ovl->get_device(ovl);

	if (!dssdev)
		return -EINVAL;

	timing = &dssdev->panel.timings;

	vout->fbuf.fmt.height = timing->y_res;
	vout->fbuf.fmt.width = timing->x_res;

	omap_vout_try_format(&f->fmt.pix);
	return 0;
}

static int vidioc_s_fmt_vid_out(struct file *file, void *fh,
			struct v4l2_format *f)
{
	int ret, bpp;
	struct omap_overlay *ovl;
	struct omapvideo_info *ovid;
	struct omap_video_timings *timing;
<<<<<<< HEAD
	struct omap_vout_device *vout = fh;
	struct omap_dss_device *dssdev;

	if (vout->streaming)
		return -EBUSY;

	mutex_lock(&vout->lock);

=======
	struct omap_vout_device *vout = video_drvdata(file);
	struct omap_dss_device *dssdev;

	if (vb2_is_busy(&vout->vq))
		return -EBUSY;

>>>>>>> upstream/android-13
	ovid = &vout->vid_info;
	ovl = ovid->overlays[0];
	dssdev = ovl->get_device(ovl);

	/* get the display device attached to the overlay */
	if (!dssdev) {
		ret = -EINVAL;
		goto s_fmt_vid_out_exit;
	}
	timing = &dssdev->panel.timings;

<<<<<<< HEAD
	/* We dont support RGB24-packed mode if vrfb rotation
=======
	/* We don't support RGB24-packed mode if vrfb rotation
>>>>>>> upstream/android-13
	 * is enabled*/
	if ((is_rotation_enabled(vout)) &&
			f->fmt.pix.pixelformat == V4L2_PIX_FMT_RGB24) {
		ret = -EINVAL;
		goto s_fmt_vid_out_exit;
	}

	/* get the framebuffer parameters */

	if (is_rotation_90_or_270(vout)) {
		vout->fbuf.fmt.height = timing->x_res;
		vout->fbuf.fmt.width = timing->y_res;
	} else {
		vout->fbuf.fmt.height = timing->y_res;
		vout->fbuf.fmt.width = timing->x_res;
	}

<<<<<<< HEAD
	/* change to samller size is OK */
=======
	/* change to smaller size is OK */
>>>>>>> upstream/android-13

	bpp = omap_vout_try_format(&f->fmt.pix);
	f->fmt.pix.sizeimage = f->fmt.pix.width * f->fmt.pix.height * bpp;

	/* try & set the new output format */
	vout->bpp = bpp;
	vout->pix = f->fmt.pix;
	vout->vrfb_bpp = 1;

	/* If YUYV then vrfb bpp is 2, for  others its 1 */
	if (V4L2_PIX_FMT_YUYV == vout->pix.pixelformat ||
			V4L2_PIX_FMT_UYVY == vout->pix.pixelformat)
		vout->vrfb_bpp = 2;

	/* set default crop and win */
	omap_vout_new_format(&vout->pix, &vout->fbuf, &vout->crop, &vout->win);

	ret = 0;

s_fmt_vid_out_exit:
<<<<<<< HEAD
	mutex_unlock(&vout->lock);
=======
>>>>>>> upstream/android-13
	return ret;
}

static int vidioc_try_fmt_vid_overlay(struct file *file, void *fh,
			struct v4l2_format *f)
{
	int ret = 0;
<<<<<<< HEAD
	struct omap_vout_device *vout = fh;
=======
	struct omap_vout_device *vout = video_drvdata(file);
>>>>>>> upstream/android-13
	struct omap_overlay *ovl;
	struct omapvideo_info *ovid;
	struct v4l2_window *win = &f->fmt.win;

	ovid = &vout->vid_info;
	ovl = ovid->overlays[0];

	ret = omap_vout_try_window(&vout->fbuf, win);

<<<<<<< HEAD
	if (!ret) {
		if ((ovl->caps & OMAP_DSS_OVL_CAP_GLOBAL_ALPHA) == 0)
			win->global_alpha = 255;
		else
			win->global_alpha = f->fmt.win.global_alpha;
	}
=======
	if (!ret && !(ovl->caps & OMAP_DSS_OVL_CAP_GLOBAL_ALPHA))
		win->global_alpha = 0;
>>>>>>> upstream/android-13

	return ret;
}

static int vidioc_s_fmt_vid_overlay(struct file *file, void *fh,
			struct v4l2_format *f)
{
	int ret = 0;
	struct omap_overlay *ovl;
	struct omapvideo_info *ovid;
<<<<<<< HEAD
	struct omap_vout_device *vout = fh;
	struct v4l2_window *win = &f->fmt.win;

	mutex_lock(&vout->lock);
=======
	struct omap_vout_device *vout = video_drvdata(file);
	struct v4l2_window *win = &f->fmt.win;

>>>>>>> upstream/android-13
	ovid = &vout->vid_info;
	ovl = ovid->overlays[0];

	ret = omap_vout_new_window(&vout->crop, &vout->win, &vout->fbuf, win);
	if (!ret) {
<<<<<<< HEAD
		/* Video1 plane does not support global alpha on OMAP3 */
		if ((ovl->caps & OMAP_DSS_OVL_CAP_GLOBAL_ALPHA) == 0)
			vout->win.global_alpha = 255;
		else
			vout->win.global_alpha = f->fmt.win.global_alpha;

		vout->win.chromakey = f->fmt.win.chromakey;
	}
	mutex_unlock(&vout->lock);
=======
		enum omap_dss_trans_key_type key_type =
			OMAP_DSS_COLOR_KEY_GFX_DST;
		int enable;

		/* Video1 plane does not support global alpha on OMAP3 */
		if (ovl->caps & OMAP_DSS_OVL_CAP_GLOBAL_ALPHA)
			vout->win.global_alpha = win->global_alpha;
		else
			win->global_alpha = 0;
		if (vout->fbuf.flags & (V4L2_FBUF_FLAG_CHROMAKEY |
					V4L2_FBUF_FLAG_SRC_CHROMAKEY))
			enable = 1;
		else
			enable = 0;
		if (vout->fbuf.flags & V4L2_FBUF_FLAG_SRC_CHROMAKEY)
			key_type = OMAP_DSS_COLOR_KEY_VID_SRC;

		if (ovl->manager && ovl->manager->get_manager_info &&
		    ovl->manager->set_manager_info) {
			struct omap_overlay_manager_info info;

			ovl->manager->get_manager_info(ovl->manager, &info);
			info.trans_enabled = enable;
			info.trans_key_type = key_type;
			info.trans_key = vout->win.chromakey;

			if (ovl->manager->set_manager_info(ovl->manager, &info))
				return -EINVAL;
		}
	}
>>>>>>> upstream/android-13
	return ret;
}

static int vidioc_g_fmt_vid_overlay(struct file *file, void *fh,
			struct v4l2_format *f)
{
<<<<<<< HEAD
	u32 key_value =  0;
	struct omap_overlay *ovl;
	struct omapvideo_info *ovid;
	struct omap_vout_device *vout = fh;
	struct omap_overlay_manager_info info;
=======
	struct omap_overlay *ovl;
	struct omapvideo_info *ovid;
	struct omap_vout_device *vout = video_drvdata(file);
>>>>>>> upstream/android-13
	struct v4l2_window *win = &f->fmt.win;

	ovid = &vout->vid_info;
	ovl = ovid->overlays[0];

	win->w = vout->win.w;
	win->field = vout->win.field;
<<<<<<< HEAD
	win->global_alpha = vout->win.global_alpha;

	if (ovl->manager && ovl->manager->get_manager_info) {
		ovl->manager->get_manager_info(ovl->manager, &info);
		key_value = info.trans_key;
	}
	win->chromakey = key_value;
=======
	win->chromakey = vout->win.chromakey;
	if (ovl->caps & OMAP_DSS_OVL_CAP_GLOBAL_ALPHA)
		win->global_alpha = vout->win.global_alpha;
	else
		win->global_alpha = 0;
	win->clips = NULL;
	win->clipcount = 0;
	win->bitmap = NULL;
>>>>>>> upstream/android-13
	return 0;
}

static int vidioc_g_selection(struct file *file, void *fh, struct v4l2_selection *sel)
{
<<<<<<< HEAD
	struct omap_vout_device *vout = fh;
=======
	struct omap_vout_device *vout = video_drvdata(file);
>>>>>>> upstream/android-13
	struct v4l2_pix_format *pix = &vout->pix;

	if (sel->type != V4L2_BUF_TYPE_VIDEO_OUTPUT)
		return -EINVAL;

	switch (sel->target) {
	case V4L2_SEL_TGT_CROP:
		sel->r = vout->crop;
		break;
	case V4L2_SEL_TGT_CROP_DEFAULT:
		omap_vout_default_crop(&vout->pix, &vout->fbuf, &sel->r);
		break;
	case V4L2_SEL_TGT_CROP_BOUNDS:
		/* Width and height are always even */
		sel->r.width = pix->width & ~1;
		sel->r.height = pix->height & ~1;
		break;
	default:
		return -EINVAL;
	}
	return 0;
}

static int vidioc_s_selection(struct file *file, void *fh, struct v4l2_selection *sel)
{
	int ret = -EINVAL;
<<<<<<< HEAD
	struct omap_vout_device *vout = fh;
=======
	struct omap_vout_device *vout = video_drvdata(file);
>>>>>>> upstream/android-13
	struct omapvideo_info *ovid;
	struct omap_overlay *ovl;
	struct omap_video_timings *timing;
	struct omap_dss_device *dssdev;

	if (sel->type != V4L2_BUF_TYPE_VIDEO_OUTPUT)
		return -EINVAL;

	if (sel->target != V4L2_SEL_TGT_CROP)
		return -EINVAL;

<<<<<<< HEAD
	if (vout->streaming)
		return -EBUSY;

	mutex_lock(&vout->lock);
=======
	if (vb2_is_busy(&vout->vq))
		return -EBUSY;

>>>>>>> upstream/android-13
	ovid = &vout->vid_info;
	ovl = ovid->overlays[0];
	/* get the display device attached to the overlay */
	dssdev = ovl->get_device(ovl);

	if (!dssdev) {
		ret = -EINVAL;
		goto s_crop_err;
	}

	timing = &dssdev->panel.timings;

	if (is_rotation_90_or_270(vout)) {
		vout->fbuf.fmt.height = timing->x_res;
		vout->fbuf.fmt.width = timing->y_res;
	} else {
		vout->fbuf.fmt.height = timing->y_res;
		vout->fbuf.fmt.width = timing->x_res;
	}

	ret = omap_vout_new_crop(&vout->pix, &vout->crop, &vout->win,
				 &vout->fbuf, &sel->r);

s_crop_err:
<<<<<<< HEAD
	mutex_unlock(&vout->lock);
=======
>>>>>>> upstream/android-13
	return ret;
}

static int omap_vout_s_ctrl(struct v4l2_ctrl *ctrl)
{
	struct omap_vout_device *vout =
		container_of(ctrl->handler, struct omap_vout_device, ctrl_handler);
	int ret = 0;

	switch (ctrl->id) {
	case V4L2_CID_ROTATE: {
		struct omapvideo_info *ovid;
		int rotation = ctrl->val;

		ovid = &vout->vid_info;

<<<<<<< HEAD
		mutex_lock(&vout->lock);
		if (rotation && ovid->rotation_type == VOUT_ROT_NONE) {
			mutex_unlock(&vout->lock);
=======
		if (rotation && ovid->rotation_type == VOUT_ROT_NONE) {
>>>>>>> upstream/android-13
			ret = -ERANGE;
			break;
		}

		if (rotation && vout->pix.pixelformat == V4L2_PIX_FMT_RGB24) {
<<<<<<< HEAD
			mutex_unlock(&vout->lock);
=======
>>>>>>> upstream/android-13
			ret = -EINVAL;
			break;
		}

		if (v4l2_rot_to_dss_rot(rotation, &vout->rotation,
							vout->mirror)) {
<<<<<<< HEAD
			mutex_unlock(&vout->lock);
			ret = -EINVAL;
			break;
		}
		mutex_unlock(&vout->lock);
=======
			ret = -EINVAL;
			break;
		}
>>>>>>> upstream/android-13
		break;
	}
	case V4L2_CID_BG_COLOR:
	{
		struct omap_overlay *ovl;
		unsigned int color = ctrl->val;
		struct omap_overlay_manager_info info;

		ovl = vout->vid_info.overlays[0];

<<<<<<< HEAD
		mutex_lock(&vout->lock);
		if (!ovl->manager || !ovl->manager->get_manager_info) {
			mutex_unlock(&vout->lock);
=======
		if (!ovl->manager || !ovl->manager->get_manager_info) {
>>>>>>> upstream/android-13
			ret = -EINVAL;
			break;
		}

		ovl->manager->get_manager_info(ovl->manager, &info);
		info.default_color = color;
		if (ovl->manager->set_manager_info(ovl->manager, &info)) {
<<<<<<< HEAD
			mutex_unlock(&vout->lock);
			ret = -EINVAL;
			break;
		}
		mutex_unlock(&vout->lock);
=======
			ret = -EINVAL;
			break;
		}
>>>>>>> upstream/android-13
		break;
	}
	case V4L2_CID_VFLIP:
	{
		struct omapvideo_info *ovid;
		unsigned int mirror = ctrl->val;

		ovid = &vout->vid_info;

<<<<<<< HEAD
		mutex_lock(&vout->lock);
		if (mirror && ovid->rotation_type == VOUT_ROT_NONE) {
			mutex_unlock(&vout->lock);
=======
		if (mirror && ovid->rotation_type == VOUT_ROT_NONE) {
>>>>>>> upstream/android-13
			ret = -ERANGE;
			break;
		}

		if (mirror  && vout->pix.pixelformat == V4L2_PIX_FMT_RGB24) {
<<<<<<< HEAD
			mutex_unlock(&vout->lock);
=======
>>>>>>> upstream/android-13
			ret = -EINVAL;
			break;
		}
		vout->mirror = mirror;
<<<<<<< HEAD
		mutex_unlock(&vout->lock);
=======
>>>>>>> upstream/android-13
		break;
	}
	default:
		return -EINVAL;
	}
	return ret;
}

static const struct v4l2_ctrl_ops omap_vout_ctrl_ops = {
	.s_ctrl = omap_vout_s_ctrl,
};

<<<<<<< HEAD
static int vidioc_reqbufs(struct file *file, void *fh,
			struct v4l2_requestbuffers *req)
{
	int ret = 0;
	unsigned int i, num_buffers = 0;
	struct omap_vout_device *vout = fh;
	struct videobuf_queue *q = &vout->vbq;

	if (req->type != V4L2_BUF_TYPE_VIDEO_OUTPUT)
		return -EINVAL;
	/* if memory is not mmp or userptr
	   return error */
	if ((V4L2_MEMORY_MMAP != req->memory) &&
			(V4L2_MEMORY_USERPTR != req->memory))
		return -EINVAL;

	mutex_lock(&vout->lock);
	/* Cannot be requested when streaming is on */
	if (vout->streaming) {
		ret = -EBUSY;
		goto reqbuf_err;
	}

	/* If buffers are already allocated free them */
	if (q->bufs[0] && (V4L2_MEMORY_MMAP == q->bufs[0]->memory)) {
		if (vout->mmap_count) {
			ret = -EBUSY;
			goto reqbuf_err;
		}
		num_buffers = (vout->vid == OMAP_VIDEO1) ?
			video1_numbuffers : video2_numbuffers;
		for (i = num_buffers; i < vout->buffer_allocated; i++) {
			omap_vout_free_buffer(vout->buf_virt_addr[i],
					vout->buffer_size);
			vout->buf_virt_addr[i] = 0;
			vout->buf_phy_addr[i] = 0;
		}
		vout->buffer_allocated = num_buffers;
		videobuf_mmap_free(q);
	} else if (q->bufs[0] && (V4L2_MEMORY_USERPTR == q->bufs[0]->memory)) {
		if (vout->buffer_allocated) {
			videobuf_mmap_free(q);
			for (i = 0; i < vout->buffer_allocated; i++) {
				kfree(q->bufs[i]);
				q->bufs[i] = NULL;
			}
			vout->buffer_allocated = 0;
		}
	}

	/*store the memory type in data structure */
	vout->memory = req->memory;

	INIT_LIST_HEAD(&vout->dma_queue);

	/* call videobuf_reqbufs api */
	ret = videobuf_reqbufs(q, req);
	if (ret < 0)
		goto reqbuf_err;

	vout->buffer_allocated = req->count;

reqbuf_err:
	mutex_unlock(&vout->lock);
	return ret;
}

static int vidioc_querybuf(struct file *file, void *fh,
			struct v4l2_buffer *b)
{
	struct omap_vout_device *vout = fh;

	return videobuf_querybuf(&vout->vbq, b);
}

static int vidioc_qbuf(struct file *file, void *fh,
			struct v4l2_buffer *buffer)
{
	struct omap_vout_device *vout = fh;
	struct videobuf_queue *q = &vout->vbq;

	if ((V4L2_BUF_TYPE_VIDEO_OUTPUT != buffer->type) ||
			(buffer->index >= vout->buffer_allocated) ||
			(q->bufs[buffer->index]->memory != buffer->memory)) {
		return -EINVAL;
	}
	if (V4L2_MEMORY_USERPTR == buffer->memory) {
		if ((buffer->length < vout->pix.sizeimage) ||
				(0 == buffer->m.userptr)) {
			return -EINVAL;
		}
	}

	if ((is_rotation_enabled(vout)) &&
			vout->vrfb_dma_tx.req_status == DMA_CHAN_NOT_ALLOTED) {
		v4l2_warn(&vout->vid_dev->v4l2_dev,
				"DMA Channel not allocated for Rotation\n");
		return -EINVAL;
	}

	return videobuf_qbuf(q, buffer);
}

static int vidioc_dqbuf(struct file *file, void *fh, struct v4l2_buffer *b)
{
	struct omap_vout_device *vout = fh;
	struct videobuf_queue *q = &vout->vbq;

	int ret;
	u32 addr;
	unsigned long size;
	struct videobuf_buffer *vb;

	if (!vout->streaming)
		return -EINVAL;

	ret = videobuf_dqbuf(q, b, !!(file->f_flags & O_NONBLOCK));
	if (ret)
		return ret;

	vb = q->bufs[b->index];

	addr = (unsigned long) vout->buf_phy_addr[vb->i];
	size = (unsigned long) vb->size;
	dma_unmap_single(vout->vid_dev->v4l2_dev.dev,  addr,
				size, DMA_TO_DEVICE);
	return 0;
}

static int vidioc_streamon(struct file *file, void *fh, enum v4l2_buf_type i)
{
	int ret = 0, j;
	u32 addr = 0, mask = 0;
	struct omap_vout_device *vout = fh;
	struct videobuf_queue *q = &vout->vbq;
	struct omapvideo_info *ovid = &vout->vid_info;

	mutex_lock(&vout->lock);

	if (vout->streaming) {
		ret = -EBUSY;
		goto streamon_err;
	}

	ret = videobuf_streamon(q);
	if (ret)
		goto streamon_err;

	if (list_empty(&vout->dma_queue)) {
		ret = -EIO;
		goto streamon_err1;
	}

	/* Get the next frame from the buffer queue */
	vout->next_frm = vout->cur_frm = list_entry(vout->dma_queue.next,
			struct videobuf_buffer, queue);
	/* Remove buffer from the buffer queue */
	list_del(&vout->cur_frm->queue);
	/* Mark state of the current frame to active */
	vout->cur_frm->state = VIDEOBUF_ACTIVE;
	/* Initialize field_id and started member */
	vout->field_id = 0;

	/* set flag here. Next QBUF will start DMA */
	vout->streaming = true;

	vout->first_int = 1;

	if (omap_vout_calculate_offset(vout)) {
		ret = -EINVAL;
		goto streamon_err1;
	}
	addr = (unsigned long) vout->queued_buf_addr[vout->cur_frm->i]
=======
static int omap_vout_vb2_queue_setup(struct vb2_queue *vq,
				     unsigned int *nbufs,
				     unsigned int *num_planes, unsigned int sizes[],
				     struct device *alloc_devs[])
{
	struct omap_vout_device *vout = vb2_get_drv_priv(vq);
	int size = vout->pix.sizeimage;

	if (is_rotation_enabled(vout) && vq->num_buffers + *nbufs > VRFB_NUM_BUFS) {
		*nbufs = VRFB_NUM_BUFS - vq->num_buffers;
		if (*nbufs == 0)
			return -EINVAL;
	}

	if (*num_planes)
		return sizes[0] < size ? -EINVAL : 0;

	*num_planes = 1;
	sizes[0] = size;
	return 0;
}

static int omap_vout_vb2_prepare(struct vb2_buffer *vb)
{
	struct omap_vout_device *vout = vb2_get_drv_priv(vb->vb2_queue);
	struct omapvideo_info *ovid = &vout->vid_info;
	struct omap_vout_buffer *voutbuf = vb2_to_omap_vout_buffer(vb);
	dma_addr_t buf_phy_addr = vb2_dma_contig_plane_dma_addr(vb, 0);

	if (vb2_plane_size(vb, 0) < vout->pix.sizeimage) {
		v4l2_dbg(1, debug, &vout->vid_dev->v4l2_dev,
			 "%s data will not fit into plane (%lu < %u)\n",
			__func__, vb2_plane_size(vb, 0), vout->pix.sizeimage);
		return -EINVAL;
	}

	vb2_set_plane_payload(vb, 0, vout->pix.sizeimage);
	voutbuf->vbuf.field = V4L2_FIELD_NONE;

	vout->queued_buf_addr[vb->index] = (u8 *)buf_phy_addr;
	if (ovid->rotation_type == VOUT_ROT_VRFB)
		return omap_vout_prepare_vrfb(vout, vb);
	return 0;
}

static void omap_vout_vb2_queue(struct vb2_buffer *vb)
{
	struct omap_vout_device *vout = vb2_get_drv_priv(vb->vb2_queue);
	struct omap_vout_buffer *voutbuf = vb2_to_omap_vout_buffer(vb);

	list_add_tail(&voutbuf->queue, &vout->dma_queue);
}

static int omap_vout_vb2_start_streaming(struct vb2_queue *vq, unsigned int count)
{
	struct omap_vout_device *vout = vb2_get_drv_priv(vq);
	struct omapvideo_info *ovid = &vout->vid_info;
	struct omap_vout_buffer *buf, *tmp;
	u32 addr = 0, mask = 0;
	int ret, j;

	/* Get the next frame from the buffer queue */
	vout->next_frm = vout->cur_frm = list_entry(vout->dma_queue.next,
			struct omap_vout_buffer, queue);
	/* Remove buffer from the buffer queue */
	list_del(&vout->cur_frm->queue);
	/* Initialize field_id and started member */
	vout->field_id = 0;
	vout->first_int = 1;
	vout->sequence = 0;

	if (omap_vout_calculate_offset(vout)) {
		ret = -EINVAL;
		goto out;
	}
	if (ovid->rotation_type == VOUT_ROT_VRFB)
		if (omap_vout_vrfb_buffer_setup(vout, &count, 0)) {
			ret = -ENOMEM;
			goto out;
		}

	addr = (unsigned long)vout->queued_buf_addr[vout->cur_frm->vbuf.vb2_buf.index]
>>>>>>> upstream/android-13
		+ vout->cropped_offset;

	mask = DISPC_IRQ_VSYNC | DISPC_IRQ_EVSYNC_EVEN | DISPC_IRQ_EVSYNC_ODD
		| DISPC_IRQ_VSYNC2;

<<<<<<< HEAD
	/* First save the configuration in ovelray structure */
=======
	/* First save the configuration in overlay structure */
>>>>>>> upstream/android-13
	ret = omapvid_init(vout, addr);
	if (ret) {
		v4l2_err(&vout->vid_dev->v4l2_dev,
				"failed to set overlay info\n");
		goto streamon_err1;
	}

	omap_dispc_register_isr(omap_vout_isr, vout, mask);

	/* Enable the pipeline and set the Go bit */
	ret = omapvid_apply_changes(vout);
	if (ret)
		v4l2_err(&vout->vid_dev->v4l2_dev, "failed to change mode\n");

	for (j = 0; j < ovid->num_overlays; j++) {
		struct omap_overlay *ovl = ovid->overlays[j];
		struct omap_dss_device *dssdev = ovl->get_device(ovl);

		if (dssdev) {
			ret = ovl->enable(ovl);
			if (ret)
				goto streamon_err1;
		}
	}
<<<<<<< HEAD

	ret = 0;

streamon_err1:
	if (ret)
		ret = videobuf_streamoff(q);
streamon_err:
	mutex_unlock(&vout->lock);
	return ret;
}

static int vidioc_streamoff(struct file *file, void *fh, enum v4l2_buf_type i)
{
	u32 mask = 0;
	int ret = 0, j;
	struct omap_vout_device *vout = fh;
	struct omapvideo_info *ovid = &vout->vid_info;

	if (!vout->streaming)
		return -EINVAL;

	vout->streaming = false;
=======
	return 0;

streamon_err1:
>>>>>>> upstream/android-13
	mask = DISPC_IRQ_VSYNC | DISPC_IRQ_EVSYNC_EVEN | DISPC_IRQ_EVSYNC_ODD
		| DISPC_IRQ_VSYNC2;

	omap_dispc_unregister_isr(omap_vout_isr, vout, mask);

	for (j = 0; j < ovid->num_overlays; j++) {
		struct omap_overlay *ovl = ovid->overlays[j];
		struct omap_dss_device *dssdev = ovl->get_device(ovl);

		if (dssdev)
			ovl->disable(ovl);
	}
<<<<<<< HEAD

	/* Turn of the pipeline */
	ret = omapvid_apply_changes(vout);
	if (ret)
		v4l2_err(&vout->vid_dev->v4l2_dev,
			 "failed to change mode in streamoff\n");

	INIT_LIST_HEAD(&vout->dma_queue);
	ret = videobuf_streamoff(&vout->vbq);

	return ret;
}

=======
	/* Turn of the pipeline */
	if (omapvid_apply_changes(vout))
		v4l2_err(&vout->vid_dev->v4l2_dev,
			 "failed to change mode in streamoff\n");

out:
	vb2_buffer_done(&vout->cur_frm->vbuf.vb2_buf, VB2_BUF_STATE_QUEUED);
	list_for_each_entry_safe(buf, tmp, &vout->dma_queue, queue) {
		list_del(&buf->queue);
		vb2_buffer_done(&buf->vbuf.vb2_buf, VB2_BUF_STATE_QUEUED);
	}
	return ret;
}

static void omap_vout_vb2_stop_streaming(struct vb2_queue *vq)
{
	struct omap_vout_device *vout = vb2_get_drv_priv(vq);
	struct omapvideo_info *ovid = &vout->vid_info;
	struct omap_vout_buffer *buf, *tmp;
	u32 mask = 0;
	int j;

	mask = DISPC_IRQ_VSYNC | DISPC_IRQ_EVSYNC_EVEN | DISPC_IRQ_EVSYNC_ODD
		| DISPC_IRQ_VSYNC2;

	omap_dispc_unregister_isr(omap_vout_isr, vout, mask);

	for (j = 0; j < ovid->num_overlays; j++) {
		struct omap_overlay *ovl = ovid->overlays[j];
		struct omap_dss_device *dssdev = ovl->get_device(ovl);

		if (dssdev)
			ovl->disable(ovl);
	}
	/* Turn of the pipeline */
	if (omapvid_apply_changes(vout))
		v4l2_err(&vout->vid_dev->v4l2_dev,
			 "failed to change mode in streamoff\n");

	if (vout->next_frm != vout->cur_frm)
		vb2_buffer_done(&vout->next_frm->vbuf.vb2_buf, VB2_BUF_STATE_ERROR);
	vb2_buffer_done(&vout->cur_frm->vbuf.vb2_buf, VB2_BUF_STATE_ERROR);
	list_for_each_entry_safe(buf, tmp, &vout->dma_queue, queue) {
		list_del(&buf->queue);
		vb2_buffer_done(&buf->vbuf.vb2_buf, VB2_BUF_STATE_ERROR);
	}
}

>>>>>>> upstream/android-13
static int vidioc_s_fbuf(struct file *file, void *fh,
				const struct v4l2_framebuffer *a)
{
	int enable = 0;
	struct omap_overlay *ovl;
	struct omapvideo_info *ovid;
<<<<<<< HEAD
	struct omap_vout_device *vout = fh;
=======
	struct omap_vout_device *vout = video_drvdata(file);
>>>>>>> upstream/android-13
	struct omap_overlay_manager_info info;
	enum omap_dss_trans_key_type key_type = OMAP_DSS_COLOR_KEY_GFX_DST;

	ovid = &vout->vid_info;
	ovl = ovid->overlays[0];

	/* OMAP DSS doesn't support Source and Destination color
	   key together */
	if ((a->flags & V4L2_FBUF_FLAG_SRC_CHROMAKEY) &&
			(a->flags & V4L2_FBUF_FLAG_CHROMAKEY))
		return -EINVAL;
	/* OMAP DSS Doesn't support the Destination color key
	   and alpha blending together */
	if ((a->flags & V4L2_FBUF_FLAG_CHROMAKEY) &&
			(a->flags & V4L2_FBUF_FLAG_LOCAL_ALPHA))
		return -EINVAL;

	if ((a->flags & V4L2_FBUF_FLAG_SRC_CHROMAKEY)) {
		vout->fbuf.flags |= V4L2_FBUF_FLAG_SRC_CHROMAKEY;
		key_type =  OMAP_DSS_COLOR_KEY_VID_SRC;
	} else
		vout->fbuf.flags &= ~V4L2_FBUF_FLAG_SRC_CHROMAKEY;

	if ((a->flags & V4L2_FBUF_FLAG_CHROMAKEY)) {
		vout->fbuf.flags |= V4L2_FBUF_FLAG_CHROMAKEY;
		key_type =  OMAP_DSS_COLOR_KEY_GFX_DST;
	} else
		vout->fbuf.flags &=  ~V4L2_FBUF_FLAG_CHROMAKEY;

	if (a->flags & (V4L2_FBUF_FLAG_CHROMAKEY |
				V4L2_FBUF_FLAG_SRC_CHROMAKEY))
		enable = 1;
	else
		enable = 0;
	if (ovl->manager && ovl->manager->get_manager_info &&
			ovl->manager->set_manager_info) {

		ovl->manager->get_manager_info(ovl->manager, &info);
		info.trans_enabled = enable;
		info.trans_key_type = key_type;
		info.trans_key = vout->win.chromakey;

		if (ovl->manager->set_manager_info(ovl->manager, &info))
			return -EINVAL;
	}
	if (a->flags & V4L2_FBUF_FLAG_LOCAL_ALPHA) {
		vout->fbuf.flags |= V4L2_FBUF_FLAG_LOCAL_ALPHA;
		enable = 1;
	} else {
		vout->fbuf.flags &= ~V4L2_FBUF_FLAG_LOCAL_ALPHA;
		enable = 0;
	}
	if (ovl->manager && ovl->manager->get_manager_info &&
			ovl->manager->set_manager_info) {
		ovl->manager->get_manager_info(ovl->manager, &info);
		/* enable this only if there is no zorder cap */
		if ((ovl->caps & OMAP_DSS_OVL_CAP_ZORDER) == 0)
			info.partial_alpha_enabled = enable;
		if (ovl->manager->set_manager_info(ovl->manager, &info))
			return -EINVAL;
	}

	return 0;
}

static int vidioc_g_fbuf(struct file *file, void *fh,
		struct v4l2_framebuffer *a)
{
	struct omap_overlay *ovl;
	struct omapvideo_info *ovid;
<<<<<<< HEAD
	struct omap_vout_device *vout = fh;
	struct omap_overlay_manager_info info;

	ovid = &vout->vid_info;
	ovl = ovid->overlays[0];

	/* The video overlay must stay within the framebuffer and can't be
	   positioned independently. */
	a->flags = V4L2_FBUF_FLAG_OVERLAY;
	a->capability = V4L2_FBUF_CAP_LOCAL_ALPHA | V4L2_FBUF_CAP_CHROMAKEY
		| V4L2_FBUF_CAP_SRC_CHROMAKEY;
=======
	struct omap_vout_device *vout = video_drvdata(file);
	struct omap_overlay_manager_info info;
	struct omap_video_timings *timing;
	struct omap_dss_device *dssdev;

	ovid = &vout->vid_info;
	ovl = ovid->overlays[0];
	/* get the display device attached to the overlay */
	dssdev = ovl->get_device(ovl);

	if (!dssdev)
		return -EINVAL;

	timing = &dssdev->panel.timings;

	vout->fbuf.fmt.height = timing->y_res;
	vout->fbuf.fmt.width = timing->x_res;
	a->fmt.field = V4L2_FIELD_NONE;
	a->fmt.colorspace = V4L2_COLORSPACE_SRGB;
	a->fmt.pixelformat = V4L2_PIX_FMT_RGBA32;
	a->fmt.height = vout->fbuf.fmt.height;
	a->fmt.width = vout->fbuf.fmt.width;
	a->fmt.bytesperline = vout->fbuf.fmt.width * 4;
	a->fmt.sizeimage = a->fmt.height * a->fmt.bytesperline;
	a->base = vout->fbuf.base;

	a->flags = vout->fbuf.flags;
	a->capability = vout->fbuf.capability;
	a->flags &= ~(V4L2_FBUF_FLAG_SRC_CHROMAKEY | V4L2_FBUF_FLAG_CHROMAKEY |
		      V4L2_FBUF_FLAG_LOCAL_ALPHA);
>>>>>>> upstream/android-13

	if (ovl->manager && ovl->manager->get_manager_info) {
		ovl->manager->get_manager_info(ovl->manager, &info);
		if (info.trans_key_type == OMAP_DSS_COLOR_KEY_VID_SRC)
			a->flags |= V4L2_FBUF_FLAG_SRC_CHROMAKEY;
		if (info.trans_key_type == OMAP_DSS_COLOR_KEY_GFX_DST)
			a->flags |= V4L2_FBUF_FLAG_CHROMAKEY;
<<<<<<< HEAD
	}
	if (ovl->manager && ovl->manager->get_manager_info) {
		ovl->manager->get_manager_info(ovl->manager, &info);
=======
>>>>>>> upstream/android-13
		if (info.partial_alpha_enabled)
			a->flags |= V4L2_FBUF_FLAG_LOCAL_ALPHA;
	}

	return 0;
}

<<<<<<< HEAD
=======
static int vidioc_enum_output(struct file *file, void *priv_fh,
			      struct v4l2_output *out)
{
	if (out->index)
		return -EINVAL;
	snprintf(out->name, sizeof(out->name), "Overlay");
	out->type = V4L2_OUTPUT_TYPE_ANALOGVGAOVERLAY;
	return 0;
}

static int vidioc_g_output(struct file *file, void *priv_fh, unsigned int *i)
{
	*i = 0;
	return 0;
}

static int vidioc_s_output(struct file *file, void *priv_fh, unsigned int i)
{
	return i ? -EINVAL : 0;
}

>>>>>>> upstream/android-13
static const struct v4l2_ioctl_ops vout_ioctl_ops = {
	.vidioc_querycap			= vidioc_querycap,
	.vidioc_enum_fmt_vid_out		= vidioc_enum_fmt_vid_out,
	.vidioc_g_fmt_vid_out			= vidioc_g_fmt_vid_out,
	.vidioc_try_fmt_vid_out			= vidioc_try_fmt_vid_out,
	.vidioc_s_fmt_vid_out			= vidioc_s_fmt_vid_out,
	.vidioc_s_fbuf				= vidioc_s_fbuf,
	.vidioc_g_fbuf				= vidioc_g_fbuf,
	.vidioc_try_fmt_vid_out_overlay		= vidioc_try_fmt_vid_overlay,
	.vidioc_s_fmt_vid_out_overlay		= vidioc_s_fmt_vid_overlay,
	.vidioc_g_fmt_vid_out_overlay		= vidioc_g_fmt_vid_overlay,
	.vidioc_g_selection			= vidioc_g_selection,
	.vidioc_s_selection			= vidioc_s_selection,
<<<<<<< HEAD
	.vidioc_reqbufs				= vidioc_reqbufs,
	.vidioc_querybuf			= vidioc_querybuf,
	.vidioc_qbuf				= vidioc_qbuf,
	.vidioc_dqbuf				= vidioc_dqbuf,
	.vidioc_streamon			= vidioc_streamon,
	.vidioc_streamoff			= vidioc_streamoff,
=======
	.vidioc_enum_output			= vidioc_enum_output,
	.vidioc_g_output			= vidioc_g_output,
	.vidioc_s_output			= vidioc_s_output,
	.vidioc_reqbufs				= vb2_ioctl_reqbufs,
	.vidioc_create_bufs			= vb2_ioctl_create_bufs,
	.vidioc_querybuf			= vb2_ioctl_querybuf,
	.vidioc_qbuf				= vb2_ioctl_qbuf,
	.vidioc_dqbuf				= vb2_ioctl_dqbuf,
	.vidioc_expbuf				= vb2_ioctl_expbuf,
	.vidioc_streamon			= vb2_ioctl_streamon,
	.vidioc_streamoff			= vb2_ioctl_streamoff,
	.vidioc_subscribe_event			= v4l2_ctrl_subscribe_event,
	.vidioc_unsubscribe_event		= v4l2_event_unsubscribe,
>>>>>>> upstream/android-13
};

static const struct v4l2_file_operations omap_vout_fops = {
	.owner		= THIS_MODULE,
<<<<<<< HEAD
	.poll		= omap_vout_poll,
	.unlocked_ioctl	= video_ioctl2,
	.mmap		= omap_vout_mmap,
	.open		= omap_vout_open,
	.release	= omap_vout_release,
=======
	.unlocked_ioctl	= video_ioctl2,
	.poll		= vb2_fop_poll,
	.mmap		= vb2_fop_mmap,
	.open		= v4l2_fh_open,
	.release	= vb2_fop_release,
};

static const struct vb2_ops omap_vout_vb2_ops = {
	.queue_setup		= omap_vout_vb2_queue_setup,
	.buf_queue		= omap_vout_vb2_queue,
	.buf_prepare		= omap_vout_vb2_prepare,
	.start_streaming	= omap_vout_vb2_start_streaming,
	.stop_streaming		= omap_vout_vb2_stop_streaming,
	.wait_prepare		= vb2_ops_wait_prepare,
	.wait_finish		= vb2_ops_wait_finish,
>>>>>>> upstream/android-13
};

/* Init functions used during driver initialization */
/* Initial setup of video_data */
static int __init omap_vout_setup_video_data(struct omap_vout_device *vout)
{
	struct video_device *vfd;
	struct v4l2_pix_format *pix;
	struct omap_overlay *ovl = vout->vid_info.overlays[0];
	struct omap_dss_device *display = ovl->get_device(ovl);
	struct v4l2_ctrl_handler *hdl;
<<<<<<< HEAD
=======
	struct vb2_queue *vq;
	int ret;
>>>>>>> upstream/android-13

	/* set the default pix */
	pix = &vout->pix;

	/* Set the default picture of QVGA  */
	pix->width = QQVGA_WIDTH;
	pix->height = QQVGA_HEIGHT;

	/* Default pixel format is RGB 5-6-5 */
	pix->pixelformat = V4L2_PIX_FMT_RGB565;
<<<<<<< HEAD
	pix->field = V4L2_FIELD_ANY;
	pix->bytesperline = pix->width * 2;
	pix->sizeimage = pix->bytesperline * pix->height;
	pix->colorspace = V4L2_COLORSPACE_JPEG;
=======
	pix->field = V4L2_FIELD_NONE;
	pix->bytesperline = pix->width * 2;
	pix->sizeimage = pix->bytesperline * pix->height;
	pix->colorspace = V4L2_COLORSPACE_SRGB;
>>>>>>> upstream/android-13

	vout->bpp = RGB565_BPP;
	vout->fbuf.fmt.width  =  display->panel.timings.x_res;
	vout->fbuf.fmt.height =  display->panel.timings.y_res;
<<<<<<< HEAD

	/* Set the data structures for the overlay parameters*/
	vout->win.global_alpha = 255;
	vout->fbuf.flags = 0;
	vout->fbuf.capability = V4L2_FBUF_CAP_LOCAL_ALPHA |
		V4L2_FBUF_CAP_SRC_CHROMAKEY | V4L2_FBUF_CAP_CHROMAKEY;
	vout->win.chromakey = 0;
=======
	vout->cropped_offset = 0;

	/* Set the data structures for the overlay parameters*/
	vout->fbuf.flags = V4L2_FBUF_FLAG_OVERLAY;
	vout->fbuf.capability = V4L2_FBUF_CAP_LOCAL_ALPHA |
		V4L2_FBUF_CAP_SRC_CHROMAKEY | V4L2_FBUF_CAP_CHROMAKEY |
		V4L2_FBUF_CAP_EXTERNOVERLAY;
	if (ovl->caps & OMAP_DSS_OVL_CAP_GLOBAL_ALPHA) {
		vout->win.global_alpha = 255;
		vout->fbuf.capability |= V4L2_FBUF_CAP_GLOBAL_ALPHA;
		vout->fbuf.flags |= V4L2_FBUF_FLAG_GLOBAL_ALPHA;
	} else {
		vout->win.global_alpha = 0;
	}
	vout->win.field = V4L2_FIELD_NONE;
>>>>>>> upstream/android-13

	omap_vout_new_format(pix, &vout->fbuf, &vout->crop, &vout->win);

	hdl = &vout->ctrl_handler;
	v4l2_ctrl_handler_init(hdl, 3);
<<<<<<< HEAD
	v4l2_ctrl_new_std(hdl, &omap_vout_ctrl_ops,
			  V4L2_CID_ROTATE, 0, 270, 90, 0);
	v4l2_ctrl_new_std(hdl, &omap_vout_ctrl_ops,
			  V4L2_CID_BG_COLOR, 0, 0xffffff, 1, 0);
	v4l2_ctrl_new_std(hdl, &omap_vout_ctrl_ops,
			  V4L2_CID_VFLIP, 0, 1, 1, 0);
=======
	if (vout->vid_info.rotation_type == VOUT_ROT_VRFB) {
		v4l2_ctrl_new_std(hdl, &omap_vout_ctrl_ops,
				  V4L2_CID_ROTATE, 0, 270, 90, 0);
		v4l2_ctrl_new_std(hdl, &omap_vout_ctrl_ops,
				  V4L2_CID_VFLIP, 0, 1, 1, 0);
	}
	v4l2_ctrl_new_std(hdl, &omap_vout_ctrl_ops,
			  V4L2_CID_BG_COLOR, 0, 0xffffff, 1, 0);
>>>>>>> upstream/android-13
	if (hdl->error)
		return hdl->error;

	vout->rotation = 0;
	vout->mirror = false;
<<<<<<< HEAD
=======
	INIT_LIST_HEAD(&vout->dma_queue);
>>>>>>> upstream/android-13
	if (vout->vid_info.rotation_type == VOUT_ROT_VRFB)
		vout->vrfb_bpp = 2;

	/* initialize the video_device struct */
	vfd = vout->vfd = video_device_alloc();

	if (!vfd) {
		printk(KERN_ERR VOUT_NAME
		       ": could not allocate video device struct\n");
		v4l2_ctrl_handler_free(hdl);
		return -ENOMEM;
	}
	vfd->ctrl_handler = hdl;
	vfd->release = video_device_release;
	vfd->ioctl_ops = &vout_ioctl_ops;

<<<<<<< HEAD
	strlcpy(vfd->name, VOUT_NAME, sizeof(vfd->name));
=======
	strscpy(vfd->name, VOUT_NAME, sizeof(vfd->name));
>>>>>>> upstream/android-13

	vfd->fops = &omap_vout_fops;
	vfd->v4l2_dev = &vout->vid_dev->v4l2_dev;
	vfd->vfl_dir = VFL_DIR_TX;
<<<<<<< HEAD
	mutex_init(&vout->lock);

	vfd->minor = -1;
	return 0;

=======
	vfd->minor = -1;
	vfd->device_caps = V4L2_CAP_STREAMING | V4L2_CAP_VIDEO_OUTPUT |
			   V4L2_CAP_VIDEO_OUTPUT_OVERLAY;
	mutex_init(&vout->lock);

	vq = &vout->vq;
	vq->type = V4L2_BUF_TYPE_VIDEO_OUTPUT;
	vq->io_modes = VB2_MMAP | VB2_DMABUF;
	vq->drv_priv = vout;
	vq->timestamp_flags = V4L2_BUF_FLAG_TIMESTAMP_MONOTONIC;
	vq->buf_struct_size = sizeof(struct omap_vout_buffer);
	vq->dev = vfd->v4l2_dev->dev;

	vq->ops = &omap_vout_vb2_ops;
	vq->mem_ops = &vb2_dma_contig_memops;
	vq->lock = &vout->lock;
	vq->min_buffers_needed = 1;
	vfd->queue = vq;

	ret = vb2_queue_init(vq);
	if (ret) {
		v4l2_ctrl_handler_free(hdl);
		video_device_release(vfd);
	}
	return ret;
>>>>>>> upstream/android-13
}

/* Setup video buffers */
static int __init omap_vout_setup_video_bufs(struct platform_device *pdev,
		int vid_num)
{
<<<<<<< HEAD
	u32 numbuffers;
	int ret = 0, i;
=======
>>>>>>> upstream/android-13
	struct omapvideo_info *ovid;
	struct omap_vout_device *vout;
	struct v4l2_device *v4l2_dev = platform_get_drvdata(pdev);
	struct omap2video_device *vid_dev =
		container_of(v4l2_dev, struct omap2video_device, v4l2_dev);
<<<<<<< HEAD
=======
	int ret = 0;
>>>>>>> upstream/android-13

	vout = vid_dev->vouts[vid_num];
	ovid = &vout->vid_info;

<<<<<<< HEAD
	numbuffers = (vid_num == 0) ? video1_numbuffers : video2_numbuffers;
	vout->buffer_size = (vid_num == 0) ? video1_bufsize : video2_bufsize;
	dev_info(&pdev->dev, "Buffer Size = %d\n", vout->buffer_size);

	for (i = 0; i < numbuffers; i++) {
		vout->buf_virt_addr[i] =
			omap_vout_alloc_buffer(vout->buffer_size,
					(u32 *) &vout->buf_phy_addr[i]);
		if (!vout->buf_virt_addr[i]) {
			numbuffers = i;
			ret = -ENOMEM;
			goto free_buffers;
		}
	}

	vout->cropped_offset = 0;

=======
>>>>>>> upstream/android-13
	if (ovid->rotation_type == VOUT_ROT_VRFB) {
		bool static_vrfb_allocation = (vid_num == 0) ?
			vid1_static_vrfb_alloc : vid2_static_vrfb_alloc;
		ret = omap_vout_setup_vrfb_bufs(pdev, vid_num,
				static_vrfb_allocation);
	}
<<<<<<< HEAD

	return ret;

free_buffers:
	for (i = 0; i < numbuffers; i++) {
		omap_vout_free_buffer(vout->buf_virt_addr[i],
						vout->buffer_size);
		vout->buf_virt_addr[i] = 0;
		vout->buf_phy_addr[i] = 0;
	}
	return ret;

=======
	return ret;
>>>>>>> upstream/android-13
}

/* Create video out devices */
static int __init omap_vout_create_video_devices(struct platform_device *pdev)
{
	int ret = 0, k;
	struct omap_vout_device *vout;
	struct video_device *vfd = NULL;
	struct v4l2_device *v4l2_dev = platform_get_drvdata(pdev);
	struct omap2video_device *vid_dev = container_of(v4l2_dev,
			struct omap2video_device, v4l2_dev);
<<<<<<< HEAD
=======
	struct omap_overlay *ovl = vid_dev->overlays[0];
	struct omap_overlay_info info;

	ovl->get_overlay_info(ovl, &info);
>>>>>>> upstream/android-13

	for (k = 0; k < pdev->num_resources; k++) {

		vout = kzalloc(sizeof(struct omap_vout_device), GFP_KERNEL);
		if (!vout) {
			dev_err(&pdev->dev, ": could not allocate memory\n");
			return -ENOMEM;
		}

		vout->vid = k;
		vid_dev->vouts[k] = vout;
		vout->vid_dev = vid_dev;
		/* Select video2 if only 1 overlay is controlled by V4L2 */
		if (pdev->num_resources == 1)
			vout->vid_info.overlays[0] = vid_dev->overlays[k + 2];
		else
			/* Else select video1 and video2 one by one. */
			vout->vid_info.overlays[0] = vid_dev->overlays[k + 1];
		vout->vid_info.num_overlays = 1;
		vout->vid_info.id = k + 1;
<<<<<<< HEAD
=======
		spin_lock_init(&vout->vbq_lock);
		/*
		 * Set the framebuffer base, this allows applications to find
		 * the fb corresponding to this overlay.
		 *
		 * To be precise: fbuf.base should match smem_start of
		 * struct fb_fix_screeninfo.
		 */
		vout->fbuf.base = (void *)info.paddr;
>>>>>>> upstream/android-13

		/* Set VRFB as rotation_type for omap2 and omap3 */
		if (omap_vout_dss_omap24xx() || omap_vout_dss_omap34xx())
			vout->vid_info.rotation_type = VOUT_ROT_VRFB;

		/* Setup the default configuration for the video devices
		 */
		if (omap_vout_setup_video_data(vout) != 0) {
			ret = -ENOMEM;
			goto error;
		}

		/* Allocate default number of buffers for the video streaming
		 * and reserve the VRFB space for rotation
		 */
		if (omap_vout_setup_video_bufs(pdev, k) != 0) {
			ret = -ENOMEM;
			goto error1;
		}

		/* Register the Video device with V4L2
		 */
		vfd = vout->vfd;
<<<<<<< HEAD
		if (video_register_device(vfd, VFL_TYPE_GRABBER, -1) < 0) {
=======
		if (video_register_device(vfd, VFL_TYPE_VIDEO, -1) < 0) {
>>>>>>> upstream/android-13
			dev_err(&pdev->dev,
				": Could not register Video for Linux device\n");
			vfd->minor = -1;
			ret = -ENODEV;
			goto error2;
		}
		video_set_drvdata(vfd, vout);

		dev_info(&pdev->dev,
			 ": registered and initialized video device %d\n",
			 vfd->minor);
		if (k == (pdev->num_resources - 1))
			return 0;

		continue;
error2:
		if (vout->vid_info.rotation_type == VOUT_ROT_VRFB)
			omap_vout_release_vrfb(vout);
<<<<<<< HEAD
		omap_vout_free_buffers(vout);
=======
>>>>>>> upstream/android-13
error1:
		video_device_release(vfd);
error:
		kfree(vout);
		return ret;
	}

	return -ENODEV;
}
/* Driver functions */
static void omap_vout_cleanup_device(struct omap_vout_device *vout)
{
	struct video_device *vfd;
	struct omapvideo_info *ovid;

	if (!vout)
		return;

	vfd = vout->vfd;
	ovid = &vout->vid_info;
	if (vfd) {
		if (!video_is_registered(vfd)) {
			/*
			 * The device was never registered, so release the
			 * video_device struct directly.
			 */
			video_device_release(vfd);
		} else {
			/*
			 * The unregister function will release the video_device
			 * struct as well as unregistering it.
			 */
			video_unregister_device(vfd);
		}
	}
	v4l2_ctrl_handler_free(&vout->ctrl_handler);
	if (ovid->rotation_type == VOUT_ROT_VRFB) {
		omap_vout_release_vrfb(vout);
		/* Free the VRFB buffer if allocated
		 * init time
		 */
		if (vout->vrfb_static_allocation)
			omap_vout_free_vrfb_buffers(vout);
	}
<<<<<<< HEAD
	omap_vout_free_buffers(vout);
=======
>>>>>>> upstream/android-13

	kfree(vout);
}

static int omap_vout_remove(struct platform_device *pdev)
{
	int k;
	struct v4l2_device *v4l2_dev = platform_get_drvdata(pdev);
	struct omap2video_device *vid_dev = container_of(v4l2_dev, struct
			omap2video_device, v4l2_dev);

	v4l2_device_unregister(v4l2_dev);
	for (k = 0; k < pdev->num_resources; k++)
		omap_vout_cleanup_device(vid_dev->vouts[k]);

	for (k = 0; k < vid_dev->num_displays; k++) {
		if (vid_dev->displays[k]->state != OMAP_DSS_DISPLAY_DISABLED)
			vid_dev->displays[k]->driver->disable(vid_dev->displays[k]);

		omap_dss_put_device(vid_dev->displays[k]);
	}
	kfree(vid_dev);
	return 0;
}

static int __init omap_vout_probe(struct platform_device *pdev)
{
	int ret = 0, i;
	struct omap_overlay *ovl;
	struct omap_dss_device *dssdev = NULL;
	struct omap_dss_device *def_display;
	struct omap2video_device *vid_dev = NULL;

	if (omapdss_is_initialized() == false)
		return -EPROBE_DEFER;

	ret = omapdss_compat_init();
	if (ret) {
		dev_err(&pdev->dev, "failed to init dss\n");
		return ret;
	}

	if (pdev->num_resources == 0) {
		dev_err(&pdev->dev, "probed for an unknown device\n");
		ret = -ENODEV;
		goto err_dss_init;
	}

	vid_dev = kzalloc(sizeof(struct omap2video_device), GFP_KERNEL);
	if (vid_dev == NULL) {
		ret = -ENOMEM;
		goto err_dss_init;
	}

	vid_dev->num_displays = 0;
	for_each_dss_dev(dssdev) {
		omap_dss_get_device(dssdev);

		if (!dssdev->driver) {
			dev_warn(&pdev->dev, "no driver for display: %s\n",
					dssdev->name);
			omap_dss_put_device(dssdev);
			continue;
		}

		vid_dev->displays[vid_dev->num_displays++] = dssdev;
	}

	if (vid_dev->num_displays == 0) {
		dev_err(&pdev->dev, "no displays\n");
		ret = -EINVAL;
		goto probe_err0;
	}

	vid_dev->num_overlays = omap_dss_get_num_overlays();
	for (i = 0; i < vid_dev->num_overlays; i++)
		vid_dev->overlays[i] = omap_dss_get_overlay(i);

	vid_dev->num_managers = omap_dss_get_num_overlay_managers();
	for (i = 0; i < vid_dev->num_managers; i++)
		vid_dev->managers[i] = omap_dss_get_overlay_manager(i);

	/* Get the Video1 overlay and video2 overlay.
	 * Setup the Display attached to that overlays
	 */
	for (i = 1; i < vid_dev->num_overlays; i++) {
		ovl = omap_dss_get_overlay(i);
		dssdev = ovl->get_device(ovl);

		if (dssdev) {
			def_display = dssdev;
		} else {
			dev_warn(&pdev->dev, "cannot find display\n");
			def_display = NULL;
		}
		if (def_display) {
			struct omap_dss_driver *dssdrv = def_display->driver;

			ret = dssdrv->enable(def_display);
			if (ret) {
				/* Here we are not considering a error
				 *  as display may be enabled by frame
				 *  buffer driver
				 */
				dev_warn(&pdev->dev,
					"'%s' Display already enabled\n",
					def_display->name);
			}
		}
	}

	if (v4l2_device_register(&pdev->dev, &vid_dev->v4l2_dev) < 0) {
		dev_err(&pdev->dev, "v4l2_device_register failed\n");
		ret = -ENODEV;
		goto probe_err1;
	}

	ret = omap_vout_create_video_devices(pdev);
	if (ret)
		goto probe_err2;

	for (i = 0; i < vid_dev->num_displays; i++) {
		struct omap_dss_device *display = vid_dev->displays[i];

		if (display->driver->update)
			display->driver->update(display, 0, 0,
					display->panel.timings.x_res,
					display->panel.timings.y_res);
	}
	return 0;

probe_err2:
	v4l2_device_unregister(&vid_dev->v4l2_dev);
probe_err1:
	for (i = 1; i < vid_dev->num_overlays; i++) {
		def_display = NULL;
		ovl = omap_dss_get_overlay(i);
		dssdev = ovl->get_device(ovl);

		if (dssdev)
			def_display = dssdev;

		if (def_display && def_display->driver)
			def_display->driver->disable(def_display);
	}
probe_err0:
	kfree(vid_dev);
err_dss_init:
	omapdss_compat_uninit();
	return ret;
}

static struct platform_driver omap_vout_driver = {
	.driver = {
		.name = VOUT_NAME,
	},
	.remove = omap_vout_remove,
};

static int __init omap_vout_init(void)
{
	if (platform_driver_probe(&omap_vout_driver, omap_vout_probe) != 0) {
		printk(KERN_ERR VOUT_NAME ":Could not register Video driver\n");
		return -EINVAL;
	}
	return 0;
}

static void omap_vout_cleanup(void)
{
	platform_driver_unregister(&omap_vout_driver);
}

late_initcall(omap_vout_init);
module_exit(omap_vout_cleanup);
