<<<<<<< HEAD
/* SPDX-License-Identifier: GPL-2.0 */
/*
=======
/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * MMC crypto engine (inline encryption) support
 *
>>>>>>> upstream/android-13
 * Copyright 2020 Google LLC
 */

#ifndef _MMC_CORE_CRYPTO_H
#define _MMC_CORE_CRYPTO_H

struct mmc_host;
struct mmc_queue_req;
<<<<<<< HEAD
struct request;
=======
>>>>>>> upstream/android-13
struct request_queue;

#ifdef CONFIG_MMC_CRYPTO

<<<<<<< HEAD
void mmc_crypto_setup_queue(struct mmc_host *host, struct request_queue *q);

void mmc_crypto_free_host(struct mmc_host *host);
=======
void mmc_crypto_set_initial_state(struct mmc_host *host);

void mmc_crypto_setup_queue(struct request_queue *q, struct mmc_host *host);
>>>>>>> upstream/android-13

void mmc_crypto_prepare_req(struct mmc_queue_req *mqrq);

#else /* CONFIG_MMC_CRYPTO */

<<<<<<< HEAD
static inline void mmc_crypto_setup_queue(struct mmc_host *host,
					  struct request_queue *q) { }

static inline void mmc_crypto_free_host(struct mmc_host *host) { }

static inline void mmc_crypto_prepare_req(struct mmc_queue_req *mqrq) { }

#endif /* CONFIG_MMC_CRYPTO */
=======
static inline void mmc_crypto_set_initial_state(struct mmc_host *host)
{
}

static inline void mmc_crypto_setup_queue(struct request_queue *q,
					  struct mmc_host *host)
{
}

static inline void mmc_crypto_prepare_req(struct mmc_queue_req *mqrq)
{
}

#endif /* !CONFIG_MMC_CRYPTO */
>>>>>>> upstream/android-13

#endif /* _MMC_CORE_CRYPTO_H */
