SOURCES	= boot.o $(patsubst %.c,%.o,$(wildcard *.c)) interrupt.o table.o
CFLAGS	= -m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector -Iheader/ -g -Werror
LDFLAGS	= -Tlink.ld -melf_i386
ASFLAGS	= -felf
CC	= i686-elf-gcc
LD	= i686-elf-ld

all: $(SOURCES) link

clean:
	-rm *.o
	-rm -rf build/*

link:
	$(LD) $(LDFLAGS) -obuild/kernel $(SOURCES) 

.s.o:
	nasm $(ASFLAGS) $<

binaries:
	cd userlaand && make
