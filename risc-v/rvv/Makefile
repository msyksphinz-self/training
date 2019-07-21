.PHONY: all

all: rvv rvv.o

.c.o:
	riscv64-unknown-elf-gcc -O3 -march=rv64gcv -c $^
	riscv64-unknown-elf-objdump -d $@ > $(subst .o,.dmp, $@)

rvv: rvv.c
	riscv64-unknown-elf-gcc -O3 -march=rv64gcv $^ -o $@
	riscv64-unknown-elf-objdump -d $@ > $@.elf.dmp

run: rvv
	spike -l --isa=rv64gcv --varch=v512:e32:s32 pk $< > run.log 2>&1

simple_run: rvv
	spike --isa=rv64gcv --varch=v512:e32:s32 pk $<

clean:
	rm -rf run.log rvv *.dmp *.o
