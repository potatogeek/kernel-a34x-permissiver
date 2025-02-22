=========================
Kernel Mode Setting (KMS)
=========================

Drivers must initialize the mode setting core by calling
<<<<<<< HEAD
:c:func:`drm_mode_config_init()` on the DRM device. The function
=======
drmm_mode_config_init() on the DRM device. The function
>>>>>>> upstream/android-13
initializes the :c:type:`struct drm_device <drm_device>`
mode_config field and never fails. Once done, mode configuration must
be setup by initializing the following fields.

-  int min_width, min_height; int max_width, max_height;
   Minimum and maximum width and height of the frame buffers in pixel
   units.

-  struct drm_mode_config_funcs \*funcs;
   Mode setting functions.

Overview
========

.. kernel-render:: DOT
   :alt: KMS Display Pipeline
   :caption: KMS Display Pipeline Overview

   digraph "KMS" {
      node [shape=box]

      subgraph cluster_static {
          style=dashed
          label="Static Objects"

          node [bgcolor=grey style=filled]
          "drm_plane A" -> "drm_crtc"
          "drm_plane B" -> "drm_crtc"
          "drm_crtc" -> "drm_encoder A"
          "drm_crtc" -> "drm_encoder B"
      }

      subgraph cluster_user_created {
          style=dashed
          label="Userspace-Created"

          node [shape=oval]
          "drm_framebuffer 1" -> "drm_plane A"
          "drm_framebuffer 2" -> "drm_plane B"
      }

      subgraph cluster_connector {
          style=dashed
          label="Hotpluggable"

          "drm_encoder A" -> "drm_connector A"
          "drm_encoder B" -> "drm_connector B"
      }
   }

The basic object structure KMS presents to userspace is fairly simple.
Framebuffers (represented by :c:type:`struct drm_framebuffer <drm_framebuffer>`,
see `Frame Buffer Abstraction`_) feed into planes. Planes are represented by
:c:type:`struct drm_plane <drm_plane>`, see `Plane Abstraction`_ for more
details. One or more (or even no) planes feed their pixel data into a CRTC
(represented by :c:type:`struct drm_crtc <drm_crtc>`, see `CRTC Abstraction`_)
for blending. The precise blending step is explained in more detail in `Plane
Composition Properties`_ and related chapters.

For the output routing the first step is encoders (represented by
:c:type:`struct drm_encoder <drm_encoder>`, see `Encoder Abstraction`_). Those
are really just internal artifacts of the helper libraries used to implement KMS
drivers. Besides that they make it unecessarily more complicated for userspace
to figure out which connections between a CRTC and a connector are possible, and
what kind of cloning is supported, they serve no purpose in the userspace API.
Unfortunately encoders have been exposed to userspace, hence can't remove them
at this point.  Futhermore the exposed restrictions are often wrongly set by
drivers, and in many cases not powerful enough to express the real restrictions.
A CRTC can be connected to multiple encoders, and for an active CRTC there must
be at least one encoder.

The final, and real, endpoint in the display chain is the connector (represented
by :c:type:`struct drm_connector <drm_connector>`, see `Connector
Abstraction`_). Connectors can have different possible encoders, but the kernel
driver selects which encoder to use for each connector. The use case is DVI,
which could switch between an analog and a digital encoder. Encoders can also
drive multiple different connectors. There is exactly one active connector for
every active encoder.

Internally the output pipeline is a bit more complex and matches today's
hardware more closely:

