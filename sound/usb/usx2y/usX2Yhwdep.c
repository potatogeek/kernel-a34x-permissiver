<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> upstream/android-13
/*
 * Driver for Tascam US-X2Y USB soundcards
 *
 * FPGA Loader + ALSA Startup
 *
 * Copyright (c) 2003 by Karsten Wiese <annabellesgarden@yahoo.de>
<<<<<<< HEAD
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
=======
>>>>>>> upstream/android-13
 */

#include <linux/interrupt.h>
#include <linux/slab.h>
#include <linux/usb.h>
#include <sound/core.h>
#include <sound/memalloc.h>
#include <sound/pcm.h>
#include <sound/hwdep.h>
#include "usx2y.h"
#include "usbusx2y.h"
#include "usX2Yhwdep.h"

static vm_fault_t snd_us428ctls_vm_fault(struct vm_fault *vmf)
{
	unsigned long offset;
<<<<<<< HEAD
	struct page * page;
=======
	struct page *page;
>>>>>>> upstream/android-13
	void *vaddr;

	snd_printdd("ENTER, start %lXh, pgoff %ld\n",
		   vmf->vma->vm_start,
		   vmf->pgoff);
<<<<<<< HEAD
	
	offset = vmf->pgoff << PAGE_SHIFT;
	vaddr = (char *)((struct usX2Ydev *)vmf->vma->vm_private_data)->us428ctls_sharedmem + offset;
=======

	offset = vmf->pgoff << PAGE_SHIFT;
	vaddr = (char *)((struct usx2ydev *)vmf->vma->vm_private_data)->us428ctls_sharedmem + offset;
>>>>>>> upstream/android-13
	page = virt_to_page(vaddr);
	get_page(page);
	vmf->page = page;

	snd_printdd("vaddr=%p made us428ctls_vm_fault() page %p\n",
		    vaddr, page);

	return 0;
}

static const struct vm_operations_struct us428ctls_vm_ops = {
	.fault = snd_us428ctls_vm_fault,
};

<<<<<<< HEAD
static int snd_us428ctls_mmap(struct snd_hwdep * hw, struct file *filp, struct vm_area_struct *area)
{
	unsigned long	size = (unsigned long)(area->vm_end - area->vm_start);
	struct usX2Ydev	*us428 = hw->private_data;

	// FIXME this hwdep interface is used twice: fpga download and mmap for controlling Lights etc. Maybe better using 2 hwdep devs?
	// so as long as the device isn't fully initialised yet we return -EBUSY here.
 	if (!(us428->chip_status & USX2Y_STAT_CHIP_INIT))
		return -EBUSY;

	/* if userspace tries to mmap beyond end of our buffer, fail */ 
        if (size > PAGE_ALIGN(sizeof(struct us428ctls_sharedmem))) {
		snd_printd( "%lu > %lu\n", size, (unsigned long)sizeof(struct us428ctls_sharedmem)); 
                return -EINVAL;
	}

	if (!us428->us428ctls_sharedmem) {
		init_waitqueue_head(&us428->us428ctls_wait_queue_head);
		if(!(us428->us428ctls_sharedmem = snd_malloc_pages(sizeof(struct us428ctls_sharedmem), GFP_KERNEL)))
			return -ENOMEM;
		memset(us428->us428ctls_sharedmem, -1, sizeof(struct us428ctls_sharedmem));
		us428->us428ctls_sharedmem->CtlSnapShotLast = -2;
	}
=======
static int snd_us428ctls_mmap(struct snd_hwdep *hw, struct file *filp, struct vm_area_struct *area)
{
	unsigned long	size = (unsigned long)(area->vm_end - area->vm_start);
	struct usx2ydev	*us428 = hw->private_data;

	// FIXME this hwdep interface is used twice: fpga download and mmap for controlling Lights etc. Maybe better using 2 hwdep devs?
	// so as long as the device isn't fully initialised yet we return -EBUSY here.
	if (!(us428->chip_status & USX2Y_STAT_CHIP_INIT))
		return -EBUSY;

	/* if userspace tries to mmap beyond end of our buffer, fail */
	if (size > US428_SHAREDMEM_PAGES) {
		snd_printd("%lu > %lu\n", size, (unsigned long)US428_SHAREDMEM_PAGES);
		return -EINVAL;
	}

>>>>>>> upstream/android-13
	area->vm_ops = &us428ctls_vm_ops;
	area->vm_flags |= VM_DONTEXPAND | VM_DONTDUMP;
	area->vm_private_data = hw->private_data;
	return 0;
}

static __poll_t snd_us428ctls_poll(struct snd_hwdep *hw, struct file *file, poll_table *wait)
{
	__poll_t	mask = 0;
<<<<<<< HEAD
	struct usX2Ydev	*us428 = hw->private_data;
	struct us428ctls_sharedmem *shm = us428->us428ctls_sharedmem;
=======
	struct usx2ydev	*us428 = hw->private_data;
	struct us428ctls_sharedmem *shm = us428->us428ctls_sharedmem;

>>>>>>> upstream/android-13
	if (us428->chip_status & USX2Y_STAT_CHIP_HUP)
		return EPOLLHUP;

	poll_wait(file, &us428->us428ctls_wait_queue_head, wait);

<<<<<<< HEAD
	if (shm != NULL && shm->CtlSnapShotLast != shm->CtlSnapShotRed)
=======
	if (shm && shm->ctl_snapshot_last != shm->ctl_snapshot_red)
>>>>>>> upstream/android-13
		mask |= EPOLLIN;

	return mask;
}


<<<<<<< HEAD
static int snd_usX2Y_hwdep_dsp_status(struct snd_hwdep *hw,
				      struct snd_hwdep_dsp_status *info)
{
	static char *type_ids[USX2Y_TYPE_NUMS] = {
=======
static int snd_usx2y_hwdep_dsp_status(struct snd_hwdep *hw,
				      struct snd_hwdep_dsp_status *info)
{
	static const char * const type_ids[USX2Y_TYPE_NUMS] = {
>>>>>>> upstream/android-13
		[USX2Y_TYPE_122] = "us122",
		[USX2Y_TYPE_224] = "us224",
		[USX2Y_TYPE_428] = "us428",
	};
<<<<<<< HEAD
	struct usX2Ydev	*us428 = hw->private_data;
=======
	struct usx2ydev	*us428 = hw->private_data;
>>>>>>> upstream/android-13
	int id = -1;

	switch (le16_to_cpu(us428->dev->descriptor.idProduct)) {
	case USB_ID_US122:
		id = USX2Y_TYPE_122;
		break;
	case USB_ID_US224:
		id = USX2Y_TYPE_224;
		break;
	case USB_ID_US428:
		id = USX2Y_TYPE_428;
		break;
	}
<<<<<<< HEAD
	if (0 > id)
=======
	if (id < 0)
>>>>>>> upstream/android-13
		return -ENODEV;
	strcpy(info->id, type_ids[id]);
	info->num_dsps = 2;		// 0: Prepad Data, 1: FPGA Code
	if (us428->chip_status & USX2Y_STAT_CHIP_INIT)
		info->chip_ready = 1;
	info->version = USX2Y_DRIVER_VERSION;
	return 0;
}

<<<<<<< HEAD

static int usX2Y_create_usbmidi(struct snd_card *card)
{
	static struct snd_usb_midi_endpoint_info quirk_data_1 = {
=======
static int usx2y_create_usbmidi(struct snd_card *card)
{
	static const struct snd_usb_midi_endpoint_info quirk_data_1 = {
>>>>>>> upstream/android-13
		.out_ep = 0x06,
		.in_ep = 0x06,
		.out_cables =	0x001,
		.in_cables =	0x001
	};
<<<<<<< HEAD
	static struct snd_usb_audio_quirk quirk_1 = {
		.vendor_name =	"TASCAM",
		.product_name =	NAME_ALLCAPS,
		.ifnum = 	0,
       		.type = QUIRK_MIDI_FIXED_ENDPOINT,
		.data = &quirk_data_1
	};
	static struct snd_usb_midi_endpoint_info quirk_data_2 = {
=======
	static const struct snd_usb_audio_quirk quirk_1 = {
		.vendor_name =	"TASCAM",
		.product_name =	NAME_ALLCAPS,
		.ifnum =	0,
		.type = QUIRK_MIDI_FIXED_ENDPOINT,
		.data = &quirk_data_1
	};
	static const struct snd_usb_midi_endpoint_info quirk_data_2 = {
>>>>>>> upstream/android-13
		.out_ep = 0x06,
		.in_ep = 0x06,
		.out_cables =	0x003,
		.in_cables =	0x003
	};
<<<<<<< HEAD
	static struct snd_usb_audio_quirk quirk_2 = {
		.vendor_name =	"TASCAM",
		.product_name =	"US428",
		.ifnum = 	0,
       		.type = QUIRK_MIDI_FIXED_ENDPOINT,
		.data = &quirk_data_2
	};
	struct usb_device *dev = usX2Y(card)->dev;
	struct usb_interface *iface = usb_ifnum_to_if(dev, 0);
	struct snd_usb_audio_quirk *quirk =
		le16_to_cpu(dev->descriptor.idProduct) == USB_ID_US428 ?
		&quirk_2 : &quirk_1;

	snd_printdd("usX2Y_create_usbmidi \n");
	return snd_usbmidi_create(card, iface, &usX2Y(card)->midi_list, quirk);
}

static int usX2Y_create_alsa_devices(struct snd_card *card)
{
	int err;

	do {
		if ((err = usX2Y_create_usbmidi(card)) < 0) {
			snd_printk(KERN_ERR "usX2Y_create_alsa_devices: usX2Y_create_usbmidi error %i \n", err);
			break;
		}
		if ((err = usX2Y_audio_create(card)) < 0) 
			break;
		if ((err = usX2Y_hwdep_pcm_new(card)) < 0)
			break;
		if ((err = snd_card_register(card)) < 0)
			break;
	} while (0);

	return err;
} 

static int snd_usX2Y_hwdep_dsp_load(struct snd_hwdep *hw,
				    struct snd_hwdep_dsp_image *dsp)
{
	struct usX2Ydev *priv = hw->private_data;
	struct usb_device* dev = priv->dev;
	int lret, err;
	char *buf;

	snd_printdd( "dsp_load %s\n", dsp->name);
=======
	static const struct snd_usb_audio_quirk quirk_2 = {
		.vendor_name =	"TASCAM",
		.product_name =	"US428",
		.ifnum =	0,
		.type = QUIRK_MIDI_FIXED_ENDPOINT,
		.data = &quirk_data_2
	};
	struct usb_device *dev = usx2y(card)->dev;
	struct usb_interface *iface = usb_ifnum_to_if(dev, 0);
	const struct snd_usb_audio_quirk *quirk =
		le16_to_cpu(dev->descriptor.idProduct) == USB_ID_US428 ?
		&quirk_2 : &quirk_1;

	snd_printdd("%s\n", __func__);
	return snd_usbmidi_create(card, iface, &usx2y(card)->midi_list, quirk);
}

static int usx2y_create_alsa_devices(struct snd_card *card)
{
	int err;

	err = usx2y_create_usbmidi(card);
	if (err < 0) {
		snd_printk(KERN_ERR "%s: usx2y_create_usbmidi error %i\n", __func__, err);
		return err;
	}
	err = usx2y_audio_create(card);
	if (err < 0)
		return err;
	err = usx2y_hwdep_pcm_new(card);
	if (err < 0)
		return err;
	err = snd_card_register(card);
	if (err < 0)
		return err;
	return 0;
}

static int snd_usx2y_hwdep_dsp_load(struct snd_hwdep *hw,
				    struct snd_hwdep_dsp_image *dsp)
{
	struct usx2ydev *priv = hw->private_data;
	struct usb_device *dev = priv->dev;
	int lret, err;
	char *buf;

	snd_printdd("dsp_load %s\n", dsp->name);
>>>>>>> upstream/android-13

	buf = memdup_user(dsp->image, dsp->length);
	if (IS_ERR(buf))
		return PTR_ERR(buf);

	err = usb_set_interface(dev, 0, 1);
	if (err)
<<<<<<< HEAD
		snd_printk(KERN_ERR "usb_set_interface error \n");
=======
		snd_printk(KERN_ERR "usb_set_interface error\n");
>>>>>>> upstream/android-13
	else
		err = usb_bulk_msg(dev, usb_sndbulkpipe(dev, 2), buf, dsp->length, &lret, 6000);
	kfree(buf);
	if (err)
		return err;
	if (dsp->index == 1) {
		msleep(250);				// give the device some time
<<<<<<< HEAD
		err = usX2Y_AsyncSeq04_init(priv);
		if (err) {
			snd_printk(KERN_ERR "usX2Y_AsyncSeq04_init error \n");
			return err;
		}
		err = usX2Y_In04_init(priv);
		if (err) {
			snd_printk(KERN_ERR "usX2Y_In04_init error \n");
			return err;
		}
		err = usX2Y_create_alsa_devices(hw->card);
		if (err) {
			snd_printk(KERN_ERR "usX2Y_create_alsa_devices error %i \n", err);
			snd_card_free(hw->card);
			return err;
		}
		priv->chip_status |= USX2Y_STAT_CHIP_INIT; 
=======
		err = usx2y_async_seq04_init(priv);
		if (err) {
			snd_printk(KERN_ERR "usx2y_async_seq04_init error\n");
			return err;
		}
		err = usx2y_in04_init(priv);
		if (err) {
			snd_printk(KERN_ERR "usx2y_in04_init error\n");
			return err;
		}
		err = usx2y_create_alsa_devices(hw->card);
		if (err) {
			snd_printk(KERN_ERR "usx2y_create_alsa_devices error %i\n", err);
			return err;
		}
		priv->chip_status |= USX2Y_STAT_CHIP_INIT;
>>>>>>> upstream/android-13
		snd_printdd("%s: alsa all started\n", hw->name);
	}
	return err;
}

<<<<<<< HEAD

int usX2Y_hwdep_new(struct snd_card *card, struct usb_device* device)
{
	int err;
	struct snd_hwdep *hw;

	if ((err = snd_hwdep_new(card, SND_USX2Y_LOADER_ID, 0, &hw)) < 0)
		return err;

	hw->iface = SNDRV_HWDEP_IFACE_USX2Y;
	hw->private_data = usX2Y(card);
	hw->ops.dsp_status = snd_usX2Y_hwdep_dsp_status;
	hw->ops.dsp_load = snd_usX2Y_hwdep_dsp_load;
=======
int usx2y_hwdep_new(struct snd_card *card, struct usb_device *device)
{
	int err;
	struct snd_hwdep *hw;
	struct usx2ydev	*us428 = usx2y(card);

	err = snd_hwdep_new(card, SND_USX2Y_LOADER_ID, 0, &hw);
	if (err < 0)
		return err;

	hw->iface = SNDRV_HWDEP_IFACE_USX2Y;
	hw->private_data = us428;
	hw->ops.dsp_status = snd_usx2y_hwdep_dsp_status;
	hw->ops.dsp_load = snd_usx2y_hwdep_dsp_load;
>>>>>>> upstream/android-13
	hw->ops.mmap = snd_us428ctls_mmap;
	hw->ops.poll = snd_us428ctls_poll;
	hw->exclusive = 1;
	sprintf(hw->name, "/dev/bus/usb/%03d/%03d", device->bus->busnum, device->devnum);
<<<<<<< HEAD
	return 0;
}

=======

	us428->us428ctls_sharedmem = alloc_pages_exact(US428_SHAREDMEM_PAGES, GFP_KERNEL);
	if (!us428->us428ctls_sharedmem)
		return -ENOMEM;
	memset(us428->us428ctls_sharedmem, -1, US428_SHAREDMEM_PAGES);
	us428->us428ctls_sharedmem->ctl_snapshot_last = -2;

	return 0;
}
>>>>>>> upstream/android-13
