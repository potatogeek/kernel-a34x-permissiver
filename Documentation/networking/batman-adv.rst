.. SPDX-License-Identifier: GPL-2.0

==========
batman-adv
==========

Batman advanced is a new approach to wireless networking which does no longer
operate on the IP basis. Unlike the batman daemon, which exchanges information
using UDP packets and sets routing tables, batman-advanced operates on ISO/OSI
Layer 2 only and uses and routes (or better: bridges) Ethernet Frames. It
emulates a virtual network switch of all nodes participating. Therefore all
nodes appear to be link local, thus all higher operating protocols won't be
affected by any changes within the network. You can run almost any protocol
above batman advanced, prominent examples are: IPv4, IPv6, DHCP, IPX.

Batman advanced was implemented as a Linux kernel driver to reduce the overhead
to a minimum. It does not depend on any (other) network driver, and can be used
on wifi as well as ethernet lan, vpn, etc ... (anything with ethernet-style
layer 2).


Configuration
=============

Load the batman-adv module into your kernel::

  $ insmod batman-adv.ko

The module is now waiting for activation. You must add some interfaces on which
<<<<<<< HEAD
batman can operate. After loading the module batman advanced will scan your
systems interfaces to search for compatible interfaces. Once found, it will
create subfolders in the ``/sys`` directories of each supported interface,
e.g.::

  $ ls /sys/class/net/eth0/batman_adv/
  elp_interval iface_status mesh_iface throughput_override

If an interface does not have the ``batman_adv`` subfolder, it probably is not
supported. Not supported interfaces are: loopback, non-ethernet and batman's
own interfaces.

Note: After the module was loaded it will continuously watch for new
interfaces to verify the compatibility. There is no need to reload the module
if you plug your USB wifi adapter into your machine after batman advanced was
initially loaded.

The batman-adv soft-interface can be created using the iproute2 tool ``ip``::
=======
batman-adv can operate. The batman-adv soft-interface can be created using the
iproute2 tool ``ip``::
>>>>>>> upstream/android-13

  $ ip link add name bat0 type batadv

To activate a given interface simply attach it to the ``bat0`` interface::

  $ ip link set dev eth0 master bat0

<<<<<<< HEAD
Repeat this step for all interfaces you wish to add. Now batman starts
using/broadcasting on this/these interface(s).

By reading the "iface_status" file you can check its status::

  $ cat /sys/class/net/eth0/batman_adv/iface_status
  active

=======
Repeat this step for all interfaces you wish to add. Now batman-adv starts
using/broadcasting on this/these interface(s).

>>>>>>> upstream/android-13
To deactivate an interface you have to detach it from the "bat0" interface::

  $ ip link set dev eth0 nomaster

<<<<<<< HEAD

All mesh wide settings can be found in batman's own interface folder::

  $ ls /sys/class/net/bat0/mesh/
  aggregated_ogms       fragmentation isolation_mark routing_algo
  ap_isolation          gw_bandwidth  log_level      vlan0
  bonding               gw_mode       multicast_mode
  bridge_loop_avoidance gw_sel_class  network_coding
  distributed_arp_table hop_penalty   orig_interval

There is a special folder for debugging information::

  $ ls /sys/kernel/debug/batman_adv/bat0/
  bla_backbone_table log         neighbors         transtable_local
  bla_claim_table    mcast_flags originators
  dat_cache          nc          socket
  gateways           nc_nodes    transtable_global

Some of the files contain all sort of status information regarding the mesh
network. For example, you can view the table of originators (mesh
participants) with::

  $ cat /sys/kernel/debug/batman_adv/bat0/originators

Other files allow to change batman's behaviour to better fit your requirements.
For instance, you can check the current originator interval (value in
milliseconds which determines how often batman sends its broadcast packets)::

  $ cat /sys/class/net/bat0/mesh/orig_interval
=======
The same can also be done using the batctl interface subcommand::

  batctl -m bat0 interface create
  batctl -m bat0 interface add -M eth0

To detach eth0 and destroy bat0::

  batctl -m bat0 interface del -M eth0
  batctl -m bat0 interface destroy

There are additional settings for each batadv mesh interface, vlan and hardif
which can be modified using batctl. Detailed information about this can be found
in its manual.

For instance, you can check the current originator interval (value
in milliseconds which determines how often batman-adv sends its broadcast
packets)::

  $ batctl -M bat0 orig_interval
>>>>>>> upstream/android-13
  1000

and also change its value::

<<<<<<< HEAD
  $ echo 3000 > /sys/class/net/bat0/mesh/orig_interval
=======
  $ batctl -M bat0 orig_interval 3000
>>>>>>> upstream/android-13

In very mobile scenarios, you might want to adjust the originator interval to a
lower value. This will make the mesh more responsive to topology changes, but
will also increase the overhead.

