// SPDX-License-Identifier: GPL-2.0
/*
<<<<<<< HEAD
 * This file contains common generic and tag-based KASAN error reporting code.
=======
 * This file contains common KASAN error reporting code.
>>>>>>> upstream/android-13
 *
 * Copyright (c) 2014 Samsung Electronics Co., Ltd.
 * Author: Andrey Ryabinin <ryabinin.a.a@gmail.com>
 *
 * Some code borrowed from https://github.com/xairy/kasan-prototype by
 *        Andrey Konovalov <andreyknvl@gmail.com>
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
=======
>>>>>>> upstream/android-13
 */

#include <linux/bitops.h>
#include <linux/ftrace.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/mm.h>
#include <linux/printk.h>
#include <linux/sched.h>
#include <linux/slab.h>
#include <linux/stackdepot.h>
#include <linux/stacktrace.h>
#include <linux/string.h>
#include <linux/types.h>
#include <linux/kasan.h>
#include <linux/module.h>
<<<<<<< HEAD

#include <asm/sections.h>

#include "kasan.h"
#include "../slab.h"

/* Shadow layout customization. */
#define SHADOW_BYTES_PER_BLOCK 1
#define SHADOW_BLOCKS_PER_ROW 16
#define SHADOW_BYTES_PER_ROW (SHADOW_BLOCKS_PER_ROW * SHADOW_BYTES_PER_BLOCK)
#define SHADOW_ROWS_AROUND_ADDR 2

=======
#include <linux/sched/task_stack.h>
#include <linux/uaccess.h>
#include <trace/events/error_report.h>

#include <asm/sections.h>

#include <kunit/test.h>

#include "kasan.h"
#include "../slab.h"

>>>>>>> upstream/android-13
static unsigned long kasan_flags;

#define KASAN_BIT_REPORTED	0
#define KASAN_BIT_MULTI_SHOT	1

<<<<<<< HEAD
=======
enum kasan_arg_fault {
	KASAN_ARG_FAULT_DEFAULT,
	KASAN_ARG_FAULT_REPORT,
	KASAN_ARG_FAULT_PANIC,
};

static enum kasan_arg_fault kasan_arg_fault __ro_after_init = KASAN_ARG_FAULT_DEFAULT;

/* kasan.fault=report/panic */
static int __init early_kasan_fault(char *arg)
{
	if (!arg)
		return -EINVAL;

	if (!strcmp(arg, "report"))
		kasan_arg_fault = KASAN_ARG_FAULT_REPORT;
	else if (!strcmp(arg, "panic"))
		kasan_arg_fault = KASAN_ARG_FAULT_PANIC;
	else
		return -EINVAL;

	return 0;
}
early_param("kasan.fault", early_kasan_fault);

>>>>>>> upstream/android-13
bool kasan_save_enable_multi_shot(void)
{
	return test_and_set_bit(KASAN_BIT_MULTI_SHOT, &kasan_flags);
}
EXPORT_SYMBOL_GPL(kasan_save_enable_multi_shot);

void kasan_restore_multi_shot(bool enabled)
{
	if (!enabled)
		clear_bit(KASAN_BIT_MULTI_SHOT, &kasan_flags);
}
EXPORT_SYMBOL_GPL(kasan_restore_multi_shot);

static int __init kasan_set_multi_shot(char *str)
{
	set_bit(KASAN_BIT_MULTI_SHOT, &kasan_flags);
	return 1;
}
__setup("kasan_multi_shot", kasan_set_multi_shot);

