.. SPDX-License-Identifier: GPL-2.0

<<<<<<< HEAD
=======
.. _inline_encryption:

>>>>>>> upstream/android-13
=================
Inline Encryption
=================

<<<<<<< HEAD
=======
Background
==========

Inline encryption hardware sits logically between memory and the disk, and can
en/decrypt data as it goes in/out of the disk. Inline encryption hardware has a
fixed number of "keyslots" - slots into which encryption contexts (i.e. the
encryption key, encryption algorithm, data unit size) can be programmed by the
kernel at any time. Each request sent to the disk can be tagged with the index
of a keyslot (and also a data unit number to act as an encryption tweak), and
the inline encryption hardware will en/decrypt the data in the request with the
encryption context programmed into that keyslot. This is very different from
full disk encryption solutions like self encrypting drives/TCG OPAL/ATA
Security standards, since with inline encryption, any block on disk could be
encrypted with any encryption context the kernel chooses.


>>>>>>> upstream/android-13
Objective
=========

We want to support inline encryption (IE) in the kernel.
To allow for testing, we also want a crypto API fallback when actual
IE hardware is absent. We also want IE to work with layered devices
like dm and loopback (i.e. we want to be able to use the IE hardware
of the underlying devices if present, or else fall back to crypto API
en/decryption).


Constraints and notes
=====================

<<<<<<< HEAD
- IE hardware have a limited number of "keyslots" that can be programmed
=======
- IE hardware has a limited number of "keyslots" that can be programmed
>>>>>>> upstream/android-13
  with an encryption context (key, algorithm, data unit size, etc.) at any time.
  One can specify a keyslot in a data request made to the device, and the
  device will en/decrypt the data using the encryption context programmed into
  that specified keyslot. When possible, we want to make multiple requests with
  the same encryption context share the same keyslot.

<<<<<<< HEAD
- We need a way for filesystems to specify an encryption context to use for
  en/decrypting a struct bio, and a device driver (like UFS) needs to be able
  to use that encryption context when it processes the bio.

- We need a way for device drivers to expose their capabilities in a unified
  way to the upper layers.
=======
- We need a way for upper layers like filesystems to specify an encryption
  context to use for en/decrypting a struct bio, and a device driver (like UFS)
  needs to be able to use that encryption context when it processes the bio.

- We need a way for device drivers to expose their inline encryption
  capabilities in a unified way to the upper layers.
>>>>>>> upstream/android-13


Design
======

<<<<<<< HEAD
We add a struct bio_crypt_ctx to struct bio that can represent an
encryption context, because we need to be able to pass this encryption
context from the FS layer to the device driver to act upon.
=======
We add a struct bio_crypt_ctx to struct bio that can
represent an encryption context, because we need to be able to pass this
encryption context from the upper layers (like the fs layer) to the
device driver to act upon.
>>>>>>> upstream/android-13

While IE hardware works on the notion of keyslots, the FS layer has no
knowledge of keyslots - it simply wants to specify an encryption context to
use while en/decrypting a bio.

We introduce a keyslot manager (KSM) that handles the translation from
encryption contexts specified by the FS to keyslots on the IE hardware.
<<<<<<< HEAD
This KSM also serves as the way IE hardware can expose their capabilities to
=======
This KSM also serves as the way IE hardware can expose its capabilities to
>>>>>>> upstream/android-13
upper layers. The generic mode of operation is: each device driver that wants
to support IE will construct a KSM and set it up in its struct request_queue.
Upper layers that want to use IE on this device can then use this KSM in
the device's struct request_queue to translate an encryption context into
a keyslot. The presence of the KSM in the request queue shall be used to mean
that the device supports IE.

<<<<<<< HEAD
On the device driver end of the interface, the device driver needs to tell the
KSM how to actually manipulate the IE hardware in the device to do things like
programming the crypto key into the IE hardware into a particular keyslot. All
this is achieved through the :c:type:`struct keyslot_mgmt_ll_ops` that the
device driver passes to the KSM when creating it.

