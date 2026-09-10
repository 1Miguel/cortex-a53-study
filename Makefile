# makefile parent directory
MAKEFILE_DIR := $(patsubst %/,%,$(dir $(abspath $(lastword $(MAKEFILE_LIST)))))

# compiler prefix (aarch64-none=baremetal)
AARCH64_PREFIX = aarch64-none-elf-

# clang-format to use
CLANG_FORMAT := clang-format-18

# linker file
LINKER_FILE = $(MAKEFILE_DIR)/bsp/aarch64/linker.ld

# list of all files to be clang formatted
CLANG_FORMAT_DIR := \
	$(MAKEFILE_DIR)/bsp/aarch64 \
	$(MAKEFILE_DIR)/drivers \
	$(MAKEFILE_DIR)/src

# Args explained:
# 	-c : compile source code into object but dont link informations
# 			 this is a must in baremetal since linking happens at a
# 			 later stage, since we create different linkers.
# 	-g : include debugging informations
COMPILE_FLAGS = -c -g

all: _main.elf

_main.o: ./src/_main.c
	$(AARCH64_PREFIX)gcc $(COMPILE_FLAGS) $< -o $@

startup64.o: ./bsp/aarch64/startup.a64
	$(AARCH64_PREFIX)as $(COMPILE_FLAGS) $< -o $@

_main.elf: _main.o startup64.o
	$(AARCH64_PREFIX)ld -T$(LINKER_FILE) $^ -o $@

_main.bin: _main.elf
	$(AARCH64_PREFIX)objcopy -O binary $< $@

clean:
	rm -f _main.elf startup64.o _main.o

clang_format:
	find $(CLANG_FORMAT_DIR) -type f \( -name '*.c' -o -name '*.h' \) -print0 \
		| xargs -0 $(CLANG_FORMAT) --style=file -i
