/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _ASM_X86_EFI_H
#define _ASM_X86_EFI_H

#include <asm/fpu/api.h>
<<<<<<< HEAD
#include <asm/pgtable.h>
=======
>>>>>>> upstream/android-13
#include <asm/processor-flags.h>
#include <asm/tlb.h>
#include <asm/nospec-branch.h>
#include <asm/mmu_context.h>
<<<<<<< HEAD
=======
#include <linux/build_bug.h>
#include <linux/kernel.h>
#include <linux/pgtable.h>

extern unsigned long efi_fw_vendor, efi_config_table;
extern unsigned long efi_mixed_mode_stack_pa;
>>>>>>> upstream/android-13

/*
 * We map the EFI regions needed for runtime services non-contiguously,
 * with preserved alignment on virtual addresses starting from -4G down
 * for a total max space of 64G. This way, we provide for stable runtime
 * services addresses across kernels so that a kexec'd kernel can still
 * use them.
 *
 * This is the main reason why we're doing stable VA mappings for RT
 * services.
<<<<<<< HEAD
 *
 * This flag is used in conjuction with a chicken bit called
 * "efi=old_map" which can be used as a fallback to the old runtime
 * services mapping method in case there's some b0rkage with a
 * particular EFI implementation (haha, it is hard to hold up the
 * sarcasm here...).
 */
#define EFI_OLD_MEMMAP		EFI_ARCH_1
=======
 */
>>>>>>> upstream/android-13

#define EFI32_LOADER_SIGNATURE	"EL32"
#define EFI64_LOADER_SIGNATURE	"EL64"

<<<<<<< HEAD
#define MAX_CMDLINE_ADDRESS	UINT_MAX

#define ARCH_EFI_IRQ_FLAGS_MASK	X86_EFLAGS_IF

#ifdef CONFIG_X86_32

extern asmlinkage unsigned long efi_call_phys(void *, ...);

#define arch_efi_call_virt_setup()					\
({									\
	kernel_fpu_begin();						\
=======
#define ARCH_EFI_IRQ_FLAGS_MASK	X86_EFLAGS_IF

/*
 * The EFI services are called through variadic functions in many cases. These
 * functions are implemented in assembler and support only a fixed number of
 * arguments. The macros below allows us to check at build time that we don't
 * try to call them with too many arguments.
 *
 * __efi_nargs() will return the number of arguments if it is 7 or less, and
 * cause a BUILD_BUG otherwise. The limitations of the C preprocessor make it
 * impossible to calculate the exact number of arguments beyond some
 * pre-defined limit. The maximum number of arguments currently supported by
 * any of the thunks is 7, so this is good enough for now and can be extended
 * in the obvious way if we ever need more.
 */

#define __efi_nargs(...) __efi_nargs_(__VA_ARGS__)
#define __efi_nargs_(...) __efi_nargs__(0, ##__VA_ARGS__,	\
	__efi_arg_sentinel(7), __efi_arg_sentinel(6),		\
	__efi_arg_sentinel(5), __efi_arg_sentinel(4),		\
	__efi_arg_sentinel(3), __efi_arg_sentinel(2),		\
	__efi_arg_sentinel(1), __efi_arg_sentinel(0))
#define __efi_nargs__(_0, _1, _2, _3, _4, _5, _6, _7, n, ...)	\
	__take_second_arg(n,					\
		({ BUILD_BUG_ON_MSG(1, "__efi_nargs limit exceeded"); 8; }))
#define __efi_arg_sentinel(n) , n

/*
 * __efi_nargs_check(f, n, ...) will cause a BUILD_BUG if the ellipsis
 * represents more than n arguments.
 */

#define __efi_nargs_check(f, n, ...)					\
	__efi_nargs_check_(f, __efi_nargs(__VA_ARGS__), n)
#define __efi_nargs_check_(f, p, n) __efi_nargs_check__(f, p, n)
#define __efi_nargs_check__(f, p, n) ({					\
	BUILD_BUG_ON_MSG(						\
		(p) > (n),						\
		#f " called with too many arguments (" #p ">" #n ")");	\
})