.. kernel-render:: DOT
   :alt: KMS Output Pipeline
   :caption: KMS Output Pipeline

   digraph "Output Pipeline" {
      node [shape=box]

      subgraph {
          "drm_crtc" [bgcolor=grey style=filled]
      }

      subgraph cluster_internal {
          style=dashed
          label="Internal Pipeline"
          {
              node [bgcolor=grey style=filled]
              "drm_encoder A";
              "drm_encoder B";
              "drm_encoder C";
          }

          {
              node [bgcolor=grey style=filled]
              "drm_encoder B" -> "drm_bridge B"
              "drm_encoder C" -> "drm_bridge C1"
              "drm_bridge C1" -> "drm_bridge C2";
          }
      }

      "drm_crtc" -> "drm_encoder A"
      "drm_crtc" -> "drm_encoder B"
      "drm_crtc" -> "drm_encoder C"


      subgraph cluster_output {
          style=dashed
          label="Outputs"

          "drm_encoder A" -> "drm_connector A";
          "drm_bridge B" -> "drm_connector B";
          "drm_bridge C2" -> "drm_connector C";

          "drm_panel"
      }
   }

Internally two additional helper objects come into play. First, to be able to
share code for encoders (sometimes on the same SoC, sometimes off-chip) one or
more :ref:`drm_bridges` (represented by :c:type:`struct drm_bridge
<drm_bridge>`) can be linked to an encoder. This link is static and cannot be
changed, which means the cross-bar (if there is any) needs to be mapped between
the CRTC and any encoders. Often for drivers with bridges there's no code left
at the encoder level. Atomic drivers can leave out all the encoder callbacks to
essentially only leave a dummy routing object behind, which is needed for
backwards compatibility since encoders are exposed to userspace.

The second object is for panels, represented by :c:type:`struct drm_panel
<drm_panel>`, see :ref:`drm_panel_helper`. Panels do not have a fixed binding
point, but are generally linked to the driver private structure that embeds
:c:type:`struct drm_connector <drm_connector>`.

Note that currently the bridge chaining and interactions with connectors and
panels are still in-flux and not really fully sorted out yet.

KMS Core Structures and Functions
=================================

.. kernel-doc:: include/drm/drm_mode_config.h
   :internal:

.. kernel-doc:: drivers/gpu/drm/drm_mode_config.c
   :export:

<<<<<<< HEAD
=======
.. _kms_base_object_abstraction:

>>>>>>> upstream/android-13
Modeset Base Object Abstraction
===============================

.. kernel-render:: DOT
   :alt: Mode Objects and Properties
   :caption: Mode Objects and Properties

   digraph {
      node [shape=box]

      "drm_property A" -> "drm_mode_object A"
      "drm_property A" -> "drm_mode_object B"
      "drm_property B" -> "drm_mode_object A"
   }

The base structure for all KMS objects is :c:type:`struct drm_mode_object
<drm_mode_object>`. One of the base services it provides is tracking properties,
which are especially important for the atomic IOCTL (see `Atomic Mode
Setting`_). The somewhat surprising part here is that properties are not
directly instantiated on each object, but free-standing mode objects themselves,
represented by :c:type:`struct drm_property <drm_property>`, which only specify
the type and value range of a property. Any given property can be attached
<<<<<<< HEAD
multiple times to different objects using :c:func:`drm_object_attach_property()
<drm_object_attach_property>`.
=======
multiple times to different objects using drm_object_attach_property().
>>>>>>> upstream/android-13

.. kernel-doc:: include/drm/drm_mode_object.h
   :internal:

.. kernel-doc:: drivers/gpu/drm/drm_mode_object.c
   :export:

Atomic Mode Setting
===================


.. kernel-render:: DOT
   :alt: Mode Objects and Properties
   :caption: Mode Objects and Properties

   digraph {
      node [shape=box]

      subgraph cluster_state {
          style=dashed
          label="Free-standing state"

          "drm_atomic_state" -> "duplicated drm_plane_state A"
          "drm_atomic_state" -> "duplicated drm_plane_state B"
          "drm_atomic_state" -> "duplicated drm_crtc_state"
          "drm_atomic_state" -> "duplicated drm_connector_state"
          "drm_atomic_state" -> "duplicated driver private state"
      }

      subgraph cluster_current {
          style=dashed
          label="Current state"

          "drm_device" -> "drm_plane A"
          "drm_device" -> "drm_plane B"
          "drm_device" -> "drm_crtc"
          "drm_device" -> "drm_connector"
          "drm_device" -> "driver private object"

          "drm_plane A" -> "drm_plane_state A"
          "drm_plane B" -> "drm_plane_state B"
          "drm_crtc" -> "drm_crtc_state"
          "drm_connector" -> "drm_connector_state"
          "driver private object" -> "driver private state"
      }

      "drm_atomic_state" -> "drm_device" [label="atomic_commit"]
      "duplicated drm_plane_state A" -> "drm_device"[style=invis]
   }

