#!/bin/sh

#=======================
# Build QEMU for RISC-V
#=======================

JOBS=4
MAKE="make -j${JOBS}"

QEMU_VER=qemu-3.0.0-rc2

ORG_DIR=`pwd`

function build_qemu {
    type qemu-system-riscv64 > /dev/null
    if [ $? -ne 0 ] ; then
        cd /tmp/
        curl -L https://download.qemu.org/${QEMU_VER}.tar.xz | tar xJ
        cd ${QEMU_VER}
        mkdir -p build
        cd build
        ../configure --target-list=riscv64-softmmu
        ${MAKE}
        sudo ${MAKE} install
        cd ${ORG_DIR}
    fi
}

#=========================
# Build Linux Main Source
#=========================

function build_linux {
    echo "Downloading Linux ..."

    LINUX_VER=linux-4.17.11
    rm -rf ${LINUX_VER}

    curl -L https://cdn.kernel.org/pub/linux/kernel/v4.x/linux-4.17.11.tar.xz | tar -xJ
    cd ${LINUX_VER}
    ${MAKE} ARCH=riscv defconfig
    sed -i 's/CONFIG_CROSS_COMPILE=\"\"/CONFIG_CROSS_COMPILE=\"riscv64-unknown-linux-gnu-\"/g' .config
    make ARCH=riscv  # I can't find why -j4 doesn't work
    cd ${ORG_DIR}
    git clone https://github.com/riscv/riscv-pk.git riscv-pk-${LINUX_VER}
    cd riscv-pk-${LINUX_VER}
    mkdir build
    cd build
    ../configure --enable-logo --host=riscv64-unknown-linux-gnu --with-payload=../../${LINUX_VER}/vmlinux
    ${MAKE}
    cd ${ORG_DIR}
}


#=========================
# Build Linux Main Source
#=========================

function build_riscv_linux {
    echo "Downloading RISC-V Linux ..."

    RISCV_LINUX_VER=4.15
    RISCV_LINUX=riscv-linux-${RISCV_LINUX_VER}
    rm -rf ${RISCV_LINUX_VER}

    git clone https://github.com/riscv/riscv-linux.git --recurse-submodules -b riscv-linux-${RISCV_LINUX_VER} --depth 1 ${RISCV_LINUX}
    cd ${RISCV_LINUX}
    ${MAKE} ARCH=riscv defconfig
    sed -i 's/CONFIG_CROSS_COMPILE=\"\"/CONFIG_CROSS_COMPILE=\"riscv64-unknown-linux-gnu-\"/g' .config
    make ARCH=riscv  # I can't find why -j4 doesn't work
    cd ${ORG_DIR}
    git clone https://github.com/riscv/riscv-pk.git riscv-pk-${RISCV_LINUX}
    cd riscv-pk-${RISCV_LINUX}
    mkdir build
    cd build
    ../configure --enable-logo --host=riscv64-unknown-elf --with-payload=../../${RISCV_LINUX}/vmlinux
    ${MAKE}
    cd ${ORG_DIR}
}

#=======================
# Download Fedora Image
#=======================

function download_fedora_image {
    if [ ! -e stage4-disk.img ] ; then
        wget https://fedorapeople.org/groups/risc-v/disk-images/stage4-disk.img.xz
        xzdec -d stage4-disk.img.xz > stage4-disk.img
    fi
}

# build_qemu            > /dev/null
# build_linux           > /dev/null
build_riscv_linux
download_fedora_image > /dev/null
