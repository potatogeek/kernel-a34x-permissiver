<<<<<<< HEAD
/*
 *  Routines for driver control interface
 *  Copyright (c) by Jaroslav Kysela <perex@perex.cz>
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
 *
=======
// SPDX-License-Identifier: GPL-2.0-or-later
/*
 *  Routines for driver control interface
 *  Copyright (c) by Jaroslav Kysela <perex@perex.cz>
>>>>>>> upstream/android-13
 */

#include <linux/threads.h>
#include <linux/interrupt.h>
#include <linux/module.h>
<<<<<<< HEAD
=======
#include <linux/moduleparam.h>
>>>>>>> upstream/android-13
#include <linux/slab.h>
#include <linux/vmalloc.h>
#include <linux/time.h>
#include <linux/mm.h>
<<<<<<< HEAD
=======
#include <linux/math64.h>
>>>>>>> upstream/android-13
#include <linux/sched/signal.h>
#include <sound/core.h>
#include <sound/minors.h>
#include <sound/info.h>
#include <sound/control.h>

<<<<<<< HEAD
/* max number of user-defined controls */
#define MAX_USER_CONTROLS	32
=======
// Max allocation size for user controls.
static int max_user_ctl_alloc_size = 8 * 1024 * 1024;
module_param_named(max_user_ctl_alloc_size, max_user_ctl_alloc_size, int, 0444);
MODULE_PARM_DESC(max_user_ctl_alloc_size, "Max allocation size for user controls");

>>>>>>> upstream/android-13
#define MAX_CONTROL_COUNT	1028

struct snd_kctl_ioctl {
	struct list_head list;		/* list of all ioctls */
	snd_kctl_ioctl_func_t fioctl;
};

static DECLARE_RWSEM(snd_ioctl_rwsem);
<<<<<<< HEAD
=======
static DECLARE_RWSEM(snd_ctl_layer_rwsem);
>>>>>>> upstream/android-13
static LIST_HEAD(snd_control_ioctls);
#ifdef CONFIG_COMPAT
static LIST_HEAD(snd_control_compat_ioctls);
#endif
<<<<<<< HEAD
=======
static struct snd_ctl_layer_ops *snd_ctl_layer;
>>>>>>> upstream/android-13

static int snd_ctl_open(struct inode *inode, struct file *file)
{
	unsigned long flags;
	struct snd_card *card;
	struct snd_ctl_file *ctl;
	int i, err;

<<<<<<< HEAD
	err = nonseekable_open(inode, file);
=======
	err = stream_open(inode, file);
>>>>>>> upstream/android-13
	if (err < 0)
		return err;

	card = snd_lookup_minor_data(iminor(inode), SNDRV_DEVICE_TYPE_CONTROL);
	if (!card) {
		err = -ENODEV;
		goto __error1;
	}
	err = snd_card_file_add(card, file);
	if (err < 0) {
		err = -ENODEV;
		goto __error1;
	}
	if (!try_module_get(card->module)) {
		err = -EFAULT;
		goto __error2;
	}
	ctl = kzalloc(sizeof(*ctl), GFP_KERNEL);
	if (ctl == NULL) {
		err = -ENOMEM;
		goto __error;
	}
	INIT_LIST_HEAD(&ctl->events);
	init_waitqueue_head(&ctl->change_sleep);
	spin_lock_init(&ctl->read_lock);
	ctl->card = card;
	for (i = 0; i < SND_CTL_SUBDEV_ITEMS; i++)
		ctl->preferred_subdevice[i] = -1;
	ctl->pid = get_pid(task_pid(current));
	file->private_data = ctl;
	write_lock_irqsave(&card->ctl_files_rwlock, flags);
	list_add_tail(&ctl->list, &card->ctl_files);
	write_unlock_irqrestore(&card->ctl_files_rwlock, flags);
	snd_card_unref(card);
	return 0;

      __error:
	module_put(card->module);
      __error2:
	snd_card_file_remove(card, file);
      __error1:
	if (card)
		snd_card_unref(card);
      	return err;
}

static void snd_ctl_empty_read_queue(struct snd_ctl_file * ctl)
{
	unsigned long flags;
	struct snd_kctl_event *cread;

	spin_lock_irqsave(&ctl->read_lock, flags);
	while (!list_empty(&ctl->events)) {
		cread = snd_kctl_event(ctl->events.next);
		list_del(&cread->list);
		kfree(cread);
	}
	spin_unlock_irqrestore(&ctl->read_lock, flags);
}

static int snd_ctl_release(struct inode *inode, struct file *file)
{
	unsigned long flags;
	struct snd_card *card;
	struct snd_ctl_file *ctl;
	struct snd_kcontrol *control;
	unsigned int idx;

	ctl = file->private_data;
	file->private_data = NULL;
	card = ctl->card;
	write_lock_irqsave(&card->ctl_files_rwlock, flags);
	list_del(&ctl->list);
	write_unlock_irqrestore(&card->ctl_files_rwlock, flags);
	down_write(&card->controls_rwsem);
	list_for_each_entry(control, &card->controls, list)
		for (idx = 0; idx < control->count; idx++)
			if (control->vd[idx].owner == ctl)
				control->vd[idx].owner = NULL;
	up_write(&card->controls_rwsem);
	snd_ctl_empty_read_queue(ctl);
	put_pid(ctl->pid);
	kfree(ctl);
	module_put(card->module);
	snd_card_file_remove(card, file);
	return 0;
}

/**
 * snd_ctl_notify - Send notification to user-space for a control change
 * @card: the card to send notification
 * @mask: the event mask, SNDRV_CTL_EVENT_*
 * @id: the ctl element id to send notification
 *
 * This function adds an event record with the given id and mask, appends
 * to the list and wakes up the user-space for notification.  This can be
 * called in the atomic context.
 */
void snd_ctl_notify(struct snd_card *card, unsigned int mask,
		    struct snd_ctl_elem_id *id)
{
	unsigned long flags;
	struct snd_ctl_file *ctl;
	struct snd_kctl_event *ev;

	if (snd_BUG_ON(!card || !id))
		return;
	if (card->shutdown)
		return;
<<<<<<< HEAD
	read_lock(&card->ctl_files_rwlock);
=======
	read_lock_irqsave(&card->ctl_files_rwlock, flags);
>>>>>>> upstream/android-13
#if IS_ENABLED(CONFIG_SND_MIXER_OSS)
	card->mixer_oss_change_count++;
#endif
	list_for_each_entry(ctl, &card->ctl_files, list) {
		if (!ctl->subscribed)
			continue;
<<<<<<< HEAD
		spin_lock_irqsave(&ctl->read_lock, flags);
=======
		spin_lock(&ctl->read_lock);
>>>>>>> upstream/android-13
		list_for_each_entry(ev, &ctl->events, list) {
			if (ev->id.numid == id->numid) {
				ev->mask |= mask;
				goto _found;
			}
		}
		ev = kzalloc(sizeof(*ev), GFP_ATOMIC);
		if (ev) {
			ev->id = *id;
			ev->mask = mask;
			list_add_tail(&ev->list, &ctl->events);
		} else {
			dev_err(card->dev, "No memory available to allocate event\n");
		}
	_found:
		wake_up(&ctl->change_sleep);
<<<<<<< HEAD
		spin_unlock_irqrestore(&ctl->read_lock, flags);
		kill_fasync(&ctl->fasync, SIGIO, POLL_IN);
	}
	read_unlock(&card->ctl_files_rwlock);
=======
		spin_unlock(&ctl->read_lock);
		kill_fasync(&ctl->fasync, SIGIO, POLL_IN);
	}
	read_unlock_irqrestore(&card->ctl_files_rwlock, flags);
>>>>>>> upstream/android-13
}
EXPORT_SYMBOL(snd_ctl_notify);

/**
<<<<<<< HEAD
=======
 * snd_ctl_notify_one - Send notification to user-space for a control change
 * @card: the card to send notification
 * @mask: the event mask, SNDRV_CTL_EVENT_*
 * @kctl: the pointer with the control instance
 * @ioff: the additional offset to the control index
 *
 * This function calls snd_ctl_notify() and does additional jobs
 * like LED state changes.
 */
void snd_ctl_notify_one(struct snd_card *card, unsigned int mask,
			struct snd_kcontrol *kctl, unsigned int ioff)
{
	struct snd_ctl_elem_id id = kctl->id;
	struct snd_ctl_layer_ops *lops;

	id.index += ioff;
	id.numid += ioff;
	snd_ctl_notify(card, mask, &id);
	down_read(&snd_ctl_layer_rwsem);
	for (lops = snd_ctl_layer; lops; lops = lops->next)
		lops->lnotify(card, mask, kctl, ioff);
	up_read(&snd_ctl_layer_rwsem);
}
EXPORT_SYMBOL(snd_ctl_notify_one);

/**
>>>>>>> upstream/android-13
 * snd_ctl_new - create a new control instance with some elements
 * @kctl: the pointer to store new control instance
 * @count: the number of elements in this control
 * @access: the default access flags for elements in this control
 * @file: given when locking these elements
 *
 * Allocates a memory object for a new control instance. The instance has
 * elements as many as the given number (@count). Each element has given
 * access permissions (@access). Each element is locked when @file is given.
 *
 * Return: 0 on success, error code on failure
 */
static int snd_ctl_new(struct snd_kcontrol **kctl, unsigned int count,
		       unsigned int access, struct snd_ctl_file *file)
{
<<<<<<< HEAD
	unsigned int size;
=======
>>>>>>> upstream/android-13
	unsigned int idx;

	if (count == 0 || count > MAX_CONTROL_COUNT)
		return -EINVAL;

<<<<<<< HEAD
	size  = sizeof(struct snd_kcontrol);
	size += sizeof(struct snd_kcontrol_volatile) * count;

	*kctl = kzalloc(size, GFP_KERNEL);
=======
	*kctl = kzalloc(struct_size(*kctl, vd, count), GFP_KERNEL);
>>>>>>> upstream/android-13
	if (!*kctl)
		return -ENOMEM;

	for (idx = 0; idx < count; idx++) {
		(*kctl)->vd[idx].access = access;
		(*kctl)->vd[idx].owner = file;
	}
	(*kctl)->count = count;

	return 0;
}

/**
 * snd_ctl_new1 - create a control instance from the template
 * @ncontrol: the initialization record
 * @private_data: the private data to set
 *
 * Allocates a new struct snd_kcontrol instance and initialize from the given
 * template.  When the access field of ncontrol is 0, it's assumed as
 * READWRITE access. When the count field is 0, it's assumes as one.
 *
 * Return: The pointer of the newly generated instance, or %NULL on failure.
 */
struct snd_kcontrol *snd_ctl_new1(const struct snd_kcontrol_new *ncontrol,
				  void *private_data)
{
	struct snd_kcontrol *kctl;
	unsigned int count;
	unsigned int access;
	int err;

	if (snd_BUG_ON(!ncontrol || !ncontrol->info))
		return NULL;

	count = ncontrol->count;
	if (count == 0)
		count = 1;

	access = ncontrol->access;
	if (access == 0)
		access = SNDRV_CTL_ELEM_ACCESS_READWRITE;
	access &= (SNDRV_CTL_ELEM_ACCESS_READWRITE |
		   SNDRV_CTL_ELEM_ACCESS_VOLATILE |
		   SNDRV_CTL_ELEM_ACCESS_INACTIVE |
		   SNDRV_CTL_ELEM_ACCESS_TLV_READWRITE |
		   SNDRV_CTL_ELEM_ACCESS_TLV_COMMAND |
<<<<<<< HEAD
		   SNDRV_CTL_ELEM_ACCESS_TLV_CALLBACK);
=======
		   SNDRV_CTL_ELEM_ACCESS_TLV_CALLBACK |
		   SNDRV_CTL_ELEM_ACCESS_LED_MASK |
		   SNDRV_CTL_ELEM_ACCESS_SKIP_CHECK);
>>>>>>> upstream/android-13

	err = snd_ctl_new(&kctl, count, access, NULL);
	if (err < 0)
		return NULL;

	/* The 'numid' member is decided when calling snd_ctl_add(). */
	kctl->id.iface = ncontrol->iface;
	kctl->id.device = ncontrol->device;
	kctl->id.subdevice = ncontrol->subdevice;
	if (ncontrol->name) {
<<<<<<< HEAD
		strlcpy(kctl->id.name, ncontrol->name, sizeof(kctl->id.name));
=======
		strscpy(kctl->id.name, ncontrol->name, sizeof(kctl->id.name));
>>>>>>> upstream/android-13
		if (strcmp(ncontrol->name, kctl->id.name) != 0)
			pr_warn("ALSA: Control name '%s' truncated to '%s'\n",
				ncontrol->name, kctl->id.name);
	}
	kctl->id.index = ncontrol->index;

	kctl->info = ncontrol->info;
	kctl->get = ncontrol->get;
	kctl->put = ncontrol->put;
	kctl->tlv.p = ncontrol->tlv.p;

	kctl->private_value = ncontrol->private_value;
	kctl->private_data = private_data;

	return kctl;
}
EXPORT_SYMBOL(snd_ctl_new1);

