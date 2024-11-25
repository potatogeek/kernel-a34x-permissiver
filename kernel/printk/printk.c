<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> upstream/android-13
/*
 *  linux/kernel/printk.c
 *
 *  Copyright (C) 1991, 1992  Linus Torvalds
 *
 * Modified to make sys_syslog() more flexible: added commands to
 * return the last 4k of kernel messages, regardless of whether
 * they've been read or not.  Added option to suppress kernel printk's
 * to the console.  Added hook for sending the console messages
 * elsewhere, in preparation for a serial line console (someday).
 * Ted Ts'o, 2/11/93.
 * Modified for sysctl support, 1/8/97, Chris Horn.
 * Fixed SMP synchronization, 08/08/99, Manfred Spraul
 *     manfred@colorfullife.com
 * Rewrote bits to get rid of console_lock
 *	01Mar01 Andrew Morton
 */

<<<<<<< HEAD
=======
#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

>>>>>>> upstream/android-13
#include <linux/kernel.h>
#include <linux/mm.h>
#include <linux/tty.h>
#include <linux/tty_driver.h>
#include <linux/console.h>
#include <linux/init.h>
#include <linux/jiffies.h>
#include <linux/nmi.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/delay.h>
#include <linux/smp.h>
#include <linux/security.h>
<<<<<<< HEAD
#include <linux/bootmem.h>
#include <linux/memblock.h>
#include <linux/syscalls.h>
#include <linux/crash_core.h>
#include <linux/kdb.h>
=======
#include <linux/memblock.h>
#include <linux/syscalls.h>
#include <linux/crash_core.h>
>>>>>>> upstream/android-13
#include <linux/ratelimit.h>
#include <linux/kmsg_dump.h>
#include <linux/syslog.h>
#include <linux/cpu.h>
#include <linux/rculist.h>
#include <linux/poll.h>
#include <linux/irq_work.h>
#include <linux/ctype.h>
#include <linux/uio.h>
#include <linux/sched/clock.h>
#include <linux/sched/debug.h>
#include <linux/sched/task_stack.h>
<<<<<<< HEAD
#include <linux/proc_fs.h>
=======
>>>>>>> upstream/android-13

#include <linux/uaccess.h>
#include <asm/sections.h>

#include <trace/events/initcall.h>
#define CREATE_TRACE_POINTS
#include <trace/events/printk.h>
<<<<<<< HEAD

#include "console_cmdline.h"
#include "braille.h"
#include "internal.h"
#include "mt-plat/mtk_printk_ctrl.h"
#include <mt-plat/aee.h>

#ifdef CONFIG_PRINTK_PREFIX_ENHANCE
static DEFINE_PER_CPU(char, printk_state);
=======
#undef CREATE_TRACE_POINTS
#include <trace/hooks/printk.h>
#include <trace/hooks/logbuf.h>

#include "printk_ringbuffer.h"
#include "console_cmdline.h"
#include "braille.h"
#include "internal.h"

#if IS_ENABLED(CONFIG_SEC_DEBUG) && IS_ENABLED(CONFIG_PRINTK_PROCESS)
#undef CONFIG_PRINTK_CALLER
>>>>>>> upstream/android-13
#endif

int console_printk[4] = {
	CONSOLE_LOGLEVEL_DEFAULT,	/* console_loglevel */
	MESSAGE_LOGLEVEL_DEFAULT,	/* default_message_loglevel */
	CONSOLE_LOGLEVEL_MIN,		/* minimum_console_loglevel */
	CONSOLE_LOGLEVEL_DEFAULT,	/* default_console_loglevel */
};
<<<<<<< HEAD
=======
EXPORT_SYMBOL_GPL(console_printk);
>>>>>>> upstream/android-13

atomic_t ignore_console_lock_warning __read_mostly = ATOMIC_INIT(0);
EXPORT_SYMBOL(ignore_console_lock_warning);

/*
 * Low level drivers may need that to know if they can schedule in
 * their unblank() callback or not. So let's export it.
 */
int oops_in_progress;
EXPORT_SYMBOL(oops_in_progress);

/*
 * console_sem protects the console_drivers list, and also
 * provides serialisation for access to the entire console
 * driver system.
 */
static DEFINE_SEMAPHORE(console_sem);
struct console *console_drivers;
EXPORT_SYMBOL_GPL(console_drivers);

<<<<<<< HEAD
=======
/*
 * System may need to suppress printk message under certain
 * circumstances, like after kernel panic happens.
 */
int __read_mostly suppress_printk;

>>>>>>> upstream/android-13
#ifdef CONFIG_LOCKDEP
static struct lockdep_map console_lock_dep_map = {
	.name = "console_lock"
};
#endif

enum devkmsg_log_bits {
	__DEVKMSG_LOG_BIT_ON = 0,
	__DEVKMSG_LOG_BIT_OFF,
	__DEVKMSG_LOG_BIT_LOCK,
};

enum devkmsg_log_masks {
	DEVKMSG_LOG_MASK_ON             = BIT(__DEVKMSG_LOG_BIT_ON),
	DEVKMSG_LOG_MASK_OFF            = BIT(__DEVKMSG_LOG_BIT_OFF),
	DEVKMSG_LOG_MASK_LOCK           = BIT(__DEVKMSG_LOG_BIT_LOCK),
};

/* Keep both the 'on' and 'off' bits clear, i.e. ratelimit by default: */
<<<<<<< HEAD
#define DEVKMSG_LOG_MASK_DEFAULT	DEVKMSG_LOG_MASK_ON
=======
#define DEVKMSG_LOG_MASK_DEFAULT	0
>>>>>>> upstream/android-13

static unsigned int __read_mostly devkmsg_log = DEVKMSG_LOG_MASK_DEFAULT;

static int __control_devkmsg(char *str)
{
<<<<<<< HEAD
	if (!str)
		return -EINVAL;

	if (!strncmp(str, "on", 2)) {
		devkmsg_log = DEVKMSG_LOG_MASK_ON;
		return 2;
	} else if (!strncmp(str, "off", 3)) {
		devkmsg_log = DEVKMSG_LOG_MASK_OFF;
		return 3;
	} else if (!strncmp(str, "ratelimit", 9)) {
		devkmsg_log = DEVKMSG_LOG_MASK_DEFAULT;
		return 9;
	}
=======
	size_t len;

	if (!str)
		return -EINVAL;

	len = str_has_prefix(str, "on");
	if (len) {
		devkmsg_log = DEVKMSG_LOG_MASK_ON;
		return len;
	}

	len = str_has_prefix(str, "off");
	if (len) {
		devkmsg_log = DEVKMSG_LOG_MASK_OFF;
		return len;
	}

	len = str_has_prefix(str, "ratelimit");
	if (len) {
		devkmsg_log = DEVKMSG_LOG_MASK_DEFAULT;
		return len;
	}

>>>>>>> upstream/android-13
	return -EINVAL;
}

static int __init control_devkmsg(char *str)
{
<<<<<<< HEAD
	if (__control_devkmsg(str) < 0)
		return 1;
=======
	if (__control_devkmsg(str) < 0) {
		pr_warn("printk.devkmsg: bad option string '%s'\n", str);
		return 1;
	}
>>>>>>> upstream/android-13

	/*
	 * Set sysctl string accordingly:
	 */
	if (devkmsg_log == DEVKMSG_LOG_MASK_ON)
		strcpy(devkmsg_log_str, "on");
	else if (devkmsg_log == DEVKMSG_LOG_MASK_OFF)
		strcpy(devkmsg_log_str, "off");
	/* else "ratelimit" which is set by default. */

	/*
	 * Sysctl cannot change it anymore. The kernel command line setting of
	 * this parameter is to force the setting to be permanent throughout the
	 * runtime of the system. This is a precation measure against userspace
	 * trying to be a smarta** and attempting to change it up on us.
	 */
	devkmsg_log |= DEVKMSG_LOG_MASK_LOCK;

<<<<<<< HEAD
	return 0;
=======
	return 1;
>>>>>>> upstream/android-13
}
__setup("printk.devkmsg=", control_devkmsg);

char devkmsg_log_str[DEVKMSG_STR_MAX_SIZE] = "ratelimit";

int devkmsg_sysctl_set_loglvl(struct ctl_table *table, int write,
<<<<<<< HEAD
			      void __user *buffer, size_t *lenp, loff_t *ppos)
=======
			      void *buffer, size_t *lenp, loff_t *ppos)
>>>>>>> upstream/android-13
{
	char old_str[DEVKMSG_STR_MAX_SIZE];
	unsigned int old;
	int err;

	if (write) {
		if (devkmsg_log & DEVKMSG_LOG_MASK_LOCK)
			return -EINVAL;

		old = devkmsg_log;
		strncpy(old_str, devkmsg_log_str, DEVKMSG_STR_MAX_SIZE);
	}

	err = proc_dostring(table, write, buffer, lenp, ppos);
	if (err)
		return err;

	if (write) {
		err = __control_devkmsg(devkmsg_log_str);

		/*
		 * Do not accept an unknown string OR a known string with
		 * trailing crap...
		 */
		if (err < 0 || (err + 1 != *lenp)) {

			/* ... and restore old setting. */
			devkmsg_log = old;
			strncpy(devkmsg_log_str, old_str, DEVKMSG_STR_MAX_SIZE);

			return -EINVAL;
		}
	}

	return 0;
}

<<<<<<< HEAD
/*
 * Number of registered extended console drivers.
 *
 * If extended consoles are present, in-kernel cont reassembly is disabled
 * and each fragment is stored as a separate log entry with proper
 * continuation flag so that every emitted message has full metadata.  This
 * doesn't change the result for regular consoles or /proc/kmsg.  For
 * /dev/kmsg, as long as the reader concatenates messages according to
 * consecutive continuation flags, the end result should be the same too.
 */
=======
/* Number of registered extended console drivers. */
>>>>>>> upstream/android-13
static int nr_ext_console_drivers;

/*
 * Helper macros to handle lockdep when locking/unlocking console_sem. We use
 * macros instead of functions so that _RET_IP_ contains useful information.
 */
#define down_console_sem() do { \
	down(&console_sem);\
	mutex_acquire(&console_lock_dep_map, 0, 0, _RET_IP_);\
} while (0)

static int __down_trylock_console_sem(unsigned long ip)
{
	int lock_failed;
	unsigned long flags;

	/*
	 * Here and in __up_console_sem() we need to be in safe mode,
	 * because spindump/WARN/etc from under console ->lock will
	 * deadlock in printk()->down_trylock_console_sem() otherwise.
	 */
	printk_safe_enter_irqsave(flags);
	lock_failed = down_trylock(&console_sem);
	printk_safe_exit_irqrestore(flags);

	if (lock_failed)
		return 1;
	mutex_acquire(&console_lock_dep_map, 0, 1, ip);
	return 0;
}
#define down_trylock_console_sem() __down_trylock_console_sem(_RET_IP_)

static void __up_console_sem(unsigned long ip)
{
	unsigned long flags;

<<<<<<< HEAD
	mutex_release(&console_lock_dep_map, 1, ip);
=======
	mutex_release(&console_lock_dep_map, ip);
>>>>>>> upstream/android-13

	printk_safe_enter_irqsave(flags);
	up(&console_sem);
	printk_safe_exit_irqrestore(flags);
}
#define up_console_sem() __up_console_sem(_RET_IP_)

/*
 * This is used for debugging the mess that is the VT code by
 * keeping track if we have the console semaphore held. It's
 * definitely not the perfect debug tool (we don't know if _WE_
 * hold it and are racing, but it helps tracking those weird code
 * paths in the console code where we end up in places I want
<<<<<<< HEAD
 * locked without the console sempahore held).
=======
 * locked without the console semaphore held).
>>>>>>> upstream/android-13
 */
static int console_locked, console_suspended;

/*
 * If exclusive_console is non-NULL then only this console is to be printed to.
 */
static struct console *exclusive_console;

/*
 *	Array of consoles built from command line options (console=)
 */

#define MAX_CMDLINECONSOLES 8

static struct console_cmdline console_cmdline[MAX_CMDLINECONSOLES];

static int preferred_console = -1;
<<<<<<< HEAD
=======
static bool has_preferred_console;
>>>>>>> upstream/android-13
int console_set_on_cmdline;
EXPORT_SYMBOL(console_set_on_cmdline);

/* Flag: console code may call schedule() */
static int console_may_schedule;

enum con_msg_format_flags {
	MSG_FORMAT_DEFAULT	= 0,
	MSG_FORMAT_SYSLOG	= (1 << 0),
};

static int console_msg_format = MSG_FORMAT_DEFAULT;

/*
<<<<<<< HEAD
 * The printk log buffer consists of a chain of concatenated variable
 * length records. Every record starts with a record header, containing
 * the overall length of the record.
 *
 * The heads to the first and last entry in the buffer, as well as the
 * sequence numbers of these entries are maintained when messages are
 * stored.
 *
 * If the heads indicate available messages, the length in the header
 * tells the start next message. A length == 0 for the next message
 * indicates a wrap-around to the beginning of the buffer.
 *
 * Every record carries the monotonic timestamp in microseconds, as well as
 * the standard userspace syslog level and syslog facility. The usual
 * kernel messages use LOG_KERN; userspace-injected messages always carry
 * a matching syslog facility, by default LOG_USER. The origin of every
 * message can be reliably determined that way.
 *
 * The human readable log message directly follows the message header. The
 * length of the message text is stored in the header, the stored message
 * is not terminated.
 *
 * Optionally, a message can carry a dictionary of properties (key/value pairs),
 * to provide userspace with a machine-readable message context.
=======
 * The printk log buffer consists of a sequenced collection of records, each
 * containing variable length message text. Every record also contains its
 * own meta-data (@info).
 *
 * Every record meta-data carries the timestamp in microseconds, as well as
 * the standard userspace syslog level and syslog facility. The usual kernel
 * messages use LOG_KERN; userspace-injected messages always carry a matching
 * syslog facility, by default LOG_USER. The origin of every message can be
 * reliably determined that way.
 *
 * The human readable log message of a record is available in @text, the
 * length of the message text in @text_len. The stored message is not
 * terminated.
 *
 * Optionally, a record can carry a dictionary of properties (key/value
 * pairs), to provide userspace with a machine-readable message context.
>>>>>>> upstream/android-13
 *
 * Examples for well-defined, commonly used property names are:
 *   DEVICE=b12:8               device identifier
 *                                b12:8         block dev_t
 *                                c127:3        char dev_t
 *                                n8            netdev ifindex
 *                                +sound:card0  subsystem:devname
 *   SUBSYSTEM=pci              driver-core subsystem name
 *
<<<<<<< HEAD
 * Valid characters in property names are [a-zA-Z0-9.-_]. The plain text value
 * follows directly after a '=' character. Every property is terminated by
 * a '\0' character. The last property is not terminated.
 *
 * Example of a message structure:
 *   0000  ff 8f 00 00 00 00 00 00      monotonic time in nsec
 *   0008  34 00                        record is 52 bytes long
 *   000a        0b 00                  text is 11 bytes long
 *   000c              1f 00            dictionary is 23 bytes long
 *   000e                    03 00      LOG_KERN (facility) LOG_ERR (level)
 *   0010  69 74 27 73 20 61 20 6c      "it's a l"
 *         69 6e 65                     "ine"
 *   001b           44 45 56 49 43      "DEVIC"
 *         45 3d 62 38 3a 32 00 44      "E=b8:2\0D"
 *         52 49 56 45 52 3d 62 75      "RIVER=bu"
 *         67                           "g"
 *   0032     00 00 00                  padding to next message header
 *
 * The 'struct printk_log' buffer header must never be directly exported to
=======
 * Valid characters in property names are [a-zA-Z0-9.-_]. Property names
 * and values are terminated by a '\0' character.
 *
 * Example of record values:
 *   record.text_buf                = "it's a line" (unterminated)
 *   record.info.seq                = 56
 *   record.info.ts_nsec            = 36863
 *   record.info.text_len           = 11
 *   record.info.facility           = 0 (LOG_KERN)
 *   record.info.flags              = 0
 *   record.info.level              = 3 (LOG_ERR)
 *   record.info.caller_id          = 299 (task 299)
 *   record.info.dev_info.subsystem = "pci" (terminated)
 *   record.info.dev_info.device    = "+pci:0000:00:01.0" (terminated)
 *
 * The 'struct printk_info' buffer must never be directly exported to
>>>>>>> upstream/android-13
 * userspace, it is a kernel-private implementation detail that might
 * need to be changed in the future, when the requirements change.
 *
 * /dev/kmsg exports the structured data in the following line format:
 *   "<level>,<sequnum>,<timestamp>,<contflag>[,additional_values, ... ];<message text>\n"
 *
 * Users of the export format should ignore possible additional values
 * separated by ',', and find the message after the ';' character.
 *
 * The optional key/value pairs are attached as continuation lines starting
 * with a space character and terminated by a newline. All possible
 * non-prinatable characters are escaped in the "\xff" notation.
 */

<<<<<<< HEAD
enum log_flags {
	LOG_NEWLINE	= 2,	/* text ended with a newline */
	LOG_PREFIX	= 4,	/* text started with a prefix */
	LOG_CONT	= 8,	/* text is a fragment of a continuation line */
};

struct printk_log {
	u64 ts_nsec;		/* timestamp in nanoseconds */
	u16 len;		/* length of entire record */
	u16 text_len;		/* length of text buffer */
	u16 dict_len;		/* length of dictionary buffer */
	u8 facility;		/* syslog facility */
	u8 flags:5;		/* internal record flags */
	u8 level:3;		/* syslog level */
#ifdef CONFIG_SEC_DEBUG_AUTO_COMMENT
	u8 for_auto_comment;
	u8 type_auto_comment;
#endif
}
#ifdef CONFIG_HAVE_EFFICIENT_UNALIGNED_ACCESS
__packed __aligned(4)
#endif
;

/*
 * The logbuf_lock protects kmsg buffer, indices, counters.  This can be taken
 * within the scheduler's rq lock. It must be released before calling
 * console_unlock() or anything else that might wake up a process.
 */
DEFINE_RAW_SPINLOCK(logbuf_lock);

/*
 * Helper macros to lock/unlock logbuf_lock and switch between
 * printk-safe/unsafe modes.
 */
#define logbuf_lock_irq()				\
	do {						\
		printk_safe_enter_irq();		\
		raw_spin_lock(&logbuf_lock);		\
	} while (0)

#define logbuf_unlock_irq()				\
	do {						\
		raw_spin_unlock(&logbuf_lock);		\
		printk_safe_exit_irq();			\
	} while (0)

#define logbuf_lock_irqsave(flags)			\
	do {						\
		printk_safe_enter_irqsave(flags);	\
		raw_spin_lock(&logbuf_lock);		\
	} while (0)

#define logbuf_unlock_irqrestore(flags)		\
	do {						\
		raw_spin_unlock(&logbuf_lock);		\
		printk_safe_exit_irqrestore(flags);	\
	} while (0)

#ifdef CONFIG_PRINTK
DECLARE_WAIT_QUEUE_HEAD(log_wait);
/* the next printk record to read by syslog(READ) or /proc/kmsg */
static u64 syslog_seq;
static u32 syslog_idx;
static size_t syslog_partial;

/* index and sequence number of the first record stored in the buffer */
static u64 log_first_seq;
static u32 log_first_idx;

/* index and sequence number of the next record to store in the buffer */
static u64 log_next_seq;
static u32 log_next_idx;

/* the next printk record to write to the console */
static u64 console_seq;
static u32 console_idx;
static u64 exclusive_console_stop_seq;

/* the next printk record to read after the last 'clear' command */
static u64 clear_seq;
static u32 clear_idx;

#define PREFIX_MAX		32
#define LOG_LINE_MAX		(1024 - PREFIX_MAX)
=======
/* syslog_lock protects syslog_* variables and write access to clear_seq. */
static DEFINE_MUTEX(syslog_lock);

#ifdef CONFIG_PRINTK
DECLARE_WAIT_QUEUE_HEAD(log_wait);
/* All 3 protected by @syslog_lock. */
/* the next printk record to read by syslog(READ) or /proc/kmsg */
static u64 syslog_seq;
static size_t syslog_partial;
static bool syslog_time;

/* All 3 protected by @console_sem. */
/* the next printk record to write to the console */
static u64 console_seq;
static u64 exclusive_console_stop_seq;
static unsigned long console_dropped;

struct latched_seq {
	seqcount_latch_t	latch;
	u64			val[2];
};

/*
 * The next printk record to read after the last 'clear' command. There are
 * two copies (updated with seqcount_latch) so that reads can locklessly
 * access a valid value. Writers are synchronized by @syslog_lock.
 */
static struct latched_seq clear_seq = {
	.latch		= SEQCNT_LATCH_ZERO(clear_seq.latch),
	.val[0]		= 0,
	.val[1]		= 0,
};

#if defined(CONFIG_PRINTK_CALLER) && defined(CONFIG_PRINTK_PROCESS)
#define PREFIX_MAX		72
#elif defined(CONFIG_PRINTK_PROCESS)
#define PREFIX_MAX		58
#elif defined(CONFIG_PRINTK_CALLER)
#define PREFIX_MAX		48
#else
#define PREFIX_MAX		32
#endif

/* the maximum size of a formatted record (i.e. with prefix added per line) */
#define CONSOLE_LOG_MAX		1024

/* the maximum size allowed to be reserved for a record */
#define LOG_LINE_MAX		(CONSOLE_LOG_MAX - PREFIX_MAX)
>>>>>>> upstream/android-13

#define LOG_LEVEL(v)		((v) & 0x07)
#define LOG_FACILITY(v)		((v) >> 3 & 0xff)

/* record buffer */
<<<<<<< HEAD
#define LOG_ALIGN __alignof__(struct printk_log)
=======
#define LOG_ALIGN __alignof__(unsigned long)
>>>>>>> upstream/android-13
#define __LOG_BUF_LEN (1 << CONFIG_LOG_BUF_SHIFT)
#define LOG_BUF_LEN_MAX (u32)(1 << 31)
static char __log_buf[__LOG_BUF_LEN] __aligned(LOG_ALIGN);
static char *log_buf = __log_buf;
static u32 log_buf_len = __LOG_BUF_LEN;

<<<<<<< HEAD
#ifdef CONFIG_PRINTK_MTK_UART_CONSOLE
/*
 * 0: uart printk enable
 * 1: uart printk disable
 * 2: uart printk always enable
 * 2 only set in lk phase by cmline
 */
int printk_disable_uart;

module_param_named(disable_uart, printk_disable_uart, int, 0644);
#endif


