/*
 * Copyright 2012 Red Hat Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE COPYRIGHT HOLDER(S) OR AUTHOR(S) BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * Authors: Ben Skeggs
 */
#include "priv.h"
#include "acpi.h"

#include <core/notify.h>
#include <core/option.h>

#include <subdev/bios.h>
#include <subdev/therm.h>

static DEFINE_MUTEX(nv_devices_mutex);
static LIST_HEAD(nv_devices);

static struct nvkm_device *
nvkm_device_find_locked(u64 handle)
{
	struct nvkm_device *device;
	list_for_each_entry(device, &nv_devices, head) {
		if (device->handle == handle)
			return device;
	}
	return NULL;
}

struct nvkm_device *
nvkm_device_find(u64 handle)
{
	struct nvkm_device *device;
	mutex_lock(&nv_devices_mutex);
	device = nvkm_device_find_locked(handle);
	mutex_unlock(&nv_devices_mutex);
	return device;
}

int
nvkm_device_list(u64 *name, int size)
{
	struct nvkm_device *device;
	int nr = 0;
	mutex_lock(&nv_devices_mutex);
	list_for_each_entry(device, &nv_devices, head) {
		if (nr++ < size)
			name[nr - 1] = device->handle;
	}
	mutex_unlock(&nv_devices_mutex);
	return nr;
}

static const struct nvkm_device_chip
null_chipset = {
	.name = "NULL",
<<<<<<< HEAD
	.bios = nvkm_bios_new,
=======
	.bios     = { 0x00000001, nvkm_bios_new },
>>>>>>> upstream/android-13
};

static const struct nvkm_device_chip
nv4_chipset = {
	.name = "NV04",
<<<<<<< HEAD
	.bios = nvkm_bios_new,
	.bus = nv04_bus_new,
	.clk = nv04_clk_new,
	.devinit = nv04_devinit_new,
	.fb = nv04_fb_new,
	.i2c = nv04_i2c_new,
	.imem = nv04_instmem_new,
	.mc = nv04_mc_new,
	.mmu = nv04_mmu_new,
	.pci = nv04_pci_new,
	.timer = nv04_timer_new,
	.disp = nv04_disp_new,
	.dma = nv04_dma_new,
	.fifo = nv04_fifo_new,
	.gr = nv04_gr_new,
	.sw = nv04_sw_new,
=======
	.bios     = { 0x00000001, nvkm_bios_new },
	.bus      = { 0x00000001, nv04_bus_new },
	.clk      = { 0x00000001, nv04_clk_new },
	.devinit  = { 0x00000001, nv04_devinit_new },
	.fb       = { 0x00000001, nv04_fb_new },
	.i2c      = { 0x00000001, nv04_i2c_new },
	.imem     = { 0x00000001, nv04_instmem_new },
	.mc       = { 0x00000001, nv04_mc_new },
	.mmu      = { 0x00000001, nv04_mmu_new },
	.pci      = { 0x00000001, nv04_pci_new },
	.timer    = { 0x00000001, nv04_timer_new },
	.disp     = { 0x00000001, nv04_disp_new },
	.dma      = { 0x00000001, nv04_dma_new },
	.fifo     = { 0x00000001, nv04_fifo_new },
	.gr       = { 0x00000001, nv04_gr_new },
	.sw       = { 0x00000001, nv04_sw_new },
>>>>>>> upstream/android-13
};

static const struct nvkm_device_chip
nv5_chipset = {
	.name = "NV05",
<<<<<<< HEAD
	.bios = nvkm_bios_new,
	.bus = nv04_bus_new,
	.clk = nv04_clk_new,
	.devinit = nv05_devinit_new,
	.fb = nv04_fb_new,
	.i2c = nv04_i2c_new,
	.imem = nv04_instmem_new,
	.mc = nv04_mc_new,
	.mmu = nv04_mmu_new,
	.pci = nv04_pci_new,
	.timer = nv04_timer_new,
	.disp = nv04_disp_new,
	.dma = nv04_dma_new,
	.fifo = nv04_fifo_new,
	.gr = nv04_gr_new,
	.sw = nv04_sw_new,
=======
	.bios     = { 0x00000001, nvkm_bios_new },
	.bus      = { 0x00000001, nv04_bus_new },
	.clk      = { 0x00000001, nv04_clk_new },
	.devinit  = { 0x00000001, nv05_devinit_new },
	.fb       = { 0x00000001, nv04_fb_new },
	.i2c      = { 0x00000001, nv04_i2c_new },
	.imem     = { 0x00000001, nv04_instmem_new },
	.mc       = { 0x00000001, nv04_mc_new },
	.mmu      = { 0x00000001, nv04_mmu_new },
	.pci      = { 0x00000001, nv04_pci_new },
	.timer    = { 0x00000001, nv04_timer_new },
	.disp     = { 0x00000001, nv04_disp_new },
	.dma      = { 0x00000001, nv04_dma_new },
	.fifo     = { 0x00000001, nv04_fifo_new },
	.gr       = { 0x00000001, nv04_gr_new },
	.sw       = { 0x00000001, nv04_sw_new },
>>>>>>> upstream/android-13
};

static const struct nvkm_device_chip
nv10_chipset = {
	.name = "NV10",
<<<<<<< HEAD
	.bios = nvkm_bios_new,
	.bus = nv04_bus_new,
	.clk = nv04_clk_new,
	.devinit = nv10_devinit_new,
	.fb = nv10_fb_new,
	.gpio = nv10_gpio_new,
	.i2c = nv04_i2c_new,
	.imem = nv04_instmem_new,
	.mc = nv04_mc_new,
	.mmu = nv04_mmu_new,
	.pci = nv04_pci_new,
	.timer = nv04_timer_new,
	.disp = nv04_disp_new,
	.dma = nv04_dma_new,
	.gr = nv10_gr_new,
=======
	.bios     = { 0x00000001, nvkm_bios_new },
	.bus      = { 0x00000001, nv04_bus_new },
	.clk      = { 0x00000001, nv04_clk_new },
	.devinit  = { 0x00000001, nv10_devinit_new },
	.fb       = { 0x00000001, nv10_fb_new },
	.gpio     = { 0x00000001, nv10_gpio_new },
	.i2c      = { 0x00000001, nv04_i2c_new },
	.imem     = { 0x00000001, nv04_instmem_new },
	.mc       = { 0x00000001, nv04_mc_new },
	.mmu      = { 0x00000001, nv04_mmu_new },
	.pci      = { 0x00000001, nv04_pci_new },
	.timer    = { 0x00000001, nv04_timer_new },
	.disp     = { 0x00000001, nv04_disp_new },
	.dma      = { 0x00000001, nv04_dma_new },
	.gr       = { 0x00000001, nv10_gr_new },
>>>>>>> upstream/android-13
};

static const struct nvkm_device_chip
nv11_chipset = {
	.name = "NV11",
<<<<<<< HEAD
	.bios = nvkm_bios_new,
	.bus = nv04_bus_new,
	.clk = nv04_clk_new,
	.devinit = nv10_devinit_new,
	.fb = nv10_fb_new,
	.gpio = nv10_gpio_new,
	.i2c = nv04_i2c_new,
	.imem = nv04_instmem_new,
	.mc = nv11_mc_new,
	.mmu = nv04_mmu_new,
	.pci = nv04_pci_new,
	.timer = nv04_timer_new,
	.disp = nv04_disp_new,
	.dma = nv04_dma_new,
	.fifo = nv10_fifo_new,
	.gr = nv15_gr_new,
	.sw = nv10_sw_new,
=======
	.bios     = { 0x00000001, nvkm_bios_new },
	.bus      = { 0x00000001, nv04_bus_new },
	.clk      = { 0x00000001, nv04_clk_new },
	.devinit  = { 0x00000001, nv10_devinit_new },
	.fb       = { 0x00000001, nv10_fb_new },
	.gpio     = { 0x00000001, nv10_gpio_new },
	.i2c      = { 0x00000001, nv04_i2c_new },
	.imem     = { 0x00000001, nv04_instmem_new },
	.mc       = { 0x00000001, nv11_mc_new },
	.mmu      = { 0x00000001, nv04_mmu_new },
	.pci      = { 0x00000001, nv04_pci_new },
	.timer    = { 0x00000001, nv04_timer_new },
	.disp     = { 0x00000001, nv04_disp_new },
	.dma      = { 0x00000001, nv04_dma_new },
	.fifo     = { 0x00000001, nv10_fifo_new },
	.gr       = { 0x00000001, nv15_gr_new },
	.sw       = { 0x00000001, nv10_sw_new },
>>>>>>> upstream/android-13
};

static const struct nvkm_device_chip
nv15_chipset = {
	.name = "NV15",
<<<<<<< HEAD
	.bios = nvkm_bios_new,
	.bus = nv04_bus_new,
	.clk = nv04_clk_new,
	.devinit = nv10_devinit_new,
	.fb = nv10_fb_new,
	.gpio = nv10_gpio_new,
	.i2c = nv04_i2c_new,
	.imem = nv04_instmem_new,
	.mc = nv04_mc_new,
	.mmu = nv04_mmu_new,
	.pci = nv04_pci_new,
	.timer = nv04_timer_new,
	.disp = nv04_disp_new,
	.dma = nv04_dma_new,
	.fifo = nv10_fifo_new,
	.gr = nv15_gr_new,
	.sw = nv10_sw_new,
=======
	.bios     = { 0x00000001, nvkm_bios_new },
	.bus      = { 0x00000001, nv04_bus_new },
	.clk      = { 0x00000001, nv04_clk_new },
	.devinit  = { 0x00000001, nv10_devinit_new },
	.fb       = { 0x00000001, nv10_fb_new },
	.gpio     = { 0x00000001, nv10_gpio_new },
	.i2c      = { 0x00000001, nv04_i2c_new },
	.imem     = { 0x00000001, nv04_instmem_new },
	.mc       = { 0x00000001, nv04_mc_new },
	.mmu      = { 0x00000001, nv04_mmu_new },
	.pci      = { 0x00000001, nv04_pci_new },
	.timer    = { 0x00000001, nv04_timer_new },
	.disp     = { 0x00000001, nv04_disp_new },
	.dma      = { 0x00000001, nv04_dma_new },
	.fifo     = { 0x00000001, nv10_fifo_new },
	.gr       = { 0x00000001, nv15_gr_new },
	.sw       = { 0x00000001, nv10_sw_new },
>>>>>>> upstream/android-13
};

static const struct nvkm_device_chip
nv17_chipset = {
	.name = "NV17",
<<<<<<< HEAD
	.bios = nvkm_bios_new,
	.bus = nv04_bus_new,
	.clk = nv04_clk_new,
	.devinit = nv10_devinit_new,
	.fb = nv10_fb_new,
	.gpio = nv10_gpio_new,
	.i2c = nv04_i2c_new,
	.imem = nv04_instmem_new,
	.mc = nv17_mc_new,
	.mmu = nv04_mmu_new,
	.pci = nv04_pci_new,
	.timer = nv04_timer_new,
	.disp = nv04_disp_new,
	.dma = nv04_dma_new,
	.fifo = nv17_fifo_new,
	.gr = nv17_gr_new,
	.sw = nv10_sw_new,
=======
	.bios     = { 0x00000001, nvkm_bios_new },
	.bus      = { 0x00000001, nv04_bus_new },
	.clk      = { 0x00000001, nv04_clk_new },
	.devinit  = { 0x00000001, nv10_devinit_new },
	.fb       = { 0x00000001, nv10_fb_new },
	.gpio     = { 0x00000001, nv10_gpio_new },
	.i2c      = { 0x00000001, nv04_i2c_new },
	.imem     = { 0x00000001, nv04_instmem_new },
	.mc       = { 0x00000001, nv17_mc_new },
	.mmu      = { 0x00000001, nv04_mmu_new },
	.pci      = { 0x00000001, nv04_pci_new },
	.timer    = { 0x00000001, nv04_timer_new },
	.disp     = { 0x00000001, nv04_disp_new },
	.dma      = { 0x00000001, nv04_dma_new },
	.fifo     = { 0x00000001, nv17_fifo_new },
	.gr       = { 0x00000001, nv17_gr_new },
	.sw       = { 0x00000001, nv10_sw_new },
>>>>>>> upstream/android-13
};

static const struct nvkm_device_chip
nv18_chipset = {
	.name = "NV18",
<<<<<<< HEAD
	.bios = nvkm_bios_new,
	.bus = nv04_bus_new,
	.clk = nv04_clk_new,
	.devinit = nv10_devinit_new,
	.fb = nv10_fb_new,
	.gpio = nv10_gpio_new,
	.i2c = nv04_i2c_new,
	.imem = nv04_instmem_new,
	.mc = nv17_mc_new,
	.mmu = nv04_mmu_new,
	.pci = nv04_pci_new,
	.timer = nv04_timer_new,
	.disp = nv04_disp_new,
	.dma = nv04_dma_new,
	.fifo = nv17_fifo_new,
	.gr = nv17_gr_new,
	.sw = nv10_sw_new,
=======
	.bios     = { 0x00000001, nvkm_bios_new },
	.bus      = { 0x00000001, nv04_bus_new },
	.clk      = { 0x00000001, nv04_clk_new },
	.devinit  = { 0x00000001, nv10_devinit_new },
	.fb       = { 0x00000001, nv10_fb_new },
	.gpio     = { 0x00000001, nv10_gpio_new },
	.i2c      = { 0x00000001, nv04_i2c_new },
	.imem     = { 0x00000001, nv04_instmem_new },
	.mc       = { 0x00000001, nv17_mc_new },
	.mmu      = { 0x00000001, nv04_mmu_new },
	.pci      = { 0x00000001, nv04_pci_new },
	.timer    = { 0x00000001, nv04_timer_new },
	.disp     = { 0x00000001, nv04_disp_new },
	.dma      = { 0x00000001, nv04_dma_new },
	.fifo     = { 0x00000001, nv17_fifo_new },
	.gr       = { 0x00000001, nv17_gr_new },
	.sw       = { 0x00000001, nv10_sw_new },
>>>>>>> upstream/android-13
};

static const struct nvkm_device_chip
nv1a_chipset = {
	.name = "nForce",
<<<<<<< HEAD
	.bios = nvkm_bios_new,
	.bus = nv04_bus_new,
	.clk = nv04_clk_new,
	.devinit = nv1a_devinit_new,
	.fb = nv1a_fb_new,
	.gpio = nv10_gpio_new,
	.i2c = nv04_i2c_new,
	.imem = nv04_instmem_new,
	.mc = nv04_mc_new,
	.mmu = nv04_mmu_new,
	.pci = nv04_pci_new,
	.timer = nv04_timer_new,
	.disp = nv04_disp_new,
	.dma = nv04_dma_new,
	.fifo = nv10_fifo_new,
	.gr = nv15_gr_new,
	.sw = nv10_sw_new,
=======
	.bios     = { 0x00000001, nvkm_bios_new },
	.bus      = { 0x00000001, nv04_bus_new },
	.clk      = { 0x00000001, nv04_clk_new },
	.devinit  = { 0x00000001, nv1a_devinit_new },
	.fb       = { 0x00000001, nv1a_fb_new },
	.gpio     = { 0x00000001, nv10_gpio_new },
	.i2c      = { 0x00000001, nv04_i2c_new },
	.imem     = { 0x00000001, nv04_instmem_new },
	.mc       = { 0x00000001, nv04_mc_new },
	.mmu      = { 0x00000001, nv04_mmu_new },
	.pci      = { 0x00000001, nv04_pci_new },
	.timer    = { 0x00000001, nv04_timer_new },
	.disp     = { 0x00000001, nv04_disp_new },
	.dma      = { 0x00000001, nv04_dma_new },
	.fifo     = { 0x00000001, nv10_fifo_new },
	.gr       = { 0x00000001, nv15_gr_new },
	.sw       = { 0x00000001, nv10_sw_new },
>>>>>>> upstream/android-13
};

static const struct nvkm_device_chip
nv1f_chipset = {
	.name = "nForce2",
<<<<<<< HEAD
	.bios = nvkm_bios_new,
	.bus = nv04_bus_new,
	.clk = nv04_clk_new,
	.devinit = nv1a_devinit_new,
	.fb = nv1a_fb_new,
	.gpio = nv10_gpio_new,
	.i2c = nv04_i2c_new,
	.imem = nv04_instmem_new,
	.mc = nv17_mc_new,
	.mmu = nv04_mmu_new,
	.pci = nv04_pci_new,
	.timer = nv04_timer_new,
	.disp = nv04_disp_new,
	.dma = nv04_dma_new,
	.fifo = nv17_fifo_new,
	.gr = nv17_gr_new,
	.sw = nv10_sw_new,
=======
	.bios     = { 0x00000001, nvkm_bios_new },
	.bus      = { 0x00000001, nv04_bus_new },
	.clk      = { 0x00000001, nv04_clk_new },
	.devinit  = { 0x00000001, nv1a_devinit_new },
	.fb       = { 0x00000001, nv1a_fb_new },
	.gpio     = { 0x00000001, nv10_gpio_new },
	.i2c      = { 0x00000001, nv04_i2c_new },
	.imem     = { 0x00000001, nv04_instmem_new },
	.mc       = { 0x00000001, nv17_mc_new },
	.mmu      = { 0x00000001, nv04_mmu_new },
	.pci      = { 0x00000001, nv04_pci_new },
	.timer    = { 0x00000001, nv04_timer_new },
	.disp     = { 0x00000001, nv04_disp_new },
	.dma      = { 0x00000001, nv04_dma_new },
	.fifo     = { 0x00000001, nv17_fifo_new },
	.gr       = { 0x00000001, nv17_gr_new },
	.sw       = { 0x00000001, nv10_sw_new },
>>>>>>> upstream/android-13
};

static const struct nvkm_device_chip
nv20_chipset = {
	.name = "NV20",
<<<<<<< HEAD
	.bios = nvkm_bios_new,
	.bus = nv04_bus_new,
	.clk = nv04_clk_new,
	.devinit = nv20_devinit_new,
	.fb = nv20_fb_new,
	.gpio = nv10_gpio_new,
	.i2c = nv04_i2c_new,
	.imem = nv04_instmem_new,
	.mc = nv17_mc_new,
	.mmu = nv04_mmu_new,
	.pci = nv04_pci_new,
	.timer = nv04_timer_new,
	.disp = nv04_disp_new,
	.dma = nv04_dma_new,
	.fifo = nv17_fifo_new,
	.gr = nv20_gr_new,
	.sw = nv10_sw_new,
=======
	.bios     = { 0x00000001, nvkm_bios_new },
	.bus      = { 0x00000001, nv04_bus_new },
	.clk      = { 0x00000001, nv04_clk_new },
	.devinit  = { 0x00000001, nv20_devinit_new },
	.fb       = { 0x00000001, nv20_fb_new },
	.gpio     = { 0x00000001, nv10_gpio_new },
	.i2c      = { 0x00000001, nv04_i2c_new },
	.imem     = { 0x00000001, nv04_instmem_new },
	.mc       = { 0x00000001, nv17_mc_new },
	.mmu      = { 0x00000001, nv04_mmu_new },
	.pci      = { 0x00000001, nv04_pci_new },
	.timer    = { 0x00000001, nv04_timer_new },
	.disp     = { 0x00000001, nv04_disp_new },
	.dma      = { 0x00000001, nv04_dma_new },
	.fifo     = { 0x00000001, nv17_fifo_new },
	.gr       = { 0x00000001, nv20_gr_new },
	.sw       = { 0x00000001, nv10_sw_new },
>>>>>>> upstream/android-13
};

static const struct nvkm_device_chip
nv25_chipset = {
	.name = "NV25",
<<<<<<< HEAD
	.bios = nvkm_bios_new,
	.bus = nv04_bus_new,
	.clk = nv04_clk_new,
	.devinit = nv20_devinit_new,
	.fb = nv25_fb_new,
	.gpio = nv10_gpio_new,
	.i2c = nv04_i2c_new,
	.imem = nv04_instmem_new,
	.mc = nv17_mc_new,
	.mmu = nv04_mmu_new,
	.pci = nv04_pci_new,
	.timer = nv04_timer_new,
	.disp = nv04_disp_new,
	.dma = nv04_dma_new,
	.fifo = nv17_fifo_new,
	.gr = nv25_gr_new,
	.sw = nv10_sw_new,
=======
	.bios     = { 0x00000001, nvkm_bios_new },
	.bus      = { 0x00000001, nv04_bus_new },
	.clk      = { 0x00000001, nv04_clk_new },
	.devinit  = { 0x00000001, nv20_devinit_new },
	.fb       = { 0x00000001, nv25_fb_new },
	.gpio     = { 0x00000001, nv10_gpio_new },
	.i2c      = { 0x00000001, nv04_i2c_new },
	.imem     = { 0x00000001, nv04_instmem_new },
	.mc       = { 0x00000001, nv17_mc_new },
	.mmu      = { 0x00000001, nv04_mmu_new },
	.pci      = { 0x00000001, nv04_pci_new },
	.timer    = { 0x00000001, nv04_timer_new },
	.disp     = { 0x00000001, nv04_disp_new },
	.dma      = { 0x00000001, nv04_dma_new },
	.fifo     = { 0x00000001, nv17_fifo_new },
	.gr       = { 0x00000001, nv25_gr_new },
	.sw       = { 0x00000001, nv10_sw_new },
>>>>>>> upstream/android-13
};

static const struct nvkm_device_chip
nv28_chipset = {
	.name = "NV28",
<<<<<<< HEAD
	.bios = nvkm_bios_new,
	.bus = nv04_bus_new,
	.clk = nv04_clk_new,
	.devinit = nv20_devinit_new,
	.fb = nv25_fb_new,
	.gpio = nv10_gpio_new,
	.i2c = nv04_i2c_new,
	.imem = nv04_instmem_new,
	.mc = nv17_mc_new,
	.mmu = nv04_mmu_new,
	.pci = nv04_pci_new,
	.timer = nv04_timer_new,
	.disp = nv04_disp_new,
	.dma = nv04_dma_new,
	.fifo = nv17_fifo_new,
	.gr = nv25_gr_new,
	.sw = nv10_sw_new,
=======
	.bios     = { 0x00000001, nvkm_bios_new },
	.bus      = { 0x00000001, nv04_bus_new },
	.clk      = { 0x00000001, nv04_clk_new },
	.devinit  = { 0x00000001, nv20_devinit_new },
	.fb       = { 0x00000001, nv25_fb_new },
	.gpio     = { 0x00000001, nv10_gpio_new },
	.i2c      = { 0x00000001, nv04_i2c_new },
	.imem     = { 0x00000001, nv04_instmem_new },
	.mc       = { 0x00000001, nv17_mc_new },
	.mmu      = { 0x00000001, nv04_mmu_new },
	.pci      = { 0x00000001, nv04_pci_new },
	.timer    = { 0x00000001, nv04_timer_new },
	.disp     = { 0x00000001, nv04_disp_new },
	.dma      = { 0x00000001, nv04_dma_new },
	.fifo     = { 0x00000001, nv17_fifo_new },
	.gr       = { 0x00000001, nv25_gr_new },
	.sw       = { 0x00000001, nv10_sw_new },
>>>>>>> upstream/android-13
};

static const struct nvkm_device_chip
nv2a_chipset = {
	.name = "NV2A",
<<<<<<< HEAD
	.bios = nvkm_bios_new,
	.bus = nv04_bus_new,
	.clk = nv04_clk_new,
	.devinit = nv20_devinit_new,
	.fb = nv25_fb_new,
	.gpio = nv10_gpio_new,
	.i2c = nv04_i2c_new,
	.imem = nv04_instmem_new,
	.mc = nv17_mc_new,
	.mmu = nv04_mmu_new,
	.pci = nv04_pci_new,
	.timer = nv04_timer_new,
	.disp = nv04_disp_new,
	.dma = nv04_dma_new,
	.fifo = nv17_fifo_new,
	.gr = nv2a_gr_new,
	.sw = nv10_sw_new,
=======
	.bios     = { 0x00000001, nvkm_bios_new },
	.bus      = { 0x00000001, nv04_bus_new },
	.clk      = { 0x00000001, nv04_clk_new },
	.devinit  = { 0x00000001, nv20_devinit_new },
	.fb       = { 0x00000001, nv25_fb_new },
	.gpio     = { 0x00000001, nv10_gpio_new },
	.i2c      = { 0x00000001, nv04_i2c_new },
	.imem     = { 0x00000001, nv04_instmem_new },
	.mc       = { 0x00000001, nv17_mc_new },
	.mmu      = { 0x00000001, nv04_mmu_new },
	.pci      = { 0x00000001, nv04_pci_new },
	.timer    = { 0x00000001, nv04_timer_new },
	.disp     = { 0x00000001, nv04_disp_new },
	.dma      = { 0x00000001, nv04_dma_new },
	.fifo     = { 0x00000001, nv17_fifo_new },
	.gr       = { 0x00000001, nv2a_gr_new },
	.sw       = { 0x00000001, nv10_sw_new },
>>>>>>> upstream/android-13
};

static const struct nvkm_device_chip
nv30_chipset = {
	.name = "NV30",
<<<<<<< HEAD
	.bios = nvkm_bios_new,
	.bus = nv04_bus_new,
	.clk = nv04_clk_new,
	.devinit = nv20_devinit_new,
	.fb = nv30_fb_new,
	.gpio = nv10_gpio_new,
	.i2c = nv04_i2c_new,
	.imem = nv04_instmem_new,
	.mc = nv17_mc_new,
	.mmu = nv04_mmu_new,
	.pci = nv04_pci_new,
	.timer = nv04_timer_new,
	.disp = nv04_disp_new,
	.dma = nv04_dma_new,
	.fifo = nv17_fifo_new,
	.gr = nv30_gr_new,
	.sw = nv10_sw_new,
=======
	.bios     = { 0x00000001, nvkm_bios_new },
	.bus      = { 0x00000001, nv04_bus_new },
	.clk      = { 0x00000001, nv04_clk_new },
	.devinit  = { 0x00000001, nv20_devinit_new },
	.fb       = { 0x00000001, nv30_fb_new },
	.gpio     = { 0x00000001, nv10_gpio_new },
	.i2c      = { 0x00000001, nv04_i2c_new },
	.imem     = { 0x00000001, nv04_instmem_new },
	.mc       = { 0x00000001, nv17_mc_new },
	.mmu      = { 0x00000001, nv04_mmu_new },
	.pci      = { 0x00000001, nv04_pci_new },
	.timer    = { 0x00000001, nv04_timer_new },
	.disp     = { 0x00000001, nv04_disp_new },
	.dma      = { 0x00000001, nv04_dma_new },
	.fifo     = { 0x00000001, nv17_fifo_new },
	.gr       = { 0x00000001, nv30_gr_new },
	.sw       = { 0x00000001, nv10_sw_new },
>>>>>>> upstream/android-13
};

static const struct nvkm_device_chip
nv31_chipset = {
	.name = "NV31",
<<<<<<< HEAD
	.bios = nvkm_bios_new,
	.bus = nv31_bus_new,
	.clk = nv04_clk_new,
	.devinit = nv20_devinit_new,
	.fb = nv30_fb_new,
	.gpio = nv10_gpio_new,
	.i2c = nv04_i2c_new,
	.imem = nv04_instmem_new,
	.mc = nv17_mc_new,
	.mmu = nv04_mmu_new,
	.pci = nv04_pci_new,
	.timer = nv04_timer_new,
	.disp = nv04_disp_new,
	.dma = nv04_dma_new,
	.fifo = nv17_fifo_new,
	.gr = nv30_gr_new,
	.mpeg = nv31_mpeg_new,
	.sw = nv10_sw_new,
=======
	.bios     = { 0x00000001, nvkm_bios_new },
	.bus      = { 0x00000001, nv31_bus_new },
	.clk      = { 0x00000001, nv04_clk_new },
	.devinit  = { 0x00000001, nv20_devinit_new },
	.fb       = { 0x00000001, nv30_fb_new },
	.gpio     = { 0x00000001, nv10_gpio_new },
	.i2c      = { 0x00000001, nv04_i2c_new },
	.imem     = { 0x00000001, nv04_instmem_new },
	.mc       = { 0x00000001, nv17_mc_new },
	.mmu      = { 0x00000001, nv04_mmu_new },
	.pci      = { 0x00000001, nv04_pci_new },
	.timer    = { 0x00000001, nv04_timer_new },
	.disp     = { 0x00000001, nv04_disp_new },
	.dma      = { 0x00000001, nv04_dma_new },
	.fifo     = { 0x00000001, nv17_fifo_new },
	.gr       = { 0x00000001, nv30_gr_new },
	.mpeg     = { 0x00000001, nv31_mpeg_new },
	.sw       = { 0x00000001, nv10_sw_new },
>>>>>>> upstream/android-13
};

static const struct nvkm_device_chip
nv34_chipset = {
	.name = "NV34",
<<<<<<< HEAD
	.bios = nvkm_bios_new,
	.bus = nv31_bus_new,
	.clk = nv04_clk_new,
	.devinit = nv10_devinit_new,
	.fb = nv10_fb_new,
	.gpio = nv10_gpio_new,
	.i2c = nv04_i2c_new,
	.imem = nv04_instmem_new,
	.mc = nv17_mc_new,
	.mmu = nv04_mmu_new,
	.pci = nv04_pci_new,
	.timer = nv04_timer_new,
	.disp = nv04_disp_new,
	.dma = nv04_dma_new,
	.fifo = nv17_fifo_new,
	.gr = nv34_gr_new,
	.mpeg = nv31_mpeg_new,
	.sw = nv10_sw_new,
=======
	.bios     = { 0x00000001, nvkm_bios_new },
	.bus      = { 0x00000001, nv31_bus_new },
	.clk      = { 0x00000001, nv04_clk_new },
	.devinit  = { 0x00000001, nv10_devinit_new },
	.fb       = { 0x00000001, nv10_fb_new },
	.gpio     = { 0x00000001, nv10_gpio_new },
	.i2c      = { 0x00000001, nv04_i2c_new },
	.imem     = { 0x00000001, nv04_instmem_new },
	.mc       = { 0x00000001, nv17_mc_new },
	.mmu      = { 0x00000001, nv04_mmu_new },
	.pci      = { 0x00000001, nv04_pci_new },
	.timer    = { 0x00000001, nv04_timer_new },
	.disp     = { 0x00000001, nv04_disp_new },
	.dma      = { 0x00000001, nv04_dma_new },
	.fifo     = { 0x00000001, nv17_fifo_new },
	.gr       = { 0x00000001, nv34_gr_new },
	.mpeg     = { 0x00000001, nv31_mpeg_new },
	.sw       = { 0x00000001, nv10_sw_new },
>>>>>>> upstream/android-13
};

static const struct nvkm_device_chip
nv35_chipset = {
	.name = "NV35",
<<<<<<< HEAD
	.bios = nvkm_bios_new,
	.bus = nv04_bus_new,
	.clk = nv04_clk_new,
	.devinit = nv20_devinit_new,
	.fb = nv35_fb_new,
	.gpio = nv10_gpio_new,
	.i2c = nv04_i2c_new,
	.imem = nv04_instmem_new,
	.mc = nv17_mc_new,
	.mmu = nv04_mmu_new,
	.pci = nv04_pci_new,
	.timer = nv04_timer_new,
	.disp = nv04_disp_new,
	.dma = nv04_dma_new,
	.fifo = nv17_fifo_new,
	.gr = nv35_gr_new,
	.sw = nv10_sw_new,
=======
	.bios     = { 0x00000001, nvkm_bios_new },
	.bus      = { 0x00000001, nv04_bus_new },
	.clk      = { 0x00000001, nv04_clk_new },
	.devinit  = { 0x00000001, nv20_devinit_new },
	.fb       = { 0x00000001, nv35_fb_new },
	.gpio     = { 0x00000001, nv10_gpio_new },
	.i2c      = { 0x00000001, nv04_i2c_new },
	.imem     = { 0x00000001, nv04_instmem_new },
	.mc       = { 0x00000001, nv17_mc_new },
	.mmu      = { 0x00000001, nv04_mmu_new },
	.pci      = { 0x00000001, nv04_pci_new },
	.timer    = { 0x00000001, nv04_timer_new },
	.disp     = { 0x00000001, nv04_disp_new },
	.dma      = { 0x00000001, nv04_dma_new },
	.fifo     = { 0x00000001, nv17_fifo_new },
	.gr       = { 0x00000001, nv35_gr_new },
	.sw       = { 0x00000001, nv10_sw_new },
>>>>>>> upstream/android-13
};

