/*
 * Copyright 2012 Red Hat Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sub license, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. IN NO EVENT SHALL
 * THE COPYRIGHT HOLDERS, AUTHORS AND/OR ITS SUPPLIERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
 * OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE
 * USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * The above copyright notice and this permission notice (including the
 * next paragraph) shall be included in all copies or substantial portions
 * of the Software.
 *
 */
/*
 * Authors: Dave Airlie <airlied@redhat.com>
 */
#ifndef __AST_DRV_H__
#define __AST_DRV_H__

<<<<<<< HEAD
#include <drm/drm_encoder.h>
#include <drm/drm_fb_helper.h>

#include <drm/ttm/ttm_bo_api.h>
#include <drm/ttm/ttm_bo_driver.h>
#include <drm/ttm/ttm_placement.h>
#include <drm/ttm/ttm_memory.h>
#include <drm/ttm/ttm_module.h>

#include <drm/drm_gem.h>

#include <linux/i2c.h>
#include <linux/i2c-algo-bit.h>
=======
#include <linux/i2c.h>
#include <linux/i2c-algo-bit.h>
#include <linux/io.h>
#include <linux/types.h>

#include <drm/drm_connector.h>
#include <drm/drm_crtc.h>
#include <drm/drm_encoder.h>
#include <drm/drm_mode.h>
#include <drm/drm_framebuffer.h>
#include <drm/drm_fb_helper.h>
>>>>>>> upstream/android-13

#define DRIVER_AUTHOR		"Dave Airlie"

#define DRIVER_NAME		"ast"
#define DRIVER_DESC		"AST"
#define DRIVER_DATE		"20120228"

#define DRIVER_MAJOR		0
#define DRIVER_MINOR		1
#define DRIVER_PATCHLEVEL	0

#define PCI_CHIP_AST2000 0x2000
#define PCI_CHIP_AST2100 0x2010
<<<<<<< HEAD
#define PCI_CHIP_AST1180 0x1180
=======
>>>>>>> upstream/android-13


enum ast_chip {
	AST2000,
	AST2100,
	AST1100,
	AST2200,
	AST2150,
	AST2300,
	AST2400,
	AST2500,
<<<<<<< HEAD
	AST1180,
=======
	AST2600,
>>>>>>> upstream/android-13
};

enum ast_tx_chip {
	AST_TX_NONE,
	AST_TX_SIL164,
	AST_TX_ITE66121,
	AST_TX_DP501,
};

#define AST_DRAM_512Mx16 0
#define AST_DRAM_1Gx16   1
#define AST_DRAM_512Mx32 2
#define AST_DRAM_1Gx32   3
#define AST_DRAM_2Gx16   6
#define AST_DRAM_4Gx16   7
#define AST_DRAM_8Gx16   8

<<<<<<< HEAD
struct ast_fbdev;

struct ast_private {
	struct drm_device *dev;

	void __iomem *regs;
	void __iomem *ioregs;
=======
/*
 * Cursor plane
 */

#define AST_MAX_HWC_WIDTH	64
#define AST_MAX_HWC_HEIGHT	64

#define AST_HWC_SIZE		(AST_MAX_HWC_WIDTH * AST_MAX_HWC_HEIGHT * 2)
#define AST_HWC_SIGNATURE_SIZE	32

#define AST_DEFAULT_HWC_NUM	2

/* define for signature structure */
#define AST_HWC_SIGNATURE_CHECKSUM	0x00
#define AST_HWC_SIGNATURE_SizeX		0x04
#define AST_HWC_SIGNATURE_SizeY		0x08
#define AST_HWC_SIGNATURE_X		0x0C
#define AST_HWC_SIGNATURE_Y		0x10
#define AST_HWC_SIGNATURE_HOTSPOTX	0x14
#define AST_HWC_SIGNATURE_HOTSPOTY	0x18

struct ast_cursor_plane {
	struct drm_plane base;

	struct {
		struct drm_gem_vram_object *gbo;
		struct dma_buf_map map;
		u64 off;
	} hwc[AST_DEFAULT_HWC_NUM];

	unsigned int next_hwc_index;
};

static inline struct ast_cursor_plane *
to_ast_cursor_plane(struct drm_plane *plane)
{
	return container_of(plane, struct ast_cursor_plane, base);
}

/*
 * Connector with i2c channel
 */

struct ast_i2c_chan {
	struct i2c_adapter adapter;
	struct drm_device *dev;
	struct i2c_algo_bit_data bit;
};

struct ast_connector {
	struct drm_connector base;
	struct ast_i2c_chan *i2c;
};

static inline struct ast_connector *
to_ast_connector(struct drm_connector *connector)
{
	return container_of(connector, struct ast_connector, base);
}

/*
 * Device
 */

struct ast_private {
	struct drm_device base;

