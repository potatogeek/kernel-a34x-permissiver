.. SPDX-License-Identifier: GPL-2.0

.. _netdev-FAQ:

==========
netdev FAQ
==========

<<<<<<< HEAD
Q: What is netdev?
------------------
A: It is a mailing list for all network-related Linux stuff.  This
=======
What is netdev?
---------------
It is a mailing list for all network-related Linux stuff.  This
>>>>>>> upstream/android-13
includes anything found under net/ (i.e. core code like IPv6) and
drivers/net (i.e. hardware specific drivers) in the Linux source tree.

Note that some subsystems (e.g. wireless drivers) which have a high
volume of traffic have their own specific mailing lists.

The netdev list is managed (like many other Linux mailing lists) through
VGER (http://vger.kernel.org/) and archives can be found below:

-  http://marc.info/?l=linux-netdev
-  http://www.spinics.net/lists/netdev/

Aside from subsystems like that mentioned above, all network-related
Linux development (i.e. RFC, review, comments, etc.) takes place on
netdev.

<<<<<<< HEAD
Q: How do the changes posted to netdev make their way into Linux?
-----------------------------------------------------------------
A: There are always two trees (git repositories) in play.  Both are
=======
How do the changes posted to netdev make their way into Linux?
--------------------------------------------------------------
There are always two trees (git repositories) in play.  Both are
>>>>>>> upstream/android-13
driven by David Miller, the main network maintainer.  There is the
``net`` tree, and the ``net-next`` tree.  As you can probably guess from
the names, the ``net`` tree is for fixes to existing code already in the
mainline tree from Linus, and ``net-next`` is where the new code goes
for the future release.  You can find the trees here:

<<<<<<< HEAD
- https://git.kernel.org/pub/scm/linux/kernel/git/davem/net.git
- https://git.kernel.org/pub/scm/linux/kernel/git/davem/net-next.git

Q: How often do changes from these trees make it to the mainline Linus tree?
----------------------------------------------------------------------------
A: To understand this, you need to know a bit of background information on
=======
- https://git.kernel.org/pub/scm/linux/kernel/git/netdev/net.git
- https://git.kernel.org/pub/scm/linux/kernel/git/netdev/net-next.git

How often do changes from these trees make it to the mainline Linus tree?
-------------------------------------------------------------------------
To understand this, you need to know a bit of background information on
>>>>>>> upstream/android-13
the cadence of Linux development.  Each new release starts off with a
two week "merge window" where the main maintainers feed their new stuff
to Linus for merging into the mainline tree.  After the two weeks, the
merge window is closed, and it is called/tagged ``-rc1``.  No new
features get mainlined after this -- only fixes to the rc1 content are
expected.  After roughly a week of collecting fixes to the rc1 content,
rc2 is released.  This repeats on a roughly weekly basis until rc7
(typically; sometimes rc6 if things are quiet, or rc8 if things are in a
state of churn), and a week after the last vX.Y-rcN was done, the
official vX.Y is released.

Relating that to netdev: At the beginning of the 2-week merge window,
the ``net-next`` tree will be closed - no new changes/features.  The
accumulated new content of the past ~10 weeks will be passed onto
mainline/Linus via a pull request for vX.Y -- at the same time, the
``net`` tree will start accumulating fixes for this pulled content
relating to vX.Y

An announcement indicating when ``net-next`` has been closed is usually
sent to netdev, but knowing the above, you can predict that in advance.

IMPORTANT: Do not send new ``net-next`` content to netdev during the
period during which ``net-next`` tree is closed.

Shortly after the two weeks have passed (and vX.Y-rc1 is released), the
tree for ``net-next`` reopens to collect content for the next (vX.Y+1)
release.

If you aren't subscribed to netdev and/or are simply unsure if
``net-next`` has re-opened yet, simply check the ``net-next`` git
repository link above for any new networking-related commits.  You may
also check the following website for the current status:

  http://vger.kernel.org/~davem/net-next.html

The ``net`` tree continues to collect fixes for the vX.Y content, and is
fed back to Linus at regular (~weekly) intervals.  Meaning that the
focus for ``net`` is on stabilization and bug fixes.

Finally, the vX.Y gets released, and the whole cycle starts over.

<<<<<<< HEAD
Q: So where are we now in this cycle?
=======
So where are we now in this cycle?
----------------------------------
>>>>>>> upstream/android-13

Load the mainline (Linus) page here:

  https://git.kernel.org/pub/scm/linux/kernel/git/torvalds/linux.git

and note the top of the "tags" section.  If it is rc1, it is early in
the dev cycle.  If it was tagged rc7 a week ago, then a release is
probably imminent.

<<<<<<< HEAD
Q: How do I indicate which tree (net vs. net-next) my patch should be in?
-------------------------------------------------------------------------
A: Firstly, think whether you have a bug fix or new "next-like" content.
=======
How do I indicate which tree (net vs. net-next) my patch should be in?
----------------------------------------------------------------------
Firstly, think whether you have a bug fix or new "next-like" content.
>>>>>>> upstream/android-13
Then once decided, assuming that you use git, use the prefix flag, i.e.
::

  git format-patch --subject-prefix='PATCH net-next' start..finish

Use ``net`` instead of ``net-next`` (always lower case) in the above for
bug-fix ``net`` content.  If you don't use git, then note the only magic
in the above is just the subject text of the outgoing e-mail, and you
can manually change it yourself with whatever MUA you are comfortable
with.

<<<<<<< HEAD
Q: I sent a patch and I'm wondering what happened to it?
--------------------------------------------------------
Q: How can I tell whether it got merged?
A: Start by looking at the main patchworks queue for netdev:

  http://patchwork.ozlabs.org/project/netdev/list/
=======
I sent a patch and I'm wondering what happened to it - how can I tell whether it got merged?
--------------------------------------------------------------------------------------------
Start by looking at the main patchworks queue for netdev:

  https://patchwork.kernel.org/project/netdevbpf/list/
>>>>>>> upstream/android-13

The "State" field will tell you exactly where things are at with your
patch.

<<<<<<< HEAD
Q: The above only says "Under Review".  How can I find out more?
----------------------------------------------------------------
A: Generally speaking, the patches get triaged quickly (in less than
=======
The above only says "Under Review".  How can I find out more?
-------------------------------------------------------------
Generally speaking, the patches get triaged quickly (in less than
>>>>>>> upstream/android-13
48h).  So be patient.  Asking the maintainer for status updates on your
patch is a good way to ensure your patch is ignored or pushed to the
bottom of the priority list.

<<<<<<< HEAD
Q: I submitted multiple versions of the patch series
----------------------------------------------------
Q: should I directly update patchwork for the previous versions of these
patch series?
A: No, please don't interfere with the patch status on patchwork, leave
=======
I submitted multiple versions of the patch series. Should I directly update patchwork for the previous versions of these patch series?
--------------------------------------------------------------------------------------------------------------------------------------
No, please don't interfere with the patch status on patchwork, leave
>>>>>>> upstream/android-13
it to the maintainer to figure out what is the most recent and current
version that should be applied. If there is any doubt, the maintainer
will reply and ask what should be done.

<<<<<<< HEAD
Q: How can I tell what patches are queued up for backporting to the various stable releases?
--------------------------------------------------------------------------------------------
A: Normally Greg Kroah-Hartman collects stable commits himself, but for
networking, Dave collects up patches he deems critical for the
networking subsystem, and then hands them off to Greg.

There is a patchworks queue that you can see here:

  http://patchwork.ozlabs.org/bundle/davem/stable/?state=*

It contains the patches which Dave has selected, but not yet handed off
to Greg.  If Greg already has the patch, then it will be here:

  https://git.kernel.org/pub/scm/linux/kernel/git/stable/stable-queue.git

A quick way to find whether the patch is in this stable-queue is to
simply clone the repo, and then git grep the mainline commit ID, e.g.
::

  stable-queue$ git grep -l 284041ef21fdf2e
  releases/3.0.84/ipv6-fix-possible-crashes-in-ip6_cork_release.patch
  releases/3.4.51/ipv6-fix-possible-crashes-in-ip6_cork_release.patch
  releases/3.9.8/ipv6-fix-possible-crashes-in-ip6_cork_release.patch
  stable/stable-queue$

Q: I see a network patch and I think it should be backported to stable.
-----------------------------------------------------------------------
Q: Should I request it via stable@vger.kernel.org like the references in
the kernel's Documentation/process/stable-kernel-rules.rst file say?
A: No, not for networking.  Check the stable queues as per above first
to see if it is already queued.  If not, then send a mail to netdev,
listing the upstream commit ID and why you think it should be a stable
candidate.

Before you jump to go do the above, do note that the normal stable rules
in :ref:`Documentation/process/stable-kernel-rules.rst <stable_kernel_rules>`
still apply.  So you need to explicitly indicate why it is a critical
fix and exactly what users are impacted.  In addition, you need to
convince yourself that you *really* think it has been overlooked,
vs. having been considered and rejected.

Generally speaking, the longer it has had a chance to "soak" in
mainline, the better the odds that it is an OK candidate for stable.  So
scrambling to request a commit be added the day after it appears should
be avoided.

Q: I have created a network patch and I think it should be backported to stable.
--------------------------------------------------------------------------------
Q: Should I add a Cc: stable@vger.kernel.org like the references in the
kernel's Documentation/ directory say?
A: No.  See above answer.  In short, if you think it really belongs in
stable, then ensure you write a decent commit log that describes who
gets impacted by the bug fix and how it manifests itself, and when the
bug was introduced.  If you do that properly, then the commit will get
handled appropriately and most likely get put in the patchworks stable
queue if it really warrants it.

If you think there is some valid information relating to it being in
stable that does *not* belong in the commit log, then use the three dash
marker line as described in
:ref:`Documentation/process/submitting-patches.rst <the_canonical_patch_format>`
to temporarily embed that information into the patch that you send.

Q: Are all networking bug fixes backported to all stable releases?
------------------------------------------------------------------
A: Due to capacity, Dave could only take care of the backports for the
last two stable releases. For earlier stable releases, each stable
branch maintainer is supposed to take care of them. If you find any
patch is missing from an earlier stable branch, please notify
stable@vger.kernel.org with either a commit ID or a formal patch
backported, and CC Dave and other relevant networking developers.

Q: Is the comment style convention different for the networking content?
------------------------------------------------------------------------
A: Yes, in a largely trivial way.  Instead of this::
=======
I made changes to only a few patches in a patch series should I resend only those changed?
------------------------------------------------------------------------------------------
No, please resend the entire patch series and make sure you do number your
patches such that it is clear this is the latest and greatest set of patches
that can be applied.

I submitted multiple versions of a patch series and it looks like a version other than the last one has been accepted, what should I do?
----------------------------------------------------------------------------------------------------------------------------------------
There is no revert possible, once it is pushed out, it stays like that.
Please send incremental versions on top of what has been merged in order to fix
the patches the way they would look like if your latest patch series was to be
merged.

Are there special rules regarding stable submissions on netdev?
---------------------------------------------------------------
While it used to be the case that netdev submissions were not supposed
to carry explicit ``CC: stable@vger.kernel.org`` tags that is no longer
the case today. Please follow the standard stable rules in
:ref:`Documentation/process/stable-kernel-rules.rst <stable_kernel_rules>`,
and make sure you include appropriate Fixes tags!

Is the comment style convention different for the networking content?
---------------------------------------------------------------------
Yes, in a largely trivial way.  Instead of this::
>>>>>>> upstream/android-13

  /*
   * foobar blah blah blah
   * another line of text
   */

it is requested that you make it look like this::

  /* foobar blah blah blah
   * another line of text
   */

<<<<<<< HEAD
Q: I am working in existing code that has the former comment style and not the latter.
--------------------------------------------------------------------------------------
Q: Should I submit new code in the former style or the latter?
A: Make it the latter style, so that eventually all code in the domain
of netdev is of this format.

Q: I found a bug that might have possible security implications or similar.
---------------------------------------------------------------------------
Q: Should I mail the main netdev maintainer off-list?**
A: No. The current netdev maintainer has consistently requested that
=======
I am working in existing code that has the former comment style and not the latter. Should I submit new code in the former style or the latter?
-----------------------------------------------------------------------------------------------------------------------------------------------
Make it the latter style, so that eventually all code in the domain
of netdev is of this format.

I found a bug that might have possible security implications or similar. Should I mail the main netdev maintainer off-list?
---------------------------------------------------------------------------------------------------------------------------
No. The current netdev maintainer has consistently requested that
>>>>>>> upstream/android-13
people use the mailing lists and not reach out directly.  If you aren't
OK with that, then perhaps consider mailing security@kernel.org or
reading about http://oss-security.openwall.org/wiki/mailing-lists/distros
as possible alternative mechanisms.

<<<<<<< HEAD
Q: What level of testing is expected before I submit my change?
---------------------------------------------------------------
A: If your changes are against ``net-next``, the expectation is that you
=======
What level of testing is expected before I submit my change?
------------------------------------------------------------
If your changes are against ``net-next``, the expectation is that you
>>>>>>> upstream/android-13
have tested by layering your changes on top of ``net-next``.  Ideally
you will have done run-time testing specific to your change, but at a
minimum, your changes should survive an ``allyesconfig`` and an
``allmodconfig`` build without new warnings or failures.

<<<<<<< HEAD
Q: Any other tips to help ensure my net/net-next patch gets OK'd?
-----------------------------------------------------------------
A: Attention to detail.  Re-read your own work as if you were the
=======
How do I post corresponding changes to user space components?
-------------------------------------------------------------
User space code exercising kernel features should be posted
alongside kernel patches. This gives reviewers a chance to see
how any new interface is used and how well it works.

When user space tools reside in the kernel repo itself all changes
should generally come as one series. If series becomes too large
or the user space project is not reviewed on netdev include a link
to a public repo where user space patches can be seen.

In case user space tooling lives in a separate repository but is
reviewed on netdev  (e.g. patches to `iproute2` tools) kernel and
user space patches should form separate series (threads) when posted
to the mailing list, e.g.::

  [PATCH net-next 0/3] net: some feature cover letter
   └─ [PATCH net-next 1/3] net: some feature prep
   └─ [PATCH net-next 2/3] net: some feature do it
   └─ [PATCH net-next 3/3] selftest: net: some feature

  [PATCH iproute2-next] ip: add support for some feature

Posting as one thread is discouraged because it confuses patchwork
(as of patchwork 2.2.2).

Can I reproduce the checks from patchwork on my local machine?
--------------------------------------------------------------

Checks in patchwork are mostly simple wrappers around existing kernel
scripts, the sources are available at:

https://github.com/kuba-moo/nipa/tree/master/tests

Running all the builds and checks locally is a pain, can I post my patches and have the patchwork bot validate them?
--------------------------------------------------------------------------------------------------------------------

No, you must ensure that your patches are ready by testing them locally
before posting to the mailing list. The patchwork build bot instance
gets overloaded very easily and netdev@vger really doesn't need more
traffic if we can help it.

netdevsim is great, can I extend it for my out-of-tree tests?
-------------------------------------------------------------

No, `netdevsim` is a test vehicle solely for upstream tests.
(Please add your tests under tools/testing/selftests/.)

We also give no guarantees that `netdevsim` won't change in the future
in a way which would break what would normally be considered uAPI.

Is netdevsim considered a "user" of an API?
-------------------------------------------

Linux kernel has a long standing rule that no API should be added unless
it has a real, in-tree user. Mock-ups and tests based on `netdevsim` are
strongly encouraged when adding new APIs, but `netdevsim` in itself
is **not** considered a use case/user.

Any other tips to help ensure my net/net-next patch gets OK'd?
--------------------------------------------------------------
Attention to detail.  Re-read your own work as if you were the
>>>>>>> upstream/android-13
reviewer.  You can start with using ``checkpatch.pl``, perhaps even with
the ``--strict`` flag.  But do not be mindlessly robotic in doing so.
If your change is a bug fix, make sure your commit log indicates the
end-user visible symptom, the underlying reason as to why it happens,
and then if necessary, explain why the fix proposed is the best way to
get things done.  Don't mangle whitespace, and as is common, don't
mis-indent function arguments that span multiple lines.  If it is your
first patch, mail it to yourself so you can test apply it to an
unpatched tree to confirm infrastructure didn't mangle it.

Finally, go back and read
:ref:`Documentation/process/submitting-patches.rst <submittingpatches>`
to be sure you are not repeating some common mistake documented there.
