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
    -kernel ./build_aarch64/firmware.elf \
    -S \
    -gdb tcp::3333
```
> [!NOTE] Option Meaning
> `qemu-system-aarch64` Starts QEMU's AArch64 system emulator. It emulates an ARM 64-bit machine.
-machine virt	Uses QEMU's generic ARM virt machine, which provides virtual hardware such as RAM, interrupt controller, UART, etc.
-cpu cortex-a53	Emulates an ARM Cortex-A53 CPU.
-kernel ./build/firmware.elf	Loads firmware.elf as the guest kernel/firmware image. QEMU examines the ELF program headers to determine where to load its segments.
-S	Do not start the virtual CPU. QEMU starts with the CPU paused. You normally use this when connecting a debugger such as GDB.
-s	Enables QEMU's built-in GDB server on TCP port 1234. It is equivalent to -gdb tcp::1234.

## QEMU with ARMv9
```bash
qemu-system-aarch64 \
    -M virt,mte=on,virtualization=on,gic-version=3 \
    -cpu max \
    -accel tcg \
    -m 4G \
    -smp 4 \
    -nographic \
    -kernel path/to/Image \
    -append "console=ttyAMA0 root=/dev/vda ro" \
    -drive if=none,id=hd0,file=path/to/rootfs.ext4,format=raw \
    -device virtio-blk-device,drive=hd0
```

The command starts an AArch64 virtual machine, boots a Linux kernel directly, and attaches a disk image containing the root filesystem.
  * `qemu-system-aarch64` Runs QEMU's 64-bit ARM system emulator.
  * `-M virt,mte=on,virtualization=on,gic-version=3` Selects QEMU's generic `virt` machine, enables the Memory Tagging Extension (MTE) and virtualization extensions, and uses a GICv3 interrupt controller. The guest kernel and CPU model must support the features you intend to use.
  * `-cpu max` Exposes a broad set of CPU features supported by this QEMU build. The exact set can vary with QEMU version and accelerator.
  * `-accel tcg` Uses QEMU's software emulation engine (TCG), rather than hardware virtualization such as KVM. TCG is portable but usually slower.
  * `-m 4G` Assigns 4 GiB of RAM to the virtual machine.
  * `-smp 4` Configures four virtual CPUs.
  * `-nographic` Runs without a graphical display and uses the terminal for console I/O.
  * `-kernel path/to/Image` Loads the Linux kernel image directly. Replace this placeholder with the path to your kernel image.
  * `-append "console=ttyAMA0 root=/dev/vda ro"` Passes arguments to the kernel: use the ARM PL011 serial console (`ttyAMA0`), mount `/dev/vda` as the root filesystem, and initially mount it read-only.
  * `-drive if=none,id=hd0,file=path/to/rootfs.ext4,format=raw` Defines a disk backed by the specified file. `if=none` leaves it unattached until a device references it; `id=hd0` gives it a name; `format=raw` identifies the image format. Replace the path with your root filesystem image.
  * `-device virtio-blk-device,drive=hd0` Attaches the `hd0` disk as a VirtIO block device. The guest sees it as `/dev/vda`, matching the `root=/dev/vda` kernel argument.

For this to boot successfully, the kernel must include support for the serial console and VirtIO block devices, and the disk image must contain a usable root filesystem.


# Running in Renode


# References
- [Arm® Architecture Reference Manual Armv8, for Armv8-A architecture profile](https://developer.arm.com/docs/ddi0487/ea/arm-architecture-reference-manual-armv8-for-armv8-a-architecture-profile)
- [ARM Cortex-A Series Programmer’s Guide for ARMv8-A](https://developer.arm.com/docs/den0024/a/preface)
- [Bare-metal Boot Code for ARMv8-A Processors](https://developer.arm.com/docs/dai0527/a/bare-metal-boot-code-for-armv8-a-processors)
- [Arm® Power State Coordination Interface - Platform Design Document](https://developer.arm.com/docs/den0022/d/arm-power-state-coordination-interface-platform-design-document)
- [aarch64 exception levels](https://krinkinmu.github.io/2021/01/04/aarch64-exception-levels.html)
