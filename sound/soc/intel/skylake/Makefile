<<<<<<< HEAD
# SPDX-License-Identifier: GPL-2.0
snd-soc-skl-objs := skl.o skl-pcm.o skl-nhlt.o skl-messages.o \
skl-topology.o
=======
# SPDX-License-Identifier: GPL-2.0-only
snd-soc-skl-objs := skl.o skl-pcm.o skl-nhlt.o skl-messages.o skl-topology.o \
		skl-sst-ipc.o skl-sst-dsp.o cnl-sst-dsp.o skl-sst-cldma.o \
		skl-sst.o bxt-sst.o cnl-sst.o skl-sst-utils.o
>>>>>>> upstream/android-13

ifdef CONFIG_DEBUG_FS
  snd-soc-skl-objs += skl-debug.o
endif

<<<<<<< HEAD
obj-$(CONFIG_SND_SOC_INTEL_SKYLAKE) += snd-soc-skl.o

# Skylake IPC Support
snd-soc-skl-ipc-objs := skl-sst-ipc.o skl-sst-dsp.o cnl-sst-dsp.o \
		skl-sst-cldma.o skl-sst.o bxt-sst.o cnl-sst.o \
		skl-sst-utils.o

obj-$(CONFIG_SND_SOC_INTEL_SKYLAKE) += snd-soc-skl-ipc.o
=======
obj-$(CONFIG_SND_SOC_INTEL_SKYLAKE_COMMON) += snd-soc-skl.o
>>>>>>> upstream/android-13

#Skylake Clock device support
snd-soc-skl-ssp-clk-objs := skl-ssp-clk.o

obj-$(CONFIG_SND_SOC_INTEL_SKYLAKE_SSP_CLK) += snd-soc-skl-ssp-clk.o
