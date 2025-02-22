<<<<<<< HEAD
/*
 * Copyright (c) 2013 Broadcom Corporation
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
 * SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION
 * OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
=======
// SPDX-License-Identifier: ISC
/*
 * Copyright (c) 2013 Broadcom Corporation
>>>>>>> upstream/android-13
 */
#ifndef BRCMFMAC_FIRMWARE_H
#define BRCMFMAC_FIRMWARE_H

#define BRCMF_FW_REQF_OPTIONAL		0x0001

#define	BRCMF_FW_NAME_LEN		320

#define BRCMF_FW_DEFAULT_PATH		"brcm/"

/**
 * struct brcmf_firmware_mapping - Used to map chipid/revmask to firmware
 *	filename and nvram filename. Each bus type implementation should create
 *	a table of firmware mappings (using the macros defined below).
 *
 * @chipid: ID of chip.
 * @revmask: bitmask of revisions, e.g. 0x10 means rev 4 only, 0xf means rev 0-3
 * @fw: name of the firmware file.
 * @nvram: name of nvram file.
 */
struct brcmf_firmware_mapping {
	u32 chipid;
	u32 revmask;
	const char *fw_base;
};

#define BRCMF_FW_DEF(fw_name, fw_base) \
static const char BRCM_ ## fw_name ## _FIRMWARE_BASENAME[] = \
	BRCMF_FW_DEFAULT_PATH fw_base; \
MODULE_FIRMWARE(BRCMF_FW_DEFAULT_PATH fw_base ".bin")

<<<<<<< HEAD
=======
/* Firmware and Country Local Matrix files */
#define BRCMF_FW_CLM_DEF(fw_name, fw_base) \
static const char BRCM_ ## fw_name ## _FIRMWARE_BASENAME[] = \
	BRCMF_FW_DEFAULT_PATH fw_base; \
MODULE_FIRMWARE(BRCMF_FW_DEFAULT_PATH fw_base ".bin"); \
MODULE_FIRMWARE(BRCMF_FW_DEFAULT_PATH fw_base ".clm_blob")

>>>>>>> upstream/android-13
#define BRCMF_FW_ENTRY(chipid, mask, name) \
	{ chipid, mask, BRCM_ ## name ## _FIRMWARE_BASENAME }

void brcmf_fw_nvram_free(void *nvram);

enum brcmf_fw_type {
	BRCMF_FW_TYPE_BINARY,
	BRCMF_FW_TYPE_NVRAM
};

struct brcmf_fw_item {
	const char *path;
	enum brcmf_fw_type type;
	u16 flags;
	union {
		const struct firmware *binary;
		struct {
			void *data;
			u32 len;
		} nv_data;
	};
};

struct brcmf_fw_request {
	u16 domain_nr;
	u16 bus_nr;
	u32 n_items;
<<<<<<< HEAD
	struct brcmf_fw_item items[0];
=======
	const char *board_type;
	struct brcmf_fw_item items[];
>>>>>>> upstream/android-13
};

struct brcmf_fw_name {
	const char *extension;
	char *path;
};

struct brcmf_fw_request *
brcmf_fw_alloc_request(u32 chip, u32 chiprev,
		       const struct brcmf_firmware_mapping mapping_table[],
		       u32 table_size, struct brcmf_fw_name *fwnames,
		       u32 n_fwnames);

/*
 * Request firmware(s) asynchronously. When the asynchronous request
 * fails it will not use the callback, but call device_release_driver()
 * instead which will call the driver .remove() callback.
 */
int brcmf_fw_get_firmwares(struct device *dev, struct brcmf_fw_request *req,
			   void (*fw_cb)(struct device *dev, int err,
					 struct brcmf_fw_request *req));

#endif /* BRCMFMAC_FIRMWARE_H */
