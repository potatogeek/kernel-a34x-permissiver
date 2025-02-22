=============================
Mode Setting Helper Functions
=============================

The DRM subsystem aims for a strong separation between core code and helper
libraries. Core code takes care of general setup and teardown and decoding
userspace requests to kernel internal objects. Everything else is handled by a
large set of helper libraries, which can be combined freely to pick and choose
for each driver what fits, and avoid shared code where special behaviour is
needed.

This distinction between core code and helpers is especially strong in the
modesetting code, where there's a shared userspace ABI for all drivers. This is
in contrast to the render side, where pretty much everything (with very few
exceptions) can be considered optional helper code.

There are a few areas these helpers can grouped into:

* Helpers to implement modesetting. The important ones here are the atomic
  helpers. Old drivers still often use the legacy CRTC helpers. They both share
  the same set of common helper vtables. For really simple drivers (anything
  that would have been a great fit in the deprecated fbdev subsystem) there's
  also the simple display pipe helpers.

* There's a big pile of helpers for handling outputs. First the generic bridge
  helpers for handling encoder and transcoder IP blocks. Second the panel helpers
  for handling panel-related information and logic. Plus then a big set of
  helpers for the various sink standards (DisplayPort, HDMI, MIPI DSI). Finally
  there's also generic helpers for handling output probing, and for dealing with
  EDIDs.

* The last group of helpers concerns itself with the frontend side of a display
  pipeline: Planes, handling rectangles for visibility checking and scissoring,
  flip queues and assorted bits.

Modeset Helper Reference for Common Vtables
===========================================

.. kernel-doc:: include/drm/drm_modeset_helper_vtables.h
   :doc: overview

.. kernel-doc:: include/drm/drm_modeset_helper_vtables.h
   :internal:

.. _drm_atomic_helper:

Atomic Modeset Helper Functions Reference
=========================================

Overview
--------

.. kernel-doc:: drivers/gpu/drm/drm_atomic_helper.c
   :doc: overview

Implementing Asynchronous Atomic Commit
---------------------------------------

.. kernel-doc:: drivers/gpu/drm/drm_atomic_helper.c
   :doc: implementing nonblocking commit

<<<<<<< HEAD
Atomic State Reset and Initialization
-------------------------------------

.. kernel-doc:: drivers/gpu/drm/drm_atomic_helper.c
   :doc: atomic state reset and initialization

=======
>>>>>>> upstream/android-13
Helper Functions Reference
--------------------------

.. kernel-doc:: include/drm/drm_atomic_helper.h
   :internal:

.. kernel-doc:: drivers/gpu/drm/drm_atomic_helper.c
   :export:

<<<<<<< HEAD
=======
Atomic State Reset and Initialization
-------------------------------------

.. kernel-doc:: drivers/gpu/drm/drm_atomic_state_helper.c
   :doc: atomic state reset and initialization

Atomic State Helper Reference
-----------------------------

.. kernel-doc:: drivers/gpu/drm/drm_atomic_state_helper.c
   :export:

GEM Atomic Helper Reference
---------------------------

.. kernel-doc:: drivers/gpu/drm/drm_gem_atomic_helper.c
   :doc: overview

.. kernel-doc:: include/drm/drm_gem_atomic_helper.h
   :internal:

.. kernel-doc:: drivers/gpu/drm/drm_gem_atomic_helper.c
   :export:

>>>>>>> upstream/android-13
Simple KMS Helper Reference
===========================

.. kernel-doc:: drivers/gpu/drm/drm_simple_kms_helper.c
   :doc: overview

.. kernel-doc:: include/drm/drm_simple_kms_helper.h
   :internal:

.. kernel-doc:: drivers/gpu/drm/drm_simple_kms_helper.c
   :export:

fbdev Helper Functions Reference
================================

.. kernel-doc:: drivers/gpu/drm/drm_fb_helper.c
   :doc: fbdev helpers

.. kernel-doc:: include/drm/drm_fb_helper.h
   :internal:

.. kernel-doc:: drivers/gpu/drm/drm_fb_helper.c
   :export:

<<<<<<< HEAD
=======
format Helper Functions Reference
=================================

.. kernel-doc:: drivers/gpu/drm/drm_format_helper.c
   :export:

>>>>>>> upstream/android-13
Framebuffer CMA Helper Functions Reference
==========================================

.. kernel-doc:: drivers/gpu/drm/drm_fb_cma_helper.c
   :doc: framebuffer cma helper functions

.. kernel-doc:: drivers/gpu/drm/drm_fb_cma_helper.c
   :export:

<<<<<<< HEAD
.. _drm_bridges:

=======
>>>>>>> upstream/android-13
Framebuffer GEM Helper Reference
================================

.. kernel-doc:: drivers/gpu/drm/drm_gem_framebuffer_helper.c
   :doc: overview

.. kernel-doc:: drivers/gpu/drm/drm_gem_framebuffer_helper.c
   :export:

<<<<<<< HEAD
=======
.. _drm_bridges:

>>>>>>> upstream/android-13
Bridges
=======

Overview
--------

.. kernel-doc:: drivers/gpu/drm/drm_bridge.c
   :doc: overview

<<<<<<< HEAD
Default bridge callback sequence
--------------------------------

.. kernel-doc:: drivers/gpu/drm/drm_bridge.c
   :doc: bridge callbacks
=======
Bridge Operations
-----------------

.. kernel-doc:: drivers/gpu/drm/drm_bridge.c
   :doc: bridge operations

Bridge Connector Helper
-----------------------

.. kernel-doc:: drivers/gpu/drm/drm_bridge_connector.c
   :doc: overview
>>>>>>> upstream/android-13


Bridge Helper Reference
-------------------------

.. kernel-doc:: include/drm/drm_bridge.h
   :internal:

.. kernel-doc:: drivers/gpu/drm/drm_bridge.c
   :export:

<<<<<<< HEAD
=======
Bridge Connector Helper Reference
---------------------------------

.. kernel-doc:: drivers/gpu/drm/drm_bridge_connector.c
   :export:

>>>>>>> upstream/android-13
Panel-Bridge Helper Reference
-----------------------------

.. kernel-doc:: drivers/gpu/drm/bridge/panel.c
   :export:

.. _drm_panel_helper:

Panel Helper Reference
======================

.. kernel-doc:: drivers/gpu/drm/drm_panel.c
   :doc: drm panel

.. kernel-doc:: include/drm/drm_panel.h
   :internal:

.. kernel-doc:: drivers/gpu/drm/drm_panel.c
   :export:

.. kernel-doc:: drivers/gpu/drm/drm_panel_orientation_quirks.c
   :export:

<<<<<<< HEAD
=======
Panel Self Refresh Helper Reference
===================================

.. kernel-doc:: drivers/gpu/drm/drm_self_refresh_helper.c
   :doc: overview

.. kernel-doc:: drivers/gpu/drm/drm_self_refresh_helper.c
   :export:

HDCP Helper Functions Reference
===============================

.. kernel-doc:: drivers/gpu/drm/drm_hdcp.c
   :export:

>>>>>>> upstream/android-13
Display Port Helper Functions Reference
=======================================

.. kernel-doc:: drivers/gpu/drm/drm_dp_helper.c
   :doc: dp helpers

.. kernel-doc:: include/drm/drm_dp_helper.h
   :internal:

.. kernel-doc:: drivers/gpu/drm/drm_dp_helper.c
   :export:

Display Port CEC Helper Functions Reference
===========================================

.. kernel-doc:: drivers/gpu/drm/drm_dp_cec.c
   :doc: dp cec helpers

.. kernel-doc:: drivers/gpu/drm/drm_dp_cec.c
   :export:

Display Port Dual Mode Adaptor Helper Functions Reference
=========================================================

.. kernel-doc:: drivers/gpu/drm/drm_dp_dual_mode_helper.c
   :doc: dp dual mode helpers

.. kernel-doc:: include/drm/drm_dp_dual_mode_helper.h
   :internal:

.. kernel-doc:: drivers/gpu/drm/drm_dp_dual_mode_helper.c
   :export:

<<<<<<< HEAD
Display Port MST Helper Functions Reference
===========================================
=======
Display Port MST Helpers
========================

Overview
--------
>>>>>>> upstream/android-13

.. kernel-doc:: drivers/gpu/drm/drm_dp_mst_topology.c
   :doc: dp mst helper

<<<<<<< HEAD
=======
.. kernel-doc:: drivers/gpu/drm/drm_dp_mst_topology.c
   :doc: Branch device and port refcounting

Functions Reference
-------------------

>>>>>>> upstream/android-13
.. kernel-doc:: include/drm/drm_dp_mst_helper.h
   :internal:

.. kernel-doc:: drivers/gpu/drm/drm_dp_mst_topology.c
   :export:

<<<<<<< HEAD
=======
Topology Lifetime Internals
---------------------------

These functions aren't exported to drivers, but are documented here to help make
the MST topology helpers easier to understand

