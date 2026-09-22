# Installing gdb-multiarch
```bash
sudo apt install gdb-multiarch
```

# Installing ARM Toolchains

## 1. Install prerequisites

On Ubuntu or Debian:

```bash
sudo apt update
sudo apt install -y wget xz-utils tar make
```

## 2. Download the newest archive

The latest release is published on Arm's official download page:
  * <https://gitlab.arm.com/tooling/gnu-toolchains-for-arm>

For developing baremetal applications, we would want to install
  * aarch64-*none*-elf
  * arm-*none*-eabi

Copy the download URL of the desired version. As of this writing, the latest version is `15.3.rel1`
  * https://gitlab.arm.com/api/v4/projects/tooling%2Fgnu-toolchains-for-arm/packages/generic/gnu-toolchain/15.3.rel1/arm-gnu-toolchain-15.3.rel1-x86_64-aarch64-none-elf.tar.xz 
  * https://gitlab.arm.com/api/v4/projects/tooling%2Fgnu-toolchains-for-arm/packages/generic/gnu-toolchain/15.3.rel1/arm-gnu-toolchain-15.3.rel1-x86_64-arm-none-eabi.tar.xz

In the terminal:
```bash
cd ~/Downloads
wget https://gitlab.arm.com/api/v4/projects/tooling%2Fgnu-toolchains-for-arm/packages/generic/gnu-toolchain/15.3.rel1/arm-gnu-toolchain-15.3.rel1-x86_64-aarch64-none-elf.tar.xz 
wget https://gitlab.arm.com/api/v4/projects/tooling%2Fgnu-toolchains-for-arm/packages/generic/gnu-toolchain/15.3.rel1/arm-gnu-toolchain-15.3.rel1-x86_64-arm-none-eabi.tar.xz
```

Verify that the archive was downloaded:

```bash
ls -lh arm-gnu-toolchain-*-x86_64-aarch64-none-elf.tar.xz arm-gnu-toolchain-*-x86_64-arm-none-eabi.tar.xz
```


## 3. Extract downloaded archives to `/opt`

```bash
cd ~/Downloads
sudo mkdir -p /opt
sudo tar -xJf arm-gnu-toolchain-*-x86_64-aarch64-none-elf.tar.xz -C /opt
sudo tar -xJf arm-gnu-toolchain-*-x86_64-arm-none-eabi.tar.xz -C /opt
```

## 4. Add the compiler to `PATH`

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
which aarch64-none-elf-gcc
which arm-none-eabi
```
