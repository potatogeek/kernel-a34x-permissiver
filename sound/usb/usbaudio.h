<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-or-later */
>>>>>>> upstream/android-13
#ifndef __USBAUDIO_H
#define __USBAUDIO_H
/*
 *   (Tentative) USB Audio Driver for ALSA
 *
 *   Copyright (c) 2002 by Takashi Iwai <tiwai@suse.de>
<<<<<<< HEAD
 *
 *
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
 */

/* handling of USB vendor/product ID pairs as 32-bit numbers */
#define USB_ID(vendor, product) (((vendor) << 16) | (product))
#define USB_ID_VENDOR(id) ((id) >> 16)
#define USB_ID_PRODUCT(id) ((u16)(id))

=======
 */

/* handling of USB vendor/product ID pairs as 32-bit numbers */
#define USB_ID(vendor, product) (((unsigned int)(vendor) << 16) | (product))
#define USB_ID_VENDOR(id) ((id) >> 16)
#define USB_ID_PRODUCT(id) ((u16)(id))

#include <linux/android_kabi.h>

>>>>>>> upstream/android-13
/*
 *
 */

<<<<<<< HEAD
=======
struct media_device;
struct media_intf_devnode;

#define MAX_CARD_INTERFACES	16

>>>>>>> upstream/android-13
struct snd_usb_audio {
	int index;
	struct usb_device *dev;
	struct snd_card *card;
<<<<<<< HEAD
	struct usb_interface *pm_intf;
	u32 usb_id;
=======
	struct usb_interface *intf[MAX_CARD_INTERFACES];
	u32 usb_id;
	uint16_t quirk_type;
>>>>>>> upstream/android-13
	struct mutex mutex;
	unsigned int system_suspend;
	atomic_t active;
	atomic_t shutdown;
	atomic_t usage_count;
	wait_queue_head_t shutdown_wait;
<<<<<<< HEAD
	unsigned int txfr_quirk:1; /* Subframe boundaries on transfers */
	unsigned int tx_length_quirk:1; /* Put length specifier in transfers */
	unsigned int setup_fmt_after_resume_quirk:1; /* setup the format to interface after resume */
=======
	unsigned int quirk_flags;
	unsigned int need_delayed_register:1; /* warn for delayed registration */
>>>>>>> upstream/android-13
	int num_interfaces;
	int num_suspended_intf;
	int sample_rate_read_error;

	int badd_profile;		/* UAC3 BADD profile */

	struct list_head pcm_list;	/* list of pcm streams */
	struct list_head ep_list;	/* list of audio-related endpoints */
<<<<<<< HEAD
=======
	struct list_head iface_ref_list; /* list of interface refcounts */
>>>>>>> upstream/android-13
	int pcm_devs;

	struct list_head midi_list;	/* list of midi interfaces */

	struct list_head mixer_list;	/* list of mixer interfaces */

	int setup;			/* from the 'device_setup' module param */
<<<<<<< HEAD
	bool autoclock;			/* from the 'autoclock' module param */
	bool keep_iface;		/* keep interface/altset after closing
					 * or parameter change
					 */

	struct usb_host_interface *ctrl_intf;	/* the audio control interface */
	struct mutex dev_lock;  /* to protect any race with disconnect */
	int card_num;	/* cache pcm card number to use upon disconnect */
	void (*disconnect_cb)(struct snd_usb_audio *chip); /* callback to cleanup on disconnect */
=======
	bool generic_implicit_fb;	/* from the 'implicit_fb' module param */
	bool autoclock;			/* from the 'autoclock' module param */

	bool lowlatency;		/* from the 'lowlatency' module param */
	struct usb_host_interface *ctrl_intf;	/* the audio control interface */
	struct media_device *media_dev;
	struct media_intf_devnode *ctl_intf_media_devnode;

	ANDROID_KABI_RESERVE(1);
	ANDROID_KABI_RESERVE(2);
	ANDROID_KABI_RESERVE(3);
	ANDROID_KABI_RESERVE(4);
>>>>>>> upstream/android-13
};

#define USB_AUDIO_IFACE_UNUSED	((void *)-1L)