Atomic provides transactional modeset (including planes) updates, but a
bit differently from the usual transactional approach of try-commit and
rollback:

- Firstly, no hardware changes are allowed when the commit would fail. This
  allows us to implement the DRM_MODE_ATOMIC_TEST_ONLY mode, which allows
  userspace to explore whether certain configurations would work or not.

- This would still allow setting and rollback of just the software state,
  simplifying conversion of existing drivers. But auditing drivers for
  correctness of the atomic_check code becomes really hard with that: Rolling
  back changes in data structures all over the place is hard to get right.

- Lastly, for backwards compatibility and to support all use-cases, atomic
  updates need to be incremental and be able to execute in parallel. Hardware
  doesn't always allow it, but where possible plane updates on different CRTCs
  should not interfere, and not get stalled due to output routing changing on
  different CRTCs.

Taken all together there's two consequences for the atomic design:

- The overall state is split up into per-object state structures:
  :c:type:`struct drm_plane_state <drm_plane_state>` for planes, :c:type:`struct
  drm_crtc_state <drm_crtc_state>` for CRTCs and :c:type:`struct
  drm_connector_state <drm_connector_state>` for connectors. These are the only
  objects with userspace-visible and settable state. For internal state drivers
  can subclass these structures through embeddeding, or add entirely new state
<<<<<<< HEAD
  structures for their globally shared hardware functions.
=======
  structures for their globally shared hardware functions, see :c:type:`struct
  drm_private_state<drm_private_state>`.
>>>>>>> upstream/android-13

- An atomic update is assembled and validated as an entirely free-standing pile
  of structures within the :c:type:`drm_atomic_state <drm_atomic_state>`
  container. Driver private state structures are also tracked in the same
  structure; see the next chapter.  Only when a state is committed is it applied
  to the driver and modeset objects. This way rolling back an update boils down
  to releasing memory and unreferencing objects like framebuffers.

<<<<<<< HEAD
=======
Locking of atomic state structures is internally using :c:type:`struct
drm_modeset_lock <drm_modeset_lock>`. As a general rule the locking shouldn't be
exposed to drivers, instead the right locks should be automatically acquired by
any function that duplicates or peeks into a state, like e.g.
drm_atomic_get_crtc_state().  Locking only protects the software data
structure, ordering of committing state changes to hardware is sequenced using
:c:type:`struct drm_crtc_commit <drm_crtc_commit>`.

>>>>>>> upstream/android-13
Read on in this chapter, and also in :ref:`drm_atomic_helper` for more detailed
coverage of specific topics.

Handling Driver Private State
-----------------------------

.. kernel-doc:: drivers/gpu/drm/drm_atomic.c
   :doc: handling driver private state

Atomic Mode Setting Function Reference
--------------------------------------

.. kernel-doc:: include/drm/drm_atomic.h
   :internal:

.. kernel-doc:: drivers/gpu/drm/drm_atomic.c
   :export:

<<<<<<< HEAD
.. kernel-doc:: drivers/gpu/drm/drm_atomic.c
   :internal:
=======
Atomic Mode Setting IOCTL and UAPI Functions
--------------------------------------------

.. kernel-doc:: drivers/gpu/drm/drm_atomic_uapi.c
   :doc: overview

.. kernel-doc:: drivers/gpu/drm/drm_atomic_uapi.c
   :export:
>>>>>>> upstream/android-13

CRTC Abstraction
================

.. kernel-doc:: drivers/gpu/drm/drm_crtc.c
   :doc: overview

