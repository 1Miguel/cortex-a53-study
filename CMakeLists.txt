cmake_minimum_required(VERSION 3.20)

project(cortex_a53_study LANGUAGES C ASM)

# Why?
#
# This indicates that startup.asm is an assembly language
# and to compile this it has to pass compiler flags
# "-x assembler". This will, at build time, invoke
#
# "{prefix}-gcc -x assembler <file>
#
# to which "{prefix}-gcc -x assembler" is similar to
# invoking "{prefix}-as <file>", basically telling the
# compiler to use assembler to process the target source
# file.
set_source_files_properties(
    bsp/aarch64/startup.asm
    PROPERTIES
        LANGUAGE ASM
        COMPILE_OPTIONS "-x;assembler"
)

add_executable(firmware
    src/main.c
    bsp/aarch64/el3_exceptions.c
    bsp/aarch64/startup.asm
    bsp/common/syscalls.c
    drivers/uart.c
)

target_include_directories(firmware
  PRIVATE
    drivers
    bsp/aarch64
    bsp/common
)

set_target_properties(firmware PROPERTIES
    OUTPUT_NAME firmware
    SUFFIX ".elf"
)

target_create_bin(firmware)
target_objdump(firmware)
