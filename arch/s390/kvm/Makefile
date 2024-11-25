# SPDX-License-Identifier: GPL-2.0
# Makefile for kernel virtual machines on s390
#
# Copyright IBM Corp. 2008

KVM := ../../../virt/kvm
<<<<<<< HEAD
common-objs = $(KVM)/kvm_main.o $(KVM)/eventfd.o  $(KVM)/async_pf.o $(KVM)/irqchip.o $(KVM)/vfio.o
=======
common-objs = $(KVM)/kvm_main.o $(KVM)/eventfd.o  $(KVM)/async_pf.o \
	      $(KVM)/irqchip.o $(KVM)/vfio.o $(KVM)/binary_stats.o
>>>>>>> upstream/android-13

ccflags-y := -Ivirt/kvm -Iarch/s390/kvm

kvm-objs := $(common-objs) kvm-s390.o intercept.o interrupt.o priv.o sigp.o
<<<<<<< HEAD
kvm-objs += diag.o gaccess.o guestdbg.o vsie.o
=======
kvm-objs += diag.o gaccess.o guestdbg.o vsie.o pv.o
>>>>>>> upstream/android-13

obj-$(CONFIG_KVM) += kvm.o
