<<<<<<< HEAD
/*
 * aQuantia Corporation Network Driver
 * Copyright (C) 2014-2017 aQuantia Corporation. All rights reserved
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
=======
/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * aQuantia Corporation Network Driver
 * Copyright (C) 2014-2017 aQuantia Corporation. All rights reserved
>>>>>>> upstream/android-13
 */

/* File aq_main.h: Main file for aQuantia Linux driver. */

#ifndef AQ_MAIN_H
#define AQ_MAIN_H

#include "aq_common.h"
<<<<<<< HEAD

=======
#include "aq_nic.h"

void aq_ndev_schedule_work(struct work_struct *work);
>>>>>>> upstream/android-13
struct net_device *aq_ndev_alloc(void);

#endif /* AQ_MAIN_H */
