<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> upstream/android-13
/*
 * GPIO driver for AMD
 *
 * Copyright (c) 2014,2015 Ken Xue <Ken.Xue@amd.com>
 *		Jeff Wu <Jeff.Wu@amd.com>
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
=======
>>>>>>> upstream/android-13
 */

#ifndef _PINCTRL_AMD_H
#define _PINCTRL_AMD_H

#define AMD_GPIO_PINS_PER_BANK  64

#define AMD_GPIO_PINS_BANK0     63
#define AMD_GPIO_PINS_BANK1     64
#define AMD_GPIO_PINS_BANK2     56
#define AMD_GPIO_PINS_BANK3     32

#define WAKE_INT_MASTER_REG 0xfc
#define EOI_MASK (1 << 29)

#define WAKE_INT_STATUS_REG0 0x2f8
#define WAKE_INT_STATUS_REG1 0x2fc

#define DB_TMR_OUT_OFF			0
#define DB_TMR_OUT_UNIT_OFF		4
#define DB_CNTRL_OFF			5
#define DB_TMR_LARGE_OFF		7
#define LEVEL_TRIG_OFF			8
#define ACTIVE_LEVEL_OFF		9
#define INTERRUPT_ENABLE_OFF		11
#define INTERRUPT_MASK_OFF		12
#define WAKE_CNTRL_OFF_S0I3             13
#define WAKE_CNTRL_OFF_S3               14
#define WAKE_CNTRL_OFF_S4               15
#define PIN_STS_OFF			16
#define DRV_STRENGTH_SEL_OFF		17
#define PULL_UP_SEL_OFF			19
#define PULL_UP_ENABLE_OFF		20
#define PULL_DOWN_ENABLE_OFF		21
#define OUTPUT_VALUE_OFF		22
#define OUTPUT_ENABLE_OFF		23
#define SW_CNTRL_IN_OFF			24
#define SW_CNTRL_EN_OFF			25
#define INTERRUPT_STS_OFF		28
#define WAKE_STS_OFF			29

#define DB_TMR_OUT_MASK	0xFUL
#define DB_CNTRl_MASK	0x3UL
#define ACTIVE_LEVEL_MASK	0x3UL
#define DRV_STRENGTH_SEL_MASK	0x3UL

#define ACTIVE_LEVEL_HIGH	0x0UL
#define ACTIVE_LEVEL_LOW	0x1UL
#define ACTIVE_LEVEL_BOTH	0x2UL

#define DB_TYPE_NO_DEBOUNCE               0x0UL
#define DB_TYPE_PRESERVE_LOW_GLITCH       0x1UL
#define DB_TYPE_PRESERVE_HIGH_GLITCH      0x2UL
#define DB_TYPE_REMOVE_GLITCH             0x3UL

#define EDGE_TRAGGER	0x0UL
#define LEVEL_TRIGGER	0x1UL

#define ACTIVE_HIGH	0x0UL
#define ACTIVE_LOW	0x1UL
#define BOTH_EADGE	0x2UL

#define ENABLE_INTERRUPT	0x1UL
#define DISABLE_INTERRUPT	0x0UL

#define ENABLE_INTERRUPT_MASK	0x0UL
#define DISABLE_INTERRUPT_MASK	0x1UL

#define CLR_INTR_STAT	0x1UL

struct amd_pingroup {
	const char *name;
	const unsigned *pins;
	unsigned npins;
};

struct amd_function {
	const char *name;
	const char * const *groups;
	unsigned ngroups;
};

struct amd_gpio {
	raw_spinlock_t          lock;
	void __iomem            *base;

	const struct amd_pingroup *groups;
	u32 ngroups;
	struct pinctrl_dev *pctrl;
	struct gpio_chip        gc;
	unsigned int            hwbank_num;
	struct resource         *res;
	struct platform_device  *pdev;
	u32			*saved_regs;
<<<<<<< HEAD
=======
	int			irq;
>>>>>>> upstream/android-13
};

