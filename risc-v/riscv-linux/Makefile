QEMU_OPT =
QEMU_OPT += -m 4G
QEMU_OPT += -nographic
QEMU_OPT += -machine virt

QEMU_OPT += -object rng-random,filename=/dev/urandom,id=rng0
QEMU_OPT += -device virtio-rng-device,rng=rng0
QEMU_OPT += -append "console=ttyS0 ro root=/dev/vda"
QEMU_OPT += -device virtio-blk-device,drive=hd0
QEMU_OPT += -drive file=stage4-disk.img,format=raw,id=hd0
QEMU_OPT += -device virtio-net-device,netdev=usernet
QEMU_OPT += -netdev user,id=usernet,hostfwd=tcp::10000-:22


run-qemu-riscv-linux:
	qemu-system-riscv64 $(QEMU_OPT) -kernel riscv-pk-riscv-linux-4.15/build/bbl

run-qemu-linux:
	qemu-system-riscv64 $(QEMU_OPT) -kernel riscv-pk-linux-4.17.11/build/bbl

run-qemu-fedora-linux:
	qemu-system-riscv64 $(QEMU_OPT) -kernel fedora/bbl

run-forest-riscv-linux:
	riscvforest --binfile riscv-pk-riscv-linux-4.15/build/bbl --bit-mode 64 --max 5000000

build-env:
	bash ./build_qemu_riscv.sh
