<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> upstream/android-13
/*
	Copyright (C) 2004 - 2009 Ivo van Doorn <IvDoorn@gmail.com>
	Copyright (C) 2004 - 2009 Gertjan van Wingerde <gwingerde@gmail.com>
	<http://rt2x00.serialmonkey.com>

<<<<<<< HEAD
	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, see <http://www.gnu.org/licenses/>.
=======
>>>>>>> upstream/android-13
 */

/*
	Module: rt2x00lib
	Abstract: rt2x00 firmware loading routines.
 */

#include <linux/kernel.h>
#include <linux/module.h>

#include "rt2x00.h"
#include "rt2x00lib.h"

static int rt2x00lib_request_firmware(struct rt2x00_dev *rt2x00dev)
{
	struct device *device = wiphy_dev(rt2x00dev->hw->wiphy);
	const struct firmware *fw;
	char *fw_name;
	int retval;

	/*
	 * Read correct firmware from harddisk.
	 */
	fw_name = rt2x00dev->ops->lib->get_firmware_name(rt2x00dev);
	if (!fw_name) {
		rt2x00_err(rt2x00dev,
			   "Invalid firmware filename\n"
			   "Please file bug report to %s\n", DRV_PROJECT);
		return -EINVAL;
	}

	rt2x00_info(rt2x00dev, "Loading firmware file '%s'\n", fw_name);

	retval = request_firmware(&fw, fw_name, device);
	if (retval) {
		rt2x00_err(rt2x00dev, "Failed to request Firmware\n");
		return retval;
	}

	if (!fw || !fw->size || !fw->data) {
		rt2x00_err(rt2x00dev, "Failed to read Firmware\n");
		release_firmware(fw);
		return -ENOENT;
	}

	rt2x00_info(rt2x00dev, "Firmware detected - version: %d.%d\n",
		    fw->data[fw->size - 4], fw->data[fw->size - 3]);
	snprintf(rt2x00dev->hw->wiphy->fw_version,
			sizeof(rt2x00dev->hw->wiphy->fw_version), "%d.%d",
			fw->data[fw->size - 4], fw->data[fw->size - 3]);

	retval = rt2x00dev->ops->lib->check_firmware(rt2x00dev, fw->data, fw->size);
	switch (retval) {
	case FW_OK:
		break;
	case FW_BAD_CRC:
		rt2x00_err(rt2x00dev, "Firmware checksum error\n");
		goto exit;
	case FW_BAD_LENGTH:
		rt2x00_err(rt2x00dev, "Invalid firmware file length (len=%zu)\n",
			   fw->size);
		goto exit;
	case FW_BAD_VERSION:
		rt2x00_err(rt2x00dev, "Current firmware does not support detected chipset\n");
		goto exit;
	}

	rt2x00dev->fw = fw;

	return 0;

exit:
	release_firmware(fw);

	return -ENOENT;
}

int rt2x00lib_load_firmware(struct rt2x00_dev *rt2x00dev)
{
	int retval;

	if (!rt2x00_has_cap_flag(rt2x00dev, REQUIRE_FIRMWARE))
		return 0;

	if (!rt2x00dev->fw) {
		retval = rt2x00lib_request_firmware(rt2x00dev);
		if (retval)
			return retval;
	}

	/*
	 * Send firmware to the device.
	 */
	retval = rt2x00dev->ops->lib->load_firmware(rt2x00dev,
						    rt2x00dev->fw->data,
						    rt2x00dev->fw->size);

	/*
	 * When the firmware is uploaded to the hardware the LED
	 * association status might have been triggered, for correct
	 * LED handling it should now be reset.
	 */
	rt2x00leds_led_assoc(rt2x00dev, false);

	return retval;
}

void rt2x00lib_free_firmware(struct rt2x00_dev *rt2x00dev)
{
	release_firmware(rt2x00dev->fw);
	rt2x00dev->fw = NULL;
}
