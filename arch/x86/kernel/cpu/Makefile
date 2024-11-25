# SPDX-License-Identifier: GPL-2.0
#
# Makefile for x86-compatible CPU details, features and quirks
#

# Don't trace early stages of a secondary CPU boot
ifdef CONFIG_FUNCTION_TRACER
CFLAGS_REMOVE_common.o = -pg
CFLAGS_REMOVE_perf_event.o = -pg
endif

# If these files are instrumented, boot hangs during the first second.
KCOV_INSTRUMENT_common.o := n
KCOV_INSTRUMENT_perf_event.o := n

<<<<<<< HEAD
# Make sure load_percpu_segment has no stackprotector
nostackp := $(call cc-option, -fno-stack-protector)
CFLAGS_common.o		:= $(nostackp)
=======
# As above, instrumenting secondary CPU boot code causes boot hangs.
KCSAN_SANITIZE_common.o := n

# Make sure load_percpu_segment has no stackprotector
CFLAGS_common.o		:= -fno-stack-protector
>>>>>>> upstream/android-13

obj-y			:= cacheinfo.o scattered.o topology.o
obj-y			+= common.o
obj-y			+= rdrand.o
obj-y			+= match.o
obj-y			+= bugs.o
obj-y			+= aperfmperf.o
obj-y			+= cpuid-deps.o
<<<<<<< HEAD
=======
obj-y			+= umwait.o
>>>>>>> upstream/android-13

obj-$(CONFIG_PROC_FS)	+= proc.o
obj-$(CONFIG_X86_FEATURE_NAMES) += capflags.o powerflags.o

<<<<<<< HEAD
obj-$(CONFIG_CPU_SUP_INTEL)		+= intel.o intel_pconfig.o tsx.o
obj-$(CONFIG_CPU_SUP_AMD)		+= amd.o
=======
obj-$(CONFIG_IA32_FEAT_CTL) += feat_ctl.o
ifdef CONFIG_CPU_SUP_INTEL
obj-y			+= intel.o intel_pconfig.o tsx.o
obj-$(CONFIG_PM)	+= intel_epb.o
endif
obj-$(CONFIG_CPU_SUP_AMD)		+= amd.o
obj-$(CONFIG_CPU_SUP_HYGON)		+= hygon.o
>>>>>>> upstream/android-13
obj-$(CONFIG_CPU_SUP_CYRIX_32)		+= cyrix.o
obj-$(CONFIG_CPU_SUP_CENTAUR)		+= centaur.o
obj-$(CONFIG_CPU_SUP_TRANSMETA_32)	+= transmeta.o
obj-$(CONFIG_CPU_SUP_UMC_32)		+= umc.o
<<<<<<< HEAD

obj-$(CONFIG_INTEL_RDT)	+= intel_rdt.o intel_rdt_rdtgroup.o intel_rdt_monitor.o
obj-$(CONFIG_INTEL_RDT)	+= intel_rdt_ctrlmondata.o intel_rdt_pseudo_lock.o
CFLAGS_intel_rdt_pseudo_lock.o = -I$(src)

obj-$(CONFIG_X86_MCE)			+= mcheck/
obj-$(CONFIG_MTRR)			+= mtrr/
obj-$(CONFIG_MICROCODE)			+= microcode/
=======
obj-$(CONFIG_CPU_SUP_ZHAOXIN)		+= zhaoxin.o

obj-$(CONFIG_X86_MCE)			+= mce/
obj-$(CONFIG_MTRR)			+= mtrr/
obj-$(CONFIG_MICROCODE)			+= microcode/
obj-$(CONFIG_X86_CPU_RESCTRL)		+= resctrl/
obj-$(CONFIG_X86_SGX)			+= sgx/
>>>>>>> upstream/android-13

obj-$(CONFIG_X86_LOCAL_APIC)		+= perfctr-watchdog.o

obj-$(CONFIG_HYPERVISOR_GUEST)		+= vmware.o hypervisor.o mshyperv.o
<<<<<<< HEAD

ifdef CONFIG_X86_FEATURE_NAMES
quiet_cmd_mkcapflags = MKCAP   $@
      cmd_mkcapflags = $(CONFIG_SHELL) $(srctree)/$(src)/mkcapflags.sh $< $@

cpufeature = $(src)/../../include/asm/cpufeatures.h

targets += capflags.c
$(obj)/capflags.c: $(cpufeature) $(src)/mkcapflags.sh FORCE
	$(call if_changed,mkcapflags)
endif
clean-files += capflags.c
=======
obj-$(CONFIG_ACRN_GUEST)		+= acrn.o

ifdef CONFIG_X86_FEATURE_NAMES
quiet_cmd_mkcapflags = MKCAP   $@
      cmd_mkcapflags = $(CONFIG_SHELL) $(srctree)/$(src)/mkcapflags.sh $@ $^

cpufeature = $(src)/../../include/asm/cpufeatures.h
vmxfeature = $(src)/../../include/asm/vmxfeatures.h

$(obj)/capflags.c: $(cpufeature) $(vmxfeature) $(src)/mkcapflags.sh FORCE
	$(call if_changed,mkcapflags)
endif
targets += capflags.c
>>>>>>> upstream/android-13