/**
 * snd_ctl_free_one - release the control instance
 * @kcontrol: the control instance
 *
 * Releases the control instance created via snd_ctl_new()
 * or snd_ctl_new1().
 * Don't call this after the control was added to the card.
 */
void snd_ctl_free_one(struct snd_kcontrol *kcontrol)
{
	if (kcontrol) {
		if (kcontrol->private_free)
			kcontrol->private_free(kcontrol);
		kfree(kcontrol);
	}
}
EXPORT_SYMBOL(snd_ctl_free_one);

static bool snd_ctl_remove_numid_conflict(struct snd_card *card,
					  unsigned int count)
{
	struct snd_kcontrol *kctl;

	/* Make sure that the ids assigned to the control do not wrap around */
	if (card->last_numid >= UINT_MAX - count)
		card->last_numid = 0;

	list_for_each_entry(kctl, &card->controls, list) {
		if (kctl->id.numid < card->last_numid + 1 + count &&
		    kctl->id.numid + kctl->count > card->last_numid + 1) {
		    	card->last_numid = kctl->id.numid + kctl->count - 1;
			return true;
		}
	}
	return false;
}

static int snd_ctl_find_hole(struct snd_card *card, unsigned int count)
{
	unsigned int iter = 100000;

	while (snd_ctl_remove_numid_conflict(card, count)) {
		if (--iter == 0) {
			/* this situation is very unlikely */
			dev_err(card->dev, "unable to allocate new control numid\n");
			return -ENOMEM;
		}
	}
	return 0;
}

