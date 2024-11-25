# SPDX-License-Identifier: GPL-2.0
<<<<<<< HEAD
hostprogs-y	:= mdp
HOST_EXTRACFLAGS += \
	-I$(srctree)/include/uapi -I$(srctree)/include \
	-I$(srctree)/security/selinux/include

always		:= $(hostprogs-y)
=======
hostprogs-always-y += mdp
HOST_EXTRACFLAGS += \
	-I$(srctree)/include/uapi -I$(srctree)/include \
	-I$(srctree)/security/selinux/include -I$(objtree)/include

>>>>>>> upstream/android-13
clean-files	:= policy.* file_contexts
