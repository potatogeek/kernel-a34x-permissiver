// SPDX-License-Identifier: GPL-2.0
#include <stdio.h>
#include <assert.h>
<<<<<<< HEAD
#include <linux/bpf.h>
#include <bpf/bpf.h>
#include "bpf_load.h"
=======
#include <bpf/bpf.h>
#include <bpf/libbpf.h>
>>>>>>> upstream/android-13
#include "sock_example.h"
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/resource.h>

<<<<<<< HEAD
#define PARSE_IP 3
#define PARSE_IP_PROG_FD (prog_fd[0])
#define PROG_ARRAY_FD (map_fd[0])

=======
>>>>>>> upstream/android-13
struct flow_key_record {
	__be32 src;
	__be32 dst;
	union {
		__be32 ports;
		__be16 port16[2];
	};
	__u32 ip_proto;
};

struct pair {
	__u64 packets;
	__u64 bytes;
};

int main(int argc, char **argv)
{
<<<<<<< HEAD
	struct rlimit r = {RLIM_INFINITY, RLIM_INFINITY};
	char filename[256];
	FILE *f;
	int i, sock, err, id, key = PARSE_IP;
	struct bpf_prog_info info = {};
	uint32_t info_len = sizeof(info);

	snprintf(filename, sizeof(filename), "%s_kern.o", argv[0]);
	setrlimit(RLIMIT_MEMLOCK, &r);

	if (load_bpf_file(filename)) {
		printf("%s", bpf_log_buf);
		return 1;
	}

	/* Test fd array lookup which returns the id of the bpf_prog */
	err = bpf_obj_get_info_by_fd(PARSE_IP_PROG_FD, &info, &info_len);
	assert(!err);
	err = bpf_map_lookup_elem(PROG_ARRAY_FD, &key, &id);
	assert(!err);
	assert(id == info.id);

	sock = open_raw_sock("lo");

	assert(setsockopt(sock, SOL_SOCKET, SO_ATTACH_BPF, &prog_fd[4],
			  sizeof(__u32)) == 0);

	if (argc > 1)
		f = popen("ping -c5 localhost", "r");
=======
	int i, sock, key, fd, main_prog_fd, jmp_table_fd, hash_map_fd;
	struct bpf_program *prog;
	struct bpf_object *obj;
	const char *section;
	char filename[256];
	FILE *f;

	snprintf(filename, sizeof(filename), "%s_kern.o", argv[0]);

	obj = bpf_object__open_file(filename, NULL);
	if (libbpf_get_error(obj)) {
		fprintf(stderr, "ERROR: opening BPF object file failed\n");
		return 0;
	}

	/* load BPF program */
	if (bpf_object__load(obj)) {
		fprintf(stderr, "ERROR: loading BPF object file failed\n");
		goto cleanup;
	}

	jmp_table_fd = bpf_object__find_map_fd_by_name(obj, "jmp_table");
	hash_map_fd = bpf_object__find_map_fd_by_name(obj, "hash_map");
	if (jmp_table_fd < 0 || hash_map_fd < 0) {
		fprintf(stderr, "ERROR: finding a map in obj file failed\n");
		goto cleanup;
	}

	bpf_object__for_each_program(prog, obj) {
		fd = bpf_program__fd(prog);

		section = bpf_program__section_name(prog);
		if (sscanf(section, "socket/%d", &key) != 1) {
			fprintf(stderr, "ERROR: finding prog failed\n");
			goto cleanup;
		}

		if (key == 0)
			main_prog_fd = fd;
		else
			bpf_map_update_elem(jmp_table_fd, &key, &fd, BPF_ANY);
	}

	sock = open_raw_sock("lo");

	/* attach BPF program to socket */
	assert(setsockopt(sock, SOL_SOCKET, SO_ATTACH_BPF, &main_prog_fd,
			  sizeof(__u32)) == 0);

	if (argc > 1)
		f = popen("ping -4 -c5 localhost", "r");
>>>>>>> upstream/android-13
	else
		f = popen("netperf -l 4 localhost", "r");
	(void) f;

	for (i = 0; i < 5; i++) {
		struct flow_key_record key = {}, next_key;
		struct pair value;

		sleep(1);
		printf("IP     src.port -> dst.port               bytes      packets\n");
<<<<<<< HEAD
		while (bpf_map_get_next_key(map_fd[2], &key, &next_key) == 0) {
			bpf_map_lookup_elem(map_fd[2], &next_key, &value);
=======
		while (bpf_map_get_next_key(hash_map_fd, &key, &next_key) == 0) {
			bpf_map_lookup_elem(hash_map_fd, &next_key, &value);
>>>>>>> upstream/android-13
			printf("%s.%05d -> %s.%05d %12lld %12lld\n",
			       inet_ntoa((struct in_addr){htonl(next_key.src)}),
			       next_key.port16[0],
			       inet_ntoa((struct in_addr){htonl(next_key.dst)}),
			       next_key.port16[1],
			       value.bytes, value.packets);
			key = next_key;
		}
	}
<<<<<<< HEAD
=======

cleanup:
	bpf_object__close(obj);
>>>>>>> upstream/android-13
	return 0;
}
