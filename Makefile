defualt:build

build:
	riscv64-unknown-linux-gnu-g++ main.c -o riscv-hpmcounters-read

dasm:
	riscv64-unknown-linux-gnu-objdump -S riscv-hpmcounters-read > riscv-hpmcounters-read.asm

clean:
	rm -rf riscv-hpmcounters-read