	void __iomem *regs;
	void __iomem *ioregs;
	void __iomem *dp501_fw_buf;
>>>>>>> upstream/android-13

	enum ast_chip chip;
	bool vga2_clone;
	uint32_t dram_bus_width;
	uint32_t dram_type;
	uint32_t mclk;
<<<<<<< HEAD
	uint32_t vram_size;

	struct ast_fbdev *fbdev;

	int fb_mtrr;

	struct {
		struct drm_global_reference mem_global_ref;
		struct ttm_bo_global_ref bo_global_ref;
		struct ttm_bo_device bdev;
	} ttm;

	struct drm_gem_object *cursor_cache;
	uint64_t cursor_cache_gpu_addr;
	/* Acces to this cache is protected by the crtc->mutex of the only crtc
	 * we have. */
	struct ttm_bo_kmap_obj cache_kmap;
	int next_cursor;
=======

	int fb_mtrr;

	struct drm_plane primary_plane;
	struct ast_cursor_plane cursor_plane;
	struct drm_crtc crtc;
	struct drm_encoder encoder;
	struct ast_connector connector;

>>>>>>> upstream/android-13
	bool support_wide_screen;
	enum {
		ast_use_p2a,
		ast_use_dt,
		ast_use_defaults
	} config_mode;

	enum ast_tx_chip tx_chip_type;
	u8 dp501_maxclk;
	u8 *dp501_fw_addr;
	const struct firmware *dp501_fw;	/* dp501 fw */
};

<<<<<<< HEAD
int ast_driver_load(struct drm_device *dev, unsigned long flags);
void ast_driver_unload(struct drm_device *dev);

struct ast_gem_object;
=======
static inline struct ast_private *to_ast_private(struct drm_device *dev)
{
	return container_of(dev, struct ast_private, base);
}

struct ast_private *ast_device_create(const struct drm_driver *drv,
				      struct pci_dev *pdev,
				      unsigned long flags);
>>>>>>> upstream/android-13

#define AST_IO_AR_PORT_WRITE		(0x40)
#define AST_IO_MISC_PORT_WRITE		(0x42)
#define AST_IO_VGA_ENABLE_PORT		(0x43)
#define AST_IO_SEQ_PORT			(0x44)
#define AST_IO_DAC_INDEX_READ		(0x47)
#define AST_IO_DAC_INDEX_WRITE		(0x48)
#define AST_IO_DAC_DATA		        (0x49)
#define AST_IO_GR_PORT			(0x4E)
#define AST_IO_CRTC_PORT		(0x54)
#define AST_IO_INPUT_STATUS1_READ	(0x5A)
#define AST_IO_MISC_PORT_READ		(0x4C)

#define AST_IO_MM_OFFSET		(0x380)

<<<<<<< HEAD
=======
#define AST_IO_VGAIR1_VREFRESH		BIT(3)

#define AST_IO_VGACRCB_HWC_ENABLED     BIT(1)
#define AST_IO_VGACRCB_HWC_16BPP       BIT(0) /* set: ARGB4444, cleared: 2bpp palette */

>>>>>>> upstream/android-13
#define __ast_read(x) \
static inline u##x ast_read##x(struct ast_private *ast, u32 reg) { \
u##x val = 0;\
val = ioread##x(ast->regs + reg); \
return val;\
}

__ast_read(8);
__ast_read(16);
__ast_read(32)

#define __ast_io_read(x) \
static inline u##x ast_io_read##x(struct ast_private *ast, u32 reg) { \
u##x val = 0;\
val = ioread##x(ast->ioregs + reg); \
return val;\
}

__ast_io_read(8);
__ast_io_read(16);
__ast_io_read(32);

#define __ast_write(x) \
static inline void ast_write##x(struct ast_private *ast, u32 reg, u##x val) {\
	iowrite##x(val, ast->regs + reg);\
	}

__ast_write(8);
__ast_write(16);
__ast_write(32);

#define __ast_io_write(x) \
static inline void ast_io_write##x(struct ast_private *ast, u32 reg, u##x val) {\
	iowrite##x(val, ast->ioregs + reg);\
	}

__ast_io_write(8);
__ast_io_write(16);
#undef __ast_io_write

static inline void ast_set_index_reg(struct ast_private *ast,
				     uint32_t base, uint8_t index,
				     uint8_t val)
{
	ast_io_write16(ast, base, ((u16)val << 8) | index);
}

void ast_set_index_reg_mask(struct ast_private *ast,
			    uint32_t base, uint8_t index,
			    uint8_t mask, uint8_t val);