CRTC Functions Reference
--------------------------------

.. kernel-doc:: include/drm/drm_crtc.h
   :internal:

.. kernel-doc:: drivers/gpu/drm/drm_crtc.c
   :export:

<<<<<<< HEAD
=======
Color Management Functions Reference
------------------------------------

.. kernel-doc:: drivers/gpu/drm/drm_color_mgmt.c
   :export:

.. kernel-doc:: include/drm/drm_color_mgmt.h
   :internal:

>>>>>>> upstream/android-13
Frame Buffer Abstraction
========================

.. kernel-doc:: drivers/gpu/drm/drm_framebuffer.c
   :doc: overview

Frame Buffer Functions Reference
--------------------------------

.. kernel-doc:: include/drm/drm_framebuffer.h
   :internal:

.. kernel-doc:: drivers/gpu/drm/drm_framebuffer.c
   :export:

DRM Format Handling
===================

<<<<<<< HEAD
=======
.. kernel-doc:: include/uapi/drm/drm_fourcc.h
   :doc: overview

Format Functions Reference
--------------------------

>>>>>>> upstream/android-13
.. kernel-doc:: include/drm/drm_fourcc.h
   :internal:

.. kernel-doc:: drivers/gpu/drm/drm_fourcc.c
   :export:

Dumb Buffer Objects
===================

.. kernel-doc:: drivers/gpu/drm/drm_dumb_buffers.c
   :doc: overview

Plane Abstraction
=================

.. kernel-doc:: drivers/gpu/drm/drm_plane.c
   :doc: overview

Plane Functions Reference
-------------------------

.. kernel-doc:: include/drm/drm_plane.h
   :internal:

.. kernel-doc:: drivers/gpu/drm/drm_plane.c
   :export:

<<<<<<< HEAD
=======
Plane Composition Functions Reference
-------------------------------------

.. kernel-doc:: drivers/gpu/drm/drm_blend.c
   :export:

Plane Damage Tracking Functions Reference
-----------------------------------------

.. kernel-doc:: drivers/gpu/drm/drm_damage_helper.c
   :export:

.. kernel-doc:: include/drm/drm_damage_helper.h
   :internal:

>>>>>>> upstream/android-13
Display Modes Function Reference
================================

.. kernel-doc:: include/drm/drm_modes.h
   :internal:

.. kernel-doc:: drivers/gpu/drm/drm_modes.c
   :export:

Connector Abstraction
=====================

.. kernel-doc:: drivers/gpu/drm/drm_connector.c
   :doc: overview

Connector Functions Reference
-----------------------------

.. kernel-doc:: include/drm/drm_connector.h
   :internal:

.. kernel-doc:: drivers/gpu/drm/drm_connector.c
   :export:

Writeback Connectors
--------------------

<<<<<<< HEAD
=======
.. kernel-doc:: include/drm/drm_writeback.h
  :internal:

>>>>>>> upstream/android-13
.. kernel-doc:: drivers/gpu/drm/drm_writeback.c
  :doc: overview

.. kernel-doc:: drivers/gpu/drm/drm_writeback.c
  :export:

Encoder Abstraction
===================

.. kernel-doc:: drivers/gpu/drm/drm_encoder.c
   :doc: overview

Encoder Functions Reference
---------------------------

.. kernel-doc:: include/drm/drm_encoder.h
   :internal:

.. kernel-doc:: drivers/gpu/drm/drm_encoder.c
   :export:

<<<<<<< HEAD
KMS Initialization and Cleanup
==============================

A KMS device is abstracted and exposed as a set of planes, CRTCs,
encoders and connectors. KMS drivers must thus create and initialize all
those objects at load time after initializing mode setting.

CRTCs (:c:type:`struct drm_crtc <drm_crtc>`)
--------------------------------------------

