/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_DEBUGOBJECTS_H
#define _LINUX_DEBUGOBJECTS_H

#include <linux/list.h>
#include <linux/spinlock.h>

enum debug_obj_state {
	ODEBUG_STATE_NONE,
	ODEBUG_STATE_INIT,
	ODEBUG_STATE_INACTIVE,
	ODEBUG_STATE_ACTIVE,
	ODEBUG_STATE_DESTROYED,
	ODEBUG_STATE_NOTAVAILABLE,
	ODEBUG_STATE_MAX,
};

struct debug_obj_descr;


struct debug_obj {
	struct hlist_node	node;
	enum debug_obj_state	state;
	unsigned int		astate;
	void			*object;
	struct debug_obj_descr	*descr;
};


struct debug_obj_descr {
	const char		*name;
	void *(*debug_hint)(void *addr);
	bool (*is_static_object)(void *addr);
	bool (*fixup_init)(void *addr, enum debug_obj_state state);
	bool (*fixup_activate)(void *addr, enum debug_obj_state state);
	bool (*fixup_destroy)(void *addr, enum debug_obj_state state);
	bool (*fixup_free)(void *addr, enum debug_obj_state state);
	bool (*fixup_assert_init)(void *addr, enum debug_obj_state state);
};

#ifdef CONFIG_DEBUG_OBJECTS
extern void debug_object_init      (void *addr, struct debug_obj_descr *descr);
extern void
debug_object_init_on_stack(void *addr, struct debug_obj_descr *descr);
extern int debug_object_activate  (void *addr, struct debug_obj_descr *descr);
extern void debug_object_deactivate(void *addr, struct debug_obj_descr *descr);
extern void debug_object_destroy   (void *addr, struct debug_obj_descr *descr);
extern void debug_object_free      (void *addr, struct debug_obj_descr *descr);
extern void debug_object_assert_init(void *addr, struct debug_obj_descr *descr);


extern void
debug_object_active_state(void *addr, struct debug_obj_descr *descr,
			  unsigned int expect, unsigned int next);

extern void debug_objects_early_init(void);
extern void debug_objects_mem_init(void);
#else
static inline void
debug_object_init      (void *addr, struct debug_obj_descr *descr) { }
static inline void
debug_object_init_on_stack(void *addr, struct debug_obj_descr *descr) { }
static inline int
debug_object_activate  (void *addr, struct debug_obj_descr *descr) { return 0; }
static inline void
debug_object_deactivate(void *addr, struct debug_obj_descr *descr) { }
static inline void
debug_object_destroy   (void *addr, struct debug_obj_descr *descr) { }
static inline void
debug_object_free      (void *addr, struct debug_obj_descr *descr) { }
static inline void
debug_object_assert_init(void *addr, struct debug_obj_descr *descr) { }

static inline void debug_objects_early_init(void) { }
static inline void debug_objects_mem_init(void) { }
#endif

#ifdef CONFIG_DEBUG_OBJECTS_FREE
extern void debug_check_no_obj_freed(const void *address, unsigned long size);
#else
static inline void
debug_check_no_obj_freed(const void *address, unsigned long size) { }
#endif

#endif
