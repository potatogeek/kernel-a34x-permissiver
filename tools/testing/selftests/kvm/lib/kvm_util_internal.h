<<<<<<< HEAD
/*
 * tools/testing/selftests/kvm/lib/kvm_util.c
 *
 * Copyright (C) 2018, Google LLC.
 *
 * This work is licensed under the terms of the GNU GPL, version 2.
 */

#ifndef KVM_UTIL_INTERNAL_H
#define KVM_UTIL_INTERNAL_H 1

#include "sparsebit.h"

#ifndef BITS_PER_BYTE
#define BITS_PER_BYTE           8
#endif

#ifndef BITS_PER_LONG
#define BITS_PER_LONG (BITS_PER_BYTE * sizeof(long))
#endif

#define DIV_ROUND_UP(n, d)	(((n) + (d) - 1) / (d))
#define BITS_TO_LONGS(nr)       DIV_ROUND_UP(nr, BITS_PER_LONG)

/* Concrete definition of struct kvm_vm. */
struct userspace_mem_region {
	struct userspace_mem_region *next, *prev;
=======
/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * tools/testing/selftests/kvm/lib/kvm_util_internal.h
 *
 * Copyright (C) 2018, Google LLC.
 */

#ifndef SELFTEST_KVM_UTIL_INTERNAL_H
#define SELFTEST_KVM_UTIL_INTERNAL_H

#include "linux/hashtable.h"
#include "linux/rbtree.h"

#include "sparsebit.h"

struct userspace_mem_region {
>>>>>>> upstream/android-13
	struct kvm_userspace_memory_region region;
	struct sparsebit *unused_phy_pages;
	int fd;
	off_t offset;
	void *host_mem;
<<<<<<< HEAD
	void *mmap_start;
	size_t mmap_size;
};

struct vcpu {
	struct vcpu *next, *prev;
	uint32_t id;
	int fd;
	struct kvm_run *state;
=======
	void *host_alias;
	void *mmap_start;
	void *mmap_alias;
	size_t mmap_size;
	struct rb_node gpa_node;
	struct rb_node hva_node;
	struct hlist_node slot_node;
};

struct vcpu {
	struct list_head list;
	uint32_t id;
	int fd;
	struct kvm_run *state;
	struct kvm_dirty_gfn *dirty_gfns;
	uint32_t fetch_index;
	uint32_t dirty_gfns_count;
};

struct userspace_mem_regions {
	struct rb_root gpa_tree;
	struct rb_root hva_tree;
	DECLARE_HASHTABLE(slot_hash, 9);
>>>>>>> upstream/android-13
};

struct kvm_vm {
	int mode;
<<<<<<< HEAD
	int kvm_fd;
	int fd;
	unsigned int page_size;
	unsigned int page_shift;
	uint64_t max_gfn;
	struct vcpu *vcpu_head;
	struct userspace_mem_region *userspace_mem_region_head;
	struct sparsebit *vpages_valid;
	struct sparsebit *vpages_mapped;

=======
	unsigned long type;
	int kvm_fd;
	int fd;
	unsigned int pgtable_levels;
	unsigned int page_size;
	unsigned int page_shift;
	unsigned int pa_bits;
	unsigned int va_bits;
	uint64_t max_gfn;
	struct list_head vcpus;
	struct userspace_mem_regions regions;
	struct sparsebit *vpages_valid;
	struct sparsebit *vpages_mapped;
>>>>>>> upstream/android-13
	bool has_irqchip;
	bool pgd_created;
	vm_paddr_t pgd;
	vm_vaddr_t gdt;
	vm_vaddr_t tss;
<<<<<<< HEAD
};

struct vcpu *vcpu_find(struct kvm_vm *vm,
	uint32_t vcpuid);
void vcpu_setup(struct kvm_vm *vm, int vcpuid, int pgd_memslot, int gdt_memslot);
void virt_dump(FILE *stream, struct kvm_vm *vm, uint8_t indent);
void regs_dump(FILE *stream, struct kvm_regs *regs,
	uint8_t indent);
void sregs_dump(FILE *stream, struct kvm_sregs *sregs,
	uint8_t indent);

#endif
=======
	vm_vaddr_t idt;
	vm_vaddr_t handlers;
	uint32_t dirty_ring_size;
};

struct vcpu *vcpu_find(struct kvm_vm *vm, uint32_t vcpuid);

/*
 * Virtual Translation Tables Dump
 *
 * Input Args:
 *   stream - Output FILE stream
 *   vm     - Virtual Machine
 *   indent - Left margin indent amount
 *
 * Output Args: None
 *
 * Return: None
 *
 * Dumps to the FILE stream given by @stream, the contents of all the
 * virtual translation tables for the VM given by @vm.
 */
void virt_dump(FILE *stream, struct kvm_vm *vm, uint8_t indent);

/*
 * Register Dump
 *
 * Input Args:
 *   stream - Output FILE stream
 *   regs   - Registers
 *   indent - Left margin indent amount
 *
 * Output Args: None
 *
 * Return: None
 *
 * Dumps the state of the registers given by @regs, to the FILE stream
 * given by @stream.
 */
void regs_dump(FILE *stream, struct kvm_regs *regs, uint8_t indent);

/*
 * System Register Dump
 *
 * Input Args:
 *   stream - Output FILE stream
 *   sregs  - System registers
 *   indent - Left margin indent amount
 *
 * Output Args: None
 *
 * Return: None
 *
 * Dumps the state of the system registers given by @sregs, to the FILE stream
 * given by @stream.
 */
void sregs_dump(FILE *stream, struct kvm_sregs *sregs, uint8_t indent);

struct userspace_mem_region *
memslot2region(struct kvm_vm *vm, uint32_t memslot);

#endif /* SELFTEST_KVM_UTIL_INTERNAL_H */
>>>>>>> upstream/android-13
