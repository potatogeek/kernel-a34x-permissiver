================
BPFTOOL
================
-------------------------------------------------------------------------------
tool for inspection and simple manipulation of eBPF programs and maps
-------------------------------------------------------------------------------

:Manual section: 8

SYNOPSIS
========

	**bpftool** [*OPTIONS*] *OBJECT* { *COMMAND* | **help** }

	**bpftool** **batch file** *FILE*

	**bpftool** **version**

<<<<<<< HEAD
	*OBJECT* := { **map** | **program** | **cgroup** | **perf** }

	*OPTIONS* := { { **-V** | **--version** } | { **-h** | **--help** }
	| { **-j** | **--json** } [{ **-p** | **--pretty** }] }

	*MAP-COMMANDS* :=
	{ **show** | **list** | **dump** | **update** | **lookup** | **getnext** | **delete**
	| **pin** | **event_pipe** | **help** }

	*PROG-COMMANDS* := { **show** | **list** | **dump jited** | **dump xlated** | **pin**
	| **load** | **help** }
=======
	*OBJECT* := { **map** | **program** | **cgroup** | **perf** | **net** | **feature** }

	*OPTIONS* := { { **-V** | **--version** } |
	{ **-j** | **--json** } [{ **-p** | **--pretty** }] | { **-d** | **--debug** } }

	*MAP-COMMANDS* :=
	{ **show** | **list** | **create** | **dump** | **update** | **lookup** | **getnext** |
	**delete** | **pin** | **event_pipe** | **help** }

	*PROG-COMMANDS* := { **show** | **list** | **dump jited** | **dump xlated** | **pin** |
	**load** | **attach** | **detach** | **help** }
>>>>>>> upstream/android-13

	*CGROUP-COMMANDS* := { **show** | **list** | **attach** | **detach** | **help** }

	*PERF-COMMANDS* := { **show** | **list** | **help** }

<<<<<<< HEAD
=======
	*NET-COMMANDS* := { **show** | **list** | **help** }

	*FEATURE-COMMANDS* := { **probe** | **help** }

>>>>>>> upstream/android-13
DESCRIPTION
===========
	*bpftool* allows for inspection and simple modification of BPF objects
	on the system.

	Note that format of the output of all tools is not guaranteed to be
	stable and should not be depended upon.

OPTIONS
=======
<<<<<<< HEAD
	-h, --help
		  Print short help message (similar to **bpftool help**).

	-v, --version
		  Print version number (similar to **bpftool version**).

	-j, --json
		  Generate JSON output. For commands that cannot produce JSON, this
		  option has no effect.

	-p, --pretty
		  Generate human-readable JSON output. Implies **-j**.

SEE ALSO
========
	**bpftool-map**\ (8), **bpftool-prog**\ (8), **bpftool-cgroup**\ (8)
        **bpftool-perf**\ (8)
=======
	.. include:: common_options.rst

	-m, --mapcompat
		  Allow loading maps with unknown map definitions.

	-n, --nomount
		  Do not automatically attempt to mount any virtual file system
		  (such as tracefs or BPF virtual file system) when necessary.
>>>>>>> upstream/android-13
