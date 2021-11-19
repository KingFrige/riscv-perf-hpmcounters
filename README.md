# README


## set $RISCV

```
$ module load riscv-toolchian/chipyard

```

## build

generate `riscv-hpmcounters-read`

```
$ make
$ ls -l
```

## usage

```
# on linux that surpport riscv arch
$ ./riscv-hpmcounters-read ls

$ ./riscv-hpmcounters-read "ls -l"
```

## sim

You can use qemu sim it! 

Please referen [FireMarshal](https://firemarshal.readthedocs.io/en/latest/index.html)


## reference

- [FireMarshal](https://firemarshal.readthedocs.io/en/latest/index.html)
- [riscv-hpmcounters](https://github.com/ccelio/riscv-hpmcounters)
- [Micro-architectural Event Tracking](https://docs.boom-core.org/en/latest/sections/uarch-counters.html#micro-architectural-event-tracking)


