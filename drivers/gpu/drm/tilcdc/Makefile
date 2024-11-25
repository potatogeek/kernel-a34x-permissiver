# SPDX-License-Identifier: GPL-2.0
<<<<<<< HEAD
ifeq (, $(findstring -W,$(EXTRA_CFLAGS)))
=======
ifeq (, $(findstring -W,$(KCFLAGS)))
>>>>>>> upstream/android-13
	ccflags-y += -Werror
endif

tilcdc-y := \
	tilcdc_plane.o \
	tilcdc_crtc.o \
<<<<<<< HEAD
	tilcdc_tfp410.o \
=======
>>>>>>> upstream/android-13
	tilcdc_panel.o \
	tilcdc_external.o \
	tilcdc_drv.o

obj-$(CONFIG_DRM_TILCDC)	+= tilcdc.o