/* console duration detect */
#ifdef CONFIG_CONSOLE_LOCK_DURATION_DETECT
struct __conwrite_stat_struct {
	struct console *con; /* current console */
	u64 time_before_conwrite; /* the last record before write */
	u64 time_after_conwrite; /* the last record after write */
	char con_write_statbuf[512]; /* con write status buf*/
};
u64 time_con_write_ttyS, time_con_write_pstore;
u64 len_con_write_ttyS, len_con_write_pstore;
static struct __conwrite_stat_struct conwrite_stat_struct = {
	.con = NULL,
	.time_before_conwrite = 0,
	.time_after_conwrite = 0
};
unsigned long rem_nsec_con_write_ttyS, rem_nsec_con_write_pstore;
bool console_status_detected;
#endif

#ifdef CONFIG_LOG_TOO_MUCH_WARNING
int printk_too_much_enable;
static int detect_count =
	/*Default max lines per second*/
	CONFIG_LOG_TOO_MUCH_DETECT_COUNT;
static bool detect_count_change; /* detect_count change flag*/
#define DETECT_COUNT_MIN 100

#define DETECT_TIME 1000000000ULL /* 1s = 1000000000ns */
#define DELAY_TIME	(CONFIG_LOG_TOO_MUCH_DETECT_GAP*DETECT_TIME*60)

static u64 delta_time;
static u64 delta_count;
static u64 t_base;
static bool flag_toomuch;

static char *log_much;
static int log_count;
static u32 start_idx;
static u64 start_seq;
#define LOG_MUCH_PLUS_LEN	(1 << 17)

static void log_much_do_check_and_delay(struct printk_log *msg);

void set_detect_count(int count)
{
	if (count >= detect_count)
		detect_count = count;
	else {
		if (count < DETECT_COUNT_MIN)
			detect_count = DETECT_COUNT_MIN;
		else
			detect_count = count;
		detect_count_change = true;
	}
	pr_info("Printk too much criteria: %d  delay_flag: %d\n",
		detect_count, detect_count_change);
}

int get_detect_count(void)
{
	return detect_count;
}


void set_logtoomuch_enable(int value)
{
	printk_too_much_enable = value;
}


int get_logtoomuch_enable(void)
{
	return printk_too_much_enable;
}

#endif
=======
/*
 * Define the average message size. This only affects the number of
 * descriptors that will be available. Underestimating is better than
 * overestimating (too many available descriptors is better than not enough).
 */
#define PRB_AVGBITS 5	/* 32 character average length */

#if CONFIG_LOG_BUF_SHIFT <= PRB_AVGBITS
#error CONFIG_LOG_BUF_SHIFT value too small.
#endif
_DEFINE_PRINTKRB(printk_rb_static, CONFIG_LOG_BUF_SHIFT - PRB_AVGBITS,
		 PRB_AVGBITS, &__log_buf[0]);

static struct printk_ringbuffer printk_rb_dynamic;

static struct printk_ringbuffer *prb = &printk_rb_static;

/*
 * We do not locate this variable in CONFIG_SEC_DEBUG block, because
 * it can be parsed without sec_debug lego module. And it should be extern
 * to prevent optimization.
 */
struct {
	struct printk_ringbuffer **pprb;
	char name[];
} __prb_name = {
	.pprb = &prb,
	.name = "!PRINTK_RINGBUFFER!",
};
>>>>>>> upstream/android-13

/*
 * We cannot access per-CPU data (e.g. per-CPU flush irq_work) before
 * per_cpu_areas are initialised. This variable is set to true when
 * it's safe to access per-CPU data.
 */
static bool __printk_percpu_data_ready __read_mostly;

bool printk_percpu_data_ready(void)
{
	return __printk_percpu_data_ready;
}

<<<<<<< HEAD
=======
/* Must be called under syslog_lock. */
static void latched_seq_write(struct latched_seq *ls, u64 val)
{
	raw_write_seqcount_latch(&ls->latch);
	ls->val[0] = val;
	raw_write_seqcount_latch(&ls->latch);
	ls->val[1] = val;
}

/* Can be called from any context. */
static u64 latched_seq_read_nolock(struct latched_seq *ls)
{
	unsigned int seq;
	unsigned int idx;
	u64 val;

	do {
		seq = raw_read_seqcount_latch(&ls->latch);
		idx = seq & 0x1;
		val = ls->val[idx];
	} while (read_seqcount_latch_retry(&ls->latch, seq));

	return val;
}

>>>>>>> upstream/android-13
/* Return log buffer address */
char *log_buf_addr_get(void)
{
	return log_buf;
}
EXPORT_SYMBOL_GPL(log_buf_addr_get);

/* Return log buffer size */
u32 log_buf_len_get(void)
{
	return log_buf_len;
}
EXPORT_SYMBOL_GPL(log_buf_len_get);

<<<<<<< HEAD
/* human readable text of the record */
static char *log_text(const struct printk_log *msg)
{
	return (char *)msg + sizeof(struct printk_log);
}

/* optional key/value pair dictionary attached to the record */
static char *log_dict(const struct printk_log *msg)
{
	return (char *)msg + sizeof(struct printk_log) + msg->text_len;
}

/* get record by index; idx must point to valid msg */
static struct printk_log *log_from_idx(u32 idx)
{
	struct printk_log *msg = (struct printk_log *)(log_buf + idx);

	/*
	 * A length == 0 record is the end of buffer marker. Wrap around and
	 * read the message at the start of the buffer.
	 */
	if (!msg->len)
		return (struct printk_log *)log_buf;
	return msg;
}

/* get next record; idx must point to valid msg */
static u32 log_next(u32 idx)
{
	struct printk_log *msg = (struct printk_log *)(log_buf + idx);

	/* length == 0 indicates the end of the buffer; wrap */
	/*
	 * A length == 0 record is the end of buffer marker. Wrap around and
	 * read the message at the start of the buffer as *this* one, and
	 * return the one after that.
	 */
	if (!msg->len) {
		msg = (struct printk_log *)log_buf;
		return msg->len;
	}
	return idx + msg->len;
}

/*
 * Check whether there is enough free space for the given message.
 *
 * The same values of first_idx and next_idx mean that the buffer
 * is either empty or full.
 *
 * If the buffer is empty, we must respect the position of the indexes.
 * They cannot be reset to the beginning of the buffer.
 */
static int logbuf_has_space(u32 msg_size, bool empty)
{
	u32 free;

	if (log_next_idx > log_first_idx || empty)
		free = max(log_buf_len - log_next_idx, log_first_idx);
	else
		free = log_first_idx - log_next_idx;

	/*
	 * We need space also for an empty header that signalizes wrapping
	 * of the buffer.
	 */
	return free >= msg_size + sizeof(struct printk_log);
}

static int log_make_free_space(u32 msg_size)
{
	while (log_first_seq < log_next_seq &&
	       !logbuf_has_space(msg_size, false)) {
		/* drop old messages until we have enough contiguous space */
		log_first_idx = log_next(log_first_idx);
		log_first_seq++;
	}

	if (clear_seq < log_first_seq) {
		clear_seq = log_first_seq;
		clear_idx = log_first_idx;
	}

	/* sequence numbers are equal, so the log buffer is empty */
	if (logbuf_has_space(msg_size, log_first_seq == log_next_seq))
		return 0;

	return -ENOMEM;
}

/* compute the message size including the padding bytes */
static u32 msg_used_size(u16 text_len, u16 dict_len, u32 *pad_len)
{
	u32 size;

	size = sizeof(struct printk_log) + text_len + dict_len;
	*pad_len = (-size) & (LOG_ALIGN - 1);
	size += *pad_len;

	return size;
}

#ifdef CONFIG_SEC_DEBUG_AUTO_COMMENT
static void (*func_hook_auto_comm)(int type, const char *buf, size_t size);

void register_set_auto_comm_buf(void (*func)(int type, const char *buf, size_t size))
=======
#if IS_ENABLED(CONFIG_SEC_DEBUG_AUTO_COMMENT)
static void (*func_hook_auto_comm)(int type, struct printk_ringbuffer *rb,
					struct printk_record *r);
static DEFINE_SPINLOCK(auto_comm_lock);

void register_set_auto_comm_buf(void (*func)(int type,
					struct printk_ringbuffer *rb,
					struct printk_record *r))
>>>>>>> upstream/android-13
{
	func_hook_auto_comm = func;
}
#endif

<<<<<<< HEAD
#ifdef CONFIG_SEC_DEBUG_INIT_LOG
static void (*func_hook_init_log)(const char *buf, size_t size);
void register_init_log_hook_func(void (*func)(const char *buf, size_t size))
{
	func_hook_init_log = func;
}
#endif

#ifdef CONFIG_SEC_EXT
static size_t hook_size;
static char hook_text[LOG_LINE_MAX + PREFIX_MAX];
static void (*log_text_hook)(const char *buf, size_t size);
static size_t msg_print_text(const struct printk_log *msg,
			     bool syslog, char *buf, size_t size);

void register_log_text_hook(void (*func)(const char *buf, size_t size))
{
	unsigned long flags;

	logbuf_lock_irqsave(flags);
	/*
	 * In register hooking function,  we should check messages already
	 * printed on log_buf. If so, they will be copyied to backup
	 * exynos log buffer
	 * */
	if (log_first_seq != log_next_seq) {
		unsigned int step_seq, step_idx, start, end;
		struct printk_log *msg;
		start = log_first_seq;
		end = log_next_seq;
		step_idx = log_first_idx;
		for (step_seq = start; step_seq < end; step_seq++) {
			msg = (struct printk_log *)(log_buf + step_idx);
			hook_size = msg_print_text(msg,
					true, hook_text, LOG_LINE_MAX + PREFIX_MAX);
			func(hook_text, hook_size);
			step_idx = log_next(step_idx);
		}
	}
	log_text_hook = func;
	logbuf_unlock_irqrestore(flags);
}
EXPORT_SYMBOL(register_log_text_hook);
#endif

=======
>>>>>>> upstream/android-13
/*
 * Define how much of the log buffer we could take at maximum. The value
 * must be greater than two. Note that only half of the buffer is available
 * when the index points to the middle.
 */
#define MAX_LOG_TAKE_PART 4
static const char trunc_msg[] = "<truncated>";

<<<<<<< HEAD
static u32 truncate_msg(u16 *text_len, u16 *trunc_msg_len,
			u16 *dict_len, u32 *pad_len)
=======
static void truncate_msg(u16 *text_len, u16 *trunc_msg_len)
>>>>>>> upstream/android-13
{
	/*
	 * The message should not take the whole buffer. Otherwise, it might
	 * get removed too soon.
	 */
	u32 max_text_len = log_buf_len / MAX_LOG_TAKE_PART;
<<<<<<< HEAD
	if (*text_len > max_text_len)
		*text_len = max_text_len;
	/* enable the warning message */
	*trunc_msg_len = strlen(trunc_msg);
	/* disable the "dict" completely */
	*dict_len = 0;
	/* compute the size again, count also the warning message */
	return msg_used_size(*text_len + *trunc_msg_len, 0, pad_len);
}

/* insert record into the buffer, discard old ones, update heads */
static int log_store(int facility, int level,
		     enum log_flags flags, u64 ts_nsec,
		     const char *dict, u16 dict_len,
		     const char *text, u16 text_len)
{
	struct printk_log *msg;
	u32 size, pad_len;
	u16 trunc_msg_len = 0;

#ifdef CONFIG_PRINTK_PREFIX_ENHANCE
	int this_cpu = smp_processor_id();
	char state = this_cpu_read(printk_state);
	char tbuf[50];
	unsigned int tlen = 0;
#endif

#ifdef CONFIG_LOG_TOO_MUCH_WARNING
	static u64 start_ts_nsec;
	static bool initialized;
#endif

#ifdef CONFIG_PRINTK_PREFIX_ENHANCE
		if (state == 0) {
			this_cpu_write(printk_state, ' ');
			state = ' ';
		}
		if (!(flags & LOG_CONT)) {
			if (console_suspended == 0)
				tlen = snprintf(tbuf, sizeof(tbuf),
					"%c(%x)[%d:%s]", state, this_cpu,
					current->pid, current->comm);
			else
				tlen = snprintf(tbuf, sizeof(tbuf), "%c(%x)",
					state, this_cpu);
		}
#endif

		/* number of '\0' padding bytes to next message */
#ifdef CONFIG_PRINTK_PREFIX_ENHANCE
		size = msg_used_size(text_len + tlen, dict_len, &pad_len);
#else
		size = msg_used_size(text_len, dict_len, &pad_len);
#endif

	if (log_make_free_space(size)) {
		/* truncate the message if it is too long for empty buffer */
		size = truncate_msg(&text_len, &trunc_msg_len,
				    &dict_len, &pad_len);
		/* survive when the log buffer is too small for trunc_msg */
		if (log_make_free_space(size))
			return 0;
	}

	if (log_next_idx + size + sizeof(struct printk_log) > log_buf_len) {
		/*
		 * This message + an additional empty header does not fit
		 * at the end of the buffer. Add an empty header with len == 0
		 * to signify a wrap around.
		 */
		memset(log_buf + log_next_idx, 0, sizeof(struct printk_log));
		log_next_idx = 0;
	}

	/* fill message */
	msg = (struct printk_log *)(log_buf + log_next_idx);
#ifdef CONFIG_PRINTK_PREFIX_ENHANCE
	memcpy(log_text(msg), tbuf, tlen);
	if (tlen + text_len > LOG_LINE_MAX)
		text_len = LOG_LINE_MAX - tlen;

	memcpy(log_text(msg) + tlen, text, text_len);
	text_len += tlen;
#else
	memcpy(log_text(msg), text, text_len);
#endif
	msg->text_len = text_len;
	if (trunc_msg_len) {
		memcpy(log_text(msg) + text_len, trunc_msg, trunc_msg_len);
		msg->text_len += trunc_msg_len;
	}
	memcpy(log_dict(msg), dict, dict_len);
	msg->dict_len = dict_len;
	msg->facility = facility;
	
#ifdef CONFIG_SEC_DEBUG_AUTO_COMMENT
	msg->for_auto_comment = (level / 10 == 9) ? 1 : 0;
	msg->type_auto_comment = (level / 10 == 9) ? level - LOGLEVEL_PR_AUTO_BASE : 0;
	level = (msg->for_auto_comment) ? 0 : level;
#endif

	msg->level = level & 7;
	msg->flags = flags & 0x1f;
	if (ts_nsec > 0)
		msg->ts_nsec = ts_nsec;
	else
		msg->ts_nsec = local_clock();
	memset(log_dict(msg) + dict_len, 0, pad_len);
	msg->len = size;

#ifdef CONFIG_SEC_EXT
	if (log_text_hook) {
		hook_size = msg_print_text(msg,
				true, hook_text, LOG_LINE_MAX + PREFIX_MAX);
		log_text_hook(hook_text, hook_size);

#ifdef CONFIG_SEC_DEBUG_AUTO_COMMENT
		if (msg->for_auto_comment && func_hook_auto_comm)
			func_hook_auto_comm(msg->type_auto_comment, hook_text, hook_size);
#endif

#ifdef CONFIG_SEC_DEBUG_INIT_LOG
		if (task_pid_nr(current) == 1 && func_hook_init_log) {
			func_hook_init_log(hook_text, hook_size);
		}
#endif
	}
#endif

	/* insert message */
	log_next_idx += msg->len;
	log_next_seq++;

	/* printk too much detect */
#ifdef CONFIG_LOG_TOO_MUCH_WARNING
	if (printk_too_much_enable == 1) {
		if (detect_count_change) {
			detect_count_change = false;
			t_base = msg->ts_nsec + DETECT_TIME*15;
		}
		if (flag_toomuch == false && t_base < msg->ts_nsec) {
			if (!initialized) {
				start_ts_nsec = msg->ts_nsec;
				start_idx = log_next_idx - msg->len;
				start_seq = log_next_seq - 1;
				initialized = true;
			}
			/* old messages were dropped */
			if (start_seq < log_first_seq) {
				initialized = false;
				start_seq = log_first_seq;
				start_idx = log_first_idx;
				delta_time = msg->ts_nsec
					- log_from_idx(start_idx)->ts_nsec;
				delta_count = log_next_seq - start_seq;
				log_much_do_check_and_delay(msg);
			} else {
				delta_time = msg->ts_nsec - start_ts_nsec;
				delta_count = log_next_seq - start_seq;
				/* check every 5 seconds */
				if (delta_time > DETECT_TIME * 5) {
					initialized = false;
					log_much_do_check_and_delay(msg);
				}
			}
		}
	}
#endif

	return msg->text_len;
=======

	if (*text_len > max_text_len)
		*text_len = max_text_len;

	/* enable the warning message (if there is room) */
	*trunc_msg_len = strlen(trunc_msg);
	if (*text_len >= *trunc_msg_len)
		*text_len -= *trunc_msg_len;
	else
		*trunc_msg_len = 0;
>>>>>>> upstream/android-13
}

int dmesg_restrict = IS_ENABLED(CONFIG_SECURITY_DMESG_RESTRICT);

static int syslog_action_restricted(int type)
{
	if (dmesg_restrict)
		return 1;
	/*
	 * Unless restricted, we allow "read all" and "get buffer size"
	 * for everybody.
	 */
	return type != SYSLOG_ACTION_READ_ALL &&
	       type != SYSLOG_ACTION_SIZE_BUFFER;
}

static int check_syslog_permissions(int type, int source)
{
	/*
	 * If this is from /proc/kmsg and we've already opened it, then we've
	 * already done the capabilities checks at open time.
	 */
	if (source == SYSLOG_FROM_PROC && type != SYSLOG_ACTION_OPEN)
		goto ok;

	if (syslog_action_restricted(type)) {
		if (capable(CAP_SYSLOG))
			goto ok;
		/*
		 * For historical reasons, accept CAP_SYS_ADMIN too, with
		 * a warning.
		 */
		if (capable(CAP_SYS_ADMIN)) {
			pr_warn_once("%s (%d): Attempt to access syslog with "
				     "CAP_SYS_ADMIN but no CAP_SYSLOG "
				     "(deprecated).\n",
				 current->comm, task_pid_nr(current));
			goto ok;
		}
		return -EPERM;
	}
ok:
	return security_syslog(type);
}

static void append_char(char **pp, char *e, char c)
{
	if (*pp < e)
		*(*pp)++ = c;
}

<<<<<<< HEAD
static ssize_t msg_print_ext_header(char *buf, size_t size,
				    struct printk_log *msg, u64 seq)
{
	u64 ts_usec = msg->ts_nsec;

	do_div(ts_usec, 1000);

	return scnprintf(buf, size, "%u,%llu,%llu,%c;",
		       (msg->facility << 3) | msg->level, seq, ts_usec,
		       msg->flags & LOG_CONT ? 'c' : '-');
}

static ssize_t msg_print_ext_body(char *buf, size_t size,
				  char *dict, size_t dict_len,
				  char *text, size_t text_len)
=======
static ssize_t info_print_ext_header(char *buf, size_t size,
				     struct printk_info *info)
{
	u64 ts_usec = info->ts_nsec;
	char caller[20];
#ifdef CONFIG_PRINTK_CALLER
	int vh_ret = 0;
	u32 id = info->caller_id;

	trace_android_vh_printk_ext_header(caller, sizeof(caller), id, &vh_ret);

	if (!vh_ret)
		snprintf(caller, sizeof(caller), ",caller=%c%u",
			 id & 0x80000000 ? 'C' : 'T', id & ~0x80000000);
#else
	caller[0] = '\0';
#endif

	do_div(ts_usec, 1000);

	return scnprintf(buf, size, "%u,%llu,%llu,%c%s;",
			 (info->facility << 3) | info->level, info->seq,
			 ts_usec, info->flags & LOG_CONT ? 'c' : '-', caller);
}

static ssize_t msg_add_ext_text(char *buf, size_t size,
				const char *text, size_t text_len,
				unsigned char endc)
>>>>>>> upstream/android-13
{
	char *p = buf, *e = buf + size;
	size_t i;

	/* escape non-printable characters */
	for (i = 0; i < text_len; i++) {
		unsigned char c = text[i];

		if (c < ' ' || c >= 127 || c == '\\')
			p += scnprintf(p, e - p, "\\x%02x", c);
		else
			append_char(&p, e, c);
	}
<<<<<<< HEAD
	append_char(&p, e, '\n');

	if (dict_len) {
		bool line = true;

		for (i = 0; i < dict_len; i++) {
			unsigned char c = dict[i];

			if (line) {
				append_char(&p, e, ' ');
				line = false;
			}

			if (c == '\0') {
				append_char(&p, e, '\n');
				line = true;
				continue;
			}

			if (c < ' ' || c >= 127 || c == '\\') {
				p += scnprintf(p, e - p, "\\x%02x", c);
				continue;
			}

			append_char(&p, e, c);
		}
		append_char(&p, e, '\n');
	}
=======
	append_char(&p, e, endc);
>>>>>>> upstream/android-13

	return p - buf;
}

<<<<<<< HEAD
/* /dev/kmsg - userspace message inject/listen interface */
struct devkmsg_user {
	u64 seq;
	u32 idx;
	struct ratelimit_state rs;
	struct mutex lock;
	char buf[CONSOLE_EXT_LOG_MAX];
};

=======
static ssize_t msg_add_dict_text(char *buf, size_t size,
				 const char *key, const char *val)
{
	size_t val_len = strlen(val);
	ssize_t len;

	if (!val_len)
		return 0;

	len = msg_add_ext_text(buf, size, "", 0, ' ');	/* dict prefix */
	len += msg_add_ext_text(buf + len, size - len, key, strlen(key), '=');
	len += msg_add_ext_text(buf + len, size - len, val, val_len, '\n');

	return len;
}

static ssize_t msg_print_ext_body(char *buf, size_t size,
				  char *text, size_t text_len,
				  struct dev_printk_info *dev_info)
{
	ssize_t len;

	len = msg_add_ext_text(buf, size, text, text_len, '\n');

	if (!dev_info)
		goto out;

	len += msg_add_dict_text(buf + len, size - len, "SUBSYSTEM",
				 dev_info->subsystem);
	len += msg_add_dict_text(buf + len, size - len, "DEVICE",
				 dev_info->device);
out:
	return len;
}

/* /dev/kmsg - userspace message inject/listen interface */
struct devkmsg_user {
	atomic64_t seq;
	struct ratelimit_state rs;
	struct mutex lock;
	char buf[CONSOLE_EXT_LOG_MAX];

	struct printk_info info;
	char text_buf[CONSOLE_EXT_LOG_MAX];
	struct printk_record record;
};

static __printf(3, 4) __cold
int devkmsg_emit(int facility, int level, const char *fmt, ...)
{
	va_list args;
	int r;

	va_start(args, fmt);
	r = vprintk_emit(facility, level, NULL, fmt, args);
	va_end(args);

	return r;
}

>>>>>>> upstream/android-13
static ssize_t devkmsg_write(struct kiocb *iocb, struct iov_iter *from)
{
	char *buf, *line;
	int level = default_message_loglevel;
	int facility = 1;	/* LOG_USER */
	struct file *file = iocb->ki_filp;
	struct devkmsg_user *user = file->private_data;
	size_t len = iov_iter_count(from);
	ssize_t ret = len;

	if (!user || len > LOG_LINE_MAX)
		return -EINVAL;

	/* Ignore when user logging is disabled. */
	if (devkmsg_log & DEVKMSG_LOG_MASK_OFF)
		return len;

	/* Ratelimit when not explicitly enabled. */
	if (!(devkmsg_log & DEVKMSG_LOG_MASK_ON)) {
		if (!___ratelimit(&user->rs, current->comm))
			return ret;
	}

	buf = kmalloc(len+1, GFP_KERNEL);
	if (buf == NULL)
		return -ENOMEM;

	buf[len] = '\0';
	if (!copy_from_iter_full(buf, len, from)) {
		kfree(buf);
		return -EFAULT;
	}

	/*
	 * Extract and skip the syslog prefix <[0-9]*>. Coming from userspace
	 * the decimal value represents 32bit, the lower 3 bit are the log
	 * level, the rest are the log facility.
	 *
	 * If no prefix or no userspace facility is specified, we
	 * enforce LOG_USER, to be able to reliably distinguish
	 * kernel-generated messages from userspace-injected ones.
	 */
	line = buf;
	if (line[0] == '<') {
		char *endp = NULL;
		unsigned int u;

		u = simple_strtoul(line + 1, &endp, 10);
		if (endp && endp[0] == '>') {
			level = LOG_LEVEL(u);
			if (LOG_FACILITY(u) != 0)
				facility = LOG_FACILITY(u);
			endp++;
<<<<<<< HEAD
			len -= endp - line;
=======
>>>>>>> upstream/android-13
			line = endp;
		}
	}

<<<<<<< HEAD
	printk_emit(facility, level, NULL, 0, "%s", line);
=======
	devkmsg_emit(facility, level, "%s", line);
>>>>>>> upstream/android-13
	kfree(buf);
	return ret;
}

static ssize_t devkmsg_read(struct file *file, char __user *buf,
			    size_t count, loff_t *ppos)
{
	struct devkmsg_user *user = file->private_data;
<<<<<<< HEAD
	struct printk_log *msg;
=======
	struct printk_record *r = &user->record;
>>>>>>> upstream/android-13
	size_t len;
	ssize_t ret;

	if (!user)
		return -EBADF;

	ret = mutex_lock_interruptible(&user->lock);
	if (ret)
		return ret;

<<<<<<< HEAD
	logbuf_lock_irq();
	while (user->seq == log_next_seq) {
		if (file->f_flags & O_NONBLOCK) {
			ret = -EAGAIN;
			logbuf_unlock_irq();
			goto out;
		}

		logbuf_unlock_irq();
		ret = wait_event_interruptible(log_wait,
					       user->seq != log_next_seq);
		if (ret)
			goto out;
		logbuf_lock_irq();
	}

	if (user->seq < log_first_seq) {
		/* our last seen message is gone, return error and reset */
		user->idx = log_first_idx;
		user->seq = log_first_seq;
		ret = -EPIPE;
		logbuf_unlock_irq();
		goto out;
	}

	msg = log_from_idx(user->idx);
	len = msg_print_ext_header(user->buf, sizeof(user->buf),
				   msg, user->seq);
	len += msg_print_ext_body(user->buf + len, sizeof(user->buf) - len,
				  log_dict(msg), msg->dict_len,
				  log_text(msg), msg->text_len);

	user->idx = log_next(user->idx);
	user->seq++;
	logbuf_unlock_irq();
=======
	if (!prb_read_valid(prb, atomic64_read(&user->seq), r)) {
		if (file->f_flags & O_NONBLOCK) {
			ret = -EAGAIN;
			goto out;
		}

		ret = wait_event_interruptible(log_wait,
				prb_read_valid(prb, atomic64_read(&user->seq), r));
		if (ret)
			goto out;
	}

	if (r->info->seq != atomic64_read(&user->seq)) {
		/* our last seen message is gone, return error and reset */
		atomic64_set(&user->seq, r->info->seq);
		ret = -EPIPE;
		goto out;
	}

	len = info_print_ext_header(user->buf, sizeof(user->buf), r->info);
	len += msg_print_ext_body(user->buf + len, sizeof(user->buf) - len,
				  &r->text_buf[0], r->info->text_len,
				  &r->info->dev_info);

	atomic64_set(&user->seq, r->info->seq + 1);
>>>>>>> upstream/android-13

	if (len > count) {
		ret = -EINVAL;
		goto out;
	}

	if (copy_to_user(buf, user->buf, len)) {
		ret = -EFAULT;
		goto out;
	}
	ret = len;
out:
	mutex_unlock(&user->lock);
	return ret;
}

<<<<<<< HEAD
=======
/*
 * Be careful when modifying this function!!!
 *
 * Only few operations are supported because the device works only with the
 * entire variable length messages (records). Non-standard values are
 * returned in the other cases and has been this way for quite some time.
 * User space applications might depend on this behavior.
 */
>>>>>>> upstream/android-13
static loff_t devkmsg_llseek(struct file *file, loff_t offset, int whence)
{
	struct devkmsg_user *user = file->private_data;
	loff_t ret = 0;

	if (!user)
		return -EBADF;
	if (offset)
		return -ESPIPE;

<<<<<<< HEAD
	logbuf_lock_irq();
	switch (whence) {
	case SEEK_SET:
		/* the first record */
		user->idx = log_first_idx;
		user->seq = log_first_seq;
=======
	switch (whence) {
	case SEEK_SET:
		/* the first record */
		atomic64_set(&user->seq, prb_first_valid_seq(prb));
>>>>>>> upstream/android-13
		break;
	case SEEK_DATA:
		/*
		 * The first record after the last SYSLOG_ACTION_CLEAR,
		 * like issued by 'dmesg -c'. Reading /dev/kmsg itself
		 * changes no global state, and does not clear anything.
		 */
<<<<<<< HEAD
		user->idx = clear_idx;
		user->seq = clear_seq;
		break;
	case SEEK_END:
		/* after the last record */
		user->idx = log_next_idx;
		user->seq = log_next_seq;
=======
		atomic64_set(&user->seq, latched_seq_read_nolock(&clear_seq));
		break;
	case SEEK_END:
		/* after the last record */
		atomic64_set(&user->seq, prb_next_seq(prb));
>>>>>>> upstream/android-13
		break;
	default:
		ret = -EINVAL;
	}
<<<<<<< HEAD
	logbuf_unlock_irq();
=======
>>>>>>> upstream/android-13
	return ret;
}

static __poll_t devkmsg_poll(struct file *file, poll_table *wait)
{
	struct devkmsg_user *user = file->private_data;
<<<<<<< HEAD
=======
	struct printk_info info;
>>>>>>> upstream/android-13
	__poll_t ret = 0;

	if (!user)
		return EPOLLERR|EPOLLNVAL;

	poll_wait(file, &log_wait, wait);

<<<<<<< HEAD
	logbuf_lock_irq();
	if (user->seq < log_next_seq) {
		/* return error when data has vanished underneath us */
		if (user->seq < log_first_seq)
=======
	if (prb_read_valid_info(prb, atomic64_read(&user->seq), &info, NULL)) {
		/* return error when data has vanished underneath us */
		if (info.seq != atomic64_read(&user->seq))
>>>>>>> upstream/android-13
			ret = EPOLLIN|EPOLLRDNORM|EPOLLERR|EPOLLPRI;
		else
			ret = EPOLLIN|EPOLLRDNORM;
	}
<<<<<<< HEAD
	logbuf_unlock_irq();
=======
>>>>>>> upstream/android-13

	return ret;
}

static int devkmsg_open(struct inode *inode, struct file *file)
{
	struct devkmsg_user *user;
	int err;

	if (devkmsg_log & DEVKMSG_LOG_MASK_OFF)
		return -EPERM;

	/* write-only does not need any file context */
	if ((file->f_flags & O_ACCMODE) != O_WRONLY) {
		err = check_syslog_permissions(SYSLOG_ACTION_READ_ALL,
					       SYSLOG_FROM_READER);
		if (err)
			return err;
	}

	user = kmalloc(sizeof(struct devkmsg_user), GFP_KERNEL);
	if (!user)
		return -ENOMEM;

	ratelimit_default_init(&user->rs);
	ratelimit_set_flags(&user->rs, RATELIMIT_MSG_ON_RELEASE);

	mutex_init(&user->lock);

<<<<<<< HEAD
	logbuf_lock_irq();
	user->idx = log_first_idx;
	user->seq = log_first_seq;
	logbuf_unlock_irq();
=======
	prb_rec_init_rd(&user->record, &user->info,
			&user->text_buf[0], sizeof(user->text_buf));

	atomic64_set(&user->seq, prb_first_valid_seq(prb));
>>>>>>> upstream/android-13

	file->private_data = user;
	return 0;
}

static int devkmsg_release(struct inode *inode, struct file *file)
{
	struct devkmsg_user *user = file->private_data;

	if (!user)
		return 0;

	ratelimit_state_exit(&user->rs);

	mutex_destroy(&user->lock);
	kfree(user);
	return 0;
}

const struct file_operations kmsg_fops = {
	.open = devkmsg_open,
	.read = devkmsg_read,
	.write_iter = devkmsg_write,
	.llseek = devkmsg_llseek,
	.poll = devkmsg_poll,
	.release = devkmsg_release,
};

#ifdef CONFIG_CRASH_CORE
/*
 * This appends the listed symbols to /proc/vmcore
 *
 * /proc/vmcore is used by various utilities, like crash and makedumpfile to
 * obtain access to symbols that are otherwise very difficult to locate.  These
 * symbols are specifically used so that utilities can access and extract the
 * dmesg log from a vmcore file after a crash.
 */
void log_buf_vmcoreinfo_setup(void)
{
<<<<<<< HEAD
	VMCOREINFO_SYMBOL(log_buf);
	VMCOREINFO_SYMBOL(log_buf_len);
	VMCOREINFO_SYMBOL(log_first_idx);
	VMCOREINFO_SYMBOL(clear_idx);
	VMCOREINFO_SYMBOL(log_next_idx);
	/*
	 * Export struct printk_log size and field offsets. User space tools can
	 * parse it and detect any changes to structure down the line.
	 */
	VMCOREINFO_STRUCT_SIZE(printk_log);
	VMCOREINFO_OFFSET(printk_log, ts_nsec);
	VMCOREINFO_OFFSET(printk_log, len);
	VMCOREINFO_OFFSET(printk_log, text_len);
	VMCOREINFO_OFFSET(printk_log, dict_len);
=======
	struct dev_printk_info *dev_info = NULL;

	VMCOREINFO_SYMBOL(prb);
	VMCOREINFO_SYMBOL(printk_rb_static);
	VMCOREINFO_SYMBOL(clear_seq);

	/*
	 * Export struct size and field offsets. User space tools can
	 * parse it and detect any changes to structure down the line.
	 */

	VMCOREINFO_STRUCT_SIZE(printk_ringbuffer);
	VMCOREINFO_OFFSET(printk_ringbuffer, desc_ring);
	VMCOREINFO_OFFSET(printk_ringbuffer, text_data_ring);
	VMCOREINFO_OFFSET(printk_ringbuffer, fail);

	VMCOREINFO_STRUCT_SIZE(prb_desc_ring);
	VMCOREINFO_OFFSET(prb_desc_ring, count_bits);
	VMCOREINFO_OFFSET(prb_desc_ring, descs);
	VMCOREINFO_OFFSET(prb_desc_ring, infos);
	VMCOREINFO_OFFSET(prb_desc_ring, head_id);
	VMCOREINFO_OFFSET(prb_desc_ring, tail_id);

	VMCOREINFO_STRUCT_SIZE(prb_desc);
	VMCOREINFO_OFFSET(prb_desc, state_var);
	VMCOREINFO_OFFSET(prb_desc, text_blk_lpos);

	VMCOREINFO_STRUCT_SIZE(prb_data_blk_lpos);
	VMCOREINFO_OFFSET(prb_data_blk_lpos, begin);
	VMCOREINFO_OFFSET(prb_data_blk_lpos, next);

	VMCOREINFO_STRUCT_SIZE(printk_info);
	VMCOREINFO_OFFSET(printk_info, seq);
	VMCOREINFO_OFFSET(printk_info, ts_nsec);
	VMCOREINFO_OFFSET(printk_info, text_len);
	VMCOREINFO_OFFSET(printk_info, caller_id);
	VMCOREINFO_OFFSET(printk_info, dev_info);

	VMCOREINFO_STRUCT_SIZE(dev_printk_info);
	VMCOREINFO_OFFSET(dev_printk_info, subsystem);
	VMCOREINFO_LENGTH(printk_info_subsystem, sizeof(dev_info->subsystem));
	VMCOREINFO_OFFSET(dev_printk_info, device);
	VMCOREINFO_LENGTH(printk_info_device, sizeof(dev_info->device));

	VMCOREINFO_STRUCT_SIZE(prb_data_ring);
	VMCOREINFO_OFFSET(prb_data_ring, size_bits);
	VMCOREINFO_OFFSET(prb_data_ring, data);
	VMCOREINFO_OFFSET(prb_data_ring, head_lpos);
	VMCOREINFO_OFFSET(prb_data_ring, tail_lpos);

	VMCOREINFO_SIZE(atomic_long_t);
	VMCOREINFO_TYPE_OFFSET(atomic_long_t, counter);

	VMCOREINFO_STRUCT_SIZE(latched_seq);
	VMCOREINFO_OFFSET(latched_seq, val);
>>>>>>> upstream/android-13
}
#endif

/* requested log_buf_len from kernel cmdline */
static unsigned long __initdata new_log_buf_len;

/* we practice scaling the ring buffer by powers of 2 */
static void __init log_buf_len_update(u64 size)
{
	if (size > (u64)LOG_BUF_LEN_MAX) {
		size = (u64)LOG_BUF_LEN_MAX;
		pr_err("log_buf over 2G is not supported.\n");
	}

	if (size)
		size = roundup_pow_of_two(size);
	if (size > log_buf_len)
		new_log_buf_len = (unsigned long)size;
}

/* save requested log_buf_len since it's too early to process it */
static int __init log_buf_len_setup(char *str)
{
	u64 size;

	if (!str)
		return -EINVAL;

	size = memparse(str, &str);

	log_buf_len_update(size);

	return 0;
}
early_param("log_buf_len", log_buf_len_setup);

#ifdef CONFIG_SMP
#define __LOG_CPU_MAX_BUF_LEN (1 << CONFIG_LOG_CPU_MAX_BUF_SHIFT)

static void __init log_buf_add_cpu(void)
{
	unsigned int cpu_extra;

	/*
	 * archs should set up cpu_possible_bits properly with
	 * set_cpu_possible() after setup_arch() but just in
	 * case lets ensure this is valid.
	 */
	if (num_possible_cpus() == 1)
		return;

	cpu_extra = (num_possible_cpus() - 1) * __LOG_CPU_MAX_BUF_LEN;

	/* by default this will only continue through for large > 64 CPUs */
	if (cpu_extra <= __LOG_BUF_LEN / 2)
		return;

	pr_info("log_buf_len individual max cpu contribution: %d bytes\n",
		__LOG_CPU_MAX_BUF_LEN);
	pr_info("log_buf_len total cpu_extra contributions: %d bytes\n",
		cpu_extra);
	pr_info("log_buf_len min size: %d bytes\n", __LOG_BUF_LEN);

	log_buf_len_update(cpu_extra + __LOG_BUF_LEN);
}
#else /* !CONFIG_SMP */
static inline void log_buf_add_cpu(void) {}
#endif /* CONFIG_SMP */

static void __init set_percpu_data_ready(void)
{
<<<<<<< HEAD
	printk_safe_init();
	/* Make sure we set this flag only after printk_safe() init is done */
	barrier();
	__printk_percpu_data_ready = true;
}

void __init setup_log_buf(int early)
{
	unsigned long flags;
	char *new_log_buf;
	unsigned int free;
=======
	__printk_percpu_data_ready = true;
}

static unsigned int __init add_to_rb(struct printk_ringbuffer *rb,
				     struct printk_record *r)
{
	struct prb_reserved_entry e;
	struct printk_record dest_r;

	prb_rec_init_wr(&dest_r, r->info->text_len);

	if (!prb_reserve(&e, rb, &dest_r))
		return 0;

	memcpy(&dest_r.text_buf[0], &r->text_buf[0], r->info->text_len);
	dest_r.info->text_len = r->info->text_len;
	dest_r.info->facility = r->info->facility;
	dest_r.info->level = r->info->level;
	dest_r.info->flags = r->info->flags;
	dest_r.info->ts_nsec = r->info->ts_nsec;
	dest_r.info->caller_id = r->info->caller_id;
	memcpy(&dest_r.info->dev_info, &r->info->dev_info, sizeof(dest_r.info->dev_info));

	prb_final_commit(&e);

	return prb_record_text_space(&e);
}

static char setup_text_buf[LOG_LINE_MAX] __initdata;

void __init setup_log_buf(int early)
{
	struct printk_info *new_infos;
	unsigned int new_descs_count;
	struct prb_desc *new_descs;
	struct printk_info info;
	struct printk_record r;
	unsigned int text_size;
	size_t new_descs_size;
	size_t new_infos_size;
	unsigned long flags;
	char *new_log_buf;
	unsigned int free;
	u64 seq;
>>>>>>> upstream/android-13

	/*
	 * Some archs call setup_log_buf() multiple times - first is very
	 * early, e.g. from setup_arch(), and second - when percpu_areas
	 * are initialised.
	 */
	if (!early)
		set_percpu_data_ready();

	if (log_buf != __log_buf)
		return;

	if (!early && !new_log_buf_len)
		log_buf_add_cpu();

	if (!new_log_buf_len)
		return;

<<<<<<< HEAD
	set_memsize_kernel_type(MEMSIZE_KERNEL_LOGBUF);
	if (early) {
		new_log_buf =
			memblock_virt_alloc(new_log_buf_len, LOG_ALIGN);
	} else {
		new_log_buf = memblock_virt_alloc_nopanic(new_log_buf_len,
							  LOG_ALIGN);
	}
	set_memsize_kernel_type(MEMSIZE_KERNEL_OTHERS);

	if (unlikely(!new_log_buf)) {
		pr_err("log_buf_len: %lu bytes not available\n",
			new_log_buf_len);
		return;
	}

	logbuf_lock_irqsave(flags);
	log_buf_len = new_log_buf_len;
	log_buf = new_log_buf;
	new_log_buf_len = 0;
	free = __LOG_BUF_LEN - log_next_idx;
	memcpy(log_buf, __log_buf, __LOG_BUF_LEN);
	logbuf_unlock_irqrestore(flags);
=======
	new_descs_count = new_log_buf_len >> PRB_AVGBITS;
	if (new_descs_count == 0) {
		pr_err("new_log_buf_len: %lu too small\n", new_log_buf_len);
		return;
	}

	new_log_buf = memblock_alloc(new_log_buf_len, LOG_ALIGN);
	if (unlikely(!new_log_buf)) {
		pr_err("log_buf_len: %lu text bytes not available\n",
		       new_log_buf_len);
		return;
	}

	new_descs_size = new_descs_count * sizeof(struct prb_desc);
	new_descs = memblock_alloc(new_descs_size, LOG_ALIGN);
	if (unlikely(!new_descs)) {
		pr_err("log_buf_len: %zu desc bytes not available\n",
		       new_descs_size);
		goto err_free_log_buf;
	}

	new_infos_size = new_descs_count * sizeof(struct printk_info);
	new_infos = memblock_alloc(new_infos_size, LOG_ALIGN);
	if (unlikely(!new_infos)) {
		pr_err("log_buf_len: %zu info bytes not available\n",
		       new_infos_size);
		goto err_free_descs;
	}

	prb_rec_init_rd(&r, &info, &setup_text_buf[0], sizeof(setup_text_buf));

	prb_init(&printk_rb_dynamic,
		 new_log_buf, ilog2(new_log_buf_len),
		 new_descs, ilog2(new_descs_count),
		 new_infos);

	local_irq_save(flags);

	log_buf_len = new_log_buf_len;
	log_buf = new_log_buf;
	new_log_buf_len = 0;

	free = __LOG_BUF_LEN;
	prb_for_each_record(0, &printk_rb_static, seq, &r) {
		text_size = add_to_rb(&printk_rb_dynamic, &r);
		if (text_size > free)
			free = 0;
		else
			free -= text_size;
	}

	prb = &printk_rb_dynamic;

	local_irq_restore(flags);

	/*
	 * Copy any remaining messages that might have appeared from
	 * NMI context after copying but before switching to the
	 * dynamic buffer.
	 */
	prb_for_each_record(seq, &printk_rb_static, seq, &r) {
		text_size = add_to_rb(&printk_rb_dynamic, &r);
		if (text_size > free)
			free = 0;
		else
			free -= text_size;
	}

	if (seq != prb_next_seq(&printk_rb_static)) {
		pr_err("dropped %llu messages\n",
		       prb_next_seq(&printk_rb_static) - seq);
	}
>>>>>>> upstream/android-13

	pr_info("log_buf_len: %u bytes\n", log_buf_len);
	pr_info("early log buf free: %u(%u%%)\n",
		free, (free * 100) / __LOG_BUF_LEN);
<<<<<<< HEAD
=======
	return;

err_free_descs:
	memblock_free_ptr(new_descs, new_descs_size);
err_free_log_buf:
	memblock_free_ptr(new_log_buf, new_log_buf_len);
>>>>>>> upstream/android-13
}

static bool __read_mostly ignore_loglevel;

static int __init ignore_loglevel_setup(char *str)
{
	ignore_loglevel = true;
	pr_info("debug: ignoring loglevel setting.\n");

	return 0;
}

early_param("ignore_loglevel", ignore_loglevel_setup);
module_param(ignore_loglevel, bool, S_IRUGO | S_IWUSR);
MODULE_PARM_DESC(ignore_loglevel,
		 "ignore loglevel setting (prints all kernel messages to the console)");

static bool suppress_message_printing(int level)
{
	return (level >= console_loglevel && !ignore_loglevel);
}

#ifdef CONFIG_BOOT_PRINTK_DELAY

static int boot_delay; /* msecs delay after each printk during bootup */
static unsigned long long loops_per_msec;	/* based on boot_delay */

static int __init boot_delay_setup(char *str)
{
	unsigned long lpj;

	lpj = preset_lpj ? preset_lpj : 1000000;	/* some guess */
	loops_per_msec = (unsigned long long)lpj / 1000 * HZ;

	get_option(&str, &boot_delay);
	if (boot_delay > 10 * 1000)
		boot_delay = 0;

	pr_debug("boot_delay: %u, preset_lpj: %ld, lpj: %lu, "
		"HZ: %d, loops_per_msec: %llu\n",
		boot_delay, preset_lpj, lpj, HZ, loops_per_msec);
	return 0;
}
early_param("boot_delay", boot_delay_setup);

static void boot_delay_msec(int level)
{
	unsigned long long k;
	unsigned long timeout;

	if ((boot_delay == 0 || system_state >= SYSTEM_RUNNING)
		|| suppress_message_printing(level)) {
		return;
	}

	k = (unsigned long long)loops_per_msec * boot_delay;

	timeout = jiffies + msecs_to_jiffies(boot_delay);
	while (k) {
		k--;
		cpu_relax();
		/*
		 * use (volatile) jiffies to prevent
		 * compiler reduction; loop termination via jiffies
		 * is secondary and may or may not happen.
		 */
		if (time_after(jiffies, timeout))
			break;
		touch_nmi_watchdog();
	}
}
#else
static inline void boot_delay_msec(int level)
{
}
#endif

static bool printk_time = IS_ENABLED(CONFIG_PRINTK_TIME);
module_param_named(time, printk_time, bool, S_IRUGO | S_IWUSR);

<<<<<<< HEAD
static size_t print_time(u64 ts, char *buf)
{
	unsigned long rem_nsec;

	if (!printk_time)
		return 0;

	rem_nsec = do_div(ts, 1000000000);

	if (!buf)
		return snprintf(NULL, 0, "[%5lu.000000] ", (unsigned long)ts);

	return sprintf(buf, "[%5lu.%06lu] ",
		       (unsigned long)ts, rem_nsec / 1000);
}

static size_t print_prefix(const struct printk_log *msg, bool syslog, char *buf)
{
	size_t len = 0;
	unsigned int prefix = (msg->facility << 3) | msg->level;

	if (syslog) {
		if (buf) {
			len += sprintf(buf, "<%u>", prefix);
		} else {
			len += 3;
			if (prefix > 999)
				len += 3;
			else if (prefix > 99)
				len += 2;
			else if (prefix > 9)
				len++;
		}
	}

	len += print_time(msg->ts_nsec, buf ? buf + len : NULL);

#if defined(CONFIG_PRINTK_PREFIX_ENHANCE) \
	 && defined(CONFIG_PRINTK_MTK_UART_CONSOLE)
	/* if uart printk enabled */
	if (syslog == false && mt_get_uartlog_status()) {
		if (buf)
			len += sprintf(buf+len, "<%d>", smp_processor_id());
		else
			len += snprintf(NULL, 0, "<%d>", smp_processor_id());
	}
#endif

	return len;
}

static size_t msg_print_text(const struct printk_log *msg, bool syslog, char *buf, size_t size)
{
	const char *text = log_text(msg);
	size_t text_size = msg->text_len;
	size_t len = 0;

	do {
		const char *next = memchr(text, '\n', text_size);
		size_t text_len;

		if (next) {
			text_len = next - text;
			next++;
			text_size -= next - text;
		} else {
			text_len = text_size;
		}

		if (buf) {
			if (print_prefix(msg, syslog, NULL) +
			    text_len + 1 >= size - len)
				break;

			len += print_prefix(msg, syslog, buf + len);
			memcpy(buf + len, text, text_len);
			len += text_len;
			buf[len++] = '\n';
		} else {
			/* SYSLOG_ACTION_* buffer size only calculation */
			len += print_prefix(msg, syslog, NULL);
			len += text_len;
			len++;
		}

		text = next;
	} while (text);
=======
static size_t print_syslog(unsigned int level, char *buf)
{
	return sprintf(buf, "<%u>", level);
}

static size_t print_time(u64 ts, char *buf)
{
	unsigned long rem_nsec = do_div(ts, 1000000000);

	return sprintf(buf, "[%5lu.%06lu]",
		       (unsigned long)ts, rem_nsec / 1000);
}

#ifdef CONFIG_PRINTK_CALLER
static size_t print_caller(u32 id, char *buf)
{
	char caller[12];
	int vh_ret = 0;

	trace_android_vh_printk_caller(caller, sizeof(caller), id, &vh_ret);
	if (!vh_ret)
		snprintf(caller, sizeof(caller), "%c%u",
			 id & 0x80000000 ? 'C' : 'T', id & ~0x80000000);
	return sprintf(buf, "[%6s]", caller);
}
#else
#define print_caller(id, buf) 0
#endif
#ifdef CONFIG_PRINTK_PROCESS
static size_t print_process(const struct printk_info *info, char *buf)
{
	return sprintf(buf, "%c[%1d:%15s:%5d]", info->in_interrupt ? 'I' : ' ',
			info->cpu, info->process, info->pid);
}
#else
#define print_process(info, buf) 0
#endif

static size_t info_print_prefix(const struct printk_info  *info, bool syslog,
				bool time, char *buf)
{
	size_t len = 0;

	if (syslog)
		len = print_syslog((info->facility << 3) | info->level, buf);

	if (time)
		len += print_time(info->ts_nsec, buf + len);

	len += print_caller(info->caller_id, buf + len);
	len += print_process(info, buf + len);

	if (IS_ENABLED(CONFIG_PRINTK_CALLER) || IS_ENABLED(CONFIG_PRINTK_PROCESS) || time) {
		buf[len++] = ' ';
		buf[len] = '\0';
	}
>>>>>>> upstream/android-13

	return len;
}

<<<<<<< HEAD
static int syslog_print(char __user *buf, int size)
{
	char *text;
	struct printk_log *msg;
	int len = 0;

	text = kmalloc(LOG_LINE_MAX + PREFIX_MAX, GFP_KERNEL);
	if (!text)
		return -ENOMEM;

	while (size > 0) {
		size_t n;
		size_t skip;

		logbuf_lock_irq();
		if (syslog_seq < log_first_seq) {
			/* messages are gone, move to first one */
			syslog_seq = log_first_seq;
			syslog_idx = log_first_idx;
			syslog_partial = 0;
		}
		if (syslog_seq == log_next_seq) {
			logbuf_unlock_irq();
			break;
		}

		skip = syslog_partial;
		msg = log_from_idx(syslog_idx);
		n = msg_print_text(msg, true, text, LOG_LINE_MAX + PREFIX_MAX);
		if (n - syslog_partial <= size) {
			/* message fits into buffer, move forward */
			syslog_idx = log_next(syslog_idx);
			syslog_seq++;
=======
/*
 * Prepare the record for printing. The text is shifted within the given
 * buffer to avoid a need for another one. The following operations are
 * done:
 *
 *   - Add prefix for each line.
 *   - Drop truncated lines that no longer fit into the buffer.
 *   - Add the trailing newline that has been removed in vprintk_store().
 *   - Add a string terminator.
 *
 * Since the produced string is always terminated, the maximum possible
 * return value is @r->text_buf_size - 1;
 *
 * Return: The length of the updated/prepared text, including the added
 * prefixes and the newline. The terminator is not counted. The dropped
 * line(s) are not counted.
 */
static size_t record_print_text(struct printk_record *r, bool syslog,
				bool time)
{
	size_t text_len = r->info->text_len;
	size_t buf_size = r->text_buf_size;
	char *text = r->text_buf;
	char prefix[PREFIX_MAX];
	bool truncated = false;
	size_t prefix_len;
	size_t line_len;
	size_t len = 0;
	char *next;

	/*
	 * If the message was truncated because the buffer was not large
	 * enough, treat the available text as if it were the full text.
	 */
	if (text_len > buf_size)
		text_len = buf_size;

	prefix_len = info_print_prefix(r->info, syslog, time, prefix);

	/*
	 * @text_len: bytes of unprocessed text
	 * @line_len: bytes of current line _without_ newline
	 * @text:     pointer to beginning of current line
	 * @len:      number of bytes prepared in r->text_buf
	 */
	for (;;) {
		next = memchr(text, '\n', text_len);
		if (next) {
			line_len = next - text;
		} else {
			/* Drop truncated line(s). */
			if (truncated)
				break;
			line_len = text_len;
		}

		/*
		 * Truncate the text if there is not enough space to add the
		 * prefix and a trailing newline and a terminator.
		 */
		if (len + prefix_len + text_len + 1 + 1 > buf_size) {
			/* Drop even the current line if no space. */
			if (len + prefix_len + line_len + 1 + 1 > buf_size)
				break;

			text_len = buf_size - len - prefix_len - 1 - 1;
			truncated = true;
		}

		memmove(text + prefix_len, text, text_len);
		memcpy(text, prefix, prefix_len);

		/*
		 * Increment the prepared length to include the text and
		 * prefix that were just moved+copied. Also increment for the
		 * newline at the end of this line. If this is the last line,
		 * there is no newline, but it will be added immediately below.
		 */
		len += prefix_len + line_len + 1;
		if (text_len == line_len) {
			/*
			 * This is the last line. Add the trailing newline
			 * removed in vprintk_store().
			 */
			text[prefix_len + line_len] = '\n';
			break;
		}

		/*
		 * Advance beyond the added prefix and the related line with
		 * its newline.
		 */
		text += prefix_len + line_len + 1;

		/*
		 * The remaining text has only decreased by the line with its
		 * newline.
		 *
		 * Note that @text_len can become zero. It happens when @text
		 * ended with a newline (either due to truncation or the
		 * original string ending with "\n\n"). The loop is correctly
		 * repeated and (if not truncated) an empty line with a prefix
		 * will be prepared.
		 */
		text_len -= line_len + 1;
	}

	/*
	 * If a buffer was provided, it will be terminated. Space for the
	 * string terminator is guaranteed to be available. The terminator is
	 * not counted in the return value.
	 */
	if (buf_size > 0)
		r->text_buf[len] = 0;

	return len;
}

static size_t get_record_print_text_size(struct printk_info *info,
					 unsigned int line_count,
					 bool syslog, bool time)
{
	char prefix[PREFIX_MAX];
	size_t prefix_len;

	prefix_len = info_print_prefix(info, syslog, time, prefix);

	/*
	 * Each line will be preceded with a prefix. The intermediate
	 * newlines are already within the text, but a final trailing
	 * newline will be added.
	 */
	return ((prefix_len * line_count) + info->text_len + 1);
}

/*
 * Beginning with @start_seq, find the first record where it and all following
 * records up to (but not including) @max_seq fit into @size.
 *
 * @max_seq is simply an upper bound and does not need to exist. If the caller
 * does not require an upper bound, -1 can be used for @max_seq.
 */
static u64 find_first_fitting_seq(u64 start_seq, u64 max_seq, size_t size,
				  bool syslog, bool time)
{
	struct printk_info info;
	unsigned int line_count;
	size_t len = 0;
	u64 seq;

	/* Determine the size of the records up to @max_seq. */
	prb_for_each_info(start_seq, prb, seq, &info, &line_count) {
		if (info.seq >= max_seq)
			break;
		len += get_record_print_text_size(&info, line_count, syslog, time);
	}

	/*
	 * Adjust the upper bound for the next loop to avoid subtracting
	 * lengths that were never added.
	 */
	if (seq < max_seq)
		max_seq = seq;

	/*
	 * Move first record forward until length fits into the buffer. Ignore
	 * newest messages that were not counted in the above cycle. Messages
	 * might appear and get lost in the meantime. This is a best effort
	 * that prevents an infinite loop that could occur with a retry.
	 */
	prb_for_each_info(start_seq, prb, seq, &info, &line_count) {
		if (len <= size || info.seq >= max_seq)
			break;
		len -= get_record_print_text_size(&info, line_count, syslog, time);
	}

	return seq;
}

/* The caller is responsible for making sure @size is greater than 0. */
static int syslog_print(char __user *buf, int size)
{
	struct printk_info info;
	struct printk_record r;
	char *text;
	int len = 0;
	u64 seq;

	text = kmalloc(CONSOLE_LOG_MAX, GFP_KERNEL);
	if (!text)
		return -ENOMEM;

	prb_rec_init_rd(&r, &info, text, CONSOLE_LOG_MAX);

	mutex_lock(&syslog_lock);

	/*
	 * Wait for the @syslog_seq record to be available. @syslog_seq may
	 * change while waiting.
	 */
	do {
		seq = syslog_seq;

		mutex_unlock(&syslog_lock);
		len = wait_event_interruptible(log_wait, prb_read_valid(prb, seq, NULL));
		mutex_lock(&syslog_lock);

		if (len)
			goto out;
	} while (syslog_seq != seq);

	/*
	 * Copy records that fit into the buffer. The above cycle makes sure
	 * that the first record is always available.
	 */
	do {
		size_t n;
		size_t skip;
		int err;

		if (!prb_read_valid(prb, syslog_seq, &r))
			break;

		if (r.info->seq != syslog_seq) {
			/* message is gone, move to next valid one */
			syslog_seq = r.info->seq;
			syslog_partial = 0;
		}

		/*
		 * To keep reading/counting partial line consistent,
		 * use printk_time value as of the beginning of a line.
		 */
		if (!syslog_partial)
			syslog_time = printk_time;

		skip = syslog_partial;
		n = record_print_text(&r, true, syslog_time);
		if (n - syslog_partial <= size) {
			/* message fits into buffer, move forward */
			syslog_seq = r.info->seq + 1;
>>>>>>> upstream/android-13
			n -= syslog_partial;
			syslog_partial = 0;
		} else if (!len){
			/* partial read(), remember position */
			n = size;
			syslog_partial += n;
		} else
			n = 0;
<<<<<<< HEAD
		logbuf_unlock_irq();
=======
>>>>>>> upstream/android-13

		if (!n)
			break;

<<<<<<< HEAD
		if (copy_to_user(buf, text + skip, n)) {
=======
		mutex_unlock(&syslog_lock);
		err = copy_to_user(buf, text + skip, n);
		mutex_lock(&syslog_lock);

		if (err) {
>>>>>>> upstream/android-13
			if (!len)
				len = -EFAULT;
			break;
		}

		len += n;
		size -= n;
		buf += n;
<<<<<<< HEAD
	}

=======
	} while (size);
out:
	mutex_unlock(&syslog_lock);
>>>>>>> upstream/android-13
	kfree(text);
	return len;
}

static int syslog_print_all(char __user *buf, int size, bool clear)
{
<<<<<<< HEAD
	char *text;
	int len = 0;
	u64 next_seq;
	u64 seq;
	u32 idx;

	text = kmalloc(LOG_LINE_MAX + PREFIX_MAX, GFP_KERNEL);
	if (!text)
		return -ENOMEM;

	logbuf_lock_irq();
=======
	struct printk_info info;
	struct printk_record r;
	char *text;
	int len = 0;
	u64 seq;
	bool time;

	text = kmalloc(CONSOLE_LOG_MAX, GFP_KERNEL);
	if (!text)
		return -ENOMEM;

	time = printk_time;
>>>>>>> upstream/android-13
	/*
	 * Find first record that fits, including all following records,
	 * into the user-provided buffer for this dump.
	 */
<<<<<<< HEAD
	seq = clear_seq;
	idx = clear_idx;
	while (seq < log_next_seq) {
		struct printk_log *msg = log_from_idx(idx);

		len += msg_print_text(msg, true, NULL, 0);
		idx = log_next(idx);
		seq++;
	}

	/* move first record forward until length fits into the buffer */
	seq = clear_seq;
	idx = clear_idx;
	while (len > size && seq < log_next_seq) {
		struct printk_log *msg = log_from_idx(idx);

		len -= msg_print_text(msg, true, NULL, 0);
		idx = log_next(idx);
		seq++;
	}

	/* last message fitting into this dump */
	next_seq = log_next_seq;

	len = 0;
	while (len >= 0 && seq < next_seq) {
		struct printk_log *msg = log_from_idx(idx);
		int textlen;

		textlen = msg_print_text(msg, true, text,
					 LOG_LINE_MAX + PREFIX_MAX);
		if (textlen < 0) {
			len = textlen;
			break;
		}
		idx = log_next(idx);
		seq++;

		logbuf_unlock_irq();
=======
	seq = find_first_fitting_seq(latched_seq_read_nolock(&clear_seq), -1,
				     size, true, time);

	prb_rec_init_rd(&r, &info, text, CONSOLE_LOG_MAX);

	len = 0;
	prb_for_each_record(seq, prb, seq, &r) {
		int textlen;

		textlen = record_print_text(&r, true, time);

		if (len + textlen > size) {
			seq--;
			break;
		}

>>>>>>> upstream/android-13
		if (copy_to_user(buf + len, text, textlen))
			len = -EFAULT;
		else
			len += textlen;
<<<<<<< HEAD
		logbuf_lock_irq();

		if (seq < log_first_seq) {
			/* messages are gone, move to next one */
			seq = log_first_seq;
			idx = log_first_idx;
		}
	}

	if (clear) {
		clear_seq = log_next_seq;
		clear_idx = log_next_idx;
	}
	logbuf_unlock_irq();
=======

		if (len < 0)
			break;
	}

	if (clear) {
		mutex_lock(&syslog_lock);
		latched_seq_write(&clear_seq, seq);
		mutex_unlock(&syslog_lock);
	}
>>>>>>> upstream/android-13

	kfree(text);
	return len;
}

static void syslog_clear(void)
{
<<<<<<< HEAD
	logbuf_lock_irq();
	clear_seq = log_next_seq;
	clear_idx = log_next_idx;
	logbuf_unlock_irq();
=======
	mutex_lock(&syslog_lock);
	latched_seq_write(&clear_seq, prb_next_seq(prb));
	mutex_unlock(&syslog_lock);
>>>>>>> upstream/android-13
}

int do_syslog(int type, char __user *buf, int len, int source)
{
<<<<<<< HEAD
=======
	struct printk_info info;
>>>>>>> upstream/android-13
	bool clear = false;
	static int saved_console_loglevel = LOGLEVEL_DEFAULT;
	int error;

	error = check_syslog_permissions(type, source);
	if (error)
		return error;

	switch (type) {
	case SYSLOG_ACTION_CLOSE:	/* Close log */
		break;
	case SYSLOG_ACTION_OPEN:	/* Open log */
		break;
	case SYSLOG_ACTION_READ:	/* Read from log */
		if (!buf || len < 0)
			return -EINVAL;
		if (!len)
			return 0;
<<<<<<< HEAD
		if (!access_ok(VERIFY_WRITE, buf, len))
			return -EFAULT;
		error = wait_event_interruptible(log_wait,
						 syslog_seq != log_next_seq);
		if (error)
			return error;
=======
		if (!access_ok(buf, len))
			return -EFAULT;
>>>>>>> upstream/android-13
		error = syslog_print(buf, len);
		break;
	/* Read/clear last kernel messages */
	case SYSLOG_ACTION_READ_CLEAR:
		clear = true;
<<<<<<< HEAD
		/* FALL THRU */
=======
		fallthrough;
>>>>>>> upstream/android-13
	/* Read last kernel messages */
	case SYSLOG_ACTION_READ_ALL:
		if (!buf || len < 0)
			return -EINVAL;
		if (!len)
			return 0;
<<<<<<< HEAD
		if (!access_ok(VERIFY_WRITE, buf, len))
=======
		if (!access_ok(buf, len))
>>>>>>> upstream/android-13
			return -EFAULT;
		error = syslog_print_all(buf, len, clear);
		break;
	/* Clear ring buffer */
	case SYSLOG_ACTION_CLEAR:
		syslog_clear();
		break;
	/* Disable logging to console */
	case SYSLOG_ACTION_CONSOLE_OFF:
		if (saved_console_loglevel == LOGLEVEL_DEFAULT)
			saved_console_loglevel = console_loglevel;
		console_loglevel = minimum_console_loglevel;
		break;
	/* Enable logging to console */
	case SYSLOG_ACTION_CONSOLE_ON:
		if (saved_console_loglevel != LOGLEVEL_DEFAULT) {
			console_loglevel = saved_console_loglevel;
			saved_console_loglevel = LOGLEVEL_DEFAULT;
		}
		break;
	/* Set level of messages printed to console */
	case SYSLOG_ACTION_CONSOLE_LEVEL:
		if (len < 1 || len > 8)
			return -EINVAL;
		if (len < minimum_console_loglevel)
			len = minimum_console_loglevel;
		console_loglevel = len;
		/* Implicitly re-enable logging to console */
		saved_console_loglevel = LOGLEVEL_DEFAULT;
		break;
	/* Number of chars in the log buffer */
	case SYSLOG_ACTION_SIZE_UNREAD:
<<<<<<< HEAD
		logbuf_lock_irq();
		if (syslog_seq < log_first_seq) {
			/* messages are gone, move to first one */
			syslog_seq = log_first_seq;
			syslog_idx = log_first_idx;
=======
		mutex_lock(&syslog_lock);
		if (!prb_read_valid_info(prb, syslog_seq, &info, NULL)) {
			/* No unread messages. */
			mutex_unlock(&syslog_lock);
			return 0;
		}
		if (info.seq != syslog_seq) {
			/* messages are gone, move to first one */
			syslog_seq = info.seq;
>>>>>>> upstream/android-13
			syslog_partial = 0;
		}
		if (source == SYSLOG_FROM_PROC) {
			/*
			 * Short-cut for poll(/"proc/kmsg") which simply checks
			 * for pending data, not the size; return the count of
			 * records, not the length.
			 */
<<<<<<< HEAD
			error = log_next_seq - syslog_seq;
		} else {
			u64 seq = syslog_seq;
			u32 idx = syslog_idx;

			while (seq < log_next_seq) {
				struct printk_log *msg = log_from_idx(idx);

				error += msg_print_text(msg, true, NULL, 0);
				idx = log_next(idx);
				seq++;
			}
			error -= syslog_partial;
		}
		logbuf_unlock_irq();
=======
			error = prb_next_seq(prb) - syslog_seq;
		} else {
			bool time = syslog_partial ? syslog_time : printk_time;
			unsigned int line_count;
			u64 seq;

			prb_for_each_info(syslog_seq, prb, seq, &info,
					  &line_count) {
				error += get_record_print_text_size(&info, line_count,
								    true, time);
				time = printk_time;
			}
			error -= syslog_partial;
		}
		mutex_unlock(&syslog_lock);
>>>>>>> upstream/android-13
		break;
	/* Size of the log buffer */
	case SYSLOG_ACTION_SIZE_BUFFER:
		error = log_buf_len;
		break;
	default:
		error = -EINVAL;
		break;
	}

	return error;
}

SYSCALL_DEFINE3(syslog, int, type, char __user *, buf, int, len)
{
	return do_syslog(type, buf, len, SYSLOG_FROM_READER);
}

/*
 * Special console_lock variants that help to reduce the risk of soft-lockups.
 * They allow to pass console_lock to another printk() call using a busy wait.
 */

#ifdef CONFIG_LOCKDEP
static struct lockdep_map console_owner_dep_map = {
	.name = "console_owner"
};
#endif

static DEFINE_RAW_SPINLOCK(console_owner_lock);
static struct task_struct *console_owner;
static bool console_waiter;

/**
 * console_lock_spinning_enable - mark beginning of code where another
 *	thread might safely busy wait
 *
 * This basically converts console_lock into a spinlock. This marks
 * the section where the console_lock owner can not sleep, because
 * there may be a waiter spinning (like a spinlock). Also it must be
 * ready to hand over the lock at the end of the section.
 */
static void console_lock_spinning_enable(void)
{
	raw_spin_lock(&console_owner_lock);
	console_owner = current;
	raw_spin_unlock(&console_owner_lock);

	/* The waiter may spin on us after setting console_owner */
	spin_acquire(&console_owner_dep_map, 0, 0, _THIS_IP_);
}

/**
 * console_lock_spinning_disable_and_check - mark end of code where another
 *	thread was able to busy wait and check if there is a waiter
 *
 * This is called at the end of the section where spinning is allowed.
 * It has two functions. First, it is a signal that it is no longer
 * safe to start busy waiting for the lock. Second, it checks if
 * there is a busy waiter and passes the lock rights to her.
 *
 * Important: Callers lose the lock if there was a busy waiter.
 *	They must not touch items synchronized by console_lock
 *	in this case.
 *
 * Return: 1 if the lock rights were passed, 0 otherwise.
 */
static int console_lock_spinning_disable_and_check(void)
{
	int waiter;

	raw_spin_lock(&console_owner_lock);
	waiter = READ_ONCE(console_waiter);
	console_owner = NULL;
	raw_spin_unlock(&console_owner_lock);

	if (!waiter) {
<<<<<<< HEAD
		spin_release(&console_owner_dep_map, 1, _THIS_IP_);
=======
		spin_release(&console_owner_dep_map, _THIS_IP_);
>>>>>>> upstream/android-13
		return 0;
	}

	/* The waiter is now free to continue */
	WRITE_ONCE(console_waiter, false);

<<<<<<< HEAD
	spin_release(&console_owner_dep_map, 1, _THIS_IP_);
=======
	spin_release(&console_owner_dep_map, _THIS_IP_);
>>>>>>> upstream/android-13

	/*
	 * Hand off console_lock to waiter. The waiter will perform
	 * the up(). After this, the waiter is the console_lock owner.
	 */
<<<<<<< HEAD
	mutex_release(&console_lock_dep_map, 1, _THIS_IP_);
=======
	mutex_release(&console_lock_dep_map, _THIS_IP_);
>>>>>>> upstream/android-13
	return 1;
}

/**
 * console_trylock_spinning - try to get console_lock by busy waiting
 *
 * This allows to busy wait for the console_lock when the current
 * owner is running in specially marked sections. It means that
 * the current owner is running and cannot reschedule until it
 * is ready to lose the lock.
 *
 * Return: 1 if we got the lock, 0 othrewise
 */
static int console_trylock_spinning(void)
{
	struct task_struct *owner = NULL;
	bool waiter;
	bool spin = false;
	unsigned long flags;

	if (console_trylock())
		return 1;

	printk_safe_enter_irqsave(flags);

	raw_spin_lock(&console_owner_lock);
	owner = READ_ONCE(console_owner);
	waiter = READ_ONCE(console_waiter);
	if (!waiter && owner && owner != current) {
		WRITE_ONCE(console_waiter, true);
		spin = true;
	}
	raw_spin_unlock(&console_owner_lock);

	/*
	 * If there is an active printk() writing to the
	 * consoles, instead of having it write our data too,
	 * see if we can offload that load from the active
	 * printer, and do some printing ourselves.
	 * Go into a spin only if there isn't already a waiter
	 * spinning, and there is an active printer, and
	 * that active printer isn't us (recursive printk?).
	 */
	if (!spin) {
		printk_safe_exit_irqrestore(flags);
		return 0;
	}

	/* We spin waiting for the owner to release us */
	spin_acquire(&console_owner_dep_map, 0, 0, _THIS_IP_);
	/* Owner will clear console_waiter on hand off */
	while (READ_ONCE(console_waiter))
		cpu_relax();
<<<<<<< HEAD
	spin_release(&console_owner_dep_map, 1, _THIS_IP_);
=======
	spin_release(&console_owner_dep_map, _THIS_IP_);
>>>>>>> upstream/android-13

	printk_safe_exit_irqrestore(flags);
	/*
	 * The owner passed the console lock to us.
	 * Since we did not spin on console lock, annotate
	 * this as a trylock. Otherwise lockdep will
	 * complain.
	 */
	mutex_acquire(&console_lock_dep_map, 0, 1, _THIS_IP_);

	return 1;
}

/*
 * Call the console drivers, asking them to write out
 * log_buf[start] to log_buf[end - 1].
 * The console_lock must be held.
 */
static void call_console_drivers(const char *ext_text, size_t ext_len,
				 const char *text, size_t len)
{
<<<<<<< HEAD
	struct console *con;
#ifdef CONFIG_CONSOLE_LOCK_DURATION_DETECT
	unsigned long interval_con_write = 0;
#endif
=======
	static char dropped_text[64];
	size_t dropped_len = 0;
	struct console *con;
>>>>>>> upstream/android-13

	trace_console_rcuidle(text, len);

	if (!console_drivers)
		return;

<<<<<<< HEAD
	for_each_console(con) {
#ifdef CONFIG_PRINTK_MTK_UART_CONSOLE
		/* if uart printk disabled */
		if (!mt_get_uartlog_status() && (con->flags & CON_CONSDEV))
			continue;
#endif
=======
	if (console_dropped) {
		dropped_len = snprintf(dropped_text, sizeof(dropped_text),
				       "** %lu printk messages dropped **\n",
				       console_dropped);
		console_dropped = 0;
	}

	for_each_console(con) {
>>>>>>> upstream/android-13
		if (exclusive_console && con != exclusive_console)
			continue;
		if (!(con->flags & CON_ENABLED))
			continue;
		if (!con->write)
			continue;
		if (!cpu_online(smp_processor_id()) &&
		    !(con->flags & CON_ANYTIME))
			continue;
		if (con->flags & CON_EXTENDED)
			con->write(con, ext_text, ext_len);
		else {
<<<<<<< HEAD
#ifdef CONFIG_CONSOLE_LOCK_DURATION_DETECT
			conwrite_stat_struct.con = con;
			conwrite_stat_struct.time_before_conwrite
				= local_clock();
			con->write(con, text, len);
			conwrite_stat_struct.time_after_conwrite
				= local_clock();
			interval_con_write =
				conwrite_stat_struct.time_after_conwrite -
				conwrite_stat_struct.time_before_conwrite;
			if (!strcmp(con->name, "ttyS")) {
				time_con_write_ttyS += interval_con_write;
				len_con_write_ttyS += len;
			} else if (!strcmp(con->name, "pstore")) {
				time_con_write_pstore += interval_con_write;
				len_con_write_pstore += len;
			}
#else
			con->write(con, text, len);
#endif
=======
			if (dropped_len)
				con->write(con, dropped_text, dropped_len);
			con->write(con, text, len);
>>>>>>> upstream/android-13
		}
	}
}

<<<<<<< HEAD
#ifdef CONFIG_MTK_AEE_FEATURE
/* if logbuf lock in aee_wdt flow, zap locks uncondationally  */
void aee_wdt_zap_locks(void)
{
	debug_locks_off();
	/* If a crash is occurring, make sure we can't deadlock */
	raw_spin_lock_init(&logbuf_lock);
	/* And make sure that we print immediately */
	sema_init(&console_sem, 1);
}
#endif

=======
/*
 * Recursion is tracked separately on each CPU. If NMIs are supported, an
 * additional NMI context per CPU is also separately tracked. Until per-CPU
 * is available, a separate "early tracking" is performed.
 */
static DEFINE_PER_CPU(u8, printk_count);
static u8 printk_count_early;
#ifdef CONFIG_HAVE_NMI
static DEFINE_PER_CPU(u8, printk_count_nmi);
static u8 printk_count_nmi_early;
#endif

/*
 * Recursion is limited to keep the output sane. printk() should not require
 * more than 1 level of recursion (allowing, for example, printk() to trigger
 * a WARN), but a higher value is used in case some printk-internal errors
 * exist, such as the ringbuffer validation checks failing.
 */
#define PRINTK_MAX_RECURSION 3

/*
 * Return a pointer to the dedicated counter for the CPU+context of the
 * caller.
 */
static u8 *__printk_recursion_counter(void)
{
#ifdef CONFIG_HAVE_NMI
	if (in_nmi()) {
		if (printk_percpu_data_ready())
			return this_cpu_ptr(&printk_count_nmi);
		return &printk_count_nmi_early;
	}
#endif
	if (printk_percpu_data_ready())
		return this_cpu_ptr(&printk_count);
	return &printk_count_early;
}

/*
 * Enter recursion tracking. Interrupts are disabled to simplify tracking.
 * The caller must check the boolean return value to see if the recursion is
 * allowed. On failure, interrupts are not disabled.
 *
 * @recursion_ptr must be a variable of type (u8 *) and is the same variable
 * that is passed to printk_exit_irqrestore().
 */
#define printk_enter_irqsave(recursion_ptr, flags)	\
({							\
	bool success = true;				\
							\
	typecheck(u8 *, recursion_ptr);			\
	local_irq_save(flags);				\
	(recursion_ptr) = __printk_recursion_counter();	\
	if (*(recursion_ptr) > PRINTK_MAX_RECURSION) {	\
		local_irq_restore(flags);		\
		success = false;			\
	} else {					\
		(*(recursion_ptr))++;			\
	}						\
	success;					\
})

/* Exit recursion tracking, restoring interrupts. */
#define printk_exit_irqrestore(recursion_ptr, flags)	\
	do {						\
		typecheck(u8 *, recursion_ptr);		\
		(*(recursion_ptr))--;			\
		local_irq_restore(flags);		\
	} while (0)

>>>>>>> upstream/android-13
int printk_delay_msec __read_mostly;

static inline void printk_delay(void)
{
	if (unlikely(printk_delay_msec)) {
		int m = printk_delay_msec;

		while (m--) {
			mdelay(1);
			touch_nmi_watchdog();
		}
	}
}

<<<<<<< HEAD
/*
 * Continuation lines are buffered, and not committed to the record buffer
 * until the line is complete, or a race forces it. The line fragments
 * though, are printed immediately to the consoles to ensure everything has
 * reached the console in case of a kernel crash.
 */
static struct cont {
	char buf[LOG_LINE_MAX];
	size_t len;			/* length == 0 means unused buffer */
	struct task_struct *owner;	/* task of first print*/
	u64 ts_nsec;			/* time of first print */
	u8 level;			/* log level of first message */
	u8 facility;			/* log facility of first message */
	enum log_flags flags;		/* prefix, newline flags */
} cont;

static void cont_flush(void)
{
	if (cont.len == 0)
		return;

	log_store(cont.facility, cont.level, cont.flags, cont.ts_nsec,
		  NULL, 0, cont.buf, cont.len);
	cont.len = 0;
}

static bool cont_add(int facility, int level, enum log_flags flags, const char *text, size_t len)
{
	/*
	 * If ext consoles are present, flush and skip in-kernel
	 * continuation.  See nr_ext_console_drivers definition.  Also, if
	 * the line gets too long, split it up in separate records.
	 */
	if (nr_ext_console_drivers || cont.len + len > sizeof(cont.buf)) {
		cont_flush();
		return false;
	}

	if (!cont.len) {
		cont.facility = facility;
		cont.level = level;
		cont.owner = current;
		cont.ts_nsec = local_clock();
		cont.flags = flags;
	}

	memcpy(cont.buf + cont.len, text, len);
	cont.len += len;

	// The original flags come from the first line,
	// but later continuations can add a newline.
	if (flags & LOG_NEWLINE) {
		cont.flags |= LOG_NEWLINE;
		cont_flush();
	}

	if (cont.len > (sizeof(cont.buf) * 80) / 100)
		cont_flush();

	return true;
}

static size_t log_output(int facility, int level, enum log_flags lflags, const char *dict, size_t dictlen, char *text, size_t text_len)
{
#ifdef CONFIG_CONSOLE_LOCK_DURATION_DETECT
	u64 log_enter_time = local_clock();
#endif
	/*
	 * If an earlier line was buffered, and we're a continuation
	 * write from the same process, try to add it to the buffer.
	 */
	if (cont.len) {
		if (cont.owner == current && (lflags & LOG_CONT)) {
			if (cont_add(facility, level, lflags, text, text_len))
				return text_len;
		}
		/* Otherwise, make sure it's flushed */
		cont_flush();
	}

	/* Skip empty continuation lines that couldn't be added - they just flush */
	if (!text_len && (lflags & LOG_CONT))
		return 0;

	/* If it doesn't end in a newline, try to buffer the current line */
	if (!(lflags & LOG_NEWLINE)) {
		if (cont_add(facility, level, lflags, text, text_len))
			return text_len;
	}

	/* Store it in the record log */
#ifdef CONFIG_CONSOLE_LOCK_DURATION_DETECT
	return log_store(facility, level, lflags,
		log_enter_time, dict, dictlen, text, text_len);
#else
	return log_store(facility, level, lflags, 0, dict, dictlen, text, text_len);
#endif
}

/* Must be called under logbuf_lock. */
int vprintk_store(int facility, int level,
		  const char *dict, size_t dictlen,
		  const char *fmt, va_list args)
{
	static char textbuf[LOG_LINE_MAX];
	char *text = textbuf;
	size_t text_len;
	enum log_flags lflags = 0;

	/*
	 * The printf needs to come first; we need the syslog
	 * prefix which might be passed-in as a parameter.
	 */
	text_len = vscnprintf(text, sizeof(textbuf), fmt, args);

	/* mark and strip a trailing newline */
	if (text_len && text[text_len-1] == '\n') {
		text_len--;
		lflags |= LOG_NEWLINE;
	}

	/* strip kernel syslog prefix and extract log level or control flags */
	if (facility == 0) {
		int kern_level;

		while ((kern_level = printk_get_level(text)) != 0) {
			switch (kern_level) {
			case '0' ... '7':
				if (level == LOGLEVEL_DEFAULT)
					level = kern_level - '0';
				/* fallthrough */
#ifdef CONFIG_SEC_DEBUG_AUTO_COMMENT
			case 'B' ... 'J':
				if (level == LOGLEVEL_DEFAULT)
					level = LOGLEVEL_PR_AUTO_BASE + (kern_level - 'A'); /* 91 ~ 99 */
				/* fallthrough */
#endif
			case 'd':	/* KERN_DEFAULT */
				lflags |= LOG_PREFIX;
				break;
			case 'c':	/* KERN_CONT */
				lflags |= LOG_CONT;
			}

			text_len -= 2;
			text += 2;
		}
	}
=======
static inline u32 printk_caller_id(void)
{
	u32 caller_id = 0;

	trace_android_vh_printk_caller_id(&caller_id);
	if (caller_id)
		return caller_id;

	return in_task() ? task_pid_nr(current) :
		0x80000000 + raw_smp_processor_id();
}

/**
 * printk_parse_prefix - Parse level and control flags.
 *
 * @text:     The terminated text message.
 * @level:    A pointer to the current level value, will be updated.
 * @flags:    A pointer to the current printk_info flags, will be updated.
 *
 * @level may be NULL if the caller is not interested in the parsed value.
 * Otherwise the variable pointed to by @level must be set to
 * LOGLEVEL_DEFAULT in order to be updated with the parsed value.
 *
 * @flags may be NULL if the caller is not interested in the parsed value.
 * Otherwise the variable pointed to by @flags will be OR'd with the parsed
 * value.
 *
 * Return: The length of the parsed level and control flags.
 */
u16 printk_parse_prefix(const char *text, int *level,
			enum printk_info_flags *flags)
{
	u16 prefix_len = 0;
	int kern_level;

	while (*text) {
		kern_level = printk_get_level(text);
		if (!kern_level)
			break;

		switch (kern_level) {
		case '0' ... '7':
			if (level && *level == LOGLEVEL_DEFAULT)
				*level = kern_level - '0';
			break;
#if IS_ENABLED(CONFIG_SEC_DEBUG_AUTO_COMMENT)
		case 'B' ... 'J':
			if (level && *level == LOGLEVEL_DEFAULT)
				*level = LOGLEVEL_PR_AUTO_BASE + (kern_level - 'A'); /* 91 ~ 99 */
			break;
#endif
		case 'c':	/* KERN_CONT */
			if (flags)
				*flags |= LOG_CONT;
		}

		prefix_len += 2;
		text += 2;
	}

	return prefix_len;
}

static u16 printk_sprint(char *text, u16 size, int facility,
			 enum printk_info_flags *flags, const char *fmt,
			 va_list args)
{
	u16 text_len;

	text_len = vscnprintf(text, size, fmt, args);

	/* Mark and strip a trailing newline. */
	if (text_len && text[text_len - 1] == '\n') {
		text_len--;
		*flags |= LOG_NEWLINE;
	}

	/* Strip log level and control flags. */
	if (facility == 0) {
		u16 prefix_len;

		prefix_len = printk_parse_prefix(text, NULL, NULL);
		if (prefix_len) {
			text_len -= prefix_len;
			memmove(text, text + prefix_len, text_len);
		}
	}

	return text_len;
}

__printf(4, 0)
int vprintk_store(int facility, int level,
		  const struct dev_printk_info *dev_info,
		  const char *fmt, va_list args)
{
	const u32 caller_id = printk_caller_id();
	struct prb_reserved_entry e;
	enum printk_info_flags flags = 0;
	struct printk_record r;
	unsigned long irqflags;
	u16 trunc_msg_len = 0;
	char prefix_buf[8];
	u8 *recursion_ptr;
	u16 reserve_size;
	va_list args2;
	u16 text_len;
	int ret = 0;
	u64 ts_nsec;
#if IS_ENABLED(CONFIG_SEC_DEBUG_AUTO_COMMENT)
	bool is_auto_comm = false;
	int type_auto_comm;
#endif

	/*
	 * Since the duration of printk() can vary depending on the message
	 * and state of the ringbuffer, grab the timestamp now so that it is
	 * close to the call of printk(). This provides a more deterministic
	 * timestamp with respect to the caller.
	 */
	ts_nsec = local_clock();

	if (!printk_enter_irqsave(recursion_ptr, irqflags))
		return 0;

	/*
	 * The sprintf needs to come first since the syslog prefix might be
	 * passed in as a parameter. An extra byte must be reserved so that
	 * later the vscnprintf() into the reserved buffer has room for the
	 * terminating '\0', which is not counted by vsnprintf().
	 */
	va_copy(args2, args);
	reserve_size = vsnprintf(&prefix_buf[0], sizeof(prefix_buf), fmt, args2) + 1;
	va_end(args2);

	if (reserve_size > LOG_LINE_MAX)
		reserve_size = LOG_LINE_MAX;

	/* Extract log level or control flags. */
	if (facility == 0)
		printk_parse_prefix(&prefix_buf[0], &level, &flags);
>>>>>>> upstream/android-13

	if (level == LOGLEVEL_DEFAULT)
		level = default_message_loglevel;

<<<<<<< HEAD
	if (dict)
		lflags |= LOG_PREFIX|LOG_NEWLINE;

	return log_output(facility, level, lflags,
			  dict, dictlen, text, text_len);
}

asmlinkage int vprintk_emit(int facility, int level,
			    const char *dict, size_t dictlen,
			    const char *fmt, va_list args)
{
	int printed_len;
	bool in_sched = false, pending_output;
	unsigned long flags;
	u64 curr_log_seq;

#ifdef CONFIG_PRINTK_PREFIX_ENHANCE
	if (irqs_disabled())
		this_cpu_write(printk_state, '-');
#ifdef CONFIG_PRINTK_MTK_UART_CONSOLE
		/* if uart printk enabled */
	else if (mt_get_uartlog_status())
		this_cpu_write(printk_state, '.');
#endif
	else
		this_cpu_write(printk_state, ' ');
#endif
=======
	if (dev_info)
		flags |= LOG_NEWLINE;

#if IS_ENABLED(CONFIG_SEC_DEBUG_AUTO_COMMENT)
	if (level / 10 == 9) {
		is_auto_comm = true;
		type_auto_comm = level - LOGLEVEL_PR_AUTO_BASE;
		level = 0;
	}
#endif

	if (flags & LOG_CONT) {
		prb_rec_init_wr(&r, reserve_size);
		if (prb_reserve_in_last(&e, prb, &r, caller_id, LOG_LINE_MAX)) {
			text_len = printk_sprint(&r.text_buf[r.info->text_len], reserve_size,
						 facility, &flags, fmt, args);
			r.info->text_len += text_len;

			if (flags & LOG_NEWLINE) {
				r.info->flags |= LOG_NEWLINE;
				prb_final_commit(&e);
			} else {
				prb_commit(&e);
			}

			trace_android_vh_logbuf_pr_cont(&r, text_len);
			ret = text_len;
			goto out;
		}
	}

	/*
	 * Explicitly initialize the record before every prb_reserve() call.
	 * prb_reserve_in_last() and prb_reserve() purposely invalidate the
	 * structure when they fail.
	 */
	prb_rec_init_wr(&r, reserve_size);
	if (!prb_reserve(&e, prb, &r)) {
		/* truncate the message if it is too long for empty buffer */
		truncate_msg(&reserve_size, &trunc_msg_len);

		prb_rec_init_wr(&r, reserve_size + trunc_msg_len);
		if (!prb_reserve(&e, prb, &r))
			goto out;
	}

	/* fill message */
	text_len = printk_sprint(&r.text_buf[0], reserve_size, facility, &flags, fmt, args);
	if (trunc_msg_len)
		memcpy(&r.text_buf[text_len], trunc_msg, trunc_msg_len);
	r.info->text_len = text_len + trunc_msg_len;
	r.info->facility = facility;
	r.info->level = level & 7;
	r.info->flags = flags & 0x1f;
	r.info->ts_nsec = ts_nsec;
	r.info->caller_id = caller_id;
#ifdef CONFIG_PRINTK_PROCESS
	strncpy(r.info->process, current->comm, sizeof(r.info->process) - 1);
	r.info->process[sizeof(r.info->process) - 1] = '\0';
	r.info->pid = task_pid_nr(current);
	r.info->cpu = smp_processor_id();
	r.info->in_interrupt = in_interrupt() ? 1 : 0;
#endif
	if (dev_info)
		memcpy(&r.info->dev_info, dev_info, sizeof(r.info->dev_info));

	/* A message without a trailing newline can be continued. */
	if (!(flags & LOG_NEWLINE))
		prb_commit(&e);
	else
		prb_final_commit(&e);

#if IS_ENABLED(CONFIG_SEC_DEBUG_AUTO_COMMENT)
	if (is_auto_comm && func_hook_auto_comm) {
		spin_lock(&auto_comm_lock);
		func_hook_auto_comm(type_auto_comm, prb, &r);
		spin_unlock(&auto_comm_lock);
	}
#endif

	trace_android_vh_logbuf(prb, &r);
	ret = text_len + trunc_msg_len;
out:
	printk_exit_irqrestore(recursion_ptr, irqflags);
	return ret;
}

asmlinkage int vprintk_emit(int facility, int level,
			    const struct dev_printk_info *dev_info,
			    const char *fmt, va_list args)
{
	int printed_len;
	bool in_sched = false;

	/* Suppress unimportant messages after panic happens */
	if (unlikely(suppress_printk))
		return 0;
>>>>>>> upstream/android-13

	if (level == LOGLEVEL_SCHED) {
		level = LOGLEVEL_DEFAULT;
		in_sched = true;
	}

	boot_delay_msec(level);
	printk_delay();

<<<<<<< HEAD
	/* This stops the holder of console_sem just where we want him */
	logbuf_lock_irqsave(flags);
	curr_log_seq = log_next_seq;
	printed_len = vprintk_store(facility, level, dict, dictlen, fmt, args);
	pending_output = (curr_log_seq != log_next_seq);
	logbuf_unlock_irqrestore(flags);

	/* If called from the scheduler, we can not call up(). */
	if (!in_sched && pending_output) {
=======
	printed_len = vprintk_store(facility, level, dev_info, fmt, args);

	/* If called from the scheduler, we can not call up(). */
	if (!in_sched) {
>>>>>>> upstream/android-13
		/*
		 * Disable preemption to avoid being preempted while holding
		 * console_sem which would prevent anyone from printing to
		 * console
		 */
		preempt_disable();
		/*
		 * Try to acquire and then immediately release the console
		 * semaphore.  The release will print out buffers and wake up
		 * /dev/kmsg and syslog() users.
		 */
		if (console_trylock_spinning())
			console_unlock();
		preempt_enable();
	}

<<<<<<< HEAD
	if (pending_output)
		wake_up_klogd();
=======
	wake_up_klogd();
>>>>>>> upstream/android-13
	return printed_len;
}
EXPORT_SYMBOL(vprintk_emit);

<<<<<<< HEAD
asmlinkage int vprintk(const char *fmt, va_list args)
{
	return vprintk_func(fmt, args);
}
EXPORT_SYMBOL(vprintk);

asmlinkage int printk_emit(int facility, int level,
			   const char *dict, size_t dictlen,
			   const char *fmt, ...)
{
	va_list args;
	int r;

	va_start(args, fmt);
	r = vprintk_emit(facility, level, dict, dictlen, fmt, args);
	va_end(args);

	return r;
}
EXPORT_SYMBOL(printk_emit);

int vprintk_default(const char *fmt, va_list args)
{
	int r;

#ifdef CONFIG_KGDB_KDB
	/* Allow to pass printk() to kdb but avoid a recursion. */
	if (unlikely(kdb_trap_printk && kdb_printf_cpu < 0)) {
		r = vkdb_printf(KDB_MSGSRC_PRINTK, fmt, args);
		return r;
	}
#endif
	r = vprintk_emit(0, LOGLEVEL_DEFAULT, NULL, 0, fmt, args);

	return r;
}
EXPORT_SYMBOL_GPL(vprintk_default);

/**
 * printk - print a kernel message
 * @fmt: format string
 *
 * This is printk(). It can be called from any context. We want it to work.
 *
 * We try to grab the console_lock. If we succeed, it's easy - we log the
 * output and call the console drivers.  If we fail to get the semaphore, we
 * place the output into the log buffer and return. The current holder of
 * the console_sem will notice the new output in console_unlock(); and will
 * send it to the consoles before releasing the lock.
 *
 * One effect of this deferred printing is that code which calls printk() and
 * then changes console_loglevel may break. This is because console_loglevel
 * is inspected when the actual printing occurs.
 *
 * See also:
 * printf(3)
 *
 * See the vsnprintf() documentation for format string extensions over C99.
 */
asmlinkage __visible int printk(const char *fmt, ...)
=======
int vprintk_default(const char *fmt, va_list args)
{
	return vprintk_emit(0, LOGLEVEL_DEFAULT, NULL, fmt, args);
}
EXPORT_SYMBOL_GPL(vprintk_default);

asmlinkage __visible int _printk(const char *fmt, ...)
>>>>>>> upstream/android-13
{
	va_list args;
	int r;

	va_start(args, fmt);
<<<<<<< HEAD
	r = vprintk_func(fmt, args);
=======
	r = vprintk(fmt, args);
>>>>>>> upstream/android-13
	va_end(args);

	return r;
}
<<<<<<< HEAD
EXPORT_SYMBOL(printk);

#else /* CONFIG_PRINTK */

#define LOG_LINE_MAX		0
#define PREFIX_MAX		0

static u64 syslog_seq;
static u32 syslog_idx;
static u64 console_seq;
static u32 console_idx;
static u64 exclusive_console_stop_seq;
static u64 log_first_seq;
static u32 log_first_idx;
static u64 log_next_seq;
static char *log_text(const struct printk_log *msg) { return NULL; }
static char *log_dict(const struct printk_log *msg) { return NULL; }
static struct printk_log *log_from_idx(u32 idx) { return NULL; }
static u32 log_next(u32 idx) { return 0; }
static ssize_t msg_print_ext_header(char *buf, size_t size,
				    struct printk_log *msg,
				    u64 seq) { return 0; }
static ssize_t msg_print_ext_body(char *buf, size_t size,
				  char *dict, size_t dict_len,
				  char *text, size_t text_len) { return 0; }
=======
EXPORT_SYMBOL(_printk);

#else /* CONFIG_PRINTK */

#define CONSOLE_LOG_MAX		0
#define printk_time		false

#define prb_read_valid(rb, seq, r)	false
#define prb_first_valid_seq(rb)		0

static u64 syslog_seq;
static u64 console_seq;
static u64 exclusive_console_stop_seq;
static unsigned long console_dropped;

static size_t record_print_text(const struct printk_record *r,
				bool syslog, bool time)
{
	return 0;
}
static ssize_t info_print_ext_header(char *buf, size_t size,
				     struct printk_info *info)
{
	return 0;
}
static ssize_t msg_print_ext_body(char *buf, size_t size,
				  char *text, size_t text_len,
				  struct dev_printk_info *dev_info) { return 0; }
>>>>>>> upstream/android-13
static void console_lock_spinning_enable(void) { }
static int console_lock_spinning_disable_and_check(void) { return 0; }
static void call_console_drivers(const char *ext_text, size_t ext_len,
				 const char *text, size_t len) {}
<<<<<<< HEAD
static size_t msg_print_text(const struct printk_log *msg,
			     bool syslog, char *buf, size_t size) { return 0; }
=======
>>>>>>> upstream/android-13
static bool suppress_message_printing(int level) { return false; }

#endif /* CONFIG_PRINTK */

#ifdef CONFIG_EARLY_PRINTK
struct console *early_console;

asmlinkage __visible void early_printk(const char *fmt, ...)
{
	va_list ap;
	char buf[512];
	int n;

	if (!early_console)
		return;

	va_start(ap, fmt);
	n = vscnprintf(buf, sizeof(buf), fmt, ap);
	va_end(ap);

	early_console->write(early_console, buf, n);
}
#endif

<<<<<<< HEAD
#ifdef CONFIG_LOG_TOO_MUCH_WARNING
static int parse_log_file(void)
{
	char buff[LOG_LINE_MAX + PREFIX_MAX];
	u32 log_index = start_idx;
	u64 log_seq = start_seq;
	size_t count = 0;
	struct printk_log *msg;
	enum log_flags prev = 0;

	if (log_much == NULL)
		return -ENOMEM;

	log_count = 0;
	while (log_seq < log_next_seq) {
		msg = log_from_idx(log_index);
		count = msg_print_text(msg, true, buff, sizeof(buff));
		prev = msg->flags;

		if (log_count + count > log_buf_len + LOG_MUCH_PLUS_LEN)
			break;
		memcpy(log_much + log_count, buff, count);
		log_count += count;

		log_index = log_next(log_index);
		log_seq++;
	}
	return 0;
}

static void log_much_do_check_and_delay(struct printk_log *msg)
{
	if (delta_count * DETECT_TIME >  detect_count * delta_time) {
		if (!parse_log_file()) {
			t_base = msg->ts_nsec + DELAY_TIME;
			flag_toomuch = true;
		}
	}
}

static int log_much_show(struct seq_file *m, void *v)
{
	if (log_much == NULL) {
		seq_puts(m, "log buff is null.\n");
		return 0;
	}
	seq_write(m, log_much, log_count);
	return 0;
}

static int log_much_open(struct inode *inode, struct file *file)
{
	return single_open(file, log_much_show, inode->i_private);
}

static const struct file_operations log_much_ops = {
	.owner = THIS_MODULE,
	.open = log_much_open,
	.read = seq_read,
	.llseek = seq_lseek,
	.release = single_release,
};
#endif

static int __add_preferred_console(char *name, int idx, char *options,
				   char *brl_options)
=======
static int __add_preferred_console(char *name, int idx, char *options,
				   char *brl_options, bool user_specified)
>>>>>>> upstream/android-13
{
	struct console_cmdline *c;
	int i;

	/*
	 *	See if this tty is not yet registered, and
	 *	if we have a slot free.
	 */
	for (i = 0, c = console_cmdline;
	     i < MAX_CMDLINECONSOLES && c->name[0];
	     i++, c++) {
		if (strcmp(c->name, name) == 0 && c->index == idx) {
			if (!brl_options)
				preferred_console = i;
<<<<<<< HEAD
=======
			if (user_specified)
				c->user_specified = true;
>>>>>>> upstream/android-13
			return 0;
		}
	}
	if (i == MAX_CMDLINECONSOLES)
		return -E2BIG;
	if (!brl_options)
		preferred_console = i;
	strlcpy(c->name, name, sizeof(c->name));
	c->options = options;
<<<<<<< HEAD
=======
	c->user_specified = user_specified;
>>>>>>> upstream/android-13
	braille_set_options(c, brl_options);

	c->index = idx;
	return 0;
}

static int __init console_msg_format_setup(char *str)
{
	if (!strcmp(str, "syslog"))
		console_msg_format = MSG_FORMAT_SYSLOG;
	if (!strcmp(str, "default"))
		console_msg_format = MSG_FORMAT_DEFAULT;
	return 1;
}
__setup("console_msg_format=", console_msg_format_setup);

/*
 * Set up a console.  Called via do_early_param() in init/main.c
 * for each "console=" parameter in the boot command line.
 */
static int __init console_setup(char *str)
{
	char buf[sizeof(console_cmdline[0].name) + 4]; /* 4 for "ttyS" */
	char *s, *options, *brl_options = NULL;
	int idx;

<<<<<<< HEAD
	if (str[0] == 0)
		return 1;
=======
	/*
	 * console="" or console=null have been suggested as a way to
	 * disable console output. Use ttynull that has been created
	 * for exactly this purpose.
	 */
	if (str[0] == 0 || strcmp(str, "null") == 0) {
		__add_preferred_console("ttynull", 0, NULL, NULL, true);
		return 1;
	}
>>>>>>> upstream/android-13

	if (_braille_console_setup(&str, &brl_options))
		return 1;

	/*
	 * Decode str into name, index, options.
	 */
	if (str[0] >= '0' && str[0] <= '9') {
		strcpy(buf, "ttyS");
		strncpy(buf + 4, str, sizeof(buf) - 5);
	} else {
		strncpy(buf, str, sizeof(buf) - 1);
	}
	buf[sizeof(buf) - 1] = 0;
	options = strchr(str, ',');
	if (options)
		*(options++) = 0;
#ifdef __sparc__
	if (!strcmp(str, "ttya"))
		strcpy(buf, "ttyS0");
	if (!strcmp(str, "ttyb"))
		strcpy(buf, "ttyS1");
#endif
	for (s = buf; *s; s++)
		if (isdigit(*s) || *s == ',')
			break;
	idx = simple_strtoul(s, NULL, 10);
	*s = 0;

<<<<<<< HEAD
	__add_preferred_console(buf, idx, options, brl_options);
=======
	__add_preferred_console(buf, idx, options, brl_options, true);
>>>>>>> upstream/android-13
	console_set_on_cmdline = 1;
	return 1;
}
__setup("console=", console_setup);

/**
 * add_preferred_console - add a device to the list of preferred consoles.
 * @name: device name
 * @idx: device index
 * @options: options for this console
 *
 * The last preferred console added will be used for kernel messages
 * and stdin/out/err for init.  Normally this is used by console_setup
 * above to handle user-supplied console arguments; however it can also
 * be used by arch-specific code either to override the user or more
 * commonly to provide a default console (ie from PROM variables) when
 * the user has not supplied one.
 */
int add_preferred_console(char *name, int idx, char *options)
{
<<<<<<< HEAD
	return __add_preferred_console(name, idx, options, NULL);
=======
	return __add_preferred_console(name, idx, options, NULL, false);
>>>>>>> upstream/android-13
}

bool console_suspend_enabled = true;
EXPORT_SYMBOL(console_suspend_enabled);

static int __init console_suspend_disable(char *str)
{
	console_suspend_enabled = false;
	return 1;
}
__setup("no_console_suspend", console_suspend_disable);
module_param_named(console_suspend, console_suspend_enabled,
		bool, S_IRUGO | S_IWUSR);
MODULE_PARM_DESC(console_suspend, "suspend console during suspend"
	" and hibernate operations");

<<<<<<< HEAD
=======
static bool printk_console_no_auto_verbose;

void console_verbose(void)
{
	if (console_loglevel && !printk_console_no_auto_verbose)
		console_loglevel = CONSOLE_LOGLEVEL_MOTORMOUTH;
}
EXPORT_SYMBOL_GPL(console_verbose);

module_param_named(console_no_auto_verbose, printk_console_no_auto_verbose, bool, 0644);
MODULE_PARM_DESC(console_no_auto_verbose, "Disable console loglevel raise to highest on oops/panic/etc");

>>>>>>> upstream/android-13
/**
 * suspend_console - suspend the console subsystem
 *
 * This disables printk() while we go into suspend states
 */
void suspend_console(void)
{
	if (!console_suspend_enabled)
		return;
	pr_info("Suspending console(s) (use no_console_suspend to debug)\n");
	console_lock();
	console_suspended = 1;
	up_console_sem();
}

void resume_console(void)
{
	if (!console_suspend_enabled)
		return;
	down_console_sem();
	console_suspended = 0;
	console_unlock();
}

/**
 * console_cpu_notify - print deferred console messages after CPU hotplug
 * @cpu: unused
 *
 * If printk() is called from a CPU that is not online yet, the messages
 * will be printed on the console only if there are CON_ANYTIME consoles.
 * This function is called when a new CPU comes online (or fails to come
 * up) or goes offline.
 */
static int console_cpu_notify(unsigned int cpu)
{
<<<<<<< HEAD
=======
	int flag = 0;

	trace_android_vh_printk_hotplug(&flag);
	if (flag)
		return 0;

>>>>>>> upstream/android-13
	if (!cpuhp_tasks_frozen) {
		/* If trylock fails, someone else is doing the printing */
		if (console_trylock())
			console_unlock();
	}
	return 0;
}

/**
 * console_lock - lock the console system for exclusive use.
 *
 * Acquires a lock which guarantees that the caller has
 * exclusive access to the console system and the console_drivers list.
 *
 * Can sleep, returns nothing.
 */
void console_lock(void)
{
	might_sleep();

	down_console_sem();
	if (console_suspended)
		return;
	console_locked = 1;
	console_may_schedule = 1;
}
EXPORT_SYMBOL(console_lock);

/**
 * console_trylock - try to lock the console system for exclusive use.
 *
 * Try to acquire a lock which guarantees that the caller has exclusive
 * access to the console system and the console_drivers list.
 *
 * returns 1 on success, and 0 on failure to acquire the lock.
 */
int console_trylock(void)
{
	if (down_trylock_console_sem())
		return 0;
	if (console_suspended) {
		up_console_sem();
		return 0;
	}
	console_locked = 1;
	console_may_schedule = 0;
	return 1;
}
EXPORT_SYMBOL(console_trylock);

int is_console_locked(void)
{
	return console_locked;
}
EXPORT_SYMBOL(is_console_locked);

/*
 * Check if we have any console that is capable of printing while cpu is
 * booting or shutting down. Requires console_sem.
 */
static int have_callable_console(void)
{
	struct console *con;

	for_each_console(con)
		if ((con->flags & CON_ENABLED) &&
				(con->flags & CON_ANYTIME))
			return 1;

	return 0;
}

/*
 * Can we actually use the console at this time on this cpu?
 *
 * Console drivers may assume that per-cpu resources have been allocated. So
 * unless they're explicitly marked as being able to cope (CON_ANYTIME) don't
 * call them until this CPU is officially up.
 */
static inline int can_use_console(void)
{
	return cpu_online(raw_smp_processor_id()) || have_callable_console();
}

/**
 * console_unlock - unlock the console system
 *
 * Releases the console_lock which the caller holds on the console system
 * and the console driver list.
 *
 * While the console_lock was held, console output may have been buffered
 * by printk().  If this is the case, console_unlock(); emits
 * the output prior to releasing the lock.
 *
 * If there is output waiting, we wake /dev/kmsg and syslog() users.
 *
 * console_unlock(); may be called from any context.
 */
void console_unlock(void)
{
	static char ext_text[CONSOLE_EXT_LOG_MAX];
<<<<<<< HEAD
	static char text[LOG_LINE_MAX + PREFIX_MAX];
	unsigned long flags;
	bool do_cond_resched, retry;

#ifdef CONFIG_LOG_TOO_MUCH_WARNING
/* length can not beyond 63 because of aee API args limitation */
	char aee_str[63] = {0};
	int add_len;
	u64 period;
	unsigned long rem_nsec;
#endif
#ifdef CONFIG_CONSOLE_LOCK_DURATION_DETECT
	bool block_overtime = false;
	u64 con_dura_time = local_clock();
	u64 current_time;

	len_con_write_ttyS = len_con_write_pstore = 0;
	time_con_write_ttyS = time_con_write_pstore = 0;
	rem_nsec_con_write_ttyS = rem_nsec_con_write_pstore = 0;
#endif
=======
	static char text[CONSOLE_LOG_MAX];
	unsigned long flags;
	bool do_cond_resched, retry;
	struct printk_info info;
	struct printk_record r;
	u64 __maybe_unused next_seq;
>>>>>>> upstream/android-13

	if (console_suspended) {
		up_console_sem();
		return;
	}

<<<<<<< HEAD
=======
	prb_rec_init_rd(&r, &info, text, sizeof(text));

>>>>>>> upstream/android-13
	/*
	 * Console drivers are called with interrupts disabled, so
	 * @console_may_schedule should be cleared before; however, we may
	 * end up dumping a lot of lines, for example, if called from
	 * console registration path, and should invoke cond_resched()
	 * between lines if allowable.  Not doing so can cause a very long
	 * scheduling stall on a slow console leading to RCU stall and
	 * softlockup warnings which exacerbate the issue with more
	 * messages practically incapacitating the system.
	 *
	 * console_trylock() is not able to detect the preemptive
	 * context reliably. Therefore the value must be stored before
<<<<<<< HEAD
	 * and cleared after the the "again" goto label.
=======
	 * and cleared after the "again" goto label.
>>>>>>> upstream/android-13
	 */
	do_cond_resched = console_may_schedule;
again:
	console_may_schedule = 0;

	/*
	 * We released the console_sem lock, so we need to recheck if
	 * cpu is online and (if not) is there at least one CON_ANYTIME
	 * console.
	 */
	if (!can_use_console()) {
		console_locked = 0;
		up_console_sem();
		return;
	}

	for (;;) {
<<<<<<< HEAD
		struct printk_log *msg;
		size_t ext_len = 0;
		size_t len;

		printk_safe_enter_irqsave(flags);
		raw_spin_lock(&logbuf_lock);
		if (console_seq < log_first_seq) {
			len = sprintf(text,
				      "** %llu printk messages dropped **\n",
				      log_first_seq - console_seq);

			/* messages are gone, move to first one */
			console_seq = log_first_seq;
			console_idx = log_first_idx;
		} else {
			len = 0;
		}
skip:
		if (console_seq == log_next_seq)
			break;

#ifdef CONFIG_CONSOLE_LOCK_DURATION_DETECT
		/* console_unlock block time over 2 seconds */
		current_time = local_clock();
		if ((current_time - con_dura_time) > 2000000000ULL) {
			unsigned long tmp_rem_nsec_start = 0,
				tmp_rem_nsec_end = 0;
			block_overtime = true;
			console_status_detected = true;

			rem_nsec_con_write_ttyS = do_div
				(time_con_write_ttyS, 1000000000);
			rem_nsec_con_write_pstore = do_div
				(time_con_write_pstore, 1000000000);
			tmp_rem_nsec_start = do_div(con_dura_time, 1000000000);
			tmp_rem_nsec_end = do_div(current_time, 1000000000);
			memset(conwrite_stat_struct.con_write_statbuf, 0x0,
				sizeof(conwrite_stat_struct.con_write_statbuf)
				- 1);
			if (snprintf(conwrite_stat_struct.con_write_statbuf,
				sizeof(conwrite_stat_struct.con_write_statbuf)
				- 1,
"cpu%d [%lu.%06lu]--[%lu.%06lu] 'ttyS' %lubytes %lu.%06lus, 'pstore' %lubytes %lu.%06lus\n",
				smp_processor_id(),
				(unsigned long)con_dura_time,
				tmp_rem_nsec_start/1000,
				(unsigned long)current_time,
				tmp_rem_nsec_end/1000,
				(unsigned long)len_con_write_ttyS,
				(unsigned long)time_con_write_ttyS,
				rem_nsec_con_write_ttyS/1000,
				(unsigned long)len_con_write_pstore,
				(unsigned long)time_con_write_pstore,
				rem_nsec_con_write_pstore/1000) < 0) {
			conwrite_stat_struct.con_write_statbuf[0] = 'N';
			conwrite_stat_struct.con_write_statbuf[1] = 'A';
			conwrite_stat_struct.con_write_statbuf[2] = '\0';
				}
			break;
		}
		/* print the uart status next time enter the console_unlock */
		if (console_status_detected) {
			len += snprintf(text + len,
				strlen(conwrite_stat_struct.con_write_statbuf),
				conwrite_stat_struct.con_write_statbuf);
			console_status_detected = false;
		}
#endif

		msg = log_from_idx(console_idx);
		if (suppress_message_printing(msg->level)) {
=======
		size_t ext_len = 0;
		int handover;
		size_t len;

skip:
		if (!prb_read_valid(prb, console_seq, &r))
			break;

		if (console_seq != r.info->seq) {
			console_dropped += r.info->seq - console_seq;
			console_seq = r.info->seq;
		}

		if (suppress_message_printing(r.info->level)) {
>>>>>>> upstream/android-13
			/*
			 * Skip record we have buffered and already printed
			 * directly to the console when we received it, and
			 * record that has level above the console loglevel.
			 */
<<<<<<< HEAD
			console_idx = log_next(console_idx);
=======
>>>>>>> upstream/android-13
			console_seq++;
			goto skip;
		}

		/* Output to all consoles once old messages replayed. */
		if (unlikely(exclusive_console &&
			     console_seq >= exclusive_console_stop_seq)) {
			exclusive_console = NULL;
		}

<<<<<<< HEAD
		len += msg_print_text(msg,
				console_msg_format & MSG_FORMAT_SYSLOG,
				text + len,
				sizeof(text) - len);
		if (nr_ext_console_drivers) {
			ext_len = msg_print_ext_header(ext_text,
						sizeof(ext_text),
						msg, console_seq);
			ext_len += msg_print_ext_body(ext_text + ext_len,
						sizeof(ext_text) - ext_len,
						log_dict(msg), msg->dict_len,
						log_text(msg), msg->text_len);
		}
		console_idx = log_next(console_idx);
		console_seq++;
		raw_spin_unlock(&logbuf_lock);
=======
		/*
		 * Handle extended console text first because later
		 * record_print_text() will modify the record buffer in-place.
		 */
		if (nr_ext_console_drivers) {
			ext_len = info_print_ext_header(ext_text,
						sizeof(ext_text),
						r.info);
			ext_len += msg_print_ext_body(ext_text + ext_len,
						sizeof(ext_text) - ext_len,
						&r.text_buf[0],
						r.info->text_len,
						&r.info->dev_info);
		}
		len = record_print_text(&r,
				console_msg_format & MSG_FORMAT_SYSLOG,
				printk_time);
		console_seq++;
>>>>>>> upstream/android-13

		/*
		 * While actively printing out messages, if another printk()
		 * were to occur on another CPU, it may wait for this one to
		 * finish. This task can not be preempted if there is a
		 * waiter waiting to take over.
<<<<<<< HEAD
		 */
		console_lock_spinning_enable();

		stop_critical_timings();	/* don't trace print latency */
#ifdef CONFIG_LOG_TOO_MUCH_WARNING
		if (flag_toomuch == true) {
			flag_toomuch = false;
			add_len = scnprintf(aee_str, 63,
				"Printk too much: >%d L/s, L: %llu, ",
				detect_count, delta_count);
			if (add_len + 12 <= 63) {
				period = delta_time;
				rem_nsec = do_div(period, 1000000000);
				scnprintf(aee_str + add_len, 63 - add_len,
					"S: %llu.%06lu\n",
					period, rem_nsec / 1000);
			}
			aee_kernel_warning_api(__FILE__, __LINE__,
				DB_OPT_PRINTK_TOO_MUCH | DB_OPT_DUMMY_DUMP,
				aee_str, "Need to shrink kernel log");
		} else
			call_console_drivers(ext_text, ext_len, text, len);
#else
		call_console_drivers(ext_text, ext_len, text, len);
#endif
		start_critical_timings();

		if (console_lock_spinning_disable_and_check()) {
			printk_safe_exit_irqrestore(flags);
			return;
		}

		printk_safe_exit_irqrestore(flags);
=======
		 *
		 * Interrupts are disabled because the hand over to a waiter
		 * must not be interrupted until the hand over is completed
		 * (@console_waiter is cleared).
		 */
		printk_safe_enter_irqsave(flags);
		console_lock_spinning_enable();

		stop_critical_timings();	/* don't trace print latency */
		call_console_drivers(ext_text, ext_len, text, len);
		start_critical_timings();

		handover = console_lock_spinning_disable_and_check();
		printk_safe_exit_irqrestore(flags);
		if (handover)
			return;
>>>>>>> upstream/android-13

		if (do_cond_resched)
			cond_resched();
	}

<<<<<<< HEAD
	console_locked = 0;

	raw_spin_unlock(&logbuf_lock);

=======
	/* Get consistent value of the next-to-be-used sequence number. */
	next_seq = console_seq;

	console_locked = 0;
>>>>>>> upstream/android-13
	up_console_sem();

	/*
	 * Someone could have filled up the buffer again, so re-check if there's
	 * something to flush. In case we cannot trylock the console_sem again,
	 * there's a new owner and the console_unlock() from them will do the
	 * flush, no worries.
	 */
<<<<<<< HEAD
	raw_spin_lock(&logbuf_lock);
#ifdef CONFIG_CONSOLE_LOCK_DURATION_DETECT
	retry = !block_overtime && (console_seq != log_next_seq);
#else
	retry = console_seq != log_next_seq;
#endif
	raw_spin_unlock(&logbuf_lock);
	printk_safe_exit_irqrestore(flags);

=======
	retry = prb_read_valid(prb, next_seq, NULL);
>>>>>>> upstream/android-13
	if (retry && console_trylock())
		goto again;
}
EXPORT_SYMBOL(console_unlock);

/**
 * console_conditional_schedule - yield the CPU if required
 *
 * If the console code is currently allowed to sleep, and
 * if this CPU should yield the CPU to another task, do
 * so here.
 *
 * Must be called within console_lock();.
 */
void __sched console_conditional_schedule(void)
{
	if (console_may_schedule)
		cond_resched();
}
EXPORT_SYMBOL(console_conditional_schedule);

void console_unblank(void)
{
	struct console *c;

	/*
	 * console_unblank can no longer be called in interrupt context unless
	 * oops_in_progress is set to 1..
	 */
	if (oops_in_progress) {
		if (down_trylock_console_sem() != 0)
			return;
	} else
		console_lock();

	console_locked = 1;
	console_may_schedule = 0;
	for_each_console(c)
		if ((c->flags & CON_ENABLED) && c->unblank)
			c->unblank();
	console_unlock();
}

/**
 * console_flush_on_panic - flush console content on panic
<<<<<<< HEAD
 *
 * Immediately output all pending messages no matter what.
 */
void console_flush_on_panic(void)
=======
 * @mode: flush all messages in buffer or just the pending ones
 *
 * Immediately output all pending messages no matter what.
 */
void console_flush_on_panic(enum con_flush_mode mode)
>>>>>>> upstream/android-13
{
	/*
	 * If someone else is holding the console lock, trylock will fail
	 * and may_schedule may be set.  Ignore and proceed to unlock so
	 * that messages are flushed out.  As this can be called from any
	 * context and we don't want to get preempted while flushing,
	 * ensure may_schedule is cleared.
	 */
	console_trylock();
	console_may_schedule = 0;
<<<<<<< HEAD
=======

	if (mode == CONSOLE_REPLAY_ALL)
		console_seq = prb_first_valid_seq(prb);
>>>>>>> upstream/android-13
	console_unlock();
}

/*
 * Return the console tty driver structure and its associated index
 */
struct tty_driver *console_device(int *index)
{
	struct console *c;
	struct tty_driver *driver = NULL;

	console_lock();
	for_each_console(c) {
		if (!c->device)
			continue;
		driver = c->device(c, index);
		if (driver)
			break;
	}
	console_unlock();
	return driver;
}

/*
 * Prevent further output on the passed console device so that (for example)
 * serial drivers can disable console output before suspending a port, and can
 * re-enable output afterwards.
 */
void console_stop(struct console *console)
{
	console_lock();
	console->flags &= ~CON_ENABLED;
	console_unlock();
}
EXPORT_SYMBOL(console_stop);

void console_start(struct console *console)
{
	console_lock();
	console->flags |= CON_ENABLED;
	console_unlock();
}
EXPORT_SYMBOL(console_start);

static int __read_mostly keep_bootcon;

static int __init keep_bootcon_setup(char *str)
{
	keep_bootcon = 1;
	pr_info("debug: skip boot console de-registration.\n");

	return 0;
}

early_param("keep_bootcon", keep_bootcon_setup);

/*
<<<<<<< HEAD
=======
 * This is called by register_console() to try to match
 * the newly registered console with any of the ones selected
 * by either the command line or add_preferred_console() and
 * setup/enable it.
 *
 * Care need to be taken with consoles that are statically
 * enabled such as netconsole
 */
static int try_enable_new_console(struct console *newcon, bool user_specified)
{
	struct console_cmdline *c;
	int i, err;

	for (i = 0, c = console_cmdline;
	     i < MAX_CMDLINECONSOLES && c->name[0];
	     i++, c++) {
		if (c->user_specified != user_specified)
			continue;
		if (!newcon->match ||
		    newcon->match(newcon, c->name, c->index, c->options) != 0) {
			/* default matching */
			BUILD_BUG_ON(sizeof(c->name) != sizeof(newcon->name));
			if (strcmp(c->name, newcon->name) != 0)
				continue;
			if (newcon->index >= 0 &&
			    newcon->index != c->index)
				continue;
			if (newcon->index < 0)
				newcon->index = c->index;

			if (_braille_register_console(newcon, c))
				return 0;

			if (newcon->setup &&
			    (err = newcon->setup(newcon, c->options)) != 0)
				return err;
		}
		newcon->flags |= CON_ENABLED;
		if (i == preferred_console) {
			newcon->flags |= CON_CONSDEV;
			has_preferred_console = true;
		}
		return 0;
	}

	/*
	 * Some consoles, such as pstore and netconsole, can be enabled even
	 * without matching. Accept the pre-enabled consoles only when match()
	 * and setup() had a chance to be called.
	 */
	if (newcon->flags & CON_ENABLED && c->user_specified ==	user_specified)
		return 0;

	return -ENOENT;
}

/*
>>>>>>> upstream/android-13
 * The console driver calls this routine during kernel initialization
 * to register the console printing procedure with printk() and to
 * print any messages that were printed by the kernel before the
 * console driver was initialized.
 *
 * This can happen pretty early during the boot process (because of
 * early_printk) - sometimes before setup_arch() completes - be careful
 * of what kernel features are used - they may not be initialised yet.
 *
 * There are two types of consoles - bootconsoles (early_printk) and
 * "real" consoles (everything which is not a bootconsole) which are
 * handled differently.
 *  - Any number of bootconsoles can be registered at any time.
 *  - As soon as a "real" console is registered, all bootconsoles
 *    will be unregistered automatically.
 *  - Once a "real" console is registered, any attempt to register a
 *    bootconsoles will be rejected
 */
void register_console(struct console *newcon)
{
<<<<<<< HEAD
	int i;
	unsigned long flags;
	struct console *bcon = NULL;
	struct console_cmdline *c;
	static bool has_preferred;

	if (console_drivers)
		for_each_console(bcon)
			if (WARN(bcon == newcon,
					"console '%s%d' already registered\n",
					bcon->name, bcon->index))
				return;
=======
	struct console *bcon = NULL;
	int err;

	for_each_console(bcon) {
		if (WARN(bcon == newcon, "console '%s%d' already registered\n",
					 bcon->name, bcon->index))
			return;
	}
>>>>>>> upstream/android-13

	/*
	 * before we register a new CON_BOOT console, make sure we don't
	 * already have a valid console
	 */
<<<<<<< HEAD
	if (console_drivers && newcon->flags & CON_BOOT) {
		/* find the last or real console */
=======
	if (newcon->flags & CON_BOOT) {
>>>>>>> upstream/android-13
		for_each_console(bcon) {
			if (!(bcon->flags & CON_BOOT)) {
				pr_info("Too late to register bootconsole %s%d\n",
					newcon->name, newcon->index);
				return;
			}
		}
	}

	if (console_drivers && console_drivers->flags & CON_BOOT)
		bcon = console_drivers;

<<<<<<< HEAD
	if (!has_preferred || bcon || !console_drivers)
		has_preferred = preferred_console >= 0;
=======
	if (!has_preferred_console || bcon || !console_drivers)
		has_preferred_console = preferred_console >= 0;
>>>>>>> upstream/android-13

	/*
	 *	See if we want to use this console driver. If we
	 *	didn't select a console we take the first one
	 *	that registers here.
	 */
<<<<<<< HEAD
	if (!has_preferred) {
=======
	if (!has_preferred_console) {
>>>>>>> upstream/android-13
		if (newcon->index < 0)
			newcon->index = 0;
		if (newcon->setup == NULL ||
		    newcon->setup(newcon, NULL) == 0) {
			newcon->flags |= CON_ENABLED;
			if (newcon->device) {
				newcon->flags |= CON_CONSDEV;
<<<<<<< HEAD
				has_preferred = true;
=======
				has_preferred_console = true;
>>>>>>> upstream/android-13
			}
		}
	}

<<<<<<< HEAD
	/*
	 *	See if this console matches one we selected on
	 *	the command line.
	 */
	for (i = 0, c = console_cmdline;
	     i < MAX_CMDLINECONSOLES && c->name[0];
	     i++, c++) {
		if (!newcon->match ||
		    newcon->match(newcon, c->name, c->index, c->options) != 0) {
			/* default matching */
			BUILD_BUG_ON(sizeof(c->name) != sizeof(newcon->name));
			if (strcmp(c->name, newcon->name) != 0)
				continue;
			if (newcon->index >= 0 &&
			    newcon->index != c->index)
				continue;
			if (newcon->index < 0)
				newcon->index = c->index;

			if (_braille_register_console(newcon, c))
				return;

			if (newcon->setup &&
			    newcon->setup(newcon, c->options) != 0)
				break;
		}

		newcon->flags |= CON_ENABLED;
		if (i == preferred_console) {
			newcon->flags |= CON_CONSDEV;
			has_preferred = true;
		}
		break;
	}

	if (!(newcon->flags & CON_ENABLED))
=======
	/* See if this console matches one we selected on the command line */
	err = try_enable_new_console(newcon, true);

	/* If not, try to match against the platform default(s) */
	if (err == -ENOENT)
		err = try_enable_new_console(newcon, false);

	/* printk() messages are not printed to the Braille console. */
	if (err || newcon->flags & CON_BRL)
>>>>>>> upstream/android-13
		return;

	/*
	 * If we have a bootconsole, and are switching to a real console,
	 * don't print everything out again, since when the boot console, and
	 * the real console are the same physical device, it's annoying to
	 * see the beginning boot messages twice
	 */
	if (bcon && ((newcon->flags & (CON_CONSDEV | CON_BOOT)) == CON_CONSDEV))
		newcon->flags &= ~CON_PRINTBUFFER;

	/*
	 *	Put this console in the list - keep the
	 *	preferred driver at the head of the list.
	 */
	console_lock();
	if ((newcon->flags & CON_CONSDEV) || console_drivers == NULL) {
		newcon->next = console_drivers;
		console_drivers = newcon;
		if (newcon->next)
			newcon->next->flags &= ~CON_CONSDEV;
<<<<<<< HEAD
=======
		/* Ensure this flag is always set for the head of the list */
		newcon->flags |= CON_CONSDEV;
>>>>>>> upstream/android-13
	} else {
		newcon->next = console_drivers->next;
		console_drivers->next = newcon;
	}

	if (newcon->flags & CON_EXTENDED)
<<<<<<< HEAD
		if (!nr_ext_console_drivers++)
			pr_info("printk: continuation disabled due to ext consoles, expect more fragments in /dev/kmsg\n");
=======
		nr_ext_console_drivers++;
>>>>>>> upstream/android-13

	if (newcon->flags & CON_PRINTBUFFER) {
		/*
		 * console_unlock(); will print out the buffered messages
		 * for us.
<<<<<<< HEAD
		 */
		logbuf_lock_irqsave(flags);
		/*
=======
		 *
>>>>>>> upstream/android-13
		 * We're about to replay the log buffer.  Only do this to the
		 * just-registered console to avoid excessive message spam to
		 * the already-registered consoles.
		 *
		 * Set exclusive_console with disabled interrupts to reduce
		 * race window with eventual console_flush_on_panic() that
		 * ignores console_lock.
		 */
		exclusive_console = newcon;
		exclusive_console_stop_seq = console_seq;
<<<<<<< HEAD
		console_seq = syslog_seq;
		console_idx = syslog_idx;
		logbuf_unlock_irqrestore(flags);
=======

		/* Get a consistent copy of @syslog_seq. */
		mutex_lock(&syslog_lock);
		console_seq = syslog_seq;
		mutex_unlock(&syslog_lock);
>>>>>>> upstream/android-13
	}
	console_unlock();
	console_sysfs_notify();

	/*
	 * By unregistering the bootconsoles after we enable the real console
	 * we get the "console xxx enabled" message on all the consoles -
	 * boot consoles, real consoles, etc - this is to ensure that end
	 * users know there might be something in the kernel's log buffer that
	 * went to the bootconsole (that they do not see on the real console)
	 */
	pr_info("%sconsole [%s%d] enabled\n",
		(newcon->flags & CON_BOOT) ? "boot" : "" ,
		newcon->name, newcon->index);
	if (bcon &&
	    ((newcon->flags & (CON_CONSDEV | CON_BOOT)) == CON_CONSDEV) &&
	    !keep_bootcon) {
		/* We need to iterate through all boot consoles, to make
		 * sure we print everything out, before we unregister them.
		 */
		for_each_console(bcon)
			if (bcon->flags & CON_BOOT)
				unregister_console(bcon);
	}
}
EXPORT_SYMBOL(register_console);

int unregister_console(struct console *console)
{
<<<<<<< HEAD
        struct console *a, *b;
=======
	struct console *con;
>>>>>>> upstream/android-13
	int res;

	pr_info("%sconsole [%s%d] disabled\n",
		(console->flags & CON_BOOT) ? "boot" : "" ,
		console->name, console->index);

	res = _braille_unregister_console(console);
<<<<<<< HEAD
	if (res)
		return res;

	res = 1;
=======
	if (res < 0)
		return res;
	if (res > 0)
		return 0;

	res = -ENODEV;
>>>>>>> upstream/android-13
	console_lock();
	if (console_drivers == console) {
		console_drivers=console->next;
		res = 0;
<<<<<<< HEAD
	} else if (console_drivers) {
		for (a=console_drivers->next, b=console_drivers ;
		     a; b=a, a=b->next) {
			if (a == console) {
				b->next = a->next;
=======
	} else {
		for_each_console(con) {
			if (con->next == console) {
				con->next = console->next;
>>>>>>> upstream/android-13
				res = 0;
				break;
			}
		}
	}

<<<<<<< HEAD
	if (!res && (console->flags & CON_EXTENDED))
=======
	if (res)
		goto out_disable_unlock;

	if (console->flags & CON_EXTENDED)
>>>>>>> upstream/android-13
		nr_ext_console_drivers--;

	/*
	 * If this isn't the last console and it has CON_CONSDEV set, we
	 * need to set it on the next preferred console.
	 */
	if (console_drivers != NULL && console->flags & CON_CONSDEV)
		console_drivers->flags |= CON_CONSDEV;

	console->flags &= ~CON_ENABLED;
	console_unlock();
	console_sysfs_notify();
<<<<<<< HEAD
=======

	if (console->exit)
		res = console->exit(console);

	return res;

out_disable_unlock:
	console->flags &= ~CON_ENABLED;
	console_unlock();

>>>>>>> upstream/android-13
	return res;
}
EXPORT_SYMBOL(unregister_console);

/*
 * Initialize the console device. This is called *early*, so
 * we can't necessarily depend on lots of kernel help here.
 * Just do some early initializations, and do the complex setup
 * later.
 */
void __init console_init(void)
{
	int ret;
	initcall_t call;
	initcall_entry_t *ce;

	/* Setup the default TTY line discipline. */
	n_tty_init();

	/*
	 * set up the console device so that later boot sequences can
	 * inform about problems etc..
	 */
	ce = __con_initcall_start;
	trace_initcall_level("console");
	while (ce < __con_initcall_end) {
		call = initcall_from_entry(ce);
		trace_initcall_start(call);
		ret = call();
		trace_initcall_finish(call, ret);
		ce++;
	}
}

/*
 * Some boot consoles access data that is in the init section and which will
 * be discarded after the initcalls have been run. To make sure that no code
 * will access this data, unregister the boot consoles in a late initcall.
 *
 * If for some reason, such as deferred probe or the driver being a loadable
 * module, the real console hasn't registered yet at this point, there will
 * be a brief interval in which no messages are logged to the console, which
 * makes it difficult to diagnose problems that occur during this time.
 *
 * To mitigate this problem somewhat, only unregister consoles whose memory
 * intersects with the init section. Note that all other boot consoles will
<<<<<<< HEAD
 * get unregistred when the real preferred console is registered.
=======
 * get unregistered when the real preferred console is registered.
>>>>>>> upstream/android-13
 */
static int __init printk_late_init(void)
{
	struct console *con;
<<<<<<< HEAD
#ifdef CONFIG_LOG_TOO_MUCH_WARNING
	struct proc_dir_entry *entry;
#endif
=======
>>>>>>> upstream/android-13
	int ret;

	for_each_console(con) {
		if (!(con->flags & CON_BOOT))
			continue;

		/* Check addresses that might be used for enabled consoles. */
		if (init_section_intersects(con, sizeof(*con)) ||
		    init_section_contains(con->write, 0) ||
		    init_section_contains(con->read, 0) ||
		    init_section_contains(con->device, 0) ||
		    init_section_contains(con->unblank, 0) ||
		    init_section_contains(con->data, 0)) {
			/*
			 * Please, consider moving the reported consoles out
			 * of the init section.
			 */
			pr_warn("bootconsole [%s%d] uses init memory and must be disabled even before the real one is ready\n",
				con->name, con->index);
			unregister_console(con);
		}
	}
	ret = cpuhp_setup_state_nocalls(CPUHP_PRINTK_DEAD, "printk:dead", NULL,
					console_cpu_notify);
	WARN_ON(ret < 0);
	ret = cpuhp_setup_state_nocalls(CPUHP_AP_ONLINE_DYN, "printk:online",
					console_cpu_notify, NULL);
	WARN_ON(ret < 0);
<<<<<<< HEAD
#ifdef CONFIG_LOG_TOO_MUCH_WARNING
	entry = proc_create("log_much", 0444, NULL, &log_much_ops);
	if (!entry) {
		pr_notice("printk: failed to create proc log much entry\n");
		return 1;
	}
	log_much = kmalloc(log_buf_len + LOG_MUCH_PLUS_LEN, GFP_KERNEL);
#endif
=======
>>>>>>> upstream/android-13
	return 0;
}
late_initcall(printk_late_init);

#if defined CONFIG_PRINTK
/*
 * Delayed printk version, for scheduler-internal messages:
 */
#define PRINTK_PENDING_WAKEUP	0x01
#define PRINTK_PENDING_OUTPUT	0x02

static DEFINE_PER_CPU(int, printk_pending);

static void wake_up_klogd_work_func(struct irq_work *irq_work)
{
	int pending = __this_cpu_xchg(printk_pending, 0);

	if (pending & PRINTK_PENDING_OUTPUT) {
		/* If trylock fails, someone else is doing the printing */
		if (console_trylock())
			console_unlock();
	}

	if (pending & PRINTK_PENDING_WAKEUP)
		wake_up_interruptible(&log_wait);
}

<<<<<<< HEAD
static DEFINE_PER_CPU(struct irq_work, wake_up_klogd_work) = {
	.func = wake_up_klogd_work_func,
	.flags = IRQ_WORK_LAZY,
};
=======
static DEFINE_PER_CPU(struct irq_work, wake_up_klogd_work) =
	IRQ_WORK_INIT_LAZY(wake_up_klogd_work_func);
>>>>>>> upstream/android-13

void wake_up_klogd(void)
{
	if (!printk_percpu_data_ready())
		return;

	preempt_disable();
	if (waitqueue_active(&log_wait)) {
		this_cpu_or(printk_pending, PRINTK_PENDING_WAKEUP);
		irq_work_queue(this_cpu_ptr(&wake_up_klogd_work));
	}
	preempt_enable();
}

void defer_console_output(void)
{
	if (!printk_percpu_data_ready())
		return;

	preempt_disable();
	__this_cpu_or(printk_pending, PRINTK_PENDING_OUTPUT);
	irq_work_queue(this_cpu_ptr(&wake_up_klogd_work));
	preempt_enable();
}

<<<<<<< HEAD
=======
void printk_trigger_flush(void)
{
	defer_console_output();
}

>>>>>>> upstream/android-13
int vprintk_deferred(const char *fmt, va_list args)
{
	int r;

<<<<<<< HEAD
	r = vprintk_emit(0, LOGLEVEL_SCHED, NULL, 0, fmt, args);
=======
	r = vprintk_emit(0, LOGLEVEL_SCHED, NULL, fmt, args);
>>>>>>> upstream/android-13
	defer_console_output();

	return r;
}

<<<<<<< HEAD
int printk_deferred(const char *fmt, ...)
=======
int _printk_deferred(const char *fmt, ...)
>>>>>>> upstream/android-13
{
	va_list args;
	int r;

	va_start(args, fmt);
	r = vprintk_deferred(fmt, args);
	va_end(args);

	return r;
}
<<<<<<< HEAD
=======
EXPORT_SYMBOL_GPL(_printk_deferred);
>>>>>>> upstream/android-13

/*
 * printk rate limiting, lifted from the networking subsystem.
 *
 * This enforces a rate limit: not more than 10 kernel messages
 * every 5s to make a denial-of-service attack impossible.
 */
DEFINE_RATELIMIT_STATE(printk_ratelimit_state, 5 * HZ, 10);

int __printk_ratelimit(const char *func)
{
	return ___ratelimit(&printk_ratelimit_state, func);
}
EXPORT_SYMBOL(__printk_ratelimit);

/**
 * printk_timed_ratelimit - caller-controlled printk ratelimiting
 * @caller_jiffies: pointer to caller's state
 * @interval_msecs: minimum interval between prints
 *
 * printk_timed_ratelimit() returns true if more than @interval_msecs
 * milliseconds have elapsed since the last time printk_timed_ratelimit()
 * returned true.
 */
bool printk_timed_ratelimit(unsigned long *caller_jiffies,
			unsigned int interval_msecs)
{
	unsigned long elapsed = jiffies - *caller_jiffies;

	if (*caller_jiffies && elapsed <= msecs_to_jiffies(interval_msecs))
		return false;

	*caller_jiffies = jiffies;
	return true;
}
EXPORT_SYMBOL(printk_timed_ratelimit);

static DEFINE_SPINLOCK(dump_list_lock);
static LIST_HEAD(dump_list);

/**
 * kmsg_dump_register - register a kernel log dumper.
 * @dumper: pointer to the kmsg_dumper structure
 *
 * Adds a kernel log dumper to the system. The dump callback in the
 * structure will be called when the kernel oopses or panics and must be
 * set. Returns zero on success and %-EINVAL or %-EBUSY otherwise.
 */
int kmsg_dump_register(struct kmsg_dumper *dumper)
{
	unsigned long flags;
	int err = -EBUSY;

	/* The dump callback needs to be set */
	if (!dumper->dump)
		return -EINVAL;

	spin_lock_irqsave(&dump_list_lock, flags);
	/* Don't allow registering multiple times */
	if (!dumper->registered) {
		dumper->registered = 1;
		list_add_tail_rcu(&dumper->list, &dump_list);
		err = 0;
	}
	spin_unlock_irqrestore(&dump_list_lock, flags);

	return err;
}
EXPORT_SYMBOL_GPL(kmsg_dump_register);

/**
 * kmsg_dump_unregister - unregister a kmsg dumper.
 * @dumper: pointer to the kmsg_dumper structure
 *
 * Removes a dump device from the system. Returns zero on success and
 * %-EINVAL otherwise.
 */
int kmsg_dump_unregister(struct kmsg_dumper *dumper)
{
	unsigned long flags;
	int err = -EINVAL;

	spin_lock_irqsave(&dump_list_lock, flags);
	if (dumper->registered) {
		dumper->registered = 0;
		list_del_rcu(&dumper->list);
		err = 0;
	}
	spin_unlock_irqrestore(&dump_list_lock, flags);
	synchronize_rcu();

	return err;
}
EXPORT_SYMBOL_GPL(kmsg_dump_unregister);

static bool always_kmsg_dump;
module_param_named(always_kmsg_dump, always_kmsg_dump, bool, S_IRUGO | S_IWUSR);

<<<<<<< HEAD
=======
const char *kmsg_dump_reason_str(enum kmsg_dump_reason reason)
{
	switch (reason) {
	case KMSG_DUMP_PANIC:
		return "Panic";
	case KMSG_DUMP_OOPS:
		return "Oops";
	case KMSG_DUMP_EMERG:
		return "Emergency";
	case KMSG_DUMP_SHUTDOWN:
		return "Shutdown";
	default:
		return "Unknown";
	}
}
EXPORT_SYMBOL_GPL(kmsg_dump_reason_str);

>>>>>>> upstream/android-13
/**
 * kmsg_dump - dump kernel log to kernel message dumpers.
 * @reason: the reason (oops, panic etc) for dumping
 *
 * Call each of the registered dumper's dump() callback, which can
 * retrieve the kmsg records with kmsg_dump_get_line() or
 * kmsg_dump_get_buffer().
 */
void kmsg_dump(enum kmsg_dump_reason reason)
{
	struct kmsg_dumper *dumper;
<<<<<<< HEAD
	unsigned long flags;

	if ((reason > KMSG_DUMP_OOPS) && !always_kmsg_dump)
		return;

	rcu_read_lock();
	list_for_each_entry_rcu(dumper, &dump_list, list) {
		if (dumper->max_reason && reason > dumper->max_reason)
			continue;

		/* initialize iterator with data about the stored records */
		dumper->active = true;

		logbuf_lock_irqsave(flags);
		dumper->cur_seq = clear_seq;
		dumper->cur_idx = clear_idx;
		dumper->next_seq = log_next_seq;
		dumper->next_idx = log_next_idx;
		logbuf_unlock_irqrestore(flags);

		/* invoke dumper which will iterate over records */
		dumper->dump(dumper, reason);

		/* reset iterator */
		dumper->active = false;
=======

	rcu_read_lock();
	list_for_each_entry_rcu(dumper, &dump_list, list) {
		enum kmsg_dump_reason max_reason = dumper->max_reason;

		/*
		 * If client has not provided a specific max_reason, default
		 * to KMSG_DUMP_OOPS, unless always_kmsg_dump was set.
		 */
		if (max_reason == KMSG_DUMP_UNDEF) {
			max_reason = always_kmsg_dump ? KMSG_DUMP_MAX :
							KMSG_DUMP_OOPS;
		}
		if (reason > max_reason)
			continue;

		/* invoke dumper which will iterate over records */
		dumper->dump(dumper, reason);
>>>>>>> upstream/android-13
	}
	rcu_read_unlock();
}

/**
<<<<<<< HEAD
 * kmsg_dump_get_line_nolock - retrieve one kmsg log line (unlocked version)
 * @dumper: registered kmsg dumper
=======
 * kmsg_dump_get_line - retrieve one kmsg log line
 * @iter: kmsg dump iterator
>>>>>>> upstream/android-13
 * @syslog: include the "<4>" prefixes
 * @line: buffer to copy the line to
 * @size: maximum size of the buffer
 * @len: length of line placed into buffer
 *
 * Start at the beginning of the kmsg buffer, with the oldest kmsg
 * record, and copy one record into the provided buffer.
 *
 * Consecutive calls will return the next available record moving
 * towards the end of the buffer with the youngest messages.
 *
 * A return value of FALSE indicates that there are no more records to
 * read.
<<<<<<< HEAD
 *
 * The function is similar to kmsg_dump_get_line(), but grabs no locks.
 */
bool kmsg_dump_get_line_nolock(struct kmsg_dumper *dumper, bool syslog,
			       char *line, size_t size, size_t *len)
{
	struct printk_log *msg;
	size_t l = 0;
	bool ret = false;

	if (!dumper->active)
		goto out;

	if (dumper->cur_seq < log_first_seq) {
		/* messages are gone, move to first available one */
		dumper->cur_seq = log_first_seq;
		dumper->cur_idx = log_first_idx;
	}

	/* last entry */
	if (dumper->cur_seq >= log_next_seq)
		goto out;

	msg = log_from_idx(dumper->cur_idx);
	l = msg_print_text(msg, syslog, line, size);

	dumper->cur_idx = log_next(dumper->cur_idx);
	dumper->cur_seq++;
=======
 */
bool kmsg_dump_get_line(struct kmsg_dump_iter *iter, bool syslog,
			char *line, size_t size, size_t *len)
{
	u64 min_seq = latched_seq_read_nolock(&clear_seq);
	struct printk_info info;
	unsigned int line_count;
	struct printk_record r;
	size_t l = 0;
	bool ret = false;

	if (iter->cur_seq < min_seq)
		iter->cur_seq = min_seq;

	prb_rec_init_rd(&r, &info, line, size);

	/* Read text or count text lines? */
	if (line) {
		if (!prb_read_valid(prb, iter->cur_seq, &r))
			goto out;
		l = record_print_text(&r, syslog, printk_time);
	} else {
		if (!prb_read_valid_info(prb, iter->cur_seq,
					 &info, &line_count)) {
			goto out;
		}
		l = get_record_print_text_size(&info, line_count, syslog,
					       printk_time);

	}

	iter->cur_seq = r.info->seq + 1;
>>>>>>> upstream/android-13
	ret = true;
out:
	if (len)
		*len = l;
	return ret;
}
<<<<<<< HEAD

/**
 * kmsg_dump_get_line - retrieve one kmsg log line
 * @dumper: registered kmsg dumper
 * @syslog: include the "<4>" prefixes
 * @line: buffer to copy the line to
 * @size: maximum size of the buffer
 * @len: length of line placed into buffer
 *
 * Start at the beginning of the kmsg buffer, with the oldest kmsg
 * record, and copy one record into the provided buffer.
 *
 * Consecutive calls will return the next available record moving
 * towards the end of the buffer with the youngest messages.
 *
 * A return value of FALSE indicates that there are no more records to
 * read.
 */
bool kmsg_dump_get_line(struct kmsg_dumper *dumper, bool syslog,
			char *line, size_t size, size_t *len)
{
	unsigned long flags;
	bool ret;

	logbuf_lock_irqsave(flags);
	ret = kmsg_dump_get_line_nolock(dumper, syslog, line, size, len);
	logbuf_unlock_irqrestore(flags);

	return ret;
}
=======
>>>>>>> upstream/android-13
EXPORT_SYMBOL_GPL(kmsg_dump_get_line);

/**
 * kmsg_dump_get_buffer - copy kmsg log lines
<<<<<<< HEAD
 * @dumper: registered kmsg dumper
 * @syslog: include the "<4>" prefixes
 * @buf: buffer to copy the line to
 * @size: maximum size of the buffer
 * @len: length of line placed into buffer
 *
 * Start at the end of the kmsg buffer and fill the provided buffer
 * with as many of the the *youngest* kmsg records that fit into it.
=======
 * @iter: kmsg dump iterator
 * @syslog: include the "<4>" prefixes
 * @buf: buffer to copy the line to
 * @size: maximum size of the buffer
 * @len_out: length of line placed into buffer
 *
 * Start at the end of the kmsg buffer and fill the provided buffer
 * with as many of the *youngest* kmsg records that fit into it.
>>>>>>> upstream/android-13
 * If the buffer is large enough, all available kmsg records will be
 * copied with a single call.
 *
 * Consecutive calls will fill the buffer with the next block of
 * available older records, not including the earlier retrieved ones.
 *
 * A return value of FALSE indicates that there are no more records to
 * read.
 */
<<<<<<< HEAD
bool kmsg_dump_get_buffer(struct kmsg_dumper *dumper, bool syslog,
			  char *buf, size_t size, size_t *len)
{
	unsigned long flags;
	u64 seq;
	u32 idx;
	u64 next_seq;
	u32 next_idx;
	size_t l = 0;
	bool ret = false;

	if (!dumper->active)
		goto out;

	logbuf_lock_irqsave(flags);
	if (dumper->cur_seq < log_first_seq) {
		/* messages are gone, move to first available one */
		dumper->cur_seq = log_first_seq;
		dumper->cur_idx = log_first_idx;
	}

	/* last entry */
	if (dumper->cur_seq >= dumper->next_seq) {
		logbuf_unlock_irqrestore(flags);
		goto out;
	}

	/* calculate length of entire buffer */
	seq = dumper->cur_seq;
	idx = dumper->cur_idx;
	while (seq < dumper->next_seq) {
		struct printk_log *msg = log_from_idx(idx);

		l += msg_print_text(msg, true, NULL, 0);
		idx = log_next(idx);
		seq++;
	}

	/* move first record forward until length fits into the buffer */
	seq = dumper->cur_seq;
	idx = dumper->cur_idx;
	while (l >= size && seq < dumper->next_seq) {
		struct printk_log *msg = log_from_idx(idx);

		l -= msg_print_text(msg, true, NULL, 0);
		idx = log_next(idx);
		seq++;
	}

	/* last message in next interation */
	next_seq = seq;
	next_idx = idx;

	l = 0;
	while (seq < dumper->next_seq) {
		struct printk_log *msg = log_from_idx(idx);

		l += msg_print_text(msg, syslog, buf + l, size - l);
		idx = log_next(idx);
		seq++;
	}

	dumper->next_seq = next_seq;
	dumper->next_idx = next_idx;
	ret = true;
	logbuf_unlock_irqrestore(flags);
out:
	if (len)
		*len = l;
=======
bool kmsg_dump_get_buffer(struct kmsg_dump_iter *iter, bool syslog,
			  char *buf, size_t size, size_t *len_out)
{
	u64 min_seq = latched_seq_read_nolock(&clear_seq);
	struct printk_info info;
	struct printk_record r;
	u64 seq;
	u64 next_seq;
	size_t len = 0;
	bool ret = false;
	bool time = printk_time;

	if (!buf || !size)
		goto out;

	if (iter->cur_seq < min_seq)
		iter->cur_seq = min_seq;

	if (prb_read_valid_info(prb, iter->cur_seq, &info, NULL)) {
		if (info.seq != iter->cur_seq) {
			/* messages are gone, move to first available one */
			iter->cur_seq = info.seq;
		}
	}

	/* last entry */
	if (iter->cur_seq >= iter->next_seq)
		goto out;

	/*
	 * Find first record that fits, including all following records,
	 * into the user-provided buffer for this dump. Pass in size-1
	 * because this function (by way of record_print_text()) will
	 * not write more than size-1 bytes of text into @buf.
	 */
	seq = find_first_fitting_seq(iter->cur_seq, iter->next_seq,
				     size - 1, syslog, time);

	/*
	 * Next kmsg_dump_get_buffer() invocation will dump block of
	 * older records stored right before this one.
	 */
	next_seq = seq;

	prb_rec_init_rd(&r, &info, buf, size);

	len = 0;
	prb_for_each_record(seq, prb, seq, &r) {
		if (r.info->seq >= iter->next_seq)
			break;

		len += record_print_text(&r, syslog, time);

		/* Adjust record to store to remaining buffer space. */
		prb_rec_init_rd(&r, &info, buf + len, size - len);
	}

	iter->next_seq = next_seq;
	ret = true;
out:
	if (len_out)
		*len_out = len;
>>>>>>> upstream/android-13
	return ret;
}
EXPORT_SYMBOL_GPL(kmsg_dump_get_buffer);

/**
<<<<<<< HEAD
 * kmsg_dump_rewind_nolock - reset the interator (unlocked version)
 * @dumper: registered kmsg dumper
 *
 * Reset the dumper's iterator so that kmsg_dump_get_line() and
 * kmsg_dump_get_buffer() can be called again and used multiple
 * times within the same dumper.dump() callback.
 *
 * The function is similar to kmsg_dump_rewind(), but grabs no locks.
 */
void kmsg_dump_rewind_nolock(struct kmsg_dumper *dumper)
{
	dumper->cur_seq = clear_seq;
	dumper->cur_idx = clear_idx;
	dumper->next_seq = log_next_seq;
	dumper->next_idx = log_next_idx;
}

/**
 * kmsg_dump_rewind - reset the interator
 * @dumper: registered kmsg dumper
=======
 * kmsg_dump_rewind - reset the iterator
 * @iter: kmsg dump iterator
>>>>>>> upstream/android-13
 *
 * Reset the dumper's iterator so that kmsg_dump_get_line() and
 * kmsg_dump_get_buffer() can be called again and used multiple
 * times within the same dumper.dump() callback.
 */
<<<<<<< HEAD
void kmsg_dump_rewind(struct kmsg_dumper *dumper)
{
	unsigned long flags;

	logbuf_lock_irqsave(flags);
	kmsg_dump_rewind_nolock(dumper);
	logbuf_unlock_irqrestore(flags);
=======
void kmsg_dump_rewind(struct kmsg_dump_iter *iter)
{
	iter->cur_seq = latched_seq_read_nolock(&clear_seq);
	iter->next_seq = prb_next_seq(prb);
>>>>>>> upstream/android-13
}
EXPORT_SYMBOL_GPL(kmsg_dump_rewind);

#endif
<<<<<<< HEAD
=======

#ifdef CONFIG_SMP
static atomic_t printk_cpulock_owner = ATOMIC_INIT(-1);
static atomic_t printk_cpulock_nested = ATOMIC_INIT(0);

/**
 * __printk_wait_on_cpu_lock() - Busy wait until the printk cpu-reentrant
 *                               spinning lock is not owned by any CPU.
 *
 * Context: Any context.
 */
void __printk_wait_on_cpu_lock(void)
{
	do {
		cpu_relax();
	} while (atomic_read(&printk_cpulock_owner) != -1);
}
EXPORT_SYMBOL(__printk_wait_on_cpu_lock);

/**
 * __printk_cpu_trylock() - Try to acquire the printk cpu-reentrant
 *                          spinning lock.
 *
 * If no processor has the lock, the calling processor takes the lock and
 * becomes the owner. If the calling processor is already the owner of the
 * lock, this function succeeds immediately.
 *
 * Context: Any context. Expects interrupts to be disabled.
 * Return: 1 on success, otherwise 0.
 */
int __printk_cpu_trylock(void)
{
	int cpu;
	int old;

	cpu = smp_processor_id();

	/*
	 * Guarantee loads and stores from this CPU when it is the lock owner
	 * are _not_ visible to the previous lock owner. This pairs with
	 * __printk_cpu_unlock:B.
	 *
	 * Memory barrier involvement:
	 *
	 * If __printk_cpu_trylock:A reads from __printk_cpu_unlock:B, then
	 * __printk_cpu_unlock:A can never read from __printk_cpu_trylock:B.
	 *
	 * Relies on:
	 *
	 * RELEASE from __printk_cpu_unlock:A to __printk_cpu_unlock:B
	 * of the previous CPU
	 *    matching
	 * ACQUIRE from __printk_cpu_trylock:A to __printk_cpu_trylock:B
	 * of this CPU
	 */
	old = atomic_cmpxchg_acquire(&printk_cpulock_owner, -1,
				     cpu); /* LMM(__printk_cpu_trylock:A) */
	if (old == -1) {
		/*
		 * This CPU is now the owner and begins loading/storing
		 * data: LMM(__printk_cpu_trylock:B)
		 */
		return 1;

	} else if (old == cpu) {
		/* This CPU is already the owner. */
		atomic_inc(&printk_cpulock_nested);
		return 1;
	}

	return 0;
}
EXPORT_SYMBOL(__printk_cpu_trylock);

/**
 * __printk_cpu_unlock() - Release the printk cpu-reentrant spinning lock.
 *
 * The calling processor must be the owner of the lock.
 *
 * Context: Any context. Expects interrupts to be disabled.
 */
void __printk_cpu_unlock(void)
{
	if (atomic_read(&printk_cpulock_nested)) {
		atomic_dec(&printk_cpulock_nested);
		return;
	}

	/*
	 * This CPU is finished loading/storing data:
	 * LMM(__printk_cpu_unlock:A)
	 */

	/*
	 * Guarantee loads and stores from this CPU when it was the
	 * lock owner are visible to the next lock owner. This pairs
	 * with __printk_cpu_trylock:A.
	 *
	 * Memory barrier involvement:
	 *
	 * If __printk_cpu_trylock:A reads from __printk_cpu_unlock:B,
	 * then __printk_cpu_trylock:B reads from __printk_cpu_unlock:A.
	 *
	 * Relies on:
	 *
	 * RELEASE from __printk_cpu_unlock:A to __printk_cpu_unlock:B
	 * of this CPU
	 *    matching
	 * ACQUIRE from __printk_cpu_trylock:A to __printk_cpu_trylock:B
	 * of the next CPU
	 */
	atomic_set_release(&printk_cpulock_owner,
			   -1); /* LMM(__printk_cpu_unlock:B) */
}
EXPORT_SYMBOL(__printk_cpu_unlock);
#endif /* CONFIG_SMP */
>>>>>>> upstream/android-13
