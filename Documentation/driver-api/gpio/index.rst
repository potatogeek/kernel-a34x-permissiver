===================================
General Purpose Input/Output (GPIO)
===================================

Contents:

.. toctree::
   :maxdepth: 2

   intro
<<<<<<< HEAD
=======
   using-gpio
>>>>>>> upstream/android-13
   driver
   consumer
   board
   drivers-on-gpio
   legacy
<<<<<<< HEAD
=======
   bt8xxgpio
>>>>>>> upstream/android-13

Core
====

.. kernel-doc:: include/linux/gpio/driver.h
   :internal:

.. kernel-doc:: drivers/gpio/gpiolib.c
   :export:

ACPI support
============

.. kernel-doc:: drivers/gpio/gpiolib-acpi.c
   :export:

Device tree support
===================

.. kernel-doc:: drivers/gpio/gpiolib-of.c
   :export:

Device-managed API
==================

<<<<<<< HEAD
.. kernel-doc:: drivers/gpio/devres.c
=======
.. kernel-doc:: drivers/gpio/gpiolib-devres.c
>>>>>>> upstream/android-13
   :export:

sysfs helpers
=============

.. kernel-doc:: drivers/gpio/gpiolib-sysfs.c
   :export:
