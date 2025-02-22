Device drivers infrastructure
=============================

The Basic Device Driver-Model Structures
----------------------------------------

.. kernel-doc:: include/linux/device.h
   :internal:
<<<<<<< HEAD
=======
   :no-identifiers: device_link_state
>>>>>>> upstream/android-13

Device Drivers Base
-------------------

.. kernel-doc:: drivers/base/init.c
   :internal:

.. kernel-doc:: drivers/base/driver.c
   :export:

.. kernel-doc:: drivers/base/core.c
   :export:

.. kernel-doc:: drivers/base/syscore.c
   :export:

.. kernel-doc:: drivers/base/class.c
   :export:

.. kernel-doc:: drivers/base/node.c
   :internal:

<<<<<<< HEAD
.. kernel-doc:: drivers/base/firmware_loader/main.c
   :export:

=======
>>>>>>> upstream/android-13
.. kernel-doc:: drivers/base/transport_class.c
   :export:

.. kernel-doc:: drivers/base/dd.c
   :export:

.. kernel-doc:: include/linux/platform_device.h
   :internal:

.. kernel-doc:: drivers/base/platform.c
   :export:

.. kernel-doc:: drivers/base/bus.c
   :export:

Device Drivers DMA Management
-----------------------------

<<<<<<< HEAD
.. kernel-doc:: kernel/dma/coherent.c
   :export:

=======
>>>>>>> upstream/android-13
.. kernel-doc:: kernel/dma/mapping.c
   :export:

Device drivers PnP support
--------------------------

.. kernel-doc:: drivers/pnp/core.c
   :internal:

.. kernel-doc:: drivers/pnp/card.c
   :export:

.. kernel-doc:: drivers/pnp/driver.c
   :internal:

.. kernel-doc:: drivers/pnp/manager.c
   :export:

.. kernel-doc:: drivers/pnp/support.c
   :export:

Userspace IO devices
--------------------

.. kernel-doc:: drivers/uio/uio.c
   :export:

.. kernel-doc:: include/linux/uio_driver.h
   :internal:

