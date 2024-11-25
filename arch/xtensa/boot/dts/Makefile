#
# arch/xtensa/boot/dts/Makefile
#
# This file is subject to the terms and conditions of the GNU General Public
# License.  See the file "COPYING" in the main directory of this archive
# for more details.
#
#

<<<<<<< HEAD
BUILTIN_DTB := $(patsubst "%",%,$(CONFIG_BUILTIN_DTB)).dtb.o
ifneq ($(CONFIG_BUILTIN_DTB),"")
obj-$(CONFIG_OF) += $(BUILTIN_DTB)
=======
BUILTIN_DTB_SOURCE := $(patsubst "%",%,$(CONFIG_BUILTIN_DTB_SOURCE)).dtb.o
ifneq ($(CONFIG_BUILTIN_DTB_SOURCE),"")
obj-$(CONFIG_OF) += $(BUILTIN_DTB_SOURCE)
>>>>>>> upstream/android-13
endif

# for CONFIG_OF_ALL_DTBS test
dtstree	:= $(srctree)/$(src)
dtb-	:= $(patsubst $(dtstree)/%.dts,%.dtb, $(wildcard $(dtstree)/*.dts))
