SOURCES	= entry_point.o clock.o common.o monitor.o boot.o descriptor_tables.o interrupt.o isr.o table.o kheap.o fs.o stream.o vfs.o paging.o virtix_proc.o elf.o flat.o kthread.o virtix_page.o single.o userspace.o ata.o fat16.o str-util.o deepfat.o
CFLAGS	= -m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector -Iheader/ -g -Werror
LDFLAGS	= -Tlink.ld -melf_i386
ASFLAGS	= -felf
CC	= gcc

all: $(SOURCES) link mkinitramfs

clean:
	cd userland && make clean
	-rm *.o grub.iso
	-rm -rf build/*
	-rm *.log

link:
	ld $(LDFLAGS) -obuild/kernel $(SOURCES) 

.s.o:
	nasm $(ASFLAGS) $<

mkinitramfs: binaries
	cd initramfs && make

run: all
	qemu-system-x86_64 -kernel build/kernel -initrd initramfs/initrd

debug: all
	objcopy --only-keep-debug build/kernel build/ksym
	qemu-system-x86_64 -nographic -no-reboot -kernel build/kernel -initrd initramfs/initrd -s -S & sleep 3
	(echo target remote 127.0.0.1:1234; cat) | gdb -s build/ksym build/kernel
	-pkill "qemu*"

dimg: all
	mkdir -p build/boot
	mkdir -p build/boot/grub
	cp grub.cfg build/boot/grub/
	cp build/kernel build/boot/kernel
	cp initramfs/initrd build/boot/initrd
	-grub-mkrescue -o grub.iso build

bochs: dimg
	-bochs -q 2> bochs.log

binaries:
	cd userland && make
	cp userland/*.x initramfs/
