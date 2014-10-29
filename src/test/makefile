SOURCES = userland.c
CFLAGS	= -m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector -Iheader/ 

all:
	nasm tester.asm -fbin -o userland.x