A CRTC is an abstraction representing a part of the chip that contains a
pointer to a scanout buffer. Therefore, the number of CRTCs available
determines how many independent scanout buffers can be active at any
given time. The CRTC structure contains several fields to support this:
a pointer to some video memory (abstracted as a frame buffer object), a
display mode, and an (x, y) offset into the video memory to support
panning or configurations where one piece of video memory spans multiple
CRTCs.

CRTC Initialization
~~~~~~~~~~~~~~~~~~~

A KMS device must create and register at least one struct
:c:type:`struct drm_crtc <drm_crtc>` instance. The instance is
allocated and zeroed by the driver, possibly as part of a larger
structure, and registered with a call to :c:func:`drm_crtc_init()`
with a pointer to CRTC functions.


Cleanup
-------

The DRM core manages its objects' lifetime. When an object is not needed
anymore the core calls its destroy function, which must clean up and
free every resource allocated for the object. Every
:c:func:`drm_\*_init()` call must be matched with a corresponding
:c:func:`drm_\*_cleanup()` call to cleanup CRTCs
(:c:func:`drm_crtc_cleanup()`), planes
(:c:func:`drm_plane_cleanup()`), encoders
(:c:func:`drm_encoder_cleanup()`) and connectors
(:c:func:`drm_connector_cleanup()`). Furthermore, connectors that
have been added to sysfs must be removed by a call to
:c:func:`drm_connector_unregister()` before calling
:c:func:`drm_connector_cleanup()`.

Connectors state change detection must be cleanup up with a call to
:c:func:`drm_kms_helper_poll_fini()`.

Output discovery and initialization example
-------------------------------------------

.. code-block:: c

    void intel_crt_init(struct drm_device *dev)
    {
        struct drm_connector *connector;
        struct intel_output *intel_output;

        intel_output = kzalloc(sizeof(struct intel_output), GFP_KERNEL);
        if (!intel_output)
            return;

        connector = &intel_output->base;
        drm_connector_init(dev, &intel_output->base,
                   &intel_crt_connector_funcs, DRM_MODE_CONNECTOR_VGA);

        drm_encoder_init(dev, &intel_output->enc, &intel_crt_enc_funcs,
                 DRM_MODE_ENCODER_DAC);

        drm_connector_attach_encoder(&intel_output->base,
                          &intel_output->enc);

        /* Set up the DDC bus. */
        intel_output->ddc_bus = intel_i2c_create(dev, GPIOA, "CRTDDC_A");
        if (!intel_output->ddc_bus) {
            dev_printk(KERN_ERR, &dev->pdev->dev, "DDC bus registration "
                   "failed.\n");
            return;
        }

        intel_output->type = INTEL_OUTPUT_ANALOG;
        connector->interlace_allowed = 0;
        connector->doublescan_allowed = 0;

        drm_encoder_helper_add(&intel_output->enc, &intel_crt_helper_funcs);
        drm_connector_helper_add(connector, &intel_crt_connector_helper_funcs);

        drm_connector_register(connector);
    }

In the example above (taken from the i915 driver), a CRTC, connector and
encoder combination is created. A device-specific i2c bus is also
created for fetching EDID data and performing monitor detection. Once
the process is complete, the new connector is registered with sysfs to
make its properties available to applications.

=======
>>>>>>> upstream/android-13
KMS Locking
===========

.. kernel-doc:: drivers/gpu/drm/drm_modeset_lock.c
   :doc: kms locking

.. kernel-doc:: include/drm/drm_modeset_lock.h
   :internal:

.. kernel-doc:: drivers/gpu/drm/drm_modeset_lock.c
   :export:

KMS Properties
==============

<<<<<<< HEAD
=======
This section of the documentation is primarily aimed at user-space developers.
For the driver APIs, see the other sections.

Requirements
------------

KMS drivers might need to add extra properties to support new features. Each
new property introduced in a driver needs to meet a few requirements, in
addition to the one mentioned above:

* It must be standardized, documenting:

  * The full, exact, name string;
  * If the property is an enum, all the valid value name strings;
  * What values are accepted, and what these values mean;
  * What the property does and how it can be used;
  * How the property might interact with other, existing properties.

