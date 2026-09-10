# A53 Renode
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
# AArch64 Boot Flow

In general, the bootflow of aarch64 is:
1. Initialize exceptions
  * Setting up the Vector Table
  * Async exception routing and masking configuration

## Setting up the Vector Table

When the CPU (Processing Element or PE) receives an *interrupt*, it interrupts the soft/firmwares *flow of execution*. In ARM terminology, that is an *exception*.

```

+--------------+    +------------+    +------+
| Peripheral 1 | -> | Interrupt  | -> | CPU  |
+--------------+    | Controller |    +------+
                    +------------+                  
                         ^
+--------------+         |
| Peripheral n | --------+ 
+--------------+
```

Exceptions to the CPU will cause the CPU execute routines.

Each exception level has its own dedicated vector table namely **VBAR_EL{n}** where **n** is the exception level (1, 2, 3). The vector table in AArch64 contains 16 entries and must be placed at a 2KB-aligned address.

# Processing State

>[!Reference]
> ARM DDI 0487G.a The AArch64 System Level Programmers’ Model D1.6 Registers for instruction processing and exception handling

In AArch64, a dedicated stack pointer is implemented for each implemented Exception Level (EL).
  * SP_EL0 for EL0
  * SP_EL1 for EL1
  * SP_EL2 for EL2 (if implemented)
  * SP_EL3 for EL3 (if implemented)

## How Stack Pointer is selected?
When executing at EL0 the processor uses the SP_EL0. When executing at higher Exception level, the processor can be configured to use SP_EL0 or the SP for that exception level. Example is the software executing at EL1, can choose to use SP_EL0 or SP_EL1 by setting the PSTATE.SP.

The selected stack pointer can be indicated by a suffix to the exception level:
  * **t** indicates the use of SP_EL0
  * **h** indicates the use of SP_EL*x*

Example, Exception Level 3 (EL3) has stack pointer selection:
  * SP_EL3**t** (SP_EL0 or SP0) when PSTATE.SP==0
  * SP_EL3**h** (SP_ELx or SPx) when PSTATE.SP==1

## Whats with SP_EL0?
EL0 is the lowest exception level. Usually, you would want software apps to run at this level since it has the least privileges. This is what we call **Thread** mode. When an exception occurs, the processor will handle the exception and execute exception routine, the processor will go into **Handler** mode. The software that executes at higher exception level can choose to use **Handler** Stack Pointer (SP_EL0) or the Stack pointer dedicated to that exception level (SP_ELx). The SP_ELx is also known as **SPx**.

But here's the thing. Since the app uses SP_EL0, normally you woudn't trust that. You wouldn't want to use that stack pointer at higher privilege, since EL0 might be running a malicious software.

# References
- [Arm® Architecture Reference Manual Armv8, for Armv8-A architecture profile](https://developer.arm.com/docs/ddi0487/ea/arm-architecture-reference-manual-armv8-for-armv8-a-architecture-profile)
- [ARM Cortex-A Series Programmer’s Guide for ARMv8-A](https://developer.arm.com/docs/den0024/a/preface)
- [Bare-metal Boot Code for ARMv8-A Processors](https://developer.arm.com/docs/dai0527/a/bare-metal-boot-code-for-armv8-a-processors)
- [Arm® Power State Coordination Interface - Platform Design Document](https://developer.arm.com/docs/den0022/d/arm-power-state-coordination-interface-platform-design-document)
- [aarch64 exception levels](https://krinkinmu.github.io/2021/01/04/aarch64-exception-levels.html)
