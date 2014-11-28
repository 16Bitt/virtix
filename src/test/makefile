SOURCES = hello.o
CFLAGS	= -m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector -Iheader/ 
LDFLAGS	= -Tlink.ld -melf_i386

all: build link

link:
	ld $(LDFLAGS) -o userland.x $(SOURCES)

build:
	gcc $(CFLAGS) -c hello.c -o hello.o
	strip --strip-unneeded hello.o
