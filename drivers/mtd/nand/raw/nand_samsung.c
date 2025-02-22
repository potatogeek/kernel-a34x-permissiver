<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> upstream/android-13
/*
 * Copyright (C) 2017 Free Electrons
 * Copyright (C) 2017 NextThing Co
 *
 * Author: Boris Brezillon <boris.brezillon@free-electrons.com>
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include <linux/mtd/rawnand.h>

static void samsung_nand_decode_id(struct nand_chip *chip)
{
	struct mtd_info *mtd = nand_to_mtd(chip);
=======
 */

#include "internals.h"

static void samsung_nand_decode_id(struct nand_chip *chip)
{
	struct nand_device *base = &chip->base;
	struct nand_ecc_props requirements = {};
	struct mtd_info *mtd = nand_to_mtd(chip);
	struct nand_memory_organization *memorg;

	memorg = nanddev_get_memorg(&chip->base);
>>>>>>> upstream/android-13

	/* New Samsung (6 byte ID): Samsung K9GAG08U0F (p.44) */
	if (chip->id.len == 6 && !nand_is_slc(chip) &&
	    chip->id.data[5] != 0x00) {
		u8 extid = chip->id.data[3];

		/* Get pagesize */
<<<<<<< HEAD
		mtd->writesize = 2048 << (extid & 0x03);
=======
		memorg->pagesize = 2048 << (extid & 0x03);
		mtd->writesize = memorg->pagesize;
>>>>>>> upstream/android-13

		extid >>= 2;

		/* Get oobsize */
		switch (((extid >> 2) & 0x4) | (extid & 0x3)) {
		case 1:
<<<<<<< HEAD
			mtd->oobsize = 128;
			break;
		case 2:
			mtd->oobsize = 218;
			break;
		case 3:
			mtd->oobsize = 400;
			break;
		case 4:
			mtd->oobsize = 436;
			break;
		case 5:
			mtd->oobsize = 512;
			break;
		case 6:
			mtd->oobsize = 640;
=======
			memorg->oobsize = 128;
			break;
		case 2:
			memorg->oobsize = 218;
			break;
		case 3:
			memorg->oobsize = 400;
			break;
		case 4:
			memorg->oobsize = 436;
			break;
		case 5:
			memorg->oobsize = 512;
			break;
		case 6:
			memorg->oobsize = 640;
>>>>>>> upstream/android-13
			break;
		default:
			/*
			 * We should never reach this case, but if that
			 * happens, this probably means Samsung decided to use
			 * a different extended ID format, and we should find
			 * a way to support it.
			 */
			WARN(1, "Invalid OOB size value");
			break;
		}

<<<<<<< HEAD
		/* Get blocksize */
		extid >>= 2;
=======
		mtd->oobsize = memorg->oobsize;

		/* Get blocksize */
		extid >>= 2;
		memorg->pages_per_eraseblock = (128 * 1024) <<
					       (((extid >> 1) & 0x04) |
						(extid & 0x03)) /
					       memorg->pagesize;
>>>>>>> upstream/android-13
		mtd->erasesize = (128 * 1024) <<
				 (((extid >> 1) & 0x04) | (extid & 0x03));

		/* Extract ECC requirements from 5th id byte*/
		extid = (chip->id.data[4] >> 4) & 0x07;
		if (extid < 5) {
<<<<<<< HEAD
			chip->ecc_step_ds = 512;
			chip->ecc_strength_ds = 1 << extid;
		} else {
			chip->ecc_step_ds = 1024;
			switch (extid) {
			case 5:
				chip->ecc_strength_ds = 24;
				break;
			case 6:
				chip->ecc_strength_ds = 40;
				break;
			case 7:
				chip->ecc_strength_ds = 60;
				break;
			default:
				WARN(1, "Could not decode ECC info");
				chip->ecc_step_ds = 0;
=======
			requirements.step_size = 512;
			requirements.strength = 1 << extid;
		} else {
			requirements.step_size = 1024;
			switch (extid) {
			case 5:
				requirements.strength = 24;
				break;
			case 6:
				requirements.strength = 40;
				break;
			case 7:
				requirements.strength = 60;
				break;
			default:
				WARN(1, "Could not decode ECC info");
				requirements.step_size = 0;
>>>>>>> upstream/android-13
			}
		}
	} else {
		nand_decode_ext_id(chip);

		if (nand_is_slc(chip)) {
			switch (chip->id.data[1]) {
			/* K9F4G08U0D-S[I|C]B0(T00) */
			case 0xDC:
<<<<<<< HEAD
				chip->ecc_step_ds = 512;
				chip->ecc_strength_ds = 1;
=======
				requirements.step_size = 512;
				requirements.strength = 1;
>>>>>>> upstream/android-13
				break;

			/* K9F1G08U0E 21nm chips do not support subpage write */
			case 0xF1:
				if (chip->id.len > 4 &&
				    (chip->id.data[4] & GENMASK(1, 0)) == 0x1)
					chip->options |= NAND_NO_SUBPAGE_WRITE;
				break;
			default:
				break;
			}
		}
	}
<<<<<<< HEAD
=======

	nanddev_set_ecc_requirements(base, &requirements);
>>>>>>> upstream/android-13
}

static int samsung_nand_init(struct nand_chip *chip)
{
	struct mtd_info *mtd = nand_to_mtd(chip);

	if (mtd->writesize > 512)
		chip->options |= NAND_SAMSUNG_LP_OPTIONS;

	if (!nand_is_slc(chip))
<<<<<<< HEAD
		chip->bbt_options |= NAND_BBT_SCANLASTPAGE;
	else
		chip->bbt_options |= NAND_BBT_SCAN2NDPAGE;
=======
		chip->options |= NAND_BBM_LASTPAGE;
	else
		chip->options |= NAND_BBM_FIRSTPAGE | NAND_BBM_SECONDPAGE;
>>>>>>> upstream/android-13

	return 0;
}

const struct nand_manufacturer_ops samsung_nand_manuf_ops = {
	.detect = samsung_nand_decode_id,
	.init = samsung_nand_init,
};
