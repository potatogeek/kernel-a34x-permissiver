<<<<<<< HEAD
/*
 *  The driver for the Cirrus Logic's Sound Fusion CS46XX based soundcards
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
 *  The driver for the Cirrus Logic's Sound Fusion CS46XX based soundcards
 *  Copyright (c) by Jaroslav Kysela <perex@perex.cz>
>>>>>>> upstream/android-13
 */

/*
  NOTES:
  - sometimes the sound is metallic and sibilant, unloading and 
    reloading the module may solve this.
*/

#include <linux/pci.h>
#include <linux/time.h>
#include <linux/init.h>
#include <linux/module.h>
#include <sound/core.h>
#include "cs46xx.h"
#include <sound/initval.h>

MODULE_AUTHOR("Jaroslav Kysela <perex@perex.cz>");
MODULE_DESCRIPTION("Cirrus Logic Sound Fusion CS46XX");
MODULE_LICENSE("GPL");
<<<<<<< HEAD
MODULE_SUPPORTED_DEVICE("{{Cirrus Logic,Sound Fusion (CS4280)},"
		"{Cirrus Logic,Sound Fusion (CS4610)},"
		"{Cirrus Logic,Sound Fusion (CS4612)},"
		"{Cirrus Logic,Sound Fusion (CS4615)},"
		"{Cirrus Logic,Sound Fusion (CS4622)},"
		"{Cirrus Logic,Sound Fusion (CS4624)},"
		"{Cirrus Logic,Sound Fusion (CS4630)}}");
=======
>>>>>>> upstream/android-13

static int index[SNDRV_CARDS] = SNDRV_DEFAULT_IDX;	/* Index 0-MAX */
static char *id[SNDRV_CARDS] = SNDRV_DEFAULT_STR;	/* ID for this card */
static bool enable[SNDRV_CARDS] = SNDRV_DEFAULT_ENABLE_PNP;	/* Enable this card */
static bool external_amp[SNDRV_CARDS];
static bool thinkpad[SNDRV_CARDS];
static bool mmap_valid[SNDRV_CARDS] = {[0 ... (SNDRV_CARDS - 1)] = 1};

module_param_array(index, int, NULL, 0444);
MODULE_PARM_DESC(index, "Index value for the CS46xx soundcard.");
module_param_array(id, charp, NULL, 0444);
MODULE_PARM_DESC(id, "ID string for the CS46xx soundcard.");
module_param_array(enable, bool, NULL, 0444);
MODULE_PARM_DESC(enable, "Enable CS46xx soundcard.");
module_param_array(external_amp, bool, NULL, 0444);
MODULE_PARM_DESC(external_amp, "Force to enable external amplifier.");
module_param_array(thinkpad, bool, NULL, 0444);
MODULE_PARM_DESC(thinkpad, "Force to enable Thinkpad's CLKRUN control.");
module_param_array(mmap_valid, bool, NULL, 0444);
MODULE_PARM_DESC(mmap_valid, "Support OSS mmap.");

static const struct pci_device_id snd_cs46xx_ids[] = {
	{ PCI_VDEVICE(CIRRUS, 0x6001), 0, },   /* CS4280 */
	{ PCI_VDEVICE(CIRRUS, 0x6003), 0, },   /* CS4612 */
	{ PCI_VDEVICE(CIRRUS, 0x6004), 0, },   /* CS4615 */
	{ 0, }
};

MODULE_DEVICE_TABLE(pci, snd_cs46xx_ids);

