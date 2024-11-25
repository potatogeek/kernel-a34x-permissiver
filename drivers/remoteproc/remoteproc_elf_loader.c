<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> upstream/android-13
/*
 * Remote Processor Framework Elf loader
 *
 * Copyright (C) 2011 Texas Instruments, Inc.
 * Copyright (C) 2011 Google, Inc.
 *
 * Ohad Ben-Cohen <ohad@wizery.com>
 * Brian Swetland <swetland@google.com>
 * Mark Grosen <mgrosen@ti.com>
 * Fernando Guzman Lugo <fernando.lugo@ti.com>
 * Suman Anna <s-anna@ti.com>
 * Robert Tivy <rtivy@ti.com>
 * Armando Uribe De Leon <x0095078@ti.com>
 * Sjur Brændeland <sjur.brandeland@stericsson.com>
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
=======
>>>>>>> upstream/android-13
 */

#define pr_fmt(fmt)    "%s: " fmt, __func__

#include <linux/module.h>
#include <linux/firmware.h>
#include <linux/remoteproc.h>
#include <linux/elf.h>

#include "remoteproc_internal.h"
<<<<<<< HEAD

/**
 * rproc_elf_sanity_check() - Sanity Check ELF firmware image
 * @rproc: the remote processor handle
 * @fw: the ELF firmware image
 *
 * Make sure this fw image is sane.
=======
#include "remoteproc_elf_helpers.h"

/**
 * rproc_elf_sanity_check() - Sanity Check for ELF32/ELF64 firmware image
 * @rproc: the remote processor handle
 * @fw: the ELF firmware image
 *
 * Make sure this fw image is sane (ie a correct ELF32/ELF64 file).
 *
 * Return: 0 on success and -EINVAL upon any failure
>>>>>>> upstream/android-13
 */
