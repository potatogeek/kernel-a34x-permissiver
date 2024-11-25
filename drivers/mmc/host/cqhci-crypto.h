<<<<<<< HEAD
/* SPDX-License-Identifier: GPL-2.0 */
/*
=======
/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * CQHCI crypto engine (inline encryption) support
 *
>>>>>>> upstream/android-13
 * Copyright 2020 Google LLC
 */

#ifndef LINUX_MMC_CQHCI_CRYPTO_H
#define LINUX_MMC_CQHCI_CRYPTO_H

<<<<<<< HEAD
=======
#include <linux/mmc/host.h>

>>>>>>> upstream/android-13
#include "cqhci.h"

#ifdef CONFIG_MMC_CRYPTO

<<<<<<< HEAD
bool cqhci_crypto_enable(struct cqhci_host *host);

int cqhci_host_init_crypto(struct cqhci_host *host);

int cqhci_prep_crypto_desc(struct mmc_request *mrq, __le64 *task_desc);

void cqhci_crypto_recovery_finish(struct cqhci_host *host);

#else /* CONFIG_MMC_CRYPTO */

static inline bool cqhci_crypto_enable(struct cqhci_host *host)
{
	return false;
}

static inline int cqhci_host_init_crypto(struct cqhci_host *host)
=======
int cqhci_crypto_init(struct cqhci_host *host);

/*
 * Returns the crypto bits that should be set in bits 64-127 of the
 * task descriptor.
 */
static inline u64 cqhci_crypto_prep_task_desc(struct mmc_request *mrq)
{
	if (!mrq->crypto_ctx)
		return 0;

	/* We set max_dun_bytes_supported=4, so all DUNs should be 32-bit. */
	WARN_ON_ONCE(mrq->crypto_ctx->bc_dun[0] > U32_MAX);

	return CQHCI_CRYPTO_ENABLE_BIT |
	       CQHCI_CRYPTO_KEYSLOT(mrq->crypto_key_slot) |
	       mrq->crypto_ctx->bc_dun[0];
}

#else /* CONFIG_MMC_CRYPTO */

static inline int cqhci_crypto_init(struct cqhci_host *host)
>>>>>>> upstream/android-13
{
	return 0;
}

<<<<<<< HEAD
static inline int cqhci_prep_crypto_desc(struct mmc_request *mrq,
					 __le64 *task_desc)
=======
static inline u64 cqhci_crypto_prep_task_desc(struct mmc_request *mrq)
>>>>>>> upstream/android-13
{
	return 0;
}

<<<<<<< HEAD
static inline void cqhci_crypto_recovery_finish(struct cqhci_host *host) { }

#endif /* CONFIG_MMC_CRYPTO */
=======
#endif /* !CONFIG_MMC_CRYPTO */
>>>>>>> upstream/android-13

#endif /* LINUX_MMC_CQHCI_CRYPTO_H */
