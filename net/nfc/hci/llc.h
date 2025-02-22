<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> upstream/android-13
/*
 * Link Layer Control manager
 *
 * Copyright (C) 2012  Intel Corporation. All rights reserved.
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see <http://www.gnu.org/licenses/>.
=======
>>>>>>> upstream/android-13
 */

#ifndef __LOCAL_LLC_H_
#define __LOCAL_LLC_H_

#include <net/nfc/hci.h>
#include <net/nfc/llc.h>
#include <linux/skbuff.h>

struct nfc_llc_ops {
	void *(*init) (struct nfc_hci_dev *hdev, xmit_to_drv_t xmit_to_drv,
		       rcv_to_hci_t rcv_to_hci, int tx_headroom,
		       int tx_tailroom, int *rx_headroom, int *rx_tailroom,
		       llc_failure_t llc_failure);
	void (*deinit) (struct nfc_llc *llc);
	int (*start) (struct nfc_llc *llc);
	int (*stop) (struct nfc_llc *llc);
	void (*rcv_from_drv) (struct nfc_llc *llc, struct sk_buff *skb);
	int (*xmit_from_hci) (struct nfc_llc *llc, struct sk_buff *skb);
};

struct nfc_llc_engine {
	const char *name;
<<<<<<< HEAD
	struct nfc_llc_ops *ops;
=======
	const struct nfc_llc_ops *ops;
>>>>>>> upstream/android-13
	struct list_head entry;
};

struct nfc_llc {
	void *data;
<<<<<<< HEAD
	struct nfc_llc_ops *ops;
=======
	const struct nfc_llc_ops *ops;
>>>>>>> upstream/android-13
	int rx_headroom;
	int rx_tailroom;
};

void *nfc_llc_get_data(struct nfc_llc *llc);

<<<<<<< HEAD
int nfc_llc_register(const char *name, struct nfc_llc_ops *ops);
=======
int nfc_llc_register(const char *name, const struct nfc_llc_ops *ops);
>>>>>>> upstream/android-13
void nfc_llc_unregister(const char *name);

int nfc_llc_nop_register(void);

#if defined(CONFIG_NFC_SHDLC)
int nfc_llc_shdlc_register(void);
#else
static inline int nfc_llc_shdlc_register(void)
{
	return 0;
}
#endif

#endif /* __LOCAL_LLC_H_ */
