/* SPDX-License-Identifier: GPL-2.0
 *
 * Copyright (C) 2014-2017 Linaro Ltd. <ard.biesheuvel@linaro.org>
 *
 * Copyright (C) 2018 Andes Technology Corporation <zong@andestech.com>
 */

#include <linux/elf.h>
#include <linux/kernel.h>
#include <linux/module.h>
<<<<<<< HEAD

u64 module_emit_got_entry(struct module *mod, u64 val)
=======
#include <linux/moduleloader.h>

unsigned long module_emit_got_entry(struct module *mod, unsigned long val)
>>>>>>> upstream/android-13
{
	struct mod_section *got_sec = &mod->arch.got;
	int i = got_sec->num_entries;
	struct got_entry *got = get_got_entry(val, got_sec);

	if (got)
<<<<<<< HEAD
		return (u64)got;
=======
		return (unsigned long)got;
>>>>>>> upstream/android-13

	/* There is no duplicate entry, create a new one */
	got = (struct got_entry *)got_sec->shdr->sh_addr;
	got[i] = emit_got_entry(val);

	got_sec->num_entries++;
	BUG_ON(got_sec->num_entries > got_sec->max_entries);

<<<<<<< HEAD
	return (u64)&got[i];
}

u64 module_emit_plt_entry(struct module *mod, u64 val)
=======
	return (unsigned long)&got[i];
}

unsigned long module_emit_plt_entry(struct module *mod, unsigned long val)
>>>>>>> upstream/android-13
{
	struct mod_section *got_plt_sec = &mod->arch.got_plt;
	struct got_entry *got_plt;
	struct mod_section *plt_sec = &mod->arch.plt;
	struct plt_entry *plt = get_plt_entry(val, plt_sec, got_plt_sec);
	int i = plt_sec->num_entries;

	if (plt)
<<<<<<< HEAD
		return (u64)plt;
=======
		return (unsigned long)plt;
>>>>>>> upstream/android-13

	/* There is no duplicate entry, create a new one */
	got_plt = (struct got_entry *)got_plt_sec->shdr->sh_addr;
	got_plt[i] = emit_got_entry(val);
	plt = (struct plt_entry *)plt_sec->shdr->sh_addr;
<<<<<<< HEAD
	plt[i] = emit_plt_entry(val, (u64)&plt[i], (u64)&got_plt[i]);
=======
	plt[i] = emit_plt_entry(val,
				(unsigned long)&plt[i],
				(unsigned long)&got_plt[i]);
>>>>>>> upstream/android-13

	plt_sec->num_entries++;
	got_plt_sec->num_entries++;
	BUG_ON(plt_sec->num_entries > plt_sec->max_entries);

<<<<<<< HEAD
	return (u64)&plt[i];
}

static int is_rela_equal(const Elf64_Rela *x, const Elf64_Rela *y)
=======
	return (unsigned long)&plt[i];
}

static int is_rela_equal(const Elf_Rela *x, const Elf_Rela *y)
>>>>>>> upstream/android-13
{
	return x->r_info == y->r_info && x->r_addend == y->r_addend;
}

<<<<<<< HEAD
static bool duplicate_rela(const Elf64_Rela *rela, int idx)
=======
static bool duplicate_rela(const Elf_Rela *rela, int idx)
>>>>>>> upstream/android-13
{
	int i;
	for (i = 0; i < idx; i++) {
		if (is_rela_equal(&rela[i], &rela[idx]))
			return true;
	}
	return false;
}

