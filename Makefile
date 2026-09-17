
MAKEFILE_DIR := $(patsubst %/,%,$(dir $(abspath $(lastword $(MAKEFILE_LIST)))))

# clang-format command
CLANG_FORMAT := clang-format-18

# list of file that will be clang formatted
CLANG_FORMAT_DIR := \
	$(MAKEFILE_DIR)/bsp/aarch64 \
	$(MAKEFILE_DIR)/drivers \
	$(MAKEFILE_DIR)/src

.PHONY: clang_format

clang_format:
	find $(CLANG_FORMAT_DIR) -type f \( -name '*.c' -o -name '*.h' \) -print0 \
		| xargs -0 $(CLANG_FORMAT) --style=file -i

