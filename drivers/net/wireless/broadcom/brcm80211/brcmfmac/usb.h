<<<<<<< HEAD
/*
 * Copyright (c) 2011 Broadcom Corporation
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
 * Copyright (c) 2011 Broadcom Corporation
>>>>>>> upstream/android-13
 */
#ifndef BRCMFMAC_USB_H
#define BRCMFMAC_USB_H

enum brcmf_usb_state {
	BRCMFMAC_USB_STATE_DOWN,
	BRCMFMAC_USB_STATE_DL_FAIL,
	BRCMFMAC_USB_STATE_DL_DONE,
	BRCMFMAC_USB_STATE_UP,
	BRCMFMAC_USB_STATE_SLEEP
};

struct brcmf_stats {
	u32 tx_ctlpkts;
	u32 tx_ctlerrs;
	u32 rx_ctlpkts;
	u32 rx_ctlerrs;
};

struct brcmf_usbdev {
	struct brcmf_bus *bus;
	struct brcmf_usbdev_info *devinfo;
	enum brcmf_usb_state state;
	struct brcmf_stats stats;
	int ntxq, nrxq, rxsize;
	u32 bus_mtu;
	int devid;
<<<<<<< HEAD
	int chiprev; /* chip revsion number */
=======
	int chiprev; /* chip revision number */
>>>>>>> upstream/android-13
};

/* IO Request Block (IRB) */
struct brcmf_usbreq {
	struct list_head list;
	struct brcmf_usbdev_info *devinfo;
	struct urb *urb;
	struct sk_buff  *skb;
};

#endif /* BRCMFMAC_USB_H */
