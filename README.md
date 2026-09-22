# A53 Renode
A study of cortex-A53 aarch64 using [Renode](https://renode.io/).

# Running QEMU
```bash
qemu-system-aarch64 \
  -machine virt \
  -cpu cortex-a53 \
  -kernel ./build/firmware.elf \
  -S \
  -s
```

# Getting Started Renode w/ A53
## Run using the example
1. Run `renode` to open the `renode` monitor
```bash
renode &
```

2. Run and load the built-in script
```bash
include @scripts/single-node/cortex-a53.resc
```

3. type start in the monitor to begin the execution
```bash
start
```

## Running with no-gui
1. Run renode with --disable-gui option
```bash
renode --disable-gui
```

2. This will create a telnet session which will allow you to connect to it in a different terminal, this is useful in automation
```bash
telnet localhost 1234
```

3. Then, include the machine file and start gdb server
```bash
include @scripts/single-node/cortex-a53.resc
machine StartGdbServer 3333
```

4. Once GDB server is started, all machine/monitor commands can be sent over gdb, at this point you can start the uart console session via
```bash
uart_connect sysbus.uart0
```


# References
- [Arm® Architecture Reference Manual Armv8, for Armv8-A architecture profile](https://developer.arm.com/docs/ddi0487/ea/arm-architecture-reference-manual-armv8-for-armv8-a-architecture-profile)
- [ARM Cortex-A Series Programmer’s Guide for ARMv8-A](https://developer.arm.com/docs/den0024/a/preface)
- [Bare-metal Boot Code for ARMv8-A Processors](https://developer.arm.com/docs/dai0527/a/bare-metal-boot-code-for-armv8-a-processors)
- [Arm® Power State Coordination Interface - Platform Design Document](https://developer.arm.com/docs/den0022/d/arm-power-state-coordination-interface-platform-design-document)
- [aarch64 exception levels](https://krinkinmu.github.io/2021/01/04/aarch64-exception-levels.html)
