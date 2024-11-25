=================================
target and iSCSI Interfaces Guide
=================================

Introduction and Overview
=========================

TBD

Target core device interfaces
=============================

<<<<<<< HEAD
.. kernel-doc:: drivers/target/target_core_device.c
    :export:
=======
This section is blank because no kerneldoc comments have been added to
drivers/target/target_core_device.c.
>>>>>>> upstream/android-13

Target core transport interfaces
================================

.. kernel-doc:: drivers/target/target_core_transport.c
    :export:

Target-supported userspace I/O
==============================

.. kernel-doc:: drivers/target/target_core_user.c
    :doc: Userspace I/O

.. kernel-doc:: include/uapi/linux/target_core_user.h
    :doc: Ring Design

iSCSI helper functions
======================

.. kernel-doc:: drivers/scsi/libiscsi.c
   :export:


iSCSI boot information
======================

.. kernel-doc:: drivers/scsi/iscsi_boot_sysfs.c
   :export:

<<<<<<< HEAD

iSCSI transport class
=====================

The file drivers/scsi/scsi_transport_iscsi.c defines transport
attributes for the iSCSI class, which sends SCSI packets over TCP/IP
connections.

.. kernel-doc:: drivers/scsi/scsi_transport_iscsi.c
   :export:


=======
>>>>>>> upstream/android-13
iSCSI TCP interfaces
====================

.. kernel-doc:: drivers/scsi/iscsi_tcp.c
   :internal:

.. kernel-doc:: drivers/scsi/libiscsi_tcp.c
   :export:

