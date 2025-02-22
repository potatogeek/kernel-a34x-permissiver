================
bpftool-perf
================
-------------------------------------------------------------------------------
tool for inspection of perf related bpf prog attachments
-------------------------------------------------------------------------------

:Manual section: 8

SYNOPSIS
========

	**bpftool** [*OPTIONS*] **perf** *COMMAND*

<<<<<<< HEAD
	*OPTIONS* := { [{ **-j** | **--json** }] [{ **-p** | **--pretty** }] }
=======
	*OPTIONS* := { { **-j** | **--json** } [{ **-p** | **--pretty** }] | { **-d** | **--debug** } }
>>>>>>> upstream/android-13

	*COMMANDS* :=
	{ **show** | **list** | **help** }

PERF COMMANDS
=============

<<<<<<< HEAD
|	**bpftool** **perf { show | list }**
=======
|	**bpftool** **perf** { **show** | **list** }
>>>>>>> upstream/android-13
|	**bpftool** **perf help**

DESCRIPTION
===========
	**bpftool perf { show | list }**
		  List all raw_tracepoint, tracepoint, kprobe attachment in the system.

		  Output will start with process id and file descriptor in that process,
		  followed by bpf program id, attachment information, and attachment point.
		  The attachment point for raw_tracepoint/tracepoint is the trace probe name.
		  The attachment point for k[ret]probe is either symbol name and offset,
		  or a kernel virtual address.
		  The attachment point for u[ret]probe is the file name and the file offset.

	**bpftool perf help**
		  Print short help message.

OPTIONS
=======
<<<<<<< HEAD
	-h, --help
		  Print short generic help message (similar to **bpftool help**).

	-v, --version
		  Print version number (similar to **bpftool version**).

	-j, --json
		  Generate JSON output. For commands that cannot produce JSON, this
		  option has no effect.

	-p, --pretty
		  Generate human-readable JSON output. Implies **-j**.
=======
	.. include:: common_options.rst
>>>>>>> upstream/android-13

EXAMPLES
========

| **# bpftool perf**

::

      pid 21711  fd 5: prog_id 5  kprobe  func __x64_sys_write  offset 0
      pid 21765  fd 5: prog_id 7  kretprobe  func __x64_sys_nanosleep  offset 0
      pid 21767  fd 5: prog_id 8  tracepoint  sys_enter_nanosleep
      pid 21800  fd 5: prog_id 9  uprobe  filename /home/yhs/a.out  offset 1159

|
| **# bpftool -j perf**

::

    [{"pid":21711,"fd":5,"prog_id":5,"fd_type":"kprobe","func":"__x64_sys_write","offset":0}, \
     {"pid":21765,"fd":5,"prog_id":7,"fd_type":"kretprobe","func":"__x64_sys_nanosleep","offset":0}, \
     {"pid":21767,"fd":5,"prog_id":8,"fd_type":"tracepoint","tracepoint":"sys_enter_nanosleep"}, \
     {"pid":21800,"fd":5,"prog_id":9,"fd_type":"uprobe","filename":"/home/yhs/a.out","offset":1159}]
<<<<<<< HEAD


SEE ALSO
========
	**bpftool**\ (8), **bpftool-prog**\ (8), **bpftool-map**\ (8)
=======
>>>>>>> upstream/android-13
