# GIC Programmers Guide (Baremetal)
This guide assumes the reader understand GIC components.

The three high level step when configuring the GIC is:
  1. Initialize the GICD_CTLR register at Secure State.
  1. Initialize Distributor
  1. Initialize the CPU interface

## Initializing Distributor
The GICD_CTLR contains the global configuration of the GIC Distributor. The GICD_CTLR register fields depends on the current exception level and level of access security. This section provides the steps to take to initialize and configure the distributor:

### 1. Configure the Distributor Security State and Affinity routing at EL3
It is recommended to do this at startup since access to `DS`, `ARE_S` and `ARE_NS` fields of GICD_CTLR is only allowed at EL3 (and baremetal doesn`t require this to be programmable at runtime):
```C
  
  ldr x0, =__gicd_base
  ldr w1, [x0]
  orr w1, w1, #(1U << 6) // disable secure state GICD_CTLR.DS 
  orr w1, w1, #(1U << 4) // enable GICD_CTLR.ARE_S
  orr w1, w1, #(1U << 5) // enable GICD_CTLR.ARE_NS
  str w1, [x0]
```

Setting the GICD_CTLR.DS to `1` permits Non-secure mode to access and modify registers that control `group0` interrupts. This means it permits i.e EL1NS to access and modify group0 interrupts. Without this, `EL1NS` wont be able to access GIC interrupts and must do an `svc` call to request `EL1S` to modify `group0` interrupts.

Enabling affinity routing (`ARE_S`, `ARE_NS`) is a must, this field is for backwards compatibility with **GICv2**, when running with **GICv3** this must always be set to 1.

### 

# Groups
Interrupt lines under group0 routes the interrupt to secure code. Interrupt lines under group1S routes it to secure/trusted OS and interrupt lines under group1 routes it to unsecured os.

Note that the type of interrupt (FIQ or IRQ) also depends on the group the interrupt line belongs to, see mapping

| EL                        | Group0 | Group1S | Group1 |
| ------------------------- | ------ | ------- | ------ |
| Secure EL0, EL1           |  FIQ   |  IRQ    |  FIQ   |
| Non-Secure EL0, EL1, EL2  |  FIQ   |  FIQ    |  IRQ   |
| EL3                       |  FIQ   |  FIQ    |  FIQ   |

Interrupts that routes to non-secure OS or software is always taken as IRQ and interrupts routed to Secure software is always taken as FIQ.

This assumes that the reader is familiar with AArch64 Exception levels.

The groupings is set via the GIC*_IGROUPR and GIC*_IGROUPMODR registers of both distributor and redistributor.

# Security State
GIC has concept of security modes.
