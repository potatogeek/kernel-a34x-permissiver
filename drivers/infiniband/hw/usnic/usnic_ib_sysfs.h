/*
 * Copyright (c) 2013, Cisco Systems, Inc. All rights reserved.
 *
 * This software is available to you under a choice of one of two
 * licenses.  You may choose to be licensed under the terms of the GNU
 * General Public License (GPL) Version 2, available from the file
 * COPYING in the main directory of this source tree, or the
 * BSD license below:
 *
 *     Redistribution and use in source and binary forms, with or
 *     without modification, are permitted provided that the following
 *     conditions are met:
 *
 *      - Redistributions of source code must retain the above
 *        copyright notice, this list of conditions and the following
 *        disclaimer.
 *
 *      - Redistributions in binary form must reproduce the above
 *        copyright notice, this list of conditions and the following
 *        disclaimer in the documentation and/or other materials
 *        provided with the distribution.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */

#ifndef USNIC_IB_SYSFS_H_
#define USNIC_IB_SYSFS_H_

#include "usnic_ib.h"

int usnic_ib_sysfs_register_usdev(struct usnic_ib_dev *us_ibdev);
void usnic_ib_sysfs_unregister_usdev(struct usnic_ib_dev *us_ibdev);
void usnic_ib_sysfs_qpn_add(struct usnic_ib_qp_grp *qp_grp);
void usnic_ib_sysfs_qpn_remove(struct usnic_ib_qp_grp *qp_grp);

<<<<<<< HEAD
=======
extern const struct attribute_group usnic_attr_group;

>>>>>>> upstream/android-13
#endif /* !USNIC_IB_SYSFS_H_ */