static const struct nvkm_device_chip
nv36_chipset = {
	.name = "NV36",
<<<<<<< HEAD
	.bios = nvkm_bios_new,
	.bus = nv31_bus_new,
	.clk = nv04_clk_new,
	.devinit = nv20_devinit_new,
	.fb = nv36_fb_new,
	.gpio = nv10_gpio_new,
	.i2c = nv04_i2c_new,
	.imem = nv04_instmem_new,
	.mc = nv17_mc_new,
	.mmu = nv04_mmu_new,
	.pci = nv04_pci_new,
	.timer = nv04_timer_new,
	.disp = nv04_disp_new,
	.dma = nv04_dma_new,
	.fifo = nv17_fifo_new,
	.gr = nv35_gr_new,
	.mpeg = nv31_mpeg_new,
	.sw = nv10_sw_new,
=======
	.bios     = { 0x00000001, nvkm_bios_new },
	.bus      = { 0x00000001, nv31_bus_new },
	.clk      = { 0x00000001, nv04_clk_new },
	.devinit  = { 0x00000001, nv20_devinit_new },
	.fb       = { 0x00000001, nv36_fb_new },
	.gpio     = { 0x00000001, nv10_gpio_new },
	.i2c      = { 0x00000001, nv04_i2c_new },
	.imem     = { 0x00000001, nv04_instmem_new },
	.mc       = { 0x00000001, nv17_mc_new },
	.mmu      = { 0x00000001, nv04_mmu_new },
	.pci      = { 0x00000001, nv04_pci_new },
	.timer    = { 0x00000001, nv04_timer_new },
	.disp     = { 0x00000001, nv04_disp_new },
	.dma      = { 0x00000001, nv04_dma_new },
	.fifo     = { 0x00000001, nv17_fifo_new },
	.gr       = { 0x00000001, nv35_gr_new },
	.mpeg     = { 0x00000001, nv31_mpeg_new },
	.sw       = { 0x00000001, nv10_sw_new },
>>>>>>> upstream/android-13
};

static const struct nvkm_device_chip
nv40_chipset = {
	.name = "NV40",
<<<<<<< HEAD
	.bios = nvkm_bios_new,
	.bus = nv31_bus_new,
	.clk = nv40_clk_new,
	.devinit = nv1a_devinit_new,
	.fb = nv40_fb_new,
	.gpio = nv10_gpio_new,
	.i2c = nv04_i2c_new,
	.imem = nv40_instmem_new,
	.mc = nv17_mc_new,
	.mmu = nv04_mmu_new,
	.pci = nv40_pci_new,
	.therm = nv40_therm_new,
	.timer = nv40_timer_new,
	.volt = nv40_volt_new,
	.disp = nv04_disp_new,
	.dma = nv04_dma_new,
	.fifo = nv40_fifo_new,
	.gr = nv40_gr_new,
	.mpeg = nv40_mpeg_new,
	.pm = nv40_pm_new,
	.sw = nv10_sw_new,
=======
	.bios     = { 0x00000001, nvkm_bios_new },
	.bus      = { 0x00000001, nv31_bus_new },
	.clk      = { 0x00000001, nv40_clk_new },
	.devinit  = { 0x00000001, nv1a_devinit_new },
	.fb       = { 0x00000001, nv40_fb_new },
	.gpio     = { 0x00000001, nv10_gpio_new },
	.i2c      = { 0x00000001, nv04_i2c_new },
	.imem     = { 0x00000001, nv40_instmem_new },
	.mc       = { 0x00000001, nv17_mc_new },
	.mmu      = { 0x00000001, nv04_mmu_new },
	.pci      = { 0x00000001, nv40_pci_new },
	.therm    = { 0x00000001, nv40_therm_new },
	.timer    = { 0x00000001, nv40_timer_new },
	.volt     = { 0x00000001, nv40_volt_new },
	.disp     = { 0x00000001, nv04_disp_new },
	.dma      = { 0x00000001, nv04_dma_new },
	.fifo     = { 0x00000001, nv40_fifo_new },
	.gr       = { 0x00000001, nv40_gr_new },
	.mpeg     = { 0x00000001, nv40_mpeg_new },
	.pm       = { 0x00000001, nv40_pm_new },
	.sw       = { 0x00000001, nv10_sw_new },
>>>>>>> upstream/android-13
};

static const struct nvkm_device_chip
nv41_chipset = {
	.name = "NV41",
<<<<<<< HEAD
	.bios = nvkm_bios_new,
	.bus = nv31_bus_new,
	.clk = nv40_clk_new,
	.devinit = nv1a_devinit_new,
	.fb = nv41_fb_new,
	.gpio = nv10_gpio_new,
	.i2c = nv04_i2c_new,
	.imem = nv40_instmem_new,
	.mc = nv17_mc_new,
	.mmu = nv41_mmu_new,
	.pci = nv40_pci_new,
	.therm = nv40_therm_new,
	.timer = nv41_timer_new,
	.volt = nv40_volt_new,
	.disp = nv04_disp_new,
	.dma = nv04_dma_new,
	.fifo = nv40_fifo_new,
	.gr = nv40_gr_new,
	.mpeg = nv40_mpeg_new,
	.pm = nv40_pm_new,
	.sw = nv10_sw_new,
=======
	.bios     = { 0x00000001, nvkm_bios_new },
	.bus      = { 0x00000001, nv31_bus_new },
	.clk      = { 0x00000001, nv40_clk_new },
	.devinit  = { 0x00000001, nv1a_devinit_new },
	.fb       = { 0x00000001, nv41_fb_new },
	.gpio     = { 0x00000001, nv10_gpio_new },
	.i2c      = { 0x00000001, nv04_i2c_new },
	.imem     = { 0x00000001, nv40_instmem_new },
	.mc       = { 0x00000001, nv17_mc_new },
	.mmu      = { 0x00000001, nv41_mmu_new },
	.pci      = { 0x00000001, nv40_pci_new },
	.therm    = { 0x00000001, nv40_therm_new },
	.timer    = { 0x00000001, nv41_timer_new },
	.volt     = { 0x00000001, nv40_volt_new },
	.disp     = { 0x00000001, nv04_disp_new },
	.dma      = { 0x00000001, nv04_dma_new },
	.fifo     = { 0x00000001, nv40_fifo_new },
	.gr       = { 0x00000001, nv40_gr_new },
	.mpeg     = { 0x00000001, nv40_mpeg_new },
	.pm       = { 0x00000001, nv40_pm_new },
	.sw       = { 0x00000001, nv10_sw_new },
>>>>>>> upstream/android-13
};

static const struct nvkm_device_chip
nv42_chipset = {
	.name = "NV42",
<<<<<<< HEAD
	.bios = nvkm_bios_new,
	.bus = nv31_bus_new,
	.clk = nv40_clk_new,
	.devinit = nv1a_devinit_new,
	.fb = nv41_fb_new,
	.gpio = nv10_gpio_new,
	.i2c = nv04_i2c_new,
	.imem = nv40_instmem_new,
	.mc = nv17_mc_new,
	.mmu = nv41_mmu_new,
	.pci = nv40_pci_new,
	.therm = nv40_therm_new,
	.timer = nv41_timer_new,
	.volt = nv40_volt_new,
	.disp = nv04_disp_new,
	.dma = nv04_dma_new,
	.fifo = nv40_fifo_new,
	.gr = nv40_gr_new,
	.mpeg = nv40_mpeg_new,
	.pm = nv40_pm_new,
	.sw = nv10_sw_new,
=======
	.bios     = { 0x00000001, nvkm_bios_new },
	.bus      = { 0x00000001, nv31_bus_new },
	.clk      = { 0x00000001, nv40_clk_new },
	.devinit  = { 0x00000001, nv1a_devinit_new },
	.fb       = { 0x00000001, nv41_fb_new },
	.gpio     = { 0x00000001, nv10_gpio_new },
	.i2c      = { 0x00000001, nv04_i2c_new },
	.imem     = { 0x00000001, nv40_instmem_new },
	.mc       = { 0x00000001, nv17_mc_new },
	.mmu      = { 0x00000001, nv41_mmu_new },
	.pci      = { 0x00000001, nv40_pci_new },
	.therm    = { 0x00000001, nv40_therm_new },
	.timer    = { 0x00000001, nv41_timer_new },
	.volt     = { 0x00000001, nv40_volt_new },
	.disp     = { 0x00000001, nv04_disp_new },
	.dma      = { 0x00000001, nv04_dma_new },
	.fifo     = { 0x00000001, nv40_fifo_new },
	.gr       = { 0x00000001, nv40_gr_new },
	.mpeg     = { 0x00000001, nv40_mpeg_new },
	.pm       = { 0x00000001, nv40_pm_new },
	.sw       = { 0x00000001, nv10_sw_new },
>>>>>>> upstream/android-13
};

static const struct nvkm_device_chip
nv43_chipset = {
	.name = "NV43",
<<<<<<< HEAD
	.bios = nvkm_bios_new,
	.bus = nv31_bus_new,
	.clk = nv40_clk_new,
	.devinit = nv1a_devinit_new,
	.fb = nv41_fb_new,
	.gpio = nv10_gpio_new,
	.i2c = nv04_i2c_new,
	.imem = nv40_instmem_new,
	.mc = nv17_mc_new,
	.mmu = nv41_mmu_new,
	.pci = nv40_pci_new,
	.therm = nv40_therm_new,
	.timer = nv41_timer_new,
	.volt = nv40_volt_new,
	.disp = nv04_disp_new,
	.dma = nv04_dma_new,
	.fifo = nv40_fifo_new,
	.gr = nv40_gr_new,
	.mpeg = nv40_mpeg_new,
	.pm = nv40_pm_new,
	.sw = nv10_sw_new,
=======
	.bios     = { 0x00000001, nvkm_bios_new },
	.bus      = { 0x00000001, nv31_bus_new },
	.clk      = { 0x00000001, nv40_clk_new },
	.devinit  = { 0x00000001, nv1a_devinit_new },
	.fb       = { 0x00000001, nv41_fb_new },
	.gpio     = { 0x00000001, nv10_gpio_new },
	.i2c      = { 0x00000001, nv04_i2c_new },
	.imem     = { 0x00000001, nv40_instmem_new },
	.mc       = { 0x00000001, nv17_mc_new },
	.mmu      = { 0x00000001, nv41_mmu_new },
	.pci      = { 0x00000001, nv40_pci_new },
	.therm    = { 0x00000001, nv40_therm_new },
	.timer    = { 0x00000001, nv41_timer_new },
	.volt     = { 0x00000001, nv40_volt_new },
	.disp     = { 0x00000001, nv04_disp_new },
	.dma      = { 0x00000001, nv04_dma_new },
	.fifo     = { 0x00000001, nv40_fifo_new },
	.gr       = { 0x00000001, nv40_gr_new },
	.mpeg     = { 0x00000001, nv40_mpeg_new },
	.pm       = { 0x00000001, nv40_pm_new },
	.sw       = { 0x00000001, nv10_sw_new },
>>>>>>> upstream/android-13
};

static const struct nvkm_device_chip
nv44_chipset = {
	.name = "NV44",
<<<<<<< HEAD
	.bios = nvkm_bios_new,
	.bus = nv31_bus_new,
	.clk = nv40_clk_new,
	.devinit = nv1a_devinit_new,
	.fb = nv44_fb_new,
	.gpio = nv10_gpio_new,
	.i2c = nv04_i2c_new,
	.imem = nv40_instmem_new,
	.mc = nv44_mc_new,
	.mmu = nv44_mmu_new,
	.pci = nv40_pci_new,
	.therm = nv40_therm_new,
	.timer = nv41_timer_new,
	.volt = nv40_volt_new,
	.disp = nv04_disp_new,
	.dma = nv04_dma_new,
	.fifo = nv40_fifo_new,
	.gr = nv44_gr_new,
	.mpeg = nv44_mpeg_new,
	.pm = nv40_pm_new,
	.sw = nv10_sw_new,
=======
	.bios     = { 0x00000001, nvkm_bios_new },
	.bus      = { 0x00000001, nv31_bus_new },
	.clk      = { 0x00000001, nv40_clk_new },
	.devinit  = { 0x00000001, nv1a_devinit_new },
	.fb       = { 0x00000001, nv44_fb_new },
	.gpio     = { 0x00000001, nv10_gpio_new },
	.i2c      = { 0x00000001, nv04_i2c_new },
	.imem     = { 0x00000001, nv40_instmem_new },
	.mc       = { 0x00000001, nv44_mc_new },
	.mmu      = { 0x00000001, nv44_mmu_new },
	.pci      = { 0x00000001, nv40_pci_new },
	.therm    = { 0x00000001, nv40_therm_new },
	.timer    = { 0x00000001, nv41_timer_new },
	.volt     = { 0x00000001, nv40_volt_new },
	.disp     = { 0x00000001, nv04_disp_new },
	.dma      = { 0x00000001, nv04_dma_new },
	.fifo     = { 0x00000001, nv40_fifo_new },
	.gr       = { 0x00000001, nv44_gr_new },
	.mpeg     = { 0x00000001, nv44_mpeg_new },
	.pm       = { 0x00000001, nv40_pm_new },
	.sw       = { 0x00000001, nv10_sw_new },
>>>>>>> upstream/android-13
};

static const struct nvkm_device_chip
nv45_chipset = {
	.name = "NV45",
<<<<<<< HEAD
	.bios = nvkm_bios_new,
	.bus = nv31_bus_new,
	.clk = nv40_clk_new,
	.devinit = nv1a_devinit_new,
	.fb = nv40_fb_new,
	.gpio = nv10_gpio_new,
	.i2c = nv04_i2c_new,
	.imem = nv40_instmem_new,
	.mc = nv17_mc_new,
	.mmu = nv04_mmu_new,
	.pci = nv40_pci_new,
	.therm = nv40_therm_new,
	.timer = nv41_timer_new,
	.volt = nv40_volt_new,
	.disp = nv04_disp_new,
	.dma = nv04_dma_new,
	.fifo = nv40_fifo_new,
	.gr = nv40_gr_new,
	.mpeg = nv44_mpeg_new,
	.pm = nv40_pm_new,
	.sw = nv10_sw_new,
=======
	.bios     = { 0x00000001, nvkm_bios_new },
	.bus      = { 0x00000001, nv31_bus_new },
	.clk      = { 0x00000001, nv40_clk_new },
	.devinit  = { 0x00000001, nv1a_devinit_new },
	.fb       = { 0x00000001, nv40_fb_new },
	.gpio     = { 0x00000001, nv10_gpio_new },
	.i2c      = { 0x00000001, nv04_i2c_new },
	.imem     = { 0x00000001, nv40_instmem_new },
	.mc       = { 0x00000001, nv17_mc_new },
	.mmu      = { 0x00000001, nv04_mmu_new },
	.pci      = { 0x00000001, nv40_pci_new },
	.therm    = { 0x00000001, nv40_therm_new },
	.timer    = { 0x00000001, nv41_timer_new },
	.volt     = { 0x00000001, nv40_volt_new },
	.disp     = { 0x00000001, nv04_disp_new },
	.dma      = { 0x00000001, nv04_dma_new },
	.fifo     = { 0x00000001, nv40_fifo_new },
	.gr       = { 0x00000001, nv40_gr_new },
	.mpeg     = { 0x00000001, nv44_mpeg_new },
	.pm       = { 0x00000001, nv40_pm_new },
	.sw       = { 0x00000001, nv10_sw_new },
>>>>>>> upstream/android-13
};

static const struct nvkm_device_chip
nv46_chipset = {
	.name = "G72",
<<<<<<< HEAD
	.bios = nvkm_bios_new,
	.bus = nv31_bus_new,
	.clk = nv40_clk_new,
	.devinit = nv1a_devinit_new,
	.fb = nv46_fb_new,
	.gpio = nv10_gpio_new,
	.i2c = nv04_i2c_new,
	.imem = nv40_instmem_new,
	.mc = nv44_mc_new,
	.mmu = nv44_mmu_new,
	.pci = nv46_pci_new,
	.therm = nv40_therm_new,
	.timer = nv41_timer_new,
	.volt = nv40_volt_new,
	.disp = nv04_disp_new,
	.dma = nv04_dma_new,
	.fifo = nv40_fifo_new,
	.gr = nv44_gr_new,
	.mpeg = nv44_mpeg_new,
	.pm = nv40_pm_new,
	.sw = nv10_sw_new,
=======
	.bios     = { 0x00000001, nvkm_bios_new },
	.bus      = { 0x00000001, nv31_bus_new },
	.clk      = { 0x00000001, nv40_clk_new },
	.devinit  = { 0x00000001, nv1a_devinit_new },
	.fb       = { 0x00000001, nv46_fb_new },
	.gpio     = { 0x00000001, nv10_gpio_new },
	.i2c      = { 0x00000001, nv04_i2c_new },
	.imem     = { 0x00000001, nv40_instmem_new },
	.mc       = { 0x00000001, nv44_mc_new },
	.mmu      = { 0x00000001, nv44_mmu_new },
	.pci      = { 0x00000001, nv46_pci_new },
	.therm    = { 0x00000001, nv40_therm_new },
	.timer    = { 0x00000001, nv41_timer_new },
	.volt     = { 0x00000001, nv40_volt_new },
	.disp     = { 0x00000001, nv04_disp_new },
	.dma      = { 0x00000001, nv04_dma_new },
	.fifo     = { 0x00000001, nv40_fifo_new },
	.gr       = { 0x00000001, nv44_gr_new },
	.mpeg     = { 0x00000001, nv44_mpeg_new },
	.pm       = { 0x00000001, nv40_pm_new },
	.sw       = { 0x00000001, nv10_sw_new },
>>>>>>> upstream/android-13
};

static const struct nvkm_device_chip
nv47_chipset = {
	.name = "G70",
<<<<<<< HEAD
	.bios = nvkm_bios_new,
	.bus = nv31_bus_new,
	.clk = nv40_clk_new,
	.devinit = nv1a_devinit_new,
	.fb = nv47_fb_new,
	.gpio = nv10_gpio_new,
	.i2c = nv04_i2c_new,
	.imem = nv40_instmem_new,
	.mc = nv17_mc_new,
	.mmu = nv41_mmu_new,
	.pci = nv40_pci_new,
	.therm = nv40_therm_new,
	.timer = nv41_timer_new,
	.volt = nv40_volt_new,
	.disp = nv04_disp_new,
	.dma = nv04_dma_new,
	.fifo = nv40_fifo_new,
	.gr = nv40_gr_new,
	.mpeg = nv44_mpeg_new,
	.pm = nv40_pm_new,
	.sw = nv10_sw_new,
=======
	.bios     = { 0x00000001, nvkm_bios_new },
	.bus      = { 0x00000001, nv31_bus_new },
	.clk      = { 0x00000001, nv40_clk_new },
	.devinit  = { 0x00000001, nv1a_devinit_new },
	.fb       = { 0x00000001, nv47_fb_new },
	.gpio     = { 0x00000001, nv10_gpio_new },
	.i2c      = { 0x00000001, nv04_i2c_new },
	.imem     = { 0x00000001, nv40_instmem_new },
	.mc       = { 0x00000001, nv17_mc_new },
	.mmu      = { 0x00000001, nv41_mmu_new },
	.pci      = { 0x00000001, nv40_pci_new },
	.therm    = { 0x00000001, nv40_therm_new },
	.timer    = { 0x00000001, nv41_timer_new },
	.volt     = { 0x00000001, nv40_volt_new },
	.disp     = { 0x00000001, nv04_disp_new },
	.dma      = { 0x00000001, nv04_dma_new },
	.fifo     = { 0x00000001, nv40_fifo_new },
	.gr       = { 0x00000001, nv40_gr_new },
	.mpeg     = { 0x00000001, nv44_mpeg_new },
	.pm       = { 0x00000001, nv40_pm_new },
	.sw       = { 0x00000001, nv10_sw_new },
>>>>>>> upstream/android-13
};

static const struct nvkm_device_chip
nv49_chipset = {
	.name = "G71",
<<<<<<< HEAD
	.bios = nvkm_bios_new,
	.bus = nv31_bus_new,
	.clk = nv40_clk_new,
	.devinit = nv1a_devinit_new,
	.fb = nv49_fb_new,
	.gpio = nv10_gpio_new,
	.i2c = nv04_i2c_new,
	.imem = nv40_instmem_new,
	.mc = nv17_mc_new,
	.mmu = nv41_mmu_new,
	.pci = nv40_pci_new,
	.therm = nv40_therm_new,
	.timer = nv41_timer_new,
	.volt = nv40_volt_new,
	.disp = nv04_disp_new,
	.dma = nv04_dma_new,
	.fifo = nv40_fifo_new,
	.gr = nv40_gr_new,
	.mpeg = nv44_mpeg_new,
	.pm = nv40_pm_new,
	.sw = nv10_sw_new,
=======
	.bios     = { 0x00000001, nvkm_bios_new },
	.bus      = { 0x00000001, nv31_bus_new },
	.clk      = { 0x00000001, nv40_clk_new },
	.devinit  = { 0x00000001, nv1a_devinit_new },
	.fb       = { 0x00000001, nv49_fb_new },
	.gpio     = { 0x00000001, nv10_gpio_new },
	.i2c      = { 0x00000001, nv04_i2c_new },
	.imem     = { 0x00000001, nv40_instmem_new },
	.mc       = { 0x00000001, nv17_mc_new },
	.mmu      = { 0x00000001, nv41_mmu_new },
	.pci      = { 0x00000001, nv40_pci_new },
	.therm    = { 0x00000001, nv40_therm_new },
	.timer    = { 0x00000001, nv41_timer_new },
	.volt     = { 0x00000001, nv40_volt_new },
	.disp     = { 0x00000001, nv04_disp_new },
	.dma      = { 0x00000001, nv04_dma_new },
	.fifo     = { 0x00000001, nv40_fifo_new },
	.gr       = { 0x00000001, nv40_gr_new },
	.mpeg     = { 0x00000001, nv44_mpeg_new },
	.pm       = { 0x00000001, nv40_pm_new },
	.sw       = { 0x00000001, nv10_sw_new },
>>>>>>> upstream/android-13
};

static const struct nvkm_device_chip
nv4a_chipset = {
	.name = "NV44A",
<<<<<<< HEAD
	.bios = nvkm_bios_new,
	.bus = nv31_bus_new,
	.clk = nv40_clk_new,
	.devinit = nv1a_devinit_new,
	.fb = nv44_fb_new,
	.gpio = nv10_gpio_new,
	.i2c = nv04_i2c_new,
	.imem = nv40_instmem_new,
	.mc = nv44_mc_new,
	.mmu = nv04_mmu_new,
	.pci = nv40_pci_new,
	.therm = nv40_therm_new,
	.timer = nv41_timer_new,
	.volt = nv40_volt_new,
	.disp = nv04_disp_new,
	.dma = nv04_dma_new,
	.fifo = nv40_fifo_new,
	.gr = nv44_gr_new,
	.mpeg = nv44_mpeg_new,
	.pm = nv40_pm_new,
	.sw = nv10_sw_new,
=======
	.bios     = { 0x00000001, nvkm_bios_new },
	.bus      = { 0x00000001, nv31_bus_new },
	.clk      = { 0x00000001, nv40_clk_new },
	.devinit  = { 0x00000001, nv1a_devinit_new },
	.fb       = { 0x00000001, nv44_fb_new },
	.gpio     = { 0x00000001, nv10_gpio_new },
	.i2c      = { 0x00000001, nv04_i2c_new },
	.imem     = { 0x00000001, nv40_instmem_new },
	.mc       = { 0x00000001, nv44_mc_new },
	.mmu      = { 0x00000001, nv04_mmu_new },
	.pci      = { 0x00000001, nv40_pci_new },
	.therm    = { 0x00000001, nv40_therm_new },
	.timer    = { 0x00000001, nv41_timer_new },
	.volt     = { 0x00000001, nv40_volt_new },
	.disp     = { 0x00000001, nv04_disp_new },
	.dma      = { 0x00000001, nv04_dma_new },
	.fifo     = { 0x00000001, nv40_fifo_new },
	.gr       = { 0x00000001, nv44_gr_new },
	.mpeg     = { 0x00000001, nv44_mpeg_new },
	.pm       = { 0x00000001, nv40_pm_new },
	.sw       = { 0x00000001, nv10_sw_new },
>>>>>>> upstream/android-13
};

static const struct nvkm_device_chip
nv4b_chipset = {
	.name = "G73",
<<<<<<< HEAD
	.bios = nvkm_bios_new,
	.bus = nv31_bus_new,
	.clk = nv40_clk_new,
	.devinit = nv1a_devinit_new,
	.fb = nv49_fb_new,
	.gpio = nv10_gpio_new,
	.i2c = nv04_i2c_new,
	.imem = nv40_instmem_new,
	.mc = nv17_mc_new,
	.mmu = nv41_mmu_new,
	.pci = nv40_pci_new,
	.therm = nv40_therm_new,
	.timer = nv41_timer_new,
	.volt = nv40_volt_new,
	.disp = nv04_disp_new,
	.dma = nv04_dma_new,
	.fifo = nv40_fifo_new,
	.gr = nv40_gr_new,
	.mpeg = nv44_mpeg_new,
	.pm = nv40_pm_new,
	.sw = nv10_sw_new,
=======
	.bios     = { 0x00000001, nvkm_bios_new },
	.bus      = { 0x00000001, nv31_bus_new },
	.clk      = { 0x00000001, nv40_clk_new },
	.devinit  = { 0x00000001, nv1a_devinit_new },
	.fb       = { 0x00000001, nv49_fb_new },
	.gpio     = { 0x00000001, nv10_gpio_new },
	.i2c      = { 0x00000001, nv04_i2c_new },
	.imem     = { 0x00000001, nv40_instmem_new },
	.mc       = { 0x00000001, nv17_mc_new },
	.mmu      = { 0x00000001, nv41_mmu_new },
	.pci      = { 0x00000001, nv40_pci_new },
	.therm    = { 0x00000001, nv40_therm_new },
	.timer    = { 0x00000001, nv41_timer_new },
	.volt     = { 0x00000001, nv40_volt_new },
	.disp     = { 0x00000001, nv04_disp_new },
	.dma      = { 0x00000001, nv04_dma_new },
	.fifo     = { 0x00000001, nv40_fifo_new },
	.gr       = { 0x00000001, nv40_gr_new },
	.mpeg     = { 0x00000001, nv44_mpeg_new },
	.pm       = { 0x00000001, nv40_pm_new },
	.sw       = { 0x00000001, nv10_sw_new },
>>>>>>> upstream/android-13
};

static const struct nvkm_device_chip
nv4c_chipset = {
	.name = "C61",
<<<<<<< HEAD
	.bios = nvkm_bios_new,
	.bus = nv31_bus_new,
	.clk = nv40_clk_new,
	.devinit = nv1a_devinit_new,
	.fb = nv46_fb_new,
	.gpio = nv10_gpio_new,
	.i2c = nv04_i2c_new,
	.imem = nv40_instmem_new,
	.mc = nv44_mc_new,
	.mmu = nv44_mmu_new,
	.pci = nv4c_pci_new,
	.therm = nv40_therm_new,
	.timer = nv41_timer_new,
	.volt = nv40_volt_new,
	.disp = nv04_disp_new,
	.dma = nv04_dma_new,
	.fifo = nv40_fifo_new,
	.gr = nv44_gr_new,
	.mpeg = nv44_mpeg_new,
	.pm = nv40_pm_new,
	.sw = nv10_sw_new,
=======
	.bios     = { 0x00000001, nvkm_bios_new },
	.bus      = { 0x00000001, nv31_bus_new },
	.clk      = { 0x00000001, nv40_clk_new },
	.devinit  = { 0x00000001, nv1a_devinit_new },
	.fb       = { 0x00000001, nv46_fb_new },
	.gpio     = { 0x00000001, nv10_gpio_new },
	.i2c      = { 0x00000001, nv04_i2c_new },
	.imem     = { 0x00000001, nv40_instmem_new },
	.mc       = { 0x00000001, nv44_mc_new },
	.mmu      = { 0x00000001, nv44_mmu_new },
	.pci      = { 0x00000001, nv4c_pci_new },
	.therm    = { 0x00000001, nv40_therm_new },
	.timer    = { 0x00000001, nv41_timer_new },
	.volt     = { 0x00000001, nv40_volt_new },
	.disp     = { 0x00000001, nv04_disp_new },
	.dma      = { 0x00000001, nv04_dma_new },
	.fifo     = { 0x00000001, nv40_fifo_new },
	.gr       = { 0x00000001, nv44_gr_new },
	.mpeg     = { 0x00000001, nv44_mpeg_new },
	.pm       = { 0x00000001, nv40_pm_new },
	.sw       = { 0x00000001, nv10_sw_new },
>>>>>>> upstream/android-13
};

static const struct nvkm_device_chip
nv4e_chipset = {
	.name = "C51",
<<<<<<< HEAD
	.bios = nvkm_bios_new,
	.bus = nv31_bus_new,
	.clk = nv40_clk_new,
	.devinit = nv1a_devinit_new,
	.fb = nv4e_fb_new,
	.gpio = nv10_gpio_new,
	.i2c = nv4e_i2c_new,
	.imem = nv40_instmem_new,
	.mc = nv44_mc_new,
	.mmu = nv44_mmu_new,
	.pci = nv4c_pci_new,
	.therm = nv40_therm_new,
	.timer = nv41_timer_new,
	.volt = nv40_volt_new,
	.disp = nv04_disp_new,
	.dma = nv04_dma_new,
	.fifo = nv40_fifo_new,
	.gr = nv44_gr_new,
	.mpeg = nv44_mpeg_new,
	.pm = nv40_pm_new,
	.sw = nv10_sw_new,
=======
	.bios     = { 0x00000001, nvkm_bios_new },
	.bus      = { 0x00000001, nv31_bus_new },
	.clk      = { 0x00000001, nv40_clk_new },
	.devinit  = { 0x00000001, nv1a_devinit_new },
	.fb       = { 0x00000001, nv4e_fb_new },
	.gpio     = { 0x00000001, nv10_gpio_new },
	.i2c      = { 0x00000001, nv4e_i2c_new },
	.imem     = { 0x00000001, nv40_instmem_new },
	.mc       = { 0x00000001, nv44_mc_new },
	.mmu      = { 0x00000001, nv44_mmu_new },
	.pci      = { 0x00000001, nv4c_pci_new },
	.therm    = { 0x00000001, nv40_therm_new },
	.timer    = { 0x00000001, nv41_timer_new },
	.volt     = { 0x00000001, nv40_volt_new },
	.disp     = { 0x00000001, nv04_disp_new },
	.dma      = { 0x00000001, nv04_dma_new },
	.fifo     = { 0x00000001, nv40_fifo_new },
	.gr       = { 0x00000001, nv44_gr_new },
	.mpeg     = { 0x00000001, nv44_mpeg_new },
	.pm       = { 0x00000001, nv40_pm_new },
	.sw       = { 0x00000001, nv10_sw_new },
>>>>>>> upstream/android-13
};

