<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> upstream/android-13
/*
 * purgatory: Runs between two kernels
 *
 * Copyright (C) 2014 Red Hat Inc.
 *
 * Author:
 *       Vivek Goyal <vgoyal@redhat.com>
<<<<<<< HEAD
 *
 * This source code is licensed under the GNU General Public License,
 * Version 2.  See the file COPYING for more details.
 */

#include <linux/bug.h>
#include <linux/sha256.h>
=======
 */

#include <linux/bug.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <crypto/sha2.h>
>>>>>>> upstream/android-13
#include <asm/purgatory.h>

#include "../boot/string.h"

<<<<<<< HEAD
unsigned long purgatory_backup_dest __section(.kexec-purgatory);
unsigned long purgatory_backup_src __section(.kexec-purgatory);
unsigned long purgatory_backup_sz __section(.kexec-purgatory);

u8 purgatory_sha256_digest[SHA256_DIGEST_SIZE] __section(.kexec-purgatory);

struct kexec_sha_region purgatory_sha_regions[KEXEC_SEGMENT_MAX] __section(.kexec-purgatory);

/*
 * On x86, second kernel requries first 640K of memory to boot. Copy
 * first 640K to a backup region in reserved memory range so that second
 * kernel can use first 640K.
 */
static int copy_backup_region(void)
{
	if (purgatory_backup_dest) {
		memcpy((void *)purgatory_backup_dest,
		       (void *)purgatory_backup_src, purgatory_backup_sz);
	}
	return 0;
}
=======
u8 purgatory_sha256_digest[SHA256_DIGEST_SIZE] __section(".kexec-purgatory");

struct kexec_sha_region purgatory_sha_regions[KEXEC_SEGMENT_MAX] __section(".kexec-purgatory");
>>>>>>> upstream/android-13

static int verify_sha256_digest(void)
{
	struct kexec_sha_region *ptr, *end;
	u8 digest[SHA256_DIGEST_SIZE];
	struct sha256_state sctx;

	sha256_init(&sctx);
	end = purgatory_sha_regions + ARRAY_SIZE(purgatory_sha_regions);

	for (ptr = purgatory_sha_regions; ptr < end; ptr++)
		sha256_update(&sctx, (uint8_t *)(ptr->start), ptr->len);

	sha256_final(&sctx, digest);

	if (memcmp(digest, purgatory_sha256_digest, sizeof(digest)))
		return 1;

	return 0;
}

void purgatory(void)
{
	int ret;

	ret = verify_sha256_digest();
	if (ret) {
		/* loop forever */
		for (;;)
			;
	}
<<<<<<< HEAD
	copy_backup_region();
=======
>>>>>>> upstream/android-13
}

/*
 * Defined in order to reuse memcpy() and memset() from
 * arch/x86/boot/compressed/string.c
 */
void warn(const char *msg) {}
