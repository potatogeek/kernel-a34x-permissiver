<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> upstream/android-13
/*
 * motu.c - a part of driver for MOTU FireWire series
 *
 * Copyright (c) 2015-2017 Takashi Sakamoto <o-takashi@sakamocchi.jp>
<<<<<<< HEAD
 *
 * Licensed under the terms of the GNU General Public License, version 2.
=======
>>>>>>> upstream/android-13
 */

#include "motu.h"

#define OUI_MOTU	0x0001f2

MODULE_DESCRIPTION("MOTU FireWire driver");
MODULE_AUTHOR("Takashi Sakamoto <o-takashi@sakamocchi.jp>");
MODULE_LICENSE("GPL v2");

const unsigned int snd_motu_clock_rates[SND_MOTU_CLOCK_RATE_COUNT] = {
	/* mode 0 */
	[0] =  44100,
	[1] =  48000,
	/* mode 1 */
	[2] =  88200,
	[3] =  96000,
	/* mode 2 */
	[4] = 176400,
	[5] = 192000,
};

static void name_card(struct snd_motu *motu)
{
	struct fw_device *fw_dev = fw_parent_device(motu->unit);
	struct fw_csr_iterator it;
	int key, val;
	u32 version = 0;

	fw_csr_iterator_init(&it, motu->unit->directory);
	while (fw_csr_iterator_next(&it, &key, &val)) {
		switch (key) {
		case CSR_MODEL:
			version = val;
			break;
		}
	}

	strcpy(motu->card->driver, "FW-MOTU");
	strcpy(motu->card->shortname, motu->spec->name);
	strcpy(motu->card->mixername, motu->spec->name);
	snprintf(motu->card->longname, sizeof(motu->card->longname),
		 "MOTU %s (version:%06x), GUID %08x%08x at %s, S%d",
		 motu->spec->name, version,
		 fw_dev->config_rom[3], fw_dev->config_rom[4],
		 dev_name(&motu->unit->device), 100 << fw_dev->max_speed);
}

<<<<<<< HEAD
static void motu_free(struct snd_motu *motu)
{
	snd_motu_transaction_unregister(motu);

	snd_motu_stream_destroy_duplex(motu);
	fw_unit_put(motu->unit);

	mutex_destroy(&motu->mutex);
	kfree(motu);
}

/*
 * This module releases the FireWire unit data after all ALSA character devices
 * are released by applications. This is for releasing stream data or finishing
 * transactions safely. Thus at returning from .remove(), this module still keep
 * references for the unit.
 */
static void motu_card_free(struct snd_card *card)
{
	motu_free(card->private_data);
}

