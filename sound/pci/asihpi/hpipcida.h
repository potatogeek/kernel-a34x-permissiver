<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> upstream/android-13
/******************************************************************************

    AudioScience HPI driver
    Copyright (C) 1997-2011  AudioScience Inc. <support@audioscience.com>

<<<<<<< HEAD
    This program is free software; you can redistribute it and/or modify
    it under the terms of version 2 of the GNU General Public License as
    published by the Free Software Foundation;

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
=======
>>>>>>> upstream/android-13

 Array initializer for PCI card IDs

(C) Copyright AudioScience Inc. 1998-2003
*******************************************************************************/

/*NOTE: when adding new lines to this header file
  they MUST be grouped by HPI entry point.
*/

{
HPI_PCI_VENDOR_ID_TI, HPI_PCI_DEV_ID_DSP6205,
		HPI_PCI_VENDOR_ID_AUDIOSCIENCE, PCI_ANY_ID, 0, 0,
		(kernel_ulong_t) HPI_6205}
, {
HPI_PCI_VENDOR_ID_TI, HPI_PCI_DEV_ID_PCI2040,
		HPI_PCI_VENDOR_ID_AUDIOSCIENCE, PCI_ANY_ID, 0, 0,
		(kernel_ulong_t) HPI_6000}
, {
0}
