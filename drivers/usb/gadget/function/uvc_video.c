// SPDX-License-Identifier: GPL-2.0+
/*
 *	uvc_video.c  --  USB Video Class Gadget driver
 *
 *	Copyright (C) 2009-2010
 *	    Laurent Pinchart (laurent.pinchart@ideasonboard.com)
 */

#include <linux/kernel.h>
#include <linux/device.h>
#include <linux/errno.h>
#include <linux/usb/ch9.h>
#include <linux/usb/gadget.h>
#include <linux/usb/video.h>

#include <media/v4l2-dev.h>

#include "uvc.h"
#include "uvc_queue.h"
#include "uvc_video.h"

/* --------------------------------------------------------------------------
 * Video codecs
 */

static int
uvc_video_encode_header(struct uvc_video *video, struct uvc_buffer *buf,
		u8 *data, int len)
{
<<<<<<< HEAD
	data[0] = 2;
	data[1] = UVC_STREAM_EOH | video->fid;

	if (buf->bytesused - video->queue.buf_used <= len - 2)
=======
	data[0] = UVCG_REQUEST_HEADER_LEN;
	data[1] = UVC_STREAM_EOH | video->fid;

	if (buf->bytesused - video->queue.buf_used <= len - UVCG_REQUEST_HEADER_LEN)
>>>>>>> upstream/android-13
		data[1] |= UVC_STREAM_EOF;

	return 2;
}

static int
uvc_video_encode_data(struct uvc_video *video, struct uvc_buffer *buf,
		u8 *data, int len)
{
	struct uvc_video_queue *queue = &video->queue;
	unsigned int nbytes;
	void *mem;

	/* Copy video data to the USB buffer. */
	mem = buf->mem + queue->buf_used;
	nbytes = min((unsigned int)len, buf->bytesused - queue->buf_used);

	memcpy(data, mem, nbytes);
	queue->buf_used += nbytes;

	return nbytes;
}

static void
uvc_video_encode_bulk(struct usb_request *req, struct uvc_video *video,
		struct uvc_buffer *buf)
{
	void *mem = req->buf;
	int len = video->req_size;
	int ret;

	/* Add a header at the beginning of the payload. */
	if (video->payload_size == 0) {
		ret = uvc_video_encode_header(video, buf, mem, len);
		video->payload_size += ret;
		mem += ret;
		len -= ret;
	}

	/* Process video data. */
	len = min((int)(video->max_payload_size - video->payload_size), len);
	ret = uvc_video_encode_data(video, buf, mem, len);

	video->payload_size += ret;
	len -= ret;

	req->length = video->req_size - len;
	req->zero = video->payload_size == video->max_payload_size;

	if (buf->bytesused == video->queue.buf_used) {
		video->queue.buf_used = 0;
		buf->state = UVC_BUF_STATE_DONE;
		uvcg_queue_next_buffer(&video->queue, buf);
		video->fid ^= UVC_STREAM_FID;

		video->payload_size = 0;
	}

	if (video->payload_size == video->max_payload_size ||
	    buf->bytesused == video->queue.buf_used)
		video->payload_size = 0;
}

static void
<<<<<<< HEAD
=======
uvc_video_encode_isoc_sg(struct usb_request *req, struct uvc_video *video,
		struct uvc_buffer *buf)
{
	unsigned int pending = buf->bytesused - video->queue.buf_used;
	struct uvc_request *ureq = req->context;
	struct scatterlist *sg, *iter;
	unsigned int len = video->req_size;
	unsigned int sg_left, part = 0;
	unsigned int i;
	int ret;

	sg = ureq->sgt.sgl;
	sg_init_table(sg, ureq->sgt.nents);

	/* Init the header. */
	ret = uvc_video_encode_header(video, buf, ureq->header,
				      video->req_size);
	sg_set_buf(sg, ureq->header, UVCG_REQUEST_HEADER_LEN);
	len -= ret;

	if (pending <= len)
		len = pending;

	req->length = (len == pending) ?
		len + UVCG_REQUEST_HEADER_LEN : video->req_size;

	/* Init the pending sgs with payload */
	sg = sg_next(sg);

