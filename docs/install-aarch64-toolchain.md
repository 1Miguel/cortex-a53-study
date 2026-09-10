# Install the Latest AArch64 Bare-Metal Toolchain

This guide installs the **AArch64 bare-metal GNU toolchain** required to build
64-bit Cortex-A53 firmware for this project and run it in Renode.

The required compiler target is:

```text
aarch64-none-elf
```

Do not use `arm-none-eabi-gcc`; that is a 32-bit ARM toolchain. Do not use
`aarch64-linux-gnu-gcc` unless you are building an application for AArch64
Linux.

## Option 1: Install the latest Arm GNU Toolchain

The latest release is published on Arm's official download page:

<https://developer.arm.com/downloads/-/arm-gnu-toolchain-downloads>

The release number changes over time, so select the newest **Linux x86_64
 hosted cross toolchain** whose archive name contains:

```text
aarch64-none-elf
```

For example, the archive name generally looks similar to:

```text
arm-gnu-toolchain-<version>-x86_64-aarch64-none-elf.tar.xz
```

### 1. Install prerequisites

On Ubuntu or Debian:

```bash
sudo apt update
sudo apt install -y curl xz-utils tar make
```

### 2. Download the newest archive

Open the Arm download page, copy the download URL for the newest
`x86_64-aarch64-none-elf.tar.xz` archive, then run:

```bash
cd ~/Downloads
curl -fL -O 'PASTE_THE_ARM_DOWNLOAD_URL_HERE'
```

Verify that the archive was downloaded:

```bash
ls -lh arm-gnu-toolchain-*-x86_64-aarch64-none-elf.tar.xz
```

If Arm provides a SHA-256 checksum beside the download, verify it before
installing:

```bash
sha256sum arm-gnu-toolchain-*-x86_64-aarch64-none-elf.tar.xz
```

Compare the printed hash with Arm's published checksum.

### 3. Extract it under `/opt`

Replace `<archive>` with the downloaded filename:

```bash
cd ~/Downloads
sudo mkdir -p /opt
sudo tar -xJf <archive> -C /opt
```

The archive creates a versioned directory such as:

```text
/opt/arm-gnu-toolchain-<version>-x86_64-aarch64-none-elf/
```

### 4. Add the compiler to `PATH`

For Bash:

```bash
TOOLCHAIN_DIR="$(find /opt -maxdepth 1 -type d -name 'arm-gnu-toolchain-*-x86_64-aarch64-none-elf' | sort -V | tail -n 1)"
echo "export PATH=\"$TOOLCHAIN_DIR/bin:\$PATH\"" >> ~/.bashrc
source ~/.bashrc
```

For Zsh:

```bash
TOOLCHAIN_DIR="$(find /opt -maxdepth 1 -type d -name 'arm-gnu-toolchain-*-x86_64-aarch64-none-elf' | sort -V | tail -n 1)"
echo "export PATH=\"$TOOLCHAIN_DIR/bin:\$PATH\"" >> ~/.zshrc
source ~/.zshrc
```

Check that the compiler is available:

```bash
command -v aarch64-none-elf-gcc
aarch64-none-elf-gcc --version
aarch64-none-elf-gcc -dumpmachine
```

The final command should print:

```text
aarch64-none-elf
```

## Option 2: Install the Ubuntu/Debian package

This is easier, but the repository version may not be the newest Arm release:

```bash
sudo apt update
sudo apt install -y gcc-aarch64-none-elf binutils-aarch64-none-elf
```

Verify it:

```bash
aarch64-none-elf-gcc --version
aarch64-none-elf-gcc -dumpmachine
```

Use Option 1 when “latest” is important. Use Option 2 for a quick, integrated
system installation.

## Build the A53 Renode firmware

From the project root:

```bash
cd ~/a53-renode
mkdir -p build

aarch64-none-elf-as \
    -mcpu=cortex-a53 \
    -o build/startup64.o \
    bsp/aarch64/startup.a64

aarch64-none-elf-gcc \
    -mcpu=cortex-a53 \
    -ffreestanding \
    -fno-builtin \
    -fno-stack-protector \
    -nostdlib \
    -nostartfiles \
    -c src/_main.c \
    -o build/main.o

aarch64-none-elf-ld \
    -m aarch64elf \
    -T bsp/aarch64/linker.ld \
    -o build/firmware.elf \
    build/startup64.o \
    build/main.o

aarch64-none-elf-objcopy \
    -O binary \
    build/firmware.elf \
    build/firmware.bin
```

Inspect the result:

```bash
aarch64-none-elf-readelf -h build/firmware.elf
aarch64-none-elf-objdump -d build/firmware.elf
```

`readelf` should report an AArch64 ELF file. The linker script places the
startup code at `0x40000000`, and the program writes UART output to
`0x09000000`, matching the typical QEMU virt/Renode PL011 address used by this
project.

## Compile-only architecture check

To confirm that GCC is generating AArch64 code:

```bash
printf 'int main(void) { return 0; }\n' >/tmp/a53-check.c
aarch64-none-elf-gcc -mcpu=cortex-a53 -S /tmp/a53-check.c -o /tmp/a53-check.s
head -30 /tmp/a53-check.s
```

The assembly should contain AArch64 registers such as `x0`, `x1`, or `w0`,
rather than 32-bit ARM registers such as `r0`.

## Troubleshooting

### `aarch64-none-elf-gcc: command not found`

Check the installation directory:

```bash
find /opt -type f -name aarch64-none-elf-gcc 2>/dev/null
```

Temporarily add its `bin` directory:

```bash
export PATH="/opt/<toolchain-directory>/bin:$PATH"
```

Then add the same path to `~/.bashrc` or `~/.zshrc`.

### The compiler reports the wrong target

Run:

```bash
aarch64-none-elf-gcc -dumpmachine
```

The expected result is `aarch64-none-elf`. If it reports
`aarch64-linux-gnu`, you installed the Linux-targeting compiler instead.

### `ld` cannot find a runtime library

This bare-metal project intentionally uses:

```text
-ffreestanding -nostdlib -nostartfiles
```

It does not require glibc or a Linux sysroot. Make sure the linker command uses
the project's `bsp/aarch64/linker.ld`.

### Installing a newer release later

Download and extract the new Arm archive under `/opt`, then restart the shell.
The `find ... | sort -V | tail -n 1` command in this guide selects the newest
versioned toolchain directory. Confirm the active version with:

```bash
which aarch64-none-elf-gcc
aarch64-none-elf-gcc --version
```

