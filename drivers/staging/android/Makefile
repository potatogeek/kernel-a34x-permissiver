<<<<<<< HEAD
ccflags-y += -I$(src)			# needed for trace events

##obj-y					+= ion/

obj-$(CONFIG_ASHMEM)			+= ashmem.o
obj-$(CONFIG_ANDROID_VSOC)		+= vsoc.o
obj-$(CONFIG_ANDROID_INTF_ALARM_DEV)	+= alarm-dev.o
obj-$(CONFIG_DEBUG_KINFO)		+= debug_kinfo.o
obj-$(CONFIG_SAMSUNG_LMKD_DEBUG)		+= lmkd_debug.o

obj-$(CONFIG_SAMSUNG_FREECESS)		+= freecess.o
obj-$(CONFIG_SAMSUNG_FREECESS)		+= freecess_pkg.o
## once CONFIG_MTK_ION is set as y, means this project
##     will use MTK_ION, NOT aosp ION.

## CONFIG_ION will always equals to y when use ION

ifeq ($(CONFIG_MTK_ION),y)
  $(info ION:enable mtk ion)
  obj-$(CONFIG_MTK_ION)             += mtk_ion/
else ifeq ($(CONFIG_ION),y)
  $(info ION: enable aosp ion)
  obj-$(CONFIG_ION)                 += aosp_ion/
else
  $(info ION: ION disable)
=======
# SPDX-License-Identifier: GPL-2.0
ccflags-y += -I$(src)			# needed for trace events

obj-$(CONFIG_ASHMEM)			+= ashmem.o
obj-$(CONFIG_DEBUG_KINFO)	+= debug_kinfo.o
obj-$(CONFIG_SAMSUNG_LMKD_DEBUG)		+= lmkd_debug.o

ifeq ($(CONFIG_EXYNOS_VARIANT1),y)
include $(srctree)/drivers/staging/android/Makefile.variant1
>>>>>>> upstream/android-13
endif
