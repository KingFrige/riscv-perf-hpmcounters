defualt:build

build:
	riscv64-unknown-linux-gnu-g++ main.c -o riscv-hpmcounters-read

clean:
	rm -rf riscv-hpmcounters-read
