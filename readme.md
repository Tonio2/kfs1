# KFS1

KFS1 is a simple operating system kernel written in C and assembly for the x86 architecture. This project demonstrates basic kernel development, including setting up a multiboot-compliant kernel, initializing a terminal, and displaying text on the screen.

## Prerequisites

To build and run KFS1, you need the following tools installed:

- Docker
- QEMU

## Building KFS1

1. To build the container, run:
```sh
docker build cross_compiler -t cross_compiler_env
```

2. To run the container on windows powershell, run (--rm: kill container if it exists, -i: interactive mode, -t: allocate a terminal to the container):
```sh
docker run --rm -it -v "${pwd}:/root/cc" cross_compiler_env
```

For linux run:
```sh
docker run --rm -it -v "$(pwd):/root/cc" cross_compiler_env
```

Once inside the container, simply run make to compile the project.


## Running MyOS

Don't forget to add qemu folder to the path !

Outside of the container, if the kernel iso has been compiled, to launch it in qemu run:
```sh
qemu-system-i386 -cdrom .\kernel.iso
```

## Project Structure

- [`src`](src/): Contains the source code for the kernel
    - [`boot.s`](src/boot.s): Assembly code for the bootloader
    - [`kernel.c`](src/kernel.c): C code for the kernel
    - [`linker.ld`](src/linker.ld): Linker script for the kernel
- [`Makefile`](Makefile): Build script for the project
- [`grub.cfg`](grub.cfg): GRUB configuration file for booting the kernel