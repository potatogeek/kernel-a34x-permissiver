// SPDX-License-Identifier: GPL-2.0
<<<<<<< HEAD
#include "../perf.h"
#include "util.h"
#include "debug.h"
#include <api/fs/fs.h>
#include <sys/mman.h>
=======
#include "util.h"
#include "debug.h"
#include "event.h"
#include <api/fs/fs.h>
>>>>>>> upstream/android-13
#include <sys/stat.h>
#include <sys/utsname.h>
#include <dirent.h>
#include <fcntl.h>
#include <inttypes.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <limits.h>
<<<<<<< HEAD
=======
#include <linux/capability.h>
>>>>>>> upstream/android-13
#include <linux/kernel.h>
#include <linux/log2.h>
#include <linux/time64.h>
#include <unistd.h>
<<<<<<< HEAD
#include "strlist.h"
=======
#include "cap.h"
#include "strlist.h"
#include "string2.h"
>>>>>>> upstream/android-13

/*
 * XXX We need to find a better place for these things...
 */

bool perf_singlethreaded = true;

void perf_set_singlethreaded(void)
{
	perf_singlethreaded = true;
}

void perf_set_multithreaded(void)
{
	perf_singlethreaded = false;
}

<<<<<<< HEAD
unsigned int page_size;

#ifdef _SC_LEVEL1_DCACHE_LINESIZE
#define cache_line_size(cacheline_sizep) *cacheline_sizep = sysconf(_SC_LEVEL1_DCACHE_LINESIZE)
#else
static void cache_line_size(int *cacheline_sizep)
{
	if (sysfs__read_int("devices/system/cpu/cpu0/cache/index0/coherency_line_size", cacheline_sizep))
		pr_debug("cannot determine cache line size");
}
#endif

int cacheline_size(void)
{
	static int size;

	if (!size)
		cache_line_size(&size);

	return size;
}

=======
>>>>>>> upstream/android-13
int sysctl_perf_event_max_stack = PERF_MAX_STACK_DEPTH;
int sysctl_perf_event_max_contexts_per_stack = PERF_MAX_CONTEXTS_PER_STACK;

int sysctl__max_stack(void)
{
	int value;

	if (sysctl__read_int("kernel/perf_event_max_stack", &value) == 0)
		sysctl_perf_event_max_stack = value;

	if (sysctl__read_int("kernel/perf_event_max_contexts_per_stack", &value) == 0)
		sysctl_perf_event_max_contexts_per_stack = value;

	return sysctl_perf_event_max_stack;
}

<<<<<<< HEAD
=======
bool sysctl__nmi_watchdog_enabled(void)
{
	static bool cached;
	static bool nmi_watchdog;
	int value;

	if (cached)
		return nmi_watchdog;

	if (sysctl__read_int("kernel/nmi_watchdog", &value) < 0)
		return false;

	nmi_watchdog = (value > 0) ? true : false;
	cached = true;

	return nmi_watchdog;
}

>>>>>>> upstream/android-13
bool test_attr__enabled;

bool perf_host  = true;
bool perf_guest = false;

void event_attr_init(struct perf_event_attr *attr)
{
	if (!perf_host)
		attr->exclude_host  = 1;
	if (!perf_guest)
		attr->exclude_guest = 1;
	/* to capture ABI version */
	attr->size = sizeof(*attr);
}

int mkdir_p(char *path, mode_t mode)
{
	struct stat st;
	int err;
	char *d = path;

	if (*d != '/')
		return -1;

	if (stat(path, &st) == 0)
		return 0;

	while (*++d == '/');

	while ((d = strchr(d, '/'))) {
		*d = '\0';
		err = stat(path, &st) && mkdir(path, mode);
		*d++ = '/';
		if (err)
			return -1;
		while (*d == '/')
			++d;
	}
	return (stat(path, &st) && mkdir(path, mode)) ? -1 : 0;
}

