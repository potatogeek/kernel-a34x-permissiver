<<<<<<< HEAD
// SPDX-License-Identifier: GPL-2.0+
=======
/* SPDX-License-Identifier: GPL-2.0+ */
>>>>>>> upstream/android-13
/*
 * bdc_ep.h - header for the BDC debug functions
 *
 * Copyright (C) 2014 Broadcom Corporation
 *
 * Author: Ashwini Pahuja
 */
#ifndef __LINUX_BDC_EP_H__
#define __LINUX_BDC_EP_H__

<<<<<<< HEAD
int bdc_init_ep(struct bdc *);
int bdc_ep_disable(struct bdc_ep *);
int bdc_ep_enable(struct bdc_ep *);
void bdc_free_ep(struct bdc *);
=======
int bdc_init_ep(struct bdc *bdc);
int bdc_ep_disable(struct bdc_ep *ep);
int bdc_ep_enable(struct bdc_ep *ep);
void bdc_free_ep(struct bdc *bdc);
>>>>>>> upstream/android-13

#endif /* __LINUX_BDC_EP_H__ */
