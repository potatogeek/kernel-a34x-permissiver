<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> upstream/android-13
/*
 *  Driver for Trident 4DWave DX/NX & SiS SI7018 Audio PCI soundcard
 *
 *  Driver was originated by Trident <audio@tridentmicro.com>
 *  			     Fri Feb 19 15:55:28 MST 1999
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
 *
=======
>>>>>>> upstream/android-13
 */

#include <linux/init.h>
#include <linux/pci.h>
#include <linux/time.h>
#include <linux/module.h>
#include <sound/core.h>
#include "trident.h"
#include <sound/initval.h>

MODULE_AUTHOR("Jaroslav Kysela <perex@perex.cz>, <audio@tridentmicro.com>");
MODULE_DESCRIPTION("Trident 4D-WaveDX/NX & SiS SI7018");
MODULE_LICENSE("GPL");
<<<<<<< HEAD
MODULE_SUPPORTED_DEVICE("{{Trident,4DWave DX},"
		"{Trident,4DWave NX},"
		"{SiS,SI7018 PCI Audio},"
		"{Best Union,Miss Melody 4DWave PCI},"
		"{HIS,4DWave PCI},"
		"{Warpspeed,ONSpeed 4DWave PCI},"
		"{Aztech Systems,PCI 64-Q3D},"
		"{Addonics,SV 750},"
		"{CHIC,True Sound 4Dwave},"
		"{Shark,Predator4D-PCI},"
		"{Jaton,SonicWave 4D},"
		"{Hoontech,SoundTrack Digital 4DWave NX}}");
=======
>>>>>>> upstream/android-13

static int index[SNDRV_CARDS] = SNDRV_DEFAULT_IDX;	/* Index 0-MAX */
static char *id[SNDRV_CARDS] = SNDRV_DEFAULT_STR;	/* ID for this card */
static bool enable[SNDRV_CARDS] = SNDRV_DEFAULT_ENABLE_PNP;	/* Enable this card */
static int pcm_channels[SNDRV_CARDS] = {[0 ... (SNDRV_CARDS - 1)] = 32};
static int wavetable_size[SNDRV_CARDS] = {[0 ... (SNDRV_CARDS - 1)] = 8192};

module_param_array(index, int, NULL, 0444);
MODULE_PARM_DESC(index, "Index value for Trident 4DWave PCI soundcard.");
module_param_array(id, charp, NULL, 0444);
MODULE_PARM_DESC(id, "ID string for Trident 4DWave PCI soundcard.");
module_param_array(enable, bool, NULL, 0444);
MODULE_PARM_DESC(enable, "Enable Trident 4DWave PCI soundcard.");
module_param_array(pcm_channels, int, NULL, 0444);
MODULE_PARM_DESC(pcm_channels, "Number of hardware channels assigned for PCM.");
module_param_array(wavetable_size, int, NULL, 0444);
MODULE_PARM_DESC(wavetable_size, "Maximum memory size in kB for wavetable synth.");

static const struct pci_device_id snd_trident_ids[] = {
	{PCI_DEVICE(PCI_VENDOR_ID_TRIDENT, PCI_DEVICE_ID_TRIDENT_4DWAVE_DX), 
		PCI_CLASS_MULTIMEDIA_AUDIO << 8, 0xffff00, 0},
	{PCI_DEVICE(PCI_VENDOR_ID_TRIDENT, PCI_DEVICE_ID_TRIDENT_4DWAVE_NX), 
		0, 0, 0},
	{PCI_DEVICE(PCI_VENDOR_ID_SI, PCI_DEVICE_ID_SI_7018), 0, 0, 0},
	{ 0, }
};

MODULE_DEVICE_TABLE(pci, snd_trident_ids);