int rproc_elf_sanity_check(struct rproc *rproc, const struct firmware *fw)
{
	const char *name = rproc->firmware;
	struct device *dev = &rproc->dev;
<<<<<<< HEAD
	struct elf32_hdr *ehdr;
	char class;
=======
	/*
	 * Elf files are beginning with the same structure. Thus, to simplify
	 * header parsing, we can use the elf32_hdr one for both elf64 and
	 * elf32.
	 */
	struct elf32_hdr *ehdr;
	u32 elf_shdr_get_size;
	u64 phoff, shoff;
	char class;
	u16 phnum;
>>>>>>> upstream/android-13

	if (!fw) {
		dev_err(dev, "failed to load %s\n", name);
		return -EINVAL;
	}

	if (fw->size < sizeof(struct elf32_hdr)) {
		dev_err(dev, "Image is too small\n");
		return -EINVAL;
	}

	ehdr = (struct elf32_hdr *)fw->data;

<<<<<<< HEAD
	/* We only support ELF32 at this point */
	class = ehdr->e_ident[EI_CLASS];
	if (class != ELFCLASS32) {
=======
	if (memcmp(ehdr->e_ident, ELFMAG, SELFMAG)) {
		dev_err(dev, "Image is corrupted (bad magic)\n");
		return -EINVAL;
	}

	class = ehdr->e_ident[EI_CLASS];
	if (class != ELFCLASS32 && class != ELFCLASS64) {
>>>>>>> upstream/android-13
		dev_err(dev, "Unsupported class: %d\n", class);
		return -EINVAL;
	}

<<<<<<< HEAD
=======
	if (class == ELFCLASS64 && fw->size < sizeof(struct elf64_hdr)) {
		dev_err(dev, "elf64 header is too small\n");
		return -EINVAL;
	}

>>>>>>> upstream/android-13
	/* We assume the firmware has the same endianness as the host */
# ifdef __LITTLE_ENDIAN
	if (ehdr->e_ident[EI_DATA] != ELFDATA2LSB) {
# else /* BIG ENDIAN */
	if (ehdr->e_ident[EI_DATA] != ELFDATA2MSB) {
# endif
		dev_err(dev, "Unsupported firmware endianness\n");
		return -EINVAL;
	}

<<<<<<< HEAD
	if (fw->size < ehdr->e_shoff + sizeof(struct elf32_shdr)) {
=======
	phoff = elf_hdr_get_e_phoff(class, fw->data);
	shoff = elf_hdr_get_e_shoff(class, fw->data);
	phnum =  elf_hdr_get_e_phnum(class, fw->data);
	elf_shdr_get_size = elf_size_of_shdr(class);

	if (fw->size < shoff + elf_shdr_get_size) {
>>>>>>> upstream/android-13
		dev_err(dev, "Image is too small\n");
		return -EINVAL;
	}

<<<<<<< HEAD
	if (memcmp(ehdr->e_ident, ELFMAG, SELFMAG)) {
		dev_err(dev, "Image is corrupted (bad magic)\n");
		return -EINVAL;
	}

	if (ehdr->e_phnum == 0) {
=======
	if (phnum == 0) {
>>>>>>> upstream/android-13
		dev_err(dev, "No loadable segments\n");
		return -EINVAL;
	}

<<<<<<< HEAD
	if (ehdr->e_phoff > fw->size) {
=======
	if (phoff > fw->size) {
>>>>>>> upstream/android-13
		dev_err(dev, "Firmware size is too small\n");
		return -EINVAL;
	}

<<<<<<< HEAD
=======
	dev_dbg(dev, "Firmware is an elf%d file\n",
		class == ELFCLASS32 ? 32 : 64);

>>>>>>> upstream/android-13
	return 0;
}
EXPORT_SYMBOL(rproc_elf_sanity_check);

/**
 * rproc_elf_get_boot_addr() - Get rproc's boot address.
 * @rproc: the remote processor handle
 * @fw: the ELF firmware image
 *
<<<<<<< HEAD
 * This function returns the entry point address of the ELF
 * image.
 *
 * Note that the boot address is not a configurable property of all remote
 * processors. Some will always boot at a specific hard-coded address.
 */
u32 rproc_elf_get_boot_addr(struct rproc *rproc, const struct firmware *fw)
{
	struct elf32_hdr *ehdr  = (struct elf32_hdr *)fw->data;

	return ehdr->e_entry;
=======
 * Note that the boot address is not a configurable property of all remote
 * processors. Some will always boot at a specific hard-coded address.
 *
 * Return: entry point address of the ELF image
 *
 */
u64 rproc_elf_get_boot_addr(struct rproc *rproc, const struct firmware *fw)
{
	return elf_hdr_get_e_entry(fw_elf_get_class(fw), fw->data);
>>>>>>> upstream/android-13
}
EXPORT_SYMBOL(rproc_elf_get_boot_addr);

/**
 * rproc_elf_load_segments() - load firmware segments to memory
 * @rproc: remote processor which will be booted using these fw segments
 * @fw: the ELF firmware image
 *
 * This function loads the firmware segments to memory, where the remote
 * processor expects them.
 *
 * Some remote processors will expect their code and data to be placed
 * in specific device addresses, and can't have them dynamically assigned.
 *
 * We currently support only those kind of remote processors, and expect
 * the program header's paddr member to contain those addresses. We then go
 * through the physically contiguous "carveout" memory regions which we
 * allocated (and mapped) earlier on behalf of the remote processor,
 * and "translate" device address to kernel addresses, so we can copy the
 * segments where they are expected.
 *
 * Currently we only support remote processors that required carveout
 * allocations and got them mapped onto their iommus. Some processors
 * might be different: they might not have iommus, and would prefer to
 * directly allocate memory for every segment/resource. This is not yet
 * supported, though.
<<<<<<< HEAD
=======
 *
 * Return: 0 on success and an appropriate error code otherwise
>>>>>>> upstream/android-13
 */
int rproc_elf_load_segments(struct rproc *rproc, const struct firmware *fw)
{
	struct device *dev = &rproc->dev;
<<<<<<< HEAD
	struct elf32_hdr *ehdr;
	struct elf32_phdr *phdr;
	int i, ret = 0;
	const u8 *elf_data = fw->data;

	ehdr = (struct elf32_hdr *)elf_data;
	phdr = (struct elf32_phdr *)(elf_data + ehdr->e_phoff);

	/* go through the available ELF segments */
	for (i = 0; i < ehdr->e_phnum; i++, phdr++) {
		u32 da = phdr->p_paddr;
		u32 memsz = phdr->p_memsz;
		u32 filesz = phdr->p_filesz;
		u32 offset = phdr->p_offset;
		void *ptr;

		if (phdr->p_type != PT_LOAD)
			continue;

		dev_dbg(dev, "phdr: type %d da 0x%x memsz 0x%x filesz 0x%x\n",
			phdr->p_type, da, memsz, filesz);

		if (filesz > memsz) {
			dev_err(dev, "bad phdr filesz 0x%x memsz 0x%x\n",
=======
	const void *ehdr, *phdr;
	int i, ret = 0;
	u16 phnum;
	const u8 *elf_data = fw->data;
	u8 class = fw_elf_get_class(fw);
	u32 elf_phdr_get_size = elf_size_of_phdr(class);

	ehdr = elf_data;
	phnum = elf_hdr_get_e_phnum(class, ehdr);
	phdr = elf_data + elf_hdr_get_e_phoff(class, ehdr);

	/* go through the available ELF segments */
	for (i = 0; i < phnum; i++, phdr += elf_phdr_get_size) {
		u64 da = elf_phdr_get_p_paddr(class, phdr);
		u64 memsz = elf_phdr_get_p_memsz(class, phdr);
		u64 filesz = elf_phdr_get_p_filesz(class, phdr);
		u64 offset = elf_phdr_get_p_offset(class, phdr);
		u32 type = elf_phdr_get_p_type(class, phdr);
		bool is_iomem = false;
		void *ptr;

		if (type != PT_LOAD)
			continue;

		dev_dbg(dev, "phdr: type %d da 0x%llx memsz 0x%llx filesz 0x%llx\n",
			type, da, memsz, filesz);

		if (filesz > memsz) {
			dev_err(dev, "bad phdr filesz 0x%llx memsz 0x%llx\n",
>>>>>>> upstream/android-13
				filesz, memsz);
			ret = -EINVAL;
			break;
		}

		if (offset + filesz > fw->size) {
<<<<<<< HEAD
			dev_err(dev, "truncated fw: need 0x%x avail 0x%zx\n",
=======
			dev_err(dev, "truncated fw: need 0x%llx avail 0x%zx\n",
>>>>>>> upstream/android-13
				offset + filesz, fw->size);
			ret = -EINVAL;
			break;
		}

<<<<<<< HEAD
		/* grab the kernel address for this device address */
		ptr = rproc_da_to_va(rproc, da, memsz);
		if (!ptr) {
			dev_err(dev, "bad phdr da 0x%x mem 0x%x\n", da, memsz);
=======
		if (!rproc_u64_fit_in_size_t(memsz)) {
			dev_err(dev, "size (%llx) does not fit in size_t type\n",
				memsz);
			ret = -EOVERFLOW;
			break;
		}

		/* grab the kernel address for this device address */
		ptr = rproc_da_to_va(rproc, da, memsz, &is_iomem);
		if (!ptr) {
			dev_err(dev, "bad phdr da 0x%llx mem 0x%llx\n", da,
				memsz);
>>>>>>> upstream/android-13
			ret = -EINVAL;
			break;
		}

		/* put the segment where the remote processor expects it */
<<<<<<< HEAD
		if (phdr->p_filesz)
			memcpy(ptr, elf_data + phdr->p_offset, filesz);
=======
		if (filesz) {
			if (is_iomem)
				memcpy_toio((void __iomem *)ptr, elf_data + offset, filesz);
			else
				memcpy(ptr, elf_data + offset, filesz);
		}
>>>>>>> upstream/android-13

		/*
		 * Zero out remaining memory for this segment.
		 *
		 * This isn't strictly required since dma_alloc_coherent already
		 * did this for us. albeit harmless, we may consider removing
		 * this.
		 */
<<<<<<< HEAD
		if (memsz > filesz)
			memset(ptr + filesz, 0, memsz - filesz);
=======
		if (memsz > filesz) {
			if (is_iomem)
				memset_io((void __iomem *)(ptr + filesz), 0, memsz - filesz);
			else
				memset(ptr + filesz, 0, memsz - filesz);
		}
>>>>>>> upstream/android-13
	}

	return ret;
}
EXPORT_SYMBOL(rproc_elf_load_segments);

<<<<<<< HEAD
static struct elf32_shdr *
find_table(struct device *dev, struct elf32_hdr *ehdr, size_t fw_size)
{
	struct elf32_shdr *shdr;
	int i;
	const char *name_table;
	struct resource_table *table = NULL;
	const u8 *elf_data = (void *)ehdr;

	/* look for the resource table and handle it */
	shdr = (struct elf32_shdr *)(elf_data + ehdr->e_shoff);
	name_table = elf_data + shdr[ehdr->e_shstrndx].sh_offset;

	for (i = 0; i < ehdr->e_shnum; i++, shdr++) {
		u32 size = shdr->sh_size;
		u32 offset = shdr->sh_offset;

		if (strcmp(name_table + shdr->sh_name, ".resource_table"))
=======
static const void *
find_table(struct device *dev, const struct firmware *fw)
{
	const void *shdr, *name_table_shdr;
	int i;
	const char *name_table;
	struct resource_table *table = NULL;
	const u8 *elf_data = (void *)fw->data;
	u8 class = fw_elf_get_class(fw);
	size_t fw_size = fw->size;
	const void *ehdr = elf_data;
	u16 shnum = elf_hdr_get_e_shnum(class, ehdr);
	u32 elf_shdr_get_size = elf_size_of_shdr(class);
	u16 shstrndx = elf_hdr_get_e_shstrndx(class, ehdr);

	/* look for the resource table and handle it */
	/* First, get the section header according to the elf class */
	shdr = elf_data + elf_hdr_get_e_shoff(class, ehdr);
	/* Compute name table section header entry in shdr array */
	name_table_shdr = shdr + (shstrndx * elf_shdr_get_size);
	/* Finally, compute the name table section address in elf */
	name_table = elf_data + elf_shdr_get_sh_offset(class, name_table_shdr);

	for (i = 0; i < shnum; i++, shdr += elf_shdr_get_size) {
		u64 size = elf_shdr_get_sh_size(class, shdr);
		u64 offset = elf_shdr_get_sh_offset(class, shdr);
		u32 name = elf_shdr_get_sh_name(class, shdr);

		if (strcmp(name_table + name, ".resource_table"))
>>>>>>> upstream/android-13
			continue;

		table = (struct resource_table *)(elf_data + offset);

		/* make sure we have the entire table */
		if (offset + size > fw_size || offset + size < size) {
			dev_err(dev, "resource table truncated\n");
			return NULL;
		}

		/* make sure table has at least the header */
		if (sizeof(struct resource_table) > size) {
			dev_err(dev, "header-less resource table\n");
			return NULL;
		}

		/* we don't support any version beyond the first */
		if (table->ver != 1) {
			dev_err(dev, "unsupported fw ver: %d\n", table->ver);
			return NULL;
		}

		/* make sure reserved bytes are zeroes */
		if (table->reserved[0] || table->reserved[1]) {
			dev_err(dev, "non zero reserved bytes\n");
			return NULL;
		}

		/* make sure the offsets array isn't truncated */
<<<<<<< HEAD
		if (table->num * sizeof(table->offset[0]) +
				sizeof(struct resource_table) > size) {
=======
		if (struct_size(table, offset, table->num) > size) {
>>>>>>> upstream/android-13
			dev_err(dev, "resource table incomplete\n");
			return NULL;
		}

		return shdr;
	}

	return NULL;
}

/**
 * rproc_elf_load_rsc_table() - load the resource table
 * @rproc: the rproc handle
 * @fw: the ELF firmware image
 *
 * This function finds the resource table inside the remote processor's
 * firmware, load it into the @cached_table and update @table_ptr.
 *
 * Return: 0 on success, negative errno on failure.
 */
int rproc_elf_load_rsc_table(struct rproc *rproc, const struct firmware *fw)
{
<<<<<<< HEAD
	struct elf32_hdr *ehdr;
	struct elf32_shdr *shdr;
=======
	const void *shdr;
>>>>>>> upstream/android-13
	struct device *dev = &rproc->dev;
	struct resource_table *table = NULL;
	const u8 *elf_data = fw->data;
	size_t tablesz;
<<<<<<< HEAD

	ehdr = (struct elf32_hdr *)elf_data;

	shdr = find_table(dev, ehdr, fw->size);
	if (!shdr)
		return -EINVAL;

	table = (struct resource_table *)(elf_data + shdr->sh_offset);
	tablesz = shdr->sh_size;
=======
	u8 class = fw_elf_get_class(fw);
	u64 sh_offset;

	shdr = find_table(dev, fw);
	if (!shdr)
		return -EINVAL;

	sh_offset = elf_shdr_get_sh_offset(class, shdr);
	table = (struct resource_table *)(elf_data + sh_offset);
	tablesz = elf_shdr_get_sh_size(class, shdr);
>>>>>>> upstream/android-13

	/*
	 * Create a copy of the resource table. When a virtio device starts
	 * and calls vring_new_virtqueue() the address of the allocated vring
	 * will be stored in the cached_table. Before the device is started,
	 * cached_table will be copied into device memory.
	 */
	rproc->cached_table = kmemdup(table, tablesz, GFP_KERNEL);
	if (!rproc->cached_table)
		return -ENOMEM;

	rproc->table_ptr = rproc->cached_table;
	rproc->table_sz = tablesz;

	return 0;
}
EXPORT_SYMBOL(rproc_elf_load_rsc_table);

/**
 * rproc_elf_find_loaded_rsc_table() - find the loaded resource table
 * @rproc: the rproc handle
 * @fw: the ELF firmware image
 *
 * This function finds the location of the loaded resource table. Don't
 * call this function if the table wasn't loaded yet - it's a bug if you do.
 *
<<<<<<< HEAD
 * Returns the pointer to the resource table if it is found or NULL otherwise.
=======
 * Return: pointer to the resource table if it is found or NULL otherwise.
>>>>>>> upstream/android-13
 * If the table wasn't loaded yet the result is unspecified.
 */
struct resource_table *rproc_elf_find_loaded_rsc_table(struct rproc *rproc,
						       const struct firmware *fw)
{
<<<<<<< HEAD
	struct elf32_hdr *ehdr = (struct elf32_hdr *)fw->data;
	struct elf32_shdr *shdr;

	shdr = find_table(&rproc->dev, ehdr, fw->size);
	if (!shdr)
		return NULL;

	return rproc_da_to_va(rproc, shdr->sh_addr, shdr->sh_size);
=======
	const void *shdr;
	u64 sh_addr, sh_size;
	u8 class = fw_elf_get_class(fw);
	struct device *dev = &rproc->dev;

	shdr = find_table(&rproc->dev, fw);
	if (!shdr)
		return NULL;

	sh_addr = elf_shdr_get_sh_addr(class, shdr);
	sh_size = elf_shdr_get_sh_size(class, shdr);

	if (!rproc_u64_fit_in_size_t(sh_size)) {
		dev_err(dev, "size (%llx) does not fit in size_t type\n",
			sh_size);
		return NULL;
	}

	return rproc_da_to_va(rproc, sh_addr, sh_size, NULL);
>>>>>>> upstream/android-13
}
EXPORT_SYMBOL(rproc_elf_find_loaded_rsc_table);