#define usb_audio_err(chip, fmt, args...) \
	dev_err(&(chip)->dev->dev, fmt, ##args)
#define usb_audio_warn(chip, fmt, args...) \
	dev_warn(&(chip)->dev->dev, fmt, ##args)
#define usb_audio_info(chip, fmt, args...) \
	dev_info(&(chip)->dev->dev, fmt, ##args)
#define usb_audio_dbg(chip, fmt, args...) \
	dev_dbg(&(chip)->dev->dev, fmt, ##args)

<<<<<<< HEAD
#define usb_audio_err_ratelimited(chip, fmt, args...) \
	dev_info_ratelimited(&(chip)->dev->dev, fmt, ##args)
#define usb_audio_info_ratelimited(chip, fmt, args...) \
	dev_info_ratelimited(&(chip)->dev->dev, fmt, ##args)
#define usb_audio_dbg_ratelimited(chip, fmt, args...) \
	dev_dbg_ratelimited(&(chip)->dev->dev, fmt, ##args)
=======
>>>>>>> upstream/android-13
/*
 * Information about devices with broken descriptors
 */

/* special values for .ifnum */
<<<<<<< HEAD
=======
#define QUIRK_NODEV_INTERFACE		-3	/* return -ENODEV */
>>>>>>> upstream/android-13
#define QUIRK_NO_INTERFACE		-2
#define QUIRK_ANY_INTERFACE		-1

enum quirk_type {
	QUIRK_IGNORE_INTERFACE,
	QUIRK_COMPOSITE,
	QUIRK_AUTODETECT,
	QUIRK_MIDI_STANDARD_INTERFACE,
	QUIRK_MIDI_FIXED_ENDPOINT,
	QUIRK_MIDI_YAMAHA,
	QUIRK_MIDI_ROLAND,
	QUIRK_MIDI_MIDIMAN,
	QUIRK_MIDI_NOVATION,
	QUIRK_MIDI_RAW_BYTES,
	QUIRK_MIDI_EMAGIC,
	QUIRK_MIDI_CME,
	QUIRK_MIDI_AKAI,
	QUIRK_MIDI_US122L,
	QUIRK_MIDI_FTDI,
	QUIRK_MIDI_CH345,
	QUIRK_AUDIO_STANDARD_INTERFACE,
	QUIRK_AUDIO_FIXED_ENDPOINT,
	QUIRK_AUDIO_EDIROL_UAXX,
<<<<<<< HEAD
	QUIRK_AUDIO_ALIGN_TRANSFER,
	QUIRK_AUDIO_STANDARD_MIXER,
	QUIRK_SETUP_FMT_AFTER_RESUME,
=======
	QUIRK_AUDIO_STANDARD_MIXER,
>>>>>>> upstream/android-13

	QUIRK_TYPE_COUNT
};

struct snd_usb_audio_quirk {
	const char *vendor_name;
	const char *product_name;
<<<<<<< HEAD
	const char *profile_name;	/* override the card->longname */
=======
>>>>>>> upstream/android-13
	int16_t ifnum;
	uint16_t type;
	const void *data;
};

#define combine_word(s)    ((*(s)) | ((unsigned int)(s)[1] << 8))
#define combine_triple(s)  (combine_word(s) | ((unsigned int)(s)[2] << 16))
#define combine_quad(s)    (combine_triple(s) | ((unsigned int)(s)[3] << 24))

int snd_usb_lock_shutdown(struct snd_usb_audio *chip);
void snd_usb_unlock_shutdown(struct snd_usb_audio *chip);

extern bool snd_usb_use_vmalloc;
<<<<<<< HEAD
=======
extern bool snd_usb_skip_validation;

/*
 * Driver behavior quirk flags, stored in chip->quirk_flags
 *
 * QUIRK_FLAG_GET_SAMPLE_RATE:
 *  Skip reading sample rate for devices, as some devices behave inconsistently
 *  or return error
 * QUIRK_FLAG_SHARE_MEDIA_DEVICE:
 *  Create Media Controller API entries
 * QUIRK_FLAG_ALIGN_TRANSFER:
 *  Allow alignment on audio sub-slot (channel samples) rather than on audio
 *  slots (audio frames)
 * QUIRK_TX_LENGTH:
 *  Add length specifier to transfers
 * QUIRK_FLAG_PLAYBACK_FIRST:
 *  Start playback stream at first even in implement feedback mode
 * QUIRK_FLAG_SKIP_CLOCK_SELECTOR:
 *  Skip clock selector setup; the device may reset to invalid state
 * QUIRK_FLAG_IGNORE_CLOCK_SOURCE:
 *  Ignore errors from clock source search; i.e. hardcoded clock
 * QUIRK_FLAG_ITF_USB_DSD_DAC:
 *  Indicates the device is for ITF-USB DSD based DACs that need a vendor cmd
 *  to switch between PCM and native DSD mode
 * QUIRK_FLAG_CTL_MSG_DELAY:
 *  Add a delay of 20ms at each control message handling
 * QUIRK_FLAG_CTL_MSG_DELAY_1M:
 *  Add a delay of 1-2ms at each control message handling
 * QUIRK_FLAG_CTL_MSG_DELAY_5M:
 *  Add a delay of 5-6ms at each control message handling
 * QUIRK_FLAG_IFACE_DELAY:
 *  Add a delay of 50ms at each interface setup
 * QUIRK_FLAG_VALIDATE_RATES:
 *  Perform sample rate validations at probe
 * QUIRK_FLAG_DISABLE_AUTOSUSPEND:
 *  Disable runtime PM autosuspend
 * QUIRK_FLAG_IGNORE_CTL_ERROR:
 *  Ignore errors for mixer access
 * QUIRK_FLAG_DSD_RAW:
 *  Support generic DSD raw U32_BE format
 * QUIRK_FLAG_SET_IFACE_FIRST:
 *  Set up the interface at first like UAC1
 */

#define QUIRK_FLAG_GET_SAMPLE_RATE	(1U << 0)
#define QUIRK_FLAG_SHARE_MEDIA_DEVICE	(1U << 1)
#define QUIRK_FLAG_ALIGN_TRANSFER	(1U << 2)
#define QUIRK_FLAG_TX_LENGTH		(1U << 3)
#define QUIRK_FLAG_PLAYBACK_FIRST	(1U << 4)
#define QUIRK_FLAG_SKIP_CLOCK_SELECTOR	(1U << 5)
#define QUIRK_FLAG_IGNORE_CLOCK_SOURCE	(1U << 6)
#define QUIRK_FLAG_ITF_USB_DSD_DAC	(1U << 7)
#define QUIRK_FLAG_CTL_MSG_DELAY	(1U << 8)
#define QUIRK_FLAG_CTL_MSG_DELAY_1M	(1U << 9)
#define QUIRK_FLAG_CTL_MSG_DELAY_5M	(1U << 10)
#define QUIRK_FLAG_IFACE_DELAY		(1U << 11)
#define QUIRK_FLAG_VALIDATE_RATES	(1U << 12)
#define QUIRK_FLAG_DISABLE_AUTOSUSPEND	(1U << 13)
#define QUIRK_FLAG_IGNORE_CTL_ERROR	(1U << 14)
#define QUIRK_FLAG_DSD_RAW		(1U << 15)
#define QUIRK_FLAG_SET_IFACE_FIRST	(1U << 16)

struct audioformat;

enum snd_vendor_pcm_open_close {
	SOUND_PCM_CLOSE = 0,
	SOUND_PCM_OPEN,
};

/**
 * struct snd_usb_audio_vendor_ops - function callbacks for USB audio accelerators
 * @connect: called when a new interface is found
 * @disconnect: called when an interface is removed
 * @set_interface: called when an interface is initialized
 * @set_rate: called when the rate is set
 * @set_pcm_buf: called when the pcm buffer is set
 * @set_pcm_intf: called when the pcm interface is set
 * @set_pcm_connection: called when pcm is opened/closed
 * @set_pcm_binterval: called when the pcm binterval is set
 * @usb_add_ctls: called when USB controls are added
 *
 * Set of callbacks for some accelerated USB audio streaming hardware.
 *
 * TODO: make this USB host-controller specific, right now this only works for
 * one USB controller in the system at a time, which is only realistic for
 * self-contained systems like phones.
 */
struct snd_usb_audio_vendor_ops {
	int (*connect)(struct usb_interface *intf);
	void (*disconnect)(struct usb_interface *intf);

	int (*set_interface)(struct usb_device *udev,
			     struct usb_host_interface *alts,
			     int iface, int alt);
	int (*set_rate)(int iface, int rate, int alt);
	int (*set_pcm_buf)(struct usb_device *udev, int iface);
	int (*set_pcm_intf)(struct usb_interface *intf, int iface, int alt,
			    int direction);
	int (*set_pcm_connection)(struct usb_device *udev,
				  enum snd_vendor_pcm_open_close onoff,
				  int direction);
	int (*set_pcm_binterval)(const struct audioformat *fp,
				 const struct audioformat *found,
				 int *cur_attr, int *attr);
	int (*usb_add_ctls)(struct snd_usb_audio *chip);
};
>>>>>>> upstream/android-13

#endif /* __USBAUDIO_H */
