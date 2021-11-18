defualt:build

build:
	riscv64-unknown-linux-gnu-g++ main.c -o riscv-hpmcounters-read.riscv

clean:
	rm -rf *.riscv
