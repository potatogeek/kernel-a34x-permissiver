<<<<<<< HEAD
lib-y	+= delay.o
lib-y	+= memcpy.o
lib-y	+= memset.o
lib-y	+= uaccess.o
lib-y	+= tishift.o

lib-$(CONFIG_32BIT) += udivdi3.o
=======
# SPDX-License-Identifier: GPL-2.0-only
lib-y			+= delay.o
lib-y			+= memcpy.o
lib-y			+= memset.o
lib-y			+= memmove.o
lib-$(CONFIG_MMU)	+= uaccess.o
lib-$(CONFIG_64BIT)	+= tishift.o

obj-$(CONFIG_FUNCTION_ERROR_INJECTION) += error-inject.o
>>>>>>> upstream/android-13
