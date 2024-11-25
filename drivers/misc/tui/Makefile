#
# Makefile for the stui Module
#

ccflags-y += -I$(srctree)/drivers/misc/tui/
ccflags-y += -I$(srctree)/drivers/misc/tzdev/include/tzdev
ccflags-y += -DUSE_TEE_CLIENT_API

<<<<<<< HEAD
obj-$(CONFIG_SAMSUNG_TUI)	+= main.o
obj-$(CONFIG_SAMSUNG_TUI)	+= stui_core.o
obj-$(CONFIG_SAMSUNG_TUI)	+= stui_inf.o


ifeq ($(CONFIG_MACH_MT6877), y)
obj-$(CONFIG_SAMSUNG_TUI)	+= platforms/mt6877/
else
obj-$(CONFIG_SAMSUNG_TUI)	+= platforms/$(CONFIG_MTK_PLATFORM)/
endif

ifdef CONFIG_SAMSUNG_TUI_LOWLEVEL
obj-$(CONFIG_SAMSUNG_TUI) += iwd_agent.o
=======
tuihw-inf-objs := stui_inf.o

tuihw-objs := main.o
tuihw-objs += stui_core.o

ifeq ($(CONFIG_SOC_S5E9925),y)
ccflags-y += -DCONFIG_SOC_S5E9925
ccflags-y += -I$(srctree)/drivers/misc/tui/platforms/exynos9925
ccflags-y += -I$(srctree)/drivers/gpu/drm/samsung/dpu
tuihw-objs += platforms/exynos9925/stui_hal_display.o
tuihw-objs += platforms/exynos9925/stui_hal_touch.o
endif

ifeq ($(CONFIG_SOC_S5E8825),y)
ccflags-y += -DCONFIG_SOC_S5E8825
ccflags-y += -I$(srctree)/drivers/misc/tui/platforms/exynos8825
ccflags-y += -I$(srctree)/drivers/gpu/drm/samsung/dpu
tuihw-objs += platforms/exynos8825/stui_hal_display.o
tuihw-objs += platforms/exynos8825/stui_hal_touch.o
endif

ifeq ($(CONFIG_SOC_S5E8535),y)
ccflags-y += -DCONFIG_SOC_S5E8535
ccflags-y += -I$(srctree)/drivers/misc/tui/platforms/exynos8535
ccflags-y += -I$(srctree)/drivers/gpu/drm/samsung/dpu
tuihw-objs += platforms/exynos8535/stui_hal_display.o
tuihw-objs += platforms/exynos8535/stui_hal_touch.o
endif

ifeq ($(CONFIG_SOC_EXYNOS2100),y)
ccflags-y += -Idrivers/video/fbdev/exynos/dpu30
ccflags-y += -I$(srctree)/drivers/misc/tui/platforms/exynos2100
tuihw-objs += platforms/exynos2100/stui_hal_display.o
tuihw-objs += platforms/exynos2100/stui_hal_touch.o
endif

ifeq ($(CONFIG_SOC_EXYNOS9830),y)
ccflags-y += -Idrivers/video/fbdev/exynos/dpu30
tuihw-objs += platforms/exynos9830/stui_hal_display.o
tuihw-objs += platforms/exynos9830/stui_hal_touch.o
endif

ifeq ($(CONFIG_SOC_EXYNOS9820),y)
ccflags-y += -Idrivers/video/fbdev/exynos/dpu20
tuihw-objs += platforms/exynos9820/stui_hal_display.o
tuihw-objs += platforms/exynos9820/stui_hal_touch.o
endif

ifeq ($(CONFIG_SOC_EXYNOS9630),y)
ccflags-y += -Idrivers/video/fbdev/exynos/dpu30
tuihw-objs += platforms/exynos980/stui_hal_display.o
tuihw-objs += platforms/exynos980/stui_hal_touch.o
endif

ifdef CONFIG_SAMSUNG_TUI_LOWLEVEL
tuihw-objs += iwd_agent.o
>>>>>>> upstream/android-13

ccflags-y += -I$(srctree)/drivers/misc
ccflags-y += -I$(srctree)/drivers/misc/tzdev/
ccflags-y += -DCONFIG_SAMSUNG_TUI_LOWLEVEL
endif
<<<<<<< HEAD
=======

obj-$(CONFIG_SAMSUNG_TUI) += tuihw.o tuihw-inf.o
>>>>>>> upstream/android-13
