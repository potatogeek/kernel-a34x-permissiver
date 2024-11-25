# SPDX-License-Identifier: GPL-2.0+
#
# Makefile for the HISILICON network device drivers.
#

ccflags-y := -I $(srctree)/drivers/net/ethernet/hisilicon/hns3
<<<<<<< HEAD

obj-$(CONFIG_HNS3_HCLGEVF) += hclgevf.o
hclgevf-objs = hclgevf_main.o hclgevf_cmd.o hclgevf_mbx.o
=======
ccflags-y += -I $(srctree)/$(src)

obj-$(CONFIG_HNS3_HCLGEVF) += hclgevf.o
hclgevf-objs = hclgevf_main.o hclgevf_cmd.o hclgevf_mbx.o  hclgevf_devlink.o
>>>>>>> upstream/android-13
