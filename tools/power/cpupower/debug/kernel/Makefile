# SPDX-License-Identifier: GPL-2.0
obj-m	:=

KDIR	:= /lib/modules/$(shell uname -r)/build
KMISC   := /lib/modules/$(shell uname -r)/cpufrequtils/

ifeq ("$(CONFIG_X86_TSC)", "y")
  obj-m	 += cpufreq-test_tsc.o
endif

default:
	$(MAKE) -C $(KDIR) M=$(CURDIR)

clean:
<<<<<<< HEAD
	- rm -rf *.o *.ko .tmp-versions .*.cmd .*.mod.* *.mod.c
	- rm -rf .tmp_versions* Module.symvers modules.order
=======
	- rm -rf *.o *.ko .*.cmd .*.mod.* *.mod.c
	- rm -rf Module.symvers modules.order
>>>>>>> upstream/android-13

install: default
	install -d $(KMISC)
	install -m 644 -c *.ko $(KMISC)
	/sbin/depmod -a

all:	default
