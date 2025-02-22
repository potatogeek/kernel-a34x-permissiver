<<<<<<< HEAD
=======
# SPDX-License-Identifier: GPL-2.0
>>>>>>> upstream/android-13
#
# Small script that visualizes the kernel feature support status
# of an architecture.
#
# (If no arguments are given then it will print the host architecture's status.)
#

ARCH=${1:-$(uname -m | sed 's/x86_64/x86/' | sed 's/i386/x86/')}

<<<<<<< HEAD
cd $(dirname $0)
echo "#"
echo "# Kernel feature support matrix of the '$ARCH' architecture:"
echo "#"

for F in */*/arch-support.txt; do
  SUBSYS=$(echo $F | cut -d/ -f1)
  N=$(grep -h "^# Feature name:"        $F | cut -c25-)
  C=$(grep -h "^#         Kconfig:"     $F | cut -c25-)
  D=$(grep -h "^#         description:" $F | cut -c25-)
  S=$(grep -hv "^#" $F | grep -w $ARCH | cut -d\| -f3)

  printf "%10s/%-22s:%s| %35s # %s\n" "$SUBSYS" "$N" "$S" "$C" "$D"
done

=======
$(dirname $0)/../../scripts/get_feat.pl list --arch $ARCH
>>>>>>> upstream/android-13
