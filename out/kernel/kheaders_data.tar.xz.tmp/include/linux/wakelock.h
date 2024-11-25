

#ifndef _LINUX_WAKELOCK_H
#define _LINUX_WAKELOCK_H

#include <linux/ktime.h>
#include <linux/device.h>
#include <linux/pm_wakeup.h>



enum {
	WAKE_LOCK_SUSPEND, 
	WAKE_LOCK_TYPE_COUNT
};

struct wake_lock {
#ifdef CONFIG_PM_WAKELOCKS
	struct wakeup_source *ptr_ws;
#else
	struct wakeup_source ws;
#endif
};

static inline void wake_lock_init(struct wake_lock *lock, int type,
				  const char *name)
{
#ifdef CONFIG_PM_WAKELOCKS
	lock->ptr_ws = wakeup_source_register(NULL, "muic_wake");
#else
	wakeup_source_init(&lock->ws, name);
#endif
}

static inline void wake_lock_destroy(struct wake_lock *lock)
{
#ifdef CONFIG_PM_WAKELOCKS
	wakeup_source_unregister(lock->ptr_ws);
#else
	wakeup_source_trash(&lock->ws);
#endif
}

static inline void wake_lock(struct wake_lock *lock)
{
#ifdef CONFIG_PM_WAKELOCKS
	__pm_stay_awake(lock->ptr_ws);
#else
	__pm_stay_awake(&lock->ws);
#endif
}

static inline void wake_lock_timeout(struct wake_lock *lock, long timeout)
{
#ifdef CONFIG_PM_WAKELOCKS
	__pm_wakeup_event(lock->ptr_ws, jiffies_to_msecs(timeout));
#else
	__pm_wakeup_event(&lock->ws, jiffies_to_msecs(timeout));
#endif
}

static inline void wake_unlock(struct wake_lock *lock)
{
#ifdef CONFIG_PM_WAKELOCKS
	__pm_relax(lock->ptr_ws);
#else
	__pm_relax(&lock->ws);
#endif
}

static inline int wake_lock_active(struct wake_lock *lock)
{
#ifdef CONFIG_PM_WAKELOCKS
	return lock->ptr_ws->active;
#else
	return lock->ws.active;
#endif
}

#endif