static const struct nvkm_device_chip
nv50_chipset = {
	.name = "G80",
<<<<<<< HEAD
	.bar = nv50_bar_new,
	.bios = nvkm_bios_new,
	.bus = nv50_bus_new,
	.clk = nv50_clk_new,
	.devinit = nv50_devinit_new,
	.fb = nv50_fb_new,
	.fuse = nv50_fuse_new,
	.gpio = nv50_gpio_new,
	.i2c = nv50_i2c_new,
	.imem = nv50_instmem_new,
	.mc = nv50_mc_new,
	.mmu = nv50_mmu_new,
	.mxm = nv50_mxm_new,
	.pci = nv46_pci_new,
	.therm = nv50_therm_new,
	.timer = nv41_timer_new,
	.volt = nv40_volt_new,
	.disp = nv50_disp_new,
	.dma = nv50_dma_new,
	.fifo = nv50_fifo_new,
	.gr = nv50_gr_new,
	.mpeg = nv50_mpeg_new,
	.pm = nv50_pm_new,
	.sw = nv50_sw_new,
=======
	.bar      = { 0x00000001, nv50_bar_new },
	.bios     = { 0x00000001, nvkm_bios_new },
	.bus      = { 0x00000001, nv50_bus_new },
	.clk      = { 0x00000001, nv50_clk_new },
	.devinit  = { 0x00000001, nv50_devinit_new },
	.fb       = { 0x00000001, nv50_fb_new },
	.fuse     = { 0x00000001, nv50_fuse_new },
	.gpio     = { 0x00000001, nv50_gpio_new },
	.i2c      = { 0x00000001, nv50_i2c_new },
	.imem     = { 0x00000001, nv50_instmem_new },
	.mc       = { 0x00000001, nv50_mc_new },
	.mmu      = { 0x00000001, nv50_mmu_new },
	.mxm      = { 0x00000001, nv50_mxm_new },
	.pci      = { 0x00000001, nv46_pci_new },
	.therm    = { 0x00000001, nv50_therm_new },
	.timer    = { 0x00000001, nv41_timer_new },
	.volt     = { 0x00000001, nv40_volt_new },
	.disp     = { 0x00000001, nv50_disp_new },
	.dma      = { 0x00000001, nv50_dma_new },
	.fifo     = { 0x00000001, nv50_fifo_new },
	.gr       = { 0x00000001, nv50_gr_new },
	.mpeg     = { 0x00000001, nv50_mpeg_new },
	.pm       = { 0x00000001, nv50_pm_new },
	.sw       = { 0x00000001, nv50_sw_new },
>>>>>>> upstream/android-13
};

static const struct nvkm_device_chip
nv63_chipset = {
	.name = "C73",
<<<<<<< HEAD
	.bios = nvkm_bios_new,
	.bus = nv31_bus_new,
	.clk = nv40_clk_new,
	.devinit = nv1a_devinit_new,
	.fb = nv46_fb_new,
	.gpio = nv10_gpio_new,
	.i2c = nv04_i2c_new,
	.imem = nv40_instmem_new,
	.mc = nv44_mc_new,
	.mmu = nv44_mmu_new,
	.pci = nv4c_pci_new,
	.therm = nv40_therm_new,
	.timer = nv41_timer_new,
	.volt = nv40_volt_new,
	.disp = nv04_disp_new,
	.dma = nv04_dma_new,
	.fifo = nv40_fifo_new,
	.gr = nv44_gr_new,
	.mpeg = nv44_mpeg_new,
	.pm = nv40_pm_new,
	.sw = nv10_sw_new,
=======
	.bios     = { 0x00000001, nvkm_bios_new },
	.bus      = { 0x00000001, nv31_bus_new },
	.clk      = { 0x00000001, nv40_clk_new },
	.devinit  = { 0x00000001, nv1a_devinit_new },
	.fb       = { 0x00000001, nv46_fb_new },
	.gpio     = { 0x00000001, nv10_gpio_new },
	.i2c      = { 0x00000001, nv04_i2c_new },
	.imem     = { 0x00000001, nv40_instmem_new },
	.mc       = { 0x00000001, nv44_mc_new },
	.mmu      = { 0x00000001, nv44_mmu_new },
	.pci      = { 0x00000001, nv4c_pci_new },
	.therm    = { 0x00000001, nv40_therm_new },
	.timer    = { 0x00000001, nv41_timer_new },
	.volt     = { 0x00000001, nv40_volt_new },
	.disp     = { 0x00000001, nv04_disp_new },
	.dma      = { 0x00000001, nv04_dma_new },
	.fifo     = { 0x00000001, nv40_fifo_new },
	.gr       = { 0x00000001, nv44_gr_new },
	.mpeg     = { 0x00000001, nv44_mpeg_new },
	.pm       = { 0x00000001, nv40_pm_new },
	.sw       = { 0x00000001, nv10_sw_new },
>>>>>>> upstream/android-13
};

static const struct nvkm_device_chip
nv67_chipset = {
	.name = "C67",
<<<<<<< HEAD
	.bios = nvkm_bios_new,
	.bus = nv31_bus_new,
	.clk = nv40_clk_new,
	.devinit = nv1a_devinit_new,
	.fb = nv46_fb_new,
	.gpio = nv10_gpio_new,
	.i2c = nv04_i2c_new,
	.imem = nv40_instmem_new,
	.mc = nv44_mc_new,
	.mmu = nv44_mmu_new,
	.pci = nv4c_pci_new,
	.therm = nv40_therm_new,
	.timer = nv41_timer_new,
	.volt = nv40_volt_new,
	.disp = nv04_disp_new,
	.dma = nv04_dma_new,
	.fifo = nv40_fifo_new,
	.gr = nv44_gr_new,
	.mpeg = nv44_mpeg_new,
	.pm = nv40_pm_new,
	.sw = nv10_sw_new,
=======
	.bios     = { 0x00000001, nvkm_bios_new },
	.bus      = { 0x00000001, nv31_bus_new },
	.clk      = { 0x00000001, nv40_clk_new },
	.devinit  = { 0x00000001, nv1a_devinit_new },
	.fb       = { 0x00000001, nv46_fb_new },
	.gpio     = { 0x00000001, nv10_gpio_new },
	.i2c      = { 0x00000001, nv04_i2c_new },
	.imem     = { 0x00000001, nv40_instmem_new },
	.mc       = { 0x00000001, nv44_mc_new },
	.mmu      = { 0x00000001, nv44_mmu_new },
	.pci      = { 0x00000001, nv4c_pci_new },
	.therm    = { 0x00000001, nv40_therm_new },
	.timer    = { 0x00000001, nv41_timer_new },
	.volt     = { 0x00000001, nv40_volt_new },
	.disp     = { 0x00000001, nv04_disp_new },
	.dma      = { 0x00000001, nv04_dma_new },
	.fifo     = { 0x00000001, nv40_fifo_new },
	.gr       = { 0x00000001, nv44_gr_new },
	.mpeg     = { 0x00000001, nv44_mpeg_new },
	.pm       = { 0x00000001, nv40_pm_new },
	.sw       = { 0x00000001, nv10_sw_new },
>>>>>>> upstream/android-13
};

static const struct nvkm_device_chip
nv68_chipset = {
	.name = "C68",
<<<<<<< HEAD
	.bios = nvkm_bios_new,
	.bus = nv31_bus_new,
	.clk = nv40_clk_new,
	.devinit = nv1a_devinit_new,
	.fb = nv46_fb_new,
	.gpio = nv10_gpio_new,
	.i2c = nv04_i2c_new,
	.imem = nv40_instmem_new,
	.mc = nv44_mc_new,
	.mmu = nv44_mmu_new,
	.pci = nv4c_pci_new,
	.therm = nv40_therm_new,
	.timer = nv41_timer_new,
	.volt = nv40_volt_new,
	.disp = nv04_disp_new,
	.dma = nv04_dma_new,
	.fifo = nv40_fifo_new,
	.gr = nv44_gr_new,
	.mpeg = nv44_mpeg_new,
	.pm = nv40_pm_new,
	.sw = nv10_sw_new,
=======
	.bios     = { 0x00000001, nvkm_bios_new },
	.bus      = { 0x00000001, nv31_bus_new },
	.clk      = { 0x00000001, nv40_clk_new },
	.devinit  = { 0x00000001, nv1a_devinit_new },
	.fb       = { 0x00000001, nv46_fb_new },
	.gpio     = { 0x00000001, nv10_gpio_new },
	.i2c      = { 0x00000001, nv04_i2c_new },
	.imem     = { 0x00000001, nv40_instmem_new },
	.mc       = { 0x00000001, nv44_mc_new },
	.mmu      = { 0x00000001, nv44_mmu_new },
	.pci      = { 0x00000001, nv4c_pci_new },
	.therm    = { 0x00000001, nv40_therm_new },
	.timer    = { 0x00000001, nv41_timer_new },
	.volt     = { 0x00000001, nv40_volt_new },
	.disp     = { 0x00000001, nv04_disp_new },
	.dma      = { 0x00000001, nv04_dma_new },
	.fifo     = { 0x00000001, nv40_fifo_new },
	.gr       = { 0x00000001, nv44_gr_new },
	.mpeg     = { 0x00000001, nv44_mpeg_new },
	.pm       = { 0x00000001, nv40_pm_new },
	.sw       = { 0x00000001, nv10_sw_new },
>>>>>>> upstream/android-13
};

static const struct nvkm_device_chip
nv84_chipset = {
	.name = "G84",
<<<<<<< HEAD
	.bar = g84_bar_new,
	.bios = nvkm_bios_new,
	.bus = nv50_bus_new,
	.clk = g84_clk_new,
	.devinit = g84_devinit_new,
	.fb = g84_fb_new,
	.fuse = nv50_fuse_new,
	.gpio = nv50_gpio_new,
	.i2c = nv50_i2c_new,
	.imem = nv50_instmem_new,
	.mc = g84_mc_new,
	.mmu = g84_mmu_new,
	.mxm = nv50_mxm_new,
	.pci = g84_pci_new,
	.therm = g84_therm_new,
	.timer = nv41_timer_new,
	.volt = nv40_volt_new,
	.bsp = g84_bsp_new,
	.cipher = g84_cipher_new,
	.disp = g84_disp_new,
	.dma = nv50_dma_new,
	.fifo = g84_fifo_new,
	.gr = g84_gr_new,
	.mpeg = g84_mpeg_new,
	.pm = g84_pm_new,
	.sw = nv50_sw_new,
	.vp = g84_vp_new,
=======
	.bar      = { 0x00000001, g84_bar_new },
	.bios     = { 0x00000001, nvkm_bios_new },
	.bus      = { 0x00000001, nv50_bus_new },
	.clk      = { 0x00000001, g84_clk_new },
	.devinit  = { 0x00000001, g84_devinit_new },
	.fb       = { 0x00000001, g84_fb_new },
	.fuse     = { 0x00000001, nv50_fuse_new },
	.gpio     = { 0x00000001, nv50_gpio_new },
	.i2c      = { 0x00000001, nv50_i2c_new },
	.imem     = { 0x00000001, nv50_instmem_new },
	.mc       = { 0x00000001, g84_mc_new },
	.mmu      = { 0x00000001, g84_mmu_new },
	.mxm      = { 0x00000001, nv50_mxm_new },
	.pci      = { 0x00000001, g84_pci_new },
	.therm    = { 0x00000001, g84_therm_new },
	.timer    = { 0x00000001, nv41_timer_new },
	.volt     = { 0x00000001, nv40_volt_new },
	.bsp      = { 0x00000001, g84_bsp_new },
	.cipher   = { 0x00000001, g84_cipher_new },
	.disp     = { 0x00000001, g84_disp_new },
	.dma      = { 0x00000001, nv50_dma_new },
	.fifo     = { 0x00000001, g84_fifo_new },
	.gr       = { 0x00000001, g84_gr_new },
	.mpeg     = { 0x00000001, g84_mpeg_new },
	.pm       = { 0x00000001, g84_pm_new },
	.sw       = { 0x00000001, nv50_sw_new },
	.vp       = { 0x00000001, g84_vp_new },
>>>>>>> upstream/android-13
};

static const struct nvkm_device_chip
nv86_chipset = {
	.name = "G86",
<<<<<<< HEAD
	.bar = g84_bar_new,
	.bios = nvkm_bios_new,
	.bus = nv50_bus_new,
	.clk = g84_clk_new,
	.devinit = g84_devinit_new,
	.fb = g84_fb_new,
	.fuse = nv50_fuse_new,
	.gpio = nv50_gpio_new,
	.i2c = nv50_i2c_new,
	.imem = nv50_instmem_new,
	.mc = g84_mc_new,
	.mmu = g84_mmu_new,
	.mxm = nv50_mxm_new,
	.pci = g84_pci_new,
	.therm = g84_therm_new,
	.timer = nv41_timer_new,
	.volt = nv40_volt_new,
	.bsp = g84_bsp_new,
	.cipher = g84_cipher_new,
	.disp = g84_disp_new,
	.dma = nv50_dma_new,
	.fifo = g84_fifo_new,
	.gr = g84_gr_new,
	.mpeg = g84_mpeg_new,
	.pm = g84_pm_new,
	.sw = nv50_sw_new,
	.vp = g84_vp_new,
=======
	.bar      = { 0x00000001, g84_bar_new },
	.bios     = { 0x00000001, nvkm_bios_new },
	.bus      = { 0x00000001, nv50_bus_new },
	.clk      = { 0x00000001, g84_clk_new },
	.devinit  = { 0x00000001, g84_devinit_new },
	.fb       = { 0x00000001, g84_fb_new },
	.fuse     = { 0x00000001, nv50_fuse_new },
	.gpio     = { 0x00000001, nv50_gpio_new },
	.i2c      = { 0x00000001, nv50_i2c_new },
	.imem     = { 0x00000001, nv50_instmem_new },
	.mc       = { 0x00000001, g84_mc_new },
	.mmu      = { 0x00000001, g84_mmu_new },
	.mxm      = { 0x00000001, nv50_mxm_new },
	.pci      = { 0x00000001, g84_pci_new },
	.therm    = { 0x00000001, g84_therm_new },
	.timer    = { 0x00000001, nv41_timer_new },
	.volt     = { 0x00000001, nv40_volt_new },
	.bsp      = { 0x00000001, g84_bsp_new },
	.cipher   = { 0x00000001, g84_cipher_new },
	.disp     = { 0x00000001, g84_disp_new },
	.dma      = { 0x00000001, nv50_dma_new },
	.fifo     = { 0x00000001, g84_fifo_new },
	.gr       = { 0x00000001, g84_gr_new },
	.mpeg     = { 0x00000001, g84_mpeg_new },
	.pm       = { 0x00000001, g84_pm_new },
	.sw       = { 0x00000001, nv50_sw_new },
	.vp       = { 0x00000001, g84_vp_new },
>>>>>>> upstream/android-13
};

static const struct nvkm_device_chip
nv92_chipset = {
	.name = "G92",
<<<<<<< HEAD
	.bar = g84_bar_new,
	.bios = nvkm_bios_new,
	.bus = nv50_bus_new,
	.clk = g84_clk_new,
	.devinit = g84_devinit_new,
	.fb = g84_fb_new,
	.fuse = nv50_fuse_new,
	.gpio = nv50_gpio_new,
	.i2c = nv50_i2c_new,
	.imem = nv50_instmem_new,
	.mc = g84_mc_new,
	.mmu = g84_mmu_new,
	.mxm = nv50_mxm_new,
	.pci = g92_pci_new,
	.therm = g84_therm_new,
	.timer = nv41_timer_new,
	.volt = nv40_volt_new,
	.bsp = g84_bsp_new,
	.cipher = g84_cipher_new,
	.disp = g84_disp_new,
	.dma = nv50_dma_new,
	.fifo = g84_fifo_new,
	.gr = g84_gr_new,
	.mpeg = g84_mpeg_new,
	.pm = g84_pm_new,
	.sw = nv50_sw_new,
	.vp = g84_vp_new,
=======
	.bar      = { 0x00000001, g84_bar_new },
	.bios     = { 0x00000001, nvkm_bios_new },
	.bus      = { 0x00000001, nv50_bus_new },
	.clk      = { 0x00000001, g84_clk_new },
	.devinit  = { 0x00000001, g84_devinit_new },
	.fb       = { 0x00000001, g84_fb_new },
	.fuse     = { 0x00000001, nv50_fuse_new },
	.gpio     = { 0x00000001, nv50_gpio_new },
	.i2c      = { 0x00000001, nv50_i2c_new },
	.imem     = { 0x00000001, nv50_instmem_new },
	.mc       = { 0x00000001, g84_mc_new },
	.mmu      = { 0x00000001, g84_mmu_new },
	.mxm      = { 0x00000001, nv50_mxm_new },
	.pci      = { 0x00000001, g92_pci_new },
	.therm    = { 0x00000001, g84_therm_new },
	.timer    = { 0x00000001, nv41_timer_new },
	.volt     = { 0x00000001, nv40_volt_new },
	.bsp      = { 0x00000001, g84_bsp_new },
	.cipher   = { 0x00000001, g84_cipher_new },
	.disp     = { 0x00000001, g84_disp_new },
	.dma      = { 0x00000001, nv50_dma_new },
	.fifo     = { 0x00000001, g84_fifo_new },
	.gr       = { 0x00000001, g84_gr_new },
	.mpeg     = { 0x00000001, g84_mpeg_new },
	.pm       = { 0x00000001, g84_pm_new },
	.sw       = { 0x00000001, nv50_sw_new },
	.vp       = { 0x00000001, g84_vp_new },
>>>>>>> upstream/android-13
};

static const struct nvkm_device_chip
nv94_chipset = {
	.name = "G94",
<<<<<<< HEAD
	.bar = g84_bar_new,
	.bios = nvkm_bios_new,
	.bus = g94_bus_new,
	.clk = g84_clk_new,
	.devinit = g84_devinit_new,
	.fb = g84_fb_new,
	.fuse = nv50_fuse_new,
	.gpio = g94_gpio_new,
	.i2c = g94_i2c_new,
	.imem = nv50_instmem_new,
	.mc = g84_mc_new,
	.mmu = g84_mmu_new,
	.mxm = nv50_mxm_new,
	.pci = g94_pci_new,
	.therm = g84_therm_new,
	.timer = nv41_timer_new,
	.volt = nv40_volt_new,
	.bsp = g84_bsp_new,
	.cipher = g84_cipher_new,
	.disp = g94_disp_new,
	.dma = nv50_dma_new,
	.fifo = g84_fifo_new,
	.gr = g84_gr_new,
	.mpeg = g84_mpeg_new,
	.pm = g84_pm_new,
	.sw = nv50_sw_new,
	.vp = g84_vp_new,
=======
	.bar      = { 0x00000001, g84_bar_new },
	.bios     = { 0x00000001, nvkm_bios_new },
	.bus      = { 0x00000001, g94_bus_new },
	.clk      = { 0x00000001, g84_clk_new },
	.devinit  = { 0x00000001, g84_devinit_new },
	.fb       = { 0x00000001, g84_fb_new },
	.fuse     = { 0x00000001, nv50_fuse_new },
	.gpio     = { 0x00000001, g94_gpio_new },
	.i2c      = { 0x00000001, g94_i2c_new },
	.imem     = { 0x00000001, nv50_instmem_new },
	.mc       = { 0x00000001, g84_mc_new },
	.mmu      = { 0x00000001, g84_mmu_new },
	.mxm      = { 0x00000001, nv50_mxm_new },
	.pci      = { 0x00000001, g94_pci_new },
	.therm    = { 0x00000001, g84_therm_new },
	.timer    = { 0x00000001, nv41_timer_new },
	.volt     = { 0x00000001, nv40_volt_new },
	.bsp      = { 0x00000001, g84_bsp_new },
	.cipher   = { 0x00000001, g84_cipher_new },
	.disp     = { 0x00000001, g94_disp_new },
	.dma      = { 0x00000001, nv50_dma_new },
	.fifo     = { 0x00000001, g84_fifo_new },
	.gr       = { 0x00000001, g84_gr_new },
	.mpeg     = { 0x00000001, g84_mpeg_new },
	.pm       = { 0x00000001, g84_pm_new },
	.sw       = { 0x00000001, nv50_sw_new },
	.vp       = { 0x00000001, g84_vp_new },
>>>>>>> upstream/android-13
};

static const struct nvkm_device_chip
nv96_chipset = {
	.name = "G96",
<<<<<<< HEAD
	.bar = g84_bar_new,
	.bios = nvkm_bios_new,
	.bus = g94_bus_new,
	.clk = g84_clk_new,
	.devinit = g84_devinit_new,
	.fb = g84_fb_new,
	.fuse = nv50_fuse_new,
	.gpio = g94_gpio_new,
	.i2c = g94_i2c_new,
	.imem = nv50_instmem_new,
	.mc = g84_mc_new,
	.mmu = g84_mmu_new,
	.mxm = nv50_mxm_new,
	.pci = g94_pci_new,
	.therm = g84_therm_new,
	.timer = nv41_timer_new,
	.volt = nv40_volt_new,
	.bsp = g84_bsp_new,
	.cipher = g84_cipher_new,
	.disp = g94_disp_new,
	.dma = nv50_dma_new,
	.fifo = g84_fifo_new,
	.gr = g84_gr_new,
	.mpeg = g84_mpeg_new,
	.pm = g84_pm_new,
	.sw = nv50_sw_new,
	.vp = g84_vp_new,
=======
	.bar      = { 0x00000001, g84_bar_new },
	.bios     = { 0x00000001, nvkm_bios_new },
	.bus      = { 0x00000001, g94_bus_new },
	.clk      = { 0x00000001, g84_clk_new },
	.devinit  = { 0x00000001, g84_devinit_new },
	.fb       = { 0x00000001, g84_fb_new },
	.fuse     = { 0x00000001, nv50_fuse_new },
	.gpio     = { 0x00000001, g94_gpio_new },
	.i2c      = { 0x00000001, g94_i2c_new },
	.imem     = { 0x00000001, nv50_instmem_new },
	.mc       = { 0x00000001, g84_mc_new },
	.mmu      = { 0x00000001, g84_mmu_new },
	.mxm      = { 0x00000001, nv50_mxm_new },
	.pci      = { 0x00000001, g94_pci_new },
	.therm    = { 0x00000001, g84_therm_new },
	.timer    = { 0x00000001, nv41_timer_new },
	.volt     = { 0x00000001, nv40_volt_new },
	.bsp      = { 0x00000001, g84_bsp_new },
	.cipher   = { 0x00000001, g84_cipher_new },
	.disp     = { 0x00000001, g94_disp_new },
	.dma      = { 0x00000001, nv50_dma_new },
	.fifo     = { 0x00000001, g84_fifo_new },
	.gr       = { 0x00000001, g84_gr_new },
	.mpeg     = { 0x00000001, g84_mpeg_new },
	.pm       = { 0x00000001, g84_pm_new },
	.sw       = { 0x00000001, nv50_sw_new },
	.vp       = { 0x00000001, g84_vp_new },
>>>>>>> upstream/android-13
};

static const struct nvkm_device_chip
nv98_chipset = {
	.name = "G98",
<<<<<<< HEAD
	.bar = g84_bar_new,
	.bios = nvkm_bios_new,
	.bus = g94_bus_new,
	.clk = g84_clk_new,
	.devinit = g98_devinit_new,
	.fb = g84_fb_new,
	.fuse = nv50_fuse_new,
	.gpio = g94_gpio_new,
	.i2c = g94_i2c_new,
	.imem = nv50_instmem_new,
	.mc = g98_mc_new,
	.mmu = g84_mmu_new,
	.mxm = nv50_mxm_new,
	.pci = g94_pci_new,
	.therm = g84_therm_new,
	.timer = nv41_timer_new,
	.volt = nv40_volt_new,
	.disp = g94_disp_new,
	.dma = nv50_dma_new,
	.fifo = g84_fifo_new,
	.gr = g84_gr_new,
	.mspdec = g98_mspdec_new,
	.msppp = g98_msppp_new,
	.msvld = g98_msvld_new,
	.pm = g84_pm_new,
	.sec = g98_sec_new,
	.sw = nv50_sw_new,
=======
	.bar      = { 0x00000001, g84_bar_new },
	.bios     = { 0x00000001, nvkm_bios_new },
	.bus      = { 0x00000001, g94_bus_new },
	.clk      = { 0x00000001, g84_clk_new },
	.devinit  = { 0x00000001, g98_devinit_new },
	.fb       = { 0x00000001, g84_fb_new },
	.fuse     = { 0x00000001, nv50_fuse_new },
	.gpio     = { 0x00000001, g94_gpio_new },
	.i2c      = { 0x00000001, g94_i2c_new },
	.imem     = { 0x00000001, nv50_instmem_new },
	.mc       = { 0x00000001, g98_mc_new },
	.mmu      = { 0x00000001, g84_mmu_new },
	.mxm      = { 0x00000001, nv50_mxm_new },
	.pci      = { 0x00000001, g94_pci_new },
	.therm    = { 0x00000001, g84_therm_new },
	.timer    = { 0x00000001, nv41_timer_new },
	.volt     = { 0x00000001, nv40_volt_new },
	.disp     = { 0x00000001, g94_disp_new },
	.dma      = { 0x00000001, nv50_dma_new },
	.fifo     = { 0x00000001, g84_fifo_new },
	.gr       = { 0x00000001, g84_gr_new },
	.mspdec   = { 0x00000001, g98_mspdec_new },
	.msppp    = { 0x00000001, g98_msppp_new },
	.msvld    = { 0x00000001, g98_msvld_new },
	.pm       = { 0x00000001, g84_pm_new },
	.sec      = { 0x00000001, g98_sec_new },
	.sw       = { 0x00000001, nv50_sw_new },
>>>>>>> upstream/android-13
};

static const struct nvkm_device_chip
nva0_chipset = {
	.name = "GT200",
<<<<<<< HEAD
	.bar = g84_bar_new,
	.bios = nvkm_bios_new,
	.bus = g94_bus_new,
	.clk = g84_clk_new,
	.devinit = g84_devinit_new,
	.fb = g84_fb_new,
	.fuse = nv50_fuse_new,
	.gpio = g94_gpio_new,
	.i2c = nv50_i2c_new,
	.imem = nv50_instmem_new,
	.mc = g84_mc_new,
	.mmu = g84_mmu_new,
	.mxm = nv50_mxm_new,
	.pci = g94_pci_new,
	.therm = g84_therm_new,
	.timer = nv41_timer_new,
	.volt = nv40_volt_new,
	.bsp = g84_bsp_new,
	.cipher = g84_cipher_new,
	.disp = gt200_disp_new,
	.dma = nv50_dma_new,
	.fifo = g84_fifo_new,
	.gr = gt200_gr_new,
	.mpeg = g84_mpeg_new,
	.pm = gt200_pm_new,
	.sw = nv50_sw_new,
	.vp = g84_vp_new,
=======
	.bar      = { 0x00000001, g84_bar_new },
	.bios     = { 0x00000001, nvkm_bios_new },
	.bus      = { 0x00000001, g94_bus_new },
	.clk      = { 0x00000001, g84_clk_new },
	.devinit  = { 0x00000001, g84_devinit_new },
	.fb       = { 0x00000001, g84_fb_new },
	.fuse     = { 0x00000001, nv50_fuse_new },
	.gpio     = { 0x00000001, g94_gpio_new },
	.i2c      = { 0x00000001, nv50_i2c_new },
	.imem     = { 0x00000001, nv50_instmem_new },
	.mc       = { 0x00000001, g84_mc_new },
	.mmu      = { 0x00000001, g84_mmu_new },
	.mxm      = { 0x00000001, nv50_mxm_new },
	.pci      = { 0x00000001, g94_pci_new },
	.therm    = { 0x00000001, g84_therm_new },
	.timer    = { 0x00000001, nv41_timer_new },
	.volt     = { 0x00000001, nv40_volt_new },
	.bsp      = { 0x00000001, g84_bsp_new },
	.cipher   = { 0x00000001, g84_cipher_new },
	.disp     = { 0x00000001, gt200_disp_new },
	.dma      = { 0x00000001, nv50_dma_new },
	.fifo     = { 0x00000001, g84_fifo_new },
	.gr       = { 0x00000001, gt200_gr_new },
	.mpeg     = { 0x00000001, g84_mpeg_new },
	.pm       = { 0x00000001, gt200_pm_new },
	.sw       = { 0x00000001, nv50_sw_new },
	.vp       = { 0x00000001, g84_vp_new },
>>>>>>> upstream/android-13
};

static const struct nvkm_device_chip
nva3_chipset = {
	.name = "GT215",
<<<<<<< HEAD
	.bar = g84_bar_new,
	.bios = nvkm_bios_new,
	.bus = g94_bus_new,
	.clk = gt215_clk_new,
	.devinit = gt215_devinit_new,
	.fb = gt215_fb_new,
	.fuse = nv50_fuse_new,
	.gpio = g94_gpio_new,
	.i2c = g94_i2c_new,
	.imem = nv50_instmem_new,
	.mc = gt215_mc_new,
	.mmu = g84_mmu_new,
	.mxm = nv50_mxm_new,
	.pci = g94_pci_new,
	.pmu = gt215_pmu_new,
	.therm = gt215_therm_new,
	.timer = nv41_timer_new,
	.volt = nv40_volt_new,
	.ce[0] = gt215_ce_new,
	.disp = gt215_disp_new,
	.dma = nv50_dma_new,
	.fifo = g84_fifo_new,
	.gr = gt215_gr_new,
	.mpeg = g84_mpeg_new,
	.mspdec = gt215_mspdec_new,
	.msppp = gt215_msppp_new,
	.msvld = gt215_msvld_new,
	.pm = gt215_pm_new,
	.sw = nv50_sw_new,
=======
	.bar      = { 0x00000001, g84_bar_new },
	.bios     = { 0x00000001, nvkm_bios_new },
	.bus      = { 0x00000001, g94_bus_new },
	.clk      = { 0x00000001, gt215_clk_new },
	.devinit  = { 0x00000001, gt215_devinit_new },
	.fb       = { 0x00000001, gt215_fb_new },
	.fuse     = { 0x00000001, nv50_fuse_new },
	.gpio     = { 0x00000001, g94_gpio_new },
	.i2c      = { 0x00000001, g94_i2c_new },
	.imem     = { 0x00000001, nv50_instmem_new },
	.mc       = { 0x00000001, gt215_mc_new },
	.mmu      = { 0x00000001, g84_mmu_new },
	.mxm      = { 0x00000001, nv50_mxm_new },
	.pci      = { 0x00000001, g94_pci_new },
	.pmu      = { 0x00000001, gt215_pmu_new },
	.therm    = { 0x00000001, gt215_therm_new },
	.timer    = { 0x00000001, nv41_timer_new },
	.volt     = { 0x00000001, nv40_volt_new },
	.ce       = { 0x00000001, gt215_ce_new },
	.disp     = { 0x00000001, gt215_disp_new },
	.dma      = { 0x00000001, nv50_dma_new },
	.fifo     = { 0x00000001, g84_fifo_new },
	.gr       = { 0x00000001, gt215_gr_new },
	.mpeg     = { 0x00000001, g84_mpeg_new },
	.mspdec   = { 0x00000001, gt215_mspdec_new },
	.msppp    = { 0x00000001, gt215_msppp_new },
	.msvld    = { 0x00000001, gt215_msvld_new },
	.pm       = { 0x00000001, gt215_pm_new },
	.sw       = { 0x00000001, nv50_sw_new },
>>>>>>> upstream/android-13
};

