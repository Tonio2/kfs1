# MyOS

MyOS is a simple operating system kernel written in C and assembly for the x86 architecture. This project demonstrates basic kernel development, including setting up a multiboot-compliant kernel, initializing a terminal, and displaying text on the screen.

## Prerequisites

To build and run MyOS, you need the following tools installed:

- GCC cross-compiler for i386-elf
- GNU Binutils
- GRUB
- QEMU

## Building MyOS

1. Install the cross-compiler and other dependencies:

    ```sh
    sudo apt update
    sudo apt install -y build-essential bison flex libgmp3-dev libmpc-dev libmpfr-dev texinfo grub-pc-bin xorriso qemu-system-x86
    ```

2. Set up the cross-compiler:

    ```sh
    mkdir ~/cross-compiler
    cd ~/cross-compiler
    wget https://ftp.gnu.org/gnu/binutils/binutils-2.41.tar.gz
    tar -xf binutils-2.41.tar.gz
    wget https://ftp.gnu.org/gnu/gcc/gcc-13.2.0/gcc-13.2.0.tar.gz
    tar -xf gcc-13.2.0.tar.gz
    mkdir binutils-build gcc-build
    cd binutils-build
    ../binutils-2.41/configure --target=i386-elf --prefix=/usr/local/cross --with-sysroot --disable-nls --disable-werror
    make
    sudo make install
    cd ../gcc-build
    ../gcc-13.2.0/configure --target=i386-elf --prefix=/usr/local/cross --disable-nls --enable-languages=c --without-headers
    make all-gcc
    make all-target-libgcc
    sudo make install-gcc
    sudo make install-target-libgcc
    echo 'export PATH=/usr/local/cross/bin:$PATH' >> ~/.zshrc
    source ~/.zshrc
    ```

3. Build the kernel:

    ```sh
    make
    ```

4. Create a bootable ISO:

    ```sh
    make build-iso
    ```

## Running MyOS

You can run MyOS using QEMU:

```sh
make run
```

or run the ISO image:

```sh
make run-iso
```

## Project Structure

- [`src`](src/): Contains the source code for the kernel
    - [`boot.s`](src/boot.s): Assembly code for the bootloader
    - [`kernel.c`](src/kernel.c): C code for the kernel
    - [`linker.ld`](src/linker.ld): Linker script for the kernel
- [`Makefile`](Makefile): Build script for the project
- [`grub.cfg`](grub.cfg): GRUB configuration file for booting the kernel