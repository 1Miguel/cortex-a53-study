file ./build/firmware.elf
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
end

define infoall
    info registers
    info breakpoints
    backtrace
end

delete

machine_reset

break _Reset
break main
