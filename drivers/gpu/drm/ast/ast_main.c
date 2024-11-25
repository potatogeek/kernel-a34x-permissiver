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
<<<<<<< HEAD
#include <drm/drmP.h>
#include "ast_drv.h"


#include <drm/drm_fb_helper.h>
#include <drm/drm_crtc_helper.h>
=======

#include <linux/pci.h>

#include <drm/drm_atomic_helper.h>
#include <drm/drm_crtc_helper.h>
#include <drm/drm_drv.h>
#include <drm/drm_gem.h>
#include <drm/drm_gem_vram_helper.h>
#include <drm/drm_managed.h>

#include "ast_drv.h"
>>>>>>> upstream/android-13

void ast_set_index_reg_mask(struct ast_private *ast,
			    uint32_t base, uint8_t index,
			    uint8_t mask, uint8_t val)
{
	u8 tmp;
	ast_io_write8(ast, base, index);
	tmp = (ast_io_read8(ast, base + 1) & mask) | val;
	ast_set_index_reg(ast, base, index, tmp);
}

uint8_t ast_get_index_reg(struct ast_private *ast,
			  uint32_t base, uint8_t index)
{
	uint8_t ret;
	ast_io_write8(ast, base, index);
	ret = ast_io_read8(ast, base + 1);
	return ret;
}

uint8_t ast_get_index_reg_mask(struct ast_private *ast,
			       uint32_t base, uint8_t index, uint8_t mask)
{
	uint8_t ret;
	ast_io_write8(ast, base, index);
	ret = ast_io_read8(ast, base + 1) & mask;
	return ret;
}

