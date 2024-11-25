FPGA Bridge
===========

API to implement a new FPGA bridge
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

<<<<<<< HEAD
=======
* struct fpga_bridge - The FPGA Bridge structure
* struct fpga_bridge_ops - Low level Bridge driver ops
* devm_fpga_bridge_create() - Allocate and init a bridge struct
* fpga_bridge_register() - Register a bridge
* fpga_bridge_unregister() - Unregister a bridge

>>>>>>> upstream/android-13
.. kernel-doc:: include/linux/fpga/fpga-bridge.h
   :functions: fpga_bridge

.. kernel-doc:: include/linux/fpga/fpga-bridge.h
   :functions: fpga_bridge_ops

.. kernel-doc:: drivers/fpga/fpga-bridge.c
<<<<<<< HEAD
   :functions: fpga_bridge_create

.. kernel-doc:: drivers/fpga/fpga-bridge.c
   :functions: fpga_bridge_free
=======
   :functions: devm_fpga_bridge_create
>>>>>>> upstream/android-13

.. kernel-doc:: drivers/fpga/fpga-bridge.c
   :functions: fpga_bridge_register

.. kernel-doc:: drivers/fpga/fpga-bridge.c
   :functions: fpga_bridge_unregister
<<<<<<< HEAD

API to control an FPGA bridge
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

You probably won't need these directly.  FPGA regions should handle this.

.. kernel-doc:: drivers/fpga/fpga-bridge.c
   :functions: of_fpga_bridge_get

.. kernel-doc:: drivers/fpga/fpga-bridge.c
   :functions: fpga_bridge_get

.. kernel-doc:: drivers/fpga/fpga-bridge.c
   :functions: fpga_bridge_put

.. kernel-doc:: drivers/fpga/fpga-bridge.c
   :functions: fpga_bridge_get_to_list

.. kernel-doc:: drivers/fpga/fpga-bridge.c
   :functions: of_fpga_bridge_get_to_list

.. kernel-doc:: drivers/fpga/fpga-bridge.c
   :functions: fpga_bridge_enable

.. kernel-doc:: drivers/fpga/fpga-bridge.c
   :functions: fpga_bridge_disable
=======
>>>>>>> upstream/android-13
