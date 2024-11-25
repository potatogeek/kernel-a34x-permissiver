// SPDX-License-Identifier: GPL-2.0
#include "unwind.h"
<<<<<<< HEAD
=======
#include "dso.h"
#include "map.h"
>>>>>>> upstream/android-13
#include "thread.h"
#include "session.h"
#include "debug.h"
#include "env.h"
<<<<<<< HEAD
=======
#include "callchain.h"
>>>>>>> upstream/android-13

struct unwind_libunwind_ops __weak *local_unwind_libunwind_ops;
struct unwind_libunwind_ops __weak *x86_32_unwind_libunwind_ops;
struct unwind_libunwind_ops __weak *arm64_unwind_libunwind_ops;

<<<<<<< HEAD
static void unwind__register_ops(struct thread *thread,
			  struct unwind_libunwind_ops *ops)
{
	thread->unwind_libunwind_ops = ops;
}

int unwind__prepare_access(struct thread *thread, struct map *map,
			   bool *initialized)
=======
static void unwind__register_ops(struct maps *maps, struct unwind_libunwind_ops *ops)
{
	maps->unwind_libunwind_ops = ops;
}

int unwind__prepare_access(struct maps *maps, struct map *map, bool *initialized)
>>>>>>> upstream/android-13
{
	const char *arch;
	enum dso_type dso_type;
	struct unwind_libunwind_ops *ops = local_unwind_libunwind_ops;
	int err;

<<<<<<< HEAD
	if (thread->addr_space) {
=======
	if (!dwarf_callchain_users)
		return 0;

	if (maps->addr_space) {
>>>>>>> upstream/android-13
		pr_debug("unwind: thread map already set, dso=%s\n",
			 map->dso->name);
		if (initialized)
			*initialized = true;
		return 0;
	}

	/* env->arch is NULL for live-mode (i.e. perf top) */
<<<<<<< HEAD
	if (!thread->mg->machine->env || !thread->mg->machine->env->arch)
		goto out_register;

	dso_type = dso__type(map->dso, thread->mg->machine);
	if (dso_type == DSO__TYPE_UNKNOWN)
		return 0;

	arch = perf_env__arch(thread->mg->machine->env);
=======
	if (!maps->machine->env || !maps->machine->env->arch)
		goto out_register;

	dso_type = dso__type(map->dso, maps->machine);
	if (dso_type == DSO__TYPE_UNKNOWN)
		return 0;

	arch = perf_env__arch(maps->machine->env);
>>>>>>> upstream/android-13

	if (!strcmp(arch, "x86")) {
		if (dso_type != DSO__TYPE_64BIT)
			ops = x86_32_unwind_libunwind_ops;
	} else if (!strcmp(arch, "arm64") || !strcmp(arch, "arm")) {
		if (dso_type == DSO__TYPE_64BIT)
			ops = arm64_unwind_libunwind_ops;
	}

	if (!ops) {
		pr_err("unwind: target platform=%s is not supported\n", arch);
		return 0;
	}
out_register:
<<<<<<< HEAD
	unwind__register_ops(thread, ops);

	err = thread->unwind_libunwind_ops->prepare_access(thread);
=======
	unwind__register_ops(maps, ops);

	err = maps->unwind_libunwind_ops->prepare_access(maps);
>>>>>>> upstream/android-13
	if (initialized)
		*initialized = err ? false : true;
	return err;
}

<<<<<<< HEAD
void unwind__flush_access(struct thread *thread)
{
	if (thread->unwind_libunwind_ops)
		thread->unwind_libunwind_ops->flush_access(thread);
}

void unwind__finish_access(struct thread *thread)
{
	if (thread->unwind_libunwind_ops)
		thread->unwind_libunwind_ops->finish_access(thread);
=======
void unwind__flush_access(struct maps *maps)
{
	if (maps->unwind_libunwind_ops)
		maps->unwind_libunwind_ops->flush_access(maps);
}

void unwind__finish_access(struct maps *maps)
{
	if (maps->unwind_libunwind_ops)
		maps->unwind_libunwind_ops->finish_access(maps);
>>>>>>> upstream/android-13
}

int unwind__get_entries(unwind_entry_cb_t cb, void *arg,
			 struct thread *thread,
			 struct perf_sample *data, int max_stack)
{
<<<<<<< HEAD
	if (thread->unwind_libunwind_ops)
		return thread->unwind_libunwind_ops->get_entries(cb, arg, thread, data, max_stack);
=======
	if (thread->maps->unwind_libunwind_ops)
		return thread->maps->unwind_libunwind_ops->get_entries(cb, arg, thread, data, max_stack);
>>>>>>> upstream/android-13
	return 0;
}
