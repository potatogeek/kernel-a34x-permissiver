<<<<<<< HEAD
/*
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
=======
// SPDX-License-Identifier: GPL-2.0-or-later
/*
>>>>>>> upstream/android-13
 */

/* USX2Y "rawusb" aka hwdep_pcm implementation

 Its usb's unableness to atomically handle power of 2 period sized data chuncs
 at standard samplerates,
<<<<<<< HEAD
 what led to this part of the usx2y module: 
=======
 what led to this part of the usx2y module:
>>>>>>> upstream/android-13
 It provides the alsa kernel half of the usx2y-alsa-jack driver pair.
 The pair uses a hardware dependent alsa-device for mmaped pcm transport.
 Advantage achieved:
         The usb_hc moves pcm data from/into memory via DMA.
         That memory is mmaped by jack's usx2y driver.
         Jack's usx2y driver is the first/last to read/write pcm data.
         Read/write is a combination of power of 2 period shaping and
         float/int conversation.
         Compared to mainline alsa/jack we leave out power of 2 period shaping inside
         snd-usb-usx2y which needs memcpy() and additional buffers.
         As a side effect possible unwanted pcm-data coruption resulting of
         standard alsa's snd-usb-usx2y period shaping scheme falls away.
         Result is sane jack operation at buffering schemes down to 128frames,
         2 periods.
         plain usx2y alsa mode is able to achieve 64frames, 4periods, but only at the
         cost of easier triggered i.e. aeolus xruns (128 or 256frames,
         2periods works but is useless cause of crackling).

 This is a first "proof of concept" implementation.
 Later, functionalities should migrate to more appropriate places:
 Userland:
 - The jackd could mmap its float-pcm buffers directly from alsa-lib.
 - alsa-lib could provide power of 2 period sized shaping combined with int/float
   conversation.
   Currently the usx2y jack driver provides above 2 services.
 Kernel:
 - rawusb dma pcm buffer transport should go to snd-usb-lib, so also snd-usb-audio
   devices can use it.
<<<<<<< HEAD
   Currently rawusb dma pcm buffer transport (this file) is only available to snd-usb-usx2y. 
=======
   Currently rawusb dma pcm buffer transport (this file) is only available to snd-usb-usx2y.
>>>>>>> upstream/android-13
*/

#include <linux/delay.h>
#include <linux/gfp.h>
#include "usbusx2yaudio.c"

#if defined(USX2Y_NRPACKS_VARIABLE) || USX2Y_NRPACKS == 1

#include <sound/hwdep.h>

<<<<<<< HEAD

static int usX2Y_usbpcm_urb_capt_retire(struct snd_usX2Y_substream *subs)
{
	struct urb	*urb = subs->completed_urb;
	struct snd_pcm_runtime *runtime = subs->pcm_substream->runtime;
	int 		i, lens = 0, hwptr_done = subs->hwptr_done;
	struct usX2Ydev	*usX2Y = subs->usX2Y;
	if (0 > usX2Y->hwdep_pcm_shm->capture_iso_start) { //FIXME
		int head = usX2Y->hwdep_pcm_shm->captured_iso_head + 1;
		if (head >= ARRAY_SIZE(usX2Y->hwdep_pcm_shm->captured_iso))
			head = 0;
		usX2Y->hwdep_pcm_shm->capture_iso_start = head;
=======
static int usx2y_usbpcm_urb_capt_retire(struct snd_usx2y_substream *subs)
{
	struct urb	*urb = subs->completed_urb;
	struct snd_pcm_runtime *runtime = subs->pcm_substream->runtime;
	int		i, lens = 0, hwptr_done = subs->hwptr_done;
	struct usx2ydev	*usx2y = subs->usx2y;
	int head;

	if (usx2y->hwdep_pcm_shm->capture_iso_start < 0) { //FIXME
		head = usx2y->hwdep_pcm_shm->captured_iso_head + 1;
		if (head >= ARRAY_SIZE(usx2y->hwdep_pcm_shm->captured_iso))
			head = 0;
		usx2y->hwdep_pcm_shm->capture_iso_start = head;
>>>>>>> upstream/android-13
		snd_printdd("cap start %i\n", head);
	}
	for (i = 0; i < nr_of_packs(); i++) {
		if (urb->iso_frame_desc[i].status) { /* active? hmm, skip this */
<<<<<<< HEAD
			snd_printk(KERN_ERR "active frame status %i. Most probably some hardware problem.\n", urb->iso_frame_desc[i].status);
			return urb->iso_frame_desc[i].status;
		}
		lens += urb->iso_frame_desc[i].actual_length / usX2Y->stride;
	}
	if ((hwptr_done += lens) >= runtime->buffer_size)
=======
			snd_printk(KERN_ERR
				   "active frame status %i. Most probably some hardware problem.\n",
				   urb->iso_frame_desc[i].status);
			return urb->iso_frame_desc[i].status;
		}
		lens += urb->iso_frame_desc[i].actual_length / usx2y->stride;
	}
	hwptr_done += lens;
	if (hwptr_done >= runtime->buffer_size)
>>>>>>> upstream/android-13
		hwptr_done -= runtime->buffer_size;
	subs->hwptr_done = hwptr_done;
	subs->transfer_done += lens;
	/* update the pointer, call callback if necessary */
	if (subs->transfer_done >= runtime->period_size) {
		subs->transfer_done -= runtime->period_size;
		snd_pcm_period_elapsed(subs->pcm_substream);
	}
	return 0;
}

<<<<<<< HEAD
static inline int usX2Y_iso_frames_per_buffer(struct snd_pcm_runtime *runtime,
					      struct usX2Ydev * usX2Y)
{
	return (runtime->buffer_size * 1000) / usX2Y->rate + 1;	//FIXME: so far only correct period_size == 2^x ?
=======
static int usx2y_iso_frames_per_buffer(struct snd_pcm_runtime *runtime,
					      struct usx2ydev *usx2y)
{
	return (runtime->buffer_size * 1000) / usx2y->rate + 1;	//FIXME: so far only correct period_size == 2^x ?
>>>>>>> upstream/android-13
}

/*
 * prepare urb for playback data pipe
 *
 * we copy the data directly from the pcm buffer.
 * the current position to be copied is held in hwptr field.
 * since a urb can handle only a single linear buffer, if the total
 * transferred area overflows the buffer boundary, we cannot send
 * it directly from the buffer.  thus the data is once copied to
 * a temporary buffer and urb points to that.
 */
<<<<<<< HEAD
static int usX2Y_hwdep_urb_play_prepare(struct snd_usX2Y_substream *subs,
					struct urb *urb)
{
	int count, counts, pack;
	struct usX2Ydev *usX2Y = subs->usX2Y;
	struct snd_usX2Y_hwdep_pcm_shm *shm = usX2Y->hwdep_pcm_shm;
	struct snd_pcm_runtime *runtime = subs->pcm_substream->runtime;

