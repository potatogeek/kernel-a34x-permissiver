/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __USBAUDIO_PCM_H
#define __USBAUDIO_PCM_H

<<<<<<< HEAD
snd_pcm_uframes_t snd_usb_pcm_delay(struct snd_usb_substream *subs,
				    unsigned int rate);

=======
>>>>>>> upstream/android-13
void snd_usb_set_pcm_ops(struct snd_pcm *pcm, int stream);
int snd_usb_pcm_suspend(struct snd_usb_stream *as);
int snd_usb_pcm_resume(struct snd_usb_stream *as);

<<<<<<< HEAD
int snd_usb_init_pitch(struct snd_usb_audio *chip, int iface,
		       struct usb_host_interface *alts,
		       struct audioformat *fmt);
void snd_usb_preallocate_buffer(struct snd_usb_substream *subs);
int snd_usb_enable_audio_stream(struct snd_usb_substream *subs,
	int datainterval, bool enable);
=======
int snd_usb_init_pitch(struct snd_usb_audio *chip,
		       const struct audioformat *fmt);
void snd_usb_preallocate_buffer(struct snd_usb_substream *subs);

int snd_usb_audioformat_set_sync_ep(struct snd_usb_audio *chip,
				    struct audioformat *fmt);

>>>>>>> upstream/android-13
#endif /* __USBAUDIO_PCM_H */
