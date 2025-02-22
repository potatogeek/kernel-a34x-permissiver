/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __USBAUDIO_CLOCK_H
#define __USBAUDIO_CLOCK_H

<<<<<<< HEAD
int snd_usb_init_sample_rate(struct snd_usb_audio *chip, int iface,
			     struct usb_host_interface *alts,
			     struct audioformat *fmt, int rate);

int snd_usb_clock_find_source(struct snd_usb_audio *chip,
			      struct audioformat *fmt, bool validate);
=======
int snd_usb_init_sample_rate(struct snd_usb_audio *chip,
			     const struct audioformat *fmt, int rate);

int snd_usb_clock_find_source(struct snd_usb_audio *chip,
			      const struct audioformat *fmt, bool validate);

int snd_usb_set_sample_rate_v2v3(struct snd_usb_audio *chip,
				 const struct audioformat *fmt,
				 int clock, int rate);
>>>>>>> upstream/android-13

#endif /* __USBAUDIO_CLOCK_H */
