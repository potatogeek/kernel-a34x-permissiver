=============================
mac80211 subsystem (advanced)
=============================

Information contained within this part of the book is of interest only
for advanced interaction of mac80211 with drivers to exploit more
hardware capabilities and improve performance.

LED support
===========

Mac80211 supports various ways of blinking LEDs. Wherever possible,
device LEDs should be exposed as LED class devices and hooked up to the
appropriate trigger, which will then be triggered appropriately by
mac80211.

.. kernel-doc:: include/net/mac80211.h
<<<<<<< HEAD
   :functions: ieee80211_get_tx_led_name

.. kernel-doc:: include/net/mac80211.h
   :functions: ieee80211_get_rx_led_name

.. kernel-doc:: include/net/mac80211.h
   :functions: ieee80211_get_assoc_led_name

.. kernel-doc:: include/net/mac80211.h
   :functions: ieee80211_get_radio_led_name

.. kernel-doc:: include/net/mac80211.h
   :functions: ieee80211_tpt_blink

.. kernel-doc:: include/net/mac80211.h
   :functions: ieee80211_tpt_led_trigger_flags

.. kernel-doc:: include/net/mac80211.h
   :functions: ieee80211_create_tpt_led_trigger
=======
   :functions:
	ieee80211_get_tx_led_name
	ieee80211_get_rx_led_name
	ieee80211_get_assoc_led_name
	ieee80211_get_radio_led_name
	ieee80211_tpt_blink
	ieee80211_tpt_led_trigger_flags
	ieee80211_create_tpt_led_trigger
>>>>>>> upstream/android-13

Hardware crypto acceleration
============================

.. kernel-doc:: include/net/mac80211.h
   :doc: Hardware crypto acceleration

.. kernel-doc:: include/net/mac80211.h
<<<<<<< HEAD
   :functions: set_key_cmd

.. kernel-doc:: include/net/mac80211.h
   :functions: ieee80211_key_conf

.. kernel-doc:: include/net/mac80211.h
   :functions: ieee80211_key_flags

.. kernel-doc:: include/net/mac80211.h
   :functions: ieee80211_get_tkip_p1k

.. kernel-doc:: include/net/mac80211.h
   :functions: ieee80211_get_tkip_p1k_iv

.. kernel-doc:: include/net/mac80211.h
   :functions: ieee80211_get_tkip_p2k
=======
   :functions:
	set_key_cmd
	ieee80211_key_conf
	ieee80211_key_flags
	ieee80211_get_tkip_p1k
	ieee80211_get_tkip_p1k_iv
	ieee80211_get_tkip_p2k
>>>>>>> upstream/android-13

Powersave support
=================

.. kernel-doc:: include/net/mac80211.h
   :doc: Powersave support

Beacon filter support
=====================

.. kernel-doc:: include/net/mac80211.h
   :doc: Beacon filter support

.. kernel-doc:: include/net/mac80211.h
   :functions: ieee80211_beacon_loss

Multiple queues and QoS support
===============================

TBD

.. kernel-doc:: include/net/mac80211.h
   :functions: ieee80211_tx_queue_params

Access point mode support
=========================

TBD

Some parts of the if_conf should be discussed here instead

Insert notes about VLAN interfaces with hw crypto here or in the hw
crypto chapter.

support for powersaving clients
-------------------------------

.. kernel-doc:: include/net/mac80211.h
   :doc: AP support for powersaving clients

.. kernel-doc:: include/net/mac80211.h
<<<<<<< HEAD
   :functions: ieee80211_get_buffered_bc

.. kernel-doc:: include/net/mac80211.h
   :functions: ieee80211_beacon_get

.. kernel-doc:: include/net/mac80211.h
   :functions: ieee80211_sta_eosp

.. kernel-doc:: include/net/mac80211.h
   :functions: ieee80211_frame_release_type

.. kernel-doc:: include/net/mac80211.h
   :functions: ieee80211_sta_ps_transition

.. kernel-doc:: include/net/mac80211.h
   :functions: ieee80211_sta_ps_transition_ni

.. kernel-doc:: include/net/mac80211.h
   :functions: ieee80211_sta_set_buffered

.. kernel-doc:: include/net/mac80211.h
   :functions: ieee80211_sta_block_awake
=======
   :functions:
	ieee80211_get_buffered_bc
	ieee80211_beacon_get
	ieee80211_sta_eosp
	ieee80211_frame_release_type
	ieee80211_sta_ps_transition
	ieee80211_sta_ps_transition_ni
	ieee80211_sta_set_buffered
	ieee80211_sta_block_awake
>>>>>>> upstream/android-13

Supporting multiple virtual interfaces
======================================

TBD

Note: WDS with identical MAC address should almost always be OK

Insert notes about having multiple virtual interfaces with different MAC
addresses here, note which configurations are supported by mac80211, add
notes about supporting hw crypto with it.

.. kernel-doc:: include/net/mac80211.h
<<<<<<< HEAD
   :functions: ieee80211_iterate_active_interfaces

