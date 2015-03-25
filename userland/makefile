CFLAGS	= -m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector -I../libc/include -g
LDFLAGS	= -Tlink.ld -melf_i386
CC	= gcc
SOURCES	= $(wildcard bin/*.c)
OUTPUTS	= $(SOURCES:.c=.o)
EXECS	= $(OUTPUTS:.o=)
LIBC	= ../libc/libc.a

all: binclean $(EXECS)
	
$(OUTPUTS): $(SOURCES)
	$(CC) $(CFLAGS) $(@:.o=.c) -c -o $@

$(EXECS): $(OUTPUTS)
	$(LD) $(LDFLAGS) $@.o $(LIBC) -o $@
	strip --strip-unneeded $@

disk:	
	if ! [ -a hdd.img ]; \
	then \
		dd if=/dev/zero of=hdd.img bs=512 count=419328; \
	fi
	/sbin/mkdosfs hdd.img
	bin/mkdf.sh . hdd.img

binclean:
	-rm $(OUTPUTS)
	-rm $(EXECS)

clean: binclean
	-rm *.img *.vdi
