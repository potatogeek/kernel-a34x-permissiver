<<<<<<< HEAD
/*
 * Copyright (c) 2007 Intel Corporation
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin St - Fifth Floor, Boston, MA 02110-1301 USA.
 *
=======
// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright (c) 2007 Intel Corporation
 *
>>>>>>> upstream/android-13
 * Authers: Jesse Barnes <jesse.barnes@intel.com>
 */

#include <linux/i2c.h>
<<<<<<< HEAD
#include <drm/drmP.h>
=======

>>>>>>> upstream/android-13
#include "psb_intel_drv.h"

/**
 * psb_intel_ddc_probe
<<<<<<< HEAD
 *
=======
 * @adapter:   Associated I2C adaptor
>>>>>>> upstream/android-13
 */
bool psb_intel_ddc_probe(struct i2c_adapter *adapter)
{
	u8 out_buf[] = { 0x0, 0x0 };
	u8 buf[2];
	int ret;
	struct i2c_msg msgs[] = {
		{
		 .addr = 0x50,
		 .flags = 0,
		 .len = 1,
		 .buf = out_buf,
		 },
		{
		 .addr = 0x50,
		 .flags = I2C_M_RD,
		 .len = 1,
		 .buf = buf,
		 }
	};

	ret = i2c_transfer(adapter, msgs, 2);
	if (ret == 2)
		return true;

	return false;
}

/**
 * psb_intel_ddc_get_modes - get modelist from monitor
 * @connector: DRM connector device to use
<<<<<<< HEAD
=======
 * @adapter:   Associated I2C adaptor
>>>>>>> upstream/android-13
 *
 * Fetch the EDID information from @connector using the DDC bus.
 */
int psb_intel_ddc_get_modes(struct drm_connector *connector,
			    struct i2c_adapter *adapter)
{
	struct edid *edid;
	int ret = 0;

	edid = drm_get_edid(connector, adapter);
	if (edid) {
		drm_connector_update_edid_property(connector, edid);
		ret = drm_add_edid_modes(connector, edid);
		kfree(edid);
	}
	return ret;
}