<<<<<<< HEAD
static void count_max_entries(Elf64_Rela *relas, int num,
=======
static void count_max_entries(Elf_Rela *relas, int num,
>>>>>>> upstream/android-13
			      unsigned int *plts, unsigned int *gots)
{
	unsigned int type, i;

	for (i = 0; i < num; i++) {
<<<<<<< HEAD
		type = ELF64_R_TYPE(relas[i].r_info);
=======
		type = ELF_RISCV_R_TYPE(relas[i].r_info);
>>>>>>> upstream/android-13
		if (type == R_RISCV_CALL_PLT) {
			if (!duplicate_rela(relas, i))
				(*plts)++;
		} else if (type == R_RISCV_GOT_HI20) {
			if (!duplicate_rela(relas, i))
				(*gots)++;
		}
	}
}

int module_frob_arch_sections(Elf_Ehdr *ehdr, Elf_Shdr *sechdrs,
			      char *secstrings, struct module *mod)
{
	unsigned int num_plts = 0;
	unsigned int num_gots = 0;
	int i;

	/*
	 * Find the empty .got and .plt sections.
	 */
	for (i = 0; i < ehdr->e_shnum; i++) {
		if (!strcmp(secstrings + sechdrs[i].sh_name, ".plt"))
			mod->arch.plt.shdr = sechdrs + i;
		else if (!strcmp(secstrings + sechdrs[i].sh_name, ".got"))
			mod->arch.got.shdr = sechdrs + i;
		else if (!strcmp(secstrings + sechdrs[i].sh_name, ".got.plt"))
			mod->arch.got_plt.shdr = sechdrs + i;
	}

	if (!mod->arch.plt.shdr) {
		pr_err("%s: module PLT section(s) missing\n", mod->name);
		return -ENOEXEC;
	}
	if (!mod->arch.got.shdr) {
		pr_err("%s: module GOT section(s) missing\n", mod->name);
		return -ENOEXEC;
	}
	if (!mod->arch.got_plt.shdr) {
		pr_err("%s: module GOT.PLT section(s) missing\n", mod->name);
		return -ENOEXEC;
	}

	/* Calculate the maxinum number of entries */
	for (i = 0; i < ehdr->e_shnum; i++) {
<<<<<<< HEAD
		Elf64_Rela *relas = (void *)ehdr + sechdrs[i].sh_offset;
		int num_rela = sechdrs[i].sh_size / sizeof(Elf64_Rela);
		Elf64_Shdr *dst_sec = sechdrs + sechdrs[i].sh_info;
=======
		Elf_Rela *relas = (void *)ehdr + sechdrs[i].sh_offset;
		int num_rela = sechdrs[i].sh_size / sizeof(Elf_Rela);
		Elf_Shdr *dst_sec = sechdrs + sechdrs[i].sh_info;
>>>>>>> upstream/android-13

		if (sechdrs[i].sh_type != SHT_RELA)
			continue;

		/* ignore relocations that operate on non-exec sections */
		if (!(dst_sec->sh_flags & SHF_EXECINSTR))
			continue;

		count_max_entries(relas, num_rela, &num_plts, &num_gots);
	}

	mod->arch.plt.shdr->sh_type = SHT_NOBITS;
	mod->arch.plt.shdr->sh_flags = SHF_EXECINSTR | SHF_ALLOC;
	mod->arch.plt.shdr->sh_addralign = L1_CACHE_BYTES;
	mod->arch.plt.shdr->sh_size = (num_plts + 1) * sizeof(struct plt_entry);
	mod->arch.plt.num_entries = 0;
	mod->arch.plt.max_entries = num_plts;

	mod->arch.got.shdr->sh_type = SHT_NOBITS;
	mod->arch.got.shdr->sh_flags = SHF_ALLOC;
	mod->arch.got.shdr->sh_addralign = L1_CACHE_BYTES;
	mod->arch.got.shdr->sh_size = (num_gots + 1) * sizeof(struct got_entry);
	mod->arch.got.num_entries = 0;
	mod->arch.got.max_entries = num_gots;

	mod->arch.got_plt.shdr->sh_type = SHT_NOBITS;
	mod->arch.got_plt.shdr->sh_flags = SHF_ALLOC;
	mod->arch.got_plt.shdr->sh_addralign = L1_CACHE_BYTES;
	mod->arch.got_plt.shdr->sh_size = (num_plts + 1) * sizeof(struct got_entry);
	mod->arch.got_plt.num_entries = 0;
	mod->arch.got_plt.max_entries = num_plts;
	return 0;
}
