<<<<<<< HEAD
=======
# SPDX-License-Identifier: GPL-2.0-only
>>>>>>> upstream/android-13
################################################################################
#
# aQuantia Ethernet Controller AQtion Linux Driver
# Copyright(c) 2014-2017 aQuantia Corporation.
#
<<<<<<< HEAD
# This program is free software; you can redistribute it and/or modify it
# under the terms and conditions of the GNU General Public License,
# version 2, as published by the Free Software Foundation.
#
# This program is distributed in the hope it will be useful, but WITHOUT
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
# FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
# more details.
#
# You should have received a copy of the GNU General Public License along
# with this program. If not, see <http://www.gnu.org/licenses/>.
#
# The full GNU General Public License is included in this distribution in
# the file called "COPYING".
#
# Contact Information: <rdc-drv@aquantia.com>
# aQuantia Corporation, 105 E. Tasman Dr. San Jose, CA 95134, USA
#
################################################################################

#
# Makefile for the AQtion(tm) Ethernet driver
#

obj-$(CONFIG_AQTION) += atlantic.o

=======
################################################################################

obj-$(CONFIG_AQTION) += atlantic.o

ccflags-y += -I$(srctree)/$(src)

>>>>>>> upstream/android-13
atlantic-objs := aq_main.o \
	aq_nic.o \
	aq_pci_func.o \
	aq_vec.o \
	aq_ring.o \
	aq_hw_utils.o \
	aq_ethtool.o \
<<<<<<< HEAD
=======
	aq_drvinfo.o \
	aq_filters.o \
	aq_phy.o \
>>>>>>> upstream/android-13
	hw_atl/hw_atl_a0.o \
	hw_atl/hw_atl_b0.o \
	hw_atl/hw_atl_utils.o \
	hw_atl/hw_atl_utils_fw2x.o \
<<<<<<< HEAD
	hw_atl/hw_atl_llh.o
=======
	hw_atl/hw_atl_llh.o \
	hw_atl2/hw_atl2.o \
	hw_atl2/hw_atl2_utils.o \
	hw_atl2/hw_atl2_utils_fw.o \
	hw_atl2/hw_atl2_llh.o \
	macsec/macsec_api.o

atlantic-$(CONFIG_MACSEC) += aq_macsec.o

atlantic-$(CONFIG_PTP_1588_CLOCK) += aq_ptp.o
>>>>>>> upstream/android-13
