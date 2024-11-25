// SPDX-License-Identifier: GPL-2.0
/* Copyright 2011 Broadcom Corporation.  All rights reserved. */

#include <linux/platform_device.h>

#include <linux/init.h>
#include <linux/slab.h>
#include <linux/module.h>
<<<<<<< HEAD
#include <linux/of.h>
=======
>>>>>>> upstream/android-13

#include "bcm2835.h"

static bool enable_hdmi;
static bool enable_headphones;
static bool enable_compat_alsa = true;
<<<<<<< HEAD
=======
static int num_channels = MAX_SUBSTREAMS;
>>>>>>> upstream/android-13

module_param(enable_hdmi, bool, 0444);
MODULE_PARM_DESC(enable_hdmi, "Enables HDMI virtual audio device");
module_param(enable_headphones, bool, 0444);
MODULE_PARM_DESC(enable_headphones, "Enables Headphones virtual audio device");
module_param(enable_compat_alsa, bool, 0444);
MODULE_PARM_DESC(enable_compat_alsa,
		 "Enables ALSA compatibility virtual audio device");
<<<<<<< HEAD

static void snd_devm_unregister_child(struct device *dev, void *res)
{
	struct device *childdev = *(struct device **)res;
	struct bcm2835_chip *chip = dev_get_drvdata(childdev);
	struct snd_card *card = chip->card;

	snd_card_free(card);

	device_unregister(childdev);
}

static int snd_devm_add_child(struct device *dev, struct device *child)
{
	struct device **dr;
	int ret;

	dr = devres_alloc(snd_devm_unregister_child, sizeof(*dr), GFP_KERNEL);
	if (!dr)
		return -ENOMEM;

	ret = device_add(child);
	if (ret) {
		devres_free(dr);
		return ret;
	}

	*dr = child;
	devres_add(dev, dr);

	return 0;
}
=======
module_param(num_channels, int, 0644);
MODULE_PARM_DESC(num_channels, "Number of audio channels (default: 8)");
>>>>>>> upstream/android-13

static void bcm2835_devm_free_vchi_ctx(struct device *dev, void *res)
{
	struct bcm2835_vchi_ctx *vchi_ctx = res;

	bcm2835_free_vchi_ctx(vchi_ctx);
}

static int bcm2835_devm_add_vchi_ctx(struct device *dev)
{
	struct bcm2835_vchi_ctx *vchi_ctx;
	int ret;

	vchi_ctx = devres_alloc(bcm2835_devm_free_vchi_ctx, sizeof(*vchi_ctx),
				GFP_KERNEL);
	if (!vchi_ctx)
		return -ENOMEM;

<<<<<<< HEAD
	memset(vchi_ctx, 0, sizeof(*vchi_ctx));

	ret = bcm2835_new_vchi_ctx(vchi_ctx);
=======
	ret = bcm2835_new_vchi_ctx(dev, vchi_ctx);
>>>>>>> upstream/android-13
	if (ret) {
		devres_free(vchi_ctx);
		return ret;
	}

	devres_add(dev, vchi_ctx);

	return 0;
}

<<<<<<< HEAD
static void snd_bcm2835_release(struct device *dev)
{
	struct bcm2835_chip *chip = dev_get_drvdata(dev);

	kfree(chip);
}

static struct device *
snd_create_device(struct device *parent,
		  struct device_driver *driver,
		  const char *name)
{
	struct device *device;
	int ret;

	device = devm_kzalloc(parent, sizeof(*device), GFP_KERNEL);
	if (!device)
		return ERR_PTR(-ENOMEM);

	device_initialize(device);
	device->parent = parent;
	device->driver = driver;
	device->release = snd_bcm2835_release;

	dev_set_name(device, "%s", name);

	ret = snd_devm_add_child(parent, device);
	if (ret)
		return ERR_PTR(ret);

	return device;
}

/* component-destructor
 * (see "Management of Cards and Components")
 */
static int snd_bcm2835_dev_free(struct snd_device *device)
{
	struct bcm2835_chip *chip = device->device_data;
	struct snd_card *card = chip->card;

	snd_device_free(card, chip);

	return 0;
}

/* chip-specific constructor
 * (see "Management of Cards and Components")
 */
static int snd_bcm2835_create(struct snd_card *card,
			      struct bcm2835_chip **rchip)
{
	struct bcm2835_chip *chip;
	int err;
	static struct snd_device_ops ops = {
		.dev_free = snd_bcm2835_dev_free,
	};

	*rchip = NULL;

	chip = kzalloc(sizeof(*chip), GFP_KERNEL);
	if (!chip)
		return -ENOMEM;

	chip->card = card;

	chip->vchi_ctx = devres_find(card->dev->parent,
				     bcm2835_devm_free_vchi_ctx, NULL, NULL);
	if (!chip->vchi_ctx) {
		kfree(chip);
		return -ENODEV;
	}

	err = snd_device_new(card, SNDRV_DEV_LOWLEVEL, chip, &ops);
	if (err) {
		kfree(chip);
		return err;
	}

	*rchip = chip;
	return 0;
}

static struct snd_card *snd_bcm2835_card_new(struct device *dev)
{
	struct snd_card *card;
	int ret;

	ret = snd_card_new(dev, -1, NULL, THIS_MODULE, 0, &card);
	if (ret)
		return ERR_PTR(ret);

	return card;
}

=======
>>>>>>> upstream/android-13
typedef int (*bcm2835_audio_newpcm_func)(struct bcm2835_chip *chip,
					 const char *name,
					 enum snd_bcm2835_route route,
					 u32 numchannels);

typedef int (*bcm2835_audio_newctl_func)(struct bcm2835_chip *chip);

struct bcm2835_audio_driver {
	struct device_driver driver;
	const char *shortname;
	const char *longname;
	int minchannels;
	bcm2835_audio_newpcm_func newpcm;
	bcm2835_audio_newctl_func newctl;
	enum snd_bcm2835_route route;
};

static int bcm2835_audio_alsa_newpcm(struct bcm2835_chip *chip,
				     const char *name,
				     enum snd_bcm2835_route route,
				     u32 numchannels)
{
	int err;

<<<<<<< HEAD
	err = snd_bcm2835_new_pcm(chip, numchannels - 1);
	if (err)
		return err;

	err = snd_bcm2835_new_spdif_pcm(chip);
=======
	err = snd_bcm2835_new_pcm(chip, "bcm2835 ALSA", 0, AUDIO_DEST_AUTO,
				  numchannels - 1, false);
	if (err)
		return err;

	err = snd_bcm2835_new_pcm(chip, "bcm2835 IEC958/HDMI", 1, 0, 1, true);
>>>>>>> upstream/android-13
	if (err)
		return err;

	return 0;
}

<<<<<<< HEAD
=======
static int bcm2835_audio_simple_newpcm(struct bcm2835_chip *chip,
				       const char *name,
				       enum snd_bcm2835_route route,
				       u32 numchannels)
{
	return snd_bcm2835_new_pcm(chip, name, 0, route, numchannels, false);
}

>>>>>>> upstream/android-13
static struct bcm2835_audio_driver bcm2835_audio_alsa = {
	.driver = {
		.name = "bcm2835_alsa",
		.owner = THIS_MODULE,
	},
	.shortname = "bcm2835 ALSA",
	.longname  = "bcm2835 ALSA",
	.minchannels = 2,
	.newpcm = bcm2835_audio_alsa_newpcm,
	.newctl = snd_bcm2835_new_ctl,
};

static struct bcm2835_audio_driver bcm2835_audio_hdmi = {
	.driver = {
		.name = "bcm2835_hdmi",
		.owner = THIS_MODULE,
	},
	.shortname = "bcm2835 HDMI",
	.longname  = "bcm2835 HDMI",
	.minchannels = 1,
<<<<<<< HEAD
	.newpcm = snd_bcm2835_new_simple_pcm,
=======
	.newpcm = bcm2835_audio_simple_newpcm,
>>>>>>> upstream/android-13
	.newctl = snd_bcm2835_new_hdmi_ctl,
	.route = AUDIO_DEST_HDMI
};

static struct bcm2835_audio_driver bcm2835_audio_headphones = {
	.driver = {
		.name = "bcm2835_headphones",
		.owner = THIS_MODULE,
	},
	.shortname = "bcm2835 Headphones",
	.longname  = "bcm2835 Headphones",
	.minchannels = 1,
<<<<<<< HEAD
	.newpcm = snd_bcm2835_new_simple_pcm,
=======
	.newpcm = bcm2835_audio_simple_newpcm,
>>>>>>> upstream/android-13
	.newctl = snd_bcm2835_new_headphones_ctl,
	.route = AUDIO_DEST_HEADPHONES
};

struct bcm2835_audio_drivers {
	struct bcm2835_audio_driver *audio_driver;
	const bool *is_enabled;
};

static struct bcm2835_audio_drivers children_devices[] = {
	{
		.audio_driver = &bcm2835_audio_alsa,
		.is_enabled = &enable_compat_alsa,
	},
	{
		.audio_driver = &bcm2835_audio_hdmi,
		.is_enabled = &enable_hdmi,
	},
	{
		.audio_driver = &bcm2835_audio_headphones,
		.is_enabled = &enable_headphones,
	},
};