	if (0 > shm->playback_iso_start) {
		shm->playback_iso_start = shm->captured_iso_head -
			usX2Y_iso_frames_per_buffer(runtime, usX2Y);
		if (0 > shm->playback_iso_start)
=======
static int usx2y_hwdep_urb_play_prepare(struct snd_usx2y_substream *subs,
					struct urb *urb)
{
	int count, counts, pack;
	struct usx2ydev *usx2y = subs->usx2y;
	struct snd_usx2y_hwdep_pcm_shm *shm = usx2y->hwdep_pcm_shm;
	struct snd_pcm_runtime *runtime = subs->pcm_substream->runtime;

	if (shm->playback_iso_start < 0) {
		shm->playback_iso_start = shm->captured_iso_head -
			usx2y_iso_frames_per_buffer(runtime, usx2y);
		if (shm->playback_iso_start < 0)
>>>>>>> upstream/android-13
			shm->playback_iso_start += ARRAY_SIZE(shm->captured_iso);
		shm->playback_iso_head = shm->playback_iso_start;
	}

	count = 0;
	for (pack = 0; pack < nr_of_packs(); pack++) {
		/* calculate the size of a packet */
<<<<<<< HEAD
		counts = shm->captured_iso[shm->playback_iso_head].length / usX2Y->stride;
=======
		counts = shm->captured_iso[shm->playback_iso_head].length / usx2y->stride;
>>>>>>> upstream/android-13
		if (counts < 43 || counts > 50) {
			snd_printk(KERN_ERR "should not be here with counts=%i\n", counts);
			return -EPIPE;
		}
		/* set up descriptor */
		urb->iso_frame_desc[pack].offset = shm->captured_iso[shm->playback_iso_head].offset;
		urb->iso_frame_desc[pack].length = shm->captured_iso[shm->playback_iso_head].length;
<<<<<<< HEAD
		if (atomic_read(&subs->state) != state_RUNNING)
=======
		if (atomic_read(&subs->state) != STATE_RUNNING)
>>>>>>> upstream/android-13
			memset((char *)urb->transfer_buffer + urb->iso_frame_desc[pack].offset, 0,
			       urb->iso_frame_desc[pack].length);
		if (++shm->playback_iso_head >= ARRAY_SIZE(shm->captured_iso))
			shm->playback_iso_head = 0;
		count += counts;
	}
<<<<<<< HEAD
	urb->transfer_buffer_length = count * usX2Y->stride;
	return 0;
}


static inline void usX2Y_usbpcm_urb_capt_iso_advance(struct snd_usX2Y_substream *subs,
						     struct urb *urb)
{
	int pack;
	for (pack = 0; pack < nr_of_packs(); ++pack) {
		struct usb_iso_packet_descriptor *desc = urb->iso_frame_desc + pack;
		if (NULL != subs) {
			struct snd_usX2Y_hwdep_pcm_shm *shm = subs->usX2Y->hwdep_pcm_shm;
			int head = shm->captured_iso_head + 1;
=======
	urb->transfer_buffer_length = count * usx2y->stride;
	return 0;
}

static void usx2y_usbpcm_urb_capt_iso_advance(struct snd_usx2y_substream *subs,
					      struct urb *urb)
{
	struct usb_iso_packet_descriptor *desc;
	struct snd_usx2y_hwdep_pcm_shm *shm;
	int pack, head;

	for (pack = 0; pack < nr_of_packs(); ++pack) {
		desc = urb->iso_frame_desc + pack;
		if (subs) {
			shm = subs->usx2y->hwdep_pcm_shm;
			head = shm->captured_iso_head + 1;
>>>>>>> upstream/android-13
			if (head >= ARRAY_SIZE(shm->captured_iso))
				head = 0;
			shm->captured_iso[head].frame = urb->start_frame + pack;
			shm->captured_iso[head].offset = desc->offset;
			shm->captured_iso[head].length = desc->actual_length;
			shm->captured_iso_head = head;
			shm->captured_iso_frames++;
		}
<<<<<<< HEAD
		if ((desc->offset += desc->length * NRURBS*nr_of_packs()) +
		    desc->length >= SSS)
=======
		desc->offset += desc->length * NRURBS * nr_of_packs();
		if (desc->offset + desc->length >= SSS)
>>>>>>> upstream/android-13
			desc->offset -= (SSS - desc->length);
	}
}

<<<<<<< HEAD
static inline int usX2Y_usbpcm_usbframe_complete(struct snd_usX2Y_substream *capsubs,
						 struct snd_usX2Y_substream *capsubs2,
						 struct snd_usX2Y_substream *playbacksubs,
						 int frame)
=======
static int usx2y_usbpcm_usbframe_complete(struct snd_usx2y_substream *capsubs,
					  struct snd_usx2y_substream *capsubs2,
					  struct snd_usx2y_substream *playbacksubs,
					  int frame)
>>>>>>> upstream/android-13
{
	int err, state;
	struct urb *urb = playbacksubs->completed_urb;

	state = atomic_read(&playbacksubs->state);
<<<<<<< HEAD
	if (NULL != urb) {
		if (state == state_RUNNING)
			usX2Y_urb_play_retire(playbacksubs, urb);
		else if (state >= state_PRERUNNING)
			atomic_inc(&playbacksubs->state);
	} else {
		switch (state) {
		case state_STARTING1:
			urb = playbacksubs->urb[0];
			atomic_inc(&playbacksubs->state);
			break;
		case state_STARTING2:
=======
	if (urb) {
		if (state == STATE_RUNNING)
			usx2y_urb_play_retire(playbacksubs, urb);
		else if (state >= STATE_PRERUNNING)
			atomic_inc(&playbacksubs->state);
	} else {
		switch (state) {
		case STATE_STARTING1:
			urb = playbacksubs->urb[0];
			atomic_inc(&playbacksubs->state);
			break;
		case STATE_STARTING2:
>>>>>>> upstream/android-13
			urb = playbacksubs->urb[1];
			atomic_inc(&playbacksubs->state);
			break;
		}
	}
	if (urb) {
<<<<<<< HEAD
		if ((err = usX2Y_hwdep_urb_play_prepare(playbacksubs, urb)) ||
		    (err = usX2Y_urb_submit(playbacksubs, urb, frame))) {
			return err;
		}
	}
	
	playbacksubs->completed_urb = NULL;

	state = atomic_read(&capsubs->state);
	if (state >= state_PREPARED) {
		if (state == state_RUNNING) {
			if ((err = usX2Y_usbpcm_urb_capt_retire(capsubs)))
				return err;
		} else if (state >= state_PRERUNNING)
			atomic_inc(&capsubs->state);
		usX2Y_usbpcm_urb_capt_iso_advance(capsubs, capsubs->completed_urb);
		if (NULL != capsubs2)
			usX2Y_usbpcm_urb_capt_iso_advance(NULL, capsubs2->completed_urb);
		if ((err = usX2Y_urb_submit(capsubs, capsubs->completed_urb, frame)))
			return err;
		if (NULL != capsubs2)
			if ((err = usX2Y_urb_submit(capsubs2, capsubs2->completed_urb, frame)))
				return err;
	}
	capsubs->completed_urb = NULL;
	if (NULL != capsubs2)
=======
		err = usx2y_hwdep_urb_play_prepare(playbacksubs, urb);
		if (err)
			return err;
		err = usx2y_hwdep_urb_play_prepare(playbacksubs, urb);
		if (err)
			return err;
	}

	playbacksubs->completed_urb = NULL;

	state = atomic_read(&capsubs->state);
	if (state >= STATE_PREPARED) {
		if (state == STATE_RUNNING) {
			err = usx2y_usbpcm_urb_capt_retire(capsubs);
			if (err)
				return err;
		} else if (state >= STATE_PRERUNNING) {
			atomic_inc(&capsubs->state);
		}
		usx2y_usbpcm_urb_capt_iso_advance(capsubs, capsubs->completed_urb);
		if (capsubs2)
			usx2y_usbpcm_urb_capt_iso_advance(NULL, capsubs2->completed_urb);
		err = usx2y_urb_submit(capsubs, capsubs->completed_urb, frame);
		if (err)
			return err;
		if (capsubs2) {
			err = usx2y_urb_submit(capsubs2, capsubs2->completed_urb, frame);
			if (err)
				return err;
		}
	}
	capsubs->completed_urb = NULL;
	if (capsubs2)
>>>>>>> upstream/android-13
		capsubs2->completed_urb = NULL;
	return 0;
}

<<<<<<< HEAD

static void i_usX2Y_usbpcm_urb_complete(struct urb *urb)
{
	struct snd_usX2Y_substream *subs = urb->context;
	struct usX2Ydev *usX2Y = subs->usX2Y;
	struct snd_usX2Y_substream *capsubs, *capsubs2, *playbacksubs;

	if (unlikely(atomic_read(&subs->state) < state_PREPARED)) {
		snd_printdd("hcd_frame=%i ep=%i%s status=%i start_frame=%i\n",
			    usb_get_current_frame_number(usX2Y->dev),
=======
static void i_usx2y_usbpcm_urb_complete(struct urb *urb)
{
	struct snd_usx2y_substream *subs = urb->context;
	struct usx2ydev *usx2y = subs->usx2y;
	struct snd_usx2y_substream *capsubs, *capsubs2, *playbacksubs;

	if (unlikely(atomic_read(&subs->state) < STATE_PREPARED)) {
		snd_printdd("hcd_frame=%i ep=%i%s status=%i start_frame=%i\n",
			    usb_get_current_frame_number(usx2y->dev),
>>>>>>> upstream/android-13
			    subs->endpoint, usb_pipein(urb->pipe) ? "in" : "out",
			    urb->status, urb->start_frame);
		return;
	}
	if (unlikely(urb->status)) {
<<<<<<< HEAD
		usX2Y_error_urb_status(usX2Y, subs, urb);
=======
		usx2y_error_urb_status(usx2y, subs, urb);
>>>>>>> upstream/android-13
		return;
	}

	subs->completed_urb = urb;
<<<<<<< HEAD
	capsubs = usX2Y->subs[SNDRV_PCM_STREAM_CAPTURE];
	capsubs2 = usX2Y->subs[SNDRV_PCM_STREAM_CAPTURE + 2];
	playbacksubs = usX2Y->subs[SNDRV_PCM_STREAM_PLAYBACK];
	if (capsubs->completed_urb && atomic_read(&capsubs->state) >= state_PREPARED &&
	    (NULL == capsubs2 || capsubs2->completed_urb) &&
	    (playbacksubs->completed_urb || atomic_read(&playbacksubs->state) < state_PREPARED)) {
		if (!usX2Y_usbpcm_usbframe_complete(capsubs, capsubs2, playbacksubs, urb->start_frame))
			usX2Y->wait_iso_frame += nr_of_packs();
		else {
			snd_printdd("\n");
			usX2Y_clients_stop(usX2Y);
=======
	capsubs = usx2y->subs[SNDRV_PCM_STREAM_CAPTURE];
	capsubs2 = usx2y->subs[SNDRV_PCM_STREAM_CAPTURE + 2];
	playbacksubs = usx2y->subs[SNDRV_PCM_STREAM_PLAYBACK];
	if (capsubs->completed_urb && atomic_read(&capsubs->state) >= STATE_PREPARED &&
	    (!capsubs2 || capsubs2->completed_urb) &&
	    (playbacksubs->completed_urb || atomic_read(&playbacksubs->state) < STATE_PREPARED)) {
		if (!usx2y_usbpcm_usbframe_complete(capsubs, capsubs2, playbacksubs, urb->start_frame)) {
			usx2y->wait_iso_frame += nr_of_packs();
		} else {
			snd_printdd("\n");
			usx2y_clients_stop(usx2y);
>>>>>>> upstream/android-13
		}
	}
}

<<<<<<< HEAD

static void usX2Y_hwdep_urb_release(struct urb **urb)
=======
static void usx2y_hwdep_urb_release(struct urb **urb)
>>>>>>> upstream/android-13
{
	usb_kill_urb(*urb);
	usb_free_urb(*urb);
	*urb = NULL;
}

/*
 * release a substream
 */
<<<<<<< HEAD
static void usX2Y_usbpcm_urbs_release(struct snd_usX2Y_substream *subs)
{
	int i;
	snd_printdd("snd_usX2Y_urbs_release() %i\n", subs->endpoint);
	for (i = 0; i < NRURBS; i++)
		usX2Y_hwdep_urb_release(subs->urb + i);
}

static void usX2Y_usbpcm_subs_startup_finish(struct usX2Ydev * usX2Y)
{
	usX2Y_urbs_set_complete(usX2Y, i_usX2Y_usbpcm_urb_complete);
	usX2Y->prepare_subs = NULL;
}

static void i_usX2Y_usbpcm_subs_startup(struct urb *urb)
{
	struct snd_usX2Y_substream *subs = urb->context;
	struct usX2Ydev *usX2Y = subs->usX2Y;
	struct snd_usX2Y_substream *prepare_subs = usX2Y->prepare_subs;
	if (NULL != prepare_subs &&
	    urb->start_frame == prepare_subs->urb[0]->start_frame) {
		atomic_inc(&prepare_subs->state);
		if (prepare_subs == usX2Y->subs[SNDRV_PCM_STREAM_CAPTURE]) {
			struct snd_usX2Y_substream *cap_subs2 = usX2Y->subs[SNDRV_PCM_STREAM_CAPTURE + 2];
			if (cap_subs2 != NULL)
				atomic_inc(&cap_subs2->state);
		}
		usX2Y_usbpcm_subs_startup_finish(usX2Y);
		wake_up(&usX2Y->prepare_wait_queue);
	}

	i_usX2Y_usbpcm_urb_complete(urb);
=======
static void usx2y_usbpcm_urbs_release(struct snd_usx2y_substream *subs)
{
	int i;

	snd_printdd("snd_usx2y_urbs_release() %i\n", subs->endpoint);
	for (i = 0; i < NRURBS; i++)
		usx2y_hwdep_urb_release(subs->urb + i);
}

static void usx2y_usbpcm_subs_startup_finish(struct usx2ydev *usx2y)
{
	usx2y_urbs_set_complete(usx2y, i_usx2y_usbpcm_urb_complete);
	usx2y->prepare_subs = NULL;
}

static void i_usx2y_usbpcm_subs_startup(struct urb *urb)
{
	struct snd_usx2y_substream *subs = urb->context;
	struct usx2ydev *usx2y = subs->usx2y;
	struct snd_usx2y_substream *prepare_subs = usx2y->prepare_subs;
	struct snd_usx2y_substream *cap_subs2;

	if (prepare_subs &&
	    urb->start_frame == prepare_subs->urb[0]->start_frame) {
		atomic_inc(&prepare_subs->state);
		if (prepare_subs == usx2y->subs[SNDRV_PCM_STREAM_CAPTURE]) {
			cap_subs2 = usx2y->subs[SNDRV_PCM_STREAM_CAPTURE + 2];
			if (cap_subs2)
				atomic_inc(&cap_subs2->state);
		}
		usx2y_usbpcm_subs_startup_finish(usx2y);
		wake_up(&usx2y->prepare_wait_queue);
	}

	i_usx2y_usbpcm_urb_complete(urb);
>>>>>>> upstream/android-13
}

/*
 * initialize a substream's urbs
 */
<<<<<<< HEAD
static int usX2Y_usbpcm_urbs_allocate(struct snd_usX2Y_substream *subs)
{
	int i;
	unsigned int pipe;
	int is_playback = subs == subs->usX2Y->subs[SNDRV_PCM_STREAM_PLAYBACK];
	struct usb_device *dev = subs->usX2Y->dev;
=======
static int usx2y_usbpcm_urbs_allocate(struct snd_usx2y_substream *subs)
{
	int i;
	unsigned int pipe;
	int is_playback = subs == subs->usx2y->subs[SNDRV_PCM_STREAM_PLAYBACK];
	struct usb_device *dev = subs->usx2y->dev;
	struct urb **purb;
>>>>>>> upstream/android-13

	pipe = is_playback ? usb_sndisocpipe(dev, subs->endpoint) :
			usb_rcvisocpipe(dev, subs->endpoint);
	subs->maxpacksize = usb_maxpacket(dev, pipe, is_playback);
	if (!subs->maxpacksize)
		return -EINVAL;

	/* allocate and initialize data urbs */
	for (i = 0; i < NRURBS; i++) {
<<<<<<< HEAD
		struct urb **purb = subs->urb + i;
=======
		purb = subs->urb + i;
>>>>>>> upstream/android-13
		if (*purb) {
			usb_kill_urb(*purb);
			continue;
		}
		*purb = usb_alloc_urb(nr_of_packs(), GFP_KERNEL);
<<<<<<< HEAD
		if (NULL == *purb) {
			usX2Y_usbpcm_urbs_release(subs);
			return -ENOMEM;
		}
		(*purb)->transfer_buffer = is_playback ?
			subs->usX2Y->hwdep_pcm_shm->playback : (
				subs->endpoint == 0x8 ?
				subs->usX2Y->hwdep_pcm_shm->capture0x8 :
				subs->usX2Y->hwdep_pcm_shm->capture0xA);
=======
		if (!*purb) {
			usx2y_usbpcm_urbs_release(subs);
			return -ENOMEM;
		}
		(*purb)->transfer_buffer = is_playback ?
			subs->usx2y->hwdep_pcm_shm->playback : (
				subs->endpoint == 0x8 ?
				subs->usx2y->hwdep_pcm_shm->capture0x8 :
				subs->usx2y->hwdep_pcm_shm->capture0xA);
>>>>>>> upstream/android-13

		(*purb)->dev = dev;
		(*purb)->pipe = pipe;
		(*purb)->number_of_packets = nr_of_packs();
		(*purb)->context = subs;
		(*purb)->interval = 1;
<<<<<<< HEAD
		(*purb)->complete = i_usX2Y_usbpcm_subs_startup;
=======
		(*purb)->complete = i_usx2y_usbpcm_subs_startup;
>>>>>>> upstream/android-13
	}
	return 0;
}

/*
 * free the buffer
 */
<<<<<<< HEAD
static int snd_usX2Y_usbpcm_hw_free(struct snd_pcm_substream *substream)
{
	struct snd_pcm_runtime *runtime = substream->runtime;
	struct snd_usX2Y_substream *subs = runtime->private_data,
		*cap_subs2 = subs->usX2Y->subs[SNDRV_PCM_STREAM_CAPTURE + 2];
	mutex_lock(&subs->usX2Y->pcm_mutex);
	snd_printdd("snd_usX2Y_usbpcm_hw_free(%p)\n", substream);

	if (SNDRV_PCM_STREAM_PLAYBACK == substream->stream) {
		struct snd_usX2Y_substream *cap_subs = subs->usX2Y->subs[SNDRV_PCM_STREAM_CAPTURE];
		atomic_set(&subs->state, state_STOPPED);
		usX2Y_usbpcm_urbs_release(subs);
=======
static int snd_usx2y_usbpcm_hw_free(struct snd_pcm_substream *substream)
{
	struct snd_pcm_runtime *runtime = substream->runtime;
	struct snd_usx2y_substream *subs = runtime->private_data;
	struct snd_usx2y_substream *cap_subs;
	struct snd_usx2y_substream *playback_subs;
	struct snd_usx2y_substream *cap_subs2;

	mutex_lock(&subs->usx2y->pcm_mutex);
	snd_printdd("%s(%p)\n", __func__, substream);

	cap_subs2 = subs->usx2y->subs[SNDRV_PCM_STREAM_CAPTURE + 2];
	if (substream->stream == SNDRV_PCM_STREAM_PLAYBACK) {
		cap_subs = subs->usx2y->subs[SNDRV_PCM_STREAM_CAPTURE];
		atomic_set(&subs->state, STATE_STOPPED);
		usx2y_usbpcm_urbs_release(subs);
>>>>>>> upstream/android-13
		if (!cap_subs->pcm_substream ||
		    !cap_subs->pcm_substream->runtime ||
		    !cap_subs->pcm_substream->runtime->status ||
		    cap_subs->pcm_substream->runtime->status->state < SNDRV_PCM_STATE_PREPARED) {
<<<<<<< HEAD
			atomic_set(&cap_subs->state, state_STOPPED);
			if (NULL != cap_subs2)
				atomic_set(&cap_subs2->state, state_STOPPED);
			usX2Y_usbpcm_urbs_release(cap_subs);
			if (NULL != cap_subs2)
				usX2Y_usbpcm_urbs_release(cap_subs2);
		}
	} else {
		struct snd_usX2Y_substream *playback_subs = subs->usX2Y->subs[SNDRV_PCM_STREAM_PLAYBACK];
		if (atomic_read(&playback_subs->state) < state_PREPARED) {
			atomic_set(&subs->state, state_STOPPED);
			if (NULL != cap_subs2)
				atomic_set(&cap_subs2->state, state_STOPPED);
			usX2Y_usbpcm_urbs_release(subs);
			if (NULL != cap_subs2)
				usX2Y_usbpcm_urbs_release(cap_subs2);
		}
	}
	mutex_unlock(&subs->usX2Y->pcm_mutex);
	return snd_pcm_lib_free_pages(substream);
}

static void usX2Y_usbpcm_subs_startup(struct snd_usX2Y_substream *subs)
{
	struct usX2Ydev * usX2Y = subs->usX2Y;
	usX2Y->prepare_subs = subs;
	subs->urb[0]->start_frame = -1;
	smp_wmb();	// Make sure above modifications are seen by i_usX2Y_subs_startup()
	usX2Y_urbs_set_complete(usX2Y, i_usX2Y_usbpcm_subs_startup);
}

static int usX2Y_usbpcm_urbs_start(struct snd_usX2Y_substream *subs)
{
	int	p, u, err,
		stream = subs->pcm_substream->stream;
	struct usX2Ydev *usX2Y = subs->usX2Y;

	if (SNDRV_PCM_STREAM_CAPTURE == stream) {
		usX2Y->hwdep_pcm_shm->captured_iso_head = -1;
		usX2Y->hwdep_pcm_shm->captured_iso_frames = 0;
	}

	for (p = 0; 3 >= (stream + p); p += 2) {
		struct snd_usX2Y_substream *subs = usX2Y->subs[stream + p];
		if (subs != NULL) {
			if ((err = usX2Y_usbpcm_urbs_allocate(subs)) < 0)
=======
			atomic_set(&cap_subs->state, STATE_STOPPED);
			if (cap_subs2)
				atomic_set(&cap_subs2->state, STATE_STOPPED);
			usx2y_usbpcm_urbs_release(cap_subs);
			if (cap_subs2)
				usx2y_usbpcm_urbs_release(cap_subs2);
		}
	} else {
		playback_subs = subs->usx2y->subs[SNDRV_PCM_STREAM_PLAYBACK];
		if (atomic_read(&playback_subs->state) < STATE_PREPARED) {
			atomic_set(&subs->state, STATE_STOPPED);
			if (cap_subs2)
				atomic_set(&cap_subs2->state, STATE_STOPPED);
			usx2y_usbpcm_urbs_release(subs);
			if (cap_subs2)
				usx2y_usbpcm_urbs_release(cap_subs2);
		}
	}
	mutex_unlock(&subs->usx2y->pcm_mutex);
	return 0;
}

static void usx2y_usbpcm_subs_startup(struct snd_usx2y_substream *subs)
{
	struct usx2ydev *usx2y = subs->usx2y;

	usx2y->prepare_subs = subs;
	subs->urb[0]->start_frame = -1;
	smp_wmb();	// Make sure above modifications are seen by i_usx2y_subs_startup()
	usx2y_urbs_set_complete(usx2y, i_usx2y_usbpcm_subs_startup);
}

static int usx2y_usbpcm_urbs_start(struct snd_usx2y_substream *subs)
{
	int	p, u, err, stream = subs->pcm_substream->stream;
	struct usx2ydev *usx2y = subs->usx2y;
	struct urb *urb;
	unsigned long pack;

	if (stream == SNDRV_PCM_STREAM_CAPTURE) {
		usx2y->hwdep_pcm_shm->captured_iso_head = -1;
		usx2y->hwdep_pcm_shm->captured_iso_frames = 0;
	}

	for (p = 0; 3 >= (stream + p); p += 2) {
		struct snd_usx2y_substream *subs = usx2y->subs[stream + p];
		if (subs) {
			err = usx2y_usbpcm_urbs_allocate(subs);
			if (err < 0)
>>>>>>> upstream/android-13
				return err;
			subs->completed_urb = NULL;
		}
	}

	for (p = 0; p < 4; p++) {
<<<<<<< HEAD
		struct snd_usX2Y_substream *subs = usX2Y->subs[p];
		if (subs != NULL && atomic_read(&subs->state) >= state_PREPARED)
=======
		struct snd_usx2y_substream *subs = usx2y->subs[p];

		if (subs && atomic_read(&subs->state) >= STATE_PREPARED)
>>>>>>> upstream/android-13
			goto start;
	}

 start:
<<<<<<< HEAD
	usX2Y_usbpcm_subs_startup(subs);
	for (u = 0; u < NRURBS; u++) {
		for (p = 0; 3 >= (stream + p); p += 2) {
			struct snd_usX2Y_substream *subs = usX2Y->subs[stream + p];
			if (subs != NULL) {
				struct urb *urb = subs->urb[u];
				if (usb_pipein(urb->pipe)) {
					unsigned long pack;
					if (0 == u)
						atomic_set(&subs->state, state_STARTING3);
					urb->dev = usX2Y->dev;
					for (pack = 0; pack < nr_of_packs(); pack++) {
						urb->iso_frame_desc[pack].offset = subs->maxpacksize * (pack + u * nr_of_packs());
						urb->iso_frame_desc[pack].length = subs->maxpacksize;
					}
					urb->transfer_buffer_length = subs->maxpacksize * nr_of_packs(); 
					if ((err = usb_submit_urb(urb, GFP_KERNEL)) < 0) {
						snd_printk (KERN_ERR "cannot usb_submit_urb() for urb %d, err = %d\n", u, err);
						err = -EPIPE;
						goto cleanup;
					}  else {
						snd_printdd("%i\n", urb->start_frame);
						if (u == 0)
							usX2Y->wait_iso_frame = urb->start_frame;
					}
					urb->transfer_flags = 0;
				} else {
					atomic_set(&subs->state, state_STARTING1);
					break;
				}			
=======
	usx2y_usbpcm_subs_startup(subs);
	for (u = 0; u < NRURBS; u++) {
		for (p = 0; 3 >= (stream + p); p += 2) {
			struct snd_usx2y_substream *subs = usx2y->subs[stream + p];

			if (!subs)
				continue;
			urb = subs->urb[u];
			if (usb_pipein(urb->pipe)) {
				if (!u)
					atomic_set(&subs->state, STATE_STARTING3);
				urb->dev = usx2y->dev;
				for (pack = 0; pack < nr_of_packs(); pack++) {
					urb->iso_frame_desc[pack].offset = subs->maxpacksize * (pack + u * nr_of_packs());
					urb->iso_frame_desc[pack].length = subs->maxpacksize;
				}
				urb->transfer_buffer_length = subs->maxpacksize * nr_of_packs();
				err = usb_submit_urb(urb, GFP_KERNEL);
				if (err < 0) {
					snd_printk(KERN_ERR "cannot usb_submit_urb() for urb %d, err = %d\n", u, err);
					err = -EPIPE;
					goto cleanup;
				}  else {
					snd_printdd("%i\n", urb->start_frame);
					if (!u)
						usx2y->wait_iso_frame = urb->start_frame;
				}
				urb->transfer_flags = 0;
			} else {
				atomic_set(&subs->state, STATE_STARTING1);
				break;
>>>>>>> upstream/android-13
			}
		}
	}
	err = 0;
<<<<<<< HEAD
	wait_event(usX2Y->prepare_wait_queue, NULL == usX2Y->prepare_subs);
	if (atomic_read(&subs->state) != state_PREPARED)
		err = -EPIPE;
		
 cleanup:
	if (err) {
		usX2Y_subs_startup_finish(usX2Y);	// Call it now
		usX2Y_clients_stop(usX2Y);		// something is completely wroong > stop evrything			
=======
	wait_event(usx2y->prepare_wait_queue, !usx2y->prepare_subs);
	if (atomic_read(&subs->state) != STATE_PREPARED)
		err = -EPIPE;

 cleanup:
	if (err) {
		usx2y_subs_startup_finish(usx2y);	// Call it now
		usx2y_clients_stop(usx2y);	// something is completely wrong > stop everything
>>>>>>> upstream/android-13
	}
	return err;
}

<<<<<<< HEAD
=======
#define USX2Y_HWDEP_PCM_PAGES	\
	PAGE_ALIGN(sizeof(struct snd_usx2y_hwdep_pcm_shm))

>>>>>>> upstream/android-13
/*
 * prepare callback
 *
 * set format and initialize urbs
 */
<<<<<<< HEAD
static int snd_usX2Y_usbpcm_prepare(struct snd_pcm_substream *substream)
{
	struct snd_pcm_runtime *runtime = substream->runtime;
	struct snd_usX2Y_substream *subs = runtime->private_data;
	struct usX2Ydev *usX2Y = subs->usX2Y;
	struct snd_usX2Y_substream *capsubs = subs->usX2Y->subs[SNDRV_PCM_STREAM_CAPTURE];
	int err = 0;
	snd_printdd("snd_usX2Y_pcm_prepare(%p)\n", substream);

	if (NULL == usX2Y->hwdep_pcm_shm) {
		if (NULL == (usX2Y->hwdep_pcm_shm = snd_malloc_pages(sizeof(struct snd_usX2Y_hwdep_pcm_shm), GFP_KERNEL)))
			return -ENOMEM;
		memset(usX2Y->hwdep_pcm_shm, 0, sizeof(struct snd_usX2Y_hwdep_pcm_shm));
	}

	mutex_lock(&usX2Y->pcm_mutex);
	usX2Y_subs_prepare(subs);
// Start hardware streams
// SyncStream first....
	if (atomic_read(&capsubs->state) < state_PREPARED) {
		if (usX2Y->format != runtime->format)
			if ((err = usX2Y_format_set(usX2Y, runtime->format)) < 0)
				goto up_prepare_mutex;
		if (usX2Y->rate != runtime->rate)
			if ((err = usX2Y_rate_set(usX2Y, runtime->rate)) < 0)
				goto up_prepare_mutex;
		snd_printdd("starting capture pipe for %s\n", subs == capsubs ?
			    "self" : "playpipe");
		if (0 > (err = usX2Y_usbpcm_urbs_start(capsubs)))
=======
static int snd_usx2y_usbpcm_prepare(struct snd_pcm_substream *substream)
{
	struct snd_pcm_runtime *runtime = substream->runtime;
	struct snd_usx2y_substream *subs = runtime->private_data;
	struct usx2ydev *usx2y = subs->usx2y;
	struct snd_usx2y_substream *capsubs = subs->usx2y->subs[SNDRV_PCM_STREAM_CAPTURE];
	int err = 0;

	snd_printdd("snd_usx2y_pcm_prepare(%p)\n", substream);

	mutex_lock(&usx2y->pcm_mutex);

	if (!usx2y->hwdep_pcm_shm) {
		usx2y->hwdep_pcm_shm = alloc_pages_exact(USX2Y_HWDEP_PCM_PAGES,
							 GFP_KERNEL);
		if (!usx2y->hwdep_pcm_shm) {
			err = -ENOMEM;
			goto up_prepare_mutex;
		}
		memset(usx2y->hwdep_pcm_shm, 0, USX2Y_HWDEP_PCM_PAGES);
	}

	usx2y_subs_prepare(subs);
	// Start hardware streams
	// SyncStream first....
	if (atomic_read(&capsubs->state) < STATE_PREPARED) {
		if (usx2y->format != runtime->format) {
			err = usx2y_format_set(usx2y, runtime->format);
			if (err < 0)
				goto up_prepare_mutex;
		}
		if (usx2y->rate != runtime->rate) {
			err = usx2y_rate_set(usx2y, runtime->rate);
			if (err < 0)
				goto up_prepare_mutex;
		}
		snd_printdd("starting capture pipe for %s\n", subs == capsubs ?
			    "self" : "playpipe");
		err = usx2y_usbpcm_urbs_start(capsubs);
		if (err < 0)
>>>>>>> upstream/android-13
			goto up_prepare_mutex;
	}

	if (subs != capsubs) {
<<<<<<< HEAD
		usX2Y->hwdep_pcm_shm->playback_iso_start = -1;
		if (atomic_read(&subs->state) < state_PREPARED) {
			while (usX2Y_iso_frames_per_buffer(runtime, usX2Y) >
			       usX2Y->hwdep_pcm_shm->captured_iso_frames) {
				snd_printdd("Wait: iso_frames_per_buffer=%i,"
					    "captured_iso_frames=%i\n",
					    usX2Y_iso_frames_per_buffer(runtime, usX2Y),
					    usX2Y->hwdep_pcm_shm->captured_iso_frames);
=======
		usx2y->hwdep_pcm_shm->playback_iso_start = -1;
		if (atomic_read(&subs->state) < STATE_PREPARED) {
			while (usx2y_iso_frames_per_buffer(runtime, usx2y) >
			       usx2y->hwdep_pcm_shm->captured_iso_frames) {
				snd_printdd("Wait: iso_frames_per_buffer=%i,captured_iso_frames=%i\n",
					    usx2y_iso_frames_per_buffer(runtime, usx2y),
					    usx2y->hwdep_pcm_shm->captured_iso_frames);
>>>>>>> upstream/android-13
				if (msleep_interruptible(10)) {
					err = -ERESTARTSYS;
					goto up_prepare_mutex;
				}
<<<<<<< HEAD
			} 
			if (0 > (err = usX2Y_usbpcm_urbs_start(subs)))
				goto up_prepare_mutex;
		}
		snd_printdd("Ready: iso_frames_per_buffer=%i,captured_iso_frames=%i\n",
			    usX2Y_iso_frames_per_buffer(runtime, usX2Y),
			    usX2Y->hwdep_pcm_shm->captured_iso_frames);
	} else
		usX2Y->hwdep_pcm_shm->capture_iso_start = -1;

 up_prepare_mutex:
	mutex_unlock(&usX2Y->pcm_mutex);
	return err;
}

static struct snd_pcm_hardware snd_usX2Y_4c =
{
=======
			}
			err = usx2y_usbpcm_urbs_start(subs);
			if (err < 0)
				goto up_prepare_mutex;
		}
		snd_printdd("Ready: iso_frames_per_buffer=%i,captured_iso_frames=%i\n",
			    usx2y_iso_frames_per_buffer(runtime, usx2y),
			    usx2y->hwdep_pcm_shm->captured_iso_frames);
	} else {
		usx2y->hwdep_pcm_shm->capture_iso_start = -1;
	}

 up_prepare_mutex:
	mutex_unlock(&usx2y->pcm_mutex);
	return err;
}

static const struct snd_pcm_hardware snd_usx2y_4c = {
>>>>>>> upstream/android-13
	.info =			(SNDRV_PCM_INFO_MMAP | SNDRV_PCM_INFO_INTERLEAVED |
				 SNDRV_PCM_INFO_BLOCK_TRANSFER |
				 SNDRV_PCM_INFO_MMAP_VALID),
	.formats =                 SNDRV_PCM_FMTBIT_S16_LE | SNDRV_PCM_FMTBIT_S24_3LE,
	.rates =                   SNDRV_PCM_RATE_44100 | SNDRV_PCM_RATE_48000,
	.rate_min =                44100,
	.rate_max =                48000,
	.channels_min =            2,
	.channels_max =            4,
	.buffer_bytes_max =	(2*128*1024),
	.period_bytes_min =	64,
	.period_bytes_max =	(128*1024),
	.periods_min =		2,
	.periods_max =		1024,
	.fifo_size =              0
};

<<<<<<< HEAD


static int snd_usX2Y_usbpcm_open(struct snd_pcm_substream *substream)
{
	struct snd_usX2Y_substream	*subs = ((struct snd_usX2Y_substream **)
					 snd_pcm_substream_chip(substream))[substream->stream];
	struct snd_pcm_runtime	*runtime = substream->runtime;

	if (!(subs->usX2Y->chip_status & USX2Y_STAT_CHIP_MMAP_PCM_URBS))
		return -EBUSY;

	runtime->hw = SNDRV_PCM_STREAM_PLAYBACK == substream->stream ? snd_usX2Y_2c :
		(subs->usX2Y->subs[3] ? snd_usX2Y_4c : snd_usX2Y_2c);
=======
static int snd_usx2y_usbpcm_open(struct snd_pcm_substream *substream)
{
	struct snd_usx2y_substream	*subs =
		((struct snd_usx2y_substream **)
		 snd_pcm_substream_chip(substream))[substream->stream];
	struct snd_pcm_runtime	*runtime = substream->runtime;

	if (!(subs->usx2y->chip_status & USX2Y_STAT_CHIP_MMAP_PCM_URBS))
		return -EBUSY;

	if (substream->stream == SNDRV_PCM_STREAM_PLAYBACK)
		runtime->hw = snd_usx2y_2c;
	else
		runtime->hw = (subs->usx2y->subs[3] ? snd_usx2y_4c : snd_usx2y_2c);
>>>>>>> upstream/android-13
	runtime->private_data = subs;
	subs->pcm_substream = substream;
	snd_pcm_hw_constraint_minmax(runtime, SNDRV_PCM_HW_PARAM_PERIOD_TIME, 1000, 200000);
	return 0;
}

<<<<<<< HEAD

static int snd_usX2Y_usbpcm_close(struct snd_pcm_substream *substream)
{
	struct snd_pcm_runtime *runtime = substream->runtime;
	struct snd_usX2Y_substream *subs = runtime->private_data;
=======
static int snd_usx2y_usbpcm_close(struct snd_pcm_substream *substream)
{
	struct snd_pcm_runtime *runtime = substream->runtime;
	struct snd_usx2y_substream *subs = runtime->private_data;
>>>>>>> upstream/android-13

	subs->pcm_substream = NULL;
	return 0;
}

<<<<<<< HEAD

static const struct snd_pcm_ops snd_usX2Y_usbpcm_ops =
{
	.open =		snd_usX2Y_usbpcm_open,
	.close =	snd_usX2Y_usbpcm_close,
	.ioctl =	snd_pcm_lib_ioctl,
	.hw_params =	snd_usX2Y_pcm_hw_params,
	.hw_free =	snd_usX2Y_usbpcm_hw_free,
	.prepare =	snd_usX2Y_usbpcm_prepare,
	.trigger =	snd_usX2Y_pcm_trigger,
	.pointer =	snd_usX2Y_pcm_pointer,
};


static int usX2Y_pcms_busy_check(struct snd_card *card)
{
	struct usX2Ydev	*dev = usX2Y(card);
	int i;

	for (i = 0; i < dev->pcm_devs * 2; i++) {
		struct snd_usX2Y_substream *subs = dev->subs[i];
=======
static const struct snd_pcm_ops snd_usx2y_usbpcm_ops = {
	.open =		snd_usx2y_usbpcm_open,
	.close =	snd_usx2y_usbpcm_close,
	.hw_params =	snd_usx2y_pcm_hw_params,
	.hw_free =	snd_usx2y_usbpcm_hw_free,
	.prepare =	snd_usx2y_usbpcm_prepare,
	.trigger =	snd_usx2y_pcm_trigger,
	.pointer =	snd_usx2y_pcm_pointer,
};

static int usx2y_pcms_busy_check(struct snd_card *card)
{
	struct usx2ydev	*dev = usx2y(card);
	struct snd_usx2y_substream *subs;
	int i;

	for (i = 0; i < dev->pcm_devs * 2; i++) {
		subs = dev->subs[i];
>>>>>>> upstream/android-13
		if (subs && subs->pcm_substream &&
		    SUBSTREAM_BUSY(subs->pcm_substream))
			return -EBUSY;
	}
	return 0;
}

<<<<<<< HEAD
static int snd_usX2Y_hwdep_pcm_open(struct snd_hwdep *hw, struct file *file)
=======
static int snd_usx2y_hwdep_pcm_open(struct snd_hwdep *hw, struct file *file)
>>>>>>> upstream/android-13
{
	struct snd_card *card = hw->card;
	int err;

<<<<<<< HEAD
	mutex_lock(&usX2Y(card)->pcm_mutex);
	err = usX2Y_pcms_busy_check(card);
	if (!err)
		usX2Y(card)->chip_status |= USX2Y_STAT_CHIP_MMAP_PCM_URBS;
	mutex_unlock(&usX2Y(card)->pcm_mutex);
	return err;
}


static int snd_usX2Y_hwdep_pcm_release(struct snd_hwdep *hw, struct file *file)
=======
	mutex_lock(&usx2y(card)->pcm_mutex);
	err = usx2y_pcms_busy_check(card);
	if (!err)
		usx2y(card)->chip_status |= USX2Y_STAT_CHIP_MMAP_PCM_URBS;
	mutex_unlock(&usx2y(card)->pcm_mutex);
	return err;
}

static int snd_usx2y_hwdep_pcm_release(struct snd_hwdep *hw, struct file *file)
>>>>>>> upstream/android-13
{
	struct snd_card *card = hw->card;
	int err;

<<<<<<< HEAD
	mutex_lock(&usX2Y(card)->pcm_mutex);
	err = usX2Y_pcms_busy_check(card);
	if (!err)
		usX2Y(hw->card)->chip_status &= ~USX2Y_STAT_CHIP_MMAP_PCM_URBS;
	mutex_unlock(&usX2Y(card)->pcm_mutex);
	return err;
}


static void snd_usX2Y_hwdep_pcm_vm_open(struct vm_area_struct *area)
{
}


static void snd_usX2Y_hwdep_pcm_vm_close(struct vm_area_struct *area)
{
}


static vm_fault_t snd_usX2Y_hwdep_pcm_vm_fault(struct vm_fault *vmf)
=======
	mutex_lock(&usx2y(card)->pcm_mutex);
	err = usx2y_pcms_busy_check(card);
	if (!err)
		usx2y(hw->card)->chip_status &= ~USX2Y_STAT_CHIP_MMAP_PCM_URBS;
	mutex_unlock(&usx2y(card)->pcm_mutex);
	return err;
}

static void snd_usx2y_hwdep_pcm_vm_open(struct vm_area_struct *area)
{
}

static void snd_usx2y_hwdep_pcm_vm_close(struct vm_area_struct *area)
{
}

static vm_fault_t snd_usx2y_hwdep_pcm_vm_fault(struct vm_fault *vmf)
>>>>>>> upstream/android-13
{
	unsigned long offset;
	void *vaddr;

	offset = vmf->pgoff << PAGE_SHIFT;
<<<<<<< HEAD
	vaddr = (char *)((struct usX2Ydev *)vmf->vma->vm_private_data)->hwdep_pcm_shm + offset;
=======
	vaddr = (char *)((struct usx2ydev *)vmf->vma->vm_private_data)->hwdep_pcm_shm + offset;
>>>>>>> upstream/android-13
	vmf->page = virt_to_page(vaddr);
	get_page(vmf->page);
	return 0;
}

<<<<<<< HEAD

static const struct vm_operations_struct snd_usX2Y_hwdep_pcm_vm_ops = {
	.open = snd_usX2Y_hwdep_pcm_vm_open,
	.close = snd_usX2Y_hwdep_pcm_vm_close,
	.fault = snd_usX2Y_hwdep_pcm_vm_fault,
};


static int snd_usX2Y_hwdep_pcm_mmap(struct snd_hwdep * hw, struct file *filp, struct vm_area_struct *area)
{
	unsigned long	size = (unsigned long)(area->vm_end - area->vm_start);
	struct usX2Ydev	*usX2Y = hw->private_data;

	if (!(usX2Y->chip_status & USX2Y_STAT_CHIP_INIT))
		return -EBUSY;

	/* if userspace tries to mmap beyond end of our buffer, fail */ 
	if (size > PAGE_ALIGN(sizeof(struct snd_usX2Y_hwdep_pcm_shm))) {
		snd_printd("%lu > %lu\n", size, (unsigned long)sizeof(struct snd_usX2Y_hwdep_pcm_shm)); 
		return -EINVAL;
	}

	if (!usX2Y->hwdep_pcm_shm) {
		return -ENODEV;
	}
	area->vm_ops = &snd_usX2Y_hwdep_pcm_vm_ops;
=======
static const struct vm_operations_struct snd_usx2y_hwdep_pcm_vm_ops = {
	.open = snd_usx2y_hwdep_pcm_vm_open,
	.close = snd_usx2y_hwdep_pcm_vm_close,
	.fault = snd_usx2y_hwdep_pcm_vm_fault,
};

static int snd_usx2y_hwdep_pcm_mmap(struct snd_hwdep *hw, struct file *filp, struct vm_area_struct *area)
{
	unsigned long	size = (unsigned long)(area->vm_end - area->vm_start);
	struct usx2ydev	*usx2y = hw->private_data;

	if (!(usx2y->chip_status & USX2Y_STAT_CHIP_INIT))
		return -EBUSY;

	/* if userspace tries to mmap beyond end of our buffer, fail */
	if (size > USX2Y_HWDEP_PCM_PAGES) {
		snd_printd("%lu > %lu\n", size, (unsigned long)USX2Y_HWDEP_PCM_PAGES);
		return -EINVAL;
	}

	if (!usx2y->hwdep_pcm_shm)
		return -ENODEV;

	area->vm_ops = &snd_usx2y_hwdep_pcm_vm_ops;
>>>>>>> upstream/android-13
	area->vm_flags |= VM_DONTEXPAND | VM_DONTDUMP;
	area->vm_private_data = hw->private_data;
	return 0;
}

<<<<<<< HEAD

static void snd_usX2Y_hwdep_pcm_private_free(struct snd_hwdep *hwdep)
{
	struct usX2Ydev *usX2Y = hwdep->private_data;
	if (NULL != usX2Y->hwdep_pcm_shm)
		snd_free_pages(usX2Y->hwdep_pcm_shm, sizeof(struct snd_usX2Y_hwdep_pcm_shm));
}


int usX2Y_hwdep_pcm_new(struct snd_card *card)
=======
static void snd_usx2y_hwdep_pcm_private_free(struct snd_hwdep *hwdep)
{
	struct usx2ydev *usx2y = hwdep->private_data;

	if (usx2y->hwdep_pcm_shm)
		free_pages_exact(usx2y->hwdep_pcm_shm, USX2Y_HWDEP_PCM_PAGES);
}

int usx2y_hwdep_pcm_new(struct snd_card *card)
>>>>>>> upstream/android-13
{
	int err;
	struct snd_hwdep *hw;
	struct snd_pcm *pcm;
<<<<<<< HEAD
	struct usb_device *dev = usX2Y(card)->dev;
	if (1 != nr_of_packs())
		return 0;

	if ((err = snd_hwdep_new(card, SND_USX2Y_USBPCM_ID, 1, &hw)) < 0)
		return err;

	hw->iface = SNDRV_HWDEP_IFACE_USX2Y_PCM;
	hw->private_data = usX2Y(card);
	hw->private_free = snd_usX2Y_hwdep_pcm_private_free;
	hw->ops.open = snd_usX2Y_hwdep_pcm_open;
	hw->ops.release = snd_usX2Y_hwdep_pcm_release;
	hw->ops.mmap = snd_usX2Y_hwdep_pcm_mmap;
=======
	struct usb_device *dev = usx2y(card)->dev;

	if (nr_of_packs() != 1)
		return 0;

	err = snd_hwdep_new(card, SND_USX2Y_USBPCM_ID, 1, &hw);
	if (err < 0)
		return err;

	hw->iface = SNDRV_HWDEP_IFACE_USX2Y_PCM;
	hw->private_data = usx2y(card);
	hw->private_free = snd_usx2y_hwdep_pcm_private_free;
	hw->ops.open = snd_usx2y_hwdep_pcm_open;
	hw->ops.release = snd_usx2y_hwdep_pcm_release;
	hw->ops.mmap = snd_usx2y_hwdep_pcm_mmap;
>>>>>>> upstream/android-13
	hw->exclusive = 1;
	sprintf(hw->name, "/dev/bus/usb/%03d/%03d/hwdeppcm", dev->bus->busnum, dev->devnum);

	err = snd_pcm_new(card, NAME_ALLCAPS" hwdep Audio", 2, 1, 1, &pcm);
<<<<<<< HEAD
	if (err < 0) {
		return err;
	}
	snd_pcm_set_ops(pcm, SNDRV_PCM_STREAM_PLAYBACK, &snd_usX2Y_usbpcm_ops);
	snd_pcm_set_ops(pcm, SNDRV_PCM_STREAM_CAPTURE, &snd_usX2Y_usbpcm_ops);

	pcm->private_data = usX2Y(card)->subs;
	pcm->info_flags = 0;

	sprintf(pcm->name, NAME_ALLCAPS" hwdep Audio");
	if (0 > (err = snd_pcm_lib_preallocate_pages(pcm->streams[SNDRV_PCM_STREAM_PLAYBACK].substream,
						     SNDRV_DMA_TYPE_CONTINUOUS,
						     snd_dma_continuous_data(GFP_KERNEL),
						     64*1024, 128*1024)) ||
	    0 > (err = snd_pcm_lib_preallocate_pages(pcm->streams[SNDRV_PCM_STREAM_CAPTURE].substream,
	    					     SNDRV_DMA_TYPE_CONTINUOUS,
	    					     snd_dma_continuous_data(GFP_KERNEL),
						     64*1024, 128*1024))) {
		return err;
	}

=======
	if (err < 0)
		return err;

	snd_pcm_set_ops(pcm, SNDRV_PCM_STREAM_PLAYBACK, &snd_usx2y_usbpcm_ops);
	snd_pcm_set_ops(pcm, SNDRV_PCM_STREAM_CAPTURE, &snd_usx2y_usbpcm_ops);

	pcm->private_data = usx2y(card)->subs;
	pcm->info_flags = 0;

	sprintf(pcm->name, NAME_ALLCAPS" hwdep Audio");
	snd_pcm_set_managed_buffer(pcm->streams[SNDRV_PCM_STREAM_PLAYBACK].substream,
				   SNDRV_DMA_TYPE_CONTINUOUS,
				   NULL,
				   64*1024, 128*1024);
	snd_pcm_set_managed_buffer(pcm->streams[SNDRV_PCM_STREAM_CAPTURE].substream,
				   SNDRV_DMA_TYPE_CONTINUOUS,
				   NULL,
				   64*1024, 128*1024);
>>>>>>> upstream/android-13

	return 0;
}

#else

<<<<<<< HEAD
int usX2Y_hwdep_pcm_new(struct snd_card *card)
=======
int usx2y_hwdep_pcm_new(struct snd_card *card)
>>>>>>> upstream/android-13
{
	return 0;
}

#endif
