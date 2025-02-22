/* SPDX-License-Identifier: GPL-2.0 */
#ifndef DDK750_CHIP_H__
#define DDK750_CHIP_H__
#define DEFAULT_INPUT_CLOCK 14318181 /* Default reference clock */
#ifndef SM750LE_REVISION_ID
#define SM750LE_REVISION_ID ((unsigned char)0xfe)
#endif

#include <linux/io.h>
#include <linux/ioport.h>
#include <linux/uaccess.h>

extern void __iomem *mmio750;

/* software control endianness */
static inline u32 peek32(u32 addr)
{
	return readl(addr + mmio750);
}

static inline void poke32(u32 addr, u32 data)
{
	writel(data, addr + mmio750);
}

/* This is all the chips recognized by this library */
enum logical_chip_type {
	SM_UNKNOWN,
	SM718,
	SM750,
	SM750LE,
};

enum clock_type {
	MXCLK_PLL,
	PRIMARY_PLL,
	SECONDARY_PLL,
	VGA0_PLL,
	VGA1_PLL,
};

struct pll_value {
<<<<<<< HEAD
	enum clock_type clockType;
	unsigned long inputFreq; /* Input clock frequency to the PLL */
=======
	enum clock_type clock_type;
	unsigned long input_freq; /* Input clock frequency to the PLL */
>>>>>>> upstream/android-13

	/* Use this when clockType = PANEL_PLL */
	unsigned long M;
	unsigned long N;
	unsigned long OD;
	unsigned long POD;
};

/* input struct to initChipParam() function */
struct initchip_param {
	/* Use power mode 0 or 1 */
<<<<<<< HEAD
	unsigned short powerMode;
=======
	unsigned short power_mode;
>>>>>>> upstream/android-13

	/*
	 * Speed of main chip clock in MHz unit
	 * 0 = keep the current clock setting
	 * Others = the new main chip clock
	 */
<<<<<<< HEAD
	unsigned short chipClock;
=======
	unsigned short chip_clock;
>>>>>>> upstream/android-13

	/*
	 * Speed of memory clock in MHz unit
	 * 0 = keep the current clock setting
	 * Others = the new memory clock
	 */
<<<<<<< HEAD
	unsigned short memClock;
=======
	unsigned short mem_clock;
>>>>>>> upstream/android-13

	/*
	 * Speed of master clock in MHz unit
	 * 0 = keep the current clock setting
	 * Others = the new master clock
	 */
<<<<<<< HEAD
	unsigned short masterClock;
=======
	unsigned short master_clock;
>>>>>>> upstream/android-13

	/*
	 * 0 = leave all engine state untouched.
	 * 1 = make sure they are off: 2D, Overlay,
	 * video alpha, alpha, hardware cursors
	 */
<<<<<<< HEAD
	unsigned short setAllEngOff;
=======
	unsigned short set_all_eng_off;
>>>>>>> upstream/android-13

	/*
	 * 0 = Do not reset the memory controller
	 * 1 = Reset the memory controller
	 */
<<<<<<< HEAD
	unsigned char resetMemory;
=======
	unsigned char reset_memory;
>>>>>>> upstream/android-13

	/* More initialization parameter can be added if needed */
};

enum logical_chip_type sm750_get_chip_type(void);
<<<<<<< HEAD
void sm750_set_chip_type(unsigned short devId, u8 revId);
unsigned int sm750_calc_pll_value(unsigned int request, struct  pll_value *pll);
unsigned int sm750_format_pll_reg(struct pll_value *pPLL);
=======
void sm750_set_chip_type(unsigned short dev_id, u8 rev_id);
unsigned int sm750_calc_pll_value(unsigned int request, struct  pll_value *pll);
unsigned int sm750_format_pll_reg(struct pll_value *p_PLL);
>>>>>>> upstream/android-13
unsigned int ddk750_get_vm_size(void);
int ddk750_init_hw(struct initchip_param *pinit_param);

#endif
