<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-or-later */
>>>>>>> upstream/android-13
/*
	Copyright (C) 2004 - 2009 Ivo van Doorn <IvDoorn@gmail.com>
	<http://rt2x00.serialmonkey.com>

<<<<<<< HEAD
	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, see <http://www.gnu.org/licenses/>.
=======
>>>>>>> upstream/android-13
 */

/*
	Module: rt2x00pci
	Abstract: Data structures for the rt2x00pci module.
 */

#ifndef RT2X00PCI_H
#define RT2X00PCI_H

#include <linux/io.h>
#include <linux/pci.h>

/*
 * PCI driver handlers.
 */
int rt2x00pci_probe(struct pci_dev *pci_dev, const struct rt2x00_ops *ops);
void rt2x00pci_remove(struct pci_dev *pci_dev);
<<<<<<< HEAD
#ifdef CONFIG_PM
int rt2x00pci_suspend(struct pci_dev *pci_dev, pm_message_t state);
int rt2x00pci_resume(struct pci_dev *pci_dev);
#else
#define rt2x00pci_suspend	NULL
#define rt2x00pci_resume	NULL
#endif /* CONFIG_PM */
=======

extern const struct dev_pm_ops rt2x00pci_pm_ops;
>>>>>>> upstream/android-13

#endif /* RT2X00PCI_H */
