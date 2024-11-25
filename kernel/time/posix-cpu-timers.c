// SPDX-License-Identifier: GPL-2.0
/*
 * Implement CPU time clocks for the POSIX clock interface.
 */

#include <linux/sched/signal.h>
#include <linux/sched/cputime.h>
#include <linux/posix-timers.h>
#include <linux/errno.h>
#include <linux/math64.h>
#include <linux/uaccess.h>
#include <linux/kernel_stat.h>
#include <trace/events/timer.h>
#include <linux/tick.h>
#include <linux/workqueue.h>
#include <linux/compat.h>
#include <linux/sched/deadline.h>

#include "posix-timers.h"

static void posix_cpu_timer_rearm(struct k_itimer *timer);

<<<<<<< HEAD
/*
 * Called after updating RLIMIT_CPU to run cpu timer and update
 * tsk->signal->cputime_expires expiration cache if necessary. Needs
 * siglock protection since other code may update expiration cache as
 * well.
=======
void posix_cputimers_group_init(struct posix_cputimers *pct, u64 cpu_limit)
{
	posix_cputimers_init(pct);
	if (cpu_limit != RLIM_INFINITY) {
		pct->bases[CPUCLOCK_PROF].nextevt = cpu_limit * NSEC_PER_SEC;
		pct->timers_active = true;
	}
}

/*
 * Called after updating RLIMIT_CPU to run cpu timer and update
 * tsk->signal->posix_cputimers.bases[clock].nextevt expiration cache if
 * necessary. Needs siglock protection since other code may update the
 * expiration cache as well.
>>>>>>> upstream/android-13
 */
void update_rlimit_cpu(struct task_struct *task, unsigned long rlim_new)
{
	u64 nsecs = rlim_new * NSEC_PER_SEC;

	spin_lock_irq(&task->sighand->siglock);
	set_process_cpu_timer(task, CPUCLOCK_PROF, &nsecs, NULL);
	spin_unlock_irq(&task->sighand->siglock);
}

<<<<<<< HEAD
static int check_clock(const clockid_t which_clock)
{
	int error = 0;
	struct task_struct *p;
	const pid_t pid = CPUCLOCK_PID(which_clock);

	if (CPUCLOCK_WHICH(which_clock) >= CPUCLOCK_MAX)
		return -EINVAL;

	if (pid == 0)
		return 0;

	rcu_read_lock();
	p = find_task_by_vpid(pid);
	if (!p || !(CPUCLOCK_PERTHREAD(which_clock) ?
		   same_thread_group(p, current) : has_group_leader_pid(p))) {
		error = -EINVAL;
	}
	rcu_read_unlock();

	return error;
=======
/*
 * Functions for validating access to tasks.
 */
static struct pid *pid_for_clock(const clockid_t clock, bool gettime)
{
	const bool thread = !!CPUCLOCK_PERTHREAD(clock);
	const pid_t upid = CPUCLOCK_PID(clock);
	struct pid *pid;

	if (CPUCLOCK_WHICH(clock) >= CPUCLOCK_MAX)
		return NULL;

	/*
	 * If the encoded PID is 0, then the timer is targeted at current
	 * or the process to which current belongs.
	 */
	if (upid == 0)
		return thread ? task_pid(current) : task_tgid(current);

	pid = find_vpid(upid);
	if (!pid)
		return NULL;

	if (thread) {
		struct task_struct *tsk = pid_task(pid, PIDTYPE_PID);
		return (tsk && same_thread_group(tsk, current)) ? pid : NULL;
	}

	/*
	 * For clock_gettime(PROCESS) allow finding the process by
	 * with the pid of the current task.  The code needs the tgid
	 * of the process so that pid_task(pid, PIDTYPE_TGID) can be
	 * used to find the process.
	 */
	if (gettime && (pid == task_pid(current)))
		return task_tgid(current);

	/*
	 * For processes require that pid identifies a process.
	 */
	return pid_has_task(pid, PIDTYPE_TGID) ? pid : NULL;
}

static inline int validate_clock_permissions(const clockid_t clock)
{
	int ret;

	rcu_read_lock();
	ret = pid_for_clock(clock, false) ? 0 : -EINVAL;
	rcu_read_unlock();

	return ret;
}

static inline enum pid_type clock_pid_type(const clockid_t clock)
{
	return CPUCLOCK_PERTHREAD(clock) ? PIDTYPE_PID : PIDTYPE_TGID;
}

static inline struct task_struct *cpu_timer_task_rcu(struct k_itimer *timer)
{
	return pid_task(timer->it.cpu.pid, clock_pid_type(timer->it_clock));
>>>>>>> upstream/android-13
}

/*
 * Update expiry time from increment, and increase overrun count,
 * given the current clock sample.
 */
<<<<<<< HEAD
static void bump_cpu_timer(struct k_itimer *timer, u64 now)
{
	int i;
	u64 delta, incr;

	if (timer->it.cpu.incr == 0)
		return;

	if (now < timer->it.cpu.expires)
		return;

	incr = timer->it.cpu.incr;
	delta = now + incr - timer->it.cpu.expires;
=======
static u64 bump_cpu_timer(struct k_itimer *timer, u64 now)
{
	u64 delta, incr, expires = timer->it.cpu.node.expires;
	int i;

	if (!timer->it_interval)
		return expires;

	if (now < expires)
		return expires;

	incr = timer->it_interval;
	delta = now + incr - expires;
>>>>>>> upstream/android-13

	/* Don't use (incr*2 < delta), incr*2 might overflow. */
	for (i = 0; incr < delta - incr; i++)
		incr = incr << 1;

	for (; i >= 0; incr >>= 1, i--) {
		if (delta < incr)
			continue;

<<<<<<< HEAD
		timer->it.cpu.expires += incr;
		timer->it_overrun += 1LL << i;
		delta -= incr;
	}
}

/**
 * task_cputime_zero - Check a task_cputime struct for all zero fields.
 *
 * @cputime:	The struct to compare.
 *
 * Checks @cputime to see if all fields are zero.  Returns true if all fields
 * are zero, false if any field is nonzero.
 */
static inline int task_cputime_zero(const struct task_cputime *cputime)
{
	if (!cputime->utime && !cputime->stime && !cputime->sum_exec_runtime)
		return 1;
	return 0;
}

static inline u64 prof_ticks(struct task_struct *p)
{
	u64 utime, stime;

	task_cputime(p, &utime, &stime);

	return utime + stime;
}
static inline u64 virt_ticks(struct task_struct *p)
{
	u64 utime, stime;

	task_cputime(p, &utime, &stime);

	return utime;
=======
		timer->it.cpu.node.expires += incr;
		timer->it_overrun += 1LL << i;
		delta -= incr;
	}
	return timer->it.cpu.node.expires;
}

/* Check whether all cache entries contain U64_MAX, i.e. eternal expiry time */
static inline bool expiry_cache_is_inactive(const struct posix_cputimers *pct)
{
	return !(~pct->bases[CPUCLOCK_PROF].nextevt |
		 ~pct->bases[CPUCLOCK_VIRT].nextevt |
		 ~pct->bases[CPUCLOCK_SCHED].nextevt);
>>>>>>> upstream/android-13
}

static int
posix_cpu_clock_getres(const clockid_t which_clock, struct timespec64 *tp)
{
<<<<<<< HEAD
	int error = check_clock(which_clock);
=======
	int error = validate_clock_permissions(which_clock);

>>>>>>> upstream/android-13
	if (!error) {
		tp->tv_sec = 0;
		tp->tv_nsec = ((NSEC_PER_SEC + HZ - 1) / HZ);
		if (CPUCLOCK_WHICH(which_clock) == CPUCLOCK_SCHED) {
			/*
			 * If sched_clock is using a cycle counter, we
			 * don't have any idea of its true resolution
			 * exported, but it is much more than 1s/HZ.
			 */
			tp->tv_nsec = 1;
		}
	}
	return error;
}

static int
<<<<<<< HEAD
posix_cpu_clock_set(const clockid_t which_clock, const struct timespec64 *tp)
{
=======
posix_cpu_clock_set(const clockid_t clock, const struct timespec64 *tp)
{
	int error = validate_clock_permissions(clock);

>>>>>>> upstream/android-13
	/*
	 * You can never reset a CPU clock, but we check for other errors
	 * in the call before failing with EPERM.
	 */
<<<<<<< HEAD
	int error = check_clock(which_clock);
	if (error == 0) {
		error = -EPERM;
	}
	return error;
}


/*
 * Sample a per-thread clock for the given task.
 */
static int cpu_clock_sample(const clockid_t which_clock,
			    struct task_struct *p, u64 *sample)
{
	switch (CPUCLOCK_WHICH(which_clock)) {
	default:
		return -EINVAL;
	case CPUCLOCK_PROF:
		*sample = prof_ticks(p);
		break;
	case CPUCLOCK_VIRT:
		*sample = virt_ticks(p);
		break;
	case CPUCLOCK_SCHED:
		*sample = task_sched_runtime(p);
		break;
=======
	return error ? : -EPERM;
}

/*
 * Sample a per-thread clock for the given task. clkid is validated.
 */
static u64 cpu_clock_sample(const clockid_t clkid, struct task_struct *p)
{
	u64 utime, stime;

	if (clkid == CPUCLOCK_SCHED)
		return task_sched_runtime(p);

	task_cputime(p, &utime, &stime);

	switch (clkid) {
	case CPUCLOCK_PROF:
		return utime + stime;
	case CPUCLOCK_VIRT:
		return utime;
	default:
		WARN_ON_ONCE(1);
>>>>>>> upstream/android-13
	}
	return 0;
}

<<<<<<< HEAD
=======
static inline void store_samples(u64 *samples, u64 stime, u64 utime, u64 rtime)
{
	samples[CPUCLOCK_PROF] = stime + utime;
	samples[CPUCLOCK_VIRT] = utime;
	samples[CPUCLOCK_SCHED] = rtime;
}

static void task_sample_cputime(struct task_struct *p, u64 *samples)
{
	u64 stime, utime;

	task_cputime(p, &utime, &stime);
	store_samples(samples, stime, utime, p->se.sum_exec_runtime);
}

static void proc_sample_cputime_atomic(struct task_cputime_atomic *at,
				       u64 *samples)
{
	u64 stime, utime, rtime;

	utime = atomic64_read(&at->utime);
	stime = atomic64_read(&at->stime);
	rtime = atomic64_read(&at->sum_exec_runtime);
	store_samples(samples, stime, utime, rtime);
}

>>>>>>> upstream/android-13
/*
 * Set cputime to sum_cputime if sum_cputime > cputime. Use cmpxchg
 * to avoid race conditions with concurrent updates to cputime.
 */
static inline void __update_gt_cputime(atomic64_t *cputime, u64 sum_cputime)
{
	u64 curr_cputime;
retry:
	curr_cputime = atomic64_read(cputime);
	if (sum_cputime > curr_cputime) {
		if (atomic64_cmpxchg(cputime, curr_cputime, sum_cputime) != curr_cputime)
			goto retry;
	}
}

<<<<<<< HEAD
static void update_gt_cputime(struct task_cputime_atomic *cputime_atomic, struct task_cputime *sum)
=======
static void update_gt_cputime(struct task_cputime_atomic *cputime_atomic,
			      struct task_cputime *sum)
>>>>>>> upstream/android-13
{
	__update_gt_cputime(&cputime_atomic->utime, sum->utime);
	__update_gt_cputime(&cputime_atomic->stime, sum->stime);
	__update_gt_cputime(&cputime_atomic->sum_exec_runtime, sum->sum_exec_runtime);
}

<<<<<<< HEAD
/* Sample task_cputime_atomic values in "atomic_timers", store results in "times". */
static inline void sample_cputime_atomic(struct task_cputime *times,
					 struct task_cputime_atomic *atomic_times)
{
	times->utime = atomic64_read(&atomic_times->utime);
	times->stime = atomic64_read(&atomic_times->stime);
	times->sum_exec_runtime = atomic64_read(&atomic_times->sum_exec_runtime);
}

void thread_group_cputimer(struct task_struct *tsk, struct task_cputime *times)
{
	struct thread_group_cputimer *cputimer = &tsk->signal->cputimer;
	struct task_cputime sum;

	/* Check if cputimer isn't running. This is accessed without locking. */
	if (!READ_ONCE(cputimer->running)) {
=======
/**
 * thread_group_sample_cputime - Sample cputime for a given task
 * @tsk:	Task for which cputime needs to be started
 * @samples:	Storage for time samples
 *
 * Called from sys_getitimer() to calculate the expiry time of an active
 * timer. That means group cputime accounting is already active. Called
 * with task sighand lock held.
 *
 * Updates @times with an uptodate sample of the thread group cputimes.
 */
void thread_group_sample_cputime(struct task_struct *tsk, u64 *samples)
{
	struct thread_group_cputimer *cputimer = &tsk->signal->cputimer;
	struct posix_cputimers *pct = &tsk->signal->posix_cputimers;

	WARN_ON_ONCE(!pct->timers_active);

	proc_sample_cputime_atomic(&cputimer->cputime_atomic, samples);
}

/**
 * thread_group_start_cputime - Start cputime and return a sample
 * @tsk:	Task for which cputime needs to be started
 * @samples:	Storage for time samples
 *
 * The thread group cputime accounting is avoided when there are no posix
 * CPU timers armed. Before starting a timer it's required to check whether
 * the time accounting is active. If not, a full update of the atomic
 * accounting store needs to be done and the accounting enabled.
 *
 * Updates @times with an uptodate sample of the thread group cputimes.
 */
static void thread_group_start_cputime(struct task_struct *tsk, u64 *samples)
{
	struct thread_group_cputimer *cputimer = &tsk->signal->cputimer;
	struct posix_cputimers *pct = &tsk->signal->posix_cputimers;

	lockdep_assert_task_sighand_held(tsk);

	/* Check if cputimer isn't running. This is accessed without locking. */
	if (!READ_ONCE(pct->timers_active)) {
		struct task_cputime sum;

>>>>>>> upstream/android-13
		/*
		 * The POSIX timer interface allows for absolute time expiry
		 * values through the TIMER_ABSTIME flag, therefore we have
		 * to synchronize the timer to the clock every time we start it.
		 */
		thread_group_cputime(tsk, &sum);
		update_gt_cputime(&cputimer->cputime_atomic, &sum);

		/*
<<<<<<< HEAD
		 * We're setting cputimer->running without a lock. Ensure
		 * this only gets written to in one operation. We set
		 * running after update_gt_cputime() as a small optimization,
		 * but barriers are not required because update_gt_cputime()
		 * can handle concurrent updates.
		 */
		WRITE_ONCE(cputimer->running, true);
	}
	sample_cputime_atomic(times, &cputimer->cputime_atomic);
}

/*
 * Sample a process (thread group) clock for the given group_leader task.
 * Must be called with task sighand lock held for safe while_each_thread()
 * traversal.
 */
static int cpu_clock_sample_group(const clockid_t which_clock,
				  struct task_struct *p,
				  u64 *sample)
{
	struct task_cputime cputime;

	switch (CPUCLOCK_WHICH(which_clock)) {
	default:
		return -EINVAL;
	case CPUCLOCK_PROF:
		thread_group_cputime(p, &cputime);
		*sample = cputime.utime + cputime.stime;
		break;
	case CPUCLOCK_VIRT:
		thread_group_cputime(p, &cputime);
		*sample = cputime.utime;
		break;
	case CPUCLOCK_SCHED:
		thread_group_cputime(p, &cputime);
		*sample = cputime.sum_exec_runtime;
		break;
	}
	return 0;
}

static int posix_cpu_clock_get_task(struct task_struct *tsk,
				    const clockid_t which_clock,
				    struct timespec64 *tp)
{
	int err = -EINVAL;
	u64 rtn;

	if (CPUCLOCK_PERTHREAD(which_clock)) {
		if (same_thread_group(tsk, current))
			err = cpu_clock_sample(which_clock, tsk, &rtn);
	} else {
		if (tsk == current || thread_group_leader(tsk))
			err = cpu_clock_sample_group(which_clock, tsk, &rtn);
	}

	if (!err)
		*tp = ns_to_timespec64(rtn);

	return err;
}


static int posix_cpu_clock_get(const clockid_t which_clock, struct timespec64 *tp)
{
	const pid_t pid = CPUCLOCK_PID(which_clock);
	int err = -EINVAL;

	if (pid == 0) {
		/*
		 * Special case constant value for our own clocks.
		 * We don't have to do any lookup to find ourselves.
		 */
		err = posix_cpu_clock_get_task(current, which_clock, tp);
	} else {
		/*
		 * Find the given PID, and validate that the caller
		 * should be able to see it.
		 */
		struct task_struct *p;
		rcu_read_lock();
		p = find_task_by_vpid(pid);
		if (p)
			err = posix_cpu_clock_get_task(p, which_clock, tp);
		rcu_read_unlock();
	}

	return err;
=======
		 * We're setting timers_active without a lock. Ensure this
		 * only gets written to in one operation. We set it after
		 * update_gt_cputime() as a small optimization, but
		 * barriers are not required because update_gt_cputime()
		 * can handle concurrent updates.
		 */
		WRITE_ONCE(pct->timers_active, true);
	}
	proc_sample_cputime_atomic(&cputimer->cputime_atomic, samples);
}

static void __thread_group_cputime(struct task_struct *tsk, u64 *samples)
{
	struct task_cputime ct;

	thread_group_cputime(tsk, &ct);
	store_samples(samples, ct.stime, ct.utime, ct.sum_exec_runtime);
}

/*
 * Sample a process (thread group) clock for the given task clkid. If the
 * group's cputime accounting is already enabled, read the atomic
 * store. Otherwise a full update is required.  clkid is already validated.
 */
static u64 cpu_clock_sample_group(const clockid_t clkid, struct task_struct *p,
				  bool start)
{
	struct thread_group_cputimer *cputimer = &p->signal->cputimer;
	struct posix_cputimers *pct = &p->signal->posix_cputimers;
	u64 samples[CPUCLOCK_MAX];

	if (!READ_ONCE(pct->timers_active)) {
		if (start)
			thread_group_start_cputime(p, samples);
		else
			__thread_group_cputime(p, samples);
	} else {
		proc_sample_cputime_atomic(&cputimer->cputime_atomic, samples);
	}

	return samples[clkid];
}

static int posix_cpu_clock_get(const clockid_t clock, struct timespec64 *tp)
{
	const clockid_t clkid = CPUCLOCK_WHICH(clock);
	struct task_struct *tsk;
	u64 t;

	rcu_read_lock();
	tsk = pid_task(pid_for_clock(clock, true), clock_pid_type(clock));
	if (!tsk) {
		rcu_read_unlock();
		return -EINVAL;
	}

	if (CPUCLOCK_PERTHREAD(clock))
		t = cpu_clock_sample(clkid, tsk);
	else
		t = cpu_clock_sample_group(clkid, tsk, false);
	rcu_read_unlock();

	*tp = ns_to_timespec64(t);
	return 0;
>>>>>>> upstream/android-13
}

/*
 * Validate the clockid_t for a new CPU-clock timer, and initialize the timer.
 * This is called from sys_timer_create() and do_cpu_nanosleep() with the
 * new timer already all-zeros initialized.
 */
static int posix_cpu_timer_create(struct k_itimer *new_timer)
{
<<<<<<< HEAD
	int ret = 0;
	const pid_t pid = CPUCLOCK_PID(new_timer->it_clock);
	struct task_struct *p;

	if (CPUCLOCK_WHICH(new_timer->it_clock) >= CPUCLOCK_MAX)
		return -EINVAL;

	new_timer->kclock = &clock_posix_cpu;

	INIT_LIST_HEAD(&new_timer->it.cpu.entry);

	rcu_read_lock();
	if (CPUCLOCK_PERTHREAD(new_timer->it_clock)) {
		if (pid == 0) {
			p = current;
		} else {
			p = find_task_by_vpid(pid);
			if (p && !same_thread_group(p, current))
				p = NULL;
		}
	} else {
		if (pid == 0) {
			p = current->group_leader;
		} else {
			p = find_task_by_vpid(pid);
			if (p && !has_group_leader_pid(p))
				p = NULL;
		}
	}
	new_timer->it.cpu.task = p;
	if (p) {
		get_task_struct(p);
	} else {
		ret = -EINVAL;
	}
	rcu_read_unlock();

	return ret;
}

=======
	static struct lock_class_key posix_cpu_timers_key;
	struct pid *pid;

	rcu_read_lock();
	pid = pid_for_clock(new_timer->it_clock, false);
	if (!pid) {
		rcu_read_unlock();
		return -EINVAL;
	}

	/*
	 * If posix timer expiry is handled in task work context then
	 * timer::it_lock can be taken without disabling interrupts as all
	 * other locking happens in task context. This requires a separate
	 * lock class key otherwise regular posix timer expiry would record
	 * the lock class being taken in interrupt context and generate a
	 * false positive warning.
	 */
	if (IS_ENABLED(CONFIG_POSIX_CPU_TIMERS_TASK_WORK))
		lockdep_set_class(&new_timer->it_lock, &posix_cpu_timers_key);

	new_timer->kclock = &clock_posix_cpu;
	timerqueue_init(&new_timer->it.cpu.node);
	new_timer->it.cpu.pid = get_pid(pid);
	rcu_read_unlock();
	return 0;
}

static struct posix_cputimer_base *timer_base(struct k_itimer *timer,
					      struct task_struct *tsk)
{
	int clkidx = CPUCLOCK_WHICH(timer->it_clock);

	if (CPUCLOCK_PERTHREAD(timer->it_clock))
		return tsk->posix_cputimers.bases + clkidx;
	else
		return tsk->signal->posix_cputimers.bases + clkidx;
}

/*
 * Force recalculating the base earliest expiration on the next tick.
 * This will also re-evaluate the need to keep around the process wide
 * cputime counter and tick dependency and eventually shut these down
 * if necessary.
 */
static void trigger_base_recalc_expires(struct k_itimer *timer,
					struct task_struct *tsk)
{
	struct posix_cputimer_base *base = timer_base(timer, tsk);

	base->nextevt = 0;
}

/*
 * Dequeue the timer and reset the base if it was its earliest expiration.
 * It makes sure the next tick recalculates the base next expiration so we
 * don't keep the costly process wide cputime counter around for a random
 * amount of time, along with the tick dependency.
 *
 * If another timer gets queued between this and the next tick, its
 * expiration will update the base next event if necessary on the next
 * tick.
 */
static void disarm_timer(struct k_itimer *timer, struct task_struct *p)
{
	struct cpu_timer *ctmr = &timer->it.cpu;
	struct posix_cputimer_base *base;

	if (!cpu_timer_dequeue(ctmr))
		return;

	base = timer_base(timer, p);
	if (cpu_timer_getexpires(ctmr) == base->nextevt)
		trigger_base_recalc_expires(timer, p);
}


>>>>>>> upstream/android-13
/*
 * Clean up a CPU-clock timer that is about to be destroyed.
 * This is called from timer deletion with the timer already locked.
 * If we return TIMER_RETRY, it's necessary to release the timer's lock
 * and try again.  (This happens when the timer is in the middle of firing.)
 */
static int posix_cpu_timer_del(struct k_itimer *timer)
{
<<<<<<< HEAD
	int ret = 0;
	unsigned long flags;
	struct sighand_struct *sighand;
	struct task_struct *p = timer->it.cpu.task;

	if (WARN_ON_ONCE(!p))
		return -EINVAL;
=======
	struct cpu_timer *ctmr = &timer->it.cpu;
	struct sighand_struct *sighand;
	struct task_struct *p;
	unsigned long flags;
	int ret = 0;

	rcu_read_lock();
	p = cpu_timer_task_rcu(timer);
	if (!p)
		goto out;
>>>>>>> upstream/android-13

	/*
	 * Protect against sighand release/switch in exit/exec and process/
	 * thread timer list entry concurrent read/writes.
	 */
	sighand = lock_task_sighand(p, &flags);
	if (unlikely(sighand == NULL)) {
		/*
<<<<<<< HEAD
		 * We raced with the reaping of the task.
		 * The deletion should have cleared us off the list.
		 */
		WARN_ON_ONCE(!list_empty(&timer->it.cpu.entry));
=======
		 * This raced with the reaping of the task. The exit cleanup
		 * should have removed this timer from the timer queue.
		 */
		WARN_ON_ONCE(ctmr->head || timerqueue_node_queued(&ctmr->node));
>>>>>>> upstream/android-13
	} else {
		if (timer->it.cpu.firing)
			ret = TIMER_RETRY;
		else
<<<<<<< HEAD
			list_del(&timer->it.cpu.entry);
=======
			disarm_timer(timer, p);
>>>>>>> upstream/android-13

		unlock_task_sighand(p, &flags);
	}

<<<<<<< HEAD
	if (!ret)
		put_task_struct(p);
=======
out:
	rcu_read_unlock();
	if (!ret)
		put_pid(ctmr->pid);
>>>>>>> upstream/android-13

	return ret;
}

<<<<<<< HEAD
static void cleanup_timers_list(struct list_head *head)
{
	struct cpu_timer_list *timer, *next;

	list_for_each_entry_safe(timer, next, head, entry)
		list_del_init(&timer->entry);
}

/*
 * Clean out CPU timers still ticking when a thread exited.  The task
 * pointer is cleared, and the expiry time is replaced with the residual
 * time for later timer_gettime calls to return.
 * This must be called with the siglock held.
 */
static void cleanup_timers(struct list_head *head)
{
	cleanup_timers_list(head);
	cleanup_timers_list(++head);
	cleanup_timers_list(++head);
=======
static void cleanup_timerqueue(struct timerqueue_head *head)
{
	struct timerqueue_node *node;
	struct cpu_timer *ctmr;

	while ((node = timerqueue_getnext(head))) {
		timerqueue_del(head, node);
		ctmr = container_of(node, struct cpu_timer, node);
		ctmr->head = NULL;
	}
}

/*
 * Clean out CPU timers which are still armed when a thread exits. The
 * timers are only removed from the list. No other updates are done. The
 * corresponding posix timers are still accessible, but cannot be rearmed.
 *
 * This must be called with the siglock held.
 */
static void cleanup_timers(struct posix_cputimers *pct)
{
	cleanup_timerqueue(&pct->bases[CPUCLOCK_PROF].tqhead);
	cleanup_timerqueue(&pct->bases[CPUCLOCK_VIRT].tqhead);
	cleanup_timerqueue(&pct->bases[CPUCLOCK_SCHED].tqhead);
>>>>>>> upstream/android-13
}

/*
 * These are both called with the siglock held, when the current thread
 * is being reaped.  When the final (leader) thread in the group is reaped,
 * posix_cpu_timers_exit_group will be called after posix_cpu_timers_exit.
 */
void posix_cpu_timers_exit(struct task_struct *tsk)
{
<<<<<<< HEAD
	cleanup_timers(tsk->cpu_timers);
}
void posix_cpu_timers_exit_group(struct task_struct *tsk)
{
	cleanup_timers(tsk->signal->cpu_timers);
}

static inline int expires_gt(u64 expires, u64 new_exp)
{
	return expires == 0 || expires > new_exp;
=======
	cleanup_timers(&tsk->posix_cputimers);
}
void posix_cpu_timers_exit_group(struct task_struct *tsk)
{
	cleanup_timers(&tsk->signal->posix_cputimers);
>>>>>>> upstream/android-13
}

/*
 * Insert the timer on the appropriate list before any timers that
 * expire later.  This must be called with the sighand lock held.
 */
<<<<<<< HEAD
static void arm_timer(struct k_itimer *timer)
{
	struct task_struct *p = timer->it.cpu.task;
	struct list_head *head, *listpos;
	struct task_cputime *cputime_expires;
	struct cpu_timer_list *const nt = &timer->it.cpu;
	struct cpu_timer_list *next;

	if (CPUCLOCK_PERTHREAD(timer->it_clock)) {
		head = p->cpu_timers;
		cputime_expires = &p->cputime_expires;
	} else {
		head = p->signal->cpu_timers;
		cputime_expires = &p->signal->cputime_expires;
	}
	head += CPUCLOCK_WHICH(timer->it_clock);

	listpos = head;
	list_for_each_entry(next, head, entry) {
		if (nt->expires < next->expires)
			break;
		listpos = &next->entry;
	}
	list_add(&nt->entry, listpos);

	if (listpos == head) {
		u64 exp = nt->expires;

		/*
		 * We are the new earliest-expiring POSIX 1.b timer, hence
		 * need to update expiration cache. Take into account that
		 * for process timers we share expiration cache with itimers
		 * and RLIMIT_CPU and for thread timers with RLIMIT_RTTIME.
		 */

		switch (CPUCLOCK_WHICH(timer->it_clock)) {
		case CPUCLOCK_PROF:
			if (expires_gt(cputime_expires->prof_exp, exp))
				cputime_expires->prof_exp = exp;
			break;
		case CPUCLOCK_VIRT:
			if (expires_gt(cputime_expires->virt_exp, exp))
				cputime_expires->virt_exp = exp;
			break;
		case CPUCLOCK_SCHED:
			if (expires_gt(cputime_expires->sched_exp, exp))
				cputime_expires->sched_exp = exp;
			break;
		}
		if (CPUCLOCK_PERTHREAD(timer->it_clock))
			tick_dep_set_task(p, TICK_DEP_BIT_POSIX_TIMER);
		else
			tick_dep_set_signal(p->signal, TICK_DEP_BIT_POSIX_TIMER);
	}
=======
static void arm_timer(struct k_itimer *timer, struct task_struct *p)
{
	struct posix_cputimer_base *base = timer_base(timer, p);
	struct cpu_timer *ctmr = &timer->it.cpu;
	u64 newexp = cpu_timer_getexpires(ctmr);

	if (!cpu_timer_enqueue(&base->tqhead, ctmr))
		return;

	/*
	 * We are the new earliest-expiring POSIX 1.b timer, hence
	 * need to update expiration cache. Take into account that
	 * for process timers we share expiration cache with itimers
	 * and RLIMIT_CPU and for thread timers with RLIMIT_RTTIME.
	 */
	if (newexp < base->nextevt)
		base->nextevt = newexp;

	if (CPUCLOCK_PERTHREAD(timer->it_clock))
		tick_dep_set_task(p, TICK_DEP_BIT_POSIX_TIMER);
	else
		tick_dep_set_signal(p, TICK_DEP_BIT_POSIX_TIMER);
>>>>>>> upstream/android-13
}

/*
 * The timer is locked, fire it and arrange for its reload.
 */
static void cpu_timer_fire(struct k_itimer *timer)
{
<<<<<<< HEAD
=======
	struct cpu_timer *ctmr = &timer->it.cpu;

>>>>>>> upstream/android-13
	if ((timer->it_sigev_notify & ~SIGEV_THREAD_ID) == SIGEV_NONE) {
		/*
		 * User don't want any signal.
		 */
<<<<<<< HEAD
		timer->it.cpu.expires = 0;
=======
		cpu_timer_setexpires(ctmr, 0);
>>>>>>> upstream/android-13
	} else if (unlikely(timer->sigq == NULL)) {
		/*
		 * This a special case for clock_nanosleep,
		 * not a normal timer from sys_timer_create.
		 */
		wake_up_process(timer->it_process);
<<<<<<< HEAD
		timer->it.cpu.expires = 0;
	} else if (timer->it.cpu.incr == 0) {
=======
		cpu_timer_setexpires(ctmr, 0);
	} else if (!timer->it_interval) {
>>>>>>> upstream/android-13
		/*
		 * One-shot timer.  Clear it as soon as it's fired.
		 */
		posix_timer_event(timer, 0);
<<<<<<< HEAD
		timer->it.cpu.expires = 0;
=======
		cpu_timer_setexpires(ctmr, 0);
>>>>>>> upstream/android-13
	} else if (posix_timer_event(timer, ++timer->it_requeue_pending)) {
		/*
		 * The signal did not get queued because the signal
		 * was ignored, so we won't get any callback to
		 * reload the timer.  But we need to keep it
		 * ticking in case the signal is deliverable next time.
		 */
		posix_cpu_timer_rearm(timer);
		++timer->it_requeue_pending;
	}
}

/*
<<<<<<< HEAD
 * Sample a process (thread group) timer for the given group_leader task.
 * Must be called with task sighand lock held for safe while_each_thread()
 * traversal.
 */
static int cpu_timer_sample_group(const clockid_t which_clock,
				  struct task_struct *p, u64 *sample)
{
	struct task_cputime cputime;

	thread_group_cputimer(p, &cputime);
	switch (CPUCLOCK_WHICH(which_clock)) {
	default:
		return -EINVAL;
	case CPUCLOCK_PROF:
		*sample = cputime.utime + cputime.stime;
		break;
	case CPUCLOCK_VIRT:
		*sample = cputime.utime;
		break;
	case CPUCLOCK_SCHED:
		*sample = cputime.sum_exec_runtime;
		break;
	}
	return 0;
}

/*
=======
>>>>>>> upstream/android-13
 * Guts of sys_timer_settime for CPU timers.
 * This is called with the timer locked and interrupts disabled.
 * If we return TIMER_RETRY, it's necessary to release the timer's lock
 * and try again.  (This happens when the timer is in the middle of firing.)
 */
static int posix_cpu_timer_set(struct k_itimer *timer, int timer_flags,
			       struct itimerspec64 *new, struct itimerspec64 *old)
{
<<<<<<< HEAD
	unsigned long flags;
	struct sighand_struct *sighand;
	struct task_struct *p = timer->it.cpu.task;
	u64 old_expires, new_expires, old_incr, val;
	int ret;

	if (WARN_ON_ONCE(!p))
		return -EINVAL;
=======
	clockid_t clkid = CPUCLOCK_WHICH(timer->it_clock);
	u64 old_expires, new_expires, old_incr, val;
	struct cpu_timer *ctmr = &timer->it.cpu;
	struct sighand_struct *sighand;
	struct task_struct *p;
	unsigned long flags;
	int ret = 0;

	rcu_read_lock();
	p = cpu_timer_task_rcu(timer);
	if (!p) {
		/*
		 * If p has just been reaped, we can no
		 * longer get any information about it at all.
		 */
		rcu_read_unlock();
		return -ESRCH;
	}
>>>>>>> upstream/android-13

	/*
	 * Use the to_ktime conversion because that clamps the maximum
	 * value to KTIME_MAX and avoid multiplication overflows.
	 */
	new_expires = ktime_to_ns(timespec64_to_ktime(new->it_value));

	/*
	 * Protect against sighand release/switch in exit/exec and p->cpu_timers
	 * and p->signal->cpu_timers read/write in arm_timer()
	 */
	sighand = lock_task_sighand(p, &flags);
	/*
	 * If p has just been reaped, we can no
	 * longer get any information about it at all.
	 */
	if (unlikely(sighand == NULL)) {
<<<<<<< HEAD
=======
		rcu_read_unlock();
>>>>>>> upstream/android-13
		return -ESRCH;
	}

	/*
	 * Disarm any old timer after extracting its expiry time.
	 */
<<<<<<< HEAD

	ret = 0;
	old_incr = timer->it.cpu.incr;
	old_expires = timer->it.cpu.expires;
	if (unlikely(timer->it.cpu.firing)) {
		timer->it.cpu.firing = -1;
		ret = TIMER_RETRY;
	} else
		list_del_init(&timer->it.cpu.entry);
=======
	old_incr = timer->it_interval;
	old_expires = cpu_timer_getexpires(ctmr);

	if (unlikely(timer->it.cpu.firing)) {
		timer->it.cpu.firing = -1;
		ret = TIMER_RETRY;
	} else {
		cpu_timer_dequeue(ctmr);
	}
>>>>>>> upstream/android-13

	/*
	 * We need to sample the current value to convert the new
	 * value from to relative and absolute, and to convert the
	 * old value from absolute to relative.  To set a process
	 * timer, we need a sample to balance the thread expiry
	 * times (in arm_timer).  With an absolute time, we must
	 * check if it's already passed.  In short, we need a sample.
	 */
<<<<<<< HEAD
	if (CPUCLOCK_PERTHREAD(timer->it_clock)) {
		cpu_clock_sample(timer->it_clock, p, &val);
	} else {
		cpu_timer_sample_group(timer->it_clock, p, &val);
	}
=======
	if (CPUCLOCK_PERTHREAD(timer->it_clock))
		val = cpu_clock_sample(clkid, p);
	else
		val = cpu_clock_sample_group(clkid, p, true);
>>>>>>> upstream/android-13

	if (old) {
		if (old_expires == 0) {
			old->it_value.tv_sec = 0;
			old->it_value.tv_nsec = 0;
		} else {
			/*
<<<<<<< HEAD
			 * Update the timer in case it has
			 * overrun already.  If it has,
			 * we'll report it as having overrun
			 * and with the next reloaded timer
			 * already ticking, though we are
			 * swallowing that pending
			 * notification here to install the
			 * new setting.
			 */
			bump_cpu_timer(timer, val);
			if (val < timer->it.cpu.expires) {
				old_expires = timer->it.cpu.expires - val;
=======
			 * Update the timer in case it has overrun already.
			 * If it has, we'll report it as having overrun and
			 * with the next reloaded timer already ticking,
			 * though we are swallowing that pending
			 * notification here to install the new setting.
			 */
			u64 exp = bump_cpu_timer(timer, val);

			if (val < exp) {
				old_expires = exp - val;
>>>>>>> upstream/android-13
				old->it_value = ns_to_timespec64(old_expires);
			} else {
				old->it_value.tv_nsec = 1;
				old->it_value.tv_sec = 0;
			}
		}
	}

	if (unlikely(ret)) {
		/*
		 * We are colliding with the timer actually firing.
		 * Punt after filling in the timer's old value, and
		 * disable this firing since we are already reporting
		 * it as an overrun (thanks to bump_cpu_timer above).
		 */
		unlock_task_sighand(p, &flags);
		goto out;
	}

	if (new_expires != 0 && !(timer_flags & TIMER_ABSTIME)) {
		new_expires += val;
	}

	/*
	 * Install the new expiry time (or zero).
	 * For a timer with no notification action, we don't actually
	 * arm the timer (we'll just fake it for timer_gettime).
	 */
<<<<<<< HEAD
	timer->it.cpu.expires = new_expires;
	if (new_expires != 0 && val < new_expires) {
		arm_timer(timer);
=======
	cpu_timer_setexpires(ctmr, new_expires);
	if (new_expires != 0 && val < new_expires) {
		arm_timer(timer, p);
>>>>>>> upstream/android-13
	}

	unlock_task_sighand(p, &flags);
	/*
	 * Install the new reload setting, and
	 * set up the signal and overrun bookkeeping.
	 */
<<<<<<< HEAD
	timer->it.cpu.incr = timespec64_to_ns(&new->it_interval);
	timer->it_interval = ns_to_ktime(timer->it.cpu.incr);
=======
	timer->it_interval = timespec64_to_ktime(new->it_interval);
>>>>>>> upstream/android-13

	/*
	 * This acts as a modification timestamp for the timer,
	 * so any automatic reload attempt will punt on seeing
	 * that we have reset the timer manually.
	 */
	timer->it_requeue_pending = (timer->it_requeue_pending + 2) &
		~REQUEUE_PENDING;
	timer->it_overrun_last = 0;
	timer->it_overrun = -1;

<<<<<<< HEAD
	if (new_expires != 0 && !(val < new_expires)) {
		/*
		 * The designated time already passed, so we notify
		 * immediately, even if the thread never runs to
		 * accumulate more time on this clock.
		 */
		cpu_timer_fire(timer);
	}

	ret = 0;
 out:
=======
	if (val >= new_expires) {
		if (new_expires != 0) {
			/*
			 * The designated time already passed, so we notify
			 * immediately, even if the thread never runs to
			 * accumulate more time on this clock.
			 */
			cpu_timer_fire(timer);
		}

		/*
		 * Make sure we don't keep around the process wide cputime
		 * counter or the tick dependency if they are not necessary.
		 */
		sighand = lock_task_sighand(p, &flags);
		if (!sighand)
			goto out;

		if (!cpu_timer_queued(ctmr))
			trigger_base_recalc_expires(timer, p);

		unlock_task_sighand(p, &flags);
	}
 out:
	rcu_read_unlock();
>>>>>>> upstream/android-13
	if (old)
		old->it_interval = ns_to_timespec64(old_incr);

	return ret;
}

static void posix_cpu_timer_get(struct k_itimer *timer, struct itimerspec64 *itp)
{
<<<<<<< HEAD
	struct task_struct *p = timer->it.cpu.task;
	u64 now;

	if (WARN_ON_ONCE(!p))
		return;
=======
	clockid_t clkid = CPUCLOCK_WHICH(timer->it_clock);
	struct cpu_timer *ctmr = &timer->it.cpu;
	u64 now, expires = cpu_timer_getexpires(ctmr);
	struct task_struct *p;

	rcu_read_lock();
	p = cpu_timer_task_rcu(timer);
	if (!p)
		goto out;
>>>>>>> upstream/android-13

	/*
	 * Easy part: convert the reload time.
	 */
<<<<<<< HEAD
	itp->it_interval = ns_to_timespec64(timer->it.cpu.incr);

	if (!timer->it.cpu.expires)
		return;
=======
	itp->it_interval = ktime_to_timespec64(timer->it_interval);

	if (!expires)
		goto out;
>>>>>>> upstream/android-13

	/*
	 * Sample the clock to take the difference with the expiry time.
	 */
<<<<<<< HEAD
	if (CPUCLOCK_PERTHREAD(timer->it_clock)) {
		cpu_clock_sample(timer->it_clock, p, &now);
	} else {
		struct sighand_struct *sighand;
		unsigned long flags;

		/*
		 * Protect against sighand release/switch in exit/exec and
		 * also make timer sampling safe if it ends up calling
		 * thread_group_cputime().
		 */
		sighand = lock_task_sighand(p, &flags);
		if (unlikely(sighand == NULL)) {
			/*
			 * The process has been reaped.
			 * We can't even collect a sample any more.
			 * Call the timer disarmed, nothing else to do.
			 */
			timer->it.cpu.expires = 0;
			return;
		} else {
			cpu_timer_sample_group(timer->it_clock, p, &now);
			unlock_task_sighand(p, &flags);
		}
	}

	if (now < timer->it.cpu.expires) {
		itp->it_value = ns_to_timespec64(timer->it.cpu.expires - now);
=======
	if (CPUCLOCK_PERTHREAD(timer->it_clock))
		now = cpu_clock_sample(clkid, p);
	else
		now = cpu_clock_sample_group(clkid, p, false);

	if (now < expires) {
		itp->it_value = ns_to_timespec64(expires - now);
>>>>>>> upstream/android-13
	} else {
		/*
		 * The timer should have expired already, but the firing
		 * hasn't taken place yet.  Say it's just about to expire.
		 */
		itp->it_value.tv_nsec = 1;
		itp->it_value.tv_sec = 0;
	}
<<<<<<< HEAD
}

static unsigned long long
check_timers_list(struct list_head *timers,
		  struct list_head *firing,
		  unsigned long long curr)
{
	int maxfire = 20;

	while (!list_empty(timers)) {
		struct cpu_timer_list *t;

		t = list_first_entry(timers, struct cpu_timer_list, entry);

		if (!--maxfire || curr < t->expires)
			return t->expires;

		t->firing = 1;
		list_move_tail(&t->entry, firing);
	}

	return 0;
=======
out:
	rcu_read_unlock();
}

#define MAX_COLLECTED	20

static u64 collect_timerqueue(struct timerqueue_head *head,
			      struct list_head *firing, u64 now)
{
	struct timerqueue_node *next;
	int i = 0;

	while ((next = timerqueue_getnext(head))) {
		struct cpu_timer *ctmr;
		u64 expires;

		ctmr = container_of(next, struct cpu_timer, node);
		expires = cpu_timer_getexpires(ctmr);
		/* Limit the number of timers to expire at once */
		if (++i == MAX_COLLECTED || now < expires)
			return expires;

		ctmr->firing = 1;
		cpu_timer_dequeue(ctmr);
		list_add_tail(&ctmr->elist, firing);
	}

	return U64_MAX;
}

static void collect_posix_cputimers(struct posix_cputimers *pct, u64 *samples,
				    struct list_head *firing)
{
	struct posix_cputimer_base *base = pct->bases;
	int i;

	for (i = 0; i < CPUCLOCK_MAX; i++, base++) {
		base->nextevt = collect_timerqueue(&base->tqhead, firing,
						    samples[i]);
	}
>>>>>>> upstream/android-13
}

static inline void check_dl_overrun(struct task_struct *tsk)
{
	if (tsk->dl.dl_overrun) {
		tsk->dl.dl_overrun = 0;
		__group_send_sig_info(SIGXCPU, SEND_SIG_PRIV, tsk);
	}
}

<<<<<<< HEAD
=======
static bool check_rlimit(u64 time, u64 limit, int signo, bool rt, bool hard)
{
	if (time < limit)
		return false;

	if (print_fatal_signals) {
		pr_info("%s Watchdog Timeout (%s): %s[%d]\n",
			rt ? "RT" : "CPU", hard ? "hard" : "soft",
			current->comm, task_pid_nr(current));
	}
	__group_send_sig_info(signo, SEND_SIG_PRIV, current);
	return true;
}

>>>>>>> upstream/android-13
/*
 * Check for any per-thread CPU timers that have fired and move them off
 * the tsk->cpu_timers[N] list onto the firing list.  Here we update the
 * tsk->it_*_expires values to reflect the remaining thread CPU timers.
 */
static void check_thread_timers(struct task_struct *tsk,
				struct list_head *firing)
{
<<<<<<< HEAD
	struct list_head *timers = tsk->cpu_timers;
	struct task_cputime *tsk_expires = &tsk->cputime_expires;
	u64 expires;
=======
	struct posix_cputimers *pct = &tsk->posix_cputimers;
	u64 samples[CPUCLOCK_MAX];
>>>>>>> upstream/android-13
	unsigned long soft;

	if (dl_task(tsk))
		check_dl_overrun(tsk);

<<<<<<< HEAD
	/*
	 * If cputime_expires is zero, then there are no active
	 * per thread CPU timers.
	 */
	if (task_cputime_zero(&tsk->cputime_expires))
		return;

	expires = check_timers_list(timers, firing, prof_ticks(tsk));
	tsk_expires->prof_exp = expires;

	expires = check_timers_list(++timers, firing, virt_ticks(tsk));
	tsk_expires->virt_exp = expires;

	tsk_expires->sched_exp = check_timers_list(++timers, firing,
						   tsk->se.sum_exec_runtime);
=======
	if (expiry_cache_is_inactive(pct))
		return;

	task_sample_cputime(tsk, samples);
	collect_posix_cputimers(pct, samples, firing);
>>>>>>> upstream/android-13

	/*
	 * Check for the special case thread timers.
	 */
	soft = task_rlimit(tsk, RLIMIT_RTTIME);
	if (soft != RLIM_INFINITY) {
<<<<<<< HEAD
		unsigned long hard = task_rlimit_max(tsk, RLIMIT_RTTIME);

		if (hard != RLIM_INFINITY &&
		    tsk->rt.timeout > DIV_ROUND_UP(hard, USEC_PER_SEC/HZ)) {
			/*
			 * At the hard limit, we just die.
			 * No need to calculate anything else now.
			 */
			if (print_fatal_signals) {
				pr_info("CPU Watchdog Timeout (hard): %s[%d]\n",
					tsk->comm, task_pid_nr(tsk));
			}
			__group_send_sig_info(SIGKILL, SEND_SIG_PRIV, tsk);
			return;
		}
		if (tsk->rt.timeout > DIV_ROUND_UP(soft, USEC_PER_SEC/HZ)) {
			/*
			 * At the soft limit, send a SIGXCPU every second.
			 */
			if (soft < hard) {
				soft += USEC_PER_SEC;
				tsk->signal->rlim[RLIMIT_RTTIME].rlim_cur =
					soft;
			}
			if (print_fatal_signals) {
				pr_info("RT Watchdog Timeout (soft): %s[%d]\n",
					tsk->comm, task_pid_nr(tsk));
			}
			__group_send_sig_info(SIGXCPU, SEND_SIG_PRIV, tsk);
		}
	}
	if (task_cputime_zero(tsk_expires))
=======
		/* Task RT timeout is accounted in jiffies. RTTIME is usec */
		unsigned long rttime = tsk->rt.timeout * (USEC_PER_SEC / HZ);
		unsigned long hard = task_rlimit_max(tsk, RLIMIT_RTTIME);

		/* At the hard limit, send SIGKILL. No further action. */
		if (hard != RLIM_INFINITY &&
		    check_rlimit(rttime, hard, SIGKILL, true, true))
			return;

		/* At the soft limit, send a SIGXCPU every second */
		if (check_rlimit(rttime, soft, SIGXCPU, true, false)) {
			soft += USEC_PER_SEC;
			tsk->signal->rlim[RLIMIT_RTTIME].rlim_cur = soft;
		}
	}

	if (expiry_cache_is_inactive(pct))
>>>>>>> upstream/android-13
		tick_dep_clear_task(tsk, TICK_DEP_BIT_POSIX_TIMER);
}

static inline void stop_process_timers(struct signal_struct *sig)
{
<<<<<<< HEAD
	struct thread_group_cputimer *cputimer = &sig->cputimer;

	/* Turn off cputimer->running. This is done without locking. */
	WRITE_ONCE(cputimer->running, false);
=======
	struct posix_cputimers *pct = &sig->posix_cputimers;

	/* Turn off the active flag. This is done without locking. */
	WRITE_ONCE(pct->timers_active, false);
>>>>>>> upstream/android-13
	tick_dep_clear_signal(sig, TICK_DEP_BIT_POSIX_TIMER);
}

static void check_cpu_itimer(struct task_struct *tsk, struct cpu_itimer *it,
			     u64 *expires, u64 cur_time, int signo)
{
	if (!it->expires)
		return;

	if (cur_time >= it->expires) {
		if (it->incr)
			it->expires += it->incr;
		else
			it->expires = 0;

		trace_itimer_expire(signo == SIGPROF ?
				    ITIMER_PROF : ITIMER_VIRTUAL,
				    task_tgid(tsk), cur_time);
		__group_send_sig_info(signo, SEND_SIG_PRIV, tsk);
	}

<<<<<<< HEAD
	if (it->expires && (!*expires || it->expires < *expires))
=======
	if (it->expires && it->expires < *expires)
>>>>>>> upstream/android-13
		*expires = it->expires;
}

/*
 * Check for any per-thread CPU timers that have fired and move them
 * off the tsk->*_timers list onto the firing list.  Per-thread timers
 * have already been taken off.
 */
static void check_process_timers(struct task_struct *tsk,
				 struct list_head *firing)
{
	struct signal_struct *const sig = tsk->signal;
<<<<<<< HEAD
	u64 utime, ptime, virt_expires, prof_expires;
	u64 sum_sched_runtime, sched_expires;
	struct list_head *timers = sig->cpu_timers;
	struct task_cputime cputime;
	unsigned long soft;

	if (dl_task(tsk))
		check_dl_overrun(tsk);

	/*
	 * If cputimer is not running, then there are no active
	 * process wide timers (POSIX 1.b, itimers, RLIMIT_CPU).
	 */
	if (!READ_ONCE(tsk->signal->cputimer.running))
		return;

        /*
	 * Signify that a thread is checking for process timers.
	 * Write access to this field is protected by the sighand lock.
	 */
	sig->cputimer.checking_timer = true;

	/*
	 * Collect the current process totals.
	 */
	thread_group_cputimer(tsk, &cputime);
	utime = cputime.utime;
	ptime = utime + cputime.stime;
	sum_sched_runtime = cputime.sum_exec_runtime;

	prof_expires = check_timers_list(timers, firing, ptime);
	virt_expires = check_timers_list(++timers, firing, utime);
	sched_expires = check_timers_list(++timers, firing, sum_sched_runtime);
=======
	struct posix_cputimers *pct = &sig->posix_cputimers;
	u64 samples[CPUCLOCK_MAX];
	unsigned long soft;

	/*
	 * If there are no active process wide timers (POSIX 1.b, itimers,
	 * RLIMIT_CPU) nothing to check. Also skip the process wide timer
	 * processing when there is already another task handling them.
	 */
	if (!READ_ONCE(pct->timers_active) || pct->expiry_active)
		return;

	/*
	 * Signify that a thread is checking for process timers.
	 * Write access to this field is protected by the sighand lock.
	 */
	pct->expiry_active = true;

	/*
	 * Collect the current process totals. Group accounting is active
	 * so the sample can be taken directly.
	 */
	proc_sample_cputime_atomic(&sig->cputimer.cputime_atomic, samples);
	collect_posix_cputimers(pct, samples, firing);
>>>>>>> upstream/android-13

	/*
	 * Check for the special case process timers.
	 */
<<<<<<< HEAD
	check_cpu_itimer(tsk, &sig->it[CPUCLOCK_PROF], &prof_expires, ptime,
			 SIGPROF);
	check_cpu_itimer(tsk, &sig->it[CPUCLOCK_VIRT], &virt_expires, utime,
			 SIGVTALRM);
	soft = task_rlimit(tsk, RLIMIT_CPU);
	if (soft != RLIM_INFINITY) {
		unsigned long psecs = div_u64(ptime, NSEC_PER_SEC);
		unsigned long hard = task_rlimit_max(tsk, RLIMIT_CPU);
		u64 x;
		if (psecs >= hard) {
			/*
			 * At the hard limit, we just die.
			 * No need to calculate anything else now.
			 */
			if (print_fatal_signals) {
				pr_info("RT Watchdog Timeout (hard): %s[%d]\n",
					tsk->comm, task_pid_nr(tsk));
			}
			__group_send_sig_info(SIGKILL, SEND_SIG_PRIV, tsk);
			return;
		}
		if (psecs >= soft) {
			/*
			 * At the soft limit, send a SIGXCPU every second.
			 */
			if (print_fatal_signals) {
				pr_info("CPU Watchdog Timeout (soft): %s[%d]\n",
					tsk->comm, task_pid_nr(tsk));
			}
			__group_send_sig_info(SIGXCPU, SEND_SIG_PRIV, tsk);
			if (soft < hard) {
				soft++;
				sig->rlim[RLIMIT_CPU].rlim_cur = soft;
			}
		}
		x = soft * NSEC_PER_SEC;
		if (!prof_expires || x < prof_expires)
			prof_expires = x;
	}

	sig->cputime_expires.prof_exp = prof_expires;
	sig->cputime_expires.virt_exp = virt_expires;
	sig->cputime_expires.sched_exp = sched_expires;
	if (task_cputime_zero(&sig->cputime_expires))
		stop_process_timers(sig);

	sig->cputimer.checking_timer = false;
=======
	check_cpu_itimer(tsk, &sig->it[CPUCLOCK_PROF],
			 &pct->bases[CPUCLOCK_PROF].nextevt,
			 samples[CPUCLOCK_PROF], SIGPROF);
	check_cpu_itimer(tsk, &sig->it[CPUCLOCK_VIRT],
			 &pct->bases[CPUCLOCK_VIRT].nextevt,
			 samples[CPUCLOCK_VIRT], SIGVTALRM);

	soft = task_rlimit(tsk, RLIMIT_CPU);
	if (soft != RLIM_INFINITY) {
		/* RLIMIT_CPU is in seconds. Samples are nanoseconds */
		unsigned long hard = task_rlimit_max(tsk, RLIMIT_CPU);
		u64 ptime = samples[CPUCLOCK_PROF];
		u64 softns = (u64)soft * NSEC_PER_SEC;
		u64 hardns = (u64)hard * NSEC_PER_SEC;

		/* At the hard limit, send SIGKILL. No further action. */
		if (hard != RLIM_INFINITY &&
		    check_rlimit(ptime, hardns, SIGKILL, false, true))
			return;

		/* At the soft limit, send a SIGXCPU every second */
		if (check_rlimit(ptime, softns, SIGXCPU, false, false)) {
			sig->rlim[RLIMIT_CPU].rlim_cur = soft + 1;
			softns += NSEC_PER_SEC;
		}

		/* Update the expiry cache */
		if (softns < pct->bases[CPUCLOCK_PROF].nextevt)
			pct->bases[CPUCLOCK_PROF].nextevt = softns;
	}

	if (expiry_cache_is_inactive(pct))
		stop_process_timers(sig);

	pct->expiry_active = false;
>>>>>>> upstream/android-13
}

/*
 * This is called from the signal code (via posixtimer_rearm)
 * when the last timer signal was delivered and we have to reload the timer.
 */
static void posix_cpu_timer_rearm(struct k_itimer *timer)
{
<<<<<<< HEAD
	struct task_struct *p = timer->it.cpu.task;
=======
	clockid_t clkid = CPUCLOCK_WHICH(timer->it_clock);
	struct task_struct *p;
>>>>>>> upstream/android-13
	struct sighand_struct *sighand;
	unsigned long flags;
	u64 now;

<<<<<<< HEAD
	if (WARN_ON_ONCE(!p))
		return;
=======
	rcu_read_lock();
	p = cpu_timer_task_rcu(timer);
	if (!p)
		goto out;

	/* Protect timer list r/w in arm_timer() */
	sighand = lock_task_sighand(p, &flags);
	if (unlikely(sighand == NULL))
		goto out;
>>>>>>> upstream/android-13

	/*
	 * Fetch the current sample and update the timer's expiry time.
	 */
<<<<<<< HEAD
	if (CPUCLOCK_PERTHREAD(timer->it_clock)) {
		cpu_clock_sample(timer->it_clock, p, &now);
		bump_cpu_timer(timer, now);
		if (unlikely(p->exit_state))
			return;

		/* Protect timer list r/w in arm_timer() */
		sighand = lock_task_sighand(p, &flags);
		if (!sighand)
			return;
	} else {
		/*
		 * Protect arm_timer() and timer sampling in case of call to
		 * thread_group_cputime().
		 */
		sighand = lock_task_sighand(p, &flags);
		if (unlikely(sighand == NULL)) {
			/*
			 * The process has been reaped.
			 * We can't even collect a sample any more.
			 */
			timer->it.cpu.expires = 0;
			return;
		} else if (unlikely(p->exit_state) && thread_group_empty(p)) {
			/* If the process is dying, no need to rearm */
			goto unlock;
		}
		cpu_timer_sample_group(timer->it_clock, p, &now);
		bump_cpu_timer(timer, now);
		/* Leave the sighand locked for the call below.  */
	}
=======
	if (CPUCLOCK_PERTHREAD(timer->it_clock))
		now = cpu_clock_sample(clkid, p);
	else
		now = cpu_clock_sample_group(clkid, p, true);

	bump_cpu_timer(timer, now);
>>>>>>> upstream/android-13

	/*
	 * Now re-arm for the new expiry time.
	 */
<<<<<<< HEAD
	arm_timer(timer);
unlock:
	unlock_task_sighand(p, &flags);
}

/**
 * task_cputime_expired - Compare two task_cputime entities.
 *
 * @sample:	The task_cputime structure to be checked for expiration.
 * @expires:	Expiration times, against which @sample will be checked.
 *
 * Checks @sample against @expires to see if any field of @sample has expired.
 * Returns true if any field of the former is greater than the corresponding
 * field of the latter if the latter field is set.  Otherwise returns false.
 */
static inline int task_cputime_expired(const struct task_cputime *sample,
					const struct task_cputime *expires)
{
	if (expires->utime && sample->utime >= expires->utime)
		return 1;
	if (expires->stime && sample->utime + sample->stime >= expires->stime)
		return 1;
	if (expires->sum_exec_runtime != 0 &&
	    sample->sum_exec_runtime >= expires->sum_exec_runtime)
		return 1;
	return 0;
=======
	arm_timer(timer, p);
	unlock_task_sighand(p, &flags);
out:
	rcu_read_unlock();
}

/**
 * task_cputimers_expired - Check whether posix CPU timers are expired
 *
 * @samples:	Array of current samples for the CPUCLOCK clocks
 * @pct:	Pointer to a posix_cputimers container
 *
 * Returns true if any member of @samples is greater than the corresponding
 * member of @pct->bases[CLK].nextevt. False otherwise
 */
static inline bool
task_cputimers_expired(const u64 *samples, struct posix_cputimers *pct)
{
	int i;

	for (i = 0; i < CPUCLOCK_MAX; i++) {
		if (samples[i] >= pct->bases[i].nextevt)
			return true;
	}
	return false;
>>>>>>> upstream/android-13
}

/**
 * fastpath_timer_check - POSIX CPU timers fast path.
 *
 * @tsk:	The task (thread) being checked.
 *
 * Check the task and thread group timers.  If both are zero (there are no
 * timers set) return false.  Otherwise snapshot the task and thread group
 * timers and compare them with the corresponding expiration times.  Return
 * true if a timer has expired, else return false.
 */
<<<<<<< HEAD
static inline int fastpath_timer_check(struct task_struct *tsk)
{
	struct signal_struct *sig;

	if (!task_cputime_zero(&tsk->cputime_expires)) {
		struct task_cputime task_sample;

		task_cputime(tsk, &task_sample.utime, &task_sample.stime);
		task_sample.sum_exec_runtime = tsk->se.sum_exec_runtime;
		if (task_cputime_expired(&task_sample, &tsk->cputime_expires))
			return 1;
	}

	sig = tsk->signal;
	/*
	 * Check if thread group timers expired when the cputimer is
	 * running and no other thread in the group is already checking
	 * for thread group cputimers. These fields are read without the
	 * sighand lock. However, this is fine because this is meant to
	 * be a fastpath heuristic to determine whether we should try to
	 * acquire the sighand lock to check/handle timers.
	 *
	 * In the worst case scenario, if 'running' or 'checking_timer' gets
	 * set but the current thread doesn't see the change yet, we'll wait
	 * until the next thread in the group gets a scheduler interrupt to
	 * handle the timer. This isn't an issue in practice because these
	 * types of delays with signals actually getting sent are expected.
	 */
	if (READ_ONCE(sig->cputimer.running) &&
	    !READ_ONCE(sig->cputimer.checking_timer)) {
		struct task_cputime group_sample;

		sample_cputime_atomic(&group_sample, &sig->cputimer.cputime_atomic);

		if (task_cputime_expired(&group_sample, &sig->cputime_expires))
			return 1;
	}

	if (dl_task(tsk) && tsk->dl.dl_overrun)
		return 1;

	return 0;
}

/*
 * This is called from the timer interrupt handler.  The irq handler has
 * already updated our counts.  We need to check if any timers fire now.
 * Interrupts are disabled.
 */
void run_posix_cpu_timers(struct task_struct *tsk)
{
	LIST_HEAD(firing);
	struct k_itimer *timer, *next;
	unsigned long flags;

	lockdep_assert_irqs_disabled();

	/*
	 * The fast path checks that there are no expired thread or thread
	 * group timers.  If that's so, just return.
	 */
	if (!fastpath_timer_check(tsk))
		return;

	if (!lock_task_sighand(tsk, &flags))
		return;
	/*
	 * Here we take off tsk->signal->cpu_timers[N] and
	 * tsk->cpu_timers[N] all the timers that are firing, and
	 * put them on the firing list.
	 */
	check_thread_timers(tsk, &firing);

	check_process_timers(tsk, &firing);

	/*
	 * We must release these locks before taking any timer's lock.
=======
static inline bool fastpath_timer_check(struct task_struct *tsk)
{
	struct posix_cputimers *pct = &tsk->posix_cputimers;
	struct signal_struct *sig;

	if (!expiry_cache_is_inactive(pct)) {
		u64 samples[CPUCLOCK_MAX];

		task_sample_cputime(tsk, samples);
		if (task_cputimers_expired(samples, pct))
			return true;
	}

	sig = tsk->signal;
	pct = &sig->posix_cputimers;
	/*
	 * Check if thread group timers expired when timers are active and
	 * no other thread in the group is already handling expiry for
	 * thread group cputimers. These fields are read without the
	 * sighand lock. However, this is fine because this is meant to be
	 * a fastpath heuristic to determine whether we should try to
	 * acquire the sighand lock to handle timer expiry.
	 *
	 * In the worst case scenario, if concurrently timers_active is set
	 * or expiry_active is cleared, but the current thread doesn't see
	 * the change yet, the timer checks are delayed until the next
	 * thread in the group gets a scheduler interrupt to handle the
	 * timer. This isn't an issue in practice because these types of
	 * delays with signals actually getting sent are expected.
	 */
	if (READ_ONCE(pct->timers_active) && !READ_ONCE(pct->expiry_active)) {
		u64 samples[CPUCLOCK_MAX];

		proc_sample_cputime_atomic(&sig->cputimer.cputime_atomic,
					   samples);

		if (task_cputimers_expired(samples, pct))
			return true;
	}

	if (dl_task(tsk) && tsk->dl.dl_overrun)
		return true;

	return false;
}

static void handle_posix_cpu_timers(struct task_struct *tsk);

#ifdef CONFIG_POSIX_CPU_TIMERS_TASK_WORK
static void posix_cpu_timers_work(struct callback_head *work)
{
	handle_posix_cpu_timers(current);
}

/*
 * Clear existing posix CPU timers task work.
 */
void clear_posix_cputimers_work(struct task_struct *p)
{
	/*
	 * A copied work entry from the old task is not meaningful, clear it.
	 * N.B. init_task_work will not do this.
	 */
	memset(&p->posix_cputimers_work.work, 0,
	       sizeof(p->posix_cputimers_work.work));
	init_task_work(&p->posix_cputimers_work.work,
		       posix_cpu_timers_work);
	p->posix_cputimers_work.scheduled = false;
}

/*
 * Initialize posix CPU timers task work in init task. Out of line to
 * keep the callback static and to avoid header recursion hell.
 */
void __init posix_cputimers_init_work(void)
{
	clear_posix_cputimers_work(current);
}

/*
 * Note: All operations on tsk->posix_cputimer_work.scheduled happen either
 * in hard interrupt context or in task context with interrupts
 * disabled. Aside of that the writer/reader interaction is always in the
 * context of the current task, which means they are strict per CPU.
 */
static inline bool posix_cpu_timers_work_scheduled(struct task_struct *tsk)
{
	return tsk->posix_cputimers_work.scheduled;
}

static inline void __run_posix_cpu_timers(struct task_struct *tsk)
{
	if (WARN_ON_ONCE(tsk->posix_cputimers_work.scheduled))
		return;

	/* Schedule task work to actually expire the timers */
	tsk->posix_cputimers_work.scheduled = true;
	task_work_add(tsk, &tsk->posix_cputimers_work.work, TWA_RESUME);
}

static inline bool posix_cpu_timers_enable_work(struct task_struct *tsk,
						unsigned long start)
{
	bool ret = true;

	/*
	 * On !RT kernels interrupts are disabled while collecting expired
	 * timers, so no tick can happen and the fast path check can be
	 * reenabled without further checks.
	 */
	if (!IS_ENABLED(CONFIG_PREEMPT_RT)) {
		tsk->posix_cputimers_work.scheduled = false;
		return true;
	}

	/*
	 * On RT enabled kernels ticks can happen while the expired timers
	 * are collected under sighand lock. But any tick which observes
	 * the CPUTIMERS_WORK_SCHEDULED bit set, does not run the fastpath
	 * checks. So reenabling the tick work has do be done carefully:
	 *
	 * Disable interrupts and run the fast path check if jiffies have
	 * advanced since the collecting of expired timers started. If
	 * jiffies have not advanced or the fast path check did not find
	 * newly expired timers, reenable the fast path check in the timer
	 * interrupt. If there are newly expired timers, return false and
	 * let the collection loop repeat.
	 */
	local_irq_disable();
	if (start != jiffies && fastpath_timer_check(tsk))
		ret = false;
	else
		tsk->posix_cputimers_work.scheduled = false;
	local_irq_enable();

	return ret;
}
#else /* CONFIG_POSIX_CPU_TIMERS_TASK_WORK */
static inline void __run_posix_cpu_timers(struct task_struct *tsk)
{
	lockdep_posixtimer_enter();
	handle_posix_cpu_timers(tsk);
	lockdep_posixtimer_exit();
}

static inline bool posix_cpu_timers_work_scheduled(struct task_struct *tsk)
{
	return false;
}

static inline bool posix_cpu_timers_enable_work(struct task_struct *tsk,
						unsigned long start)
{
	return true;
}
#endif /* CONFIG_POSIX_CPU_TIMERS_TASK_WORK */

static void handle_posix_cpu_timers(struct task_struct *tsk)
{
	struct k_itimer *timer, *next;
	unsigned long flags, start;
	LIST_HEAD(firing);

	if (!lock_task_sighand(tsk, &flags))
		return;

	do {
		/*
		 * On RT locking sighand lock does not disable interrupts,
		 * so this needs to be careful vs. ticks. Store the current
		 * jiffies value.
		 */
		start = READ_ONCE(jiffies);
		barrier();

		/*
		 * Here we take off tsk->signal->cpu_timers[N] and
		 * tsk->cpu_timers[N] all the timers that are firing, and
		 * put them on the firing list.
		 */
		check_thread_timers(tsk, &firing);

		check_process_timers(tsk, &firing);

		/*
		 * The above timer checks have updated the expiry cache and
		 * because nothing can have queued or modified timers after
		 * sighand lock was taken above it is guaranteed to be
		 * consistent. So the next timer interrupt fastpath check
		 * will find valid data.
		 *
		 * If timer expiry runs in the timer interrupt context then
		 * the loop is not relevant as timers will be directly
		 * expired in interrupt context. The stub function below
		 * returns always true which allows the compiler to
		 * optimize the loop out.
		 *
		 * If timer expiry is deferred to task work context then
		 * the following rules apply:
		 *
		 * - On !RT kernels no tick can have happened on this CPU
		 *   after sighand lock was acquired because interrupts are
		 *   disabled. So reenabling task work before dropping
		 *   sighand lock and reenabling interrupts is race free.
		 *
		 * - On RT kernels ticks might have happened but the tick
		 *   work ignored posix CPU timer handling because the
		 *   CPUTIMERS_WORK_SCHEDULED bit is set. Reenabling work
		 *   must be done very carefully including a check whether
		 *   ticks have happened since the start of the timer
		 *   expiry checks. posix_cpu_timers_enable_work() takes
		 *   care of that and eventually lets the expiry checks
		 *   run again.
		 */
	} while (!posix_cpu_timers_enable_work(tsk, start));

	/*
	 * We must release sighand lock before taking any timer's lock.
>>>>>>> upstream/android-13
	 * There is a potential race with timer deletion here, as the
	 * siglock now protects our private firing list.  We have set
	 * the firing flag in each timer, so that a deletion attempt
	 * that gets the timer lock before we do will give it up and
	 * spin until we've taken care of that timer below.
	 */
	unlock_task_sighand(tsk, &flags);

	/*
	 * Now that all the timers on our list have the firing flag,
	 * no one will touch their list entries but us.  We'll take
	 * each timer's lock before clearing its firing flag, so no
	 * timer call will interfere.
	 */
<<<<<<< HEAD
	list_for_each_entry_safe(timer, next, &firing, it.cpu.entry) {
		int cpu_firing;

		spin_lock(&timer->it_lock);
		list_del_init(&timer->it.cpu.entry);
=======
	list_for_each_entry_safe(timer, next, &firing, it.cpu.elist) {
		int cpu_firing;

		/*
		 * spin_lock() is sufficient here even independent of the
		 * expiry context. If expiry happens in hard interrupt
		 * context it's obvious. For task work context it's safe
		 * because all other operations on timer::it_lock happen in
		 * task context (syscall or exit).
		 */
		spin_lock(&timer->it_lock);
		list_del_init(&timer->it.cpu.elist);
>>>>>>> upstream/android-13
		cpu_firing = timer->it.cpu.firing;
		timer->it.cpu.firing = 0;
		/*
		 * The firing flag is -1 if we collided with a reset
		 * of the timer, which already reported this
		 * almost-firing as an overrun.  So don't generate an event.
		 */
		if (likely(cpu_firing >= 0))
			cpu_timer_fire(timer);
		spin_unlock(&timer->it_lock);
	}
}

/*
<<<<<<< HEAD
 * Set one of the process-wide special case CPU timers or RLIMIT_CPU.
 * The tsk->sighand->siglock must be held by the caller.
 */
void set_process_cpu_timer(struct task_struct *tsk, unsigned int clock_idx,
			   u64 *newval, u64 *oldval)
{
	u64 now;
	int ret;

	if (WARN_ON_ONCE(clock_idx >= CPUCLOCK_SCHED))
		return;

	ret = cpu_timer_sample_group(clock_idx, tsk, &now);

	if (oldval && ret != -EINVAL) {
=======
 * This is called from the timer interrupt handler.  The irq handler has
 * already updated our counts.  We need to check if any timers fire now.
 * Interrupts are disabled.
 */
void run_posix_cpu_timers(void)
{
	struct task_struct *tsk = current;

	lockdep_assert_irqs_disabled();

	/*
	 * If the actual expiry is deferred to task work context and the
	 * work is already scheduled there is no point to do anything here.
	 */
	if (posix_cpu_timers_work_scheduled(tsk))
		return;

	/*
	 * The fast path checks that there are no expired thread or thread
	 * group timers.  If that's so, just return.
	 */
	if (!fastpath_timer_check(tsk))
		return;

	__run_posix_cpu_timers(tsk);
}

/*
 * Set one of the process-wide special case CPU timers or RLIMIT_CPU.
 * The tsk->sighand->siglock must be held by the caller.
 */
void set_process_cpu_timer(struct task_struct *tsk, unsigned int clkid,
			   u64 *newval, u64 *oldval)
{
	u64 now, *nextevt;

	if (WARN_ON_ONCE(clkid >= CPUCLOCK_SCHED))
		return;

	nextevt = &tsk->signal->posix_cputimers.bases[clkid].nextevt;
	now = cpu_clock_sample_group(clkid, tsk, true);

	if (oldval) {
>>>>>>> upstream/android-13
		/*
		 * We are setting itimer. The *oldval is absolute and we update
		 * it to be relative, *newval argument is relative and we update
		 * it to be absolute.
		 */
		if (*oldval) {
			if (*oldval <= now) {
				/* Just about to fire. */
				*oldval = TICK_NSEC;
			} else {
				*oldval -= now;
			}
		}

<<<<<<< HEAD
		if (!*newval)
			return;
		*newval += now;
	}

	/*
	 * Update expiration cache if we are the earliest timer, or eventually
	 * RLIMIT_CPU limit is earlier than prof_exp cpu timer expire.
	 */
	switch (clock_idx) {
	case CPUCLOCK_PROF:
		if (expires_gt(tsk->signal->cputime_expires.prof_exp, *newval))
			tsk->signal->cputime_expires.prof_exp = *newval;
		break;
	case CPUCLOCK_VIRT:
		if (expires_gt(tsk->signal->cputime_expires.virt_exp, *newval))
			tsk->signal->cputime_expires.virt_exp = *newval;
		break;
	}

	tick_dep_set_signal(tsk->signal, TICK_DEP_BIT_POSIX_TIMER);
=======
		if (*newval)
			*newval += now;
	}

	/*
	 * Update expiration cache if this is the earliest timer. CPUCLOCK_PROF
	 * expiry cache is also used by RLIMIT_CPU!.
	 */
	if (*newval < *nextevt)
		*nextevt = *newval;

	tick_dep_set_signal(tsk, TICK_DEP_BIT_POSIX_TIMER);
>>>>>>> upstream/android-13
}

static int do_cpu_nanosleep(const clockid_t which_clock, int flags,
			    const struct timespec64 *rqtp)
{
	struct itimerspec64 it;
	struct k_itimer timer;
	u64 expires;
	int error;

	/*
	 * Set up a temporary timer and then wait for it to go off.
	 */
	memset(&timer, 0, sizeof timer);
	spin_lock_init(&timer.it_lock);
	timer.it_clock = which_clock;
	timer.it_overrun = -1;
	error = posix_cpu_timer_create(&timer);
	timer.it_process = current;
<<<<<<< HEAD
=======

>>>>>>> upstream/android-13
	if (!error) {
		static struct itimerspec64 zero_it;
		struct restart_block *restart;

		memset(&it, 0, sizeof(it));
		it.it_value = *rqtp;

		spin_lock_irq(&timer.it_lock);
		error = posix_cpu_timer_set(&timer, flags, &it, NULL);
		if (error) {
			spin_unlock_irq(&timer.it_lock);
			return error;
		}

		while (!signal_pending(current)) {
<<<<<<< HEAD
			if (timer.it.cpu.expires == 0) {
=======
			if (!cpu_timer_getexpires(&timer.it.cpu)) {
>>>>>>> upstream/android-13
				/*
				 * Our timer fired and was reset, below
				 * deletion can not fail.
				 */
				posix_cpu_timer_del(&timer);
				spin_unlock_irq(&timer.it_lock);
				return 0;
			}

			/*
			 * Block until cpu_timer_fire (or a signal) wakes us.
			 */
			__set_current_state(TASK_INTERRUPTIBLE);
			spin_unlock_irq(&timer.it_lock);
			schedule();
			spin_lock_irq(&timer.it_lock);
		}

		/*
		 * We were interrupted by a signal.
		 */
<<<<<<< HEAD
		expires = timer.it.cpu.expires;
=======
		expires = cpu_timer_getexpires(&timer.it.cpu);
>>>>>>> upstream/android-13
		error = posix_cpu_timer_set(&timer, 0, &zero_it, &it);
		if (!error) {
			/*
			 * Timer is now unarmed, deletion can not fail.
			 */
			posix_cpu_timer_del(&timer);
		}
		spin_unlock_irq(&timer.it_lock);

		while (error == TIMER_RETRY) {
			/*
			 * We need to handle case when timer was or is in the
			 * middle of firing. In other cases we already freed
			 * resources.
			 */
			spin_lock_irq(&timer.it_lock);
			error = posix_cpu_timer_del(&timer);
			spin_unlock_irq(&timer.it_lock);
		}

		if ((it.it_value.tv_sec | it.it_value.tv_nsec) == 0) {
			/*
			 * It actually did fire already.
			 */
			return 0;
		}

		error = -ERESTART_RESTARTBLOCK;
		/*
		 * Report back to the user the time still remaining.
		 */
		restart = &current->restart_block;
		restart->nanosleep.expires = expires;
		if (restart->nanosleep.type != TT_NONE)
			error = nanosleep_copyout(restart, &it.it_value);
	}

	return error;
}

static long posix_cpu_nsleep_restart(struct restart_block *restart_block);

static int posix_cpu_nsleep(const clockid_t which_clock, int flags,
			    const struct timespec64 *rqtp)
{
	struct restart_block *restart_block = &current->restart_block;
	int error;

	/*
	 * Diagnose required errors first.
	 */
	if (CPUCLOCK_PERTHREAD(which_clock) &&
	    (CPUCLOCK_PID(which_clock) == 0 ||
	     CPUCLOCK_PID(which_clock) == task_pid_vnr(current)))
		return -EINVAL;

	error = do_cpu_nanosleep(which_clock, flags, rqtp);

	if (error == -ERESTART_RESTARTBLOCK) {

		if (flags & TIMER_ABSTIME)
			return -ERESTARTNOHAND;

		restart_block->nanosleep.clockid = which_clock;
		set_restart_fn(restart_block, posix_cpu_nsleep_restart);
	}
	return error;
}

static long posix_cpu_nsleep_restart(struct restart_block *restart_block)
{
	clockid_t which_clock = restart_block->nanosleep.clockid;
	struct timespec64 t;

	t = ns_to_timespec64(restart_block->nanosleep.expires);

	return do_cpu_nanosleep(which_clock, TIMER_ABSTIME, &t);
}

#define PROCESS_CLOCK	make_process_cpuclock(0, CPUCLOCK_SCHED)
#define THREAD_CLOCK	make_thread_cpuclock(0, CPUCLOCK_SCHED)

static int process_cpu_clock_getres(const clockid_t which_clock,
				    struct timespec64 *tp)
{
	return posix_cpu_clock_getres(PROCESS_CLOCK, tp);
}
static int process_cpu_clock_get(const clockid_t which_clock,
				 struct timespec64 *tp)
{
	return posix_cpu_clock_get(PROCESS_CLOCK, tp);
}
static int process_cpu_timer_create(struct k_itimer *timer)
{
	timer->it_clock = PROCESS_CLOCK;
	return posix_cpu_timer_create(timer);
}
static int process_cpu_nsleep(const clockid_t which_clock, int flags,
			      const struct timespec64 *rqtp)
{
	return posix_cpu_nsleep(PROCESS_CLOCK, flags, rqtp);
}
static int thread_cpu_clock_getres(const clockid_t which_clock,
				   struct timespec64 *tp)
{
	return posix_cpu_clock_getres(THREAD_CLOCK, tp);
}
static int thread_cpu_clock_get(const clockid_t which_clock,
				struct timespec64 *tp)
{
	return posix_cpu_clock_get(THREAD_CLOCK, tp);
}
static int thread_cpu_timer_create(struct k_itimer *timer)
{
	timer->it_clock = THREAD_CLOCK;
	return posix_cpu_timer_create(timer);
}

const struct k_clock clock_posix_cpu = {
<<<<<<< HEAD
	.clock_getres	= posix_cpu_clock_getres,
	.clock_set	= posix_cpu_clock_set,
	.clock_get	= posix_cpu_clock_get,
	.timer_create	= posix_cpu_timer_create,
	.nsleep		= posix_cpu_nsleep,
	.timer_set	= posix_cpu_timer_set,
	.timer_del	= posix_cpu_timer_del,
	.timer_get	= posix_cpu_timer_get,
	.timer_rearm	= posix_cpu_timer_rearm,
};

const struct k_clock clock_process = {
	.clock_getres	= process_cpu_clock_getres,
	.clock_get	= process_cpu_clock_get,
	.timer_create	= process_cpu_timer_create,
	.nsleep		= process_cpu_nsleep,
};

const struct k_clock clock_thread = {
	.clock_getres	= thread_cpu_clock_getres,
	.clock_get	= thread_cpu_clock_get,
	.timer_create	= thread_cpu_timer_create,
=======
	.clock_getres		= posix_cpu_clock_getres,
	.clock_set		= posix_cpu_clock_set,
	.clock_get_timespec	= posix_cpu_clock_get,
	.timer_create		= posix_cpu_timer_create,
	.nsleep			= posix_cpu_nsleep,
	.timer_set		= posix_cpu_timer_set,
	.timer_del		= posix_cpu_timer_del,
	.timer_get		= posix_cpu_timer_get,
	.timer_rearm		= posix_cpu_timer_rearm,
};

const struct k_clock clock_process = {
	.clock_getres		= process_cpu_clock_getres,
	.clock_get_timespec	= process_cpu_clock_get,
	.timer_create		= process_cpu_timer_create,
	.nsleep			= process_cpu_nsleep,
};

const struct k_clock clock_thread = {
	.clock_getres		= thread_cpu_clock_getres,
	.clock_get_timespec	= thread_cpu_clock_get,
	.timer_create		= thread_cpu_timer_create,
>>>>>>> upstream/android-13
};