static inline void efi_fpu_begin(void)
{
	/*
	 * The UEFI calling convention (UEFI spec 2.3.2 and 2.3.4) requires
	 * that FCW and MXCSR (64-bit) must be initialized prior to calling
	 * UEFI code.  (Oddly the spec does not require that the FPU stack
	 * be empty.)
	 */
	kernel_fpu_begin_mask(KFPU_387 | KFPU_MXCSR);
}

static inline void efi_fpu_end(void)
{
	kernel_fpu_end();
}

#ifdef CONFIG_X86_32
#define arch_efi_call_virt_setup()					\
({									\
	efi_fpu_begin();						\
>>>>>>> upstream/android-13
	firmware_restrict_branch_speculation_start();			\
})

#define arch_efi_call_virt_teardown()					\
({									\
	firmware_restrict_branch_speculation_end();			\
<<<<<<< HEAD
	kernel_fpu_end();						\
})


/*
 * Wrap all the virtual calls in a way that forces the parameters on the stack.
 */
#define arch_efi_call_virt(p, f, args...)				\
({									\
	((efi_##f##_t __attribute__((regparm(0)))*) p->f)(args);	\
})

#define efi_ioremap(addr, size, type, attr)	ioremap_cache(addr, size)
=======
	efi_fpu_end();							\
})

#define arch_efi_call_virt(p, f, args...)	p->f(args)
>>>>>>> upstream/android-13

#else /* !CONFIG_X86_32 */

#define EFI_LOADER_SIGNATURE	"EL64"

<<<<<<< HEAD
extern asmlinkage u64 efi_call(void *fp, ...);

#define efi_call_phys(f, args...)		efi_call((f), args)

/*
 * struct efi_scratch - Scratch space used while switching to/from efi_mm
 * @phys_stack: stack used during EFI Mixed Mode
 * @prev_mm:    store/restore stolen mm_struct while switching to/from efi_mm
 */
struct efi_scratch {
	u64			phys_stack;
	struct mm_struct	*prev_mm;
} __packed;
=======
extern asmlinkage u64 __efi_call(void *fp, ...);

#define efi_call(...) ({						\
	__efi_nargs_check(efi_call, 7, __VA_ARGS__);			\
	__efi_call(__VA_ARGS__);					\
})
>>>>>>> upstream/android-13

#define arch_efi_call_virt_setup()					\
({									\
	efi_sync_low_kernel_mappings();					\
<<<<<<< HEAD
	kernel_fpu_begin();						\
	firmware_restrict_branch_speculation_start();			\
									\
	if (!efi_enabled(EFI_OLD_MEMMAP))				\
		efi_switch_mm(&efi_mm);					\
=======
	efi_fpu_begin();						\
	firmware_restrict_branch_speculation_start();			\
	efi_enter_mm();							\
>>>>>>> upstream/android-13
})

#define arch_efi_call_virt(p, f, args...)				\
	efi_call((void *)p->f, args)					\

#define arch_efi_call_virt_teardown()					\
({									\
<<<<<<< HEAD
	if (!efi_enabled(EFI_OLD_MEMMAP))				\
		efi_switch_mm(efi_scratch.prev_mm);			\
									\
	firmware_restrict_branch_speculation_end();			\
	kernel_fpu_end();						\
})

extern void __iomem *__init efi_ioremap(unsigned long addr, unsigned long size,
					u32 type, u64 attribute);

=======
	efi_leave_mm();							\
	firmware_restrict_branch_speculation_end();			\
	efi_fpu_end();							\
})

>>>>>>> upstream/android-13
#ifdef CONFIG_KASAN
/*
 * CONFIG_KASAN may redefine memset to __memset.  __memset function is present
 * only in kernel binary.  Since the EFI stub linked into a separate binary it
 * doesn't have __memset().  So we should use standard memset from
 * arch/x86/boot/compressed/string.c.  The same applies to memcpy and memmove.
 */
#undef memcpy
#undef memset
#undef memmove
#endif

#endif /* CONFIG_X86_32 */