static void print_error_description(struct kasan_access_info *info)
{
	pr_err("BUG: KASAN: %s in %pS\n",
<<<<<<< HEAD
		get_bug_type(info), (void *)info->ip);
	pr_err("%s of size %zu at addr %px by task %s/%d\n",
		info->is_write ? "Write" : "Read", info->access_size,
		info->access_addr, current->comm, task_pid_nr(current));
=======
		kasan_get_bug_type(info), (void *)info->ip);
	if (info->access_size)
		pr_err("%s of size %zu at addr %px by task %s/%d\n",
			info->is_write ? "Write" : "Read", info->access_size,
			info->access_addr, current->comm, task_pid_nr(current));
	else
		pr_err("%s at addr %px by task %s/%d\n",
			info->is_write ? "Write" : "Read",
			info->access_addr, current->comm, task_pid_nr(current));
>>>>>>> upstream/android-13
}

static DEFINE_SPINLOCK(report_lock);

static void start_report(unsigned long *flags)
{
	/*
	 * Make sure we don't end up in loop.
	 */
	kasan_disable_current();
	spin_lock_irqsave(&report_lock, *flags);
	pr_err("==================================================================\n");
}

<<<<<<< HEAD
static void end_report(unsigned long *flags)
{
	pr_err("==================================================================\n");
	add_taint(TAINT_BAD_PAGE, LOCKDEP_NOW_UNRELIABLE);
	spin_unlock_irqrestore(&report_lock, *flags);
	if (panic_on_warn)
		panic("panic_on_warn set ...\n");
	kasan_enable_current();
}

=======
static void end_report(unsigned long *flags, unsigned long addr)
{
	if (!kasan_async_fault_possible())
		trace_error_report_end(ERROR_DETECTOR_KASAN, addr);
	pr_err("==================================================================\n");
	add_taint(TAINT_BAD_PAGE, LOCKDEP_NOW_UNRELIABLE);
	spin_unlock_irqrestore(&report_lock, *flags);
	if (panic_on_warn && !test_bit(KASAN_BIT_MULTI_SHOT, &kasan_flags)) {
		/*
		 * This thread may hit another WARN() in the panic path.
		 * Resetting this prevents additional WARN() from panicking the
		 * system on this thread.  Other threads are blocked by the
		 * panic_mutex in panic().
		 */
		panic_on_warn = 0;
		panic("panic_on_warn set ...\n");
	}
	if (kasan_arg_fault == KASAN_ARG_FAULT_PANIC)
		panic("kasan.fault=panic set ...\n");
	kasan_enable_current();
}

static void print_stack(depot_stack_handle_t stack)
{
	unsigned long *entries;
	unsigned int nr_entries;

	nr_entries = stack_depot_fetch(stack, &entries);
	stack_trace_print(entries, nr_entries, 0);
}

>>>>>>> upstream/android-13
static void print_track(struct kasan_track *track, const char *prefix)
{
	pr_err("%s by task %u:\n", prefix, track->pid);
	if (track->stack) {
<<<<<<< HEAD
		struct stack_trace trace;

		depot_fetch_stack(track->stack, &trace);
		print_stack_trace(&trace, 0);
=======
		print_stack(track->stack);
>>>>>>> upstream/android-13
	} else {
		pr_err("(stack is not available)\n");
	}
}

struct page *kasan_addr_to_page(const void *addr)
{
	if ((addr >= (void *)PAGE_OFFSET) &&
			(addr < high_memory))
		return virt_to_head_page(addr);
	return NULL;
}

static void describe_object_addr(struct kmem_cache *cache, void *object,
				const void *addr)
{
	unsigned long access_addr = (unsigned long)addr;
	unsigned long object_addr = (unsigned long)object;
	const char *rel_type;
	int rel_bytes;

	pr_err("The buggy address belongs to the object at %px\n"
	       " which belongs to the cache %s of size %d\n",
		object, cache->name, cache->object_size);

	if (!addr)
		return;

	if (access_addr < object_addr) {
		rel_type = "to the left";
		rel_bytes = object_addr - access_addr;
	} else if (access_addr >= object_addr + cache->object_size) {
		rel_type = "to the right";
		rel_bytes = access_addr - (object_addr + cache->object_size);
	} else {
		rel_type = "inside";
		rel_bytes = access_addr - object_addr;
	}

	pr_err("The buggy address is located %d bytes %s of\n"
	       " %d-byte region [%px, %px)\n",
		rel_bytes, rel_type, cache->object_size, (void *)object_addr,
		(void *)(object_addr + cache->object_size));
}