<<<<<<< HEAD
static int snd_add_child_device(struct device *device,
				struct bcm2835_audio_driver *audio_driver,
				u32 numchans)
{
	struct snd_card *card;
	struct device *child;
	struct bcm2835_chip *chip;
	int err, i;

	child = snd_create_device(device, &audio_driver->driver,
				  audio_driver->driver.name);
	if (IS_ERR(child)) {
		dev_err(device,
			"Unable to create child device %p, error %ld",
			audio_driver->driver.name,
			PTR_ERR(child));
		return PTR_ERR(child);
	}

	card = snd_bcm2835_card_new(child);
	if (IS_ERR(card)) {
		dev_err(child, "Failed to create card");
		return PTR_ERR(card);
	}

	snd_card_set_dev(card, child);
	strcpy(card->driver, audio_driver->driver.name);
	strcpy(card->shortname, audio_driver->shortname);
	strcpy(card->longname, audio_driver->longname);

	err = snd_bcm2835_create(card, &chip);
	if (err) {
		dev_err(child, "Failed to create chip, error %d\n", err);
		return err;
	}

	chip->dev = child;
=======
static void bcm2835_card_free(void *data)
{
	snd_card_free(data);
}

static int snd_add_child_device(struct device *dev,
				struct bcm2835_audio_driver *audio_driver,
				u32 numchans)
{
	struct bcm2835_chip *chip;
	struct snd_card *card;
	int err;

	err = snd_card_new(dev, -1, NULL, THIS_MODULE, sizeof(*chip), &card);
	if (err < 0) {
		dev_err(dev, "Failed to create card");
		return err;
	}

	chip = card->private_data;
	chip->card = card;
	chip->dev = dev;
	mutex_init(&chip->audio_mutex);

	chip->vchi_ctx = devres_find(dev,
				     bcm2835_devm_free_vchi_ctx, NULL, NULL);
	if (!chip->vchi_ctx) {
		err = -ENODEV;
		goto error;
	}

	strscpy(card->driver, audio_driver->driver.name, sizeof(card->driver));
	strscpy(card->shortname, audio_driver->shortname, sizeof(card->shortname));
	strscpy(card->longname, audio_driver->longname, sizeof(card->longname));
>>>>>>> upstream/android-13

	err = audio_driver->newpcm(chip, audio_driver->shortname,
		audio_driver->route,
		numchans);
	if (err) {
<<<<<<< HEAD
		dev_err(child, "Failed to create pcm, error %d\n", err);
		return err;
=======
		dev_err(dev, "Failed to create pcm, error %d\n", err);
		goto error;
>>>>>>> upstream/android-13
	}

	err = audio_driver->newctl(chip);
	if (err) {
<<<<<<< HEAD
		dev_err(child, "Failed to create controls, error %d\n", err);
		return err;
	}

	for (i = 0; i < numchans; i++)
		chip->avail_substreams |= (1 << i);

	err = snd_card_register(card);
	if (err) {
		dev_err(child, "Failed to register card, error %d\n", err);
		return err;
	}

	dev_set_drvdata(child, chip);
	dev_info(child, "card created with %d channels\n", numchans);

	return 0;
=======
		dev_err(dev, "Failed to create controls, error %d\n", err);
		goto error;
	}

	err = snd_card_register(card);
	if (err) {
		dev_err(dev, "Failed to register card, error %d\n", err);
		goto error;
	}

	dev_set_drvdata(dev, chip);

	err = devm_add_action(dev, bcm2835_card_free, card);
	if (err < 0) {
		dev_err(dev, "Failed to add devm action, err %d\n", err);
		goto error;
	}

	dev_info(dev, "card created with %d channels\n", numchans);
	return 0;

 error:
	snd_card_free(card);
	return err;
>>>>>>> upstream/android-13
}