static void do_registration(struct work_struct *work)
{
	struct snd_motu *motu = container_of(work, struct snd_motu, dwork.work);
	int err;

	if (motu->registered)
		return;

	err = snd_card_new(&motu->unit->device, -1, NULL, THIS_MODULE, 0,
			   &motu->card);
	if (err < 0)
		return;
=======
static void motu_card_free(struct snd_card *card)
{
	struct snd_motu *motu = card->private_data;

	snd_motu_transaction_unregister(motu);
	snd_motu_stream_destroy_duplex(motu);

	mutex_destroy(&motu->mutex);
	fw_unit_put(motu->unit);
}

static int motu_probe(struct fw_unit *unit, const struct ieee1394_device_id *entry)
{
	struct snd_card *card;
	struct snd_motu *motu;
	int err;

	err = snd_card_new(&unit->device, -1, NULL, THIS_MODULE, sizeof(*motu), &card);
	if (err < 0)
		return err;
	card->private_free = motu_card_free;

	motu = card->private_data;
	motu->unit = fw_unit_get(unit);
	dev_set_drvdata(&unit->device, motu);
	motu->card = card;

	motu->spec = (const struct snd_motu_spec *)entry->driver_data;
	mutex_init(&motu->mutex);
	spin_lock_init(&motu->lock);
	init_waitqueue_head(&motu->hwdep_wait);
>>>>>>> upstream/android-13

	name_card(motu);

	err = snd_motu_transaction_register(motu);
	if (err < 0)
		goto error;

	err = snd_motu_stream_init_duplex(motu);
	if (err < 0)
		goto error;

	snd_motu_proc_init(motu);

	err = snd_motu_create_pcm_devices(motu);
	if (err < 0)
		goto error;

	if ((motu->spec->flags & SND_MOTU_SPEC_RX_MIDI_2ND_Q) ||
	    (motu->spec->flags & SND_MOTU_SPEC_RX_MIDI_3RD_Q) ||
	    (motu->spec->flags & SND_MOTU_SPEC_TX_MIDI_2ND_Q) ||
	    (motu->spec->flags & SND_MOTU_SPEC_TX_MIDI_3RD_Q)) {
		err = snd_motu_create_midi_devices(motu);
		if (err < 0)
			goto error;
	}

	err = snd_motu_create_hwdep_device(motu);
	if (err < 0)
		goto error;

<<<<<<< HEAD
	err = snd_card_register(motu->card);
	if (err < 0)
		goto error;

	/*
	 * After registered, motu instance can be released corresponding to
	 * releasing the sound card instance.
	 */
	motu->card->private_free = motu_card_free;
	motu->card->private_data = motu;
	motu->registered = true;

	return;
error:
	snd_motu_transaction_unregister(motu);
	snd_motu_stream_destroy_duplex(motu);
	snd_card_free(motu->card);
	dev_info(&motu->unit->device,
		 "Sound card registration failed: %d\n", err);
}

static int motu_probe(struct fw_unit *unit,
		      const struct ieee1394_device_id *entry)
{
	struct snd_motu *motu;

	/* Allocate this independently of sound card instance. */
	motu = kzalloc(sizeof(struct snd_motu), GFP_KERNEL);
	if (motu == NULL)
		return -ENOMEM;

	motu->spec = (const struct snd_motu_spec *)entry->driver_data;
	motu->unit = fw_unit_get(unit);
	dev_set_drvdata(&unit->device, motu);

	mutex_init(&motu->mutex);
	spin_lock_init(&motu->lock);
	init_waitqueue_head(&motu->hwdep_wait);

	/* Allocate and register this sound card later. */
	INIT_DEFERRABLE_WORK(&motu->dwork, do_registration);
	snd_fw_schedule_registration(unit, &motu->dwork);

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

static void motu_remove(struct fw_unit *unit)
{
	struct snd_motu *motu = dev_get_drvdata(&unit->device);

<<<<<<< HEAD
	/*
	 * Confirm to stop the work for registration before the sound card is
	 * going to be released. The work is not scheduled again because bus
	 * reset handler is not called anymore.
	 */
	cancel_delayed_work_sync(&motu->dwork);

	if (motu->registered) {
		/* No need to wait for releasing card object in this context. */
		snd_card_free_when_closed(motu->card);
	} else {
		/* Don't forget this case. */
		motu_free(motu);
	}
=======
	// Block till all of ALSA character devices are released.
	snd_card_free(motu->card);
>>>>>>> upstream/android-13
}

static void motu_bus_update(struct fw_unit *unit)
{
	struct snd_motu *motu = dev_get_drvdata(&unit->device);

<<<<<<< HEAD
	/* Postpone a workqueue for deferred registration. */
	if (!motu->registered)
		snd_fw_schedule_registration(unit, &motu->dwork);

=======
>>>>>>> upstream/android-13
	/* The handler address register becomes initialized. */
	snd_motu_transaction_reregister(motu);
}

<<<<<<< HEAD
static const struct snd_motu_spec motu_828mk2 = {
	.name = "828mk2",
	.protocol = &snd_motu_protocol_v2,
	.flags = SND_MOTU_SPEC_SUPPORT_CLOCK_X2 |
		 SND_MOTU_SPEC_TX_MICINST_CHUNK |
		 SND_MOTU_SPEC_TX_RETURN_CHUNK |
		 SND_MOTU_SPEC_RX_SEPARETED_MAIN |
		 SND_MOTU_SPEC_HAS_OPT_IFACE_A |
		 SND_MOTU_SPEC_RX_MIDI_2ND_Q |
		 SND_MOTU_SPEC_TX_MIDI_2ND_Q,

	.analog_in_ports = 8,
	.analog_out_ports = 8,
};

const struct snd_motu_spec snd_motu_spec_traveler = {
	.name = "Traveler",
	.protocol = &snd_motu_protocol_v2,
	.flags = SND_MOTU_SPEC_SUPPORT_CLOCK_X2 |
		 SND_MOTU_SPEC_SUPPORT_CLOCK_X4 |
		 SND_MOTU_SPEC_TX_RETURN_CHUNK |
		 SND_MOTU_SPEC_HAS_AESEBU_IFACE |
		 SND_MOTU_SPEC_HAS_OPT_IFACE_A |
		 SND_MOTU_SPEC_RX_MIDI_2ND_Q |
		 SND_MOTU_SPEC_TX_MIDI_2ND_Q,

	.analog_in_ports = 8,
	.analog_out_ports = 8,
};

static const struct snd_motu_spec motu_828mk3 = {
	.name = "828mk3",
	.protocol = &snd_motu_protocol_v3,
	.flags = SND_MOTU_SPEC_SUPPORT_CLOCK_X2 |
		 SND_MOTU_SPEC_SUPPORT_CLOCK_X4 |
		 SND_MOTU_SPEC_TX_MICINST_CHUNK |
		 SND_MOTU_SPEC_TX_RETURN_CHUNK |
		 SND_MOTU_SPEC_TX_REVERB_CHUNK |
		 SND_MOTU_SPEC_RX_SEPARETED_MAIN |
		 SND_MOTU_SPEC_HAS_OPT_IFACE_A |
		 SND_MOTU_SPEC_HAS_OPT_IFACE_B |
		 SND_MOTU_SPEC_RX_MIDI_3RD_Q |
		 SND_MOTU_SPEC_TX_MIDI_3RD_Q,

	.analog_in_ports = 8,
	.analog_out_ports = 8,
};

static const struct snd_motu_spec motu_audio_express = {
	.name = "AudioExpress",
	.protocol = &snd_motu_protocol_v3,
	.flags = SND_MOTU_SPEC_SUPPORT_CLOCK_X2 |
		 SND_MOTU_SPEC_TX_MICINST_CHUNK |
		 SND_MOTU_SPEC_TX_RETURN_CHUNK |
		 SND_MOTU_SPEC_RX_SEPARETED_MAIN |
		 SND_MOTU_SPEC_RX_MIDI_2ND_Q |
		 SND_MOTU_SPEC_TX_MIDI_3RD_Q,
	.analog_in_ports = 2,
	.analog_out_ports = 4,
};

static const struct snd_motu_spec motu_4pre = {
	.name = "4pre",
	.protocol = &snd_motu_protocol_v3,
	.flags = SND_MOTU_SPEC_SUPPORT_CLOCK_X2 |
		 SND_MOTU_SPEC_TX_MICINST_CHUNK |
		 SND_MOTU_SPEC_TX_RETURN_CHUNK |
		 SND_MOTU_SPEC_RX_SEPARETED_MAIN,
	.analog_in_ports = 2,
	.analog_out_ports = 2,
};

=======
>>>>>>> upstream/android-13
#define SND_MOTU_DEV_ENTRY(model, data)			\
{							\
	.match_flags	= IEEE1394_MATCH_VENDOR_ID |	\
			  IEEE1394_MATCH_SPECIFIER_ID |	\
			  IEEE1394_MATCH_VERSION,	\
	.vendor_id	= OUI_MOTU,			\
	.specifier_id	= OUI_MOTU,			\
	.version	= model,			\
	.driver_data	= (kernel_ulong_t)data,		\
}

static const struct ieee1394_device_id motu_id_table[] = {
<<<<<<< HEAD
	SND_MOTU_DEV_ENTRY(0x000003, &motu_828mk2),
	SND_MOTU_DEV_ENTRY(0x000009, &snd_motu_spec_traveler),
	SND_MOTU_DEV_ENTRY(0x000015, &motu_828mk3),	/* FireWire only. */
	SND_MOTU_DEV_ENTRY(0x000035, &motu_828mk3),	/* Hybrid. */
	SND_MOTU_DEV_ENTRY(0x000033, &motu_audio_express),
	SND_MOTU_DEV_ENTRY(0x000045, &motu_4pre),
=======
	SND_MOTU_DEV_ENTRY(0x000001, &snd_motu_spec_828),
	SND_MOTU_DEV_ENTRY(0x000002, &snd_motu_spec_896),
	SND_MOTU_DEV_ENTRY(0x000003, &snd_motu_spec_828mk2),
	SND_MOTU_DEV_ENTRY(0x000005, &snd_motu_spec_896hd),
	SND_MOTU_DEV_ENTRY(0x000009, &snd_motu_spec_traveler),
	SND_MOTU_DEV_ENTRY(0x00000d, &snd_motu_spec_ultralite),
	SND_MOTU_DEV_ENTRY(0x00000f, &snd_motu_spec_8pre),
	SND_MOTU_DEV_ENTRY(0x000015, &snd_motu_spec_828mk3_fw), // FireWire only.
	SND_MOTU_DEV_ENTRY(0x000019, &snd_motu_spec_ultralite_mk3), // FireWire only.
	SND_MOTU_DEV_ENTRY(0x000030, &snd_motu_spec_ultralite_mk3), // Hybrid.
	SND_MOTU_DEV_ENTRY(0x000035, &snd_motu_spec_828mk3_hybrid), // Hybrid.
	SND_MOTU_DEV_ENTRY(0x000033, &snd_motu_spec_audio_express),
	SND_MOTU_DEV_ENTRY(0x000045, &snd_motu_spec_4pre),
>>>>>>> upstream/android-13
	{ }
};
MODULE_DEVICE_TABLE(ieee1394, motu_id_table);

static struct fw_driver motu_driver = {
	.driver   = {
		.owner	= THIS_MODULE,
		.name	= KBUILD_MODNAME,
		.bus	= &fw_bus_type,
	},
	.probe    = motu_probe,
	.update   = motu_bus_update,
	.remove   = motu_remove,
	.id_table = motu_id_table,
};

static int __init alsa_motu_init(void)
{
	return driver_register(&motu_driver.driver);
}

static void __exit alsa_motu_exit(void)
{
	driver_unregister(&motu_driver.driver);
}

module_init(alsa_motu_init);
module_exit(alsa_motu_exit);
