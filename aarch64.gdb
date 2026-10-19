file ./build_aarch64/firmware.elf

target remote :3333
set pagination off
set confirm off
set print pretty on
set print object on
set print array on
set disassemble-next-line on

define bmain
    break main
end

define regs
    info registers x0 x1 x2 x3 x4 x5 x6 x7 x8 x9 x10 x11 x12 x13 x14 x15 x16 x17 x18 x19 x20 x21 x22 x23 x24 x25 x26 x27 x28 x29 x30 sp pc cpsr
end

define machine_reset
    monitor pause
    monitor runMacro $reset
    monitor start
end

define infoall
    info registers
    info breakpoints
    backtrace
end

define info_cntp
  info registers CNTFRQ_EL0 CNTP_CTL_EL0 CNTP_TVAL_EL0 CNTP_CVAL_EL0
end

define gic_spi
    set $irq_id = $arg0
    if $irq_id < 32 || $irq_id > 1019
        printf "GICD SPI ID must be in the range 32..1019 (got %u)\n", $irq_id
    else
        set $irq_word = $irq_id / 32
        set $irq_bit = 1 << ($irq_id % 32)
        printf "GICD configuration for SPI %u (bit mask 0x%x)\n", $irq_id, $irq_bit
        printf "GICD_CTLR:\n"
        x/wx 0x08000000
        printf "IGROUPR[%u]:\n", $irq_word
        x/wx (0x08000080 + 4 * $irq_word)
        printf "IGROUPMODR[%u]:\n", $irq_word
        x/wx (0x08000d00 + 4 * $irq_word)
        printf "ISENABLER[%u]:\n", $irq_word
        x/wx (0x08000100 + 4 * $irq_word)
        printf "ISPENDR[%u]:\n", $irq_word
        x/wx (0x08000200 + 4 * $irq_word)
        printf "ICFGR[%u]:\n", $irq_id / 16
        x/wx (0x08000c00 + 4 * ($irq_id / 16))
        printf "IPRIORITYR[%u]:\n", $irq_id
        x/bx (0x08000400 + $irq_id)
        printf "IROUTER[%u]:\n", $irq_id
        x/gx (0x08006000 + 8 * $irq_id)
    end
end

delete

machine_reset

b gic_v3.c:122
b int_33_irq_handler
b _reset
b main