static int snd_add_child_devices(struct device *device, u32 numchans)
{
<<<<<<< HEAD
	int i;
	int count_devices = 0;
	int minchannels = 0;
	int extrachannels = 0;
	int extrachannels_per_driver = 0;
	int extrachannels_remainder = 0;
=======
	int extrachannels_per_driver = 0;
	int extrachannels_remainder = 0;
	int count_devices = 0;
	int extrachannels = 0;
	int minchannels = 0;
	int i;
>>>>>>> upstream/android-13

	for (i = 0; i < ARRAY_SIZE(children_devices); i++)
		if (*children_devices[i].is_enabled)
			count_devices++;

	if (!count_devices)
		return 0;

	for (i = 0; i < ARRAY_SIZE(children_devices); i++)
		if (*children_devices[i].is_enabled)
			minchannels +=
				children_devices[i].audio_driver->minchannels;

	if (minchannels < numchans) {
		extrachannels = numchans - minchannels;
		extrachannels_per_driver = extrachannels / count_devices;
		extrachannels_remainder = extrachannels % count_devices;
	}

	dev_dbg(device, "minchannels %d\n", minchannels);
	dev_dbg(device, "extrachannels %d\n", extrachannels);
	dev_dbg(device, "extrachannels_per_driver %d\n",
		extrachannels_per_driver);
	dev_dbg(device, "extrachannels_remainder %d\n",
		extrachannels_remainder);

	for (i = 0; i < ARRAY_SIZE(children_devices); i++) {
<<<<<<< HEAD
		int err;
		int numchannels_this_device;
		struct bcm2835_audio_driver *audio_driver;
=======
		struct bcm2835_audio_driver *audio_driver;
		int numchannels_this_device;
		int err;
>>>>>>> upstream/android-13

		if (!*children_devices[i].is_enabled)
			continue;

		audio_driver = children_devices[i].audio_driver;

		if (audio_driver->minchannels > numchans) {
			dev_err(device,
				"Out of channels, needed %d but only %d left\n",
				audio_driver->minchannels,
				numchans);
			continue;
		}

		numchannels_this_device =
			audio_driver->minchannels + extrachannels_per_driver +
			extrachannels_remainder;
		extrachannels_remainder = 0;

		numchans -= numchannels_this_device;

		err = snd_add_child_device(device, audio_driver,
					   numchannels_this_device);
		if (err)
			return err;
	}

	return 0;
}

<<<<<<< HEAD
static int snd_bcm2835_alsa_probe_dt(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	u32 numchans;
	int err;

	err = of_property_read_u32(dev->of_node, "brcm,pwm-channels",
				   &numchans);
	if (err) {
		dev_err(dev, "Failed to get DT property 'brcm,pwm-channels'");
		return err;
	}

	if (numchans == 0 || numchans > MAX_SUBSTREAMS) {
		numchans = MAX_SUBSTREAMS;
		dev_warn(dev,
			 "Illegal 'brcm,pwm-channels' value, will use %u\n",
			 numchans);
=======
static int snd_bcm2835_alsa_probe(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	int err;

	if (num_channels <= 0 || num_channels > MAX_SUBSTREAMS) {
		num_channels = MAX_SUBSTREAMS;
		dev_warn(dev, "Illegal num_channels value, will use %u\n",
			 num_channels);
>>>>>>> upstream/android-13
	}

	err = bcm2835_devm_add_vchi_ctx(dev);
	if (err)
		return err;

<<<<<<< HEAD
	err = snd_add_child_devices(dev, numchans);
=======
	err = snd_add_child_devices(dev, num_channels);
>>>>>>> upstream/android-13
	if (err)
		return err;

	return 0;
}

#ifdef CONFIG_PM

static int snd_bcm2835_alsa_suspend(struct platform_device *pdev,
				    pm_message_t state)
{
	return 0;
}

static int snd_bcm2835_alsa_resume(struct platform_device *pdev)
{
	return 0;
}

#endif

<<<<<<< HEAD
static const struct of_device_id snd_bcm2835_of_match_table[] = {
	{ .compatible = "brcm,bcm2835-audio",},
	{},
};
MODULE_DEVICE_TABLE(of, snd_bcm2835_of_match_table);

static struct platform_driver bcm2835_alsa0_driver = {
	.probe = snd_bcm2835_alsa_probe_dt,
=======
static struct platform_driver bcm2835_alsa_driver = {
	.probe = snd_bcm2835_alsa_probe,
>>>>>>> upstream/android-13
#ifdef CONFIG_PM
	.suspend = snd_bcm2835_alsa_suspend,
	.resume = snd_bcm2835_alsa_resume,
#endif
	.driver = {
		.name = "bcm2835_audio",
<<<<<<< HEAD
		.of_match_table = snd_bcm2835_of_match_table,
	},
};

static int bcm2835_alsa_device_init(void)
{
	int retval;

	retval = platform_driver_register(&bcm2835_alsa0_driver);
	if (retval)
		pr_err("Error registering bcm2835_audio driver %d .\n", retval);

	return retval;
}

static void bcm2835_alsa_device_exit(void)
{
	platform_driver_unregister(&bcm2835_alsa0_driver);
}

late_initcall(bcm2835_alsa_device_init);
module_exit(bcm2835_alsa_device_exit);
=======
	},
};
module_platform_driver(bcm2835_alsa_driver);
>>>>>>> upstream/android-13

MODULE_AUTHOR("Dom Cobley");
MODULE_DESCRIPTION("Alsa driver for BCM2835 chip");
MODULE_LICENSE("GPL");
<<<<<<< HEAD
=======
MODULE_ALIAS("platform:bcm2835_audio");
>>>>>>> upstream/android-13
