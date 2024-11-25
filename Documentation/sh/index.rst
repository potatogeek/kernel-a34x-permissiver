=======================
SuperH Interfaces Guide
=======================

:Author: Paul Mundt

<<<<<<< HEAD
=======
.. toctree::
    :maxdepth: 1

    booting
    new-machine
    register-banks

    features

>>>>>>> upstream/android-13
Memory Management
=================

SH-4
----

Store Queue API
~~~~~~~~~~~~~~~

.. kernel-doc:: arch/sh/kernel/cpu/sh4/sq.c
   :export:

<<<<<<< HEAD
SH-5
----

TLB Interfaces
~~~~~~~~~~~~~~

.. kernel-doc:: arch/sh/mm/tlb-sh5.c
   :internal:

.. kernel-doc:: arch/sh/include/asm/tlb_64.h
   :internal:

=======
>>>>>>> upstream/android-13
Machine Specific Interfaces
===========================

mach-dreamcast
--------------

.. kernel-doc:: arch/sh/boards/mach-dreamcast/rtc.c
   :internal:

mach-x3proto
------------

.. kernel-doc:: arch/sh/boards/mach-x3proto/ilsel.c
   :export:

Busses
======

SuperHyway
----------

.. kernel-doc:: drivers/sh/superhyway/superhyway.c
   :export:

Maple
-----

.. kernel-doc:: drivers/sh/maple/maple.c
   :export:
