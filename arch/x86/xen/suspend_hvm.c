// SPDX-License-Identifier: GPL-2.0
#include <linux/types.h>

#include <xen/xen.h>
<<<<<<< HEAD
=======
#include <xen/hvm.h>
>>>>>>> upstream/android-13
#include <xen/features.h>
#include <xen/interface/features.h>

#include "xen-ops.h"

void xen_hvm_post_suspend(int suspend_cancelled)
{
	if (!suspend_cancelled) {
		xen_hvm_init_shared_info();
		xen_vcpu_restore();
	}
<<<<<<< HEAD
	xen_callback_vector();
=======
	xen_setup_callback_vector();
>>>>>>> upstream/android-13
	xen_unplug_emulated_devices();
}
