<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> upstream/android-13
/*
 * runtime-wrappers.c - Runtime Services function call wrappers
 *
 * Implementation summary:
 * -----------------------
 * 1. When user/kernel thread requests to execute efi_runtime_service(),
 * enqueue work to efi_rts_wq.
 * 2. Caller thread waits for completion until the work is finished
 * because it's dependent on the return status and execution of
 * efi_runtime_service().
 * For instance, get_variable() and get_next_variable().
 *
 * Copyright (C) 2014 Linaro Ltd. <ard.biesheuvel@linaro.org>
 *
 * Split off from arch/x86/platform/efi/efi.c
 *
 * Copyright (C) 1999 VA Linux Systems
 * Copyright (C) 1999 Walt Drummond <drummond@valinux.com>
 * Copyright (C) 1999-2002 Hewlett-Packard Co.
 * Copyright (C) 2005-2008 Intel Co.
 * Copyright (C) 2013 SuSE Labs
<<<<<<< HEAD
 *
 * This file is released under the GPLv2.
=======
>>>>>>> upstream/android-13
 */

#define pr_fmt(fmt)	"efi: " fmt

#include <linux/bug.h>
#include <linux/efi.h>
#include <linux/irqflags.h>
#include <linux/mutex.h>
#include <linux/semaphore.h>
#include <linux/stringify.h>
#include <linux/workqueue.h>
#include <linux/completion.h>

#include <asm/efi.h>

/*
 * Wrap around the new efi_call_virt_generic() macros so that the
 * code doesn't get too cluttered:
 */
#define efi_call_virt(f, args...)   \
<<<<<<< HEAD
	efi_call_virt_pointer(efi.systab->runtime, f, args)
#define __efi_call_virt(f, args...) \
	__efi_call_virt_pointer(efi.systab->runtime, f, args)
=======
	efi_call_virt_pointer(efi.runtime, f, args)
#define __efi_call_virt(f, args...) \
	__efi_call_virt_pointer(efi.runtime, f, args)
>>>>>>> upstream/android-13

struct efi_runtime_work efi_rts_work;

/*
 * efi_queue_work:	Queue efi_runtime_service() and wait until it's done
 * @rts:		efi_runtime_service() function identifier
 * @rts_arg<1-5>:	efi_runtime_service() function arguments
 *
 * Accesses to efi_runtime_services() are serialized by a binary
 * semaphore (efi_runtime_lock) and caller waits until the work is
 * finished, hence _only_ one work is queued at a time and the caller
 * thread waits for completion.
 */
#define efi_queue_work(_rts, _arg1, _arg2, _arg3, _arg4, _arg5)		\
({									\
	efi_rts_work.status = EFI_ABORTED;				\
									\
<<<<<<< HEAD
=======
	if (!efi_enabled(EFI_RUNTIME_SERVICES)) {			\
		pr_warn_once("EFI Runtime Services are disabled!\n");	\
		goto exit;						\
	}								\
									\
>>>>>>> upstream/android-13
	init_completion(&efi_rts_work.efi_rts_comp);			\
	INIT_WORK(&efi_rts_work.work, efi_call_rts);			\
	efi_rts_work.arg1 = _arg1;					\
	efi_rts_work.arg2 = _arg2;					\
	efi_rts_work.arg3 = _arg3;					\
	efi_rts_work.arg4 = _arg4;					\
	efi_rts_work.arg5 = _arg5;					\
	efi_rts_work.efi_rts_id = _rts;					\
									\
	/*								\
	 * queue_work() returns 0 if work was already on queue,         \
	 * _ideally_ this should never happen.                          \
	 */								\
	if (queue_work(efi_rts_wq, &efi_rts_work.work))			\
		wait_for_completion(&efi_rts_work.efi_rts_comp);	\
	else								\
		pr_err("Failed to queue work to efi_rts_wq.\n");	\
									\
<<<<<<< HEAD
	efi_rts_work.status;						\
})

=======
exit:									\
	efi_rts_work.efi_rts_id = EFI_NONE;				\
	efi_rts_work.status;						\
})

#ifndef arch_efi_save_flags
#define arch_efi_save_flags(state_flags)	local_save_flags(state_flags)
#define arch_efi_restore_flags(state_flags)	local_irq_restore(state_flags)
#endif

unsigned long efi_call_virt_save_flags(void)
{
	unsigned long flags;

	arch_efi_save_flags(flags);
	return flags;
}

>>>>>>> upstream/android-13
void efi_call_virt_check_flags(unsigned long flags, const char *call)
{
	unsigned long cur_flags, mismatch;

<<<<<<< HEAD
	local_save_flags(cur_flags);
=======
	cur_flags = efi_call_virt_save_flags();
>>>>>>> upstream/android-13

	mismatch = flags ^ cur_flags;
	if (!WARN_ON_ONCE(mismatch & ARCH_EFI_IRQ_FLAGS_MASK))
		return;

	add_taint(TAINT_FIRMWARE_WORKAROUND, LOCKDEP_NOW_UNRELIABLE);
	pr_err_ratelimited(FW_BUG "IRQ flags corrupted (0x%08lx=>0x%08lx) by EFI %s\n",
			   flags, cur_flags, call);
<<<<<<< HEAD
	local_irq_restore(flags);
=======
	arch_efi_restore_flags(flags);
>>>>>>> upstream/android-13
}

/*
 * According to section 7.1 of the UEFI spec, Runtime Services are not fully
 * reentrant, and there are particular combinations of calls that need to be
 * serialized. (source: UEFI Specification v2.4A)
 *
 * Table 31. Rules for Reentry Into Runtime Services
 * +------------------------------------+-------------------------------+
 * | If previous call is busy in	| Forbidden to call		|
 * +------------------------------------+-------------------------------+
 * | Any				| SetVirtualAddressMap()	|
 * +------------------------------------+-------------------------------+
 * | ConvertPointer()			| ConvertPointer()		|
 * +------------------------------------+-------------------------------+
 * | SetVariable()			| ResetSystem()			|
 * | UpdateCapsule()			|				|
 * | SetTime()				|				|
 * | SetWakeupTime()			|				|
 * | GetNextHighMonotonicCount()	|				|
 * +------------------------------------+-------------------------------+
 * | GetVariable()			| GetVariable()			|
 * | GetNextVariableName()		| GetNextVariableName()		|
 * | SetVariable()			| SetVariable()			|
 * | QueryVariableInfo()		| QueryVariableInfo()		|
 * | UpdateCapsule()			| UpdateCapsule()		|
 * | QueryCapsuleCapabilities()		| QueryCapsuleCapabilities()	|
 * | GetNextHighMonotonicCount()	| GetNextHighMonotonicCount()	|
 * +------------------------------------+-------------------------------+
 * | GetTime()				| GetTime()			|
 * | SetTime()				| SetTime()			|
 * | GetWakeupTime()			| GetWakeupTime()		|
 * | SetWakeupTime()			| SetWakeupTime()		|
 * +------------------------------------+-------------------------------+
 *
 * Due to the fact that the EFI pstore may write to the variable store in
 * interrupt context, we need to use a lock for at least the groups that
 * contain SetVariable() and QueryVariableInfo(). That leaves little else, as
 * none of the remaining functions are actually ever called at runtime.
 * So let's just use a single lock to serialize all Runtime Services calls.
 */
static DEFINE_SEMAPHORE(efi_runtime_lock);

/*
 * Expose the EFI runtime lock to the UV platform
 */
#ifdef CONFIG_X86_UV
extern struct semaphore __efi_uv_runtime_lock __alias(efi_runtime_lock);
#endif

/*
 * Calls the appropriate efi_runtime_service() with the appropriate
 * arguments.
 *
 * Semantics followed by efi_call_rts() to understand efi_runtime_work:
 * 1. If argument was a pointer, recast it from void pointer to original
 * pointer type.
 * 2. If argument was a value, recast it from void pointer to original
 * pointer type and dereference it.
 */
