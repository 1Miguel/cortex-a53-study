r A53 Renode
A study of cortex-A53 aarch64 using [Renode](https://renode.io/).

# Terminologies
Some ARM terminologies you have to know.
|Terms|Meaning|
|:-|-|
| Execution State | Mode of execution, architecture mode (AArch64/AArch32)|
| EL | Exception Level |
| PE | Processing Element (CPU) |
| IRQ | Interrupt Request, the standard normal-prio Hardware Interrupt. |
| FIQ | Fast Interrupt Request, high prio low latency Hardware Interupt. |
| Thread | Thread mode, runs app tasks. |
| Handler | Handler mode, runs system exceptions or interrupts. |

# Running in QEMU
``` bash
qemu-system-aarch64 \
    -machine virt \
    -cpu cortex-a53 \
    -kernel ./build/firmware.elf \
    -S \
    -s
```
> [!NOTE] Option Meaning
> `qemu-system-aarch64` Starts QEMU's AArch64 system emulator. It emulates an ARM 64-bit machine.
-machine virt	Uses QEMU's generic ARM virt machine, which provides virtual hardware such as RAM, interrupt controller, UART, etc.
-cpu cortex-a53	Emulates an ARM Cortex-A53 CPU.
-kernel ./build/firmware.elf	Loads firmware.elf as the guest kernel/firmware image. QEMU examines the ELF program headers to determine where to load its segments.
-S	Do not start the virtual CPU. QEMU starts with the CPU paused. You normally use this when connecting a debugger such as GDB.
-s	Enables QEMU's built-in GDB server on TCP port 1234. It is equivalent to -gdb tcp::1234.

# Running in Renode


# References
- [Arm® Architecture Reference Manual Armv8, for Armv8-A architecture profile](https://developer.arm.com/docs/ddi0487/ea/arm-architecture-reference-manual-armv8-for-armv8-a-architecture-profile)
- [ARM Cortex-A Series Programmer’s Guide for ARMv8-A](https://developer.arm.com/docs/den0024/a/preface)
- [Bare-metal Boot Code for ARMv8-A Processors](https://developer.arm.com/docs/dai0527/a/bare-metal-boot-code-for-armv8-a-processors)
- [Arm® Power State Coordination Interface - Platform Design Document](https://developer.arm.com/docs/den0022/d/arm-power-state-coordination-interface-platform-design-document)
- [aarch64 exception levels](https://krinkinmu.github.io/2021/01/04/aarch64-exception-levels.html)
