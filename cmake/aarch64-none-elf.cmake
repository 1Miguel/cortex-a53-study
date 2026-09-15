set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR aarch64)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)
set(LINKER_SCRIPT "${CMAKE_CURRENT_SOURCE_DIR}/bsp/aarch64/linker.ld")

set(AARCH64_TOOLCHAIN_PREFIX "aarch64-none-elf" CACHE STRING
    "Bare-metal AArch64 toolchain prefix")

find_program(AARCH64_CC
    NAMES "${AARCH64_TOOLCHAIN_PREFIX}-gcc"
    REQUIRED
)
find_program(AARCH64_AS
    NAMES "${AARCH64_TOOLCHAIN_PREFIX}-gcc"
    REQUIRED
)
find_program(AARCH64_OBJCOPY
    NAMES "${AARCH64_TOOLCHAIN_PREFIX}-objcopy"
    REQUIRED
)
find_program(AARCH64_OBJDUMP
    NAMES "${AARCH64_TOOLCHAIN_PREFIX}-objdump"
    REQUIRED
)

set(CMAKE_C_COMPILER "${AARCH64_CC}" CACHE FILEPATH "AArch64 C compiler")
set(CMAKE_ASM_COMPILER "${AARCH64_AS}" CACHE FILEPATH "AArch64 assembler")
set(CMAKE_OBJCOPY "${AARCH64_OBJCOPY}" CACHE FILEPATH "AArch64 objcopy")
set(CMAKE_OBJDUMP "${AARCH64_OBJDUMP}" CACHE FILEPATH "AArch64 objdump")

set(CMAKE_C_COMPILER_TARGET "${AARCH64_TOOLCHAIN_PREFIX}" CACHE STRING
    "AArch64 compiler target")
set(CMAKE_ASM_COMPILER_TARGET "${AARCH64_TOOLCHAIN_PREFIX}" CACHE STRING
    "AArch64 assembler target")

add_compile_options(
  # flag that hints the compiler that the compilation target is a
  # freestanding environment, implies -fno-builtin, program may or may
  # not have a program startup before main.
  -ffreestanding
  -fno-stack-protector

  # instead of one big chunk of mem section for function and data, the
  # compiler splits every function and bariable into its own isolated
  # sections.aarc
  #-fdata-sections
  #-ffunction-sections

  # Optiomization level
  #-O2

  # This option enable adding debug symbols in the generate elf file
  # this requires `.debug` section to be present in the linker script
  # as all debug symbol tables will be placed in `.debug` section.
  #
  # NOTE: that .debug section is a read-only section.
  -g

  # These modify/override the internal configuration of the compiler
  # driver. 
  #
  # because we're doing a baremetal build, we will use nosys.specs
  # which means to link this program with libnosys.a. With libnosys.a
  # it will be up to the program/application to provide a do-nothing
  # stubs.
  #
  # NOTE that nosys.specs options can be found in
  # aarch64-none-elf/lib/nosys.specs
  -Wl,--specs=nosys.specs
)

# Set all linker flags, this is equivalent to calling
#
# aarch64-none-elf-ld $(CMAKE_EXE_LINKER_FLAGS_INIT) -o $(target)
add_link_options(
  -T${LINKER_SCRIPT}
  #-nostdlib
  #-nodefaultlibs
  #-lc
  #-lgcc
  -Wl,--gc-sections
  --specs=nosys.specs
  # ... you might wonder what's with the `-Wl` option? take a minute and read
  # aarch64-none-elf-ld options by invoking 
  #
  #  > man aarch64-none-elf-ld
  #
  # in the terminal .....
  # you'll find that cmake invokes gcc, (indirectly invoking ld), all linker
  # options must be prefix with `-Wld,` i.e -Wl,<option>, which tells gcc that
  # these options must be propagated to ld.
  -Wl,-Map firmware.map

)

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

macro(target_create_bin TARGET_OBJ)
  set(TARGET_FILE_NAME $<TARGET_FILE:${TARGET_OBJ}>)
  add_custom_command(TARGET ${TARGET_OBJ} POST_BUILD
      COMMAND "${CMAKE_OBJCOPY}" -O binary
              # input file
              ${TARGET_FILE_NAME} 
              # output file
              "${CMAKE_CURRENT_BINARY_DIR}/${TARGET_OBJ}.bin"
      BYPRODUCTS "${CMAKE_CURRENT_BINARY_DIR}/${TARGET_OBJ}.bin"
      COMMENT "Creating ${TARGET_OBJ}.bin"
  )
endmacro()

macro(target_objdump TARGET_OBJ)
  add_custom_command(TARGET ${TARGET_OBJ} POST_BUILD
      COMMAND "${CMAKE_OBJDUMP}" -x
              ${TARGET_FILE_NAME} 
              > "${CMAKE_CURRENT_BINARY_DIR}/${TARGET_OBJ}.objdump"
      BYPRODUCTS "${CMAKE_CURRENT_BINARY_DIR}/${TARGET_OBJ}.objdump"
      COMMENT "Creating ${TARGET_OBJ}_dump.objdump"
  )
endmacro()