<<<<<<< HEAD
/* add a new kcontrol object; call with card->controls_rwsem locked */
static int __snd_ctl_add(struct snd_card *card, struct snd_kcontrol *kcontrol)
{
	struct snd_ctl_elem_id id;
	unsigned int idx;
	unsigned int count;
=======
/* check whether the given id is contained in the given kctl */
static bool elem_id_matches(const struct snd_kcontrol *kctl,
			    const struct snd_ctl_elem_id *id)
{
	return kctl->id.iface == id->iface &&
		kctl->id.device == id->device &&
		kctl->id.subdevice == id->subdevice &&
		!strncmp(kctl->id.name, id->name, sizeof(kctl->id.name)) &&
		kctl->id.index <= id->index &&
		kctl->id.index + kctl->count > id->index;
}

#ifdef CONFIG_SND_CTL_FAST_LOOKUP
/* Compute a hash key for the corresponding ctl id
 * It's for the name lookup, hence the numid is excluded.
 * The hash key is bound in LONG_MAX to be used for Xarray key.
 */
#define MULTIPLIER	37
static unsigned long get_ctl_id_hash(const struct snd_ctl_elem_id *id)
{
	unsigned long h;
	const unsigned char *p;

	h = id->iface;
	h = MULTIPLIER * h + id->device;
	h = MULTIPLIER * h + id->subdevice;
	for (p = id->name; *p; p++)
		h = MULTIPLIER * h + *p;
	h = MULTIPLIER * h + id->index;
	h &= LONG_MAX;
	return h;
}

/* add hash entries to numid and ctl xarray tables */
static void add_hash_entries(struct snd_card *card,
			     struct snd_kcontrol *kcontrol)
{
	struct snd_ctl_elem_id id = kcontrol->id;
	int i;

	xa_store_range(&card->ctl_numids, kcontrol->id.numid,
		       kcontrol->id.numid + kcontrol->count - 1,
		       kcontrol, GFP_KERNEL);

	for (i = 0; i < kcontrol->count; i++) {
		id.index = kcontrol->id.index + i;
		if (xa_insert(&card->ctl_hash, get_ctl_id_hash(&id),
			      kcontrol, GFP_KERNEL)) {
			/* skip hash for this entry, noting we had collision */
			card->ctl_hash_collision = true;
			dev_dbg(card->dev, "ctl_hash collision %d:%s:%d\n",
				id.iface, id.name, id.index);
		}
	}
}

/* remove hash entries that have been added */
static void remove_hash_entries(struct snd_card *card,
				struct snd_kcontrol *kcontrol)
{
	struct snd_ctl_elem_id id = kcontrol->id;
	struct snd_kcontrol *matched;
	unsigned long h;
	int i;

	for (i = 0; i < kcontrol->count; i++) {
		xa_erase(&card->ctl_numids, id.numid);
		h = get_ctl_id_hash(&id);
		matched = xa_load(&card->ctl_hash, h);
		if (matched && (matched == kcontrol ||
				elem_id_matches(matched, &id)))
			xa_erase(&card->ctl_hash, h);
		id.index++;
		id.numid++;
	}
}
#else /* CONFIG_SND_CTL_FAST_LOOKUP */
static inline void add_hash_entries(struct snd_card *card,
				    struct snd_kcontrol *kcontrol)
{
}
static inline void remove_hash_entries(struct snd_card *card,
				       struct snd_kcontrol *kcontrol)
{
}
#endif /* CONFIG_SND_CTL_FAST_LOOKUP */

enum snd_ctl_add_mode {
	CTL_ADD_EXCLUSIVE, CTL_REPLACE, CTL_ADD_ON_REPLACE,
};

/* add/replace a new kcontrol object; call with card->controls_rwsem locked */
static int __snd_ctl_add_replace(struct snd_card *card,
				 struct snd_kcontrol *kcontrol,
				 enum snd_ctl_add_mode mode)
{
	struct snd_ctl_elem_id id;
	unsigned int idx;
	struct snd_kcontrol *old;
	int err;
>>>>>>> upstream/android-13

	id = kcontrol->id;
	if (id.index > UINT_MAX - kcontrol->count)
		return -EINVAL;

<<<<<<< HEAD
	if (snd_ctl_find_id(card, &id)) {
		dev_err(card->dev,
			"control %i:%i:%i:%s:%i is already present\n",
			id.iface, id.device, id.subdevice, id.name, id.index);
		return -EBUSY;
=======
	old = snd_ctl_find_id(card, &id);
	if (!old) {
		if (mode == CTL_REPLACE)
			return -EINVAL;
	} else {
		if (mode == CTL_ADD_EXCLUSIVE) {
			dev_err(card->dev,
				"control %i:%i:%i:%s:%i is already present\n",
				id.iface, id.device, id.subdevice, id.name,
				id.index);
			return -EBUSY;
		}

		err = snd_ctl_remove(card, old);
		if (err < 0)
			return err;
>>>>>>> upstream/android-13
	}

	if (snd_ctl_find_hole(card, kcontrol->count) < 0)
		return -ENOMEM;

	list_add_tail(&kcontrol->list, &card->controls);
	card->controls_count += kcontrol->count;
	kcontrol->id.numid = card->last_numid + 1;
	card->last_numid += kcontrol->count;

<<<<<<< HEAD
	id = kcontrol->id;
	count = kcontrol->count;
	for (idx = 0; idx < count; idx++, id.index++, id.numid++)
		snd_ctl_notify(card, SNDRV_CTL_EVENT_MASK_ADD, &id);
=======
	add_hash_entries(card, kcontrol);

	for (idx = 0; idx < kcontrol->count; idx++)
		snd_ctl_notify_one(card, SNDRV_CTL_EVENT_MASK_ADD, kcontrol, idx);
>>>>>>> upstream/android-13

	return 0;
}

<<<<<<< HEAD
=======
static int snd_ctl_add_replace(struct snd_card *card,
			       struct snd_kcontrol *kcontrol,
			       enum snd_ctl_add_mode mode)
{
	int err = -EINVAL;

	if (! kcontrol)
		return err;
	if (snd_BUG_ON(!card || !kcontrol->info))
		goto error;

	down_write(&card->controls_rwsem);
	err = __snd_ctl_add_replace(card, kcontrol, mode);
	up_write(&card->controls_rwsem);
	if (err < 0)
		goto error;
	return 0;

 error:
	snd_ctl_free_one(kcontrol);
	return err;
}

>>>>>>> upstream/android-13
/**
 * snd_ctl_add - add the control instance to the card
 * @card: the card instance
 * @kcontrol: the control instance to add
 *
 * Adds the control instance created via snd_ctl_new() or
 * snd_ctl_new1() to the given card. Assigns also an unique
 * numid used for fast search.
 *
 * It frees automatically the control which cannot be added.
 *
 * Return: Zero if successful, or a negative error code on failure.
 *
 */
int snd_ctl_add(struct snd_card *card, struct snd_kcontrol *kcontrol)
{
<<<<<<< HEAD
	int err = -EINVAL;

	if (! kcontrol)
		return err;
	if (snd_BUG_ON(!card || !kcontrol->info))
		goto error;

	down_write(&card->controls_rwsem);
	err = __snd_ctl_add(card, kcontrol);
	up_write(&card->controls_rwsem);
	if (err < 0)
		goto error;
	return 0;

 error:
	snd_ctl_free_one(kcontrol);
	return err;
=======
	return snd_ctl_add_replace(card, kcontrol, CTL_ADD_EXCLUSIVE);
>>>>>>> upstream/android-13
}
EXPORT_SYMBOL(snd_ctl_add);

/**
 * snd_ctl_replace - replace the control instance of the card
 * @card: the card instance
 * @kcontrol: the control instance to replace
 * @add_on_replace: add the control if not already added
 *
 * Replaces the given control.  If the given control does not exist
 * and the add_on_replace flag is set, the control is added.  If the
 * control exists, it is destroyed first.
 *
 * It frees automatically the control which cannot be added or replaced.
 *
 * Return: Zero if successful, or a negative error code on failure.
 */
int snd_ctl_replace(struct snd_card *card, struct snd_kcontrol *kcontrol,
		    bool add_on_replace)
{
<<<<<<< HEAD
	struct snd_ctl_elem_id id;
	unsigned int count;
	unsigned int idx;
	struct snd_kcontrol *old;
	int ret;

	if (!kcontrol)
		return -EINVAL;
	if (snd_BUG_ON(!card || !kcontrol->info)) {
		ret = -EINVAL;
		goto error;
	}
	id = kcontrol->id;
	down_write(&card->controls_rwsem);
	old = snd_ctl_find_id(card, &id);
	if (!old) {
		if (add_on_replace)
			goto add;
		up_write(&card->controls_rwsem);
		ret = -EINVAL;
		goto error;
	}
	ret = snd_ctl_remove(card, old);
	if (ret < 0) {
		up_write(&card->controls_rwsem);
		goto error;
	}
add:
	if (snd_ctl_find_hole(card, kcontrol->count) < 0) {
		up_write(&card->controls_rwsem);
		ret = -ENOMEM;
		goto error;
	}
	list_add_tail(&kcontrol->list, &card->controls);
	card->controls_count += kcontrol->count;
	kcontrol->id.numid = card->last_numid + 1;
	card->last_numid += kcontrol->count;
	id = kcontrol->id;
	count = kcontrol->count;
	up_write(&card->controls_rwsem);
	for (idx = 0; idx < count; idx++, id.index++, id.numid++)
		snd_ctl_notify(card, SNDRV_CTL_EVENT_MASK_ADD, &id);
	return 0;

error:
	snd_ctl_free_one(kcontrol);
	return ret;
}
EXPORT_SYMBOL(snd_ctl_replace);

=======
	return snd_ctl_add_replace(card, kcontrol,
				   add_on_replace ? CTL_ADD_ON_REPLACE : CTL_REPLACE);
}
EXPORT_SYMBOL(snd_ctl_replace);

static int __snd_ctl_remove(struct snd_card *card,
			    struct snd_kcontrol *kcontrol,
			    bool remove_hash)
{
	unsigned int idx;

	if (snd_BUG_ON(!card || !kcontrol))
		return -EINVAL;
	list_del(&kcontrol->list);

	if (remove_hash)
		remove_hash_entries(card, kcontrol);

	card->controls_count -= kcontrol->count;
	for (idx = 0; idx < kcontrol->count; idx++)
		snd_ctl_notify_one(card, SNDRV_CTL_EVENT_MASK_REMOVE, kcontrol, idx);
	snd_ctl_free_one(kcontrol);
	return 0;
}

>>>>>>> upstream/android-13
/**
 * snd_ctl_remove - remove the control from the card and release it
 * @card: the card instance
 * @kcontrol: the control instance to remove
 *
 * Removes the control from the card and then releases the instance.
 * You don't need to call snd_ctl_free_one(). You must be in
 * the write lock - down_write(&card->controls_rwsem).
 *
 * Return: 0 if successful, or a negative error code on failure.
 */
int snd_ctl_remove(struct snd_card *card, struct snd_kcontrol *kcontrol)
{
<<<<<<< HEAD
	struct snd_ctl_elem_id id;
	unsigned int idx;

	if (snd_BUG_ON(!card || !kcontrol))
		return -EINVAL;
	list_del(&kcontrol->list);
	card->controls_count -= kcontrol->count;
	id = kcontrol->id;
	for (idx = 0; idx < kcontrol->count; idx++, id.index++, id.numid++)
		snd_ctl_notify(card, SNDRV_CTL_EVENT_MASK_REMOVE, &id);
	snd_ctl_free_one(kcontrol);
	return 0;
=======
	return __snd_ctl_remove(card, kcontrol, true);
>>>>>>> upstream/android-13
}
EXPORT_SYMBOL(snd_ctl_remove);

/**
 * snd_ctl_remove_id - remove the control of the given id and release it
 * @card: the card instance
 * @id: the control id to remove
 *
 * Finds the control instance with the given id, removes it from the
 * card list and releases it.
 *
 * Return: 0 if successful, or a negative error code on failure.
 */
int snd_ctl_remove_id(struct snd_card *card, struct snd_ctl_elem_id *id)
{
	struct snd_kcontrol *kctl;
	int ret;

	down_write(&card->controls_rwsem);
	kctl = snd_ctl_find_id(card, id);
	if (kctl == NULL) {
		up_write(&card->controls_rwsem);
		return -ENOENT;
	}
	ret = snd_ctl_remove(card, kctl);
	up_write(&card->controls_rwsem);
	return ret;
}
EXPORT_SYMBOL(snd_ctl_remove_id);

/**
 * snd_ctl_remove_user_ctl - remove and release the unlocked user control
 * @file: active control handle
 * @id: the control id to remove
 *
 * Finds the control instance with the given id, removes it from the
 * card list and releases it.
 *
 * Return: 0 if successful, or a negative error code on failure.
 */
static int snd_ctl_remove_user_ctl(struct snd_ctl_file * file,
				   struct snd_ctl_elem_id *id)
{
	struct snd_card *card = file->card;
	struct snd_kcontrol *kctl;
	int idx, ret;

	down_write(&card->controls_rwsem);
	kctl = snd_ctl_find_id(card, id);
	if (kctl == NULL) {
		ret = -ENOENT;
		goto error;
	}
	if (!(kctl->vd[0].access & SNDRV_CTL_ELEM_ACCESS_USER)) {
		ret = -EINVAL;
		goto error;
	}
	for (idx = 0; idx < kctl->count; idx++)
		if (kctl->vd[idx].owner != NULL && kctl->vd[idx].owner != file) {
			ret = -EBUSY;
			goto error;
		}
	ret = snd_ctl_remove(card, kctl);
<<<<<<< HEAD
	if (ret < 0)
		goto error;
	card->user_ctl_count--;
=======
>>>>>>> upstream/android-13
error:
	up_write(&card->controls_rwsem);
	return ret;
}

/**
 * snd_ctl_activate_id - activate/inactivate the control of the given id
 * @card: the card instance
 * @id: the control id to activate/inactivate
 * @active: non-zero to activate
 *
 * Finds the control instance with the given id, and activate or
 * inactivate the control together with notification, if changed.
 * The given ID data is filled with full information.
 *
 * Return: 0 if unchanged, 1 if changed, or a negative error code on failure.
 */
int snd_ctl_activate_id(struct snd_card *card, struct snd_ctl_elem_id *id,
			int active)
{
	struct snd_kcontrol *kctl;
	struct snd_kcontrol_volatile *vd;
	unsigned int index_offset;
	int ret;

	down_write(&card->controls_rwsem);
	kctl = snd_ctl_find_id(card, id);
	if (kctl == NULL) {
		ret = -ENOENT;
		goto unlock;
	}
	index_offset = snd_ctl_get_ioff(kctl, id);
	vd = &kctl->vd[index_offset];
	ret = 0;
	if (active) {
		if (!(vd->access & SNDRV_CTL_ELEM_ACCESS_INACTIVE))
			goto unlock;
		vd->access &= ~SNDRV_CTL_ELEM_ACCESS_INACTIVE;
	} else {
		if (vd->access & SNDRV_CTL_ELEM_ACCESS_INACTIVE)
			goto unlock;
		vd->access |= SNDRV_CTL_ELEM_ACCESS_INACTIVE;
	}
	snd_ctl_build_ioff(id, kctl, index_offset);
<<<<<<< HEAD
	ret = 1;
 unlock:
	up_write(&card->controls_rwsem);
	if (ret > 0)
		snd_ctl_notify(card, SNDRV_CTL_EVENT_MASK_INFO, id);
=======
	downgrade_write(&card->controls_rwsem);
	snd_ctl_notify_one(card, SNDRV_CTL_EVENT_MASK_INFO, kctl, index_offset);
	up_read(&card->controls_rwsem);
	return 1;

 unlock:
	up_write(&card->controls_rwsem);
>>>>>>> upstream/android-13
	return ret;
}
EXPORT_SYMBOL_GPL(snd_ctl_activate_id);

/**
 * snd_ctl_rename_id - replace the id of a control on the card
 * @card: the card instance
 * @src_id: the old id
 * @dst_id: the new id
 *
 * Finds the control with the old id from the card, and replaces the
 * id with the new one.
 *
 * Return: Zero if successful, or a negative error code on failure.
 */
int snd_ctl_rename_id(struct snd_card *card, struct snd_ctl_elem_id *src_id,
		      struct snd_ctl_elem_id *dst_id)
{
	struct snd_kcontrol *kctl;

	down_write(&card->controls_rwsem);
	kctl = snd_ctl_find_id(card, src_id);
	if (kctl == NULL) {
		up_write(&card->controls_rwsem);
		return -ENOENT;
	}
<<<<<<< HEAD
	kctl->id = *dst_id;
	kctl->id.numid = card->last_numid + 1;
	card->last_numid += kctl->count;
=======
	remove_hash_entries(card, kctl);
	kctl->id = *dst_id;
	kctl->id.numid = card->last_numid + 1;
	card->last_numid += kctl->count;
	add_hash_entries(card, kctl);
>>>>>>> upstream/android-13
	up_write(&card->controls_rwsem);
	return 0;
}
EXPORT_SYMBOL(snd_ctl_rename_id);

<<<<<<< HEAD
=======
#ifndef CONFIG_SND_CTL_FAST_LOOKUP
static struct snd_kcontrol *
snd_ctl_find_numid_slow(struct snd_card *card, unsigned int numid)
{
	struct snd_kcontrol *kctl;

	list_for_each_entry(kctl, &card->controls, list) {
		if (kctl->id.numid <= numid && kctl->id.numid + kctl->count > numid)
			return kctl;
	}
	return NULL;
}
#endif /* !CONFIG_SND_CTL_FAST_LOOKUP */

>>>>>>> upstream/android-13
/**
 * snd_ctl_find_numid - find the control instance with the given number-id
 * @card: the card instance
 * @numid: the number-id to search
 *
 * Finds the control instance with the given number-id from the card.
 *
 * The caller must down card->controls_rwsem before calling this function
 * (if the race condition can happen).
 *
 * Return: The pointer of the instance if found, or %NULL if not.
 *
 */
struct snd_kcontrol *snd_ctl_find_numid(struct snd_card *card, unsigned int numid)
{
<<<<<<< HEAD
	struct snd_kcontrol *kctl;

	if (snd_BUG_ON(!card || !numid))
		return NULL;
	list_for_each_entry(kctl, &card->controls, list) {
		if (kctl->id.numid <= numid && kctl->id.numid + kctl->count > numid)
			return kctl;
	}
	return NULL;
=======
	if (snd_BUG_ON(!card || !numid))
		return NULL;
#ifdef CONFIG_SND_CTL_FAST_LOOKUP
	return xa_load(&card->ctl_numids, numid);
#else
	return snd_ctl_find_numid_slow(card, numid);
#endif
>>>>>>> upstream/android-13
}
EXPORT_SYMBOL(snd_ctl_find_numid);

/**
 * snd_ctl_find_id - find the control instance with the given id
 * @card: the card instance
 * @id: the id to search
 *
 * Finds the control instance with the given id from the card.
 *
 * The caller must down card->controls_rwsem before calling this function
 * (if the race condition can happen).
 *
 * Return: The pointer of the instance if found, or %NULL if not.
 *
 */
struct snd_kcontrol *snd_ctl_find_id(struct snd_card *card,
				     struct snd_ctl_elem_id *id)
{
	struct snd_kcontrol *kctl;

	if (snd_BUG_ON(!card || !id))
		return NULL;
	if (id->numid != 0)
		return snd_ctl_find_numid(card, id->numid);
<<<<<<< HEAD
	list_for_each_entry(kctl, &card->controls, list) {
		if (kctl->id.iface != id->iface)
			continue;
		if (kctl->id.device != id->device)
			continue;
		if (kctl->id.subdevice != id->subdevice)
			continue;
		if (strncmp(kctl->id.name, id->name, sizeof(kctl->id.name)))
			continue;
		if (kctl->id.index > id->index)
			continue;
		if (kctl->id.index + kctl->count <= id->index)
			continue;
		return kctl;
	}
=======
#ifdef CONFIG_SND_CTL_FAST_LOOKUP
	kctl = xa_load(&card->ctl_hash, get_ctl_id_hash(id));
	if (kctl && elem_id_matches(kctl, id))
		return kctl;
	if (!card->ctl_hash_collision)
		return NULL; /* we can rely on only hash table */
#endif
	/* no matching in hash table - try all as the last resort */
	list_for_each_entry(kctl, &card->controls, list)
		if (elem_id_matches(kctl, id))
			return kctl;

>>>>>>> upstream/android-13
	return NULL;
}
EXPORT_SYMBOL(snd_ctl_find_id);

static int snd_ctl_card_info(struct snd_card *card, struct snd_ctl_file * ctl,
			     unsigned int cmd, void __user *arg)
{
	struct snd_ctl_card_info *info;

	info = kzalloc(sizeof(*info), GFP_KERNEL);
	if (! info)
		return -ENOMEM;
	down_read(&snd_ioctl_rwsem);
	info->card = card->number;
<<<<<<< HEAD
	strlcpy(info->id, card->id, sizeof(info->id));
	strlcpy(info->driver, card->driver, sizeof(info->driver));
	strlcpy(info->name, card->shortname, sizeof(info->name));
	strlcpy(info->longname, card->longname, sizeof(info->longname));
	strlcpy(info->mixername, card->mixername, sizeof(info->mixername));
	strlcpy(info->components, card->components, sizeof(info->components));
=======
	strscpy(info->id, card->id, sizeof(info->id));
	strscpy(info->driver, card->driver, sizeof(info->driver));
	strscpy(info->name, card->shortname, sizeof(info->name));
	strscpy(info->longname, card->longname, sizeof(info->longname));
	strscpy(info->mixername, card->mixername, sizeof(info->mixername));
	strscpy(info->components, card->components, sizeof(info->components));
>>>>>>> upstream/android-13
	up_read(&snd_ioctl_rwsem);
	if (copy_to_user(arg, info, sizeof(struct snd_ctl_card_info))) {
		kfree(info);
		return -EFAULT;
	}
	kfree(info);
	return 0;
}

static int snd_ctl_elem_list(struct snd_card *card,
<<<<<<< HEAD
			     struct snd_ctl_elem_list __user *_list)
{
	struct snd_ctl_elem_list list;
=======
			     struct snd_ctl_elem_list *list)
{
>>>>>>> upstream/android-13
	struct snd_kcontrol *kctl;
	struct snd_ctl_elem_id id;
	unsigned int offset, space, jidx;
	int err = 0;

<<<<<<< HEAD
	if (copy_from_user(&list, _list, sizeof(list)))
		return -EFAULT;
	offset = list.offset;
	space = list.space;

	down_read(&card->controls_rwsem);
	list.count = card->controls_count;
	list.used = 0;
=======
	offset = list->offset;
	space = list->space;

	down_read(&card->controls_rwsem);
	list->count = card->controls_count;
	list->used = 0;
>>>>>>> upstream/android-13
	if (space > 0) {
		list_for_each_entry(kctl, &card->controls, list) {
			if (offset >= kctl->count) {
				offset -= kctl->count;
				continue;
			}
			for (jidx = offset; jidx < kctl->count; jidx++) {
				snd_ctl_build_ioff(&id, kctl, jidx);
<<<<<<< HEAD
				if (copy_to_user(list.pids + list.used, &id,
=======
				if (copy_to_user(list->pids + list->used, &id,
>>>>>>> upstream/android-13
						 sizeof(id))) {
					err = -EFAULT;
					goto out;
				}
<<<<<<< HEAD
				list.used++;
=======
				list->used++;
>>>>>>> upstream/android-13
				if (!--space)
					goto out;
			}
			offset = 0;
		}
	}
 out:
	up_read(&card->controls_rwsem);
<<<<<<< HEAD
	if (!err && copy_to_user(_list, &list, sizeof(list)))
		err = -EFAULT;
	return err;
}

static bool validate_element_member_dimension(struct snd_ctl_elem_info *info)
{
	unsigned int members;
	unsigned int i;

	if (info->dimen.d[0] == 0)
		return true;

	members = 1;
	for (i = 0; i < ARRAY_SIZE(info->dimen.d); ++i) {
		if (info->dimen.d[i] == 0)
			break;
		members *= info->dimen.d[i];

		/*
		 * info->count should be validated in advance, to guarantee
		 * calculation soundness.
		 */
		if (members > info->count)
			return false;
	}

	for (++i; i < ARRAY_SIZE(info->dimen.d); ++i) {
		if (info->dimen.d[i] > 0)
			return false;
	}

	return members == info->count;
}

static int snd_ctl_elem_info(struct snd_ctl_file *ctl,
			     struct snd_ctl_elem_info *info)
{
	struct snd_card *card = ctl->card;
	struct snd_kcontrol *kctl;
=======
	return err;
}

static int snd_ctl_elem_list_user(struct snd_card *card,
				  struct snd_ctl_elem_list __user *_list)
{
	struct snd_ctl_elem_list list;
	int err;

	if (copy_from_user(&list, _list, sizeof(list)))
		return -EFAULT;
	err = snd_ctl_elem_list(card, &list);
	if (err)
		return err;
	if (copy_to_user(_list, &list, sizeof(list)))
		return -EFAULT;

	return 0;
}

/* Check whether the given kctl info is valid */
static int snd_ctl_check_elem_info(struct snd_card *card,
				   const struct snd_ctl_elem_info *info)
{
	static const unsigned int max_value_counts[] = {
		[SNDRV_CTL_ELEM_TYPE_BOOLEAN]	= 128,
		[SNDRV_CTL_ELEM_TYPE_INTEGER]	= 128,
		[SNDRV_CTL_ELEM_TYPE_ENUMERATED] = 128,
		[SNDRV_CTL_ELEM_TYPE_BYTES]	= 512,
		[SNDRV_CTL_ELEM_TYPE_IEC958]	= 1,
		[SNDRV_CTL_ELEM_TYPE_INTEGER64] = 64,
	};

	if (info->type < SNDRV_CTL_ELEM_TYPE_BOOLEAN ||
	    info->type > SNDRV_CTL_ELEM_TYPE_INTEGER64) {
		if (card)
			dev_err(card->dev,
				"control %i:%i:%i:%s:%i: invalid type %d\n",
				info->id.iface, info->id.device,
				info->id.subdevice, info->id.name,
				info->id.index, info->type);
		return -EINVAL;
	}
	if (info->type == SNDRV_CTL_ELEM_TYPE_ENUMERATED &&
	    info->value.enumerated.items == 0) {
		if (card)
			dev_err(card->dev,
				"control %i:%i:%i:%s:%i: zero enum items\n",
				info->id.iface, info->id.device,
				info->id.subdevice, info->id.name,
				info->id.index);
		return -EINVAL;
	}
	if (info->count > max_value_counts[info->type]) {
		if (card)
			dev_err(card->dev,
				"control %i:%i:%i:%s:%i: invalid count %d\n",
				info->id.iface, info->id.device,
				info->id.subdevice, info->id.name,
				info->id.index, info->count);
		return -EINVAL;
	}

	return 0;
}

/* The capacity of struct snd_ctl_elem_value.value.*/
static const unsigned int value_sizes[] = {
	[SNDRV_CTL_ELEM_TYPE_BOOLEAN]	= sizeof(long),
	[SNDRV_CTL_ELEM_TYPE_INTEGER]	= sizeof(long),
	[SNDRV_CTL_ELEM_TYPE_ENUMERATED] = sizeof(unsigned int),
	[SNDRV_CTL_ELEM_TYPE_BYTES]	= sizeof(unsigned char),
	[SNDRV_CTL_ELEM_TYPE_IEC958]	= sizeof(struct snd_aes_iec958),
	[SNDRV_CTL_ELEM_TYPE_INTEGER64] = sizeof(long long),
};

#ifdef CONFIG_SND_CTL_VALIDATION
/* fill the remaining snd_ctl_elem_value data with the given pattern */
static void fill_remaining_elem_value(struct snd_ctl_elem_value *control,
				      struct snd_ctl_elem_info *info,
				      u32 pattern)
{
	size_t offset = value_sizes[info->type] * info->count;

	offset = DIV_ROUND_UP(offset, sizeof(u32));
	memset32((u32 *)control->value.bytes.data + offset, pattern,
		 sizeof(control->value) / sizeof(u32) - offset);
}

/* check whether the given integer ctl value is valid */
static int sanity_check_int_value(struct snd_card *card,
				  const struct snd_ctl_elem_value *control,
				  const struct snd_ctl_elem_info *info,
				  int i)
{
	long long lval, lmin, lmax, lstep;
	u64 rem;

	switch (info->type) {
	default:
	case SNDRV_CTL_ELEM_TYPE_BOOLEAN:
		lval = control->value.integer.value[i];
		lmin = 0;
		lmax = 1;
		lstep = 0;
		break;
	case SNDRV_CTL_ELEM_TYPE_INTEGER:
		lval = control->value.integer.value[i];
		lmin = info->value.integer.min;
		lmax = info->value.integer.max;
		lstep = info->value.integer.step;
		break;
	case SNDRV_CTL_ELEM_TYPE_INTEGER64:
		lval = control->value.integer64.value[i];
		lmin = info->value.integer64.min;
		lmax = info->value.integer64.max;
		lstep = info->value.integer64.step;
		break;
	case SNDRV_CTL_ELEM_TYPE_ENUMERATED:
		lval = control->value.enumerated.item[i];
		lmin = 0;
		lmax = info->value.enumerated.items - 1;
		lstep = 0;
		break;
	}

	if (lval < lmin || lval > lmax) {
		dev_err(card->dev,
			"control %i:%i:%i:%s:%i: value out of range %lld (%lld/%lld) at count %i\n",
			control->id.iface, control->id.device,
			control->id.subdevice, control->id.name,
			control->id.index, lval, lmin, lmax, i);
		return -EINVAL;
	}
	if (lstep) {
		div64_u64_rem(lval, lstep, &rem);
		if (rem) {
			dev_err(card->dev,
				"control %i:%i:%i:%s:%i: unaligned value %lld (step %lld) at count %i\n",
				control->id.iface, control->id.device,
				control->id.subdevice, control->id.name,
				control->id.index, lval, lstep, i);
			return -EINVAL;
		}
	}

	return 0;
}

/* perform sanity checks to the given snd_ctl_elem_value object */
static int sanity_check_elem_value(struct snd_card *card,
				   const struct snd_ctl_elem_value *control,
				   const struct snd_ctl_elem_info *info,
				   u32 pattern)
{
	size_t offset;
	int i, ret = 0;
	u32 *p;

	switch (info->type) {
	case SNDRV_CTL_ELEM_TYPE_BOOLEAN:
	case SNDRV_CTL_ELEM_TYPE_INTEGER:
	case SNDRV_CTL_ELEM_TYPE_INTEGER64:
	case SNDRV_CTL_ELEM_TYPE_ENUMERATED:
		for (i = 0; i < info->count; i++) {
			ret = sanity_check_int_value(card, control, info, i);
			if (ret < 0)
				return ret;
		}
		break;
	default:
		break;
	}

	/* check whether the remaining area kept untouched */
	offset = value_sizes[info->type] * info->count;
	offset = DIV_ROUND_UP(offset, sizeof(u32));
	p = (u32 *)control->value.bytes.data + offset;
	for (; offset < sizeof(control->value) / sizeof(u32); offset++, p++) {
		if (*p != pattern) {
			ret = -EINVAL;
			break;
		}
		*p = 0; /* clear the checked area */
	}

	return ret;
}
#else
static inline void fill_remaining_elem_value(struct snd_ctl_elem_value *control,
					     struct snd_ctl_elem_info *info,
					     u32 pattern)
{
}

static inline int sanity_check_elem_value(struct snd_card *card,
					  struct snd_ctl_elem_value *control,
					  struct snd_ctl_elem_info *info,
					  u32 pattern)
{
	return 0;
}
#endif

static int __snd_ctl_elem_info(struct snd_card *card,
			       struct snd_kcontrol *kctl,
			       struct snd_ctl_elem_info *info,
			       struct snd_ctl_file *ctl)
{
>>>>>>> upstream/android-13
	struct snd_kcontrol_volatile *vd;
	unsigned int index_offset;
	int result;

<<<<<<< HEAD
	down_read(&card->controls_rwsem);
	kctl = snd_ctl_find_id(card, &info->id);
	if (kctl == NULL) {
		up_read(&card->controls_rwsem);
		return -ENOENT;
	}
#ifdef CONFIG_SND_DEBUG
	info->access = 0;
#endif
	result = kctl->info(kctl, info);
=======
#ifdef CONFIG_SND_DEBUG
	info->access = 0;
#endif
	result = snd_power_ref_and_wait(card);
	if (!result)
		result = kctl->info(kctl, info);
	snd_power_unref(card);
>>>>>>> upstream/android-13
	if (result >= 0) {
		snd_BUG_ON(info->access);
		index_offset = snd_ctl_get_ioff(kctl, &info->id);
		vd = &kctl->vd[index_offset];
		snd_ctl_build_ioff(&info->id, kctl, index_offset);
		info->access = vd->access;
		if (vd->owner) {
			info->access |= SNDRV_CTL_ELEM_ACCESS_LOCK;
			if (vd->owner == ctl)
				info->access |= SNDRV_CTL_ELEM_ACCESS_OWNER;
			info->owner = pid_vnr(vd->owner->pid);
		} else {
			info->owner = -1;
		}
<<<<<<< HEAD
	}
=======
		if (!snd_ctl_skip_validation(info) &&
		    snd_ctl_check_elem_info(card, info) < 0)
			result = -EINVAL;
	}
	return result;
}

static int snd_ctl_elem_info(struct snd_ctl_file *ctl,
			     struct snd_ctl_elem_info *info)
{
	struct snd_card *card = ctl->card;
	struct snd_kcontrol *kctl;
	int result;

	down_read(&card->controls_rwsem);
	kctl = snd_ctl_find_id(card, &info->id);
	if (kctl == NULL)
		result = -ENOENT;
	else
		result = __snd_ctl_elem_info(card, kctl, info, ctl);
>>>>>>> upstream/android-13
	up_read(&card->controls_rwsem);
	return result;
}

static int snd_ctl_elem_info_user(struct snd_ctl_file *ctl,
				  struct snd_ctl_elem_info __user *_info)
{
	struct snd_ctl_elem_info info;
	int result;

	if (copy_from_user(&info, _info, sizeof(info)))
		return -EFAULT;
<<<<<<< HEAD
	result = snd_power_wait(ctl->card, SNDRV_CTL_POWER_D0);
	if (result < 0)
		return result;
	result = snd_ctl_elem_info(ctl, &info);
	if (result < 0)
		return result;
=======
	result = snd_ctl_elem_info(ctl, &info);
	if (result < 0)
		return result;
	/* drop internal access flags */
	info.access &= ~(SNDRV_CTL_ELEM_ACCESS_SKIP_CHECK|
			 SNDRV_CTL_ELEM_ACCESS_LED_MASK);
>>>>>>> upstream/android-13
	if (copy_to_user(_info, &info, sizeof(info)))
		return -EFAULT;
	return result;
}

static int snd_ctl_elem_read(struct snd_card *card,
			     struct snd_ctl_elem_value *control)
{
	struct snd_kcontrol *kctl;
	struct snd_kcontrol_volatile *vd;
	unsigned int index_offset;
<<<<<<< HEAD
	int ret;

	down_read(&card->controls_rwsem);
	kctl = snd_ctl_find_id(card, &control->id);
	if (kctl == NULL) {
		ret = -ENOENT;
		goto unlock;
	}

	index_offset = snd_ctl_get_ioff(kctl, &control->id);
	vd = &kctl->vd[index_offset];
	if (!(vd->access & SNDRV_CTL_ELEM_ACCESS_READ) || kctl->get == NULL) {
		ret = -EPERM;
		goto unlock;
	}

	snd_ctl_build_ioff(&control->id, kctl, index_offset);
	ret = kctl->get(kctl, control);
	if (ret < 0)
		goto unlock;
unlock:
	up_read(&card->controls_rwsem);
=======
	struct snd_ctl_elem_info info;
	const u32 pattern = 0xdeadbeef;
	int ret;

	kctl = snd_ctl_find_id(card, &control->id);
	if (kctl == NULL)
		return -ENOENT;

	index_offset = snd_ctl_get_ioff(kctl, &control->id);
	vd = &kctl->vd[index_offset];
	if (!(vd->access & SNDRV_CTL_ELEM_ACCESS_READ) || kctl->get == NULL)
		return -EPERM;

	snd_ctl_build_ioff(&control->id, kctl, index_offset);

#ifdef CONFIG_SND_CTL_VALIDATION
	/* info is needed only for validation */
	memset(&info, 0, sizeof(info));
	info.id = control->id;
	ret = __snd_ctl_elem_info(card, kctl, &info, NULL);
	if (ret < 0)
		return ret;
#endif

	if (!snd_ctl_skip_validation(&info))
		fill_remaining_elem_value(control, &info, pattern);
	ret = snd_power_ref_and_wait(card);
	if (!ret)
		ret = kctl->get(kctl, control);
	snd_power_unref(card);
	if (ret < 0)
		return ret;
	if (!snd_ctl_skip_validation(&info) &&
	    sanity_check_elem_value(card, control, &info, pattern) < 0) {
		dev_err(card->dev,
			"control %i:%i:%i:%s:%i: access overflow\n",
			control->id.iface, control->id.device,
			control->id.subdevice, control->id.name,
			control->id.index);
		return -EINVAL;
	}
>>>>>>> upstream/android-13
	return ret;
}

static int snd_ctl_elem_read_user(struct snd_card *card,
				  struct snd_ctl_elem_value __user *_control)
{
	struct snd_ctl_elem_value *control;
	int result;

	control = memdup_user(_control, sizeof(*control));
	if (IS_ERR(control))
		return PTR_ERR(control);

<<<<<<< HEAD
	result = snd_power_wait(card, SNDRV_CTL_POWER_D0);
	if (result < 0)
		goto error;

	result = snd_ctl_elem_read(card, control);
=======
	down_read(&card->controls_rwsem);
	result = snd_ctl_elem_read(card, control);
	up_read(&card->controls_rwsem);
>>>>>>> upstream/android-13
	if (result < 0)
		goto error;

	if (copy_to_user(_control, control, sizeof(*control)))
		result = -EFAULT;
 error:
	kfree(control);
	return result;
}

static int snd_ctl_elem_write(struct snd_card *card, struct snd_ctl_file *file,
			      struct snd_ctl_elem_value *control)
{
	struct snd_kcontrol *kctl;
	struct snd_kcontrol_volatile *vd;
	unsigned int index_offset;
	int result;

<<<<<<< HEAD
	kctl = snd_ctl_find_id(card, &control->id);
	if (kctl == NULL)
		return -ENOENT;
=======
	down_write(&card->controls_rwsem);
	kctl = snd_ctl_find_id(card, &control->id);
	if (kctl == NULL) {
		up_write(&card->controls_rwsem);
		return -ENOENT;
	}
>>>>>>> upstream/android-13

	index_offset = snd_ctl_get_ioff(kctl, &control->id);
	vd = &kctl->vd[index_offset];
	if (!(vd->access & SNDRV_CTL_ELEM_ACCESS_WRITE) || kctl->put == NULL ||
	    (file && vd->owner && vd->owner != file)) {
<<<<<<< HEAD
=======
		up_write(&card->controls_rwsem);
>>>>>>> upstream/android-13
		return -EPERM;
	}

	snd_ctl_build_ioff(&control->id, kctl, index_offset);
<<<<<<< HEAD
	result = kctl->put(kctl, control);
	if (result < 0)
		return result;

	if (result > 0) {
		struct snd_ctl_elem_id id = control->id;
		snd_ctl_notify(card, SNDRV_CTL_EVENT_MASK_VALUE, &id);
=======
	result = snd_power_ref_and_wait(card);
	if (!result)
		result = kctl->put(kctl, control);
	snd_power_unref(card);
	if (result < 0) {
		up_write(&card->controls_rwsem);
		return result;
	}

	if (result > 0) {
		downgrade_write(&card->controls_rwsem);
		snd_ctl_notify_one(card, SNDRV_CTL_EVENT_MASK_VALUE, kctl, index_offset);
		up_read(&card->controls_rwsem);
	} else {
		up_write(&card->controls_rwsem);
>>>>>>> upstream/android-13
	}

	return 0;
}

static int snd_ctl_elem_write_user(struct snd_ctl_file *file,
				   struct snd_ctl_elem_value __user *_control)
{
	struct snd_ctl_elem_value *control;
	struct snd_card *card;
	int result;

	control = memdup_user(_control, sizeof(*control));
	if (IS_ERR(control))
		return PTR_ERR(control);

	card = file->card;
<<<<<<< HEAD
	result = snd_power_wait(card, SNDRV_CTL_POWER_D0);
	if (result < 0)
		goto error;

	down_write(&card->controls_rwsem);
	result = snd_ctl_elem_write(card, file, control);
	up_write(&card->controls_rwsem);
=======
	result = snd_ctl_elem_write(card, file, control);
>>>>>>> upstream/android-13
	if (result < 0)
		goto error;

	if (copy_to_user(_control, control, sizeof(*control)))
		result = -EFAULT;
 error:
	kfree(control);
	return result;
}

static int snd_ctl_elem_lock(struct snd_ctl_file *file,
			     struct snd_ctl_elem_id __user *_id)
{
	struct snd_card *card = file->card;
	struct snd_ctl_elem_id id;
	struct snd_kcontrol *kctl;
	struct snd_kcontrol_volatile *vd;
	int result;

	if (copy_from_user(&id, _id, sizeof(id)))
		return -EFAULT;
	down_write(&card->controls_rwsem);
	kctl = snd_ctl_find_id(card, &id);
	if (kctl == NULL) {
		result = -ENOENT;
	} else {
		vd = &kctl->vd[snd_ctl_get_ioff(kctl, &id)];
		if (vd->owner != NULL)
			result = -EBUSY;
		else {
			vd->owner = file;
			result = 0;
		}
	}
	up_write(&card->controls_rwsem);
	return result;
}

static int snd_ctl_elem_unlock(struct snd_ctl_file *file,
			       struct snd_ctl_elem_id __user *_id)
{
	struct snd_card *card = file->card;
	struct snd_ctl_elem_id id;
	struct snd_kcontrol *kctl;
	struct snd_kcontrol_volatile *vd;
	int result;

	if (copy_from_user(&id, _id, sizeof(id)))
		return -EFAULT;
	down_write(&card->controls_rwsem);
	kctl = snd_ctl_find_id(card, &id);
	if (kctl == NULL) {
		result = -ENOENT;
	} else {
		vd = &kctl->vd[snd_ctl_get_ioff(kctl, &id)];
		if (vd->owner == NULL)
			result = -EINVAL;
		else if (vd->owner != file)
			result = -EPERM;
		else {
			vd->owner = NULL;
			result = 0;
		}
	}
	up_write(&card->controls_rwsem);
	return result;
}

struct user_element {
	struct snd_ctl_elem_info info;
	struct snd_card *card;
	char *elem_data;		/* element data */
	unsigned long elem_data_size;	/* size of element data in bytes */
	void *tlv_data;			/* TLV data */
	unsigned long tlv_data_size;	/* TLV data size */
	void *priv_data;		/* private data (like strings for enumerated type) */
};

<<<<<<< HEAD
=======
// check whether the addition (in bytes) of user ctl element may overflow the limit.
static bool check_user_elem_overflow(struct snd_card *card, ssize_t add)
{
	return (ssize_t)card->user_ctl_alloc_size + add > max_user_ctl_alloc_size;
}

>>>>>>> upstream/android-13
static int snd_ctl_elem_user_info(struct snd_kcontrol *kcontrol,
				  struct snd_ctl_elem_info *uinfo)
{
	struct user_element *ue = kcontrol->private_data;
	unsigned int offset;

	offset = snd_ctl_get_ioff(kcontrol, &uinfo->id);
	*uinfo = ue->info;
	snd_ctl_build_ioff(&uinfo->id, kcontrol, offset);

	return 0;
}

static int snd_ctl_elem_user_enum_info(struct snd_kcontrol *kcontrol,
				       struct snd_ctl_elem_info *uinfo)
{
	struct user_element *ue = kcontrol->private_data;
	const char *names;
	unsigned int item;
	unsigned int offset;

	item = uinfo->value.enumerated.item;

	offset = snd_ctl_get_ioff(kcontrol, &uinfo->id);
	*uinfo = ue->info;
	snd_ctl_build_ioff(&uinfo->id, kcontrol, offset);

	item = min(item, uinfo->value.enumerated.items - 1);
	uinfo->value.enumerated.item = item;

	names = ue->priv_data;
	for (; item > 0; --item)
		names += strlen(names) + 1;
	strcpy(uinfo->value.enumerated.name, names);

	return 0;
}

static int snd_ctl_elem_user_get(struct snd_kcontrol *kcontrol,
				 struct snd_ctl_elem_value *ucontrol)
{
	struct user_element *ue = kcontrol->private_data;
	unsigned int size = ue->elem_data_size;
	char *src = ue->elem_data +
			snd_ctl_get_ioff(kcontrol, &ucontrol->id) * size;

	memcpy(&ucontrol->value, src, size);
	return 0;
}

static int snd_ctl_elem_user_put(struct snd_kcontrol *kcontrol,
				 struct snd_ctl_elem_value *ucontrol)
{
	int change;
	struct user_element *ue = kcontrol->private_data;
	unsigned int size = ue->elem_data_size;
	char *dst = ue->elem_data +
			snd_ctl_get_ioff(kcontrol, &ucontrol->id) * size;

	change = memcmp(&ucontrol->value, dst, size) != 0;
	if (change)
		memcpy(dst, &ucontrol->value, size);
	return change;
}

<<<<<<< HEAD
=======
/* called in controls_rwsem write lock */
>>>>>>> upstream/android-13
static int replace_user_tlv(struct snd_kcontrol *kctl, unsigned int __user *buf,
			    unsigned int size)
{
	struct user_element *ue = kctl->private_data;
	unsigned int *container;
<<<<<<< HEAD
	struct snd_ctl_elem_id id;
=======
>>>>>>> upstream/android-13
	unsigned int mask = 0;
	int i;
	int change;

	if (size > 1024 * 128)	/* sane value */
		return -EINVAL;

<<<<<<< HEAD
=======
	// does the TLV size change cause overflow?
	if (check_user_elem_overflow(ue->card, (ssize_t)(size - ue->tlv_data_size)))
		return -ENOMEM;

>>>>>>> upstream/android-13
	container = vmemdup_user(buf, size);
	if (IS_ERR(container))
		return PTR_ERR(container);

	change = ue->tlv_data_size != size;
	if (!change)
		change = memcmp(ue->tlv_data, container, size) != 0;
	if (!change) {
		kvfree(container);
		return 0;
	}

	if (ue->tlv_data == NULL) {
		/* Now TLV data is available. */
		for (i = 0; i < kctl->count; ++i)
			kctl->vd[i].access |= SNDRV_CTL_ELEM_ACCESS_TLV_READ;
		mask = SNDRV_CTL_EVENT_MASK_INFO;
<<<<<<< HEAD
	}

	kvfree(ue->tlv_data);
	ue->tlv_data = container;
	ue->tlv_data_size = size;

	mask |= SNDRV_CTL_EVENT_MASK_TLV;
	for (i = 0; i < kctl->count; ++i) {
		snd_ctl_build_ioff(&id, kctl, i);
		snd_ctl_notify(ue->card, mask, &id);
	}
=======
	} else {
		ue->card->user_ctl_alloc_size -= ue->tlv_data_size;
		ue->tlv_data_size = 0;
		kvfree(ue->tlv_data);
	}

	ue->tlv_data = container;
	ue->tlv_data_size = size;
	// decremented at private_free.
	ue->card->user_ctl_alloc_size += size;

	mask |= SNDRV_CTL_EVENT_MASK_TLV;
	for (i = 0; i < kctl->count; ++i)
		snd_ctl_notify_one(ue->card, mask, kctl, i);
>>>>>>> upstream/android-13

	return change;
}

static int read_user_tlv(struct snd_kcontrol *kctl, unsigned int __user *buf,
			 unsigned int size)
{
	struct user_element *ue = kctl->private_data;

	if (ue->tlv_data_size == 0 || ue->tlv_data == NULL)
		return -ENXIO;

	if (size < ue->tlv_data_size)
		return -ENOSPC;

	if (copy_to_user(buf, ue->tlv_data, ue->tlv_data_size))
		return -EFAULT;

	return 0;
}

static int snd_ctl_elem_user_tlv(struct snd_kcontrol *kctl, int op_flag,
				 unsigned int size, unsigned int __user *buf)
{
	if (op_flag == SNDRV_CTL_TLV_OP_WRITE)
		return replace_user_tlv(kctl, buf, size);
	else
		return read_user_tlv(kctl, buf, size);
}

<<<<<<< HEAD
=======
/* called in controls_rwsem write lock */
>>>>>>> upstream/android-13
static int snd_ctl_elem_init_enum_names(struct user_element *ue)
{
	char *names, *p;
	size_t buf_len, name_len;
	unsigned int i;
	const uintptr_t user_ptrval = ue->info.value.enumerated.names_ptr;

<<<<<<< HEAD
	if (ue->info.value.enumerated.names_length > 64 * 1024)
		return -EINVAL;

	names = vmemdup_user((const void __user *)user_ptrval,
		ue->info.value.enumerated.names_length);
=======
	buf_len = ue->info.value.enumerated.names_length;
	if (buf_len > 64 * 1024)
		return -EINVAL;

	if (check_user_elem_overflow(ue->card, buf_len))
		return -ENOMEM;
	names = vmemdup_user((const void __user *)user_ptrval, buf_len);
>>>>>>> upstream/android-13
	if (IS_ERR(names))
		return PTR_ERR(names);

	/* check that there are enough valid names */
<<<<<<< HEAD
	buf_len = ue->info.value.enumerated.names_length;
=======
>>>>>>> upstream/android-13
	p = names;
	for (i = 0; i < ue->info.value.enumerated.items; ++i) {
		name_len = strnlen(p, buf_len);
		if (name_len == 0 || name_len >= 64 || name_len == buf_len) {
			kvfree(names);
			return -EINVAL;
		}
		p += name_len + 1;
		buf_len -= name_len + 1;
	}

	ue->priv_data = names;
	ue->info.value.enumerated.names_ptr = 0;
<<<<<<< HEAD
=======
	// increment the allocation size; decremented again at private_free.
	ue->card->user_ctl_alloc_size += ue->info.value.enumerated.names_length;
>>>>>>> upstream/android-13

	return 0;
}

<<<<<<< HEAD
=======
static size_t compute_user_elem_size(size_t size, unsigned int count)
{
	return sizeof(struct user_element) + size * count;
}

>>>>>>> upstream/android-13
static void snd_ctl_elem_user_free(struct snd_kcontrol *kcontrol)
{
	struct user_element *ue = kcontrol->private_data;

<<<<<<< HEAD
=======
	// decrement the allocation size.
	ue->card->user_ctl_alloc_size -= compute_user_elem_size(ue->elem_data_size, kcontrol->count);
	ue->card->user_ctl_alloc_size -= ue->tlv_data_size;
	if (ue->priv_data)
		ue->card->user_ctl_alloc_size -= ue->info.value.enumerated.names_length;

>>>>>>> upstream/android-13
	kvfree(ue->tlv_data);
	kvfree(ue->priv_data);
	kfree(ue);
}

static int snd_ctl_elem_add(struct snd_ctl_file *file,
			    struct snd_ctl_elem_info *info, int replace)
{
<<<<<<< HEAD
	/* The capacity of struct snd_ctl_elem_value.value.*/
	static const unsigned int value_sizes[] = {
		[SNDRV_CTL_ELEM_TYPE_BOOLEAN]	= sizeof(long),
		[SNDRV_CTL_ELEM_TYPE_INTEGER]	= sizeof(long),
		[SNDRV_CTL_ELEM_TYPE_ENUMERATED] = sizeof(unsigned int),
		[SNDRV_CTL_ELEM_TYPE_BYTES]	= sizeof(unsigned char),
		[SNDRV_CTL_ELEM_TYPE_IEC958]	= sizeof(struct snd_aes_iec958),
		[SNDRV_CTL_ELEM_TYPE_INTEGER64] = sizeof(long long),
	};
	static const unsigned int max_value_counts[] = {
		[SNDRV_CTL_ELEM_TYPE_BOOLEAN]	= 128,
		[SNDRV_CTL_ELEM_TYPE_INTEGER]	= 128,
		[SNDRV_CTL_ELEM_TYPE_ENUMERATED] = 128,
		[SNDRV_CTL_ELEM_TYPE_BYTES]	= 512,
		[SNDRV_CTL_ELEM_TYPE_IEC958]	= 1,
		[SNDRV_CTL_ELEM_TYPE_INTEGER64] = 64,
	};
=======
>>>>>>> upstream/android-13
	struct snd_card *card = file->card;
	struct snd_kcontrol *kctl;
	unsigned int count;
	unsigned int access;
	long private_size;
<<<<<<< HEAD
=======
	size_t alloc_size;
>>>>>>> upstream/android-13
	struct user_element *ue;
	unsigned int offset;
	int err;

	if (!*info->id.name)
		return -EINVAL;
	if (strnlen(info->id.name, sizeof(info->id.name)) >= sizeof(info->id.name))
		return -EINVAL;

	/* Delete a control to replace them if needed. */
	if (replace) {
		info->id.numid = 0;
		err = snd_ctl_remove_user_ctl(file, &info->id);
		if (err)
			return err;
	}

<<<<<<< HEAD
	/*
	 * The number of userspace controls are counted control by control,
	 * not element by element.
	 */
	if (card->user_ctl_count + 1 > MAX_USER_CONTROLS)
		return -ENOMEM;

=======
>>>>>>> upstream/android-13
	/* Check the number of elements for this userspace control. */
	count = info->owner;
	if (count == 0)
		count = 1;

	/* Arrange access permissions if needed. */
	access = info->access;
	if (access == 0)
		access = SNDRV_CTL_ELEM_ACCESS_READWRITE;
	access &= (SNDRV_CTL_ELEM_ACCESS_READWRITE |
		   SNDRV_CTL_ELEM_ACCESS_INACTIVE |
		   SNDRV_CTL_ELEM_ACCESS_TLV_WRITE);

	/* In initial state, nothing is available as TLV container. */
	if (access & SNDRV_CTL_ELEM_ACCESS_TLV_WRITE)
		access |= SNDRV_CTL_ELEM_ACCESS_TLV_CALLBACK;
	access |= SNDRV_CTL_ELEM_ACCESS_USER;

	/*
	 * Check information and calculate the size of data specific to
	 * this userspace control.
	 */
<<<<<<< HEAD
	if (info->type < SNDRV_CTL_ELEM_TYPE_BOOLEAN ||
	    info->type > SNDRV_CTL_ELEM_TYPE_INTEGER64)
		return -EINVAL;
	if (info->type == SNDRV_CTL_ELEM_TYPE_ENUMERATED &&
	    info->value.enumerated.items == 0)
		return -EINVAL;
	if (info->count < 1 ||
	    info->count > max_value_counts[info->type])
		return -EINVAL;
	if (!validate_element_member_dimension(info))
		return -EINVAL;
	private_size = value_sizes[info->type] * info->count;
=======
	/* pass NULL to card for suppressing error messages */
	err = snd_ctl_check_elem_info(NULL, info);
	if (err < 0)
		return err;
	/* user-space control doesn't allow zero-size data */
	if (info->count < 1)
		return -EINVAL;
	private_size = value_sizes[info->type] * info->count;
	alloc_size = compute_user_elem_size(private_size, count);

	down_write(&card->controls_rwsem);
	if (check_user_elem_overflow(card, alloc_size)) {
		err = -ENOMEM;
		goto unlock;
	}
>>>>>>> upstream/android-13

	/*
	 * Keep memory object for this userspace control. After passing this
	 * code block, the instance should be freed by snd_ctl_free_one().
	 *
	 * Note that these elements in this control are locked.
	 */
	err = snd_ctl_new(&kctl, count, access, file);
	if (err < 0)
<<<<<<< HEAD
		return err;
	memcpy(&kctl->id, &info->id, sizeof(kctl->id));
	kctl->private_data = kzalloc(sizeof(struct user_element) + private_size * count,
				     GFP_KERNEL);
	if (kctl->private_data == NULL) {
		kfree(kctl);
		return -ENOMEM;
	}
	kctl->private_free = snd_ctl_elem_user_free;

	/* Set private data for this userspace control. */
	ue = (struct user_element *)kctl->private_data;
=======
		goto unlock;
	memcpy(&kctl->id, &info->id, sizeof(kctl->id));
	ue = kzalloc(alloc_size, GFP_KERNEL);
	if (!ue) {
		kfree(kctl);
		err = -ENOMEM;
		goto unlock;
	}
	kctl->private_data = ue;
	kctl->private_free = snd_ctl_elem_user_free;

	// increment the allocated size; decremented again at private_free.
	card->user_ctl_alloc_size += alloc_size;

	/* Set private data for this userspace control. */
>>>>>>> upstream/android-13
	ue->card = card;
	ue->info = *info;
	ue->info.access = 0;
	ue->elem_data = (char *)ue + sizeof(*ue);
	ue->elem_data_size = private_size;
	if (ue->info.type == SNDRV_CTL_ELEM_TYPE_ENUMERATED) {
		err = snd_ctl_elem_init_enum_names(ue);
		if (err < 0) {
			snd_ctl_free_one(kctl);
<<<<<<< HEAD
			return err;
=======
			goto unlock;
>>>>>>> upstream/android-13
		}
	}

	/* Set callback functions. */
	if (info->type == SNDRV_CTL_ELEM_TYPE_ENUMERATED)
		kctl->info = snd_ctl_elem_user_enum_info;
	else
		kctl->info = snd_ctl_elem_user_info;
	if (access & SNDRV_CTL_ELEM_ACCESS_READ)
		kctl->get = snd_ctl_elem_user_get;
	if (access & SNDRV_CTL_ELEM_ACCESS_WRITE)
		kctl->put = snd_ctl_elem_user_put;
	if (access & SNDRV_CTL_ELEM_ACCESS_TLV_WRITE)
		kctl->tlv.c = snd_ctl_elem_user_tlv;

	/* This function manage to free the instance on failure. */
<<<<<<< HEAD
	down_write(&card->controls_rwsem);
	err = __snd_ctl_add(card, kctl);
=======
	err = __snd_ctl_add_replace(card, kctl, CTL_ADD_EXCLUSIVE);
>>>>>>> upstream/android-13
	if (err < 0) {
		snd_ctl_free_one(kctl);
		goto unlock;
	}
	offset = snd_ctl_get_ioff(kctl, &info->id);
	snd_ctl_build_ioff(&info->id, kctl, offset);
	/*
	 * Here we cannot fill any field for the number of elements added by
	 * this operation because there're no specific fields. The usage of
	 * 'owner' field for this purpose may cause any bugs to userspace
	 * applications because the field originally means PID of a process
	 * which locks the element.
	 */
<<<<<<< HEAD

	card->user_ctl_count++;

=======
>>>>>>> upstream/android-13
 unlock:
	up_write(&card->controls_rwsem);
	return err;
}

static int snd_ctl_elem_add_user(struct snd_ctl_file *file,
				 struct snd_ctl_elem_info __user *_info, int replace)
{
	struct snd_ctl_elem_info info;
	int err;

	if (copy_from_user(&info, _info, sizeof(info)))
		return -EFAULT;
	err = snd_ctl_elem_add(file, &info, replace);
	if (err < 0)
		return err;
	if (copy_to_user(_info, &info, sizeof(info))) {
		snd_ctl_remove_user_ctl(file, &info.id);
		return -EFAULT;
	}

	return 0;
}

static int snd_ctl_elem_remove(struct snd_ctl_file *file,
			       struct snd_ctl_elem_id __user *_id)
{
	struct snd_ctl_elem_id id;

	if (copy_from_user(&id, _id, sizeof(id)))
		return -EFAULT;
	return snd_ctl_remove_user_ctl(file, &id);
}

static int snd_ctl_subscribe_events(struct snd_ctl_file *file, int __user *ptr)
{
	int subscribe;
	if (get_user(subscribe, ptr))
		return -EFAULT;
	if (subscribe < 0) {
		subscribe = file->subscribed;
		if (put_user(subscribe, ptr))
			return -EFAULT;
		return 0;
	}
	if (subscribe) {
		file->subscribed = 1;
		return 0;
	} else if (file->subscribed) {
		snd_ctl_empty_read_queue(file);
		file->subscribed = 0;
	}
	return 0;
}

static int call_tlv_handler(struct snd_ctl_file *file, int op_flag,
			    struct snd_kcontrol *kctl,
			    struct snd_ctl_elem_id *id,
			    unsigned int __user *buf, unsigned int size)
{
	static const struct {
		int op;
		int perm;
	} pairs[] = {
		{SNDRV_CTL_TLV_OP_READ,  SNDRV_CTL_ELEM_ACCESS_TLV_READ},
		{SNDRV_CTL_TLV_OP_WRITE, SNDRV_CTL_ELEM_ACCESS_TLV_WRITE},
		{SNDRV_CTL_TLV_OP_CMD,   SNDRV_CTL_ELEM_ACCESS_TLV_COMMAND},
	};
	struct snd_kcontrol_volatile *vd = &kctl->vd[snd_ctl_get_ioff(kctl, id)];
<<<<<<< HEAD
	int i;
=======
	int i, ret;
>>>>>>> upstream/android-13

	/* Check support of the request for this element. */
	for (i = 0; i < ARRAY_SIZE(pairs); ++i) {
		if (op_flag == pairs[i].op && (vd->access & pairs[i].perm))
			break;
	}
	if (i == ARRAY_SIZE(pairs))
		return -ENXIO;

	if (kctl->tlv.c == NULL)
		return -ENXIO;

	/* Write and command operations are not allowed for locked element. */
	if (op_flag != SNDRV_CTL_TLV_OP_READ &&
	    vd->owner != NULL && vd->owner != file)
		return -EPERM;

<<<<<<< HEAD
	return kctl->tlv.c(kctl, op_flag, size, buf);
=======
	ret = snd_power_ref_and_wait(file->card);
	if (!ret)
		ret = kctl->tlv.c(kctl, op_flag, size, buf);
	snd_power_unref(file->card);
	return ret;
>>>>>>> upstream/android-13
}

static int read_tlv_buf(struct snd_kcontrol *kctl, struct snd_ctl_elem_id *id,
			unsigned int __user *buf, unsigned int size)
{
	struct snd_kcontrol_volatile *vd = &kctl->vd[snd_ctl_get_ioff(kctl, id)];
	unsigned int len;

	if (!(vd->access & SNDRV_CTL_ELEM_ACCESS_TLV_READ))
		return -ENXIO;

	if (kctl->tlv.p == NULL)
		return -ENXIO;

	len = sizeof(unsigned int) * 2 + kctl->tlv.p[1];
	if (size < len)
		return -ENOMEM;

	if (copy_to_user(buf, kctl->tlv.p, len))
		return -EFAULT;

	return 0;
}

static int snd_ctl_tlv_ioctl(struct snd_ctl_file *file,
			     struct snd_ctl_tlv __user *buf,
                             int op_flag)
{
	struct snd_ctl_tlv header;
	unsigned int __user *container;
	unsigned int container_size;
	struct snd_kcontrol *kctl;
	struct snd_ctl_elem_id id;
	struct snd_kcontrol_volatile *vd;

	if (copy_from_user(&header, buf, sizeof(header)))
		return -EFAULT;

	/* In design of control core, numerical ID starts at 1. */
	if (header.numid == 0)
		return -EINVAL;

	/* At least, container should include type and length fields.  */
	if (header.length < sizeof(unsigned int) * 2)
		return -EINVAL;
	container_size = header.length;
	container = buf->tlv;

	kctl = snd_ctl_find_numid(file->card, header.numid);
	if (kctl == NULL)
		return -ENOENT;

	/* Calculate index of the element in this set. */
	id = kctl->id;
	snd_ctl_build_ioff(&id, kctl, header.numid - id.numid);
	vd = &kctl->vd[snd_ctl_get_ioff(kctl, &id)];

	if (vd->access & SNDRV_CTL_ELEM_ACCESS_TLV_CALLBACK) {
		return call_tlv_handler(file, op_flag, kctl, &id, container,
					container_size);
	} else {
		if (op_flag == SNDRV_CTL_TLV_OP_READ) {
			return read_tlv_buf(kctl, &id, container,
					    container_size);
		}
	}

	/* Not supported. */
	return -ENXIO;
}

static long snd_ctl_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
	struct snd_ctl_file *ctl;
	struct snd_card *card;
	struct snd_kctl_ioctl *p;
	void __user *argp = (void __user *)arg;
	int __user *ip = argp;
	int err;

	ctl = file->private_data;
	card = ctl->card;
	if (snd_BUG_ON(!card))
		return -ENXIO;
	switch (cmd) {
	case SNDRV_CTL_IOCTL_PVERSION:
		return put_user(SNDRV_CTL_VERSION, ip) ? -EFAULT : 0;
	case SNDRV_CTL_IOCTL_CARD_INFO:
		return snd_ctl_card_info(card, ctl, cmd, argp);
	case SNDRV_CTL_IOCTL_ELEM_LIST:
<<<<<<< HEAD
		return snd_ctl_elem_list(card, argp);
=======
		return snd_ctl_elem_list_user(card, argp);
>>>>>>> upstream/android-13
	case SNDRV_CTL_IOCTL_ELEM_INFO:
		return snd_ctl_elem_info_user(ctl, argp);
	case SNDRV_CTL_IOCTL_ELEM_READ:
		return snd_ctl_elem_read_user(card, argp);
	case SNDRV_CTL_IOCTL_ELEM_WRITE:
		return snd_ctl_elem_write_user(ctl, argp);
	case SNDRV_CTL_IOCTL_ELEM_LOCK:
		return snd_ctl_elem_lock(ctl, argp);
	case SNDRV_CTL_IOCTL_ELEM_UNLOCK:
		return snd_ctl_elem_unlock(ctl, argp);
	case SNDRV_CTL_IOCTL_ELEM_ADD:
		return snd_ctl_elem_add_user(ctl, argp, 0);
	case SNDRV_CTL_IOCTL_ELEM_REPLACE:
		return snd_ctl_elem_add_user(ctl, argp, 1);
	case SNDRV_CTL_IOCTL_ELEM_REMOVE:
		return snd_ctl_elem_remove(ctl, argp);
	case SNDRV_CTL_IOCTL_SUBSCRIBE_EVENTS:
		return snd_ctl_subscribe_events(ctl, ip);
	case SNDRV_CTL_IOCTL_TLV_READ:
		down_read(&ctl->card->controls_rwsem);
		err = snd_ctl_tlv_ioctl(ctl, argp, SNDRV_CTL_TLV_OP_READ);
		up_read(&ctl->card->controls_rwsem);
		return err;
	case SNDRV_CTL_IOCTL_TLV_WRITE:
		down_write(&ctl->card->controls_rwsem);
		err = snd_ctl_tlv_ioctl(ctl, argp, SNDRV_CTL_TLV_OP_WRITE);
		up_write(&ctl->card->controls_rwsem);
		return err;
	case SNDRV_CTL_IOCTL_TLV_COMMAND:
		down_write(&ctl->card->controls_rwsem);
		err = snd_ctl_tlv_ioctl(ctl, argp, SNDRV_CTL_TLV_OP_CMD);
		up_write(&ctl->card->controls_rwsem);
		return err;
	case SNDRV_CTL_IOCTL_POWER:
		return -ENOPROTOOPT;
	case SNDRV_CTL_IOCTL_POWER_STATE:
<<<<<<< HEAD
#ifdef CONFIG_PM
		return put_user(card->power_state, ip) ? -EFAULT : 0;
#else
		return put_user(SNDRV_CTL_POWER_D0, ip) ? -EFAULT : 0;
#endif
=======
		return put_user(SNDRV_CTL_POWER_D0, ip) ? -EFAULT : 0;
>>>>>>> upstream/android-13
	}
	down_read(&snd_ioctl_rwsem);
	list_for_each_entry(p, &snd_control_ioctls, list) {
		err = p->fioctl(card, ctl, cmd, arg);
		if (err != -ENOIOCTLCMD) {
			up_read(&snd_ioctl_rwsem);
			return err;
		}
	}
	up_read(&snd_ioctl_rwsem);
	dev_dbg(card->dev, "unknown ioctl = 0x%x\n", cmd);
	return -ENOTTY;
}

static ssize_t snd_ctl_read(struct file *file, char __user *buffer,
			    size_t count, loff_t * offset)
{
	struct snd_ctl_file *ctl;
	int err = 0;
	ssize_t result = 0;

	ctl = file->private_data;
	if (snd_BUG_ON(!ctl || !ctl->card))
		return -ENXIO;
	if (!ctl->subscribed)
		return -EBADFD;
	if (count < sizeof(struct snd_ctl_event))
		return -EINVAL;
	spin_lock_irq(&ctl->read_lock);
	while (count >= sizeof(struct snd_ctl_event)) {
		struct snd_ctl_event ev;
		struct snd_kctl_event *kev;
		while (list_empty(&ctl->events)) {
			wait_queue_entry_t wait;
			if ((file->f_flags & O_NONBLOCK) != 0 || result > 0) {
				err = -EAGAIN;
				goto __end_lock;
			}
			init_waitqueue_entry(&wait, current);
			add_wait_queue(&ctl->change_sleep, &wait);
			set_current_state(TASK_INTERRUPTIBLE);
			spin_unlock_irq(&ctl->read_lock);
			schedule();
			remove_wait_queue(&ctl->change_sleep, &wait);
			if (ctl->card->shutdown)
				return -ENODEV;
			if (signal_pending(current))
				return -ERESTARTSYS;
			spin_lock_irq(&ctl->read_lock);
		}
		kev = snd_kctl_event(ctl->events.next);
		ev.type = SNDRV_CTL_EVENT_ELEM;
		ev.data.elem.mask = kev->mask;
		ev.data.elem.id = kev->id;
		list_del(&kev->list);
		spin_unlock_irq(&ctl->read_lock);
		kfree(kev);
		if (copy_to_user(buffer, &ev, sizeof(struct snd_ctl_event))) {
			err = -EFAULT;
			goto __end;
		}
		spin_lock_irq(&ctl->read_lock);
		buffer += sizeof(struct snd_ctl_event);
		count -= sizeof(struct snd_ctl_event);
		result += sizeof(struct snd_ctl_event);
	}
      __end_lock:
	spin_unlock_irq(&ctl->read_lock);
      __end:
      	return result > 0 ? result : err;
}

static __poll_t snd_ctl_poll(struct file *file, poll_table * wait)
{
	__poll_t mask;
	struct snd_ctl_file *ctl;

	ctl = file->private_data;
	if (!ctl->subscribed)
		return 0;
	poll_wait(file, &ctl->change_sleep, wait);

	mask = 0;
	if (!list_empty(&ctl->events))
		mask |= EPOLLIN | EPOLLRDNORM;

	return mask;
}

/*
 * register the device-specific control-ioctls.
 * called from each device manager like pcm.c, hwdep.c, etc.
 */
static int _snd_ctl_register_ioctl(snd_kctl_ioctl_func_t fcn, struct list_head *lists)
{
	struct snd_kctl_ioctl *pn;

	pn = kzalloc(sizeof(struct snd_kctl_ioctl), GFP_KERNEL);
	if (pn == NULL)
		return -ENOMEM;
	pn->fioctl = fcn;
	down_write(&snd_ioctl_rwsem);
	list_add_tail(&pn->list, lists);
	up_write(&snd_ioctl_rwsem);
	return 0;
}

/**
 * snd_ctl_register_ioctl - register the device-specific control-ioctls
 * @fcn: ioctl callback function
 *
 * called from each device manager like pcm.c, hwdep.c, etc.
 */
int snd_ctl_register_ioctl(snd_kctl_ioctl_func_t fcn)
{
	return _snd_ctl_register_ioctl(fcn, &snd_control_ioctls);
}
EXPORT_SYMBOL(snd_ctl_register_ioctl);

#ifdef CONFIG_COMPAT
/**
 * snd_ctl_register_ioctl_compat - register the device-specific 32bit compat
 * control-ioctls
 * @fcn: ioctl callback function
 */
int snd_ctl_register_ioctl_compat(snd_kctl_ioctl_func_t fcn)
{
	return _snd_ctl_register_ioctl(fcn, &snd_control_compat_ioctls);
}
EXPORT_SYMBOL(snd_ctl_register_ioctl_compat);
#endif

/*
 * de-register the device-specific control-ioctls.
 */
static int _snd_ctl_unregister_ioctl(snd_kctl_ioctl_func_t fcn,
				     struct list_head *lists)
{
	struct snd_kctl_ioctl *p;

	if (snd_BUG_ON(!fcn))
		return -EINVAL;
	down_write(&snd_ioctl_rwsem);
	list_for_each_entry(p, lists, list) {
		if (p->fioctl == fcn) {
			list_del(&p->list);
			up_write(&snd_ioctl_rwsem);
			kfree(p);
			return 0;
		}
	}
	up_write(&snd_ioctl_rwsem);
	snd_BUG();
	return -EINVAL;
}

/**
 * snd_ctl_unregister_ioctl - de-register the device-specific control-ioctls
 * @fcn: ioctl callback function to unregister
 */
int snd_ctl_unregister_ioctl(snd_kctl_ioctl_func_t fcn)
{
	return _snd_ctl_unregister_ioctl(fcn, &snd_control_ioctls);
}
EXPORT_SYMBOL(snd_ctl_unregister_ioctl);

#ifdef CONFIG_COMPAT
/**
<<<<<<< HEAD
 * snd_ctl_unregister_ioctl - de-register the device-specific compat 32bit
 * control-ioctls
=======
 * snd_ctl_unregister_ioctl_compat - de-register the device-specific compat
 * 32bit control-ioctls
>>>>>>> upstream/android-13
 * @fcn: ioctl callback function to unregister
 */
int snd_ctl_unregister_ioctl_compat(snd_kctl_ioctl_func_t fcn)
{
	return _snd_ctl_unregister_ioctl(fcn, &snd_control_compat_ioctls);
}
EXPORT_SYMBOL(snd_ctl_unregister_ioctl_compat);
#endif

static int snd_ctl_fasync(int fd, struct file * file, int on)
{
	struct snd_ctl_file *ctl;

	ctl = file->private_data;
	return fasync_helper(fd, file, on, &ctl->fasync);
}

/* return the preferred subdevice number if already assigned;
 * otherwise return -1
 */
int snd_ctl_get_preferred_subdevice(struct snd_card *card, int type)
{
	struct snd_ctl_file *kctl;
	int subdevice = -1;
<<<<<<< HEAD

	read_lock(&card->ctl_files_rwlock);
=======
	unsigned long flags;

	read_lock_irqsave(&card->ctl_files_rwlock, flags);
>>>>>>> upstream/android-13
	list_for_each_entry(kctl, &card->ctl_files, list) {
		if (kctl->pid == task_pid(current)) {
			subdevice = kctl->preferred_subdevice[type];
			if (subdevice != -1)
				break;
		}
	}
<<<<<<< HEAD
	read_unlock(&card->ctl_files_rwlock);
=======
	read_unlock_irqrestore(&card->ctl_files_rwlock, flags);
>>>>>>> upstream/android-13
	return subdevice;
}
EXPORT_SYMBOL_GPL(snd_ctl_get_preferred_subdevice);

/*
 * ioctl32 compat
 */
#ifdef CONFIG_COMPAT
#include "control_compat.c"
#else
#define snd_ctl_ioctl_compat	NULL
#endif

/*
<<<<<<< HEAD
=======
 * control layers (audio LED etc.)
 */

/**
 * snd_ctl_request_layer - request to use the layer
 * @module_name: Name of the kernel module (NULL == build-in)
 *
 * Return an error code when the module cannot be loaded.
 */
int snd_ctl_request_layer(const char *module_name)
{
	struct snd_ctl_layer_ops *lops;

	if (module_name == NULL)
		return 0;
	down_read(&snd_ctl_layer_rwsem);
	for (lops = snd_ctl_layer; lops; lops = lops->next)
		if (strcmp(lops->module_name, module_name) == 0)
			break;
	up_read(&snd_ctl_layer_rwsem);
	if (lops)
		return 0;
	return request_module(module_name);
}
EXPORT_SYMBOL_GPL(snd_ctl_request_layer);

/**
 * snd_ctl_register_layer - register new control layer
 * @lops: operation structure
 *
 * The new layer can track all control elements and do additional
 * operations on top (like audio LED handling).
 */
void snd_ctl_register_layer(struct snd_ctl_layer_ops *lops)
{
	struct snd_card *card;
	int card_number;

	down_write(&snd_ctl_layer_rwsem);
	lops->next = snd_ctl_layer;
	snd_ctl_layer = lops;
	up_write(&snd_ctl_layer_rwsem);
	for (card_number = 0; card_number < SNDRV_CARDS; card_number++) {
		card = snd_card_ref(card_number);
		if (card) {
			down_read(&card->controls_rwsem);
			lops->lregister(card);
			up_read(&card->controls_rwsem);
			snd_card_unref(card);
		}
	}
}
EXPORT_SYMBOL_GPL(snd_ctl_register_layer);

/**
 * snd_ctl_disconnect_layer - disconnect control layer
 * @lops: operation structure
 *
 * It is expected that the information about tracked cards
 * is freed before this call (the disconnect callback is
 * not called here).
 */
void snd_ctl_disconnect_layer(struct snd_ctl_layer_ops *lops)
{
	struct snd_ctl_layer_ops *lops2, *prev_lops2;

	down_write(&snd_ctl_layer_rwsem);
	for (lops2 = snd_ctl_layer, prev_lops2 = NULL; lops2; lops2 = lops2->next) {
		if (lops2 == lops) {
			if (!prev_lops2)
				snd_ctl_layer = lops->next;
			else
				prev_lops2->next = lops->next;
			break;
		}
		prev_lops2 = lops2;
	}
	up_write(&snd_ctl_layer_rwsem);
}
EXPORT_SYMBOL_GPL(snd_ctl_disconnect_layer);

/*
>>>>>>> upstream/android-13
 *  INIT PART
 */

static const struct file_operations snd_ctl_f_ops =
{
	.owner =	THIS_MODULE,
	.read =		snd_ctl_read,
	.open =		snd_ctl_open,
	.release =	snd_ctl_release,
	.llseek =	no_llseek,
	.poll =		snd_ctl_poll,
	.unlocked_ioctl =	snd_ctl_ioctl,
	.compat_ioctl =	snd_ctl_ioctl_compat,
	.fasync =	snd_ctl_fasync,
};

/*
 * registration of the control device
 */
static int snd_ctl_dev_register(struct snd_device *device)
{
	struct snd_card *card = device->device_data;
<<<<<<< HEAD

	return snd_register_device(SNDRV_DEVICE_TYPE_CONTROL, card, -1,
				   &snd_ctl_f_ops, card, &card->ctl_dev);
=======
	struct snd_ctl_layer_ops *lops;
	int err;

	err = snd_register_device(SNDRV_DEVICE_TYPE_CONTROL, card, -1,
				  &snd_ctl_f_ops, card, &card->ctl_dev);
	if (err < 0)
		return err;
	down_read(&card->controls_rwsem);
	down_read(&snd_ctl_layer_rwsem);
	for (lops = snd_ctl_layer; lops; lops = lops->next)
		lops->lregister(card);
	up_read(&snd_ctl_layer_rwsem);
	up_read(&card->controls_rwsem);
	return 0;
>>>>>>> upstream/android-13
}

/*
 * disconnection of the control device
 */
static int snd_ctl_dev_disconnect(struct snd_device *device)
{
	struct snd_card *card = device->device_data;
	struct snd_ctl_file *ctl;
<<<<<<< HEAD

	read_lock(&card->ctl_files_rwlock);
=======
	struct snd_ctl_layer_ops *lops;
	unsigned long flags;

	read_lock_irqsave(&card->ctl_files_rwlock, flags);
>>>>>>> upstream/android-13
	list_for_each_entry(ctl, &card->ctl_files, list) {
		wake_up(&ctl->change_sleep);
		kill_fasync(&ctl->fasync, SIGIO, POLL_ERR);
	}
<<<<<<< HEAD
	read_unlock(&card->ctl_files_rwlock);
=======
	read_unlock_irqrestore(&card->ctl_files_rwlock, flags);

	down_read(&card->controls_rwsem);
	down_read(&snd_ctl_layer_rwsem);
	for (lops = snd_ctl_layer; lops; lops = lops->next)
		lops->ldisconnect(card);
	up_read(&snd_ctl_layer_rwsem);
	up_read(&card->controls_rwsem);
>>>>>>> upstream/android-13

	return snd_unregister_device(&card->ctl_dev);
}

/*
 * free all controls
 */
static int snd_ctl_dev_free(struct snd_device *device)
{
	struct snd_card *card = device->device_data;
	struct snd_kcontrol *control;

	down_write(&card->controls_rwsem);
	while (!list_empty(&card->controls)) {
		control = snd_kcontrol(card->controls.next);
<<<<<<< HEAD
		snd_ctl_remove(card, control);
	}
=======
		__snd_ctl_remove(card, control, false);
	}

#ifdef CONFIG_SND_CTL_FAST_LOOKUP
	xa_destroy(&card->ctl_numids);
	xa_destroy(&card->ctl_hash);
#endif
>>>>>>> upstream/android-13
	up_write(&card->controls_rwsem);
	put_device(&card->ctl_dev);
	return 0;
}