static void efi_call_rts(struct work_struct *work)
{
	void *arg1, *arg2, *arg3, *arg4, *arg5;
	efi_status_t status = EFI_NOT_FOUND;

	arg1 = efi_rts_work.arg1;
	arg2 = efi_rts_work.arg2;
	arg3 = efi_rts_work.arg3;
	arg4 = efi_rts_work.arg4;
	arg5 = efi_rts_work.arg5;

	switch (efi_rts_work.efi_rts_id) {
<<<<<<< HEAD
	case GET_TIME:
		status = efi_call_virt(get_time, (efi_time_t *)arg1,
				       (efi_time_cap_t *)arg2);
		break;
	case SET_TIME:
		status = efi_call_virt(set_time, (efi_time_t *)arg1);
		break;
	case GET_WAKEUP_TIME:
		status = efi_call_virt(get_wakeup_time, (efi_bool_t *)arg1,
				       (efi_bool_t *)arg2, (efi_time_t *)arg3);
		break;
	case SET_WAKEUP_TIME:
		status = efi_call_virt(set_wakeup_time, *(efi_bool_t *)arg1,
				       (efi_time_t *)arg2);
		break;
	case GET_VARIABLE:
=======
	case EFI_GET_TIME:
		status = efi_call_virt(get_time, (efi_time_t *)arg1,
				       (efi_time_cap_t *)arg2);
		break;
	case EFI_SET_TIME:
		status = efi_call_virt(set_time, (efi_time_t *)arg1);
		break;
	case EFI_GET_WAKEUP_TIME:
		status = efi_call_virt(get_wakeup_time, (efi_bool_t *)arg1,
				       (efi_bool_t *)arg2, (efi_time_t *)arg3);
		break;
	case EFI_SET_WAKEUP_TIME:
		status = efi_call_virt(set_wakeup_time, *(efi_bool_t *)arg1,
				       (efi_time_t *)arg2);
		break;
	case EFI_GET_VARIABLE:
>>>>>>> upstream/android-13
		status = efi_call_virt(get_variable, (efi_char16_t *)arg1,
				       (efi_guid_t *)arg2, (u32 *)arg3,
				       (unsigned long *)arg4, (void *)arg5);
		break;
<<<<<<< HEAD
	case GET_NEXT_VARIABLE:
=======
	case EFI_GET_NEXT_VARIABLE:
>>>>>>> upstream/android-13
		status = efi_call_virt(get_next_variable, (unsigned long *)arg1,
				       (efi_char16_t *)arg2,
				       (efi_guid_t *)arg3);
		break;
<<<<<<< HEAD
	case SET_VARIABLE:
=======
	case EFI_SET_VARIABLE:
>>>>>>> upstream/android-13
		status = efi_call_virt(set_variable, (efi_char16_t *)arg1,
				       (efi_guid_t *)arg2, *(u32 *)arg3,
				       *(unsigned long *)arg4, (void *)arg5);
		break;
<<<<<<< HEAD
	case QUERY_VARIABLE_INFO:
		status = efi_call_virt(query_variable_info, *(u32 *)arg1,
				       (u64 *)arg2, (u64 *)arg3, (u64 *)arg4);
		break;
	case GET_NEXT_HIGH_MONO_COUNT:
		status = efi_call_virt(get_next_high_mono_count, (u32 *)arg1);
		break;
	case UPDATE_CAPSULE:
=======
	case EFI_QUERY_VARIABLE_INFO:
		status = efi_call_virt(query_variable_info, *(u32 *)arg1,
				       (u64 *)arg2, (u64 *)arg3, (u64 *)arg4);
		break;
	case EFI_GET_NEXT_HIGH_MONO_COUNT:
		status = efi_call_virt(get_next_high_mono_count, (u32 *)arg1);
		break;
	case EFI_UPDATE_CAPSULE:
>>>>>>> upstream/android-13
		status = efi_call_virt(update_capsule,
				       (efi_capsule_header_t **)arg1,
				       *(unsigned long *)arg2,
				       *(unsigned long *)arg3);
		break;
<<<<<<< HEAD
	case QUERY_CAPSULE_CAPS:
=======
	case EFI_QUERY_CAPSULE_CAPS:
>>>>>>> upstream/android-13
		status = efi_call_virt(query_capsule_caps,
				       (efi_capsule_header_t **)arg1,
				       *(unsigned long *)arg2, (u64 *)arg3,
				       (int *)arg4);
		break;
	default:
		/*
		 * Ideally, we should never reach here because a caller of this
		 * function should have put the right efi_runtime_service()
		 * function identifier into efi_rts_work->efi_rts_id
		 */
		pr_err("Requested executing invalid EFI Runtime Service.\n");
	}
	efi_rts_work.status = status;
	complete(&efi_rts_work.efi_rts_comp);
}

static efi_status_t virt_efi_get_time(efi_time_t *tm, efi_time_cap_t *tc)
{
	efi_status_t status;

	if (down_interruptible(&efi_runtime_lock))
		return EFI_ABORTED;
<<<<<<< HEAD
	status = efi_queue_work(GET_TIME, tm, tc, NULL, NULL, NULL);
=======
	status = efi_queue_work(EFI_GET_TIME, tm, tc, NULL, NULL, NULL);
>>>>>>> upstream/android-13
	up(&efi_runtime_lock);
	return status;
}

static efi_status_t virt_efi_set_time(efi_time_t *tm)
{
	efi_status_t status;

	if (down_interruptible(&efi_runtime_lock))
		return EFI_ABORTED;
<<<<<<< HEAD
	status = efi_queue_work(SET_TIME, tm, NULL, NULL, NULL, NULL);
=======
	status = efi_queue_work(EFI_SET_TIME, tm, NULL, NULL, NULL, NULL);
>>>>>>> upstream/android-13
	up(&efi_runtime_lock);
	return status;
}

static efi_status_t virt_efi_get_wakeup_time(efi_bool_t *enabled,
					     efi_bool_t *pending,
					     efi_time_t *tm)
{
	efi_status_t status;

	if (down_interruptible(&efi_runtime_lock))
		return EFI_ABORTED;
<<<<<<< HEAD
	status = efi_queue_work(GET_WAKEUP_TIME, enabled, pending, tm, NULL,
=======
	status = efi_queue_work(EFI_GET_WAKEUP_TIME, enabled, pending, tm, NULL,
>>>>>>> upstream/android-13
				NULL);
	up(&efi_runtime_lock);
	return status;
}

static efi_status_t virt_efi_set_wakeup_time(efi_bool_t enabled, efi_time_t *tm)
{
	efi_status_t status;

	if (down_interruptible(&efi_runtime_lock))
		return EFI_ABORTED;
<<<<<<< HEAD
	status = efi_queue_work(SET_WAKEUP_TIME, &enabled, tm, NULL, NULL,
=======
	status = efi_queue_work(EFI_SET_WAKEUP_TIME, &enabled, tm, NULL, NULL,
>>>>>>> upstream/android-13
				NULL);
	up(&efi_runtime_lock);
	return status;
}

static efi_status_t virt_efi_get_variable(efi_char16_t *name,
					  efi_guid_t *vendor,
					  u32 *attr,
					  unsigned long *data_size,
					  void *data)
{
	efi_status_t status;

	if (down_interruptible(&efi_runtime_lock))
		return EFI_ABORTED;
<<<<<<< HEAD
	status = efi_queue_work(GET_VARIABLE, name, vendor, attr, data_size,
=======
	status = efi_queue_work(EFI_GET_VARIABLE, name, vendor, attr, data_size,
>>>>>>> upstream/android-13
				data);
	up(&efi_runtime_lock);
	return status;
}

static efi_status_t virt_efi_get_next_variable(unsigned long *name_size,
					       efi_char16_t *name,
					       efi_guid_t *vendor)
{
	efi_status_t status;

	if (down_interruptible(&efi_runtime_lock))
		return EFI_ABORTED;
<<<<<<< HEAD
	status = efi_queue_work(GET_NEXT_VARIABLE, name_size, name, vendor,
=======
	status = efi_queue_work(EFI_GET_NEXT_VARIABLE, name_size, name, vendor,
>>>>>>> upstream/android-13
				NULL, NULL);
	up(&efi_runtime_lock);
	return status;
}

static efi_status_t virt_efi_set_variable(efi_char16_t *name,
					  efi_guid_t *vendor,
					  u32 attr,
					  unsigned long data_size,
					  void *data)
{
	efi_status_t status;

	if (down_interruptible(&efi_runtime_lock))
		return EFI_ABORTED;
<<<<<<< HEAD
	status = efi_queue_work(SET_VARIABLE, name, vendor, &attr, &data_size,
=======
	status = efi_queue_work(EFI_SET_VARIABLE, name, vendor, &attr, &data_size,
>>>>>>> upstream/android-13
				data);
	up(&efi_runtime_lock);
	return status;
}

static efi_status_t
virt_efi_set_variable_nonblocking(efi_char16_t *name, efi_guid_t *vendor,
				  u32 attr, unsigned long data_size,
				  void *data)
{
	efi_status_t status;

	if (down_trylock(&efi_runtime_lock))
		return EFI_NOT_READY;

	status = efi_call_virt(set_variable, name, vendor, attr, data_size,
			       data);
	up(&efi_runtime_lock);
	return status;
}


static efi_status_t virt_efi_query_variable_info(u32 attr,
						 u64 *storage_space,
						 u64 *remaining_space,
						 u64 *max_variable_size)
{
	efi_status_t status;

	if (efi.runtime_version < EFI_2_00_SYSTEM_TABLE_REVISION)
		return EFI_UNSUPPORTED;

	if (down_interruptible(&efi_runtime_lock))
		return EFI_ABORTED;
<<<<<<< HEAD
	status = efi_queue_work(QUERY_VARIABLE_INFO, &attr, storage_space,
=======
	status = efi_queue_work(EFI_QUERY_VARIABLE_INFO, &attr, storage_space,
>>>>>>> upstream/android-13
				remaining_space, max_variable_size, NULL);
	up(&efi_runtime_lock);
	return status;
}

static efi_status_t
virt_efi_query_variable_info_nonblocking(u32 attr,
					 u64 *storage_space,
					 u64 *remaining_space,
					 u64 *max_variable_size)
{
	efi_status_t status;

	if (efi.runtime_version < EFI_2_00_SYSTEM_TABLE_REVISION)
		return EFI_UNSUPPORTED;

	if (down_trylock(&efi_runtime_lock))
		return EFI_NOT_READY;

	status = efi_call_virt(query_variable_info, attr, storage_space,
			       remaining_space, max_variable_size);
	up(&efi_runtime_lock);
	return status;
}

static efi_status_t virt_efi_get_next_high_mono_count(u32 *count)
{
	efi_status_t status;

	if (down_interruptible(&efi_runtime_lock))
		return EFI_ABORTED;
<<<<<<< HEAD
	status = efi_queue_work(GET_NEXT_HIGH_MONO_COUNT, count, NULL, NULL,
=======
	status = efi_queue_work(EFI_GET_NEXT_HIGH_MONO_COUNT, count, NULL, NULL,
>>>>>>> upstream/android-13
				NULL, NULL);
	up(&efi_runtime_lock);
	return status;
}

static void virt_efi_reset_system(int reset_type,
				  efi_status_t status,
				  unsigned long data_size,
				  efi_char16_t *data)
{
<<<<<<< HEAD
	if (down_interruptible(&efi_runtime_lock)) {
=======
	if (down_trylock(&efi_runtime_lock)) {
>>>>>>> upstream/android-13
		pr_warn("failed to invoke the reset_system() runtime service:\n"
			"could not get exclusive access to the firmware\n");
		return;
	}
<<<<<<< HEAD
=======
	efi_rts_work.efi_rts_id = EFI_RESET_SYSTEM;
>>>>>>> upstream/android-13
	__efi_call_virt(reset_system, reset_type, status, data_size, data);
	up(&efi_runtime_lock);
}

static efi_status_t virt_efi_update_capsule(efi_capsule_header_t **capsules,
					    unsigned long count,
					    unsigned long sg_list)
{
	efi_status_t status;

	if (efi.runtime_version < EFI_2_00_SYSTEM_TABLE_REVISION)
		return EFI_UNSUPPORTED;

	if (down_interruptible(&efi_runtime_lock))
		return EFI_ABORTED;
<<<<<<< HEAD
	status = efi_queue_work(UPDATE_CAPSULE, capsules, &count, &sg_list,
=======
	status = efi_queue_work(EFI_UPDATE_CAPSULE, capsules, &count, &sg_list,
>>>>>>> upstream/android-13
				NULL, NULL);
	up(&efi_runtime_lock);
	return status;
}

static efi_status_t virt_efi_query_capsule_caps(efi_capsule_header_t **capsules,
						unsigned long count,
						u64 *max_size,
						int *reset_type)
{
	efi_status_t status;

	if (efi.runtime_version < EFI_2_00_SYSTEM_TABLE_REVISION)
		return EFI_UNSUPPORTED;

	if (down_interruptible(&efi_runtime_lock))
		return EFI_ABORTED;
<<<<<<< HEAD
	status = efi_queue_work(QUERY_CAPSULE_CAPS, capsules, &count,
=======
	status = efi_queue_work(EFI_QUERY_CAPSULE_CAPS, capsules, &count,
>>>>>>> upstream/android-13
				max_size, reset_type, NULL);
	up(&efi_runtime_lock);
	return status;
}

void efi_native_runtime_setup(void)
{
	efi.get_time = virt_efi_get_time;
	efi.set_time = virt_efi_set_time;
	efi.get_wakeup_time = virt_efi_get_wakeup_time;
	efi.set_wakeup_time = virt_efi_set_wakeup_time;
	efi.get_variable = virt_efi_get_variable;
	efi.get_next_variable = virt_efi_get_next_variable;
	efi.set_variable = virt_efi_set_variable;
	efi.set_variable_nonblocking = virt_efi_set_variable_nonblocking;
	efi.get_next_high_mono_count = virt_efi_get_next_high_mono_count;
	efi.reset_system = virt_efi_reset_system;
	efi.query_variable_info = virt_efi_query_variable_info;
	efi.query_variable_info_nonblocking = virt_efi_query_variable_info_nonblocking;
	efi.update_capsule = virt_efi_update_capsule;
	efi.query_capsule_caps = virt_efi_query_capsule_caps;
}