<<<<<<< HEAD
extern struct efi_scratch efi_scratch;
extern void __init efi_set_executable(efi_memory_desc_t *md, bool executable);
extern int __init efi_memblock_x86_reserve_range(void);
extern pgd_t * __init efi_call_phys_prolog(void);
extern void __init efi_call_phys_epilog(pgd_t *save_pgd);
extern void __init efi_print_memmap(void);
extern void __init efi_memory_uc(u64 addr, unsigned long size);
=======
extern int __init efi_memblock_x86_reserve_range(void);
extern void __init efi_print_memmap(void);
>>>>>>> upstream/android-13
extern void __init efi_map_region(efi_memory_desc_t *md);
extern void __init efi_map_region_fixed(efi_memory_desc_t *md);
extern void efi_sync_low_kernel_mappings(void);
extern int __init efi_alloc_page_tables(void);
extern int __init efi_setup_page_tables(unsigned long pa_memmap, unsigned num_pages);
<<<<<<< HEAD
extern void __init old_map_region(efi_memory_desc_t *md);
extern void __init runtime_code_page_mkexec(void);
=======
>>>>>>> upstream/android-13
extern void __init efi_runtime_update_mappings(void);
extern void __init efi_dump_pagetable(void);
extern void __init efi_apply_memmap_quirks(void);
extern int __init efi_reuse_config(u64 tables, int nr_tables);
extern void efi_delete_dummy_variable(void);
<<<<<<< HEAD
extern void efi_switch_mm(struct mm_struct *mm);

struct efi_setup_data {
	u64 fw_vendor;
	u64 runtime;
=======
extern void efi_crash_gracefully_on_page_fault(unsigned long phys_addr);
extern void efi_free_boot_services(void);

void efi_enter_mm(void);
void efi_leave_mm(void);

/* kexec external ABI */
struct efi_setup_data {
	u64 fw_vendor;
	u64 __unused;
>>>>>>> upstream/android-13
	u64 tables;
	u64 smbios;
	u64 reserved[8];
};

extern u64 efi_setup;

#ifdef CONFIG_EFI
<<<<<<< HEAD

static inline bool efi_is_native(void)
{
	return IS_ENABLED(CONFIG_X86_64) == efi_enabled(EFI_64BIT);
=======
extern efi_status_t __efi64_thunk(u32, ...);

#define efi64_thunk(...) ({						\
	__efi_nargs_check(efi64_thunk, 6, __VA_ARGS__);			\
	__efi64_thunk(__VA_ARGS__);					\
})

static inline bool efi_is_mixed(void)
{
	if (!IS_ENABLED(CONFIG_EFI_MIXED))
		return false;
	return IS_ENABLED(CONFIG_X86_64) && !efi_enabled(EFI_64BIT);
>>>>>>> upstream/android-13
}

static inline bool efi_runtime_supported(void)
{
<<<<<<< HEAD
	if (efi_is_native())
		return true;

	if (IS_ENABLED(CONFIG_EFI_MIXED) && !efi_enabled(EFI_OLD_MEMMAP))
		return true;

	return false;
}

extern struct console early_efi_console;
extern void parse_efi_setup(u64 phys_addr, u32 data_len);

extern void efifb_setup_from_dmi(struct screen_info *si, const char *opt);

#ifdef CONFIG_EFI_MIXED
extern void efi_thunk_runtime_setup(void);
extern efi_status_t efi_thunk_set_virtual_address_map(
	void *phys_set_virtual_address_map,
	unsigned long memory_map_size,
	unsigned long descriptor_size,
	u32 descriptor_version,
	efi_memory_desc_t *virtual_map);
#else
static inline void efi_thunk_runtime_setup(void) {}
static inline efi_status_t efi_thunk_set_virtual_address_map(
	void *phys_set_virtual_address_map,
	unsigned long memory_map_size,
	unsigned long descriptor_size,
	u32 descriptor_version,
	efi_memory_desc_t *virtual_map)
{
	return EFI_SUCCESS;
}
#endif /* CONFIG_EFI_MIXED */


/* arch specific definitions used by the stub code */

struct efi_config {
	u64 image_handle;
	u64 table;
	u64 runtime_services;
	u64 boot_services;
	u64 text_output;
	efi_status_t (*call)(unsigned long, ...);
	bool is64;
} __packed;

__pure const struct efi_config *__efi_early(void);

static inline bool efi_is_64bit(void)
{
	if (!IS_ENABLED(CONFIG_X86_64))
		return false;

	if (!IS_ENABLED(CONFIG_EFI_MIXED))
		return true;

	return __efi_early()->is64;
}