static void ast_detect_config_mode(struct drm_device *dev, u32 *scu_rev)
{
<<<<<<< HEAD
	struct device_node *np = dev->pdev->dev.of_node;
	struct ast_private *ast = dev->dev_private;
=======
	struct device_node *np = dev->dev->of_node;
	struct ast_private *ast = to_ast_private(dev);
	struct pci_dev *pdev = to_pci_dev(dev->dev);
>>>>>>> upstream/android-13
	uint32_t data, jregd0, jregd1;

	/* Defaults */
	ast->config_mode = ast_use_defaults;
	*scu_rev = 0xffffffff;

	/* Check if we have device-tree properties */
	if (np && !of_property_read_u32(np, "aspeed,scu-revision-id",
					scu_rev)) {
		/* We do, disable P2A access */
		ast->config_mode = ast_use_dt;
<<<<<<< HEAD
		DRM_INFO("Using device-tree for configuration\n");
=======
		drm_info(dev, "Using device-tree for configuration\n");
>>>>>>> upstream/android-13
		return;
	}

	/* Not all families have a P2A bridge */
<<<<<<< HEAD
	if (dev->pdev->device != PCI_CHIP_AST2000)
=======
	if (pdev->device != PCI_CHIP_AST2000)
>>>>>>> upstream/android-13
		return;

	/*
	 * The BMC will set SCU 0x40 D[12] to 1 if the P2 bridge
	 * is disabled. We force using P2A if VGA only mode bit
	 * is set D[7]
	 */
	jregd0 = ast_get_index_reg_mask(ast, AST_IO_CRTC_PORT, 0xd0, 0xff);
	jregd1 = ast_get_index_reg_mask(ast, AST_IO_CRTC_PORT, 0xd1, 0xff);
	if (!(jregd0 & 0x80) || !(jregd1 & 0x10)) {
<<<<<<< HEAD
		/* Double check it's actually working */
		data = ast_read32(ast, 0xf004);
		if (data != 0xFFFFFFFF) {
			/* P2A works, grab silicon revision */
			ast->config_mode = ast_use_p2a;

			DRM_INFO("Using P2A bridge for configuration\n");
=======
		/* Patch AST2500 */
		if (((pdev->revision & 0xF0) == 0x40)
			&& ((jregd0 & AST_VRAM_INIT_STATUS_MASK) == 0))
			ast_patch_ahb_2500(ast);

		/* Double check it's actually working */
		data = ast_read32(ast, 0xf004);
		if ((data != 0xFFFFFFFF) && (data != 0x00)) {
			/* P2A works, grab silicon revision */
			ast->config_mode = ast_use_p2a;

			drm_info(dev, "Using P2A bridge for configuration\n");
>>>>>>> upstream/android-13

			/* Read SCU7c (silicon revision register) */
			ast_write32(ast, 0xf004, 0x1e6e0000);
			ast_write32(ast, 0xf000, 0x1);
			*scu_rev = ast_read32(ast, 0x1207c);
			return;
		}
	}

	/* We have a P2A bridge but it's disabled */
<<<<<<< HEAD
	DRM_INFO("P2A bridge disabled, using default configuration\n");
=======
	drm_info(dev, "P2A bridge disabled, using default configuration\n");
>>>>>>> upstream/android-13
}

static int ast_detect_chip(struct drm_device *dev, bool *need_post)
{
<<<<<<< HEAD
	struct ast_private *ast = dev->dev_private;
=======
	struct ast_private *ast = to_ast_private(dev);
	struct pci_dev *pdev = to_pci_dev(dev->dev);
>>>>>>> upstream/android-13
	uint32_t jreg, scu_rev;

	/*
	 * If VGA isn't enabled, we need to enable now or subsequent
	 * access to the scratch registers will fail. We also inform
	 * our caller that it needs to POST the chip
	 * (Assumption: VGA not enabled -> need to POST)
	 */
	if (!ast_is_vga_enabled(dev)) {
		ast_enable_vga(dev);
<<<<<<< HEAD
		DRM_INFO("VGA not enabled on entry, requesting chip POST\n");
=======
		drm_info(dev, "VGA not enabled on entry, requesting chip POST\n");
>>>>>>> upstream/android-13
		*need_post = true;
	} else
		*need_post = false;


	/* Enable extended register access */
	ast_open_key(ast);
	ast_enable_mmio(dev);

	/* Find out whether P2A works or whether to use device-tree */
	ast_detect_config_mode(dev, &scu_rev);

	/* Identify chipset */
<<<<<<< HEAD
	if (dev->pdev->device == PCI_CHIP_AST1180) {
		ast->chip = AST1100;
		DRM_INFO("AST 1180 detected\n");
	} else {
		if (dev->pdev->revision >= 0x40) {
			ast->chip = AST2500;
			DRM_INFO("AST 2500 detected\n");
		} else if (dev->pdev->revision >= 0x30) {
			ast->chip = AST2400;
			DRM_INFO("AST 2400 detected\n");
		} else if (dev->pdev->revision >= 0x20) {
			ast->chip = AST2300;
			DRM_INFO("AST 2300 detected\n");
		} else if (dev->pdev->revision >= 0x10) {
			switch (scu_rev & 0x0300) {
			case 0x0200:
				ast->chip = AST1100;
				DRM_INFO("AST 1100 detected\n");
				break;
			case 0x0100:
				ast->chip = AST2200;
				DRM_INFO("AST 2200 detected\n");
				break;
			case 0x0000:
				ast->chip = AST2150;
				DRM_INFO("AST 2150 detected\n");
				break;
			default:
				ast->chip = AST2100;
				DRM_INFO("AST 2100 detected\n");
				break;
			}
			ast->vga2_clone = false;
		} else {
			ast->chip = AST2000;
			DRM_INFO("AST 2000 detected\n");
		}
=======
	if (pdev->revision >= 0x50) {
		ast->chip = AST2600;
		drm_info(dev, "AST 2600 detected\n");
	} else if (pdev->revision >= 0x40) {
		ast->chip = AST2500;
		drm_info(dev, "AST 2500 detected\n");
	} else if (pdev->revision >= 0x30) {
		ast->chip = AST2400;
		drm_info(dev, "AST 2400 detected\n");
	} else if (pdev->revision >= 0x20) {
		ast->chip = AST2300;
		drm_info(dev, "AST 2300 detected\n");
	} else if (pdev->revision >= 0x10) {
		switch (scu_rev & 0x0300) {
		case 0x0200:
			ast->chip = AST1100;
			drm_info(dev, "AST 1100 detected\n");
			break;
		case 0x0100:
			ast->chip = AST2200;
			drm_info(dev, "AST 2200 detected\n");
			break;
		case 0x0000:
			ast->chip = AST2150;
			drm_info(dev, "AST 2150 detected\n");
			break;
		default:
			ast->chip = AST2100;
			drm_info(dev, "AST 2100 detected\n");
			break;
		}
		ast->vga2_clone = false;
	} else {
		ast->chip = AST2000;
		drm_info(dev, "AST 2000 detected\n");
>>>>>>> upstream/android-13
	}

	/* Check if we support wide screen */
	switch (ast->chip) {
<<<<<<< HEAD
	case AST1180:
		ast->support_wide_screen = true;
		break;
=======
>>>>>>> upstream/android-13
	case AST2000:
		ast->support_wide_screen = false;
		break;
	default:
		jreg = ast_get_index_reg_mask(ast, AST_IO_CRTC_PORT, 0xd0, 0xff);
		if (!(jreg & 0x80))
			ast->support_wide_screen = true;
		else if (jreg & 0x01)
			ast->support_wide_screen = true;
		else {
			ast->support_wide_screen = false;
			if (ast->chip == AST2300 &&
			    (scu_rev & 0x300) == 0x0) /* ast1300 */
				ast->support_wide_screen = true;
			if (ast->chip == AST2400 &&
			    (scu_rev & 0x300) == 0x100) /* ast1400 */
				ast->support_wide_screen = true;
			if (ast->chip == AST2500 &&
			    scu_rev == 0x100)           /* ast2510 */
				ast->support_wide_screen = true;
		}
		break;
	}

	/* Check 3rd Tx option (digital output afaik) */
	ast->tx_chip_type = AST_TX_NONE;

	/*
	 * VGACRA3 Enhanced Color Mode Register, check if DVO is already
	 * enabled, in that case, assume we have a SIL164 TMDS transmitter
	 *
	 * Don't make that assumption if we the chip wasn't enabled and
	 * is at power-on reset, otherwise we'll incorrectly "detect" a
	 * SIL164 when there is none.
	 */
	if (!*need_post) {
		jreg = ast_get_index_reg_mask(ast, AST_IO_CRTC_PORT, 0xa3, 0xff);
		if (jreg & 0x80)
			ast->tx_chip_type = AST_TX_SIL164;
	}

	if ((ast->chip == AST2300) || (ast->chip == AST2400)) {
		/*
		 * On AST2300 and 2400, look the configuration set by the SoC in
		 * the SOC scratch register #1 bits 11:8 (interestingly marked
		 * as "reserved" in the spec)
		 */
		jreg = ast_get_index_reg_mask(ast, AST_IO_CRTC_PORT, 0xd1, 0xff);
		switch (jreg) {
		case 0x04:
			ast->tx_chip_type = AST_TX_SIL164;
			break;
		case 0x08:
<<<<<<< HEAD
			ast->dp501_fw_addr = kzalloc(32*1024, GFP_KERNEL);
			if (ast->dp501_fw_addr) {
				/* backup firmware */
				if (ast_backup_fw(dev, ast->dp501_fw_addr, 32*1024)) {
					kfree(ast->dp501_fw_addr);
					ast->dp501_fw_addr = NULL;
				}
			}
			/* fallthrough */
=======
			ast->dp501_fw_addr = drmm_kzalloc(dev, 32*1024, GFP_KERNEL);
			if (ast->dp501_fw_addr) {
				/* backup firmware */
				if (ast_backup_fw(dev, ast->dp501_fw_addr, 32*1024)) {
					drmm_kfree(dev, ast->dp501_fw_addr);
					ast->dp501_fw_addr = NULL;
				}
			}
			fallthrough;
>>>>>>> upstream/android-13
		case 0x0c:
			ast->tx_chip_type = AST_TX_DP501;
		}
	}

	/* Print stuff for diagnostic purposes */
	switch(ast->tx_chip_type) {
	case AST_TX_SIL164:
<<<<<<< HEAD
		DRM_INFO("Using Sil164 TMDS transmitter\n");
		break;
	case AST_TX_DP501:
		DRM_INFO("Using DP501 DisplayPort transmitter\n");
		break;
	default:
		DRM_INFO("Analog VGA only\n");
=======
		drm_info(dev, "Using Sil164 TMDS transmitter\n");
		break;
	case AST_TX_DP501:
		drm_info(dev, "Using DP501 DisplayPort transmitter\n");
		break;
	default:
		drm_info(dev, "Analog VGA only\n");
>>>>>>> upstream/android-13
	}
	return 0;
}

static int ast_get_dram_info(struct drm_device *dev)
{
<<<<<<< HEAD
	struct device_node *np = dev->pdev->dev.of_node;
	struct ast_private *ast = dev->dev_private;
=======
	struct device_node *np = dev->dev->of_node;
	struct ast_private *ast = to_ast_private(dev);
>>>>>>> upstream/android-13
	uint32_t mcr_cfg, mcr_scu_mpll, mcr_scu_strap;
	uint32_t denum, num, div, ref_pll, dsel;

	switch (ast->config_mode) {
	case ast_use_dt:
		/*
		 * If some properties are missing, use reasonable
		 * defaults for AST2400
		 */
		if (of_property_read_u32(np, "aspeed,mcr-configuration",
					 &mcr_cfg))
			mcr_cfg = 0x00000577;
		if (of_property_read_u32(np, "aspeed,mcr-scu-mpll",
					 &mcr_scu_mpll))
			mcr_scu_mpll = 0x000050C0;
		if (of_property_read_u32(np, "aspeed,mcr-scu-strap",
					 &mcr_scu_strap))
			mcr_scu_strap = 0;
		break;
	case ast_use_p2a:
		ast_write32(ast, 0xf004, 0x1e6e0000);
		ast_write32(ast, 0xf000, 0x1);
		mcr_cfg = ast_read32(ast, 0x10004);
		mcr_scu_mpll = ast_read32(ast, 0x10120);
		mcr_scu_strap = ast_read32(ast, 0x10170);
		break;
	case ast_use_defaults:
	default:
		ast->dram_bus_width = 16;
		ast->dram_type = AST_DRAM_1Gx16;
		if (ast->chip == AST2500)
			ast->mclk = 800;
		else
			ast->mclk = 396;
		return 0;
	}

	if (mcr_cfg & 0x40)
		ast->dram_bus_width = 16;
	else
		ast->dram_bus_width = 32;

	if (ast->chip == AST2500) {
		switch (mcr_cfg & 0x03) {
		case 0:
			ast->dram_type = AST_DRAM_1Gx16;
			break;
		default:
		case 1:
			ast->dram_type = AST_DRAM_2Gx16;
			break;
		case 2:
			ast->dram_type = AST_DRAM_4Gx16;
			break;
		case 3:
			ast->dram_type = AST_DRAM_8Gx16;
			break;
		}
	} else if (ast->chip == AST2300 || ast->chip == AST2400) {
		switch (mcr_cfg & 0x03) {
		case 0:
			ast->dram_type = AST_DRAM_512Mx16;
			break;
		default:
		case 1:
			ast->dram_type = AST_DRAM_1Gx16;
			break;
		case 2:
			ast->dram_type = AST_DRAM_2Gx16;
			break;
		case 3:
			ast->dram_type = AST_DRAM_4Gx16;
			break;
		}
	} else {
		switch (mcr_cfg & 0x0c) {
		case 0:
		case 4:
			ast->dram_type = AST_DRAM_512Mx16;
			break;
		case 8:
			if (mcr_cfg & 0x40)
				ast->dram_type = AST_DRAM_1Gx16;
			else
				ast->dram_type = AST_DRAM_512Mx32;
			break;
		case 0xc:
			ast->dram_type = AST_DRAM_1Gx32;
			break;
		}
	}

	if (mcr_scu_strap & 0x2000)
		ref_pll = 14318;
	else
		ref_pll = 12000;

	denum = mcr_scu_mpll & 0x1f;
	num = (mcr_scu_mpll & 0x3fe0) >> 5;
	dsel = (mcr_scu_mpll & 0xc000) >> 14;
	switch (dsel) {
	case 3:
		div = 0x4;
		break;
	case 2:
	case 1:
		div = 0x2;
		break;
	default:
		div = 0x1;
		break;
	}
	ast->mclk = ref_pll * (num + 2) / ((denum + 2) * (div * 1000));
	return 0;
}

<<<<<<< HEAD
static void ast_user_framebuffer_destroy(struct drm_framebuffer *fb)
{
	struct ast_framebuffer *ast_fb = to_ast_framebuffer(fb);

	drm_gem_object_put_unlocked(ast_fb->obj);
	drm_framebuffer_cleanup(fb);
	kfree(ast_fb);
}

static const struct drm_framebuffer_funcs ast_fb_funcs = {
	.destroy = ast_user_framebuffer_destroy,
};


int ast_framebuffer_init(struct drm_device *dev,
			 struct ast_framebuffer *ast_fb,
			 const struct drm_mode_fb_cmd2 *mode_cmd,
			 struct drm_gem_object *obj)
{
	int ret;

	drm_helper_mode_fill_fb_struct(dev, &ast_fb->base, mode_cmd);
	ast_fb->obj = obj;
	ret = drm_framebuffer_init(dev, &ast_fb->base, &ast_fb_funcs);
	if (ret) {
		DRM_ERROR("framebuffer init failed %d\n", ret);
		return ret;
	}
	return 0;
}

static struct drm_framebuffer *
ast_user_framebuffer_create(struct drm_device *dev,
	       struct drm_file *filp,
	       const struct drm_mode_fb_cmd2 *mode_cmd)
{
	struct drm_gem_object *obj;
	struct ast_framebuffer *ast_fb;
	int ret;

	obj = drm_gem_object_lookup(filp, mode_cmd->handles[0]);
	if (obj == NULL)
		return ERR_PTR(-ENOENT);

	ast_fb = kzalloc(sizeof(*ast_fb), GFP_KERNEL);
	if (!ast_fb) {
		drm_gem_object_put_unlocked(obj);
		return ERR_PTR(-ENOMEM);
	}

	ret = ast_framebuffer_init(dev, ast_fb, mode_cmd, obj);
	if (ret) {
		drm_gem_object_put_unlocked(obj);
		kfree(ast_fb);
		return ERR_PTR(ret);
	}
	return &ast_fb->base;
}

static const struct drm_mode_config_funcs ast_mode_funcs = {
	.fb_create = ast_user_framebuffer_create,
};

static u32 ast_get_vram_info(struct drm_device *dev)
{
	struct ast_private *ast = dev->dev_private;
	u8 jreg;
	u32 vram_size;
	ast_open_key(ast);

	vram_size = AST_VIDMEM_DEFAULT_SIZE;
	jreg = ast_get_index_reg_mask(ast, AST_IO_CRTC_PORT, 0xaa, 0xff);
	switch (jreg & 3) {
	case 0: vram_size = AST_VIDMEM_SIZE_8M; break;
	case 1: vram_size = AST_VIDMEM_SIZE_16M; break;
	case 2: vram_size = AST_VIDMEM_SIZE_32M; break;
	case 3: vram_size = AST_VIDMEM_SIZE_64M; break;
	}

	jreg = ast_get_index_reg_mask(ast, AST_IO_CRTC_PORT, 0x99, 0xff);
	switch (jreg & 0x03) {
	case 1:
		vram_size -= 0x100000;
		break;
	case 2:
		vram_size -= 0x200000;
		break;
	case 3:
		vram_size -= 0x400000;
		break;
	}

	return vram_size;
}

int ast_driver_load(struct drm_device *dev, unsigned long flags)
{
=======
/*
 * Run this function as part of the HW device cleanup; not
 * when the DRM device gets released.
 */
static void ast_device_release(void *data)
{
	struct ast_private *ast = data;

	/* enable standard VGA decode */
	ast_set_index_reg(ast, AST_IO_CRTC_PORT, 0xa1, 0x04);
}

struct ast_private *ast_device_create(const struct drm_driver *drv,
				      struct pci_dev *pdev,
				      unsigned long flags)
{
	struct drm_device *dev;
>>>>>>> upstream/android-13
	struct ast_private *ast;
	bool need_post;
	int ret = 0;

<<<<<<< HEAD
	ast = kzalloc(sizeof(struct ast_private), GFP_KERNEL);
	if (!ast)
		return -ENOMEM;

	dev->dev_private = ast;
	ast->dev = dev;

	ast->regs = pci_iomap(dev->pdev, 1, 0);
	if (!ast->regs) {
		ret = -EIO;
		goto out_free;
	}
=======
	ast = devm_drm_dev_alloc(&pdev->dev, drv, struct ast_private, base);
	if (IS_ERR(ast))
		return ast;
	dev = &ast->base;

	pci_set_drvdata(pdev, dev);

	ast->regs = pcim_iomap(pdev, 1, 0);
	if (!ast->regs)
		return ERR_PTR(-EIO);
>>>>>>> upstream/android-13

	/*
	 * If we don't have IO space at all, use MMIO now and
	 * assume the chip has MMIO enabled by default (rev 0x20
	 * and higher).
	 */
<<<<<<< HEAD
	if (!(pci_resource_flags(dev->pdev, 2) & IORESOURCE_IO)) {
		DRM_INFO("platform has no IO space, trying MMIO\n");
=======
	if (!(pci_resource_flags(pdev, 2) & IORESOURCE_IO)) {
		drm_info(dev, "platform has no IO space, trying MMIO\n");
>>>>>>> upstream/android-13
		ast->ioregs = ast->regs + AST_IO_MM_OFFSET;
	}

	/* "map" IO regs if the above hasn't done so already */
	if (!ast->ioregs) {
<<<<<<< HEAD
		ast->ioregs = pci_iomap(dev->pdev, 2, 0);
		if (!ast->ioregs) {
			ret = -EIO;
			goto out_free;
		}
=======
		ast->ioregs = pcim_iomap(pdev, 2, 0);
		if (!ast->ioregs)
			return ERR_PTR(-EIO);
>>>>>>> upstream/android-13
	}

	ast_detect_chip(dev, &need_post);

<<<<<<< HEAD
	if (need_post)
		ast_post_gpu(dev);

	if (ast->chip != AST1180) {
		ret = ast_get_dram_info(dev);
		if (ret)
			goto out_free;
		ast->vram_size = ast_get_vram_info(dev);
		DRM_INFO("dram MCLK=%u Mhz type=%d bus_width=%d size=%08x\n",
			 ast->mclk, ast->dram_type,
			 ast->dram_bus_width, ast->vram_size);
	}

	ret = ast_mm_init(ast);
	if (ret)
		goto out_free;

	drm_mode_config_init(dev);

	dev->mode_config.funcs = (void *)&ast_mode_funcs;
	dev->mode_config.min_width = 0;
	dev->mode_config.min_height = 0;
	dev->mode_config.preferred_depth = 24;
	dev->mode_config.prefer_shadow = 1;
	dev->mode_config.fb_base = pci_resource_start(ast->dev->pdev, 0);

	if (ast->chip == AST2100 ||
	    ast->chip == AST2200 ||
	    ast->chip == AST2300 ||
	    ast->chip == AST2400 ||
	    ast->chip == AST2500 ||
	    ast->chip == AST1180) {
		dev->mode_config.max_width = 1920;
		dev->mode_config.max_height = 2048;
	} else {
		dev->mode_config.max_width = 1600;
		dev->mode_config.max_height = 1200;
	}

	ret = ast_mode_init(dev);
	if (ret)
		goto out_free;

	ret = ast_fbdev_init(dev);
	if (ret)
		goto out_free;

	return 0;
out_free:
	kfree(ast);
	dev->dev_private = NULL;
	return ret;
}

void ast_driver_unload(struct drm_device *dev)
{
	struct ast_private *ast = dev->dev_private;

	/* enable standard VGA decode */
	ast_set_index_reg(ast, AST_IO_CRTC_PORT, 0xa1, 0x04);

	ast_release_firmware(dev);
	kfree(ast->dp501_fw_addr);
	ast_mode_fini(dev);
	ast_fbdev_fini(dev);
	drm_mode_config_cleanup(dev);

	ast_mm_fini(ast);
	if (ast->ioregs != ast->regs + AST_IO_MM_OFFSET)
		pci_iounmap(dev->pdev, ast->ioregs);
	pci_iounmap(dev->pdev, ast->regs);
	kfree(ast);
}

int ast_gem_create(struct drm_device *dev,
		   u32 size, bool iskernel,
		   struct drm_gem_object **obj)
{
	struct ast_bo *astbo;
	int ret;

	*obj = NULL;

	size = roundup(size, PAGE_SIZE);
	if (size == 0)
		return -EINVAL;

	ret = ast_bo_create(dev, size, 0, 0, &astbo);
	if (ret) {
		if (ret != -ERESTARTSYS)
			DRM_ERROR("failed to allocate GEM object\n");
		return ret;
	}
	*obj = &astbo->gem;
	return 0;
}

int ast_dumb_create(struct drm_file *file,
		    struct drm_device *dev,
		    struct drm_mode_create_dumb *args)
{
	int ret;
	struct drm_gem_object *gobj;
	u32 handle;

	args->pitch = args->width * ((args->bpp + 7) / 8);
	args->size = args->pitch * args->height;

	ret = ast_gem_create(dev, args->size, false,
			     &gobj);
	if (ret)
		return ret;

	ret = drm_gem_handle_create(file, gobj, &handle);
	drm_gem_object_put_unlocked(gobj);
	if (ret)
		return ret;

	args->handle = handle;
	return 0;
}

static void ast_bo_unref(struct ast_bo **bo)
{
	struct ttm_buffer_object *tbo;

	if ((*bo) == NULL)
		return;

	tbo = &((*bo)->bo);
	ttm_bo_unref(&tbo);
	*bo = NULL;
}

void ast_gem_free_object(struct drm_gem_object *obj)
{
	struct ast_bo *ast_bo = gem_to_ast_bo(obj);

	ast_bo_unref(&ast_bo);
}


static inline u64 ast_bo_mmap_offset(struct ast_bo *bo)
{
	return drm_vma_node_offset_addr(&bo->bo.vma_node);
}
int
ast_dumb_mmap_offset(struct drm_file *file,
		     struct drm_device *dev,
		     uint32_t handle,
		     uint64_t *offset)
{
	struct drm_gem_object *obj;
	struct ast_bo *bo;

	obj = drm_gem_object_lookup(file, handle);
	if (obj == NULL)
		return -ENOENT;

	bo = gem_to_ast_bo(obj);
	*offset = ast_bo_mmap_offset(bo);

	drm_gem_object_put_unlocked(obj);

	return 0;

}

=======
	ret = ast_get_dram_info(dev);
	if (ret)
		return ERR_PTR(ret);

	drm_info(dev, "dram MCLK=%u Mhz type=%d bus_width=%d\n",
		 ast->mclk, ast->dram_type, ast->dram_bus_width);

	if (need_post)
		ast_post_gpu(dev);

	ret = ast_mm_init(ast);
	if (ret)
		return ERR_PTR(ret);

	/* map reserved buffer */
	ast->dp501_fw_buf = NULL;
	if (dev->vram_mm->vram_size < pci_resource_len(pdev, 0)) {
		ast->dp501_fw_buf = pci_iomap_range(pdev, 0, dev->vram_mm->vram_size, 0);
		if (!ast->dp501_fw_buf)
			drm_info(dev, "failed to map reserved buffer!\n");
	}

	ret = ast_mode_config_init(ast);
	if (ret)
		return ERR_PTR(ret);

	ret = devm_add_action_or_reset(dev->dev, ast_device_release, ast);
	if (ret)
		return ERR_PTR(ret);

	return ast;
}
>>>>>>> upstream/android-13
