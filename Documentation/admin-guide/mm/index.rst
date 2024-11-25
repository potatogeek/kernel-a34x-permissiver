=================
Memory Management
=================

Linux memory management subsystem is responsible, as the name implies,
<<<<<<< HEAD
for managing the memory in the system. This includes implemnetation of
virtual memory and demand paging, memory allocation both for kernel
internal structures and user space programms, mapping of files into
=======
for managing the memory in the system. This includes implementation of
virtual memory and demand paging, memory allocation both for kernel
internal structures and user space programs, mapping of files into
>>>>>>> upstream/android-13
processes address space and many other cool things.

Linux memory management is a complex system with many configurable
settings. Most of these settings are available via ``/proc``
filesystem and can be quired and adjusted using ``sysctl``. These APIs
<<<<<<< HEAD
are described in Documentation/sysctl/vm.txt and in `man 5 proc`_.
=======
are described in Documentation/admin-guide/sysctl/vm.rst and in `man 5 proc`_.
>>>>>>> upstream/android-13

.. _man 5 proc: http://man7.org/linux/man-pages/man5/proc.5.html

Linux memory management has its own jargon and if you are not yet
familiar with it, consider reading
:ref:`Documentation/admin-guide/mm/concepts.rst <mm_concepts>`.

Here we document in detail how to interact with various mechanisms in
the Linux memory management.

.. toctree::
   :maxdepth: 1

   concepts
<<<<<<< HEAD
   hugetlbpage
   idle_page_tracking
   ksm
   numa_memory_policy
=======
   cma_debugfs
   damon/index
   hugetlbpage
   idle_page_tracking
   ksm
   memory-hotplug
   multigen_lru
   nommu-mmap
   numa_memory_policy
   numaperf
>>>>>>> upstream/android-13
   pagemap
   soft-dirty
   transhuge
   userfaultfd
