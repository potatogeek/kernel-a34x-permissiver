<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> upstream/android-13
/*
 * UBSAN error reporting functions
 *
 * Copyright (c) 2014 Samsung Electronics Co., Ltd.
 * Author: Andrey Ryabinin <ryabinin.a.a@gmail.com>
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
#include <linux/bug.h>
#include <linux/ctype.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/sched.h>
<<<<<<< HEAD

#include "ubsan.h"

#include "../kernel/sched/sched.h"
#include "../drivers/misc/mediatek/include/mt-plat/aee.h"

const char *type_check_kinds[] = {
=======
#include <linux/uaccess.h>
#include <kunit/test-bug.h>

#include "ubsan.h"

static const char * const type_check_kinds[] = {
>>>>>>> upstream/android-13
	"load of",
	"store to",
	"reference binding to",
	"member access within",
	"member call on",
	"constructor call on",
	"downcast of",
	"downcast of"
};

#define REPORTED_BIT 31

#if (BITS_PER_LONG == 64) && defined(__BIG_ENDIAN)
#define COLUMN_MASK (~(1U << REPORTED_BIT))
#define LINE_MASK   (~0U)
#else
#define COLUMN_MASK   (~0U)
#define LINE_MASK (~(1U << REPORTED_BIT))
#endif

#define VALUE_LENGTH 40

static bool was_reported(struct source_location *location)
{
	return test_and_set_bit(REPORTED_BIT, &location->reported);
}

<<<<<<< HEAD
static void print_source_location(const char *prefix,
				struct source_location *loc)
{
	pr_err("%s %s:%d:%d\n", prefix, loc->file_name,
		loc->line & LINE_MASK, loc->column & COLUMN_MASK);
}

=======
>>>>>>> upstream/android-13
static bool suppress_report(struct source_location *loc)
{
	return current->in_ubsan || was_reported(loc);
}

static bool type_is_int(struct type_descriptor *type)
{
	return type->type_kind == type_kind_int;
}

static bool type_is_signed(struct type_descriptor *type)
{
	WARN_ON(!type_is_int(type));
	return  type->type_info & 1;
}

static unsigned type_bit_width(struct type_descriptor *type)
{
	return 1 << (type->type_info >> 1);
}

static bool is_inline_int(struct type_descriptor *type)
{
	unsigned inline_bits = sizeof(unsigned long)*8;
	unsigned bits = type_bit_width(type);

	WARN_ON(!type_is_int(type));

	return bits <= inline_bits;
}

static s_max get_signed_val(struct type_descriptor *type, void *val)
{
	if (is_inline_int(type)) {
		unsigned extra_bits = sizeof(s_max)*8 - type_bit_width(type);
		unsigned long ulong_val = (unsigned long)val;

		return ((s_max)ulong_val) << extra_bits >> extra_bits;
	}

	if (type_bit_width(type) == 64)
		return *(s64 *)val;

	return *(s_max *)val;
}

static bool val_is_negative(struct type_descriptor *type, void *val)
{
	return type_is_signed(type) && get_signed_val(type, val) < 0;
}

static u_max get_unsigned_val(struct type_descriptor *type, void *val)
{
	if (is_inline_int(type))
		return (unsigned long)val;

	if (type_bit_width(type) == 64)
		return *(u64 *)val;

	return *(u_max *)val;
}

static void val_to_string(char *str, size_t size, struct type_descriptor *type,
			void *value)
{
	if (type_is_int(type)) {
		if (type_bit_width(type) == 128) {
<<<<<<< HEAD
#if defined(CONFIG_ARCH_SUPPORTS_INT128) && defined(__SIZEOF_INT128__)
=======
#if defined(CONFIG_ARCH_SUPPORTS_INT128)
>>>>>>> upstream/android-13
			u_max val = get_unsigned_val(type, value);

			scnprintf(str, size, "0x%08x%08x%08x%08x",
				(u32)(val >> 96),
				(u32)(val >> 64),
				(u32)(val >> 32),
				(u32)(val));
#else
			WARN_ON(1);
#endif
		} else if (type_is_signed(type)) {
			scnprintf(str, size, "%lld",
				(s64)get_signed_val(type, value));
		} else {
			scnprintf(str, size, "%llu",
				(u64)get_unsigned_val(type, value));
		}
	}
}

<<<<<<< HEAD
static DEFINE_SPINLOCK(report_lock);

static void ubsan_prologue(struct source_location *location,
			unsigned long *flags)
{
	current->in_ubsan++;
	spin_lock_irqsave(&report_lock, *flags);

	pr_err("========================================"
		"========================================\n");
	print_source_location("UBSAN: Undefined behaviour in", location);
}

static void ubsan_epilogue(unsigned long *flags)
{
	int cpu;
	struct rq *rq;

	dump_stack();
	pr_err("========================================"
		"========================================\n");
	spin_unlock_irqrestore(&report_lock, *flags);
	current->in_ubsan--;

	cpu = raw_smp_processor_id();
	rq = cpu_rq(cpu);
	if (!raw_spin_is_locked(&rq->lock)) {
		/* AEE Kernel API Dump for UBSan */
		aee_kernel_warning_api(__FILE__, __LINE__, DB_OPT_DEFAULT,
			"UBSan error",
			"[UBSan report]");
	} else {
		BUG();
	}

}

static void handle_overflow(struct overflow_data *data, void *lhs,
			void *rhs, char op)
{

	struct type_descriptor *type = data->type;
	unsigned long flags;
	char lhs_val_str[VALUE_LENGTH];
=======
static void ubsan_prologue(struct source_location *loc, const char *reason)
{
	current->in_ubsan++;

	pr_err("========================================"
		"========================================\n");
	pr_err("UBSAN: %s in %s:%d:%d\n", reason, loc->file_name,
		loc->line & LINE_MASK, loc->column & COLUMN_MASK);

	kunit_fail_current_test("%s in %s", reason, loc->file_name);
}

static void ubsan_epilogue(void)
{
	dump_stack();
	pr_err("========================================"
		"========================================\n");

	current->in_ubsan--;

	if (panic_on_warn) {
		/*
		 * This thread may hit another WARN() in the panic path.
		 * Resetting this prevents additional WARN() from panicking the
		 * system on this thread.  Other threads are blocked by the
		 * panic_mutex in panic().
		 */
		panic_on_warn = 0;
		panic("panic_on_warn set ...\n");
	}
}

void __ubsan_handle_divrem_overflow(void *_data, void *lhs, void *rhs)
{
	struct overflow_data *data = _data;
>>>>>>> upstream/android-13
	char rhs_val_str[VALUE_LENGTH];

	if (suppress_report(&data->location))
		return;

<<<<<<< HEAD
	ubsan_prologue(&data->location, &flags);

	val_to_string(lhs_val_str, sizeof(lhs_val_str), type, lhs);
	val_to_string(rhs_val_str, sizeof(rhs_val_str), type, rhs);
	pr_err("%s integer overflow:\n",
		type_is_signed(type) ? "signed" : "unsigned");
	pr_err("%s %c %s cannot be represented in type %s\n",
		lhs_val_str,
		op,
		rhs_val_str,
		type->type_name);

	ubsan_epilogue(&flags);
}

void __ubsan_handle_add_overflow(struct overflow_data *data,
				void *lhs, void *rhs)
{

	handle_overflow(data, lhs, rhs, '+');
}
EXPORT_SYMBOL(__ubsan_handle_add_overflow);

void __ubsan_handle_sub_overflow(struct overflow_data *data,
				void *lhs, void *rhs)
{
	handle_overflow(data, lhs, rhs, '-');
}
EXPORT_SYMBOL(__ubsan_handle_sub_overflow);

void __ubsan_handle_mul_overflow(struct overflow_data *data,
				void *lhs, void *rhs)
{
	handle_overflow(data, lhs, rhs, '*');
}
EXPORT_SYMBOL(__ubsan_handle_mul_overflow);

void __ubsan_handle_negate_overflow(struct overflow_data *data,
				void *old_val)
{
	unsigned long flags;
	char old_val_str[VALUE_LENGTH];

	if (suppress_report(&data->location))
		return;

	ubsan_prologue(&data->location, &flags);

	val_to_string(old_val_str, sizeof(old_val_str), data->type, old_val);

	pr_err("negation of %s cannot be represented in type %s:\n",
		old_val_str, data->type->type_name);

	ubsan_epilogue(&flags);
}
EXPORT_SYMBOL(__ubsan_handle_negate_overflow);


void __ubsan_handle_divrem_overflow(struct overflow_data *data,
				void *lhs, void *rhs)
{
	unsigned long flags;
	char rhs_val_str[VALUE_LENGTH];

	if (suppress_report(&data->location))
		return;

	ubsan_prologue(&data->location, &flags);
=======
	ubsan_prologue(&data->location, "division-overflow");
>>>>>>> upstream/android-13

	val_to_string(rhs_val_str, sizeof(rhs_val_str), data->type, rhs);

	if (type_is_signed(data->type) && get_signed_val(data->type, rhs) == -1)
		pr_err("division of %s by -1 cannot be represented in type %s\n",
			rhs_val_str, data->type->type_name);
	else
		pr_err("division by zero\n");

<<<<<<< HEAD
	ubsan_epilogue(&flags);
=======
	ubsan_epilogue();
>>>>>>> upstream/android-13
}
EXPORT_SYMBOL(__ubsan_handle_divrem_overflow);

static void handle_null_ptr_deref(struct type_mismatch_data_common *data)
{
<<<<<<< HEAD
	unsigned long flags;

	if (suppress_report(data->location))
		return;

	ubsan_prologue(data->location, &flags);
=======
	if (suppress_report(data->location))
		return;

	ubsan_prologue(data->location, "null-ptr-deref");
>>>>>>> upstream/android-13

	pr_err("%s null pointer of type %s\n",
		type_check_kinds[data->type_check_kind],
		data->type->type_name);

<<<<<<< HEAD
	ubsan_epilogue(&flags);
=======
	ubsan_epilogue();
>>>>>>> upstream/android-13
}

static void handle_misaligned_access(struct type_mismatch_data_common *data,
				unsigned long ptr)
{
<<<<<<< HEAD
	unsigned long flags;

	if (suppress_report(data->location))
		return;

	ubsan_prologue(data->location, &flags);
=======
	if (suppress_report(data->location))
		return;

	ubsan_prologue(data->location, "misaligned-access");
>>>>>>> upstream/android-13

	pr_err("%s misaligned address %p for type %s\n",
		type_check_kinds[data->type_check_kind],
		(void *)ptr, data->type->type_name);
	pr_err("which requires %ld byte alignment\n", data->alignment);

<<<<<<< HEAD
	ubsan_epilogue(&flags);
=======
	ubsan_epilogue();
>>>>>>> upstream/android-13
}

static void handle_object_size_mismatch(struct type_mismatch_data_common *data,
					unsigned long ptr)
{
<<<<<<< HEAD
	unsigned long flags;

	if (suppress_report(data->location))
		return;

	ubsan_prologue(data->location, &flags);
=======
	if (suppress_report(data->location))
		return;

	ubsan_prologue(data->location, "object-size-mismatch");
>>>>>>> upstream/android-13
	pr_err("%s address %p with insufficient space\n",
		type_check_kinds[data->type_check_kind],
		(void *) ptr);
	pr_err("for an object of type %s\n", data->type->type_name);
<<<<<<< HEAD
	ubsan_epilogue(&flags);
=======
	ubsan_epilogue();
>>>>>>> upstream/android-13
}

static void ubsan_type_mismatch_common(struct type_mismatch_data_common *data,
				unsigned long ptr)
{
<<<<<<< HEAD
=======
	unsigned long flags = user_access_save();
>>>>>>> upstream/android-13

	if (!ptr)
		handle_null_ptr_deref(data);
	else if (data->alignment && !IS_ALIGNED(ptr, data->alignment))
		handle_misaligned_access(data, ptr);
	else
		handle_object_size_mismatch(data, ptr);
<<<<<<< HEAD
=======

	user_access_restore(flags);
>>>>>>> upstream/android-13
}

void __ubsan_handle_type_mismatch(struct type_mismatch_data *data,
				void *ptr)
{
	struct type_mismatch_data_common common_data = {
		.location = &data->location,
		.type = data->type,
		.alignment = data->alignment,
		.type_check_kind = data->type_check_kind
	};

	ubsan_type_mismatch_common(&common_data, (unsigned long)ptr);
}
EXPORT_SYMBOL(__ubsan_handle_type_mismatch);

<<<<<<< HEAD
void __ubsan_handle_type_mismatch_v1(struct type_mismatch_data_v1 *data,
				void *ptr)
{

=======
void __ubsan_handle_type_mismatch_v1(void *_data, void *ptr)
{
	struct type_mismatch_data_v1 *data = _data;
>>>>>>> upstream/android-13
	struct type_mismatch_data_common common_data = {
		.location = &data->location,
		.type = data->type,
		.alignment = 1UL << data->log_alignment,
		.type_check_kind = data->type_check_kind
	};

	ubsan_type_mismatch_common(&common_data, (unsigned long)ptr);
}
EXPORT_SYMBOL(__ubsan_handle_type_mismatch_v1);

<<<<<<< HEAD
void __ubsan_handle_vla_bound_not_positive(struct vla_bound_data *data,
					void *bound)
{
	unsigned long flags;
	char bound_str[VALUE_LENGTH];

	if (suppress_report(&data->location))
		return;

	ubsan_prologue(&data->location, &flags);

	val_to_string(bound_str, sizeof(bound_str), data->type, bound);
	pr_err("variable length array bound value %s <= 0\n", bound_str);

	ubsan_epilogue(&flags);
}
EXPORT_SYMBOL(__ubsan_handle_vla_bound_not_positive);

void __ubsan_handle_out_of_bounds(struct out_of_bounds_data *data, void *index)
{
	unsigned long flags;
=======
void __ubsan_handle_out_of_bounds(void *_data, void *index)
{
	struct out_of_bounds_data *data = _data;
>>>>>>> upstream/android-13
	char index_str[VALUE_LENGTH];

	if (suppress_report(&data->location))
		return;

<<<<<<< HEAD
	ubsan_prologue(&data->location, &flags);
=======
	ubsan_prologue(&data->location, "array-index-out-of-bounds");
>>>>>>> upstream/android-13

	val_to_string(index_str, sizeof(index_str), data->index_type, index);
	pr_err("index %s is out of range for type %s\n", index_str,
		data->array_type->type_name);
<<<<<<< HEAD
	ubsan_epilogue(&flags);
}
EXPORT_SYMBOL(__ubsan_handle_out_of_bounds);

void __ubsan_handle_shift_out_of_bounds(struct shift_out_of_bounds_data *data,
					void *lhs, void *rhs)
{
	unsigned long flags;
=======
	ubsan_epilogue();
}
EXPORT_SYMBOL(__ubsan_handle_out_of_bounds);

void __ubsan_handle_shift_out_of_bounds(void *_data, void *lhs, void *rhs)
{
	struct shift_out_of_bounds_data *data = _data;
>>>>>>> upstream/android-13
	struct type_descriptor *rhs_type = data->rhs_type;
	struct type_descriptor *lhs_type = data->lhs_type;
	char rhs_str[VALUE_LENGTH];
	char lhs_str[VALUE_LENGTH];
<<<<<<< HEAD

	if (suppress_report(&data->location))
		return;

	ubsan_prologue(&data->location, &flags);
=======
	unsigned long ua_flags = user_access_save();

	if (suppress_report(&data->location))
		goto out;

	ubsan_prologue(&data->location, "shift-out-of-bounds");
>>>>>>> upstream/android-13

	val_to_string(rhs_str, sizeof(rhs_str), rhs_type, rhs);
	val_to_string(lhs_str, sizeof(lhs_str), lhs_type, lhs);

	if (val_is_negative(rhs_type, rhs))
		pr_err("shift exponent %s is negative\n", rhs_str);

	else if (get_unsigned_val(rhs_type, rhs) >=
		type_bit_width(lhs_type))
		pr_err("shift exponent %s is too large for %u-bit type %s\n",
			rhs_str,
			type_bit_width(lhs_type),
			lhs_type->type_name);
	else if (val_is_negative(lhs_type, lhs))
		pr_err("left shift of negative value %s\n",
			lhs_str);
	else
		pr_err("left shift of %s by %s places cannot be"
			" represented in type %s\n",
			lhs_str, rhs_str,
			lhs_type->type_name);

<<<<<<< HEAD
	ubsan_epilogue(&flags);
=======
	ubsan_epilogue();
out:
	user_access_restore(ua_flags);
>>>>>>> upstream/android-13
}
EXPORT_SYMBOL(__ubsan_handle_shift_out_of_bounds);


<<<<<<< HEAD
void __ubsan_handle_builtin_unreachable(struct unreachable_data *data)
{
	unsigned long flags;

	ubsan_prologue(&data->location, &flags);
	pr_err("calling __builtin_unreachable()\n");
	ubsan_epilogue(&flags);
=======
void __ubsan_handle_builtin_unreachable(void *_data)
{
	struct unreachable_data *data = _data;
	ubsan_prologue(&data->location, "unreachable");
	pr_err("calling __builtin_unreachable()\n");
	ubsan_epilogue();
>>>>>>> upstream/android-13
	panic("can't return from __builtin_unreachable()");
}
EXPORT_SYMBOL(__ubsan_handle_builtin_unreachable);

<<<<<<< HEAD
void __ubsan_handle_load_invalid_value(struct invalid_value_data *data,
				void *val)
{
	unsigned long flags;
=======
void __ubsan_handle_load_invalid_value(void *_data, void *val)
{
	struct invalid_value_data *data = _data;
>>>>>>> upstream/android-13
	char val_str[VALUE_LENGTH];

	if (suppress_report(&data->location))
		return;

<<<<<<< HEAD
	ubsan_prologue(&data->location, &flags);
=======
	ubsan_prologue(&data->location, "invalid-load");
>>>>>>> upstream/android-13

	val_to_string(val_str, sizeof(val_str), data->type, val);

	pr_err("load of value %s is not a valid value for type %s\n",
		val_str, data->type->type_name);

<<<<<<< HEAD
	ubsan_epilogue(&flags);
}
EXPORT_SYMBOL(__ubsan_handle_load_invalid_value);
=======
	ubsan_epilogue();
}
EXPORT_SYMBOL(__ubsan_handle_load_invalid_value);

void __ubsan_handle_alignment_assumption(void *_data, unsigned long ptr,
					 unsigned long align,
					 unsigned long offset);
void __ubsan_handle_alignment_assumption(void *_data, unsigned long ptr,
					 unsigned long align,
					 unsigned long offset)
{
	struct alignment_assumption_data *data = _data;
	unsigned long real_ptr;

	if (suppress_report(&data->location))
		return;

	ubsan_prologue(&data->location, "alignment-assumption");

	if (offset)
		pr_err("assumption of %lu byte alignment (with offset of %lu byte) for pointer of type %s failed",
		       align, offset, data->type->type_name);
	else
		pr_err("assumption of %lu byte alignment for pointer of type %s failed",
		       align, data->type->type_name);

	real_ptr = ptr - offset;
	pr_err("%saddress is %lu aligned, misalignment offset is %lu bytes",
	       offset ? "offset " : "", BIT(real_ptr ? __ffs(real_ptr) : 0),
	       real_ptr & (align - 1));

	ubsan_epilogue();
}
EXPORT_SYMBOL(__ubsan_handle_alignment_assumption);
>>>>>>> upstream/android-13
