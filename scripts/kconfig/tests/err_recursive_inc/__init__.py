<<<<<<< HEAD
=======
# SPDX-License-Identifier: GPL-2.0
>>>>>>> upstream/android-13
"""
Detect recursive inclusion error.

If recursive inclusion is detected, it should fail with error messages.
"""


def test(conf):
    assert conf.oldaskconfig() != 0
    assert conf.stderr_contains('expected_stderr')
