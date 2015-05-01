CFLAGS	= -m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector -I../libc/include -g
LDFLAGS	= -Tlink.ld -melf_i386
CC	= i686-elf-gcc
LD	= i686-elf-ld
SOURCES	= $(wildcard src/*.c)
OUTPUTS	= $(SOURCES:.c=.o)
EXECS	= $(OUTPUTS:.o=)
LIBC	= ../libc/libc.a

all: binclean $(EXECS) 
	
$(OUTPUTS): $(SOURCES)
	$(CC) $(CFLAGS) $(@:.o=.c) -c -o $@

$(EXECS): $(OUTPUTS)
	$(LD) $(LDFLAGS) $@.o $(LIBC) -o $@
	strip --strip-unneeded $@
	mv $@ bin/

disk:
	cp mkdf.sh bin
	if ! [ -e hdd.img ]; \
	then \
		dd if=/dev/zero of=hdd.img bs=512 count=819328; \
	fi
	/sbin/mkdosfs -F16 -f1 -R1 -a hdd.img
	bin/mkdf.sh . hdd.img

binclean:
	-rm $(OUTPUTS)
	-rm bin/*
	-rm *.x16

clean: binclean
	-rm *.img *.vdi

gfx.x16:
	nasm -fbin gfx.asm -o gfx.x16
