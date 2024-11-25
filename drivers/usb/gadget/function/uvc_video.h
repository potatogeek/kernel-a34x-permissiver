<<<<<<< HEAD
// SPDX-License-Identifier: GPL-2.0
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> upstream/android-13
/*
 *	uvc_video.h  --  USB Video Class Gadget driver
 *
 * Copyright (C) 2009-2010
 *		Laurent Pinchart (laurent.pinchart@ideasonboard.com)
 *
 * Copyright (c) 2013 Samsung Electronics Co., Ltd.
 *		http://www.samsung.com
<<<<<<< HEAD
 *		Author: Andrzej Pietrasiewicz <andrzej.p@samsung.com>
=======
 *		Author: Andrzej Pietrasiewicz <andrzejtp2010@gmail.com>
>>>>>>> upstream/android-13
 */
#ifndef __UVC_VIDEO_H__
#define __UVC_VIDEO_H__

<<<<<<< HEAD
struct uvc_video;

int uvcg_video_pump(struct uvc_video *video);

int uvcg_video_enable(struct uvc_video *video, int enable);

int uvcg_video_init(struct uvc_video *video);
=======
#define UVCG_REQUEST_HEADER_LEN			2

struct uvc_video;

int uvcg_video_enable(struct uvc_video *video, int enable);

int uvcg_video_init(struct uvc_video *video, struct uvc_device *uvc);
>>>>>>> upstream/android-13

#endif /* __UVC_VIDEO_H__ */