#define efi_table_attr(table, attr, instance)				\
	(efi_is_64bit() ?						\
		((table##_64_t *)(unsigned long)instance)->attr :	\
		((table##_32_t *)(unsigned long)instance)->attr)

#define efi_call_proto(protocol, f, instance, ...)			\
	__efi_early()->call(efi_table_attr(protocol, f, instance),	\
		instance, ##__VA_ARGS__)

#define efi_call_early(f, ...)						\
	__efi_early()->call(efi_table_attr(efi_boot_services, f,	\
		__efi_early()->boot_services), __VA_ARGS__)

#define __efi_call_early(f, ...)					\
	__efi_early()->call((unsigned long)f, __VA_ARGS__);

#define efi_call_runtime(f, ...)					\
	__efi_early()->call(efi_table_attr(efi_runtime_services, f,	\
		__efi_early()->runtime_services), __VA_ARGS__)

extern bool efi_reboot_required(void);

=======
	if (IS_ENABLED(CONFIG_X86_64) == efi_enabled(EFI_64BIT))
		return true;

	return IS_ENABLED(CONFIG_EFI_MIXED);
}

extern void parse_efi_setup(u64 phys_addr, u32 data_len);

extern void efi_thunk_runtime_setup(void);
efi_status_t efi_set_virtual_address_map(unsigned long memory_map_size,
					 unsigned long descriptor_size,
					 u32 descriptor_version,
					 efi_memory_desc_t *virtual_map,
					 unsigned long systab_phys);

/* arch specific definitions used by the stub code */

#ifdef CONFIG_EFI_MIXED

#define ARCH_HAS_EFISTUB_WRAPPERS

static inline bool efi_is_64bit(void)
{
	extern const bool efi_is64;

	return efi_is64;
}

static inline bool efi_is_native(void)
{
	return efi_is_64bit();
}

#define efi_mixed_mode_cast(attr)					\
	__builtin_choose_expr(						\
		__builtin_types_compatible_p(u32, __typeof__(attr)),	\
			(unsigned long)(attr), (attr))

#define efi_table_attr(inst, attr)					\
	(efi_is_native()						\
		? inst->attr						\
		: (__typeof__(inst->attr))				\
			efi_mixed_mode_cast(inst->mixed_mode.attr))

/*
 * The following macros allow translating arguments if necessary from native to
 * mixed mode. The use case for this is to initialize the upper 32 bits of
 * output parameters, and where the 32-bit method requires a 64-bit argument,
 * which must be split up into two arguments to be thunked properly.
 *
 * As examples, the AllocatePool boot service returns the address of the
 * allocation, but it will not set the high 32 bits of the address. To ensure
 * that the full 64-bit address is initialized, we zero-init the address before
 * calling the thunk.
 *
 * The FreePages boot service takes a 64-bit physical address even in 32-bit
 * mode. For the thunk to work correctly, a native 64-bit call of
 * 	free_pages(addr, size)
 * must be translated to
 * 	efi64_thunk(free_pages, addr & U32_MAX, addr >> 32, size)
 * so that the two 32-bit halves of addr get pushed onto the stack separately.
 */

static inline void *efi64_zero_upper(void *p)
{
	((u32 *)p)[1] = 0;
	return p;
}

static inline u32 efi64_convert_status(efi_status_t status)
{
	return (u32)(status | (u64)status >> 32);
}

#define __efi64_argmap_free_pages(addr, size)				\
	((addr), 0, (size))

#define __efi64_argmap_get_memory_map(mm_size, mm, key, size, ver)	\
	((mm_size), (mm), efi64_zero_upper(key), efi64_zero_upper(size), (ver))

#define __efi64_argmap_allocate_pool(type, size, buffer)		\
	((type), (size), efi64_zero_upper(buffer))

#define __efi64_argmap_create_event(type, tpl, f, c, event)		\
	((type), (tpl), (f), (c), efi64_zero_upper(event))

#define __efi64_argmap_set_timer(event, type, time)			\
	((event), (type), lower_32_bits(time), upper_32_bits(time))

#define __efi64_argmap_wait_for_event(num, event, index)		\
	((num), (event), efi64_zero_upper(index))

#define __efi64_argmap_handle_protocol(handle, protocol, interface)	\
	((handle), (protocol), efi64_zero_upper(interface))

#define __efi64_argmap_locate_protocol(protocol, reg, interface)	\
	((protocol), (reg), efi64_zero_upper(interface))

#define __efi64_argmap_locate_device_path(protocol, path, handle)	\
	((protocol), (path), efi64_zero_upper(handle))

#define __efi64_argmap_exit(handle, status, size, data)			\
	((handle), efi64_convert_status(status), (size), (data))

/* PCI I/O */
#define __efi64_argmap_get_location(protocol, seg, bus, dev, func)	\
	((protocol), efi64_zero_upper(seg), efi64_zero_upper(bus),	\
	 efi64_zero_upper(dev), efi64_zero_upper(func))

/* LoadFile */
#define __efi64_argmap_load_file(protocol, path, policy, bufsize, buf)	\
	((protocol), (path), (policy), efi64_zero_upper(bufsize), (buf))

/* Graphics Output Protocol */
#define __efi64_argmap_query_mode(gop, mode, size, info)		\
	((gop), (mode), efi64_zero_upper(size), efi64_zero_upper(info))

/*
 * The macros below handle the plumbing for the argument mapping. To add a
 * mapping for a specific EFI method, simply define a macro
 * __efi64_argmap_<method name>, following the examples above.
 */

#define __efi64_thunk_map(inst, func, ...)				\
	efi64_thunk(inst->mixed_mode.func,				\
		__efi64_argmap(__efi64_argmap_ ## func(__VA_ARGS__),	\
			       (__VA_ARGS__)))

#define __efi64_argmap(mapped, args)					\
	__PASTE(__efi64_argmap__, __efi_nargs(__efi_eat mapped))(mapped, args)
#define __efi64_argmap__0(mapped, args) __efi_eval mapped
#define __efi64_argmap__1(mapped, args) __efi_eval args

#define __efi_eat(...)
#define __efi_eval(...) __VA_ARGS__

/* The three macros below handle dispatching via the thunk if needed */

#define efi_call_proto(inst, func, ...)					\
	(efi_is_native()						\
		? inst->func(inst, ##__VA_ARGS__)			\
		: __efi64_thunk_map(inst, func, inst, ##__VA_ARGS__))

#define efi_bs_call(func, ...)						\
	(efi_is_native()						\
		? efi_system_table->boottime->func(__VA_ARGS__)		\
		: __efi64_thunk_map(efi_table_attr(efi_system_table,	\
						   boottime),		\
				    func, __VA_ARGS__))

#define efi_rt_call(func, ...)						\
	(efi_is_native()						\
		? efi_system_table->runtime->func(__VA_ARGS__)		\
		: __efi64_thunk_map(efi_table_attr(efi_system_table,	\
						   runtime),		\
				    func, __VA_ARGS__))

#else /* CONFIG_EFI_MIXED */

static inline bool efi_is_64bit(void)
{
	return IS_ENABLED(CONFIG_X86_64);
}

#endif /* CONFIG_EFI_MIXED */

extern bool efi_reboot_required(void);
extern bool efi_is_table_address(unsigned long phys_addr);

extern void efi_find_mirror(void);
extern void efi_reserve_boot_services(void);
>>>>>>> upstream/android-13
#else
static inline void parse_efi_setup(u64 phys_addr, u32 data_len) {}
static inline bool efi_reboot_required(void)
{
	return false;
}
<<<<<<< HEAD
#endif /* CONFIG_EFI */

=======
static inline  bool efi_is_table_address(unsigned long phys_addr)
{
	return false;
}
static inline void efi_find_mirror(void)
{
}
static inline void efi_reserve_boot_services(void)
{
}
#endif /* CONFIG_EFI */

#ifdef CONFIG_EFI_FAKE_MEMMAP
extern void __init efi_fake_memmap_early(void);
#else
static inline void efi_fake_memmap_early(void)
{
}
#endif

#define arch_ima_efi_boot_mode	\
	({ extern struct boot_params boot_params; boot_params.secure_boot; })

>>>>>>> upstream/android-13
#endif /* _ASM_X86_EFI_H */
