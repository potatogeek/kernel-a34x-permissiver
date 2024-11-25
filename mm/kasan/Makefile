# SPDX-License-Identifier: GPL-2.0
KASAN_SANITIZE := n
<<<<<<< HEAD
UBSAN_SANITIZE_common.o := n
UBSAN_SANITIZE_generic.o := n
UBSAN_SANITIZE_generic_report.o := n
UBSAN_SANITIZE_tags.o := n
KCOV_INSTRUMENT := n

CFLAGS_REMOVE_common.o = -pg
CFLAGS_REMOVE_generic.o = -pg
CFLAGS_REMOVE_generic_report.o = -pg
CFLAGS_REMOVE_tags.o = -pg

# Function splitter causes unnecessary splits in __asan_load1/__asan_store1
# see: https://gcc.gnu.org/bugzilla/show_bug.cgi?id=63533

CFLAGS_common.o := $(call cc-option, -fno-conserve-stack -fno-stack-protector)
CFLAGS_generic.o := $(call cc-option, -fno-conserve-stack -fno-stack-protector)
CFLAGS_generic_report.o := $(call cc-option, -fno-conserve-stack -fno-stack-protector)
CFLAGS_tags.o := $(call cc-option, -fno-conserve-stack -fno-stack-protector)

obj-$(CONFIG_KASAN) := common.o init.o report.o
obj-$(CONFIG_KASAN_GENERIC) += generic.o generic_report.o quarantine.o
obj-$(CONFIG_KASAN_SW_TAGS) += tags.o tags_report.o
=======
UBSAN_SANITIZE := n
KCOV_INSTRUMENT := n

# Disable ftrace to avoid recursion.
CFLAGS_REMOVE_common.o = $(CC_FLAGS_FTRACE)
CFLAGS_REMOVE_generic.o = $(CC_FLAGS_FTRACE)
CFLAGS_REMOVE_init.o = $(CC_FLAGS_FTRACE)
CFLAGS_REMOVE_quarantine.o = $(CC_FLAGS_FTRACE)
CFLAGS_REMOVE_report.o = $(CC_FLAGS_FTRACE)
CFLAGS_REMOVE_report_generic.o = $(CC_FLAGS_FTRACE)
CFLAGS_REMOVE_report_hw_tags.o = $(CC_FLAGS_FTRACE)
CFLAGS_REMOVE_report_sw_tags.o = $(CC_FLAGS_FTRACE)
CFLAGS_REMOVE_shadow.o = $(CC_FLAGS_FTRACE)
CFLAGS_REMOVE_hw_tags.o = $(CC_FLAGS_FTRACE)
CFLAGS_REMOVE_sw_tags.o = $(CC_FLAGS_FTRACE)

# Function splitter causes unnecessary splits in __asan_load1/__asan_store1
# see: https://gcc.gnu.org/bugzilla/show_bug.cgi?id=63533
CC_FLAGS_KASAN_RUNTIME := $(call cc-option, -fno-conserve-stack)
CC_FLAGS_KASAN_RUNTIME += -fno-stack-protector
# Disable branch tracing to avoid recursion.
CC_FLAGS_KASAN_RUNTIME += -DDISABLE_BRANCH_PROFILING

CFLAGS_common.o := $(CC_FLAGS_KASAN_RUNTIME)
CFLAGS_generic.o := $(CC_FLAGS_KASAN_RUNTIME)
CFLAGS_init.o := $(CC_FLAGS_KASAN_RUNTIME)
CFLAGS_quarantine.o := $(CC_FLAGS_KASAN_RUNTIME)
CFLAGS_report.o := $(CC_FLAGS_KASAN_RUNTIME)
CFLAGS_report_generic.o := $(CC_FLAGS_KASAN_RUNTIME)
CFLAGS_report_hw_tags.o := $(CC_FLAGS_KASAN_RUNTIME)
CFLAGS_report_sw_tags.o := $(CC_FLAGS_KASAN_RUNTIME)
CFLAGS_shadow.o := $(CC_FLAGS_KASAN_RUNTIME)
CFLAGS_hw_tags.o := $(CC_FLAGS_KASAN_RUNTIME)
CFLAGS_sw_tags.o := $(CC_FLAGS_KASAN_RUNTIME)

obj-$(CONFIG_KASAN) := common.o report.o
obj-$(CONFIG_KASAN_GENERIC) += init.o generic.o report_generic.o shadow.o quarantine.o
obj-$(CONFIG_KASAN_HW_TAGS) += hw_tags.o report_hw_tags.o tags.o report_tags.o
obj-$(CONFIG_KASAN_SW_TAGS) += init.o report_sw_tags.o shadow.o sw_tags.o tags.o report_tags.o
>>>>>>> upstream/android-13
