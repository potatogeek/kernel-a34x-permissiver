/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __UNWIND_H
#define __UNWIND_H

#include <linux/compiler.h>
#include <linux/types.h>
<<<<<<< HEAD

struct map;
struct perf_sample;
struct symbol;
struct thread;

struct unwind_entry {
	struct map	*map;
	struct symbol	*sym;
	u64		ip;
=======
#include "util/map_symbol.h"

struct maps;
struct perf_sample;
struct thread;

struct unwind_entry {
	struct map_symbol ms;
	u64		  ip;
>>>>>>> upstream/android-13
};

typedef int (*unwind_entry_cb_t)(struct unwind_entry *entry, void *arg);

struct unwind_libunwind_ops {
<<<<<<< HEAD
	int (*prepare_access)(struct thread *thread);
	void (*flush_access)(struct thread *thread);
	void (*finish_access)(struct thread *thread);
=======
	int (*prepare_access)(struct maps *maps);
	void (*flush_access)(struct maps *maps);
	void (*finish_access)(struct maps *maps);
>>>>>>> upstream/android-13
	int (*get_entries)(unwind_entry_cb_t cb, void *arg,
			   struct thread *thread,
			   struct perf_sample *data, int max_stack);
};

#ifdef HAVE_DWARF_UNWIND_SUPPORT
int unwind__get_entries(unwind_entry_cb_t cb, void *arg,
			struct thread *thread,
			struct perf_sample *data, int max_stack);
/* libunwind specific */
#ifdef HAVE_LIBUNWIND_SUPPORT
#ifndef LIBUNWIND__ARCH_REG_ID
#define LIBUNWIND__ARCH_REG_ID(regnum) libunwind__arch_reg_id(regnum)
#endif

#ifndef LIBUNWIND__ARCH_REG_SP
#define LIBUNWIND__ARCH_REG_SP PERF_REG_SP
#endif

#ifndef LIBUNWIND__ARCH_REG_IP
#define LIBUNWIND__ARCH_REG_IP PERF_REG_IP
#endif

int LIBUNWIND__ARCH_REG_ID(int regnum);
<<<<<<< HEAD
int unwind__prepare_access(struct thread *thread, struct map *map,
			   bool *initialized);
void unwind__flush_access(struct thread *thread);
void unwind__finish_access(struct thread *thread);
#else
static inline int unwind__prepare_access(struct thread *thread __maybe_unused,
=======
int unwind__prepare_access(struct maps *maps, struct map *map, bool *initialized);
void unwind__flush_access(struct maps *maps);
void unwind__finish_access(struct maps *maps);
#else
static inline int unwind__prepare_access(struct maps *maps __maybe_unused,
>>>>>>> upstream/android-13
					 struct map *map __maybe_unused,
					 bool *initialized __maybe_unused)
{
	return 0;
}

<<<<<<< HEAD
static inline void unwind__flush_access(struct thread *thread __maybe_unused) {}
static inline void unwind__finish_access(struct thread *thread __maybe_unused) {}
=======
static inline void unwind__flush_access(struct maps *maps __maybe_unused) {}
static inline void unwind__finish_access(struct maps *maps __maybe_unused) {}
>>>>>>> upstream/android-13
#endif
#else
static inline int
unwind__get_entries(unwind_entry_cb_t cb __maybe_unused,
		    void *arg __maybe_unused,
		    struct thread *thread __maybe_unused,
		    struct perf_sample *data __maybe_unused,
		    int max_stack __maybe_unused)
{
	return 0;
}

<<<<<<< HEAD
static inline int unwind__prepare_access(struct thread *thread __maybe_unused,
=======
static inline int unwind__prepare_access(struct maps *maps __maybe_unused,
>>>>>>> upstream/android-13
					 struct map *map __maybe_unused,
					 bool *initialized __maybe_unused)
{
	return 0;
}

<<<<<<< HEAD
static inline void unwind__flush_access(struct thread *thread __maybe_unused) {}
static inline void unwind__finish_access(struct thread *thread __maybe_unused) {}
=======
static inline void unwind__flush_access(struct maps *maps __maybe_unused) {}
static inline void unwind__finish_access(struct maps *maps __maybe_unused) {}
>>>>>>> upstream/android-13
#endif /* HAVE_DWARF_UNWIND_SUPPORT */
#endif /* __UNWIND_H */
