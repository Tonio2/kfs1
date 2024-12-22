MFLAGS      =   -fno-builtin -fno-exceptions -fno-stack-protector -nostdlib -nodefaultlibs
CFLAGS      =   -std=gnu99 -ffreestanding -O2 -Wall -Wextra $(MFLAGS)
ASFLAGS     =   -ffreestanding

OBJDIR      =   obj
OBJS        =   $(OBJDIR)/boot.o $(OBJDIR)/kernel.o

BOOT		=	src/boot.s
KERNEL		=	src/kernel.c
LINKER		=	src/linker.ld

all: $(OBJDIR) myos.bin

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(OBJDIR)/boot.o: $(BOOT)
	i386-elf-gcc $(ASFLAGS) -c $(BOOT) -o $(OBJDIR)/boot.o

$(OBJDIR)/kernel.o: $(KERNEL)
	i386-elf-gcc $(CFLAGS) -c $(KERNEL) -o $(OBJDIR)/kernel.o

myos.bin: $(OBJS) $(LINKER)
	@ld -m elf_i386 -T ${LINKER} -o myos.bin $(OBJS)

run:
	qemu-system-i386 -kernel myos.bin

build-iso:
	mkdir -p isodir/boot/grub
	cp myos.bin isodir/boot/myos.bin
	cp grub.cfg isodir/boot/grub/grub.cfg
	grub-mkrescue -o myos.iso isodir

run-iso: build-iso
	qemu-system-i386 -cdrom myos.iso

clean:
	rm -rf $(OBJDIR) myos.bin myos.iso isodir

re: clean all