static int snd_card_cs46xx_probe(struct pci_dev *pci,
				 const struct pci_device_id *pci_id)
{
	static int dev;
	struct snd_card *card;
	struct snd_cs46xx *chip;
	int err;

	if (dev >= SNDRV_CARDS)
		return -ENODEV;
	if (!enable[dev]) {
		dev++;
		return -ENOENT;
	}

<<<<<<< HEAD
	err = snd_card_new(&pci->dev, index[dev], id[dev], THIS_MODULE,
			   0, &card);
	if (err < 0)
		return err;
	if ((err = snd_cs46xx_create(card, pci,
				     external_amp[dev], thinkpad[dev],
				     &chip)) < 0) {
		snd_card_free(card);
		return err;
	}
	card->private_data = chip;
	chip->accept_valid = mmap_valid[dev];
	if ((err = snd_cs46xx_pcm(chip, 0)) < 0) {
		snd_card_free(card);
		return err;
	}
#ifdef CONFIG_SND_CS46XX_NEW_DSP
	if ((err = snd_cs46xx_pcm_rear(chip, 1)) < 0) {
		snd_card_free(card);
		return err;
	}
	if ((err = snd_cs46xx_pcm_iec958(chip, 2)) < 0) {
		snd_card_free(card);
		return err;
	}
#endif
	if ((err = snd_cs46xx_mixer(chip, 2)) < 0) {
		snd_card_free(card);
		return err;
	}
#ifdef CONFIG_SND_CS46XX_NEW_DSP
	if (chip->nr_ac97_codecs ==2) {
		if ((err = snd_cs46xx_pcm_center_lfe(chip, 3)) < 0) {
			snd_card_free(card);
			return err;
		}
	}
#endif
	if ((err = snd_cs46xx_midi(chip, 0)) < 0) {
		snd_card_free(card);
		return err;
	}
	if ((err = snd_cs46xx_start_dsp(chip)) < 0) {
		snd_card_free(card);
		return err;
	}

=======
	err = snd_devm_card_new(&pci->dev, index[dev], id[dev], THIS_MODULE,
				sizeof(*chip), &card);
	if (err < 0)
		return err;
	chip = card->private_data;
	err = snd_cs46xx_create(card, pci,
				external_amp[dev], thinkpad[dev]);
	if (err < 0)
		return err;
	card->private_data = chip;
	chip->accept_valid = mmap_valid[dev];
	err = snd_cs46xx_pcm(chip, 0);
	if (err < 0)
		return err;
#ifdef CONFIG_SND_CS46XX_NEW_DSP
	err = snd_cs46xx_pcm_rear(chip, 1);
	if (err < 0)
		return err;
	err = snd_cs46xx_pcm_iec958(chip, 2);
	if (err < 0)
		return err;
#endif
	err = snd_cs46xx_mixer(chip, 2);
	if (err < 0)
		return err;
#ifdef CONFIG_SND_CS46XX_NEW_DSP
	if (chip->nr_ac97_codecs ==2) {
		err = snd_cs46xx_pcm_center_lfe(chip, 3);
		if (err < 0)
			return err;
	}
#endif
	err = snd_cs46xx_midi(chip, 0);
	if (err < 0)
		return err;
	err = snd_cs46xx_start_dsp(chip);
	if (err < 0)
		return err;
>>>>>>> upstream/android-13

	snd_cs46xx_gameport(chip);

	strcpy(card->driver, "CS46xx");
	strcpy(card->shortname, "Sound Fusion CS46xx");
	sprintf(card->longname, "%s at 0x%lx/0x%lx, irq %i",
		card->shortname,
		chip->ba0_addr,
		chip->ba1_addr,
		chip->irq);

<<<<<<< HEAD
	if ((err = snd_card_register(card)) < 0) {
		snd_card_free(card);
		return err;
	}
=======
	err = snd_card_register(card);
	if (err < 0)
		return err;
>>>>>>> upstream/android-13

	pci_set_drvdata(pci, card);
	dev++;
	return 0;
}

<<<<<<< HEAD
static void snd_card_cs46xx_remove(struct pci_dev *pci)
{
	snd_card_free(pci_get_drvdata(pci));
}

=======
>>>>>>> upstream/android-13
static struct pci_driver cs46xx_driver = {
	.name = KBUILD_MODNAME,
	.id_table = snd_cs46xx_ids,
	.probe = snd_card_cs46xx_probe,
<<<<<<< HEAD
	.remove = snd_card_cs46xx_remove,
=======
>>>>>>> upstream/android-13
#ifdef CONFIG_PM_SLEEP
	.driver = {
		.pm = &snd_cs46xx_pm,
	},
#endif
};

module_pci_driver(cs46xx_driver);