/*
 * create control core:
 * called from init.c
 */
int snd_ctl_create(struct snd_card *card)
{
<<<<<<< HEAD
	static struct snd_device_ops ops = {
=======
	static const struct snd_device_ops ops = {
>>>>>>> upstream/android-13
		.dev_free = snd_ctl_dev_free,
		.dev_register =	snd_ctl_dev_register,
		.dev_disconnect = snd_ctl_dev_disconnect,
	};
	int err;

	if (snd_BUG_ON(!card))
		return -ENXIO;
	if (snd_BUG_ON(card->number < 0 || card->number >= SNDRV_CARDS))
		return -ENXIO;

	snd_device_initialize(&card->ctl_dev, card);
	dev_set_name(&card->ctl_dev, "controlC%d", card->number);

	err = snd_device_new(card, SNDRV_DEV_CONTROL, card, &ops);
	if (err < 0)
		put_device(&card->ctl_dev);
	return err;
}

/*
 * Frequently used control callbacks/helpers
 */

/**
 * snd_ctl_boolean_mono_info - Helper function for a standard boolean info
 * callback with a mono channel
 * @kcontrol: the kcontrol instance
 * @uinfo: info to store
 *
 * This is a function that can be used as info callback for a standard
 * boolean control with a single mono channel.
 */
int snd_ctl_boolean_mono_info(struct snd_kcontrol *kcontrol,
			      struct snd_ctl_elem_info *uinfo)
{
	uinfo->type = SNDRV_CTL_ELEM_TYPE_BOOLEAN;
	uinfo->count = 1;
	uinfo->value.integer.min = 0;
	uinfo->value.integer.max = 1;
	return 0;
}
EXPORT_SYMBOL(snd_ctl_boolean_mono_info);

/**
 * snd_ctl_boolean_stereo_info - Helper function for a standard boolean info
 * callback with stereo two channels
 * @kcontrol: the kcontrol instance
 * @uinfo: info to store
 *
 * This is a function that can be used as info callback for a standard
 * boolean control with stereo two channels.
 */
int snd_ctl_boolean_stereo_info(struct snd_kcontrol *kcontrol,
				struct snd_ctl_elem_info *uinfo)
{
	uinfo->type = SNDRV_CTL_ELEM_TYPE_BOOLEAN;
	uinfo->count = 2;
	uinfo->value.integer.min = 0;
	uinfo->value.integer.max = 1;
	return 0;
}
EXPORT_SYMBOL(snd_ctl_boolean_stereo_info);

/**
 * snd_ctl_enum_info - fills the info structure for an enumerated control
 * @info: the structure to be filled
 * @channels: the number of the control's channels; often one
 * @items: the number of control values; also the size of @names
 * @names: an array containing the names of all control values
 *
 * Sets all required fields in @info to their appropriate values.
 * If the control's accessibility is not the default (readable and writable),
 * the caller has to fill @info->access.
 *
 * Return: Zero.
 */
int snd_ctl_enum_info(struct snd_ctl_elem_info *info, unsigned int channels,
		      unsigned int items, const char *const names[])
{
	info->type = SNDRV_CTL_ELEM_TYPE_ENUMERATED;
	info->count = channels;
	info->value.enumerated.items = items;
	if (!items)
		return 0;
	if (info->value.enumerated.item >= items)
		info->value.enumerated.item = items - 1;
	WARN(strlen(names[info->value.enumerated.item]) >= sizeof(info->value.enumerated.name),
	     "ALSA: too long item name '%s'\n",
	     names[info->value.enumerated.item]);
<<<<<<< HEAD
	strlcpy(info->value.enumerated.name,
=======
	strscpy(info->value.enumerated.name,
>>>>>>> upstream/android-13
		names[info->value.enumerated.item],
		sizeof(info->value.enumerated.name));
	return 0;
}
EXPORT_SYMBOL(snd_ctl_enum_info);
