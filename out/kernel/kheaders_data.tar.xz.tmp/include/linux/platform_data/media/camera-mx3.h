

#ifndef _MX3_CAMERA_H_
#define _MX3_CAMERA_H_

#include <linux/device.h>

#define MX3_CAMERA_CLK_SRC	1
#define MX3_CAMERA_EXT_VSYNC	2
#define MX3_CAMERA_DP		4
#define MX3_CAMERA_PCP		8
#define MX3_CAMERA_HSP		0x10
#define MX3_CAMERA_VSP		0x20
#define MX3_CAMERA_DATAWIDTH_4	0x40
#define MX3_CAMERA_DATAWIDTH_8	0x80
#define MX3_CAMERA_DATAWIDTH_10	0x100
#define MX3_CAMERA_DATAWIDTH_15	0x200

#define MX3_CAMERA_DATAWIDTH_MASK (MX3_CAMERA_DATAWIDTH_4 | MX3_CAMERA_DATAWIDTH_8 | \
				   MX3_CAMERA_DATAWIDTH_10 | MX3_CAMERA_DATAWIDTH_15)

struct v4l2_async_subdev;


struct mx3_camera_pdata {
	unsigned long flags;
	unsigned long mclk_10khz;
	struct device *dma_dev;
	struct v4l2_async_subdev **asd;	
	int *asd_sizes;			
};

#endif