static int snd_trident_probe(struct pci_dev *pci,
			     const struct pci_device_id *pci_id)
{
	static int dev;
	struct snd_card *card;
	struct snd_trident *trident;
	const char *str;
	int err, pcm_dev = 0;

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

	if ((err = snd_trident_create(card, pci,
				      pcm_channels[dev],
				      ((pci->vendor << 16) | pci->device) == TRIDENT_DEVICE_ID_SI7018 ? 1 : 2,
				      wavetable_size[dev],
				      &trident)) < 0) {
		snd_card_free(card);
		return err;
	}
	card->private_data = trident;
=======
	err = snd_devm_card_new(&pci->dev, index[dev], id[dev], THIS_MODULE,
				sizeof(*trident), &card);
	if (err < 0)
		return err;
	trident = card->private_data;

	err = snd_trident_create(card, pci,
				 pcm_channels[dev],
				 ((pci->vendor << 16) | pci->device) == TRIDENT_DEVICE_ID_SI7018 ? 1 : 2,
				 wavetable_size[dev]);
	if (err < 0)
		return err;
>>>>>>> upstream/android-13

	switch (trident->device) {
	case TRIDENT_DEVICE_ID_DX:
		str = "TRID4DWAVEDX";
		break;
	case TRIDENT_DEVICE_ID_NX:
		str = "TRID4DWAVENX";
		break;
	case TRIDENT_DEVICE_ID_SI7018:
		str = "SI7018";
		break;
	default:
		str = "Unknown";
	}
	strcpy(card->driver, str);
	if (trident->device == TRIDENT_DEVICE_ID_SI7018) {
		strcpy(card->shortname, "SiS ");
	} else {
		strcpy(card->shortname, "Trident ");
	}
	strcat(card->shortname, str);
	sprintf(card->longname, "%s PCI Audio at 0x%lx, irq %d",
		card->shortname, trident->port, trident->irq);

<<<<<<< HEAD
	if ((err = snd_trident_pcm(trident, pcm_dev++)) < 0) {
		snd_card_free(card);
		return err;
	}
	switch (trident->device) {
	case TRIDENT_DEVICE_ID_DX:
	case TRIDENT_DEVICE_ID_NX:
		if ((err = snd_trident_foldback_pcm(trident, pcm_dev++)) < 0) {
			snd_card_free(card);
			return err;
		}
		break;
	}
	if (trident->device == TRIDENT_DEVICE_ID_NX || trident->device == TRIDENT_DEVICE_ID_SI7018) {
		if ((err = snd_trident_spdif_pcm(trident, pcm_dev++)) < 0) {
			snd_card_free(card);
			return err;
		}
	}
	if (trident->device != TRIDENT_DEVICE_ID_SI7018 &&
	    (err = snd_mpu401_uart_new(card, 0, MPU401_HW_TRID4DWAVE,
				       trident->midi_port,
				       MPU401_INFO_INTEGRATED |
				       MPU401_INFO_IRQ_HOOK,
				       -1, &trident->rmidi)) < 0) {
		snd_card_free(card);
		return err;
=======
	err = snd_trident_pcm(trident, pcm_dev++);
	if (err < 0)
		return err;
	switch (trident->device) {
	case TRIDENT_DEVICE_ID_DX:
	case TRIDENT_DEVICE_ID_NX:
		err = snd_trident_foldback_pcm(trident, pcm_dev++);
		if (err < 0)
			return err;
		break;
	}
	if (trident->device == TRIDENT_DEVICE_ID_NX || trident->device == TRIDENT_DEVICE_ID_SI7018) {
		err = snd_trident_spdif_pcm(trident, pcm_dev++);
		if (err < 0)
			return err;
	}
	if (trident->device != TRIDENT_DEVICE_ID_SI7018) {
		err = snd_mpu401_uart_new(card, 0, MPU401_HW_TRID4DWAVE,
					  trident->midi_port,
					  MPU401_INFO_INTEGRATED |
					  MPU401_INFO_IRQ_HOOK,
					  -1, &trident->rmidi);
		if (err < 0)
			return err;
>>>>>>> upstream/android-13
	}

	snd_trident_create_gameport(trident);

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
static void snd_trident_remove(struct pci_dev *pci)
{
	snd_card_free(pci_get_drvdata(pci));
}

=======
>>>>>>> upstream/android-13
static struct pci_driver trident_driver = {
	.name = KBUILD_MODNAME,
	.id_table = snd_trident_ids,
	.probe = snd_trident_probe,
<<<<<<< HEAD
	.remove = snd_trident_remove,
=======
>>>>>>> upstream/android-13
#ifdef CONFIG_PM_SLEEP
	.driver = {
		.pm = &snd_trident_pm,
	},
#endif
};

module_pci_driver(trident_driver);
