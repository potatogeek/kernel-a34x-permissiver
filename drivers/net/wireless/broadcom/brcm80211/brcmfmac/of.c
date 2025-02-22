<<<<<<< HEAD
/*
 * Copyright (c) 2014 Broadcom Corporation
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
 * SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION
 * OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
=======
// SPDX-License-Identifier: ISC
/*
 * Copyright (c) 2014 Broadcom Corporation
>>>>>>> upstream/android-13
 */
#include <linux/init.h>
#include <linux/of.h>
#include <linux/of_irq.h>

#include <defs.h>
#include "debug.h"
#include "core.h"
#include "common.h"
#include "of.h"

<<<<<<< HEAD
=======
static int brcmf_of_get_country_codes(struct device *dev,
				      struct brcmf_mp_device *settings)
{
	struct device_node *np = dev->of_node;
	struct brcmfmac_pd_cc_entry *cce;
	struct brcmfmac_pd_cc *cc;
	int count;
	int i;

	count = of_property_count_strings(np, "brcm,ccode-map");
	if (count < 0) {
		/* The property is optional, so return success if it doesn't
		 * exist. Otherwise propagate the error code.
		 */
		return (count == -EINVAL) ? 0 : count;
	}

	cc = devm_kzalloc(dev, sizeof(*cc) + count * sizeof(*cce), GFP_KERNEL);
	if (!cc)
		return -ENOMEM;

	cc->table_size = count;

	for (i = 0; i < count; i++) {
		const char *map;

		cce = &cc->table[i];

		if (of_property_read_string_index(np, "brcm,ccode-map",
						  i, &map))
			continue;

		/* String format e.g. US-Q2-86 */
		if (sscanf(map, "%2c-%2c-%d", cce->iso3166, cce->cc,
			   &cce->rev) != 3)
			brcmf_err("failed to read country map %s\n", map);
		else
			brcmf_dbg(INFO, "%s-%s-%d\n", cce->iso3166, cce->cc,
				  cce->rev);
	}

	settings->country_codes = cc;

	return 0;
}

>>>>>>> upstream/android-13
void brcmf_of_probe(struct device *dev, enum brcmf_bus_type bus_type,
		    struct brcmf_mp_device *settings)
{
	struct brcmfmac_sdio_pd *sdio = &settings->bus.sdio;
<<<<<<< HEAD
	struct device_node *np = dev->of_node;
	int irq;
	u32 irqf;
	u32 val;

	if (!np || bus_type != BRCMF_BUSTYPE_SDIO ||
	    !of_device_is_compatible(np, "brcm,bcm4329-fmac"))
=======
	struct device_node *root, *np = dev->of_node;
	int irq;
	int err;
	u32 irqf;
	u32 val;

	/* Set board-type to the first string of the machine compatible prop */
	root = of_find_node_by_path("/");
	if (root) {
		int i, len;
		char *board_type;
		const char *tmp;

		of_property_read_string_index(root, "compatible", 0, &tmp);

		/* get rid of '/' in the compatible string to be able to find the FW */
		len = strlen(tmp) + 1;
		board_type = devm_kzalloc(dev, len, GFP_KERNEL);
		strscpy(board_type, tmp, len);
		for (i = 0; i < board_type[i]; i++) {
			if (board_type[i] == '/')
				board_type[i] = '-';
		}
		settings->board_type = board_type;

		of_node_put(root);
	}

	if (!np || !of_device_is_compatible(np, "brcm,bcm4329-fmac"))
		return;

	err = brcmf_of_get_country_codes(dev, settings);
	if (err)
		brcmf_err("failed to get OF country code map (err=%d)\n", err);

	if (bus_type != BRCMF_BUSTYPE_SDIO)
>>>>>>> upstream/android-13
		return;

	if (of_property_read_u32(np, "brcm,drive-strength", &val) == 0)
		sdio->drive_strength = val;

	/* make sure there are interrupts defined in the node */
	if (!of_find_property(np, "interrupts", NULL))
		return;

	irq = irq_of_parse_and_map(np, 0);
	if (!irq) {
		brcmf_err("interrupt could not be mapped\n");
		return;
	}
	irqf = irqd_get_trigger_type(irq_get_irq_data(irq));

	sdio->oob_irq_supported = true;
	sdio->oob_irq_nr = irq;
	sdio->oob_irq_flags = irqf;
}
