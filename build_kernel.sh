#!/bin/bash

<<<<<<< HEAD
export PATH=$(pwd)/toolchain/bin:$PATH
export CROSS_COMPILE=$(pwd)/toolchain/bin/aarch64-linux-gnu
export CC=$(pwd)/toolchain/bin/clang
export CLANG_TRIPLE=aarch64-linux-gnu
export ARCH=arm64
export PLATFORM_VERSION=14
export TARGET_SOC=mt6877
export TARGET_BUILD_VARIANT=user

export KCFLAGS=-w
export CONFIG_SECTION_MISMATCH_WARN_ONLY=y

export KCFLAGS=-w
export CONFIG_SECTION_MISMATCH_WARN_ONLY=y

make -C $(pwd) O=$(pwd)/out KCFLAGS=-w CONFIG_SECTION_MISMATCH_WARN_ONLY=y LLVM=1 LLVM_IAS=1 a34x_defconfig
make -C $(pwd) O=$(pwd)/out KCFLAGS=-w CONFIG_SECTION_MISMATCH_WARN_ONLY=y LLVM=1 LLVM_IAS=1 -j16

cp out/arch/arm64/boot/Image $(pwd)/arch/arm64/boot/Image

=======
export PATH=$(pwd)/toolchain/clang/host/linux-x86/clang-r450784d/bin:$PATH
export PATH=$(pwd)/toolchain/build/kernel/build-tools/path/linux-x86/:$PATH
export HOSTCFLAGS="--sysroot=$(pwd)/toolchain/build/kernel/build-tools/sysroot -I$(pwd)/toolchain/prebuilts/kernel-build-tools/linux-x86/include"
export HOSTLDFLAGS="--sysroot=$(pwd)/toolchain/build/kernel/build-tools/sysroot  -Wl,-rpath,$(pwd)/toolchain/prebuilts/kernel-build-tools/linux-x86/lib64 -L $(pwd)/toolchain/prebuilts/kernel-build-tools/linux-x86/lib64 -fuse-ld=lld --rtlib=compiler-rt"

export DTC_FLAGS="-@"
export PLATFORM_VERSION=13
export ANDROID_MAJOR_VERSION=t
export LLVM=1
export DEPMOD=depmod
export ARCH=arm64
export TARGET_SOC=s5e8535
make s5e8535-a14xxx_defconfig
make
>>>>>>> upstream/android-13