	for_each_sg(sg, iter, ureq->sgt.nents - 1, i) {
		if (!len || !buf->sg)
			break;

		sg_left = sg_dma_len(buf->sg) - buf->offset;
		part = min_t(unsigned int, len, sg_left);

		sg_set_page(iter, sg_page(buf->sg), part, buf->offset);

		if (part == sg_left) {
			buf->offset = 0;
			buf->sg = sg_next(buf->sg);
		} else {
			buf->offset += part;
		}
		len -= part;
	}

	/* Assign the video data with header. */
	req->buf = NULL;
	req->sg	= ureq->sgt.sgl;
	req->num_sgs = i + 1;

	req->length -= len;
	video->queue.buf_used += req->length - UVCG_REQUEST_HEADER_LEN;

	if (buf->bytesused == video->queue.buf_used || !buf->sg) {
		video->queue.buf_used = 0;
		buf->state = UVC_BUF_STATE_DONE;
		buf->offset = 0;
		uvcg_queue_next_buffer(&video->queue, buf);
		video->fid ^= UVC_STREAM_FID;
	}
}

static void
>>>>>>> upstream/android-13
uvc_video_encode_isoc(struct usb_request *req, struct uvc_video *video,
		struct uvc_buffer *buf)
{
	void *mem = req->buf;
	int len = video->req_size;
	int ret;

	/* Add the header. */
	ret = uvc_video_encode_header(video, buf, mem, len);
	mem += ret;
	len -= ret;

	/* Process video data. */
	ret = uvc_video_encode_data(video, buf, mem, len);
	len -= ret;

	req->length = video->req_size - len;

	if (buf->bytesused == video->queue.buf_used) {
		video->queue.buf_used = 0;
		buf->state = UVC_BUF_STATE_DONE;
		uvcg_queue_next_buffer(&video->queue, buf);
		video->fid ^= UVC_STREAM_FID;
	}
}

/* --------------------------------------------------------------------------
 * Request handling
 */

static int uvcg_video_ep_queue(struct uvc_video *video, struct usb_request *req)
{
	int ret;

	ret = usb_ep_queue(video->ep, req, GFP_ATOMIC);
	if (ret < 0) {
<<<<<<< HEAD
		printk(KERN_INFO "Failed to queue request (%d).\n", ret);
		/* Isochronous endpoints can't be halted. */
		if (usb_endpoint_xfer_bulk(video->ep->desc))
			usb_ep_set_halt(video->ep);
=======
		uvcg_err(&video->uvc->func, "Failed to queue request (%d).\n",
			 ret);

		/* If the endpoint is disabled the descriptor may be NULL. */
		if (video->ep->desc) {
			/* Isochronous endpoints can't be halted. */
			if (usb_endpoint_xfer_bulk(video->ep->desc))
				usb_ep_set_halt(video->ep);
		}
>>>>>>> upstream/android-13
	}

	return ret;
}

<<<<<<< HEAD
/*
 * I somehow feel that synchronisation won't be easy to achieve here. We have
 * three events that control USB requests submission:
 *
 * - USB request completion: the completion handler will resubmit the request
 *   if a video buffer is available.
 *
 * - USB interface setting selection: in response to a SET_INTERFACE request,
 *   the handler will start streaming if a video buffer is available and if
 *   video is not currently streaming.
 *
 * - V4L2 buffer queueing: the driver will start streaming if video is not
 *   currently streaming.
 *
 * Race conditions between those 3 events might lead to deadlocks or other
 * nasty side effects.
 *
 * The "video currently streaming" condition can't be detected by the irqqueue
 * being empty, as a request can still be in flight. A separate "queue paused"
 * flag is thus needed.
 *
 * The paused flag will be set when we try to retrieve the irqqueue head if the
 * queue is empty, and cleared when we queue a buffer.
 *
 * The USB request completion handler will get the buffer at the irqqueue head
 * under protection of the queue spinlock. If the queue is empty, the streaming
 * paused flag will be set. Right after releasing the spinlock a userspace
 * application can queue a buffer. The flag will then cleared, and the ioctl
 * handler will restart the video stream.
 */
static void
uvc_video_complete(struct usb_ep *ep, struct usb_request *req)
{
	struct uvc_video *video = req->context;
	struct uvc_video_queue *queue = &video->queue;
	struct uvc_buffer *buf;
	unsigned long flags;
	int ret;
=======
static void
uvc_video_complete(struct usb_ep *ep, struct usb_request *req)
{
	struct uvc_request *ureq = req->context;
	struct uvc_video *video = ureq->video;
	struct uvc_video_queue *queue = &video->queue;
	struct uvc_device *uvc = video->uvc;
	unsigned long flags;
>>>>>>> upstream/android-13

	switch (req->status) {
	case 0:
		break;

	case -ESHUTDOWN:	/* disconnect from host. */
<<<<<<< HEAD
		printk(KERN_DEBUG "VS request cancelled.\n");
		uvcg_queue_cancel(queue, 1);
		goto requeue;

	default:
		printk(KERN_INFO "VS request completed with status %d.\n",
			req->status);
		uvcg_queue_cancel(queue, 0);
		goto requeue;
	}

	spin_lock_irqsave(&video->queue.irqlock, flags);
	buf = uvcg_queue_head(&video->queue);
	if (buf == NULL) {
		spin_unlock_irqrestore(&video->queue.irqlock, flags);
		goto requeue;
	}

	video->encode(req, video, buf);

	ret = uvcg_video_ep_queue(video, req);
	spin_unlock_irqrestore(&video->queue.irqlock, flags);

	if (ret < 0) {
		uvcg_queue_cancel(queue, 0);
		goto requeue;
	}

	return;

requeue:
	spin_lock_irqsave(&video->req_lock, flags);
	list_add_tail(&req->list, &video->req_free);
	spin_unlock_irqrestore(&video->req_lock, flags);
=======
		uvcg_dbg(&video->uvc->func, "VS request cancelled.\n");
		uvcg_queue_cancel(queue, 1);
		break;

	default:
		uvcg_info(&video->uvc->func,
			  "VS request completed with status %d.\n",
			  req->status);
		uvcg_queue_cancel(queue, 0);
	}

	spin_lock_irqsave(&video->req_lock, flags);
	list_add_tail(&req->list, &video->req_free);
	spin_unlock_irqrestore(&video->req_lock, flags);

	if (uvc->state == UVC_STATE_STREAMING)
		schedule_work(&video->pump);
>>>>>>> upstream/android-13
}

