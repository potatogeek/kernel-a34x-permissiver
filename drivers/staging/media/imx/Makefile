# SPDX-License-Identifier: GPL-2.0
<<<<<<< HEAD
imx-media-objs := imx-media-dev.o imx-media-internal-sd.o imx-media-of.o
imx-media-common-objs := imx-media-utils.o imx-media-fim.o
imx-media-ic-objs := imx-ic-common.o imx-ic-prp.o imx-ic-prpencvf.o

obj-$(CONFIG_VIDEO_IMX_MEDIA) += imx-media.o
obj-$(CONFIG_VIDEO_IMX_MEDIA) += imx-media-common.o
obj-$(CONFIG_VIDEO_IMX_MEDIA) += imx-media-capture.o
obj-$(CONFIG_VIDEO_IMX_MEDIA) += imx-media-vdic.o
obj-$(CONFIG_VIDEO_IMX_MEDIA) += imx-media-ic.o

obj-$(CONFIG_VIDEO_IMX_CSI) += imx-media-csi.o
obj-$(CONFIG_VIDEO_IMX_CSI) += imx6-mipi-csi2.o
=======
imx-media-common-objs := imx-media-capture.o imx-media-dev-common.o \
	imx-media-of.o imx-media-utils.o

imx6-media-objs := imx-media-dev.o imx-media-internal-sd.o \
	imx-ic-common.o imx-ic-prp.o imx-ic-prpencvf.o imx-media-vdic.o \
	imx-media-csc-scaler.o

imx6-media-csi-objs := imx-media-csi.o imx-media-fim.o

obj-$(CONFIG_VIDEO_IMX_MEDIA) += imx-media-common.o

obj-$(CONFIG_VIDEO_IMX_CSI) += imx6-media.o
obj-$(CONFIG_VIDEO_IMX_CSI) += imx6-media-csi.o
obj-$(CONFIG_VIDEO_IMX_CSI) += imx6-mipi-csi2.o

obj-$(CONFIG_VIDEO_IMX7_CSI) += imx7-media-csi.o
obj-$(CONFIG_VIDEO_IMX7_CSI) += imx7-mipi-csis.o
obj-$(CONFIG_VIDEO_IMX7_CSI) += imx8mq-mipi-csi2.o
>>>>>>> upstream/android-13
