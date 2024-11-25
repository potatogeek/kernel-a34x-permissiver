<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> upstream/android-13
/*
 * ff.c - a part of driver for RME Fireface series
 *
 * Copyright (c) 2015-2017 Takashi Sakamoto
<<<<<<< HEAD
 *
 * Licensed under the terms of the GNU General Public License, version 2.
=======
>>>>>>> upstream/android-13
 */

#include "ff.h"

#define OUI_RME	0x000a35

MODULE_DESCRIPTION("RME Fireface series Driver");
MODULE_AUTHOR("Takashi Sakamoto <o-takashi@sakamocchi.jp>");
MODULE_LICENSE("GPL v2");

static void name_card(struct snd_ff *ff)
{
	struct fw_device *fw_dev = fw_parent_device(ff->unit);
<<<<<<< HEAD

	strcpy(ff->card->driver, "Fireface");
	strcpy(ff->card->shortname, ff->spec->name);
	strcpy(ff->card->mixername, ff->spec->name);
	snprintf(ff->card->longname, sizeof(ff->card->longname),
		 "RME %s, GUID %08x%08x at %s, S%d", ff->spec->name,
=======
	const char *const names[] = {
		[SND_FF_UNIT_VERSION_FF800]	= "Fireface800",
		[SND_FF_UNIT_VERSION_FF400]	= "Fireface400",
		[SND_FF_UNIT_VERSION_UFX]	= "FirefaceUFX",
		[SND_FF_UNIT_VERSION_UCX]	= "FirefaceUCX",
		[SND_FF_UNIT_VERSION_802]	= "Fireface802",
	};
	const char *name;

	name = names[ff->unit_version];

	strcpy(ff->card->driver, "Fireface");
	strcpy(ff->card->shortname, name);
	strcpy(ff->card->mixername, name);
	snprintf(ff->card->longname, sizeof(ff->card->longname),
		 "RME %s, GUID %08x%08x at %s, S%d", name,
>>>>>>> upstream/android-13
		 fw_dev->config_rom[3], fw_dev->config_rom[4],
		 dev_name(&ff->unit->device), 100 << fw_dev->max_speed);
}

<<<<<<< HEAD
static void ff_free(struct snd_ff *ff)
{
	snd_ff_stream_destroy_duplex(ff);
	snd_ff_transaction_unregister(ff);

	fw_unit_put(ff->unit);

	mutex_destroy(&ff->mutex);
	kfree(ff);
}

static void ff_card_free(struct snd_card *card)
{
	ff_free(card->private_data);
}

static void do_registration(struct work_struct *work)
{
	struct snd_ff *ff = container_of(work, struct snd_ff, dwork.work);
	int err;

	if (ff->registered)
		return;

	err = snd_card_new(&ff->unit->device, -1, NULL, THIS_MODULE, 0,
			   &ff->card);
	if (err < 0)
		return;
=======
static void ff_card_free(struct snd_card *card)
{
	struct snd_ff *ff = card->private_data;

	snd_ff_stream_destroy_duplex(ff);
	snd_ff_transaction_unregister(ff);

	mutex_destroy(&ff->mutex);
	fw_unit_put(ff->unit);
}

static int snd_ff_probe(struct fw_unit *unit, const struct ieee1394_device_id *entry)
{
	struct snd_card *card;
	struct snd_ff *ff;
	int err;

	err = snd_card_new(&unit->device, -1, NULL, THIS_MODULE, sizeof(*ff), &card);
	if (err < 0)
		return err;
	card->private_free = ff_card_free;

	ff = card->private_data;
	ff->unit = fw_unit_get(unit);
	dev_set_drvdata(&unit->device, ff);
	ff->card = card;

	mutex_init(&ff->mutex);
	spin_lock_init(&ff->lock);
	init_waitqueue_head(&ff->hwdep_wait);

	ff->unit_version = entry->version;
	ff->spec = (const struct snd_ff_spec *)entry->driver_data;
>>>>>>> upstream/android-13

	err = snd_ff_transaction_register(ff);
	if (err < 0)
		goto error;

	name_card(ff);

	err = snd_ff_stream_init_duplex(ff);
	if (err < 0)
		goto error;

	snd_ff_proc_init(ff);

	err = snd_ff_create_midi_devices(ff);
	if (err < 0)
		goto error;

	err = snd_ff_create_pcm_devices(ff);
	if (err < 0)
		goto error;

	err = snd_ff_create_hwdep_devices(ff);
	if (err < 0)
		goto error;

<<<<<<< HEAD
	err = snd_card_register(ff->card);
	if (err < 0)
		goto error;

	ff->card->private_free = ff_card_free;
	ff->card->private_data = ff;
	ff->registered = true;

	return;
error:
	snd_ff_transaction_unregister(ff);
	snd_ff_stream_destroy_duplex(ff);
	snd_card_free(ff->card);
	dev_info(&ff->unit->device,
		 "Sound card registration failed: %d\n", err);
}

static int snd_ff_probe(struct fw_unit *unit,
			   const struct ieee1394_device_id *entry)
{
	struct snd_ff *ff;

	ff = kzalloc(sizeof(struct snd_ff), GFP_KERNEL);
	if (ff == NULL)
		return -ENOMEM;

	/* initialize myself */
	ff->unit = fw_unit_get(unit);
	dev_set_drvdata(&unit->device, ff);

	mutex_init(&ff->mutex);
	spin_lock_init(&ff->lock);
	init_waitqueue_head(&ff->hwdep_wait);

	ff->spec = (const struct snd_ff_spec *)entry->driver_data;

	/* Register this sound card later. */
	INIT_DEFERRABLE_WORK(&ff->dwork, do_registration);
	snd_fw_schedule_registration(unit, &ff->dwork);

	return 0;
=======
	err = snd_card_register(card);
	if (err < 0)
		goto error;

	return 0;
error:
	snd_card_free(card);
	return err;
>>>>>>> upstream/android-13
}

static void snd_ff_update(struct fw_unit *unit)
{
	struct snd_ff *ff = dev_get_drvdata(&unit->device);

<<<<<<< HEAD
	/* Postpone a workqueue for deferred registration. */
	if (!ff->registered)
		snd_fw_schedule_registration(unit, &ff->dwork);

	snd_ff_transaction_reregister(ff);

	if (ff->registered)
		snd_ff_stream_update_duplex(ff);
=======
	snd_ff_transaction_reregister(ff);

	snd_ff_stream_update_duplex(ff);
>>>>>>> upstream/android-13
}

static void snd_ff_remove(struct fw_unit *unit)
{
	struct snd_ff *ff = dev_get_drvdata(&unit->device);

<<<<<<< HEAD
	/*
	 * Confirm to stop the work for registration before the sound card is
	 * going to be released. The work is not scheduled again because bus
	 * reset handler is not called anymore.
	 */
	cancel_work_sync(&ff->dwork.work);

	if (ff->registered) {
		/* No need to wait for releasing card object in this context. */
		snd_card_free_when_closed(ff->card);
	} else {
		/* Don't forget this case. */
		ff_free(ff);
	}
}

static const struct snd_ff_spec spec_ff400 = {
	.name = "Fireface400",
=======
	// Block till all of ALSA character devices are released.
	snd_card_free(ff->card);
}

static const struct snd_ff_spec spec_ff800 = {
	.pcm_capture_channels = {28, 20, 12},
	.pcm_playback_channels = {28, 20, 12},
	.midi_in_ports = 1,
	.midi_out_ports = 1,
	.protocol = &snd_ff_protocol_ff800,
	.midi_high_addr = 0x000200000320ull,
	.midi_addr_range = 12,
	.midi_rx_addrs = {0x000080180000ull, 0},
};

static const struct snd_ff_spec spec_ff400 = {
>>>>>>> upstream/android-13
	.pcm_capture_channels = {18, 14, 10},
	.pcm_playback_channels = {18, 14, 10},
	.midi_in_ports = 2,
	.midi_out_ports = 2,
	.protocol = &snd_ff_protocol_ff400,
<<<<<<< HEAD
};

static const struct ieee1394_device_id snd_ff_id_table[] = {
=======
	.midi_high_addr = 0x0000801003f4ull,
	.midi_addr_range = SND_FF_MAXIMIM_MIDI_QUADS * 4,
	.midi_rx_addrs = {0x000080180000ull, 0x000080190000ull},
};

static const struct snd_ff_spec spec_ucx = {
	.pcm_capture_channels = {18, 14, 12},
	.pcm_playback_channels = {18, 14, 12},
	.midi_in_ports = 2,
	.midi_out_ports = 2,
	.protocol = &snd_ff_protocol_latter,
	.midi_high_addr = 0xffff00000034ull,
	.midi_addr_range = 0x80,
	.midi_rx_addrs = {0xffff00000030ull, 0xffff00000030ull},
};

static const struct snd_ff_spec spec_ufx_802 = {
	.pcm_capture_channels = {30, 22, 14},
	.pcm_playback_channels = {30, 22, 14},
	.midi_in_ports = 1,
	.midi_out_ports = 1,
	.protocol = &snd_ff_protocol_latter,
	.midi_high_addr = 0xffff00000034ull,
	.midi_addr_range = 0x80,
	.midi_rx_addrs = {0xffff00000030ull, 0xffff00000030ull},
};

static const struct ieee1394_device_id snd_ff_id_table[] = {
	/* Fireface 800 */
	{
		.match_flags	= IEEE1394_MATCH_VENDOR_ID |
				  IEEE1394_MATCH_SPECIFIER_ID |
				  IEEE1394_MATCH_VERSION |
				  IEEE1394_MATCH_MODEL_ID,
		.vendor_id	= OUI_RME,
		.specifier_id	= OUI_RME,
		.version	= SND_FF_UNIT_VERSION_FF800,
		.model_id	= 0x101800,
		.driver_data	= (kernel_ulong_t)&spec_ff800,
	},
>>>>>>> upstream/android-13
	/* Fireface 400 */
	{
		.match_flags	= IEEE1394_MATCH_VENDOR_ID |
				  IEEE1394_MATCH_SPECIFIER_ID |
				  IEEE1394_MATCH_VERSION |
				  IEEE1394_MATCH_MODEL_ID,
		.vendor_id	= OUI_RME,
<<<<<<< HEAD
		.specifier_id	= 0x000a35,
		.version	= 0x000002,
		.model_id	= 0x101800,
		.driver_data	= (kernel_ulong_t)&spec_ff400,
	},
=======
		.specifier_id	= OUI_RME,
		.version	= SND_FF_UNIT_VERSION_FF400,
		.model_id	= 0x101800,
		.driver_data	= (kernel_ulong_t)&spec_ff400,
	},
	// Fireface UFX.
	{
		.match_flags	= IEEE1394_MATCH_VENDOR_ID |
				  IEEE1394_MATCH_SPECIFIER_ID |
				  IEEE1394_MATCH_VERSION |
				  IEEE1394_MATCH_MODEL_ID,
		.vendor_id	= OUI_RME,
		.specifier_id	= OUI_RME,
		.version	= SND_FF_UNIT_VERSION_UFX,
		.model_id	= 0x101800,
		.driver_data	= (kernel_ulong_t)&spec_ufx_802,
	},
	// Fireface UCX.
	{
		.match_flags	= IEEE1394_MATCH_VENDOR_ID |
				  IEEE1394_MATCH_SPECIFIER_ID |
				  IEEE1394_MATCH_VERSION |
				  IEEE1394_MATCH_MODEL_ID,
		.vendor_id	= OUI_RME,
		.specifier_id	= OUI_RME,
		.version	= SND_FF_UNIT_VERSION_UCX,
		.model_id	= 0x101800,
		.driver_data	= (kernel_ulong_t)&spec_ucx,
	},
	// Fireface 802.
	{
		.match_flags	= IEEE1394_MATCH_VENDOR_ID |
				  IEEE1394_MATCH_SPECIFIER_ID |
				  IEEE1394_MATCH_VERSION |
				  IEEE1394_MATCH_MODEL_ID,
		.vendor_id	= OUI_RME,
		.specifier_id	= OUI_RME,
		.version	= SND_FF_UNIT_VERSION_802,
		.model_id	= 0x101800,
		.driver_data	= (kernel_ulong_t)&spec_ufx_802,
	},
>>>>>>> upstream/android-13
	{}
};
MODULE_DEVICE_TABLE(ieee1394, snd_ff_id_table);

static struct fw_driver ff_driver = {
	.driver = {
		.owner	= THIS_MODULE,
<<<<<<< HEAD
		.name	= "snd-fireface",
=======
		.name	= KBUILD_MODNAME,
>>>>>>> upstream/android-13
		.bus	= &fw_bus_type,
	},
	.probe    = snd_ff_probe,
	.update   = snd_ff_update,
	.remove   = snd_ff_remove,
	.id_table = snd_ff_id_table,
};

static int __init snd_ff_init(void)
{
	return driver_register(&ff_driver.driver);
}

static void __exit snd_ff_exit(void)
{
	driver_unregister(&ff_driver.driver);
}

module_init(snd_ff_init);
module_exit(snd_ff_exit);
