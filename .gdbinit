file _main.elf
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

define infoall
    info registers
    info breakpoints
    backtrace
end

break _main