.. kernel-doc:: drivers/gpu/drm/drm_dp_mst_topology.c
   :functions: drm_dp_mst_topology_try_get_mstb drm_dp_mst_topology_get_mstb
               drm_dp_mst_topology_put_mstb
               drm_dp_mst_topology_try_get_port drm_dp_mst_topology_get_port
               drm_dp_mst_topology_put_port
               drm_dp_mst_get_mstb_malloc drm_dp_mst_put_mstb_malloc

MIPI DBI Helper Functions Reference
===================================

.. kernel-doc:: drivers/gpu/drm/drm_mipi_dbi.c
   :doc: overview

.. kernel-doc:: include/drm/drm_mipi_dbi.h
   :internal:

.. kernel-doc:: drivers/gpu/drm/drm_mipi_dbi.c
   :export:

>>>>>>> upstream/android-13
MIPI DSI Helper Functions Reference
===================================

.. kernel-doc:: drivers/gpu/drm/drm_mipi_dsi.c
   :doc: dsi helpers

.. kernel-doc:: include/drm/drm_mipi_dsi.h
   :internal:

.. kernel-doc:: drivers/gpu/drm/drm_mipi_dsi.c
   :export:

<<<<<<< HEAD
=======
Display Stream Compression Helper Functions Reference
=====================================================

.. kernel-doc:: drivers/gpu/drm/drm_dsc.c
   :doc: dsc helpers

.. kernel-doc:: include/drm/drm_dsc.h
   :internal:

.. kernel-doc:: drivers/gpu/drm/drm_dsc.c
   :export:

>>>>>>> upstream/android-13
Output Probing Helper Functions Reference
=========================================

.. kernel-doc:: drivers/gpu/drm/drm_probe_helper.c
   :doc: output probing helper overview

.. kernel-doc:: drivers/gpu/drm/drm_probe_helper.c
   :export:

EDID Helper Functions Reference
===============================

.. kernel-doc:: include/drm/drm_edid.h
   :internal:

.. kernel-doc:: drivers/gpu/drm/drm_edid.c
   :export:

SCDC Helper Functions Reference
===============================

.. kernel-doc:: drivers/gpu/drm/drm_scdc_helper.c
   :doc: scdc helpers

.. kernel-doc:: include/drm/drm_scdc_helper.h
   :internal:

.. kernel-doc:: drivers/gpu/drm/drm_scdc_helper.c
   :export:

<<<<<<< HEAD
Rectangle Utilities Reference
=============================

.. kernel-doc:: include/drm/drm_rect.h
   :doc: rect utils

.. kernel-doc:: include/drm/drm_rect.h
   :internal:

.. kernel-doc:: drivers/gpu/drm/drm_rect.c
   :export:

=======
>>>>>>> upstream/android-13
HDMI Infoframes Helper Reference
================================

Strictly speaking this is not a DRM helper library but generally useable
by any driver interfacing with HDMI outputs like v4l or alsa drivers.
But it nicely fits into the overall topic of mode setting helper
libraries and hence is also included here.

.. kernel-doc:: include/linux/hdmi.h
   :internal:

.. kernel-doc:: drivers/video/hdmi.c
   :export:

<<<<<<< HEAD
=======
Rectangle Utilities Reference
=============================

.. kernel-doc:: include/drm/drm_rect.h
   :doc: rect utils

.. kernel-doc:: include/drm/drm_rect.h
   :internal:

.. kernel-doc:: drivers/gpu/drm/drm_rect.c
   :export:

>>>>>>> upstream/android-13
Flip-work Helper Reference
==========================

.. kernel-doc:: include/drm/drm_flip_work.h
   :doc: flip utils

.. kernel-doc:: include/drm/drm_flip_work.h
   :internal:

.. kernel-doc:: drivers/gpu/drm/drm_flip_work.c
   :export:

Auxiliary Modeset Helpers
=========================

.. kernel-doc:: drivers/gpu/drm/drm_modeset_helper.c
   :doc: aux kms helpers

.. kernel-doc:: drivers/gpu/drm/drm_modeset_helper.c
   :export:

OF/DT Helpers
=============

.. kernel-doc:: drivers/gpu/drm/drm_of.c
   :doc: overview

.. kernel-doc:: drivers/gpu/drm/drm_of.c
   :export:

Legacy Plane Helper Reference
=============================

.. kernel-doc:: drivers/gpu/drm/drm_plane_helper.c
   :doc: overview

.. kernel-doc:: drivers/gpu/drm/drm_plane_helper.c
   :export:

Legacy CRTC/Modeset Helper Functions Reference
==============================================

.. kernel-doc:: drivers/gpu/drm/drm_crtc_helper.c
   :doc: overview

.. kernel-doc:: drivers/gpu/drm/drm_crtc_helper.c
   :export:
