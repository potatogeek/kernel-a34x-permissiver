/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __USB_STREAM_H
#define __USB_STREAM_H

#include <uapi/sound/usb_stream.h>

#define USB_STREAM_NURBS 4
#define USB_STREAM_URBDEPTH 4

struct usb_stream_kernel {
	struct usb_stream *s;

	void *write_page;

<<<<<<< HEAD
	unsigned n_o_ps;
=======
	unsigned int n_o_ps;
>>>>>>> upstream/android-13

	struct urb *inurb[USB_STREAM_NURBS];
	struct urb *idle_inurb;
	struct urb *completed_inurb;
	struct urb *outurb[USB_STREAM_NURBS];
	struct urb *idle_outurb;
	struct urb *completed_outurb;
	struct urb *i_urb;

	int iso_frame_balance;

	wait_queue_head_t sleep;

<<<<<<< HEAD
	unsigned out_phase;
	unsigned out_phase_peeked;
	unsigned freqn;
=======
	unsigned int out_phase;
	unsigned int out_phase_peeked;
	unsigned int freqn;
>>>>>>> upstream/android-13
};

struct usb_stream *usb_stream_new(struct usb_stream_kernel *sk,
				  struct usb_device *dev,
<<<<<<< HEAD
				  unsigned in_endpoint, unsigned out_endpoint,
				  unsigned sample_rate, unsigned use_packsize,
				  unsigned period_frames, unsigned frame_size);
void usb_stream_free(struct usb_stream_kernel *);
int usb_stream_start(struct usb_stream_kernel *);
void usb_stream_stop(struct usb_stream_kernel *);
=======
				  unsigned int in_endpoint,
				  unsigned int out_endpoint,
				  unsigned int sample_rate,
				  unsigned int use_packsize,
				  unsigned int period_frames,
				  unsigned int frame_size);
void usb_stream_free(struct usb_stream_kernel *sk);
int usb_stream_start(struct usb_stream_kernel *sk);
void usb_stream_stop(struct usb_stream_kernel *sk);
>>>>>>> upstream/android-13

#endif /* __USB_STREAM_H */
