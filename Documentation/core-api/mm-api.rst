======================
Memory Management APIs
======================

User Space Memory Access
========================

.. kernel-doc:: arch/x86/include/asm/uaccess.h
   :internal:

.. kernel-doc:: arch/x86/lib/usercopy_32.c
   :export:

<<<<<<< HEAD
.. kernel-doc:: mm/util.c
   :functions: get_user_pages_fast

Memory Allocation Controls
==========================

Functions which need to allocate memory often use GFP flags to express
how that memory should be allocated. The GFP acronym stands for "get
free pages", the underlying memory allocation function. Not every GFP
flag is allowed to every function which may allocate memory. Most
users will want to use a plain ``GFP_KERNEL``.
=======
.. kernel-doc:: mm/gup.c
   :functions: get_user_pages_fast

.. _mm-api-gfp-flags:

Memory Allocation Controls
==========================

.. kernel-doc:: include/linux/gfp.h
   :internal:
>>>>>>> upstream/android-13

.. kernel-doc:: include/linux/gfp.h
   :doc: Page mobility and placement hints

.. kernel-doc:: include/linux/gfp.h
   :doc: Watermark modifiers

.. kernel-doc:: include/linux/gfp.h
   :doc: Reclaim modifiers

.. kernel-doc:: include/linux/gfp.h
<<<<<<< HEAD
   :doc: Common combinations
=======
   :doc: Useful GFP flag combinations
>>>>>>> upstream/android-13

The Slab Cache
==============

.. kernel-doc:: include/linux/slab.h
   :internal:

.. kernel-doc:: mm/slab.c
   :export:

<<<<<<< HEAD
.. kernel-doc:: mm/util.c
   :functions: kfree_const kvmalloc_node kvfree

More Memory Management Functions
================================
=======
.. kernel-doc:: mm/slab_common.c
   :export:

.. kernel-doc:: mm/util.c
   :functions: kfree_const kvmalloc_node kvfree

Virtually Contiguous Mappings
=============================

.. kernel-doc:: mm/vmalloc.c
   :export:

File Mapping and Page Cache
===========================
>>>>>>> upstream/android-13

.. kernel-doc:: mm/readahead.c
   :export:

.. kernel-doc:: mm/filemap.c
   :export:

<<<<<<< HEAD
.. kernel-doc:: mm/memory.c
   :export:

.. kernel-doc:: mm/vmalloc.c
   :export:

.. kernel-doc:: mm/page_alloc.c
   :internal:

.. kernel-doc:: mm/mempool.c
   :export:

.. kernel-doc:: mm/dmapool.c
   :export:

=======
>>>>>>> upstream/android-13
.. kernel-doc:: mm/page-writeback.c
   :export:

.. kernel-doc:: mm/truncate.c
   :export:
<<<<<<< HEAD
=======

.. kernel-doc:: include/linux/pagemap.h
   :internal:

Memory pools
============

.. kernel-doc:: mm/mempool.c
   :export:

DMA pools
=========

.. kernel-doc:: mm/dmapool.c
   :export:

More Memory Management Functions
================================

.. kernel-doc:: mm/memory.c
   :export:

.. kernel-doc:: mm/page_alloc.c
.. kernel-doc:: mm/mempolicy.c
.. kernel-doc:: include/linux/mm_types.h
   :internal:
.. kernel-doc:: include/linux/mm.h
   :internal:
.. kernel-doc:: include/linux/mmzone.h
>>>>>>> upstream/android-13
