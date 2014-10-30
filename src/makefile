SOURCES	= entry_point.o clock.o common.o monitor.o boot.o descriptor_tables.o interrupt.o isr.o table.o kheap.o fs.o stream.o vfs.o paging.o virtix_proc.o elf.o flat.o
CFLAGS	= -m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector -Iheader/ 
LDFLAGS	= -Tlink.ld -melf_i386
ASFLAGS	= -felf

all: $(SOURCES) link mkinitramfs

clean:
	-rm *.o build/kernel

link:
	ld $(LDFLAGS) -o build/kernel $(SOURCES) 

.s.o:
	nasm $(ASFLAGS) $<

mkinitramfs:
	cd test && make
	cp test/*.x initramfs/
	cd initramfs && make

run: all
	qemu-system-x86_64 -d cpu_reset,pcall -no-reboot -kernel build/kernel -initrd initramfs/initrd -monitor stdio
