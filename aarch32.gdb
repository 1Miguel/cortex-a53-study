
#set architecture arm
set pagination off
set confirm off
set print pretty on
set print object on
set print array on
set disassemble-next-line on

file ./build_aarch32/firmware.elf
target remote :3333

define bmain
    break main
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

delete

machine_reset
b _reset
b main