uint8_t ast_get_index_reg(struct ast_private *ast,
			  uint32_t base, uint8_t index);
uint8_t ast_get_index_reg_mask(struct ast_private *ast,
			       uint32_t base, uint8_t index, uint8_t mask);

static inline void ast_open_key(struct ast_private *ast)
{
	ast_set_index_reg(ast, AST_IO_CRTC_PORT, 0x80, 0xA8);
}

#define AST_VIDMEM_SIZE_8M    0x00800000
#define AST_VIDMEM_SIZE_16M   0x01000000
#define AST_VIDMEM_SIZE_32M   0x02000000
#define AST_VIDMEM_SIZE_64M   0x04000000
#define AST_VIDMEM_SIZE_128M  0x08000000

#define AST_VIDMEM_DEFAULT_SIZE AST_VIDMEM_SIZE_8M

<<<<<<< HEAD
#define AST_MAX_HWC_WIDTH 64
#define AST_MAX_HWC_HEIGHT 64

#define AST_HWC_SIZE                (AST_MAX_HWC_WIDTH*AST_MAX_HWC_HEIGHT*2)
#define AST_HWC_SIGNATURE_SIZE      32

#define AST_DEFAULT_HWC_NUM 2
/* define for signature structure */
#define AST_HWC_SIGNATURE_CHECKSUM  0x00
#define AST_HWC_SIGNATURE_SizeX     0x04
#define AST_HWC_SIGNATURE_SizeY     0x08
#define AST_HWC_SIGNATURE_X         0x0C
#define AST_HWC_SIGNATURE_Y         0x10
#define AST_HWC_SIGNATURE_HOTSPOTX  0x14
#define AST_HWC_SIGNATURE_HOTSPOTY  0x18


struct ast_i2c_chan {
	struct i2c_adapter adapter;
	struct drm_device *dev;
	struct i2c_algo_bit_data bit;
};

struct ast_connector {
	struct drm_connector base;
	struct ast_i2c_chan *i2c;
};

struct ast_crtc {
	struct drm_crtc base;
	struct drm_gem_object *cursor_bo;
	uint64_t cursor_addr;
	int cursor_width, cursor_height;
	u8 offset_x, offset_y;
};

struct ast_encoder {
	struct drm_encoder base;
};

struct ast_framebuffer {
	struct drm_framebuffer base;
	struct drm_gem_object *obj;
};

struct ast_fbdev {
	struct drm_fb_helper helper;
	struct ast_framebuffer afb;
	void *sysram;
	int size;
	struct ttm_bo_kmap_obj mapping;
	int x1, y1, x2, y2; /* dirty rect */
	spinlock_t dirty_lock;
};

#define to_ast_crtc(x) container_of(x, struct ast_crtc, base)
#define to_ast_connector(x) container_of(x, struct ast_connector, base)
#define to_ast_encoder(x) container_of(x, struct ast_encoder, base)
#define to_ast_framebuffer(x) container_of(x, struct ast_framebuffer, base)

=======
>>>>>>> upstream/android-13
struct ast_vbios_stdtable {
	u8 misc;
	u8 seq[4];
	u8 crtc[25];
	u8 ar[20];
	u8 gr[9];
};

struct ast_vbios_enhtable {
	u32 ht;
	u32 hde;
	u32 hfp;
	u32 hsync;
	u32 vt;
	u32 vde;
	u32 vfp;
	u32 vsync;
	u32 dclk_index;
	u32 flags;
	u32 refresh_rate;
	u32 refresh_rate_index;
	u32 mode_id;
};

struct ast_vbios_dclk_info {
	u8 param1;
	u8 param2;
	u8 param3;
};

struct ast_vbios_mode_info {
	const struct ast_vbios_stdtable *std_table;
	const struct ast_vbios_enhtable *enh_table;
};

<<<<<<< HEAD
extern int ast_mode_init(struct drm_device *dev);
extern void ast_mode_fini(struct drm_device *dev);

int ast_framebuffer_init(struct drm_device *dev,
			 struct ast_framebuffer *ast_fb,
			 const struct drm_mode_fb_cmd2 *mode_cmd,
			 struct drm_gem_object *obj);

int ast_fbdev_init(struct drm_device *dev);
void ast_fbdev_fini(struct drm_device *dev);
void ast_fbdev_set_suspend(struct drm_device *dev, int state);
void ast_fbdev_set_base(struct ast_private *ast, unsigned long gpu_addr);

