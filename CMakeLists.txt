cmake_minimum_required(VERSION 3.20)

if (NOT DEFINED TARGET_ARCH)
  set(TARGET_ARCH aarch64)
endif()

if (TARGET_ARCH STREQUAL "aarch64")
  set(CMAKE_TOOLCHAIN_FILE ./cmake/aarch64-none-elf.cmake CACHE STRING "")
elseif(TARGET_ARCH STREQUAL "aarch32")
  set(LINKER_SCRIPT "${CMAKE_CURRENT_SOURCE_DIR}/bsp/aarch32/linker.ld")
  set(CMAKE_TOOLCHAIN_FILE ./cmake/arm-none-eabi.cmake CACHE STRING "")
else()
  message(FATAL "${TARGET_ARCH} Target Architecture is unsupported")
endif()

project(cortex_a53_study LANGUAGES C ASM)

message(STATUS "Target Architecture: ${TARGET_ARCH}")

add_executable(firmware
    src/main.c
    bsp/common/syscalls.c
    drivers/uart.c
    $<$<STREQUAL:"${TARGET_ARCH}","aarch64">:bsp/aarch64/el3_exceptions.c>
    $<$<STREQUAL:"${TARGET_ARCH}","aarch64">:bsp/aarch64/startup.s>

    $<$<STREQUAL:"${TARGET_ARCH}","aarch32">:bsp/aarch32/exceptions.c>
    $<$<STREQUAL:"${TARGET_ARCH}","aarch32">:bsp/aarch32/startup.s>
)

target_include_directories(firmware
  PRIVATE
    drivers
    bsp/common
    $<$<STREQUAL:"${TARGET_ARCH}","aarch64">:${CMAKE_CURRENT_LIST_DIR}/bsp/aarch64>
)

set_target_properties(firmware PROPERTIES
    OUTPUT_NAME firmware
    SUFFIX ".elf"
)

target_create_bin(firmware)

target_objdump(firmware)
