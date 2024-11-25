<<<<<<< HEAD
// SPDX-License-Identifier: GPL-2.0
/*
=======
// SPDX-License-Identifier: GPL-2.0-only
/*
 * MMC crypto engine (inline encryption) support
 *
>>>>>>> upstream/android-13
 * Copyright 2020 Google LLC
 */

#include <linux/blk-crypto.h>
<<<<<<< HEAD
#include <linux/blkdev.h>
#include <linux/keyslot-manager.h>
#include <linux/mmc/host.h>

#include "core.h"
#include "queue.h"

void mmc_crypto_setup_queue(struct mmc_host *host, struct request_queue *q)
{
	if (host->caps2 & MMC_CAP2_CRYPTO)
		q->ksm = host->ksm;
}
EXPORT_SYMBOL_GPL(mmc_crypto_setup_queue);

void mmc_crypto_free_host(struct mmc_host *host)
{
	keyslot_manager_destroy(host->ksm);
}

=======
#include <linux/mmc/host.h>

#include "core.h"
#include "crypto.h"
#include "queue.h"

void mmc_crypto_set_initial_state(struct mmc_host *host)
{
	/* Reset might clear all keys, so reprogram all the keys. */
	if (host->caps2 & MMC_CAP2_CRYPTO)
		blk_ksm_reprogram_all_keys(&host->ksm);
}

void mmc_crypto_setup_queue(struct request_queue *q, struct mmc_host *host)
{
	if (host->caps2 & MMC_CAP2_CRYPTO)
		blk_ksm_register(&host->ksm, q);
}
EXPORT_SYMBOL_GPL(mmc_crypto_setup_queue);

>>>>>>> upstream/android-13
void mmc_crypto_prepare_req(struct mmc_queue_req *mqrq)
{
	struct request *req = mmc_queue_req_to_req(mqrq);
	struct mmc_request *mrq = &mqrq->brq.mrq;
<<<<<<< HEAD
	const struct bio_crypt_ctx *bc;

	if (!bio_crypt_should_process(req))
		return;

	bc = req->bio->bi_crypt_context;
	mrq->crypto_key_slot = bc->bc_keyslot;
	/*
	 * OTA with ext4 (dun is 512 bytes) used LBA,
	 * with F2FS (dun is 512 bytes), the dun[0] had
	 * multiplied by 8.
	 */
	if (bc->hie_ext4 == true)
		mrq->data_unit_num = blk_rq_pos(req);
	else
		mrq->data_unit_num = lower_32_bits(bc->bc_dun[0]);
	mrq->crypto_key = bc->bc_key;
=======

	if (!req->crypt_ctx)
		return;

	mrq->crypto_ctx = req->crypt_ctx;
	if (req->crypt_keyslot)
		mrq->crypto_key_slot = blk_ksm_get_slot_idx(req->crypt_keyslot);
>>>>>>> upstream/android-13
}
EXPORT_SYMBOL_GPL(mmc_crypto_prepare_req);