static const struct nvkm_device_chip
nva5_chipset = {
	.name = "GT216",
<<<<<<< HEAD
	.bar = g84_bar_new,
	.bios = nvkm_bios_new,
	.bus = g94_bus_new,
	.clk = gt215_clk_new,
	.devinit = gt215_devinit_new,
	.fb = gt215_fb_new,
	.fuse = nv50_fuse_new,
	.gpio = g94_gpio_new,
	.i2c = g94_i2c_new,
	.imem = nv50_instmem_new,
	.mc = gt215_mc_new,
	.mmu = g84_mmu_new,
	.mxm = nv50_mxm_new,
	.pci = g94_pci_new,
	.pmu = gt215_pmu_new,
	.therm = gt215_therm_new,
	.timer = nv41_timer_new,
	.volt = nv40_volt_new,
	.ce[0] = gt215_ce_new,
	.disp = gt215_disp_new,
	.dma = nv50_dma_new,
	.fifo = g84_fifo_new,
	.gr = gt215_gr_new,
	.mspdec = gt215_mspdec_new,
	.msppp = gt215_msppp_new,
	.msvld = gt215_msvld_new,
	.pm = gt215_pm_new,
	.sw = nv50_sw_new,
=======
	.bar      = { 0x00000001, g84_bar_new },
	.bios     = { 0x00000001, nvkm_bios_new },
	.bus      = { 0x00000001, g94_bus_new },
	.clk      = { 0x00000001, gt215_clk_new },
	.devinit  = { 0x00000001, gt215_devinit_new },
	.fb       = { 0x00000001, gt215_fb_new },
	.fuse     = { 0x00000001, nv50_fuse_new },
	.gpio     = { 0x00000001, g94_gpio_new },
	.i2c      = { 0x00000001, g94_i2c_new },
	.imem     = { 0x00000001, nv50_instmem_new },
	.mc       = { 0x00000001, gt215_mc_new },
	.mmu      = { 0x00000001, g84_mmu_new },
	.mxm      = { 0x00000001, nv50_mxm_new },
	.pci      = { 0x00000001, g94_pci_new },
	.pmu      = { 0x00000001, gt215_pmu_new },
	.therm    = { 0x00000001, gt215_therm_new },
	.timer    = { 0x00000001, nv41_timer_new },
	.volt     = { 0x00000001, nv40_volt_new },
	.ce       = { 0x00000001, gt215_ce_new },
	.disp     = { 0x00000001, gt215_disp_new },
	.dma      = { 0x00000001, nv50_dma_new },
	.fifo     = { 0x00000001, g84_fifo_new },
	.gr       = { 0x00000001, gt215_gr_new },
	.mspdec   = { 0x00000001, gt215_mspdec_new },
	.msppp    = { 0x00000001, gt215_msppp_new },
	.msvld    = { 0x00000001, gt215_msvld_new },
	.pm       = { 0x00000001, gt215_pm_new },
	.sw       = { 0x00000001, nv50_sw_new },
>>>>>>> upstream/android-13
};

static const struct nvkm_device_chip
nva8_chipset = {
	.name = "GT218",
<<<<<<< HEAD
	.bar = g84_bar_new,
	.bios = nvkm_bios_new,
	.bus = g94_bus_new,
	.clk = gt215_clk_new,
	.devinit = gt215_devinit_new,
	.fb = gt215_fb_new,
	.fuse = nv50_fuse_new,
	.gpio = g94_gpio_new,
	.i2c = g94_i2c_new,
	.imem = nv50_instmem_new,
	.mc = gt215_mc_new,
	.mmu = g84_mmu_new,
	.mxm = nv50_mxm_new,
	.pci = g94_pci_new,
	.pmu = gt215_pmu_new,
	.therm = gt215_therm_new,
	.timer = nv41_timer_new,
	.volt = nv40_volt_new,
	.ce[0] = gt215_ce_new,
	.disp = gt215_disp_new,
	.dma = nv50_dma_new,
	.fifo = g84_fifo_new,
	.gr = gt215_gr_new,
	.mspdec = gt215_mspdec_new,
	.msppp = gt215_msppp_new,
	.msvld = gt215_msvld_new,
	.pm = gt215_pm_new,
	.sw = nv50_sw_new,
=======
	.bar      = { 0x00000001, g84_bar_new },
	.bios     = { 0x00000001, nvkm_bios_new },
	.bus      = { 0x00000001, g94_bus_new },
	.clk      = { 0x00000001, gt215_clk_new },
	.devinit  = { 0x00000001, gt215_devinit_new },
	.fb       = { 0x00000001, gt215_fb_new },
	.fuse     = { 0x00000001, nv50_fuse_new },
	.gpio     = { 0x00000001, g94_gpio_new },
	.i2c      = { 0x00000001, g94_i2c_new },
	.imem     = { 0x00000001, nv50_instmem_new },
	.mc       = { 0x00000001, gt215_mc_new },
	.mmu      = { 0x00000001, g84_mmu_new },
	.mxm      = { 0x00000001, nv50_mxm_new },
	.pci      = { 0x00000001, g94_pci_new },
	.pmu      = { 0x00000001, gt215_pmu_new },
	.therm    = { 0x00000001, gt215_therm_new },
	.timer    = { 0x00000001, nv41_timer_new },
	.volt     = { 0x00000001, nv40_volt_new },
	.ce       = { 0x00000001, gt215_ce_new },
	.disp     = { 0x00000001, gt215_disp_new },
	.dma      = { 0x00000001, nv50_dma_new },
	.fifo     = { 0x00000001, g84_fifo_new },
	.gr       = { 0x00000001, gt215_gr_new },
	.mspdec   = { 0x00000001, gt215_mspdec_new },
	.msppp    = { 0x00000001, gt215_msppp_new },
	.msvld    = { 0x00000001, gt215_msvld_new },
	.pm       = { 0x00000001, gt215_pm_new },
	.sw       = { 0x00000001, nv50_sw_new },
>>>>>>> upstream/android-13
};

static const struct nvkm_device_chip
nvaa_chipset = {
	.name = "MCP77/MCP78",
<<<<<<< HEAD
	.bar = g84_bar_new,
	.bios = nvkm_bios_new,
	.bus = g94_bus_new,
	.clk = mcp77_clk_new,
	.devinit = g98_devinit_new,
	.fb = mcp77_fb_new,
	.fuse = nv50_fuse_new,
	.gpio = g94_gpio_new,
	.i2c = g94_i2c_new,
	.imem = nv50_instmem_new,
	.mc = g98_mc_new,
	.mmu = mcp77_mmu_new,
	.mxm = nv50_mxm_new,
	.pci = g94_pci_new,
	.therm = g84_therm_new,
	.timer = nv41_timer_new,
	.volt = nv40_volt_new,
	.disp = mcp77_disp_new,
	.dma = nv50_dma_new,
	.fifo = g84_fifo_new,
	.gr = gt200_gr_new,
	.mspdec = g98_mspdec_new,
	.msppp = g98_msppp_new,
	.msvld = g98_msvld_new,
	.pm = g84_pm_new,
	.sec = g98_sec_new,
	.sw = nv50_sw_new,
=======
	.bar      = { 0x00000001, g84_bar_new },
	.bios     = { 0x00000001, nvkm_bios_new },
	.bus      = { 0x00000001, g94_bus_new },
	.clk      = { 0x00000001, mcp77_clk_new },
	.devinit  = { 0x00000001, g98_devinit_new },
	.fb       = { 0x00000001, mcp77_fb_new },
	.fuse     = { 0x00000001, nv50_fuse_new },
	.gpio     = { 0x00000001, g94_gpio_new },
	.i2c      = { 0x00000001, g94_i2c_new },
	.imem     = { 0x00000001, nv50_instmem_new },
	.mc       = { 0x00000001, g98_mc_new },
	.mmu      = { 0x00000001, mcp77_mmu_new },
	.mxm      = { 0x00000001, nv50_mxm_new },
	.pci      = { 0x00000001, g94_pci_new },
	.therm    = { 0x00000001, g84_therm_new },
	.timer    = { 0x00000001, nv41_timer_new },
	.volt     = { 0x00000001, nv40_volt_new },
	.disp     = { 0x00000001, mcp77_disp_new },
	.dma      = { 0x00000001, nv50_dma_new },
	.fifo     = { 0x00000001, g84_fifo_new },
	.gr       = { 0x00000001, gt200_gr_new },
	.mspdec   = { 0x00000001, g98_mspdec_new },
	.msppp    = { 0x00000001, g98_msppp_new },
	.msvld    = { 0x00000001, g98_msvld_new },
	.pm       = { 0x00000001, g84_pm_new },
	.sec      = { 0x00000001, g98_sec_new },
	.sw       = { 0x00000001, nv50_sw_new },
>>>>>>> upstream/android-13
};

static const struct nvkm_device_chip
nvac_chipset = {
	.name = "MCP79/MCP7A",
<<<<<<< HEAD
	.bar = g84_bar_new,
	.bios = nvkm_bios_new,
	.bus = g94_bus_new,
	.clk = mcp77_clk_new,
	.devinit = g98_devinit_new,
	.fb = mcp77_fb_new,
	.fuse = nv50_fuse_new,
	.gpio = g94_gpio_new,
	.i2c = g94_i2c_new,
	.imem = nv50_instmem_new,
	.mc = g98_mc_new,
	.mmu = mcp77_mmu_new,
	.mxm = nv50_mxm_new,
	.pci = g94_pci_new,
	.therm = g84_therm_new,
	.timer = nv41_timer_new,
	.volt = nv40_volt_new,
	.disp = mcp77_disp_new,
	.dma = nv50_dma_new,
	.fifo = g84_fifo_new,
	.gr = mcp79_gr_new,
	.mspdec = g98_mspdec_new,
	.msppp = g98_msppp_new,
	.msvld = g98_msvld_new,
	.pm = g84_pm_new,
	.sec = g98_sec_new,
	.sw = nv50_sw_new,
=======
	.bar      = { 0x00000001, g84_bar_new },
	.bios     = { 0x00000001, nvkm_bios_new },
	.bus      = { 0x00000001, g94_bus_new },
	.clk      = { 0x00000001, mcp77_clk_new },
	.devinit  = { 0x00000001, g98_devinit_new },
	.fb       = { 0x00000001, mcp77_fb_new },
	.fuse     = { 0x00000001, nv50_fuse_new },
	.gpio     = { 0x00000001, g94_gpio_new },
	.i2c      = { 0x00000001, g94_i2c_new },
	.imem     = { 0x00000001, nv50_instmem_new },
	.mc       = { 0x00000001, g98_mc_new },
	.mmu      = { 0x00000001, mcp77_mmu_new },
	.mxm      = { 0x00000001, nv50_mxm_new },
	.pci      = { 0x00000001, g94_pci_new },
	.therm    = { 0x00000001, g84_therm_new },
	.timer    = { 0x00000001, nv41_timer_new },
	.volt     = { 0x00000001, nv40_volt_new },
	.disp     = { 0x00000001, mcp77_disp_new },
	.dma      = { 0x00000001, nv50_dma_new },
	.fifo     = { 0x00000001, g84_fifo_new },
	.gr       = { 0x00000001, mcp79_gr_new },
	.mspdec   = { 0x00000001, g98_mspdec_new },
	.msppp    = { 0x00000001, g98_msppp_new },
	.msvld    = { 0x00000001, g98_msvld_new },
	.pm       = { 0x00000001, g84_pm_new },
	.sec      = { 0x00000001, g98_sec_new },
	.sw       = { 0x00000001, nv50_sw_new },
>>>>>>> upstream/android-13
};

static const struct nvkm_device_chip
nvaf_chipset = {
	.name = "MCP89",
<<<<<<< HEAD
	.bar = g84_bar_new,
	.bios = nvkm_bios_new,
	.bus = g94_bus_new,
	.clk = gt215_clk_new,
	.devinit = mcp89_devinit_new,
	.fb = mcp89_fb_new,
	.fuse = nv50_fuse_new,
	.gpio = g94_gpio_new,
	.i2c = g94_i2c_new,
	.imem = nv50_instmem_new,
	.mc = gt215_mc_new,
	.mmu = g84_mmu_new,
	.mxm = nv50_mxm_new,
	.pci = g94_pci_new,
	.pmu = gt215_pmu_new,
	.therm = gt215_therm_new,
	.timer = nv41_timer_new,
	.volt = nv40_volt_new,
	.ce[0] = gt215_ce_new,
	.disp = mcp89_disp_new,
	.dma = nv50_dma_new,
	.fifo = g84_fifo_new,
	.gr = mcp89_gr_new,
	.mspdec = gt215_mspdec_new,
	.msppp = gt215_msppp_new,
	.msvld = mcp89_msvld_new,
	.pm = gt215_pm_new,
	.sw = nv50_sw_new,
=======
	.bar      = { 0x00000001, g84_bar_new },
	.bios     = { 0x00000001, nvkm_bios_new },
	.bus      = { 0x00000001, g94_bus_new },
	.clk      = { 0x00000001, gt215_clk_new },
	.devinit  = { 0x00000001, mcp89_devinit_new },
	.fb       = { 0x00000001, mcp89_fb_new },
	.fuse     = { 0x00000001, nv50_fuse_new },
	.gpio     = { 0x00000001, g94_gpio_new },
	.i2c      = { 0x00000001, g94_i2c_new },
	.imem     = { 0x00000001, nv50_instmem_new },
	.mc       = { 0x00000001, gt215_mc_new },
	.mmu      = { 0x00000001, mcp77_mmu_new },
	.mxm      = { 0x00000001, nv50_mxm_new },
	.pci      = { 0x00000001, g94_pci_new },
	.pmu      = { 0x00000001, gt215_pmu_new },
	.therm    = { 0x00000001, gt215_therm_new },
	.timer    = { 0x00000001, nv41_timer_new },
	.volt     = { 0x00000001, nv40_volt_new },
	.ce       = { 0x00000001, gt215_ce_new },
	.disp     = { 0x00000001, mcp89_disp_new },
	.dma      = { 0x00000001, nv50_dma_new },
	.fifo     = { 0x00000001, g84_fifo_new },
	.gr       = { 0x00000001, mcp89_gr_new },
	.mspdec   = { 0x00000001, gt215_mspdec_new },
	.msppp    = { 0x00000001, gt215_msppp_new },
	.msvld    = { 0x00000001, mcp89_msvld_new },
	.pm       = { 0x00000001, gt215_pm_new },
	.sw       = { 0x00000001, nv50_sw_new },
>>>>>>> upstream/android-13
};

static const struct nvkm_device_chip
nvc0_chipset = {
	.name = "GF100",
<<<<<<< HEAD
	.bar = gf100_bar_new,
	.bios = nvkm_bios_new,
	.bus = gf100_bus_new,
	.clk = gf100_clk_new,
	.devinit = gf100_devinit_new,
	.fb = gf100_fb_new,
	.fuse = gf100_fuse_new,
	.gpio = g94_gpio_new,
	.i2c = g94_i2c_new,
	.ibus = gf100_ibus_new,
	.iccsense = gf100_iccsense_new,
	.imem = nv50_instmem_new,
	.ltc = gf100_ltc_new,
	.mc = gf100_mc_new,
	.mmu = gf100_mmu_new,
	.mxm = nv50_mxm_new,
	.pci = gf100_pci_new,
	.pmu = gf100_pmu_new,
	.therm = gt215_therm_new,
	.timer = nv41_timer_new,
	.volt = gf100_volt_new,
	.ce[0] = gf100_ce_new,
	.ce[1] = gf100_ce_new,
	.disp = gt215_disp_new,
	.dma = gf100_dma_new,
	.fifo = gf100_fifo_new,
	.gr = gf100_gr_new,
	.mspdec = gf100_mspdec_new,
	.msppp = gf100_msppp_new,
	.msvld = gf100_msvld_new,
	.pm = gf100_pm_new,
	.sw = gf100_sw_new,
=======
	.bar      = { 0x00000001, gf100_bar_new },
	.bios     = { 0x00000001, nvkm_bios_new },
	.bus      = { 0x00000001, gf100_bus_new },
	.clk      = { 0x00000001, gf100_clk_new },
	.devinit  = { 0x00000001, gf100_devinit_new },
	.fb       = { 0x00000001, gf100_fb_new },
	.fuse     = { 0x00000001, gf100_fuse_new },
	.gpio     = { 0x00000001, g94_gpio_new },
	.i2c      = { 0x00000001, g94_i2c_new },
	.iccsense = { 0x00000001, gf100_iccsense_new },
	.imem     = { 0x00000001, nv50_instmem_new },
	.ltc      = { 0x00000001, gf100_ltc_new },
	.mc       = { 0x00000001, gf100_mc_new },
	.mmu      = { 0x00000001, gf100_mmu_new },
	.mxm      = { 0x00000001, nv50_mxm_new },
	.pci      = { 0x00000001, gf100_pci_new },
	.pmu      = { 0x00000001, gf100_pmu_new },
	.privring = { 0x00000001, gf100_privring_new },
	.therm    = { 0x00000001, gt215_therm_new },
	.timer    = { 0x00000001, nv41_timer_new },
	.volt     = { 0x00000001, gf100_volt_new },
	.ce       = { 0x00000003, gf100_ce_new },
	.disp     = { 0x00000001, gt215_disp_new },
	.dma      = { 0x00000001, gf100_dma_new },
	.fifo     = { 0x00000001, gf100_fifo_new },
	.gr       = { 0x00000001, gf100_gr_new },
	.mspdec   = { 0x00000001, gf100_mspdec_new },
	.msppp    = { 0x00000001, gf100_msppp_new },
	.msvld    = { 0x00000001, gf100_msvld_new },
	.pm       = { 0x00000001, gf100_pm_new },
	.sw       = { 0x00000001, gf100_sw_new },
>>>>>>> upstream/android-13
};

static const struct nvkm_device_chip
nvc1_chipset = {
	.name = "GF108",
<<<<<<< HEAD
	.bar = gf100_bar_new,
	.bios = nvkm_bios_new,
	.bus = gf100_bus_new,
	.clk = gf100_clk_new,
	.devinit = gf100_devinit_new,
	.fb = gf108_fb_new,
	.fuse = gf100_fuse_new,
	.gpio = g94_gpio_new,
	.i2c = g94_i2c_new,
	.ibus = gf100_ibus_new,
	.iccsense = gf100_iccsense_new,
	.imem = nv50_instmem_new,
	.ltc = gf100_ltc_new,
	.mc = gf100_mc_new,
	.mmu = gf100_mmu_new,
	.mxm = nv50_mxm_new,
	.pci = gf106_pci_new,
	.pmu = gf100_pmu_new,
	.therm = gt215_therm_new,
	.timer = nv41_timer_new,
	.volt = gf100_volt_new,
	.ce[0] = gf100_ce_new,
	.disp = gt215_disp_new,
	.dma = gf100_dma_new,
	.fifo = gf100_fifo_new,
	.gr = gf108_gr_new,
	.mspdec = gf100_mspdec_new,
	.msppp = gf100_msppp_new,
	.msvld = gf100_msvld_new,
	.pm = gf108_pm_new,
	.sw = gf100_sw_new,
=======
	.bar      = { 0x00000001, gf100_bar_new },
	.bios     = { 0x00000001, nvkm_bios_new },
	.bus      = { 0x00000001, gf100_bus_new },
	.clk      = { 0x00000001, gf100_clk_new },
	.devinit  = { 0x00000001, gf100_devinit_new },
	.fb       = { 0x00000001, gf108_fb_new },
	.fuse     = { 0x00000001, gf100_fuse_new },
	.gpio     = { 0x00000001, g94_gpio_new },
	.i2c      = { 0x00000001, g94_i2c_new },
	.iccsense = { 0x00000001, gf100_iccsense_new },
	.imem     = { 0x00000001, nv50_instmem_new },
	.ltc      = { 0x00000001, gf100_ltc_new },
	.mc       = { 0x00000001, gf100_mc_new },
	.mmu      = { 0x00000001, gf100_mmu_new },
	.mxm      = { 0x00000001, nv50_mxm_new },
	.pci      = { 0x00000001, gf106_pci_new },
	.pmu      = { 0x00000001, gf100_pmu_new },
	.privring = { 0x00000001, gf100_privring_new },
	.therm    = { 0x00000001, gt215_therm_new },
	.timer    = { 0x00000001, nv41_timer_new },
	.volt     = { 0x00000001, gf100_volt_new },
	.ce       = { 0x00000001, gf100_ce_new },
	.disp     = { 0x00000001, gt215_disp_new },
	.dma      = { 0x00000001, gf100_dma_new },
	.fifo     = { 0x00000001, gf100_fifo_new },
	.gr       = { 0x00000001, gf108_gr_new },
	.mspdec   = { 0x00000001, gf100_mspdec_new },
	.msppp    = { 0x00000001, gf100_msppp_new },
	.msvld    = { 0x00000001, gf100_msvld_new },
	.pm       = { 0x00000001, gf108_pm_new },
	.sw       = { 0x00000001, gf100_sw_new },
>>>>>>> upstream/android-13
};

static const struct nvkm_device_chip
nvc3_chipset = {
	.name = "GF106",
<<<<<<< HEAD
	.bar = gf100_bar_new,
	.bios = nvkm_bios_new,
	.bus = gf100_bus_new,
	.clk = gf100_clk_new,
	.devinit = gf100_devinit_new,
	.fb = gf100_fb_new,
	.fuse = gf100_fuse_new,
	.gpio = g94_gpio_new,
	.i2c = g94_i2c_new,
	.ibus = gf100_ibus_new,
	.iccsense = gf100_iccsense_new,
	.imem = nv50_instmem_new,
	.ltc = gf100_ltc_new,
	.mc = gf100_mc_new,
	.mmu = gf100_mmu_new,
	.mxm = nv50_mxm_new,
	.pci = gf106_pci_new,
	.pmu = gf100_pmu_new,
	.therm = gt215_therm_new,
	.timer = nv41_timer_new,
	.volt = gf100_volt_new,
	.ce[0] = gf100_ce_new,
	.disp = gt215_disp_new,
	.dma = gf100_dma_new,
	.fifo = gf100_fifo_new,
	.gr = gf104_gr_new,
	.mspdec = gf100_mspdec_new,
	.msppp = gf100_msppp_new,
	.msvld = gf100_msvld_new,
	.pm = gf100_pm_new,
	.sw = gf100_sw_new,
=======
	.bar      = { 0x00000001, gf100_bar_new },
	.bios     = { 0x00000001, nvkm_bios_new },
	.bus      = { 0x00000001, gf100_bus_new },
	.clk      = { 0x00000001, gf100_clk_new },
	.devinit  = { 0x00000001, gf100_devinit_new },
	.fb       = { 0x00000001, gf100_fb_new },
	.fuse     = { 0x00000001, gf100_fuse_new },
	.gpio     = { 0x00000001, g94_gpio_new },
	.i2c      = { 0x00000001, g94_i2c_new },
	.iccsense = { 0x00000001, gf100_iccsense_new },
	.imem     = { 0x00000001, nv50_instmem_new },
	.ltc      = { 0x00000001, gf100_ltc_new },
	.mc       = { 0x00000001, gf100_mc_new },
	.mmu      = { 0x00000001, gf100_mmu_new },
	.mxm      = { 0x00000001, nv50_mxm_new },
	.pci      = { 0x00000001, gf106_pci_new },
	.pmu      = { 0x00000001, gf100_pmu_new },
	.privring = { 0x00000001, gf100_privring_new },
	.therm    = { 0x00000001, gt215_therm_new },
	.timer    = { 0x00000001, nv41_timer_new },
	.volt     = { 0x00000001, gf100_volt_new },
	.ce       = { 0x00000001, gf100_ce_new },
	.disp     = { 0x00000001, gt215_disp_new },
	.dma      = { 0x00000001, gf100_dma_new },
	.fifo     = { 0x00000001, gf100_fifo_new },
	.gr       = { 0x00000001, gf104_gr_new },
	.mspdec   = { 0x00000001, gf100_mspdec_new },
	.msppp    = { 0x00000001, gf100_msppp_new },
	.msvld    = { 0x00000001, gf100_msvld_new },
	.pm       = { 0x00000001, gf100_pm_new },
	.sw       = { 0x00000001, gf100_sw_new },
>>>>>>> upstream/android-13
};

static const struct nvkm_device_chip
nvc4_chipset = {
	.name = "GF104",
<<<<<<< HEAD
	.bar = gf100_bar_new,
	.bios = nvkm_bios_new,
	.bus = gf100_bus_new,
	.clk = gf100_clk_new,
	.devinit = gf100_devinit_new,
	.fb = gf100_fb_new,
	.fuse = gf100_fuse_new,
	.gpio = g94_gpio_new,
	.i2c = g94_i2c_new,
	.ibus = gf100_ibus_new,
	.iccsense = gf100_iccsense_new,
	.imem = nv50_instmem_new,
	.ltc = gf100_ltc_new,
	.mc = gf100_mc_new,
	.mmu = gf100_mmu_new,
	.mxm = nv50_mxm_new,
	.pci = gf100_pci_new,
	.pmu = gf100_pmu_new,
	.therm = gt215_therm_new,
	.timer = nv41_timer_new,
	.volt = gf100_volt_new,
	.ce[0] = gf100_ce_new,
	.ce[1] = gf100_ce_new,
	.disp = gt215_disp_new,
	.dma = gf100_dma_new,
	.fifo = gf100_fifo_new,
	.gr = gf104_gr_new,
	.mspdec = gf100_mspdec_new,
	.msppp = gf100_msppp_new,
	.msvld = gf100_msvld_new,
	.pm = gf100_pm_new,
	.sw = gf100_sw_new,
=======
	.bar      = { 0x00000001, gf100_bar_new },
	.bios     = { 0x00000001, nvkm_bios_new },
	.bus      = { 0x00000001, gf100_bus_new },
	.clk      = { 0x00000001, gf100_clk_new },
	.devinit  = { 0x00000001, gf100_devinit_new },
	.fb       = { 0x00000001, gf100_fb_new },
	.fuse     = { 0x00000001, gf100_fuse_new },
	.gpio     = { 0x00000001, g94_gpio_new },
	.i2c      = { 0x00000001, g94_i2c_new },
	.iccsense = { 0x00000001, gf100_iccsense_new },
	.imem     = { 0x00000001, nv50_instmem_new },
	.ltc      = { 0x00000001, gf100_ltc_new },
	.mc       = { 0x00000001, gf100_mc_new },
	.mmu      = { 0x00000001, gf100_mmu_new },
	.mxm      = { 0x00000001, nv50_mxm_new },
	.pci      = { 0x00000001, gf100_pci_new },
	.pmu      = { 0x00000001, gf100_pmu_new },
	.privring = { 0x00000001, gf100_privring_new },
	.therm    = { 0x00000001, gt215_therm_new },
	.timer    = { 0x00000001, nv41_timer_new },
	.volt     = { 0x00000001, gf100_volt_new },
	.ce       = { 0x00000003, gf100_ce_new },
	.disp     = { 0x00000001, gt215_disp_new },
	.dma      = { 0x00000001, gf100_dma_new },
	.fifo     = { 0x00000001, gf100_fifo_new },
	.gr       = { 0x00000001, gf104_gr_new },
	.mspdec   = { 0x00000001, gf100_mspdec_new },
	.msppp    = { 0x00000001, gf100_msppp_new },
	.msvld    = { 0x00000001, gf100_msvld_new },
	.pm       = { 0x00000001, gf100_pm_new },
	.sw       = { 0x00000001, gf100_sw_new },
>>>>>>> upstream/android-13
};

static const struct nvkm_device_chip
nvc8_chipset = {
	.name = "GF110",
<<<<<<< HEAD
	.bar = gf100_bar_new,
	.bios = nvkm_bios_new,
	.bus = gf100_bus_new,
	.clk = gf100_clk_new,
	.devinit = gf100_devinit_new,
	.fb = gf100_fb_new,
	.fuse = gf100_fuse_new,
	.gpio = g94_gpio_new,
	.i2c = g94_i2c_new,
	.ibus = gf100_ibus_new,
	.iccsense = gf100_iccsense_new,
	.imem = nv50_instmem_new,
	.ltc = gf100_ltc_new,
	.mc = gf100_mc_new,
	.mmu = gf100_mmu_new,
	.mxm = nv50_mxm_new,
	.pci = gf100_pci_new,
	.pmu = gf100_pmu_new,
	.therm = gt215_therm_new,
	.timer = nv41_timer_new,
	.volt = gf100_volt_new,
	.ce[0] = gf100_ce_new,
	.ce[1] = gf100_ce_new,
	.disp = gt215_disp_new,
	.dma = gf100_dma_new,
	.fifo = gf100_fifo_new,
	.gr = gf110_gr_new,
	.mspdec = gf100_mspdec_new,
	.msppp = gf100_msppp_new,
	.msvld = gf100_msvld_new,
	.pm = gf100_pm_new,
	.sw = gf100_sw_new,
=======
	.bar      = { 0x00000001, gf100_bar_new },
	.bios     = { 0x00000001, nvkm_bios_new },
	.bus      = { 0x00000001, gf100_bus_new },
	.clk      = { 0x00000001, gf100_clk_new },
	.devinit  = { 0x00000001, gf100_devinit_new },
	.fb       = { 0x00000001, gf100_fb_new },
	.fuse     = { 0x00000001, gf100_fuse_new },
	.gpio     = { 0x00000001, g94_gpio_new },
	.i2c      = { 0x00000001, g94_i2c_new },
	.iccsense = { 0x00000001, gf100_iccsense_new },
	.imem     = { 0x00000001, nv50_instmem_new },
	.ltc      = { 0x00000001, gf100_ltc_new },
	.mc       = { 0x00000001, gf100_mc_new },
	.mmu      = { 0x00000001, gf100_mmu_new },
	.mxm      = { 0x00000001, nv50_mxm_new },
	.pci      = { 0x00000001, gf100_pci_new },
	.pmu      = { 0x00000001, gf100_pmu_new },
	.privring = { 0x00000001, gf100_privring_new },
	.therm    = { 0x00000001, gt215_therm_new },
	.timer    = { 0x00000001, nv41_timer_new },
	.volt     = { 0x00000001, gf100_volt_new },
	.ce       = { 0x00000003, gf100_ce_new },
	.disp     = { 0x00000001, gt215_disp_new },
	.dma      = { 0x00000001, gf100_dma_new },
	.fifo     = { 0x00000001, gf100_fifo_new },
	.gr       = { 0x00000001, gf110_gr_new },
	.mspdec   = { 0x00000001, gf100_mspdec_new },
	.msppp    = { 0x00000001, gf100_msppp_new },
	.msvld    = { 0x00000001, gf100_msvld_new },
	.pm       = { 0x00000001, gf100_pm_new },
	.sw       = { 0x00000001, gf100_sw_new },
>>>>>>> upstream/android-13
};

static const struct nvkm_device_chip
nvce_chipset = {
	.name = "GF114",
<<<<<<< HEAD
	.bar = gf100_bar_new,
	.bios = nvkm_bios_new,
	.bus = gf100_bus_new,
	.clk = gf100_clk_new,
	.devinit = gf100_devinit_new,
	.fb = gf100_fb_new,
	.fuse = gf100_fuse_new,
	.gpio = g94_gpio_new,
	.i2c = g94_i2c_new,
	.ibus = gf100_ibus_new,
	.iccsense = gf100_iccsense_new,
	.imem = nv50_instmem_new,
	.ltc = gf100_ltc_new,
	.mc = gf100_mc_new,
	.mmu = gf100_mmu_new,
	.mxm = nv50_mxm_new,
	.pci = gf100_pci_new,
	.pmu = gf100_pmu_new,
	.therm = gt215_therm_new,
	.timer = nv41_timer_new,
	.volt = gf100_volt_new,
	.ce[0] = gf100_ce_new,
	.ce[1] = gf100_ce_new,
	.disp = gt215_disp_new,
	.dma = gf100_dma_new,
	.fifo = gf100_fifo_new,
	.gr = gf104_gr_new,
	.mspdec = gf100_mspdec_new,
	.msppp = gf100_msppp_new,
	.msvld = gf100_msvld_new,
	.pm = gf100_pm_new,
	.sw = gf100_sw_new,
=======
	.bar      = { 0x00000001, gf100_bar_new },
	.bios     = { 0x00000001, nvkm_bios_new },
	.bus      = { 0x00000001, gf100_bus_new },
	.clk      = { 0x00000001, gf100_clk_new },
	.devinit  = { 0x00000001, gf100_devinit_new },
	.fb       = { 0x00000001, gf100_fb_new },
	.fuse     = { 0x00000001, gf100_fuse_new },
	.gpio     = { 0x00000001, g94_gpio_new },
	.i2c      = { 0x00000001, g94_i2c_new },
	.iccsense = { 0x00000001, gf100_iccsense_new },
	.imem     = { 0x00000001, nv50_instmem_new },
	.ltc      = { 0x00000001, gf100_ltc_new },
	.mc       = { 0x00000001, gf100_mc_new },
	.mmu      = { 0x00000001, gf100_mmu_new },
	.mxm      = { 0x00000001, nv50_mxm_new },
	.pci      = { 0x00000001, gf100_pci_new },
	.pmu      = { 0x00000001, gf100_pmu_new },
	.privring = { 0x00000001, gf100_privring_new },
	.therm    = { 0x00000001, gt215_therm_new },
	.timer    = { 0x00000001, nv41_timer_new },
	.volt     = { 0x00000001, gf100_volt_new },
	.ce       = { 0x00000003, gf100_ce_new },
	.disp     = { 0x00000001, gt215_disp_new },
	.dma      = { 0x00000001, gf100_dma_new },
	.fifo     = { 0x00000001, gf100_fifo_new },
	.gr       = { 0x00000001, gf104_gr_new },
	.mspdec   = { 0x00000001, gf100_mspdec_new },
	.msppp    = { 0x00000001, gf100_msppp_new },
	.msvld    = { 0x00000001, gf100_msvld_new },
	.pm       = { 0x00000001, gf100_pm_new },
	.sw       = { 0x00000001, gf100_sw_new },
>>>>>>> upstream/android-13
};

static const struct nvkm_device_chip
nvcf_chipset = {
	.name = "GF116",
<<<<<<< HEAD
	.bar = gf100_bar_new,
	.bios = nvkm_bios_new,
	.bus = gf100_bus_new,
	.clk = gf100_clk_new,
	.devinit = gf100_devinit_new,
	.fb = gf100_fb_new,
	.fuse = gf100_fuse_new,
	.gpio = g94_gpio_new,
	.i2c = g94_i2c_new,
	.ibus = gf100_ibus_new,
	.iccsense = gf100_iccsense_new,
	.imem = nv50_instmem_new,
	.ltc = gf100_ltc_new,
	.mc = gf100_mc_new,
	.mmu = gf100_mmu_new,
	.mxm = nv50_mxm_new,
	.pci = gf106_pci_new,
	.pmu = gf100_pmu_new,
	.therm = gt215_therm_new,
	.timer = nv41_timer_new,
	.volt = gf100_volt_new,
	.ce[0] = gf100_ce_new,
	.disp = gt215_disp_new,
	.dma = gf100_dma_new,
	.fifo = gf100_fifo_new,
	.gr = gf104_gr_new,
	.mspdec = gf100_mspdec_new,
	.msppp = gf100_msppp_new,
	.msvld = gf100_msvld_new,
	.pm = gf100_pm_new,
	.sw = gf100_sw_new,
=======
	.bar      = { 0x00000001, gf100_bar_new },
	.bios     = { 0x00000001, nvkm_bios_new },
	.bus      = { 0x00000001, gf100_bus_new },
	.clk      = { 0x00000001, gf100_clk_new },
	.devinit  = { 0x00000001, gf100_devinit_new },
	.fb       = { 0x00000001, gf100_fb_new },
	.fuse     = { 0x00000001, gf100_fuse_new },
	.gpio     = { 0x00000001, g94_gpio_new },
	.i2c      = { 0x00000001, g94_i2c_new },
	.iccsense = { 0x00000001, gf100_iccsense_new },
	.imem     = { 0x00000001, nv50_instmem_new },
	.ltc      = { 0x00000001, gf100_ltc_new },
	.mc       = { 0x00000001, gf100_mc_new },
	.mmu      = { 0x00000001, gf100_mmu_new },
	.mxm      = { 0x00000001, nv50_mxm_new },
	.pci      = { 0x00000001, gf106_pci_new },
	.pmu      = { 0x00000001, gf100_pmu_new },
	.privring = { 0x00000001, gf100_privring_new },
	.therm    = { 0x00000001, gt215_therm_new },
	.timer    = { 0x00000001, nv41_timer_new },
	.volt     = { 0x00000001, gf100_volt_new },
	.ce       = { 0x00000001, gf100_ce_new },
	.disp     = { 0x00000001, gt215_disp_new },
	.dma      = { 0x00000001, gf100_dma_new },
	.fifo     = { 0x00000001, gf100_fifo_new },
	.gr       = { 0x00000001, gf104_gr_new },
	.mspdec   = { 0x00000001, gf100_mspdec_new },
	.msppp    = { 0x00000001, gf100_msppp_new },
	.msvld    = { 0x00000001, gf100_msvld_new },
	.pm       = { 0x00000001, gf100_pm_new },
	.sw       = { 0x00000001, gf100_sw_new },
>>>>>>> upstream/android-13
};

static const struct nvkm_device_chip
nvd7_chipset = {
	.name = "GF117",
<<<<<<< HEAD
	.bar = gf100_bar_new,
	.bios = nvkm_bios_new,
	.bus = gf100_bus_new,
	.clk = gf100_clk_new,
	.devinit = gf100_devinit_new,
	.fb = gf100_fb_new,
	.fuse = gf100_fuse_new,
	.gpio = gf119_gpio_new,
	.i2c = gf117_i2c_new,
	.ibus = gf117_ibus_new,
	.iccsense = gf100_iccsense_new,
	.imem = nv50_instmem_new,
	.ltc = gf100_ltc_new,
	.mc = gf100_mc_new,
	.mmu = gf100_mmu_new,
	.mxm = nv50_mxm_new,
	.pci = gf106_pci_new,
	.therm = gf119_therm_new,
	.timer = nv41_timer_new,
	.volt = gf117_volt_new,
	.ce[0] = gf100_ce_new,
	.disp = gf119_disp_new,
	.dma = gf119_dma_new,
	.fifo = gf100_fifo_new,
	.gr = gf117_gr_new,
	.mspdec = gf100_mspdec_new,
	.msppp = gf100_msppp_new,
	.msvld = gf100_msvld_new,
	.pm = gf117_pm_new,
	.sw = gf100_sw_new,
=======
	.bar      = { 0x00000001, gf100_bar_new },
	.bios     = { 0x00000001, nvkm_bios_new },
	.bus      = { 0x00000001, gf100_bus_new },
	.clk      = { 0x00000001, gf100_clk_new },
	.devinit  = { 0x00000001, gf100_devinit_new },
	.fb       = { 0x00000001, gf100_fb_new },
	.fuse     = { 0x00000001, gf100_fuse_new },
	.gpio     = { 0x00000001, gf119_gpio_new },
	.i2c      = { 0x00000001, gf117_i2c_new },
	.iccsense = { 0x00000001, gf100_iccsense_new },
	.imem     = { 0x00000001, nv50_instmem_new },
	.ltc      = { 0x00000001, gf100_ltc_new },
	.mc       = { 0x00000001, gf100_mc_new },
	.mmu      = { 0x00000001, gf100_mmu_new },
	.mxm      = { 0x00000001, nv50_mxm_new },
	.pci      = { 0x00000001, gf106_pci_new },
	.privring = { 0x00000001, gf117_privring_new },
	.therm    = { 0x00000001, gf119_therm_new },
	.timer    = { 0x00000001, nv41_timer_new },
	.volt     = { 0x00000001, gf117_volt_new },
	.ce       = { 0x00000001, gf100_ce_new },
	.disp     = { 0x00000001, gf119_disp_new },
	.dma      = { 0x00000001, gf119_dma_new },
	.fifo     = { 0x00000001, gf100_fifo_new },
	.gr       = { 0x00000001, gf117_gr_new },
	.mspdec   = { 0x00000001, gf100_mspdec_new },
	.msppp    = { 0x00000001, gf100_msppp_new },
	.msvld    = { 0x00000001, gf100_msvld_new },
	.pm       = { 0x00000001, gf117_pm_new },
	.sw       = { 0x00000001, gf100_sw_new },
>>>>>>> upstream/android-13
};

static const struct nvkm_device_chip
nvd9_chipset = {
	.name = "GF119",
<<<<<<< HEAD
	.bar = gf100_bar_new,
	.bios = nvkm_bios_new,
	.bus = gf100_bus_new,
	.clk = gf100_clk_new,
	.devinit = gf100_devinit_new,
	.fb = gf100_fb_new,
	.fuse = gf100_fuse_new,
	.gpio = gf119_gpio_new,
	.i2c = gf119_i2c_new,
	.ibus = gf117_ibus_new,
	.iccsense = gf100_iccsense_new,
	.imem = nv50_instmem_new,
	.ltc = gf100_ltc_new,
	.mc = gf100_mc_new,
	.mmu = gf100_mmu_new,
	.mxm = nv50_mxm_new,
	.pci = gf106_pci_new,
	.pmu = gf119_pmu_new,
	.therm = gf119_therm_new,
	.timer = nv41_timer_new,
	.volt = gf100_volt_new,
	.ce[0] = gf100_ce_new,
	.disp = gf119_disp_new,
	.dma = gf119_dma_new,
	.fifo = gf100_fifo_new,
	.gr = gf119_gr_new,
	.mspdec = gf100_mspdec_new,
	.msppp = gf100_msppp_new,
	.msvld = gf100_msvld_new,
	.pm = gf117_pm_new,
	.sw = gf100_sw_new,
=======
	.bar      = { 0x00000001, gf100_bar_new },
	.bios     = { 0x00000001, nvkm_bios_new },
	.bus      = { 0x00000001, gf100_bus_new },
	.clk      = { 0x00000001, gf100_clk_new },
	.devinit  = { 0x00000001, gf100_devinit_new },
	.fb       = { 0x00000001, gf100_fb_new },
	.fuse     = { 0x00000001, gf100_fuse_new },
	.gpio     = { 0x00000001, gf119_gpio_new },
	.i2c      = { 0x00000001, gf119_i2c_new },
	.iccsense = { 0x00000001, gf100_iccsense_new },
	.imem     = { 0x00000001, nv50_instmem_new },
	.ltc      = { 0x00000001, gf100_ltc_new },
	.mc       = { 0x00000001, gf100_mc_new },
	.mmu      = { 0x00000001, gf100_mmu_new },
	.mxm      = { 0x00000001, nv50_mxm_new },
	.pci      = { 0x00000001, gf106_pci_new },
	.pmu      = { 0x00000001, gf119_pmu_new },
	.privring = { 0x00000001, gf117_privring_new },
	.therm    = { 0x00000001, gf119_therm_new },
	.timer    = { 0x00000001, nv41_timer_new },
	.volt     = { 0x00000001, gf100_volt_new },
	.ce       = { 0x00000001, gf100_ce_new },
	.disp     = { 0x00000001, gf119_disp_new },
	.dma      = { 0x00000001, gf119_dma_new },
	.fifo     = { 0x00000001, gf100_fifo_new },
	.gr       = { 0x00000001, gf119_gr_new },
	.mspdec   = { 0x00000001, gf100_mspdec_new },
	.msppp    = { 0x00000001, gf100_msppp_new },
	.msvld    = { 0x00000001, gf100_msvld_new },
	.pm       = { 0x00000001, gf117_pm_new },
	.sw       = { 0x00000001, gf100_sw_new },
>>>>>>> upstream/android-13
};

static const struct nvkm_device_chip
nve4_chipset = {
	.name = "GK104",
<<<<<<< HEAD
	.bar = gf100_bar_new,
	.bios = nvkm_bios_new,
	.bus = gf100_bus_new,
	.clk = gk104_clk_new,
	.devinit = gf100_devinit_new,
	.fb = gk104_fb_new,
	.fuse = gf100_fuse_new,
	.gpio = gk104_gpio_new,
	.i2c = gk104_i2c_new,
	.ibus = gk104_ibus_new,
	.iccsense = gf100_iccsense_new,
	.imem = nv50_instmem_new,
	.ltc = gk104_ltc_new,
	.mc = gk104_mc_new,
	.mmu = gk104_mmu_new,
	.mxm = nv50_mxm_new,
	.pci = gk104_pci_new,
	.pmu = gk104_pmu_new,
	.therm = gk104_therm_new,
	.timer = nv41_timer_new,
	.top = gk104_top_new,
	.volt = gk104_volt_new,
	.ce[0] = gk104_ce_new,
	.ce[1] = gk104_ce_new,
	.ce[2] = gk104_ce_new,
	.disp = gk104_disp_new,
	.dma = gf119_dma_new,
	.fifo = gk104_fifo_new,
	.gr = gk104_gr_new,
	.mspdec = gk104_mspdec_new,
	.msppp = gf100_msppp_new,
	.msvld = gk104_msvld_new,
	.pm = gk104_pm_new,
	.sw = gf100_sw_new,
=======
	.bar      = { 0x00000001, gf100_bar_new },
	.bios     = { 0x00000001, nvkm_bios_new },
	.bus      = { 0x00000001, gf100_bus_new },
	.clk      = { 0x00000001, gk104_clk_new },
	.devinit  = { 0x00000001, gf100_devinit_new },
	.fb       = { 0x00000001, gk104_fb_new },
	.fuse     = { 0x00000001, gf100_fuse_new },
	.gpio     = { 0x00000001, gk104_gpio_new },
	.i2c      = { 0x00000001, gk104_i2c_new },
	.iccsense = { 0x00000001, gf100_iccsense_new },
	.imem     = { 0x00000001, nv50_instmem_new },
	.ltc      = { 0x00000001, gk104_ltc_new },
	.mc       = { 0x00000001, gk104_mc_new },
	.mmu      = { 0x00000001, gk104_mmu_new },
	.mxm      = { 0x00000001, nv50_mxm_new },
	.pci      = { 0x00000001, gk104_pci_new },
	.pmu      = { 0x00000001, gk104_pmu_new },
	.privring = { 0x00000001, gk104_privring_new },
	.therm    = { 0x00000001, gk104_therm_new },
	.timer    = { 0x00000001, nv41_timer_new },
	.top      = { 0x00000001, gk104_top_new },
	.volt     = { 0x00000001, gk104_volt_new },
	.ce       = { 0x00000007, gk104_ce_new },
	.disp     = { 0x00000001, gk104_disp_new },
	.dma      = { 0x00000001, gf119_dma_new },
	.fifo     = { 0x00000001, gk104_fifo_new },
	.gr       = { 0x00000001, gk104_gr_new },
	.mspdec   = { 0x00000001, gk104_mspdec_new },
	.msppp    = { 0x00000001, gf100_msppp_new },
	.msvld    = { 0x00000001, gk104_msvld_new },
	.pm       = { 0x00000001, gk104_pm_new },
	.sw       = { 0x00000001, gf100_sw_new },
>>>>>>> upstream/android-13
};

static const struct nvkm_device_chip
nve6_chipset = {
	.name = "GK106",
<<<<<<< HEAD
	.bar = gf100_bar_new,
	.bios = nvkm_bios_new,
	.bus = gf100_bus_new,
	.clk = gk104_clk_new,
	.devinit = gf100_devinit_new,
	.fb = gk104_fb_new,
	.fuse = gf100_fuse_new,
	.gpio = gk104_gpio_new,
	.i2c = gk104_i2c_new,
	.ibus = gk104_ibus_new,
	.iccsense = gf100_iccsense_new,
	.imem = nv50_instmem_new,
	.ltc = gk104_ltc_new,
	.mc = gk104_mc_new,
	.mmu = gk104_mmu_new,
	.mxm = nv50_mxm_new,
	.pci = gk104_pci_new,
	.pmu = gk104_pmu_new,
	.therm = gk104_therm_new,
	.timer = nv41_timer_new,
	.top = gk104_top_new,
	.volt = gk104_volt_new,
	.ce[0] = gk104_ce_new,
	.ce[1] = gk104_ce_new,
	.ce[2] = gk104_ce_new,
	.disp = gk104_disp_new,
	.dma = gf119_dma_new,
	.fifo = gk104_fifo_new,
	.gr = gk104_gr_new,
	.mspdec = gk104_mspdec_new,
	.msppp = gf100_msppp_new,
	.msvld = gk104_msvld_new,
	.pm = gk104_pm_new,
	.sw = gf100_sw_new,
=======
	.bar      = { 0x00000001, gf100_bar_new },
	.bios     = { 0x00000001, nvkm_bios_new },
	.bus      = { 0x00000001, gf100_bus_new },
	.clk      = { 0x00000001, gk104_clk_new },
	.devinit  = { 0x00000001, gf100_devinit_new },
	.fb       = { 0x00000001, gk104_fb_new },
	.fuse     = { 0x00000001, gf100_fuse_new },
	.gpio     = { 0x00000001, gk104_gpio_new },
	.i2c      = { 0x00000001, gk104_i2c_new },
	.iccsense = { 0x00000001, gf100_iccsense_new },
	.imem     = { 0x00000001, nv50_instmem_new },
	.ltc      = { 0x00000001, gk104_ltc_new },
	.mc       = { 0x00000001, gk104_mc_new },
	.mmu      = { 0x00000001, gk104_mmu_new },
	.mxm      = { 0x00000001, nv50_mxm_new },
	.pci      = { 0x00000001, gk104_pci_new },
	.pmu      = { 0x00000001, gk104_pmu_new },
	.privring = { 0x00000001, gk104_privring_new },
	.therm    = { 0x00000001, gk104_therm_new },
	.timer    = { 0x00000001, nv41_timer_new },
	.top      = { 0x00000001, gk104_top_new },
	.volt     = { 0x00000001, gk104_volt_new },
	.ce       = { 0x00000007, gk104_ce_new },
	.disp     = { 0x00000001, gk104_disp_new },
	.dma      = { 0x00000001, gf119_dma_new },
	.fifo     = { 0x00000001, gk104_fifo_new },
	.gr       = { 0x00000001, gk104_gr_new },
	.mspdec   = { 0x00000001, gk104_mspdec_new },
	.msppp    = { 0x00000001, gf100_msppp_new },
	.msvld    = { 0x00000001, gk104_msvld_new },
	.pm       = { 0x00000001, gk104_pm_new },
	.sw       = { 0x00000001, gf100_sw_new },
>>>>>>> upstream/android-13
};

static const struct nvkm_device_chip
nve7_chipset = {
	.name = "GK107",
<<<<<<< HEAD
	.bar = gf100_bar_new,
	.bios = nvkm_bios_new,
	.bus = gf100_bus_new,
	.clk = gk104_clk_new,
	.devinit = gf100_devinit_new,
	.fb = gk104_fb_new,
	.fuse = gf100_fuse_new,
	.gpio = gk104_gpio_new,
	.i2c = gk104_i2c_new,
	.ibus = gk104_ibus_new,
	.iccsense = gf100_iccsense_new,
	.imem = nv50_instmem_new,
	.ltc = gk104_ltc_new,
	.mc = gk104_mc_new,
	.mmu = gk104_mmu_new,
	.mxm = nv50_mxm_new,
	.pci = gk104_pci_new,
	.pmu = gk104_pmu_new,
	.therm = gk104_therm_new,
	.timer = nv41_timer_new,
	.top = gk104_top_new,
	.volt = gk104_volt_new,
	.ce[0] = gk104_ce_new,
	.ce[1] = gk104_ce_new,
	.ce[2] = gk104_ce_new,
	.disp = gk104_disp_new,
	.dma = gf119_dma_new,
	.fifo = gk104_fifo_new,
	.gr = gk104_gr_new,
	.mspdec = gk104_mspdec_new,
	.msppp = gf100_msppp_new,
	.msvld = gk104_msvld_new,
	.pm = gk104_pm_new,
	.sw = gf100_sw_new,
=======
	.bar      = { 0x00000001, gf100_bar_new },
	.bios     = { 0x00000001, nvkm_bios_new },
	.bus      = { 0x00000001, gf100_bus_new },
	.clk      = { 0x00000001, gk104_clk_new },
	.devinit  = { 0x00000001, gf100_devinit_new },
	.fb       = { 0x00000001, gk104_fb_new },
	.fuse     = { 0x00000001, gf100_fuse_new },
	.gpio     = { 0x00000001, gk104_gpio_new },
	.i2c      = { 0x00000001, gk104_i2c_new },
	.iccsense = { 0x00000001, gf100_iccsense_new },
	.imem     = { 0x00000001, nv50_instmem_new },
	.ltc      = { 0x00000001, gk104_ltc_new },
	.mc       = { 0x00000001, gk104_mc_new },
	.mmu      = { 0x00000001, gk104_mmu_new },
	.mxm      = { 0x00000001, nv50_mxm_new },
	.pci      = { 0x00000001, gk104_pci_new },
	.pmu      = { 0x00000001, gk104_pmu_new },
	.privring = { 0x00000001, gk104_privring_new },
	.therm    = { 0x00000001, gk104_therm_new },
	.timer    = { 0x00000001, nv41_timer_new },
	.top      = { 0x00000001, gk104_top_new },
	.volt     = { 0x00000001, gk104_volt_new },
	.ce       = { 0x00000007, gk104_ce_new },
	.disp     = { 0x00000001, gk104_disp_new },
	.dma      = { 0x00000001, gf119_dma_new },
	.fifo     = { 0x00000001, gk104_fifo_new },
	.gr       = { 0x00000001, gk104_gr_new },
	.mspdec   = { 0x00000001, gk104_mspdec_new },
	.msppp    = { 0x00000001, gf100_msppp_new },
	.msvld    = { 0x00000001, gk104_msvld_new },
	.pm       = { 0x00000001, gk104_pm_new },
	.sw       = { 0x00000001, gf100_sw_new },
>>>>>>> upstream/android-13
};

static const struct nvkm_device_chip
nvea_chipset = {
	.name = "GK20A",
<<<<<<< HEAD
	.bar = gk20a_bar_new,
	.bus = gf100_bus_new,
	.clk = gk20a_clk_new,
	.fb = gk20a_fb_new,
	.fuse = gf100_fuse_new,
	.ibus = gk20a_ibus_new,
	.imem = gk20a_instmem_new,
	.ltc = gk104_ltc_new,
	.mc = gk20a_mc_new,
	.mmu = gk20a_mmu_new,
	.pmu = gk20a_pmu_new,
	.timer = gk20a_timer_new,
	.top = gk104_top_new,
	.volt = gk20a_volt_new,
	.ce[2] = gk104_ce_new,
	.dma = gf119_dma_new,
	.fifo = gk20a_fifo_new,
	.gr = gk20a_gr_new,
	.pm = gk104_pm_new,
	.sw = gf100_sw_new,
=======
	.bar      = { 0x00000001, gk20a_bar_new },
	.bus      = { 0x00000001, gf100_bus_new },
	.clk      = { 0x00000001, gk20a_clk_new },
	.fb       = { 0x00000001, gk20a_fb_new },
	.fuse     = { 0x00000001, gf100_fuse_new },
	.imem     = { 0x00000001, gk20a_instmem_new },
	.ltc      = { 0x00000001, gk104_ltc_new },
	.mc       = { 0x00000001, gk20a_mc_new },
	.mmu      = { 0x00000001, gk20a_mmu_new },
	.pmu      = { 0x00000001, gk20a_pmu_new },
	.privring = { 0x00000001, gk20a_privring_new },
	.timer    = { 0x00000001, gk20a_timer_new },
	.top      = { 0x00000001, gk104_top_new },
	.volt     = { 0x00000001, gk20a_volt_new },
	.ce       = { 0x00000004, gk104_ce_new },
	.dma      = { 0x00000001, gf119_dma_new },
	.fifo     = { 0x00000001, gk20a_fifo_new },
	.gr       = { 0x00000001, gk20a_gr_new },
	.pm       = { 0x00000001, gk104_pm_new },
	.sw       = { 0x00000001, gf100_sw_new },
>>>>>>> upstream/android-13
};

static const struct nvkm_device_chip
nvf0_chipset = {
	.name = "GK110",
<<<<<<< HEAD
	.bar = gf100_bar_new,
	.bios = nvkm_bios_new,
	.bus = gf100_bus_new,
	.clk = gk104_clk_new,
	.devinit = gf100_devinit_new,
	.fb = gk110_fb_new,
	.fuse = gf100_fuse_new,
	.gpio = gk104_gpio_new,
	.i2c = gk104_i2c_new,
	.ibus = gk104_ibus_new,
	.iccsense = gf100_iccsense_new,
	.imem = nv50_instmem_new,
	.ltc = gk104_ltc_new,
	.mc = gk104_mc_new,
	.mmu = gk104_mmu_new,
	.mxm = nv50_mxm_new,
	.pci = gk104_pci_new,
	.pmu = gk110_pmu_new,
	.therm = gk104_therm_new,
	.timer = nv41_timer_new,
	.top = gk104_top_new,
	.volt = gk104_volt_new,
	.ce[0] = gk104_ce_new,
	.ce[1] = gk104_ce_new,
	.ce[2] = gk104_ce_new,
	.disp = gk110_disp_new,
	.dma = gf119_dma_new,
	.fifo = gk110_fifo_new,
	.gr = gk110_gr_new,
	.mspdec = gk104_mspdec_new,
	.msppp = gf100_msppp_new,
	.msvld = gk104_msvld_new,
	.sw = gf100_sw_new,
=======
	.bar      = { 0x00000001, gf100_bar_new },
	.bios     = { 0x00000001, nvkm_bios_new },
	.bus      = { 0x00000001, gf100_bus_new },
	.clk      = { 0x00000001, gk104_clk_new },
	.devinit  = { 0x00000001, gf100_devinit_new },
	.fb       = { 0x00000001, gk110_fb_new },
	.fuse     = { 0x00000001, gf100_fuse_new },
	.gpio     = { 0x00000001, gk104_gpio_new },
	.i2c      = { 0x00000001, gk110_i2c_new },
	.iccsense = { 0x00000001, gf100_iccsense_new },
	.imem     = { 0x00000001, nv50_instmem_new },
	.ltc      = { 0x00000001, gk104_ltc_new },
	.mc       = { 0x00000001, gk104_mc_new },
	.mmu      = { 0x00000001, gk104_mmu_new },
	.mxm      = { 0x00000001, nv50_mxm_new },
	.pci      = { 0x00000001, gk104_pci_new },
	.pmu      = { 0x00000001, gk110_pmu_new },
	.privring = { 0x00000001, gk104_privring_new },
	.therm    = { 0x00000001, gk104_therm_new },
	.timer    = { 0x00000001, nv41_timer_new },
	.top      = { 0x00000001, gk104_top_new },
	.volt     = { 0x00000001, gk104_volt_new },
	.ce       = { 0x00000007, gk104_ce_new },
	.disp     = { 0x00000001, gk110_disp_new },
	.dma      = { 0x00000001, gf119_dma_new },
	.fifo     = { 0x00000001, gk110_fifo_new },
	.gr       = { 0x00000001, gk110_gr_new },
	.mspdec   = { 0x00000001, gk104_mspdec_new },
	.msppp    = { 0x00000001, gf100_msppp_new },
	.msvld    = { 0x00000001, gk104_msvld_new },
	.sw       = { 0x00000001, gf100_sw_new },
>>>>>>> upstream/android-13
};

static const struct nvkm_device_chip
nvf1_chipset = {
	.name = "GK110B",
<<<<<<< HEAD
	.bar = gf100_bar_new,
	.bios = nvkm_bios_new,
	.bus = gf100_bus_new,
	.clk = gk104_clk_new,
	.devinit = gf100_devinit_new,
	.fb = gk110_fb_new,
	.fuse = gf100_fuse_new,
	.gpio = gk104_gpio_new,
	.i2c = gk104_i2c_new,
	.ibus = gk104_ibus_new,
	.iccsense = gf100_iccsense_new,
	.imem = nv50_instmem_new,
	.ltc = gk104_ltc_new,
	.mc = gk104_mc_new,
	.mmu = gk104_mmu_new,
	.mxm = nv50_mxm_new,
	.pci = gk104_pci_new,
	.pmu = gk110_pmu_new,
	.therm = gk104_therm_new,
	.timer = nv41_timer_new,
	.top = gk104_top_new,
	.volt = gk104_volt_new,
	.ce[0] = gk104_ce_new,
	.ce[1] = gk104_ce_new,
	.ce[2] = gk104_ce_new,
	.disp = gk110_disp_new,
	.dma = gf119_dma_new,
	.fifo = gk110_fifo_new,
	.gr = gk110b_gr_new,
	.mspdec = gk104_mspdec_new,
	.msppp = gf100_msppp_new,
	.msvld = gk104_msvld_new,
	.sw = gf100_sw_new,
=======
	.bar      = { 0x00000001, gf100_bar_new },
	.bios     = { 0x00000001, nvkm_bios_new },
	.bus      = { 0x00000001, gf100_bus_new },
	.clk      = { 0x00000001, gk104_clk_new },
	.devinit  = { 0x00000001, gf100_devinit_new },
	.fb       = { 0x00000001, gk110_fb_new },
	.fuse     = { 0x00000001, gf100_fuse_new },
	.gpio     = { 0x00000001, gk104_gpio_new },
	.i2c      = { 0x00000001, gk110_i2c_new },
	.iccsense = { 0x00000001, gf100_iccsense_new },
	.imem     = { 0x00000001, nv50_instmem_new },
	.ltc      = { 0x00000001, gk104_ltc_new },
	.mc       = { 0x00000001, gk104_mc_new },
	.mmu      = { 0x00000001, gk104_mmu_new },
	.mxm      = { 0x00000001, nv50_mxm_new },
	.pci      = { 0x00000001, gk104_pci_new },
	.pmu      = { 0x00000001, gk110_pmu_new },
	.privring = { 0x00000001, gk104_privring_new },
	.therm    = { 0x00000001, gk104_therm_new },
	.timer    = { 0x00000001, nv41_timer_new },
	.top      = { 0x00000001, gk104_top_new },
	.volt     = { 0x00000001, gk104_volt_new },
	.ce       = { 0x00000007, gk104_ce_new },
	.disp     = { 0x00000001, gk110_disp_new },
	.dma      = { 0x00000001, gf119_dma_new },
	.fifo     = { 0x00000001, gk110_fifo_new },
	.gr       = { 0x00000001, gk110b_gr_new },
	.mspdec   = { 0x00000001, gk104_mspdec_new },
	.msppp    = { 0x00000001, gf100_msppp_new },
	.msvld    = { 0x00000001, gk104_msvld_new },
	.sw       = { 0x00000001, gf100_sw_new },
>>>>>>> upstream/android-13
};

static const struct nvkm_device_chip
nv106_chipset = {
	.name = "GK208B",
<<<<<<< HEAD
	.bar = gf100_bar_new,
	.bios = nvkm_bios_new,
	.bus = gf100_bus_new,
	.clk = gk104_clk_new,
	.devinit = gf100_devinit_new,
	.fb = gk110_fb_new,
	.fuse = gf100_fuse_new,
	.gpio = gk104_gpio_new,
	.i2c = gk104_i2c_new,
	.ibus = gk104_ibus_new,
	.iccsense = gf100_iccsense_new,
	.imem = nv50_instmem_new,
	.ltc = gk104_ltc_new,
	.mc = gk20a_mc_new,
	.mmu = gk104_mmu_new,
	.mxm = nv50_mxm_new,
	.pci = gk104_pci_new,
	.pmu = gk208_pmu_new,
	.therm = gk104_therm_new,
	.timer = nv41_timer_new,
	.top = gk104_top_new,
	.volt = gk104_volt_new,
	.ce[0] = gk104_ce_new,
	.ce[1] = gk104_ce_new,
	.ce[2] = gk104_ce_new,
	.disp = gk110_disp_new,
	.dma = gf119_dma_new,
	.fifo = gk208_fifo_new,
	.gr = gk208_gr_new,
	.mspdec = gk104_mspdec_new,
	.msppp = gf100_msppp_new,
	.msvld = gk104_msvld_new,
	.sw = gf100_sw_new,
=======
	.bar      = { 0x00000001, gf100_bar_new },
	.bios     = { 0x00000001, nvkm_bios_new },
	.bus      = { 0x00000001, gf100_bus_new },
	.clk      = { 0x00000001, gk104_clk_new },
	.devinit  = { 0x00000001, gf100_devinit_new },
	.fb       = { 0x00000001, gk110_fb_new },
	.fuse     = { 0x00000001, gf100_fuse_new },
	.gpio     = { 0x00000001, gk104_gpio_new },
	.i2c      = { 0x00000001, gk110_i2c_new },
	.iccsense = { 0x00000001, gf100_iccsense_new },
	.imem     = { 0x00000001, nv50_instmem_new },
	.ltc      = { 0x00000001, gk104_ltc_new },
	.mc       = { 0x00000001, gk20a_mc_new },
	.mmu      = { 0x00000001, gk104_mmu_new },
	.mxm      = { 0x00000001, nv50_mxm_new },
	.pci      = { 0x00000001, gk104_pci_new },
	.pmu      = { 0x00000001, gk208_pmu_new },
	.privring = { 0x00000001, gk104_privring_new },
	.therm    = { 0x00000001, gk104_therm_new },
	.timer    = { 0x00000001, nv41_timer_new },
	.top      = { 0x00000001, gk104_top_new },
	.volt     = { 0x00000001, gk104_volt_new },
	.ce       = { 0x00000007, gk104_ce_new },
	.disp     = { 0x00000001, gk110_disp_new },
	.dma      = { 0x00000001, gf119_dma_new },
	.fifo     = { 0x00000001, gk208_fifo_new },
	.gr       = { 0x00000001, gk208_gr_new },
	.mspdec   = { 0x00000001, gk104_mspdec_new },
	.msppp    = { 0x00000001, gf100_msppp_new },
	.msvld    = { 0x00000001, gk104_msvld_new },
	.sw       = { 0x00000001, gf100_sw_new },
>>>>>>> upstream/android-13
};

static const struct nvkm_device_chip
nv108_chipset = {
	.name = "GK208",
<<<<<<< HEAD
	.bar = gf100_bar_new,
	.bios = nvkm_bios_new,
	.bus = gf100_bus_new,
	.clk = gk104_clk_new,
	.devinit = gf100_devinit_new,
	.fb = gk110_fb_new,
	.fuse = gf100_fuse_new,
	.gpio = gk104_gpio_new,
	.i2c = gk104_i2c_new,
	.ibus = gk104_ibus_new,
	.iccsense = gf100_iccsense_new,
	.imem = nv50_instmem_new,
	.ltc = gk104_ltc_new,
	.mc = gk20a_mc_new,
	.mmu = gk104_mmu_new,
	.mxm = nv50_mxm_new,
	.pci = gk104_pci_new,
	.pmu = gk208_pmu_new,
	.therm = gk104_therm_new,
	.timer = nv41_timer_new,
	.top = gk104_top_new,
	.volt = gk104_volt_new,
	.ce[0] = gk104_ce_new,
	.ce[1] = gk104_ce_new,
	.ce[2] = gk104_ce_new,
	.disp = gk110_disp_new,
	.dma = gf119_dma_new,
	.fifo = gk208_fifo_new,
	.gr = gk208_gr_new,
	.mspdec = gk104_mspdec_new,
	.msppp = gf100_msppp_new,
	.msvld = gk104_msvld_new,
	.sw = gf100_sw_new,
=======
	.bar      = { 0x00000001, gf100_bar_new },
	.bios     = { 0x00000001, nvkm_bios_new },
	.bus      = { 0x00000001, gf100_bus_new },
	.clk      = { 0x00000001, gk104_clk_new },
	.devinit  = { 0x00000001, gf100_devinit_new },
	.fb       = { 0x00000001, gk110_fb_new },
	.fuse     = { 0x00000001, gf100_fuse_new },
	.gpio     = { 0x00000001, gk104_gpio_new },
	.i2c      = { 0x00000001, gk110_i2c_new },
	.iccsense = { 0x00000001, gf100_iccsense_new },
	.imem     = { 0x00000001, nv50_instmem_new },
	.ltc      = { 0x00000001, gk104_ltc_new },
	.mc       = { 0x00000001, gk20a_mc_new },
	.mmu      = { 0x00000001, gk104_mmu_new },
	.mxm      = { 0x00000001, nv50_mxm_new },
	.pci      = { 0x00000001, gk104_pci_new },
	.pmu      = { 0x00000001, gk208_pmu_new },
	.privring = { 0x00000001, gk104_privring_new },
	.therm    = { 0x00000001, gk104_therm_new },
	.timer    = { 0x00000001, nv41_timer_new },
	.top      = { 0x00000001, gk104_top_new },
	.volt     = { 0x00000001, gk104_volt_new },
	.ce       = { 0x00000007, gk104_ce_new },
	.disp     = { 0x00000001, gk110_disp_new },
	.dma      = { 0x00000001, gf119_dma_new },
	.fifo     = { 0x00000001, gk208_fifo_new },
	.gr       = { 0x00000001, gk208_gr_new },
	.mspdec   = { 0x00000001, gk104_mspdec_new },
	.msppp    = { 0x00000001, gf100_msppp_new },
	.msvld    = { 0x00000001, gk104_msvld_new },
	.sw       = { 0x00000001, gf100_sw_new },
>>>>>>> upstream/android-13
};

static const struct nvkm_device_chip
nv117_chipset = {
	.name = "GM107",
<<<<<<< HEAD
	.bar = gm107_bar_new,
	.bios = nvkm_bios_new,
	.bus = gf100_bus_new,
	.clk = gk104_clk_new,
	.devinit = gm107_devinit_new,
	.fb = gm107_fb_new,
	.fuse = gm107_fuse_new,
	.gpio = gk104_gpio_new,
	.i2c = gk104_i2c_new,
	.ibus = gk104_ibus_new,
	.iccsense = gf100_iccsense_new,
	.imem = nv50_instmem_new,
	.ltc = gm107_ltc_new,
	.mc = gk20a_mc_new,
	.mmu = gk104_mmu_new,
	.mxm = nv50_mxm_new,
	.pci = gk104_pci_new,
	.pmu = gm107_pmu_new,
	.therm = gm107_therm_new,
	.timer = gk20a_timer_new,
	.top = gk104_top_new,
	.volt = gk104_volt_new,
	.ce[0] = gm107_ce_new,
	.ce[2] = gm107_ce_new,
	.disp = gm107_disp_new,
	.dma = gf119_dma_new,
	.fifo = gm107_fifo_new,
	.gr = gm107_gr_new,
	.sw = gf100_sw_new,
=======
	.bar      = { 0x00000001, gm107_bar_new },
	.bios     = { 0x00000001, nvkm_bios_new },
	.bus      = { 0x00000001, gf100_bus_new },
	.clk      = { 0x00000001, gk104_clk_new },
	.devinit  = { 0x00000001, gm107_devinit_new },
	.fb       = { 0x00000001, gm107_fb_new },
	.fuse     = { 0x00000001, gm107_fuse_new },
	.gpio     = { 0x00000001, gk104_gpio_new },
	.i2c      = { 0x00000001, gk110_i2c_new },
	.iccsense = { 0x00000001, gf100_iccsense_new },
	.imem     = { 0x00000001, nv50_instmem_new },
	.ltc      = { 0x00000001, gm107_ltc_new },
	.mc       = { 0x00000001, gk20a_mc_new },
	.mmu      = { 0x00000001, gk104_mmu_new },
	.mxm      = { 0x00000001, nv50_mxm_new },
	.pci      = { 0x00000001, gk104_pci_new },
	.pmu      = { 0x00000001, gm107_pmu_new },
	.privring = { 0x00000001, gk104_privring_new },
	.therm    = { 0x00000001, gm107_therm_new },
	.timer    = { 0x00000001, gk20a_timer_new },
	.top      = { 0x00000001, gk104_top_new },
	.volt     = { 0x00000001, gk104_volt_new },
	.ce       = { 0x00000005, gm107_ce_new },
	.disp     = { 0x00000001, gm107_disp_new },
	.dma      = { 0x00000001, gf119_dma_new },
	.fifo     = { 0x00000001, gm107_fifo_new },
	.gr       = { 0x00000001, gm107_gr_new },
	.nvdec    = { 0x00000001, gm107_nvdec_new },
	.nvenc    = { 0x00000001, gm107_nvenc_new },
	.sw       = { 0x00000001, gf100_sw_new },
>>>>>>> upstream/android-13
};

static const struct nvkm_device_chip
nv118_chipset = {
	.name = "GM108",
<<<<<<< HEAD
	.bar = gm107_bar_new,
	.bios = nvkm_bios_new,
	.bus = gf100_bus_new,
	.clk = gk104_clk_new,
	.devinit = gm107_devinit_new,
	.fb = gm107_fb_new,
	.fuse = gm107_fuse_new,
	.gpio = gk104_gpio_new,
	.i2c = gk104_i2c_new,
	.ibus = gk104_ibus_new,
	.iccsense = gf100_iccsense_new,
	.imem = nv50_instmem_new,
	.ltc = gm107_ltc_new,
	.mc = gk20a_mc_new,
	.mmu = gk104_mmu_new,
	.mxm = nv50_mxm_new,
	.pci = gk104_pci_new,
	.pmu = gm107_pmu_new,
	.therm = gm107_therm_new,
	.timer = gk20a_timer_new,
	.top = gk104_top_new,
	.volt = gk104_volt_new,
	.ce[0] = gm107_ce_new,
	.ce[2] = gm107_ce_new,
	.disp = gm107_disp_new,
	.dma = gf119_dma_new,
	.fifo = gm107_fifo_new,
	.gr = gm107_gr_new,
	.sw = gf100_sw_new,
=======
	.bar      = { 0x00000001, gm107_bar_new },
	.bios     = { 0x00000001, nvkm_bios_new },
	.bus      = { 0x00000001, gf100_bus_new },
	.clk      = { 0x00000001, gk104_clk_new },
	.devinit  = { 0x00000001, gm107_devinit_new },
	.fb       = { 0x00000001, gm107_fb_new },
	.fuse     = { 0x00000001, gm107_fuse_new },
	.gpio     = { 0x00000001, gk104_gpio_new },
	.i2c      = { 0x00000001, gk110_i2c_new },
	.iccsense = { 0x00000001, gf100_iccsense_new },
	.imem     = { 0x00000001, nv50_instmem_new },
	.ltc      = { 0x00000001, gm107_ltc_new },
	.mc       = { 0x00000001, gk20a_mc_new },
	.mmu      = { 0x00000001, gk104_mmu_new },
	.mxm      = { 0x00000001, nv50_mxm_new },
	.pci      = { 0x00000001, gk104_pci_new },
	.pmu      = { 0x00000001, gm107_pmu_new },
	.privring = { 0x00000001, gk104_privring_new },
	.therm    = { 0x00000001, gm107_therm_new },
	.timer    = { 0x00000001, gk20a_timer_new },
	.top      = { 0x00000001, gk104_top_new },
	.volt     = { 0x00000001, gk104_volt_new },
	.ce       = { 0x00000005, gm107_ce_new },
	.disp     = { 0x00000001, gm107_disp_new },
	.dma      = { 0x00000001, gf119_dma_new },
	.fifo     = { 0x00000001, gm107_fifo_new },
	.gr       = { 0x00000001, gm107_gr_new },
	.sw       = { 0x00000001, gf100_sw_new },
>>>>>>> upstream/android-13
};

static const struct nvkm_device_chip
nv120_chipset = {
	.name = "GM200",
<<<<<<< HEAD
	.bar = gm107_bar_new,
	.bios = nvkm_bios_new,
	.bus = gf100_bus_new,
	.devinit = gm200_devinit_new,
	.fb = gm200_fb_new,
	.fuse = gm107_fuse_new,
	.gpio = gk104_gpio_new,
	.i2c = gm200_i2c_new,
	.ibus = gm200_ibus_new,
	.iccsense = gf100_iccsense_new,
	.imem = nv50_instmem_new,
	.ltc = gm200_ltc_new,
	.mc = gk20a_mc_new,
	.mmu = gm200_mmu_new,
	.mxm = nv50_mxm_new,
	.pci = gk104_pci_new,
	.pmu = gm107_pmu_new,
	.therm = gm200_therm_new,
	.secboot = gm200_secboot_new,
	.timer = gk20a_timer_new,
	.top = gk104_top_new,
	.volt = gk104_volt_new,
	.ce[0] = gm200_ce_new,
	.ce[1] = gm200_ce_new,
	.ce[2] = gm200_ce_new,
	.disp = gm200_disp_new,
	.dma = gf119_dma_new,
	.fifo = gm200_fifo_new,
	.gr = gm200_gr_new,
	.sw = gf100_sw_new,
=======
	.acr      = { 0x00000001, gm200_acr_new },
	.bar      = { 0x00000001, gm107_bar_new },
	.bios     = { 0x00000001, nvkm_bios_new },
	.bus      = { 0x00000001, gf100_bus_new },
	.devinit  = { 0x00000001, gm200_devinit_new },
	.fb       = { 0x00000001, gm200_fb_new },
	.fuse     = { 0x00000001, gm107_fuse_new },
	.gpio     = { 0x00000001, gk104_gpio_new },
	.i2c      = { 0x00000001, gm200_i2c_new },
	.iccsense = { 0x00000001, gf100_iccsense_new },
	.imem     = { 0x00000001, nv50_instmem_new },
	.ltc      = { 0x00000001, gm200_ltc_new },
	.mc       = { 0x00000001, gk20a_mc_new },
	.mmu      = { 0x00000001, gm200_mmu_new },
	.mxm      = { 0x00000001, nv50_mxm_new },
	.pci      = { 0x00000001, gk104_pci_new },
	.pmu      = { 0x00000001, gm200_pmu_new },
	.privring = { 0x00000001, gm200_privring_new },
	.therm    = { 0x00000001, gm200_therm_new },
	.timer    = { 0x00000001, gk20a_timer_new },
	.top      = { 0x00000001, gk104_top_new },
	.volt     = { 0x00000001, gk104_volt_new },
	.ce       = { 0x00000007, gm200_ce_new },
	.disp     = { 0x00000001, gm200_disp_new },
	.dma      = { 0x00000001, gf119_dma_new },
	.fifo     = { 0x00000001, gm200_fifo_new },
	.gr       = { 0x00000001, gm200_gr_new },
	.nvdec    = { 0x00000001, gm107_nvdec_new },
	.nvenc    = { 0x00000003, gm107_nvenc_new },
	.sw       = { 0x00000001, gf100_sw_new },
>>>>>>> upstream/android-13
};

static const struct nvkm_device_chip
nv124_chipset = {
	.name = "GM204",
<<<<<<< HEAD
	.bar = gm107_bar_new,
	.bios = nvkm_bios_new,
	.bus = gf100_bus_new,
	.devinit = gm200_devinit_new,
	.fb = gm200_fb_new,
	.fuse = gm107_fuse_new,
	.gpio = gk104_gpio_new,
	.i2c = gm200_i2c_new,
	.ibus = gm200_ibus_new,
	.iccsense = gf100_iccsense_new,
	.imem = nv50_instmem_new,
	.ltc = gm200_ltc_new,
	.mc = gk20a_mc_new,
	.mmu = gm200_mmu_new,
	.mxm = nv50_mxm_new,
	.pci = gk104_pci_new,
	.pmu = gm107_pmu_new,
	.therm = gm200_therm_new,
	.secboot = gm200_secboot_new,
	.timer = gk20a_timer_new,
	.top = gk104_top_new,
	.volt = gk104_volt_new,
	.ce[0] = gm200_ce_new,
	.ce[1] = gm200_ce_new,
	.ce[2] = gm200_ce_new,
	.disp = gm200_disp_new,
	.dma = gf119_dma_new,
	.fifo = gm200_fifo_new,
	.gr = gm200_gr_new,
	.sw = gf100_sw_new,
=======
	.acr      = { 0x00000001, gm200_acr_new },
	.bar      = { 0x00000001, gm107_bar_new },
	.bios     = { 0x00000001, nvkm_bios_new },
	.bus      = { 0x00000001, gf100_bus_new },
	.devinit  = { 0x00000001, gm200_devinit_new },
	.fb       = { 0x00000001, gm200_fb_new },
	.fuse     = { 0x00000001, gm107_fuse_new },
	.gpio     = { 0x00000001, gk104_gpio_new },
	.i2c      = { 0x00000001, gm200_i2c_new },
	.iccsense = { 0x00000001, gf100_iccsense_new },
	.imem     = { 0x00000001, nv50_instmem_new },
	.ltc      = { 0x00000001, gm200_ltc_new },
	.mc       = { 0x00000001, gk20a_mc_new },
	.mmu      = { 0x00000001, gm200_mmu_new },
	.mxm      = { 0x00000001, nv50_mxm_new },
	.pci      = { 0x00000001, gk104_pci_new },
	.pmu      = { 0x00000001, gm200_pmu_new },
	.privring = { 0x00000001, gm200_privring_new },
	.therm    = { 0x00000001, gm200_therm_new },
	.timer    = { 0x00000001, gk20a_timer_new },
	.top      = { 0x00000001, gk104_top_new },
	.volt     = { 0x00000001, gk104_volt_new },
	.ce       = { 0x00000007, gm200_ce_new },
	.disp     = { 0x00000001, gm200_disp_new },
	.dma      = { 0x00000001, gf119_dma_new },
	.fifo     = { 0x00000001, gm200_fifo_new },
	.gr       = { 0x00000001, gm200_gr_new },
	.nvdec    = { 0x00000001, gm107_nvdec_new },
	.nvenc    = { 0x00000003, gm107_nvenc_new },
	.sw       = { 0x00000001, gf100_sw_new },
>>>>>>> upstream/android-13
};

static const struct nvkm_device_chip
nv126_chipset = {
	.name = "GM206",
<<<<<<< HEAD
	.bar = gm107_bar_new,
	.bios = nvkm_bios_new,
	.bus = gf100_bus_new,
	.devinit = gm200_devinit_new,
	.fb = gm200_fb_new,
	.fuse = gm107_fuse_new,
	.gpio = gk104_gpio_new,
	.i2c = gm200_i2c_new,
	.ibus = gm200_ibus_new,
	.iccsense = gf100_iccsense_new,
	.imem = nv50_instmem_new,
	.ltc = gm200_ltc_new,
	.mc = gk20a_mc_new,
	.mmu = gm200_mmu_new,
	.mxm = nv50_mxm_new,
	.pci = gk104_pci_new,
	.pmu = gm107_pmu_new,
	.therm = gm200_therm_new,
	.secboot = gm200_secboot_new,
	.timer = gk20a_timer_new,
	.top = gk104_top_new,
	.volt = gk104_volt_new,
	.ce[0] = gm200_ce_new,
	.ce[1] = gm200_ce_new,
	.ce[2] = gm200_ce_new,
	.disp = gm200_disp_new,
	.dma = gf119_dma_new,
	.fifo = gm200_fifo_new,
	.gr = gm200_gr_new,
	.sw = gf100_sw_new,
=======
	.acr      = { 0x00000001, gm200_acr_new },
	.bar      = { 0x00000001, gm107_bar_new },
	.bios     = { 0x00000001, nvkm_bios_new },
	.bus      = { 0x00000001, gf100_bus_new },
	.devinit  = { 0x00000001, gm200_devinit_new },
	.fb       = { 0x00000001, gm200_fb_new },
	.fuse     = { 0x00000001, gm107_fuse_new },
	.gpio     = { 0x00000001, gk104_gpio_new },
	.i2c      = { 0x00000001, gm200_i2c_new },
	.iccsense = { 0x00000001, gf100_iccsense_new },
	.imem     = { 0x00000001, nv50_instmem_new },
	.ltc      = { 0x00000001, gm200_ltc_new },
	.mc       = { 0x00000001, gk20a_mc_new },
	.mmu      = { 0x00000001, gm200_mmu_new },
	.mxm      = { 0x00000001, nv50_mxm_new },
	.pci      = { 0x00000001, gk104_pci_new },
	.pmu      = { 0x00000001, gm200_pmu_new },
	.privring = { 0x00000001, gm200_privring_new },
	.therm    = { 0x00000001, gm200_therm_new },
	.timer    = { 0x00000001, gk20a_timer_new },
	.top      = { 0x00000001, gk104_top_new },
	.volt     = { 0x00000001, gk104_volt_new },
	.ce       = { 0x00000007, gm200_ce_new },
	.disp     = { 0x00000001, gm200_disp_new },
	.dma      = { 0x00000001, gf119_dma_new },
	.fifo     = { 0x00000001, gm200_fifo_new },
	.gr       = { 0x00000001, gm200_gr_new },
	.nvdec    = { 0x00000001, gm107_nvdec_new },
	.nvenc    = { 0x00000001, gm107_nvenc_new },
	.sw       = { 0x00000001, gf100_sw_new },
>>>>>>> upstream/android-13
};

static const struct nvkm_device_chip
nv12b_chipset = {
	.name = "GM20B",
<<<<<<< HEAD
	.bar = gm20b_bar_new,
	.bus = gf100_bus_new,
	.clk = gm20b_clk_new,
	.fb = gm20b_fb_new,
	.fuse = gm107_fuse_new,
	.ibus = gk20a_ibus_new,
	.imem = gk20a_instmem_new,
	.ltc = gm200_ltc_new,
	.mc = gk20a_mc_new,
	.mmu = gm20b_mmu_new,
	.pmu = gm20b_pmu_new,
	.secboot = gm20b_secboot_new,
	.timer = gk20a_timer_new,
	.top = gk104_top_new,
	.ce[2] = gm200_ce_new,
	.volt = gm20b_volt_new,
	.dma = gf119_dma_new,
	.fifo = gm20b_fifo_new,
	.gr = gm20b_gr_new,
	.sw = gf100_sw_new,
=======
	.acr      = { 0x00000001, gm20b_acr_new },
	.bar      = { 0x00000001, gm20b_bar_new },
	.bus      = { 0x00000001, gf100_bus_new },
	.clk      = { 0x00000001, gm20b_clk_new },
	.fb       = { 0x00000001, gm20b_fb_new },
	.fuse     = { 0x00000001, gm107_fuse_new },
	.imem     = { 0x00000001, gk20a_instmem_new },
	.ltc      = { 0x00000001, gm200_ltc_new },
	.mc       = { 0x00000001, gk20a_mc_new },
	.mmu      = { 0x00000001, gm20b_mmu_new },
	.pmu      = { 0x00000001, gm20b_pmu_new },
	.privring = { 0x00000001, gk20a_privring_new },
	.timer    = { 0x00000001, gk20a_timer_new },
	.top      = { 0x00000001, gk104_top_new },
	.volt     = { 0x00000001, gm20b_volt_new },
	.ce       = { 0x00000004, gm200_ce_new },
	.dma      = { 0x00000001, gf119_dma_new },
	.fifo     = { 0x00000001, gm20b_fifo_new },
	.gr       = { 0x00000001, gm20b_gr_new },
	.sw       = { 0x00000001, gf100_sw_new },
>>>>>>> upstream/android-13
};

static const struct nvkm_device_chip
nv130_chipset = {
	.name = "GP100",
<<<<<<< HEAD
	.bar = gm107_bar_new,
	.bios = nvkm_bios_new,
	.bus = gf100_bus_new,
	.devinit = gm200_devinit_new,
	.fault = gp100_fault_new,
	.fb = gp100_fb_new,
	.fuse = gm107_fuse_new,
	.gpio = gk104_gpio_new,
	.i2c = gm200_i2c_new,
	.ibus = gm200_ibus_new,
	.imem = nv50_instmem_new,
	.ltc = gp100_ltc_new,
	.mc = gp100_mc_new,
	.mmu = gp100_mmu_new,
	.therm = gp100_therm_new,
	.secboot = gm200_secboot_new,
	.pci = gp100_pci_new,
	.pmu = gp100_pmu_new,
	.timer = gk20a_timer_new,
	.top = gk104_top_new,
	.ce[0] = gp100_ce_new,
	.ce[1] = gp100_ce_new,
	.ce[2] = gp100_ce_new,
	.ce[3] = gp100_ce_new,
	.ce[4] = gp100_ce_new,
	.ce[5] = gp100_ce_new,
	.dma = gf119_dma_new,
	.disp = gp100_disp_new,
	.fifo = gp100_fifo_new,
	.gr = gp100_gr_new,
	.sw = gf100_sw_new,
=======
	.acr      = { 0x00000001, gm200_acr_new },
	.bar      = { 0x00000001, gm107_bar_new },
	.bios     = { 0x00000001, nvkm_bios_new },
	.bus      = { 0x00000001, gf100_bus_new },
	.devinit  = { 0x00000001, gm200_devinit_new },
	.fault    = { 0x00000001, gp100_fault_new },
	.fb       = { 0x00000001, gp100_fb_new },
	.fuse     = { 0x00000001, gm107_fuse_new },
	.gpio     = { 0x00000001, gk104_gpio_new },
	.i2c      = { 0x00000001, gm200_i2c_new },
	.imem     = { 0x00000001, nv50_instmem_new },
	.ltc      = { 0x00000001, gp100_ltc_new },
	.mc       = { 0x00000001, gp100_mc_new },
	.mmu      = { 0x00000001, gp100_mmu_new },
	.therm    = { 0x00000001, gp100_therm_new },
	.pci      = { 0x00000001, gp100_pci_new },
	.pmu      = { 0x00000001, gm200_pmu_new },
	.privring = { 0x00000001, gm200_privring_new },
	.timer    = { 0x00000001, gk20a_timer_new },
	.top      = { 0x00000001, gk104_top_new },
	.ce       = { 0x0000003f, gp100_ce_new },
	.dma      = { 0x00000001, gf119_dma_new },
	.disp     = { 0x00000001, gp100_disp_new },
	.fifo     = { 0x00000001, gp100_fifo_new },
	.gr       = { 0x00000001, gp100_gr_new },
	.nvdec    = { 0x00000001, gm107_nvdec_new },
	.nvenc    = { 0x00000007, gm107_nvenc_new },
	.sw       = { 0x00000001, gf100_sw_new },
>>>>>>> upstream/android-13
};

static const struct nvkm_device_chip
nv132_chipset = {
	.name = "GP102",
<<<<<<< HEAD
	.bar = gm107_bar_new,
	.bios = nvkm_bios_new,
	.bus = gf100_bus_new,
	.devinit = gm200_devinit_new,
	.fault = gp100_fault_new,
	.fb = gp102_fb_new,
	.fuse = gm107_fuse_new,
	.gpio = gk104_gpio_new,
	.i2c = gm200_i2c_new,
	.ibus = gm200_ibus_new,
	.imem = nv50_instmem_new,
	.ltc = gp102_ltc_new,
	.mc = gp100_mc_new,
	.mmu = gp100_mmu_new,
	.therm = gp100_therm_new,
	.secboot = gp102_secboot_new,
	.pci = gp100_pci_new,
	.pmu = gp102_pmu_new,
	.timer = gk20a_timer_new,
	.top = gk104_top_new,
	.ce[0] = gp102_ce_new,
	.ce[1] = gp102_ce_new,
	.ce[2] = gp102_ce_new,
	.ce[3] = gp102_ce_new,
	.disp = gp102_disp_new,
	.dma = gf119_dma_new,
	.fifo = gp100_fifo_new,
	.gr = gp102_gr_new,
	.nvdec = gp102_nvdec_new,
	.sec2 = gp102_sec2_new,
	.sw = gf100_sw_new,
=======
	.acr      = { 0x00000001, gp102_acr_new },
	.bar      = { 0x00000001, gm107_bar_new },
	.bios     = { 0x00000001, nvkm_bios_new },
	.bus      = { 0x00000001, gf100_bus_new },
	.devinit  = { 0x00000001, gm200_devinit_new },
	.fault    = { 0x00000001, gp100_fault_new },
	.fb       = { 0x00000001, gp102_fb_new },
	.fuse     = { 0x00000001, gm107_fuse_new },
	.gpio     = { 0x00000001, gk104_gpio_new },
	.i2c      = { 0x00000001, gm200_i2c_new },
	.imem     = { 0x00000001, nv50_instmem_new },
	.ltc      = { 0x00000001, gp102_ltc_new },
	.mc       = { 0x00000001, gp100_mc_new },
	.mmu      = { 0x00000001, gp100_mmu_new },
	.therm    = { 0x00000001, gp100_therm_new },
	.pci      = { 0x00000001, gp100_pci_new },
	.pmu      = { 0x00000001, gp102_pmu_new },
	.privring = { 0x00000001, gm200_privring_new },
	.timer    = { 0x00000001, gk20a_timer_new },
	.top      = { 0x00000001, gk104_top_new },
	.ce       = { 0x0000000f, gp102_ce_new },
	.disp     = { 0x00000001, gp102_disp_new },
	.dma      = { 0x00000001, gf119_dma_new },
	.fifo     = { 0x00000001, gp100_fifo_new },
	.gr       = { 0x00000001, gp102_gr_new },
	.nvdec    = { 0x00000001, gm107_nvdec_new },
	.nvenc    = { 0x00000003, gm107_nvenc_new },
	.sec2     = { 0x00000001, gp102_sec2_new },
	.sw       = { 0x00000001, gf100_sw_new },
>>>>>>> upstream/android-13
};