It uses refcounts to track which keyslots are idle (either they have no
=======
The KSM uses refcounts to track which keyslots are idle (either they have no
>>>>>>> upstream/android-13
encryption context programmed, or there are no in-flight struct bios
referencing that keyslot). When a new encryption context needs a keyslot, it
tries to find a keyslot that has already been programmed with the same
encryption context, and if there is no such keyslot, it evicts the least
recently used idle keyslot and programs the new encryption context into that
one. If no idle keyslots are available, then the caller will sleep until there
is at least one.


<<<<<<< HEAD
Blk-crypto
==========

The above is sufficient for simple cases, but does not work if there is a
need for a crypto API fallback, or if we are want to use IE with layered
devices. To these ends, we introduce blk-crypto. Blk-crypto allows us to
present a unified view of encryption to the FS (so FS only needs to specify
an encryption context and not worry about keyslots at all), and blk-crypto
can decide whether to delegate the en/decryption to IE hardware or to the
crypto API. Blk-crypto maintains an internal KSM that serves as the crypto
API fallback.

Blk-crypto needs to ensure that the encryption context is programmed into the
"correct" keyslot manager for IE. If a bio is submitted to a layered device
that eventually passes the bio down to a device that really does support IE, we
want the encryption context to be programmed into a keyslot for the KSM of the
device with IE support. However, blk-crypto does not know a priori whether a
particular device is the final device in the layering structure for a bio or
not. So in the case that a particular device does not support IE, since it is
possibly the final destination device for the bio, if the bio requires
encryption (i.e. the bio is doing a write operation), blk-crypto must fallback
to the crypto API *before* sending the bio to the device.

Blk-crypto ensures that:

- The bio's encryption context is programmed into a keyslot in the KSM of the
  request queue that the bio is being submitted to (or the crypto API fallback
  KSM if the request queue doesn't have a KSM), and that the ``bc_ksm``
  in the ``bi_crypt_context`` is set to this KSM

- That the bio has its own individual reference to the keyslot in this KSM.
  Once the bio passes through blk-crypto, its encryption context is programmed
  in some KSM. The "its own individual reference to the keyslot" ensures that
  keyslots can be released by each bio independently of other bios while
  ensuring that the bio has a valid reference to the keyslot when, for e.g., the
  crypto API fallback KSM in blk-crypto performs crypto on the device's behalf.
  The individual references are ensured by increasing the refcount for the
  keyslot in the ``bc_ksm`` when a bio with a programmed encryption
  context is cloned.


What blk-crypto does on bio submission
--------------------------------------

**Case 1:** blk-crypto is given a bio with only an encryption context that hasn't
been programmed into any keyslot in any KSM (for e.g. a bio from the FS).
  In this case, blk-crypto will program the encryption context into the KSM of the
  request queue the bio is being submitted to (and if this KSM does not exist,
  then it will program it into blk-crypto's internal KSM for crypto API
  fallback). The KSM that this encryption context was programmed into is stored
  as the ``bc_ksm`` in the bio's ``bi_crypt_context``.

**Case 2:** blk-crypto is given a bio whose encryption context has already been
programmed into a keyslot in the *crypto API fallback* KSM.
  In this case, blk-crypto does nothing; it treats the bio as not having
  specified an encryption context. Note that we cannot do here what we will do
  in Case 3 because we would have already encrypted the bio via the crypto API
  by this point.

**Case 3:** blk-crypto is given a bio whose encryption context has already been
programmed into a keyslot in some KSM (that is *not* the crypto API fallback
KSM).
  In this case, blk-crypto first releases that keyslot from that KSM and then
  treats the bio as in Case 1.

This way, when a device driver is processing a bio, it can be sure that
the bio's encryption context has been programmed into some KSM (either the
device driver's request queue's KSM, or blk-crypto's crypto API fallback KSM).
It then simply needs to check if the bio's ``bc_ksm`` is the device's
request queue's KSM. If so, then it should proceed with IE. If not, it should
simply do nothing with respect to crypto, because some other KSM (perhaps the
blk-crypto crypto API fallback KSM) is handling the en/decryption.

Blk-crypto will release the keyslot that is being held by the bio (and also
decrypt it if the bio is using the crypto API fallback KSM) once
``bio_remaining_done`` returns true for the bio.

=======
blk-mq changes, other block layer changes and blk-crypto-fallback
=================================================================

We add a pointer to a ``bi_crypt_context`` and ``keyslot`` to
struct request. These will be referred to as the ``crypto fields``
for the request. This ``keyslot`` is the keyslot into which the
``bi_crypt_context`` has been programmed in the KSM of the ``request_queue``
that this request is being sent to.

We introduce ``block/blk-crypto-fallback.c``, which allows upper layers to remain
blissfully unaware of whether or not real inline encryption hardware is present
underneath. When a bio is submitted with a target ``request_queue`` that doesn't
support the encryption context specified with the bio, the block layer will
en/decrypt the bio with the blk-crypto-fallback.

If the bio is a ``WRITE`` bio, a bounce bio is allocated, and the data in the bio
is encrypted stored in the bounce bio - blk-mq will then proceed to process the
bounce bio as if it were not encrypted at all (except when blk-integrity is
concerned). ``blk-crypto-fallback`` sets the bounce bio's ``bi_end_io`` to an
internal function that cleans up the bounce bio and ends the original bio.

If the bio is a ``READ`` bio, the bio's ``bi_end_io`` (and also ``bi_private``)
is saved and overwritten by ``blk-crypto-fallback`` to
``bio_crypto_fallback_decrypt_bio``.  The bio's ``bi_crypt_context`` is also
overwritten with ``NULL``, so that to the rest of the stack, the bio looks
as if it was a regular bio that never had an encryption context specified.
``bio_crypto_fallback_decrypt_bio`` will decrypt the bio, restore the original
``bi_end_io`` (and also ``bi_private``) and end the bio again.

Regardless of whether real inline encryption hardware is used or the
blk-crypto-fallback is used, the ciphertext written to disk (and hence the
on-disk format of data) will be the same (assuming the hardware's implementation
of the algorithm being used adheres to spec and functions correctly).

If a ``request queue``'s inline encryption hardware claimed to support the
encryption context specified with a bio, then it will not be handled by the
``blk-crypto-fallback``. We will eventually reach a point in blk-mq when a
struct request needs to be allocated for that bio. At that point,
blk-mq tries to program the encryption context into the ``request_queue``'s
keyslot_manager, and obtain a keyslot, which it stores in its newly added
``keyslot`` field. This keyslot is released when the request is completed.

When the first bio is added to a request, ``blk_crypto_rq_bio_prep`` is called,
which sets the request's ``crypt_ctx`` to a copy of the bio's
``bi_crypt_context``. bio_crypt_do_front_merge is called whenever a subsequent
bio is merged to the front of the request, which updates the ``crypt_ctx`` of
the request so that it matches the newly merged bio's ``bi_crypt_context``. In particular, the request keeps a copy of the ``bi_crypt_context`` of the first
bio in its bio-list (blk-mq needs to be careful to maintain this invariant
during bio and request merges).

To make it possible for inline encryption to work with request queue based
layered devices, when a request is cloned, its ``crypto fields`` are cloned as
well. When the cloned request is submitted, blk-mq programs the
``bi_crypt_context`` of the request into the clone's request_queue's keyslot
manager, and stores the returned keyslot in the clone's ``keyslot``.


API presented to users of the block layer
=========================================

``struct blk_crypto_key`` represents a crypto key (the raw key, size of the
key, the crypto algorithm to use, the data unit size to use, and the number of
bytes required to represent data unit numbers that will be specified with the
``bi_crypt_context``).

``blk_crypto_init_key`` allows upper layers to initialize such a
``blk_crypto_key``.

``bio_crypt_set_ctx`` should be called on any bio that a user of
the block layer wants en/decrypted via inline encryption (or the
blk-crypto-fallback, if hardware support isn't available for the desired
crypto configuration). This function takes the ``blk_crypto_key`` and the
data unit number (DUN) to use when en/decrypting the bio.

``blk_crypto_config_supported`` allows upper layers to query whether or not the
an encryption context passed to request queue can be handled by blk-crypto
(either by real inline encryption hardware, or by the blk-crypto-fallback).
This is useful e.g. when blk-crypto-fallback is disabled, and the upper layer
wants to use an algorithm that may not supported by hardware - this function
lets the upper layer know ahead of time that the algorithm isn't supported,
and the upper layer can fallback to something else if appropriate.

``blk_crypto_start_using_key`` - Upper layers must call this function on
``blk_crypto_key`` and a ``request_queue`` before using the key with any bio
headed for that ``request_queue``. This function ensures that either the
hardware supports the key's crypto settings, or the crypto API fallback has
transforms for the needed mode allocated and ready to go. Note that this
function may allocate an ``skcipher``, and must not be called from the data
path, since allocating ``skciphers`` from the data path can deadlock.

``blk_crypto_evict_key`` *must* be called by upper layers before a
``blk_crypto_key`` is freed. Further, it *must* only be called only once
there are no more in-flight requests that use that ``blk_crypto_key``.
``blk_crypto_evict_key`` will ensure that a key is removed from any keyslots in
inline encryption hardware that the key might have been programmed into (or the blk-crypto-fallback).

API presented to device drivers
===============================

A :c:type:``struct blk_keyslot_manager`` should be set up by device drivers in
the ``request_queue`` of the device. The device driver needs to call
``blk_ksm_init`` (or its resource-managed variant ``devm_blk_ksm_init``) on the
``blk_keyslot_manager``, while specifying the number of keyslots supported by
the hardware.

The device driver also needs to tell the KSM how to actually manipulate the
IE hardware in the device to do things like programming the crypto key into
the IE hardware into a particular keyslot. All this is achieved through the
struct blk_ksm_ll_ops field in the KSM that the device driver
must fill up after initing the ``blk_keyslot_manager``.

The KSM also handles runtime power management for the device when applicable
(e.g. when it wants to program a crypto key into the IE hardware, the device
must be runtime powered on) - so the device driver must also set the ``dev``
field in the ksm to point to the `struct device` for the KSM to use for runtime
power management.

``blk_ksm_reprogram_all_keys`` can be called by device drivers if the device
needs each and every of its keyslots to be reprogrammed with the key it
"should have" at the point in time when the function is called. This is useful
e.g. if a device loses all its keys on runtime power down/up.

If the driver used ``blk_ksm_init`` instead of ``devm_blk_ksm_init``, then
``blk_ksm_destroy`` should be called to free up all resources used by a
``blk_keyslot_manager`` once it is no longer needed.
>>>>>>> upstream/android-13

Layered Devices
===============

<<<<<<< HEAD
Layered devices that wish to support IE need to create their own keyslot
manager for their request queue, and expose whatever functionality they choose.
When a layered device wants to pass a bio to another layer (either by
resubmitting the same bio, or by submitting a clone), it doesn't need to do
anything special because the bio (or the clone) will once again pass through
blk-crypto, which will work as described in Case 3. If a layered device wants
for some reason to do the IO by itself instead of passing it on to a child
device, but it also chose to expose IE capabilities by setting up a KSM in its
request queue, it is then responsible for en/decrypting the data itself. In
such cases, the device can choose to call the blk-crypto function
``blk_crypto_fallback_to_kernel_crypto_api`` (TODO: Not yet implemented), which will
cause the en/decryption to be done via the crypto API fallback.
=======
Request queue based layered devices like dm-rq that wish to support IE need to
create their own keyslot manager for their request queue, and expose whatever
functionality they choose. When a layered device wants to pass a clone of that
request to another ``request_queue``, blk-crypto will initialize and prepare the
clone as necessary - see ``blk_crypto_insert_cloned_request`` in
``blk-crypto.c``.
>>>>>>> upstream/android-13


Future Optimizations for layered devices
========================================

<<<<<<< HEAD
Creating a keyslot manager for the layered device uses up memory for each
keyslot, and in general, a layered device (like dm-linear) merely passes the
request on to a "child" device, so the keyslots in the layered device itself
might be completely unused. We can instead define a new type of KSM; the
"passthrough KSM", that layered devices can use to let blk-crypto know that
this layered device *will* pass the bio to some child device (and hence
through blk-crypto again, at which point blk-crypto can program the encryption
context, instead of programming it into the layered device's KSM). Again, if
the device "lies" and decides to do the IO itself instead of passing it on to
a child device, it is responsible for doing the en/decryption (and can choose
to call ``blk_crypto_fallback_to_kernel_crypto_api``). Another use case for the
"passthrough KSM" is for IE devices that want to manage their own keyslots/do
not have a limited number of keyslots.
=======
Creating a keyslot manager for a layered device uses up memory for each
keyslot, and in general, a layered device merely passes the request on to a
"child" device, so the keyslots in the layered device itself are completely
unused, and don't need any refcounting or keyslot programming. We can instead
define a new type of KSM; the "passthrough KSM", that layered devices can use
to advertise an unlimited number of keyslots, and support for any encryption
algorithms they choose, while not actually using any memory for each keyslot.
Another use case for the "passthrough KSM" is for IE devices that do not have a
limited number of keyslots.


Interaction between inline encryption and blk integrity
=======================================================

At the time of this patch, there is no real hardware that supports both these
features. However, these features do interact with each other, and it's not
completely trivial to make them both work together properly. In particular,
when a WRITE bio wants to use inline encryption on a device that supports both
features, the bio will have an encryption context specified, after which
its integrity information is calculated (using the plaintext data, since
the encryption will happen while data is being written), and the data and
integrity info is sent to the device. Obviously, the integrity info must be
verified before the data is encrypted. After the data is encrypted, the device
must not store the integrity info that it received with the plaintext data
since that might reveal information about the plaintext data. As such, it must
re-generate the integrity info from the ciphertext data and store that on disk
instead. Another issue with storing the integrity info of the plaintext data is
that it changes the on disk format depending on whether hardware inline
encryption support is present or the kernel crypto API fallback is used (since
if the fallback is used, the device will receive the integrity info of the
ciphertext, not that of the plaintext).

Because there isn't any real hardware yet, it seems prudent to assume that
hardware implementations might not implement both features together correctly,
and disallow the combination for now. Whenever a device supports integrity, the
kernel will pretend that the device does not support hardware inline encryption
(by essentially setting the keyslot manager in the request_queue of the device
to NULL). When the crypto API fallback is enabled, this means that all bios with
and encryption context will use the fallback, and IO will complete as usual.
When the fallback is disabled, a bio with an encryption context will be failed.
>>>>>>> upstream/android-13
