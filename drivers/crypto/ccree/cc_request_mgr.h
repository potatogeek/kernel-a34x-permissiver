/* SPDX-License-Identifier: GPL-2.0 */
<<<<<<< HEAD
/* Copyright (C) 2012-2018 ARM Limited or its affiliates. */
=======
/* Copyright (C) 2012-2019 ARM Limited (or its affiliates). */
>>>>>>> upstream/android-13

/* \file cc_request_mgr.h
 * Request Manager
 */

#ifndef __REQUEST_MGR_H__
#define __REQUEST_MGR_H__

#include "cc_hw_queue_defs.h"

int cc_req_mgr_init(struct cc_drvdata *drvdata);

<<<<<<< HEAD
/*!
 * Enqueue caller request to crypto hardware.
 *
 * \param drvdata
 * \param cc_req The request to enqueue
 * \param desc The crypto sequence
 * \param len The crypto sequence length
 * \param is_dout If "true": completion is handled by the caller
 *	  If "false": this function adds a dummy descriptor completion
 *	  and waits upon completion signal.
 *
 * \return int Returns -EINPROGRESS or error
=======
/**
 * cc_send_request() - Enqueue caller request to crypto hardware.
 *
 * @drvdata: Associated device driver context
 * @cc_req: The request to enqueue
 * @desc: The crypto sequence
 * @len: The crypto sequence length
 * @req: Asynchronous crypto request
 *
 * Return:
 * Returns -EINPROGRESS or error
>>>>>>> upstream/android-13
 */
int cc_send_request(struct cc_drvdata *drvdata, struct cc_crypto_req *cc_req,
		    struct cc_hw_desc *desc, unsigned int len,
		    struct crypto_async_request *req);

int cc_send_sync_request(struct cc_drvdata *drvdata,
			 struct cc_crypto_req *cc_req, struct cc_hw_desc *desc,
			 unsigned int len);

int send_request_init(struct cc_drvdata *drvdata, struct cc_hw_desc *desc,
		      unsigned int len);

void complete_request(struct cc_drvdata *drvdata);

void cc_req_mgr_fini(struct cc_drvdata *drvdata);

#endif /*__REQUEST_MGR_H__*/
