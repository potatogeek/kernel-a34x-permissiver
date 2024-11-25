<<<<<<< HEAD
// SPDX-License-Identifier: GPL-2.0
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> upstream/android-13
/*
 * xhci-plat.h - xHCI host controller driver platform Bus Glue.
 *
 * Copyright (C) 2015 Renesas Electronics Corporation
 */

#ifndef _XHCI_PLAT_H
#define _XHCI_PLAT_H

#include "xhci.h"	/* for hcd_to_xhci() */

struct xhci_plat_priv {
	const char *firmware_name;
<<<<<<< HEAD
	void (*plat_start)(struct usb_hcd *);
	int (*init_quirk)(struct usb_hcd *);
=======
	unsigned long long quirks;
	int (*plat_setup)(struct usb_hcd *);
	void (*plat_start)(struct usb_hcd *);
	int (*init_quirk)(struct usb_hcd *);
	int (*suspend_quirk)(struct usb_hcd *);
>>>>>>> upstream/android-13
	int (*resume_quirk)(struct usb_hcd *);
};

#define hcd_to_xhci_priv(h) ((struct xhci_plat_priv *)hcd_to_xhci(h)->priv)
<<<<<<< HEAD

#if IS_ENABLED(CONFIG_MTK_USB_OFFLOAD)
struct xhci_plat_priv_overwrite {
	struct xhci_vendor_ops *vendor_ops;
};

int xhci_plat_register_vendor_ops(struct xhci_vendor_ops *vendor_ops);
#endif
=======
#define xhci_to_priv(x) ((struct xhci_plat_priv *)(x)->priv)
>>>>>>> upstream/android-13
#endif	/* _XHCI_PLAT_H */