<<<<<<< HEAD
=======
Information about the current state can be accessed via the batadv generic
netlink family. batctl provides a human readable version via its debug tables
subcommands.

>>>>>>> upstream/android-13

Usage
=====

To make use of your newly created mesh, batman advanced provides a new
interface "bat0" which you should use from this point on. All interfaces added
to batman advanced are not relevant any longer because batman handles them for
you. Basically, one "hands over" the data by using the batman interface and
batman will make sure it reaches its destination.

The "bat0" interface can be used like any other regular interface. It needs an
IP address which can be either statically configured or dynamically (by using
DHCP or similar services)::

  NodeA: ip link set up dev bat0
  NodeA: ip addr add 192.168.0.1/24 dev bat0

  NodeB: ip link set up dev bat0
  NodeB: ip addr add 192.168.0.2/24 dev bat0
  NodeB: ping 192.168.0.1

Note: In order to avoid problems remove all IP addresses previously assigned to
interfaces now used by batman advanced, e.g.::

  $ ip addr flush dev eth0


Logging/Debugging
=================

All error messages, warnings and information messages are sent to the kernel
log. Depending on your operating system distribution this can be read in one of
a number of ways. Try using the commands: ``dmesg``, ``logread``, or looking in
the files ``/var/log/kern.log`` or ``/var/log/syslog``. All batman-adv messages
are prefixed with "batman-adv:" So to see just these messages try::

  $ dmesg | grep batman-adv

When investigating problems with your mesh network, it is sometimes necessary to
<<<<<<< HEAD
see more detail debug messages. This must be enabled when compiling the
batman-adv module. When building batman-adv as part of kernel, use "make
menuconfig" and enable the option ``B.A.T.M.A.N. debugging``
(``CONFIG_BATMAN_ADV_DEBUG=y``).

Those additional debug messages can be accessed using a special file in
debugfs::

  $ cat /sys/kernel/debug/batman_adv/bat0/log

The additional debug output is by default disabled. It can be enabled during
run time. Following log_levels are defined:

.. flat-table::

   * - 0
     - All debug output disabled
   * - 1
     - Enable messages related to routing / flooding / broadcasting
   * - 2
     - Enable messages related to route added / changed / deleted
   * - 4
     - Enable messages related to translation table operations
   * - 8
     - Enable messages related to bridge loop avoidance
   * - 16
     - Enable messages related to DAT, ARP snooping and parsing
   * - 32
     - Enable messages related to network coding
   * - 64
     - Enable messages related to multicast
   * - 128
     - Enable messages related to throughput meter
   * - 255
     - Enable all messages

The debug output can be changed at runtime using the file
``/sys/class/net/bat0/mesh/log_level``. e.g.::

  $ echo 6 > /sys/class/net/bat0/mesh/log_level

will enable debug messages for when routes change.
=======
see more detailed debug messages. This must be enabled when compiling the
batman-adv module. When building batman-adv as part of the kernel, use "make
menuconfig" and enable the option ``B.A.T.M.A.N. debugging``
(``CONFIG_BATMAN_ADV_DEBUG=y``).

Those additional debug messages can be accessed using the perf infrastructure::

  $ trace-cmd stream -e batadv:batadv_dbg

The additional debug output is by default disabled. It can be enabled during
run time::

  $ batctl -m bat0 loglevel routes tt

will enable debug messages for when routes and translation table entries change.
>>>>>>> upstream/android-13

Counters for different types of packets entering and leaving the batman-adv
module are available through ethtool::

  $ ethtool --statistics bat0


batctl
======

As batman advanced operates on layer 2, all hosts participating in the virtual
switch are completely transparent for all protocols above layer 2. Therefore
the common diagnosis tools do not work as expected. To overcome these problems,
batctl was created. At the moment the batctl contains ping, traceroute, tcpdump
and interfaces to the kernel module settings.

For more information, please see the manpage (``man batctl``).

batctl is available on https://www.open-mesh.org/


Contact
=======

Please send us comments, experiences, questions, anything :)

IRC:
<<<<<<< HEAD
  #batman on irc.freenode.org
Mailing-list:
  b.a.t.m.a.n@open-mesh.org (optional subscription at
  https://lists.open-mesh.org/mm/listinfo/b.a.t.m.a.n)
=======
  #batadv on ircs://irc.hackint.org/
Mailing-list:
  b.a.t.m.a.n@open-mesh.org (optional subscription at
  https://lists.open-mesh.org/mailman3/postorius/lists/b.a.t.m.a.n.lists.open-mesh.org/)
>>>>>>> upstream/android-13

You can also contact the Authors:

* Marek Lindner <mareklindner@neomailbox.ch>
* Simon Wunderlich <sw@simonwunderlich.de>
