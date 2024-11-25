<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-or-later */
>>>>>>> upstream/android-13
/*
 * Crypto engine API
 *
 * Copyright (c) 2016 Baolin Wang <baolin.wang@linaro.org>
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version.
 *
=======
>>>>>>> upstream/android-13
 */
#ifndef _CRYPTO_ENGINE_H
#define _CRYPTO_ENGINE_H

#include <linux/crypto.h>
#include <linux/list.h>
#include <linux/kernel.h>
#include <linux/kthread.h>
#include <crypto/algapi.h>
#include <crypto/aead.h>
#include <crypto/akcipher.h>
#include <crypto/hash.h>
#include <crypto/skcipher.h>

#define ENGINE_NAME_LEN	30
/*
 * struct crypto_engine - crypto hardware engine
 * @name: the engine name
 * @idling: the engine is entering idle state
 * @busy: request pump is busy
 * @running: the engine is on working
<<<<<<< HEAD
 * @cur_req_prepared: current request is prepared
 * @list: link with the global crypto engine list
 * @queue_lock: spinlock to syncronise access to request queue
=======
 * @retry_support: indication that the hardware allows re-execution
 * of a failed backlog request
 * crypto-engine, in head position to keep order
 * @list: link with the global crypto engine list
 * @queue_lock: spinlock to synchronise access to request queue
>>>>>>> upstream/android-13
 * @queue: the crypto queue of the engine
 * @rt: whether this queue is set to run as a realtime task
 * @prepare_crypt_hardware: a request will soon arrive from the queue
 * so the subsystem requests the driver to prepare the hardware
 * by issuing this call
 * @unprepare_crypt_hardware: there are currently no more requests on the
 * queue so the subsystem notifies the driver that it may relax the
 * hardware by issuing this call
<<<<<<< HEAD
=======
 * @do_batch_requests: execute a batch of requests. Depends on multiple
 * requests support.
>>>>>>> upstream/android-13
 * @kworker: kthread worker struct for request pump
 * @pump_requests: work struct for scheduling work to the request pump
 * @priv_data: the engine private data
 * @cur_req: the current request which is on processing
 */
struct crypto_engine {
	char			name[ENGINE_NAME_LEN];
	bool			idling;
	bool			busy;
	bool			running;
<<<<<<< HEAD
	bool			cur_req_prepared;
=======

	bool			retry_support;
>>>>>>> upstream/android-13

	struct list_head	list;
	spinlock_t		queue_lock;
	struct crypto_queue	queue;
	struct device		*dev;

	bool			rt;

	int (*prepare_crypt_hardware)(struct crypto_engine *engine);
	int (*unprepare_crypt_hardware)(struct crypto_engine *engine);
<<<<<<< HEAD
=======
	int (*do_batch_requests)(struct crypto_engine *engine);

>>>>>>> upstream/android-13

	struct kthread_worker           *kworker;
	struct kthread_work             pump_requests;

	void				*priv_data;
	struct crypto_async_request	*cur_req;
};

/*
 * struct crypto_engine_op - crypto hardware engine operations
 * @prepare__request: do some prepare if need before handle the current request
 * @unprepare_request: undo any work done by prepare_request()
 * @do_one_request: do encryption for current request
 */
struct crypto_engine_op {
	int (*prepare_request)(struct crypto_engine *engine,
			       void *areq);
	int (*unprepare_request)(struct crypto_engine *engine,
				 void *areq);
	int (*do_one_request)(struct crypto_engine *engine,
			      void *areq);
};

struct crypto_engine_ctx {
	struct crypto_engine_op op;
};

<<<<<<< HEAD
int crypto_transfer_ablkcipher_request_to_engine(struct crypto_engine *engine,
						 struct ablkcipher_request *req);
=======
>>>>>>> upstream/android-13
int crypto_transfer_aead_request_to_engine(struct crypto_engine *engine,
					   struct aead_request *req);
int crypto_transfer_akcipher_request_to_engine(struct crypto_engine *engine,
					       struct akcipher_request *req);
int crypto_transfer_hash_request_to_engine(struct crypto_engine *engine,
					       struct ahash_request *req);
int crypto_transfer_skcipher_request_to_engine(struct crypto_engine *engine,
					       struct skcipher_request *req);
<<<<<<< HEAD
void crypto_finalize_ablkcipher_request(struct crypto_engine *engine,
					struct ablkcipher_request *req, int err);
=======
>>>>>>> upstream/android-13
void crypto_finalize_aead_request(struct crypto_engine *engine,
				  struct aead_request *req, int err);
void crypto_finalize_akcipher_request(struct crypto_engine *engine,
				      struct akcipher_request *req, int err);
void crypto_finalize_hash_request(struct crypto_engine *engine,
				  struct ahash_request *req, int err);
void crypto_finalize_skcipher_request(struct crypto_engine *engine,
				      struct skcipher_request *req, int err);
int crypto_engine_start(struct crypto_engine *engine);
int crypto_engine_stop(struct crypto_engine *engine);
struct crypto_engine *crypto_engine_alloc_init(struct device *dev, bool rt);
<<<<<<< HEAD
=======
struct crypto_engine *crypto_engine_alloc_init_and_set(struct device *dev,
						       bool retry_support,
						       int (*cbk_do_batch)(struct crypto_engine *engine),
						       bool rt, int qlen);
>>>>>>> upstream/android-13
int crypto_engine_exit(struct crypto_engine *engine);

#endif /* _CRYPTO_ENGINE_H */
