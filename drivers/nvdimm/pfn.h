<<<<<<< HEAD
/*
 * Copyright (c) 2014-2015, Intel Corporation.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
=======
/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Copyright (c) 2014-2015, Intel Corporation.
>>>>>>> upstream/android-13
 */

#ifndef __NVDIMM_PFN_H
#define __NVDIMM_PFN_H

#include <linux/types.h>
#include <linux/mmzone.h>

#define PFN_SIG_LEN 16
#define PFN_SIG "NVDIMM_PFN_INFO\0"
#define DAX_SIG "NVDIMM_DAX_INFO\0"

struct nd_pfn_sb {
	u8 signature[PFN_SIG_LEN];
	u8 uuid[16];
	u8 parent_uuid[16];
	__le32 flags;
	__le16 version_major;
	__le16 version_minor;
	__le64 dataoff; /* relative to namespace_base + start_pad */
	__le64 npfns;
	__le32 mode;
	/* minor-version-1 additions for section alignment */
<<<<<<< HEAD
=======
	/**
	 * @start_pad: Deprecated attribute to pad start-misaligned namespaces
	 *
	 * start_pad is deprecated because the original definition did
	 * not comprehend that dataoff is relative to the base address
	 * of the namespace not the start_pad adjusted base. The result
	 * is that the dax path is broken, but the block-I/O path is
	 * not. The kernel will no longer create namespaces using start
	 * padding, but it still supports block-I/O for legacy
	 * configurations mainly to allow a backup, reconfigure the
	 * namespace, and restore flow to repair dax operation.
	 */
>>>>>>> upstream/android-13
	__le32 start_pad;
	__le32 end_trunc;
	/* minor-version-2 record the base alignment of the mapping */
	__le32 align;
	/* minor-version-3 guarantee the padding and flags are zero */
<<<<<<< HEAD
	u8 padding[4000];
	__le64 checksum;
};

#ifdef CONFIG_SPARSEMEM
#define PFN_SECTION_ALIGN_DOWN(x) SECTION_ALIGN_DOWN(x)
#define PFN_SECTION_ALIGN_UP(x) SECTION_ALIGN_UP(x)
#else
/*
 * In this case ZONE_DEVICE=n and we will disable 'pfn' device support,
 * but we still want pmem to compile.
 */
#define PFN_SECTION_ALIGN_DOWN(x) (x)
#define PFN_SECTION_ALIGN_UP(x) (x)
#endif

#define PHYS_SECTION_ALIGN_DOWN(x) PFN_PHYS(PFN_SECTION_ALIGN_DOWN(PHYS_PFN(x)))
#define PHYS_SECTION_ALIGN_UP(x) PFN_PHYS(PFN_SECTION_ALIGN_UP(PHYS_PFN(x)))
=======
	/* minor-version-4 record the page size and struct page size */
	__le32 page_size;
	__le16 page_struct_size;
	u8 padding[3994];
	__le64 checksum;
};

>>>>>>> upstream/android-13
#endif /* __NVDIMM_PFN_H */
