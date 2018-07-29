.PHONY: run forest debug-run run_x86 debug-run_x86

run: file_access_riscv
	spike pk $< > $<.log 2>&1
debug-run: file_access_riscv
	spike -l pk $< > $<.debug-log 2>&1
forest: file_access_riscv
	riscvforest --bit-mode 64 --use-pk ${RISCV}/riscv64-unknown-elf/bin/pk --binfile $^ --debug --out $<.forest --trace-hier --trace-out $<.trace --max 500000

run_x86: file_access_x86
	./$<

file_access_riscv: file_access.c
	riscv64-unknown-elf-gcc $^ -o $@

file_access_x86: file_access.c
	gcc $^ -o $@

clean:
	rm -rf file_access_riscv file_access_riscv.log file_access_x86
