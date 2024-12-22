# Install cross compiler
```
mkdir kf1
cd kf1
gcc --version
ld --version
gcc -dumpmachine
sudo apt updatesudo apt install -y build-essential bison flex libgmp3-dev libmpc-dev libmpfr-dev texinfo
mkdir ~/cross-compiler
cd ~/cross-compiler
wget https://ftp.gnu.org/gnu/binutils/binutils-2.41.tar.gz
tar -xf binutils-2.41.tar.gz
wget https://ftp.gnu.org/gnu/gcc/gcc-13.2.0/gcc-13.2.0.tar.gz
tar -xf gcc-13.2.0.tar.gz
mkdir binutils-build gcc-build
mkdir /usr/local/cross
sudo mkdir /usr/local/cross
ls /usr/local
cd ~/cross-compiler/binutils-build
../binutils-2.41/configure --target=i386-elf --prefix=/usr/local/cross --with-sysroot --disable-nls --disable-werror
make
sudo make install
cd ~/cross-compiler/gcc-build
../gcc-13.2.0/configure --target=i386-elf --prefix=/usr/local/cross --disable-nls --enable-languages=c --without-headers
make all-gcc
make all-target-libgcc
sudo make install-gcc
sudo make install-target-libgcc
echo 'export PATH=/usr/local/cross/bin:$PATH' >> ~/.zshrc
cat ~/.zshrc
source ~/.zshrc
i386-elf-gcc --version
cd projects/kf1
i386-elf-gcc -c test.c -o test.o
rm test.o
```

# Does not work
```
i386-elf-gcc boot.s -o boot.o
nasm -f elf32 boot.s -o boot.o
```

# Create myos.bin
```
i386-elf-gcc -ffreestanding -c boot.s -o boot.o
i386-elf-gcc -c kernel.c -o kernel.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
i386-elf-gcc -T linker.ld -o myos.bin -ffreestanding -O2 -nostdlib boot.o kernel.o -lgcc
grub-file --is-x86-multiboot myos.bin
echo $?
```

# Create iso file
```
mkdir -p isodir/boot/grub
cp myos.bin isodir/boot/myos.bin
cp grub.cfg isodir/boot/grub/grub.cfg
grub-mkrescue -o myos.iso isodir
sudo apt updatesudo apt install -y xorriso
grub-mkrescue -o myos.iso isodir
sudo apt install -y mtools
ls /etc/mtools.conf
cat /etc/mtools.conf
grub-mkrescue -o myos.iso isodir
qemu-system-i386 -cdrom myos.iso
```

# Remove snap dependencies
```
sudo snap remove qemu
sudo apt updatesudo apt install -y qemu-system-x86
qemu-system-i386 -cdrom myos.iso
which qemu-system-i386
sudo find / -name "qemu-system-i386" 2>/dev/null
/usr/bin/qemu-system-i386 -cdrom myos.iso
sudo apt purge snapd
sudo rm -rf /snap /var/snap /var/lib/snapd
sudo find / -name "snap" -type d 2>/dev/null
ldd /usr/bin/qemu-system-i386
sudo apt updatesudo apt install --reinstall qemu-system-x86
ldd /usr/bin/qemu-system-i386
qemu-system-i386 -cdrom myos.iso
```

# Still not working
```
qemu-system-i386 -cdrom myos.iso -boot d
```

## Adding boot et echo to grub.cfg
```
vim isodir/boot/grub/grub.cfg
grub-mkrescue -o myos.iso isodir
grub-file --is-x86-multiboot myos.bin
qemu-system-i386 -cdrom myos.iso -boot d
qemu-system-i386 -cdrom myos.iso -serial stdio
```

## Verify grub version
```
grub-install --version
```

## Why the ISO is not bootable
Even though your grub.cfg is present and you see grub-mkrescue in the Makefile, by default grub-mkrescue can produce an EFI‐only ISO if you only have the EFI GRUB packages installed. For a 32-bit OS you need the i386 BIOS version of GRUB (the “pc” platform) included in your system’s GRUB packages.

On many distros you need to install something like:

```
sudo apt-get install grub-pc-bin xorriso
```

so that grub-mkrescue has the needed i386-pc modules to produce a BIOS-bootable ISO. Otherwise you end up with an ISO that has no BIOS‐boot sector, and QEMU’s BIOS can’t see anything to boot from.