struct ast_bo {
	struct ttm_buffer_object bo;
	struct ttm_placement placement;
	struct ttm_bo_kmap_obj kmap;
	struct drm_gem_object gem;
	struct ttm_place placements[3];
	int pin_count;
};
#define gem_to_ast_bo(gobj) container_of((gobj), struct ast_bo, gem)

static inline struct ast_bo *
ast_bo(struct ttm_buffer_object *bo)
{
	return container_of(bo, struct ast_bo, bo);
}


#define to_ast_obj(x) container_of(x, struct ast_gem_object, base)
=======
struct ast_crtc_state {
	struct drm_crtc_state base;

	/* Last known format of primary plane */
	const struct drm_format_info *format;

	struct ast_vbios_mode_info vbios_mode_info;
};

#define to_ast_crtc_state(state) container_of(state, struct ast_crtc_state, base)

int ast_mode_config_init(struct ast_private *ast);
>>>>>>> upstream/android-13

#define AST_MM_ALIGN_SHIFT 4
#define AST_MM_ALIGN_MASK ((1 << AST_MM_ALIGN_SHIFT) - 1)

<<<<<<< HEAD
extern int ast_dumb_create(struct drm_file *file,
			   struct drm_device *dev,
			   struct drm_mode_create_dumb *args);

extern void ast_gem_free_object(struct drm_gem_object *obj);
extern int ast_dumb_mmap_offset(struct drm_file *file,
				struct drm_device *dev,
				uint32_t handle,
				uint64_t *offset);

#define DRM_FILE_PAGE_OFFSET (0x100000000ULL >> PAGE_SHIFT)

int ast_mm_init(struct ast_private *ast);
void ast_mm_fini(struct ast_private *ast);

int ast_bo_create(struct drm_device *dev, int size, int align,
		  uint32_t flags, struct ast_bo **pastbo);

int ast_gem_create(struct drm_device *dev,
		   u32 size, bool iskernel,
		   struct drm_gem_object **obj);

int ast_bo_pin(struct ast_bo *bo, u32 pl_flag, u64 *gpu_addr);
int ast_bo_unpin(struct ast_bo *bo);

static inline int ast_bo_reserve(struct ast_bo *bo, bool no_wait)
{
	int ret;

	ret = ttm_bo_reserve(&bo->bo, true, no_wait, NULL);
	if (ret) {
		if (ret != -ERESTARTSYS && ret != -EBUSY)
			DRM_ERROR("reserve failed %p\n", bo);
		return ret;
	}
	return 0;
}

static inline void ast_bo_unreserve(struct ast_bo *bo)
{
	ttm_bo_unreserve(&bo->bo);
}

void ast_ttm_placement(struct ast_bo *bo, int domain);
int ast_bo_push_sysram(struct ast_bo *bo);
int ast_mmap(struct file *filp, struct vm_area_struct *vma);
=======
#define AST_DP501_FW_VERSION_MASK	GENMASK(7, 4)
#define AST_DP501_FW_VERSION_1		BIT(4)
#define AST_DP501_PNP_CONNECTED		BIT(1)

#define AST_DP501_DEFAULT_DCLK	65

#define AST_DP501_GBL_VERSION	0xf000
#define AST_DP501_PNPMONITOR	0xf010
#define AST_DP501_LINKRATE	0xf014
#define AST_DP501_EDID_DATA	0xf020

/* Define for Soc scratched reg */
#define AST_VRAM_INIT_STATUS_MASK	GENMASK(7, 6)
//#define AST_VRAM_INIT_BY_BMC		BIT(7)
//#define AST_VRAM_INIT_READY		BIT(6)

int ast_mm_init(struct ast_private *ast);
>>>>>>> upstream/android-13

/* ast post */
void ast_enable_vga(struct drm_device *dev);
void ast_enable_mmio(struct drm_device *dev);
bool ast_is_vga_enabled(struct drm_device *dev);
void ast_post_gpu(struct drm_device *dev);
u32 ast_mindwm(struct ast_private *ast, u32 r);
void ast_moutdwm(struct ast_private *ast, u32 r, u32 v);
<<<<<<< HEAD
=======
void ast_patch_ahb_2500(struct ast_private *ast);
>>>>>>> upstream/android-13
/* ast dp501 */
void ast_set_dp501_video_output(struct drm_device *dev, u8 mode);
bool ast_backup_fw(struct drm_device *dev, u8 *addr, u32 size);
bool ast_dp501_read_edid(struct drm_device *dev, u8 *ediddata);
u8 ast_get_dp501_max_clk(struct drm_device *dev);
void ast_init_3rdtx(struct drm_device *dev);
<<<<<<< HEAD
void ast_release_firmware(struct drm_device *dev);
=======

>>>>>>> upstream/android-13
#endif
