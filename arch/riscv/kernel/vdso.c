<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> upstream/android-13
/*
 * Copyright (C) 2004 Benjamin Herrenschmidt, IBM Corp.
 *                    <benh@kernel.crashing.org>
 * Copyright (C) 2012 ARM Limited
 * Copyright (C) 2015 Regents of the University of California
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

=======
 */

#include <linux/elf.h>
>>>>>>> upstream/android-13
#include <linux/mm.h>
#include <linux/slab.h>
#include <linux/binfmts.h>
#include <linux/err.h>
<<<<<<< HEAD

#include <asm/vdso.h>

extern char vdso_start[], vdso_end[];

static unsigned int vdso_pages;
static struct page **vdso_pagelist;
=======
#include <asm/page.h>
#include <asm/vdso.h>

#ifdef CONFIG_GENERIC_TIME_VSYSCALL
#include <vdso/datapage.h>
#else
struct vdso_data {
};
#endif

extern char vdso_start[], vdso_end[];

enum vvar_pages {
	VVAR_DATA_PAGE_OFFSET,
	VVAR_NR_PAGES,
};

#define VVAR_SIZE  (VVAR_NR_PAGES << PAGE_SHIFT)

static unsigned int vdso_pages __ro_after_init;
static struct page **vdso_pagelist __ro_after_init;
>>>>>>> upstream/android-13

/*
 * The vDSO data page.
 */
static union {
	struct vdso_data	data;
	u8			page[PAGE_SIZE];
} vdso_data_store __page_aligned_data;
struct vdso_data *vdso_data = &vdso_data_store.data;

static int __init vdso_init(void)
{
	unsigned int i;

	vdso_pages = (vdso_end - vdso_start) >> PAGE_SHIFT;
	vdso_pagelist =
<<<<<<< HEAD
		kcalloc(vdso_pages + 1, sizeof(struct page *), GFP_KERNEL);
=======
		kcalloc(vdso_pages + VVAR_NR_PAGES, sizeof(struct page *), GFP_KERNEL);
>>>>>>> upstream/android-13
	if (unlikely(vdso_pagelist == NULL)) {
		pr_err("vdso: pagelist allocation failed\n");
		return -ENOMEM;
	}

	for (i = 0; i < vdso_pages; i++) {
		struct page *pg;

		pg = virt_to_page(vdso_start + (i << PAGE_SHIFT));
<<<<<<< HEAD
		ClearPageReserved(pg);
=======
>>>>>>> upstream/android-13
		vdso_pagelist[i] = pg;
	}
	vdso_pagelist[i] = virt_to_page(vdso_data);

	return 0;
}
arch_initcall(vdso_init);

int arch_setup_additional_pages(struct linux_binprm *bprm,
	int uses_interp)
{
	struct mm_struct *mm = current->mm;
	unsigned long vdso_base, vdso_len;
	int ret;

<<<<<<< HEAD
	vdso_len = (vdso_pages + 1) << PAGE_SHIFT;

	down_write(&mm->mmap_sem);
=======
	BUILD_BUG_ON(VVAR_NR_PAGES != __VVAR_PAGES);

	vdso_len = (vdso_pages + VVAR_NR_PAGES) << PAGE_SHIFT;

	if (mmap_write_lock_killable(mm))
		return -EINTR;

>>>>>>> upstream/android-13
	vdso_base = get_unmapped_area(NULL, 0, vdso_len, 0, 0);
	if (IS_ERR_VALUE(vdso_base)) {
		ret = vdso_base;
		goto end;
	}

<<<<<<< HEAD
=======
	mm->context.vdso = NULL;
	ret = install_special_mapping(mm, vdso_base, VVAR_SIZE,
		(VM_READ | VM_MAYREAD), &vdso_pagelist[vdso_pages]);
	if (unlikely(ret))
		goto end;

	ret =
	   install_special_mapping(mm, vdso_base + VVAR_SIZE,
		vdso_pages << PAGE_SHIFT,
		(VM_READ | VM_EXEC | VM_MAYREAD | VM_MAYWRITE | VM_MAYEXEC),
		vdso_pagelist);

	if (unlikely(ret))
		goto end;

>>>>>>> upstream/android-13
	/*
	 * Put vDSO base into mm struct. We need to do this before calling
	 * install_special_mapping or the perf counter mmap tracking code
	 * will fail to recognise it as a vDSO (since arch_vma_name fails).
	 */
<<<<<<< HEAD
	mm->context.vdso = (void *)vdso_base;

	ret = install_special_mapping(mm, vdso_base, vdso_len,
		(VM_READ | VM_EXEC | VM_MAYREAD | VM_MAYWRITE | VM_MAYEXEC),
		vdso_pagelist);

	if (unlikely(ret))
		mm->context.vdso = NULL;

end:
	up_write(&mm->mmap_sem);
=======
	mm->context.vdso = (void *)vdso_base + VVAR_SIZE;

end:
	mmap_write_unlock(mm);
>>>>>>> upstream/android-13
	return ret;
}

const char *arch_vma_name(struct vm_area_struct *vma)
{
	if (vma->vm_mm && (vma->vm_start == (long)vma->vm_mm->context.vdso))
		return "[vdso]";
<<<<<<< HEAD
	return NULL;
}

/*
 * Function stubs to prevent linker errors when AT_SYSINFO_EHDR is defined
 */

int in_gate_area_no_mm(unsigned long addr)
{
	return 0;
}

int in_gate_area(struct mm_struct *mm, unsigned long addr)
{
	return 0;
}

struct vm_area_struct *get_gate_vma(struct mm_struct *mm)
{
=======
	if (vma->vm_mm && (vma->vm_start ==
			   (long)vma->vm_mm->context.vdso - VVAR_SIZE))
		return "[vdso_data]";
>>>>>>> upstream/android-13
	return NULL;
}
