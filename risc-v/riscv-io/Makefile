run: file_access_riscv file_access_x86
	spike pk $<

file_access_riscv: file_access.c
	riscv64-unknown-elf-gcc $^ -o $@

file_access_x86: file_access.c
	gcc $^ -o $@
