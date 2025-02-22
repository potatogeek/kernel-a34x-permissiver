<<<<<<< HEAD
/*
 *  This program is free software; you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License as published by the Free
 *  Software Foundation; either version 2 of the License, or (at your option)
 *  any later version.
 *
 *  This program is distributed in the hope that it will be useful, but WITHOUT
 *  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 *  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 *  more details.
 *
 *  You should have received a copy of the GNU General Public License along with
 *  this program; if not, write to the Free Software Foundation, Inc., 59
 *  Temple Place - Suite 330, Boston, MA  02111-1307, USA.
=======
/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
>>>>>>> upstream/android-13
 */
#ifndef __SOUND_HDA_INTEL_H
#define __SOUND_HDA_INTEL_H

#include "hda_controller.h"

struct hda_intel {
	struct azx chip;

	/* for pending irqs */
	struct work_struct irq_pending_work;

	/* sync probing */
	struct completion probe_wait;
<<<<<<< HEAD
	struct work_struct probe_work;
=======
	struct delayed_work probe_work;
>>>>>>> upstream/android-13

	/* card list (for power_save trigger) */
	struct list_head list;

	/* extra flags */
	unsigned int irq_pending_warned:1;
	unsigned int probe_continued:1;

	/* vga_switcheroo setup */
	unsigned int use_vga_switcheroo:1;
<<<<<<< HEAD
	unsigned int need_eld_notify_link:1;
	unsigned int vga_switcheroo_registered:1;
	unsigned int init_failed:1; /* delayed init failed */

	bool need_i915_power:1; /* the hda controller needs i915 power */
=======
	unsigned int vga_switcheroo_registered:1;
	unsigned int init_failed:1; /* delayed init failed */
	unsigned int freed:1; /* resources already released */

	bool need_i915_power:1; /* the hda controller needs i915 power */

	int probe_retry;	/* being probe-retry */
>>>>>>> upstream/android-13
};

#endif