<<<<<<< HEAD
static struct kasan_track *kasan_get_free_track(struct kmem_cache *cache,
		void *object, u8 tag)
{
	struct kasan_alloc_meta *alloc_meta;
	int i = 0;

	alloc_meta = get_alloc_info(cache, object);

#ifdef CONFIG_KASAN_SW_TAGS_IDENTIFY
	for (i = 0; i < KASAN_NR_FREE_STACKS; i++) {
		if (alloc_meta->free_pointer_tag[i] == tag)
			break;
	}
	if (i == KASAN_NR_FREE_STACKS)
		i = alloc_meta->free_track_idx;
#endif

	return &alloc_meta->free_track[i];
=======
static void describe_object_stacks(struct kmem_cache *cache, void *object,
					const void *addr, u8 tag)
{
	struct kasan_alloc_meta *alloc_meta;
	struct kasan_track *free_track;

	alloc_meta = kasan_get_alloc_meta(cache, object);
	if (alloc_meta) {
		print_track(&alloc_meta->alloc_track, "Allocated");
		pr_err("\n");
	}

	free_track = kasan_get_free_track(cache, object, tag);
	if (free_track) {
		print_track(free_track, "Freed");
		pr_err("\n");
	}

#ifdef CONFIG_KASAN_GENERIC
	if (!alloc_meta)
		return;
	if (alloc_meta->aux_stack[0]) {
		pr_err("Last potentially related work creation:\n");
		print_stack(alloc_meta->aux_stack[0]);
		pr_err("\n");
	}
	if (alloc_meta->aux_stack[1]) {
		pr_err("Second to last potentially related work creation:\n");
		print_stack(alloc_meta->aux_stack[1]);
		pr_err("\n");
	}
#endif
>>>>>>> upstream/android-13
}

static void describe_object(struct kmem_cache *cache, void *object,
				const void *addr, u8 tag)
{
<<<<<<< HEAD
	struct kasan_alloc_meta *alloc_info = get_alloc_info(cache, object);

	if (cache->flags & SLAB_KASAN) {
		struct kasan_track *free_track;

		print_track(&alloc_info->alloc_track, "Allocated");
		pr_err("\n");
		free_track = kasan_get_free_track(cache, object, tag);
		print_track(free_track, "Freed");
		pr_err("\n");
	}

=======
	if (kasan_stack_collection_enabled())
		describe_object_stacks(cache, object, addr, tag);
>>>>>>> upstream/android-13
	describe_object_addr(cache, object, addr);
}

static inline bool kernel_or_module_addr(const void *addr)
{
<<<<<<< HEAD
	if (addr >= (void *)_stext && addr < (void *)_end)
=======
	if (is_kernel((unsigned long)addr))
>>>>>>> upstream/android-13
		return true;
	if (is_module_address((unsigned long)addr))
		return true;
	return false;
}

static inline bool init_task_stack_addr(const void *addr)
{
	return addr >= (void *)&init_thread_union.stack &&
		(addr <= (void *)&init_thread_union.stack +
			sizeof(init_thread_union.stack));
}

static void print_address_description(void *addr, u8 tag)
{
	struct page *page = kasan_addr_to_page(addr);

<<<<<<< HEAD
	dump_stack();
=======
	dump_stack_lvl(KERN_ERR);
>>>>>>> upstream/android-13
	pr_err("\n");

	if (page && PageSlab(page)) {
		struct kmem_cache *cache = page->slab_cache;
		void *object = nearest_obj(cache, page,	addr);

		describe_object(cache, object, addr, tag);
	}

	if (kernel_or_module_addr(addr) && !init_task_stack_addr(addr)) {
		pr_err("The buggy address belongs to the variable:\n");
		pr_err(" %pS\n", addr);
	}

	if (page) {
		pr_err("The buggy address belongs to the page:\n");
		dump_page(page, "kasan: bad access detected");
	}
<<<<<<< HEAD
}

static bool row_is_guilty(const void *row, const void *guilty)
{
	return (row <= guilty) && (guilty < row + SHADOW_BYTES_PER_ROW);
}

static int shadow_pointer_offset(const void *row, const void *shadow)
{
	/* The length of ">ff00ff00ff00ff00: " is
	 *    3 + (BITS_PER_LONG/8)*2 chars.
	 */
	return 3 + (BITS_PER_LONG/8)*2 + (shadow - row)*2 +
		(shadow - row) / SHADOW_BYTES_PER_BLOCK + 1;
}

static void print_shadow_for_address(const void *addr)
{
	int i;
	const void *shadow = kasan_mem_to_shadow(addr);
	const void *shadow_row;

	shadow_row = (void *)round_down((unsigned long)shadow,
					SHADOW_BYTES_PER_ROW)
		- SHADOW_ROWS_AROUND_ADDR * SHADOW_BYTES_PER_ROW;

	pr_err("Memory state around the buggy address:\n");

	for (i = -SHADOW_ROWS_AROUND_ADDR; i <= SHADOW_ROWS_AROUND_ADDR; i++) {
		const void *kaddr = kasan_shadow_to_mem(shadow_row);
		char buffer[4 + (BITS_PER_LONG/8)*2];
		char shadow_buf[SHADOW_BYTES_PER_ROW];

		snprintf(buffer, sizeof(buffer),
			(i == 0) ? ">%px: " : " %px: ", kaddr);
=======

	kasan_print_address_stack_frame(addr);
}

static bool meta_row_is_guilty(const void *row, const void *addr)
{
	return (row <= addr) && (addr < row + META_MEM_BYTES_PER_ROW);
}

static int meta_pointer_offset(const void *row, const void *addr)
{
	/*
	 * Memory state around the buggy address:
	 *  ff00ff00ff00ff00: 00 00 00 05 fe fe fe fe fe fe fe fe fe fe fe fe
	 *  ...
	 *
	 * The length of ">ff00ff00ff00ff00: " is
	 *    3 + (BITS_PER_LONG / 8) * 2 chars.
	 * The length of each granule metadata is 2 bytes
	 *    plus 1 byte for space.
	 */
	return 3 + (BITS_PER_LONG / 8) * 2 +
		(addr - row) / KASAN_GRANULE_SIZE * 3 + 1;
}

static void print_memory_metadata(const void *addr)
{
	int i;
	void *row;

	row = (void *)round_down((unsigned long)addr, META_MEM_BYTES_PER_ROW)
			- META_ROWS_AROUND_ADDR * META_MEM_BYTES_PER_ROW;

	pr_err("Memory state around the buggy address:\n");

	for (i = -META_ROWS_AROUND_ADDR; i <= META_ROWS_AROUND_ADDR; i++) {
		char buffer[4 + (BITS_PER_LONG / 8) * 2];
		char metadata[META_BYTES_PER_ROW];

		snprintf(buffer, sizeof(buffer),
				(i == 0) ? ">%px: " : " %px: ", row);

>>>>>>> upstream/android-13
		/*
		 * We should not pass a shadow pointer to generic
		 * function, because generic functions may try to
		 * access kasan mapping for the passed address.
		 */
<<<<<<< HEAD
		memcpy(shadow_buf, shadow_row, SHADOW_BYTES_PER_ROW);
		print_hex_dump(KERN_ERR, buffer,
			DUMP_PREFIX_NONE, SHADOW_BYTES_PER_ROW, 1,
			shadow_buf, SHADOW_BYTES_PER_ROW, 0);

		if (row_is_guilty(shadow_row, shadow))
			pr_err("%*c\n",
				shadow_pointer_offset(shadow_row, shadow),
				'^');

		shadow_row += SHADOW_BYTES_PER_ROW;
=======
		kasan_metadata_fetch_row(&metadata[0], row);

		print_hex_dump(KERN_ERR, buffer,
			DUMP_PREFIX_NONE, META_BYTES_PER_ROW, 1,
			metadata, META_BYTES_PER_ROW, 0);

		if (meta_row_is_guilty(row, addr))
			pr_err("%*c\n", meta_pointer_offset(row, addr), '^');

		row += META_MEM_BYTES_PER_ROW;
>>>>>>> upstream/android-13
	}
}

static bool report_enabled(void)
{
<<<<<<< HEAD
	if (current->kasan_depth)
		return false;
=======
#if defined(CONFIG_KASAN_GENERIC) || defined(CONFIG_KASAN_SW_TAGS)
	if (current->kasan_depth)
		return false;
#endif
>>>>>>> upstream/android-13
	if (test_bit(KASAN_BIT_MULTI_SHOT, &kasan_flags))
		return true;
	return !test_and_set_bit(KASAN_BIT_REPORTED, &kasan_flags);
}

<<<<<<< HEAD
=======
static void kasan_update_kunit_status(struct kunit *cur_test, bool sync)
{
	struct kunit_resource *resource;
	struct kunit_kasan_status *status;

	resource = kunit_find_named_resource(cur_test, "kasan_status");

	if (!resource) {
		kunit_set_failure(cur_test);
		return;
	}

	status = (struct kunit_kasan_status *)resource->data;
	WRITE_ONCE(status->report_found, true);
	WRITE_ONCE(status->sync_fault, sync);
	kunit_put_resource(resource);
}

>>>>>>> upstream/android-13
void kasan_report_invalid_free(void *object, unsigned long ip)
{
	unsigned long flags;
	u8 tag = get_tag(object);

<<<<<<< HEAD
	object = reset_tag(object);
	start_report(&flags);
	pr_err("BUG: KASAN: double-free or invalid-free in %pS\n", (void *)ip);
	print_tags(tag, object);
	pr_err("\n");
	print_address_description(object, tag);
	pr_err("\n");
	print_shadow_for_address(object);
	end_report(&flags);
	/* trigger KE to get the KAsan corruption message */
	BUG();
}

void __kasan_report(unsigned long addr, size_t size, bool is_write, unsigned long ip)
=======
	object = kasan_reset_tag(object);

	if (current->kunit_test)
		kasan_update_kunit_status(current->kunit_test, true);

	start_report(&flags);
	pr_err("BUG: KASAN: double-free or invalid-free in %pS\n", (void *)ip);
	kasan_print_tags(tag, object);
	pr_err("\n");
	print_address_description(object, tag);
	pr_err("\n");
	print_memory_metadata(object);
	end_report(&flags, (unsigned long)object);
}

#ifdef CONFIG_KASAN_HW_TAGS
void kasan_report_async(void)
{
	unsigned long flags;

	if (current->kunit_test)
		kasan_update_kunit_status(current->kunit_test, false);

	start_report(&flags);
	pr_err("BUG: KASAN: invalid-access\n");
	pr_err("Asynchronous mode enabled: no access details available\n");
	pr_err("\n");
	dump_stack_lvl(KERN_ERR);
	end_report(&flags, 0);
}
#endif /* CONFIG_KASAN_HW_TAGS */

static void __kasan_report(unsigned long addr, size_t size, bool is_write,
				unsigned long ip)
>>>>>>> upstream/android-13
{
	struct kasan_access_info info;
	void *tagged_addr;
	void *untagged_addr;
	unsigned long flags;

<<<<<<< HEAD
	if (likely(!report_enabled()))
		return;
=======
	if (current->kunit_test)
		kasan_update_kunit_status(current->kunit_test, true);
>>>>>>> upstream/android-13

	disable_trace_on_warning();

	tagged_addr = (void *)addr;
<<<<<<< HEAD
	untagged_addr = reset_tag(tagged_addr);

	info.access_addr = tagged_addr;
	if (addr_has_shadow(untagged_addr))
		info.first_bad_addr = find_first_bad_addr(tagged_addr, size);
=======
	untagged_addr = kasan_reset_tag(tagged_addr);

	info.access_addr = tagged_addr;
	if (addr_has_metadata(untagged_addr))
		info.first_bad_addr =
			kasan_find_first_bad_addr(tagged_addr, size);
>>>>>>> upstream/android-13
	else
		info.first_bad_addr = untagged_addr;
	info.access_size = size;
	info.is_write = is_write;
	info.ip = ip;

	start_report(&flags);

	print_error_description(&info);
<<<<<<< HEAD
	if (addr_has_shadow(untagged_addr))
		print_tags(get_tag(tagged_addr), info.first_bad_addr);
	pr_err("\n");

	if (addr_has_shadow(untagged_addr)) {
		print_address_description(untagged_addr, get_tag(tagged_addr));
		pr_err("\n");
		print_shadow_for_address(info.first_bad_addr);
	} else {
		dump_stack();
	}

	end_report(&flags);
	/* trigger KE to get the KAsan corruption message */
	BUG();
}
=======
	if (addr_has_metadata(untagged_addr))
		kasan_print_tags(get_tag(tagged_addr), info.first_bad_addr);
	pr_err("\n");

	if (addr_has_metadata(untagged_addr)) {
		print_address_description(untagged_addr, get_tag(tagged_addr));
		pr_err("\n");
		print_memory_metadata(info.first_bad_addr);
	} else {
		dump_stack_lvl(KERN_ERR);
	}

	end_report(&flags, addr);
}

