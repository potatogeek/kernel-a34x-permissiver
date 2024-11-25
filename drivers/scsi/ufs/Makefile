# SPDX-License-Identifier: GPL-2.0
# UFSHCD makefile
<<<<<<< HEAD

MTK_MISC_INC := drivers/misc/mediatek/include

ifeq ($(CONFIG_SCSI_UFS_MEDIATEK),y)

MTK_MISC_INC := drivers/misc/mediatek/include
ifeq (y, $(filter y, $(CONFIG_MACH_MT6893)))
MTK_PLATFORM := mt6893
else ifeq (y, $(filter y, $(CONFIG_MACH_MT6877)))
MTK_PLATFORM := mt6877
else ifeq (y, $(filter y, $(CONFIG_MACH_MT6781)))
MTK_PLATFORM := mt6781
else
MTK_PLATFORM := $(subst ",,$(CONFIG_MTK_PLATFORM))
endif


ccflags-y += -I$(srctree)/$(MTK_MISC_INC)
ccflags-y += -I$(srctree)/$(MTK_MISC_INC)/mt-plat
ccflags-y += -I$(srctree)/$(MTK_MISC_INC)/mt-plat/$(MTK_PLATFORM)/include
ccflags-y += -I$(srctree)/drivers/misc/mediatek/base/power/include
endif

obj-$(CONFIG_SCSI_UFS_DWC_TC_PCI) += tc-dwc-g210-pci.o ufshcd-dwc.o tc-dwc-g210.o
obj-$(CONFIG_SCSI_UFS_DWC_TC_PLATFORM) += tc-dwc-g210-pltfrm.o ufshcd-dwc.o tc-dwc-g210.o
obj-$(CONFIG_SCSI_UFS_QCOM) += ufs-qcom.o
obj-$(CONFIG_SCSI_UFSHCD) += ufshcd-core.o
ufshcd-core-objs := ufshcd.o ufs-sysfs.o
ufshcd-core-$(CONFIG_SCSI_UFS_BSG)	+= ufs_bsg.o
obj-$(CONFIG_SCSI_UFSHCD) += ufs-sec-feature.o
obj-$(CONFIG_SCSI_UFSHCD) += ufs-sec-sysfs.o
obj-$(CONFIG_SCSI_UFS_FEATURE) += ufsfeature.o
obj-$(CONFIG_SCSI_UFS_HPB) += ufshpb.o
obj-$(CONFIG_SCSI_UFS_TW) += ufstw.o
obj-$(CONFIG_SCSI_SKHPB) += ufshpb_skh.o
obj-$(CONFIG_SCSI_UFSHCD_PCI) += ufshcd-pci.o
obj-$(CONFIG_SCSI_UFSHCD_PLATFORM) += ufshcd-pltfrm.o
obj-$(CONFIG_SCSI_UFS_HISI) += ufs-hisi.o
obj-$(CONFIG_SCSI_UFS_MEDIATEK) += ufs-mediatek.o ufs-mediatek-dbg.o
ufshcd-core-$(CONFIG_SCSI_UFS_CRYPTO) += ufshcd-crypto.o
obj-$(CONFIG_MTK_UFS_BLOCK_IO_LOG) += ufs-mtk-block.o
=======
ifdef CONFIG_SCSI_UFS_EXYNOS
ccflags-y += -I$(srctree)/drivers/scsi/ufs/$(TARGET_SOC)
endif

# The link order is important here. ufshcd-core must initialize
# before vendor drivers.
obj-$(CONFIG_SCSI_UFSHCD)		+= ufshcd-core.o
ufshcd-core-y				+= ufshcd.o ufs-sysfs.o
ufshcd-core-$(CONFIG_DEBUG_FS)		+= ufs-debugfs.o
ufshcd-core-$(CONFIG_SCSI_UFS_BSG)	+= ufs_bsg.o
ufshcd-core-$(CONFIG_SCSI_UFS_CRYPTO)	+= ufshcd-crypto.o
ufshcd-core-$(CONFIG_SCSI_UFS_HPB)	+= ufshpb.o
ufshcd-core-$(CONFIG_SCSI_UFS_FAULT_INJECTION) += ufs-fault-injection.o
ufshcd-core-$(CONFIG_SCSI_UFS_HWMON) += ufs-hwmon.o

obj-$(CONFIG_SCSI_UFS_DWC_TC_PCI) += tc-dwc-g210-pci.o ufshcd-dwc.o tc-dwc-g210.o
obj-$(CONFIG_SCSI_UFS_DWC_TC_PLATFORM) += tc-dwc-g210-pltfrm.o ufshcd-dwc.o tc-dwc-g210.o
obj-$(CONFIG_SCSI_UFS_CDNS_PLATFORM) += cdns-pltfrm.o
obj-$(CONFIG_SCSI_UFS_QCOM) += ufs_qcom.o
ufs_qcom-y += ufs-qcom.o
ufs_qcom-$(CONFIG_SCSI_UFS_CRYPTO) += ufs-qcom-ice.o
obj-$(CONFIG_SCSI_UFSHCD_PCI) += ufshcd-pci.o
obj-$(CONFIG_SCSI_UFSHCD_PLATFORM) += ufshcd-pltfrm.o
obj-$(CONFIG_SCSI_UFS_HISI) += ufs-hisi.o
obj-$(CONFIG_SCSI_UFS_MEDIATEK) += ufs-mediatek.o
obj-$(CONFIG_SCSI_UFS_TI_J721E) += ti-j721e-ufs.o

obj-$(CONFIG_SCSI_UFS_EXYNOS) += ufs-exynos-core.o
ufs-exynos-core-$(CONFIG_SCSI_UFS_EXYNOS) += ufs-exynos.o ufs-exynos-dbg.o ufs-exynos-perf.o ufs-exynos-perf-v1.o ufs-exynos-gear.o\
					     $(TARGET_SOC)/ufs-cal-if.o
ufs-exynos-core-$(CONFIG_SCSI_UFS_EXYNOS_SRPMB) += ufs-exynos-srpmb.o
ufs-exynos-core-$(CONFIG_SCSI_UFS_EXYNOS_FMP) += ufs-exynos-fmp.o
ufs-exynos-core-$(CONFIG_SEC_UFS_FEATURE) += ufs-sec-feature.o ufs-sec-sysfs.o
>>>>>>> upstream/android-13
