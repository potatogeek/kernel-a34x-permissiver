<<<<<<< HEAD
/*
 * Copyright (c) 2013 Broadcom Corporation
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
 * Copyright (c) 2013 Broadcom Corporation
>>>>>>> upstream/android-13
 */
#ifndef BRCMFMAC_BCDC_H
#define BRCMFMAC_BCDC_H

#ifdef CONFIG_BRCMFMAC_PROTO_BCDC
int brcmf_proto_bcdc_attach(struct brcmf_pub *drvr);
<<<<<<< HEAD
void brcmf_proto_bcdc_detach_pre_delif(struct brcmf_pub *drvr);
void brcmf_proto_bcdc_detach_post_delif(struct brcmf_pub *drvr);
=======
void brcmf_proto_bcdc_detach(struct brcmf_pub *drvr);
>>>>>>> upstream/android-13
void brcmf_proto_bcdc_txflowblock(struct device *dev, bool state);
void brcmf_proto_bcdc_txcomplete(struct device *dev, struct sk_buff *txp,
				 bool success);
struct brcmf_fws_info *drvr_to_fws(struct brcmf_pub *drvr);
#else
static inline int brcmf_proto_bcdc_attach(struct brcmf_pub *drvr) { return 0; }
<<<<<<< HEAD
static void brcmf_proto_bcdc_detach_pre_delif(struct brcmf_pub *drvr) {};
static inline void brcmf_proto_bcdc_detach_post_delif(struct brcmf_pub *drvr) {}
=======
static inline void brcmf_proto_bcdc_detach(struct brcmf_pub *drvr) {}
>>>>>>> upstream/android-13
#endif

#endif /* BRCMFMAC_BCDC_H */
