CFLAGS	= -m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector -Iheader/ 
LDFLAGS	= -Tlink.ld -melf_i386
CC	= gcc
TARGET	= hello.x

all: disk

disk:	
	if ! [ -a hdd.img ]; \
	then \
		dd if=/dev/zero of=hdd.img bs=512 count=419328; \
	fi
	/sbin/mkdosfs hdd.img
	bin/mkdf.sh ./ hdd.img

clean:
	-rm *.o *.x *.img

libc-build:
	cd libc && make
