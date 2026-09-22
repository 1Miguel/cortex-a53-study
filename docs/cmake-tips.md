
CMake only recognize `.s` file types as assembly files. If an asm source file requires a different extension, use `set_source_file_properties` to explicitly indicate that a source file is an `ASM` file by setting the `LANGUAGE` to `ASM`. The `COMPILE_OPTIONS` indicates that when compiler is invoked, invoked it with flag `-x assembler` i.
```bash
  {prefix}-gcc -x assembler <file>
```

In gcc, invoking this is equivalent to invoking
```bash
  {prefix}-as <file>
```
basically telling the compiler to use only the assembler to process the target source file.

```cmake
if (TARGET_ARCH STREQUAL "aarch64")
  set_source_files_properties(
      bsp/aarch64/startup.asm
      PROPERTIES
          LANGUAGE ASM
          COMPILE_OPTIONS "-x;assembler"
  )
endif()
```