static int
uvc_video_free_requests(struct uvc_video *video)
{
	unsigned int i;

<<<<<<< HEAD
	for (i = 0; i < UVC_NUM_REQUESTS; ++i) {
		if (video->req[i]) {
			usb_ep_free_request(video->ep, video->req[i]);
			video->req[i] = NULL;
		}

		if (video->req_buffer[i]) {
			kfree(video->req_buffer[i]);
			video->req_buffer[i] = NULL;
		}
=======
	if (video->ureq) {
		for (i = 0; i < video->uvc_num_requests; ++i) {
			sg_free_table(&video->ureq[i].sgt);

			if (video->ureq[i].req) {
				usb_ep_free_request(video->ep, video->ureq[i].req);
				video->ureq[i].req = NULL;
			}

			if (video->ureq[i].req_buffer) {
				kfree(video->ureq[i].req_buffer);
				video->ureq[i].req_buffer = NULL;
			}
		}

		kfree(video->ureq);
		video->ureq = NULL;
>>>>>>> upstream/android-13
	}

	INIT_LIST_HEAD(&video->req_free);
	video->req_size = 0;
	return 0;
}

static int
uvc_video_alloc_requests(struct uvc_video *video)
{
	unsigned int req_size;
	unsigned int i;
	int ret = -ENOMEM;

	BUG_ON(video->req_size);

	req_size = video->ep->maxpacket
		 * max_t(unsigned int, video->ep->maxburst, 1)
		 * (video->ep->mult);

<<<<<<< HEAD
	for (i = 0; i < UVC_NUM_REQUESTS; ++i) {
		video->req_buffer[i] = kmalloc(req_size, GFP_KERNEL);
		if (video->req_buffer[i] == NULL)
			goto error;

		video->req[i] = usb_ep_alloc_request(video->ep, GFP_KERNEL);
		if (video->req[i] == NULL)
			goto error;

		video->req[i]->buf = video->req_buffer[i];
		video->req[i]->length = 0;
		video->req[i]->complete = uvc_video_complete;
		video->req[i]->context = video;

		list_add_tail(&video->req[i]->list, &video->req_free);
=======
	video->ureq = kcalloc(video->uvc_num_requests, sizeof(struct uvc_request), GFP_KERNEL);
	if (video->ureq == NULL)
		return -ENOMEM;

	for (i = 0; i < video->uvc_num_requests; ++i) {
		video->ureq[i].req_buffer = kmalloc(req_size, GFP_KERNEL);
		if (video->ureq[i].req_buffer == NULL)
			goto error;

		video->ureq[i].req = usb_ep_alloc_request(video->ep, GFP_KERNEL);
		if (video->ureq[i].req == NULL)
			goto error;

		video->ureq[i].req->buf = video->ureq[i].req_buffer;
		video->ureq[i].req->length = 0;
		video->ureq[i].req->complete = uvc_video_complete;
		video->ureq[i].req->context = &video->ureq[i];
		video->ureq[i].video = video;

		list_add_tail(&video->ureq[i].req->list, &video->req_free);
		/* req_size/PAGE_SIZE + 1 for overruns and + 1 for header */
		sg_alloc_table(&video->ureq[i].sgt,
			       DIV_ROUND_UP(req_size - 2, PAGE_SIZE) + 2,
			       GFP_KERNEL);
>>>>>>> upstream/android-13
	}

	video->req_size = req_size;

	return 0;

error:
	uvc_video_free_requests(video);
	return ret;
}

/* --------------------------------------------------------------------------
 * Video streaming
 */

/*
 * uvcg_video_pump - Pump video data into the USB requests
 *
 * This function fills the available USB requests (listed in req_free) with
 * video data from the queued buffers.
 */
<<<<<<< HEAD
int uvcg_video_pump(struct uvc_video *video)
{
	struct uvc_video_queue *queue = &video->queue;
	struct usb_request *req;
=======
static void uvcg_video_pump(struct work_struct *work)
{
	struct uvc_video *video = container_of(work, struct uvc_video, pump);
	struct uvc_video_queue *queue = &video->queue;
	struct usb_request *req = NULL;
>>>>>>> upstream/android-13
	struct uvc_buffer *buf;
	unsigned long flags;
	int ret;

<<<<<<< HEAD
	/* FIXME TODO Race between uvcg_video_pump and requests completion
	 * handler ???
	 */

	while (1) {
=======
	while (video->ep->enabled) {
>>>>>>> upstream/android-13
		/* Retrieve the first available USB request, protected by the
		 * request lock.
		 */
		spin_lock_irqsave(&video->req_lock, flags);
		if (list_empty(&video->req_free)) {
			spin_unlock_irqrestore(&video->req_lock, flags);
<<<<<<< HEAD
			return 0;
=======
			return;
>>>>>>> upstream/android-13
		}
		req = list_first_entry(&video->req_free, struct usb_request,
					list);
		list_del(&req->list);
		spin_unlock_irqrestore(&video->req_lock, flags);

		/* Retrieve the first available video buffer and fill the
		 * request, protected by the video queue irqlock.
		 */
		spin_lock_irqsave(&queue->irqlock, flags);
		buf = uvcg_queue_head(queue);
		if (buf == NULL) {
			spin_unlock_irqrestore(&queue->irqlock, flags);
			break;
		}

		video->encode(req, video, buf);

<<<<<<< HEAD
=======
		/* With usb3 we have more requests. This will decrease the
		 * interrupt load to a quarter but also catches the corner
		 * cases, which needs to be handled */
		if (list_empty(&video->req_free) ||
		    buf->state == UVC_BUF_STATE_DONE ||
		    !(video->req_int_count %
		       DIV_ROUND_UP(video->uvc_num_requests, 4))) {
			video->req_int_count = 0;
			req->no_interrupt = 0;
		} else {
			req->no_interrupt = 1;
		}

>>>>>>> upstream/android-13
		/* Queue the USB request */
		ret = uvcg_video_ep_queue(video, req);
		spin_unlock_irqrestore(&queue->irqlock, flags);

		if (ret < 0) {
			uvcg_queue_cancel(queue, 0);
			break;
		}
<<<<<<< HEAD
	}

	spin_lock_irqsave(&video->req_lock, flags);
	list_add_tail(&req->list, &video->req_free);
	spin_unlock_irqrestore(&video->req_lock, flags);
	return 0;
=======

		/* Endpoint now owns the request */
		req = NULL;
		video->req_int_count++;
	}

	if (!req)
		return;

	spin_lock_irqsave(&video->req_lock, flags);
	list_add_tail(&req->list, &video->req_free);
	spin_unlock_irqrestore(&video->req_lock, flags);
	return;
>>>>>>> upstream/android-13
}

/*
 * Enable or disable the video stream.
 */
int uvcg_video_enable(struct uvc_video *video, int enable)
{
	unsigned int i;
	int ret;

	if (video->ep == NULL) {
<<<<<<< HEAD
		printk(KERN_INFO "Video enable failed, device is "
			"uninitialized.\n");
=======
		uvcg_info(&video->uvc->func,
			  "Video enable failed, device is uninitialized.\n");
>>>>>>> upstream/android-13
		return -ENODEV;
	}

	if (!enable) {
<<<<<<< HEAD
		for (i = 0; i < UVC_NUM_REQUESTS; ++i)
			if (video->req[i])
				usb_ep_dequeue(video->ep, video->req[i]);
=======
		cancel_work_sync(&video->pump);
		uvcg_queue_cancel(&video->queue, 0);

		for (i = 0; i < video->uvc_num_requests; ++i)
			if (video->ureq && video->ureq[i].req)
				usb_ep_dequeue(video->ep, video->ureq[i].req);
>>>>>>> upstream/android-13

		uvc_video_free_requests(video);
		uvcg_queue_enable(&video->queue, 0);
		return 0;
	}

	if ((ret = uvcg_queue_enable(&video->queue, 1)) < 0)
		return ret;

	if ((ret = uvc_video_alloc_requests(video)) < 0)
		return ret;

	if (video->max_payload_size) {
		video->encode = uvc_video_encode_bulk;
		video->payload_size = 0;
	} else
<<<<<<< HEAD
		video->encode = uvc_video_encode_isoc;

	return uvcg_video_pump(video);
=======
		video->encode = video->queue.use_sg ?
			uvc_video_encode_isoc_sg : uvc_video_encode_isoc;

	video->req_int_count = 0;

	schedule_work(&video->pump);

	return ret;
>>>>>>> upstream/android-13
}

/*
 * Initialize the UVC video stream.
 */
<<<<<<< HEAD
int uvcg_video_init(struct uvc_video *video)
{
	INIT_LIST_HEAD(&video->req_free);
	spin_lock_init(&video->req_lock);

=======
int uvcg_video_init(struct uvc_video *video, struct uvc_device *uvc)
{
	INIT_LIST_HEAD(&video->req_free);
	spin_lock_init(&video->req_lock);
	INIT_WORK(&video->pump, uvcg_video_pump);

	video->uvc = uvc;
>>>>>>> upstream/android-13
	video->fcc = V4L2_PIX_FMT_YUYV;
	video->bpp = 16;
	video->width = 320;
	video->height = 240;
	video->imagesize = 320 * 240 * 2;

	/* Initialize the video buffers queue. */
<<<<<<< HEAD
	uvcg_queue_init(&video->queue, V4L2_BUF_TYPE_VIDEO_OUTPUT,
			&video->mutex);
=======
	uvcg_queue_init(&video->queue, uvc->v4l2_dev.dev->parent,
			V4L2_BUF_TYPE_VIDEO_OUTPUT, &video->mutex);
>>>>>>> upstream/android-13
	return 0;
}