bool kasan_report(unsigned long addr, size_t size, bool is_write,
			unsigned long ip)
{
	unsigned long flags = user_access_save();
	bool ret = false;

	if (likely(report_enabled())) {
		__kasan_report(addr, size, is_write, ip);
		ret = true;
	}

	user_access_restore(flags);

	return ret;
}

#ifdef CONFIG_KASAN_INLINE
/*
 * With CONFIG_KASAN_INLINE, accesses to bogus pointers (outside the high
 * canonical half of the address space) cause out-of-bounds shadow memory reads
 * before the actual access. For addresses in the low canonical half of the
 * address space, as well as most non-canonical addresses, that out-of-bounds
 * shadow memory access lands in the non-canonical part of the address space.
 * Help the user figure out what the original bogus pointer was.
 */
void kasan_non_canonical_hook(unsigned long addr)
{
	unsigned long orig_addr;
	const char *bug_type;

	if (addr < KASAN_SHADOW_OFFSET)
		return;

	orig_addr = (addr - KASAN_SHADOW_OFFSET) << KASAN_SHADOW_SCALE_SHIFT;
	/*
	 * For faults near the shadow address for NULL, we can be fairly certain
	 * that this is a KASAN shadow memory access.
	 * For faults that correspond to shadow for low canonical addresses, we
	 * can still be pretty sure - that shadow region is a fairly narrow
	 * chunk of the non-canonical address space.
	 * But faults that look like shadow for non-canonical addresses are a
	 * really large chunk of the address space. In that case, we still
	 * print the decoded address, but make it clear that this is not
	 * necessarily what's actually going on.
	 */
	if (orig_addr < PAGE_SIZE)
		bug_type = "null-ptr-deref";
	else if (orig_addr < TASK_SIZE)
		bug_type = "probably user-memory-access";
	else
		bug_type = "maybe wild-memory-access";
	pr_alert("KASAN: %s in range [0x%016lx-0x%016lx]\n", bug_type,
		 orig_addr, orig_addr + KASAN_GRANULE_SIZE - 1);
}
#endif
>>>>>>> upstream/android-13