static const struct nvkm_device_chip
nv134_chipset = {
	.name = "GP104",
<<<<<<< HEAD
	.bar = gm107_bar_new,
	.bios = nvkm_bios_new,
	.bus = gf100_bus_new,
	.devinit = gm200_devinit_new,
	.fault = gp100_fault_new,
	.fb = gp102_fb_new,
	.fuse = gm107_fuse_new,
	.gpio = gk104_gpio_new,
	.i2c = gm200_i2c_new,
	.ibus = gm200_ibus_new,
	.imem = nv50_instmem_new,
	.ltc = gp102_ltc_new,
	.mc = gp100_mc_new,
	.mmu = gp100_mmu_new,
	.therm = gp100_therm_new,
	.secboot = gp102_secboot_new,
	.pci = gp100_pci_new,
	.pmu = gp102_pmu_new,
	.timer = gk20a_timer_new,
	.top = gk104_top_new,
	.ce[0] = gp102_ce_new,
	.ce[1] = gp102_ce_new,
	.ce[2] = gp102_ce_new,
	.ce[3] = gp102_ce_new,
	.disp = gp102_disp_new,
	.dma = gf119_dma_new,
	.fifo = gp100_fifo_new,
	.gr = gp104_gr_new,
	.nvdec = gp102_nvdec_new,
	.sec2 = gp102_sec2_new,
	.sw = gf100_sw_new,
=======
	.acr      = { 0x00000001, gp102_acr_new },
	.bar      = { 0x00000001, gm107_bar_new },
	.bios     = { 0x00000001, nvkm_bios_new },
	.bus      = { 0x00000001, gf100_bus_new },
	.devinit  = { 0x00000001, gm200_devinit_new },
	.fault    = { 0x00000001, gp100_fault_new },
	.fb       = { 0x00000001, gp102_fb_new },
	.fuse     = { 0x00000001, gm107_fuse_new },
	.gpio     = { 0x00000001, gk104_gpio_new },
	.i2c      = { 0x00000001, gm200_i2c_new },
	.imem     = { 0x00000001, nv50_instmem_new },
	.ltc      = { 0x00000001, gp102_ltc_new },
	.mc       = { 0x00000001, gp100_mc_new },
	.mmu      = { 0x00000001, gp100_mmu_new },
	.therm    = { 0x00000001, gp100_therm_new },
	.pci      = { 0x00000001, gp100_pci_new },
	.pmu      = { 0x00000001, gp102_pmu_new },
	.privring = { 0x00000001, gm200_privring_new },
	.timer    = { 0x00000001, gk20a_timer_new },
	.top      = { 0x00000001, gk104_top_new },
	.ce       = { 0x0000000f, gp102_ce_new },
	.disp     = { 0x00000001, gp102_disp_new },
	.dma      = { 0x00000001, gf119_dma_new },
	.fifo     = { 0x00000001, gp100_fifo_new },
	.gr       = { 0x00000001, gp104_gr_new },
	.nvdec    = { 0x00000001, gm107_nvdec_new },
	.nvenc    = { 0x00000003, gm107_nvenc_new },
	.sec2     = { 0x00000001, gp102_sec2_new },
	.sw       = { 0x00000001, gf100_sw_new },
>>>>>>> upstream/android-13
};

static const struct nvkm_device_chip
nv136_chipset = {
	.name = "GP106",
<<<<<<< HEAD
	.bar = gm107_bar_new,
	.bios = nvkm_bios_new,
	.bus = gf100_bus_new,
	.devinit = gm200_devinit_new,
	.fault = gp100_fault_new,
	.fb = gp102_fb_new,
	.fuse = gm107_fuse_new,
	.gpio = gk104_gpio_new,
	.i2c = gm200_i2c_new,
	.ibus = gm200_ibus_new,
	.imem = nv50_instmem_new,
	.ltc = gp102_ltc_new,
	.mc = gp100_mc_new,
	.mmu = gp100_mmu_new,
	.therm = gp100_therm_new,
	.secboot = gp102_secboot_new,
	.pci = gp100_pci_new,
	.pmu = gp102_pmu_new,
	.timer = gk20a_timer_new,
	.top = gk104_top_new,
	.ce[0] = gp102_ce_new,
	.ce[1] = gp102_ce_new,
	.ce[2] = gp102_ce_new,
	.ce[3] = gp102_ce_new,
	.disp = gp102_disp_new,
	.dma = gf119_dma_new,
	.fifo = gp100_fifo_new,
	.gr = gp104_gr_new,
	.nvdec = gp102_nvdec_new,
	.sec2 = gp102_sec2_new,
	.sw = gf100_sw_new,
=======
	.acr      = { 0x00000001, gp102_acr_new },
	.bar      = { 0x00000001, gm107_bar_new },
	.bios     = { 0x00000001, nvkm_bios_new },
	.bus      = { 0x00000001, gf100_bus_new },
	.devinit  = { 0x00000001, gm200_devinit_new },
	.fault    = { 0x00000001, gp100_fault_new },
	.fb       = { 0x00000001, gp102_fb_new },
	.fuse     = { 0x00000001, gm107_fuse_new },
	.gpio     = { 0x00000001, gk104_gpio_new },
	.i2c      = { 0x00000001, gm200_i2c_new },
	.imem     = { 0x00000001, nv50_instmem_new },
	.ltc      = { 0x00000001, gp102_ltc_new },
	.mc       = { 0x00000001, gp100_mc_new },
	.mmu      = { 0x00000001, gp100_mmu_new },
	.therm    = { 0x00000001, gp100_therm_new },
	.pci      = { 0x00000001, gp100_pci_new },
	.pmu      = { 0x00000001, gp102_pmu_new },
	.privring = { 0x00000001, gm200_privring_new },
	.timer    = { 0x00000001, gk20a_timer_new },
	.top      = { 0x00000001, gk104_top_new },
	.ce       = { 0x0000000f, gp102_ce_new },
	.disp     = { 0x00000001, gp102_disp_new },
	.dma      = { 0x00000001, gf119_dma_new },
	.fifo     = { 0x00000001, gp100_fifo_new },
	.gr       = { 0x00000001, gp104_gr_new },
	.nvdec    = { 0x00000001, gm107_nvdec_new },
	.nvenc    = { 0x00000001, gm107_nvenc_new },
	.sec2     = { 0x00000001, gp102_sec2_new },
	.sw       = { 0x00000001, gf100_sw_new },
>>>>>>> upstream/android-13
};

static const struct nvkm_device_chip
nv137_chipset = {
	.name = "GP107",
<<<<<<< HEAD
	.bar = gm107_bar_new,
	.bios = nvkm_bios_new,
	.bus = gf100_bus_new,
	.devinit = gm200_devinit_new,
	.fault = gp100_fault_new,
	.fb = gp102_fb_new,
	.fuse = gm107_fuse_new,
	.gpio = gk104_gpio_new,
	.i2c = gm200_i2c_new,
	.ibus = gm200_ibus_new,
	.imem = nv50_instmem_new,
	.ltc = gp102_ltc_new,
	.mc = gp100_mc_new,
	.mmu = gp100_mmu_new,
	.therm = gp100_therm_new,
	.secboot = gp102_secboot_new,
	.pci = gp100_pci_new,
	.pmu = gp102_pmu_new,
	.timer = gk20a_timer_new,
	.top = gk104_top_new,
	.ce[0] = gp102_ce_new,
	.ce[1] = gp102_ce_new,
	.ce[2] = gp102_ce_new,
	.ce[3] = gp102_ce_new,
	.disp = gp102_disp_new,
	.dma = gf119_dma_new,
	.fifo = gp100_fifo_new,
	.gr = gp107_gr_new,
	.nvdec = gp102_nvdec_new,
	.sec2 = gp102_sec2_new,
	.sw = gf100_sw_new,
=======
	.acr      = { 0x00000001, gp102_acr_new },
	.bar      = { 0x00000001, gm107_bar_new },
	.bios     = { 0x00000001, nvkm_bios_new },
	.bus      = { 0x00000001, gf100_bus_new },
	.devinit  = { 0x00000001, gm200_devinit_new },
	.fault    = { 0x00000001, gp100_fault_new },
	.fb       = { 0x00000001, gp102_fb_new },
	.fuse     = { 0x00000001, gm107_fuse_new },
	.gpio     = { 0x00000001, gk104_gpio_new },
	.i2c      = { 0x00000001, gm200_i2c_new },
	.imem     = { 0x00000001, nv50_instmem_new },
	.ltc      = { 0x00000001, gp102_ltc_new },
	.mc       = { 0x00000001, gp100_mc_new },
	.mmu      = { 0x00000001, gp100_mmu_new },
	.therm    = { 0x00000001, gp100_therm_new },
	.pci      = { 0x00000001, gp100_pci_new },
	.pmu      = { 0x00000001, gp102_pmu_new },
	.privring = { 0x00000001, gm200_privring_new },
	.timer    = { 0x00000001, gk20a_timer_new },
	.top      = { 0x00000001, gk104_top_new },
	.ce       = { 0x0000000f, gp102_ce_new },
	.disp     = { 0x00000001, gp102_disp_new },
	.dma      = { 0x00000001, gf119_dma_new },
	.fifo     = { 0x00000001, gp100_fifo_new },
	.gr       = { 0x00000001, gp107_gr_new },
	.nvdec    = { 0x00000001, gm107_nvdec_new },
	.nvenc    = { 0x00000003, gm107_nvenc_new },
	.sec2     = { 0x00000001, gp102_sec2_new },
	.sw       = { 0x00000001, gf100_sw_new },
>>>>>>> upstream/android-13
};

static const struct nvkm_device_chip
nv138_chipset = {
	.name = "GP108",
<<<<<<< HEAD
	.bar = gm107_bar_new,
	.bios = nvkm_bios_new,
	.bus = gf100_bus_new,
	.devinit = gm200_devinit_new,
	.fault = gp100_fault_new,
	.fb = gp102_fb_new,
	.fuse = gm107_fuse_new,
	.gpio = gk104_gpio_new,
	.i2c = gm200_i2c_new,
	.ibus = gm200_ibus_new,
	.imem = nv50_instmem_new,
	.ltc = gp102_ltc_new,
	.mc = gp100_mc_new,
	.mmu = gp100_mmu_new,
	.therm = gp100_therm_new,
	.secboot = gp108_secboot_new,
	.pci = gp100_pci_new,
	.pmu = gp102_pmu_new,
	.timer = gk20a_timer_new,
	.top = gk104_top_new,
	.ce[0] = gp102_ce_new,
	.ce[1] = gp102_ce_new,
	.ce[2] = gp102_ce_new,
	.ce[3] = gp102_ce_new,
	.disp = gp102_disp_new,
	.dma = gf119_dma_new,
	.fifo = gp100_fifo_new,
	.gr = gp107_gr_new,
	.nvdec = gp102_nvdec_new,
	.sec2 = gp102_sec2_new,
	.sw = gf100_sw_new,
=======
	.acr      = { 0x00000001, gp108_acr_new },
	.bar      = { 0x00000001, gm107_bar_new },
	.bios     = { 0x00000001, nvkm_bios_new },
	.bus      = { 0x00000001, gf100_bus_new },
	.devinit  = { 0x00000001, gm200_devinit_new },
	.fault    = { 0x00000001, gp100_fault_new },
	.fb       = { 0x00000001, gp102_fb_new },
	.fuse     = { 0x00000001, gm107_fuse_new },
	.gpio     = { 0x00000001, gk104_gpio_new },
	.i2c      = { 0x00000001, gm200_i2c_new },
	.imem     = { 0x00000001, nv50_instmem_new },
	.ltc      = { 0x00000001, gp102_ltc_new },
	.mc       = { 0x00000001, gp100_mc_new },
	.mmu      = { 0x00000001, gp100_mmu_new },
	.therm    = { 0x00000001, gp100_therm_new },
	.pci      = { 0x00000001, gp100_pci_new },
	.pmu      = { 0x00000001, gp102_pmu_new },
	.privring = { 0x00000001, gm200_privring_new },
	.timer    = { 0x00000001, gk20a_timer_new },
	.top      = { 0x00000001, gk104_top_new },
	.ce       = { 0x0000000f, gp102_ce_new },
	.disp     = { 0x00000001, gp102_disp_new },
	.dma      = { 0x00000001, gf119_dma_new },
	.fifo     = { 0x00000001, gp100_fifo_new },
	.gr       = { 0x00000001, gp108_gr_new },
	.nvdec    = { 0x00000001, gm107_nvdec_new },
	.sec2     = { 0x00000001, gp108_sec2_new },
	.sw       = { 0x00000001, gf100_sw_new },
>>>>>>> upstream/android-13
};

static const struct nvkm_device_chip
nv13b_chipset = {
	.name = "GP10B",
<<<<<<< HEAD
	.bar = gm20b_bar_new,
	.bus = gf100_bus_new,
	.fault = gp100_fault_new,
	.fb = gp10b_fb_new,
	.fuse = gm107_fuse_new,
	.ibus = gp10b_ibus_new,
	.imem = gk20a_instmem_new,
	.ltc = gp102_ltc_new,
	.mc = gp10b_mc_new,
	.mmu = gp10b_mmu_new,
	.secboot = gp10b_secboot_new,
	.pmu = gm20b_pmu_new,
	.timer = gk20a_timer_new,
	.top = gk104_top_new,
	.ce[2] = gp102_ce_new,
	.dma = gf119_dma_new,
	.fifo = gp10b_fifo_new,
	.gr = gp10b_gr_new,
	.sw = gf100_sw_new,
=======
	.acr      = { 0x00000001, gp10b_acr_new },
	.bar      = { 0x00000001, gm20b_bar_new },
	.bus      = { 0x00000001, gf100_bus_new },
	.fault    = { 0x00000001, gp10b_fault_new },
	.fb       = { 0x00000001, gp10b_fb_new },
	.fuse     = { 0x00000001, gm107_fuse_new },
	.imem     = { 0x00000001, gk20a_instmem_new },
	.ltc      = { 0x00000001, gp10b_ltc_new },
	.mc       = { 0x00000001, gp10b_mc_new },
	.mmu      = { 0x00000001, gp10b_mmu_new },
	.pmu      = { 0x00000001, gp10b_pmu_new },
	.privring = { 0x00000001, gp10b_privring_new },
	.timer    = { 0x00000001, gk20a_timer_new },
	.top      = { 0x00000001, gk104_top_new },
	.ce       = { 0x00000001, gp100_ce_new },
	.dma      = { 0x00000001, gf119_dma_new },
	.fifo     = { 0x00000001, gp10b_fifo_new },
	.gr       = { 0x00000001, gp10b_gr_new },
	.sw       = { 0x00000001, gf100_sw_new },
>>>>>>> upstream/android-13
};

static const struct nvkm_device_chip
nv140_chipset = {
	.name = "GV100",
<<<<<<< HEAD
	.bar = gm107_bar_new,
	.bios = nvkm_bios_new,
	.bus = gf100_bus_new,
	.devinit = gv100_devinit_new,
	.fault = gv100_fault_new,
	.fb = gv100_fb_new,
	.fuse = gm107_fuse_new,
	.gpio = gk104_gpio_new,
	.i2c = gm200_i2c_new,
	.ibus = gm200_ibus_new,
	.imem = nv50_instmem_new,
	.ltc = gp102_ltc_new,
	.mc = gp100_mc_new,
	.mmu = gv100_mmu_new,
	.pci = gp100_pci_new,
	.pmu = gp102_pmu_new,
	.secboot = gp108_secboot_new,
	.therm = gp100_therm_new,
	.timer = gk20a_timer_new,
	.top = gk104_top_new,
	.disp = gv100_disp_new,
	.ce[0] = gv100_ce_new,
	.ce[1] = gv100_ce_new,
	.ce[2] = gv100_ce_new,
	.ce[3] = gv100_ce_new,
	.ce[4] = gv100_ce_new,
	.ce[5] = gv100_ce_new,
	.ce[6] = gv100_ce_new,
	.ce[7] = gv100_ce_new,
	.ce[8] = gv100_ce_new,
	.dma = gv100_dma_new,
	.fifo = gv100_fifo_new,
	.gr = gv100_gr_new,
	.nvdec = gp102_nvdec_new,
	.sec2 = gp102_sec2_new,
=======
	.acr      = { 0x00000001, gp108_acr_new },
	.bar      = { 0x00000001, gm107_bar_new },
	.bios     = { 0x00000001, nvkm_bios_new },
	.bus      = { 0x00000001, gf100_bus_new },
	.devinit  = { 0x00000001, gv100_devinit_new },
	.fault    = { 0x00000001, gv100_fault_new },
	.fb       = { 0x00000001, gv100_fb_new },
	.fuse     = { 0x00000001, gm107_fuse_new },
	.gpio     = { 0x00000001, gk104_gpio_new },
	.gsp      = { 0x00000001, gv100_gsp_new },
	.i2c      = { 0x00000001, gm200_i2c_new },
	.imem     = { 0x00000001, nv50_instmem_new },
	.ltc      = { 0x00000001, gp102_ltc_new },
	.mc       = { 0x00000001, gp100_mc_new },
	.mmu      = { 0x00000001, gv100_mmu_new },
	.pci      = { 0x00000001, gp100_pci_new },
	.pmu      = { 0x00000001, gp102_pmu_new },
	.privring = { 0x00000001, gm200_privring_new },
	.therm    = { 0x00000001, gp100_therm_new },
	.timer    = { 0x00000001, gk20a_timer_new },
	.top      = { 0x00000001, gk104_top_new },
	.ce       = { 0x000001ff, gv100_ce_new },
	.disp     = { 0x00000001, gv100_disp_new },
	.dma      = { 0x00000001, gv100_dma_new },
	.fifo     = { 0x00000001, gv100_fifo_new },
	.gr       = { 0x00000001, gv100_gr_new },
	.nvdec    = { 0x00000001, gm107_nvdec_new },
	.nvenc    = { 0x00000007, gm107_nvenc_new },
	.sec2     = { 0x00000001, gp108_sec2_new },
};

static const struct nvkm_device_chip
nv162_chipset = {
	.name = "TU102",
	.acr      = { 0x00000001, tu102_acr_new },
	.bar      = { 0x00000001, tu102_bar_new },
	.bios     = { 0x00000001, nvkm_bios_new },
	.bus      = { 0x00000001, gf100_bus_new },
	.devinit  = { 0x00000001, tu102_devinit_new },
	.fault    = { 0x00000001, tu102_fault_new },
	.fb       = { 0x00000001, gv100_fb_new },
	.fuse     = { 0x00000001, gm107_fuse_new },
	.gpio     = { 0x00000001, gk104_gpio_new },
	.gsp      = { 0x00000001, gv100_gsp_new },
	.i2c      = { 0x00000001, gm200_i2c_new },
	.imem     = { 0x00000001, nv50_instmem_new },
	.ltc      = { 0x00000001, gp102_ltc_new },
	.mc       = { 0x00000001, tu102_mc_new },
	.mmu      = { 0x00000001, tu102_mmu_new },
	.pci      = { 0x00000001, gp100_pci_new },
	.pmu      = { 0x00000001, gp102_pmu_new },
	.privring = { 0x00000001, gm200_privring_new },
	.therm    = { 0x00000001, gp100_therm_new },
	.timer    = { 0x00000001, gk20a_timer_new },
	.top      = { 0x00000001, gk104_top_new },
	.ce       = { 0x0000001f, tu102_ce_new },
	.disp     = { 0x00000001, tu102_disp_new },
	.dma      = { 0x00000001, gv100_dma_new },
	.fifo     = { 0x00000001, tu102_fifo_new },
	.gr       = { 0x00000001, tu102_gr_new },
	.nvdec    = { 0x00000001, gm107_nvdec_new },
	.nvenc    = { 0x00000001, gm107_nvenc_new },
	.sec2     = { 0x00000001, tu102_sec2_new },
};

static const struct nvkm_device_chip
nv164_chipset = {
	.name = "TU104",
	.acr      = { 0x00000001, tu102_acr_new },
	.bar      = { 0x00000001, tu102_bar_new },
	.bios     = { 0x00000001, nvkm_bios_new },
	.bus      = { 0x00000001, gf100_bus_new },
	.devinit  = { 0x00000001, tu102_devinit_new },
	.fault    = { 0x00000001, tu102_fault_new },
	.fb       = { 0x00000001, gv100_fb_new },
	.fuse     = { 0x00000001, gm107_fuse_new },
	.gpio     = { 0x00000001, gk104_gpio_new },
	.gsp      = { 0x00000001, gv100_gsp_new },
	.i2c      = { 0x00000001, gm200_i2c_new },
	.imem     = { 0x00000001, nv50_instmem_new },
	.ltc      = { 0x00000001, gp102_ltc_new },
	.mc       = { 0x00000001, tu102_mc_new },
	.mmu      = { 0x00000001, tu102_mmu_new },
	.pci      = { 0x00000001, gp100_pci_new },
	.pmu      = { 0x00000001, gp102_pmu_new },
	.privring = { 0x00000001, gm200_privring_new },
	.therm    = { 0x00000001, gp100_therm_new },
	.timer    = { 0x00000001, gk20a_timer_new },
	.top      = { 0x00000001, gk104_top_new },
	.ce       = { 0x0000001f, tu102_ce_new },
	.disp     = { 0x00000001, tu102_disp_new },
	.dma      = { 0x00000001, gv100_dma_new },
	.fifo     = { 0x00000001, tu102_fifo_new },
	.gr       = { 0x00000001, tu102_gr_new },
	.nvdec    = { 0x00000003, gm107_nvdec_new },
	.nvenc    = { 0x00000001, gm107_nvenc_new },
	.sec2     = { 0x00000001, tu102_sec2_new },
};

static const struct nvkm_device_chip
nv166_chipset = {
	.name = "TU106",
	.acr      = { 0x00000001, tu102_acr_new },
	.bar      = { 0x00000001, tu102_bar_new },
	.bios     = { 0x00000001, nvkm_bios_new },
	.bus      = { 0x00000001, gf100_bus_new },
	.devinit  = { 0x00000001, tu102_devinit_new },
	.fault    = { 0x00000001, tu102_fault_new },
	.fb       = { 0x00000001, gv100_fb_new },
	.fuse     = { 0x00000001, gm107_fuse_new },
	.gpio     = { 0x00000001, gk104_gpio_new },
	.gsp      = { 0x00000001, gv100_gsp_new },
	.i2c      = { 0x00000001, gm200_i2c_new },
	.imem     = { 0x00000001, nv50_instmem_new },
	.ltc      = { 0x00000001, gp102_ltc_new },
	.mc       = { 0x00000001, tu102_mc_new },
	.mmu      = { 0x00000001, tu102_mmu_new },
	.pci      = { 0x00000001, gp100_pci_new },
	.pmu      = { 0x00000001, gp102_pmu_new },
	.privring = { 0x00000001, gm200_privring_new },
	.therm    = { 0x00000001, gp100_therm_new },
	.timer    = { 0x00000001, gk20a_timer_new },
	.top      = { 0x00000001, gk104_top_new },
	.ce       = { 0x0000001f, tu102_ce_new },
	.disp     = { 0x00000001, tu102_disp_new },
	.dma      = { 0x00000001, gv100_dma_new },
	.fifo     = { 0x00000001, tu102_fifo_new },
	.gr       = { 0x00000001, tu102_gr_new },
	.nvdec    = { 0x00000007, gm107_nvdec_new },
	.nvenc    = { 0x00000001, gm107_nvenc_new },
	.sec2     = { 0x00000001, tu102_sec2_new },
};

static const struct nvkm_device_chip
nv167_chipset = {
	.name = "TU117",
	.acr      = { 0x00000001, tu102_acr_new },
	.bar      = { 0x00000001, tu102_bar_new },
	.bios     = { 0x00000001, nvkm_bios_new },
	.bus      = { 0x00000001, gf100_bus_new },
	.devinit  = { 0x00000001, tu102_devinit_new },
	.fault    = { 0x00000001, tu102_fault_new },
	.fb       = { 0x00000001, gv100_fb_new },
	.fuse     = { 0x00000001, gm107_fuse_new },
	.gpio     = { 0x00000001, gk104_gpio_new },
	.gsp      = { 0x00000001, gv100_gsp_new },
	.i2c      = { 0x00000001, gm200_i2c_new },
	.imem     = { 0x00000001, nv50_instmem_new },
	.ltc      = { 0x00000001, gp102_ltc_new },
	.mc       = { 0x00000001, tu102_mc_new },
	.mmu      = { 0x00000001, tu102_mmu_new },
	.pci      = { 0x00000001, gp100_pci_new },
	.pmu      = { 0x00000001, gp102_pmu_new },
	.privring = { 0x00000001, gm200_privring_new },
	.therm    = { 0x00000001, gp100_therm_new },
	.timer    = { 0x00000001, gk20a_timer_new },
	.top      = { 0x00000001, gk104_top_new },
	.ce       = { 0x0000001f, tu102_ce_new },
	.disp     = { 0x00000001, tu102_disp_new },
	.dma      = { 0x00000001, gv100_dma_new },
	.fifo     = { 0x00000001, tu102_fifo_new },
	.gr       = { 0x00000001, tu102_gr_new },
	.nvdec    = { 0x00000001, gm107_nvdec_new },
	.nvenc    = { 0x00000001, gm107_nvenc_new },
	.sec2     = { 0x00000001, tu102_sec2_new },
};

static const struct nvkm_device_chip
nv168_chipset = {
	.name = "TU116",
	.acr      = { 0x00000001, tu102_acr_new },
	.bar      = { 0x00000001, tu102_bar_new },
	.bios     = { 0x00000001, nvkm_bios_new },
	.bus      = { 0x00000001, gf100_bus_new },
	.devinit  = { 0x00000001, tu102_devinit_new },
	.fault    = { 0x00000001, tu102_fault_new },
	.fb       = { 0x00000001, gv100_fb_new },
	.fuse     = { 0x00000001, gm107_fuse_new },
	.gpio     = { 0x00000001, gk104_gpio_new },
	.gsp      = { 0x00000001, gv100_gsp_new },
	.i2c      = { 0x00000001, gm200_i2c_new },
	.imem     = { 0x00000001, nv50_instmem_new },
	.ltc      = { 0x00000001, gp102_ltc_new },
	.mc       = { 0x00000001, tu102_mc_new },
	.mmu      = { 0x00000001, tu102_mmu_new },
	.pci      = { 0x00000001, gp100_pci_new },
	.pmu      = { 0x00000001, gp102_pmu_new },
	.privring = { 0x00000001, gm200_privring_new },
	.therm    = { 0x00000001, gp100_therm_new },
	.timer    = { 0x00000001, gk20a_timer_new },
	.top      = { 0x00000001, gk104_top_new },
	.ce       = { 0x0000001f, tu102_ce_new },
	.disp     = { 0x00000001, tu102_disp_new },
	.dma      = { 0x00000001, gv100_dma_new },
	.fifo     = { 0x00000001, tu102_fifo_new },
	.gr       = { 0x00000001, tu102_gr_new },
	.nvdec    = { 0x00000001, gm107_nvdec_new },
	.nvenc    = { 0x00000001, gm107_nvenc_new },
	.sec2     = { 0x00000001, tu102_sec2_new },
};

static const struct nvkm_device_chip
nv170_chipset = {
	.name = "GA100",
	.bar      = { 0x00000001, tu102_bar_new },
	.bios     = { 0x00000001, nvkm_bios_new },
	.devinit  = { 0x00000001, ga100_devinit_new },
	.fb       = { 0x00000001, ga100_fb_new },
	.gpio     = { 0x00000001, gk104_gpio_new },
	.i2c      = { 0x00000001, gm200_i2c_new },
	.imem     = { 0x00000001, nv50_instmem_new },
	.mc       = { 0x00000001, ga100_mc_new },
	.mmu      = { 0x00000001, tu102_mmu_new },
	.pci      = { 0x00000001, gp100_pci_new },
	.privring = { 0x00000001, gm200_privring_new },
	.timer    = { 0x00000001, gk20a_timer_new },
	.top      = { 0x00000001, ga100_top_new },
};

static const struct nvkm_device_chip
nv172_chipset = {
	.name = "GA102",
	.bar      = { 0x00000001, tu102_bar_new },
	.bios     = { 0x00000001, nvkm_bios_new },
	.devinit  = { 0x00000001, ga100_devinit_new },
	.fb       = { 0x00000001, ga102_fb_new },
	.gpio     = { 0x00000001, ga102_gpio_new },
	.i2c      = { 0x00000001, gm200_i2c_new },
	.imem     = { 0x00000001, nv50_instmem_new },
	.mc       = { 0x00000001, ga100_mc_new },
	.mmu      = { 0x00000001, tu102_mmu_new },
	.pci      = { 0x00000001, gp100_pci_new },
	.privring = { 0x00000001, gm200_privring_new },
	.timer    = { 0x00000001, gk20a_timer_new },
	.top      = { 0x00000001, ga100_top_new },
	.disp     = { 0x00000001, ga102_disp_new },
	.dma      = { 0x00000001, gv100_dma_new },
	.fifo     = { 0x00000001, ga102_fifo_new },
};

static const struct nvkm_device_chip
nv174_chipset = {
	.name = "GA104",
	.bar      = { 0x00000001, tu102_bar_new },
	.bios     = { 0x00000001, nvkm_bios_new },
	.devinit  = { 0x00000001, ga100_devinit_new },
	.fb       = { 0x00000001, ga102_fb_new },
	.gpio     = { 0x00000001, ga102_gpio_new },
	.i2c      = { 0x00000001, gm200_i2c_new },
	.imem     = { 0x00000001, nv50_instmem_new },
	.mc       = { 0x00000001, ga100_mc_new },
	.mmu      = { 0x00000001, tu102_mmu_new },
	.pci      = { 0x00000001, gp100_pci_new },
	.privring = { 0x00000001, gm200_privring_new },
	.timer    = { 0x00000001, gk20a_timer_new },
	.top      = { 0x00000001, ga100_top_new },
	.disp     = { 0x00000001, ga102_disp_new },
	.dma      = { 0x00000001, gv100_dma_new },
	.fifo     = { 0x00000001, ga102_fifo_new },
};

static const struct nvkm_device_chip
nv176_chipset = {
	.name = "GA106",
	.bar      = { 0x00000001, tu102_bar_new },
	.bios     = { 0x00000001, nvkm_bios_new },
	.devinit  = { 0x00000001, ga100_devinit_new },
	.fb       = { 0x00000001, ga102_fb_new },
	.gpio     = { 0x00000001, ga102_gpio_new },
	.i2c      = { 0x00000001, gm200_i2c_new },
	.imem     = { 0x00000001, nv50_instmem_new },
	.mc       = { 0x00000001, ga100_mc_new },
	.mmu      = { 0x00000001, tu102_mmu_new },
	.pci      = { 0x00000001, gp100_pci_new },
	.privring = { 0x00000001, gm200_privring_new },
	.timer    = { 0x00000001, gk20a_timer_new },
	.top      = { 0x00000001, ga100_top_new },
	.disp     = { 0x00000001, ga102_disp_new },
	.dma      = { 0x00000001, gv100_dma_new },
	.fifo     = { 0x00000001, ga102_fifo_new },
};

static const struct nvkm_device_chip
nv177_chipset = {
	.name = "GA107",
	.bar      = { 0x00000001, tu102_bar_new },
	.bios     = { 0x00000001, nvkm_bios_new },
	.devinit  = { 0x00000001, ga100_devinit_new },
	.fb       = { 0x00000001, ga102_fb_new },
	.gpio     = { 0x00000001, ga102_gpio_new },
	.i2c      = { 0x00000001, gm200_i2c_new },
	.imem     = { 0x00000001, nv50_instmem_new },
	.mc       = { 0x00000001, ga100_mc_new },
	.mmu      = { 0x00000001, tu102_mmu_new },
	.pci      = { 0x00000001, gp100_pci_new },
	.privring = { 0x00000001, gm200_privring_new },
	.timer    = { 0x00000001, gk20a_timer_new },
	.top      = { 0x00000001, ga100_top_new },
	.disp     = { 0x00000001, ga102_disp_new },
	.dma      = { 0x00000001, gv100_dma_new },
	.fifo     = { 0x00000001, ga102_fifo_new },
>>>>>>> upstream/android-13
};

static int
nvkm_device_event_ctor(struct nvkm_object *object, void *data, u32 size,
		       struct nvkm_notify *notify)
{
	if (!WARN_ON(size != 0)) {
		notify->size  = 0;
		notify->types = 1;
		notify->index = 0;
		return 0;
	}
	return -EINVAL;
}

