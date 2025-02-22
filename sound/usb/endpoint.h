/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __USBAUDIO_ENDPOINT_H
#define __USBAUDIO_ENDPOINT_H

#define SND_USB_ENDPOINT_TYPE_DATA     0
#define SND_USB_ENDPOINT_TYPE_SYNC     1

<<<<<<< HEAD
struct snd_usb_endpoint *snd_usb_add_endpoint(struct snd_usb_audio *chip,
					      struct usb_host_interface *alts,
					      int ep_num, int direction, int type);

int snd_usb_endpoint_set_params(struct snd_usb_endpoint *ep,
				snd_pcm_format_t pcm_format,
				unsigned int channels,
				unsigned int period_bytes,
				unsigned int period_frames,
				unsigned int buffer_periods,
				unsigned int rate,
				struct audioformat *fmt,
				struct snd_usb_endpoint *sync_ep);

int  snd_usb_endpoint_start(struct snd_usb_endpoint *ep);
void snd_usb_endpoint_stop(struct snd_usb_endpoint *ep);
void snd_usb_endpoint_sync_pending_stop(struct snd_usb_endpoint *ep);
int  snd_usb_endpoint_activate(struct snd_usb_endpoint *ep);
void snd_usb_endpoint_deactivate(struct snd_usb_endpoint *ep);
void snd_usb_endpoint_release(struct snd_usb_endpoint *ep);
void snd_usb_endpoint_free(struct snd_usb_endpoint *ep);

int snd_usb_endpoint_implicit_feedback_sink(struct snd_usb_endpoint *ep);
int snd_usb_endpoint_next_packet_size(struct snd_usb_endpoint *ep);

void snd_usb_handle_sync_urb(struct snd_usb_endpoint *ep,
			     struct snd_usb_endpoint *sender,
			     const struct urb *urb);
=======
struct snd_usb_endpoint *snd_usb_get_endpoint(struct snd_usb_audio *chip,
					      int ep_num);

int snd_usb_add_endpoint(struct snd_usb_audio *chip, int ep_num, int type);

struct snd_usb_endpoint *
snd_usb_endpoint_open(struct snd_usb_audio *chip,
		      const struct audioformat *fp,
		      const struct snd_pcm_hw_params *params,
		      bool is_sync_ep);
void snd_usb_endpoint_close(struct snd_usb_audio *chip,
			    struct snd_usb_endpoint *ep);
int snd_usb_endpoint_configure(struct snd_usb_audio *chip,
			       struct snd_usb_endpoint *ep);
int snd_usb_endpoint_get_clock_rate(struct snd_usb_audio *chip, int clock);

bool snd_usb_endpoint_compatible(struct snd_usb_audio *chip,
				 struct snd_usb_endpoint *ep,
				 const struct audioformat *fp,
				 const struct snd_pcm_hw_params *params);

void snd_usb_endpoint_set_sync(struct snd_usb_audio *chip,
			       struct snd_usb_endpoint *data_ep,
			       struct snd_usb_endpoint *sync_ep);
void snd_usb_endpoint_set_callback(struct snd_usb_endpoint *ep,
				   int (*prepare)(struct snd_usb_substream *subs,
						  struct urb *urb,
						  bool in_stream_lock),
				   void (*retire)(struct snd_usb_substream *subs,
						  struct urb *urb),
				   struct snd_usb_substream *data_subs);

int snd_usb_endpoint_start(struct snd_usb_endpoint *ep);
void snd_usb_endpoint_stop(struct snd_usb_endpoint *ep, bool keep_pending);
void snd_usb_endpoint_sync_pending_stop(struct snd_usb_endpoint *ep);
void snd_usb_endpoint_suspend(struct snd_usb_endpoint *ep);
int  snd_usb_endpoint_activate(struct snd_usb_endpoint *ep);
void snd_usb_endpoint_release(struct snd_usb_endpoint *ep);
void snd_usb_endpoint_free_all(struct snd_usb_audio *chip);

int snd_usb_endpoint_implicit_feedback_sink(struct snd_usb_endpoint *ep);
int snd_usb_endpoint_next_packet_size(struct snd_usb_endpoint *ep,
				      struct snd_urb_ctx *ctx, int idx,
				      unsigned int avail);
void snd_usb_queue_pending_output_urbs(struct snd_usb_endpoint *ep,
				       bool in_stream_lock);
>>>>>>> upstream/android-13

#endif /* __USBAUDIO_ENDPOINT_H */
