// SPDX-License-Identifier: GPL-2.0
#include <stdio.h>
#include <unistd.h>
<<<<<<< HEAD
#include <linux/bpf.h>
#include <string.h>
#include <assert.h>
#include <sys/resource.h>
#include "libbpf.h"
#include "bpf_load.h"
=======
#include <string.h>
#include <assert.h>
#include <sys/resource.h>
#include <bpf/libbpf.h>
#include <bpf/bpf.h>
>>>>>>> upstream/android-13
#include "trace_helpers.h"

int main(int ac, char **argv)
{
<<<<<<< HEAD
	struct rlimit r = {RLIM_INFINITY, RLIM_INFINITY};
	long key, next_key, value;
	char filename[256];
	struct ksym *sym;
	int i;

	snprintf(filename, sizeof(filename), "%s_kern.o", argv[0]);
	setrlimit(RLIMIT_MEMLOCK, &r);
=======
	char filename[256], symbol[256];
	struct bpf_object *obj = NULL;
	struct bpf_link *links[20];
	long key, next_key, value;
	struct bpf_program *prog;
	int map_fd, i, j = 0;
	const char *section;
	struct ksym *sym;
>>>>>>> upstream/android-13

	if (load_kallsyms()) {
		printf("failed to process /proc/kallsyms\n");
		return 2;
	}

<<<<<<< HEAD
	if (load_bpf_file(filename)) {
		printf("%s", bpf_log_buf);
		return 1;
=======
	snprintf(filename, sizeof(filename), "%s_kern.o", argv[0]);
	obj = bpf_object__open_file(filename, NULL);
	if (libbpf_get_error(obj)) {
		fprintf(stderr, "ERROR: opening BPF object file failed\n");
		obj = NULL;
		goto cleanup;
	}

	/* load BPF program */
	if (bpf_object__load(obj)) {
		fprintf(stderr, "ERROR: loading BPF object file failed\n");
		goto cleanup;
	}

	map_fd = bpf_object__find_map_fd_by_name(obj, "my_map");
	if (map_fd < 0) {
		fprintf(stderr, "ERROR: finding a map in obj file failed\n");
		goto cleanup;
	}

	bpf_object__for_each_program(prog, obj) {
		section = bpf_program__section_name(prog);
		if (sscanf(section, "kprobe/%s", symbol) != 1)
			continue;

		/* Attach prog only when symbol exists */
		if (ksym_get_addr(symbol)) {
			links[j] = bpf_program__attach(prog);
			if (libbpf_get_error(links[j])) {
				fprintf(stderr, "bpf_program__attach failed\n");
				links[j] = NULL;
				goto cleanup;
			}
			j++;
		}
>>>>>>> upstream/android-13
	}

	for (i = 0; i < 5; i++) {
		key = 0;
		printf("kprobing funcs:");
<<<<<<< HEAD
		while (bpf_map_get_next_key(map_fd[0], &key, &next_key) == 0) {
			bpf_map_lookup_elem(map_fd[0], &next_key, &value);
			assert(next_key == value);
			sym = ksym_search(value);
			printf(" %s", sym->name);
			key = next_key;
=======
		while (bpf_map_get_next_key(map_fd, &key, &next_key) == 0) {
			bpf_map_lookup_elem(map_fd, &next_key, &value);
			assert(next_key == value);
			sym = ksym_search(value);
			key = next_key;
			if (!sym) {
				printf("ksym not found. Is kallsyms loaded?\n");
				continue;
			}

			printf(" %s", sym->name);
>>>>>>> upstream/android-13
		}
		if (key)
			printf("\n");
		key = 0;
<<<<<<< HEAD
		while (bpf_map_get_next_key(map_fd[0], &key, &next_key) == 0)
			bpf_map_delete_elem(map_fd[0], &next_key);
		sleep(1);
	}

=======
		while (bpf_map_get_next_key(map_fd, &key, &next_key) == 0)
			bpf_map_delete_elem(map_fd, &next_key);
		sleep(1);
	}

cleanup:
	for (j--; j >= 0; j--)
		bpf_link__destroy(links[j]);

	bpf_object__close(obj);
>>>>>>> upstream/android-13
	return 0;
}