static const struct nvkm_event_func
nvkm_device_event_func = {
	.ctor = nvkm_device_event_ctor,
};

struct nvkm_subdev *
<<<<<<< HEAD
nvkm_device_subdev(struct nvkm_device *device, int index)
{
	struct nvkm_engine *engine;

	if (device->disable_mask & (1ULL << index))
		return NULL;

	switch (index) {
#define _(n,p,m) case NVKM_SUBDEV_##n: if (p) return (m); break
	_(BAR     , device->bar     , &device->bar->subdev);
	_(VBIOS   , device->bios    , &device->bios->subdev);
	_(BUS     , device->bus     , &device->bus->subdev);
	_(CLK     , device->clk     , &device->clk->subdev);
	_(DEVINIT , device->devinit , &device->devinit->subdev);
	_(FAULT   , device->fault   , &device->fault->subdev);
	_(FB      , device->fb      , &device->fb->subdev);
	_(FUSE    , device->fuse    , &device->fuse->subdev);
	_(GPIO    , device->gpio    , &device->gpio->subdev);
	_(I2C     , device->i2c     , &device->i2c->subdev);
	_(IBUS    , device->ibus    ,  device->ibus);
	_(ICCSENSE, device->iccsense, &device->iccsense->subdev);
	_(INSTMEM , device->imem    , &device->imem->subdev);
	_(LTC     , device->ltc     , &device->ltc->subdev);
	_(MC      , device->mc      , &device->mc->subdev);
	_(MMU     , device->mmu     , &device->mmu->subdev);
	_(MXM     , device->mxm     ,  device->mxm);
	_(PCI     , device->pci     , &device->pci->subdev);
	_(PMU     , device->pmu     , &device->pmu->subdev);
	_(SECBOOT , device->secboot , &device->secboot->subdev);
	_(THERM   , device->therm   , &device->therm->subdev);
	_(TIMER   , device->timer   , &device->timer->subdev);
	_(TOP     , device->top     , &device->top->subdev);
	_(VOLT    , device->volt    , &device->volt->subdev);
#undef _
	default:
		engine = nvkm_device_engine(device, index);
		if (engine)
			return &engine->subdev;
		break;
	}
=======
nvkm_device_subdev(struct nvkm_device *device, int type, int inst)
{
	struct nvkm_subdev *subdev;

	list_for_each_entry(subdev, &device->subdev, head) {
		if (subdev->type == type && subdev->inst == inst)
			return subdev;
	}

>>>>>>> upstream/android-13
	return NULL;
}

struct nvkm_engine *
<<<<<<< HEAD
nvkm_device_engine(struct nvkm_device *device, int index)
{
	if (device->disable_mask & (1ULL << index))
		return NULL;

	switch (index) {
#define _(n,p,m) case NVKM_ENGINE_##n: if (p) return (m); break
	_(BSP    , device->bsp     ,  device->bsp);
	_(CE0    , device->ce[0]   ,  device->ce[0]);
	_(CE1    , device->ce[1]   ,  device->ce[1]);
	_(CE2    , device->ce[2]   ,  device->ce[2]);
	_(CE3    , device->ce[3]   ,  device->ce[3]);
	_(CE4    , device->ce[4]   ,  device->ce[4]);
	_(CE5    , device->ce[5]   ,  device->ce[5]);
	_(CE6    , device->ce[6]   ,  device->ce[6]);
	_(CE7    , device->ce[7]   ,  device->ce[7]);
	_(CE8    , device->ce[8]   ,  device->ce[8]);
	_(CIPHER , device->cipher  ,  device->cipher);
	_(DISP   , device->disp    , &device->disp->engine);
	_(DMAOBJ , device->dma     , &device->dma->engine);
	_(FIFO   , device->fifo    , &device->fifo->engine);
	_(GR     , device->gr      , &device->gr->engine);
	_(IFB    , device->ifb     ,  device->ifb);
	_(ME     , device->me      ,  device->me);
	_(MPEG   , device->mpeg    ,  device->mpeg);
	_(MSENC  , device->msenc   ,  device->msenc);
	_(MSPDEC , device->mspdec  ,  device->mspdec);
	_(MSPPP  , device->msppp   ,  device->msppp);
	_(MSVLD  , device->msvld   ,  device->msvld);
	_(NVENC0 , device->nvenc[0],  device->nvenc[0]);
	_(NVENC1 , device->nvenc[1],  device->nvenc[1]);
	_(NVENC2 , device->nvenc[2],  device->nvenc[2]);
	_(NVDEC  , device->nvdec   , &device->nvdec->engine);
	_(PM     , device->pm      , &device->pm->engine);
	_(SEC    , device->sec     ,  device->sec);
	_(SEC2   , device->sec2    , &device->sec2->engine);
	_(SW     , device->sw      , &device->sw->engine);
	_(VIC    , device->vic     ,  device->vic);
	_(VP     , device->vp      ,  device->vp);
#undef _
	default:
		WARN_ON(1);
		break;
	}
=======
nvkm_device_engine(struct nvkm_device *device, int type, int inst)
{
	struct nvkm_subdev *subdev = nvkm_device_subdev(device, type, inst);
	if (subdev && subdev->func == &nvkm_engine)
		return container_of(subdev, struct nvkm_engine, subdev);
>>>>>>> upstream/android-13
	return NULL;
}

int
nvkm_device_fini(struct nvkm_device *device, bool suspend)
{
	const char *action = suspend ? "suspend" : "fini";
	struct nvkm_subdev *subdev;
<<<<<<< HEAD
	int ret, i;
=======
	int ret;
>>>>>>> upstream/android-13
	s64 time;

	nvdev_trace(device, "%s running...\n", action);
	time = ktime_to_us(ktime_get());

	nvkm_acpi_fini(device);

<<<<<<< HEAD
	for (i = NVKM_SUBDEV_NR - 1; i >= 0; i--) {
		if ((subdev = nvkm_device_subdev(device, i))) {
			ret = nvkm_subdev_fini(subdev, suspend);
			if (ret && suspend)
				goto fail;
		}
=======
	list_for_each_entry_reverse(subdev, &device->subdev, head) {
		ret = nvkm_subdev_fini(subdev, suspend);
		if (ret && suspend)
			goto fail;
>>>>>>> upstream/android-13
	}

	nvkm_therm_clkgate_fini(device->therm, suspend);

	if (device->func->fini)
		device->func->fini(device, suspend);

	time = ktime_to_us(ktime_get()) - time;
	nvdev_trace(device, "%s completed in %lldus...\n", action, time);
	return 0;

fail:
<<<<<<< HEAD
	do {
		if ((subdev = nvkm_device_subdev(device, i))) {
			int rret = nvkm_subdev_init(subdev);
			if (rret)
				nvkm_fatal(subdev, "failed restart, %d\n", ret);
		}
	} while (++i < NVKM_SUBDEV_NR);
=======
	list_for_each_entry_from(subdev, &device->subdev, head) {
		int rret = nvkm_subdev_init(subdev);
		if (rret)
			nvkm_fatal(subdev, "failed restart, %d\n", ret);
	}
>>>>>>> upstream/android-13

	nvdev_trace(device, "%s failed with %d\n", action, ret);
	return ret;
}

static int
nvkm_device_preinit(struct nvkm_device *device)
{
	struct nvkm_subdev *subdev;
<<<<<<< HEAD
	int ret, i;
=======
	int ret;
>>>>>>> upstream/android-13
	s64 time;

	nvdev_trace(device, "preinit running...\n");
	time = ktime_to_us(ktime_get());

	if (device->func->preinit) {
		ret = device->func->preinit(device);
		if (ret)
			goto fail;
	}

<<<<<<< HEAD
	for (i = 0; i < NVKM_SUBDEV_NR; i++) {
		if ((subdev = nvkm_device_subdev(device, i))) {
			ret = nvkm_subdev_preinit(subdev);
			if (ret)
				goto fail;
		}
	}

	ret = nvkm_devinit_post(device->devinit, &device->disable_mask);
=======
	list_for_each_entry(subdev, &device->subdev, head) {
		ret = nvkm_subdev_preinit(subdev);
		if (ret)
			goto fail;
	}

	ret = nvkm_devinit_post(device->devinit);
>>>>>>> upstream/android-13
	if (ret)
		goto fail;

	time = ktime_to_us(ktime_get()) - time;
	nvdev_trace(device, "preinit completed in %lldus\n", time);
	return 0;

fail:
	nvdev_error(device, "preinit failed with %d\n", ret);
	return ret;
}

int
nvkm_device_init(struct nvkm_device *device)
{
	struct nvkm_subdev *subdev;
<<<<<<< HEAD
	int ret, i;
=======
	int ret;
>>>>>>> upstream/android-13
	s64 time;

	ret = nvkm_device_preinit(device);
	if (ret)
		return ret;

	nvkm_device_fini(device, false);

	nvdev_trace(device, "init running...\n");
	time = ktime_to_us(ktime_get());

	if (device->func->init) {
		ret = device->func->init(device);
		if (ret)
			goto fail;
	}

<<<<<<< HEAD
	for (i = 0; i < NVKM_SUBDEV_NR; i++) {
		if ((subdev = nvkm_device_subdev(device, i))) {
			ret = nvkm_subdev_init(subdev);
			if (ret)
				goto fail_subdev;
		}
=======
	list_for_each_entry(subdev, &device->subdev, head) {
		ret = nvkm_subdev_init(subdev);
		if (ret)
			goto fail_subdev;
>>>>>>> upstream/android-13
	}

	nvkm_acpi_init(device);
	nvkm_therm_clkgate_enable(device->therm);

	time = ktime_to_us(ktime_get()) - time;
	nvdev_trace(device, "init completed in %lldus\n", time);
	return 0;

fail_subdev:
<<<<<<< HEAD
	do {
		if ((subdev = nvkm_device_subdev(device, i)))
			nvkm_subdev_fini(subdev, false);
	} while (--i >= 0);

=======
	list_for_each_entry_from(subdev, &device->subdev, head)
		nvkm_subdev_fini(subdev, false);
>>>>>>> upstream/android-13
fail:
	nvkm_device_fini(device, false);

	nvdev_error(device, "init failed with %d\n", ret);
	return ret;
}

void
nvkm_device_del(struct nvkm_device **pdevice)
{
	struct nvkm_device *device = *pdevice;
<<<<<<< HEAD
	int i;
	if (device) {
		mutex_lock(&nv_devices_mutex);
		device->disable_mask = 0;
		for (i = NVKM_SUBDEV_NR - 1; i >= 0; i--) {
			struct nvkm_subdev *subdev =
				nvkm_device_subdev(device, i);
			nvkm_subdev_del(&subdev);
		}
=======
	struct nvkm_subdev *subdev, *subtmp;
	if (device) {
		mutex_lock(&nv_devices_mutex);

		list_for_each_entry_safe_reverse(subdev, subtmp, &device->subdev, head)
			nvkm_subdev_del(&subdev);
>>>>>>> upstream/android-13

		nvkm_event_fini(&device->event);

		if (device->pri)
			iounmap(device->pri);
		list_del(&device->head);

		if (device->func->dtor)
			*pdevice = device->func->dtor(device);
		mutex_unlock(&nv_devices_mutex);

		kfree(*pdevice);
		*pdevice = NULL;
	}
}

<<<<<<< HEAD
=======
/* returns true if the GPU is in the CPU native byte order */
static inline bool
nvkm_device_endianness(struct nvkm_device *device)
{
#ifdef __BIG_ENDIAN
	const bool big_endian = true;
#else
	const bool big_endian = false;
#endif

	/* Read NV_PMC_BOOT_1, and assume non-functional endian switch if it
	 * doesn't contain the expected values.
	 */
	u32 pmc_boot_1 = nvkm_rd32(device, 0x000004);
	if (pmc_boot_1 && pmc_boot_1 != 0x01000001)
		return !big_endian; /* Assume GPU is LE in this case. */

	/* 0 means LE and 0x01000001 means BE GPU. Condition is true when
	 * GPU/CPU endianness don't match.
	 */
	if (big_endian == !pmc_boot_1) {
		nvkm_wr32(device, 0x000004, 0x01000001);
		nvkm_rd32(device, 0x000000);
		if (nvkm_rd32(device, 0x000004) != (big_endian ? 0x01000001 : 0x00000000))
			return !big_endian; /* Assume GPU is LE on any unexpected read-back. */
	}

	/* CPU/GPU endianness should (hopefully) match. */
	return true;
}

>>>>>>> upstream/android-13
int
nvkm_device_ctor(const struct nvkm_device_func *func,
		 const struct nvkm_device_quirk *quirk,
		 struct device *dev, enum nvkm_device_type type, u64 handle,
		 const char *name, const char *cfg, const char *dbg,
		 bool detect, bool mmio, u64 subdev_mask,
		 struct nvkm_device *device)
{
	struct nvkm_subdev *subdev;
	u64 mmio_base, mmio_size;
<<<<<<< HEAD
	u32 boot0, strap;
	void __iomem *map;
	int ret = -EEXIST;
	int i;
=======
	u32 boot0, boot1, strap;
	int ret = -EEXIST, j;
	unsigned chipset;
>>>>>>> upstream/android-13

	mutex_lock(&nv_devices_mutex);
	if (nvkm_device_find_locked(handle))
		goto done;

	device->func = func;
	device->quirk = quirk;
	device->dev = dev;
	device->type = type;
	device->handle = handle;
	device->cfgopt = cfg;
	device->dbgopt = dbg;
	device->name = name;
	list_add_tail(&device->head, &nv_devices);
	device->debug = nvkm_dbgopt(device->dbgopt, "device");
<<<<<<< HEAD
=======
	INIT_LIST_HEAD(&device->subdev);
>>>>>>> upstream/android-13

	ret = nvkm_event_init(&nvkm_device_event_func, 1, 1, &device->event);
	if (ret)
		goto done;

	mmio_base = device->func->resource_addr(device, 0);
	mmio_size = device->func->resource_size(device, 0);

<<<<<<< HEAD
	/* identify the chipset, and determine classes of subdev/engines */
	if (detect) {
		map = ioremap(mmio_base, 0x102000);
		if (ret = -ENOMEM, map == NULL)
			goto done;

		/* switch mmio to cpu's native endianness */
#ifndef __BIG_ENDIAN
		if (ioread32_native(map + 0x000004) != 0x00000000) {
#else
		if (ioread32_native(map + 0x000004) == 0x00000000) {
#endif
			iowrite32_native(0x01000001, map + 0x000004);
			ioread32_native(map);
		}

		/* read boot0 and strapping information */
		boot0 = ioread32_native(map + 0x000000);
		strap = ioread32_native(map + 0x101000);
		iounmap(map);
=======
	if (detect || mmio) {
		device->pri = ioremap(mmio_base, mmio_size);
		if (device->pri == NULL) {
			nvdev_error(device, "unable to map PRI\n");
			ret = -ENOMEM;
			goto done;
		}
	}

	/* identify the chipset, and determine classes of subdev/engines */
	if (detect) {
		/* switch mmio to cpu's native endianness */
		if (!nvkm_device_endianness(device)) {
			nvdev_error(device,
				    "Couldn't switch GPU to CPUs endianess\n");
			ret = -ENOSYS;
			goto done;
		}

		boot0 = nvkm_rd32(device, 0x000000);

		/* chipset can be overridden for devel/testing purposes */
		chipset = nvkm_longopt(device->cfgopt, "NvChipset", 0);
		if (chipset) {
			u32 override_boot0;

			if (chipset >= 0x10) {
				override_boot0  = ((chipset & 0x1ff) << 20);
				override_boot0 |= 0x000000a1;
			} else {
				if (chipset != 0x04)
					override_boot0 = 0x20104000;
				else
					override_boot0 = 0x20004000;
			}

			nvdev_warn(device, "CHIPSET OVERRIDE: %08x -> %08x\n",
				   boot0, override_boot0);
			boot0 = override_boot0;
		}
>>>>>>> upstream/android-13

		/* determine chipset and derive architecture from it */
		if ((boot0 & 0x1f000000) > 0) {
			device->chipset = (boot0 & 0x1ff00000) >> 20;
			device->chiprev = (boot0 & 0x000000ff);
			switch (device->chipset & 0x1f0) {
			case 0x010: {
				if (0x461 & (1 << (device->chipset & 0xf)))
					device->card_type = NV_10;
				else
					device->card_type = NV_11;
				device->chiprev = 0x00;
				break;
			}
			case 0x020: device->card_type = NV_20; break;
			case 0x030: device->card_type = NV_30; break;
			case 0x040:
			case 0x060: device->card_type = NV_40; break;
			case 0x050:
			case 0x080:
			case 0x090:
			case 0x0a0: device->card_type = NV_50; break;
			case 0x0c0:
			case 0x0d0: device->card_type = NV_C0; break;
			case 0x0e0:
			case 0x0f0:
			case 0x100: device->card_type = NV_E0; break;
			case 0x110:
			case 0x120: device->card_type = GM100; break;
			case 0x130: device->card_type = GP100; break;
			case 0x140: device->card_type = GV100; break;
<<<<<<< HEAD
=======
			case 0x160: device->card_type = TU100; break;
			case 0x170: device->card_type = GA100; break;
>>>>>>> upstream/android-13
			default:
				break;
			}
		} else
		if ((boot0 & 0xff00fff0) == 0x20004000) {
			if (boot0 & 0x00f00000)
				device->chipset = 0x05;
			else
				device->chipset = 0x04;
			device->card_type = NV_04;
		}

		switch (device->chipset) {
		case 0x004: device->chip = &nv4_chipset; break;
		case 0x005: device->chip = &nv5_chipset; break;
		case 0x010: device->chip = &nv10_chipset; break;
		case 0x011: device->chip = &nv11_chipset; break;
		case 0x015: device->chip = &nv15_chipset; break;
		case 0x017: device->chip = &nv17_chipset; break;
		case 0x018: device->chip = &nv18_chipset; break;
		case 0x01a: device->chip = &nv1a_chipset; break;
		case 0x01f: device->chip = &nv1f_chipset; break;
		case 0x020: device->chip = &nv20_chipset; break;
		case 0x025: device->chip = &nv25_chipset; break;
		case 0x028: device->chip = &nv28_chipset; break;
		case 0x02a: device->chip = &nv2a_chipset; break;
		case 0x030: device->chip = &nv30_chipset; break;
		case 0x031: device->chip = &nv31_chipset; break;
		case 0x034: device->chip = &nv34_chipset; break;
		case 0x035: device->chip = &nv35_chipset; break;
		case 0x036: device->chip = &nv36_chipset; break;
		case 0x040: device->chip = &nv40_chipset; break;
		case 0x041: device->chip = &nv41_chipset; break;
		case 0x042: device->chip = &nv42_chipset; break;
		case 0x043: device->chip = &nv43_chipset; break;
		case 0x044: device->chip = &nv44_chipset; break;
		case 0x045: device->chip = &nv45_chipset; break;
		case 0x046: device->chip = &nv46_chipset; break;
		case 0x047: device->chip = &nv47_chipset; break;
		case 0x049: device->chip = &nv49_chipset; break;
		case 0x04a: device->chip = &nv4a_chipset; break;
		case 0x04b: device->chip = &nv4b_chipset; break;
		case 0x04c: device->chip = &nv4c_chipset; break;
		case 0x04e: device->chip = &nv4e_chipset; break;
		case 0x050: device->chip = &nv50_chipset; break;
		case 0x063: device->chip = &nv63_chipset; break;
		case 0x067: device->chip = &nv67_chipset; break;
		case 0x068: device->chip = &nv68_chipset; break;
		case 0x084: device->chip = &nv84_chipset; break;
		case 0x086: device->chip = &nv86_chipset; break;
		case 0x092: device->chip = &nv92_chipset; break;
		case 0x094: device->chip = &nv94_chipset; break;
		case 0x096: device->chip = &nv96_chipset; break;
		case 0x098: device->chip = &nv98_chipset; break;
		case 0x0a0: device->chip = &nva0_chipset; break;
		case 0x0a3: device->chip = &nva3_chipset; break;
		case 0x0a5: device->chip = &nva5_chipset; break;
		case 0x0a8: device->chip = &nva8_chipset; break;
		case 0x0aa: device->chip = &nvaa_chipset; break;
		case 0x0ac: device->chip = &nvac_chipset; break;
		case 0x0af: device->chip = &nvaf_chipset; break;
		case 0x0c0: device->chip = &nvc0_chipset; break;
		case 0x0c1: device->chip = &nvc1_chipset; break;
		case 0x0c3: device->chip = &nvc3_chipset; break;
		case 0x0c4: device->chip = &nvc4_chipset; break;
		case 0x0c8: device->chip = &nvc8_chipset; break;
		case 0x0ce: device->chip = &nvce_chipset; break;
		case 0x0cf: device->chip = &nvcf_chipset; break;
		case 0x0d7: device->chip = &nvd7_chipset; break;
		case 0x0d9: device->chip = &nvd9_chipset; break;
		case 0x0e4: device->chip = &nve4_chipset; break;
		case 0x0e6: device->chip = &nve6_chipset; break;
		case 0x0e7: device->chip = &nve7_chipset; break;
		case 0x0ea: device->chip = &nvea_chipset; break;
		case 0x0f0: device->chip = &nvf0_chipset; break;
		case 0x0f1: device->chip = &nvf1_chipset; break;
		case 0x106: device->chip = &nv106_chipset; break;
		case 0x108: device->chip = &nv108_chipset; break;
		case 0x117: device->chip = &nv117_chipset; break;
		case 0x118: device->chip = &nv118_chipset; break;
		case 0x120: device->chip = &nv120_chipset; break;
		case 0x124: device->chip = &nv124_chipset; break;
		case 0x126: device->chip = &nv126_chipset; break;
		case 0x12b: device->chip = &nv12b_chipset; break;
		case 0x130: device->chip = &nv130_chipset; break;
		case 0x132: device->chip = &nv132_chipset; break;
		case 0x134: device->chip = &nv134_chipset; break;
		case 0x136: device->chip = &nv136_chipset; break;
		case 0x137: device->chip = &nv137_chipset; break;
		case 0x138: device->chip = &nv138_chipset; break;
		case 0x13b: device->chip = &nv13b_chipset; break;
		case 0x140: device->chip = &nv140_chipset; break;
<<<<<<< HEAD
		default:
			nvdev_error(device, "unknown chipset (%08x)\n", boot0);
			goto done;
=======
		case 0x162: device->chip = &nv162_chipset; break;
		case 0x164: device->chip = &nv164_chipset; break;
		case 0x166: device->chip = &nv166_chipset; break;
		case 0x167: device->chip = &nv167_chipset; break;
		case 0x168: device->chip = &nv168_chipset; break;
		case 0x172: device->chip = &nv172_chipset; break;
		case 0x174: device->chip = &nv174_chipset; break;
		case 0x176: device->chip = &nv176_chipset; break;
		case 0x177: device->chip = &nv177_chipset; break;
		default:
			if (nvkm_boolopt(device->cfgopt, "NvEnableUnsupportedChipsets", false)) {
				switch (device->chipset) {
				case 0x170: device->chip = &nv170_chipset; break;
				default:
					break;
				}
			}

			if (!device->chip) {
				nvdev_error(device, "unknown chipset (%08x)\n", boot0);
				ret = -ENODEV;
				goto done;
			}
			break;
>>>>>>> upstream/android-13
		}

		nvdev_info(device, "NVIDIA %s (%08x)\n",
			   device->chip->name, boot0);

<<<<<<< HEAD
=======
		/* vGPU detection */
		boot1 = nvkm_rd32(device, 0x0000004);
		if (device->card_type >= TU100 && (boot1 & 0x00030000)) {
			nvdev_info(device, "vGPUs are not supported\n");
			ret = -ENODEV;
			goto done;
		}

		/* read strapping information */
		strap = nvkm_rd32(device, 0x101000);

>>>>>>> upstream/android-13
		/* determine frequency of timing crystal */
		if ( device->card_type <= NV_10 || device->chipset < 0x17 ||
		    (device->chipset >= 0x20 && device->chipset < 0x25))
			strap &= 0x00000040;
		else
			strap &= 0x00400040;

		switch (strap) {
		case 0x00000000: device->crystal = 13500; break;
		case 0x00000040: device->crystal = 14318; break;
		case 0x00400000: device->crystal = 27000; break;
		case 0x00400040: device->crystal = 25000; break;
		}
	} else {
		device->chip = &null_chipset;
	}

	if (!device->name)
		device->name = device->chip->name;

<<<<<<< HEAD
	if (mmio) {
		device->pri = ioremap(mmio_base, mmio_size);
		if (!device->pri) {
			nvdev_error(device, "unable to map PRI\n");
			ret = -ENOMEM;
			goto done;
		}
	}

	mutex_init(&device->mutex);

	for (i = 0; i < NVKM_SUBDEV_NR; i++) {
#define _(s,m) case s:                                                         \
	if (device->chip->m && (subdev_mask & (1ULL << (s)))) {                \
		ret = device->chip->m(device, (s), &device->m);                \
		if (ret) {                                                     \
			subdev = nvkm_device_subdev(device, (s));              \
			nvkm_subdev_del(&subdev);                              \
			device->m = NULL;                                      \
			if (ret != -ENODEV) {                                  \
				nvdev_error(device, "%s ctor failed, %d\n",    \
					    nvkm_subdev_name[s], ret);         \
				goto done;                                     \
			}                                                      \
		}                                                              \
	}                                                                      \
	break
		switch (i) {
		_(NVKM_SUBDEV_BAR     ,      bar);
		_(NVKM_SUBDEV_VBIOS   ,     bios);
		_(NVKM_SUBDEV_BUS     ,      bus);
		_(NVKM_SUBDEV_CLK     ,      clk);
		_(NVKM_SUBDEV_DEVINIT ,  devinit);
		_(NVKM_SUBDEV_FAULT   ,    fault);
		_(NVKM_SUBDEV_FB      ,       fb);
		_(NVKM_SUBDEV_FUSE    ,     fuse);
		_(NVKM_SUBDEV_GPIO    ,     gpio);
		_(NVKM_SUBDEV_I2C     ,      i2c);
		_(NVKM_SUBDEV_IBUS    ,     ibus);
		_(NVKM_SUBDEV_ICCSENSE, iccsense);
		_(NVKM_SUBDEV_INSTMEM ,     imem);
		_(NVKM_SUBDEV_LTC     ,      ltc);
		_(NVKM_SUBDEV_MC      ,       mc);
		_(NVKM_SUBDEV_MMU     ,      mmu);
		_(NVKM_SUBDEV_MXM     ,      mxm);
		_(NVKM_SUBDEV_PCI     ,      pci);
		_(NVKM_SUBDEV_PMU     ,      pmu);
		_(NVKM_SUBDEV_SECBOOT ,  secboot);
		_(NVKM_SUBDEV_THERM   ,    therm);
		_(NVKM_SUBDEV_TIMER   ,    timer);
		_(NVKM_SUBDEV_TOP     ,      top);
		_(NVKM_SUBDEV_VOLT    ,     volt);
		_(NVKM_ENGINE_BSP     ,      bsp);
		_(NVKM_ENGINE_CE0     ,    ce[0]);
		_(NVKM_ENGINE_CE1     ,    ce[1]);
		_(NVKM_ENGINE_CE2     ,    ce[2]);
		_(NVKM_ENGINE_CE3     ,    ce[3]);
		_(NVKM_ENGINE_CE4     ,    ce[4]);
		_(NVKM_ENGINE_CE5     ,    ce[5]);
		_(NVKM_ENGINE_CE6     ,    ce[6]);
		_(NVKM_ENGINE_CE7     ,    ce[7]);
		_(NVKM_ENGINE_CE8     ,    ce[8]);
		_(NVKM_ENGINE_CIPHER  ,   cipher);
		_(NVKM_ENGINE_DISP    ,     disp);
		_(NVKM_ENGINE_DMAOBJ  ,      dma);
		_(NVKM_ENGINE_FIFO    ,     fifo);
		_(NVKM_ENGINE_GR      ,       gr);
		_(NVKM_ENGINE_IFB     ,      ifb);
		_(NVKM_ENGINE_ME      ,       me);
		_(NVKM_ENGINE_MPEG    ,     mpeg);
		_(NVKM_ENGINE_MSENC   ,    msenc);
		_(NVKM_ENGINE_MSPDEC  ,   mspdec);
		_(NVKM_ENGINE_MSPPP   ,    msppp);
		_(NVKM_ENGINE_MSVLD   ,    msvld);
		_(NVKM_ENGINE_NVENC0  , nvenc[0]);
		_(NVKM_ENGINE_NVENC1  , nvenc[1]);
		_(NVKM_ENGINE_NVENC2  , nvenc[2]);
		_(NVKM_ENGINE_NVDEC   ,    nvdec);
		_(NVKM_ENGINE_PM      ,       pm);
		_(NVKM_ENGINE_SEC     ,      sec);
		_(NVKM_ENGINE_SEC2    ,     sec2);
		_(NVKM_ENGINE_SW      ,       sw);
		_(NVKM_ENGINE_VIC     ,      vic);
		_(NVKM_ENGINE_VP      ,       vp);
		default:
			WARN_ON(1);
			continue;
		}
#undef _
	}

	ret = 0;
done:
=======
	mutex_init(&device->mutex);

#define NVKM_LAYOUT_ONCE(type,data,ptr)                                                      \
	if (device->chip->ptr.inst && (subdev_mask & (BIT_ULL(type)))) {                     \
		WARN_ON(device->chip->ptr.inst != 0x00000001);                               \
		ret = device->chip->ptr.ctor(device, (type), -1, &device->ptr);              \
		subdev = nvkm_device_subdev(device, (type), 0);                              \
		if (ret) {                                                                   \
			nvkm_subdev_del(&subdev);                                            \
			device->ptr = NULL;                                                  \
			if (ret != -ENODEV) {                                                \
				nvdev_error(device, "%s ctor failed: %d\n",                  \
					    nvkm_subdev_type[(type)], ret);                  \
				goto done;                                                   \
			}                                                                    \
		} else {                                                                     \
			subdev->pself = (void **)&device->ptr;                               \
		}                                                                            \
	}
#define NVKM_LAYOUT_INST(type,data,ptr,cnt)                                                  \
	WARN_ON(device->chip->ptr.inst & ~((1 << ARRAY_SIZE(device->ptr)) - 1));             \
	for (j = 0; device->chip->ptr.inst && j < ARRAY_SIZE(device->ptr); j++) {            \
		if ((device->chip->ptr.inst & BIT(j)) && (subdev_mask & BIT_ULL(type))) {    \
			ret = device->chip->ptr.ctor(device, (type), (j), &device->ptr[j]);  \
			subdev = nvkm_device_subdev(device, (type), (j));                    \
			if (ret) {                                                           \
				nvkm_subdev_del(&subdev);                                    \
				device->ptr[j] = NULL;                                       \
				if (ret != -ENODEV) {                                        \
					nvdev_error(device, "%s%d ctor failed: %d\n",        \
						    nvkm_subdev_type[(type)], (j), ret);     \
					goto done;                                           \
				}                                                            \
			} else {                                                             \
				subdev->pself = (void **)&device->ptr[j];                    \
			}                                                                    \
		}                                                                            \
	}
#include <core/layout.h>
#undef NVKM_LAYOUT_INST
#undef NVKM_LAYOUT_ONCE

	ret = 0;
done:
	if (device->pri && (!mmio || ret)) {
		iounmap(device->pri);
		device->pri = NULL;
	}
>>>>>>> upstream/android-13
	mutex_unlock(&nv_devices_mutex);
	return ret;
}