<<<<<<< HEAD
int rm_rf(const char *path)
{
	DIR *dir;
	int ret = 0;
	struct dirent *d;
	char namebuf[PATH_MAX];

	dir = opendir(path);
	if (dir == NULL)
		return 0;

	while ((d = readdir(dir)) != NULL && !ret) {
		struct stat statbuf;
=======
static bool match_pat(char *file, const char **pat)
{
	int i = 0;

	if (!pat)
		return true;

	while (pat[i]) {
		if (strglobmatch(file, pat[i]))
			return true;

		i++;
	}

	return false;
}

/*
 * The depth specify how deep the removal will go.
 * 0       - will remove only files under the 'path' directory
 * 1 .. x  - will dive in x-level deep under the 'path' directory
 *
 * If specified the pat is array of string patterns ended with NULL,
 * which are checked upon every file/directory found. Only matching
 * ones are removed.
 *
 * The function returns:
 *    0 on success
 *   -1 on removal failure with errno set
 *   -2 on pattern failure
 */
static int rm_rf_depth_pat(const char *path, int depth, const char **pat)
{
	DIR *dir;
	int ret;
	struct dirent *d;
	char namebuf[PATH_MAX];
	struct stat statbuf;

	/* Do not fail if there's no file. */
	ret = lstat(path, &statbuf);
	if (ret)
		return 0;

	/* Try to remove any file we get. */
	if (!(statbuf.st_mode & S_IFDIR))
		return unlink(path);

	/* We have directory in path. */
	dir = opendir(path);
	if (dir == NULL)
		return -1;

	while ((d = readdir(dir)) != NULL && !ret) {
>>>>>>> upstream/android-13

		if (!strcmp(d->d_name, ".") || !strcmp(d->d_name, ".."))
			continue;

<<<<<<< HEAD
=======
		if (!match_pat(d->d_name, pat)) {
			ret =  -2;
			break;
		}

>>>>>>> upstream/android-13
		scnprintf(namebuf, sizeof(namebuf), "%s/%s",
			  path, d->d_name);

		/* We have to check symbolic link itself */
		ret = lstat(namebuf, &statbuf);
		if (ret < 0) {
			pr_debug("stat failed: %s\n", namebuf);
			break;
		}

		if (S_ISDIR(statbuf.st_mode))
<<<<<<< HEAD
			ret = rm_rf(namebuf);
=======
			ret = depth ? rm_rf_depth_pat(namebuf, depth - 1, pat) : 0;
>>>>>>> upstream/android-13
		else
			ret = unlink(namebuf);
	}
	closedir(dir);

	if (ret < 0)
		return ret;

	return rmdir(path);
}

<<<<<<< HEAD
=======
static int rm_rf_kcore_dir(const char *path)
{
	char kcore_dir_path[PATH_MAX];
	const char *pat[] = {
		"kcore",
		"kallsyms",
		"modules",
		NULL,
	};

	snprintf(kcore_dir_path, sizeof(kcore_dir_path), "%s/kcore_dir", path);

	return rm_rf_depth_pat(kcore_dir_path, 0, pat);
}

int rm_rf_perf_data(const char *path)
{
	const char *pat[] = {
		"data",
		"data.*",
		NULL,
	};

	rm_rf_kcore_dir(path);

	return rm_rf_depth_pat(path, 0, pat);
}

int rm_rf(const char *path)
{
	return rm_rf_depth_pat(path, INT_MAX, NULL);
}

>>>>>>> upstream/android-13
/* A filter which removes dot files */
bool lsdir_no_dot_filter(const char *name __maybe_unused, struct dirent *d)
{
	return d->d_name[0] != '.';
}

/* lsdir reads a directory and store it in strlist */
struct strlist *lsdir(const char *name,
		      bool (*filter)(const char *, struct dirent *))
{
	struct strlist *list = NULL;
	DIR *dir;
	struct dirent *d;

	dir = opendir(name);
	if (!dir)
		return NULL;

	list = strlist__new(NULL, NULL);
	if (!list) {
		errno = ENOMEM;
		goto out;
	}

	while ((d = readdir(dir)) != NULL) {
		if (!filter || filter(name, d))
			strlist__add(list, d->d_name);
	}

out:
	closedir(dir);
	return list;
}

<<<<<<< HEAD
static int slow_copyfile(const char *from, const char *to, struct nsinfo *nsi)
{
	int err = -1;
	char *line = NULL;
	size_t n;
	FILE *from_fp, *to_fp;
	struct nscookie nsc;

	nsinfo__mountns_enter(nsi, &nsc);
	from_fp = fopen(from, "r");
	nsinfo__mountns_exit(&nsc);
	if (from_fp == NULL)
		goto out;

	to_fp = fopen(to, "w");
	if (to_fp == NULL)
		goto out_fclose_from;

	while (getline(&line, &n, from_fp) > 0)
		if (fputs(line, to_fp) == EOF)
			goto out_fclose_to;
	err = 0;
out_fclose_to:
	fclose(to_fp);
	free(line);
out_fclose_from:
	fclose(from_fp);
out:
	return err;
}

static int copyfile_offset(int ifd, loff_t off_in, int ofd, loff_t off_out, u64 size)
{
	void *ptr;
	loff_t pgoff;

	pgoff = off_in & ~(page_size - 1);
	off_in -= pgoff;

	ptr = mmap(NULL, off_in + size, PROT_READ, MAP_PRIVATE, ifd, pgoff);
	if (ptr == MAP_FAILED)
		return -1;

	while (size) {
		ssize_t ret = pwrite(ofd, ptr + off_in, size, off_out);
		if (ret < 0 && errno == EINTR)
			continue;
		if (ret <= 0)
			break;

		size -= ret;
		off_in += ret;
		off_out += ret;
	}
	munmap(ptr, off_in + size);

	return size ? -1 : 0;
}

static int copyfile_mode_ns(const char *from, const char *to, mode_t mode,
			    struct nsinfo *nsi)
{
	int fromfd, tofd;
	struct stat st;
	int err;
	char *tmp = NULL, *ptr = NULL;
	struct nscookie nsc;

	nsinfo__mountns_enter(nsi, &nsc);
	err = stat(from, &st);
	nsinfo__mountns_exit(&nsc);
	if (err)
		goto out;
	err = -1;

	/* extra 'x' at the end is to reserve space for '.' */
	if (asprintf(&tmp, "%s.XXXXXXx", to) < 0) {
		tmp = NULL;
		goto out;
	}
	ptr = strrchr(tmp, '/');
	if (!ptr)
		goto out;
	ptr = memmove(ptr + 1, ptr, strlen(ptr) - 1);
	*ptr = '.';

	tofd = mkstemp(tmp);
	if (tofd < 0)
		goto out;

	if (fchmod(tofd, mode))
		goto out_close_to;

	if (st.st_size == 0) { /* /proc? do it slowly... */
		err = slow_copyfile(from, tmp, nsi);
		goto out_close_to;
	}

	nsinfo__mountns_enter(nsi, &nsc);
	fromfd = open(from, O_RDONLY);
	nsinfo__mountns_exit(&nsc);
	if (fromfd < 0)
		goto out_close_to;

	err = copyfile_offset(fromfd, 0, tofd, 0, st.st_size);

	close(fromfd);
out_close_to:
	close(tofd);
	if (!err)
		err = link(tmp, to);
	unlink(tmp);
out:
	free(tmp);
	return err;
}

int copyfile_ns(const char *from, const char *to, struct nsinfo *nsi)
{
	return copyfile_mode_ns(from, to, 0755, nsi);
}

int copyfile_mode(const char *from, const char *to, mode_t mode)
{
	return copyfile_mode_ns(from, to, mode, NULL);
}

int copyfile(const char *from, const char *to)
{
	return copyfile_mode(from, to, 0755);
}

static ssize_t ion(bool is_read, int fd, void *buf, size_t n)
{
	void *buf_start = buf;
	size_t left = n;

	while (left) {
		/* buf must be treated as const if !is_read. */
		ssize_t ret = is_read ? read(fd, buf, left) :
					write(fd, buf, left);

		if (ret < 0 && errno == EINTR)
			continue;
		if (ret <= 0)
			return ret;

		left -= ret;
		buf  += ret;
	}

	BUG_ON((size_t)(buf - buf_start) != n);
	return n;
}

/*
 * Read exactly 'n' bytes or return an error.
 */
ssize_t readn(int fd, void *buf, size_t n)
{
	return ion(true, fd, buf, n);
}

/*
 * Write exactly 'n' bytes or return an error.
 */
ssize_t writen(int fd, const void *buf, size_t n)
{
	/* ion does not modify buf. */
	return ion(false, fd, (void *)buf, n);
}

=======
>>>>>>> upstream/android-13
size_t hex_width(u64 v)
{
	size_t n = 1;

	while ((v >>= 4))
		++n;

	return n;
}

<<<<<<< HEAD
/*
 * While we find nice hex chars, build a long_val.
 * Return number of chars processed.
 */
int hex2u64(const char *ptr, u64 *long_val)
{
	char *p;

	*long_val = strtoull(ptr, &p, 16);

	return p - ptr;
}

=======
>>>>>>> upstream/android-13
int perf_event_paranoid(void)
{
	int value;

	if (sysctl__read_int("kernel/perf_event_paranoid", &value))
		return INT_MAX;

	return value;
}
<<<<<<< HEAD
=======

bool perf_event_paranoid_check(int max_level)
{
	return perf_cap__capable(CAP_SYS_ADMIN) ||
			perf_cap__capable(CAP_PERFMON) ||
			perf_event_paranoid() <= max_level;
}

>>>>>>> upstream/android-13
static int
fetch_ubuntu_kernel_version(unsigned int *puint)
{
	ssize_t len;
	size_t line_len = 0;
	char *ptr, *line = NULL;
	int version, patchlevel, sublevel, err;
	FILE *vsig;

	if (!puint)
		return 0;

	vsig = fopen("/proc/version_signature", "r");
	if (!vsig) {
		pr_debug("Open /proc/version_signature failed: %s\n",
			 strerror(errno));
		return -1;
	}

	len = getline(&line, &line_len, vsig);
	fclose(vsig);
	err = -1;
	if (len <= 0) {
		pr_debug("Reading from /proc/version_signature failed: %s\n",
			 strerror(errno));
		goto errout;
	}

	ptr = strrchr(line, ' ');
	if (!ptr) {
		pr_debug("Parsing /proc/version_signature failed: %s\n", line);
		goto errout;
	}

	err = sscanf(ptr + 1, "%d.%d.%d",
		     &version, &patchlevel, &sublevel);
	if (err != 3) {
		pr_debug("Unable to get kernel version from /proc/version_signature '%s'\n",
			 line);
		goto errout;
	}

	*puint = (version << 16) + (patchlevel << 8) + sublevel;
	err = 0;
errout:
	free(line);
	return err;
}

int
fetch_kernel_version(unsigned int *puint, char *str,
		     size_t str_size)
{
	struct utsname utsname;
	int version, patchlevel, sublevel, err;
	bool int_ver_ready = false;

	if (access("/proc/version_signature", R_OK) == 0)
		if (!fetch_ubuntu_kernel_version(puint))
			int_ver_ready = true;

	if (uname(&utsname))
		return -1;

	if (str && str_size) {
		strncpy(str, utsname.release, str_size);
		str[str_size - 1] = '\0';
	}

	if (!puint || int_ver_ready)
		return 0;

	err = sscanf(utsname.release, "%d.%d.%d",
		     &version, &patchlevel, &sublevel);

	if (err != 3) {
		pr_debug("Unable to get kernel version from uname '%s'\n",
			 utsname.release);
		return -1;
	}

	*puint = (version << 16) + (patchlevel << 8) + sublevel;
	return 0;
}

<<<<<<< HEAD
const char *perf_tip(const char *dirpath)
{
	struct strlist *tips;
	struct str_node *node;
	char *tip = NULL;
=======
int perf_tip(char **strp, const char *dirpath)
{
	struct strlist *tips;
	struct str_node *node;
>>>>>>> upstream/android-13
	struct strlist_config conf = {
		.dirname = dirpath,
		.file_only = true,
	};
<<<<<<< HEAD

	tips = strlist__new("tips.txt", &conf);
	if (tips == NULL)
		return errno == ENOENT ? NULL :
			"Tip: check path of tips.txt or get more memory! ;-p";
=======
	int ret = 0;

	*strp = NULL;
	tips = strlist__new("tips.txt", &conf);
	if (tips == NULL)
		return -errno;
>>>>>>> upstream/android-13

	if (strlist__nr_entries(tips) == 0)
		goto out;

	node = strlist__entry(tips, random() % strlist__nr_entries(tips));
<<<<<<< HEAD
	if (asprintf(&tip, "Tip: %s", node->s) < 0)
		tip = (char *)"Tip: get more memory! ;-)";
=======
	if (asprintf(strp, "Tip: %s", node->s) < 0)
		ret = -ENOMEM;
>>>>>>> upstream/android-13

out:
	strlist__delete(tips);

<<<<<<< HEAD
	return tip;
=======
	return ret;
}

char *perf_exe(char *buf, int len)
{
	int n = readlink("/proc/self/exe", buf, len);
	if (n > 0) {
		buf[n] = 0;
		return buf;
	}
	return strcpy(buf, "perf");
>>>>>>> upstream/android-13
}