/*  KERNCZ configuration*/
static const struct pinctrl_pin_desc kerncz_pins[] = {
	PINCTRL_PIN(0, "GPIO_0"),
	PINCTRL_PIN(1, "GPIO_1"),
	PINCTRL_PIN(2, "GPIO_2"),
	PINCTRL_PIN(3, "GPIO_3"),
	PINCTRL_PIN(4, "GPIO_4"),
	PINCTRL_PIN(5, "GPIO_5"),
	PINCTRL_PIN(6, "GPIO_6"),
	PINCTRL_PIN(7, "GPIO_7"),
	PINCTRL_PIN(8, "GPIO_8"),
	PINCTRL_PIN(9, "GPIO_9"),
	PINCTRL_PIN(10, "GPIO_10"),
	PINCTRL_PIN(11, "GPIO_11"),
	PINCTRL_PIN(12, "GPIO_12"),
	PINCTRL_PIN(13, "GPIO_13"),
	PINCTRL_PIN(14, "GPIO_14"),
	PINCTRL_PIN(15, "GPIO_15"),
	PINCTRL_PIN(16, "GPIO_16"),
	PINCTRL_PIN(17, "GPIO_17"),
	PINCTRL_PIN(18, "GPIO_18"),
	PINCTRL_PIN(19, "GPIO_19"),
	PINCTRL_PIN(20, "GPIO_20"),
<<<<<<< HEAD
=======
	PINCTRL_PIN(21, "GPIO_21"),
	PINCTRL_PIN(22, "GPIO_22"),
>>>>>>> upstream/android-13
	PINCTRL_PIN(23, "GPIO_23"),
	PINCTRL_PIN(24, "GPIO_24"),
	PINCTRL_PIN(25, "GPIO_25"),
	PINCTRL_PIN(26, "GPIO_26"),
<<<<<<< HEAD
	PINCTRL_PIN(39, "GPIO_39"),
	PINCTRL_PIN(40, "GPIO_40"),
	PINCTRL_PIN(43, "GPIO_42"),
=======
	PINCTRL_PIN(27, "GPIO_27"),
	PINCTRL_PIN(28, "GPIO_28"),
	PINCTRL_PIN(29, "GPIO_29"),
	PINCTRL_PIN(30, "GPIO_30"),
	PINCTRL_PIN(31, "GPIO_31"),
	PINCTRL_PIN(32, "GPIO_32"),
	PINCTRL_PIN(33, "GPIO_33"),
	PINCTRL_PIN(34, "GPIO_34"),
	PINCTRL_PIN(35, "GPIO_35"),
	PINCTRL_PIN(36, "GPIO_36"),
	PINCTRL_PIN(37, "GPIO_37"),
	PINCTRL_PIN(38, "GPIO_38"),
	PINCTRL_PIN(39, "GPIO_39"),
	PINCTRL_PIN(40, "GPIO_40"),
	PINCTRL_PIN(41, "GPIO_41"),
	PINCTRL_PIN(42, "GPIO_42"),
	PINCTRL_PIN(43, "GPIO_43"),
	PINCTRL_PIN(44, "GPIO_44"),
	PINCTRL_PIN(45, "GPIO_45"),
>>>>>>> upstream/android-13
	PINCTRL_PIN(46, "GPIO_46"),
	PINCTRL_PIN(47, "GPIO_47"),
	PINCTRL_PIN(48, "GPIO_48"),
	PINCTRL_PIN(49, "GPIO_49"),
	PINCTRL_PIN(50, "GPIO_50"),
	PINCTRL_PIN(51, "GPIO_51"),
	PINCTRL_PIN(52, "GPIO_52"),
	PINCTRL_PIN(53, "GPIO_53"),
	PINCTRL_PIN(54, "GPIO_54"),
	PINCTRL_PIN(55, "GPIO_55"),
	PINCTRL_PIN(56, "GPIO_56"),
	PINCTRL_PIN(57, "GPIO_57"),
	PINCTRL_PIN(58, "GPIO_58"),
	PINCTRL_PIN(59, "GPIO_59"),
	PINCTRL_PIN(60, "GPIO_60"),
	PINCTRL_PIN(61, "GPIO_61"),
	PINCTRL_PIN(62, "GPIO_62"),
	PINCTRL_PIN(64, "GPIO_64"),
	PINCTRL_PIN(65, "GPIO_65"),
	PINCTRL_PIN(66, "GPIO_66"),
<<<<<<< HEAD
=======
	PINCTRL_PIN(67, "GPIO_67"),
>>>>>>> upstream/android-13
	PINCTRL_PIN(68, "GPIO_68"),
	PINCTRL_PIN(69, "GPIO_69"),
	PINCTRL_PIN(70, "GPIO_70"),
	PINCTRL_PIN(71, "GPIO_71"),
	PINCTRL_PIN(72, "GPIO_72"),
<<<<<<< HEAD
	PINCTRL_PIN(74, "GPIO_74"),
	PINCTRL_PIN(75, "GPIO_75"),
	PINCTRL_PIN(76, "GPIO_76"),
=======
	PINCTRL_PIN(73, "GPIO_73"),
	PINCTRL_PIN(74, "GPIO_74"),
	PINCTRL_PIN(75, "GPIO_75"),
	PINCTRL_PIN(76, "GPIO_76"),
	PINCTRL_PIN(77, "GPIO_77"),
	PINCTRL_PIN(78, "GPIO_78"),
	PINCTRL_PIN(79, "GPIO_79"),
	PINCTRL_PIN(80, "GPIO_80"),
	PINCTRL_PIN(81, "GPIO_81"),
	PINCTRL_PIN(82, "GPIO_82"),
	PINCTRL_PIN(83, "GPIO_83"),
>>>>>>> upstream/android-13
	PINCTRL_PIN(84, "GPIO_84"),
	PINCTRL_PIN(85, "GPIO_85"),
	PINCTRL_PIN(86, "GPIO_86"),
	PINCTRL_PIN(87, "GPIO_87"),
	PINCTRL_PIN(88, "GPIO_88"),
	PINCTRL_PIN(89, "GPIO_89"),
	PINCTRL_PIN(90, "GPIO_90"),
	PINCTRL_PIN(91, "GPIO_91"),
	PINCTRL_PIN(92, "GPIO_92"),
	PINCTRL_PIN(93, "GPIO_93"),
<<<<<<< HEAD
=======
	PINCTRL_PIN(94, "GPIO_94"),
>>>>>>> upstream/android-13
	PINCTRL_PIN(95, "GPIO_95"),
	PINCTRL_PIN(96, "GPIO_96"),
	PINCTRL_PIN(97, "GPIO_97"),
	PINCTRL_PIN(98, "GPIO_98"),
	PINCTRL_PIN(99, "GPIO_99"),
	PINCTRL_PIN(100, "GPIO_100"),
	PINCTRL_PIN(101, "GPIO_101"),
	PINCTRL_PIN(102, "GPIO_102"),
<<<<<<< HEAD
=======
	PINCTRL_PIN(103, "GPIO_103"),
	PINCTRL_PIN(104, "GPIO_104"),
	PINCTRL_PIN(105, "GPIO_105"),
	PINCTRL_PIN(106, "GPIO_106"),
	PINCTRL_PIN(107, "GPIO_107"),
	PINCTRL_PIN(108, "GPIO_108"),
	PINCTRL_PIN(109, "GPIO_109"),
	PINCTRL_PIN(110, "GPIO_110"),
	PINCTRL_PIN(111, "GPIO_111"),
	PINCTRL_PIN(112, "GPIO_112"),
>>>>>>> upstream/android-13
	PINCTRL_PIN(113, "GPIO_113"),
	PINCTRL_PIN(114, "GPIO_114"),
	PINCTRL_PIN(115, "GPIO_115"),
	PINCTRL_PIN(116, "GPIO_116"),
	PINCTRL_PIN(117, "GPIO_117"),
	PINCTRL_PIN(118, "GPIO_118"),
	PINCTRL_PIN(119, "GPIO_119"),
	PINCTRL_PIN(120, "GPIO_120"),
	PINCTRL_PIN(121, "GPIO_121"),
	PINCTRL_PIN(122, "GPIO_122"),
<<<<<<< HEAD
	PINCTRL_PIN(126, "GPIO_126"),
=======
	PINCTRL_PIN(123, "GPIO_123"),
	PINCTRL_PIN(124, "GPIO_124"),
	PINCTRL_PIN(125, "GPIO_125"),
	PINCTRL_PIN(126, "GPIO_126"),
	PINCTRL_PIN(127, "GPIO_127"),
	PINCTRL_PIN(128, "GPIO_128"),
>>>>>>> upstream/android-13
	PINCTRL_PIN(129, "GPIO_129"),
	PINCTRL_PIN(130, "GPIO_130"),
	PINCTRL_PIN(131, "GPIO_131"),
	PINCTRL_PIN(132, "GPIO_132"),
	PINCTRL_PIN(133, "GPIO_133"),
<<<<<<< HEAD
=======
	PINCTRL_PIN(134, "GPIO_134"),
>>>>>>> upstream/android-13
	PINCTRL_PIN(135, "GPIO_135"),
	PINCTRL_PIN(136, "GPIO_136"),
	PINCTRL_PIN(137, "GPIO_137"),
	PINCTRL_PIN(138, "GPIO_138"),
	PINCTRL_PIN(139, "GPIO_139"),
	PINCTRL_PIN(140, "GPIO_140"),
	PINCTRL_PIN(141, "GPIO_141"),
	PINCTRL_PIN(142, "GPIO_142"),
	PINCTRL_PIN(143, "GPIO_143"),
	PINCTRL_PIN(144, "GPIO_144"),
	PINCTRL_PIN(145, "GPIO_145"),
	PINCTRL_PIN(146, "GPIO_146"),
	PINCTRL_PIN(147, "GPIO_147"),
	PINCTRL_PIN(148, "GPIO_148"),
<<<<<<< HEAD
=======
	PINCTRL_PIN(149, "GPIO_149"),
	PINCTRL_PIN(150, "GPIO_150"),
	PINCTRL_PIN(151, "GPIO_151"),
	PINCTRL_PIN(152, "GPIO_152"),
	PINCTRL_PIN(153, "GPIO_153"),
	PINCTRL_PIN(154, "GPIO_154"),
	PINCTRL_PIN(155, "GPIO_155"),
	PINCTRL_PIN(156, "GPIO_156"),
	PINCTRL_PIN(157, "GPIO_157"),
	PINCTRL_PIN(158, "GPIO_158"),
	PINCTRL_PIN(159, "GPIO_159"),
	PINCTRL_PIN(160, "GPIO_160"),
	PINCTRL_PIN(161, "GPIO_161"),
	PINCTRL_PIN(162, "GPIO_162"),
	PINCTRL_PIN(163, "GPIO_163"),
	PINCTRL_PIN(164, "GPIO_164"),
	PINCTRL_PIN(165, "GPIO_165"),
>>>>>>> upstream/android-13
	PINCTRL_PIN(166, "GPIO_166"),
	PINCTRL_PIN(167, "GPIO_167"),
	PINCTRL_PIN(168, "GPIO_168"),
	PINCTRL_PIN(169, "GPIO_169"),
	PINCTRL_PIN(170, "GPIO_170"),
	PINCTRL_PIN(171, "GPIO_171"),
	PINCTRL_PIN(172, "GPIO_172"),
	PINCTRL_PIN(173, "GPIO_173"),
	PINCTRL_PIN(174, "GPIO_174"),
	PINCTRL_PIN(175, "GPIO_175"),
	PINCTRL_PIN(176, "GPIO_176"),
	PINCTRL_PIN(177, "GPIO_177"),
<<<<<<< HEAD
=======
	PINCTRL_PIN(178, "GPIO_178"),
	PINCTRL_PIN(179, "GPIO_179"),
	PINCTRL_PIN(180, "GPIO_180"),
	PINCTRL_PIN(181, "GPIO_181"),
	PINCTRL_PIN(182, "GPIO_182"),
	PINCTRL_PIN(183, "GPIO_183"),
>>>>>>> upstream/android-13
};

static const unsigned i2c0_pins[] = {145, 146};
static const unsigned i2c1_pins[] = {147, 148};
static const unsigned i2c2_pins[] = {113, 114};
static const unsigned i2c3_pins[] = {19, 20};

static const unsigned uart0_pins[] = {135, 136, 137, 138, 139};
static const unsigned uart1_pins[] = {140, 141, 142, 143, 144};

static const struct amd_pingroup kerncz_groups[] = {
	{
		.name = "i2c0",
		.pins = i2c0_pins,
		.npins = 2,
	},
	{
		.name = "i2c1",
		.pins = i2c1_pins,
		.npins = 2,
	},
	{
		.name = "i2c2",
		.pins = i2c2_pins,
		.npins = 2,
	},
	{
		.name = "i2c3",
		.pins = i2c3_pins,
		.npins = 2,
	},
	{
		.name = "uart0",
		.pins = uart0_pins,
		.npins = 5,
	},
	{
		.name = "uart1",
		.pins = uart1_pins,
		.npins = 5,
	},
};

#endif
