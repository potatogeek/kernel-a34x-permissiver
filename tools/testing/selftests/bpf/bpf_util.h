/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __BPF_UTIL__
#define __BPF_UTIL__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
<<<<<<< HEAD

static inline unsigned int bpf_num_possible_cpus(void)
{
	static const char *fcpu = "/sys/devices/system/cpu/possible";
	unsigned int start, end, possible_cpus = 0;
	char buff[128];
	FILE *fp;
	int len, n, i, j = 0;

	fp = fopen(fcpu, "r");
	if (!fp) {
		printf("Failed to open %s: '%s'!\n", fcpu, strerror(errno));
		exit(1);
	}

	if (!fgets(buff, sizeof(buff), fp)) {
		printf("Failed to read %s!\n", fcpu);
		exit(1);
	}

	len = strlen(buff);
	for (i = 0; i <= len; i++) {
		if (buff[i] == ',' || buff[i] == '\0') {
			buff[i] = '\0';
			n = sscanf(&buff[j], "%u-%u", &start, &end);
			if (n <= 0) {
				printf("Failed to retrieve # possible CPUs!\n");
				exit(1);
			} else if (n == 1) {
				end = start;
			}
			possible_cpus += end - start + 1;
			j = i + 1;
		}
	}

	fclose(fp);

=======
#include <bpf/libbpf.h> /* libbpf_num_possible_cpus */

static inline unsigned int bpf_num_possible_cpus(void)
{
	int possible_cpus = libbpf_num_possible_cpus();

	if (possible_cpus < 0) {
		printf("Failed to get # of possible cpus: '%s'!\n",
		       strerror(-possible_cpus));
		exit(1);
	}
>>>>>>> upstream/android-13
	return possible_cpus;
}

#define __bpf_percpu_val_align	__attribute__((__aligned__(8)))

#define BPF_DECLARE_PERCPU(type, name)				\
	struct { type v; /* padding */ } __bpf_percpu_val_align	\
		name[bpf_num_possible_cpus()]
#define bpf_percpu(name, cpu) name[(cpu)].v

#ifndef ARRAY_SIZE
# define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))
#endif

<<<<<<< HEAD
=======
#ifndef sizeof_field
#define sizeof_field(TYPE, MEMBER) sizeof((((TYPE *)0)->MEMBER))
#endif

#ifndef offsetofend
#define offsetofend(TYPE, MEMBER) \
	(offsetof(TYPE, MEMBER)	+ sizeof_field(TYPE, MEMBER))
#endif

>>>>>>> upstream/android-13
#endif /* __BPF_UTIL__ */