.. kernel-doc:: include/net/mac80211.h
   :functions: ieee80211_iterate_active_interfaces_atomic
=======
   :functions:
	ieee80211_iterate_active_interfaces
	ieee80211_iterate_active_interfaces_atomic
>>>>>>> upstream/android-13

Station handling
================

TODO

.. kernel-doc:: include/net/mac80211.h
<<<<<<< HEAD
   :functions: ieee80211_sta

.. kernel-doc:: include/net/mac80211.h
   :functions: sta_notify_cmd

.. kernel-doc:: include/net/mac80211.h
   :functions: ieee80211_find_sta

.. kernel-doc:: include/net/mac80211.h
   :functions: ieee80211_find_sta_by_ifaddr
=======
   :functions:
	ieee80211_sta
	sta_notify_cmd
	ieee80211_find_sta
	ieee80211_find_sta_by_ifaddr
>>>>>>> upstream/android-13

Hardware scan offload
=====================

TBD

.. kernel-doc:: include/net/mac80211.h
   :functions: ieee80211_scan_completed

Aggregation
===========

TX A-MPDU aggregation
---------------------

.. kernel-doc:: net/mac80211/agg-tx.c
   :doc: TX A-MPDU aggregation

.. WARNING: DOCPROC directive not supported: !Cnet/mac80211/agg-tx.c

RX A-MPDU aggregation
---------------------

.. kernel-doc:: net/mac80211/agg-rx.c
   :doc: RX A-MPDU aggregation

.. WARNING: DOCPROC directive not supported: !Cnet/mac80211/agg-rx.c

.. kernel-doc:: include/net/mac80211.h
   :functions: ieee80211_ampdu_mlme_action

Spatial Multiplexing Powersave (SMPS)
=====================================

.. kernel-doc:: include/net/mac80211.h
   :doc: Spatial multiplexing power save

.. kernel-doc:: include/net/mac80211.h
<<<<<<< HEAD
   :functions: ieee80211_request_smps

.. kernel-doc:: include/net/mac80211.h
   :functions: ieee80211_smps_mode
=======
   :functions:
	ieee80211_request_smps
	ieee80211_smps_mode
>>>>>>> upstream/android-13

TBD

This part of the book describes the rate control algorithm interface and
how it relates to mac80211 and drivers.

Rate Control API
================

TBD

.. kernel-doc:: include/net/mac80211.h
<<<<<<< HEAD
   :functions: ieee80211_start_tx_ba_session

.. kernel-doc:: include/net/mac80211.h
   :functions: ieee80211_start_tx_ba_cb_irqsafe

.. kernel-doc:: include/net/mac80211.h
   :functions: ieee80211_stop_tx_ba_session

.. kernel-doc:: include/net/mac80211.h
   :functions: ieee80211_stop_tx_ba_cb_irqsafe

.. kernel-doc:: include/net/mac80211.h
   :functions: ieee80211_rate_control_changed

.. kernel-doc:: include/net/mac80211.h
   :functions: ieee80211_tx_rate_control

.. kernel-doc:: include/net/mac80211.h
   :functions: rate_control_send_low
=======
   :functions:
	ieee80211_start_tx_ba_session
	ieee80211_start_tx_ba_cb_irqsafe
	ieee80211_stop_tx_ba_session
	ieee80211_stop_tx_ba_cb_irqsafe
	ieee80211_rate_control_changed
	ieee80211_tx_rate_control
>>>>>>> upstream/android-13

TBD

This part of the book describes mac80211 internals.

Key handling
============

Key handling basics
-------------------

.. kernel-doc:: net/mac80211/key.c
   :doc: Key handling basics

MORE TBD
--------

TBD

Receive processing
==================

TBD

Transmit processing
===================

TBD

Station info handling
=====================

Programming information
-----------------------

.. kernel-doc:: net/mac80211/sta_info.h
<<<<<<< HEAD
   :functions: sta_info

.. kernel-doc:: net/mac80211/sta_info.h
   :functions: ieee80211_sta_info_flags
=======
   :functions:
	sta_info
	ieee80211_sta_info_flags
>>>>>>> upstream/android-13

STA information lifetime rules
------------------------------

.. kernel-doc:: net/mac80211/sta_info.c
   :doc: STA information lifetime rules

<<<<<<< HEAD
Aggregation
===========

.. kernel-doc:: net/mac80211/sta_info.h
   :functions: sta_ampdu_mlme

.. kernel-doc:: net/mac80211/sta_info.h
   :functions: tid_ampdu_tx

.. kernel-doc:: net/mac80211/sta_info.h
   :functions: tid_ampdu_rx

Synchronisation
===============
=======
Aggregation Functions
=====================

.. kernel-doc:: net/mac80211/sta_info.h
   :functions:
	sta_ampdu_mlme
	tid_ampdu_tx
	tid_ampdu_rx

Synchronisation Functions
=========================
>>>>>>> upstream/android-13

TBD

Locking, lots of RCU
