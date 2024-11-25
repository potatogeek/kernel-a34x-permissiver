# SPDX-License-Identifier: GPL-2.0
###
# scripts contains sources for various helper programs used throughout
# the kernel for the build process.
<<<<<<< HEAD
# ---------------------------------------------------------------------------
# kallsyms:      Find all symbols in vmlinux
# pnmttologo:    Convert pnm files to logo files
# conmakehash:   Create chartable
# conmakehash:	 Create arrays for initializing the kernel console tables

HOST_EXTRACFLAGS += -I$(srctree)/tools/include
=======
>>>>>>> upstream/android-13

CRYPTO_LIBS = $(shell pkg-config --libs libcrypto 2> /dev/null || echo -lcrypto)
CRYPTO_CFLAGS = $(shell pkg-config --cflags libcrypto 2> /dev/null)

<<<<<<< HEAD
hostprogs-$(CONFIG_BUILD_BIN2C)  += bin2c
hostprogs-$(CONFIG_KALLSYMS)     += kallsyms
hostprogs-$(CONFIG_LOGO)         += pnmtologo
hostprogs-$(CONFIG_VT)           += conmakehash
hostprogs-$(BUILD_C_RECORDMCOUNT) += recordmcount
hostprogs-$(CONFIG_BUILDTIME_EXTABLE_SORT) += sortextable
hostprogs-$(CONFIG_ASN1)	 += asn1_compiler
hostprogs-$(CONFIG_MODULE_SIG)	 += sign-file
hostprogs-$(CONFIG_SYSTEM_TRUSTED_KEYRING) += extract-cert
hostprogs-$(CONFIG_SYSTEM_EXTRA_CERTIFICATE) += insert-sys-cert

HOSTCFLAGS_sortextable.o = -I$(srctree)/tools/include
=======
hostprogs-always-$(CONFIG_BUILD_BIN2C)			+= bin2c
hostprogs-always-$(CONFIG_KALLSYMS)			+= kallsyms
hostprogs-always-$(BUILD_C_RECORDMCOUNT)		+= recordmcount
hostprogs-always-$(CONFIG_BUILDTIME_TABLE_SORT)		+= sorttable
hostprogs-always-$(CONFIG_ASN1)				+= asn1_compiler
hostprogs-always-$(CONFIG_MODULE_SIG_FORMAT)		+= sign-file
hostprogs-always-$(CONFIG_SYSTEM_TRUSTED_KEYRING)	+= extract-cert
hostprogs-always-$(CONFIG_SYSTEM_EXTRA_CERTIFICATE)	+= insert-sys-cert
hostprogs-always-$(CONFIG_SYSTEM_REVOCATION_LIST)	+= extract-cert

HOSTCFLAGS_sorttable.o = -I$(srctree)/tools/include
>>>>>>> upstream/android-13
HOSTCFLAGS_asn1_compiler.o = -I$(srctree)/include
HOSTCFLAGS_sign-file.o = $(CRYPTO_CFLAGS)
HOSTLDLIBS_sign-file = $(CRYPTO_LIBS)
HOSTCFLAGS_extract-cert.o = $(CRYPTO_CFLAGS)
HOSTLDLIBS_extract-cert = $(CRYPTO_LIBS)

<<<<<<< HEAD
always		:= $(hostprogs-y) $(hostprogs-m)

# The following hostprogs-y programs are only build on demand
hostprogs-y += unifdef

extra-$(CONFIG_LTO_CLANG)   += module-lto.lds

# These targets are used internally to avoid "is up to date" messages
PHONY += build_unifdef
build_unifdef: $(obj)/unifdef
	@:

subdir-$(CONFIG_MODVERSIONS) += genksyms
subdir-y                     += mod
subdir-$(CONFIG_SECURITY_SELINUX) += selinux
subdir-$(CONFIG_DTC)         += dtc
subdir-$(CONFIG_GDB_SCRIPTS) += gdb

# Let clean descend into subdirs
subdir-	+= basic kconfig package gcc-plugins
=======
ifdef CONFIG_UNWINDER_ORC
ifeq ($(ARCH),x86_64)
ARCH := x86
endif
HOSTCFLAGS_sorttable.o += -I$(srctree)/tools/arch/x86/include
HOSTCFLAGS_sorttable.o += -DUNWINDER_ORC_ENABLED
HOSTLDLIBS_sorttable = -lpthread
endif

# The following programs are only built on demand
hostprogs += unifdef

# The module linker script is preprocessed on demand
targets += module.lds

subdir-$(CONFIG_GCC_PLUGINS) += gcc-plugins
subdir-$(CONFIG_MODVERSIONS) += genksyms
subdir-$(CONFIG_SECURITY_SELINUX) += selinux

# Let clean descend into subdirs
subdir-	+= basic dtc gdb kconfig mod
>>>>>>> upstream/android-13
