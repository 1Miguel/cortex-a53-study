cmake_minimum_required(VERSION 3.20)

project(cortex_a53_study LANGUAGES C ASM)

set(BSP_DIR "${CMAKE_CURRENT_SOURCE_DIR}/bsp/aarch64")

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
    "${BSP_DIR}/startup.asm"
    PROPERTIES
        LANGUAGE ASM
        COMPILE_OPTIONS "-x;assembler"
)

add_executable(firmware
    src/main.c
    ${BSP_DIR}/el3_exceptions.c
    ${BSP_DIR}/startup.asm
    ${BSP_DIR}/syscalls.c
)

target_include_directories(firmware
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/drivers
    ${CMAKE_CURRENT_SOURCE_DIR}/bsp/aarch64
)

target_sources(firmware
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/drivers/uart.c
)

set_target_properties(firmware PROPERTIES
    OUTPUT_NAME firmware
    SUFFIX ".elf"
)

target_create_bin(firmware)
target_objdump(firmware)
