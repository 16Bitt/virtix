SOURCES	= entry_point.o clock.o common.o monitor.o boot.o descriptor_tables.o interrupt.o isr.o table.o kheap.o fs.o vfs.o paging.o virtix_proc.o elf.o flat.o kthread.o virtix_page.o single.o userspace.o ata.o fat16.o str-util.o deepfat.o fd.o file.o
CFLAGS	= -m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector -Iheader/ -g -Werror
LDFLAGS	= -Tlink.ld -melf_i386
ASFLAGS	= -felf
CC	= gcc

all: $(SOURCES) link

clean:
	-rm *.o
	-rm -rf build/*

link:
	ld $(LDFLAGS) -obuild/kernel $(SOURCES) 

.s.o:
	nasm $(ASFLAGS) $<

binaries:
	cd userland && make
