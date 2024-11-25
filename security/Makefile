# SPDX-License-Identifier: GPL-2.0
#
# Makefile for the kernel security code
#

obj-$(CONFIG_KEYS)			+= keys/
<<<<<<< HEAD
subdir-$(CONFIG_SECURITY_SELINUX)	+= selinux
subdir-$(CONFIG_SECURITY_SMACK)		+= smack
subdir-$(CONFIG_SECURITY_TOMOYO)        += tomoyo
subdir-$(CONFIG_SECURITY_APPARMOR)	+= apparmor
subdir-$(CONFIG_SECURITY_YAMA)		+= yama
subdir-$(CONFIG_SECURITY_LOADPIN)	+= loadpin
=======
>>>>>>> upstream/android-13

# always enable default capabilities
obj-y					+= commoncap.o
obj-$(CONFIG_MMU)			+= min_addr.o

# Object file lists
obj-$(CONFIG_SECURITY)			+= security.o
obj-$(CONFIG_SECURITYFS)		+= inode.o
obj-$(CONFIG_SECURITY_SELINUX)		+= selinux/
obj-$(CONFIG_SECURITY_SMACK)		+= smack/
<<<<<<< HEAD
obj-$(CONFIG_AUDIT)			+= lsm_audit.o
=======
obj-$(CONFIG_SECURITY)			+= lsm_audit.o
>>>>>>> upstream/android-13
obj-$(CONFIG_SECURITY_TOMOYO)		+= tomoyo/
obj-$(CONFIG_SECURITY_APPARMOR)		+= apparmor/
obj-$(CONFIG_SECURITY_YAMA)		+= yama/
obj-$(CONFIG_SECURITY_LOADPIN)		+= loadpin/
<<<<<<< HEAD
obj-$(CONFIG_CGROUP_DEVICE)		+= device_cgroup.o

# Object integrity file lists
subdir-$(CONFIG_INTEGRITY)		+= integrity
obj-$(CONFIG_INTEGRITY)			+= integrity/
obj-y += samsung/defex_lsm/ # ADDED BY LEGO AUTOMATICALLY: DO NOT SUBMIT
obj-y += samsung/proca/ # ADDED BY LEGO AUTOMATICALLY: DO NOT SUBMIT
obj-y += samsung/proca/gaf/ # ADDED BY LEGO AUTOMATICALLY: DO NOT SUBMIT
obj-y += samsung/dsms/ # ADDED BY LEGO AUTOMATICALLY: DO NOT SUBMIT
obj-y += samsung/five/ # ADDED BY LEGO AUTOMATICALLY: DO NOT SUBMIT
=======
obj-$(CONFIG_SECURITY_SAFESETID)       += safesetid/
obj-$(CONFIG_SECURITY_LOCKDOWN_LSM)	+= lockdown/
obj-$(CONFIG_CGROUPS)			+= device_cgroup.o
obj-$(CONFIG_BPF_LSM)			+= bpf/
obj-$(CONFIG_SECURITY_LANDLOCK)		+= landlock/

# Object integrity file lists
obj-$(CONFIG_INTEGRITY)			+= integrity/
obj-y += samsung/proca/ # ADDED BY LEGO AUTOMATICALLY: DO NOT SUBMIT
obj-y += samsung/proca/gaf/ # ADDED BY LEGO AUTOMATICALLY: DO NOT SUBMIT
obj-y += samsung/mz/ # ADDED BY LEGO AUTOMATICALLY: DO NOT SUBMIT
obj-y += samsung/five/ # ADDED BY LEGO AUTOMATICALLY: DO NOT SUBMIT
obj-y += samsung/mz_tee_driver/ # ADDED BY LEGO AUTOMATICALLY: DO NOT SUBMIT
>>>>>>> upstream/android-13