* It must provide a generic helper in the core code to register that
  property on the object it attaches to.

* Its content must be decoded by the core and provided in the object's
  associated state structure. That includes anything drivers might want
  to precompute, like struct drm_clip_rect for planes.

* Its initial state must match the behavior prior to the property
  introduction. This might be a fixed value matching what the hardware
  does, or it may be inherited from the state the firmware left the
  system in during boot.

* An IGT test must be submitted where reasonable.

>>>>>>> upstream/android-13
Property Types and Blob Property Support
----------------------------------------

.. kernel-doc:: drivers/gpu/drm/drm_property.c
   :doc: overview

.. kernel-doc:: include/drm/drm_property.h
   :internal:

.. kernel-doc:: drivers/gpu/drm/drm_property.c
   :export:

Standard Connector Properties
-----------------------------

.. kernel-doc:: drivers/gpu/drm/drm_connector.c
   :doc: standard connector properties

HDMI Specific Connector Properties
----------------------------------

.. kernel-doc:: drivers/gpu/drm/drm_connector.c
   :doc: HDMI connector properties

<<<<<<< HEAD
=======
Standard CRTC Properties
------------------------

.. kernel-doc:: drivers/gpu/drm/drm_crtc.c
   :doc: standard CRTC properties

Standard Plane Properties
-------------------------

.. kernel-doc:: drivers/gpu/drm/drm_plane.c
   :doc: standard plane properties

>>>>>>> upstream/android-13
Plane Composition Properties
----------------------------

.. kernel-doc:: drivers/gpu/drm/drm_blend.c
   :doc: overview

<<<<<<< HEAD
.. kernel-doc:: drivers/gpu/drm/drm_blend.c
   :export:
=======
Damage Tracking Properties
--------------------------

.. kernel-doc:: drivers/gpu/drm/drm_plane.c
   :doc: damage tracking
>>>>>>> upstream/android-13

Color Management Properties
---------------------------

.. kernel-doc:: drivers/gpu/drm/drm_color_mgmt.c
   :doc: overview

<<<<<<< HEAD
.. kernel-doc:: drivers/gpu/drm/drm_color_mgmt.c
   :export:

=======
>>>>>>> upstream/android-13
Tile Group Property
-------------------

.. kernel-doc:: drivers/gpu/drm/drm_connector.c
   :doc: Tile group

Explicit Fencing Properties
---------------------------

<<<<<<< HEAD
.. kernel-doc:: drivers/gpu/drm/drm_atomic.c
   :doc: explicit fencing properties

=======
.. kernel-doc:: drivers/gpu/drm/drm_atomic_uapi.c
   :doc: explicit fencing properties


Variable Refresh Properties
---------------------------

.. kernel-doc:: drivers/gpu/drm/drm_connector.c
   :doc: Variable refresh properties

>>>>>>> upstream/android-13
Existing KMS Properties
-----------------------

The following table gives description of drm properties exposed by various
modules/drivers. Because this table is very unwieldy, do not add any new
properties here. Instead document them in a section above.

.. csv-table::
   :header-rows: 1
   :file: kms-properties.csv

Vertical Blanking
=================

.. kernel-doc:: drivers/gpu/drm/drm_vblank.c
   :doc: vblank handling

Vertical Blanking and Interrupt Handling Functions Reference
------------------------------------------------------------

.. kernel-doc:: include/drm/drm_vblank.h
   :internal:

.. kernel-doc:: drivers/gpu/drm/drm_vblank.c
   :export:
<<<<<<< HEAD
=======

Vertical Blank Work
===================

.. kernel-doc:: drivers/gpu/drm/drm_vblank_work.c
   :doc: vblank works

Vertical Blank Work Functions Reference
---------------------------------------

.. kernel-doc:: include/drm/drm_vblank_work.h
   :internal:

.. kernel-doc:: drivers/gpu/drm/drm_vblank_work.c
   :export:
>>>>>>> upstream/android-13
