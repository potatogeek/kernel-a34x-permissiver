/* SPDX-License-Identifier: GPL-2.0 */
#ifndef GIT_COMPAT_UTIL_H
#define GIT_COMPAT_UTIL_H

#define _BSD_SOURCE 1
/* glibc 2.20 deprecates _BSD_SOURCE in favour of _DEFAULT_SOURCE */
#define _DEFAULT_SOURCE 1

<<<<<<< HEAD
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdarg.h>
=======
#include <fcntl.h>
#include <stdbool.h>
#include <stddef.h>
>>>>>>> upstream/android-13
#include <linux/compiler.h>
#include <sys/types.h>

/* General helper functions */
void usage(const char *err) __noreturn;
void die(const char *err, ...) __noreturn __printf(1, 2);

<<<<<<< HEAD
static inline void *zalloc(size_t size)
{
	return calloc(1, size);
}

#define zfree(ptr) ({ free((void *)*ptr); *ptr = NULL; })

struct dirent;
struct nsinfo;
=======
struct dirent;
>>>>>>> upstream/android-13
struct strlist;

int mkdir_p(char *path, mode_t mode);
int rm_rf(const char *path);
<<<<<<< HEAD
struct strlist *lsdir(const char *name, bool (*filter)(const char *, struct dirent *));
bool lsdir_no_dot_filter(const char *name, struct dirent *d);
int copyfile(const char *from, const char *to);
int copyfile_mode(const char *from, const char *to, mode_t mode);
int copyfile_ns(const char *from, const char *to, struct nsinfo *nsi);

ssize_t readn(int fd, void *buf, size_t n);
ssize_t writen(int fd, const void *buf, size_t n);

size_t hex_width(u64 v);
int hex2u64(const char *ptr, u64 *val);

extern unsigned int page_size;
int __pure cacheline_size(void);

int sysctl__max_stack(void);

=======
int rm_rf_perf_data(const char *path);
struct strlist *lsdir(const char *name, bool (*filter)(const char *, struct dirent *));
bool lsdir_no_dot_filter(const char *name, struct dirent *d);

size_t hex_width(u64 v);

int sysctl__max_stack(void);

bool sysctl__nmi_watchdog_enabled(void);

>>>>>>> upstream/android-13
int fetch_kernel_version(unsigned int *puint,
			 char *str, size_t str_sz);
#define KVER_VERSION(x)		(((x) >> 16) & 0xff)
#define KVER_PATCHLEVEL(x)	(((x) >> 8) & 0xff)
#define KVER_SUBLEVEL(x)	((x) & 0xff)
#define KVER_FMT	"%d.%d.%d"
#define KVER_PARAM(x)	KVER_VERSION(x), KVER_PATCHLEVEL(x), KVER_SUBLEVEL(x)

<<<<<<< HEAD
const char *perf_tip(const char *dirpath);

#ifndef HAVE_GET_CURRENT_DIR_NAME
char *get_current_dir_name(void);
#endif
=======
int perf_tip(char **strp, const char *dirpath);
>>>>>>> upstream/android-13

#ifndef HAVE_SCHED_GETCPU_SUPPORT
int sched_getcpu(void);
#endif

<<<<<<< HEAD
#ifndef HAVE_SETNS_SUPPORT
int setns(int fd, int nstype);
#endif

=======
>>>>>>> upstream/android-13
extern bool perf_singlethreaded;

void perf_set_singlethreaded(void);
void perf_set_multithreaded(void);

<<<<<<< HEAD
=======
char *perf_exe(char *buf, int len);

>>>>>>> upstream/android-13
#ifndef O_CLOEXEC
#ifdef __sparc__
#define O_CLOEXEC      0x400000
#elif defined(__alpha__) || defined(__hppa__)
#define O_CLOEXEC      010000000
#else
#define O_CLOEXEC      02000000
#endif
#endif

<<<<<<< HEAD
=======
extern bool test_attr__enabled;
void test_attr__ready(void);
void test_attr__init(void);
struct perf_event_attr;
void test_attr__open(struct perf_event_attr *attr, pid_t pid, int cpu,
		     int fd, int group_fd, unsigned long flags);
>>>>>>> upstream/android-13
#endif /* GIT_COMPAT_UTIL_H */
